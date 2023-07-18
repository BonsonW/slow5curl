#include "../slow5lib/src/slow5_idx.h"
#include "fetch.h"
#include "s5curl.h"

extern enum slow5_log_level_opt  slow5_log_level;

int s5curl_read(
    slow5_curl_t *s5c,
    const char *read_id,
    slow5_rec_t *read
) {
    slow5_idx_t *s_idx = s5c->s5p->index;
    slow5_file_t *s5p = s5c->s5p;

    struct slow5_rec_idx read_index;
	if (slow5_idx_get(s_idx, read_id, &read_index) < 0) {
		SLOW5_ERROR("Error getting index for read %s.", read_id);
		return -1;
	}
	
	// exclude meta data before copying record
	size_t bytes = read_index.size - sizeof(slow5_rec_size_t);
	response_t resp = {0};

	int ret = fetch_bytes_into_resp(
	    &resp,
		s5c->url, 
		read_index.offset,
		read_index.size
	);
	if (ret < 0) {
		SLOW5_ERROR("Error fetching bytes for read %s.", read_id);
		return -1;
	}
	
	char *read_start = resp.data + sizeof(slow5_rec_size_t);

	if (slow_decode((void *)&read_start, &bytes, &read, s5p) < 0) { 
		SLOW5_ERROR("Error decoding read %s\n", read_id);
		return -1;
	}

    response_free(&resp);

    return 0;
}

// static void add_transfer(CURLM *cm, int i, int *left)
// {
//     CURL *eh = curl_easy_init();
//     curl_easy_setopt(eh, CURLOPT_WRITEFUNCTION, write_cb);
//     curl_easy_setopt(eh, CURLOPT_URL, urls[i]);
//     curl_easy_setopt(eh, CURLOPT_PRIVATE, urls[i]);
//     curl_multi_add_handle(cm, eh);
//     (*left)++;
// }

// int s5curl_read_list(
//     slow5_curl_t *s5c,
//     uint64_t max_connects,
//     uint64_t n_reads,
//     const char **read_ids,
//     slow5_rec_t **reads
// ) {
//     CURLM *cm;
//     CURLMsg *msg;
//     unsigned int transfers = 0;
//     int msgs_left = -1;
//     int left = 0;

//     cm = curl_multi_init();

//     curl_multi_setopt(cm, CURLMOPT_MAXCONNECTS, max_connects);
 
//     for (transfers = 0; transfers < max_connects && transfers < n_reads; transfers++) {
//         add_transfer(cm, transfers, &left);
//     }
    
//     do {
//         int still_alive = 1;
//         curl_multi_perform(cm, &still_alive);

//         while((msg = curl_multi_info_read(cm, &msgs_left))) {
//             if (msg->msg == CURLMSG_DONE) {
//                 char *url;
//                 CURL *e = msg->easy_handle;
                
//                 curl_easy_getinfo(msg->easy_handle, CURLINFO_PRIVATE, &url);
//                 fprintf(stderr, "R: %d - %s <%s>\n",
//                         msg->data.result, curl_easy_strerror(msg->data.result), url);

//                 curl_multi_remove_handle(cm, e);
//                 curl_easy_cleanup(e);
//                 left--;
//             } else {
//                 // error
//             }

//             if (transfers < n_reads) {
//                 add_transfer(cm, transfers++, &left);
//             }
//         }

//         if (left) {
//             curl_multi_wait(cm, NULL, 0, 1000, NULL);
//         }
//     } while(left);

//     curl_multi_cleanup(cm);

//     return EXIT_SUCCESS;
// }
