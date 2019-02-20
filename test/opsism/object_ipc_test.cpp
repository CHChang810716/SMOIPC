#include <opsism/object_ipc.hpp>
#include <gtest/gtest.h>
#include "object_ipc_test/com_trait.hpp"
#include <opsism/shm/host.hpp>
#include <boost/dll.hpp>
#include <boost/process.hpp>
TEST(opsism_object_ipc, basic_test) {
    opsism::shm::Host shm_host("object_ipc_test", 65535);
    auto queue = shm_host.find_or_construct<typename IPCProto::ObjectQueue>("queue")();
    auto producer_path = boost::dll::program_location().parent_path() / "opsism-object_ipc_test-receiver";
    boost::process::child producer(producer_path.string(), 
        boost::process::std_out > boost::process::null, 
        boost::process::std_err > boost::process::null 
    );
    boost::asio::io_service io_context;
    IPCProto::Receiver receiver(queue, io_context);
    receiver.on_received<std::string>([](auto&& str){
        EXPECT_EQ(str, "hello world");
    });
    io_context.run();
}