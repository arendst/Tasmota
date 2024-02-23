#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "tock.h"

typedef struct {
  subscribe_upcall *cb;
  int arg0;
  int arg1;
  int arg2;
  void* ud;
} tock_task_t;

#define TASK_QUEUE_SIZE  16
static tock_task_t task_queue[TASK_QUEUE_SIZE];
static int task_cur  = 0;
static int task_last = 0;

int tock_enqueue(subscribe_upcall cb, int arg0, int arg1, int arg2, void* ud) {
  int next_task_last = (task_last + 1) % TASK_QUEUE_SIZE;
  if (next_task_last == task_cur) {
    return -1;
  }

  task_queue[task_last].cb   = cb;
  task_queue[task_last].arg0 = arg0;
  task_queue[task_last].arg1 = arg1;
  task_queue[task_last].arg2 = arg2;
  task_queue[task_last].ud   = ud;
  task_last = next_task_last;

  return task_last;
}

int tock_status_to_returncode(statuscode_t status) {
  // Conversion is easy. Since ReturnCode numeric mappings are -1*ErrorCode,
  // and success is 0 in both cases, we can just multiply by -1.
  return -1 * status;
}

int tock_command_return_novalue_to_returncode(syscall_return_t command_return) {
  if (command_return.type == TOCK_SYSCALL_SUCCESS) {
    return RETURNCODE_SUCCESS;
  } else if (command_return.type == TOCK_SYSCALL_FAILURE) {
    return tock_status_to_returncode(command_return.data[0]);
  } else {
    // The remaining SyscallReturn variants must never happen if using this
    // function. We return `EBADRVAL` to signal an unexpected return variant.
    return RETURNCODE_EBADRVAL;
  }
}

int tock_command_return_u32_to_returncode(syscall_return_t command_return, uint32_t* val) {
  if (command_return.type == TOCK_SYSCALL_SUCCESS_U32) {
    *val = command_return.data[0];
    return RETURNCODE_SUCCESS;
  } else if (command_return.type == TOCK_SYSCALL_FAILURE) {
    return tock_status_to_returncode(command_return.data[0]);
  } else {
    // The remaining SyscallReturn variants must never happen if using this
    // function. We return `EBADRVAL` to signal an unexpected return variant.
    return RETURNCODE_EBADRVAL;
  }
}

int tock_subscribe_return_to_returncode(subscribe_return_t subscribe_return) {
  // If the subscribe was successful, easily return SUCCESS.
  if (subscribe_return.success) {
    return RETURNCODE_SUCCESS;
  } else {
    // Not success, so return the proper returncode.
    return tock_status_to_returncode(subscribe_return.status);
  }
}

int tock_allow_rw_return_to_returncode(allow_rw_return_t allow_return) {
  // If the allow was successful, easily return SUCCESS.
  if (allow_return.success) {
    return RETURNCODE_SUCCESS;
  } else {
    // Not success, so return the proper returncode.
    return tock_status_to_returncode(allow_return.status);
  }
}

int tock_allow_ro_return_to_returncode(allow_ro_return_t allow_return) {
  // If the allow was successful, easily return SUCCESS.
  if (allow_return.success) {
    return RETURNCODE_SUCCESS;
  } else {
    // Not success, so return the proper returncode.
    return tock_status_to_returncode(allow_return.status);
  }
}

void yield_for(bool *cond) {
  while (!*cond) {
    yield();
  }
}

// Returns 1 if a task is processed, 0 otherwise
int yield_check_tasks(void) {
  if (task_cur != task_last) {
    tock_task_t task = task_queue[task_cur];
    task_cur = (task_cur + 1) % TASK_QUEUE_SIZE;
    task.cb(task.arg0, task.arg1, task.arg2, task.ud);
    return 1;
  } else {
    return 0;
  }
}

#if defined(__thumb__)


