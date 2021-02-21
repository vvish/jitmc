#pragma once

#include <cstdint>

namespace jit
{

template <std::uint8_t Width, std::uint8_t Code>
struct cpu_reg {
    constexpr static std::uint8_t width = Width;
    constexpr static std::uint8_t code = Code;
};

template <typename... Traits>
struct operation : Traits... {
    using Traits::encode_operation...;

    template <typename... Op>
    static constexpr auto make(Op... op)
    {
        return encode_operation(op...);
    };
};

#define DEFINE_OP(op_name, ...)                            \
    using op_name##_alias = ::jit::operation<__VA_ARGS__>; \
    template <typename... Op>                              \
    static constexpr auto op_name(Op... op)                \
    {                                                      \
        return op_name##_alias::make(op...);               \
    }

}  // namespace jit
