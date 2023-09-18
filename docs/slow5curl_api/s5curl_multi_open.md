# s5curl_multi_open

## NAME
s5curl_multi_open - initializes a resource for multithreaded operations

## SYNOPSYS
`s5curl_multi_t s5curl_multi_open(int32_t n_conns)`

## DESCRIPTION
`s5curl_multi_open()` initializes a resource to be used by multithreaded read fetches. This resource is not thread safe but can be reused throughout the program by multiple calls.

This *s5curl_multi_t* should be freed by the user program using `s5curl_multi_close()`.

## RETURN VALUE
Upon successful completion, `s5curl_multi_open()` returns a *s5curl_multi_t* pointer. Otherwise, NULL is returned and `slow5_errno` is set to indicate the error.

## NOTES
`slow5_errno` will be set in future to indicate the type of error that occurred.

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

    s5curl_multi_t *multi = s5curl_multi_open(MAX_CONNECTS);
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

    s5curl_multi_close(multi);

    curl_global_cleanup();
}
```

## SEE ALSO

[s5curl_get_batch()](s5curl_get_batch.md), [s5curl_multi_close()](s5curl_multi_close.md)