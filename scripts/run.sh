#! /bin/bash

URL="https://gtgseq.s3.amazonaws.com/ont-r10-dna/NA12878/raw/PGXXHX230142_reads.blow5"
IDX="/home/hasindu/scratch/na12878_prom_lsk114/PGXXHX230142_reads.blow5.idx"
READ_LIST="../test/100_reads.txt"

cd build
./slow5curl get -h
# ./slow5curl get ${URL} --index ${IDX} -o reads.blow5 "00002194-fea5-433c-ba89-1eb6b60f0f28"
# ./slow5curl get ${URL} --index ${IDX} -t 32 -K 50 -o reads.blow5 --list ${READ_LIST}
./slow5curl get ${URL} -t 32 -K 50 -o reads.blow5 --list ${READ_LIST}
