#pragma once

#include <memory>

#include "debug.h"

// op_element

template<typename T>
class OpElement {
    public:
        uint64_t key_;
        T *rcdptr_; // received_pointer?

        OpElement() : key_(0), rcdptr_(nullptr) {}
        OpElement(uint64_t key) : key_(key) {}
        OpElement(uint64_t key, T *rcdptr) : key_(key), rcdptr_(rcdptr) {}
};

// silo_op_element

template<typename T>
class ReadElement : public OpElement<T> {
    public:
        using OpElement<T>::OpElement;

        ReadElement(uint64_t key, T *rcdptr, char *val, TIDword tidword) : OpElement<T>::OpElement(key, rcdptr) {
            tidword_.obj_ = tidword.obj_;
            memcpy(this->val_, val, VAL_SIZE);
        }

        bool operator < (const ReadElement &right) const {  // オーバーロード演算子の意味わからんちんちん
            return this->key_ < right.key_;
        }
        
        TIDword get_tidword() {
            return tidword_;
        }
    
    private:
        TIDword tidword_;
        char val_[VAL_SIZE];
};

template<typename T>
class WriteElement : public OpElement<T> {
    public:
        using OpElement<T>::OpElement;

        WriteElement(uint64_t key, T *rcdptr, std::string_view val) : OpElement<T>::OpElement(key, rcdptr) {
            static_assert(std::string_view("").size() == 0, "Expected behavior was broken.");   // A:処理系によっては空文字列が0にならないのがあるかもしれないからそれをはじくためらしい
            if (val.size() != 0) {
                val_ptr_ = std::make_unique<char[]>(val.size());    // A:writeで書き込むvalueのデータを格納する領域を確保しているらしい
                memcpy(val_ptr_.get(), val.data(), val.size());
                val_length_ = val.size();
            } else {
                val_length_ = 0;
            }
        }

        bool operator < (const WriteElement &right) const {
            return this->key_ < right.key_;
        }

        char *get_val_ptr() {
            return val_ptr_.get();
        }

        std::size_t get_val_length() {
            return val_length_;
        }

    private:
        std::unique_ptr<char[]> val_ptr_; // NOLINT
        std::size_t val_length_{};
};