#include "LOLIN_HP303B.h"



const int32_t LOLIN_HP303B::scaling_facts[HP303B__NUM_OF_SCAL_FACTS]
	= {524288, 1572864, 3670016, 7864320, 253952, 516096, 1040384, 2088960};



//////// 		Constructor, Destructor, begin, end			////////


/**
 * Standard Constructor
 */
LOLIN_HP303B::LOLIN_HP303B(void)
{
	//assume that initialization has failed before it has been done
	m_initFail = 1U;
}

/**
 * Standard Destructor
 */
LOLIN_HP303B::~LOLIN_HP303B(void)
{
	end();
}



/**
 * Standard I2C begin function
 *
 * &bus: 			I2CBus which connects MC to HP303B
 * slaveAddress: 	Address of the HP303B (0x77 or 0x76)
 */
uint8_t LOLIN_HP303B::begin(TwoWire &bus, uint8_t slaveAddress)
{
	//this flag will show if the initialization was successful
	m_initFail = 0U;

	//Set I2C bus connection
	m_SpiI2c = 1U;
	m_i2cbus = &bus;
	m_slaveAddress = slaveAddress;

	// Init bus
	m_i2cbus->begin();

	delay(50);		//startup time of HP303B

	return init();
}

uint8_t LOLIN_HP303B::begin(uint8_t slaveAddress)
{
	return begin(Wire,slaveAddress);
}

/**
 * SPI begin function for HP303B with 4-wire SPI
 */
uint8_t LOLIN_HP303B::begin(SPIClass &bus, int32_t chipSelect)
{
	return begin(bus, chipSelect, 0U);
}

/**
 * Standard SPI begin function
 *
 * &bus: 			SPI bus which connects MC to HP303B
 * chipSelect: 		Number of the CS line for the HP303B
 * threeWire: 		1 if HP303B is connected with 3-wire SPI
 * 					0 if HP303B is connected with 4-wire SPI (standard)
 */
uint8_t LOLIN_HP303B::begin(SPIClass &bus, int32_t chipSelect, uint8_t threeWire)
{
	//this flag will show if the initialization was successful
	m_initFail = 0U;

	//Set SPI bus connection
	m_SpiI2c = 0U;
	m_spibus = &bus;
	m_chipSelect = chipSelect;

	// Init bus
	m_spibus->begin();
	m_spibus->setDataMode(SPI_MODE3);

	pinMode(m_chipSelect, OUTPUT);
	digitalWrite(m_chipSelect, HIGH);

	delay(50);		//startup time of HP303B

	//switch to 3-wire mode if necessary
	//do not use writeByteBitfield or check option to set SPI mode!
	//Reading is not possible until SPI-mode is valid
	if(threeWire)
	{
		m_threeWire = 1U;
		if(writeByte(HP303B__REG_ADR_SPI3W, HP303B__REG_CONTENT_SPI3W))
		{
			m_initFail = 1U;
			return 0U;
		}
	}

	return init();
}

/**
 * End function for HP303B
 * Sets the sensor to idle mode
 */
void LOLIN_HP303B::end(void)
{
	standby();
}


////////		Declaration of other public functions starts here			////////


/**
 * returns the Product ID of the connected HP303B sensor
 */
uint8_t LOLIN_HP303B::getProductId(void)
{
	return m_productID;
}

/**
 * returns the Revision ID of the connected HP303B sensor
 */
uint8_t LOLIN_HP303B::getRevisionId(void)
{
	return m_revisionID;
}

/**
 * Sets the HP303B to standby mode
 *
 * returns:		0 on success
 * 				-2 if object initialization failed
 * 				-1 on other fail
 */
int16_t LOLIN_HP303B::standby(void)
{
	//abort if initialization failed
	if(m_initFail)
	{
		return HP303B__FAIL_INIT_FAILED;
	}
	//set device to idling mode
	int16_t ret = setOpMode(IDLE);
	if(ret != HP303B__SUCCEEDED)
	{
		return ret;
	}
	//flush the FIFO
	ret = writeByteBitfield(1U, HP303B__REG_INFO_FIFO_FL);
	if(ret < 0)
	{
		return ret;
	}
	//disable the FIFO
	ret = writeByteBitfield(0U, HP303B__REG_INFO_FIFO_EN);
	return ret;
}


/**
 * performs one temperature measurement and writes result to the given address
 *
 * &result:		reference to a 32-Bit signed Integer value where the result will be written
 * 				It will not be written if result==NULL
 * returns: 	0 on success
 * 				-4 if the HP303B is could not finish its measurement in time
 * 				-3 if the HP303B is already busy
 * 				-2 if the object initialization failed
 * 				-1 on other fail
 */
int16_t LOLIN_HP303B::measureTempOnce(float &result)
{
	return measureTempOnce(result, m_slaveAddress, m_tempOsr);
}

/**
 * performs one temperature measurement and writes result to the given address
 *
 * &result:		reference to a 32-Bit signed Integer value where the result will be written
 * 				It will not be written if result==NULL
 * returns: 	0 on success
 * 				-4 if the HP303B is could not finish its measurement in time
 * 				-3 if the HP303B is already busy
 * 				-2 if the object initialization failed
 * 				-1 on other fail
 */
int16_t LOLIN_HP303B::measureTempOnce(float &result, uint8_t slaveAddress)
{
	return measureTempOnce(result, slaveAddress, m_tempOsr);
}

/**
 * performs one temperature measurement and writes result to the given address
 * the desired precision can be set with oversamplingRate
 *
 * &result:				reference to a 32-Bit signed Integer where the result will be written
 * 						It will not be written if result==NULL
 * oversamplingRate: 	a value from 0 to 7 that decides about the precision
 * 						of the measurement
 * 						If this value equals n, the HP303B will perform
 * 						2^n measurements and combine the results
 * returns: 			0 on success
 * 						-4 if the HP303B is could not finish its measurement in time
 * 						-3 if the HP303B is already busy
 * 						-2 if the object initialization failed
 * 						-1 on other fail
 */
int16_t LOLIN_HP303B::measureTempOnce(float &result, uint8_t slaveAddress, uint8_t oversamplingRate)
{
	//Set I2C bus connection
	m_slaveAddress = slaveAddress;

	//Start measurement
	int16_t ret = startMeasureTempOnce(oversamplingRate);
	if(ret!=HP303B__SUCCEEDED)
	{
		return ret;
	}

	//wait until measurement is finished
	delay(calcBusyTime(0U, m_tempOsr)/HP303B__BUSYTIME_SCALING);
	delay(HP303B__BUSYTIME_FAILSAFE);

	ret = getSingleResult(result);
	if(ret!=HP303B__SUCCEEDED)
	{
		standby();
	}
	return ret;
}

/**
 * starts a single temperature measurement
 *
 * returns: 	0 on success
 * 				-3 if the HP303B is already busy
 * 				-2 if the object initialization failed
 * 				-1 on other fail
 */
int16_t LOLIN_HP303B::startMeasureTempOnce(void)
{
	return startMeasureTempOnce(m_tempOsr);
}

