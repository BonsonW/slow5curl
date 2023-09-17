/*
MIT License

Copyright (c) 2023 Bonson Wong

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef S5CURL_H_
#define S5CURL_H_

#include <slow5/slow5.h>
#include <curl/curl.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct s5curl_conn_stack {
    CURL **curls;
    int32_t top;
    int32_t n_conns;
} s5curl_conn_stack_t;

typedef struct slow5_curl {
    char *url;
    slow5_file_t *s5p;
} slow5_curl_t;

typedef struct s5curl_multi {
    s5curl_conn_stack_t *conns;
} s5curl_multi_t;

slow5_curl_t *s5curl_open(const char *url);

void s5curl_close(slow5_curl_t *s5c);

int s5curl_idx_load(slow5_curl_t *s5c);

int s5curl_idx_load_with(slow5_curl_t *s5c, const char *path);

void s5curl_idx_unload(slow5_curl_t *s5c);

int s5curl_get(slow5_curl_t *s5c, CURL *curl, const char *read_id, slow5_rec_t **record);

int s5curl_get_batch(slow5_curl_t *s5c, s5curl_multi_t *s5curl_multi, long max_conns, uint64_t n_reads, char **read_ids, slow5_rec_t **records);

s5curl_multi_t *s5curl_multi_open(int32_t n_conns);

void s5curl_multi_close(s5curl_multi_t *s5curl_multi);

int s5curl_conns_push(s5curl_conn_stack_t *conns, CURL *curl);

CURL *s5curl_conns_pop(s5curl_conn_stack_t *conns);

#ifdef __cplusplus
}
#endif

#endif