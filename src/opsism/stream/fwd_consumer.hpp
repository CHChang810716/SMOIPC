#pragma once
#include "buffer.hpp"
namespace opsism::stream {

template<class T>
struct FwdConsumer {
    using ShBuf = Buffer<T>;

    Consumer(ShBuf* buffer)
    : shared_buffer_    ( buffer )
    {}

    bool pop(T& obj) {
        return shared_buffer_->inter_.pop(obj);
    }

private:
    ShBuf*                      shared_buffer_  ;
};

}
