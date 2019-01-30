#!/usr/bin/env bash

base_dir=$PWD

cd $base_dir/mupen64plus-core/projects/unix
make clean

cd $base_dir/mupen64plus-rsp-hle/projects/unix
make clean

cd $base_dir/mupen64plus-rsp-cxd4/projects/unix
make clean

cd $base_dir/mupen64plus-input-sdl/projects/unix
make clean

cd $base_dir/mupen64plus-audio-sdl2/projects/unix
make clean

rm -rf $base_dir/mupen64plus-gui/build

cd $base_dir/GLideN64/projects/cmake
make clean

rm -rf $base_dir/angrylion-rdp-plus/build

cd $base_dir
rm -rf $base_dir/mupen64plus
rm $base_dir/*.zip
