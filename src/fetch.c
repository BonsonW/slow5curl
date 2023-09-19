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

#include <stdlib.h>
#include <string.h>

#include "fetch.h"

extern enum slow5_log_level_opt  slow5_log_level;

s5curl_resp_t *s5curl_resp_init() {
    s5curl_resp_t *resp = calloc(1, sizeof *resp);
    return resp;
}

void s5curl_resp_cleanup(s5curl_resp_t *resp) {
    free(resp->data);
    free(resp);
}

// adapted from https://curl.se/libcurl/c/CURLOPT_WRITEFUNCTION.html
static size_t callback(
    void *data,
    size_t size,
    size_t nmemb,
    void *clientp
) {
    size_t realsize = size * nmemb;
    struct s5curl_resp *mem = (struct s5curl_resp *)clientp;

    char *ptr = realloc(mem->data, mem->size + realsize + 1);
    if (ptr == NULL) {
        return 0;
    }

    mem->data = ptr;
    memcpy(&(mem->data[mem->size]), data, realsize);
    mem->size += realsize;
    mem->data[mem->size] = 0;

    return realsize;
}

static CURLcode construct_byte_range(
    char **range,
    uint64_t begin,
    uint64_t size
) {
    int len = snprintf(NULL, 0, "%zu", begin) + snprintf(NULL, 0, "%zu", begin+size-1);
    *range = malloc(len + 2);
    if (range == NULL) return CURLE_OUT_OF_MEMORY;
    sprintf(*range, "%zu-%zu", begin, begin+size-1);
    return CURLE_OK;
}

CURLcode s5curl_byte_fetch_init(
    CURL *curl,
    const char *url,
    uint64_t begin,
    uint64_t size
) {
    CURLcode res;

    res = curl_easy_setopt(curl, CURLOPT_URL, url);
    if (res != CURLE_OK) return res;

    // construct range field
    char *range;
    res = construct_byte_range(&range, begin, size);
    if (res != CURLE_OK) return res;

    res = curl_easy_setopt(curl, CURLOPT_RANGE, range);
    if (res != CURLE_OK) {
        free(range);
        return res;
    }
    free(range);

    return CURLE_OK;
}

CURLcode s5curl_fetch_bytes_into_resp(
    CURL *curl,
    s5curl_resp_t *resp,
    const char *url,
    uint64_t begin,
    uint64_t size
) {
    if (!curl) return CURLE_FAILED_INIT;
    CURLcode res;

    // follow redirects
    res = curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    if (res != CURLE_OK) return res;

    // write into s5curl_resp
    res = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);
    if (res != CURLE_OK) return res;
    res = curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)resp);
    if (res != CURLE_OK) return res;

    res = s5curl_byte_fetch_init(curl, url, begin, size);
    if (res != CURLE_OK) return res;

    return curl_easy_perform(curl);
}

CURLcode s5curl_fetch_into_resp(
    CURL *curl,
    s5curl_resp_t *resp,
    const char *url
) {
    if (!curl) return CURLE_FAILED_INIT;
    CURLcode res;

    res = curl_easy_setopt(curl, CURLOPT_URL, url);
    if (res != CURLE_OK) return res;

    // follow redirects
    res = curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    if (res != CURLE_OK) return res;

    // write into s5curl_resp
    res = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);
    if (res != CURLE_OK) return res;
    res = curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)resp);
    if (res != CURLE_OK) return res;

    return curl_easy_perform(curl);
}

CURLcode s5curl_fetch_bytes_into_file(
    CURL *curl,
    FILE *fp,
    const char *url,
    uint64_t begin,
    uint64_t size
) {
    if (!curl) return CURLE_FAILED_INIT;
    CURLcode res;

    // follow redirects
    res = curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    if (res != CURLE_OK) return res;

    // write to file
    res = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
    if (res != CURLE_OK) return res;
    res = curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)fp);
    if (res != CURLE_OK) return res;

    res = s5curl_byte_fetch_init(curl, url, begin, size);
    if (res != CURLE_OK) return res;

    return curl_easy_perform(curl);
}

CURLcode s5curl_fetch_into_file(
    CURL *curl,
    FILE *fp,
    const char *url
) {
    if (!curl) return CURLE_FAILED_INIT;
    CURLcode res;

    res = curl_easy_setopt(curl, CURLOPT_URL, url);
    if (res != CURLE_OK) return res;

    // follow redirects
    res = curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    if (res != CURLE_OK) return res;

    // write to file
    res = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
    if (res != CURLE_OK) return res;
    res = curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)fp);
    if (res != CURLE_OK) return res;

    return curl_easy_perform(curl);
}