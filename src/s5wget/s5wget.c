#include "../slow5lib/src/slow5.c"
#include "../slow5lib/src/slow5_idx.c"

#include "s5wget.h"
#include "fetch.h"
#include "slow5/slow5.h"
#include <stdint.h>
#include <string.h>

const size_t BLOW5_HDR_META_SIZE = 68;
const size_t SLOW5_MAX_HDR_SIZE = 10 * 1024 * 1024; // 10MB max header size

struct slow5_file *s5wget_init(FILE *fp, const char *pathname, enum slow5_fmt format) {
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
        SLOW5_WARNING("Could not set a large buffer for file stream of '%s': %s.", pathname, strerror(errno));;
        free(fread_buff);
        fread_buff = NULL;
    }
    else {
        SLOW5_LOG_DEBUG("Buffer for file stream of '%s' was set to %d.", pathname, SLOW5_FSTREAM_BUFF_SIZE);
    }

    // TODO Attempt to determine from magic number

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
    s5p->meta.fd = -1;

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

slow5_file_t *s5wget_open(
    const char *url,
    enum slow5_fmt format
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

	int ret = fetch_bytes(
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
	
	if (header_size > SLOW5_MAX_HDR_SIZE) {
        SLOW5_ERROR("File '%s' has exceeded the max header size of 10MB.", url);
    }
	
	// get rest of header data
	response_t hdr = {0};

	ret = fetch_bytes(
	    &hdr,
		url, 
		0,
		header_size+BLOW5_HDR_META_SIZE
	);
	if (ret < 0) {
		SLOW5_ERROR("Reading file header of '%s' failed.", url);
		return NULL;
	}
	
	// treat header buffer as data stream
	// todo: read the header line by line with fetch_bytes
	FILE *fp = fmemopen(hdr.data, hdr.size, mode);
    
    if (!fp) {
        SLOW5_ERROR_EXIT("Error opening file '%s': %s.", url, strerror(errno));
        slow5_errno = SLOW5_ERR_IO;
        return NULL;
    }
    
    // initialize slow5 file
    struct slow5_file *s5p = s5wget_init(fp, url, format);
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
    response_free(&hdr);
    
    return s5p;
}

slow5_idx_t *slow5_idx_init_from_path(
    slow5_file_t *s5p,
    const char *path
) {
    slow5_idx_t *index = slow5_idx_init_empty();
    if (!index) {
        return NULL;
    }
    
    index->pathname = strdup(path);

    FILE *index_fp;

    if ((index_fp = fopen(index->pathname, "r")) == NULL) {
        SLOW5_ERROR("Index file not found. Creating an index at '%s'.", index->pathname);
        fclose(index->fp);
        slow5_idx_free(index);
        index->fp = NULL;
        return NULL;
    }
    
    index->fp = index_fp;
    
    // todo: verify that the idx file is up to date
    
    if (slow5_idx_read(index) != 0) {
        slow5_idx_free(index);
        return NULL;
    }
    
    if (index->version.major != s5p->header->version.major ||
            index->version.minor != s5p->header->version.minor ||
            index->version.patch != s5p->header->version.patch) {
        SLOW5_ERROR("Index file version '" SLOW5_VERSION_STRING_FORMAT "' is different to slow5 file version '" SLOW5_VERSION_STRING_FORMAT "'. Please re-index.",
                index->version.major, index->version.minor, index->version.patch,
                s5p->header->version.major, s5p->header->version.minor, s5p->header->version.patch);
        slow5_idx_free(index);
        return NULL;
    }

    return index;
}

int slow5_idx_load_from_path(
    slow5_file_t *s5p,
    const char *path
) {
    s5p->index = slow5_idx_init_from_path(s5p, path);
    if (s5p->index) {
        return 0;
    } else {
        return -1;
    }
}


// fetch read from URL given slow5 pointer and slow5 index 
int s5wget_read(
    const char *url,
    const char *read_id,
    slow5_file_t *sp,
    slow5_idx_t *s_idx,
    slow5_rec_t *read
) {
    struct slow5_rec_idx read_index;
	int ret = slow5_idx_get(s_idx, read_id, &read_index);
	if (ret < 0) {
		SLOW5_ERROR("Error getting index for read %s.", read_id);
		return -1;
	}
	
	// exclude meta data before copying record
	size_t bytes = read_index.size - sizeof(slow5_rec_size_t);
	response_t resp = {0};

	ret = fetch_bytes(
	    &resp,
		url, 
		read_index.offset,
		read_index.size
	);
	if (ret < 0) {
		SLOW5_ERROR("Error fetching bytes for read %s.", read_id);
		return -1;
	}
	
	char *read_start = resp.data + sizeof(slow5_rec_size_t);

	ret = slow_decode((void *)&read_start, &bytes, &read, sp);
	slow5_rec_free(read);
	response_free(&resp);

	if (ret < 0) { 
		SLOW5_ERROR("Error decoding read %s\n", read_id);
		return -1;
	}

    return 0;
}
