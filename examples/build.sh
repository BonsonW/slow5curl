#!/bin/sh

BUILD_DIR="examples/build/"

# exit on error
set -x
# prints the command to the console
set -e

gcc -Wall -O2 -I include/ -I slow5lib/include/ examples/get_single_read.c lib/libslow5curl.a -o ${BUILD_DIR}/get_single_read -lcurl -lz
gcc -Wall -O2 -I include/ -I slow5lib/include/ examples/get_multi_read.c lib/libslow5curl.a -o ${BUILD_DIR}/get_multi_read -lcurl -lz -lpthread
gcc -Wall -O2 -I include/ -I slow5lib/include/ examples/get_single_read.c lib/libslow5curl.a -o ${BUILD_DIR}/get_custom_local_read -lcurl -lz
gcc -Wall -O2 -I include/ -I slow5lib/include/ examples/get_single_read.c lib/libslow5curl.a -o ${BUILD_DIR}/get_custom_remote_read -lcurl -lz

# append -lzstd to above commands if your slow5lib is built with zstd support