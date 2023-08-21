/* @file main.c
**
******************************************************************************/

#include <stdio.h>
#include <slow5/slow5.h>
#include <curl/curl.h>

#include "s5curl/s5curl.h"
#include "s5curl/index.h"

int main(int argc, char* argv[]) {
    int n_reads = 10;
    uint64_t max_connects = 5;
    const char *url = "https://gtgseq.s3.amazonaws.com/ont-r10-dna/NA12878/raw/PGXXHX230142_reads.blow5";
    const char *idx_path = "/home/hasindu/scratch/na12878_prom_lsk114/PGXXHX230142_reads.blow5.idx";
    const char *read_ids_path = "/home/hasindu/scratch/na12878_prom_lsk114/chr22/chr22reads.list";

    char **read_ids = malloc(n_reads * sizeof *read_ids);

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
        line[n_chars-1] = '\0'; // remove trailing newline, may cause problems
        fprintf(stderr, "%s\n", line);
        read_ids[cur_read] = strdup(line);
        cur_read++;
    }

    fclose(fp);

    // network stuff
    curl_global_init(CURL_GLOBAL_ALL);
    
    // slow5 setup
    slow5_curl_t *s5c = s5curl_open(url);
    if (s5c == NULL) {
       fprintf(stderr, "Error opening file.\n");
       return -1;
    }

    int ret = 0;
    ret = slow5_idx_load_from_path(s5c->s5p, idx_path);
    // ret = s5curl_idx_load(s5c);
    if (ret < 0) {
        s5curl_close(s5c);
        curl_global_cleanup();
        fprintf(stderr, "Error loading index.\n");
        return -1;
    }
    
    // one read
    slow5_rec_t *read = NULL;
    ret = s5curl_read(s5c, read_ids[0], read);
    if (ret < 0) {
        s5curl_idx_unload(s5c);
        s5curl_close(s5c);
        curl_global_cleanup();
        fprintf(stderr, "Could not perform read.\n");
        return -1;
    }
    slow5_rec_free(read);
    
    // multiple reads
    slow5_rec_t **reads = calloc(n_reads, sizeof *reads);
    ret = s5curl_read_list(
        s5c,
        max_connects,
        n_reads,
        read_ids,
        reads
    );
    if (ret < 0) {
        s5curl_idx_unload(s5c);
        s5curl_close(s5c);
        curl_global_cleanup();
        fprintf(stderr, "Could not perform read list.\n");
        return -1;
    }


    // cleanup
    for (int i = 0; i < n_reads; i++) {
        slow5_rec_free(reads[i]);
    }
    free(reads);

    free(read_ids);
    s5curl_idx_unload(s5c);
    s5curl_close(s5c);
    curl_global_cleanup();

    return 0;
}
