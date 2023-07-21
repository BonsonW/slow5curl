#include "../slow5lib/src/slow5_idx.h"
#include "../slow5lib/src/slow5_extra.h"
#include "index.h"

extern enum slow5_log_level_opt  slow5_log_level;

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

// static int s5curl_idx_read(
//     struct slow5_idx *index
// ) {
//     struct slow5_version max_supported = SLOW5_VERSION_ARRAY;
//     const char magic[] = SLOW5_INDEX_MAGIC_NUMBER;
//     char buf_magic[sizeof magic];
//     if (fread(buf_magic, sizeof *magic, sizeof magic, index->fp) != sizeof magic) {
//         return SLOW5_ERR_IO;
//     }
//     if (memcmp(magic, buf_magic, sizeof *magic * sizeof magic) != 0) {
//         return SLOW5_ERR_MAGIC;
//     }
    
//     if (fread(&index->version.major, sizeof index->version.major, 1, index->fp) != 1 ||
//         fread(&index->version.minor, sizeof index->version.minor, 1, index->fp) != 1 ||
//         fread(&index->version.patch, sizeof index->version.patch, 1, index->fp) != 1) {
//         return SLOW5_ERR_IO;
//     }

//     if (slow5_is_version_compatible(index->version, max_supported) == 0){
//         SLOW5_ERROR("Index file version '" SLOW5_VERSION_STRING_FORMAT "' is higher than the max slow5 version '" SLOW5_VERSION_STRING "' supported by this slow5lib! Please re-index or use a newer version of slow5lib.",
//                 index->version.major, index->version.minor, index->version.patch);
//         return SLOW5_ERR_VERSION;
//     }
    
//     uint64_t read_offset = SLOW5_INDEX_HEADER_SIZE_OFFSET;
//     while (1) {
//         slow5_rid_len_t read_id_len;

//         // fetch read id len
//         response_t hdr_rid_len = {0};
//     	if ((fetch_bytes_into_resp(&hdr_rid_len, index->pathname, read_offset, 2)) < 0) {
//     		SLOW5_ERROR("Fetching read_id from '%s' failed.", index->pathname);
//     		return SLOW5_ERR_IO;
//     	}
//     	read_offset += 2;
    	
//     	memcpy(&read_id_len, hdr_rid_len.data, 2);
//         char *read_id = (char *) malloc((read_id_len + 1) * sizeof *read_id); // +1 for '\0'
//         SLOW5_MALLOC_CHK(read_id);
//         fprintf(stderr, "read_id_len: %u\n", read_id_len);
        
//         // fetch entry data
//         response_t hdr = {0};
//         uint64_t entry_size = read_id_len + (2 * sizeof(uint64_t));
//     	if ((fetch_bytes_into_resp(&hdr, index->pathname,  read_offset, entry_size)) < 0) {
//     		SLOW5_ERROR("Fetching entry from '%s' failed.", index->pathname);
//     		return SLOW5_ERR_IO;
//     	}
//     	read_offset += entry_size;
    	
//     	fprintf(stderr, "file pointer: %zu\n", read_offset);

//         memcpy(read_id, hdr.data, read_id_len);
//         read_id[read_id_len] = '\0'; // Add null byte

//         uint64_t offset;
//         uint64_t size;
        
//         memcpy(&offset, hdr.data + read_id_len, sizeof offset);
//         memcpy(&size, hdr.data + read_id_len + sizeof offset, sizeof size);

//         // if (slow5_idx_insert(index, read_id, offset, size) == -1) {
//         //     SLOW5_ERROR("Inserting '%s' to index failed", read_id);
//         //     // TODO handle error and free
//         //     return -1;
//         // }
        
//         fprintf(stderr, "id:        %s\n", read_id);
//         fprintf(stderr, "offset:    %zu\n", offset);
//         fprintf(stderr, "size:      %zu\n\n", size);
        
//         response_free(&hdr_rid_len);
//         response_free(&hdr);
//     }

//     return 0;
// }

// slow5_idx_t *slow5_idx_init_from_url(
//     slow5_file_t *s5p,
//     const char *url
// ) {
//     slow5_idx_t *index = slow5_idx_init_empty();
//     if (!index) {
//         return NULL;
//     }
    
//     index->pathname = strdup(url);

//     FILE *index_fp = fmemopen(NULL, SLOW5_INDEX_HEADER_SIZE_OFFSET+1, "r+");
//     if (index_fp == NULL) {
//         SLOW5_ERROR("Could not create buffer for '%s'.", index->pathname);
//         slow5_idx_free(index);
//         index->fp = NULL;
//         return NULL;
//     }
    
//     // get header meta data
// 	int ret = fetch_bytes_into_fb(
// 	    index_fp,
// 		url, 
// 		0,
// 		SLOW5_INDEX_HEADER_SIZE_OFFSET
// 	);
// 	if (ret < 0) {
// 		SLOW5_ERROR("Reading file header meta data of '%s' failed.", url);
// 		return NULL;
// 	}
// 	fseek(index_fp, 0, SEEK_SET);
    
//     index->fp = index_fp;
    
//     // todo: verify that the idx file is up to date
    
//     ret = s5curl_idx_read(index);
//     if (ret != 0) {
//         SLOW5_ERROR("Error reading idx %s.", strerror(ret));
//         slow5_idx_free(index);
//         return NULL;
//     }
    
//     if (index->version.major != s5p->header->version.major ||
//             index->version.minor != s5p->header->version.minor ||
//             index->version.patch != s5p->header->version.patch) {
//         SLOW5_ERROR("Index file version '" SLOW5_VERSION_STRING_FORMAT "' is different to slow5 file version '" SLOW5_VERSION_STRING_FORMAT "'. Please re-index.",
//                 index->version.major, index->version.minor, index->version.patch,
//                 s5p->header->version.major, s5p->header->version.minor, s5p->header->version.patch);
//         slow5_idx_free(index);
//         return NULL;
//     }

//     return index;
// }

// int s5curl_idx(
//     slow5_curl_t *s5c
// ) {
//     slow5_file_t *s5p = s5c->s5p;
//     s5p->index = slow5_idx_init_from_url(s5p, url);
//     if (s5p->index) {
//         return 0;
//     } else {
//         return -1;
//     }
// }
