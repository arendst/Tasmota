//
// Perf Test
//
#include <JPEGDEC.h>
#include <time.h>
#include "../../../test_images/tulips.h" // 640x480 56k byte test image
JPEGDEC jpeg;

long micros(void)
{
int iTime;
struct timespec res;

    clock_gettime(CLOCK_MONOTONIC, &res);
    iTime = 1000000*res.tv_sec + res.tv_nsec/1000;

    return (long)iTime;
} /* micros() */

int JPEGDraw(JPEGDRAW *pDraw)
{
  // do nothing
  return 1; // continue decode
} /* JPEGDraw() */

int main(int argc, char *argv[]) {
long lTime;

  if (jpeg.openFLASH((uint8_t *)tulips, sizeof(tulips), JPEGDraw)) {
    lTime = micros();
    if (jpeg.decode(0,0,0)) { // full sized decode
      lTime = micros() - lTime;
      printf("full sized decode in %d us\n", (int)lTime);
    }
    jpeg.close();
  }
  if (jpeg.openFLASH((uint8_t *)tulips, sizeof(tulips), JPEGDraw)) {
    lTime = micros();
    if (jpeg.decode(0,0,JPEG_SCALE_HALF)) { // 1/2 sized decode
      lTime = micros() - lTime;
      printf("half sized decode in %d us\n", (int)lTime);
    }
    jpeg.close();
  }
  if (jpeg.openFLASH((uint8_t *)tulips, sizeof(tulips), JPEGDraw)) {
    lTime = micros();
    if (jpeg.decode(0,0,JPEG_SCALE_QUARTER)) { // 1/4 sized decode
      lTime = micros() - lTime;
      printf("quarter sized decode in %d us\n", (int)lTime);
    }
    jpeg.close();
  }
  if (jpeg.openFLASH((uint8_t *)tulips, sizeof(tulips), JPEGDraw)) {
    lTime = micros();
    if (jpeg.decode(0,0,JPEG_SCALE_EIGHTH)) { // 1/8 sized decode
      lTime = micros() - lTime;
      printf("eighth sized decode in %d us\n", (int)lTime);
    }
    jpeg.close();
  }
  return 0;
} /* main() */
