#!/bin/sh

! [ -n "$DST_DIR" ] && {
	echo "DST_DIR not set! cannot install!"
	exit 1
}

for i in $@; do
	[ -f "$i" ] && cp "$i" /usr/bin/"$i" || :
done