#include <slow5/slow5.h>

slow5_file_t *s5wget_open(
    const char *url,
    enum slow5_fmt format
);

int slow5_idx_load_from_path(
    slow5_file_t *s5p,
    const char *path
);

int s5wget_read(
    const char *url,
    const char *read_id,
    slow5_file_t *sp,
    slow5_idx_t *s_idx,
    slow5_rec_t *read
);
