#pragma once

#include <vector>
#include <unordered_set>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "common.h"
#include "tsc.h"    // TODO:いる？

class Logger;

class Notifier {
    private:
        std::thread thread_;
        std::mutex mutex_;
        std::condition_variable cv_enq_;
        std::condition_variable cv_deq_;
        std::condition_variable cv_finish_;
        std::size_t capa_ = 100000000;
        std::size_t push_size_ = 0;
        std::size_t max_buffers_ = 0;
        std::size_t byte_count_ = 0;
        std::size_t write_count_ = 0;
        std::size_t buffer_count_ = 0;

        std::uint64_t wait_latency_ = 0;
        std::uint64_t write_latency_ = 0;
        std::uint64_t write_start_ = ~(uint64_t)0;
        std::uint64_t write_end_ = 0;
        std::size_t try_count_ = 0;
        double throughput_ = 0;
        std::uint64_t start_clock_;
        std::vector<std::vector<std::uint64_t>> epoch_log_;
        bool quit_ = false;
        bool joined_ = false;
        std::unordered_set<Logger*> logger_set_;
        // Stats depoch_diff_;      // Stats未実装なので
        // NidStats nid_stats_;     // NidStats未実装なので
    
    public:
        // TODO: 型未実装
        // PepochFile pepoch_file_;
        // NidBuffer buffer_;
        // NotifyStats notify_stats_;
        std::vector<std::array<std::uint64_t, 6>> *latency_log_;    //uint64_tが6個で1セットのやつがvectorに入っている

        Notifier() {
            latency_log_ = new std::vector<std::array<std::uint64_t, 6>>;
            latency_log_->reserve(65536);   // 事前にメモリを確保しておく
            start_clock_ = rdtscp();
            // pepoch_file_.open(); // TODO:pepoch未実装
        }

        ~Notifier() {
            delete latency_log_;
        }

        // TODO:その他の関数は未実装

};