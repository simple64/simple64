# mupen64plus-GLideN64

**Downloads found here: https://github.com/loganmc10/m64p/releases**
___

Linux Dependencies (Ubuntu 18.04)
```
sudo apt install qt5-default libsdl2-2.0-0 libhidapi-hidraw0 libminizip1 p7zip-full
```
Linux Dependencies (Manjaro/Arch)
```
sudo pacman -S --needed qt5-base sdl2 hidapi minizip p7zip
```
Linux Dependencies (Fedora)
```
sudo dnf install minizip-compat mesa-libGLU hidapi p7zip
```
___
Building:
```
git clone --recursive https://github.com/loganmc10/m64p.git
cd m64p
./build.sh
```
Build options (environment variables):
  - `NO_ASM`: set to `0` to enable assembly optimizations in mupen64plus components (default: `1`)
  - `GLIDE_NOHQ`: set to `On` or `1` to build GLideN64 without realtime texture enhancer library (default: `On`)
  - `GLIDE_VEC4`: set to `On` or `1` to build GLideN64 with additional VEC4 optimization (default: `On`)
  - `GLIDE_CRC`: set to `On` or `1` to build GLideN64 using xxHash to calculate texture CRC (default: `On`)
