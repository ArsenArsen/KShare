#!/usr/bin/env bash
if [[ "$1" == "" ]]
then
	echo "Provide a version with arg1" >&2
	exit 1
fi
cp deb work -r
sed "s/%ver/$1/g" deb/DEBIAN/control > work/DEBIAN/control
mkdir -p work/usr/bin
mkdir compiling
cd compiling 
qmake ../../KShare.pro
if make
then
cd ..
cp compiling/src/kshare work/usr/bin/kshare
else
	rm -rf compiling
	echo "Failed to make!"
	exit 2
fi
cd work
md5sum usr/bin/kshare usr/share/applications/KShare.desktop > DEBIAN/md5sums
cd ..
dpkg-deb -b work/
rm -rf work
