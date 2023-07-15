#include <slow5/slow5.h>

// open file, only supports reads
struct slow5_file *slow5_wget_open(
    const char *url,
    enum slow5_fmt format
);