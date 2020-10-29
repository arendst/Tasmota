/*
****************************************************************************
* Copyright (C) 2015 - 2016 Bosch Sensortec GmbH
*
* File :bma4.c
*
* Date: 12 Oct 2017
*
* Revision: 2.1.9 $
*
* Usage: Sensor Driver for BMA4 family of sensors
*
****************************************************************************
* Disclaimer
*
* Common:
* Bosch Sensortec products are developed for the consumer goods industry.
* They may only be used within the parameters of the respective valid
* product data sheet.  Bosch Sensortec products are provided with the
* express understanding that there is no warranty of fitness for a
* particular purpose.They are not fit for use in life-sustaining,
* safety or security sensitive systems or any system or device
* that may lead to bodily harm or property damage if the system
* or device malfunctions. In addition,Bosch Sensortec products are
* not fit for use in products which interact with motor vehicle systems.
* The resale and or use of products are at the purchasers own risk and
* his own responsibility. The examination of fitness for the intended use
* is the sole responsibility of the Purchaser.
*
* The purchaser shall indemnify Bosch Sensortec from all third party
* claims, including any claims for incidental, or consequential damages,
* arising from any product use not covered by the parameters of
* the respective valid product data sheet or not approved by
* Bosch Sensortec and reimburse Bosch Sensortec for all costs in
* connection with such claims.
*
* The purchaser must monitor the market for the purchased products,
* particularly with regard to product safety and inform Bosch Sensortec
* without delay of all security relevant incidents.
*
* Engineering Samples are marked with an asterisk (*) or (e).
* Samples may vary from the valid technical specifications of the product
* series. They are therefore not intended or fit for resale to third
* parties or for use in end products. Their sole purpose is internal
* client testing. The testing of an engineering sample may in no way
* replace the testing of a product series. Bosch Sensortec assumes
* no liability for the use of engineering samples.
* By accepting the engineering samples, the Purchaser agrees to indemnify
* Bosch Sensortec from all claims arising from the use of engineering
* samples.
*
* Special:
* This software module (hereinafter called "Software") and any information
* on application-sheets (hereinafter called "Information") is provided
* free of charge for the sole purpose to support your application work.
* The Software and Information is subject to the following
* terms and conditions:
*
* The Software is specifically designed for the exclusive use for
* Bosch Sensortec products by personnel who have special experience
* and training. Do not use this Software if you do not have the
* proper experience or training.
*
* This Software package is provided `` as is `` and without any expressed
* or implied warranties,including without limitation, the implied warranties
* of merchantability and fitness for a particular purpose.
*
* Bosch Sensortec and their representatives and agents deny any liability
* for the functional impairment
* of this Software in terms of fitness, performance and safety.
* Bosch Sensortec and their representatives and agents shall not be liable
* for any direct or indirect damages or injury, except as
* otherwise stipulated in mandatory applicable law.
*
* The Information provided is believed to be accurate and reliable.
* Bosch Sensortec assumes no responsibility for the consequences of use
* of such Information nor for any infringement of patents or
* other rights of third parties which may result from its use.
* No license is granted by implication or otherwise under any patent or
* patent rights of Bosch. Specifications mentioned in the Information are
* subject to change without notice.
**************************************************************************/
/*! \file bma4.c
    \brief Sensor Driver for BMA4 family of sensors */
/***************************************************************************/
/**\name        Header files
****************************************************************************/
#include "bma4.h"

/***************************************************************************/
/**\name    Static Data Buffer
****************************************************************************/
/* Local array to store the values read from the register
 * using read_regs API  */
static uint8_t temp_buff[BMA4_MAX_BUFFER_SIZE] = {0};

/***************************************************************************/
/**\name        Local structures
****************************************************************************/
/*!
 * @brief Accel difference value of axis.
 */
struct data_with_sign {
    /*! Difference value */
    int16_t val;
    /*! Indicates negative value if set */
    uint8_t is_negative;
};

/*!
 * @brief Accel data deviation from ideal value
 */
struct offset_delta {
    /*! Accel x axis */
    struct data_with_sign x;
    /*! Accel y axis */
    struct data_with_sign y;
    /*! Accel z axis */
    struct data_with_sign z;
};

/*!
 * @brief Accel offset xyz structure
 */
struct accel_offset {
    /*! Accel offset X  data */
    uint8_t x;
    /*! Accel offset Y  data */
    uint8_t y;
    /*! Accel offset Z  data */
    uint8_t z;
};

/*!
 * @brief Accel self test diff xyz data structure
 */
struct selftest_delta_limit {
    /*! Accel X  data */
    int32_t x;
    /*! Accel Y  data */
    int32_t y;
    /*! Accel Z  data */
    int32_t z;
};

/*!
 * @brief Structure to store temp data values
 */
struct accel_temp {
    /*! Accel X temp data */
    int32_t x;
    /*! Accel Y temp data */
    int32_t y;
    /*! Accel Z temp data */
    int32_t z;
};

/***************************************************************************/
/*! Static Function Declarations
****************************************************************************/

/*!
 *  @brief This API validates the bandwidth and perfmode
 *  value set by the user.
 *
 *  param bandwidth[in] : bandwidth value set by the user.
 *  param perf_mode[in] : perf_mode value set by the user.
 */
static uint16_t validate_bandwidth_perfmode(uint8_t bandwidth, uint8_t perf_mode);

/*!
 *  @brief @brief This API validates the ODR value set by the user.
 *
 *  param bandwidth[in] : odr for accelerometer
 */
static uint16_t validate_odr(uint8_t odr);

/*!
 *  @brief This API is used to reset the FIFO related configurations
 *  in the fifo_frame structure.
 *
 *  @param dev[in,out]  : Structure instance of bma4_dev
 *
 */
static void reset_fifo_data_structure(const struct bma4_dev *dev);

/*!
 *  @brief This API computes the number of bytes of accel FIFO data
 *  which is to be parsed in header-less mode
 *
 *  @param[out] start_idx   : The start index for parsing data
 *  @param[out] len         : Number of bytes to be parsed
 *  @param[in]  acc_count   : Number of accelerometer frames to be read
 *  @param[in]  dev         : Structure instance of bma4_dev.
 *
 */
static void get_accel_len_to_parse(uint16_t *start_idx, uint16_t *len, const uint16_t *acc_count,
                                   const struct bma4_dev *dev);

/*!
 *  @brief This API checks the fifo read data as empty frame, if it
 *  is empty frame then moves the index to last byte.
 *
 *  @param[in,out] data_index   : The index of the current data to
 *                                be parsed from fifo data
 *  @param[in]  dev             : Structure instance of bma4_dev.
 */
static void check_empty_fifo(uint16_t *data_index, const struct bma4_dev *dev);

/*!
 *  @brief This API is used to parse the accelerometer data from the
 *  FIFO data in header mode.
 *
 *  @param[in,out] accel_data   : Structure instance of bma4_accel where
 *                                the accelerometer data in FIFO is stored.
 *  @param[in,out] accel_length : Number of accelerometer frames
 *                                (x,y,z axes data)
 *  @param[in,out] dev          : Structure instance of bma4_dev.
 *
 */
static void extract_accel_header_mode(struct bma4_accel *accel_data, uint16_t *accel_length,
                                      const struct bma4_dev *dev);

/*!
 *  @brief This API is used to parse the accelerometer data from the
 *  FIFO data in both header mode and header-less mode.
 *  It update the idx value which is used to store the index of
 *  the current data byte which is parsed.
 *
 *  @param[in,out] acc       : Structure instance of bma4_accel.
 *  @param[in,out] idx       : Index value of number of bytes parsed
 *  @param[in,out] acc_idx   : Index value of accelerometer data
 *                             (x,y,z axes) frame to be parsed
 *  @param[in] frm           : It consists of either fifo_data_enable
 *                             parameter (Accel and/or mag data enabled in FIFO)
 *                             in header-less mode or frame header data
 *                             in header mode
 *  @param[in]  dev          : Structure instance of bma4_dev.
 *
 */
static void unpack_acc_frm(struct bma4_accel *acc, uint16_t *idx, uint16_t *acc_idx, uint8_t frm,
                           const struct bma4_dev *dev);

/*!
 *  @brief This API is used to parse the accelerometer data from the
 *  FIFO data and store it in the instance of the structure bma4_accel.
 *
 *  @param[out] accel_data        : Structure instance of bma4_accel where
 *                                  the parsed accel data bytes are stored.
 *  @param[in] data_start_index   : Index value of the accel data bytes
 *                                  which is to be parsed from the fifo data.
 *  @param[in] dev                :  Structure instance of bma4_dev.
 *
 */
static void unpack_accel_data(struct bma4_accel *accel_data, uint16_t data_start_index, const struct bma4_dev *dev);
/*!
 *  @brief This API computes the number of bytes of Mag FIFO data which is
 *  to be parsed in header-less mode
 *
 *  @param[out] start_idx   : The start index for parsing data
 *  @param[out] len         : Number of bytes to be parsed
 *  @param[in]  mag_count   : Number of magnetometer frames to be read
 *  @param[in]  dev         : Structure instance of bma4_dev.
 *
 */
static void get_mag_len_to_parse(uint16_t *start_idx, uint16_t *len, const uint16_t *mag_count,
                                 const struct bma4_dev *dev);
/*!
 *  @brief This API is used to parse the magnetometer data from the
 *  FIFO data in header mode.
 *
 *  @param[in,out] data     : Structure instance of bma4_mag_xyzr where
 *                            the magnetometer data in FIFO is extracted
 *                            and stored.
 *  @param[in,out] len      : Number of magnetometer frames
 *                           (x,y,z,r data)
 *  @param[in,out] dev      : Structure instance of bma4_dev.
 *
 *  @return Result of API execution status
 *  @retval 0 -> Success
 *  @retval Any non zero value -> Fail
 *
 */
static uint16_t extract_mag_header_mode(struct bma4_mag *data, uint16_t *len, const struct bma4_dev *dev);

/*!
 *  @brief This API is used to parse the magnetometer data from the
 *  FIFO data in both header mode and header-less mode and update the
 *  idx value which is used to store the index of the current
 *  data byte which is parsed.
 *
 *  @param data     : Structure instance of bma4_mag_xyzr.
 *  @param idx      : Index value of number of bytes parsed
 *  @param mag_idx  : Index value magnetometer data frame (x,y,z,r)
 *                    to be parsed
 *  @param frm      : It consists of either the fifo_data_enable parameter
 *                    (Accel and/or mag data enabled in FIFO) in
 *                    header-less mode and frame header data in header mode
 *  @param dev      : Structure instance of bma4_dev.
 *
 *  @return Result of API execution status
 *  @retval 0 -> Success
 *  @retval Any non zero value -> Fail
 *
 */
static uint16_t unpack_mag_frm(struct bma4_mag *data, uint16_t *idx, uint16_t *mag_idx, uint8_t frm,
                               const struct bma4_dev *dev);

/*!
 *  @brief This API is used to parse the auxiliary magnetometer data from
 *  the FIFO data and store it in the instance of the structure mag_data.
 *
 *  @param mag_data         : Structure instance of bma4_mag_xyzr where the
 *                            parsed magnetometer data bytes are stored.
 *  @param start_idx        : Index value of the magnetometer data bytes
 *                            which is to be parsed from the FIFO data
 *  @param dev              : Structure instance of bma4_dev.
 *
 *  @return Result of API execution status
 *  @retval 0 -> Success
 *  @retval Any non zero value -> Fail
 *
 */
static uint16_t unpack_mag_data(struct bma4_mag *mag_data, uint16_t start_idx, const struct bma4_dev *dev);

/*!
 *  @brief This API is used to parse and store the sensor time from the
 *  FIFO data in the structure instance dev.
 *
 *  @param[in,out] data_index : Index of the FIFO data which
 *                              has the sensor time.
 *  @param[in,out] dev        : Structure instance of bma4_dev.
 *
 */
static void unpack_sensortime_frame(uint16_t *data_index, const struct bma4_dev *dev);

/*!
 *  @brief This API is used to parse and store the skipped_frame_count from
 *  the FIFO data in the structure instance dev.
 *
 *  @param[in,out] data_index       : Index of the FIFO data which
 *                                    has the skipped frame count.
 *  @param[in,out] dev              : Structure instance of bma4_dev.
 *
 */
static void unpack_skipped_frame(uint16_t *data_index, const struct bma4_dev *dev);

/*!
 *  @brief This API is used to parse and store the dropped_frame_count from
 *  the FIFO data in the structure instance dev.
 *
 *  @param[in,out] data_index       : Index of the FIFO data which
 *                                    has the dropped frame data.
 *  @param[in,out] dev              : Structure instance of bma4_dev.
 *
 */
static void unpack_dropped_frame(uint16_t *data_index, const struct bma4_dev *dev);

/*!
 *  @brief This API is used to move the data index ahead of the
 *  current_frame_length parameter when unnecessary FIFO data appears while
 *  extracting the user specified data.
 *
 *  @param[in,out] data_index       : Index of the FIFO data which
 *                                    is to be moved ahead of the
 *                                    current_frame_length
 *  @param[in] current_frame_length : Number of bytes in a particular frame
 *  @param[in] dev                  : Structure instance of bma4_dev.
 *
 */
static void move_next_frame(uint16_t *data_index, uint8_t current_frame_length, const struct bma4_dev *dev);

/*!
*  @brief This API writes the config stream data in memory using burst mode
*
*  @param[in] stream_data : Pointer to store data of 32 bytes
*  @param[in] index : Represents value in multiple of 32 bytes
*  @param[in] dev : Structure instance of bma4_dev.
*
*  @return Result of API execution status
*  @retval 0 -> Success
*  @retval Any non zero value -> Fail
*/
static uint16_t stream_transfer_write(const uint8_t *stream_data, uint16_t index, struct bma4_dev *dev);

/*!
 *  @brief This API enables or disables the Accel Self test feature in the
 *  sensor.
 *
 *  @param[in] accel_selftest_enable : Variable used to enable or disable
 *  the Accel self test feature
 *  Value   |  Description
 *  --------|---------------
 *  0x00    | BMA4_DISABLE
 *  0x01    | BMA4_ENABLE
 *
 *  @param[in] dev : Structure instance of bma4_dev
 *
 *  @return Result of API execution status
 *  @retval 0 -> Success
 *  @retval Any non zero value -> Fail
 *
 */
static uint16_t set_accel_selftest_enable(uint8_t accel_selftest_axis, struct bma4_dev *dev);

/*!
 *  @brief This API selects the sign of Accel self-test excitation
 *
 *  @param[in] accel_selftest_sign: Variable used to select the Accel
 *  self test sign
 *  Value   |  Description
 *  --------|--------------------------
 *  0x00    | BMA4_DISABLE (negative)
 *  0x01    | BMA4_ENABLE (positive)
 *
 *  @param[in] dev : Structure instance of bma4_dev
 *
 *  @return Result of API execution status
 *  @retval 0 -> Success
 *  @retval Any non zero value -> Fail
 *
 */
static uint16_t set_accel_selftest_sign(uint8_t accel_selftest_sign, struct bma4_dev *dev);

/*!
 *  @brief This API sets the Accel self test amplitude in the sensor.
 *
 *  @param[in] accel_selftest_amp : Variable used to specify the Accel self
 *  test amplitude
 *  Value   |  Description
 *  --------|------------------------------------
 *  0x00    | BMA4_SELFTEST_AMP_LOW
 *  0x01    | BMA4_SELFTEST_AMP_HIGH
 *
 *  @param[in] dev : structure instance of bma4_dev
 *
 *  @return Result of API execution status
 *  @retval 0 -> Success
 *  @retval Any non zero value -> Fail
 *
 */
static uint16_t set_accel_selftest_amp(uint8_t accel_selftest_amp, struct bma4_dev *dev);

/*!
 *  @brief This function enables and configures the Accel which is needed
 *  for Self test operation.
 *
 *  @param[in] dev : Structure instance of bma4_dev
 *
 *  @return results of self test
 *  @retval 0 -> Success
 *  @retval Any non zero value -> Fail
 *
 */
static uint16_t set_accel_selftest_config(struct bma4_dev *dev);

/*!
 *  @brief This API validates the Accel g value provided as input by the
 *  user for Accel offset compensation.
 *
 *  @param[in] gvalue : Pointer variable used to specify the g value
 *  set by the user for Accel offset compensation.
 *
 *  @note The g-values to be passed to the parameter should be
 *  multiples of 1000000.
 *
 *  @return results of the status of user input validation
 *  @retval 0 -> Success
 *  @retval Any non zero value -> Fail
 *
 */
static uint16_t validate_user_input(const int32_t *gvalue);
/*!
 *  @brief This API converts the range value into corresponding integer
 *  value.
 *
 *  @param[in] range_in : input range value.
 *  @param[out] range_out : pointer to store the integer value of range.
 *
 *  @return results of the status of user input validation
 *  @retval 0 -> Success
 *  @retval Any non zero value -> Fail
 *
 */
static void map_range(uint8_t range_in, uint8_t *range_out);

/*!
 *  @brief This API normalise the data with offset.
 *
 *  @param[out] compensated_data : pointer to store the compensated data.
 *  @param[in] offset_data : pointer of offset.
 *
 *  @return results of the status of user input validation
 *  @retval 0 -> Success
 *  @retval Any non zero value -> Fail
 *
 */
static void normalise_offset(const struct offset_delta *compensated_data, struct accel_offset *offset_data);

/*!
 *  @brief This API normalise the data with offset.
 *
 *  @param[in] res : resolution of the sensor.
 *  @param[in] range : G-range of the accel.
 *  @param[in] delta : pointer of offset_delta.
 *  @param[out] data : pointer to store accel_offset data.
 *
 *  @return results of the status of user input validation
 *  @retval 0 -> Success
 *  @retval Any non zero value -> Fail
 *
 */
static void scale_offset(uint8_t res, uint8_t range, const struct offset_delta *delta, struct accel_offset *data);

/*!
 *  @brief This API compensate the accel data against gravity.
 *
 *  @param[in] lsb_per_g : lsb value pre 1g.
 *  @param[in] g_val : G reference value of all axis.
 *  @param[in] data : pointer of accel_offset data.
 *  @param[out] comp_data : pointer to store compensated data.
 *
 *  @note The g-values to be passed to the parameter should be
 *  multiples of 1000000.
 *
 *  @return results of the status of user input validation
 *  @retval 0 -> Success
 *  @retval Any non zero value -> Fail
 *
 */
static void comp_for_grvty(uint16_t lsb_per_g, const int32_t g_val[3], const struct bma4_accel *data,
                           struct offset_delta *comp_data);
/*!
 *  @brief This function validates the Accel Self test data and decides the
 *  result of Self test operation.
 *
 *  @param[in] accel_data_diff : Pointer to structure variable which holds
 *  the Accel data difference of Self test operation
 *  @param[in] dev : Structure instance of bma4_dev
 *
 *  @return results of self test operation
 *  @retval 0 -> Success
 *  @retval Any non zero value -> Fail
 *
 */
