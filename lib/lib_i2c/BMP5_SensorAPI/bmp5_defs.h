/**
* Copyright (c) 2022 Bosch Sensortec GmbH. All rights reserved.
*
* BSD-3-Clause
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
*
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the distribution.
*
* 3. Neither the name of the copyright holder nor the names of its
*    contributors may be used to endorse or promote products derived from
*    this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
* COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*
* @file       bmp5_defs.h
* @date       2022-08-11
* @version    v1.1.1
*
*/

/*! @file bmp5_defs.h
 *  @brief Sensor Driver for BMP5 sensor
 */
#ifndef _BMP5_DEFS_H
#define _BMP5_DEFS_H

/******************************************************************************/
/*! @name       HEADER INCLUDES                           */
/******************************************************************************/
#ifdef __KERNEL__
#include <linux/types.h>
#else
#include <stdint.h>
#include <stddef.h>
#include <math.h>
#endif

/******************************************************************************/
/*! @name       COMMON MACROS                         */
/******************************************************************************/
#ifdef __KERNEL__

#if (!defined(UINT8_C) && !defined(INT8_C))
#define INT8_C(x)    S8_C(x)
#define UINT8_C(x)   U8_C(x)
#endif

#if (!defined(UINT16_C) && !defined(INT16_C))
#define INT16_C(x)   S16_C(x)
#define UINT16_C(x)  U16_C(x)
#endif

#if (!defined(INT32_C) && !defined(UINT32_C))
#define INT32_C(x)   S32_C(x)
#define UINT32_C(x)  U32_C(x)
#endif

#if (!defined(INT64_C) && !defined(UINT64_C))
#define INT64_C(x)   S64_C(x)
#define UINT64_C(x)  U64_C(x)
#endif

#else /* __KERNEL__ */

#if (!defined(UINT8_C) && !defined(INT8_C))
#define INT8_C(x)    (x)
#define UINT8_C(x)   (x##U)
#endif

#if (!defined(UINT16_C) && !defined(INT16_C))
#define INT16_C(x)   (x)
#define UINT16_C(x)  (x##U)
#endif

#if (!defined(INT32_C) && !defined(UINT32_C))
#define INT32_C(x)   (x)
#define UINT32_C(x)  (x##U)
#endif

#if (!defined(INT64_C) && !defined(UINT64_C))
#define INT64_C(x)   (x##LL)
#define UINT64_C(x)  (x##ULL)
#endif

#endif /* __KERNEL__ */

/*! @name C standard macros */
#ifndef NULL
#ifdef __cplusplus
#define NULL         0
#else
#define NULL         ((void *) 0)
#endif
#endif

/******************************************************************************/
/*! @name        Compiler switch macros Definitions                */
/******************************************************************************/
#ifndef BMP5_USE_FIXED_POINT /*< Check if floating point (using BMP5_USE_FIXED_POINT) is enabled */
#ifndef BMP5_USE_FLOATING_POINT /*< If fixed point is not enabled then enable BMP5_USE_FLOATING_POINT */
#define BMP5_USE_FLOATING_POINT
#endif
#endif

#ifdef BMP5_USE_FIXED_POINT /*< If Fixed point is defined set BMP5_FIXED_POINT_DIGIT_PRECISION */
#ifdef BMP5_FIXED_POINT_DIGIT_PRECISION
#if BMP5_FIXED_POINT_DIGIT_PRECISION > 6 /* If BMP5_FIXED_POINT_DIGIT_PRECISION(provided by user) is greater than 6 then
                                          * set precision value to 6 digits */
#undef BMP5_FIXED_POINT_DIGIT_PRECISION
#define BMP5_FIXED_POINT_DIGIT_PRECISION  UINT8_C(6)
#endif
#endif

/* By default BMP5_FIXED_POINT_DIGIT_PRECISION will be 6 digits */
/* User can vary the precision between 0 to 6 digits */
#ifndef BMP5_FIXED_POINT_DIGIT_PRECISION
#define BMP5_FIXED_POINT_DIGIT_PRECISION  UINT8_C(6)
#endif
#endif

/******************************************************************************/
/*! @name        GENERAL MACRO DEFINITIONS                */
/******************************************************************************/

/*!
 * BMP5_INTF_RET_TYPE is the read/write interface return type which can be overwritten by the build system.
 */
#ifndef BMP5_INTF_RET_TYPE
#define BMP5_INTF_RET_TYPE                        int8_t
#endif

/*!
 * The last error code from read/write interface is stored in the device structure as intf_rslt.
 */
#ifndef BMP5_INTF_RET_SUCCESS
#define BMP5_INTF_RET_SUCCESS                     INT8_C(0)
#endif

/*! @name BOOLEAN TYPES */
#ifndef TRUE
#define TRUE                                      UINT8_C(0x01)
#endif

#ifndef FALSE
#define FALSE                                     UINT8_C(0x00)
#endif

