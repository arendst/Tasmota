#ifdef USE_I2C
#ifdef USE_MEMS_GAS

#define XSNS_120           120
#define XI2C_96            96         // See I2CDEVICES.md

#include <Wire.h>
#include <DFRobot_MICS.h>

#define Mics_I2C_ADDRESS 0x75
TwoWire& myWire = I2cGetWire();
DFRobot_MICS_I2C mics(&Wire, Mics_I2C_ADDRESS);

#define CALIBRATION_TIME   3   

bool DFRobot_MICS_start = false;
bool DFRobot_MICS_begin = false;
bool DFRobot_MICS_wakeup = false;
bool DFRobot_MICS_warmup = false;


struct {
  int loop_count = 0;
  float Methane = 0.0f;
  float Ethanol = 0.0f;
  float Hydrogen = 0.0f;
  float Ammonia = 0.0f;
  float Carbon_Monoxide = 0.0f;
  float Nitrogen_Dioxide = 0.0f;    

  int8_t Carbon_Monoxide_b = 0x00;
  int8_t Methane_b = 0x00;
  int8_t Ethanol_b = 0x00;
  int8_t Propane_b = 0x00;
  int8_t Iso_Butane_b = 0x00;
  int8_t Hydrogen_b = 0x00;
  int8_t Hydrothion_b = 0x00;
  int8_t Ammonia_b    = 0x00;
  int8_t Nitric_Oxide_b = 0x00;
  int8_t Nitrogen_Dioxide_b    = 0x00;
  
} DFRobot_MICS_data;

struct {

  char Methane[8];
  char Ethanol[8];
  char Hydrogen[8];
  char Ammonia[8];
  char Carbon_Monoxide[8];
  char Nitrogen_Dioxide[8];  
  char Carbon_Monoxide_b[8];
  char Methane_b[8];
  char Ethanol_b[8];
  char Propane_b[8];
  char Iso_Butane_b[8];
  char Hydrogen_b[8];
  char Hydrothion_b[8];
  char Ammonia_b[8];
  char Nitric_Oxide_b[8];
  char Nitrogen_Dioxide_b[8];
  
} DFRobot_MICS_ch;

void DFRobot_MICS_init(void) {
  AddLog(LOG_LEVEL_INFO, PSTR("DFROBOT: howdy sir"));
  if (!DFRobot_MICS_begin) {
    DFRobot_MICS_begin = mics.begin();
  }
  else if (!DFRobot_MICS_wakeup) {
    AddLog(LOG_LEVEL_INFO, PSTR("DFROBOT: howdy wakeup sir"));  
    uint8_t mode = mics.getPowerState();
        if(mode == SLEEP_MODE){
            mics.wakeUpMode();
        //  Serial.println("wake up sensor success!");
        }
      
      DFRobot_MICS_wakeup = true;
      AddLog(LOG_LEVEL_INFO, PSTR("DFROBOT: howdy end wakeup sir"));  
  }
  else if (!DFRobot_MICS_warmup) {
    AddLog(LOG_LEVEL_INFO, PSTR("DFROBOT: howdy wormup sir"));  
    DFRobot_MICS_warmup = mics.warmUpTime(CALIBRATION_TIME);
      AddLog(LOG_LEVEL_INFO, PSTR("DFROBOT: howdy end warmup sir"));
  }
  
}

