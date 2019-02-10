#pragma once
#include "buffer.hpp"
namespace opsism::stream {

template<class T, std::size_t buffered_obj_num = 1000>
struct FwdProducer {
    using ShBuf = Buffer<T, buffered_obj_num>;

    FwdProducer(ShBuf* buffer)
    : shared_buffer_( buffer )
    {}

    bool push(const T& obj) {
        return shared_buffer_->inter_.push(obj);
    }

private:
    ShBuf*              shared_buffer_      ;
};

template<class T, std::size_t buffered_obj_num>
auto make_fwd_producer(Buffer<T, buffered_obj_num>* buffer) {
    return FwdProducer<T, buffered_obj_num>(buffer);
}

}