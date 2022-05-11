#!/bin/sh

for i in $@; do
	[ -f "/usr/bin/$i" ] && rm "/usr/bin/$i" || :
done