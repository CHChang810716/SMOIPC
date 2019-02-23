#pragma one
#include <boost/interprocess/managed_shared_memory.hpp>
#include "clean_guard.hpp"
namespace smoipc::shm {
struct Host 
: public CleanGuard
, public boost::interprocess::managed_shared_memory 
{
    Host(const std::string& shm_name, std::size_t space_bytes) 
    : CleanGuard(shm_name)
    , boost::interprocess::managed_shared_memory(
        boost::interprocess::create_only, 
        shm_name.c_str(),
        space_bytes
    )
    {
    }
};

}