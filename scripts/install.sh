#!/bin/sh

for i in $@; do
	[ -f "$i" ] && cp "$i" /usr/bin/"$i" || :
done