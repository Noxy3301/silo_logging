#pragma once

#include <atomic>
#include <mutex>
#include <condition_variable>
#include <map>
#include <thread>
#include <vector>

#include "consts.h"
#include "log_buffer.h" // TODO:いらないかも？

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
                // std::lock_guard<std::mutex> lock(mutex_);   // mutex_を用いてLock, destructor時にlock解放できる
                // TODO:auto &v = queue_[x->min_epoch_]; 一旦パス
            }   // ここでmutex_のlockが解放される
        }

        void wait_deq() {
            // TODO:
        }

        void quit() {
            // TODO:
        }
        
        std::vector<LogBuffer*> deq() {
            // TODO:
        }

        bool empty() {
            return queue_.empty();
        }

        uint64_t min_epoch() {
            if (empty()) {
                return ~(uint64_t)0;
            } else {
                return queue_.cbegin()->first;
            }
        }

        void terminate() {
            quit_.store(true);
            cv_deq_.notify_all();
        }
};