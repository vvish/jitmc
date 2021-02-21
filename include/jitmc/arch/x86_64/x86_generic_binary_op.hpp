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

enum class sib_scale : uint8_t {
    x1 = 0,
    x2 = 1,
    x4 = 2,
    x8 = 3,
};

struct sib {
    uint8_t base_register;
    uint8_t index_register;
    sib_scale scale;
};

template <typename Disp>
struct sib_disp {
    sib reference;
    Disp displacement;
};

struct displacement {
    uint32_t value;
};

template <std::uint8_t Width, std::uint8_t CodeSource, std::uint8_t CodeDest>
constexpr std::uint8_t make_mod_reg_rm(
    cpu_reg<Width, CodeSource>, cpu_reg<Width, CodeDest>)
{
    return std::uint8_t{
        static_cast<uint8_t>(addressing_mode::register_addressing) << 6
        | CodeSource << 3 | CodeDest};
};

// d should be 1 (reg is destination)
template <typename DisplacementType, std::uint8_t Width, std::uint8_t CodeDest>
constexpr std::uint8_t make_mod_reg_rm(
    DisplacementType displacement, cpu_reg<Width, CodeDest>)
{
    constexpr uint8_t sib_rm = 0b100;
    return static_cast<uint8_t>(addressing_mode::indirect) << 6 | CodeDest << 3
           | sib_rm;
}

template <uint8_t OpCode, uint8_t ExCode>
struct generic_binary_op_imm_reg {
    template <typename Type, std::uint8_t Width, std::uint8_t CodeDest>
    static constexpr auto encode_operation(
        Type const immediate, cpu_reg<Width, CodeDest> const dest)
    {
        constexpr uint8_t immediate_offset
            = encoding_sizes::prefix_v<Width> + encoding_sizes::op_code
              + encoding_sizes::mod_reg_rm;

        constexpr uint8_t operands_length
            = immediate_offset + encoding_sizes::immidiate_v<Type>;

        constexpr uint8_t mod_reg_rm
            = static_cast<uint8_t>(addressing_mode::register_addressing) << 6
              | ExCode << 3 | CodeDest;

        constexpr uint8_t op_code_mask
            = ((Width == 16 || Width == 32 || Width == 64) ? 1 : 0)
              | (sizeof(Type) != (Width / 8) ? 0b10 : 0) | 0b10000000;

        using op_bytes = code_t<operands_length>;

        if constexpr (Width == 16) {
            op_bytes result{
                prefixes::operand_override,
                OpCode | op_code_mask,
                static_cast<uint8_t>(mod_reg_rm)};

            place_bytes(
                nonstd::make_span(result).subspan(
                    immediate_offset, encoding_sizes::immidiate_v<Type>),
                immediate);

            return result;

        } else if constexpr (Width == 64) {
            op_bytes result{
                prefixes::rex_v<prefixes::rex_fields::w>,
                OpCode | op_code_mask,
                static_cast<uint8_t>(mod_reg_rm)};

            place_bytes(
                nonstd::make_span(result).subspan(
                    immediate_offset, encoding_sizes::immidiate_v<Type>),
                immediate);

            return result;

        } else {
            op_bytes result{
                OpCode | op_code_mask, static_cast<uint8_t>(mod_reg_rm)};

            place_bytes(
                nonstd::make_span(result).subspan(
                    immediate_offset, encoding_sizes::immidiate_v<Type>),
                immediate);

            return result;
        }
    }

    template <typename Type, std::uint8_t Width, std::uint8_t CodeDest>
    static constexpr auto encode_operation(
        place_holder<Type> const immediate, cpu_reg<Width, CodeDest> const dest)
    {
        constexpr uint8_t immediate_offset
            = encoding_sizes::prefix_v<Width> + encoding_sizes::op_code
              + encoding_sizes::mod_reg_rm;

        constexpr uint8_t operands_length
            = immediate_offset + encoding_sizes::immidiate_v<Type>;

        using op_encoding = patchable_code<operands_length>;

        return op_encoding{
            immediate.id,
            immediate_offset,
            place_holder<Type>::size,
            encode_operation(Type{0}, dest)};
    }
};

