#!/bin/bash
# Monitors current directory and copies changed files to a remote directory.
# This script is useful if the build process is done on a remote machine.
#
# Usage: ./copy_task.sh /target/dir
# Dependencies: fswatch, rsync
#
# TODO: support file deletion
#       figure out why copy process fails sometimes

IGNORE=(".git/" "___jb_old___" "___jb_bak___")
DEST="$1"

if [ -z "$DEST" ]; then exit 1; fi

show_date() {
    date
}

valid_file () {
    local in=0
    local file=$1
    for element in "${IGNORE[@]}"; do
        if [ -z "${file##*$element*}" ] ;then
            in=1
            break
        fi
    done
    return $in
}

rsync -azvqR . ${DEST}

fswatch -r . |\
while read file
do
	file=$(realpath --relative-to="." "$file")

	if valid_file $file; then
        echo "[$(show_date)]" $file
      	rsync -azvqR $file ${DEST}
    #else
        #echo "$file is ignored"
    fi
done
