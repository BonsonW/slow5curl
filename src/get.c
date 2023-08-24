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
    struct slow5_rec_idx read_index;
	if (slow5_idx_get(s5c->s5p->index, read_id, &read_index) < 0) {
		SLOW5_ERROR("Error getting index for read %s.", read_id);
        slow5_errno = SLOW5_ERR_NOTFOUND;
		return slow5_errno;
	}

	response_t *resp = response_init();

	int res = resp_byte_fetch_init(
        curl,
	    resp,
		s5c->url, 
		read_index.offset + sizeof(slow5_rec_size_t),
		read_index.size - sizeof(slow5_rec_size_t)
	);
    if (res < 0) {
		SLOW5_ERROR("Initializing fetch for read %s failed: %s.", read_id, curl_easy_strerror(res));
        slow5_errno = SLOW5_ERR_OTH;
		return slow5_errno;
	}

    curl_easy_perform(curl);
	if (res < 0) {
		SLOW5_ERROR("Fetch bytes for read %s failed: %s.", read_id, curl_easy_strerror(res));
        slow5_errno = SLOW5_ERR_OTH;
		return slow5_errno;
	}

    res = slow5_decode((void *)&resp->data, &resp->size, record, s5c->s5p);
    response_cleanup(resp);

    if (res < 0) {
		SLOW5_ERROR("Decoding read %s failed.", read_id);
        slow5_errno = SLOW5_ERR_OTH;
		return slow5_errno;
	}

    return EXIT_SUCCESS;
}

static int add_transfer(
    CURL *curl,
    slow5_curl_t *s5c,
    CURLM *cm,
    char *read_id,
    uint32_t transfer,
    int *left
) {
    if (!curl) {
        SLOW5_ERROR("Transfer for read %s failed: %s.", read_id, curl_easy_strerror(CURLE_FAILED_INIT));
        slow5_errno = SLOW5_ERR_OTH;
        return slow5_errno;
    }

    struct slow5_rec_idx read_index;
	if (slow5_idx_get(s5c->s5p->index, read_id, &read_index) < 0) {
		SLOW5_ERROR("Error getting index for read %s.", read_id);
        slow5_errno = SLOW5_ERR_NOTFOUND;
		return slow5_errno;
	}

    response_t *resp = (response_t *)malloc(sizeof(response_t));
    resp->data = NULL;
    resp->size = 0;
    resp->id = transfer;

    if (
        resp_byte_fetch_init(curl, resp, s5c->url, read_index.offset + sizeof(slow5_rec_size_t), read_index.size - sizeof(slow5_rec_size_t)) ||
        curl_easy_setopt(curl, CURLOPT_PRIVATE, resp) ||
        curl_multi_add_handle(cm, curl)
    ) {
        SLOW5_ERROR("Initializing transfer for read %s failed.", read_id);
        slow5_errno = SLOW5_ERR_OTH;
        return slow5_errno;
    }

    (*left)++;

    return EXIT_SUCCESS;
}

int s5curl_get_batch(
    slow5_curl_t *s5c,
    conn_stack_t *conns,
    CURLM *cm,
    uint64_t n_reads,
    char **read_ids,
    slow5_rec_t **records
) {
    CURLMsg *msg;
    size_t transfers = 0;
    int msgs_left = -1;
    int left = 0;
    int res;
    
    res = curl_multi_setopt(cm, CURLMOPT_MAXCONNECTS, (long)conns->n_conns);
    if (res < 0) {
        SLOW5_ERROR("Setting connection limit failed: %s.", curl_easy_strerror(res));
        slow5_errno = SLOW5_ERR_OTH;
        return slow5_errno;
    }
 
    for (transfers = 0; transfers < conns->n_conns && transfers < n_reads; transfers++) {
        res = add_transfer(s5curl_conns_pop(conns), s5c, cm, read_ids[transfers], transfers, &left);
        if (res < 0) {
            SLOW5_ERROR("%s", "Queuing transfer failed.");
            return res;
        }
    }

    do {
        int still_alive = 1;
        curl_multi_perform(cm, &still_alive);

        while((msg = curl_multi_info_read(cm, &msgs_left))) {
            if (msg->msg == CURLMSG_DONE) {
                response_t *resp;
                CURL *e = msg->easy_handle;
                
                if (curl_easy_getinfo(msg->easy_handle, CURLINFO_PRIVATE, &resp) != CURLE_OK) return -1;
                size_t index = resp->id;

                slow5_rec_t *record = NULL;

                res = slow5_decode((void *)&resp->data, &resp->size, &record, s5c->s5p);
                if (res < 0) SLOW5_ERROR("Error decoding read %s.\n", read_ids[index]);

                records[index] = record;

                response_cleanup(resp);
                
                res = curl_multi_remove_handle(cm, e);
                if (res < 0) { 
                    SLOW5_ERROR("Removing connection handle failed: %s.", curl_easy_strerror(res));
                    slow5_errno = SLOW5_ERR_OTH;
                    return slow5_errno;
                }
                
                left--;
                res = s5curl_conns_push(conns, e);
                if (res < 0) { 
                    SLOW5_ERROR("%s\n", "Pushing to connection stack failed.");
                    return res;
                }
            } else {
                SLOW5_ERROR("%s", "Error fetching bytes for read.");
                slow5_errno = SLOW5_ERR_OTH;
                return slow5_errno;
            }

            if (transfers < n_reads) {
                res = add_transfer(s5curl_conns_pop(conns), s5c, cm, read_ids[transfers], transfers, &left);
                if (res < 0) {
                    SLOW5_ERROR("%s", "Queuing transfer failed.");
                    return res;
                }

                transfers++;
            }
        }

        if (left) {
            curl_multi_wait(cm, NULL, 0, 1000, NULL);
        }
    } while(left);

    return EXIT_SUCCESS;
}
