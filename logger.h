#pragma once

#include <vector>
#include <mutex>
#include <atomic>
#include <thread>
#include <condition_variable>   // 特定の条件を満たすまでthreadを待機させるためのクラス
#include <unordered_set>        // indexがないset集合
#include <map>                  // 平衡二分木
#include <stdio.h>
#include <iostream>

#include "notifier.h"
#include "common.h"

class LogBuffer;

/* * * * * * * * * * * * * * * * * * * *
	Logger_th
* * * * * * * * * * * * * * * * * * * */

void logger_th(int thID, Notifier &notifier, std::atomic<Logger*> *logp);

/* * * * * * * * * * * * * * * * * * * *
	LogBuffer
* * * * * * * * * * * * * * * * * * * */


/* * * * * * * * * * * * * * * * * * * *
    LogQueue
* * * * * * * * * * * * * * * * * * * */

class LogQueue {
    private:
        std::atomic<unsigned int> my_mutex_;
        std::mutex mutex_;
        std::condition_variable cv_deq_;
        std::map<uint64_t, std::vector<LogBuffer*>> queue_;
        std::size_t capacity_ = 1000;
        std::atomic<bool> quit_;
        std::chrono::microseconds timeout_;

        void my_lock() {
            for (;;) {
                unsigned int lock = 0;  // expectedとして使う
                if (my_mutex_.compare_exchange_strong(lock, 1)) return;
                std::this_thread::sleep_for(std::chrono::nanoseconds(30));
            }
        }

        void my_unlock() {
            my_mutex_.store(0);
        }
    
    public:
        LogQueue() {    // constructor
            my_mutex_.store(0);
            quit_.store(false);
            timeout_ = std::chrono::microseconds((int)(EPOCH_TIME*1000));
        }

        void enq(LogBuffer* x) {    //入れる方
            {
                std::lock_guard<std::mutex> lock(mutex_);   // mutex_を用いてLock, destructor時にlock解放できる
                // TODO:auto &v = queue_[x->min_epoch_]; 一旦パス
            }   // ここでmutex_のlockが解放される
        }
};

/* * * * * * * * * * * * * * * * * * * *
	Logger
* * * * * * * * * * * * * * * * * * * */

class LoggerNode {
    public:
        int logger_cpu_;
        std::vector<int> worker_cpu_;

        LoggerNode() {}
};

class LoggerAffinity {
    public:
        std::vector<LoggerNode> nodes_;
        unsigned worker_num_ = 0;
        unsigned logger_num_ = 0;
        void init(unsigned worker_num, unsigned logger_num);
};

class Logger {
    private:
        std::mutex mutex_;
        std::thread thread_;
        std::condition_variable cv_finish_;
        bool joined_ = false;
        std::size_t capacity_ = 1000;
        unsigned int counter_ = 0;

        void logging(bool quit);
        void rotate_logfile(uint64_t epoch);
        void show_result();

    public:
        int thid_;
        std::vector<int> thid_vec_;
        std::unordered_set<int> thid_set_;
        LogQueue queue_;
        // PosixWriter logfile_;    // LOGWRITER
        std::string logdir_;
        std::string logpath_;
        std::uint64_t rotate_epoch_ = 100;
        Notifier notifier_;
        Notifier &notifier_stats_;
        // NidStats nid_stats_;
        // NidBuffer nid_buffer_;

        std::size_t byte_count_ = 0;
        std::size_t write_count_ = 0;
        std::size_t buffer_count_ = 0;
        std::uint64_t wait_latency_ = 0;
        std::uint64_t write_latency_ = 0;
        std::uint64_t write_start_ = 0;
        std::uint64_t write_end_ = 0;
        // Stats depoch_diff_;

        Logger(int i, Notifier &n) : thid_(i), notifier_stats_(n) {}

        // TODO:その他の関数は未実装
};