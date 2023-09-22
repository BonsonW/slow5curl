/**
 * @file head.c
 * @brief print the header of a remote BLOW5 file
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
    "Prints the header for a remote BLOW5 file. "\
    USAGE_MSG \
    "\n" \
    HELP_MSG_HELP \

extern int slow5tools_verbosity_level;

static void print_hdr(s5curl_t* s5c) {
    slow5_press_method_t press_method = {SLOW5_COMPRESS_NONE, SLOW5_COMPRESS_NONE};
    slow5_hdr_print(s5c->s5p->header, SLOW5_FORMAT_ASCII, press_method);
}

int head_main(int argc, char **argv, struct program_meta *meta){

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

    print_hdr(slow5curl);

    s5curl_close(slow5curl);
    curl_global_cleanup();

    return EXIT_SUCCESS;
}