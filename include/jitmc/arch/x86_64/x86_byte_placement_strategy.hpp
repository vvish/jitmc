#pragma once

#include "jitmc/arch/x86_64/x86_common.hpp"

namespace jit
{
namespace x86_64
{

struct x86_byte_placement_strategy {
    template <typename T>
    constexpr void place_bytes(nonstd::span<uint8_t> const &dest, T const value)
    {
        ::jit::x86_64::place_bytes(dest, value);
    }
};

}  // namespace x86
}  // namespace jit
