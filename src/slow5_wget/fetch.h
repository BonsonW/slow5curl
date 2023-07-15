#include <slow5/slow5.h>
#include <curl/curl.h>

struct memory {
  char *response;
  size_t size;
};

typedef struct memory memory_t;

int get_object_bytes(
    memory_t *chunk,
    const char *url,
    uint64_t begin,
    uint64_t size
);

// read 
int fetch_read(
    const char *url,
    const char *read_id,
    slow5_file_t *sp,
    slow5_idx_t *s_idx
);
