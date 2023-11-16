/**
 * @file get.c
 * @brief get read(record) given the read_id from a remote BLOW5 file
 * @author Hiruna Samarakoon (h.samarakoon@garvan.org.au) Sasha Jenner (jenner.sasha@gmail.com), Bonson Wong (bonson.ym@gmail.com)
 * @date 27/02/2021
 */

#define _XOPEN_SOURCE 700
#include <getopt.h>
#include <stdio.h>
#include <unistd.h>

#include <slow5curl/s5curl.h>
#include "../slow5lib/src/slow5_idx.h"
#include "cmd.h"
#include "misc.h"

#define READ_ID_INIT_CAPACITY (128)

#define USAGE_MSG "Usage: %s [OPTIONS] [SLOW5_FILE] [READ_ID]...\n"
#define HELP_LARGE_MSG \
    "Display the read entry for each specified READ_ID from a blow5 file.\n" \
    "If READ_ID is not specified, a newline separated list of read ids will be read from the standard input .\n" \
    USAGE_MSG \
    "\n" \
    "OPTIONS:\n" \
    "    --to FORMAT                   specify output file format (slow5 or blow5)\n" \
    "    -o, --output [FILE]           output contents to FILE (.slow5 or .blow5 extensions) [default: stdout]\n" \
    HELP_MSG_PRESS \
    HELP_MSG_THREADS \
    HELP_MSG_BATCH \
    "    -l --list [FILE]              list of read ids provided as a single-column text file with one read id per line.\n" \
    "    --skip                        warn and continue if a read_id was not found.\n" \
    "    --index [FILE]                path to a custom slow5 index (experimental).\n" \
    HELP_MSG_HELP \
    HELP_FORMATS_METHODS

typedef struct {
    double fetch;
    double encode;
    double write;
    double idx;
    double header;
} times_stamps_t;

/* core data structure that has information that are global to all the threads */
typedef struct {
    enum slow5_fmt format_out;
    slow5_press_method_t press_method;
    bool benchmark;
    times_stamps_t ts;
    int num_retry;
    int retry_wait_sec;
} core_t;

void get_batch(
    core_t *core,
    s5curl_mt_t *s5c_mt,
    slow5_mt_t *s5p_mt,
    slow5_batch_t *db,
    char **rid,
    int num_ids
) {
    // Time stamps
    double start;
    double end;

    start = slow5_realtime();
    s5curl_get_batch(s5c_mt, db, rid, num_ids);
    end = slow5_realtime();
    core->ts.fetch += end - start;

    start = slow5_realtime();
    if (!core->benchmark) {
        slow5_file_t *sf = s5p_mt->sf;

        // dummy settings
        enum slow5_fmt o_fmt = sf->format;
        enum slow5_press_method o_rec_press = sf->compress->record_press->method;
        enum slow5_press_method o_sig_press = sf->compress->signal_press->method;

        sf->format = core->format_out;
        sf->compress->record_press->method = core->press_method.record_method;
        sf->compress->signal_press->method = core->press_method.signal_method;

        // peform
        slow5_encode_batch(s5p_mt, db, db->n_rec);

        // restore
        sf->format = o_fmt;
        sf->compress->record_press->method = o_rec_press;
        sf->compress->signal_press->method = o_sig_press;
    }
    end = slow5_realtime();
    core->ts.encode += end - start;
}

