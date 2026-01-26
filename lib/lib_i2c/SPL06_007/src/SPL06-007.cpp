#include "SPL06-007.h"
#include "Wire.h"

uint8_t SPL_CHIP_ADDRESS = 0x76;

void SPL_init(uint8_t spl_address)
{
	if(spl_address == 0x77)
		SPL_CHIP_ADDRESS = 0x77;
		
	// ---- Oversampling of >8x for temperature or pressuse requires FIFO operational mode which is not implemented ---
	// ---- Use rates of 8x or less until feature is implemented ---
	i2c_eeprom_write_uint8_t(SPL_CHIP_ADDRESS, 0X06, 0x03);	// Pressure 8x oversampling

	i2c_eeprom_write_uint8_t(SPL_CHIP_ADDRESS, 0X07, 0X83);	// Temperature 8x oversampling

	i2c_eeprom_write_uint8_t(SPL_CHIP_ADDRESS, 0X08, 0B0111);	// continuous temp and pressure measurement

	i2c_eeprom_write_uint8_t(SPL_CHIP_ADDRESS, 0X09, 0X00);	// FIFO Pressure measurement  
}

uint8_t get_spl_id()
{
	return i2c_eeprom_read_uint8_t(SPL_CHIP_ADDRESS, 0x0D);	
}

uint8_t get_spl_prs_cfg()
{
	return i2c_eeprom_read_uint8_t(SPL_CHIP_ADDRESS, 0x06);
}

uint8_t get_spl_tmp_cfg()
{
	return i2c_eeprom_read_uint8_t(SPL_CHIP_ADDRESS, 0x07);
}

uint8_t get_spl_meas_cfg()
{
	return i2c_eeprom_read_uint8_t(SPL_CHIP_ADDRESS, 0x08);
}

uint8_t get_spl_cfg_reg()
{
	return i2c_eeprom_read_uint8_t(SPL_CHIP_ADDRESS, 0x09);
}

uint8_t get_spl_int_sts()
{
	return i2c_eeprom_read_uint8_t(SPL_CHIP_ADDRESS, 0x0A);
}

uint8_t get_spl_fifo_sts()
{
	return i2c_eeprom_read_uint8_t(SPL_CHIP_ADDRESS, 0x0B);
}

float get_traw_sc() {
	int32_t traw = get_traw();
	return (float(traw)/get_temperature_scale_factor());
}

float get_temp_c() {
	int16_t c0,c1;
	c0 = get_c0();
	c1 = get_c1();
	float traw_sc = get_traw_sc();
	return (float(c0) * 0.5f) + (float(c1) * traw_sc);
}

float get_temperature_scale_factor()
{
  float k;
  uint8_t tmp_Byte;
  tmp_Byte = i2c_eeprom_read_uint8_t(SPL_CHIP_ADDRESS, 0X07); // MSB
  //tmp_Byte = tmp_Byte >> 4; //Focus on bits 6-4
  tmp_Byte = tmp_Byte & 0B00000111;

  switch (tmp_Byte) 
  {
    case 0B000:
      k = 524288.0f;
    break;

    case 0B001:
      k = 1572864.0f;
    break;

    case 0B010:
      k = 3670016.0f;
    break;

    case 0B011:
      k = 7864320.0f;
    break;

    case 0B100:
      k = 253952.0f;
    break;

    case 0B101:
      k = 516096.0f;
    break;

    case 0B110:
      k = 1040384.0f;
    break;

    case 0B111:
      k = 2088960.0f;
    break;
  }
  return k;
}


int32_t get_traw()
{
  int32_t tmp;
  uint8_t tmp_MSB,tmp_LSB,tmp_XLSB;
  tmp_MSB = i2c_eeprom_read_uint8_t(SPL_CHIP_ADDRESS, 0X03); // MSB

  tmp_LSB = i2c_eeprom_read_uint8_t(SPL_CHIP_ADDRESS, 0X04); // LSB

  tmp_XLSB = i2c_eeprom_read_uint8_t(SPL_CHIP_ADDRESS, 0X05); // XLSB

  tmp = (tmp_MSB << 8) | tmp_LSB;
  tmp = (tmp << 8) | tmp_XLSB;

  if(tmp & (1 << 23))
    tmp = tmp | 0XFF000000; // Set left bits to one for 2's complement conversion of negitive number
  return tmp;
}

