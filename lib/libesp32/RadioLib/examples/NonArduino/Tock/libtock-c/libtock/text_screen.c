#include "text_screen.h"
#include "tock.h"
#include <stdlib.h>

typedef struct {
  int error;
  int data1;
  int data2;
  bool done;
} TextScreenReturn;

static void text_screen_callback(int   status,
                                 int   data1,
                                 int   data2,
                                 void* ud) {
  TextScreenReturn *ret = (TextScreenReturn*) ud;
  ret->error = tock_status_to_returncode(status);
  ret->data1 = data1;
  ret->data2 = data2;
  ret->done  = true;
}

static uint8_t *buffer   = NULL;
static size_t buffer_len = 0;

static int text_screen_subscribe (subscribe_upcall cb, void *userdata) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_TEXT_SCREEN, 0, cb, userdata);
  return tock_subscribe_return_to_returncode(sval);
}

static int text_screen_command (int command_num, int data1, int data2) {
  syscall_return_t cval = command(DRIVER_NUM_TEXT_SCREEN, command_num, data1, data2);
  return tock_command_return_novalue_to_returncode(cval);
}

static int text_screen_allow (const void* ptr, size_t size) {
  allow_ro_return_t aval = allow_readonly(DRIVER_NUM_TEXT_SCREEN, 0, ptr, size);
  return tock_allow_ro_return_to_returncode(aval);
}

int text_screen_init (size_t len)
{
  int r = RETURNCODE_SUCCESS;
  if (buffer != NULL) {
    r = RETURNCODE_EALREADY;
  } else {
    buffer = (uint8_t*) calloc(1, len);
    if (buffer != NULL) {
      buffer_len = len;
      r = text_screen_allow(buffer, len);
    } else {
      r = RETURNCODE_FAIL;
    }
  }
  return r;
}

uint8_t* text_screen_buffer (void)
{
  return buffer;
}

int text_screen_display_on (void)
{
  TextScreenReturn ret;
  ret.done = false;

  int err = text_screen_subscribe(text_screen_callback, &ret);
  if (err < 0) return err;

  err = text_screen_command(2, 0, 0);
  if (err < 0) return err;

  yield_for(&ret.done);

  return ret.error;
}

int text_screen_display_off (void)
{
  TextScreenReturn ret;
  ret.done = false;

  int err = text_screen_subscribe(text_screen_callback, &ret);
  if (err < 0) return err;

  err = text_screen_command(3, 0, 0);
  if (err < 0) return err;

  yield_for(&ret.done);

  return ret.error;
}

int text_screen_blink_on (void)
{
  TextScreenReturn ret;
  ret.done = false;

  int err = text_screen_subscribe(text_screen_callback, &ret);
  if (err < 0) return err;

  err = text_screen_command(4, 0, 0);
  if (err < 0) return err;

  yield_for(&ret.done);

  return ret.error;
}

int text_screen_blink_off (void)
{
  TextScreenReturn ret;
  ret.done = false;

  int err = text_screen_subscribe(text_screen_callback, &ret);
  if (err < 0) return err;

  err = text_screen_command(5, 0, 0);

  yield_for(&ret.done);

  return ret.error;
}

int text_screen_show_cursor (void)
{
  TextScreenReturn ret;
  ret.done = false;

  int err = text_screen_subscribe(text_screen_callback, &ret);
  if (err < 0) return err;

  err = text_screen_command(6, 0, 0);
  if (err < 0) return err;

  yield_for(&ret.done);

  return ret.error;
}

int text_screen_hide_cursor (void)
{
  TextScreenReturn ret;
  ret.done = false;

  int err = text_screen_subscribe(text_screen_callback, &ret);
  if (err < 0) return err;

  err = text_screen_command(7, 0, 0);
  if (err < 0) return err;

  yield_for(&ret.done);

  return ret.error;
}

int text_screen_clear (void)
{
  TextScreenReturn ret;
  ret.done = false;

  int err = text_screen_subscribe(text_screen_callback, &ret);
  if (err < 0) return err;

  err = text_screen_command(9, 0, 0);
  if (err < 0) return err;

  yield_for(&ret.done);

  return ret.error;
}

int text_screen_home (void)
{
  TextScreenReturn ret;
  ret.done = false;

  int err = text_screen_subscribe(text_screen_callback, &ret);
  if (err < 0) return err;

  err = text_screen_command(10, 0, 0);
  if (err < 0) return err;

  yield_for(&ret.done);

  return ret.error;
}

int text_screen_set_cursor (uint8_t col, uint8_t row)
{
  TextScreenReturn ret;
  ret.done = false;

  int err = text_screen_subscribe(text_screen_callback, &ret);
  if (err < 0) return err;

  err = text_screen_command(11, col, row);
  if (err < 0) return err;

  yield_for(&ret.done);

  return ret.error;
}

int text_screen_write (size_t len)
{
  TextScreenReturn ret;
  ret.done = false;

  int err = text_screen_subscribe(text_screen_callback, &ret);
  if (err < 0) return err;

  err = text_screen_command(8, len, 0);
  if (err < 0) return err;

  yield_for(&ret.done);

  return ret.error;
}

int text_screen_get_size (size_t* width, size_t* height)
{
  TextScreenReturn ret;
  ret.done  = false;
  ret.data1 = 0;
  ret.data2 = 0;

  int err = text_screen_subscribe(text_screen_callback, &ret);
  if (err < 0) return err;

  err = text_screen_command(1, 0, 0);
  if (err < 0) return err;

  yield_for(&ret.done);

  *width  = ret.data1;
  *height = ret.data2;

  return ret.error;
}
