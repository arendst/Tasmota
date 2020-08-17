//----------------------------------------------------------------------
// NeoPixelTopologyTest
// This will display specific colors in specific locations on the led panel
//
// This is useful in confirming the layout of your panel
//
// It does require that you have the actual panel connected
//----------------------------------------------------------------------

#include <NeoPixelAnimator.h>
#include <NeoPixelBus.h>

// uncomment one of these that matches your panel pixel layouts and
// how you want them rotated.  Not all the rotations are listed here
// but you can modifiy the name to include the rotation of 90,180, or 270.

typedef ColumnMajorAlternatingLayout MyPanelLayout;
// typedef ColumnMajorLayout MyPanelLayout;
// typedef RowMajorAlternatingLayout MyPanelLayout;
// typedef RowMajorLayout MyPanelLayout;
// typedef RowMajor90Layout MyPanelLayout; // note rotation 90 was used

// make sure to set these panel values to the sizes of yours
const uint8_t PanelWidth = 8;  // 8 pixel x 8 pixel matrix of leds
const uint8_t PanelHeight = 8;
const uint16_t PixelCount = PanelWidth * PanelHeight;
const uint8_t PixelPin = 2;  // make sure to set this to the correct pin, ignored for Esp8266

NeoTopology<MyPanelLayout> topo(PanelWidth, PanelHeight);

NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);
//NeoPixelBus<NeoRgbFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);
//NeoPixelBus<NeoRgbwFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);
// for esp8266 omit the pin
//NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount);

RgbColor red(128, 0, 0);
RgbColor green(0, 128, 0);
RgbColor blue(0, 0, 128);
RgbColor white(128);
// if using NeoRgbwFeature above, use this white instead to use
// the correct white element of the LED
//RgbwColor white(128); 
RgbColor black(0);

const uint16_t left = 0;
const uint16_t right = PanelWidth - 1;
const uint16_t top = 0;
const uint16_t bottom = PanelHeight - 1;

void setup()
{
    Serial.begin(115200);
    while (!Serial); // wait for serial attach

    Serial.println();
    Serial.println("Initializing...");

    strip.Begin();
    strip.Show();

    Serial.println();
    Serial.println("Running...");
}

void loop()
{
    delay(2500);

    Serial.println();
    Serial.println("If your panel is correctly defined, you should see ...");
    Serial.println("Upper left is white.");
    Serial.println("Upper right is Red.");
    Serial.println("Lower right is Green");
    Serial.println("Lower Left is Blue");

    // use the topo to map the 2d cordinate to the pixel
    // and use that to SetPixelColor
    strip.SetPixelColor(topo.Map(left, top), white);
    strip.SetPixelColor(topo.Map(right, top), red);
    strip.SetPixelColor(topo.Map(right, bottom), green);
    strip.SetPixelColor(topo.Map(left, bottom), blue);
    strip.Show();

    delay(5000);

    Serial.println();
    Serial.println("Cleared to black ...");
    strip.ClearTo(black);
    strip.Show();
}