static uint16_t validate_selftest(const struct selftest_delta_limit *accel_data_diff,
                                  const struct bma4_dev *dev);

/*!
 *  @brief This function configure the Accel for FOC.
 *
 *  @param[in] acc_conf : accel config structure instance
 *  @param[in] acc_en : enables the accel
 *  @param[in] pwr_mode : set the power mode
 *  @param[in] dev : Structure instance of bma4_dev
 *
 *  @return results of self test operation
 *  @retval 0 -> Success
 *  @retval Any non zero value -> Fail
 *
 */
static uint16_t foc_config(struct bma4_accel_config *acc_conf, uint8_t *acc_en, uint8_t *pwr_mode,
                           struct bma4_dev *dev);

/*!
 *  @brief This API converts lsb value of axes to mg for self-test
 *
 *  @param[in] accel_data_diff : Pointer variable used to pass accel difference
 *  values in g
 *  @param[out] accel_data_diff_mg : Pointer variable used to store accel
 *  difference values in mg
 *  @param[out] dev : Structure instance of bma4_dev
 *
 *  @return None *
 */
static void convert_lsb_g(const struct selftest_delta_limit *accel_data_diff,
                          struct selftest_delta_limit *accel_data_diff_mg,
                          const struct bma4_dev *dev);

/*!
 *  @brief This API sets the feature config. data start address in the sensor.
 *
 *  @param[in] dev  : Structure instance of bma4_dev.
 *
 *  @return Result of API execution status
 *  @retval 0 -> Success
 *  @retval Any non zero value -> Fail
 */
static uint16_t set_feature_config_start_addr(struct bma4_dev *dev);

/*!
 *  @brief This API increments the feature config. data address according to the user
 *  provided read/write length in the dev structure.
 *
 *  @param[in] dev  : Structure instance of bma4_dev.
 *
 *  @return Result of API execution status
 *  @retval 0 -> Success
 *  @retval Any non zero value -> Fail
 */
static uint16_t increment_feature_config_addr(const struct bma4_dev *dev);

/*!
 *  @brief This API reads the 8-bit data from the given register
 *  in the sensor.
 */
static uint16_t read_regs(uint8_t addr, uint8_t *data, uint8_t len, const struct bma4_dev *dev);

/*!
 *  @brief This API writes the 8-bit data to the given register
 *  in the sensor.
 */
static uint16_t write_regs(uint8_t addr, uint8_t *data, uint8_t len, const struct bma4_dev *dev);

/*!
 *  @brief This API sets the feature config. data start address in the sensor.
 */
static uint16_t get_feature_config_start_addr(struct bma4_dev *dev);

/*!
 * @brief This API is used to calculate the power of given
 * base value.
 *
 * @param[in] base : value of base
 * @param[in] resolution : resolution of the sensor
 *
 * @return : return the value of base^resolution
 */

static int32_t power(int16_t base, uint8_t resolution);

/*!
 * @brief This API performs roundoff on given value
 *
 * @param[in] value : Value which is need to be rounded off
 *
 * @return : None
 */
static int8_t roundoff(int32_t value);

/*!
 * @brief This API finds the bit position of 3.9mg according to given range
 * and resolution.
 *
 * @param[in] range : range of the accel.
 * @param[in] res : resolution of the accel.
 *
 * @return : bit position of 3.9mg
 */
static int8_t get_bit_pos_3_9mg(uint8_t range, uint8_t res);

/*!
 * @brief This API finds the the null error of the device pointer structure
 *
 * @param[in] dev : Structure instance of bma4_dev.
 *
 * @return Null error
 */
static uint16_t bma4_null_pointer_check(const struct bma4_dev *dev);

/*!
 * @brief This internal API brings up the secondary interface to access
 *          auxiliary sensor
 *
 * @param[in] dev : Structure instance of bma4_dev.
 *
 * @return Result of API execution status
 *
 * @retval 0 if success, else fail
 */
static uint16_t bma4_set_aux_interface_config(struct bma4_dev *dev);

/*!
 * @brief This internal API reads the data from the auxiliary sensor
 * depending on burst length configured
 *
 * @param[in] dev  : Structure instance of bma4_dev.
 * @param[out] aux_data  : Pointer variable to store data read
 * @param[in] aux_reg_addr  : Variable to pass address from where
 *                            data is to be read
 *
 * @return Result of API execution status
 *
 * @retval 0 if success, else fail
 */
static uint16_t bma4_extract_aux_data(uint8_t aux_reg_addr, uint8_t *aux_data, uint16_t len, struct bma4_dev *dev);

/*!
 * @brief This internal API maps the actual burst read length with user length set.
 *
 * @param[in] dev  : Structure instance of bma4_dev.
 * @param[out] len  : Pointer variable to store mapped length
 *
 * @return Result of API execution status
 *
 * @retval 0 if success, else fail
 */
static uint16_t bma4_map_read_len(uint8_t *len, const struct bma4_dev *dev);

/***************************************************************************/
/**\name        Extern Declarations
****************************************************************************/

/***************************************************************************/
/**\name        Globals
****************************************************************************/


/***************************************************************************/
/**\name        Function definitions
****************************************************************************/
/*!
 *  @brief This API is the entry point.
 *  Call this API before using all other APIs.
 *  This API reads the chip-id of the sensor which is the first step to
 *  verify the sensor and also it configures the read mechanism of SPI and
 *  I2C interface.
 */
uint16_t bma4_init(struct bma4_dev *dev)
{
    uint16_t rslt = 0;
    uint8_t data = 0;
    /* Check the bma4 structure as NULL */
    if ((dev == NULL) || (dev->bus_read == NULL) || (dev->bus_write == NULL)) {
        rslt |= BMA4_E_NULL_PTR;
    } else {
        if (dev->interface == BMA4_SPI_INTERFACE)
            dev->dummy_byte = 1;
        else
            dev->dummy_byte = 0;

        rslt |= bma4_read_regs(BMA4_CHIP_ID_ADDR, &data, 1, dev);

        if (rslt == BMA4_OK) {
            /* Assign Chip Id */
            dev->chip_id = data;
        }
    }

    return rslt;
}

/*!
 *  @brief This API is used to write the binary configuration in the sensor
 */
uint16_t bma4_write_config_file(struct bma4_dev *dev)
{
    uint16_t rslt;
    /* Config loading disable*/
    uint8_t config_load = 0;
    uint16_t index = 0;
    uint8_t config_stream_status = 0;

    /* Disable advanced power save */
    rslt = bma4_set_advance_power_save(BMA4_DISABLE, dev);
    /* Wait for sensor time synchronization. Refer the data-sheet for
    more information*/
    dev->delay(1);

    if (rslt == BMA4_OK) {
        /* Disable config loading*/
        rslt |= bma4_write_regs(BMA4_INIT_CTRL_ADDR, &config_load, 1, dev);
        /* Write the config stream */
        for (index = 0; index < BMA4_CONFIG_STREAM_SIZE; index += dev->read_write_len)
            rslt |= stream_transfer_write((dev->config_file_ptr + index), index, dev);

        /* Enable config loading and FIFO mode */
        config_load = 0x01;
        rslt |= bma4_write_regs(BMA4_INIT_CTRL_ADDR, &config_load, 1, dev);
        /* Wait till ASIC is initialized. Refer the data-sheet for
        more information*/
        dev->delay(150);
        /* Read the status of config stream operation */
        rslt |= bma4_read_regs(BMA4_INTERNAL_STAT, &config_stream_status, 1, dev);

        if (config_stream_status != BMA4_ASIC_INITIALIZED) {
            rslt |= BMA4_E_CONFIG_STREAM_ERROR;
        } else {
            /* Enable advanced power save */
            rslt |= bma4_set_advance_power_save(BMA4_ENABLE, dev);
            rslt |= get_feature_config_start_addr(dev);
        }
    }
    return rslt;
}

/*!
 *  @brief This API checks whether the write operation requested is for feature
 *  config or register write and accordingly writes the data in the sensor.
 */
uint16_t bma4_write_regs(uint8_t addr, uint8_t *data, uint8_t len, struct bma4_dev *dev)
{
    uint8_t i;
    uint8_t loop_count;
    uint8_t overflow;
    uint8_t index;
    uint16_t rslt = BMA4_OK;
    uint8_t adv_pwr_save = 0;

    /* Check the dev structure as NULL*/
    if (dev == NULL) {
        rslt |= BMA4_E_NULL_PTR;
    } else {
        if (addr == BMA4_FEATURE_CONFIG_ADDR) {
            /* Disable APS if enabled before writing the feature config register */
            rslt = bma4_get_advance_power_save(&adv_pwr_save, dev);
            if (adv_pwr_save == BMA4_ENABLE) {
                rslt |= bma4_set_advance_power_save(BMA4_DISABLE, dev);
                /* Wait for sensor time synchronization. Refer the data-sheet for
                more information*/
                dev->delay(1);
            }

            if (((len % 2) == 0) && (len <= dev->feature_len) && (rslt == BMA4_OK)) {
                if (dev->read_write_len < len) {
                    /* Calculate the no of writes to be performed according
                    to the read/write length */
                    loop_count = len / dev->read_write_len;
                    overflow = len % dev->read_write_len;
                    index = 0;
                    rslt = set_feature_config_start_addr(dev);
                    for (i = 0; i < loop_count; i++) {
                        rslt |= write_regs(BMA4_FEATURE_CONFIG_ADDR, data + index,
                                           dev->read_write_len, dev);
                        rslt |= increment_feature_config_addr(dev);
                        index = index + dev->read_write_len;
                    }
                    if (overflow)
                        rslt |= write_regs(BMA4_FEATURE_CONFIG_ADDR, data + index,
                                           overflow, dev);
                    rslt |= set_feature_config_start_addr(dev);
                } else {
                    rslt = write_regs(BMA4_FEATURE_CONFIG_ADDR, data, len, dev);
                }
            } else {
                rslt = BMA4_E_RD_WR_LENGTH_INVALID;
            }

            if (rslt == BMA4_OK) {
                /* Enable APS once write feature config register is done */
                rslt = bma4_get_advance_power_save(&adv_pwr_save, dev);
                if (adv_pwr_save == BMA4_DISABLE) {
                    rslt |= bma4_set_advance_power_save(BMA4_ENABLE, dev);
                    /* Wait for sensor time synchronization. Refer the data-sheet for
                    more information*/
                    dev->delay(1);
                }
            }

        } else {
            rslt = write_regs(addr, data, len, dev);
        }
    }

    return rslt;
}

/*!
 *  @brief This API writes the 8-bit data to the given register
 *  in the sensor.
 */
static uint16_t write_regs(uint8_t addr, uint8_t *data, uint8_t len, const struct bma4_dev *dev)
{
    uint16_t rslt = 0;

    /* Check the bma4 structure as NULL */
    if (dev == NULL) {
        rslt |= BMA4_E_NULL_PTR;
    } else {
        if (dev->interface == BMA4_SPI_INTERFACE)
            addr = addr & BMA4_SPI_WR_MASK;
        /* write data in the register*/
        rslt |= dev->bus_write(dev->dev_addr, addr, data, len);
    }
    return rslt;
}

/*!
 *  @brief This API sets the feature config. data start address in the sensor.
 */
static uint16_t get_feature_config_start_addr(struct bma4_dev *dev)
{
    uint16_t rslt;
    uint8_t asic_lsb;
    uint8_t asic_msb;

    rslt = read_regs(BMA4_RESERVED_REG_5B_ADDR, &asic_lsb, 1, dev);
    rslt |= read_regs(BMA4_RESERVED_REG_5C_ADDR, &asic_msb, 1, dev);

    /* Store asic info in dev structure */
    dev->asic_data.asic_lsb = asic_lsb & 0x0F;
    dev->asic_data.asic_msb = asic_msb;

    return rslt;
}

/*!
 *  @brief This API sets the feature config. data start address in the sensor.
 */
static uint16_t set_feature_config_start_addr(struct bma4_dev *dev)
{
    uint16_t rslt;

    rslt = write_regs(BMA4_RESERVED_REG_5B_ADDR, &dev->asic_data.asic_lsb, 1, dev);
    rslt |= write_regs(BMA4_RESERVED_REG_5C_ADDR, &dev->asic_data.asic_msb, 1, dev);

    return rslt;
}

/*!
 *  @brief This API increments the feature config. data address according to the user
 *  provided read/write length in the dev structure.
 */
static uint16_t increment_feature_config_addr(const struct bma4_dev *dev)
{
    uint16_t rslt;
    uint16_t asic_addr;
    uint8_t asic_lsb;
    uint8_t asic_msb;

    /* Read the asic address from the sensor */
    rslt = read_regs(BMA4_RESERVED_REG_5B_ADDR, &asic_lsb, 1, dev);
    rslt |= read_regs(BMA4_RESERVED_REG_5C_ADDR, &asic_msb, 1, dev);
    /* Get the asic address */
    asic_addr = (asic_msb << 4) | (asic_lsb & 0x0F);
    /* Sum the asic address with read/write length after converting from
    byte to word */
    asic_addr = asic_addr + (dev->read_write_len / 2);
    /* Split the asic address */
    asic_lsb = asic_addr & 0x0F;
    asic_msb = (uint8_t)(asic_addr  >> 4);
    /* Write the asic address in the sensor */
    rslt |= write_regs(BMA4_RESERVED_REG_5B_ADDR, &asic_lsb, 1, dev);
    rslt |= write_regs(BMA4_RESERVED_REG_5C_ADDR, &asic_msb, 1, dev);

    return rslt;
}

/*!
 *  @brief This API checks whether the read operation requested is for feature
 *  or register read and accordingly reads the data from the sensor.
 */
uint16_t bma4_read_regs(uint8_t addr, uint8_t *data, uint8_t len, struct bma4_dev *dev)
{
    uint8_t i;
    uint8_t loop_count;
    uint8_t overflow;
    uint8_t index;
    uint16_t rslt = BMA4_OK;
    uint8_t adv_pwr_save = 0;

    /* Check the dev structure as NULL*/
    if (dev == NULL) {
        rslt |= BMA4_E_NULL_PTR;
    } else {
        if (addr == BMA4_FEATURE_CONFIG_ADDR) {
            /* Disable APS if enabled before reading the feature config register */
            rslt = bma4_get_advance_power_save(&adv_pwr_save, dev);
            if (adv_pwr_save == BMA4_ENABLE) {
                rslt |= bma4_set_advance_power_save(BMA4_DISABLE, dev);
                /* Wait for sensor time synchronization. Refer the data-sheet for
                more information*/
                dev->delay(1);
            }

            if (((len % 2) == 0) && (len <= dev->feature_len) && (rslt == BMA4_OK)) {
                if (dev->read_write_len < len) {
                    /* Calculate the no of writes to be performed according
                    to the read/write length */
                    loop_count = len / dev->read_write_len;
                    overflow = len % dev->read_write_len;
                    index = 0;
                    rslt = set_feature_config_start_addr(dev);
                    for (i = 0; i < loop_count; i++) {
                        rslt |= read_regs(BMA4_FEATURE_CONFIG_ADDR, data + index,
                                          dev->read_write_len, dev);
                        rslt |= increment_feature_config_addr(dev);
                        index = index + dev->read_write_len;
                    }
                    if (overflow)
                        rslt |= read_regs(BMA4_FEATURE_CONFIG_ADDR, data + index,
                                          overflow, dev);
                    rslt |= set_feature_config_start_addr(dev);
                } else {
                    rslt = read_regs(BMA4_FEATURE_CONFIG_ADDR, data, len, dev);
                }
            } else {
                rslt = BMA4_E_RD_WR_LENGTH_INVALID;
            }
            if (rslt == BMA4_OK) {
                /* Enable APS once read feature config register is done */
                rslt = bma4_get_advance_power_save(&adv_pwr_save, dev);
                if (adv_pwr_save == BMA4_DISABLE) {
                    rslt |= bma4_set_advance_power_save(BMA4_ENABLE, dev);
                    /* Wait for sensor time synchronization. Refer the data-sheet for
                    more information*/
                    dev->delay(1);
                }
            }
        } else {
            rslt = read_regs(addr, data, len, dev);
        }
    }

    return rslt;
}

/*!
 *  @brief This API reads the 8-bit data from the given register
 *  in the sensor.
 */
static uint16_t read_regs(uint8_t addr, uint8_t *data, uint8_t len, const struct bma4_dev *dev)
{
    /* variable used to return the status of communication result*/
    uint16_t rslt = 0;
    uint16_t temp_len = len + dev->dummy_byte;
    uint16_t i;

    if (dev->interface == BMA4_SPI_INTERFACE) {
        /* SPI mask added */
        addr = addr | BMA4_SPI_RD_MASK;
    }

    if (temp_len > BMA4_MAX_BUFFER_SIZE) {
        /* Buffer size is not sufficient */
        rslt |= BMA4_E_OUT_OF_RANGE;
    }

    if (rslt == BMA4_OK) {
        /* Read the data from the register */
        rslt |= dev->bus_read(dev->dev_addr, addr, temp_buff, temp_len);

        for (i = 0; i < len; i++) {
            /* Parsing and storing the valid data */
            data[i] = temp_buff[i + dev->dummy_byte];
        }
    }

    return rslt;
}

/*!
 *  @brief This API reads the error status from the sensor.
 */
uint16_t bma4_get_error_status(struct bma4_err_reg *err_reg, struct bma4_dev *dev)
{
    uint16_t rslt = 0;
    uint8_t data = 0;

    /* Check the bma4 structure as NULL */
    if (dev == NULL) {
        rslt |= BMA4_E_NULL_PTR;
    } else {
        /* Read the error codes*/
        rslt |= bma4_read_regs(BMA4_ERROR_ADDR, &data, 1, dev);

        if (rslt == BMA4_OK) {
            /* Fatal error*/
            err_reg->fatal_err = BMA4_GET_BITS_POS_0(data, BMA4_FATAL_ERR);
            /* Cmd error*/
            err_reg->cmd_err = BMA4_GET_BITSLICE(data, BMA4_CMD_ERR);
            /* User error*/
            err_reg->err_code = BMA4_GET_BITSLICE(data, BMA4_ERR_CODE);
            /* FIFO error*/
            err_reg->fifo_err = BMA4_GET_BITSLICE(data, BMA4_FIFO_ERR);
            /* Mag data ready error*/
            err_reg->aux_err = BMA4_GET_BITSLICE(data, BMA4_AUX_ERR);
        }
    }

    return rslt;
}

/*!
 *  @brief This API reads the sensor status from the sensor.
 */
uint16_t bma4_get_status(uint8_t *status, struct bma4_dev *dev)
{
    uint16_t rslt = 0;
    uint8_t data = 0;

    /* Check the bma4 structure as NULL */
    if (dev == NULL) {
        rslt |= BMA4_E_NULL_PTR;
    } else {
        /* Read the error codes*/
        rslt |= bma4_read_regs(BMA4_STATUS_ADDR, &data, 1, dev);

        if (rslt == BMA4_OK)
            *status = data;
    }

    return rslt;
}

