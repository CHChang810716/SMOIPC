#pragma once
#include <cstdint>
#include <type_traits>
namespace smoipc::mpl {

template<class... Types>
struct TypeList{};

template<class Type0, class... Types>
struct TypeList<Type0, Types...>{
    using Head = Type0;
    using Tail = TypeList<Types...>;
};
template<>
struct TypeList<>{
};

// template<class List, template< class T > class Func>
// struct TransformImpl {};
// template<class... Types, template< class T > class Func>
// struct TransformImpl<TypeList<Types...>, Func> {
//     using Result = TypeList<Func<Types>...>;
// };
// template<class List, template< class T > class Func>
// using Transform = typename TransformImpl<List, Func>::Result;


template<class Type, class List>
struct Find{
    constexpr static std::size_t value = []() constexpr {
        if constexpr(std::is_same_v<Type, typename List::Head>) {
            return 0;
        } else {
            return 1 + Find<Type, typename List::Tail>::value;
        }
    }();

};

}