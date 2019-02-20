#pragma once
#include "object_ipc/receiver.hpp"
#include "object_ipc/sender.hpp"
namespace opsism {
template<class... Types>
using TypeList = boost::mpl::vector<Types...>;

template<class Trait>
struct ObjectIPC {
    constexpr static auto buffer_bytes = Trait::buffer_bytes;
    using Receiver = opsism::object_ipc::Receiver<
        typename Trait::ComTypes, buffer_bytes 
    >;
    using Sender = opsism::object_ipc::Sender<
        typename Trait::ComTypes, buffer_bytes
    >;
    using ObjectQueue = object_ipc::SpscObjectQueue<buffer_bytes>;
};
#include "object_ipc/spsc_object_queue.hpp"

}