/**
 * starts a single temperature measurement
 * The desired precision can be set with oversamplingRate
 *
 * oversamplingRate: 	a value from 0 to 7 that decides about the precision
 * 						of the measurement
 * 						If this value equals n, the HP303B will perform
 * 						2^n measurements and combine the results
 * returns: 			0 on success
 * 						-3 if the HP303B is already busy
 * 						-2 if the object initialization failed
 * 						-1 on other fail
 */
int16_t LOLIN_HP303B::startMeasureTempOnce(uint8_t oversamplingRate)
{
	//abort if initialization failed
	if(m_initFail)
	{
		return HP303B__FAIL_INIT_FAILED;
	}
	//abort if device is not in idling mode
	if(m_opMode!=IDLE)
	{
		return HP303B__FAIL_TOOBUSY;
	}

	if(oversamplingRate!=m_tempOsr)
	{
		//configuration of oversampling rate
		if(configTemp(0U, oversamplingRate) != HP303B__SUCCEEDED)
		{
			return HP303B__FAIL_UNKNOWN;
		}
	}

	//set device to temperature measuring mode
	return setOpMode(0U, 1U, 0U);
}

/**
 * performs one pressure measurement and writes result to the given address
 *
 * &result:		reference to a 32-Bit signed Integer value where the result will be written
 * 				It will not be written if result==NULL
 * returns: 	0 on success
 * 				-4 if the HP303B is could not finish its measurement in time
 * 				-3 if the HP303B is already busy
 * 				-2 if the object initialization failed
 * 				-1 on other fail
 */
int16_t LOLIN_HP303B::measurePressureOnce(float &result)
{
	return measurePressureOnce(result, m_slaveAddress, m_prsOsr);
}

/**
 * performs one pressure measurement and writes result to the given address
 *
 * &result:		reference to a 32-Bit signed Integer value where the result will be written
 * 				It will not be written if result==NULL
 * returns: 	0 on success
 * 				-4 if the HP303B is could not finish its measurement in time
 * 				-3 if the HP303B is already busy
 * 				-2 if the object initialization failed
 * 				-1 on other fail
 */
int16_t LOLIN_HP303B::measurePressureOnce(float &result, uint8_t slaveAddress)
{
	return measurePressureOnce(result, slaveAddress, m_prsOsr);
}

/**
 * performs one pressure measurement and writes result to the given address
 * the desired precision can be set with oversamplingRate
 *
 * &result:				reference to a 32-Bit signed Integer where the result will be written
 * 						It will not be written if result==NULL
 * oversamplingRate: 	a value from 0 to 7 that decides about the precision
 * 						of the measurement
 * 						If this value equals n, the HP303B will perform
 * 						2^n measurements and combine the results
 * returns: 			0 on success
 * 						-4 if the HP303B is could not finish its measurement in time
 * 						-3 if the HP303B is already busy
 * 						-2 if the object initialization failed
 * 						-1 on other fail
 */
int16_t LOLIN_HP303B::measurePressureOnce(float &result, uint8_t slaveAddress, uint8_t oversamplingRate)
{
	//Set I2C bus connection
	m_slaveAddress = slaveAddress;

	//start the measurement
	int16_t ret = startMeasurePressureOnce(oversamplingRate);
	if(ret != HP303B__SUCCEEDED)
	{
		return ret;
	}

	//wait until measurement is finished
	delay(calcBusyTime(0U, m_prsOsr)/HP303B__BUSYTIME_SCALING);
	delay(HP303B__BUSYTIME_FAILSAFE);

	ret = getSingleResult(result);
	if(ret!=HP303B__SUCCEEDED)
	{
		standby();
	}
	return ret;
}

/**
 * starts a single pressure measurement
 *
 * returns: 	0 on success
 * 				-3 if the HP303B is already busy
 * 				-2 if the object initialization failed
 * 				-1 on other fail
 */
int16_t LOLIN_HP303B::startMeasurePressureOnce(void)
{
	return startMeasurePressureOnce(m_prsOsr);
}

/**
 * starts a single pressure measurement
 * The desired precision can be set with oversamplingRate
 *
 * oversamplingRate: 	a value from 0 to 7 that decides about the precision
 * 						of the measurement
 * 						If this value equals n, the HP303B will perform
 * 						2^n measurements and combine the results
 * returns: 			0 on success
 * 						-3 if the HP303B is already busy
 * 						-2 if the object initialization failed
 * 						-1 on other fail
 */
int16_t LOLIN_HP303B::startMeasurePressureOnce(uint8_t oversamplingRate)
{
	//abort if initialization failed
	if(m_initFail)
	{
		return HP303B__FAIL_INIT_FAILED;
	}
	//abort if device is not in idling mode
	if(m_opMode != IDLE)
	{
		return HP303B__FAIL_TOOBUSY;
	}
	//configuration of oversampling rate, lowest measure rate to avoid conflicts
	if(oversamplingRate != m_prsOsr)
	{
		if(configPressure(0U, oversamplingRate))
		{
			return HP303B__FAIL_UNKNOWN;
		}
	}
	//set device to pressure measuring mode
	return setOpMode(0U, 0U, 1U);
}

/**
 * gets the result a single temperature or pressure measurement in °C or Pa
 *
 * &result:		reference to a 32-Bit signed Integer value where the result will be written
 * returns: 	0 on success
 * 				-4 if the HP303B is still busy
 * 				-3 if the HP303B is not in command mode
 * 				-2 if the object initialization failed
 * 				-1 on other fail
 */
int16_t LOLIN_HP303B::getSingleResult(float &result)
{
	//abort if initialization failed
	if(m_initFail)
	{
		return HP303B__FAIL_INIT_FAILED;
	}

	//read finished bit for current opMode
	int16_t rdy;
	switch(m_opMode)
	{
	case CMD_TEMP: 	//temperature
		rdy = readByteBitfield(HP303B__REG_INFO_TEMP_RDY);
		break;
	case CMD_PRS: 	//pressure
		rdy = readByteBitfield(HP303B__REG_INFO_PRS_RDY);
		break;
	default: 	//HP303B not in command mode
		return HP303B__FAIL_TOOBUSY;
	}

	//read new measurement result
	switch(rdy)
	{
	case HP303B__FAIL_UNKNOWN: 	//could not read ready flag
		return HP303B__FAIL_UNKNOWN;
	case 0: 						//ready flag not set, measurement still in progress
		return HP303B__FAIL_UNFINISHED;
	case 1: 						//measurement ready, expected case
		LOLIN_HP303B::Mode oldMode = m_opMode;
		m_opMode = IDLE;				//opcode was automatically reseted by HP303B
		switch(oldMode)
		{
		case CMD_TEMP: 	//temperature
			return getTemp(&result);		//get and calculate the temperature value
		case CMD_PRS: 	//pressure
			return getPressure(&result);	//get and calculate the pressure value
		default:
			return HP303B__FAIL_UNKNOWN;	//should already be filtered above
		}
	}
	return HP303B__FAIL_UNKNOWN;
}

