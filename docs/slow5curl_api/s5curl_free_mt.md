# s5curl_free_mt

## NAME
s5curl_free_mt - unloads a the *s5curl_mt_t* struct from memory

## SYNOPSYS
`void s5curl_free_mt(s5curl_mt_t *core)`

## DESCRIPTION
`s5curl_free_mt()` unloads a the *s5curl_mt_t* struct from memory, which must have been previously loaded using `s5curl_init_mt()`. Otherwise, or if *s5curl_free_mt()* has already been called before, undefined behaviour occurs.

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
#define N_THREADS 10

int main () {

    curl_global_init(CURL_GLOBAL_ALL);

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

    ret = s5curl_get_batch(core, batch, read_ids, num_reads);

    //...

    s5curl_free_mt(s5c);

    s5curl_close(s5c);

    s5curl_free_mt(core);

    curl_global_cleanup();
}
```

## SEE ALSO

[s5curl_get_batch()](s5curl_get_batch.md), [s5curl_init_mt()](s5curl_init_mt.md)