#!/bin/bash

RAW="test/data/raw/"
EXP="test/data/exp/"

URL="https://github.com/BonsonW/slow5curl/raw/main/test/data/raw/reads_10.blow5"
IDX="${RAW}reads_10.blow5.idx"
IDX_REM="https://github.com/BonsonW/slow5curl/raw/main/test/data/raw/reads_10.blow5.idx"
OUT="test/data/out/"
BLOW_OUT="${OUT}reads.blow5"
SLOW_OUT="${OUT}reads.slow5"
TXT_OUT="${OUT}text.txt"
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
        valgrind --leak-check=full --show-leak-kinds=all --error-exitcode=1 "$@"
    else
        "$@"
    fi
}

echo_test_name() {
    printf '\n--%s--\n' "$1"
}

test -d ${OUT} && rm -r ${OUT}
mkdir ${OUT} || die "mkdir failed"

# version
TESTCASE_NAME="version"
echo_test_name ${TESTCASE_NAME}
ex ./slow5curl --version

# cache opt
TESTCASE_NAME="get_cached"
echo_test_name ${TESTCASE_NAME}
rm ${OUT}*
ex ./slow5curl get ${URL} --cache "${OUT}cached" -o ${BLOW_OUT} "00002194-fea5-433c-ba89-1eb6b60f0f28" || die "Running the tool failed for test: ${TESTCASE_NAME}"
ex ./slow5curl get ${URL} --index "${OUT}cached" -o ${BLOW_OUT} "00002194-fea5-433c-ba89-1eb6b60f0f28" || die "Running the tool failed for test: ${TESTCASE_NAME}"
diff -q ${EXP}reads_1.blow5 ${BLOW_OUT} || die "diff failed for test: ${TESTCASE_NAME}"

TESTCASE_NAME="reads_cached"
rm ${OUT}*
test -f "${OUT}cached" && rm "${OUT}cached"
ex ./slow5curl reads ${URL} > ${TXT_OUT} --cache "${OUT}cached" || die "Running the tool failed for test: ${TESTCASE_NAME}"
ex ./slow5curl get ${URL} --index "${OUT}cached" -o ${BLOW_OUT} "00002194-fea5-433c-ba89-1eb6b60f0f28" || die "Running the tool failed for test: ${TESTCASE_NAME}"
diff -q ${EXP}reads_1.blow5 ${BLOW_OUT} || die "diff failed for test: ${TESTCASE_NAME}"

TESTCASE_NAME="get_cached_customeremoteindex"
rm ${OUT}*
test -f "${OUT}cached" && rm "${OUT}cached"
ex ./slow5curl get ${URL} --index ${IDX_REM} --cache "${OUT}cached" -o ${BLOW_OUT} "00002194-fea5-433c-ba89-1eb6b60f0f28" || die "Running the tool failed for test: ${TESTCASE_NAME}"
ex ./slow5curl get ${URL} --index "${OUT}cached" -o ${BLOW_OUT} "00002194-fea5-433c-ba89-1eb6b60f0f28" || die "Running the tool failed for test: ${TESTCASE_NAME}"
diff -q ${EXP}reads_1.blow5 ${BLOW_OUT} || die "diff failed for test: ${TESTCASE_NAME}"

TESTCASE_NAME="reads_cached_customeremoteindex"
echo_test_name ${TESTCASE_NAME}
rm ${OUT}*
ex ./slow5curl reads ${URL} > ${TXT_OUT} --index ${IDX_REM} --cache "${OUT}cached" || die "Running the tool failed for test: ${TESTCASE_NAME}"
ex ./slow5curl get ${URL} --index "${OUT}cached" -o ${BLOW_OUT} "00002194-fea5-433c-ba89-1eb6b60f0f28" || die "Running the tool failed for test: ${TESTCASE_NAME}"
diff -q ${EXP}reads_1.blow5 ${BLOW_OUT} || die "diff failed for test: ${TESTCASE_NAME}"

TESTCASE_NAME="get_cached_customelocalindex"
rm ${OUT}*
test -f "${OUT}cached" && rm "${OUT}cached"
ex ./slow5curl get ${URL} --index ${IDX} --cache "${OUT}cached" -o ${BLOW_OUT} "00002194-fea5-433c-ba89-1eb6b60f0f28" || die "Running the tool failed for test: ${TESTCASE_NAME}"
ex ./slow5curl get ${URL} --index "${OUT}cached" -o ${BLOW_OUT} "00002194-fea5-433c-ba89-1eb6b60f0f28" && die "Running the tool failed for test: ${TESTCASE_NAME}"

