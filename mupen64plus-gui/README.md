# mupen64plus-gui

![m64p](http://i.imgur.com/NIa5Btx.png)

mupen64plus-gui is written in Qt5.
It supports everything you'd expect from a mupen64plus frontend
(plugin selection, configuration editing, save state management, screenshots, pausing, etc).

You can try this out by downloading [**m64p**](https://m64p.github.io/).
**m64p** is mupen64plus + GLideN64 + this GUI

It is [free/libre software](https://www.fsf.org/blogs/rms/20140407-geneva-tedx-talk-free-software-free-society)
under [license GPL](https://www.gnu.org/licenses/gpl)v3.0.
Enjoy and happy hacking!

## Dependencies

- [Qt](https://www.qt.io/) 5.4
  (this comes statically linked with the Windows binary build in m64p, so you don't need to worry about this on Windows)
- [SDL2](https://www.libsdl.org/) ***Your copy of mupen64plus-core (libmupen64plus.so.2) also needs to be linked against SDL2***
- 7za binary must be installed on your system for 7z/zip support

## Building (tested on GNU/Linux and MinGW)

On Debian and Ubuntu, you'll want to install ```qt5-default```.
```
git clone https://github.com/m64p/mupen64plus-gui.git
cd mupen64plus-gui
mkdir build
cd build
qmake ../mupen64plus-gui.pro
make -j`nproc`
```
Then you can run ```./mupen64plus-gui```.
