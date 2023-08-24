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

response_t *response_init();

void response_cleanup(response_t *resp);

CURLcode resp_byte_fetch_init(CURL *curl, response_t *resp, const char *url, uint64_t begin, uint64_t size);

CURLcode fetch_bytes_into_fb(CURL *curl, FILE *fp, const char *url, uint64_t begin, uint64_t size);

CURLcode fetch_file_size(CURL *curl, curl_off_t *file_size, const char *url);

#ifdef __cplusplus
}
#endif

#endif