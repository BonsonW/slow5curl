/**
 * @file get.c
 * @brief get read(record) given the read_id from a SLOW5 file
 * @author Hiruna Samarakoon (h.samarakoon@garvan.org.au) Sasha Jenner (jenner.sasha@gmail.com), Hasindu Gamaarachchi (hasindu@garvan.org.au)
 * @date 27/02/2021
 */

#define _XOPEN_SOURCE 700
#include <getopt.h>
#include <stdio.h>

#include <slow5curl/s5curl.h>
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

/* core data structure that has information that are global to all the threads */
typedef struct {
    enum slow5_fmt format_out;
    slow5_press_method_t press_method;
    bool benchmark;
} core_t;

void get_batch(
    core_t *core,
    s5curl_mt_t *mt,
    slow5_batch_t *db,
    char **rid,
    int num_ids
) {
    // Time stamps
    double fetch_time = 0;
    double compress_time = 0;

    double start;
    double end;

    start = slow5_realtime();
    s5curl_get_batch(mt, db, rid, num_ids);
    end = slow5_realtime();
    fetch_time = end - start;
    VERBOSE("fetch time = %.3f sec", fetch_time);

    start = slow5_realtime();
    for (size_t i = 0; i < num_ids; ++i) {
        if (db->slow5_rec[i] == NULL) {
            db->mem_records[i] = NULL;
            db->mem_bytes[i] = -1;
        } else {
            if (core->benchmark == false) {
                size_t record_size;
                struct slow5_press* compress = slow5_press_init(core->press_method);
                if (!compress) {
                    ERROR("Could not initialize the slow5 compression method%s","");
                    exit(EXIT_FAILURE);
                }
                db->mem_records[i] = slow5_rec_to_mem(db->slow5_rec[i], mt->s5c->s5p->header->aux_meta, core->format_out, compress, &record_size);
                db->mem_bytes[i] = record_size;
                slow5_press_free(compress);
            }
        }
    }
    end = slow5_realtime();
    compress_time = end - start;
    VERBOSE("compression time = %.3f sec", compress_time);
}

