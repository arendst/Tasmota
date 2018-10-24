// 'wavface' example sketch for Adafruit I2C 8x8 LED backpacks
// and Wave Shield:
//
//  www.adafruit.com/products/870   www.adafruit.com/products/1049
//  www.adafruit.com/products/871   www.adafruit.com/products/1050
//  www.adafruit.com/products/872   www.adafruit.com/products/1051
//  www.adafruit.com/products/959   www.adafruit.com/products/1052
//                  www.adafruit.com/products/94
//
// Requires Adafruit_LEDBackpack, Adafruit_GFX libraries and WaveHC
// libraries.
//
// This sketch shows animation roughly synchronized to prerecorded
// speech.  It's fairly complex and may be overwhelming to novice
// programmers, who may want to start with the 'matrix8x8' example
// and then 'roboface' before working through this code.  Also, much
// of the comments relating to the face animation have been stripped
// here for brevity...refer to the 'roboface' sketch if you have any
// questions how that part works.
//
// Additional hardware required: sounds are triggered using three
// normally-open momentary buttons connected to Digital pins 6, 7, 8
// and GND. (e.g. www.adafruit.com/products/1009 )
//
// Adafruit invests time and resources providing this open source code,
// please support Adafruit and open-source hardware by purchasing
// products from Adafruit!
//
// Written by P. Burgess for Adafruit Industries, parts adapted from
// 'PiSpeakHC' sketch included with WaveHC library.
// BSD license, all text above must be included in any redistribution.

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <WaveHC.h>
#include <WaveUtil.h>
#include "Adafruit_LEDBackpack.h"

// These WAV files should be in the root level of the SD card:
static const char PROGMEM
  wav0[] = "beware_i.wav",
  wav1[] = "ihunger.wav",
  wav2[] = "run_cowd.wav";
static const char * const wavname[] PROGMEM = { wav0, wav1, wav2 };
// PROGMEM makes frequent appearances throughout this code, reason being that
// the SD card library requires gobs of precious RAM (leaving very little to
// our own sketch).  PROGMEM lets us put fixed data into program flash memory,
// which is considerably more spacious.  String tables are paritcularly nasty.
// See www.arduino.cc/en/Reference/PROGMEM for more info.

SdReader  card; // This object holds the information for the card
FatVolume vol;  // This holds the information for the partition on the card
FatReader root; // This holds the information for the volumes root directory
FatReader file; // This object represent the WAV file for a phrase
WaveHC    wave; // A single wave object -- only one sound is played at a time

// Because the two eye matrices share the same address, only four
// matrix objects are needed for the five displays:
#define MATRIX_EYES         0
#define MATRIX_MOUTH_LEFT   1
#define MATRIX_MOUTH_MIDDLE 2
#define MATRIX_MOUTH_RIGHT  3
Adafruit_8x8matrix matrix[4] = { // Array of Adafruit_8x8matrix objects
  Adafruit_8x8matrix(), Adafruit_8x8matrix(),
  Adafruit_8x8matrix(), Adafruit_8x8matrix() };

// Rather than assigning matrix addresses sequentially in a loop, each
// has a spot in this array.  This makes it easier if you inadvertently
// install one or more matrices in the wrong physical position --
// re-order the addresses in this table and you can still refer to
// matrices by index above, no other code or wiring needs to change.
static const uint8_t PROGMEM matrixAddr[] = { 0x70, 0x71, 0x72, 0x73 };

