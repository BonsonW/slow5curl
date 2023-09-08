# slow5curl

Slow5curl is a library and toolkit for fetching reads from remote SLOW5 files. This project is still under heavy development, only the C API is available.

**About SLOW5 format:**<br/>
SLOW5 is a new file format for storing signal data from Oxford Nanopore Technologies (ONT) devices. SLOW5 was developed to overcome inherent limitations in the standard FAST5 signal data format that prevent efficient, scalable analysis and cause many headaches for developers. SLOW5 can be encoded in human-readable ASCII format, or a more compact and efficient binary format (BLOW5) - this is analogous to the seminal SAM/BAM format for storing DNA sequence alignments. The BLOW5 binary format supports  *zlib* (DEFLATE) compression, or other compression methods (see [notes](https://github.com/hasindu2008/slow5tools#notes)), thereby minimising the data storage footprint while still permitting efficient parallel access. Detailed benchmarking experiments have shown that SLOW5 format is an order of magnitude faster and significantly smaller than FAST5.

Full documentation: https://hasindu2008.github.io/slow5tools<br/>
<!-- Pre-print: https://www.biorxiv.org/content/10.1101/2021.06.29.450255v1<br/> -->
Publication (SLOW5 format): https://www.nature.com/articles/s41587-021-01147-4<br/>
SLOW5 specification: https://hasindu2008.github.io/slow5specs<br/>
slow5 ecosystem: https://hasindu2008.github.io/slow5<br/>

To convert to and from ONT's new POD5 format, you use [blue_crab](https://github.com/Psy-Fer/blue-crab). If POD5 format and the associated POD5 C/C++ API reaches maturity/stability and adheres to C++11 standard, capabilities for POD5 <-> SLOW5 conversion will be added to slow5tools. slow5tools is strictly adhering to C++11 standard for wider compatibility.

## Quick start

## Table of Contents

- [Quick start](#quick-start)
- [Building](#building)
    - [Building a release](#building-a-release)
    - [Building from GitHub](#building-from-github)
    - [Other building options](#other-building-options)
- [Tool Usage](#tool-usage)
    - [Tool Examples](#tool-examples)
    - [Troubleshooting/Questions](#troubleshootingquestions)
- [Library Usage](#library-usage)
    - [Library Examples](#library-examples)
- [Notes](#notes)
- [Acknowledgement](#acknowledgement)
- [Citation](#citation)

## Building

### Building a release

### Building from GitHub

**WARNING: Building from GitHub is meant for advanced users to test latest features. For production purposes, use the latest release version that is thoroughly tested.**

To build from GitHub:

```
sudo apt-get install zlib1g-dev libcurl4-openssl-dev
git clone --recursive https://github.com/BonsonW/slow5curl
cd slow5curl
make
```

### Other building options

## Tool Usage

* Visit the [man page](https://hasindu2008.github.io/slow5tools/commands.html) for all the commands and options.
* A guide on using BLOW5 for archiving and steps to verify if data integrity is preserved is [here](https://hasindu2008.github.io/slow5tools/archive.html).
* A script for performing real-time FAST5 to BLOW5 conversion during sequencing is provided [here](https://github.com/hasindu2008/slow5tools/tree/master/scripts/realtime-f2s).

### Tool Examples

```sh
# fetch slow5 reads one at a time
slow5curl get [OPTIONS] BLOW5_URL readid1 readid2 ....
# fetch slow5 reads by batch
slow5curl get [OPTIONS] BLOW5_URL --list readids.txt
```

Visit [here](https://hasindu2008.github.io/slow5tools/workflows.html) for example workflows. See [here](https://hasindu2008.github.io/slow5tools/oneliners.html) for example bash one-liners with slow5tools.


### Troubleshooting/Questions

Visit the [frequently asked questions](https://hasindu2008.github.io/slow5tools/faq.html) or open an [issue](https://github.com/hasindu2008/slow5tools/issues).


## Library Usage

Simply include `<slow5curl/s5curl.h>` in your C program and call the API functions.

For the documentation of the C API visit [here](https://hasindu2008.github.io/slow5lib/slow5_api/slow5.html) and for the Python API visit [here](https://hasindu2008.github.io/slow5lib/pyslow5_api/pyslow5.html).

### Library Examples

Examples are provided under [examples](https://github.com/BonsonW/slow5curl/tree/master/examples).
- *get_multi_read.c* demonstrates how to fetch a list of reads from a remote blow5 file.
- *get_single_read.c* demonstrates how to fetch a singlefrom a remote blow5 file.

## Notes

*slow5lib* from version 0.3.0 onwards has built in [StreamVByte](https://github.com/lemire/streamvbyte) compression support to enable even smaller file sizes, which is applied to the raw signal by default when producing BLOW5 files.  *zlib* compression is then applied by default to each record. If *zstd* is used instead of *zlib* on top of *StreamVByte*, it is similar to ONT's latest [vbz](https://github.com/nanoporetech/vbz_compression) compression. BLOW5 files compressed with *zstd+StreamVByte* are still significantly smaller than vbz compressed FAST5 files.

## Acknowledgement

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
