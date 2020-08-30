/*!
 *  @file Adafruit_MLX90640.h
 *
 * 	I2C Driver for MLX90640 24x32 IR Thermal Camera
 *
 * 	This is a library for the Adafruit MLX90640 breakout:
 * 	https://www.adafruit.com/products/4407
 *
 * 	Adafruit invests time and resources providing this open source code,
 *  please support Adafruit and open-source hardware by purchasing products from
 * 	Adafruit!
 *
 *
 *	BSD license (see license.txt)
 */

#ifndef _ADAFRUIT_MLX90640_H
#define _ADAFRUIT_MLX90640_H

#include "Arduino.h"
#include "headers/MLX90640_API.h"
#include <Adafruit_I2CDevice.h>
#include <Wire.h>

#define MLX90640_I2CADDR_DEFAULT 0x33 ///< I2C address by default

#define MLX90640_DEVICEID1 0x2407 ///< I2C identification register

/** Mode to read pixel frames (two per image) */
typedef enum mlx90640_mode {
  MLX90640_INTERLEAVED, ///< Read data from camera by interleaved lines
  MLX90640_CHESS,       ///< Read data from camera in alternating pixels
} mlx90640_mode_t;

/** Internal ADC resolution for pixel calculation */
typedef enum mlx90640_res {
  MLX90640_ADC_16BIT,
  MLX90640_ADC_17BIT,
  MLX90640_ADC_18BIT,
  MLX90640_ADC_19BIT,
} mlx90640_resolution_t;

/** How many PAGES we will read per second (2 pages per frame) */
typedef enum mlx90640_refreshrate {
  MLX90640_0_5_HZ,
  MLX90640_1_HZ,
  MLX90640_2_HZ,
  MLX90640_4_HZ,
  MLX90640_8_HZ,
  MLX90640_16_HZ,
  MLX90640_32_HZ,
  MLX90640_64_HZ,
} mlx90640_refreshrate_t;

#define OPENAIR_TA_SHIFT 8 ///< Default 8 degree offset from ambient air

/*!
 *    @brief  Class that stores state and functions for interacting with
 *            the MLX90640 sensor
 */
class Adafruit_MLX90640 {
public:
  Adafruit_MLX90640();
  boolean begin(uint8_t i2c_addr = MLX90640_I2CADDR_DEFAULT,
                TwoWire *wire = &Wire);

  mlx90640_mode_t getMode(void);
  void setMode(mlx90640_mode_t mode);
  mlx90640_resolution_t getResolution(void);
  void setResolution(mlx90640_resolution_t res);
  mlx90640_refreshrate_t getRefreshRate(void);
  void setRefreshRate(mlx90640_refreshrate_t res);

  int getFrame(float *framebuf);

  // uint16_t serialNumber[3]; ///< Unique serial number read from device

private:
  int MLX90640_I2CRead(uint8_t slaveAddr, uint16_t startAddress,
                       uint16_t nMemAddressRead, uint16_t *data);
  int MLX90640_I2CWrite(uint8_t slaveAddr, uint16_t writeAddress,
                        uint16_t data);

  Adafruit_I2CDevice *i2c_dev;
  paramsMLX90640 _params;

  int MLX90640_DumpEE(uint8_t slaveAddr, uint16_t *eeData);
  int MLX90640_GetFrameData(uint8_t slaveAddr, uint16_t *frameData);
  int MLX90640_ExtractParameters(uint16_t *eeData, paramsMLX90640 *mlx90640);
  float MLX90640_GetVdd(uint16_t *frameData, const paramsMLX90640 *params);
  float MLX90640_GetTa(uint16_t *frameData, const paramsMLX90640 *params);
  void MLX90640_GetImage(uint16_t *frameData, const paramsMLX90640 *params,
                         float *result);
  void MLX90640_CalculateTo(uint16_t *frameData, const paramsMLX90640 *params,
                            float emissivity, float tr, float *result);
  int MLX90640_SetResolution(uint8_t slaveAddr, uint8_t resolution);
  int MLX90640_GetCurResolution(uint8_t slaveAddr);
  int MLX90640_SetRefreshRate(uint8_t slaveAddr, uint8_t refreshRate);
  int MLX90640_GetRefreshRate(uint8_t slaveAddr);
  int MLX90640_GetSubPageNumber(uint16_t *frameData);
  int MLX90640_GetCurMode(uint8_t slaveAddr);
  int MLX90640_SetInterleavedMode(uint8_t slaveAddr);
  int MLX90640_SetChessMode(uint8_t slaveAddr);
  void MLX90640_BadPixelsCorrection(uint16_t *pixels, float *to, int mode,
                                    paramsMLX90640 *params);
};
#endif
