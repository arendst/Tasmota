/*
  MQ-sensor measure (set for MQ5)
  Copyright (C) 2018 RL

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
#if !defined USE_ADC_VCC || !defined USE_VOLTAGE

#if defined USE_MQ_SENSOR
/************************************Pre requirements******************************************\
 * sensor supply is 5V the additional voltage devider measurment is needed, and separate supply
 for ESP by i.e. voltage converter 3.3 => 5v
 https://www.google.pl/search?q=3.3V+to+5V+DC-DC+converter&dcr=0&tbm=isch&tbo=u&source=univ&sa=X&ved=0ahUKEwiQ_rW6-q7ZAhUJ3CwKHWGfCxsQsAQIOw&biw=1350&bih=576
 and,
 http://www.ohmslawcalculator.com/voltage-divider-calculator
 For cler ADC imput there is: R1=1mohm R2=250kohm (1% tolerancy) devider that gives 1v max output (ESP ADC max value) when input is 5V
 ESP wemeos D1 - devider has  max 3.3v input A0,
 For module without devider you have to count new values for adding devider to ADC
 D1 wemeos that has 330kohm sumarised value of devider - need to be replaced or added additional resistor for double devider
 For 5V you have to use only one serial imput A0 resistwebserver
 utput of MQ sensor and imput A0
 it gives 3.299 V for A0.
 See https://escapequotes.net/wp-content/uploads/2017/06/esp8266-wemos-d1-mini-pro-schematics.png
 https://electronics.stackexchange.com/questions/197105/multiple-voltage-dividers
Description:
 * MQ_SENSOR - GAS sensor for MQ-5 for others the RS_air need to be set
 http://wiki.seeed.cc/Grove-Gas_Sensor-MQ5/

 Source taken for creation:
 http://sandboxelectronics.com/?p=165
 https://www.mysensors.org/build/gas
 https://github.com/mdsiraj1992/Gassensors/blob/master/arduino%20codes/mq5_sensor/mq5_sensor.ino
*/
/************************MQsensor************************************************************
 choice proper MQ sensor according
 codes for mq2, mq3, mq4, mq5, mq6, mq7, mq8, mq9, mq135
 https://github.com/mdsiraj1992/Gassensors
************************Hardware Related Macros for MQ-5************************************/
#define         MQ5PIN                       (A0) // (pin[GPIO_ADC0])    //define which analog input channel you are going to use
#define         RL_VALUE_MQ5                 (1)     //define the load resistance on the board, in kilo ohms
#define         RO_CLEAN_AIR_FACTOR_MQ5      (6.455) //RO_CLEAR_AIR_FACTOR=(Sensor resistance in clean air)/RO,
                                                     //which is derived from the chart in datasheet
#define         MQ5_SUPPLY                   (4.9)   // value could be betwen 4.9V and 5.1V for voltage divider 1Mohm/250k
                                                     //A0 wemeos - 1V output reference, need to be measured
                                                     //for callibration, directly on sensor side
/**********************Application Related Macros**********************************/
#define         GAS_HYDROGEN                 (0)
#define         GAS_LPG                      (1)
#define         GAS_METHANE                  (2)
#define         GAS_CARBON_MONOXIDE          (3)
#define         GAS_ALCOHOL                  (4)
#define         GAS_SMOG                     (5)
#define         accuracy                     (0)    //for linearcurves
//#define         accuracy                     (1)    //for nonlinearcurves, un comment this
                                                    //line and comment the above line if calculations
                                                    //are to be done using non linear curve equations
/***********************Software Related Macros************************************/
#define         CALIBARAION_SAMPLE_TIMES     (50)  //define how many samples you are going to take in the calibration phase
#define         CALIBRATION_SAMPLE_INTERVAL  (1)   //define the time interal(in milisecond) between each samples in the
                                                     //cablibration phase
#define         READ_SAMPLE_TIMES             (50)   //define how many samples you are going to take in normal operation
#define         READ_SAMPLE_INTERVAL         (1500)  //define the time interal(in microsecond) between each samples in

//normal air R0 ADC read should be 18-20 for 1 kohm RL and Vcc = 4.9 V measured on sensor side
//measure could be calibrate after 48 h sensor heating, below value is taken in 15℃
#define R0 19.65530596 //for ADC 9
// You have to enable ADC in sonoff_template.h to get/see value in clear air and set the calibration RO value
// as R0 = ([RL_VALUE_MQ5 kohm] * (1023- [MQ5PIN read value])/[MQ5PIN read value])))/RO_CLEAN_AIR_FACTOR_MQ5
/*****************************Globals************************************************/
float Ro = R0; //Ro is initialized, when calibration of sensor is done.
//Restart module in clean air the calibration will be set automaticaly in module init phase.

