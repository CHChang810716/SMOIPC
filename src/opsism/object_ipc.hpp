#pragma once
#include "object_ipc/receiver.hpp"
#include "object_ipc/sender.hpp"
namespace opsism {
template<class... Types>
using TypeList = boost::mpl::vector<Types...>;

template<class Trait>
struct ObjectIPC {
    constexpr static auto buffer_bytes = Traits::buffer_bytes;
    using Receiver = opsism::object_ipc::Receiver<
        Trait::ComTypes, buffer_bytes 
    >;
    using Sender = opsism::object_ipc::Sender<
        Trait::ComTypes, buffer_bytes
    >;
};

}