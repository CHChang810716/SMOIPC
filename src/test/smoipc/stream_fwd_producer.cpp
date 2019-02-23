#include <smoipc/stream/fwd_producer.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <smoipc/stream/spsc_queue.hpp>
using ShmIntBuffer = smoipc::stream::SpscQueue<int>;
int main() {
    boost::interprocess::managed_shared_memory segment(
        boost::interprocess::open_or_create, "smoipc_fwd_real_ipc_shm_test", 65536
    );
    auto queue = segment.find_or_construct<ShmIntBuffer>("queue")();
    auto producer = smoipc::stream::make_fwd_producer(queue);
    producer.push(12);
    producer.push(13);
    producer.push(17);
}