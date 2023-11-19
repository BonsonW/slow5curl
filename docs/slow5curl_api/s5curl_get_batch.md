# s5curl_get_batch

## NAME
s5curl_get_batch - fetches a list of record from a remote BLOW5 file corresponding to given read IDs

## SYNOPSYS
`int s5curl_get_batch(s5curl_mt_t *core, slow5_batch_t *db, char **rid, int num_rid)`

## DESCRIPTION
`s5curl_get_batch()` fetches and decodes records from a remote BLOW5 file *s5c* for a specified list of *read_id*s into *db*.

The argument *rid* points to an array of read identifier strings.

The argument *num_rid* corresponds with the number of read IDs passed in the method.

**records* should have the appropriate memory allocated before `s5curl_get_batch()` is called. Each *slow5_rec_t* in **records* should be freed by the user program using `slow5_rec_free()`.

The argument *s5c* points to a *slow5_file_t* opened using `s5curl_open()`. `s5curl_get_batch()` requires the SLOW index to be pre-loaded to *s5c* using `s5curl_idx_load()` or `s5curl_idx_load_with()`.

The argument *core* points to an initialised *s5curl_mt_t*. This is done with `s5curl_init_mt`.

The argument *db* points to an initialised *slow5_batch_t*. This is done with `slow5_init_batch`.

## RETURN VALUE
Upon successful completion, `s5curl_get_batch()` returns the number of reads passed in. Otherwise, the process will exit and `s5curl_errno` is set to indicate the error.

## EXAMPLES
```c
#include <slow5curl/s5curl.h>

#define N_THREADS 10
#define BATCH_CAPACITY 100

int main () {

    // setup
    
    s5curl_mt_t *core = s5curl_init_mt(N_THREADS, s5c);

    slow5_batch_t *db = slow5_init_batch(BATCH_CAPACITY);

    ret = s5curl_get_batch(core, db, read_ids, num_reads);
    if (ret != num_reads) {
        fprintf(stderr, "Error fetching batch.\n");
        return EXIT_FAILURE;
    }

    slow5_free_batch(db);
    
    s5curl_free_mt(core);

    // cleanup
}
```

## SEE ALSO
[s5curl_init_mt()](s5curl_init_mt.md), [slow5_batch_t](https://hasindu2008.github.io/slow5lib/slow5_api/slow5_mt_api.html), [slow5_init_batch()](https://hasindu2008.github.io/slow5lib/slow5_api/mt_api/slow5_init_batch.html), [slow5_free_batch()](https://hasindu2008.github.io/slow5lib/slow5_api/mt_api/slow5_free_batch.html)
