#include "include/atomic_tool.h"
#include "include/util_silo.h"
#include "include/util.h"
#include "include/notifier.h"
#include "include/common.h"

// 全てのworker_thがlatest epochを読んでいるか確認する
bool chkEpochLoaded() {
    uint64_t nowepo = atomicLoadGE();
    // leader_workを実行しているのはthid:0だからforは1から回している？
    for (unsigned int i = 1; i < THREAD_NUM; i++) {
        if (__atomic_load_n(&(ThLocalEpoch[i].obj_), __ATOMIC_ACQUIRE) != nowepo) return false;
    }
    return true;
}

// 40ms経過してepoch進行に問題がないならGEをaddする
void leaderWork(uint64_t &epoch_timer_start, uint64_t &epoch_timer_stop) {
    epoch_timer_stop = rdtscp();
    if (chkClkSpan(epoch_timer_start, epoch_timer_stop, EPOCH_TIME * CLOCKS_PER_US * 1000) && chkEpochLoaded()) {
        atomicAddGE();
        epoch_timer_start = epoch_timer_stop;
    }
}