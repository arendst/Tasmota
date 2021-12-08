/*
    MutichannelGasSensor.cpp
    2015 Copyright (c) Seeed Technology Inc.  All right reserved.

    Author: Jacky Zhang
    2015-3-17
    http://www.seeed.cc/
    modi by Jack, 2015-8

    The MIT License (MIT)

    Copyright (c) 2015 Seeed Technology Inc.

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/

#include <math.h>
#include <Wire.h>
#include <Arduino.h>
#include "MutichannelGasSensor.h"

/*********************************************************************************************************
** Function name:           begin
** Descriptions:            initialize I2C
*********************************************************************************************************/
void MutichannelGasSensor::begin(int address)
{
    __version = 1;          // version 1/2
    r0_inited = false;

    Wire.begin();
    i2cAddress = address;
    __send_error = false;
    __version = getVersion();
}

bool MutichannelGasSensor::isError()
{
    return __send_error;
}

unsigned char MutichannelGasSensor::getVersion()
{
    if(get_addr_dta(CMD_READ_EEPROM, ADDR_IS_SET) == 1126)        // get version
    {
        __version = 2;
        return __version;
    }
    if (!__send_error) {
	__version = 1;
    }
    else {
	__version = -1;
    }
    return __version;
}

void MutichannelGasSensor::begin()
{
    begin(DEFAULT_I2C_ADDR);
}

/*********************************************************************************************************
** Function name:           sendI2C
** Descriptions:            send one byte to I2C Wire
*********************************************************************************************************/
void MutichannelGasSensor::sendI2C(unsigned char dta)
{
    __send_error = false;
    Wire.beginTransmission(i2cAddress);               // transmit to device #4
    Wire.write(dta);                                  // sends one byte
    if (Wire.endTransmission()) {                     // stop transmitting
	__send_error = true;
    }
}


unsigned int MutichannelGasSensor::get_addr_dta(unsigned char addr_reg)
{
    int trys = 0;
START:
    __send_error = false;
    Wire.beginTransmission(i2cAddress);
    Wire.write(addr_reg);
    if (Wire.endTransmission()) {    // stop transmitting
	trys++;
	__send_error = true;
	if (trys > 3) {
	    return 0;
	}
    }

    Wire.requestFrom(i2cAddress, (uint8_t)2);

    unsigned int dta = 0;

    unsigned char raw[10];
    int cnt = 0;

    while(Wire.available())
    {
        raw[cnt++] = Wire.read();
    }

    if(cnt == 0)goto START;

    dta = raw[0];
    dta <<= 8;
    dta += raw[1];

    switch(addr_reg)
    {
        case CH_VALUE_NH3:

        if(dta > 0)
        {
            adcValueR0_NH3_Buf = dta;
        }
        else
        {
            dta = adcValueR0_NH3_Buf;
        }

        break;

        case CH_VALUE_CO:

        if(dta > 0)
        {
            adcValueR0_CO_Buf = dta;
        }
        else
        {
            dta = adcValueR0_CO_Buf;
        }

        break;

        case CH_VALUE_NO2:

        if(dta > 0)
        {
            adcValueR0_NO2_Buf = dta;
        }
        else
        {
            dta = adcValueR0_NO2_Buf;
        }

        break;

        default:;
    }
    return dta;
}

unsigned int MutichannelGasSensor::get_addr_dta(unsigned char addr_reg, unsigned char __dta)
{
    int trys = 0;
START:
    __send_error = false;
    Wire.beginTransmission(i2cAddress);
    Wire.write(addr_reg);
    Wire.write(__dta);
    if (Wire.endTransmission()) {    // stop transmitting
	trys++;
	__send_error = true;
	if (trys > 3) {
	    return 0;
	}
    }

    Wire.requestFrom(i2cAddress, (uint8_t)2);

    unsigned int dta = 0;

    unsigned char raw[10];
    int cnt = 0;

    while(Wire.available())
    {
        raw[cnt++] = Wire.read();
    }

    if(cnt == 0)goto START;

    dta = raw[0];
    dta <<= 8;
    dta += raw[1];


    return dta;
}


