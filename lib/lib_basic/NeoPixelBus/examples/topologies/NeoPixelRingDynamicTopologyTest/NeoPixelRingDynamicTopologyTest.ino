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
public:
    void Begin() {
      // this is where you load your dynamic rings layout and init Rings and RingCount
      // this example will just set these to static numbers to simulate a dynamic layout
      RingCount = 6;
      Rings = new uint16_t[RingCount];

      Rings[0] = 1;
      Rings[1] = 6;
      Rings[2] = 12;
      Rings[3] = 16;
      Rings[4] = 24;
      Rings[5] = 60; // don't forget the final count of pixels as the last item
    }

protected:
    uint16_t* Rings; 
    uint8_t RingCount;
    
    uint8_t _ringCount() const
    {
        return RingCount;
    }
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

    topo.Begin();

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
