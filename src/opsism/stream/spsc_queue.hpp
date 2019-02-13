#pragma once
#include <boost/circular_buffer.hpp>
#include <boost/lockfree/spsc_queue.hpp>
namespace opsism::stream {

template<class ShBuf>
struct FwdProducer;

template<class ShBuf>
struct FwdConsumer;

template<class T, std::size_t buffered_obj_num = 1000>
struct SpscQueue 
: protected boost::lockfree::spsc_queue<
    T, boost::lockfree::capacity<buffered_obj_num>
> {
    using Element = T;
    using This = SpscQueue<T, buffered_obj_num>;
friend struct FwdProducer<This>;
friend struct FwdConsumer<This>;
    static std::size_t required_space_lowerbound() {
        return buffered_obj_num * sizeof(T);
    }
};

}