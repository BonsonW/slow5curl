#define _XOPEN_SOURCE 700
#include "../slow5lib/src/slow5_idx.h"
#include "../slow5lib/src/slow5_extra.h"
#include "fetch.h"
#include <slow5curl/s5curl.h>
#include <inttypes.h>

extern enum slow5_log_level_opt  slow5_log_level;

#define MAX_IDX_BUF_SIZE (1000 * 1024 * 1024)

static int s5curl_idx_read(struct slow5_idx *index) {

    struct slow5_version max_supported = SLOW5_VERSION_ARRAY;
    const char magic[] = SLOW5_INDEX_MAGIC_NUMBER;
    char buf_magic[sizeof magic]; // TODO is this a vla?
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

    if (fseek(index->fp, SLOW5_INDEX_HEADER_SIZE_OFFSET, SEEK_SET) == -1) {
        return SLOW5_ERR_IO;
    }

    while (1) {
        slow5_rid_len_t read_id_len;
        if (fread(&read_id_len, sizeof read_id_len, 1, index->fp) != 1) {
            SLOW5_ERROR("Malformed slow5 index. Failed to read the read ID length.%s", feof(index->fp) ? " Missing index end of file marker." : "");
            if (feof(index->fp)) {
                slow5_errno = SLOW5_ERR_TRUNC;
            } else {
                slow5_errno = SLOW5_ERR_IO;
            }
            return slow5_errno;
        }
        char *read_id = (char *) malloc((read_id_len + 1) * sizeof *read_id); // +1 for '\0'
        SLOW5_MALLOC_CHK(read_id);

        size_t bytes_read;
        if ((bytes_read = fread(read_id, sizeof *read_id, read_id_len, index->fp)) != read_id_len) {
            free(read_id);
            bytes_read += sizeof read_id_len;
            const char eof[] = SLOW5_INDEX_EOF;
            if (bytes_read == sizeof eof) {
                /* check if eof marker */
                int is_eof = slow5_is_eof(index->fp, eof, sizeof eof);
                if (is_eof == -1) { /* io/mem error */
                    SLOW5_ERROR("%s", "Internal error while checking for index eof marker.");
                } else if (is_eof == -2) {
                    SLOW5_ERROR("%s", "Malformed index. End of file marker found, but end of file not reached.");
                } else if (is_eof == 1) {
                    /* slow5_errno = SLOW5_ERR_EOF; */
                    break;
                }
            } else {
                slow5_errno = SLOW5_ERR_IO;
            }
            return slow5_errno;
        }
        read_id[read_id_len] = '\0'; // Add null byte

        uint64_t offset;
        uint64_t size;

        if (fread(&offset, sizeof offset, 1, index->fp) != 1 ||
                fread(&size, sizeof size, 1, index->fp) != 1) {
            return SLOW5_ERR_IO;
        }

        if (slow5_idx_insert(index, read_id, offset, size) == -1) {
            SLOW5_ERROR("Inserting '%s' to index failed", read_id);
            // TODO handle error and free
            return -1;
        }
    }

    return 0;
}

static inline struct slow5_idx *slow5_idx_init_empty(void) {

    struct slow5_idx *index = (struct slow5_idx *) calloc(1, sizeof *index);
    SLOW5_MALLOC_CHK(index);
    index->hash = kh_init(slow5_s2i);

    return index;
}

slow5_idx_t *slow5_idx_init_from_url(
    slow5_curl_t *s5c,
    CURL *curl
) {
    slow5_idx_t *index = slow5_idx_init_empty();
    if (!index) {
        return NULL;
    }
    slow5_file_t *s5p = s5c->s5p;

    index->pathname = malloc(strlen(s5c->url)+5);
    if (!index->pathname) {
        slow5_idx_free(index);
        return NULL;
    }
    strcpy(index->pathname, s5c->url);
    strcat(index->pathname, ".idx");

    FILE *index_fp = tmpfile();
    if (index_fp == NULL) {
        SLOW5_ERROR("Could not create temporary index file for '%s'.", index->pathname);
        slow5_idx_free(index);
        index->fp = NULL;
        return NULL;
    }

    curl_easy_reset(curl);
	int ret = fetch_into_file(
        curl,
	    index_fp,
		index->pathname
	);
	if (ret != 0) {
		SLOW5_ERROR("Fetching index data of '%s' failed: %s.", index->pathname, curl_easy_strerror(ret));
		return NULL;
	}
    fseek(index_fp, 0, SEEK_SET);

    index->fp = index_fp;

    ret = s5curl_idx_read(index);
    if (ret < 0) {
        SLOW5_ERROR("Reading idx failed: %s.", strerror(ret));
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

int s5curl_idx_load(
    slow5_curl_t *s5c
) {
    CURL *curl = curl_easy_init();
    s5c->s5p->index = slow5_idx_init_from_url(s5c, curl);
    curl_easy_cleanup(curl);
    if (s5c->s5p->index) {
        return 0;
    } else {
        return -1;
    }
}
