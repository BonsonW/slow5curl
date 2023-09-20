# s5curl_open

## NAME
s5curl_open - fetches and loads a remote BLOW5 file's meta data

## SYNOPSYS
`slow5_file_t *s5curl_open(const char *url)`

## DESCRIPTION
The `s5curl_open()` function fetches and loads a remote BLOW5 file's meta data pointed by the argument *url*, parses and populates the BLOW5 header.

An open slow5 file should be closed at the end using `s5curl_open()` function.

## RETURN VALUE
Upon successful completion, `s5curl_open()` returns a *s5curl_t* pointer. Otherwise, NULL is returned. If the error occured with SLOW5 then `slow5_errno` is set to indicate the error.

## ERRORS
* `S5CURL_ERR_FETCH`       
    &nbsp;&nbsp;&nbsp;&nbsp; Fetching data failed.
* `S5CURL_ERR_SLOW5`       
    &nbsp;&nbsp;&nbsp;&nbsp; Failed to parse SLOW5 data.


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