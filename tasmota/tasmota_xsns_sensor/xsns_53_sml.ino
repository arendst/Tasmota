/*
  xsns_53_sml.ino - SML,OBIS,EBUS,RAW,COUNTER interface for Tasmota

  Created by Gerhard Mutz on 07.10.11.
  adapted for Tasmota

  Copyright (C) 2021  Gerhard Mutz and Theo Arends

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifdef USE_SML_M

#define XSNS_53 53

// this driver depends on use USE_SCRIPT !!!


// debug counter input to led for counter1 and 2
//#define DEBUG_CNT_LED1 2
//#define DEBUG_CNT_LED1 2

#include <TasmotaSerial.h>


// use special no wait serial driver, should be always on
#ifndef ESP32
#define SPECIAL_SS
#endif

// max number of meters , may be adjusted
#ifndef MAX_METERS
#define MAX_METERS 5
#endif


/* additional defines
	USE_ESP32_SW_SERIAL
	default off, uses a special combo driver that allows more then 3 serial ports on ESP32.
	define rec pins as negativ to use software serial

	USE_SML_AUTHKEY
	rarely used , thus off by default
*/

// if you have to save more RAM you may disable these options by defines in user_config_override

#ifndef NO_SML_REPLACE_VARS
// allows to replace values in decoder section with script string variables
#undef SML_REPLACE_VARS
#define SML_REPLACE_VARS
#endif

#ifndef NO_USE_SML_SPECOPT
// allows to define special option 1 for meters that use a direction bit
#undef USE_SML_SPECOPT
#define USE_SML_SPECOPT
#endif

#ifndef NO_USE_SML_SCRIPT_CMD
// allows several sml cmds from scripts, as well as access to sml registers
#undef USE_SML_SCRIPT_CMD
#define USE_SML_SCRIPT_CMD
#endif

#ifndef NO_USE_SML_DECRYPT
// allows 256 bit AES decryption
#define USE_SML_DECRYPT
#endif

#ifndef NO_USE_SML_TCP
// modbus over TCP
#define USE_SML_TCP
#endif

#ifndef NO_SML_OBIS_LINE
// obis in line mode
#define SML_OBIS_LINE
#endif


#ifdef USE_SML_TCP_SECURE
#define USE_SML_TCP_IP_STR
#endif


// median filter eliminates outliers, but uses much RAM and CPU cycles
// 672 bytes extra RAM with SML_MAX_VARS = 16
// default compile on, but must be enabled by descriptor flag 16
// may be undefined if RAM must be saved

#ifndef NO_USE_SML_MEDIAN_FILTER
#undef USE_SML_MEDIAN_FILTER
#define USE_SML_MEDIAN_FILTER
#endif


#ifdef USE_SML_DECRYPT
#include "han_Parser.h"
#endif

/* special options per meter
1:
special binary SML option for meters that use a bit in the status register to sign import or export like ED300L, AS2020 or DTZ541
a. obis code that holds the direction bit,
b. Flag identifier,
c. direction bit,
d. second Flag identifier (some meters use 2 different flags),
e. second bit,
f. obis code of value to be inverted on direction bit.
e.g. 1,=so1,00010800,65,11,65,11,00100700 for DTZ541

2:
flags, currently only bit 0 and 1
if 1 fix DWS74 bug
if 2 use asci obis line compare instead a pattern compare
e.g. 1,=so2,2  set obis line mode on meter 1

3:
serial buffers
a. serial buffer size
b. serial irq buffer size, a must be given
c. dumplog buffer size, default is 128 , a and b must be given
e.g. 1,=so3,256,256  set serial buffers on meter 1

4:
decrytion key, 16 bytes hex btw 32 chars without spaces or commas
defining key switches decryption mode on

5:
authentication key, 16 bytes hex btw 32 chars without spaces or commas
needs USE_SML_AUTHKEY

6:
synchronisation timout in milliseconds, after no serial data within this
time serial pointer is reset to zero

7:
on esp32 the uart index may be set, normally it is allocated from 2 down to 0 automatically
thus you can combine serial SML with serial script , berry or serial drivers.

*/

//#define MODBUS_DEBUG

// ESP32 combined hardware and software serial driver, software read only
#ifdef ESP32
#ifdef USE_ESP32_SW_SERIAL
#ifndef ESP32_SWS_BUFFER_SIZE
#define ESP32_SWS_BUFFER_SIZE 256
#endif

class SML_ESP32_SERIAL : public Stream {
public:
	SML_ESP32_SERIAL(uint32_t uart_index);
  virtual ~SML_ESP32_SERIAL();
  bool begin(uint32_t speed, uint32_t smode, int32_t recpin, int32_t trxpin, int32_t invert);
  int32_t peek(void);
  int read(void) override;
  size_t write(uint8_t byte) override;
  int available(void) override;
  void flush(void) override;
  void setRxBufferSize(uint32_t size);
  void updateBaudRate(uint32_t baud);
  void rxRead(void);
  void end();
  using Print::write;
private:
  // Member variables
  void setbaud(uint32_t speed);
  uint32_t uart_index;
  int8_t m_rx_pin;
  int8_t m_tx_pin;
  uint32_t cfgmode;
  uint32_t ss_byte;
  uint32_t ss_bstart;
  uint32_t ss_index;
  uint32_t m_bit_time;
  uint32_t m_in_pos;
  uint32_t m_out_pos;
  uint16_t serial_buffer_size;
  bool m_valid;
  uint8_t *m_buffer;
  HardwareSerial *hws;
};


void IRAM_ATTR sml_callRxRead(void *self) { ((SML_ESP32_SERIAL*)self)->rxRead(); };

SML_ESP32_SERIAL::SML_ESP32_SERIAL(uint32_t index) {
  uart_index = index;
  m_valid = true;
}

SML_ESP32_SERIAL::~SML_ESP32_SERIAL(void) {
  if (hws) {
    hws->end();
		delete(hws);
  } else {
    detachInterrupt(m_rx_pin);
    if (m_buffer) {
      free(m_buffer);
    }
  }
}

void SML_ESP32_SERIAL::setbaud(uint32_t speed) {
  m_bit_time = ESP.getCpuFreqMHz() * 1000000 / speed;
}

void SML_ESP32_SERIAL::end(void) {
  if (m_buffer) {
    free(m_buffer);
  }
}

bool SML_ESP32_SERIAL::begin(uint32_t speed, uint32_t smode, int32_t recpin, int32_t trxpin, int invert) {
  if (!m_valid) { return false; }

  m_buffer = 0;
  if (recpin < 0) {
    setbaud(speed);
    m_rx_pin = -recpin;
    serial_buffer_size = ESP32_SWS_BUFFER_SIZE;
    m_buffer = (uint8_t*)malloc(serial_buffer_size);
    if (m_buffer == NULL) return false;
    pinMode(m_rx_pin, INPUT_PULLUP);
    attachInterruptArg(m_rx_pin, sml_callRxRead, this, CHANGE);
    m_in_pos = m_out_pos = 0;
    hws = nullptr;
  } else {
    cfgmode = smode;
    m_rx_pin = recpin;
    m_tx_pin = trxpin;
    hws = new HardwareSerial(uart_index);
    if (hws) {
      hws->begin(speed, cfgmode, m_rx_pin, m_tx_pin, invert);
    }
  }
  return true;
}

void SML_ESP32_SERIAL::flush(void) {
  if (hws) {
    hws->flush();
  } else {
    m_in_pos = m_out_pos = 0;
  }
}

int32_t SML_ESP32_SERIAL::peek(void) {
  if (hws) {
    return  hws->peek();
  } else {
    if (m_in_pos == m_out_pos) return -1;
    return m_buffer[m_out_pos];
  }
}

int SML_ESP32_SERIAL::read(void) {
  if (hws) {
    return hws->read();
  } else {
    if (m_in_pos == m_out_pos) return -1;
    uint32_t ch = m_buffer[m_out_pos];
    m_out_pos = (m_out_pos + 1) % serial_buffer_size;
    return ch;
  }
}

int SML_ESP32_SERIAL::available(void) {
  if (hws) {
    return hws->available();
  } else {
    int avail = m_in_pos - m_out_pos;
    if (avail < 0) avail += serial_buffer_size;
    return avail;
  }
}

size_t SML_ESP32_SERIAL::write(uint8_t byte) {
  if (hws) {
    return hws->write(byte);
  }
  return 0;
}

void SML_ESP32_SERIAL::setRxBufferSize(uint32_t size) {
  if (hws) {
    hws->setRxBufferSize(size);
  } else {
    if (m_buffer) {
        free(m_buffer);
    }
    serial_buffer_size = size;
    m_buffer = (uint8_t*)malloc(size);
  }
}
void SML_ESP32_SERIAL::updateBaudRate(uint32_t baud) {
  if (hws) {
    hws->updateBaudRate(baud);
  } else {
    setbaud(baud);
  }
}

// no wait mode only 8N1  (or 7X1, obis only, ignoring parity)
void IRAM_ATTR SML_ESP32_SERIAL::rxRead(void) {
  uint32_t diff;
  uint32_t level;

#define SML_LASTBIT 9

  level = digitalRead(m_rx_pin);

  if (!level && !ss_index) {
    // start condition
    ss_bstart = ESP.getCycleCount() - (m_bit_time / 4);
    ss_byte = 0;
    ss_index++;
  } else {
    // now any bit changes go here
    // calc bit number
    diff = (ESP.getCycleCount() - ss_bstart) / m_bit_time;

    if (!level && diff > SML_LASTBIT) {
      // start bit of next byte, store  and restart
      // leave irq at change
      for (uint32_t i = ss_index; i <= SML_LASTBIT; i++) {
        ss_byte |= (1 << i);
      }
      uint32_t next = (m_in_pos + 1) % serial_buffer_size;
      if (next != (uint32_t)m_out_pos) {
        m_buffer[m_in_pos] = ss_byte >> 1;
        m_in_pos = next;
      }

      ss_bstart = ESP.getCycleCount() - (m_bit_time / 4);
      ss_byte = 0;
      ss_index = 1;
      return;
    }
    if (diff >= SML_LASTBIT) {
      // bit zero was 0,
      uint32_t next = (m_in_pos + 1) % serial_buffer_size;
      if (next != (uint32_t)m_out_pos) {
        m_buffer[m_in_pos] = ss_byte >> 1;
        m_in_pos = next;
      }
      ss_byte = 0;
      ss_index = 0;
    } else {
      // shift in
      for (uint32_t i = ss_index; i < diff; i++) {
        if (!level) ss_byte |= (1 << i);
      }
      ss_index = diff;
    }
  }
}
#endif // USE_ESP32_SW_SERIAL
#endif // ESP32


typedef union {
  uint8_t data;
  struct {
    uint8_t trxenpol : 1;  // string or number
    uint8_t trxen : 1;
    uint8_t trxenpin : 6;
  };
} TRX_EN_TYPE;

typedef union {
  uint8_t data;
  struct {
    uint8_t SO_DWS74_BUG : 1;
    uint8_t SO_OBIS_LINE : 1;
    uint8_t SO_TRX_INVERT : 1;
  };
} SO_FLAGS;

#ifndef TMSBSIZ
#define TMSBSIZ 256
#endif

#ifndef SML_STIMEOUT
#define SML_STIMEOUT 1000
#endif

#define METER_ID_SIZE 24

#define SML_CRYPT_SIZE 16

#ifndef SML_PREFIX_SIZE
#define SML_PREFIX_SIZE 8
#endif

struct METER_DESC {
  int8_t srcpin;
  uint8_t type;
  uint16_t flag;
  int32_t params;
  char prefix[SML_PREFIX_SIZE];
  int8_t trxpin;
  uint8_t tsecs;
  char *txmem;
  uint8_t index;
  uint8_t max_index;
  char *script_str;
  uint8_t sopt;
  TRX_EN_TYPE trx_en;
  bool shift_mode;
  uint16_t sbsiz;
  uint8_t *sbuff;
  uint16_t spos;
  uint16_t sibsiz;
	uint32_t lastms;
	uint16_t tout_ms;
  SO_FLAGS so_flags;
  char meter_id[METER_ID_SIZE];
#ifdef USE_SML_SPECOPT
  uint32_t so_obis1;
  uint32_t so_obis2;
  uint8_t so_fcode1;
  uint8_t so_bpos1;
  uint8_t so_fcode2;
  uint8_t so_bpos2;
#endif // USE_SML_SPECOPT

#ifdef ESP32
#ifndef USE_ESP32_SW_SERIAL
  HardwareSerial *meter_ss;
#else
  SML_ESP32_SERIAL *meter_ss;
#endif
#endif  // ESP32

// software serial pointers
#ifdef ESP8266
  TasmotaSerial *meter_ss;
#endif  // ESP8266

#ifdef USE_SML_DECRYPT
	bool use_crypt = false;
	uint8_t last_iob;
	uint8_t key[SML_CRYPT_SIZE];
	Han_Parser *hp;
#ifdef USE_SML_AUTHKEY
	uint8_t auth[SML_CRYPT_SIZE];
#endif // USE_SML_AUTHKEY
#endif // USE_SML_DECRYPT

#ifdef USE_SML_TCP

#ifdef USE_SML_TCP_IP_STR
  char ip_addr[16];
#else
  IPAddress ip_addr;
#endif // USE_SML_TCP_IP_STR

#ifdef USE_SML_TCP_SECURE
  WiFiClientSecure *client;
#else
  WiFiClient *client;
#endif // USE_SML_TCP_SECURE

#endif // USE_SML_TCP

#ifdef ESP32
  int8_t uart_index;
#endif
};



#define TCP_MODE_FLG 0x7f

struct METER_DESC  meter_desc[MAX_METERS];


// this driver uses double because some meter vars would not fit in float
//=====================================================

// serial buffers, may be made larger depending on telegram lenght
#ifndef SML_BSIZ
#define SML_BSIZ 48
#endif

#define VBUS_SYNC		0xaa
#define SML_SYNC		0x77
#define EBUS_SYNC		0xaa
#define EBUS_ESC    0xa9


// calulate deltas
#define MAX_DVARS MAX_METERS*2

#ifndef SML_DUMP_SIZE
#define SML_DUMP_SIZE 128
#endif

// median filter, should be odd size
#define MEDIAN_SIZE 5
struct SML_MEDIAN_FILTER {
double buffer[MEDIAN_SIZE];
int8_t index;
};


struct SML_GLOBS {
  uint8_t sml_send_blocks;
  uint8_t sml_100ms_cnt;
  uint8_t sml_desc_cnt;
  uint8_t meters_used;
  uint8_t maxvars;
  uint8_t *meter_p;
  double *meter_vars;
  uint8_t *dvalid;
  double dvalues[MAX_DVARS];
  uint32_t dtimes[MAX_DVARS];
  char sml_start;
  uint8_t dump2log = 0;
  uint8_t ser_act_LED_pin = 255;
  uint8_t ser_act_meter_num = 0;
  uint16_t sml_logindex;
  char *log_data;
	uint16_t logsize = SML_DUMP_SIZE;
#if defined(ED300L) || defined(AS2020) || defined(DTZ541) || defined(USE_SML_SPECOPT)
  uint8_t sml_status[MAX_METERS];
  uint8_t g_mindex;
#endif
#ifdef USE_SML_MEDIAN_FILTER
  struct SML_MEDIAN_FILTER *sml_mf;
#endif
	uint8_t *script_meter;
	struct METER_DESC *mp;
  uint8_t to_cnt;
  bool ready;
} sml_globs;



#define SML_OPTIONS_JSON_ENABLE 1
uint8_t sml_options = SML_OPTIONS_JSON_ENABLE;

#ifdef USE_SML_MEDIAN_FILTER

#ifndef FLT_MAX
#define FLT_MAX 99999999
#endif

double sml_median_array(double *array, uint8_t len) {
      uint8_t ind[len];
      uint8_t mind = 0, index = 0, flg;
      double min = FLT_MAX;

      for (uint8_t hcnt = 0; hcnt < len / 2 + 1; hcnt++) {
          for (uint8_t mcnt = 0; mcnt < len; mcnt++) {
              flg = 0;
              for (uint8_t icnt = 0; icnt < index; icnt++) {
                  if (ind[icnt] == mcnt) {
                      flg = 1;
                  }
              }
              if (!flg) {
                  if (array[mcnt] < min) {
                      min = array[mcnt];
                      mind = mcnt;
                  }
              }
          }
          ind[index] = mind;
          index++;
          min = FLT_MAX;
      }
      return array[ind[len / 2]];
  }


