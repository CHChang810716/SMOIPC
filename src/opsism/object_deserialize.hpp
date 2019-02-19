#pragma once
#include <boost/mpl/vector.hpp>
#include <vector>
#include <cstdint>
#include <sstream>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/mpl/size.hpp>
namespace opsism {

constexpr struct ObjectDeserialize {
    template<class Type, class Func>
    void operator()(
        const std::string& buffer, Func func
     ) const {
        std::istringstream is;
        is.str(buffer);
        boost::archive::binary_iarchive bi(is);
        Type object;
        bi >> object;
        func(object);
    }
    template<class Type>
    void operator()(
        Type& object,
        const std::string& buffer 
     ) const {
        std::istringstream is;
        is.str(buffer);
        boost::archive::binary_iarchive bi(is);
        bi >> object;
    }
} object_deserialize;

}