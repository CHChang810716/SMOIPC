#pragma once
#include <boost/asio.hpp>
#include <memory>
namespace opsism::utils {

template<class Func>
struct TickEvent {
    TickEvent(
        boost::asio::io_service& ios, 
        Func func,
        const boost::posix_time::time_duration& intvl = boost::posix_time::millisec(50)
    )
    : func_             (std::move(func))
    , ios_              (ios)
    , tick_pop_timer_   (new boost::asio::deadline_timer(ios))
    , tick_interval_    (intvl)
    {
    }
    void start() {
        tick_pop_timer_->expires_from_now(tick_interval_);
        tick_pop_timer_->async_wait(*this);
    }
    void operator()(const boost::system::error_code& ec) {
        func_(ec);
        tick_pop_timer_->expires_from_now(tick_interval_);
        tick_pop_timer_->async_wait(*this);
    }
private:
    boost::asio::io_service&            ios_;
    Func                                func_;
    std::shared_ptr<
        boost::asio::deadline_timer
    >                                   tick_pop_timer_     ;
    boost::posix_time::time_duration    tick_interval_      ;
};

template<class Func>
auto make_tick_event(
    boost::asio::io_service& ios, 
    Func func, 
    const boost::posix_time::time_duration& intvl = boost::posix_time::millisec(50)
) {
    return TickEvent<Func>(ios, std::move(func), intvl);
}

}