#!/bin/bash

# well windows requires us to do this shit this way for some reason
# poor windows has a hard time dealing with files with no extension 🤔
[ -f demix.exe ] && {
	echo "copying \"demix.exe\" to \"demix\""
	mv demix.exe demix
	cp demix demix.exe
}