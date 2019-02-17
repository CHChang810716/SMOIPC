#pragma once
#include <opsism/stream/spsc_queue.hpp>
#include <cstddef>
namespace opsism::object_ipc {

template<std::size_t buffer_bytes = 1024>
struct SpscObjectQueue 
{

/* friend section */
template<class MplVector, std::size_t buffer_bytes>
friend struct Sender;

template<class MplVector, std::size_t buffer_bytes>
friend struct Receiver;

/* struct content */
    using This      = SpscQueue<buffer_bytes>;
    using BinQueue  = stream::SpscQueue<char, buffer_bytes>; // NOTE: std::stream::char_type?
private:
    bool pop(Elememt& obj) {
        consumer_last_pop = std::chrono::system_clock::now();
        return bin_queue_->pop(obj);
    }
    bool push(const Element& obj) {
        producer_last_push = std::chrono::system_clock::now();
        return bin_queue_->push(obj);
    }
    BinQueue                                bin_queue_                 ;
    std::size_t                             recv_object_size_   {0}    ;
    std::size_t                             send_object_size_   {0}    ;
    std::chrono::system_clock::time_point   producer_last_push         ;
    std::chrono::system_clock::time_point   consumer_last_pop          ;
};

}