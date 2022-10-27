#pragma once

#define INLINE __attribute__((always_inline)) inline

#include <stdint.h>

#include "debug.h"
#include "procedure.h"
#include "random.h"
#include "result.h"
#include "tsc.h"
#include "zipf.h"

// TODO:コピペなのでいるか精査する
class LibcError : public std::exception {
private:
  std::string str_;

  static std::string generateMessage(int errnum, const std::string &msg) {
    std::string s(msg);
    const size_t BUF_SIZE = 1024;
    char buf[BUF_SIZE];
    ::snprintf(buf, 1024, " %d ", errnum);
    s += buf;
#ifdef Linux
    if (::strerror_r(errnum, buf, BUF_SIZE) != nullptr)
#endif  // Linux
#ifdef Darwin
    if (::strerror_r(errnum, buf, BUF_SIZE) != 0)
#endif  // Darwin
    s += buf;
    return s;
  }

public:
  explicit LibcError(int errnum = errno, const std::string &msg = "libc_error:")
          : str_(generateMessage(errnum, msg)) {}
};

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
    bool ronly_flag(true), wonly_flag(true);
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
            ronly_flag = false;
            if (rmw) {
                // 使わないけど実装だけ
                pro.emplace_back(Ope::READ_MODIFY_WRITE, tmpkey);
            } else {
                pro.emplace_back(Ope::WRITE, tmpkey);
            }
        }
    }
    // A:TicTocとかCicadaとかで使う用、siloでは使ていない
    (*pro.begin()).ronly_ = ronly_flag;
    (*pro.begin()).wonly_ = wonly_flag;
}

