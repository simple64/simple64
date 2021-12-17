On systems without Snap (like Fedora) you can use [toolbox](https://github.com/containers/toolbox) to easily resolve dependencies.

First, download and unzip the latest release (https://github.com/loganmc10/m64p/releases) somewhere in your home directory
```
sudo dnf -y install toolbox
toolbox create --image quay.io/loganmc10/m64p m64p -y

toolbox run -c m64p ~/path_to/mupen64plus/mupen64plus-gui
```
