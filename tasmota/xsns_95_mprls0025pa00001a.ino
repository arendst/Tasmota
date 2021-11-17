#include <SparkFun_MicroPressure.h>

#ifdef USE_I2C
#ifdef USE_MPR

#define XSNS_95                          95
#define XI2C_65                          65
#define MPR_I2C_ADDR                     0x18

typedef struct {
    SparkFun_MicroPressure lib;
    bool MPRSensorDetected     = false;     /** Status if sensor is connected */
    float current_pressure     = 0.00;      /** Current pressure reading **/
    float sealevel_pressure    = 0.00;      /** Sea level pressure **/
} MPR;
MPR mpr;



void MPRDetect(void) {
  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_I2C "MPR: MPRDetect()"));
  if (mpr.MPRSensorDetected) { return; }

  if (mpr.lib.begin(MPR_I2C_ADDR)) { 
    mpr.MPRSensorDetected = true;
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_I2C "MPR: MicroPressure init successful."));
  } else { 
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_I2C "MPR: begin() failed. check your MicroPressure Interface and I2C Address."));
  }
}

void MPRRead() {
  if (!mpr.MPRSensorDetected) { return; }

  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_I2C "MPR: MPRRead() OLD Pressure value = %f"), mpr.current_pressure);
  mpr.current_pressure = mpr.lib.readPressure(HPA);
  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_I2C "MPR: MPRRead() NEW Pressure value = %f"), mpr.current_pressure);

  // Convert to sea level pressure
  mpr.sealevel_pressure = ConvertPressureForSeaLevel(mpr.current_pressure);
}

void MPRShowJson() {
  if (!mpr.MPRSensorDetected) { return; }

  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_I2C "MPR: MPRShowJson() Pressure value = %f"), mpr.current_pressure);
  ResponseAppend_P(PSTR(",\"%s\":{\"" D_JSON_PRESSURE "\":%f }"), "MPR", mpr.current_pressure);
}

#ifdef USE_WEBSERVER
void MPRShowWeb() {
  if (!mpr.MPRSensorDetected) { return; }

  char current_pressure_str[33];          /** Current pressure as string **/
  dtostrfd(mpr.current_pressure, Settings->flag2.pressure_resolution, current_pressure_str);
  char sea_pressure_str[33];              /** Sea level pressure as string **/
  dtostrfd(mpr.sealevel_pressure, Settings->flag2.pressure_resolution, sea_pressure_str);

  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_I2C "MPR: MPRShowWeb() Pressure value = %f"), mpr.current_pressure);
  WSContentSend_PD(HTTP_SNS_PRESSURE, "MPR", current_pressure_str, PressureUnit().c_str());
  if (Settings->altitude != 0) {
    WSContentSend_PD(HTTP_SNS_SEAPRESSURE, "MPR", sea_pressure_str, PressureUnit().c_str());
  }
}
#endif  // USE_WEBSERVER

bool Xsns95(uint8_t function) {
  if (!I2cEnabled(XI2C_65)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_I2C "MPR: FUNC_INIT"));
    MPRDetect();
  }
  else if (mpr.MPRSensorDetected) {
    switch (function) {
      case FUNC_EVERY_SECOND:
        MPRRead();
        break;
      case FUNC_JSON_APPEND:
        MPRShowJson();
        break;
      case FUNC_SAVE_BEFORE_RESTART:
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        MPRShowWeb();
        break;
#endif  // USE_WEBSERVER
    }
  }
  
  return result;
}

#endif  // USE_MPR
#endif  // USE_I2C
