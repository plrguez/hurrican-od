#! /bin/sh

if [ -f hurrican-od.opk ]; then
rm hurrican-od.opk
fi
mksquashfs data lang hurrican-od default.gcw0.desktop hurrican-od.sh readme.txt Hurrican.png hurrican-od.opk -all-root -no-xattrs -noappend -no-exports
