#pragma once
#include <opsism/stream/fwd_producer.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/find.hpp>
#include <sstream>
#include <boost/archive/binary_oarchive.hpp>
#include "spsc_object_queue.hpp"
#include <opsism/utils/integer_serialize.hpp>
namespace opsism::object_ipc {

template<class MplVector, std::size_t buffer_bytes>
struct Sender {};

template<class... T, std::size_t buffer_bytes>
struct Sender<boost::mpl::vector<T...>, buffer_bytes> {
    using Type          = boost::mpl::vector<T...>;
    using ObjectQueue   = SpscObjectQueue<buffer_bytes>;
    using BinQueue      = typename ObjectQueue::BinQueue;
    Sender(
        ObjectQueue*                              buffer, 
        boost::asio::io_service&                  ios
    )
    : ios_            (ios)
    , basic_producer_ (buffer->bin_queue_)
    , object_queue_   (buffer)
    {

    }
    template<class Obj>
    void async_send(Obj&& obj) {
        using Itr = typename boost::mpl::find<Type, Obj>::type;
        std::ostringstream os;
        boost::archive::binary_oarchive oa(os);
        std::uint16_t type_id = Itr::pos::value;
        oa << type_id;
        oa << obj;
        auto b_data = os.str();
        async_send_impl(b_data);
    }
protected:
    void send_byte(const char& c) {
        while(!basic_producer_.push(c)) {
            ios_.poll_one();
        }
        // TODO: timeout detection
    }
    void async_send_impl(std::string b_data) {
        // make sure object queue is clean
        while(object_queue_->send_object_size_ > 0) {
            send_byte(0);
        }
        send_byte(char(0));

        // start send object
        std::size_t tid_object_size = b_data.size();
        auto tid_object_size_bin = utils::integer_serialize(tid_object_size);
        // TODO: set shm: send object size
        for(auto&& c : tid_object_size_bin) {
            send_byte(c);
        }
        for(auto&& c : b_data) {
            send_byte(c);
        }
        send_byte(char(1));
    }
    boost::asio::io_service&    ios_                ;
    FwdProducer<BinQueue>       basic_producer_     ;
    ObjectQueue*                object_queue_       ;
};

}