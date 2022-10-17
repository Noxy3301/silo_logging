#pragma once

#define GLOBAL extern

#define CACHE_LINE_SIZE 64
#define THREAD_NUM 10
#define LOGGER_NUM 1
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
