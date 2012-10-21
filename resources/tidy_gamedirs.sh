#/bin/sh
#
# Author: Chris Babcock
#
# This script copies old Dgamename directories to ./games and renames them to
# gamename. This is neccessary for using the latest judge version.

mkdir games
for g in `ls | grep ^D | egrep -o [a-z0-9]+`;
	do cp -r D$g games/$g;
done
