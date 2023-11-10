# s5curl_init_mt

## NAME
s5curl_init_mt - initializes a resource for multithreaded operations

## SYNOPSYS
`s5curl_mt_t *s5curl_init_mt(int num_thread, s5curl_t *s5c)`

## DESCRIPTION
`s5curl_init_mt()` initializes a resource to be used by multithreaded read fetches. This resource is not thread safe but can be reused throughout the program by multiple calls.

This *s5curl_mt_t* should be freed by the user program using `s5curl_free_mt()`.

## RETURN VALUE
Upon successful completion, `s5curl_init_mt()` returns a *s5curl_mt_t* pointer. Otherwise, NULL is returned and `slow5_errno` is set to indicate the error.

## NOTES
`slow5_errno` will be set in future to indicate the type of error that occurred.

## EXAMPLES
```
#include <stdio.h>
#include <stdlib.h>
#include <slow5curl/s5curl.h>

#define URL "https://example.blow5"
#define N_THREADS 10
#define BATCH_CAPACITY 100

int main () {

    s5curl_global_init();

    s5curl_t *s5c = s5curl_open(URL);
    if (s5c == NULL) {
       fprintf(stderr, "Error fetching slow5 file\n");
       exit(EXIT_FAILURE);
    }
    
    s5curl_mt_t *core = s5curl_init_mt(N_THREADS, s5c);
    if (!core) {
        fprintf(stderr, "Error opening connections.\n");
        return EXIT_FAILURE;
    }

    ret = s5curl_idx_load(s5c);
    if (ret < 0) {
        fprintf(stderr, "Error in loading index\n");
        exit(EXIT_FAILURE);
    }

    slow5_batch_t *db = slow5_init_batch(batch_capacity);
    if (!db) {
        fprintf(stderr, "Error initializing read batch.\n");
        return EXIT_FAILURE;
    }

    ret = s5curl_get_batch(core, batch, read_ids, num_reads);

    //...

    s5curl_free_mt(s5c);

    s5curl_close(s5c);

    s5curl_free_mt(core);

    s5curl_global_cleanup();
}
```

## SEE ALSO
[s5curl_get_batch()](s5curl_get_batch.md), [s5curl_free_mt()](s5curl_free_mt.md)