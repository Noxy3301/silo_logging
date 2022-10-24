#include <iostream>
#include <vector>
#include <thread>

#define GLOBAL_VALUE_DEFINE

#include "include/atomic_tool.h"
#include "include/atomic_wrapper.h"
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


std::mutex m;



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
        // m.lock(); std::cout << "running_" << thid << std::endl; m.unlock();
        if (thid == 0) leaderWork(epoch_timer_start, epoch_timer_stop);
        trans.durableEpochWork(epoch_timer_start, epoch_timer_stop, quit);
        
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
            // m.lock(); std::cout << "commit!!!!!!!!!!!" << thid << std::endl; m.unlock();
            storeRelease(myres.local_commit_counts_, loadAcquire(myres.local_commit_counts_) + 1);
        } else {
            trans.abort();
            myres.local_abort_counts_++;
            goto RETRY;
        }
    }

    trans.log_buffer_pool_.terminate(myres_log);
    logger->worker_end(thid);

    return;
}



void logger_th(int thid, Notifier &notifier, std::atomic<Logger*> *logp) {
    alignas(CACHE_LINE_SIZE) Logger logger(thid, notifier);
    notifier.add_logger(&logger);
    logp->store(&logger);
    logger.worker();
    return;
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
    LoggerAffinity affin;   // Nodeごとに管理する用のやつ
    affin.init(THREAD_NUM, LOGGER_NUM);

    if (posix_memalign((void **) &ThLocalEpoch, CACHE_LINE_SIZE, THREAD_NUM * sizeof(uint64_t_64byte)) != 0) ERR;
    if (posix_memalign((void **) &CTIDW, CACHE_LINE_SIZE, THREAD_NUM * sizeof(uint64_t_64byte)) != 0) ERR;

    for (unsigned int i = 0; i < THREAD_NUM; i++) {
        ThLocalEpoch[i].obj_ = 0;
        // CTIDW[i].obj_ = 0;
        CTIDW[i].obj_ = ~(uint64_t)0;   // util->util_logでCTIDWに対して2回初期化を行っているから後者を採用しておく
    }

    if (posix_memalign((void **)&ThLocalDurableEpoch, CACHE_LINE_SIZE, LOGGER_NUM * sizeof(uint64_t_64byte)) != 0) ERR;


    for (unsigned int i = 0; i < LOGGER_NUM; i++) {
        ThLocalDurableEpoch[i].obj_ = 0;
    }
    DurableEpoch.obj_ = 0;

    makeDB();   // DBの作成

    bool start = false;
    bool quit = false;

    SiloResult.resize(THREAD_NUM);  // threadの個数だけresultを格納するvectorを生成しておく
    std::vector<char> readys(THREAD_NUM);

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

    // m.lock(); std::cout << "owari!" << std::endl; m.unlock();

    for (auto &th : lthv) th.join();
    // m.lock(); std::cout << "kokoka??" << std::endl; m.unlock();
    for (auto &th : wthv) th.join();

    m.lock(); std::cout << "owari!!" << std::endl; m.unlock();

    for (unsigned int i = 0; i < THREAD_NUM; i++) {
        SiloResult[0].addLocalAllResult(SiloResult[i]);
    }
    SiloResult[0].displayAllResult(CLOCKS_PER_US, EXTIME, THREAD_NUM);
    // notifier.display();

    return 0;
}