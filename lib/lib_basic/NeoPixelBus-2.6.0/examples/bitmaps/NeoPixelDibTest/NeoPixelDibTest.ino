// NeoPixelDibTest
// This example will provide a shader class to the NeoPixelDib that will dim and brighten
// the pixels that are in the Dib (Device Independant Bitmap)
//

#include <NeoPixelBus.h>

const uint16_t PixelCount = 64; // set this to the size of your strip
const uint8_t PixelPin = 2;  // make sure to set this to the correct pin, ignored for Esp8266

// three element GRB pixels, change to your needs
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);

// the DIB object, using RgbColor and initialized with the same number of pixels as our strip
NeoDib<RgbColor> image(PixelCount);

const RgbColor BrightRed(255, 0, 0);
const RgbColor BrightGreen(0, 255, 0);
const RgbColor BrightBlue(0, 0, 255);

const RgbColor BrightYellow(255, 255, 0);
const RgbColor BrightCyan(0, 255, 255);
const RgbColor BrightPurple(255, 0, 255);

const RgbColor DarkRed(32, 0, 0);
const RgbColor DarkGreen(0, 32, 0);
const RgbColor DarkBlue(0, 0, 32);

const RgbColor DarkYellow(32, 32, 0);
const RgbColor DarkCyan(0, 32, 32);
const RgbColor DarkPurple(32, 0, 32);

const RgbColor White(255);
const RgbColor Black(0);

// define a custom shader object that provides brightness support
// based upon the NeoShaderBase
class BrightnessShader : public NeoShaderBase
{
public:
  BrightnessShader():
    NeoShaderBase(),
    _brightness(255) // default to full bright
  {}

  // required for a shader object, it will be called for
  // every pixel
  RgbColor Apply(uint16_t index, RgbColor original)
  {
    // we don't care what the index is so we ignore it
    //
    // to apply our brightness shader, modify the original color and return the color we want
    // blend from black (_brightness == 0.0) to the original color (_brightness == 1.0)
    
    return RgbColor::LinearBlend(Black, original, (float)_brightness / 255.0f);
  }

  // provide an accessor to set brightness
  void setBrightness(uint8_t brightness)
  {
    _brightness = brightness;
    Dirty(); // must call dirty when a property changes
  }

  // provide an accessor to get brightness
  uint8_t getBrightness()
  {
    return _brightness;
  }
  
private:
  uint8_t _brightness;    
};

// create an instance of our shader object
BrightnessShader shader;

// some dimming tracking variables and settings
int8_t delta;

void setup()
{
    Serial.begin(115200);
    while (!Serial); // wait for serial attach

    Serial.println();
    Serial.println("Initializing...");
    Serial.flush();

    // this resets all the neopixels to an off state
    strip.Begin();
    strip.Show();

    // dibs do not default to any color, 
    // so clear it to black if you aren't going to draw
    // into every pixel
    image.ClearTo(Black); 

    // draw a pattern into the image
    uint8_t index = 0;
    image.SetPixelColor(index++, DarkRed);
    image.SetPixelColor(index++, DarkYellow);
    image.SetPixelColor(index++, DarkGreen);
    image.SetPixelColor(index++, DarkCyan);
    image.SetPixelColor(index++, DarkBlue);
    image.SetPixelColor(index++, DarkPurple);

    image.SetPixelColor(index++, Black);
    image.SetPixelColor(index++, Black);

    image.SetPixelColor(index++, BrightRed);
    image.SetPixelColor(index++, BrightYellow);
    image.SetPixelColor(index++, BrightGreen);
    image.SetPixelColor(index++, BrightCyan);
    image.SetPixelColor(index++, BrightBlue);
    image.SetPixelColor(index++, BrightPurple);
    
    Serial.println();
    Serial.println("Running...");

    delta = -1; // start by dimming downward
}

void loop()
{
  // we increment by delta every 30ms
  delay(30);
  
  // update the brightness in shader
  //
  uint8_t brightness = shader.getBrightness();
  // check if we flip directions
  if (brightness == 0)
  {
    delta = 1; // increment
  }
  else if (brightness == 255)
  {
    delta = -1; // decrement
  }
  // modify and apply
  brightness += delta;
  shader.setBrightness(brightness);
  
  Serial.println(brightness);
   
  
  // render the image using the shader and then call Show()
  // these two should be called together in order
  //

  // need to provide the type of color feature for the strip and
  // the type of our custom shader
  image.Render<NeoGrbFeature, BrightnessShader>(strip, shader);
  strip.Show();

}

