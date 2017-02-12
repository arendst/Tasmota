/*
Copyright (c) 2017 Heiko Krupp.  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/

#ifdef USE_IR_REMOTE
/*********************************************************************************************\
 * IR Remote send using IRremoteESP8266 library
\*********************************************************************************************/

#include <IRremoteESP8266.h>

IRsend *irsend=NULL;

void ir_send_init(void)
{
    irsend = new IRsend(pin[GPIO_IRSEND]); // an IR led is at GPIO_IRSEND
    irsend->begin();
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

boolean ir_send_command(char *type, uint16_t index, char *dataBuf, uint16_t data_len, int16_t payload, char *svalue, uint16_t ssvalue)
{
  boolean serviced = true;
  const char *protocol;
  uint8_t  bits=0;
  uint32_t data=0;
  
  if (!strcmp(type,"IRSEND")) {
	  if(data_len) {
			JsonObject &ir_json = jsonBuffer.parseObject(dataBuf);

    if (!ir_json.success())
      serviced = false;

    if(serviced)
    {
      protocol = ir_json["protocol"];
      bits = ir_json["bits"];
      data = ir_json["data"];

      if(protocol)
      {
        Serial.println(protocol);
        Serial.println(bits);
        Serial.println(data);
		    if      (!strcmp(protocol,"NEC"))     irsend->sendNEC(data, bits);
		    else if (!strcmp(protocol,"SONY"))    irsend->sendSony(data, bits);
			  else if (!strcmp(protocol,"RC5"))     irsend->sendRC5(data, bits);
			  else if (!strcmp(protocol,"RC6"))     irsend->sendRC6(data, bits);
			  else if (!strcmp(protocol,"DISH"))    irsend->sendDISH(data, bits);
			  else if (!strcmp(protocol,"JVC"))     irsend->sendJVC(data, bits, 1);
			  else if (!strcmp(protocol,"SAMSUNG")) irsend->sendSAMSUNG(data, bits);
        else serviced = false;
      } else Serial.println("Invalid JSON data for IR"); 
		} 
  }
  else {
    serviced = false;
  }
  return serviced;
}
#endif  // USE_IR_REMOTE
