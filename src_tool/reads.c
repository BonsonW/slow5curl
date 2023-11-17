/**
 * @file reads.c
 * @brief print reads from a remote BLOW5 file
 * @author Hasindu Gamaarachchi (hasindu@garvan.org.au), Bonson Wong (bonson.ym@gmail.com)
 * @date 26/08/2021
 */

#include <getopt.h>
#include "error.h"
#include "cmd.h"
#include "misc.h"
#include "../src/index.h"
#include "../slow5lib/src/slow5_idx.h"
#include <slow5curl/s5curl.h>

#define USAGE_MSG "Usage: %s [OPTIONS] [SLOW5_FILE]\n"
#define HELP_LARGE_MSG \
    "Prints the reads in a remote BLOW5 file. "\
    USAGE_MSG \
    "\n" \
    HELP_MSG_HELP \

extern int slow5curl_verbosity_level;

static void print_rid(s5curl_t* s5c) {
    uint64_t num_reads = 0;
    char **read_ids = slow5_get_rids(s5c->s5p, &num_reads);
    if (read_ids == NULL) {
        fprintf(stderr,"Error in getting list of read IDs\n");
        exit(EXIT_FAILURE);
    }

    for(uint64_t i = 0; i < num_reads; i++) {
        printf("%s\n", read_ids[i]);
    }

    s5curl_idx_unload(s5c);
}

int reads_main(int argc, char **argv, struct program_meta *meta){

    // Debug: print arguments
    print_args(argc,argv);

    static struct option long_opts[] = {
        {"help", no_argument, NULL, 'h' }, //0
        {"index",       required_argument, NULL, 0 }, //1
        {"cache",       required_argument, NULL, 0 }, //2
        {NULL, 0, NULL, 0 }
    };

    opt_t user_opts;
    init_opt(&user_opts);

    // Input arguments
    const char *slow5_index = NULL;
    const char *idx_cache_path = NULL;

    int longindex = 0;
    int opt;

    // Parse options
    while ((opt = getopt_long(argc, argv, "ht:K:", long_opts, &longindex)) != -1) {
        DEBUG("opt='%c', optarg=\"%s\", optind=%d, opterr=%d, optopt='%c'",
                  opt, optarg, optind, opterr, optopt);
        switch (opt) {
            case 'h':
                DEBUG("displaying large help message%s","");

                fprintf(stdout, HELP_LARGE_MSG, argv[0]);

                EXIT_MSG(EXIT_SUCCESS, argv, meta);
                exit(EXIT_SUCCESS);
            case 'K':
                user_opts.arg_batch = optarg;
                break;
            case 't':
                user_opts.arg_num_threads = optarg;
                break;
            case 0  :
                switch (longindex) {
                    case 1:
                        slow5_index = optarg;
                        break;
                    case 2:
                        idx_cache_path = optarg;
                        break;
                }
                break;

            default: // case '?'
                fprintf(stderr, HELP_SMALL_MSG, argv[0]);
                EXIT_MSG(EXIT_FAILURE, argv, meta);
                return EXIT_FAILURE;
        }
    }

    if (argc - optind > 1){
        ERROR("%s", "Too many arguments");
        fprintf(stderr, HELP_SMALL_MSG, argv[0]);
        EXIT_MSG(EXIT_FAILURE, argv, meta);
        exit(EXIT_FAILURE);
    }

    s5curl_global_init();

    char *f_in_name = argv[optind];

    VERBOSE("%s", "Loading remote BLOW5 file.");
    s5curl_t *slow5curl = s5curl_open(f_in_name);
    if (!slow5curl) {
        ERROR("cannot open %s. \n", f_in_name);
        return EXIT_FAILURE;
    }

    VERBOSE("%s", "Loading index (this may take a while).");
    if (slow5_index == NULL) {
        int ret_idx;
        if (idx_cache_path != NULL) {
            VERBOSE("Caching index to %s.", idx_cache_path);
            ret_idx = s5curl_idx_load_and_cache(slow5curl, idx_cache_path);
        } else {
            ret_idx = s5curl_idx_load(slow5curl);
        }
        if (ret_idx < 0) {
            ERROR("Error loading index file for %s\n", f_in_name);
            EXIT_MSG(EXIT_FAILURE, argv, meta);
            return EXIT_FAILURE;
        }

    } else {
        int ret_idx;
        
        if (idx_cache_path != NULL) {
            VERBOSE("Caching index to %s.", idx_cache_path);
            ret_idx = s5curl_idx_load_with_and_cache(slow5curl, slow5_index, idx_cache_path);
        } else {
            ret_idx = s5curl_idx_load_with(slow5curl, slow5_index);
        }
        
        if (ret_idx < 0) {
            ERROR("Error loading index file for %s from file path %s\n", f_in_name, slow5_index);
            EXIT_MSG(EXIT_FAILURE, argv, meta);
            return EXIT_FAILURE;
        }
    }

    print_rid(slow5curl);

    s5curl_close(slow5curl);
    s5curl_global_cleanup();

    return EXIT_SUCCESS;
}