void yield(void) {
  if (yield_check_tasks()) {
    return;
  } else {
    // Note: A process stops yielding when there is a callback ready to run,
    // which the kernel executes by modifying the stack frame pushed by the
    // hardware. The kernel copies the PC value from the stack frame to the LR
    // field, and sets the PC value to callback to run. When this frame is
    // unstacked during the interrupt return, the effectively clobbers the LR
    // register.
    //
    // At this point, the callback function is now executing, which may itself
    // clobber any of the other caller-saved registers. Thus we mark this
    // inline assembly as conservatively clobbering all caller-saved registers,
    // forcing yield to save any live registers.
    //
    // Upon direct observation of this function, the LR is the only register
    // that is live across the SVC invocation, however, if the yield call is
    // inlined, it is possible that the LR won't be live at all (commonly seen
    // for the `while (1) { yield(); }` idiom) or that other registers are
    // live, thus it is important to let the compiler do the work here.
    //
    // According to the AAPCS: A subroutine must preserve the contents of the
    // registers r4-r8, r10, r11 and SP (and r9 in PCS variants that designate
    // r9 as v6) As our compilation flags mark r9 as the PIC base register, it
    // does not need to be saved. Thus we must clobber r0-3, r12, and LR
    register uint32_t wait __asm__ ("r0")       = 1; // yield-wait
    register uint32_t wait_field __asm__ ("r1") = 0; // yield result ptr
    __asm__ volatile (
      "svc 0       \n"
      :
      : "r" (wait), "r" (wait_field)
      : "memory", "r2", "r3", "r12", "lr"
      );
  }
}

int yield_no_wait(void) {
  if (yield_check_tasks()) {
    return 1;
  } else {
    // Note: A process stops yielding when there is a callback ready to run,
    // which the kernel executes by modifying the stack frame pushed by the
    // hardware. The kernel copies the PC value from the stack frame to the LR
    // field, and sets the PC value to callback to run. When this frame is
    // unstacked during the interrupt return, the effectively clobbers the LR
    // register.
    //
    // At this point, the callback function is now executing, which may itself
    // clobber any of the other caller-saved registers. Thus we mark this
    // inline assembly as conservatively clobbering all caller-saved registers,
    // forcing yield to save any live registers.
    //
    // Upon direct observation of this function, the LR is the only register
    // that is live across the SVC invocation, however, if the yield call is
    // inlined, it is possible that the LR won't be live at all (commonly seen
    // for the `while (1) { yield(); }` idiom) or that other registers are
    // live, thus it is important to let the compiler do the work here.
    //
    // According to the AAPCS: A subroutine must preserve the contents of the
    // registers r4-r8, r10, r11 and SP (and r9 in PCS variants that designate
    // r9 as v6) As our compilation flags mark r9 as the PIC base register, it
    // does not need to be saved. Thus we must clobber r0-3, r12, and LR
    uint8_t result = 0;
    register uint32_t wait __asm__ ("r0")       = 0; // yield-no-wait
    register uint8_t* wait_field __asm__ ("r1") = &result; // yield result ptr
    __asm__ volatile (
      "svc 0       \n"
      :
      : "r" (wait), "r" (wait_field)
      : "memory", "r2", "r3", "r12", "lr"
      );
    return (int)result;
  }
}

void tock_exit(uint32_t completion_code) {
  register uint32_t r0 __asm__ ("r0") = 0; // Terminate
  register uint32_t r1 __asm__ ("r1") = completion_code;
  __asm__ volatile (
    "svc 6"
    :
    : "r" (r0), "r" (r1)
    : "memory");
  __builtin_unreachable();
}


void tock_restart(uint32_t completion_code) {
  register uint32_t r0 __asm__ ("r0") = 1; // Restart
  register uint32_t r1 __asm__ ("r1") = completion_code;
  __asm__ volatile (
    "svc 6"
    :
    : "r" (r0), "r" (r1)
    : "memory");
  __builtin_unreachable();
}