TESTCASE_NAME="reads_cached_customelocalindex"
echo_test_name ${TESTCASE_NAME}
rm ${OUT}*
ex ./slow5curl reads ${URL} > ${TXT_OUT} --index ${IDX} --cache "${OUT}cached" || die "Running the tool failed for test: ${TESTCASE_NAME}"
ex ./slow5curl get ${URL} --index "${OUT}cached" -o ${BLOW_OUT} "00002194-fea5-433c-ba89-1eb6b60f0f28" && die "Running the tool failed for test: ${TESTCASE_NAME}"

# head
TESTCASE_NAME="head"
echo_test_name ${TESTCASE_NAME}
rm ${OUT}*
ex ./slow5curl head ${URL} > ${TXT_OUT} || die "Running the tool failed for test: ${TESTCASE_NAME}"
diff -q ${EXP}head.txt ${TXT_OUT} || die "diff failed for test: ${TESTCASE_NAME}"

# reads
TESTCASE_NAME="reads_remoteindex"
echo_test_name ${TESTCASE_NAME}
rm ${OUT}*
ex ./slow5curl reads ${URL} > ${TXT_OUT} || die "Running the tool failed for test: ${TESTCASE_NAME}"
diff -q ${EXP}reads_10.txt ${TXT_OUT} || die "diff failed for test: ${TESTCASE_NAME}"

TESTCASE_NAME="reads_customlocalindex"
echo_test_name ${TESTCASE_NAME}
rm ${OUT}*
ex ./slow5curl reads ${URL} --index ${IDX} > ${TXT_OUT} || die "Running the tool failed for test: ${TESTCASE_NAME}"
diff -q ${EXP}reads_10.txt ${TXT_OUT} || die "diff failed for test: ${TESTCASE_NAME}"

TESTCASE_NAME="reads_customremoteindex"
echo_test_name ${TESTCASE_NAME}
rm ${OUT}*
ex ./slow5curl reads ${URL} --index ${IDX_REM} > ${TXT_OUT} || die "Running the tool failed for test: ${TESTCASE_NAME}"
diff -q ${EXP}reads_10.txt ${TXT_OUT} || die "diff failed for test: ${TESTCASE_NAME}"

# get
TESTCASE_NAME="singlethread_singleread_customlocalindex"
echo_test_name ${TESTCASE_NAME}
rm ${OUT}*
ex ./slow5curl get ${URL} --index ${IDX} -o ${BLOW_OUT} "00002194-fea5-433c-ba89-1eb6b60f0f28" || die "Running the tool failed for test: ${TESTCASE_NAME}"
diff -q ${EXP}reads_1.blow5 ${BLOW_OUT} || die "diff failed for test: ${TESTCASE_NAME}"

TESTCASE_NAME="singlethread_singleread_customremoteindex"
echo_test_name ${TESTCASE_NAME}
rm ${OUT}*
ex ./slow5curl get ${URL} --index ${IDX_REM} -o ${BLOW_OUT} "00002194-fea5-433c-ba89-1eb6b60f0f28" || die "Running the tool failed for test: ${TESTCASE_NAME}"
diff -q ${EXP}reads_1.blow5 ${BLOW_OUT} || die "diff failed for test: ${TESTCASE_NAME}"

TESTCASE_NAME="singlethread_singleread_remoteindex"
echo_test_name ${TESTCASE_NAME}
rm ${OUT}*
ex ./slow5curl get ${URL} -o ${BLOW_OUT} "00002194-fea5-433c-ba89-1eb6b60f0f28" || die "Running the tool failed for test: ${TESTCASE_NAME}"
diff -q ${EXP}reads_1.blow5 ${BLOW_OUT} || die "diff failed for test: ${TESTCASE_NAME}"

TESTCASE_NAME="multithread_multiread_localindex"
echo_test_name ${TESTCASE_NAME}
rm ${OUT}*
ex ./slow5curl get ${URL} --index ${IDX} -o ${BLOW_OUT} -l ${READ_LIST} -t 10 -K 10 || die "Running the tool failed for test: ${TESTCASE_NAME}"
diff -q ${EXP}reads_10.blow5 ${BLOW_OUT} || die "diff failed for test: ${TESTCASE_NAME}"

TESTCASE_NAME="multithread_multiread_remoteindex"
echo_test_name ${TESTCASE_NAME}
rm ${OUT}*
ex ./slow5curl get ${URL} -o ${BLOW_OUT} -l ${READ_LIST} -t 10 -K 10 || die "Running the tool failed for test: ${TESTCASE_NAME}"
diff -q ${EXP}reads_10.blow5 ${BLOW_OUT} || die "diff failed for test: ${TESTCASE_NAME}"

