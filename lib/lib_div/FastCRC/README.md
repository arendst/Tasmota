FastCRC
=======

Fast CRC Arduino library 
Up to 30 times faster than crc16.h (_avr_libc)

 - uses the on-chip hardware for Teensy 3.0 / 3.1 / 3.2 / 3.5 / 3.6
 - uses fast table-algorithms for other chips
 
List of supported CRC calculations:
-
7 BIT:

CRC7
 (poly=0x09 init=0x00 refin=false refout=false xorout=0x00 check=0x75)
 MultiMediaCard interface

 
8 BIT:

SMBUS
 (poly=0x07 init=0x00 refin=false refout=false xorout=0x00 check=0xf4)
 
MAXIM
 (poly=0x31 init=0x00 refin=true refout=true xorout=0x00  check=0xa1)
 
 
16 BIT:

KERMIT (Alias CRC-16/CCITT, CRC-16/CCITT-TRUE, CRC-CCITT)
 (poly=0x1021 init=0x0000 refin=true refout=true xorout=0x0000 check=0x2189
  Attention: sometimes you'll find byteswapped presentation of result in other implementations)
 
CCITT-FALSE
 (poly=0x1021 init=0xffff refin=false refout=false xorout=0x0000 check=0x29b1)
 
MCRF4XX
 (poly=0x1021 init=0xffff refin=true refout=true xorout=0x0000 check=0x6f91)
 
MODBUS
 (poly=0x8005 init=0xffff refin=true refout=true xorout=0x0000 check=0x4b37)
 
XMODEM (Alias ZMODEM, CRC-16/ACORN)
 (poly=0x1021 init=0x0000 refin=false refout=false xorout=0x0000 check=0x31c3)
 
X25 (Alias CRC-16/IBM-SDLC, CRC-16/ISO-HDLC, CRC-B)
 (poly=0x1021 init=0xffff refin=true refout=true xorout=0xffff check=0x906e)

 
32 BIT:

CRC32, CRC-32/ADCCP, PKZIP, ETHERNET, 802.3
  (poly=0x04c11db7 init=0xffffffff refin=true refout=true xorout=0xffffffff check=0xcbf43926)
  
CKSUM, CRC-32/POSIX
  (poly=0x04c11db7 init=0x00000000 refin=false refout=false xorout=0xffffffff check=0x765e7680)
