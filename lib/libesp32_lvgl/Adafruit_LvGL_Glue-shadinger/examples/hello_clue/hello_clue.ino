// Minimal "Hello" example for LittlevGL on Adafruit CLUE. Requires
// LittlevGL, Adafruit_LvGL_Glue, Adafruit_GFX and Adafruit_ST7735
// libraries.

// Prior Adafruit_LvGL_Glue users: see hello_changes example for updates!

#include <Adafruit_LvGL_Glue.h> // Always include this BEFORE lvgl.h!
#include <lvgl.h>
#include <Adafruit_ST7789.h>

#define TFT_ROTATION   1 // Landscape orientation on CLUE
#define TFT_SPI     SPI1 // CLUE display peripheral & pins
#define TFT_CS        31
#define TFT_DC        32
#define TFT_RST       33
#define TFT_BACKLIGHT 34

Adafruit_ST7789    tft(&TFT_SPI, TFT_CS, TFT_DC, TFT_RST);
Adafruit_LvGL_Glue glue;

// This example sketch's LittlevGL UI-building calls are all in this
// function rather than in setup(), so simple programs can just
// copy-and-paste this sketch as a starting point, then embellish here:
void lvgl_setup(void) {
  // Create simple label centered on screen
  lv_obj_t *label = lv_label_create(lv_scr_act(), NULL);
  lv_label_set_text(label, "Hello Arduino!");
  lv_obj_align(label, NULL, LV_ALIGN_CENTER, 0, 0);
}

void setup(void) {
  Serial.begin(115200);

  // Initialize display BEFORE glue setup
  tft.init(240, 240);
  tft.setRotation(TFT_ROTATION);
  pinMode(TFT_BACKLIGHT, OUTPUT);
  digitalWrite(TFT_BACKLIGHT, HIGH);

  // Initialize glue, passing in address of display
  LvGLStatus status = glue.begin(&tft);
  if(status != LVGL_OK) {
    Serial.printf("Glue error %d\r\n", (int)status);
    for(;;);
  }

  lvgl_setup(); // Call UI-building function above
}

void loop(void) {
  lv_task_handler(); // Call LittleVGL task handler periodically
  delay(5);
}

// NOTE TO FUTURE SELF: this sketch is essentially the same as the Gizmo
// widgets example. If updating one sketch, make sure the other is kept
// in sync. Aside from screen setup, differences include backlight control
// and A/B button setup & read (CPX is active high, CLUE is active low).