// calc median
double sml_median(struct SML_MEDIAN_FILTER* mf, double in) {
  //double tbuff[MEDIAN_SIZE],tmp;
  //uint8_t flag;
  mf->buffer[mf->index] = in;
  mf->index++;
  if (mf->index >= MEDIAN_SIZE) mf->index = 0;

  return sml_median_array(mf->buffer, MEDIAN_SIZE);
/*
  // sort list and take median
  memmove(tbuff,mf->buffer,sizeof(tbuff));
  for (byte ocnt=0; ocnt<MEDIAN_SIZE; ocnt++) {
    flag=0;
    for (byte count=0; count<MEDIAN_SIZE-1; count++) {
      if (tbuff[count]>tbuff[count+1]) {
        tmp=tbuff[count];
        tbuff[count]=tbuff[count+1];
        tbuff[count+1]=tmp;
        flag=1;
      }
    }
    if (!flag) break;
  }
  return tbuff[MEDIAN_SIZE/2];
  */
}
#endif




#define SML_SAVAILABLE Serial_available()
#define SML_SREAD Serial_read()
#define SML_SPEEK Serial_peek()

uint16_t Serial_available() {
  uint8_t num = sml_globs.dump2log & 7;
  if (num < 1 || num > sml_globs.meters_used) num = 1;
  num--;
  if (meter_desc[num].srcpin != TCP_MODE_FLG) {
    if (!meter_desc[num].meter_ss) return 0;
    return meter_desc[num].meter_ss->available();
  } else {
    if (meter_desc[num].client) {
      return meter_desc[num].client->available();
    } else {
      return 0;
    }
  }
}

uint8_t Serial_read() {
  uint8_t num = sml_globs.dump2log & 7;
  if (num < 1 || num > sml_globs.meters_used) num = 1;
  num--;
  if (meter_desc[num].srcpin != TCP_MODE_FLG) {
    if (!meter_desc[num].meter_ss) return 0;
    return meter_desc[num].meter_ss->read();
  } else {
    if (meter_desc[num].client) {
      return meter_desc[num].client->read();
    } else {
      return 0;
    }
  }
}

uint8_t Serial_peek() {
  uint8_t num = sml_globs.dump2log & 7;
  if (num < 1 || num > sml_globs.meters_used) num = 1;
  num--;
  if (meter_desc[num].srcpin != TCP_MODE_FLG) {
    if (!meter_desc[num].meter_ss) return 0;
    return meter_desc[num].meter_ss->peek();
  } else {
    if (meter_desc[num].client) {
      return meter_desc[num].client->peek();
    } else {
      return 0;
    }
  }
}

void sml_dump_start(char c) {
	sml_globs.log_data[0] = ':';
	sml_globs.log_data[1] = c;
	sml_globs.sml_logindex = 2;
}


#define SML_EBUS_SKIP_SYNC_DUMPS


void dump2log(void) {
  int16_t index = 0, hcnt = 0;
  uint32_t d_lastms;
  uint8_t dchars[16];
	uint8_t meter = (sml_globs.dump2log & 7) - 1;
  uint8_t type = sml_globs.mp[meter].type;

  //if (!SML_SAVAILABLE) return;
	if (!sml_globs.log_data) return;

#ifdef USE_SML_DECRYPT
	struct METER_DESC *mp = &meter_desc[meter];
	if (mp->use_crypt == true) {
			d_lastms = millis();
      while ((millis() - d_lastms) < 50) {
        while (SML_SAVAILABLE) {
					d_lastms = millis();
					uint16_t logsiz;
					uint8_t *payload;
					if (mp->hp->readHanPort(&payload, &logsiz)) {
						if (logsiz > mp->sbsiz) {
							logsiz = mp->sbsiz;
						}
						memmove(mp->sbuff, payload, logsiz);
						AddLog(LOG_LEVEL_INFO, PSTR("SML: decrypted block: %d bytes"), logsiz);
						uint16_t index = 0;
						while (logsiz) {
							sml_dump_start('>');
							for (uint16_t cnt = 0; cnt < 16; cnt++) {
								sprintf(&sml_globs.log_data[sml_globs.sml_logindex], "%02x ", mp->sbuff[index++]);
								if (sml_globs.sml_logindex < sml_globs.logsize - 7) {
				          sml_globs.sml_logindex += 3;
				        }
								logsiz--;
								if (!logsiz) {
									break;
								}
							}
							AddLogData(LOG_LEVEL_INFO, sml_globs.log_data);
						}
					} else {
						// dump serial buffer
						sml_dump_start(' ');
						while (index < mp->spos) {
							sprintf(&sml_globs.log_data[sml_globs.sml_logindex], "%02x ", mp->sbuff[index++]);
							if (sml_globs.sml_logindex >= 32*3+2) {
								AddLogData(LOG_LEVEL_INFO, sml_globs.log_data);
								sml_dump_start(' ');
							}
						}
					}
        }
      }
			if (sml_globs.sml_logindex > 2) {
				AddLogData(LOG_LEVEL_INFO, sml_globs.log_data);
				sml_dump_start(' ');
			}
			mp->hp->len = 0;
			return;
	}
#endif

  if (sml_globs.dump2log & 8) {
    // combo mode
    while (SML_SAVAILABLE) {
      sml_globs.log_data[index] = ':';
      index++;
      sml_globs.log_data[index] = ' ';
      index++;
      d_lastms = millis();
      while ((millis() - d_lastms) < 40) {
        if (SML_SAVAILABLE) {
          uint8_t c = SML_SREAD;
          sprintf(&sml_globs.log_data[index], "%02x ", c);
          dchars[hcnt] = c;
          index += 3;
          hcnt++;
          if (hcnt > 15) {
            // line complete, build asci chars
            sml_globs.log_data[index++] = '=';
            sml_globs.log_data[index++] = '>';
            sml_globs.log_data[index++] = ' ';
            for (uint8_t ccnt = 0; ccnt < 16; ccnt++) {
              if (isprint(dchars[ccnt])) {
                sml_globs.log_data[index] = dchars[ccnt];
              } else {
                sml_globs.log_data[index] = ' ';
              }
              index++;
            }
            break;
          }
        }
      }
      if (index > 0) {
        sml_globs.log_data[index] = 0;
        AddLogData(LOG_LEVEL_INFO, sml_globs.log_data);
        index = 0;
        hcnt = 0;
      }
    }
  } else {
		switch (type) {
     	case 'o':
      	// obis
      	while (SML_SAVAILABLE) {
        	char c = SML_SREAD&0x7f;
        	if (c == '\n' || c == '\r') {
          	if (sml_globs.sml_logindex > 2) {
            	sml_globs.log_data[sml_globs.sml_logindex] = 0;
            	AddLogData(LOG_LEVEL_INFO, sml_globs.log_data);
							sml_dump_start(' ');
          	}
          	continue;
        	}
        	sml_globs.log_data[sml_globs.sml_logindex] = c;
        	if (sml_globs.sml_logindex < sml_globs.logsize - 2) {
          	sml_globs.sml_logindex++;
        	}
      	}
				break;
     	case 'v':
      	// vbus
				{ uint8_t c;
      	while (SML_SAVAILABLE) {
        	c = SML_SREAD;
        	if (c == VBUS_SYNC) {
          	sml_globs.log_data[sml_globs.sml_logindex] = 0;
          	AddLogData(LOG_LEVEL_INFO, sml_globs.log_data);
						sml_dump_start(' ');
        	}
        	sprintf(&sml_globs.log_data[sml_globs.sml_logindex], "%02x ", c);
        	if (sml_globs.sml_logindex < sml_globs.logsize - 7) {
          	sml_globs.sml_logindex += 3;
        	}
      	}
				}
				break;
     	case 'e':
      	// ebus
      	{ uint8_t c, p;
      	while (SML_SAVAILABLE) {
        	c = SML_SREAD;
        	if (c == EBUS_SYNC) {
          	p = SML_SPEEK;
          	if (p != EBUS_SYNC && sml_globs.sml_logindex > 5) {
            	// new packet, plot last one
            	sml_globs.log_data[sml_globs.sml_logindex] = 0;
            	AddLogData(LOG_LEVEL_INFO, sml_globs.log_data);
            	strcpy(&sml_globs.log_data[0], ": aa ");
            	sml_globs.sml_logindex = 5;
          	}
          	continue;
        	}
        	sprintf(&sml_globs.log_data[sml_globs.sml_logindex], "%02x ", c);
        	if (sml_globs.sml_logindex < sml_globs.logsize - 7) {
          	sml_globs.sml_logindex += 3;
        	}
      	}
				}
				break;
     	case 's':
      	// sml
      	{ uint8_t c;
      	while (SML_SAVAILABLE) {
        	c = SML_SREAD;
        	if (c == SML_SYNC) {
						sml_globs.log_data[sml_globs.sml_logindex] = 0;
          	AddLogData(LOG_LEVEL_INFO, sml_globs.log_data);
						sml_dump_start(' ');
        	}
        	sprintf(&sml_globs.log_data[sml_globs.sml_logindex], "%02x ", c);
        	if (sml_globs.sml_logindex < sml_globs.logsize - 7) {
          	sml_globs.sml_logindex += 3;
        	}
      	}
				}
				break;
    	default:
      	// raw dump
      	d_lastms = millis();
      	sml_dump_start(' ');
				while ((millis() - d_lastms) < 40) {
					while (SML_SAVAILABLE) {
						d_lastms = millis();
						yield();
          	sprintf(&sml_globs.log_data[sml_globs.sml_logindex], "%02x ", SML_SREAD);
						if (sml_globs.sml_logindex < sml_globs.logsize - 7) {
	          	sml_globs.sml_logindex += 3;
	        	}
						if (sml_globs.sml_logindex >= 32*3+2) {
							AddLogData(LOG_LEVEL_INFO, sml_globs.log_data);
							sml_dump_start(' ');
						}
					}
      	}
      	if (sml_globs.sml_logindex > 2) {
        	sml_globs.log_data[sml_globs.sml_logindex] = 0;
        	AddLogData(LOG_LEVEL_INFO, sml_globs.log_data);
      	}
				break;

	 	}
  }
}

void Hexdump(uint8_t *sbuff, uint32_t slen) {
  char cbuff[slen*3+10];
  char *cp = cbuff;
  *cp++ = '>';
  *cp++ = ' ';
  for (uint32_t cnt = 0; cnt < slen; cnt ++) {
    sprintf(cp, "%02x ", sbuff[cnt]);
    cp += 3;
  }
  AddLogData(LOG_LEVEL_INFO, cbuff);
}

#define DOUBLE2CHAR dtostrfd


// skip sml entries
uint8_t *skip_sml(uint8_t *cp,int16_t *res) {
    uint8_t len,len1,type;
    len=*cp&0xf;
    type=*cp&0x70;
    if (type==0x70) {
        // list, skip entries
        // list
        cp++;
        while (len--) {
            len1=*cp&0x0f;
            cp+=len1;
        }
         *res=0;
    } else {
        // skip len
        *res=(signed char)*(cp+1);
        cp+=len;
    }
    return cp;
}

// get sml binary value
// not defined for unsigned >0x7fff ffff ffff ffff (should never happen)
double sml_getvalue(unsigned char *cp, uint8_t index) {
uint8_t len,unit,type;
int16_t scaler,result;
int64_t value;
double dval;

  // scan for values
  // check status
#ifdef ED300L
  unsigned char *cpx=cp-5;
  // decode OBIS 0180 amd extract direction info
  if (*cp==0x64 && *cpx==0 && *(cpx+1)==0x01 && *(cpx+2)==0x08 && *(cpx+3)==0) {
      sml_globs.sml_status[sml_globs.g_mindex]=*(cp+3);
  }
  if (*cp==0x63 && *cpx==0 && *(cpx+1)==0x01 && *(cpx+2)==0x08 && *(cpx+3)==0) {
      sml_globs.sml_status[sml_globs.g_mindex]=*(cp+2);
  }
#endif
#ifdef AS2020
  unsigned char *cpx=cp-5;
  // decode OBIS 0180 amd extract direction info
  if (*cp==0x64 && *cpx==0 && *(cpx+1)==0x01 && *(cpx+2)==0x08 && *(cpx+3)==0) {
      sml_globs.sml_status[sml_globs.g_mindex]=*(cp+2);
  }
  if (*cp==0x63 && *cpx==0 && *(cpx+1)==0x01 && *(cpx+2)==0x08 && *(cpx+3)==0) {
      sml_globs.sml_status[sml_globs.g_mindex]=*(cp+1);
  }
#endif
#ifdef DTZ541
  unsigned char *cpx=cp-5;
  // decode OBIS 0180 amd extract direction info
  if (*cp==0x65 && *cpx==0 && *(cpx+1)==0x01 && *(cpx+2)==0x08 && *(cpx+3)==0) {
    sml_globs.sml_status[sml_globs.g_mindex]=*(cp+3);
  }
#endif

#ifdef USE_SML_SPECOPT
 unsigned char *cpx = cp - 5;
 uint32_t ocode = (*(cpx + 0) << 24) | (*(cpx + 1) << 16) | (*(cpx + 2) << 8) | (*(cpx + 3) << 0);

 if (ocode == meter_desc[sml_globs.g_mindex].so_obis1) {
   sml_globs.sml_status[sml_globs.g_mindex] &= 0xfe;
   uint32_t flag = 0;
   uint16_t bytes = 0;
   if (*cp == meter_desc[sml_globs.g_mindex].so_fcode1) {
     cpx = cp + 1;
     bytes = (meter_desc[sml_globs.g_mindex].so_fcode1 & 0xf) - 1;
     for (uint16_t cnt = 0; cnt < bytes; cnt++) {
        flag <<= 8;
        flag |= *cpx++;
     }
     if (flag & (1 << meter_desc[sml_globs.g_mindex].so_bpos1)) {
       sml_globs.sml_status[sml_globs.g_mindex] |= 1;
     }
   }
   if (*cp == meter_desc[sml_globs.g_mindex].so_fcode2) {
     cpx = cp + 1;
     bytes = (meter_desc[sml_globs.g_mindex].so_fcode2 & 0xf) - 1;
     for (uint16_t cnt = 0; cnt < bytes; cnt++) {
       flag <<= 8;
       flag |= *cpx++;
     }
     if (flag & (1 << meter_desc[sml_globs.g_mindex].so_bpos1)) {
       sml_globs.sml_status[sml_globs.g_mindex] |= 1;
     }
   }
 }
#endif

  cp = skip_sml(cp, &result);
  // check time
  cp = skip_sml(cp, &result);
  // check unit
  cp = skip_sml(cp, &result);
  // check scaler
  cp = skip_sml(cp, &result);
  scaler = result;

  // get value
  type = *cp & 0x70;
  len = *cp & 0x0f;
  cp++;
    if (type == 0x50 || type == 0x60) {
        // shift into 64 bit
        uint64_t uvalue = 0;
        uint8_t nlen = len;
        while (--nlen) {
            uvalue <<= 8;
            uvalue |= *cp++;
        }
        if (type == 0x50) {
            // signed
            switch (len - 1) {
                case 1:
                    // byte
                    value = (signed char)uvalue;
                    break;
                case 2:
                    // signed 16 bit
                    if (meter_desc[index].so_flags.SO_DWS74_BUG) {
                      if (scaler == -2) {
                        value = (uint32_t)uvalue;
                      } else {
                        value = (int16_t)uvalue;
                      }
                    } else {
                      value = (int16_t)uvalue;
                    }
                    break;
                case 3:
                  // signed 24 bit
                  value = (int32_t)(uvalue << 8);
                  value /= 256;
                  break;

                case 4:
                    // signed 32 bit
                    value = (int32_t)uvalue;
                    break;
                case 5:
                case 6:
                case 7:
                case 8:
                    // signed 64 bit
                    value = (int64_t)uvalue;
                    break;
            }
        } else {
            // unsigned
            value = uvalue;
        }

    } else {
        if (!(type & 0xf0)) {
            // octet string serial number
            // no coding found on the net
            // up to now 2 types identified on Hager
            if (len == 9) {
              // serial number on hager => 24 bit - 24 bit
                cp++;
                uint32_t s1,s2;
                s1 = *cp << 16 | *(cp + 1) <<8 | *(cp + 2);
                cp += 4;
                s2 = *cp << 16 | *(cp + 1) <<8 | *(cp + 2);
                sprintf(&meter_desc[index].meter_id[0], "%u-%u", s1, s2);
            } else {
                // server id on hager
                char *str = &meter_desc[index].meter_id[0];
                for (type = 0; type < len - 1; type++) {
                    sprintf(str,"%02x", *cp++);
                    str += 2;
                }
            }
            value = 0;
        } else {
            value = 999999;
            scaler = 0;
        }
    }
    dval = value;
    if (scaler == -1) {
      dval /= 10;
    } else if (scaler == -2) {
      dval /= 100;
    } else if (scaler == -3) {
      dval /= 1000;
    } else if (scaler == -4) {
      dval /= 10000;
    } else if (scaler == 1) {
      dval *= 10;
    } else if (scaler == 2) {
      dval *= 100;
    } else if (scaler == 3) {
      dval *= 1000;
    }
  #ifdef ED300L
    // decode current power OBIS 00 0F 07 00
    if (*cpx==0x00 && *(cpx+1)==0x0f && *(cpx+2)==0x07 && *(cpx+3)==0) {
        if (sml_globs.sml_status[sml_globs.g_mindex]&0x20) {
          // and invert sign on solar feed
          dval*=-1;
        }
    }
  #endif
  #ifdef AS2020
    // decode current power OBIS 00 10 07 00
    if (*cpx==0x00 && *(cpx+1)==0x10 && *(cpx+2)==0x07 && *(cpx+3)==0) {
        if (sml_globs.sml_status[sml_globs.g_mindex]&0x08) {
          // and invert sign on solar feed
          dval*=-1;
        }
    }
  #endif
  #ifdef DTZ541
    // decode current power OBIS 00 10 07 00
    if (*cpx==0x00 && *(cpx+1)==0x10 && *(cpx+2)==0x07 && *(cpx+3)==0) {
        if (sml_globs.sml_status[sml_globs.g_mindex]&0x08) {
          // and invert sign on solar feed
          dval*=-1;
        }
    }
  #endif
  #ifdef USE_SML_SPECOPT
    ocode = (*(cpx + 0) << 24) | (*(cpx + 1) << 16) | (*(cpx + 2) << 8) | (*(cpx + 3) << 0);
    if (ocode == meter_desc[sml_globs.g_mindex].so_obis2) {
      if (sml_globs.sml_status[sml_globs.g_mindex] & 1) {
        // and invert sign on solar feed
        dval *= -1;
      }
    }
  #endif

    return dval;
}

