#pragma once
#include "receiver.hpp"
#include <tuple>
#include <boost/asio.hpp>
#include <opsism/tuple/fill_construct.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/find.hpp>
namespace opsism::object_ipc {

template<class... T>
using ReceiverTableBase = std::tuple<Receiver<T>...>;

template<class MplVector>
struct ReceiverTable {};

template<class... T>
struct ReceiverTable<boost::mpl::vector<T...>> 
: public ReceiverTableBase<T...>
{
    using TypeTable = boost::mpl::vector<T...>;
    using Base = ReceiverTableBase<T...>;

    ReceiverTable(boost::asio::io_service& ios)
    : Base(tuple::FillConstruct<Base>::run(ios))
    {}

    template<class Obj, class Func>
    void on_received(Func&& func) {
        using Itr = typename boost::mpl::find<TypeTable, Obj>::type;
        std::get<Itr::pos::value>(*this).set_slot(
            std::forward<Func>(func)
        );
    }
protected:
    template<class Obj>
    void receive(Obj&& obj) {
        using Itr = typename boost::mpl::find<TypeTable, Obj>::type;
        std::get<Itr::pos::value>(*this).trigger(std::move(obj));
    }
};

}