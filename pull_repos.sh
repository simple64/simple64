git remote add mupen64plus-core https://github.com/loganmc10/mupen64plus-core.git
git fetch mupen64plus-core
git merge -s subtree mupen64plus-core/m64p --allow-unrelated-histories --squash
git commit -m "update core"

git remote add mupen64plus-gui https://github.com/m64p/mupen64plus-gui.git
git fetch mupen64plus-gui
git merge -s subtree mupen64plus-gui/main --allow-unrelated-histories --squash
git commit -m "update gui"

git remote add mupen64plus-audio-sdl2 https://github.com/m64p/mupen64plus-audio-sdl2.git
git fetch mupen64plus-audio-sdl2
git merge -s subtree mupen64plus-audio-sdl2/main --allow-unrelated-histories --squash
git commit -m "update audio"

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

git remote add parallel-rdp-standalone https://github.com/loganmc10/parallel-rdp-standalone.git
git fetch parallel-rdp-standalone
git merge -s subtree parallel-rdp-standalone/master --allow-unrelated-histories --squash
git commit -m "update parallel rdp"

git push origin main
