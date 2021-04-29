// Minimal "Hello" example for LittlevGL on Adafruit PyPortal. Requires
// LittlevGL, Adafruit_LvGL_Glue, Adafruit Touchscreen, Adafruit_GFX and
// Adafruit_ILI9341 (PyPortal, PyPortal Pynt) or Adafruit_HX8357 (PyPortal
// Titano) libraries. This example doesn't use any touchscreen input, but
// it's declared anyway so this sketch can be copied-and-pasted to serve
// as a starting point for other projects. If display is scrambled, check
// that the correct board is selected -- PyPortal vs PyPortal Titano.

// Prior Adafruit_LvGL_Glue users: see hello_changes example for updates!

#include <Adafruit_LvGL_Glue.h> // Always include this BEFORE lvgl.h!
#include <lvgl.h>
#include <TouchScreen.h>

#define TFT_ROTATION   3 // Landscape orientation on PyPortal
#define TFT_D0        34 // PyPortal TFT pins
#define TFT_WR        26
#define TFT_DC        10
#define TFT_CS        11
#define TFT_RST       24
#define TFT_RD         9
#define TFT_BACKLIGHT 25
#define YP            A4 // PyPortal touchscreen pins
#define XP            A5
#define YM            A6
#define XM            A7

#if defined(ADAFRUIT_PYPORTAL_M4_TITANO)
  #include <Adafruit_HX8357.h>
  Adafruit_HX8357  tft(tft8bitbus, TFT_D0, TFT_WR, TFT_DC, TFT_CS, TFT_RST,
    TFT_RD);
#else
  #include <Adafruit_ILI9341.h>
  Adafruit_ILI9341 tft(tft8bitbus, TFT_D0, TFT_WR, TFT_DC, TFT_CS, TFT_RST,
    TFT_RD);
#endif
TouchScreen        ts(XP, YP, XM, YM, 300);
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
  tft.begin();
  tft.setRotation(TFT_ROTATION);
  pinMode(TFT_BACKLIGHT, OUTPUT);
  digitalWrite(TFT_BACKLIGHT, HIGH);

  // PyPortal touchscreen needs no init

  // Initialize glue, passing in address of display & touchscreen
  LvGLStatus status = glue.begin(&tft, &ts);
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
