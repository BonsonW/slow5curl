#ifndef INDEX_H_
#define INDEX_H_

#include <slow5/slow5.h>

static int slow5_idx_read(
    struct slow5_idx *index
);

slow5_idx_t *slow5_idx_init_from_path(
    slow5_file_t *s5p,
    const char *path
);

int slow5_idx_load_from_path(
    slow5_file_t *s5p,
    const char *path
);

#endif