#pragma once

#include "nonstd/span.hpp"

#include <jitmc/core/jit_code.hpp>

namespace jit
{

namespace detail
{

template <typename>
struct function_ptr_helper;

template <typename R, typename... Ps>
struct function_ptr_helper<R(Ps...)> {
    using type = R (*)(Ps...);
};

template <typename F>
using function_ptr_t = typename function_ptr_helper<F>::type;

}  // namespace detail

template <typename Containers>
class code_block_t {
public:
    code_block_t(Containers &containers) : _containers(containers)
    {}

    ~code_block_t() = default;

    template <size_t CodeSize, size_t PatchesCount, size_t LabelsCount>
    code_block_t &operator<<(
        raw_code_block_t<CodeSize, PatchesCount, LabelsCount> const &op)
    {
        _containers.code.append(op.bytes.begin(), op.bytes.end());

        for (auto const &label : op.labels)
            _containers.labels.append(label);

        for (auto const &patch : op.patches)
            _containers.patches.insert(patch.dependency_id, patch);

        return *this;
    }

    nonstd::span<uint8_t const> get_code() const
    {
        return _containers.code.get_code();
    }

    template <typename T>
    void set_immediate(place_holder<T> const &descriptor, T value)
    {
        auto const &patch = _containers.patches.find(descriptor.id);
        if (patch) {
            _containers.placement_strategy.place_bytes(
                _containers.code.get_code().subspan(
                    patch->patch_location.offset, patch->patch_location.size),
                value);
        }
    }

    template <typename F>
    auto as_function()
    {
        return reinterpret_cast<detail::function_ptr_t<F>>(
            _containers.code.get_code().begin());
    }

    void prepare()
    {
        resolve_labels();
        _containers.code.get_allocator().make_memory_executable();
    }

private:
    void resolve_labels()
    {
        for (auto const &label : _containers.labels) {
            auto const patch = _containers.patches.find(label.id);
            if (patch) {
                _containers.placement_strategy.place_bytes(
                    _containers.code.get_code().subspan(
                        patch->patch_location.offset),
                    // arch dependant jump offset calculator should be injected
                    // currently works for x86_64 short jump only
                    static_cast<uint8_t>(
                        label.offset - patch->op_location.offset
                        - patch->op_location.size));
            }
        }
    }

private:
    Containers &_containers;
};

}  // namespace jit