uint8_t hexnibble(char chr) {
  uint8_t rVal = 0;
  if (isdigit(chr)) {
    rVal = chr - '0';
  } else  {
    chr=toupper(chr);
    if (chr >= 'A' && chr <= 'F') rVal = chr + 10 - 'A';
  }
  return rVal;
}

uint8_t sb_counter;

// need double precision in this driver
double CharToDouble(const char *str)
{
  // simple ascii to double, because atof or strtod are too large
  char strbuf[24];

  strlcpy(strbuf, str, sizeof(strbuf));
  char *pt = strbuf;
  while ((*pt != '\0') && isspace(*pt)) { pt++; }  // Trim leading spaces

  signed char sign = 1;
  if (*pt == '-') { sign = -1; }
  if (*pt == '-' || *pt=='+') { pt++; }            // Skip any sign

  double left = 0;
  if (*pt != '.') {
    left = atoll(pt);                               // Get left part
    while (isdigit(*pt)) { pt++; }                 // Skip number
  }

  double right = 0;
  if (*pt == '.') {
    pt++;
    right = atoll(pt);                              // Decimal part
    while (isdigit(*pt)) {
      pt++;
      right /= 10.0;
    }
  }

  double result = left + right;
  if (sign < 0) {
    return -result;                                // Add negative sign
  }
  return result;
}


// remove ebus escapes
void ebus_esc(uint8_t *ebus_buffer, unsigned char len) {
    short count,count1;
    for (count = 0; count < len; count++) {
        if (ebus_buffer[count] == EBUS_ESC) {
            //found escape
            ebus_buffer[count] += ebus_buffer[count + 1];
            // remove 2. char
            count++;
            for (count1 = count; count1 < len; count1++) {
                ebus_buffer[count1] = ebus_buffer[count1 + 1];
            }
        }
    }

}

uint8_t ebus_crc8(uint8_t data, uint8_t crc_init) {
	uint8_t crc;
	uint8_t polynom;
	int i;

	crc = crc_init;
	for (i = 0; i < 8; i++) {
		if (crc & 0x80) {
			polynom = (uint8_t) 0x9B;
		}
		else {
			polynom = (uint8_t) 0;
		}
		crc = (uint8_t)((crc & ~0x80) << 1);
		if (data & 0x80) {
			crc = (uint8_t)(crc | 1) ;
		}
		crc = (uint8_t)(crc ^ polynom);
		data = (uint8_t)(data << 1);
	}
	return (crc);
}

// ebus crc
uint8_t ebus_CalculateCRC( uint8_t *Data, uint16_t DataLen ) {
	uint16_t i;
	uint8_t Crc = 0;
	for(i = 0 ; i < DataLen ; ++i, ++Data ) {
      Crc = ebus_crc8( *Data, Crc );
   }
   return Crc;
}

void sml_empty_receiver(uint32_t meters) {
  while (meter_desc[meters].meter_ss->available()) {
    meter_desc[meters].meter_ss->read();
  }
}

void sml_shift_in(uint32_t meters, uint32_t shard) {
  uint32_t count;

  struct METER_DESC *mp = &meter_desc[meters];

  if (!mp->sbuff) return;

#ifdef USE_SML_DECRYPT
	if (mp->use_crypt) {
		if (mp->hp) {
			uint32_t timediff = millis() - mp->lastms;
			if (timediff > mp->tout_ms) {
				mp->hp->len = 0;
				mp->spos = 0;
				AddLog(LOG_LEVEL_DEBUG, PSTR("SML: sync"));
			}
			mp->lastms = millis();
			uint16_t len;
			uint8_t *payload;
			if (mp->hp->readHanPort(&payload, &len)) {
				if (len > mp->sbsiz) {
					len = mp->sbsiz;
				}
				memmove(mp->sbuff, payload, len);
				AddLog(LOG_LEVEL_DEBUG, PSTR("SML: decrypted block: %d bytes"), len);
				SML_Decode(meters);
			}
		}
		return;
	}
#endif

  if (mp->shift_mode) {
    // shift in
    for (count = 0; count < mp->sbsiz - 1; count++) {
      mp->sbuff[count] = mp->sbuff[count + 1];
    }
  }
    
  uint8_t iob;
  if (mp->srcpin != TCP_MODE_FLG) {
    iob = (uint8_t)mp->meter_ss->read(); 
  } else {
    if (mp->client) {
      iob = (uint8_t)mp->client->read();
    } else {
      iob = 0;
    }
  }

  switch (mp->type) {
    case 'o':
      // asci obis
      if (!(mp->so_flags.SO_OBIS_LINE)) {
        mp->sbuff[mp->sbsiz - 1] = iob & 0x7f;
      } else {
        iob &= 0x7f;
        mp->sbuff[mp->spos] = iob;
        mp->spos++;
        if (mp->spos >= mp->sbsiz) {
          mp->spos = 0;
        }
        if ((iob == 0x0a) || (iob == 0x0d)) {
          SML_Decode(meters);
          mp->spos = 0;
        }
      }
      break;
    case 's':
      // binary obis = sml
      mp->sbuff[mp->sbsiz - 1] = iob;
      break;
    case 'r':
      // raw with shift
      mp->sbuff[mp->sbsiz - 1] = iob;
      break;
    case 'R':
      // raw without shift
			{
			uint32_t timediff = millis() - mp->lastms;
			if (timediff > mp->tout_ms) {
				mp->spos = 0;
				AddLog(LOG_LEVEL_DEBUG, PSTR("SML: sync"));
			}
			mp->lastms = millis();
      mp->sbuff[mp->spos] = iob;
      mp->spos++;
      if (mp->spos > mp->sbsiz) {
        mp->spos = 0;
      }
			}
      break;
    case 'k':
      // Kamstrup
      if (iob == 0x40) {
        mp->spos = 0;
      } else if (iob == 0x0d) {
        uint8_t index = 0;
        uint8_t *ucp = &mp->sbuff[0];
        for (uint16_t cnt = 0; cnt < mp->spos; cnt++) {
          uint8_t iob = mp->sbuff[cnt] ;
          if (iob == 0x1b) {
            *ucp++ = mp->sbuff[cnt + 1]  ^ 0xff;
            cnt++;
          } else {
            *ucp++ = iob;
          }
          index++;
        }
        uint16_t crc = KS_calculateCRC(&mp->sbuff[0], index);
        if (!crc) {
          SML_Decode(meters);
        }
        sml_empty_receiver(meters);
        mp->spos = 0;
      } else {
        mp->sbuff[mp->spos] = iob;
        mp->spos++;
        if (mp->spos >= mp->sbsiz) {
          mp->spos = 0;
        }
      }
      break;
    case 'm':
    case 'M':
      // modbus
      mp->sbuff[mp->spos] = iob;
      mp->spos++;
      if (mp->spos >= mp->sbsiz) {
        mp->spos = 0;
      }
      if (mp->srcpin == TCP_MODE_FLG) {
        // tcp read
        if (mp->spos >= 6) {
          uint8_t tlen = (mp->sbuff[4] << 8) | mp->sbuff[5];
          if (mp->spos == 6 + tlen) {
            mp->spos = 0;
            memmove(&mp->sbuff[0], &mp->sbuff[6], mp->sbsiz - 6);
            SML_Decode(meters);
            if (mp->client) {
              mp->client->flush();
            }
            //Hexdump(mp->sbuff + 6, 10);
          }
        }
        break;
      }

      if (mp->spos >= 3) {
        uint32_t mlen = mp->sbuff[2] + 5;
        if (mlen > mp->sbsiz) mlen = mp->sbsiz;
        if (mp->spos >= mlen) {
#ifdef MODBUS_DEBUG
          AddLog(LOG_LEVEL_INFO, PSTR("receive index >> %d"), mp->index);
          Hexdump(mp->sbuff, 10);
#endif
          SML_Decode(meters);
          sml_empty_receiver(meters);
          mp->spos = 0;
        }
      }
      break;
    case 'p':
      // pzem
      mp->sbuff[mp->spos] = iob;
      mp->spos++;
      if (mp->spos >= 7) {
        SML_Decode(meters);
        sml_empty_receiver(meters);
        mp->spos = 0;
      }
      break;
    case 'v':
      // vbus
      if (iob == EBUS_SYNC) {
        sb_counter = 0;
        SML_Decode(meters);
        mp->sbuff[0] = iob;
        mp->spos = 1;
      } else {
        if (mp->spos < mp->sbsiz) {
          mp->sbuff[mp->spos] = iob;
          mp->spos++;
        }
      }
      break;
    case 'e':
      // ebus
      if (iob == EBUS_SYNC) {
        // should be end of telegramm
        // QQ,ZZ,PB,SB,NN ..... CRC, ACK SYNC
        if (mp->spos > 4 + 5) {
          // get telegramm lenght
          uint16_t tlen = mp->sbuff[4] + 5;
          // test crc
          if (mp->sbuff[tlen] = ebus_CalculateCRC(mp->sbuff, tlen)) {
              ebus_esc(mp->sbuff, tlen);
              SML_Decode(meters);
          } else {
              // crc error
              //AddLog(LOG_LEVEL_INFO, PSTR("ebus crc error"));
          }
        }
        mp->spos = 0;
        return;
      }
      mp->sbuff[mp->spos] = iob;
      mp->spos++;
      if (mp->spos >= mp->sbsiz) {
        mp->spos = 0;
      }
      break;
  }
  sb_counter++;

  if (mp->shift_mode) {
    SML_Decode(meters);
  }
}


uint16_t sml_count = 0;

// polled every 50 ms
void SML_Poll(void) {
uint32_t meters;

    for (meters = 0; meters < sml_globs.meters_used; meters++) {
      struct METER_DESC *mp = &meter_desc[meters];
      if (mp->type != 'c') {
        if (mp->srcpin != TCP_MODE_FLG) {
          if (!mp->meter_ss) continue;
          // poll for serial input
          if (sml_globs.ser_act_LED_pin != 255 && (sml_globs.ser_act_meter_num == 0 || sml_globs.ser_act_meter_num - 1 == meters)) {
            digitalWrite(sml_globs.ser_act_LED_pin, mp->meter_ss->available() && !digitalRead(sml_globs.ser_act_LED_pin)); // Invert LED, if queue is continuously full
          }
          while (mp->meter_ss->available()) {
            sml_shift_in(meters, 0);
          }
        } else {
#ifdef USE_SML_TCP
          if (mp->client) {    
            while (mp->client->available()){
              sml_shift_in(meters, 0);
            }
          }
#endif
        }
      }
    }
}

#define VBUS_BAD_CRC 0
// get vbus septet with 6 bytes
uint32_t vbus_get_septet(uint8_t *cp) {
  uint32_t result = 0;

  //AddLog(LOG_LEVEL_INFO,PSTR("septet: %02x %02x %02x %02x %02x %02x"),cp[0] ,cp[1],cp[2],cp[3],cp[4],cp[5]);

  uint8_t Crc = 0x7F;
  for (uint32_t i = 0; i < 5; i++) {
    Crc = (Crc - cp[i]) & 0x7f;
  }
  if (Crc != cp[5]) {
    result = VBUS_BAD_CRC;
  } else {
    result = (cp[3] | ((cp[4]&8)<<4));
    result <<= 8;
    result |= (cp[2] | ((cp[4]&4)<<5));
    result <<= 8;
    result |= (cp[1] | ((cp[4]&2)<<6));
    result <<= 8;
    result |= (cp[0] | ((cp[4]&1)<<7));
  }

  //AddLog(LOG_LEVEL_INFO,PSTR("septet r: %d"),result);
  return result;
}


char *skip_double(char *cp) {
  if (*cp == '+' || *cp == '-') {
    cp++;
  }
  while (*cp) {
    if (*cp == '.') {
      cp++;
    }
    if (!isdigit(*cp)) {
      return cp;
    }
    cp++;
  }
  return 0;
}

uint8_t *sml_find(uint8_t *src, uint16_t ssize, uint8_t *pattern, uint16_t psize) {
	//AddLog(LOG_LEVEL_INFO, PSTR(">> %02x %02x %02x %02x"),pattern[0],pattern[1],pattern[2],pattern[3]);
	if (psize >= ssize) {
		return 0;
	}
	for (uint32_t cnt = 0; cnt < ssize - psize; cnt++) {
		if (!memcmp(src, pattern, psize)) {
			return src;
		}
		src++;
	}
	return 0;
}

#ifdef USE_SML_DECRYPT
double sml_get_obis_value(uint8_t *data) {
	double out = 0;
	CosemData *item = (CosemData *)data;
	switch (item->base.type) {
		case CosemTypeLongSigned: {
				out = ntohs(item->ls.data);
				break;
		}
		case CosemTypeLongUnsigned: {
				out = ntohs(item->lu.data);
				break;
		}
		case CosemTypeDLongSigned: {
				out = ntohl(item->dlu.data);
				break;
		}
		case CosemTypeDLongUnsigned: {
				out = ntohl(item->dlu.data);
				break;
		}
		case CosemTypeLong64Signed: {
				out = ntohll(item->l64s.data);
				break;
		}
		case CosemTypeLong64Unsigned: {
				out = ntohll(item->l64u.data);
				break;
		}
	}
	return out;
}
#endif // USE_SML_DECRYPT



