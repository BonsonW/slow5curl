#include <slow5/slow5.h>

// open file, only supports reads
slow5_file_t *slow5_wget_init(
    const char *url,
    enum slow5_fmt format
);