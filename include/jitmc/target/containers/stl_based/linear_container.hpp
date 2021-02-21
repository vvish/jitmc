#pragma once

#include <utility>
#include <vector>

namespace jit
{
namespace stl_based
{

template <typename T>
class linear_container {
public:
    using container_type = std::vector<T>;
    using iterator = typename container_type::iterator;

    template <typename I>
    bool append(I &&item)
    {
        _container.emplace_back(std::forward<I>(item));
        return true;
    }

    iterator begin()
    {
        return _container.begin();
    }

    iterator end()
    {
        return _container.end();
    }

private:
    container_type _container;
};

}  // namespace stl_based
}  // namespace jit