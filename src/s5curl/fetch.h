#ifndef FETCH_H_
#define FETCH_H_

#include <stdint.h>
#include <curl/curl.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct response {
    char *data;
    size_t size;
    uint32_t id;
} response_t;

int response_free(response_t *resp);

CURLcode fetch_bytes_into_resp(CURL *curl, response_t *resp, const char *url, uint64_t begin, uint64_t size);

CURLcode fetch_bytes_into_fb(CURL *curl, FILE *fp, const char *url, uint64_t begin, uint64_t size);

CURLcode queue_fetch_bytes_into_resp(CURL *curl, response_t *resp, const char *url, uint64_t begin, uint64_t size, CURLM *cm);

CURLcode fetch_file_size(CURL *curl, curl_off_t *file_size, const char *url);

#ifdef __cplusplus
}
#endif

#endif