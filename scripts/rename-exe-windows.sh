#!/bin/bash

# well, when you type the name of the program line ./enc + tab
# windows just can't resist putting an .exe in there because for some reason,
# gcc compiled progams default to "some-crap.exe"

# to stop this, it requires us to do this weird shit this way for some reason
# poor windows has a hard time dealing with files with no extension 🤔
[ -f "$1.exe" ] && {
	echo "copying \"$1.exe\" to \"$1\""
	mv "$1.exe" "$1"
	cp "$1" "$1.exe"
}