subscribe_return_t subscribe(uint32_t driver, uint32_t subscribe,
                             subscribe_upcall cb, void* userdata) {
  register uint32_t r0 __asm__ ("r0") = driver;
  register uint32_t r1 __asm__ ("r1") = subscribe;
  register void*    r2 __asm__ ("r2") = cb;
  register void*    r3 __asm__ ("r3") = userdata;
  register int rtype __asm__ ("r0");
  register int rv1 __asm__ ("r1");
  register int rv2 __asm__ ("r2");
  register int rv3 __asm__ ("r3");
  __asm__ volatile (
    "svc 1"
    : "=r" (rtype), "=r" (rv1), "=r" (rv2), "=r" (rv3)
    : "r" (r0), "r" (r1), "r" (r2), "r" (r3)
    : "memory");

  if (rtype == TOCK_SYSCALL_SUCCESS_U32_U32) {
    subscribe_return_t rval = {true, (subscribe_upcall*)rv1, (void*)rv2, 0};
    return rval;
  } else if (rtype == TOCK_SYSCALL_FAILURE_U32_U32) {
    subscribe_return_t rval = {false, (subscribe_upcall*)rv2, (void*)rv3, (statuscode_t)rv1};
    return rval;
  } else {
    exit(1);
  }
}

syscall_return_t command(uint32_t driver, uint32_t command,
                         int arg1, int arg2) {
  register uint32_t r0 __asm__ ("r0") = driver;
  register uint32_t r1 __asm__ ("r1") = command;
  register uint32_t r2 __asm__ ("r2") = arg1;
  register uint32_t r3 __asm__ ("r3") = arg2;
  register uint32_t rtype __asm__ ("r0");
  register uint32_t rv1 __asm__ ("r1");
  register uint32_t rv2 __asm__ ("r2");
  register uint32_t rv3 __asm__ ("r3");
  __asm__ volatile (
    "svc 2"
    : "=r" (rtype), "=r" (rv1), "=r" (rv2), "=r" (rv3)
    : "r" (r0), "r" (r1), "r" (r2), "r" (r3)
    : "memory"
    );
  syscall_return_t rval = {rtype, {rv1, rv2, rv3}};
  return rval;
}

allow_ro_return_t allow_readonly(uint32_t driver, uint32_t allow, const void* ptr, size_t size) {
  register uint32_t r0 __asm__ ("r0")       = driver;
  register uint32_t r1 __asm__ ("r1")       = allow;
  register const void*    r2 __asm__ ("r2") = ptr;
  register size_t r3 __asm__ ("r3")         = size;
  register int rtype __asm__ ("r0");
  register int rv1 __asm__ ("r1");
  register int rv2 __asm__ ("r2");
  register int rv3 __asm__ ("r3");
  __asm__ volatile (
    "svc 4"
    : "=r" (rtype), "=r" (rv1), "=r" (rv2), "=r" (rv3)
    : "r" (r0), "r" (r1), "r" (r2), "r" (r3)
    : "memory"
    );
  if (rtype == TOCK_SYSCALL_SUCCESS_U32_U32) {
    allow_ro_return_t rv = {true, (const void*)rv1, (size_t)rv2, 0};
    return rv;
  } else if (rtype == TOCK_SYSCALL_FAILURE_U32_U32) {
    allow_ro_return_t rv = {false, (const void*)rv2, (size_t)rv3, (statuscode_t)rv1};
    return rv;
  } else {
    // Invalid return type
    exit(1);
  }
}

allow_rw_return_t allow_readwrite(uint32_t driver, uint32_t allow, void* ptr, size_t size) {
  register uint32_t r0 __asm__ ("r0")       = driver;
  register uint32_t r1 __asm__ ("r1")       = allow;
  register const void*    r2 __asm__ ("r2") = ptr;
  register size_t r3 __asm__ ("r3")         = size;
  register int rtype __asm__ ("r0");
  register int rv1 __asm__ ("r1");
  register int rv2 __asm__ ("r2");
  register int rv3 __asm__ ("r3");
  __asm__ volatile (
    "svc 3"
    : "=r" (rtype), "=r" (rv1), "=r" (rv2), "=r" (rv3)
    : "r" (r0), "r" (r1), "r" (r2), "r" (r3)
    : "memory"
    );
  if (rtype == TOCK_SYSCALL_SUCCESS_U32_U32) {
    allow_rw_return_t rv = {true, (void*)rv1, (size_t)rv2, 0};
    return rv;
  } else if (rtype == TOCK_SYSCALL_FAILURE_U32_U32) {
    allow_rw_return_t rv = {false, (void*)rv2, (size_t)rv3, (statuscode_t)rv1};
    return rv;
  } else {
    // Invalid return type
    exit(1);
  }
}


