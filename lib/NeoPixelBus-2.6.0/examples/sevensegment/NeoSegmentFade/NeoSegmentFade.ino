// NeoSegmentBus
// This example will demonstrate using the NeoSegmentBus which provides support for a
// seven segment LED digit driven by three WS2811; connected in series with other digits
//
// See https://shop.idlehandsdev.com/products/addressable-7-segment-display for a hardware example
//
// This example will print current seconds since start of the Arduino 
// with a digit animating a circling path for each second
//

#include <NeoPixelSegmentBus.h>
#include <NeoPixelAnimator.h>

const uint16_t DigitCount = 5; // Max Digits, not segments, not pixels
const uint8_t BusPin = 2;  // make sure to set this to the correct pin, ignored for Esp8266
const uint16_t CycleDigit = 0;
const uint16_t SecondsDigit = 1;

#define brightness 128

NeoPixelSegmentBus<SevenSegmentFeature, NeoWs2811Method> strip(DigitCount, BusPin);

enum Animation
{
    Animation_Cycle, // animation for the cycle indicator
    Animation_Fade, // animation for fade of seconds
    Animation_COUNT
};

NeoPixelAnimator animations(Animation_COUNT); 

void CycleAnimation(const AnimationParam& param)
{
    // calculate which segment should be on using the animation progress
    uint8_t bitfield = 1 << (uint8_t)(param.progress * LedSegment_F);
    // instant a digit with that segment on
    SevenSegDigit digit(bitfield, brightness);
    // apply it to the strip
    strip.SetPixelColor(CycleDigit, digit); 
}

// for the animation of fading the new number in, we use
// two digit DIBs (Device Independant Bitmaps) of SevenSegDigit to blend with; 
// each sized one less than the strip due to the first is a used for the cycle
// animation.
typedef NeoDib<SevenSegDigit> SevenSegDib;

SevenSegDib StartingDigits(DigitCount - 1); 
SevenSegDib EndingDigits(DigitCount - 1);

// shader class that will do the "string" blending
//
class DigitBlendShader 
{
public:
    // this shader always renders and doesn't track a dirty state
    bool IsDirty() const
    { 
        return true;
    }

    void ResetDirty() 
    {  
    }

    SevenSegDigit Apply(uint16_t indexDigit, SevenSegDigit digit)
    {
        // since we call EndingDigits.Render below, the digit argument is
        // from the EndingDigits so no need to call GetPixelColor to get it
        // create a digit that is a blend between the last seconds
        // value and the next seconds value using the BlendAmount
        SevenSegDigit blendDigit = SevenSegDigit::LinearBlend(
            StartingDigits.GetPixelColor(indexDigit),
            digit,
            BlendAmount);

        return blendDigit;
    }

    float BlendAmount;
};

// the instance of our shader class
DigitBlendShader blendShader;

void FadeAnimation(const AnimationParam& param)
{
    // set the shader property BlendAmount to the animation progress
    blendShader.BlendAmount = param.progress;
    // apply it to the strip at the SecondsDigit location
    EndingDigits.Render<SevenSegmentFeature, DigitBlendShader>(strip, 
        blendShader, 
        SecondsDigit);
}

uint32_t lastSeconds;

void setup()
{
    lastSeconds = millis() / 1000;

    strip.Begin();
    strip.Show(); 

    // init animation Dibs as cleared
    StartingDigits.ClearTo(0);
    EndingDigits.ClearTo(0);
}

void loop()
{
    uint32_t seconds = millis() / 1000;
    
    // when the seconds change, start animations for the update
    //
    if (seconds != lastSeconds)
    {
        // copy last animation ending digits as starting digits
        StartingDigits = EndingDigits;
        
        // format and display new value in ending digits dib
        String display(seconds);
        SevenSegDigit::SetString<SevenSegDib>(EndingDigits,
            0,
            display.c_str(),
            brightness);

        // start the seconds fade animation
        animations.StartAnimation(Animation_Fade, 1000, FadeAnimation);

        // start the cycle animation for the next second
        animations.StartAnimation(Animation_Cycle, 1000, CycleAnimation);

        lastSeconds = seconds;
    }

    if (animations.IsAnimating())
    {
        // the normal loop just needs these two to run the active animations
        animations.UpdateAnimations();
        strip.Show();
    }
}

