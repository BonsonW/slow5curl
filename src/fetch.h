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
} response_t;

size_t resp_callback(void *data, size_t size, size_t nmemb, void *clientp);

response_t *response_init();

void response_cleanup(response_t *resp);

CURLcode byte_fetch_init(CURL *curl, const char *url, uint64_t begin, uint64_t size);

CURLcode fetch_bytes_into_resp(CURL *curl, response_t *resp, const char *url, uint64_t begin, uint64_t size);

CURLcode fetch_bytes_into_fb(CURL *curl, FILE *fp, const char *url, uint64_t begin, uint64_t size);

CURLcode fetch_file_size(CURL *curl, curl_off_t *file_size, const char *url);

#ifdef __cplusplus
}
#endif

#endif