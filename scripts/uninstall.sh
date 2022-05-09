#!/bin/sh

for i in $@; do
	[ -f "/usr/bin/$i" ] && rm "/usr/bin/$i" || :

	# to work on retarded mongoloid windows uno"perating system
	[ -f "/usr/bin/$i.exe" ] && rm "/usr/bin/$i.exe" || :
done