#pragma once
#include <cstdint>
#include <opsism/object_ipc.hpp>
#include <boost/serialization/string.hpp>
struct Person {
    std::string name;
    std::int16_t id;
};
struct ComTrait {
    constexpr static std::size_t buffer_bytes = 1024;
    using ComTypes = opsism::TypeList<
        std::string,
        std::int64_t
        // Person,
        // std::tuple<int, char, std::string>
    >;
};
using IPCProto = opsism::ObjectIPC<ComTrait>;