#!/bin/bash

URL="https://gtgseq.s3.amazonaws.com/ont-r10-dna/NA24385/raw/PGXX22394_reads.blow5"
READ_LIST="test/data/raw/PGXX22394_reads_chr22_readid.list"
OUT="test/data/out/"
BLOW_OUT="${OUT}reads.blow5"
EXP="363428f5351918ebcaf6f0f4a95e755c"
THREADS="512"

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

echo_test_name() {
    printf '\n--%s--\n' "$1"
}

# clean out dir
test -d ${OUT} && rm -r ${OUT}
mkdir ${OUT} || die "mkdir failed"

# large tests
TESTCASE_NAME="chr22_diff"
echo_test_name ${TESTCASE_NAME}
rm ${OUT}*
ex ./slow5curl get ${URL} -o ${BLOW_OUT} -l ${READ_LIST} -t ${THREADS} || die "Running the tool failed for test: ${TESTCASE_NAME}"
[[ $(md5sum ${BLOW_OUT} | cut -d' ' -f1 ) == ${EXP} ]] || die "diff failed for test: ${TESTCASE_NAME}"