template <uint8_t OpCode>
struct binary_operation_traits_short_imm8 {
    template <std::uint8_t CodeDest>
    static constexpr auto encode_operation(
        uint8_t const immediate, cpu_reg<8, CodeDest> const &dest)
    {
        constexpr uint8_t operands_length = 2;
        using op_bytes = code_t<operands_length>;
        return op_bytes{OpCode + CodeDest, immediate};
    }
};

template <uint8_t OpCode>
struct binary_operation_traits_short_imm32 {
    template <std::uint8_t CodeDest>
    static constexpr auto encode_operation(
        uint32_t const immediate, cpu_reg<32, CodeDest> const &dest)
    {
        constexpr uint8_t operands_length = 5;
        constexpr uint8_t immediate_offset = 1;
        using op_bytes = code_t<operands_length>;

        op_bytes result{OpCode + CodeDest};
        place_bytes(
            nonstd::make_span(result).subspan(
                immediate_offset, encoding_sizes::immidiate_v<uint32_t>),
            immediate);

        return result;
    }
};

template <uint8_t OpCode, uint8_t ExCode = 0>
struct generic_binary_op_disp_reg {
    template <std::uint8_t Width, std::uint8_t CodeDest>
    static constexpr auto encode_operation(
        displacement const &disp, cpu_reg<Width, CodeDest> const dest)
    {
        constexpr uint8_t operands_length = (Width == 16 ? 8 : 7);
        constexpr uint8_t mod_reg_rm = make_mode_reg_rm(disp, dest);
        constexpr uint8_t disp_offset
            = encoding_sizes::prefix_v<Width> + encoding_sizes::op_code
              + encoding_sizes::mod_reg_rm + encoding_sizes::sib;

        constexpr uint8_t op_code_mask = (Width == 32) ? 0b11 : 0b10;
        constexpr uint8_t disp_only_sib = 0b00100101;

        using op_bytes = code_t<operands_length>;

        if constexpr (Width == 16) {
            op_bytes result{
                prefixes::operand_override,
                OpCode | op_code_mask,
                mod_reg_rm,
                disp_only_sib};

            place_bytes(
                nonstd::make_span(result).subspan(
                    disp_offset, encoding_sizes::displacement),
                disp.value);
            return result;

        } else {
            op_bytes result{OpCode | op_code_mask, mod_reg_rm, disp_only_sib};

            place_bytes(
                nonstd::make_span(result).subspan(
                    disp_offset, encoding_sizes::displacement),
                disp.value);

            return result;
        }
    }
};

template <uint8_t OpCode>
struct generic_binary_op_reg_reg {
    template <
        std::uint8_t Width,
        std::uint8_t CodeSource,
        std::uint8_t CodeDest>
    static constexpr auto encode_operation(
        cpu_reg<Width, CodeSource> const source,
        cpu_reg<Width, CodeDest> const dest)
    {
        constexpr uint8_t operands_length
            = encoding_sizes::prefix_v<Width> + encoding_sizes::op_code
              + encoding_sizes::mod_reg_rm;
        constexpr uint8_t mod_reg_rm = make_mod_reg_rm(source, dest);
        constexpr uint8_t op_code_mask
            = (Width == 16 || Width == 32 || Width == 64) ? 1 : 0;

        using op_bytes = code_t<operands_length>;
        if constexpr (Width == 16) {
            return op_bytes{
                prefixes::operand_override,
                OpCode | op_code_mask,
                static_cast<uint8_t>(mod_reg_rm)};
        } else if constexpr (Width == 64) {
            return op_bytes{
                prefixes::rex_v<prefixes::rex_fields::w>,
                OpCode | op_code_mask,
                static_cast<uint8_t>(mod_reg_rm)};
        } else {
            return op_bytes{
                OpCode | op_code_mask, static_cast<uint8_t>(mod_reg_rm)};
        }
    }
};

}  // namespace x86_64
}  // namespace jit