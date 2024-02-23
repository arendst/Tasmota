#include "tock.h"
#include <string.h>

#if defined(STACK_SIZE)
#warning Attempt to compile libtock with a fixed STACK_SIZE.
#warning
#warning Instead, STACK_SIZE should be a variable that is linked in,
#warning usually at compile time via something like this:
#warning   `gcc ... -Xlinker --defsym=STACK_SIZE=2048`
#warning
#warning This allows applications to set their own STACK_SIZE.
#error Fixed STACK_SIZE.
#endif

extern int main(void);

// Allow _start to go undeclared
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wmissing-prototypes"

// The structure populated by the linker script at the very beginning of the
// text segment. It represents sizes and offsets from the text segment of
// sections that need some sort of loading and/or relocation.
struct hdr {
  //  0: Offset of GOT symbols in flash from the start of the application
  //     binary.
  uint32_t got_sym_start;
  //  4: Offset of where the GOT section needs to be placed in memory from the
  //     start of the application's memory region.
  uint32_t got_start;
  //  8: Size of GOT section.
  uint32_t got_size;
  // 12: Offset of data symbols in flash from the start of the application
  //     binary.
  uint32_t data_sym_start;
  // 16: Offset of where the data section needs to be placed in memory from the
  //     start of the application's memory region.
  uint32_t data_start;
  // 20: Size of data section.
  uint32_t data_size;
  // 24: Offset of where the BSS section needs to be placed in memory from the
  //     start of the application's memory region.
  uint32_t bss_start;
  // 28: Size of BSS section.
  uint32_t bss_size;
  // 32: First address offset after program flash, where elf2tab places
  //     .rel.data section
  uint32_t reldata_start;
  // 36: The size of the stack requested by this application.
  uint32_t stack_size;
};

// The structure of the relative data section. This structure comes from the
// compiler.
struct reldata {
  // Number of relative addresses.
  uint32_t len;
  // Array of offsets of the address to be updated relative to the start of the
  // application's memory region. Each address at these offsets needs to be
  // adjusted to be a fixed address relative to the start of the app's actual
  // flash or RAM start address.
  uint32_t data[];
};

