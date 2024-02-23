#include <stdio.h>
#include <string.h>
#include <timer.h>

#include <ipc.h>

size_t rot13_svc_num = 0;

char buf[64] __attribute__((aligned(64)));

struct rot13_buf {
  int8_t length;
  char buf[31];
};

static void rot13_callback(__attribute__ ((unused)) int pid,
                           __attribute__ ((unused)) int len,
                           __attribute__ ((unused)) int arg2, void* ud) {
  struct rot13_buf *rb = (struct rot13_buf*)ud;
  printf("%d: %.*s\n", rb->length, rb->length, rb->buf);
  delay_ms(500);
  ipc_notify_service(rot13_svc_num);
}

int main(void) {
  int err;
  err = ipc_discover("org.tockos.examples.rot13", &rot13_svc_num);
  if (err < 0) {
    printf("No rot13 service\n");
    return -1;
  }

  struct rot13_buf *rb = (struct rot13_buf*)buf;
  ipc_register_client_callback(rot13_svc_num, rot13_callback, rb);

  rb->length = snprintf(rb->buf, sizeof(rb->buf), "Hello World!");
  ipc_share(rot13_svc_num, rb, 64);

  ipc_notify_service(rot13_svc_num);
  return 0;
}
