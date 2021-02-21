#pragma once

#include "jitmc/arch/x86_64/x86_branches.hpp"
#include "jitmc/arch/x86_64/x86_generic_binary_op.hpp"
#include "jitmc/arch/x86_64/x86_generic_unary_op.hpp"
#include "jitmc/core/operations.hpp"

namespace jit
{
namespace x86_64
{

DEFINE_OP(
    mov,
    generic_binary_op_imm_reg<0x88, 0x00>,
    generic_binary_op_reg_reg<0x88>,
    generic_binary_op_disp_reg<0x88>,
    binary_operation_traits_short_imm8<0xb0>,
    binary_operation_traits_short_imm32<0xb8>);

DEFINE_OP(
    add,
    generic_binary_op_imm_reg<0x00, 0x00>,
    generic_binary_op_reg_reg<0x00>,
    generic_binary_op_disp_reg<0x00>);

// 'not' is a C/C++ keyword
DEFINE_OP(n0t, unary_operation_traits<0xf6, 0x2>);

DEFINE_OP(jmp, branch_operation_traits);
DEFINE_OP(ret, ret_traits);

}  // namespace x86_64
}  // namespace jit