/* Copyright (c) 2002, Marek Michalkiewicz
   Copyright (c) 2005, 2007 Joerg Wunsch
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:

   * Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.

   * Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in
     the documentation and/or other materials provided with the
     distribution.

   * Neither the name of the copyright holders nor the names of
     contributors may be used to endorse or promote products derived
     from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE. 
  
  Modified January 2017 by Bjorn Hammarberg (bjoham@esp8266.com) - i2c slave support
*/

/* $Id$ */
/* copied from: Id: avr/twi.h,v 1.4 2004/11/01 21:19:54 arcanum Exp */

#ifndef _UTIL_TWI_H_
#define _UTIL_TWI_H_ 1

//#include <avr/io.h>

/** \file */
/** \defgroup util_twi <util/twi.h>: TWI bit mask definitions
    \code #include <util/twi.h> \endcode

    This header file contains bit mask definitions for use with
    the AVR TWI interface.
*/
/** \name TWSR values

  Mnemonics:
  <br>TW_MT_xxx - master transmitter
  <br>TW_MR_xxx - master receiver
  <br>TW_ST_xxx - slave transmitter
  <br>TW_SR_xxx - slave receiver
  */

/*@{*/
/* Master */
/** \ingroup util_twi
    \def TW_START
    start condition transmitted */
#define TW_START		0x08

/** \ingroup util_twi
    \def TW_REP_START
    repeated start condition transmitted */
#define TW_REP_START		0x10

/* Master Transmitter */
/** \ingroup util_twi
    \def TW_MT_SLA_ACK
    SLA+W transmitted, ACK received */
#define TW_MT_SLA_ACK		0x18

/** \ingroup util_twi
    \def TW_MT_SLA_NACK
    SLA+W transmitted, NACK received */
#define TW_MT_SLA_NACK		0x20

/** \ingroup util_twi
    \def TW_MT_DATA_ACK
    data transmitted, ACK received */
#define TW_MT_DATA_ACK		0x28

/** \ingroup util_twi
    \def TW_MT_DATA_NACK
    data transmitted, NACK received */
#define TW_MT_DATA_NACK		0x30

/** \ingroup util_twi
    \def TW_MT_ARB_LOST
    arbitration lost in SLA+W or data */
#define TW_MT_ARB_LOST		0x38

/* Master Receiver */
/** \ingroup util_twi
    \def TW_MR_ARB_LOST
    arbitration lost in SLA+R or NACK */
#define TW_MR_ARB_LOST		0x38

/** \ingroup util_twi
    \def TW_MR_SLA_ACK
    SLA+R transmitted, ACK received */
#define TW_MR_SLA_ACK		0x40

/** \ingroup util_twi
    \def TW_MR_SLA_NACK
    SLA+R transmitted, NACK received */
#define TW_MR_SLA_NACK		0x48

/** \ingroup util_twi
    \def TW_MR_DATA_ACK
    data received, ACK returned */
#define TW_MR_DATA_ACK		0x50

/** \ingroup util_twi
    \def TW_MR_DATA_NACK
    data received, NACK returned */
#define TW_MR_DATA_NACK		0x58

/* Slave Transmitter */
/** \ingroup util_twi
    \def TW_ST_SLA_ACK
    SLA+R received, ACK returned */
#define TW_ST_SLA_ACK		0xA8

/** \ingroup util_twi
    \def TW_ST_ARB_LOST_SLA_ACK
    arbitration lost in SLA+RW, SLA+R received, ACK returned */
#define TW_ST_ARB_LOST_SLA_ACK	0xB0

/** \ingroup util_twi
    \def TW_ST_DATA_ACK
    data transmitted, ACK received */
#define TW_ST_DATA_ACK		0xB8

/** \ingroup util_twi
    \def TW_ST_DATA_NACK
    data transmitted, NACK received */
#define TW_ST_DATA_NACK		0xC0

/** \ingroup util_twi
    \def TW_ST_LAST_DATA
    last data byte transmitted, ACK received */
#define TW_ST_LAST_DATA		0xC8

/* Slave Receiver */
/** \ingroup util_twi
    \def TW_SR_SLA_ACK
    SLA+W received, ACK returned */
#define TW_SR_SLA_ACK		0x60

/** \ingroup util_twi
    \def TW_SR_ARB_LOST_SLA_ACK
    arbitration lost in SLA+RW, SLA+W received, ACK returned */
#define TW_SR_ARB_LOST_SLA_ACK	0x68

/** \ingroup util_twi
    \def TW_SR_GCALL_ACK
    general call received, ACK returned */
#define TW_SR_GCALL_ACK		0x70

/** \ingroup util_twi
    \def TW_SR_ARB_LOST_GCALL_ACK
    arbitration lost in SLA+RW, general call received, ACK returned */
#define TW_SR_ARB_LOST_GCALL_ACK 0x78

/** \ingroup util_twi
    \def TW_SR_DATA_ACK
    data received, ACK returned */
#define TW_SR_DATA_ACK		0x80

/** \ingroup util_twi
    \def TW_SR_DATA_NACK
    data received, NACK returned */
#define TW_SR_DATA_NACK		0x88

/** \ingroup util_twi
    \def TW_SR_GCALL_DATA_ACK
    general call data received, ACK returned */
#define TW_SR_GCALL_DATA_ACK	0x90

/** \ingroup util_twi
    \def TW_SR_GCALL_DATA_NACK
    general call data received, NACK returned */
#define TW_SR_GCALL_DATA_NACK	0x98

/** \ingroup util_twi
    \def TW_SR_STOP
    stop or repeated start condition received while selected */
#define TW_SR_STOP		0xA0

/* Misc */
/** \ingroup util_twi
    \def TW_NO_INFO
    no state information available */
#define TW_NO_INFO		0xF8

/** \ingroup util_twi
    \def TW_BUS_ERROR
    illegal start or stop condition */
#define TW_BUS_ERROR		0x00

#if 0

/**
 * \ingroup util_twi
 * \def TW_STATUS_MASK
 * The lower 3 bits of TWSR are reserved on the ATmega163.
 * The 2 LSB carry the prescaler bits on the newer ATmegas.
 */
#define TW_STATUS_MASK		(_BV(TWS7)|_BV(TWS6)|_BV(TWS5)|_BV(TWS4)|\
				_BV(TWS3))
/**
 * \ingroup util_twi
 * \def TW_STATUS
 *
 * TWSR, masked by TW_STATUS_MASK
 */
#define TW_STATUS		(TWSR & TW_STATUS_MASK)
/*@}*/
#endif


/**
 * \name R/~W bit in SLA+R/W address field.
 */

/*@{*/
/** \ingroup util_twi
    \def TW_READ
    SLA+R address */
#define TW_READ		1

/** \ingroup util_twi
    \def TW_WRITE
    SLA+W address */
#define TW_WRITE	0
/*@}*/

#endif  /* _UTIL_TWI_H_ */
