#pragma once

#include <stdlib.h>
#include <string.h>

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define IPC_DRIVER_NUM 0x10000

// Performs service discovery
//
// Retrieves the process identifier of the process with the given package name,
// or a negative value on error.
int ipc_discover(const char* pkg_name, size_t* svc_id);

// Registers a service callback for this process.
//
// Service callbacks are called in response to `notify`s from clients and take
// the following arguments in order:
//
//   pkg_name  - the package name of this service
//   callback  - the address callback function to execute when clients notify
//   void* ud  - `userdata`. data passed to callback function
int ipc_register_service_callback(const char *pkg_name,
                                  subscribe_upcall callback, void *ud);

// Registers a client callback for a particular service.
//
// `svc_id` is the (non-zero) process id of the service to subscribe to.
//
// Client callbacks are called in response to `notify`s from a particular
// service and take the following arguments in order:
//
//   size_t pid - the notifying service's process id
//   int len    - the length of the shared buffer or zero if no buffer is
//                shared from the service.
//   char* buf  - the base address of the shared buffer, or NULL if no buffer
//                is shared from the service.
//   void* ud   - `userdata`. same as the argument to this function.
int ipc_register_client_callback(size_t svc_id, subscribe_upcall callback, void *ud);

// Send a notify to the client at the given process id
int ipc_notify_client(size_t pid);

// Send a notify to the service at the given process id
int ipc_notify_service(size_t pid);

// Share a buffer with the given process (either service or client)
//
// `pid` is the non-zero process id of the recipient.
// `base` must be aligned to the value of `len`.
// `len` must be a power-of-two larger than 16.
int ipc_share(size_t pid, void* base, int len);

#ifdef __cplusplus
}
#endif