float get_praw_sc()
{
	int32_t praw = get_praw();
	return (float(praw)/get_pressure_scale_factor());
}

float get_pcomp()
{
	int32_t c00,c10;
	int16_t c01,c11,c20,c21,c30;
	c00 = get_c00();
	c10 = get_c10();
	c01 = get_c01();
	c11 = get_c11();
	c20 = get_c20();
	c21 = get_c21();
	c30 = get_c30();
	float traw_sc = get_traw_sc();
	float praw_sc = get_praw_sc();
	return float(c00) + praw_sc * (float(c10) + praw_sc * (float(c20) + praw_sc * float(c30))) + traw_sc * float(c01) + traw_sc * praw_sc * ( float(c11) + praw_sc * float(c21));
}

float get_pressure()
{
	return get_pcomp() / 100; // convert to mb
}

float get_pressure_scale_factor()
{
	float k;

	uint8_t tmp_Byte;
	tmp_Byte = i2c_eeprom_read_uint8_t(SPL_CHIP_ADDRESS, 0X06); // MSB

	tmp_Byte = tmp_Byte & 0B00000111; // Focus on 2-0 oversampling rate 


	switch (tmp_Byte) // oversampling rate
	{
		case 0B000:
			k = 524288.0f;
		break;

		case 0B001:
			k = 1572864.0f;
		break;

		case 0B010:
			k = 3670016.0f;
		break;

		case 0B011:
			k = 7864320.0f;
		break;

		case 0B100:
			k = 253952.0f;
		break;

		case 0B101:
			k = 516096.0f;
		break;

		case 0B110:
			k = 1040384.0f;
		break;

		case 0B111:
			k = 2088960.0f;
		break;
	}

	return k;
}

int32_t get_praw()
{
  int32_t tmp;
  uint8_t tmp_MSB,tmp_LSB,tmp_XLSB;
  tmp_MSB = i2c_eeprom_read_uint8_t(SPL_CHIP_ADDRESS, 0X00); // MSB


  tmp_LSB = i2c_eeprom_read_uint8_t(SPL_CHIP_ADDRESS, 0X01); // LSB


  tmp_XLSB = i2c_eeprom_read_uint8_t(SPL_CHIP_ADDRESS, 0X02); // XLSB

  
  tmp = (tmp_MSB << 8) | tmp_LSB;
  tmp = (tmp << 8) | tmp_XLSB;
  
  if(tmp & (1 << 23))
    tmp = tmp | 0XFF000000; // Set left bits to one for 2's complement conversion of negitive number
  
  
  return tmp;
}

int16_t get_c0()
{
  int16_t tmp; 
  uint8_t tmp_MSB,tmp_LSB;
  
  tmp_MSB = i2c_eeprom_read_uint8_t(SPL_CHIP_ADDRESS, 0X10); 
  tmp_LSB = i2c_eeprom_read_uint8_t(SPL_CHIP_ADDRESS, 0X11); 

  tmp_LSB = tmp_LSB >> 4;

  tmp = (tmp_MSB << 4) | tmp_LSB;

  if(tmp & (1 << 11)) // Check for 2's complement negative number
    tmp = tmp | 0XF000; // Set left bits to one for 2's complement conversion of negitive number
  return tmp;
}

int16_t get_c1()
{
  int16_t tmp; 
  uint8_t tmp_MSB,tmp_LSB;
  
  tmp_MSB = i2c_eeprom_read_uint8_t(SPL_CHIP_ADDRESS, 0X11); 
  tmp_LSB = i2c_eeprom_read_uint8_t(SPL_CHIP_ADDRESS, 0X12); 

  tmp_MSB = tmp_MSB & 0XF;

  tmp = (tmp_MSB << 8) | tmp_LSB;

  if(tmp & (1 << 11)) // Check for 2's complement negative number
    tmp = tmp | 0XF000; // Set left bits to one for 2's complement conversion of negitive number
  
  return tmp;
}

int32_t get_c00()
{
  int32_t tmp;
  uint8_t tmp_MSB,tmp_LSB,tmp_XLSB;

  tmp_MSB = i2c_eeprom_read_uint8_t(SPL_CHIP_ADDRESS, 0X13);
  tmp_LSB = i2c_eeprom_read_uint8_t(SPL_CHIP_ADDRESS, 0X14);
  tmp_XLSB = i2c_eeprom_read_uint8_t(SPL_CHIP_ADDRESS, 0X15);
  tmp = (tmp_MSB & 0x80 ? 0xFFF00000 : 0) | ((uint32_t)tmp_MSB << 12) | ((uint32_t)tmp_LSB << 4) | (((uint32_t)tmp_XLSB & 0xF0) >> 4);
  return tmp;
}

