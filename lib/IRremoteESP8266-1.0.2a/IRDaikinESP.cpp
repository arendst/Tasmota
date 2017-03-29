/*
An Arduino sketch to emulate IR Daikin ARC433** remote control unit
Read more on http://harizanov.com/2012/02/control-daikin-air-conditioner-over-the-internet/
*/

#include <IRDaikinESP.h>

IRDaikinESP::IRDaikinESP(int pin) : _irsend(pin)
{
}

void IRDaikinESP::begin()
{
    _irsend.begin();
}

void IRDaikinESP::send()
{
  _irsend.sendDaikin(daikin);
}

void IRDaikinESP::checksum()
{
    uint8_t sum = 0;
    uint8_t i;

    for(i = 0; i <= 6; i++){
        sum += daikin[i];
    }

    daikin[7] = sum &0xFF;
    sum=0;
    for(i = 8; i <= 25; i++){
        sum += daikin[i];
    }
    daikin[26] = sum &0xFF;
}

void IRDaikinESP::on()
{
    //state = ON;
    daikin[13] |= 0x01;
    checksum();
}

void IRDaikinESP::off()
{
    //state = OFF;
    daikin[13] &= 0xFE;
    checksum();
}

uint8_t IRDaikinESP::getPower()
{
    return (daikin[13])&0x01;
}

// DAIKIN_SILENT or DAIKIN_POWERFUL
void IRDaikinESP::setAux(uint8_t aux)
{
    daikin[21] = aux;
    checksum();
}

uint8_t IRDaikinESP::getAux(){
    return daikin[21];
}


// Set the temp in deg C
void IRDaikinESP::setTemp(uint8_t temp)
{
    if (temp < 18)
        temp = 18;
    else if (temp > 32)
        temp = 32;
    daikin[14] = (temp)*2;
    checksum();
}

uint8_t IRDaikinESP::getTemp()
{
    return (daikin[14])/2;
}

// Set the speed of the fan, 0-5, 0 is auto, 1-5 is the speed
void IRDaikinESP::setFan(uint8_t fan)
{
    // Set the fan speed bits, leave low 4 bits alone
    uint8_t fanset;
    daikin[16] = daikin[16] & 0x0F;
    if (fan >= 1 && fan <= 5)
        fanset = 0x20 + (0x10 * fan);
    else
        fanset = 0xA0;
    daikin[16] = daikin[16] | fanset;
    checksum();
}

uint8_t IRDaikinESP::getFan()
{
    uint8_t fan = daikin[16] >> 4;
    fan = fan - 2;
    if (fan > 5)
        fan = 0;
    return fan;
}

uint8_t IRDaikinESP::getMode()
{/*
  DAIKIN_COOL
  DAIKIN_HEAT
  DAIKIN_FAN
  DAIKIN_AUTO
  DAIKIN_DRY
  */
    return (daikin[13])>>4;
}

void IRDaikinESP::setMode(uint8_t mode)
{
    daikin[13]=mode<<4 | getPower();
    checksum();
}

void IRDaikinESP::setSwingVertical(uint8_t swing)
{
    if (swing)
        daikin[16] = daikin[16] | 0x0F;
    else
        daikin[16] = daikin[16] & 0xF0;
    checksum();
}

uint8_t IRDaikinESP::getSwingVertical()
{
    return (daikin[16])&0x01;
}

void IRDaikinESP::setSwingHorizontal(uint8_t swing)
{
    if (swing)
        daikin[17] = daikin[17] | 0x0F;
    else
        daikin[17] = daikin[17] & 0xF0;
    checksum();
}

uint8_t IRDaikinESP::getSwingHorizontal()
{
    return (daikin[17])&0x01;
}
