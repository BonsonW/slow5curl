#!/bin/sh

# exit on error
set -x
# prints the command to the console
set -e

gcc -Wall -O2 -I include/ -I slow5lib/include/ examples/get_single_read.c libslow5curl.a -lcurl -lz
gcc -Wall -O2 -I include/ -I slow5lib/include/ examples/get_multi_read.c libslow5curl.a -lcurl -lz

# append -lzstd to above commands if your slow5lib is built with zstd support