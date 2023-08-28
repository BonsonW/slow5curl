#!/bin/bash

URL="https://gtgseq.s3.amazonaws.com/ont-r10-dna/NA12878/raw/PGXXHX230142_reads.blow5"
IDX="test/raw/reads_10.blow5.idx"
OUT="test/reads.blow5.idx"
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

./slow5curl get -i ${IDX} -o ${OUT} "00002194-fea5-433c-ba89-1eb6b60f0f28"  || die "Running the tool failed"
diff -q test/exp/reads_10.blow5 ${OUT} || die "diff failed"

echo "Test passed"