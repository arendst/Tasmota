#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>
#include "Adafruit_seesaw.h"

Adafruit_seesaw ss;
#define BUTTON_RIGHT 6
#define BUTTON_DOWN  7
#define BUTTON_LEFT  9
#define BUTTON_UP    10
#define BUTTON_START   14
uint32_t button_mask = (1 << BUTTON_RIGHT) | (1 << BUTTON_DOWN) | 
                (1 << BUTTON_LEFT) | (1 << BUTTON_UP) | (1 << BUTTON_START);
#if defined(ESP8266)
  #define IRQ_PIN   2
#elif defined(ESP32)
  #define IRQ_PIN   14
#elif defined(ARDUINO_NRF52832_FEATHER)
  #define IRQ_PIN   27
#elif defined(TEENSYDUINO)
  #define IRQ_PIN   8
#elif defined(ARDUINO_ARCH_WICED)
  #define IRQ_PIN   PC5
#else
  #define IRQ_PIN   5
#endif

// For the breakout, you can use any 2 or 3 pins
// These pins will also work for the 1.8" TFT shield
#define TFT_CS     6
#define TFT_DC     10
#define TFT_RST    9  // you can also connect this to the Arduino reset
                      // in which case, set this #define pin to -1!

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);

void setup()   {
  //while (!Serial);
  Serial.begin(115200);

  if(!ss.begin(0x49)){
    Serial.println("ERROR!");
    while(1);
  }
  else{
    Serial.println("seesaw started");
    Serial.print("version: ");
    Serial.println(ss.getVersion(), HEX);
  }
  ss.pinModeBulk(button_mask, INPUT_PULLUP);
  ss.setGPIOInterrupts(button_mask, 1);
  pinMode(IRQ_PIN, INPUT);

  Serial.println(F("TFT TIME"));

  tft.initR(INITR_MINI160x80);   // initialize a ST7735S chip, mini display
  Serial.println("Initialized");
  tft.setRotation(1);

  uint16_t time = millis();
  tft.fillScreen(ST7735_BLACK);
  time = millis() - time;

  Serial.println(time, DEC);
  delay(500);

  // large block of text
  tft.fillScreen(ST7735_BLACK);
}

int last_x = 0, last_y = 0;
void loop() {
  int y = ss.analogRead(2);
  int x = ss.analogRead(3);

  if(x > 600 && last_x < 600){
    tft.fillTriangle(120, 30, 120, 50, 110, 40, ST7735_WHITE);
    Serial.println(F("LEFT"));
  }
  else if(last_x > 600 && x < 600){
    tft.fillTriangle(120, 30, 120, 50, 110, 40, ST7735_BLACK);
  }
  
  if(x < 400 && last_x > 400){
    tft.fillTriangle(150, 30, 150, 50, 160, 40, ST7735_WHITE);
    Serial.println(F("RIGHT"));
  }
  else if(last_x < 400 && x > 400){
    tft.fillTriangle(150, 30, 150, 50, 160, 40, ST7735_BLACK);
  }
  
  if(y > 600 && last_y < 600){
    tft.fillTriangle(125, 26, 145, 26, 135, 16, ST7735_WHITE);
    Serial.println(F("DOWN"));
  }
  else if(last_y > 600 && y < 600){
    tft.fillTriangle(125, 26, 145, 26, 135, 16, ST7735_BLACK);
  }
  
  if(y < 400 && last_y > 400){
    tft.fillTriangle(125, 53, 145, 53, 135, 63, ST7735_WHITE);
    Serial.println(F("UP"));
  }
  else if(last_y < 400 && y > 400){
    tft.fillTriangle(125, 53, 145, 53, 135, 63, ST7735_BLACK);
  }

  if ( (abs(x - last_x) > 3)  ||  (abs(y - last_y) > 3)) {
    Serial.print(x); Serial.print(", "); Serial.println(y);
    last_x = x;
    last_y = y;
  }
  
  if(!digitalRead(IRQ_PIN)){
    uint32_t buttons = ss.digitalReadBulk(button_mask);
    //Serial.println(buttons, BIN);
    if (! (buttons & (1 << BUTTON_DOWN))) {
      tft.fillCircle(30, 18, 10, ST7735_GREEN);
      Serial.println("B");
    }
    else tft.fillCircle(30, 18, 10, ST7735_BLACK);
    
    if (! (buttons & (1 << BUTTON_RIGHT))) {
      tft.fillCircle(10, 40, 10, ST7735_RED);
      Serial.println("A");
    }
    else tft.fillCircle(10, 40, 10, ST7735_BLACK);
    
    if (! (buttons & (1 << BUTTON_LEFT))) {
      tft.fillCircle(50, 40, 10, ST7735_BLUE);
      Serial.println("Y");
    }
    else tft.fillCircle(50, 40, 10, ST7735_BLACK);
    
    if (! (buttons & (1 << BUTTON_UP))) {
      tft.fillCircle(30, 57, 10, ST7735_YELLOW);
      Serial.println("X");
    }
    else tft.fillCircle(30, 57, 10, ST7735_BLACK);
    
    if (! (buttons & (1 << BUTTON_START))) {
      tft.fillCircle(80, 40, 7, ST7735_WHITE);
      Serial.println(F("START"));
    }
    else tft.fillCircle(80, 40, 7, ST7735_BLACK);
  }
  delay(10);
}
