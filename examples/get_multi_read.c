// an example programme that uses slow5curl to fetch a list of records

#include <stdio.h>
#include <stdlib.h>
#include <slow5curl/s5curl.h>

#define URL "https://github.com/BonsonW/slow5curl/raw/main/examples/data/reads_10.blow5"
#define READ_LIST_PATH "examples/data/reads_10.txt"
#define N_THREADS 3
#define N_READS 10
#define BATCH_CAPACITY 100

#define TO_PICOAMPS(RAW_VAL, DIGITISATION, OFFSET, RANGE) (((RAW_VAL)+(OFFSET))*((RANGE)/(DIGITISATION)))

int main(){
    char **read_ids = (char **)malloc(N_READS * sizeof *read_ids);
    if (!read_ids) {
        fprintf(stderr, "Error allocating memory for read ids.\n");
        exit(EXIT_FAILURE);
    }

    // read the list of read IDs from file
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t n_chars;
    size_t cur_read = 0;

    fp = fopen(READ_LIST_PATH, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error opening read ids.\n");
        exit(EXIT_FAILURE);
    }

    while ((n_chars = getline(&line, &len, fp)) != -1 && cur_read < N_READS) {
        line[n_chars-1] = '\0';
        read_ids[cur_read] = strdup(line);
        cur_read++;
    }

    free(line);

    fclose(fp);

    // global curl init
    int ret = s5curl_global_init();
    if (ret < 0) {
        fprintf(stderr, "Error initializing global resources.\n");
        return EXIT_FAILURE;
    }

    s5curl_t *s5c = s5curl_open(URL);
    if (!s5c) {
        fprintf(stderr, "Error opening slow5 file %s.\n", URL);
        return EXIT_FAILURE;
    }

    // initialize a multithread struct
    s5curl_mt_t *s5c_mt = s5curl_init_mt(N_THREADS, s5c);
    if (!s5c_mt) {
        fprintf(stderr, "Error initializing multithread stcut.\n");
        return EXIT_FAILURE;
    }

    slow5_batch_t *db = slow5_init_batch(BATCH_CAPACITY);
    if (!db) {
        fprintf(stderr, "Error initializing read batch.\n");
        return EXIT_FAILURE;
    }

    // load the SLOW5 index
    ret = s5curl_idx_load(s5c);
    if (ret < 0) {
        fprintf(stderr, "Error fetching index for %s\n", URL);
        exit(EXIT_FAILURE);
    }

    // fetch the records from read id list
    ret = s5curl_get_batch(s5c_mt, db, read_ids, N_READS);
    if (ret != N_READS) {
        fprintf(stderr, "Error in when fetching the read.\n");
        exit(EXIT_FAILURE);
    } else {
        for (int i = 0; i < N_READS; ++i) {
            slow5_rec_t *rec = db->slow5_rec[i];
            printf("%s\t", rec->read_id);
            uint64_t len_raw_signal = rec->len_raw_signal;
            for(uint64_t i = 0; i < len_raw_signal; i++) { // iterate through the raw signal and print in picoamperes
                double pA = TO_PICOAMPS(rec->raw_signal[i], rec->digitisation, rec->offset, rec->range);
                printf("%f ",pA);
            }
            printf("\n");
        }
    }

    //..... fetch any other reads using s5curl_get_batch (as above)

    // free the list of read ids
    for (int i = 0; i < N_READS; ++i) {
        free(read_ids[i]);
    }
    free(read_ids);

    // free the SLOW5 index
    s5curl_idx_unload(s5c);

    // close the SLOW5 file
    s5curl_close(s5c);

    // free batch struct
    slow5_free_batch(db);

    // close multi-thread struct
    s5curl_free_mt(s5c_mt);

    // global curl cleanup
    s5curl_global_cleanup();

    return 0;
}
