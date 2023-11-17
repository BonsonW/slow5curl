# s5curl_open

## NAME
s5curl_open - fetches and loads a remote BLOW5 file's meta data

## SYNOPSYS
`slow5_file_t *s5curl_open(const char *url)`

## DESCRIPTION
The `s5curl_open()` function fetches and loads a remote BLOW5 file's meta data pointed by the argument *url*, parses and populates the BLOW5 header.

An open slow5 file should be closed at the end using `s5curl_open()` function.

## RETURN VALUE
Upon successful completion, `s5curl_open()` returns a *s5curl_t* pointer. Otherwise, NULL is returned and `s5curl_errno` is set to indicate the error.

## ERRORS
* `S5CURL_ERR_CURL`
    &nbsp;&nbsp;&nbsp;&nbsp; Failed to initialise CURL handle.
* `S5CURL_ERR_FETCH`
    &nbsp;&nbsp;&nbsp;&nbsp; Fetching data failed.
* `S5CURL_ERR_ARG`
    &nbsp;&nbsp;&nbsp;&nbsp; Invalid argument.
* `S5CURL_ERR_IO`
    &nbsp;&nbsp;&nbsp;&nbsp; Other IO error.
* `S5CURL_ERR_MEM`
    &nbsp;&nbsp;&nbsp;&nbsp; Memory (re)allocation error.
* `S5CURL_ERR_OTH`
    &nbsp;&nbsp;&nbsp;&nbsp; Other error (big endian, internal error, etc.).

## NOTES
Currently only opens binary slow5 files

## EXAMPLES
```c
#include <stdio.h>
#include <stdlib.h>
#include <slow5curl/s5curl.h>

#define URL "https://example.blow5"

int main () {

    int ret = s5curl_global_init();
    if (ret < 0) {
        fprintf(stderr, "Error initializing global resources\n");
        exit(EXIT_FAILURE);
    }

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
[s5curl_close()](s5curl_close.md)