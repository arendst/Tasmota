#include "touch.h"
#include <stdlib.h>

static int touch_subscribe(int subscribe_num, subscribe_upcall callback, void* callback_args) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_TOUCH, subscribe_num, callback, callback_args);
  return tock_subscribe_return_to_returncode(sval);
}

static int touch_allow(int allow_num, void* data, int len) {
  allow_rw_return_t aval = allow_readwrite(DRIVER_NUM_TOUCH, allow_num, data, len);
  return tock_allow_rw_return_to_returncode(aval);
}

static touch_t *multi_touch_buffer = NULL;
static unsigned char num_touches   = 0;

static touch_callback *single_touch_upcall = NULL;
static gesture_callback *gesture_upcall    = NULL;

static void touch_single_touch_callback (int status, int xy, int data2 __attribute__((unused)), void *ud) {
  if (single_touch_upcall) single_touch_upcall(status, ((unsigned int)xy >> 16), (unsigned int)xy & 0xFFFF, ud);
}

static void touch_gesture_callback (int gesture, int data1 __attribute__((unused)), int data2 __attribute__(
                                      (unused)), void *ud) {
  if (gesture_upcall) gesture_upcall(gesture, ud);
}

int get_number_of_touches (int* touches) {
  syscall_return_t cval = command(DRIVER_NUM_TOUCH, 100, 0, 0);
  return tock_command_return_u32_to_returncode(cval, (uint32_t*) touches);
}

int enable_single_touch(void) {
  syscall_return_t cval = command(DRIVER_NUM_TOUCH, 1, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

int disable_single_touch(void) {
  syscall_return_t cval = command(DRIVER_NUM_TOUCH, 2, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

int single_touch_set_callback (touch_callback cb, void* ud) {
  single_touch_upcall = cb;
  return touch_subscribe(0, cb != NULL ? touch_single_touch_callback : NULL, ud);
}

int enable_multi_touch(void) {
  syscall_return_t cval = command(DRIVER_NUM_TOUCH, 11, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

int disable_multi_touch(void) {
  syscall_return_t cval = command(DRIVER_NUM_TOUCH, 12, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

int multi_touch_set_callback (touch_callback cb, void* ud, int max_touches) {
  int err = RETURNCODE_SUCCESS;
  if (cb != NULL) {
    if (multi_touch_buffer == NULL) {
      multi_touch_buffer = (touch_t*)malloc(max_touches * sizeof(touch_t));
      if (multi_touch_buffer) {
        num_touches = max_touches;
        err         = touch_allow(2, multi_touch_buffer, max_touches * sizeof(touch_t));
        if (err == RETURNCODE_SUCCESS) {
          err = touch_subscribe(2, cb, ud);
        }
        if (err != RETURNCODE_SUCCESS) {
          free(multi_touch_buffer);
          multi_touch_buffer = NULL;
        }
      } else {
        err = RETURNCODE_ENOMEM;
      }
    } else {
      err = RETURNCODE_EALREADY;
    }
  } else {
    if (multi_touch_buffer != NULL) {
      num_touches = 0;
      touch_allow(2, NULL, 0);
      err = touch_subscribe(2, cb, ud);
      free(multi_touch_buffer);
      multi_touch_buffer = NULL;
    }
  }
  return err;
}

int gesture_set_callback (gesture_callback cb, void* ud) {
  gesture_upcall = cb;
  return touch_subscribe(1, cb != NULL ? touch_gesture_callback : NULL, ud);
}

// get multi touch

int read_touch (int index, unsigned char *id, unsigned char *status, unsigned short *x, unsigned short *y) {
  if (index < num_touches) {
    if (multi_touch_buffer != NULL) {
      *id     = multi_touch_buffer[index].id;
      *status = multi_touch_buffer[index].status;
      *x      = multi_touch_buffer[index].x;
      *y      = multi_touch_buffer[index].y;
      return RETURNCODE_SUCCESS;
    } else {
      return RETURNCODE_ENOMEM;
    }
  } else {
    return RETURNCODE_EINVAL;
  }
}

int read_touch_full (int index, unsigned char *id, unsigned char *status, unsigned short *x, unsigned short *y,
                     unsigned char *size, unsigned char *pressure) {
  if (multi_touch_buffer != NULL) {
    int err = read_touch(index, id, status, x, y);
    if (err == RETURNCODE_SUCCESS) {
      *size     = multi_touch_buffer[index].size;
      *pressure = multi_touch_buffer[index].pressure;
    }
    return err;
  } else {
    return RETURNCODE_ENOMEM;
  }
}

int multi_touch_next (void) {
  syscall_return_t cval = command(DRIVER_NUM_TOUCH, 10, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}
