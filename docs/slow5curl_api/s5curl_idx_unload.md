# s5curl_idx_unload

## NAME
s5curl_idx_unload - unloads a BLOW5 index from the memory

## SYNOPSYS
`void s5curl_idx_unload(s5curl_t *s5c)`

## DESCRIPTION
`s5curl_idx_unload()` unloads an index file loaded into memory for a remote BLOW5 file pointed by *s5c*, which must have been previously loaded using `s5curl_idx_load()` or `s5curl_idx_load_with()`. Otherwise, or if *s5curl_idx_unload(s5c)* has already been called before, undefined behaviour occurs.

## RETURN VALUE
No return value.

## EXAMPLES
```c
#include <slow5curl/s5curl.h>

#define URL "https://example.blow5"

int main () {

    // setup

    s5curl_t *s5c = s5curl_open(URL);
    
    ret = s5curl_idx_load(s5c);
    if (ret < 0) {
        fprintf(stderr, "Error in loading index\n");
        exit(EXIT_FAILURE);
    }

    // s5curl operations

    s5curl_idx_unload(s5c);

    s5curl_close(s5c);

    // cleanup
}
```

## SEE ALSO
[s5curl_idx_load()](s5curl_idx_load.md), [s5curl_idx_load_with()](s5curl_idx_load_with.md)