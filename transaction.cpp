#include <algorithm>
#include <unistd.h>

// #define DEBUG

#ifdef DEBUG
#include <bitset>
#endif

#include "include/atomic_tool.h"
#include "include/log.h"
#include "include/util_silo.h"
#include "include/transaction.h"
#include "include/transaction_log.h"
#include "include/notifier.h"
#include "include/string.h"
#include "include/atomic_wrapper.h"

#include "include/fileio.h"

TxExecutor::TxExecutor(int thid, Result *sres) : thid_(thid), sres_(sres) {
    read_set_.reserve(MAX_OPE);
    write_set_.reserve(MAX_OPE);
    pro_set_.reserve(MAX_OPE);

    max_rset_.obj_ = 0;
    max_wset_.obj_ = 0;

    genStringRepeatedNumber(write_val_, VAL_SIZE, thid);
}

void TxExecutor::abort() {
    read_set_.clear();
    write_set_.clear();
}

void TxExecutor::begin() {
    status_ = TransactionStatus::InFlight;
    max_wset_.obj_ = 0;
    max_rset_.obj_ = 0;
}

void TxExecutor::lockWriteSet() {
    TIDword expected, desired;
    for (auto itr = write_set_.begin(); itr != write_set_.end(); itr++) {
        expected.obj_ = loadAcquire((*itr).rcdptr_->tidword_.obj_);
        for (;;) {
            if (expected.lock) {
                this->status_ = TransactionStatus::Aborted;     // w-w conflict時は即Abort
                if (itr != write_set_.begin()) unlockWriteSet(itr); // Q:ここでUnlockし始める理由は？abort時にUnlockするクネ？
                return;
            } else {
                desired = expected;
                desired.lock = 1;
                if (compareExchange((*itr).rcdptr_->tidword_.obj_, expected.obj_, desired.obj_)) break; // lock取得をCASでやっている？
            }
        }
        max_wset_ = std::max(max_wset_, expected);
    }
}

void TxExecutor::read(std::uint64_t key) {
    TIDword expected, check;

    if (searchReadSet(key) || searchWriteSet(key)) goto FINISH_READ;

    Tuple *tuple;
    tuple = get_tuple(Table, key);

    #ifdef DEBUG
        std::cout << "["
            << "epo:" << tuple->tidword_.epoch << " "
            << "tid:" << tuple->tidword_.TID << " "
            << "abs:" << tuple->tidword_.absent << " lat:" << tuple->tidword_.latest << " loc:" << tuple->tidword_.lock << "] "
        << std::endl;
    #endif

    //(a) reads the TID word, spinning until the lock is clear
    expected.obj_ = loadAcquire(tuple->tidword_.obj_);  // 一回読まないとね
    // lockが解放されるまでspin wait
    for (;;) {
        while (expected.lock) {
            expected.obj_ = loadAcquire(tuple->tidword_.obj_);
        }
        //(b) checks whether the record is the latest version
        // omit. because this is implemented by single version

        //(c) reads the data
        memcpy(return_val_, tuple->val_, VAL_SIZE);

        //(d) performs a memory fence
        // don't need.
        // order of load don't exchange.    // Q:解説募

        //(e) checks the TID word again
        check.obj_ = loadAcquire(tuple->tidword_.obj_);
        if (expected == check) break;
        expected = check;
    }
    read_set_.emplace_back(key, tuple, return_val_, expected);
    
    FINISH_READ:
    return;
}

ReadElement<Tuple> *TxExecutor::searchReadSet(std::uint64_t key) {
    for (auto itr = read_set_.begin(); itr != read_set_.end(); itr++) {
        if ((*itr).key_ == key) return &(*itr);
    }
    return nullptr;
}

WriteElement<Tuple> *TxExecutor::searchWriteSet(std::uint64_t key) {
    for (auto itr = write_set_.begin(); itr != write_set_.end(); itr++) {
        if ((*itr).key_ == key) return &(*itr);
    }
    return nullptr;
}

void TxExecutor::unlockWriteSet() {
    TIDword expected, desired;
    for (auto itr = write_set_.begin(); itr != write_set_.end(); itr++) {
        expected.obj_ = loadAcquire((*itr).rcdptr_->tidword_.obj_);
        desired = expected;
        desired.lock = 0;
        storeRelease((*itr).rcdptr_->tidword_.obj_, desired.obj_);
    }
}