void SML_Decode(uint8_t index) {
  const char *mp = (const char*)sml_globs.meter_p;
  int8_t mindex;
  uint8_t *cp;
  uint8_t dindex = 0, vindex = 0;
  delay(0);

  if (!sml_globs.ready) {
    return;
  }

  while (mp != NULL) {
    // check list of defines
    if (*mp == 0) break;

    // new section
    mindex = ((*mp) & 7) - 1;

    if (mindex < 0 || mindex >= sml_globs.meters_used) mindex = 0;
    mp += 2;
    if (*mp == '=' && *(mp+1) == 'h') {
      mp = strchr(mp, '|');
      if (mp) mp++;
      continue;
    }

    if (*mp == '=' && *(mp+1) == 's') {
      mp = strchr(mp, '|');
      if (mp) mp++;
      continue;
    }

    // =d must handle dindex
    if (*mp == '=' && *(mp + 1) == 'd') {
      if (index != mindex) {
        dindex++;
      }
    }

    if (index != mindex) goto nextsect;

    // start of serial source buffer
    cp = meter_desc[mindex].sbuff;

    // compare
    if (*mp == '=') {
      // calculated entry, check syntax
      mp++;
      // do math m 1+2+3
      if (*mp == 'm' && !sb_counter) {
        // only every 256 th byte
        // else it would be calculated every single serial byte
        mp++;
        while (*mp == ' ') mp++;
        // 1. index
        double dvar;
        uint8_t opr;
        uint8_t mind;
        int32_t ind;
        mind = strtol((char*)mp, (char**)&mp, 10);
        if (mind < 1 || mind > sml_globs.maxvars) mind = 1;
        dvar = sml_globs.meter_vars[mind - 1];
        while (*mp==' ') mp++;
        for (uint8_t p = 0; p < 8; p++) {
          if (*mp == '@') {
            // store result
            sml_globs.meter_vars[vindex] = dvar;
            mp++;
            break;
          }
          opr = *mp;
          mp++;
          uint8_t iflg = 0;
          if (*mp == '#') {
            iflg = 1;
            mp++;
          }
          ind = strtol((char*)mp, (char**)&mp, 10);
          mind = ind;
          if (mind < 1 || mind > sml_globs.maxvars) mind = 1;
          switch (opr) {
              case '+':
                if (iflg) dvar += ind;
                else dvar += sml_globs.meter_vars[mind - 1];
                break;
              case '-':
                if (iflg) dvar -= ind;
                else dvar -= sml_globs.meter_vars[mind - 1];
                break;
              case '*':
                if (iflg) dvar *= ind;
                else dvar *= sml_globs.meter_vars[mind - 1];
                break;
              case '/':
                if (iflg) dvar /= ind;
                else dvar /= sml_globs.meter_vars[mind - 1];
                break;
          }
          while (*mp==' ') mp++;
          if (*mp == '@') {
            // store result
            sml_globs.meter_vars[vindex] = dvar;
            mp++;
            break;
          }
        }
        double fac = CharToDouble((char*)mp);
        sml_globs.meter_vars[vindex] /= fac;
        SML_Immediate_MQTT((const char*)mp, vindex, mindex);
        sml_globs.dvalid[vindex] = 1;
        // get sfac
      } else if (*mp == 'd') {
        // calc deltas d ind 10 (eg every 10 secs)
        if (dindex < MAX_DVARS) {
          // only n indexes
          mp++;
          while (*mp == ' ') mp++;
          uint8_t ind = atoi(mp);
          while (*mp >= '0' && *mp <= '9') mp++;
          if (ind < 1 || ind > sml_globs.maxvars) ind = 1;
          uint32_t delay = atoi(mp) * 1000;
          uint32_t dtime = millis() - sml_globs.dtimes[dindex];
          if (dtime > delay) {
            // calc difference
            sml_globs.dtimes[dindex] = millis();
            double vdiff = sml_globs.meter_vars[ind - 1] - sml_globs.dvalues[dindex];
            sml_globs.dvalues[dindex] = sml_globs.meter_vars[ind - 1];
            double dres = (double)360000.0 * vdiff / ((double)dtime / 10000.0);

            sml_globs.dvalid[vindex] += 1;

            if (sml_globs.dvalid[vindex] >= 2) {
              // differece is only valid after 2. calculation
              sml_globs.dvalid[vindex] = 2;
#ifdef USE_SML_MEDIAN_FILTER
              if (sml_globs.mp[mindex].flag & 16) {
                sml_globs.meter_vars[vindex] = sml_median(&sml_globs.sml_mf[vindex], dres);
              } else {
                sml_globs.meter_vars[vindex] = dres;
              }
#else
              sml_globs.meter_vars[vindex] = dres;
#endif
            }
            mp=strchr(mp,'@');
            if (mp) {
              mp++;
              double fac = CharToDouble((char*)mp);
              sml_globs.meter_vars[vindex] /= fac;
              SML_Immediate_MQTT((const char*)mp, vindex, mindex);
            }
          }
          //sml_globs.dvalid[vindex] = 1;
          dindex++;
        }
      } else if (*mp == 'h') {
        // skip html tag line
        mp = strchr(mp, '|');
        if (mp) mp++;
        continue;
      } else if (*mp == 's') {
        // skip spec option tag line
        mp = strchr(mp, '|');
        if (mp) mp++;
        continue;
      }
    } else {
      // compare value
      uint8_t found = 1;
      double ebus_dval = 99;
      double mbus_dval = 99;
      while (*mp != '@') {
        if (found == 0) {
          // skip rest of decoder part
          mp++;
          continue;
        }
        if (sml_globs.mp[mindex].type == 'o' || sml_globs.mp[mindex].type == 'c') {
          if (*mp++ != *cp++) {
            found=0;
          }
        } else {
          if (sml_globs.mp[mindex].type == 's') {
            // sml
            uint8_t val = hexnibble(*mp++) << 4;
            val |= hexnibble(*mp++);
            if (val != *cp++) {
              found = 0;
            }
          } else {
            // ebus modbus pzem vbus or raw
						if (!strncmp(mp, "pm(", 3)) {
							// pattern match
							uint8_t dp = 0;
							mp += 3;
							// default to asci obis
							uint8_t aflg = 3;
							if (*mp == 'r') {
								aflg = 0;
								mp++;
							} else if (*mp == 'h') {
								aflg = 1;
								mp++;
							}
							uint8_t pattern[64];
							// check for obis pattern
							for (uint32_t cnt = 0; cnt < sizeof(pattern); cnt++) {
								if (*mp == '@' || !*mp) {
									break;
								}
								if (*mp == ')') {
									mp++;
									if ((aflg & 2) && (dp == 2)) {
										pattern[cnt] = 0xff;
										cnt++;
									}
									pattern[cnt] = 0;
									uint8_t *ucp = sml_find(cp, meter_desc[index].sbsiz, pattern, cnt);
									if (ucp) {
										cp = ucp + cnt;
										// check auto type
										if (aflg & 1) {
											// METER_ID_SIZE
#ifdef USE_SML_DECRYPT
											CosemData *item = (CosemData *)cp;
											switch (item->base.type) {
            						case CosemTypeString:
                					memcpy(meter_desc[mindex].meter_id, item->str.data, item->str.length);
                					meter_desc[mindex].meter_id[item->str.length] = 0;
                					break;
            						case CosemTypeOctetString:
                					memcpy(meter_desc[mindex].meter_id, item->oct.data, item->oct.length);
                					meter_desc[mindex].meter_id[item->oct.length] = 0;
                					break;
												default:
													ebus_dval = sml_get_obis_value(cp);
        							}
#endif
										}
									}
									break;
								}
								uint8_t iob;
								if (aflg & 2) {
									iob = strtol((char*)mp, (char**)&mp, 10);
									if (*mp == '.') {
										mp++;
										dp++;
									}
								} else {
									iob = hexnibble(*mp++) << 4;
									iob |= hexnibble(*mp++);
								}
								pattern[cnt] = iob;
							}
						} else if (*mp == 'x') {
              if (*(mp + 1) == 'x') {
                //ignore one byte
                mp += 2;
                cp++;
              } else {
                mp++;
                if (isdigit(*mp)) {
                  uint8_t skip = strtol((char*)mp, (char**)&mp, 10);
                  cp += skip;
                }
              }
            } else if (!strncmp(mp, "U64", 3)) {
              uint32_t valh = (cp[0]<<24) | (cp[1]<<16) | (cp[2]<<8) | (cp[3]<<0);
              uint32_t vall = (cp[4]<<24) | (cp[5]<<16) | (cp[6]<<8) | (cp[7]<<0);
              uint64_t val = ((uint64_t)valh<<32) | vall;
              mp += 3;
              cp += 8;
              ebus_dval = val;
              mbus_dval = val;
            } else if (!strncmp(mp, "u64", 3)) {
              uint64_t valh = (cp[1]<<24) | (cp[0]<<16) | (cp[3]<<8) | (cp[2]<<0);
              uint64_t vall = (cp[5]<<24) | (cp[4]<<16) | (cp[7]<<8) | (cp[6]<<0);
              uint64_t val = ((uint64_t)valh<<32) | vall;
              mp += 3;
              cp += 8;
              ebus_dval = val;
              mbus_dval = val;
            } else if (!strncmp(mp, "U32", 3)) {
              mp += 3;
              goto U32_do;
            } else if (!strncmp(mp, "UUuuUUuu", 8)) {
              mp += 8;
              U32_do:
              uint32_t val = (cp[0]<<24) | (cp[1]<<16) | (cp[2]<<8) | (cp[3]<<0);
              cp += 4;
              if (*mp == 's') {
                mp++;
                // swap words
                val = (val>>16) | (val<<16);
              }
              ebus_dval = val;
              mbus_dval = val;
            } else if (!strncmp(mp, "u32", 3)) {
              mp += 3;
              goto u32_do;
            } else if (!strncmp(mp, "uuUUuuUU", 8)) {
              mp += 8;
              u32_do:
              uint32_t val = (cp[1]<<24) | (cp[0]<<16) | (cp[3]<<8) | (cp[2]<<0);
              cp += 4;
              if (*mp == 's') {
                mp++;
                // swap words
                val = (val>>16) | (val<<16);
              }
              ebus_dval = val;
              mbus_dval = val;
            } else if (!strncmp(mp, "UUuu", 4)) {
              uint16_t val = cp[1] | (cp[0]<<8);
              mbus_dval = val;
              ebus_dval = val;
              mp += 4;
              cp += 2;
            } else if (!strncmp(mp, "S32", 3)) {
              mp += 3;
              goto S32_do;
            } else if (!strncmp(mp, "SSssSSss", 8)) {
              mp += 8;
              S32_do:
              int32_t val = (cp[0]<<24) | (cp[1]<<16) | (cp[2]<<8) | (cp[3]<<0);
              cp += 4;
              if (*mp == 's') {
                mp++;
                // swap words
                val = ((uint32_t)val>>16) | ((uint32_t)val<<16);
              }
              ebus_dval = val;
              mbus_dval = val;
            } else if (!strncmp(mp, "s32", 3)) {
              mp += 3;
              goto s32_do;
            } else if (!strncmp(mp, "ssSSssSS", 8)) {
              mp += 8;
              s32_do:
              int32_t val = (cp[1]<<24) | (cp[0]<<16) | (cp[3]<<8) | (cp[2]<<0);
              cp += 4;
              if (*mp == 's') {
                mp++;
                // swap words
                val = ((uint32_t)val>>16) | ((uint32_t)val<<16);
              }
              ebus_dval = val;
              mbus_dval = val;
            } else if (!strncmp(mp, "uuUU", 4)) {
              uint16_t val = cp[0] | (cp[1]<<8);
              mbus_dval = val;
              ebus_dval = val;
              mp += 4;
              cp += 2;
            } else if (!strncmp(mp, "uu", 2)) {
              uint8_t val = *cp++;
              mbus_dval = val;
              ebus_dval = val;
              mp += 2;
            } else if (!strncmp(mp, "ssSS", 4)) {
              int16_t val = *cp | (*(cp+1)<<8);
              mbus_dval = val;
              ebus_dval = val;
              mp += 4;
              cp += 2;
            } else if (!strncmp(mp, "SSss", 4)) {
              int16_t val = cp[1] | (cp[0]<<8);
              mbus_dval = val;
              ebus_dval = val;
              mp += 4;
              cp += 2;
            } else if (!strncmp(mp,"ss", 2)) {
              int8_t val = *cp++;
              mbus_dval = val;
              ebus_dval = val;
              mp += 2;
            } else if (!strncmp(mp, "ffffffff", 8)) {
              uint32_t val = (cp[0]<<24) | (cp[1]<<16) | (cp[2]<<8) | (cp[3]<<0);
              float *fp = (float*)&val;
              ebus_dval = *fp;
              mbus_dval = *fp;
              mp += 8;
              cp += 4;
            } else if (!strncmp(mp, "FFffFFff", 8)) {
              // reverse word float
              uint32_t val = (cp[1]<<0) | (cp[0]<<8) | (cp[3]<<16) | (cp[2]<<24);
              float *fp = (float*)&val;
              ebus_dval = *fp;
              mbus_dval = *fp;
              mp += 8;
              cp += 4;
            } else if (!strncmp(mp, "eeeeee", 6)) {
              uint32_t val = (cp[0]<<16) | (cp[1]<<8) | (cp[2]<<0);
              mbus_dval = val;
              mp += 6;
              cp += 3;
            } else if (!strncmp(mp, "vvvvvv", 6)) {
              mbus_dval = (float)((cp[0]<<8) | (cp[1])) + ((float)cp[2]/10.0);
              mp += 6;
              cp += 3;
            } else if (!strncmp(mp, "cccccc", 6)) {
              mbus_dval = (float)((cp[0]<<8) | (cp[1])) + ((float)cp[2]/100.0);
              mp += 6;
              cp += 3;
            } else if (!strncmp(mp, "pppp", 4)) {
              mbus_dval = (float)((cp[0]<<8) | cp[1]);
              mp += 4;
              cp += 2;
            }  else if (!strncmp(mp, "kstr", 4)) {
              mp += 4;
              // decode the mantissa
              uint32_t x = 0;
              for (uint16_t i = 0; i < cp[1]; i++) {
                x <<= 8;
                x |= cp[i + 3];
              }
              // decode the exponent
              int32_t i = cp[2] & 0x3f;
              if (cp[2] & 0x40) {
                i = -i;
              };
              //float ifl = pow(10, i);
              float ifl = 1;
              for (uint16_t x = 1; x <= i; ++x) {
                ifl *= 10;
              }
              if (cp[2] & 0x80) {
                ifl = -ifl;
              }
              mbus_dval = (double )(x * ifl);

            } else if (!strncmp(mp, "bcd", 3)) {
              mp += 3;
              uint8_t digits = strtol((char*)mp, (char**)&mp, 10);
              if (digits < 2) digits = 2;
              if (digits > 12) digits = 12;
              uint64_t bcdval = 0;
              uint64_t mfac = 1;
              for (uint32_t cnt = 0; cnt < digits; cnt += 2) {
                uint8_t iob = *cp++;
                bcdval += (iob & 0xf) * mfac;
                mfac *= 10;
                bcdval += (iob >> 4) * mfac;
                mfac *= 10;
              }
              mbus_dval = bcdval;
              ebus_dval = bcdval;
            } else if (*mp == 'v') {
              // vbus values vul, vsl, vuwh, vuwl, wswh, vswl, vswh
              // vub3, vsb3 etc
              mp++;
              int16_t offset = -1;
              if (*mp == 'o') {
                mp++;
                offset = strtol((char*)mp, (char**)&mp, 10);
                cp += (offset / 4) * 6;
              }
              uint8_t usign;
              if (*mp == 'u') {
                usign = 1;
              } else if (*mp == 's') {
                usign = 0;
              }
              mp++;
              switch (*mp) {
                case 'l':
                  mp++;
                  // get long value
                  if (usign) {
                    ebus_dval = vbus_get_septet(cp);
                  } else {
                    ebus_dval = (int32_t)vbus_get_septet(cp);
                  }
                  break;
                case 'w':
                  mp++;
                  char wflg;
                  if (offset >= 0) {
                    if (offset % 4) {
                      wflg = 'h';
                    } else {
                      wflg = 'l';
                    }
                  } else {
                    wflg = *mp;
                    mp++;
                  }
                  // get word value
                  if (wflg == 'h') {
                    // high word
                    if (usign) ebus_dval = (vbus_get_septet(cp) >> 16) & 0xffff;
                    else ebus_dval = (int16_t)((vbus_get_septet(cp) >> 16) & 0xffff);
                  } else {
                    // low word
                    if (usign) ebus_dval = vbus_get_septet(cp) & 0xffff;
                    else ebus_dval = (int16_t)(vbus_get_septet(cp) & 0xffff);
                  }
                  break;
                case 'b':
                  mp++;
                  char bflg;
                  if (offset >= 0) {
                    bflg = 0x30 | (offset % 4);
                  } else {
                    bflg = *mp;
                    mp++;
                  }
                  switch (bflg) {
                    case '3':
                      if (usign) ebus_dval = vbus_get_septet(cp) >> 24;
                      else ebus_dval = (int8_t)(vbus_get_septet(cp) >> 24);
                      break;
                    case '2':
                      if (usign) ebus_dval = (vbus_get_septet(cp) >> 16) & 0xff;
                      else ebus_dval = (int8_t)((vbus_get_septet(cp) >> 16) & 0xff);
                      break;
                    case '1':
                      if (usign) ebus_dval = (vbus_get_septet(cp) >> 8) & 0xff;
                      else ebus_dval = (int8_t)((vbus_get_septet(cp) >> 8) & 0xff);
                      break;
                    case '0':
                      if (usign) ebus_dval = vbus_get_septet(cp) & 0xff;
                      else ebus_dval = (int8_t)(vbus_get_septet(cp) & 0xff);
                      break;
                  }
                  break;
                case 't':
                  mp++;
                  { uint16_t time;
                    if (offset % 4) {
                      time = (vbus_get_septet(cp) >> 16) & 0xffff;
                    } else {
                      time = vbus_get_septet(cp) & 0xffff;
                    }
                    sprintf(&meter_desc[index].meter_id[0], "%02d:%02d", time / 60, time % 60);
                  }
                  break;
              }
              cp += 6;
            }
            else {
              uint8_t val = hexnibble(*mp++) << 4;
              val |= hexnibble(*mp++);
              if (val != *cp++) {
                found = 0;
              }
            }
          }
        }
      }
      if (found) {
        // matches, get value
        sml_globs.dvalid[vindex] = 1;
        mp++;
#if defined(ED300L) || defined(AS2020) || defined(DTZ541) || defined(USE_SML_SPECOPT)
        sml_globs.g_mindex = mindex;
#endif
        if (*mp == '#') {
          // get string value
          getstr:
          mp++;
          if (sml_globs.mp[mindex].type != 'v') {
            if (sml_globs.mp[mindex].type == 'o') {
              uint32_t p;
              for (p = 0; p < METER_ID_SIZE - 2; p++) {
                if (*cp == *mp) {
                  break;
                }
                meter_desc[mindex].meter_id[p] = *cp++;
              }
              meter_desc[mindex].meter_id[p] = 0;
            } else if (sml_globs.mp[mindex].type == 'k') {
              // 220901
              uint32_t date = mbus_dval;
              uint8_t year = date / 10000; // = 22
              date -= year * 10000;
              uint8_t month = date / 100; // = 09
              uint8_t day = date % 100; // = 01
              sprintf(&meter_desc[mindex].meter_id[0],"%02d.%02d.%02d",day, month, year);
            } else {
              sml_getvalue(cp, mindex);
            }
          }
        } else {
          double dval;
          char type = sml_globs.mp[mindex].type;
          if (type != 'e' && type != 'r' && type != 'R' && type != 'm' && type != 'M' && type != 'k' && type != 'p' && type != 'v') {
            // get numeric values
            if (type == 'o' || type == 'c') {
              if (*mp == '(') {
                mp++;
                // skip this number of brackets
                uint8_t toskip = strtol((char*)mp,(char**)&mp, 10);
                mp++;
                char *lcp = (char*)cp;
                if (toskip) {
                  char *bp = (char*)cp;
                  for (uint32_t cnt = 0; cnt < toskip; cnt++) {
                    bp = strchr(bp, '(');
                    if (!bp) {
                      break;
                    }
                    bp++;
                    lcp = bp;
                  }
                }
                if (*mp == '#') {
                  cp = (uint8_t*)lcp;
                  goto getstr;
                }
                dval = CharToDouble((char*)lcp);
              } else {
                dval = CharToDouble((char*)cp);
              }
            } else {
              dval = sml_getvalue(cp, mindex);
            }
          } else {
            // ebus pzem vbus or mbus or raw
            if (*mp == 'b') {
              mp++;
              uint8_t shift = *mp&7;
              ebus_dval = (uint32_t)ebus_dval >> shift;
              ebus_dval = (uint32_t)ebus_dval & 1;
              mp+=2;
            }
            if (*mp == 'i') {
              // mbus index
              mp++;
              uint8_t mb_index = strtol((char*)mp, (char**)&mp, 10);
              if (mb_index != sml_globs.mp[mindex].index) {
                goto nextsect;
              }
              if (sml_globs.mp[mindex].type == 'k') {
                // crc is already checked, get float value
                dval = mbus_dval;
                mp++;
              } else {
                if (meter_desc[mindex].srcpin != TCP_MODE_FLG) {
                  uint16_t pos = meter_desc[mindex].sbuff[2] + 3;
                  if (pos > (meter_desc[mindex].sbsiz - 2)) pos = meter_desc[mindex].sbsiz - 2;
                  uint16_t crc = MBUS_calculateCRC(&meter_desc[mindex].sbuff[0], pos, 0xFFFF);
                  if (lowByte(crc) != meter_desc[mindex].sbuff[pos]) goto nextsect;
                  if (highByte(crc) != meter_desc[mindex].sbuff[pos + 1]) goto nextsect;
                }
                dval = mbus_dval;
                //AddLog(LOG_LEVEL_INFO, PSTR(">> %s"),mp);
                mp++;
              }
            } else {
              if (sml_globs.mp[mindex].type == 'p') {
                uint8_t crc = SML_PzemCrc(&meter_desc[mindex].sbuff[0],6);
                if (crc != meter_desc[mindex].sbuff[6]) goto nextsect;
                dval = mbus_dval;
              } else {
                dval = ebus_dval;
              }
            }

          }
#ifdef USE_SML_MEDIAN_FILTER
          if (sml_globs.mp[mindex].flag & 16) {
            sml_globs.meter_vars[vindex] = sml_median(&sml_globs.sml_mf[vindex], dval);
          } else {
            sml_globs.meter_vars[vindex] = dval;
          }
#else
          sml_globs.meter_vars[vindex] = dval;
#endif

          //AddLog(LOG_LEVEL_INFO, PSTR(">> %s"),mp);
          // get scaling factor
          double fac = CharToDouble((char*)mp);
          // get optional offset to calibrate meter
          char *cp = skip_double((char*)mp);
          if (cp && (*cp == '+' || *cp == '-')) {
            double offset = CharToDouble(cp);
            sml_globs.meter_vars[vindex] += offset;
          }
          sml_globs.meter_vars[vindex] /= fac;
          SML_Immediate_MQTT((const char*)mp, vindex, mindex);
        }
      }
      //AddLog(LOG_LEVEL_INFO, PSTR("set valid in line %d"), vindex);
    }
nextsect:
    // next section
    if (vindex < sml_globs.maxvars - 1) {
      vindex++;
    }
    mp = strchr(mp, '|');
    if (mp) mp++;
  }
}

