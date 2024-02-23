#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "console.h"

typedef struct putstr_data {
  char* buf;
  int len;
  bool called;
  struct putstr_data* next;
} putstr_data_t;

static putstr_data_t *putstr_head = NULL;
static putstr_data_t *putstr_tail = NULL;

static void putstr_upcall(int   _x __attribute__ ((unused)),
                          int   _y __attribute__ ((unused)),
                          int   _z __attribute__ ((unused)),
                          void* ud __attribute__ ((unused))) {
  putstr_data_t* data = putstr_head;
  data->called = true;
  putstr_head  = data->next;

  if (putstr_head == NULL) {
    putstr_tail = NULL;
  } else {
    int ret;
    ret = putnstr_async(putstr_head->buf, putstr_head->len, putstr_upcall, NULL);
    if (ret < 0) {
      // XXX There's no path to report errors currently, so just drop it
      putstr_upcall(0, 0, 0, NULL);
    }
  }
}

int putnstr(const char *str, size_t len) {
  int ret = RETURNCODE_SUCCESS;

  putstr_data_t* data = (putstr_data_t*)malloc(sizeof(putstr_data_t));
  if (data == NULL) return RETURNCODE_ENOMEM;

  data->len    = len;
  data->called = false;
  data->buf    = (char*)malloc(len * sizeof(char));
  if (data->buf == NULL) {
    ret = RETURNCODE_ENOMEM;
    goto putnstr_fail_buf_alloc;
  }
  strncpy(data->buf, str, len);
  data->next = NULL;

  if (putstr_tail == NULL) {
    // Invariant, if tail is NULL, head is also NULL
    ret = putnstr_async(data->buf, data->len, putstr_upcall, NULL);
    if (ret < 0) goto putnstr_fail_async;
    putstr_head = data;
    putstr_tail = data;
  } else {
    putstr_tail->next = data;
    putstr_tail       = data;
  }

  yield_for(&data->called);

putnstr_fail_async:
  free(data->buf);
putnstr_fail_buf_alloc:
  free(data);

  return ret;
}

int putnstr_async(const char *str, size_t len, subscribe_upcall cb, void* userdata) {
#pragma GCC diagnostic push
#pragma GCC diagnostic pop

  allow_ro_return_t ro = allow_readonly(DRIVER_NUM_CONSOLE, 1, str, len);
  if (!ro.success) {
    return tock_status_to_returncode(ro.status);
  }

  subscribe_return_t sub = subscribe(DRIVER_NUM_CONSOLE, 1, cb, userdata);
  if (!sub.success) {
    return tock_status_to_returncode(sub.status);
  }

  syscall_return_t com = command(DRIVER_NUM_CONSOLE, 1, len, 0);
  return tock_command_return_novalue_to_returncode(com);
}

int getnstr_async(char *buf, size_t len, subscribe_upcall cb, void* userdata) {
  allow_rw_return_t rw = allow_readwrite(DRIVER_NUM_CONSOLE, 1, buf, len);
  if (!rw.success) {
    return tock_status_to_returncode(rw.status);
  }

  subscribe_return_t sub = subscribe(DRIVER_NUM_CONSOLE, 2, cb, userdata);
  if (!sub.success) {
    return tock_status_to_returncode(sub.status);
  }

  syscall_return_t com = command(DRIVER_NUM_CONSOLE, 2, len, 0);
  return tock_command_return_novalue_to_returncode(com);
}

typedef struct getnstr_data {
  bool called;
  int result;
} getnstr_data_t;

static getnstr_data_t getnstr_data = { true, 0 };

static void getnstr_upcall(int   result,
                           int   _y __attribute__ ((unused)),
                           int   _z __attribute__ ((unused)),
                           void* ud __attribute__ ((unused))) {
  getnstr_data.result = result;
  getnstr_data.called = true;
}

int getnstr(char *str, size_t len) {
  int ret;

  if (!getnstr_data.called) {
    // A call is already in progress
    return RETURNCODE_EALREADY;
  }
  getnstr_data.called = false;

  ret = getnstr_async(str, len, getnstr_upcall, NULL);
  if (ret < 0) {
    return ret;
  }

  yield_for(&getnstr_data.called);

  return getnstr_data.result;
}

int getch(void) {
  int r;
  char buf[1];

  r = getnstr(buf, 1);
  return (r == RETURNCODE_SUCCESS) ? buf[0] : RETURNCODE_FAIL;
}

int getnstr_abort(void) {
  syscall_return_t com = command(DRIVER_NUM_CONSOLE, 3, 0, 0);
  return tock_command_return_novalue_to_returncode(com);
}
