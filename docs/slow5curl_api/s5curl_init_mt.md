# s5curl_init_mt

## NAME
s5curl_init_mt - initializes a resource for multithreaded operations

## SYNOPSYS
`s5curl_mt_t *s5curl_init_mt(int num_thread, s5curl_t *s5c)`

## DESCRIPTION
`s5curl_init_mt()` initializes a resource to be used by multithreaded read fetches. This resource is not thread safe but can be reused throughout the program by multiple calls.

This *s5curl_mt_t* should be freed by the user program using `s5curl_free_mt()`.

## RETURN VALUE
Upon successful completion, `s5curl_init_mt()` returns a *s5curl_mt_t* pointer. Otherwise, NULL is returned and `s5curl_errno` is set to indicate the error.

## EXAMPLES
```c
#include <slow5curl/s5curl.h>

#define N_THREADS 10
#define BATCH_CAPACITY 100

int main () {

    // setup
    
    s5curl_mt_t *core = s5curl_init_mt(N_THREADS, s5c);
    if (!core) {
        fprintf(stderr, "Error opening connections.\n");
        return EXIT_FAILURE;
    }

    slow5_batch_t *db = slow5_init_batch(BATCH_CAPACITY);
    if (!db) {
        fprintf(stderr, "Error initializing read batch.\n");
        return EXIT_FAILURE;
    }

    ret = s5curl_get_batch(core, db, read_ids, num_reads);

    slow5_free_batch(db);
    
    s5curl_free_mt(core);

    // cleanup
}
```

## SEE ALSO
[s5curl_get_batch()](s5curl_get_batch.md), [s5curl_free_mt()](s5curl_free_mt.md)
