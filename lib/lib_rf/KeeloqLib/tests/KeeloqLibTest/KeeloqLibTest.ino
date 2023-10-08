#include <KeeloqLib.h>

void setup( void )
{
  Serial.begin(9600);
  
  Keeloq k(0x01020304,0x05060708);

  const unsigned long p   = 6623281UL;
  const unsigned long enc = k.encrypt(p);
  const unsigned long dec = k.decrypt(enc);

  Serial.print("Plaintext    : ");
  Serial.println(p,DEC);
  
  Serial.print("After encrypt: ");
  Serial.println(enc,HEX);
  
  Serial.print("After decrypt: ");
  Serial.println(dec,DEC);
}

void loop( void )
{
  delay(3000);
}

