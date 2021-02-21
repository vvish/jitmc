#pragma once

#include <optional>
#include <unordered_map>
#include <utility>

namespace jit
{
namespace stl_based
{

template <typename K, typename V>
class associative_container {
public:
    using container_type = std::unordered_map<K, V>;

    template <typename... Args>
    bool insert(Args &&...args)
    {
        _container.emplace(std::forward<Args>(args)...);
        return true;
    }

    std::optional<V> find(K const &key) const
    {
        auto const it = _container.find(key);
        return (it != _container.end()) ? std::optional<V>{it->second}
                                        : std::nullopt;
    }

    size_t size() const
    {
        return _container.size();
    }

private:
    container_type _container;
};

}  // namespace stl_based
}  // namespace jit