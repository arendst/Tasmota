#include "MPU6886.h"
#include <math.h>
#include <Arduino.h>

void MPU6886::I2C_Read_NBytes(uint8_t driver_Addr, uint8_t start_Addr, uint8_t number_Bytes, uint8_t *read_Buffer){
    
  myWire->beginTransmission(driver_Addr);
  myWire->write(start_Addr);  
  myWire->endTransmission(false);
  uint8_t i = 0;
  myWire->requestFrom(driver_Addr,number_Bytes);
  
  //! Put read results in the Rx buffer
  while (myWire->available()) {
    read_Buffer[i++] = myWire->read();
  }        
}

void MPU6886::I2C_Write_NBytes(uint8_t driver_Addr, uint8_t start_Addr, uint8_t number_Bytes, uint8_t *write_Buffer){

  myWire->beginTransmission(driver_Addr);
  myWire->write(start_Addr);
  myWire->write(*write_Buffer);
  myWire->endTransmission();

}

int MPU6886::Init(void){
  unsigned char tempdata[1];
  unsigned char regdata;
    
  I2C_Read_NBytes(MPU6886_ADDRESS, MPU6886_WHOAMI, 1, tempdata);
  if(tempdata[0] != 0x19)
    return -1;
  delay(1);
  
  regdata = 0x00;
  I2C_Write_NBytes(MPU6886_ADDRESS, MPU6886_PWR_MGMT_1, 1, &regdata);
  delay(10);

  regdata = (0x01<<7);
  I2C_Write_NBytes(MPU6886_ADDRESS, MPU6886_PWR_MGMT_1, 1, &regdata);
  delay(10);

  regdata = (0x01<<0);
  I2C_Write_NBytes(MPU6886_ADDRESS, MPU6886_PWR_MGMT_1, 1, &regdata);
  delay(10);

  regdata = 0x10; // AFS_8G
  I2C_Write_NBytes(MPU6886_ADDRESS, MPU6886_ACCEL_CONFIG, 1, &regdata);
  delay(1);

  regdata = 0x18; // GFS_2000DPS
  I2C_Write_NBytes(MPU6886_ADDRESS, MPU6886_GYRO_CONFIG, 1, &regdata);
  delay(1);

  regdata = 0x01;
  I2C_Write_NBytes(MPU6886_ADDRESS, MPU6886_CONFIG, 1, &regdata);
  delay(1);

  regdata = 0x05;
  I2C_Write_NBytes(MPU6886_ADDRESS, MPU6886_SMPLRT_DIV, 1,&regdata);
  delay(1);

  regdata = 0x00;
  I2C_Write_NBytes(MPU6886_ADDRESS, MPU6886_INT_ENABLE, 1, &regdata);
  delay(1);

  regdata = 0x00;
  I2C_Write_NBytes(MPU6886_ADDRESS, MPU6886_ACCEL_CONFIG2, 1, &regdata);
  delay(1);

  regdata = 0x00;
  I2C_Write_NBytes(MPU6886_ADDRESS, MPU6886_USER_CTRL, 1, &regdata);
  delay(1);

  regdata = 0x00;
  I2C_Write_NBytes(MPU6886_ADDRESS, MPU6886_FIFO_EN, 1, &regdata);
  delay(1);

  regdata = 0x22;
  I2C_Write_NBytes(MPU6886_ADDRESS, MPU6886_INT_PIN_CFG, 1, &regdata);
  delay(1);

  regdata = 0x01;
  I2C_Write_NBytes(MPU6886_ADDRESS, MPU6886_INT_ENABLE, 1, &regdata);

  delay(100);
  getGres();
  getAres();
  return 0;
}

void MPU6886::getAccelAdc(int16_t* ax, int16_t* ay, int16_t* az){

   uint8_t buf[6];  
   I2C_Read_NBytes(MPU6886_ADDRESS,MPU6886_ACCEL_XOUT_H,6,buf);
   
   *ax=((int16_t)buf[0]<<8)|buf[1];
   *ay=((int16_t)buf[2]<<8)|buf[3];
   *az=((int16_t)buf[4]<<8)|buf[5];

}
void MPU6886::getGyroAdc(int16_t* gx, int16_t* gy, int16_t* gz){

  uint8_t buf[6];
  I2C_Read_NBytes(MPU6886_ADDRESS,MPU6886_GYRO_XOUT_H,6,buf);
  
  *gx=((uint16_t)buf[0]<<8)|buf[1];  
  *gy=((uint16_t)buf[2]<<8)|buf[3];  
  *gz=((uint16_t)buf[4]<<8)|buf[5];
  
}

void MPU6886::getTempAdc(int16_t *t){
  
  uint8_t buf[2];  
  I2C_Read_NBytes(MPU6886_ADDRESS,MPU6886_TEMP_OUT_H,2,buf);
  
  *t=((uint16_t)buf[0]<<8)|buf[1];  
}



// //!俯仰，航向，横滚：pitch，yaw，roll，指三维空间中飞行器的旋转状态。
// void MPU6886::getAhrsData(float *pitch,float *roll,float *yaw){

