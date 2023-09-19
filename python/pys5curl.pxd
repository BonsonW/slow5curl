#cython: language_level=3
from libc.stdio cimport *
from libc.stdint cimport *
from libc.stdlib cimport *

cdef extern from "pys5curl.h":

    # structs

    ctypedef struct s5curl_t:
        char *url;
        slow5_file_t *s5p;

    ctypedef struct s5curl_conn_stack_t:
        pass

    ctypedef struct s5curl_mt_t:
        s5curl_conn_stack_t *conns;
    

    # methods

    s5curl_t *s5curl_open(const char *url);

    void s5curl_close(s5curl_t *s5c);

    int s5curl_idx_load(s5curl_t *s5c);

    int s5curl_idx_load_with(s5curl_t *s5c, const char *path);

    void s5curl_idx_unload(s5curl_t *s5c);

    int s5curl_get(const char *read_id, slow5_rec_t **record, CURL *curl, s5curl_t *s5c);

    int s5curl_get_batch(s5curl_mt_t *core, slow5_batch_t *db, char **rid, int num_rid);

    s5curl_mt_t *s5curl_init_mt(int num_thread, s5curl_t *s5c);

    void s5curl_free_mt(s5curl_mt_t *s5curl_mt_t);
