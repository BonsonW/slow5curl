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
#include <stdint.h>
#include <string.h>

#include <curl/curl.h>

#include "../slow5lib/src/slow5_idx.h"
#include "../slow5lib/src/slow5_extra.h"
#include "../slow5lib/src/slow5_misc.h"
#include <slow5curl/s5curl.h>
#include "fetch.h"

#define SLOW5_FSTREAM_BUFF_SIZE (131072) // buffer size for freads and fwrites
extern enum slow5_log_level_opt  slow5_log_level;
extern enum slow5_exit_condition_opt slow5_exit_condition;

#define BLOW5_HDR_META_SIZE (68)
#define BLOW5_MAX_HDR_SIZE (32 * 1024 * 1024) // 32MB max header size

void s5curl_global_init() {
    curl_global_init(CURL_GLOBAL_ALL);
}

void s5curl_global_cleanup() {
    curl_global_cleanup();
}

S5CURLCONN *s5curl_conn_init() {
    return (S5CURLCONN *)curl_easy_init();
}

void s5curl_conn_cleanup(S5CURLCONN *conn) {
    curl_easy_cleanup(conn);
}

void s5curl_close(
    s5curl_t *s5c
) {
    slow5_close(s5c->s5p);
    free(s5c->url);
    free(s5c);
}

void s5curl_idx_unload(
    s5curl_t *s5c
) {
    slow5_idx_free(s5c->s5p->index);
    s5c->s5p->index = NULL;
}

static struct slow5_file *s5curl_init(
    FILE *fp,
    const char *pathname,
    enum slow5_fmt format
) {
    // pathname allowed to be NULL at this point
    if (!fp) {
        SLOW5_ERROR("Argument '%s' cannot be NULL.", SLOW5_TO_STR(fp));
        slow5_errno = SLOW5_ERR_ARG;
        return NULL;
    }

    char *fread_buff = (char *)calloc(SLOW5_FSTREAM_BUFF_SIZE, sizeof(char));
    if (!fread_buff) {
        SLOW5_MALLOC_ERROR();
        slow5_errno = SLOW5_ERR_MEM;
        return NULL;
    }

    if (setvbuf(fp, fread_buff, _IOFBF, SLOW5_FSTREAM_BUFF_SIZE) != 0){
        SLOW5_WARNING("Could not set a large buffer for file stream of '%s': %s.", pathname, strerror(errno));
        free(fread_buff);
        fread_buff = NULL;
    } else {
        SLOW5_LOG_DEBUG("Buffer for file stream of '%s' was set to %d.", pathname, SLOW5_FSTREAM_BUFF_SIZE);
    }

    slow5_press_method_t method;
    struct slow5_hdr *header = slow5_hdr_init(fp, format, &method);
    if (!header) {
        free(fread_buff);
        SLOW5_ERROR("Parsing slow5 header of file '%s' failed.", pathname);
        return NULL;
    }

    struct slow5_file *s5p = (struct slow5_file *)calloc(1, sizeof *s5p);
    if (!s5p) {
        SLOW5_MALLOC_ERROR();
        slow5_hdr_free(header);
        free(fread_buff);
        slow5_errno = SLOW5_ERR_MEM;
        return NULL;
    }

    s5p->fp = fp;
    s5p->format = format;
    s5p->header = header;
    s5p->meta.fread_buffer = fread_buff;

    s5p->compress = slow5_press_init(method);
    if (!s5p->compress) {
        free(fread_buff);
        slow5_hdr_free(header);
        free(s5p);
        return NULL;
    }

    s5p->meta.pathname = pathname;
    if ((s5p->meta.start_rec_offset = ftello(fp)) == -1) {
        SLOW5_ERROR("Obtaining file offset with ftello() failed: %s.", strerror(errno));
        free(fread_buff);
        slow5_press_free(s5p->compress);
        slow5_hdr_free(header);
        free(s5p);
        slow5_errno = SLOW5_ERR_IO;
        return NULL;
    }

    return s5p;
}