__attribute__ ((section(".start"), used))
__attribute__ ((weak))
__attribute__ ((naked))
__attribute__ ((noreturn))
void _start(void* app_start __attribute__((unused)),
            void* mem_start __attribute__((unused)),
            void* memory_len __attribute__((unused)),
            void* app_heap_break __attribute__((unused))) {
#if defined(__thumb__)
  // Assembly written to adhere to any modern thumb arch

  // Allocate stack and data. `brk` to stack_size + got_size + data_size +
  // bss_size from start of memory. Also make sure that the stack starts on an
  // 8 byte boundary per section 5.2.1.2 here:
  // http://infocenter.arm.com/help/topic/com.arm.doc.ihi0042f/IHI0042F_aapcs.pdf

  __asm__ volatile (
    // Compute the stack top
    //
    // struct hdr* myhdr = (struct hdr*)app_start;
    // uint32_t stacktop = (((uint32_t)mem_start + myhdr->stack_size + 7) & 0xfffffff8);
    "ldr  r4, [r0, #36]\n"      // r4 = myhdr->stack_size
    "add  r4, #7\n"             // r4 = myhdr->stack_size + 7
    "add  r4, r4, r1\n"         // r4 = mem_start + myhdr->stack_size + 7
    "movs r5, #7\n"
    "bic  r4, r4, r5\n"         // r4 = (mem_start + myhdr->stack_size + 7) & ~0x7
    //
    // Compute the app data size and where initial app brk should go.
    // This includes the GOT, data, and BSS sections. However, we can't be sure
    // the linker puts them back-to-back, but we do assume that BSS is last
    // (i.e. myhdr->got_start < myhdr->bss_start && myhdr->data_start <
    // myhdr->bss_start). With all of that true, then the size is equivalent
    // to the end of the BSS section.
    //
    // uint32_t app_brk = mem_start + myhdr->bss_start + myhdr->bss_size;
    "ldr  r5, [r0, #24]\n"      // r6 = myhdr->bss_start
    "ldr  r6, [r0, #28]\n"      // r6 = myhdr->bss_size
    "add  r5, r5, r6\n"         // r5 = bss_start + bss_size
    "add  r5, r5, r1\n"         // r5 = mem_start + bss_start + bss_size = app_brk
    //
    // Move registers we need to keep over to callee-saved locations
    "movs r6, r0\n"             // r6 = app_start
    "movs r7, r1\n"             // r7 = mem_start
    //
    // Now we may want to move the stack pointer. If the kernel set the
    // `app_heap_break` larger than we need (and we are going to call `brk()`
    // to reduce it) then our stack pointer will fit and we can move it now.
    // Otherwise after the first syscall (the memop to set the brk), the return
    // will use a stack that is outside of the process accessible memory.
    //
    "cmp r5, r3\n"              // Compare `app_heap_break` with new brk.
    "bgt skip_set_sp\n"         // If our current `app_heap_break` is larger
                                // then we need to move the stack pointer
                                // before we call the `brk` syscall.
    "mov  sp, r4\n"             // Update the stack pointer.
    //
    "skip_set_sp:\n"            // Back to regularly scheduled programming.
    //
    // Call `brk` to set to requested memory
    //
    // memop(0, app_brk);
    "movs r0, #0\n"
    "movs r1, r5\n"
    "svc 5\n"                   // memop
    //
    // Setup initial stack pointer for normal execution. If we did this before
    // then this is redundant and just a no-op. If not then no harm in
    // re-setting it.
    "mov  sp, r4\n"
    //
    // Debug support, tell the kernel the stack location
    //
    // memop(10, stacktop);
    "movs r0, #10\n"
    "movs r1, r4\n"
    "svc 5\n"                   // memop
    //
    // Debug support, tell the kernel the heap location
    //
    // memop(11, app_brk);
    "movs r0, #11\n"
    "movs r1, r5\n"
    "svc 5\n"                   // memop
    //
    // Set the special PIC register r9. This has to be set to the address of the
    // beginning of the GOT section. The PIC code uses this as a reference point
    // to enable the RAM section of the app to be at any address.
    "ldr  r0, [r6, #4]\n"       // r0 = myhdr->got_start
    "add  r0, r0, r7\n"         // r0 = myhdr->got_start + mem_start
    "mov  r9, r0\n"             // r9 = r0
    //
    // Call into the rest of startup.
    // This should never return, if it does, trigger a breakpoint (which will
    // promote to a HardFault in the absence of a debugger)
    "movs r0, r6\n"             // first arg is app_start
    "movs r1, r7\n"             // second arg is mem_start
    "bl _c_start_pic\n"
    "bkpt #255\n"
    );

#elif defined(__riscv)

  __asm__ volatile (
    // Compute the stack top.
    //
    // struct hdr* myhdr = (struct hdr*) app_start;
    // uint32_t stacktop = (((uint32_t) mem_start + myhdr->stack_size + 7) & 0xfffffff8);
    "lw   t0, 36(a0)\n"         // t0 = myhdr->stack_size
    "addi t0, t0, 7\n"          // t0 = myhdr->stack_size + 7
    "add  t0, t0, a1\n"         // t0 = mem_start + myhdr->stack_size + 7
    "li   t1, 7\n"              // t1 = 7
    "not  t1, t1\n"             // t1 = ~0x7
    "and  t0, t0, t1\n"         // t0 = (mem_start + myhdr->stack_size + 7) & ~0x7
    //
    // Compute the app data size and where initial app brk should go.
    // This includes the GOT, data, and BSS sections. However, we can't be sure
    // the linker puts them back-to-back, but we do assume that BSS is last
    // (i.e. myhdr->got_start < myhdr->bss_start && myhdr->data_start <
    // myhdr->bss_start). With all of that true, then the size is equivalent
    // to the end of the BSS section.
    //
    // uint32_t app_brk = mem_start + myhdr->bss_start + myhdr->bss_size;
    "lw   t1, 24(a0)\n"         // t1 = myhdr->bss_start
    "lw   t2, 28(a0)\n"         // t2 = myhdr->bss_size
    "add  t1, t1, t2\n"         // t1 = bss_start + bss_size
    "add  t1, t1, a1\n"         // t1 = mem_start + bss_start + bss_size = app_brk
    //
    // Move arguments we need to keep over to callee-saved locations.
    "mv   s0, a0\n"             // s0 = void* app_start
    "mv   s1, t0\n"             // s1 = stack_top
    "mv   s2, a1\n"             // s2 = mem_start
    //
    // Now we may want to move the stack pointer. If the kernel set the
    // `app_heap_break` larger than we need (and we are going to call `brk()`
    // to reduce it) then our stack pointer will fit and we can move it now.
    // Otherwise after the first syscall (the memop to set the brk), the return
    // will use a stack that is outside of the process accessible memory.
    //
    "bgt t1, a3, skip_set_sp\n" // Compare `app_heap_break` with new brk.
                                // If our current `app_heap_break` is larger
                                // then we need to move the stack pointer
                                // before we call the `brk` syscall.
    "mv  sp, t0\n"              // Update the stack pointer

    "skip_set_sp:\n"            // Back to regularly scheduled programming.

    // Call `brk` to set to requested memory
    // memop(0, stacktop + appdata_size);
    "li  a4, 5\n"               // a4 = 5   // memop syscall
    "li  a0, 0\n"               // a0 = 0
    "mv  a1, t1\n"              // a1 = app_brk
    "ecall\n"                   // memop

    //
    // Setup initial stack pointer for normal execution
    "mv   sp, s1\n"             // sp = stacktop

    //
    // Debug support, tell the kernel the stack location
    //
    // memop(10, stacktop);
    "li  a4, 5\n"               // a4 = 5   // memop syscall
    "li  a0, 10\n"              // a0 = 10
    "mv  a1, s1\n"              // a1 = stacktop
    "ecall\n"                   // memop
    //
    // Debug support, tell the kernel the heap location
    //
    // memop(11, app_brk);
    "li  a4, 5\n"               // a4 = 5   // memop syscall
    "li  a0, 11\n"              // a0 = 11
    "mv  a1, t1\n"              // a1 = app_brk
    "ecall\n"                   // memop

    // Call into the rest of startup. This should never return.
    "mv   a0, s0\n"             // first arg is app_start
    "mv   s0, sp\n"             // Set the frame pointer to sp.
    "mv   a1, s2\n"             // second arg is mem_start
    "jal  _c_start_nopic\n"
    );

#else
#error Missing initial stack setup trampoline for current arch.
#endif
}