bool get_single(s5curl_t *s5c, const char *read_id, char **argv, struct program_meta *meta, enum slow5_fmt format_out,
                  slow5_press_method_t press_method, bool benchmark, FILE *slow5_file_pointer, CURL *curl) {

    bool success = true;

    int len = 0;
    slow5_rec_t *record = NULL;

    // Time stamps
    double fetch_time = 0;
    double compress_time = 0;

    double start;
    double end;

    start = slow5_realtime();
    len = s5curl_get(s5c, curl, read_id, &record);
    end = slow5_realtime();
    fetch_time = end - start;
    VERBOSE("fetch time = %.3f sec", fetch_time);

    start = slow5_realtime();
    if (record == NULL || len != 0) {
        success = false;
    } else {
        if (benchmark == false) {
            struct slow5_press* compress = slow5_press_init(press_method);
            if (!compress) {
                ERROR("Could not initialize the slow5 compression method%s","");
                exit(EXIT_FAILURE);
            }
            slow5_rec_fwrite(slow5_file_pointer, record, s5c->s5p->header->aux_meta, format_out, compress);
            slow5_press_free(compress);
        }
        slow5_rec_free(record);
    }
    end = slow5_realtime();
    compress_time = end - start;
    VERBOSE("compression time = %.3f sec", compress_time);

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

    // Time stamps
    double read_time = 0;
    double write_time = 0;
    double idx_load_time = 0;
    double file_load_time = 0;

    double start;
    double end;

    curl_global_init(CURL_GLOBAL_ALL);

    start = slow5_realtime();
    s5curl_t *slow5curl = s5curl_open(f_in_name);
    if (!slow5curl) {
        ERROR("cannot open %s. \n", f_in_name);
        return EXIT_FAILURE;
    }
    end = slow5_realtime();
    file_load_time = end - start;

    slow5_press_method_t press_out = {user_opts.record_press_out, user_opts.signal_press_out};

    if (benchmark == false) {
        if(slow5_hdr_fwrite(user_opts.f_out, slow5curl->s5p->header, user_opts.fmt_out, press_out) == -1){
            ERROR("Could not write the output header%s\n", "");
            return EXIT_FAILURE;
        }
    }

    start = slow5_realtime();
    if (slow5_index == NULL) {
        int ret_idx = s5curl_idx_load(slow5curl);
        if (ret_idx < 0) {
            ERROR("Error loading index file for %s\n", f_in_name);
            EXIT_MSG(EXIT_FAILURE, argv, meta);
            return EXIT_FAILURE;
        }
    } else {
        WARNING("%s","Loading index from custom path is an experimental feature. keep an eye.");
        int ret_idx = s5curl_idx_load_with(slow5curl, slow5_index);
        if (ret_idx < 0) {
            ERROR("Error loading index file for %s from file path %s\n", f_in_name, slow5_index);
            EXIT_MSG(EXIT_FAILURE, argv, meta);
            return EXIT_FAILURE;
        }
    }
    end = slow5_realtime();
    idx_load_time = end - start;

    if (read_stdin) {
        // Setup core
        core_t core;
        core.format_out = user_opts.fmt_out;
        core.press_method = press_out;
        core.benchmark = benchmark;
        
        // Setup multithreading structures
        s5curl_mt_t *mt = s5curl_init_mt(user_opts.num_threads, slow5curl);
        slow5_batch_t *db = slow5_init_batch(user_opts.read_id_batch_capacity);
        int64_t cap_ids = READ_ID_INIT_CAPACITY;
        char **rid = malloc(cap_ids * sizeof *rid);
        int64_t rid_real_size = 0;

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
            if (num_ids > rid_real_size) {
                rid_real_size = num_ids;
            }

            // Fetch records for read ids in the batch
            start = slow5_realtime();
            // Fetch records for read ids in the batch
            get_batch(&core, mt, db, rid, num_ids);
            end = slow5_realtime();
            read_time += end - start;

            VERBOSE("Fetched %ld reads.", num_ids);

            // Print records
            if (benchmark == false) {
                for (int64_t i = 0; i < num_ids; ++ i) {
                    void *buffer = db->mem_records[i];
                    int len = db->mem_bytes[i];
                    if (buffer == NULL || len < 0) {
                        if (skip_flag) continue;
                        ERROR("Could not write the fetched read.%s", "");
                        return EXIT_FAILURE;
                    } else {
                        fwrite(buffer, 1, len, user_opts.f_out);
                        free(buffer);
                    }
                    free(rid[i]);
                }
            }
        }
        
        s5curl_free_mt(mt);
        slow5_free_batch(db);

        free(rid);
    } else {
        CURL *curl = curl_easy_init();
        for (int i = optind + 1; i < argc; ++i){
            start = slow5_realtime();
            bool success = get_single(slow5curl, argv[i], argv, meta, user_opts.fmt_out, press_out, benchmark, user_opts.f_out, curl);
            end = slow5_realtime();
            read_time += end - start;

            if (!success) {
                if(skip_flag) continue;
                ERROR("Could not fetch records.%s","");
                return EXIT_FAILURE;
            }
        }
        curl_easy_cleanup(curl);
    }

    VERBOSE("file load time = %.3f sec", file_load_time);
    VERBOSE("index load time = %.3f sec", idx_load_time);
    VERBOSE("total read time = %.3f sec", read_time);

    if (benchmark == false) {
        if (user_opts.fmt_out == SLOW5_FORMAT_BINARY) {
            start = slow5_realtime();
            slow5_eof_fwrite(user_opts.f_out);
            end = slow5_realtime();
            VERBOSE("write time = %.3f sec", write_time);
        }
    }

    s5curl_idx_unload(slow5curl);
    s5curl_close(slow5curl);
    fclose(read_list_in);
    if (user_opts.arg_fname_out != NULL) fclose(user_opts.f_out);

    curl_global_cleanup();

    EXIT_MSG(EXIT_SUCCESS, argv, meta);
    return EXIT_SUCCESS;
}