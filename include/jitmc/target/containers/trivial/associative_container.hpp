#pragma once

#include <algorithm>
#include <array>
#include <optional>
#include <utility>

namespace jit
{
namespace trivial
{

template <typename K, typename V, size_t Capacity>
class associative_container {
public:
    template <typename... Args>
    bool insert(Args &&...args)
    {
        if (_next_index < _container.size()) {
            _container[_next_index++] = value_type{std::forward<Args>(args)...};
            return true;
        } else {
            return false;
        }
    }

    std::optional<V> find(K const &key) const
    {
        auto const it = std::find_if(
            _container.begin(), _container.end(), [&key](auto const &i) {
                return i.first == key;
            });
        return (it != _container.end()) ? std::optional<V>{it->second}
                                        : std::nullopt;
    }

    size_t size() const
    {
        return _next_index;
    }

private:
    using value_type = std::pair<K, V>;
    using container_type = std::array<value_type, Capacity>;

    container_type _container;
    size_t _next_index = 0;
};

}  // namespace trivial
}  // namespace jit