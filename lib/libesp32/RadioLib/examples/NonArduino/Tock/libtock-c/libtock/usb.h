#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_USB 0x20005

// Does the driver exist?
int usb_exists(void);

// Register a callback to receive asynchronous results
//
// The callback will receive these parameters, in order:
//    status: SUCCESS if all inputs are valid, else EINVAL
int usb_subscribe(subscribe_upcall, void *);

// Enable the USB controller and attach to the bus
//
// Returns EINVAL if usb_subscribe() has not previously been called to register a callback.
// Returns SUCCESS if the callback is present and will be used
// to report completion of this operation.
//
int usb_enable_and_attach_async(void);

int usb_enable_and_attach(void);

#ifdef __cplusplus
}
#endif
