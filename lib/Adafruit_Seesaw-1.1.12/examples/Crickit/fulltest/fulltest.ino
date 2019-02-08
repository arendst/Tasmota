#include "Adafruit_Crickit.h"
#include "seesaw_neopixel.h"
#include "seesaw_servo.h"
#define NEOPIX_PIN 20

#define USE_NEOPIX

seesaw_NeoPixel strip = seesaw_NeoPixel(24, NEOPIX_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_Crickit crickit;
seesaw_Servo s1(&crickit);
seesaw_Servo s2(&crickit);
seesaw_Servo s3(&crickit);
seesaw_Servo s4(&crickit);

#define NUM_SERVOS 4
seesaw_Servo *servos[NUM_SERVOS] = {&s1, &s2, &s3, &s4};

#define COLOR_MAX 150
#define RED  strip.Color(COLOR_MAX, 0, 0)
#define YELLOW  strip.Color(COLOR_MAX, 150, 0)
#define GREEN  strip.Color(0, COLOR_MAX, 0)
#define CYAN  strip.Color(0, COLOR_MAX, 255)
#define BLUE  strip.Color(0, 0, COLOR_MAX)
#define PURPLE  strip.Color(180, 0, COLOR_MAX)

#define CRICKIT_NUM_ADC 8
static const uint8_t crickit_adc[CRICKIT_NUM_ADC] = { CRICKIT_SIGNAL1, CRICKIT_SIGNAL2, CRICKIT_SIGNAL3, CRICKIT_SIGNAL4,
                          CRICKIT_SIGNAL5, CRICKIT_SIGNAL6, CRICKIT_SIGNAL7, CRICKIT_SIGNAL8 };

#define CRICKIT_NUM_TOUCH 4
static const uint8_t crickit_drive[CRICKIT_NUM_TOUCH] = {CRICKIT_DRIVE1, CRICKIT_DRIVE2, CRICKIT_DRIVE3, CRICKIT_DRIVE4};

#define CAPTOUCH_THRESH 500

void setup() {
  Serial.begin(9600);
  while(!Serial);
  if(!crickit.begin(0x49, A0)){
    Serial.println("ERROR Starting crickit");
    while(1);
  }
  else Serial.println("seesaw started");

  if(!strip.begin(0x49, A0)){
    Serial.println("ERROR Starting neopix");
    while(1);
  }
  Serial.println("neopix started!");

  s1.attach(CRICKIT_SERVO1);
  s2.attach(CRICKIT_SERVO2);
  s3.attach(CRICKIT_SERVO3);
  s4.attach(CRICKIT_SERVO4);
}

void loop() {

#ifdef USE_NEOPIX
  for(uint16_t i=0; i<strip.numPixels(); i++)
    strip.setPixelColor(i, RED);
  strip.show();
#endif

  for(int i=0; i<CRICKIT_NUM_ADC; i++){
    Serial.print(crickit.analogRead(crickit_adc[i]));
    Serial.print("\t");
  }
  Serial.println("");
  
  //TODO: fix drive3 and drive4
  for(int i=0; i<4; i++){
    uint16_t val = crickit.touchRead(i);

    if(val > CAPTOUCH_THRESH){
      crickit.analogWrite(crickit_drive[i], (1UL << 16) - 1);
      Serial.print("CT");
      Serial.print(i + 1);
      Serial.print(" touched! value: ");
      Serial.println(val);
    }
    else
      crickit.analogWrite(crickit_drive[i], 0);
  }

#ifdef USE_NEOPIX
  for(uint16_t i=0; i<strip.numPixels(); i++)
    strip.setPixelColor(i, GREEN);
  strip.show();
#endif

  for(int i=0; i<NUM_SERVOS; i++){
    seesaw_Servo *s = servos[i];
    s->write(1000);
  }
  delay(500);

#ifdef USE_NEOPIX
  for(uint16_t i=0; i<strip.numPixels(); i++)
    strip.setPixelColor(i, BLUE);
  strip.show();
#endif

  for(int i=0; i<NUM_SERVOS; i++){
    seesaw_Servo *s = servos[i];
    s->write(2000);
  }
  delay(500);
}
