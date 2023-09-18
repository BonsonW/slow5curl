# s5curl_open

## NAME
s5curl_open - fetches and loads a remote SLOW5 file's meta data

## SYNOPSYS
`slow5_file_t *s5curl_open(const char *url)`

## DESCRIPTION

The `s5curl_open()` function fetches and loads a remote SLOW5 file's meta data pointed by the argument *url*, parses and populates the SLOW5 header.

An open slow5 file should be closed at the end using `s5curl_open()` function.

## RETURN VALUE

Upon successful completion, `s5curl_open()` returns a *s5curl_t* pointer. Otherwise, NULL is returned and `slow5_errno` is set to indicate the error.

## ERRORS

* `SLOW5_ERR_ARG`
  &nbsp;&nbsp;&nbsp;&nbsp; Invalid argument - pathname provided was NULL.
* `SLOW5_ERR_IO`
  &nbsp;&nbsp;&nbsp;&nbsp; File I/O error, for instance, `fopen`, `ftello` or `fileno` failed.
* `SLOW5_ERR_UNK`
  &nbsp;&nbsp;&nbsp;&nbsp; Wrong file extension, that is neither *.slow5* nor *.blow5*
* `SLOW5_ERR_HDRPARSE`
  &nbsp;&nbsp;&nbsp;&nbsp; Parsing and populating the header failed.
* `SLOW5_ERR_MEM`
  &nbsp;&nbsp;&nbsp;&nbsp; Memory allocation failed.
* `SLOW5_ERR_TRUNC`
  &nbsp;&nbsp;&nbsp;&nbsp; End of file (EOF) reached prematurely.
* `SLOW5_ERR_MAGIC`
  &nbsp;&nbsp;&nbsp;&nbsp; Invalid magic number.
* `SLOW5_ERR_VERSION`
  &nbsp;&nbsp;&nbsp;&nbsp; File version is incompatible with this library version.
* `SLOW5_ERR_PRESS`
  &nbsp;&nbsp;&nbsp;&nbsp; Initialisation of compression/decompression buffers failed.
* `SLOW5_ERR_OTH`
  &nbsp;&nbsp;&nbsp;&nbsp; Other error.

## NOTES
Currently only opens binary slow5 files

## EXAMPLES
```
#include <stdio.h>
#include <stdlib.h>
#include <slow5curl/s5curl.h>
#include <curl/curl.h>

#define URL "https://example.blow5"

int main () {

    curl_global_init(CURL_GLOBAL_ALL);

    s5curl_t *s5c = s5curl_open(URL);
    if (s5c == NULL) {
       fprintf(stderr, "Error fetching slow5 file\n");
       exit(EXIT_FAILURE);
    }

    //...

    s5curl_close(s5c);

    curl_global_cleanup();
}
```

## SEE ALSO

[s5curl_close()](s5curl_close.md)