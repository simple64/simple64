On systems without Snap (like Fedora) you can use [toolbox](https://containertoolbx.org/) to easily install m64p.
## Nvidia
The instructions on this page don't currently work with Nvidia proprietary graphics drivers. For these cases, you should use the [Flatpak on Flathub](https://flathub.org/apps/details/io.github.m64p.m64p) (I don't maintain this flatpak).
## Prep
### Fedora
```
sudo dnf -y install toolbox wget
```
### Arch/Manjaro
```
sudo pacman -S toolbox wget
```
## Installation
```
bash -x <(wget -qO- https://raw.githubusercontent.com/loganmc10/m64p/main/toolbox/install.sh)
```
## Updating
To update, simply re-run the installation script
## Removal
```
bash -x <(wget -qO- https://raw.githubusercontent.com/loganmc10/m64p/main/toolbox/remove.sh)
```
## Launching
Search for "m64p" in your application menu and launch from there.