/*!
 *  @brief This API reads the Accel data for x,y and z axis from the sensor.
 *  The data units is in LSB format.
 */
uint16_t bma4_read_accel_xyz(struct bma4_accel *accel, struct bma4_dev *dev)
{
    uint16_t rslt = 0;
    uint16_t lsb = 0;
    uint16_t msb = 0;
    uint8_t data[BMA4_ACCEL_DATA_LENGTH] = {0};

    /* Check the bma4 structure as NULL */
    if (dev == NULL) {
        rslt |= BMA4_E_NULL_PTR;
    } else {
        rslt |= bma4_read_regs(BMA4_DATA_8_ADDR, data, BMA4_ACCEL_DATA_LENGTH, dev);

        if (rslt == BMA4_OK) {
            msb = data[1];
            lsb = data[0];
            /* Accel data x axis */
            accel->x = (int16_t)((msb << 8) | lsb);

            msb = data[3];
            lsb = data[2];
            /* Accel data y axis */
            accel->y = (int16_t)((msb << 8) | lsb);

            msb = data[5];
            lsb = data[4];
            /* Accel data z axis */
            accel->z = (int16_t)((msb << 8) | lsb);

            if (dev->resolution == BMA4_12_BIT_RESOLUTION) {
                accel->x = (accel->x / 0x10);
                accel->y = (accel->y / 0x10);
                accel->z = (accel->z / 0x10);
            } else if (dev->resolution == BMA4_14_BIT_RESOLUTION) {
                accel->x = (accel->x / 0x04);
                accel->y = (accel->y / 0x04);
                accel->z = (accel->z / 0x04);
            }
        }
    }

    return rslt;
}

/*!
 *  @brief This API reads the sensor time of Sensor time gets updated
 *  with every update of data register or FIFO.
 */
uint16_t bma4_get_sensor_time(uint32_t *sensor_time, struct bma4_dev *dev)
{
    uint16_t rslt = 0;
    uint8_t data[BMA4_SENSOR_TIME_LENGTH] = {0};
    uint8_t msb = 0;
    uint8_t xlsb = 0;
    uint8_t lsb = 0;

    /* Check the bma4 structure as NULL */
    if (dev == NULL) {
        rslt |= BMA4_E_NULL_PTR;
    } else {
        rslt |= bma4_read_regs(BMA4_SENSORTIME_0_ADDR, data, BMA4_SENSOR_TIME_LENGTH, dev);

        if (rslt == BMA4_OK) {
            msb = data[BMA4_SENSOR_TIME_MSB_BYTE];
            xlsb = data[BMA4_SENSOR_TIME_XLSB_BYTE];
            lsb = data[BMA4_SENSOR_TIME_LSB_BYTE];
            *sensor_time = (uint32_t)((msb << 16) | (xlsb << 8) | lsb);
        }
    }

    return rslt;
}

/*!
 *  @brief This API reads the chip temperature of sensor.
 *
 *  @note Using a scaling factor of 1000, to obtain integer values, which
 *  at the user end, are used to get accurate temperature value .
 *  BMA4_FAHREN_SCALED = 1.8 * 1000, BMA4_KELVIN_SCALED = 273.15 * 1000
 */
uint16_t bma4_get_temperature(int32_t *temp, uint8_t temp_unit, struct bma4_dev *dev)
{
    uint16_t rslt = 0;
    uint8_t data[BMA4_TEMP_DATA_SIZE] = {0};
    int32_t temp_raw_scaled = 0;

    /* Check for Null pointer error */
    if (dev == NULL) {
        rslt |= BMA4_E_NULL_PTR;
    } else {
        /* Read temperature value from the register */
        rslt |= bma4_read_regs(BMA4_TEMPERATURE_ADDR, data, BMA4_TEMP_DATA_SIZE, dev);

        if (rslt == BMA4_OK)
            temp_raw_scaled = (int32_t)data[BMA4_TEMP_BYTE] * BMA4_SCALE_TEMP;

        /* '0' value read from the register corresponds to 23 degree C */
        (*temp) = temp_raw_scaled + (BMA4_OFFSET_TEMP * BMA4_SCALE_TEMP);

        switch (temp_unit) {
        case BMA4_DEG:
            break;

        case BMA4_FAHREN:
            /* Temperature in degree Fahrenheit */
            (*temp) = (((*temp) / BMA4_SCALE_TEMP) * BMA4_FAHREN_SCALED) + (32 * BMA4_SCALE_TEMP);
            break;

        case BMA4_KELVIN:
            /* Temperature in degree Kelvin */
            (*temp) = (*temp) + BMA4_KELVIN_SCALED;
            break;

        default:
            break;
        }
    }

    return rslt;
}

/*!
 *  @brief This API reads the Output data rate, Bandwidth, perf_mode
 *  and Range of accel.
 */
uint16_t bma4_get_accel_config(struct bma4_accel_config *accel, struct bma4_dev *dev)
{
    uint16_t rslt = 0;
    uint8_t data[2] = {0};

    /* Check the bma4 structure as NULL */
    if (dev == NULL) {
        rslt |= BMA4_E_NULL_PTR;
    } else {
        rslt |= bma4_read_regs(BMA4_ACCEL_CONFIG_ADDR, data, BMA4_ACCEL_CONFIG_LENGTH, dev);

        if (rslt == BMA4_OK) {
            /* To get the ODR */
            accel->odr =  BMA4_GET_BITS_POS_0(data[0], BMA4_ACCEL_ODR);
            /* To get the bandwidth */
            accel->bandwidth = BMA4_GET_BITSLICE(data[0], BMA4_ACCEL_BW);
            /* To get the under sampling mode */
            accel->perf_mode = BMA4_GET_BITSLICE(data[0], BMA4_ACCEL_PERFMODE);
            /* Read the Accel range */
            accel->range = BMA4_GET_BITS_POS_0(data[1], BMA4_ACCEL_RANGE);
        }
    }

    return rslt;
}

/*!
 *  @brief This API sets the output_data_rate, bandwidth, perf_mode
 *  and range of Accel.
 */
uint16_t bma4_set_accel_config(const struct bma4_accel_config *accel, struct bma4_dev *dev)
{
    uint16_t rslt = 0;
    uint8_t accel_config_data[2] = {0, 0};

    /* Check the bma4 structure as NULL */
    if (dev == NULL) {
        rslt |= BMA4_E_NULL_PTR;
    } else {
        /* check whether the bandwidth and perfmode
            settings are valid */
        rslt = validate_bandwidth_perfmode(accel->bandwidth, accel->perf_mode);

        if (rslt == BMA4_OK) {
            /* check ODR is valid */
            rslt |= validate_odr(accel->odr);

            if (rslt == BMA4_OK) {
                accel_config_data[0] = accel->odr & BMA4_ACCEL_ODR_MSK;
                accel_config_data[0] |= (uint8_t)(accel->bandwidth << BMA4_ACCEL_BW_POS);
                accel_config_data[0] |= (uint8_t)(accel->perf_mode << BMA4_ACCEL_PERFMODE_POS);
                accel_config_data[1] = accel->range & BMA4_ACCEL_RANGE_MSK;

                /* Burst write is not possible in
                suspend mode hence individual write is
                used with delay of 1 ms */
                rslt |= bma4_write_regs(BMA4_ACCEL_CONFIG_ADDR, &accel_config_data[0], 1, dev);
                dev->delay(BMA4_GEN_READ_WRITE_DELAY);
                rslt |= bma4_write_regs((BMA4_ACCEL_CONFIG_ADDR + 1), &accel_config_data[1],
                                        1, dev);
            }
        }
    }

    return rslt;
}

/*!
 *  @brief This API validates the bandwidth and perf_mode
 *  value set by the user.
 */
static uint16_t validate_bandwidth_perfmode(uint8_t bandwidth, uint8_t perf_mode)
{
    uint16_t rslt = BMA4_OK;

    if (perf_mode == BMA4_CONTINUOUS_MODE) {
        if (bandwidth > BMA4_ACCEL_NORMAL_AVG4) {
            /* Invalid bandwidth error for continuous mode */
            rslt = BMA4_E_OUT_OF_RANGE;
        }
    } else if (perf_mode == BMA4_CIC_AVG_MODE) {
        if (bandwidth > BMA4_ACCEL_RES_AVG128) {
            /* Invalid bandwidth error for CIC avg. mode */
            rslt = BMA4_E_OUT_OF_RANGE;
        }
    } else {
        rslt = BMA4_E_OUT_OF_RANGE;
    }

    return rslt;
}

/*!
 *  @brief This API validates the ODR value set by the user.
 */
static uint16_t validate_odr(uint8_t odr)
{
    uint16_t rslt = BMA4_OK;

    if ((odr < BMA4_OUTPUT_DATA_RATE_0_78HZ) || (odr > BMA4_OUTPUT_DATA_RATE_1600HZ)) {
        /* If odr is not valid return error */
        rslt = BMA4_E_OUT_OF_RANGE;
    }

    return rslt;
}

/*!
 *  @brief This API sets the advance power save mode in the sensor.
 */
uint16_t bma4_set_advance_power_save(uint8_t adv_pwr_save, struct bma4_dev *dev)
{
    uint16_t rslt = BMA4_OK;
    uint8_t data = 0;

    /* Check the bma4 structure as NULL */
    if (dev == NULL) {
        rslt |= BMA4_E_NULL_PTR;
    } else {
        rslt |= bma4_read_regs(BMA4_POWER_CONF_ADDR, &data, 1, dev);
        if (rslt == BMA4_OK) {
            data = BMA4_SET_BITS_POS_0(data, BMA4_ADVANCE_POWER_SAVE, adv_pwr_save);
            rslt |= bma4_write_regs(BMA4_POWER_CONF_ADDR, &data, 1, dev);
        }
    }

    return rslt;
}

/*!
 *  @brief This API reads the status of advance power save mode
 *  from the sensor.
 */
uint16_t bma4_get_advance_power_save(uint8_t *adv_pwr_save, struct bma4_dev *dev)
{
    uint16_t rslt = BMA4_OK;
    uint8_t data = 0;

    /* Check the bma4 structure as NULL */
    if (dev == NULL) {
        rslt |= BMA4_E_NULL_PTR;
    } else {
        rslt |= bma4_read_regs(BMA4_POWER_CONF_ADDR, &data, 1, dev);

        if (rslt == BMA4_OK)
            *adv_pwr_save = BMA4_GET_BITS_POS_0(data, BMA4_ADVANCE_POWER_SAVE);
    }

    return rslt;
}

/*!
 *  @brief This API sets the FIFO self wake up functionality in the sensor.
 */
uint16_t bma4_set_fifo_self_wakeup(uint8_t fifo_self_wakeup, struct bma4_dev *dev)
{
    uint16_t rslt = 0;
    uint8_t data = 0;

    /* Check the bma4 structure as NULL */
    if (dev == NULL) {
        rslt |= BMA4_E_NULL_PTR;
    } else {
        rslt |= bma4_read_regs(BMA4_POWER_CONF_ADDR, &data, 1, dev);

        if (rslt == BMA4_OK) {
            data = BMA4_SET_BITSLICE(data, BMA4_FIFO_SELF_WAKE_UP, fifo_self_wakeup);
            rslt |= bma4_write_regs(BMA4_POWER_CONF_ADDR, &data, 1, dev);
        }
    }

    return rslt;
}

/*!
 *  @brief This API gets the status of FIFO self wake up functionality from
 *  the sensor.
 */
uint16_t bma4_get_fifo_self_wakeup(uint8_t *fifo_self_wake_up, struct bma4_dev *dev)
{
    uint16_t rslt = 0;
    uint8_t data = 0;

    /* Check the bma4 structure as NULL */
    if (dev == NULL) {
        rslt |= BMA4_E_NULL_PTR;
    } else {
        rslt |= bma4_read_regs(BMA4_POWER_CONF_ADDR, &data, 1, dev);

        if (rslt == BMA4_OK)
            *fifo_self_wake_up = BMA4_GET_BITSLICE(data, BMA4_FIFO_SELF_WAKE_UP);
    }

    return rslt;
}

/*!
 *  @brief This API enables or disables the Accel in the sensor.
 */
uint16_t bma4_set_accel_enable(uint8_t accel_en, struct bma4_dev *dev)
{
    uint16_t rslt = 0;
    uint8_t data = 0;

    /* Check the bma4 structure as NULL */
    if (dev == NULL) {
        rslt |= BMA4_E_NULL_PTR;
    } else {
        rslt |= bma4_read_regs(BMA4_POWER_CTRL_ADDR, &data, 1, dev);

        if (rslt == BMA4_OK) {
            data = BMA4_SET_BITSLICE(data, BMA4_ACCEL_ENABLE, accel_en);
            rslt |= bma4_write_regs(BMA4_POWER_CTRL_ADDR, &data, 1, dev);
        }
    }

    return rslt;
}

/*!
 *  @brief This API checks whether Accel is enabled or not in the sensor.
 */
uint16_t bma4_get_accel_enable(uint8_t *accel_en, struct bma4_dev *dev)
{
    uint16_t rslt = 0;
    uint8_t data = 0;

    /* Check the bma4 structure as NULL */
    if (dev == NULL) {
        rslt |= BMA4_E_NULL_PTR;
    } else {
        rslt |= bma4_read_regs(BMA4_POWER_CTRL_ADDR, &data, 1, dev);

        if (rslt == BMA4_OK)
            *accel_en = BMA4_GET_BITSLICE(data, BMA4_ACCEL_ENABLE);
    }

    return rslt;
}

/*!
 *  @brief This API is used to enable or disable auxiliary Mag
 *  in the sensor.
 */
uint16_t bma4_set_mag_enable(uint8_t mag_en, struct bma4_dev *dev)
{
    uint16_t rslt = 0;
    uint8_t data = 0;

    /* Check the bma4 structure as NULL */
    if (dev == NULL) {
        rslt |= BMA4_E_NULL_PTR;
    } else {
        rslt |= bma4_read_regs(BMA4_POWER_CTRL_ADDR, &data, 1, dev);

        if (rslt == BMA4_OK) {
            data = BMA4_SET_BITS_POS_0(data, BMA4_MAG_ENABLE, mag_en);
            rslt |= bma4_write_regs(BMA4_POWER_CTRL_ADDR, &data, 1, dev);
        }
    }

    return rslt;
}

/*!
 *  @brief This API is used to check whether the auxiliary Mag is enabled
 *  or not in the sensor.
 */
uint16_t bma4_get_mag_enable(uint8_t *mag_en, struct bma4_dev *dev)
{
    uint16_t rslt = 0;
    uint8_t data = 0;

    /* Check the bma4 structure as NULL */
    if (dev == NULL) {
        rslt |= BMA4_E_NULL_PTR;
    } else {
        rslt |= bma4_read_regs(BMA4_POWER_CTRL_ADDR, &data, 1, dev);

        if (rslt == BMA4_OK)
            *mag_en = BMA4_GET_BITS_POS_0(data, BMA4_MAG_ENABLE);
    }

    return rslt;
}

/*!
 *  @brief This API reads the SPI interface mode which is set for primary
 *  interface.
 */
uint16_t bma4_get_spi_interface(uint8_t *spi, struct bma4_dev *dev)
{
    uint16_t rslt  = 0;
    uint8_t data = 0;

    /* Check the bma4 structure as NULL */
    if (dev == NULL) {
        rslt |= BMA4_E_NULL_PTR;
    } else {
        /* Read SPI mode */
        rslt |= bma4_read_regs(BMA4_IF_CONFIG_ADDR, &data, 1, dev);

        if (rslt == BMA4_OK)
            *spi = BMA4_GET_BITS_POS_0(data, BMA4_CONFIG_SPI3);
    }

    return rslt;
}

/*!
 *  @brief This API configures the SPI interface Mode for primary interface
 */
uint16_t bma4_set_spi_interface(uint8_t spi, struct bma4_dev *dev)
{
    uint16_t rslt = 0;
    uint8_t data = 0;

    /* Check the bma4 structure as NULL */
    if (dev == NULL) {
        rslt |= BMA4_E_NULL_PTR;
    } else {
        if (spi <= BMA4_MAX_VALUE_SPI3) {
            /* Write SPI mode */
            rslt |= bma4_read_regs(BMA4_IF_CONFIG_ADDR, &data, 1, dev);

            if (rslt == BMA4_OK) {
                data = BMA4_SET_BITS_POS_0(data, BMA4_CONFIG_SPI3, spi);
                rslt |= bma4_write_regs(BMA4_IF_CONFIG_ADDR, &data, 1, dev);
            }
        } else {
            rslt |= BMA4_E_OUT_OF_RANGE;
        }
    }

    return rslt;
}

/*!
*  @brief This API writes the available sensor specific commands
*  to the sensor.
*/
uint16_t bma4_set_command_register(uint8_t command_reg, struct bma4_dev *dev)
{
    uint16_t rslt  = 0;

    /* Check the bma4 structure as NULL */
    if (dev == NULL) {
        rslt |= BMA4_E_NULL_PTR;
    } else {
        /* Write command register */
        rslt |= bma4_write_regs(BMA4_CMD_ADDR, &command_reg, 1, dev);
    }

    return rslt;
}

/*!
 * @brief This API sets the I2C device address of auxiliary sensor
 */
uint16_t bma4_set_i2c_device_addr(struct bma4_dev *dev)
{
    uint16_t rslt = 0;
    uint8_t data = 0, dev_id = 0;

    /* Check the bma4 structure as NULL */
    if (dev == NULL) {
        rslt |= BMA4_E_NULL_PTR;
    } else {
        /* Write the auxiliary I2C device address */
        rslt |= bma4_read_regs(BMA4_AUX_DEV_ID_ADDR, &data, 1, dev);

        if (rslt == BMA4_OK) {
            dev_id = BMA4_SET_BITSLICE(data, BMA4_I2C_DEVICE_ADDR, dev->aux_config.aux_dev_addr);
            rslt |= bma4_write_regs(BMA4_AUX_DEV_ID_ADDR, &dev_id, 1, dev);
        }
    }

    return rslt;
}

/*!
 *  @brief This API sets the register access on MAG_IF[2], MAG_IF[3],
 *  MAG_IF[4] in the sensor. This implies that the DATA registers are
 *  not updated with Mag values automatically.
 */
uint16_t bma4_set_mag_manual_enable(uint8_t mag_manual, struct bma4_dev *dev)
{
    uint16_t rslt = 0;
    uint8_t data = 0;
    /* Check the bma4 structure as NULL */
    if (dev == NULL) {
        rslt |= BMA4_E_NULL_PTR;
    } else {
        /* Write the Mag manual*/
        rslt |= bma4_read_regs(BMA4_AUX_IF_CONF_ADDR, &data, 1, dev);
        dev->delay(BMA4_GEN_READ_WRITE_DELAY);

        if (rslt == BMA4_OK) {
            /* Set the bit of Mag manual enable */
            data = BMA4_SET_BITSLICE(data, BMA4_MAG_MANUAL_ENABLE, mag_manual);
            rslt |= bma4_write_regs(BMA4_AUX_IF_CONF_ADDR, &data, 1, dev);

            if (rslt == BMA4_OK)
                dev->aux_config.manual_enable = (uint8_t)mag_manual;
        } else {
            /*dev->mag_manual_enable = 0;*/
            dev->aux_config.manual_enable = 0;
        }
    }

    return rslt;
}

