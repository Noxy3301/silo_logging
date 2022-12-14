#pragma once

#include "transaction.h"
#include "log_buffer.h"
#include "notifier.h"

class TxExecutorLog : public TxExecutor {
    public:
        ResultLog *sres_lg_;
        LogBufferPool log_buffer_pool_;
        NotificationId nid_;
        std::uint32_t nid_counter_ = 0; // Notification ID
        int logger_thid_;

        TxExecutorLog(int thid, ResultLog *sres_lg);
        void begin() override;
        void wal(std::uint64_t ctid) override;
        bool pauseCondition();
        void epochWork(uint64_t &epoch_timer_start, uint64_t &epoch_timer_stop);
        void durableEpochWork(uint64_t &epoch_timer_start, uint64_t &epoch_timer_stop, const bool &quit);
};