// NeoPixelBuffer
// This example will animate pixels using a bitmap stored on a SD card
// 
//
// This will demonstrate the use of the NeoBitmapFile object 
// NOTE:  The images provided in the example directory should be copied to
// the root of the SD card so the below code will find it.
// NOTE:  This sample and the included images were built for a 144 pixel strip so
// running this with a smaller string may not look as interesting.  Try providing
// your own 24 bit bitmap for better results.

#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>
#include <SPI.h> 
#include <SD.h> 

const int chipSelect = D8; // make sure to set this to your SD carder reader CS

//typedef NeoGrbFeature MyPixelColorFeature;
typedef NeoGrbwFeature MyPixelColorFeature;

const uint16_t PixelCount = 144; // the sample images are meant for 144 pixels
const uint16_t PixelPin = 2;
const uint16_t AnimCount = 1; // we only need one

NeoPixelBus<MyPixelColorFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);
// for esp8266 omit the pin
//NeoPixelBus<MyPixelColorFeature, Neo800KbpsMethod> strip(PixelCount);
NeoPixelAnimator animations(AnimCount); // NeoPixel animation management object

// our NeoBitmapFile will use the same color feature as NeoPixelBus and
// we want it to use the SD File object 
NeoBitmapFile<MyPixelColorFeature, File> image;

uint16_t animState;

void LoopAnimUpdate(const AnimationParam& param)
{
    // wait for this animation to complete,
    // we are using it as a timer of sorts
    if (param.state == AnimationState_Completed)
    {
        // done, time to restart this position tracking animation/timer
        animations.RestartAnimation(param.index);

        // draw the complete row at animState to the complete strip
        image.Blt(strip, 0, 0, animState, image.Width());
        animState = (animState + 1) % image.Height(); // increment and wrap
    }
}

void setup() {
    Serial.begin(115200);
    while (!Serial); // wait for serial attach

    strip.Begin();
    strip.Show();

    Serial.print("Initializing SD card...");

    // see if the card is present and can be initialized:
    if (!SD.begin(chipSelect))
    {
        Serial.println("Card failed, or not present");
        // don't do anything more:
        return;
    }
    Serial.println("card initialized.");

    // open the file
    File bitmapFile = SD.open("strings.bmp"); 
    if (!bitmapFile)
    {
        Serial.println("File open fail, or not present");
        // don't do anything more:
        return;
    }

    // initialize the image with the file
    if (!image.Begin(bitmapFile))
    {
        Serial.println("File format fail, not a supported bitmap");
        // don't do anything more:
        return;
    }

    animState = 0;
    // we use the index 0 animation to time how often we rotate all the pixels
    animations.StartAnimation(0, 30, LoopAnimUpdate);
}

void loop() {
    // this is all that is needed to keep it running
    // and avoiding using delay() is always a good thing for
    // any timing related routines
    animations.UpdateAnimations();
    strip.Show();
}