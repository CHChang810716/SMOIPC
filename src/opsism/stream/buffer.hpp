#pragma once
#include <boost/circular_buffer.hpp>
#include <opsism/allocator.hpp>
#include <boost/lockfree/spsc_queue.hpp>
namespace opsism::stream {

template<class T>
struct Buffer {
    friend struct FwdConsumer<T>;
    friend struct FwdProducer<T>;
protected:
    boost::lockfree::spsc_queue<
        T, boost::lockfree::capacity<1000>
    > inter_;
};

}