static const uint8_t PROGMEM // Bitmaps are stored in program memory
  blinkImg[][8] = {    // Eye animation frames
  { B00111100,         // Fully open eye
    B01111110,
    B11111111,
    B11111111,
    B11111111,
    B11111111,
    B01111110,
    B00111100 },
  { B00000000,
    B01111110,
    B11111111,
    B11111111,
    B11111111,
    B11111111,
    B01111110,
    B00111100 },
  { B00000000,
    B00000000,
    B00111100,
    B11111111,
    B11111111,
    B11111111,
    B00111100,
    B00000000 },
  { B00000000,
    B00000000,
    B00000000,
    B00111100,
    B11111111,
    B01111110,
    B00011000,
    B00000000 },
  { B00000000,         // Fully closed eye
    B00000000,
    B00000000,
    B00000000,
    B10000001,
    B01111110,
    B00000000,
    B00000000 } },
  mouthImg[][24] = {                 // Mouth animation frames
  { B00000000, B00000000, B00000000, // Mouth position A
    B00000000, B00000000, B00000000,
    B01111111, B11111111, B11111110,
    B00000000, B00000000, B00000000,
    B00000000, B00000000, B00000000,
    B00000000, B00000000, B00000000,
    B00000000, B00000000, B00000000,
    B00000000, B00000000, B00000000 },
  { B00000000, B00000000, B00000000, // Mouth position B
    B00000000, B00000000, B00000000,
    B00111111, B11111111, B11111100,
    B00000111, B00000000, B11100000,
    B00000000, B11111111, B00000000,
    B00000000, B00000000, B00000000,
    B00000000, B00000000, B00000000,
    B00000000, B00000000, B00000000 },
  { B00000000, B00000000, B00000000, // Mouth position C
    B00000000, B00000000, B00000000,
    B00111111, B11111111, B11111100,
    B00001000, B00000000, B00010000,
    B00000110, B00000000, B01100000,
    B00000001, B11000011, B10000000,
    B00000000, B00111100, B00000000,
    B00000000, B00000000, B00000000 },
  { B00000000, B00000000, B00000000, // Mouth position D
    B00000000, B00000000, B00000000,
    B00111111, B11111111, B11111100,
    B00100000, B00000000, B00000100,
    B00010000, B00000000, B00001000,
    B00001100, B00000000, B00110000,
    B00000011, B10000001, B11000000,
    B00000000, B01111110, B00000000 },
  { B00000000, B00000000, B00000000, // Mouth position E
    B00000000, B00111100, B00000000,
    B00011111, B11000011, B11111000,
    B00000011, B10000001, B11000000,
    B00000000, B01111110, B00000000,
    B00000000, B00000000, B00000000,
    B00000000, B00000000, B00000000,
    B00000000, B00000000, B00000000 },
  { B00000000, B00111100, B00000000, // Mouth position F
    B00000000, B11000011, B00000000,
    B00001111, B00000000, B11110000,
    B00000001, B00000000, B10000000,
    B00000000, B11000011, B00000000,
    B00000000, B00111100, B00000000,
    B00000000, B00000000, B00000000,
    B00000000, B00000000, B00000000 } };

// Animation sequences corresponding to each WAV.  First number in
// each pair is a mouth bitmap index.  Second number is the hold
// time (in frames).  255 marks end of list.
// There is no 'magic' here, the software is NOT deriving mouth
// position from the sound...the tables were determined by hand,
// just as animators do it.  Further explanation here:
// http://www.idleworm.com/how/anm/03t/talk1.shtml

static const uint8_t PROGMEM
  seq1[] = { 0, 2,   2, 5,   5, 3,   3, 7, // "Beware, I live!"
             4, 5,   3, 4,   2, 5,   4, 3,
             3, 4,   1, 5,   3, 5,    255 },
  seq2[] = { 0, 1,   3, 5,   1, 5,   4, 2, // "I hunger!"
             3, 2,   1, 2,   4, 4,   1, 3,
             4, 2,   255 },
  seq3[] = { 0, 1,   1, 2,   3, 6,   2, 5, // "Run, coward!"
             0, 1,   4, 4,   5, 2,   1, 5,
             3, 6,   1, 4,    255 };
static const uint8_t * const anim[] = { seq1, seq2, seq3 };

const uint8_t
  blinkIndex[] PROGMEM = { 1, 2, 3, 4, 3, 2, 1 }; // Blink bitmap sequence
uint8_t
  blinkCountdown = 100, // Countdown to next blink (in frames)
  gazeCountdown  =  75, // Countdown to next eye movement
  gazeFrames     =  50, // Duration of eye movement (smaller = faster)
  mouthPos       =   0, // Current image number for mouth
  mouthCountdown =  10, // Countdown to next mouth change
  newPos         = 255, // New mouth position for current frame
  *seq,                 // Animation sequence currently being played back
  idx,                  // Current array index within animation sequence
  prevBtn        = 99,  // Button # pressed on last loop() iteration
  btnCount       = 0;   // Number of iterations same button has been held
int8_t
  eyeX = 3, eyeY = 3,   // Current eye position
  newX = 3, newY = 3,   // Next eye position
  dX   = 0, dY   = 0;   // Distance from prior to new position

void setup() {

  Serial.begin(9600);           
  
  Serial.println(F("WAV face"));
  
  if(!card.init())        Serial.println(F("Card init. failed!"));
  if(!vol.init(card))     Serial.println(F("No partition!"));
  if(!root.openRoot(vol)) Serial.println(F("Couldn't open dir"));
  Serial.println(F("Files found:"));
  root.ls();

  // Seed random number generator from an unused analog input:
  randomSeed(analogRead(A0));

  // Initialize each matrix object:
  for(uint8_t i=0; i<4; i++) {
    matrix[i].begin(pgm_read_byte(&matrixAddr[i]));
    // If using 'small' (1.2") displays vs. 'mini' (0.8"), enable this:
    // matrix[i].setRotation(3);
  }

  // Enable pull-up resistors on three button inputs.
  // Other end of each button then connects to GND.
  for(uint8_t i=6; i<=8; i++) {
    pinMode(i, INPUT);
    digitalWrite(i, HIGH); // Enable pullup
  }
}

