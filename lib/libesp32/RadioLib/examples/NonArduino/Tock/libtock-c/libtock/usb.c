#include "usb.h"

int usb_exists(void) {
  return driver_exists(DRIVER_NUM_USB);
}

int usb_subscribe(subscribe_upcall upcall, void *ud) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_USB, 0, upcall, ud);
  return tock_subscribe_return_to_returncode(sval);
}

int usb_enable_and_attach_async(void) {
  syscall_return_t cval = command(DRIVER_NUM_USB, 1, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

struct data {
  bool fired;
  int rcode;
};

static void callback(int                         status,
                     __attribute__((unused)) int v1,
                     __attribute__((unused)) int v2,
                     void *                      data)
{
  struct data *d = data;
  d->fired = true;
  d->rcode = tock_status_to_returncode(status);
}

int usb_enable_and_attach(void)
{
  int err;

  struct data d = { .fired = false };

  err = usb_subscribe(callback, (void *) &d);
  if (err < 0) return err;

  err = usb_enable_and_attach_async();
  if (err < 0) return err;

  yield_for(&d.fired);

  return d.rcode;
}
