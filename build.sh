#!/usr/bin/env bash

set -e
set -o pipefail

RELEASE_TYPE="Release"

UNAME=$(uname -s)
PLATFORM=$(uname -m)
if [[ ${UNAME} == *"MINGW64"* ]]; then
  suffix=".dll"
elif [[ ${UNAME} == *"Darwin"* ]]; then
  suffix=".dylib"
  qt_version=$(ls /usr/local/Cellar/qt)
  export CXXFLAGS='-stdlib=libc++'
  export LDFLAGS='-mmacosx-version-min=11.0'
else
  suffix=".so"
fi

install_dir=${PWD}/simple64
mkdir -p "${install_dir}"
base_dir=${PWD}

mkdir -p "${base_dir}/mupen64plus-core/build"
cd "${base_dir}/mupen64plus-core/build"
cmake -G Ninja -DCMAKE_BUILD_TYPE="${RELEASE_TYPE}" ..
VERBOSE=1 cmake --build .
cp ./*mupen64plus"${suffix}" "${install_dir}"
cp "${base_dir}/mupen64plus-core/data/"* "${install_dir}"

mkdir -p "${base_dir}/simple64-input-raphnetraw/build"
cd "${base_dir}/simple64-input-raphnetraw/build"
cmake -G Ninja -DCMAKE_BUILD_TYPE="${RELEASE_TYPE}" ..
VERBOSE=1 cmake --build .
cp simple64-input-raphnetraw.* "${install_dir}"

mkdir -p "${base_dir}/simple64-input-qt/build"
cd "${base_dir}/simple64-input-qt/build"
cmake -G Ninja -DCMAKE_BUILD_TYPE="${RELEASE_TYPE}" ..
VERBOSE=1 cmake --build .
cp simple64-input-qt.* "${install_dir}"

mkdir -p "${base_dir}/simple64-audio-sdl2/build"
cd "${base_dir}/simple64-audio-sdl2/build"
cmake -G Ninja -DCMAKE_BUILD_TYPE="${RELEASE_TYPE}" ..
VERBOSE=1 cmake --build .
cp simple64-audio-sdl2.* "${install_dir}"

cd "${base_dir}"
GUI_VERSION_FILE=${base_dir}/simple64-gui/version.h
rev=\"$(git rev-parse HEAD)\"
if [[ -f ${GUI_VERSION_FILE} ]]; then
  lastrev=$(head -n 1 "${GUI_VERSION_FILE}" | cut -d " " -f 3)
else
  lastrev=""
fi

echo current revision "${rev}"
echo last build revision "${lastrev}"

if [[ "${lastrev}" != "${rev}" ]]; then
  echo "#define GUI_VERSION ${rev}" >"${GUI_VERSION_FILE}"
fi

mkdir -p "${base_dir}/simple64-gui/build"
cd "${base_dir}/simple64-gui/build"
cmake -G Ninja -DCMAKE_BUILD_TYPE="${RELEASE_TYPE}" ..
VERBOSE=1 cmake --build .

if [[ ${UNAME} == *"Darwin"* ]]; then
  "/usr/local/Cellar/qt/${qt_version}/bin/macdeployqt" "${base_dir}/simple64-gui/build/simple64-gui.app"
  cp -a "${base_dir}/simple64-gui/build/simple64-gui.app" "${install_dir}"
else
  cp "${base_dir}/simple64-gui/build/simple64-gui" "${install_dir}"
fi

mkdir -p "${base_dir}/parallel-rsp/build"
cd "${base_dir}/parallel-rsp/build"
cmake -G Ninja -DCMAKE_BUILD_TYPE="${RELEASE_TYPE}" ..
VERBOSE=1 cmake --build .
cp simple64-rsp-parallel.* "${install_dir}"

mkdir -p "${base_dir}/parallel-rdp-standalone/build"
cd "${base_dir}/parallel-rdp-standalone/build"
cmake -G Ninja -DCMAKE_BUILD_TYPE="${RELEASE_TYPE}" ..
VERBOSE=1 cmake --build .
cp simple64-video-parallel.* "${install_dir}"

if [[ ${UNAME} == *"MINGW64"* ]]; then
  cd "${install_dir}"
  windeployqt-qt6.exe --no-translations simple64-gui.exe
  my_os=win64

  # shellcheck disable=SC2154
  cp -v "${MSYSTEM_PREFIX}/bin/libgcc_s_seh-1.dll" "${install_dir}"
  cp -v "${MSYSTEM_PREFIX}/bin/libwinpthread-1.dll" "${install_dir}"
  cp -v "${MSYSTEM_PREFIX}/bin/libstdc++-6.dll" "${install_dir}"
  cp -v "${MSYSTEM_PREFIX}/bin/libdouble-conversion.dll" "${install_dir}"
  cp -v "${MSYSTEM_PREFIX}/bin/zlib1.dll" "${install_dir}"
  cp -v "${MSYSTEM_PREFIX}/bin/libicuin73.dll" "${install_dir}"
  cp -v "${MSYSTEM_PREFIX}/bin/libicuuc73.dll" "${install_dir}"
  cp -v "${MSYSTEM_PREFIX}/bin/libicudt73.dll" "${install_dir}"
  cp -v "${MSYSTEM_PREFIX}/bin/libbrotlidec.dll" "${install_dir}"
  cp -v "${MSYSTEM_PREFIX}/bin/libbrotlicommon.dll" "${install_dir}"
  cp -v "${MSYSTEM_PREFIX}/bin/libpcre2-16-0.dll" "${install_dir}"
  cp -v "${MSYSTEM_PREFIX}/bin/libpcre2-8-0.dll" "${install_dir}"
  cp -v "${MSYSTEM_PREFIX}/bin/libharfbuzz-0.dll" "${install_dir}"
  cp -v "${MSYSTEM_PREFIX}/bin/libb2-1.dll" "${install_dir}"
  cp -v "${MSYSTEM_PREFIX}/bin/libmd4c.dll" "${install_dir}"
  cp -v "${MSYSTEM_PREFIX}/bin/libpng16-16.dll" "${install_dir}"
  cp -v "${MSYSTEM_PREFIX}/bin/libfreetype-6.dll" "${install_dir}"
  cp -v "${MSYSTEM_PREFIX}/bin/libglib-2.0-0.dll" "${install_dir}"
  cp -v "${MSYSTEM_PREFIX}/bin/SDL2.dll" "${install_dir}"
  cp -v "${MSYSTEM_PREFIX}/bin/SDL2_net.dll" "${install_dir}"
  cp -v "${MSYSTEM_PREFIX}/bin/libbz2-1.dll" "${install_dir}"
  cp -v "${MSYSTEM_PREFIX}/bin/libgraphite2.dll" "${install_dir}"
  cp -v "${MSYSTEM_PREFIX}/bin/libintl-8.dll" "${install_dir}"
  cp -v "${MSYSTEM_PREFIX}/bin/libiconv-2.dll" "${install_dir}"
  cp -v "${MSYSTEM_PREFIX}/bin/libhidapi-0.dll" "${install_dir}"
  cp -v "${MSYSTEM_PREFIX}/bin/libcrypto-3-x64.dll" "${install_dir}" # used by Qt at runtime
  cp -v "${MSYSTEM_PREFIX}/bin/libssl-3-x64.dll" "${install_dir}" # used by Qt at runtime
  cp -v "${base_dir}/7za.exe" "${install_dir}"
  cp -v "${base_dir}/simple64-gui/discord/discord_game_sdk.dll" "${install_dir}"
  cp -v "${base_dir}/simple64-input-qt/vosk/x86_64/vosk.dll" "${install_dir}"
elif [[ ${UNAME} == *"Darwin"* ]]; then
  cp "${base_dir}/simple64-gui/discord/discord_game_sdk.dylib" "${install_dir}"
  cd "${base_dir}"
  sh ./link-mac.sh
else
  if [[ "${PLATFORM}" == "aarch64" ]]; then
    my_os=linux_aarch64
    cp "${base_dir}/simple64-input-qt/vosk/aarch64/libvosk.so" "${install_dir}"
  else
    my_os=linux_x86_64
    cp "${base_dir}/simple64-gui/discord/libdiscord_game_sdk.so" "${install_dir}"
    cp "${base_dir}/simple64-input-qt/vosk/x86_64/libvosk.so" "${install_dir}"
  fi
fi

if [[ "$1" != "nozip" ]]; then
  if [[ ${UNAME} != *"Darwin"* ]]; then
    cd "${base_dir}"
    rm -f "${base_dir}/"*.zip
    HASH=$(git rev-parse --short HEAD)
    zip --symlinks -r "simple64-${my_os}-${HASH}.zip" simple64
  fi
fi
