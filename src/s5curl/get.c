#include <stdint.h>
#include <curl/curl.h>

#include "../slow5lib/src/slow5_idx.h"
#include "fetch.h"
#include <slow5curl/s5curl.h>

extern enum slow5_log_level_opt  slow5_log_level;

int s5curl_get(
    slow5_curl_t *s5c,
    CURL *curl, 
    char *read_id,
    slow5_rec_t *read
) {
    slow5_idx_t *s_idx = s5c->s5p->index;
    slow5_file_t *s5p = s5c->s5p;

    struct slow5_rec_idx read_index;
	if (slow5_idx_get(s_idx, read_id, &read_index) != 0) {
		SLOW5_ERROR("Error getting index for read %s.", read_id);
        slow5_errno = SLOW5_ERR_NOTFOUND;
		return slow5_errno;
	}
	
	// exclude meta data before copying record
	response_t resp = {0};

	CURLcode res = fetch_bytes_into_resp(
        curl,
	    &resp,
		s5c->url, 
		read_index.offset + sizeof(slow5_rec_size_t),
		read_index.size
	);

	if (res != 0) {
		SLOW5_ERROR("Fetch bytes for read %s failed: %s.", read_id, curl_easy_strerror(res));
        slow5_errno = SLOW5_ERR_OTH;
		return slow5_errno;
	}

    int decoded = slow5_decode((void *)&resp.data, &resp.size, &read, s5p);
    response_free(&resp);
    return decoded;
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

    slow5_idx_t *s_idx = s5c->s5p->index;

    struct slow5_rec_idx read_index;
	if (slow5_idx_get(s_idx, read_id, &read_index) != 0) {
		SLOW5_ERROR("Error getting index for read %s.", read_id);
        slow5_errno = SLOW5_ERR_NOTFOUND;
		return slow5_errno;
	}

    response_t *resp = (response_t *)malloc(sizeof(response_t));
    resp->data = NULL;
    resp->size = 0;
    resp->id = transfer;

    CURLcode res = queue_fetch_bytes_into_resp(
        curl,
        resp,
        s5c->url,
        read_index.offset + sizeof(slow5_rec_size_t),
        read_index.size - sizeof(slow5_rec_size_t),
        cm
    );

    if (res != 0) {
        SLOW5_ERROR("Transfer for read %s failed: %s.", read_id, curl_easy_strerror(res));
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
    slow5_rec_t **reads
) {
    CURLMsg *msg;
    uint32_t transfers = 0;
    int msgs_left = -1;
    int left = 0;

    curl_multi_setopt(cm, CURLMOPT_MAXCONNECTS, (long)conns->n_conns);
 
    for (transfers = 0; transfers < conns->n_conns && transfers < n_reads; transfers++) {
        int res = add_transfer(s5curl_conns_pop(conns), s5c, cm, read_ids[transfers], transfers, &left);
        if (res != 0) return res;
    }
    
    do {
        int still_alive = 1;
        curl_multi_perform(cm, &still_alive);

        while((msg = curl_multi_info_read(cm, &msgs_left))) {
            if (msg->msg == CURLMSG_DONE) {
                response_t *resp;
                CURL *e = msg->easy_handle;
                
                if (curl_easy_getinfo(msg->easy_handle, CURLINFO_PRIVATE, &resp) != CURLE_OK) return -1;
                uint32_t index = resp->id;

                slow5_rec_t *read = NULL;

                int res = slow5_decode((void *)&resp->data, &resp->size, &read, s5c->s5p);

                if (res != 0) { 
                    SLOW5_ERROR("Error decoding read %s.\n", read_ids[index]);
                    return res;
                }
                reads[index] = read;

                response_free(resp);
                free(resp);
                
                curl_multi_remove_handle(cm, e);
                left--;
                s5curl_conns_push(conns, e);
            } else {
                SLOW5_ERROR("%s", "Error fetching bytes for read.");
                slow5_errno = SLOW5_ERR_OTH;
                return slow5_errno;
            }

            if (transfers < n_reads) {
                int res = add_transfer(s5curl_conns_pop(conns), s5c, cm, read_ids[transfers], transfers, &left);
                if (res != 0) return res;

                transfers++;
            }
        }

        if (left) {
            curl_multi_wait(cm, NULL, 0, 1000, NULL);
        }
    } while(left);

    return EXIT_SUCCESS;
}
