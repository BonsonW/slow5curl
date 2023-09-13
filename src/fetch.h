#ifndef FETCH_H_
#define FETCH_H_

#include <stdint.h>
#include <curl/curl.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct s5curl_resp {
    char *data;
    size_t size;
} s5curl_resp_t;

size_t s5curl_resp_callback(void *data, size_t size, size_t nmemb, void *clientp);

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