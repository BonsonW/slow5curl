#include <slow5/slow5.h>
#include <curl/curl.h>

// read 
int fetch_read(
    const char *url,
    const char *read_id,
    slow5_file_t *sp,
    slow5_idx_t *s_idx
);