// C startup routine that configures memory for the process. This also handles
// PIC fixups that are required for the application.
//
// Arguments:
// - `app_start`: The address of where the app binary starts in flash. This does
//   not include the TBF header or any padding before the app.
// - `mem_start`: The starting address of the memory region assigned to this
//   app.
__attribute__((noreturn))
void _c_start_pic(uint32_t app_start, uint32_t mem_start) {
  struct hdr* myhdr = (struct hdr*)app_start;

  // Fix up the Global Offset Table (GOT).

  // Get the address in memory of where the table should go.
  uint32_t* got_start = (uint32_t*)(myhdr->got_start + mem_start);
  // Get the address in flash of where the table currently is.
  uint32_t* got_sym_start = (uint32_t*)(myhdr->got_sym_start + app_start);
  // Iterate all entries in the table and correct the addresses.
  for (uint32_t i = 0; i < (myhdr->got_size / (uint32_t)sizeof(uint32_t)); i++) {
    // Use the sentinel here. If the most significant bit is 0, then we know
    // this offset is pointing to an address in memory. If the MSB is 1, then
    // the offset refers to a value in flash.
    if ((got_sym_start[i] & 0x80000000) == 0) {
      // This is an address for something in memory, and we need to correct the
      // address now that we know where this app is actually running in memory.
      // This equation is really:
      //
      //     got_entry = (got_stored_entry - original_RAM_start_address) + actual_RAM_start_address
      //
      // However, we compiled the app where `original_RAM_start_address` is 0x0,
      // so we can omit that.
      got_start[i] = got_sym_start[i] + mem_start;
    } else {
      // Otherwise, this address refers to something in flash. Now that we know
      // where the app has actually been loaded, we can reference from the
      // actual `app_start` address. We also have to remove our fake flash
      // address sentinel (by ORing with 0x80000000).
      got_start[i] = (got_sym_start[i] ^ 0x80000000) + app_start;
    }
  }

  // Load the data section from flash into RAM. We use the offsets from our
  // crt0 header so we know where this starts and where it should go.
  void* data_start     = (void*)(myhdr->data_start + mem_start);
  void* data_sym_start = (void*)(myhdr->data_sym_start + app_start);
  memcpy(data_start, data_sym_start, myhdr->data_size);

  // Zero BSS segment. Again, we know where this should be in the process RAM
  // based on the crt0 header.
  char* bss_start = (char*)(myhdr->bss_start + mem_start);
  memset(bss_start, 0, myhdr->bss_size);

  // Do relative data address fixups. We know these entries are stored at the end
  // of flash and can be located using the crt0 header.
  //
  // The data structure used for these is `struct reldata`, where a 32 bit
  // length field is followed by that many entries. We iterate each entry and
  // correct addresses.
  struct reldata* rd = (struct reldata*)(myhdr->reldata_start + (uint32_t)app_start);
  for (uint32_t i = 0; i < (rd->len / (int)sizeof(uint32_t)); i += 2) {
    // The entries are offsets from the beginning of the app's memory region.
    // First, we get a pointer to the location of the address we need to fix.
    uint32_t* target = (uint32_t*)(rd->data[i] + mem_start);
    if ((*target & 0x80000000) == 0) {
      // Again, we use our sentinel. If the address at that location has a MSB
      // of 0, then we know this is an address in RAM. We need to fix the
      // address by including the offset where the app actual ended up in
      // memory. This is a simple addition since the app was compiled with a
      // memory address of zero.
      *target += mem_start;
    } else {
      // When the MSB is 1, the address is in flash. We clear our sentinel, and
      // then make the address an offset from the start of where the app is
      // located in flash.
      *target = (*target ^ 0x80000000) + app_start;
    }
  }

  main();
  while (1) {
    yield();
  }
}

