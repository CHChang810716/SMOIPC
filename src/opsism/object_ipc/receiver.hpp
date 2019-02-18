#pragma once
#include <tuple>
#include <boost/asio.hpp>
#include <opsism/tuple/fill_construct.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/find.hpp>
#include <opsism/utils/event.hpp>
#include <opsism/stream/fwd_consumer.hpp>
#include <opsism/utils/integer_serialize.hpp>
#include <opsism/utils/tick_event.hpp>
namespace opsism::object_ipc {

template<class... T>
using ReceiverBase = std::tuple<utils::Event<T>...>;

template<class MplVector, std::size_t buffer_bytes>
struct Receiver {};

template<class... T, std::size_t buffer_bytes>
struct Receiver<boost::mpl::vector<T...>, buffer_bytes> 
: public ReceiverBase<T...>
{
    using Type          = boost::mpl::vector<T...>;
    using Base          = ReceiverBase<T...>;
    using ObjectQueue   = SpscObjectQueue<buffer_bytes>;
    using BinQueue      = typename ObjectQueue::BinQueue;

    Receiver(
        ObjectQueue*                              buffer, 
        boost::asio::io_service&                  ios, 
        const boost::posix_time::time_duration&   intvl = boost::posix_time::millisec(50)
    )
    : Base            (tuple::FillConstruct<Base>::run(ios))
    , ios_            (ios)
    , basic_consumer_ (buffer->bin_queue_)
    , object_queue_   (buffer)
    , wait_initial_   (false)
    , tick_event_     (ios, TickHandler(this), intvl)
    {
        object_queue_->require_reset_if_need(
            object_queue_->consumer_last_pop_,
            wait_initial_
        );
        tick_pop_timer_->expires_from_now(tick_interval_);
        tick_pop_timer_->async_wait(tick_handler_);
    }

    template<class Obj, class Func>
    void on_received(Func&& func) {
        using Itr = typename boost::mpl::find<Type, Obj>::type;
        std::get<Itr::pos::value>(*this).set_slot(
            std::forward<Func>(func)
        );
    }
protected:
    template<class Obj>
    void receive(Obj&& obj) {
        using Itr = typename boost::mpl::find<Type, Obj>::type;
        std::get<Itr::pos::value>(*this).signal(std::move(obj));
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
            = utils::integer_deserialize<std::size_t>(tid_object_size_bin);
        std::vector<char> tid_bin(sizeof(std::uint16_t));
        std::vector<char> object_bin(tid_object_size - tid_bin.size());
        for(auto& c : tid_bin){
            recv_byte(c);
        }
        for(auto& c : object_bin){
            recv_byte(c);
        }
        // TODO: doing object translate

    }
    struct TickHandler {
        TickHandler(This* inst) 
        : inst_(inst)
        {}
        void operator()(const boost::system::error_code& ec) { // tick pop
            // make sure object queue is good
            if(!object_queue_->reset_if_required(wait_initial_)) {
                // no need async recursive call, because receiver has tick timer.
                return;
            }
            if(object_queue_->bin_queue_.read_available() > 0) {
                inst_->async_recv_impl();
            }
        }
        This* inst_;
    };
    boost::asio::io_service&            ios_                ;
    FwdConsumer<BinQueue>               basic_consumer_     ;
    ObjectQueue*                        object_queue_       ;
    bool                                wait_initial_       ;

    utils::TickEvent<TickHandler>       tick_event_         ;
};

}