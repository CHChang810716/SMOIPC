#pragma one
#include <boost/interprocess/shared_memory_object.hpp>
#include <string>
namespace smoipc::shm {
struct CleanGuard {
    CleanGuard(const std::string& shm_name) 
    : shm_name_(shm_name)
    {
        boost::interprocess::shared_memory_object::remove(shm_name_.c_str());
    }
    ~CleanGuard() {
        boost::interprocess::shared_memory_object::remove(shm_name_.c_str());
    }
private:
    std::string shm_name_;
};
}