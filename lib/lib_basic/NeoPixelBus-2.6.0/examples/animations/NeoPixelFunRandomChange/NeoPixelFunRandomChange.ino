// NeoPixelFunRandomChange
// This example will randomly select a number pixels and then
// start an animation to blend them from their current color to
// randomly selected a color
// 

#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>

const uint16_t PixelCount = 16; // make sure to set this to the number of pixels in your strip
const uint8_t PixelPin = 2;  // make sure to set this to the correct pin, ignored for Esp8266

NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);
// For Esp8266, the Pin is omitted and it uses GPIO3 due to DMA hardware use.  
// There are other Esp8266 alternative methods that provide more pin options, but also have
// other side effects.
// for details see wiki linked here https://github.com/Makuna/NeoPixelBus/wiki/ESP8266-NeoMethods 

NeoPixelAnimator animations(PixelCount); // NeoPixel animation management object

// what is stored for state is specific to the need, in this case, the colors.
// Basically what ever you need inside the animation update function
struct MyAnimationState
{
    RgbColor StartingColor;
    RgbColor EndingColor;
};

// one entry per pixel to match the animation timing manager
MyAnimationState animationState[PixelCount];

void SetRandomSeed()
{
    uint32_t seed;

    // random works best with a seed that can use 31 bits
    // analogRead on a unconnected pin tends toward less than four bits
    seed = analogRead(0);
    delay(1);

    for (int shifts = 3; shifts < 31; shifts += 3)
    {
        seed ^= analogRead(0) << shifts;
        delay(1);
    }

    // Serial.println(seed);
    randomSeed(seed);
}

// simple blend function
void BlendAnimUpdate(const AnimationParam& param)
{
    // this gets called for each animation on every time step
    // progress will start at 0.0 and end at 1.0
    // we use the blend function on the RgbColor to mix
    // color based on the progress given to us in the animation
    RgbColor updatedColor = RgbColor::LinearBlend(
        animationState[param.index].StartingColor,
        animationState[param.index].EndingColor,
        param.progress);
    // apply the color to the strip
    strip.SetPixelColor(param.index, updatedColor);
}

void PickRandom(float luminance)
{
    // pick random count of pixels to animate
    uint16_t count = random(PixelCount);
    while (count > 0)
    {
        // pick a random pixel
        uint16_t pixel = random(PixelCount);

        // pick random time and random color
        // we use HslColor object as it allows us to easily pick a color
        // with the same saturation and luminance 
        uint16_t time = random(100, 400);
        animationState[pixel].StartingColor = strip.GetPixelColor(pixel);
        animationState[pixel].EndingColor = HslColor(random(360) / 360.0f, 1.0f, luminance);

        animations.StartAnimation(pixel, time, BlendAnimUpdate);

        count--;
    }
}

void setup()
{
    strip.Begin();
    strip.Show();

    SetRandomSeed();
}


void loop()
{
    if (animations.IsAnimating())
    {
        // the normal loop just needs these two to run the active animations
        animations.UpdateAnimations();
        strip.Show();
    }
    else
    {
        // no animations runnning, start some 
        //
        PickRandom(0.2f); // 0.0 = black, 0.25 is normal, 0.5 is bright
    }
}



