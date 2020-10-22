/*
 * File:   spi.cpp
 * Author:
 *
 * Created on
 *
 * Inspired from spi speed test from wiringPi
 * wiringPi/examples/spiSpeed.c
 */

#include "spi.h"

#include <wiringPi.h>
#include <wiringPiSPI.h>

#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

#define RF24_SPI_SPEED		8 * 1000000 // 8Mhz
#define RF24_SPI_CHANNEL	0

SPI::SPI():fd(-1)
{
    printf("wiringPi RF24 DRIVER\n");
}

void SPI::begin(int csn_pin)
{
    // initialize the wiringPiSPI
    if ((this->fd = wiringPiSPISetup(RF24_SPI_CHANNEL, RF24_SPI_SPEED)) < 0)
    {
        printf("Cannot configure the SPI device!\n");
        fflush(stdout);
        abort();
    }
    else
        printf("Configured SPI fd: %d - pin: %d\n", fd, csn_pin);
}

uint8_t SPI::transfer(uint8_t tx)
{
    memset(&msgByte, 0, sizeof(msgByte));
    memcpy(&msgByte, &tx, sizeof(tx));

    if(wiringPiSPIDataRW(RF24_SPI_CHANNEL, &msgByte, sizeof(tx)) < 0)
    {
        printf("transfer(): Cannot send data: %s\n", strerror(errno));
        fflush(stdout);
        abort();
    }

    return msgByte;
}

void SPI::transfern(char* buf, uint32_t len)
{
    printf("transfern(tx: %s)\n", buf);

    if(wiringPiSPIDataRW(RF24_SPI_CHANNEL, (uint8_t *)buf, len) < 0)
    {
        printf("transfern(): Cannot send data %s\n", strerror(errno));
        fflush(stdout);
        abort();
    }
}

void SPI::transfernb(char* tbuf, char* rbuf, uint32_t len)
{
    // using an auxiliary buffer to keep tx and rx different
    memset(msg, 0, sizeof(msg));
    memcpy(msg, tbuf, len);

    if(wiringPiSPIDataRW(RF24_SPI_CHANNEL, msg, len) < 0)
    {
        printf("transfernb() Cannot send data %s\n", strerror(errno));
        fflush(stdout);
        abort();
    }

    memcpy(rbuf, msg, len);
}

SPI::~SPI()
{
    if (!(this->fd < 0))
    {
        close(this->fd);
        this->fd = -1;
    }
}
