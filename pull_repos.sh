git remote add mupen64plus-core https://github.com/loganmc10/mupen64plus-core.git
git fetch mupen64plus-core
git merge -s subtree mupen64plus-core/simple64 --allow-unrelated-histories --squash
git commit -m "update core"

git remote add simple64-gui https://github.com/simple64/simple64-gui.git
git fetch simple64-gui
git merge -s subtree simple64-gui/main --allow-unrelated-histories --squash
git commit -m "update gui"

git remote add simple64-audio-sdl2 https://github.com/simple64/simple64-audio-sdl2.git
git fetch simple64-audio-sdl2
git merge -s subtree simple64-audio-sdl2/main --allow-unrelated-histories --squash
git commit -m "update audio"

git remote add simple64-input-qt https://github.com/simple64/simple64-input-qt.git
git fetch simple64-input-qt
git merge -s subtree simple64-input-qt/main --allow-unrelated-histories --squash
git commit -m "update input qt"

git remote add simple64-input-raphnetraw https://github.com/simple64/simple64-input-raphnetraw.git
git fetch simple64-input-raphnetraw
git merge -s subtree simple64-input-raphnetraw/simple64 --allow-unrelated-histories --squash
git commit -m "update raphnet"

git remote add parallel-rsp https://github.com/loganmc10/parallel-rsp.git
git fetch parallel-rsp
git merge -s subtree parallel-rsp/simple64 --allow-unrelated-histories --squash
git commit -m "update parallel rsp"

git remote add parallel-rdp-standalone https://github.com/loganmc10/parallel-rdp-standalone.git
git fetch parallel-rdp-standalone
git merge -s subtree parallel-rdp-standalone/simple64 --allow-unrelated-histories --squash
git commit -m "update parallel rdp"

git push origin main
