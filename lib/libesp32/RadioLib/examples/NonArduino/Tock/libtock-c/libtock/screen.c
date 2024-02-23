#include "screen.h"
#include "tock.h"
#include <stdlib.h>

typedef struct {
  int error;
  int data1;
  int data2;
  bool done;
} ScreenReturn;

static void screen_callback(int   status,
                            int   data1,
                            int   data2,
                            void* ud) {
  ScreenReturn *fbr = (ScreenReturn*)ud;
  fbr->error = tock_status_to_returncode(status);
  fbr->data1 = data1;
  fbr->data2 = data2;
  fbr->done  = true;
}

static uint8_t *buffer   = NULL;
static size_t buffer_len = 0;

static int screen_subscribe (subscribe_upcall cb, void *userdata) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_SCREEN, 0, cb, userdata);
  return tock_subscribe_return_to_returncode(sval);
}

static int screen_allow (const void* ptr, size_t size) {
  allow_ro_return_t aval = allow_readonly(DRIVER_NUM_SCREEN, 0, ptr, size);
  return tock_allow_ro_return_to_returncode(aval);
}

int screen_get_supported_resolutions (int* resolutions) {
  syscall_return_t com = command(DRIVER_NUM_SCREEN, 11, 0, 0);
  if (com.type == TOCK_SYSCALL_SUCCESS_U32) {
    *resolutions = com.data[0];
    return RETURNCODE_SUCCESS;
  } else if (com.type == TOCK_SYSCALL_FAILURE) {
    return tock_status_to_returncode(com.data[0]);
  } else {
    return RETURNCODE_EINVAL;
  }
}

int screen_get_supported_resolution (size_t index, size_t *width, size_t *height) {
  syscall_return_t com = command(DRIVER_NUM_SCREEN, 12, index, 0);
  if (com.type == TOCK_SYSCALL_SUCCESS_U32_U32) {
    *width  = com.data[0];
    *height = com.data[1];
    return RETURNCODE_SUCCESS;
  } else if (com.type == TOCK_SYSCALL_FAILURE) {
    return tock_status_to_returncode(com.data[0]);
  } else {
    return RETURNCODE_EINVAL;
  }
}

int screen_get_supported_pixel_formats (int* formats) {
  syscall_return_t com = command(DRIVER_NUM_SCREEN, 13, 0, 0);
  if (com.type == TOCK_SYSCALL_SUCCESS_U32) {
    *formats = com.data[0];
    return RETURNCODE_SUCCESS;
  } else if (com.type == TOCK_SYSCALL_FAILURE) {
    return tock_status_to_returncode(com.data[0]);
  } else {
    return RETURNCODE_EINVAL;
  }
}

int screen_get_supported_pixel_format (size_t index, int* format) {
  syscall_return_t com = command(DRIVER_NUM_SCREEN, 14, index, 0);
  if (com.type == TOCK_SYSCALL_SUCCESS_U32) {
    *format = com.data[0];
    return RETURNCODE_SUCCESS;
  } else if (com.type == TOCK_SYSCALL_FAILURE) {
    return tock_status_to_returncode(com.data[0]);
  } else {
    return RETURNCODE_EINVAL;
  }
}

bool screen_setup_enabled (void) {
  syscall_return_t com = command(DRIVER_NUM_SCREEN, 1, 0, 0);
  return com.type == TOCK_SYSCALL_SUCCESS_U32 && com.data[0] != 0;
}

int screen_set_brightness (size_t brightness) {
  ScreenReturn fbr;
  fbr.done = false;

  int ret = screen_subscribe(screen_callback, &fbr);
  if (ret == RETURNCODE_SUCCESS) {
    syscall_return_t com = command(DRIVER_NUM_SCREEN, 3, brightness, 0);
    ret = tock_command_return_novalue_to_returncode(com);
    if (ret == RETURNCODE_SUCCESS) {
      yield_for(&fbr.done);
      ret = fbr.error;
    }
  }
  return ret;
}

