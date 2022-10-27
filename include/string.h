#pragma once

#include <stdio.h>
#include <stdint.h>

static void genStringRepeatedNumber(char *string, size_t val_size, size_t thid) {
    size_t digit(1), thidnum(thid);
    for (;;) {
        thidnum /= 10;
        if (thidnum != 0) {
            digit++;
        } else {
            break;
        }
    }

    // generate write value for this thread.
    sprintf(string, "%ld", thid);
    for (uint8_t i = digit; i < val_size - 2; i++) {    // Q:意味わからない大賞受賞,ゼロ埋めしているけど、なんで-2なのかは意味わからない大賞受賞しました
        string[i] = '0';
    }
}