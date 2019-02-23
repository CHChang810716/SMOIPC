#pragma once
#include <tuple>
#include <boost/asio.hpp>
#include <smoipc/tuple/fill_construct.hpp>
#include <smoipc/utils/event.hpp>
#include <smoipc/stream/fwd_consumer.hpp>
#include <smoipc/utils/integer_serialize.hpp>
#include <smoipc/utils/tick_event.hpp>
#include <smoipc/type_id_to_object.hpp>
#include "spsc_object_queue.hpp"
#include <smoipc/mpl/type_list.hpp>
namespace smoipc::object_ipc {

template<class... T>
using ReceiverBase = std::tuple<utils::Event<T>...>;

template<class MplVector, std::size_t buffer_bytes>
struct Receiver {};

template<class... T, std::size_t buffer_bytes>
struct Receiver<mpl::TypeList<T...>, buffer_bytes> 
: public ReceiverBase<T...>
{
    using Type          = mpl::TypeList<T...>;
    using Base          = ReceiverBase<T...>;
    using ObjectQueue   = SpscObjectQueue<buffer_bytes>;
    using BinQueue      = typename ObjectQueue::BinQueue;
    using This          = Receiver<Type, buffer_bytes>;

    Receiver(
        ObjectQueue*                              buffer, 
        boost::asio::io_service&                  ios, 
        const boost::posix_time::time_duration&   intvl = boost::posix_time::millisec(50)
    )
    : Base            (tuple::FillConstruct<Base>::run(ios))
    , ios_            (ios)
    , basic_consumer_ (&(buffer->bin_queue_))
    , object_queue_   (buffer)
    , wait_initial_   (false)
    , tick_event_     (ios, TickHandler(this), intvl)
    {
        object_queue_->require_reset_if_need(
            object_queue_->consumer_last_pop_,
            wait_initial_
        );
        tick_event_.start();
    }

    template<class Obj, class Func>
    void on_received(Func&& func) {
        std::get<mpl::Find<std::decay_t<Obj>, Type>::value>(*this).set_slot(
            std::forward<Func>(func)
        );
    }
protected:
    template<class Obj>
    void receive(Obj&& obj) {
        std::get<mpl::Find<std::decay_t<Obj>, Type>::value>(*this).signal(std::move(obj));
    }
    void recv_byte(char& c) {
        while(!basic_consumer_.pop(c)) {
            ios_.poll_one();
        }
    }
    void async_recv_impl() {
        std::vector<char> tid_object_size_bin(sizeof(std::size_t));
        for(auto& c : tid_object_size_bin){
            recv_byte(c);
        }
        auto tid_object_size 
            = utils::integer_deserialize.operator()<std::size_t>(tid_object_size_bin);
        std::string b_data;
        b_data.resize(tid_object_size);
        for(auto& c : b_data) {
            recv_byte(c);
        }
        char check_good;
        recv_byte(check_good);
        if(check_good == 1) {
            std::istringstream is;
            is.str(b_data);
            boost::archive::binary_iarchive bi(is);
            std::uint16_t tid;
            bi >> tid;
            TypeIdToObject<Type>::run(tid, [&bi, this](auto&& obj){
                bi >> obj;
                receive(std::move(obj));
            });
        }
    }
    struct TickHandler {
        TickHandler(This* inst) 
        : inst_(inst)
        {}
        void operator()(const boost::system::error_code& ec) const { // tick pop
            // make sure object queue is good
            if(!inst_->object_queue_->reset_if_required(inst_->wait_initial_)) {
                // no need async recursive call, because receiver has tick timer.
                return;
            }
            if(inst_->object_queue_->bin_queue_.read_available() > 0) {
                inst_->async_recv_impl();
            }
        }
        This* inst_;
    };
    boost::asio::io_service&            ios_                ;
    stream::FwdConsumer<BinQueue>       basic_consumer_     ;
    ObjectQueue*                        object_queue_       ;
    bool                                wait_initial_       ;

    utils::TickEvent<TickHandler>       tick_event_         ;
};

}