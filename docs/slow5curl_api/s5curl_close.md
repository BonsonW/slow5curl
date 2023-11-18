# s5curl_close

## NAME
s5curl_close - frees the *s5curl_t* struct and its resources from memory

## SYNOPSYS
`int s5curl_close(s5curl_t *s5c)`

## DESCRIPTION
`s5curl_close()` frees the *s5curl_t* struct and its resources from memory.
The argument *s5p* is the *s5curl_t* pointer returned by a previous `s5curl_open()` call.

The behaviour of `s5curl_close()` is undefined if the parameter is an illegal pointer, or if `s5curl_close()` has been previous invoked on the pointer.

If a slow5 index has been associated with the *s5p* structure (using a previous `s5curl_idx_load()` invocation), `s5curl_idx_load()` must be invoked before calling `s5curl_close()`.

## RETURN VALUE
No return value.

## EXAMPLES
```c
#include <slow5curl/s5curl.h>

#define URL "https://example.blow5"

int main () {

    // setup

    s5curl_t *s5c = s5curl_open(URL);
    if (s5c == NULL) {
       fprintf(stderr, "Error fetching slow5 file\n");
       exit(EXIT_FAILURE);
    }

    // s5curl operations

    s5curl_close(s5c);

    // cleanup
}
```

## SEE ALSO
[s5curl_open()](s5curl_open.md)
