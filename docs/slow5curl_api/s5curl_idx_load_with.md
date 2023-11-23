# s5curl_idx_load_with

## NAME
s5curl_idx_load_with - loads a custom index file for a remote BLOW5 file

## SYNOPSYS
`int s5curl_idx_load_with(s5curl_t *s5c, const char *path)`

## DESCRIPTION
`s5curl_idx_load_with()` loads a custom index file for a remote BLOW5 file pointed by *s5c* to its URL and associates the index with *s5c*. You may pass in either a local index file path, or a URL to an index file.

`s5curl_idx_load_with()` should be called successfully before using `s5curl_get()` or `s5curl_get_batch()`.

## RETURN VALUE
Upon successful completion, `s5curl_idx_load_with()` returns a non negative integer (>=0). Otherwise, a negative value is returned that indicates the error. If the error occured with SLOW5 then `slow5_errno` is set to indicate the error.

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

## EXAMPLES
```c
#include <slow5curl/s5curl.h>

#define URL "https://example.blow5"
#define IDX_LOCAL "example/example.blow5.idx"

int main () {

    // setup

    s5curl_t *s5c = s5curl_open(URL);
    
    ret = s5curl_idx_load_with(s5c, IDX_LOCAL);
    if (ret < 0) {
        fprintf(stderr, "Error in loading index\n");
        exit(EXIT_FAILURE);
    }

    // s5curl operations

    s5curl_idx_unload(s5c);

    s5curl_close(s5c);

    // cleanup
}
```

## SEE ALSO
[s5curl_idx_unload()](s5curl_idx_unload.md), [s5curl_idx_load_with()](s5curl_idx_load_with.md)