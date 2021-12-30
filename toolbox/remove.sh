#!/bin/bash

podman stop m64p
toolbox rm m64p -y
sleep 5
podman image prune -a -f
rm -f ~/.local/share/icons/m64p.png
rm -f ~/.local/share/applications/m64p.desktop
