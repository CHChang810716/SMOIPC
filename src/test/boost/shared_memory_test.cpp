#include <gtest/gtest.h>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/asio.hpp>
#include <boost/lockfree/spsc_queue.hpp>
#include <thread>
TEST(boost_shmem, basic_test) {
    boost::posix_time::time_duration aaaa = boost::posix_time::millisec(50);
    boost::lockfree::spsc_queue<int> queue(10);
    queue.push(10);
    std::thread tr([&queue](){
        queue.push(20);
    });
    int n;
    queue.pop(n);
    std::cout << n << std::endl;
    queue.pop(n);
    std::cout << n << std::endl;
    tr.join();
}