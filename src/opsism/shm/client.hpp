#pragma once
#include <boost/interprocess/managed_shared_memory.hpp>
namespace opsism::shm {
struct Client 
: public boost::interprocess::managed_shared_memory 
{
    Client(const std::string& shm_name, std::size_t space_bytes) 
    : boost::interprocess::managed_shared_memory(
        boost::interprocess::open_or_create, 
        shm_name.c_str(),
        space_bytes
    )
    {
    }
};

}