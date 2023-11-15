# slow5curl

slow5curl is a command line tool and a library and for fetching reads from remote BLOW5 files, which is built on top of [slow5lib](https://github.com/hasindu2008/slow5lib) and [libcurl](https://curl.se/libcurl/). This project is still under active development, and currently the tool and the C API is available. Python API is under construction.

[![CI](https://github.com/BonsonW/slow5curl/actions/workflows/c-cpp.yml/badge.svg)](https://github.com/BonsonW/slow5curl/actions/workflows/c-cpp.yml)

Full documentation: https://BonsonW.github.io/slow5curl<br/>
Publication (SLOW5 format): https://www.nature.com/articles/s41587-021-01147-4<br/>
SLOW5 specification: https://hasindu2008.github.io/slow5specs<br/>
SLOW5 ecosystem: https://hasindu2008.github.io/slow5<br/>

## Quick Start

## Table of Contents

- [Quick start](#quick-start)
- [Building](#building)
    - [Building A Release](#building-a-release)
    - [Building From GitHub](#building-from-github)
    - [Other Building Options](#other-building-options)
- [Tool](#tool)
    - [Tool Examples](#tool-examples)
    - [Troubleshooting/Questions](#troubleshootingquestions)
- [Library](#library)
- [Citation](#citation)

## Building

### Building A Release

todo

### Building From GitHub

**WARNING: Building from GitHub is meant for advanced users to test the latest features. For production purposes, use the latest release version that is thoroughly tested.**

To build from GitHub:

```sh
sudo apt-get install zlib1g-dev libcurl4-openssl-dev #install zlib and libcurl development libraries
git clone --recursive https://github.com/BonsonW/slow5curl
cd slow5curl
make
```

The commands to install libcurl and zlib __development libraries__ on some popular distributions :
```sh
On Debian/Ubuntu : sudo apt-get install libcurl4-openssl-dev zlib1g-dev
On Fedora/CentOS : sudo dnf/yum install libcurl-devel zlib-devel
On OS X : brew install curl zlib
```

### Other Building Options

#### Optional zstd compression

If you want to access BLOW5 files compressed with *zstd*, you can optionally enable [*zstd* compression](https://facebook.github.io/zstd) support in slow5lib when building *slow5curl* by invoking `make zstd=1`. This requires __zstd 1.3 or higher development libraries__ installed on your system:

```sh
On Debian/Ubuntu : sudo apt-get install libzstd1-dev # libzstd-dev on newer distributions if libzstd1-dev is unavailable
On Fedora/CentOS : sudo yum libzstd-devel
On OS X : brew install zstd
```

## Tool

[Manual](https://bonsonw.github.io/slow5curl/commands.html): Commands and options.

[One-Liners](https://bonsonw.github.io/slow5curl/oneliners.html): Example bash one-liners.

[Workflows](https://bonsonw.github.io/slow5curl/workflows.html): Example bash workflows.

### Tool Examples

```sh
# fetch slow5 reads one at a time
slow5curl get [OPTIONS] https://url/to/file.blow5 readid1 readid2 ....

# fetch slow5 reads, a batch at a time
slow5curl get [OPTIONS] https://url/to/file.blow5 --list readids.txt

# use a custom slow5 index
slow5curl get [OPTIONS] https://url/to/file.blow5 --index https://url/to/file.blow5.idx --list readids.txt

# use a custom slow5 index which is available locally
slow5curl get [OPTIONS] https://url/to/file.blow5 --index /path/to/file.blow5.idx --list readids.txt
```

An actual example:

```sh
slow5curl get https://gtgseq.s3.amazonaws.com/ont-r10-dna/NA24385/raw/PGXX22394_reads.blow5 05ef1592-a969-4eb8-b917-44ca536bec36 -o read.blow5
```

### Troubleshooting/Questions

Open an [Issue](https://github.com/BonsonW/slow5curl/issues).

## Library

Simply include `<slow5curl/s5curl.h>` in your C program and call the API functions. To compile your program and statically link against slow5curl:

```sh
gcc [OPTIONS] -I path/to/slow5curl/include your_program.c path/to/slow5curl/lib/libslow5curl.a -lm -lz -lpthread
```

path/to/slow5curl/ is the absolute or relative path to the slow5curl repository cloned above.

If you compiled slow5curl with zstd support enabled, make sure you append `-lzstd`` to the above two commands.

[C API](https://bonsonw.github.io/slow5curl/slow5curl_api/slow5curl): Full documentation of the C API.

[Python API](): Full documentation of the Python API.

[C API Examples](https://github.com/BonsonW/slow5curl/tree/master/examples): Main features of the C API and how to use them.

[Python API Examples](): Main features of the Python API and how to use them.

## Citation

Please cite the following in your publications when using *SLOW5* file format:

> Gamaarachchi, H., Samarakoon, H., Jenner, S.P. et al. Fast nanopore sequencing data analysis with SLOW5. Nat Biotechnol 40, 1026-1029 (2022). https://doi.org/10.1038/s41587-021-01147-4

```
@article{gamaarachchi2022fast,
  title={Fast nanopore sequencing data analysis with SLOW5},
  author={Gamaarachchi, Hasindu and Samarakoon, Hiruna and Jenner, Sasha P and Ferguson, James M and Amos, Timothy G and Hammond, Jillian M and Saadat, Hassaan and Smith, Martin A and Parameswaran, Sri and Deveson, Ira W},
  journal={Nature biotechnology},
  pages={1--4},
  year={2022},
  publisher={Nature Publishing Group}
}
```
