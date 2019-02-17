#pragma once
#include <opsism/stream/fwd_producer.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/find.hpp>
#include <sstream>
#include <boost/archive/binary_oarchive.hpp>
namespace opsism::object_ipc {

template<class MplVector>
struct Sender {};

template<class... T>
struct Sender<boost::mpl::vector<T...>> {
    using Type = boost::mpl::vector<T...>;
    Sender(
        ShBuf*                                    buffer, 
        boost::asio::io_service&                  ios
    )
    : ios_            (&ios)
    , basic_producer_ (buffer)
    {}
    template<class Obj>
    void async_send(Obj&& obj) {
        using Itr = typename boost::mpl::find<Type, Obj>::type;
        std::ostringstream os;
        boost::archive::binary_oarchive oa(os);
        oa << Itr::pos::value;
        oa << obj;
        auto b_data = os.str();
        for(auto&& c : b_data) {
        }

    }
protected:
    void async_send_impl(std::size_t type_id, std::string b_data) {

    }
    boost::asio::io_service ios_;
    FwdProducer<char> basic_producer_;
};

}