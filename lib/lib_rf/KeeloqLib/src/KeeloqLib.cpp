/*
  Keeloq.cpp - Keeloq encryption/decryption
  Written by Frank Kienast in November, 2010
*/

#include "KeeloqLib.h"

#define KeeLoq_NLF              (0x3A5C742EUL)

Keeloq::Keeloq(const unsigned long keyHigh, const unsigned long keyLow) :
_keyHigh( keyHigh ),
_keyLow( keyLow )
{
}

unsigned long Keeloq::encrypt( const unsigned long data )
{
  unsigned long x = data;
  unsigned long r;
  int keyBitNo, index;
  unsigned long keyBitVal,bitVal;

  for ( r = 0; r < 528; r++ )
  {
    keyBitNo = r & 63;
    if(keyBitNo < 32)
      keyBitVal = bitRead(_keyLow,keyBitNo);
    else
      keyBitVal = bitRead(_keyHigh, keyBitNo - 32);
    index = 1 * bitRead(x,1) + 2 * bitRead(x,9) + 4 * bitRead(x,20) + 8 * bitRead(x,26) + 16 * bitRead(x,31);
    bitVal = bitRead(x,0) ^ bitRead(x, 16) ^ bitRead(KeeLoq_NLF,index) ^ keyBitVal;
    x = (x>>1) ^ bitVal<<31;
  }
  return x;
}

unsigned long Keeloq::decrypt( const unsigned long data )
{
  unsigned long x = data;
  unsigned long r;
  int keyBitNo, index;
  unsigned long keyBitVal,bitVal;

  for (r = 0; r < 528; r++)
  {
    keyBitNo = (15-r) & 63;
    if(keyBitNo < 32)
      keyBitVal = bitRead(_keyLow,keyBitNo);
    else
      keyBitVal = bitRead(_keyHigh, keyBitNo - 32);
    index = 1 * bitRead(x,0) + 2 * bitRead(x,8) + 4 * bitRead(x,19) + 8 * bitRead(x,25) + 16 * bitRead(x,30);
    bitVal = bitRead(x,31) ^ bitRead(x, 15) ^ bitRead(KeeLoq_NLF,index) ^ keyBitVal;
    x = (x<<1) ^ bitVal;
  }
  return x;
 }
 

