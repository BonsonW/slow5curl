# s5curl_get_batch

## NAME

s5curl_get_batch - fetches a list of record from a remote BLOW5 file corresponding to given read IDs

## SYNOPSYS

`int s5curl_get_batch(s5curl_mt_t *core, slow5_batch_t *db, char **rid, int num_rid)`

## DESCRIPTION

`s5curl_get_batch()` fetches and decodes a record from a remote BLOW5 file *s5c* for a specified *read_id* into a *slow5_rec_t* and stores it in **record*.

The argument *rid* points to an array of read identifier strings.

The argument *num_rid* corresponds with the number of read IDs passed in the method.

**records* should have the appropriate memory allocated before `s5curl_get_batch()` is called. Each *slow5_rec_t* in **records* should be freed by the user program using `slow5_rec_free()`.

The argument *s5c* points to a *slow5_file_t* opened using `s5curl_open()`. `s5curl_get_batch()` requires the SLOW index to be pre-loaded to *s5c* using `s5curl_idx_load()` or `s5curl_idx_load_with()`.

The argument *core* points to an initialized *s5curl_mt_t*. This is done with `s5curl_init_mt`.

The argument *db* points to an initialized *slow5_batch_t*. This is done with `slow5_init_batch`.

## RETURN VALUE

Upon successful completion, `s5curl_get_batch()` returns a non negative integer (>=0). Otherwise, a negative value is returned that indicates the error and `slow5_errno` is set to indicate the error.

## ERRORS

* `SLOW5_ERR_NOTFOUND`  
    &nbsp;&nbsp;&nbsp;&nbsp; Read_id was not found in the index.
* `SLOW5_ERR_ARG`       
    &nbsp;&nbsp;&nbsp;&nbsp; Invalid argument - read_id, read or s5c is NULL.
* `SLOW5_ERR_RECPARSE`  
    &nbsp;&nbsp;&nbsp;&nbsp; Record parsing error.
* `SLOW5_ERR_NOIDX`     
    &nbsp;&nbsp;&nbsp;&nbsp; The index has not been loaded.
* `SLOW5_ERR_IO`        
    &nbsp;&nbsp;&nbsp;&nbsp; I/O error when reading the slow5 file, for instance, `pread()`failed.
* `SLOW5_ERR_MEM`        
    &nbsp;&nbsp;&nbsp;&nbsp; Memory allocation error.
* `SLOW5_ERR_UNK`        
    &nbsp;&nbsp;&nbsp;&nbsp; Slow5 file format is unknown.
* `SLOW5_ERR_PRESS`      
    &nbsp;&nbsp;&nbsp;&nbsp; Decompression error.

## NOTES

## EXAMPLES
```
#include <stdio.h>
#include <stdlib.h>
#include <slow5curl/s5curl.h>
#include <curl/curl.h>

#define URL "https://example.blow5"
#define MAX_CONNECTS 100

int main () {

    curl_global_init(CURL_GLOBAL_ALL);

    s5curl_mt_t *multi = s5curl_init_mt(MAX_CONNECTS);
    if (!multi) {
        fprintf(stderr, "Error opening connections.\n");
        return EXIT_FAILURE;
    }

    s5curl_t *s5c = s5curl_open(URL);
    if (s5c == NULL) {
       fprintf(stderr, "Error fetching slow5 file\n");
       exit(EXIT_FAILURE);
    }

    ret = s5curl_idx_load(s5c);
    if (ret < 0) {
        fprintf(stderr, "Error in loading index\n");
        exit(EXIT_FAILURE);
    }

    ret = s5curl_get_batch(*s5c, multi, MAX_CONNECTS, N_READS, read_ids, records);

    //...

    s5curl_idx_unload(s5c);

    s5curl_close(s5c);

    s5curl_free_mt(multi);

    curl_global_cleanup();
}
```

## SEE ALSO
[s5curl_init_mt()](s5curl_init_mt.md)