allow_userspace_r_return_t allow_userspace_read(uint32_t driver,
                                                uint32_t allow, void* ptr,
                                                size_t size) {
  register uint32_t r0 __asm__ ("r0")       = driver;
  register uint32_t r1 __asm__ ("r1")       = allow;
  register const void*    r2 __asm__ ("r2") = ptr;
  register size_t r3 __asm__ ("r3")         = size;
  register int rtype __asm__ ("r0");
  register int rv1 __asm__ ("r1");
  register int rv2 __asm__ ("r2");
  register int rv3 __asm__ ("r3");
  __asm__ volatile (
    "svc 7"
    : "=r" (rtype), "=r" (rv1), "=r" (rv2), "=r" (rv3)
    : "r" (r0), "r" (r1), "r" (r2), "r" (r3)
    : "memory"
    );
  if (rtype == TOCK_SYSCALL_SUCCESS_U32_U32) {
    allow_userspace_r_return_t rv = {true, (void*)rv1, (size_t)rv2, 0};
    return rv;
  } else if (rtype == TOCK_SYSCALL_FAILURE_U32_U32) {
    allow_userspace_r_return_t rv = {false, (void*)rv2, (size_t)rv3, (statuscode_t)rv1};
    return rv;
  } else {
    // Invalid return type
    exit(-1);
  }
}

memop_return_t memop(uint32_t op_type, int arg1) {
  register uint32_t r0 __asm__ ("r0") = op_type;
  register int r1 __asm__ ("r1")      = arg1;
  register uint32_t val __asm__ ("r1");
  register uint32_t code __asm__ ("r0");
  __asm__ volatile (
    "svc 5"
    : "=r" (code), "=r" (val)
    : "r" (r0), "r" (r1)
    : "memory"
    );
  if (code == TOCK_SYSCALL_SUCCESS) {
    memop_return_t rv = {TOCK_STATUSCODE_SUCCESS, 0};
    return rv;
  } else if (code == TOCK_SYSCALL_SUCCESS_U32) {
    memop_return_t rv = {TOCK_STATUSCODE_SUCCESS, val};
    return rv;
  } else if (code == TOCK_SYSCALL_FAILURE) {
    memop_return_t rv = {(statuscode_t) val, 0};
    return rv;
  } else {
    // Invalid return type
    exit(1);
  }
}

#elif defined(__riscv)

// Implementation of the syscalls for generic RISC-V platforms.
//
// For RISC-V, the arguments are passed through registers a0-a4. Generally,
// the syscall number is put in a4, and the required arguments are specified in
// a0-a3. Nothing specifically syscall related is pushed to the process stack.

void yield(void) {
  if (yield_check_tasks()) {
    return;
  } else {
    register uint32_t a0  __asm__ ("a0")        = 1; // yield-wait
    register uint32_t wait_field __asm__ ("a1") = 0; // yield result ptr
    __asm__ volatile (
      "li       a4, 0\n"
      "ecall\n"
      :
      : "r" (a0), "r" (wait_field)
      : "memory", "a2", "a3", "a4", "a5", "a6", "a7",
      "t0", "t1", "t2", "t3", "t4", "t5", "t6", "ra"
      );

  }
}

int yield_no_wait(void) {
  if (yield_check_tasks()) {
    return 1;
  } else {
    uint8_t result = 0;
    register uint32_t a0  __asm__ ("a0") = 0; // yield-no-wait
    register uint8_t* a1  __asm__ ("a1") = &result;
    __asm__ volatile (
      "li       a4, 0\n"
      "ecall\n"
      :
      : "r" (a0), "r" (a1)
      : "memory", "a2", "a3", "a4", "a5", "a6", "a7",
      "t0", "t1", "t2", "t3", "t4", "t5", "t6", "ra"
      );
    return (int)result;
  }
}


void tock_restart(uint32_t completion_code) {
  register uint32_t a0  __asm__ ("a0") = 1; // exit-restart
  register uint32_t a1  __asm__ ("a1") = completion_code;
  register uint32_t a4  __asm__ ("a4") = 6;
  __asm__ volatile (
    "ecall\n"
    :
    : "r" (a0), "r" (a1), "r" (a4)
    : "memory");
  __builtin_unreachable();
}

