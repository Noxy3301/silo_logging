#include "include/logger.h"

// Nodeにloggerとworkerを担うCPU(コア)を振り分ける
// Nodeに均等にCPUを割り振って、thread_num > num_cpusなら最後のworkerがloggerを兼任、そうじゃないならworkerの最後のやつをloggerに転職
void LoggerAffinity::init(unsigned worker_num, unsigned logger_num) {
    unsigned num_cpus = std::thread::hardware_concurrency();
    if (logger_num > num_cpus || worker_num > num_cpus) {
        std::cout << "too many threads" << std::endl;
    }
    // LoggerAffinityのworker_numとlogger_numにコピー
    worker_num_ = worker_num;
    logger_num_ = logger_num;
    for (unsigned i = 0; i < logger_num; i++) {
        nodes_.emplace_back();
    }
    unsigned thread_num = logger_num + worker_num;
    if (thread_num > num_cpus) {
        for (unsigned i = 0; i < worker_num; i++) {
            nodes_[i * logger_num/worker_num].worker_cpu_.emplace_back(i);
        }
        for (unsigned i = 0; i < logger_num; i++) {
            nodes_[i].logger_cpu_ = nodes_[i].worker_cpu_.back();
        }
    } else {
        for (unsigned i = 0; i < thread_num; i++) {
            nodes_[i * logger_num/thread_num].worker_cpu_.emplace_back(i);
        }
        for (unsigned i = 0; i < logger_num; i++) {
            nodes_[i].logger_cpu_ = nodes_[i].worker_cpu_.back();
            nodes_[i].worker_cpu_.pop_back();
        }
    }
}

void Logger::add_tx_executor(TxExecutorLog &trans) {
    trans.logger_thid_ = thid_;
    LogBufferPool &pool = std::ref(trans.log_buffer_pool_);
    pool.queue_ = &queue_;
    std::lock_guard<std::mutex> lock(mutex_);
    thid_vec_.emplace_back(trans.thid_);
    thid_set_.emplace(trans.thid_);
}

void Logger::worker_end(int thid) {
    std::unique_lock<std::mutex> lock(mutex_);
    thid_set_.erase(thid);
    if (thid_set_.empty()) {
        queue_.terminate();
    }
    cv_finish_.wait(lock, [this]{return joined_;});     // Q:?
}