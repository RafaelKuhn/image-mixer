#!/bin/bash

# well windows requires us to do this shit for some reason
# poor windows has a hard time dealing with files with no extension 🤔
[ -f demix.exe ] && {
	mv demix.exe demix;
	cp demix demix.exe;
}