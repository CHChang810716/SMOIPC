#include <gtest/gtest.h>
#include <smoipc/stream/fwd_producer.hpp>
#include <smoipc/stream/fwd_consumer.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/dll.hpp>
#include <boost/process.hpp>
#include <atomic>
#include <smoipc/stream/spsc_queue.hpp>
using ShmIntBuffer = smoipc::stream::SpscQueue<int>;
TEST(smoipc_stream, fwd_producer_basic_test) {
    boost::interprocess::shared_memory_object::remove("smoipc_stream_fwd_basic_test");
    boost::interprocess::managed_shared_memory segment(
        boost::interprocess::open_or_create, "smoipc_stream_fwd_basic_test", 65536
    );
    auto queue = segment.find_or_construct<ShmIntBuffer>("queue")();
    auto producer = smoipc::stream::make_fwd_producer(queue);
    EXPECT_TRUE(producer.push(12));
    EXPECT_TRUE(producer.push(13));
    EXPECT_TRUE(producer.push(17));
}
TEST(smoipc_stream, fwd_consumer_basic_test) {
    boost::interprocess::managed_shared_memory segment(
        boost::interprocess::open_or_create, "smoipc_stream_fwd_basic_test", 65536
    );
    auto queue = segment.find_or_construct<ShmIntBuffer>("queue")();
    auto consumer = smoipc::stream::make_fwd_consumer(queue);
    int n;
    EXPECT_TRUE(consumer.pop(n));
    EXPECT_EQ(n, 12);
    EXPECT_TRUE(consumer.pop(n));
    EXPECT_EQ(n, 13);
    EXPECT_TRUE(consumer.pop(n));
    EXPECT_EQ(n, 17);
    EXPECT_FALSE(consumer.pop(n));
    boost::interprocess::shared_memory_object::remove("smoipc_stream_fwd_basic_test");
}
template<class T>
auto block_pop(T& consumer) {
    int n;
    while(!consumer.pop(n)) {
        std::this_thread::sleep_for(
            std::chrono::milliseconds(1)
        );
    }
    return n;
}
TEST(smoipc_stream, fwd_real_ipc_shm_test) {
    boost::interprocess::shared_memory_object::remove("smoipc_fwd_real_ipc_shm_test");
    boost::interprocess::managed_shared_memory segment(
        boost::interprocess::open_or_create, "smoipc_fwd_real_ipc_shm_test", 65536
    );
    auto queue = segment.find_or_construct<ShmIntBuffer>("queue")();

    auto producer_path = boost::dll::program_location().parent_path() / "test-smoipc-stream_fwd_producer";
    boost::process::child producer(producer_path.string(), 
        boost::process::std_out > boost::process::null, 
        boost::process::std_err > boost::process::null 
    );

    auto consumer = smoipc::stream::make_fwd_consumer(queue);
    EXPECT_EQ(block_pop(consumer), 12);
    EXPECT_EQ(block_pop(consumer), 13);
    EXPECT_EQ(block_pop(consumer), 17);
    boost::interprocess::shared_memory_object::remove("smoipc_fwd_real_ipc_shm_test");
}