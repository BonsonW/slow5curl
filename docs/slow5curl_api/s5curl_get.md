# s5curl_get

## NAME
s5curl_get - fetches a record from a remote BLOW5 file corresponding to a given read ID

## SYNOPSYS
`int s5curl_get(const char *read_id, slow5_rec_t **record, S5CURLCONN *curl, s5curl_t *s5c)`

## DESCRIPTION
`s5curl_get()` fetches and decodes a record from a remote BLOW5 file *s5c* for a specified *read_id* into a *slow5_rec_t* and stores it in **record*.

The argument *read_id* points to a read identifier string.

If **record* is set to NULL before the call, then `s5curl_get()` will allocate a *slow5_rec_t* for storing the record. This *slow5_rec_t* should be freed by the user program using `slow5_rec_free()`.

The argument *s5c* points to a *slow5_file_t* opened using `s5curl_open()`. `s5curl_get()` requires the SLOW index to be pre-loaded to *s5c* using `s5curl_idx_load()` or `s5curl_idx_load_with()`.

The argument *curl* points to a *S5CURLCONN* handle.

`s5curl_get()` can be called by multiple threads in parallel on the same *slow5_file_t* pointer, however it must have a different *CURL* handle.

## RETURN VALUE
Upon successful completion, `s5curl_get()` returns a non negative integer (>=0). Otherwise, a negative value is returned that indicates the error. If the error occured with SLOW5 then `slow5_errno` is set to indicate the error.

## ERRORS
* `S5CURL_ERR_FETCH`       
    &nbsp;&nbsp;&nbsp;&nbsp; Fetching data failed.
* `S5CURL_ERR_SLOW5`       
    &nbsp;&nbsp;&nbsp;&nbsp; Failed to parse SLOW5 data.
* `S5CURL_ERR_NOTFOUND`       
    &nbsp;&nbsp;&nbsp;&nbsp; Could not find read ID.

## EXAMPLES
```c
#include <slow5curl/s5curl.h>

#define READ_ID "0032812b-1ea5-46f1-a844-5bcc3bf3c21f"

int main () {

    // setup

    S5CURLCONN *curl = s5curl_conn_init();

    slow5_rec_t *rec = NULL;

    ret = s5curl_get(s5c, curl, READ_ID, &rec);
    if (ret < 0) {
        fprintf(stderr, "Error in when fetching the read\n");
        exit(EXIT_FAILURE);
    }

    slow5_rec_free(rec);

    s5curl_conn_cleanup(curl);

    // cleanup
}
```

## SEE ALSO
[slow5_file_t](https://hasindu2008.github.io/slow5lib/slow5_api/slow5.html), [slow5_rec_free()](https://hasindu2008.github.io/slow5lib/slow5_api/slow5_rec_free.html)