void MutichannelGasSensor::write_i2c(unsigned char addr, unsigned char *dta, unsigned char dta_len)
{
    __send_error = false;
    Wire.beginTransmission(addr);
    for(int i=0; i<dta_len; i++)
    {
        Wire.write(dta[i]);
    }
    if (Wire.endTransmission()) {
	__send_error = true;
    }
}


/*********************************************************************************************************
** Function name:           readData
** Descriptions:            read 4 bytes from I2C slave
*********************************************************************************************************/
int16_t MutichannelGasSensor::readData(uint8_t cmd)
{
    uint16_t timeout = 0;
    uint8_t buffer[4];
    uint8_t checksum = 0;
    int16_t rtnData = 0;

    //send command
    sendI2C(cmd);
    //wait for a while
    delay(2);
    //get response
    Wire.requestFrom(i2cAddress, (uint8_t)4);    // request 4 bytes from slave device
    if(Wire.available() != 4)
        return -3;//rtnData length wrong
    buffer[0] = Wire.read();
    buffer[1] = Wire.read();
    buffer[2] = Wire.read();
    buffer[3] = Wire.read();
    checksum = (uint8_t)(buffer[0] + buffer[1] + buffer[2]);
    if(checksum != buffer[3])
        return -4;//checksum wrong
    rtnData = ((buffer[1] << 8) + buffer[2]);

    return rtnData;//successful
}

/*********************************************************************************************************
** Function name:           readR0
** Descriptions:            read R0 stored in slave MCU
*********************************************************************************************************/
int16_t MutichannelGasSensor::readR0(void)
{
    int16_t rtnData = 0;

    rtnData = readData(0x11);

    if(rtnData > 0)
        res0[0] = rtnData;
    else
        return rtnData;         //unsuccessful

    rtnData = readData(0x12);
    if(rtnData > 0)
        res0[1] = rtnData;
    else
        return rtnData;         //unsuccessful

    rtnData = readData(0x13);
    if(rtnData > 0)
        res0[2] = rtnData;
    else
        return rtnData;         //unsuccessful

    return 1;//successful
}

/*********************************************************************************************************
** Function name:           readR
** Descriptions:            read resistance value of each channel from slave MCU
*********************************************************************************************************/
int16_t MutichannelGasSensor::readR(void)
{
    int16_t rtnData = 0;

    rtnData = readData(0x01);
    if(rtnData >= 0)
        res[0] = rtnData;
    else
        return rtnData;//unsuccessful

    rtnData = readData(0x02);
    if(rtnData >= 0)
        res[1] = rtnData;
    else
        return rtnData;//unsuccessful

    rtnData = readData(0x03);
    if(rtnData >= 0)
        res[2] = rtnData;
    else
        return rtnData;//unsuccessful

    return 0;//successful
}

