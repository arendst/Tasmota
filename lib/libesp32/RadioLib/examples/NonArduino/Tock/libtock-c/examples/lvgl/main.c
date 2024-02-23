#include "lvgl_driver.h"
#include <stdio.h>
#include <timer.h>

static void event_handler(lv_event_t * e)
{
  lv_event_code_t code  = lv_event_get_code(e);
  unsigned int *seconds = (unsigned int*)lv_event_get_user_data(e);

  if (code == LV_EVENT_CLICKED) {
    LV_LOG_USER("Clicked");
    *seconds = 0;
  } else if (code == LV_EVENT_VALUE_CHANGED) {
    LV_LOG_USER("Toggled");
  }
}

int main (void)
{
  unsigned int seconds = 0;

  screen_set_brightness(100);
  int status = lvgl_driver_init(5);
  if (status == RETURNCODE_SUCCESS) {
    /* LittlevGL's Hello World tutorial example */

    lv_obj_t * scr = lv_disp_get_scr_act(NULL);         /*Get the current screen*/

    /*Create a Label on the currently active screen*/
    lv_obj_t * label1 =  lv_label_create(scr);

    /*Modify the Label's text*/
    lv_label_set_text(label1, "Hello world!");

    /* Align the Label to the center
     * NULL means align on parent (which is the screen now)
     * 0, 0 at the end means an x, y offset after alignment*/
    lv_obj_align(label1, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t * btn1 = lv_btn_create(scr);
    lv_obj_add_event_cb(btn1, event_handler, LV_EVENT_ALL, &seconds);
    lv_obj_align(btn1, LV_ALIGN_CENTER, 0, -40);

    lv_obj_t * label = lv_label_create(btn1);
    lv_label_set_text(label, "Reset");
    lv_obj_center(label);

    /* main loop */
    while (1) {
      seconds++;
      if (seconds % 200 == 0) {
        char buffer[100];
        snprintf(buffer, 99, "Seconds: %d", seconds / 200);
        lv_label_set_text(label1, buffer);
      }
      delay_ms(5);
      lvgl_driver_event(5);
    }
  } else {
    printf("lvgl init error: %s\n", tock_strrcode(status));
  }
  return 0;
}