//"1-0:1.8.0*255(@1," D_TPWRIN ",kWh," DJ_TPWRIN ",4|"
void SML_Immediate_MQTT(const char *mp,uint8_t index,uint8_t mindex) {
  char tpowstr[32];
  char jname[24];

  // we must skip sf,webname,unit
  char *cp = strchr(mp, ',');
  if (cp) {
    cp++;
    // wn
    cp = strchr(cp,',');
    if (cp) {
      cp++;
      // unit
      cp = strchr(cp,',');
      if (cp) {
        cp++;
        // json mqtt
        for (uint8_t count = 0; count < sizeof(jname); count++) {
          if (*cp == ',') {
            jname[count] = 0;
            break;
          }
          jname[count] = *cp++;
        }
        cp++;
        uint8_t dp = atoi(cp);
        if (dp & 0x10) {
          // immediate mqtt
          DOUBLE2CHAR(sml_globs.meter_vars[index], dp & 0xf, tpowstr);
          ResponseTime_P(PSTR(",\"%s\":{\"%s\":%s}}"), sml_globs.mp[mindex].prefix, jname, tpowstr);
          MqttPublishTeleSensor();
        }
      }
    }
  }
}

// web + json interface
void SML_Show(boolean json) {
  int8_t count, mindex, cindex = 0;
  char tpowstr[32];
  char name[24];
  char unit[8];
  char jname[24];
  int8_t index = 0, mid = 0;
  char *mp = (char*)sml_globs.meter_p;
  char *cp, nojson = 0;
  //char b_mqtt_data[MESSZ];
  //b_mqtt_data[0]=0;

    if (!sml_globs.meters_used) return;

    int8_t lastmind = ((*mp) & 7) - 1;
    if (lastmind < 0 || lastmind >= sml_globs.meters_used) lastmind = 0;
    while (mp != NULL) {
        if (*mp == 0) break;
        // setup sections
        mindex = ((*mp) & 7) - 1;

        if (mindex < 0 || mindex >= sml_globs.meters_used) mindex = 0;
        if (sml_globs.mp[mindex].prefix[0] == '*' && sml_globs.mp[mindex].prefix[1] == 0) {
          nojson = 1;
        } else {
          nojson = 0;
        }
        mp += 2;
        if (*mp == '=' && *(mp + 1) == 'h') {
          mp += 2;
          // html tag
          if (json) {
            mp = strchr(mp, '|');
            if (mp) mp++;
            continue;
          }
          // web ui export
          uint8_t i;
          for (i = 0; i < sizeof(tpowstr) - 2; i++) {
            if (*mp == '|' || *mp == 0) break;
            tpowstr[i] = *mp++;
          }
          tpowstr[i] = 0;
#ifdef USE_WEBSERVER
          // export html
          //snprintf_P(b_mqtt_data, sizeof(b_mqtt_data), "%s{s}%s{e}", b_mqtt_data,tpowstr);
          WSContentSend_PD(PSTR("{s}%s{e}"), tpowstr);
#endif  // USE_WEBSERVER
          // rewind, to ensure strchr
          mp--;
          mp = strchr(mp, '|');
          if (mp) mp++;
          continue;
        }
        if (*mp == '=' && *(mp + 1) == 's') {
          mp = strchr(mp, '|');
          if (mp) mp++;
          continue;
        }
        // skip compare section
        cp = strchr(mp, '@');
        if (cp) {
          cp++;
          tststr:
          if (*cp == '#') {
            // meter id
            if (*(cp + 1) == 'x') {
              // convert hex to asci
              sml_hex_asci(mindex, tpowstr);
            } else {
              sprintf(tpowstr,"\"%s\"", &meter_desc[mindex].meter_id[0]);
            }
            mid = 1;
          } else if (*cp == '(') {
            if (sml_globs.mp[mindex].type == 'o') {
              cp++;
              strtol((char*)cp,(char**)&cp, 10);
              cp++;
              goto tststr;
            } else {
              mid = 0;
            }
          } else if (*cp == 'b') {
            // bit value
#ifdef SML_BIT_TEXT
            sprintf_P(tpowstr, PSTR("\"%s\""), (uint8_t)sml_globs.meter_vars[index]?D_ON:D_OFF);
            mid = 2;
#endif
          } else {
            mid = 0;
          }
          // skip scaling
          cp = strchr(cp, ',');
          if (cp) {
            // this is the name in web UI
            cp++;
            for (count = 0; count < sizeof(name); count++) {
              if (*cp == ',') {
                name[count] = 0;
                break;
              }
              name[count] = *cp++;
            }
            cp++;

            for (count = 0; count < sizeof(unit); count++) {
              if (*cp == ',') {
                unit[count] = 0;
                break;
              }
              unit[count] = *cp++;
            }
            cp++;

            for (count = 0; count < sizeof(jname); count++) {
              if (*cp == ',') {
                jname[count] = 0;
                break;
              }
              jname[count] = *cp++;
            }

            cp++;

            if (!mid) {
              uint8_t dp = atoi(cp) & 0xf;
              DOUBLE2CHAR(sml_globs.meter_vars[index], dp, tpowstr);
            }

            if (json) {
              //if (sml_globs.dvalid[index]) {

                //AddLog(LOG_LEVEL_INFO, PSTR("not yet valid line %d"), index);
              //}
              // json export
              if (index == 0) {
                  //snprintf_P(b_mqtt_data, sizeof(b_mqtt_data), "%s,\"%s\":{\"%s\":%s", b_mqtt_data,sml_globs.mp[mindex].prefix,jname,tpowstr);
                  if (!nojson) {
                    ResponseAppend_P(PSTR(",\"%s\":{\"%s\":%s"), sml_globs.mp[mindex].prefix, jname, tpowstr);
                  }
              }
              else {
                if (lastmind != mindex) {
                  // meter changed, close mqtt
                  //snprintf_P(b_mqtt_data, sizeof(b_mqtt_data), "%s}", b_mqtt_data);
                  if (!nojson) {
                     ResponseAppend_P(PSTR("}"));
                   }
                    // and open new
                    //snprintf_P(b_mqtt_data, sizeof(b_mqtt_data), "%s,\"%s\":{\"%s\":%s", b_mqtt_data,sml_globs.mp[mindex].prefix,jname,tpowstr);
                  if (!nojson) {
                    ResponseAppend_P(PSTR(",\"%s\":{\"%s\":%s"), sml_globs.mp[mindex].prefix, jname, tpowstr);
                  }
                  lastmind = mindex;
                } else {
                  //snprintf_P(b_mqtt_data, sizeof(b_mqtt_data), "%s,\"%s\":%s", b_mqtt_data,jname,tpowstr);
                  if (!nojson) {
                    ResponseAppend_P(PSTR(",\"%s\":%s"), jname, tpowstr);
                  }
                }
              }
#ifdef USE_WEBSERVER
            } else {
              // web ui export
              //snprintf_P(b_mqtt_data, sizeof(b_mqtt_data), "%s{s}%s %s: {m}%s %s{e}", b_mqtt_data,meter_desc[mindex].prefix,name,tpowstr,unit);
             if (strcmp(name, "*"))  WSContentSend_PD(PSTR("{s}%s %s {m}%s %s{e}"), sml_globs.mp[mindex].prefix, name,tpowstr, unit);
#endif  // USE_WEBSERVER
            }
          }
        }
        if (index < sml_globs.maxvars - 1) {
          index++;
        }
        // next section
        mp = strchr(cp, '|');
        if (mp) mp++;
    }
    if (json) {
     //snprintf_P(b_mqtt_data, sizeof(b_mqtt_data), "%s}", b_mqtt_data);
     //ResponseAppend_P(PSTR("%s"),b_mqtt_data);
     if (!nojson) {
       ResponseAppend_P(PSTR("}"));
     }
#ifdef USE_WEBSERVER
   } else {
     //WSContentSend_PD(PSTR("%s"),b_mqtt_data);
#endif  // USE_WEBSERVER
   }


#ifdef USE_DOMOTICZ
  if (json && !TasmotaGlobal.tele_period) {
    char str[16];
    DOUBLE2CHAR(sml_globs.meter_vars[0], 1, str);
    DomoticzSensorPowerEnergy(sml_globs.meter_vars[1], str);  // PowerUsage, EnergyToday
    DOUBLE2CHAR(sml_globs.meter_vars[2], 1, str);
    DomoticzSensor(DZ_VOLTAGE, str);  // Voltage
    DOUBLE2CHAR(sml_globs.meter_vars[3], 1, str);
    DomoticzSensor(DZ_CURRENT, str);  // Current
  }
#endif  // USE_DOMOTICZ

}

struct SML_COUNTER {
  uint8_t sml_cnt_debounce;
  uint8_t sml_cnt_old_state;
  uint32_t sml_cnt_last_ts;
  uint32_t sml_counter_ltime;
  uint32_t sml_counter_lfalltime;
  uint32_t sml_counter_pulsewidth;
  uint16_t sml_debounce;
  uint8_t sml_cnt_updated;
} sml_counters[MAX_COUNTERS];

uint8_t sml_counter_pinstate;
uint8_t sml_cnt_index[MAX_COUNTERS] =  { 0, 1, 2, 3 };

void IRAM_ATTR SML_CounterIsr(void *arg) {
  uint32_t index = *static_cast<uint8_t*>(arg);

  uint32_t time = millis();
  uint32_t debounce_time;

  if (digitalRead(sml_globs.mp[sml_counters[index].sml_cnt_old_state].srcpin) == bitRead(sml_counter_pinstate, index)) {
    return;
  }

  debounce_time = time - sml_counters[index].sml_counter_ltime;

  if (debounce_time <= sml_counters[index].sml_debounce) return;

  if bitRead(sml_counter_pinstate, index) {
    // falling edge
    RtcSettings.pulse_counter[index]++;
    sml_counters[index].sml_counter_pulsewidth = time - sml_counters[index].sml_counter_lfalltime;
    sml_counters[index].sml_counter_lfalltime = time;
    sml_counters[index].sml_cnt_updated = 1;
  }
  sml_counters[index].sml_counter_ltime = time;
  sml_counter_pinstate ^= (1 << index);
}

#ifndef METER_DEF_SIZE
#define METER_DEF_SIZE 3000
#endif


#ifdef SML_REPLACE_VARS

#ifndef SML_SRCBSIZE
#define SML_SRCBSIZE 256
#endif

uint32_t SML_getlinelen(char *lp) {
uint32_t cnt;
  for (cnt = 0; cnt < SML_SRCBSIZE - 1; cnt++) {
    if (lp[cnt] == SCRIPT_EOL) {
      break;
    }
  }
  return cnt;
}

uint32_t SML_getscriptsize(char *lp) {
uint32_t mlen = 0;
char dstbuf[SML_SRCBSIZE * 2];
  while (1) {
    Replace_Cmd_Vars(lp, 1, dstbuf, sizeof(dstbuf));
    lp += SML_getlinelen(lp) + 1;
    uint32_t slen = strlen(dstbuf);
    //AddLog(LOG_LEVEL_INFO, PSTR("%d - %s"),slen,dstbuf);
    mlen += slen + 1;
    if (*lp == '#') break;
    if (*lp == '>') break;
    if (*lp == 0) break;
  }
  //AddLog(LOG_LEVEL_INFO, PSTR("len=%d"),mlen);
  return mlen + 32;
}
#else
uint32_t SML_getscriptsize(char *lp) {
  uint32_t mlen = 0;
  for (uint32_t cnt = 0; cnt < METER_DEF_SIZE - 1; cnt++) {
    if (lp[cnt] == '\n' && lp[cnt + 1] == '#') {
      mlen = cnt + 3;
      break;
    }
  }
  //AddLog(LOG_LEVEL_INFO, PSTR("len=%d"),mlen);
  return mlen;
}
#endif // SML_REPLACE_VARS

bool Gpio_used(uint8_t gpiopin) {
  if ((gpiopin < nitems(TasmotaGlobal.gpio_pin)) && (TasmotaGlobal.gpio_pin[gpiopin] > 0)) {
    return true;
  }
  return false;
}

