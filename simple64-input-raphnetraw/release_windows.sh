#!/bin/bash

PROJECT=mupen64plus-input-raphnetraw-windows

if [ $# -ne 2 ]; then
	echo "Syntax: ./release.sh version releasedir"
	echo
	exit
fi

VERSION=$1
RELEASEDIR=$2
DIRNAME=$PROJECT-$VERSION
FILENAME=$PROJECT-$VERSION.zip

if [ -d $RELEASEDIR/$DIRNAME ]; then
	echo "$RELEASEDIR/$DIRNAME already exists";
	exit 1
fi

mkdir -p $RELEASEDIR/$DIRNAME
if [ $? -ne 0 ]; then
	echo "Could not mkdir"
	exit 1
fi

cd projects/unix
if [ $? -ne 0 ]; then
	echo "Could not chdir"
	exit 1
fi

./buildwin32.sh
if [ $? -ne 0 ]; then
	echo "Win32 build failed"
	exit 1
fi

./buildwin64.sh
if [ $? -ne 0 ]; then
	echo "Win64 build failed"
	exit 1
fi

cd .. # exit unix
cd .. # exit projects

cp projects/unix/dist_win32 -rv $RELEASEDIR/$DIRNAME
cp projects/unix/dist_win64 -rv $RELEASEDIR/$DIRNAME
cp README.md -rv $RELEASEDIR/$DIRNAME
cp README.md -rv $RELEASEDIR/$DIRNAME/readme.txt
cp RELEASE -rv $RELEASEDIR/$DIRNAME/releases.txt

cd $RELEASEDIR

zip -r $FILENAME $DIRNAME
echo
echo
echo
echo -------------------------------------
ls -lh $FILENAME
echo Done