/**
 * starts a continuous temperature measurement
 * The desired precision can be set with oversamplingRate
 * The desired number of measurements per second can be set with measureRate
 *
 * measureRate: 		a value from 0 to 7 that decides about
 * 						the number of measurements per second
 * 						If this value equals n, the HP303B will perform
 * 						2^n measurements per second
 * oversamplingRate: 	a value from 0 to 7 that decides about
 * 						the precision of the measurements
 * 						If this value equals m, the HP303B will perform
 * 						2^m internal measurements and combine the results
 * 						to one more exact measurement
 * returns: 			0 on success
 * 						-4 if measureRate or oversamplingRate is too high
 * 						-3 if the HP303B is already busy
 * 						-2 if the object initialization failed
 * 						-1 on other fail
 * 	NOTE: 				If measure rate is n and oversampling rate is m,
 * 						the HP303B performs 2^(n+m) internal measurements per second.
 * 						The HP303B cannot operate with high precision and high speed
 * 						at the same time.
 * 						Consult the datasheet for more information.
 */
int16_t LOLIN_HP303B::startMeasureTempCont(uint8_t measureRate, uint8_t oversamplingRate)
{
	//abort if initialization failed
	if(m_initFail)
	{
		return HP303B__FAIL_INIT_FAILED;
	}
	//abort if device is not in idling mode
	if(m_opMode != IDLE)
	{
		return HP303B__FAIL_TOOBUSY;
	}
	//abort if speed and precision are too high
	if(calcBusyTime(measureRate, oversamplingRate) >= HP303B__MAX_BUSYTIME)
	{
		return HP303B__FAIL_UNFINISHED;
	}
	//update precision and measuring rate
	if(configTemp(measureRate, oversamplingRate))
	{
		return HP303B__FAIL_UNKNOWN;
	}
	//enable result FIFO
	if(writeByteBitfield(1U, HP303B__REG_INFO_FIFO_EN))
	{
		return HP303B__FAIL_UNKNOWN;
	}
	//Start measuring in background mode
	if(setOpMode(1U, 1U, 0U))
	{
		return HP303B__FAIL_UNKNOWN;
	}
	return HP303B__SUCCEEDED;
}


/**
 * starts a continuous temperature measurement
 * The desired precision can be set with oversamplingRate
 * The desired number of measurements per second can be set with measureRate
 *
 * measureRate: 		a value from 0 to 7 that decides about
 * 						the number of measurements per second
 * 						If this value equals n, the HP303B will perform
 * 						2^n measurements per second
 * oversamplingRate: 	a value from 0 to 7 that decides about the precision
 * 						of the measurements
 * 						If this value equals m, the HP303B will perform
 * 						2^m internal measurements
 * 						and combine the results to one more exact measurement
 * returns: 			0 on success
 * 						-4 if measureRate or oversamplingRate is too high
 * 						-3 if the HP303B is already busy
 * 						-2 if the object initialization failed
 * 						-1 on other fail
 * 	NOTE: 				If measure rate is n and oversampling rate is m,
 * 						the HP303B performs 2^(n+m) internal measurements per second.
 * 						The HP303B cannot operate with high precision and high speed
 * 						at the same time.
 * 						Consult the datasheet for more information.
 */
int16_t LOLIN_HP303B::startMeasurePressureCont(uint8_t measureRate, uint8_t oversamplingRate)
{
	//abort if initialization failed
	if(m_initFail)
	{
		return HP303B__FAIL_INIT_FAILED;
	}
	//abort if device is not in idling mode
	if(m_opMode != IDLE)
	{
		return HP303B__FAIL_TOOBUSY;
	}
	//abort if speed and precision are too high
	if(calcBusyTime(measureRate, oversamplingRate) >= HP303B__MAX_BUSYTIME)
	{
		return HP303B__FAIL_UNFINISHED;
	}
	//update precision and measuring rate
	if(configPressure(measureRate, oversamplingRate))
		return HP303B__FAIL_UNKNOWN;
	//enable result FIFO
	if(writeByteBitfield(1U, HP303B__REG_INFO_FIFO_EN))
	{
		return HP303B__FAIL_UNKNOWN;
	}
	//Start measuring in background mode
	if(setOpMode(1U, 0U, 1U))
	{
		return HP303B__FAIL_UNKNOWN;
	}
	return HP303B__SUCCEEDED;
}

/**
 * starts a continuous temperature and pressure measurement
 * The desired precision can be set with tempOsr and prsOsr
 * The desired number of measurements per second can be set with tempMr and prsMr
 *
 * tempMr				measure rate for temperature
 * tempOsr				oversampling rate for temperature
 * prsMr				measure rate for pressure
 * prsOsr				oversampling rate for pressure
 * returns: 			0 on success
 * 						-4 if precision or speed is too high
 * 						-3 if the HP303B is already busy
 * 						-2 if the object initialization failed
 * 						-1 on other fail
 * 	NOTE: 				High precision and speed for both temperature and pressure
 * 						can not be reached at the same time.
 * 						Estimated time for temperature and pressure measurement
 * 						is the sum of both values.
 * 						This sum must not be more than 1 second.
 * 						Consult the datasheet for more information.
 */
int16_t LOLIN_HP303B::startMeasureBothCont(uint8_t tempMr,
										 uint8_t tempOsr,
										 uint8_t prsMr,
										 uint8_t prsOsr)
{
	//abort if initialization failed
	if(m_initFail)
	{
		return HP303B__FAIL_INIT_FAILED;
	}
	//abort if device is not in idling mode
	if(m_opMode!=IDLE)
	{
		return HP303B__FAIL_TOOBUSY;
	}
	//abort if speed and precision are too high
	if(calcBusyTime(tempMr, tempOsr) + calcBusyTime(prsMr, prsOsr)>=HP303B__MAX_BUSYTIME)
	{
		return HP303B__FAIL_UNFINISHED;
	}
	//update precision and measuring rate
	if(configTemp(tempMr, tempOsr))
	{
		return HP303B__FAIL_UNKNOWN;
	}
	//update precision and measuring rate
	if(configPressure(prsMr, prsOsr))
		return HP303B__FAIL_UNKNOWN;
	//enable result FIFO
	if(writeByteBitfield(1U, HP303B__REG_INFO_FIFO_EN))
	{
		return HP303B__FAIL_UNKNOWN;
	}
	//Start measuring in background mode
	if(setOpMode(1U, 1U, 1U))
	{
		return HP303B__FAIL_UNKNOWN;
	}
	return HP303B__SUCCEEDED;
}

