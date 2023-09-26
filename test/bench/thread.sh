#! /bin/bash

# num_threads, list, url, name
run () {
    /usr/bin/time -v ./slow5curl get $3 -t $1 --list $2 2> "logs/[$4][t: $1].txt" -o reads.blow5
}

READS="test/data/raw/PGXX22394_reads_chr22_readid.list"
URL="https://gtgseq.s3.amazonaws.com/ont-r10-dna/NA24385/raw/PGXX22394_reads.blow5"
NAME=chr22

for THREADS in 4 8 16 32 64 128 256 512
do
    run $THREADS $READS $URL $NAME
done