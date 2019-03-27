#!/usr/bin/env bash

base_dir=$PWD

make clean -C $base_dir/mupen64plus-core/projects/unix

make clean -C $base_dir/mupen64plus-rsp-hle/projects/unix

make clean -C $base_dir/mupen64plus-input-sdl/projects/unix

make clean -C $base_dir/mupen64plus-audio-sdl2/projects/unix

rm -rf $base_dir/mupen64plus-gui/build

rm -rf $base_dir/GLideN64/src/GLideNUI/build
make clean -C $base_dir/GLideN64/projects/cmake/GLideNHQ
make clean -C $base_dir/GLideN64/projects/cmake

rm -rf $base_dir/mupen64plus
rm $base_dir/*.zip
