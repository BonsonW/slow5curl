/**
 * @file get.c
 * @brief get read(record) given the read_id from a SLOW5 file
 * @author Hiruna Samarakoon (h.samarakoon@garvan.org.au) Sasha Jenner (jenner.sasha@gmail.com), Hasindu Gamaarachchi (hasindu@garvan.org.au)
 * @date 27/02/2021
 */
#include <getopt.h>
#include <stdio.h>

#include <curl/curl.h>

#include <slow5/slow5.h>
#include <slow5curl/s5curl.h>
#include "thread.h"
#include "cmd.h"
#include "misc.h"

#define READ_ID_INIT_CAPACITY (128)

#define USAGE_MSG "Usage: %s [OPTIONS] [SLOW5_FILE] [READ_ID]...\n"
#define HELP_LARGE_MSG \
    "Display the read entry for each specified read id from a slow5 file.\n" \
    "With no READ_ID, read from standard input newline separated read ids.\n" \
    USAGE_MSG \
    "\n" \
    "OPTIONS:\n" \
    "    --to FORMAT                   specify output file format\n" \
    "    -o, --output [FILE]           output contents to FILE [default: stdout]\n" \
    HELP_MSG_PRESS \
    HELP_MSG_THREADS \
    HELP_MSG_BATCH \
    "    -l --list [FILE]              list of read ids provided as a single-column text file with one read id per line.\n" \
    "    --skip                        warn and continue if a read_id was not found.\n" \
    "    --index [FILE]                path to a custom slow5 index (experimental).\n" \
    HELP_MSG_HELP \
    HELP_FORMATS_METHODS

void get_batch(core_t *core, db_t *db) {
    slow5_rec_t **reads = calloc(db->n_batch, sizeof *reads);

    int res = s5curl_get_batch(
        core->s5c,
        db->conns,
        db->curl_multi,
        db->n_batch,
        db->read_id,
        reads
    );

    // TODO: print errors for get batch, or maybe just skip it and have it return reads??

    for (size_t i = 0; i < db->n_batch; ++i) {
        slow5_rec_t *record = reads[i];

        // todo: does not check len of record read (check this, may not be relevant)
        if (record == NULL) {
            ++db->n_err;
            db->read_record[i].buffer = NULL;
            db->read_record[i].len = -1;
        } else {
            if (core->benchmark == false){
                size_t record_size;
                struct slow5_press* compress = slow5_press_init(core->press_method);
                if(!compress){
                    ERROR("Could not initialize the slow5 compression method%s","");
                    exit(EXIT_FAILURE);
                }
                db->read_record[i].buffer = slow5_rec_to_mem(record, core->s5c->s5p->header->aux_meta, core->format_out, compress, &record_size);
                db->read_record[i].len = record_size;
                slow5_press_free(compress);
            }
            slow5_rec_free(record);
        }
        free(db->read_id[i]);
    }
    free(reads);
}

bool get_single(slow5_curl_t *s5c, const char *read_id, char **argv, struct program_meta *meta, enum slow5_fmt format_out,
                  slow5_press_method_t press_method, bool benchmark, FILE *slow5_file_pointer, CURL *curl) {

    bool success = true;

    int len = 0;
    slow5_rec_t *record=NULL;

    len = s5curl_get(s5c, curl, read_id, record);

    if (record == NULL || len < 0) {
        success = false;

    } else {
        if (benchmark == false){
            struct slow5_press* compress = slow5_press_init(press_method);
            if(!compress){
                ERROR("Could not initialize the slow5 compression method%s","");
                exit(EXIT_FAILURE);
            }
            slow5_rec_fwrite(slow5_file_pointer, record, s5c->s5p->header->aux_meta, format_out, compress);
            slow5_press_free(compress);
        }
        slow5_rec_free(record);
    }

    return success;
}

