// Do nothing. yield() immediately.

// Define the `_start_` signature.
void _start(void* app_start, void* mem_start, void* memory_len, void* app_heap_break);

__attribute__((naked))
void _start(void* app_start __attribute__((unused)),
            void* mem_start __attribute__((unused)),
            void* memory_len __attribute__((unused)),
            void* app_heap_break __attribute__((unused))) {
  // Yield.
#if defined(__thumb__)
  __asm__ volatile (
    "movs r0, #1 \n"
    "svc 0       \n"
    :
    :
    : "memory", "r0", "r1", "r2", "r3", "r12", "lr"
    );

#elif defined(__riscv)
  __asm__ volatile (
    "li    a0, 1\n"   // yield-wait
    "li    a4, 0\n"   // yield system call class
    "ecall\n"
    :
    :
    : "memory", "a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7",
    "t0", "t1", "t2", "t3", "t4", "t5", "t6", "ra"
    );
#endif
}
