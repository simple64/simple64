#!/usr/bin/env bash

contents=mupen64plus/mupen64plus-gui.app/Contents

# Get files in correct directories
mv mupen64plus/font.ttf $contents/MacOS
mv mupen64plus/libmupen64plus.dylib $contents/MacOS
mv mupen64plus/GLideN64.custom.ini $contents/MacOS
mv mupen64plus/mupen64plus-audio-sdl2.dylib $contents/MacOS
mv mupen64plus/mupen64plus-input-qt.dylib $contents/MacOS
mv mupen64plus/mupen64plus-input-raphnetraw.dylib $contents/MacOS
mv mupen64plus/mupen64plus-rsp-hle.dylib $contents/MacOS
mv mupen64plus/mupen64plus-video-GLideN64.dylib $contents/MacOS
mv mupen64plus/mupen64plus.ini $contents/MacOS
mv mupen64plus/mupencheat.txt $contents/MacOS

# SDL
cp /usr/local/opt/sdl2_net/lib/libSDL2-2.0.0.dylib $contents/Frameworks
otool -L $contents/Frameworks/libSDL2_net-2.0.0.dylib

cp /usr/local/opt/sdl2_net/lib/libSDL2_net-2.0.0.dylib $contents/Frameworks
install_name_tool -change /usr/local/opt/sdl2/lib/libSDL2-2.0.0.dylib @executable_path/../Frameworks/libSDL2-2.0.0.dylib $contents/Frameworks/libSDL2_net-2.0.0.dylib

otool -L $contents/Frameworks/libSDL2_net-2.0.0.dylib

# libpng
cp /usr/local/opt/libpng/lib/libpng16.16.dylib $contents/Frameworks

otool -L $contents/Frameworks/libpng16.16.dylib

# libsamplerate
cp /usr/local/opt/libsamplerate/lib/libsamplerate.0.dylib $contents/Frameworks

otool -L $contents/Frameworks/libsamplerate.0.dylib

# hidapi
cp /usr/local/opt/hidapi/lib/libhidapi.0.dylib $contents/Frameworks

otool -L $contents/Frameworks/libhidapi.0.dylib

# freetype
cp /usr/local/opt/freetype/lib/libfreetype.6.dylib $contents/Frameworks

install_name_tool -change /usr/local/opt/libpng/lib/libpng16.16.dylib @executable_path/../Frameworks/libpng16.16.dylib $contents/Frameworks/libfreetype.6.dylib

otool -L $contents/Frameworks/libfreetype.6.dylib

# mupen64plus-gui
# nothing to link
otool -L $contents/MacOS/mupen64plus-gui

# libmupen64plus.dylib
install_name_tool -change /usr/local/opt/sdl2/lib/libSDL2-2.0.0.dylib @executable_path/../Frameworks/libSDL2-2.0.0.dylib $contents/MacOS/libmupen64plus.dylib
install_name_tool -change /usr/local/opt/sdl2_net/lib/libSDL2_net-2.0.0.dylib @executable_path/../Frameworks/libSDL2_net-2.0.0.dylib $contents/MacOS/libmupen64plus.dylib
install_name_tool -change /usr/local/opt/libpng/lib/libpng16.16.dylib @executable_path/../Frameworks/libpng16.16.dylib $contents/MacOS/libmupen64plus.dylib

otool -L $contents/MacOS/libmupen64plus.dylib

# mupen64plus-audio-sdl2.dylib
install_name_tool -change /usr/local/opt/sdl2/lib/libSDL2-2.0.0.dylib @executable_path/../Frameworks/libSDL2-2.0.0.dylib $contents/MacOS/mupen64plus-audio-sdl2.dylib
install_name_tool -change /usr/local/opt/libsamplerate/lib/libsamplerate.0.dylib @executable_path/../Frameworks/libsamplerate.0.dylib $contents/MacOS/mupen64plus-audio-sdl2.dylib

otool -L $contents/MacOS/mupen64plus-audio-sdl2.dylib

# mupen64plus-input-qt.dylib
install_name_tool -change /usr/local/opt/sdl2/lib/libSDL2-2.0.0.dylib @executable_path/../Frameworks/libSDL2-2.0.0.dylib $contents/MacOS/mupen64plus-input-qt.dylib
install_name_tool -change /usr/local/opt/qt/lib/QtWidgets.framework/Versions/5/QtWidgets @executable_path/../Frameworks/QtWidgets.framework/Versions/5/QtWidgets $contents/MacOS/mupen64plus-input-qt.dylib
install_name_tool -change /usr/local/opt/qt/lib/QtGui.framework/Versions/5/QtGui @executable_path/../Frameworks/QtGui.framework/Versions/5/QtGui $contents/MacOS/mupen64plus-input-qt.dylib
install_name_tool -change /usr/local/opt/qt/lib/QtCore.framework/Versions/5/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/5/QtCore $contents/MacOS/mupen64plus-input-qt.dylib

otool -L $contents/MacOS/mupen64plus-input-qt.dylib

# mupen64plus-input-raphnetraw.dylib
install_name_tool -change /usr/local/opt/hidapi/lib/libhidapi.0.dylib @executable_path/../Frameworks/libhidapi.0.dylib $contents/MacOS/mupen64plus-input-raphnetraw.dylib

otool -L $contents/MacOS/mupen64plus-input-raphnetraw.dylib

# mupen64plus-rsp-hle.dylib
# nothing to link
otool -L $contents/MacOS/mupen64plus-rsp-hle.dylib

# mupen64plus-video-GLideN64.dylib
install_name_tool -change /usr/local/opt/qt/lib/QtWidgets.framework/Versions/5/QtWidgets @executable_path/../Frameworks/QtWidgets.framework/Versions/5/QtWidgets $contents/MacOS/mupen64plus-video-GLideN64.dylib
install_name_tool -change /usr/local/opt/qt/lib/QtGui.framework/Versions/5/QtGui @executable_path/../Frameworks/QtGui.framework/Versions/5/QtGui $contents/MacOS/mupen64plus-video-GLideN64.dylib
install_name_tool -change /usr/local/opt/qt/lib/QtCore.framework/Versions/5/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/5/QtCore $contents/MacOS/mupen64plus-video-GLideN64.dylib
install_name_tool -change /usr/local/opt/freetype/lib/libfreetype.6.dylib @executable_path/../Frameworks/libfreetype.6.dylib $contents/MacOS/mupen64plus-video-GLideN64.dylib

otool -L $contents/MacOS/mupen64plus-video-GLideN64.dylib

# Time to create the DMG

# 1. Create symlink to Applications folder
ln -s /Applications mupen64plus/Applications

# 2. Create DMG
hdiutil create -volname m64p -srcfolder mupen64plus -ov -format UDZO mupen64plus/m64p.dmg