// end(任意)のところまでunlockするってこと？
void TxExecutor::unlockWriteSet(std::vector<WriteElement<Tuple>>::iterator end) {
    TIDword expected, desired;
    for (auto itr = write_set_.begin(); itr != end; itr++) {
        expected.obj_ = loadAcquire((*itr).rcdptr_->tidword_.obj_);
        desired = expected;
        desired.lock = 0;
        storeRelease((*itr).rcdptr_->tidword_.obj_, desired.obj_);
    }
}

bool TxExecutor::validationPhase() {
    // Phase1
    // lock write_set_ sorted

    sort(write_set_.begin(), write_set_.end());
    lockWriteSet();
    if (this->status_ == TransactionStatus::Aborted) return false;  // ccbenchのdefaultでw-w conflict時は即abortになっていたので

    asm volatile("":: : "memory");
    atomicStoreThLocalEpoch(thid_, atomicLoadGE());
    asm volatile("":: : "memory");

    #ifdef DEBUG
        std::cout << "read_set_.len = " << read_set_.size() << std::endl;
        for (auto itr = read_set_.begin(); itr != read_set_.end(); itr++) {
            std::cout << "["
                << "epo:" << (*itr).get_tidword().epoch << " "
                << "tid:" << (*itr).get_tidword().TID << " "
                << "abs:" << (*itr).get_tidword().absent << " lat:" << (*itr).get_tidword().latest << " loc:" << (*itr).get_tidword().lock << "] " << (*itr).rcdptr_->tidword_.obj_
            << std::endl;
        }
    #endif

    // Phase2 
    // Read validation
    TIDword check;
    for (auto itr = read_set_.begin(); itr != read_set_.end(); itr++) {
        // 1. tid of read_set_ changed from it that was got in Read Phase.
        check.obj_ = loadAcquire((*itr).rcdptr_->tidword_.obj_);
        if ((*itr).get_tidword().epoch != check.epoch || (*itr).get_tidword().TID != check.TID) {
            this->status_ = TransactionStatus::Aborted;
            unlockWriteSet();
            return false;
        }
        // 2. "omit" if (!check.latest) return false;
        
        // 3. the tuple is locked and it isn't included by its write set.
        if (check.lock && !searchWriteSet((*itr).key_)) {
            this->status_ = TransactionStatus::Aborted;
            unlockWriteSet();
            return false;
        }
        max_rset_ = std::max(max_rset_, check); // TID算出用
    }

    // goto Phase 3
    this->status_ = TransactionStatus::Committed;
    return true;
}

void TxExecutor::wal(std::uint64_t ctid) {
    for (auto itr = write_set_.begin(); itr != write_set_.end(); itr++) {
        LogRecord log(ctid, (*itr).key_, write_val_);
        log_set_.emplace_back(log);
        latest_log_header_.chkSum_ += log.computeChkSum();
        latest_log_header_.logRecNum_++;
    }

    if (log_set_.size() > LOGSET_SIZE / 2) {    // log吐き出さないと
        // prepare write header
        latest_log_header_.convertChkSumIntoComplementOnTwo();
        // write header
        printf("head\n");
        logfile_.write((void *) &latest_log_header_, sizeof(LogHeader));
        // write log record
        printf("lg\n");
        logfile_.write((void *) &(log_set_[0]), sizeof(LogRecord) * latest_log_header_.logRecNum_);

        latest_log_header_.init();
        log_set_.clear();
    }
}

void TxExecutor::write(std::uint64_t key, std::string_view val) {
    if (searchWriteSet(key)) goto FINISH_WRITE;
    // search tuple from data structure
    Tuple *tuple;
    ReadElement<Tuple> *re;
    re = searchReadSet(key);
    if (re) {   // Q:?
        tuple = re->rcdptr_;
    } else {
        tuple = get_tuple(Table, key);
    }
    write_set_.emplace_back(key, tuple, val);

    FINISH_WRITE:
    return;
}

