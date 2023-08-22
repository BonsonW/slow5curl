#ifndef S5CURL_H_
#define S5CURL_H_

#include <slow5/slow5.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct slow5_curl {
    const char *url;
    slow5_file_t *s5p;
} slow5_curl_t;

typedef struct conn_stack {
    CURL **curls;
    size_t top;
    size_t n_conns;
} conn_stack_t;

conn_stack_t *s5curl_open_conns(size_t n_conns);

void s5curl_close_conns(conn_stack_t *conns);

CURL *s5curl_conns_pop(conn_stack_t *conns);

int s5curl_conns_push(conn_stack_t *conns, CURL *curl);

slow5_curl_t *s5curl_open(const char *url);

void s5curl_close(slow5_curl_t *s5c);

int s5curl_idx_load(slow5_curl_t *s5c);

void s5curl_idx_unload(slow5_curl_t *s5c);

int s5curl_get(slow5_curl_t *s5c, CURL *curl, char *read_id, slow5_rec_t *read);

int s5curl_get_batch(slow5_curl_t *s5c, conn_stack_t *conns, CURLM *cm, uint64_t n_reads, char **read_ids, slow5_rec_t **reads);

#ifdef __cplusplus
}
#endif

#endif