TESTCASE_NAME="multithread_multiread_localindex_multibatch"
echo_test_name ${TESTCASE_NAME}
rm ${OUT}*
ex ./slow5curl get ${URL} --index ${IDX} -o ${BLOW_OUT} -l ${READ_LIST} -t 2 -K 5 || die "Running the tool failed for test: ${TESTCASE_NAME}"
diff -q ${EXP}reads_10.blow5 ${BLOW_OUT} || die "diff failed for test: ${TESTCASE_NAME}"

# file type conversion
TESTCASE_NAME="singlethread_singleread_remoteindex_auto_blow5"
echo_test_name ${TESTCASE_NAME}
rm ${OUT}*
ex ./slow5curl get ${URL} -o ${BLOW_OUT} "00002194-fea5-433c-ba89-1eb6b60f0f28" || die "Running the tool failed for test: ${TESTCASE_NAME}"
diff -q ${EXP}reads_1.blow5 ${BLOW_OUT} || die "diff failed for test: ${TESTCASE_NAME}"

TESTCASE_NAME="singlethread_singleread_remoteindex_auto_slow5"
echo_test_name ${TESTCASE_NAME}
rm ${OUT}*
ex ./slow5curl get ${URL} -o ${SLOW_OUT} "00002194-fea5-433c-ba89-1eb6b60f0f28" || die "Running the tool failed for test: ${TESTCASE_NAME}"
diff -q ${EXP}reads_1.slow5 ${SLOW_OUT} || die "diff failed for test: ${TESTCASE_NAME}"

TESTCASE_NAME="singlethread_singleread_remoteindex_to_blow5"
echo_test_name ${TESTCASE_NAME}
rm ${OUT}*
ex ./slow5curl get ${URL} -o ${BLOW_OUT} --to "blow5" "00002194-fea5-433c-ba89-1eb6b60f0f28" || die "Running the tool failed for test: ${TESTCASE_NAME}"
diff -q ${EXP}reads_1.blow5 ${BLOW_OUT} || die "diff failed for test: ${TESTCASE_NAME}"

TESTCASE_NAME="singlethread_singleread_remoteindex_to_slow5"
echo_test_name ${TESTCASE_NAME}
rm ${OUT}*
ex ./slow5curl get ${URL} -o ${SLOW_OUT} --to "slow5" "00002194-fea5-433c-ba89-1eb6b60f0f28" || die "Running the tool failed for test: ${TESTCASE_NAME}"
diff -q ${EXP}reads_1.slow5 ${SLOW_OUT} || die "diff failed for test: ${TESTCASE_NAME}"

# compression type
TESTCASE_NAME="singlethread_singleread_remoteindex_compress_none"
echo_test_name ${TESTCASE_NAME}
rm ${OUT}*
ex ./slow5curl get ${URL} -o "${OUT}press_none.blow5" -c "none" "00002194-fea5-433c-ba89-1eb6b60f0f28" || die "Running the tool failed for test: ${TESTCASE_NAME}"
diff -q ${EXP}press_none.blow5 "${OUT}press_none.blow5" || die "diff failed for test: ${TESTCASE_NAME}"

TESTCASE_NAME="singlethread_singleread_remoteindex_compress_zlib"
echo_test_name ${TESTCASE_NAME}
rm ${OUT}*
ex ./slow5curl get ${URL} -o "${OUT}press_zlib.blow5" -c "zlib" "00002194-fea5-433c-ba89-1eb6b60f0f28" || die "Running the tool failed for test: ${TESTCASE_NAME}"
diff -q ${EXP}press_zlib.blow5 "${OUT}press_zlib.blow5" || die "diff failed for test: ${TESTCASE_NAME}"

# signal type
TESTCASE_NAME="singlethread_singleread_remoteindex_signal_none"
echo_test_name ${TESTCASE_NAME}
rm ${OUT}*
ex ./slow5curl get ${URL} -o "${OUT}sig_none.blow5" -s "none" "00002194-fea5-433c-ba89-1eb6b60f0f28" || die "Running the tool failed for test: ${TESTCASE_NAME}"
diff -q ${EXP}sig_none.blow5 "${OUT}sig_none.blow5" || die "diff failed for test: ${TESTCASE_NAME}"

TESTCASE_NAME="singlethread_singleread_remoteindex_signal_svb-zd"
echo_test_name ${TESTCASE_NAME}
rm ${OUT}*
ex ./slow5curl get ${URL} -o "${OUT}sig_svb-zd.blow5" -c "zlib" "00002194-fea5-433c-ba89-1eb6b60f0f28" || die "Running the tool failed for test: ${TESTCASE_NAME}"
diff -q ${EXP}sig_svb-zd.blow5 "${OUT}sig_svb-zd.blow5" || die "diff failed for test: ${TESTCASE_NAME}"