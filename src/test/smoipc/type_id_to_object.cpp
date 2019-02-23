#include <boost/serialization/string.hpp>
#include <smoipc/type_id_to_object.hpp>
#include <smoipc/object_deserialize.hpp>
#include <gtest/gtest.h>
#include <boost/archive/binary_oarchive.hpp>
#include <iostream>
#include <string>
constexpr struct Visitor {
    template<class T>
    void operator()(T& obj) const {
    }
    void operator()(const std::string& obj) const {
        EXPECT_EQ("hello world", obj);
    }
} visitor;
TEST(smoipc_type_id_to_object, basic_test) {
    using Types = smoipc::mpl::TypeList< 
        std::size_t, char, std::string 
    >;
    std::stringstream ss;
    boost::archive::binary_oarchive bo(ss);
    bo << std::string("hello world");
    auto buffer = ss.str();
    smoipc::TypeIdToObject<Types>::run(2, [&buffer](auto&& obj){
        smoipc::object_deserialize(obj, buffer);
        visitor(obj);
    });
}