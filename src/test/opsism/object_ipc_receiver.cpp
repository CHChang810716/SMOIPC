#include "object_ipc/com_trait.hpp"
#include <opsism/shm/client.hpp>
int main() {
    opsism::shm::Client shm_client("object_ipc_test", 65535);
    auto queue = shm_client.find_or_construct<typename IPCProto::ObjectQueue>("queue")();
    boost::asio::io_service io_context;
    IPCProto::Sender sender(queue, io_context);
    sender.async_send(std::string("hello world"));
    io_context.run();
}