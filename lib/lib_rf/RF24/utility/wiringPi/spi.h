
/**
 * @file spi.h
 * Class declaration for SPI helper files
 */

#ifndef SPI_H
#define	SPI_H

 /**
 * Example of spi.h class declaration for SPI portability
 *
 * @defgroup Porting_SPI Porting: SPI
 *
 * @{
 */

#include <stdio.h>
#include <inttypes.h>

using namespace std;
class SPI {
public:
        /**
        * SPI default constructor
        */
        SPI();

        /**
        * Start SPI communication
        * @param pin used for SPI
        */
        void begin(int);

        /**
        * Transfer a single byte of data
        * @param tx Byte to send
        * @return Data returned via spi
        */
        uint8_t transfer(uint8_t);

        /**
        * Transfer a buffer of data using rx and tx buffer
        * @param tbuf Transmit buffer
        * @param rbuf Receive buffer
        * @param len Length of the data
        */
        void transfernb(char*, char*, uint32_t);

        /**
        * Transfer a buffer of data without using an rx buffer
        * @param buf Pointer to a buffer of data
        * @param len Length of the data
        */
        void transfern(char*, const uint32_t);

        /**
        * SPI destructor
        */
        virtual ~SPI();

private:
        int fd;
        uint8_t msg[32 + 1];
        uint8_t msgByte;
};

/*@}*/
#endif	/* SPI_H */