//   float accX = 0; 
//   float accY = 0;
//   float accZ = 0;

//   float gyroX = 0;
//   float gyroY = 0;
//   float gyroZ = 0;


//   getGyroData(&gyroX,&gyroY,&gyroZ);
//   getAccelData(&accX,&accY,&accZ);
  
//   MahonyAHRSupdateIMU(gyroX * DEG_TO_RAD, gyroY * DEG_TO_RAD, gyroZ * DEG_TO_RAD, accX, accY, accZ,pitch,roll,yaw);

// }

void MPU6886::getGres(){

   switch (Gyscale)
   {
  // Possible gyro scales (and their register bit settings) are:
     case GFS_250DPS:
           gRes = 250.0f/32768.0f;
           gyRange = 250;
           break;
     case GFS_500DPS:
           gRes = 500.0f/32768.0f;
           gyRange = 500;
           break;
     case GFS_1000DPS:
           gRes = 1000.0f/32768.0f;
           gyRange = 1000;
           break;
     case GFS_2000DPS:
           gRes = 2000.0f/32768.0f;
           gyRange = 2000;
           break;
   }

}


void MPU6886::getAres(){
   switch (Acscale)
   {
   // Possible accelerometer scales (and their register bit settings) are:
   // 2 Gs (00), 4 Gs (01), 8 Gs (10), and 16 Gs  (11). 
   // Here's a bit of an algorith to calculate DPS/(ADC tick) based on that 2-bit value:
    case AFS_2G:
          aRes = 2.0/32768.0;
          acRange = 2000;
          break;
    case AFS_4G:
          aRes = 4.0/32768.0;
          acRange = 4000;
          break;
    case AFS_8G:
          aRes = 8.0/32768.0;
          acRange = 8000;
          break;
    case AFS_16G:
          aRes = 16.0/32768.0;
          acRange = 16000;
          break;
  }

}
 
void MPU6886::SetGyroFsr(Gscale scale)
{
    //return IIC_Write_Byte(MPU_GYRO_CFG_REG,scale<<3);//设置陀螺仪满量程范围
    unsigned char regdata;	
    regdata = (scale<<3);
    I2C_Write_NBytes(MPU6886_ADDRESS, MPU6886_GYRO_CONFIG, 1, &regdata);
    delay(10);

    Gyscale = scale;
    getGres();
}

void MPU6886::SetAccelFsr(Ascale scale)
{
    unsigned char regdata;	
    regdata = (scale<<3);
    I2C_Write_NBytes(MPU6886_ADDRESS, MPU6886_ACCEL_CONFIG, 1, &regdata);
    delay(10);

    Acscale = scale;
    getAres();
}


// x/y/z are in 1/1000 if g
// avoiding costly float calculations
void MPU6886::getAccelDataInt(int16_t* ax, int16_t* ay, int16_t* az) {
  int16_t accX = 0;
  int16_t accY = 0;
  int16_t accZ = 0;
  getAccelAdc(&accX, &accY, &accZ);

  if (ax != nullptr) { *ax = ((int32_t)accX * acRange) / 0x7FFFL; }
  if (ay != nullptr) { *ay = ((int32_t)accY * acRange) / 0x7FFFL; }
  if (az != nullptr) { *az = ((int32_t)accZ * acRange) / 0x7FFFL; }

}

void MPU6886::getAccelData(float* ax, float* ay, float* az){


  int16_t accX = 0;
  int16_t accY = 0;
  int16_t accZ = 0;
  getAccelAdc(&accX,&accY,&accZ);


  *ax = (float)accX * aRes;
  *ay = (float)accY * aRes;
  *az = (float)accZ * aRes;

}
      
// x/y/z are in dps - degrees per second
// avoiding costly float calculations
void MPU6886::getGyroDataInt(int16_t* ax, int16_t* ay, int16_t* az) {
  int16_t gyX = 0;
  int16_t gyY = 0;
  int16_t gyZ = 0;
  getGyroAdc(&gyX, &gyY, &gyZ);

  if (ax != nullptr) { *ax = ((int32_t)gyX * gyRange) / 0x7FFFL; }
  if (ay != nullptr) { *ay = ((int32_t)gyY * gyRange) / 0x7FFFL; }
  if (az != nullptr) { *az = ((int32_t)gyZ * gyRange) / 0x7FFFL; }

}

void MPU6886::getGyroData(float* gx, float* gy, float* gz){
  int16_t gyroX = 0;
  int16_t gyroY = 0;
  int16_t gyroZ = 0;
  getGyroAdc(&gyroX,&gyroY,&gyroZ);

  *gx = (float)gyroX * gRes;
  *gy = (float)gyroY * gRes;
  *gz = (float)gyroZ * gRes;
}

void MPU6886::getTempData(float *t){
  
  int16_t temp = 0;
  getTempAdc(&temp);
  
  *t = (float)temp / 326.8 + 25.0;
}
