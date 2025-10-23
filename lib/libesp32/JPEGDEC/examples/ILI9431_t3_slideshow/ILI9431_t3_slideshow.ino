// A simple image slideshow which reads all the .JPG files from the root
// directory of a SD card and shows each for 1 second on an ILI9341 display.

#include <JPEGDEC.h>
#include <ILI9341_t3.h>
#include <Bounce.h>
#include <SD.h>

#define TFT_DC  9
#define TFT_CS 10

ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC);
JPEGDEC jpeg;

// Setup - initialize ILI9341 display, wait for serial monitor, open SD card
void setup() {
  pinMode(34, INPUT_PULLDOWN);
  pinMode(33, OUTPUT);
  digitalWrite(33, HIGH); // pushbuttons short pins 33 & 34 together
  Serial.begin(115200);
  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_YELLOW);
  tft.setTextSize(2);
  tft.println("Waiting for Arduino Serial Monitor...");

  while (!Serial && millis() < 3000); // wait up to 3 seconds for Arduino Serial Monitor
  Serial.println("ILI9341 Slideshow");
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0, 0);

  while (!SD.begin(BUILTIN_SDCARD)) {
    Serial.println("Unable to access SD Card");
    tft.println("Unable to access SD Card");
    delay(1000);
  }
}

// Functions to access a file on the SD card
File myfile;

void * myOpen(const char *filename, int32_t *size) {
  myfile = SD.open(filename);
  *size = myfile.size();
  return &myfile;
}
void myClose(void *handle) {
  if (myfile) myfile.close();
}
int32_t myRead(JPEGFILE *handle, uint8_t *buffer, int32_t length) {
  if (!myfile) return 0;
  return myfile.read(buffer, length);
}
int32_t mySeek(JPEGFILE *handle, int32_t position) {
  if (!myfile) return 0;
  return myfile.seek(position);
}

// Function to draw pixels to the display
int JPEGDraw(JPEGDRAW *pDraw) {
//  Serial.printf("jpeg draw: x,y=%d,%d, cx,cy = %d,%d\n",
//     pDraw->x, pDraw->y, pDraw->iWidth, pDraw->iHeight);
//     for (int i=0; i<pDraw->iWidth*pDraw->iHeight; i++) {
//      pDraw->pPixels[i] = __builtin_bswap16(pDraw->pPixels[i]);
//     }
  tft.writeRect(pDraw->x, pDraw->y, pDraw->iWidth, pDraw->iHeight, pDraw->pPixels);
  return 1;
}

// Main loop, scan for all .JPG files on the card and display them
void loop() {
  int filecount = 0;
  tft.setCursor(0, 0);
  File dir = SD.open("/");
  while (true) {
    File entry = dir.openNextFile();
    if (!entry) break;
    if (entry.isDirectory() == false) {
      const char *name = entry.name();
      const int len = strlen(name);
      if (len > 3 && strcasecmp(name + len - 3, "JPG") == 0) {
        Serial.print("File: ");
        Serial.println(name);
        tft.print("File: ");
        tft.println(name);
        if (jpeg.open((const char *)name, myOpen, myClose, myRead, mySeek, JPEGDraw)) {
          jpeg.decode(0,0,0);
          jpeg.close();
        } else {
          Serial.print("error = ");
          Serial.println(jpeg.getLastError(), DEC);
        } 
        filecount = filecount + 1;
        if (digitalRead(34) == LOW) {
          // skip delay between images when pushbutton is pressed
          delay(1000);
        }
      }
    }
    entry.close();
  }
  if (filecount == 0) {
    Serial.println("No .JPG files found");
    tft.println("No .JPG files found");
    delay(2000);
  }
}
