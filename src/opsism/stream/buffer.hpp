#pragma once
#include <boost/circular_buffer.hpp>
#include <opsism/allocator.hpp>
namespace opsism::stream {

template<class T>
struct Buffer {
    friend struct Consumer<T>;
    friend struct Producer<T>;
protected:
    boost::circular_buffer<T, Allocator<T>> inter_;
};

}