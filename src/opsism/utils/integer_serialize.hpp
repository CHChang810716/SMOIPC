#pragma once
#include <vector>
namespace opsism::utils {

constexpr struct IntegerSerialize {
    template<class Int>
    std::vector<char> operator()(Int n) {
        std::vector<char> res(sizeof(Int), 0);
        for(auto& c : res) {
            c = n & 0xFF;
            n = n >> 8;
        }
        return res;
    }
} integer_serialize;

}