#pragma once

#include "transaction.h"
#include "log_buffer.h"
#include "notifier.h"

class TxExecutorLog : public TxExecutor {
    public:
        ResultLog *sres_log_;
        LogBufferPool log_buffer_pool_;
        int logger_thid_;

        TxExecutorLog(int thid, ResultLog *sres_log);

        void begin() override;
        void wal(std::uint64_t ctid) override;  // 使わないかも
        bool pauseCondition();
        void epochWork(uint64_t &epoch_timer_start, uint64_t &epoch_timer_stop);
        void durableEpochWork(uint64_t &epoch_timer_start, uint64_t &epoch_timer_stop, const bool &quit);
};