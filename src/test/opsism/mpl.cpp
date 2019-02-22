#include <opsism/mpl/type_list.hpp>
#include <gtest/gtest.h>

TEST(opsism_mpl, type_list) {
    using List = opsism::mpl::TypeList<int, char, float, double, const char*>;
    EXPECT_EQ((opsism::mpl::Find<float, List>::value), 2);
    EXPECT_EQ((opsism::mpl::Find<const char*, List>::value), 4);
    EXPECT_EQ((opsism::mpl::Find<int, List>::value), 0);
}