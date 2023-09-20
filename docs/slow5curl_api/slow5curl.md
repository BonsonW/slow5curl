# slow5curl

**Note: This documentation is far from being complete and perfect. So if you need a new functionality, want to clarify something, find something is not right, facing a weird bug when using this API etc. do not hesitate to open an [issue](https://github.com/BonsonW/slow5curl/issues). Nowadays, bioinformatics tools written in C/C++ are getting fewer and fewer, so I would be glad to help.**

## NAME
slow5curl - Overview

## DESCRIPTION
slow5curl is a library for fetching records from remote BLOW5 files. Compiling slow5curl requires a C compiler that conforms to at least c99 standard with X/Open 7, incorporating POSIX 2008 extension support.

### Data Structures
The *s5curl_t* structure stores the url and the metadata of the remote BLOW5 file. This structure has the following form:

```
typedef struct {

  char *url;                      // URL to the BLOW5 file

  /* private struct members that are not supposed to be directly accessed are not shown.
      the order of the memebers in this struct can subject to change.
  */

} s5curl_t;
```

The *s5curl_mt_t* is a struct intended to be used by multi-threaded fetches. This structure has the following form:

```
typedef struct {

  int32_t num_thread;                      // number of threads dispatched pre batch call
  /* private struct members that are not supposed to be directly accessed are not shown.
      the order of the memebers in this struct can subject to change.
  */

} s5curl_mt_t;
```

### High-level API
High-level API consists of following functions:

#### Resource Initialization
* [s5curl_open](s5curl_open.md)<br/>
  &nbsp;&nbsp;&nbsp;&nbsp;opens a remote BLOW5 file
* [s5curl_close](s5curl_close.md)<br/>
  &nbsp;&nbsp;&nbsp;&nbsp;closes a remote BLOW5 file
* [s5curl_idx_load](s5curl_idx_load.md)<br/>
  &nbsp;&nbsp;&nbsp;&nbsp;fetches and loads the index file for a remote BLOW5 file
* [s5curl_idx_load_with](s5curl_idx_load_with.md)<br/>
  &nbsp;&nbsp;&nbsp;&nbsp;loads a custom index (remote or local) for a remote BLOW5 file
* [s5curl_idx_unload](s5curl_idx_unload.md)<br/>
  &nbsp;&nbsp;&nbsp;&nbsp;unloads a BLOW5 index from the memory
* [s5curl_init_mt](s5curl_init_mt.md)<br/>
  &nbsp;&nbsp;&nbsp;&nbsp;initializes a multithread struct
* [s5curl_free_mt](s5curl_free_mt.md)<br/>
  &nbsp;&nbsp;&nbsp;&nbsp;frees a multithread struct from memory

### Fetching
* [s5curl_get](s5curl_get.md)<br/>
  &nbsp;&nbsp;&nbsp;&nbsp;fetches a record corresponding to a given read ID
* [s5curl_get_batch](s5curl_get_batch.md)<br/>
  &nbsp;&nbsp;&nbsp;&nbsp;fetches records corresponding to a list of read IDs using multiple threads

## CITATION
Please cite the following in your publications when using *slow5curl/pyslow5curl*:

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


<!--
### Low-level API for reading and writing SLOW5 files
* [slow5_open_with](low_level_api/slow5_open_with.md)
    Open a SLOW5 file. User can specify the SLOW5 format.

adding read groups
setting different compression

-->