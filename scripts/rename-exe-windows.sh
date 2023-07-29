#!/bin/bash

# well, when you type the name of the program line ./enc + tab
# windows just can't resist putting an .exe in there because for some reason,
# gcc compiled progams default to "some-crap.exe"

# to stop this, it requires us to do this weird shit this way for some reason
# poor windows has a hard time dealing with files with no extension 🤔

! [ -n "$DST_DIR" ] && {
	echo "cannot rename, DST_DIR not found"
	exit 1
}

for arg in $@; do
	WITH_EXE="$DST_DIR/$arg.exe"
	WITHOUT_EXE="$DST_DIR/$arg"
	[ -f "$WITH_EXE" ] && {
		echo "copying \"$WITH_EXE\" to \"$WITHOUT_EXE\""
		mv "$WITH_EXE" "$WITHOUT_EXE"
		cp "$WITHOUT_EXE" "$WITH_EXE"
	}
done
