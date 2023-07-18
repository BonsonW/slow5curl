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
    const char *read_id,
    slow5_rec_t *read
);

// int s5curl_read_list(
//     const char *url,
//     const char **read_ids,
//     slow5_file_t *sp,
//     slow5_idx_t *s_idx,
//     slow5_rec_t **reads
// );

int slow5_idx_load_from_path(
    slow5_file_t *s5p,
    const char *path
);
