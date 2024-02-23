#include "ipc.h"
#include "tock.h"

int ipc_discover(const char* pkg_name, size_t* svc_id) {
  int len = strlen(pkg_name);

  allow_ro_return_t prev = allow_readonly(IPC_DRIVER_NUM, 0, pkg_name, len);
  if (!prev.success) return tock_status_to_returncode(prev.status);

  syscall_return_t res = command(IPC_DRIVER_NUM, 1, 0, 0);
  int ret = tock_command_return_u32_to_returncode(res, (uint32_t*) svc_id);
  if (ret < 0) return ret;

  prev = allow_readonly(IPC_DRIVER_NUM, 0, prev.ptr, prev.size);
  if (!prev.success) return tock_status_to_returncode(prev.status);

  return RETURNCODE_SUCCESS;
}

int ipc_register_service_callback(const char *pkg_name,
                                  subscribe_upcall callback, void *ud) {
  size_t svc_id;

  // Look up the service id so we can subscribe as the service
  int ret = ipc_discover(pkg_name, &svc_id);
  if (ret < 0) return ret;

  subscribe_return_t sval = subscribe(IPC_DRIVER_NUM, svc_id, callback, ud);
  return tock_subscribe_return_to_returncode(sval);
}

int ipc_register_client_callback(size_t svc_id, subscribe_upcall callback, void *ud) {
  subscribe_return_t sval = subscribe(IPC_DRIVER_NUM, svc_id, callback, ud);
  return tock_subscribe_return_to_returncode(sval);
}

int ipc_notify_service(size_t pid) {
  syscall_return_t res = command(IPC_DRIVER_NUM, 2, (int) pid, 0);
  return tock_command_return_novalue_to_returncode(res);
}

int ipc_notify_client(size_t pid) {
  syscall_return_t res = command(IPC_DRIVER_NUM, 3, (int) pid, 0);
  return tock_command_return_novalue_to_returncode(res);
}

int ipc_share(size_t pid, void* base, int len) {
  allow_rw_return_t aval = allow_readwrite(IPC_DRIVER_NUM, (int) pid, base, len);
  return tock_allow_rw_return_to_returncode(aval);
}