/*!
 *  @brief This API checks whether the Mag access is done manually or
 *  automatically in the sensor.
 *  If the Mag access is done through manual mode then Mag data registers
 *  in sensor are not updated automatically.
 */
uint16_t bma4_get_mag_manual_enable(uint8_t *mag_manual, struct bma4_dev *dev)
{
    uint16_t rslt = 0;
    uint8_t data = 0;

    /* Check the bma4 structure as NULL */
    if (dev == NULL) {
        rslt |= BMA4_E_NULL_PTR;
    } else {
        /* Read Mag manual */
        rslt |= bma4_read_regs(BMA4_AUX_IF_CONF_ADDR, &data, 1, dev);

        if (rslt == BMA4_OK)
            *mag_manual = BMA4_GET_BITSLICE(data, BMA4_MAG_MANUAL_ENABLE);
    }

    return rslt;
}

/*!
 *  @brief This API sets the I2C interface configuration(if) mode
 *  for auxiliary Mag.
 */
uint16_t bma4_set_aux_if_mode(uint8_t if_mode, struct bma4_dev *dev)
{
    uint16_t rslt = 0;
    uint8_t data = 0;

    /* Check the bma4 structure as NULL */
    if (dev == NULL) {
        rslt |= BMA4_E_NULL_PTR;
    } else {
        rslt |= bma4_read_regs(BMA4_IF_CONFIG_ADDR, &data, 1, dev);

        if (rslt == BMA4_OK) {
            data = BMA4_SET_BITSLICE(data, BMA4_IF_CONFIG_IF_MODE, if_mode);
            rslt |= bma4_write_regs(BMA4_IF_CONFIG_ADDR, &data, 1, dev);
        }
    }

    return rslt;
}

/*!
 *  @brief This API gets the address of the register of Aux Mag sensor
 *  where the data to be read.
 */
uint16_t bma4_get_mag_read_addr(uint8_t *mag_read_addr, struct bma4_dev *dev)
{
    uint16_t rslt = 0;
    uint8_t data = 0;

    /* Check the bma4 structure as NULL */
    if (dev == NULL) {
        rslt |= BMA4_E_NULL_PTR;
    } else {
        rslt |= bma4_read_regs(BMA4_AUX_RD_ADDR, &data, 1, dev);

        if (rslt == BMA4_OK)
            *mag_read_addr = BMA4_GET_BITS_POS_0(data, BMA4_READ_ADDR);
    }

    return rslt;
}

/*!
 *  @brief This API sets the address of the register of Aux Mag sensor
 *  where the data to be read.
 */
uint16_t bma4_set_mag_read_addr(uint8_t mag_read_addr, struct bma4_dev *dev)
{
    uint16_t rslt = 0;

    /* Check the bma4 structure as NULL */
    if (dev == NULL) {
        rslt |= BMA4_E_NULL_PTR;
    } else {
        /* Write the Mag read address*/
        rslt |= bma4_write_regs(BMA4_AUX_RD_ADDR, &mag_read_addr, 1, dev);
    }

    return rslt;
}

/*!
 *  @brief This API gets the Aux Mag write address from the sensor.
 *  Mag write address is where the Mag data will be written.
 */
uint16_t bma4_get_mag_write_addr(uint8_t *mag_write_addr, struct bma4_dev *dev)
{
    uint16_t rslt = 0;
    uint8_t data = 0;

    /* Check the bma4 structure as NULL */
    if (dev == NULL) {
        rslt |= BMA4_E_NULL_PTR;
    } else {
        rslt |= bma4_read_regs(BMA4_AUX_WR_ADDR, &data, 1, dev);

        if (rslt == BMA4_OK)
            *mag_write_addr = BMA4_GET_BITS_POS_0(data, BMA4_WRITE_ADDR);
    }

    return rslt;
}

/*!
 *  @brief This API sets the Aux Mag write address in the sensor.
 *  Mag write address is where the Mag data will be written.
 */
uint16_t bma4_set_mag_write_addr(uint8_t mag_write_addr, struct bma4_dev *dev)
{
    uint16_t rslt = 0;

    /* Check the bma4 structure as NULL */
    if (dev == NULL)
        rslt |= BMA4_E_NULL_PTR;
    else
        rslt |= bma4_write_regs(BMA4_AUX_WR_ADDR, &mag_write_addr, 1, dev);

    return rslt;
}

/*!
 *  @brief This API reads the data from the sensor which is written to the
 *  Mag.
 */
uint16_t bma4_get_mag_write_data(uint8_t *mag_write_data, struct bma4_dev *dev)
{
    uint16_t rslt = 0;
    uint8_t data = 0;

    /* Check the bma4 structure as NULL */
    if (dev == NULL) {
        rslt |= BMA4_E_NULL_PTR;
    } else {
        rslt |= bma4_read_regs(BMA4_AUX_WR_DATA_ADDR, &data, 1, dev);

        if (rslt == BMA4_OK)
            *mag_write_data = BMA4_GET_BITS_POS_0(data, BMA4_WRITE_DATA);
    }

    return rslt;
}

/*!
 *  @brief This API sets the data in the sensor which in turn will
 *  be written to Mag.
 */
uint16_t bma4_set_mag_write_data(uint8_t mag_write_data, struct bma4_dev *dev)
{
    uint16_t rslt = 0;

    /* Check the bma4 structure as NULL */
    if (dev == NULL)
        rslt |= BMA4_E_NULL_PTR;
    else
        rslt |= bma4_write_regs(BMA4_AUX_WR_DATA_ADDR, &mag_write_data, 1, dev);

    return rslt;
}

/*!
 *  @brief This API reads the x,y,z and r axis data from the auxiliary
 *  Mag BMM150/AKM9916 sensor.
 */
uint16_t bma4_read_mag_xyzr(struct bma4_mag_xyzr *mag, struct bma4_dev *dev)
{
    uint16_t rslt = 0;
    uint16_t msb = 0;
    uint16_t lsb = 0;
    uint8_t data[BMA4_MAG_XYZR_DATA_LENGTH] = {0};

    /* Check the bma4 structure as NULL */
    if (dev == NULL) {
        rslt |= BMA4_E_NULL_PTR;
    } else {
        rslt |= bma4_read_regs(BMA4_DATA_0_ADDR, data, BMA4_MAG_XYZR_DATA_LENGTH, dev);

        if (rslt == BMA4_OK) {
            /* Data X */
            /* X-axis LSB value shifting */
            lsb = BMA4_GET_BITSLICE(data[BMA4_MAG_X_LSB_BYTE], BMA4_DATA_MAG_X_LSB);
            msb = data[BMA4_MAG_X_MSB_BYTE];
            mag->x = (int16_t)((msb << 8) | lsb);
            mag->x = (mag->x / 0x08);

            /* Data Y */
            /* Y-axis LSB value shifting */
            lsb = BMA4_GET_BITSLICE(data[BMA4_MAG_Y_LSB_BYTE], BMA4_DATA_MAG_Y_LSB);
            msb = data[BMA4_MAG_Y_MSB_BYTE];
            mag->y = (int16_t)((msb << 8) |  lsb);
            mag->y = (mag->y / 0x08);

            /* Data Z */
            /* Z-axis LSB value shifting */
            lsb = BMA4_GET_BITSLICE(data[BMA4_MAG_Z_LSB_BYTE], BMA4_DATA_MAG_Z_LSB);
            msb = data[BMA4_MAG_Z_MSB_BYTE];
            mag->z = (int16_t)((msb << 8) | lsb);
            mag->z = (mag->z / 0x02);

            /* RHall */
            /* R-axis LSB value shifting */
            lsb = BMA4_GET_BITSLICE(data[BMA4_MAG_R_LSB_BYTE], BMA4_DATA_MAG_R_LSB);
            msb = data[BMA4_MAG_R_MSB_BYTE];
            mag->r = (int16_t)((msb << 8) | lsb);
            mag->r = (mag->r / 0x04);
        }
    }

    return rslt;
}

/*!
 *  @brief This API sets the burst data length (1,2,6,8 byte) of auxiliary
 *  Mag sensor.
 */
uint16_t bma4_set_mag_burst(uint8_t mag_burst, struct bma4_dev *dev)
{
    uint16_t rslt = 0;
    uint8_t data = 0;

    /* Check the bma4 structure as NULL */
    if (dev == NULL) {
        rslt |= BMA4_E_NULL_PTR;
    } else {
        /* Write auxiliary burst mode length*/
        rslt |= bma4_read_regs(BMA4_AUX_IF_CONF_ADDR, &data, 1, dev);

        if (rslt == BMA4_OK) {
            data = BMA4_SET_BITS_POS_0(data, BMA4_MAG_BURST, mag_burst);
            rslt |= bma4_write_regs(BMA4_AUX_IF_CONF_ADDR, &data, 1, dev);
        }
    }

    return rslt;
}

/*!
 *  @brief This API reads the burst data length of Mag set in the sensor.
 */
uint16_t bma4_get_mag_burst(uint8_t *mag_burst, struct bma4_dev *dev)
{
    uint16_t rslt = 0;
    uint8_t data = 0;

    /* Check the bma4 structure as NULL */
    if (dev == NULL) {
        rslt |= BMA4_E_NULL_PTR;
    } else {
        /* Write Mag burst mode length*/
        rslt |= bma4_read_regs(BMA4_AUX_IF_CONF_ADDR, &data, 1, dev);

        if (rslt == BMA4_OK)
            *mag_burst = BMA4_GET_BITS_POS_0(data, BMA4_MAG_BURST);
    }

    return rslt;
}

/*!
 *  @brief This API reads the FIFO data of Accel and/or Mag sensor
 */
uint16_t bma4_read_fifo_data(struct bma4_dev *dev)
{
    uint16_t rslt = 0;
    uint8_t data = 0;
    uint8_t addr = BMA4_FIFO_DATA_ADDR;
    /* check the bma4 structure as NULL*/
    if (dev == NULL || dev->fifo == NULL || dev->fifo->data == NULL) {
        rslt |= BMA4_E_NULL_PTR;
    } else {
        reset_fifo_data_structure(dev);
        /* read FIFO data*/
        if (dev->interface == BMA4_SPI_INTERFACE)
            addr = addr | BMA4_SPI_RD_MASK;

        rslt |= dev->bus_read(dev->dev_addr, addr, dev->fifo->data, dev->fifo->length);
        /* read fifo frame content configuration*/
        rslt |= bma4_read_regs(BMA4_FIFO_CONFIG_1_ADDR, &data, 1, dev);
        /* filter fifo header enabled status */
        dev->fifo->fifo_header_enable = data & BMA4_FIFO_HEADER;
        /* filter accel/mag data enabled status */
        dev->fifo->fifo_data_enable = data & BMA4_FIFO_M_A_ENABLE;
    }

    return rslt;
}

/*!
 *  @brief This API parses and extracts the accelerometer frames from
 *  FIFO data read by the "bma4_read_fifo_data" API and stores it in the
 *  "accel_data" structure instance.
 */
uint16_t bma4_extract_accel(struct bma4_accel *accel_data, uint16_t *accel_length, const struct bma4_dev *dev)
{
    uint16_t rslt = 0;
    uint16_t data_index = 0;
    uint16_t accel_index = 0;
    uint16_t data_read_length = 0;

    if (dev == NULL || dev->fifo == NULL || dev->fifo->data == NULL) {
        rslt |= BMA4_E_NULL_PTR;
    } else {
        /* Parsing the FIFO data in header-less mode */
        if (dev->fifo->fifo_header_enable == 0) {
            get_accel_len_to_parse(&data_index, &data_read_length, accel_length, dev);

            for (; data_index < data_read_length;) {
                unpack_acc_frm(accel_data, &data_index, &accel_index, dev->fifo->fifo_data_enable, dev);
                /*Check for the availability of next
                two bytes of FIFO data */
                check_empty_fifo(&data_index, dev);
            }
            /* update number of accel data read*/
            *accel_length = accel_index;
            /*update the accel byte index*/
            dev->fifo->accel_byte_start_idx = data_index;
        } else {
            /* Parsing the FIFO data in header mode */
            extract_accel_header_mode(accel_data, accel_length, dev);
        }
    }

    return rslt;
}

/*!
 *  @brief This API parses and extracts the magnetometer frames from
 *  FIFO data read by the "bma4_read_fifo_data" API and stores it in the
 *  "mag_data" structure instance parameter of this API
 */
uint16_t bma4_extract_mag(struct bma4_mag *mag_data, uint16_t *mag_length, const struct bma4_dev *dev)
{
    uint16_t rslt = 0;
    uint16_t data_index = 0;
    uint16_t mag_index = 0;
    uint16_t data_read_length = 0;

    if (dev == NULL || dev->fifo == NULL || dev->fifo->data == NULL) {
        rslt |= BMA4_E_NULL_PTR;
    } else {
        /* Parsing the FIFO data in header-less mode */
        if (dev->fifo->fifo_header_enable == 0) {
            get_mag_len_to_parse(&data_index, &data_read_length, mag_length, dev);
            for (; data_index < data_read_length;) {
                rslt |= unpack_mag_frm(mag_data, &data_index, &mag_index,
                                       dev->fifo->fifo_data_enable, dev);
                /*Check for the availability of next
                two bytes of FIFO data */
                check_empty_fifo(&data_index, dev);
            }
            /* update number of Aux. sensor data read*/
            *mag_length = mag_index;
            /*update the Aux. sensor frame index*/
            dev->fifo->mag_byte_start_idx = data_index;
        } else {
            /* Parsing the FIFO data in header mode */
            rslt |= extract_mag_header_mode(mag_data, mag_length, dev);
        }
    }

    return rslt;
}

/*!
 *  @brief This API reads the FIFO water mark level which is set
 *  in the sensor.
 */
uint16_t bma4_get_fifo_wm(uint16_t *fifo_wm, struct bma4_dev *dev)
{
    uint16_t rslt = 0;
    uint8_t data[2] = {0, 0};

    /* Check the bma4  structure as NULL*/
    if (dev == NULL) {
        rslt |= BMA4_E_NULL_PTR;
    } else {
        /* Read the FIFO water mark level*/
        rslt |= bma4_read_regs(BMA4_FIFO_WTM_0_ADDR, data, BMA4_FIFO_WM_LENGTH, dev);

        if (BMA4_OK == rslt)
            *fifo_wm = (data[1] << 8) | (data[0]);
    }

    return rslt;
}

/*!
 *  @brief This API sets the FIFO watermark level in the sensor.
 */
uint16_t bma4_set_fifo_wm(uint16_t fifo_wm, struct bma4_dev *dev)
{
    uint16_t rslt = 0;
    uint8_t data[2] = {0, 0};

    /* Check the bma4  structure as NULL*/
    if (dev == NULL) {
        rslt |= BMA4_E_NULL_PTR;
    } else {

        data[0] = BMA4_GET_LSB(fifo_wm);
        data[1] = BMA4_GET_MSB(fifo_wm);
        /* consecutive write is not possible in suspend mode hence
             separate write is used with delay of 1 ms*/
        /* Write the fifo watermark level*/
        rslt |= bma4_write_regs(BMA4_FIFO_WTM_0_ADDR, &data[0], 1, dev);
        dev->delay(BMA4_GEN_READ_WRITE_DELAY);
        rslt |= bma4_write_regs((BMA4_FIFO_WTM_0_ADDR + 1), &data[1], 1, dev);
    }

    return rslt;
}

/*!
 *  @brief This API checks whether the Accel FIFO data is set for filtered
 *  or unfiltered mode.
 */
uint16_t bma4_get_accel_fifo_filter_data(uint8_t *accel_fifo_filter, struct bma4_dev *dev)
{
    uint16_t rslt = 0;
    uint8_t data = 0;

    /* Check the bma4 structure as NULL */
    if (dev == NULL) {
        rslt |= BMA4_E_NULL_PTR;
    } else {
        /* Read the Accel FIFO filter data */
        rslt |= bma4_read_regs(BMA4_FIFO_DOWN_ADDR, &data, 1, dev);

        if (rslt == BMA4_OK)
            *accel_fifo_filter = BMA4_GET_BITSLICE(data, BMA4_FIFO_FILTER_ACCEL);
    }

    return rslt;
}

/*!
 *  @brief This API sets the condition of Accel FIFO data either to
 *  filtered or unfiltered mode.
 */
uint16_t bma4_set_accel_fifo_filter_data(uint8_t accel_fifo_filter, struct bma4_dev *dev)
{
    uint16_t rslt = 0;
    uint8_t data = 0;

    /* Check the bma4 structure as NULL */
    if (dev == NULL) {
        rslt |= BMA4_E_NULL_PTR;
    } else {
        if (accel_fifo_filter <= BMA4_MAX_VALUE_FIFO_FILTER) {
            rslt |= bma4_read_regs(BMA4_FIFO_DOWN_ADDR, &data, 1, dev);

            if (rslt == BMA4_OK) {
                /* Write Accel FIFO filter data */
                data = BMA4_SET_BITSLICE(data, BMA4_FIFO_FILTER_ACCEL, accel_fifo_filter);
                rslt |= bma4_write_regs(BMA4_FIFO_DOWN_ADDR, &data, 1, dev);
            }
        } else {
            rslt |= BMA4_E_OUT_OF_RANGE;
        }
    }
    return rslt;
}

/*!
 *  @brief This API reads the down sampling rates which is configured
 *  for Accel FIFO data.
 */
uint16_t bma4_get_fifo_down_accel(uint8_t *fifo_down, struct bma4_dev *dev)
{
    uint16_t rslt = 0;
    uint8_t data = 0;


    /* Check the bma4 structure as NULL */
    if (dev == NULL) {
        rslt |= BMA4_E_NULL_PTR;
    } else {
        /* Read the Accel FIFO down data */
        rslt |= bma4_read_regs(BMA4_FIFO_DOWN_ADDR, &data, 1, dev);

        if (rslt == BMA4_OK)
            *fifo_down = BMA4_GET_BITSLICE(data, BMA4_FIFO_DOWN_ACCEL);
    }

    return rslt;
}

/*!
 *  @brief This API sets the down-sampling rates for Accel FIFO.
 */
uint16_t bma4_set_fifo_down_accel(uint8_t fifo_down, struct bma4_dev *dev)
{
    uint16_t rslt = 0;
    uint8_t data = 0;

    /* Check the bma4 structure as NULL */
    if (dev == NULL) {
        rslt |= BMA4_E_NULL_PTR;
    } else {
        /* Write the Accel FIFO down data */
        rslt |= bma4_read_regs(BMA4_FIFO_DOWN_ADDR, &data, 1, dev);

        if (rslt == BMA4_OK) {
            data = BMA4_SET_BITSLICE(data, BMA4_FIFO_DOWN_ACCEL, fifo_down);
            rslt |= bma4_write_regs(BMA4_FIFO_DOWN_ADDR, &data, 1, dev);
        }
    }
    return rslt;
}