int32_t get_c10()
{
  int32_t tmp;
  uint8_t tmp_MSB,tmp_LSB,tmp_XLSB;

  tmp_MSB = i2c_eeprom_read_uint8_t(SPL_CHIP_ADDRESS, 0X15); // 4 bits
  tmp_LSB = i2c_eeprom_read_uint8_t(SPL_CHIP_ADDRESS, 0X16); // 8 bits
  tmp_XLSB = i2c_eeprom_read_uint8_t(SPL_CHIP_ADDRESS, 0X17); // 8 bits
  tmp = (tmp_MSB & 0x8 ? 0xFFF00000 : 0) | (((uint32_t)tmp_MSB & 0x0F) << 16) | ((uint32_t)tmp_LSB << 8) | (uint32_t)tmp_XLSB;
  return tmp;
}

int16_t get_c01()
{
  int16_t tmp; 
  uint8_t tmp_MSB,tmp_LSB;
  
  tmp_MSB = i2c_eeprom_read_uint8_t(SPL_CHIP_ADDRESS, 0X18); 
  tmp_LSB = i2c_eeprom_read_uint8_t(SPL_CHIP_ADDRESS, 0X19); 

  tmp = (tmp_MSB << 8) | tmp_LSB;
  return tmp;
}

int16_t get_c11()
{
  int16_t tmp; 
  uint8_t tmp_MSB,tmp_LSB;
  
  tmp_MSB = i2c_eeprom_read_uint8_t(SPL_CHIP_ADDRESS, 0X1A); 
  tmp_LSB = i2c_eeprom_read_uint8_t(SPL_CHIP_ADDRESS, 0X1B); 

  tmp = (tmp_MSB << 8) | tmp_LSB;
  return tmp;
}

int16_t get_c20()
{
  int16_t tmp; 
  uint8_t tmp_MSB,tmp_LSB;
  
  tmp_MSB = i2c_eeprom_read_uint8_t(SPL_CHIP_ADDRESS, 0X1C); 
  tmp_LSB = i2c_eeprom_read_uint8_t(SPL_CHIP_ADDRESS, 0X1D); 

  tmp = (tmp_MSB << 8) | tmp_LSB;
  return tmp;
}

int16_t get_c21()
{
  int16_t tmp; 
  uint8_t tmp_MSB,tmp_LSB;
  
  tmp_MSB = i2c_eeprom_read_uint8_t(SPL_CHIP_ADDRESS, 0X1E); 
  tmp_LSB = i2c_eeprom_read_uint8_t(SPL_CHIP_ADDRESS, 0X1F); 

  tmp = (tmp_MSB << 8) | tmp_LSB;
  return tmp;
}

int16_t get_c30()
{
  int16_t tmp; 
  uint8_t tmp_MSB,tmp_LSB;
  
  tmp_MSB = i2c_eeprom_read_uint8_t(SPL_CHIP_ADDRESS, 0X20); 
  tmp_LSB = i2c_eeprom_read_uint8_t(SPL_CHIP_ADDRESS, 0X21); 

  tmp = (tmp_MSB << 8) | tmp_LSB;
  return tmp;
}

void i2c_eeprom_write_uint8_t(  uint8_t deviceaddress, uint8_t eeaddress, uint8_t data ) 
{
    uint8_t rdata = data;
    delay(5); // Make sure to delay log enough for EEPROM I2C refresh time
    Wire.beginTransmission(deviceaddress);
    Wire.write((uint8_t)(eeaddress));
    Wire.write(rdata);
    Wire.endTransmission();
}

uint8_t i2c_eeprom_read_uint8_t(  uint8_t deviceaddress, uint8_t eeaddress ) 
{
    uint8_t rdata = 0xFF;
    Wire.beginTransmission(deviceaddress);
    Wire.write(eeaddress); 
    Wire.endTransmission(false); // false to not release the line
    
    Wire.requestFrom(deviceaddress, (uint8_t)1);
    if (Wire.available()) rdata = Wire.read();
    return rdata;
}
