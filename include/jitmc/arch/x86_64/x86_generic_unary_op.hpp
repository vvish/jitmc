#pragma once

#include "jitmc/arch/x86_64/x86_common.hpp"
#include "jitmc/arch/x86_64/x86_registers.hpp"
#include "jitmc/core/jit_code.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

namespace jit
{
namespace x86_64
{

template <std::uint8_t Width, std::uint8_t CodeDest>
constexpr std::uint8_t make_mod_reg_rm(
    cpu_reg<Width, CodeDest> const &, uint8_t ex_opcode = 0)
{
    return std::uint8_t{
        static_cast<uint8_t>(addressing_mode::register_addressing) << 6
        | ex_opcode << 3 | CodeDest};
};

template <uint8_t OpCode, uint8_t ExCode>
struct unary_operation_traits {

    template <std::uint8_t Width, std::uint8_t CodeDest>
    static constexpr auto encode_operation(cpu_reg<Width, CodeDest> const &dest)
    {
        constexpr uint8_t operands_length
            = encoding_sizes::prefix_v<Width> + encoding_sizes::op_code
              + encoding_sizes::mod_reg_rm;

        constexpr uint8_t mod_reg_rm = make_mod_reg_rm(dest, ExCode);
        constexpr uint8_t op_code_mask
            = (Width == 16 || Width == 32 || Width == 64) ? 1 : 0;

        using op_bytes = code_t<operands_length>;
        if constexpr (Width == 16) {
            return op_bytes{
                prefixes::operand_override, OpCode | op_code_mask, mod_reg_rm};
        } else if constexpr (Width == 64) {
            return op_bytes{
                prefixes::rex_v<prefixes::rex_fields::w>,
                OpCode | op_code_mask,
                mod_reg_rm};
        } else {
            return op_bytes{OpCode | op_code_mask, mod_reg_rm};
        }
    }
};

}  // namespace x86_64
}  // namespace jit