#define SML_MINSB 64
char *SpecOptions(char *cp, uint32_t mnum) {
// special option
struct METER_DESC *mp = &meter_desc[mnum];
	switch (*cp) {
		case '1':
			cp++;
#ifdef USE_SML_SPECOPT
			if (*cp == ',') {
		    cp++;
		    mp->so_obis1 = strtol(cp, &cp, 16);
		  }
		  if (*cp == ',') {
		    cp++;
		    mp->so_fcode1 = strtol(cp, &cp, 16);
		  }
		  if (*cp == ',') {
		    cp++;
		    mp->so_bpos1 = strtol(cp, &cp, 10);
		  }
		  if (*cp == ',') {
		    cp++;
		    mp->so_fcode2 = strtol(cp, &cp, 16);
		  }
		  if (*cp == ',') {
		    cp++;
		    mp->so_bpos2 = strtol(cp, &cp, 10);
		  }
		  if (*cp == ',') {
		    cp++;
		    mp->so_obis2 = strtol(cp, &cp, 16);
		  }
#endif
			break;
 		case '2':
			cp += 2;
			mp->so_flags.data = strtol(cp, &cp, 16);
			break;
		case '3':
			cp += 2;
			mp->sbsiz = strtol(cp, &cp, 10);
			if (*cp == ',') {
				cp++;
				mp->sibsiz = strtol(cp, &cp, 10);
				if (mp->sibsiz < SML_MINSB) {
					mp->sibsiz = SML_MINSB;
				}
			}
			if (*cp == ',') {
				cp++;
				sml_globs.logsize = strtol(cp, &cp, 10);
			}
			break;
		case '4':
			cp += 2;
#ifdef USE_SML_DECRYPT
			meter_desc[mnum].use_crypt = true;
			for (uint8_t cnt = 0; cnt < (SML_CRYPT_SIZE * 2); cnt += 2) {
				mp->key[cnt / 2] = (sml_hexnibble(cp[cnt]) << 4) | sml_hexnibble(cp[cnt + 1]);
			}
			AddLog(LOG_LEVEL_INFO, PSTR("SML: crypto mode used for meter %d"), mnum + 1);
			break;
#ifdef USE_SML_AUTHKEY
		case '5':
			cp += 2;
			for (uint8_t cnt = 0; cnt < (SML_CRYPT_SIZE * 2); cnt += 2) {
				mp->auth[cnt / 2] = (sml_hexnibble(cp[cnt]) << 4) | sml_hexnibble(cp[cnt + 1]);
			}
			break;
#endif // USE_SML_AUTHKEY
#endif // USE_SML_DECRYPT
		case '6':
			cp += 2;
			mp->tout_ms = strtol(cp, &cp, 10);
      break;
  	case '7':
			cp += 2;
#ifdef ESP32     
			mp->uart_index = strtol(cp, &cp, 10);
#endif // ESP32
			break;
	}
	return cp;
}

#ifdef USE_SML_DECRYPT
uint16_t serial_dispatch(uint8_t meter, uint8_t sel) {
	struct METER_DESC *mp = &meter_desc[meter];
	if (!sel) {
		return mp->meter_ss->available();
	}
	uint8_t iob = mp->meter_ss->read();
	return iob;
}

int SML_print(const char *format, ...) {
	static char loc_buf[64];
	char* temp = loc_buf;
	int len;
	va_list arg;
	va_list copy;
	va_start(arg, format);
	va_copy(copy, arg);
	len = vsnprintf(NULL, 0, format, arg);
	va_end(copy);
	if (len >= sizeof(loc_buf)) {
		temp = (char*)malloc(len + 1);
		if (temp == NULL) {
	  	return 0;
	  }
	}
	vsnprintf(temp, len + 1, format, arg);
	AddLog(LOG_LEVEL_DEBUG, PSTR("SML: %s"),temp);
	va_end(arg);
	if (len >= sizeof(loc_buf)) {
		free(temp);
	}
	return len;
}
#endif // USE_SML_DECRYPT

void reset_sml_vars(uint16_t maxmeters) {

  for (uint32_t meters = 0; meters < maxmeters; meters++) {

		struct METER_DESC *mp = &meter_desc[meters];
    mp->spos = 0;
    mp->sbsiz = SML_BSIZ;
    mp->sibsiz = TMSBSIZ;
    if (mp->sbuff) {
      free(mp->sbuff);
      mp->sbuff = 0;
    }
#ifdef USE_SML_SPECOPT
    mp->so_obis1 = 0;
    mp->so_obis2 = 0;
#endif
    mp->so_flags.data = 0;
    // addresses a bug in meter DWS74
#ifdef DWS74_BUG
    mp->so_flags.SO_DWS74_BUG = 1;
#endif

#ifdef SML_OBIS_LINE
    mp->so_flags.SO_OBIS_LINE = 1;
#endif
    if (mp->txmem) {
      free(mp->txmem);
      mp->txmem = 0;
    }
    mp->txmem = 0;
    mp->trxpin = -1;
    if (mp->meter_ss) {
        delete mp->meter_ss;
        mp->meter_ss = NULL;
    }

		mp->lastms = millis();
		mp->tout_ms = SML_STIMEOUT;

#ifdef ESP32
    mp->uart_index = -1;
#endif

#ifdef USE_SML_DECRYPT
		if (mp->use_crypt) {
			if (mp->hp) {
				delete mp->hp;
				mp->hp = NULL;
			}
		}
		mp->use_crypt = 0;
#ifdef USE_SML_AUTHKEY
		memset(mp->auth, 0, SML_CRYPT_SIZE);
#endif
#endif // USE_SML_DECRYPT
  }
}


void SML_Init(void) {

  sml_globs.ready = false;

  if (!bitRead(Settings->rule_enabled, 0)) {
    return;
  }

	sml_globs.mp = meter_desc;

  uint8_t meter_script = Run_Scripter(">M", -2, 0);
  if (meter_script != 99) {
    AddLog(LOG_LEVEL_INFO, PSTR("no meter section found!"));
    return;
  }

  char *lp = glob_script_mem.section_ptr;
  uint8_t new_meters_used;

  // use script definition
  if (sml_globs.script_meter) {
    // restart condition
    free(sml_globs.script_meter);
    if (sml_globs.meter_vars) {
      free(sml_globs.meter_vars);
      sml_globs.meter_vars = 0;
    }
    if (sml_globs.dvalid) {
      free(sml_globs.dvalid);
      sml_globs.dvalid = 0;
    }
#ifdef USE_SML_MEDIAN_FILTER
    if (sml_globs.sml_mf) {
      free(sml_globs.sml_mf);
      sml_globs.sml_mf = 0;
    }
#endif
    reset_sml_vars(sml_globs.meters_used);
  }

  if (*lp == '>' && *(lp + 1) == 'M') {
    lp += 2;
    sml_globs.meters_used = strtol(lp, &lp, 10);
  } else {
    return;
  }

  sml_globs.maxvars = 0;

  reset_sml_vars(sml_globs.meters_used);

  sml_globs.sml_desc_cnt = 0;

  sml_globs.script_meter = 0;
  uint8_t *tp = 0;
  uint16_t index = 0;
  uint8_t section = 0;
  int8_t srcpin = 0;
  uint32_t mlen;
	uint16_t memory = 0;

#ifdef ESP32
  uint32_t uart_index = SOC_UART_NUM - 1;
#endif

  sml_globs.sml_send_blocks = 0;
  lp = glob_script_mem.section_ptr;
  struct METER_DESC *mmp;
  while (lp) {
      if (!section) {
        if (*lp == '>' && *(lp + 1) == 'M') {
          lp += 2;
          section = 1;
          mlen = SML_getscriptsize(lp);
          if (mlen == 0) return; // missing end #
          sml_globs.script_meter = (uint8_t*)calloc(mlen, 1);
					memory += mlen;
          if (!sml_globs.script_meter) {
            goto dddef_exit;
          }
          tp = sml_globs.script_meter;
          goto next_line;
        }
      }
      else {
        if (!*lp || *lp == '#' || *lp == '>') {
          if (*(tp - 1) == '|') *(tp - 1) = 0;
          break;
        }
        if (*lp == '+') {
          // add descriptor +1,1,c,0,10,H20
          //toLogEOL(">>",lp);
          lp++;
          index = *lp & 7;
          lp += 2;
          if (index < 1 || index > sml_globs.meters_used) {
            AddLog(LOG_LEVEL_INFO, PSTR("illegal meter number!"));
            goto next_line;
          }
          index--;
          mmp = &meter_desc[index];
          if (*lp == '[') {
            // sign TCP mode
            srcpin = TCP_MODE_FLG;
            lp++;
            char str[32];
            uint8_t cnt;
            for (cnt = 0; cnt < sizeof(str) - 1; cnt++) {
              if (!*lp || *lp == '\n' || *lp == ']') {
                break;
              }
              str[cnt] = *lp++;
            }
            str[cnt] = 0;
            lp++;
#ifdef USE_SML_TCP
#ifdef USE_SML_TCP_IP_STR
            strcpy(mmp->ip_addr, str);
#else
            mmp->ip_addr.fromString(str);
#endif
#endif
          } else {
            srcpin  = strtol(lp, &lp, 10);
            if (Gpio_used(abs(srcpin))) {
              AddLog(LOG_LEVEL_INFO, PSTR("SML: Error: Duplicate GPIO %d defined. Not usable for RX in meter number %d"), abs(srcpin), index + 1);
dddef_exit:
              if (sml_globs.script_meter) free(sml_globs.script_meter);
              sml_globs.script_meter = 0;
              return;
            }
          }
          mmp->srcpin = srcpin;
          if (*lp != ',') goto next_line;
          lp++;
          mmp->type = *lp;
          lp++;
          if (*lp != ',') {
            switch (*lp) {
              case 'N':
                lp++;
                mmp->sopt = 0x10 | (*lp & 3);
                lp++;
                break;
              case 'E':
                lp++;
                mmp->sopt = 0x20 | (*lp & 3);
                lp++;
                break;
              case 'O':
                lp++;
                mmp->sopt = 0x30 | (*lp & 3);
                lp++;
                break;
              default:
                mmp->sopt = *lp&7;
                lp++;
            }
          } else {
            mmp->sopt = 0;
          }
          lp++;
          mmp->flag = strtol(lp, &lp, 10);
          if (*lp != ',') goto next_line;
          lp++;
          mmp->params = strtol(lp, &lp, 10);
          if (*lp != ',') goto next_line;
          lp++;
          mmp->prefix[SML_PREFIX_SIZE - 1] = 0;
          for (uint32_t cnt = 0; cnt < SML_PREFIX_SIZE; cnt++) {
            if (*lp == SCRIPT_EOL || *lp == ',') {
              mmp->prefix[cnt] = 0;
              break;
            }
           mmp->prefix[cnt] = *lp++;
          }
          if (*lp == ',') {
            lp++;
            // get TRX pin
            mmp->trxpin = strtol(lp, &lp, 10);
            if (mmp->srcpin != TCP_MODE_FLG) {
              if (Gpio_used(mmp->trxpin)) {
                AddLog(LOG_LEVEL_INFO, PSTR("SML: Error: Duplicate GPIO %d defined. Not usable for TX in meter number %d"), meter_desc[index].trxpin, index + 1);
                goto dddef_exit;
              }
            }
            // optional transmit enable pin
            if (*lp == '(') {
              lp++;
              if (*lp == 'i') {
                lp++;
                mmp->trx_en.trxenpol = 1;
              } else {
                mmp->trx_en.trxenpol = 0;
              }
              mmp->trx_en.trxenpin = strtol(lp, &lp, 10);
              if (*lp != ')') {
                goto dddef_exit;
              }
              lp++;
              if (Gpio_used(mmp->trx_en.trxenpin)) {
                AddLog(LOG_LEVEL_INFO, PSTR("SML: Error: Duplicate GPIO %d defined. Not usable for TX enable in meter number %d"), meter_desc[index].trx_en.trxenpin, index + 1);
                goto dddef_exit;
              }
              mmp->trx_en.trxen = 1;
              pinMode(mmp->trx_en.trxenpin, OUTPUT);
              digitalWrite(mmp->trx_en.trxenpin, mmp->trx_en.trxenpol);
            } else {
              mmp->trx_en.trxen = 0;
            }
            if (*lp != ',') goto next_line;
            lp++;
            mmp->tsecs = strtol(lp, &lp, 10);
            if (*lp == ',') {
              lp++;
              // look ahead
              uint16_t txlen = 0;
              uint16_t tx_entries = 1;
              char *txp = lp;
              while (*txp) {
                if (*txp == ',') tx_entries++;
                if (*txp == SCRIPT_EOL) {
                  if (tx_entries > 1) {
                    if (*(txp - 1) != ',' ) {
                      break;
                    }
                    // line ends with ,
                  } else {
                    // single entry
                    break;
                  }
                }
                txp++;
                txlen++;
              }
              if (txlen) {
                mmp->txmem = (char*)calloc(txlen + 2, 1);
								memory += txlen + 2;
                if (mmp->txmem) {
                  // now copy send blocks
                  char *txp = lp;
                  uint16_t tind = 0;
                  for (uint32_t cnt = 0; cnt < txlen; cnt++) {
                      if (*txp == SCRIPT_EOL) {
                        txp++;
                      } else {
                        mmp->txmem[tind] = *txp++;
                        tind++;
                      }
                  }
                }
                //AddLog(LOG_LEVEL_INFO, PSTR(">>> %s - %d"), meter_desc[index].txmem, txlen);
                mmp->index = 0;
                mmp->max_index = tx_entries;
                sml_globs.sml_send_blocks++;
                lp += txlen;
              }
            }
          }
          if (*lp == SCRIPT_EOL) lp--;
          goto next_line;
        }
				char *lp1;
#ifdef SML_REPLACE_VARS
        char dstbuf[SML_SRCBSIZE*2];
        Replace_Cmd_Vars(lp, 1, dstbuf, sizeof(dstbuf));
        lp += SML_getlinelen(lp);
				lp1 = dstbuf;
#else
				lp1 = lp;
				lp += SML_getlinelen(lp);
#endif // SML_REPLACE_VARS

        //AddLog(LOG_LEVEL_INFO, PSTR("%s"),dstbuf);
        if (*lp1 == '-' || isdigit(*lp1)) {
          //toLogEOL(">>",lp);
          // add meters line -1,1-0:1.8.0*255(@10000,H2OIN,cbm,COUNTER,4|
          if (*lp1 == '-') lp1++;
          uint8_t mnum = strtol(lp1, 0, 10);
          if (mnum < 1 || mnum > sml_globs.meters_used) {
            AddLog(LOG_LEVEL_INFO, PSTR("illegal meter number!"));
            goto next_line;
          }
          // 1,=h—————————————
          if (!strncmp(lp1 + 1, ",=h", 3) || !strncmp(lp1 + 1, ",=so", 4)) {
            if (!strncmp(lp1 + 1, ",=so", 4)) {
							SpecOptions(lp1 + 5, mnum - 1);
            }
          } else {
            sml_globs.maxvars++;
          }

          while (1) {
            if (*lp1 == 0) {
              *tp++ = '|';
              goto next_line;
            }
            *tp++ = *lp1++;
            index++;
            if (index >= METER_DEF_SIZE) break;
          }
        }
      }

next_line:
      if (*lp == SCRIPT_EOL) {
        lp++;
      } else {
        lp = strchr(lp, SCRIPT_EOL);
        if (!lp) break;
        lp++;
      }
    }
    *tp = 0;
    sml_globs.meter_p = sml_globs.script_meter;

    // set serial buffers
  for (uint32_t meters = 0; meters < sml_globs.meters_used; meters++ ) {
    struct METER_DESC *mp = &meter_desc[meters];
    if (mp->sbsiz) {
      mp->sbuff = (uint8_t*)calloc(mp->sbsiz, 1);
			memory += mp->sbsiz;
    }
  }

  // initialize hardware
  typedef void (*function)();
  uint8_t cindex = 0;
  // preloud counters
  for (uint8_t i = 0; i < MAX_COUNTERS; i++) {
      RtcSettings.pulse_counter[i] = Settings->pulse_counter[i];
      sml_counters[i].sml_cnt_last_ts = millis();
  }

  sml_counter_pinstate = 0;
  for (uint8_t meters = 0; meters < sml_globs.meters_used; meters++) {
    METER_DESC *mp = &meter_desc[meters];
    if (mp->type == 'c') {
        if (mp->flag & 2) {

        } else {
          // counters, set to input with pullup
          if (mp->flag & 1) {
            pinMode(mp->srcpin, INPUT_PULLUP);
          } else {
            pinMode(mp->srcpin, INPUT);
          }
          // check for irq mode
          if (mp->params <= 0) {
            // init irq mode
            sml_counters[cindex].sml_cnt_old_state = meters;
            sml_counters[cindex].sml_debounce = -sml_globs.mp[meters].params;
            attachInterruptArg(mp->srcpin, SML_CounterIsr, &sml_cnt_index[cindex], CHANGE);
            if (digitalRead(mp->srcpin) > 0) {
              sml_counter_pinstate |= (1 << cindex);
            }
            sml_counters[cindex].sml_counter_ltime = millis();
          }

          RtcSettings.pulse_counter[cindex] = Settings->pulse_counter[cindex];
          InjektCounterValue(meters, RtcSettings.pulse_counter[cindex], 0.0);
          cindex++;
        }
    } else {
      // serial input, init
      if (mp->srcpin == TCP_MODE_FLG) {
#ifdef USE_SML_TCP
        sml_tcp_init(mp);
#endif
      } else {
        // serial mode
#ifdef ESP8266
#ifdef SPECIAL_SS
        char type = mp->type;
        if (type == 'm' || type == 'M' || type == 'k' || type == 'p' || type == 'R' || type == 'v') {
          mp->meter_ss = new TasmotaSerial(mp->srcpin, mp->trxpin, 1, 0, mp->sibsiz);
        } else {
          mp->meter_ss = new TasmotaSerial(mp->srcpin, mp->trxpin, 1, 1, mp->sibsiz);
        }
#else
        mp->meter_ss = new TasmotaSerial(mp->srcpin, mp->trxpin, 1, 0, mp->sibsiz);
#endif  // SPECIAL_SS
#endif // ESP8266

#ifdef ESP32
        // use hardware serial
        if (mp->uart_index >= 0) {
          uart_index = mp->uart_index;
        }
        AddLog(LOG_LEVEL_INFO, PSTR("SML: uart used: %d"),uart_index);
#ifdef USE_ESP32_SW_SERIAL
        mp->meter_ss = new SML_ESP32_SERIAL(uart_index);
        if (mp->srcpin >= 0) {
          if (uart_index == 0) { ClaimSerial(); }
          uart_index--;
          if (uart_index < 0) uart_index = 0;
        }
#else
        mp->meter_ss = new HardwareSerial(uart_index);
        if (uart_index == 0) { ClaimSerial(); }
        uart_index--;
        if (uart_index < 0) uart_index = 0;
        mp->meter_ss->setRxBufferSize(mp->sibsiz);
#endif // USE_ESP32_SW_SERIAL

#endif  // ESP32

        uint32_t smode = SERIAL_8N1;

        if (mp->sopt & 0xf0) {
          // new serial config
          switch (mp->sopt >> 4) {
            case 1:
              if ((mp->sopt & 1) == 1) smode = SERIAL_8N1;
              else smode = SERIAL_8N2;
              break;
            case 2:
              if ((mp->sopt & 1) == 1) smode = SERIAL_8E1;
              else smode = SERIAL_8E2;
              break;
            case 3:
              if ((mp->sopt & 1) == 1) smode = SERIAL_8O1;
              else smode = SERIAL_8O2;
              break;
          }
        } else {
          // deprecated serial config
          if (mp->sopt == 2) {
            smode = SERIAL_8N2;
          }
          if (mp->type=='M') {
            smode = SERIAL_8E1;
            if (mp->sopt == 2) {
              smode = SERIAL_8E2;
            }
          }
        }

#ifdef ESP8266
        if (mp->meter_ss->begin(mp->params)) {
          mp->meter_ss->flush();
        }
        if (mp->meter_ss->hardwareSerial()) {
          Serial.begin(mp->params, (SerialConfig)smode);
          ClaimSerial();
          if (mp->so_flags.SO_TRX_INVERT) {
            U0C0 = U0C0 | BIT(UCRXI) | BIT(UCTXI); // Inverse RX, TX
          }
        }
#endif  // ESP8266

#ifdef ESP32
        mp->meter_ss->begin(mp->params, smode, mp->srcpin, mp->trxpin, mp->so_flags.SO_TRX_INVERT);
#ifdef USE_ESP32_SW_SERIAL
				mp->meter_ss->setRxBufferSize(mp->sibsiz);
#endif
#endif  // ESP32
      }
    }
  }

  sml_globs.meter_vars = (double*)calloc(sml_globs.maxvars, sizeof(double));
  sml_globs.dvalid = (uint8_t*)calloc(sml_globs.maxvars, sizeof(uint8_t));

#ifdef USE_SML_MEDIAN_FILTER
  sml_globs.sml_mf = (struct SML_MEDIAN_FILTER*)calloc(sml_globs.maxvars, sizeof(struct SML_MEDIAN_FILTER));
#endif

  if (!sml_globs.maxvars || !sml_globs.meter_vars || !sml_globs.dvalid || !sml_globs.sml_mf) {
    AddLog(LOG_LEVEL_INFO, PSTR("sml memory error!"));
    return;
  }

  memory += sizeof(sml_globs) + sizeof(meter_desc) + sml_globs.maxvars * (sizeof(double) +  sizeof(uint8_t) + sizeof(struct SML_MEDIAN_FILTER));

  AddLog(LOG_LEVEL_INFO, PSTR("meters: %d , decode lines: %d, memory used: %d bytes"), sml_globs.meters_used, sml_globs.maxvars, memory);


// speed optimize shift flag
  for (uint32_t meters = 0; meters < sml_globs.meters_used; meters++ ) {
    struct METER_DESC *mp = &meter_desc[meters];
    char type = mp->type;

    if (!(mp->so_flags.SO_OBIS_LINE)) {
      mp->shift_mode = (type != 'e' && type != 'k' && type != 'm' && type != 'M' && type != 'p' && type != 'R' && type != 'v');
    } else {
      mp->shift_mode = (type != 'o' && type != 'e' && type != 'k' && type != 'm' && type != 'M' && type != 'p' && type != 'R' && type != 'v');
    }

#ifdef USE_SML_DECRYPT
		if (mp->use_crypt) {
#ifdef USE_SML_AUTHKEY
			mp->hp = new Han_Parser(serial_dispatch, meters, mp->key, mp->auth);
#else
			mp->hp = new Han_Parser(serial_dispatch, meters, mp->key, nullptr);
#endif
		}
#endif
  }

  sml_globs.ready = true;
}


