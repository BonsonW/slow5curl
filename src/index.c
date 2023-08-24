#include "../slow5lib/src/slow5_idx.h"
#include "../slow5lib/src/slow5_extra.h"
#include "fetch.h"
#include <slow5curl/s5curl.h>
#include <inttypes.h>

extern enum slow5_log_level_opt  slow5_log_level;

#define MAX_BUF_SIZE (32 * 1024 * 1024)
#define DOWNLOAD_SIZE (MAX_BUF_SIZE / 2)

static inline struct slow5_idx *slow5_idx_init_empty(void) {

    struct slow5_idx *index = (struct slow5_idx *) calloc(1, sizeof *index);
    SLOW5_MALLOC_CHK(index);
    index->hash = kh_init(slow5_s2i);

    return index;
}

static int slow5_idx_read(
    struct slow5_idx *index
) {
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
    struct slow5_idx *index,
    const char *url,
    CURL *curl
) {
    struct slow5_version max_supported = SLOW5_VERSION_ARRAY;
    const char magic[] = SLOW5_INDEX_MAGIC_NUMBER;
    char buf_magic[sizeof magic];
    if (fread(buf_magic, sizeof *magic, sizeof magic, index->fp) != sizeof magic) {
        return SLOW5_ERR_IO;
    }
    if (memcmp(magic, buf_magic, sizeof *magic * sizeof magic) != 0) {
        fprintf(stderr, "%s\n", buf_magic);
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

    // get file size
    curl_off_t file_size = 0;
    curl_easy_reset(curl);
    CURLcode ret = fetch_file_size(curl, &file_size, url);
    if (ret < 0) {
        SLOW5_ERROR("Fetching file size of '%s' failed: %s.", url, curl_easy_strerror(ret));
    }
    uint64_t file_offt_max = file_size;

    uint64_t real_size = DOWNLOAD_SIZE;
    uint64_t file_offt = DOWNLOAD_SIZE;

    while (1) {
        slow5_rid_len_t read_id_len;

        // download next part
        if ((uint64_t)ftell(index->fp) + (uint64_t)(read_id_len + (sizeof(uint64_t) * 2)) > real_size) {
            // copy rest of buf into start
            uint64_t rest_size = real_size - (uint64_t)ftell(index->fp);
            char *rest = malloc(rest_size);
            fread(rest, sizeof(char), rest_size, index->fp);
            fseek(index->fp, 0, SEEK_SET);
            fwrite(rest, sizeof(char), rest_size, index->fp);
            free(rest);

            // download next set of memory and copy into buffer
            curl_easy_reset(curl);
            ret = fetch_bytes_into_fb(
                curl,
                index->fp,
                url, 
                file_offt,
                DOWNLOAD_SIZE
            );
            if (ret < 0) {
                SLOW5_ERROR("Fetching index data of '%s' failed: %s.", url, curl_easy_strerror(ret));
            }
            fseek(index->fp, 0, SEEK_SET);

            // update file offset
            real_size = DOWNLOAD_SIZE + rest_size;
            file_offt += DOWNLOAD_SIZE;
        }
        
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

        // download next part
        if ((uint64_t)ftell(index->fp) + (uint64_t)(read_id_len + (sizeof(uint64_t) * 2)) > real_size) {
            // copy rest of buf into start
            uint64_t rest_size = real_size - (uint64_t)ftell(index->fp);
            char *rest = malloc(rest_size);
            fread(rest, sizeof(char), rest_size, index->fp);
            fseek(index->fp, 0, SEEK_SET);
            fwrite(rest, sizeof(char), rest_size, index->fp);
            free(rest);

            // download next set of memory and copy into buffer
            curl_easy_reset(curl);
            ret = fetch_bytes_into_fb(
                curl,
                index->fp,
                url, 
                file_offt,
                DOWNLOAD_SIZE
            );
            if (ret < 0) {
                SLOW5_ERROR("Fetching index data of '%s' failed: %s.", url, curl_easy_strerror(ret));
            }
            fseek(index->fp, 0, SEEK_SET);

            // update file offset
            real_size = DOWNLOAD_SIZE + rest_size;
            file_offt += DOWNLOAD_SIZE;
        }

        size_t bytes_read;
        bytes_read = fread(read_id, sizeof *read_id, read_id_len, index->fp);

        // check if the file offset is over the file size
        if ((file_offt - DOWNLOAD_SIZE) + (uint64_t)ftell(index->fp) > file_offt_max) {
            free(read_id);
            // todo: check EOF
            break;

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

    FILE *index_fp = fmemopen(NULL, MAX_BUF_SIZE, "r+");
    if (index_fp == NULL) {
        SLOW5_ERROR("Could not create buffer for '%s'.", index->pathname); 
        slow5_idx_free(index);
        index->fp = NULL;
        return NULL;
    }
    
    // get first part of index file
    curl_easy_reset(curl);
	CURLcode ret = fetch_bytes_into_fb(
        curl,
	    index_fp,
		index->pathname, 
		0,
		DOWNLOAD_SIZE
	);
	if (ret < 0) {
		SLOW5_ERROR("Fetching index data of '%s' failed: %s.", index->pathname, curl_easy_strerror(ret));
		return NULL;
	}
	fseek(index_fp, 0, SEEK_SET);
    
    index->fp = index_fp;

    ret = s5curl_idx_read(index, index->pathname, curl);
    if (ret < 0) {
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