/*!
 *  @brief This API reads the length of FIFO data available in the sensor
 *  in the units of bytes.
 */
uint16_t bma4_get_fifo_length(uint16_t *fifo_length, struct bma4_dev *dev)
{
    uint16_t rslt = 0;
    uint8_t index = 0;
    uint8_t data[BMA4_FIFO_DATA_LENGTH] = {0, 0};

    /* Check the bma4 structure as NULL */
    if (dev == NULL) {
        rslt |= BMA4_E_NULL_PTR;
    } else {
        /* Read FIFO length*/
        rslt |= bma4_read_regs(BMA4_FIFO_LENGTH_0_ADDR, data, BMA4_FIFO_DATA_LENGTH, dev);

        if (rslt == BMA4_OK) {
            index = BMA4_FIFO_LENGTH_MSB_BYTE;
            data[index] = BMA4_GET_BITS_POS_0(data[index], BMA4_FIFO_BYTE_COUNTER_MSB);
            *fifo_length = ((data[index] << 8) | data[index - 1]);
        }
    }

    return rslt;
}

/*!
 *  @brief This API aligns and compensates the Mag data of BMM150/AKM9916
 *  sensor.
 */
uint16_t bma4_second_if_mag_compensate_xyz(struct bma4_mag_fifo_data mag_fifo_data,
        uint8_t mag_second_if, struct bma4_mag *compensated_mag_data)
{
    uint16_t rslt = 0;
#ifdef BMM150
    int16_t mag_x = 0;
    int16_t mag_y = 0;
    int16_t mag_z = 0;
    uint16_t mag_r = 0;
#endif

    switch (mag_second_if) {
#ifdef BMM150
    case BMA4_SEC_IF_BMM150:
        /* X data*/
        mag_x = (int16_t)((mag_fifo_data.mag_x_msb << 8) | (mag_fifo_data.mag_x_lsb));
        mag_x = (int16_t) (mag_x / 0x08);

        /* Y data*/
        mag_y = (int16_t)((mag_fifo_data.mag_y_msb << 8) | (mag_fifo_data.mag_y_lsb));
        mag_y = (int16_t) (mag_y / 0x08);

        /* Z data*/
        mag_z = (int16_t)((mag_fifo_data.mag_z_msb << 8) | (mag_fifo_data.mag_z_lsb));
        mag_z = (int16_t) (mag_z / 0x02);

        /* R data*/
        mag_r = (uint16_t)((mag_fifo_data.mag_r_y2_msb << 8) | (mag_fifo_data.mag_r_y2_lsb));
        mag_r = (uint16_t) (mag_r >> 2);

        /* Compensated Mag x data */
        compensated_mag_data->x = bma4_bmm150_mag_compensate_X(mag_x, mag_r);

        /* Compensated Mag y data */
        compensated_mag_data->y = bma4_bmm150_mag_compensate_Y(mag_y, mag_r);

        /* Compensated Mag z data */
        compensated_mag_data->z = bma4_bmm150_mag_compensate_Z(mag_z, mag_r);
        break;
#endif

#ifdef AKM9916

    case BMA4_SEC_IF_AKM09916:
        /* Compensated X data */
        compensated_mag_data->x = (int16_t)((mag_fifo_data.mag_x_msb << 8) | (mag_fifo_data.mag_x_lsb));
        /* Compensated Y data*/
        compensated_mag_data->y = (int16_t)((mag_fifo_data.mag_y_msb << 8) | (mag_fifo_data.mag_y_lsb));
        /* Compensated Z data*/
        compensated_mag_data->z = (int16_t)((mag_fifo_data.mag_z_msb << 8) | (mag_fifo_data.mag_z_lsb));
        break;

#endif
    default:
        rslt |= BMA4_E_OUT_OF_RANGE;
        break;
    }
    return rslt;
}

/*!
 *  @brief This API reads Mag. x,y and z axis data from either BMM150 or
 *  AKM9916 sensor
 */
uint16_t bma4_read_mag_xyz(struct bma4_mag *mag, uint8_t sensor_select, struct bma4_dev *dev)
{
    uint16_t rslt = 0;

#if defined(AKM9916) || defined(BMM150)
    uint8_t index;
    uint16_t msb = 0;
    uint16_t lsb = 0;
    uint8_t data[BMA4_MAG_XYZ_DATA_LENGTH] = {0};
#endif

    /* Check the bma4 structure as NULL */
    if (dev == NULL) {
        rslt |= BMA4_E_NULL_PTR;
    } else {
        switch (sensor_select) {

#if defined(BMM150)

        case BMA4_SEC_IF_BMM150:
            rslt |= bma4_read_regs(BMA4_DATA_0_ADDR, data, BMA4_MAG_XYZ_DATA_LENGTH, dev);

            if (rslt == BMA4_OK) {
                index = BMA4_MAG_X_LSB_BYTE;
                /*X-axis LSB value shifting*/
                data[index] = BMA4_GET_BITSLICE(data[index], BMA4_DATA_MAG_X_LSB);
                /* Data X */
                msb = data[index + 1];
                lsb = data[index];
                mag->x = (int16_t)((msb << 8) | lsb);
                mag->x = (mag->x / 0x08);

                /* Data Y */
                /*Y-axis LSB value shifting*/
                data[index + 2] = BMA4_GET_BITSLICE(data[index + 2], BMA4_DATA_MAG_Y_LSB);
                msb = data[index + 3];
                lsb = data[index + 2];
                mag->y = (int16_t)((msb << 8) | lsb);
                mag->y = (mag->y / 0x08);

                /* Data Z */
                /*Z-axis LSB value shifting*/
                data[index + 4] = BMA4_GET_BITSLICE(data[index + 4], BMA4_DATA_MAG_Z_LSB);
                msb = data[index + 5];
                lsb = data[index + 4];
                mag->z = (int16_t)((msb << 8) | lsb);
                mag->z = (mag->z / 0x02);
            }
            break;
#endif

#if defined(AKM9916)
        case BMA4_SEC_IF_AKM09916:

            if (AKM9916_SENSOR == dev->aux_sensor) {
                rslt |= bma4_read_regs(BMA4_DATA_0_ADDR, data, BMA4_MAG_XYZ_DATA_LENGTH, dev);

                if (rslt == BMA4_OK) {
                    index = BMA4_MAG_X_LSB_BYTE;
                    /* Data X */
                    msb = data[index + 1];
                    lsb = data[index];
                    mag->x = (int16_t)((msb << 8) | lsb);

                    /* Data Y */
                    msb = data[index + 3];
                    lsb = data[index + 2];
                    mag->y  = (int32_t)((msb << 8) | lsb);

                    /* Data Z */
                    msb = data[index + 5];
                    lsb = data[index + 4];
                    mag->z = (int16_t)((msb << 8) | lsb);
                }
            }
            break;
#endif

        default:
            rslt |= BMA4_E_OUT_OF_RANGE;
            break;
        }
    }

    return rslt;
}

/*!
 *  @brief This API reads the auxiliary I2C interface configuration which
 *  is set in the sensor.
 */
uint16_t bma4_get_if_mode(uint8_t *if_mode, struct bma4_dev *dev)
{
    uint16_t rslt  = 0;
    uint8_t data = 0;

    /* Check the bma4 structure as NULL */
    if (dev == NULL) {
        rslt |= BMA4_E_NULL_PTR;
    } else {
        /* Read auxiliary interface configuration */
        rslt |= bma4_read_regs(BMA4_IF_CONFIG_ADDR, &data, 1, dev);

        if (rslt == BMA4_OK)
            *if_mode = BMA4_GET_BITSLICE(data, BMA4_IF_CONFIG_IF_MODE);
    }

    return rslt;
}

/*!
 *  @brief This API sets the auxiliary interface configuration in the
 *  sensor.
 */
uint16_t bma4_set_if_mode(uint8_t if_mode, struct bma4_dev *dev)
{
    uint16_t rslt = 0;
    uint8_t data = 0;

    /* Check the bma4 structure as NULL */
    if (dev == NULL) {
        rslt |= BMA4_E_NULL_PTR;
    } else {
        if (if_mode <= BMA4_MAX_IF_MODE) {
            /* Write the interface configuration mode */
            rslt |= bma4_read_regs(BMA4_IF_CONFIG_ADDR, &data, 1, dev);
            if (rslt == BMA4_OK) {
                data = BMA4_SET_BITSLICE(data, BMA4_IF_CONFIG_IF_MODE, if_mode);
                rslt |= bma4_write_regs(BMA4_IF_CONFIG_ADDR, &data, 1, dev);
            }
        } else {
            rslt |= BMA4_E_OUT_OF_RANGE;
        }
    }

    return rslt;
}

/*!
 *  @brief This API reads the data ready status of Accel from the sensor.
 */
uint16_t bma4_get_accel_data_rdy(uint8_t *data_rdy, struct bma4_dev *dev)
{
    uint16_t rslt = 0;
    uint8_t data = 0;

    /* Check the bma4 structure as NULL */
    if (dev == NULL) {
        rslt |= BMA4_E_NULL_PTR;
    } else {
        /*Reads the status of Accel data ready*/
        rslt |= bma4_read_regs(BMA4_STATUS_ADDR, &data, 1, dev);

        if (rslt == BMA4_OK)
            *data_rdy = BMA4_GET_BITSLICE(data, BMA4_STAT_DATA_RDY_ACCEL);
    }

    return rslt;
}

/*!
 *  @brief This API reads the data ready status of Mag from the sensor.
 *  The status get reset when Mag data register is read.
 */
uint16_t bma4_get_mag_data_rdy(uint8_t *data_rdy, struct bma4_dev *dev)
{
    uint16_t rslt = 0;
    uint8_t data = 0;

    /* Check the bma4 structure as NULL */
    if (dev == NULL) {
        rslt |= BMA4_E_NULL_PTR;
    } else {
        /*Reads the status of Accel data ready*/
        rslt |= bma4_read_regs(BMA4_STATUS_ADDR, &data, 1, dev);

        if (rslt == BMA4_OK)
            *data_rdy = BMA4_GET_BITSLICE(data, BMA4_STAT_DATA_RDY_MAG);
    }

    return rslt;
}

/*!
 *  @brief This API reads the ASIC status from the sensor.
 *  The status information is mentioned in the below table.
 */
uint16_t bma4_get_asic_status(struct bma4_asic_status *asic_status, struct bma4_dev *dev)
{
    uint16_t rslt = 0;
    uint8_t data = 0;

    /* Check the bma4 structure as NULL */
    if (dev == NULL) {
        rslt |= BMA4_E_NULL_PTR;
    } else {
        /* Read the Mag I2C device address*/
        rslt |= bma4_read_regs(BMA4_INTERNAL_ERROR, &data, 1, dev);

        if (rslt == BMA4_OK) {
            asic_status->sleep = (data & 0x01);
            asic_status->irq_ovrn = ((data & 0x02) >> 0x01);
            asic_status->wc_event = ((data & 0x04) >> 0x02);
            asic_status->stream_transfer_active = ((data & 0x08) >> 0x03);
        }
    }

    return rslt;
}

/*!
 *  @brief This API enables the offset compensation for filtered and
 *  unfiltered  Accel data.
 */
uint16_t bma4_set_offset_comp(uint8_t offset_en, struct bma4_dev *dev)
{
    uint16_t rslt = 0;
    uint8_t data = 0;

    /* Check the bma4 structure as NULL */
    if (dev == NULL) {
        rslt |= BMA4_E_NULL_PTR;
    } else {
        rslt |= bma4_read_regs(BMA4_NV_CONFIG_ADDR, &data, 1, dev);

        if (rslt == BMA4_OK) {
            /* Write Accel FIFO filter data */
            data = BMA4_SET_BITSLICE(data, BMA4_NV_ACCEL_OFFSET, offset_en);
            rslt |= bma4_write_regs(BMA4_NV_CONFIG_ADDR, &data, 1, dev);
        }
    }

    return rslt;
}

/*!
 *  @brief This API gets the status of Accel offset compensation
 */
uint16_t bma4_get_offset_comp(uint8_t *offset_en, struct bma4_dev *dev)
{
    uint16_t rslt = 0;
    uint8_t data = 0;

    /* Check the bma4 structure as NULL */
    if (dev == NULL) {
        rslt |= BMA4_E_NULL_PTR;
    } else {
        rslt |= bma4_read_regs(BMA4_NV_CONFIG_ADDR, &data, 1, dev);

        if (rslt == BMA4_OK) {
            /* Write Accel FIFO filter data */
            *offset_en = BMA4_GET_BITSLICE(data, BMA4_NV_ACCEL_OFFSET);
        }
    }

    return rslt;
}

/*!
 *  @brief This API performs Fast Offset Compensation for Accel.
 *
 *  @note The g-values to be passed to the parameter should be
 *  multiples of 1000000.
 */
uint16_t bma4_perform_accel_foc(const int32_t accel_g_value[3], struct bma4_dev *dev)
{
    uint16_t rslt = 0;
    struct bma4_accel accel_value[10] = { {0} };
    struct accel_offset offset = {0};
    struct offset_delta delta = { {0} };
    struct bma4_accel_config acc_conf = {0};
    uint8_t accel_en = 0;
    uint8_t adv_pwr_save = 0;
    uint8_t range = 0;
    uint16_t lsb_per_g = 0;
    struct accel_temp temp = {0};
    struct bma4_accel avg = {0};
    struct bma4_accel accel_data = {0};
    uint8_t i = 0;

    /* used to validate user input */
    rslt |= validate_user_input(accel_g_value);

    if (BMA4_OK ==  rslt) {
        /* Configure accel config, accel enable and
        advance power save for FOC */
        rslt |= foc_config(&acc_conf, &accel_en, &adv_pwr_save, dev);

        /*TO DO: Check for data ready status before
        reading accel values*/

        if (BMA4_OK == rslt) {
            /* Giving a delay of 20ms before reading accel data
            since odr is configured as 50Hz */
            for (i = 0; i < 10; i++) {
                dev->delay(20);
                rslt |= bma4_read_accel_xyz(&accel_value[i], dev);
                temp.x = temp.x + (int32_t)accel_value[i].x;
                temp.y = temp.y + (int32_t)accel_value[i].y;
                temp.z = temp.z + (int32_t)accel_value[i].z;
            }

            /* Take average of x, y and z data for lesser noise */
            avg.x = (int16_t)(temp.x / 10);
            avg.y = (int16_t)(temp.y / 10);
            avg.z = (int16_t)(temp.z / 10);

            /* Copy average value in another structure */
            accel_data = avg;

            if (BMA4_OK == rslt) {
                /* Get the exact range value */
                map_range(acc_conf.range, &range);
                /* Get LSB per bit given the range and resolution */
                lsb_per_g = (uint16_t)(power(2, dev->resolution) / (2 * range));
                /* Compensate accel data against gravity */
                comp_for_grvty(lsb_per_g, accel_g_value, &accel_data, &delta);
                /* scale according to offset register resolution*/
                scale_offset(dev->resolution, range, &delta, &offset);
                /* normalise the data with offset*/
                normalise_offset(&delta, &offset);

                /* offset values are written in the offset register */
                rslt |= bma4_write_regs(BMA4_OFFSET_0_ADDR, (uint8_t *)&offset.x, 1, dev);
                rslt |= bma4_write_regs(BMA4_OFFSET_1_ADDR, (uint8_t *)&offset.y, 1, dev);
                rslt |= bma4_write_regs(BMA4_OFFSET_2_ADDR, (uint8_t *)&offset.z, 1, dev);

                /* Enable offset compensation */
                rslt |= bma4_set_offset_comp(BMA4_ENABLE, dev);

                /* Set accel config, accel enable and advance power save */
                rslt |= bma4_set_accel_config(&acc_conf, dev);
                rslt |= bma4_set_accel_enable(accel_en, dev);
                rslt |= bma4_set_advance_power_save(adv_pwr_save, dev);
            } else {
                rslt |= BMA4_E_FOC_FAIL;
            }
        }
    }
    return rslt;
}

/*!
 *  @brief This API checks whether the self test functionality of the sensor
 *  is working or not.
 *  The following parameter of struct bma4_dev, should have the valid value before
 *  performing the Self test,
 *  1.  Variant and 2. Resolution
 */
uint16_t bma4_perform_accel_selftest(uint8_t *result, struct bma4_dev *dev)
{
    uint16_t rslt = 0;
    struct bma4_accel positive = {0};
    struct bma4_accel negative = {0};
    /*! Structure for difference of accel values in g*/
    struct selftest_delta_limit accel_data_diff = {0};
    /*! Structure for difference of accel values in mg*/
    struct selftest_delta_limit accel_data_diff_mg = {0};

    *result = BMA4_SELFTEST_FAIL;

    if (dev == NULL) {
        rslt |= BMA4_E_NULL_PTR;
    } else {

        rslt = set_accel_selftest_config(dev);
        dev->delay(20);
        rslt |= bma4_selftest_config(BMA4_ENABLE, dev);

        if (rslt == BMA4_OK) {
            dev->delay(100);
            rslt = bma4_read_accel_xyz(&positive, dev);
            rslt |= bma4_selftest_config(BMA4_DISABLE, dev);

            if (rslt == BMA4_OK) {
                dev->delay(100);
                rslt = bma4_read_accel_xyz(&negative, dev);

                accel_data_diff.x = ABS(positive.x) + ABS(negative.x);
                accel_data_diff.y = ABS(positive.y) + ABS(negative.y);
                accel_data_diff.z = ABS(positive.z) + ABS(negative.z);

                /*! Converting LSB of the differences of accel values to mg*/
                convert_lsb_g(&accel_data_diff, &accel_data_diff_mg, dev);
                /*! Validating self test for accel values in mg*/
                rslt |= validate_selftest(&accel_data_diff_mg, dev);

                if (rslt == BMA4_OK)
                    *result = BMA4_SELFTEST_PASS;

                /* Triggers a soft reset */
                rslt |= bma4_set_command_register(0xB6, dev);
                dev->delay(200);
            }
        }
    }

    return rslt;
}

/*!
 *  @brief This API performs the steps needed for Self test operation
 *  before reading the Accel Self test data.
 */
uint16_t bma4_selftest_config(uint8_t sign, struct bma4_dev *dev)
{
    uint16_t rslt = 0;

    rslt |= set_accel_selftest_enable(BMA4_ENABLE, dev);
    rslt |= set_accel_selftest_sign(sign, dev);

    /* Set self test amplitude based on variant */
    switch (dev->variant) {
    case BMA42X_VARIANT:
        /* Set self test amplitude to high for BMA42x */
        rslt |= set_accel_selftest_amp(BMA4_ENABLE, dev);
        break;

    case BMA45X_VARIANT:
        /* Set self test amplitude to low for BMA45x */
        rslt |= set_accel_selftest_amp(BMA4_DISABLE, dev);
        break;

    default:
        rslt = BMA4_E_INVALID_SENSOR;
        break;
    }

    return rslt;
}