/*********************************************************************************************************
** Function name:           readR
** Descriptions:            calculate gas concentration of each channel from slave MCU
** Parameters:
                            gas - gas type
** Returns:
                            float value - concentration of the gas
*********************************************************************************************************/
float MutichannelGasSensor::calcGas(int gas)
{

    float ratio0, ratio1, ratio2;
    if(1 == __version)
    {
        if(!r0_inited)
        {
            if(readR0() >= 0) r0_inited = true;
            else return -1.0f;
        }

        if(readR() < 0)
            return -2.0f;

        ratio0 = (float)res[0] / res0[0];
        ratio1 = (float)res[1] / res0[1];
        ratio2 = (float)res[2] / res0[2];
    }
    else if(2 == __version)
    {
        // how to calc ratio/123
        ledOn();
        int A0_0 = get_addr_dta(6, ADDR_USER_ADC_HN3);
        int A0_1 = get_addr_dta(6, ADDR_USER_ADC_CO);
        int A0_2 = get_addr_dta(6, ADDR_USER_ADC_NO2);

        int An_0 = get_addr_dta(CH_VALUE_NH3);
        int An_1 = get_addr_dta(CH_VALUE_CO);
        int An_2 = get_addr_dta(CH_VALUE_NO2);

        ratio0 = (float)An_0/(float)A0_0*(1023.0f-A0_0)/(1023.0f-An_0);
        ratio1 = (float)An_1/(float)A0_1*(1023.0f-A0_1)/(1023.0f-An_1);
        ratio2 = (float)An_2/(float)A0_2*(1023.0f-A0_2)/(1023.0f-An_2);

    }

    float c = 0;

    switch(gas)
    {
        case CO:
        {
            c = pow(ratio1, -1.179f)*4.385f;  //mod by jack
            break;
        }
        case NO2:
        {
            c = pow(ratio2, 1.007f)/6.855f;  //mod by jack
            break;
        }
        case NH3:
        {
            c = pow(ratio0, -1.67f)/1.47f;  //modi by jack
            break;
        }
        case C3H8:  //add by jack
        {
            c = pow(ratio0, -2.518f)*570.164f;
            break;
        }
        case C4H10:  //add by jack
        {
            c = pow(ratio0, -2.138f)*398.107f;
            break;
        }
        case GAS_CH4:  //add by jack
        {
            c = pow(ratio1, -4.363f)*630.957f;
            break;
        }
        case H2:  //add by jack
        {
            c = pow(ratio1, -1.8f)*0.73f;
            break;
        }
        case C2H5OH:  //add by jack
        {
            c = pow(ratio1, -1.552f)*1.622f;
            break;
        }
        default:
            break;
    }

    if(2==__version)ledOff();
    return isnan(c)?-3:c;
}

/*********************************************************************************************************
** Function name:           changeI2cAddr
** Descriptions:            change I2C address of the slave MCU, and this address will be stored in EEPROM of slave MCU
*********************************************************************************************************/
void MutichannelGasSensor::changeI2cAddr(uint8_t newAddr)
{
    Wire.beginTransmission(i2cAddress); // transmit to device
    Wire.write(0x23);              // sends one byte
    Wire.write(newAddr);              // sends one byte
    if (Wire.endTransmission()){    // stop transmitting
	__send_error = true;
    } else {
	i2cAddress = newAddr;
    }
}

/*********************************************************************************************************
** Function name:           doCalibrate
** Descriptions:            tell slave to do a calibration, it will take about 8s
                            after the calibration, must reread the R0 values
*********************************************************************************************************/
void MutichannelGasSensor::doCalibrate(void)
{

    if(1 == __version)
    {
    START:

        sendI2C(0x22);
        if(readR0() > 0)
        {
            for(int i=0; i<3; i++)
            {
                Serial.print(res0[i]);
                Serial.print('\t');
            }
        }
        else
        {
            delay(5000);
            Serial.println("continue...");
            for(int i=0; i<3; i++)
            {
                Serial.print(res0[i]);
                Serial.print('\t');
            }
            Serial.println();
            goto START;
        }
    }
    else if(2 == __version)
    {
        unsigned int i, a0, a1, a2;
        while(1)
        {
            a0 = get_addr_dta(CH_VALUE_NH3);
            a1 = get_addr_dta(CH_VALUE_CO);
            a2 = get_addr_dta(CH_VALUE_NO2);

            Serial.print(a0);
            Serial.print('\t');
            Serial.print(a1);
            Serial.print('\t');
            Serial.print(a2);
            Serial.println('\t');
            ledOn();

            int cnt = 0;
            for(i=0; i<20; i++)
            {
                if((a0 - get_addr_dta(CH_VALUE_NH3)) > 2 || (get_addr_dta(CH_VALUE_NH3) - a0) > 2)cnt++;
                if((a1 - get_addr_dta(CH_VALUE_CO)) > 2 || (get_addr_dta(CH_VALUE_CO) - a1) > 2)cnt++;
                if((a2 - get_addr_dta(CH_VALUE_NO2)) > 2 || (get_addr_dta(CH_VALUE_NO2) - a2) > 2)cnt++;

                if(cnt>5)
                {
                    break;
                }
                delay(1000);
            }

            ledOff();
            if(cnt <= 5)break;
            delay(200);
        }

        Serial.print("write user adc value: ");
        Serial.print(a0);Serial.print('\t');
        Serial.print(a1);Serial.print('\t');
        Serial.print(a2);Serial.println('\t');

        unsigned char tmp[7];

        tmp[0] = 7;

        tmp[1] = a0>>8;
        tmp[2] = a0&0xff;

        tmp[3] = a1>>8;
        tmp[4] = a1&0xff;

        tmp[5] = a2>>8;
        tmp[6] = a2&0xff;

        write_i2c(i2cAddress, tmp, 7);
    }
}

