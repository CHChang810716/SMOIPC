#pragma once
#include <tuple>
namespace opsism::tuple {
template<class Tuple>
struct FillConstruct {
    template<class Arg0, class... Args>
    static Tuple run(Arg0& arg0, Args&... args) {
        if constexpr(std::tuple_size<Tuple>::value == sizeof...(Args) + 1) {
            return Tuple(arg0, args...);
        } else {
            return (*this)(
                arg0,arg0,args...
            );
        }
    }
};
}