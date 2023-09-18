/*
MIT License

Copyright (c) 2021 Hasindu Gamaarachchi
Copyright (c) 2023 Bonson Wong

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <errno.h>
#include <pthread.h>
#include <slow5curl/s5curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SLOW5_WORK_STEAL 1
#define SLOW5_STEAL_THRESH 1

extern enum slow5_log_level_opt  slow5_log_level;
extern enum slow5_exit_condition_opt slow5_exit_condition;

typedef struct {
	s5curl_mt_t *core;
	s5curl_batch_t *db;
	int32_t starti;
	int32_t endi;
	void (*func)(s5curl_mt_t *, s5curl_batch_t *, int32_t, int32_t);
	int32_t thread_index;
	int32_t curl_index;
#ifdef WORK_STEAL
    void *all_pthread_args;
#endif
} pthread_arg_t;

s5curl_batch_t *s5curl_init_batch(
	int batch_capacity
) {
	s5curl_batch_t *db  = calloc(1, sizeof *db);
    SLOW5_MALLOC_CHK_EXIT(db);

    db->capacity_rec = batch_capacity;
    db->n_rec = 0;
    db->n_err = 0;

    db->rid = malloc(batch_capacity * sizeof(char*));
	SLOW5_MALLOC_CHK_EXIT(db->rid);
	
    db->raw_rec = malloc(batch_capacity * sizeof(raw_record_t));
    SLOW5_MALLOC_CHK_EXIT(db->raw_rec);

    return db;
}

void s5curl_free_batch(
	s5curl_batch_t *db
) {
    free(db->raw_rec);
    
    free(db->rid);
    free(db);
}

s5curl_mt_t *s5curl_init_mt(
	int num_thread,
	s5curl_t *s5c
) {
	s5curl_mt_t *core = (s5curl_mt_t *)malloc(sizeof(s5curl_mt_t));
	SLOW5_MALLOC_CHK_EXIT(core);
	
	core->curl = calloc(num_thread, sizeof *core->curl);
    for (size_t i = 0; i < num_thread; ++i) {
        core->curl[i] = curl_easy_init();
        SLOW5_MALLOC_CHK_EXIT(core->curl[i]);
    }

	core->s5c = s5c;
	core->num_thread = num_thread;

	return core;
}

void s5curl_free_mt(
	s5curl_mt_t *core
) {
	for (size_t i = 0; i < core->num_thread; ++i) {
        curl_easy_cleanup(core->curl[i]);
    }
	free(core->curl);
	free(core);
}

static inline int32_t steal_work(
	pthread_arg_t *all_args,
	int32_t n_threads
) {
	int32_t i, c_i = -1;
	int32_t k;
	for (i = 0; i < n_threads; ++i) {
		pthread_arg_t args = all_args[i];
		if (args.endi - args.starti > SLOW5_STEAL_THRESH) {
			c_i = i;
			break;
		}
	}
	if (c_i < 0) {
		return -1;
	}
	k = __sync_fetch_and_add(&(all_args[c_i].starti), 1);
	return k >= all_args[c_i].endi ? -1 : k;
}

static void *pthread_single(
	void *voidargs
) {
	int32_t i;
	pthread_arg_t *args = (pthread_arg_t *)voidargs;
	s5curl_batch_t *db = args->db;
	s5curl_mt_t *core = args->core;

#ifndef WORK_STEAL
	for (i = args->starti; i < args->endi; i++) {
		args->func(core, db, i, args->curl_index);
	}
#else
	pthread_arg_t *all_args = (pthread_arg_t *)(args->all_pthread_args);
	// adapted from kthread.c in minimap2
	for (;;) {
		i = __sync_fetch_and_add(&args->starti, 1);
		if (i >= args->endi) {
			break;
		}
		args->func(core, db, i, args->curl_index);
	}
	while ((i = steal_work(all_args, core->num_thread)) >= 0) {
		args->func(core, db, i, args->curl_index);
	}
#endif

  // fprintf(stderr,"Thread %d done\n",(myargs->position)/THREADS);
  pthread_exit(0);
}

static void pthread_db(
	s5curl_mt_t *core,
	s5curl_batch_t *db,
	void (*func)(s5curl_mt_t *, s5curl_batch_t *, int32_t, int32_t)
) {
	// create threads
	pthread_t tids[core->num_thread];
	pthread_arg_t pt_args[core->num_thread];
	int32_t t, ret;
	int32_t i = 0;
	int32_t num_thread = core->num_thread;
	int32_t step = (db->n_rec + num_thread - 1) / num_thread;
	// todo : check for higher num of threads than the data
	// current works but many threads are created despite

	// set the data structures
	for (t = 0; t < num_thread; t++) {
		pt_args[t].core = core;
		pt_args[t].db = db;
		pt_args[t].starti = i;
		i += step;
		
		if (i > db->n_rec) {
			pt_args[t].endi = db->n_rec;
		} else {
			pt_args[t].endi = i;
		}
		
		pt_args[t].func = func;
		pt_args[t].curl_index = t;
#ifdef WORK_STEAL
        pt_args[t].all_pthread_args = (void *)pt_args;
#endif
        // fprintf(stderr,"t%d : %d-%d\n",t,pt_args[t].starti,pt_args[t].endi);
	}

    // create threads
	for (t = 0; t < core->num_thread; t++) {
		ret = pthread_create(&tids[t], NULL, pthread_single, (void *)(&pt_args[t]));
		if (ret < 0) {
            SLOW5_ERROR("Error creating thread %d\n",t);
            exit(EXIT_FAILURE);
        }
	}

	// pthread joining
	for (t = 0; t < core->num_thread; t++) {
		int ret = pthread_join(tids[t], NULL);
		if (ret < 0) {
            SLOW5_ERROR("Error creating thread %d\n",t);
            exit(EXIT_FAILURE);
        }
	}
}

/* process all reads in the given batch db */
static void work_db(
	s5curl_mt_t *core,
	s5curl_batch_t *db,
	void (*func)(s5curl_mt_t *, s5curl_batch_t *, int32_t, int32_t)
) {

	if (core->num_thread == 1) {
		int32_t i = 0;
		for (i = 0; i < db->n_rec; i++) {
			func(core, db, i, 0);
		}
	}

	else {
		pthread_db(core, db, func);
	}
}

