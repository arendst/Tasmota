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
* @file       bmp5.h
* @date       2022-08-11
* @version    v1.1.1
*
*/

/*!
 * @defgroup bmp5 BMP5
 * @brief Sensor driver for BMP5 sensor
 */

 #ifndef BMP5_H_
 #define BMP5_H_
 
 /*! CPP guard */
 #ifdef __cplusplus
 extern "C" {
 #endif
 
 /***************************************************************************/
 
 /*!             Header files
  ****************************************************************************/
 #include "bmp5_defs.h"
 
 /***************************************************************************/
 
 /*!                 BMP5 User Interface function prototypes
  ****************************************************************************/
 
 /**
  * \ingroup bmp5
  * \defgroup bmp5ApiInit Initialization
  * @brief Initialize the sensor and device structure
  */
 
 /*!
  * \ingroup bmp5ApiInit
  * \page bmp5_api_bmp5_init bmp5_init
  * \code
  * int8_t bmp5_init(struct bmp5_dev *dev);
  * \endcode
  * @details This API is the entry point. Call this API before using all other APIs.
  * This API reads the chip-id of the sensor and sets the resolution, feature
  * length and the type of variant.
  *
  * @param[in,out] dev : Structure instance of bmp5_dev.
  *
  * @return Result of API execution status.
  * @return 0 -> Success
  * @return < 0 -> Fail
  *
  */
 int8_t bmp5_init(struct bmp5_dev *dev);
 
 /**
  * \ingroup bmp5
  * \defgroup bmp5ApiRegister Registers
  * @brief Generic API for accessing sensor registers
  */
 
 /*!
  * \ingroup bmp5ApiRegister
  * \page bmp5_api_bmp5_get_regs bmp5_get_regs
  * \code
  * int8_t bmp5_get_regs(uint8_t reg_addr, uint8_t *data, uint32_t len, struct bmp5_dev *dev);
  * \endcode
  * @details This API reads the data from the given register address of sensor.
  *
  * @param[in] reg_addr  : Register address from where the data to be read.
  * @param[out] data     : Pointer to data buffer to store the read data.
  * @param[in] len       : No of bytes of data to be read.
  * @param[in,out] dev   : Structure instance of bmp5_dev.
  *
  * @return Result of API execution status
  * @return 0 -> Success
  * @return < 0 -> Fail
  */
 int8_t bmp5_get_regs(uint8_t reg_addr, uint8_t *data, uint32_t len, struct bmp5_dev *dev);
 
 /*!
  * \ingroup bmp5ApiRegister
  * \page bmp5_api_bmp5_set_regs bmp5_set_regs
  * \code
  * int8_t bmp5_set_regs(uint8_t reg_addr, const uint8_t *data, uint32_t len, struct bmp5_dev *dev);
  * \endcode
  * @details This API writes the given data to the register address of sensor.
  *
  * @param[in] reg_addr  : Register address where the data is to be written.
  * @param[in] data      : Pointer to data buffer, whose data is to be written
  *                        in the sensor.
  * @param[in] len       : No of bytes of data to be writen.
  * @param[in,out] dev   : Structure instance of bmp5_dev.
  *
  * @return Result of API execution status
  * @return 0 -> Success
  * @return < 0 -> Fail
  */
 int8_t bmp5_set_regs(uint8_t reg_addr, const uint8_t *data, uint32_t len, struct bmp5_dev *dev);
 
 /**
  * \ingroup bmp5
  * \defgroup bmp5ApiSystem System
  * @brief API that performs system-level operations
  */
 
 /*!
  * \ingroup bmp5ApiSystem
  * \page bmp5_api_bmp5_soft_reset bmp5_soft_reset
  * \code
  * int8_t bmp5_soft_reset(struct bmp5_dev *dev);
  * \endcode
  * @details This API is used to soft-reset the sensor where all the registers are reset to their default values.
  *
  * @note If this register is set using I2C, an ACK will NOT be transmitted to
  * the host.
  *
  * @param[in] dev       : Structure instance of bmp5_dev.
  *
  * @return Result of API execution status.
  * @return 0 -> Success
  * @return < 0 -> Fail
  */
 int8_t bmp5_soft_reset(struct bmp5_dev *dev);
 
 /**
  * \ingroup bmp5
  * \defgroup bmp5ApiStatus NVM and Interrupt status
  * @brief NVM and Interrupt status API
  */
 
 /*!
  * \ingroup bmp5ApiStatus
  * \page bmp5_api_bmp5_get_interrupt_status bmp5_get_interrupt_status
  * \code
  * int8_t bmp5_get_interrupt_status(uint8_t *int_status, struct bmp5_dev *dev);
  * \endcode
  * @details This API is used to get the interrupt status (data ready interrupt, fifo full interrupt,
  * fifo threshold interrupt, pressure out of range interrupt and power-on reset/software reset complete interrupt).
  *
  * @param[out] int_status   : Variable to store interrupt status.
  * @param[in] dev           : Structure instance of bmp5_dev.
  *
  * @note : Below are values of int_status possible.
  *
  *@verbatim
  * Macros                                   |  Functionality
  * -----------------------------------------|----------------------------
  * BMP5_INT_ASSERTED_DRDY                   |  Data ready interrupt asserted
  * BMP5_INT_ASSERTED_FIFO_FULL              |  Fifo full interrupt asserted
  * BMP5_INT_ASSERTED_FIFO_THRES             |  Fifo threshold interrupt asserted
  * BMP5_INT_ASSERTED_PRESSURE_OOR           |  Pressure out of range interrupt asserted
  * BMP5_INT_ASSERTED_POR_SOFTRESET_COMPLETE |  Power-on reset/Software reset complete interrupt asserted
  *@endverbatim
  *
  * @return Result of API execution status.
  * @return 0 -> Success
  * @return < 0 -> Fail
  */
 int8_t bmp5_get_interrupt_status(uint8_t *int_status, struct bmp5_dev *dev);
 
 /**
  * \ingroup bmp5
  * \defgroup bmp5ApiPowermode Powermode
  * @brief Powermode of the sensor API
  */
 
 /*!
  * \ingroup bmp5ApiPowermode
  * \page bmp5_api_bmp5_set_power_mode bmp5_set_power_mode
  * \code
  * int8_t bmp5_set_power_mode(enum bmp5_powermode powermode, struct bmp5_dev *dev);
  * \endcode
  * @details This API sets the power mode of the sensor.
  *
  * @param[in] powermode : Select powermode.
  * @param[in] dev       : Structure instance of bmp5_dev.
  *
  * @note Below mentioned are the possible powermode that can be set by the user.
  *
  *@verbatim
  * powermode        |   Macros
  * -----------------|-------------------
  *     0            | BMP5_POWERMODE_STANBY
  *     1            | BMP5_POWERMODE_NORMAL
  *     2            | BMP5_POWERMODE_FORCED
  *     3            | BMP5_POWERMODE_CONTINOUS
  *     4            | BMP5_POWERMODE_DEEP_STANBY
  *@endverbatim
  *
  * @return Result of API execution status.
  * @return 0 -> Success
  * @return < 0 -> Fail
  */
 int8_t bmp5_set_power_mode(enum bmp5_powermode powermode, struct bmp5_dev *dev);
 
 /*!
  * \ingroup bmp5ApiPowermode
  * \page bmp5_api_bmp5_get_power_mode bmp5_get_power_mode
  * \code
  * int8_t bmp5_get_power_mode(enum bmp5_powermode *powermode, struct bmp5_dev *dev);
  * \endcode
  * @details This API gets the power mode of the sensor.
  *
  * @param[out] powermode : To store the power mode.
  * @param[in] dev        : Structure instance of bmp5_dev.
  *
  * @note Below mentioned are the possible powermode from the sensor
  *
  *@verbatim
  * powermode        |   Macros
  * -----------------|-------------------
  *     0            | BMP5_POWERMODE_STANBY
  *     1            | BMP5_POWERMODE_NORMAL
  *     2            | BMP5_POWERMODE_FORCED
  *     3            | BMP5_POWERMODE_CONTINOUS
  *     4            | BMP5_POWERMODE_DEEP_STANBY
  *@endverbatim
  *
  * @return Result of API execution status.
  * @return 0 -> Success
  * @return < 0 -> Fail
  */
 int8_t bmp5_get_power_mode(enum bmp5_powermode *powermode, struct bmp5_dev *dev);
 
 /**
  * \ingroup bmp5
  * \defgroup bmp5ApiData Sensor Data
  * @brief Get Sensor data
  */
 
 /*!
  * \ingroup bmp5ApiData
  * \page bmp5_api_bmp5_get_sensor_data bmp5_get_sensor_data
  * \code
  * int8_t bmp5_get_sensor_data(struct bmp5_sensor_data *sensor_data,
  *                             const struct bmp5_osr_odr_press_config *osr_odr_press_cfg, struct bmp5_dev *dev);
  * \endcode
  * @details This API reads the temperature(deg C) or both pressure(Pa) and temperature(deg C) data from the
  * sensor and store it in the bmp5_sensor_data structure instance passed by the user.
  *
  * @param[out] sensor_data       : Structure instance of bmp5_sensor_data.
  * @param[in] osr_odr_press_cfg  : Structure instance of bmp5_osr_odr_press_config.
  * @param[in] dev                : Structure instance of bmp5_dev.
  *
  * @note If fixed point(BMP5_USE_FIXED_POINT) is selected then data return type is uin64_t for pressure
  *  and int64_t for temperature. While for floating point(BMP5_USE_FLOATING_POINT) data return type is float for
  *  pressure and temperature.
  *
  * @return Result of API execution status.
  * @return 0 -> Success
  * @return < 0 -> Fail
  */
 int8_t bmp5_get_sensor_data(struct bmp5_sensor_data *sensor_data,
                             const struct bmp5_osr_odr_press_config *osr_odr_press_cfg,
                             struct bmp5_dev *dev);
 
 /**
  * \ingroup bmp5
  * \defgroup bmp5ApiInterrupt Interrupt
  * @brief Interrupt API
  */
 
 /*!
  * \ingroup bmp5ApiInterrupt
  * \page bmp5_api_bmp5_int_source_select bmp5_int_source_select
  * \code
  * int8_t bmp5_int_source_select(const struct bmp5_int_source_select *int_source_select, struct bmp5_dev *dev);
  * \endcode
  * @details This API is used to enable the interrupts(drdy interrupt, fifo full interrupt,
  *   fifo threshold enable and pressure data out of range interrupt).
  *
  * @param[in] int_source_select : Structure instance of bmp5_int_enable.
  * @param[in] dev               : Structure instance of bmp5_dev.
  *
  * @return Result of API execution status.
  * @return 0 -> Success
  * @return < 0 -> Fail
  */
 int8_t bmp5_int_source_select(const struct bmp5_int_source_select *int_source_select, struct bmp5_dev *dev);
 
 /*!
  * \ingroup bmp5ApiInterrupt
  * \page bmp5_api_bmp5_configure_interrupt bmp5_configure_interrupt
  * \code
  * int8_t bmp5_configure_interrupt(enum bmp5_intr_mode int_mode,
  *                                 enum bmp5_intr_polarity int_pol,
  *                                 enum bmp5_intr_drive int_od,
  *                                 enum bmp5_intr_en_dis int_en,
  *                                 struct bmp5_dev *dev);
  * \endcode
  * @details This API is used to configure the interrupt settings.
  *
  * @param[in] int_mode          : Sets either latched or pulsed.
  * @param[in] int_pol           : Sets either polarity high or low.
  * @param[in] int_od            : Sets either open drain or push pull.
  * @param[in] int_en            : Enable/ Disable interrupt pin.
  * @param[in] dev               : Structure instance of bmp5_dev.
  *
  * @return Result of API execution status.
  * @return 0 -> Success
  * @return < 0 -> Fail
  */
 int8_t bmp5_configure_interrupt(enum bmp5_intr_mode int_mode,
                                 enum bmp5_intr_polarity int_pol,
                                 enum bmp5_intr_drive int_od,
                                 enum bmp5_intr_en_dis int_en,
                                 struct bmp5_dev *dev);
 
 /**
  * \ingroup bmp5
  * \defgroup bmp5ApiSettings Settings
  * @brief Get/set sensor settings APIs
  */
 
 /*!
  * \ingroup bmp5ApiSettings
  * \page bmp5_api_bmp5_get_osr_odr_press_config bmp5_get_osr_odr_press_config
  * \code
  * int8_t bmp5_get_osr_odr_press_config(struct bmp5_osr_odr_press_config *osr_odr_press_cfg, struct bmp5_dev *dev);
  * \endcode
  * @details This API gets the configuration for oversampling of temperature, oversampling of
  * pressure and ODR configuration for normal mode along with pressure enable.
  *
  * @param[out] osr_odr_press_cfg : Structure instance of bmp5_osr_odr_press_config.
  * @param[in] dev                : Structure instance of bmp5_dev.
  *
  * @return Result of API execution status.
  * @return 0 -> Success
  * @return < 0 -> Fail
  */
 int8_t bmp5_get_osr_odr_press_config(struct bmp5_osr_odr_press_config *osr_odr_press_cfg, struct bmp5_dev *dev);
 
 /*!
  * \ingroup bmp5ApiSettings
  * \page bmp5_api_bmp5_set_osr_odr_press_config bmp5_set_osr_odr_press_config
  * \code
  * int8_t bmp5_set_osr_odr_press_config(const struct bmp5_osr_odr_press_config *osr_odr_press_cfg, struct bmp5_dev *dev);
  * \endcode
  * @details This API sets the configuration for oversampling of temperature, oversampling of
  * pressure and ODR configuration along with pressure enable.
  *
  * @param[in] osr_odr_press_cfg  : Structure instance of bmp5_osr_odr_press_config.
  * @param[in] dev                 : Structure instance of bmp5_dev.
  *
  * @note If ODR is set to a value higher than 5Hz then powermode is set as standby mode, as ODR value greater than 5HZ
  * without disabling deep-standby mode makes powermode invalid.
  *
  * @return Result of API execution status.
  * @return 0 -> Success
  * @return < 0 -> Fail
  */
 int8_t bmp5_set_osr_odr_press_config(const struct bmp5_osr_odr_press_config *osr_odr_press_cfg, struct bmp5_dev *dev);
 
 /*!
  * \ingroup bmp5ApiSettings
  * \page bmp5_api_bmp5_get_iir_config bmp5_get_iir_config
  * \code
  * int8_t bmp5_get_iir_config(struct bmp5_iir_config *iir_cfg, struct bmp5_dev *dev);
  * \endcode
  * @details This API gets the configuration for IIR of temperature and pressure.
  *
  * @param[out] iir_cfg  : Structure instance of bmp5_iir_config.
  * @param[in] dev       : Structure instance of bmp5_dev.
  *
  * @return Result of API execution status.
  * @return 0 -> Success
  * @return < 0 -> Fail
  */
 int8_t bmp5_get_iir_config(struct bmp5_iir_config *iir_cfg, struct bmp5_dev *dev);
 
 /*!
  * \ingroup bmp5ApiSettings
  * \page bmp5_api_bmp5_set_iir_config bmp5_set_iir_config
  * \code
  * int8_t bmp5_set_iir_config(const struct bmp5_iir_config *iir_cfg, struct bmp5_dev *dev);
  * \endcode
  * @details This API sets the configuration for IIR of temperature and pressure.
  *
  * @note
  * 1. The IIR configuration can be performed only in standby mode. So in this API before updating IIR
  * configuration powermode is switched to standby mode and reverted back to earlier powermode after
  * IIR configuration is updated.
  * 2. If IIR value for both temperature and pressure is set a value other than bypass then powermode is set
  * as standby mode, as IIR with value other than bypass without disabling deep-standby mode makes powermode invalid.
  *
  * @param[in] iir_cfg  : Structure instance of bmp5_iir_config.
  * @param[in] dev       : Structure instance of bmp5_dev.
  *
  * @return Result of API execution status.
  * @return 0 -> Success
  * @return < 0 -> Fail
  */
 int8_t bmp5_set_iir_config(const struct bmp5_iir_config *iir_cfg, struct bmp5_dev *dev);
 
 /*!
  * \ingroup bmp5ApiSettings
  * \page bmp5_api_bmp5_get_osr_odr_eff bmp5_get_osr_odr_eff
  * \code
  * int8_t bmp5_get_osr_odr_eff(struct bmp5_osr_odr_eff *osr_odr_eff, struct bmp5_dev *dev);
  * \endcode
  * @details This API gets the configuration for effective OSR of temperature,
  * effective OSR of pressure and ODR valid status.
  *
  * @param[out] osr_odr_eff  : Structure instance of bmp5_osr_odr_eff.
  * @param[in] dev           : Structure instance of bmp5_dev.
  *
  * @return Result of API execution status.
  * @return 0 -> Success
  * @return < 0 -> Fail
  */
 int8_t bmp5_get_osr_odr_eff(struct bmp5_osr_odr_eff *osr_odr_eff, struct bmp5_dev *dev);
 
 /**
  * \ingroup bmp5
  * \defgroup bmp5ApiFIFO FIFO
  * @brief FIFO operations of the sensor
  */
 
 /*!
  * \ingroup bmp5ApiFIFO
  * \page bmp5_api_bmp5_get_fifo_configuration bmp5_get_fifo_configuration
  * \code
  * int8_t bmp5_get_fifo_configuration(struct bmp5_fifo *fifo, struct bmp5_dev *dev);
  * \endcode
  * @details This API used to get the configurations of fifo from the sensor.
  *
  * @param[out] fifo : Structure instance of bmp5_fifo.
  * @param[in] dev   : Structure instance of bmp5_dev.
  *
  * @return Result of API execution status.
  * @return 0 -> Success
  * @return < 0 -> Fail
  */
 int8_t bmp5_get_fifo_configuration(struct bmp5_fifo *fifo, struct bmp5_dev *dev);
 
 /*!
  * \ingroup bmp5ApiFIFO
  * \page bmp5_api_bmp5_set_fifo_configuration bmp5_set_fifo_configuration
  * \code
  * int8_t bmp5_set_fifo_configuration(const struct bmp5_fifo *fifo, struct bmp5_dev *dev);
  * \endcode
  * @details This API used to set the configurations of fifo in the sensor.
  *
  * @param[in] fifo : Structure instance of bmp5_fifo.
  * @param[in] dev  : Structure instance of bmp5_dev.
  *
  * @note If Fifo frame selection is enabled then powermode is set as standby mode, as fifo frame selection
  * enabled without disabling deep-standby mode makes powermode invalid.
  *
  * @return Result of API execution status.
  * @return 0 -> Success
  * @return < 0 -> Fail
  */
 int8_t bmp5_set_fifo_configuration(const struct bmp5_fifo *fifo, struct bmp5_dev *dev);
 
 /*!
  * \ingroup bmp5ApiFIFO
  * \page bmp5_api_bmp5_get_fifo_len bmp5_get_fifo_len
  * \code
  * int8_t bmp5_get_fifo_len(uint8_t *fifo_len, struct bmp5_fifo *fifo, struct bmp5_dev *dev);
  * \endcode
  * @details This API is used to get the length of fifo from the sensor.
  *
  * @param[out] fifo_len : Variable to store fifo length.
  * @param[out] fifo     : Structure instance of bmp5_fifo.
  * @param[in] dev       : Structure instance of bmp5_dev.
  *
  * @return Result of API execution status.
  * @return 0 -> Success
  * @return < 0 -> Fail
  */
 int8_t bmp5_get_fifo_len(uint16_t *fifo_len, struct bmp5_fifo *fifo, struct bmp5_dev *dev);
 
 /*!
  * \ingroup bmp5ApiFIFO
  * \page bmp5_api_bmp5_get_fifo_data bmp5_get_fifo_data
  * \code
  * int8_t bmp5_get_fifo_data(struct bmp5_fifo *fifo, struct bmp5_dev *dev);
  * \endcode
  * @details This API is used to get the fifo data from the sensor.
  *
  * @param[out] fifo       : Structure instance of bmp5_fifo.
  * @param[in] dev         : Structure instance of bmp5_dev.
  *
  * @return Result of API execution status.
  * @return 0 -> Success
  * @return < 0 -> Fail
  */
 int8_t bmp5_get_fifo_data(struct bmp5_fifo *fifo, struct bmp5_dev *dev);
 
 /*!
  * \ingroup bmp5ApiFIFO
  * \page bmp5_api_bmp5_extract_fifo_data bmp5_extract_fifo_data
  * \code
  * int8_t bmp5_extract_fifo_data(const struct bmp5_fifo *fifo, struct bmp5_sensor_data *sensor_data);
  * \endcode
  * @details This API extract the temperature and/or pressure data from the fifo data which is
  * already read from the fifo.
  *
  * @param[in] fifo         : Structure instance of bmp5_fifo.
  * @param[out] sensor_data : Structure instance of bmp5_sensor_data.
  * @param[in] dev          : Structure instance of bmp5_dev.
  *
  * @return Result of API execution status.
  * @return 0 -> Success
  * @return < 0 -> Fail
  */
 int8_t bmp5_extract_fifo_data(const struct bmp5_fifo *fifo, struct bmp5_sensor_data *sensor_data);
 
 /**
  * \ingroup bmp5
  * \defgroup bmp5ApiOOR OOR pressure
  * @brief OOR pressure API
  */
 
 /*!
  * \ingroup bmp5ApiOOR
  * \page bmp5_api_bmp5_get_oor_configuration bmp5_get_oor_configuration
  * \code
  * int8_t bmp5_get_oor_configuration(struct bmp5_oor_press_configuration *oor_press_config, struct bmp5_dev *dev);
  * \endcode
  * @details This API gets the configuration for out-of-range pressure threshold, range
  *  count limit and IIR.
  *
  * @param[out] oor_press_config : Structure instance of bmp5_oor_press_configuration.
  * @param[in] dev               : Structure instance of bmp5_dev.
  *
  * @return Result of API execution status.
  * @return 0 -> Success
  * @return < 0 -> Fail
  */
 int8_t bmp5_get_oor_configuration(struct bmp5_oor_press_configuration *oor_press_config, struct bmp5_dev *dev);
 
 /*!
  * \ingroup bmp5ApiOOR
  * \page bmp5_api_bmp5_set_oor_configuration bmp5_set_oor_configuration
  * \code
  * int8_t bmp5_set_oor_configuration(const struct bmp5_oor_press_configuration *oor_press_config, struct bmp5_dev *dev);
  * \endcode
  * @details This API sets the configuration for out-of-range pressure threshold, range
  *  count limit and IIR.
  *
  * @param[in] oor_press_config : Structure instance of bmp5_oor_press_configuration.
  * @param[in] dev              : Structure instance of bmp5_dev.
  *
  * @return Result of API execution status.
  * @return 0 -> Success
  * @return < 0 -> Fail
  */
 int8_t bmp5_set_oor_configuration(const struct bmp5_oor_press_configuration *oor_press_config, struct bmp5_dev *dev);
 
 /**
  * \ingroup bmp5
  * \defgroup bmp5ApiNVM NVM
  * @brief NVM operation of the sensor
  */
 
 /*!
  * \ingroup bmp5ApiNVM
  * \page bmp5_api_bmp5_nvm_read bmp5_nvm_read
  * \code
  * int8_t bmp5_nvm_read(uint8_t nvm_addr, uint16_t *nvm_data, struct bmp5_dev *dev);
  * \endcode
  * @details This API is used to perform NVM read.
  *
  * @param[in] nvm_addr  : Variable that holds the nvm address.
  * @param[out] nvm_data : Variable to store nvm data.
  * @param[in] dev       : Structure instance of bmp5_dev.
  *
  * @return Result of API execution status.
  * @return 0 -> Success
  * @return < 0 -> Fail
  */
 int8_t bmp5_nvm_read(uint8_t nvm_addr, uint16_t *nvm_data, struct bmp5_dev *dev);
 
 /*!
  * \ingroup bmp5ApiNVM
  * \page bmp5_api_bmp5_nvm_write bmp5_nvm_write
  * \code
  * int8_t bmp5_nvm_write(uint8_t nvm_addr, const uint16_t *nvm_data, struct bmp5_dev *dev);
  * \endcode
  * @details This API used to perform NVM write.
  *
  * @param[in] nvm_addr  : Variable that holds the nvm address.
  * @param[in] nvm_data  : Variable that holds the nvm data.
  * @param[in] dev       : Structure instance of bmp5_dev.
  *
  * @return Result of API execution status.
  * @return 0 -> Success
  * @return < 0 -> Fail
  */
 int8_t bmp5_nvm_write(uint8_t nvm_addr, const uint16_t *nvm_data, struct bmp5_dev *dev);
 
 #ifdef __cplusplus
 }
 #endif /* End of CPP guard */
 
 #endif /* End of header guard macro */