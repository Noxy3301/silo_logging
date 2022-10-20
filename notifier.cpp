// TODO: 作る

#include <iostream>

#include "include/notifier.h"

void Notifier::logger_end(Logger *logger) {
    std::unique_lock<std::mutex> lock(mutex_);
    // nid_stats_.add();    実装予定なし？
    
    // analyze関連っぽいから実装しなくてもいいかも？

    // std::cout << "unchiburi" << logger << std::endl;

    logger_set_.erase(logger);
}