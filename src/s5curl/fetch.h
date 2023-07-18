#ifndef FETCH_H_
#define FETCH_H_

#include <stdint.h>
#include <slow5/slow5.h>

struct response {
    char *data;
    size_t size;
};

typedef struct response response_t;

int response_free(response_t *resp);

int fetch_bytes_into_resp(
    response_t *resp,
    const char *url,
    uint64_t begin,
    uint64_t size
);

int fetch_bytes_into_fb(
    FILE *fp,
    const char *url,
    uint64_t begin,
    uint64_t size
);

#endif