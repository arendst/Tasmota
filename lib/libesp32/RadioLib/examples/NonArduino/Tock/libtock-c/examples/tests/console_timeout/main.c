#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <console.h>
#include <timer.h>

char buf[100];

tock_timer_t t;

static void getnstr_cb(int   result __attribute__ ((unused)),
                       int   len,
                       int   _z __attribute__ ((unused)),
                       void* ud __attribute__ ((unused))) {
  printf("Userspace call to read console returned: ");
  for (int i = 0; i < len; i++) {
    printf("%c", buf[i]);
  }
  printf("\n");
}

static void timer_cb(int   result __attribute__ ((unused)),
                     int   _y __attribute__ ((unused)),
                     int   _z __attribute__ ((unused)),
                     void* ud __attribute__ ((unused))) {
  getnstr_abort();
}

int main(void) {

  int ret = getnstr_async(buf, 60, getnstr_cb, NULL);
  if (ret != RETURNCODE_SUCCESS) {
    printf("Error doing UART receive.\n");
  }

  // Generate a timeout to abort the receive call.
  timer_in(5000, timer_cb, NULL, &t);
}
