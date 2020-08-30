/**
 * @copyright (C) 2017 Melexis N.V.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
/**
 * As the timings depend heavily on the MCU in use, it is recommended
 * to make sure that the proper timings are achieved. For that purpose
 * an oscilloscope might be needed to strobe the SCL and SDA signals.
 * The Wait(int) function could be modified in order to better
 * trim the frequency. For coarse setting of the frequency or
 * dynamic frequency change using the default function implementation,
 * ‘freqCnt’ argument should be changed – lower value results in
 * higher frequency.
 */

#if 0 // we dont be using the mbed layer!

#include "MLX90640_I2C_Driver.h"
#include "mbed.h"


DigitalInOut sda(p9);
DigitalOut scl(p10);

#define LOW 0;
#define HIGH 1;

#define SCL_HIGH scl = HIGH;
#define SCL_LOW scl = LOW;
#define SDA_HIGH sda.input();
#define SDA_LOW                                                                \
  sda.output();                                                                \
  sda = LOW;           

int I2CSendByte(int8_t);
void I2CReadBytes(int, char *);
void I2CStart(void);
void I2CStop(void);
void I2CRepeatedStart(void);
void I2CSendACK(void);
void I2CSendNack(void);
int I2CReceiveAck(void);
void Wait(int);

static int freqCnt;

void MLX90640_I2CInit()
{   
    I2CStop();
}
    
int MLX90640_I2CRead(uint8_t slaveAddr, uint16_t startAddress,uint16_t nMemAddressRead, uint16_t *data)
{
    uint8_t sa;
    int ack = 0;
    int cnt = 0;
    int i = 0;
    char cmd[2] = {0,0};
    char i2cData[1664] = {0};
    uint16_t *p;
    
    p = data;
    sa = (slaveAddr << 1);
    cmd[0] = startAddress >> 8;
    cmd[1] = startAddress & 0x00FF;
    
    I2CStop();
    Wait(freqCnt);  
    I2CStart();
    Wait(freqCnt);
    
    ack = I2CSendByte(sa)!=0;
    if(ack != 0)
    {
        return -1;
    } 
    
    ack = I2CSendByte(cmd[0])!=0;   
    if(ack != 0)
    {
        return -1;
    }
    
    ack = I2CSendByte(cmd[1])!=0;    
    if(ack != 0)
    {
        return -1;
    }  
    
    I2CRepeatedStart();
       
    sa = sa | 0x01;
    
    ack = I2CSendByte(sa);
    if(ack != 0)
    {
        return -1;
    } 
        
    I2CReadBytes((nMemAddressRead << 1), i2cData);
              
    I2CStop();   

    for(cnt=0; cnt < nMemAddressRead; cnt++)
    {
        i = cnt << 1;
        *p++ = (int)i2cData[i]*256 + (int)i2cData[i+1];
    } 
    return 0;
  
} 

void MLX90640_I2CFreqSet(int freq)
{
    freqCnt = freq>>1;
}

int MLX90640_I2CWrite(uint8_t slaveAddr, uint16_t writeAddress, uint16_t data)
{
    uint8_t sa;
    int ack = 0;
    char cmd[4] = {0,0,0,0};
    static uint16_t dataCheck;

    sa = (slaveAddr << 1);
    cmd[0] = writeAddress >> 8;
    cmd[1] = writeAddress & 0x00FF;
    cmd[2] = data >> 8;
    cmd[3] = data & 0x00FF;

    I2CStop();
    Wait(freqCnt);
    I2CStart();
    ack = I2CSendByte(sa);
    if (ack != 0x00)
    {
        return 1; 
    }  
    
    for(int i = 0; i<4; i++)
    {
        ack = I2CSendByte(cmd[i]);
    
        if (ack != 0x00)
        {
            return -1;
        }  
    }           
    I2CStop();   
    
    MLX90640_I2CRead(slaveAddr,writeAddress,1, &dataCheck);
    
    if ( dataCheck != data)
    {
        return -2;
    }    
    
    return 0;
}

int I2CSendByte(int8_t data)
{
   int ack = 1;
   int8_t byte = data; 
   
   for(int i=0;i<8;i++)
   {
       Wait(freqCnt);
       
       if(byte & 0x80)
       {
           SDA_HIGH;
       }
       else
       {
           SDA_LOW;
       }
       Wait(freqCnt);
       SCL_HIGH;
       Wait(freqCnt);
       Wait(freqCnt);
       SCL_LOW;
       byte = byte<<1;        
   }    
   
   Wait(freqCnt);
   ack = I2CReceiveAck();
   
   return ack; 
}

void I2CReadBytes(int nBytes, char *dataP)
{
    char data;
    for(int j=0;j<nBytes;j++)
    {
        Wait(freqCnt);
        SDA_HIGH;    
        
        data = 0;
        for(int i=0;i<8;i++){
            Wait(freqCnt);
            SCL_HIGH;
            Wait(freqCnt);
            data = data<<1;
            if(sda == 1){
                data = data+1;  
            }
            Wait(freqCnt);
            SCL_LOW;
            Wait(freqCnt);
        }  
        
        if(j == (nBytes-1))
        {
            I2CSendNack();
        }
        else
        {                  
            I2CSendACK();
        }
            
        *(dataP+j) = data; 
    }    
    
}
        
void Wait(int freqCnt)
{
    int cnt;
    for(int i = 0;i<freqCnt;i++)
    {
        cnt = cnt++; 
    }    
} 

void I2CStart(void)
{
    SDA_HIGH;
    SCL_HIGH;
    Wait(freqCnt);
    Wait(freqCnt);
    SDA_LOW;
    Wait(freqCnt);
    SCL_LOW;
    Wait(freqCnt);    
    
}

void I2CStop(void)
{
    SCL_LOW;
    SDA_LOW;
    Wait(freqCnt);
    SCL_HIGH;
    Wait(freqCnt);
    SDA_HIGH;
    Wait(freqCnt);
} 
 
void I2CRepeatedStart(void)
{
    SCL_LOW;
    Wait(freqCnt);
    SDA_HIGH;
    Wait(freqCnt);
    SCL_HIGH;
    Wait(freqCnt);
    SDA_LOW;
    Wait(freqCnt);
    SCL_LOW;
           
}

void I2CSendACK(void)
{
    SDA_LOW;
    Wait(freqCnt);
    SCL_HIGH;
    Wait(freqCnt);
    Wait(freqCnt);
    SCL_LOW;
    Wait(freqCnt);
    SDA_HIGH;
    
}

void I2CSendNack(void)
{
    SDA_HIGH;
    Wait(freqCnt);
    SCL_HIGH;
    Wait(freqCnt);
    Wait(freqCnt);
    SCL_LOW;
    Wait(freqCnt);
    SDA_HIGH;
    
}

int I2CReceiveAck(void)
{
    int ack;
    
    SDA_HIGH;
    Wait(freqCnt);
    SCL_HIGH;
    Wait(freqCnt);
    if(sda == 0)
    {
        ack = 0;
    }
    else
    {
        ack = 1;
    }
    Wait(freqCnt);        
    SCL_LOW;
    SDA_LOW;
    
    return ack;    
}

#endif
