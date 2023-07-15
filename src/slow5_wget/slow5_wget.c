#include "../slow5lib/src/slow5.c"

#include "slow5_wget.h"
#include "fetch.h"

const size_t BLOW5_HDR_SIZE = 68;

struct slow5_file *slow5_wget_open(
    const char *url,
    enum slow5_fmt format
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
    
    const char *mode = "r";
    
    response_t resp = {0};

	int ret = get_object_bytes(
	    &resp,
		url, 
		0,
		BLOW5_HDR_SIZE
	);
	if (ret < 0) {
		SLOW5_ERROR("Reading file header of '%s' failed.", url);
		return NULL;
	}
	
	FILE *fp = fmemopen(resp.data, resp.size, mode);

    if (!fp) {
        SLOW5_ERROR_EXIT("Error opening file '%s': %s.", url, strerror(errno));
        slow5_errno = SLOW5_ERR_IO;
        return NULL;
    }

    struct slow5_file *s5p = slow5_init(fp, url, format);
    if (!s5p) {
        if (fclose(fp) == EOF) {
            SLOW5_ERROR("Error closing file '%s': %s.", url, strerror(errno));
        }
        SLOW5_EXIT_IF_ON_ERR();
    } else {
        s5p->meta.mode = mode;
    }
    
    response_free(&resp);
    return s5p;
}
