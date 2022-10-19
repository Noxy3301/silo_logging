#pragma once

#include <string>
#include <iostream>

// TODO:名前しか実装してないぞ！
class File {
    private:
        int fd_;
        bool autoClose_;

        void throwOpenError(const std::string &filePath) const {}   // 実装しなくていいかも？
    
    public:
        File() : fd_(-1), autoClose_(false) {}

        // bool open(const std::string &filePath, int flags) {
        //     fd_ = ::open(filePath.c_str(), flags);
        //     autoClose_ = true;
        //     return fd_ >= 0;
        // }
};