void convertGasDataToString(void) {
  // Convert float values using dtostrf()
  dtostrfd(DFRobot_MICS_data.Methane, 2, DFRobot_MICS_ch.Methane);
  dtostrfd(DFRobot_MICS_data.Ethanol,  2, DFRobot_MICS_ch.Ethanol);
  dtostrfd(DFRobot_MICS_data.Hydrogen,  2, DFRobot_MICS_ch.Hydrogen);
  dtostrfd(DFRobot_MICS_data.Ammonia,  2, DFRobot_MICS_ch.Ammonia);
  dtostrfd(DFRobot_MICS_data.Carbon_Monoxide,  2, DFRobot_MICS_ch.Carbon_Monoxide);
  dtostrfd(DFRobot_MICS_data.Nitrogen_Dioxide, 2, DFRobot_MICS_ch.Nitrogen_Dioxide);

  // Convert int8_t values using itoa()
  itoa(DFRobot_MICS_data.Carbon_Monoxide_b, DFRobot_MICS_ch.Carbon_Monoxide_b, 10);
  itoa(DFRobot_MICS_data.Methane_b, DFRobot_MICS_ch.Methane_b, 10);
  itoa(DFRobot_MICS_data.Ethanol_b, DFRobot_MICS_ch.Ethanol_b, 10);
  itoa(DFRobot_MICS_data.Propane_b, DFRobot_MICS_ch.Propane_b, 10);
  itoa(DFRobot_MICS_data.Iso_Butane_b, DFRobot_MICS_ch.Iso_Butane_b, 10);
  itoa(DFRobot_MICS_data.Hydrogen_b, DFRobot_MICS_ch.Hydrogen_b, 10);
  itoa(DFRobot_MICS_data.Hydrothion_b, DFRobot_MICS_ch.Hydrothion_b, 10);
  itoa(DFRobot_MICS_data.Ammonia_b, DFRobot_MICS_ch.Ammonia_b, 10);
  itoa(DFRobot_MICS_data.Nitric_Oxide_b, DFRobot_MICS_ch.Nitric_Oxide_b, 10);
  itoa(DFRobot_MICS_data.Nitrogen_Dioxide_b, DFRobot_MICS_ch.Nitrogen_Dioxide_b, 10);
}


void DFRobot_MICS_show(bool json) {
  AddLog(LOG_LEVEL_INFO, PSTR("DFROBOT: howdy show sir"));
  convertGasDataToString();
    if (json) {
      ResponseAppend_P(PSTR(
        ",\"DFRobot_MICS\":{"
        "\"Methane\":\"%s\","
        "\"Ethanol\":\"%s\","
        "\"Hydrogen\":\"%s\","
        "\"Ammonia\":\"%s\","
        "\"Carbon_Monoxide\":\"%s\","
        "\"Nitrogen_Dioxide\":\"%s\","
    
        "\"Carbon_Monoxide_b\":%s,"
        "\"Methane_b\":%s,"
        "\"Ethanol_b\":%s,"
        "\"Propane_b\":%s,"
        "\"Iso_Butane_b\":%s,"
        "\"Hydrogen_b\":%s,"
        "\"Hydrothion_b\":%s,"
        "\"Ammonia_b\":%s,"
        "\"Nitric_Oxide_b\":%s,"
        "\"Nitrogen_Dioxide_b\":%s"
        "}"), 
    
        DFRobot_MICS_ch.Methane,
        DFRobot_MICS_ch.Ethanol,
        DFRobot_MICS_ch.Hydrogen,
        DFRobot_MICS_ch.Ammonia,
        DFRobot_MICS_ch.Carbon_Monoxide,
        DFRobot_MICS_ch.Nitrogen_Dioxide,
    
        DFRobot_MICS_ch.Carbon_Monoxide_b,
        DFRobot_MICS_ch.Methane_b,
        DFRobot_MICS_ch.Ethanol_b,
        DFRobot_MICS_ch.Propane_b,
        DFRobot_MICS_ch.Iso_Butane_b,
        DFRobot_MICS_ch.Hydrogen_b,
        DFRobot_MICS_ch.Hydrothion_b,
        DFRobot_MICS_ch.Ammonia_b,
        DFRobot_MICS_ch.Nitric_Oxide_b,
        DFRobot_MICS_ch.Nitrogen_Dioxide_b
    );
      //  ResponseAppend_P(PSTR("}"));
      AddLog(LOG_LEVEL_INFO, PSTR(",\"DFRobotwritng data}" ));
    }
}