#ifdef USE_SML_SCRIPT_CMD

uint32_t SML_SetBaud(uint32_t meter, uint32_t br) {
  if (sml_globs.ready == false) return 0;
  if (meter < 1 || meter > sml_globs.meters_used) return 0;
  meter--;
  if (!meter_desc[meter].meter_ss) return 0;
#ifdef ESP8266
  if (meter_desc[meter].meter_ss->begin(br)) {
    meter_desc[meter].meter_ss->flush();
  }
  if (meter_desc[meter].meter_ss->hardwareSerial()) {
    if (sml_globs.mp[meter].type=='M') {
      Serial.begin(br, SERIAL_8E1);
    }
  }
#endif  // ESP8266
#ifdef ESP32
  meter_desc[meter].meter_ss->flush();
  meter_desc[meter].meter_ss->updateBaudRate(br);
  /*
  if (sml_globs.mp[meter].type=='M') {
    meter_desc.meter_ss[meter]->begin(br,SERIAL_8E1,sml_globs.mp[meter].srcpin,sml_globs.mp[meter].trxpin);
  } else {
    meter_desc.meter_ss[meter]->begin(br,SERIAL_8N1,sml_globs.mp[meter].srcpin,sml_globs.mp[meter].trxpin);
  }*/
#endif  // ESP32
  return 1;
}

uint32_t sml_status(uint32_t meter) {
  if (sml_globs.ready == false) return 0;
  if (meter < 1 || meter > sml_globs.meters_used) return 0;
  meter--;
#if defined(ED300L) || defined(AS2020) || defined(DTZ541) || defined(USE_SML_SPECOPT)
  return sml_globs.sml_status[meter];
#else
  return 0;
#endif
}

uint32_t SML_Write(int32_t meter, char *hstr) {
  if (sml_globs.ready == false) return 0;
  int8_t flag = meter;
  meter = abs(meter);
  if (meter < 1 || meter > sml_globs.meters_used) return 0;
  meter--;
  if (!meter_desc[meter].meter_ss) return 0;
  if (flag > 0) {
    SML_Send_Seq(meter, hstr);
  } else {
    // 9600:8E1, only hardware serial
    uint32_t baud = strtol(hstr, &hstr, 10);
    hstr++;
    // currently only 8 bits and ignore stopbits
    hstr++;
    uint32_t smode;
    switch (*hstr) {
      case 'N':
        smode = SERIAL_8N1;
        break;
      case 'E':
        smode = SERIAL_8E1;
        break;
      case 'O':
        smode = SERIAL_8O1;
        break;
    }

#ifdef ESP8266
    Serial.begin(baud, (SerialConfig)smode);
#else
    meter_desc[meter].meter_ss->begin(baud, smode, sml_globs.mp[meter].srcpin, sml_globs.mp[meter].trxpin, sml_globs.mp[meter].so_flags.SO_TRX_INVERT);
#endif
  }
  return 1;
}

uint32_t SML_Read(int32_t meter, char *str, uint32_t slen) {
  if (sml_globs.ready == false) return 0;

  uint8_t hflg = 0;
  if (meter < 0) {
    meter = abs(meter);
    hflg = 1;
  }
  if (meter < 1 || meter > sml_globs.meters_used) return 0;
  meter--;
  if (!meter_desc[meter].meter_ss) return 0;

  struct METER_DESC *mp = &meter_desc[meter];

  if (!mp->spos) {
    return 0;
  }

  mp->sbuff[mp->spos] = 0;

  if (!hflg) {
    strlcpy(str, (char*)&mp->sbuff[0], slen);
  } else {
    uint32_t index = 0;
    for (uint32_t cnt = 0; cnt < mp->spos; cnt++) {
      sprintf(str,"%02x", mp->sbuff[cnt]);
      str += 2;
      index += 2;
      if (index >= slen - 2) break;
    }
  }
  mp->spos = 0;
  return 1;
}

uint32_t sml_getv(uint32_t sel) {
  if (sml_globs.ready == false) return 0;
  if (!sel) {
    for (uint8_t cnt = 0; cnt < sml_globs.maxvars; cnt++) {
      sml_globs.dvalid[cnt] = 0;
    }
    sel = 0;
  } else {
    if (sel < 1 || sel > sml_globs.maxvars) { sel = 1;}
    sel = sml_globs.dvalid[sel - 1];
  }
  return sel;
}

double SML_GetVal(uint32_t index) {
  if (sml_globs.ready == false) return 0;
  if (index < 1 || index > sml_globs.maxvars) { index = 1;}
  return sml_globs.meter_vars[index - 1];
}

char *SML_GetSVal(uint32_t index) {
  if (sml_globs.ready == false) return 0;
  if (index < 1 || index > sml_globs.meters_used) { index = 1;}
  return (char*)meter_desc[index - 1].meter_id;
}

int32_t SML_Set_WStr(uint32_t meter, char *hstr) {
  if (sml_globs.ready == false) return 0;
  if (meter < 1 || meter > sml_globs.meters_used) return -1;
  meter--;
  if (!meter_desc[meter].meter_ss) return -2;
  meter_desc[meter].script_str = hstr;
  return 0;
}

#endif // USE_SML_SCRIPT_CMD


void SetDBGLed(uint8_t srcpin, uint8_t ledpin) {
    pinMode(ledpin, OUTPUT);
    if (digitalRead(srcpin)) {
      digitalWrite(ledpin,LOW);
    } else {
      digitalWrite(ledpin,HIGH);
    }
}

// force channel math on counters
void SML_Counter_Poll_1s(void) {
	for (uint32_t meter = 0; meter < sml_globs.meters_used; meter++) {
		if (sml_globs.mp[meter].type == 'c') {
			SML_Decode(meter);
		}
	}
}


#ifndef CNT_PULSE_TIMEOUT
#define CNT_PULSE_TIMEOUT 5000
#endif

// fast counter polling
void SML_Counter_Poll(void) {
uint16_t meters, cindex = 0;
uint32_t ctime = millis();

  for (meters = 0; meters < sml_globs.meters_used; meters++) {
    if (sml_globs.mp[meters].type == 'c') {
      // poll for counters and debouce
      if (sml_globs.mp[meters].params > 0) {
        if (ctime - sml_counters[cindex].sml_cnt_last_ts > sml_globs.mp[meters].params) {
          sml_counters[cindex].sml_cnt_last_ts = ctime;

          if (sml_globs.mp[meters].flag & 2) {
            // analog mode, get next value
          } else {
            // poll digital input
            uint8_t state;
            sml_counters[cindex].sml_cnt_debounce <<= 1;
            sml_counters[cindex].sml_cnt_debounce |= (digitalRead(sml_globs.mp[meters].srcpin) & 1) | 0x80;
            if (sml_counters[cindex].sml_cnt_debounce == 0xc0) {
              // is 1
              state = 1;
            } else {
              // is 0, means switch down
              state = 0;
            }
            if (sml_counters[cindex].sml_cnt_old_state != state) {
              // state has changed
              sml_counters[cindex].sml_cnt_old_state = state;
              if (state == 0) {
                // inc counter
                RtcSettings.pulse_counter[cindex]++;
                sml_counters[cindex].sml_counter_pulsewidth = ctime - sml_counters[cindex].sml_counter_lfalltime;
                sml_counters[cindex].sml_counter_lfalltime = ctime;
                InjektCounterValue(meters, RtcSettings.pulse_counter[cindex], 60000.0 / (float)sml_counters[cindex].sml_counter_pulsewidth);
              }
            }
          }
        }
#ifdef DEBUG_CNT_LED1
        if (cindex == 0) SetDBGLed(sml_globs.mp[meters].srcpin, DEBUG_CNT_LED1);
#endif
#ifdef DEBUG_CNT_LED2
        if (cindex == 1) SetDBGLed(sml_globs.mp[meters].srcpin, DEBUG_CNT_LED2);
#endif
      } else {
        if (ctime - sml_counters[cindex].sml_cnt_last_ts > 10) {
          sml_counters[cindex].sml_cnt_last_ts = ctime;
#ifdef DEBUG_CNT_LED1
          if (cindex == 0) SetDBGLed(sml_globs.mp[meters].srcpin, DEBUG_CNT_LED1);
#endif
#ifdef DEBUG_CNT_LED2
          if (cindex == 1) SetDBGLed(sml_globs.mp[meters].srcpin, DEBUG_CNT_LED2);
#endif
        }

        if (sml_counters[cindex].sml_cnt_updated) {
          InjektCounterValue(meters, RtcSettings.pulse_counter[cindex], 60000.0 / (float)sml_counters[cindex].sml_counter_pulsewidth);
          sml_counters[cindex].sml_cnt_updated = 0;
        }
				// check timeout
				uint32_t time = millis();
				if ((time - sml_counters[cindex].sml_counter_lfalltime) > CNT_PULSE_TIMEOUT) {
					InjektCounterValue(meters, RtcSettings.pulse_counter[cindex], 0);
					sml_counters[cindex].sml_counter_lfalltime = time;
				}
      }
      cindex++;
    }
  }
}

#ifdef USE_SCRIPT
char *SML_Get_Sequence(char *cp,uint32_t index) {
  if (!index) return cp;
  uint32_t cindex = 0;
  while (cp) {
    cp = strchr(cp, ',');
    if (cp) {
      cp++;
      cindex++;
      if (cindex == index) {
        return cp;
      }
    }
  }
  return cp;
}

void SML_Check_Send(void) {
  sml_globs.sml_100ms_cnt++;
  char *cp;
  for (uint32_t cnt = sml_globs.sml_desc_cnt; cnt < sml_globs.meters_used; cnt++) {
    if (meter_desc[cnt].trxpin >= 0 && meter_desc[cnt].txmem) {
      //AddLog(LOG_LEVEL_INFO, PSTR("100 ms>> %d - %s - %d"),sml_globs.sml_desc_cnt,meter_desc[cnt].txmem,meter_desc[cnt].tsecs);
      if ((sml_globs.sml_100ms_cnt >= meter_desc[cnt].tsecs)) {
        sml_globs.sml_100ms_cnt = 0;
        // check for scriptsync extra output
        if (meter_desc[cnt].script_str) {
          cp = meter_desc[cnt].script_str;
          meter_desc[cnt].script_str = 0;
        } else {
          //AddLog(LOG_LEVEL_INFO, PSTR("100 ms>> 2"),cp);
          if (meter_desc[cnt].max_index > 1) {
            meter_desc[cnt].index++;
            if (meter_desc[cnt].index >= meter_desc[cnt].max_index) {
              meter_desc[cnt].index = 0;
              sml_globs.sml_desc_cnt++;
            }
            cp = SML_Get_Sequence(meter_desc[cnt].txmem, meter_desc[cnt].index);
            //SML_Send_Seq(cnt,cp);
          } else {
            cp = meter_desc[cnt].txmem;
            //SML_Send_Seq(cnt,cp);
            sml_globs.sml_desc_cnt++;
          }
        }
        //AddLog(LOG_LEVEL_INFO, PSTR(">> %s"),cp);
        SML_Send_Seq(cnt,cp);
        if (sml_globs.sml_desc_cnt >= sml_globs.meters_used) {
          sml_globs.sml_desc_cnt = 0;
        }
        break;
      }
    } else {
      sml_globs.sml_desc_cnt++;
    }

    if (sml_globs.sml_desc_cnt >= sml_globs.meters_used) {
      sml_globs.sml_desc_cnt = 0;
    }
  }
}

