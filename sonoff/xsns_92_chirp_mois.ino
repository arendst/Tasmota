/*
  xsns_20_chirp_mois.ino - CHIRP ambient light sensor support for Sonoff-Tasmota

  Copyright (C) 2017  Theo Arends - modyfied by RL

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifdef USE_I2C

#ifdef USE_CHIRP

//https://github.com/Apollon77/I2CSoilMoistureSensor/blob/master/I2CSoilMoistureSensor.cpp
#include <I2CSoilMoistureSensor.h>

/*********************************************************************************************\
 * CHIRP - Ambient Light Intensity
\*********************************************************************************************/

#define CHIRP_ADDR1         0x20

#define CHIRP_LIGHT_CALIB 65535  //calibrate darknes value for light sensor
#define CHIRP_CAPACITANCE_MIN 235  //calibrate CAPACITANCE min value
#define CHIRP_CAPACITANCE_MAX 750  //c0calibrate CAPACITANCE max value
#define CHIRP_TEMP_CAL (0)  //calibrate TEMP value

/*********************************************************************************************\
 * global
\*********************************************************************************************/

uint8_t chirpaddr = CHIRP_ADDR1;
uint8_t chirptype = 0;
uint16_t light = 0;
boolean success = false;
float readmoisture = 0;
float readtemperature = 0;
char temperature[5];
char moisture[5];
char * light_read;
uint8_t ver = 0;
char chirpstype[7];
byte count = 0;


I2CSoilMoistureSensor sensor(chirpaddr);


/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

#ifdef USE_WEBSERVER
 #ifndef USE_BH1750  // avoid duplicate definition
  const char HTTP_SNS_ILLUMINANCE[] PROGMEM =  "%s{s}%s " D_ILLUMINANCE "{m}%d%%{e}";
 #endif //USE_BH1750
  const char HTTP_SNS_MOISTURE[] PROGMEM = "%s{s}%s " "Wilgoć" "{m}%s%%{e}";
  //const char HTTP_SNS_CHIRPTEMP[] PROGMEM = "%s{s}%s " D_JSON_TEMPERATURE ": {m}%s&deg;C{e}";
  const char HTTP_SNS_VER[] PROGMEM =  "%s{s}%s Firmware ver {m}0x%2x{e}";
#endif // USE_WEBSERVER
const char JSON_SNS_LIGHTMOISTTEMP[] PROGMEM = "%s,\"%s\":{\"" D_JSON_LIGHT "\":%d,\"" D_JSON_MOISTURE "\":%s,\"" D_JSON_TEMPERATURE "\":%s}";

/*********************************************************************************************\
 * Functions
\*********************************************************************************************/

uint8_t I2cScan_() {
   byte error;
   byte address;
   uint8_t address_st;
   byte any = 0;


  for (address = 7; address <= 117; address++) {
    Wire.beginTransmission(address);

    error = Wire.endTransmission();

    if (0 == error) {
        any = 1;
        address_st = address;
      } else if (4 == error) {
        continue;
        any = 0;
     }
   }
    if (any) {
      return address_st;
     } else {
      return 0;
    }
}


uint16_t chirp_readLux() {

  uint8_t testing = 0;
  sensor.startMeasureLight();

  do {

    //yield();
    delay(100);
    testing++;
  } while (sensor.isBusy() && testing < 50);

  return sensor.getLight(false);

}


boolean chirp_detect() {

  uint8_t scan = 0;
  byte counter = 0;

  Wire.beginTransmission(chirpaddr);

  if (0==Wire.endTransmission()){

      sensor.startMeasureLight();

      success=true;


   } else {

      success=false;

   }


  scan = I2cScan_();

  if (scan != 0) {

        if (scan != 0 && scan >=7 && scan <= 117 ){

        if (I2cWrite(scan, SOILMOISTURESENSOR_SET_ADDRESS, 0x20, 1))  {
            I2cWrite(scan, SOILMOISTURESENSOR_RESET, 0, 0);

               while (sensor.isBusy() && counter < 20) {
                //  yield();
                  delay(100);
                  counter++;
               }

        }



        success = true;
        chirptype = 1;

    }

      #ifdef CHIRP_CONTINUOUS_HIGH_RES_MODE
        I2cWrite(chirpaddr, CHIRP_CONTINUOUS_HIGH_RES_MODE, 1, 0);
      #endif

    } else if (scan == 0 || scan < 7 || scan > 117 ) {

      snprintf_P(log_data, sizeof(log_data), PSTR("{CHRIP: " D_JSON_I2CSCAN_UNKNOWN_ERROR_AT  ": 0x%2x}"), scan);
      AddLog(LOG_LEVEL_INFO);

      success = false;
      chirptype = 0;

    }   else  {

      success = true;
      chirptype = 1;
    }

  switch (success) {

    case false:

      chirptype = 0;
      snprintf_P(log_data, sizeof(log_data), PSTR("CHRIP: " D_JSON_I2CSCAN_NO_DEVICES_FOUND ": 0x%2x"), chirpaddr);
      AddLog(LOG_LEVEL_INFO);


      break;

    case true:
    chirptype = 1;
    chirpaddr = CHIRP_ADDR1;


    break;
  }

  return success;
}


