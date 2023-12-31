#!/bin/bash

set -e
set -x

# terminate script
die() {
	echo "$1" >&2
	echo
	exit 1
}

BTEEL="/install/buttery-eel-buttery-eel-0.4.0+6.5.7/scripts/eel"
GUPPY="/install/ont-guppy-6.5.7/bin/"
CONFIG="dna_r9.4.1_450bps_sup_prom.cfg"
DEVICE="cuda:all"

TIME_BASECALL () {
    NAME=$1
	DIRNAME=$(echo ${NAME} | awk -F"_" '{print $1}')

    /usr/bin/time -v ${BTEEL} -g ${GUPPY} -x ${DEVICE} --config ${CONFIG} -i "out/${NAME}_reads.blow5" -o "out/${NAME}_reads.fastq" 2> logs/${NAME}_basecall.txt || die "Basecall failed. Exiting."

    echo "done for $NAME"
}

TIME_BASECALL HG00735_1
TIME_BASECALL HG00735_3
TIME_BASECALL HG00735_5

TIME_BASECALL HG00438_1
TIME_BASECALL HG00438_3
TIME_BASECALL HG00438_5
TIME_BASECALL HG00621_1
TIME_BASECALL HG00621_3
TIME_BASECALL HG00621_5
TIME_BASECALL HG00673_1
TIME_BASECALL HG00673_3
TIME_BASECALL HG00673_5

TIME_BASECALL HG00741_1
TIME_BASECALL HG00741_3
TIME_BASECALL HG00741_5
TIME_BASECALL HG01071_1
TIME_BASECALL HG01071_3
TIME_BASECALL HG01071_5
TIME_BASECALL HG01106_1
TIME_BASECALL HG01106_3
TIME_BASECALL HG01106_5
TIME_BASECALL HG01123_1
TIME_BASECALL HG01123_3
TIME_BASECALL HG01123_5
TIME_BASECALL HG01175_1


TIME_BASECALL HG01175_3
TIME_BASECALL HG01175_5
TIME_BASECALL HG01258_1
TIME_BASECALL HG01258_3
TIME_BASECALL HG01258_5
TIME_BASECALL HG01358_1
TIME_BASECALL HG01358_3
TIME_BASECALL HG01358_5
TIME_BASECALL HG01361_1
TIME_BASECALL HG01361_3
TIME_BASECALL HG01361_5
TIME_BASECALL HG01891_2
TIME_BASECALL HG01891_3

TIME_BASECALL HG01891_4
TIME_BASECALL HG01928_1
TIME_BASECALL HG01928_3
TIME_BASECALL HG01928_5
TIME_BASECALL HG01952_1
TIME_BASECALL HG01952_3
TIME_BASECALL HG01952_5
TIME_BASECALL HG01978_1
TIME_BASECALL HG01978_3
TIME_BASECALL HG01978_5
TIME_BASECALL HG02148_1
TIME_BASECALL HG02148_3

TIME_BASECALL HG02148_5
TIME_BASECALL HG02257_1
TIME_BASECALL HG02257_3
TIME_BASECALL HG02257_5
TIME_BASECALL HG02486_1
TIME_BASECALL HG02486_2
TIME_BASECALL HG02486_3
TIME_BASECALL HG02559_1
TIME_BASECALL HG02559_2
TIME_BASECALL HG02559_3
TIME_BASECALL HG02559_5
TIME_BASECALL HG02572_1
TIME_BASECALL HG02572_3
TIME_BASECALL HG02572_5
TIME_BASECALL HG02622_1

TIME_BASECALL HG02622_3
TIME_BASECALL HG02622_5
TIME_BASECALL HG02630_1
TIME_BASECALL HG02630_3
TIME_BASECALL HG02630_5
TIME_BASECALL HG02717_1
TIME_BASECALL HG02717_3
TIME_BASECALL HG02717_5
TIME_BASECALL HG02886_1
TIME_BASECALL HG02886_3
TIME_BASECALL HG02886_5
TIME_BASECALL HG03453_1
TIME_BASECALL HG03453_3
TIME_BASECALL HG03453_5
TIME_BASECALL HG03471_1

TIME_BASECALL HG03471_3
TIME_BASECALL HG03471_5
TIME_BASECALL HG03516_1
TIME_BASECALL HG03516_3
TIME_BASECALL HG03516_5
TIME_BASECALL HG03540_1
TIME_BASECALL HG03540_3
TIME_BASECALL HG03540_5
TIME_BASECALL HG03579_1
TIME_BASECALL HG03579_3
TIME_BASECALL HG03579_5
