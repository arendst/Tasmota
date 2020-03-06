
/**
 * @file spi.h
 * Class declaration for SPI helper files
 */

 /**
 * Example of spi.h class declaration for SPI portability
 *
 * @defgroup Porting_SPI Porting: SPI
 *
 * 
 * @{
 */

#ifndef __XMEGASPI_H__
#define __XMEGASPI_H__

#include <avr/io.h>
#include <stdint.h>
//#include <string.h>
//#include <unistd.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <getopt.h>
//#include <fcntl.h>
//#include <sys/ioctl.h>
//#include <inttypes.h>
//#include <linux/types.h>
//#include <linux/spi/spidev.h> 

#define SPI_SS_bm (1<<4) /* SPI SS pin mask 4 */
#define SPI_MOSI_bm (1<<5) /* SPI MOSI pin mask 5 */
#define SPI_MISO_bm (1<<6) /* SPI MISO pin mask 6 */
#define SPI_SCK_bm (1<<7) /* SPI SCK pin mask 7 */

using namespace std;



class SPI {
public:

	/**
	* SPI constructor
	*/	 
	SPI();
	
	/**
	* Start SPI
	* @param port is the SPI port (XMEGA_SPI_PORT_C for SPI on PORTC, XMEGA_SPI_PORT_D on PORTD etc).
	*/
	void begin(uint8_t port);
	/**
	* Transfer a single byte
	* @param tx_ Byte to send
	* @return Data returned via spi
	*/
	uint8_t transfer(uint8_t tx_);
	
	/**
	* Transfer a buffer of data
	* @param tbuf Transmit buffer
	* @param rbuf Receive buffer
	* @param len Length of the data
	*/
	void transfernb(char* tbuf, char* rbuf, uint32_t len);

	/**
	* Transfer a buffer of data without an rx buffer
	* @param buf Pointer to a buffer of data
	* @param len Length of the data
	*/	
	void transfern(char* buf, uint32_t len);
	
	virtual ~SPI();

private:
    
	/** Default SPI device */
	SPI_t * device;
	/* Port of the SPI */
	PORT_t * port;

	void init();	
};

#endif /*__XMEGASPI_H__*/

/*@}*/