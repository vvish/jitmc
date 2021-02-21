#pragma once

namespace jit
{
template <typename... Ts>
struct substitutions {
    constexpr substitutions()
        : _placeholders(generate_placeholders(std::index_sequence_for<Ts...>{}))
    {}

    template <typename Containers>
    constexpr void bind(jit::code_block_t<Containers>& code, Ts... values) const
    {
        uint32_t i = 0;
        (code.set_immediate(jit::place_holder<Ts>{i++}, Ts{values}), ...);
    }

    template <size_t I>
    constexpr auto get() const
    {
        return std::get<I>(_placeholders);
    }

private:
    template <size_t... Idxs>
    static constexpr auto generate_placeholders(std::index_sequence<Idxs...>)
    {
        return std::tuple{
            jit::place_holder<Ts>{static_cast<uint32_t>(Idxs)}...};
    }

private:
    std::tuple<jit::place_holder<Ts>...> _placeholders;
};

}  // namespace jit