#ifndef NULL
#define NULL                                      UINT8_C(0x00)
#endif

#ifndef ABS
#define ABS(a)                                    ((a) > 0 ? (a) : -(a))    /*!< Absolute value */
#endif

/*! @name UTILITY MACROS */
#define BMP5_SET_LOW_BYTE                         UINT16_C(0x00FF)
#define BMP5_SET_HIGH_BYTE                        UINT16_C(0xFF00)

/*! @name BIT SLICE GET AND SET FUNCTIONS */
#define BMP5_GET_BITSLICE(regvar, bitname) \
    ((regvar & bitname##_MSK) >> bitname##_POS)

#define BMP5_SET_BITSLICE(regvar, bitname, val) \
    ((regvar & ~bitname##_MSK) | \
     ((val << bitname##_POS) & bitname##_MSK))

#define BMP5_GET_LSB(var)                         (uint8_t)(var & BMP5_SET_LOW_BYTE)
#define BMP5_GET_MSB(var)                         (uint8_t)((var & BMP5_SET_HIGH_BYTE) >> 8)

#define BMP5_SET_BIT_VAL_0(reg_data, bitname)     (reg_data & ~(bitname##_MSK))

#define BMP5_SET_BITS_POS_0(reg_data, bitname, data) \
    ((reg_data & ~(bitname##_MSK)) | \
     (data & bitname##_MSK))

#define BMP5_GET_BITS_POS_0(reg_data, bitname)    (reg_data & (bitname##_MSK))

/*! @name Chip id of BMP5 */
#define BMP5_CHIP_ID_PRIM                         UINT8_C(0x50)
#define BMP5_CHIP_ID_SEC                          UINT8_C(0x51)

/*! @name API success code */
#define BMP5_OK                                   INT8_C(0)

/*! @name API error codes */
#define BMP5_E_NULL_PTR                           INT8_C(-1)
#define BMP5_E_COM_FAIL                           INT8_C(-2)
#define BMP5_E_DEV_NOT_FOUND                      INT8_C(-3)
#define BMP5_E_INVALID_CHIP_ID                    INT8_C(-4)
#define BMP5_E_POWER_UP                           INT8_C(-5)
#define BMP5_E_POR_SOFTRESET                      INT8_C(-6)
#define BMP5_E_INVALID_POWERMODE                  INT8_C(-7)
#define BMP5_E_INVALID_THRESHOLD                  INT8_C(-8)
#define BMP5_E_FIFO_FRAME_EMPTY                   INT8_C(-9)
#define BMP5_E_NVM_INVALID_ADDR                   INT8_C(-10)
#define BMP5_E_NVM_NOT_READY                      INT8_C(-11)

#define BMP5_ENABLE                               UINT8_C(0x01)
#define BMP5_DISABLE                              UINT8_C(0x00)

/*! @name Register addresses */
#define BMP5_REG_CHIP_ID                          UINT8_C(0x01)
#define BMP5_REG_REV_ID                           UINT8_C(0x02)
#define BMP5_REG_CHIP_STATUS                      UINT8_C(0x11)
#define BMP5_REG_DRIVE_CONFIG                     UINT8_C(0x13)
#define BMP5_REG_INT_CONFIG                       UINT8_C(0x14)
#define BMP5_REG_INT_SOURCE                       UINT8_C(0x15)
#define BMP5_REG_FIFO_CONFIG                      UINT8_C(0x16)
#define BMP5_REG_FIFO_COUNT                       UINT8_C(0x17)
#define BMP5_REG_FIFO_SEL                         UINT8_C(0x18)
#define BMP5_REG_TEMP_DATA_XLSB                   UINT8_C(0x1D)
#define BMP5_REG_TEMP_DATA_LSB                    UINT8_C(0x1E)
#define BMP5_REG_TEMP_DATA_MSB                    UINT8_C(0x1F)
#define BMP5_REG_PRESS_DATA_XLSB                  UINT8_C(0x20)
#define BMP5_REG_PRESS_DATA_LSB                   UINT8_C(0x21)
#define BMP5_REG_PRESS_DATA_MSB                   UINT8_C(0x22)
#define BMP5_REG_INT_STATUS                       UINT8_C(0x27)
#define BMP5_REG_STATUS                           UINT8_C(0x28)
#define BMP5_REG_FIFO_DATA                        UINT8_C(0x29)
#define BMP5_REG_NVM_ADDR                         UINT8_C(0x2B)
#define BMP5_REG_NVM_DATA_LSB                     UINT8_C(0x2C)
#define BMP5_REG_NVM_DATA_MSB                     UINT8_C(0x2D)
#define BMP5_REG_DSP_CONFIG                       UINT8_C(0x30)
#define BMP5_REG_DSP_IIR                          UINT8_C(0x31)
#define BMP5_REG_OOR_THR_P_LSB                    UINT8_C(0x32)
#define BMP5_REG_OOR_THR_P_MSB                    UINT8_C(0x33)
#define BMP5_REG_OOR_RANGE                        UINT8_C(0x34)
#define BMP5_REG_OOR_CONFIG                       UINT8_C(0x35)
#define BMP5_REG_OSR_CONFIG                       UINT8_C(0x36)
#define BMP5_REG_ODR_CONFIG                       UINT8_C(0x37)
#define BMP5_REG_OSR_EFF                          UINT8_C(0x38)
#define BMP5_REG_CMD                              UINT8_C(0x7E)

/*! @name I2C addresses */
#define BMP5_I2C_ADDR_PRIM                        UINT8_C(0x46)
#define BMP5_I2C_ADDR_SEC                         UINT8_C(0x47)

/*! @name NVM addresses */
#define BMP5_NVM_START_ADDR                       UINT8_C(0x20)
#define BMP5_NVM_END_ADDR                         UINT8_C(0x22)

/*! @name Interface settings */
#define BMP5_SPI_RD_MASK                          UINT8_C(0x80)

/*! @name Delay definition */
#define BMP5_DELAY_US_SOFT_RESET                  UINT16_C(2000)
#define BMP5_DELAY_US_STANDBY                     UINT16_C(2500)
#define BMP5_DELAY_US_NVM_READY_READ              UINT8_C(800)
#define BMP5_DELAY_US_NVM_READY_WRITE             UINT16_C(10000)

/*! @name Soft reset command */
#define BMP5_SOFT_RESET_CMD                       UINT8_C(0xB6)

/*! NVM command */
#define BMP5_NVM_FIRST_CMND                       UINT8_C(0x5D)
#define BMP5_NVM_READ_ENABLE_CMND                 UINT8_C(0xA5)
#define BMP5_NVM_WRITE_ENABLE_CMND                UINT8_C(0xA0)

/*! @name Deepstandby enable/disable */
#define BMP5_DEEP_ENABLED                         UINT8_C(0)
#define BMP5_DEEP_DISABLED                        UINT8_C(1)

/*! @name ODR settings */
#define BMP5_ODR_240_HZ                           UINT8_C(0x00)
#define BMP5_ODR_218_5_HZ                         UINT8_C(0x01)
#define BMP5_ODR_199_1_HZ                         UINT8_C(0x02)
#define BMP5_ODR_179_2_HZ                         UINT8_C(0x03)
#define BMP5_ODR_160_HZ                           UINT8_C(0x04)
#define BMP5_ODR_149_3_HZ                         UINT8_C(0x05)
#define BMP5_ODR_140_HZ                           UINT8_C(0x06)
#define BMP5_ODR_129_8_HZ                         UINT8_C(0x07)
#define BMP5_ODR_120_HZ                           UINT8_C(0x08)
#define BMP5_ODR_110_1_HZ                         UINT8_C(0x09)
#define BMP5_ODR_100_2_HZ                         UINT8_C(0x0A)
#define BMP5_ODR_89_6_HZ                          UINT8_C(0x0B)
#define BMP5_ODR_80_HZ                            UINT8_C(0x0C)
#define BMP5_ODR_70_HZ                            UINT8_C(0x0D)
#define BMP5_ODR_60_HZ                            UINT8_C(0x0E)
#define BMP5_ODR_50_HZ                            UINT8_C(0x0F)
#define BMP5_ODR_45_HZ                            UINT8_C(0x10)
#define BMP5_ODR_40_HZ                            UINT8_C(0x11)
#define BMP5_ODR_35_HZ                            UINT8_C(0x12)
#define BMP5_ODR_30_HZ                            UINT8_C(0x13)
#define BMP5_ODR_25_HZ                            UINT8_C(0x14)
#define BMP5_ODR_20_HZ                            UINT8_C(0x15)
#define BMP5_ODR_15_HZ                            UINT8_C(0x16)
#define BMP5_ODR_10_HZ                            UINT8_C(0x17)
#define BMP5_ODR_05_HZ                            UINT8_C(0x18)
#define BMP5_ODR_04_HZ                            UINT8_C(0x19)
#define BMP5_ODR_03_HZ                            UINT8_C(0x1A)
#define BMP5_ODR_02_HZ                            UINT8_C(0x1B)
#define BMP5_ODR_01_HZ                            UINT8_C(0x1C)
#define BMP5_ODR_0_5_HZ                           UINT8_C(0x1D)
#define BMP5_ODR_0_250_HZ                         UINT8_C(0x1E)
#define BMP5_ODR_0_125_HZ                         UINT8_C(0x1F)

/*! @name Oversampling for temperature and pressure */
#define BMP5_OVERSAMPLING_1X                      UINT8_C(0x00)
#define BMP5_OVERSAMPLING_2X                      UINT8_C(0x01)
#define BMP5_OVERSAMPLING_4X                      UINT8_C(0x02)
#define BMP5_OVERSAMPLING_8X                      UINT8_C(0x03)
#define BMP5_OVERSAMPLING_16X                     UINT8_C(0x04)
#define BMP5_OVERSAMPLING_32X                     UINT8_C(0x05)
#define BMP5_OVERSAMPLING_64X                     UINT8_C(0x06)
#define BMP5_OVERSAMPLING_128X                    UINT8_C(0x07)

/*! @name IIR filter for temperature and pressure */
#define BMP5_IIR_FILTER_BYPASS                    UINT8_C(0x00)
#define BMP5_IIR_FILTER_COEFF_1                   UINT8_C(0x01)
#define BMP5_IIR_FILTER_COEFF_3                   UINT8_C(0x02)
#define BMP5_IIR_FILTER_COEFF_7                   UINT8_C(0x03)
#define BMP5_IIR_FILTER_COEFF_15                  UINT8_C(0x04)
#define BMP5_IIR_FILTER_COEFF_31                  UINT8_C(0x05)
#define BMP5_IIR_FILTER_COEFF_63                  UINT8_C(0x06)
#define BMP5_IIR_FILTER_COEFF_127                 UINT8_C(0x07)

/*! Fifo frame configuration */
#define BMP5_FIFO_EMPTY                           UINT8_C(0X7F)
#define BMP5_FIFO_MAX_THRESHOLD_P_T_MODE          UINT8_C(0x0F)
#define BMP5_FIFO_MAX_THRESHOLD_P_MODE            UINT8_C(0x1F)

/*! @name Macro is used to bypass both iir_t and iir_p together */
#define BMP5_IIR_BYPASS                           UINT8_C(0xC0)

/*! @name Pressure Out-of-range count limit */
#define BMP5_OOR_COUNT_LIMIT_1                    UINT8_C(0x00)
#define BMP5_OOR_COUNT_LIMIT_3                    UINT8_C(0x01)
#define BMP5_OOR_COUNT_LIMIT_7                    UINT8_C(0x02)
#define BMP5_OOR_COUNT_LIMIT_15                   UINT8_C(0x03)

/*! @name Interrupt configurations */
#define BMP5_INT_MODE_PULSED                      UINT8_C(0)
#define BMP5_INT_MODE_LATCHED                     UINT8_C(1)

#define BMP5_INT_POL_ACTIVE_LOW                   UINT8_C(0)
#define BMP5_INT_POL_ACTIVE_HIGH                  UINT8_C(1)

#define BMP5_INT_OD_PUSHPULL                      UINT8_C(0)
#define BMP5_INT_OD_OPENDRAIN                     UINT8_C(1)

/*! @name NVM and Interrupt status asserted macros */
#define BMP5_INT_ASSERTED_DRDY                    UINT8_C(0x01)
#define BMP5_INT_ASSERTED_FIFO_FULL               UINT8_C(0x02)
#define BMP5_INT_ASSERTED_FIFO_THRES              UINT8_C(0x04)
#define BMP5_INT_ASSERTED_PRESSURE_OOR            UINT8_C(0x08)
#define BMP5_INT_ASSERTED_POR_SOFTRESET_COMPLETE  UINT8_C(0x10)
#define BMP5_INT_NVM_RDY                          UINT8_C(0x02)
#define BMP5_INT_NVM_ERR                          UINT8_C(0x04)
#define BMP5_INT_NVM_CMD_ERR                      UINT8_C(0x08)

/*! @name Interrupt configurations */
#define BMP5_INT_MODE_MSK                         UINT8_C(0x01)

#define BMP5_INT_POL_MSK                          UINT8_C(0x02)
#define BMP5_INT_POL_POS                          UINT8_C(1)

#define BMP5_INT_OD_MSK                           UINT8_C(0x04)
#define BMP5_INT_OD_POS                           UINT8_C(2)

#define BMP5_INT_EN_MSK                           UINT8_C(0x08)
#define BMP5_INT_EN_POS                           UINT8_C(3)

#define BMP5_INT_DRDY_EN_MSK                      UINT8_C(0x01)

#define BMP5_INT_FIFO_FULL_EN_MSK                 UINT8_C(0x02)
#define BMP5_INT_FIFO_FULL_EN_POS                 UINT8_C(1)

#define BMP5_INT_FIFO_THRES_EN_MSK                UINT8_C(0x04)
#define BMP5_INT_FIFO_THRES_EN_POS                UINT8_C(2)

#define BMP5_INT_OOR_PRESS_EN_MSK                 UINT8_C(0x08)
#define BMP5_INT_OOR_PRESS_EN_POS                 UINT8_C(3)

/*! @name ODR configuration */
#define BMP5_ODR_MSK                              UINT8_C(0x7C)
#define BMP5_ODR_POS                              UINT8_C(2)

/*! @name OSR configurations */
#define BMP5_TEMP_OS_MSK                          UINT8_C(0x07)

#define BMP5_PRESS_OS_MSK                         UINT8_C(0x38)
#define BMP5_PRESS_OS_POS                         UINT8_C(3)

/*! @name Pressure enable */
#define BMP5_PRESS_EN_MSK                         UINT8_C(0x40)
#define BMP5_PRESS_EN_POS                         UINT8_C(6)

/*! @name IIR configurations */
#define BMP5_SET_IIR_TEMP_MSK                     UINT8_C(0x07)

#define BMP5_SET_IIR_PRESS_MSK                    UINT8_C(0x38)
#define BMP5_SET_IIR_PRESS_POS                    UINT8_C(3)

#define BMP5_OOR_SEL_IIR_PRESS_MSK                UINT8_C(0x80)
#define BMP5_OOR_SEL_IIR_PRESS_POS                UINT8_C(7)

#define BMP5_SHDW_SET_IIR_TEMP_MSK                UINT8_C(0x08)
#define BMP5_SHDW_SET_IIR_TEMP_POS                UINT8_C(3)

#define BMP5_SHDW_SET_IIR_PRESS_MSK               UINT8_C(0x20)
#define BMP5_SHDW_SET_IIR_PRESS_POS               UINT8_C(5)

#define BMP5_SET_FIFO_IIR_TEMP_MSK                UINT8_C(0x10)
#define BMP5_SET_FIFO_IIR_TEMP_POS                UINT8_C(4)

#define BMP5_SET_FIFO_IIR_PRESS_MSK               UINT8_C(0x40)
#define BMP5_SET_FIFO_IIR_PRESS_POS               UINT8_C(6)

#define BMP5_IIR_FLUSH_FORCED_EN_MSK              UINT8_C(0x04)
#define BMP5_IIR_FLUSH_FORCED_EN_POS              UINT8_C(2)

/*! @name Effective OSR configurations and ODR valid status */
#define BMP5_OSR_TEMP_EFF_MSK                     UINT8_C(0x07)

#define BMP5_OSR_PRESS_EFF_MSK                    UINT8_C(0x38)
#define BMP5_OSR_PRESS_EFF_POS                    UINT8_C(3)

#define BMP5_ODR_IS_VALID_MSK                     UINT8_C(0x80)
#define BMP5_ODR_IS_VALID_POS                     UINT8_C(7)

/*! @name Powermode */
#define BMP5_POWERMODE_MSK                        UINT8_C(0x03)

#define BMP5_DEEP_DISABLE_MSK                     UINT8_C(0x80)
#define BMP5_DEEP_DISABLE_POS                     UINT8_C(7)

/*! @name Fifo configurations */
#define BMP5_FIFO_THRESHOLD_MSK                   UINT8_C(0x1F)

#define BMP5_FIFO_MODE_MSK                        UINT8_C(0x20)
#define BMP5_FIFO_MODE_POS                        UINT8_C(5)

#define BMP5_FIFO_DEC_SEL_MSK                     UINT8_C(0x1C)
#define BMP5_FIFO_DEC_SEL_POS                     UINT8_C(2)

#define BMP5_FIFO_COUNT_MSK                       UINT8_C(0x3F)

#define BMP5_FIFO_FRAME_SEL_MSK                   UINT8_C(0x03)

/*! @name Out-of-range configuration */
#define BMP5_OOR_THR_P_LSB_MSK                    UINT32_C(0x0000FF)

#define BMP5_OOR_THR_P_MSB_MSK                    UINT32_C(0x00FF00)

#define BMP5_OOR_THR_P_XMSB_MSK                   UINT32_C(0x010000)
#define BMP5_OOR_THR_P_XMSB_POS                   UINT16_C(16)

/* Macro to mask xmsb value of oor threshold from register(0x35) value */
#define BMP5_OOR_THR_P_XMSB_REG_MSK               UINT8_C(0x01)

#define BMP5_OOR_COUNT_LIMIT_MSK                  UINT8_C(0xC0)
#define BMP5_OOR_COUNT_LIMIT_POS                  UINT8_C(6)

/*! @name NVM configuration */
#define BMP5_NVM_ADDR_MSK                         UINT8_C(0x3F)

#define BMP5_NVM_PROG_EN_MSK                      UINT8_C(0x40)
#define BMP5_NVM_PROG_EN_POS                      UINT8_C(6)

#define BMP5_NVM_DATA_LSB_MSK                     UINT16_C(0x00FF)

#define BMP5_NVM_DATA_MSB_MSK                     UINT16_C(0xFF00)

/******************************************************************************/
/*!  @name         TYPEDEF DEFINITIONS                                        */
/******************************************************************************/

/*!
 * @brief Bus communication function pointer which should be mapped to
 * the platform specific read functions of the user
 *
 * @param[in] reg_addr       : Register address from which data is read.
 * @param[out] read_data     : Pointer to data buffer where read data is stored.
 * @param[in] len            : Number of bytes of data to be read.
 * @param[in, out] intf_ptr  : Void pointer that can enable the linking of descriptors
 *                                  for interface related call backs.
 *
 * @retval 0 for Success
 * @retval Non-zero for Failure
 */
typedef BMP5_INTF_RET_TYPE (*bmp5_read_fptr_t)(uint8_t reg_addr, uint8_t *read_data, uint32_t len, void *intf_ptr);

/*!
 * @brief Bus communication function pointer which should be mapped to
 * the platform specific write functions of the user
 *
 * @param[in] reg_addr      : Register address to which the data is written.
 * @param[in] read_data     : Pointer to data buffer in which data to be written
 *                            is stored.
 * @param[in] len           : Number of bytes of data to be written.
 * @param[in, out] intf_ptr : Void pointer that can enable the linking of descriptors
 *                            for interface related call backs
 *
 * @retval 0 for Success
 * @retval Non-zero for Failure
 */
typedef BMP5_INTF_RET_TYPE (*bmp5_write_fptr_t)(uint8_t reg_addr, const uint8_t *read_data, uint32_t len,
                                                void *intf_ptr);

/*!
 * @brief Delay function pointer which should be mapped to
 * delay function of the user
 *
 * @param[in] period              : Delay in microseconds.
 * @param[in, out] intf_ptr       : Void pointer that can enable the linking of descriptors
 *                                  for interface related call backs
 *
 */
typedef void (*bmp5_delay_us_fptr_t)(uint32_t period, void *intf_ptr);

/******************************************************************************/
/*!  @name         ENUMERATION DEFINITIONS                                      */
/******************************************************************************/

/*!
 * @brief Enumerator for interface selection
 */
enum bmp5_intf {
    /*! SPI interface */
    BMP5_SPI_INTF,
    /*! I2C interface */
    BMP5_I2C_INTF,
    /*! I3C interface */
    BMP5_I3C_INTF
};

/*!
 * @brief Enumerator for powermode selection
 */
enum bmp5_powermode {
    /*! Standby powermode */
    BMP5_POWERMODE_STANDBY,
    /*! Normal powermode */
    BMP5_POWERMODE_NORMAL,
    /*! Forced powermode */
    BMP5_POWERMODE_FORCED,
    /*! Continous powermode */
    BMP5_POWERMODE_CONTINOUS,
    /*! Deep standby powermode */
    BMP5_POWERMODE_DEEP_STANDBY
};

/*!
 * @brief Enumerator for interrupt enable disable
 */
enum bmp5_intr_en_dis {
    /*! Interrupt diable */
    BMP5_INTR_DISABLE = BMP5_DISABLE,
    /*! Interrupt enable */
    BMP5_INTR_ENABLE = BMP5_ENABLE
};

/*!
 * @brief Enumerator for interrupt mode
 */
enum bmp5_intr_mode {
    /*! Interrupt mode - pulsed */
    BMP5_PULSED = BMP5_INT_MODE_PULSED,
    /*! Interrupt mode - latched */
    BMP5_LATCHED = BMP5_INT_MODE_LATCHED
};

/*!
 * @brief Enumerator for interrupt polarity
 */
enum bmp5_intr_polarity {
    /*! Interrupt polarity - active low */
    BMP5_ACTIVE_LOW = BMP5_INT_POL_ACTIVE_LOW,
    /*! Interrupt polarity - active high */
    BMP5_ACTIVE_HIGH = BMP5_INT_POL_ACTIVE_HIGH
};

/*!
 * @brief Enumerator for interrupt drive
 */
enum bmp5_intr_drive {
    /*! Interrupt drive - push-pull */
    BMP5_INTR_PUSH_PULL = BMP5_INT_OD_PUSHPULL,
    /*! Interrupt drive - open drain */
    BMP5_INTR_OPEN_DRAIN = BMP5_INT_OD_OPENDRAIN
};

/*!
 * @brief Enumerator for fifo frame selection
 */
enum bmp5_fifo_frame_sel {
    /*! Fifo disabled */
    BMP5_FIFO_NOT_ENABLED,
    /*! Fifo temperature data only enabled */
    BMP5_FIFO_TEMPERATURE_DATA,
    /*! Fifo pressure data only enabled */
    BMP5_FIFO_PRESSURE_DATA,
    /*! Fifo pressure and temperature data enabled */
    BMP5_FIFO_PRESS_TEMP_DATA
};

/*!
 * @brief Enumerator for fifo decimation factor(downsampling) selection
 */
enum bmp5_fifo_dec_sel {
    /*! Fifo downsampling disabled */
    BMP5_FIFO_NO_DOWNSAMPLING,
    /*! Fifo 2X downsampling */
    BMP5_FIFO_DOWNSAMPLING_2X,
    /*! Fifo 4X downsampling */
    BMP5_FIFO_DOWNSAMPLING_4X,
    /*! Fifo 8X downsampling */
    BMP5_FIFO_DOWNSAMPLING_8X,
    /*! Fifo 16X downsampling */
    BMP5_FIFO_DOWNSAMPLING_16X,
    /*! Fifo 32X downsampling */
    BMP5_FIFO_DOWNSAMPLING_32X,
    /*! Fifo 64X downsampling */
    BMP5_FIFO_DOWNSAMPLING_64X,
    /*! Fifo 128X downsampling */
    BMP5_FIFO_DOWNSAMPLING_128X
};

/*!
 * @brief Enumerator for fifo mode selection
 */
enum bmp5_fifo_mode {
    /*! Fifo mode - streaming */
    BMP5_FIFO_MODE_STREAMING,
    /*! Fifo mode - stop on full */
    BMP5_FIFO_MODE_STOP_ON_FULL
};

/******************************************************************************/
/*!  @name         STRUCTURE DEFINITIONS                                      */
/******************************************************************************/

/*!
 * @brief OSR, ODR and pressure configuration structure
 */
struct bmp5_osr_odr_press_config
{
    /*! Temperature oversampling
     * Assignable macros :
     * - BMP5_OVERSAMPLING_1X
     * - BMP5_OVERSAMPLING_2X
     * - BMP5_OVERSAMPLING_4X
     * - BMP5_OVERSAMPLING_8X
     * - BMP5_OVERSAMPLING_16X
     * - BMP5_OVERSAMPLING_32X
     * - BMP5_OVERSAMPLING_64X
     * - BMP5_OVERSAMPLING_128X
     */
    uint8_t osr_t;

    /*! Pressure oversampling
     * Assignable macros :
     * - BMP5_OVERSAMPLING_1X
     * - BMP5_OVERSAMPLING_2X
     * - BMP5_OVERSAMPLING_4X
     * - BMP5_OVERSAMPLING_8X
     * - BMP5_OVERSAMPLING_16X
     * - BMP5_OVERSAMPLING_32X
     * - BMP5_OVERSAMPLING_64X
     * - BMP5_OVERSAMPLING_128X
     */
    uint8_t osr_p;

    /*! Enable pressure
     * BMP5_ENABLE  = Enables pressure data
     * BMP5_DISABLE = Disables pressure data
     */
    uint8_t press_en;

    /*! Output Data Rate */
    uint8_t odr;
};

/*!
 * @brief IIR configuration structure
 */
struct bmp5_iir_config
{
    /*! Temperature IIR
     * Assignable macros :
     * - BMP5_IIR_FILTER_BYPASS
     * - BMP5_IIR_FILTER_0_20980
     * - BMP5_IIR_FILTER_0_08045
     * - BMP5_IIR_FILTER_0_03695
     * - BMP5_IIR_FILTER_0_01785
     * - BMP5_IIR_FILTER_0_00880
     * - BMP5_IIR_FILTER_0_00435
     * - BMP5_IIR_FILTER_0_00220
     */
    uint8_t set_iir_t;

    /*! Pressure IIR
     * Assignable macros :
     * - BMP5_IIR_FILTER_BYPASS
     * - BMP5_IIR_FILTER_0_20980
     * - BMP5_IIR_FILTER_0_08045
     * - BMP5_IIR_FILTER_0_03695
     * - BMP5_IIR_FILTER_0_01785
     * - BMP5_IIR_FILTER_0_00880
     * - BMP5_IIR_FILTER_0_00435
     * - BMP5_IIR_FILTER_0_00220
     */
    uint8_t set_iir_p;

    /*! Shadow IIR selection for temperature
     * Assignable macros :
     * - BMP5_ENABLE
     * - BMP5_DISABLE
     */
    uint8_t shdw_set_iir_t;

    /*! Shadow IIR selection for pressure
     * Assignable macros :
     * - BMP5_ENABLE
     * - BMP5_DISABLE
     */
    uint8_t shdw_set_iir_p;

    /*! IIR flush in forced mode enable
     * Assignable macros :
     * - BMP5_ENABLE
     * - BMP5_DISABLE
     */
    uint8_t iir_flush_forced_en;
};

/*!
 * @brief Effective OSR configuration and ODR valid status structure
 */
struct bmp5_osr_odr_eff
{
    /*! Effective temperature OSR */
    uint8_t osr_t_eff;

    /*! Effective pressure OSR */
    uint8_t osr_p_eff;

    /*! If asserted, the ODR parametrization is valid */
    uint8_t odr_is_valid;
};

/*!
 * @brief BMP5 interrupt source selection.
 */
struct bmp5_int_source_select
{
    /*! Data ready interrupt enable
     * BMP5_ENABLE  = Enables data ready interrupt
     * BMP5_DISABLE = Disables data ready interrupt
     */
    uint8_t drdy_en;

    /*! Fifo full interrupt enable
     * BMP5_ENABLE  = Enables fifo full interrupt
     * BMP5_DISABLE = Disables fifo full interrupt
     */
    uint8_t fifo_full_en;

    /*! Fifo threshold interrupt enable
     * BMP5_ENABLE  = Enables fifo threshold interrupt
     * BMP5_DISABLE = Disables fifo threshold interrupt
     */
    uint8_t fifo_thres_en;

    /*! Pressure out of range interrupt enable
     * BMP5_ENABLE  = Enables pressure out of range interrupt
     * BMP5_DISABLE = Disables pressure out of range interrupt
     */
    uint8_t oor_press_en;
};

/*!
 * @brief BMP5 fifo configurations.
 */
struct bmp5_fifo
{
    /*! Pointer to fifo data */
    uint8_t *data;

    /*! Length of user defined bytes of fifo to be read */
    uint16_t length;

    /*! Fifo frame data source selection
     * Assignable macros :
     * - BMP5_FIFO_NOT_ENABLED
     * - BMP5_FIFO_TEMPERATURE_DATA
     * - BMP5_FIFO_PRESSURE_DATA
     * - BMP5_FIFO_PRESS_TEMP_DATA
     */
    uint8_t frame_sel;

    /*! Fifo decimation factor(downsampling) selection
     * Assignable macros :
     * - BMP5_FIFO_NO_DOWNSAMPLING
     * - BMP5_FIFO_DOWNSAMPLING_2X
     * - BMP5_FIFO_DOWNSAMPLING_4X
     * - BMP5_FIFO_DOWNSAMPLING_8X
     * - BMP5_FIFO_DOWNSAMPLING_16X
     * - BMP5_FIFO_DOWNSAMPLING_32X
     * - BMP5_FIFO_DOWNSAMPLING_64X
     * - BMP5_FIFO_DOWNSAMPLING_128X
     */
    uint8_t dec_sel;

    /*! Fifo frame count */
    uint8_t fifo_count;

    /*! Fifo mode selection
     * Assignable macros :
     * - BMP5_FIFO_MODE_STREAMING
     * - BMP5_FIFO_MODE_STOP_ON_FULL
     */
    uint8_t mode;

    /*! Threshold for fifo */
    uint8_t threshold;

    /*! Fifo temperature IIR
     * Assignable macros :
     * - BMP5_ENABLE
     * - BMP5_DISABLE
     */
    uint8_t set_fifo_iir_t;

    /*! Fifo pressure IIR
     * Assignable macros :
     * - BMP5_ENABLE
     * - BMP5_DISABLE
     */
    uint8_t set_fifo_iir_p;
};

/*!
 * @brief BMP5 Out-of-range pressure configuration.
 */
struct bmp5_oor_press_configuration
{
    /*! Out-of-range pressure threshold */
    uint32_t oor_thr_p;

    /*! Out-of-range pressure range
     *  Range can span up to +/- 255 Pa around the threshold value.
     */
    uint8_t oor_range_p;

    /*! Out-of-range pressure count limit
     * Assignable macros :
     * - BMP5_OOR_COUNT_LIMIT_1
     * - BMP5_OOR_COUNT_LIMIT_3
     * - BMP5_OOR_COUNT_LIMIT_7
     * - BMP5_OOR_COUNT_LIMIT_15
     */
    uint8_t cnt_lim;

    /*! Out-of-range pressure IIR
     * Assignable macros :
     * - BMP5_ENABLE
     * - BMP5_DISABLE
     */
    uint8_t oor_sel_iir_p;
};

/*!
 * @brief BMP5 sensor data structure which comprises of temperature and pressure.
 */

#ifdef BMP5_USE_FIXED_POINT

/*!
 * @brief BMP5 sensor data structure which comprises of temperature and pressure in fixed point with data type as
 *  uint64_t for pressure and int64_t for temperature.
 */
struct bmp5_sensor_data
{
    /*! Pressure data */
    uint64_t pressure;

    /*! Temperature data */
    int64_t temperature;
};

#else

/*!
 * @brief BMP5 sensor data structure which comprises of temperature and pressure in floating point with data type as
 *  float for pressure and temperature.
 */
struct bmp5_sensor_data
{
    /*! Pressure data */
    float pressure;

    /*! Temperature data */
    float temperature;
};

#endif

/*!
 * @brief API device structure
 */
struct bmp5_dev
{
    /*! Chip ID */
    uint8_t chip_id;

    /*!
     * The interface pointer is used to enable the user
     * to link their interface descriptors for reference during the
     * implementation of the read and write interfaces to the
     * hardware.
     */
    void *intf_ptr;

    /*! Read function pointer */
    bmp5_read_fptr_t read;

    /*! Write function pointer */
    bmp5_write_fptr_t write;

    /*! Delay function pointer */
    bmp5_delay_us_fptr_t delay_us;

    /*! To store interface pointer error */
    BMP5_INTF_RET_TYPE intf_rslt;

    /*! Type of Interface  */
    enum bmp5_intf intf;
};

#endif /* End of _BMP5_DEFS_H */