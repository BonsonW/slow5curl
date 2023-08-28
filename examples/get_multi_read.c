// an example programme that uses slow5curl to fetch a list of records

#include <stdio.h>
#include <stdlib.h>
#include <slow5curl/s5curl.h>
#include <curl/curl.h>

#define URL "https://gtgseq.s3.amazonaws.com/ont-r10-dna/NA12878/raw/PGXXHX230142_reads.blow5"
#define MAX_CONNECTS 100
#define N_READS 10

#define TO_PICOAMPS(RAW_VAL, DIGITISATION, OFFSET, RANGE) (((RAW_VAL)+(OFFSET))*((RANGE)/(DIGITISATION)))

int main(){
    const char *read_ids_path = "reads_10.txt";
    char **read_ids = malloc(N_READS * sizeof *read_ids);

    // read from file
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t n_chars;
    size_t cur_read = 0;

    fp = fopen(read_ids_path, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error opening read ids.\n");
        exit(EXIT_FAILURE);
    }

    while ((n_chars = getline(&line, &len, fp)) != -1 && cur_read < n_reads) {
        line[n_chars-1] = '\0';
        read_ids[cur_read] = strdup(line);
        cur_read++;
    }

    fclose(fp);

    // global curl init
    curl_global_init(CURL_GLOBAL_ALL);

    // initialize a connection stack
    conn_stack_t *conns = s5curl_open_conns(MAX_CONNECTS);
    if (!conns) {
        fprintf(stderr, "Error opening connections.\n");
        return EXIT_FAILURE;
    }

    slow5_curl_t *s5c = s5curl_open(URL);
    if (!s5c) {
        fprintf(stderr, "Error fetching slow5 file %s.\n", URL);
        return EXIT_FAILURE;
    }

    slow5_rec_t **records = calloc(db->n_batch, sizeof *records); // slow5 records to be read
    int ret = 0; // for return value

    // load the SLOW5 index
    ret = s5curl_idx_load(s5c);
    if (ret_idx < 0) {
        ERROR("Error fetching index for %s\n", URL);
        exit(EXIT_FAILURE);
    }

    // fetch the records from read id list
    ret = s5curl_get_batch(*s5c, conns, MAX_CONNECTS, N_READS, read_ids, records);
    if (ret < 0) {
        fprintf(stderr,"Error in when fetching the read.\n");
    } else {
        printf("%s\t",rec->read_id);
        uint64_t len_raw_signal = rec->len_raw_signal;
        for(uint64_t i = 0; i < len_raw_signal; i++) { // iterate through the raw signal and print in picoamperes
            double pA = TO_PICOAMPS(rec->raw_signal[i], rec->digitisation, rec->offset, rec->range);
            printf("%f ",pA);
        }
        printf("\n");
    }

    //..... fetch any other reads using slow5_get (as above)

    // free records and read ids
    for (int i = 0; i < N_READS; ++i) {
        slow5_rec_free(records[i]);
        free(read_ids[i]);
    }
    free(records);
    free(read_ids);

    // free the SLOW5 index
    s5curl_idx_unload(s5c);

    // close the SLOW5 file
    s5curl_close(s5c);

    // close connection stack
    s5curl_close_conns(conns);

    // global curl cleanup
    curl_global_cleanup();

    return 0;
}