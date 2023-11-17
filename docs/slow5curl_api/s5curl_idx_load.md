# s5curl_idx_load

## NAME
s5curl_idx_load - fetches and loads the index file for a remote BLOW5 file

## SYNOPSYS
`int s5curl_idx_load(s5curl_t *s5c)`

## DESCRIPTION
`s5curl_idx_load()` fetches and loads an index file for a remote BLOW5 file pointed by *s5c* to its URL and associates the index with *s5c*.

`s5curl_idx_load()` should be called successfully before using `s5curl_get()` or `s5curl_get_batch()`.

## RETURN VALUE
Upon successful completion, `s5curl_idx_load()` returns a non negative integer (>=0). Otherwise, a negative value is returned that indicates the error. If the error occured with SLOW5 then `slow5_errno` is set to indicate the error.

## ERRORS
* `S5CURL_ERR_SLOW5`       
    &nbsp;&nbsp;&nbsp;&nbsp; Failed to parse SLOW5 data.
* `S5CURL_ERR_CURL`
    &nbsp;&nbsp;&nbsp;&nbsp; Failed to initialise CURL handle.
* `S5CURL_ERR_FETCH`
    &nbsp;&nbsp;&nbsp;&nbsp; Fetching data failed.
* `S5CURL_ERR_ARG`
    &nbsp;&nbsp;&nbsp;&nbsp; Invalid argument.
* `S5CURL_ERR_IO`
    &nbsp;&nbsp;&nbsp;&nbsp; Other IO error.
* `S5CURL_ERR_MEM`
    &nbsp;&nbsp;&nbsp;&nbsp; Memory (re)allocation error.
* `S5CURL_ERR_VERSION`
    &nbsp;&nbsp;&nbsp;&nbsp; Version incompatible.

## NOTES
`slow5_errno` will be set in future to indicate the type of error that occurred.

## EXAMPLES
```c
#include <stdio.h>
#include <stdlib.h>
#include <slow5curl/s5curl.h>

#define URL "https://example.blow5"

int main () {

    s5curl_global_init();

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

    //...

    s5curl_idx_unload(s5c);

    s5curl_close(s5c);

    s5curl_global_cleanup();
}
```

## SEE ALSO
[s5curl_idx_unload()](s5curl_idx_unload.md), [s5curl_idx_load_with()](s5curl_idx_load_with.md)