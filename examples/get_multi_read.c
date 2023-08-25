// an example programme that uses slow5curl to fetch a list of records

#include <stdio.h>
#include <stdlib.h>
#include <slow5/slow5.h>
#include <slow5curl/s5curl.h>
#include <curl/curl.h>

#define URL "https://gtgseq.s3.amazonaws.com/ont-r10-dna/NA12878/raw/PGXXHX230142_reads.blow5"

#define TO_PICOAMPS(RAW_VAL, DIGITISATION, OFFSET, RANGE) (((RAW_VAL)+(OFFSET))*((RANGE)/(DIGITISATION)))

int main(){
    const char *read_ids_path = "/home/hasindu/scratch/na12878_prom_lsk114/chr22/chr22reads.list";
    char **read_ids = malloc(n_reads * sizeof *read_ids);

    // global curl init
    curl_global_init(CURL_GLOBAL_ALL);

    // initialize a multi stack
    CURL *curl_multi = curl_multi_init();
    if (!curl_multi) {
        fprintf(stderr, "Error initializing multi stack.\n");
        return EXIT_FAILURE;
    }

    // initialize a connection stack of size 100
    conn_stack_t *conns = s5curl_open_conns(100);
    if (!conns) {
        fprintf(stderr, "Error opening connections.\n");
        return EXIT_FAILURE;
    }

    slow5_curl_t *s5c = s5curl_open(URL);
    if (!s5c) {
        fprintf(stderr, "Error fetching slow5 file %s.\n", URL);
        return EXIT_FAILURE;
    }

    slow5_rec_t *rec = NULL; // slow5 record to be read
    int ret = 0; // for return value

    // load the SLOW5 index
    ret = s5curl_idx_load(s5c);
    if (ret_idx < 0) {
        ERROR("Error fetching index for %s\n", URL);
        exit(EXIT_FAILURE);
    }

    // fetch the read with read_id "0032812b-1ea5-46f1-a844-5bcc3bf3c21f"
    ret = s5curl_get(s5c, curl, "0032812b-1ea5-46f1-a844-5bcc3bf3c21f", &rec);
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

    //..... fetch any other read using slow5_get (as above)

    // free the SLOW5 record
    slow5_rec_free(rec);

    // free the SLOW5 index
    s5curl_idx_unload(slow5curl);

    // close the SLOW5 file
    s5curl_close(slow5curl);

    // close multi stack
    curl_multi_cleanup(curl_multi);

    // close connection stack
    s5curl_close_conns(conns);

    // global curl cleanup
    curl_global_cleanup();

    return 0;
}