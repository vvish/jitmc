#include "jitmc/target/containers/trivial/associative_container.hpp"

#include <string>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace test
{
TEST(Test_trivial_associative_container, create_empty)
{
    jit::trivial::associative_container<uint8_t, std::string, 10> container;
    EXPECT_EQ(container.size(), 0);
}

TEST(Test_trivial_associative_container, insertion_failed)
{
    jit::trivial::associative_container<uint8_t, std::string, 2> container;
    ASSERT_TRUE(container.insert(2, "two"));
    ASSERT_TRUE(container.insert(4, "four"));
    ASSERT_FALSE(container.insert(7, "seven"));
    ASSERT_FALSE(container.insert(9, "nine"));

    EXPECT_EQ(container.size(), 2);
}

TEST(Test_trivial_associative_container, insert_find)
{
    jit::trivial::associative_container<uint8_t, std::string, 10> container;
    container.insert(2, "two");
    container.insert(4, "four");
    container.insert(7, "seven");

    EXPECT_EQ(container.size(), 3);

    auto const two = container.find(2);
    ASSERT_TRUE(two);
    EXPECT_EQ(*two, "two");

    auto const four = container.find(4);
    ASSERT_TRUE(four);
    EXPECT_EQ(*four, "four");

    auto const seven = container.find(7);
    ASSERT_TRUE(seven);
    EXPECT_EQ(*seven, "seven");

    auto const five = container.find(5);
    ASSERT_FALSE(five);
}
}  // namespace test