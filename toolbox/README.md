On systems without Snap (like Fedora) you can use [toolbox](https://github.com/containers/toolbox) to easily resolve dependencies.

```
sudo dnf -y install toolbox
toolbox create --image quay.io/loganmc10/m64p m64p -y

toolbox run -c m64p start
```
