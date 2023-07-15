#include <slow5/slow5.h>
#include <curl/curl.h>

struct response {
    char *data;
    size_t size;
};

typedef struct response response_t;

int response_free(response_t *resp);

int get_object_bytes(
    response_t *resp,
    const char *url,
    uint64_t begin,
    uint64_t size
);

// read 
int fetch_read(
    const char *url,
    const char *read_id,
    slow5_file_t *sp,
    slow5_idx_t *s_idx
);
