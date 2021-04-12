git remote add mupen64plus-core https://github.com/mupen64plus/mupen64plus-core.git
git fetch mupen64plus-core
git merge -s subtree mupen64plus-core/master --allow-unrelated-histories --squash
git commit -m "update core"

git remote add GLideN64 https://github.com/loganmc10/GLideN64.git
git fetch GLideN64
git merge -s subtree GLideN64/NUI --allow-unrelated-histories --squash
git commit -m "update gliden64"

git remote add mupen64plus-gui https://github.com/m64p/mupen64plus-gui.git
git fetch mupen64plus-gui
git merge -s subtree mupen64plus-gui/main --allow-unrelated-histories --squash
git commit -m "update gui"

git remote add mupen64plus-audio-sdl2 https://github.com/m64p/mupen64plus-audio-sdl2.git
git fetch mupen64plus-audio-sdl2
git merge -s subtree mupen64plus-audio-sdl2/main --allow-unrelated-histories --squash
git commit -m "update audio"

git remote add mupen64plus-rsp-hle https://github.com/mupen64plus/mupen64plus-rsp-hle.git
git fetch mupen64plus-rsp-hle
git merge -s subtree mupen64plus-rsp-hle/master --allow-unrelated-histories --squash
git commit -m "update rsp hle"

git remote add mupen64plus-input-qt https://github.com/m64p/mupen64plus-input-qt.git
git fetch mupen64plus-input-qt
git merge -s subtree mupen64plus-input-qt/main --allow-unrelated-histories --squash
git commit -m "update input qt"

git remote add mupen64plus-input-raphnetraw https://github.com/raphnet/mupen64plus-input-raphnetraw.git
git fetch mupen64plus-input-raphnetraw
git merge -s subtree mupen64plus-input-raphnetraw/master --allow-unrelated-histories --squash
git commit -m "update raphnet"

git remote add parallel-rsp https://github.com/loganmc10/parallel-rsp.git
git fetch parallel-rsp
git merge -s subtree parallel-rsp/master --allow-unrelated-histories --squash
git commit -m "update parallel rsp"

git remote add angrylion-rdp-plus https://github.com/ata4/angrylion-rdp-plus.git
git fetch angrylion-rdp-plus
git merge -s subtree angrylion-rdp-plus/master --allow-unrelated-histories --squash
git commit -m "update angrylion"

git push origin main
