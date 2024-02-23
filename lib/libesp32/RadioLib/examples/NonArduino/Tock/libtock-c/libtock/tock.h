#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (subscribe_upcall)(int, int, int, void*);

////////////////////////////////////////////////////////////////////////////////
///
/// RETURN AND ERROR TYPES
///
////////////////////////////////////////////////////////////////////////////////

// `SyscallReturn` variants. These identify the structure of the syscall return
// type.
//
// There are multiple failure and success versions based on how many and the
// size of any included values.
typedef enum {
  TOCK_SYSCALL_FAILURE             =   0,
  TOCK_SYSCALL_FAILURE_U32         =   1,
  TOCK_SYSCALL_FAILURE_U32_U32     =   2,
  TOCK_SYSCALL_FAILURE_U64         =   3,
  TOCK_SYSCALL_SUCCESS             = 128,
  TOCK_SYSCALL_SUCCESS_U32         = 129,
  TOCK_SYSCALL_SUCCESS_U32_U32     = 130,
  TOCK_SYSCALL_SUCCESS_U64         = 131,
  TOCK_SYSCALL_SUCCESS_U32_U32_U32 = 132,
  TOCK_SYSCALL_SUCCESS_U32_U64     = 133
} syscall_rtype_t;

// ReturnCode type in libtock-c.
//
// 0 is success, and a negative value is an error (consistent with C
// conventions). The error cases are -1*ErrorCode values.
typedef enum {
  RETURNCODE_SUCCESS      = 0,
  RETURNCODE_FAIL         = -1,
  RETURNCODE_EBUSY        = -2,
  RETURNCODE_EALREADY     = -3,
  RETURNCODE_EOFF         = -4,
  RETURNCODE_ERESERVE     = -5,
  RETURNCODE_EINVAL       = -6,
  RETURNCODE_ESIZE        = -7,
  RETURNCODE_ECANCEL      = -8,
  RETURNCODE_ENOMEM       = -9,
  RETURNCODE_ENOSUPPORT   = -10,
  RETURNCODE_ENODEVICE    = -11,
  RETURNCODE_EUNINSTALLED = -12,
  RETURNCODE_ENOACK       = -13,
  RETURNCODE_EBADRVAL     = -1024
} returncode_t;

// StatusCode from the kernel. Uses same mapping for errors as ErrorCode, but
// includes a success case with identifier 0.
typedef enum {
  TOCK_STATUSCODE_SUCCESS     = 0,
  TOCK_STATUSCODE_FAIL        = 1,
  TOCK_STATUSCODE_BUSY        = 2,
  TOCK_STATUSCODE_ALREADY     = 3,
  TOCK_STATUSCODE_OFF         = 4,
  TOCK_STATUSCODE_RESERVE     = 5,
  TOCK_STATUSCODE_INVAL       = 6,
  TOCK_STATUSCODE_SIZE        = 7,
  TOCK_STATUSCODE_CANCEL      = 8,
  TOCK_STATUSCODE_NOMEM       = 9,
  TOCK_STATUSCODE_NOSUPPORT   = 10,
  TOCK_STATUSCODE_NODEVICE    = 11,
  TOCK_STATUSCODE_UNINSTALLED = 12,
  TOCK_STATUSCODE_NOACK       = 13,
} statuscode_t;

// Generic return structure from a system call.
typedef struct {
  syscall_rtype_t type;
  uint32_t data[3];
} syscall_return_t;

// Return structure from a subscribe syscall. The `subscribe()` implementation
// automatically converts to this return type.
typedef struct {
  bool success;
  subscribe_upcall* callback;
  void* userdata;
  statuscode_t status;
} subscribe_return_t;

// Return structure from an allow read-write syscall. The syscall implementation
// does the conversion into this type.
typedef struct {
  bool success;
  void* ptr;
  size_t size;
  statuscode_t status;
} allow_rw_return_t;

// Return structure from an allow read-only syscall. The syscall implementation
// does the conversion into this type.
typedef struct {
  bool success;
  const void* ptr;
  size_t size;
  statuscode_t status;
} allow_ro_return_t;

