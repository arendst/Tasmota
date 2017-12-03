#include "WEMOS_SHT3X.h"

/* Motor()

*/
SHT3X::SHT3X(uint8_t address)
{
	Wire.begin();
	_address=address;
}



byte SHT3X::get()
{
	unsigned int data[6];

	// Start I2C Transmission
	Wire.beginTransmission(_address);
	// Send measurement command
	Wire.write(0x2C);
	Wire.write(0x06);
	// Stop I2C transmission
	if (Wire.endTransmission()!=0) 
		return 1;  

	delay(500);

	// Request 6 bytes of data
	Wire.requestFrom(_address, (uint8_t)6);

	// Read 6 bytes of data
	// cTemp msb, cTemp lsb, cTemp crc, humidity msb, humidity lsb, humidity crc
	for (int i=0;i<6;i++) {
		data[i]=Wire.read();
	};
	
	delay(50);
	
	if (Wire.available()!=0) 
		return 2;

	// Convert the data
	cTemp = ((((data[0] * 256.0) + data[1]) * 175) / 65535.0) - 45;
	fTemp = (cTemp * 1.8) + 32;
	humidity = ((((data[3] * 256.0) + data[4]) * 100) / 65535.0);

	return 0;
}
