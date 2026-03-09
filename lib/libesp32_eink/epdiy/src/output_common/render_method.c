#include "render_method.h"
#include "sdkconfig.h"

#ifdef CONFIG_IDF_TARGET_ESP32
const enum EpdRenderMethod EPD_CURRENT_RENDER_METHOD = RENDER_METHOD_I2S;
#elif defined(CONFIG_IDF_TARGET_ESP32S3)
const enum EpdRenderMethod EPD_CURRENT_RENDER_METHOD = RENDER_METHOD_LCD;
#else
#error "unknown chip, cannot choose render method!"
#endif