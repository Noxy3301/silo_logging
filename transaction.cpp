#include "include/atomic_tool.h"
#include "include/log.h"
#include "include/util_silo.h"
#include "include/transaction.h"
#include "include/transaction_log.h"
#include "include/notifier.h"

TxExecutorLog::TxExecutorLog(int thid, ResultLog *sres_log) : TxExecutor(thid, &(sres_log->result_)), sres_log_(sres_log) {}

// bool TxExecutorLog::pauseCondition() {
//     auto dlepoch = __atomic_load_n(&(ThLocalDurableEpoch[logger_thid_].obj_))
// }

void TxExecutorLog::epochWork(uint64_t &epoch_timer_start, uint64_t &epoch_timer_stop) {

}

// void TxExecutorLog::durableEpochWork(uint64_t &epoch_timer_start, uint64_t &epoch_timer_stop, const bool &quit) {
//     std::uint64_t t = rdtscp();
//     // pause this worker until Durable epoch catches up
//     if (pauseCondition())
// }