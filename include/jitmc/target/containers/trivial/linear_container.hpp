#pragma once

#include <array>
#include <utility>

namespace jit
{
namespace trivial
{

template <typename T, size_t Capacity>
class linear_container {
public:
    using value_type = T;
    using iterator = value_type *;
    using const_iterator = value_type const *;

    template <typename I>
    bool append(I &&item)
    {
        if (_next_index < _container.size()) {
            _container[_next_index++] = value_type{std::forward<I>(item)};
            return true;
        } else {
            return false;
        }
    }

    iterator begin()
    {
        return _container.begin();
    }

    iterator end()
    {
        return _container.begin() + _next_index;
    }

    const_iterator begin() const
    {
        return _container.begin();
    }

    const_iterator end() const
    {
        return _container.begin() + _next_index;
    }

    size_t size() const
    {
        return _next_index;
    }

private:
    using container_type = std::array<T, Capacity>;

    container_type _container;
    size_t _next_index = 0;
};

}  // namespace trivial
}  // namespace jit