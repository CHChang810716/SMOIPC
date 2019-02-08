#pragma once
#include <thread>
#include"fwd_consumer.hpp"
namespace opsism::stream {

template<class T, class Executor>
struct LocalBufferedConsumer {
    using ShBuf = Buffer<T>;
    LocalBufferedConsumer(ShBuf* buffer)
    : shared_buffer_        (buffer)
    , local_buffer_         ()
    , worker_               ()
    {}


private:
    ShBuf*      shared_buffer_  ;

    boost::lockfree::spsc_queue<
        T, boost::lockfree::capacity<100>
    >           local_buffer_   ;

    Executor    worker_         ;
};


}