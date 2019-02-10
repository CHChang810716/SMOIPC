#pragma once
#include "buffer.hpp"
namespace opsism::stream {

template<class T, std::size_t buffered_obj_num = 1000>
struct FwdConsumer {
    using ShBuf = Buffer<T, buffered_obj_num>;

    FwdConsumer(ShBuf* buffer)
    : shared_buffer_    ( buffer )
    {}

    bool pop(T& obj) {
        return shared_buffer_->inter_.pop(obj);
    }

private:
    ShBuf*                      shared_buffer_  ;
};

template<class T, std::size_t buffered_obj_num>
auto make_fwd_consumer(Buffer<T, buffered_obj_num>* buffer) {
    return FwdConsumer<T, buffered_obj_num>(buffer);
}

}
