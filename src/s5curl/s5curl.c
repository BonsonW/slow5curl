#include <stdint.h>
#include <string.h>

#include "../slow5lib/src/slow5_idx.h"
#include "../slow5lib/src/slow5_extra.h"
#include "../slow5lib/src/slow5_misc.h"
#include "s5curl.h"
#include "fetch.h"

#define SLOW5_FSTREAM_BUFF_SIZE (131072)  /* buffer size for freads and fwrites */
extern enum slow5_log_level_opt  slow5_log_level;
enum slow5_exit_condition_opt slow5_exit_condition = SLOW5_EXIT_OFF;

const size_t BLOW5_HDR_META_SIZE = 68;
const size_t BLOW5_MAX_HDR_SIZE = 32 * 1024 * 1024; // 32MB max header size

void s5curl_close(
    slow5_curl_t *s5c
) {
    slow5_close(s5c->s5p);
}

void s5curl_idx_unload(
    slow5_curl_t *s5c
) {
    slow5_idx_free(s5c->s5p->index);
    s5c->s5p->index = NULL;
}

struct slow5_file *s5curl_init(
    FILE *fp,
    const char *pathname,
    enum slow5_fmt format
) {
    /* pathname allowed to be NULL at this point */
    if (!fp) {
        SLOW5_ERROR("Argument '%s' cannot be NULL.", SLOW5_TO_STR(fp));
        slow5_errno = SLOW5_ERR_ARG;
        return NULL;
    }

    // Attempt to determine format from pathname
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
    }
    else {
        SLOW5_LOG_DEBUG("Buffer for file stream of '%s' was set to %d.", pathname, SLOW5_FSTREAM_BUFF_SIZE);
    }

    slow5_press_method_t method;
    struct slow5_hdr *header = slow5_hdr_init(fp, format, &method);
    if (!header) {
        free(fread_buff);
        SLOW5_ERROR("Parsing slow5 header of file '%s' failed.", pathname);
        return NULL;
    }

    struct slow5_file *s5p = (struct slow5_file *) calloc(1, sizeof *s5p);
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

slow5_curl_t *s5curl_open(
    const char *url
) {
    const char *mode = "r";
    
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
    response_t hdr_meta = {0};

	int ret = fetch_bytes_into_resp(
	    &hdr_meta,
		url, 
		0,
		BLOW5_HDR_META_SIZE
	);
	if (ret < 0) {
		SLOW5_ERROR("Reading file header meta data of '%s' failed.", url);
		return NULL;
	}
	
	// get header size
	uint32_t header_size = 0;
	memcpy((void *)&header_size, hdr_meta.data + 64, 4);
	
	if (header_size > BLOW5_MAX_HDR_SIZE) {
        SLOW5_ERROR("File '%s' has exceeded the max header size.", url);
    }
	
	// get rest of header data
	FILE *fp = fmemopen(NULL, header_size+BLOW5_HDR_META_SIZE+1, "r+");
	if (!fp) {
        SLOW5_ERROR_EXIT("Error opening file '%s': %s.", url, strerror(errno));
        slow5_errno = SLOW5_ERR_IO;
        return NULL;
    }

	ret = fetch_bytes_into_fb(
	    fp,
		url, 
		0,
		header_size+BLOW5_HDR_META_SIZE
	);
	if (ret < 0) {
		SLOW5_ERROR("Reading file header of '%s' failed.", url);
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
    
    // cleanup
    response_free(&hdr_meta);

    slow5_curl_t *s5c = (slow5_curl_t *)calloc(1, sizeof *s5c);
    s5c->url = strdup(url);
    s5c->s5p = s5p;

    return s5c;
}
