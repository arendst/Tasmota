// DotStarTest
// This example will cycle between showing four pixels as Red, Green, Blue, White
// and then showing those pixels as Black.
//
// There is serial output of the current state so you can confirm and follow along
//

#include <NeoPixelBus.h>

const uint16_t PixelCount = 4; // this example assumes 4 pixels, making it smaller will cause a failure

// make sure to set this to the correct pins
const uint8_t DotClockPin = 2;
const uint8_t DotDataPin = 3;  

#define colorSaturation 128

// for software bit bang
NeoPixelBus<DotStarBgrFeature, DotStarMethod> strip(PixelCount, DotClockPin, DotDataPin);

// for hardware SPI (best performance but must use hardware pins)
//NeoPixelBus<DotStarBgrFeature, DotStarSpiMethod> strip(PixelCount);

// DotStars that support RGB color and a overall luminance/brightness value
// NeoPixelBus<DotStarLbgrFeature, DotStarMethod> strip(PixelCount, DotClockPin, DotDataPin);
// DotStars that support RGBW color with a seperate white element
//NeoPixelBus<DotStarWbgrFeature, DotStarMethod> strip(PixelCount, DotClockPin, DotDataPin);

RgbColor red(colorSaturation, 0, 0);
RgbColor green(0, colorSaturation, 0);
RgbColor blue(0, 0, colorSaturation);
RgbColor white(colorSaturation);
RgbColor black(0);

// for use with RGB DotStars when using the luminance/brightness global value
// note that its range is only 0 - 31 (31 is full bright) and 
// also note that it is not useful for POV displays as it will cause more flicker
RgbwColor redL(colorSaturation, 0, 0, 31); // use white value to store luminance
RgbwColor greenL(0, colorSaturation, 0, 31); // use white value to store luminance
RgbwColor blueL(0, 0, colorSaturation, 31); // use white value to store luminance
RgbwColor whiteL(255, 255, 255, colorSaturation / 8); // luminance is only 0-31

void setup()
{
    Serial.begin(115200);
    while (!Serial); // wait for serial attach

    Serial.println();
    Serial.println("Initializing...");
    Serial.flush();

    // this resets all the neopixels to an off state
    strip.Begin();
    strip.ClearTo(black);
    strip.Show();

    Serial.println();
    Serial.println("Running...");
}


void loop()
{
    delay(5000);

    Serial.println("Colors R, G, B, W...");

    // set the colors, 
    strip.SetPixelColor(0, red);
    strip.SetPixelColor(1, green);
    strip.SetPixelColor(2, blue);
    strip.SetPixelColor(3, white);
    strip.Show();


    delay(5000);

    Serial.println("Off ...");

    // turn off the pixels
    strip.SetPixelColor(0, black);
    strip.SetPixelColor(1, black);
    strip.SetPixelColor(2, black);
    strip.SetPixelColor(3, black);
    strip.Show();

}