boolean chirp_Get() {


boolean status = false;


///--------------sensor exists -----------------

 if (!I2cDevice(chirpaddr) ) {

    chirptype = 0;

      snprintf_P(log_data, sizeof(log_data), PSTR("CHRIP: " D_JSON_I2CSCAN_NO_DEVICES_FOUND ": 0x%2x !!!"), chirpaddr);
      AddLog(LOG_LEVEL_INFO);

      count++;

       switch (count % 5) {
        case 0:
        ExecuteCommandPower(1,7);
        ExecuteCommandPower(1,6);
        //yield();
        delay(100);
        ExecuteCommandPower(1,7);
        break;
       }
       switch (count % 10) {
        case 0:
        AddLog_P(LOG_LEVEL_DEBUG, PSTR("CHRIP Retraing..."));

         if (chirp_detect()) {
          chirptype = 1;
          count = 0;
         }
        break;
       }
     status = false;
 } else {
    status = true;
    count = 0;
    AddLog_P(LOG_LEVEL_DEBUG, PSTR("CHRIP OK."));
    chirpaddr = CHIRP_ADDR1;
    chirptype = 1;
/// ----- sensor is avaliable ------------------

   if (chirptype == 1) {
     //sensor.getVersion();

     uint8_t counter = 0;
     uint8_t test = 0;
     strcpy(chirpstype, "CHIRP");
     //yield();
     delay(500);

///------------test sensor access ----------------------

      do {

         test = sensor.isBusy();
         //yield();
         delay(100);
         counter++;
      } while (test !=0 && counter < 16 );
///------------test sensor acces end ----------------------

///------------get data if sensor is avaliable instead old data will be displayed-----------

//-------------------test start----------------------
    if (test == 0)  {
      ver  = sensor.getVersion();
      delay(500);

///------------readtemperature----------------------
      counter=0;
       //while (sensor.isBusy() ) delay(50);
       do {

           readtemperature = sensor.getTemperature();
    //       yield();
           delay(50);

         counter++;

       } while ((readtemperature >850.0 || readtemperature < -400.0 ) && counter < 5);

///------------readtemperature end---------------------

///------------readmoisture----------------------
        counter = 0;

        do {

             readmoisture = sensor.getCapacitance();
            // yield();
             delay(50);

          counter++;

       } while ((readmoisture > CHIRP_CAPACITANCE_MAX + 300 || readmoisture < CHIRP_CAPACITANCE_MIN - 100 )&& counter < 5 );

///------------readmoisture  end-----------------------


///------------light------------------------
      counter = 0;

     if (!sensor.isBusy() ) {
         light = chirp_readLux();

       } else {

         do {

            light = sensor.getLight(false);
        //    yield();
            delay(50);
            counter++;
         } while ((light > CHIRP_LIGHT_CALIB) && counter < 5);

          counter = 0;

      } ///---light  end

///------conversion---------

       light_read = (char *)(map((light > 58000  ? CHIRP_LIGHT_CALIB : light),CHIRP_LIGHT_CALIB,0,0,100));
       readtemperature = ((readtemperature+CHIRP_TEMP_CAL)/10.0);
       readmoisture = (map(readmoisture,CHIRP_CAPACITANCE_MIN,CHIRP_CAPACITANCE_MAX,0,1000)/10.0);
       dtostrfd((readmoisture < 0.0 ? 0.0 : (readmoisture > 99 ? 100.0 : readmoisture)), Settings.flag2.humidity_resolution, moisture);
       dtostrfd((readtemperature > 85.0 ? 85.0 : readtemperature) , Settings.flag2.temperature_resolution-1, temperature);


    } ///---test end
    status = true;
   } ///---sensor is avaliable end
 } ///---sensor exists end
 return status;
}

void chirp_Show(boolean json) {

  if (chirptype == 1) {

    if (json) {

      snprintf_P(mqtt_data, sizeof(mqtt_data), JSON_SNS_LIGHTMOISTTEMP, mqtt_data, chirpstype,light_read, moisture,temperature);

      #ifdef USE_DOMOTICZ
      if (0 == tele_period ){
        DomoticzTempHumSensor(temperature, moisture);
        DomoticzSensor(DZ_ILLUMINANCE,light_read);
      }
      #endif  // USE_DOMOTICZ


      #ifdef USE_WEBSERVER
       } else {


       snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_ILLUMINANCE, mqtt_data, chirpstype, light_read);
       snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_MOISTURE, mqtt_data, chirpstype, moisture);
       snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_TEMP, mqtt_data, chirpstype, temperature, TempUnit());
       snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_VER, mqtt_data, chirpstype, (char)ver);
       light_read[0]= '\0';
       moisture[0]= '\0';
       temperature[0]= '\0';


      #endif // USE_WEBSERVER

     }

  }
}




/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define XSNS_92

boolean Xsns92(byte function)
{
  boolean result = false;

  if (i2c_flg) {
    switch (function) {
      case FUNC_INIT:
       Wire.setClockStretchLimit(2500);
        chirp_detect();
        //yield();
        delay(500);
        break;
      case FUNC_PREP_BEFORE_TELEPERIOD:


        break;
      case FUNC_JSON_APPEND:

      if (!I2cDevice(chirpaddr) ) {
         chirp_detect();
         //yield();
         delay(500);
       }
        chirp_Get();
        //delay(100);
        chirp_Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_APPEND:

        chirp_Show(0);
        break;
#endif // USE_WEBSERVER
    }
  }
  return result;
}

#endif // USE_CHIRP
#endif // USE_I2C
