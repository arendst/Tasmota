#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <console.h>

char buf[100];

static void getnstr_cb(int   result __attribute__ ((unused)),
                       int   len,
                       int   _z __attribute__ ((unused)),
                       void* ud __attribute__ ((unused))) {
  printf("\n\nconsole_recv_long: ");
  for (int i = 0; i < len; i++) {
    printf("%c", buf[i]);
  }
  printf("\n");
}


int main(void) {
  int ret = getnstr_async(buf, 61, getnstr_cb, NULL);
  if (ret != RETURNCODE_SUCCESS) {
    printf("[LONG] Error doing UART receive: %i\n", ret);
  }
}
