# slow5curl Examples

This directory contains following examples.
- *get_single_read.c* shows how to use slow5curl to fetch a single record.
- *get_multi_read.c* shows how to use slow5curl to multiple records.

You can invoke [build.sh](build.sh) from slow5lib directory as `examples/build.sh` to compile the example programmes. Have a look at the script to see the commands used for compiling and linking. As an example, the command to compile [get_single_read.c](get_single_read.c) is `gcc -Wall -O2 -I include/ -I slow5lib/include/ examples/get_single_read.c libslow5curl.a -lcurl -lz`. Make sure that you first call `make` so that `libslow5curl.a` becomes available to be linked with. If you compiled *slow5curl* with *zstd* support enabled (`make zstd=1`), make sure you append `-lzstd` to the compilation commands.
