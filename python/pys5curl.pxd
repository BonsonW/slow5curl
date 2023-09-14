#cython: language_level=3
from libc.stdio cimport *
from libc.stdint cimport *
from libc.stdlib cimport *

cdef extern from "pys5curl.h":

    # structs

    ctypedef struct slow5_curl_t:
        char *url;
        slow5_file_t *s5p;

    ctypedef struct conn_stack_t:
        pass

    ctypedef struct s5curl_multi_t:
        conn_stack_t *conns;
    

    # methods

    slow5_curl_t *s5curl_open(const char *url);

    void s5curl_close(slow5_curl_t *s5c);

    int s5curl_idx_load(slow5_curl_t *s5c);

    int s5curl_idx_load_with(slow5_curl_t *s5c, const char *path);

    void s5curl_idx_unload(slow5_curl_t *s5c);

    int s5curl_get(slow5_curl_t *s5c, CURL *curl, const char *read_id, slow5_rec_t **record);

    int s5curl_get_batch(slow5_curl_t *s5c, s5curl_multi_t *s5curl_multi, long max_conns, uint64_t n_reads, char **read_ids, slow5_rec_t **records);

    s5curl_multi_t *s5curl_multi_open(int32_t n_conns);

    void s5curl_multi_close(s5curl_multi_t *s5curl_multi);
