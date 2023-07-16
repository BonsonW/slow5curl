#include <slow5/slow5.h>

slow5_file_t *s5wget_open(
    const char *url,
    enum slow5_fmt format
);

int slow5_idx_load_from_path(
    slow5_file_t *s5p,
    const char *path
);