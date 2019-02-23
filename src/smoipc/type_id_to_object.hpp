#pragma once
#include <vector>
#include <cstdint>
#include <sstream>
#include <boost/archive/binary_iarchive.hpp>
#include <smoipc/mpl/type_list.hpp>
namespace smoipc {
template<class MplVector>
struct TypeIdToObject {
};

template<std::uint16_t stid, class Type, class... Types>
struct TypeIdToObjectImpl {
    template<class Func>
    static void run(
        std::uint16_t tid, 
        Func func
    ) {
        if(tid == stid) {
            Type object;
            func(object);
        } else {
            TypeIdToObjectImpl<stid + 1, Types...>::run(
                std::move(tid), std::move(func)
            );
        }
    }
};
// template<std::uint16_t stid, class... Types>
// struct TypeIdToObjectImpl<stid, boost::mpl::na, Types...> {
//     template<class Func>
//     static void run(
//         std::uint16_t tid, 
//         Func func
//     ) {
//         throw std::runtime_error(
//             "tid: " + std::to_string(tid) + " is not in type list"
//         );
//     }
// };
template<std::uint16_t stid, class Type>
struct TypeIdToObjectImpl<stid, Type> {
    template<class Func>
    static void run(
        std::uint16_t tid, 
        Func func
    ) {
        if(tid == stid) {
            Type object;
            func(object);
        } else {
            throw std::runtime_error(
                "tid: " + std::to_string(tid) + " is not in type list"
            );
        }
    }
};

template<class... Types>
struct TypeIdToObject<mpl::TypeList<Types...>> {
    using MplVector = mpl::TypeList<Types...>;
    template<class Func>
    static void run(
        std::uint16_t tid, 
        Func func
    ) {
        TypeIdToObjectImpl<0, Types...>::run(
            tid, std::move(func)
        );
    }
};

}