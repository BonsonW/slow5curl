#!/bin/bash

RAW="test/data/raw/"
EXP="test/data/exp/"

URL="https://gtgseq.s3.amazonaws.com/ont-r10-dna/NA12878/raw/PGXXHX230142_reads.blow5"
IDX="/home/hasindu/scratch/na12878_prom_lsk114/PGXXHX230142_reads.blow5.idx"
READ_LIST="${RAW}reads_100.txt"
OUT="test/data/reads.blow5"

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

TESTCASE_NAME="singlethread_singleread_localindex"
echo_test_name ${TESTCASE_NAME}
ex ./slow5curl get ${URL} --index ${IDX} -o ${OUT} "00002194-fea5-433c-ba89-1eb6b60f0f28" || die "Running the tool failed for test: ${TESTCASE_NAME}"
diff -q ${EXP}reads_1.blow5 ${OUT} || die "diff failed for test: ${TESTCASE_NAME}"

TESTCASE_NAME="singlethread_singleread_remoteindex"
echo_test_name ${TESTCASE_NAME}
ex ./slow5curl get ${URL} -o ${OUT} "00002194-fea5-433c-ba89-1eb6b60f0f28" || die "Running the tool failed for test: ${TESTCASE_NAME}"
diff -q ${EXP}reads_1.blow5 ${OUT} || die "diff failed for test: ${TESTCASE_NAME}"

TESTCASE_NAME="multithread_multiread_localindex"
echo_test_name ${TESTCASE_NAME}
ex ./slow5curl get ${URL} --index ${IDX} -o ${OUT} -l ${READ_LIST} || die "Running the tool failed for test: ${TESTCASE_NAME}"
diff -q ${EXP}reads_100.blow5 ${OUT} || die "diff failed for test: ${TESTCASE_NAME}"

TESTCASE_NAME="multithread_multiread_remoteindex"
echo_test_name ${TESTCASE_NAME}
ex ./slow5curl get ${URL} -o ${OUT} -l ${READ_LIST} || die "Running the tool failed for test: ${TESTCASE_NAME}"
diff -q ${EXP}reads_100.blow5 ${OUT} || die "diff failed for test: ${TESTCASE_NAME}"

TESTCASE_NAME="multithread_multiread_localindex_multibatch"
echo_test_name ${TESTCASE_NAME}
ex ./slow5curl get ${URL} --index ${IDX} -o ${OUT} -l ${READ_LIST} -t 32 -K 64 || die "Running the tool failed for test: ${TESTCASE_NAME}"
diff -q ${EXP}reads_100.blow5 ${OUT} || die "diff failed for test: ${TESTCASE_NAME}"

TESTCASE_NAME="multithread_multiread_localindex_multibatch_oneconnection"
echo_test_name ${TESTCASE_NAME}
ex ./slow5curl get ${URL} --index ${IDX} -o ${OUT} -l ${READ_LIST} -t 1 -K 64 || die "Running the tool failed for test: ${TESTCASE_NAME}"
diff -q ${EXP}reads_100.blow5 ${OUT} || die "diff failed for test: ${TESTCASE_NAME}"