void sml_hex_asci(uint32_t mindex, char *tpowstr) {
  char *cp = meter_desc[mindex].meter_id;
  uint16_t slen = strlen(cp);
  slen &= 0xfffe;
  uint16_t cnt;
  *tpowstr++ = '"';
  for (cnt = 0; cnt < slen; cnt += 2) {
    uint8_t iob = (sml_hexnibble(cp[cnt]) << 4) | sml_hexnibble(cp[cnt + 1]);
    *tpowstr++ = iob;
  }
  *tpowstr++ = '"';
  *tpowstr = 0;
}


uint8_t sml_hexnibble(char chr) {
  uint8_t rVal = 0;
  if (isdigit(chr)) {
    rVal = chr - '0';
  } else  {
    if (chr >= 'A' && chr <= 'F') rVal = chr + 10 - 'A';
    if (chr >= 'a' && chr <= 'f') rVal = chr + 10 - 'a';
  }
  return rVal;
}

typedef struct {
  uint16_t T_ID;
  uint16_t P_ID;
  uint16_t SIZE;
  uint8_t U_ID;
  uint8_t payload[8];
 } MODBUS_TCP_HEADER;

uint16_t sml_swap(uint16_t in) {
  return (in << 8) | in >> 8;
}

// send modbus TCP frame with payload
// given ip addr  and port in baudrate
void sml_tcp_send(uint32_t meter, uint8_t *sbuff, uint16_t slen) {
MODBUS_TCP_HEADER tcph;

  //tcph.T_ID = sml_swap(0x1234);
  tcph.T_ID = random(0xffff);

  tcph.P_ID = 0;
  tcph.SIZE = sml_swap(6);
  tcph.U_ID = *sbuff;

  sbuff++;
  for (uint8_t cnt = 0; cnt < slen - 3; cnt++) {
    tcph.payload[cnt] = *sbuff++;
  }

#ifdef USE_SML_TCP
  // AddLog(LOG_LEVEL_INFO, PSTR("slen >> %d "),slen);
  if (meter_desc[meter].client) {
    if (meter_desc[meter].client->connected()) {
      meter_desc[meter].client->write((uint8_t*)&tcph, 7 + slen - 3);
    }
  }
#endif
}

#ifdef USE_SML_TCP
int32_t sml_tcp_init(struct METER_DESC *mp) {  
  if (!TasmotaGlobal.global_state.wifi_down) {
    if (!mp->client) {
      // tcp mode
#ifdef USE_SML_TCP_SECURE
      mp->client = new WiFiClientSecure;
      //client(new BearSSL::WiFiClientSecure_light(1024,1024)) {
      mp->client->setInsecure();
#else        
      mp->client = new WiFiClient;
#endif // USE_SML_TCP_SECURE
    }
    int32_t err = mp->client->connect(mp->ip_addr, mp->params);
    char ipa[32];
#ifdef USE_SML_TCP_IP_STR
    strcpy(ipa, mp->ip_addr);
#else
    strcpy(ipa, mp->ip_addr.toString().c_str());
#endif
    if (!err) {
      AddLog(LOG_LEVEL_INFO, PSTR("SML: could not connect TCP to %s:%d"),ipa, mp->params);
    } else {
      AddLog(LOG_LEVEL_INFO, PSTR("SML: connected TCP to %s:%d"),ipa, mp->params);
    }
  } else {
    AddLog(LOG_LEVEL_INFO, PSTR("SML: could not connect TCP since wifi is down"));
    mp->client = nullptr;
    return -1;
  }
  return 0;
}

#ifndef TCP_TIMEOUT
#define TCP_TIMEOUT 30
#endif

void sml_tcp_check(void) {
  sml_globs.to_cnt++;
  if (sml_globs.to_cnt > TCP_TIMEOUT) {
    sml_globs.to_cnt = 0;
    for (uint32_t meter = 0; meter < sml_globs.meters_used; meter++) {
      struct METER_DESC *mp = &sml_globs.mp[meter];
		  if (mp->srcpin == TCP_MODE_FLG) {
			  if (!mp->client) {
          sml_tcp_init(mp);
        } else {
          if (!mp->client->connected()) {
            sml_tcp_init(mp);
          }
        }
		  }
	  }
  }
}


#endif // USE_SML_TCP


// send sequence every N Seconds
void SML_Send_Seq(uint32_t meter, char *seq) {
  uint8_t sbuff[48];
  uint8_t *ucp = sbuff, slen = 0;
  char *cp = seq;
  uint8_t rflg = 0;
  if (*cp == 'r') {
    rflg = 1;
    cp++;
  }
  while (*cp) {
    if (!*cp || !*(cp+1)) break;
    if (*cp == ',') break;
    uint8_t iob = (sml_hexnibble(*cp) << 4) | sml_hexnibble(*(cp + 1));
    cp += 2;
    *ucp++ = iob;
    slen++;
    if (slen >= sizeof(sbuff)-6) break; // leave space for checksum
  }
  if (meter_desc[meter].type == 'm' || meter_desc[meter].type == 'M' || meter_desc[meter].type == 'k') {
    if (meter_desc[meter].type == 'k') {
      // kamstrup, append crc, cr
      *ucp++ = 0;
      *ucp++ = 0;
      slen += 2;
      uint16_t crc = KS_calculateCRC(sbuff, slen);
      ucp -= 2;
      *ucp++ = highByte(crc);
      *ucp++ = lowByte(crc);

      // now check for escapes
      uint8_t ksbuff[24];
      ucp = ksbuff;
      *ucp++ = 0x80;
      uint8_t klen = 1;
      for (uint16_t cnt = 0; cnt < slen; cnt++) {
        uint8_t iob = sbuff[cnt];
        if ((iob == 0x80) || (iob == 0x40) || (iob == 0x0d) || (iob == 0x06) || (iob == 0x1b)) {
          *ucp++ = 0x1b;
          *ucp++ = iob ^= 0xff;
          klen += 2;
        } else {
          *ucp++ = iob;
          klen++;
        }
      }
      *ucp++ = 0xd;
      slen = klen + 1;
      memcpy(sbuff, ksbuff, slen);
    } else {
      if (!rflg) {
        *ucp++ = 0;
        *ucp++ = 2;
        slen += 2;
      }
      // append crc
      uint16_t crc = MBUS_calculateCRC(sbuff, slen, 0xFFFF);
      *ucp++ = lowByte(crc);
      *ucp++ = highByte(crc);
      slen += 2;
    }

  }
  if (meter_desc[meter].type == 'o') {
    for (uint32_t cnt = 0; cnt < slen; cnt++) {
      sbuff[cnt] |= (CalcEvenParity(sbuff[cnt]) << 7);
    }
  }
  if (meter_desc[meter].type == 'p') {
    *ucp++ = 0xc0;
    *ucp++ = 0xa8;
    *ucp++ = 1;
    *ucp++ = 1;
    *ucp++ = 0;
    *ucp++ = SML_PzemCrc(sbuff, 6);
    slen += 6;
  }

  if (meter_desc[meter].srcpin == TCP_MODE_FLG) {
    sml_tcp_send(meter, sbuff, slen);
  } else {
    if (meter_desc[meter].trx_en.trxen) {
      digitalWrite(meter_desc[meter].trx_en.trxenpin, meter_desc[meter].trx_en.trxenpol ^ 1);
    }
    meter_desc[meter].meter_ss->flush();
    meter_desc[meter].meter_ss->write(sbuff, slen);
    if (meter_desc[meter].trx_en.trxen) {
      // must wait for all data sent
      meter_desc[meter].meter_ss->flush();
      digitalWrite(meter_desc[meter].trx_en.trxenpin, meter_desc[meter].trx_en.trxenpol);
    }
  }

  if (sml_globs.dump2log) {
#ifdef SML_DUMP_OUT_ALL
    Hexdump(sbuff, slen);
#else
    uint8_t type = sml_globs.mp[(sml_globs.dump2log&7) - 1].type;
    if (type == 'm' || type == 'M' || type == 'k') {
      Hexdump(sbuff, slen);
    }
#endif
  }

#ifdef MODBUS_DEBUG
  uint8_t type = meter_desc[meter].type;
  if (!sml_globs.dump2log && (type == 'm' || type == 'M' || type == 'k')) {
    AddLog(LOG_LEVEL_INFO, PSTR("transmit index >> %d"),sml_globs.mp[meter].index);
    Hexdump(sbuff, slen);
  }
#endif

}
#endif // USE_SCRIPT

uint16_t MBUS_calculateCRC(uint8_t *frame, uint8_t num, uint16_t start) {
  uint16_t crc, flag;
  //crc = 0xFFFF;
  crc = start;
  for (uint32_t i = 0; i < num; i++) {
    crc ^= frame[i];
    for (uint32_t j = 8; j; j--) {
      if ((crc & 0x0001) != 0) {        // If the LSB is set
        crc >>= 1;                      // Shift right and XOR 0xA001
        crc ^= 0xA001;
      } else {                          // Else LSB is not set
        crc >>= 1;                      // Just shift right
      }
    }
  }
  return crc;
}


uint16_t KS_calculateCRC(const uint8_t *frame, uint8_t num) {
  uint32_t crc = 0;
  for (uint32_t i = 0; i < num; i++) {
      uint8_t mask = 0x80;
      uint8_t iob = frame[i];
      while (mask) {
          crc <<= 1;
          if (iob & mask) {
              crc |= 1;
          }
          mask >>= 1;
          if (crc & 0x10000) {
              crc &= 0xffff;
              crc ^= 0x1021;
          }
      }
  }
  return crc;
}

uint8_t SML_PzemCrc(uint8_t *data, uint8_t len) {
  uint16_t crc = 0;
  for (uint32_t i = 0; i < len; i++) crc += *data++;
  return (uint8_t)(crc & 0xFF);
}

// for odd parity init with 1
uint8_t CalcEvenParity(uint8_t data) {
uint8_t parity=0;

  while(data) {
    parity^=(data &1);
    data>>=1;
  }
  return parity;
}



// dump to log shows serial data on console
// has to be off for normal use
// in console sensor53 d1, d2, d3 ... or d0 for normal use
// set counter => sensor53 c1 xxxx
// restart driver => sensor53 r
// meter number for monitoring serial activity => sensor53 m1, m2, m3 ... or m0 for all (default)
// LED-GPIO for monitoring serial activity => sensor53 l2, l13, l15 ... or l255 for turn off (default)

bool XSNS_53_cmd(void) {
  bool serviced = true;
  if (XdrvMailbox.data_len > 0) {
      char *cp = XdrvMailbox.data;
      if (*cp == 'd') {
        // set dump mode
				if (sml_globs.ready) {
        	cp++;
        	uint8_t index = atoi(cp);
        	if ((index & 7) > sml_globs.meters_used) index = 1;
        	if (index > 0 && sml_globs.mp[(index & 7) - 1].type == 'c') {
          	index = 0;
        	}
					if (sml_globs.log_data) {
						free(sml_globs.log_data);
						sml_globs.log_data = 0;
					}

					if (index > 0) {
						sml_globs.log_data = (char*)calloc(sml_globs.logsize, sizeof(char));
					}
        	sml_globs.dump2log = index;
        	ResponseTime_P(PSTR(",\"SML\":{\"CMD\":\"dump: %d\"}}"), sml_globs.dump2log);
				}
      } else if (*cp == 'c') {
        // set counter
          cp++;
          uint8_t index = *cp&7;
          if (index < 1 || index > MAX_COUNTERS) index = 1;
          cp++;
          while (*cp == ' ') cp++;
          if (isdigit(*cp)) {
            uint32_t cval = atoi(cp);
            while (isdigit(*cp)) cp++;
            RtcSettings.pulse_counter[index - 1] = cval;
            uint8_t cindex = 0;
            for (uint8_t meters = 0; meters < sml_globs.meters_used; meters++) {
              if (sml_globs.mp[meters].type == 'c') {
                InjektCounterValue(meters, RtcSettings.pulse_counter[cindex], 0.0);
                cindex++;
              }
            }
          }
          ResponseTime_P(PSTR(",\"SML\":{\"CMD\":\"counter%d: %d\"}}"), index, RtcSettings.pulse_counter[index - 1]);
      } else if (*cp == 'r') {
        // restart
        ResponseTime_P(PSTR(",\"SML\":{\"CMD\":\"restart\"}}"));
        SML_CounterSaveState();
        SML_Init();
      } else if (*cp == 'm') {
        // meter number for serial activity
        cp++;
        if (!isdigit(*cp)) {
          ResponseTime_P(PSTR(",\"SML\":{\"CMD\":\"sml_globs.ser_act_meter_num: %d\"}}"), sml_globs.ser_act_meter_num);
        } else {
          sml_globs.ser_act_meter_num = atoi(cp);
          ResponseTime_P(PSTR(",\"SML\":{\"CMD\":\"sml_globs.ser_act_meter_num: %d\"}}"), sml_globs.ser_act_meter_num);
        }
      } else if (*cp == 'l') {
        // serial activity LED-GPIO
        cp++;
        if (!isdigit(*cp)) {
          ResponseTime_P(PSTR(",\"SML\":{\"CMD\":\"sml_globs.ser_act_LED_pin: %d\"}}"), sml_globs.ser_act_LED_pin);
        } else {
          sml_globs.ser_act_LED_pin = atoi(cp);
          if (Gpio_used(sml_globs.ser_act_LED_pin)) {
            AddLog(LOG_LEVEL_INFO, PSTR("SML: Error: Duplicate GPIO %d defined. Not usable for LED."), sml_globs.ser_act_LED_pin);
            sml_globs.ser_act_LED_pin = 255;
          }
          if (sml_globs.ser_act_LED_pin != 255) {
            pinMode(sml_globs.ser_act_LED_pin, OUTPUT);
          }
          ResponseTime_P(PSTR(",\"SML\":{\"CMD\":\"sml_globs.ser_act_LED_pin: %d\"}}"), sml_globs.ser_act_LED_pin);
        }
      } else {
        serviced = false;
      }
  }
  return serviced;
}

void InjektCounterValue(uint8_t meter, uint32_t counter, float rate) {

  snprintf((char*)&meter_desc[meter].sbuff[0], meter_desc[meter].sbsiz, "1-0:1.8.0*255(%d)", counter);
  SML_Decode(meter);

	char freq[16];
	freq[0] = 0;
	if (rate) {
		DOUBLE2CHAR(rate, 4, freq);
	}
  snprintf((char*)&meter_desc[meter].sbuff[0], meter_desc[meter].sbsiz, "1-0:1.7.0*255(%s)", freq);
  SML_Decode(meter);
}

void SML_CounterSaveState(void) {
  for (byte i = 0; i < MAX_COUNTERS; i++) {
      Settings->pulse_counter[i] = RtcSettings.pulse_counter[i];
  }
}


/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns53(uint32_t function) {
  bool result = false;
    switch (function) {
      case FUNC_INIT:
        SML_Init();
        break;
      case FUNC_LOOP:
        if (bitRead(Settings->rule_enabled, 0)) {
          if (sml_globs.ready) {
            SML_Counter_Poll();
            if (sml_globs.dump2log) {
              dump2log();
            } else {
              SML_Poll();
            }
          }
        }
        break;
      case FUNC_EVERY_100_MSECOND:
        if (bitRead(Settings->rule_enabled, 0)) {
          if (sml_globs.ready) {
            SML_Check_Send();
          }
        }
        break;
			case FUNC_EVERY_SECOND:
					if (bitRead(Settings->rule_enabled, 0)) {
						if (sml_globs.ready) {
							SML_Counter_Poll_1s();
#ifdef USE_SML_TCP
              sml_tcp_check();
#endif
						}
					}
					break;
      case FUNC_JSON_APPEND:
        if (sml_globs.ready) {
          if (sml_options & SML_OPTIONS_JSON_ENABLE) {
            SML_Show(1);
          }
        }
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        if (sml_globs.ready) {
          SML_Show(0);
        }
        break;
#endif  // USE_WEBSERVER
      case FUNC_COMMAND_SENSOR:
        if (XSNS_53 == XdrvMailbox.index) {
          result = XSNS_53_cmd();
        }
        break;
      case FUNC_SAVE_BEFORE_RESTART:
      case FUNC_SAVE_AT_MIDNIGHT:
        if (sml_globs.ready) {
          SML_CounterSaveState();
        }
        break;
    }
  return result;
}

#endif  // USE_SML