/*!
 *  @brief API sets the interrupt to either interrupt1 or
 *  interrupt2 pin in the sensor.
 */
uint16_t bma4_map_interrupt(uint8_t int_line, uint16_t int_map, uint8_t enable, struct bma4_dev *dev)
{
    uint16_t rslt = 0;
    uint8_t data[3] = {0, 0, 0};
    uint8_t index[2] = {BMA4_INT_MAP_1_ADDR, BMA4_INT_MAP_2_ADDR};

    /* Check the bma4 structure as NULL */
    if (dev == NULL) {
        rslt |= BMA4_E_NULL_PTR;
    } else {

        rslt |= bma4_read_regs(BMA4_INT_MAP_1_ADDR, data, 3, dev);

        if (enable == TRUE) {
            /* Feature interrupt mapping */
            data[int_line] |= (uint8_t)(int_map & (0x00FF));
            /* Hardware interrupt mapping */
            if (int_line == BMA4_INTR2_MAP)
                data[2] |= (uint8_t)((int_map & (0xFF00)) >> 4);
            else
                data[2] |= (uint8_t)((int_map & (0xFF00)) >> 8);

            rslt |= bma4_write_regs(index[int_line], &data[int_line], 1, dev);
            rslt |= bma4_write_regs(BMA4_INT_MAP_DATA_ADDR, &data[2], 1, dev);

        } else {
            /* Feature interrupt un-mapping */
            data[int_line] &= (~(uint8_t)(int_map & (0x00FF)));
            /* Hardware interrupt un-mapping */
            if (int_line == BMA4_INTR2_MAP)
                data[2] &= (~(uint8_t)((int_map & (0xFF00)) >> 4));
            else
                data[2] &= (~(uint8_t)((int_map & (0xFF00)) >> 8));

            rslt |= bma4_write_regs(index[int_line], &data[int_line], 1, dev);
            rslt |= bma4_write_regs(BMA4_INT_MAP_DATA_ADDR, &data[2], 1, dev);

        }
    }

    return rslt;
}

/*!
 *  @brief This API sets the interrupt mode in the sensor.
 */
uint16_t bma4_set_interrupt_mode(uint8_t mode, struct bma4_dev *dev)
{
    uint16_t rslt = 0;

    /* Check the bma4 structure as NULL */
    if (dev == NULL) {
        rslt |= BMA4_E_NULL_PTR;
    } else {

        if (mode == BMA4_NON_LATCH_MODE || mode == BMA4_LATCH_MODE)
            rslt |= bma4_write_regs(BMA4_INTR_LATCH_ADDR, &mode, 1, dev);
        else
            rslt |= BMA4_E_OUT_OF_RANGE;
    }

    return rslt;
}

/*!
 *  @brief This API gets the interrupt mode which is set in the sensor.
 */
uint16_t bma4_get_interrupt_mode(uint8_t *mode, struct bma4_dev *dev)
{
    uint16_t rslt = 0;
    uint8_t data = 0;

    /* Check the bma4 structure as NULL */
    if (dev == NULL) {
        rslt |= BMA4_E_NULL_PTR;
    } else {
        rslt |= bma4_read_regs(BMA4_INTR_LATCH_ADDR, &data, 1, dev);
        *mode = data;
    }

    return rslt;

}

/*!
 *  @brief This API sets the auxiliary Mag(BMM150 or AKM9916) output data
 *  rate and offset.
 */
uint16_t bma4_set_aux_mag_config(const struct bma4_aux_mag_config *aux_mag, struct bma4_dev *dev)
{
    uint16_t rslt = 0;
    uint8_t data = 0;

    /* Check the bma4 structure as NULL */
    if (dev == NULL) {
        rslt |= BMA4_E_NULL_PTR;
    } else {
        if ((aux_mag->odr >= BMA4_OUTPUT_DATA_RATE_0_78HZ) &&
                (aux_mag->odr <= BMA4_OUTPUT_DATA_RATE_1600HZ)
                && ((aux_mag->offset & BMA4_MAG_CONFIG_OFFSET_MSK) == 0x00)) {
            data = (uint8_t)(aux_mag->odr |
                             ((aux_mag->offset <<
                               BMA4_MAG_CONFIG_OFFSET_POS)));
            rslt |= bma4_write_regs(BMA4_AUX_CONFIG_ADDR, &data, 1, dev);
        } else {
            rslt |= BMA4_E_OUT_OF_RANGE;
        }
    }

    return rslt;
}

/*!
 *  @brief This API reads the auxiliary Mag(BMM150 or AKM9916) output data
 *  rate and offset.
 */
uint16_t bma4_get_aux_mag_config(struct bma4_aux_mag_config *aux_mag, struct bma4_dev *dev)
{
    uint16_t rslt = 0;
    uint8_t data = 0;

    /* Check the bma4 structure as NULL */
    if (dev == NULL) {
        rslt |= BMA4_E_NULL_PTR;
    } else {
        rslt |= bma4_read_regs(BMA4_AUX_CONFIG_ADDR, &data, 1, dev);

        if (rslt == BMA4_OK) {
            aux_mag->odr = (data & 0x0F);
            aux_mag->offset = (data & BMA4_MAG_CONFIG_OFFSET_MSK) >> 4;
        }
    }

    return rslt;
}

/*! @brief This API sets the FIFO configuration in the sensor.
 */
uint16_t bma4_set_fifo_config(uint8_t config, uint8_t enable, struct bma4_dev *dev)
{
    uint16_t rslt = 0;
    uint8_t data[2] = {0, 0};
    uint8_t fifo_config_0 = config & BMA4_FIFO_CONFIG_0_MASK;

    /* Check the bma4 structure as NULL */
    if (dev == NULL) {
        rslt |= BMA4_E_NULL_PTR;
    } else {
        rslt |= bma4_read_regs(BMA4_FIFO_CONFIG_0_ADDR, data, BMA4_FIFO_CONFIG_LENGTH, dev);

        if (rslt == BMA4_OK) {

            if (fifo_config_0 > 0) {

                if (enable == TRUE)
                    data[0] = data[0] | fifo_config_0;
                else
                    data[0] = data[0] & (~fifo_config_0);
            }

            if (enable == TRUE)
                data[1] = data[1] | (config & BMA4_FIFO_CONFIG_1_MASK);
            else
                data[1] = data[1] & (~(config & BMA4_FIFO_CONFIG_1_MASK));

            /* Burst write is not possible in suspend mode hence
            separate write is used with delay of 1 ms*/
            rslt |= bma4_write_regs(BMA4_FIFO_CONFIG_0_ADDR, &data[0], 1, dev);
            dev->delay(BMA4_GEN_READ_WRITE_DELAY);
            rslt |= bma4_write_regs((BMA4_FIFO_CONFIG_0_ADDR + 1), &data[1], 1, dev);
        }
    }

    return rslt;
}

/*! @brief This API reads the FIFO configuration from the sensor.
 */
uint16_t bma4_get_fifo_config(uint8_t *fifo_config, struct bma4_dev *dev)
{
    uint16_t rslt = 0;
    uint8_t data[2] = {0, 0};

    /* Check the bma4 structure as NULL */
    if (dev == NULL) {
        rslt |= BMA4_E_NULL_PTR;
    } else {
        rslt |= bma4_read_regs(BMA4_FIFO_CONFIG_0_ADDR, data, BMA4_FIFO_CONFIG_LENGTH, dev);

        if (rslt == BMA4_OK)
            *fifo_config = ((uint8_t)((data[0] & BMA4_FIFO_CONFIG_0_MASK) | (data[1])));

    }

    return rslt;
}

/*! @brief This function sets the electrical behaviour of interrupt pin1 or
 *  pin2 in the sensor.
 */
uint16_t bma4_set_int_pin_config(const struct bma4_int_pin_config *int_pin_config, uint8_t int_line,
                                 struct bma4_dev *dev)
{
    uint16_t rslt = 0;
    uint8_t interrupt_address_array[2] = {BMA4_INT1_IO_CTRL_ADDR, BMA4_INT2_IO_CTRL_ADDR};
    uint8_t data = 0;

    /* Check the bma4 structure as NULL */
    if (dev == NULL) {
        rslt |= BMA4_E_NULL_PTR;
    } else {
        if (int_line <= 1) {
            data = ((uint8_t)((int_pin_config->edge_ctrl & BMA4_INT_EDGE_CTRL_MASK) |
                              ((int_pin_config->lvl << 1) & BMA4_INT_LEVEL_MASK) |
                              ((int_pin_config->od << 2) & BMA4_INT_OPEN_DRAIN_MASK) |
                              ((int_pin_config->output_en << 3) & BMA4_INT_OUTPUT_EN_MASK) |
                              ((int_pin_config->input_en << 4) & BMA4_INT_INPUT_EN_MASK)));

            rslt |= bma4_write_regs(interrupt_address_array[int_line],  &data, 1, dev);
        } else {
            rslt |= BMA4_E_INT_LINE_INVALID;
        }
    }

    return rslt;
}

/*! @brief This API reads the electrical behavior of interrupt pin1 or pin2
 *  from the sensor.
 */
uint16_t bma4_get_int_pin_config(struct bma4_int_pin_config *int_pin_config, uint8_t int_line, struct bma4_dev *dev)
{
    uint16_t rslt = 0;
    uint8_t interrupt_address_array[2] = {BMA4_INT1_IO_CTRL_ADDR, BMA4_INT2_IO_CTRL_ADDR};
    uint8_t data = 0;

    if (dev == NULL) {
        rslt |= BMA4_E_NULL_PTR;
    } else {
        if (int_line <= 1) {
            rslt |= bma4_read_regs(interrupt_address_array[int_line], &data, 1, dev);
            /* Assign interrupt configurations to the
            structure members*/
            if (rslt == BMA4_OK) {
                int_pin_config->edge_ctrl = data & BMA4_INT_EDGE_CTRL_MASK;
                int_pin_config->lvl = ((data & BMA4_INT_LEVEL_MASK) >> BMA4_INT_LEVEL_POS);
                int_pin_config->od = ((data & BMA4_INT_OPEN_DRAIN_MASK) >> BMA4_INT_OPEN_DRAIN_POS);
                int_pin_config->output_en = ((data & BMA4_INT_OUTPUT_EN_MASK)
                                             >> BMA4_INT_OUTPUT_EN_POS);
                int_pin_config->input_en = ((data & BMA4_INT_INPUT_EN_MASK) >> BMA4_INT_INPUT_EN_POS);
            }
        } else {
            rslt |= BMA4_E_INT_LINE_INVALID;
        }
    }

    return rslt;
}

/*!
 *  @brief This API reads the Feature and Hardware interrupt status from the sensor.
 */
uint16_t bma4_read_int_status(uint16_t *int_status, struct bma4_dev *dev)
{
    uint16_t rslt = 0;
    uint8_t data[2] = {0};

    /* Check the bma4 structure as NULL */
    if (dev == NULL) {
        rslt |= BMA4_E_NULL_PTR;
    } else {
        rslt |= bma4_read_regs(BMA4_INT_STAT_0_ADDR, data, 2, dev);

        if (rslt == BMA4_OK) {
            *int_status = data[0];
            *((uint8_t *)int_status + 1) = data[1];
        }
    }

    return rslt;
}

/*!
 *  @brief This API reads the Feature interrupt status from the sensor.
 */
uint16_t bma4_read_int_status_0(uint8_t *int_status_0, struct bma4_dev *dev)
{
    uint16_t rslt = BMA4_OK;

    /* Check the bma4 structure as NULL */
    if (dev == NULL) {
        /* Null pointer check */
        rslt = BMA4_E_NULL_PTR;
    } else {
        rslt = bma4_read_regs(BMA4_INT_STAT_0_ADDR, int_status_0, 1, dev);
    }

    return rslt;
}

/*!
 *  @brief This API reads the Hardware interrupt status from the sensor.
 */
uint16_t bma4_read_int_status_1(uint8_t *int_status_1, struct bma4_dev *dev)
{
    uint16_t rslt = BMA4_OK;

    /* Check the bma4 structure as NULL */
    if (dev == NULL) {
        /* Null pointer check */
        rslt = BMA4_E_NULL_PTR;
    } else {
        rslt = bma4_read_regs(BMA4_INT_STAT_1_ADDR, int_status_1, 1, dev);
    }

    return rslt;
}

/*!
 * @brief This API initializes the auxiliary interface to access
 * auxiliary sensor
 */
uint16_t bma4_aux_interface_init(struct bma4_dev *dev)
{
    /* Variable to return error codes */
    uint16_t rslt = BMA4_OK;

    /* Check for Null pointer error */
    rslt |= bma4_null_pointer_check(dev);
    if (rslt == BMA4_OK) {
        /* Set the auxiliary sensor configuration */
        rslt = bma4_set_aux_interface_config(dev);
        if (rslt != BMA4_OK)
            rslt = BMA4_E_AUX_CONFIG_FAIL;
    }

    return rslt;
}

/*!
 * @brief This API reads the data from the auxiliary sensor
*/
uint16_t bma4_aux_read(uint8_t aux_reg_addr, uint8_t *aux_data, uint16_t len, struct bma4_dev *dev)
{
    /* Variable to return error codes */
    uint16_t rslt = BMA4_OK;

    /* Check for Null pointer error */
    rslt |= bma4_null_pointer_check(dev);
    if (rslt == BMA4_OK) {
        /* Read the data from the data register in terms of
        user defined length  */
        rslt = bma4_extract_aux_data(aux_reg_addr, aux_data, len, dev);
    }

    return rslt;
}

/*!
 * @brief This API writes the data into the auxiliary sensor
*/
uint16_t bma4_aux_write(uint8_t aux_reg_addr, uint8_t *aux_data, uint16_t len, struct bma4_dev *dev)
{

    uint16_t rslt = BMA4_OK;

    /* Check for Null pointer error */
    rslt |= bma4_null_pointer_check(dev);
    if (rslt == BMA4_OK) {
        /* Write data in terms of user defined length  */
        if (len > 0) {
            while (len--) {
                /* First set data to write */
                rslt = bma4_write_regs(BMA4_AUX_WR_DATA_ADDR, aux_data, 1, dev);
                dev->delay(BMA4_AUX_COM_DELAY);
                if (rslt == BMA4_OK) {
                    /* Then set address to write */
                    rslt = bma4_write_regs(BMA4_AUX_WR_ADDR, &aux_reg_addr, 1, dev);
                    dev->delay(BMA4_AUX_COM_DELAY);

                    /* Increment data array and register address until
                     *  user-defined length is greater than 0 */
                    if ((rslt == BMA4_OK) && (len > 0)) {
                        aux_data++;
                        aux_reg_addr++;
                    }
                } else {
                    rslt = BMA4_E_FAIL;
                }
            }
        } else {
            rslt = BMA4_E_RD_WR_LENGTH_INVALID;
        }
    }

    return rslt;
}

/*****************************************************************************/
/* Static function definition */
/*!
 *  @brief This API converts lsb value of axes to mg for self-test *
 */
static void convert_lsb_g(const struct selftest_delta_limit *accel_data_diff,
                          struct selftest_delta_limit *accel_data_diff_mg,
                          const struct bma4_dev *dev)
{
    uint32_t lsb_per_g;
    /*! Range considered for self-test is 8g */
    uint8_t range = 8;

    /*! lsb_per_g for the respective resolution and 8g range*/
    lsb_per_g = (uint32_t)(power(2, dev->resolution) / (2 * range));
    /*! accel x value in mg */
    accel_data_diff_mg->x = (accel_data_diff->x / (int32_t)lsb_per_g) * 1000;
    /*! accel y value in mg */
    accel_data_diff_mg->y = (accel_data_diff->y / (int32_t)lsb_per_g) * 1000;
    /*! accel z value in mg */
    accel_data_diff_mg->z = (accel_data_diff->z / (int32_t)lsb_per_g) * 1000;
}

/*!
 *  @brief This API writes the config stream data in memory using burst mode
 *  @note index value should be even number.
 */
static uint16_t  stream_transfer_write(const uint8_t *stream_data, uint16_t index, struct bma4_dev *dev)
{
    uint16_t rslt = 0;
    uint8_t asic_msb = (uint8_t)((index / 2) >> 4);
    uint8_t asic_lsb = ((index / 2) & 0x0F);

    rslt |= bma4_write_regs(BMA4_RESERVED_REG_5B_ADDR, &asic_lsb, 1, dev);

    if (rslt == BMA4_OK) {
        rslt |= bma4_write_regs(BMA4_RESERVED_REG_5C_ADDR, &asic_msb, 1, dev);

        if (rslt == BMA4_OK)
            rslt |= write_regs(BMA4_FEATURE_CONFIG_ADDR, (uint8_t *)stream_data, dev->read_write_len, dev);
    }

    return rslt;
}

/*!
 *  @brief This API enables or disables the Accel Self test feature in the
 *  sensor.
 */
static uint16_t set_accel_selftest_enable(uint8_t accel_selftest_enable, struct bma4_dev *dev)
{
    uint16_t rslt = 0;
    uint8_t data = 0;
    /* Check the bma4 structure as NULL */
    if (dev == NULL) {
        rslt |= BMA4_E_NULL_PTR;
    } else {
        /* Read the self test register */
        rslt |= bma4_read_regs(BMA4_ACC_SELF_TEST_ADDR, &data, 1, dev);

        if (rslt == BMA4_OK) {
            data = BMA4_SET_BITS_POS_0(data, BMA4_ACCEL_SELFTEST_ENABLE, accel_selftest_enable);
            rslt |= bma4_write_regs(BMA4_ACC_SELF_TEST_ADDR, &data, 1, dev);
        }
    }

    return rslt;
}

/*!
 *  @brief This API selects the sign of Accel self-test excitation.
 */
static uint16_t set_accel_selftest_sign(uint8_t accel_selftest_sign, struct bma4_dev *dev)
{
    uint16_t rslt = 0;
    uint8_t data = 0;
    /* Check the bma4 structure as NULL */
    if (dev == NULL) {
        rslt |= BMA4_E_NULL_PTR;
    } else {

        if (accel_selftest_sign <= BMA4_MAX_VALUE_SELFTEST_SIGN) {
            /* Read the Accel self test sign*/
            rslt |= bma4_read_regs(BMA4_ACC_SELF_TEST_ADDR, &data, 1, dev);

            if (rslt == BMA4_OK) {
                data = BMA4_SET_BITSLICE(data, BMA4_ACCEL_SELFTEST_SIGN, accel_selftest_sign);
                rslt |= bma4_write_regs(BMA4_ACC_SELF_TEST_ADDR, &data, 1, dev);
            }
        } else {
            rslt = BMA4_E_OUT_OF_RANGE;
        }
    }

    return rslt;
}