/**
 * Gets the results from continuous measurements and writes them to given arrays
 *
 * *tempBuffer: 	The start address of the buffer where the temperature results
 * 					are written
 * 					If this is NULL, no temperature results will be written out
 * &tempCount:		This has to be a reference to a number which contains
 * 					the size of the buffer for temperature results.
 * 					When the function ends, it will contain
 * 					the number of bytes written to the buffer
 * *prsBuffer: 		The start address of the buffer where the pressure results
 * 					are written
 * 					If this is NULL, no pressure results will be written out
 * &prsCount:		This has to be a reference to a number which contains
 * 					the size of the buffer for pressure results.
 * 					When the function ends, it will contain
 * 					the number of bytes written to the buffer
 * returns:			0 on success
 * 					-3 if HP303B is not in background mode
 * 					-2 if the object initialization failed
 * 					-1 on other fail
 */
int16_t LOLIN_HP303B::getContResults(float *tempBuffer,
								   uint8_t &tempCount,
								   float *prsBuffer,
								   uint8_t &prsCount)
{
	if(m_initFail)
	{
		return HP303B__FAIL_INIT_FAILED;
	}
	//abort if device is not in background mode
	if(!(m_opMode & INVAL_OP_CONT_NONE))
	{
		return HP303B__FAIL_TOOBUSY;
	}

	//prepare parameters for buffer length and count
	uint8_t tempLen = tempCount;
	uint8_t prsLen = prsCount;
	tempCount = 0U;
	prsCount = 0U;

	//while FIFO is not empty
	while(readByteBitfield(HP303B__REG_INFO_FIFO_EMPTY) == 0)
	{
		float result;
		//read next result from FIFO
		int16_t type = getFIFOvalue(&result);
		switch(type)
		{
		case 0: //temperature
			//calculate compensated pressure value
			result = calcTemp(result);
			//if buffer exists and is not full
			//write result to buffer and increase temperature result counter
			if(tempBuffer != NULL)
			{
				if(tempCount<tempLen)
				{
					tempBuffer[tempCount++] = result;
				}
			}
			break;
		case 1: //pressure
			//calculate compensated pressure value
			result = calcPressure(result);
			//if buffer exists and is not full
			//write result to buffer and increase pressure result counter
			if(prsBuffer != NULL)
			{
				if(prsCount<prsLen)
				{
					prsBuffer[prsCount++] = result;
				}
			}
			break;
		case -1: //read failed
			break;	//continue while loop
					//if connection failed permanently,
					//while condition will become false
					//if read failed only once, loop will try again
		}
	}
	return HP303B__SUCCEEDED;
}

/**
 * Sets the active state of the Interrupt pin
 *
 * polarity: 	If this is 0, the interrupt pin of the HP303B will be low-active
 * 				If this is 1, the interrupt pin of the HP303B will be high-active
 * returns:		0 on success,
 * 				-1 on fail
 */
int16_t LOLIN_HP303B::setInterruptPolarity(uint8_t polarity)
{
	//Interrupts are not supported with 4 Wire SPI
	if(!m_SpiI2c & !m_threeWire)
	{
		return HP303B__FAIL_UNKNOWN;
	}
	return writeByteBitfield(polarity, HP303B__REG_INFO_INT_HL);
}

/**
 * Sets the sources that are able to cause interrupts
 *
 * fifoFull: 	if this is 1, an interrupt will be generated
 * 				when the FIFO is full
 * 				if this is 0, the FIFO will not generate any interrupts
 * tempReady: 	if this is 1, an interrupt will be generated
 * 				when a temperature measurement is finished
 * 				if this is 0, no interrupt will be generated
 * 				after finishing a temperature measurement
 * prsReady: 	if this is 1, an interrupt will be generated
 * 				when a pressure measurement is finished
 * 				if this is 0, no interrupt will be generated
 * 				after finishing a pressure measurement
 * returns: 	0 on success, -1 on fail
 */
int16_t LOLIN_HP303B::setInterruptSources(uint8_t fifoFull, uint8_t tempReady, uint8_t prsReady)
{
	//Interrupts are not supported with 4 Wire SPI
	if(!m_SpiI2c & !m_threeWire)
	{
		return HP303B__FAIL_UNKNOWN;
	}
	//mask parameters
	fifoFull &= HP303B__REG_MASK_INT_EN_FIFO >> HP303B__REG_SHIFT_INT_EN_FIFO;
	tempReady &= HP303B__REG_MASK_INT_EN_TEMP >> HP303B__REG_SHIFT_INT_EN_TEMP;
	prsReady &= HP303B__REG_MASK_INT_EN_PRS >> HP303B__REG_SHIFT_INT_EN_PRS;
	//read old value from register
	int16_t regData = readByte(HP303B__REG_ADR_INT_EN_FIFO);
	if(regData <0)
	{
		return HP303B__FAIL_UNKNOWN;
	}
	uint8_t toWrite = (uint8_t)regData;
	//update FIFO enable bit
	toWrite &= ~HP303B__REG_MASK_INT_EN_FIFO;	//clear bit
	toWrite |= fifoFull << HP303B__REG_SHIFT_INT_EN_FIFO;	//set new bit
	//update TempReady enable bit
	toWrite &= ~HP303B__REG_MASK_INT_EN_TEMP;
	toWrite |= tempReady << HP303B__REG_SHIFT_INT_EN_TEMP;
	//update PrsReady enable bit
	toWrite &= ~HP303B__REG_MASK_INT_EN_PRS;
	toWrite |= prsReady << HP303B__REG_SHIFT_INT_EN_PRS;
	//write updated value to register
	return writeByte(HP303B__REG_ADR_INT_EN_FIFO, toWrite);
}

/**
 * Gets the interrupt status flag of the FIFO
 *
 * Returns: 	1 if the FIFO is full and caused an interrupt
 * 				0 if the FIFO is not full or FIFO interrupt is disabled
 * 				-1 on fail
 */
int16_t LOLIN_HP303B::getIntStatusFifoFull(void)
{
	return readByteBitfield(HP303B__REG_INFO_INT_FLAG_FIFO);
}

/**
 * Gets the interrupt status flag that indicates a finished temperature measurement
 *
 * Returns: 	1 if a finished temperature measurement caused an interrupt
 * 				0 if there is no finished temperature measurement
 * 					or interrupts are disabled
 * 				-1 on fail
 */
int16_t LOLIN_HP303B::getIntStatusTempReady(void)
{
	return readByteBitfield(HP303B__REG_INFO_INT_FLAG_TEMP);
}

/**
 * Gets the interrupt status flag that indicates a finished pressure measurement
 *
 * Returns: 	1 if a finished pressure measurement caused an interrupt
 * 				0 if there is no finished pressure measurement
 * 					or interrupts are disabled
 * 				-1 on fail
 */
int16_t LOLIN_HP303B::getIntStatusPrsReady(void)
{
	return readByteBitfield(HP303B__REG_INFO_INT_FLAG_PRS);
}

/**
 * Function to fix a hardware problem on some devices
 * You have this problem if you measure a temperature which is too high (e.g. 60°C when temperature is around 20°C)
 * Call correctTemp() directly after begin() to fix this issue
 */
