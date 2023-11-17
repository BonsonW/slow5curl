# slow5curl

slow5curl is a command line tool and a library and for fetching reads from remote BLOW5 files, which is built on top of [slow5lib](https://github.com/hasindu2008/slow5lib) and [libcurl](https://curl.se/libcurl/). See [slow5lib](https://github.com/hasindu2008/slow5lib) or [slow5tools](https://github.com/hasindu2008/slow5tools) for fully featured SLOW5 file manipulation; slow5curl is kept standalone for its specific use case and dependencies.


*This project is still under active development. Currently, the tool and the C API is available. Python API is under construction.*


[![GitHub Downloads](https://img.shields.io/github/downloads/BonsonW/slow5curl/total?logo=GitHub)](https://github.com/BonsonW/slow5curl/releases)
[![CI](https://github.com/BonsonW/slow5curl/actions/workflows/c-cpp.yml/badge.svg)](https://github.com/BonsonW/slow5curl/actions/workflows/c-cpp.yml)

Full documentation: https://BonsonW.github.io/slow5curl<br/>
Publication (SLOW5 format): https://www.nature.com/articles/s41587-021-01147-4<br/>
SLOW5 specification: https://hasindu2008.github.io/slow5specs<br/>
SLOW5 ecosystem: https://hasindu2008.github.io/slow5<br/>

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
- [Acknowledgement](#acknowledgement)
- [Citation](#citation)

## Quick Start

If you are a Linux user on x86_64 architecture and want to quickly try slow5curl out, download the compiled binaries from the [latest release](https://github.com/BonsonW/slow5curl/releases). Binaries should work on most Linux distributions as long as the `curl` and `zlib` runtime libraries are available. You can install `curl` using `apt-get install curl` on Ubuntu. `zlib` is typically available by default on most Linux distributions. For compiled binaries to work, your processor must support SSSE3 instructions or higher (processors after 2007 have these) and your operating system must have GLIBC 2.17 or higher (Linux distributions from 2014 onwards typically have this). 

```sh
sudo apt-get install curl # curl runtime library on Ubuntu (CentOS have this by default)
VERSION=v0.1.1
wget "https://github.com/BonsonW/slow5curl/releases/download/$VERSION/slow5curl-$VERSION-x86_64-linux-binaries.tar.gz" && tar xvf slow5curl-$VERSION-x86_64-linux-binaries.tar.gz && cd slow5curl-$VERSION/
./slow5curl
```

## Building

### Building A Release
Users are recommended to build from the  [latest release](https://github.com/BonsonW/slow5curl/releases) tar ball. A compiler that supports C99 is needed to build slow5curl.

Quick example for Ubuntu:

```sh
sudo apt-get install zlib1g-dev libcurl4-openssl-dev # install zlib and libcurl development libraries
VERSION=v0.1.1
wget "https://github.com/BonsonW/slow5curl/releases/download/$VERSION/slow5curl-$VERSION-release.tar.gz" && tar xvf slow5curl-$VERSION-release.tar.gz && cd slow5curl-$VERSION/
make
```
The commands to install libcurl and zlib __development libraries__ on some popular distributions:
```sh
On Debian/Ubuntu: sudo apt-get install libcurl4-openssl-dev zlib1g-dev
On Fedora/CentOS: sudo dnf/yum install libcurl-devel zlib-devel
On OS X: brew install curl zlib
```


### Building From GitHub

To build from GitHub:

```sh
sudo apt-get install zlib1g-dev libcurl4-openssl-dev # install zlib and libcurl development libraries
git clone --recursive https://github.com/BonsonW/slow5curl
cd slow5curl
make
```

The commands to install libcurl and zlib __development libraries__ on some popular distributions:
```sh
On Debian/Ubuntu: sudo apt-get install libcurl4-openssl-dev zlib1g-dev
On Fedora/CentOS: sudo dnf/yum install libcurl-devel zlib-devel
On OS X: brew install curl zlib
```

### Other Building Options

#### Optional zstd compression

If you want to access BLOW5 files compressed with *zstd*, you can optionally enable [*zstd* compression](https://facebook.github.io/zstd) support in slow5lib when building *slow5curl* by invoking `make zstd=1`. This requires __zstd 1.3 or higher development libraries__ installed on your system:

```sh
On Debian/Ubuntu: sudo apt-get install libzstd1-dev # libzstd-dev on newer distributions if libzstd1-dev is unavailable
On Fedora/CentOS: sudo yum libzstd-devel
On OS X: brew install zstd
```

## Tool

[Manual](https://bonsonw.github.io/slow5curl/commands.html): Commands and options.

[One-Liners](https://bonsonw.github.io/slow5curl/oneliners.html): Example bash one-liners.

[Workflows](https://bonsonw.github.io/slow5curl/workflows.html): Example bash workflows.

### Tool Examples

```sh
# fetch reads from a remote BLOW5, one at a time
slow5curl get https://url/to/file.blow5 readid1 readid2 ....

# fetch reads from a remote BLOW5, a batch at a time
slow5curl get https://url/to/file.blow5 --list readids.txt

# get a read from a remote BLOW5, caching the index
slow5curl get https://url/to/file.blow5 readid1 --cache /path/to/file.blow5.idx -o read.blow5

# use a custom BLOW5 index which is available locally
slow5curl get https://url/to/file.blow5 --index /path/to/file.blow5.idx --list readids.txt

# use a custom remote BLOW5 index
slow5curl get https://url/to/file.blow5 --index https://url/to/file.blow5.idx --list readids.txt

# get the header info of a remote BLOW5
slow5curl head https://url/to/file1.blow5

# get the list of read-ids of a remote BLOW5
slow5curl reads https://url/to/file1.blow5
```

Working examples:

```sh
# fetch from a small BLOW5
slow5curl get https://github.com/BonsonW/slow5curl/raw/main/examples/data/reads_10.blow5 000286ab-1f80-40e3-a778-8d89e4e52940 -o read.blow5

# fetch from a large BLOW5 while caching the indexing (note downloading the index will take some time)
slow5curl get https://gtgseq.s3.amazonaws.com/ont-r10-dna/NA24385/raw/PGXX22394_reads.blow5 05ef1592-a969-4eb8-b917-44ca536bec36  --cache /tmp/PGXX22394_reads.blow5.idx -o read.blow5

# fetch from a large BLOW5 with the cached index
slow5curl get https://gtgseq.s3.amazonaws.com/ont-r10-dna/NA24385/raw/PGXX22394_reads.blow5 05ef1592-a969-4eb8-b917-44ca536bec36 --index /tmp/PGXX22394_reads.blow5.idx -o read.blow5 
```

### Troubleshooting/Questions

Open an [Issue](https://github.com/BonsonW/slow5curl/issues).

## Library

Follow the steps to [build slow5curl from GitHub](https://github.com/BonsonW/slow5curl/edit/main/README.md#building-from-github). Include `<slow5curl/s5curl.h>` in your C program and call the API functions. To compile your program and statically link against slow5curl:

```sh
gcc [OPTIONS] -I path/to/slow5curl/include -I path/to/slow5curl/slow5lib/include your_program.c path/to/slow5curl/lib/libslow5curl.a -lcurl -lpthread -lz
```

*path/to/slow5curl/* is the absolute or relative path to the slow5curl repository cloned above.

If you compiled slow5curl with zstd support enabled, make sure you append `-lzstd` to the above command.

[C API](https://bonsonw.github.io/slow5curl/slow5curl_api/slow5curl): Full documentation of the C API.

<!--
[Python API](): Full documentation of the Python API.
-->

[C API Examples](https://github.com/BonsonW/slow5curl/tree/main/examples): Main features of the C API and how to use them.

<!--
[Python API Examples](): Main features of the Python API and how to use them.
-->

If you need any help with using the library please open an [Issue](https://github.com/BonsonW/slow5curl/issues).


## Acknowledgement

AWS Open Data Sponsorship Program for generously hosting [open data in BLOW5 format](https://github.com/GenTechGp/gtgseq), which greatly helped us to develop and test this tool.

## Citation

Please cite the following in your publications when using the *SLOW5* file format:

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
