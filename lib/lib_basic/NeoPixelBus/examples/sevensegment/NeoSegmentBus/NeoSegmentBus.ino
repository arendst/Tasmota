// NeoSegmentBus
// This example will demonstrate using the NeoSegmentBus which provides support for a
// seven segment LED digit driven by three WS2811; connected in series with other digits
//
// See https://shop.idlehandsdev.com/products/addressable-7-segment-display for a hardware example
//
// This example will print the string "3.14" and then rotate it through the available digits
//

#include <NeoPixelSegmentBus.h>

const uint16_t DigitCount = 4; // Max Digits, not segments, not pixels
const uint8_t BusPin = 2;  // make sure to set this to the correct pin, ignored for Esp8266

#define brightness 128

NeoPixelSegmentBus<SevenSegmentFeature, NeoWs2811Method> strip(DigitCount, BusPin);

void setup()
{
    strip.Begin();
    strip.Show(); // clears all digits by default

    delay(500);
    strip.SetString(0, "3.14", brightness);
    strip.Show(); 
}

void loop()
{
    delay(2000);

    strip.RotateRight(1); // reads right to left, so it is reversed
    strip.Show();
}

