#ifndef CMD_H
#define CMD_H

#ifdef __cplusplus
extern "C" {
#endif

#define DEFAULT_NUM_THREADS 128
#define DEFAULT_NUM_PROCESSES 8
#define DEFAULT_BATCH_SIZE 4096
#define DEFAULT_AUXILIARY_FIELDS_NOT_OUT 0
#define DEFAULT_ALLOW_RUN_ID_MISMATCH 0
#define DEFAULT_RETAIN_DIR_STRUCTURE 0
#define DEFAULT_DUMP_ALL 0
#define DEFAULT_CONTINUE_MERGE 0
#define DEFAULT_NUM_RETRY 1
#define DEFAULT_RETRY_WAIT 1

#define TO_STR(x) TO_STR2(x)
#define TO_STR2(x) #x

#define HELP_SMALL_MSG \
    "Try '%s --help' for more information.\n"

#define HELP_MSG_OUTPUT_FILE \
    "    -o, --output FILE             output to FILE [stdout]\n"

#define HELP_MSG_OUTPUT_DIRECTORY \
    "    -d, --out-dir DIR             output to directory\n"

#define HELP_MSG_LOSSLESS \
    "        --lossless                retain information in auxiliary fields during the conversion [true]\n"

// except for view
#define HELP_MSG_OUTPUT_FORMAT \
    "        --to FORMAT               specify output file format [blow5, auto detected using extension if -o FILE is provided]\n"

// for view
#define HELP_MSG_OUTPUT_FORMAT_VIEW \
    "        --to FORMAT               specify output file format [slow5, auto detected using extension if -o FILE is provided]\n"

#define HELP_MSG_PRESS \
    "    -c, --compress REC_MTD        record compression method [zlib] (only for blow5 format)\n" \
    "    -s, --sig-compress SIG_MTD    signal compression method [svb-zd] (only for blow5 format)\n"

#define HELP_MSG_THREADS \
    "    -t, --threads INT             number of threads [" TO_STR(DEFAULT_NUM_THREADS) "]\n"

#define HELP_MSG_PROCESSES \
    "    -p, --iop INT                 number of I/O processes [" TO_STR(DEFAULT_NUM_PROCESSES) "]\n"

#define HELP_MSG_BATCH \
    "    -K, --batchsize INT           number of records loaded to the memory at once [" TO_STR(DEFAULT_BATCH_SIZE) "]\n" \

#define HELP_MSG_RETRY \
    "    -r, --retry INT               number of retries on a fetch before aborting the batch [" TO_STR(DEFAULT_NUM_RETRY) "]\n" \
    "    -w, --wait INT                time (sec) to wait before the next fetch retry [" TO_STR(DEFAULT_RETRY_WAIT) "]\n"

#define HELP_MSG_HELP \
    "    -h, --help                    display this message and exit\n" \

#define HELP_FORMATS_METHODS \
    "FORMAT:\n" \
    "    slow5 - SLOW5 ASCII\n" \
    "    blow5 - SLOW5 binary (BLOW5)\n" \
    "REC_MTD:\n" \
    "    none - no record compression\n" \
    "    zlib - Z library (also known as gzip or DEFLATE)\n" \
    "    zstd - Z standard \n" \
    "SIG_MTD:\n" \
    "    none - no special signal compression\n" \
    "    svb-zd - StreamVByte with zig-zag delta \n\n" \
    "See https://slow5.page.link/slow5curl for a detailed description of these command-line options.\n"

struct program_meta {
    int verbosity_level;
};

struct command {
    const char *name;
    int (*main)(int, char **, struct program_meta *);
};

#ifdef __cplusplus
}
#endif

#endif