/*!
 *  @brief This API sets the Accel self test amplitude in the sensor.
 */
static uint16_t set_accel_selftest_amp(uint8_t accel_selftest_amp, struct bma4_dev *dev)
{
    uint16_t rslt = 0;
    uint8_t data = 0;
    /* Check the bma4 structure as NULL */
    if (dev == NULL) {
        rslt |= BMA4_E_NULL_PTR;
    } else {

        if (accel_selftest_amp <= BMA4_MAX_VALUE_SELFTEST_AMP) {
            /* Write  self test amplitude*/
            rslt |= bma4_read_regs(BMA4_ACC_SELF_TEST_ADDR, &data, 1, dev);

            if (rslt == BMA4_OK) {
                data = BMA4_SET_BITSLICE(data, BMA4_SELFTEST_AMP, accel_selftest_amp);

                rslt |= bma4_write_regs(BMA4_ACC_SELF_TEST_ADDR, &data, 1, dev);
            }
        } else {
            rslt |= BMA4_E_OUT_OF_RANGE;
        }
    }

    return rslt;
}

/*!
 *  @brief This function enables and configures the Accel which is needed
 *  for Self test operation.
 */
static uint16_t set_accel_selftest_config(struct bma4_dev *dev)
{
    uint16_t rslt = 0;
    struct bma4_accel_config accel = {0};

    accel.odr = BMA4_OUTPUT_DATA_RATE_1600HZ;
    accel.bandwidth = BMA4_ACCEL_NORMAL_AVG4;
    accel.perf_mode = BMA4_ENABLE;
    accel.range = BMA4_ACCEL_RANGE_8G;

    rslt |= bma4_set_accel_enable(BMA4_ENABLE, dev);
    dev->delay(1);
    rslt |= bma4_set_accel_config(&accel, dev);

    return rslt;
}

/*!
 *  @brief This API validates the Accel g value provided as input by the
 *  user for Accel offset compensation.
 *
 *  @note The g-values to be passed to the parameter should be
 *  multiples of 1000000.
 */
static uint16_t validate_user_input(const int32_t *gvalue)

{
    uint8_t index = 0;
    int32_t min_gval = (int32_t)(-1.0 * BMA4XY_MULTIPLIER);
    int32_t max_gval = (int32_t)(1.0 * BMA4XY_MULTIPLIER);

    while (index < 3) {
        if (gvalue[index] >= min_gval && gvalue[index] <= max_gval)
            index++;
        else
            return BMA4_E_OUT_OF_RANGE;
    }

    return BMA4_OK;
}

/*!
 *  @brief This API normalise the data with offset
 */
static void normalise_offset(const struct offset_delta *compensated_data, struct accel_offset *offset_data)
{
    /* for handling negative offset */
    /* employing twos's Complement method*/
    if (compensated_data->x.is_negative == TRUE) {
        offset_data->x = ~offset_data->x;
        offset_data->x += 1;
    }

    if (compensated_data->y.is_negative == TRUE) {
        offset_data->y = ~offset_data->y;
        offset_data->y += 1;
    }

    if (compensated_data->z.is_negative == TRUE) {
        offset_data->z = ~offset_data->z;
        offset_data->z += 1;
    }

    offset_data->x = (uint8_t)((offset_data->x) * (-1));
    offset_data->y = (uint8_t)((offset_data->y) * (-1));
    offset_data->z = (uint8_t)((offset_data->z) * (-1));
}

/*!
 *  @brief This API normalize the data with offset.
 */
static void scale_offset(uint8_t res, uint8_t range, const struct offset_delta *delta, struct accel_offset *data)
{
    int8_t bit_pos_3_9mg, bit_pos_3_9mg_nextbit;
    uint8_t round_off = 0;

    /* Find the bit position of 3.9mg */
    bit_pos_3_9mg = get_bit_pos_3_9mg(range, res);

    /* Data register resolution less than or equal to  3.9 mg */
    if (bit_pos_3_9mg > 0) {
        /* Round off, consider if the next bit is high */
        bit_pos_3_9mg_nextbit = bit_pos_3_9mg - 1;
        round_off = (uint8_t)(1 * power(2, ((uint8_t) bit_pos_3_9mg_nextbit)));
        /* scale according to offset register resolution*/
        data->x = (uint8_t)((delta->x.val + round_off) / power(2, ((uint8_t)bit_pos_3_9mg)));
        data->y = (uint8_t)((delta->y.val + round_off) / power(2, ((uint8_t)bit_pos_3_9mg)));
        data->z = (uint8_t)((delta->z.val + round_off) / power(2, ((uint8_t)bit_pos_3_9mg)));
    } else if (bit_pos_3_9mg < 0) {
        bit_pos_3_9mg = (int8_t)(bit_pos_3_9mg * -1);
        data->x = (uint8_t)(delta->x.val * power(2, ((uint8_t)bit_pos_3_9mg)));
        data->y = (uint8_t)(delta->y.val * power(2, ((uint8_t)bit_pos_3_9mg)));
        data->z = (uint8_t)(delta->z.val * power(2, ((uint8_t)bit_pos_3_9mg)));
    } else {
        /* Scale according to offset register resolution */
        data->x = (uint8_t)(delta->x.val);
        data->y = (uint8_t)(delta->y.val);
        data->z = (uint8_t)(delta->z.val);
    }
}

/*!
 *  @brief This API compensate the accel data against gravity.
 *
 *  @note The g-values to be passed to the parameter should be
 *  multiples of 1000000.
 */
static void comp_for_grvty(uint16_t lsb_per_g, const int32_t g_val[3], const struct bma4_accel *data,
                           struct offset_delta *comp_data)
{
    int64_t accel_value_lsb[3] = {0};
    uint8_t index;

    for (index = 0; index < 3; index++) {
        /* convert g to lsb */
        accel_value_lsb[index] = ((int64_t)(lsb_per_g) * (int64_t)(g_val[index]));
    }

    /*  Dividing the accel value in LSB by 1000000 to get
    compensated data back in g-value */
    comp_data->x.val = (int16_t)(data->x - (int16_t)((accel_value_lsb[BMA4_X_AXIS] / (int64_t)BMA4XY_MULTIPLIER)));
    comp_data->y.val = (int16_t)(data->y - (int16_t)((accel_value_lsb[BMA4_Y_AXIS] / (int64_t)BMA4XY_MULTIPLIER)));
    comp_data->z.val = (int16_t)(data->z - (int16_t)((accel_value_lsb[BMA4_Z_AXIS] / (int64_t)BMA4XY_MULTIPLIER)));

    if (comp_data->x.val < 0) {
        comp_data->x.val = ABS(comp_data->x.val);
        comp_data->x.is_negative = 1;
    }

    if (comp_data->y.val < 0) {
        comp_data->y.val = ABS(comp_data->y.val);
        comp_data->y.is_negative = 1;
    }

    if (comp_data->z.val < 0) {
        comp_data->z.val = ABS(comp_data->z.val);
        comp_data->z.is_negative = 1;
    }
}

/*!
 *  @brief This API converts the range value into corresponding
 *  integer value.
 */
static void map_range(uint8_t range_in, uint8_t *range_out)
{
    switch (range_in) {
    case  BMA4_ACCEL_RANGE_2G:
        *range_out = 2;
        break;
    case  BMA4_ACCEL_RANGE_4G:
        *range_out = 4;
        break;
    case  BMA4_ACCEL_RANGE_8G:
        *range_out = 8;
        break;
    case  BMA4_ACCEL_RANGE_16G:
        *range_out = 16;
        break;
    default:
        break;
    }
}

/*!
 *  @brief This API is used to reset the FIFO related configurations
 *  in the fifo_frame structure.
 *
 */
static void reset_fifo_data_structure(const struct bma4_dev *dev)
{
    /*Prepare for next FIFO read by resetting FIFO's
    internal data structures*/
    dev->fifo->accel_byte_start_idx = 0;
    dev->fifo->mag_byte_start_idx = 0;
    dev->fifo->sc_frame_byte_start_idx = 0;
    dev->fifo->sensor_time = 0;
    dev->fifo->skipped_frame_count = 0;
    dev->fifo->accel_dropped_frame_count = 0;
    dev->fifo->mag_dropped_frame_count = 0;
}

/*!
 *  @brief This API computes the number of bytes of accel FIFO data
 *  which is to be parsed in header-less mode
 */
static void get_accel_len_to_parse(uint16_t *start_idx, uint16_t *len, const uint16_t *acc_count,
                                   const struct bma4_dev *dev)
{
    uint8_t dummy_byte_spi = 0;

    /*Check if this is the first iteration of data unpacking
     if yes, then consider dummy byte on SPI*/
    if (dev->fifo->accel_byte_start_idx == 0)
        dummy_byte_spi = dev->dummy_byte;

    /*Data start index*/
    *start_idx = dev->fifo->accel_byte_start_idx + dummy_byte_spi;

    if (dev->fifo->fifo_data_enable == BMA4_FIFO_A_ENABLE) {
        /*Len has the number of bytes to loop for */
        *len = (uint16_t)(((*acc_count) * BMA4_FIFO_A_LENGTH) + dummy_byte_spi);
    } else if (dev->fifo->fifo_data_enable == BMA4_FIFO_M_A_ENABLE) {
        /*Len has the number of bytes to loop for */
        *len = (uint16_t)(((*acc_count) * BMA4_FIFO_MA_LENGTH) + dummy_byte_spi);
    } else {
        /*Only aux. sensor or no sensor is enabled in FIFO,
        so there will be no accel data.
        Update the data index as complete*/
        *start_idx = dev->fifo->length;
    }
    if ((*len) > dev->fifo->length) {
        /*Handling the case where more data is requested
        than available*/
        *len = dev->fifo->length;
    }
}

/*!
 *  @brief This API checks the fifo read data as empty frame, if it
 *  is empty frame then moves the index to last byte.
 */
static void check_empty_fifo(uint16_t *data_index, const struct bma4_dev *dev)
{
    if ((*data_index + 2) < dev->fifo->length) {
        /* Check if FIFO is empty */
        if ((dev->fifo->data[*data_index] == FIFO_MSB_CONFIG_CHECK)
                && (dev->fifo->data[*data_index + 1] == FIFO_LSB_CONFIG_CHECK)) {
            /*Update the data index as complete*/
            *data_index = dev->fifo->length;
        }
    }
}

/*!
 *  @brief This API is used to parse the accelerometer data from the
 *  FIFO data in header mode.
 *
 */
static void extract_accel_header_mode(struct bma4_accel *accel_data, uint16_t *accel_length, const struct bma4_dev *dev)
{
    uint8_t frame_header = 0;
    uint16_t data_index;
    uint16_t accel_index = 0;
    uint16_t frame_to_read = *accel_length;
    /*Check if this is the first iteration of data unpacking
     if yes, then consider dummy byte on SPI*/
    if (dev->fifo->accel_byte_start_idx == 0)
        dev->fifo->accel_byte_start_idx = dev->dummy_byte;

    for (data_index = dev->fifo->accel_byte_start_idx; data_index < dev->fifo->length;) {
        /*Header byte is stored in the variable frame_header*/
        frame_header = dev->fifo->data[data_index];
        /*Get the frame details from header*/
        frame_header = frame_header & BMA4_FIFO_TAG_INTR_MASK;
        /*Index is moved to next byte where the data is starting*/
        data_index++;

        switch (frame_header) {
        /* Accel frame */
        case FIFO_HEAD_A:
        case FIFO_HEAD_M_A:
            unpack_acc_frm(accel_data, &data_index, &accel_index, frame_header, dev);
            break;
        /* Aux. sensor frame */
        case FIFO_HEAD_M:
            move_next_frame(&data_index, BMA4_FIFO_M_LENGTH, dev);
            break;
        /* Sensor time frame */
        case FIFO_HEAD_SENSOR_TIME:
            unpack_sensortime_frame(&data_index, dev);
            break;
        /* Skip frame */
        case FIFO_HEAD_SKIP_FRAME:
            unpack_skipped_frame(&data_index, dev);
            break;
        /* Input config frame */
        case FIFO_HEAD_INPUT_CONFIG:
            move_next_frame(&data_index, 1, dev);
            break;
        /* Sample drop frame */
        case FIFO_HEAD_SAMPLE_DROP:
            unpack_dropped_frame(&data_index, dev);
            break;
        /* Over read FIFO data */
        case FIFO_HEAD_OVER_READ_MSB:
            /* Update the data index as complete*/
            data_index = dev->fifo->length;
            break;
        default:
            break;
        }
        if (frame_to_read == accel_index) {
            /*Number of frames to read completed*/
            break;
        }
    }

    /*Update number of accel data read*/
    *accel_length = accel_index;
    /*Update the accel frame index*/
    dev->fifo->accel_byte_start_idx = data_index;
}

/*!
 *  @brief This API is used to parse the accelerometer data from the
 *  FIFO data in both header mode and header-less mode.
 *  It update the idx value which is used to store the index of
 *  the current data byte which is parsed.
 */
static void unpack_acc_frm(struct bma4_accel *acc, uint16_t *idx, uint16_t *acc_idx, uint8_t frm,
                           const struct bma4_dev *dev)
{
    switch (frm) {
    case FIFO_HEAD_A:
    case BMA4_FIFO_A_ENABLE:
        /*Partial read, then skip the data*/
        if ((*idx + BMA4_FIFO_A_LENGTH) > dev->fifo->length) {
            /*Update the data index as complete*/
            *idx = dev->fifo->length;
            break;
        }
        /*Unpack the data array into the structure instance "acc" */
        unpack_accel_data(&acc[*acc_idx], *idx, dev);
        /*Move the data index*/
        *idx = *idx + BMA4_FIFO_A_LENGTH;
        (*acc_idx)++;
        break;
    case FIFO_HEAD_M_A:
    case BMA4_FIFO_M_A_ENABLE:
        /*Partial read, then skip the data*/
        if ((*idx + BMA4_FIFO_MA_LENGTH) > dev->fifo->length) {
            /*Update the data index as complete*/
            *idx = dev->fifo->length;
            break;
        }
        /*Unpack the data array into structure instance "acc"*/
        unpack_accel_data(&acc[*acc_idx], *idx + BMA4_MA_FIFO_A_X_LSB, dev);
        /*Move the data index*/
        *idx = *idx + BMA4_FIFO_MA_LENGTH;
        (*acc_idx)++;
        break;
    /* Aux. sensor frame */
    case FIFO_HEAD_M:
    case BMA4_FIFO_M_ENABLE:
        (*idx) = (*idx) + BMA4_FIFO_M_LENGTH;
        break;
    default:
        break;
    }
}

/*!
 *  @brief This API is used to parse the accelerometer data from the
 *  FIFO data and store it in the instance of the structure bma4_accel.
 */
static void unpack_accel_data(struct bma4_accel *accel_data, uint16_t data_start_index, const struct bma4_dev *dev)
{
    uint16_t data_lsb;
    uint16_t data_msb;

    /* Accel raw x data */
    data_lsb = dev->fifo->data[data_start_index++];
    data_msb = dev->fifo->data[data_start_index++];
    accel_data->x = (int16_t)((data_msb << 8) | data_lsb);

    /* Accel raw y data */
    data_lsb = dev->fifo->data[data_start_index++];
    data_msb = dev->fifo->data[data_start_index++];
    accel_data->y = (int16_t)((data_msb << 8) | data_lsb);

    /* Accel raw z data */
    data_lsb = dev->fifo->data[data_start_index++];
    data_msb = dev->fifo->data[data_start_index++];
    accel_data->z = (int16_t)((data_msb << 8) | data_lsb);

    if (dev->resolution == BMA4_12_BIT_RESOLUTION) {
        accel_data->x = (accel_data->x / 0x10);
        accel_data->y = (accel_data->y / 0x10);
        accel_data->z = (accel_data->z / 0x10);
    } else if (dev->resolution == BMA4_14_BIT_RESOLUTION) {
        accel_data->x = (accel_data->x / 0x04);
        accel_data->y = (accel_data->y / 0x04);
        accel_data->z = (accel_data->z / 0x04);
    }
}

/*!
 *  @brief This API computes the number of bytes of Mag FIFO data which is
 *  to be parsed in header-less mode
 *
 */
static void get_mag_len_to_parse(uint16_t *start_idx, uint16_t *len, const uint16_t *mag_count,
                                 const struct bma4_dev *dev)
{
    uint8_t dummy_byte_spi = 0;

    /*Check if this is the first iteration of data unpacking
     if yes, then consider dummy byte on SPI*/
    if (dev->fifo->mag_byte_start_idx == 0)
        dummy_byte_spi = dev->dummy_byte;

    /*Data start index*/
    *start_idx = dev->fifo->mag_byte_start_idx + dummy_byte_spi;

    if (dev->fifo->fifo_data_enable == BMA4_FIFO_M_ENABLE) {
        /*Len has the number of bytes to loop for */
        *len = (uint16_t)(((*mag_count) * BMA4_FIFO_M_LENGTH) + dummy_byte_spi);
    } else if (dev->fifo->fifo_data_enable == BMA4_FIFO_M_A_ENABLE) {
        /*Len has the number of bytes to loop for */
        *len = (uint16_t)(((*mag_count) * BMA4_FIFO_MA_LENGTH) + dummy_byte_spi);
    } else {
        /*Only accel sensor or no sensor is enabled in FIFO,
        so there will be no mag data.
        Update the data index as complete*/
        *start_idx = dev->fifo->length;
    }

    /*Handling the case where more data is requested than available*/
    if ((*len) > dev->fifo->length) {
        /*Len is equal to the FIFO length*/
        *len = dev->fifo->length;
    }
}

/*!
 *  @brief This API is used to parse the magnetometer data from the
 *  FIFO data in header mode.
 *
 */
static uint16_t extract_mag_header_mode(struct bma4_mag *data, uint16_t *len, const struct bma4_dev *dev)
{
    uint16_t rslt = 0;
    uint8_t frame_header = 0;
    uint16_t data_index;
    uint16_t mag_index = 0;
    uint16_t frame_to_read = *len;

    /*Check if this is the first iteration of data unpacking
     if yes, then consider dummy byte on SPI*/
    if (dev->fifo->mag_byte_start_idx == 0)
        dev->fifo->mag_byte_start_idx = dev->dummy_byte;

    for (data_index = dev->fifo->mag_byte_start_idx; data_index < dev->fifo->length;) {
        /*Header byte is stored in the variable frame_header*/
        frame_header = dev->fifo->data[data_index];
        /*Get the frame details from header*/
        frame_header = frame_header & BMA4_FIFO_TAG_INTR_MASK;
        /*Index is moved to next byte where the data is starting*/
        data_index++;

        switch (frame_header) {
        /* Aux. sensor frame */
        case FIFO_HEAD_M:
        case FIFO_HEAD_M_A:
            rslt |= unpack_mag_frm(data, &data_index, &mag_index, frame_header, dev);
            break;
        /* Aux. sensor frame */
        case FIFO_HEAD_A:
            move_next_frame(&data_index, BMA4_FIFO_A_LENGTH, dev);
            break;
        /* Sensor time frame */
        case FIFO_HEAD_SENSOR_TIME:
            unpack_sensortime_frame(&data_index, dev);
            break;
        /* Skip frame */
        case FIFO_HEAD_SKIP_FRAME:
            unpack_skipped_frame(&data_index, dev);
            break;
        /* Input config frame */
        case FIFO_HEAD_INPUT_CONFIG:
            move_next_frame(&data_index, 1, dev);
            break;
        /* Sample drop frame */
        case FIFO_HEAD_SAMPLE_DROP:
            unpack_dropped_frame(&data_index, dev);
            break;
        case FIFO_HEAD_OVER_READ_MSB:
            /*Update the data index as complete*/
            data_index = dev->fifo->length;
            break;
        default:
            break;
        }
        if (frame_to_read == mag_index) {
            /*Number of frames to read completed*/
            break;
        }
    }
    /*update number of Aux. sensor data read*/
    *len = mag_index;
    /*update the Aux. sensor frame index*/
    dev->fifo->mag_byte_start_idx = data_index;
    return rslt;
}

