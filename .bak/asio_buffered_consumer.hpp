#pragma once
#include "fwd_consumer.hpp"
#include "buffer.hpp"
#include <boost/asio.hpp>
#include <memory>
namespace opsism::stream {

template<class T, std::size_t sh_buffered_obj_num = 1000, std::size_t loc_buffered_obj_num = 1000>
struct ASIOBufferedConsumer {
    using This   = ASIOBufferedConsumer<T, sh_buffered_obj_num, loc_buffered_obj_num>;
    using ShBuf  = Buffer<T>;
    using LocBuf = boost::lockfree::spsc_queue<
        T, boost::lockfree::capacity<loc_buffered_obj_num>
    >; 

    ASIOBufferedConsumer(
        ShBuf*                                    buffer, 
        boost::asio::io_service&                  ios, 
        const boost::posix_time::time_duration&   intvl = boost::posix_time::millisec(50)
    )
    : tick_handler_         (this)
    , shared_buffer_        (buffer)
    , basic_consumer_       (buffer)
    , local_buffer_         (new LocBuf())
    , ios_                  (&ios)
    , tick_pop_timer_       (new boost::asio::deadline_timer(ios))
    , tick_interval_        (intvl)
    {
        tick_pop_timer_->expires_from_now(tick_interval_);
        tick_pop_timer_->async_wait(tick_handler_);
    }

    bool pop(T& obj) {
        return local_buffer_->pop(obj);
    }
private:
    struct TickHandler {
        TickHandler(This* inst) 
        : inst_(inst)
        {}
        void operator()(const boost::system::error_code& ec) { // tick pop
            T obj;
            if( inst_->basic_consumer_.pop(obj)) {
                inst_->local_buffer_->push(obj);
            }
            inst_->tick_pop_timer_->expires_from_now(inst_->tick_interval_);
            inst_->tick_pop_timer_->async_wait(*this);
        }
    private:
        This* inst_;
    }                                   tick_handler_   ;
    ShBuf*                              shared_buffer_  ;
    FwdConsumer<T>                      basic_consumer_ ;
    std::unique_ptr<LocBuf>             local_buffer_   ;
    boost::asio::io_service*            ios_            ;
    std::unique_ptr<
        boost::asio::deadline_timer
    >                                   tick_pop_timer_ ;
    boost::posix_time::time_duration    tick_interval_  ;
};

template<class T, std::size_t sh_buffered_obj_num, std::size_t loc_buffered_obj_num = 1000>
auto make_asio_buffered_consumer(
    Buffer<T, sh_buffered_obj_num>*             buffer,
    boost::asio::io_service&                    ios,
    const boost::posix_time::time_duration&     intvl 
        = boost::posix_time::millisec(50),
    boost::mpl::size_t<loc_buffered_obj_num>    loc_buf_size 
        = boost::mpl::size_t<1000>()
) {
    return ASIOBufferedConsumer<T, sh_buffered_obj_num, loc_buffered_obj_num>(buffer, ios, intvl);
}

}