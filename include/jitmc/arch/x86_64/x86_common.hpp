#pragma once

#include "nonstd/span.hpp"

#include <array>
#include <cstdint>

namespace jit
{
namespace x86_64
{

namespace prefixes
{
constexpr uint8_t operand_override = 0x66;
constexpr uint8_t address_override = 0x67;

enum class rex_fields : uint8_t {
    w = 0b1000,
    r = 0b0100,
    x = 0b0010,
    b = 0b0001,
};

template <rex_fields... Fs>
constexpr uint8_t rex_v
    = (uint8_t{0b01000000} | ... | static_cast<uint8_t>(Fs));

}  // namespace prefixes

namespace encoding_sizes
{

template <typename ImmType>
constexpr size_t immidiate_v = sizeof(ImmType) == 1 ? 1 : 4;

template <uint8_t RegisterWidthInBits>
constexpr size_t prefix_v
    = (RegisterWidthInBits == 64 || RegisterWidthInBits == 16) ? 1 : 0;

constexpr size_t displacement = sizeof(uint32_t);
constexpr size_t op_code = 1;
constexpr size_t mod_reg_rm = 1;
constexpr size_t sib = 1;

}  // namespace encoding_sizes

template <typename T>
constexpr std::array<uint8_t, sizeof(T)> get_bytes(T const value)
{
    std::array<uint8_t, sizeof(T)> result{};
    for (auto i = 0; i < sizeof(T); ++i) {
        result[i] = ((value >> i * 8) & 0xFF);
    }

    return result;
}

template <typename T>
constexpr void place_bytes(nonstd::span<uint8_t> const &dest, T const value)
{
    auto const bytes_to_place_cnt = std::min(dest.size(), sizeof(T));
    for (auto i = 0; i < bytes_to_place_cnt; ++i) {
        dest[i] = ((value >> i * 8) & 0xFF);
    }
}

enum class addressing_mode {
    indirect = 0b00,
    one_byte_displacement = 0b01,
    four_byte_displacement = 0b10,
    register_addressing = 0b11,
};

}  // namespace x86_64
}  // namespace jit