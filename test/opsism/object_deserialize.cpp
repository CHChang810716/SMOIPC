#include <opsism/object_deserialize.hpp>
#include <gtest/gtest.h>
#include <boost/archive/binary_oarchive.hpp>
#include <iostream>
#include <string>
TEST(opsism_object_deserialize, basic_test) {
    // using Types = boost::mpl::vector< 
    //     std::size_t, char, std::string 
    // >;
    std::stringstream ss;
    boost::archive::binary_oarchive bo(ss);
    bo << std::string("hello world");

    // opsism::BinToObject<Types>::run(2, ss.str(), [](auto&& obj){
    //     // std::cout << obj << std::endl;
    // });
    opsism::object_deserialize.operator()<std::string>(ss.str(), [](auto&& object){
        EXPECT_EQ(object, "hello world");
    });
}