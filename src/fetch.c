#include <stdlib.h>
#include <string.h>

#include <curl/curl.h>
#include <slow5curl/s5curl.h>
#include "fetch.h"

extern enum slow5_log_level_opt  slow5_log_level;

response_t *response_init() {
    response_t *resp = calloc(1, sizeof *resp);
    return resp;
}

void response_cleanup(response_t *resp) {
    free(resp->data);
    free(resp);
}

// adapted from https://curl.se/libcurl/c/CURLOPT_WRITEFUNCTION.html
size_t resp_callback(
    void *data,
    size_t size,
    size_t nmemb,
    void *clientp
) {
    size_t realsize = size * nmemb;
    struct response *mem = (struct response *)clientp;

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

CURLcode byte_fetch_init(
    CURL *curl,
    const char *url,
    uint64_t begin,
    uint64_t size
) {
    CURLcode res;

    // construct range field
    char *range;
    res = construct_byte_range(&range, begin, size);
    if (res != CURLE_OK) return res;

    res = curl_easy_setopt(curl, CURLOPT_URL, url);
    if (res != CURLE_OK) {
        free(range);
        return res;
    }
    res = curl_easy_setopt(curl, CURLOPT_RANGE, range);
    if (res != CURLE_OK) {
        free(range);
        return res;
    }
    free(range);

    return CURLE_OK;
}

CURLcode fetch_bytes_into_resp(
    CURL *curl,
    response_t *resp,
    const char *url,
    uint64_t begin,
    uint64_t size
) {
    if (!curl) return CURLE_FAILED_INIT;
    CURLcode res;

    // write into response
    res = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, resp_callback);
    if (res != CURLE_OK){
        SLOW5_ERROR("Error in curl_easy_setopt: %s", curl_easy_strerror(res));
        return res;
    }

    res = curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)resp);
    if (res != CURLE_OK) return res;

    res = byte_fetch_init(curl, url, begin, size);

    return curl_easy_perform(curl);
}

CURLcode fetch_bytes_into_fb(
    CURL *curl,
    FILE *fp,
    const char *url,
    uint64_t begin,
    uint64_t size
) {
    if (!curl) return CURLE_FAILED_INIT;
    CURLcode res;

    // write into file pointer
    res = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
    if (res != CURLE_OK) return res;
    res = curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)fp);
    if (res != CURLE_OK) return res;

    res = byte_fetch_init(curl, url, begin, size);

    return curl_easy_perform(curl);
}

CURLcode fetch_file_size(
    CURL *curl,
    curl_off_t *file_size,
    const char *url
) {
    if (!curl) return CURLE_FAILED_INIT;
    CURLcode res;

    // fetch file info
    res = curl_easy_setopt(curl, CURLOPT_NOBODY, 1);
    if (res != CURLE_OK) return res;
    res = curl_easy_setopt(curl, CURLOPT_URL, url);
    if (res != CURLE_OK) return res;

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) return res;

    // return length
    res = curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, file_size);
    if (res != CURLE_OK) return res;

    return CURLE_OK;
}