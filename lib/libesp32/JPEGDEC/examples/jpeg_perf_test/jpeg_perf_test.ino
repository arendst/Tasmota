//
// Perf Test
//
#include <JPEGDEC.h>
#include "../test_images/tulips.h" // 640x480 56k byte test image
JPEGDEC jpeg;

int JPEGDraw(JPEGDRAW *pDraw)
{
  // do nothing
  return 1; // continue decode
} /* JPEGDraw() */

void setup() {
  Serial.begin(115200);
  delay(100); // allow time for Serial to start
} /* setup() */

void loop() {
long lTime;

  if (jpeg.openFLASH((uint8_t *)tulips, sizeof(tulips), JPEGDraw)) {
    lTime = micros();
    if (jpeg.decode(0,0,0)) { // full sized decode
      lTime = micros() - lTime;
   //   Serial.printf("full sized decode in %d us\n", (int)lTime);
    }
    jpeg.close();
  }
  if (jpeg.openFLASH((uint8_t *)tulips, sizeof(tulips), JPEGDraw)) {
    lTime = micros();
    if (jpeg.decode(0,0,JPEG_SCALE_HALF)) { // 1/2 sized decode
      lTime = micros() - lTime;
    //  Serial.printf("half sized decode in %d us\n", (int)lTime);
    }
    jpeg.close();
  }
  if (jpeg.openFLASH((uint8_t *)tulips, sizeof(tulips), JPEGDraw)) {
    lTime = micros();
    if (jpeg.decode(0,0,JPEG_SCALE_QUARTER)) { // 1/4 sized decode
      lTime = micros() - lTime;
    //  Serial.printf("quarter sized decode in %d us\n", (int)lTime);
    }
    jpeg.close();
  }
  if (jpeg.openFLASH((uint8_t *)tulips, sizeof(tulips), JPEGDraw)) {
    lTime = micros();
    if (jpeg.decode(0,0,JPEG_SCALE_EIGHTH)) { // 1/8 sized decode
      lTime = micros() - lTime;
    //  Serial.printf("eighth sized decode in %d us\n", (int)lTime);
    }
    jpeg.close();
  }
  delay(5000);
} /* loop() */
