/* @file main.c
**
******************************************************************************/

#include <stdio.h>
#include <slow5/slow5.h>
#include <curl/curl.h>

#define TO_PICOAMPS(RAW_VAL,DIGITISATION,OFFSET,RANGE) (((RAW_VAL)+(OFFSET))*((RANGE)/(DIGITISATION)))

int main(int argc, char* argv[]) {
    curl_global_init(CURL_GLOBAL_ALL);
    
    const char *slow5_path = "/home/hasindu/scratch/na12878_prom_lsk114/PGXXHX230142_reads.blow5";
    const char *read_ids_path = "/home/hasindu/scratch/na12878_prom_lsk114/chr22/chr22reads.list";

    // slow5 setup
    slow5_file_t *sp = slow5_open(slow5_path, "r");
    if (sp == NULL) {
       fprintf(stderr, "Error in opening file\n");
       return -1;
    }

    int ret = 0;
    ret = slow5_idx_load(sp);
    if (ret < 0) {
        fprintf(stderr, "Error in loading index\n");
        return -1;
    }

    slow5_idx_t *s_idx = sp->index;
    
    // read here
    
    slow5_idx_unload(sp);
    slow5_close(sp);
    
    curl_global_cleanup();

    return 0;
}
