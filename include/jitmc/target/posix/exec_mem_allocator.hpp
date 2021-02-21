#pragma once

#include <memory>
#include <new>

#include <sys/mman.h>

namespace jit
{
namespace posix
{

/*
Allocates memory that can be later marked as executable
*/

template <typename T>
class exec_mem_allocator {
public:
    using value_type = T;

    // The allocator can/should be propagated on move and
    // can not be copied
    using propagate_on_container_move_assignment = std::true_type;
    using propagate_on_container_swap = std::true_type;

    using propagate_on_container_copy_assignment = std::false_type;
    static exec_mem_allocator select_on_container_copy_construction()
    {
        return {};
    }

public:
    exec_mem_allocator() : _mapped_memory(nullptr), _size(0)
    {}

    exec_mem_allocator(const exec_mem_allocator&) = default;
    exec_mem_allocator(exec_mem_allocator&&) = default;

    exec_mem_allocator& operator=(const exec_mem_allocator&) = default;
    exec_mem_allocator& operator=(exec_mem_allocator&&) = default;

    T* allocate(size_t const num)
    {
        auto const size = sizeof(T) * num;
        auto const ptr = mmap(
            0,
            size,
            PROT_READ | PROT_WRITE,
            MAP_PRIVATE | MAP_ANONYMOUS,
            -1,
            0);
        if (ptr == reinterpret_cast<void*>(-1)) {
            throw std::bad_alloc();
        }

        _size = size;
        _mapped_memory = ptr;

        return reinterpret_cast<T*>(ptr);
    }

    void deallocate(T* ptr, size_t num)
    {
        auto const size = sizeof(T) * num;
        munmap(ptr, size);
    }

    void make_memory_executable()
    {
        // TODO: error handling
        if (mprotect(_mapped_memory, _size, PROT_READ | PROT_EXEC) == -1) {
        
        }
    }

private:
    void* _mapped_memory;
    size_t _size;
};

template <typename T1, typename T2>
bool operator==(
    const exec_mem_allocator<T1>& lhs, const exec_mem_allocator<T2>& rhs)
{
    return false;
}

template <typename T1, typename T2>
bool operator!=(
    const exec_mem_allocator<T1>& lhs, const exec_mem_allocator<T2>& rhs)
{
    return true;
}

}  // namespace posix
}  // namespace jit