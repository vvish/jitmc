#pragma once

#include "jitmc/core/operations.hpp"

namespace jit
{
namespace x86_64
{
constexpr auto rax = cpu_reg<64, 0>{};
constexpr auto rcx = cpu_reg<64, 1>{};
constexpr auto rdx = cpu_reg<64, 2>{};
constexpr auto rbx = cpu_reg<64, 3>{};
constexpr auto rsp = cpu_reg<64, 4>{};
constexpr auto rbp = cpu_reg<64, 5>{};
constexpr auto rsi = cpu_reg<64, 6>{};
constexpr auto rdi = cpu_reg<64, 7>{};

constexpr auto eax = cpu_reg<32, 0>{};
constexpr auto ecx = cpu_reg<32, 1>{};
constexpr auto edx = cpu_reg<32, 2>{};
constexpr auto ebx = cpu_reg<32, 3>{};
constexpr auto esp = cpu_reg<32, 4>{};
constexpr auto ebp = cpu_reg<32, 5>{};
constexpr auto esi = cpu_reg<32, 6>{};
constexpr auto edi = cpu_reg<32, 7>{};

constexpr auto ax = cpu_reg<16, 0>{};
constexpr auto cx = cpu_reg<16, 1>{};
constexpr auto dx = cpu_reg<16, 2>{};
constexpr auto bx = cpu_reg<16, 3>{};
constexpr auto sp = cpu_reg<16, 4>{};
constexpr auto bp = cpu_reg<16, 5>{};
constexpr auto si = cpu_reg<16, 6>{};
constexpr auto di = cpu_reg<16, 7>{};

constexpr auto al = cpu_reg<8, 0>{};
constexpr auto cl = cpu_reg<8, 1>{};
constexpr auto dl = cpu_reg<8, 2>{};
constexpr auto bl = cpu_reg<8, 3>{};
constexpr auto ah = cpu_reg<8, 4>{};
constexpr auto ch = cpu_reg<8, 5>{};
constexpr auto dh = cpu_reg<8, 6>{};
constexpr auto bh = cpu_reg<8, 7>{};

}  // namespace x86_64
}  // namespace jit