int get_main(int argc, char **argv, struct program_meta *meta) {

    // Debug: print arguments
    print_args(argc, argv);

    // No arguments given
    if (argc <= 1) {
        fprintf(stderr, HELP_LARGE_MSG, argv[0]);
        EXIT_MSG(EXIT_FAILURE, argv, meta);
        return EXIT_FAILURE;
    }

    static struct option long_opts[] = {
        {"to",          required_argument, NULL, 'b'},    //0
        {"compress",    required_argument, NULL, 'c'},  //1
        {"sig-compress",required_argument,  NULL, 's'}, //2
        {"batchsize",   required_argument, NULL, 'K'},  //3
        {"output",      required_argument, NULL, 'o'}, //4
        {"list",        required_argument, NULL, 'l'},  //5
        {"skip",        no_argument, NULL, 0},  //6
        {"threads",     required_argument, NULL, 't' }, //7
        {"help",        no_argument, NULL, 'h' }, //8
        {"benchmark",   no_argument, NULL, 'e' }, //9
        {"index",       required_argument, NULL, 0 }, //10
        {NULL, 0, NULL, 0 }
    };

    bool read_stdin = false;
    bool benchmark = false;

    opt_t user_opts;
    init_opt(&user_opts);

    // Input arguments
    char* read_list_file_in = NULL;
    const char *slow5_index = NULL;

    int opt;
    int longindex = 0;

    int skip_flag = 0;

    // Parse options
    while ((opt = getopt_long(argc, argv, "o:b:c:s:K:l:t:he", long_opts, &longindex)) != -1) {
        DEBUG("opt='%c', optarg=\"%s\", optind=%d, opterr=%d, optopt='%c'",
                  opt, optarg, optind, opterr, optopt);
        switch (opt) {
            case 'b':
                user_opts.arg_fmt_out = optarg;
                break;
            case 'c':
                user_opts.arg_record_press_out = optarg;
                break;
            case 's':
                user_opts.arg_signal_press_out = optarg;
                break;
            case 't':
                user_opts.arg_num_threads = optarg;
                break;
            case 'e':
                benchmark = true;
                break;
            case 'o':
                user_opts.arg_fname_out = optarg;
                break;
            case 'K':
                user_opts.arg_batch = optarg;
                break;
            case 'l':
                read_list_file_in = optarg;
                break;
            case 'h':
                DEBUG("displaying large help message%s","");
                fprintf(stdout, HELP_LARGE_MSG, argv[0]);
                EXIT_MSG(EXIT_SUCCESS, argv, meta);
                exit(EXIT_SUCCESS);
            case 0  :
                switch (longindex) {
                    case 6:
                        skip_flag = 1;
                        break;
                    case 10:
                        slow5_index = optarg;
                        break;
                }
                break;

            default: // case '?'
                fprintf(stderr, HELP_SMALL_MSG, argv[0]);
                EXIT_MSG(EXIT_FAILURE, argv, meta);
                return EXIT_FAILURE;
        }
    }

    if(skip_flag){
        WARNING("Will skip records that are not found%s","");
        slow5_set_exit_condition(SLOW5_EXIT_OFF);
    }

    if(parse_num_threads(&user_opts,argc,argv,meta) < 0){
        EXIT_MSG(EXIT_FAILURE, argv, meta);
        return EXIT_FAILURE;
    }

    if(parse_batch_size(&user_opts,argc,argv) < 0){
        EXIT_MSG(EXIT_FAILURE, argv, meta);
        return EXIT_FAILURE;
    }

    if(parse_format_args(&user_opts,argc,argv,meta) < 0){
        EXIT_MSG(EXIT_FAILURE, argv, meta);
        return EXIT_FAILURE;
    }
    if(auto_detect_formats(&user_opts, 1) < 0){
        EXIT_MSG(EXIT_FAILURE, argv, meta);
        return EXIT_FAILURE;
    }

    if(parse_compression_opts(&user_opts) < 0){
        EXIT_MSG(EXIT_FAILURE, argv, meta);
        return EXIT_FAILURE;
    }

    if(benchmark && user_opts.arg_fname_out){
        ERROR("Benchmark does not support writing records out%s", "");
        return EXIT_FAILURE;
    }

    // Parse output argument
    if (user_opts.arg_fname_out != NULL) {
        DEBUG("opening output file%s","");
        // Create new file or
        // Truncate existing file
        FILE *new_file;
        new_file = fopen(user_opts.arg_fname_out, "w");

        // An error occurred
        if (new_file == NULL) {
            ERROR("File '%s' could not be opened - %s.",
                  user_opts.arg_fname_out, strerror(errno));

            EXIT_MSG(EXIT_FAILURE, argv, meta);
            return EXIT_FAILURE;
        } else {
            user_opts.f_out = new_file;
        }
    }

    // Check for remaining files to parse
    if (optind >= argc) {
        ERROR("missing slow5 or blow5 file%s", "");
        fprintf(stderr, HELP_SMALL_MSG, argv[0]);

        EXIT_MSG(EXIT_FAILURE, argv, meta);
        return EXIT_FAILURE;

    } else if (optind == argc - 1) {
        read_stdin = true;
    }

    FILE* read_list_in = stdin;
    if(read_list_file_in){
        read_list_in = fopen(read_list_file_in, "r");
        if (!read_list_in) {
            ERROR("Output file %s could not be opened - %s.", read_list_file_in, strerror(errno));
            return EXIT_FAILURE;
        }
    }

    char *f_in_name = argv[optind];
    
    curl_global_init(CURL_GLOBAL_ALL);

    slow5_curl_t *slow5curl = s5curl_open(f_in_name);
    if (!slow5curl) {
        ERROR("cannot open %s. \n", f_in_name);
        return EXIT_FAILURE;
    }

    slow5_press_method_t press_out = {user_opts.record_press_out, user_opts.signal_press_out};

    if(benchmark == false){
        if(slow5_hdr_fwrite(user_opts.f_out, slow5curl->s5p->header, user_opts.fmt_out, press_out) == -1){
            ERROR("Could not write the output header%s\n", "");
            return EXIT_FAILURE;
        }
    }

    if(slow5_index == NULL){
        int ret_idx = s5curl_idx_load(slow5curl);
        if (ret_idx < 0) {
            ERROR("Error loading index file for %s\n", f_in_name);
            EXIT_MSG(EXIT_FAILURE, argv, meta);
            return EXIT_FAILURE;
        }
    } else {
        WARNING("%s","Loading index from custom path is an experimental feature. keep an eye.");
        int ret_idx = slow5_idx_load_from_path(slow5curl->s5p, slow5_index);
        if (ret_idx < 0) {
            ERROR("Error loading index file for %s from file path %s\n", f_in_name, slow5_index);
            EXIT_MSG(EXIT_FAILURE, argv, meta);
            return EXIT_FAILURE;
        }
    }

    if (read_stdin) {
        // Time spend reading slow5
        double read_time = 0;

        // Setup multithreading structures
        core_t core;
        core.num_thread = user_opts.num_threads;
        core.s5c = slow5curl;
        core.format_out = user_opts.fmt_out;
        core.press_method = press_out;
        core.benchmark = benchmark;

        db_t db = { 0 };
        int64_t cap_ids = READ_ID_INIT_CAPACITY;
        db.read_id = (char **) malloc(cap_ids * sizeof(char*));
        db.read_record = (raw_record_t*) malloc(cap_ids * sizeof(raw_record_t));
        MALLOC_CHK(db.read_id);
        MALLOC_CHK(db.read_record);

        db.curl_multi = curl_multi_init();
        db.conns = s5curl_open_conns(core.num_thread);
        MALLOC_CHK(db.curl_multi);
        MALLOC_CHK(db.conns);

        bool end_of_file = false;
        while (!end_of_file) {
            int64_t num_ids = 0;
            while (num_ids < user_opts.read_id_batch_capacity) {
                char *buf = NULL;
                size_t cap_buf = 0;
                ssize_t nread;
                if ((nread = getline(&buf, &cap_buf, read_list_in)) == -1) {
                    end_of_file = true;
                    free(buf);
                    break;
                }
                size_t len_buf = nread - 1; // Ignore '\n'
                char *curr_id = strndup(buf, len_buf);
                curr_id[len_buf] = '\0'; // Add string terminator '\0'
                free(buf); // Free buffer
                if (num_ids >= cap_ids) {
                    // Double read id list capacity
                    cap_ids *= 2;
                    db.read_id = (char **) realloc(db.read_id, cap_ids * sizeof *db.read_id);
                    db.read_record = (raw_record_t*) realloc(db.read_record, cap_ids * sizeof *db.read_record);
                }
                db.read_id[num_ids] = curr_id;
                ++ num_ids;
            }

            db.n_batch = num_ids;

            // Measure reading time
            double start = slow5_realtime();

            // Fetch records for read ids in the batch
            get_batch(&core, &db);

            double end = slow5_realtime();
            read_time += end - start;

            VERBOSE("Fetched %ld reads of %ld", num_ids - db.n_err, num_ids);

            // Print records
            if(benchmark == false){
                for (int64_t i = 0; i < num_ids; ++ i) {
                    void *buffer = db.read_record[i].buffer;
                    int len = db.read_record[i].len;
                    if (buffer == NULL || len < 0) {
                        if(skip_flag) continue;
                        ERROR("Could not write the fetched read.%s", "");
                        return EXIT_FAILURE;
                    } else {
                        fwrite(buffer, 1, len, user_opts.f_out);
                        free(buffer);
                    }
                }
            }
        }
        // Print total time to read slow5
        VERBOSE("read time = %.3f sec", read_time);
        // Free everything
        free(db.read_id);
        free(db.read_record);

        curl_multi_cleanup(db.curl_multi);
        s5curl_close_conns(db.conns);
    } else {
        CURL *curl = curl_easy_init();
        for (int i = optind + 1; i < argc; ++i){
            curl_easy_reset(curl);
            bool success = get_single(slow5curl, argv[i], argv, meta, user_opts.fmt_out, press_out, benchmark, user_opts.f_out, curl);
            if (!success) {
                if(skip_flag) continue;
                ERROR("Could not fetch records.%s","");
                return EXIT_FAILURE;
            }
        }
        curl_easy_cleanup(curl);
    }

    if (benchmark == false) {
        if (user_opts.fmt_out == SLOW5_FORMAT_BINARY) {
                slow5_eof_fwrite(user_opts.f_out);
            }
    }

    s5curl_idx_unload(slow5curl);
    s5curl_close(slow5curl);
    fclose(read_list_in);

    curl_global_cleanup();

    EXIT_MSG(EXIT_SUCCESS, argv, meta);
    return EXIT_SUCCESS;
}