int screen_invert_on (void) {
  ScreenReturn fbr;
  fbr.done = false;

  int ret = screen_subscribe(screen_callback, &fbr);
  if (ret == RETURNCODE_SUCCESS) {
    syscall_return_t com = command(DRIVER_NUM_SCREEN, 4, 0, 0);
    ret = tock_command_return_novalue_to_returncode(com);
    if (ret == RETURNCODE_SUCCESS) {
      yield_for(&fbr.done);
      ret = fbr.error;
    }
  }
  return ret;
}

int screen_invert_off (void) {
  ScreenReturn fbr;
  fbr.done = false;

  int ret = screen_subscribe(screen_callback, &fbr);
  if (ret == RETURNCODE_SUCCESS) {
    syscall_return_t com = command(DRIVER_NUM_SCREEN, 5, 0, 0);
    ret = tock_command_return_novalue_to_returncode(com);
    if (ret == RETURNCODE_SUCCESS) {
      yield_for(&fbr.done);
      ret = fbr.error;
    }
  }
  return ret;
}

int screen_init (size_t len)
{
  int r = TOCK_STATUSCODE_SUCCESS;
  if (buffer != NULL) {
    r = TOCK_STATUSCODE_ALREADY;
  } else {
    buffer = (uint8_t*)calloc(1, len);
    if (buffer != NULL) {
      buffer_len = len;
      r = TOCK_STATUSCODE_SUCCESS;
    } else {
      r = TOCK_STATUSCODE_FAIL;
    }
  }
  return r;
}

uint8_t * screen_buffer (void)
{
  return buffer;
}

int screen_get_resolution (size_t *width, size_t *height) {
  syscall_return_t com = command(DRIVER_NUM_SCREEN, 23, 0, 0);
  if (com.type == TOCK_SYSCALL_SUCCESS_U32_U32) {
    *width  = com.data[0];
    *height = com.data[1];
    return RETURNCODE_SUCCESS;
  } else if (com.type == TOCK_SYSCALL_FAILURE) {
    return tock_status_to_returncode(com.data[0]);
  } else {
    return RETURNCODE_EINVAL;
  }
}

int screen_set_resolution (size_t width, size_t height) {
  ScreenReturn fbr;
  fbr.done = false;

  int ret = screen_subscribe(screen_callback, &fbr);
  if (ret == RETURNCODE_SUCCESS) {
    syscall_return_t com = command(DRIVER_NUM_SCREEN, 24, width, height);
    ret = tock_command_return_novalue_to_returncode(com);
    if (ret == RETURNCODE_SUCCESS) {
      yield_for(&fbr.done);
      ret = fbr.error;
    }
  }
  return ret;
}

int screen_get_bits_per_pixel (size_t format)
{
  switch (format) {
    case SCREEN_PIXEL_FORMAT_MONO:
      return 1;

    case SCREEN_PIXEL_FORMAT_RGB_233:
      return 8;

    case SCREEN_PIXEL_FORMAT_RGB_565:
      return 16;

    case SCREEN_PIXEL_FORMAT_RGB_888:
      return 24;

    case SCREEN_PIXEL_FORMAT_ARGB_8888:
      return 32;

    default:
      return 0;
  }
}

int screen_get_pixel_format (int* format) {
  ScreenReturn fbr;
  fbr.data1 = SCREEN_PIXEL_FORMAT_ERROR;
  fbr.done  = false;

  int ret = screen_subscribe(screen_callback, &fbr);
  if (ret == RETURNCODE_SUCCESS) {
    syscall_return_t com = command(DRIVER_NUM_SCREEN, 25, 0, 0);
    ret = tock_command_return_novalue_to_returncode(com);
    if (ret == RETURNCODE_SUCCESS) {
      yield_for(&fbr.done);
      *format = fbr.data1;
      ret     = fbr.error;
    }
  }
  return ret;
}

