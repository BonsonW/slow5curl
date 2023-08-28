#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <slow5/slow5.h>
#include <slow5curl/s5curl.h>
#include "error.h"

#ifdef __cplusplus
extern "C" {
#endif

/* core data structure that has information that are global to all the threads */
typedef struct {
    int32_t num_thread;
    slow5_curl_t *s5c;
    enum slow5_fmt format_out;
    slow5_press_method_t press_method;
    // for view
    bool benchmark;
    // for merge
    int lossy;
    int slow5_file_index;
    slow5_aux_meta_t *aux_meta;
    // skim
    void *param;
} core_t;

typedef struct{
    int len;
    void *buffer;
} raw_record_t;

/* data structure for a batch of reads */
typedef struct {
    int64_t n_batch;                // number of records in this batch
    int64_t n_err;                  // number of errors in this batch
    raw_record_t *read_record;      // the list of read records (output) //change to whatever the data type
    // for get
    char **read_id;                 // the list of read ids (input)
    conn_stack_t *conns;            // stack of reusable connections (curl easy handles)
    // for view
    char **mem_records;             // list of slow5_get_next_mem() records
    size_t *mem_bytes;              // lengths of slow5_get_next_mem() records
    // for split
    uint32_t *read_group_vector;
} db_t;

#ifdef __cplusplus
}
#endif

#endif