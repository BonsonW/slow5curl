#ifndef INDEX_H_
#define INDEX_H_

#include <slow5/slow5.h>

#ifdef __cplusplus
extern "C" {
#endif

int slow5_idx_load_from_path(
    slow5_file_t *s5p,
    const char *path
);

#ifdef __cplusplus
}
#endif

#endif