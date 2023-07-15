#include "fetch.h"
#include "../slow5lib/src/slow5_idx.h"

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

// write byte-range GET response to buffer
int get_object_bytes(
    response_t *resp,
    const char *url,
    uint64_t begin,
    uint64_t size
) {
    CURL *curl = curl_easy_init();
    
    if (curl) {
        CURLcode res;

        // setup write callback
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)resp);
        
        // construct range field
        int len = snprintf(NULL, 0, "%zu", begin) + snprintf(NULL, 0, "%zu", begin+size-1);
        char *range = malloc(len + 2);
        sprintf(range, "%zu-%zu", begin, size);
        
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_RANGE, range);
        
        res = curl_easy_perform(curl);

        curl_easy_cleanup(curl);
    } else {
        return -1;
    }
    
    return 0;
}

// fetch read from URL given slow5 pointer and slow5 index 
int fetch_read(
    const char *url,
    const char *read_id,
    slow5_file_t *sp,
    slow5_idx_t *s_idx
) {
    struct slow5_rec_idx read_index;
	int ret = slow5_idx_get(s_idx, read_id, &read_index);
	if (ret < 0) {
		fprintf(stderr, "Error in getting index for read %s\n", read_id);
		return -1;
	}
	
	// exclude meta data before copying record
	size_t bytes = read_index.size - sizeof(slow5_rec_size_t);
	response_t resp = {0};

	ret = get_object_bytes(
	    &resp,
		url, 
		read_index.offset,
		read_index.size
	);
	if (ret < 0) {
		fprintf(stderr, "Error in getting index for read %s\n", read_id);
		return -1;
	}

	fprintf(stderr, "Successfully fetched read %s\n", read_id);

	slow5_rec_t *read = NULL;
	
	char *read_start = resp.data + sizeof(slow5_rec_size_t);

	ret = slow_decode((void *)&read_start, &bytes, &read, sp);
	slow5_rec_free(read);
	response_free(&resp);

	if (ret < 0) { 
		fprintf(stderr, "Error decoding read %s\n", read_id);
		return -1;
	} else {
		fprintf(stderr, "Successfully decoded read %s\n", read_id);
	}

    return 0;
}
