#include <iostream>
#include <vector>
#include <thread>

#define GLOBAL_VALUE_DEFINE

#include "include/atomic_tool.h"
#include "include/common.h"
#include "include/util_silo.h"
#include "include/result.h"
#include "include/transaction_log.h"
#include "include/logger.h"
#include "include/notifier.h"

#include "include/debug.h"
#include "include/fileio.h"
#include "include/random.h"
#include "include/tsc.h"
#include "include/util.h"
#include "include/zipf.h"

void worker_th(int thid, char &ready, const bool &start, const bool &quit, std::atomic<Logger*> *logp) {
    ResultLog &myres_log = std::ref(SiloResult[thid]);
    Result &myres = std::ref(myres_log.result_);
    Xoroshiro128Plus rnd;
    rnd.init();
    TxExecutorLog trans(thid, (ResultLog *) &myres_log);
    FastZipf zipf(&rnd, ZIPF_SKEW, TUPLE_NUM);
    uint64_t epoch_timer_start, epoch_timer_stop;

    Logger *logger;
    for (;;) {
        logger = logp->load();
        if (logger != 0) break;
        std::this_thread::sleep_for(std::chrono::nanoseconds(100));
    }
    logger->add_tx_executor(trans);
    
    std::cout << "Hi(worker_th), my thid is " << thid << std::endl;

    __atomic_store_n(&ready, 1, __ATOMIC_RELEASE);
    while (true) {
        if (__atomic_load_n(&start, __ATOMIC_ACQUIRE)) break;
    }

    // thid:0のworkerはGlobal Epochの更新も兼任するよ

    if (thid == 0) epoch_timer_start = rdtscp();
    
    // quitがtrueになるまで処理するよ
    while (true) {
        if (__atomic_load_n(&quit, __ATOMIC_ACQUIRE)) break;
        // 後ろから3番目はpartition, 使い方は知らん Q:
        // Q:っていうかycsbってなんだ？r/wの割合の話だとは思うけど
        makeProcedure(trans.pro_set_, rnd, zipf, TUPLE_NUM, MAX_OPE, THREAD_NUM, RRAITO, RMW, YCSB, false, thid, myres);
        
    RETRY:
        if (thid == 0) leaderWork(epoch_timer_start, epoch_timer_stop);
        trans.durableEpochWork(epoch_timer_start, epoch_timer_stop, quit);
        // DURABLE EPOCHの話はやらなくてもよさげ？
        if (__atomic_load_n(&quit, __ATOMIC_ACQUIRE)) break;
        
        trans.begin();
        for (auto itr = trans.pro_set_.begin(); itr != trans.pro_set_.end(); itr++) {
            if ((*itr).ope_ == Ope::READ) {
                trans.read((*itr).key_);
            } else if ((*itr).ope_ == Ope::WRITE) {
                trans.write((*itr).key_);
            } else if ((*itr).ope_ == Ope::READ_MODIFY_WRITE) {
                trans.read((*itr).key_);
                trans.write((*itr).key_);
            } else {
                ERR;
            }
        }

        if (trans.validationPhase()) {
            trans.writePhase();
            // local_commit_countsはbackoff?で使うらしいから実装しなくてもいいかも
        } else {
            trans.abort();
            // myres.local_abort_counts_++;
            goto RETRY;
        }
    }
    trans.log_buffer_pool_.terminate(myres_log);
    logger->worker_end(thid);
}



void logger_th(int thid, Notifier &notifier, std::atomic<Logger*> *logp) {
    std::cout << "Hi(logger_th), my thid is " << thid << std::endl;
}


void partTableInit(int thid, int start, int end) {
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

    // affinの構造を理解したので実装をccbenchに寄せる
    // manualでaffinityを設定する予定はないのでset_cpuはナシで
    int i = 0, j = 0;
    for (auto itr = affin.nodes_.begin(); itr != affin.nodes_.end(); itr++, j++) {
        int lcpu = itr->logger_cpu_;
        logs[j].store(0);
        lthv.emplace_back(logger_th, j, std::ref(notifier), &(logs[j]));
        for (auto wcpu = itr->worker_cpu_.begin(); wcpu != itr->worker_cpu_.end(); wcpu++, i++) {
            wthv.emplace_back(worker_th, i, std::ref(readys[i]), std::ref(start), std::ref(quit), &(logs[j]));  // logsはj番目(loggerと共通のやつ)
        }
    }

    waitForReady(readys);
    __atomic_store_n(&start, true, __ATOMIC_RELEASE);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000 * EXTIME));
    __atomic_store_n(&quit, true, __ATOMIC_RELEASE);

    for (auto &th : lthv) th.join();
    for (auto &th : wthv) th.join();

    return 0;
}