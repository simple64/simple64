#!/usr/bin/env bash

set -e

contents=simple64/simple64-gui.app/Contents

# Get files in correct directories
mv simple64/font.ttf $contents/MacOS
mv simple64/libmupen64plus.dylib $contents/MacOS
mv simple64/simple64-audio-sdl2.dylib $contents/MacOS
mv simple64/simple64-input-qt.dylib $contents/MacOS
mv simple64/simple64-input-raphnetraw.dylib $contents/MacOS
mv simple64/simple64-rsp-parallel.dylib $contents/MacOS
mv simple64/simple64-video-parallel.dylib $contents/MacOS
mv simple64/mupen64plus.ini $contents/MacOS
mv simple64/mupencheat.txt $contents/MacOS
mv simple64/discord_game_sdk.dylib $contents/MacOS
mv simple64/libvosk.dylib $contents/MacOS

# SDL
cp /usr/local/opt/sdl2_net/lib/libSDL2-2.0.0.dylib $contents/Frameworks
otool -L $contents/Frameworks/libSDL2_net-2.0.0.dylib

cp /usr/local/opt/sdl2_net/lib/libSDL2_net-2.0.0.dylib $contents/Frameworks
install_name_tool -change /usr/local/opt/sdl2/lib/libSDL2-2.0.0.dylib @executable_path/../Frameworks/libSDL2-2.0.0.dylib $contents/Frameworks/libSDL2_net-2.0.0.dylib

otool -L $contents/Frameworks/libSDL2_net-2.0.0.dylib

# libpng
cp /usr/local/opt/libpng/lib/libpng16.16.dylib $contents/Frameworks

otool -L $contents/Frameworks/libpng16.16.dylib

# hidapi
cp /usr/local/opt/hidapi/lib/libhidapi.0.dylib $contents/Frameworks

otool -L $contents/Frameworks/libhidapi.0.dylib

# freetype
cp /usr/local/opt/freetype/lib/libfreetype.6.dylib $contents/Frameworks

install_name_tool -change /usr/local/opt/libpng/lib/libpng16.16.dylib @executable_path/../Frameworks/libpng16.16.dylib $contents/Frameworks/libfreetype.6.dylib

otool -L $contents/Frameworks/libfreetype.6.dylib

# simple64-gui
# nothing to link
otool -L $contents/MacOS/simple64-gui

# libmupen64plus.dylib
install_name_tool -change /usr/local/opt/sdl2/lib/libSDL2-2.0.0.dylib @executable_path/../Frameworks/libSDL2-2.0.0.dylib $contents/MacOS/libmupen64plus.dylib
install_name_tool -change /usr/local/opt/sdl2_net/lib/libSDL2_net-2.0.0.dylib @executable_path/../Frameworks/libSDL2_net-2.0.0.dylib $contents/MacOS/libmupen64plus.dylib
install_name_tool -change /usr/local/opt/libpng/lib/libpng16.16.dylib @executable_path/../Frameworks/libpng16.16.dylib $contents/MacOS/libmupen64plus.dylib

otool -L $contents/MacOS/libmupen64plus.dylib

# simple64-audio-sdl2.dylib
install_name_tool -change /usr/local/opt/sdl2/lib/libSDL2-2.0.0.dylib @executable_path/../Frameworks/libSDL2-2.0.0.dylib $contents/MacOS/simple64-audio-sdl2.dylib

otool -L $contents/MacOS/simple64-audio-sdl2.dylib

# simple64-input-qt.dylib
install_name_tool -change /usr/local/opt/sdl2/lib/libSDL2-2.0.0.dylib @executable_path/../Frameworks/libSDL2-2.0.0.dylib $contents/MacOS/simple64-input-qt.dylib
install_name_tool -change /usr/local/opt/qt/lib/QtWidgets.framework/Versions/5/QtWidgets @executable_path/../Frameworks/QtWidgets.framework/Versions/5/QtWidgets $contents/MacOS/simple64-input-qt.dylib
install_name_tool -change /usr/local/opt/qt/lib/QtGui.framework/Versions/5/QtGui @executable_path/../Frameworks/QtGui.framework/Versions/5/QtGui $contents/MacOS/simple64-input-qt.dylib
install_name_tool -change /usr/local/opt/qt/lib/QtCore.framework/Versions/5/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/5/QtCore $contents/MacOS/simple64-input-qt.dylib

otool -L $contents/MacOS/simple64-input-qt.dylib

# simple64-input-raphnetraw.dylib
install_name_tool -change /usr/local/opt/hidapi/lib/libhidapi.0.dylib @executable_path/../Frameworks/libhidapi.0.dylib $contents/MacOS/simple64-input-raphnetraw.dylib

otool -L $contents/MacOS/simple64-input-raphnetraw.dylib

# simple64-rsp-parallel.dylib

otool -L $contents/MacOS/simple64-rsp-parallel.dylib

# simple64-video-parallel.dylib

otool -L $contents/MacOS/simple64-video-parallel.dylib

# Time to create the DMG

# 1. Create symlink to Applications folder
ln -s /Applications simple64/Applications

# 2. Create DMG
HASH=$(git rev-parse --short HEAD)
hdiutil create -volname simple64 -srcfolder simple64 -ov -format UDZO "simple64/simple64-osx-$HASH.dmg"
