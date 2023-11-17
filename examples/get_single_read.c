// an example programme that uses slow5curl to fetch a single record

#include <stdio.h>
#include <stdlib.h>
#include <slow5curl/s5curl.h>

#define URL "https://github.com/BonsonW/slow5curl/raw/main/examples/data/reads_10.blow5"

#define TO_PICOAMPS(RAW_VAL, DIGITISATION, OFFSET, RANGE) (((RAW_VAL)+(OFFSET))*((RANGE)/(DIGITISATION)))

int main(){
    // global curl init
    int ret = s5curl_global_init();
    if (ret < 0) {
        fprintf(stderr, "Error initializing global resources.\n");
        return EXIT_FAILURE;
    }

    // initialize a connection
    // to be re-used across s5curl_get(s) in a single thread
    S5CURLCONN *curl = s5curl_conn_init();
    if (!curl) {
        fprintf(stderr, "Error initializing connection.\n");
        return EXIT_FAILURE;
    }

    s5curl_t *s5c = s5curl_open(URL);
    if (!s5c) {
        fprintf(stderr, "Error opening slow5 file %s.\n", URL);
        return EXIT_FAILURE;
    }

    slow5_rec_t *rec = NULL; // slow5 record to be read

    // load the SLOW5 index
    ret = s5curl_idx_load(s5c);
    if (ret < 0) {
        fprintf(stderr, "Error fetching index for %s\n", URL);
        exit(EXIT_FAILURE);
    }

    // fetch the read with read_id "000286ab-1f80-40e3-a778-8d89e4e52940"
    ret = s5curl_get("000286ab-1f80-40e3-a778-8d89e4e52940", &rec, curl, s5c);
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

    //..... fetch any other read using s5curl_get (as above)

    // free the SLOW5 record
    slow5_rec_free(rec);

    // free the SLOW5 index
    s5curl_idx_unload(s5c);

    // close the SLOW5 file
    s5curl_close(s5c);

    // close connection
    s5curl_conn_cleanup(curl);

    // global curl cleanup
    s5curl_global_cleanup();

    return 0;
}