// C startup routine for apps compiled with fixed addresses (i.e. no PIC).
//
// Arguments:
// - `app_start`: The address of where the app binary starts in flash. This does
//   not include the TBF header or any padding before the app.
// - `mem_start`: The starting address of the memory region assigned to this
//   app.
__attribute__((noreturn))
void _c_start_nopic(uint32_t app_start, uint32_t mem_start) {
  struct hdr* myhdr = (struct hdr*)app_start;

  // Copy over the Global Offset Table (GOT). The GOT seems to still get created
  // and used in some cases, even though nothing is being relocated and the
  // addresses are static. So, all we need to do is copy the GOT entries from
  // flash to RAM, without doing any address changes. Of course, if the GOT
  // length is 0 this is a no-op.
  void* got_start     = (void*)(myhdr->got_start + mem_start);
  void* got_sym_start = (void*)(myhdr->got_sym_start + app_start);
  memcpy(got_start, got_sym_start, myhdr->got_size);

  // Load the data section from flash into RAM. We use the offsets from our
  // crt0 header so we know where this starts and where it should go.
  void* data_start     = (void*)(myhdr->data_start + mem_start);
  void* data_sym_start = (void*)(myhdr->data_sym_start + app_start);
  memcpy(data_start, data_sym_start, myhdr->data_size);

  // Zero BSS segment. Again, we know where this should be in the process RAM
  // based on the crt0 header.
  char* bss_start = (char*)(myhdr->bss_start + mem_start);
  memset(bss_start, 0, myhdr->bss_size);

  main();
  while (1) {
    yield();
  }
}
