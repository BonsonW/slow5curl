# s5curl_close_conns

## NAME
s5curl_close_conns - unloads a stack of *CURL* handles from memory

## SYNOPSYS
`void s5curl_close_conns(conn_stack_t *conns)`

## DESCRIPTION
`s5curl_close_conns()` unloads a stack of *CURL* handles from memory pointed by *conns*, which must have been previously loaded using `s5curl_open_conns()`. Otherwise, or if *s5curl_close_conns(conns)* has already been called before, undefined behaviour occurs.

## RETURN VALUE

No return value

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

    s5curl_close_conns(s5c);

    s5curl_close(s5c);

    s5curl_close_conns(conns);

    curl_global_cleanup();
}
```

## SEE ALSO

[s5curl_get_batch()](s5curl_get_batch.md), [s5curl_open_conns()](s5curl_open_conns.md)