// Return structure from an userspace readable allow syscall. The syscall
// implementation does the conversion into this type.
typedef struct {
  bool success;
  void* ptr;
  size_t size;
  statuscode_t status;
} allow_userspace_r_return_t;

// Return structure from a memop syscall. The syscall implementation does the
// conversion into this type.
typedef struct {
  // Returned statuscode from syscall.
  statuscode_t status;
  // Optional return data depending on the memop variant called. Only set if
  // status is `TOCK_STATUSCODE_SUCCESS`.
  uint32_t data;
} memop_return_t;

////////////////////////////////////////////////////////////////////////////////
///
/// HELPER FUNCTIONS
///
////////////////////////////////////////////////////////////////////////////////

// Convert a kernel StatusCode to the libtock-c ReturnCode. ReturnCodes are used
// in libtock-c because they follow the C convention of 0 as success and
// negative numbers as errors.
int tock_status_to_returncode(statuscode_t);

// Convert a `syscall_return_t` with no values to a `returncode_t`.
//
// This expects no values to be returned (i.e. the only success case is
// `TOCK_SYSCALL_SUCCESS`). Do not use with other expected SyscallReturn
// variants.
int tock_command_return_novalue_to_returncode(syscall_return_t);

// Convert a `syscall_return_t` with one u32 to a `returncode_t`.
//
// This expects exactly one u32 to be returned (i.e. the only success case is
// `TOCK_SYSCALL_SUCCESS_U32`). Do not use with other expected SyscallReturn
// variants.
int tock_command_return_u32_to_returncode(syscall_return_t, uint32_t*);

// Convert a `subscribe_return_t` to a `returncode_t`.
int tock_subscribe_return_to_returncode(subscribe_return_t);

// Convert a `allow_rw_return_t` to a `returncode_t`.
int tock_allow_rw_return_to_returncode(allow_rw_return_t);

// Convert a `allow_ro_return_t` to a `returncode_t`.
int tock_allow_ro_return_to_returncode(allow_ro_return_t);

int tock_enqueue(subscribe_upcall cb, int arg0, int arg1, int arg2, void* ud);

int yield_check_tasks(void);
void yield(void);
void yield_for(bool*);
int yield_no_wait(void);

void tock_exit(uint32_t completion_code) __attribute__ ((noreturn));
void tock_restart(uint32_t completion_code) __attribute__ ((noreturn));

__attribute__ ((warn_unused_result))
syscall_return_t command(uint32_t driver, uint32_t command, int arg1, int arg2);

// Pass this to the subscribe syscall as a function pointer to
// be the Null Upcall.
#define TOCK_NULL_UPCALL 0

__attribute__ ((warn_unused_result))
subscribe_return_t subscribe(uint32_t driver, uint32_t subscribe, subscribe_upcall uc, void* userdata);

__attribute__ ((warn_unused_result))
allow_rw_return_t allow_readwrite(uint32_t driver, uint32_t allow, void* ptr, size_t size);

__attribute__ ((warn_unused_result))
allow_userspace_r_return_t allow_userspace_read(uint32_t driver,
                                                uint32_t allow, void* ptr,
                                                size_t size);

__attribute__ ((warn_unused_result))
allow_ro_return_t allow_readonly(uint32_t driver, uint32_t allow, const void* ptr, size_t size);

// Call the memop syscall.
memop_return_t memop(uint32_t op_type, int arg1);

// Wrappers around memop to support app introspection
void* tock_app_memory_begins_at(void);
void* tock_app_memory_ends_at(void);
void* tock_app_flash_begins_at(void);
void* tock_app_flash_ends_at(void);
void* tock_app_grant_begins_at(void);
int tock_app_number_writeable_flash_regions(void);
void* tock_app_writeable_flash_region_begins_at(int region_index);
void* tock_app_writeable_flash_region_ends_at(int region_index);


// Checks to see if the given driver number exists on this platform.
bool driver_exists(uint32_t driver);


const char* tock_strerr(statuscode_t status);
const char* tock_strrcode(returncode_t returncode);

void tock_expect(int expected, int actual, const char* file, unsigned line);
#define TOCK_EXPECT(_e, _a) tock_expect((_e), (_a), __FILE__, __LINE__)

#ifdef __cplusplus
}
#endif
