#pragma once
#include "fwd_consumer.hpp"
#include "buffer.hpp"
#include <boost/asio.hpp>
namespace opsism::stream {

template<class T>
struct ASIOBufferedConsumer {
    using ShBuf = Buffer<T>;
    using LocBuf = boost::lockfree::spsc_queue<
        T, boost::lockfree::capacity<1000>
    > inter_;

    ASIOBufferedConsumer(
        ShBuf*                              buffer, 
        boost::asio::io_service&            ios, 
        boost::posix_time::time_duration&   intvl = boost::posix_time::millisec(50)
    )
    : shared_buffer_        (buffer)
    , basic_consumer_       (buffer)
    , local_buffer_         ()
    , ios_                  (&ios)
    , tick_pop_timer_       (ios)
    , tick_intervel_        (invl)
    {
        tick_pop_timer_.expires_from_now(intvl);
        tick_pop_timer_.async_wait(*this);
    }

    bool pop(T& obj) {
        return local_buffer_.pop(obj);
    }
    void operator()(const boost::system::error_code& ec) { // tick pop
        T obj;
        if( basic_consumer_.pop(obj)) {
            local_buffer_.push(obj);
        }
        tick_pop_timer_.expires_from_now(invl);
        tick_pop_timer_.async_wait(*this);
    }

private:
    ShBuf*                              shared_buffer_  ;
    FwdConsumer<T>                      basic_consumer_ ;
    LocBuf                              local_buffer_   ;
    boost::asio::io_service*            ios_            ;
    boost::asio::deadline_timer         tick_pop_timer_ ;
    boost::posix_time::time_duration    tick_intervel_  ;
};


}