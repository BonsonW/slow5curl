#ifndef S5CURL_H_
#define S5CURL_H_

#include <slow5/slow5.h>
#include <curl/curl.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct slow5_curl {
    char *url;
    slow5_file_t *s5p;
} slow5_curl_t;

typedef struct conn_stack {
    CURL **curls;
    int32_t top;
    int32_t n_conns;
} conn_stack_t;

slow5_curl_t *s5curl_open(const char *url);

void s5curl_close(slow5_curl_t *s5c);

int s5curl_idx_load(slow5_curl_t *s5c);

int s5curl_idx_load_with(slow5_curl_t *s5c, const char *path);

void s5curl_idx_unload(slow5_curl_t *s5c);

int s5curl_get(slow5_curl_t *s5c, CURL *curl, const char *read_id, slow5_rec_t **record);

int s5curl_get_batch(slow5_curl_t *s5c, conn_stack_t *conns, long max_conns, uint64_t n_reads, char **read_ids, slow5_rec_t **records);

conn_stack_t *s5curl_open_conns(int32_t n_conns);

void s5curl_close_conns(conn_stack_t *conns);

#ifdef __cplusplus
}
#endif

#endif