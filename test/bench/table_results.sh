#!/bin/bash

set -e

# terminate script
die() {
	echo "$1" >&2
	echo
	exit 1
}

OUTPUT="results.csv"

test -f $OUTPUT || touch $OUTPUT

time_stamp_to_secs () {
    ret=0
    while read field; do
        ret=$(echo "$ret * 60" | bc)
        ret=$(echo "$ret + $field" | bc)
    done
    echo $ret
}

insert_entry () {
    name=$1
	dir_name=$(echo ${name} | awk -F"_" '{print $1}')

    sam_load_time=$(grep "Elapsed (wall clock) time (h:mm:ss or m:ss):" "logs/${name}_sam.txt" | grep -Eo "[0-9]+:.*" | sed 's/:/\n/g' | time_stamp_to_secs)
    basecall_time=$(grep "Elapsed (wall clock) time (h:mm:ss or m:ss):" "logs/${name}_basecall.txt" | grep -Eo "[0-9]+:.*" | sed 's/:/\n/g' | time_stamp_to_secs)
    total_s5curl_time=$(grep "Elapsed (wall clock) time (h:mm:ss or m:ss):" "logs/${name}_curl.txt" | grep -Eo "[0-9]+:.*" | sed 's/:/\n/g' | time_stamp_to_secs)
    header_load_time=$(grep "Header Load" "logs/${name}_curl.txt" | grep -Eo "[0-9]+\.[0-9]+")
    index_load_time=$(grep "Index Load:" "logs/${name}_curl.txt" | grep -Eo "[0-9]+\.[0-9]+")
    fetch_time=$(grep "Fetch:" "logs/${name}_curl.txt" | grep -Eo "[0-9]+\.[0-9]+")
    encode_time=$(grep "Encode:" "logs/${name}_curl.txt" | grep -Eo "[0-9]+\.[0-9]+")
    write_time=$(grep "Write:" "logs/${name}_curl.txt" | grep -Eo "[0-9]+\.[0-9]+")
    n_reads=$(wc -l < "rids/${name}_rids.list")
    blow_size=$(du "out/${name}_reads.blow5" | cut -f1)

    echo -n "${name}, " >> $OUTPUT 
    echo -n "${sam_load_time}, " >> $OUTPUT 
    echo -n "${total_s5curl_time}, " >> $OUTPUT 
    echo -n "${header_load_time}, " >> $OUTPUT
    echo -n "${index_load_time}, " >> $OUTPUT
    echo -n "${fetch_time}, " >> $OUTPUT
    echo -n "${encode_time}, " >> $OUTPUT
    echo -n "${write_time}, " >> $OUTPUT
    echo -n "${n_reads}, " >> $OUTPUT
    echo -n "${blow_size}, " >> $OUTPUT
    echo "${basecall_time} " >> $OUTPUT
    
    echo "done for $name"
}

echo "Genome, Total SAM Time (sec), Total SLOW5CURL Time (sec), Header Load Time (sec), Index Load Time (sec), Fetch Time (sec), Encode Time (sec), Write Time (sec), Number of Reads, Extracted File Size (bytes), Total Basecall Time (sec)" > $OUTPUT 

insert_entry HG00735_1
insert_entry HG00735_3
insert_entry HG00735_5

insert_entry HG00438_1
insert_entry HG00438_3
insert_entry HG00438_5
insert_entry HG00621_1
insert_entry HG00621_3
insert_entry HG00621_5
insert_entry HG00673_1
insert_entry HG00673_3
insert_entry HG00673_5

insert_entry HG00741_1
insert_entry HG00741_3
insert_entry HG00741_5
insert_entry HG01071_1
insert_entry HG01071_3
insert_entry HG01071_5
insert_entry HG01106_1
insert_entry HG01106_3
insert_entry HG01106_5
insert_entry HG01123_1
insert_entry HG01123_3
insert_entry HG01123_5
insert_entry HG01175_1


insert_entry HG01175_3
insert_entry HG01175_5
insert_entry HG01258_1
insert_entry HG01258_3
insert_entry HG01258_5
insert_entry HG01358_1
insert_entry HG01358_3
insert_entry HG01358_5
insert_entry HG01361_1
insert_entry HG01361_3
insert_entry HG01361_5
insert_entry HG01891_2
insert_entry HG01891_3

insert_entry HG01891_4
insert_entry HG01928_1
insert_entry HG01928_3
insert_entry HG01928_5
insert_entry HG01952_1
insert_entry HG01952_3
insert_entry HG01952_5
insert_entry HG01978_1
insert_entry HG01978_3
insert_entry HG01978_5
insert_entry HG02148_1
insert_entry HG02148_3

insert_entry HG02148_5
insert_entry HG02257_1
insert_entry HG02257_3
insert_entry HG02257_5
insert_entry HG02486_1
insert_entry HG02486_2
insert_entry HG02486_3
insert_entry HG02559_1
insert_entry HG02559_2
insert_entry HG02559_3
insert_entry HG02559_5
insert_entry HG02572_1
insert_entry HG02572_3
insert_entry HG02572_5
insert_entry HG02622_1

insert_entry HG02622_3
insert_entry HG02622_5
insert_entry HG02630_1
insert_entry HG02630_3
insert_entry HG02630_5
insert_entry HG02717_1
insert_entry HG02717_3
insert_entry HG02717_5
insert_entry HG02886_1
insert_entry HG02886_3
insert_entry HG02886_5
insert_entry HG03453_1
insert_entry HG03453_3
insert_entry HG03453_5
insert_entry HG03471_1

insert_entry HG03471_3
insert_entry HG03471_5
insert_entry HG03516_1
insert_entry HG03516_3
insert_entry HG03516_5
insert_entry HG03540_1
insert_entry HG03540_3
insert_entry HG03540_5
insert_entry HG03579_1
insert_entry HG03579_3
insert_entry HG03579_5
