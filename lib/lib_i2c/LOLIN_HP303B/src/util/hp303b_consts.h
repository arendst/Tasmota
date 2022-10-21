/**
 * 
 *
 */

#ifndef __HP303B_CONSTS_H_
#define __HP303B_CONSTS_H_


	//general Constants
#define HP303B__PROD_ID						0U
#define HP303B__STD_SLAVE_ADDRESS 			0x77U
#define HP303B__SPI_WRITE_CMD 				0x00U
#define HP303B__SPI_READ_CMD 				0x80U
#define HP303B__SPI_RW_MASK 				0x80U
#define HP303B__SPI_MAX_FREQ 				100000U

#define HP303B__LSB 0x01U

#define HP303B__TEMP_STD_MR					2U
#define HP303B__TEMP_STD_OSR				3U
#define HP303B__PRS_STD_MR					2U
#define HP303B__PRS_STD_OSR					3U
#define HP303B__OSR_SE 						3U
//we use 0.1 mS units for time calculations, so 10 units are one millisecond
#define HP303B__BUSYTIME_SCALING 			10U
// DPS310 has 10 milliseconds of spare time for each synchronous measurement / per second for asynchronous measurements
// this is for error prevention on friday-afternoon-products :D
// you can set it to 0 if you dare, but there is no warranty that it will still work
#define HP303B__BUSYTIME_FAILSAFE			10U
#define HP303B__MAX_BUSYTIME 				((1000U-HP303B__BUSYTIME_FAILSAFE)*HP303B__BUSYTIME_SCALING)
#define HP303B__NUM_OF_SCAL_FACTS			8

#define HP303B__SUCCEEDED 					0
#define HP303B__FAIL_UNKNOWN 				-1
#define HP303B__FAIL_INIT_FAILED 			-2
#define HP303B__FAIL_TOOBUSY 				-3
#define HP303B__FAIL_UNFINISHED 			-4


	//Constants for register manipulation
			//SPI mode (3 or 4 wire)
#define HP303B__REG_ADR_SPI3W 				0x09U
#define HP303B__REG_CONTENT_SPI3W 			0x01U


			//product id
#define HP303B__REG_INFO_PROD_ID 			HP303B__REG_ADR_PROD_ID, \
												HP303B__REG_MASK_PROD_ID, \
												HP303B__REG_SHIFT_PROD_ID
#define HP303B__REG_ADR_PROD_ID 			0x0DU
#define HP303B__REG_MASK_PROD_ID 			0x0FU
#define HP303B__REG_SHIFT_PROD_ID 			0U

			//revision id
#define HP303B__REG_INFO_REV_ID 			HP303B__REG_ADR_REV_ID, \
												HP303B__REG_MASK_REV_ID, \
												HP303B__REG_SHIFT_REV_ID
#define HP303B__REG_ADR_REV_ID 				0x0DU
#define HP303B__REG_MASK_REV_ID 			0xF0U
#define HP303B__REG_SHIFT_REV_ID 			4U

			//operating mode
#define HP303B__REG_INFO_OPMODE 			HP303B__REG_ADR_OPMODE, \
												HP303B__REG_MASK_OPMODE, \
												HP303B__REG_SHIFT_OPMODE
#define HP303B__REG_ADR_OPMODE 				0x08U
#define HP303B__REG_MASK_OPMODE 			0x07U
#define HP303B__REG_SHIFT_OPMODE 			0U


			//temperature measure rate
#define HP303B__REG_INFO_TEMP_MR 			HP303B__REG_ADR_TEMP_MR, \
												HP303B__REG_MASK_TEMP_MR, \
												HP303B__REG_SHIFT_TEMP_MR
#define HP303B__REG_ADR_TEMP_MR 			0x07U
#define HP303B__REG_MASK_TEMP_MR 			0x70U
#define HP303B__REG_SHIFT_TEMP_MR 			4U

			//temperature oversampling rate
#define HP303B__REG_INFO_TEMP_OSR 			HP303B__REG_ADR_TEMP_OSR, \
												HP303B__REG_MASK_TEMP_OSR, \
												HP303B__REG_SHIFT_TEMP_OSR
#define HP303B__REG_ADR_TEMP_OSR 			0x07U
#define HP303B__REG_MASK_TEMP_OSR 			0x07U
#define HP303B__REG_SHIFT_TEMP_OSR 			0U

			//temperature sensor
#define HP303B__REG_INFO_TEMP_SENSOR 		HP303B__REG_ADR_TEMP_SENSOR, \
												HP303B__REG_MASK_TEMP_SENSOR, \
												HP303B__REG_SHIFT_TEMP_SENSOR
