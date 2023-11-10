# s5curl_idx_unload

## NAME
s5curl_idx_unload - unloads a BLOW5 index from the memory

## SYNOPSYS
`void s5curl_idx_unload(s5curl_t *s5c)`

## DESCRIPTION
`s5curl_idx_unload()` unloads an index file loaded into memory for a remote BLOW5 file pointed by *s5c*, which must have been previously loaded using `s5curl_idx_load()` or `s5curl_idx_load_with()`. Otherwise, or if *s5curl_idx_unload(s5c)* has already been called before, undefined behaviour occurs.

## RETURN VALUE
No return value.

## NOTES

## EXAMPLES
```
#include <stdio.h>
#include <stdlib.h>
#include <slow5curl/s5curl.h>

#define URL "https://example.blow5"

int main () {

    s5curl_global_init();

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

    //...

    s5curl_idx_unload(s5c);

    s5curl_close(s5c);

    s5curl_global_cleanup();
}
```

## SEE ALSO
[s5curl_idx_load()](s5curl_idx_load.md), [s5curl_idx_load_with()](s5curl_idx_load_with.md)