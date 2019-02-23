#pragma once
#include <cstdint>
#include <smoipc/object_ipc.hpp>
#include <boost/serialization/string.hpp>
struct Person {
    std::string name;
    std::int16_t id;
};
struct ComTrait {
    constexpr static std::size_t buffer_bytes = 1024;
    using ComTypes = smoipc::TypeList<
        std::string,
        std::int64_t
        // Person,
        // std::tuple<int, char, std::string>
    >;
};
using IPCProto = smoipc::ObjectIPC<ComTrait>;