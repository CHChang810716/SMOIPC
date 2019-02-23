#include <smoipc/tuple/fill_construct.hpp>
#include <gtest/gtest.h>

TEST(tuple_test, fill_construct) {
    using TupleFillConst = smoipc::tuple::FillConstruct<std::tuple<
        int, std::vector<char>, char 
    >>;
    int n = 1;
    auto tuple = TupleFillConst::run(n);
    EXPECT_EQ(std::get<0>(tuple), 1);
    EXPECT_EQ(std::get<1>(tuple).size(), 1);
    EXPECT_EQ(std::get<2>(tuple), char(1));
}