#include "read_only_state.h"

int read_only_state_get_version(void) {
  syscall_return_t res = command(READ_ONLY_STATEDRIVER_NUM, 0, 0, 0);

  if (res.type != TOCK_SYSCALL_SUCCESS_U32) {
    return RETURNCODE_ENOSUPPORT;
  } else {
    return res.data[0];
  }
}

int read_only_state_allocate_region(void* base, int len) {
  allow_userspace_r_return_t buf;

  if (len < READ_ONLY_STATEBUFFER_LEN) {
    // The buffer is not long enough
    return RETURNCODE_ESIZE;
  }

  buf = allow_userspace_read(READ_ONLY_STATEDRIVER_NUM, 0, base, len);

  if (!buf.success) {
    return tock_status_to_returncode(buf.status);
  }
  return RETURNCODE_SUCCESS;
}

uint32_t read_only_state_get_pending_tasks(void* base) {
  uint32_t* ptr = base;
  return ptr[1];
}

uint64_t read_only_state_get_ticks(void* base) {
  uint32_t* ptr = base;

  // Start with the high bytes set to 0
  uint32_t high, low;

  do {
    // Get the high bytes the value in memory
    high = ptr[3];
    // Read the low bytes
    low = ptr[2];
    // If the high bytes don't match what is still in memory re-try
    // the load
  } while (high != ptr[3]);

  return ((uint64_t)high << 32) | low;
}

int read_only_state_quick_yield(void* base) {
  if (yield_check_tasks()) {
    return 1;
  } else {
    uint32_t tasks = read_only_state_get_pending_tasks(base);

    if (tasks > 0) {
      // Waiting tasks, call yield
      yield();
    }

    return tasks;
  }
}