/*********************************************************************************************************
** Function name:           powerOn
** Descriptions:            power on sensor heater
*********************************************************************************************************/
void MutichannelGasSensor::powerOn(void)
{
    if(__version == 1)
    sendI2C(0x21);
    else if(__version == 2)
    {
        dta_test[0] = 11;
        dta_test[1] = 1;
        write_i2c(i2cAddress, dta_test, 2);
    }
}

/*********************************************************************************************************
** Function name:           powerOff
** Descriptions:            power off sensor heater
*********************************************************************************************************/
void MutichannelGasSensor::powerOff(void)
{
    if(__version == 1)
    sendI2C(0x20);
    else if(__version == 2)
    {
        dta_test[0] = 11;
        dta_test[1] = 0;
        write_i2c(i2cAddress, dta_test, 2);
    }
}

void MutichannelGasSensor::display_eeprom()
{
    if(__version == 1)
    {
        Serial.println("ERROR: display_eeprom() is NOT support by V1 firmware.");
        return ;
    }

    Serial.print("ADDR_IS_SET = "); Serial.println(get_addr_dta(CMD_READ_EEPROM, ADDR_IS_SET));
    Serial.print("ADDR_FACTORY_ADC_NH3 = "); Serial.println(get_addr_dta(CMD_READ_EEPROM, ADDR_FACTORY_ADC_NH3));
    Serial.print("ADDR_FACTORY_ADC_CO = "); Serial.println(get_addr_dta(CMD_READ_EEPROM, ADDR_FACTORY_ADC_CO));
    Serial.print("ADDR_FACTORY_ADC_NO2 = "); Serial.println(get_addr_dta(CMD_READ_EEPROM, ADDR_FACTORY_ADC_NO2));
    Serial.print("ADDR_USER_ADC_HN3 = "); Serial.println(get_addr_dta(CMD_READ_EEPROM, ADDR_USER_ADC_HN3));
    Serial.print("ADDR_USER_ADC_CO = "); Serial.println(get_addr_dta(CMD_READ_EEPROM, ADDR_USER_ADC_CO));
    Serial.print("ADDR_USER_ADC_NO2 = "); Serial.println(get_addr_dta(CMD_READ_EEPROM, ADDR_USER_ADC_NO2));
    Serial.print("ADDR_I2C_ADDRESS = "); Serial.println(get_addr_dta(CMD_READ_EEPROM, ADDR_I2C_ADDRESS));
}

