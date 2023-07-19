#ifndef S5CURL_H_
#define S5CURL_H_

#include <slow5/slow5.h>

typedef struct slow5_curl {
    const char *url;
    slow5_file_t *s5p;
} slow5_curl_t;

void s5curl_close(
    slow5_curl_t *s5c
);

void s5curl_idx_unload(
    slow5_curl_t *s5c
);

slow5_curl_t *s5curl_open(
    const char *url
);

// int s5curl_idx(
//     slow5_curl_t *s5c
// );

int s5curl_read(
    slow5_curl_t *s5c,
    char *read_id,
    slow5_rec_t *read
);

int s5curl_read_list(
    slow5_curl_t *s5c,
    uint64_t max_connects,
    uint64_t n_reads,
    char **read_ids,
    slow5_rec_t **reads
);

#endif