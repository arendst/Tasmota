//----------------------------------------------------------------------
// NeoPixelRingTopologyTest
// This will display specific colors in specific locations on the led rings
//
// This is useful in confirming the layout of your rings
//
// It does require that you have the actual series of rings connected
//----------------------------------------------------------------------

#include <NeoPixelBus.h>

const uint8_t PixelCount = 119;
const uint8_t PixelPin = 2;  // make sure to set this to the correct pin, ignored for Esp8266

// define the layout of your series of rings
//
// This example is using all of Adafruits rings and a Jewel in the center.
// The center is the input and all the rings are connected in series going outward
//
// Rings:
// 0 - 1 (virtual ring, the center of the jewel)
// 1 - 6 (virtual ring, the outer ring of the jewel)
// 2 - 12 count ring
// 3 - 16 count ring
// 4 - 24 count ring
// 5 - 60 count ring comprised of four arc segments
//
// The values below in Rings[] are the index of the first pixel in each ring.
// An extra value is appended for a virtual ring start that also
// represents the total count of pixels in the complete series and this extra
// value is required.
//
class MyRingsLayout
{
protected:
    const uint16_t Rings[7] = {0, 1, 7, 19, 35, 59, PixelCount}; 
};

// use the MyRingsLayout to declare the topo object
//
NeoRingTopology<MyRingsLayout> topo;

// declare our strip
//
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);

// define some handy colors
//
RgbColor red(128, 0, 0);
RgbColor green(0, 128, 0);
RgbColor blue(0, 0, 128);
RgbColor black(0);

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
    Serial.println("First pixel in each ring is Red.");
    Serial.println("Middle pixel in each ring is Green.");
    Serial.println("Last Pixel in each ring is Blue.");
    

    // use the topo to map the 2d polar cordinate to the pixel
    // and use that to SetPixelColor
    for (uint16_t ring = 0; ring < topo.getCountOfRings(); ring++)
    {
        // first pixel in each ring is red
        strip.SetPixelColor(topo.Map(ring, 0), red); 
        // last pixel in each ring is blue
        strip.SetPixelColor(topo.Map(ring, topo.getPixelCountAtRing(ring) - 1), blue); 
        // middle pixel in each ring is green
        strip.SetPixelColor(topo.Map(ring, topo.getPixelCountAtRing(ring) / 2), green); 
    }
    strip.Show();

    delay(5000);

    Serial.println();
    Serial.println("Cleared to black ...");
    strip.ClearTo(black);
    strip.Show();
}

