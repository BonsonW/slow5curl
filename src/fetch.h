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

#ifndef FETCH_H_
#define FETCH_H_

#include <stdint.h>
#include <slow5curl/s5curl.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct s5curl_resp {
    char *data;
    size_t size;
} s5curl_resp_t;

s5curl_resp_t *s5curl_resp_init();

void s5curl_resp_cleanup(s5curl_resp_t *resp);

CURLcode s5curl_byte_fetch_init(CURL *curl, const char *url, uint64_t begin, uint64_t size);

CURLcode s5curl_fetch_into_resp(CURL *curl, s5curl_resp_t *resp, const char *url);

CURLcode s5curl_fetch_into_file(CURL *curl, FILE *fp, const char *url);

CURLcode s5curl_fetch_bytes_into_resp(CURL *curl, s5curl_resp_t *resp, const char *url, uint64_t begin, uint64_t size);

CURLcode s5curl_fetch_bytes_into_file(CURL *curl, FILE *fp, const char *url, uint64_t begin, uint64_t size);

#ifdef __cplusplus
}
#endif

#endif