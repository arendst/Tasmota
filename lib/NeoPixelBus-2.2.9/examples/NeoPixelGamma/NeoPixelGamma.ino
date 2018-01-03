// NeoPixelGamma
// This example will display a timed series of color gradiants with gamma correction
// and then without.
// If the last pixel is on, then the colors being shown are color corrected.
// It will show Red grandiant, Green grandiant, Blue grandiant, a White grandiant, and
// then repeat.
// 
// This will demonstrate the use of the NeoGamma class
//
//

#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>

const uint16_t PixelCount = 16; // make sure to set this to the number of pixels in your strip
const uint8_t PixelPin = 2;  // make sure to set this to the correct pin, ignored for Esp8266

NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);
// for esp8266 omit the pin
//NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount);

// uncomment only one of these to compare memory use or speed
//
// NeoGamma<NeoGammaEquationMethod> colorGamma;
NeoGamma<NeoGammaTableMethod> colorGamma;

void DrawPixels(bool corrected, HslColor startColor, HslColor stopColor)
{
    for (uint16_t index = 0; index < strip.PixelCount() - 1; index++)
    {
        float progress = index / static_cast<float>(strip.PixelCount() - 2);
        RgbColor color = HslColor::LinearBlend<NeoHueBlendShortestDistance>(startColor, stopColor, progress);
        if (corrected)
        {
            color = colorGamma.Correct(color);
        }
        strip.SetPixelColor(index, color);
    }

    // use the last pixel to indicate if we are showing corrected colors or not
    if (corrected)
    {
        strip.SetPixelColor(strip.PixelCount() - 1, RgbColor(64));
    }
    else
    {
        strip.SetPixelColor(strip.PixelCount() - 1, RgbColor(0));
    }

    strip.Show();
}

void setup()
{
    strip.Begin();
    strip.Show();
}

void loop()
{
    HslColor startColor;
    HslColor stopColor;

    // red color 
    startColor = HslColor(0.0f, 1.0f, 0.0f);
    stopColor = HslColor(0.0f, 1.0f, 0.5f);
    DrawPixels(true, startColor, stopColor);
    delay(5000);
    DrawPixels(false, startColor, stopColor);
    delay(5000);

    // green color 
    startColor = HslColor(0.33f, 1.0f, 0.0f);
    stopColor = HslColor(0.33f, 1.0f, 0.5f);
    DrawPixels(true, startColor, stopColor);
    delay(5000);
    DrawPixels(false, startColor, stopColor);
    delay(5000);

    // blue color 
    startColor = HslColor(0.66f, 1.0f, 0.0f);
    stopColor = HslColor(0.66f, 1.0f, 0.5f);
    DrawPixels(true, startColor, stopColor);
    delay(5000);
    DrawPixels(false, startColor, stopColor);
    delay(5000);

    // white color 
    startColor = HslColor(0.0f, 0.0f, 0.0f);
    stopColor = HslColor(0.0f, 0.0f, 0.5f);
    DrawPixels(true, startColor, stopColor);
    delay(5000);
    DrawPixels(false, startColor, stopColor);
    delay(5000);
}