int16_t LOLIN_HP303B::correctTemp(void)
{
	if(m_initFail)
	{
		return HP303B__FAIL_INIT_FAILED;
	}
	writeByte(0x0E, 0xA5);
	writeByte(0x0F, 0x96);
	writeByte(0x62, 0x02);
	writeByte(0x0E, 0x00);
	writeByte(0x0F, 0x00);

	//perform a first temperature measurement (again)
	//the most recent temperature will be saved internally
	//and used for compensation when calculating pressure
	float trash;
	measureTempOnce(trash);

	return HP303B__SUCCEEDED;
}



//////// 	Declaration of private functions starts here	////////


/**
 * Initializes the sensor.
 * This function has to be called from begin()
 * and requires a valid bus initialization.
 */
uint8_t LOLIN_HP303B::init(void)
{
	int16_t prodId = readByteBitfield(HP303B__REG_INFO_PROD_ID);
	if(prodId != HP303B__PROD_ID)
	{
		//Connected device is not a HP303B
		m_initFail = 1U;
		return 0U;
	}
	m_productID = prodId;

	int16_t revId = readByteBitfield(HP303B__REG_INFO_REV_ID);
	if(revId < 0)
	{
		m_initFail = 1U;
		return 0U;
	}
	m_revisionID = revId;

	//find out which temperature sensor is calibrated with coefficients...
	int16_t sensor = readByteBitfield(HP303B__REG_INFO_TEMP_SENSORREC);
	if(sensor < 0)
	{
		m_initFail = 1U;
		return 0U;
	}

	//...and use this sensor for temperature measurement
	m_tempSensor = sensor;
	if(writeByteBitfield((uint8_t)sensor, HP303B__REG_INFO_TEMP_SENSOR) < 0)
	{
		m_initFail = 1U;
		return 0U;
	}

	//read coefficients
	if(readcoeffs() < 0)
	{
		m_initFail = 1U;
		return 0U;
	}

	//set to standby for further configuration
	standby();

	//set measurement precision and rate to standard values;
	configTemp(HP303B__TEMP_STD_MR, HP303B__TEMP_STD_OSR);
	configPressure(HP303B__PRS_STD_MR, HP303B__PRS_STD_OSR);

	//perform a first temperature measurement
	//the most recent temperature will be saved internally
	//and used for compensation when calculating pressure
	float trash;
	measureTempOnce(trash);

	//make sure the HP303B is in standby after initialization
	standby();

	// Fix IC with a fuse bit problem, which lead to a wrong temperature
	// Should not affect ICs without this problem
	correctTemp();

	return 1U;
}


/**
 * reads the compensation coefficients from the HP303B
 * this is called once from init(), which is called from begin()
 *
 * returns: 	0 on success, -1 on fail
 */
int16_t LOLIN_HP303B::readcoeffs(void)
{
	uint8_t buffer[HP303B__REG_LEN_COEF];
	//read COEF registers to buffer
	int16_t ret = readBlock(HP303B__REG_ADR_COEF,
							HP303B__REG_LEN_COEF,
							buffer);
	//abort if less than REG_LEN_COEF bytes were read
	if(ret < HP303B__REG_LEN_COEF)
	{
		return HP303B__FAIL_UNKNOWN;
	}

	//compose coefficients from buffer content
	m_c0Half =    ((uint32_t)buffer[0] << 4)
				| (((uint32_t)buffer[1] >> 4) & 0x0F);
	//this construction recognizes non-32-bit negative numbers
	//and converts them to 32-bit negative numbers with 2's complement
	if(m_c0Half & ((uint32_t)1 << 11))
	{
		m_c0Half -= (uint32_t)1 << 12;
	}
	//c0 is only used as c0*0.5, so c0_half is calculated immediately
	m_c0Half = m_c0Half / 2U;

	//now do the same thing for all other coefficients
	m_c1 = (((uint32_t)buffer[1] & 0x0F) << 8) | (uint32_t)buffer[2];
	if(m_c1 & ((uint32_t)1 << 11))
	{
		m_c1 -= (uint32_t)1 << 12;
	}

	m_c00 =   ((uint32_t)buffer[3] << 12)
			| ((uint32_t)buffer[4] << 4)
			| (((uint32_t)buffer[5] >> 4) & 0x0F);
	if(m_c00 & ((uint32_t)1 << 19))
	{
		m_c00 -= (uint32_t)1 << 20;
	}

	m_c10 =   (((uint32_t)buffer[5] & 0x0F) << 16)
			| ((uint32_t)buffer[6] << 8)
			| (uint32_t)buffer[7];
	if(m_c10 & ((uint32_t)1<<19))
	{
		m_c10 -= (uint32_t)1 << 20;
	}

	m_c01 =   ((uint32_t)buffer[8] << 8)
			| (uint32_t)buffer[9];
	if(m_c01 & ((uint32_t)1 << 15))
	{
		m_c01 -= (uint32_t)1 << 16;
	}

	m_c11 =   ((uint32_t)buffer[10] << 8)
			| (uint32_t)buffer[11];
	if(m_c11 & ((uint32_t)1 << 15))
	{
		m_c11 -= (uint32_t)1 << 16;
	}

	m_c20 =   ((uint32_t)buffer[12] << 8)
			| (uint32_t)buffer[13];
	if(m_c20 & ((uint32_t)1 << 15))
	{
		m_c20 -= (uint32_t)1 << 16;
	}

	m_c21 =   ((uint32_t)buffer[14] << 8)
			| (uint32_t)buffer[15];
	if(m_c21 & ((uint32_t)1 << 15))
	{
		m_c21 -= (uint32_t)1 << 16;
	}

	m_c30 =   ((uint32_t)buffer[16] << 8)
			| (uint32_t)buffer[17];
	if(m_c30 & ((uint32_t)1 << 15))
	{
		m_c30 -= (uint32_t)1 << 16;
	}

	return HP303B__SUCCEEDED;
}

/**
 * Sets the Operation Mode of the HP303B
 *
 * background:		determines the general behavior of the HP303B
 *  				0 enables command mode (only measure on commands)
 * 					1 enables background mode (continuous work in background)
 * temperature: 	set 1 to measure temperature
 * pressure: 		set 1 to measure pressure
 * return:			0 on success, -1 on fail
 *
 * NOTE!
 * You cannot set background to 1 without setting temperature and pressure
 * You cannot set both temperature and pressure when background mode is disabled
 */
int16_t LOLIN_HP303B::setOpMode(uint8_t background, uint8_t temperature, uint8_t pressure)
{
	uint8_t opMode =  (background & HP303B__LSB) << 2U
					| (temperature & HP303B__LSB) << 1U
					| (pressure & HP303B__LSB);
	return setOpMode(opMode);
}


/**
 * Sets the Operation Mode of the HP303B
 *
 * opMode: 			the new OpMode that has to be set
 * return: 			0 on success, -1 on fail
 *
 * NOTE!
 * You cannot set background to 1 without setting temperature and pressure
 * You cannot set both temperature and pressure when background mode is disabled
 */
