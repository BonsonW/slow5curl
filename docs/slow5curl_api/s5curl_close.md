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

Upon successful completion, `s5curl_close()` returns 0. Otherwise, a negative value (EOF) is returned and `slow5_errno` is set to indicate the error.

## ERRORS
* `SLOW5_ERR_IO`
  &nbsp;&nbsp;&nbsp;&nbsp;File I/O error, for instance, `fclose` failed.

## NOTES

## EXAMPLES
```c
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

    //...

    s5curl_close(s5c);

    s5curl_global_cleanup();
}
```

## SEE ALSO
[s5curl_open()](s5curl_open.md)
