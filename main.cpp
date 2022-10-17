#include <iostream>
#include <vector>
#include <thread>

#include "common.h"
#include "result.h"
#include "record.h"
#include "worker.h"
#include "logger.h"

alignas(CACHE_LINE_SIZE) std::vector<ResultLog> SiloResult;

alignas(CACHE_LINE_SIZE) Tuple *Table;


void partTableInit(int thID, int start, int end) {
    for (auto i = start; i <= end; i++) {
        Tuple *tmp;
        tmp = &Table[i];
        tmp->tidword_.epoch = 1;
        tmp->tidword_.latest = 1;
        tmp->tidword_.lock = 0;
        tmp->val_[0] = 'a';
        tmp->val_[1] = '\0';
    }
}

void makeDB() {
    if (posix_memalign((void **) &Table, PAGE_SIZE, TUPLE_NUM * sizeof(Tuple)) != 0) ERR;
    // tuple_numを割り切れる最大のThread数を求める
    auto maxthread = std::thread::hardware_concurrency();
    for (maxthread ; maxthread > 0; maxthread--) {
        if (TUPLE_NUM % maxthread == 0) break;
    }
    std::vector<std::thread> thv;
    for (size_t i = 0; i < maxthread; i++) {
        thv.emplace_back(partTableInit, i, i * (TUPLE_NUM / maxthread), (i + 1) * (TUPLE_NUM / maxthread) - 1);
    }
    for (auto &th : thv) th.join();
}

void waitForReady(const std::vector<char> &readys) {
    while (true) {
        bool failed = false;
        for (const auto &ready : readys) {
            if (!__atomic_load_n(&ready, __ATOMIC_ACQUIRE)) {
                failed = true;
            }
        }
        if (!failed) break;
    }
}

int main() {
    LoggerAffinity affin;   // Nodeごとに管理する用のやつ, numa仕様じゃないので特に気にしなくていい
    affin.init(THREAD_NUM, LOGGER_NUM);

    makeDB();   // DBの作成

    bool start = false;
    bool quit = false;

    SiloResult.resize(THREAD_NUM);  // threadの個数だけresultを格納するvectorを生成しておく
    std::vector<char> readys(THREAD_NUM);

    // TODO: workerとloggerの作成
    std::atomic<Logger *> logs[LOGGER_NUM];
    Notifier notifier;
    std::vector<std::thread> lthv;  // logger threads
    std::vector<std::thread> wthv;  // worker threads

    // set_affinityを一旦しない方針でやるので、とりあえずthreadを立てよう
    int i = 0, j = 0;
    for ( ; i < LOGGER_NUM; i++) {
        logs[i].store(0);
        lthv.emplace_back(logger_th, i, std::ref(notifier), &(logs[i]));
    }
    // worker_thの最後の引数は恐らくLog担当者にLogを渡すためのアドレス？
    for ( ; j < THREAD_NUM; j++) {
        wthv.emplace_back(worker_th, j, std::ref(readys[j]), std::ref(start), std::ref(quit), &(logs[i]));
    }

    waitForReady(readys);
    __atomic_store_n(&start, true, __ATOMIC_RELEASE);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000 * EXTIME));
    __atomic_store_n(&quit, true, __ATOMIC_RELEASE);

    for (auto &th : lthv) th.join();
    for (auto &th : wthv) th.join();

    return 0;
}