int16_t LOLIN_HP303B::setOpMode(uint8_t opMode)
{
	//Filter irrelevant bits
	opMode &= HP303B__REG_MASK_OPMODE >> HP303B__REG_SHIFT_OPMODE;
	//Filter invalid OpModes
	if(opMode == INVAL_OP_CMD_BOTH || opMode == INVAL_OP_CONT_NONE)
	{
		return HP303B__FAIL_UNKNOWN;
	}
	//Set OpMode
	if(writeByte(HP303B__REG_ADR_OPMODE, opMode))
	{
		return HP303B__FAIL_UNKNOWN;
	}
	m_opMode = (LOLIN_HP303B::Mode)opMode;
	return HP303B__SUCCEEDED;
}

/**
 * Configures temperature measurement
 *
 * tempMr: 	the new measure rate for temperature
 * 				This can be a value from 0U to 7U.
 * 				Actual measure rate will be 2^tempMr,
 * 				so this will be a value from 1 to 128.
 * tempOsr: 	the new oversampling rate for temperature
 * 				This can be a value from 0U to 7U.
 * 				Actual measure rate will be 2^tempOsr,
 * 				so this will be a value from 1 to 128.
 * returns: 	0 normally or -1 on fail
 */
int16_t LOLIN_HP303B::configTemp(uint8_t tempMr, uint8_t tempOsr)
{
	//mask parameters
	tempMr &= HP303B__REG_MASK_TEMP_MR >> HP303B__REG_SHIFT_TEMP_MR;
	tempOsr &= HP303B__REG_MASK_TEMP_OSR >> HP303B__REG_SHIFT_TEMP_OSR;

	//set config register according to parameters
	uint8_t toWrite = tempMr << HP303B__REG_SHIFT_TEMP_MR;
	toWrite |= tempOsr << HP303B__REG_SHIFT_TEMP_OSR;
	//using recommended temperature sensor
	toWrite |=    HP303B__REG_MASK_TEMP_SENSOR
				& (m_tempSensor << HP303B__REG_SHIFT_TEMP_SENSOR);
	int16_t ret = writeByte(HP303B__REG_ADR_TEMP_MR, toWrite);
	//abort immediately on fail
	if(ret != HP303B__SUCCEEDED)
	{
		return HP303B__FAIL_UNKNOWN;
	}

	//set TEMP SHIFT ENABLE if oversampling rate higher than eight(2^3)
	if(tempOsr > HP303B__OSR_SE)
	{
		ret=writeByteBitfield(1U, HP303B__REG_INFO_TEMP_SE);
	}
	else
	{
		ret=writeByteBitfield(0U, HP303B__REG_INFO_TEMP_SE);
	}

	if(ret == HP303B__SUCCEEDED)
	{	//save new settings
		m_tempMr = tempMr;
		m_tempOsr = tempOsr;
	}
	else
	{
		//try to rollback on fail avoiding endless recursion
		//this is to make sure that shift enable and oversampling rate
		//are always consistent
		if(tempMr != m_tempMr || tempOsr != m_tempOsr)
		{
			configTemp(m_tempMr, m_tempOsr);
		}
	}
	return ret;
}

/**
 * Configures pressure measurement
 *
 * prsMr: 		the new measure rate for pressure
 * 				This can be a value from 0U to 7U.
 * 				Actual measure rate will be 2^prs_mr,
 * 				so this will be a value from 1 to 128.
 * prsOs: 	the new oversampling rate for temperature
 * 				This can be a value from 0U to 7U.
 * 				Actual measure rate will be 2^prsOsr,
 * 				so this will be a value from 1 to 128.
 * returns: 	0 normally or -1 on fail
 */
int16_t LOLIN_HP303B::configPressure(uint8_t prsMr, uint8_t prsOsr)
{
	//mask parameters
	prsMr &= HP303B__REG_MASK_PRS_MR >> HP303B__REG_SHIFT_PRS_MR;
	prsOsr &= HP303B__REG_MASK_PRS_OSR >> HP303B__REG_SHIFT_PRS_OSR;

	//set config register according to parameters
	uint8_t toWrite = prsMr << HP303B__REG_SHIFT_PRS_MR;
	toWrite |= prsOsr << HP303B__REG_SHIFT_PRS_OSR;
	int16_t ret = writeByte(HP303B__REG_ADR_PRS_MR, toWrite);
	//abort immediately on fail
	if(ret != HP303B__SUCCEEDED)
	{
		return HP303B__FAIL_UNKNOWN;
	}

	//set PM SHIFT ENABLE if oversampling rate higher than eight(2^3)
	if(prsOsr > HP303B__OSR_SE)
	{
		ret = writeByteBitfield(1U, HP303B__REG_INFO_PRS_SE);
	}
	else
	{
		ret = writeByteBitfield(0U, HP303B__REG_INFO_PRS_SE);
	}

	if(ret == HP303B__SUCCEEDED)
	{	//save new settings
		m_prsMr = prsMr;
		m_prsOsr = prsOsr;
	}
	else
	{	//try to rollback on fail avoiding endless recursion
		//this is to make sure that shift enable and oversampling rate
		//are always consistent
		if(prsMr != m_prsMr || prsOsr != m_prsOsr)
		{
			configPressure(m_prsMr, m_prsOsr);
		}
	}
	return ret;
}

/**
 * calculates the time that the HP303B needs for 2^mr measurements
 * with an oversampling rate of 2^osr
 *
 * mr: 		Measure rate for temperature or pressure
 * osr: 	Oversampling rate for temperature or pressure
 * returns: time that the HP303B needs for this measurement
 * 			a value of 10000 equals 1 second
 * 	NOTE! 	The measurement time for temperature and pressure
 * 			in sum must not be more than 1 second!
 * 			Timing behavior of pressure and temperature sensors
 * 			can be considered as equal.
 */
uint16_t LOLIN_HP303B::calcBusyTime(uint16_t mr, uint16_t osr)
{
	//mask parameters first
	mr &= HP303B__REG_MASK_TEMP_MR >> HP303B__REG_SHIFT_TEMP_MR;
	osr &= HP303B__REG_MASK_TEMP_OSR >> HP303B__REG_SHIFT_TEMP_OSR;
	//formula from datasheet (optimized)
	return ((uint32_t)20U << mr) + ((uint32_t)16U << (osr + mr));
}

/**
 * Gets the next temperature measurement result in degrees of Celsius
 *
 * result: 	address where the result will be written
 * returns:	0 on success
 * 			-1 on fail;
 */
int16_t LOLIN_HP303B::getTemp(float *result)
{
	unsigned char buffer[3] = {0};
	//read raw pressure data to buffer

	int16_t i = readBlock(HP303B__REG_ADR_TEMP,
							HP303B__REG_LEN_TEMP,
							buffer);
	if(i != HP303B__REG_LEN_TEMP)
	{
		//something went wrong
		return HP303B__FAIL_UNKNOWN;
	}

	//compose raw temperature value from buffer
	float temp = buffer[0] << 16
					| buffer[1] << 8
					| buffer[2];
	//recognize non-32-bit negative numbers
	//and convert them to 32-bit negative numbers using 2's complement
	if(temp > 0x7FFFFF)
	{
		temp = temp - 0x1000000;
	}

	//return temperature
	*result = calcTemp(temp);
	return HP303B__SUCCEEDED;
}

