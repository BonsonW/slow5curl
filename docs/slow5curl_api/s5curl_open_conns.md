# s5curl_open_conns

## NAME
s5curl_open_conns - initializes a stack of *CURL* handles

## SYNOPSYS
`conn_stack_t s5curl_open_conns(int32_t n_conns)`

## DESCRIPTION
`s5curl_open_conns()` initializes a stack of *CURL* handles to be used by multithreaded read fetches. This connection stack is not thread safe but can be reused throughout the program by multiple calls.

This *conn_stack_t* should be freed by the user program using `s5curl_close_conns()`.

## RETURN VALUE
Upon successful completion, `s5curl_open_conns()` returns a *conn_stack_t* pointer. Otherwise, NULL is returned and `slow5_errno` is set to indicate the error.

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

    conn_stack_t *conns = s5curl_open_conns(MAX_CONNECTS);
    if (!conns) {
        fprintf(stderr, "Error opening connections.\n");
        return EXIT_FAILURE;
    }

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

    ret = s5curl_get_batch(*s5c, conns, MAX_CONNECTS, N_READS, read_ids, records);

    //...

    s5curl_idx_unload(s5c);

    s5curl_close(s5c);

    s5curl_close_conns(conns);

    curl_global_cleanup();
}
```

## SEE ALSO

[s5curl_get_batch()](s5curl_get_batch.md), [s5curl_close_conns()](s5curl_close_conns.md)