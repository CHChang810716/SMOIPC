#pragma once
#include <vector>
namespace smoipc::utils {

constexpr struct IntegerSerialize {
    template<class Int>
    std::vector<char> operator()(Int n) const {
        std::vector<char> res(sizeof(Int), 0);
        for(auto& c : res) {
            c = n & 0xFF;
            n = n >> 8;
        }
        return res;
    }
} integer_serialize;
constexpr struct IntegerDeserialize {
    template<class Int>
    Int operator()(const std::vector<char>& bin) const {
        Int res = 0;
        for(auto itr = bin.rbegin(); itr != bin.rend(); itr ++) {
            auto& c = *itr;
            res = res << 8;
            res = res | c;
        }
        return res;
    }
} integer_deserialize;

}