void loop() {

  uint8_t i;

  // Draw eyeball in current state of blinkyness (no pupil).
  matrix[MATRIX_EYES].clear();
  matrix[MATRIX_EYES].drawBitmap(0, 0,
    blinkImg[
      (blinkCountdown < sizeof(blinkIndex)) ?      // Currently blinking?
      pgm_read_byte(&blinkIndex[blinkCountdown]) : // Yes, look up bitmap #
      0                                            // No, show bitmap 0
    ], 8, 8, LED_ON);
  // Decrement blink counter.  At end, set random time for next blink.
  if(--blinkCountdown == 0) blinkCountdown = random(5, 180);

  if(--gazeCountdown <= gazeFrames) {
    // Eyes are in motion - draw pupil at interim position
    matrix[MATRIX_EYES].fillRect(
      newX - (dX * gazeCountdown / gazeFrames),
      newY - (dY * gazeCountdown / gazeFrames),
      2, 2, LED_OFF);
    if(gazeCountdown == 0) {    // Last frame?
      eyeX = newX; eyeY = newY; // Yes.  What's new is old, then...
      do { // Pick random positions until one is within the eye circle
        newX = random(7); newY = random(7);
        dX   = newX - 3;  dY   = newY - 3;
      } while((dX * dX + dY * dY) >= 10);      // Thank you Pythagoras
      dX            = newX - eyeX;             // Horizontal distance to move
      dY            = newY - eyeY;             // Vertical distance to move
      gazeFrames    = random(3, 15);           // Duration of eye movement
      gazeCountdown = random(gazeFrames, 120); // Count to end of next movement
    }
  } else {
    // Not in motion yet -- draw pupil at current static position
    matrix[MATRIX_EYES].fillRect(eyeX, eyeY, 2, 2, LED_OFF);
  }

  // Scan buttons 6, 7, 8 looking for first button pressed...
  for(i=0; (i<3) && (digitalRead(i+6) == HIGH); i++);

  if(i < 3) {               // Anything pressed?  Yes!
    if(i == prevBtn) {      // Same as last time we checked?  Good!
      if(++btnCount == 3) { // 3 passes to 'debounce' button input
        playfile((char *)pgm_read_word(&wavname[i])); // Start WAV
        // Look up animation sequence # corresponding to this WAV...
        seq            = (uint8_t *)pgm_read_word(&anim[i]);
        idx            = 0; // Begin at first byte of data
        newPos         = pgm_read_byte(&seq[idx++]); // Initial mouth pos
        mouthCountdown = pgm_read_byte(&seq[idx++]); // Hold time for pos
      }
    } else btnCount = 0; // Different button than before - start count over
    prevBtn = i;
  } else prevBtn = 99;   // No buttons pressed

  if(newPos != 255) { // Is the mouth in motion?
    if(--mouthCountdown == 0) { // Count down frames to next position
      newPos = pgm_read_byte(&seq[idx++]); // New mouth position
      if(newPos == 255) { // End of list?
        mouthPos = 0;     // Yes, set mouth to neutral position
      } else {
        mouthPos       = newPos; // Set mouth to new position
        mouthCountdown = pgm_read_byte(&seq[idx++]); // Read hold time
      }
    }
  } else mouthPos = 0; // Mouth not in motion -- set to neutral position

  drawMouth(mouthImg[mouthPos]);

  // Refresh all matrices in one quick pass
  for(uint8_t i=0; i<4; i++) matrix[i].writeDisplay();

  delay(20);
}

// Draw mouth image across three adjacent displays
void drawMouth(const uint8_t *img) {
  for(uint8_t i=0; i<3; i++) {
    matrix[MATRIX_MOUTH_LEFT + i].clear();
    matrix[MATRIX_MOUTH_LEFT + i].drawBitmap(i * -8, 0, img, 24, 8, LED_ON);
  }
}

// Open and start playing a WAV file
void playfile(const char *name) {
  char filename[13]; // 8.3+NUL

  if(wave.isplaying) wave.stop(); // Stop any currently-playing WAV

  strcpy_P(filename, name); // Copy name out of PROGMEM into RAM

  if(!file.open(root, filename)) {
    Serial.print(F("Couldn't open file "));
    Serial.println(filename);
    return;
  }
  if(!wave.create(file)) {
    Serial.println(F("Not a valid WAV"));
    return;
  }
  wave.play();
}

