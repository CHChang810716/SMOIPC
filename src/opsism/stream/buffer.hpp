#pragma once
#include <boost/circular_buffer.hpp>
#include <boost/lockfree/spsc_queue.hpp>
namespace opsism::stream {

template<class T, std::size_t buffered_obj_num>
struct FwdProducer;

template<class T, std::size_t buffered_obj_num>
struct FwdConsumer;

template<class T, std::size_t buffered_obj_num = 1000>
struct Buffer {
    friend struct FwdProducer<T, buffered_obj_num>;
    friend struct FwdConsumer<T, buffered_obj_num>;
    static std::size_t required_space_lowerbound() {
        return buffered_obj_num * sizeof(T);
    }
protected:
    boost::lockfree::spsc_queue<
        T, boost::lockfree::capacity<buffered_obj_num>
    > inter_;
};

}