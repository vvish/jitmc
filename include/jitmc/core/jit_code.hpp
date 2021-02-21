#pragma once

#include "nonstd/span.hpp"

#include <array>
#include <cstddef>

namespace jit
{

template <typename Type>
struct place_holder {
    uint32_t const id;
    static constexpr uint8_t size = sizeof(Type);
};

template <typename Traits = void>
struct label_t {
    uint32_t const id;
    constexpr size_t size() const
    {
        return 0;
    }
};

struct code_area {
    size_t offset;
    size_t size;
};

struct patch_location {
    uint32_t dependency_id;
    code_area op_location;
    code_area patch_location;
};

template <size_t Size>
using code_t = std::array<uint8_t, Size>;

using code_span_t = nonstd::span<uint8_t>;

template <size_t Size>
struct patchable_code {
    uint32_t dependency_id;
    size_t patch_offset;
    size_t patch_size;
    code_t<Size> bytes;

    constexpr size_t size() const
    {
        return Size;
    }
};

struct label_descriptor {
    uint32_t id;
    uint32_t offset;
};

template <size_t CodeSize, size_t PatchesCount, size_t LabelsCount>
struct raw_code_block_t {
    code_t<CodeSize> bytes;
    std::array<patch_location, PatchesCount> patches;
    std::array<label_descriptor, LabelsCount> labels;
};

namespace detail
{
template <typename T>
using remove_cvref_t = std::remove_cv_t<std::remove_reference_t<T>>;

template <typename T>
struct is_patch : std::false_type {};

template <size_t Size>
struct is_patch<jit::patchable_code<Size>> : std::true_type {};

template <typename T>
struct is_label : std::false_type {};

template <typename LabelTraits>
struct is_label<label_t<LabelTraits>> : std::true_type {};

struct processing_context {
    size_t code_position;
    size_t patch_count;
    size_t label_count;
};

template <size_t T1, size_t T2, size_t PatchesCount, size_t LabelsCount>
constexpr void append(
    raw_code_block_t<T1, PatchesCount, LabelsCount> &to,
    code_t<T2> const &from,
    processing_context &context)
{
    for (auto i = 0; i < from.size(); ++i, ++context.code_position)
        to.bytes[context.code_position] = from[i];
}

template <size_t T1, size_t T2, size_t PatchesCount, size_t LabelsCount>
constexpr void append(
    raw_code_block_t<T1, PatchesCount, LabelsCount> &to,
    patchable_code<T2> const &from,
    processing_context &context)
{
    to.patches[context.patch_count++] = jit::patch_location{
        from.dependency_id,
        {context.code_position, from.size()},
        {context.code_position + from.patch_offset, from.patch_size}};

    for (auto i = 0; i < from.bytes.size(); ++i, ++context.code_position)
        to.bytes[context.code_position] = from.bytes[i];
}

template <size_t T1, size_t PatchesCount, size_t LabelsCount, typename LabelTag>
constexpr void append(
    raw_code_block_t<T1, PatchesCount, LabelsCount> &to,
    label_t<LabelTag> const &from,
    processing_context &context)
{
    to.labels[context.label_count++]
        = label_descriptor{from.id, context.code_position};
}

}  // namespace detail

template <typename... T>
constexpr auto make_code(T &&...args)
{
    constexpr auto code_size = (size_t(0) + ... + args.size());
    constexpr auto patches_count
        = (0 + ... + detail::is_patch<detail::remove_cvref_t<T>>::value);
    constexpr auto labels_count
        = (0 + ... + detail::is_label<detail::remove_cvref_t<T>>::value);

    raw_code_block_t<code_size, patches_count, labels_count> result{};
    detail::processing_context context{};
    (detail::append(result, std::forward<T>(args), context), ...);

    return result;
}

}  // namespace jit