static s5curl_t *s5curl_open_with(
    const char *url,
    CURL *curl,
    const char *mode
) {
    if (slow5_is_big_endian()) {
        SLOW5_ERROR_EXIT("%s", "Big endian machine detected. slow5lib only supports little endian at this time. Please open a github issue stating your machine spec <https://github.com/hasindu2008/slow5lib/issues>.");
        slow5_errno = SLOW5_ERR_OTH;
        return NULL;
    }
    if (!url) {
        SLOW5_ERROR_EXIT("Argument '%s' cannot be NULL.", SLOW5_TO_STR(url));
        slow5_errno = SLOW5_ERR_ARG;
        return NULL;
    }

    // check protocol
    S5CURLProtocol protocol = S5CURLP_UNKOWN;
    char ftp[] = "ftp";
    char http[] = "http";
    char buf[strlen(url)+1];

    memcpy(buf, url, strlen(ftp));
    buf[strlen(ftp)] = '\0';

    if (strcmp(ftp, buf) == 0) protocol = S5CURLP_FTP;

    memcpy(buf, url, strlen(http));
    buf[strlen(http)] = '\0';

    if (strcmp(http, buf) == 0) protocol = S5CURLP_HTTP;

    // get header meta data
    s5curl_resp_t *hdr_meta = s5curl_resp_init();

    curl_easy_reset(curl);
	int res = s5curl_fetch_bytes_into_resp(
        curl,
	    hdr_meta,
		url,
		0,
		BLOW5_HDR_META_SIZE
	);
	if (res != 0) {
		SLOW5_ERROR("Fetching file header meta data of '%s' failed: %s.", url, curl_easy_strerror(res));
        slow5_errno = SLOW5_ERR_OTH;
		return NULL;
	}
    long s5curl_resp_code;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &s5curl_resp_code);
    if (protocol == S5CURLP_HTTP &&  s5curl_resp_code != S5CURL_HTTP_PARTIAL) {
        SLOW5_ERROR("Fetching file header data of '%s' failed: %li.", url, s5curl_resp_code);
        return NULL;
    } else if (protocol == S5CURLP_FTP && s5curl_resp_code != S5CURL_FTP_PARTAL) {
        SLOW5_ERROR("Fetching file header data of '%s' failed: %li.", url, s5curl_resp_code);
        return NULL;
    }

    
	uint32_t header_size = 0;
	memcpy((void *)&header_size, hdr_meta->data + 64, 4);

	if (header_size > BLOW5_MAX_HDR_SIZE) {
        SLOW5_ERROR("File '%s' with header size %u has exceeded the max header size.", url, header_size);
        slow5_errno = SLOW5_ERR_OTH;
        return NULL;
    }

	// get rest of header
	FILE *fp = fmemopen(NULL, header_size+BLOW5_HDR_META_SIZE+1, "r+");
	if (!fp) {
        SLOW5_ERROR_EXIT("Error opening file '%s': %s.", url, strerror(errno));
        slow5_errno = SLOW5_ERR_IO;
        return NULL;
    }

    curl_easy_reset(curl);
	res = s5curl_fetch_bytes_into_file(
        curl,
	    fp,
		url,
		0,
		header_size+BLOW5_HDR_META_SIZE
	);
	if (res != 0) {
		SLOW5_ERROR("Fetching file header of '%s' failed: %s.", url, curl_easy_strerror(res));
        slow5_errno = SLOW5_ERR_OTH;
		return NULL;
	}
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &s5curl_resp_code);
    if (protocol == S5CURLP_HTTP &&  s5curl_resp_code != S5CURL_HTTP_PARTIAL) {
        SLOW5_ERROR("Fetching file header data of '%s' failed: %li.", url, s5curl_resp_code);
        return NULL;
    } else if (protocol == S5CURLP_FTP && s5curl_resp_code != S5CURL_FTP_PARTAL) {
        SLOW5_ERROR("Fetching file header data of '%s' failed: %li.", url, s5curl_resp_code);
        return NULL;
    }
	fseek(fp, 0, SEEK_SET);

    // initialize slow5 file
    struct slow5_file *s5p = s5curl_init(fp, url, SLOW5_FORMAT_BINARY);
    if (!s5p) {
        if (fclose(fp) == EOF) {
            SLOW5_ERROR("Error closing file '%s': %s.", url, strerror(errno));
        }
        SLOW5_EXIT_IF_ON_ERR();
    } else {
        s5p->meta.mode = mode;
    }

    s5curl_t *s5c = (s5curl_t *)calloc(1, sizeof *s5c);
    s5c->url = strdup(url);
    s5c->s5p = s5p;
    s5c->protocol = protocol;

    // cleanup
    s5curl_resp_cleanup(hdr_meta);

    return s5c;
}

s5curl_t *s5curl_open(
    const char *url
) {
    CURL *curl = curl_easy_init();
    if (!curl) {
        SLOW5_ERROR("Failed to initialize connection for url '%s'.", url);
        return NULL;
    }

    s5curl_t *ret = s5curl_open_with(url, curl, "r");
    curl_easy_cleanup(curl);
    return ret;
}

int s5curl_get(
    const char *read_id,
    slow5_rec_t **record,
    CURL *curl,
    s5curl_t *s5c
) {
    if (!curl) {
        SLOW5_ERROR("Invalid CURL handle passed for fetching read: %s.", read_id);
        return S5CURL_ERR_CURL;
    }
    curl_easy_reset(curl);

    // get offset and size
    struct slow5_rec_idx read_index;
	if (slow5_idx_get(s5c->s5p->index, read_id, &read_index) < 0) {
		SLOW5_ERROR("Error getting index for read %s.", read_id);
		slow5_errno = SLOW5_ERR_NOTFOUND;
        return S5CURL_ERR_SLOW5;
	}
    
    // fetch
	s5curl_resp_t *resp = s5curl_resp_init();

	int res = s5curl_fetch_bytes_into_resp(
        curl,
	    resp,
		s5c->url, 
		read_index.offset + sizeof(slow5_rec_size_t),
		read_index.size - sizeof(slow5_rec_size_t)
	);
	if (res != 0) {
		SLOW5_ERROR("Fetch bytes for read %s failed: %s.", read_id, curl_easy_strerror(res));
		return S5CURL_ERR_FETCH;
	}
    long s5curl_resp_code;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &s5curl_resp_code);
    if (s5c->protocol == S5CURLP_HTTP && s5curl_resp_code != S5CURL_HTTP_PARTIAL) {
        SLOW5_ERROR("Fetching read %s failed with error code: %li.", read_id, s5curl_resp_code);
        return S5CURL_ERR_FETCH;
    } else if (s5c->protocol == S5CURLP_FTP && s5curl_resp_code != S5CURL_FTP_PARTAL) {
        SLOW5_ERROR("Fetching read %s failed with error code: %li.", read_id, s5curl_resp_code);
        return S5CURL_ERR_FETCH;
    }

    res = slow5_decode((void *)&resp->data, &resp->size, record, s5c->s5p);
    if (res < 0) {
        SLOW5_ERROR("Error decoding read %s.", read_id);
        return S5CURL_ERR_SLOW5;
    }

    // cleanup
    s5curl_resp_cleanup(resp);

    return S5CURL_ERR_OK;
}
