#pragma once

#include "jitmc/core/jit_code.hpp"

#include <type_traits>
namespace jit
{
namespace x86_64
{

namespace detail
{
struct short_label {};
struct far_label {};
};  // namespace detail

using short_label = label_t<detail::short_label>;
using long_label = label_t<detail::far_label>;

struct branch_operation_traits {
    template <typename Type>
    static constexpr auto encode_operation(label_t<Type> const &target)
    {
        constexpr uint8_t opcode = 0xE9;

        if constexpr (std::is_same_v<Type, detail::short_label>) {
            constexpr uint8_t op_code_mask = 0b10;
            constexpr uint8_t operands_length = 2;
            return patchable_code<operands_length>{
                target.id, 1, 1, {opcode | op_code_mask, 0}};
        } else {
            constexpr uint8_t operands_length = 5;
            return patchable_code<operands_length>{
                target.id, 1, 4, {opcode, 0, 0, 0, 0}};
        }
    }
};

struct ret_traits {
    static constexpr auto encode_operation()
    {
        return code_t<1>{0xc3};
    };

    static constexpr auto encode_operation(uint16_t stack_to_clean_size)
    {
        return code_t<3>{
            0xc2,
            static_cast<uint8_t>(stack_to_clean_size & 0xFF),
            static_cast<uint8_t>(stack_to_clean_size >> 8 & 0xFF)};
    };
};

}  // namespace x86_64
}  // namespace jit