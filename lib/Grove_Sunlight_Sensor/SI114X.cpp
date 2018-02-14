/*
 * SI114X.cpp
 * A library for Grove - Sunlight Sensor v1.0
 *
 * Copyright (c) 2015 seeed technology inc.
 * Website    : www.seeed.cc
 * Author     : Fuhua.Chen
 * Modified Time: June 2015
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "SI114X.h"
#include "Wire.h"
/*--------------------------------------------------------//
default init

 */
void SI114X::DeInit(void)
{  
  //ENABLE UV reading  
  //these reg must be set to the fixed value
  WriteByte(SI114X_UCOEFF0, 0x29);
  WriteByte(SI114X_UCOEFF1, 0x89);
  WriteByte(SI114X_UCOEFF2, 0x02);
  WriteByte(SI114X_UCOEFF3, 0x00);
  WriteParamData(SI114X_CHLIST, SI114X_CHLIST_ENUV |SI114X_CHLIST_ENALSIR | SI114X_CHLIST_ENALSVIS |SI114X_CHLIST_ENPS1);
  //
  //set LED1 CURRENT(22.4mA)(It is a normal value for many LED)
  //
  WriteParamData(SI114X_PS1_ADCMUX, SI114X_ADCMUX_LARGE_IR);
  WriteByte(SI114X_PS_LED21, SI114X_LED_CURRENT_22MA);
  WriteParamData(SI114X_PSLED12_SELECT, SI114X_PSLED12_SELECT_PS1_LED1); //
  //
  //PS ADC SETTING
  //
  WriteParamData(SI114X_PS_ADC_GAIN, SI114X_ADC_GAIN_DIV1);
  WriteParamData(SI114X_PS_ADC_COUNTER, SI114X_ADC_COUNTER_511ADCCLK);
  WriteParamData(SI114X_PS_ADC_MISC, SI114X_ADC_MISC_HIGHRANGE|SI114X_ADC_MISC_ADC_RAWADC); 
  //
  //VIS ADC SETTING
  //
  WriteParamData(SI114X_ALS_VIS_ADC_GAIN, SI114X_ADC_GAIN_DIV1);
  WriteParamData(SI114X_ALS_VIS_ADC_COUNTER, SI114X_ADC_COUNTER_511ADCCLK);
  WriteParamData(SI114X_ALS_VIS_ADC_MISC, SI114X_ADC_MISC_HIGHRANGE);
  //
  //IR ADC SETTING
  //
  WriteParamData(SI114X_ALS_IR_ADC_GAIN, SI114X_ADC_GAIN_DIV1);
  WriteParamData(SI114X_ALS_IR_ADC_COUNTER, SI114X_ADC_COUNTER_511ADCCLK);
  WriteParamData(SI114X_ALS_IR_ADC_MISC, SI114X_ADC_MISC_HIGHRANGE);
  //
  //interrupt enable
  //
  WriteByte(SI114X_INT_CFG, SI114X_INT_CFG_INTOE);  
  WriteByte(SI114X_IRQ_ENABLE, SI114X_IRQEN_ALS);  
  //
  //AUTO RUN
  //
  WriteByte(SI114X_MEAS_RATE0, 0xFF);
  WriteByte(SI114X_COMMAND, SI114X_PSALS_AUTO);
}

/*--------------------------------------------------------//
 Init the si114x and begin to collect data  


 */
bool SI114X::Begin(void)
{
  Wire.begin();
  //
  //Init IIC  and reset si1145
  //
  if(ReadByte(SI114X_PART_ID)!=0X45)
  {
    return false;
  }
  Reset();
  //
  //INIT 
  //
  DeInit();
  return true;
}
/*--------------------------------------------------------//
reset the si114x 
inclue IRQ reg, command regs...

 */
void SI114X::Reset(void)
{
  WriteByte(SI114X_MEAS_RATE0, 0);
  WriteByte(SI114X_MEAS_RATE1, 0);
  WriteByte(SI114X_IRQ_ENABLE, 0);
  WriteByte(SI114X_IRQ_MODE1, 0);
  WriteByte(SI114X_IRQ_MODE2, 0);
  WriteByte(SI114X_INT_CFG, 0);
  WriteByte(SI114X_IRQ_STATUS, 0xFF);

  WriteByte(SI114X_COMMAND, SI114X_RESET);
  delay(10);
  WriteByte(SI114X_HW_KEY, 0x17);
  delay(10);
}

uint8_t SI114X::GetTransmissionError()
{
    return TransmissionError;
}

/*--------------------------------------------------------//
write one byte into si114x's reg

 */
void SI114X::WriteByte(uint8_t Reg, uint8_t Value)
{
  Wire.beginTransmission(SI114X_ADDR); 
  Wire.write(Reg); 
  Wire.write(Value); 
  TransmissionError = Wire.endTransmission(); 
}
/*--------------------------------------------------------//
read one byte data from si114x

 */
uint8_t SI114X::ReadByte(uint8_t Reg)
{
    Wire.beginTransmission(SI114X_ADDR);
    Wire.write(Reg);
    TransmissionError = Wire.endTransmission();
    Wire.requestFrom(SI114X_ADDR, 1);  
    return Wire.read();
}
/*--------------------------------------------------------//
read half word(2 bytes) data from si114x

 */
uint16_t SI114X::ReadHalfWord(uint8_t Reg)
{
  uint16_t Value;
  Wire.beginTransmission(SI114X_ADDR);
  Wire.write(Reg); 
  TransmissionError = Wire.endTransmission(); 
  Wire.requestFrom(SI114X_ADDR, 2);
  Value = Wire.read();
  Value |= (uint16_t)Wire.read() << 8; 
  return Value;
}
/*--------------------------------------------------------//
read param data

 */
uint8_t SI114X::ReadParamData(uint8_t Reg)
{
	WriteByte(SI114X_COMMAND, Reg | SI114X_QUERY);
	return ReadByte(SI114X_RD);
}
/*--------------------------------------------------------//
writ param data

 */
uint8_t SI114X::WriteParamData(uint8_t Reg,uint8_t Value)
{
	//write Value into PARAMWR reg first
   WriteByte(SI114X_WR, Value);
   WriteByte(SI114X_COMMAND, Reg | SI114X_SET);
   //SI114X writes value out to PARAM_RD,read and confirm its right
   return ReadByte(SI114X_RD);
}

/*--------------------------------------------------------//
Read Visible Value

 */
 uint16_t SI114X::ReadVisible(void)
{
  return ReadHalfWord(SI114X_ALS_VIS_DATA0); 	
}
 /*--------------------------------------------------------//
Read IR Value

 */
 uint16_t SI114X::ReadIR(void)
{
  return ReadHalfWord(SI114X_ALS_IR_DATA0); 	
} 
/*--------------------------------------------------------//
Read Proximity Value

 */
 uint16_t SI114X::ReadProximity(uint8_t PSn)
{
  return ReadHalfWord(PSn); 	
}
 /*--------------------------------------------------------//
Read UV Value
this function is a int value ,but the real value must be div 100

 */
uint16_t SI114X::ReadUV(void)
{
  return (ReadHalfWord(SI114X_AUX_DATA0_UVINDEX0)); 	
}

