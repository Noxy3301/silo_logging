#include "include/result.h"

#include "include/cache_line_size.h"

void ResultLog::displayAllResult(size_t clocks_per_us, size_t extime, size_t thread_num) {
    result_.displayAllResult(clocks_per_us, extime, thread_num);
    //TODO : その他の結果は興味ないので未実装
}


void ResultLog::addLocalAllResult(const ResultLog &other) {
    result_.addLocalAllResult(other.result_);
    //TODO : その他の結果は興味ないので未実装
}