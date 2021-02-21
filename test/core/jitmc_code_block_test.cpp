#include "jitmc/arch/x86_64/x86_byte_placement_strategy.hpp"
#include "jitmc/core/code_block.hpp"
#include "jitmc/target/containers/stl_based/associative_container.hpp"
#include "jitmc/target/containers/stl_based/code_container.hpp"
#include "jitmc/target/containers/stl_based/linear_container.hpp"
#include "jitmc/target/posix/exec_mem_allocator.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace test
{
struct allocator_mock {
    MOCK_METHOD0(make_memory_executable, void());
};
struct code_container {
    using value_type = uint8_t;

    template <typename It>
    void append(It const &begin, It const &end)
    {
        _code.insert(_code.end(), begin, end);
    }

    nonstd::span<uint8_t> get_code()
    {
        return _code;
    }

    MOCK_METHOD0(get_allocator, allocator_mock &());

    std::vector<value_type> _code;
};

struct containers {
    code_container code;
    jit::stl_based::linear_container<jit::label_descriptor> labels;
    jit::stl_based::associative_container<uint32_t, jit::patch_location>
        patches;
    jit::x86_64::x86_byte_placement_strategy placement_strategy;
};

TEST(Test_CodeBlock, make_code_block)
{
    constexpr size_t code_size = 9;
    constexpr size_t patches_count = 1;
    constexpr size_t labels_count = 1;

    std::array<uint8_t, 3> const code1{0x0, 0x1, 0x2};
    jit::patchable_code<4> const patch{0, 0, 4, {0x3, 0x4, 0x5, 0x6}};
    std::array<uint8_t, 2> const code2{0x7, 0x8};

    jit::label_t<uint8_t> const label{0};

    jit::raw_code_block_t<code_size, patches_count, labels_count> const code
        = jit::make_code(code1, patch, code2, label);

    containers deps;
    jit::code_block_t code_block(deps);

    code_block << code;

    allocator_mock allocator;
    EXPECT_THAT(
        code_block.get_code(),
        testing::ElementsAre(0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8));

    EXPECT_CALL(deps.code, get_allocator)
        .Times(1)
        .WillOnce(testing::ReturnRef(allocator));

    EXPECT_CALL(allocator, make_memory_executable).Times(1);
    code_block.prepare();

    EXPECT_THAT(
        code_block.get_code(),
        testing::ElementsAre(0x0, 0x1, 0x2, 0x2, 0x4, 0x5, 0x6, 0x7, 0x8));
}

}  // namespace test