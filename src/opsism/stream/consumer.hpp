#pragma once
#include <string>
#include <boost/circular_buffer.hpp>
#include "buffer.hpp"
namespace opsism::stream {

template<class T, template<class T> class Transmitter>
struct Consumer {
    using ShBuf = Buffer<T>;
    Consumer(ShBuf* buffer)
    : shared_buffer_(buffer)
    , worker_       ([](){
        return internal_consume();
    })
    {}

    T consume() {
        return transmitter_.get_one();
    }

private:
    T internal_consume() {
        auto obj = std::move(shared_buffer_->inter_.front());
        shared_buffer_->inter_.pop_front();
        return obj;
    }
    ShBuf*                      shared_buffer_  ;
    Transmitter<T>              transmitter_    ;
};

}
