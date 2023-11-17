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

The argument *core* points to an initialized *s5curl_mt_t*. This is done with `s5curl_init_mt`.

The argument *db* points to an initialized *slow5_batch_t*. This is done with `slow5_init_batch`.

## RETURN VALUE
Upon successful completion, `s5curl_get_batch()` returns the number of reads passed in. Otherwise, the process will exit and `s5curl_errno` is set to indicate the error.

## NOTES

## EXAMPLES
```c
#include <stdio.h>
#include <stdlib.h>
#include <slow5curl/s5curl.h>
#define URL "https://example.blow5"
#define N_THREADS 10

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
[s5curl_init_mt()](s5curl_init_mt.md)