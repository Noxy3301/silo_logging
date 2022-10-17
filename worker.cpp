#include "logger.h"

void worker_th(int thID, char &ready, const bool &start, const bool &quit, std::atomic<Logger*> *logp) {
    // workerにlogger渡しているのいまいち腑に落ちんな
    // ResultLg &myres_lg = std::ref(SiloResult[thid]);
    // Result &myres = std::ref(myres_lg.result_);
    // Xoroshiro128Plus rnd;
    // rnd.init();

    std::cout << "Hi, my thID is " << thID << std::endl;

    __atomic_store_n(&ready, 1, __ATOMIC_RELEASE);
    while (true) {
        if (__atomic_load_n(&start, __ATOMIC_ACQUIRE)) break;
    }

    // epoch timer周りの処理

    // quitがtrueになるまで処理するよ
    while (true) {
        if (__atomic_load_n(&quit, __ATOMIC_ACQUIRE)) break;
    }
    
}
