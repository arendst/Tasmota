// Crash immediately by executing a system call with an invalid system call class id

// Define the `_start_` signature.
void _start(void* app_start, void* mem_start, void* memory_len, void* app_heap_break);

__attribute__((naked))
void _start(void* app_start __attribute__((unused)),
            void* mem_start __attribute__((unused)),
            void* memory_len __attribute__((unused)),
            void* app_heap_break __attribute__((unused))) {
  // invalid syscall
#if defined(__thumb__)
  __asm__ volatile (
    "svc 0xbd       \n"
    :
    :
    : "memory", "r0", "r1", "r2", "r3", "r12", "lr"
    );
#elif defined(__riscv)
  __asm__ volatile (
    "li    a4, 0xbd\n"
    "ecall\n"
    :
    :
    : "memory", "a2", "a3", "a4", "a5", "a6", "a7",
    "t0", "t1", "t2", "t3", "t4", "t5", "t6", "ra"
    );
#endif
}
