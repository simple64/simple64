#!/usr/bin/env bash

UNAME=$(uname -s)
if [[ $UNAME == *"MINGW"* ]]; then
  suffix=".dll"
  if [[ $UNAME == *"MINGW64"* ]]; then
    mingw_prefix="mingw64"
  else
    mingw_prefix="mingw32"
  fi
elif [[ $UNAME == "Darwin" ]]; then
  suffix=".dylib"
else
  suffix=".so"
fi

#if [[ $HOST_CPU == "i686" ]]; then
#  export NEW_DYNAREC=1
#fi

install_dir=$PWD/mupen64plus
mkdir $install_dir
base_dir=$PWD

cd $base_dir/mupen64plus-core/projects/unix
make -j4 all
cp -P $base_dir/mupen64plus-core/projects/unix/*$suffix* $install_dir
cp $base_dir/mupen64plus-core/data/* $install_dir

cd $base_dir/mupen64plus-rsp-hle/projects/unix
make -j4 all
cp $base_dir/mupen64plus-rsp-hle/projects/unix/*$suffix $install_dir

cd $base_dir/mupen64plus-input-sdl/projects/unix
make -j4 all
cp $base_dir/mupen64plus-input-sdl/projects/unix/*$suffix $install_dir
cp $base_dir/mupen64plus-input-sdl/data/* $install_dir

cd $base_dir/mupen64plus-audio-sdl2/projects/unix
make -j4 all
cp $base_dir/mupen64plus-audio-sdl2/projects/unix/*$suffix $install_dir

mkdir -p $base_dir/mupen64plus-gui/build
cd $base_dir/mupen64plus-gui/build
if [[ $UNAME == *"MINGW"* ]]; then
  /$mingw_prefix/qt5-static/bin/qmake ../mupen64plus-gui.pro
  make -j4 release
  cp $base_dir/mupen64plus-gui/build/release/mupen64plus-gui.exe $install_dir
elif [[ $UNAME == "Darwin" ]]; then
  /usr/local/Cellar/qt5/*/bin/qmake ../mupen64plus-gui.pro
  make -j4
  cp -Rp $base_dir/mupen64plus-gui/build/mupen64plus-gui.app $install_dir
else
  qmake ../mupen64plus-gui.pro
  make -j4
  cp $base_dir/mupen64plus-gui/build/mupen64plus-gui $install_dir
fi

cd $base_dir/GLideN64/src
./getRevision.sh

mkdir -p $base_dir/GLideN64/src/GLideNUI/build
cd $base_dir/GLideN64/src/GLideNUI/build
qmake ../GLideNUI.pro
make -j4

cd $base_dir/GLideN64/projects/cmake
if [[ $UNAME == *"MINGW"* ]]; then
  sed -i 's/check_ipo_supported(RESULT result)//g' ../../src/CMakeLists.txt
  cmake -G "MSYS Makefiles" -DVEC4_OPT=On -DCRC_OPT=On -DMUPENPLUSAPI=On ../../src/
else
  rm -rf ../../src/GLideNHQ/inc
  cmake -DUSE_SYSTEM_LIBS=On -DVEC4_OPT=On -DCRC_OPT=On -DMUPENPLUSAPI=On ../../src/
fi
make -j4

if [[ $UNAME == *"MINGW"* ]]; then
  cp mupen64plus-video-GLideN64$suffix $install_dir
else
  cp plugin/Release/mupen64plus-video-GLideN64$suffix $install_dir
fi
cp $base_dir/GLideN64/ini/GLideN64.custom.ini $install_dir

cd $base_dir

if [[ $UNAME == *"MINGW"* ]]; then
  if [[ $UNAME == *"MINGW64"* ]]; then
    my_os=win64
    cp /$mingw_prefix/bin/libgcc_s_seh-1.dll $install_dir
  else
    my_os=win32
    cp /$mingw_prefix/bin/libgcc_s_dw2-1.dll $install_dir
  fi
  cp /$mingw_prefix/bin/libwinpthread-1.dll $install_dir
  cp /$mingw_prefix/bin/SDL2.dll $install_dir
  cp /$mingw_prefix/bin/libpng16-16.dll $install_dir
  cp /$mingw_prefix/bin/libglib-2.0-0.dll $install_dir
  cp /$mingw_prefix/bin/libstdc++-6.dll $install_dir
  cp /$mingw_prefix/bin/zlib1.dll $install_dir
  cp /$mingw_prefix/bin/libintl-8.dll $install_dir
  cp /$mingw_prefix/bin/libpcre-1.dll $install_dir
  cp /$mingw_prefix/bin/libiconv-2.dll $install_dir
  cp /$mingw_prefix/bin/libharfbuzz-0.dll $install_dir
  cp /$mingw_prefix/bin/libgraphite2.dll $install_dir
  cp /$mingw_prefix/bin/libfreetype-6.dll $install_dir
  cp /$mingw_prefix/bin/libbz2-1.dll $install_dir
  cp /$mingw_prefix/bin/libminizip-1.dll $install_dir
  cp /$mingw_prefix/bin/libsamplerate-0.dll $install_dir
  cp /$mingw_prefix/bin/libjasper-4.dll $install_dir
  cp /$mingw_prefix/bin/libjpeg-8.dll $install_dir
  cp /$mingw_prefix/bin/libicudt62.dll $install_dir
  cp /$mingw_prefix/bin/libicuin62.dll $install_dir
  cp /$mingw_prefix/bin/libicuuc62.dll $install_dir
  cp $base_dir/7za.exe $install_dir
elif [[ $UNAME == "Darwin" ]]; then
  my_os=macos

  find mupen64plus -type f -depth 1 \
    -exec mv {} mupen64plus/mupen64plus-gui.app/Contents/MacOS/ \;

  cd $install_dir
  /usr/local/Cellar/qt5/*/bin/macdeployqt mupen64plus-gui.app

  for P in $(find mupen64plus-gui.app -type f -name 'Qt*'; find mupen64plus-gui.app -type f -name '*.dylib'); do
    for P1 in $(otool -L $P | awk '/\/usr\/local\/Cellar/ {print $1}'); do
      PATHNAME=$(echo $P1 | awk '{sub(/(\/Qt.+\.framework|[^\/]*\.dylib).*/, ""); print}')
      PSLASH1=$(echo $P1 | sed "s,$PATHNAME,@executable_path/../Frameworks,g")
      install_name_tool -change $P1 $PSLASH1 $P
    done
  done

  cd $base_dir
else
  if [[ $HOST_CPU == "i686" ]]; then
    my_os=linux32
  else
    my_os=linux64
  fi
fi

if [[ $1 == "aws" ]]; then
  rm $base_dir/*.zip
  DISTRO=$(lsb_release -i | cut -d: -f2 | sed s/'^\t'//)
  zip -r mupen64plus-GLideN64-$my_os.zip mupen64plus
  aws s3 cp mupen64plus-GLideN64-*.zip s3://m64p/m64p/ --acl public-read
fi
