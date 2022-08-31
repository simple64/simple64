# simple64-gui

![simple64](http://i.imgur.com/NIa5Btx.png)

simple64-gui is written in Qt6.

You can try this out by downloading [**simple64**](https://simple64.github.io/).

It is [free/libre software](https://www.fsf.org/blogs/rms/20140407-geneva-tedx-talk-free-software-free-society)
under [license GPL](https://www.gnu.org/licenses/gpl)v3.0.
Enjoy and happy hacking!

## Dependencies

- [Qt6](https://www.qt.io/)
- [SDL2](https://www.libsdl.org/)
- 7za binary must be installed on your system for 7z/zip support

## Building (tested on GNU/Linux and MinGW)

On Debian and Ubuntu, you'll want to install ```qt5-default```.
```
git clone https://github.com/simple64/simple64-gui.git
cd simple64-gui
mkdir build
cd build
qmake ../simple64-gui.pro
make -j`nproc`
```
Then you can run ```./simple64-gui```.