#define HP303B__REG_ADR_TEMP_SENSOR 		0x07U
#define HP303B__REG_MASK_TEMP_SENSOR 		0x80U
#define HP303B__REG_SHIFT_TEMP_SENSOR 		7U

			//temperature sensor recommendation
#define HP303B__REG_INFO_TEMP_SENSORREC 	HP303B__REG_ADR_TEMP_SENSORREC, \
												HP303B__REG_MASK_TEMP_SENSORREC, \
												HP303B__REG_SHIFT_TEMP_SENSORREC
#define HP303B__REG_ADR_TEMP_SENSORREC 		0x28U
#define HP303B__REG_MASK_TEMP_SENSORREC 	0x80U
#define HP303B__REG_SHIFT_TEMP_SENSORREC 	7U

			//temperature shift enable (if temp_osr>3)
#define HP303B__REG_INFO_TEMP_SE 			HP303B__REG_ADR_TEMP_SE, \
												HP303B__REG_MASK_TEMP_SE, \
												HP303B__REG_SHIFT_TEMP_SE
#define HP303B__REG_ADR_TEMP_SE 			0x09U
#define HP303B__REG_MASK_TEMP_SE 			0x08U
#define HP303B__REG_SHIFT_TEMP_SE 			3U


			//pressure measure rate
#define HP303B__REG_INFO_PRS_MR 			HP303B__REG_ADR_PRS_MR, \
												HP303B__REG_MASK_PRS_MR, \
												HP303B__REG_SHIFT_PRS_MR
#define HP303B__REG_ADR_PRS_MR 				0x06U
#define HP303B__REG_MASK_PRS_MR 			0x70U
#define HP303B__REG_SHIFT_PRS_MR 			4U

			//pressure oversampling rate
#define HP303B__REG_INFO_PRS_OSR 			HP303B__REG_ADR_PRS_OSR, \
												HP303B__REG_MASK_PRS_OSR, \
												HP303B__REG_SHIFT_PRS_OSR
#define HP303B__REG_ADR_PRS_OSR 			0x06U
#define HP303B__REG_MASK_PRS_OSR 			0x07U
#define HP303B__REG_SHIFT_PRS_OSR 			0U

			//pressure shift enable (if prs_osr>3)
#define HP303B__REG_INFO_PRS_SE 			HP303B__REG_ADR_PRS_SE, \
												HP303B__REG_MASK_PRS_SE, \
												HP303B__REG_SHIFT_PRS_SE
#define HP303B__REG_ADR_PRS_SE 				0x09U
#define HP303B__REG_MASK_PRS_SE 			0x04U
#define HP303B__REG_SHIFT_PRS_SE 			2U


			//temperature ready flag
#define HP303B__REG_INFO_TEMP_RDY 			HP303B__REG_ADR_TEMP_RDY, \
												HP303B__REG_MASK_TEMP_RDY, \
												HP303B__REG_SHIFT_TEMP_RDY
#define HP303B__REG_ADR_TEMP_RDY 			0x08U
#define HP303B__REG_MASK_TEMP_RDY			0x20U
#define HP303B__REG_SHIFT_TEMP_RDY 			5U

			//pressure ready flag
#define HP303B__REG_INFO_PRS_RDY 			HP303B__REG_ADR_PRS_RDY, \
												HP303B__REG_MASK_PRS_RDY, \
												HP303B__REG_SHIFT_PRS_RDY
#define HP303B__REG_ADR_PRS_RDY 			0x08U
#define HP303B__REG_MASK_PRS_RDY 			0x10U
#define HP303B__REG_SHIFT_PRS_RDY 			4U

			//pressure value
#define HP303B__REG_ADR_PRS 				0x00U
#define HP303B__REG_LEN_PRS 				3U

			//temperature value
#define HP303B__REG_ADR_TEMP 				0x03U
#define HP303B__REG_LEN_TEMP 				3U

			//compensation coefficients
#define HP303B__REG_ADR_COEF 				0x10U
#define HP303B__REG_LEN_COEF 				18


			//FIFO enable
#define HP303B__REG_INFO_FIFO_EN 			HP303B__REG_ADR_FIFO_EN, \
												HP303B__REG_MASK_FIFO_EN, \
												HP303B__REG_SHIFT_FIFO_EN
#define HP303B__REG_ADR_FIFO_EN 			0x09U
#define HP303B__REG_MASK_FIFO_EN 			0x02U
#define HP303B__REG_SHIFT_FIFO_EN 			1U

			//FIFO flush
#define HP303B__REG_INFO_FIFO_FL 			HP303B__REG_ADR_FIFO_EN, \
												HP303B__REG_MASK_FIFO_EN, \
												HP303B__REG_SHIFT_FIFO_EN
#define HP303B__REG_ADR_FIFO_FL 			0x0CU
#define HP303B__REG_MASK_FIFO_FL 			0x80U
#define HP303B__REG_SHIFT_FIFO_FL 			7U

			//FIFO empty
