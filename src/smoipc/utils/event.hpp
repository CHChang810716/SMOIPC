#pragma once
#include <boost/asio.hpp>
#include <vector>
#include <functional>
namespace smoipc::utils {

template<class T>
struct Event
{
    Event(boost::asio::io_service& ios)
    : ios_ (&ios)
    {}

    void signal(T&& obj) {
        ios_->post([
            this, tmp = std::move(obj)
        ]() mutable {
            slot_(tmp);            
        });
    }
    template<class Func>
    void set_slot(Func&& func) {
        slot_ = std::forward<Func>(func);
    }

private:
    boost::asio::io_service*    ios_    ;
    std::function<void(T&)>     slot_   ;
};

}
