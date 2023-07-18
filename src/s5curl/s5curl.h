#include <slow5/slow5.h>

slow5_file_t *s5curl_file(
    const char *url,
    enum slow5_fmt format
);

int s5curl_idx(
    slow5_file_t *s5p,
    const char *url
);

int s5curl_read(
    const char *url,
    const char *read_id,
    slow5_file_t *sp,
    slow5_idx_t *s_idx,
    slow5_rec_t *read
);

int slow5_idx_load_from_path(
    slow5_file_t *s5p,
    const char *path
);
