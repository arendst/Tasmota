// DotStarTest_Esp32Advanced - This example only works on the ESP32
// This example will cycle between showing four pixels as Red, Green, Blue, White
// and then showing those pixels as Black.  This example uses DMA to drive Hardware SPI on the ESP32
// with options to use alternate pins for SPI and drive two SPI ports
//
// There is serial output of the current state so you can confirm and follow along
//

#include <NeoPixelBus.h>

#define USE_DEFAULT_SPI_PORT 1
#define USE_ALTERNATE_SPI_PORT 1

#if (USE_DEFAULT_SPI_PORT == 1)
    const uint16_t PixelCount = 4; // this example assumes 4 pixels, making it smaller will cause a failure

    // It's usually better to use alternate pins.  If set to false, strip will use GPIO 18 for Clock, GPIO 23 for Data, and pin 5 will output a chip select signal
    const bool useSpiAlternatePins = true;

    // If you set useSpiAlternatePins true, then these pins will be used instead.  Any output-capable GPIO can be used.
    const uint8_t DotClockPin = 18;
    const uint8_t DotDataPin = 23;  
    const int8_t DotChipSelectPin = -1; // -1 means the chip select signal won't be output, freeing up one pin compared to useSpiAlternatePins=false

    // for software bit bang (only use if neither SPI peripheral is available)
    //NeoPixelBus<DotStarBgrFeature, DotStarMethod> strip(PixelCount, DotClockPin, DotDataPin);

    // for hardware SPI (best performance) with default SPI peripheral 
    NeoPixelBus<DotStarBgrFeature, DotStarEsp32DmaVspiMethod> strip(PixelCount);

    // DotStarEsp32DmaVspiMethod defaults to 10MHz clock speed.  For other speeds, replace "DotStarSpiMethod" with another method specifying speed, e.g. "DotStarSpi2MhzMethod" (see wiki for more details)
    // See DotStarTest_Esp32Advanced example for how to set clock speed at runtime
#endif

#if (USE_ALTERNATE_SPI_PORT == 1)
    const uint16_t PixelCount2 = 4; // this example assumes 4 pixels, making it smaller will cause a failure

    // It's usually better to use alternate pins.  If set to false, strip2 will use GPIO 14 for Clock, GPIO 13 for Data, and pin 15 will output a chip select signal
    const bool useSpiAlternatePins2 = true;

    // If you set useSpiAlternatePins2 true, then these pins will be used instead.  Any output-capable GPIO can be used.
    const uint8_t DotClockPin2 = 14;
    const uint8_t DotDataPin2 = 13;  
    const int8_t DotChipSelectPin2 = -1; // -1 means the chip select signal won't be output, freeing up one pin compared to useSpiAlternatePins2=false

    // for hardware SPI (best performance) with alternate SPI peripheral
    NeoPixelBus<DotStarBgrFeature, DotStarEsp32DmaHspiMethod> strip2(PixelCount2);

    // DotStarEsp32DmaHspiMethod defaults to 10MHz clock speed.  For other speeds, replace "DotStarSpiMethod" with another method specifying speed, e.g. "DotStarHspi2MhzMethod" (see wiki for more details)
#endif

#define colorSaturation 128

// Note that both DotStarEsp32DmaVspiMethod and DotStarEsp32DmaHspiMethod can be used with DotStarLbgrFeature and DotStarWbgrFeature but to keep things simple those are excluded from this example, see DotStarTest for more details

RgbColor red(colorSaturation, 0, 0);
RgbColor green(0, colorSaturation, 0);
RgbColor blue(0, 0, colorSaturation);
RgbColor white(colorSaturation);
RgbColor black(0);

void setup()
{
    Serial.begin(115200);
    while (!Serial); // wait for serial attach

    Serial.println();
    Serial.println("Initializing...");
    Serial.flush();

#if (USE_DEFAULT_SPI_PORT == 1)
    if (useSpiAlternatePins)
    {
        strip.Begin(DotClockPin, DotDataPin, DotDataPin, DotChipSelectPin);
    }
    else
    {
        strip.Begin();
    }

    strip.ClearTo(black);   // this resets all the DotStars to an off state
    strip.Show();
#endif

#if (USE_ALTERNATE_SPI_PORT == 1)
    if (useSpiAlternatePins2)
    {
        strip2.Begin(DotClockPin2, DotDataPin2, DotDataPin2, DotChipSelectPin2);
    }
    else
    {
        strip2.Begin();
    }

    strip2.ClearTo(black);  // this resets all the DotStars to an off state
    strip2.Show();
#endif

    Serial.println();
    Serial.println("Running...");
}

void loop()
{
    delay(500);

#if (USE_DEFAULT_SPI_PORT == 1)
    Serial.println("Default SPI Colors R, G, B, W...");
    // set the colors, 
    strip.SetPixelColor(0, red);
    strip.SetPixelColor(1, green);
    strip.SetPixelColor(2, blue);
    strip.SetPixelColor(3, white);
    strip.Show();
#endif

#if (USE_ALTERNATE_SPI_PORT == 1)
    Serial.println("Alt SPI Colors W, B, G, R...");
    // set the colors, 
    strip2.SetPixelColor(0, white);
    strip2.SetPixelColor(1, blue);
    strip2.SetPixelColor(2, green);
    strip2.SetPixelColor(3, red);
    strip2.Show();
#endif

    delay(500);

    Serial.println("Off ...");

#if (USE_DEFAULT_SPI_PORT == 1)
    // turn off the pixels
    strip.SetPixelColor(0, black);
    strip.SetPixelColor(1, black);
    strip.SetPixelColor(2, black);
    strip.SetPixelColor(3, black);
    strip.Show();
#endif

#if (USE_ALTERNATE_SPI_PORT == 1)
    // turn off the pixels
    strip2.SetPixelColor(0, black);
    strip2.SetPixelColor(1, black);
    strip2.SetPixelColor(2, black);
    strip2.SetPixelColor(3, black);
    strip2.Show();
#endif
}
