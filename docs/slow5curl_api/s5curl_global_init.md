# s5curl_global_init

## NAME
s5curl_global_init - global initialization for slow5curl

## SYNOPSYS
`int s5curl_global_init()`

## DESCRIPTION
The `s5curl_global_init()` function sets up the program environment for s5curl. This must be called once before all s5curl orperations are peformed.

Every `s5curl_global_init()` should be followed by a corresponding `s5curl_global_cleanup()` function when all s5curl operations are completed.

## RETURN VALUE
Upon successful completion, `s5curl_global_init()` returns a non negative integer (>=0). Otherwise, a negative value is returned that indicates the error.

## ERRORS

* `S5CURL_ERR_CURL`
    &nbsp;&nbsp;&nbsp;&nbsp; Failed to initialise CURL global resrouces.

## NOTES

## EXAMPLES
```c
#include <stdio.h>
#include <stdlib.h>
#include <slow5curl/s5curl.h>

#define URL "https://example.blow5"
#define READ_ID "0032812b-1ea5-46f1-a844-5bcc3bf3c21f"

int main () {

    int ret = s5curl_global_init();
    if (ret < 0) {
        fprintf(stderr, "Error initializing global resources\n");
        exit(EXIT_FAILURE);
    }

    // s5curl operations

    s5curl_global_cleanup();
}
```

## SEE ALSO
[s5curl_global_cleanup()](s5curl_global_cleanup.md)