void tock_exit(uint32_t completion_code) {
  register uint32_t a0  __asm__ ("a0") = 0; // exit-terminate
  register uint32_t a1  __asm__ ("a1") = completion_code;
  register uint32_t a4  __asm__ ("a4") = 6;
  __asm__ volatile (
    "ecall\n"
    :
    : "r" (a0), "r" (a1), "r" (a4)
    : "memory");
  __builtin_unreachable();
}

subscribe_return_t subscribe(uint32_t driver, uint32_t subscribe,
                             subscribe_upcall uc, void* userdata) {
  register uint32_t a0  __asm__ ("a0") = driver;
  register uint32_t a1  __asm__ ("a1") = subscribe;
  register void*    a2  __asm__ ("a2") = uc;
  register void*    a3  __asm__ ("a3") = userdata;
  register uint32_t a4  __asm__ ("a4") = 1;
  register int rtype __asm__ ("a0");
  register int rv1 __asm__ ("a1");
  register int rv2 __asm__ ("a2");
  register int rv3 __asm__ ("a3");
  __asm__ volatile (
    "ecall\n"
    : "=r" (rtype), "=r" (rv1), "=r" (rv2), "=r" (rv3)
    : "r" (a0), "r" (a1), "r" (a2), "r" (a3), "r" (a4)
    : "memory");
  if (rtype == TOCK_SYSCALL_SUCCESS_U32_U32) {
    subscribe_return_t rval = {true, (subscribe_upcall*)rv1, (void*)rv2, 0};
    return rval;
  } else if (rtype == TOCK_SYSCALL_FAILURE_U32_U32) {
    subscribe_return_t rval = {false, (subscribe_upcall*)rv2, (void*)rv3, (statuscode_t)rv1};
    return rval;
  } else {
    exit(1);
  }
}

syscall_return_t command(uint32_t driver, uint32_t command,
                         int arg1, int arg2) {
  register uint32_t a0  __asm__ ("a0") = driver;
  register uint32_t a1  __asm__ ("a1") = command;
  register uint32_t a2  __asm__ ("a2") = arg1;
  register uint32_t a3  __asm__ ("a3") = arg2;
  register uint32_t a4  __asm__ ("a4") = 2;
  register int rtype __asm__ ("a0");
  register int rv1 __asm__ ("a1");
  register int rv2 __asm__ ("a2");
  register int rv3 __asm__ ("a3");
  __asm__ volatile (
    "ecall\n"
    : "=r" (rtype), "=r" (rv1), "=r" (rv2), "=r" (rv3)
    : "r" (a0), "r" (a1), "r" (a2), "r" (a3), "r" (a4)
    : "memory");
  syscall_return_t rval = {rtype, {rv1, rv2, rv3}};
  return rval;
}

allow_rw_return_t allow_readwrite(uint32_t driver, uint32_t allow,
                                  void* ptr, size_t size) {
  register uint32_t a0  __asm__ ("a0") = driver;
  register uint32_t a1  __asm__ ("a1") = allow;
  register void*    a2  __asm__ ("a2") = ptr;
  register size_t a3  __asm__ ("a3")   = size;
  register uint32_t a4  __asm__ ("a4") = 3;
  register int rtype __asm__ ("a0");
  register int rv1  __asm__ ("a1");
  register int rv2  __asm__ ("a2");
  register int rv3  __asm__ ("a3");
  __asm__ volatile (
    "ecall\n"
    : "=r" (rtype), "=r" (rv1), "=r" (rv2), "=r" (rv3)
    : "r" (a0), "r" (a1), "r" (a2), "r" (a3), "r" (a4)
    : "memory");
  if (rtype == TOCK_SYSCALL_SUCCESS_U32_U32) {
    allow_rw_return_t rv = {true, (void*)rv1, (size_t)rv2, 0};
    return rv;
  } else if (rtype == TOCK_SYSCALL_FAILURE_U32_U32) {
    allow_rw_return_t rv = {false, (void*)rv2, (size_t)rv3, (statuscode_t)rv1};
    return rv;
  } else {
    // Invalid return type
    exit(1);
  }
}

