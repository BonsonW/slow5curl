#!/bin/bash

# terminate script
die() {
	echo "$1" >&2
	echo
	exit 1
}

./tool test/example.blow5 > test/tmp.txt  || die "Running the tool failed"
diff -q test/example.exp test/tmp.txt || die "diff failed"

echo "Test passed"