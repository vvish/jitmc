#include "jitmc/arch/x86_64/x86_generic_binary_op.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace test
{

DEFINE_OP(test_op_reg, jit::x86_64::generic_binary_op_reg_reg<0x00>);

constexpr auto reg_src8 = jit::cpu_reg<8, 1>{};
constexpr auto reg_dst8 = jit::cpu_reg<8, 2>{};

constexpr auto reg_src16 = jit::cpu_reg<16, 1>{};
constexpr auto reg_dst16 = jit::cpu_reg<16, 2>{};

constexpr auto reg_src32 = jit::cpu_reg<32, 1>{};
constexpr auto reg_dst32 = jit::cpu_reg<32, 2>{};

constexpr auto reg_src64 = jit::cpu_reg<64, 1>{};
constexpr auto reg_dst64 = jit::cpu_reg<64, 2>{};

TEST(Test_x86_binary_op, register_addressing)
{
    EXPECT_THAT(
        test_op_reg(reg_src8, reg_dst8), testing::ElementsAre(0x00, 0xca));

    EXPECT_THAT(
        test_op_reg(reg_src16, reg_dst16),
        testing::ElementsAre(0x66, 0x01, 0xca));

    EXPECT_THAT(
        test_op_reg(reg_src32, reg_dst32), testing::ElementsAre(0x01, 0xca));

    EXPECT_THAT(
        test_op_reg(reg_src64, reg_dst64),
        testing::ElementsAre(0x48, 0x01, 0xca));
}

DEFINE_OP(test_op_imm, jit::x86_64::generic_binary_op_imm_reg<0x00, 0x00>);

TEST(Test_x86_binary_op, 8_bit_immediate_and_register)
{
    uint8_t const imm = 7;

    EXPECT_THAT(
        test_op_imm(imm, reg_dst8), testing::ElementsAre(0x80, 0xc2, 0x07));

    EXPECT_THAT(
        test_op_imm(imm, reg_dst16),
        testing::ElementsAre(0x66, 0x83, 0xc2, 0x07));

    EXPECT_THAT(
        test_op_imm(imm, reg_dst32), testing::ElementsAre(0x83, 0xc2, 0x07));

    EXPECT_THAT(
        test_op_imm(imm, reg_dst64),
        testing::ElementsAre(0x48, 0x83, 0xc2, 0x07));
}

DEFINE_OP(
    test_op_extra_opcode_imm, jit::x86_64::generic_binary_op_imm_reg<0x00, 0x05>);

TEST(Test_x86_binary_op_extra_opcode, 8_bit_immediate_and_register)
{
    uint8_t const imm = 7;

    EXPECT_THAT(
        test_op_extra_opcode_imm(imm, reg_dst8),
        testing::ElementsAre(0x80, 0xea, 0x07));

    EXPECT_THAT(
        test_op_extra_opcode_imm(imm, reg_dst16),
        testing::ElementsAre(0x66, 0x83, 0xea, 0x07));

    EXPECT_THAT(
        test_op_extra_opcode_imm(imm, reg_dst32),
        testing::ElementsAre(0x83, 0xea, 0x07));

    EXPECT_THAT(
        test_op_extra_opcode_imm(imm, reg_dst64),
        testing::ElementsAre(0x48, 0x83, 0xea, 0x07));
}

}  // namespace test
