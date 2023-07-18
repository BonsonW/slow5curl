#include "../slow5lib/src/slow5.c"
#include "../slow5lib/src/slow5_idx.c"

#include "s5curl.h"
#include "fetch.h"
#include "slow5/slow5.h"
#include <stdint.h>
#include <string.h>

const size_t BLOW5_HDR_META_SIZE = 68;
const size_t BLOW5_MAX_HDR_SIZE = 10 * 1024 * 1024; // 10MB max header size

// fetch read from URL given slow5 pointer and slow5 index 
int s5curl_read(
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

	ret = fetch_bytes_into_resp(
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

slow5_file_t *s5curl_file(
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
        SLOW5_ERROR("File '%s' has exceeded the max header size of 10MB.", url);
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
    struct slow5_file *s5p = s5curl_init(fp, url, format);
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

static int s5curl_idx_read(
    struct slow5_idx *index
) {
    struct slow5_version max_supported = SLOW5_VERSION_ARRAY;
    const char magic[] = SLOW5_INDEX_MAGIC_NUMBER;
    char buf_magic[sizeof magic];
    if (fread(buf_magic, sizeof *magic, sizeof magic, index->fp) != sizeof magic) {
        return SLOW5_ERR_IO;
    }
    if (memcmp(magic, buf_magic, sizeof *magic * sizeof magic) != 0) {
        return SLOW5_ERR_MAGIC;
    }
    
    if (fread(&index->version.major, sizeof index->version.major, 1, index->fp) != 1 ||
        fread(&index->version.minor, sizeof index->version.minor, 1, index->fp) != 1 ||
        fread(&index->version.patch, sizeof index->version.patch, 1, index->fp) != 1) {
        return SLOW5_ERR_IO;
    }

    if (slow5_is_version_compatible(index->version, max_supported) == 0){
        SLOW5_ERROR("Index file version '" SLOW5_VERSION_STRING_FORMAT "' is higher than the max slow5 version '" SLOW5_VERSION_STRING "' supported by this slow5lib! Please re-index or use a newer version of slow5lib.",
                index->version.major, index->version.minor, index->version.patch);
        return SLOW5_ERR_VERSION;
    }
    
    uint64_t read_offset = SLOW5_INDEX_HEADER_SIZE_OFFSET;
    while (1) {
        slow5_rid_len_t read_id_len;

        // fetch read id len
        response_t hdr_rid_len = {0};
    	if ((fetch_bytes_into_resp(&hdr_rid_len, index->pathname, read_offset, 2)) < 0) {
    		SLOW5_ERROR("Fetching read_id from '%s' failed.", index->pathname);
    		return SLOW5_ERR_IO;
    	}
    	read_offset += 2;
    	
    	memcpy(&read_id_len, hdr_rid_len.data, 2);
        char *read_id = (char *) malloc((read_id_len + 1) * sizeof *read_id); // +1 for '\0'
        SLOW5_MALLOC_CHK(read_id);
        fprintf(stderr, "read_id_len: %u\n", read_id_len);
        
        // fetch entry data
        response_t hdr = {0};
        uint64_t entry_size = read_id_len + (2 * sizeof(uint64_t));
    	if ((fetch_bytes_into_resp(&hdr, index->pathname,  read_offset, entry_size)) < 0) {
    		SLOW5_ERROR("Fetching entry from '%s' failed.", index->pathname);
    		return SLOW5_ERR_IO;
    	}
    	read_offset += entry_size;
    	
    	fprintf(stderr, "file pointer: %zu\n", read_offset);

        memcpy(read_id, hdr.data, read_id_len);
        read_id[read_id_len] = '\0'; // Add null byte

        uint64_t offset;
        uint64_t size;
        
        memcpy(&offset, hdr.data + read_id_len, sizeof offset);
        memcpy(&size, hdr.data + read_id_len + sizeof offset, sizeof size);

        // if (slow5_idx_insert(index, read_id, offset, size) == -1) {
        //     SLOW5_ERROR("Inserting '%s' to index failed", read_id);
        //     // TODO handle error and free
        //     return -1;
        // }
        
        fprintf(stderr, "id:        %s\n", read_id);
        fprintf(stderr, "offset:    %zu\n", offset);
        fprintf(stderr, "size:      %zu\n\n", size);
        
        response_free(&hdr_rid_len);
        response_free(&hdr);
    }

    return 0;
}

slow5_idx_t *slow5_idx_init_from_url(
    slow5_file_t *s5p,
    const char *url
) {
    slow5_idx_t *index = slow5_idx_init_empty();
    if (!index) {
        return NULL;
    }
    
    index->pathname = strdup(url);

    FILE *index_fp = fmemopen(NULL, SLOW5_INDEX_HEADER_SIZE_OFFSET+1, "r+");
    if (index_fp == NULL) {
        SLOW5_ERROR("Could not create buffer for '%s'.", index->pathname);
        slow5_idx_free(index);
        index->fp = NULL;
        return NULL;
    }
    
    // get header meta data
	int ret = fetch_bytes_into_fb(
	    index_fp,
		url, 
		0,
		SLOW5_INDEX_HEADER_SIZE_OFFSET
	);
	if (ret < 0) {
		SLOW5_ERROR("Reading file header meta data of '%s' failed.", url);
		return NULL;
	}
	fseek(index_fp, 0, SEEK_SET);
    
    index->fp = index_fp;
    
    // todo: verify that the idx file is up to date
    
    ret = s5curl_idx_read(index);
    if (ret != 0) {
        SLOW5_ERROR("Error reading idx %s.", strerror(ret));
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

int s5curl_idx(
    slow5_file_t *s5p,
    const char *url
) {
    s5p->index = slow5_idx_init_from_url(s5p, url);
    if (s5p->index) {
        return 0;
    } else {
        return -1;
    }
}
