# s5curl_conn_cleanup

## NAME
s5curl_conn_cleanup - closes an existing connection handle

## SYNOPSYS
`void s5curl_conn_cleanup(S5CURLCONN *conn)`

## DESCRIPTION
The `s5curl_conn_cleanup()` function closes an existing connection handle opened by `s5curl_conn_init()`.

Every `s5curl_conn_init()` should be followed by a corresponding `s5curl_conn_cleanup()` function.

## RETURN VALUE
No return value.

## EXAMPLES
```c
#include <slow5curl/s5curl.h>

#define READ_ID "0032812b-1ea5-46f1-a844-5bcc3bf3c21f"

int main () {

    // setup

    S5CURLCONN *curl = s5curl_conn_init();

    slow5_rec_t *rec = NULL;

    ret = s5curl_get(s5c, curl, READ_ID, &rec);

    slow5_rec_free(rec);

    s5curl_conn_cleanup(curl);

    // cleanup
}
```

## SEE ALSO
[s5curl_conn_init()](s5curl_conn_init.md)
