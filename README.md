# SMOIPC- Shared memory object interprocess communication
Travis CI: [![Build Status](https://travis-ci.com/CHChang810716/SMOIPC.svg?branch=master)](https://travis-ci.org/CHChang810716/SMOIPC)

AppVeyor CI: [![Build status](https://ci.appveyor.com/api/projects/status/il9sv4l0lw2ybkng/branch/master?svg=true)](https://ci.appveyor.com/project/CHChang810716/SMOIPC)

## Intro

The SMOIPC is a lockfree inter process object communication tool based on Boost interprocess library.

## Example

Producer side

```c++
#include <smoipc/stream/fwd_producer.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <smoipc/stream/spsc_queue.hpp>
using ShmIntBuffer = smoipc::stream::SpscQueue<int>;
int main() {
    // define share memory and construct communication queue
    boost::interprocess::managed_shared_memory segment(
        boost::interprocess::open_or_create, "smoipc_fwd_real_ipc_shm_test", 65536
    );
    auto queue = segment.find_or_construct<ShmIntBuffer>("queue")();
    auto producer = smoipc::stream::make_fwd_producer(queue);
    // put object to queue
    producer.push(12);
    producer.push(13);
    producer.push(17);
}

```

Consumer side

```c++
#include <smoipc/stream/fwd_consumer.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <smoipc/stream/spsc_queue.hpp>
using ShmIntBuffer = smoipc::stream::SpscQueue<int>;

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

int main() {
    // define share memory and construct communication queue

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

    // pull object from queue
    std::cout << block_pop(consumer) << std::endl; // will be 12
    std::cout << block_pop(consumer) << std::endl; // will be 13
    std::cout << block_pop(consumer) << std::endl; // will be 17

    // clean shared memory
    boost::interprocess::shared_memory_object::remove("smoipc_fwd_real_ipc_shm_test");
}

```

## Build Project

### Recommended environments

* g++ >= 7.3
* CMake >= 3.13
* Linux, Windows is tested (see CI tag)
* On Windows, mingw is recommended

### Steps

```bash
>cd SMOIPC
>mkdir build
>cd build
>cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../stage -DBUILD_TEST=ON
# build with 4 core
>cmake --build . --target install -j 4
# run test
>ctest
```

## Use Package

This project is using CMake Hunter to maintain 3rd party, but not yet register to official Hunter site.

Therefore, use Hunter submodule solution to link this package is recommended, see [here](https://hunter.readthedocs.io/en/latest/user-guides/hunter-user/git-submodule.html) for more details.
