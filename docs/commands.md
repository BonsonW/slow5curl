# Commands and Options

## COMMANDS

* `get`:<br/>
    Retrieve records for specified read IDs from a remote BLOW5 file URL.

* `head`:<br/>
    Print header information from a remote BLOW5 file URL.

* `reads`:<br/>
    Print list of all read IDs from a remote BLOW5 file URL.

### get

Retrieve records for specified read IDs from a remote BLOW5 file URL.

```
slow5curl get [OPTIONS] https://url/to/file1.blow5 readid1 readid2 ....
slow5curl get [OPTIONS] https://url/to/file1.blow5 --list readids.txt
```

*  `--to format_type`:<br/>
    Specifies the format of output files. `format_type` can be `slow5` for SLOW5 ASCII or `blow5` for SLOW5 binary (BLOW5) [default value: blow5].
*  `-o FILE`, `--output FILE`:<br/>
    Outputs data to FILE [default value: stdout]. This can auto detect the output format from the file extension.
*  `-c, --compress compression_type`:<br/>
    Specifies the compression method used for BLOW5 output. `compression_type` can be `none` for uncompressed binary; `zlib` for zlib-based (also known as gzip or DEFLATE) compression; or `zstd` for Z-standard-based compression [default value: zlib]. This option is only valid for BLOW5. `zstd` will only function if slow5curl has been built with zstd support which is turned off by default.
*  `-s, --sig-compress compression_type`:<br/>
    Specifies the raw signal compression method used for BLOW5 output. `compression_type` can be `none` for uncompressed raw signal or `svb-zd` to compress the raw signal using StreamVByte zig-zag delta [default value: svb-zd]. Note that record compression (-c option above) is still applied on top of the compressed signal. Signal compression with svb-zd and record compression with zstd is similar to ONT's vbz.  zstd+svb-zd offers slightly smaller file size and slightly better performance compared to the default zlib+svb-zd, however, will be less portable.
* `-t, --threads INT`:<br/>
    Number of threads (connections) [default value: 8].
* `-K, --batchsize`:<br/>
    The batch size. This is the number of records on the memory at once [default value: 4096]. An increased batch size improves multi-threaded performance at cost of higher RAM.
* `-l, --list FILE`:<br/>
    List of read ids provided as a single-column text file with one read id per line.
* `--index FILE`:<br/>
    URL to a custom slow5 index. This can also be a path to an index that is residing locally on your disk. Useful if your index file is located somewhere other https://url/file.blow5.idx when your input file is https://url/file.blow5.
*  `-h`, `--help`:<br/>
    Prints the help menu.

### head

Print header information from a remote BLOW5 file URL.

```
slow5curl head https://url/to/file1.blow5
```

### reads

Print the list of read IDs from a remote BLOW5 file URL.

```
slow5curl reads https://url/to/file1.blow5
```

## GLOBAL OPTIONS

*  `-V, --version`:<br/>
    Print the slow5curl version number.
*  `-v INT, --verbose INT`:<br/>
    Set the verbosity level (0-6) [default value: 4]. 0-off, 1-errors, 2-warnings, 3-information, 4-verbose, 5-debug, 6-trace.
*  `-h, --help`:<br/>
    Prints the help menu.
*  `--cite`:<br/>
    Prints the citation information.

## LIBRARY

todo