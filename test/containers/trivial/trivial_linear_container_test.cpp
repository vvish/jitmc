#include "jitmc/target/containers/trivial/linear_container.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace test
{
TEST(Test_trivial_liniar_container, create_empty)
{
    jit::trivial::linear_container<uint8_t, 10> container;
    EXPECT_EQ(container.size(), 0);
}

TEST(Test_trivial_liniar_container, append_failed)
{
    jit::trivial::linear_container<uint8_t, 2> container;
    ASSERT_TRUE(container.append(2));
    ASSERT_TRUE(container.append(4));
    ASSERT_FALSE(container.append(7));
    ASSERT_FALSE(container.append(9));

    EXPECT_EQ(container.size(), 2);
}

TEST(Test_trivial_liniar_container, append)
{
    jit::trivial::linear_container<uint8_t, 10> container;
    container.append(2);
    container.append(4);
    container.append(7);

    ASSERT_EQ(container.size(), 3);
    EXPECT_THAT(container, ::testing::ElementsAre(2, 4, 7));
}
}  // namespace test