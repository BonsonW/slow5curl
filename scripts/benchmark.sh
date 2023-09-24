#! /bin/bash

# num_threads, list path
run () {
    ./slow5curl get $3 -t $1 --list $2 2> "logs/[chr22][t: $1].txt"
}

READS="test/data/raw/PGXX22394_reads_chr22_readid.list"
URL="https://gtgseq.s3.amazonaws.com/ont-r10-dna/NA12878/raw/PGXXHX230142_reads.blow5"

for THREADS in 4 8 16 32 64 128 256 512
do
    run $THREADS $READS $URL
done