int screen_set_pixel_format (size_t format) {
  ScreenReturn fbr;
  fbr.done = false;

  int ret = screen_subscribe(screen_callback, &fbr);
  if (ret == RETURNCODE_SUCCESS) {
    syscall_return_t com = command(DRIVER_NUM_SCREEN, 26, format, 0);
    ret = tock_command_return_novalue_to_returncode(com);
    if (ret == RETURNCODE_SUCCESS) {
      yield_for(&fbr.done);
      ret = fbr.error;
    }
  }
  return ret;
}

int screen_get_rotation (int* rotation) {
  ScreenReturn fbr;
  fbr.data1 = SCREEN_ROTATION_NORMAL;
  fbr.done  = false;

  int ret = screen_subscribe(screen_callback, &fbr);
  if (ret == RETURNCODE_SUCCESS) {
    syscall_return_t com = command(DRIVER_NUM_SCREEN, 21, 0, 0);
    ret = tock_command_return_novalue_to_returncode(com);
    if (ret == RETURNCODE_SUCCESS) {
      yield_for(&fbr.done);
      *rotation = fbr.data1;
      ret       = fbr.error;
    }
  }
  return ret;
}

int screen_set_rotation (size_t rotation) {
  ScreenReturn fbr;
  fbr.done = false;

  int ret = screen_subscribe(screen_callback, &fbr);
  if (ret == RETURNCODE_SUCCESS) {
    syscall_return_t com = command(DRIVER_NUM_SCREEN, 22, rotation, 0);
    ret = tock_command_return_novalue_to_returncode(com);
    if (ret == RETURNCODE_SUCCESS) {
      yield_for(&fbr.done);
      ret = fbr.error;
    }
  }
  return ret;
}

int screen_set_color (size_t position, size_t color) {
  // TODO color mode
  int r = RETURNCODE_SUCCESS;
  if (position < buffer_len - 2) {
    buffer[position * 2]     = (color >> 8) & 0xFF;
    buffer[position * 2 + 1] = color & 0xFF;
  } else {
    r = RETURNCODE_ESIZE;
  }
  return r;
}

int screen_set_frame (uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
  ScreenReturn fbr;
  fbr.done = false;

  int ret = screen_subscribe(screen_callback, &fbr);
  if (ret == RETURNCODE_SUCCESS) {
    syscall_return_t com = command(DRIVER_NUM_SCREEN, 100, ((x & 0xFFFF) << 16) | ((y & 0xFFFF)),
                                   ((width & 0xFFFF) << 16) | ((height & 0xFFFF)));
    ret = tock_command_return_novalue_to_returncode(com);
    if (ret == RETURNCODE_SUCCESS) {
      yield_for(&fbr.done);
      ret = fbr.error;
    }
  }
  return ret;
}

int screen_fill (size_t color) {
  ScreenReturn fbr;
  fbr.done = false;

  int ret = screen_set_color(0, color);
  if (ret < 0) return ret;

  ret = screen_allow(buffer, buffer_len);
  if (ret == TOCK_STATUSCODE_SUCCESS) {
    ret = screen_subscribe(screen_callback, &fbr);
    if (ret == TOCK_STATUSCODE_SUCCESS) {
      syscall_return_t com = command(DRIVER_NUM_SCREEN, 300, 0, 0);
      ret = tock_command_return_novalue_to_returncode(com);
      if (ret == TOCK_STATUSCODE_SUCCESS) {
        yield_for(&fbr.done);
        ret = fbr.error;
      }
    }
    screen_allow(NULL, 0);
  }
  return ret;
}

int screen_write (size_t length) {
  ScreenReturn fbr;
  fbr.done = false;

  int ret = screen_allow(buffer, buffer_len);
  if (ret == TOCK_STATUSCODE_SUCCESS) {
    ret = screen_subscribe(screen_callback, &fbr);
    if (ret == TOCK_STATUSCODE_SUCCESS) {
      syscall_return_t com = command(DRIVER_NUM_SCREEN, 200, length, 0);
      ret = tock_command_return_novalue_to_returncode(com);
      if (ret == TOCK_STATUSCODE_SUCCESS) {
        yield_for(&fbr.done);
        ret = fbr.error;
      }
    }
    screen_allow(NULL, 0);
  }
  return ret;
}