void DFRobot_MICS_measure(void) {
  AddLog(LOG_LEVEL_INFO, PSTR("DFROBOT: howdy measurement sir"));

  float tempFloat;  // Temporary variable for getGasData() (float)
  int8_t tempInt;   // Temporary variable for getGasExist() (int8_t)

  switch (DFRobot_MICS_data.loop_count) {
      case 0:
          tempFloat = mics.getGasData(CH4);
          if (tempFloat > DFRobot_MICS_data.Methane) DFRobot_MICS_data.Methane = tempFloat;
          break;
      case 1:
          tempFloat = mics.getGasData(C2H5OH);
          if (tempFloat > DFRobot_MICS_data.Ethanol) DFRobot_MICS_data.Ethanol = tempFloat;
          break;
      case 2:
          tempFloat = mics.getGasData(H2);
          if (tempFloat > DFRobot_MICS_data.Hydrogen) DFRobot_MICS_data.Hydrogen = tempFloat;
          break;
      case 3:
          tempFloat = mics.getGasData(NH3);
          if (tempFloat > DFRobot_MICS_data.Ammonia) DFRobot_MICS_data.Ammonia = tempFloat;
          break;
      case 4:
          tempFloat = mics.getGasData(CO);
          if (tempFloat > DFRobot_MICS_data.Carbon_Monoxide) DFRobot_MICS_data.Carbon_Monoxide = tempFloat;
          break;
      case 5:
          tempFloat = mics.getGasData(NO2);
          if (tempFloat > DFRobot_MICS_data.Nitrogen_Dioxide) DFRobot_MICS_data.Nitrogen_Dioxide = tempFloat;
          break;
      case 6:
          tempInt = mics.getGasExist(CO);
          if (tempInt > DFRobot_MICS_data.Carbon_Monoxide_b) DFRobot_MICS_data.Carbon_Monoxide_b = tempInt;
          break;
      case 7:
          tempInt = mics.getGasExist(CH4);
          if (tempInt > DFRobot_MICS_data.Methane_b) DFRobot_MICS_data.Methane_b = tempInt;
          break;
      case 8:
          tempInt = mics.getGasExist(C2H5OH);
          if (tempInt > DFRobot_MICS_data.Ethanol_b) DFRobot_MICS_data.Ethanol_b = tempInt;
          break;
      case 9:
          tempInt = mics.getGasExist(C3H8);
          if (tempInt > DFRobot_MICS_data.Propane_b) DFRobot_MICS_data.Propane_b = tempInt;
          break;
      case 10:
          tempInt = mics.getGasExist(C4H10);
          if (tempInt > DFRobot_MICS_data.Iso_Butane_b) DFRobot_MICS_data.Iso_Butane_b = tempInt;
          break;
      case 11:
          tempInt = mics.getGasExist(H2);
          if (tempInt > DFRobot_MICS_data.Hydrogen_b) DFRobot_MICS_data.Hydrogen_b = tempInt;
          break;
      case 12:
          tempInt = mics.getGasExist(H2S);
          if (tempInt > DFRobot_MICS_data.Hydrothion_b) DFRobot_MICS_data.Hydrothion_b = tempInt;
          break;
      case 13:
          tempInt = mics.getGasExist(NH3);
          if (tempInt > DFRobot_MICS_data.Ammonia_b) DFRobot_MICS_data.Ammonia_b = tempInt;
          break;
      case 14:
          tempInt = mics.getGasExist(NO);
          if (tempInt > DFRobot_MICS_data.Nitric_Oxide_b) DFRobot_MICS_data.Nitric_Oxide_b = tempInt;
          break;
      case 15:
          tempInt = mics.getGasExist(NO2);
          if (tempInt > DFRobot_MICS_data.Nitrogen_Dioxide_b) DFRobot_MICS_data.Nitrogen_Dioxide_b = tempInt;
          DFRobot_MICS_data.loop_count = 0; 
          return;
  }

  DFRobot_MICS_data.loop_count++;
}


bool Xsns120(uint32_t function) {
  if (!I2cEnabled(XI2C_96)) { return false; }

  bool result = false;

  if (!DFRobot_MICS_warmup && (FUNC_EVERY_SECOND == function) && (TasmotaGlobal.uptime > 12)) {
    DFRobot_MICS_init();
  }
  else if (DFRobot_MICS_warmup) {
    switch (function) {
      case FUNC_EVERY_SECOND:
        DFRobot_MICS_measure();
        break;
      case FUNC_JSON_APPEND:
      DFRobot_MICS_show(1);
        break;
    }
  }
  return result;
}




#endif  // USE_MEMS_GAS
#endif  // USE_I2C