bool get_single(
    s5curl_t *s5c,
    const char *read_id,
    core_t *core,
    FILE *slow5_file_pointer,
    CURL *curl
) {
    bool success = true;

    int ret = 0;
    slow5_rec_t *record = NULL;

    // Time stamps
    double start;
    double end;

    start = slow5_realtime();
    ret = s5curl_get(read_id, &record, curl, s5c);
    for (int k = 1; k <= core->num_retry && ret == S5CURL_ERR_FETCH; ++k) {
		VERBOSE("Retry %d/%d: fetch %s", k, core->num_retry, read_id);
        sleep(core->retry_wait_sec);
		ret = s5curl_get(read_id, &record, curl, s5c);
	}
    end = slow5_realtime();
    core->ts.fetch = end - start;

    start = slow5_realtime();
    if (record == NULL || ret != 0) {
        success = false;
    } else {
        if (core->benchmark == false) {
            struct slow5_press* compress = slow5_press_init(core->press_method);
            if (!compress) {
                ERROR("Could not initialize the slow5 compression method%s","");
                exit(EXIT_FAILURE);
            }
            slow5_rec_fwrite(slow5_file_pointer, record, s5c->s5p->header->aux_meta, core->format_out, compress);
            slow5_press_free(compress);
        }
        slow5_rec_free(record);
    }
    end = slow5_realtime();
    core->ts.fetch = end - start;

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
        {"cache",       required_argument, NULL, 0 }, //11
        {NULL, 0, NULL, 0 }
    };

    bool read_stdin = false;
    bool benchmark = false;

    opt_t user_opts;
    init_opt(&user_opts);

    // Input arguments
    char* read_list_file_in = NULL;
    const char *slow5_index = NULL;
    const char *idx_cache_path = NULL;

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
                    case 11:
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

    if (skip_flag) {
        WARNING("Will skip records that are not found%s","");
        slow5_set_exit_condition(SLOW5_EXIT_OFF);
    }

    if (parse_num_threads(&user_opts,argc,argv,meta) < 0) {
        EXIT_MSG(EXIT_FAILURE, argv, meta);
        return EXIT_FAILURE;
    }

    if (parse_batch_size(&user_opts,argc,argv) < 0) {
        EXIT_MSG(EXIT_FAILURE, argv, meta);
        return EXIT_FAILURE;
    }

    if (parse_format_args(&user_opts,argc,argv,meta) < 0) {
        EXIT_MSG(EXIT_FAILURE, argv, meta);
        return EXIT_FAILURE;
    }
    if (auto_detect_formats(&user_opts, 1) < 0) {
        EXIT_MSG(EXIT_FAILURE, argv, meta);
        return EXIT_FAILURE;
    }

    if (parse_compression_opts(&user_opts) < 0) {
        EXIT_MSG(EXIT_FAILURE, argv, meta);
        return EXIT_FAILURE;
    }

    if (benchmark && user_opts.arg_fname_out) {
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
    if (read_list_file_in) {
        read_list_in = fopen(read_list_file_in, "r");
        if (!read_list_in) {
            ERROR("Output file %s could not be opened - %s.", read_list_file_in, strerror(errno));
            return EXIT_FAILURE;
        }
    }

    char *f_in_name = argv[optind];

    slow5_press_method_t press_out = {user_opts.record_press_out, user_opts.signal_press_out};

    // Setup core
    core_t core = {0};
    core.format_out = user_opts.fmt_out;
    core.press_method = press_out;
    core.benchmark = benchmark;
    core.num_retry = 1;
    core.retry_wait_sec = 1;

    // Time stamps
    double start;
    double end;

    s5curl_global_init();

    VERBOSE("%s", "Loading remote BLOW5 file.");
    start = slow5_realtime();
    s5curl_t *slow5curl = s5curl_open(f_in_name);
    if (!slow5curl) {
        ERROR("cannot open %s. \n", f_in_name);
        return EXIT_FAILURE;
    }
    end = slow5_realtime();
    core.ts.header = end - start;

    if (benchmark == false) {
        if(slow5_hdr_fwrite(user_opts.f_out, slow5curl->s5p->header, user_opts.fmt_out, press_out) == -1){
            ERROR("Could not write the output header%s\n", "");
            return EXIT_FAILURE;
        }
    }

    VERBOSE("%s", "Loading index (this may take a while).");
    start = slow5_realtime();
    if (slow5_index == NULL) {
        int ret_idx = s5curl_idx_load(slow5curl);
        if (ret_idx < 0) {
            ERROR("Error loading index file for %s\n", f_in_name);
            EXIT_MSG(EXIT_FAILURE, argv, meta);
            return EXIT_FAILURE;
        }
        if (idx_cache_path != NULL) {
            VERBOSE("%s", "Caching index.");
            slow5_idx_t *idx = slow5curl->s5p->index;
            copy_file_to(idx->fp, idx_cache_path);
        }
    } else {
        int ret_idx = s5curl_idx_load_with(slow5curl, slow5_index);
        if (ret_idx < 0) {
            ERROR("Error loading index file for %s from file path %s\n", f_in_name, slow5_index);
            EXIT_MSG(EXIT_FAILURE, argv, meta);
            return EXIT_FAILURE;
        }
    }
    end = slow5_realtime();
    core.ts.idx = end - start;

    if (read_stdin) {
        VERBOSE("%s", "Fetching reads.");

        // Setup multithreading structures
        s5curl_mt_t *s5c_mt = s5curl_init_mt(user_opts.num_threads, slow5curl);
        s5c_mt->num_retry = core.num_retry;
        s5c_mt->retry_wait_sec = core.retry_wait_sec;
        slow5_mt_t *s5p_mt = slow5_init_mt(s5c_mt->num_thread, s5c_mt->s5c->s5p);
        slow5_batch_t *db = slow5_init_batch(user_opts.read_id_batch_capacity);
        int64_t cap_ids = READ_ID_INIT_CAPACITY;
        char **rid = malloc(cap_ids * sizeof *rid);
        
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
                    rid= realloc(rid, cap_ids * sizeof *rid);
                }
                rid[num_ids] = curr_id;
                ++num_ids;
            }

            // Fetch records for read ids in the batch
            start = slow5_realtime();
            get_batch(&core, s5c_mt, s5p_mt, db, rid, num_ids);
            end = slow5_realtime();

            VERBOSE("Fetched %ld reads in %.3f seconds.", num_ids, end - start);

            // Print records
            start = slow5_realtime();
            if (benchmark == false) {
                for (int64_t i = 0; i < num_ids; ++i) {
                    void *buffer = db->mem_records[i];
                    int len = db->mem_bytes[i];
                    if (buffer == NULL || len < 0) {
                        if (skip_flag) continue;
                        ERROR("Could not write the fetched read %s.", db->rid[i]);
                        return EXIT_FAILURE;
                    } else {
                        fwrite(buffer, 1, len, user_opts.f_out);
                        free(buffer);
                    }
                    free(rid[i]);
                }
            }
            end = slow5_realtime();
            core.ts.write += end - start;
        }

        s5curl_free_mt(s5c_mt);
        slow5_free_mt(s5p_mt);
        slow5_free_batch(db);

        free(rid);

    } else {
        S5CURLCONN *curl = s5curl_conn_init();
        start = slow5_realtime();
        for (int i = optind + 1; i < argc; ++i){
            bool success = get_single(slow5curl, argv[i], &core, user_opts.f_out, curl);
            
            if (!success) {
                if(skip_flag) continue;
                ERROR("%s","Error fetching records.");
                return EXIT_FAILURE;
            }
        }
        end = slow5_realtime();
        VERBOSE("Fetched %d reads in %.3f seconds.", argc - (optind + 1), end - start);

        curl_easy_cleanup(curl);
    }
    VERBOSE("%s","Finished.\n");

    VERBOSE("%s", "Timings:");
    VERBOSE("   Header Load:   %.3f sec.", core.ts.header);
    VERBOSE("   Index Load:    %.3f sec.", core.ts.idx);
    VERBOSE("   Fetch:         %.3f sec.", core.ts.fetch);
    VERBOSE("   Encode:        %.3f sec.", core.ts.encode);
    VERBOSE("   Write:         %.3f sec.", core.ts.write);

    if (benchmark == false) {
        if (user_opts.fmt_out == SLOW5_FORMAT_BINARY) {
            slow5_eof_fwrite(user_opts.f_out);
        }
    }

    s5curl_idx_unload(slow5curl);
    s5curl_close(slow5curl);
    fclose(read_list_in);
    if (user_opts.arg_fname_out != NULL) fclose(user_opts.f_out);

    s5curl_global_cleanup();

    EXIT_MSG(EXIT_SUCCESS, argv, meta);
    return EXIT_SUCCESS;
}