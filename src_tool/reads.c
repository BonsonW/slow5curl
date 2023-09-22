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
#include <slow5curl/s5curl.h>

#define USAGE_MSG "Usage: %s [OPTIONS] [SLOW5_FILE]\n"
#define HELP_LARGE_MSG \
    "Prints the reads in a remote BLOW5 file. "\
    USAGE_MSG \
    "\n" \
    HELP_MSG_HELP \

extern int slow5tools_verbosity_level;

static void print_rid(s5curl_t* s5c) {
    int ret = 0;
    ret = s5curl_idx_load(s5c);
    if (ret < 0) {
        fprintf(stderr,"Error in loading index\n");
        exit(EXIT_FAILURE);
    }

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

    //WARNING("%s","slow5tools is experiemental. Use with caution. Report any bugs under GitHub issues");

    static struct option long_opts[] = {
            {"help", no_argument, NULL, 'h' }, //0
            {NULL, 0, NULL, 0 }
    };

    opt_t user_opts;
    init_opt(&user_opts);

    // Input arguments
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

    curl_global_init(CURL_GLOBAL_ALL);
    s5curl_t *slow5curl = s5curl_open(argv[optind]);
    if (!slow5curl) {
        ERROR("cannot open %s. \n", argv[optind]);
        return EXIT_FAILURE;
    }

    print_rid(slow5curl);

    s5curl_close(slow5curl);
    curl_global_cleanup();

    return EXIT_SUCCESS;
}