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
#include <slow5/slow5_mt.h>
#include <curl/curl.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    char *url;
    slow5_file_t *s5p;
} s5curl_t;

typedef struct {
    int32_t num_thread;
    s5curl_t *s5c;
    CURL **curl;
} s5curl_mt_t;

s5curl_t *s5curl_open(const char *url);

void s5curl_close(s5curl_t *s5c);

int s5curl_idx_load(s5curl_t *s5c);

int s5curl_idx_load_with(s5curl_t *s5c, const char *path);

void s5curl_idx_unload(s5curl_t *s5c);

int s5curl_get(s5curl_t *s5c, CURL *curl, const char *read_id, slow5_rec_t **record);

int s5curl_get_batch(s5curl_mt_t *core, slow5_batch_t *db, char **rid, int num_rid);

s5curl_mt_t *s5curl_init_mt(int num_thread, s5curl_t *s5c);

void s5curl_free_mt(s5curl_mt_t *core);

#ifdef __cplusplus
}
#endif

#endif