allow_userspace_r_return_t allow_userspace_read(uint32_t driver,
                                                uint32_t allow, void* ptr,
                                                size_t size) {
  register uint32_t a0  __asm__ ("a0") = driver;
  register uint32_t a1  __asm__ ("a1") = allow;
  register void*    a2  __asm__ ("a2") = ptr;
  register size_t a3  __asm__ ("a3")   = size;
  register int rtype __asm__ ("a0");
  register int rv1  __asm__ ("a1");
  register int rv2  __asm__ ("a2");
  register int rv3  __asm__ ("a3");
  __asm__ volatile (
    "li    a4, 7\n"
    "ecall\n"
    : "=r" (rtype), "=r" (rv1), "=r" (rv2), "=r" (rv3)
    : "r" (a0), "r" (a1), "r" (a2), "r" (a3)
    : "memory");
  if (rtype == TOCK_SYSCALL_SUCCESS_U32_U32) {
    allow_userspace_r_return_t rv = {true, (void*)rv1, (size_t)rv2, 0};
    return rv;
  } else if (rtype == TOCK_SYSCALL_FAILURE_U32_U32) {
    allow_userspace_r_return_t rv = {false, (void*)rv2, (size_t)rv3, (statuscode_t)rv1};
    return rv;
  } else {
    // Invalid return type
    exit(-1);
  }
}

allow_ro_return_t allow_readonly(uint32_t driver, uint32_t allow,
                                 const void* ptr, size_t size) {
  register uint32_t a0  __asm__ ("a0")    = driver;
  register uint32_t a1  __asm__ ("a1")    = allow;
  register const void* a2  __asm__ ("a2") = ptr;
  register size_t a3  __asm__ ("a3")      = size;
  register uint32_t a4  __asm__ ("a4")    = 4;
  register int rtype __asm__ ("a0");
  register int rv1 __asm__ ("a1");
  register int rv2 __asm__ ("a2");
  register int rv3 __asm__ ("a3");
  __asm__ volatile (
    "ecall\n"
    : "=r" (rtype), "=r" (rv1), "=r" (rv2), "=r" (rv3)
    : "r" (a0), "r" (a1), "r" (a2), "r" (a3), "r" (a4)
    : "memory");
  if (rtype == TOCK_SYSCALL_SUCCESS_U32_U32) {
    allow_ro_return_t rv = {true, (const void*)rv1, (size_t)rv2, 0};
    return rv;
  } else if (rtype == TOCK_SYSCALL_FAILURE_U32_U32) {
    allow_ro_return_t rv = {false, (const void*)rv2, (size_t)rv3, (statuscode_t)rv1};
    return rv;
  } else {
    // Invalid return type
    exit(1);
  }
}

memop_return_t memop(uint32_t op_type, int arg1) {
  register uint32_t a0    __asm__ ("a0") = op_type;
  register int a1         __asm__ ("a1") = arg1;
  register uint32_t a4    __asm__ ("a4") = 5;
  register uint32_t val   __asm__ ("a1");
  register uint32_t code  __asm__ ("a0");
  __asm__ volatile (
    "ecall\n"
    : "=r" (code), "=r" (val)
    : "r" (a0), "r" (a1), "r" (a4)
    : "memory"
    );
  if (code == TOCK_SYSCALL_SUCCESS) {
    memop_return_t rv = {TOCK_STATUSCODE_SUCCESS, 0};
    return rv;
  } else if (code == TOCK_SYSCALL_SUCCESS_U32) {
    memop_return_t rv = {TOCK_STATUSCODE_SUCCESS, val};
    return rv;
  } else if (code == TOCK_SYSCALL_FAILURE) {
    memop_return_t rv = {(statuscode_t) val, 0};
    return rv;
  } else {
    // Invalid return type
    exit(1);
  }
}

#endif

// Returns the address where the process's RAM region starts.
void* tock_app_memory_begins_at(void) {
  memop_return_t ret = memop(2, 0);
  if (ret.status == TOCK_STATUSCODE_SUCCESS) {
    return (void*) ret.data;
  } else {
    return NULL;
  }
}

// Returns the address immediately after the end of the process's RAM region.
void* tock_app_memory_ends_at(void) {
  memop_return_t ret = memop(3, 0);
  if (ret.status == TOCK_STATUSCODE_SUCCESS) {
    return (void*) ret.data;
  } else {
    return NULL;
  }
}

