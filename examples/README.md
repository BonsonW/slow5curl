# Examples

These examples demonstrate the main features of the C API and how to use them.

You can invoke [build](../examples/build.sh) from slow5curl directory as **examples/build.sh** to compile the example programmes. Have a look at the script to see the commands used for compiling and linking.

As an example, the command to compile [get_single_read](../examples/get_single_read.c) is:
```sh
gcc -Wall -O2 -I include/ -I slow5lib/include/ examples/get_single_read.c lib/libslow5curl.a -lcurl -lz lpthread
```
Make sure that you first call make so that libslow5curl.a becomes available to be linked with.

If you compiled slow5curl with zstd support enabled (make zstd=1), make sure you append -lzstd to the compilation commands.

Example | Description
--- | ---
[get_single_read](../examples/get_single_read.c) | fetch a single read from a remote blow5 file
[get_multi_read](../examples/get_multi_read.c) | fetch a list of reads from a remote blow5 file using multiple threads
[with_custom_local_index](../examples/with_custom_local_index.c) | fetch a single read with a custom local index file
[with_custom_remote_index](../examples/with_custom_remote_index.c) | fetch a single read with a custom remote index file
