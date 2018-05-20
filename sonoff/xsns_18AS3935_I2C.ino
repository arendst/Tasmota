/*
  xsns_18AS3935_I2C.ino - AS3935 Lightning Sensor

  Copyright (C) 2018  Palich2000

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

#ifdef USE_AS3935
/*********************************************************************************************\
 * AS3935 Lightning Sensor  http://www.playingwithfusion.com/productview.php?pdid=22&catid=1012
 *
 *
 * library https://github.com/PlayingWithFusion/PWFusion_AS3935_I2C
\*********************************************************************************************/

#include "SPI.h"
#include "PWFusion_AS3935.h"


bool search_and_init_AS3935() {
    return(true);
}

bool AS3935Init() {
    return(true);
}

//int8_t CS_PIN  = D8;
//int8_t SI_PIN  = 0;
//int8_t IRQ_PIN = D4;                       // digital pins 2 and 3 are available for interrupt capability
int8_t CS_PIN  = D4;
int8_t SI_PIN  = 0;
int8_t IRQ_PIN = D8;                       // digital pins 2 and 3 are available for interrupt capability
volatile int8_t AS3935_ISR_Trig = 0;

// #defines
#define AS3935_INDOORS       0
#define AS3935_OUTDOORS      1
#define AS3935_DIST_DIS      0
#define AS3935_DIST_EN       1
#define AS3935_CAPACITANCE   40           // <-- SET THIS VALUE TO THE NUMBER LISTED ON YOUR BOARD.

class PWF_AS3935_MY: public PWF_AS3935 {
public:
    using PWF_AS3935::PWF_AS3935;
    virtual void Debug(const char * format, ...) {
        va_list args;
        va_start(args, format);
        vsnprintf_P(log_data, sizeof(log_data), format, args);
        AddLog(LOG_LEVEL_ERROR);
        va_end(args);
    }
};

#define SYSLOG(format, args...) \
{ \
    snprintf_P(log_data, sizeof(log_data), PSTR(format), ##args); \
    AddLog(LOG_LEVEL_ERROR); \
}

PWF_AS3935_MY  lightning0(CS_PIN, IRQ_PIN, SI_PIN);

void AS3935_ISR() {
    AS3935_ISR_Trig++;
}


boolean AS3935Prep() {
    static int flag = 0;
    if (!flag) {
        flag = 1;
        SPI.begin();                            // begin SPI
        SPI.setClockDivider(SPI_CLOCK_DIV16);   // SPI speed to SPI_CLOCK_DIV16/1MHz (max 2MHz, NEVER 500kHz!)
        SPI.setDataMode(SPI_MODE1);             // MAX31855 is a Mode 1 device
        //    --> clock starts low, read on rising edge
        SPI.setBitOrder(MSBFIRST);              // data sent to chip MSb first.

        lightning0.AS3935_DefInit();                        // set registers to default..
        // now update sensor cal for your application and power up chip
        lightning0.AS3935_ManualCal(AS3935_CAPACITANCE, AS3935_INDOORS, AS3935_DIST_EN);
        // AS3935_ManualCal Parameters:
        //   --> capacitance, in pF (marked on package)
        //   --> indoors/outdoors (AS3935_INDOORS:0 / AS3935_OUTDOORS:1)
        //   --> disturbers (AS3935_DIST_EN:1 / AS3935_DIST_DIS:2)
        // function also powers up the chip

        // enable interrupt (hook IRQ pin to Arduino Uno/Mega interrupt input: 0 -> pin 2, 1 -> pin 3 )
        attachInterrupt(IRQ_PIN, AS3935_ISR, RISING);
        lightning0.AS3935_PrintAllRegs();
    }
    return(true);
}

bool AS3935Read(float * visible, float * IR, float * UV) {
    return(false);
}

void AS3935ShowJSON() {
}


#ifdef MQTT_USE
void AS3935ShowMQTT() {
}
#endif //  MQTT_USE


#ifdef USE_WEBSERVER
void AS3935ShowWEB() {
    if (AS3935_ISR_Trig) {
        SYSLOG(" interrupt counter = %d", AS3935_ISR_Trig);
        uint8_t int_src = lightning0.AS3935_GetInterruptSrc();
        if(0 == int_src) {
            SYSLOG(" interrupt source result not expected");
        } else if(1 == int_src) {
            uint8_t lightning_dist_km = lightning0.AS3935_GetLightningDistKm();
            SYSLOG(" Lightning detected! Distance to strike: %d kilometers", lightning_dist_km);
        } else if(2 == int_src) {
            SYSLOG(" Disturber detected");
        } else if(3 == int_src) {
            SYSLOG(" Noise level too high");
        }
        lightning0.AS3935_PrintAllRegs();
        AS3935_ISR_Trig = 0;
    }
}
#endif  // USE_WEBSERVER



/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define XSNS_18

boolean Xsns18(byte function) {
    boolean result = false;
    switch (function) {
    case FUNC_XSNS_INIT:
        result = AS3935Init();
        break;
    case FUNC_XSNS_PREP:
        result = AS3935Prep();
        break;
    case FUNC_XSNS_JSON_APPEND:
        AS3935ShowJSON();
        break;
#ifdef MQTT_USE
    case FUNC_XSNS_MQTT_SHOW:
        AS3935ShowMQTT();
        break;
#endif //MQTT_USE
#ifdef USE_WEBSERVER
    case FUNC_XSNS_WEB:
        AS3935ShowWEB();
        break;
#endif  // USE_WEBSERVER
    }
    return result;
}

#endif  // USE_AS3935
