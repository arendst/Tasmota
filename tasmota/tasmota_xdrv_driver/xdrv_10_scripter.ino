/*
  xdrv_10_scripter.ino - script support for Tasmota

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


#ifdef USE_SCRIPT
#ifndef USE_RULES
/*********************************************************************************************\
for documentation see up to date docs in file SCRIPTER.md
uses about 17 k of flash

to do
optimize code for space

remarks

goal is fast execution time, minimal use of ram and intuitive syntax
therefore =>
case sensitive cmds and vars (lowercase uses time and code)
no math hierarchy  (costs ram and execution time, better group with brackets, anyhow better readable for beginners)
(will probably make math hierarchy an ifdefed option)
keywords if then else endif, or, and are better readable for beginners (others may use {})

// to doo

\*********************************************************************************************/

#define XDRV_10             10


#ifndef TS_FLOAT
#define TS_FLOAT float
#endif
// float = 4, double = 8 bytes


const uint8_t SCRIPT_VERS[2] = {5, 2};

#define SCRIPT_DEBUG 0

#ifndef MAXVARS
#define MAXVARS 50
#endif
#ifndef MAXSVARS
#define MAXSVARS 5
#endif
#define MAXNVARS MAXVARS-MAXSVARS

#ifdef USE_SML_M
#ifndef NO_USE_SML_SCRIPT_CMD
// allows several sml cmds from scripts, as well as access to sml registers
#undef USE_SML_SCRIPT_CMD
#define USE_SML_SCRIPT_CMD
#endif
#endif // USE_SML_M

#ifndef MAXFILT
#define MAXFILT 5
#endif
#define SCRIPT_SVARSIZE 20
#ifndef SCRIPT_MAXSSIZE
#define SCRIPT_MAXSSIZE 48
#endif

#ifndef SCRIPT_CMDMEM
#define SCRIPT_CMDMEM 512
#endif
#define MAX_SCRIPT_CMDBUFFER 4096

#define SPI_FLASH_2SEC_SIZE SPI_FLASH_SEC_SIZE*2

#define SCRIPT_EOL '\n'
#define SCRIPT_FLOAT_PRECISION 2
#define PMEM_SIZE sizeof(Settings->script_pram)
#define SCRIPT_MAXPERM (PMEM_SIZE)-4/sizeof(TS_FLOAT)
#define MAX_SCRIPT_SIZE MAX_RULE_SIZE*MAX_RULE_SETS

#ifndef MAX_SARRAY_NUM
#define MAX_SARRAY_NUM 32
#endif

int32_t fast_mux(uint32_t flag, uint32_t time, TS_FLOAT *buf, uint32_t len);
void Draw_jpeg(uint8_t *mem, uint16_t jpgsize, uint16_t xp, uint16_t yp, uint8_t scale);
uint32_t EncodeLightId(uint8_t relay_id);
uint32_t DecodeLightId(uint32_t hue_id);
char *web_send_line(char mc, char *lp);
int32_t web_send_file(char mc, char *file);
char *Get_esc_char(char *cp, char *esc_chr);
#define SPECIAL_EEPMODE_SIZE 6200

#ifndef STASK_STACK
#define STASK_STACK 8192-2048
#endif

#ifdef USE_UFILESYS

#undef USE_SCRIPT_FATFS
#define USE_SCRIPT_FATFS -1

#else // USE_UFILESYS

// eeprom script
#ifdef EEP_SCRIPT_SIZE

#ifdef ESP32
#error "unsupported option for ESP32"
#endif

#ifdef USE_24C256
#pragma message "script 24c256 file option used"
#else

#if EEP_SCRIPT_SIZE==SPECIAL_EEPMODE_SIZE || EEP_SCRIPT_SIZE==SPI_FLASH_2SEC_SIZE || EEP_SCRIPT_SIZE==SPI_FLASH_SEC_SIZE
#if EEP_SCRIPT_SIZE==SPI_FLASH_2SEC_SIZE || EEP_SCRIPT_SIZE==SPI_FLASH_SEC_SIZE
#pragma message "internal special flash script buffer used"
#else
#pragma message "internal compressed eeprom script buffer used"
#endif
#else
#error "unsupported eeprom option used"
#endif
#endif // USE_24C256

#else // EEP_SCRIPT_SIZE

// default
#pragma message "script compression option used"

#endif // EEP_SCRIPT_SIZE

#endif // USE_UFILESYS

#include <unishox.h>
#define SCRIPT_COMPRESS compressor.unishox_compress
#define SCRIPT_DECOMPRESS compressor.unishox_decompress
#ifndef UNISHOXRSIZE
#define UNISHOXRSIZE 2560
#endif

#ifndef MAX_EXT_ARRAYS
#define MAX_EXT_ARRAYS 5
#endif

#ifndef STASK_PRIO
#define STASK_PRIO 1
#endif

#ifdef ESP32
#include <driver/i2s.h>
#endif

#ifdef USE_SCRIPT_TIMER
#include <Ticker.h>
Ticker Script_ticker1;
Ticker Script_ticker2;
Ticker Script_ticker3;
Ticker Script_ticker4;

void Script_ticker1_end(void) {
  Script_ticker1.detach();
  Run_Scripter1(">ti1", 4, 0);
}
void Script_ticker2_end(void) {
  Script_ticker2.detach();
  Run_Scripter1(">ti2", 4, 0);
}
void Script_ticker3_end(void) {
  Script_ticker3.detach();
  Run_Scripter1(">ti3", 4, 0);
}
void Script_ticker4_end(void) {
  Script_ticker4.detach();
  Run_Scripter1(">ti4", 4, 0);
}
#endif

#ifndef HARDWARE_FALLBACK
#define HARDWARE_FALLBACK          2
#endif

// EEPROM MACROS
// i2c eeprom
#define EEP_WRITE(A,B,C) eeprom_writeBytes(A, B, (uint8_t*)C);
#define EEP_READ(A,B,C) eeprom_readBytes(A, B, (uint8_t*)C);

#ifdef ESP8266
#ifdef USE_24C256
#undef EEP_INIT
#define EEP_INIT(A) eeprom_init(A)
#else
#undef EEP_INIT
#define EEP_INIT(A) alt_eeprom_init(A)
#endif
#endif

#if defined(USE_SML_M) && defined (USE_SML_SCRIPT_CMD)
extern uint8_t sml_options;
#endif

#if defined(EEP_SCRIPT_SIZE) && !defined(ESP32)

// seems to be the last untouched sector, beside OTA and serial Flash
#define SPEC_SCRIPT_FLASH 0x000F2000

uint32_t eeprom_block;

// these support only one 4 k block below EEPROM (eeprom @0x402FB000) this steals 4k of application area
uint32_t alt_eeprom_init(uint32_t size) {
    //EEPROM.begin(size);
    //eeprom_block = (uint32_t)&_EEPROM_start - 0x40200000 - SPI_FLASH_SEC_SIZE;
#if EEP_SCRIPT_SIZE==SPI_FLASH_2SEC_SIZE
    eeprom_block = SPEC_SCRIPT_FLASH - SPI_FLASH_SEC_SIZE;
    //eeprom_block = SPEC_SCRIPT_FLASH;
#else
    eeprom_block = SPEC_SCRIPT_FLASH;
#endif
    return 1;
}

void alt_eeprom_writeBytes(uint32_t adr, uint32_t len, uint8_t *buf) {
  uint32_t *lwp = (uint32_t*)buf;
#if EEP_SCRIPT_SIZE==SPI_FLASH_2SEC_SIZE
  ESP.flashEraseSector(eeprom_block  / SPI_FLASH_SEC_SIZE);
  ESP.flashEraseSector((eeprom_block + SPI_FLASH_SEC_SIZE)  / SPI_FLASH_SEC_SIZE);
#else
  ESP.flashEraseSector(eeprom_block / SPI_FLASH_SEC_SIZE);
#endif
  ESP.flashWrite(eeprom_block , lwp, len);
}

void alt_eeprom_readBytes(uint32_t adr, uint32_t len, uint8_t *buf) {
  uint32_t *lwp = (uint32_t*)buf;
  ESP.flashRead(eeprom_block, lwp, len);
}
#endif // EEP_SCRIPT_SIZE

#include "FS.h"

#if USE_SCRIPT_FATFS==-1
#ifdef ESP32
//#include "FS.h"
//#include "FFat.h"
#else
//#include <LittleFS.h>
#endif


#endif // LITTLEFS_SCRIPT_SIZE

#include <uri/UriGlob.h>

#include <TasmotaSerial.h>

#ifdef TESLA_POWERWALL
#include "include/powerwall.h"
#endif

#ifdef USE_DISPLAY_DUMP
#include <renderer.h>
extern Renderer *renderer;
#endif

// offsets epoch readings by 1.1.2019 00:00:00 to fit into float with second resolution
#ifndef EPOCH_OFFSET
#define EPOCH_OFFSET 1546300800
#endif

enum {OPER_EQU=1,OPER_PLS,OPER_MIN,OPER_MUL,OPER_DIV,OPER_PLSEQU,OPER_MINEQU,OPER_MULEQU,OPER_DIVEQU,OPER_EQUEQU,OPER_NOTEQU,OPER_GRTEQU,OPER_LOWEQU,OPER_GRT,OPER_LOW,OPER_PERC,OPER_XOR,OPER_AND,OPER_OR,OPER_ANDEQU,OPER_OREQU,OPER_XOREQU,OPER_PERCEQU,OPER_SHLEQU,OPER_SHREQU,OPER_SHL,OPER_SHR};
enum {SCRIPT_LOGLEVEL=1,SCRIPT_TELEPERIOD,SCRIPT_EVENT_HANDLED,SML_JSON_ENABLE,SCRIPT_EPOFFS,SCRIPT_CBSIZE};


#ifdef USE_UFILESYS
extern uint8_t ufs_type;
extern FS *ufsp;

#ifndef UFSYS_SIZE
#define UFSYS_SIZE 8192
#endif

#define FAT_SCRIPT_NAME "/script.txt"

#endif // USE_UFILESYS

extern "C" int32_t homekit_main(char *, uint32_t);

#ifdef SUPPORT_MQTT_EVENT
  #include <LinkedList.h>                 // Import LinkedList library
  typedef struct {
    String Event;
    String Topic;
    String Key;
  } MQTT_Subscription;
  LinkedList<MQTT_Subscription> subscriptions;
#endif    //SUPPORT_MQTT_EVENT

#ifdef USE_DISPLAY
#ifdef USE_TOUCH_BUTTONS
#include <renderer.h>
#ifndef MAX_TOUCH_BUTTONS
#define MAX_TOUCH_BUTTONS 16
#endif
extern VButton *buttons[MAX_TOUCH_BUTTONS];
#endif
#endif

typedef union {
  uint16_t data;
  struct {
    uint8_t is_string : 1;  // string or number
    uint8_t is_permanent : 1;
    uint8_t is_timer : 1;
    uint8_t is_autoinc : 1;
    uint8_t changed : 1;
    uint8_t settable : 1;
    uint8_t is_filter : 1;
    uint8_t constant : 1;
    uint8_t global : 1;
    uint8_t hchanged : 1;
    uint8_t integer : 1;
  };
} SCRIPT_TYPE;

struct T_INDEX {
  uint8_t index;
  SCRIPT_TYPE bits;
};

struct M_FILT {
#ifdef LARGE_ARRAYS
  uint16_t numvals;
  uint16_t index;
#else
  uint8_t numvals;
  uint8_t index;
#endif // LARGE_ARRAYS
  TS_FLOAT maccu;
  TS_FLOAT rbuff[1];
};


#ifdef LARGE_ARRAYS
#undef AND_FILT_MASK
#undef OR_FILT_MASK
#define AND_FILT_MASK 0x7fff
#define OR_FILT_MASK 0x8000
#undef MAX_ARRAY_SIZE
#define MAX_ARRAY_SIZE 1000
#else
#undef AND_FILT_MASK
#undef OR_FILT_MASK
#define AND_FILT_MASK 0x7f
#define OR_FILT_MASK 0x80
#undef MAX_ARRAY_SIZE
#define MAX_ARRAY_SIZE 127
#endif


typedef union {
  uint8_t data;
  struct {
      uint8_t nutu8 : 1;
      uint8_t nutu7 : 1;
      uint8_t nutu6 : 1;
      uint8_t nutu5 : 1;
      uint8_t nutu4 : 1;
      uint8_t nutu3 : 1;
      uint8_t is_dir : 1;
      uint8_t is_open : 1;
  };
} FILE_FLAGS;

typedef union {
  uint8_t data;
  struct {
      uint8_t nutu8 : 1;
      uint8_t nutu7 : 1;
      uint8_t nutu6 : 1;
      uint8_t nutu5 : 1;
      uint8_t nutu4 : 1;
      uint8_t nutu3 : 1;
      uint8_t udp_connected : 1;
      uint8_t udp_used : 1;
  };
} UDP_FLAGS;

typedef union {
  uint8_t data;
  struct {
      uint8_t nutu8 : 1;
      uint8_t nutu7 : 1;
      uint8_t nutu6 : 1;
      uint8_t nutu5 : 1;
      uint8_t nutu4 : 1;
      uint8_t nutu3 : 1;
      bool fsys : 1;
      bool eeprom : 1;
  };
} FS_FLAGS;


struct GVARS {
  JsonParserObject *jo;
  int16_t numind;
  int16_t strind;
};

#ifdef USE_SCRIPT_SPI
struct SCRIPT_SPI {
  int8_t sclk;
  int8_t mosi;
  int8_t miso;
  int8_t cs[4];
  SPIClass *spip;
  SPISettings settings;
};
#endif


#define NUM_RES 0xfe
#define STR_RES 0xfd
#define VAR_NV 0xff

#define NTYPE 0
#define STYPE 0x80

#ifndef FLT_MAX
#define FLT_MAX 99999999
#endif

#ifdef USE_SCRIPT_ONEWIRE
#include <OneWire.h>
#include <DS2480B.h>

#ifndef MAX_DS_SENSORS
#define MAX_DS_SENSORS 20
#endif

typedef struct {
  OneWire *ds;
  DS2480B *dsh;
  TasmotaSerial *ts;
  uint8_t ds_address[MAX_DS_SENSORS][8];
} ScriptOneWire;
#endif // USE_SCRIPT_ONEWIRE

#define SFS_MAX 4
// global memory
struct SCRIPT_MEM {
    TS_FLOAT *fvars; // number var pointer
    TS_FLOAT *s_fvars; // shadow var pointer
    struct T_INDEX *type; // type and index pointer
    struct M_FILT *mfilt;
    char *glob_vnp; // var name pointer
#ifdef SCRIPT_LARGE_VNBUFF
    uint16_t *vnp_offset;
#else
    uint8_t *vnp_offset;
#endif
    char *glob_snp; // string vars pointer
    char *scriptptr;
    char *section_ptr;
    char *scriptptr_bu;
    char *script_ram;
    uint16_t script_size;
    uint8_t *script_pram;
    uint16_t script_pram_size;
    uint8_t numvars;
    uint8_t arres;
    void *script_mem;
    uint16_t script_mem_size;
    uint8_t script_dprec;
    char script_sepc;
    uint8_t script_lzero;
    uint8_t var_not_found;
    uint8_t glob_error;
    uint8_t max_ssize;
    uint8_t script_loglevel;
    FS_FLAGS FLAGS;
    uint8_t si_num[3];
    uint8_t siro_num[3];
    uint8_t sind_num;
    char *last_index_string[3];
    uint16_t cmdbuffer_size = SCRIPT_CMDMEM / 2;

#ifdef USE_SCRIPT_FATFS
    File files[SFS_MAX];
    FILE_FLAGS file_flags[SFS_MAX];
    uint8_t script_sd_found;
    char flink[2][14];
#endif //USE_SCRIPT_FATFS
#ifdef USE_SCRIPT_GLOBVARS
    UDP_FLAGS udp_flags;
    IPAddress last_udp_ip;
    WiFiUDP Script_PortUdp;
    IPAddress script_udp_remote_ip;
#endif // USE_SCRIPT_GLOBVARS
    char web_mode;
    char *glob_script = 0;
    char *fast_script = 0;
    char *event_script = 0;
    char *html_script = 0;
    char *teleperiod = 0;
    char *web_pages[10];
    uint32_t script_lastmillis;
    bool event_handeled = false;
    bool res_ivar = false;
#ifdef USE_BUTTON_EVENT
    int8_t script_button[MAX_KEYS];
#endif //USE_BUTTON_EVENT

#ifdef USE_HOMEKIT
    bool homekit_running = false;
#endif // USE_HOMEKIT
    uint32_t epoch_offset = EPOCH_OFFSET;
#ifdef USE_SCRIPT_SERIAL
    TasmotaSerial *sp;
#endif
    TS_FLOAT retval;
    char *retstr;
#ifdef USE_SCRIPT_SPI
    struct SCRIPT_SPI spi;
#endif
#ifdef USE_FEXTRACT
    uint32_t from_time;
    uint32_t to_time;
#endif
#if defined(USE_SML_M) && defined(USE_SML_SCRIPT_CMD) && defined(USE_SCRIPT_SERIAL)
    char *hstr;
#endif

#ifdef USE_SCRIPT_I2C
    uint8_t script_i2c_addr;
    TwoWire *script_i2c_wire;
#endif

#ifdef USE_SCRIPT_ONEWIRE
    ScriptOneWire ow;
#endif

} glob_script_mem;


uint8_t tasm_cmd_activ = 0;
void flt2char(TS_FLOAT num, char *nbuff);

void flt2char(TS_FLOAT num, char *nbuff) {
  dtostrfd(num, glob_script_mem.script_dprec, nbuff);
}

void f2char(double num, uint32_t dprec, uint32_t lzeros, char *nbuff, char dsep);

// convert float to char with leading zeros
void f2char(double num, uint32_t dprec, uint32_t lzeros, char *nbuff, char dsep) {
  dtostrfd(num, dprec, nbuff);
  if (lzeros > 1) {
    // check leading zeros
    uint32_t nd = strlen(nbuff);
    for (uint8_t cnt = 0; cnt < nd; cnt++) {
      if (nbuff[cnt] == '.') {
        nd = cnt;
        break;
      }
    }
    if (lzeros > nd) {
      // insert zeros
      char cpbuf[24];
      char *cp = cpbuf;
      for (uint32_t cnt = 0; cnt < lzeros - nd; cnt++) {
        *cp++='0';
      }
      *cp=0;
      strcat(cpbuf,nbuff);
      strcpy(nbuff,cpbuf);
    }
  }
  if (dsep != '.') {
    for (uint16_t cnt = 0; cnt < strlen(nbuff); cnt++) {
      if (nbuff[cnt] == '.') {
        nbuff[cnt] = dsep;
      }
    }
  }
}

uint32_t match_vars(char *dvnam, TS_FLOAT **fp, char **sp, uint32_t *ind);
uint32_t script_sspi_trans(int32_t cs_index, TS_FLOAT *array, uint32_t len, uint32_t size);
char *scripter_sub(char *lp, uint8_t fromscriptcmd);
char *GetNumericArgument(char *lp,uint8_t lastop,TS_FLOAT *fp, struct GVARS *gv);
char *GetStringArgument(char *lp,uint8_t lastop,char *cp, struct GVARS *gv);
char *ForceStringVar(char *lp,char *dstr);
void send_download(void);
uint8_t UfsReject(char *name);
#ifdef USE_UFILESYS
void fread_str_fp(File *fp, char *sp, uint16_t slen, uint16_t flg);
int32_t script_copy_file(File *source, File *dest, uint32_t sf_from, uint32_t sf_to, uint32_t flag, WiFiClient *client);
int32_t opt_fext(File *fp,  char *ts_from, char *ts_to, uint32_t flg);
int32_t extract_from_file(File *fp,  char *ts_from, char *ts_to, int8_t coffs, TS_FLOAT **a_ptr, uint16_t *a_len, uint8_t numa, int16_t accum);
#endif
char *eval_sub(char *lp, TS_FLOAT *fvar, char *rstr);
uint32_t script_ow(uint8_t sel, uint32_t val);
int32_t script_logfile_write(char *path, char *payload, uint32_t size);
void script_sort_array(TS_FLOAT *array, uint16_t size);

void ScriptEverySecond(void) {

  if (bitRead(Settings->rule_enabled, 0)) {
    struct T_INDEX *vtp = glob_script_mem.type;
    TS_FLOAT delta = (millis() - glob_script_mem.script_lastmillis) / 1000.0;
    glob_script_mem.script_lastmillis = millis();
    for (uint8_t count=0; count<glob_script_mem.numvars; count++) {
      if (vtp[count].bits.is_timer) {
        // decrements timers
        TS_FLOAT *fp = &glob_script_mem.fvars[vtp[count].index];
        if (*fp>0) {
          // decrement
          *fp -= delta;
          if (*fp<0) *fp = 0;
        }
      }
      if (vtp[count].bits.is_autoinc) {
        // increments timers
        TS_FLOAT *fp = &glob_script_mem.fvars[vtp[count].index];
        if (*fp>=0) {
          *fp += delta;
        }
      }
    }
    Run_Scripter1(">S", 2, 0);

#ifdef USE_HOMEKIT
    if (glob_script_mem.homekit_running == false) {
      uint8_t homekit_found = Run_Scripter1(">h", -2, 0);
      if (homekit_found == 99) {
        if (!TasmotaGlobal.global_state.wifi_down) {
          homekit_main(glob_script_mem.section_ptr, 0);
          glob_script_mem.homekit_running = true;
        }
      }
    }
#endif // USE_HOMEKIT

  }
}

void SetChanged(uint32_t index) {
  glob_script_mem.type[index].bits.changed = 1;
#ifdef USE_SCRIPT_GLOBVARS
#ifdef USE_HOMEKIT
  glob_script_mem.type[index].bits.hchanged = 1;
#endif
#endif
//AddLog(LOG_LEVEL_INFO, PSTR("Change: %d"), index);
}


#define SCRIPT_SKIP_SPACES while (*lp==' ' || *lp=='\t') lp++;
#define SCRIPT_SKIP_EOL while (*lp == SCRIPT_EOL) lp++;

TS_FLOAT *Get_MFAddr(uint8_t index, uint16_t *len, uint16_t *ipos);

uint32_t Script_Find_Vars(char *sp) {
  uint16_t numvars = 0;
  uint16_t svars = 0;
  while (*sp) {
    if (*sp == '\n' || *sp == '\r') {
      sp++;
      while (*sp == '=') {
        sp++;
      }
      if (*sp == '#' || *sp == '>') {
        break;
      }
      char *cp = strchr(sp, '=');
      if (cp) {
        cp++;
        while (*cp == ' ') {
          cp++;
        }
        if (*cp == '"') {
          svars += 1;
        } else if (isdigit(*cp) || *cp == '-') {
          numvars += 1;
        }
        sp = cp;
      }
    }
    sp++;
  }
  return (svars << 16) | numvars;
}


// allocates all variables and presets them
int16_t Init_Scripter(void) {
char *script;

    int16_t err = 0;
    script = glob_script_mem.script_ram;
    if (!*script) return -999;

    uint32_t xvars = Script_Find_Vars(script + 1);
    uint16_t maxnvars = xvars & 0xffff;
    if (maxnvars < 1) {
      maxnvars = 1;
    }
    uint16_t maxsvars = xvars >> 16;
    if (maxsvars < 1) {
      maxsvars = 1;
    }
    uint16_t maxvars = maxsvars + maxnvars;
    //AddLog(LOG_LEVEL_INFO, PSTR("SCR: svar = %d, nvars = %d"), maxsvars, maxnvars);

    // scan lines for >DEF
    uint16_t lines = 0;
    uint16_t nvars = 0;
    uint16_t svars = 0;
    uint16_t vars = 0;
    char *lp = script;
    uint16_t imemsize = (maxvars * 10) + 4;
    uint8_t *imemptr = (uint8_t*)calloc(imemsize, 1);
    if (!imemptr) {
      return -7;
    }

    char *vnames = (char*)imemptr;

    char *vnp[maxvars];
    TS_FLOAT fvalues[maxvars];
    struct T_INDEX vtypes[maxvars];

    //char strings[MAXSVARS*SCRIPT_MAXSSIZE];
    //char *strings_p = strings;
    char *strings_op = (char*)calloc(maxsvars * SCRIPT_MAXSSIZE, 1);
    char *strings_p = strings_op;
    if (!strings_op) {
      free(imemptr);
      return -8;
    }

/*
    uint32_t imemp = (uint32_t)imemptr;
    imemp += (MAXVARS*10);
    imemp = (imemp & 0xfffc) + 4;
    Serial.printf(">1 %x\n",imemp);
    char *vnp[MAXVARS];

    //char **vnp = (char**)imemp;
    imemp += (sizeof(char*)*MAXVARS);
    imemp = (imemp & 0xfffc) + 4;
    Serial.printf(">2 %x\n",imemp);

    TS_FLOAT fvalues[MAXVARS];
    //TS_FLOAT *fvalues = (TS_FLOAT*)imemp;
    imemp += (sizeof(TS_FLOAT*)*MAXVARS);
    imemp = (imemp & 0xfffc) + 4;
    Serial.printf(">3 %x\n",imemp);

    struct T_INDEX vtypes[MAXVARS];
    //struct T_INDEX *vtypes = (struct T_INDEX*)imemp;
*/

    char *vnames_p = vnames;
    char **vnp_p = vnp;

    char *snp[maxsvars];

    struct M_FILT mfilt[MAXFILT];

    char **snp_p = snp;
    uint8_t numperm = 0;
    uint8_t numflt = 0;
    uint16_t count;

    glob_script_mem.max_ssize = SCRIPT_SVARSIZE;
    glob_script_mem.scriptptr = 0;

    char init = 0;
    while (1) {
        // check line
        // skip leading spaces
        SCRIPT_SKIP_SPACES
        // skip empty line
        if (*lp == '\n' || *lp == '\r') goto next_line;
        // skip comment
        if (*lp == ';') goto next_line;
        if (init) {
            // init section
            if (*lp == '>' || !*lp) {
                init = 0;
                break;
            }
            char *op = strchr(lp, '=');
            if (op) {
                vtypes[vars].bits.data = 0;
                // found variable definition
                if (*lp == 'p' && *(lp + 1) == ':') {
                    lp += 2;
                    if (numperm < SCRIPT_MAXPERM) {
                      vtypes[vars].bits.is_permanent = 1;
                      numperm++;
                    }
                } else {
                    vtypes[vars].bits.is_permanent = 0;
                }
                if (*lp == 't' && *(lp + 1) == ':') {
                    lp += 2;
                    vtypes[vars].bits.is_timer = 1;
                } else {
                    vtypes[vars].bits.is_timer = 0;
                }
                if (*lp == 'i' && *(lp + 1) == ':') {
                    lp += 2;
                    vtypes[vars].bits.is_autoinc = 1;
                } else {
                    vtypes[vars].bits.is_autoinc = 0;
                }

#ifdef USE_SCRIPT_GLOBVARS
                if (*lp == 'g' && *(lp + 1) == ':') {
                    lp += 2;
                    vtypes[vars].bits.global = 1;
                    glob_script_mem.udp_flags.udp_used = 1;
                } else {
                    vtypes[vars].bits.global = 0;
                }
#endif //USE_SCRIPT_GLOBVARS
                if (*lp == 'I' && *(lp + 1) == ':') {
                    lp += 2;
                    vtypes[vars].bits.integer = 1;
                } else {
                    vtypes[vars].bits.integer = 0;
                }
                if ((*lp == 'm' || *lp == 'M') && *(lp + 1) == ':') {
                    uint8_t flg = *lp;
                    lp += 2;
                    if (*lp == 'p' && *(lp + 1) == ':') {
                      vtypes[vars].bits.is_permanent = 1;
                      lp += 2;
                    }
                    if (flg == 'M') mfilt[numflt].numvals = 8;
                    else mfilt[numflt].numvals = 5;
                    vtypes[vars].bits.is_filter = 1;
                    mfilt[numflt].index = 0;
                    if (flg == 'M') {
                      mfilt[numflt].numvals |= OR_FILT_MASK;
                    }
                    vtypes[vars].index = numflt;
                    numflt++;
                    if (numflt > MAXFILT) {
                      if (imemptr) free(imemptr);
                      if (strings_op) free(strings_op);
                      return -6;
                    }
                } else {
                    vtypes[vars].bits.is_filter = 0;
                }

                *vnp_p++ = vnames_p;
                while (lp < op) {
                  if (*lp == ' ') {
                    // no spaces
                    lp++;
                  } else {
                    *vnames_p++ = *lp++;
                  }
                }
                *vnames_p++ = 0;
                // init variable
                op++;
                while (*op == ' ') {
                  // skip spaces
                  op++;
                }
                if (*op != '"') {
                    TS_FLOAT fv;
                    if (*op == '0' && *(op + 1) == 'x') {
                      op += 2;
                      if (vtypes[vars].bits.integer) {
                        *(uint32_t*)&fv = strtoll(op, &op, 16);
                      } else {
                        fv = strtol(op, &op, 16);
                      }
                    } else {
                      if (vtypes[vars].bits.integer) {
                        *(int32_t*)&fv = strtol(op, &op, 10);
                      } else {
                        fv=CharToFloat(op);
                      }
                    }
                    fvalues[nvars] = fv;
                    vtypes[vars].bits.is_string = 0;
                    if (!vtypes[vars].bits.is_filter) vtypes[vars].index = nvars;
                    nvars++;
                    if (nvars > maxnvars) {
                      if (imemptr) free(imemptr);
                      if (strings_op) free(strings_op);
                      return -1;
                    }
                    if (vtypes[vars].bits.is_filter) {
                      while (isdigit(*op) || *op == '.' || *op == '-') {
                        op++;
                      }
                      while (*op == ' ') op++;
                      if (isdigit(*op)) {
                        // lenght define follows
                        uint16_t flen = atoi(op);
                        if (flen > MAX_ARRAY_SIZE) {
                          // limit array size
                          flen = MAX_ARRAY_SIZE;
                        }
                        mfilt[numflt - 1].numvals &= OR_FILT_MASK;
                        mfilt[numflt - 1].numvals |= flen & AND_FILT_MASK;
                      }
                    }

                } else {
                    // string vars
                    op++;
                    *snp_p ++= strings_p;
                    while (*op != '\"') {
                      if (*op == SCRIPT_EOL) break;
                      char iob;
                      op = Get_esc_char(op, &iob);
                      //*strings_p++ = *op++;
                      *strings_p++ = iob;
                    }
                    *strings_p++ = 0;
                    vtypes[vars].bits.is_string = 1;
                    vtypes[vars].index = svars;
                    svars++;
                    if (svars > maxsvars) {
                      if (imemptr) free(imemptr);
                      if (strings_op) free(strings_op);
                      return -2;
                    }
                }
                vars++;
                if (vars > maxvars) {
                  if (imemptr) free(imemptr);
                  if (strings_op) free(strings_op);
                  return -3;
                }
            }
        } else {
            if (!strncmp(lp, ">D", 2)) {
              lp += 2;
              SCRIPT_SKIP_SPACES
              if (isdigit(*lp)) {
                uint8_t ssize = atoi(lp) + 1;
                if (ssize < 10 || ssize > SCRIPT_MAXSSIZE) ssize = SCRIPT_MAXSSIZE;
                glob_script_mem.max_ssize = ssize;
              }
              init = 1;
            }
        }
        // next line
    next_line:
        lp = strchr(lp, SCRIPT_EOL);
        if (!lp) break;
        lp++;
    }

    uint16_t fsize = 0;
    for (count = 0; count < numflt; count++) {
      fsize += sizeof(struct M_FILT) + ((mfilt[count].numvals & AND_FILT_MASK) - 1) * sizeof(TS_FLOAT);
    }

    // now copy vars to memory
    uint32_t script_mem_size =
    // number and number shadow vars
    (sizeof(TS_FLOAT)*nvars) +
    (sizeof(TS_FLOAT)*nvars) +
    // var names
    (vnames_p-vnames) +
    // vars offsets
#ifdef SCRIPT_LARGE_VNBUFF
    (sizeof(uint16_t)*vars) +
#else
    (sizeof(uint8_t)*vars) +
#endif
    // strings
    (glob_script_mem.max_ssize*svars) +
    // type array
    (sizeof(struct T_INDEX)*vars) +
    fsize;

    script_mem_size += 16;
    uint8_t *script_mem;
    script_mem = (uint8_t*)special_malloc(script_mem_size);
    if (!script_mem) {
      if (imemptr) free(imemptr);
      if (strings_op) free(strings_op);
      return -4;
    }

    memset(script_mem, 0, script_mem_size);

    glob_script_mem.script_mem = script_mem;
    glob_script_mem.script_mem_size = script_mem_size;

    // now copy all vars
    // numbers
    glob_script_mem.fvars = (TS_FLOAT*)script_mem;
    uint16_t size = sizeof(TS_FLOAT) * nvars;
    memcpy(script_mem, fvalues, size);
    script_mem += size;
    glob_script_mem.s_fvars = (TS_FLOAT*)script_mem;
    size = sizeof(TS_FLOAT) * nvars;
    memcpy(script_mem, fvalues, size);
    script_mem += size;

    glob_script_mem.mfilt = (struct M_FILT*)script_mem;
    script_mem += fsize;

    // memory types
    glob_script_mem.type = (struct T_INDEX*)script_mem;
    size = sizeof(struct T_INDEX)*vars;
    memcpy(script_mem, vtypes, size);
    script_mem += size;

    // var name strings
    char *namep = (char*)script_mem;
    glob_script_mem.glob_vnp = (char*)script_mem;
    size = vnames_p - vnames;
    memcpy(script_mem, vnames, size);
    script_mem += size;

#ifdef SCRIPT_LARGE_VNBUFF
    uint32_t alignedmem = (uint32_t)script_mem;
    if (alignedmem&1) {
      alignedmem++;
      size = vars*sizeof(uint16_t)+1;
    } else {
      size = vars*sizeof(uint16_t);
    }
    glob_script_mem.vnp_offset = (uint16_t*)alignedmem;
#else
    glob_script_mem.vnp_offset = (uint8_t*)script_mem;
    size = vars*sizeof(uint8_t);
#endif

    script_mem += size;

    // strings
    char *snamep = (char*)script_mem;
    glob_script_mem.glob_snp = (char*)script_mem;
    size = glob_script_mem.max_ssize * svars;
    //memcpy(script_mem,strings,size);
    script_mem += size;

    // now must recalc memory offsets
    uint16_t index = 0;
#ifdef SCRIPT_LARGE_VNBUFF
#ifndef MAXVNSIZ
#define MAXVNSIZ 4096
#endif
    uint16_t *cp = glob_script_mem.vnp_offset;
#else
#undef MAXVNSIZ
#define MAXVNSIZ 255
    uint8_t *cp = glob_script_mem.vnp_offset;
#endif
    for (count = 0; count < vars; count++) {
        *cp++ = index;
        while (*namep) {
            index++;
            namep++;
        }
        namep++;
        index++;
        if (index > MAXVNSIZ) {
          free(glob_script_mem.script_mem);
          if (imemptr) free(imemptr);
          if (strings_op) free(strings_op);
          return -5;
        }
    }

    // variables usage info
    AddLog(LOG_LEVEL_INFO, PSTR("SCR: nv=%d, tv=%d, vns=%d, vmem=%d, smem=%d"), nvars, svars, index, glob_script_mem.script_mem_size, glob_script_mem.script_size);

    // copy string variables
    char *cp1 = glob_script_mem.glob_snp;
    //char *sp = strings;
    char *sp = strings_op;

    for (count = 0; count < svars; count++) {
        strcpy(cp1,sp);
        sp += strlen(sp) + 1;
        cp1 += glob_script_mem.max_ssize;
    }

    // setup filter vars
    uint8_t *mp = (uint8_t*)glob_script_mem.mfilt;
    for (count = 0; count<numflt; count++) {
      struct M_FILT *mflp = (struct M_FILT*)mp;
      mflp->numvals = mfilt[count].numvals;
      mp += sizeof(struct M_FILT) + ((mfilt[count].numvals & AND_FILT_MASK) - 1) * sizeof(TS_FLOAT);
    }

    glob_script_mem.numvars = vars;
    glob_script_mem.script_dprec = SCRIPT_FLOAT_PRECISION;
    glob_script_mem.script_lzero = 0;
    glob_script_mem.script_sepc = '.';
    glob_script_mem.script_loglevel = LOG_LEVEL_INFO;


#if SCRIPT_DEBUG>2
    struct T_INDEX *dvtp = glob_script_mem.type;
    char out[128];
    char string[32];
    for (uint16_t count = 0; count < glob_script_mem.numvars; count++) {
      char *cp = glob_script_mem.glob_vnp + glob_script_mem.vnp_offset[count];
      if (dvtp[count].bits.is_string) {
        strlcpy(string, glob_script_mem.glob_snp + (dvtp[count].index * glob_script_mem.max_ssize), SCRIPT_MAXSSIZE);
      } else {
        f2char(glob_script_mem.fvars[dvtp[count].index], glob_script_mem.script_dprec, glob_script_mem.script_lzero, string, '.');
      }
      sprintf(out, "%d : %s = %s", count, cp, string);
      toLog(out);
    }
#endif //SCRIPT_DEBUG

    // now preset permanent vars
    TS_FLOAT *fp = (TS_FLOAT*)glob_script_mem.script_pram;
    struct T_INDEX *vtp = glob_script_mem.type;
    for (uint8_t count = 0; count<glob_script_mem.numvars; count++) {
      if (vtp[count].bits.is_permanent && !vtp[count].bits.is_string) {
        uint8_t index = vtp[count].index;
        if (vtp[count].bits.is_filter) {
            // preset array
            uint16_t len = 0;
            TS_FLOAT *fa = Get_MFAddr(index, &len, 0);
            while (len--) {
              *fa++ = *fp++;
            }
        } else {
          if (!isnan(*fp)) {
            glob_script_mem.fvars[index] = *fp;
          } else {
            *fp = glob_script_mem.fvars[index];
          }
          fp++;
        }
      }
    }
    sp = (char*)fp;
    for (uint8_t count = 0; count<glob_script_mem.numvars; count++) {
      if (vtp[count].bits.is_permanent && vtp[count].bits.is_string) {
        uint8_t index = vtp[count].index;
        char *dp = glob_script_mem.glob_snp + (index * glob_script_mem.max_ssize);
        uint8_t slen = strlen(sp);
        strlcpy(dp, sp, glob_script_mem.max_ssize);
        sp += slen + 1;
      }
    }

#if SCRIPT_DEBUG>0
    ClaimSerial();
    SetSerialBaudrate(9600);
#endif //SCRIPT_DEBUG

    // store start of actual program here
    glob_script_mem.scriptptr = lp - 1;
    glob_script_mem.scriptptr_bu = glob_script_mem.scriptptr;

#ifdef USE_SCRIPT_GLOBVARS
    if (glob_script_mem.udp_flags.udp_used) {
      Script_Init_UDP();
      if (Run_Scripter1(">G", -2, 0) == 99) {glob_script_mem.glob_script = glob_script_mem.section_ptr + 2;} else {glob_script_mem.glob_script = 0;}
    }
#endif //USE_SCRIPT_GLOBVARS

    if (imemptr) {
      free(imemptr);
      if (strings_op) free(strings_op);
    }
    return err;
}


int32_t udp_call(char *url, uint32_t port, char *sbuf) {
  WiFiUDP udp;
  IPAddress adr;
  adr.fromString(url);
  udp.begin(port);
  udp.beginPacket(adr, port);
  udp.write((const uint8_t*)sbuf, strlen(sbuf));
  udp.endPacket();
  udp.flush();
  udp.stop();
  return 0;
}

#ifdef USE_SCRIPT_GLOBVARS
#define SCRIPT_UDP_BUFFER_SIZE 128
#define SCRIPT_UDP_PORT 1999

//#define SCRIPT_DEBUG_UDP

void Restart_globvars(void) {
  Script_Stop_UDP();
  Script_Init_UDP();
}

void Script_Stop_UDP(void) {
  if (!glob_script_mem.udp_flags.udp_used) return;
  if (glob_script_mem.udp_flags.udp_connected) {
    glob_script_mem.Script_PortUdp.flush();
    glob_script_mem.Script_PortUdp.stop();
    glob_script_mem.udp_flags.udp_connected  = 0;
  }
}

void Script_Init_UDP() {
  if (TasmotaGlobal.global_state.network_down) return;
  if (!glob_script_mem.udp_flags.udp_used) return;
  if (glob_script_mem.udp_flags.udp_connected) return;

  //if (glob_script_mem.Script_PortUdp.beginMulticast(WiFi.localIP(), IPAddress(239,255,255,250), SCRIPT_UDP_PORT)) {
#ifdef ESP8266
  if (glob_script_mem.Script_PortUdp.beginMulticast(WiFi.localIP(), IPAddress(239,255,255,250), SCRIPT_UDP_PORT)) {
#else
  if (glob_script_mem.Script_PortUdp.beginMulticast(IPAddress(239,255,255,250), SCRIPT_UDP_PORT)) {
#endif


#ifdef SCRIPT_DEBUG_UDP
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_UPNP "SCR: UDP started"));
#endif
    glob_script_mem.udp_flags.udp_connected = 1;
  } else {
#ifdef SCRIPT_DEBUG_UDP
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_UPNP "SCR: UDP failed"));
#endif
    glob_script_mem.udp_flags.udp_connected  = 0;
  }
}



void Script_PollUdp(void) {
  if (TasmotaGlobal.global_state.network_down) return;
  if (!glob_script_mem.udp_flags.udp_used) return;
  if (glob_script_mem.udp_flags.udp_connected ) {
    uint32_t timeout = millis();
    while (glob_script_mem.Script_PortUdp.parsePacket()) {
      // not more then 500 ms
      if (millis() - timeout > 500) { break;}
      char packet_buffer[SCRIPT_UDP_BUFFER_SIZE];
      int32_t len = glob_script_mem.Script_PortUdp.read(packet_buffer, SCRIPT_UDP_BUFFER_SIZE - 1);
      packet_buffer[len] = 0;
      glob_script_mem.script_udp_remote_ip = glob_script_mem.Script_PortUdp.remoteIP();
#ifdef SCRIPT_DEBUG_UDP
      //AddLog(LOG_LEVEL_DEBUG, PSTR("UDP: Packet %s - %d - %s"), packet_buffer, len, script_udp_remote_ip.toString().c_str());
      AddLog(LOG_LEVEL_DEBUG, PSTR("UDP: Packet %s - %d - %_I"), packet_buffer, len, (uint32_t)glob_script_mem.script_udp_remote_ip);
#endif
      char *lp = packet_buffer;
      if (!strncmp(lp,"=>", 2)) {
        lp += 2;
        char *cp=strchr(lp, '=');
        if (cp) {
          char vnam[32];
          for (uint32_t count = 0; count<len; count++) {
            if (lp[count] == '=') {
              vnam[count] = 0;
              break;
            }
            vnam[count] = lp[count];
          }
          TS_FLOAT *fp;
          char *sp;
          uint32_t index;
          uint32_t res = match_vars(vnam, &fp, &sp, &index);
          if (res == NUM_RES) {
#ifdef SCRIPT_DEBUG_UDP
            AddLog(LOG_LEVEL_DEBUG, PSTR("SCR: num var found - %s - %d - %d"), vnam, res, index);
#endif
            *fp=CharToFloat(cp + 1);
          } else if (res == STR_RES) {
#ifdef SCRIPT_DEBUG_UDP
            AddLog(LOG_LEVEL_DEBUG, PSTR("SCR: string var found - %s - %d - %d"), vnam, res, index);
#endif
            strlcpy(sp, cp + 1, SCRIPT_MAXSSIZE);
          } else {
            // error var not found
          }
          if (res) {
            // mark changed
            glob_script_mem.last_udp_ip = glob_script_mem.Script_PortUdp.remoteIP();
            SetChanged(index);
            if (glob_script_mem.glob_script) {
              Run_Scripter1(glob_script_mem.glob_script, 0, 0);
            }
          }
        }
      }
      optimistic_yield(100);
    }
  } else {
    Script_Init_UDP();
  }
}

void script_udp_sendvar(char *vname, TS_FLOAT *fp, char *sp);

void script_udp_sendvar(char *vname, TS_FLOAT *fp, char *sp) {
  if (!glob_script_mem.udp_flags.udp_used) return;
  if (!glob_script_mem.udp_flags.udp_connected) return;

  char sbuf[SCRIPT_MAXSSIZE + 4];
  strcpy(sbuf, "=>");
  strcat(sbuf, vname);
  strcat(sbuf, "=");
  if (fp) {
    char flstr[16];
    dtostrfd(*fp, 8, flstr);
    strcat(sbuf, flstr);
#ifdef SCRIPT_DEBUG_UDP
    AddLog(LOG_LEVEL_DEBUG, PSTR("SCR: num var updated - %s"), sbuf);
#endif
  } else {
    strcat(sbuf, sp);
#ifdef SCRIPT_DEBUG_UDP
    AddLog(LOG_LEVEL_DEBUG, PSTR("SCR: string var updated - %s"), sbuf);
#endif
  }
  glob_script_mem.Script_PortUdp.beginPacket(IPAddress(239, 255, 255, 250), SCRIPT_UDP_PORT);
  //  Udp.print(String("RET UC: ") + String(recv_Packet));
  glob_script_mem.Script_PortUdp.write((const uint8_t*)sbuf, strlen(sbuf));
  glob_script_mem.Script_PortUdp.endPacket();
}

#endif //USE_SCRIPT_GLOBVARS

#ifdef USE_LIGHT
#ifdef USE_WS2812
void ws2812_set_array(TS_FLOAT *array ,uint32_t len, uint32_t offset);

void ws2812_set_array(TS_FLOAT *array ,uint32_t len, uint32_t offset) {

  Ws2812ForceSuspend();
  for (uint32_t cnt = 0; cnt < len; cnt++) {
    uint32_t index;
    if (! (offset & 0x1000)) {
      index = cnt + (offset & 0x7ff);
    } else {
      index = cnt/2 + (offset & 0x7ff);
    }
    if (index > Settings->light_pixels) break;
    if (! (offset & 0x1000)) {
      uint32_t col = array[cnt];
      Ws2812SetColor(index + 1, col>>16, col>>8, col, 0);
    } else {
      uint32_t hcol = array[cnt];
      cnt++;
      uint32_t lcol = array[cnt];
      Ws2812SetColor(index + 1, hcol>>8, hcol, lcol>>8, lcol);
    }
  }
  Ws2812ForceUpdate();
}
#endif //USE_WS2812
#endif //USE_LIGHT

TS_FLOAT median_array(TS_FLOAT *array, uint16_t len);
TS_FLOAT median_array(TS_FLOAT *array, uint16_t len) {
    uint8_t ind[len];
    uint8_t mind = 0;
    uint8_t index = 0;
    uint8_t flg;
    TS_FLOAT min = FLT_MAX;

    for (uint16_t hcnt = 0; hcnt < len / 2 + 1; hcnt++) {
        for (uint16_t mcnt = 0; mcnt < len; mcnt++) {
            flg = 0;
            for (uint16_t icnt = 0; icnt < index; icnt++) {
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

TS_FLOAT *Get_MFAddr(uint8_t index, uint16_t *len, uint16_t *ipos);
TS_FLOAT *Get_MFAddr(uint8_t index, uint16_t *len, uint16_t *ipos) {
  *len = 0;
  uint8_t *mp = (uint8_t*)glob_script_mem.mfilt;
  for (uint8_t count = 0; count < MAXFILT; count++) {
    struct M_FILT *mflp = (struct M_FILT*)mp;
    if (count == index) {
        *len = mflp->numvals & AND_FILT_MASK;
        if (ipos) *ipos = mflp->index;
        return mflp->rbuff;
    }
    mp += sizeof(struct M_FILT) + ((mflp->numvals & AND_FILT_MASK) - 1) * sizeof(TS_FLOAT);
  }
  return 0;
}

char *isvar(char *lp, uint8_t *vtype, struct T_INDEX *tind, TS_FLOAT *fp, char *sp, struct GVARS *gv);
char *get_array_by_name(char *lp, TS_FLOAT **fp, uint16_t *alen, uint16_t *ipos);

char *get_array_by_name(char *lp, TS_FLOAT **fp, uint16_t *alen, uint16_t *ipos) {
  struct T_INDEX ind;
  uint8_t vtype;
  while (*lp == ' ') lp++;
  lp = isvar(lp, &vtype, &ind, 0, 0, 0);
  if (vtype == VAR_NV) return 0;
  if (vtype & STYPE) return 0;
  uint16_t index = glob_script_mem.type[ind.index].index;
  if (glob_script_mem.type[ind.index].bits.is_filter) {
    TS_FLOAT *fa = Get_MFAddr(index, alen, ipos);
    *fp = fa;
    return lp;
  }
  *fp = 0;
  return lp;
}
TS_FLOAT *get_array_by_name(char *name, uint16_t *alen);
TS_FLOAT *get_array_by_name(char *name, uint16_t *alen) {
  struct T_INDEX ind;
  uint8_t vtype;
  isvar(name, &vtype, &ind, 0, 0, 0);
  if (vtype == VAR_NV) return 0;
  if (vtype & STYPE) return 0;
  uint16_t index = glob_script_mem.type[ind.index].index;

  if (glob_script_mem.type[ind.index].bits.is_filter) {
    TS_FLOAT *fa = Get_MFAddr(index, alen, 0);
    return fa;
  }
  return 0;
}
TS_FLOAT Get_MFVal(uint8_t index, int16_t bind);
TS_FLOAT Get_MFVal(uint8_t index, int16_t bind) {
  uint8_t *mp = (uint8_t*)glob_script_mem.mfilt;
  for (uint8_t count = 0; count < MAXFILT; count++) {
    struct M_FILT *mflp = (struct M_FILT*)mp;
    if (count == index) {
        uint16_t maxind = mflp->numvals & AND_FILT_MASK;
        if (!bind) {
          return mflp->index;
        }
        if (bind == -1) {
          return maxind;
        }
        if (bind == -2) {
          TS_FLOAT summ = 0;
          for (uint32_t cnt = 0; cnt < maxind; cnt++) {
            summ += mflp->rbuff[cnt];
          }
          return summ / maxind;
        }
        if (bind == -3) {
          TS_FLOAT summ = 0;
          for (uint32_t cnt = 0; cnt < maxind; cnt++) {
            summ += mflp->rbuff[cnt];
          }
          return summ;
        }
        if (bind < -2 || bind > maxind ) bind = 1;
        return mflp->rbuff[bind - 1];
    }
    mp += sizeof(struct M_FILT) + ((mflp->numvals & AND_FILT_MASK) - 1) * sizeof(TS_FLOAT);
  }
  return 0;
}
void Set_MFVal(uint8_t index, uint16_t bind, TS_FLOAT val);
void Set_MFVal(uint8_t index, uint16_t bind, TS_FLOAT val) {
  uint8_t *mp = (uint8_t*)glob_script_mem.mfilt;
  for (uint8_t count = 0; count < MAXFILT; count++) {
    struct M_FILT *mflp = (struct M_FILT*)mp;
    if (count == index) {
        uint16_t maxind = mflp->numvals & AND_FILT_MASK;
        if (!bind) {
          if (val < 0) {
            // shift whole array by value
          } else {
            // set array index value
            if (val < 0 || val >= maxind) val = 0;
            mflp->index = val;
          }
        } else {
          if (bind >= 1 && bind <= maxind) {
            mflp->rbuff[bind - 1] = val;
          }
        }
        return;
    }
    mp += sizeof(struct M_FILT) + ((mflp->numvals & AND_FILT_MASK) - 1) * sizeof(TS_FLOAT);
  }
}

TS_FLOAT Get_MFilter(uint8_t index);
TS_FLOAT Get_MFilter(uint8_t index) {
  uint8_t *mp = (uint8_t*)glob_script_mem.mfilt;
  for (uint8_t count = 0; count < MAXFILT; count++) {
    struct M_FILT *mflp = (struct M_FILT*)mp;
    if (count == index) {
      if (mflp->numvals & OR_FILT_MASK) {
        // moving average
        return mflp->maccu / (mflp->numvals & AND_FILT_MASK);
      } else {
        // median, sort array indices
        return median_array(mflp->rbuff, mflp->numvals);
      }
    }
    mp += sizeof(struct M_FILT) + ((mflp->numvals & AND_FILT_MASK) - 1) * sizeof(TS_FLOAT);
  }
  return 0;
}
void Set_MFilter(uint8_t index, TS_FLOAT invar);
void Set_MFilter(uint8_t index, TS_FLOAT invar) {
  uint8_t *mp = (uint8_t*)glob_script_mem.mfilt;
  for (uint8_t count = 0; count<MAXFILT; count++) {
    struct M_FILT *mflp = (struct M_FILT*)mp;
    if (count==index) {
      if (mflp->numvals & OR_FILT_MASK) {
        // moving average
        mflp->maccu -= mflp->rbuff[mflp->index];
        mflp->maccu += invar;
        mflp->rbuff[mflp->index] = invar;
        mflp->index++;
        if (mflp->index>=(mflp->numvals&AND_FILT_MASK)) mflp->index = 0;
      } else {
        // median
        mflp->rbuff[mflp->index] = invar;
        mflp->index++;
        if (mflp->index>=mflp->numvals) mflp->index = 0;
      }
      break;
    }
    mp += sizeof(struct M_FILT) + ((mflp->numvals & AND_FILT_MASK) - 1) * sizeof(TS_FLOAT);
  }
}

#define MEDIAN_SIZE 5
#define MEDIAN_FILTER_NUM 2

struct MEDIAN_FILTER {
TS_FLOAT buffer[MEDIAN_SIZE];
int8_t index;
} script_mf[MEDIAN_FILTER_NUM];
TS_FLOAT DoMedian5(uint8_t index, TS_FLOAT in);
TS_FLOAT DoMedian5(uint8_t index, TS_FLOAT in) {

  if (index >= MEDIAN_FILTER_NUM) index = 0;

  struct MEDIAN_FILTER* mf = &script_mf[index];
  mf->buffer[mf->index] = in;
  mf->index++;
  if (mf->index >= MEDIAN_SIZE) mf->index = 0;
  return median_array(mf->buffer, MEDIAN_SIZE);
}


#ifdef USE_UFILESYS
void fread_str_fp(File *fp, char *sp, uint16_t slen, uint16_t flg) {
  uint16_t index = 0;
  while (fp->available()) {
    uint8_t buf[1], iob;
    fp->read(buf, 1);
    iob = buf[0];
    if (flg) {
      if (iob == '\n') {
        break;
      }
    } else {
      if (iob == '\t' || iob == ',' || iob == '\n' || iob == '\r') {
        break;
      }
    }
    *sp++ = iob;
    index++;
    if (index >= slen - 1) break;
  }
  *sp = 0;
}

int32_t script_copy_file(File *source, File *dest, uint32_t sf_from, uint32_t sf_to, uint32_t flag, WiFiClient *client) {
int32_t res = 0;
uint32_t fsize = sf_to - sf_from;

  uint8_t *fbuff = (uint8_t*)malloc(512);
  uint16_t rsize = 512;
  if (fbuff) {
    if (flag) {
      // flag > 0 copy header
      source->seek(0, SeekSet);
      fread_str_fp(source, (char*)fbuff, rsize, 1);
      uint16_t ssize = strlen((char*)fbuff);
      fbuff[ssize++] = '\n';
      fbuff[ssize] = 0;
      if (dest) {
        dest->write(fbuff, ssize);
      }
      if (client) {
        client->write(fbuff, ssize);
      }
    }

    // seek to start
    source->seek(sf_from, SeekSet);
    while (fsize) {
      if (fsize < rsize) {
        rsize = fsize;
      }
      source->read(fbuff, rsize);
      if (dest) {
        dest->write(fbuff, rsize);
      }
      if (client) {
        client->write(fbuff, rsize);
      }
      fsize -= rsize;
    }
    free(fbuff);
  } else {
    return -3;
  }
  return res;
}


#ifdef USE_FEXTRACT

struct FE_TM {
  uint16_t year;
  uint8_t month;
  uint8_t day;
  uint8_t hour;
  uint8_t mins;
  uint8_t secs;
};

// timestamp math add days
int32_t tso(char *src, int32_t days, int32_t flg) {
struct tm tmx;
struct tm *tmp;
struct FE_TM tm;
  uint8_t mode = ts2ts(&tm, src);

  tmx.tm_sec = tm.secs;
  tmx.tm_min = tm.mins;
  tmx.tm_hour = tm.hour;
  tmx.tm_mon = tm.month - 1;
  tmx.tm_year = tm.year + 100;
  tmx.tm_mday = tm.day;
  time_t tmd = mktime(&tmx);
  tmd += days * (24 * 3600);
  tmp = gmtime(&tmd);
  if (!flg) {
    tm.secs = tmp->tm_sec;
    tm.mins = tmp->tm_min;
    tm.hour = tmp->tm_hour;
  } else {
    tm.secs = 0;
    tm.mins = 0;
    tm.hour = 0;
  }
  tm.month = tmp->tm_mon + 1;
  tm.year  = tmp->tm_year - 100;
  tm.day = tmp->tm_mday;
  tss2ts(&tm, src, mode);
  return 0;
}

uint32_t ts2ts(struct FE_TM *tm, char *ts) {
  if (strchr(ts, 'T')) {
    // 2020-12-16T15:36:41
    tm->year = strtol(ts, &ts, 10);
    if (tm->year < 2020 || tm->year > 2040) {
      tm->year = 2020;
    }
    tm->year -= 2000;
    ts++;
    tm->month = strtol(ts, &ts, 10);
    ts++;
    tm->day = strtol(ts, &ts, 10);
    ts++;
    tm->hour = strtol(ts, &ts, 10);
    ts++;
    tm->mins = strtol(ts, &ts, 10);
    ts++;
    tm->secs = strtol(ts, &ts, 10);
    return 0;
  } else {
    // german excel format 16.12.20 15:36
    tm->day = strtol(ts, &ts, 10);
    ts++;
    tm->month = strtol(ts, &ts, 10);
    ts++;
    tm->year = strtol(ts, &ts, 10);
    ts++;
    tm->hour = strtol(ts, &ts, 10);
    ts++;
    tm->mins = strtol(ts, &ts, 10);
    tm->secs = 0;
    return 1;
  }
  return 0;
}

void tss2ts(struct FE_TM *tm, char *dst, uint8_t mode) {
  if (mode & 1 == 1) {
    // was tsm format go to 16.12.20 15:36
    char c = ' ';
    if (mode & 0x80) {
        c = '-';
    }
    sprintf(dst, "%01d.%01d.%01d%c%01d:%02d", tm->day, tm->month, tm->year, c, tm->hour, tm->mins);
  } else {
    // 2020-12-16T15:36:41
    sprintf(dst, "%04d-%02d-%02dT%02d:%02d:%02d", tm->year + 2000, tm->month, tm->day, tm->hour, tm->mins, tm->secs);
  }
}

// convert time stamp format
void cnvts(char *dst, char *src, uint8_t flg ) {
struct FE_TM tm;
  uint8_t mode = ts2ts(&tm, src);
  tss2ts(&tm, dst, flg);
}

// convert tasmota time stamp to ul seconds
uint32_t tstamp2l(char *ts) {
struct FE_TM tm;
struct tm tmx;
  uint8_t mode = ts2ts(&tm, ts);

  tmx.tm_sec = tm.secs;
  tmx.tm_min = tm.mins;
  tmx.tm_hour = tm.hour;
  tmx.tm_mon = tm.month - 1;
  tmx.tm_year = tm.year + 100;
  tmx.tm_mday = tm.day;
  time_t tmd = mktime(&tmx);

  return tmd;
}

// convert seconds to tasmota time stamp
uint32_t s2tstamp(char *ts, uint32_t tsize, uint32_t seconds, uint32_t flg) {

  time_t tmd = seconds;
  struct tm *tmp;
  struct FE_TM tm;
  tmp = gmtime(&tmd);
  if (!flg) {
    tm.secs = tmp->tm_sec;
    tm.mins = tmp->tm_min;
    tm.hour = tmp->tm_hour;
  } else {
    tm.secs = 0;
    tm.mins = 0;
    tm.hour = 0;
  }
  tm.month = tmp->tm_mon + 1;
  tm.year  = tmp->tm_year - 100;
  tm.day = tmp->tm_mday;
  tss2ts(&tm, ts, 0);
  return 0;
}

// optimized access, estimate entry point
int32_t opt_fext(File *fp,  char *ts_from, char *ts_to, uint32_t flg) {
  // seek to start
  int32_t fres = extract_from_file(fp,  ts_from, ts_to, -2, 0, 0, 0, 0);
  int32_t start = fres;
  char tsf[32];
  fread_str_fp(fp, tsf, sizeof(tsf), 0);
  uint32_t ltsf = tstamp2l(tsf);
  fres = extract_from_file(fp,  ts_from, ts_to, -1, 0, 0, 0, 0);
  int32_t end = fres;
  fread_str_fp(fp, tsf, sizeof(tsf), 0);
  uint32_t tssiz = tstamp2l(tsf) - ltsf;
  uint32_t tspos = tstamp2l(ts_from) - ltsf;
  TS_FLOAT perc =  (TS_FLOAT)tspos / (TS_FLOAT)tssiz * 0.8;
  if (perc < 0) perc = 0;
  if (perc > 1) perc = 1;
  TS_FLOAT fsize = fp->size();
  uint32_t spos = perc * fsize;
  //AddLog(LOG_LEVEL_INFO,PSTR(">>> 1 %d, %d"), (uint32_t)perc, spos);
  fp->seek(spos, SeekSet);
  fres = extract_from_file(fp,  ts_from, ts_to, -3, 0, 0, 0, 0);
  if (fres < 0) {
    if (flg) {
      if (flg == 1) {
        fres = start;
      } else {
        fres = end;
      }
    }
  }
  return fres;
}

// assume 1. entry is timestamp, others are tab delimited values until LF
// file reference, from timestamp, to timestampm, column offset, array pointers, array lenght, number of arrays
int32_t extract_from_file(File *fp,  char *ts_from, char *ts_to, int8_t coffs, TS_FLOAT **a_ptr, uint16_t *a_len, uint8_t numa, int16_t accum) {

  char rstr[32];
  uint8_t sindex = 0;
  uint8_t colpos = 0;
  uint8_t range = 0;
  if (coffs < 0) {
    uint32_t cpos = fp->size();
    if (coffs == -1) {
      // seek to last entry
      if (cpos > 1) cpos -= 2;
      // now seek back to last line
      uint8_t lbuff[256];
      uint8_t iob;
      uint16_t index = sizeof(lbuff) -1;
      fp->seek(cpos - sizeof(lbuff), SeekSet);
      fp->read(lbuff, sizeof(lbuff));
      while (cpos) {
        iob = lbuff[index];
        if (iob == '\n' || iob == '\r') {
          break;
        }
        cpos--;
        index--;
      }
      fp->seek(cpos, SeekSet);
    } else if (coffs == -2) {
      // seek to line 2
      fp->seek(0, SeekSet);
      for (uint32_t cp = 0; cp < cpos; cp++) {
        uint8_t buff[2], iob;
        fp->read(buff, 1);
        iob = buff[0];
        if (iob == '\n' || iob == '\r') {
          cpos = cp + 1;
          break;
        }
      }
    } else {
      // seek to pos of ts_from
      cpos = fp->position();
      uint32_t tsfrom = tstamp2l(ts_from);
      while (fp->available()) {
        uint8_t buff[2], iob;
        fp->read(buff, 1);
        cpos++;
        iob = buff[0];
        if (iob == '\n' || iob == '\r') {
          // read time stamp
          char ts[22];
          fp->read((uint8_t*)ts, sizeof(ts));
          char *cp = strchr(ts, '\t');
          if (cp) {
            *cp = 0;
            uint32_t tstc = tstamp2l(ts);
            //Serial.printf(">>> %s - %d - %d\n",ts, tstc, cpos );
            if (tstc >= tsfrom) {
              fp->seek(cpos, SeekSet);
              return cpos;
            }
          }
          cpos += sizeof(ts);
        }
      }
      return -1;
    }
    return cpos;
  }
  uint32_t ipos = fp->position();
  fp->seek(0, SeekSet);
  uint32_t tsfrom = tstamp2l(ts_from);
  uint32_t tsto = tstamp2l(ts_to);
  //AddLog(LOG_LEVEL_INFO, PSTR("from: %d  to: %d"),tsfrom,  tsto);
  uint16_t lines = 0;
  uint16_t rlines = 0;
  TS_FLOAT summs[numa];
  TS_FLOAT lastv[numa];
  uint16_t accnt[numa];
  uint8_t mflg[numa];
  uint32_t lastpos = 0;
  for (uint8_t cnt = 0; cnt < numa; cnt++) {
     summs[cnt] = 0;
     accnt[cnt] = 0;
     mflg[cnt] = 0;
     lastv[cnt] = 0;
  }
  uint8_t dflg = 0;
  if (accum < 0) {
    dflg = 1;
    accum = -accum;
  }
  if (accum == 0) accum = 1;
  while (fp->available()) {
    // scan through file
    uint8_t buff[2], iob;
    fp->read(buff, 1);
    iob = buff[0];
    if (iob == '\t' || iob == ',' || iob == '\n' || iob == '\r') {
      rstr[sindex] = 0;
      sindex = 0;
      if (lines == 0) {
        // header line, analye column names
        if (colpos >= 1 && colpos >= coffs) {
          uint8_t curpos = colpos - coffs;
          if (curpos < numa) {
            mflg[curpos] = dflg;
            char *tp = strstr(rstr, "_a");
            if (tp) {
              // mark average values
              mflg[curpos] |= 2;
            }
          }
        }
      } else {
        if (colpos == 0) {
          // timestamp  2020-12-16T15:36:41
          // decompose timestamps
          uint32_t cts = tstamp2l(rstr);
          if (cts > tsto) {
            // end of range must seek back to last LF, for next scan
            fp->seek(lastpos, SeekSet);
            break;
          }
          if (cts >= tsfrom && cts <= tsto) {
            // we want this range
            range = 1;
            rlines++;
          } else {
            range = 0;
          }
        } else {
          // data columns
          if (range) {
            uint8_t curpos = colpos - coffs;
            if (colpos >= coffs && curpos < numa) {
              if (a_len[curpos]) {
                TS_FLOAT fval = CharToFloat(rstr);
                uint8_t flg = 1;
                if ((mflg[curpos] & 1) == 1) {
                  // absolute values, build diffs
                  if (!(mflg[curpos] & 0x80)) {
                    lastv[curpos] = fval;
                    mflg[curpos] |= 0x80;
                    flg = 0;
                  } else {
                    if (!(mflg[curpos] & 2)) {
                      TS_FLOAT tmp = fval;
                      fval -= lastv[curpos];
                       // must be positive value
#ifndef EXTRACT_DIFF_NOCHK
                      if (fval < 0) fval = 0;
#endif
                      lastv[curpos] = tmp;
                    }
                  }
                }
                // average values
                //AddLog(LOG_LEVEL_INFO, PSTR("cpos %d colp %d numa %d - %s %d - %d"),curpos, colpos, a_len[curpos], rstr, (uint32_t)fval, flg);
                if (flg) {
                  summs[curpos] += fval;
                  accnt[curpos] += 1;
                  if (accnt[curpos] == accum) {
                    *a_ptr[curpos]++ = summs[curpos] / accum;
                    summs[curpos] = 0;
                    accnt[curpos] = 0;
                    a_len[curpos]--;
                  }
                }
              } else {
                break;
              }
            }
          }
        }
      }
      colpos++;
      if (iob == '\n' || iob == '\r') {
        lastpos = fp->position();
        colpos = 0;
        lines ++;
        if (lines == 1) {
          if (ipos) {
            fp->seek(ipos, SeekSet);
          }
        }
      }
    }
    rstr[sindex] = iob;
    sindex++;
  }
  return rlines;
}
#endif // USE_FEXTRACT
#endif // USE_UFILESYS


uint32_t script_bcd(uint8_t sel, uint32_t val) {
uint32_t res = 0;
  if (sel) {
    // to bcd
    uint32_t mfac = 1;
    for (uint32_t cnt = 0; cnt < 6; cnt++) {
      res |= (val % 10) << 24;
      val /= 10;
      res >>= 4;
    }
  } else {
    // from bcd
    uint32_t mfac = 1;
    for (uint32_t cnt = 0; cnt < 6; cnt++) {
      res += (val & 0xf) * mfac;
      val >>= 4;
      mfac *= 10;
    }
  }
  return res;
}


uint8_t script_hexnibble(char chr) {
  uint8_t rVal = 0;
  if (isdigit(chr)) {
    rVal = chr - '0';
  } else  {
    if (chr >= 'A' && chr <= 'F') rVal = chr + 10 - 'A';
    if (chr >= 'a' && chr <= 'f') rVal = chr + 10 - 'a';
  }
  return rVal;
}


#ifdef USE_LIGHT
uint32_t HSVToRGB(uint16_t hue, uint8_t saturation, uint8_t value) {
TS_FLOAT r = 0, g = 0, b = 0;
struct HSV {
	TS_FLOAT H;
	TS_FLOAT S;
	TS_FLOAT V;
} hsv;

hsv.H = hue;
hsv.S = (TS_FLOAT)saturation / 100.0;
hsv.V = (TS_FLOAT)value / 100.0;

if (hsv.S == 0) {
		r = hsv.V;
		g = hsv.V;
		b = hsv.V;
	} else {
		int i;
		TS_FLOAT f, p, q, t;

		if (hsv.H == 360)
			hsv.H = 0;
		else
			hsv.H = hsv.H / 60;

		i = (int)trunc(hsv.H);
		f = hsv.H - i;

		p = hsv.V * (1.0 - hsv.S);
		q = hsv.V * (1.0 - (hsv.S * f));
		t = hsv.V * (1.0 - (hsv.S * (1.0 - f)));

		switch (i)
		{
		case 0:
			r = hsv.V;
			g = t;
			b = p;
			break;

		case 1:
			r = q;
			g = hsv.V;
			b = p;
			break;

		case 2:
			r = p;
			g = hsv.V;
			b = t;
			break;

		case 3:
			r = p;
			g = q;
			b = hsv.V;
			break;

		case 4:
			r = t;
			g = p;
			b = hsv.V;
			break;

		default:
			r = hsv.V;
			g = p;
			b = q;
			break;
		}

	}

  uint8_t ir, ig, ib;
  ir = r * 255;
  ig = g * 255;
  ib = b * 255;

	uint32_t rgb = (ir<<16) | (ig<<8) | ib;
	return rgb;
}
#endif //USE_LIGHT

#ifdef ESP32
#ifdef JPEG_PICTS
#ifdef STREAM_JPEG_PICTS
struct JPG_TASK {
  char boundary[40];
  bool draw;
  uint8_t scale;
  uint16_t xp;
  uint16_t yp;
  WiFiClient stream;
  HTTPClient http;
} jpg_task;

// "e8b8c539-047d-4777-a985-fbba6edff11e"

int32_t fetch_jpg(uint32_t sel, char *url, uint32_t xp, uint32_t yp, uint32_t scale) {
  char hbuff[64];
  int32_t httpCode = 0;
  const char * headerKeys[] = {"Content-Type", "Content-Length"} ;
  const size_t numberOfHeaders = 2;

  switch (sel) {
    case 0:
      // open
      jpg_task.boundary[0] = 0;
      jpg_task.draw = false;
      jpg_task.xp = xp;
      jpg_task.yp = yp;
      jpg_task.scale = scale;
      sprintf(hbuff,"http://%s", url);
      jpg_task.http.begin(jpg_task.stream, hbuff);
      jpg_task.http.collectHeaders(headerKeys, numberOfHeaders);
      httpCode = jpg_task.http.GET();
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        String boundary = jpg_task.http.header(headerKeys[0]);
        char *cp = strchr(boundary.c_str(), '=');
        if (cp) {
          strcpy(jpg_task.boundary,cp + 1);
        }
      } else {
        AddLog(LOG_LEVEL_INFO,PSTR("SCR: HTTP error %d = %s"), httpCode, jpg_task.http.errorToString(httpCode).c_str());
      }
      return httpCode;
      break;
    case 1:
      // close
      jpg_task.stream.stop();
      jpg_task.http.end();
      break;
    case 2:
      // get next frame
      /*Wc.client.print("--" BOUNDARY "\r\n");
      Wc.client.printf("Content-Type: image/jpeg\r\n"
        "Content-Length: %d\r\n"
        "\r\n", static_cast<int>(_jpg_buf_len));
        */
      {
        if (jpg_task.http.connected()) {
          char inbuff[64];
          memset(inbuff, 0, sizeof(inbuff));
          jpg_task.stream.readBytesUntil('\n', inbuff, sizeof(inbuff));
          if (inbuff[0] == '\r') {
            memset(inbuff, 0, sizeof(inbuff));
            jpg_task.stream.readBytesUntil('\n', inbuff, sizeof(inbuff));
          }
          //AddLog(LOG_LEVEL_INFO, PSTR("boundary = %s"), inbuff);
          memset(inbuff, 0, sizeof(inbuff));
          jpg_task.stream.readBytesUntil('\n', inbuff, sizeof(inbuff));
          //AddLog(LOG_LEVEL_INFO, PSTR("type = %s"), inbuff);
          memset(inbuff, 0, sizeof(inbuff));
          jpg_task.stream.readBytesUntil('\n', inbuff, sizeof(inbuff));
          //AddLog(LOG_LEVEL_INFO, PSTR("size = %s"), inbuff);
          char *cp = strchr(inbuff, ':');
          uint16_t size = 0;
          if (cp) {
            size = atoi(cp + 1);
          }
          jpg_task.stream.readBytesUntil('\n', inbuff, sizeof(inbuff));

          if (size > 0) {
            //AddLog(LOG_LEVEL_INFO, PSTR("size = %d"), size);
            uint8_t *buff = (uint8_t *)special_malloc(size);
            if (buff) {
              jpg_task.stream.readBytes(buff, size);
            }
            if (jpg_task.draw) {
              Draw_jpeg(buff, size, jpg_task.xp, jpg_task.yp, jpg_task.scale);
            }
            if (buff) {
              free(buff);
            }
          }
          return size;
        }
      }
      break;
    case 3:
      // stop drawing
      jpg_task.draw = false;
      break;
    case 4:
      // resume drawing
      jpg_task.draw = true;
      break;
  }
  return 0;
}
#endif // STREAM_JPEG_PICTS
#endif // JPEG_PICTS
#endif // ESP32


#ifdef USE_ANGLE_FUNC
uint32_t pulse_time_hl;
uint32_t pulse_time_lh;
uint32_t pulse_ltime_hl;
uint32_t pulse_ltime_lh;
uint8_t pt_pin;

#define MPT_DEBOUNCE 10

void IRAM_ATTR MP_Timer(void) {
  uint32_t level = digitalRead(pt_pin&0x3f);
  uint32_t ms = millis();
  uint32_t time;
  if (level) {
    // rising edge
    pulse_ltime_lh = ms;
    time = ms - pulse_ltime_hl;
    if (time>MPT_DEBOUNCE) pulse_time_hl = time;
  } else {
    // falling edge
    pulse_ltime_hl = ms;
    time = ms - pulse_ltime_lh;
    if (time>MPT_DEBOUNCE) pulse_time_lh = time;
  }
}

uint32_t MeasurePulseTime(int32_t in) {
  if (in >= 0) {
    // define pin;
    pt_pin = in;
    pinMode(pt_pin & 0x3f, INPUT_PULLUP);
    attachInterrupt(pt_pin & 0x3f, MP_Timer, CHANGE);
    pulse_ltime_lh = millis();
    pulse_ltime_hl = millis();
    return 0;
  }
  uint32_t ptime;
  if (in==-1) {
    ptime = pulse_time_lh;
    pulse_time_lh = 0;
  } else {
    ptime = pulse_time_hl;
    pulse_time_hl = 0;
  }
  return ptime;
}
#endif // USE_ANGLE_FUNC

#ifdef USE_SCRIPT_GLOBVARS
uint32_t match_vars(char *dvnam, TS_FLOAT **fp, char **sp, uint32_t *ind) {
  uint16_t olen = strlen(dvnam);
  struct T_INDEX *vtp = glob_script_mem.type;
  for (uint32_t count = 0; count < glob_script_mem.numvars; count++) {
    char *cp = glob_script_mem.glob_vnp + glob_script_mem.vnp_offset[count];
    uint8_t slen = strlen(cp);
    if (slen == olen && *cp == dvnam[0]) {
      if (!strncmp(cp, dvnam, olen)) {
        uint8_t index = vtp[count].index;
        if (vtp[count].bits.is_string == 0) {
          if (vtp[count].bits.is_filter) {
            // error
            return 0;
          } else {
            *fp = &glob_script_mem.fvars[index];
            *ind = count;
            return NUM_RES;
          }
        } else {
          *sp = glob_script_mem.glob_snp + (index * glob_script_mem.max_ssize);
          *ind = count;
          return STR_RES;
        }
      }
    }
  }
  return 0;
}
#endif //USE_SCRIPT_GLOBVARS

#ifndef SCRIPT_IS_STRING_MAXSIZE
#define SCRIPT_IS_STRING_MAXSIZE 256
#endif


void script_sort_string_array(uint8_t num) {
  uint16_t sasize = glob_script_mem.si_num[num];
  char *sa = glob_script_mem.last_index_string[num];
  if (!sa) {
    return;
  }
  char temp[SCRIPT_MAXSSIZE];
  bool swapped;
  do {
    swapped = false;
    for (uint16_t i = 0; i < sasize - 1; ++i) {
      char *s1 = sa + (i * glob_script_mem.max_ssize);
      char *s2 = sa + ((i + 1) * glob_script_mem.max_ssize);
      if (strcmp(s1, s2) > 0) {
        // swap
        strcpy(temp, s1);
        strcpy(s1, s2);
        strcpy(s2, temp);
        swapped = true;
      }
    }
    sasize -= 1;
  } while (swapped);
}


char *isargs(char *lp, uint32_t isind) {
  TS_FLOAT fvar;
  lp = GetNumericArgument(lp, OPER_EQU, &fvar, 0);
  SCRIPT_SKIP_SPACES
  if (*lp != '"') {
    return lp;
  }
  lp++;

  if (glob_script_mem.si_num[isind] > 0 && glob_script_mem.last_index_string[isind]) {
    free(glob_script_mem.last_index_string[isind]);
  }
  char *sstart = lp;
  uint8_t slen = 0;
  for (uint32_t cnt = 0; cnt < SCRIPT_IS_STRING_MAXSIZE; cnt++) {
    if (*lp == '\n' || *lp == '"' || *lp == 0) {
      lp++;
      if (cnt > 0 && !slen) {
        slen++;
      }
      glob_script_mem.siro_num[isind] = slen;
      break;
    }
    if (*lp == '|') {
      slen++;
    }
    lp++;
  }

  glob_script_mem.si_num[isind] = fvar;
  if (glob_script_mem.si_num[isind] > 0) {
    if (glob_script_mem.si_num[isind] > MAX_SARRAY_NUM) {
      glob_script_mem.si_num[isind] = MAX_SARRAY_NUM;
    }

    glob_script_mem.last_index_string[isind] = (char*)calloc(glob_script_mem.max_ssize * glob_script_mem.si_num[isind], 1);
    for (uint32_t cnt = 0; cnt < glob_script_mem.siro_num[isind]; cnt++) {
      char str[SCRIPT_MAXSSIZE];
      GetTextIndexed(str, sizeof(str), cnt, sstart);
      strlcpy(glob_script_mem.last_index_string[isind] + (cnt * glob_script_mem.max_ssize), str, glob_script_mem.max_ssize);
    }
  } else {
    glob_script_mem.last_index_string[isind] = sstart;
  }
  lp++;
  return lp;
}

char *Get_esc_char(char *cp, char *esc_chr) {
char iob = *cp;
  if (iob == '\\') {
    cp++;
    if (*cp == 't') {
      iob = '\t';
    } else if (*cp == 'n') {
      iob = '\n';
    } else if (*cp == 'r') {
      iob = '\r';
    } else if (*cp == '"') {
      iob = '"';
    } else if (*cp == '0' && *(cp + 1) == 'x') {
      cp += 2;
      iob = strtol(cp, 0, 16);
      cp++;
    } else if (*cp == '\\') {
      iob = '\\';
    }
     else {
      cp--;
    }
  }
  *esc_chr = iob;
  cp++;
  return cp;
}


char *isget(char *lp, char *sp, uint32_t isind, struct GVARS *gv) {
TS_FLOAT fvar;
  lp = GetNumericArgument(lp, OPER_EQU, &fvar, 0);
  SCRIPT_SKIP_SPACES
  char str[SCRIPT_MAXSSIZE];
  str[0] = 0;
  uint8_t index = fvar;
  if (index < 1) index = 1;
  index--;
  if (gv) gv->strind = index;
  glob_script_mem.sind_num = isind;
  if (glob_script_mem.last_index_string[isind]) {
    if (!glob_script_mem.si_num[isind]) {
      if (index <= glob_script_mem.siro_num[isind]) {
        GetTextIndexed(str, sizeof(str), index , glob_script_mem.last_index_string[isind]);
      }
    } else {
      if (index >= glob_script_mem.si_num[isind]) {
        index = glob_script_mem.si_num[isind] - 1;
      }
      strlcpy(str,glob_script_mem.last_index_string[isind] + (index * glob_script_mem.max_ssize), glob_script_mem.max_ssize);
    }
  }
  lp++;
  if (sp) strlcpy(sp, str, glob_script_mem.max_ssize);
  return lp;
}


#ifdef SCRIPT_SAVE_RAM
#undef strncmp_XP
#define strncmp_XP strncmp_P
#undef XPSTR
#define XPSTR(A) PSTR(A)
#else
#undef strncmp_XP
#define strncmp_XP strncmp
#undef XPSTR
#define XPSTR(A) A
#endif

// vtype => ff=nothing found, fe=constant number,fd = constant string else bit 7 => 80 = string, 0 = number
// no flash strings here for performance reasons!!!
char *isvar(char *lp, uint8_t *vtype, struct T_INDEX *tind, TS_FLOAT *fp, char *sp, struct GVARS *gv);
char *isvar(char *lp, uint8_t *vtype, struct T_INDEX *tind, TS_FLOAT *fp, char *sp, struct GVARS *gv) {
    uint16_t count, len = 0;
    uint8_t nres = 0;
    char vname[64];
    TS_FLOAT fvar = 0;
    tind->index = 0;
    tind->bits.data = 0;

    //Serial.printf("Stack 2: %d\n",GetStack());

    if ( (*lp == '#') && (*(lp + 1) == '-' || isdigit(*(lp + 1))) ) {
      // 32 bit integer
      lp++;
      if (fp) {
          if (*lp == '0' && *(lp + 1) == 'x') {
            lp += 2;
            *(uint32_t*)fp = strtoll(lp, &lp, 16);
          } else {
            *(int32_t*)fp = strtoll(lp, &lp, 10);
          }
      }
      tind->bits.constant = 1;
      tind->bits.is_string = 0;
      tind->bits.integer = 1;
      *vtype = NUM_RES;
      return lp;
    }

    if (isdigit(*lp) || (*lp == '-' && isdigit(*(lp + 1))) || *lp == '.') {
      // isnumber
        if (fp) {
          if (*lp == '0' && *(lp + 1) == 'x') {

            lp += 2;
            *fp = strtoll(lp, &lp, 16);
          } else {
            *fp = CharToFloat(lp);
            if (*lp == '-') lp++;
            while (isdigit(*lp) || *lp == '.') {
              if (*lp == 0 || *lp == SCRIPT_EOL) break;
              lp++;
            }
          }
        }
        tind->bits.constant = 1;
        tind->bits.is_string = 0;
        *vtype = NUM_RES;
        return lp;
    }



    if (*lp == '"') {
      lp++;
      while (*lp != '"') {
        if (*lp == 0 || *lp == SCRIPT_EOL) break;
        char iob;
        lp = Get_esc_char(lp, &iob);
        if (sp) *sp++ = iob;
      }
      if (sp) *sp = 0;
      *vtype = STR_RES;
      tind->bits.constant = 1;
      tind->bits.is_string = 1;
      return lp + 1;
    }

    if (*lp=='-') {
      // inverted var
      nres = 1;
      lp++;
    }

    const char *term="\n\r ])=+-/*%><!^&|}{";
    for (count = 0; count < sizeof(vname); count++) {
        char iob = lp[count];
        if (!iob || strchr(term, iob)) {
            vname[count] = 0;
            break;
        }
        vname[count] = iob;
        len += 1;
    }

    if (!vname[0]) {
      // empty string
      *vtype = VAR_NV;
      tind->index = VAR_NV;
      glob_script_mem.var_not_found = 1;
      return lp;
    }

    struct T_INDEX *vtp = glob_script_mem.type;
    char dvnam[32];
    strcpy (dvnam, vname);
    uint8_t olen = len;
    if (gv) {
      gv->numind = -1;
      gv->strind = -1;
    }
    char *ja = strchr(dvnam, '[');
    if (ja) {
      *ja = 0;
      ja++;
      olen = strlen(dvnam);
    }


    glob_script_mem.arres = 0;
    for (count = 0; count < glob_script_mem.numvars; count++) {
        char *cp = glob_script_mem.glob_vnp + glob_script_mem.vnp_offset[count];
        uint8_t slen = strlen(cp);
        if (slen == olen && *cp == dvnam[0]) {
            if (!strncmp(cp, dvnam, olen)) {
                uint16_t index = vtp[count].index;
                *tind = vtp[count];
                tind->index = count; // overwrite with global var index
                if (vtp[count].bits.is_string == 0) {
                    *vtype = NTYPE; // | index;
                    if (vtp[count].bits.is_filter) {
                      if (ja) {
                        lp += olen + 1;
                        lp = GetNumericArgument(lp, OPER_EQU, &fvar, gv);
                        if (gv) gv->numind = fvar;
                        fvar = Get_MFVal(index, fvar);
                        len = 1;
                      } else {
                        fvar = Get_MFilter(index);
                        glob_script_mem.arres = 1;
                      }
                    } else {
                      if (ja) continue;
                      fvar = glob_script_mem.fvars[index];
                    }
                    if (nres) fvar = -fvar;
                    if (fp) *fp = fvar;
                } else {
                    *vtype = STYPE;  //|index;
                    if (sp) strlcpy(sp, glob_script_mem.glob_snp + (index * glob_script_mem.max_ssize), SCRIPT_MAXSSIZE);
                }
                return lp + len;
            }
        }
    }


#define USE_SCRIPT_JSON
//#define USE_SCRIPT_FULL_JSON_PARSER

#ifdef USE_SCRIPT_JSON
    if (gv && gv->jo) {
      // look for json input

#ifdef USE_SCRIPT_FULL_JSON_PARSER
  // epoch offset missing in this version
      char str_value[SCRIPT_MAXSSIZE];
      str_value[0]=0;
      TS_FLOAT fv;
      uint32_t res = JsonParsePath(gv->jo, vname, '#', &fv, str_value, sizeof(str_value));
      if (!res) {
        goto chknext;
      }
      if (res == 1) {
        // numeric
nexit:
        if (fp) *fp = fv;
        *vtype = NUM_RES;
        tind->bits.constant = 1;
        tind->bits.is_string = 0;
        return lp + len;
      } else {
        // string
        if (!strncmp_XP(str_value, XPSTR("ON"), 2)) {
          if (fp) *fp = 1;
          goto nexit;
        } else if (!strncmp_XP(str_value, XPSTR("OFF"), 3)) {
          if (fp) *fp = 0;
          goto nexit;
        } else {
          *vtype = STR_RES;
          tind->bits.constant = 1;
          tind->bits.is_string = 1;
          if (sp) strlcpy(sp, str_value, SCRIPT_MAXSSIZE);
          return lp + len;
        }
      }

#else
      JsonParserObject *jpo = gv->jo;
      char jvname[64];
      strcpy(jvname, vname);
      const char* str_value;
      uint8_t aindex;
      String vn;
      char *ja=strchr(jvname, '[');
      if (ja) {
        // json array
        *ja = 0;
        ja++;
        // fetch array index
        TS_FLOAT fvar;
        GetNumericArgument(ja, OPER_EQU, &fvar, 0);
        aindex = fvar;
        if (aindex<1 || aindex>6) aindex = 1;
        aindex--;
      }
      if (jpo->isValid()) {
        char *subtype = strchr(jvname, '#');
        char *subtype2;
        if (subtype) {
          *subtype = 0;
          subtype++;
          subtype2 = strchr(subtype, '#');
          if (subtype2) {
            *subtype2 = 0;
            *subtype2++;
          }
        }
        vn = jvname;
        str_value = (*jpo)[vn].getStr();
        if ((*jpo)[vn].isValid()) {
          if (subtype) {
            JsonParserObject jobj1 = (*jpo)[vn];
            if (jobj1.isValid()) {
              vn = subtype;
              jpo = &jobj1;
              str_value = (*jpo)[vn].getStr();
              if ((*jpo)[vn].isValid()) {
                // 2. stage
                if (subtype2) {
                  JsonParserObject jobj2 = (*jpo)[vn];
                  if ((*jpo)[vn].isValid()) {
                    vn = subtype2;
                    jpo = &jobj2;
                    str_value = (*jpo)[vn].getStr();
                    if ((*jpo)[vn].isValid()) {
                      goto skip;
                    } else {
                      goto chknext;
                    }
                  } else {
                    goto chknext;
                  }
                }
                // end
                goto skip;
              }
            } else {
              goto chknext;
            }
          }
          skip:
          if (ja) {
            // json array
            str_value = (*jpo)[vn].getArray()[aindex].getStr();
          }
          if (str_value && *str_value) {
            if ((*jpo)[vn].isStr()) {
              if (!strncmp_XP(str_value, XPSTR("ON"), 2)) {
                if (fp) *fp = 1;
                goto nexit;
              } else if (!strncmp_XP(str_value, XPSTR("OFF"), 3)) {
                if (fp) *fp = 0;
                goto nexit;
              } else {
                *vtype = STR_RES;
                tind->bits.constant = 1;
                tind->bits.is_string = 1;
                if (sp) strlcpy(sp, str_value, SCRIPT_MAXSSIZE);
                return lp + len;
              }

            } else {
              if (fp) {
                if (!strncmp_XP(vn.c_str(), XPSTR("Epoch"), 5)) {
                  *fp = atoi(str_value) - (uint32_t)glob_script_mem.epoch_offset;
                } else {
                  *fp = CharToFloat((char*)str_value);
                }
              }
              nexit:
              *vtype = NUM_RES;
              tind->bits.constant = 1;
              tind->bits.is_string = 0;
              return lp + len;
            }
          }
        }
      }
#endif

    }

#endif


chknext:

    switch (vname[0]) {

      case 'a':
#ifdef USE_ANGLE_FUNC
        if (!strncmp_XP(lp, XPSTR("acos("), 5)) {
          lp = GetNumericArgument(lp + 5, OPER_EQU, &fvar, gv);
          fvar = acosf(fvar);
          goto nfuncexit;
        }
#endif
        if (!strncmp_XP(lp, XPSTR("abs("), 4)) {
          lp = GetNumericArgument(lp + 4, OPER_EQU, &fvar, gv);
          fvar = fabs(fvar);
          goto nfuncexit;
        }

        if (!strncmp_XP(lp, XPSTR("asc("), 4)) {
          char str[SCRIPT_MAXSSIZE];
          lp = GetStringArgument(lp + 4, OPER_EQU, str, gv);
          fvar = str[0];
          goto nfuncexit;
        }
        if (!strncmp_XP(lp, XPSTR("adc("), 4)) {
          lp = GetNumericArgument(lp + 4, OPER_EQU, &fvar, gv);
          while (*lp==' ') lp++;
          TS_FLOAT pin = 1;
          if (*lp!=')') {
            lp = GetNumericArgument(lp, OPER_EQU, &pin, gv);
#ifdef CONFIG_IDF_TARGET_ESP32S3
            if (pin<1 || pin>20) pin = 1;
#else
            if (pin<32 || pin>39) pin = 32;
#endif
          }
          lp++;
          if (fvar > 7) fvar = 7;
#ifdef ESP32
          // ESP32
#ifdef USE_ADC
          fvar = AdcRead(pin, fvar);
#else
          fvar = 999.999;
#endif // USE_ADC
#else
          // ESP8266
#ifndef USE_ADC_VCC
          fvar = AdcRead(17, fvar);
#else
          fvar = (TS_FLOAT)ESP.getVcc() / 1000.0;
#endif // USE_ADC_VCC
#endif // ESP32
          len = 0;
          goto exit;
        }

        if (!strncmp_XP(lp, XPSTR("acp("), 4)) {
          lp += 4;
          SCRIPT_SKIP_SPACES
          uint16_t alend;
          fvar = -1;
          TS_FLOAT *fpd;
          lp = get_array_by_name(lp, &fpd, &alend, 0);
          SCRIPT_SKIP_SPACES
          uint16_t alens;
          TS_FLOAT *fps;
          lp = get_array_by_name(lp, &fps, &alens, 0);
          SCRIPT_SKIP_SPACES
          if (alens < alend) {
            alend = alens;
          }
          memcpy(fpd, fps, alend * sizeof(TS_FLOAT));
          fvar = alend;
          goto nfuncexit;
        }

        if (!strncmp_XP(lp, XPSTR("as("), 3)) {
          uint16_t alen;
          TS_FLOAT *fa;
          lp = get_array_by_name(lp + 3, &fa, &alen, 0);
          if (!fa) {
            fvar = -1;
            goto exit;
          }
          script_sort_array(fa, alen);
          fvar = 0;
          goto nfuncexit;
        }

        if (!strncmp_XP(lp, XPSTR("af("), 3)) {
          // array to float
          uint16_t alend;
          TS_FLOAT *fpd;
          lp = get_array_by_name(lp + 3, &fpd, &alend, 0);
          SCRIPT_SKIP_SPACES
          if (*lp != ')') {
            lp = GetNumericArgument(lp, OPER_EQU, &fvar, 0);
            uint16_t offset = 0;
            offset = fvar;
            if (offset > (alend - 4)) {
              offset = alend - 4;
            }
            fpd += offset;
          }
          if (fpd) {
            uint8_t fbytes[4];
            fbytes[0] = *fpd++;
            fbytes[1] = *fpd++;
            fbytes[2] = *fpd++;
            fbytes[3] = *fpd++;
            fpd = (TS_FLOAT*)fbytes;
            fvar = *fpd;
          } else {
            fvar = 0;
          }
          goto nfuncexit;
        }
        if (!strncmp_XP(lp, XPSTR("ap("), 3)) {
          //TasmotaGlobal.restart_flag = 216;
          lp = GetNumericArgument(lp + 3, OPER_EQU, &fvar, gv);
          switch ((uint8_t)fvar) {
            case 0:
              { char ssid[SCRIPT_MAXSSIZE];
              lp = GetStringArgument(lp, OPER_EQU, ssid, 0);
              char pw[SCRIPT_MAXSSIZE];
              lp = GetStringArgument(lp, OPER_EQU, pw, 0);
              IPAddress local_IP(192,168,189,1);
              IPAddress gateway(192,168,189,1);
              IPAddress subnet(255,255,255,0);
              // Begin Access Point
              WiFi.softAPConfig(local_IP, gateway, subnet);
              fvar = WiFi.softAP(ssid, pw);
              }
              break;
            case 1:
              fvar = WiFi.softAPdisconnect(true);
              break;
            case 2:
              fvar = WiFi.disconnect(true);
              break;
          }

          //Web.state = HTTP_ADMIN;
          // 192.168.4.1
          // WiFi.softAPIP();

          goto nfuncexit;
        }
        break;

      case 'b':
        if (!strncmp_XP(vname, XPSTR("boot"), 4)) {
          if (TasmotaGlobal.rules_flag.system_boot) {
            TasmotaGlobal.rules_flag.system_boot = 0;
            fvar = 1;
          }
          goto exit;
        }
#ifdef USE_BUTTON_EVENT
        if (!strncmp_XP(lp, XPSTR("bt["), 3)) {
          // tasmota button state
          lp = GetNumericArgument(lp + 3, OPER_EQU, &fvar, gv);
          uint32_t index = fvar;
          if (index<1 || index>MAX_KEYS) index = 1;
          fvar = glob_script_mem.script_button[index - 1];
          glob_script_mem.script_button[index - 1] |= 0x80;
          goto nfuncexit;
        }
#endif //USE_BUTTON_EVENT
        if (!strncmp_XP(lp, XPSTR("bcd("), 4)) {
          lp = GetNumericArgument(lp + 4, OPER_EQU, &fvar, gv);
          uint32_t sel = fvar;
          while (*lp==' ') lp++;
          lp = GetNumericArgument(lp, OPER_EQU, &fvar, gv);
          fvar = script_bcd(sel, fvar);
          goto nfuncexit;
        }

#ifdef USE_FLASH_BDIR
        if (!strncmp_XP(lp, XPSTR("bdir("), 5)) {
          lp = GetNumericArgument(lp + 5, OPER_EQU, &fvar, gv);
          char str[SCRIPT_MAXSSIZE];
          if (fvar > 1) {
            lp = GetStringArgument(lp, OPER_EQU, str, 0);
          }
          fvar = flash_bindir(fvar, str);
          goto nfuncexit;
        }
#endif // USE_FLASH_BDIR
        break;
      case 'c':
        if (!strncmp_XP(lp, XPSTR("chg["), 4)) {
          // var changed
          struct T_INDEX ind;
          uint8_t vtype;
          lp = isvar(lp + 4, &vtype, &ind, 0, 0, gv);
          if (!ind.bits.constant) {
            uint8_t index = glob_script_mem.type[ind.index].index;
            fvar = glob_script_mem.fvars[index] != glob_script_mem.s_fvars[index];
            glob_script_mem.s_fvars[index] = glob_script_mem.fvars[index];
          } else {
            fvar = 0;
          }
          goto nfuncexit;
        }
#ifdef ESP32
        if (!strncmp_XP(vname, XPSTR("core"), 4)) {
          fvar = xPortGetCoreID();
          goto exit;
        }
#ifdef USE_M5STACK_CORE2
        if (!strncmp_XP(lp, XPSTR("c2ps("), 5)) {
          lp = GetNumericArgument(lp + 5, OPER_EQU, &fvar, gv);
          while (*lp==' ') lp++;
          TS_FLOAT fvar1;
          lp = GetNumericArgument(lp, OPER_EQU, &fvar1, gv);
          fvar = Core2SetAxpPin(fvar, fvar1);
          goto nfuncexit;
        }
#endif // USE_M5STACK_CORE2

#ifdef USE_SCRIPT_TASK
        if (!strncmp_XP(lp, XPSTR("ct("), 3)) {
          lp = GetNumericArgument(lp + 3, OPER_EQU, &fvar, gv);
          while (*lp==' ') lp++;
          TS_FLOAT fvar1;
          lp = GetNumericArgument(lp, OPER_EQU, &fvar1, gv);
          while (*lp==' ') lp++;
          TS_FLOAT fvar2;
          lp = GetNumericArgument(lp, OPER_EQU, &fvar2, gv);
          SCRIPT_SKIP_SPACES
          TS_FLOAT prio = STASK_PRIO;
          if (*lp!=')') {
            lp = GetNumericArgument(lp, OPER_EQU, &prio, gv);
          }
          SCRIPT_SKIP_SPACES
          TS_FLOAT stack = STASK_STACK;
          if (*lp!=')') {
            lp = GetNumericArgument(lp, OPER_EQU, &stack, gv);
          }
          fvar = scripter_create_task(fvar, fvar1, fvar2, prio, stack);
          goto nfuncexit;
        }
#endif //USE_SCRIPT_TASK
#endif //ESP32

#ifdef USE_ANGLE_FUNC
        if (!strncmp_XP(lp, XPSTR("cos("), 4)) {
          lp = GetNumericArgument(lp + 4, OPER_EQU, &fvar, gv);
          fvar = cosf(fvar);
          goto nfuncexit;
        }
        if (!strncmp_XP(lp, XPSTR("ceil("), 5)) {
          lp = GetNumericArgument(lp + 5, OPER_EQU, &fvar, gv);
          fvar = ceilf(fvar);
          goto nfuncexit;
        }
#endif
#ifdef USE_FEXTRACT
        if (!strncmp_XP(lp, XPSTR("cts("), 4)) {
          char tsin[SCRIPT_MAXSSIZE];
          lp = GetStringArgument(lp + 4, OPER_EQU, tsin, 0);
          SCRIPT_SKIP_SPACES
          int8_t flg = -1;
          if (*lp != ')') {
            lp = GetNumericArgument(lp, OPER_EQU, &fvar, gv);
            flg = fvar;
          }
          char tsout[SCRIPT_MAXSSIZE];
          cnvts(tsout, tsin, flg);
          if (sp) strlcpy(sp, tsout, glob_script_mem.max_ssize);
          lp++;
          len = 0;
          goto strexit;
        }
#endif // USE_FEXTRACT
        if (!strncmp_XP(lp, XPSTR("cbs"), 3)) {
          fvar = glob_script_mem.cmdbuffer_size;
          tind->index = SCRIPT_CBSIZE;
          goto exit_settable;
        }
#ifdef USE_W8960
extern void W8960_SetGain(uint8_t sel, uint16_t value);

        if (!strncmp_XP(lp, XPSTR("codec("), 6)) {
          TS_FLOAT sel;
          lp = GetNumericArgument(lp + 6, OPER_EQU, &sel, gv);
          lp = GetNumericArgument(lp, OPER_EQU, &fvar, gv);
          W8960_SetGain(sel, fvar);
          fvar = 0;
          goto nfuncexit;
        }
#endif

#ifdef USE_UFILESYS
        if (!strncmp_XP(lp, XPSTR("cpf("), 4)) {
          // copy file with offsets sfd, sfstart, sfstop, df
          TS_FLOAT sfd, sf_from, sf_to, dfd;
          lp = GetNumericArgument(lp + 4, OPER_EQU, &sfd, 0);
          lp = GetNumericArgument(lp, OPER_EQU, &sf_from, 0);
          lp = GetNumericArgument(lp, OPER_EQU, &sf_to, 0);
          lp = GetNumericArgument(lp, OPER_EQU, &dfd, 0);
          if (*lp != ')') {
            lp = GetNumericArgument(lp, OPER_EQU, &fvar, 0);
          } else {
            fvar = 0;
          }
          uint8_t source = sfd;
          uint8_t dest = dfd;

          if (!glob_script_mem.file_flags[source].is_open) {
            fvar -1;
            goto nfuncexit;
          }
          if (!glob_script_mem.file_flags[dest].is_open) {
            fvar -2;
            goto nfuncexit;
          }
          fvar = script_copy_file(&glob_script_mem.files[source], &glob_script_mem.files[dest], sf_from, sf_to, fvar, 0);
          glob_script_mem.files[source].close();
          glob_script_mem.file_flags[source].is_open = 0;
          glob_script_mem.files[dest].close();
          glob_script_mem.file_flags[dest].is_open = 0;
          goto nfuncexit;
        }
#endif
        if (!strncmp_XP(vname, XPSTR("ctper"), 5)) {
          fvar = TasmotaGlobal.tele_period;
          goto exit;
        }
        break;
      case 'd':
        if (!strncmp_XP(vname, XPSTR("day"), 3)) {
          fvar = RtcTime.day_of_month;
          goto exit;
        }
        if (!strncmp_XP(vname, XPSTR("dvnm"), 4)) {
          if (sp) strlcpy(sp, SettingsText(SET_DEVICENAME), glob_script_mem.max_ssize);
          goto strexit;
        }
        if (!strncmp_XP(lp, XPSTR("dp("), 3)) {
          lp = GetNumericArgument(lp + 3, OPER_EQU, &fvar, gv);
          while (*lp == ' ') lp++;
          glob_script_mem.script_lzero = fvar;
          if (*lp == ',' || *lp == '.') {
            glob_script_mem.script_sepc = *lp;
            lp++;
          }
          lp = GetNumericArgument(lp , OPER_EQU, &fvar, gv);
          while (*lp == ' ') lp++;
          glob_script_mem.script_dprec = fvar;
          fvar = 0;
          goto nfuncexit;
        }
        if (!strncmp_XP(lp, XPSTR("diff["), 5)) {
          struct T_INDEX ind;
          uint8_t vtype;
          lp = isvar(lp + 5, &vtype, &ind, 0, 0, gv);
          if (!ind.bits.constant) {
            uint8_t index = glob_script_mem.type[ind.index].index;
            fvar = glob_script_mem.fvars[index] - glob_script_mem.s_fvars[index];
            glob_script_mem.s_fvars[index] = glob_script_mem.fvars[index];
          } else {
            fvar = 0;
          }
          goto nfuncexit;
        }
        break;
      case 'e':
        if (!strncmp_XP(vname, XPSTR("epoch"), 5)) {
          fvar = UtcTime() - (uint32_t)glob_script_mem.epoch_offset;
          goto exit;
        }
        if (!strncmp_XP(vname, XPSTR("epoffs"), 6)) {
          fvar = (uint32_t)glob_script_mem.epoch_offset;
          tind->index = SCRIPT_EPOFFS;
          goto exit_settable;
        }
        if (!strncmp_XP(vname, XPSTR("eres"), 4)) {
          fvar = glob_script_mem.event_handeled;
          tind->index = SCRIPT_EVENT_HANDLED;
          goto exit_settable;
        }
#ifdef USE_ENERGY_SENSOR
        if (!strncmp_XP(lp, XPSTR("enrg["), 5)) {
          lp = GetNumericArgument(lp + 5, OPER_EQU, &fvar, gv);
          while (*lp == ' ') lp++;
          switch ((uint32_t)fvar) {
            case 0:
              fvar = Energy->total_sum;
              break;
            case 1:
              fvar = Energy->voltage[0];
              break;
            case 2:
              fvar = Energy->voltage[1];
              break;
            case 3:
              fvar = Energy->voltage[2];
              break;
            case 4:
              fvar = Energy->current[0];
              break;
            case 5:
              fvar = Energy->current[1];
              break;
            case 6:
              fvar = Energy->current[2];
              break;
            case 7:
              fvar = Energy->active_power[0];
              break;
            case 8:
              fvar = Energy->active_power[1];
              break;
            case 9:
              fvar = Energy->active_power[2];
              break;
            case 10:
              fvar = Energy->start_energy[0];
              break;
            case 11:
              fvar = Energy->daily_sum;
              break;
            case 12:
              fvar = Energy->yesterday_sum;
              break;

            default:
              fvar = 99999;
              break;
          }
          goto nfuncexit;
        }
#endif //USE_ENERGY_SENSOR
        break;
      case 'f':
//#define DEBUG_FS
#ifdef USE_SCRIPT_FATFS
        if (!strncmp_XP(lp, XPSTR("fo("), 3)) {
          char str[SCRIPT_MAXSSIZE];
          lp = GetStringArgument(lp + 3, OPER_EQU, str, 0);
          while (*lp == ' ') lp++;
          uint8_t mode = 0;
          if ((*lp == 'r') || (*lp == 'w') || (*lp == 'a')) {
            switch (*lp) {
              case 'r':
                mode = 0;
                break;
              case 'w':
                mode = 1;
                break;
              case 'a':
                mode = 2;
                break;
            }
            lp++;
          } else {
            lp = GetNumericArgument(lp, OPER_EQU, &fvar, gv);
            mode = fvar;
          }
          fvar = -1;
          for (uint8_t cnt = 0;cnt<SFS_MAX;cnt++) {
            if (!glob_script_mem.file_flags[cnt].is_open) {
              if (mode == 0) {
#ifdef DEBUG_FS
                AddLog(LOG_LEVEL_INFO, PSTR("SCR: open file for read %d"), cnt);
#endif
                glob_script_mem.files[cnt] = ufsp->open(str, FS_FILE_READ);
                if (glob_script_mem.files[cnt].isDirectory()) {
                  glob_script_mem.files[cnt].rewindDirectory();
                  glob_script_mem.file_flags[cnt].is_dir = 1;
                } else {
                  glob_script_mem.file_flags[cnt].is_dir = 0;
                }
              }
              else {
                if (mode == 1) {
                  glob_script_mem.files[cnt] = ufsp->open(str,FS_FILE_WRITE);
#ifdef DEBUG_FS
                  AddLog(LOG_LEVEL_INFO, PSTR("SCR: open file for write %d"), cnt);
#endif
                } else {
                  glob_script_mem.files[cnt] = ufsp->open(str,FS_FILE_APPEND);
#ifdef DEBUG_FS
                  AddLog(LOG_LEVEL_INFO, PSTR("SCR: open file for append %d"), cnt);
#endif
                }
              }
              if (glob_script_mem.files[cnt]) {
                fvar = cnt;
                glob_script_mem.file_flags[cnt].is_open = 1;
              } else {
                AddLog(LOG_LEVEL_INFO, PSTR("SCR: file open failed"));
              }
              break;
            }
          }
          goto nfuncexit;
        }
        if (!strncmp_XP(lp, XPSTR("fc("), 3)) {
          lp = GetNumericArgument(lp + 3, OPER_EQU, &fvar, gv);
          if (fvar >= 0) {
            uint8_t ind = fvar;
            if (ind >= SFS_MAX) ind = SFS_MAX - 1;
#ifdef DEBUG_FS
            AddLog(LOG_LEVEL_INFO, PSTR("SCR: closing file %d"), ind);
#endif
            glob_script_mem.files[ind].close();
            glob_script_mem.file_flags[ind].is_open = 0;
          }
          fvar = 0;
          goto nfuncexit;
        }
        if (!strncmp_XP(lp, XPSTR("ff("), 3)) {
          lp = GetNumericArgument(lp + 3, OPER_EQU, &fvar, gv);
          uint8_t ind = fvar;
          if (ind >= SFS_MAX) ind = SFS_MAX - 1;
          glob_script_mem.files[ind].flush();
          fvar = 0;
          goto nfuncexit;
        }
        if (!strncmp_XP(lp, XPSTR("fw("), 3)) {
          char str[SCRIPT_MAXSSIZE];
          lp = ForceStringVar(lp + 3, str);
          while (*lp == ' ') lp++;
          lp = GetNumericArgument(lp, OPER_EQU, &fvar, gv);
          uint8_t ind = fvar;
          if (ind >= SFS_MAX) ind = SFS_MAX - 1;
          if (glob_script_mem.file_flags[ind].is_open) {
            fvar = glob_script_mem.files[ind].print(str);
          } else {
            fvar = 0;
          }
          goto nfuncexit;
        }
        if (!strncmp_XP(lp, XPSTR("fwb("), 4)) {
          lp = GetNumericArgument(lp + 4, OPER_EQU, &fvar, gv);
          uint8_t buf[2];
          buf[0] = fvar;
          SCRIPT_SKIP_SPACES
          lp = GetNumericArgument(lp, OPER_EQU, &fvar, gv);
          uint8_t ind = fvar;
          if (ind >= SFS_MAX) ind = SFS_MAX - 1;
          if (glob_script_mem.file_flags[ind].is_open) {
            fvar = glob_script_mem.files[ind].write(buf, 1);
          } else {
            fvar = 0;
          }
          goto nfuncexit;
        }

        if (!strncmp_XP(lp, XPSTR("fr("), 3)) {
          struct T_INDEX ind;
          uint8_t vtype;
          uint8_t sindex = 0;
          lp = isvar(lp + 3, &vtype, &ind, 0, 0, gv);
          if (vtype != VAR_NV) {
            // found variable as result
            if ((vtype & STYPE) == 0) {
                  // error
                  fvar = 0;
                  goto exit;
            } else {
              // string result
              sindex = glob_script_mem.type[ind.index].index;
            }
          } else {
              // error
              fvar = 0;
              goto exit;
          }
          while (*lp == ' ') lp++;
          lp = GetNumericArgument(lp, OPER_EQU, &fvar, gv);
          uint8_t find = fvar;
          if (find >= SFS_MAX) find = SFS_MAX - 1;
          while (*lp == ' ') lp++;
          uint8_t options = 0;
          if (*lp != ')') {
              // options
             lp = GetNumericArgument(lp, OPER_EQU, &fvar, gv);
             options = fvar;
          }
          uint8_t index = 0;
          char str[SCRIPT_MAXSSIZE];
          char *cp = str;
          if (glob_script_mem.file_flags[find].is_open) {
            if (glob_script_mem.file_flags[find].is_dir) {
              while (true) {
                File entry = glob_script_mem.files[find].openNextFile();
                if (entry) {
                  if (!UfsReject((char*)entry.name())) {
                    char *ep = (char*)entry.name();
                    if (*ep == '/') ep++;
                    char *lcp = strrchr(ep,'/');
                    if (lcp) {
                      ep = lcp + 1;
                    }
                    strcpy(str, ep);
                    entry.close();
                    break;
                  }
                } else {
                  *cp = 0;
                  break;
                }
                entry.close();
              }
              index = strlen(str);
            } else {
              while (glob_script_mem.files[find].available()) {
                uint8_t buf[1];
                glob_script_mem.files[find].read(buf,1);
                if (!options && (buf[0] == '\t' || buf[0] == ',' || buf[0] == '\n' || buf[0] == '\r')) {
                  break;
                } else {
                  if (options && (buf[0] == '\n' || buf[0] == '\r')) {
                    break;
                  }
                  *cp++ = buf[0];
                  index++;
                  if (index >= glob_script_mem.max_ssize - 1) break;
                }
              }
              *cp = 0;
            }
          } else {
            strcpy(str, "file error");
          }
          lp++;
          strlcpy(glob_script_mem.glob_snp + (sindex * glob_script_mem.max_ssize), str, glob_script_mem.max_ssize);
          fvar = index;
          len = 0;
          goto exit;
        }
        if (!strncmp_XP(lp, XPSTR("frb("), 4)) {
          lp = GetNumericArgument(lp, OPER_EQU, &fvar, gv);
          uint8_t ind = fvar;
          if (ind >= SFS_MAX) ind = SFS_MAX - 1;
          if (glob_script_mem.file_flags[ind].is_open) {
            uint8_t buf[2];
            buf[0] = 0;
            glob_script_mem.files[ind].read(buf, 1);
            fvar = buf[0];
          } else {
            fvar = 0;
          }
          goto nfuncexit;
        }
        if (!strncmp_XP(lp, XPSTR("fa("), 3)) {
          lp = GetNumericArgument(lp + 3, OPER_EQU, &fvar, gv);
          uint8_t ind = fvar;
          if (ind >= SFS_MAX) ind = SFS_MAX - 1;
          if (glob_script_mem.file_flags[ind].is_open) {
            fvar = glob_script_mem.files[ind].available();
          } else {
            fvar = -1;
          }
          goto nfuncexit;
        }
        if (!strncmp_XP(lp, XPSTR("fs("), 3)) {
          lp = GetNumericArgument(lp + 3, OPER_EQU, &fvar, gv);
          uint8_t ind = fvar;
          SCRIPT_SKIP_SPACES
          lp = GetNumericArgument(lp, OPER_EQU, &fvar, gv);
          SCRIPT_SKIP_SPACES
          if (ind >= SFS_MAX) ind = SFS_MAX - 1;
          if (glob_script_mem.file_flags[ind].is_open) {
            fvar = glob_script_mem.files[ind].seek(fvar, SeekSet);
          } else {
            fvar = -1;
          }
          goto nfuncexit;
        }
        if (!strncmp_XP(lp, XPSTR("fz("), 3)) {
          lp = GetNumericArgument(lp + 3, OPER_EQU, &fvar, gv);
          SCRIPT_SKIP_SPACES
          uint8_t ind = fvar;
          if (ind >= SFS_MAX) ind = SFS_MAX - 1;
          if (glob_script_mem.file_flags[ind].is_open) {
            fvar = glob_script_mem.files[ind].size();
          } else {
            fvar = -1;
          }
          goto nfuncexit;
        }
        if (!strncmp_XP(lp, XPSTR("fd("), 3)) {
          char str[SCRIPT_MAXSSIZE];
          lp = GetStringArgument(lp + 3, OPER_EQU, str, 0);
          ufsp->remove(str);
          goto nfuncexit;
        }
#ifdef USE_UFILESYS
        if (!strncmp_XP(lp, XPSTR("frw("), 4)) {
          // read file from web
          lp = GetNumericArgument(lp + 4, OPER_EQU, &fvar, gv);
          SCRIPT_SKIP_SPACES
          char url[SCRIPT_MAXSSIZE];
          lp = ForceStringVar(lp, url);
          SCRIPT_SKIP_SPACES
          fvar = url2file(fvar, url);
          goto nfuncexit;
        }
#endif
#if defined(ESP32) && defined(USE_WEBCAM)
        if (!strncmp_XP(lp, XPSTR("fwp("), 4)) {
          lp = GetNumericArgument(lp + 4, OPER_EQU, &fvar, gv);
          while (*lp == ' ') lp++;
          TS_FLOAT fvar1;
          lp = GetNumericArgument(lp, OPER_EQU, &fvar1, gv);
          uint8_t ind = fvar1;
          if (ind >= SFS_MAX) ind = SFS_MAX - 1;
          if (glob_script_mem.file_flags[ind].is_open) {
            uint8_t *buff;
            TS_FLOAT maxps = WcGetPicstore(-1, 0);
            if (fvar < 1 || fvar > maxps) fvar = 1;
            uint32_t len = WcGetPicstore(fvar - 1, &buff);
            if (len) {
              //glob_script_mem.files[ind].seek(0,SeekEnd);
              fvar = glob_script_mem.files[ind].write(buff, len);
            } else {
              fvar = 0;
            }
            //AddLog(LOG_LEVEL_INFO, PSTR("picture save: %d"), len);
          } else {
            fvar = 0;
          }
          goto nfuncexit;
        }
#endif //ESP32 && USE_WEBCAM
#ifdef USE_SCRIPT_FATFS_EXT
        if (!strncmp_XP(lp, XPSTR("fe("), 3)) {
          char str[SCRIPT_MAXSSIZE];
          lp = GetStringArgument(lp + 3, OPER_EQU, str, 0);
          // execute script
          File ef = ufsp->open(str, FS_FILE_READ);
          if (ef) {
            uint16_t fsiz = ef.size();
            if (fsiz < 2048) {
              char *script = (char*)special_malloc(fsiz + 16);
              if (script) {
                memset(script, 0, fsiz + 16);
                ef.read((uint8_t*)script, fsiz);
                execute_script(script);
                free(script);
                fvar = 1;
              }
            }
            ef.close();
          }
          goto nfuncexit;
        }
        if (!strncmp_XP(lp, XPSTR("fmd("), 4)) {
          char str[SCRIPT_MAXSSIZE];
          lp = GetStringArgument(lp + 4, OPER_EQU, str, 0);
          fvar = ufsp->mkdir(str);
          goto nfuncexit;
        }
        if (!strncmp_XP(lp, XPSTR("fmt("), 4)) {
          lp = GetNumericArgument(lp + 4, OPER_EQU, &fvar, gv);
          if (!fvar) {
            LittleFS.format();
          } else {
            //SD.format();
          }
          goto nfuncexit;
        }
        if (!strncmp_XP(lp, XPSTR("frd("), 4)) {
          char str[SCRIPT_MAXSSIZE];
          lp = GetStringArgument(lp + 4, OPER_EQU, str, 0);
          fvar = ufsp->rmdir(str);
          goto nfuncexit;
        }
        if (!strncmp_XP(lp, XPSTR("fx("), 3)) {
          char str[SCRIPT_MAXSSIZE];
          lp = GetStringArgument(lp + 3, OPER_EQU, str, 0);
          if (ufsp->exists(str)) fvar = 1;
          else fvar = 0;
          goto nfuncexit;
        }

        if (!strncmp_XP(lp, XPSTR("fsi("), 4)) {
          lp = GetNumericArgument(lp + 4, OPER_EQU, &fvar, gv);
          fvar = UfsInfo(fvar, 0);
          goto nfuncexit;
        }
        if (!strncmp_XP(lp, XPSTR("frn("), 4)) {
          // rename a file
          char fn_from[SCRIPT_MAXSSIZE];
          lp = GetStringArgument(lp + 4, OPER_EQU, fn_from, 0);
          SCRIPT_SKIP_SPACES

          char fn_to[SCRIPT_MAXSSIZE];
          lp = GetStringArgument(lp, OPER_EQU, fn_to, 0);
          SCRIPT_SKIP_SPACES

          fvar = ufsp->rename(fn_from, fn_to);
          goto nfuncexit;
        }

#ifdef USE_FEXTRACT
        if (!strncmp_XP(lp, XPSTR("fxt"), 3)) {
          lp += 3;
          uint8_t oflg = 0;
          if (*lp == 'o') {
            oflg = 1;
            lp++;
          }
          if (*lp == '(') {
            lp++;
          } else {
            break;
          }
          // extract from file
          lp = GetNumericArgument(lp, OPER_EQU, &fvar, gv);
          SCRIPT_SKIP_SPACES
          uint8_t fref = fvar;

          //2020-12-16T14:30:00
          char ts_from[24];
          lp = GetStringArgument(lp, OPER_EQU, ts_from, 0);
          SCRIPT_SKIP_SPACES

          char ts_to[24];
          lp = GetStringArgument(lp, OPER_EQU, ts_to, 0);
          SCRIPT_SKIP_SPACES

          lp = GetNumericArgument(lp, OPER_EQU, &fvar, gv);
          SCRIPT_SKIP_SPACES
          int8_t coffs = fvar;
          if (coffs >= 0) {
            lp = GetNumericArgument(lp, OPER_EQU, &fvar, gv);
            SCRIPT_SKIP_SPACES
            int16_t accum = fvar;

            uint16_t a_len[MAX_EXT_ARRAYS];
            TS_FLOAT *a_ptr[MAX_EXT_ARRAYS];

            uint8_t index = 0;
            while (index < MAX_EXT_ARRAYS) {
              lp = get_array_by_name(lp, &a_ptr[index], &a_len[index], 0);
              SCRIPT_SKIP_SPACES
              index++;
              if (*lp == ')' || *lp == '\n') {
                break;
              }
            }
            if (!glob_script_mem.file_flags[fref].is_open) {
              fvar = -1;
              goto nfuncexit;
            }
            if (oflg) {
              // optimized access
              int32_t fres = opt_fext(&glob_script_mem.files[fref],  ts_from, ts_to, 1);
              //AddLog(LOG_LEVEL_INFO,PSTR(">>> 2 %s - %d - %d"), ts_from, fres, (uint32_t)(perc*100));
              if (fres > 0) {
                fvar = extract_from_file(&glob_script_mem.files[fref],  ts_from, ts_to, coffs, a_ptr, a_len, index, accum);
              } else {
                // fatal error time stamp out of range
                fvar = -2;
              }
            } else {
              fvar = extract_from_file(&glob_script_mem.files[fref],  ts_from, ts_to, coffs, a_ptr, a_len, index, accum);
            }
          } else {
            if (oflg) {
              fvar = opt_fext(&glob_script_mem.files[fref],  ts_from, ts_to, 0);
              if (coffs == -4) {
                goto nfuncexit;
              }
            }
            fvar = extract_from_file(&glob_script_mem.files[fref],  ts_from, ts_to, coffs, 0, 0, 0, 0);
          }

          goto nfuncexit;
        }
#endif // USE_FEXTRACT
        if (!strncmp_XP(lp, XPSTR("fwa("), 4)) {
          uint16_t alen;
          TS_FLOAT *fa;
          lp = get_array_by_name(lp + 4, &fa, &alen, 0);
          if (!fa) {
            fvar = 0;
            goto exit;
          }
          SCRIPT_SKIP_SPACES
          lp = GetNumericArgument(lp, OPER_EQU, &fvar, gv);
          SCRIPT_SKIP_SPACES
          uint8_t append = 0;
          if (*lp == 'a') {
            lp++;
            append = 1;
          }
          uint8_t index = fvar;
          if (index >= SFS_MAX) index = SFS_MAX - 1;
          if (glob_script_mem.file_flags[index].is_open) {
            char dstr[24];
            for (uint32_t cnt = 0; cnt < alen; cnt++) {
              //dtostrfd(*fa, glob_script_mem.script_dprec, dstr);
              ext_snprintf_P(dstr, sizeof(dstr), PSTR("%*_f"), -glob_script_mem.script_dprec, fa);
              fa++;
              if (cnt < (alen - 1)) {
                strcat(dstr,"\t");
              } else {
                if (!append) {
                  strcat(dstr,"\n");
                } else {
                  strcat(dstr,"\t");
                }
              }
              glob_script_mem.files[index].print(dstr);
            }

          } else {
            fvar = 0;
          }
          goto nfuncexit;
        }
        if (!strncmp_XP(lp, XPSTR("fra("), 4)) {
          uint16_t alen;
          TS_FLOAT *fa;
          lp = get_array_by_name(lp + 4, &fa, &alen, 0);
          if (!fa) {
            fvar = 0;
            goto exit;
          }
          SCRIPT_SKIP_SPACES
          lp = GetNumericArgument(lp, OPER_EQU, &fvar, gv);
          uint8_t find = fvar;
          SCRIPT_SKIP_SPACES

          if (find >= SFS_MAX) find = SFS_MAX - 1;
          char str[SCRIPT_MAXSSIZE];
          if (glob_script_mem.file_flags[find].is_open) {
            uint8_t first = 0;
            for (uint32_t cnt = 0; cnt < alen; cnt++) {
              uint8_t slen = 0;
              char *cp = str;
              *cp = 0;
              while (glob_script_mem.files[find].available()) {
                uint8_t buf[1];
                glob_script_mem.files[find].read(buf,1);
                if (buf[0] == '\t' || buf[0] == ',' || buf[0] == '\n' || buf[0] == '\r') {
                  // skip leading TAB
                  if (first) {
                    break;
                  }
                } else {
                  *cp++ = buf[0];
                  first = 1;
                  slen++;
                  if (slen >= glob_script_mem.max_ssize - 1) break;
                }
              }
              *cp = 0;
              *fa++=CharToFloat(str);
            }
          } else {
            fvar = 0;
          }
          goto nfuncexit;
        }

#endif // USE_SCRIPT_FATFS_EXT
        if (!strncmp_XP(lp, XPSTR("fl1("), 4) || !strncmp_XP(lp, XPSTR("fl2("), 4) )  {
          uint8_t lknum = *(lp + 2) & 3;
          char str[SCRIPT_MAXSSIZE];
          lp = GetStringArgument(lp + 4, OPER_EQU, str, 0);
          if (lknum < 1 || lknum > 2) lknum = 1;
          strlcpy(glob_script_mem.flink[lknum - 1], str, 14);
          fvar = 0;
          goto nfuncexit;
        }
        if (!strncmp_XP(lp, XPSTR("fsm"), 3)) {
          fvar = (uint32_t)ufsp;
          goto exit;
        }
#endif //USE_SCRIPT_FATFS
        if (!strncmp_XP(lp, XPSTR("freq"), 4)) {
#ifdef ESP32
          fvar = getCpuFrequencyMhz();
#else
          fvar = ESP.getCpuFreqMHz();
#endif
          goto exit;
        }
        if (!strncmp_XP(lp, XPSTR("frnm"), 4)) {
          if (sp) strlcpy(sp, SettingsText(SET_FRIENDLYNAME1), glob_script_mem.max_ssize);
          goto strexit;
        }
#ifdef USE_ANGLE_FUNC
        if (!strncmp_XP(lp, XPSTR("floor("), 6)) {
          lp = GetNumericArgument(lp + 6, OPER_EQU, &fvar, gv);
          fvar = floorf(fvar);
          goto nfuncexit;
        }
#endif

        if (!strncmp_XP(lp, XPSTR("f("), 2)) {
          // convert to float var
          lp = GetNumericArgument(lp + 2, OPER_EQU, &fvar, gv);
          fvar = *(uint32_t*)&fvar;
          goto nfuncexit;
        }
        break;

      case 'g':
        if (!strncmp_XP(vname, XPSTR("gtmp"), 4)) {
          fvar = TasmotaGlobal.temperature_celsius;
          goto exit;
        }
        if (!strncmp_XP(vname, XPSTR("ghum"), 4)) {
          fvar = TasmotaGlobal.humidity;
          goto exit;
        }
        if (!strncmp_XP(vname, XPSTR("gprs"), 4)) {
          fvar = TasmotaGlobal.pressure_hpa;
          goto exit;
        }
        if (!strncmp_XP(vname, XPSTR("gtopic"), 6)) {
          if (sp) strlcpy(sp, SettingsText(SET_MQTT_GRP_TOPIC), glob_script_mem.max_ssize);
          goto strexit;
        }
#ifdef USE_WEBSEND_RESPONSE
        if (!strncmp_XP(lp, XPSTR("gwr("), 4)) {
          char delim[SCRIPT_MAXSSIZE];
          lp = GetStringArgument(lp + 4, OPER_EQU, delim, 0);
          SCRIPT_SKIP_SPACES
          lp = GetNumericArgument(lp, OPER_EQU, &fvar, gv);
          SCRIPT_SKIP_SPACES
          char delimc = 0;
          if (*lp != ')') {
            // get delimiter
            delimc = *lp;
            lp++;
          }

          char rstring[SCRIPT_MAXSSIZE];
          rstring[0] = 0;
          int8_t index = fvar;
          char *wd = ResponseData();

          strlcpy(rstring, wd, glob_script_mem.max_ssize);
          if (index) {
            if (strlen(wd) && index) {
              if (index < 0) {
                // assume val=xxx
                rstring[0] = 0;
                char *cp = strstr(wd, delim);
                if (cp) {
                    cp = strchr(cp, '=');
                    if (cp) {
                      cp++;
                      for (uint32_t cnt = 0; cnt < glob_script_mem.max_ssize; cnt++) {
                        if (*cp == ',' || *cp == ':' || *cp == 0) {
                          rstring[cnt] = 0;
                          break;
                        }
                        rstring[cnt] = *cp++;
                      }
                    }
                }
              } else {
                // preserve mqtt_data
                char *mqd = (char*)malloc(ResponseSize() + 2);
                if (mqd) {
                  strlcpy(mqd, ResponseData(), ResponseSize());
                  wd = mqd;
                  char *lwd = wd;
                  while (index) {
                    char *cp = strstr(wd, delim);
                    if (cp) {
                      index--;
                      if (!index) {
                        // take this substring
                        *cp = 0;
                        strlcpy(rstring, lwd, glob_script_mem.max_ssize);
                      } else {
                        wd = cp + strlen(delim);
                        lwd = wd;
                      }
                    } else {
                      // fail or last string
                      strlcpy(rstring, lwd, glob_script_mem.max_ssize);
                      break;
                    }
                  }
                  if (delimc) {
                    char *xp = strchr(rstring, delimc);
                     if (xp) {
                      *xp = 0;              
                    }
                  }
                  free(mqd);
                }
              }
            }
          }
          if (sp) strlcpy(sp, rstring, glob_script_mem.max_ssize);
          lp++;
          len = 0;
          goto strexit;
        }
#endif

#ifdef SCRIPT_GET_HTTPS_JP
        if (!strncmp_XP(lp, XPSTR("gjp("), 4)) {
          char host[SCRIPT_MAXSSIZE];
          lp = GetStringArgument(lp + 4, OPER_EQU, host, 0);
          SCRIPT_SKIP_SPACES
          char path[SCRIPT_MAXSSIZE];
          lp = GetStringArgument(lp, OPER_EQU, path, 0);
          fvar = call2https(host, path);
          goto nfuncexit;
        }
#endif //SCRIPT_GET_HTTPS_JP

#ifdef TESLA_POWERWALL
        if (!strncmp_XP(lp, XPSTR("gpwl("), 5)) {
          char path[SCRIPT_MAXSSIZE];
          lp = GetStringArgument(lp + 5, OPER_EQU, path, 0);
          fvar = call2pwl(path);
          goto nfuncexit;
        }
#endif

        if (!strncmp_XP(lp, XPSTR("gi("), 3)) {
          lp += 3;
          if (!strncmp_XP(lp, XPSTR("epoch"), 5)) {
            lp += 5;
            *(uint32_t*)&fvar = UtcTime();
          } else if (*lp == '0' && *(lp + 1) == 'x') {
            lp += 2;
            *(uint32_t*)&fvar = strtoll(lp, &lp, 16);
          } else {
            *(int32_t*)&fvar = strtol(lp, &lp, 10);
          }
          goto nfuncexit;
        }
        break;
      case 'h':
        if (!strncmp_XP(vname, XPSTR("hours"), 5)) {
          fvar = RtcTime.hour;
          goto exit;
        }
        if (!strncmp_XP(vname, XPSTR("heap"), 4)) {
          fvar = ESP_getFreeHeap();
          goto exit;
        }
        if (!strncmp_XP(lp, XPSTR("hni("), 4)) {
          lp = GetNumericArgument(lp + 4, OPER_EQU, &fvar, gv);
          uint8_t iob = *(uint32_t*)&fvar;
          lp++;
          len = 0;
          if (sp) {
            sprintf(sp, "%02x", iob);
          }
          goto strexit;
        }
        if (!strncmp_XP(lp, XPSTR("hn("), 3)) {
          lp = GetNumericArgument(lp + 3, OPER_EQU, &fvar, gv);
          if (fvar < 0 || fvar > 255) fvar = 0;
          lp++;
          len = 0;
          if (sp) {
            sprintf(sp, "%02x", (uint8_t)fvar);
          }
          goto strexit;
        }
        if (!strncmp_XP(lp, XPSTR("hxi("), 4)) {
          lp = GetNumericArgument(lp + 4, OPER_EQU, &fvar, gv);
          lp++;
          len = 0;
          if (sp) {
            sprintf(sp, "%08x", *(uint32_t*)&fvar);
          }
          goto strexit;
        }
        if (!strncmp_XP(lp, XPSTR("hx("), 3)) {
          lp = GetNumericArgument(lp + 3, OPER_EQU, &fvar, gv);
          lp++;
          len = 0;
          if (sp) {
            sprintf(sp, "%08x", (uint32_t)fvar);
          }
          goto strexit;
        }
        if (!strncmp_XP(lp, XPSTR("hd("), 3)) {
          char str[SCRIPT_MAXSSIZE];
          lp = GetStringArgument(lp + 3, OPER_EQU, str, 0);
          fvar = strtol(str, NULL, 16);
          goto nfuncexit;
        }
        if (!strncmp_XP(lp, XPSTR("hf("), 3)) {
          char str[SCRIPT_MAXSSIZE];
          lp = GetStringArgument(lp + 3, OPER_EQU, str, 0);
          SCRIPT_SKIP_SPACES
          if (strlen(str) != 8) {
            fvar = -1;
          } else {
            uint8_t *ucp = (uint8_t*)&fvar;
            uint8_t rflg = 0;
            if (*lp == 'r') {
              rflg = 1;
              ucp += sizeof(TS_FLOAT);
              lp++;
            }
            char substr[3];
            char *cp = str;
            for (uint32_t cnt = 0; cnt < 4; cnt++) {
              substr[0] = *cp++;
              substr[1] = *cp++;
              substr[2] = 0;
              if (!rflg) {
                *ucp++ = strtol(substr, NULL, 16);
              } else {
                *--ucp = strtol(substr, NULL, 16);
              }
            }
          }
          goto nfuncexit;
        }
        if (!strncmp_XP(lp, XPSTR("http("), 5)) {
          char host[SCRIPT_MAXSSIZE];
          lp = GetStringArgument(lp + 5, OPER_EQU, host, 0);
          SCRIPT_SKIP_SPACES
          char request[SCRIPT_MAXSSIZE];
          lp = GetStringArgument(lp, OPER_EQU, request, 0);
          SCRIPT_SKIP_SPACES
          fvar = http_req(host, request);
          goto nfuncexit;
        }
#ifdef USE_LIGHT
        if (!strncmp_XP(lp, XPSTR("hsvrgb("), 7)) {
          lp = GetNumericArgument(lp + 7, OPER_EQU, &fvar, gv);
          if (fvar < 0 || fvar > 360) fvar = 0;
          SCRIPT_SKIP_SPACES
          // arg2
          TS_FLOAT fvar2;
          lp = GetNumericArgument(lp, OPER_EQU, &fvar2, gv);
          if (fvar2 < 0 || fvar2 > 100) fvar2 = 0;
          SCRIPT_SKIP_SPACES
          // arg3
          TS_FLOAT fvar3;
          lp = GetNumericArgument(lp, OPER_EQU, &fvar3, gv);
          if (fvar3 < 0 || fvar3 > 100) fvar3 = 0;

          fvar = HSVToRGB(fvar, fvar2, fvar3);
          goto nfuncexit;
        }
#endif //USE_LIGHT

#ifdef USE_HOMEKIT
        if (!strncmp_XP(lp, XPSTR("hki("), 4)) {
          if (!TasmotaGlobal.global_state.wifi_down) {
            // erase nvs
            lp = GetNumericArgument(lp + 4, OPER_EQU, &fvar, gv);
            int32_t sel = fvar;
            fvar = homekit_main(0, sel);
            if (sel >= 98) {
              glob_script_mem.homekit_running == false;
            }
          }
          goto nfuncexit;
        }
#endif
        if (!strncmp_XP(lp, XPSTR("hstr("), 5)) {
          char hstr[SCRIPT_MAXSSIZE];
          lp = GetStringArgument(lp + 5, OPER_EQU, hstr, 0);
          uint16_t cnt;
          uint16_t slen = strlen(hstr);
          slen &= 0xfffe;
          for (cnt = 0; cnt < slen; cnt += 2) {
            hstr[cnt / 2] = (script_hexnibble(hstr[cnt]) << 4) | script_hexnibble(hstr[cnt + 1] );
          }
          hstr[cnt / 2 + 1] = 0;
          if (sp) strlcpy(sp, hstr, strlen(hstr));
          len = 0;
          lp++;
          goto strexit;
        }
        break;

      case 'i':
        if (!strncmp_XP(lp, XPSTR("ins("), 4)) {
          char s1[SCRIPT_MAXSSIZE];
          lp = GetStringArgument(lp + 4, OPER_EQU, s1, 0);
          SCRIPT_SKIP_SPACES
          char s2[SCRIPT_MAXSSIZE];
          lp = GetStringArgument(lp, OPER_EQU, s2, 0);
          SCRIPT_SKIP_SPACES
          char *cp = strstr(s1, s2);
          if (cp) {
            fvar = ((uint32_t)cp - (uint32_t)s1);
          } else {
            fvar = -1;
          }
          goto nfuncexit;
        }
        if (!strncmp_XP(lp, XPSTR("int("), 4)) {
          lp = GetNumericArgument(lp + 4, OPER_EQU, &fvar, gv);
          fvar = floor(fvar);
          goto nfuncexit;
        }
        if (!strncmp_XP(lp, XPSTR("is("), 3)) {
          lp = isargs(lp + 3, 0);
          fvar = 0;
          len = 0;
          goto exit;
        }
        if (!strncmp_XP(lp, XPSTR("is1("), 4)) {
          lp = isargs(lp + 4, 1);
          fvar = 0;
          len = 0;
          goto exit;
        }
        if (!strncmp_XP(lp, XPSTR("is2("), 4)) {
          lp = isargs(lp + 4, 2);
          fvar = 0;
          len = 0;
          goto exit;
        }
        if (!strncmp_XP(lp, XPSTR("is["), 3)) {
          lp = isget(lp + 3, sp, 0, gv);
          len = 0;
          goto strexit;
        }
        if (!strncmp_XP(lp, XPSTR("is1["), 4)) {
          lp = isget(lp + 4, sp, 1, gv);
          len = 0;
          goto strexit;
        }
        if (!strncmp_XP(lp, XPSTR("is2["), 4)) {
          lp = isget(lp + 4, sp, 2, gv);
          len = 0;
          goto strexit;
        }
#ifdef USE_SCRIPT_I2C
        if (!strncmp_XP(lp, XPSTR("ia"), 2)) {
          uint8_t bus = 0;
          lp += 2;
          if (*lp == '2') {
            lp++;
            bus = 1;
          }
          lp = GetNumericArgument(lp + 1, OPER_EQU, &fvar, gv);
          fvar = script_i2c(0, fvar, bus);
          goto nfuncexit;
        }
        if (!strncmp_XP(lp, XPSTR("iw"), 2)) {
          uint8_t bytes = 1;
          lp += 2;
          if (*lp != '(') {
            bytes = *lp & 0xf;
            if (bytes < 1) bytes = 1;
            if (bytes > 4) bytes = 4;
            lp++;
          }
          lp = GetNumericArgument(lp + 1, OPER_EQU, &fvar, gv);
          SCRIPT_SKIP_SPACES
          // arg2
          TS_FLOAT fvar2;
          lp = GetNumericArgument(lp, OPER_EQU, &fvar2, gv);
          if (glob_script_mem.res_ivar) {
            fvar = script_i2c(9 + bytes, fvar, *(uint32_t*)&fvar2);
          } else {
            fvar = script_i2c(9 + bytes, fvar, fvar2);
          }
          goto nfuncexit;
        }
        if (!strncmp_XP(lp, XPSTR("ir"), 2)) {
          uint8_t bytes = 1;
          lp += 2;
          if (*lp != '(') {
            bytes = *lp & 0xf;
            if (bytes < 1) bytes = 1;
            if (bytes > 4) bytes = 4;
            lp++;
          }
          lp = GetNumericArgument(lp + 1, OPER_EQU, &fvar, gv);
          if (glob_script_mem.res_ivar) {
            uint32_t intres = script_i2c(2, fvar, bytes);
            (*(uint32_t*)&fvar) = intres;
          } else {
            fvar = script_i2c(2, fvar, bytes);
          }
          goto nfuncexit;
        }
#endif // USE_SCRIPT_I2C

#ifdef ESP32
#ifdef USE_I2S_AUDIO
        if (!strncmp_XP(lp, XPSTR("i2sw("), 5)) {
          TS_FLOAT port;
          lp = GetNumericArgument(lp + 5, OPER_EQU, &port, gv);
          uint16_t alen = 0;
          TS_FLOAT *fa = 0;
          lp = get_array_by_name(lp, &fa, &alen, 0);
          if (!fa) {
            fvar = -1;
            goto nfuncexit;
          }
          lp = GetNumericArgument(lp, OPER_EQU, &fvar, gv);
          uint32_t bytes_written;
          int16_t *wp = (int16_t*)special_malloc(alen * 2);
          if (wp) {
            for (uint16_t cnt = 0; cnt < alen; cnt++) {
                wp[cnt] = fa[cnt];
            }
            i2s_write((i2s_port_t)port, (const uint8_t*)wp, fvar, &bytes_written, 0);
            free(wp);
            fvar = bytes_written;
          }
          goto nfuncexit;
        }
#endif // USE_I2S_AUDIO
#endif // ESP32
        if (!strncmp_XP(lp, XPSTR("i("), 2)) {
          // convert to integer var
          lp = GetNumericArgument(lp + 2, OPER_EQU, &fvar, gv);
          *(uint32_t*)&fvar = fvar;
          goto nfuncexit;
        }
        break;

#ifdef ESP32
#ifdef JPEG_PICTS
#ifdef STREAM_JPEG_PICTS
      case 'j':
        if (!strncmp_XP(lp, XPSTR("jpg("), 4)) {
          lp = GetNumericArgument(lp + 4, OPER_EQU, &fvar, 0);
          uint8_t selector = fvar;
          switch (selector) {
            case 0:
              // start streaming
              char url[SCRIPT_MAXSSIZE];
              lp = GetStringArgument(lp, OPER_EQU, url, 0);
              TS_FLOAT xp, yp, scale ;
              lp = GetNumericArgument(lp, OPER_EQU, &xp, 0);
              lp = GetNumericArgument(lp, OPER_EQU, &yp, 0);
              lp = GetNumericArgument(lp, OPER_EQU, &scale, 0);
              fvar = fetch_jpg(0, url, xp, yp, scale);
              break;
            default:
              // other cmds
              fvar = fetch_jpg(selector, 0, 0, 0, 0);
              break;
          }
          goto nfuncexit;
        }
        break;
#endif // STREAM_JPEG_PICTS
#endif // JPEG_PICTS
#endif // ESP32

#ifdef USE_KNX
      case 'k':
        if (!strncmp_XP(lp, XPSTR("knx("), 4)) {
          TS_FLOAT type;
          lp = GetNumericArgument(lp + 4, OPER_EQU, &type, gv);
          lp = GetNumericArgument(lp, OPER_EQU, &fvar, gv);
          SCRIPT_SKIP_SPACES
          KnxSensor(type, fvar);
          goto nfuncexit;
        }
        break;
#endif

      case 'l':
        if (!strncmp_XP(lp, XPSTR("lip"), 3)) {
          if (sp) strlcpy(sp, (const char*)WiFi.localIP().toString().c_str(), glob_script_mem.max_ssize);
          goto strexit;
        }
#ifdef USE_SCRIPT_GLOBVARS
        if (!strncmp_XP(vname, XPSTR("luip"), 4)) {
          if (sp) strlcpy(sp, glob_script_mem.last_udp_ip.toString().c_str(), glob_script_mem.max_ssize);
          goto strexit;
        }
#endif //USE_SCRIPT_GLOBVARS
        if (!strncmp_XP(vname, XPSTR("loglvl"), 6)) {
          fvar = glob_script_mem.script_loglevel;
          tind->index = SCRIPT_LOGLEVEL;
  exit_settable:
          if (fp) *fp = fvar;
          *vtype = NTYPE;
          tind->bits.settable = 1;
          tind->bits.is_string = 0;
          return lp + len;
        }
#ifdef USE_LVGL
        if (!strncmp_XP(lp, XPSTR("lvgl("), 5)) {
          lp = GetNumericArgument(lp + 5, OPER_EQU, &fvar, gv);
          SCRIPT_SKIP_SPACES
          fvar = lvgl_test(&lp, fvar);
          goto nfuncexit;
        }
#endif // USE_LVGL

#ifdef USE_UFILESYS
#ifdef USE_SCRIPT_FATFS_EXT
        if (!strncmp_XP(lp, XPSTR("lfw("), 4)) {
          char path[SCRIPT_MAXSSIZE];
          lp = GetStringArgument(lp + 4, OPER_EQU, path, 0);
          char payload[SCRIPT_MAXSSIZE];
          lp = GetStringArgument(lp, OPER_EQU, payload, 0);
          lp = GetNumericArgument(lp, OPER_EQU, &fvar, 0);
          // write to logfile
          fvar = script_logfile_write(path, payload, fvar);
          goto nfuncexit;
        }
#endif // USE_SCRIPT_FATFS_EXT
#endif

#ifdef USE_ANGLE_FUNC
        if (!strncmp_XP(lp, XPSTR("log("), 4)) {
          lp = GetNumericArgument(lp + 4, OPER_EQU, &fvar, gv);
          SCRIPT_SKIP_SPACES
          fvar = log(fvar);
          goto nfuncexit;
        }
#endif
        break;
      case 'm':
        if (!strncmp_XP(lp, XPSTR("med("), 4)) {
          TS_FLOAT fvar1;
          lp = GetNumericArgument(lp + 4, OPER_EQU, &fvar1, gv);
          SCRIPT_SKIP_SPACES
          // arg2
          TS_FLOAT fvar2;
          lp = GetNumericArgument(lp, OPER_EQU, &fvar2, gv);
          fvar = DoMedian5(fvar1, fvar2);
          goto nfuncexit;
        }
#ifdef USE_ANGLE_FUNC
        if (!strncmp_XP(lp, XPSTR("mpt("), 4)) {
          lp = GetNumericArgument(lp + 4, OPER_EQU, &fvar, gv);
          fvar = MeasurePulseTime(fvar);
          goto nfuncexit;
        }
#endif //USE_ANGLE_FUNC
        if (!strncmp_XP(vname, XPSTR("micros"), 6)) {
          fvar = micros();
          goto exit;
        }
        if (!strncmp_XP(vname, XPSTR("millis"), 6)) {
          fvar = millis();
          goto exit;
        }
        if (!strncmp_XP(vname, XPSTR("mins"), 4)) {
          fvar = RtcTime.minute;
          goto exit;
        }
        if (!strncmp_XP(vname, XPSTR("month"), 5)) {
          fvar = RtcTime.month;
          goto exit;
        }
        if (!strncmp_XP(vname, XPSTR("mqttc"), 5)) {
          if (TasmotaGlobal.rules_flag.mqtt_connected) {
            TasmotaGlobal.rules_flag.mqtt_connected = 0;
            fvar = 1;
          }
          goto exit;
        }
        if (!strncmp_XP(vname, XPSTR("mqttd"), 5)) {
          if (TasmotaGlobal.rules_flag.mqtt_disconnected) {
            TasmotaGlobal.rules_flag.mqtt_disconnected = 0;
            fvar = 1;
          }
          goto exit;
        }
        if (!strncmp_XP(vname, XPSTR("mqtts"), 5)) {
          fvar = !TasmotaGlobal.global_state.mqtt_down;
          goto exit;
        }
        if (!strncmp_XP(lp, XPSTR("mp("), 3)) {
          TS_FLOAT fvar1;
          lp = GetNumericArgument(lp + 3, OPER_EQU, &fvar1, gv);
          SCRIPT_SKIP_SPACES
          while (*lp != ')') {
            char *opp = lp;
            lp++;
            TS_FLOAT fvar2;
            lp = GetNumericArgument(lp, OPER_EQU, &fvar2, gv);
            SCRIPT_SKIP_SPACES
            fvar = fvar1;
            if ((*opp == '<' && fvar1 < fvar2) ||
                (*opp == '>' && fvar1 > fvar2) ||
                (*opp == '=' && fvar1 == fvar2)) {
                  if (*lp !='<' && *lp != '>' && *lp != '=' && *lp != ')' && *lp != SCRIPT_EOL) {
                    TS_FLOAT fvar3;
                    lp = GetNumericArgument(lp, OPER_EQU, &fvar3, gv);
                    SCRIPT_SKIP_SPACES
                    fvar = fvar3;
                  } else {
                    fvar  = fvar2;
                  }
                  break;
            }
            while (*lp != '<' && *lp != '>' && *lp != '=' && *lp != ')' && *lp != SCRIPT_EOL) lp++;
          }
          len = 0;
          goto exit;
        }
#ifdef USE_MORITZ
        if (!strncmp_XP(lp, XPSTR("mo("), 3)) {
          TS_FLOAT fvar1;
          lp = GetNumericArgument(lp + 3, OPER_EQU, &fvar1, gv);
          SCRIPT_SKIP_SPACES
          TS_FLOAT fvar2;
          lp = GetNumericArgument(lp, OPER_EQU, &fvar2, gv);
          SCRIPT_SKIP_SPACES
          char rbuff[64];
          fvar = mo_getvars(fvar1, fvar2, rbuff);
          lp++;
          if (sp) strlcpy(sp, rbuff, glob_script_mem.max_ssize);
          len = 0;
          goto strexit;
        }
#endif //USE_MORITZ
#ifdef ESP32_FAST_MUX
        if (!strncmp_XP(lp, XPSTR("mux("), 4)) {
          lp = GetNumericArgument(lp + 4, OPER_EQU, &fvar, gv);
          if (fvar == 0) {
            // start
            lp = GetNumericArgument(lp, OPER_EQU, &fvar, gv);
            uint16_t alen;
            TS_FLOAT *fa;
            lp = get_array_by_name(lp, &fa, &alen, 0);
            if (!fa) {
              fvar = -1;
              goto nfuncexit;
            }
            TS_FLOAT falen;
            lp = GetNumericArgument(lp, OPER_EQU, &falen, gv);
            if (falen > alen) {
              falen = alen;
            }
            fvar = fast_mux(0, fvar, fa, falen);
          } else if (fvar == 1) {
            // stop
            fvar = fast_mux(1, 0, 0, 0);
          } else if (fvar == 2) {
            // set array
            uint16_t alen;
            TS_FLOAT *fa;
            lp = get_array_by_name(lp, &fa, &alen, 0);
            if (!fa) {
              fvar = -1;
              goto nfuncexit;
            }
            lp = GetNumericArgument(lp, OPER_EQU, &fvar, gv);
            if (fvar > alen) {
              fvar = alen;
            }
            fvar = fast_mux(2, 0, fa, fvar);
          } else {
            fvar = fast_mux(3, 0, 0, 0);
          }
          goto nfuncexit;
        }
#endif
        break;

      case 'n':
        if (!strncmp_XP(vname, XPSTR("npwr"), 4)) {
          fvar = TasmotaGlobal.devices_present;
          goto exit;
        }
        break;

#ifdef USE_SCRIPT_ONEWIRE
      case 'o':
        if (!strncmp_XP(vname, XPSTR("ow("), 3)) {
          lp = GetNumericArgument(lp + 3, OPER_EQU, &fvar, 0);
          uint8_t sel = fvar;
          SCRIPT_SKIP_SPACES
          if (*lp != ')') {
            lp = GetNumericArgument(lp, OPER_EQU, &fvar, 0);
          }
          fvar = script_ow(sel, fvar);
          goto nfuncexit;
        }
        break;
#endif // USE_SCRIPT_ONEWIRE

      case 'p':
        if (!strncmp_XP(lp, XPSTR("pin["), 4)) {
          // raw pin level
          GetNumericArgument(lp + 4, OPER_EQU, &fvar, gv);
          fvar = digitalRead((uint8_t)fvar);
          // skip ] bracket
          len++;
          goto exit;
        }
        if (!strncmp_XP(lp, XPSTR("pn["), 3)) {
          GetNumericArgument(lp + 3, OPER_EQU, &fvar, gv);
          fvar = Pin(fvar);
          // skip ] bracket
          len++;
          goto exit;
        }
#if  defined(ESP32) && (defined(USE_I2S_AUDIO) || defined(USE_TTGO_WATCH) || defined(USE_M5STACK_CORE2))
        if (!strncmp_XP(lp, XPSTR("pl("), 3)) {
          char path[SCRIPT_MAXSSIZE];
          lp = GetStringArgument(lp + 3, OPER_EQU, path, 0);
          Play_mp3(path);
          len++;
          len = 0;
          goto exit;
        }
#endif // USE_I2S_AUDIO
        if (!strncmp_XP(lp, XPSTR("pd["), 3)) {
          GetNumericArgument(lp + 3, OPER_EQU, &fvar, gv);
          uint8_t gpiopin = fvar;
          if ((gpiopin < nitems(TasmotaGlobal.gpio_pin)) && (TasmotaGlobal.gpio_pin[gpiopin] > 0)) {
            fvar = TasmotaGlobal.gpio_pin[gpiopin];
            // skip ] bracket
            len++;
            goto exit;
          }
          fvar = 999;
          goto exit;
        }
#ifdef ESP32
        if (!strncmp_XP(vname, XPSTR("pheap"), 5)) {
          fvar = ESP.getFreePsram();
          goto exit;
        }
#endif //ESP32
        if (!strncmp_XP(vname, XPSTR("prefix1"), 7)) {
          if (sp) strlcpy(sp, SettingsText(SET_MQTTPREFIX1), glob_script_mem.max_ssize);
          goto strexit;
        }
        if (!strncmp_XP(vname, XPSTR("prefix2"), 7)) {
          if (sp) strlcpy(sp, SettingsText(SET_MQTTPREFIX2), glob_script_mem.max_ssize);
          goto strexit;
        }
        if (!strncmp_XP(vname, XPSTR("prefix3"), 7)) {
          if (sp) strlcpy(sp, SettingsText(SET_MQTTPREFIX3), glob_script_mem.max_ssize);
          goto strexit;
        }
        if (!strncmp_XP(lp, XPSTR("pow("), 4)) {
          // arg1
          TS_FLOAT fvar1;
          lp = GetNumericArgument(lp + 4, OPER_EQU, &fvar1, gv);
          SCRIPT_SKIP_SPACES
          // arg2
          TS_FLOAT fvar2;
          lp = GetNumericArgument(lp, OPER_EQU, &fvar2, gv);
          fvar = FastPrecisePowf(fvar1, fvar2);
          goto nfuncexit;
        }
        if (!strncmp_XP(lp, XPSTR("pwr["), 4)) {
          GetNumericArgument(lp + 4, OPER_EQU, &fvar, gv);
          uint8_t index = fvar;
          if (index <= TasmotaGlobal.devices_present) {
            fvar = bitRead(TasmotaGlobal.power, index - 1);
          } else {
            fvar = -1;
          }
          len += 1;
          goto exit;
        }
        if (!strncmp_XP(lp, XPSTR("pc["), 3)) {
          GetNumericArgument(lp + 3, OPER_EQU, &fvar, gv);
          uint8_t index = fvar;
          if (index < 1 || index > MAX_COUNTERS) index = 1;
          fvar = RtcSettings.pulse_counter[index - 1];
          len += 1;
          goto exit;
        }

        break;

      case 'r':
        if (!strncmp_XP(vname, XPSTR("ram"), 3)) {
          fvar = glob_script_mem.script_mem_size + (glob_script_mem.script_size) + (PMEM_SIZE);
          goto exit;
        }
        if (!strncmp_XP(lp, XPSTR("rnd("), 4)) {
          // tasmota switch state
          GetNumericArgument(lp + 4, OPER_EQU, &fvar, gv);
          if (fvar<0) {
            randomSeed(-fvar);
            fvar = 0;
          } else {
            fvar = random(fvar);
          }
          // skip ] bracket
          len++;
          goto exit;
        }
        if (!strncmp_XP(lp, XPSTR("rma("), 4)) {
          uint16_t alen;
          TS_FLOAT *array;
          lp = get_array_by_name(lp + 4, &array, &alen, 0);
          char str[SCRIPT_MAXSSIZE];
          lp = GetStringArgument(lp, OPER_EQU, str, 0);
          uint32_t raddr = strtol(str, NULL, 16);
          raddr &= 0xfffffffc;
          const uint32_t *addr = (const uint32_t*)raddr;
          lp = GetNumericArgument(lp, OPER_EQU, &fvar, gv);
          uint32_t mlen = fvar;
          mlen &= 0xfffffffc;
          uint32_t *lp = (uint32_t*)special_malloc(mlen);
          if (lp) {
            for (uint32_t cnt = 0; cnt < mlen/4; cnt++) {
              lp[cnt] = addr[cnt];
            }
            //esp_flash_read();

            uint8_t *ucp = (uint8_t*)lp;
            for (uint32_t cnt = 0; cnt < mlen; cnt++) {
              array[cnt] = ucp[cnt];
            }
            free(lp);
            fvar = 0;
          } else {
            fvar = -1;
          }
          goto nfuncexit;
        }
/*
#if defined(ESP32) && (defined(USE_I2S_AUDIO) || defined(USE_M5STACK_CORE2) || defined(ESP32S3_BOX) || defined(USE_I2S_MIC))
        if (!strncmp_XP(lp, XPSTR("rec("), 4)) {
          char str[SCRIPT_MAXSSIZE];
          lp = GetStringArgument(lp + 4, OPER_EQU, str, 0);
          //SCRIPT_SKIP_SPACES
          //lp = GetNumericArgument(lp, OPER_EQU, &fvar, gv);
          fvar = i2s_record_shine(str);
          len++;
          goto exit;
        }
#endif
*/

        if (!strncmp_XP(lp, XPSTR("rr("), 3)) {
          lp += 4;
          len = 0;
          const char *cp = GetResetReason().c_str();
          if (sp) {
              if (cp) {
                strlcpy(sp, cp, glob_script_mem.max_ssize);
              } else {
                strlcpy(sp, "-", glob_script_mem.max_ssize);
              }
          }
          goto strexit;
        }
        if (!strncmp_XP(lp, XPSTR("rrsn"), 4)) {
          fvar = ESP_ResetInfoReason();
          goto exit;
        }
        if (!strncmp_XP(lp, XPSTR("rax"), 3)) {
          TasmotaGlobal.no_autoexec = 0;
          goto exit;
        }
#ifdef USE_ANGLE_FUNC
        if (!strncmp_XP(lp, XPSTR("rad("), 4)) {
          GetNumericArgument(lp + 4, OPER_EQU, &fvar, gv);
          fvar = fvar * 3.1415916535f / 180.0f;
          goto nfuncexit;
        }
        if (!strncmp_XP(lp, XPSTR("round("), 6)) {
          lp = GetNumericArgument(lp + 6, OPER_EQU, &fvar, gv);
          fvar = floorf(fvar);
          goto nfuncexit;
        }
#endif
        break;

      case 's':
        if (!strncmp_XP(vname, XPSTR("secs"), 4)) {
          fvar = RtcTime.second;
          goto exit;
        }
        if (!strncmp_XP(lp, XPSTR("sw["), 3)) {
          // tasmota switch state
          GetNumericArgument(lp + 3, OPER_EQU, &fvar, gv);
          fvar = SwitchLastState((uint32_t)fvar);
          // skip ] bracket
          len++;
          goto exit;
        }
        if (!strncmp_XP(vname, XPSTR("stack"), 5)) {
          fvar = GetStack();
          goto exit;
        }
#ifdef ESP32
        if (!strncmp_XP(vname, XPSTR("stkwm"), 5)) {
          fvar = uxTaskGetStackHighWaterMark(NULL);
          goto exit;
        }
#endif // ESP32
        if (!strncmp_XP(vname, XPSTR("slen"), 4)) {
          fvar = strlen(glob_script_mem.script_ram);
          goto exit;
        }
        if (!strncmp_XP(lp, XPSTR("sl("), 3)) {
          char str[SCRIPT_MAXSSIZE];
          lp = GetStringArgument(lp + 3, OPER_EQU, str, 0);
          fvar = strlen(str);
          goto nfuncexit;
        }
        if (!strncmp_XP(lp, XPSTR("sb("), 3)) {
          char str[SCRIPT_MAXSSIZE];
          lp = GetStringArgument(lp + 3, OPER_EQU, str, 0);
          SCRIPT_SKIP_SPACES
          TS_FLOAT fvar1;
          lp = GetNumericArgument(lp, OPER_EQU, &fvar1, gv);
          SCRIPT_SKIP_SPACES
          TS_FLOAT fvar2;
          lp = GetNumericArgument(lp, OPER_EQU, &fvar2, gv);
          lp++;
          len = 0;
          if (fvar1 < 0) {
            fvar1 = strlen(str) + fvar1;
          }
          if (sp) {
            memcpy(sp, &str[(uint8_t)fvar1], (uint8_t)fvar2);
            sp[(uint8_t)fvar2] = '\0';
          }
          goto strexit;
        }
        if (!strncmp_XP(lp, XPSTR("st("), 3)) {
          char str[SCRIPT_MAXSSIZE];
          lp = GetStringArgument(lp + 3, OPER_EQU, str, 0);
          SCRIPT_SKIP_SPACES
          char token[2];
          if (*lp == '\'') {
            lp++;
            lp = Get_esc_char(lp, token);
            lp++;
          } else {
            token[0] = *lp++;
          }
          token[1] = 0;
          SCRIPT_SKIP_SPACES
          lp = GetNumericArgument(lp, OPER_EQU, &fvar, gv);
          SCRIPT_SKIP_SPACES
          // skip ) bracket
          lp++;
          len = 0;
          if (sp) {
            // get stringtoken
            char *st = strtok(str, token);
            if (!st) {
              *sp = 0;
            } else {
              if (fvar > 0) {
                for (uint8_t cnt = 1; cnt <= fvar; cnt++) {
                  if (cnt == fvar) {
                    strcpy(sp, st);
                    break;
                  }
                  st = strtok(NULL, token);
                  if (!st) {
                    *sp = 0;
                    break;
                  }
                }
              } else {
                // get string before token
                fvar = -fvar;
                char cstr[SCRIPT_MAXSSIZE];
                strcpy(cstr, str);
                for (uint8_t cnt = 1; cnt <= fvar; cnt++) {
                  if (cnt == fvar - 1) {
                    *st = 0;
                    strcpy(sp, cstr);
                    break;
                  }
                  st = strtok(NULL, token);
                  if (!st) {
                    *sp = 0;
                    break;
                  }
                  strcat(cstr, token);
                  strcat(cstr, st);
                }
              }
            }
          }
          goto strexit;
        }
        if (!strncmp_XP(lp, XPSTR("s("), 2)) {
          lp += 2;
          uint8_t dprec = glob_script_mem.script_dprec;
          uint8_t lzero = glob_script_mem.script_lzero;
          char dsep = glob_script_mem.script_sepc;
          if (isdigit(*lp)) {
            if (*(lp + 1) == '.' || *(lp + 1) == ',') {
              dsep = *(lp + 1);
              lzero = *lp & 0xf;
              lp += 2;
              dprec = *lp & 0xf;
              lp++;
            } else {
              dprec = *lp & 0xf;
              lp++;
            }
          }
          bool isint = false;
          if (*lp != '(') {
            isint = is_int_var(lp);
          }
          lp = GetNumericArgument(lp, OPER_EQU, &fvar, gv);
          char str[SCRIPT_MAXSSIZE];
          if (isint) {
            double dvar = *(int32_t*)&fvar;
            f2char(dvar, dprec, lzero, str, dsep);
          } else {
            f2char(fvar, dprec, lzero, str, dsep);
          }
          if (sp) strlcpy(sp, str, glob_script_mem.max_ssize);
          lp++;
          len = 0;
          goto strexit;
        }
#if  defined(ESP32) && (defined(USE_I2S_AUDIO) || defined(USE_TTGO_WATCH) || defined(USE_M5STACK_CORE2))
        if (!strncmp_XP(lp, XPSTR("say("), 4)) {
          char text[SCRIPT_MAXSSIZE];
          lp = GetStringArgument(lp + 4, OPER_EQU, text, 0);
          Say(text);
          len++;
          len = 0;
          goto exit;
        }
#endif // USE_I2S_AUDIO

#ifdef ESP32
        if (!strncmp_XP(lp, XPSTR("sf("), 3)) {
          lp = GetNumericArgument(lp + 3, OPER_EQU, &fvar, gv);
          if (fvar < 80) fvar = 80;
          if (fvar > 240) fvar = 240;
          setCpuFrequencyMhz(fvar);
          fvar = getCpuFrequencyMhz();
          goto nfuncexit;
        }
#endif //ESP32
#ifdef USE_TTGO_WATCH
        if (!strncmp_XP(lp, XPSTR("slp("), 4)) {
          lp = GetNumericArgument(lp + 4, OPER_EQU, &fvar, gv);
          SCRIPT_SKIP_SPACES
          TTGO_Sleep(fvar);
          goto nfuncexit;
        }
#endif //USE_TTGO_WATCH
#if defined(USE_TIMERS) && defined(USE_SUNRISE)
        if (!strncmp_XP(vname, XPSTR("sunrise"), 7)) {
          fvar = SunMinutes(0);
          goto exit;
        }
        if (!strncmp_XP(vname, XPSTR("sunset"), 6)) {
          fvar = SunMinutes(1);
          goto exit;
        }
#endif //USE_TIMERS

#ifdef USE_SHUTTER
        if (!strncmp_XP(lp, XPSTR("sht["), 4)) {
          GetNumericArgument(lp + 4, OPER_EQU, &fvar, gv);
          uint8_t index = fvar;
          if (index <= TasmotaGlobal.shutters_present) {
            fvar = Settings->shutter_position[index - 1];
          } else {
            fvar = -1;
          }
          len += 1;
          goto exit;
        }
#endif //USE_SHUTTER
#ifdef USE_ANGLE_FUNC
        if (!strncmp_XP(lp, XPSTR("sin("), 4)) {
          lp = GetNumericArgument(lp + 4, OPER_EQU, &fvar, gv);
          fvar = sinf(fvar);
          goto nfuncexit;
        }
        if (!strncmp_XP(lp, XPSTR("sqrt("), 5)) {
          lp = GetNumericArgument(lp + 5, OPER_EQU, &fvar, gv);
          fvar = sqrtf(fvar);
          goto nfuncexit;
        }
#endif //USE_ANGLE_FUNC


#if defined(USE_SML_M) && defined (USE_SML_SCRIPT_CMD)
uint32_t sml_status(uint32_t meter);
extern char *SML_GetSVal(uint32_t index);

        if (!strncmp_XP(lp, XPSTR("sml["), 4)) {
          lp = GetNumericArgument(lp + 4, OPER_EQU, &fvar, gv);
          SCRIPT_SKIP_SPACES
          fvar = SML_GetVal(fvar);
          goto nfuncexit;
        }
        if (!strncmp_XP(lp, XPSTR("smls["), 5)) {
          lp = GetNumericArgument(lp + 5, OPER_EQU, &fvar, gv);
          SCRIPT_SKIP_SPACES
          lp++;
          len = 0;
          if (fvar > 0) {
            if (sp) strlcpy(sp, SML_GetSVal(fvar), glob_script_mem.max_ssize);
          } else {
            char sbuff[SCRIPT_MAXSSIZE];
            fvar = fabs(fvar);
            if (fvar < 1) {
              fvar = 1;
            }
            dtostrfd(SML_GetVal(fvar), glob_script_mem.script_dprec, sbuff);
            if (sp) strlcpy(sp, sbuff, glob_script_mem.max_ssize);
          }
          goto strexit;
        }
        if (!strncmp_XP(lp, XPSTR("sml("), 4)) {
          TS_FLOAT fvar1;
          lp = GetNumericArgument(lp + 4, OPER_EQU, &fvar1, gv);
          SCRIPT_SKIP_SPACES
          TS_FLOAT fvar2;
          lp = GetNumericArgument(lp, OPER_EQU, &fvar2, gv);
          SCRIPT_SKIP_SPACES
          if (fvar2 == 0) {
            TS_FLOAT fvar3;
            lp = GetNumericArgument(lp, OPER_EQU, &fvar3, gv);
            fvar = SML_SetBaud(fvar1, fvar3);
          } else if (fvar2 == 1) {
            char str[SCRIPT_MAXSSIZE];
            lp = GetStringArgument(lp, OPER_EQU, str, 0);
            fvar = SML_Write(fvar1, str);
          } else if (fvar2 == 2) {
            char str[SCRIPT_MAXSSIZE];
            str[0] = 0;
            fvar = SML_Read(fvar1, str, SCRIPT_MAXSSIZE);
            if (sp) strlcpy(sp, str, glob_script_mem.max_ssize);
            lp++;
            len = 0;
            goto strexit;
          } else if (fvar2 == 3) {
            uint8_t vtype;
            struct T_INDEX ind;
            lp = isvar(lp, &vtype, &ind, 0, 0, 0);
            if (vtype != VAR_NV) {
              // found variable as result
              if (vtype == NUM_RES || (vtype & STYPE) == 0) {
                // numeric result
                fvar = -1;
              } else {
                // string result
                uint8_t sindex = glob_script_mem.type[ind.index].index;
                char *cp = glob_script_mem.glob_snp + (sindex * glob_script_mem.max_ssize);
                fvar = SML_Set_WStr(fvar1, cp);
              }
            } else {
              fvar = -99;
            }
          } else {
            fvar = sml_status(fvar1);
          }
          goto nfuncexit;
        }
        if (!strncmp_XP(vname, XPSTR("smlj"), 4)) {
          fvar = sml_options;
          tind->index = SML_JSON_ENABLE;
          goto exit_settable;
        }
        if (!strncmp_XP(lp, XPSTR("smld("), 5)) {
          lp = GetNumericArgument(lp + 5, OPER_EQU, &fvar, gv);
          if (fvar < 1) fvar = 1;
          SML_Decode(fvar - 1);
          goto nfuncexit;
        }
        if (!strncmp_XP(lp, XPSTR("smlv["), 5)) {
          lp = GetNumericArgument(lp + 5, OPER_EQU, &fvar, gv);
          fvar = sml_getv(fvar);
          goto nfuncexit;
        }
#endif //USE_SML_M

#ifdef USE_SCRIPT_SERIAL
        if (!strncmp_XP(lp, XPSTR("so("), 3)) {
          TS_FLOAT rxpin, txpin, br;
          lp = GetNumericArgument(lp + 3, OPER_EQU, &rxpin, gv);
          SCRIPT_SKIP_SPACES
          lp = GetNumericArgument(lp, OPER_EQU, &txpin, gv);
          SCRIPT_SKIP_SPACES
          lp = GetNumericArgument(lp, OPER_EQU, &br, gv);
          SCRIPT_SKIP_SPACES
          uint32_t sconfig = TS_SERIAL_8N1;
          if (*lp != ')') {
            // serial options, must be 3 chars 8N1, 7E2 etc
            uint8_t bits = *lp++ & 0xf;
            uint8_t parity = 0;
            if (*lp == 'E') parity = 1;
            if (*lp == 'O') parity = 2;
            lp++;
            uint8_t stopb = (*lp++ & 0x3) - 1;
            sconfig = (bits - 5) + (parity * 8) + stopb * 4;
          }
          SCRIPT_SKIP_SPACES
          // check for rec buffer
          TS_FLOAT rxbsiz = 128;
          if (*lp != ')') {
              lp = GetNumericArgument(lp, OPER_EQU, &rxbsiz, gv);
          }
          fvar = -1;
          if (glob_script_mem.sp) {
            fvar == -1;
          } else {
            if (Is_gpio_used(rxpin) || Is_gpio_used(txpin)) {
              AddLog(LOG_LEVEL_INFO, PSTR("SCR: warning, pins already used"));
            }
 
            glob_script_mem.sp = new TasmotaSerial(rxpin, txpin, HARDWARE_FALLBACK, 0, rxbsiz);

            if (glob_script_mem.sp) {
              fvar = glob_script_mem.sp->begin(br, ConvertSerialConfig(sconfig));
              uint32_t savc = Settings->serial_config;
              //setRxBufferSize(TMSBSIZ);

              Settings->serial_config = sconfig;
              uint8_t uart = 0;
#ifdef ESP32
              uart = glob_script_mem.sp->getUart();
#endif
              AddLog(LOG_LEVEL_INFO, PSTR("SCR: Serial port set to %s %d bit/s at rx=%d tx=%d rbu=%d uart=%d"), GetSerialConfig().c_str(), (uint32_t)br,  (uint32_t)rxpin, (uint32_t)txpin, (uint32_t)rxbsiz, uart);
              Settings->serial_config = savc;
              if (glob_script_mem.sp->hardwareSerial()) {
                ClaimSerial();
              }
            } else {
              fvar = -2;
            }
          }
          goto nfuncexit;
        }
        if (!strncmp_XP(lp, XPSTR("sw("), 3)) {
          char str[SCRIPT_MAXSSIZE];
          lp = GetStringArgument(lp + 3, OPER_EQU, str, 0);
          fvar = -1;
          if (glob_script_mem.sp) {
            glob_script_mem.sp->write(str, strlen(str));
            fvar = 0;
          }
          goto nfuncexit;
        }
        if (!strncmp_XP(lp, XPSTR("swb("), 4)) {
          lp = GetNumericArgument(lp + 4, OPER_EQU, &fvar, 0);
          if (glob_script_mem.sp) {
            glob_script_mem.sp->write((uint8_t)fvar);
            fvar = 0;
          }
          goto nfuncexit;
        }
        if (!strncmp_XP(lp, XPSTR("sa("), 3)) {
          fvar = -1;
          if (glob_script_mem.sp) {
            fvar = glob_script_mem.sp->available();
          }
          goto nfuncexit;
        }
        if (!strncmp_XP(lp, XPSTR("srb("), 3)) {
          fvar = -1;
          if (glob_script_mem.sp) {
            fvar = glob_script_mem.sp->available();
            if (fvar > 0) {
              fvar = glob_script_mem.sp->read();
            }
          }
          goto nfuncexit;
        }
        if (!strncmp_XP(lp, XPSTR("sp("), 3)) {
          fvar = -1;
          if (glob_script_mem.sp) {
            fvar = glob_script_mem.sp->available();
            if (fvar > 0) {
              fvar = glob_script_mem.sp->peek();
            }
          }
          goto nfuncexit;
        }
        if (!strncmp_XP(lp, XPSTR("sr("), 3)) {
          uint16_t size = glob_script_mem.max_ssize;
          char str[SCRIPT_MAXSSIZE];
          memset(str, 0, size);
          lp += 3;
          uint8_t runt = 0;
          if (*lp != ')') {
            // read until
            lp = GetNumericArgument(lp, OPER_EQU, &fvar, 0);
            runt = fvar;
          }
          fvar = -1;
          uint8_t flg = 0;
          if (glob_script_mem.sp) {
            for (uint16_t index = 0; index < (size - 1); index++) {
              if (!glob_script_mem.sp->available()) {
                flg = 1;
                break;
              }
              uint8_t iob = glob_script_mem.sp->read();
              if (iob == runt) {
                flg = 2;
                break;
              }
              str[index] = iob;
            }
          }
          //AddLog(LOG_LEVEL_INFO, PSTR(">>: %d - %d - %d - %s"), runt, flg, index, str);
          lp++;
          len = 0;
          if (sp) strlcpy(sp, str, size);
          goto strexit;;
        }
        if (!strncmp_XP(lp, XPSTR("sc("), 3)) {
          fvar = -1;
          if (Script_Close_Serial()) {
            fvar = 0;
          }
          lp += 4;
          len = 0;
          goto exit;
        }
        // serial write array
        if (!strncmp_XP(lp, XPSTR("swa("), 4)) {
          fvar = -1;
          uint8_t modbus_buffer[64];
          uint16_t alen;
          TS_FLOAT *array;
          lp = get_array_by_name(lp + 4, &array, &alen, 0);
          SCRIPT_SKIP_SPACES
          if (!array) {
            goto exit;
          }
          TS_FLOAT len;
          lp = GetNumericArgument(lp, OPER_EQU, &len, 0);
          SCRIPT_SKIP_SPACES
          if (len > alen) len = alen;
          if (len < 1) len = 1;
          if (*lp != ')') {
            TS_FLOAT opt;
            lp = GetNumericArgument(lp, OPER_EQU, &opt, 0);
            SCRIPT_SKIP_SPACES
            uint16_t opts = opt;
            // calc modbus checksum
            uint16_t meter = opts >> 4;
            opts &= 3;
#ifdef USE_SML_M
            // calc modbus checksum
            if (len > sizeof(modbus_buffer)) len = sizeof(modbus_buffer);
            for (uint32_t cnt = 0; cnt < len; cnt++) {
              modbus_buffer[cnt] = *array++;
            }
            uint16_t crc = 0xffff;
            uint8_t *mbp = modbus_buffer;
            if (opts == 1) {
              mbp++;
              crc = 0;
            }
            uint8_t index = 6;
            crc = MBUS_calculateCRC(mbp, index, crc);
            if (opts == 1) {
              mbp[index] = highByte(crc);
              mbp[index + 1] = lowByte(crc);
            } else {
              mbp[index] = lowByte(crc);
              mbp[index + 1] = highByte(crc);
            }

#define SCRIPT_HSTR_LEN 32
#ifdef USE_SML_SCRIPT_CMD
            if (meter) {
              if (!glob_script_mem.hstr) {
                glob_script_mem.hstr = (char*)malloc(SCRIPT_HSTR_LEN);
              }
              if (len > SCRIPT_HSTR_LEN/2) {
                len = SCRIPT_HSTR_LEN/2;
              }
              char *cp = glob_script_mem.hstr;
              *cp++ = 'r';

              uint8_t *mbp = modbus_buffer;
              for (uint32_t cnt = 0; cnt < len; cnt++) {
                sprintf(cp,"%02x",*mbp++);
                cp += 2;
              }
              //01 06 00 03 00 20 78 12 00 02 81 c1
              SML_Set_WStr(meter, glob_script_mem.hstr);
            }
#endif
#endif
            if (!meter) {
              uint8_t *ucp = modbus_buffer;
              if (glob_script_mem.sp) {
                while (len) {
                  glob_script_mem.sp->write(*ucp);
                  //AddLog(LOG_LEVEL_INFO,PSTR(">> %02x"),*ucp);
                  ucp++;
                  len--;
                }
              }
            }
          } else {
            if (glob_script_mem.sp) {
              while (len) {
                glob_script_mem.sp->write((uint8_t)*array++);
                len--;
              }
            }
          }
          goto nfuncexit;
        }
        // serial read array
        if (!strncmp_XP(lp, XPSTR("sra("), 4)) {
          fvar = -1;
          if (glob_script_mem.sp) {
            uint16_t alen;
            TS_FLOAT *array = 0;
            lp = get_array_by_name(lp + 4, &array, &alen, 0);
            SCRIPT_SKIP_SPACES
            if (!array) {
              goto exit;
            }
            TS_FLOAT opts = -1;
            if (*lp != ')') {
              lp = GetNumericArgument(lp, OPER_EQU, &opts, 0);
              SCRIPT_SKIP_SPACES
              // calc modbus checksum
            }

            uint16_t index = 0;
            for (index = 0; index < alen; index++) {
              if (!glob_script_mem.sp->available()) {
                break;
              }
              array[index] = glob_script_mem.sp->read();
            }
            fvar = index;
#ifdef USE_SML_M
            if (index == 8 || opts >= 0) {
              uint8_t *modbus_response = (uint8_t*)special_malloc(alen);
              if (modbus_response) {
                for (uint8_t cnt = 0; cnt < alen; cnt++) {
                  modbus_response[cnt] = array[cnt];
                }
                uint16_t crc = 0xffff;
                uint8_t *mbp = modbus_response;
                if (opts == 1) {
                  mbp++;
                  crc = 0;
                }
                crc = MBUS_calculateCRC(mbp, mbp[2] + 3, crc);
                if (opts == 1) {
                  if ((mbp[mbp[2] + 3] != highByte(crc)) || (mbp[mbp[2] + 4] != lowByte(crc))) {
                    fvar = -2;
                  }
                } else {
                  if ((mbp[mbp[2] + 3] != lowByte(crc)) || (mbp[mbp[2] + 4] != highByte(crc))) {
                    fvar = -2;
                  }
                }
                free(modbus_response);
              }
            }
#endif
          }
          goto nfuncexit;
        }
#ifdef USE_SML_M
        // serial modbus write float, 010404ffffffffxxxx
        if (!strncmp_XP(lp, XPSTR("smw("), 4)) {
          fvar = -1;
          if (glob_script_mem.sp) {
            TS_FLOAT addr;
            lp = GetNumericArgument(lp + 4, OPER_EQU, &addr, 0);
            SCRIPT_SKIP_SPACES
            TS_FLOAT mode;
            lp = GetNumericArgument(lp, OPER_EQU, &mode, 0);
            SCRIPT_SKIP_SPACES

            uint16_t alend;
            TS_FLOAT *fpd;
            lp = get_array_by_name(lp, &fpd, &alend, 0);
            SCRIPT_SKIP_SPACES

            TS_FLOAT nvals;
            lp = GetNumericArgument(lp, OPER_EQU, &nvals, 0);
            SCRIPT_SKIP_SPACES

            if (nvals > alend) {
              nvals = alend;
            }

            uint8_t modbus_response[128];

            uint8_t mb_index = 0;
            modbus_response[mb_index] = addr;
            mb_index++;
            modbus_response[mb_index] = 4;
            mb_index++;

            if (mode == 0) {
              modbus_response[mb_index] = 2 * nvals;
            } else {
              modbus_response[mb_index] = 4 * nvals;
            }
            mb_index++;

            for (uint16_t cnt = 0; cnt < nvals; cnt++) {
              TS_FLOAT fpval = *fpd++;
              uint32_t ui32 = fpval;
              uint32_t uval, *uvp;
              uvp = &uval;
              *(uvp) = *(uint32_t*)&fpval;

              switch  ((uint8_t)mode) {
                case 0:
                  // UINT16
                  modbus_response[mb_index] = (ui32 >> 16);
                  mb_index++;
                  modbus_response[mb_index] = (ui32 >> 0);
                  mb_index++;
                  break;
                case 1:
                  // UINT32
                  modbus_response[mb_index] = (ui32 >> 24);
                  mb_index++;
                  modbus_response[mb_index] = (ui32 >> 16);
                  mb_index++;
                  modbus_response[mb_index] = (ui32 >> 8);
                  mb_index++;
                  modbus_response[mb_index] = (ui32 >> 0);
                  mb_index++;
                  break;
                default:
                  // float
                  modbus_response[mb_index] = (uval >> 24);
                  mb_index++;
                  modbus_response[mb_index] = (uval >> 16);
                  mb_index++;
                  modbus_response[mb_index] = (uval >> 8);
                  mb_index++;
                  modbus_response[mb_index] = (uval >> 0);
                  mb_index++;
                  break;
                }
              }

            // calc modbus checksum
            uint16_t crc = MBUS_calculateCRC(modbus_response, modbus_response[2] + 3, 0xFFFF);
            modbus_response[modbus_response[2] + 3] = lowByte(crc);
            modbus_response[modbus_response[2] + 4] = highByte(crc);
            glob_script_mem.sp->write(modbus_response, mb_index + 2);
            fvar = 0;
#if 0
            // show response
            char hexbuff[256];
            sprintf(hexbuff,"%02x%02x%02x - ",modbus_response[0],modbus_response[1],modbus_response[2]);
            for (uint16_t cnt = 3; cnt < mb_index; cnt+=4) {
              char cbuff[32];
              sprintf(cbuff," %02x%02x%02x%02x",modbus_response[cnt],modbus_response[cnt+1],modbus_response[cnt+2],modbus_response[cnt+3]);
              strcat(hexbuff,cbuff);
            }
            char cbuff[32];
            sprintf(cbuff," - %02x%02x",modbus_response[mb_index],modbus_response[mb_index+1]);
            strcat(hexbuff,cbuff);
            AddLog(LOG_LEVEL_INFO,PSTR("SCR: >> %s"),hexbuff);
#endif
          }
          lp++;
          len = 0;
          goto exit;
        }
#endif

#endif //USE_SCRIPT_SERIAL

        if (!strncmp_XP(lp, XPSTR("sas("), 4)) {
          lp = GetNumericArgument(lp + 4, OPER_EQU, &fvar, 0);
          if (fvar < 1 || fvar > 3) {
            fvar = 1;
          }
          script_sort_string_array(fvar - 1);
          goto nfuncexit;
        }

#ifdef USE_SCRIPT_SPI
        if (!strncmp_XP(lp, XPSTR("spi("), 4)) {
          lp = GetNumericArgument(lp + 4, OPER_EQU, &fvar, 0);
          uint8_t sel = fvar;
          uint8_t index;
          switch (sel) {
            case 0:
              // set bus pins
              lp = GetNumericArgument(lp , OPER_EQU, &fvar, 0);
              glob_script_mem.spi.sclk = fvar;

              if (glob_script_mem.spi.sclk < 0) {
                // attach to existing Tasmota SPI
                lp = GetNumericArgument(lp , OPER_EQU, &fvar, 0);
                fvar *= 1000000;
                glob_script_mem.spi.settings = SPISettings(fvar, MSBFIRST, SPI_MODE0);

                if (TasmotaGlobal.spi_enabled) {
#ifdef ESP8266
                  SPI.begin();
                  glob_script_mem.spi.spip = &SPI;
#endif // ESP8266

#ifdef ESP32
                  if (glob_script_mem.spi.sclk == -1) {
                    SPI.begin(Pin(GPIO_SPI_CLK), Pin(GPIO_SPI_MISO), Pin(GPIO_SPI_MOSI), -1);
                    glob_script_mem.spi.spip = &SPI;
                  } else {
                    glob_script_mem.spi.spip = new SPIClass(HSPI);
                    glob_script_mem.spi.spip->begin(Pin(GPIO_SPI_CLK, 1), Pin(GPIO_SPI_MISO, 1), Pin(GPIO_SPI_MOSI, 1), -1);
                  }

#endif // ESP32
                } else {
                  AddLog(LOG_LEVEL_INFO, PSTR("SCR: error, spi pins not defined"));
                }
                break;
              }
              pinMode(glob_script_mem.spi.sclk , OUTPUT);
              digitalWrite(glob_script_mem.spi.sclk , 0);

              lp = GetNumericArgument(lp , OPER_EQU, &fvar, 0);
              glob_script_mem.spi.mosi = fvar;
              if (glob_script_mem.spi.mosi >= 0) {
                pinMode(glob_script_mem.spi.mosi , OUTPUT);
                digitalWrite(glob_script_mem.spi.mosi , 0);
              }

              lp = GetNumericArgument(lp , OPER_EQU, &fvar, 0);
              glob_script_mem.spi.miso = fvar;
              if (glob_script_mem.spi.miso >= 0) {
                  pinMode(glob_script_mem.spi.miso , INPUT_PULLUP);
              }

              if (Is_gpio_used(glob_script_mem.spi.mosi) || Is_gpio_used(glob_script_mem.spi.miso)
                  || Is_gpio_used(glob_script_mem.spi.sclk) ) {
                AddLog(LOG_LEVEL_INFO, PSTR("SCR: warning, pins already used"));
              }
              break;

            case 1:
              // set cs
              lp = GetNumericArgument(lp , OPER_EQU, &fvar, 0);
              index = fvar - 1;
              index &= 3;
              lp = GetNumericArgument(lp , OPER_EQU, &fvar, 0);
              glob_script_mem.spi.cs[index] = fvar;
              pinMode(glob_script_mem.spi.cs[index] , OUTPUT);
              digitalWrite(glob_script_mem.spi.cs[index] , 1);
              if (Is_gpio_used(glob_script_mem.spi.cs[index])) {
                AddLog(LOG_LEVEL_INFO, PSTR("SCR: warning, pins already used"));
              }
              break;

            case 2:
              // transfer bytes
              lp = GetNumericArgument(lp , OPER_EQU, &fvar, 0);
              int8_t index = fvar - 1;

              TS_FLOAT *fpd = 0;
              uint16_t alend;
              lp = get_array_by_name(lp, &fpd, &alend, 0);

              // len
              TS_FLOAT len = alend;
              lp = GetNumericArgument(lp , OPER_EQU, &len, 0);
              if (len > alend) {
                len = alend;
              }

              // type
              lp = GetNumericArgument(lp , OPER_EQU, &fvar, 0);

              script_sspi_trans(index, fpd, len, fvar);
              break;
          }
          lp++;
          len = 0;
          goto exit;
        }
#endif // USE_SCRIPT_SPI
        if (!strncmp_XP(lp, XPSTR("s2hms("), 6)) {
          lp = GetNumericArgument(lp + 6, OPER_EQU, &fvar, 0);
          lp++;
          char tbuff[16];
          uint8_t hours = (uint32_t)fvar / 3600;
          fvar -= (hours * 3600);
          uint8_t mins = (uint32_t)fvar / 60;
          uint8_t secs = (uint32_t)fvar % 60;
          sprintf_P(tbuff,PSTR("%02d:%02d:%02d"), hours, mins, secs);
          if (sp) strlcpy(sp, tbuff, glob_script_mem.max_ssize);
          len = 0;
          goto strexit;
        }
#ifdef USE_FEXTRACT
        if (!strncmp_XP(lp, XPSTR("s2t("), 4)) {
          lp = GetNumericArgument(lp + 4, OPER_EQU, &fvar, 0);
          char str[SCRIPT_MAXSSIZE];
          s2tstamp(str, SCRIPT_MAXSSIZE, fvar, 0);
          if (sp) strlcpy(sp, str, glob_script_mem.max_ssize);
          len = 0;
          goto strexit;
        }
#endif // USE_FEXTRACT

        break;

      case 't':
        if (!strncmp_XP(vname, XPSTR("time"), 4)) {
          fvar = MinutesPastMidnight();
          goto exit;
        }
        if (!strncmp_XP(vname, XPSTR("tper"), 4)) {
          fvar = Settings->tele_period;
          tind->index = SCRIPT_TELEPERIOD;
          goto exit_settable;
        }
        if (!strncmp_XP(vname, XPSTR("tinit"), 5)) {
          fvar = TasmotaGlobal.rules_flag.time_init;
          goto exit;
        }
        if (!strncmp_XP(vname, XPSTR("tset"), 4)) {
          fvar = TasmotaGlobal.rules_flag.time_set;
          goto exit;
        }
        if (!strncmp_XP(vname, XPSTR("tstamp"), 6)) {
          if (sp) strlcpy(sp, GetDateAndTime(DT_LOCAL).c_str(), glob_script_mem.max_ssize);
          goto strexit;
        }
        if (!strncmp_XP(vname, XPSTR("topic"), 5)) {
          if (sp) strlcpy(sp, TasmotaGlobal.mqtt_topic, glob_script_mem.max_ssize);
          goto strexit;
        }
#ifdef USE_SCRIPT_TIMER
        if (!strncmp_XP(lp, XPSTR("ts1("), 4)) {
          lp = GetNumericArgument(lp + 4, OPER_EQU, &fvar, gv);
          if (fvar < 10) fvar = 10;
          Script_ticker1.attach_ms(fvar, Script_ticker1_end);
          goto nfuncexit;
        }
        if (!strncmp_XP(lp, XPSTR("ts2("), 4)) {
          lp = GetNumericArgument(lp + 4, OPER_EQU, &fvar, gv);
          if (fvar < 10) fvar = 10;
          Script_ticker2.attach_ms(fvar, Script_ticker2_end);
          goto nfuncexit;
        }
        if (!strncmp_XP(lp, XPSTR("ts3("), 4)) {
          lp = GetNumericArgument(lp + 4, OPER_EQU, &fvar, gv);
          if (fvar < 10) fvar = 10;
          Script_ticker3.attach_ms(fvar, Script_ticker3_end);
          goto nfuncexit;
        }
        if (!strncmp_XP(lp, XPSTR("ts4("), 4)) {
          lp = GetNumericArgument(lp + 4, OPER_EQU, &fvar, gv);
          if (fvar < 10) fvar = 10;
          Script_ticker4.attach_ms(fvar, Script_ticker4_end);
          goto nfuncexit;
        }
#endif // USE_SCRIPT_TIMER

#ifdef USE_DISPLAY
#ifdef USE_TOUCH_BUTTONS
        if (!strncmp_XP(lp, XPSTR("tbut["), 5)) {
          GetNumericArgument(lp + 5, OPER_EQU, &fvar, gv);
          uint8_t index = fvar;
          if (index < 1 || index > MAX_TOUCH_BUTTONS) index = 1;
          index--;
          if (buttons[index]) {
            fvar = buttons[index]->vpower.on_off;
          } else {
            fvar = -1;
          }
          len += 1;
          goto exit;
        }

#endif //USE_TOUCH_BUTTONS
#endif //USE_DISPLAY


#if 0
        if (!strncmp_XP(lp, XPSTR("test("), 5)) {
          lp = GetNumericArgument(lp + 5, OPER_EQU, &fvar, gv);
          uint32_t cycles;
          uint64_t accu = 0;
          char sbuffer[32];

          GT911_Touch_Init(&Wire1, -1, -1, 960, 540);

          /*
          // PSTR performance test
          // this is best case since everything will be in cache
          // PSTR at least 3 times slower here, will be much slower if cache missed
          for (uint16 cnt=0; cnt<1000; cnt++) {
            cycles=ESP.getCycleCount();
            if (fvar>0) {
              snprintf_P(sbuffer, sizeof(sbuffer), PSTR("1234"));
            } else {
              snprintf(sbuffer, sizeof(sbuffer), "1234");
            }
            accu += ESP.getCycleCount()-cycles;
          }
          fvar = accu / 1000;
          */
          goto nfuncexit;
        }
#endif

#ifdef USE_TIMERS
        if (!strncmp_XP(lp, XPSTR("ttget("), 6)) {
          lp = GetNumericArgument(lp + 6, OPER_EQU, &fvar, gv);
          SCRIPT_SKIP_SPACES
          uint8_t index = fvar;
          if (index < 1 || index > MAX_TIMERS) index = 1;
          lp = GetNumericArgument(lp, OPER_EQU, &fvar, gv);
          SCRIPT_SKIP_SPACES
          fvar = get_tpars(index - 1, fvar);
          goto nfuncexit;
        }
#endif

#ifdef USE_FEXTRACT
        if (!strncmp_XP(lp, XPSTR("tso("), 4)) {
          char str[SCRIPT_MAXSSIZE];
          lp = GetStringArgument(lp + 4, OPER_EQU, str, 0);
          fvar = -1;
          SCRIPT_SKIP_SPACES
          lp = GetNumericArgument(lp, OPER_EQU, &fvar, gv);
          SCRIPT_SKIP_SPACES
          uint8_t flg = 0;
          if (*lp =='0') {
            lp++;
            flg = 1;
          }
          tso(str, fvar, flg);
          if (sp) strlcpy(sp, str, glob_script_mem.max_ssize);
          lp++;
          len = 0;
          goto strexit;
        }
        if (!strncmp_XP(lp, XPSTR("tsn("), 4)) {
          char str[SCRIPT_MAXSSIZE];
          lp = GetStringArgument(lp + 4, OPER_EQU, str, 0);
          fvar = tstamp2l(str);
          goto nfuncexit;
        }
#endif
        if (!strncmp_XP(lp, XPSTR("tc("), 3)) {
          lp = GetNumericArgument(lp + 3, OPER_EQU, &fvar, gv);
          lp++;
          if (sp) {
            sp[0] = fvar;
            sp[1] = 0;
          }
          len = 0;
          goto strexit;
        }
        break;
      case 'u':
        if (!strncmp_XP(vname, XPSTR("uptime"), 6)) {
          fvar = MinutesUptime();
          goto exit;
        }
        if (!strncmp_XP(vname, XPSTR("upsecs"), 6)) {
          fvar = TasmotaGlobal.uptime;
          goto exit;
        }
        if (!strncmp_XP(lp, XPSTR("upd["), 4)) {
          // var was updated
          struct T_INDEX ind;
          uint8_t vtype;
          isvar(lp + 4, &vtype, &ind, 0, 0, gv);
          if (!ind.bits.constant) {
            if (!ind.bits.changed) {
              fvar = 0;
              len++;
              goto exit;
            } else {
              glob_script_mem.type[ind.index].bits.changed = 0;
              fvar = 1;
              len++;
              goto exit;
            }
          }
          goto notfound;
        }
        if (!strncmp_XP(lp, XPSTR("udp("), 4)) {
          char url[SCRIPT_MAXSSIZE];
          lp = GetStringArgument(lp + 4, OPER_EQU, url, 0);
          TS_FLOAT port;
          lp = GetNumericArgument(lp, OPER_EQU, &port, gv);
          char payload[SCRIPT_MAXSSIZE];
          lp = GetStringArgument(lp, OPER_EQU, payload, 0);
          fvar = udp_call(url, port, payload);
          goto nfuncexit;
        }
        break;

      case 'w':
#if defined(ESP32) && defined(USE_WEBCAM)
        if (!strncmp_XP(lp, XPSTR("wc("), 3)) {
          TS_FLOAT fvar1;
          lp = GetNumericArgument(lp + 3, OPER_EQU, &fvar1, gv);
          SCRIPT_SKIP_SPACES
          switch ((uint32)fvar1) {
            case 0:
              { TS_FLOAT fvar2;
                lp = GetNumericArgument(lp, OPER_EQU, &fvar2, gv);
                fvar = WcSetup(fvar2);
              }
              break;
            case 1:
              { TS_FLOAT fvar2;
                lp = GetNumericArgument(lp, OPER_EQU, &fvar2, gv);
                fvar = WcGetFrame(fvar2);
              }
              break;
            case 2:
              { TS_FLOAT fvar2,fvar3;
                lp = GetNumericArgument(lp, OPER_EQU, &fvar2, gv);
                SCRIPT_SKIP_SPACES
                lp = GetNumericArgument(lp, OPER_EQU, &fvar3, gv);
                fvar = WcSetOptions(fvar2, fvar3);
              }
              break;
            case 3:
              fvar = WcGetWidth();
              break;
            case 4:
              fvar = WcGetHeight();
              break;
            case 5:
              { TS_FLOAT fvar2;
                lp = GetNumericArgument(lp, OPER_EQU, &fvar2, gv);
                fvar = WcSetStreamserver(fvar2);
              }
              break;
            case 6:
              { TS_FLOAT fvar2;
                lp = GetNumericArgument(lp, OPER_EQU, &fvar2, gv);
                fvar = WcSetMotionDetect(fvar2);
              }
              break;
              /*
#ifdef USE_FACE_DETECT
            case 7:
              { TS_FLOAT fvar2;
                lp = GetNumericArgument(lp, OPER_EQU, &fvar2, gv);
                fvar = WcSetFaceDetect(fvar2);
              }
              break;
#endif //USE_FACE_DETECT
*/
            default:
              fvar = 0;
          }
          goto nfuncexit;
        }
#endif //ESP32, USE_WEBCAM
#if defined(USE_TTGO_WATCH) && defined(USE_BMA423)
        if (!strncmp_XP(vname, XPSTR("wdclk"), 5)) {
          fvar = TTGO_doubleclick();
          goto exit;
        }
        if (!strncmp_XP(vname, XPSTR("wbut"), 4)) {
          fvar = TTGO_button();
          goto exit;
        }
#endif // USE_TTGO_WATCH
#if defined(USE_FT5206) || defined(USE_XPT2046) || defined(USE_LILYGO47) || defined(USE_GT911)
        if (!strncmp_XP(lp, XPSTR("wtch("), 5)) {
          lp = GetNumericArgument(lp + 5, OPER_EQU, &fvar, gv);
          fvar = Touch_Status(fvar);
          goto nfuncexit;
        }
#endif // USE_FT5206
        if (!strncmp_XP(vname, XPSTR("wm"), 2)) {
          fvar = glob_script_mem.web_mode;
          goto exit;
        }
        if (!strncmp_XP(vname, XPSTR("wday"), 4)) {
          fvar = RtcTime.day_of_week;
          goto exit;
        }
        if (!strncmp_XP(vname, XPSTR("wific"), 5)) {
          if (TasmotaGlobal.rules_flag.wifi_connected) {
            TasmotaGlobal.rules_flag.wifi_connected = 0;
            fvar = 1;
          }
          goto exit;
        }
        if (!strncmp_XP(vname, XPSTR("wifid"), 5)) {
          if (TasmotaGlobal.rules_flag.wifi_disconnected) {
            TasmotaGlobal.rules_flag.wifi_disconnected = 0;
            fvar = 1;
          }
          goto exit;
        }
        if (!strncmp_XP(vname, XPSTR("wifis"), 5)) {
          fvar = !TasmotaGlobal.global_state.wifi_down;
          goto exit;
        }
        if (!strncmp_XP(vname, XPSTR("wlp"), 3)) {
          OsWatchLoop();
          fvar = 0;
          goto exit;
        }
#ifdef xUSE_SHINE
        if (!strncmp_XP(vname, XPSTR("wav2mp3("), 8)) {
          char path[SCRIPT_MAXSSIZE];
          lp = GetStringArgument(lp + 8, OPER_EQU, path, 0);
          fvar = wav2mp3(path);
          goto nfuncexit;
        }
#endif
        break;
      case 'y':
        if (!strncmp_XP(vname, XPSTR("year"), 4)) {
          fvar = RtcTime.year;
          goto exit;
        }
        break;
      case '#':
        len = 0;
        lp = eval_sub(lp, &fvar, sp);
        if (glob_script_mem.retstr)  {
          goto strexit;
        } else {
          goto exit;
        }
        break;

      default:
        break;
    }

    // nothing valid found
notfound:
    if (fp) *fp = 0;
    *vtype = VAR_NV;
    tind->index = VAR_NV;
    glob_script_mem.var_not_found = 1;
    return lp;
    // return constant numbers
nfuncexit:
    lp++;
    len = 0;
exit:
    if (fp) *fp = fvar;
    *vtype = NUM_RES;
    tind->bits.constant = 1;
    tind->bits.is_string = 0;
    return lp + len;
    // return constant strings
strexit:
    *vtype = STYPE;
    tind->bits.constant = 1;
    tind->bits.is_string = 1;
    return lp + len;
}

char *getop(char *lp, uint8_t *operand) {
    switch (*lp) {
        case '=':
            if (*(lp + 1)=='=') {
                *operand = OPER_EQUEQU;
                return lp + 2;
            } else {
                *operand = OPER_EQU;
                return lp + 1;
            }
            break;
        case '+':
            if (*(lp + 1)=='=') {
                *operand = OPER_PLSEQU;
                return lp + 2;
            } else {
                *operand = OPER_PLS;
                return lp + 1;
            }
            break;
        case '-':
            if (*(lp + 1)=='=') {
                *operand = OPER_MINEQU;
                return lp + 2;
            } else {
                *operand = OPER_MIN;
                return lp + 1;
            }
            break;
        case '*':
            if (*(lp + 1)=='=') {
                *operand = OPER_MULEQU;
                return lp + 2;
            } else {
                *operand = OPER_MUL;
                return lp + 1;
            }
            break;
        case '/':
            if (*(lp + 1)=='=') {
                *operand = OPER_DIVEQU;
                return lp + 2;
            } else {
                *operand = OPER_DIV;
                return lp + 1;
            }
            break;
        case '!':
            if (*(lp + 1)=='=') {
                *operand = OPER_NOTEQU;
                return lp + 2;
            }
            break;
        case '>':
            if (*(lp + 1)=='>') {
              if (*(lp + 2) == '=') {
                *operand = OPER_SHREQU;
                return lp + 3;
              } else {
                *operand = OPER_SHR;
                return lp + 2;
              }
            } else {
              if (*(lp + 1)=='=') {
                *operand = OPER_GRTEQU;
                return lp + 2;
              } else {
                *operand = OPER_GRT;
                return lp + 1;

              }
            }
            break;
        case '<':
            if (*(lp + 1)=='<') {
              if (*(lp + 2) == '=') {
                *operand = OPER_SHLEQU;
                return lp + 3;
              } else {
                *operand = OPER_SHL;
                return lp + 2;
              }
            } else {
              if (*(lp + 1)=='=') {
                *operand = OPER_LOWEQU;
                return lp + 2;
              } else {
                *operand = OPER_LOW;
                return lp + 1;
              }
            }
            break;
        case '%':
            if (*(lp + 1)=='=') {
                *operand = OPER_PERCEQU;
                return lp + 2;
            } else {
                *operand = OPER_PERC;
                return lp + 1;
            }
            break;
        case '^':
            if (*(lp + 1)=='=') {
                *operand = OPER_XOREQU;
                return lp + 2;
            } else {
                *operand = OPER_XOR;
                return lp + 1;
            }
            break;
        case '&':
            if (*(lp + 1)=='=') {
                *operand = OPER_ANDEQU;
                return lp + 2;
            } else {
                *operand = OPER_AND;
                return lp + 1;
            }
            break;
        case '|':
            if (*(lp + 1)=='=') {
                *operand = OPER_OREQU;
                return lp + 2;
            } else {
                *operand = OPER_OR;
                return lp + 1;
            }
            break;
    }
    *operand = 0;
    return lp;
}

#ifdef USE_SCRIPT_FATFS_EXT
#ifdef USE_UFILESYS
int32_t script_logfile_write(char *path, char *payload, uint32_t size) {

      File rfd = ufsp->open(path, FS_FILE_APPEND);
      if (rfd == 0) {
        return -1;
      }

      uint32_t fsize = rfd.size();
      // append string
      rfd.write((uint8_t*)payload, strlen(payload));
      rfd.write((uint8_t*)"\n", 1);
      if (fsize < size) {
        rfd.close();
        return fsize;
      }
      rfd.seek(0, SeekSet);
      String line = rfd.readStringUntil('\n');
      File wfd = ufsp->open("/ltmp", FS_FILE_WRITE);
      if (!wfd) {
        return -2;
      }
      while (rfd.available()) {
        line = rfd.readStringUntil('\n');
        wfd.write((uint8_t*)line.c_str(), line.length());
        wfd.write((uint8_t*)"\n", 1);
      }
      rfd.close();
      wfd.close();
      ufsp->remove(path);
      ufsp->rename("/ltmp", path);

  return fsize;
}
#endif // USE_UFILESYS
#endif // USE_SCRIPT_FATFS_EXT

#ifdef ESP8266
extern "C" {
#include <cont.h>
  extern cont_t* g_pcont;
}
uint16_t GetStack(void) {
  register uint32_t *sp asm("a1");
  return (4 * (sp - g_pcont->stack));
}
#else
uint16_t GetStack(void) {
  register uint8_t *sp asm("a1");
  return (sp - pxTaskGetStackStart(NULL));
}
#endif //ESP8266

char *GetStringArgument(char *lp, uint8_t lastop, char *cp, struct GVARS *gv) {
  uint8_t operand = 0;
  uint8_t vtype;
  char *slp;
  struct T_INDEX ind;
  char str[SCRIPT_MAXSSIZE],str1[SCRIPT_MAXSSIZE];
  while (*lp == ' ') { lp++; } // skip leading spaces
  while (1) {
    lp = isvar(lp, &vtype, &ind, 0, str1, gv);
    if (vtype != STR_RES && !(vtype & STYPE)) {
      // numeric type
      glob_script_mem.glob_error = 1;
      return lp;
    }
    switch (lastop) {
        case OPER_EQU:
          strlcpy(str, str1, sizeof(str));
          break;
        case OPER_PLS:
          strncat(str, str1, sizeof(str) - strlen(str1));
          break;
    }
    slp = lp;
    lp = getop(lp, &operand);
    switch (operand) {
      case OPER_EQUEQU:
      case OPER_NOTEQU:
      case OPER_LOW:
      case OPER_LOWEQU:
      case OPER_GRT:
      case OPER_GRTEQU:
          lp = slp;
          strcpy(cp, str);
          return lp;
          break;
      default:
          break;
    }
    lastop = operand;
    if (!operand) {
      strcpy(cp, str);
      return lp;
    }
  }
  return lp;
}
char *GetNumericArgument(char *lp, uint8_t lastop, TS_FLOAT *fp, struct GVARS *gv);
char *GetNumericArgument(char *lp, uint8_t lastop, TS_FLOAT *fp, struct GVARS *gv) {
uint8_t operand = 0;
TS_FLOAT fvar1,fvar;
char *slp;
uint8_t vtype;
while (*lp == ' ') { lp++; } // skip leading spaces
struct T_INDEX ind;
    ind.bits.integer = 0;
    while (1) {
        // get 1. value
        if (*lp=='(') {
            lp++;
            lp = GetNumericArgument(lp, OPER_EQU, &fvar1, gv);
            lp++;
            //if (*lp==')') lp++;
        } else {
            lp = isvar(lp, &vtype, &ind, &fvar1, 0, gv);
            if ((vtype != NUM_RES) && (vtype & STYPE)) {
              // string type
              glob_script_mem.glob_error = 1;
            }
        }
        if (ind.bits.integer) {
          switch (lastop) {
            case OPER_EQU:
                fvar = fvar1;
                break;
            case OPER_PLS:
                *(int32_t*)&fvar += *(int32_t*)&fvar1;
                break;
            case OPER_MIN:
                *(int32_t*)&fvar -= *(int32_t*)&fvar1;
                break;
            case OPER_MUL:
                *(int32_t*)&fvar *= *(int32_t*)&fvar1;
                break;
            case OPER_DIV:
                *(int32_t*)&fvar /= *(int32_t*)&fvar1;
                break;
            case OPER_PERC:
                *(int32_t*)&fvar %= *(int32_t*)&fvar1;
                break;
            case OPER_XOR:
                *(uint32_t*)&fvar ^= *(uint32_t*)&fvar1;
                break;
            case OPER_AND:
                *(uint32_t*)&fvar &= *(uint32_t*)&fvar1;
                break;
            case OPER_OR:
                *(uint32_t*)&fvar |= *(uint32_t*)&fvar1;
                break;
            case OPER_SHL:
                *(uint32_t*)&fvar <<= *(uint32_t*)&fvar1;
                break;
            case OPER_SHR:
                *(uint32_t*)&fvar >>= *(uint32_t*)&fvar1;
                break;
            default:
                break;

          }
        } else {
          switch (lastop) {
            case OPER_EQU:
                fvar = fvar1;
                break;
            case OPER_PLS:
                fvar += fvar1;
                break;
            case OPER_MIN:
                fvar -= fvar1;
                break;
            case OPER_MUL:
                fvar *= fvar1;
                break;
            case OPER_DIV:
                fvar /= fvar1;
                break;
            case OPER_PERC:
                fvar = fmodf(fvar, fvar1);
                break;
            case OPER_XOR:
                fvar = (uint32_t)fvar ^ (uint32_t)fvar1;
                break;
            case OPER_AND:
                fvar = (uint32_t)fvar & (uint32_t)fvar1;
                break;
            case OPER_OR:
                fvar = (uint32_t)fvar | (uint32_t)fvar1;
                break;
            case OPER_SHL:
                fvar = (uint32_t)fvar << (uint32_t)fvar1;
                break;
            case OPER_SHR:
                fvar = (uint32_t)fvar >> (uint32_t)fvar1;
                break;
            default:
                break;

          }
        }
        slp = lp;
        lp = getop(lp, &operand);
        switch (operand) {
            case OPER_EQUEQU:
            case OPER_NOTEQU:
            case OPER_LOW:
            case OPER_LOWEQU:
            case OPER_GRT:
            case OPER_GRTEQU:
                lp = slp;
                *fp = fvar;
                return lp;
                break;
            default:
                break;
        }
        lastop = operand;
        if (!operand) {
            *fp = fvar;
            return lp;
        }
    }
}


char *ForceStringVar(char *lp, char *dstr) {
  TS_FLOAT fvar;
  char *slp = lp;
  glob_script_mem.glob_error = 0;
  lp = GetStringArgument(lp, OPER_EQU, dstr, 0);
  if (glob_script_mem.glob_error) {
    // mismatch
    lp = GetNumericArgument(slp, OPER_EQU, &fvar, 0);
    dtostrfd(fvar, 6, dstr);
    glob_script_mem.glob_error = 0;
  }
  return lp;
}

#ifdef USE_HOMEKIT

int32_t UpdVar(char *vname, float *fvar, uint32_t mode);

extern "C" {

  uint32_t Ext_UpdVar(char *vname, float *fvar, uint32_t mode) {
    return UpdVar(vname, fvar, mode);
  }
  void Ext_toLog(char *str) {
    toLog(str);
  }

  char *GetFName(void) {
    return SettingsText(SET_FRIENDLYNAME1);
  }
}

int32_t UpdVar(char *vname, float *fvar, uint32_t mode) {
  uint8_t type;
  uint8_t index;
  if (*vname == '@') {
      vname++;
      type = *vname;
      vname++;
      index = (*vname & 0x0f);
      if (index < 1) index = 1;
      if (index > 9) index = 9;
      switch (type) {
        case 'p':
          if (mode) {
            // set power
            ExecuteCommandPower(index, *fvar, SRC_BUTTON);
            return 0;
          } else {
            // read power
            *fvar = bitRead(TasmotaGlobal.power,  index - 1);
            return 1;
          }
          break;
        case 's':
          *fvar = SwitchLastState(index - 1);
          return 1;
          break;
        case 'b':
          *fvar = Button.last_state[index - 1];
          return 1;
          break;
      }
      return 0;
  }
  struct T_INDEX ind;
  uint8_t vtype;
  TS_FLOAT res = *fvar;
  isvar(vname, &vtype, &ind, fvar, 0, 0);
  if (vtype != VAR_NV) {
    // found variable as result
    if (vtype == NUM_RES || (vtype & STYPE) == 0) {
      if (mode) {
        // set var
        //AddLog(LOG_LEVEL_DEBUG, PSTR("write from homekit: %s - %d"), vname, (uint32_t)res);
        index = glob_script_mem.type[ind.index].index;
        glob_script_mem.fvars[index] = res;
        glob_script_mem.type[ind.index].bits.changed = 1;
#ifdef USE_SCRIPT_GLOBVARS
        if (glob_script_mem.type[ind.index].bits.global) {
          script_udp_sendvar(vname, &res, 0);
        }
#endif //USE_SCRIPT_GLOBVARS
        return 0;
      } else {
        // get var
        //index = glob_script_mem.type[ind.index].index;
        int32_t ret = 0;
#ifdef USE_SCRIPT_GLOBVARS
        ret = glob_script_mem.type[ind.index].bits.hchanged;
        glob_script_mem.type[ind.index].bits.hchanged = 0;
#endif
        //AddLog(LOG_LEVEL_DEBUG, PSTR("read from homekit: %s - %d - %d"), vname, (uint32_t)*fvar, ret);
        return ret;
      }
    } else {
      //  break;
    }
  }
  return -1;
}


extern "C" {
  void Ext_Replace_Cmd_Vars(char *srcbuf, uint32_t srcsize, char *dstbuf, uint32_t dstsize) {
    Replace_Cmd_Vars(srcbuf, srcsize, dstbuf, dstsize);
  }
}

#endif // USE_HOMEKIT


bool is_int_var(char *name) {
uint8_t vtype;
struct T_INDEX ind;

  isvar(name, &vtype, &ind, 0, 0, 0);

  if (vtype != VAR_NV) {
    if (vtype == NUM_RES || (vtype & STYPE) == 0) {
      // numeric result
      if (ind.bits.integer) {
        return true;
      }
    }
  }
  return false;
}

// replace vars in cmd %var%
void Replace_Cmd_Vars(char *srcbuf, uint32_t srcsize, char *dstbuf, uint32_t dstsize) {
    char *cp;
    uint16_t count;
    uint8_t vtype;
    uint8_t dprec = glob_script_mem.script_dprec;
    char dsep = glob_script_mem.script_sepc;
    uint8_t lzero = glob_script_mem.script_lzero;
    TS_FLOAT fvar;
    cp = srcbuf;
    struct T_INDEX ind;
    char string[SCRIPT_MAXSSIZE];
    dstsize -= 2;
    for (count = 0; count < dstsize; count++) {
        if (srcsize && (*cp == SCRIPT_EOL)) break;
        if (*cp == '%') {
            cp++;
            if (*cp == '%') {
              dstbuf[count] = *cp++;
            } else {
              if (isdigit(*cp)) {
                if (*(cp + 1) == '.' || *(cp + 1) == ',') {
                  dsep = *(cp + 1);
                  lzero = *cp & 0xf;
                  cp+=2;
                }
                dprec = *cp & 0xf;
                cp++;
              } else {
                dprec = glob_script_mem.script_dprec;
                lzero = glob_script_mem.script_lzero;
                dsep = glob_script_mem.script_sepc;
              }
              if (*cp=='(') {
                // math expression
                cp++;
                glob_script_mem.glob_error = 0;
                char *slp = cp;
                cp = GetNumericArgument(cp, OPER_EQU, &fvar, 0);
                if (glob_script_mem.glob_error == 1) {
                  glob_script_mem.glob_error = 0;
                  cp = GetStringArgument(slp, OPER_EQU, string, 0);
                } else {
                  f2char(fvar, dprec, lzero, string, dsep);
                }
                uint8_t slen = strlen(string);
                if (count + slen < dstsize - 1) {
                  strcpy(&dstbuf[count], string);
                  count += slen - 1;
                }
                cp += 2;
              } else {
                cp = isvar(cp, &vtype, &ind, &fvar, string, 0);
                if (vtype != VAR_NV) {
                  // found variable as result
                  if (vtype == NUM_RES || (vtype & STYPE) == 0) {
                    // numeric result
                    if (ind.bits.integer) {
                      double dval = *(int32_t*)&fvar;
                      f2char(dval, dprec, lzero, string, dsep);
                    } else {
                      f2char(fvar, dprec, lzero, string, dsep);
                    }
                  } else {
                    // string result
                  }
                  uint8_t slen = strlen(string);
                  if (count + slen < dstsize - 1) {
                    strcpy(&dstbuf[count], string);
                    count += slen - 1;
                  }
                  cp++;
                } else {
                  strcpy(&dstbuf[count], "???");
                  count += 2;
                  while (*cp != '%') {
                    if (*cp == 0 || *cp == SCRIPT_EOL) {
                      dstbuf[count+1] = 0;
                      return;
                    }
                    cp++;
                  }
                  cp++;
                }
              }
            }
        } else {
            if (*cp=='\\') {
              cp++;
              if (*cp=='n') {
                dstbuf[count] = '\n';
              } else if (*cp=='r') {
                dstbuf[count] = '\r';
              } else if (*cp=='\\') {
                dstbuf[count] = '\\';
              } else if (*cp=='#') {
                dstbuf[count] = '#';
              } else {
                dstbuf[count] = *cp;
              }
            } else {
              dstbuf[count] = *cp;
            }
            if (*cp==0) {
                break;
            }
            cp++;
        }
    }
    dstbuf[count] = 0;
}

void toLog(const char *str) {
  if (!str) return;
  AddLog(LOG_LEVEL_INFO, str);
}

void toLogN(const char *cp, uint8_t len) {
  if (!cp) return;
  char str[32];
  if (len>=sizeof(str)) len = sizeof(str);
  strlcpy(str, cp, len);
  toSLog(str);
}

void toLogEOL(const char *s1,const char *str) {
  if (!str) return;
  uint8_t index = 0;
  char log_data[700];   // Was MAX_LOGSZ
  char *cp = log_data;
  strcpy(cp, s1);
  cp += strlen(s1);
  while (*str) {
    if (*str==SCRIPT_EOL) break;
    *cp++ = *str++;
  }
  *cp = 0;
  AddLogData(LOG_LEVEL_INFO, log_data);
}


void toSLog(const char *str) {
  if (!str) return;
#if SCRIPT_DEBUG>0
  while (*str) {
    Serial.write(*str);
    str++;
  }
#endif
}

char *Evaluate_expression(char *lp, uint8_t and_or, uint8_t *result, struct GVARS *gv) {
  TS_FLOAT fvar,*dfvar,fvar1;
  uint8_t numeric;
  struct T_INDEX ind;
  uint8_t vtype = 0,lastop;
  uint8_t res = 0;
  char *llp = lp;
  char *slp;

  SCRIPT_SKIP_SPACES
  if (*lp=='(') {
    uint8_t res = 0;
    uint8_t xand_or = 0;
    lp++;

loop:
    SCRIPT_SKIP_SPACES
    lp = Evaluate_expression(lp, xand_or, &res, gv);
    if (*lp==')') {
      lp++;
      goto exit0;
    }
    // check for next and or
    SCRIPT_SKIP_SPACES
    if (!strncmp(lp, "or", 2)) {
      lp += 2;
      xand_or = 1;
      goto loop;
    } else if (!strncmp(lp, "and", 3)) {
      lp += 3;
      xand_or = 2;
      goto loop;
    }
exit0:
    if (!and_or) {
      *result = res;
    } else if (and_or==1) {
      *result|=res;
    } else {
      *result &= res;
    }
    goto exit10;
  }

  llp = lp;
  // compare
  dfvar = &fvar;
  glob_script_mem.glob_error = 0;
  slp = lp;
  numeric = 1;
  lp = GetNumericArgument(lp, OPER_EQU, dfvar, gv);
  if (glob_script_mem.glob_error==1) {
    // was string, not number
	  char cmpstr[SCRIPT_MAXSSIZE];
    lp = slp;
    numeric = 0;
    // get the string
    lp = isvar(lp, &vtype, &ind, 0, cmpstr, gv);
	  lp = getop(lp, &lastop);
    // compare string
    char str[SCRIPT_MAXSSIZE];
    lp = GetStringArgument(lp, OPER_EQU, str, gv);
    if (lastop==OPER_EQUEQU || lastop==OPER_NOTEQU) {
      res = strcmp(cmpstr, str);
      if (lastop==OPER_EQUEQU) res=!res;
      goto exit;
    }

  } else {
    // numeric
    // evaluate operand
    lp = getop(lp, &lastop);
    lp = GetNumericArgument(lp, OPER_EQU, &fvar1, gv);
    switch (lastop) {
      case OPER_EQUEQU:
          res = (*dfvar==fvar1);
          break;
      case OPER_NOTEQU:
          res = (*dfvar!=fvar1);
          break;
      case OPER_LOW:
          res = (*dfvar<fvar1);
          break;
      case OPER_LOWEQU:
          res = (*dfvar<=fvar1);
          break;
      case OPER_GRT:
          res = (*dfvar>fvar1);
          break;
      case OPER_GRTEQU:
          res = (*dfvar>=fvar1);
          break;
      default:
          // error
          break;
    }

exit:
    if (!and_or) {
      *result = res;
    } else if (and_or==1) {
      *result |= res;
    } else {
      *result &= res;
    }
  }


exit10:
#if IFTHEN_DEBUG>0
  char tbuff[128];
  sprintf(tbuff,"p1=%d,p2=%d,cmpres=%d,and_or=%d line: ", (int32_t)*dfvar, (int32_t)fvar1, *result, and_or);
  toLogEOL(tbuff, llp);
#endif
  return lp;
}

#ifdef ESP32

TimerHandle_t beep_th;
void StopBeep( TimerHandle_t xTimer );

void StopBeep( TimerHandle_t xTimer ) {
  ledcWriteTone(7, 0);
  xTimerStop(xTimer, 0);
}

void esp32_beep(int32_t freq ,uint32_t len) {
  if (freq<0) {
    if (freq <= -64) freq = 0;
    ledcSetup(7, 500, 10);
    ledcAttachPin(-freq, 7);
    ledcWriteTone(7, 0);
    if (!beep_th) {
      beep_th = xTimerCreate("beep", 100, pdFALSE, ( void * ) 0, StopBeep);
    }
  } else {
    if (!beep_th) return;
    if (!freq) {
      ledcWriteTone(7, 0);
      xTimerStop(beep_th, 10);
      return;
    }
    if (len < 10) return;
    if (xTimerIsTimerActive(beep_th)) return;
    ledcWriteTone(7, freq);
    uint32_t ticks = pdMS_TO_TICKS(len);
    xTimerChangePeriod( beep_th, ticks, 10);
  }
}
#endif // ESP32

uint8_t pwmpin[5];

void esp_pwm(int32_t value, uint32 freq, uint32_t channel) {

#ifdef ESP32
  if (channel < 1 || channel > 8) channel = 1;
  channel += 7;
  if (value < 0) {
    if (value <= -64) value = 0;
    // set range to 10 bit
    ledcSetup(channel, freq, 10);
    ledcAttachPin(-value, channel);
    ledcWrite(channel, 0);
  } else {
    if (value > 1023) {
      value = 1023;
    }
    ledcWrite(channel, value);
  }
#else
  // esp8266 default to range 0-1023
  if (channel < 1 || channel > 5) channel = 1;
  channel-=1;
  if (value < 0) {
    if (value <= -64) value = 0;
    pwmpin[channel] = -value;
    pinMode(pwmpin[channel], OUTPUT);
    analogWriteFreq(freq);
    analogWrite(pwmpin[channel], 0);
  } else {
    if (value > 1023) {
      value = 1023;
    }
    analogWrite(pwmpin[channel],value);
  }
#endif // ESP32
}
char *eval_sub(char *lp, TS_FLOAT *fvar, char *rstr);
char *eval_sub(char *lp, TS_FLOAT *fvar, char *rstr) {
  scripter_sub(lp - 1, 0);
  while (1) {
    if (*lp == ')') {
      lp++;
      break;
    }
    if (*lp == SCRIPT_EOL || *lp == '+' || *lp == '-') {
      break;
    }
    lp++;
  }
  if (glob_script_mem.retstr) {
    if (rstr) {
      strcpy(rstr, glob_script_mem.retstr);
    } else {
      *fvar = CharToFloat(glob_script_mem.retstr);
    }
    free (glob_script_mem.retstr);
  } else {
    if (fvar) {
      *fvar = glob_script_mem.retval;
    } else {
      dtostrfd(glob_script_mem.retval, 6, rstr);
    }
  }
  return lp;
}

//#define IFTHEN_DEBUG

char *scripter_sub(char *lp, uint8_t fromscriptcmd) {
  lp += 1;
  char *slp = lp;
  uint8_t plen = 0;
  while (*lp) {
    if (*lp=='\n'|| *lp=='\r'|| *lp=='(') {
      break;
    }
    lp++;
    plen++;
  }
  if (fromscriptcmd) {
    char *sp = glob_script_mem.scriptptr;
    glob_script_mem.scriptptr = glob_script_mem.scriptptr_bu;
    Run_Scripter1(slp, plen, 0);
    glob_script_mem.scriptptr = sp;
  } else {
    Run_Scripter1(slp, plen, 0);
  }
  lp = slp;
  return lp;
}

int16_t Run_script_sub(const char *type, int8_t tlen, struct GVARS *gv);

#define IF_NEST 8

int16_t Run_Scripter1(const char *type, int8_t tlen, const char *js) {
int16_t retval;
  if (!glob_script_mem.scriptptr) {
    return -99;
  }
  if (tasm_cmd_activ && tlen >= 0) return 0;

  struct GVARS gv;
  gv.jo = 0;
  retval = Run_script_sub(type, tlen, &gv);
  return retval;
}

// execute section of scripter
int16_t Run_Scripter(const char *type, int8_t tlen, const char *js) {
int16_t retval;

    if (!glob_script_mem.scriptptr) {
      return -99;
    }

    if (tasm_cmd_activ && tlen >= 0) return 0;

    struct GVARS gv;

    JsonParserObject jo;

    if (js) {
      String jss = js;    // copy the string to a new buffer, not sure we can change the original buffer
      JsonParser parser((char*)jss.c_str());
      jo = parser.getRootObject();
      gv.jo = &jo;
      retval = Run_script_sub(type, tlen, &gv);
    } else {
      gv.jo = 0;
      retval = Run_script_sub(type, tlen, &gv);
    }
    return retval;
}

#define SCRIPT_LOOP_NEST 3
int16_t Run_script_sub(const char *type, int8_t tlen, struct GVARS *gv) {
    uint8_t vtype = 0, sindex, xflg, fromscriptcmd = 0;
    int16_t globvindex;
    // 22 bytes per nested loop
    uint8_t floop[SCRIPT_LOOP_NEST] = {0, 0, 0};
    int8_t loopdepth = -1;
    char *lp_next[SCRIPT_LOOP_NEST];
    char *cv_ptr[SCRIPT_LOOP_NEST];
    TS_FLOAT *cv_count[SCRIPT_LOOP_NEST], cv_max[SCRIPT_LOOP_NEST], cv_inc[SCRIPT_LOOP_NEST];
    int16_t globaindex, saindex;
    struct T_INDEX ind;
    uint8_t operand, lastop, numeric = 1, if_state[IF_NEST], if_exe[IF_NEST], if_result[IF_NEST], and_or, ifstck = 0;
    if_state[ifstck] = 0;
    if_result[ifstck] = 0;
    if_exe[ifstck] = 1;
    char cmpstr[SCRIPT_MAXSSIZE];
    TS_FLOAT *dfvar;

    TS_FLOAT fvar = 0, fvar1, sysvar, swvar;
    uint8_t section = 0, sysv_type = 0, swflg = 0;

    char *lp;
    if (tlen == 0) {
        section = 1;
        lp = (char*)type;
    } else {
        lp = glob_script_mem.scriptptr;
    }

    uint8_t check = 0;
    if (tlen < 0) {
      tlen = abs(tlen);
      check = 1;
    }


    while (1) {
        // check line
        // skip leading spaces and tabs
startline:
        while (*lp == '\t' || *lp == ' ') {
          lp++;
        }
        // skip empty line
        SCRIPT_SKIP_EOL

        while (*lp == '\t' || *lp == ' ') {
          lp++;
        }
        
        // skip comment
        if (*lp == ';') goto next_line;
        if (!*lp) break;

        if (section) {
            // we are in section
            if (*lp == '>') {
                return 0;
            }
            if (*lp == '#') {
                return 0;
            }
            glob_script_mem.var_not_found = 0;

//#if SCRIPT_DEBUG>0
#ifdef IFTHEN_DEBUG
            char tbuff[128];
            sprintf(tbuff, "stack=%d,exe=%d,state=%d,cmpres=%d line: ", ifstck, if_exe[ifstck], if_state[ifstck], if_result[ifstck]);
            toLogEOL(tbuff, lp);
#endif //IFTHEN_DEBUG

//if (if_state[s_ifstck]==3 && if_result[s_ifstck]) goto next_line;
//if (if_state[s_ifstck]==2 && !if_result[s_ifstck]) goto next_line;

            if (!strncmp(lp, "if", 2)) {
                lp += 2;
                if (ifstck < IF_NEST - 1) ifstck++;
                if_state[ifstck] = 1;
                if_result[ifstck] = 0;
                if (ifstck == 1) if_exe[ifstck] = 1;
                else if_exe[ifstck] = if_exe[ifstck - 1];
                and_or = 0;
            } else if (!strncmp(lp, "then", 4) && if_state[ifstck] == 1) {
                lp += 4;
                if_state[ifstck] = 2;
                if (if_exe[ifstck - 1]) if_exe[ifstck] = if_result[ifstck];
            } else if (!strncmp(lp, "else", 4) && if_state[ifstck] == 2) {
                lp += 4;
                if_state[ifstck] = 3;
                if (if_exe[ifstck - 1]) if_exe[ifstck] = !if_result[ifstck];
            } else if (!strncmp(lp, "endif", 5) && if_state[ifstck] >= 2) {
                lp += 5;
                if (ifstck>0) {
                  if_state[ifstck] = 0;
                  ifstck--;
                }
                goto next_line;
            } else if (!strncmp(lp, "or", 2) && if_state[ifstck] == 1) {
                lp += 2;
                and_or = 1;
            } else if (!strncmp(lp, "and", 3) && if_state[ifstck] == 1) {
                lp += 3;
                and_or = 2;
            }

            if (*lp == '{' && if_state[ifstck] == 1) {
              lp += 1; // then
              if_state[ifstck] = 2;
              if (if_exe[ifstck - 1]) if_exe[ifstck] = if_result[ifstck];
            } else if (*lp == '{' && if_state[ifstck] == 3) {
              lp += 1; // after else
              //if_state[ifstck]=3;
            } else if (*lp == '}' && if_state[ifstck] >= 2) {
              lp++; // must check for else
              char *slp = lp;
              uint8_t iselse = 0;
              for (uint8_t count = 0; count < 8; count++) {
                if (*lp == '}') {
                  // must be endif
                  break;
                }
                if (!strncmp(lp, "else", 4)) {
                  // is before else, no endif
                  if_state[ifstck] = 3;
                  if (if_exe[ifstck - 1]) if_exe[ifstck] =! if_result[ifstck];
                  lp += 4;
                  iselse = 1;
                  SCRIPT_SKIP_SPACES
                  if (*lp == '{') lp++;
                  break;
                }
                lp++;
              }
              if (!iselse) {
                lp = slp;
                // endif
                if (ifstck > 0) {
                  if_state[ifstck] = 0;
                  ifstck--;
                }
                goto next_line;
              }
            }

            if (!strncmp(lp, "for", 3)) {
              // start for next loop, fetch 3 params
              // simple implementation, zero loop count not supported
              lp += 3;
              SCRIPT_SKIP_SPACES
              loopdepth++;
              if (loopdepth >= SCRIPT_LOOP_NEST) {
                loopdepth = SCRIPT_LOOP_NEST - 1;
              }
              lp_next[loopdepth] = 0;
              lp = isvar(lp, &vtype, &ind, 0, 0, gv);
              if ((vtype != VAR_NV) && (vtype & STYPE) == 0) {
                  // numeric var
                  uint8_t index = glob_script_mem.type[ind.index].index;
                  cv_count[loopdepth] = &glob_script_mem.fvars[index];
                  lp = GetNumericArgument(lp, OPER_EQU, cv_count[loopdepth], 0);
                  lp = GetNumericArgument(lp, OPER_EQU, &cv_max[loopdepth], 0);
                  lp = GetNumericArgument(lp, OPER_EQU, &cv_inc[loopdepth], 0);
                  //SCRIPT_SKIP_EOL
                  cv_ptr[loopdepth] = lp;
                  if (*cv_count[loopdepth] <= cv_max[loopdepth] && cv_inc[loopdepth] > 0) {
                    // inc loop
                    floop[loopdepth] = 1;
                  } else {
                    // dec loop
                    floop[loopdepth]  = 2;
                    if (cv_inc[loopdepth] > 0) {
                      floop[loopdepth] = 1;
                    }
                  }
              } else {
                      // error
                  toLogEOL("for error", lp);
              }
            } else if (!strncmp(lp, "next", 4)) {
getnext:
              if (loopdepth >= 0) {
                lp_next[loopdepth] = lp;
                if (floop[loopdepth] > 0) {
                  // for next loop
                  *cv_count[loopdepth] += cv_inc[loopdepth];
                  if (floop[loopdepth] == 1) {
                    if (*cv_count[loopdepth] <= cv_max[loopdepth]) {
                      lp = cv_ptr[loopdepth];
                    } else {
                      lp += 4;
                      floop[loopdepth] = 0;
                      loopdepth--;
                      if (loopdepth < -1) {
                        loopdepth = -1;
                      }
                    }
                  } else {
                    if (*cv_count[loopdepth] >= cv_max[loopdepth]) {
                      lp = cv_ptr[loopdepth];
                    } else {
                      lp += 4;
                      floop[loopdepth] = 0;
                      loopdepth--;
                      if (loopdepth < -1) {
                        loopdepth = -1;
                      }
                    }
                  }
                } else {
                  lp += 4;
                  loopdepth--;
                  if (loopdepth < -1) {
                    loopdepth = -1;
                  }
                }
              }
            }

            if (!strncmp(lp, "switch", 6)) {
              lp += 6;
              SCRIPT_SKIP_SPACES
              char *slp = lp;
              lp = GetNumericArgument(lp, OPER_EQU, &swvar, 0);
              if (glob_script_mem.glob_error == 1) {
                // was string, not number
                lp = slp;
                // get the string
                lp = isvar(lp, &vtype, &ind, 0, cmpstr, gv);
                swflg = 0x81;
              } else {
                swflg = 1;
              }
            } else if (!strncmp(lp, "case", 4) && swflg>0) {
              lp += 4;
              SCRIPT_SKIP_SPACES
              TS_FLOAT cvar;
              if (!(swflg & 0x80)) {
                lp = GetNumericArgument(lp, OPER_EQU, &cvar, 0);
                if (swvar != cvar) {
                  swflg = 2;
                } else {
                  swflg = 1;
                }
              } else {
                char str[SCRIPT_MAXSSIZE];
                lp = GetStringArgument(lp, OPER_EQU, str, 0);
                if (!strcmp(cmpstr, str)) {
                    swflg = 0x81;
                } else {
                  swflg = 0x82;
                }
              }
            } else if (!strncmp(lp, "ends", 4) && swflg > 0) {
              lp += 4;
              swflg = 0;
            }
            if ((swflg & 3) == 2) goto next_line;

            SCRIPT_SKIP_SPACES
            //SCRIPT_SKIP_EOL
            if (*lp == SCRIPT_EOL) {
             goto next_line;
            }

            //toLogN(lp,16);
            if (!if_exe[ifstck] && if_state[ifstck] != 1) goto next_line;

#ifdef IFTHEN_DEBUG
            sprintf(tbuff, "stack=%d,exe=%d,state=%d,cmpres=%d execute line: ", ifstck, if_exe[ifstck], if_state[ifstck], if_result[ifstck]);
            toLogEOL(tbuff, lp);
#endif //IFTHEN_DEBUG

            if (!strncmp(lp, "return", 6)) {
              lp += 6;
              SCRIPT_SKIP_SPACES
              if (*lp == SCRIPT_EOL) {
                goto next_line;
              }
              glob_script_mem.glob_error = 0;
              char *slp = lp;
              lp = GetNumericArgument(lp, OPER_EQU, &glob_script_mem.retval, 0);
              if (glob_script_mem.glob_error == 1) {
                // mismatch was string, not number
                lp = slp;
                glob_script_mem.glob_error = 0;
                glob_script_mem.retstr = (char*)calloc(SCRIPT_MAXSSIZE, 1);
                if (glob_script_mem.retstr) {
                  lp = GetStringArgument(lp, OPER_EQU, glob_script_mem.retstr, 0);
                }
              } else {
                glob_script_mem.retstr = 0;
              }
              section = 99;
              goto next_line;
            } else if (!strncmp(lp, "break", 5)) {
              lp += 5;
              if (loopdepth >= 0) {
                if (floop[loopdepth] ) {
                  // should break loop
                  if (lp_next[loopdepth]) {
                    lp = lp_next[loopdepth];
                  }
                  floop[loopdepth]  = 0;
                  goto getnext;
                }
              } else {
                section = 99;
                // leave immediately
                goto next_line;
              }

            } else if (!strncmp(lp, "dp", 2) && isdigit(*(lp + 2))) {
              lp += 2;
              // number precision
              if (*(lp + 1) == '.' || *(lp + 1) == ',' ) {
                glob_script_mem.script_sepc = *(lp + 1);
                glob_script_mem.script_lzero = atoi(lp);
                lp += 2;
              }
              glob_script_mem.script_dprec = atoi(lp);
              goto next_line;
            }
#ifdef USE_DISPLAY
            else if (!strncmp(lp, "dt ", 3)) {
              //char dstbuf[256];
              lp += 2;
              SCRIPT_SKIP_SPACES
              char *dstbuf = (char*)malloc(256);
              if (dstbuf) {
                Replace_Cmd_Vars(lp, 1, dstbuf, 256);
                char *savptr = XdrvMailbox.data;
                XdrvMailbox.data = dstbuf;
                XdrvMailbox.data_len = 0;
                DisplayText();
                XdrvMailbox.data = savptr;
                free(dstbuf);
              }
              goto next_line;
            }
#endif //USE_DISPLAY
            else if (!strncmp(lp, "delay(", 6)) {
              // delay
              lp = GetNumericArgument(lp + 5, OPER_EQU, &fvar, 0);
              delay(fvar);
              goto next_line;
            } else if (!strncmp(lp, "spinm(", 6)) {
              // set pin mode
              lp = GetNumericArgument(lp + 6, OPER_EQU, &fvar, 0);
              int8_t pinnr = fvar;
              if (Is_gpio_used(pinnr)) {
                AddLog(LOG_LEVEL_INFO, PSTR("SCR: warning, pins already used"));
              }
              SCRIPT_SKIP_SPACES
              uint8_t mode = 0;
              if ((*lp == 'I') || (*lp == 'O') || (*lp == 'P')) {
                switch (*lp) {
                  case 'I':
                    mode = 0;
                    break;
                  case 'O':
                    mode = 1;
                    break;
                  case 'P':
                    mode = 2;
                    break;
                }
                lp++;
              } else {
                lp = GetNumericArgument(lp, OPER_EQU, &fvar, 0);
                mode = fvar;
              }
              uint8_t pm = 0;
              if (mode == 0) pm = INPUT;
              if (mode == 1) pm = OUTPUT;
              if (mode == 2) pm = INPUT_PULLUP;
              pinMode(pinnr, pm);
              goto next_line;
            } else if (!strncmp(lp, "spin(", 5)) {
              // set pin
              lp = GetNumericArgument(lp + 5, OPER_EQU, &fvar, 0);
              int8_t pinnr = fvar;
              SCRIPT_SKIP_SPACES
              lp = GetNumericArgument(lp, OPER_EQU, &fvar, 0);
              int8_t mode = fvar;
              digitalWrite(pinnr, mode & 1);
              goto next_line;
            } else if (!strncmp(lp, "svars", 5)) {
              lp += 5;
              // save vars
              Scripter_save_pvars();
              goto next_line;
            }
#ifdef USE_SCRIPT_GLOBVARS
            else if (!strncmp(lp, "gvr", 3)) {
              lp += 3;
              // reset global vars udp server
              Restart_globvars();
              goto next_line;
            }
#endif // USE_SCRIPT_GLOBVARS
#ifdef USE_LIGHT
#ifdef USE_WS2812
            else if (!strncmp(lp, "ws2812(", 7)) {
              lp = isvar(lp + 7, &vtype, &ind, 0, 0, gv);
              if (vtype != VAR_NV) {
                SCRIPT_SKIP_SPACES
                if (*lp != ')') {
                  lp = GetNumericArgument(lp, OPER_EQU, &fvar, 0);
                } else {
                  fvar = 0;
                }
                // found variable as result
                uint8_t index = glob_script_mem.type[ind.index].index;
                if ((vtype & STYPE) == 0) {
                    // numeric result
                  if (glob_script_mem.type[ind.index].bits.is_filter) {
                    uint16_t len = 0;
                    TS_FLOAT *fa = Get_MFAddr(index, &len, 0);
                    //Serial.printf(">> 2 %d\n",(uint32_t)*fa);
                    if (fa && len) ws2812_set_array(fa, len, fvar);
                  }
                }
              }
              goto next_line;
            }
#endif //USE_WS2812
#endif //USE_LIGHT
#ifdef ESP32
            else if (!strncmp(lp, "beep(", 5)) {
              lp = GetNumericArgument(lp + 5, OPER_EQU, &fvar, 0);
              SCRIPT_SKIP_SPACES
              TS_FLOAT fvar1;
              lp = GetNumericArgument(lp, OPER_EQU, &fvar1, 0);
              esp32_beep(fvar, fvar1);
              lp++;
              goto next_line;
            }
#endif //ESP32

            else if (!strncmp(lp, "pwm", 3) && lp[4] == '(') {
              lp += 3;
              uint8_t channel = *lp & 0x0f;
#ifdef ESP8266
              if (channel > 5) {channel = 5;}
#endif // ESP8266
#ifdef ESP32
              if (channel > 8) {channel = 8;}
#endif // ESP32
              if (channel < 1) {channel = 1;}
              lp += 2;
              lp = GetNumericArgument(lp, OPER_EQU, &fvar, 0);
              SCRIPT_SKIP_SPACES
              TS_FLOAT fvar1 = 4000;
              if (*lp != ')') {
                lp = GetNumericArgument(lp, OPER_EQU, &fvar1, 0);
              }
              esp_pwm(fvar, fvar1, channel);
              lp++;
              goto next_line;
            }
#ifdef USE_SCRIPT_WEB_DISPLAY
            else if (!strncmp(lp, "wcs ", 4)) {
              lp += 4;
              // skip one space after cmd
              web_send_line(0, lp);
              //WSContentFlush();
              goto next_line;
            }
            else if (!strncmp(lp, "wfs ", 4)) {
              lp += 4;
              // skip one space after cmd
              web_send_file(0, lp);
              //WSContentFlush();
              goto next_line;
            }
            else if (!strncmp(lp, "wcf\n", 4)) {
              WSContentFlush();
              goto next_line;
            }
#endif
            else if (!strncmp(lp, "rapp ", 4)) {
              lp += 4;
              // skip one space after cmd
              char *tmp = (char*)malloc(256);
              if (tmp) {
                Replace_Cmd_Vars(lp ,1 , tmp, 256);
                ResponseAppend_P(PSTR("%s"), tmp);
                free(tmp);
              }
              goto next_line;
            }


#if defined(USE_SENDMAIL) || defined(USE_ESP32MAIL)
            else if (!strncmp(lp, "mail ", 5)) {
              lp += 5;
              //char tmp[256];
              char *tmp = (char*)malloc(256);
              if (tmp) {
                Replace_Cmd_Vars(lp ,1 , tmp, 256);
                SendMail(tmp);
                free(tmp);
              }
              goto next_line;
            }
#endif
            else if (!strncmp(lp, "=>", 2) || !strncmp(lp, "->", 2) || !strncmp(lp, "+>", 2) || !strncmp(lp, "print", 5)) {
                // execute cmd
                uint8_t sflag = 0,pflg = 0,svmqtt,swll;
                if (*lp == 'p') {
                 pflg = 1;
                 lp += 5;
                }
                else {
                  if (*lp == '-') sflag = 1;
                  if (*lp == '+') sflag = 2;
                  lp += 2;
                }
                char *slp = lp;
                SCRIPT_SKIP_SPACES
                char *cmdmem = (char*)malloc(glob_script_mem.cmdbuffer_size * 2);
                if (cmdmem) {
                  char *cmd = cmdmem;
                  uint16_t count;
                  for (count = 0; count < glob_script_mem.cmdbuffer_size-2; count++) {
                    //if (*lp=='\r' || *lp=='\n' || *lp=='}') {
                    if (!*lp || *lp == '\r' || *lp == '\n') {
                        cmd[count] = 0;
                        break;
                    }
                    cmd[count] = *lp++;
                  }
                  //AddLog(LOG_LEVEL_INFO, tmp);
                  // replace vars in cmd
                  char *tmp = cmdmem + glob_script_mem.cmdbuffer_size;
                  Replace_Cmd_Vars(cmd, 0, tmp, glob_script_mem.cmdbuffer_size);
                  //toSLog(tmp);

                  if (!strncmp(tmp, "print", 5) || pflg) {
                    if (pflg) toLog(tmp);
                    else toLog(&tmp[5]);
                  } else {
                    if (!sflag) {
                      tasm_cmd_activ = 1;
                      AddLog(glob_script_mem.script_loglevel&0x7f, PSTR("SCR: performs \"%s\""), tmp);
                    } else if (sflag == 2) {
                      // allow recursive call
                    } else {
                      tasm_cmd_activ = 1;
                      svmqtt = Settings->flag.mqtt_enabled;  // SetOption3 - Enable MQTT
                      swll = Settings->weblog_level;
                      Settings->flag.mqtt_enabled = 0;       // SetOption3 - Enable MQTT
                      Settings->weblog_level = 0;
                    }
                    ExecuteCommand((char*)tmp, SRC_RULE);
                    tasm_cmd_activ = 0;
                    if (sflag == 1) {
                      Settings->flag.mqtt_enabled = svmqtt;  // SetOption3  - Enable MQTT
                      Settings->weblog_level = swll;
                    }
                  }
                  if (cmdmem) free(cmdmem);
                }
                lp = slp;
                goto next_line;
            } else if (!strncmp(lp, "=#", 2)) {
                // subroutine
                lp = scripter_sub(lp, fromscriptcmd);
                goto next_line;
            } else if (!strncmp(lp, "=(", 2)) {
                lp += 2;
                char str[128];
                str[0] = '>';
                lp = GetStringArgument(lp, OPER_EQU, &str[1], 0);
                lp++;
                //execute_script(str);
                char *svd_sp = glob_script_mem.scriptptr;
                strcat(str, "\n#");
                glob_script_mem.scriptptr = str;
                Run_script_sub(">", 1, gv);
                glob_script_mem.scriptptr = svd_sp;
            }

            // check for variable result
            if (if_state[ifstck] == 1) {
              // evaluate exxpression
              lp = Evaluate_expression(lp, and_or, &if_result[ifstck], gv);
              SCRIPT_SKIP_SPACES
              if (*lp == '{' && if_state[ifstck] == 1) {
                lp += 1; // then
                if_state[ifstck] = 2;
                if (if_exe[ifstck - 1]) if_exe[ifstck] = if_result[ifstck];
              }
              goto next_line;
            } else {
              char *vnp = lp;
              lp = isvar(lp, &vtype, &ind, &sysvar, 0, gv);
              if (vtype != VAR_NV) {
#ifdef USE_SCRIPT_GLOBVARS
                  char varname[16];
                  uint32_t vnl = (uint32_t)lp - (uint32)vnp;
                  strncpy(varname, vnp, vnl);
                  varname[vnl] = 0;
#endif //USE_SCRIPT_GLOBVARS

                  // found variable as result
                  globvindex = ind.index; // save destination var index here
                  if (gv) globaindex = gv->numind;
                  else globaindex = -1;
                  uint8_t index = glob_script_mem.type[ind.index].index;
                  if ((vtype & STYPE) == 0) {
                      // numeric result
                      if (ind.bits.settable || ind.bits.is_filter) {
                        dfvar = &sysvar;
                        if (ind.bits.settable) {
                          sysv_type = ind.index;
                          globvindex = -1;
                        } else {
                          sysv_type = 0;
                        }
                      } else {
                        dfvar = &glob_script_mem.fvars[index];
                        sysv_type = 0;
                        glob_script_mem.res_ivar = ind.bits.integer;
                      }
                      numeric = 1;
                      SCRIPT_SKIP_SPACES
                      lp = getop(lp, &lastop);
#ifdef SCRIPT_LM_SUB
                      if (*lp == '#') {
                        // subroutine
                        lp = eval_sub(lp, &fvar, 0);
                      } else {
#endif
                        SCRIPT_SKIP_SPACES
                        if ( (glob_script_mem.arres > 0) && (lastop == OPER_EQU) && (*lp == '{') ) {
                          glob_script_mem.arres = 2;
                        } else {
                          char *slp = lp;
                          glob_script_mem.glob_error = 0;
                          //Serial.printf("Stack 1: %d\n",GetStack());
                          lp = GetNumericArgument(lp, OPER_EQU, &fvar, gv);
                          if (glob_script_mem.glob_error == 1) {
                            // mismatch was string, not number
                            // get the string and convert to number
                            lp = isvar(slp, &vtype, &ind, 0, cmpstr, gv);
                            fvar = CharToFloat(cmpstr);
                          }
                        }
#ifdef SCRIPT_LM_SUB
                      }
#endif
                      if (ind.bits.integer) {
                        switch (lastop) {
                          case OPER_EQU:
                              if (glob_script_mem.var_not_found) {
                                if (!gv || !gv->jo) toLogEOL("var not found: ",lp);
                                goto next_line;
                              }
                              *dfvar = fvar;
                              break;
                          case OPER_PLSEQU:
                              *(int32_t*)dfvar += *(int32_t*)&fvar;
                              break;
                          case OPER_MINEQU:
                              *(int32_t*)dfvar -= *(int32_t*)&fvar;
                              break;
                          case OPER_MULEQU:
                              *(int32_t*)dfvar *= *(int32_t*)&fvar;
                              break;
                          case OPER_DIVEQU:
                              *(int32_t*)dfvar /= *(int32_t*)&fvar;
                              break;
                          case OPER_PERCEQU:
                              *(int32_t*)dfvar %= *(int32_t*)&fvar;
                              break;
                          case OPER_ANDEQU:
                              *(uint32_t*)dfvar &= *(int32_t*)&fvar;
                              break;
                          case OPER_OREQU:
                              *(uint32_t*)dfvar |= *(int32_t*)&fvar;
                              break;
                          case OPER_XOREQU:
                              *(uint32_t*)dfvar ^= *(int32_t*)&fvar;
                              break;
                          case OPER_SHLEQU:
                              *(uint32_t*)dfvar <<= *(int32_t*)&fvar;
                              break;
                          case OPER_SHREQU:
                              *(uint32_t*)dfvar >>= *(int32_t*)&fvar;
                              break;
                          default:
                              // error
                              break;
                        }
                      } else {
                        switch (lastop) {
                          case OPER_EQU:
                              if (glob_script_mem.var_not_found) {
                                if (!gv || !gv->jo) toLogEOL("var not found: ",lp);
                                goto next_line;
                              }
                              *dfvar = fvar;
                              break;
                          case OPER_PLSEQU:
                              *dfvar += fvar;
                              break;
                          case OPER_MINEQU:
                              *dfvar -= fvar;
                              break;
                          case OPER_MULEQU:
                              *dfvar *= fvar;
                              break;
                          case OPER_DIVEQU:
                              *dfvar /= fvar;
                              break;
                          case OPER_PERCEQU:
                              *dfvar = fmodf(*dfvar, fvar);
                              break;
                          case OPER_ANDEQU:
                              *dfvar = (uint32_t)*dfvar & (uint32_t)fvar;
                              break;
                          case OPER_OREQU:
                              *dfvar = (uint32_t)*dfvar | (uint32_t)fvar;
                              break;
                          case OPER_XOREQU:
                              *dfvar = (uint32_t)*dfvar ^ (uint32_t)fvar;
                              break;
                          case OPER_SHLEQU:
                              *dfvar = (uint32_t)*dfvar << (uint32_t)fvar;
                              break;
                          case OPER_SHREQU:
                              *dfvar = (uint32_t)*dfvar >> (uint32_t)fvar;
                              break;
                          default:
                              // error
                              break;
                        }
                      }

                      // var was changed
                      if (globvindex >= 0) SetChanged(globvindex);
#ifdef USE_SCRIPT_GLOBVARS
                      if (globvindex >= 0 ) {
                        if (glob_script_mem.type[globvindex].bits.global) {
                          script_udp_sendvar(varname, dfvar, 0);
                        }
                      }
#endif //USE_SCRIPT_GLOBVARS
                      if (globvindex >= 0) {
                        if (glob_script_mem.type[globvindex].bits.is_filter) {
                          if (globaindex >= 0) {
                            Set_MFVal(glob_script_mem.type[globvindex].index, globaindex, *dfvar);
                          } else {
                            if (glob_script_mem.arres == 2) {
                              // fetch var preset
                              lp++;
                              while (*lp && *lp != SCRIPT_EOL) {
                                if (*lp == '}') {
                                  lp++;
                                  break;
                                }
                                lp = GetNumericArgument(lp, OPER_EQU, &fvar, gv);
                                Set_MFilter(glob_script_mem.type[globvindex].index, fvar);
                              }
                            }  else {
                              Set_MFilter(glob_script_mem.type[globvindex].index, *dfvar);
                            }
                          }
                        }
                      }
                      if (sysv_type) {
                        switch (sysv_type) {
                          case SCRIPT_LOGLEVEL:
                            glob_script_mem.script_loglevel = *dfvar;
                            break;
                          case SCRIPT_TELEPERIOD:
                            if (*dfvar < 10) *dfvar = 10;
                            if (*dfvar > 300) *dfvar = 300;
                            Settings->tele_period = *dfvar;
                            break;
                          case SCRIPT_EVENT_HANDLED:
                            glob_script_mem.event_handeled = *dfvar;
                            break;
                          case SCRIPT_EPOFFS:
                            glob_script_mem.epoch_offset = *dfvar;
                            break;
                          case SCRIPT_CBSIZE:
                            if (*dfvar > MAX_SCRIPT_CMDBUFFER) {
                              *dfvar = MAX_SCRIPT_CMDBUFFER;
                            }
                            glob_script_mem.cmdbuffer_size = *dfvar;
                            break;
#if defined(USE_SML_M) && defined (USE_SML_SCRIPT_CMD)
                          case SML_JSON_ENABLE:
                            sml_options = *dfvar;
                            break;
#endif
                        }
                        sysv_type = 0;
                      }
                  } else {
                    // string result
                    numeric = 0;
                    sindex = index;
                    saindex = gv->strind;
                    // string result
                    char str[SCRIPT_MAXSSIZE];
                    SCRIPT_SKIP_SPACES
                    lp = getop(lp, &lastop);
#ifdef SCRIPT_LM_SUB
                    if (*lp == '#') {
                      // subroutine
                      lp = eval_sub(lp, 0, str);
                    } else {
#endif
                      char *slp = lp;
                      glob_script_mem.glob_error = 0;
                      lp = GetStringArgument(lp, OPER_EQU, str, gv);
                      if ((!gv || !gv->jo) && glob_script_mem.glob_error) {
                        // mismatch
                        lp = GetNumericArgument(slp, OPER_EQU, &fvar, 0);
                        dtostrfd(fvar, 6, str);
                        glob_script_mem.glob_error = 0;
                      }
#ifdef SCRIPT_LM_SUB
                    }
#endif
                    if (!glob_script_mem.var_not_found) {
                      // var was changed
                      if (globvindex >= 0) SetChanged(globvindex);
#ifdef USE_SCRIPT_GLOBVARS
                      if (globvindex >= 0) {
                        if (glob_script_mem.type[globvindex].bits.global) {
                          script_udp_sendvar(varname, 0, str);
                        }
                      }
#endif //USE_SCRIPT_GLOBVARS
                      if (saindex >= 0) {
                        if (lastop == OPER_EQU) {
                          strlcpy(glob_script_mem.last_index_string[glob_script_mem.sind_num] + (saindex * glob_script_mem.max_ssize), str, glob_script_mem.max_ssize);
                        } else if (lastop == OPER_PLSEQU) {
                          strncat(glob_script_mem.last_index_string[glob_script_mem.sind_num] + (saindex * glob_script_mem.max_ssize), str, glob_script_mem.max_ssize);
                        }
                        gv->strind = -1;
                      } else {
                        if (lastop == OPER_EQU) {
                          strlcpy(glob_script_mem.glob_snp + (sindex * glob_script_mem.max_ssize), str, glob_script_mem.max_ssize);
                        } else if (lastop == OPER_PLSEQU) {
                          strncat(glob_script_mem.glob_snp + (sindex * glob_script_mem.max_ssize), str, glob_script_mem.max_ssize);
                        }
                      }
                    }
                  }

            }
            SCRIPT_SKIP_SPACES
            if (*lp == '{' && if_state[ifstck] == 3) {
              lp += 1; // else
              //if_state[ifstck]=3;
            }
            goto next_line;
          }
        } else {
            //Serial.printf(">> decode %s\n",lp );
            // decode line
            if (*lp == '>' && tlen == 1) {
              // called from cmdline
              lp++;
              section = 1;
              fromscriptcmd = 1;
              goto startline;
            }
            if (!strncmp(lp, type, tlen)) {
                // found section
                section = 1;
                glob_script_mem.section_ptr = lp;
                if (check) {
                  return 99;
                }
                // check for subroutine
                char *ctype = (char*)type;
                if (*ctype == '#') {
                  // check for parameter
                  ctype += tlen;
                  char nxttok = '(';
                  char *argptr = ctype + tlen;
                  lp += tlen;
                  do {
                    if (*ctype == nxttok && *lp == nxttok) {
                      TS_FLOAT fparam;
                      numeric = 1;
                      glob_script_mem.glob_error = 0;
                      argptr = GetNumericArgument((char*)ctype + 1, OPER_EQU, &fparam, 0);
                      if (glob_script_mem.glob_error == 1) {
                        // was string, not number
                        numeric = 0;
                        // get the string
                        argptr = GetStringArgument((char*)ctype + 1, OPER_EQU, cmpstr, 0);
                      }
                      if (*lp == nxttok) {
                        // fetch destination
                        lp++;
                        lp = isvar(lp, &vtype, &ind, 0, 0, gv);
                        if (vtype != VAR_NV) {
                          // found variable as result
                          uint8_t index = glob_script_mem.type[ind.index].index;
                          if ((vtype & STYPE) == 0) {
                              // numeric result
                              dfvar = &glob_script_mem.fvars[index];
                              if (numeric) {
                                *dfvar = fparam;
                              } else {
                                // mismatch
                                *dfvar = CharToFloat(cmpstr);
                              }
                          } else {
                              // string result
                              sindex = index;
                              if (!numeric) {
                                strlcpy(glob_script_mem.glob_snp + (sindex * glob_script_mem.max_ssize), cmpstr, glob_script_mem.max_ssize);
                              } else {
                                // mismatch
                                dtostrfd(fparam, 6, glob_script_mem.glob_snp + (sindex * glob_script_mem.max_ssize));
                              }
                          }
                        }
                      }
                    } else {
                      if (*ctype == nxttok || (*lp != SCRIPT_EOL && *lp != '?')) {
                        // revert
                        section = 0;
                      }
                    }
                    nxttok = ' ';
                    ctype = argptr;
                  } while (*lp == ' ' && (section == 1) );
                }
            }
        }
        // next line
    next_line:
        if (section == 99) return 0;
        if (*lp == SCRIPT_EOL) {
          lp++;
        } else {
          lp = strchr(lp, SCRIPT_EOL);
          if (!lp) {
            if (section) {
              return 0;
            } else {
              return -1;
            }
          }
          lp++;
        }
    }
    return -1;
}

#ifdef USE_SCRIPT_ONEWIRE

bool script_OneWireCrc8(uint8_t *addr) {
  uint8_t crc = 0;
  uint8_t len = 8;

  while (len--) {
    uint8_t inbyte = *addr++;          // from 0 to 7
    for (uint32_t i = 8; i; i--) {
      uint8_t mix = (crc ^ inbyte) & 0x01;
      crc >>= 1;
      if (mix) {
        crc ^= 0x8C;
      }
      inbyte >>= 1;
    }
  }
  return (crc == *addr);               // addr 8
}

uint32_t script_ow(uint8_t sel, uint32_t val) {
uint32_t res = 0;
uint8_t bits;
bool invert = false;
ScriptOneWire *ow = &glob_script_mem.ow;

  if (sel >= 10 && sel <= 18) {
      if (val < 1 || val > MAX_DS_SENSORS) {
        val = 1;
      }
    return ow->ds_address[val - 1][sel - 10];
  }

  if (sel > 0 && (ow->ds == nullptr && ow->dsh == nullptr)) {
    return 0xffff;
  }

  switch (sel) {
    case 0:
      if (val & 0x8000) {
        if (val & 0x10000) {
          // inverted serial
          invert = true;
        }
        val &= 0x7fff;
        ow->ts = new TasmotaSerial(val & 0xff, (val >> 8) & 0x7f, 1, 0, 64);
        if (ow->ts) {

          ow->ts->begin(9600);

#ifdef ESP8266
          if (ow->ts->hardwareSerial()) {
            ClaimSerial();
#ifdef ALLOW_OW_INVERT
            if (invert == true) {
              U0C0 = U0C0 | BIT(UCRXI) | BIT(UCTXI); // Inverse RX, TX
            }
#endif
          }
#endif // ESP8266

#ifdef ESP32
#ifdef ALLOW_OW_INVERT
          if (invert == true) {
            HardwareSerial *hws = ow->ts->getesp32hws();
            hws->end();
            hws->begin(9600, SERIAL_8N1, val & 0xff, (val >> 8) & 0x7f, true);
          }
#endif
#endif // ESP32

          ow->dsh = new DS2480B(ow->ts);
          ow->dsh->begin();
        }
        ow->ds = nullptr;
      } else {
        ow->ds = new OneWire(val);
        ow->dsh = nullptr;
      }
      break;
    case 1:
      if (ow->ds) {
        ow->ds->reset();
      } else {
        ow->dsh->reset();
      }
      break;
    case 2:
      if (ow->ds) {
        ow->ds->skip();
      } else {
        ow->dsh->skip();
      }
      break;
    case 3:
      if (ow->ds) {
        ow->ds->write(val, 1);
      } else {
        ow->dsh->write(val, 1);
      }
      break;
    case 4:
      if (ow->ds) {
        return ow->ds->read();
      } else {
        return ow->dsh->read();
      }
      break;
    case 5:
      if (ow->ds) {
        ow->ds->reset_search();
      } else {
        ow->dsh->reset_search();
      }
      break;
    case 6:
      if (val < 1 || val > MAX_DS_SENSORS) {
        val = 1;
      }
      if (ow->ds) {
        return ow->ds->search(ow->ds_address[val - 1]);
      } else {
        return ow->dsh->search(ow->ds_address[val - 1]);
      }
      break;
    case 7:
      if (val < 1 || val > MAX_DS_SENSORS) {
        val = 1;
      }
      if (ow->ds) {
        ow->ds->select(ow->ds_address[val - 1]);
      } else {
        ow->dsh->select(ow->ds_address[val - 1]);
      }
      break;
    case 8:
      bits = val & 0xc0;
      val &= 0x3f;
      if (val < 1 || val > MAX_DS_SENSORS) {
        val = 1;
      }
      if (ow->ds) {
        ow->ds->reset();
        ow->ds->select(ow->ds_address[val - 1]);
        ow->ds->write(0xf5, 1);
        ow->ds->write(0x0c, 1);
        ow->ds->write(0xff, 1);
        res = ow->ds->read();
        ow->ds->write(bits, 1);
      } else {
        ow->dsh->reset();
        ow->dsh->select(ow->ds_address[val - 1]);
        ow->dsh->write(0xf5, 1);
        ow->dsh->write(0x0c, 1);
        ow->dsh->write(0xff, 1);
        res = ow->dsh->read();
        ow->dsh->write(bits, 1);
      }
      break;
    case 9:
      bits = val & 0x80;
      val &= 0x3f;
      if (val < 1 || val > MAX_DS_SENSORS) {
        val = 1;
      }

      if (ow->ds) {
        uint8_t data[9];
        ow->ds->reset();
        ow->ds->select(ow->ds_address[val - 1]);
        if (!bits) {
          ow->ds->write(0x44, 1);
        } else {
          ow->ds->write(0xbe, 1);
          for (uint32_t cnt = 0; cnt < 9; cnt++) {
            data[cnt] = ow->ds->read();
          }
          if (script_OneWireCrc8(data)) {
            res = data[0];
            res |= data[1] << 8;
          } else {
            res = 0;
          }
          ow->ds->reset();
        }
      } else {
        uint8_t data[9];
        ow->dsh->reset();
        ow->dsh->select(ow->ds_address[val - 1]);
        if (!bits) {
          ow->dsh->write(0x44, 1);
        } else {
          ow->dsh->write(0xbe, 1);
          for (uint32_t cnt = 0; cnt < 9; cnt++) {
            data[cnt] = ow->dsh->read();
          }
          if (script_OneWireCrc8(data)) {
            res = data[0];
            res |= data[1] << 8;
          } else {
            res = 0;
          }
          ow->dsh->reset();
        }
      }
      break;
    case 99:
      if (ow->ds) {
        ow->ds->reset();
        delete ow->ds;
        ow->ds = nullptr;
      } else {
        ow->dsh->reset();
        delete ow->dsh;
        ow->dsh = nullptr;
        delete ow->ts;
      }
      break;
    case 98:
      ow->ts->write(val);
      break;
  }
  return res;
}
#endif // USE_SCRIPT_ONEWIRE



#ifdef USE_SCRIPT_SPI
// transfer 1-3 bytes
uint32_t script_sspi_trans(int32_t cs_index, TS_FLOAT *array, uint32_t len, uint32_t size) {
  uint32_t out = 0;
  if (cs_index >= 0) {
    cs_index &= 3;
    digitalWrite(glob_script_mem.spi.cs[cs_index], 0);
  }

  if (glob_script_mem.spi.sclk < 0) {
    // use existing hardware spi
    glob_script_mem.spi.spip->beginTransaction(glob_script_mem.spi.settings);
    for (uint32_t cnt = 0; cnt < len; cnt++) {
      if (size == 1) {
        out = glob_script_mem.spi.spip->transfer((uint8_t)*array);
      }
      if (size == 2) {
        out = glob_script_mem.spi.spip->transfer16((uint16_t)*array);
      }
      if (size == 3) {
        out = glob_script_mem.spi.spip->transfer((uint32_t)*array >> 16);
        out <<= 16;
        out |= glob_script_mem.spi.spip->transfer16((uint32_t)*array);
      }
      if (size == 4) {
        // special byte transfer with cs low
        digitalWrite(glob_script_mem.spi.cs[cs_index], 0);
        out = glob_script_mem.spi.spip->transfer((uint8_t)*array);
        digitalWrite(glob_script_mem.spi.cs[cs_index], 1);
      }
      *array++ = out;
    }
    glob_script_mem.spi.spip->endTransaction();

  } else {
    if (size == 4) {
      for (uint32_t cnt = 0; cnt < len; cnt++) {
        digitalWrite(glob_script_mem.spi.cs[cs_index], 0);
        uint32_t bit = 1 << ((1 * 8) - 1);
        out = 0;
        uint32_t uvar = *array;
        while (bit) {
          digitalWrite(glob_script_mem.spi.sclk, 0);
          if (glob_script_mem.spi.mosi >= 0) {
            if (uvar & bit) digitalWrite(glob_script_mem.spi.mosi, 1);
            else   digitalWrite(glob_script_mem.spi.mosi, 0);
          }
          digitalWrite(glob_script_mem.spi.sclk, 1);
          if (glob_script_mem.spi.miso >= 0) {
            if (digitalRead(glob_script_mem.spi.miso)) {
              out |= bit;
            }
          }
          bit >>= 1;
        }
        *array++ = out;
        digitalWrite(glob_script_mem.spi.cs[cs_index], 1);
      }
    } else {
      if (size < 1 || size > 3) size = 1;
      for (uint32_t cnt = 0; cnt < len; cnt++) {
        uint32_t bit = 1 << ((size * 8) - 1);
        out = 0;
        uint32_t uvar = *array;
        while (bit) {
          digitalWrite(glob_script_mem.spi.sclk, 0);
          if (glob_script_mem.spi.mosi >= 0) {
            if (uvar & bit) digitalWrite(glob_script_mem.spi.mosi, 1);
            else   digitalWrite(glob_script_mem.spi.mosi, 0);
          }
          digitalWrite(glob_script_mem.spi.sclk, 1);
          if (glob_script_mem.spi.miso >= 0) {
            if (digitalRead(glob_script_mem.spi.miso)) {
              out |= bit;
            }
          }
          bit >>= 1;
        }
        *array++ = out;
      }
    }
  }
  if (cs_index >= 0) {
    digitalWrite(glob_script_mem.spi.cs[cs_index], 1);
  }
  return len;
}
#endif // USE_SCRIPT_SPI

#ifdef USE_SCRIPT_SERIAL
bool Script_Close_Serial() {
  if (glob_script_mem.sp) {
    glob_script_mem.sp->flush();
    delay(100);
    delete(glob_script_mem.sp);
    glob_script_mem.sp = 0;
    return true;
  }
  return false;
}
#endif //USE_SCRIPT_SERIAL


void script_sort_array(TS_FLOAT *array, uint16_t size) {
  bool swapped;
  do {
    swapped = false;
    for (uint16_t i = 0; i < size - 1; ++i) {
      if (array[i] > array[i + 1]) {
        // swap
        TS_FLOAT tmp = array[i];
        array[i] = array[i + 1];
        array[i + 1] = tmp;
        swapped = true;
      }
    }
    size -= 1;
  } while (swapped);
}


bool Is_gpio_used(uint8_t gpiopin) {
  if (gpiopin >= 0 && (gpiopin < nitems(TasmotaGlobal.gpio_pin)) && (TasmotaGlobal.gpio_pin[gpiopin] > 0)) {
    return true;
  }
  return false;
}

void ScripterEvery100ms(void) {
  static uint8_t xsns_index = 0;

  if (bitRead(Settings->rule_enabled, 0) && (TasmotaGlobal.uptime > 4)) {
    ResponseClear();
    uint16_t script_tele_period_save = TasmotaGlobal.tele_period;
    TasmotaGlobal.tele_period = 2;
    XsnsNextCall(FUNC_JSON_APPEND, xsns_index);
    TasmotaGlobal.tele_period = script_tele_period_save;
    if (ResponseLength()) {
      ResponseJsonStart();
      ResponseJsonEnd();
      //Run_Scripter(">T", 2, ResponseData());
      if (glob_script_mem.teleperiod) Run_Scripter(glob_script_mem.teleperiod, 0, ResponseData());
    }
  }
  if (bitRead(Settings->rule_enabled, 0)) {
    if (glob_script_mem.fast_script) Run_Scripter1(glob_script_mem.fast_script, 0, 0);
  }
}

//mems[5] is 50 bytes in 6.5
// can hold 11 floats or floats + strings
// should report overflow later
void Scripter_save_pvars(void) {
  int16_t mlen = 0;
  TS_FLOAT *fp = (TS_FLOAT*)glob_script_mem.script_pram;
  mlen+=sizeof(TS_FLOAT);
  struct T_INDEX *vtp = glob_script_mem.type;
  for (uint8_t count = 0; count<glob_script_mem.numvars; count++) {
    if (vtp[count].bits.is_permanent && !vtp[count].bits.is_string) {
      uint8_t index = vtp[count].index;
      if (vtp[count].bits.is_filter) {
        // save array
        uint16_t len = 0;
        TS_FLOAT *fa = Get_MFAddr(index, &len, 0);
        mlen += sizeof(TS_FLOAT) * len;
        if (mlen>glob_script_mem.script_pram_size) {
          vtp[count].bits.is_permanent = 0;
          return;
        }
        while (len--) {
          *fp++ = *fa++;
        }
      } else {
        mlen += sizeof(TS_FLOAT);
        if (mlen>glob_script_mem.script_pram_size) {
          vtp[count].bits.is_permanent = 0;
          return;
        }
        *fp++ = glob_script_mem.fvars[index];
      }
    }
  }
  char *cp = (char*)fp;
  for (uint8_t count = 0; count<glob_script_mem.numvars; count++) {
    if (vtp[count].bits.is_permanent && vtp[count].bits.is_string) {
      uint8_t index = vtp[count].index;
      char *sp = glob_script_mem.glob_snp + (index * glob_script_mem.max_ssize);
      uint8_t slen = strlen(sp);
      mlen += slen + 1;
      if (mlen>glob_script_mem.script_pram_size) {
        vtp[count].bits.is_permanent = 0;
        return;
      }
      strcpy(cp, sp);
      cp += slen + 1;
    }
  }
}

// works only with webserver
#ifdef USE_WEBSERVER

#define WEB_HANDLE_SCRIPT "s10"

const char HTTP_BTN_MENU_RULES[] PROGMEM =
  "<p><form action='" WEB_HANDLE_SCRIPT "' method='get'><button>" D_CONFIGURE_SCRIPT "</button></form></p>";


const char HTTP_FORM_SCRIPT[] PROGMEM =
    "<fieldset><legend><b>&nbsp;" D_SCRIPT "&nbsp;</b></legend>"
    "<form method='post' action='" WEB_HANDLE_SCRIPT "'>";

const char HTTP_FORM_SCRIPT1[] PROGMEM =
    "<div style='text-align:right' id='charNum'> </div>"
    "<label><input style='width:3%%;' id='c%d' name='c%d' type='checkbox'%s><b>" D_SCRIPT_ENABLE "</b></label><br/>"
    "<br><textarea  id='t1' name='t1' rows='8' cols='80' maxlength='%d' style='font-size: 12pt' >";

const char HTTP_FORM_SCRIPT1b[] PROGMEM =
    "</textarea>"
    "<script type='text/javascript'>"
    "eb('charNum').innerHTML='-';"
    "var ta=eb('t1');"
    "ta.addEventListener('keydown',function(e){"
    "e = e || window.event;"
    "var ml=this.getAttribute('maxlength');"
    "var cl=this.value.length;"
    "if(cl>=ml){"
    "eb('charNum').innerHTML='" D_SCRIPT_CHARS_NO_MORE "';"
    "}else{"
    "eb('charNum').innerHTML=ml-cl+' " D_SCRIPT_CHARS_LEFT "';"
    "}"

#if 0
    // catch shift ctrl v
    "if ((e.metaKey || e.ctrlKey) && e.shiftKey && e.which === 86) {"
    // clipboard fails here
      "var paste = window.clipboardData.getData('Text');"
      //"var paste = e.view.clipboardData.getData('Text');"

      "var out=\"\";"
      "var re=/\\r\\n|\\n\\r|\\n|\\r/g;"
      "var allLines=paste.replace(re,\"\\n\").split(\"\\n\");"
      "allLines.forEach((line) => {"
        "if(line.length>0) {"
          "if(line.charAt(0)!=';'){"
            "out+=line+'\\n';"
          "}"
        "}"
      "});"
      "alert(out);"
    "}"
#endif

    "return true;"
    "});"


// this works only once on a reloaded page
#ifdef SCRIPT_STRIP_COMMENTS
    "ta.addEventListener('paste',function(e){"
      "let paste = (e.clipboardData || window.clipboardData).getData('text');"
      "var ml=this.getAttribute('maxlength');"
      "if(paste.length>=ml){"
        "var out=\"\";"
        "var re=/\\r\\n|\\n\\r|\\n|\\r/g;"
        "var allLines=paste.replace(re,\"\\n\").split(\"\\n\");"
        "allLines.forEach((line) => {"
          "if(line.length>0) {"
            "if(line.charAt(0)!=';'){"
              "out+=line+'\\n';"
            "}"
          "}"
        "});"
        "event.preventDefault();"
        "eb('t1').textContent=out;"
      "}"

    //  "alert(out);"
    // this pastes the text on the wrong position ????
    //"const selection = Window.getSelection();"
    //"if (!selection.rangeCount) return false;"
    //"selection.deleteFromDocument();"
    //"selection.getRangeAt(0).insertNode(document.createTextNode(paste));"

    //"return true;"

    "});"


#endif //SCRIPT_STRIP_COMMENTS

    "</script>";

const char HTTP_SCRIPT_FORM_END[] PROGMEM =
      "<br/>"
      "<button name='save' type='submit' formmethod='post' formenctype='multipart/form-data' formaction='/ta' class='button bgrn'>" D_SAVE "</button>"
      "</form></fieldset>";

#ifdef USE_SCRIPT_FATFS
const char HTTP_FORM_SCRIPT1c[] PROGMEM =
    "<button name='d%d' type='submit' class='button bgrn'>" D_SCRIPT_DOWNLOAD " '%s'</button>";


const char HTTP_FORM_SCRIPT1d[] PROGMEM =
    "<button method='post' name='upl' type='submit' class='button bgrn'>" D_SCRIPT_UPLOAD_FILES "</button>";

const char S_SCRIPT_FILE_UPLOAD[] PROGMEM = D_SDCARD_UPLOAD;

#endif // USE_SCRIPT_FATFS


uint8_t *script_ex_ptr;
uint16_t uplsize;
uint8_t sc_state;

// upload script and start immediately
void script_upload_start(void) {

  //AddLog(LOG_LEVEL_INFO, PSTR("HTP: file upload execute"));

  HTTPUpload& upload = Webserver->upload();
  if (upload.status == UPLOAD_FILE_START) {
    //AddLog(LOG_LEVEL_INFO, PSTR("HTP: upload start"));
    script_ex_ptr = (uint8_t*)glob_script_mem.script_ram;
    //AddLog(LOG_LEVEL_INFO, PSTR("HTP: upload file %s, %d"),upload.filename.c_str(),upload.totalSize);

    if (strcmp(upload.filename.c_str(), "execute_script")) {
      Web.upload_error = 1;
      WSSend(500, CT_PLAIN, F("500: wrong filename"));
      return;
    }
    if (upload.totalSize >= glob_script_mem.script_size) {
      Web.upload_error = 1;
      WSSend(500, CT_PLAIN, F("500: file to large"));
      return;
    }
    uplsize = 0;

    sc_state = bitRead(Settings->rule_enabled, 0);
    bitWrite(Settings->rule_enabled, 0, 0);

  } else if(upload.status == UPLOAD_FILE_WRITE) {
    //AddLog(LOG_LEVEL_INFO, PSTR("HTP: upload write"));
    uint32_t csiz = upload.currentSize;
    uint32_t tsiz = glob_script_mem.script_size - 1;
    if (uplsize<tsiz) {
      if (uplsize + csiz < tsiz) {
        memcpy(script_ex_ptr, upload.buf, csiz);
        script_ex_ptr += csiz;
        uplsize += csiz;
      } else {
        csiz = tsiz - uplsize;
        memcpy(script_ex_ptr, upload.buf, csiz);
        script_ex_ptr += csiz;
        uplsize += csiz;
      }
    }

    //if (upload_file) upload_file.write(upload.buf,upload.currentSize);
  } else if(upload.status == UPLOAD_FILE_END) {
    //if (upload_file) upload_file.close();
    if (Web.upload_error) {
      AddLog(LOG_LEVEL_INFO, PSTR("HTP: upload error"));
    } else {
      *script_ex_ptr = 0;
      bitWrite(Settings->rule_enabled, 0, sc_state);
      SaveScript();
      SaveScriptEnd();
      //AddLog(LOG_LEVEL_INFO, PSTR("HTP: upload success"));
    }
  } else {
    Web.upload_error = 1;
    WSSend(500, CT_PLAIN, F("500: couldn't create file"));
  }
}

void ScriptExecuteUploadSuccess(void) {
  WSSend(200, CT_PLAIN, F("transfer OK"));
}


#ifdef USE_SCRIPT_FATFS

uint8_t DownloadFile(char *file) {
  File download_file;
  WiFiClient download_Client;

    if (!ufsp->exists(file)) {
      AddLog(LOG_LEVEL_INFO,PSTR("SCR: file not found"));
      return 0;
    }

    download_file = ufsp->open(file, FS_FILE_READ);
    if (!download_file) {
      AddLog(LOG_LEVEL_INFO,PSTR("SCR: could not open file"));
      return 0;
    }

    if (download_file.isDirectory()) {
      download_file.close();
      return 1;
    }

    uint32_t flen = download_file.size();

    download_Client = Webserver->client();
    Webserver->setContentLength(flen);

    char attachment[100];
    char *cp;
    for (uint8_t cnt = strlen(file); cnt>=0; cnt--) {
      if (file[cnt]=='/') {
        cp = &file[cnt + 1];
        break;
      }
    }
    snprintf_P(attachment, sizeof(attachment), PSTR("attachment; filename=%s"), cp);
    Webserver->sendHeader(F("Content-Disposition"), attachment);
    WSSend(200, CT_APP_STREAM, "");

    uint8_t buff[512];
    uint16_t bread;

    // transfer is about 150kb/s
    uint8_t cnt = 0;
    while (download_file.available()) {
      bread = download_file.read(buff, sizeof(buff));
      uint16_t bw = download_Client.write((const char*)buff, bread);
      if (!bw) break;
      cnt++;
      if (cnt>7) {
        cnt = 0;
        if (glob_script_mem.script_loglevel & 0x80) {
          // this indeed multitasks, but is slower 50 kB/s
          loop();
        }
      }
    }
    download_file.close();
    download_Client.stop();
    return 0;
}

#endif // USE_SCRIPT_FATFS


void HandleScriptTextareaConfiguration(void) {
  if (!HttpCheckPriviledgedAccess()) { return; }

  if (Webserver->hasArg("save")) {
    ScriptSaveSettings();
    HandleManagement();
    return;
  }
}


void HandleScriptConfiguration(void) {

    if (!HttpCheckPriviledgedAccess()) { return; }

    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_CONFIGURE_SCRIPT));

#ifdef USE_SCRIPT_FATFS
    if (Webserver->hasArg("d1")) {
      DownloadFile(glob_script_mem.flink[0]);
    }
    if (Webserver->hasArg("d2")) {
      DownloadFile(glob_script_mem.flink[1]);
    }
#endif // USE_SCRIPT_FATFS

    WSContentStart_P(PSTR(D_CONFIGURE_SCRIPT));
    WSContentSendStyle();
    WSContentSend_P(HTTP_FORM_SCRIPT);


#ifdef xSCRIPT_STRIP_COMMENTS
    uint16_t ssize = glob_script_mem.script_size;
    if (bitRead(Settings->rule_enabled, 1)) ssize *= 2;
    WSContentSend_P(HTTP_FORM_SCRIPT1,1,1,bitRead(Settings->rule_enabled,0) ? PSTR(" checked") : "",ssize);
#else
    WSContentSend_P(HTTP_FORM_SCRIPT1,1,1,bitRead(Settings->rule_enabled,0) ? PSTR(" checked") : "",glob_script_mem.script_size);
#endif // xSCRIPT_STRIP_COMMENTS

    // script is to large for WSContentSend_P
    if (glob_script_mem.script_ram[0]) {
      WSContentFlush();
      _WSContentSend(glob_script_mem.script_ram);
    }
    WSContentSend_P(HTTP_FORM_SCRIPT1b);

#ifdef USE_SCRIPT_FATFS
    if (ufsp) {
      //WSContentSend_P(HTTP_FORM_SCRIPT1d);
      if (glob_script_mem.flink[0][0]) WSContentSend_P(HTTP_FORM_SCRIPT1c, 1, glob_script_mem.flink[0]);
      if (glob_script_mem.flink[1][0]) WSContentSend_P(HTTP_FORM_SCRIPT1c, 2, glob_script_mem.flink[1]);
    }
#endif //USE_SCRIPT_FATFS

    WSContentSend_P(HTTP_SCRIPT_FORM_END);
    WSContentSpaceButton(BUTTON_MANAGEMENT);
    WSContentStop();
}

void SaveScript(void) {

#ifdef USE_UFILESYS
  if (glob_script_mem.FLAGS.fsys == true) {
    ufsp->remove(FAT_SCRIPT_NAME);
    File file = ufsp->open(FAT_SCRIPT_NAME, FS_FILE_WRITE);
    file.write((const uint8_t*)glob_script_mem.script_ram, strlen(glob_script_mem.script_ram));
    file.close();
  } else {
    // fallback to compressed mode
    script_compress(Settings->rules[0], MAX_SCRIPT_SIZE-1);
  }
#else // USE_UFILESYS

#ifdef EEP_SCRIPT_SIZE
  // here we handle EEPROM modes
  if (glob_script_mem.FLAGS.eeprom == true) {
    if (EEP_SCRIPT_SIZE < SPECIAL_EEPMODE_SIZE && EEP_SCRIPT_SIZE != SPI_FLASH_SEC_SIZE) {
      EEP_WRITE(0, EEP_SCRIPT_SIZE, glob_script_mem.script_ram);
    } else {
#if EEP_SCRIPT_SIZE==SPI_FLASH_2SEC_SIZE || EEP_SCRIPT_SIZE==SPI_FLASH_SEC_SIZE
      alt_eeprom_writeBytes(0, EEP_SCRIPT_SIZE, (uint8_t*)glob_script_mem.script_ram);
#else
      uint8_t *ucs;
      ucs = (uint8_t*)calloc(SPI_FLASH_SEC_SIZE + 4, 1);
      if (ucs) {
        if (!script_compress((char*)ucs, EEP_SCRIPT_SIZE - 1)) {
          alt_eeprom_writeBytes(0, EEP_SCRIPT_SIZE, ucs);
        }
        free(ucs);
      }
#endif
    }
  }
#else
    // default mode is compression
    script_compress(Settings->rules[0],MAX_SCRIPT_SIZE-1);
#endif // EEP_SCRIPT_SIZE


#endif // USE_UFILESYS
}

void ScriptSaveSettings(void) {

  if (Webserver->hasArg("c1")) {
    bitWrite(Settings->rule_enabled, 0, 1);
  } else {
    bitWrite(Settings->rule_enabled, 0, 0);
  }

  String str = Webserver->arg("t1");

  if (*str.c_str()) {

    str.replace("\r\n", "\n");
    str.replace("\r", "\n");

    strlcpy(glob_script_mem.script_ram, str.c_str(), glob_script_mem.script_size);

    if (glob_script_mem.script_ram[0]!='>' && glob_script_mem.script_ram[1]!='D') {
      AddLog(LOG_LEVEL_INFO, PSTR("SCR: error, must start with >D"));
      bitWrite(Settings->rule_enabled, 0, 0);
    }

    SaveScript();

  } else {
    AddLog(LOG_LEVEL_INFO, PSTR("SCR: memory error"));
  }

  SaveScriptEnd();
}

//
uint32_t script_compress(char *dest, uint32_t size) {
  //AddLog(LOG_LEVEL_INFO,PSTR("len: %d dsize = %d"), size, strlen(glob_script_mem.script_ram));
  yield();
  int32_t len_compressed = SCRIPT_COMPRESS(glob_script_mem.script_ram, strlen(glob_script_mem.script_ram), dest, size);
  yield();
  if (len_compressed > 0) {
    dest[len_compressed] = 0;
    AddLog(LOG_LEVEL_INFO,PSTR("SCR: compressed to %d bytes = %d %%"),len_compressed,len_compressed * 100 / strlen(glob_script_mem.script_ram));
    return 0;
  } else {
    AddLog(LOG_LEVEL_INFO, PSTR("SCR: compress error %d"), len_compressed);
    return 1;
  }
}
//#endif // USE_SCRIPT_COMPRESSION

void SaveScriptEnd(void) {

#ifdef USE_SCRIPT_GLOBVARS
  Script_Stop_UDP();
#endif //USE_SCRIPT_GLOBVARS

  if (glob_script_mem.script_mem) {
    // script was restarted
    Run_Scripter1(">R\n", 3, 0);
    Scripter_save_pvars();
    free(glob_script_mem.script_mem);
    glob_script_mem.script_mem = 0;
    glob_script_mem.script_mem_size = 0;
 #ifdef USE_SCRIPT_SERIAL
    Script_Close_Serial();
#endif
  }

  if (bitRead(Settings->rule_enabled, 0)) {

    int16_t res = Init_Scripter();
    if (res) {
      AddLog(LOG_LEVEL_INFO, PSTR("SCR: init error %d"), res);
      return;
    }

    set_callbacks();

    Run_Scripter1(">B\n", 3, 0);
    Run_Scripter1(">BS", 3, 0);

    //glob_script_mem.fast_script = Run_Scripter(">F", -2, 0);

    script_set_web_pages();

  }
}

void set_wpages(char *id, uint16_t index) {
  uint16_t idlen = strlen(id);
  uint16_t idl2 = idlen;
  if (id[idlen - 1] ==' ') idl2--;
  if (Run_Scripter1(id, -idlen, 0) == 99) {glob_script_mem.web_pages[index] = glob_script_mem.section_ptr + idl2;} else {glob_script_mem.web_pages[index] = 0;}
}

#define WEB_PAGE_WS 8
#define WEB_PAGE_WM 9

const char SWPAGES[] PROGMEM = {"W|w |w1 |w2 |w3 |w4 |w5 |w6 |WS|WM|$"};

void script_set_web_pages(void) {
  char lbl[6];
  lbl[0] = '>';

  uint16_t index = 0;
  while (1) {
    GetTextIndexed(&lbl[1], sizeof(lbl) - 1, index, SWPAGES);
    if (lbl[1] == '$') {
      break;
    }
    set_wpages(lbl, index);
    index++;
  }
}

#endif // USE_WEBSERVER

void set_callbacks() {
  if (Run_Scripter1(">F", -2, 0) == 99) {glob_script_mem.fast_script = glob_script_mem.section_ptr + 2;} else {glob_script_mem.fast_script = 0;}
  if (Run_Scripter1(">E", -2, 0) == 99) {glob_script_mem.event_script = glob_script_mem.section_ptr + 2;} else {glob_script_mem.event_script = 0;}
  if (Run_Scripter1(">C", -2, 0) == 99) {glob_script_mem.html_script = glob_script_mem.section_ptr + 2;} else {glob_script_mem.html_script = 0;}
  if (Run_Scripter1(">T", -2, 0) == 99) {glob_script_mem.teleperiod = glob_script_mem.section_ptr + 2;} else {glob_script_mem.teleperiod = 0;}
}


//#define SCRIPT_HUE_DEBUG

#if defined(USE_SCRIPT_HUE) && defined(USE_WEBSERVER) && defined(USE_EMULATION) && defined(USE_EMULATION_HUE) && defined(USE_LIGHT)
#define HUE_DEV_MVNUM 5
#define HUE_DEV_NSIZE 16
struct HUE_SCRIPT {
  char name[HUE_DEV_NSIZE];
  uint8_t type;
  uint8_t index[HUE_DEV_MVNUM];
  uint8_t vindex[HUE_DEV_MVNUM];
} hue_script[32];


const char SCRIPT_HUE_LIGHTS_STATUS_JSON1[] PROGMEM =
  "{\"state\":"
  "{\"on\":{state},"
  "{light_status}"
  "\"alert\":\"none\","
  "\"effect\":\"none\","
  "\"reachable\":true}"
  ",\"type\":\"{type}\","
  "\"name\":\"{j1\","
  "\"modelid\":\"{m1}\","
  "\"uniqueid\":\"{j2\","
  "\"swversion\":\"5.50.1.19085\"}";

/*
const char SCRIPT_HUE_LIGHTS_STATUS_JSON2[] PROGMEM =
  "{\"state\":"
  "{\"temperature\": 2674,"
  "\"lastupdated\": \"2019-08-04T12:13:04\"},"
  "\"config\": {"
  "\"on\": true,"
  "\"battery\": 100,"
  "\"reachable\": true,"
  "\"alert\": \"none\","
  "\"ledindication\": false,"
  "\"usertest\": false,"
  "\"pending\": []"
  "},"
  "\"name\": \"{j1\","
  "\"type\": \"ZLLTemperature\","
  "\"modelid\": \"SML001\","
  "\"manufacturername\": \"Philips\","
  "\"swversion\": \"6.1.0.18912\","
  "\"uniqueid\": \"{j2\"}";
*/


const char SCRIPT_HUE_LIGHTS_STATUS_JSON2[] PROGMEM =
"{\"state\":{"
"\"presence\":{state},"
"\"lastupdated\":\"2017-10-01T12:37:30\""
"},"
"\"swupdate\":{"
"\"state\":\"noupdates\","
"\"lastinstall\": null"
"},"
"\"config\":{"
"\"on\":true,"
"\"battery\":100,"
"\"reachable\":true,"
"\"alert\":\"none\","
"\"ledindication\":false,"
"\"usertest\":false,"
"\"sensitivity\":2,"
"\"sensitivitymax\":2,"
"\"pending\":[]"
"},"
"\"name\":\"{j1\","
"\"type\":\"ZLLPresence\","
"\"modelid\":\"SML001\","
"\"manufacturername\":\"Philips\","
"\"swversion\":\"6.1.0.18912\","
"\"uniqueid\":\"{j2\""
"}";

/*


Color Ligh
Dimmable Light
Color Temperature Light
Extended Color Light
On/Off light

ZGPSwitch
ZLLSwitch
CLIPSwitch
CLIPOpenClose
CLIPPresence
CLIPTemperature
CLIPHumidity
Daylight
CLIPLightlevel


  temperature ZLLTemperature
  lightlevel ZLLLightLevel
  presence ZLLPresence
  */


/*

case 'T':
response->replace("{type}","ZLLTemperature");
temp=glob_script_mem.fvars[hue_script[hue_devs].index[2]-1];
light_status += "\"temperature\":";
light_status += String(temp*100);
light_status += ",";
break;
case 'L':
response->replace("{type}","ZLLLightLevel");
temp=glob_script_mem.fvars[hue_script[hue_devs].index[2]-1];
light_status += "\"lightlevel\":";
light_status += String(temp);
light_status += ",";
break;
case 'P':
response->replace("{type}","ZLLPresence");
temp=glob_script_mem.fvars[hue_script[hue_devs].index[0]-1];
light_status += "\"presence\":";
if (temp==0)light_status += "false";
else light_status += "true";
light_status += ",";
break;
*/
String GetHueDeviceId(uint16_t id, uint8_t ep);

void Script_HueStatus(String *response, uint16_t hue_devs) {

  if (hue_script[hue_devs].type=='p') {
    *response += FPSTR(SCRIPT_HUE_LIGHTS_STATUS_JSON2);
    response->replace("{j1", hue_script[hue_devs].name);
    response->replace("{j2", GetHueDeviceId(hue_devs, 0));
    uint8_t pwr = glob_script_mem.fvars[hue_script[hue_devs].index[0] - 1];
    response->replace("{state}", (pwr ? "true" : "false"));
    return;
  }

  *response += FPSTR(SCRIPT_HUE_LIGHTS_STATUS_JSON1);
  uint8_t pwr = glob_script_mem.fvars[hue_script[hue_devs].index[0] - 1];
  response->replace("{state}", (pwr ? "true" : "false"));
  String light_status = "";
  if (hue_script[hue_devs].index[1]>0) {
    // bri
    light_status += "\"bri\":";
    uint32_t bri = glob_script_mem.fvars[hue_script[hue_devs].index[1] - 1];
    if (bri > 254)  bri = 254;
    if (bri < 1)    bri = 1;
    light_status += String(bri);
    light_status += ",";
  }
  if (hue_script[hue_devs].index[2]>0) {
    // hue
    uint32_t hue = glob_script_mem.fvars[hue_script[hue_devs].index[2] - 1];
    //hue = changeUIntScale(hue, 0, 359, 0, 65535);
    light_status += "\"hue\":";
    light_status += String(hue);
    light_status += ",";
  }
  if (hue_script[hue_devs].index[3]>0) {
    // sat
    uint32_t sat = glob_script_mem.fvars[hue_script[hue_devs].index[3] - 1] ;
    if (sat > 254)  sat = 254;
    if (sat < 1)    sat = 1;
    light_status += "\"sat\":";
    light_status += String(sat);
    light_status += ",";
  }
  if (hue_script[hue_devs].index[4]>0) {
    // ct
    uint32_t ct = glob_script_mem.fvars[hue_script[hue_devs].index[4] - 1];
    light_status += "\"ct\":";
    light_status += String(ct);
    light_status += ",";
  }

  TS_FLOAT temp;
  switch (hue_script[hue_devs].type) {
    case 'C':
      response->replace("{type}","Color Ligh"); // alexa ok
      response->replace("{m1","LST001");
      break;
    case 'D':
      response->replace("{type}","Dimmable Light"); // alexa NO
      response->replace("{m1","LWB004");
      break;
    case 'T':
      response->replace("{type}","Color Temperature Light"); // alexa NO
      response->replace("{m1","LTW011");
      break;
    case 'E':
      response->replace("{type}","Extended color light"); // alexa ok
      response->replace("{m1","LCT007");
      break;
    case 'S':
      response->replace("{type}","On/Off light"); // alexa ok
      response->replace("{m1","LCT007");
      break;
    default:
      response->replace("{type}","color light");
      response->replace("{m1","LST001");
      break;
  }

  response->replace("{light_status}", light_status);
  response->replace("{j1", hue_script[hue_devs].name);
  response->replace("{j2", GetHueDeviceId(hue_devs, 0));

}

void Script_Check_Hue(String *response) {
  if (!bitRead(Settings->rule_enabled, 0)) return;

  uint8_t hue_script_found = Run_Scripter1(">H", -2, 0);
  if (hue_script_found!=99) return;

  char tmp[256];
  uint8_t hue_devs = 0;
  uint8_t vindex = 0;
  char *cp;
  char *lp = glob_script_mem.section_ptr + 2;
  while (lp) {
    SCRIPT_SKIP_SPACES
    while (*lp == SCRIPT_EOL) {
     lp++;
    }
    if (!*lp || *lp=='#' || *lp=='>') {
        break;
    }
    if (*lp!=';') {
      // check this line
      Replace_Cmd_Vars(lp, 1, tmp, sizeof(tmp));
      // check for hue defintions
      // NAME, TYPE , vars
      cp = tmp;
      cp = strchr(cp,',');
      if (!cp) break;
      *cp = 0;
      // copy name
      strlcpy(hue_script[hue_devs].name, tmp, HUE_DEV_NSIZE);
      cp++;
      while (*cp==' ') cp++;
      // get type
      hue_script[hue_devs].type = *cp;

      for (vindex = 0; vindex < HUE_DEV_MVNUM; vindex++) {
        hue_script[hue_devs].index[vindex] = 0;
      }
      vindex = 0;
      while (1) {
        cp = strchr(cp,',');
        if (!cp) break;
        // get vars, on,hue,sat,bri,ct,
        cp++;
        while (*cp==' ') cp++;

        vindex == 0xff;
        if (!strncmp(cp, "on=", 3)) {
          cp += 3;
          vindex = 0;
        } else if (!strncmp(cp, "bri=", 4)) {
          cp += 4;
          vindex = 1;
        } else if (!strncmp(cp, "hue=", 4)) {
          cp += 4;
          vindex = 2;
        } else if (!strncmp(cp, "sat=", 4)) {
          cp += 4;
          vindex = 3;
        } else if (!strncmp(cp, "ct=", 3)) {
          cp += 3;
          vindex = 4;
        } else {
          // error
          vindex == 0xff;
          break;
        }
        if (vindex!=0xff) {
          struct T_INDEX ind;
          uint8_t vtype;
          char vname[16];
          for (uint32_t cnt = 0; cnt < sizeof(vname) - 1; cnt++) {
            if (*cp==',' || *cp==0) {
              vname[cnt] = 0;
              break;
            }
            vname[cnt] = *cp++;
          }
          isvar(vname, &vtype, &ind, 0, 0, 0);
          if (vtype != VAR_NV) {
            // found variable as result
            if (vtype == NUM_RES || (vtype & STYPE) == 0) {
              hue_script[hue_devs].vindex[vindex] = ind.index;
              hue_script[hue_devs].index[vindex] = glob_script_mem.type[ind.index].index+1;
            } else {
            //  break;
            }
          }
        }
      }
      // append response
#ifdef SCRIPT_HUE_DEBUG
      AddLog(LOG_LEVEL_INFO, PSTR("HUE: %s - %d "),hue_script[hue_devs].name, hue_devs);
#endif
      if (response) {
        if (TasmotaGlobal.devices_present) {
          *response += ",\"";
        }
        else {
          if (hue_devs>0) *response += ",\"";
          else *response += "\"";
        }
        *response += String(EncodeLightId(hue_devs + TasmotaGlobal.devices_present + 1))+"\":";
        Script_HueStatus(response, hue_devs);
#ifdef SCRIPT_HUE_DEBUG
        AddLog(LOG_LEVEL_INFO, PSTR("HUE: %s - %d "),response->c_str(), hue_devs);
#endif
      }
      hue_devs++;
    }
    if (*lp==SCRIPT_EOL) {
      lp++;
    } else {
      lp = strchr(lp, SCRIPT_EOL);
      if (!lp) break;
      lp++;
    }
  }
#ifdef SCRIPT_HUE_DEBUG
  if (response) {
    AddLog(LOG_LEVEL_INFO, PSTR("HUE: %d"), hue_devs);
    toLog(">>>>");
    toLog(response->c_str());
    toLog(response->c_str()+700);   // Was MAX_LOGSZ
  }
#endif
}

const char sHUE_LIGHT_RESPONSE_JSON[] PROGMEM =
  "{\"success\":{\"/lights/{id/state/{cm\":{re}}";

const char sHUE_SENSOR_RESPONSE_JSON[] PROGMEM =
  "{\"success\":{\"/lights/{id/state/{cm\":{re}}";

const char sHUE_ERROR_JSON[] PROGMEM =
  "[{\"error\":{\"type\":901,\"address\":\"/\",\"description\":\"Internal Error\"}}]";


// get alexa arguments
void Script_Handle_Hue(String path) {
  String response;
  int code = 200;
  uint16_t tmp = 0;
  uint16_t hue = 0;
  uint8_t  sat = 0;
  uint8_t  bri = 254;
  uint16_t ct = 0;
  bool resp = false;

  uint8_t device = DecodeLightId(atoi(path.c_str()));
  uint8_t index = device - TasmotaGlobal.devices_present - 1;

  uint16_t args = Webserver->args();

#ifdef ESP82666
  char *json = (char*)Webserver->arg(args - 1).c_str();
#else
   String request_arg = Webserver->arg(args - 1);
   char *json = (char*)request_arg.c_str();
#endif

#ifdef SCRIPT_HUE_DEBUG
  AddLog(LOG_LEVEL_INFO, PSTR("HUE: 0 %s - %d "),path.c_str(), device);
  AddLog(LOG_LEVEL_INFO, PSTR("HUE: 1 %d, %s"), args, json);
#endif
  if (Webserver->args()) {
    response = "[";

    JsonParser parser(json);
    JsonParserObject root = parser.getRootObject();
    JsonParserToken hue_on = root[PSTR("on")];

    if (hue_on) {
      response += FPSTR(sHUE_LIGHT_RESPONSE_JSON);
      response.replace("{id", String(EncodeLightId(device)));
      response.replace("{cm", "on");

      bool on = hue_on.getBool();
      if (on == false) {
        glob_script_mem.fvars[hue_script[index].index[0] - 1] = 0;
        response.replace("{re", "false");
      } else {
        glob_script_mem.fvars[hue_script[index].index[0] - 1] = 1;
          response.replace("{re", "true");
      }
      SetChanged(hue_script[index].vindex[0]);
      resp = true;
    }

    parser.setCurrent();
    JsonParserToken hue_bri = root[PSTR("bri")];
    if (hue_bri) {             // Brightness is a scale from 1 (the minimum the light is capable of) to 254 (the maximum). Note: a brightness of 1 is not off.
      tmp = hue_bri.getUInt();
      bri = tmp;
      if (254 <= bri) { bri = 255; }
      if (resp) { response += ","; }
      response += FPSTR(sHUE_LIGHT_RESPONSE_JSON);
      response.replace("{id", String(EncodeLightId(device)));
      response.replace("{cm", "bri");
      response.replace("{re", String(tmp));
      glob_script_mem.fvars[hue_script[index].index[1] - 1] = bri;
      SetChanged(hue_script[index].vindex[1]);
      resp = true;
    }

    JsonParserToken hue_xy = root[PSTR("xy")];
    if (hue_xy) {             // Saturation of the light. 254 is the most saturated (colored) and 0 is the least saturated (white).
      TS_FLOAT x, y;
      JsonParserArray arr_xy = JsonParserArray(hue_xy);
      JsonParserToken tok_x = arr_xy[0];
      JsonParserToken tok_y = arr_xy[1];
      x = tok_x.getFloat();
      y = tok_y.getFloat();
      String x_str = tok_x.getStr();
      String y_str = tok_y.getStr();
      uint8_t rr,gg,bb;
      XyToRgb(x, y, &rr, &gg, &bb);
      RgbToHsb(rr, gg, bb, &hue, &sat, nullptr);
      if (resp) { response += ","; }
      response += FPSTR(sHUE_LIGHT_RESPONSE_JSON);
      response.replace("{id", String(device));
      response.replace("{cm", "xy");
      response.replace("{re", "[" + x_str + "," + y_str + "]");
      glob_script_mem.fvars[hue_script[index].index[2]-1] = hue;
      SetChanged(hue_script[index].vindex[2]);
      glob_script_mem.fvars[hue_script[index].index[3]-1] = sat;
      SetChanged(hue_script[index].vindex[3]);
      resp = true;
    }

    JsonParserToken hue_hue = root[PSTR("hue")];
    if (hue_hue) {             // The hue value is a wrapping value between 0 and 65535. Both 0 and 65535 are red, 25500 is green and 46920 is blue.
      tmp = hue_hue.getUInt();
      //hue = changeUIntScale(tmp, 0, 65535, 0, 359);
      //tmp = changeUIntScale(hue, 0, 359, 0, 65535);
      hue = tmp;
      if (resp) { response += ","; }
      response += FPSTR(sHUE_LIGHT_RESPONSE_JSON);
      response.replace("{id", String(EncodeLightId(device)));
      response.replace("{cm", "hue");
      response.replace("{re", String(tmp));
      glob_script_mem.fvars[hue_script[index].index[2] - 1] = hue;
      SetChanged(hue_script[index].vindex[2]);
      resp = true;
    }

    JsonParserToken hue_sat = root[PSTR("sat")];
    if (hue_sat) {             // Saturation of the light. 254 is the most saturated (colored) and 0 is the least saturated (white).
      tmp = hue_sat.getUInt();
      sat = tmp;
      if (254 <= sat) { sat = 255; }
      if (resp) { response += ","; }
      response += FPSTR(sHUE_LIGHT_RESPONSE_JSON);
      response.replace("{id", String(EncodeLightId(device)));
      response.replace("{cm", "sat");
      response.replace("{re", String(tmp));
      glob_script_mem.fvars[hue_script[index].index[3] - 1] = sat;
      SetChanged(hue_script[index].vindex[3]);
      resp = true;
    }

    JsonParserToken hue_ct = root[PSTR("ct")];
    if (hue_ct) {  // Color temperature 153 (Cold) to 500 (Warm)
      ct = hue_ct.getUInt();
      if (resp) { response += ","; }
      response += FPSTR(sHUE_LIGHT_RESPONSE_JSON);
      response.replace("{id", String(EncodeLightId(device)));
      response.replace("{cm", "ct");
      response.replace("{re", String(ct));
      glob_script_mem.fvars[hue_script[index].index[4] - 1] = ct;
      SetChanged(hue_script[index].vindex[4]);
      resp = true;
    }
    response += "]";

  } else {
    response = FPSTR(sHUE_ERROR_JSON);
  }
#ifdef SCRIPT_HUE_DEBUG
  AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_HTTP D_HUE " Result (%s)"), response.c_str());
#else
  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_HTTP D_HUE " Result (%s)"), response.c_str());
#endif
  WSSend(code, CT_APP_JSON, response);
  if (resp) {
    //Run_Scripter(">E", 2, 0);
    if (glob_script_mem.event_script) Run_Scripter1(glob_script_mem.event_script, 0, 0);
  }
}
#endif  // hue interface


#ifdef USE_SCRIPT_SUB_COMMAND
bool Script_SubCmd(void) {
  if (!bitRead(Settings->rule_enabled, 0)) return false;

  if (tasm_cmd_activ) return false;
  //AddLog(LOG_LEVEL_INFO,PSTR(">> %s, %s, %d, %d "),XdrvMailbox.topic, XdrvMailbox.data, XdrvMailbox.payload, XdrvMailbox.index);

  char command[CMDSZ];
  strlcpy(command, XdrvMailbox.topic, CMDSZ);
  if (XdrvMailbox.index > 1) {
    char ind[2];
    ind[0] = XdrvMailbox.index | 0x30;
    ind[1] = 0;
    strcat(command, ind);
  }

  int32_t pl = XdrvMailbox.payload;

  char cmdbuff[128];
  char *cp = cmdbuff;
  *cp++ = '#';
  strcpy(cp, command);
  uint8_t tlen = strlen(command);
  cp += tlen;
  if (XdrvMailbox.data_len > 0) {
    *cp++ = '(';
    strncpy(cp, XdrvMailbox.data,XdrvMailbox.data_len);
    cp += XdrvMailbox.data_len;
    *cp++ = ')';
    *cp = 0;
  }
  //toLog(cmdbuff);
  uint32_t res = Run_Scripter1(cmdbuff, tlen + 1, 0);
  //AddLog(LOG_LEVEL_INFO,">>%d",res);
  if (res) {
    return false;
  }
  else {
    cp = XdrvMailbox.data;
    while (*cp==' ') cp++;
    if (isdigit(*cp) || *cp=='-') {
      Response_P(S_JSON_COMMAND_NVALUE, command, XdrvMailbox.payload);
    } else {
      Response_P(S_JSON_COMMAND_SVALUE, command, XdrvMailbox.data);
    }
  }
  return true;
}
#endif //USE_SCRIPT_SUB_COMMAND


void script_version(void) {
uint32_t options = 0;
#ifdef USE_BUTTON_EVENT
  options |= 0x00000001;
#endif
#ifdef USE_SCRIPT_JSON_EXPORT
  options |= 0x00000002;
#endif
#ifdef USE_SCRIPT_SUB_COMMAND
  options |= 0x00000004;
#endif
#ifdef USE_SCRIPT_HUE
  options |= 0x00000008;
#endif
#ifdef USE_HOMEKIT
  options |= 0x00000010;
#endif
#ifdef USE_SCRIPT_STATUS
  options |= 0x00000020;
#endif
#ifdef SUPPORT_MQTT_EVENT
  options |= 0x00000040;
#endif
#ifdef USE_SENDMAIL
  options |= 0x00000080;
#endif
#ifdef USE_SCRIPT_WEB_DISPLAY
  options |= 0x00000100;
#endif
#ifdef SCRIPT_FULL_WEBPAGE
  options |= 0x00000200;
#endif
#ifdef USE_TOUCH_BUTTONS
  options |= 0x00000400;
#endif
#ifdef USE_WEBSEND_RESPONSE
  options |= 0x00000800;
#endif
#ifdef USE_ANGLE_FUNC
  options |= 0x00001000;
#endif
#ifdef USE_SCRIPT_TASK
  options |= 0x00002000;
#endif
#ifdef USE_SCRIPT_GLOBVARS
  options |= 0x00004000;
#endif
#ifdef USE_SCRIPT_TIMER
  options |= 0x00008000;
#endif
#ifdef SCRIPT_GET_HTTPS_JP
  options |= 0x00010000;
#endif
#ifdef LARGE_ARRAYS
  options |= 0x00020000;
#endif
#ifdef SCRIPT_LARGE_VNBUFF
  options |= 0x00040000;
#endif
#ifdef USE_GOOGLE_CHARTS
  options |= 0x00080000;
#endif
#ifdef USE_FEXTRACT
  options |= 0x00100000;
#endif
#ifdef USE_SCRIPT_SPI
  options |= 0x00200000;
#endif
#ifdef USE_SCRIPT_I2C
  options |= 0x00400000;
#endif
#ifdef USE_DSIPLAY_DUMP
  options |= 0x00800000;
#endif
#ifdef USE_SCRIPT_SERIAL
  options |= 0x01000000;
#endif

  Response_P(PSTR("{\"script\":{\"vers\":%d.%d,\"opts\":%08x}}"), SCRIPT_VERS[0], SCRIPT_VERS[1], options);
}


void execute_script(char *script) {
  char *svd_sp = glob_script_mem.scriptptr;
  strcat(script, "\n#");
  glob_script_mem.scriptptr = script;
  Run_Scripter1(">", 1, 0);
  glob_script_mem.scriptptr = svd_sp;
}
#define D_CMND_SCRIPT "Script"
#define D_CMND_SUBSCRIBE "Subscribe"
#define D_CMND_UNSUBSCRIBE "Unsubscribe"


enum ScriptCommands { CMND_SCRIPT,CMND_SUBSCRIBE, CMND_UNSUBSCRIBE, CMND_SUBTEST};
const char kScriptCommands[] PROGMEM = D_CMND_SCRIPT "|" D_CMND_SUBSCRIBE "|" D_CMND_UNSUBSCRIBE
#ifdef DEBUG_MQTT_EVENT
  "|" "SUBTEST"
#endif
;
bool ScriptCommand(void) {
  char command[CMDSZ];
  bool serviced = true;
  uint8_t index = XdrvMailbox.index;

  if (tasm_cmd_activ) return false;

  int command_code = GetCommandCode(command, sizeof(command), XdrvMailbox.topic, kScriptCommands);
  if (-1 == command_code) {
    serviced = false;  // Unknown command
  }
  else if ((CMND_SCRIPT == command_code) && (index > 0)) {

    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 9)) {
      switch (XdrvMailbox.payload) {
        case 0: // Off
        case 1: // On
          bitWrite(Settings->rule_enabled, index -1, XdrvMailbox.payload);
          break;
        case 8: // stop on error Off
        case 9: // On
          bitWrite(Settings->rule_stop, index - 1, XdrvMailbox.payload & 1);
          break;

#ifdef xSCRIPT_STRIP_COMMENTS
        case 2:
          bitWrite(Settings->rule_enabled, 1, 0);
          break;
        case 3:
          bitWrite(Settings->rule_enabled, 1, 1);
          break;
#endif //xSCRIPT_STRIP_COMMENTS
        default:
          break;
      }
    } else {
      if ('>' == XdrvMailbox.data[0]) {
        // execute script
        Response_P(PSTR("{\"%s\":\"%s\"}"), command, XdrvMailbox.data);
        if (bitRead(Settings->rule_enabled, 0)) {
          for (uint8_t count = 0; count<XdrvMailbox.data_len; count++) {
            if (XdrvMailbox.data[count]==';') XdrvMailbox.data[count] = '\n';
          }
          execute_script(XdrvMailbox.data);
        }
      }
      if (!strcmp(XdrvMailbox.data, "-v")) {
        script_version();
        return serviced;
      }
      if ('?' == XdrvMailbox.data[0]) {
        char *lp = XdrvMailbox.data;
        lp++;
        while (*lp==' ') lp++;
        TS_FLOAT fvar;
        char str[SCRIPT_MAXSSIZE];
        glob_script_mem.glob_error = 0;
        TS_FLOAT *fpd = 0;
        uint16_t alend;
        char *cp = get_array_by_name(lp, &fpd, &alend, 0);
        if (fpd && cp && (!strchr(lp, '[')) ) {
          // is array
          Response_P(PSTR("{\"script\":{\"%s\":["), lp);
          for (uint16_t cnt = 0; cnt < alend; cnt++) {
            TS_FLOAT tvar = *fpd++;
            ext_snprintf_P(str, sizeof(str), PSTR("%*_f"), -glob_script_mem.script_dprec, &tvar);
            if (cnt) {
              ResponseAppend_P(PSTR(",%s"), str);
            } else {
              ResponseAppend_P(PSTR("%s"), str);
            }
          }
          ResponseAppend_P(PSTR("]}}"));
        } else {
          glob_script_mem.glob_error = 0;
          GetNumericArgument(lp, OPER_EQU, &fvar, 0);
          if (glob_script_mem.glob_error == 1) {
            // was string, not number
            GetStringArgument(lp, OPER_EQU, str, 0);
            Response_P(PSTR("{\"script\":{\"%s\":\"%s\"}}"), lp, str);
          } else {
            ext_snprintf_P(str, sizeof(str), PSTR("%*_f"), -glob_script_mem.script_dprec, &fvar);
            Response_P(PSTR("{\"script\":{\"%s\":%s}}"), lp, str);
          }
        }
      }
      return serviced;
    }
    Response_P(PSTR("{\"%s\":\"%s\",\"StopOnError\":\"%s\",\"Free\":%d}"), command, GetStateText(bitRead(Settings->rule_enabled, 0)), GetStateText(bitRead(Settings->rule_stop, 0)),glob_script_mem.script_size - strlen(glob_script_mem.script_ram));
#ifdef SUPPORT_MQTT_EVENT
  } else if (CMND_SUBSCRIBE == command_code) {			//MQTT Subscribe command. Subscribe <Event>, <Topic> [, <Key>]
      String result = ScriptSubscribe(XdrvMailbox.data, XdrvMailbox.data_len);
      Response_P(S_JSON_COMMAND_SVALUE, command, result.c_str());
    } else if (CMND_UNSUBSCRIBE == command_code) {			//MQTT Un-subscribe command. UnSubscribe <Event>
      String result = ScriptUnsubscribe(XdrvMailbox.data, XdrvMailbox.data_len);
      Response_P(S_JSON_COMMAND_SVALUE, command, result.c_str());
#ifdef DEBUG_MQTT_EVENT
    } else if (CMND_SUBTEST == command_code) {
      XdrvMailbox.topic = (char*)"stat/tasmota/SENSOR";
      ScriptMqttData();
      serviced = true;
#endif
#endif //SUPPORT_MQTT_EVENT
    }
  return serviced;
}


#ifdef USE_SCRIPT_FATFS

uint16_t xFAT_DATE(uint16_t year, uint8_t month, uint8_t day) {
  return (year - 1980) << 9 | month << 5 | day;
}
uint16_t xFAT_TIME(uint8_t hour, uint8_t minute, uint8_t second) {
  return hour << 11 | minute << 5 | second >> 1;
}

void dateTime(uint16_t* date, uint16_t* time) {
  // return date using FAT_DATE macro to format fields
  *date = xFAT_DATE(RtcTime.year,RtcTime.month, RtcTime.day_of_month);
  // return time using FAT_TIME macro to format fields
  *time = xFAT_TIME(RtcTime.hour,RtcTime.minute,RtcTime.second);
}

#endif //USE_SCRIPT_FATFS



#ifdef SUPPORT_MQTT_EVENT

#ifndef MQTT_EVENT_MSIZE
#define MQTT_EVENT_MSIZE 256
#endif // MQTT_EVENT_MSIZE


/********************************************************************************************/
/*
 * Script: Process received MQTT message.
 *        If the message is in our subscription list, trigger an event with the value parsed from MQTT data
 * Input:
 *      void      - We are going to access XdrvMailbox data directly.
 * Return:
 *      true      - The message is consumed.
 *      false     - The message is not in our list.
 */
bool ScriptMqttData(void) {
  bool serviced = false;
  //toLog(">>> 1");
  //toLog(XdrvMailbox.data);
  if (XdrvMailbox.data_len < 1 || XdrvMailbox.data_len > MQTT_EVENT_MSIZE) {
    return false;
  }
  String sTopic = XdrvMailbox.topic;
  String buData = XdrvMailbox.data;

#ifdef DEBUG_MQTT_EVENT
    AddLog(LOG_LEVEL_INFO, PSTR("SCR: MQTT Topic %s, Event %s"), XdrvMailbox.topic, XdrvMailbox.data);
#endif

  MQTT_Subscription event_item;
  //Looking for matched topic
  for (uint32_t index = 0; index < subscriptions.size(); index++) {
    event_item = subscriptions.get(index);
    uint8_t json_valid = 0;

    String sData = buData;

#ifdef DEBUG_MQTT_EVENT
    AddLog(LOG_LEVEL_INFO, PSTR("SCR: Match MQTT message Topic %s with subscription topic %s and key %s"), sTopic.c_str(), event_item.Topic.c_str(),event_item.Key.c_str());
#endif
    if (sTopic.startsWith(event_item.Topic)) {
      //This topic is subscribed by us, so serve it
      serviced = true;
      String value;
      String lkey;
      if (event_item.Key.length() == 0) {   //If did not specify Key
        value = sData;
        json_valid = 1;
      } else {      //If specified Key, need to parse Key/Value from JSON data


#ifndef SUPPORT_MQTT_EVENT_MORE
        JsonParser parser((char*)sData.c_str());
        JsonParserObject jsonData = parser.getRootObject();
        String key1 = event_item.Key;
        String key2;
        if (!jsonData) break;       //Failed to parse JSON data, ignore this message.
        int dot;
        if ((dot = key1.indexOf('.')) > 0) {
          key2 = key1.substring(dot+1);
          key1 = key1.substring(0, dot);
          lkey = key2;
          JsonParserToken val = jsonData[key1.c_str()].getObject()[key2.c_str()];
          if (!val) break;   //Failed to get the key/value, ignore this message.
          value = val.getStr();
          json_valid = 1;
        } else {
          JsonParserToken val = jsonData[key1.c_str()];
          if (!val) break;
          value = val.getStr();
          lkey = key1;
          json_valid = 1;
        }
#else

        JsonParser parser((char*)sData.c_str());
        JsonParserObject obj = parser.getRootObject();
        char sres[64];
        uint32_t res = JsonParsePath(&obj, event_item.Key.c_str(), '.', NULL, sres, sizeof(sres));
        if (res) {
          json_valid = 1;
          value = sres;
        }
#endif // SUPPORT_MQTT_EVENT_MORE
      }
      if (json_valid) {
        value.trim();
        char sbuffer[128];

        if (!strncmp(lkey.c_str(), "Epoch", 5)) {
          uint32_t ep = atoi(value.c_str()) - (uint32_t)glob_script_mem.epoch_offset;
          snprintf_P(sbuffer, sizeof(sbuffer), PSTR(">%s=%d\n"), event_item.Event.c_str(), ep);
        } else {
          snprintf_P(sbuffer, sizeof(sbuffer), PSTR(">%s=\"%s\"\n"), event_item.Event.c_str(), value.c_str());
        }
#ifdef DEBUG_MQTT_EVENT
        AddLog(LOG_LEVEL_INFO, PSTR("SCR: setting script var %s"), sbuffer);
#endif
        //toLog(sbuffer);
        execute_script(sbuffer);
      }
    }
  }
  return serviced;
}

/********************************************************************************************/
/*
 * Subscribe a MQTT topic (with or without key) and assign an event name to it
 * Command Subscribe format:
 *      Subscribe <event_name>, <topic> [, <key>]
 *        This command will subscribe a <topic> and give it an event name <event_name>.
 *        The optional parameter <key> is for parse the specified key/value from MQTT message
 *            payload with JSON format.
 *      Subscribe
 *        Subscribe command without any parameter will list all topics currently subscribed.
 * Input:
 *      data      - A char buffer with all the parameters
 *      data_len  - Length of the parameters
 * Return:
 *      A string include subscribed event, topic and key.
 */
String ScriptSubscribe(const char *data, int data_len)
{
  MQTT_Subscription subscription_item;
  String events;
  if (data_len > 0) {
    char parameters[data_len + 1];
    memcpy(parameters, data, data_len);
    parameters[data_len] = '\0';
    String event_name, topic, key;

    char * pos = strtok(parameters, ",");
    if (pos) {
      event_name = Trim(pos);
      pos = strtok(nullptr, ",");
      if (pos) {
        topic = Trim(pos);
        pos = strtok(nullptr, ",");
        if (pos) {
          key = Trim(pos);
        }
      }
    }
    //AddLog(LOG_LEVEL_DEBUG, PSTR("Script: Subscribe command with parameters: %s, %s, %s."), event_name.c_str(), topic.c_str(), key.c_str());
    //event_name.toUpperCase();
    if (event_name.length() > 0 && topic.length() > 0) {
      //Search all subscriptions
      for (uint32_t index = 0; index < subscriptions.size(); index++) {
        if (subscriptions.get(index).Event.equals(event_name)) {
          //If find exists one, remove it.
          String stopic = subscriptions.get(index).Topic + "/#";
          MqttUnsubscribe(stopic.c_str());
          subscriptions.remove(index);
          break;
        }
      }
      //Add "/#" to the topic
      if (!topic.endsWith("#")) {
        if (topic.endsWith("/")) {
          topic.concat("#");
        } else {
          topic.concat("/#");
        }
      }
      // AddLog(LOG_LEVEL_DEBUG, PSTR("Script: New topic: %s."), topic.c_str());
      //MQTT Subscribe
      subscription_item.Event = event_name;
      subscription_item.Topic = topic.substring(0, topic.length() - 2);   //Remove "/#" so easy to match
      subscription_item.Key = key;
      subscriptions.add(subscription_item);

      MqttSubscribe(topic.c_str());
      events.concat(event_name + "," + topic
        + (key.length()>0 ? "," : "")
        + key);
    } else {
      events = D_JSON_WRONG_PARAMETERS;
    }
  } else {
    //If did not specify the event name, list all subscribed event
    for (uint32_t index = 0; index < subscriptions.size(); index++) {
      subscription_item = subscriptions.get(index);
      events.concat(subscription_item.Event + "," + subscription_item.Topic
        + (subscription_item.Key.length()>0 ? "," : "")
        + subscription_item.Key + "; ");
    }
  }
  return events;
}

/********************************************************************************************/
/*
 * Unsubscribe specified MQTT event. If no event specified, Unsubscribe all.
 * Command Unsubscribe format:
 *      Unsubscribe [<event_name>]
 * Input:
 *      data      - Event name
 *      data_len  - Length of the parameters
 * Return:
 *      list all the events unsubscribed.
 */
String ScriptUnsubscribe(const char * data, int data_len)
{
  MQTT_Subscription subscription_item;
  String events;
  if (data_len > 0) {
    for (uint32_t index = 0; index < subscriptions.size(); index++) {
      subscription_item = subscriptions.get(index);
      if (subscription_item.Event.equalsIgnoreCase(data)) {
        String stopic = subscription_item.Topic + "/#";
        MqttUnsubscribe(stopic.c_str());
        events = subscription_item.Event;
        subscriptions.remove(index);
        break;
      }
    }
  } else {
    //If did not specify the event name, unsubscribe all event
    String stopic;
    while (subscriptions.size() > 0) {
      events.concat(subscriptions.get(0).Event + "; ");
      stopic = subscriptions.get(0).Topic + "/#";
      MqttUnsubscribe(stopic.c_str());
      subscriptions.remove(0);
    }
  }
  return events;
}
#endif //     SUPPORT_MQTT_EVENT


#if defined(ESP32) && defined(USE_UFILESYS) && defined(USE_SCRIPT_ALT_DOWNLOAD)

#ifndef SCRIPT_DLPORT
#define SCRIPT_DLPORT 82
#endif

ESP8266WebServer *http82_Server;
bool download82_busy;

void script_download_task82(void *path) {
  SendFile_sub((char*) path, 1);
  free(path);
  download82_busy = false;
  //AddLog(LOG_LEVEL_INFO, PSTR("UFS 82: Download finished"));
  vTaskDelete( NULL );
}
void ScriptServeFile82(void) {
  String stmp = http82_Server->uri();

  char *cp = strstr_P(stmp.c_str(), PSTR("/ufs/"));
  if (cp) {
    cp += 4;
    if (ufsp) {
#ifndef USE_FEXTRACT
      if (ufsp->exists(cp)) {
#endif
        if (download82_busy == true) {
          AddLog(LOG_LEVEL_INFO, PSTR("UFS: 82 Download is busy"));
          return;
        }
        download82_busy = true;
        char *path = (char*)malloc(128);
        strcpy(path, cp);
        xTaskCreatePinnedToCore(script_download_task82, "DT", 6000, (void*)path, 3, NULL, 1);
        //AddLog(LOG_LEVEL_INFO, PSTR("Sendfile 82 started"));
        return;
      }
#ifndef USE_FEXTRACT
    }
#endif
  }

  Handle82NotFound();
}

void Handle82NotFound(void) {
  Send82Header(404, "not found");
}

void Handle82Root(void) {
  Send82Header(403, "forbidden");
}

void WebServer82Loop(void) {
  if (http82_Server != nullptr) {
    http82_Server->handleClient();
  }
}

void Send82Header(uint32_t type, const char *message) {
  http82_Server->client().printf_P(PSTR("HTTP/1.1 %d\r\n"), type);
  http82_Server->client().printf_P(PSTR("Content-type: text/plain\r\n\r\n"));
  http82_Server->client().printf_P(PSTR("%s\n"), message);
}

void WebServer82Init(void) {
  if (http82_Server != nullptr) {
    return;
  }
  http82_Server = new ESP8266WebServer(SCRIPT_DLPORT);
  if (http82_Server != nullptr) {
    http82_Server->on(UriGlob("/ufs/*"), HTTP_GET, ScriptServeFile82);
    http82_Server->on("/", HTTP_GET, Handle82Root);
    http82_Server->onNotFound(Handle82NotFound);
    http82_Server->begin();
    AddLog(LOG_LEVEL_INFO, PSTR("SCR: HTTP DL Server started on port: %d "), SCRIPT_DLPORT);
  } else {
    AddLog(LOG_LEVEL_INFO, PSTR("SCR: HTTP DL Server failed"));
  }
}

#endif // USE_SCRIPT_ALT_DOWNLOAD



#ifdef USE_SCRIPT_WEB_DISPLAY

#ifdef USE_UFILESYS

const char HTTP_SCRIPT_MIMES[] PROGMEM =
  "HTTP/1.1 200 OK\r\n"
  "Content-disposition: inline; "
  "Content-type: %s\r\n\r\n";

void ScriptServeFile(void) {

  if (!HttpCheckPriviledgedAccess()) { return; }

  String stmp = Webserver->uri();

  char *cp = strstr_P(stmp.c_str(), PSTR("/ufs/"));

  if (cp) {
    cp += 4;
    if (ufsp) {
      if (strstr_P(cp, PSTR("scrdmp.bmp"))) {
        SendFile(cp);
        return;
      } else {
        if (!SendFile(cp)) {
          return;
        }
      }
    }
  }
  HandleNotFound();
}

extern uint8_t *buffer;
bool script_download_busy;

//#define USE_DLTASK

int32_t SendFile(char *fname) {

#ifdef ESP8266
  return SendFile_sub(fname, 0);
#endif // ESP8266

#ifdef ESP32
#ifdef USE_DLTASK
  if (script_download_busy == true) {
    AddLog(LOG_LEVEL_INFO, PSTR("UFS: Download is busy"));
    return -1;
  }
  script_download_busy = true;
  char *path = (char*)malloc(128);
  strcpy(path, fname);
  xTaskCreatePinnedToCore(script_download_task, "DT", 6000, (void*)path, 3, NULL, 1);
#else
  return SendFile_sub(fname, 0);
#endif

#endif // ESP32
  return 0;
}

#ifdef USE_DLTASK
void script_download_task(void *path) {
  SendFile_sub((char*) path, 0);
  free(path);
  script_download_busy = false;
  vTaskDelete( NULL );
}
#endif // USE_DLTASK

#define REVERT_M5EPD

int32_t SendFile_sub(char *path, uint8_t stype) {
char buff[512];
WiFiClient client;
uint8_t sflg = 0;
File file;
uint32_t fsize;

#ifdef USE_FEXTRACT
  char *lp = strchr(path, '@');
  if (lp) {
    *lp = 0;
    lp++;
    // /ufs/test.txt@1.2.22-00:00_12.2.22-00:00
    char *tp = strchr(lp, '_');
    if (tp) {
      *tp = 0;
      tp++;
      glob_script_mem.from_time = tstamp2l(lp);
      glob_script_mem.to_time = tstamp2l(tp);
    }
  }
#endif // USE_FEXTRACT

#ifdef USE_DISPLAY_DUMP
  char *sbmp = strstr_P(path, PSTR("scrdmp.bmp"));
  if (sbmp) {
    sflg = 1;
  }
#endif // USE_DISPLAY_DUMP

  if ( strstr_P(path, PSTR(".jpg"))) {
    strcpy_P(buff,PSTR("image/jpeg"));
  } else if (strstr_P(path, PSTR(".bmp"))) {
    strcpy_P(buff,PSTR("image/bmp"));
  } else if (strstr_P(path, PSTR(".html"))) {
    strcpy_P(buff,PSTR("text/html"));
  } else if (strstr_P(path, PSTR(".txt"))) {
    strcpy_P(buff,PSTR("text/plain"));
  } else if (strstr_P(path, PSTR(".pdf"))) {
    strcpy_P(buff,PSTR("application/pdf"));
  } else {
    strcpy_P(buff,PSTR("text/plain"));
  }

  if (!buff[0]) return -2;

  if (!sflg) {
    if (!ufsp->exists(path)) {
      return -1;
    }
    file = ufsp->open(path, FS_FILE_READ);
    fsize = file.size();
  }

  if (0 == stype) {
    WSContentSend_P(HTTP_SCRIPT_MIMES, buff);
    WSContentFlush();
    client = Webserver->client();
  } else {
#ifdef USE_SCRIPT_ALT_DOWNLOAD
    client = http82_Server->client();
#else
    client = Webserver->client();
#endif
    client.printf_P(PSTR("HTTP/1.1 200 OK\r\n"));
    char *cp = path;
    for (uint32_t cnt = strlen(path) - 1; cnt >= 0; cnt--) {
      if (path[cnt] == '/') {
        cp = &path[cnt + 1];
        break;
      }
    }
    client.printf_P(PSTR("Content-Disposition: attachment; filename=\"%s\"\r\n"), cp);
    client.printf_P(PSTR("Content-Length: %d\r\n"), fsize);
    client.printf_P(PSTR("Content-type: application/octet-stream\r\n\r\n"));
  }

  if (sflg) {
#ifdef USE_DISPLAY_DUMP
//#include <renderer.h>
//extern Renderer *renderer;

    // screen copy
    #define fileHeaderSize 14
    #define infoHeaderSize 40

    if (renderer && renderer->framebuffer) {
      uint8_t *bp = renderer->framebuffer;
      uint8_t *lbuf = (uint8_t*)special_malloc(Settings->display_width * 3 + 2);
      memset(lbuf, 0, Settings->display_width * 3);
      if (!lbuf) return -3;
      uint8_t dmflg = 0;
      if (renderer->disp_bpp & 0x40) {
        dmflg = 1;
      }
      int8_t bpp = renderer->disp_bpp & 0xbf;;
      uint8_t *lbp;
      uint8_t fileHeader[fileHeaderSize];
      createBitmapFileHeader(Settings->display_height , Settings->display_width , fileHeader);
      client.write((uint8_t *)fileHeader, fileHeaderSize);
      uint8_t infoHeader[infoHeaderSize];
      createBitmapInfoHeader(Settings->display_height, Settings->display_width, infoHeader );
      client.write((uint8_t *)infoHeader, infoHeaderSize);
      if (bpp < 0) {
        for (uint32_t lins = Settings->display_height - 1; lins >= 0 ; lins--) {
          lbp = lbuf;
          for (uint32_t cols = 0; cols < Settings->display_width; cols ++) {
            uint8_t pixel = 0;
            if (bp[cols + (lins / 8) * Settings->display_width] & (1 << (lins & 7))) {
              pixel = 0xff;
            }
            *lbp++ = pixel;
            *lbp++ = pixel;
            *lbp++ = pixel;
          }
          client.write((const char*)lbuf, Settings->display_width * 3);
        }
      } else {
        for (uint32_t lins = 0; lins < Settings->display_height; lins++) {
          lbp = lbuf + (Settings->display_width * 3);
          if (bpp == 4) {
            // 16 gray scales
#ifdef REVERT_M5EPD
            if (dmflg) {
              bp = &renderer->framebuffer[(Settings->display_height - lins) * (Settings->display_width / 2)];
              bp--;
            }
#endif
            for (uint32_t cols = 0; cols < Settings->display_width; cols += 2) {
              uint8_t pixel;
              if (!dmflg) {
                for (uint32_t cnt = 0; cnt <= 1; cnt++) {
                  if (cnt & 1) {
                    pixel = *bp >> 4;
                  } else {
                    pixel = *bp & 0xf;
                  }
                  pixel *= 15;
                  *--lbp = pixel;
                  *--lbp = pixel;
                  *--lbp = pixel;
                }
                bp++;
              } else {
                for (uint32_t cnt = 0; cnt <= 1; cnt++) {
#ifdef REVERT_M5EPD
                  if (cnt & 1) {
#else
                  if (!(cnt & 1)) {
#endif
                    pixel = *bp >> 4;
                  } else {
                    pixel = *bp & 0xf;
                  }
                  pixel *= 15;
                  *--lbp = pixel;
                  *--lbp = pixel;
                  *--lbp = pixel;
                }
#ifdef REVERT_M5EPD
                bp--;
#else
                bp++;
#endif
              }
            }
          } else {
            // one bit
            for (uint32_t cols = 0; cols < Settings->display_width; cols += 8) {
              uint8_t bits = 0x80;
              while (bits) {
                if (!((*bp) & bits)) {
                  *--lbp = 0xff;
                  *--lbp = 0xff;
                  *--lbp = 0xff;
                } else {
                  *--lbp = 0;
                  *--lbp = 0;
                  *--lbp = 0;
                }
                bits = bits>>1;
              }
              bp++;
            }
          }
          client.write((const char*)lbuf, Settings->display_width * 3);
        }
      }
      if (lbuf) free(lbuf);
      client.stop();
    }
#endif // USE_DISPLAY_DUMP
  } else {
#ifdef USE_FEXTRACT
    if (glob_script_mem.to_time > glob_script_mem.from_time) {
      char ts[32];
      s2tstamp(ts, sizeof(ts), glob_script_mem.from_time, 0);
      int32_t fo_from = opt_fext(&file, ts, ts, 1);
      s2tstamp(ts, sizeof(ts), glob_script_mem.to_time, 0);
      //int32_t fo_to = opt_fext(&file, ts, ts, 2);
      int32_t fo_to = extract_from_file(&file,  ts, ts, -3, 0, 0, 0, 0);
      if (fo_to < 0) {
        fo_to = extract_from_file(&file,  ts, ts, -1, 0, 0, 0, 0);
      }
      if (fo_from >= 0 && fo_to >= 0) {
        script_copy_file(&file, 0, fo_from, fo_to, 1, &client);
      }
      file.close();
      client.stop();
      glob_script_mem.to_time = 0;
      glob_script_mem.from_time = 0;
      return 0;
    }
#endif
    uint32_t len = sizeof(buff);
    while (fsize > 0) {
      if (len > fsize) len = fsize;
      file.read((uint8_t *)buff, len);
      client.write((const char*)buff, len);
      fsize -= len;
    }
    file.close();
    client.stop();
  }
  return 0;
}
#endif // USE_UFILESYS


#ifdef SCRIPT_FULL_WEBPAGE
const char HTTP_WEB_FULL_DISPLAY[] PROGMEM =
  "<p><form action='sfd%1d' method='get'><button>%s</button></form></p>";

const char HTTP_SCRIPT_FULLPAGE1[] PROGMEM =
    "var rfsh=1;"
    "function la(p){"
      "var a='';"
      "if(la.arguments.length==1){"
        "a=p;"
        "clearTimeout(lt);"
      "}"
      "if(x!=null){x.abort();}"             // Abort if no response within 2 seconds (happens on restart 1)
      "x=new XMLHttpRequest();"
      "x.onreadystatechange=()=>{"
        "if(x.readyState==4&&x.status==200){"
        //  "var s=x.responseText.replace(/{t}/g,\"<table style='width:100%%'>\").replace(/{s}/g,\"<tr><th>\").replace(/{m}/g,\"</th><td>\").replace(/{e}/g,\"</td></tr>\").replace(/{c}/g,\"%%'><div style='text-align:center;font-weight:\");"
          "var s=x.responseText.replace(/{t}/g,\"<table style='width:100%%'>\").replace(/{s}/g,\"<tr><th>\").replace(/{m}/g,\"</th><td>\").replace(/{e}/g,\"</td></tr>\");"
          "eb('l1').innerHTML=s;"
        "}"
      "};"
      "if (rfsh) {"
        "x.open('GET','./sfd%1d?m=1'+a,true);"       // ?m related to Webserver->hasArg("m")
        "x.send();"
        "lt=setTimeout(la,%d);"               // Settings->web_refresh
      "}"
    "}";

const char HTTP_SCRIPT_FULLPAGE2[] PROGMEM =
    "function seva(par,ivar){"
      "la('&sv='+ivar+'_'+par);"
    "}"
    "function siva(par,ivar){"
      "rfsh=1;"
      "la('&sv='+ivar+'_'+par);"
      "rfsh=0;"
    "}"
    "function sivat(par,ivar){"
      "rfsh=1;"
      // remove : from time string
      "par = par.slice(0, 2) + par.slice(3);"
      "la('&sv='+ivar+'_'+par);"
      "rfsh=0;"
    "}"
    "function pr(f){"
      "if (f) {"
        "lt=setTimeout(la,%d);"
        "rfsh=1;"
      "} else {"
        "clearTimeout(lt);"
        "rfsh=0;"
      "}"
    "}"
    "</script>";

void ScriptFullWebpage1(void) {
  ScriptFullWebpage(1);
}
void ScriptFullWebpage2(void) {
  ScriptFullWebpage(2);
}
void ScriptFullWebpage3(void) {
  ScriptFullWebpage(3);
}
void ScriptFullWebpage4(void) {
  ScriptFullWebpage(4);
}
void ScriptFullWebpage5(void) {
  ScriptFullWebpage(5);
}
void ScriptFullWebpage6(void) {
  ScriptFullWebpage(6);
}
void ScriptFullWebpage7(void) {
  ScriptFullWebpage(7);
}

void ScriptFullWebpage(uint8_t page) {
  uint32_t fullpage_refresh = 10000;
  if (!HttpCheckPriviledgedAccess()) { return; }

  String stmp = Webserver->uri();

  if (Webserver->hasArg("m")) {     // Status refresh requested
    if (Webserver->hasArg("sv")) {
      Script_Check_HTML_Setvars();
    }
      WSContentBegin(200, CT_HTML);
      ScriptWebShow('w', page);
      WSContentEnd();
      //Serial.printf("fwp update sv %s\n",stmp.c_str() );
      return; //goto redraw;
//    } else {
  //    Serial.printf("fwp update %s\n",stmp.c_str() );
  //  }

    return;
  } else {
    //Serial.printf("fwp other %s\n",stmp.c_str() );
  }

  WSContentBegin(200, CT_HTML);
  WSContentSend_P(HTTP_HEADER1, PSTR(D_HTML_LANGUAGE), SettingsText(SET_DEVICENAME), PSTR("Full Screen"));
  WSContentSend_P(HTTP_SCRIPT_FULLPAGE1, page , fullpage_refresh);
  WSContentSend_P(HTTP_SCRIPT_FULLPAGE2, fullpage_refresh);
  //WSContentSend_P(PSTR("<div id='l1' name='l1'></div>"));

  //WSContentSendStyle();

  WSContentSend_P(PSTR("<div id='l1' name='l1'>"));
  ScriptWebShow('w', page);
  WSContentSend_P(PSTR("</div>"));

  ScriptWebShow('x', page);

  WSContentStop();
}
#endif //SCRIPT_FULL_WEBPAGE

void Script_Check_HTML_Setvars(void) {

  if (!HttpCheckPriviledgedAccess()) { return; }

  //if (Webserver->hasArg("gv")) {
    // get variable
  //  String stmp = Webserver->arg("gv");
  //}

  if (Webserver->hasArg("sv")) {
    // set variable
    String stmp = Webserver->arg("sv");
    //Serial.printf("fwp has arg dv %s\n", stmp.c_str());
    char cmdbuf[64];
    memset(cmdbuf, 0, sizeof(cmdbuf));
    char *cp = cmdbuf;
    *cp++ = '>';
    strncpy(cp, stmp.c_str(), sizeof(cmdbuf) - 1);
    char *cp1 = strchr(cp, '_');
    if (!cp1) return;
    *cp1 = 0;
    char vname[32];
    strncpy(vname, cp, sizeof(vname));
    *cp1 = '=';
    cp1++;

    if (is_int_var(vname)) {
      memmove(cp1 + 1, cp1, strlen(cp1));
      *cp1++ = '#';
    }

    struct T_INDEX ind;
    uint8_t vtype;
    isvar(vname, &vtype, &ind, 0, 0, 0);
    if (vtype != NUM_RES && vtype & STYPE) {
      // string type must insert quotes
      uint8_t tlen = strlen(cp1);
      memmove(cp1 + 1, cp1, tlen);
      *cp1 = '\"';
      *(cp1 + tlen +1 ) = '\"';
    }
    //toLog(cmdbuf);
    execute_script(cmdbuf);

#ifdef USE_HTML_CALLBACK
    if (glob_script_mem.html_script) Run_Scripter1(glob_script_mem.html_script, 0, 0);
#else
    if (glob_script_mem.event_script) Run_Scripter1(glob_script_mem.event_script, 0, 0);
#endif
  }
}


const char SCRIPT_MSG_BUTTONa[] PROGMEM =
  "<button type='submit' style=\"width:%d%%\" onclick='seva(%d,\"%s\")'>%s</button>";

const char SCRIPT_MSG_BUTTONa_TBL[] PROGMEM =
  "<td style=\"width:%d%%\"><button type='submit' onclick='seva(%d,\"%s\")'>%s</button></td>";

const char SCRIPT_MSG_BUTTONb[] PROGMEM =
  "<img width=\"%d%%\"></img>";

const char SCRIPT_MSG_BUT_START[] PROGMEM =
  "<div>";
const char SCRIPT_MSG_BUT_START_TBL[] PROGMEM =
  "<table style='width:100%%'><tr>";

const char SCRIPT_MSG_BUT_STOP[] PROGMEM =
  "</div>";
const char SCRIPT_MSG_BUT_STOP_TBL[] PROGMEM =
  "</tr></table>";

const char SCRIPT_MSG_SLIDER[] PROGMEM =
  "<div><span class='p'>%s</span><center><b>%s</b><span class='q'>%s</span></div>"
  "<div><input type='range' min='%d' max='%d' value='%d' onchange='seva(value,\"%s\")'></div>";

const char SCRIPT_MSG_CHKBOX[] PROGMEM =
  "%s<label><b>%s</b><input type='checkbox' %s onchange='seva(%d,\"%s\")'></label>";

const char SCRIPT_MSG_PULLDOWNa[] PROGMEM =
  "%s<label for=\'pu_%s\'>%s</label><select style='width:%dpx' name='pu%d' id='pu_%s' onchange='seva(value,\"%s\")'>";
const char SCRIPT_MSG_PULLDOWNb[] PROGMEM =
  "<option %s value='%d'>%s</option>";
const char SCRIPT_MSG_PULLDOWNc[] PROGMEM =
  "</select>";

const char SCRIPT_MSG_RADIOa[] PROGMEM =
  "%s<fieldset style='width:%dpx'><legend>%s</legend>";
const char SCRIPT_MSG_RADIOa0[] PROGMEM =
  "%s<fieldset><legend>%s</legend>";
const char SCRIPT_MSG_RADIOb[] PROGMEM =
  "<div align='left'><input type='radio' name='%s' onclick='seva(%d%,\"%s\")'%s>"
	"<label>%s</label></div>";
const char SCRIPT_MSG_RADIOc[] PROGMEM =
  "</fieldset>";

const char SCRIPT_MSG_TEXTINP[] PROGMEM =
  "%s<label><b>%s</b><input type='text'  value='%s' style='width:%dpx'  onfocusin='pr(0)' onfocusout='pr(1)' onchange='siva(value,\"%s\")'></label>";

const char SCRIPT_MSG_TEXTINP_U[] PROGMEM =
  "%s<label><b>%s</b><input type='%s'  value='%s' min='%s' max='%s' style='width:%dpx'  onfocusin='pr(0)' onfocusout='pr(1)' onchange='%s(value,\"%s\")'></label>";

const char SCRIPT_MSG_NUMINP[] PROGMEM =
  "%s<label><b>%s</b><input  min='%s' max='%s' step='%s' value='%s' type='number' style='width:%dpx' onfocusin='pr(0)' onfocusout='pr(1)' onchange='siva(value,\"%s\")'></label>";


#ifdef USE_GOOGLE_CHARTS
const char SCRIPT_MSG_GTABLE[] PROGMEM =
  "<script type='text/javascript' src='https://www.gstatic.com/charts/loader.js'></script>"
  "<script type='text/javascript'>google.charts.load('current',{packages:['corechart']});</script>"
  "<style>.hRow{font-weight:bold;color:black;background-color:lightblue;}.hCol{font-weight:bold;color:black;background-color:lightblue;}.tCell{color:black}</style>"
  "<style>#chart1{display: inline-block;margin: 0 auto;#timeline text{fill:magenta;}}</style>";

const char SCRIPT_MSG_TABLE[] PROGMEM =
  "<script type='text/javascript'>google.charts.load('current',{packages:['table']});</script>";
const char SCRIPT_MSG_GAUGE[] PROGMEM =
  "<script type='text/javascript'>google.charts.load('current',{packages:['gauge']});</script>";
const char SCRIPT_MSG_TIMELINE[] PROGMEM =
  "<script type='text/javascript'>google.charts.load('current',{packages:['timeline']});</script>";


const char SCRIPT_MSG_GTABLEa[] PROGMEM =
  "<script language='JavaScript'>function drawChart(){"
  "var cssc={'headerRow':'hRow','rowNumberCell':'hCol','tableCell':'tCell'};"
  "var data=google.visualization.arrayToDataTable([";


const char SCRIPT_MSG_GTABLEd[] PROGMEM =
"['Timeline','start','end'],";
const char SCRIPT_MSG_GTABLEe[] PROGMEM =
"['Timeline','Label','start','end'],";

//#define CHART_EXTRA_OPTIONS ",width:'640px',height:'480px'"
#define CHART_EXTRA_OPTIONS

const char SCRIPT_MSG_GTABLEb[] PROGMEM =
 "]);"
 "var options={%s" CHART_EXTRA_OPTIONS "};";

const char SCRIPT_MSG_GTABLEbx[] PROGMEM =
 "var chart=new google.visualization.%s(document.getElementById('chart%1d'));"
 "chart.draw(data,options);}"
 "google.charts.setOnLoadCallback(drawChart);</script>";

const char SCRIPT_MSG_GOPT1[] PROGMEM =
"title:'%s',isStacked:%s";


const char SCRIPT_MSG_GAUGEOPT[] PROGMEM =
"max:%d,redFrom:%d,redTo:%d,yellowFrom:%d,yellowTo:%d";

const char SCRIPT_MSG_GOPT2[] PROGMEM =
"showRowNumber:true,sort:'disable',allowHtml:true,width:'100%%',height:'100%%',cssClassNames:cssc";

const char SCRIPT_MSG_GOPT3[] PROGMEM =
"title:'%s',isStacked:%s,vAxes:{0:{maxValue:%s},1:{maxValue:%s}},series:{0:{targetAxisIndex:0},1:{targetAxisIndex:1}}%s";

const char SCRIPT_MSG_GOPT4[] PROGMEM =
//"hAxis:{minValue:new Date(0,1,1,0,0),maxValue:new Date(0,1,2,0,0),format:'HH:mm'}";
"hAxis:{format:'HH:mm',minValue:new Date(0,0,0,0,0),maxValue:new Date(0,0,0,23,59)},theme: 'maximized',timeline:{tooltipDateFormat:'HH:mm'},";

const char SCRIPT_MSG_GOPT5[] PROGMEM =
"new Date(0,0,0,%d,%d)";

const char SCRIPT_MSG_GOPT6[] PROGMEM =
"title:'%s',isStacked:%s,vAxis:{viewWindow:{min:%s,max:%s}}%s";

const char SCRIPT_MSG_GTE1[] PROGMEM = "'%s'";

#define GLIBS_MAIN 1<<0
#define GLIBS_TABLE 1<<1
#define GLIBS_GAUGE 1<<2
#define GLIBS_TIMELINE 1<<3

#ifndef MAX_GARRAY
#define MAX_GARRAY 4
#endif

char *gc_get_arrays(char *lp, TS_FLOAT **arrays, uint8_t *ranum, uint16_t *rentries, uint16_t *ipos);
char *gc_get_arrays(char *lp, TS_FLOAT **arrays, uint8_t *ranum, uint16_t *rentries, uint16_t *ipos) {
struct T_INDEX ind;
uint8_t vtype;
uint16 entries = 0;
uint16_t cipos = 0;

  uint8_t anum = 0;
  while (anum < MAX_GARRAY) {
    if (*lp == ')' || *lp == 0) break;
    char *lp1 = lp;
    TS_FLOAT sysvar;
    lp = isvar(lp, &vtype, &ind, &sysvar, 0, 0);
    if (vtype != VAR_NV) {
      SCRIPT_SKIP_SPACES
      uint8_t index = glob_script_mem.type[ind.index].index;
      if ((vtype & STYPE) == 0) {
        // numeric result
        //Serial.printf("numeric %d - %d \n",ind.index,index);
        if (glob_script_mem.type[ind.index].bits.is_filter) {
          //Serial.printf("numeric array\n");
          uint16_t len = 0;
          TS_FLOAT *fa = Get_MFAddr(index, &len, &cipos);
          //Serial.printf(">> 2 %d\n",len);
          if (fa && len >= entries) {
            if (!entries) {
              entries = len;
            }
            // add array to list
            arrays[anum] = fa;
            anum++;
          }
        } else {
          // single numeric
          arrays[anum] = &glob_script_mem.fvars[index];
          anum++;
          entries = 1;
        }
      } else {
        lp = lp1;
        break;
      }
    }
  }
  //Serial.printf(">> %d - %d - %d\n",anum,entries,(uint32_t)*arrays[0]);
  *ranum = anum;
  *rentries = entries;
  *ipos = cipos;
  return lp;
}

char *gc_send_labels(char *lp,uint32_t anum) {
  WSContentSend_P("[");
  for (uint32_t cnt = 0; cnt < anum + 1; cnt++) {
    char label[SCRIPT_MAXSSIZE];
    lp = GetStringArgument(lp, OPER_EQU, label, 0);
    SCRIPT_SKIP_SPACES
    WSContentSend_P(SCRIPT_MSG_GTE1, label);
    //Serial.printf("labels %s\n",label);
    if (cnt<anum) { WSContentSend_P(","); }
  }
  WSContentSend_P("],");
  return lp;
}

const char *GC_type(uint8_t type) {
  switch (type) {
    case 'l':
      return PSTR("LineChart");
    case 'b':
      return PSTR("BarChart");
    case 'p':
      return PSTR("PieChart");
    case 'g':
      return PSTR("Gauge");
    case 't':
      return PSTR("Table");
    case 'T':
      return PSTR("Timeline");
    case 'h':
      return PSTR("Histogram");
    case 'c':
      return PSTR("ColumnChart");
    case 'C':
      return PSTR("ComboChart");
    default:
      return PSTR("*");
  }
}


#endif // USE_GOOGLE_CHARTS

void ScriptGetVarname(char *nbuf,char *sp, uint32_t blen) {
uint32_t cnt;
  for (cnt = 0; cnt < blen - 1; cnt++) {
    if (*sp==' ' || *sp==')') {
      break;
    }
    nbuf[cnt] = *sp++;
  }
  nbuf[cnt] = 0;
}

#define WSO_NOCENTER 1
#define WSO_NODIV 2
#define WSO_FORCEPLAIN 4
#define WSO_FORCEMAIN 8
#define WSO_FORCEGUI 16
#define WSO_STOP_DIV 0x80

void WCS_DIV(uint8_t flag) {
  if (flag & WSO_NODIV) return;
  if (flag & WSO_STOP_DIV) {
    WSContentSend_P(SCRIPT_MSG_BUT_STOP);
  } else {
    WSContentSend_P(SCRIPT_MSG_BUT_START);
  }
}


uint8_t specopt;
uint8_t chartindex;
uint8_t google_libs;
#ifdef USE_GOOGLE_CHARTS
char gs_ctype;
#endif

void ScriptWebShow(char mc, uint8_t page) {
  TS_FLOAT cv_max = 0;
  TS_FLOAT cv_inc = 0;
  TS_FLOAT *cv_count = 0;
  char *cv_ptr;

  //uint8_t web_script;
  glob_script_mem.web_mode = mc;
  if (mc == 'w' || mc == 'x' || page >= WEB_PAGE_WS) {
    if (mc == 'x') {
      mc = '$';
    }
    glob_script_mem.section_ptr = glob_script_mem.web_pages[page];
  } else {
    glob_script_mem.section_ptr = glob_script_mem.web_pages[0];
  }

  if (glob_script_mem.section_ptr) {

    chartindex = 1;
    google_libs = 0;
    char *lp = glob_script_mem.section_ptr;
    if (mc == 'w') {
      while (*lp) {
        if (*lp == '\n') break;
        lp++;
      }
    }

    specopt = 0;
    while (lp) {
      while (*lp == SCRIPT_EOL) {
       lp++;
      }
      if (!*lp || *lp=='#' || *lp=='>') {
          break;
      }
      if (*lp != ';') {
        // send this line to web
        SCRIPT_SKIP_SPACES
        if (!strncmp(lp, "%for ", 5)) {
          // for next loop
          struct T_INDEX ind;
          uint8_t vtype;
          lp = isvar(lp + 5, &vtype, &ind, 0, 0, 0);
          if ((vtype != VAR_NV) && (vtype & STYPE) == 0) {
            uint16_t index = glob_script_mem.type[ind.index].index;
            cv_count = &glob_script_mem.fvars[index];
            SCRIPT_SKIP_SPACES
            lp = GetNumericArgument(lp , OPER_EQU, cv_count, 0);
            SCRIPT_SKIP_SPACES
            lp = GetNumericArgument(lp , OPER_EQU, &cv_max, 0);
            SCRIPT_SKIP_SPACES
            lp = GetNumericArgument(lp , OPER_EQU, &cv_inc, 0);
            cv_ptr = lp;
            //goto nextwebline;
          } else {
            continue;
          }
        } else if (!strncmp(lp, "%next", 5)) {
          if (cv_count) {
            // for next loop
            *cv_count += cv_inc;
            if (*cv_count <= cv_max) {
              lp = cv_ptr;
            } else {
              cv_count = 0;
              //goto nextwebline;
            }
          } else {
            //goto nextwebline;
          }
        } else if (!strncmp(lp, "%=#", 3)) {
          // subroutine
          uint8_t sflg = specopt;
          specopt = WSO_FORCEPLAIN;
          lp = scripter_sub(lp + 1, 0);
          specopt = sflg;
          //goto nextwebline;
        } else if (!strncmp(lp, "%/", 2)) {
          // send file
          if (mc) {
            web_send_file(mc, lp + 1);
          }
        } else {
          web_send_line(mc, lp);
        }
      }
nextwebline:
      if (*lp == SCRIPT_EOL) {
        lp++;
      } else {
        lp = strchr(lp, SCRIPT_EOL);
        if (!lp) break;
        lp++;
      }
    }
  }
}

#define WSF_BSIZE 256
int32_t web_send_file(char mc, char *fname) {
  char path[32];

#ifdef USE_UFILESYS

  char *lbuff = (char*)special_malloc(WSF_BSIZE);
  if (!lbuff) {
    return -1;
  }

  cpy2lf(path, sizeof(path), fname);
  File file = ufsp->open(path, FS_FILE_READ);
  if (file) {
    WSContentFlush();
    while (file.available()) {
      uint16_t len;
      len = file.readBytesUntil('\n', lbuff, WSF_BSIZE);
      lbuff[len] = 0;
      char *lp = lbuff;
      while (*lp == ' ') lp++;
      if (*lp == '/' && *(lp + 1) == '/') {
        // skip comment lines
        continue;
      }
      web_send_line(mc, lbuff);
    }
    file.close();
    free(lbuff);
    return 0;
  } else {
    AddLog(LOG_LEVEL_INFO, PSTR("SCR: WEB file %s not found"), path);
  }
  free(lbuff);
#endif
  return -2;
}

char *web_send_line(char mc, char *lp1) {
char tmp[256];
char center[10];
uint8_t optflg = 0;
const char *gc_str;

  Replace_Cmd_Vars(lp1, 1, tmp, sizeof(tmp));

  char *lin = tmp;

  if (!strncmp(lin, "so(", 3)) {
    // set options
    TS_FLOAT var;
    lin = GetNumericArgument(lin + 3, OPER_EQU, &var, 0);
    specopt = var;
    return lin;
  }

  if (specopt & WSO_NOCENTER) {
    center[0] = 0;
  } else {
    strcpy_P(center, PSTR("<center>"));
  }

  bool dogui = ((!mc && (*lin != '$')) || (mc == 'w' && (*lin != '$'))) && (!(specopt & WSO_FORCEMAIN));

  if ((dogui && !(specopt & WSO_FORCEGUI)) || (!dogui && (specopt & WSO_FORCEGUI))) {
  //if ( ((!mc && (*lin != '$')) || (mc == 'w' && (*lin != '$'))) && (!(specopt & WSO_FORCEMAIN)) || (specopt & WSO_FORCEGUI)) {
    // normal web section
    //AddLog(LOG_LEVEL_INFO, PSTR("normal %s"), lin);
    if (*lin == '@') {
      lin++;
      optflg = 1;
    } else {
      optflg = 0;
    }

    if (specopt & WSO_NOCENTER) {
      center[0] = 0;
    } else {
      strcpy_P(center, PSTR("<center>"));
    }

    if (!strncmp(lin, "sl(", 3)) {
      // insert slider sl(min max var left mid right)
      char *lp = lin;
      TS_FLOAT min;
      lp = GetNumericArgument(lp + 3, OPER_EQU, &min, 0);
      SCRIPT_SKIP_SPACES
      // arg2
      TS_FLOAT max;
      lp = GetNumericArgument(lp, OPER_EQU, &max, 0);
      SCRIPT_SKIP_SPACES
      TS_FLOAT val;
      char *slp = lp;
      lp = GetNumericArgument(lp, OPER_EQU, &val, 0);
      SCRIPT_SKIP_SPACES

      char vname[16];
      ScriptGetVarname(vname, slp, sizeof(vname));

      char left[SCRIPT_MAXSSIZE];
      lp = GetStringArgument(lp, OPER_EQU, left, 0);
      SCRIPT_SKIP_SPACES
      char mid[SCRIPT_MAXSSIZE];
      lp = GetStringArgument(lp, OPER_EQU, mid, 0);
      SCRIPT_SKIP_SPACES
      char right[SCRIPT_MAXSSIZE];
      lp = GetStringArgument(lp, OPER_EQU, right, 0);
      SCRIPT_SKIP_SPACES

      WSContentSend_P(SCRIPT_MSG_SLIDER, left, mid, right, (uint32_t)min, (uint32_t)max, (uint32_t)val, vname);
      lp++;

    } else if (!strncmp(lin, "ck(", 3)) {
      char *lp = lin + 3;
      char *slp = lp;
      TS_FLOAT val;
      lp = GetNumericArgument(lp, OPER_EQU, &val, 0);
      SCRIPT_SKIP_SPACES

      char vname[16];
      ScriptGetVarname(vname, slp, sizeof(vname));

      char label[SCRIPT_MAXSSIZE];
      lp = GetStringArgument(lp, OPER_EQU, label, 0);
      const char *cp;
      uint8_t uval;
      if (val>0) {
        cp = "checked='checked'";
        uval = 0;
      } else {
        cp = "";
        uval = 1;
      }
      WCS_DIV(specopt);
      WSContentSend_P(SCRIPT_MSG_CHKBOX, center, label, (char*)cp, uval, vname);
      WCS_DIV(specopt | WSO_STOP_DIV);
      lp++;
    } else if (!strncmp(lin, "pd(", 3)) {
      // pull down
      char *lp = lin + 3;
      char *slp = lp;
      TS_FLOAT val;
      lp = GetNumericArgument(lp, OPER_EQU, &val, 0);
      SCRIPT_SKIP_SPACES

      char vname[16];
      ScriptGetVarname(vname, slp, sizeof(vname));

      SCRIPT_SKIP_SPACES
      char pulabel[SCRIPT_MAXSSIZE];
      lp = GetStringArgument(lp, OPER_EQU, pulabel, 0);
      SCRIPT_SKIP_SPACES

      glob_script_mem.glob_error = 0;
      uint16_t tsiz = 200;
      TS_FLOAT fvar;
      char *slp1 = lp;
      lp = GetNumericArgument(lp, OPER_EQU, &fvar, 0);
      if (!glob_script_mem.glob_error) {
        tsiz = fvar;
      } else {
        lp = slp1;
      }
      WCS_DIV(specopt);
      WSContentSend_P(SCRIPT_MSG_PULLDOWNa, center, vname, pulabel, tsiz, 1, vname, vname);

      // get pu labels
      uint8_t index = 1;
      while (*lp) {
        SCRIPT_SKIP_SPACES
        lp = GetStringArgument(lp, OPER_EQU, pulabel, 0);
        if (index == 1 && pulabel[0] == '#') {
          // number range
          char *cp = &pulabel[1];
          uint8_t from = strtol(cp, &cp, 10);
          uint8_t to = from;
          if (*cp == '-') {
            cp++;
            to = strtol(cp, &cp, 10);
          }
          for (uint32_t cnt = from; cnt <= to; cnt++) {
            sprintf(pulabel, "%d", cnt);
            if (val == index) {
              cp = (char*)"selected";
            } else {
              cp = (char*)"";
            }
            WSContentSend_P(SCRIPT_MSG_PULLDOWNb, cp, index, pulabel);
            index++;
          }
          break;
        }

        char *cp;
        if (val == index) {
          cp = (char*)"selected";
        } else {
          cp = (char*)"";
        }
        WSContentSend_P(SCRIPT_MSG_PULLDOWNb, cp, index, pulabel);
        SCRIPT_SKIP_SPACES
        if (*lp == ')') {
          lp++;
          break;
        }
        index++;
      }
      WSContentSend_P(SCRIPT_MSG_PULLDOWNc);
      WCS_DIV(specopt | WSO_STOP_DIV);
    } else if (!strncmp(lin, "rb(", 3)) {
      // radio buttons
      char *lp = lin + 3;
      char *slp = lp;
      TS_FLOAT val;
      lp = GetNumericArgument(lp, OPER_EQU, &val, 0);
      SCRIPT_SKIP_SPACES

      char vname[16];
      ScriptGetVarname(vname, slp, sizeof(vname));

      SCRIPT_SKIP_SPACES
      char pulabel[SCRIPT_MAXSSIZE];
      lp = GetStringArgument(lp, OPER_EQU, pulabel, 0);
      SCRIPT_SKIP_SPACES

      glob_script_mem.glob_error = 0;
      int16_t tsiz = -1;
      TS_FLOAT fvar;
      char *slp1 = lp;
      lp = GetNumericArgument(lp, OPER_EQU, &fvar, 0);
      if (!glob_script_mem.glob_error) {
        tsiz = fvar;
      } else {
        lp = slp1;
      }

      WCS_DIV(specopt);
      if (tsiz < 0) {
        WSContentSend_P(SCRIPT_MSG_RADIOa0, center, pulabel);
      } else {
        WSContentSend_P(SCRIPT_MSG_RADIOa, center, tsiz, pulabel);
      }

      // get pu labels
      uint8_t index = 1;
      while (*lp) {
        SCRIPT_SKIP_SPACES
        lp = GetStringArgument(lp, OPER_EQU, pulabel, 0);
        char *cp;
        if (val == index) {
          cp = (char*)"checked";
        } else {
          cp = (char*)"";
        }
        WSContentSend_P(SCRIPT_MSG_RADIOb, vname, index, vname, cp, pulabel);
        SCRIPT_SKIP_SPACES
        if (*lp == ')') {
          lp++;
          break;
        }
        index++;
      }
      WSContentSend_P(SCRIPT_MSG_RADIOc);
      WCS_DIV(specopt | WSO_STOP_DIV);
      WSContentFlush();
    } else if (!strncmp(lin, "bu(", 3)) {
      char *lp = lin + 3;
      uint8_t bcnt = 0;
      char *found = lin;
      while (bcnt < 4) {
        found = strstr(found, "bu(");
        if (!found) break;
        found += 3;
        bcnt++;
      }
      uint8_t proz = 100 / bcnt;
      if (!optflg && bcnt>1) proz -= 2;
      if (optflg) WSContentSend_P(SCRIPT_MSG_BUT_START_TBL);
      else WSContentSend_P(SCRIPT_MSG_BUT_START);
      for (uint32_t cnt = 0; cnt < bcnt; cnt++) {
        TS_FLOAT val;
        char *slp = lp;
        lp = GetNumericArgument(lp, OPER_EQU, &val, 0);
        SCRIPT_SKIP_SPACES

        char vname[16];
        ScriptGetVarname(vname, slp, sizeof(vname));

        SCRIPT_SKIP_SPACES
        char ontxt[SCRIPT_MAXSSIZE];
        lp = GetStringArgument(lp, OPER_EQU, ontxt, 0);
        SCRIPT_SKIP_SPACES
        char offtxt[SCRIPT_MAXSSIZE];
        lp = GetStringArgument(lp, OPER_EQU, offtxt, 0);

        char *cp;
        uint8_t uval;
        if (val>0) {
          cp = ontxt;
          uval = 0;
        } else {
          cp = offtxt;
          uval = 1;
        }
        if (bcnt > 1 && cnt == bcnt - 1) {
          if (!optflg) proz += 2;
        }
        if (!optflg) {
          WSContentSend_P(SCRIPT_MSG_BUTTONa, proz, uval, vname, cp);
        } else {
          WSContentSend_P(SCRIPT_MSG_BUTTONa_TBL, proz, uval, vname, cp);
        }
        if (bcnt > 1 && cnt < bcnt - 1) {
          if (!optflg) WSContentSend_P(SCRIPT_MSG_BUTTONb, 2);
        }
        lp += 4;
      }
      if (optflg) WSContentSend_P(SCRIPT_MSG_BUT_STOP_TBL);
      else WSContentSend_P(SCRIPT_MSG_BUT_STOP);

    }  else if (!strncmp(lin, "tm(", 3)) {
      // time only HH:MM
      TS_FLOAT val;
      char *lp = lin + 3;
      char *slp = lp;
      lp = GetNumericArgument(lp, OPER_EQU, &val, 0);
      SCRIPT_SKIP_SPACES
      char vname[16];
      ScriptGetVarname(vname, slp, sizeof(vname));
      SCRIPT_SKIP_SPACES
      char label[SCRIPT_MAXSSIZE];
      lp = GetStringArgument(lp, OPER_EQU, label, 0);
      SCRIPT_SKIP_SPACES
      uint16_t tsiz = 70;
      if (*lp != ')') {
        TS_FLOAT fvar;
        lp = GetNumericArgument(lp, OPER_EQU, &fvar, 0);
        tsiz = fvar;
      }
      lp++;
      char vstr[16];
      uint32_t tv = val;
      sprintf_P(vstr,PSTR("%02d:%02d"),tv / 100, tv % 100);
      const char *type = PSTR("time");
      const char *min = PSTR("00:00");
      const char *max = PSTR("23:59");
      const char *styp = PSTR("sivat");
      WCS_DIV(specopt);
      WSContentSend_P(SCRIPT_MSG_TEXTINP_U, center, label, type, vstr, min, max, tsiz, styp, vname);
      WCS_DIV(specopt | WSO_STOP_DIV);
    }  else if (!strncmp(lin, "tx(", 3)) {
      // text
      char *lp = lin + 3;
      char *slp = lp;
      char str[SCRIPT_MAXSSIZE];
      lp = ForceStringVar(lp, str);
      SCRIPT_SKIP_SPACES
      char label[SCRIPT_MAXSSIZE];
      lp = GetStringArgument(lp, OPER_EQU, label, 0);
      char vname[16];
      ScriptGetVarname(vname, slp, sizeof(vname));
      SCRIPT_SKIP_SPACES

      uint16_t tsiz = 200;
      if (*lp != ')') {
        glob_script_mem.glob_error = 0;
        TS_FLOAT fvar;
        char *slp1 = lp;
        lp = GetNumericArgument(lp, OPER_EQU, &fvar, 0);
        SCRIPT_SKIP_SPACES
        if (!glob_script_mem.glob_error) {
          tsiz = fvar;
        } else {
          lp = slp1;
        }

        if (*lp != ')') {
          char type[SCRIPT_MAXSSIZE];
          lp = GetStringArgument(lp, OPER_EQU, type, 0);
          SCRIPT_SKIP_SPACES
          // also requires min max values
          char min[SCRIPT_MAXSSIZE];
          lp = GetStringArgument(lp, OPER_EQU, min, 0);
          SCRIPT_SKIP_SPACES
          char max[SCRIPT_MAXSSIZE];
          lp = GetStringArgument(lp, OPER_EQU, max, 0);
          SCRIPT_SKIP_SPACES
          WCS_DIV(specopt);
          const char *styp = PSTR("siva");
          WSContentSend_P(SCRIPT_MSG_TEXTINP_U, center, label, type, str, min, max, tsiz, styp, vname);
          WCS_DIV(specopt | WSO_STOP_DIV);
        } else {
          WCS_DIV(specopt);
          WSContentSend_P(SCRIPT_MSG_TEXTINP, center, label, str, tsiz, vname);
          WCS_DIV(specopt | WSO_STOP_DIV);
        }
      } else {
        WCS_DIV(specopt);
        WSContentSend_P(SCRIPT_MSG_TEXTINP, center, label, str, tsiz, vname);
        WCS_DIV(specopt | WSO_STOP_DIV);
      }
      lp++;
      //goto restart;

    } else if (!strncmp(lin, "nm(", 3)) {
      char *lp = lin;
      TS_FLOAT min;
      lp = GetNumericArgument(lp + 3, OPER_EQU, &min, 0);
      SCRIPT_SKIP_SPACES
      TS_FLOAT max;
      lp = GetNumericArgument(lp, OPER_EQU, &max, 0);
      SCRIPT_SKIP_SPACES
      TS_FLOAT step;
      lp = GetNumericArgument(lp, OPER_EQU, &step, 0);
      SCRIPT_SKIP_SPACES
      TS_FLOAT val;
      char *slp = lp;
      lp = GetNumericArgument(lp, OPER_EQU, &val, 0);
      SCRIPT_SKIP_SPACES
      char vname[16];
      ScriptGetVarname(vname, slp, sizeof(vname));

      bool isint = is_int_var(vname);

      char label[SCRIPT_MAXSSIZE];
      lp = GetStringArgument(lp, OPER_EQU, label, 0);
      SCRIPT_SKIP_SPACES
      uint16_t tsiz = 200;
      uint8_t dprec = 1;
      if (*lp != ')') {
        TS_FLOAT val;
        lp = GetNumericArgument(lp, OPER_EQU, &val, 0);
        SCRIPT_SKIP_SPACES
        tsiz = val;
        if (*lp != ')') {
          lp = GetNumericArgument(lp, OPER_EQU, &val, 0);
          dprec = val;
        }
      }

      char vstr[16],minstr[16],maxstr[16],stepstr[16];
      if (isint) {
        dtostrfd(*(int32_t*)&val, 0, vstr);
        dtostrfd(*(int32_t*)&min, dprec, minstr);
        dtostrfd(*(int32_t*)&max, dprec, maxstr);
        dtostrfd(*(int32_t*)&step, dprec, stepstr);
      } else {
        dtostrfd(val, dprec, vstr);
        dtostrfd(min, dprec, minstr);
        dtostrfd(max, dprec, maxstr);
        dtostrfd(step, dprec, stepstr);
      }
      WCS_DIV(specopt);
      WSContentSend_P(SCRIPT_MSG_NUMINP, center, label, minstr, maxstr, stepstr, vstr, tsiz, vname);
      WCS_DIV(specopt | WSO_STOP_DIV);
      lp++;

    } else {
      if (mc == 'w' || (specopt & WSO_FORCEPLAIN)) {
        WSContentSend_P(PSTR("%s"), lin);
      } else {
        if (optflg) {
          WSContentSend_P(PSTR("<div>%s</div>"), lin);
        } else {
          WSContentSend_P(PSTR("{s}%s{e}"), lin);
        }
      }
    }
    // end standard web interface
  } else {
    //  main section interface
    //AddLog(LOG_LEVEL_INFO, PSTR("main %s"), lin);
    if ( (*lin == mc) || (mc == 'z') || (specopt & WSO_FORCEMAIN)) {

#ifdef USE_GOOGLE_CHARTS
      if (mc != 'z') {
        if (!(specopt & WSO_FORCEMAIN)) {
          lin++;
        }
      }
exgc:
      char *lp;

      char *cp = strstr_P(lin, PSTR("insa("));
      if (cp) {
        // insert array
        char valstr[128];
        uint16_t len = (uint32_t)cp - (uint32_t)lin;
        strncpy(valstr, lin, len);
        valstr[len] = 0;
        WSContentSend_P(PSTR("%s"), valstr);
        TS_FLOAT *fpd = 0;
        uint16_t alend;
        uint16_t ipos;
        lp = get_array_by_name(cp + 5, &fpd, &alend, &ipos);
        SCRIPT_SKIP_SPACES
        if (*lp != ')') {
          // limit array lenght
          TS_FLOAT val;
          lp = GetNumericArgument(lp, OPER_EQU, &val, 0);
          if (val > alend) {
            val = alend;
          }
          alend = val;
        }

        if (ipos >= alend) ipos = 0;
        if (fpd) {
          for (uint32_t cnt = 0; cnt < alend; cnt++) {
            dtostrfd(fpd[ipos], 3, valstr);
            ipos++;
            if (ipos >= alend) {
              ipos = 0;
            }
            if (cnt == 0) {
              WSContentSend_P(PSTR("%s"), valstr);
            } else {
              WSContentSend_P(PSTR(",%s"), valstr);
            }
          }
        }
        lp++;
        WSContentSend_P(PSTR("%s"), lp);
        return lp;
      }

      cp = strstr_P(lin, PSTR("=#"));
      if (cp) {
        // insert from subroutine
        char valstr[128];
        uint16_t len = (uint32_t)cp - (uint32_t)lin;
        strncpy(valstr, lin, len);
        valstr[len] = 0;
        WSContentSend_P(PSTR("%s"), valstr);
        scripter_sub(cp , 0);
        cp = strchr(cp, ')');
        if (cp) {
          WSContentSend_P(PSTR("%s"), cp + 1);
        }
        return lp;
      }

      if (!strncmp(lin, "gc(", 3)) {
        // get google table
        lp = lin + 3;
        SCRIPT_SKIP_SPACES

        const char *func;
        char options[312];
        uint8_t nanum = MAX_GARRAY;
        uint8_t y2f = 0;
        uint8_t tonly = 0;
        char type = *lp;
        if (type != 'e') {
          gs_ctype = type;
        }
        lp++;
        if (!(google_libs & GLIBS_MAIN)) {
          google_libs |= GLIBS_MAIN;
          WSContentSend_P(SCRIPT_MSG_GTABLE);
        }

        gc_str = GC_type(gs_ctype);

        switch (gs_ctype) {
          case 'g':
            if (!(google_libs & GLIBS_GAUGE)) {
              google_libs |= GLIBS_GAUGE;
              WSContentSend_P(SCRIPT_MSG_GAUGE);
            }
            break;
          case 't':
            if (!(google_libs & GLIBS_TABLE)) {
              google_libs |= GLIBS_TABLE;
              WSContentSend_P(SCRIPT_MSG_TABLE);
            }
            break;
          case 'T':
            if (!(google_libs & GLIBS_TIMELINE)) {
              google_libs |= GLIBS_TIMELINE;
              WSContentSend_P(SCRIPT_MSG_TIMELINE);
            }
            break;
        }
        if (type == 'e') {
          WSContentSend_P(SCRIPT_MSG_GTABLEbx, gc_str, chartindex);
          chartindex++;
          return lp1;
        }

        char stacked[6];
        strcpy_P(stacked,"false");
        if (gs_ctype == 'l' && *lp == 'f') {
          lp++;
          func = PSTR(",curveType:'function'");
        } else {
          func = "";
        }
        if (gs_ctype == 'c' && *lp == 's') {
          lp++;
          strcpy_P(stacked,"true");
        }
        if (*lp == '2') {
          lp++;
          nanum = 2;
          y2f = 1;
        }
        if (*lp == 't') {
          lp++;
          tonly = 1;
        }
        SCRIPT_SKIP_SPACES

        //Serial.printf("type %d\n",ctype);
        TS_FLOAT max_entries = 0;

        struct T_INDEX ind;
        uint8_t vtype;
        char *slp = lp;
        lp = isvar(lp, &vtype, &ind, &max_entries, 0, 0);
        if (vtype != VAR_NV) {
          if ((vtype & STYPE) == 0) {
            // numeric result
            if (!ind.bits.constant && glob_script_mem.type[ind.index].bits.is_filter) {
              // is 1. array
              lp = slp;
              max_entries = 0;
            }
          }
        }
        SCRIPT_SKIP_SPACES

        TS_FLOAT *arrays[MAX_GARRAY];
        uint8_t anum = 0;
        uint16_t entries = 0;
        uint16_t ipos = 0;
        lp = gc_get_arrays(lp, &arrays[0], &anum, &entries, &ipos);

        if (anum > nanum) {
          return lp1;
          //goto nextwebline;
        }

        // override array size
        if (max_entries > 0) {
          if (max_entries > entries) {
            max_entries = entries;
          }
          entries = max_entries;
        }

        // we know how many arrays and the number of entries
        //Serial.printf("arrays %d\n",anum);
        //Serial.printf("entries %d\n",entries);
        if (gs_ctype == 'T') {
          if (anum && !(entries & 1)) {
            WSContentSend_P(SCRIPT_MSG_GTABLEa);
            char label[SCRIPT_MAXSSIZE];
            lp = GetStringArgument(lp, OPER_EQU, label, 0);
            SCRIPT_SKIP_SPACES
            char lab2[SCRIPT_MAXSSIZE];
            lab2[0] = 0;
            if (*lp != ')') {
              lp = GetStringArgument(lp, OPER_EQU, lab2, 0);
              WSContentSend_P(SCRIPT_MSG_GTABLEe);
            } else {
              WSContentSend_P(SCRIPT_MSG_GTABLEd);
            }

            for (uint32_t ind = 0; ind < anum; ind++) {
              char lbl[16];
              TS_FLOAT *fp = arrays[ind];
              GetTextIndexed(lbl, sizeof(lbl), ind, label);
              char lbl2[16];
              if (lab2[0]) {
                GetTextIndexed(lbl2, sizeof(lbl2), ind, lab2);
              }
              uint32_t ventries = 0;
              for (uint32_t cnt = 0; cnt < entries; cnt += 2) {
                if (fp[cnt]!=0 && fp[cnt+1]!=0) {
                  ventries+=2;
                } else {
                  break;
                }
              }

              for (uint32_t cnt = 0; cnt < ventries; cnt += 2) {
                WSContentSend_P("['%s',",lbl);
                if (lab2[0]) {
                  WSContentSend_P("'%s',",lbl2);
                }
                uint32_t time = fp[cnt];
                WSContentSend_P(SCRIPT_MSG_GOPT5, time / 60, time % 60);
                WSContentSend_P(",");
                time = fp[cnt + 1];
                WSContentSend_P(SCRIPT_MSG_GOPT5, time / 60, time % 60);
                  WSContentSend_P("]");
                  if (cnt < ventries - 2) { WSContentSend_P(","); }
              }
              if (ind < anum - 1) {
                if (ventries) {
                WSContentSend_P(",");
              }
            }
          }
          snprintf_P(options,sizeof(options), SCRIPT_MSG_GOPT4);
        }
        if (tonly) {
          WSContentSend_P("]);");
          return lp1;
          //goto nextwebline;
        }
      } else {

        // we need to fetch the labels now
        WSContentSend_P(SCRIPT_MSG_GTABLEa);
        lp = gc_send_labels(lp, anum);

        // now we have to export the values
        // fetch label part only once in combo string
        char label[SCRIPT_MAXSSIZE];
        lp = GetStringArgument(lp, OPER_EQU, label, 0);
        SCRIPT_SKIP_SPACES

        uint8_t asflg = 1;
        if (label[0] == '&') {
          strcpy(label, &label[1]);
          asflg = 0;
        }

        int16_t divflg = 1;
        int16_t todflg = -1;
        uint8_t hmflg = 0;
        if (!strncmp(label, "cnt", 3)) {
          char *cp = &label[3];
          if (*cp == 'h') {
            hmflg = 1;
            cp++;
          }
          // cnth2016/240

          //todflg=atoi(&label[3]);
          todflg = strtol(cp, &cp, 10);
          if (!hmflg) {
            if (todflg >= entries) todflg = entries - 1;
            if (todflg < 0) todflg = 0;
          }
          if (*cp == '/') {
            cp++;
            divflg = strtol(cp, &cp, 10);
          }
        } else {
          char *lp = label;
          if (!strncmp(label, "wdh:", 4)) {
            // one week hours
            todflg = -2;
            lp += 4;
          }
          uint16 segments = 1;
          for (uint32_t cnt = 0; cnt < strlen(lp); cnt++) {
            if (lp[cnt] == '|') {
              segments++;
            }
          }
          divflg = entries / segments;
          if (!divflg) divflg = 1;
        }
        uint32_t aind = ipos;
        if (aind >= entries) aind = entries - 1;
        for (uint32_t cnt = 0; cnt < entries; cnt++) {
          WSContentSend_P("['");
          char lbl[16];
          if (todflg >= 0) {
            uint16_t mins = (TS_FLOAT)(todflg % divflg) * (TS_FLOAT)((TS_FLOAT)60 / (TS_FLOAT)divflg);
            if (hmflg) {
              sprintf(lbl, "%d:%02d", todflg / divflg, mins);
            } else {
              sprintf(lbl, "%d", todflg / divflg);
            }
            todflg++;
            if (hmflg == 0) {
              if (todflg >= entries) {
                todflg = 0;
              }
            } else {
              if ((todflg / divflg) >= 24) {
                todflg = 0;
              }
            }
          } else {
            if (todflg == -1) {
              GetTextIndexed(lbl, sizeof(lbl), aind / divflg, label);
            } else {
              // day,hours,mins
              GetTextIndexed(lbl, sizeof(lbl), aind / divflg, label + 4);
              sprintf(lbl, "%s-%02d", lbl, aind % divflg);
            }
          }
          WSContentSend_P(lbl);
          WSContentSend_P("',");
          for (uint32_t ind = 0; ind < anum; ind++) {
            char acbuff[32];
            TS_FLOAT *fp = arrays[ind];
            TS_FLOAT fval;
            if (asflg) {
              fval = fp[aind];
            } else {
              fval = fp[cnt];
            }
            f2char(fval, glob_script_mem.script_dprec, glob_script_mem.script_lzero, acbuff, '.');
            WSContentSend_P("%s", acbuff);
            if (ind < anum - 1) { WSContentSend_P(","); }
          }
          WSContentSend_P("]");
          if (cnt < entries - 1) { WSContentSend_P(","); }
          aind++;
          if (aind >= entries) {
            aind = 0;
          }
        }
        // table complete
        if (tonly) {
          WSContentSend_P("]);");
          return lp1;
          //goto nextwebline;
        }
        // get header
        char header[SCRIPT_MAXSSIZE];
        lp = GetStringArgument(lp, OPER_EQU, header, 0);
        SCRIPT_SKIP_SPACES

        switch (gs_ctype) {
          case 't':
            snprintf_P(options, sizeof(options), SCRIPT_MSG_GOPT2);
            break;
          default:
            snprintf_P(options, sizeof(options), SCRIPT_MSG_GOPT1, header, stacked);
            break;
        }
        // check for 2 axis option
        if (y2f) {
          // 2 y axes variant
          SCRIPT_SKIP_SPACES
          TS_FLOAT max1;
          lp = GetNumericArgument(lp, OPER_EQU, &max1, 0);
          SCRIPT_SKIP_SPACES
          TS_FLOAT max2;
          lp = GetNumericArgument(lp, OPER_EQU, &max2, 0);
          SCRIPT_SKIP_SPACES
          char maxstr1[16];
          dtostrfd(max1, 3, maxstr1);
          char maxstr2[16];
          dtostrfd(max2, 3, maxstr2);
          //snprintf_P(options, sizeof(options), SCRIPT_MSG_GOPT3, header, (uint32_t)max1, (uint32_t)max2, func);
          snprintf_P(options, sizeof(options), SCRIPT_MSG_GOPT3, header, stacked, maxstr1, maxstr2, func);
        } else {
          SCRIPT_SKIP_SPACES
          if (gs_ctype != 'g') {
            if (*lp != ')') {
              TS_FLOAT max1;
              lp = GetNumericArgument(lp, OPER_EQU, &max1, 0);
              SCRIPT_SKIP_SPACES
              TS_FLOAT max2;
              lp = GetNumericArgument(lp, OPER_EQU, &max2, 0);
              SCRIPT_SKIP_SPACES
              char maxstr1[16];
              dtostrfd(max1, 3, maxstr1);
              char maxstr2[16];
              dtostrfd(max2, 3, maxstr2);
              //nprintf_P(options, sizeof(options), SCRIPT_MSG_GOPT6, header, (uint32_t)max1, (uint32_t)max2, func);
              snprintf_P(options, sizeof(options), SCRIPT_MSG_GOPT6, header, stacked, maxstr1, maxstr2, func);
            }
          }
        }

        if (gs_ctype == 'g') {
          TS_FLOAT yellowFrom;
          lp = GetNumericArgument(lp, OPER_EQU, &yellowFrom, 0);
          SCRIPT_SKIP_SPACES
          TS_FLOAT redFrom;
          lp = GetNumericArgument(lp, OPER_EQU, &redFrom, 0);
          SCRIPT_SKIP_SPACES
          TS_FLOAT maxValue;
          lp = GetNumericArgument(lp, OPER_EQU, &maxValue, 0);
          SCRIPT_SKIP_SPACES
          TS_FLOAT redTo = maxValue;
          TS_FLOAT yellowTo = redFrom;
          snprintf_P(options, sizeof(options), SCRIPT_MSG_GAUGEOPT, (uint32_t)maxValue, (uint32_t)redFrom, (uint32_t)redTo,
            (uint32_t)yellowFrom, (uint32_t)yellowTo);
          }
        }
        WSContentSend_P(SCRIPT_MSG_GTABLEb, options);
        WSContentSend_P(SCRIPT_MSG_GTABLEbx, gc_str, chartindex);
        chartindex++;
      } else {
        WSContentSend_P(PSTR("%s"), lin);
      }
#else

      if (mc != 'z') {
        if (!(specopt & WSO_FORCEMAIN)) {
          lin++;
        }
      }
      WSContentSend_P(PSTR("%s"), lin);
    } else {
          //  WSContentSend_P(PSTR("%s"),lin);
#endif //USE_GOOGLE_CHARTS
    }
  }
  return lp1;
}

#endif //USE_SCRIPT_WEB_DISPLAY


#if defined(USE_SENDMAIL) || defined(USE_ESP32MAIL)

void script_send_email_body(void(*func)(char *)) {
uint8_t msect = Run_Scripter1(">m", -2, 0);
  if (msect == 99) {
    char tmp[256];
    char *lp = glob_script_mem.section_ptr + 2;
    while (lp) {
      while (*lp == SCRIPT_EOL) {
       lp++;
      }
      if (!*lp || *lp == '#' || *lp == '>') {
          break;
      }
      if (*lp!=';') {
        // send this line to smtp
        Replace_Cmd_Vars(lp, 1, tmp, sizeof(tmp));
        //client->println(tmp);
        func(tmp);
      }
      if (*lp == SCRIPT_EOL) {
        lp++;
      } else {
        lp = strchr(lp, SCRIPT_EOL);
        if (!lp) break;
        lp++;
      }
    }
  } else {
    //client->println("*");
    func((char*)"*");
  }
}
#endif //USE_SENDMAIL

#ifdef USE_SCRIPT_JSON_EXPORT
void ScriptJsonAppend(void) {
  uint8_t web_script = Run_Scripter1(">J", -2, 0);
  if (web_script==99) {
    char tmp[256];
    char *lp = glob_script_mem.section_ptr + 2;
    while (lp) {
      while (*lp == SCRIPT_EOL) {
       lp++;
      }
      if (!*lp || *lp=='#' || *lp=='>') {
          break;
      }
      if (*lp!=';') {
        // send this line to mqtt
        if (!strncmp(lp, "%=#", 3)) {
          // subroutine
          lp = scripter_sub(lp + 1, 0);
        } else {
          Replace_Cmd_Vars(lp, 1, tmp, sizeof(tmp));
          ResponseAppend_P(PSTR("%s"), tmp);
        }
      }
      if (*lp==SCRIPT_EOL) {
        lp++;
      } else {
        lp = strchr(lp, SCRIPT_EOL);
        if (!lp) break;
        lp++;
      }
    }
  }
}
#endif //USE_SCRIPT_JSON_EXPORT

// returns section as string
String ScriptLoadSection(const char *sect) {
  uint16_t slen = strlen(sect);
  if (Run_Scripter1(sect, -slen, 0) == 99) {
    char *spo = glob_script_mem.section_ptr + slen;
    char *sp = spo;
    while (*sp) {
      if (*sp == '#') {
        *sp = 0;
        String str = spo;
        *sp = '#';
        return str;
      }
      sp++;
    }
  }
  return "";
}


bool RulesProcessEvent(const char *json_event) {
  if (bitRead(Settings->rule_enabled, 0)) {
    //Run_Scripter(">E", 2, json_event);
    if (glob_script_mem.event_script) Run_Scripter(glob_script_mem.event_script, 0, json_event);
  }
  return true;
}

#ifdef ESP32
#ifdef ESP32_FAST_MUX

#define MUX_SIZE 128
struct FAST_PIN_MUX {
  volatile uint8_t scan_cnt;
  uint8_t scan_buff[MUX_SIZE];
  uint8_t scan_buff_size;
  uint8_t time;
  uint32_t low_pins;
  uint32_t high_pins;
  hw_timer_t * scan_timer = NULL;
  portMUX_TYPE scan_timerMux = portMUX_INITIALIZER_UNLOCKED;
} fast_pin_mux;


void IRAM_ATTR fast_mux_irq() {
  portENTER_CRITICAL_ISR(&fast_pin_mux.scan_timerMux);
  // this could be optimized for multiple pins
  if (fast_pin_mux.scan_buff_size) {
    while (fast_pin_mux.scan_cnt < fast_pin_mux.scan_buff_size) {
      uint8_t iob = fast_pin_mux.scan_buff[fast_pin_mux.scan_cnt];
      if (iob & 0x20) {
        if (iob & 1) {
          GPIO.out_w1tc = fast_pin_mux.low_pins;
        }
        if (iob & 2) {
          GPIO.out_w1ts = fast_pin_mux.high_pins;
        }
        if (iob & 4) {
          // modify mux timer
          uint8_t fac = (iob & 0x1f) >> 2;
          timerAlarmWrite(fast_pin_mux.scan_timer, fast_pin_mux.time * fac, true);
        }
      } else {
        uint8_t mode = (iob >> 6) & 1;
        digitalWrite(iob & 0x1f, mode);
      }
      fast_pin_mux.scan_cnt++;
      if (iob & 0x80) {
        break;
      }
    }
    if (fast_pin_mux.scan_cnt >= fast_pin_mux.scan_buff_size) {
      fast_pin_mux.scan_cnt = 0;
    }
  }
  portEXIT_CRITICAL_ISR(&fast_pin_mux.scan_timerMux);
}

/* uint8_t pin nr, 0x40 = value, 0x80 = next
*/

int32_t fast_mux(uint32_t flag, uint32_t time, TS_FLOAT *buf, uint32_t len) {
int32_t retval;
  if (!flag) {
    if (len > MUX_SIZE) {
      len = MUX_SIZE;
    }
    fast_pin_mux.high_pins = 0;
    fast_pin_mux.low_pins = 0;
    for (uint32_t cnt = 0; cnt < len; cnt++) {
      uint8_t iob = *buf++;
      fast_pin_mux.scan_buff[cnt] = iob;
      uint8_t pin = iob & 0x1f;
      pinMode(pin, OUTPUT);
      if (iob & 0x40) {
        digitalWrite(pin, 1);
        fast_pin_mux.high_pins |= (1 << pin);
      } else {
        digitalWrite(pin, 0);
        fast_pin_mux.low_pins |= (1 << pin);
      }
    }
    fast_pin_mux.scan_buff_size = 0;

    if (fast_pin_mux.scan_timer) {
      timerStop(fast_pin_mux.scan_timer);
      timerDetachInterrupt(fast_pin_mux.scan_timer);
      timerEnd(fast_pin_mux.scan_timer);
    }
    fast_pin_mux.scan_timer = timerBegin(3, 1000, true);
    if (!fast_pin_mux.scan_timer) {
      return -1;
    }
    fast_pin_mux.time = time;
    timerAttachInterrupt(fast_pin_mux.scan_timer, &fast_mux_irq, true);
    timerSetAutoReload(fast_pin_mux.scan_timer, true);
    timerAlarmWrite(fast_pin_mux.scan_timer, fast_pin_mux.time, true);
    timerAlarmEnable(fast_pin_mux.scan_timer);
    timerStart(fast_pin_mux.scan_timer);
  } else if (flag == 1) {
    if (fast_pin_mux.scan_timer) {
      timerStop(fast_pin_mux.scan_timer);
      timerDetachInterrupt(fast_pin_mux.scan_timer);
      timerEnd(fast_pin_mux.scan_timer);
      fast_pin_mux.scan_timer = 0;
    }
  } else if (flag == 2) {
    portENTER_CRITICAL(&fast_pin_mux.scan_timerMux);
    for (uint32_t cnt = 0; cnt < len; cnt++) {
      fast_pin_mux.scan_buff[cnt] = *buf++;
    }
    fast_pin_mux.scan_buff_size = len;
    portEXIT_CRITICAL(&fast_pin_mux.scan_timerMux);
  } else {
    portENTER_CRITICAL(&fast_pin_mux.scan_timerMux);
    retval = fast_pin_mux.scan_cnt;
    portEXIT_CRITICAL(&fast_pin_mux.scan_timerMux);
    return retval;
  }
  return 0;
}
#endif // ESP32_FAST_MUX
#endif // ESP32

#ifdef ESP32
#ifdef USE_SCRIPT_TASK



struct ESP32_Task {
  uint16_t task_timer;
  TaskHandle_t task_t;
  char *tstart;
} esp32_tasks[2];


void script_task1(void *arg) {
  //uint32_t lastms=millis();
  //uint32_t time;
  while (1) {
    //time=millis()-lastms;
    //lastms=millis();
    //time=esp32_tasks[0].task_timer-time;
    //if (time<esp32_tasks[1].task_timer) {delay(time); }
    //if (time<=esp32_tasks[0].task_timer) {vTaskDelay( pdMS_TO_TICKS( time ) ); }
    if (bitRead(Settings->rule_enabled, 0)) {
      if (esp32_tasks[0].tstart) Run_Scripter1(esp32_tasks[0].tstart, 0, 0);
    }
    if (esp32_tasks[0].task_timer) {
      delay(esp32_tasks[0].task_timer);
    } else {
      esp32_tasks[0].task_t = 0;
      vTaskDelete( NULL );
    }
  }
}

void script_task2(void *arg) {
  while (1) {
    if (bitRead(Settings->rule_enabled, 0)) {
      if (esp32_tasks[1].tstart) Run_Scripter1(esp32_tasks[1].tstart, 0, 0);
    }
    if (esp32_tasks[1].task_timer) {
      delay(esp32_tasks[1].task_timer);
    } else {
      esp32_tasks[1].task_t = 0;
      vTaskDelete( NULL );
    }
  }
}
uint32_t scripter_create_task(uint32_t num, uint32_t time, uint32_t core, int32_t prio,  int32_t stack) {
  //return 0;
  BaseType_t res = 0;
  if (core > 1) { core = 1; }
  if (num < 1) { num = 1; }
  if (num > 2) { num = 2; }
  num--;
  if (esp32_tasks[num].task_t) {
    vTaskDelete(esp32_tasks[num].task_t);
    esp32_tasks[num].task_t = 0;
  }
  if (prio >= 0) {
    char *sp = 0;
    esp32_tasks[num].task_timer = time;
    if (!num) {
      if (Run_Scripter1(">t1", -3, 0) == 99) {
        sp = glob_script_mem.section_ptr + 2;
        res = xTaskCreatePinnedToCore(script_task1, "T1", stack, NULL, prio, &esp32_tasks[num].task_t, core);
      }
    } else {
      if (Run_Scripter1(">t2", -3, 0) == 99) {
        sp = glob_script_mem.section_ptr + 2;
        res = xTaskCreatePinnedToCore(script_task2, "T2", stack, NULL, prio, &esp32_tasks[num].task_t, core);
      }
    }
    esp32_tasks[num].tstart = sp;
  }
  return res;
}
#endif // USE_SCRIPT_TASK
#endif // ESP32


#ifdef USE_UFILESYS
// read http content to file
int32_t url2file(uint8_t fref, char *url) {
  WiFiClient http_client;
  HTTPClient http;
  int32_t httpCode = 0;
  String weburl = "http://"+UrlEncode(url);
  for (uint32_t retry = 0; retry < 3; retry++) {
    http.begin(http_client, weburl);
    delay(100);
    httpCode = http.GET();
    if (httpCode >= 0) {
      break;
    }
  }
  if (httpCode < 0) {
    AddLog(LOG_LEVEL_INFO,PSTR("SCR: HTTP error %d = %s"), httpCode, http.errorToString(httpCode).c_str());
  }
  if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
    WiFiClient *stream = http.getStreamPtr();
    int32_t len = http.getSize();
    if (len < 0) len = 99999999;
    uint8_t buff[512];
    while (http.connected() && (len > 0)) {
      size_t size = stream->available();
      if (size) {
        if (size > sizeof(buff)) {
          size = sizeof(buff);
        }
        uint32_t read = stream->readBytes(buff, size);
        glob_script_mem.files[fref].write(buff, read);
        len -= read;
        AddLog(LOG_LEVEL_DEBUG,PSTR("SCR: HTTP read %d"), len);
      }
      delayMicroseconds(1);
    }
  }
  http.end();
  http_client.stop();
  return httpCode;
}
#endif

int32_t http_req(char *host, char *request) {
  WiFiClient http_client;
  HTTPClient http;
  int32_t httpCode = 0;
  uint8_t mode = 0;
  char hbuff[128];
  strcpy(hbuff, "http://");
  strcat(hbuff, host);

  if (*request == '_') {
    mode = 1;
    request++;
  }

#ifdef HTTP_DEBUG
  AddLog(LOG_LEVEL_INFO, PSTR("SCR: HTTP heap %d"), ESP_getFreeHeap());
#endif

  if (!mode) {
    // GET
    strcat(hbuff, request);
    //AddLog(LOG_LEVEL_INFO, PSTR("HTTP GET %s"),hbuff);
    http.begin(http_client, hbuff);
    httpCode = http.GET();
  } else {
    // POST
    //AddLog(LOG_LEVEL_INFO, PSTR("HTTP POST %s - %s"),hbuff, request);
    http.begin(http_client, hbuff);
    http.addHeader("Content-Type", "text/plain");
    httpCode = http.POST(request);
  }

#ifdef HTTP_DEBUG
  AddLog(LOG_LEVEL_INFO, PSTR("SCR: HTTP RESULT %s"), http.getString().c_str());
#endif

#ifdef USE_WEBSEND_RESPONSE
#ifdef MQTT_DATA_STRING
  TasmotaGlobal.mqtt_data = http.getString();
#else
  strlcpy(TasmotaGlobal.mqtt_data, http.getString().c_str(), ResponseSize());
#endif

#ifdef HTTP_DEBUG
  AddLog(LOG_LEVEL_INFO, PSTR("SCR: HTTP MQTT BUFFER %s"), ResponseData());
#endif

//  AddLog(LOG_LEVEL_INFO, PSTR("JSON %s"), wd_jstr);
//  TasmotaGlobal.mqtt_data = wd_jstr;
  //Run_Scripter(">E", 2, ResponseData());
  if (glob_script_mem.event_script) Run_Scripter(glob_script_mem.event_script, 0, ResponseData());


  glob_script_mem.glob_error = 0;
#endif

  http.end();
  http_client.stop();

  return httpCode;
}


#ifdef SCRIPT_GET_HTTPS_JP

#ifdef TESLA_POWERWALL
Powerwall powerwall = Powerwall();

int32_t call2pwl(const char *url) {
  uint8_t debug = 0;
  if (*url == 'D') {
    url++;
    debug = 1;
  }
  String cookie = powerwall.AuthCookie();
  if (*url == 'N') {
    url++;
    cookie = "";
  }
  String result = powerwall.GetRequest(String(url), cookie);
  //AddLog(LOG_LEVEL_INFO, PSTR("PWL: result: %s"), result.c_str());

  // shrink data size because it exceeds json parser maxsize
  result.replace("communication_time", "ct");
  result.replace("instant", "i");
  result.replace("apparent", "a");
  result.replace("reactive", "r");

// custom replace
#ifdef TESLA_POWERWALL_CTS1
  result.replace(TESLA_POWERWALL_CTS1, "PW_CTS1");
#endif

#ifdef TESLA_POWERWALL_CTS2
  result.replace(TESLA_POWERWALL_CTS2, "PW_CTS2");
#endif

  if (result.length()>4095) {
    AddLog(LOG_LEVEL_INFO, PSTR("PWL: result overflow: %d"), result.length());
  }


#ifdef MQTT_DATA_STRING
  TasmotaGlobal.mqtt_data = result;
#else
  strncpy(TasmotaGlobal.mqtt_data, result.c_str(), MESSZ);
#endif

  // meter aggregates has also too many tokens
  char *cp = (char*)result.c_str();
  if (!strncmp_P(cp, PSTR("{\"site\""), 7)) {
    // split into 2 sets
    char *sp = strstr_P(cp, PSTR(",\"load\":"));
    if (sp) {
      *sp = '}';
      *(sp + 1 ) = 0;
      if (debug) {
        AddLog(LOG_LEVEL_INFO, PSTR("PWL: result 1: %s"), cp);
      }
      Run_Scripter(">jp", 3, cp);
      *sp = '{';
      *(sp + 1 ) = '\"';
      if (debug) {
        AddLog(LOG_LEVEL_INFO, PSTR("PWL: result 2: %s"), sp);
      }
      Run_Scripter(">jp", 3, sp);
    }
  } else {
    if (debug) {
      AddLog(LOG_LEVEL_INFO, PSTR("PWL: result: %s"), result.c_str());
    }
    Run_Scripter(">jp", 3, result.c_str());
  }
  return 0;
}
#endif // TESLA_POWERWALL


#include "WiFiClientSecureLightBearSSL.h"

// get https info page json string
uint32_t call2https(const char *host, const char *path) {
  //if (TasmotaGlobal.global_state.wifi_down) return 1;
  uint32_t status = 0;

  BearSSL::WiFiClientSecure_light *httpsClient;
  httpsClient = new BearSSL::WiFiClientSecure_light(1024, 1024);

  httpsClient->setTimeout(2000);
  httpsClient->setInsecure();

 // AddLog(LOG_LEVEL_INFO,PSTR(">>> host %s"),host);

  uint32_t retry = 0;
  while ((!httpsClient->connect(host, 443)) && (retry < 10)) {
    delay(100);
    retry++;
  }
  if (retry == 10) {
    return 2;
  }
  AddLog(LOG_LEVEL_DEBUG,PSTR("SCR: connected"));

  String request;

  request = String("GET ") + path +
                    " HTTP/1.1\r\n" +
                    "Host: " + host +
                    "\r\n" + "Connection: close\r\n\r\n";
  httpsClient->print(request);
//  AddLog(LOG_LEVEL_INFO,PSTR(">>> get request %s"),(char*)request.c_str());

  while (httpsClient->connected()) {
    String line = httpsClient->readStringUntil('\n');
    if (line == "\r") {
      break;
    }
  }
  String result;
  while (httpsClient->available()) {
    String line = httpsClient->readStringUntil('\n');
    if (line!="") {
      result += line;
    }
  }
  httpsClient->stop();
  delete httpsClient;
//  AddLog(LOG_LEVEL_INFO,PSTR(">>> response 2 %s"),(char*)result.c_str());
  Run_Scripter(">jp", 3, (char*)result.c_str());
  return 0;
}
#endif // SCRIPT_GET_HTTPS_JP


void cpy2lf(char *dst, uint32_t dstlen, char *src) {
  for (uint32_t cnt=0; cnt<dstlen; cnt++) {
    if (src[cnt]==0 || src[cnt]=='\n') {
      dst[cnt] = 0;
      break;
    }
    dst[cnt] = src[cnt];
  }
}

#ifdef USE_SCRIPT_I2C
uint32_t script_i2c(uint8_t sel, uint16_t val, uint32_t val1) {
  uint32_t rval = 0;
  uint8_t bytes = 1;

  if (sel > 0) {
    if (!glob_script_mem.script_i2c_wire) return 0;
  }

  switch (sel) {
    case 0:
      glob_script_mem.script_i2c_addr = val;
#ifdef ESP32
      if (val1 == 0) glob_script_mem.script_i2c_wire = &Wire;
      else glob_script_mem.script_i2c_wire = &Wire1;
#else
      glob_script_mem.script_i2c_wire = &Wire;
#endif
      glob_script_mem.script_i2c_wire->beginTransmission(glob_script_mem.script_i2c_addr);
      return (0 == glob_script_mem.script_i2c_wire->endTransmission());
      break;
    case 2:
      // read 1..4 bytes
      if ((val & 0x8000) == 0) {
        glob_script_mem.script_i2c_wire->beginTransmission(glob_script_mem.script_i2c_addr);
        glob_script_mem.script_i2c_wire->write(val);
        glob_script_mem.script_i2c_wire->endTransmission();
      }
      glob_script_mem.script_i2c_wire->requestFrom((int)glob_script_mem.script_i2c_addr, (int)val1);

      for (uint8_t cnt = 0; cnt < val1; cnt++) {
        rval <<= 8;
        rval |= (uint8_t)glob_script_mem.script_i2c_wire->read();
      }
      break;

    case 10:
    case 11:
    case 12:
    case 13:
      // write 1 .. 4 bytes
      bytes = sel - 9;
      glob_script_mem.script_i2c_wire->beginTransmission(glob_script_mem.script_i2c_addr);
      if ((val & 0x8000) == 0) {
        glob_script_mem.script_i2c_wire->write(val);
      }
      if ((val & 0x4000) == 0) {
        for (uint8_t cnt = 0; cnt < bytes; cnt++) {
          glob_script_mem.script_i2c_wire->write(val1);
          val1 >>= 8;
        }
      } else {
        uint32_t wval = 0;
        for (uint8_t cnt = 0; cnt < bytes; cnt++) {
          wval = val1 >> ((bytes - 1 - cnt) * 8);
          glob_script_mem.script_i2c_wire->write(wval);
        }
      }
      glob_script_mem.script_i2c_wire->endTransmission();
      break;

  }
  return rval;
}
#endif // USE_SCRIPT_I2C


#ifdef USE_LVGL
#include <renderer.h>
#include "lvgl.h"

#define MAX_LVGL_OBJS 8
uint8_t lvgl_numobjs;
lv_obj_t *lvgl_buttons[MAX_LVGL_OBJS];

void start_lvgl(const char * uconfig);
lv_event_code_t lvgl_last_event;
uint8_t lvgl_last_object;
uint8_t lvgl_last_slider;
static lv_obj_t * kb;
static lv_obj_t * ta;

void lvgl_set_last(lv_obj_t * obj, lv_event_code_t event);
void lvgl_set_last(lv_obj_t * obj, lv_event_code_t event) {
  lvgl_last_event = event;
  lvgl_last_object = 0;
  for (uint8_t cnt = 0; cnt < MAX_LVGL_OBJS; cnt++) {
    if (lvgl_buttons[cnt] == obj) {
      lvgl_last_object = cnt + 1;
      return;
    }
  }
}


void btn_event_cb(lv_event_t * e);
void btn_event_cb(lv_event_t * e) {
  lvgl_set_last(e->target, e->code);
  if (e->code == LV_EVENT_CLICKED) {
    Run_Scripter1(">lvb", 4, 0);
  }
}

void slider_event_cb(lv_event_t * e);
void slider_event_cb(lv_event_t * e) {
  lvgl_set_last(e->target, e->code);
  lvgl_last_slider = lv_slider_get_value(e->target);
  if (e->code == LV_EVENT_VALUE_CHANGED) {
    Run_Scripter1(">lvs", 4, 0);
  }
}

static void kb_create(void);
static void ta_event_cb(lv_event_t * e);
static void kb_event_cb(lv_event_t * e);

static void kb_event_cb(lv_event_t * e) {
    lv_keyboard_def_event_cb(e);
    if(e->code == LV_EVENT_CANCEL) {
        lv_keyboard_set_textarea(kb, NULL);
        lv_obj_del(kb);
        kb = NULL;
    }
}

static void kb_create(void) {
    kb = lv_keyboard_create(lv_scr_act());
    lv_obj_add_event_cb(kb, kb_event_cb, LV_EVENT_ALL, nullptr);
    lv_keyboard_set_textarea(kb, ta);
}

static void ta_event_cb(lv_event_t * e) {
    if(e->code == LV_EVENT_CLICKED && kb == NULL) {
      kb_create();
    }
}


void lvgl_StoreObj(lv_obj_t *obj);
void lvgl_StoreObj(lv_obj_t *obj) {
  if (lvgl_numobjs < MAX_LVGL_OBJS) {
    lvgl_buttons[lvgl_numobjs] = obj;
    lvgl_numobjs++;
  }
}

int32_t lvgl_test(char **lpp, int32_t p) {
  char *lp = *lpp;
  lv_obj_t *obj;
  lv_obj_t *label;
  TS_FLOAT xp, yp, xs, ys, min, max;
  lv_meter_scale_t * scale;
  lv_meter_indicator_t * indic;
  char str[SCRIPT_MAXSSIZE];
  int32_t res = 0;

  switch (p) {
    case 0:
      start_lvgl(0);
      lvgl_numobjs = 0;
      for (uint8_t cnt = 0; cnt < MAX_LVGL_OBJS; cnt++) {
        lvgl_buttons[cnt] = 0;
      }
      break;

    case 1:
      lv_obj_clean(lv_scr_act());
      break;

    case 2:
      // create button;
      lp = GetNumericArgument(lp, OPER_EQU, &xp, 0);
      SCRIPT_SKIP_SPACES
      lp = GetNumericArgument(lp, OPER_EQU, &yp, 0);
      SCRIPT_SKIP_SPACES
      lp = GetNumericArgument(lp, OPER_EQU, &xs, 0);
      SCRIPT_SKIP_SPACES
      lp = GetNumericArgument(lp, OPER_EQU, &ys, 0);
      SCRIPT_SKIP_SPACES
      lp = GetStringArgument(lp, OPER_EQU, str, 0);
      SCRIPT_SKIP_SPACES

      obj = lv_btn_create(lv_scr_act());
      lv_obj_set_pos(obj, xp, yp);
      lv_obj_set_size(obj, xs, ys);
      lv_obj_add_event_cb(obj, btn_event_cb, LV_EVENT_ALL, nullptr);
      label = lv_label_create(obj);
      lv_label_set_text(label, str);
      lvgl_StoreObj(obj);
      break;

    case 3:
      lp = GetNumericArgument(lp, OPER_EQU, &xp, 0);
      SCRIPT_SKIP_SPACES
      lp = GetNumericArgument(lp, OPER_EQU, &yp, 0);
      SCRIPT_SKIP_SPACES
      lp = GetNumericArgument(lp, OPER_EQU, &xs, 0);
      SCRIPT_SKIP_SPACES
      lp = GetNumericArgument(lp, OPER_EQU, &ys, 0);
      SCRIPT_SKIP_SPACES

      obj = lv_slider_create(lv_scr_act());
      lv_obj_set_pos(obj, xp, yp);
      lv_obj_set_size(obj, xs, ys);
      lv_obj_add_event_cb(obj, slider_event_cb, LV_EVENT_ALL, nullptr);
      lvgl_StoreObj(obj);
      break;

    case 4:
      lp = GetNumericArgument(lp, OPER_EQU, &xp, 0);
      SCRIPT_SKIP_SPACES
      lp = GetNumericArgument(lp, OPER_EQU, &yp, 0);
      SCRIPT_SKIP_SPACES
      lp = GetNumericArgument(lp, OPER_EQU, &xs, 0);
      SCRIPT_SKIP_SPACES
      lp = GetNumericArgument(lp, OPER_EQU, &ys, 0);
      SCRIPT_SKIP_SPACES
      lp = GetNumericArgument(lp, OPER_EQU, &min, 0);
      SCRIPT_SKIP_SPACES
      lp = GetNumericArgument(lp, OPER_EQU, &max, 0);
      SCRIPT_SKIP_SPACES

      obj = lv_meter_create(lv_scr_act());
      lv_obj_set_pos(obj, xp, yp);
      lv_obj_set_size(obj, xs, ys);
      scale = lv_meter_add_scale(obj);
      /*Add a needle line indicator*/
      indic = lv_meter_add_needle_line(obj, scale, 4, lv_palette_main(LV_PALETTE_GREY), -10);

      // lv_gauge_set_range(obj, min, max);   // TODO LVGL8
      lvgl_StoreObj(obj);
      break;

    case 5:
      lp = GetNumericArgument(lp, OPER_EQU, &min, 0);
      SCRIPT_SKIP_SPACES
      lp = GetNumericArgument(lp, OPER_EQU, &max, 0);
      SCRIPT_SKIP_SPACES
      if (lvgl_buttons[(uint8_t)min - 1]) {
        // lv_gauge_set_value(lvgl_buttons[(uint8_t)min - 1], 0, max);   // TODO LVGL8
      }
      break;

    case 6:
      // create label;
      lp = GetNumericArgument(lp, OPER_EQU, &xp, 0);
      SCRIPT_SKIP_SPACES
      lp = GetNumericArgument(lp, OPER_EQU, &yp, 0);
      SCRIPT_SKIP_SPACES
      lp = GetNumericArgument(lp, OPER_EQU, &xs, 0);
      SCRIPT_SKIP_SPACES
      lp = GetNumericArgument(lp, OPER_EQU, &ys, 0);
      SCRIPT_SKIP_SPACES
      lp = GetStringArgument(lp, OPER_EQU, str, 0);
      SCRIPT_SKIP_SPACES

      obj = lv_label_create(lv_scr_act());
      lv_obj_set_pos(obj, xp, yp);
      lv_obj_set_size(obj, xs, ys);
      lv_label_set_text(obj, str);
      lvgl_StoreObj(obj);
      break;

    case 7:
      lp = GetNumericArgument(lp, OPER_EQU, &min, 0);
      SCRIPT_SKIP_SPACES
      lp = GetStringArgument(lp, OPER_EQU, str, 0);
      SCRIPT_SKIP_SPACES
      if (lvgl_buttons[(uint8_t)min - 1]) {
        lv_label_set_text(lvgl_buttons[(uint8_t)min - 1], str);
      }
      break;

    case 8:
      {
      ta  = lv_textarea_create(lv_scr_act());
      lv_obj_align(ta, LV_ALIGN_TOP_MID, 0, LV_DPI_DEF / 16);
      lv_obj_add_event_cb(ta, ta_event_cb, LV_EVENT_ALL, nullptr);
      lv_textarea_set_text(ta, "");
      lv_coord_t max_h = LV_VER_RES / 2 - LV_DPI_DEF / 8;
      if (lv_obj_get_height(ta) > max_h) lv_obj_set_height(ta, max_h);
      kb_create();
      }
      break;

    case 50:
      res = lvgl_last_object;
      break;
    case 51:
      res = lvgl_last_event;
      break;
    case 52:
      res = lvgl_last_slider;
      break;


    default:
      start_lvgl(0);
      lvgl_setup();
      break;
  }

  *lpp = lp;
  return res;
}


lv_obj_t          *tabview,        // LittlevGL tabview object
                  *gauge,          // Gauge object (on first of three tabs)
                  *chart,          // Chart object (second tab)
                  *canvas;         // Canvas object (third tab)
uint8_t            active_tab = 0, // Index of currently-active tab (0-2)
                   prev_tab   = 0; // Index of previously-active tab
lv_chart_series_t *series;         // 'Series' data for the bar chart
lv_draw_line_dsc_t draw_dsc; // Drawing style (for canvas) is similarly global

#define CANVAS_WIDTH  200 // Dimensions in pixels
#define CANVAS_HEIGHT 150

void lvgl_setup(void) {
  // Create a tabview object, by default this covers the full display.
  tabview = lv_tabview_create(lv_disp_get_scr_act(NULL), LV_DIR_TOP, 50);
  // The CLUE display has a lot of pixels and can't refresh very fast.
  // To show off the tabview animation, let's slow it down to 1 second.
  // lv_tabview_set_anim_time(tabview, 1000);  // LVGL8 TODO

  // Because they're referenced any time an object is drawn, styles need
  // to be permanent in scope; either declared globally (outside all
  // functions), or static. The styles used on tabs are never modified after
  // they're used here, so let's use static on those...
  static lv_style_t tab_style, tab_background_style, indicator_style;

  // This is the background style "behind" the tabs. This is what shows
  // through for "off" (inactive) tabs -- a vertical green gradient,
  // minimal padding around edges (zero at bottom).
  lv_style_init(&tab_background_style);
  lv_style_set_bg_color(&tab_background_style, lv_color_hex(0x408040));
  lv_style_set_bg_grad_color(&tab_background_style, lv_color_hex(0x304030));
  lv_style_set_bg_grad_dir(&tab_background_style, LV_GRAD_DIR_VER);
  lv_style_set_pad_top(&tab_background_style, 2);
  lv_style_set_pad_left(&tab_background_style, 2);
  lv_style_set_pad_right(&tab_background_style, 2);
  lv_style_set_pad_bottom(&tab_background_style, 0);
  // lv_obj_add_style(tabview, LV_TABVIEW_PART_TAB_BG, &tab_background_style); // LVGL8 TODO
  //lv_tabview_add_tab(tabview, LV_TABVIEW_PART_TAB_BG, &tab_background_style); // LVGL8 TODO

  // Style for tabs. Active tab is white with opaque background, inactive
  // tabs are transparent so the background shows through (only the white
  // text is seen). A little top & bottom padding reduces scrunchyness.
  lv_style_init(&tab_style);
  lv_style_set_pad_top(&tab_style, 3);
  lv_style_set_pad_bottom(&tab_style, 10);
  lv_style_set_bg_color(&tab_style, lv_color_white());
  lv_style_set_bg_opa(&tab_style, LV_OPA_100);
  lv_style_set_text_color(&tab_style, lv_color_make(0xff, 0xff, 0xff));
  lv_style_set_bg_opa(&tab_style, LV_OPA_TRANSP);
  lv_style_set_text_color(&tab_style, lv_color_white());
  // lv_obj_add_style(tabview, LV_TABVIEW_PART_TAB_BTN, &tab_style);  // LVGL8 TODO

  // Style for the small indicator bar that appears below the active tab.
  lv_style_init(&indicator_style);
  lv_style_set_bg_color(&indicator_style, lv_color_make(0xff, 0x00, 0x00));
  lv_style_set_size(&indicator_style, 5);
  // lv_obj_add_style(tabview, LV_TABVIEW_PART_INDIC, &indicator_style);  // LVGL8 TODO

  // Back to creating widgets...

  // Add three tabs to the tabview
  lv_obj_t *tab1 = lv_tabview_add_tab(tabview, "Gauge");
  lv_obj_t *tab2 = lv_tabview_add_tab(tabview, "Chart");
  lv_obj_t *tab3 = lv_tabview_add_tab(tabview, "Canvas");

  // And then add stuff in each tab...

  // The first tab holds a gauge. To keep the demo simple, let's just use
  // the default style and range (0-100). See LittlevGL docs for options.
  gauge = lv_meter_create(tab1);
  lv_obj_set_size(gauge, 186, 186);
  lv_obj_align(gauge,LV_ALIGN_CENTER, 0, 0);

  // Second tab, make a chart...
  chart = lv_chart_create(tab2);
  lv_obj_set_size(chart, 200, 180);
  lv_obj_align(chart, LV_ALIGN_CENTER, 0, 0);
  lv_chart_set_type(chart, LV_CHART_TYPE_BAR);
  // For simplicity, we'll stick with the chart's default 10 data points:
  // series = lv_chart_add_series(chart, lv_color_make(0xff, 0x00, 0x00));  // LVGL8 TODO
  // lv_chart_init_points(chart, series, 0);  // LVGL8 TODO
  // Make each column shift left as new values enter on right:
  lv_chart_set_update_mode(chart, LV_CHART_UPDATE_MODE_SHIFT);

  // Third tab is a canvas, which we'll fill with random colored lines.
  // LittlevGL draw functions only work on TRUE_COLOR canvas.
/*  canvas = lv_canvas_create(tab3);
  lv_canvas_set_buffer(canvas, canvas_buffer,
    CANVAS_WIDTH, CANVAS_HEIGHT, LV_IMG_CF_TRUE_COLOR);
  lv_obj_align(canvas, LV_ALIGN_CENTER, 0, 0);
  lv_canvas_fill_bg(canvas, lv_color_white(), LV_OPA_100);

  // Set up canvas line-drawing style based on defaults.
  // Later we'll change color settings when drawing each line.
  lv_draw_line_dsc_init(&draw_dsc);
  */
}


#endif // USE_LVGL


#ifdef USE_TIMERS
int32_t get_tpars(uint32_t index, uint32_t sel) {
int32_t retval = 0;
  switch (sel) {
    case 0:
      retval = Settings->timer[index].time;
      break;
    case 1:
      //retval = Settings->timer[index].window;
      retval = timer_window[index];
      break;
    case 2:
      retval = Settings->timer[index].repeat;
      break;
    case 3:
      retval = Settings->timer[index].days;
      break;
    case 4:
      retval = Settings->timer[index].device;
      break;
    case 5:
      retval = Settings->timer[index].power;
      break;
    case 6:
      retval = Settings->timer[index].mode;
      break;
    case 7:
      retval = Settings->timer[index].arm;
      break;
    case 8:
      retval = Settings->flag3.timers_enable;
      break;

  }
  return retval;
}

#endif

#ifdef SCRIPT_FULL_WEBPAGE
void script_add_subpage(uint8_t num) {
  //uint8_t web_script = Run_Scripter(code, -strlen(code), 0);
  if (glob_script_mem.web_pages[num]) {
      char bname[48];
      cpy2lf(bname, sizeof(bname), glob_script_mem.web_pages[num] + 1);

      void (*wptr)(void);

      char id[8];
      switch (num) {
        case 1:
          wptr = ScriptFullWebpage1;
          break;
        case 2:
          wptr = ScriptFullWebpage2;
          break;
        case 3:
          wptr = ScriptFullWebpage3;
          break;
        case 4:
          wptr = ScriptFullWebpage4;
          break;
        case 5:
          wptr = ScriptFullWebpage5;
          break;
        case 6:
          wptr = ScriptFullWebpage6;
          break;
        case 7:
          wptr = ScriptFullWebpage7;
          break;
      }
      sprintf_P(id, PSTR("/sfd%1d"), num);
      Webserver->on(id, wptr);
      WSContentSend_P(HTTP_WEB_FULL_DISPLAY, num, bname);
  }
}
#endif // SCRIPT_FULL_WEBPAGE

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/
//const esp_partition_t *esp32_part;



bool Xdrv10(uint32_t function)
{
  bool result = false;
  glob_script_mem.event_handeled = false;
  char *sprt;

  switch (function) {
    //case FUNC_PRE_INIT:
    //case FUNC_INIT:
    case FUNC_SETUP_RING1:  // We need to setup SCRIPT before call to ScriptLoadSection()

      //bitWrite(Settings->rule_enabled, 0, 0); // >>>>>>>>>>>
#ifndef NO_SCRIPT_STOP_ON_ERROR
      bitWrite(Settings->rule_stop, 0, 1);
#endif

      // set defaults to rules memory
      //bitWrite(Settings->rule_enabled,0,0);
      glob_script_mem.script_ram = Settings->rules[0];
      glob_script_mem.script_size = MAX_SCRIPT_SIZE;
      glob_script_mem.FLAGS.fsys = false;
      glob_script_mem.FLAGS.eeprom = false;
      glob_script_mem.script_pram = (uint8_t*)Settings->script_pram[0];
      glob_script_mem.script_pram_size = PMEM_SIZE;

#ifdef USE_UFILESYS
      if (ufs_type) {
        // we have a file system
        AddLog(LOG_LEVEL_INFO,PSTR("SCR: ufilesystem found"));
        char *script;
        script = (char*)special_malloc(UFSYS_SIZE + 4);
        if (!script) break;
        memset(script, 0, UFSYS_SIZE);
        glob_script_mem.script_ram = script;
        glob_script_mem.script_size = UFSYS_SIZE;
        if (ufsp->exists(FAT_SCRIPT_NAME)) {
          File file = ufsp->open(FAT_SCRIPT_NAME, FS_FILE_READ);
          file.read((uint8_t*)script, UFSYS_SIZE);
          file.close();
        }
        script[UFSYS_SIZE - 1] = 0;
        // use rules storage for permanent vars
        glob_script_mem.script_pram = (uint8_t*)Settings->rules[0];
        glob_script_mem.script_pram_size = MAX_SCRIPT_SIZE;
        glob_script_mem.FLAGS.fsys = true;
        // indicates scripter use no compression
        bitWrite(Settings->rule_once, 6, 0);
      } else {
        AddLog(LOG_LEVEL_INFO,PSTR("SCR: No ufilesystem, using compression"));
        int32_t len_decompressed;
        sprt = (char*)calloc(UNISHOXRSIZE + 8,1);
        if (!sprt) { break; }
        glob_script_mem.script_ram = sprt;
        glob_script_mem.script_size = UNISHOXRSIZE;
        len_decompressed = SCRIPT_DECOMPRESS(Settings->rules[0], strlen(Settings->rules[0]), glob_script_mem.script_ram, glob_script_mem.script_size);
        if (len_decompressed>0) glob_script_mem.script_ram[len_decompressed] = 0;
        // indicates scripter use compression
        bitWrite(Settings->rule_once, 6, 1);
      }
#else // USE_UFILESYS

#ifdef EEP_SCRIPT_SIZE

      if (EEP_INIT(EEP_SCRIPT_SIZE)) {
          // found 32kb eeprom,
          char *script;
#if EEP_SCRIPT_SIZE<SPECIAL_EEPMODE_SIZE && EEP_SCRIPT_SIZE!=SPI_FLASH_SEC_SIZE
            script = (char*)calloc(EEP_SCRIPT_SIZE + 4, 1);
            if (!script) break;
            glob_script_mem.script_ram = script;
            glob_script_mem.script_size = EEP_SCRIPT_SIZE;
            EEP_READ(0, EEP_SCRIPT_SIZE, script);
            if (*script == 0xff) {
              memset(script, EEP_SCRIPT_SIZE, 0);
            }
            script[EEP_SCRIPT_SIZE - 1] = 0;
#else
            uint8_t *ucs;
#if EEP_SCRIPT_SIZE==SPI_FLASH_2SEC_SIZE || EEP_SCRIPT_SIZE==SPI_FLASH_SEC_SIZE
            ucs = (uint8_t*)calloc(EEP_SCRIPT_SIZE + 4, 1);
            if (!ucs) break;
            alt_eeprom_readBytes(0, EEP_SCRIPT_SIZE, ucs);
            if (*ucs == 0xff) {
              memset(ucs, EEP_SCRIPT_SIZE, 0);
            }
            ucs[EEP_SCRIPT_SIZE - 1] = 0;
            glob_script_mem.script_ram = (char*)ucs;
            glob_script_mem.script_size = EEP_SCRIPT_SIZE;

#else
            ucs = (uint8_t*)calloc(SPI_FLASH_SEC_SIZE + 4, 1);
            if (!ucs) break;
            alt_eeprom_readBytes(0, SPI_FLASH_SEC_SIZE, ucs);
            if (*ucs == 0xff) {
              memset(ucs, SPI_FLASH_SEC_SIZE, 0);
            }
            ucs[SPI_FLASH_SEC_SIZE - 1] = 0;

            script = (char*)calloc(EEP_SCRIPT_SIZE + 4, 1);
            if (!script) break;
            glob_script_mem.script_ram = script;
            glob_script_mem.script_size = EEP_SCRIPT_SIZE;

            int32_t len_decompressed;
            len_decompressed = SCRIPT_DECOMPRESS((char*)ucs, strlen((char*)ucs), glob_script_mem.script_ram, glob_script_mem.script_size);
            if (len_decompressed>0) glob_script_mem.script_ram[len_decompressed] = 0;

            if (ucs) free(ucs);
#endif // EEP_SCRIPT_SIZE==SPI_FLASH_2SEC_SIZE
#endif // EEP_SCRIPT_SIZE<SPECIAL_EEPMODE_SIZE

          // use rules storage for permanent vars
          glob_script_mem.script_pram = (uint8_t*)Settings->rules[0];
          glob_script_mem.script_pram_size = MAX_SCRIPT_SIZE;

          glob_script_mem.FLAGS.eeprom = true;
      }
#else // EEP_SCRIPT_SIZE

      // default mode is compression
      int32_t len_decompressed;
      sprt = (char*)calloc(UNISHOXRSIZE + 8,1);
      if (!sprt) { break; }
      glob_script_mem.script_ram = sprt;
      glob_script_mem.script_size = UNISHOXRSIZE;
      len_decompressed = SCRIPT_DECOMPRESS(Settings->rules[0], strlen(Settings->rules[0]), glob_script_mem.script_ram, glob_script_mem.script_size);
      if (len_decompressed>0) glob_script_mem.script_ram[len_decompressed] = 0;
      // indicates scripter use compression
      bitWrite(Settings->rule_once, 6, 1);

#endif

#endif // USE_UFILESYS


// indicates scripter enabled (use rules[][] as single array)
      bitWrite(Settings->rule_once, 7, 1);

#ifdef USE_BUTTON_EVENT
      for (uint32_t cnt = 0; cnt < MAX_KEYS; cnt++) {
        glob_script_mem.script_button[cnt] = -1;
      }
#endif //USE_BUTTON_EVENT

      // a valid script MUST start with >D
      if (glob_script_mem.script_ram[0] != '>' && glob_script_mem.script_ram[1] != 'D') {
        // clr all
        memset(glob_script_mem.script_ram, 0, glob_script_mem.script_size);
#ifdef PRECONFIGURED_SCRIPT
        strcpy_P(glob_script_mem.script_ram, PSTR(PRECONFIGURED_SCRIPT));
#else
        strcpy_P(glob_script_mem.script_ram, PSTR(">D\nscript error must start with >D"));
#endif
#ifdef START_SCRIPT_FROM_BOOT
        bitWrite(Settings->rule_enabled, 0, 1);
#else
        bitWrite(Settings->rule_enabled, 0, 0);
#endif
      }

      // assure permanent memory is 4 byte aligned
      { uint32_t ptr = (uint32_t)glob_script_mem.script_pram;
      ptr &= 0xfffffffc;
      ptr += 4;
      glob_script_mem.script_pram = (uint8_t*)ptr;
      glob_script_mem.script_pram_size -= 4;
      }

      if (bitRead(Settings->rule_enabled, 0)) Init_Scripter();

      break;
    case FUNC_INIT:
      if (bitRead(Settings->rule_enabled, 0)) {
        set_callbacks();
        Run_Scripter1(">B\n", 3, 0);
#ifdef USE_WEBSERVER
        script_set_web_pages();
#endif
#if defined(USE_SCRIPT_HUE) && defined(USE_WEBSERVER) && defined(USE_EMULATION) && defined(USE_EMULATION_HUE) && defined(USE_LIGHT)
        Script_Check_Hue(0);
#endif //USE_SCRIPT_HUE

#if defined(USE_SML_M) && defined(USE_SML_SCRIPT_CMD) && defined(USE_SCRIPT_SERIAL)
      glob_script_mem.hstr = 0;
#endif
      }
      break;
    case FUNC_EVERY_100_MSECOND:
      ScripterEvery100ms();
      break;

    case FUNC_EVERY_SECOND:
      ScriptEverySecond();
      break;
    case FUNC_COMMAND:
      result = ScriptCommand();
      break;
    case FUNC_SET_POWER:
#ifdef SCRIPT_POWER_SECTION
      if (bitRead(Settings->rule_enabled, 0)) Run_Scripter1(">P", 2, 0);
#else
      if (bitRead(Settings->rule_enabled, 0)) {
        //Run_Scripter(">E", 2, 0);
        if (glob_script_mem.event_script) Run_Scripter1(glob_script_mem.event_script, 0, 0);
        result = glob_script_mem.event_handeled;
      }
#endif //SCRIPT_POWER_SECTION
      break;
    case FUNC_RULES_PROCESS:
      if (bitRead(Settings->rule_enabled, 0)) {
#ifdef USE_SCRIPT_STATUS
        if (!strncmp_P(ResponseData(), PSTR("{\"Status"), 8)) {
          Run_Scripter(">U", 2, ResponseData());
        } else {
          //Run_Scripter(">E", 2, ResponseData());
          if (glob_script_mem.event_script) Run_Scripter(glob_script_mem.event_script, 0, ResponseData());
        }
#else
        //Run_Scripter(">E", 2, ResponseData());
        if (glob_script_mem.event_script) Run_Scripter(glob_script_mem.event_script, 0, ResponseData());
#endif

        result = glob_script_mem.event_handeled;
      }
      break;
    case FUNC_TELEPERIOD_RULES_PROCESS:
      if (bitRead(Settings->rule_enabled, 0)) {
        if (ResponseLength()) {
          //Run_Scripter(">T", 2, ResponseData());
          if (glob_script_mem.teleperiod) Run_Scripter(glob_script_mem.teleperiod, 0, ResponseData());
        }
      }
      break;
#ifdef USE_WEBSERVER
    case FUNC_WEB_ADD_CONSOLE_BUTTON:
      if (XdrvMailbox.index) {
        XdrvMailbox.index++;
      } else {
        WSContentSend_P(HTTP_BTN_MENU_RULES);
      }
      break;
#ifdef USE_SCRIPT_WEB_DISPLAY
    case FUNC_WEB_ADD_MAIN_BUTTON:
      if (bitRead(Settings->rule_enabled, 0)) {
        if (glob_script_mem.web_pages[WEB_PAGE_WM]) {
          ScriptWebShow('z', WEB_PAGE_WM);
        } else {
          ScriptWebShow('$', 0);
        }
#ifdef SCRIPT_FULL_WEBPAGE
        script_add_subpage(1);
        script_add_subpage(2);
        script_add_subpage(3);
        script_add_subpage(4);
        script_add_subpage(5);
        script_add_subpage(6);
        script_add_subpage(7);
#endif // SCRIPT_FULL_WEBPAGE

      }
      break;
#endif // USE_SCRIPT_WEB_DISPLAY
    case FUNC_WEB_ADD_HANDLER:
      Webserver->on("/" WEB_HANDLE_SCRIPT, HandleScriptConfiguration);
      Webserver->on("/ta",HTTP_POST, HandleScriptTextareaConfiguration);
      Webserver->on("/exs", HTTP_POST,[]() { Webserver->sendHeader("Location","/exs");Webserver->send(303);}, script_upload_start);
      Webserver->on("/exs", HTTP_GET, ScriptExecuteUploadSuccess);
#if defined(USE_UFILESYS) && defined(USE_SCRIPT_WEB_DISPLAY)
      Webserver->on(UriGlob("/ufs/*"), HTTP_GET, ScriptServeFile);
#endif
#if defined(USE_UFILESYS) && defined(USE_SCRIPT_ALT_DOWNLOAD)
      WebServer82Init();
#endif // USE_SCRIPT_ALT_DOWNLOAD
      break;
#endif // USE_WEBSERVER

    case FUNC_SAVE_BEFORE_RESTART:
      if (bitRead(Settings->rule_enabled, 0)) {
        Run_Scripter1(">R", 2, 0);
        Scripter_save_pvars();
      }
#ifdef USE_SCRIPT_GLOBVARS
      Script_Stop_UDP();
#endif //USE_SCRIPT_GLOBVARS
      break;
#ifdef SUPPORT_MQTT_EVENT
    case FUNC_MQTT_DATA:
      if (bitRead(Settings->rule_enabled, 0)) {
        result = ScriptMqttData();
      }
      break;
#endif    //SUPPORT_MQTT_EVENT
#ifdef USE_SCRIPT_WEB_DISPLAY
    case FUNC_WEB_SENSOR:
      if (bitRead(Settings->rule_enabled, 0)) {
        if (glob_script_mem.web_pages[WEB_PAGE_WS]) {
          ScriptWebShow(0, WEB_PAGE_WS);
        } else {
          ScriptWebShow(0, 0);
        }
      }
      break;
#endif //USE_SCRIPT_WEB_DISPLAY

#ifdef USE_SCRIPT_JSON_EXPORT
    case FUNC_JSON_APPEND:
      if (bitRead(Settings->rule_enabled, 0)) {
        ScriptJsonAppend();
      }
      break;
#endif //USE_SCRIPT_JSON_EXPORT

#ifdef USE_BUTTON_EVENT
    case FUNC_BUTTON_PRESSED:
      if (bitRead(Settings->rule_enabled, 0)) {
        if ((glob_script_mem.script_button[XdrvMailbox.index]&1)!=(XdrvMailbox.payload&1)) {
          glob_script_mem.script_button[XdrvMailbox.index] = XdrvMailbox.payload;
          Run_Scripter1(">b", 2, 0);
        }
      }
      break;
    case FUNC_BUTTON_MULTI_PRESSED:
      if (bitRead(Settings->rule_enabled, 0)) {
        Run_Scripter1(">b", 2, 0);
      }
#endif //USE_BUTTON_EVENT

    case FUNC_LOOP:
#ifdef USE_SCRIPT_GLOBVARS
      Script_PollUdp();
#endif //USE_SCRIPT_GLOBVARS
#ifdef USE_SCRIPT_ALT_DOWNLOAD
      WebServer82Loop();
#endif
      break;

    case FUNC_NETWORK_UP:
      break;
  }
  return result;
}

#endif  // Do not USE_RULES
#endif  // USE_SCRIPT
