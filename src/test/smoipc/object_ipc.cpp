#include <smoipc/object_ipc.hpp>
#include <gtest/gtest.h>
#include "object_ipc/com_trait.hpp"
#include <smoipc/shm/host.hpp>
#include <boost/dll.hpp>
#include <boost/process.hpp>
TEST(smoipc_object_ipc, basic_test) {
    smoipc::shm::Host shm_host("object_ipc_test", 65535);
    auto queue = shm_host.find_or_construct<typename IPCProto::ObjectQueue>("queue")();
    auto producer_path = boost::dll::program_location().parent_path() / "test-smoipc-object_ipc_sender";
    boost::process::child producer(producer_path.string(), 
        boost::process::std_out > boost::process::null, 
        boost::process::std_err > boost::process::null 
    );
    boost::asio::io_service io_context;
    IPCProto::Receiver receiver(queue, io_context);
    std::size_t recv_obj_num = 0;
    receiver.on_received<std::string>([&](auto&& str){
        recv_obj_num ++;
        EXPECT_EQ(str, "hello world");
    });
    receiver.on_received<std::int64_t>([&](auto&& n){
        recv_obj_num ++;
        EXPECT_EQ(n, 77777);
    });
    while(recv_obj_num < 2) {
        io_context.poll_one();
    }
}