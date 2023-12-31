/*
MIT License

Copyright (c) 2020 Hasindu Gamaarachchi
Copyright (c) 2020 Sasha Jenner
Copyright (c) 2020 Hiruna Samarakoon
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

#define _XOPEN_SOURCE 700
#include "../slow5lib/src/slow5_idx.h"
#include "../slow5lib/src/slow5_extra.h"
#include "fetch.h"
#include <slow5curl/s5curl.h>
#include <inttypes.h>

extern enum slow5_log_level_opt  slow5_log_level;

static slow5_idx_t *s5curl_idx_init_from_url(
    s5curl_t *s5c,
    CURL *curl,
    const char *cache
) {
    slow5_idx_t *index = slow5_idx_init_empty();
    if (!index) {
        s5curl_errno = S5CURL_ERR_SLOW5;
        return NULL;
    }
    slow5_file_t *s5p = s5c->s5p;

    index->pathname = malloc(strlen(s5c->url)+5);
    if (!index->pathname) {
        slow5_idx_free(index);
        s5curl_errno = S5CURL_ERR_MEM;
        return NULL;
    }
    strcpy(index->pathname, s5c->url);
    strcat(index->pathname, ".idx");


    FILE *index_fp;

    if (cache == NULL) {
        index_fp = tmpfile();
    } else {
        index_fp = fopen(cache, "w+");
    }

    if (index_fp == NULL) {
        SLOW5_ERROR("Could not create index file for '%s'.", index->pathname);
        s5curl_errno = S5CURL_ERR_IO;
        slow5_idx_free(index);
        index->fp = NULL;
        return NULL;
    }

    curl_easy_reset(curl);
	int ret = s5curl_fetch_into_file(
        curl,
	    index_fp,
		index->pathname
	);
	if (ret != 0) {
		SLOW5_ERROR("Fetching index data of '%s' failed: %s.", index->pathname, curl_easy_strerror(ret));
        s5curl_errno = S5CURL_ERR_FETCH;
		return NULL;
	}
    long s5curl_resp_code;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &s5curl_resp_code);
    if (s5c->protocol == S5CURLP_HTTP && s5curl_resp_code != S5CURL_HTTP_OK) {
        SLOW5_ERROR("Fetching index data of '%s' failed: %li.", index->pathname, s5curl_resp_code);
        s5curl_errno = S5CURL_ERR_FETCH;
        return NULL;
    } else if (s5c->protocol == S5CURLP_FTP && s5curl_resp_code != S5CURL_FTP_OK) {
        SLOW5_ERROR("Fetching index data of '%s' failed: %li.", index->pathname, s5curl_resp_code);
        s5curl_errno = S5CURL_ERR_FETCH;
        return NULL;
    }
    if (fseek(index_fp, 0, SEEK_SET)) {
        SLOW5_ERROR("%s", "Failed to rewind index file ptr");
        s5curl_errno = S5CURL_ERR_IO;
        return NULL;
    }

    index->fp = index_fp;

    ret = slow5_idx_read(index);
    if (ret < 0) {
        SLOW5_ERROR("Reading idx failed: %s.", strerror(ret));
        s5curl_errno = S5CURL_ERR_SLOW5;
        slow5_idx_free(index);
        return NULL;
    }

    if (index->version.major != s5p->header->version.major ||
            index->version.minor != s5p->header->version.minor ||
            index->version.patch != s5p->header->version.patch) {
        SLOW5_ERROR("Index file version '" SLOW5_VERSION_STRING_FORMAT "' is different to slow5 file version '" SLOW5_VERSION_STRING_FORMAT "'. Please re-index.",
                index->version.major, index->version.minor, index->version.patch,
                s5p->header->version.major, s5p->header->version.minor, s5p->header->version.patch);
        s5curl_errno = S5CURL_ERR_VERSION;
        slow5_idx_free(index);
        return NULL;
    }

    return index;
}

int s5curl_idx_load(
    s5curl_t *s5c
) {
    CURL *curl = curl_easy_init();
    if (!curl) {
        SLOW5_ERROR("Failed to initialise CURL handle: %s.", curl_easy_strerror(CURLE_FAILED_INIT));
        return S5CURL_ERR_CURL;
    }

    s5c->s5p->index = s5curl_idx_init_from_url(s5c, curl, NULL);
    curl_easy_cleanup(curl);

    if (!s5c->s5p->index) {
        return s5curl_errno;
    }

    return S5CURL_ERR_OK;
}

int s5curl_idx_load_with(
    s5curl_t *s5c,
    const char *path
) {
    if (s5curl_is_url(path)) {
        CURL *curl = curl_easy_init();
        if (!curl) {
            SLOW5_ERROR("Failed to initialise CURL handle: %s.", curl_easy_strerror(CURLE_FAILED_INIT));
            return S5CURL_ERR_CURL;
        }
        s5c->s5p->index = s5curl_idx_init_from_url(s5c, curl, NULL);
        curl_easy_cleanup(curl);
    
        if (!s5c->s5p->index) {
            return s5curl_errno;
        }
    } else {
        int res = slow5_idx_load_with(s5c->s5p, path);
        if (res != 0) {
            s5curl_errno = res;
            return S5CURL_ERR_SLOW5;
        }
    }
    return S5CURL_ERR_OK;
}

int s5curl_idx_load_and_cache(
    s5curl_t *s5c,
    const char *cache
) {
    CURL *curl = curl_easy_init();
    if (!curl) {
        SLOW5_ERROR("Failed to initialise CURL handle: %s.", curl_easy_strerror(CURLE_FAILED_INIT));
        return S5CURL_ERR_CURL;
    }

    s5c->s5p->index = s5curl_idx_init_from_url(s5c, curl, cache);
    curl_easy_cleanup(curl);

    if (!s5c->s5p->index) {
        return s5curl_errno;
    }

    return S5CURL_ERR_OK;
}

int s5curl_idx_load_with_and_cache(
    s5curl_t *s5c,
    const char *path,
    const char *cache
) {
    if (s5curl_is_url(path)) {
        CURL *curl = curl_easy_init();
        if (!curl) {
            SLOW5_ERROR("Failed to initialise CURL handle: %s.", curl_easy_strerror(CURLE_FAILED_INIT));
            return S5CURL_ERR_CURL;
        }
        s5c->s5p->index = s5curl_idx_init_from_url(s5c, curl, cache);
        curl_easy_cleanup(curl);
    
        if (!s5c->s5p->index) {
            return s5curl_errno;
        }
    } else {
        SLOW5_INFO("%s","The provided index file is already local, and hence will not be cached to a separate location.");
        int res = slow5_idx_load_with(s5c->s5p, path);
        if (res != 0) {
            s5curl_errno = res;
            return S5CURL_ERR_SLOW5;
        }
    }
    return S5CURL_ERR_OK;
}