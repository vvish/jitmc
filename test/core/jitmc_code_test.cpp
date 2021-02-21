#include "jitmc/core/jit_code.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace test
{

// test would not compile in case of error
TEST(Test_CodeBlock, make_empty_code)
{
    jit::raw_code_block_t<0, 0, 0> const code = jit::make_code();
}

TEST(Test_Code, make_code)
{
    constexpr size_t code_size = 10;
    constexpr size_t patches_count = 1;
    constexpr size_t labels_count = 1;

    jit::code_t<2> const code1{0x0, 0x1};
    jit::code_t<3> const code2{0x2, 0x3, 0x4};

    jit::patchable_code<5> const patch{0, 1, 2, {0x5, 0x6, 0x7, 0x8, 0x9}};
    jit::label_t<void> const label{0};

    jit::raw_code_block_t<code_size, patches_count, labels_count> const code
        = jit::make_code(code1, code2, patch, label);
}

TEST(Test_CodeBlock, make_code)
{
    constexpr size_t code_size = 10;
    constexpr size_t patches_count = 1;
    constexpr size_t labels_count = 1;

    constexpr size_t label_offset = 10;

    jit::code_t<2> const code1{0x0, 0x1};
    jit::code_t<3> const code2{0x2, 0x3, 0x4};

    jit::patchable_code<5> const patch{0, 1, 2, {0x5, 0x6, 0x7, 0x8, 0x9}};
    jit::label_t<void> const label{1};

    jit::raw_code_block_t<code_size, patches_count, labels_count> const code
        = jit::make_code(code1, code2, patch, label);

    EXPECT_EQ(code.bytes.size(), 10);
    EXPECT_EQ(code.patches.size(), 1);
    EXPECT_EQ(code.labels.size(), 1);

    EXPECT_THAT(
        code.bytes,
        testing::ElementsAre(0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9));

    EXPECT_EQ(code.patches[0].dependency_id, patch.dependency_id);
    EXPECT_EQ(code.patches[0].op_location.offset, 5);
    EXPECT_EQ(code.patches[0].op_location.size, 5);
    EXPECT_EQ(code.patches[0].patch_location.offset, 5 + patch.patch_offset);
    EXPECT_EQ(code.patches[0].patch_location.size, patch.patch_size);
    

    EXPECT_EQ(code.labels[0].id, label.id);
    EXPECT_EQ(code.labels[0].offset, label_offset);
}

}  // namespace test