#define HP303B__REG_INFO_FIFO_EMPTY 		HP303B__REG_ADR_FIFO_EMPTY, \
												HP303B__REG_MASK_FIFO_EMPTY, \
												HP303B__REG_SHIFT_FIFO_EMPTY
#define HP303B__REG_ADR_FIFO_EMPTY 			0x0BU
#define HP303B__REG_MASK_FIFO_EMPTY 		0x01U
#define HP303B__REG_SHIFT_FIFO_EMPTY 		0U

			//FIFO full
#define HP303B__REG_INFO_FIFO_FULL 			HP303B__REG_ADR_FIFO_FULL, \
												HP303B__REG_MASK_FIFO_FULL, \
												HP303B__REG_SHIFT_FIFO_FULL
#define HP303B__REG_ADR_FIFO_FULL 			0x0BU
#define HP303B__REG_MASK_FIFO_FULL 			0x02U
#define HP303B__REG_SHIFT_FIFO_FULL 		1U


			//INT HL
#define HP303B__REG_INFO_INT_HL 			HP303B__REG_ADR_INT_HL, \
												HP303B__REG_MASK_INT_HL, \
												HP303B__REG_SHIFT_INT_HL
#define HP303B__REG_ADR_INT_HL 				0x09U
#define HP303B__REG_MASK_INT_HL 			0x80U
#define HP303B__REG_SHIFT_INT_HL 			7U

			//INT FIFO enable
#define HP303B__REG_INFO_INT_EN_FIFO 		HP303B__REG_ADR_INT_EN_FIFO, \
												HP303B__REG_MASK_INT_EN_FIFO, \
												HP303B__REG_SHIFT_INT_EN_FIFO
#define HP303B__REG_ADR_INT_EN_FIFO 		0x09U
#define HP303B__REG_MASK_INT_EN_FIFO 		0x40U
#define HP303B__REG_SHIFT_INT_EN_FIFO 		6U

			//INT TEMP enable
#define HP303B__REG_INFO_INT_EN_TEMP 		HP303B__REG_ADR_INT_EN_TEMP, \
												HP303B__REG_MASK_INT_EN_TEMP, \
												HP303B__REG_SHIFT_INT_EN_TEMP
#define HP303B__REG_ADR_INT_EN_TEMP 		0x09U
#define HP303B__REG_MASK_INT_EN_TEMP 		0x20U
#define HP303B__REG_SHIFT_INT_EN_TEMP 		5U

			//INT PRS enable
#define HP303B__REG_INFO_INT_EN_PRS 		HP303B__REG_ADR_INT_EN_PRS, \
												HP303B__REG_MASK_INT_EN_PRS, \
												HP303B__REG_SHIFT_INT_EN_PRS
#define HP303B__REG_ADR_INT_EN_PRS 			0x09U
#define HP303B__REG_MASK_INT_EN_PRS 		0x10U
#define HP303B__REG_SHIFT_INT_EN_PRS 		4U

			//INT FIFO flag
#define HP303B__REG_INFO_INT_FLAG_FIFO 		HP303B__REG_ADR_INT_FLAG_FIFO, \
												HP303B__REG_MASK_INT_FLAG_FIFO, \
												HP303B__REG_SHIFT_INT_FLAG_FIFO
#define HP303B__REG_ADR_INT_FLAG_FIFO 		0x0AU
#define HP303B__REG_MASK_INT_FLAG_FIFO 		0x04U
#define HP303B__REG_SHIFT_INT_FLAG_FIFO 	2U

			//INT TMP flag
#define HP303B__REG_INFO_INT_FLAG_TEMP 		HP303B__REG_ADR_INT_FLAG_TEMP, \
												HP303B__REG_MASK_INT_FLAG_TEMP, \
												HP303B__REG_SHIFT_INT_FLAG_TEMP
#define HP303B__REG_ADR_INT_FLAG_TEMP 		0x0AU
#define HP303B__REG_MASK_INT_FLAG_TEMP 		0x02U
#define HP303B__REG_SHIFT_INT_FLAG_TEMP 	1U

			//INT PRS flag
#define HP303B__REG_INFO_INT_FLAG_PRS 		HP303B__REG_ADR_INT_FLAG_PRS, \
												HP303B__REG_MASK_INT_FLAG_PRS, \
												HP303B__REG_SHIFT_INT_FLAG_PRS
#define HP303B__REG_ADR_INT_FLAG_PRS 		0x0AU
#define HP303B__REG_MASK_INT_FLAG_PRS 		0x01U
#define HP303B__REG_SHIFT_INT_FLAG_PRS 		0U



#endif /* DPS310_CONSTS_H_ */