char gastype[11];
int ppm=0;
byte counter_measure=0;
// nask for printf
const char HTTP_GAS[] PROGMEM = "%s{s}" D_GAS " %s{m} %d" D_UNIT_PARTS_PER_MILLION "{e}";
const char JSON_SNS_GAS[] PROGMEM = "%s,\"" D_GAS "\":{\"Type\":\"%s\",\"ppm\":%d}";


/****************** MQResistanceCalculation ****************************************
Input:   raw_adc - raw value read from adc, which represents the voltage
Output:  the calculated sensor resistance
Remarks: The sensor and the load resistor forms a voltage divider. Given the voltage
         across the load resistor and its resistance, the resistance of the sensor
         could be derived.
************************************************************************************/
float MQResistanceCalculation(int raw_adc)
{


  return ( ((float)RL_VALUE_MQ5*(1023-raw_adc)/raw_adc));
}

/***************************** MQCalibration ****************************************
Input:   mq_pin - analog channel
Output:  Ro of the sensor
Remarks: This function assumes that the sensor is in clean air. It use
         MQResistanceCalculation to calculates the sensor resistance in clean air
         and then divides it with RO_CLEAN_AIR_FACTOR. RO_CLEAN_AIR_FACTOR is about
         10, which differs slightly between different sensors.
************************************************************************************/

float MQCalibration(int mq_pin)
{
  int i;
  float RS_AIR_val=0,r0;



  for (i=0;i<CALIBARAION_SAMPLE_TIMES;i++) {                     //take multiple samples
    RS_AIR_val += MQResistanceCalculation(analogRead(mq_pin));
    delay(CALIBRATION_SAMPLE_INTERVAL);
  }
  RS_AIR_val = RS_AIR_val/CALIBARAION_SAMPLE_TIMES;              //calculate the average value

  r0 = RS_AIR_val/RO_CLEAN_AIR_FACTOR_MQ5;                      //RS_AIR_val divided by RO_CLEAN_AIR_FACTOR yields the Ro
                                                                 //according to the chart in the datasheet
  #define CALIBRATIONDONE
  return r0;
}

/*****************************  MQRead *********************************************
Input:   mq_pin - analog channel
Output:  Rs of the sensor
Remarks: This function use MQResistanceCalculation to caculate the sensor resistenc (Rs).
         The Rs changes as the sensor is in the different consentration of the target
         gas. The sample times and the time interval between samples could be configured
         by changing the definition of the macros.
************************************************************************************/
float MQRead(int mq_pin)
{
  int i;
  float rs=0;

  for (i=0;i<READ_SAMPLE_TIMES;i++) {
    rs += MQResistanceCalculation(analogRead(mq_pin));
    delayMicroseconds(READ_SAMPLE_INTERVAL);

  }

  rs = rs/READ_SAMPLE_TIMES;

  return rs;
}


uint16_t readvalue() {

  uint16_t inputadc = 0;
  for (byte i = 0; i < 32; i++) {
    inputadc += analogRead(MQ5PIN);
    delayMicroseconds(500);

  }
  inputadc >>= 5;
  return inputadc;

}


int getppm (float ratio){

  //float R0;  //  Get the value of RS via in a clear air
  // the minimum concentration we can test is 200ppm and max is set till 10000ppm

   int getvalue = 0;
     //dym zaczyna sie od 600 ppm
     if  (ratio >= 5.4 ) {

      getvalue = 0;
      strcpy(gastype,"CLEANAIR");
      return getvalue;
      //return 0 ppm;
    } else if //SMOG

    (ratio < 5.4 && ratio >= 3.8 ) {

      getvalue = MQGetGasPercentage(MQRead(MQ5PIN)/Ro,GAS_SMOG);
      getvalue = constrain (getvalue,200,10000);
      strcpy(gastype,"SMOG");
      return getvalue;
      //return ppm;
    } else if  //CO

    (ratio < 3.8 &&  ratio >= 2.35) {

     getvalue = MQGetGasPercentage(MQRead(MQ5PIN)/Ro,GAS_CARBON_MONOXIDE);

     getvalue = constrain (getvalue,200,10000);
     strcpy(gastype,"CO");
     return getvalue;
    //return ppm;
  } else if  // Alcochol starts from 2000 ppm

   (ratio  < 2.35 &&  ratio >= 1.65) {

    getvalue = MQGetGasPercentage(MQRead(MQ5PIN)/Ro,GAS_ALCOHOL);

    getvalue = constrain (getvalue,200,10000);
    strcpy(gastype,"ALC");
    return getvalue;
    //return ppm;
  } else if  // H2 starts ends 2000 ppm

   (ratio  < 1.65 &&  ratio >= 0.99) {

    getvalue = MQGetGasPercentage(MQRead(MQ5PIN)/Ro,GAS_HYDROGEN);

    getvalue = constrain (getvalue,200,10000);
    strcpy(gastype,"H2");
    return getvalue;
    //return ppm;
  } else if  // CH4 and LPG, above 4000 ppm could be LPG in CH4 is 10000ppm, both concetrations are dengerous

     (ratio  < 0.99 &&  ratio >= 0.14) {

      getvalue = MQGetGasPercentage(MQRead(MQ5PIN)/Ro,GAS_METHANE);

      if (ratio  < 0.22 && ratio >= 0.14)  {
        getvalue = MQGetGasPercentage(MQRead(MQ5PIN)/Ro,GAS_LPG);

        strcpy(gastype,"LPG");
      } else {
        strcpy(gastype,"CH4");
      }
      getvalue = constrain (getvalue,200,10000);
      return getvalue;
      //return ppm;
   } else if (ratio  < 0.14){
     //explosion more than 1% = 10000ppm of CH4 or LPG  in Air - self-acting explosion
     //return 99999;
     strcpy(gastype,"EXPLOSION");
     return 99999;
     //return ppm;
  } else {

    //no sensor
    strcpy(gastype,"ERROR");
    return -1;

  }
}