/**
 * Gets the next pressure measurement result in Pa
 *
 * result: 	address where the result will be written
 * returns: 0 on success
 * 			-1 on fail;
 */
int16_t LOLIN_HP303B::getPressure(float *result)
{
	unsigned char buffer[3] = {0};
	//read raw pressure data to buffer
	int16_t i = readBlock(HP303B__REG_ADR_PRS,
							HP303B__REG_LEN_PRS,
							buffer);
	if(i != HP303B__REG_LEN_PRS)
	{
		//something went wrong
		//negative pressure is not allowed
		return HP303B__FAIL_UNKNOWN;
	}

	//compose raw pressure value from buffer
	float prs = buffer[0] << 16 | buffer[1] << 8 | buffer[2];
	//recognize non-32-bit negative numbers
	//and convert them to 32-bit negative numbers using 2's complement
	if(prs > 0x7FFFFF)
	{
		prs = prs - 0x1000000;
	}

	*result = calcPressure(prs);
	return HP303B__SUCCEEDED;
}

/**
 * reads the next raw value from the HP303B FIFO
 *
 * value: 	address where the value will be written
 * returns:	-1 on fail
 * 			0 if result is a temperature raw value
 * 			1 if result is a pressure raw value
 */
int16_t LOLIN_HP303B::getFIFOvalue(float *value)
{
	//abort on invalid argument
	if(value == NULL)
	{
		return HP303B__FAIL_UNKNOWN;
	}

	unsigned char buffer[HP303B__REG_LEN_PRS] = {0};
	//always read from pressure raw value register
	int16_t i = readBlock(HP303B__REG_ADR_PRS,
							HP303B__REG_LEN_PRS,
							buffer);
	if(i != HP303B__REG_LEN_PRS)
	{
		//something went wrong
		//return error code
		return HP303B__FAIL_UNKNOWN;
	}
	//compose raw pressure value from buffer
	*value =  buffer[0] << 16
			| buffer[1] << 8
			| buffer[2];
	//recognize non-32-bit negative numbers
	//and convert them to 32-bit negative numbers using 2's complement
	if(*value > 0x7FFFFF)
	{
		*value = *value - 0x1000000;
	}

	//least significant bit shows measurement type
	return buffer[2] & HP303B__LSB;
}

/**
 * Calculates a scaled and compensated pressure value from raw data
 * raw: 	raw temperature value read from HP303B
 * returns: temperature value in °C
 */
float LOLIN_HP303B::calcTemp(float raw)
{
	double temp = raw;

	//scale temperature according to scaling table and oversampling
	temp /= scaling_facts[m_tempOsr];

	//update last measured temperature
	//it will be used for pressure compensation
	m_lastTempScal = temp;

	//Calculate compensated temperature
	temp = m_c0Half + m_c1 * temp;

	//return temperature
	return (float)temp;
}

/**
 * Calculates a scaled and compensated pressure value from raw data
 * raw: 	raw pressure value read from HP303B
 * returns: pressure value in Pa
 */
float LOLIN_HP303B::calcPressure(float raw)
{
	double prs = raw;

	//scale pressure according to scaling table and oversampling
	prs /= scaling_facts[m_prsOsr];

	//Calculate compensated pressure
	prs =   m_c00
			+ prs * (m_c10 + prs * (m_c20 + prs * m_c30))
			+ m_lastTempScal * (m_c01 + prs * (m_c11 + prs * m_c21));

	//return pressure
	return (float)prs;
}

/**
 * reads a byte from HP303B
 *
 * regAdress: 	Address that has to be read
 * returns: 	register content or -1 on fail
 */
int16_t LOLIN_HP303B::readByte(uint8_t regAddress)
{
	//delegate to specialized function if HP303B is connected via SPI
	if(m_SpiI2c==0)
	{
		return readByteSPI(regAddress);
	}

	m_i2cbus->beginTransmission(m_slaveAddress);
	m_i2cbus->write(regAddress);
    m_i2cbus->endTransmission(0);
	//request 1 byte from slave
	if(m_i2cbus->requestFrom(m_slaveAddress, 1U, 1U) > 0)
	{
		return m_i2cbus->read();	//return this byte on success
	}
	else
	{
		return HP303B__FAIL_UNKNOWN;	//if 0 bytes were read successfully
	}
}

/**
 * reads a byte from HP303B via SPI
 * this function is automatically called by readByte
 * if HP303B is connected via SPI
 *
 * regAdress: 	Address that has to be read
 * returns: 	register content or -1 on fail
 */
int16_t LOLIN_HP303B::readByteSPI(uint8_t regAddress)
{
	//this function is only made for communication via SPI
	if(m_SpiI2c != 0)
	{
		return HP303B__FAIL_UNKNOWN;
	}
	//mask regAddress
	regAddress &= ~HP303B__SPI_RW_MASK;
	//reserve and initialize bus
	m_spibus->beginTransaction(SPISettings(HP303B__SPI_MAX_FREQ,
											MSBFIRST,
											SPI_MODE3));
	//enable ChipSelect for HP303B
	digitalWrite(m_chipSelect, LOW);
	//send address with read command to HP303B
	m_spibus->transfer(regAddress | HP303B__SPI_READ_CMD);
	//receive register content from HP303B
	uint8_t ret = m_spibus->transfer(0xFF);	//send a dummy byte while receiving
	//disable ChipSelect for HP303B
	digitalWrite(m_chipSelect, HIGH);
	//close current SPI transaction
	m_spibus->endTransaction();
	//return received data
	return ret;
}

/**
 * reads a block from HP303B
 *
 * regAdress: 	Address that has to be read
 * length: 		Length of data block
 * buffer: 	Buffer where data will be stored
 * returns: 	number of bytes that have been read successfully
 * 				NOTE! This is not always equal to length
 * 					  due to rx-Buffer overflow etc.
 */
int16_t LOLIN_HP303B::readBlock(uint8_t regAddress, uint8_t length, uint8_t *buffer)
{
	//delegate to specialized function if HP303B is connected via SPI
	if(m_SpiI2c == 0)
	{
		return readBlockSPI(regAddress, length, buffer);
	}
	//do not read if there is no buffer
	if(buffer == NULL)
	{
		return 0;	//0 bytes read successfully
	}

	m_i2cbus->beginTransmission(m_slaveAddress);
	m_i2cbus->write(regAddress);
    m_i2cbus->endTransmission(0);
	//request length bytes from slave
	int16_t ret = m_i2cbus->requestFrom(m_slaveAddress, length, 1U);
	//read all received bytes to buffer
	for(int16_t count = 0; count < ret; count++)
	{
		buffer[count] = m_i2cbus->read();
	}
	return ret;
}

