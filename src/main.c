/* @file main.c
**
******************************************************************************/

#include <stdio.h>
#include <slow5/slow5.h>
#include <curl/curl.h>

#include "s5wget/s5wget.h"

int main(int argc, char* argv[]) {
    curl_global_init(CURL_GLOBAL_ALL);
    
    const char *url = "https://gtgseq.s3.amazonaws.com/ont-r10-dna/NA12878/raw/PGXXHX230142_reads.blow5";
    // const char *read_ids_path = "/home/hasindu/scratch/na12878_prom_lsk114/chr22/chr22reads.list";
    const char *idx_path = "/home/bonson/PGXXHX230142_reads.blow5.idx";
    
    // slow5 setup
    slow5_file_t *sp = s5wget_open(url, SLOW5_FORMAT_BINARY);
    if (sp == NULL) {
       fprintf(stderr, "Error in opening file\n");
       return -1;
    }

    int ret = 0;
    ret = slow5_idx_load_from_path(sp, idx_path);
    if (ret < 0) {
        fprintf(stderr, "Error in loading index\n");
        return -1;
    }

    slow5_idx_t *s_idx = sp->index;
    
    // todo: parse read list
    // todo: fetch reads
    
    slow5_idx_unload(sp);
    slow5_close(sp);
    curl_global_cleanup();

    return 0;
}
