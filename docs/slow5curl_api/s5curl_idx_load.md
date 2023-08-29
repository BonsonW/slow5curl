# s5curl_idx_load

## NAME
s5curl_idx_load - fetches and loads the index file for a remote SLOW5 file

## SYNOPSYS
`int s5curl_idx_load(slow5_curl_t *s5c)`

## DESCRIPTION
`s5curl_idx_load()` fetches and loads an index file for a remote SLOW5 file pointed by *s5c* to its URL and associates the index with *s5c*.

`s5curl_idx_load()` should be called successfully before using `s5curl_get()` or `s5curl_get_batch()`.

## RETURN VALUE
Upon successful completion, `s5curl_idx_load()` returns a non-negative integer. Otherwise, a negative value is returned.

## NOTES
`slow5_errno` will be set in future to indicate the type of error that occurred.

## EXAMPLES
```
#include <stdio.h>
#include <stdlib.h>
#include <slow5curl/s5curl.h>
#include <curl/curl.h>

#define URL "https://example.blow5"

int main () {

    curl_global_init(CURL_GLOBAL_ALL);

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

    //...

    s5curl_idx_unload(s5c);

    s5curl_close(s5c);

    curl_global_cleanup();
}
```

## SEE ALSO

[s5curl_idx_unload()](s5curl_idx_unload.md), [s5curl_idx_load_with()](s5curl_idx_load_with.md)