static void work_per_single_read_get(
	s5curl_mt_t *core,
	s5curl_batch_t *db,
	int32_t i,
	int32_t tid
) {
	slow5_file_t *s5p = core->s5c->s5p;
	
	char *read_id = db->rid[i];
	CURL *curl = core->curl[tid];
	
	slow5_rec_t *record = NULL;

	int len = s5curl_get(core->s5c, curl, read_id, &record);

	if (record == NULL || len != 0) {
		++db->n_err;
		db->raw_rec[i].buffer = NULL;
		db->raw_rec[i].len = -1;
	} else {
		size_t record_size;
		slow5_press_method_t press_out = {s5p->compress->record_press->method, s5p->compress->signal_press->method};
        struct slow5_press* compress = slow5_press_init(press_out);
        if (!compress) {
            SLOW5_ERROR("Could not initialize the slow5 compression method %s.","");
            exit(EXIT_FAILURE);
        }
        db->raw_rec[i].buffer = slow5_rec_to_mem(record, s5p->header->aux_meta, s5p->format, compress, &record_size);
        db->raw_rec[i].len = record_size;
        slow5_press_free(compress);
        slow5_rec_free(record);
        
        // todo peform callback here
	}
	free(read_id);
}

int s5curl_get_batch(
	s5curl_mt_t *core,
	s5curl_batch_t *db,
	char **rid,
	int num_rid
) {
	// db->rid = rid;
	db->n_rec = num_rid;
	work_db(core, db, work_per_single_read_get);
	SLOW5_LOG_DEBUG("loaded and parsed %d recs\n", num_rid);    
	return num_rid;
}
