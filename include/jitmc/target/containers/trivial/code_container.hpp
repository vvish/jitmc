#pragma once

#include "nonstd/span.hpp"

#include <algorithm>
#include <cstdint>
#include <memory>

namespace jit
{
namespace trivial
{

template <typename Alloc, size_t CodeSize>
class code_container {
public:
    using value_type = uint8_t;

    explicit code_container(Alloc const &alloc = Alloc())
        : _alloc(alloc)
        , _code(
              _alloc.allocate(CodeSize),
              [al = alloc](auto *ptr) mutable { al.deallocate(ptr, CodeSize); })
        , _position(0)
    {}

    code_container(code_container &) = delete;
    code_container &operator=(code_container &) = delete;

    template <typename It>
    void append(It const &begin, It const &end)
    {
        std::copy(begin, end, _code.get() + _position);
        _position += (end - begin);
    }

    nonstd::span<uint8_t> get_code()
    {
        return nonstd::span<value_type>(_code.get(), _position);
    }

    Alloc get_allocator() const
    {
        return _alloc;
    }

private:
    Alloc _alloc;
    using memory_descriptor = std::shared_ptr<value_type>;
    memory_descriptor _code;
    size_t _position;
};

}  // namespace trivial
}  // namespace jit