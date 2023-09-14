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

static s5curl_conn_stack_t *s5curl_open_conns(
    int32_t n_conns
) {
    if (n_conns <= 0) {
        SLOW5_ERROR("%s", "Connection size must be a positive number.");
        slow5_errno = SLOW5_ERR_OTH;
        return NULL;
    }

    s5curl_conn_stack_t *s5curl_conn_stack = calloc(1, sizeof *s5curl_conn_stack);
    if (!s5curl_conn_stack) {
        SLOW5_MALLOC_ERROR();
        slow5_errno = SLOW5_ERR_MEM;
        return NULL;
    }

    s5curl_conn_stack->curls = calloc(n_conns, sizeof *s5curl_conn_stack->curls);
    if (!s5curl_conn_stack->curls) {
        SLOW5_MALLOC_ERROR();
        free(s5curl_conn_stack);
        slow5_errno = SLOW5_ERR_MEM;
        return NULL;
    }

    for (size_t i = 0; i < n_conns; ++i) {
        s5curl_conn_stack->curls[i] = curl_easy_init();
        if (!s5curl_conn_stack->curls[i]) {
            SLOW5_ERROR("Error initializing connection: %s", curl_easy_strerror(CURLE_FAILED_INIT));
            free(s5curl_conn_stack);
            free(s5curl_conn_stack->curls);
            slow5_errno = SLOW5_ERR_OTH;
            return NULL;
        }
        curl_easy_reset(s5curl_conn_stack->curls[i]);
    }
    s5curl_conn_stack->top = n_conns-1;
    s5curl_conn_stack->n_conns = n_conns;

    return s5curl_conn_stack;
}

static void s5curl_close_conns(
    s5curl_conn_stack_t *conns
) {
    for (size_t i = 0; i < conns->n_conns; ++i) {
        curl_easy_cleanup(conns->curls[i]);
    }
    free(conns->curls);
    free(conns);
}

CURL *s5curl_conns_pop(
    s5curl_conn_stack_t *conns
) {
    if (conns->top < 0) return NULL;
    curl_easy_reset(conns->curls[conns->top]);
    return conns->curls[conns->top--];
}

int s5curl_conns_push(
    s5curl_conn_stack_t *conns,
    CURL *curl
) {
    if (conns->top + 1 >= conns->n_conns) return -1;
    conns->curls[++conns->top] = curl;
    return 0;
}

s5curl_multi_t *s5curl_multi_open(
    int32_t n_conns
) {
    s5curl_multi_t *s5curl_multi = calloc(1, sizeof *s5curl_multi);
    if (!s5curl_multi) {
        SLOW5_MALLOC_ERROR();
        slow5_errno = SLOW5_ERR_MEM;
        return NULL;
    }

    s5curl_multi->conns = s5curl_open_conns(n_conns);
    if (!s5curl_multi->conns) {
        free(s5curl_multi);
        SLOW5_MALLOC_ERROR();
        return NULL;
    }

    return s5curl_multi;
}

void s5curl_multi_close(
    s5curl_multi_t *s5curl_multi
) {
    s5curl_close_conns(s5curl_multi->conns);
    free(s5curl_multi);
}

void s5curl_close(
    slow5_curl_t *s5c
) {
    slow5_close(s5c->s5p);
    free(s5c->url);
    free(s5c);
}

void s5curl_idx_unload(
    slow5_curl_t *s5c
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

    // determine format from pathname
    if (format == SLOW5_FORMAT_UNKNOWN &&
            (format = slow5_path_get_fmt(pathname)) == SLOW5_FORMAT_UNKNOWN) {
        SLOW5_ERROR("Unknown slow5 format for file '%s'. Extension must be '%s' or '%s'.",
                pathname, SLOW5_ASCII_EXTENSION, SLOW5_BINARY_EXTENSION);
        slow5_errno = SLOW5_ERR_UNK;
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

slow5_curl_t *s5curl_open_with(
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
        if (!url) {
            SLOW5_ERROR_EXIT("Argument '%s' cannot be NULL.", SLOW5_TO_STR(url));
        }
        slow5_errno = SLOW5_ERR_ARG;
        return NULL;
    }

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
	fseek(fp, 0, SEEK_SET);

    // initialize slow5 file
    struct slow5_file *s5p = s5curl_init(fp, url, SLOW5_FORMAT_UNKNOWN);
    if (!s5p) {
        if (fclose(fp) == EOF) {
            SLOW5_ERROR("Error closing file '%s': %s.", url, strerror(errno));
        }
        SLOW5_EXIT_IF_ON_ERR();
    } else {
        s5p->meta.mode = mode;
    }

    slow5_curl_t *s5c = (slow5_curl_t *)calloc(1, sizeof *s5c);
    s5c->url = strdup(url);
    s5c->s5p = s5p;

    // cleanup
    s5curl_resp_cleanup(hdr_meta);

    return s5c;
}

slow5_curl_t *s5curl_open(
    const char *url
) {
    CURL *curl = curl_easy_init();
    slow5_curl_t *ret = s5curl_open_with(url, curl, "r");
    curl_easy_cleanup(curl);
    return ret;
}