int MQGetGasPercentage(float rs_ro_ratio, int gas_id)
{
#if accuracy == (0)
  if ( accuracy == 0 ) {
  if ( gas_id == GAS_HYDROGEN ) {
    return (pow(10,((-3.986*(log10(rs_ro_ratio))) + 3.075)));
  } else if ( gas_id == GAS_LPG ) {
    return (pow(10,((-2.513*(log10(rs_ro_ratio))) + 1.878)));
  } else if ( gas_id == GAS_METHANE ) {
    return (pow(10,((-2.554*(log10(rs_ro_ratio))) + 2.265 )));
  } else if ( gas_id == GAS_CARBON_MONOXIDE ) {
    return (pow(10,((-6.900*(log10(rs_ro_ratio))) + 6.241)));
  } else if ( gas_id == GAS_ALCOHOL ) {
    return (pow(10,((-4.590*(log10(rs_ro_ratio))) + 4.851)));
  } else if ( gas_id == GAS_SMOG ) {
   return (pow(10,((log10(rs_ro_ratio)-0.971863872)/-0.1)));
  }
}
#endif
#if accuracy == (1)
  else if ( accuracy == 1 ) {
    if ( gas_id == GAS_HYDROGEN ) {
    return (pow(10,(-22.89*pow((log10(rs_ro_ratio)), 3) + 8.873*pow((log10(rs_ro_ratio)), 2) - 3.587*(log10(rs_ro_ratio)) + 2.948)));
  } else if ( gas_id == GAS_LPG ) {
    return (pow(10,((-2.513*(log10(rs_ro_ratio))) + 1.878)));
  } else if ( gas_id == GAS_METHANE ) {
    return (pow(10,(-0.428*pow((log10(rs_ro_ratio)), 2) - 2.867*(log10(rs_ro_ratio)) + 2.224)));
  } else if ( gas_id == GAS_CARBON_MONOXIDE ) {
    return (pow(10,(1401*pow((log10(rs_ro_ratio)), 4) - 2777*pow((log10(rs_ro_ratio)), 3) + 2059*pow((log10(rs_ro_ratio)), 2) - 682.5*(log10(rs_ro_ratio)) + 88.81)));
  } else if ( gas_id == GAS_ALCOHOL ) {
    return (pow(10,(14.90*pow((log10(rs_ro_ratio)), 3) - 19.26*pow((log10(rs_ro_ratio)), 2) + 3.108*(log10(rs_ro_ratio)) + 3.922)));
  } else if ( gas_id == GAS_SMOG ) {
   return (pow(10,((log10(rs_ro_ratio)-0.971863872)/-0.1)));
  }
}
#endif
  return 0;
}



void measure(boolean json)
{
counter_measure++;
if (counter_measure == 20) {
 ppm=getppm(MQRead(MQ5PIN)/Ro);
 counter_measure=0;
}
  if (json) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), JSON_SNS_GAS, mqtt_data, gastype, ppm);

  #ifdef USE_DOMOTICZ
          if (0 == tele_period) {
            DomoticzSensor(DZ_POWER_ENERGY,gastype);
            DomoticzSensor(DZ_AIRQUALITY,ppm );

            }
  #endif  // USE_DOMOTICZ

  #ifdef USE_WEBSERVER
    } else {
      snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_GAS, mqtt_data,gastype, ppm);

  #endif  // USE_WEBSERVER
    }
}


/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define XSNS_94

boolean Xsns94(byte function)
{
  boolean result = false;


    switch (function) {
      case FUNC_INIT:
        //ratio_check();
        #ifdef CALIBRATION
        break;
        #else
        Ro = MQCalibration(MQ5PIN);
        #endif
        break;
      case FUNC_PREP_BEFORE_TELEPERIOD:
        //measure(0);

        break;
      case FUNC_JSON_APPEND:

        measure(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_APPEND:

        measure(0);
        break;
#endif // USE_WEBSERVER
    }

  return result;
}


#endif // USE_MQ_SENSOR
#endif //end #ifndef USE_ADC_VCC
