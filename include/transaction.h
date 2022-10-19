#pragma once

#include <cstdint>
#include <string_view>
#include <vector>

#include "fileio.h"
#include "procedure.h"
#include "result.h"
// #include "string.h"  使わないかも
#include "common.h"
#include "log.h"
#include "silo_op_element.h"
#include "tuple.h"


enum class TransactionStatus : uint8_t {
    InFlight,
    Committed,
    Aborted,
};

class TxExecutor {
    public:
        std::vector<ReadElement<Tuple>> read_set_;
        std::vector<WriteElement<Tuple>> write_set_;
        std::vector<Procedure> pro_set_;
        
        std::vector<LogRecord> log_set_;
        LogHeader latest_log_header_;

        TransactionStatus status_;
        unsigned int thid_;

        Result *sres_;
        File logfile_;

        TIDword mrctid_;
        TIDword max_rset_, max_wset_;

        char write_val_[VAL_SIZE];
        char return_val_[VAL_SIZE];

        TxExecutor(int thid, Result *sres);

        void abort();
        virtual void begin();
        void tx_delete(uint64_t key);
        void displayWriteSet();
        Tuple *get_tuple(Tuple *table, uint64_t key) { return &table[key]; }
        void insert();  // 使わない気がするから未定義で
        void lockWriteSet();
        void read(uint64_t key);
        ReadElement<Tuple> *searchReadSet(uint64_t key);
        WriteElement<Tuple> *searchWriteSet(uint64_t key);
        void unlockWriteSet();
        void unlockWriteSet(std::vector<WriteElement<Tuple>>::iterator end);    // Q:?
        bool validationPhase();
        virtual void wal(uint64_t ctid);    // 使わない気がする、というかナニコレ？
        // void write(uint64_t key, std::string_view val = "");    // そもそもstring_viewはSGX対応してなさそうだしつかない方が良いかも、kv形式にしたいかも
        void write(uint64_t key);   // TODO:あとでvalueを受け取るようにする
        virtual void writePhase();
};