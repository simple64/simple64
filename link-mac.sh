#!/bin/bash

# Get files in correct directories
mv mupen64plus/font.ttf mupen64plus/mupen64plus-gui.app/Contents/MacOS
mv mupen64plus/libmupen64plus.dylib mupen64plus/mupen64plus-gui.app/Contents/MacOS
mv mupen64plus/GLideN64.custom.ini mupen64plus/mupen64plus-gui.app/Contents/MacOS
mv mupen64plus/mupen64plus-audio-sdl2.dylib mupen64plus/mupen64plus-gui.app/Contents/MacOS
mv mupen64plus/mupen64plus-input-qt.dylib mupen64plus/mupen64plus-gui.app/Contents/MacOS
mv mupen64plus/mupen64plus-input-raphnetraw.dylib mupen64plus/mupen64plus-gui.app/Contents/MacOS
mv mupen64plus/mupen64plus-rsp-hle.dylib mupen64plus/mupen64plus-gui.app/Contents/MacOS
mv mupen64plus/mupen64plus-video-GLideN64.dylib mupen64plus/mupen64plus-gui.app/Contents/MacOS
mv mupen64plus/mupen64plus.ini mupen64plus/mupen64plus-gui.app/Contents/MacOS
mv mupen64plus/mupencheat.txt mupen64plus/mupen64plus-gui.app/Contents/MacOS

# SDL
cp /usr/local/opt/sdl2_net/lib/libSDL2_net-2.0.0.dylib mupen64plus/mupen64plus-gui.app/Contents/Frameworks
install_name_tool -change /usr/local/opt/sdl2/lib/libSDL2-2.0.0.dylib @executable_path/../Frameworks/libSDL2-2.0.0.dylib mupen64plus/mupen64plus-gui.app/Contents/Frameworks/libSDL2_net-2.0.0.dylib

otool -L mupen64plus/mupen64plus-gui.app/Contents/Frameworks/libSDL2_net-2.0.0.dylib

# mupen64plus-gui
# nothing to link
otool -L mupen64plus/mupen64plus-gui.app/Contents/MacOS/mupen64plus-gui

# libmupen64plus.dylib
install_name_tool -change /usr/local/opt/sdl2/lib/libSDL2-2.0.0.dylib @executable_path/../Frameworks/libSDL2-2.0.0.dylib mupen64plus/mupen64plus-gui.app/Contents/MacOS/libmupen64plus.dylib
install_name_tool -change /usr/local/opt/sdl2_net/lib/libSDL2_net-2.0.0.dylib @executable_path/../Frameworks/libSDL2_net-2.0.0.dylib mupen64plus/mupen64plus-gui.app/Contents/MacOS/libmupen64plus.dylib

otool -L mupen64plus/mupen64plus-gui.app/Contents/MacOS/libmupen64plus.dylib

# mupen64plus-audio-sdl2.dylib
install_name_tool -change /usr/local/opt/sdl2/lib/libSDL2-2.0.0.dylib @executable_path/../Frameworks/libSDL2-2.0.0.dylib mupen64plus/mupen64plus-gui.app/Contents/MacOS/mupen64plus-audio-sdl2.dylib

otool -L mupen64plus/mupen64plus-gui.app/Contents/MacOS/libmupen64plus.dylib

# mupen64plus-input-qt.dylib
install_name_tool -change /usr/local/opt/sdl2/lib/libSDL2-2.0.0.dylib @executable_path/../Frameworks/libSDL2-2.0.0.dylib mupen64plus/mupen64plus-gui.app/Contents/MacOS/mupen64plus-input-qt.dylib
install_name_tool -change /usr/local/opt/qt/lib/QtWidgets.framework/Versions/5/QtWidgets @executable_path/../Frameworks/QtWidgets.framework/Versions/5/QtWidgets mupen64plus/mupen64plus-gui.app/Contents/MacOS/mupen64plus-input-qt.dylib
install_name_tool -change /usr/local/opt/qt/lib/QtGui.framework/Versions/5/QtGui @executable_path/../Frameworks/QtGui.framework/Versions/5/QtGui mupen64plus/mupen64plus-gui.app/Contents/MacOS/mupen64plus-input-qt.dylib
install_name_tool -change /usr/local/opt/qt/lib/QtCore.framework/Versions/5/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/5/QtCore mupen64plus/mupen64plus-gui.app/Contents/MacOS/mupen64plus-input-qt.dylib

otool -L mupen64plus/mupen64plus-gui.app/Contents/MacOS/mupen64plus-input-qt.dylib

# mupen64plus-input-raphnetraw.dylib
# nothing to link
otool -L mupen64plus/mupen64plus-gui.app/Contents/MacOS/mupen64plus-input-raphnetraw.dylib

# mupen64plus-rsp-hle.dylib
# nothing to link
otool -L mupen64plus/mupen64plus-gui.app/Contents/MacOS/mupen64plus-rsp-hle.dylib

# mupen64plus-video-GLideN64.dylib
install_name_tool -change /usr/local/opt/qt/lib/QtWidgets.framework/Versions/5/QtWidgets @executable_path/../Frameworks/QtWidgets.framework/Versions/5/QtWidgets mupen64plus/mupen64plus-gui.app/Contents/MacOS/mupen64plus-video-GLideN64.dylib
install_name_tool -change /usr/local/opt/qt/lib/QtGui.framework/Versions/5/QtGui @executable_path/../Frameworks/QtGui.framework/Versions/5/QtGui mupen64plus/mupen64plus-gui.app/Contents/MacOS/mupen64plus-video-GLideN64.dylib
install_name_tool -change /usr/local/opt/qt/lib/QtCore.framework/Versions/5/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/5/QtCore mupen64plus/mupen64plus-gui.app/Contents/MacOS/mupen64plus-video-GLideN64.dylib

otool -L mupen64plus/mupen64plus-gui.app/Contents/MacOS/mupen64plus-video-GLideN64.dylib
