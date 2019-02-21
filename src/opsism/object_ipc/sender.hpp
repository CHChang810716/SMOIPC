#pragma once
#include <opsism/stream/fwd_producer.hpp>
#include <sstream>
#include <boost/archive/binary_oarchive.hpp>
#include "spsc_object_queue.hpp"
#include <opsism/utils/integer_serialize.hpp>
#include <boost/asio.hpp>
#include <boost/lockfree/queue.hpp>
#include <opsism/utils/tick_event.hpp>
#include <opsism/utils/mux_queue.hpp>
#include <opsism/mpl/type_list.hpp>
namespace opsism::object_ipc {

template<class MplVector, std::size_t buffer_bytes>
struct Sender {};

template<class... T, std::size_t buffer_bytes>
struct Sender<mpl::TypeList<T...>, buffer_bytes> {
    using Type          = mpl::TypeList<T...>;
    using ObjectQueue   = SpscObjectQueue<buffer_bytes>;
    using BinQueue      = typename ObjectQueue::BinQueue;
    using TaskQueue     = opsism::utils::MuxQueue<std::string>;
    using This          = Sender<Type, buffer_bytes>;
    Sender(
        ObjectQueue*                              buffer, 
        boost::asio::io_service&                  ios,
        const boost::posix_time::time_duration&   intvl = boost::posix_time::millisec(50)
    )
    : ios_            (ios)
    , basic_producer_ (&(buffer->bin_queue_))
    , object_queue_   (buffer)
    , wait_initial_   (false)
    , task_queue_     (new TaskQueue())
    , tick_event_     (ios, TickHandler(this), intvl)
    {
        object_queue_->require_reset_if_need(
            object_queue_->producer_last_push_,
            wait_initial_
        );
        tick_event_.start();
    }
    template<class Obj>
    void async_send(Obj&& obj) {
        std::ostringstream os;
        boost::archive::binary_oarchive oa(os);
        std::uint16_t type_id = mpl::Find<Obj, Type>::value;
        oa << type_id;
        oa << obj;
        auto b_data = os.str();
        // async_send_impl(std::move(b_data));
        task_queue_->push(std::move(b_data));
    }
protected:
    void send_byte(const char& c) {
        // TODO: check remote reset flag
        while(!basic_producer_.push(c)) {
            ios_.poll_one();
        }
        // TODO: timeout detection
    }
    void async_send_impl(std::string b_data) {
        // start send object
        std::size_t tid_object_size = b_data.size();
        auto tid_object_size_bin = utils::integer_serialize(tid_object_size);
        for(auto&& c : tid_object_size_bin) {
            send_byte(c);
        }
        for(auto&& c : b_data) {
            send_byte(c);
        }
        send_byte(char(1));
    }
    struct TickHandler          {
        TickHandler(This* t)
        : inst(t)
        {}
        void operator()() {
            // make sure object queue is good
            if(!object_queue_->reset_if_required(wait_initial_)) {
                return;
            }
            if(inst->task_queue_->pop(curr_task)) {
                inst->async_send_impl(std::move(curr_task));
                curr_task.clear();
            }
        }
        std::string curr_task;
        This* inst;
    };
    boost::asio::io_service&        ios_                ;
    stream::FwdProducer<BinQueue>   basic_producer_     ;
    ObjectQueue*                    object_queue_       ;
    bool                            wait_initial_       ;

    std::unique_ptr<TaskQueue>      task_queue_         ;
    utils::TickEvent<TickHandler>   tick_event_         ;
};

}