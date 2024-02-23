#include <stdio.h>
#include <sys/types.h>

int main(void) {
  printf("Stack Test App\n");

  register uint32_t stack_pointer;

#if defined(__thumb__)
  asm volatile ("MOV %0, sp\n" : "=r" (stack_pointer));
#elif defined(__riscv)
  asm volatile ("mv %0, sp\n" : "=r" (stack_pointer));
#else
#error Unknown architecture
#endif

  printf("Current stack pointer: 0x%lx\n", stack_pointer);

  return 0;
}