/**
 * reads a block from HP303B via SPI
 *
 * regAdress: 	Address that has to be read
 * length: 		Length of data block
 * readbuffer: 	Buffer where data will be stored
 * returns: 	number of bytes that have been read successfully
 * 				NOTE! This is not always equal to length
 * 					  due to rx-Buffer overflow etc.
 */
int16_t LOLIN_HP303B::readBlockSPI(uint8_t regAddress, uint8_t length, uint8_t *buffer)
{
	//this function is only made for communication via SPI
	if(m_SpiI2c != 0)
	{
		return HP303B__FAIL_UNKNOWN;
	}
	//do not read if there is no buffer
	if(buffer == NULL)
	{
		return 0;		//0 bytes were read successfully
	}
	//mask regAddress
	regAddress &= ~HP303B__SPI_RW_MASK;
	//reserve and initialize bus
	m_spibus->beginTransaction(SPISettings(HP303B__SPI_MAX_FREQ,
											MSBFIRST,
											SPI_MODE3));
	//enable ChipSelect for HP303B
	digitalWrite(m_chipSelect, LOW);
	//send address with read command to HP303B
	m_spibus->transfer(regAddress | HP303B__SPI_READ_CMD);

	//receive register contents from HP303B
	for(uint8_t count = 0; count < length; count++)
	{
		buffer[count] = m_spibus->transfer(0xFF);//send a dummy byte while receiving
	}

	//disable ChipSelect for HP303B
	digitalWrite(m_chipSelect, HIGH);
	//close current SPI transaction
	m_spibus->endTransaction();
	//return received data
	return length;
}

/**
 * writes a given byte to a given register of HP303B without checking
 *
 * regAdress: 	Address of the register that has to be updated
 * data:		Byte that will be written to the register
 * return:		0 if byte was written successfully
 * 				or -1 on fail
 */
int16_t LOLIN_HP303B::writeByte(uint8_t regAddress, uint8_t data)
{
	return writeByte(regAddress, data, 0U);
}

/**
 * writes a given byte to a given register of HP303B
 *
 * regAdress: 	Address of the register that has to be updated
 * data:		Byte that will be written to the register
 * check: 		If this is true, register content will be read after writing
 * 				to check if update was successful
 * return:		0 if byte was written successfully
 * 				or -1 on fail
 */
int16_t LOLIN_HP303B::writeByte(uint8_t regAddress, uint8_t data, uint8_t check)
{
	//delegate to specialized function if HP303B is connected via SPI
	if(m_SpiI2c==0)
	{
		return writeByteSpi(regAddress, data, check);
	}
	m_i2cbus->beginTransmission(m_slaveAddress);
	m_i2cbus->write(regAddress);			//Write Register number to buffer
	m_i2cbus->write(data);					//Write data to buffer
	if(m_i2cbus->endTransmission() != 0) 	//Send buffer content to slave
	{
		return HP303B__FAIL_UNKNOWN;
	}
	else
	{
		if(check == 0) return 0;			//no checking
		if(readByte(regAddress) == data)	//check if desired by calling function
		{
			return HP303B__SUCCEEDED;
		}
		else
		{
			return HP303B__FAIL_UNKNOWN;
		}
	}
}

/**
 * writes a given byte to a given register of HP303B via SPI
 *
 * regAdress: 	Address of the register that has to be updated
 * data:		Byte that will be written to the register
 * check: 		If this is true, register content will be read after writing
 * 				to check if update was successful
 * return:		0 if byte was written successfully
 * 				or -1 on fail
 */
int16_t LOLIN_HP303B::writeByteSpi(uint8_t regAddress, uint8_t data, uint8_t check)
{
	//this function is only made for communication via SPI
	if(m_SpiI2c != 0)
	{
		return HP303B__FAIL_UNKNOWN;
	}
	//mask regAddress
	regAddress &= ~HP303B__SPI_RW_MASK;
	//reserve and initialize bus
	m_spibus->beginTransaction(SPISettings(HP303B__SPI_MAX_FREQ,
											MSBFIRST,
											SPI_MODE3));
	//enable ChipSelect for HP303B
	digitalWrite(m_chipSelect, LOW);
	//send address with read command to HP303B
	m_spibus->transfer(regAddress | HP303B__SPI_WRITE_CMD);

	//write register content from HP303B
	m_spibus->transfer(data);

	//disable ChipSelect for HP303B
	digitalWrite(m_chipSelect, HIGH);
	//close current SPI transaction
	m_spibus->endTransaction();

	//check if necessary
	if(check == 0)
	{
		//no checking necessary
		return HP303B__SUCCEEDED;
	}
	//checking necessary
	if(readByte(regAddress) == data)
	{
		//check passed
		return HP303B__SUCCEEDED;
	}
	else
	{
		//check failed
		return HP303B__FAIL_UNKNOWN;
	}
}

/**
 * updates some given bits of a given register of HP303B without checking
 *
 * regAdress: 	Address of the register that has to be updated
 * data:		BitValues that will be written to the register
 * shift:		Amount of bits the data byte is shifted (left) before being masked
 * mask: 		Masks the bits of the register that have to be updated
 * 				Bits with value 1 are updated
 * 				Bits with value 0 are not changed
 * return:		0 if byte was written successfully
 * 				or -1 on fail
 */
int16_t LOLIN_HP303B::writeByteBitfield(uint8_t data,
										uint8_t regAddress,
										uint8_t mask,
										uint8_t shift)
{
	return writeByteBitfield(data, regAddress, mask, shift, 0U);
}

/**
 * updates some given bits of a given register of HP303B
 *
 * regAdress: 	Address of the register that has to be updated
 * data:		BitValues that will be written to the register
 * shift:		Amount of bits the data byte is shifted (left) before being masked
 * mask: 		Masks the bits of the register that have to be updated
 * 				Bits with value 1 are updated
 * 				Bits with value 0 are not changed
 * check: 		enables/disables check after writing
 * 				0 disables check
 * 				if check fails, -1 will be returned
 * return:		0 if byte was written successfully
 * 				or -1 on fail
 */
int16_t LOLIN_HP303B::writeByteBitfield(uint8_t data,
										uint8_t regAddress,
										uint8_t mask,
										uint8_t shift,
										uint8_t check)
{
	int16_t old = readByte(regAddress);
	if(old < 0)
	{
		//fail while reading
		return old;
	}
	return writeByte(regAddress, ((uint8_t)old & ~mask)|((data << shift) & mask), check);
}

/**
 * reads some given bits of a given register of HP303B
 *
 * regAdress: 	Address of the register that has to be updated
 * mask: 		Masks the bits of the register that have to be updated
 * 				Bits masked with value 1 are read
 * 				Bits masked with value 0 are set 0
 * shift:		Amount of bits the data byte is shifted (right) after being masked
 * return:		read and processed bits
 * 				or -1 on fail
 */
int16_t LOLIN_HP303B::readByteBitfield(uint8_t regAddress, uint8_t mask, uint8_t shift)
{
	int16_t ret = readByte(regAddress);
	if(ret<0)
	{
		return ret;
	}
	return (((uint8_t)ret) & mask) >> shift;
}
