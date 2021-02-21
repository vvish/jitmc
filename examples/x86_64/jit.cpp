#include "jitmc/arch/x86_64/x86_byte_placement_strategy.hpp"
#include "jitmc/arch/x86_64/x86_operations.hpp"
#include "jitmc/core/core.hpp"
#include "jitmc/target/containers/trivial/all.hpp"
#include "jitmc/target/posix/exec_mem_allocator.hpp"

#include <iostream>

namespace
{
namespace cntr = jit::trivial;
struct dependencies {
    cntr::code_container<jit::posix::exec_mem_allocator<uint8_t>, 256> code;
    cntr::linear_container<jit::label_descriptor, 10> labels;
    cntr::associative_container<uint32_t, jit::patch_location, 10> patches;
    jit::x86_64::x86_byte_placement_strategy placement_strategy;
};
}  // namespace

int main()
{
    using namespace jit;

    constexpr substitutions<uint8_t> subst;
    dependencies deps;
    code_block_t code(deps);

    x86_64::short_label jmp_label{5};
    code << make_code(
        x86_64::mov(x86_64::rdi, x86_64::rax),
        x86_64::add(subst.get<0>(), x86_64::rax),
        x86_64::jmp(jmp_label),
        x86_64::n0t(x86_64::rax),
        jmp_label,
        x86_64::ret());

    subst.bind(code, 10);
    code.prepare();
    auto f = code.as_function<uint64_t(uint64_t)>();

    std::cout << std::hex;
    for (auto const b : code.get_code()) {
        std::cout << (int)b << " ";
    }

    std::cout << std::endl;
    std::cout << std::dec;
    std::cout << f(20);
    std::cout << std::endl;

    return 0;
}