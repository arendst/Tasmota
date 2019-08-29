// NeoPixelBufferShader
// This example will provide a shader class to the NeoPixelBuffer that will dim and brighten
// the pixels that are in the buffer (a device dependent bitmap)
//

#include <NeoPixelBus.h>

const uint16_t PixelCount = 64; // set this to the size of your strip
const uint8_t PixelPin = 2;  // make sure to set this to the correct pin, ignored for Esp8266

// three element GRB pixels, change to your needs
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);

// the buffer object, 
// defined to use memory with the same feature as the strip
// initialized with the same number of pixels as our strip
NeoBuffer<NeoBufferMethod<NeoGrbFeature>> image(8,8,NULL);

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
template<typename T_COLOR_FEATURE> class BrightnessShader : public NeoShaderBase
{
public:
  BrightnessShader():
    NeoShaderBase(),
    _brightness(255) // default to full bright
  {}

  // required for a shader object, it will be called for
  // every pixel
  void Apply(uint16_t index, uint8_t* pDest, uint8_t* pSrc)
  {
    // we don't care what the index is so we ignore it
    //
    // to apply our brightness shader, 
    // use the source color, modify, and apply to the destination 
    
    // for every byte in the pixel,
    // scale the source value by the brightness and 
    // store it in the destination byte
    const uint8_t* pSrcEnd = pSrc + T_COLOR_FEATURE::PixelSize;
    while (pSrc != pSrcEnd)
    {
        *pDest++ = (*pSrc++ * (uint16_t(_brightness) + 1)) >> 8;
    }
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

// create an instance of our shader object with the same feature as our buffer
BrightnessShader<NeoGrbFeature> shader;

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
    uint8_t x = 0;
    uint8_t y = 0;
    image.SetPixelColor(x++, y, DarkRed);
    image.SetPixelColor(x++, y, DarkYellow);
    image.SetPixelColor(x++, y, DarkGreen);
    image.SetPixelColor(x++, y, DarkCyan);
    image.SetPixelColor(x++, y, DarkBlue);
    image.SetPixelColor(x++, y, DarkPurple);

    x = 0;
    y = 1;
    image.SetPixelColor(x++, y, BrightRed);
    image.SetPixelColor(x++, y, BrightYellow);
    image.SetPixelColor(x++, y, BrightGreen);
    image.SetPixelColor(x++, y, BrightCyan);
    image.SetPixelColor(x++, y, BrightBlue);
    image.SetPixelColor(x++, y, BrightPurple);
    
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
  image.Render<BrightnessShader<NeoGrbFeature>>(strip, shader);
  strip.Show();

}

