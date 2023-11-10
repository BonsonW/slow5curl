# s5curl_global_init

## NAME
s5curl_global_cleanup - releases resources acquired by s5curl_global_init

## SYNOPSYS
`void s5curl_global_cleanup()`

## DESCRIPTION
The `s5curl_global_cleanup()` releases resources acquired by `s5curl_global_init`. This must be called once after all s5curl orperations are peformed.

Every `s5curl_global_init()` should be followed by a corresponding `s5curl_global_cleanup()` function when all s5curl operations are completed.

## RETURN VALUE
No return value.

## NOTES

## EXAMPLES
```
#include <stdio.h>
#include <stdlib.h>
#include <slow5curl/s5curl.h>

#define URL "https://example.blow5"
#define READ_ID "0032812b-1ea5-46f1-a844-5bcc3bf3c21f"

int main () {

    s5curl_global_init();

    S5CURLCONN *curl = s5curl_conn_init();

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

    ret = s5curl_get(s5c, curl, READ_ID, &rec);
    if (ret < 0) {
        fprintf(stderr, "Error in when fetching the read\n");
    }

    //...

    s5curl_idx_unload(s5c);

    s5curl_close(s5c);

    s5curl_conn_cleanup(curl);

    s5curl_global_cleanup();
}
```

## SEE ALSO
[s5curl_global_init()](s5curl_global_init.md)