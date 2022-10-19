#pragma once

#define INLINE __attribute__((always_inline)) inline

#include <stdint.h>

#include "debug.h"
#include "procedure.h"
#include "random.h"
#include "result.h"
#include "tsc.h"
#include "zipf.h"


inline static bool chkClkSpan(const uint64_t start, const uint64_t stop, const uint64_t threshold) {
    uint64_t diff = 0;
    diff = stop - start;
    if (diff > threshold) {
        return true;
    } else {
        return false;
    }
}

inline static void makeProcedure(std::vector<Procedure> &pro, Xoroshiro128Plus &rnd, FastZipf &zipf, 
                                 size_t tuple_num, size_t max_ope, size_t thread_num, size_t rraito,
                                 bool rmw, bool ycsb, bool partition, size_t thread_id, Result &res) {
    pro.clear();
    bool ronly_flag = true, wonly_flag = true;
    for (size_t i = 0; i < max_ope; i++) {
        uint64_t tmpkey;
        // keyの決定
        if (ycsb) {
            if (partition) {
                // 使わないけど一応実装だけしておく
                size_t block_size = tuple_num / thread_num;
                tmpkey = (block_size * thread_id) + (zipf() % block_size);
            } else {
                tmpkey = zipf() % tuple_num;
            }
        } else {
            // 使わないけど実装だけしておく
            if (partition) {
                size_t block_size = tuple_num / thread_num;
                tmpkey = (block_size * thread_id) + (rnd.next() % block_size);
            } else {
                tmpkey = rnd.next() % tuple_num;
            }
        }

        // Operation typeの決定
        if ((rnd.next() % 100) < rraito) {
            wonly_flag = false;
            pro.emplace_back(Ope::READ, tmpkey);
        } else {
            if (rmw) {
                // 使わないけど実装だけ
                pro.emplace_back(Ope::READ_MODIFY_WRITE, tmpkey);
            } else {
                pro.emplace_back(Ope::WRITE, tmpkey);
            }
        }
    }
    // Q:?
    (*pro.begin()).ronly_ = ronly_flag;
    (*pro.begin()).wonly_ = wonly_flag;
}

