#pragma once

#include "nonstd/span.hpp"

#include <cstdint>
#include <vector>

namespace jit
{
namespace stl_based
{

template <typename Alloc>
class code_container {
public:
    using value_type = uint8_t;

    explicit code_container(
        size_t const initial_code_size = 1024, Alloc const &alloc = Alloc())
        : _code(alloc)
    {
        _code.reserve(1024);
    }

    code_container(code_container &) = delete;
    code_container &operator=(code_container &) = delete;

    template <typename It>
    void append(It const &begin, It const &end)
    {
        _code.insert(_code.end(), begin, end);
    }

    nonstd::span<uint8_t> get_code()
    {
        return _code;
    }

    Alloc get_allocator() const
    {
        return _code.get_allocator();
    }

private:
    std::vector<value_type, Alloc> _code;
};

}  // namespace stl_based
}  // namespace jit