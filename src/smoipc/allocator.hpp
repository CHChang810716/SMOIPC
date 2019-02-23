#pragma once
#include <boost/interprocess/segment_manager.hpp>
namespace smoipc {

template<class T>
using Allocator = boost::interprocess::allocator<
    T, boost::interprocess::managed_shared_memory::segment_manager
>;

}