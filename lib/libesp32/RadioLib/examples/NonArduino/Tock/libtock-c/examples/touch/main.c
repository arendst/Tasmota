#include <screen.h>
#include <stdio.h>
#include <stdlib.h>
#include <timer.h>
#include <touch.h>

static void touch_event (int status, int x, int y, void *ud __attribute__ ((unused))) {
  switch (status) {
    case TOUCH_STATUS_PRESSED: {
      printf("pressed ");
      break;
    }
    case TOUCH_STATUS_RELEASED: {
      printf("released ");
      break;
    }
    case TOUCH_STATUS_MOVED: {
      printf("moved ");
      break;
    }
    default:
      printf("error ");
  }
  printf("(%d): %d y %d\n", status, x, y);
}

static void multi_touch_event (int num_touches, int data2 __attribute__ ((unused)), int data3 __attribute__ (
                                 (unused)), void *ud __attribute__ ((unused))) {
  for (int i = 0; i < num_touches; i++) {
    unsigned char id, status;
    unsigned short x, y;
    read_touch(i, &id, &status, &x, &y);
    printf("%d: ", id);
    switch (status) {
      case TOUCH_STATUS_PRESSED: {
        printf("pressed ");
        break;
      }
      case TOUCH_STATUS_RELEASED: {
        printf("released ");
        break;
      }
      case TOUCH_STATUS_MOVED: {
        printf("moved ");
        break;
      }
      default:
        printf("error ");
    }
    printf("(%d): %d y %d, ", status, x, y);
  }
  printf("\n");
  // ack the multi touch event and enable the next event
  multi_touch_next();
}

int main(void) {
  int num_touches = 0;
  get_number_of_touches(&num_touches);
  printf("Number of touches: %d\n", num_touches);
  if (num_touches == 0) {
    printf("No touch found\n");
  } else if (num_touches == 1) {
    // single touch
    enable_single_touch();
    single_touch_set_callback(touch_event, NULL);
  } else {
    // multi touch
    enable_multi_touch();
    multi_touch_set_callback(multi_touch_event, NULL, num_touches);
  }
  return 0;
}
