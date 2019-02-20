#pragma once
#include <mutex>
#include <queue>
namespace opsism::utils {

template<class T>
struct MuxQueue {
    bool push(T&& obj) {
        std::lock_guard<std::mutex> _lock_file(mux_);
        core_.push(std::move(obj));
        return true;
    }
    bool pop(T& obj) {
        if(core_.empty()) return false;
        std::lock_guard<std::mutex> _lock_file(mux_);
        obj = std::move(core_.front());
        core_.pop();
        return true;
    }
private:
    std::queue<T> core_ ;
    std::mutex    mux_  ;
};

}