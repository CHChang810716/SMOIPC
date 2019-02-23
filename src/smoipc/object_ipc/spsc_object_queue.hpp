#pragma once
#include <smoipc/stream/spsc_queue.hpp>
#include <cstddef>
namespace smoipc::object_ipc {

template<std::size_t buffer_bytes = 1024>
struct SpscObjectQueue 
{

/* friend section */
template<class MplVector, std::size_t bb>
friend struct Sender;

template<class MplVector, std::size_t bb>
friend struct Receiver;

/* struct content */
    using This      = SpscObjectQueue<buffer_bytes>;
    using BinQueue  = stream::SpscQueue<char, buffer_bytes>; // NOTE: std::stream::char_type?
    using Element   = typename BinQueue::Element;
private:
    bool pop(Element& obj) {
        consumer_last_pop_ = std::chrono::system_clock::now();
        return bin_queue_->pop(obj);
    }
    bool push(const Element& obj) {
        producer_last_push_ = std::chrono::system_clock::now();
        return bin_queue_->push(obj);
    }
    void require_reset_if_need(
        const std::chrono::system_clock::time_point& my_tp, bool& wait_initial
    ) {
        if( my_tp != std::chrono::system_clock::time_point() ) {
            // current worker is dead and resume, raise the reset flag;
            bool require_queue_reset = require_reset_.load();
            if(require_queue_reset) return; // already raised, leave
            wait_initial = require_reset_.compare_exchange_strong(
                require_queue_reset, true
            );
        }
        wait_initial = false;
    }
    bool reset_if_required(bool& wait_initial) {
        if(wait_initial) {
            if(require_reset_.load()) {
                return false;
            } else {
                wait_initial = false;
                return true;
            }
        } else {
            if(require_reset_.load()) {
                bin_queue_.reset();
                require_reset_.store(false);
                return true;
            } else {
                return true;
            }
        }

    }
    

    BinQueue                                bin_queue_                 ;
    std::chrono::system_clock::time_point   producer_last_push_        ;
    std::chrono::system_clock::time_point   consumer_last_pop_         ;
    std::atomic<bool>                       require_reset_     {false} ;
};

}