/*!
 *  @brief This API is used to parse the magnetometer data from the
 *  FIFO data in both header mode and header-less mode and update the
 *  data_index value which is used to store the index of the current
 *  data byte which is parsed.
 *
 */
static uint16_t unpack_mag_frm(struct bma4_mag *data, uint16_t *idx, uint16_t *mag_idx, uint8_t frm,
                               const struct bma4_dev *dev)
{
    uint16_t rslt = 0;

    switch (frm) {
    case FIFO_HEAD_M:
    case BMA4_FIFO_M_ENABLE:
        /*partial read, then skip the data*/
        if ((*idx + BMA4_FIFO_M_LENGTH) > dev->fifo->length) {
            /*update the data index as complete*/
            *idx = dev->fifo->length;
            break;
        }

        /*unpack the data array into Aux. sensor data structure*/
        rslt |= unpack_mag_data(&data[*mag_idx], *idx, dev);
        /*move the data index*/
        *idx = *idx + BMA4_FIFO_M_LENGTH;
        (*mag_idx)++;
        break;
    case FIFO_HEAD_M_A:
    case BMA4_FIFO_M_A_ENABLE:
        /*partial read, then skip the data*/
        if ((*idx + BMA4_FIFO_MA_LENGTH) > dev->fifo->length) {
            /*update the data index as complete*/
            *idx = dev->fifo->length;
            break;
        }

        /*unpack the data array into Aux. sensor data structure*/
        rslt |= unpack_mag_data(&data[*mag_idx], *idx, dev);
        /*move the data index to next frame*/
        *idx = *idx + BMA4_FIFO_MA_LENGTH;
        (*mag_idx)++;
        break;
    /* aux. sensor frame */
    case FIFO_HEAD_A:
    case BMA4_FIFO_A_ENABLE:
        (*idx) = (*idx) + BMA4_FIFO_A_LENGTH;
        break;
    default:
        break;
    }

    return rslt;
}

/*!
 *  @brief This API is used to parse the auxiliary magnetometer data from
 *  the FIFO data and store it in the instance of the structure mag_data.
 *
 */
static uint16_t unpack_mag_data(struct bma4_mag *mag_data, uint16_t start_idx, const struct bma4_dev *dev)
{
    uint16_t rslt = 0;
    struct bma4_mag_fifo_data mag_fifo_data;

    /* Aux. mag sensor raw x data */
    mag_fifo_data.mag_x_lsb = dev->fifo->data[start_idx++];
    mag_fifo_data.mag_x_msb = dev->fifo->data[start_idx++];

    /* Aux. mag sensor raw y data */
    mag_fifo_data.mag_y_lsb = dev->fifo->data[start_idx++];
    mag_fifo_data.mag_y_msb = dev->fifo->data[start_idx++];

    /* Aux. mag sensor raw z data */
    mag_fifo_data.mag_z_lsb = dev->fifo->data[start_idx++];
    mag_fifo_data.mag_z_msb = dev->fifo->data[start_idx++];

    /* Aux. mag sensor raw r data */
    mag_fifo_data.mag_r_y2_lsb = dev->fifo->data[start_idx++];
    mag_fifo_data.mag_r_y2_msb = dev->fifo->data[start_idx++];

    /*Compensated FIFO data output*/
    rslt |= bma4_second_if_mag_compensate_xyz(mag_fifo_data, dev->aux_sensor, mag_data);

    return rslt;
}

/*!
 *  @brief This API is used to parse and store the sensor time from the
 *  FIFO data in the structure instance dev.
 *
 */
static void unpack_sensortime_frame(uint16_t *data_index, const struct bma4_dev *dev)
{
    uint32_t sensor_time_byte3 = 0;
    uint16_t sensor_time_byte2 = 0;
    uint8_t sensor_time_byte1 = 0;

    /*Partial read, then move the data index to last data*/
    if ((*data_index + BMA4_SENSOR_TIME_LENGTH) > dev->fifo->length) {
        /*Update the data index as complete*/
        *data_index = dev->fifo->length;
    } else {
        sensor_time_byte3 = dev->fifo->data[(*data_index) + BMA4_SENSOR_TIME_MSB_BYTE] << 16;
        sensor_time_byte2 = dev->fifo->data[(*data_index) + BMA4_SENSOR_TIME_XLSB_BYTE] << 8;
        sensor_time_byte1 = dev->fifo->data[(*data_index)];
        /* Sensor time */
        dev->fifo->sensor_time = (uint32_t)(sensor_time_byte3 | sensor_time_byte2 | sensor_time_byte1);
        *data_index = (*data_index) + BMA4_SENSOR_TIME_LENGTH;
    }
}

/*!
 *  @brief This API is used to parse and store the skipped_frame_count from
 *  the FIFO data in the structure instance dev.
 */
static void unpack_skipped_frame(uint16_t *data_index, const struct bma4_dev *dev)
{
    /*Partial read, then move the data index to last data*/
    if (*data_index >= dev->fifo->length) {
        /*Update the data index as complete*/
        *data_index = dev->fifo->length;
    } else {
        dev->fifo->skipped_frame_count = dev->fifo->data[*data_index];
        /*Move the data index*/
        *data_index = (*data_index) + 1;
    }
}

/*!
 *  @brief This API is used to parse and store the dropped_frame_count from
 *  the FIFO data in the structure instance dev.
 */
static void unpack_dropped_frame(uint16_t *data_index, const struct bma4_dev *dev)
{
    uint8_t dropped_frame = 0;
    /*Partial read, then move the data index to last data*/
    if (*data_index >= dev->fifo->length) {
        /*Update the data index as complete*/
        *data_index = dev->fifo->length;
    } else {
        /*Extract accel and mag dropped frame count*/
        dropped_frame = dev->fifo->data[*data_index] & ACCEL_AUX_FIFO_DROP;
        /*Move the data index and update the dropped frame count*/
        switch (dropped_frame) {
        case ACCEL_FIFO_DROP:
            *data_index = (*data_index) + BMA4_FIFO_A_LENGTH;
            dev->fifo->accel_dropped_frame_count = dev->fifo->accel_dropped_frame_count + 1;
            break;
        case AUX_FIFO_DROP:
            *data_index = (*data_index) + BMA4_FIFO_M_LENGTH;
            dev->fifo->mag_dropped_frame_count = dev->fifo->mag_dropped_frame_count + 1;
            break;
        case ACCEL_AUX_FIFO_DROP:
            *data_index = (*data_index) + BMA4_FIFO_MA_LENGTH;
            dev->fifo->accel_dropped_frame_count = dev->fifo->accel_dropped_frame_count + 1;
            dev->fifo->mag_dropped_frame_count = dev->fifo->mag_dropped_frame_count + 1;
            break;
        default:
            break;
        }
    }
}

/*!
 *  @brief This API is used to move the data index ahead of the
 *  current_frame_length parameter when unnecessary FIFO data appears while
 *  extracting the user specified data.
 */
static void move_next_frame(uint16_t *data_index, uint8_t current_frame_length, const struct bma4_dev *dev)
{
    /*Partial read, then move the data index to last data*/
    if ((*data_index + current_frame_length) > dev->fifo->length) {
        /*Update the data index as complete*/
        *data_index = dev->fifo->length;
    } else {
        /*Move the data index to next frame*/
        *data_index = *data_index + current_frame_length;
    }
}

/*!
 *  @brief This function validates the Accel Self test data and decides the
 *  result of Self test operation.
 */
static uint16_t validate_selftest(const struct selftest_delta_limit *accel_data_diff,
                                  const struct bma4_dev *dev)
{
    uint16_t rslt = 0;

    /* Set self test amplitude based on variant */
    switch (dev->variant) {
    case BMA42X_VARIANT:
        /* Validating accel data by comparing with minimum value of the axes in mg */
        /* For BMA42x - > x axis limit 400mg, y axis limit 800mg and z axis limit 400mg */
        if ((accel_data_diff->x > BMA42X_ST_ACC_X_AXIS_SIGNAL_DIFF) &&
                (accel_data_diff->y > BMA42X_ST_ACC_Y_AXIS_SIGNAL_DIFF) &&
                (accel_data_diff->z > BMA42X_ST_ACC_Z_AXIS_SIGNAL_DIFF)) {
            rslt = BMA4_OK;
        } else {
            rslt = BMA4_E_SELF_TEST_FAIL;
        }
        break;

    case BMA45X_VARIANT:
        /* Validating accel data by comparing with minimum value of the axes in mg */
        /* For BMA45x - > x axis limit 1800mg, y axis limit 1800mg and z axis limit 1800mg */
        if ((accel_data_diff->x > BMA45X_ST_ACC_X_AXIS_SIGNAL_DIFF) &&
                (accel_data_diff->y > BMA45X_ST_ACC_Y_AXIS_SIGNAL_DIFF) &&
                (accel_data_diff->z > BMA45X_ST_ACC_Z_AXIS_SIGNAL_DIFF)) {
            rslt = BMA4_OK;
        } else {
            rslt = BMA4_E_SELF_TEST_FAIL;
        }
        break;

    default:
        rslt = BMA4_E_INVALID_SENSOR;
        break;
    }

    return rslt;
}

/*!
 *  @brief This function configure the Accel for FOC.
 */
static uint16_t foc_config(struct bma4_accel_config *acc_conf, uint8_t *acc_en, uint8_t *pwr_mode, struct bma4_dev *dev)
{
    uint16_t rslt = 0;
    uint8_t accel_cnf = BMA4_ACCEL_CONFIG_FOC;

    /* for saving Accel configuration,
     Accel enable status, advance power save*/
    rslt |= bma4_get_accel_config(acc_conf, dev);
    rslt |= bma4_get_accel_enable(acc_en, dev);
    rslt |= bma4_get_advance_power_save(pwr_mode, dev);

    /* Disabling offset compensation that is in place*/
    rslt |= bma4_set_offset_comp(BMA4_DISABLE, dev);

    if (rslt == BMA4_OK) {
        /* Set Accel config to 50Hz, continuous filter mode,
        no under sampling */
        rslt |= bma4_write_regs(BMA4_ACCEL_CONFIG_ADDR, &accel_cnf, 1, dev);

        if (rslt == BMA4_OK) {
            /* Switch Accel to normal mode and
            advance power save to zero*/
            rslt |= bma4_set_accel_enable(BMA4_ENABLE, dev);
            rslt |= bma4_set_advance_power_save(BMA4_DISABLE, dev);
        }
    }

    return rslt;
}

/*!
 * @brief This API is used to calculate the power of 2
 */
static int32_t power(int16_t base, uint8_t resolution)
{
    uint8_t i = 1;
    /* Initialize variable to store the power of 2 value */
    int32_t value = 1;

    for (; i <= resolution; i++)
        value = (int32_t)(value * base);

    return value;
}

/*!
 * @brief This API performs the roundoff on given value
 */
static int8_t roundoff(int32_t value)
{
    /* Variable to return the round off value */
    int8_t ret = 0;

    /* Since the value passed is scaled in multiples of 100,
    the return value is divided by 100 to get the round off value */
    if (value < 0)
        ret = (int8_t)(((value) - 50) / 100);
    else
        ret = (int8_t)(((value) + 50) / 100);


    return ret;
}

/*!
 * @brief This API finds the bit position of 3.9mg according to given range
 * and resolution.
 */
static int8_t get_bit_pos_3_9mg(uint8_t range, uint8_t res)
{
    /* Variable to store the bit position of 3.9mg */
    int8_t bit_pos_3_9mg = 0;
    /* Variable to store the value to be rounded off */
    int32_t value = 0;
    /* Variable to store the LSB per bit in micros */
    int32_t ug_per_bit;
    /* Variable to store the rounded off value */
    int8_t round_off_int;
    /* Variable to store the bit count */
    uint8_t bit_count = 0;
    /* Variable to store the signed range value */
    int32_t range_value;

    /* Scaling range with a multiplier to get integer value of ug_per_bit */
    range_value = (int32_t)(2 * range * BMA4XY_MULTIPLIER);

    /* Get the G-per bit resolution*/
    ug_per_bit = (int32_t)(range_value / power(2, res));

    /* Compare for -ve & +ve bit position w.r.t 3900micro-g or as reference
     * Note: Value scaled in 100s to get accurate integer value */
    if (ug_per_bit > 3900)
        value = (int32_t)(ug_per_bit * 100 / 3900);
    else
        value = (int32_t)(3900 * 100 / ug_per_bit);

    /* Round off the value */
    round_off_int = (int8_t)(roundoff(value));

    /* Find the bit position of 3.9mg*/
    while (round_off_int != 1) {
        round_off_int = (round_off_int / 0x02);
        bit_count++;
    }

    /* Check for +ve and -ve bit position of 3.9 mg */
    if (ug_per_bit > 3900)
        bit_pos_3_9mg = (int8_t)(bit_count * (-1));
    else
        bit_pos_3_9mg = (int8_t)bit_count;

    return bit_pos_3_9mg;
}
/*!
 * @brief This internal API brings up the secondary interface to access
 *          auxiliary sensor *
 */
static uint16_t bma4_set_aux_interface_config(struct bma4_dev *dev)
{
    /* Variable to return error codes */
    uint16_t rslt = BMA4_OK;

    /* Check for null pointer error */
    rslt |= bma4_null_pointer_check(dev);

    if (rslt == BMA4_OK) {
        /* Enable the auxiliary sensor */
        rslt |= bma4_set_mag_enable(0x01, dev);
        dev->delay(BMA4_AUX_COM_DELAY);

        /* Disable advance power save */
        rslt |= bma4_set_advance_power_save(0x00, dev);
        dev->delay(BMA4_AUX_COM_DELAY);

        /* Set the I2C device address of auxiliary device */
        rslt |= bma4_set_i2c_device_addr(dev);
        dev->delay(BMA4_AUX_COM_DELAY);

        /* Set auxiliary interface to manual mode */
        rslt |= bma4_set_mag_manual_enable(dev->aux_config.manual_enable, dev);
        dev->delay(BMA4_AUX_COM_DELAY);

        /* Set the number of bytes for burst read */
        rslt |= bma4_set_mag_burst(dev->aux_config.burst_read_length, dev);
        dev->delay(BMA4_AUX_COM_DELAY);

        /* Switch on the the auxiliary interface mode */
        rslt |= bma4_set_if_mode(dev->aux_config.if_mode, dev);
        dev->delay(BMA4_AUX_COM_DELAY);

    }

    return rslt;
}

/*!
* @brief This internal API reads the data from the auxiliary sensor
*        depending on burst length configured
*/
static uint16_t bma4_extract_aux_data(uint8_t aux_reg_addr, uint8_t *aux_data, uint16_t len, struct bma4_dev *dev)
{
    /* Variable to return error codes */
    uint16_t rslt = BMA4_OK;
    /* Pointer variable to read data from the register */
    uint8_t data[15] = {0};
    /* Variable to define length counts */
    uint8_t len_count = 0;
    /* Variable to define burst read length */
    uint8_t burst_len = 0;
    /* Variable to define read length */
    uint8_t read_length = 0;
    /* Variable to define the number of burst reads */
    uint8_t burst_count;
    /* Variable to define address of the data register*/
    uint8_t aux_read_addr = BMA4_DATA_0_ADDR;

    /* Extract burst read length in a variable */
    rslt |= bma4_map_read_len(&burst_len, dev);

    for (burst_count = 0; burst_count < len; burst_count += burst_len) {
        /* Set the address whose data is to be read */
        rslt |= bma4_set_mag_read_addr(aux_reg_addr, dev);
        dev->delay(BMA4_AUX_COM_DELAY);

        if (rslt == BMA4_OK) {
            /* If user defined length is valid */
            if (len > 0) {
                /* Read the data from the data register */
                rslt |= bma4_read_regs(aux_read_addr, data, (uint8_t)burst_len, dev);
                dev->delay(BMA4_AUX_COM_DELAY);

                if (rslt == BMA4_OK) {
                    /* If defined user length or remaining length after a burst
                    read  is less than burst length */
                    if ((len - burst_count) < burst_len) {
                        /* Read length is equal to burst_length or
                           remaining length*/
                        read_length = (uint8_t)(len - burst_count);
                    } else {
                        /* Read length is equal to burst_length */
                        read_length = burst_len;
                    }

                    /* Copy the read data in terms of given read length */
                    for (len_count = 0; len_count < read_length; len_count++)
                        aux_data[burst_count +  len_count] = data[len_count];

                    /* Increment the register address by burst read length */
                    aux_reg_addr += burst_len;
                } else {
                    rslt = BMA4_E_RD_WR_LENGTH_INVALID;
                }
            } else {
                rslt = BMA4_E_FAIL;
            }
        } else {
            rslt = BMA4_E_FAIL;
        }
    }

    return rslt;
}

/*!
 * @brief This internal API maps the actual burst read length with user
            length set.
 */
static uint16_t bma4_map_read_len(uint8_t *len, const struct bma4_dev *dev)
{
    /* Variable to return error codes */
    uint16_t rslt = BMA4_OK;

    switch (dev->aux_config.burst_read_length) {

    case BMA4_AUX_READ_LEN_0:
        *len = 1;
        break;
    case BMA4_AUX_READ_LEN_1:
        *len = 2;
        break;
    case BMA4_AUX_READ_LEN_2:
        *len = 6;
        break;
    case BMA4_AUX_READ_LEN_3:
        *len = 8;
        break;
    default:
        rslt = BMA4_E_OUT_OF_RANGE;
        break;
    }

    return rslt;
}

/*!
* @brief This internal API checks null pointer error
*/
static uint16_t bma4_null_pointer_check(const struct bma4_dev *dev)
{
    uint16_t rslt = BMA4_OK;

    if ((dev == NULL) || (dev->bus_read == NULL) || (dev->bus_write == NULL))
        rslt |= BMA4_E_NULL_PTR;
    else
        rslt = BMA4_OK;

    return rslt;
}
