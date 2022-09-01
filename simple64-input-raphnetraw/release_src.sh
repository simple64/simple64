#!/bin/bash

PREFIX=mupen64plus-input-raphnetraw
HEXFILE=$PREFIX.hex

echo "Release script for $PREFIX"

if [ $# -ne 2 ]; then
	echo "Syntax: ./release.sh version releasedir"
	echo
	echo "ex: './release 1.0' will produce $PREFIX-1.0.tar.gz in releasedir out of git HEAD,"
	echo "and place it in the release directory."
	echo
	echo "It will also create a tag named v1.0"
	exit
fi

VERSION=$1
RELEASEDIR=$2
DIRNAME=$PREFIX-$VERSION
FILENAME=$PREFIX-$VERSION.tar.gz
TAG=v$VERSION

echo "Version: $VERSION"
echo "Filename: $FILENAME"
echo "Release directory: $RELEASEDIR"
echo "--------"
echo "Ready? Press ENTER to go ahead (or CTRL+C to cancel)"

read

if [ -f $RELEASEDIR/$FILENAME ]; then
	echo "Release file already exists!"
	exit 1
fi

git tag $TAG -f
git archive --format=tar --prefix=$DIRNAME/ HEAD | gzip > $RELEASEDIR/$FILENAME
