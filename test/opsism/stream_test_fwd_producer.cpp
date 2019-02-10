#include <opsism/stream/fwd_producer.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
using ShmIntBuffer = opsism::stream::Buffer<int>;
int main() {
    boost::interprocess::managed_shared_memory segment(
        boost::interprocess::open_or_create, "opsism_fwd_real_ipc_shm_test", 65536
    );
    auto queue = segment.find_or_construct<ShmIntBuffer>("queue")();
    auto producer = opsism::stream::make_fwd_producer(queue);
    producer.push(12);
    producer.push(13);
    producer.push(17);
}