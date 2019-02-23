#include <smoipc/mpl/type_list.hpp>
#include <gtest/gtest.h>

TEST(smoipc_mpl, type_list) {
    using List = smoipc::mpl::TypeList<int, char, float, double, const char*>;
    EXPECT_EQ((smoipc::mpl::Find<float, List>::value), 2);
    EXPECT_EQ((smoipc::mpl::Find<const char*, List>::value), 4);
    EXPECT_EQ((smoipc::mpl::Find<int, List>::value), 0);
}