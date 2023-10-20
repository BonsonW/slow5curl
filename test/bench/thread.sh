#! /bin/bash

# num_threads, list, url, name
run () {
    /usr/bin/time -v ./slow5curl get $3 -t $1 --list $2 -o reads.blow5 --index $5 2> "logs/[$4 $6][t: $1].txt"
}

READS="slow5curl/test/data/raw/PGXX22394_reads_chr22_readid.list"
URL="https://gtgseq.s3.amazonaws.com/ont-r10-dna/NA24385/raw/PGXX22394_reads.blow5"
INDEX="/home/hasindu/scratch/hg2_prom_lsk114/PGXX22394_reads.blow5.idx"
NAME=chr22
NUM_RUNS=1
ALL_THREADS=(
    4
    8
    16
    32
    64
    128
    256
    512
)

for i in $(seq 1 $NUM_RUNS)
do
    for THREADS in ${ALL_THREADS[@]}
    do
        run $THREADS $READS $URL $NAME $INDEX $i
    done
done