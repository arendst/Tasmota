// NeoPixelBufferCylon
// This example will move a Cylon Red Eye back and forth across the 
// the full collection of pixels on the strip. 
//
// This will demonstrate the use of the NeoVerticalSpriteSheet 
// 

#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>

// The actual image is contained in the data structure in one of the Cylon*.h files
// You will need to use the one that has the same color feature as your NeoPixelBus
// There are two provided, but you can create your own easily enough using 
// free versions of Paint.Net and the plugin 
#include "CylonGrb.h"
typedef NeoGrbFeature MyPixelColorFeature;

// #include "CylonGrbw.h"
// typedef NeoGrbwFeature MyPixelColorFeature;

const uint16_t PixelCount = 16; // the sample images are meant for 16 pixels
const uint16_t PixelPin = 2;  
const uint16_t AnimCount = 1; // we only need one

NeoPixelBus<MyPixelColorFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);
// for esp8266 omit the pin
//NeoPixelBus<MyPixelColorFeature, Neo800KbpsMethod> strip(PixelCount);
NeoPixelAnimator animations(AnimCount); // NeoPixel animation management object

// sprite sheet stored in progmem using the same pixel feature as the NeoPixelBus
NeoVerticalSpriteSheet<NeoBufferProgmemMethod<MyPixelColorFeature>> spriteSheet(
        myImageWidth, // image width and sprite width since its vertical sprite sheet
        myImageHeight,  // image height
        1, // sprite is only one pixel high
        myImage);

uint16_t indexSprite;

void LoopAnimUpdate(const AnimationParam& param)
{
    // wait for this animation to complete,
    // we are using it as a timer of sorts
    if (param.state == AnimationState_Completed)
    {
        // done, time to restart this position tracking animation/timer
        animations.RestartAnimation(param.index);

        // draw the next frame in the sprite
        spriteSheet.Blt(strip, 0, indexSprite);
        indexSprite = (indexSprite + 1) % myImageHeight; // increment and wrap
    }
}

void setup()
{
    strip.Begin();
    strip.Show();

    indexSprite = 0;

    // we use the index 0 animation to time how often we rotate all the pixels
    animations.StartAnimation(0, 60, LoopAnimUpdate);
}


void loop()
{
    // this is all that is needed to keep it running
    // and avoiding using delay() is always a good thing for
    // any timing related routines
    animations.UpdateAnimations();
    strip.Show();
}

