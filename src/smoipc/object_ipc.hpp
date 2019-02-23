#pragma once
#include "object_ipc/receiver.hpp"
#include "object_ipc/sender.hpp"
namespace smoipc {
template<class... Types>
using TypeList = mpl::TypeList<Types...>;

template<class Trait>
struct ObjectIPC {
    constexpr static auto buffer_bytes = Trait::buffer_bytes;
    using Receiver = smoipc::object_ipc::Receiver<
        typename Trait::ComTypes, buffer_bytes 
    >;
    using Sender = smoipc::object_ipc::Sender<
        typename Trait::ComTypes, buffer_bytes
    >;
    using ObjectQueue = object_ipc::SpscObjectQueue<buffer_bytes>;
};
#include "object_ipc/spsc_object_queue.hpp"

}