// Returns the address where the process's flash region starts.
void* tock_app_flash_begins_at(void) {
  memop_return_t ret = memop(4, 0);
  if (ret.status == TOCK_STATUSCODE_SUCCESS) {
    return (void*) ret.data;
  } else {
    return NULL;
  }
}

// Returns the address immediately after the end of the process's flash region.
void* tock_app_flash_ends_at(void) {
  memop_return_t ret = memop(5, 0);
  if (ret.status == TOCK_STATUSCODE_SUCCESS) {
    return (void*) ret.data;
  } else {
    return NULL;
  }
}

// Returns the address where the process's grant region (which is memory owned
// by the kernel) begins.
void* tock_app_grant_begins_at(void) {
  memop_return_t ret = memop(6, 0);
  if (ret.status == TOCK_STATUSCODE_SUCCESS) {
    return (void*) ret.data;
  } else {
    return NULL;
  }
}

// Returns the number of writeable flash regions defined in the process's
// header.
int tock_app_number_writeable_flash_regions(void) {
  memop_return_t ret = memop(7, 0);
  if (ret.status == TOCK_STATUSCODE_SUCCESS) {
    return (int) ret.data;
  } else {
    return 0;
  }
}

// Returns the address where the writeable flash region specified by
// `region_index` starts. Returns NULL if the specified writeable flash region
// does not exist.
void* tock_app_writeable_flash_region_begins_at(int region_index) {
  memop_return_t ret = memop(8, region_index);
  if (ret.status == TOCK_STATUSCODE_SUCCESS) {
    return (void*) ret.data;
  } else {
    return NULL;
  }
}

// Returns the address immediately after the writeable flash region specified by
// `region_index` ends. Returns NULL if the specified writeable flash region
// does not exist.
void* tock_app_writeable_flash_region_ends_at(int region_index) {
  memop_return_t ret = memop(9, region_index);
  if (ret.status == TOCK_STATUSCODE_SUCCESS) {
    return (void*) ret.data;
  } else {
    return NULL;
  }
}

bool driver_exists(uint32_t driver) {
  syscall_return_t sval = command(driver, 0, 0, 0);
  // Any success type says the driver exists.
  if (sval.type >= TOCK_SYSCALL_SUCCESS) {
    return true;
  } else {
    return false;
  }
}

const char* tock_strerr(statuscode_t status) {
  return tock_strrcode(tock_status_to_returncode(status));
}

// Convert a ReturnCode to a string.
const char* tock_strrcode(returncode_t returncode) {
  switch (returncode) {
    case RETURNCODE_SUCCESS:
      return "Success";
    case RETURNCODE_FAIL:
      return "Unknown Error";
    case RETURNCODE_EBUSY:
      return "Underlying system is busy; retry";
    case RETURNCODE_EALREADY:
      return "The state requested is already set";
    case RETURNCODE_EOFF:
      return "The component is powered down";
    case RETURNCODE_ERESERVE:
      return "Reservation required before use";
    case RETURNCODE_EINVAL:
      return "An invalid parameter was passed";
    case RETURNCODE_ESIZE:
      return "Parameter passed was too large";
    case RETURNCODE_ECANCEL:
      return "Operation cancelled by a call";
    case RETURNCODE_ENOMEM:
      return "Memory required not available";
    case RETURNCODE_ENOSUPPORT:
      return "Operation or command is unsupported";
    case RETURNCODE_ENODEVICE:
      return "Device does not exist";
    case RETURNCODE_EUNINSTALLED:
      return "Device is not physically installed";
    case RETURNCODE_ENOACK:
      return "Packet transmission not acknowledged";
    case RETURNCODE_EBADRVAL:
      return "Invalid SyscallReturn variant";
  }
  return "Invalid error number";
}

void tock_expect(int expected, int actual, const char* file, unsigned line) {
  if (expected != actual) {
    printf("Expectation failure in \"%s\" at line %u\n", file, line);
    printf("Expected value: %d\n", expected);
    printf(" But got value: %d (possible error: %s)\n", actual, tock_strrcode(actual));
    exit(1);
  }
}
