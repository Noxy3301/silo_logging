#include "include/log_buffer.h"
#include "include/log_queue.h"
#include "include/notifier.h"
#include "include/result.h"

void LogBufferPool::terminate(ResultLog &myres) {
    quit_ = true;
    // TODO: logの吐き出し処理は未実装
}