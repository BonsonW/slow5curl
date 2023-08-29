#include <stdint.h>
#include <curl/curl.h>

#include "../slow5lib/src/slow5_idx.h"
#include "fetch.h"
#include <slow5curl/s5curl.h>

extern enum slow5_log_level_opt  slow5_log_level;

int s5curl_get(
    slow5_curl_t *s5c,
    CURL *curl, 
    const char *read_id,
    slow5_rec_t **record
) {
    if (!curl) {
        SLOW5_ERROR("Get single read %s failed: %s.", read_id, curl_easy_strerror(CURLE_FAILED_INIT));
        return SLOW5_ERR_OTH;
    }
    curl_easy_reset(curl);

    // get offset and size
    struct slow5_rec_idx read_index;
	if (slow5_idx_get(s5c->s5p->index, read_id, &read_index) < 0) {
		SLOW5_ERROR("Error getting index for read %s.", read_id);
		return SLOW5_ERR_NOTFOUND;
	}
    
    // fetch
	response_t *resp = response_init();

	int res = fetch_bytes_into_resp(
        curl,
	    resp,
		s5c->url, 
		read_index.offset + sizeof(slow5_rec_size_t),
		read_index.size - sizeof(slow5_rec_size_t)
	);
	if (res != 0) {
		SLOW5_ERROR("Fetch bytes for read %s failed: %s.", read_id, curl_easy_strerror(res));
		return SLOW5_ERR_OTH;
	}

    long response_code;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
    if (response_code != 206) {
        SLOW5_ERROR("Fetching read %s failed with error code: %li.", read_id, response_code);
    }

    // decode
    res = slow5_decode((void *)&resp->data, &resp->size, record, s5c->s5p);
    if (res < 0) SLOW5_ERROR("Error decoding read %s.", read_id);

    // cleanup
    response_cleanup(resp);

    if (res < 0) {
		SLOW5_ERROR("Decoding read %s failed.", read_id);
		return SLOW5_ERR_OTH;
	}

    return EXIT_SUCCESS;
}

typedef struct indexed_resp {
    response_t *resp;
    uint32_t index;
} indexed_resp_t;

static int add_transfer(
    CURL *curl,
    slow5_curl_t *s5c,
    CURLM *curl_multi,
    char *read_id,
    uint32_t transfer,
    int *left
) {
    if (!curl) {
        SLOW5_ERROR("Queueing read %s failed: %s.", read_id, curl_easy_strerror(CURLE_FAILED_INIT));
        return SLOW5_ERR_OTH;
    }
    
    // get offset and size
    struct slow5_rec_idx read_index;
	if (slow5_idx_get(s5c->s5p->index, read_id, &read_index) < 0) {
		SLOW5_ERROR("Error getting index for read %s.", read_id);
		return SLOW5_ERR_NOTFOUND;
	}

    // queue transfer
    indexed_resp_t *resp_i = malloc(sizeof *resp_i);
    resp_i->resp = response_init();
    SLOW5_MALLOC_CHK(resp_i->resp);
    resp_i->index = transfer;
    
    if (
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L) ||
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, resp_callback) ||
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)resp_i->resp) ||
        curl_easy_setopt(curl, CURLOPT_PRIVATE, resp_i) ||
        byte_fetch_init(curl, s5c->url, read_index.offset + sizeof(slow5_rec_size_t), read_index.size - sizeof(slow5_rec_size_t)) ||
        curl_multi_add_handle(curl_multi, curl)
    ) {
        SLOW5_ERROR("Initializing transfer for read %s failed.", read_id);
        return SLOW5_ERR_OTH;
    }

    (*left)++;

    return EXIT_SUCCESS;
}

int s5curl_get_batch(
    slow5_curl_t *s5c,
    conn_stack_t *conns,
    long max_conns,
    uint64_t n_reads,
    char **read_ids,
    slow5_rec_t **records
) {
    CURLM *curl_multi = curl_multi_init();
    if (!curl_multi) {
        SLOW5_ERROR("Get multi read failed: %s.", curl_easy_strerror(CURLE_FAILED_INIT));
        return SLOW5_ERR_OTH;
    }

    CURLMsg *msg;
    size_t transfers = 0;
    int msgs_left = -1;
    int left = 0;
    int res;
    
    // init multi_stack
    res = curl_multi_setopt(curl_multi, CURLMOPT_MAXCONNECTS, max_conns);
    if (res != 0) {
        curl_multi_cleanup(curl_multi);
        SLOW5_ERROR("Setting connection limit failed: %s.", curl_easy_strerror(res));
        return SLOW5_ERR_OTH;
    }
    
    // queue initial transfers
    for (transfers = 0; transfers < conns->n_conns && transfers < n_reads; transfers++) {
        res = add_transfer(s5curl_conns_pop(conns), s5c, curl_multi, read_ids[transfers], transfers, &left);
        if (res != 0) {
            curl_multi_cleanup(curl_multi);
            SLOW5_ERROR("%s", "Queuing transfer failed.");
            return res;
        }
    }
    
    do {
        int still_alive = 1;
        curl_multi_perform(curl_multi, &still_alive);

        while((msg = curl_multi_info_read(curl_multi, &msgs_left))) {
            if (msg->msg == CURLMSG_DONE) {
                // get response
                indexed_resp_t *resp_i;
                CURL *e = msg->easy_handle;
                
                if (curl_easy_getinfo(e, CURLINFO_PRIVATE, &resp_i) != CURLE_OK) return -1;
                size_t index = resp_i->index;

                slow5_rec_t *record = NULL;

                long response_code;
                curl_easy_getinfo(e, CURLINFO_RESPONSE_CODE, &response_code);
                if (response_code != 206) {
                    SLOW5_ERROR("Fetching read %s failed with error code: %li.", read_ids[index], response_code);
                } else {
                    // decode
                    res = slow5_decode((void *)&resp_i->resp->data, &resp_i->resp->size, &record, s5c->s5p);
                    if (res < 0) SLOW5_ERROR("Error decoding read %s.\n", read_ids[index]);
                }

                records[index] = record;

                // cleanup
                response_cleanup(resp_i->resp);
                free(resp_i);
                
                // dequeue from multi_stack
                res = curl_multi_remove_handle(curl_multi, e);
                if (res != 0) {
                    curl_multi_cleanup(curl_multi);
                    SLOW5_ERROR("Removing connection handle failed: %s.", curl_easy_strerror(res));
                    return SLOW5_ERR_OTH;
                }
                
                // push connection back to stack
                left--;
                res = s5curl_conns_push(conns, e);
                if (res != 0) {
                    curl_multi_cleanup(curl_multi);
                    SLOW5_ERROR("%s\n", "Pushing to connection stack failed.");
                    return res;
                }
            } else {
                curl_multi_cleanup(curl_multi);
                SLOW5_ERROR("%s", "Error fetching bytes for read.");
                return SLOW5_ERR_OTH;
            }

            // queue transfers
            if (transfers < n_reads) {
                res = add_transfer(s5curl_conns_pop(conns), s5c, curl_multi, read_ids[transfers], transfers, &left);
                if (res != 0) {
                    curl_multi_cleanup(curl_multi);
                    SLOW5_ERROR("%s", "Queuing transfer failed.");
                    return res;
                }

                transfers++;
            }
        }

        if (left) {
            curl_multi_wait(curl_multi, NULL, 0, 1000, NULL);
        }
    } while(left);

    curl_multi_cleanup(curl_multi);

    return EXIT_SUCCESS;
}
