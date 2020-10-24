# m64p

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

Mac OSX Dependencies

```sh
# First, install brew: https://brew.sh/

# Now, install dependencies
brew install freetype cmake libsamplerate hidapi unixodbc libpng pkg-config sdl2_net sdl2 qt libpq

# Work around brew + QT bug - https://github.com/Homebrew/legacy-homebrew/issues/29938
# Note that you should swap out the 5.15.1 with the version of Qt that was installed above
ln -s /usr/local/Cellar/qt/5.15.1/plugins /usr/local/plugins
ln -s /usr/local/Cellar/qt/5.15.1/mkspecs /usr/local/mkspecs
ln -s /usr/local/Cellar/qt/5.15.1/qmake /usr/local/qmake
```
___
Building:
```
git clone https://github.com/loganmc10/m64p.git
cd m64p
./build.sh
```
