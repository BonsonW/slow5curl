#include <stdlib.h>
#include <string.h>

#include <curl/curl.h>

#include "fetch.h"

int response_free(response_t *resp) {
    free(resp->data);
    return 0;
}

// adapted from https://curl.se/libcurl/c/CURLOPT_WRITEFUNCTION.html
static size_t callback(
    void *data,
    size_t size,
    size_t nmemb,
    void *clientp
) {
    size_t realsize = size * nmemb;
    struct response *mem = (struct response *)clientp;
    
    char *ptr = realloc(mem->data, mem->size + realsize + 1);
    if (ptr == NULL) { // out of mem
        return 0;
    }
    
    mem->data = ptr;
    memcpy(&(mem->data[mem->size]), data, realsize);
    mem->size += realsize;
    mem->data[mem->size] = 0;
    
    return realsize;
}

// todo: check error for setting the header and mallocing
static int construct_byte_range(
    char **range,
    uint64_t begin,
    uint64_t size
) {
    int len = snprintf(NULL, 0, "%zu", begin) + snprintf(NULL, 0, "%zu", begin+size-1);
    *range = malloc(len + 2);
    sprintf(*range, "%zu-%zu", begin, begin+size-1);
    return 0;
}

int fetch_bytes_into_resp(
    response_t *resp,
    const char *url,
    uint64_t begin,
    uint64_t size
) {
    CURL *curl = curl_easy_init();
    
    if (curl) {
        CURLcode res;

        // write into response
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)resp);
        
        // construct range field
        char *range;
        construct_byte_range(&range, begin, size);
        
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_RANGE, range);
        free(range);
        
        res = curl_easy_perform(curl);

        curl_easy_cleanup(curl);
    } else {
        return -1;
    }
    
    return 0;
}

int fetch_bytes_into_fb(
    FILE *fp,
    const char *url,
    uint64_t begin,
    uint64_t size
) {
    CURL *curl = curl_easy_init();
    
    if (curl) {
        CURLcode res;

        // write into file pointer
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)fp);
        
        // construct range field
        char *range;
        construct_byte_range(&range, begin, size);
        
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_RANGE, range);
        free(range);
        
        res = curl_easy_perform(curl);
        
        curl_easy_cleanup(curl);
    } else {
        return -1;
    }
    
    return 0;
}

int queue_fetch_bytes_into_resp(
    response_t *resp,
    const char *url,
    uint64_t begin,
    uint64_t size,
    CURLM *cm
) {
    CURL *curl = curl_easy_init();
    
    // write into response
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)resp);
    curl_easy_setopt(curl, CURLOPT_PRIVATE, resp);
    
    // construct range field
    char *range;
    construct_byte_range(&range, begin, size);
    
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_RANGE, range);
    free(range);

    curl_multi_add_handle(cm, curl);

    return 0;
}

int fetch_file_size(
    curl_off_t *file_size,
    const char *url
) {
    CURL *curl = curl_easy_init();
    
    if (curl) {
        CURLcode res;

        // get 
        curl_easy_setopt(curl, CURLOPT_NOBODY, 1);
        curl_easy_setopt(curl, CURLOPT_URL, url);
        
        res = curl_easy_perform(curl);

        curl_easy_cleanup(curl);
    } else {
        return -1;
    }

    curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD_T, file_size);
    
    return 0;
}