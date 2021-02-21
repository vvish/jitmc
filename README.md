# jitmc

## Description

jitmc is supposed to be JIT macro language engine.    
**The project is currently in the "proof of concept" state and is very "raw".**

The purpose is to provide low-overhead facilities for runtime optimization.   
The engine allows to define set of macro instructions with code generation rules   
and is not platform dependant.   

Using framework the invariant code blocks can be prepared during compile time and   
patch/reorganized during runtime.

Currently, small subset of the x86_64 ISA is supported (include/jitmc/arch/x86_64)  

```c++
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
std::cout << f(20);
```

The minimalist sample application for POSIX system    
(using mmap and mprotect to allocate executable memory) can be found in examples/x86_64  

