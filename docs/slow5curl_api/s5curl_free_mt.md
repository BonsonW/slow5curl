# s5curl_free_mt

## NAME
s5curl_free_mt - unloads a the *s5curl_mt_t* struct from memory

## SYNOPSYS
`void s5curl_free_mt(s5curl_mt_t *core)`

## DESCRIPTION
`s5curl_free_mt()` unloads a the *s5curl_mt_t* struct from memory, which must have been previously loaded using `s5curl_init_mt()`. Otherwise, or if `s5curl_free_mt()` has already been called before, undefined behaviour occurs.

## RETURN VALUE
No return value.

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

    slow5_init_batch(db);
    
    s5curl_free_mt(core);

    // cleanup
}
```

## SEE ALSO
[s5curl_get_batch()](s5curl_get_batch.md), [s5curl_init_mt()](s5curl_init_mt.md)