void TxExecutor::writePhase() {
    // TID算出
    TIDword tid_a, tid_b, tid_c;

    // r/wの最大TIDより1大きい
    tid_a = std::max(max_wset_, max_rset_);
    tid_a.TID++;
    // workerが発行したTIDより1大きい
    tid_b = mrctid_;
    tid_b.TID++;
    // 上位32bitがnowのepoch
    tid_c.epoch = ThLocalEpoch[thid_].obj_;

    TIDword maxtid = std::max({tid_a, tid_b, tid_c});
    maxtid.lock = 0;
    maxtid.latest = 1;
    mrctid_ = maxtid;

    wal(maxtid.obj_);

    // write [record, commit_tid]
    for (auto itr = write_set_.begin(); itr != write_set_.end(); itr++) {
        // update and unlock
        if ((*itr).get_val_length() == 0) {
            memcpy((*itr).rcdptr_->val_, write_val_, VAL_SIZE); // benchmark用のfast approachらしいけど、知らん
        } else {
            memcpy((*itr).rcdptr_->val_, (*itr).get_val_ptr(), (*itr).get_val_length());    // Q:内容理解してない
        }
        storeRelease((*itr).rcdptr_->tidword_.obj_, maxtid.obj_);
    }

    read_set_.clear();
    write_set_.clear();
}




TxExecutorLog::TxExecutorLog(int thid, ResultLog *sres_lg) : TxExecutor(thid, &(sres_lg->result_)), sres_lg_(sres_lg) {}

void TxExecutorLog::begin() {
    TxExecutor::begin();
    nid_ = NotificationId(nid_counter_++, thid_, rdtscp());
}

void TxExecutorLog::wal(std::uint64_t ctid) {
    TIDword old_tid;
    TIDword new_tid;
    old_tid.obj_ = loadAcquire(CTIDW[thid_].obj_);
    new_tid.obj_ = ctid;
    bool new_epoch_begins = (old_tid.epoch != new_tid.epoch);
    log_buffer_pool_.push(ctid, nid_, write_set_, write_val_, new_epoch_begins);
    if (new_epoch_begins) {
      // store CTIDW
      __atomic_store_n(&(CTIDW[thid_].obj_), ctid, __ATOMIC_RELEASE);
    }
}


bool TxExecutorLog::pauseCondition() {
    auto dlepoch = loadAcquire(ThLocalDurableEpoch[logger_thid_].obj_);
    return loadAcquire(ThLocalEpoch[thid_].obj_) > dlepoch + EPOCH_DIFF;
}

void TxExecutorLog::epochWork(uint64_t &epoch_timer_start, uint64_t &epoch_timer_stop) {
    usleep(1);
    if (thid_ == 0) leaderWork(epoch_timer_start, epoch_timer_stop);
    TIDword old_tid;
    old_tid.obj_ = loadAcquire(CTIDW[thid_].obj_);
    // load GE
    atomicStoreThLocalEpoch(thid_, atomicLoadGE());
    uint64_t new_epoch = loadAcquire(ThLocalEpoch[thid_].obj_);
    if (old_tid.epoch != new_epoch) {
        TIDword tid;
        tid.epoch = new_epoch;
        tid.lock = 0;
        tid.latest = 1;
        // store CTIDW
        __atomic_store_n(&(CTIDW[thid_].obj_), tid.obj_, __ATOMIC_RELEASE);
    }
}

// TODO:コピペなので要確認
void TxExecutorLog::durableEpochWork(uint64_t &epoch_timer_start,
                                   uint64_t &epoch_timer_stop, const bool &quit) {
  std::uint64_t t = rdtscp();
  // pause this worker until Durable epoch catches up
  if (EPOCH_DIFF > 0) {
    if (pauseCondition()) {
      log_buffer_pool_.publish();
      do {
        epochWork(epoch_timer_start, epoch_timer_stop);
        if (loadAcquire(quit)) return;
      } while (pauseCondition());
    }
  }
  while (!log_buffer_pool_.is_ready()) {
    epochWork(epoch_timer_start, epoch_timer_stop);
    if (loadAcquire(quit)) return;
  }
  if (log_buffer_pool_.current_buffer_==NULL) std::abort();
  sres_lg_->local_wait_depoch_latency_ += rdtscp() - t;
}
