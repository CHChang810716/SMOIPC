#pragma once
#include "buffer.hpp"
namespace opsism::stream {

template<class T>
struct FwdProducer {
    using ShBuf = Buffer<T>;

    Producer(ShBuf* buffer)
    : shared_buffer_( buffer )
    {}

    bool push(const T& obj) {
        return shared_buffer_->inter_.push(obj);
    }

private:
    ShBuf*              shared_buffer_      ;
};

}