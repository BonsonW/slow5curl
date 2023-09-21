#!/bin/bash

RAW="test/data/raw/"
EXP="test/data/exp/"

URL="https://github.com/BonsonW/slow5curl/raw/main/test/data/raw/reads_10.blow5"
IDX="${RAW}reads_10.blow5.idx"
IDX_REM="https://github.com/BonsonW/slow5curl/raw/main/test/data/raw/reads_10.blow5.idx"
OUT="test/data/reads.blow5"
READ_LIST="${RAW}reads_10.txt"

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

TESTCASE_NAME="singlethread_singleread_customlocalindex"
echo_test_name ${TESTCASE_NAME}
ex ./slow5curl get ${URL} --index ${IDX} -o ${OUT} "00002194-fea5-433c-ba89-1eb6b60f0f28" || die "Running the tool failed for test: ${TESTCASE_NAME}"
diff -q ${EXP}reads_1.blow5 ${OUT} || die "diff failed for test: ${TESTCASE_NAME}"

TESTCASE_NAME="singlethread_singleread_customremoteindex"
echo_test_name ${TESTCASE_NAME}
ex ./slow5curl get ${URL} --index ${IDX_REM} -o ${OUT} "00002194-fea5-433c-ba89-1eb6b60f0f28" || die "Running the tool failed for test: ${TESTCASE_NAME}"
diff -q ${EXP}reads_1.blow5 ${OUT} || die "diff failed for test: ${TESTCASE_NAME}"

TESTCASE_NAME="singlethread_singleread_remoteindex"
echo_test_name ${TESTCASE_NAME}
ex ./slow5curl get ${URL} -o ${OUT} "00002194-fea5-433c-ba89-1eb6b60f0f28" || die "Running the tool failed for test: ${TESTCASE_NAME}"
diff -q ${EXP}reads_1.blow5 ${OUT} || die "diff failed for test: ${TESTCASE_NAME}"

TESTCASE_NAME="multithread_multiread_localindex"
echo_test_name ${TESTCASE_NAME}
ex ./slow5curl get ${URL} --index ${IDX} -o ${OUT} -l ${READ_LIST} -t 10 -K 10 || die "Running the tool failed for test: ${TESTCASE_NAME}"
diff -q ${EXP}reads_10.blow5 ${OUT} || die "diff failed for test: ${TESTCASE_NAME}"

TESTCASE_NAME="multithread_multiread_remoteindex"
echo_test_name ${TESTCASE_NAME}
ex ./slow5curl get ${URL} -o ${OUT} -l ${READ_LIST} -t 10 -K 10 || die "Running the tool failed for test: ${TESTCASE_NAME}"
diff -q ${EXP}reads_10.blow5 ${OUT} || die "diff failed for test: ${TESTCASE_NAME}"

TESTCASE_NAME="multithread_multiread_localindex_multibatch"
echo_test_name ${TESTCASE_NAME}
ex ./slow5curl get ${URL} --index ${IDX} -o ${OUT} -l ${READ_LIST} -t 2 -K 5 || die "Running the tool failed for test: ${TESTCASE_NAME}"
diff -q ${EXP}reads_10.blow5 ${OUT} || die "diff failed for test: ${TESTCASE_NAME}"

TESTCASE_NAME="multithread_multiread_localindex_multibatch_oneconnection"
echo_test_name ${TESTCASE_NAME}
ex ./slow5curl get ${URL} --index ${IDX} -o ${OUT} -l ${READ_LIST} -t 1 -K 5 || die "Running the tool failed for test: ${TESTCASE_NAME}"
diff -q ${EXP}reads_10.blow5 ${OUT} || die "diff failed for test: ${TESTCASE_NAME}"

TESTCASE_NAME="multithread_multiread_localindex_multibatch_defaults"
echo_test_name ${TESTCASE_NAME}
ex ./slow5curl get ${URL} --index ${IDX} -o ${OUT} -l ${READ_LIST} || die "Running the tool failed for test: ${TESTCASE_NAME}"
diff -q ${EXP}reads_10.blow5 ${OUT} || die "diff failed for test: ${TESTCASE_NAME}"