float MutichannelGasSensor::getR0(unsigned char ch)         // 0:CH3, 1:CO, 2:NO2
{
    if(__version == 1)
    {
        Serial.println("ERROR: getR0() is NOT support by V1 firmware.");
        return -1;
    }

    int a = 0;
    switch(ch)
    {
        case 0:         // CH3
        a = get_addr_dta(CMD_READ_EEPROM, ADDR_USER_ADC_HN3);
        Serial.print("a_ch3 = ");
        Serial.println(a);
        break;

        case 1:         // CO
        a = get_addr_dta(CMD_READ_EEPROM, ADDR_USER_ADC_CO);
        Serial.print("a_co = ");
        Serial.println(a);
        break;

        case 2:         // NO2
        a = get_addr_dta(CMD_READ_EEPROM, ADDR_USER_ADC_NO2);
        Serial.print("a_no2 = ");
        Serial.println(a);
        break;

        default:;
    }

    float r = 56.0f*(float)a/(1023.0f-(float)a);
    return r;
}

float MutichannelGasSensor::getRs(unsigned char ch)         // 0:CH3, 1:CO, 2:NO2
{

    if(__version == 1)
    {
        Serial.println("ERROR: getRs() is NOT support by V1 firmware.");
        return -1;
    }

    int a = 0;
    switch(ch)
    {
        case 0:         // NH3
        a = get_addr_dta(1);
        break;

        case 1:         // CO
        a = get_addr_dta(2);
        break;

        case 2:         // NO2
        a = get_addr_dta(3);
        break;

        default:;
    }

    float r = 56.0f*(float)a/(1023.0f-(float)a);
    return r;
}

// 1. change i2c address to 0x04
// 2. change adc value of R0 to default
void MutichannelGasSensor::factory_setting()
{

    unsigned char tmp[7];

    unsigned char error;
    unsigned char address = 0;

    for(address = 1; address < 127; address++ )
    {
        // The i2c_scanner uses the return value of
        // the Write.endTransmisstion to see if
        // a device did acknowledge to the address.
        Wire.beginTransmission(address);
        error = Wire.endTransmission();
        if (error == 0)
        {
            // change i2c to 0x04

            Serial.print("I2C address is: 0x");
            Serial.println(address, HEX);
            Serial.println("Change I2C address to 0x04");

            dta_test[0] = CMD_CHANGE_I2C;
            dta_test[1] = 0x04;
            write_i2c(address, dta_test, 2);

            i2cAddress = 0x04;
            delay(100);
            getVersion();
            break;
        }
    }

    unsigned int a0 = get_addr_dta(CMD_READ_EEPROM, ADDR_FACTORY_ADC_NH3);
    unsigned int a1 = get_addr_dta(CMD_READ_EEPROM, ADDR_FACTORY_ADC_CO);
    unsigned int a2 = get_addr_dta(CMD_READ_EEPROM, ADDR_FACTORY_ADC_NO2);

    tmp[0] = 7;
    tmp[1] = a0>>8;
    tmp[2] = a0&0xff;
    tmp[3] = a1>>8;
    tmp[4] = a1&0xff;

    tmp[5] = a2>>8;
    tmp[6] = a2&0xff;
    delay(100);
    write_i2c(i2cAddress, tmp, 7);
    delay(100);
}

void MutichannelGasSensor::change_i2c_address(unsigned char addr)
{
    dta_test[0] = CMD_CHANGE_I2C;
    dta_test[1] = addr;
    write_i2c(i2cAddress, dta_test, 2);


    Serial.print("FUNCTION: CHANGE I2C ADDRESS: 0X");
    Serial.print(i2cAddress, HEX);
    Serial.print(" > 0x");
    Serial.println(addr, HEX);

    i2cAddress = addr;
}

void MutichannelGasSensor::ledOn()
{
    dta_test[0] = CMD_CONTROL_LED;
    dta_test[1] = 1;
    write_i2c(i2cAddress, dta_test, 2);
}

void MutichannelGasSensor::ledOff()
{
    dta_test[0] = CMD_CONTROL_LED;
    dta_test[1] = 0;
    write_i2c(i2cAddress, dta_test, 2);
}

MutichannelGasSensor gas;
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/