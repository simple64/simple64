# mupen64plus-GLideN64

**Downloads found here: https://github.com/loganmc10/m64p/releases**
___

Linux Dependencies (Ubuntu 18.04)
```
sudo apt install qt5-default libqt5websockets5 libsdl2-2.0-0 libsdl2-net-2.0-0 libhidapi-hidraw0 libminizip1 p7zip-full
```
Linux Dependencies (Manjaro/Arch)
```
sudo pacman -S --needed qt5-base qt5-websockets sdl2 sdl2_net hidapi minizip p7zip
```
Linux Dependencies (Fedora)
```
sudo dnf install minizip-compat mesa-libGLU hidapi p7zip qt5-qtwebsockets
```
___
Building:
```
git clone --recursive https://github.com/loganmc10/m64p.git
cd m64p
./build.sh
```
