#pragma once

#include <cstdint>

enum class Ope : uint8_t {
    READ,
    WRITE,
    READ_MODIFY_WRITE,
};

class Procedure {
    public:
        Ope ope_;
        uint64_t key_;
        bool ronly_ = false;    // read only flag
        bool wonly_ = false;    // write only flag

        Procedure() : ope_(Ope::READ), key_(0) {}
        Procedure(Ope ope, uint64_t key) : ope_(ope), key_(key) {}

        bool operator < (const Procedure &right) const {
            if (this->key_ == right.key_ && this->ope_ == Ope::WRITE && right.ope_ == Ope::READ) {
                return true;
            } else if (this->key_ == right.key_ && this->ope_ == Ope::WRITE && right.ope_ == Ope::WRITE) {
                return true;
            }
            // Q:解説希望, operatorのオーバーロード演算子もよくわからん
            // 同一キーなら先にwriteを実行したい, read->writeよりwrite->read
            // 同一キーで自分がreadでここまで来たら、下記の式で絶対にfalseになり、自分(read)が昇順で後ろ回しになるのでOKらしい

            // A:Procedureをソートするときに使っているらしい(故にsilo単体の実装では使わない)
            // ただ、write->readの順が良いのかは結局不明

            return this->key_ < right.key_;
        }
};