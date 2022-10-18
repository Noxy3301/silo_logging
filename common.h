#pragma once

#define GLOBAL extern

#define CACHE_LINE_SIZE 64
#define WORKER_NUM 9    // worker
#define LOGGER_NUM 3    // logger
#define PAGE_SIZE 4096
#define TUPLE_NUM 1000000
#define EXTIME 3
#define EPOCH_TIME 40

#define NNN                                                                                             \
    do {                                                                                                \
        fprintf(stderr, "%ld %16s %4d %16s\n", (long int)pthread_self(), __FILE__, __LINE__, __func__); \
        fflush(stderr);                                                                                 \
    } while(0)

#define ERR                 \
    do {                    \
        perror("ERROR");    \
        NNN;                \
        exit(1);            \
    } while(0)
