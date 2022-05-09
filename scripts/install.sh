#!/bin/sh

for i in $@; do
	[ -f "$i" ] && cp "$i" /usr/bin/"$i" || :
	
	# to work on retarded mongoloid windows unoperating system
	[ -f "$i.exe" ] && cp "$i.exe" /usr/bin/"$i.exe" || :
done