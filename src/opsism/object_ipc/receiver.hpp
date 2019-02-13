#pragma once
#include <boost/asio.hpp>
#include <vector>
#include <functional>
namespace opsism::object_ipc {

template<class T>
struct Receiver
{
    Receiver(boost::asio::io_service& ios)
    : ios_ (&ios)
    {}

    void trigger(T&& obj) {
        boost::asio::post(*ios_, [
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
