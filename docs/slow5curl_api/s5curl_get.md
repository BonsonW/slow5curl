# s5curl_get

## NAME

s5curl_get - fetches a record from a remote SLOW5 file corresponding to a given read ID

## SYNOPSYS

`int s5curl_get(slow5_curl_t *s5c, CURL *curl, const char *read_id, slow5_rec_t **record)`

## DESCRIPTION

`s5curl_get()` fetches and decodes a record from a remote SLOW5 file *s5c* for a specified *read_id* into a *slow5_rec_t* and stores it in **record*.

The argument *read_id* points to a read identifier string.

If **record* is set to NULL before the call, then `s5curl_get()` will allocate a *slow5_rec_t* for storing the record. This *slow5_rec_t* should be freed by the user program using `slow5_rec_free()`.

The argument *s5c* points to a *slow5_file_t* opened using `s5curl_open()`. `s5curl_get()` requires the SLOW index to be pre-loaded to *s5c* using `s5curl_idx_load()` or `s5curl_idx_load_with()`.

The argument *curl* points to a *CURL* handle.

`s5curl_get()` can be called by multiple threads in parallel on the same *slow5_file_t* pointer, however it must have a different *CURL* handle.

## RETURN VALUE

Upon successful completion, `s5curl_get()` returns a non negative integer (>=0). Otherwise, a negative value is returned that indicates the error and `slow5_errno` is set to indicate the error.

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
#define READ_ID "0032812b-1ea5-46f1-a844-5bcc3bf3c21f"

int main () {

    curl_global_init(CURL_GLOBAL_ALL);

    CURL *curl = curl_easy_init();

    slow5_curl_t *s5c = s5curl_open(URL);
    if (s5c == NULL) {
       fprintf(stderr, "Error fetching slow5 file\n");
       exit(EXIT_FAILURE);
    }

    ret = s5curl_idx_load(s5c);
    if (ret < 0) {
        fprintf(stderr, "Error in loading index\n");
        exit(EXIT_FAILURE);
    }

    ret = s5curl_get(s5c, curl, READ_ID, &rec);
    if (ret < 0) {
        fprintf(stderr, "Error in when fetching the read\n");
    }

    //...

    s5curl_idx_unload(s5c);

    s5curl_close(s5c);

    curl_easy_cleanup(curl);

    curl_global_cleanup();
}
```

## SEE ALSO
