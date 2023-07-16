#include <curl/curl.h>
#include <stdint.h>

struct response {
    char *data;
    size_t size;
};

typedef struct response response_t;

int response_free(response_t *resp);

int fetch_bytes(
    response_t *resp,
    const char *url,
    uint64_t begin,
    uint64_t size
);
