#!/bin/bash

URL="https://github.com/BonsonW/slow5curl/raw/main/test/raw/reads_10.blow5"
IDX="test/raw/reads_10.blow5.idx"
OUT="test/reads.blow5"
READ_LIST="test/raw/reads_10.txt"

# terminate script
die() {
	echo "$1" >&2
	echo
	exit 1
}

if [ "$1" = 'mem' ]; then
    mem=1
else
    mem=0
fi

ex() {
    if [ $mem -eq 1 ]; then
        valgrind --leak-check=full --error-exitcode=1 "$@"
    else
        "$@"
    fi
}

# # test single thread
# ex ./slow5curl get ${URL} --index ${IDX} -o ${OUT} "00002194-fea5-433c-ba89-1eb6b60f0f28" || die "Running the tool failed"
# diff -q test/exp/reads_1.blow5 ${OUT} || die "diff failed"

# # test single thread remote idx
# ex ./slow5curl get ${URL} -o ${OUT} "00002194-fea5-433c-ba89-1eb6b60f0f28" || die "Running the tool failed"
# diff -q test/exp/reads_1.blow5 ${OUT} || die "diff failed"

# # test multi thread
# ex ./slow5curl get ${URL} --index ${IDX} -o ${OUT} -l ${READ_LIST} || die "Running the tool failed"
# diff -q test/exp/reads_10.blow5 ${OUT} || die "diff failed"

# # test multi thread remote idx
# ex ./slow5curl get ${URL} -o ${OUT} -l ${READ_LIST} || die "Running the tool failed"
# diff -q test/exp/reads_10.blow5 ${OUT} || die "diff failed"

# test multi thread w/ partitioned batches
ex ./slow5curl get ${URL} --index ${IDX} -o ${OUT} -l ${READ_LIST} -t 2 -K 5 || die "Running the tool failed"
diff -q test/exp/reads_10.blow5 ${OUT} || die "diff failed"
