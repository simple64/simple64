#!/usr/bin/env bash

set -e

TARGET = simple64

all: $(TARGET)

UNAME=$(uname -s)
if [[ $UNAME == *"MINGW"* ]]; then
  suffix=".dll"
  if [[ $UNAME == *"MINGW64"* ]]; then
    mingw_prefix="mingw64"
  else
    mingw_prefix="mingw32"
else
  suffix=".so"
fi

install_dir=$PWD/simple64
mkdir -p $install_dir
base_dir=$PWD

export OPTFLAGS="-O3 -flto -march=x86-64-v3"

cd $base_dir/mupen64plus-core/projects/unix
make NETPLAY=1 NO_ASM=1 OSD=0 V=1 -j4 all
cp -P $base_dir/mupen64plus-core/projects/unix/*$suffix* $install_dir
cp $base_dir/mupen64plus-core/data/* $install_dir

cd $base_dir/simple64-input-raphnetraw/projects/unix
make V=1 -j4 all
cp $base_dir/simple64-input-raphnetraw/projects/unix/*$suffix $install_dir

mkdir -p $base_dir/simple64-input-qt/build
cd $base_dir/simple64-input-qt/build
qmake6 ../simple64-input-qt.pro
make -j4
if [[ $UNAME == *"MINGW"* ]]; then
  cp $base_dir/simple64-input-qt/build/release/simple64-input-qt.dll $install_dir
else
  cp $base_dir/simple64-input-qt/build/libsimple64-input-qt$suffix $install_dir/simple64-input-qt$suffix
fi

cd $base_dir/simple64-audio-sdl2/projects/unix
make V=1 -j4 all
cp $base_dir/simple64-audio-sdl2/projects/unix/*$suffix $install_dir

cd $base_dir
GUI_DIRECTORY=$base_dir/simple64-gui
rev=\"`git rev-parse HEAD`\"
lastrev=$(head -n 1 $GUI_DIRECTORY/version.h | awk -F'GUI_VERSION ' {'print $2'})

echo current revision $rev
echo last build revision $lastrev

if [ "$lastrev" != "$rev" ]
then
   echo "#define GUI_VERSION $rev" > $GUI_DIRECTORY/version.h
fi

mkdir -p $base_dir/simple64-gui/build
cd $base_dir/simple64-gui/build
qmake6 ../simple64-gui.pro
make -j4
if [[ $UNAME == *"MINGW"* ]]; then
  cp $base_dir/simple64-gui/build/release/simple64-gui.exe $install_dir
else
  cp $base_dir/simple64-gui/build/simple64-gui $install_dir
fi

mkdir -p $base_dir/parallel-rsp/build
cd $base_dir/parallel-rsp/build
if [[ $UNAME == *"MINGW"* ]]; then
  cmake -G "MSYS Makefiles" -DCMAKE_BUILD_TYPE=Release ..
else
  cmake -DCMAKE_BUILD_TYPE=Release ..
fi
VERBOSE=1 cmake --build .
cp simple64-rsp-parallel.* $install_dir

mkdir -p $base_dir/parallel-rdp-standalone/build
cd $base_dir/parallel-rdp-standalone/build
if [[ $UNAME == *"MINGW"* ]]; then
  cmake -G "MSYS Makefiles" -DCMAKE_BUILD_TYPE=Release ..
else
  cmake -DCMAKE_BUILD_TYPE=Release ..
fi
VERBOSE=1 cmake --build .
cp simple64-video-parallel.* $install_dir

if [[ $UNAME == *"MINGW"* ]]; then
  cd $install_dir
  windeployqt-qt6.exe --no-translations simple64-gui.exe

  if [[ $UNAME == *"MINGW64"* ]]; then
    my_os=win64
    cp /$mingw_prefix/bin/libgcc_s_seh-1.dll $install_dir
  else
    my_os=win32
    cp /$mingw_prefix/bin/libgcc_s_dw2-1.dll $install_dir
  fi
# WINEDEBUG=+loaddll wine ./simple64-gui.exe 2> out.txt
# cat out.txt | grep found
  cp -v /$mingw_prefix/bin/libwinpthread-1.dll $install_dir
  cp -v /$mingw_prefix/bin/libstdc++-6.dll $install_dir
  cp -v /$mingw_prefix/bin/libdouble-conversion.dll $install_dir
  cp -v /$mingw_prefix/bin/zlib1.dll $install_dir
  cp -v /$mingw_prefix/bin/libicuin71.dll $install_dir
  cp -v /$mingw_prefix/bin/libicuuc71.dll $install_dir
  cp -v /$mingw_prefix/bin/libicudt71.dll $install_dir
  cp -v /$mingw_prefix/bin/libbrotlidec.dll $install_dir
  cp -v /$mingw_prefix/bin/libbrotlicommon.dll $install_dir
  cp -v /$mingw_prefix/bin/libpcre2-16-0.dll $install_dir
  cp -v /$mingw_prefix/bin/libharfbuzz-0.dll $install_dir
  cp -v /$mingw_prefix/bin/libb2-1.dll $install_dir
  cp -v /$mingw_prefix/bin/libmd4c.dll $install_dir
  cp -v /$mingw_prefix/bin/libpng16-16.dll $install_dir
  cp -v /$mingw_prefix/bin/libfreetype-6.dll $install_dir
  cp -v /$mingw_prefix/bin/libglib-2.0-0.dll $install_dir
  cp -v /$mingw_prefix/bin/SDL2.dll $install_dir
  cp -v /$mingw_prefix/bin/SDL2_net.dll $install_dir
  cp -v /$mingw_prefix/bin/libbz2-1.dll $install_dir
  cp -v /$mingw_prefix/bin/libgraphite2.dll $install_dir
  cp -v /$mingw_prefix/bin/libintl-8.dll $install_dir
  cp -v /$mingw_prefix/bin/libpcre-1.dll $install_dir
  cp -v /$mingw_prefix/bin/libiconv-2.dll $install_dir
  cp -v /$mingw_prefix/bin/libhidapi-0.dll $install_dir
  cp -v $base_dir/7za.exe $install_dir
  cp -v $base_dir/simple64-gui/discord/discord_game_sdk.dll $install_dir
  cp -v $base_dir/simple64-input-qt/vosk/vosk.dll $install_dir
else
  if [[ $HOST_CPU == "i686" ]]; then
    my_os=linux32
  else
    my_os=linux64
  fi
  cp $base_dir/simple64-gui/discord/libdiscord_game_sdk.so $install_dir
  cp $base_dir/simple64-input-qt/vosk/libvosk.so $install_dir
fi
