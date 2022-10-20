#include <iostream>
#include <cmath>

#include "include/result.h"

using namespace std;

void Result::displayAbortCounts() {
    cout << "abort_counts_:\t" << total_abort_counts_ << endl;
}

void Result::displayCommitCounts() {
    cout << "commit_counts_:\t" << total_commit_counts_ << endl;
}

void Result::displayTps(size_t extime, size_t thread_num) {
    uint64_t result = total_commit_counts_ / extime;
    cout << "latency[ns]:\t" << powl(10.0, 9.0) / result * thread_num << endl;
    cout << "throughput[tps]:\t" << result << endl;
}

void Result::addLocalAbortCounts(const uint64_t count) {
    total_abort_counts_ += count;
}

void Result::addLocalCommitCounts(const uint64_t count) {
    total_commit_counts_ += count;
}

void Result::displayAllResult(size_t clocks_per_us, size_t extime, size_t thread_num) {
    displayAbortCounts();
    displayCommitCounts();
    // displayRusageRUMaxrss(); //なにこれ
    // displayAbortRate();
    displayTps(extime, thread_num);

}

void Result::addLocalAllResult(const Result &other) {
    addLocalAbortCounts(other.local_abort_counts_);
    addLocalCommitCounts(other.local_commit_counts_);
}