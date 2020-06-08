/*
  xdrv_10_scripter.ino - script support for Tasmota

  Copyright (C) 2020  Gerhard Mutz and Theo Arends

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
g:var gloabal vars (via udp broadcast)

remarks

goal is fast execution time, minimal use of ram and intuitive syntax
therefore =>
case sensitive cmds and vars (lowercase uses time and code)
no math hierarchy  (costs ram and execution time, better group with brackets, anyhow better readable for beginners)
(will probably make math hierarchy an ifdefed option)
keywords if then else endif, or, and are better readable for beginners (others may use {})

\*********************************************************************************************/

#define XDRV_10             10
#define XI2C_37             37  // See I2CDEVICES.md

#define SCRIPT_DEBUG 0


#ifndef MAXVARS
#define MAXVARS 50
#endif
#ifndef MAXSVARS
#define MAXSVARS 5
#endif
#define MAXNVARS MAXVARS-MAXSVARS

#ifndef MAXFILT
#define MAXFILT 5
#endif
#define SCRIPT_SVARSIZE 20
#define SCRIPT_MAXSSIZE 48
#define SCRIPT_EOL '\n'
#define SCRIPT_FLOAT_PRECISION 2
#define PMEM_SIZE sizeof(Settings.script_pram)
#define SCRIPT_MAXPERM (PMEM_SIZE)-4/sizeof(float)
#define MAX_SCRIPT_SIZE MAX_RULE_SIZE*MAX_RULE_SETS


uint32_t EncodeLightId(uint8_t relay_id);
uint32_t DecodeLightId(uint32_t hue_id);

// solve conficting defines
// highest priority
#ifdef USE_SCRIPT_FATFS
#undef LITTLEFS_SCRIPT_SIZE
#undef EEP_SCRIPT_SIZE
#undef USE_UNISHOX_COMPRESSION
#if USE_SCRIPT_FATFS==-1
#ifdef ESP32
#error "script fat file option -1 currently not supported for ESP32"
#else
#pragma message "script fat file option -1 used"
#endif
#else
#pragma message "script fat file SDC option used"
#endif
#endif // USE_SCRIPT_FATFS

// lfs on esp8266 spiffs on esp32
#ifdef LITTLEFS_SCRIPT_SIZE
#undef EEP_SCRIPT_SIZE
#undef USE_UNISHOX_COMPRESSION
#pragma message "script little file system option used"
#endif // LITTLEFS_SCRIPT_SIZE

// eeprom script
#ifdef EEP_SCRIPT_SIZE
#undef USE_UNISHOX_COMPRESSION
#ifdef USE_24C256
#pragma message "script 24c256 file option used"
#else
#warning "EEP_SCRIPT_SIZE also needs USE_24C256"
#define USE_24C256
#endif
#endif // EEP_SCRIPT_SIZE

// compression last option before default
#ifdef USE_UNISHOX_COMPRESSION
#pragma message "script compression option used"
#endif // USE_UNISHOX_COMPRESSION


#ifdef USE_UNISHOX_COMPRESSION
#include <unishox.h>

#define SCRIPT_COMPRESS compressor.unishox_compress
#define SCRIPT_DECOMPRESS compressor.unishox_decompress
#ifndef UNISHOXRSIZE
#define UNISHOXRSIZE 2560
#endif
#endif // USE_UNISHOX_COMPRESSION


#if defined(LITTLEFS_SCRIPT_SIZE) || (USE_SCRIPT_FATFS==-1)
#ifdef ESP32
#include "FS.h"
#include "SPIFFS.h"
#else
#include <LittleFS.h>
#endif
FS *fsp;
#endif // LITTLEFS_SCRIPT_SIZE


#ifdef LITTLEFS_SCRIPT_SIZE
void SaveFile(const char *name,const uint8_t *buf,uint32_t len) {
  File file = fsp->open(name, "w");
  if (!file) return;
  file.write(buf, len);
  file.close();
}

#define FORMAT_SPIFFS_IF_FAILED true
uint8_t fs_mounted=0;

void LoadFile(const char *name,uint8_t *buf,uint32_t len) {
  if (!fs_mounted) {
#ifdef ESP32
    if(!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)){
#else
    if(!fsp->begin()){
#endif
          //Serial.println("SPIFFS Mount Failed");
      return;
    }
    fs_mounted=1;
  }
  File file = fsp->open(name, "r");
  if (!file) return;
  file.read(buf, len);
  file.close();
}
#endif // LITTLEFS_SCRIPT_SIZE

// offsets epoch readings by 1.1.2019 00:00:00 to fit into float with second resolution
#define EPOCH_OFFSET 1546300800

enum {OPER_EQU=1,OPER_PLS,OPER_MIN,OPER_MUL,OPER_DIV,OPER_PLSEQU,OPER_MINEQU,OPER_MULEQU,OPER_DIVEQU,OPER_EQUEQU,OPER_NOTEQU,OPER_GRTEQU,OPER_LOWEQU,OPER_GRT,OPER_LOW,OPER_PERC,OPER_XOR,OPER_AND,OPER_OR,OPER_ANDEQU,OPER_OREQU,OPER_XOREQU,OPER_PERCEQU};
enum {SCRIPT_LOGLEVEL=1,SCRIPT_TELEPERIOD};

#ifdef USE_SCRIPT_FATFS

#if USE_SCRIPT_FATFS>=0
#include <SPI.h>
#include <SD.h>
#ifdef ESP32
FS *fsp;
#else
SDClass *fsp;
#endif
#endif

#ifndef ESP32
// esp8266

#if USE_SCRIPT_FATFS>=0
// old fs
#undef FILE_WRITE
#define FILE_WRITE (sdfat::O_READ | sdfat::O_WRITE | sdfat::O_CREAT)
#define FILE_APPEND (sdfat::O_READ | sdfat::O_WRITE | sdfat::O_CREAT | sdfat::O_APPEND)

#else
// new fs
#undef FILE_WRITE
#define FILE_WRITE "w"
#undef FILE_READ
#define FILE_READ "r"
#undef FILE_APPEND
#define FILE_APPEND "a"
#endif

#endif // USE_SCRIPT_FATFS>=0


#ifndef FAT_SCRIPT_SIZE
#define FAT_SCRIPT_SIZE 4096
#endif

#ifdef ESP32
#undef FAT_SCRIPT_NAME
#define FAT_SCRIPT_NAME "/script.txt"
#else
#undef FAT_SCRIPT_NAME
#define FAT_SCRIPT_NAME "script.txt"
#endif

#if USE_STANDARD_SPI_LIBRARY==0
#warning ("FATFS standard spi should be used");
#endif

#endif // USE_SCRIPT_FATFS

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
extern VButton *buttons[MAXBUTTONS];
#endif
#endif

typedef union {
  uint8_t data;
  struct {
    uint8_t is_string : 1;  // string or number
    uint8_t is_permanent : 1;
    uint8_t is_timer : 1;
    uint8_t is_autoinc : 1;
    uint8_t changed : 1;
    uint8_t settable : 1;
    uint8_t is_filter : 1;
    uint8_t constant : 1;
  };
} SCRIPT_TYPE;

struct T_INDEX {
  uint8_t index;
  SCRIPT_TYPE bits;
};

struct M_FILT {
  uint8_t numvals;
  uint8_t index;
  float maccu;
  float rbuff[1];
};

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

#define SFS_MAX 4
// global memory
struct SCRIPT_MEM {
    float *fvars; // number var pointer
    float *s_fvars; // shadow var pointer
    struct T_INDEX *type; // type and index pointer
    struct M_FILT *mfilt;
    char *glob_vnp; // var name pointer
    uint8_t *vnp_offset;
    char *glob_snp; // string vars pointer
    char *scriptptr;
    char *section_ptr;
    char *scriptptr_bu;
    char *script_ram;
    uint16_t script_size;
    uint8_t *script_pram;
    uint16_t script_pram_size;
    uint8_t numvars;
    void *script_mem;
    uint16_t script_mem_size;
    uint8_t script_dprec;
    uint8_t var_not_found;
    uint8_t glob_error;
    uint8_t max_ssize;
    uint8_t script_loglevel;
    uint8_t flags;
#ifdef USE_SCRIPT_FATFS
    File files[SFS_MAX];
    FILE_FLAGS file_flags[SFS_MAX];
    uint8_t script_sd_found;
    char flink[2][14];
#endif
} glob_script_mem;


int16_t last_findex;
uint8_t tasm_cmd_activ=0;
uint8_t fast_script=0;
uint32_t script_lastmillis;


#ifdef USE_BUTTON_EVENT
int8_t script_button[MAX_KEYS];
#endif

char *GetNumericResult(char *lp,uint8_t lastop,float *fp,JsonObject *jo);
char *GetStringResult(char *lp,uint8_t lastop,char *cp,JsonObject *jo);
char *ForceStringVar(char *lp,char *dstr);
void send_download(void);
uint8_t reject(char *name);

void ScriptEverySecond(void) {

  if (bitRead(Settings.rule_enabled, 0)) {
    struct T_INDEX *vtp=glob_script_mem.type;
    float delta=(millis()-script_lastmillis)/1000;
    script_lastmillis=millis();
    for (uint8_t count=0; count<glob_script_mem.numvars; count++) {
      if (vtp[count].bits.is_timer) {
        // decrements timers
        float *fp=&glob_script_mem.fvars[vtp[count].index];
        if (*fp>0) {
          // decrement
          *fp-=delta;
          if (*fp<0) *fp=0;
        }
      }
      if (vtp[count].bits.is_autoinc) {
        // increments timers
        float *fp=&glob_script_mem.fvars[vtp[count].index];
        if (*fp>=0) {
          *fp+=delta;
        }
      }
    }
    Run_Scripter(">S",2,0);
  }
}

void RulesTeleperiod(void) {
  if (bitRead(Settings.rule_enabled, 0) && mqtt_data[0]) Run_Scripter(">T",2, mqtt_data);
}

// EEPROM MACROS
// i2c eeprom
#define EEP_WRITE(A,B,C) eeprom_writeBytes(A,B,(uint8_t*)C);
#define EEP_READ(A,B,C) eeprom_readBytes(A,B,(uint8_t*)C);


#define SCRIPT_SKIP_SPACES while (*lp==' ' || *lp=='\t') lp++;
#define SCRIPT_SKIP_EOL while (*lp==SCRIPT_EOL) lp++;

// allocates all variables and presets them
int16_t Init_Scripter(void) {
char *script;

    script=glob_script_mem.script_ram;

    // scan lines for >DEF
    uint16_t lines=0,nvars=0,svars=0,vars=0;
    char *lp=script;
    char vnames[MAXVARS*10];
    char *vnames_p=vnames;
    char *vnp[MAXVARS];
    char **vnp_p=vnp;
    char strings[MAXSVARS*SCRIPT_MAXSSIZE];
    struct M_FILT mfilt[MAXFILT];

    char *strings_p=strings;
    char *snp[MAXSVARS];
    char **snp_p=snp;
    uint8_t numperm=0,numflt=0,count;

    glob_script_mem.max_ssize=SCRIPT_SVARSIZE;
    glob_script_mem.scriptptr=0;

    if (!*script) return -999;

    float fvalues[MAXVARS];
    struct T_INDEX vtypes[MAXVARS];
    char init=0;
    while (1) {
        // check line
        // skip leading spaces
        SCRIPT_SKIP_SPACES
        // skip empty line
        if (*lp=='\n' || *lp=='\r') goto next_line;
        // skip comment
        if (*lp==';') goto next_line;
        if (init) {
            // init section
            if (*lp=='>') {
                init=0;
                break;
            }
            char *op=strchr(lp,'=');
            if (op) {
                vtypes[vars].bits.data=0;
                // found variable definition
                if (*lp=='p' && *(lp+1)==':') {
                    lp+=2;
                    if (numperm<SCRIPT_MAXPERM) {
                      vtypes[vars].bits.is_permanent=1;
                      numperm++;
                    }
                } else {
                    vtypes[vars].bits.is_permanent=0;
                }
                if (*lp=='t' && *(lp+1)==':') {
                    lp+=2;
                    vtypes[vars].bits.is_timer=1;
                } else {
                    vtypes[vars].bits.is_timer=0;
                }
                if (*lp=='i' && *(lp+1)==':') {
                    lp+=2;
                    vtypes[vars].bits.is_autoinc=1;
                } else {
                    vtypes[vars].bits.is_autoinc=0;
                }
                if ((*lp=='m' || *lp=='M') && *(lp+1)==':') {
                    uint8_t flg=*lp;
                    lp+=2;
                    if (flg=='M') mfilt[numflt].numvals=8;
                    else mfilt[numflt].numvals=5;
                    vtypes[vars].bits.is_filter=1;
                    mfilt[numflt].index=0;
                    if (flg=='M') {
                      mfilt[numflt].numvals|=0x80;
                    }
                    vtypes[vars].index=numflt;
                    numflt++;
                    if (numflt>MAXFILT) {
                      return -6;
                    }
                } else {
                    vtypes[vars].bits.is_filter=0;
                }
                *vnp_p++=vnames_p;
                while (lp<op) {
                    *vnames_p++=*lp++;
                }
                *vnames_p++=0;
                // init variable
                op++;
                if (*op!='"') {
                    float fv;
                    if (*op=='0' && *(op+1)=='x') {
                      op+=2;
                      fv=strtol(op,&op,16);
                    } else {
                      fv=CharToFloat(op);
                    }
                    fvalues[nvars]=fv;
                    vtypes[vars].bits.is_string=0;
                    if (!vtypes[vars].bits.is_filter) vtypes[vars].index=nvars;
                    nvars++;
                    if (nvars>MAXNVARS) {
                      return -1;
                    }
                    if (vtypes[vars].bits.is_filter) {
                      while (isdigit(*op) || *op=='.' || *op=='-') {
                        op++;
                      }
                      while (*op==' ') op++;
                      if (isdigit(*op)) {
                        // lenght define follows
                        uint8_t flen=atoi(op);
                        mfilt[numflt-1].numvals&=0x80;
                        mfilt[numflt-1].numvals|=flen&0x7f;
                      }
                    }

                } else {
                    // string vars
                    op++;
                    *snp_p++=strings_p;
                    while (*op!='\"') {
                      if (*op==SCRIPT_EOL) break;
                      *strings_p++=*op++;
                    }
                    *strings_p++=0;
                    vtypes[vars].bits.is_string=1;
                    vtypes[vars].index=svars;
                    svars++;
                    if (svars>MAXSVARS) {
                      return -2;
                    }
                }
                vars++;
                if (vars>MAXVARS) {
                  return -3;
                }
            }
        } else {
            if (!strncmp(lp,">D",2)) {
              lp+=2;
              SCRIPT_SKIP_SPACES
              if (isdigit(*lp)) {
                uint8_t ssize=atoi(lp)+1;
                if (ssize<10 || ssize>SCRIPT_MAXSSIZE) ssize=SCRIPT_MAXSSIZE;
                glob_script_mem.max_ssize=ssize;
              }
              init=1;
            }
        }
        // next line
    next_line:
        lp = strchr(lp, SCRIPT_EOL);
        if (!lp) break;
        lp++;
    }

    uint16_t fsize=0;
    for (count=0; count<numflt; count++) {
      fsize+=sizeof(struct M_FILT)+((mfilt[count].numvals&0x7f)-1)*sizeof(float);
    }

    // now copy vars to memory
    uint16_t script_mem_size=
    // number and number shadow vars
    (sizeof(float)*nvars)+
    (sizeof(float)*nvars)+
    // var names
    (vnames_p-vnames)+
    // vars offsets
    (sizeof(uint8_t)*vars)+
    // strings
    (glob_script_mem.max_ssize*svars)+
    // type array
    (sizeof(struct T_INDEX)*vars)+
    fsize;

    script_mem_size+=16;
    uint8_t *script_mem;
    script_mem=(uint8_t*)calloc(script_mem_size,1);
    if (!script_mem) {
      return -4;
    }
    glob_script_mem.script_mem=script_mem;
    glob_script_mem.script_mem_size=script_mem_size;

    // now copy all vars
    // numbers
    glob_script_mem.fvars=(float*)script_mem;
    uint16_t size=sizeof(float)*nvars;
    memcpy(script_mem,fvalues,size);
    script_mem+=size;
    glob_script_mem.s_fvars=(float*)script_mem;
    size=sizeof(float)*nvars;
    memcpy(script_mem,fvalues,size);
    script_mem+=size;

    glob_script_mem.mfilt=(struct M_FILT*)script_mem;
    script_mem+=fsize;

    // memory types
    glob_script_mem.type=(struct T_INDEX*)script_mem;
    size=sizeof(struct T_INDEX)*vars;
    memcpy(script_mem,vtypes,size);
    script_mem+=size;

    // var name strings
    char *namep=(char*)script_mem;
    glob_script_mem.glob_vnp=(char*)script_mem;
    size=vnames_p-vnames;
    memcpy(script_mem,vnames,size);
    script_mem+=size;

    glob_script_mem.vnp_offset=(uint8_t*)script_mem;
    size=vars*sizeof(uint8_t);
    script_mem+=size;

    // strings
    char *snamep=(char*)script_mem;
    glob_script_mem.glob_snp=(char*)script_mem;
    size=glob_script_mem.max_ssize*svars;
    //memcpy(script_mem,strings,size);
    script_mem+=size;

    // now must recalc memory offsets
    uint16_t index=0;
    uint8_t *cp=glob_script_mem.vnp_offset;
    for (count=0;count<vars;count++) {
        *cp++=index;
        while (*namep) {
            index++;
            namep++;
        }
        namep++;
        index++;
        if (index>255) {
          free(glob_script_mem.script_mem);
          return -5;
        }
    }
    // variables usage info
    AddLog_P2(LOG_LEVEL_INFO, PSTR("Script: nv=%d, tv=%d, vns=%d, ram=%d"), nvars, svars, index, glob_script_mem.script_mem_size);

    // copy string variables
    char *cp1=glob_script_mem.glob_snp;
    char *sp=strings;
    for (count=0; count<svars;count++) {
        strcpy(cp1,sp);
        sp+=strlen(sp)+1;
        cp1+=glob_script_mem.max_ssize;
    }

    // setup filter vars
    uint8_t *mp=(uint8_t*)glob_script_mem.mfilt;
    for (count=0; count<numflt; count++) {
      struct M_FILT *mflp=(struct M_FILT*)mp;
      mflp->numvals=mfilt[count].numvals;
      mp+=sizeof(struct M_FILT)+((mfilt[count].numvals&0x7f)-1)*sizeof(float);
    }

    glob_script_mem.numvars=vars;
    glob_script_mem.script_dprec=SCRIPT_FLOAT_PRECISION;
    glob_script_mem.script_loglevel=LOG_LEVEL_INFO;


#if SCRIPT_DEBUG>2
    struct T_INDEX *dvtp=glob_script_mem.type;
    for (uint8_t count=0; count<glob_script_mem.numvars; count++) {
      if (dvtp[count].bits.is_string) {

      } else {
        char string[32];
        dtostrfd(glob_script_mem.fvars[dvtp[count].index],glob_script_mem.script_dprec,string);
        toLog(string);
      }
    }
#endif

    // now preset permanent vars
    float *fp=(float*)glob_script_mem.script_pram;
    struct T_INDEX *vtp=glob_script_mem.type;
    for (uint8_t count=0; count<glob_script_mem.numvars; count++) {
      if (vtp[count].bits.is_permanent && !vtp[count].bits.is_string) {
        uint8_t index=vtp[count].index;
        if (!isnan(*fp)) {
          glob_script_mem.fvars[index]=*fp;
        } else {
          *fp=glob_script_mem.fvars[index];
        }
        fp++;
      }
    }
    sp=(char*)fp;
    for (uint8_t count=0; count<glob_script_mem.numvars; count++) {
      if (vtp[count].bits.is_permanent && vtp[count].bits.is_string) {
        uint8_t index=vtp[count].index;
        char *dp=glob_script_mem.glob_snp+(index*glob_script_mem.max_ssize);
        uint8_t slen=strlen(sp);
        strlcpy(dp,sp,glob_script_mem.max_ssize);
        sp+=slen+1;
      }
    }


#ifdef USE_SCRIPT_FATFS
    if (!glob_script_mem.script_sd_found) {

#if USE_SCRIPT_FATFS>=0
      fsp=&SD;
      if (SD.begin(USE_SCRIPT_FATFS)) {
#else
      if (fsp->begin()) {
#endif
        glob_script_mem.script_sd_found=1;
      } else {
        glob_script_mem.script_sd_found=0;
      }
    }
    for (uint8_t cnt=0;cnt<SFS_MAX;cnt++) {
      glob_script_mem.file_flags[cnt].is_open=0;
    }
#endif

#if SCRIPT_DEBUG>0
    ClaimSerial();
    SetSerialBaudrate(9600);
#endif

    // store start of actual program here
    glob_script_mem.scriptptr=lp-1;
    glob_script_mem.scriptptr_bu=glob_script_mem.scriptptr;
    return 0;

}

#ifdef USE_LIGHT
#ifdef USE_WS2812
void ws2812_set_array(float *array ,uint32_t len, uint32_t offset) {

  Ws2812ForceSuspend();
  for (uint32_t cnt=0;cnt<len;cnt++) {
    uint32_t index=cnt+offset;
    if (index>Settings.light_pixels) break;
    uint32_t col=array[cnt];
    Ws2812SetColor(index+1,col>>16,col>>8,col,0);
  }
  Ws2812ForceUpdate();
}
#endif
#endif

#define NUM_RES 0xfe
#define STR_RES 0xfd
#define VAR_NV 0xff

#define NTYPE 0
#define STYPE 0x80

#ifndef FLT_MAX
#define FLT_MAX 99999999
#endif

float median_array(float *array,uint8_t len) {
    uint8_t ind[len];
    uint8_t mind=0,index=0,flg;
    float min=FLT_MAX;

    for (uint8_t hcnt=0; hcnt<len/2+1; hcnt++) {
        for (uint8_t mcnt=0; mcnt<len; mcnt++) {
            flg=0;
            for (uint8_t icnt=0; icnt<index; icnt++) {
                if (ind[icnt]==mcnt) {
                    flg=1;
                }
            }
            if (!flg) {
                if (array[mcnt]<min) {
                    min=array[mcnt];
                    mind=mcnt;
                }
            }
        }
        ind[index]=mind;
        index++;
        min=FLT_MAX;
    }
    return array[ind[len/2]];
}


float *Get_MFAddr(uint8_t index,uint8_t *len,uint8_t *ipos) {
  *len=0;
  uint8_t *mp=(uint8_t*)glob_script_mem.mfilt;
  for (uint8_t count=0; count<MAXFILT; count++) {
    struct M_FILT *mflp=(struct M_FILT*)mp;
    if (count==index) {
        *len=mflp->numvals&0x7f;
        if (ipos) *ipos=mflp->index;
        return mflp->rbuff;
    }
    mp+=sizeof(struct M_FILT)+((mflp->numvals&0x7f)-1)*sizeof(float);
  }
  return 0;
}


float Get_MFVal(uint8_t index,uint8_t bind) {
  uint8_t *mp=(uint8_t*)glob_script_mem.mfilt;
  for (uint8_t count=0; count<MAXFILT; count++) {
    struct M_FILT *mflp=(struct M_FILT*)mp;
    if (count==index) {
        uint8_t maxind=mflp->numvals&0x7f;
        if (!bind) {
          return mflp->index;
        }
        if (bind<1 || bind>maxind) bind=maxind;
        return mflp->rbuff[bind-1];
    }
    mp+=sizeof(struct M_FILT)+((mflp->numvals&0x7f)-1)*sizeof(float);
  }
  return 0;
}

void Set_MFVal(uint8_t index,uint8_t bind,float val) {
  uint8_t *mp=(uint8_t*)glob_script_mem.mfilt;
  for (uint8_t count=0; count<MAXFILT; count++) {
    struct M_FILT *mflp=(struct M_FILT*)mp;
    if (count==index) {
        uint8_t maxind=mflp->numvals&0x7f;
        if (!bind) {
          mflp->index=val;
        } else {
          if (bind<1 || bind>maxind) bind=maxind;
          mflp->rbuff[bind-1]=val;
        }
        return;
    }
    mp+=sizeof(struct M_FILT)+((mflp->numvals&0x7f)-1)*sizeof(float);
  }
}


float Get_MFilter(uint8_t index) {
  uint8_t *mp=(uint8_t*)glob_script_mem.mfilt;
  for (uint8_t count=0; count<MAXFILT; count++) {
    struct M_FILT *mflp=(struct M_FILT*)mp;
    if (count==index) {
      if (mflp->numvals&0x80) {
        // moving average
        return mflp->maccu/(mflp->numvals&0x7f);
      } else {
        // median, sort array indices
        return median_array(mflp->rbuff,mflp->numvals);
      }
    }
    mp+=sizeof(struct M_FILT)+((mflp->numvals&0x7f)-1)*sizeof(float);
  }
  return 0;
}

void Set_MFilter(uint8_t index, float invar) {
  uint8_t *mp=(uint8_t*)glob_script_mem.mfilt;
  for (uint8_t count=0; count<MAXFILT; count++) {
    struct M_FILT *mflp=(struct M_FILT*)mp;
    if (count==index) {
      if (mflp->numvals&0x80) {
        // moving average
        mflp->maccu-=mflp->rbuff[mflp->index];
        mflp->maccu+=invar;
        mflp->rbuff[mflp->index]=invar;
        mflp->index++;
        if (mflp->index>=(mflp->numvals&0x7f)) mflp->index=0;
      } else {
        // median
        mflp->rbuff[mflp->index]=invar;
        mflp->index++;
        if (mflp->index>=mflp->numvals) mflp->index=0;
      }
      break;
    }
    mp+=sizeof(struct M_FILT)+((mflp->numvals&0x7f)-1)*sizeof(float);
  }
}

#define MEDIAN_SIZE 5
#define MEDIAN_FILTER_NUM 2

struct MEDIAN_FILTER {
float buffer[MEDIAN_SIZE];
int8_t index;
} script_mf[MEDIAN_FILTER_NUM];

float DoMedian5(uint8_t index, float in) {

  if (index>=MEDIAN_FILTER_NUM) index=0;

  struct MEDIAN_FILTER* mf=&script_mf[index];
  mf->buffer[mf->index]=in;
  mf->index++;
  if (mf->index>=MEDIAN_SIZE) mf->index=0;
  return median_array(mf->buffer,MEDIAN_SIZE);
}

#ifdef USE_LIGHT
//#ifdef USE_WS2812
uint32_t HSVToRGB(uint16_t hue, uint8_t saturation, uint8_t value) {
float r = 0, g = 0, b = 0;
struct HSV {
	float H;
	float S;
	float V;
} hsv;

hsv.H=hue;
hsv.S=(float)saturation/100.0;
hsv.V=(float)value/100.0;

if (hsv.S == 0) {
		r = hsv.V;
		g = hsv.V;
		b = hsv.V;
	} else {
		int i;
		float f, p, q, t;

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

  uint8_t ir,ig,ib;
  ir=r*255;
  ig=g*255;
  ib=b*255;

	uint32_t rgb=(ir<<16)|(ig<<8)|ib;
	return rgb;
}
#endif
//#endif

#ifdef USE_ANGLE_FUNC
uint32_t pulse_time_hl;
uint32_t pulse_time_lh;
uint32_t pulse_ltime_hl;
uint32_t pulse_ltime_lh;
uint8_t pt_pin;

void MP_Timer(void) ICACHE_RAM_ATTR;

#define MPT_DEBOUNCE 10

void MP_Timer(void) {
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
    pinMode(pt_pin&0x3f,INPUT_PULLUP);
    attachInterrupt(pt_pin&0x3f, MP_Timer, CHANGE);
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

// vtype => ff=nothing found, fe=constant number,fd = constant string else bit 7 => 80 = string, 0 = number
// no flash strings here for performance reasons!!!
char *isvar(char *lp, uint8_t *vtype,struct T_INDEX *tind,float *fp,char *sp,JsonObject *jo) {
    uint16_t count,len=0;
    uint8_t nres=0;
    char vname[32];
    float fvar=0;
    tind->index=0;
    tind->bits.data=0;

    if (isdigit(*lp) || (*lp=='-' && isdigit(*(lp+1))) || *lp=='.') {
      // isnumber
        if (fp) {
          if (*lp=='0' && *(lp+1)=='x') {
            lp+=2;
            *fp=strtol(lp,0,16);
          } else {
            *fp=CharToFloat(lp);
          }
        }
        if (*lp=='-') lp++;
        while (isdigit(*lp) || *lp=='.') {
          if (*lp==0 || *lp==SCRIPT_EOL) break;
          lp++;
        }
        tind->bits.constant=1;
        tind->bits.is_string=0;
        *vtype=NUM_RES;
        return lp;
    }
    if (*lp=='"') {
      lp++;
      while (*lp!='"') {
        if (*lp==0 || *lp==SCRIPT_EOL) break;
        uint8_t iob=*lp;
        if (iob=='\\') {
          lp++;
          if (*lp=='t') {
            iob='\t';
          } else if (*lp=='n') {
            iob='\n';
          } else if (*lp=='r') {
            iob='\r';
          } else if (*lp=='\\') {
            iob='\\';
          } else {
            lp--;
          }
          if (sp) *sp++=iob;
        } else {
          if (sp) *sp++=iob;
        }
        lp++;
      }
      if (sp) *sp=0;
      *vtype=STR_RES;
      tind->bits.constant=1;
      tind->bits.is_string=1;
      return lp+1;
    }

    if (*lp=='-') {
      // inverted var
      nres=1;
      lp++;
    }

    const char *term="\n\r ])=+-/*%><!^&|}";
    for (count=0; count<sizeof(vname); count++) {
        char iob=lp[count];
        if (!iob || strchr(term,iob)) {
            vname[count]=0;
            break;
        }
        vname[count]=iob;
        len+=1;
    }

    if (!vname[0]) {
      // empty string
      *vtype=VAR_NV;
      tind->index=VAR_NV;
      glob_script_mem.var_not_found=1;
      return lp;
    }

    struct T_INDEX *vtp=glob_script_mem.type;
    char dvnam[32];
    strcpy (dvnam,vname);
    uint8_t olen=len;
    last_findex=-1;
    char *ja=strchr(dvnam,'[');
    if (ja) {
      *ja=0;
      ja++;
      olen=strlen(dvnam);
    }
    for (count=0; count<glob_script_mem.numvars; count++) {
        char *cp=glob_script_mem.glob_vnp+glob_script_mem.vnp_offset[count];
        uint8_t slen=strlen(cp);
        if (slen==olen && *cp==dvnam[0]) {
            if (!strncmp(cp,dvnam,olen)) {
                uint8_t index=vtp[count].index;
                *tind=vtp[count];
                tind->index=count; // overwrite with global var index
                if (vtp[count].bits.is_string==0) {
                    *vtype=NTYPE|index;
                    if (vtp[count].bits.is_filter) {
                      if (ja) {
                        lp+=olen+1;
                        lp=GetNumericResult(lp,OPER_EQU,&fvar,0);
                        last_findex=fvar;
                        fvar=Get_MFVal(index,fvar);
                        len=1;
                      } else {
                        fvar=Get_MFilter(index);
                      }
                    } else {
                      fvar=glob_script_mem.fvars[index];
                    }
                    if (nres) fvar=-fvar;
                    if (fp) *fp=fvar;
                } else {
                    *vtype=STYPE|index;
                    if (sp) strlcpy(sp,glob_script_mem.glob_snp+(index*glob_script_mem.max_ssize),SCRIPT_MAXSSIZE);
                }
                return lp+len;
            }
        }
    }

    if (jo) {
      // look for json input
      const char* str_value;
      uint8_t aindex;
      String vn;
      char *ja=strchr(vname,'[');
      if (ja) {
        // json array
        *ja=0;
        ja++;
        // fetch array index
        float fvar;
        GetNumericResult(ja,OPER_EQU,&fvar,0);
        aindex=fvar;
        if (aindex<1 || aindex>6) aindex=1;
        aindex--;
      }
      if (jo->success()) {
        char *subtype=strchr(vname,'#');
        char *subtype2;
        if (subtype) {
          *subtype=0;
          subtype++;
          subtype2=strchr(subtype,'#');
          if (subtype2) {
            *subtype2=0;
            *subtype2++;
          }
        }
        vn=vname;
        str_value = (*jo)[vn];
        if ((*jo)[vn].success()) {
          if (subtype) {
            JsonObject &jobj1=(*jo)[vn];
            if (jobj1.success()) {
              vn=subtype;
              jo=&jobj1;
              str_value = (*jo)[vn];
              if ((*jo)[vn].success()) {
                // 2. stage
                if (subtype2) {
                  JsonObject &jobj2=(*jo)[vn];
                  if ((*jo)[vn].success()) {
                    vn=subtype2;
                    jo=&jobj2;
                    str_value = (*jo)[vn];
                    if ((*jo)[vn].success()) {
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
            str_value = (*jo)[vn][aindex];
          }
          if (str_value && *str_value) {
            if ((*jo).is<char*>(vn)) {
              if (!strncmp(str_value,"ON",2)) {
                if (fp) *fp=1;
                goto nexit;
              } else if (!strncmp(str_value,"OFF",3)) {
                if (fp) *fp=0;
                goto nexit;
              } else {
                *vtype=STR_RES;
                tind->bits.constant=1;
                tind->bits.is_string=1;
                if (sp) strlcpy(sp,str_value,SCRIPT_MAXSSIZE);
                return lp+len;
              }

            } else {
              if (fp) {
                if (!strncmp(vn.c_str(),"Epoch",5)) {
                  *fp=atoi(str_value)-(uint32_t)EPOCH_OFFSET;
                } else {
                  *fp=CharToFloat((char*)str_value);
                }
              }
              nexit:
              *vtype=NUM_RES;
              tind->bits.constant=1;
              tind->bits.is_string=0;
              return lp+len;
            }
          }
        }
      }
    }

chknext:
    switch (vname[0]) {
      case 'a':
#ifdef USE_ANGLE_FUNC
        if (!strncmp(vname,"acos(",5)) {
            lp+=5;
            lp=GetNumericResult(lp,OPER_EQU,&fvar,0);
            fvar=acosf(fvar);
            lp++;
            len=0;
            goto exit;
        }
#endif
        break;

      case 'b':
        if (!strncmp(vname,"boot",4)) {
          if (rules_flag.system_boot) {
            rules_flag.system_boot=0;
            fvar=1;
          }
          goto exit;
        }
#ifdef USE_BUTTON_EVENT
        if (!strncmp(vname,"bt[",3)) {
          // tasmota button state
          GetNumericResult(vname+3,OPER_EQU,&fvar,0);
          uint32_t index=fvar;
          if (index<1 || index>MAX_KEYS) index=1;
          fvar=script_button[index-1];
          script_button[index-1]|=0x80;
          len++;
          goto exit;
        }
#endif
        break;
      case 'c':
        if (!strncmp(vname,"chg[",4)) {
          // var changed
          struct T_INDEX ind;
          uint8_t vtype;
          isvar(vname+4,&vtype,&ind,0,0,0);
          if (!ind.bits.constant) {
            uint8_t index=glob_script_mem.type[ind.index].index;
            if (glob_script_mem.fvars[index]!=glob_script_mem.s_fvars[index]) {
              // var has changed
              glob_script_mem.s_fvars[index]=glob_script_mem.fvars[index];
              fvar=1;
              len++;
              goto exit;
            } else {
              fvar=0;
              len++;
              goto exit;
            }
          }
        }
#ifdef ESP32
        if (!strncmp(vname,"core",4)) {
          fvar=xPortGetCoreID();
          goto exit;
        }
#ifdef USE_SCRIPT_TASK
        if (!strncmp(vname,"ct(",3)) {
          lp+=3;
          lp=GetNumericResult(lp,OPER_EQU,&fvar,0);
          while (*lp==' ') lp++;
          float fvar1;
          lp=GetNumericResult(lp,OPER_EQU,&fvar1,0);
          while (*lp==' ') lp++;
          float fvar2;
          lp=GetNumericResult(lp,OPER_EQU,&fvar2,0);
          lp++;
          fvar=scripter_create_task(fvar,fvar1,fvar2);
          len=0;
          goto exit;
        }
#endif
#endif
        break;
      case 'd':
        if (!strncmp(vname,"day",3)) {
          fvar=RtcTime.day_of_month;
          goto exit;
        }
        break;
      case 'e':
        if (!strncmp(vname,"epoch",5)) {
          fvar=UtcTime()-(uint32_t)EPOCH_OFFSET;
          goto exit;
        }
#ifdef USE_ENERGY_SENSOR
        if (!strncmp(vname,"enrg[",5)) {
          lp+=5;
          lp=GetNumericResult(lp,OPER_EQU,&fvar,0);
          while (*lp==' ') lp++;
          switch ((uint32_t)fvar) {
            case 0:
              fvar=Energy.total;
              break;
            case 1:
              fvar=Energy.voltage[0];
              break;
            case 2:
              fvar=Energy.voltage[1];
              break;
            case 3:
              fvar=Energy.voltage[2];
              break;
            case 4:
              fvar=Energy.current[0];
              break;
            case 5:
              fvar=Energy.current[1];
              break;
            case 6:
              fvar=Energy.current[2];
              break;
            case 7:
              fvar=Energy.active_power[0];
              break;
            case 8:
              fvar=Energy.active_power[1];
              break;
            case 9:
              fvar=Energy.active_power[2];
              break;

            default:
              fvar=99999;
              break;
          }
          len=0;
          lp++;
          goto exit;
        }
#endif //USE_ENERGY_SENSOR
        break;
      case 'f':
//#define DEBUG_FS
#ifdef USE_SCRIPT_FATFS
        if (!strncmp(vname,"fo(",3)) {
          lp+=3;
          char str[SCRIPT_MAXSSIZE];
          lp=GetStringResult(lp,OPER_EQU,str,0);
          while (*lp==' ') lp++;
          uint8_t mode=0;
          if ((*lp=='r') || (*lp=='w') || (*lp=='a')) {
            switch (*lp) {
              case 'r':
                mode=0;
                break;
              case 'w':
                mode=1;
                break;
              case 'a':
                mode=2;
                break;
            }
            lp++;
          } else {
            lp=GetNumericResult(lp,OPER_EQU,&fvar,0);
            mode=fvar;
          }
          fvar=-1;
          for (uint8_t cnt=0;cnt<SFS_MAX;cnt++) {
            if (!glob_script_mem.file_flags[cnt].is_open) {
              if (mode==0) {
#ifdef DEBUG_FS
                AddLog_P2(LOG_LEVEL_INFO,PSTR("open file for read %d"),cnt);
#endif
                glob_script_mem.files[cnt]=fsp->open(str,FILE_READ);
                if (glob_script_mem.files[cnt].isDirectory()) {
                  glob_script_mem.files[cnt].rewindDirectory();
                  glob_script_mem.file_flags[cnt].is_dir=1;
                } else {
                  glob_script_mem.file_flags[cnt].is_dir=0;
                }
              }
              else {
                if (mode==1) {
                  glob_script_mem.files[cnt]=fsp->open(str,FILE_WRITE);
#ifdef DEBUG_FS
                  AddLog_P2(LOG_LEVEL_INFO,PSTR("open file for write %d"),cnt);
#endif
                } else {
                  glob_script_mem.files[cnt]=fsp->open(str,FILE_APPEND);
#ifdef DEBUG_FS
                  AddLog_P2(LOG_LEVEL_INFO,PSTR("open file for append %d"),cnt);
#endif
                }
              }
              if (glob_script_mem.files[cnt]) {
                fvar=cnt;
                glob_script_mem.file_flags[cnt].is_open=1;
              } else {
                AddLog_P(LOG_LEVEL_INFO,PSTR("file open failed"));
              }
              break;
            }
          }
          lp++;
          len=0;
          goto exit;
        }
        if (!strncmp(vname,"fc(",3)) {
          lp+=3;
          lp=GetNumericResult(lp,OPER_EQU,&fvar,0);
          if (fvar>=0) {
            uint8_t ind=fvar;
            if (ind>=SFS_MAX) ind=SFS_MAX-1;
#ifdef DEBUG_FS
            AddLog_P2(LOG_LEVEL_INFO,PSTR("closing file %d"),ind);
#endif
            glob_script_mem.files[ind].close();
            glob_script_mem.file_flags[ind].is_open=0;
          }
          fvar=0;
          lp++;
          len=0;
          goto exit;
        }
        if (!strncmp(vname,"ff(",3)) {
          lp+=3;
          lp=GetNumericResult(lp,OPER_EQU,&fvar,0);
          uint8_t ind=fvar;
          if (ind>=SFS_MAX) ind=SFS_MAX-1;
          glob_script_mem.files[ind].flush();
          fvar=0;
          lp++;
          len=0;
          goto exit;
        }
        if (!strncmp(vname,"fw(",3)) {
          lp+=3;
          char str[SCRIPT_MAXSSIZE];
          lp=ForceStringVar(lp,str);
          while (*lp==' ') lp++;
          lp=GetNumericResult(lp,OPER_EQU,&fvar,0);
          uint8_t ind=fvar;
          if (ind>=SFS_MAX) ind=SFS_MAX-1;
          if (glob_script_mem.file_flags[ind].is_open) {
            fvar=glob_script_mem.files[ind].print(str);
          } else {
            fvar=0;
          }
          lp++;
          len=0;
          goto exit;
        }
        if (!strncmp(vname,"fr(",3)) {
          lp+=3;
          struct T_INDEX ind;
          uint8_t vtype;
          uint8_t sindex=0;
          lp=isvar(lp,&vtype,&ind,0,0,0);
          if (vtype!=VAR_NV) {
            // found variable as result
            if ((vtype&STYPE)==0) {
                  // error
                  fvar=0;
                  goto exit;
            } else {
              // string result
              sindex=glob_script_mem.type[ind.index].index;
            }
          } else {
              // error
              fvar=0;
              goto exit;
          }
          while (*lp==' ') lp++;
          lp=GetNumericResult(lp,OPER_EQU,&fvar,0);
          uint8_t find=fvar;
          if (find>=SFS_MAX) find=SFS_MAX-1;
          uint8_t index=0;
          char str[glob_script_mem.max_ssize+1];
          char *cp=str;
          if (glob_script_mem.file_flags[find].is_open) {
            if (glob_script_mem.file_flags[find].is_dir) {
              while (true) {
                File entry=glob_script_mem.files[find].openNextFile();
                if (entry) {
                  if (!reject((char*)entry.name())) {
                    char *ep=(char*)entry.name();
                    if (*ep=='/') ep++;
                    char *lcp = strrchr(ep,'/');
                    if (lcp) {
                      ep=lcp+1;
                    }
                    strcpy(str,ep);
                    entry.close();
                    break;
                  }
                } else {
                  *cp=0;
                  break;
                }
                entry.close();
              }
              index=strlen(str);
            } else {
              while (glob_script_mem.files[find].available()) {
                uint8_t buf[1];
                glob_script_mem.files[find].read(buf,1);
                if (buf[0]=='\t' || buf[0]==',' || buf[0]=='\n' || buf[0]=='\r') {
                  break;
                } else {
                  *cp++=buf[0];
                  index++;
                  if (index>=glob_script_mem.max_ssize-1) break;
                }
              }
              *cp=0;
            }
          } else {
            strcpy(str,"file error");
          }
          lp++;
          strlcpy(glob_script_mem.glob_snp+(sindex*glob_script_mem.max_ssize),str,glob_script_mem.max_ssize);
          fvar=index;
          len=0;
          goto exit;
        }
        if (!strncmp(vname,"fd(",3)) {
          lp+=3;
          char str[glob_script_mem.max_ssize+1];
          lp=GetStringResult(lp,OPER_EQU,str,0);
          fsp->remove(str);
          lp++;
          len=0;
          goto exit;
        }
#if defined(ESP32) && defined(USE_WEBCAM)
        if (!strncmp(vname,"fwp(",4)) {
          lp+=4;
          lp=GetNumericResult(lp,OPER_EQU,&fvar,0);
          while (*lp==' ') lp++;
          float fvar1;
          lp=GetNumericResult(lp,OPER_EQU,&fvar1,0);
          uint8_t ind=fvar1;
          if (ind>=SFS_MAX) ind=SFS_MAX-1;
          if (glob_script_mem.file_flags[ind].is_open) {
            uint8_t *buff;
            float maxps=WcGetPicstore(-1,0);
            if (fvar<1 || fvar>maxps) fvar=1;
            uint32_t len=WcGetPicstore(fvar-1, &buff);
            if (len) {
              //glob_script_mem.files[ind].seek(0,SeekEnd);
              fvar=glob_script_mem.files[ind].write(buff,len);
            } else {
              fvar=0;
            }
            //AddLog_P2(LOG_LEVEL_INFO, PSTR("picture save: %d"), len);
          } else {
            fvar=0;
          }
          lp++;
          len=0;
          goto exit;
        }
#endif
#ifdef USE_SCRIPT_FATFS_EXT
        if (!strncmp(vname,"fe(",3)) {
          lp+=3;
          char str[glob_script_mem.max_ssize+1];
          lp=GetStringResult(lp,OPER_EQU,str,0);
          // execute script
          File ef=fsp->open(str,FILE_READ);
          if (ef) {
            uint16_t fsiz=ef.size();
            if (fsiz<2048) {
              char *script=(char*)calloc(fsiz+16,1);
              if (script) {
                ef.read((uint8_t*)script,fsiz);
                execute_script(script);
                free(script);
                fvar=1;
              }
            }
            ef.close();
          }
          lp++;
          len=0;
          goto exit;
        }
        if (!strncmp(vname,"fmd(",4)) {
          lp+=4;
          char str[glob_script_mem.max_ssize+1];
          lp=GetStringResult(lp,OPER_EQU,str,0);
          fvar=fsp->mkdir(str);
          lp++;
          len=0;
          goto exit;
        }
        if (!strncmp(vname,"frd(",4)) {
          lp+=4;
          char str[glob_script_mem.max_ssize+1];
          lp=GetStringResult(lp,OPER_EQU,str,0);
          fvar=fsp->rmdir(str);
          lp++;
          len=0;
          goto exit;
        }
        if (!strncmp(vname,"fx(",3)) {
          lp+=3;
          char str[glob_script_mem.max_ssize+1];
          lp=GetStringResult(lp,OPER_EQU,str,0);
          if (fsp->exists(str)) fvar=1;
          else fvar=0;
          lp++;
          len=0;
          goto exit;
        }
#endif // USE_SCRIPT_FATFS_EXT
        if (!strncmp(vname,"fl1(",4) || !strncmp(vname,"fl2(",4) )  {
          uint8_t lknum=*(lp+2)&3;
          lp+=4;
          char str[glob_script_mem.max_ssize+1];
          lp=GetStringResult(lp,OPER_EQU,str,0);
          if (lknum<1 || lknum>2) lknum=1;
          strlcpy(glob_script_mem.flink[lknum-1],str,14);
          lp++;
          fvar=0;
          len=0;
          goto exit;
        }
        if (!strncmp(vname,"fsm",3)) {
          fvar=glob_script_mem.script_sd_found;
          //card_init();
          goto exit;
        }
#endif //USE_SCRIPT_FATFS
        if (!strncmp(vname,"freq",4)) {
#ifdef ESP32
          fvar=getCpuFrequencyMhz();
#else
          fvar=ESP.getCpuFreqMHz();
#endif
          goto exit;
        }
        break;
      case 'g':
        if (!strncmp(vname,"gtmp",4)) {
          fvar=global_temperature;
          goto exit;
        }
        if (!strncmp(vname,"ghum",4)) {
          fvar=global_humidity;
          goto exit;
        }
        if (!strncmp(vname,"gprs",4)) {
          fvar=global_pressure;
          goto exit;
        }
        if (!strncmp(vname,"gtopic",6)) {
          if (sp) strlcpy(sp,SettingsText(SET_MQTT_GRP_TOPIC),glob_script_mem.max_ssize);
          goto strexit;
        }
        break;
      case 'h':
        if (!strncmp(vname,"hours",5)) {
          fvar=RtcTime.hour;
          goto exit;
        }
        if (!strncmp(vname,"heap",4)) {
          fvar=ESP_getFreeHeap();
          goto exit;
        }
        if (!strncmp(vname,"hn(",3)) {
          lp=GetNumericResult(lp+3,OPER_EQU,&fvar,0);
          if (fvar<0 || fvar>255) fvar=0;
          lp++;
          len=0;
          if (sp) {
            sprintf(sp,"%02x",(uint8_t)fvar);
          }
          goto strexit;
        }
        if (!strncmp(vname,"hx(",3)) {
          lp=GetNumericResult(lp+3,OPER_EQU,&fvar,0);
          lp++;
          len=0;
          if (sp) {
            sprintf(sp,"%08x",(uint32_t)fvar);
          }
          goto strexit;
        }
#ifdef USE_LIGHT
//#ifdef USE_WS2812
        if (!strncmp(vname,"hsvrgb(",7)) {
          lp=GetNumericResult(lp+7,OPER_EQU,&fvar,0);
          if (fvar<0 || fvar>360) fvar=0;
          SCRIPT_SKIP_SPACES
          // arg2
          float fvar2;
          lp=GetNumericResult(lp,OPER_EQU,&fvar2,0);
          if (fvar2<0 || fvar2>100) fvar2=0;
          SCRIPT_SKIP_SPACES
          // arg3
          float fvar3;
          lp=GetNumericResult(lp,OPER_EQU,&fvar3,0);
          if (fvar3<0 || fvar3>100) fvar3=0;

          fvar=HSVToRGB(fvar,fvar2,fvar3);

          lp++;
          len=0;
          goto exit;
        }
//#endif
#endif
        break;
      case 'i':
        if (!strncmp(vname,"int(",4)) {
          lp=GetNumericResult(lp+4,OPER_EQU,&fvar,0);
          fvar=floor(fvar);
          lp++;
          len=0;
          goto exit;
        }
        break;
      case 'l':
        if (!strncmp(vname,"lip",3)) {
          if (sp) strlcpy(sp,(const char*)WiFi.localIP().toString().c_str(),glob_script_mem.max_ssize);
          goto strexit;
        }
        if (!strncmp(vname,"loglvl",6)) {
          fvar=glob_script_mem.script_loglevel;
          tind->index=SCRIPT_LOGLEVEL;
  exit_settable:
          if (fp) *fp=fvar;
          *vtype=NTYPE;
          tind->bits.settable=1;
          tind->bits.is_string=0;
          return lp+len;
        }
        break;
      case 'm':
        if (!strncmp(vname,"med(",4)) {
          float fvar1;
          lp=GetNumericResult(lp+4,OPER_EQU,&fvar1,0);
          SCRIPT_SKIP_SPACES
          // arg2
          float fvar2;
          lp=GetNumericResult(lp,OPER_EQU,&fvar2,0);
          fvar=DoMedian5(fvar1,fvar2);
          lp++;
          len=0;
          goto exit;
        }
#ifdef USE_ANGLE_FUNC
        if (!strncmp(vname,"mpt(",4)) {
          lp=GetNumericResult(lp+4,OPER_EQU,&fvar,0);
          fvar=MeasurePulseTime(fvar);
          lp++;
          len=0;
          goto exit;
        }
#endif
        if (!strncmp(vname,"micros",6)) {
          fvar=micros();
          goto exit;
        }
        if (!strncmp(vname,"millis",6)) {
          fvar=millis();
          goto exit;
        }
        if (!strncmp(vname,"mins",4)) {
          fvar=RtcTime.minute;
          goto exit;
        }
        if (!strncmp(vname,"month",5)) {
          fvar=RtcTime.month;
          goto exit;
        }
        if (!strncmp(vname,"mqttc",5)) {
          if (rules_flag.mqtt_connected) {
            rules_flag.mqtt_connected=0;
            fvar=1;
          }
          goto exit;
        }
        if (!strncmp(vname,"mqttd",5)) {
          if (rules_flag.mqtt_disconnected) {
            rules_flag.mqtt_disconnected=0;
            fvar=1;
          }
          goto exit;
        }
        if (!strncmp(vname,"mqtts",5)) {
          fvar=!global_state.mqtt_down;
          goto exit;
        }
        if (!strncmp(vname,"mp(",3)) {
          lp+=3;
          float fvar1;
          lp=GetNumericResult(lp,OPER_EQU,&fvar1,0);
          SCRIPT_SKIP_SPACES
          while (*lp!=')') {
            char *opp=lp;
            lp++;
            float fvar2;
            lp=GetNumericResult(lp,OPER_EQU,&fvar2,0);
            SCRIPT_SKIP_SPACES
            fvar=fvar1;
            if ((*opp=='<' && fvar1<fvar2) ||
                (*opp=='>' && fvar1>fvar2) ||
                (*opp=='=' && fvar1==fvar2))
            {
              if (*lp!='<' && *lp!='>' && *lp!='=' && *lp!=')' && *lp!=SCRIPT_EOL) {
                float fvar3;
                lp=GetNumericResult(lp,OPER_EQU,&fvar3,0);
                SCRIPT_SKIP_SPACES
                fvar=fvar3;
              } else {
                fvar=fvar2;
              }
              break;
            }
            while (*lp!='<' && *lp!='>' && *lp!='=' && *lp!=')' && *lp!=SCRIPT_EOL) lp++;
          }
          len=0;
          goto exit;
        }
        break;
      case 'p':
        if (!strncmp(vname,"pin[",4)) {
          // raw pin level
          GetNumericResult(vname+4,OPER_EQU,&fvar,0);
          fvar=digitalRead((uint8_t)fvar);
          // skip ] bracket
          len++;
          goto exit;
        }
        if (!strncmp(vname,"pn[",3)) {
          GetNumericResult(vname+3,OPER_EQU,&fvar,0);
          fvar=Pin(fvar);
          // skip ] bracket
          len++;
          goto exit;
        }
        if (!strncmp(vname,"pn[",3)) {
          GetNumericResult(vname+3,OPER_EQU,&fvar,0);
//          fvar=pin_gpio[(uint8_t)fvar];
          fvar=Pin(fvar);
          // skip ] bracket
          len++;
          goto exit;
        }
        if (!strncmp(vname,"pd[",3)) {
          GetNumericResult(vname+3,OPER_EQU,&fvar,0);
          uint8_t gpiopin=fvar;
/*
          for (uint8_t i=0;i<GPIO_SENSOR_END;i++) {
//            if (pin_gpio[i]==gpiopin) {
            if (Pin(i)==gpiopin) {
              fvar=i;
              // skip ] bracket
              len++;
              goto exit;
            }
          }
*/
          if ((gpiopin < ARRAY_SIZE(gpio_pin)) && (gpio_pin[gpiopin] > 0)) {
            fvar = gpio_pin[gpiopin];
            // skip ] bracket
            len++;
            goto exit;
          }
          fvar=999;
          goto exit;
        }
#ifdef ESP32
        if (!strncmp(vname,"pheap",5)) {
          fvar=ESP.getFreePsram();
          goto exit;
        }
#endif
        if (!strncmp(vname,"prefix1",7)) {
          if (sp) strlcpy(sp,SettingsText(SET_MQTTPREFIX1),glob_script_mem.max_ssize);
          goto strexit;
        }
        if (!strncmp(vname,"prefix2",7)) {
          if (sp) strlcpy(sp,SettingsText(SET_MQTTPREFIX2),glob_script_mem.max_ssize);
          goto strexit;
        }
        if (!strncmp(vname,"prefix3",7)) {
          if (sp) strlcpy(sp,SettingsText(SET_MQTTPREFIX3),glob_script_mem.max_ssize);
          goto strexit;
        }
        if (!strncmp(vname,"pow(",4)) {
          // arg1
          float fvar1;
          lp=GetNumericResult(lp+4,OPER_EQU,&fvar1,0);
          SCRIPT_SKIP_SPACES
          // arg2
          float fvar2;
          lp=GetNumericResult(lp,OPER_EQU,&fvar2,0);
          lp++;
          //fvar=pow(fvar1,fvar2);
          fvar=FastPrecisePowf(fvar1,fvar2);
          len=0;
          goto exit;
        }
        if (!strncmp(vname,"pwr[",4)) {
          GetNumericResult(vname+4,OPER_EQU,&fvar,0);
          uint8_t index=fvar;
          if (index<=devices_present) {
            fvar=bitRead(power,index-1);
          } else {
            fvar=-1;
          }
          len+=1;
          goto exit;
        }
        if (!strncmp(vname,"pc[",3)) {
          GetNumericResult(vname+3,OPER_EQU,&fvar,0);
          uint8_t index=fvar;
          if (index<1 || index>MAX_COUNTERS) index=1;
          fvar=RtcSettings.pulse_counter[index-1];
          len+=1;
          goto exit;
        }
        break;

      case 'r':
        if (!strncmp(vname,"ram",3)) {
          fvar=glob_script_mem.script_mem_size+(glob_script_mem.script_size)+(PMEM_SIZE);
          goto exit;
        }
        if (!strncmp(vname,"rnd(",4)) {
          // tasmota switch state
          GetNumericResult(vname+4,OPER_EQU,&fvar,0);
          if (fvar<0) {
            randomSeed(-fvar);
            fvar=0;
          } else {
            fvar=random(fvar);
          }
          // skip ] bracket
          len++;
          goto exit;
        }
        break;
      case 's':
        if (!strncmp(vname,"secs",4)) {
          fvar=RtcTime.second;
          goto exit;
        }
        if (!strncmp(vname,"sw[",3)) {
          // tasmota switch state
          GetNumericResult(vname+3,OPER_EQU,&fvar,0);
          fvar=SwitchLastState((uint32_t)fvar);
          // skip ] bracket
          len++;
          goto exit;
        }
        if (!strncmp(vname,"stack",5)) {
          fvar=GetStack();
          goto exit;
        }
        if (!strncmp(vname,"slen",4)) {
          fvar=strlen(glob_script_mem.script_ram);
          goto exit;
        }
        if (!strncmp(vname,"sl(",3)) {
          lp+=3;
          char str[SCRIPT_MAXSSIZE];
          lp=GetStringResult(lp,OPER_EQU,str,0);
          lp++;
          len=0;
          fvar=strlen(str);
          goto exit;
        }
        if (!strncmp(vname,"sb(",3)) {
          lp+=3;
          char str[SCRIPT_MAXSSIZE];
          lp=GetStringResult(lp,OPER_EQU,str,0);
          SCRIPT_SKIP_SPACES
          float fvar1;
          lp=GetNumericResult(lp,OPER_EQU,&fvar1,0);
          SCRIPT_SKIP_SPACES
          float fvar2;
          lp=GetNumericResult(lp,OPER_EQU,&fvar2,0);
          lp++;
          len=0;
          if (fvar1<0) {
            fvar1=strlen(str)+fvar1;
          }
          memcpy(sp,&str[(uint8_t)fvar1],(uint8_t)fvar2);
          sp[(uint8_t)fvar2] = '\0';
          goto strexit;
        }
        if (!strncmp(vname,"st(",3)) {
          lp+=3;
          char str[SCRIPT_MAXSSIZE];
          lp=GetStringResult(lp,OPER_EQU,str,0);
          while (*lp==' ') lp++;
          char token[2];
          token[0]=*lp++;
          token[1]=0;
          while (*lp==' ') lp++;
          lp=GetNumericResult(lp,OPER_EQU,&fvar,0);
          // skip ) bracket
          lp++;
          len=0;
          if (sp) {
            // get stringtoken
            char *st=strtok(str,token);
            if (!st) {
              *sp=0;
            } else {
              for (uint8_t cnt=1; cnt<=fvar; cnt++) {
                if (cnt==fvar) {
                  strcpy(sp,st);
                  break;
                }
                st=strtok(NULL,token);
                if (!st) {
                  *sp=0;
                  break;
                }
              }
            }
          }
          goto strexit;
        }
        if (!strncmp(vname,"s(",2)) {
          lp+=2;
          lp=GetNumericResult(lp,OPER_EQU,&fvar,0);
          char str[glob_script_mem.max_ssize+1];
          dtostrfd(fvar,glob_script_mem.script_dprec,str);
          if (sp) strlcpy(sp,str,glob_script_mem.max_ssize);
          lp++;
          len=0;
          goto strexit;
        }
#ifdef ESP32
        if (!strncmp(vname,"sf(",3)) {
          lp+=2;
          lp=GetNumericResult(lp,OPER_EQU,&fvar,0);
          if (fvar<80) fvar=80;
          if (fvar>240) fvar=240;
          setCpuFrequencyMhz(fvar);
          fvar=getCpuFrequencyMhz();
          lp++;
          len=0;
          goto exit;
        }
#endif
#if defined(USE_TIMERS) && defined(USE_SUNRISE)
        if (!strncmp(vname,"sunrise",7)) {
          fvar=SunMinutes(0);
          goto exit;
        }
        if (!strncmp(vname,"sunset",6)) {
          fvar=SunMinutes(1);
          goto exit;
        }
#endif

#ifdef USE_SHUTTER
        if (!strncmp(vname,"sht[",4)) {
          GetNumericResult(vname+4,OPER_EQU,&fvar,0);
          uint8_t index=fvar;
          if (index<=shutters_present) {
            fvar=Settings.shutter_position[index-1];
          } else {
            fvar=-1;
          }
          len+=1;
          goto exit;
        }
#endif
#ifdef USE_ANGLE_FUNC
        if (!strncmp(vname,"sin(",4)) {
          lp+=4;
          lp=GetNumericResult(lp,OPER_EQU,&fvar,0);
          fvar=sinf(fvar);
          lp++;
          len=0;
          goto exit;
        }
        if (!strncmp(vname,"sqrt(",5)) {
          lp+=5;
          lp=GetNumericResult(lp,OPER_EQU,&fvar,0);
          fvar=sqrtf(fvar);
          lp++;
          len=0;
          goto exit;
        }
#endif

#if defined(USE_SML_M) && defined (USE_SML_SCRIPT_CMD)
        if (!strncmp(vname,"sml[",4)) {
          lp+=4;
          lp=GetNumericResult(lp,OPER_EQU,&fvar,0);
          SCRIPT_SKIP_SPACES
          fvar=SML_GetVal(fvar);
          lp++;
          len=0;
          goto exit;
        }
        if (!strncmp(vname,"sml(",4)) {
          lp+=4;
          float fvar1;
          lp=GetNumericResult(lp,OPER_EQU,&fvar1,0);
          SCRIPT_SKIP_SPACES
          float fvar2;
          lp=GetNumericResult(lp,OPER_EQU,&fvar2,0);
          SCRIPT_SKIP_SPACES
          if (fvar2==0) {
            float fvar3;
            lp=GetNumericResult(lp,OPER_EQU,&fvar3,0);
            fvar=SML_SetBaud(fvar1,fvar3);
          } else if (fvar2==1) {
            char str[SCRIPT_MAXSSIZE];
            lp=GetStringResult(lp,OPER_EQU,str,0);
            fvar=SML_Write(fvar1,str);
          } else {
#ifdef ED300L
            fvar=SML_Status(fvar1);
#else
            fvar=0;
#endif
          }
          lp++;
          len=0;
          goto exit;
        }
#endif
        break;
      case 't':
        if (!strncmp(vname,"time",4)) {
          fvar=MinutesPastMidnight();
          goto exit;
        }
        if (!strncmp(vname,"tper",4)) {
          fvar=Settings.tele_period;
          tind->index=SCRIPT_TELEPERIOD;
          goto exit_settable;
        }
        if (!strncmp(vname,"tinit",5)) {
          if (rules_flag.time_init) {
            rules_flag.time_init=0;
            fvar=1;
          }
          goto exit;
        }
        if (!strncmp(vname,"tset",4)) {
          if (rules_flag.time_set) {
            rules_flag.time_set=0;
            fvar=1;
          }
          goto exit;
        }
        if (!strncmp(vname,"tstamp",6)) {
          if (sp) strlcpy(sp,GetDateAndTime(DT_LOCAL).c_str(),glob_script_mem.max_ssize);
          goto strexit;
        }
        if (!strncmp(vname,"topic",5)) {
          if (sp) strlcpy(sp,SettingsText(SET_MQTT_TOPIC),glob_script_mem.max_ssize);
          goto strexit;
        }
#ifdef USE_DISPLAY
#ifdef USE_TOUCH_BUTTONS
        if (!strncmp(vname,"tbut[",5)) {
          GetNumericResult(vname+5,OPER_EQU,&fvar,0);
          uint8_t index=fvar;
          if (index<1 || index>MAXBUTTONS) index=1;
          index--;
          if (buttons[index]) {
            fvar=buttons[index]->vpower&0x80;
          } else {
            fvar=-1;
          }
          len+=1;
          goto exit;
        }

#endif
#endif
        break;
      case 'u':
        if (!strncmp(vname,"uptime",6)) {
          fvar=MinutesUptime();
          goto exit;
        }
        if (!strncmp(vname,"upsecs",6)) {
          fvar=uptime;
          goto exit;
        }
        if (!strncmp(vname,"upd[",4)) {
          // var was updated
          struct T_INDEX ind;
          uint8_t vtype;
          isvar(vname+4,&vtype,&ind,0,0,0);
          if (!ind.bits.constant) {
            if (!ind.bits.changed) {
              fvar=0;
              len++;
              goto exit;
            } else {
              glob_script_mem.type[ind.index].bits.changed=0;
              fvar=1;
              len++;
              goto exit;
            }
          }
          goto notfound;
        }
        break;

      case 'w':
#if defined(ESP32) && defined(USE_WEBCAM)
        if (!strncmp(vname,"wc(",3)) {
          lp+=3;
          float fvar1;
          lp=GetNumericResult(lp,OPER_EQU,&fvar1,0);
          SCRIPT_SKIP_SPACES
          switch ((uint32)fvar1) {
            case 0:
              { float fvar2;
                lp=GetNumericResult(lp,OPER_EQU,&fvar2,0);
                fvar=WcSetup(fvar2);
              }
              break;
            case 1:
              { float fvar2;
                lp=GetNumericResult(lp,OPER_EQU,&fvar2,0);
                fvar=WcGetFrame(fvar2);
              }
              break;
            case 2:
              { float fvar2,fvar3;
                lp=GetNumericResult(lp,OPER_EQU,&fvar2,0);
                SCRIPT_SKIP_SPACES
                lp=GetNumericResult(lp,OPER_EQU,&fvar3,0);
                fvar=WcSetOptions(fvar2,fvar3);
              }
              break;
            case 3:
              fvar=WcGetWidth();
              break;
            case 4:
              fvar=WcGetHeight();
              break;
            case 5:
              { float fvar2;
                lp=GetNumericResult(lp,OPER_EQU,&fvar2,0);
                fvar=WcSetStreamserver(fvar2);
              }
              break;
            case 6:
              { float fvar2;
                lp=GetNumericResult(lp,OPER_EQU,&fvar2,0);
                fvar=WcSetMotionDetect(fvar2);
              }
              break;
#ifdef USE_FACE_DETECT
            case 7:
              { float fvar2;
                lp=GetNumericResult(lp,OPER_EQU,&fvar2,0);
                fvar=WcSetFaceDetect(fvar2);
              }
              break;
#endif
            default:
              fvar=0;
          }
          lp++;
          len=0;
          goto exit;
        }
#endif //ESP32, USE_WEBCAM
        if (!strncmp(vname,"wday",4)) {
          fvar=RtcTime.day_of_week;
          goto exit;
        }
        if (!strncmp(vname,"wific",5)) {
          if (rules_flag.wifi_connected) {
            rules_flag.wifi_connected=0;
            fvar=1;
          }
          goto exit;
        }
        if (!strncmp(vname,"wifid",5)) {
          if (rules_flag.wifi_disconnected) {
            rules_flag.wifi_disconnected=0;
            fvar=1;
          }
          goto exit;
        }
        if (!strncmp(vname,"wifis",5)) {
          fvar=!global_state.wifi_down;
          goto exit;
        }
        break;
      case 'y':
        if (!strncmp(vname,"year",4)) {
          fvar=RtcTime.year;
          goto exit;
        }
        break;
      default:
        break;
    }
    // nothing valid found
notfound:
    if (fp) *fp=0;
    *vtype=VAR_NV;
    tind->index=VAR_NV;
    glob_script_mem.var_not_found=1;
    return lp;
    // return constant numbers
exit:
    if (fp) *fp=fvar;
    *vtype=NUM_RES;
    tind->bits.constant=1;
    tind->bits.is_string=0;
    return lp+len;
    // return constant strings
strexit:
    *vtype=STYPE;
    tind->bits.constant=1;
    tind->bits.is_string=1;
    return lp+len;
}



char *getop(char *lp, uint8_t *operand) {
    switch (*lp) {
        case '=':
            if (*(lp+1)=='=') {
                *operand=OPER_EQUEQU;
                return lp+2;
            } else {
                *operand=OPER_EQU;
                return lp+1;
            }
            break;
        case '+':
            if (*(lp+1)=='=') {
                *operand=OPER_PLSEQU;
                return lp+2;
            } else {
                *operand=OPER_PLS;
                return lp+1;
            }
            break;
        case '-':
            if (*(lp+1)=='=') {
                *operand=OPER_MINEQU;
                return lp+2;
            } else {
                *operand=OPER_MIN;
                return lp+1;
            }
            break;
        case '*':
            if (*(lp+1)=='=') {
                *operand=OPER_MULEQU;
                return lp+2;
            } else {
                *operand=OPER_MUL;
                return lp+1;
            }
            break;
        case '/':
            if (*(lp+1)=='=') {
                *operand=OPER_DIVEQU;
                return lp+2;
            } else {
                *operand=OPER_DIV;
                return lp+1;
            }
            break;
        case '!':
            if (*(lp+1)=='=') {
                *operand=OPER_NOTEQU;
                return lp+2;
            }
            break;
        case '>':
            if (*(lp+1)=='=') {
                *operand=OPER_GRTEQU;
                return lp+2;
            } else {
                *operand=OPER_GRT;
                return lp+1;

            }
            break;
        case '<':
            if (*(lp+1)=='=') {
                *operand=OPER_LOWEQU;
                return lp+2;
            } else {
                *operand=OPER_LOW;
                return lp+1;
            }
            break;
        case '%':
            if (*(lp+1)=='=') {
                *operand=OPER_PERCEQU;
                return lp+2;
            } else {
                *operand=OPER_PERC;
                return lp+1;
            }
            break;
        case '^':
            if (*(lp+1)=='=') {
                *operand=OPER_XOREQU;
                return lp+2;
            } else {
                *operand=OPER_XOR;
                return lp+1;
            }
            break;
        case '&':
            if (*(lp+1)=='=') {
                *operand=OPER_ANDEQU;
                return lp+2;
            } else {
                *operand=OPER_AND;
                return lp+1;
            }
            break;
        case '|':
            if (*(lp+1)=='=') {
                *operand=OPER_OREQU;
                return lp+2;
            } else {
                *operand=OPER_OR;
                return lp+1;
            }
            break;
    }
    *operand=0;
    return lp;
}


#ifdef ESP8266
#if defined(ARDUINO_ESP8266_RELEASE_2_3_0) || defined(ARDUINO_ESP8266_RELEASE_2_4_0) || defined(ARDUINO_ESP8266_RELEASE_2_4_1)
// All version before core 2.4.2
// https://github.com/esp8266/Arduino/issues/2557
extern "C" {
#include <cont.h>
  extern cont_t g_cont;
}
uint16_t GetStack(void) {
  register uint32_t *sp asm("a1");
  return (4 * (sp - g_cont.stack));
}

#else
extern "C" {
#include <cont.h>
  extern cont_t* g_pcont;
}
uint16_t GetStack(void) {
  register uint32_t *sp asm("a1");
  return (4 * (sp - g_pcont->stack));
}
#endif
#else
uint16_t GetStack(void) {
  register uint8_t *sp asm("a1");
  return (sp - pxTaskGetStackStart(NULL));
}
#endif

char *GetStringResult(char *lp,uint8_t lastop,char *cp,JsonObject *jo) {
  uint8_t operand=0;
  uint8_t vtype;
  char *slp;
  struct T_INDEX ind;
  char str[SCRIPT_MAXSSIZE],str1[SCRIPT_MAXSSIZE];
  while (1) {
    lp=isvar(lp,&vtype,&ind,0,str1,jo);
    if (vtype!=STR_RES && !(vtype&STYPE)) {
      // numeric type
      glob_script_mem.glob_error=1;
      return lp;
    }
    switch (lastop) {
        case OPER_EQU:
          strlcpy(str,str1,sizeof(str));
          break;
        case OPER_PLS:
          strncat(str,str1,sizeof(str));
          break;
    }
    slp=lp;
    lp=getop(lp,&operand);
    switch (operand) {
      case OPER_EQUEQU:
      case OPER_NOTEQU:
      case OPER_LOW:
      case OPER_LOWEQU:
      case OPER_GRT:
      case OPER_GRTEQU:
          lp=slp;
          strcpy(cp,str);
          return lp;
          break;
      default:
          break;
    }
    lastop=operand;
    if (!operand) {
      strcpy(cp,str);
      return lp;
    }
  }
}

char *GetNumericResult(char *lp,uint8_t lastop,float *fp,JsonObject *jo) {
uint8_t operand=0;
float fvar1,fvar;
char *slp;
uint8_t vtype;
struct T_INDEX ind;
    while (1) {
        // get 1. value
        if (*lp=='(') {
            lp++;
            lp=GetNumericResult(lp,OPER_EQU,&fvar1,jo);
            lp++;
            //if (*lp==')') lp++;
        } else {
            lp=isvar(lp,&vtype,&ind,&fvar1,0,jo);
            if (vtype!=NUM_RES && vtype&STYPE) {
              // string type
              glob_script_mem.glob_error=1;
            }
        }
        switch (lastop) {
            case OPER_EQU:
                fvar=fvar1;
                break;
            case OPER_PLS:
                fvar+=fvar1;
                break;
            case OPER_MIN:
                fvar-=fvar1;
                break;
            case OPER_MUL:
                fvar*=fvar1;
                break;
            case OPER_DIV:
                fvar/=fvar1;
                break;
            case OPER_PERC:
                fvar=fmodf(fvar,fvar1);
                break;
            case OPER_XOR:
                fvar=(uint32_t)fvar^(uint32_t)fvar1;
                break;
            case OPER_AND:
                fvar=(uint32_t)fvar&(uint32_t)fvar1;
                break;
            case OPER_OR:
                fvar=(uint32_t)fvar|(uint32_t)fvar1;
                break;
            default:
                break;

        }
        slp=lp;
        lp=getop(lp,&operand);
        switch (operand) {
            case OPER_EQUEQU:
            case OPER_NOTEQU:
            case OPER_LOW:
            case OPER_LOWEQU:
            case OPER_GRT:
            case OPER_GRTEQU:
                lp=slp;
                *fp=fvar;
                return lp;
                break;
            default:
                break;
        }
        lastop=operand;
        if (!operand) {
            *fp=fvar;
            return lp;
        }
    }
}


char *ForceStringVar(char *lp,char *dstr) {
  float fvar;
  char *slp=lp;
  glob_script_mem.glob_error=0;
  lp=GetStringResult(lp,OPER_EQU,dstr,0);
  if (glob_script_mem.glob_error) {
    // mismatch
    lp=GetNumericResult(slp,OPER_EQU,&fvar,0);
    dtostrfd(fvar,6,dstr);
    glob_script_mem.glob_error=0;
  }
  return lp;
}

// replace vars in cmd %var%
void Replace_Cmd_Vars(char *srcbuf,uint32_t srcsize, char *dstbuf,uint32_t dstsize) {
    char *cp;
    uint16_t count;
    uint8_t vtype;
    uint8_t dprec=glob_script_mem.script_dprec;
    float fvar;
    cp=srcbuf;
    struct T_INDEX ind;
    char string[SCRIPT_MAXSSIZE];
    dstsize-=2;
    for (count=0;count<dstsize;count++) {
        if (srcsize && (*cp==SCRIPT_EOL)) break;
        if (*cp=='%') {
            cp++;
            if (*cp=='%') {
              dstbuf[count]=*cp++;
            } else {
              if (isdigit(*cp)) {
                dprec=*cp&0xf;
                cp++;
              } else {
                dprec=glob_script_mem.script_dprec;
              }
              cp=isvar(cp,&vtype,&ind,&fvar,string,0);
              if (vtype!=VAR_NV) {
                // found variable as result
                if (vtype==NUM_RES || (vtype&STYPE)==0) {
                    // numeric result
                    dtostrfd(fvar,dprec,string);
                } else {
                    // string result
                }
                uint8_t slen=strlen(string);
                if (count+slen<dstsize-1) {
                  strcpy(&dstbuf[count],string);
                  count+=slen-1;
                }
                cp++;
              } else {
                strcpy(&dstbuf[count],"???");
                count+=2;
                while (*cp!='%') {
                  if (*cp==0 || *cp==SCRIPT_EOL) {
                    dstbuf[count+1]=0;
                    return;
                  }
                  cp++;
                }
                cp++;
              }
            }
        } else {
            if (*cp=='\\') {
              cp++;
              if (*cp=='n') {
                dstbuf[count]='\n';
              } else if (*cp=='r') {
                dstbuf[count]='\r';
              } else if (*cp=='\\') {
                dstbuf[count]='\\';
              } else {
                dstbuf[count]=*cp;
              }
            } else {
              dstbuf[count]=*cp;
            }
            if (*cp==0) {
                break;
            }
            cp++;
        }
    }
    dstbuf[count]=0;
}

void toLog(const char *str) {
  if (!str) return;
  AddLog_P(LOG_LEVEL_INFO, str);
}


void toLogN(const char *cp,uint8_t len) {
  if (!cp) return;
  char str[32];
  if (len>=sizeof(str)) len=len>=sizeof(str);
  strlcpy(str,cp,len);
  toSLog(str);
}

void toLogEOL(const char *s1,const char *str) {
  if (!str) return;
  uint8_t index=0;
  char *cp=log_data;
  strcpy(cp,s1);
  cp+=strlen(s1);
  while (*str) {
    if (*str==SCRIPT_EOL) break;
    *cp++=*str++;
  }
  *cp=0;
  AddLog(LOG_LEVEL_INFO);
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

char *Evaluate_expression(char *lp,uint8_t and_or, uint8_t *result,JsonObject  *jo) {
  float fvar,*dfvar,fvar1;
  uint8_t numeric;
  struct T_INDEX ind;
  uint8_t vtype=0,lastop;
  uint8_t res=0;
  char *llp=lp;
  char *slp;

  SCRIPT_SKIP_SPACES
  if (*lp=='(') {
    uint8_t res=0;
    uint8_t xand_or=0;
    lp++;

loop:
    SCRIPT_SKIP_SPACES
    lp=Evaluate_expression(lp,xand_or,&res,jo);
    if (*lp==')') {
      lp++;
      goto exit0;
    }
    // check for next and or
    SCRIPT_SKIP_SPACES
    if (!strncmp(lp,"or",2)) {
      lp+=2;
      xand_or=1;
      goto loop;
    } else if (!strncmp(lp,"and",3)) {
      lp+=3;
      xand_or=2;
      goto loop;
    }
exit0:
    if (!and_or) {
      *result=res;
    } else if (and_or==1) {
      *result|=res;
    } else {
      *result&=res;
    }
    goto exit10;
  }

  llp=lp;
  // compare
  dfvar=&fvar;
  glob_script_mem.glob_error=0;
  slp=lp;
  numeric=1;
  lp=GetNumericResult(lp,OPER_EQU,dfvar,0);
  if (glob_script_mem.glob_error==1) {
    // was string, not number
	  char cmpstr[SCRIPT_MAXSSIZE];
    lp=slp;
    numeric=0;
    // get the string
    lp=isvar(lp,&vtype,&ind,0,cmpstr,0);
	  lp=getop(lp,&lastop);
    // compare string
    char str[SCRIPT_MAXSSIZE];
    lp=GetStringResult(lp,OPER_EQU,str,jo);
    if (lastop==OPER_EQUEQU || lastop==OPER_NOTEQU) {
      res=strcmp(cmpstr,str);
      if (lastop==OPER_EQUEQU) res=!res;
      goto exit;
    }

  } else {
    // numeric
    // evaluate operand
    lp=getop(lp,&lastop);
    lp=GetNumericResult(lp,OPER_EQU,&fvar1,jo);
    switch (lastop) {
      case OPER_EQUEQU:
          res=(*dfvar==fvar1);
          break;
      case OPER_NOTEQU:
          res=(*dfvar!=fvar1);
          break;
      case OPER_LOW:
          res=(*dfvar<fvar1);
          break;
      case OPER_LOWEQU:
          res=(*dfvar<=fvar1);
          break;
      case OPER_GRT:
          res=(*dfvar>fvar1);
          break;
      case OPER_GRTEQU:
          res=(*dfvar>=fvar1);
          break;
      default:
          // error
          break;
    }

exit:
    if (!and_or) {
      *result=res;
    } else if (and_or==1) {
      *result|=res;
    } else {
      *result&=res;
    }
  }


exit10:
#if SCRIPT_DEBUG>0
  char tbuff[128];
  sprintf(tbuff,"p1=%d,p2=%d,cmpres=%d,and_or=%d line: ",(int32_t)*dfvar,(int32_t)fvar1,*result,and_or);
  toLogEOL(tbuff,llp);
#endif
  return lp;
}

#ifdef ESP32

TimerHandle_t beep_th;
void StopBeep( TimerHandle_t xTimer );

void StopBeep( TimerHandle_t xTimer ) {
  ledcWriteTone(7,0);
  xTimerStop(xTimer, 0);
}

void esp32_beep(int32_t freq ,uint32_t len) {
  if (freq<0) {
    ledcSetup(7,500,10);
    ledcAttachPin(-freq,7);
    ledcWriteTone(7,0);
    if (!beep_th) {
      beep_th = xTimerCreate("beep",100,pdFALSE,( void * ) 0,StopBeep);
    }
  } else {
    if (!beep_th) return;
    if (!freq) {
      ledcWriteTone(7,0);
      xTimerStop(beep_th, 10);
      return;
    }
    if (len < 10) return;
    if (xTimerIsTimerActive(beep_th)) return;
    ledcWriteTone(7,freq);
    uint32_t ticks = pdMS_TO_TICKS(len);
    xTimerChangePeriod( beep_th, ticks, 10);
  }
}
#endif // ESP32

//#define IFTHEN_DEBUG

#define IF_NEST 8
// execute section of scripter
int16_t Run_Scripter(const char *type, int8_t tlen, char *js) {

    if (tasm_cmd_activ && tlen>0) return 0;

    uint8_t vtype=0,sindex,xflg,floop=0,globvindex,fromscriptcmd=0;
    char *lp_next;
    int8_t globaindex;
    struct T_INDEX ind;
    uint8_t operand,lastop,numeric=1,if_state[IF_NEST],if_exe[IF_NEST],if_result[IF_NEST],and_or,ifstck=0;
    if_state[ifstck]=0;
    if_result[ifstck]=0;
    if_exe[ifstck]=1;
    char cmpstr[SCRIPT_MAXSSIZE];
    uint8_t check=0;
    if (tlen<0) {
      tlen=abs(tlen);
      check=1;
    }

    float *dfvar,*cv_count,cv_max,cv_inc;
    char *cv_ptr;
    float fvar=0,fvar1,sysvar,swvar;
    uint8_t section=0,sysv_type=0,swflg=0;

    if (!glob_script_mem.scriptptr) {
      return -99;
    }

    JsonObject  *jo=0;
    DynamicJsonBuffer jsonBuffer; // on heap
    JsonObject &jobj=jsonBuffer.parseObject(js);
    if (js) {
      jo=&jobj;
    } else {
      jo=0;
    }

    char *lp=glob_script_mem.scriptptr;

    while (1) {
        // check line
        // skip leading spaces
        startline:
        SCRIPT_SKIP_SPACES
        // skip empty line
        SCRIPT_SKIP_EOL
        // skip comment
        if (*lp==';') goto next_line;
        if (!*lp) break;

        if (section) {
            // we are in section
            if (*lp=='>') {
                return 0;
            }
            if (*lp=='#') {
                return 0;
            }
            glob_script_mem.var_not_found=0;

//#if SCRIPT_DEBUG>0
#ifdef IFTHEN_DEBUG
            char tbuff[128];
            sprintf(tbuff,"stack=%d,exe=%d,state=%d,cmpres=%d line: ",ifstck,if_exe[ifstck],if_state[ifstck],if_result[ifstck]);
            toLogEOL(tbuff,lp);
#endif

//if (if_state[s_ifstck]==3 && if_result[s_ifstck]) goto next_line;
//if (if_state[s_ifstck]==2 && !if_result[s_ifstck]) goto next_line;

            if (!strncmp(lp,"if",2)) {
                lp+=2;
                if (ifstck<IF_NEST-1) ifstck++;
                if_state[ifstck]=1;
                if_result[ifstck]=0;
                if (ifstck==1) if_exe[ifstck]=1;
                else if_exe[ifstck]=if_exe[ifstck-1];
                and_or=0;
            } else if (!strncmp(lp,"then",4) && if_state[ifstck]==1) {
                lp+=4;
                if_state[ifstck]=2;
                if (if_exe[ifstck-1]) if_exe[ifstck]=if_result[ifstck];
            } else if (!strncmp(lp,"else",4) && if_state[ifstck]==2) {
                lp+=4;
                if_state[ifstck]=3;
                if (if_exe[ifstck-1]) if_exe[ifstck]=!if_result[ifstck];
            } else if (!strncmp(lp,"endif",5) && if_state[ifstck]>=2) {
                lp+=5;
                if (ifstck>0) {
                  if_state[ifstck]=0;
                  ifstck--;
                }
                goto next_line;
            } else if (!strncmp(lp,"or",2) && if_state[ifstck]==1) {
                lp+=2;
                and_or=1;
            } else if (!strncmp(lp,"and",3) && if_state[ifstck]==1) {
                lp+=3;
                and_or=2;
            }

            if (*lp=='{' && if_state[ifstck]==1) {
              lp+=1; // then
              if_state[ifstck]=2;
              if (if_exe[ifstck-1]) if_exe[ifstck]=if_result[ifstck];
            } else if (*lp=='{' && if_state[ifstck]==3) {
              lp+=1; // after else
              //if_state[ifstck]=3;
            } else if (*lp=='}' && if_state[ifstck]>=2) {
              lp++; // must check for else
              char *slp=lp;
              uint8_t iselse=0;
              for (uint8_t count=0; count<8;count++) {
                if (*lp=='}') {
                  // must be endif
                  break;
                }
                if (!strncmp(lp,"else",4)) {
                  // is before else, no endif
                  if_state[ifstck]=3;
                  if (if_exe[ifstck-1]) if_exe[ifstck]=!if_result[ifstck];
                  lp+=4;
                  iselse=1;
                  SCRIPT_SKIP_SPACES
                  if (*lp=='{') lp++;
                  break;
                }
                lp++;
              }
              if (!iselse) {
                lp=slp;
                // endif
                if (ifstck>0) {
                  if_state[ifstck]=0;
                  ifstck--;
                }
                goto next_line;
              }
            }

            if (!strncmp(lp,"for",3)) {
              // start for next loop, fetch 3 params
              // simple implementation, zero loop count not supported
              lp+=3;
              SCRIPT_SKIP_SPACES
              lp_next=0;
              lp=isvar(lp,&vtype,&ind,0,0,0);
              if ((vtype!=VAR_NV) && (vtype&STYPE)==0) {
                  // numeric var
                  uint8_t index=glob_script_mem.type[ind.index].index;
                  cv_count=&glob_script_mem.fvars[index];
                  SCRIPT_SKIP_SPACES
                  lp=GetNumericResult(lp,OPER_EQU,cv_count,0);
                  SCRIPT_SKIP_SPACES
                  lp=GetNumericResult(lp,OPER_EQU,&cv_max,0);
                  SCRIPT_SKIP_SPACES
                  lp=GetNumericResult(lp,OPER_EQU,&cv_inc,0);
                  //SCRIPT_SKIP_EOL
                  cv_ptr=lp;
                  if (*cv_count<cv_max) {
                    floop=1;
                  } else {
                    floop=2;
                  }
              } else {
                      // error
                  toLogEOL("for error",lp);
              }
            } else if (!strncmp(lp,"next",4)) {
              lp_next=lp;
              if (floop>0) {
                // for next loop
                *cv_count+=cv_inc;
                if (floop==1) {
                  if (*cv_count<=cv_max) {
                    lp=cv_ptr;
                  } else {
                    lp+=4;
                    floop=0;
                  }
                } else {
                  if (*cv_count>=cv_max) {
                    lp=cv_ptr;
                  } else {
                    lp+=4;
                    floop=0;
                  }
                }
              }
            }

            if (!strncmp(lp,"switch",6)) {
              lp+=6;
              SCRIPT_SKIP_SPACES
              char *slp=lp;
              lp=GetNumericResult(lp,OPER_EQU,&swvar,0);
              if (glob_script_mem.glob_error==1) {
                // was string, not number
                lp=slp;
                // get the string
                lp=isvar(lp,&vtype,&ind,0,cmpstr,0);
                swflg=0x81;
              } else {
                swflg=1;
              }
            } else if (!strncmp(lp,"case",4) && swflg>0) {
              lp+=4;
              SCRIPT_SKIP_SPACES
              float cvar;
              if (!(swflg&0x80)) {
                lp=GetNumericResult(lp,OPER_EQU,&cvar,0);
                if (swvar!=cvar) {
                  swflg=2;
                } else {
                  swflg=1;
                }
              } else {
                char str[SCRIPT_MAXSSIZE];
                lp=GetStringResult(lp,OPER_EQU,str,0);
                if (!strcmp(cmpstr,str)) {
                    swflg=0x81;
                } else {
                  swflg=0x82;
                }
              }
            } else if (!strncmp(lp,"ends",4) && swflg>0) {
              lp+=4;
              swflg=0;
            }
            if ((swflg&3)==2) goto next_line;

            SCRIPT_SKIP_SPACES
            //SCRIPT_SKIP_EOL
            if (*lp==SCRIPT_EOL) {
             goto next_line;
            }

            //toLogN(lp,16);
            if (!if_exe[ifstck] && if_state[ifstck]!=1) goto next_line;

#ifdef IFTHEN_DEBUG
            sprintf(tbuff,"stack=%d,exe=%d,state=%d,cmpres=%d execute line: ",ifstck,if_exe[ifstck],if_state[ifstck],if_result[ifstck]);
            toLogEOL(tbuff,lp);
#endif

            if (!strncmp(lp,"break",5)) {
              lp+=5;
              if (floop && lp_next) {
                // should break loop
                lp=lp_next;
                floop=0;
              } else {
                section=0;
              }
              goto next_line;
            } else if (!strncmp(lp,"dp",2) && isdigit(*(lp+2))) {
              lp+=2;
              // number precision
              glob_script_mem.script_dprec=atoi(lp);
              goto next_line;
            } else if (!strncmp(lp,"delay(",6)) {
              lp+=5;
              // delay
              lp=GetNumericResult(lp,OPER_EQU,&fvar,0);
              delay(fvar);
              goto next_line;
            } else if (!strncmp(lp,"spinm(",6)) {
              lp+=6;
              // set pin mode
              lp=GetNumericResult(lp,OPER_EQU,&fvar,0);
              int8_t pinnr=fvar;
              SCRIPT_SKIP_SPACES
              uint8_t mode=0;
              if ((*lp=='I') || (*lp=='O') || (*lp=='P')) {
                switch (*lp) {
                  case 'I':
                    mode=0;
                    break;
                  case 'O':
                    mode=1;
                    break;
                  case 'P':
                    mode=2;
                    break;
                }
                lp++;
              } else {
                lp=GetNumericResult(lp,OPER_EQU,&fvar,0);
                mode=fvar;
              }
              uint8_t pm=0;
              if (mode==0) pm=INPUT;
              if (mode==1) pm=OUTPUT;
              if (mode==2) pm=INPUT_PULLUP;
              pinMode(pinnr,pm);
              goto next_line;
            } else if (!strncmp(lp,"spin(",5)) {
              lp+=5;
              // set pin
              lp=GetNumericResult(lp,OPER_EQU,&fvar,0);
              int8_t pinnr=fvar;
              SCRIPT_SKIP_SPACES
              lp=GetNumericResult(lp,OPER_EQU,&fvar,0);
              int8_t mode=fvar;
              digitalWrite(pinnr,mode&1);
              goto next_line;
            } else if (!strncmp(lp,"svars(",5)) {
              lp+=5;
              // save vars
              Scripter_save_pvars();
              goto next_line;
            }
#ifdef USE_LIGHT
#ifdef USE_WS2812
            else if (!strncmp(lp,"ws2812(",7)) {
              lp+=7;
              lp=isvar(lp,&vtype,&ind,0,0,0);
              if (vtype!=VAR_NV) {
                SCRIPT_SKIP_SPACES
                if (*lp!=')') {
                  lp=GetNumericResult(lp,OPER_EQU,&fvar,0);
                } else {
                  fvar=0;
                }
                // found variable as result
                uint8_t index=glob_script_mem.type[ind.index].index;
                if ((vtype&STYPE)==0) {
                    // numeric result
                  if (glob_script_mem.type[ind.index].bits.is_filter) {
                    uint8_t len=0;
                    float *fa=Get_MFAddr(index,&len,0);
                    //Serial.printf(">> 2 %d\n",(uint32_t)*fa);
                    if (fa && len) ws2812_set_array(fa,len,fvar);
                  }
                }
              }
              goto next_line;
            }
#endif
#endif
#ifdef ESP32
            else if (!strncmp(lp,"beep(",5)) {
              lp+=5;
              lp=GetNumericResult(lp,OPER_EQU,&fvar,0);
              SCRIPT_SKIP_SPACES
              float fvar1;
              lp=GetNumericResult(lp,OPER_EQU,&fvar1,0);
              esp32_beep(fvar,fvar1);
              lp++;
              goto next_line;
            }
#endif

            else if (!strncmp(lp,"=>",2) || !strncmp(lp,"->",2) || !strncmp(lp,"+>",2) || !strncmp(lp,"print",5)) {
                // execute cmd
                uint8_t sflag=0,pflg=0,svmqtt,swll;
                if (*lp=='p') {
                 pflg=1;
                 lp+=5;
                }
                else {
                  if (*lp=='-') sflag=1;
                  if (*lp=='+') sflag=2;
                  lp+=2;
                }
                char *slp=lp;
                SCRIPT_SKIP_SPACES
                #define SCRIPT_CMDMEM 512
                char *cmdmem=(char*)malloc(SCRIPT_CMDMEM);
                if (cmdmem) {
                  char *cmd=cmdmem;
                  uint16_t count;
                  for (count=0; count<SCRIPT_CMDMEM/2-2; count++) {
                    //if (*lp=='\r' || *lp=='\n' || *lp=='}') {
                    if (!*lp || *lp=='\r' || *lp=='\n') {
                        cmd[count]=0;
                        break;
                    }
                    cmd[count]=*lp++;
                  }
                  //AddLog_P(LOG_LEVEL_INFO, tmp);
                  // replace vars in cmd
                  char *tmp=cmdmem+SCRIPT_CMDMEM/2;
                  Replace_Cmd_Vars(cmd,0,tmp,SCRIPT_CMDMEM/2);
                  //toSLog(tmp);

                  if (!strncmp(tmp,"print",5) || pflg) {
                    if (pflg) toLog(tmp);
                    else toLog(&tmp[5]);
                  } else {
                    if (!sflag) {
                      tasm_cmd_activ=1;
                      AddLog_P2(glob_script_mem.script_loglevel&0x7f, PSTR("Script: performs \"%s\""), tmp);
                    } else if (sflag==2) {
                      // allow recursive call
                    } else {
                      tasm_cmd_activ=1;
                      svmqtt=Settings.flag.mqtt_enabled;  // SetOption3 - Enable MQTT
                      swll=Settings.weblog_level;
                      Settings.flag.mqtt_enabled=0;       // SetOption3 - Enable MQTT
                      Settings.weblog_level=0;
                    }
                    ExecuteCommand((char*)tmp, SRC_RULE);
                    tasm_cmd_activ=0;
                    if (sflag==1) {
                      Settings.flag.mqtt_enabled=svmqtt;  // SetOption3  - Enable MQTT
                      Settings.weblog_level=swll;
                    }
                  }
                  if (cmdmem) free(cmdmem);
                }
                lp=slp;
                goto next_line;
            } else if (!strncmp(lp,"=#",2)) {
                // subroutine
                lp+=1;
                char *slp=lp;
                uint8_t plen=0;
                while (*lp) {
                  if (*lp=='\n'|| *lp=='\r'|| *lp=='(') {
                    break;
                  }
                  lp++;
                  plen++;
                }
                if (fromscriptcmd) {
                  char *sp=glob_script_mem.scriptptr;
                  glob_script_mem.scriptptr=glob_script_mem.scriptptr_bu;
                  Run_Scripter(slp,plen,0);
                  glob_script_mem.scriptptr=sp;
                } else {
                  Run_Scripter(slp,plen,0);
                }
                lp=slp;
                goto next_line;
            } else if (!strncmp(lp,"=(",2)) {
                lp+=2;
                char str[128];
                str[0]='>';
                lp=GetStringResult(lp,OPER_EQU,&str[1],0);
                lp++;
                execute_script(str);
            }

            // check for variable result
            if (if_state[ifstck]==1) {
              // evaluate exxpression
              lp=Evaluate_expression(lp,and_or,&if_result[ifstck],jo);
              SCRIPT_SKIP_SPACES
              if (*lp=='{' && if_state[ifstck]==1) {
                lp+=1; // then
                if_state[ifstck]=2;
                if (if_exe[ifstck-1]) if_exe[ifstck]=if_result[ifstck];
              }
              goto next_line;
            } else {
              lp=isvar(lp,&vtype,&ind,&sysvar,0,0);
              if (vtype!=VAR_NV) {
                  // found variable as result
                  globvindex=ind.index; // save destination var index here
                  globaindex=last_findex;
                  uint8_t index=glob_script_mem.type[ind.index].index;
                  if ((vtype&STYPE)==0) {
                      // numeric result
                      if (ind.bits.settable || ind.bits.is_filter) {
                        dfvar=&sysvar;
                        if (ind.bits.settable) {
                          sysv_type=ind.index;
                        } else {
                          sysv_type=0;
                        }

                      } else {
                        dfvar=&glob_script_mem.fvars[index];
                        sysv_type=0;
                      }
                      numeric=1;
                      lp=getop(lp,&lastop);
                      char *slp=lp;
                      glob_script_mem.glob_error=0;
                      lp=GetNumericResult(lp,OPER_EQU,&fvar,jo);
                      if (glob_script_mem.glob_error==1) {
                        // mismatch was string, not number
                        // get the string and convert to number
                        lp=isvar(slp,&vtype,&ind,0,cmpstr,jo);
                        fvar=CharToFloat(cmpstr);
                      }
                      switch (lastop) {
                          case OPER_EQU:
                              if (glob_script_mem.var_not_found) {
                                if (!js) toLogEOL("var not found: ",lp);
                                goto next_line;
                              }
                              *dfvar=fvar;
                              break;
                          case OPER_PLSEQU:
                              *dfvar+=fvar;
                              break;
                          case OPER_MINEQU:
                              *dfvar-=fvar;
                              break;
                          case OPER_MULEQU:
                              *dfvar*=fvar;
                              break;
                          case OPER_DIVEQU:
                              *dfvar/=fvar;
                              break;
                          case OPER_PERCEQU:
                              *dfvar=fmodf(*dfvar,fvar);
                              break;
                          case OPER_ANDEQU:
                              *dfvar=(uint32_t)*dfvar&(uint32_t)fvar;
                              break;
                          case OPER_OREQU:
                              *dfvar=(uint32_t)*dfvar|(uint32_t)fvar;
                              break;
                          case OPER_XOREQU:
                              *dfvar=(uint32_t)*dfvar^(uint32_t)fvar;
                              break;
                          default:
                              // error
                              break;
                      }
                      // var was changed
                      glob_script_mem.type[globvindex].bits.changed=1;
                      if (glob_script_mem.type[globvindex].bits.is_filter) {
                        if (globaindex>=0) {
                          Set_MFVal(glob_script_mem.type[globvindex].index,globaindex,*dfvar);
                        } else {
                          Set_MFilter(glob_script_mem.type[globvindex].index,*dfvar);
                        }
                      }

                      if (sysv_type) {
                        switch (sysv_type) {
                          case SCRIPT_LOGLEVEL:
                            glob_script_mem.script_loglevel=*dfvar;
                            break;
                          case SCRIPT_TELEPERIOD:
                            if (*dfvar<10) *dfvar=10;
                            if (*dfvar>300) *dfvar=300;
                            Settings.tele_period=*dfvar;
                            break;
                        }
                        sysv_type=0;
                      }
                  } else {
                    // string result
                    numeric=0;
                    sindex=index;
                    // string result
                    char str[SCRIPT_MAXSSIZE];
                    lp=getop(lp,&lastop);
                    char *slp=lp;
                    glob_script_mem.glob_error=0;
                    lp=GetStringResult(lp,OPER_EQU,str,jo);
                    if (!js && glob_script_mem.glob_error) {
                      // mismatch
                      lp=GetNumericResult(slp,OPER_EQU,&fvar,0);
                      dtostrfd(fvar,6,str);
                      glob_script_mem.glob_error=0;
                    }

                    if (!glob_script_mem.var_not_found) {
                      // var was changed
                      glob_script_mem.type[globvindex].bits.changed=1;
                      if (lastop==OPER_EQU) {
                        strlcpy(glob_script_mem.glob_snp+(sindex*glob_script_mem.max_ssize),str,glob_script_mem.max_ssize);
                      } else if (lastop==OPER_PLSEQU) {
                        strncat(glob_script_mem.glob_snp+(sindex*glob_script_mem.max_ssize),str,glob_script_mem.max_ssize);
                      }
                    }
                  }

            }
            SCRIPT_SKIP_SPACES
            if (*lp=='{' && if_state[ifstck]==3) {
              lp+=1; // else
              //if_state[ifstck]=3;
            }
            goto next_line;
          }
        } else {
            //Serial.printf(">> decode %s\n",lp );
            // decode line
            if (*lp=='>' && tlen==1) {
              // called from cmdline
              lp++;
              section=1;
              fromscriptcmd=1;
              goto startline;
            }
            if (!strncmp(lp,type,tlen)) {
                // found section
                section=1;
                glob_script_mem.section_ptr=lp;
                if (check) {
                  return 99;
                }
                // check for subroutine
                char *ctype=(char*)type;
                if (*ctype=='#') {
                  // check for parameter
                  ctype+=tlen;
                  if (*ctype=='(' && *(lp+tlen)=='(') {
                    float fparam;
                    numeric=1;
                    glob_script_mem.glob_error=0;
                    GetNumericResult((char*)ctype,OPER_EQU,&fparam,0);
                    if (glob_script_mem.glob_error==1) {
                      // was string, not number
                      numeric=0;
                      // get the string
                      GetStringResult((char*)ctype+1,OPER_EQU,cmpstr,0);
                    }
                    lp+=tlen;
                    if (*lp=='(') {
                      // fetch destination
                      lp++;
                      lp=isvar(lp,&vtype,&ind,0,0,0);
                      if (vtype!=VAR_NV) {
                        // found variable as result
                        uint8_t index=glob_script_mem.type[ind.index].index;
                        if ((vtype&STYPE)==0) {
                            // numeric result
                            dfvar=&glob_script_mem.fvars[index];
                            if (numeric) {
                              *dfvar=fparam;
                            } else {
                              // mismatch
                              *dfvar=CharToFloat(cmpstr);
                            }
                        } else {
                            // string result
                            sindex=index;
                            if (!numeric) {
                              strlcpy(glob_script_mem.glob_snp+(sindex*glob_script_mem.max_ssize),cmpstr,glob_script_mem.max_ssize);
                            } else {
                              // mismatch
                              dtostrfd(fparam,6,glob_script_mem.glob_snp+(sindex*glob_script_mem.max_ssize));
                            }
                        }
                      }
                    }
                  } else {
                    lp+=tlen;
                    if (*ctype=='(' || (*lp!=SCRIPT_EOL && *lp!='?')) {
                      // revert
                      section=0;
                    }
                  }
                }
            }
        }
        // next line
    next_line:
        if (*lp==SCRIPT_EOL) {
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

uint8_t script_xsns_index = 0;


void ScripterEvery100ms(void) {

  if (Settings.rule_enabled && (uptime > 4)) {
    mqtt_data[0] = '\0';
    uint16_t script_tele_period_save = tele_period;
    tele_period = 2;
    XsnsNextCall(FUNC_JSON_APPEND, script_xsns_index);
    tele_period = script_tele_period_save;
    if (strlen(mqtt_data)) {
      mqtt_data[0] = '{';
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s}"), mqtt_data);
      Run_Scripter(">T",2, mqtt_data);
    }
  }
  if (fast_script==99) Run_Scripter(">F",2,0);
}

//mems[5] is 50 bytes in 6.5
// can hold 11 floats or floats + strings
// should report overflow later
void Scripter_save_pvars(void) {
  int16_t mlen=0;
  float *fp=(float*)glob_script_mem.script_pram;
  mlen+=sizeof(float);
  struct T_INDEX *vtp=glob_script_mem.type;
  for (uint8_t count=0; count<glob_script_mem.numvars; count++) {
    if (vtp[count].bits.is_permanent && !vtp[count].bits.is_string) {
      uint8_t index=vtp[count].index;
      mlen+=sizeof(float);
      if (mlen>PMEM_SIZE) {
        vtp[count].bits.is_permanent=0;
        return;
      }
      *fp++=glob_script_mem.fvars[index];
    }
  }
  char *cp=(char*)fp;
  for (uint8_t count=0; count<glob_script_mem.numvars; count++) {
    if (vtp[count].bits.is_permanent && vtp[count].bits.is_string) {
      uint8_t index=vtp[count].index;
      char *sp=glob_script_mem.glob_snp+(index*glob_script_mem.max_ssize);
      uint8_t slen=strlen(sp);
      mlen+=slen+1;
      if (mlen>PMEM_SIZE) {
        vtp[count].bits.is_permanent=0;
        return;
      }
      strcpy(cp,sp);
      cp+=slen+1;
    }
  }
}

// works only with webserver
#ifdef USE_WEBSERVER

#define WEB_HANDLE_SCRIPT "s10"

const char S_CONFIGURE_SCRIPT[] PROGMEM = D_CONFIGURE_SCRIPT;

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


#endif

    "</script>";

const char HTTP_SCRIPT_FORM_END[] PROGMEM =
      "<br/>"
      "<button name='save' type='submit' formmethod='post' formenctype='multipart/form-data' formaction='/ta' class='button bgrn'>" D_SAVE "</button>"
      "</form></fieldset>";

#ifdef USE_SCRIPT_FATFS
const char HTTP_FORM_SCRIPT1c[] PROGMEM =
    "<button name='d%d' type='submit' class='button bgrn'>" D_SCRIPT_DOWNLOAD " '%s'</button>";
#ifdef SDCARD_DIR
const char HTTP_FORM_SCRIPT1d[] PROGMEM =
    "<button method='post' name='upl' type='submit' class='button bgrn'>" D_SDCARD_DIR "</button>";
#else
const char HTTP_FORM_SCRIPT1d[] PROGMEM =
    "<button method='post' name='upl' type='submit' class='button bgrn'>" D_SCRIPT_UPLOAD_FILES "</button>";
#endif

#ifdef SDCARD_DIR
const char S_SCRIPT_FILE_UPLOAD[] PROGMEM = D_SDCARD_DIR;
#else
const char S_SCRIPT_FILE_UPLOAD[] PROGMEM = D_SDCARD_UPLOAD;
#endif

const char HTTP_FORM_FILE_UPLOAD[] PROGMEM =
"<div id='f1' name='f1' style='display:block;'>"
"<fieldset><legend><b>&nbsp;%s"  "&nbsp;</b></legend>";
const char HTTP_FORM_FILE_UPG[] PROGMEM =
"<form method='post' action='u3' enctype='multipart/form-data'>"
"<br/><input type='file' name='u3'><br/>"
"<br/><button type='submit' onclick='eb(\"f1\").style.display=\"none\";eb(\"f2\").style.display=\"block\";this.form.submit();'>" D_START " %s</button></form>";

const char HTTP_FORM_FILE_UPGb[] PROGMEM =
"</fieldset>"
"</div>"
"<div id='f2' name='f2' style='display:none;text-align:center;'><b>" D_UPLOAD_STARTED " ...</b></div>";

const char HTTP_FORM_SDC_DIRa[] PROGMEM =
"<div style='text-align:left'>";
const char HTTP_FORM_SDC_DIRb[] PROGMEM =
 "<pre><a href='%s' file='%s'>%s</a>     %s : %8d</pre>";
const char HTTP_FORM_SDC_DIRd[] PROGMEM =
"<pre><a href='%s' file='%s'>%s</a></pre>";
const char HTTP_FORM_SDC_DIRc[] PROGMEM =
"</div>";
const char HTTP_FORM_SDC_HREF[] PROGMEM =
"http://%s/upl?download=%s/%s";
#endif


uint8_t *script_ex_ptr;
uint16_t uplsize;
uint8_t sc_state;

// upload script and start immediately
void script_upload_start(void) {

  //AddLog_P(LOG_LEVEL_INFO, PSTR("HTP: file upload execute"));

  HTTPUpload& upload = Webserver->upload();
  if (upload.status == UPLOAD_FILE_START) {
    //AddLog_P(LOG_LEVEL_INFO, PSTR("HTP: upload start"));
    script_ex_ptr=(uint8_t*)glob_script_mem.script_ram;
    //AddLog_P2(LOG_LEVEL_INFO, PSTR("HTP: upload file %s, %d"),upload.filename.c_str(),upload.totalSize);

    if (strcmp(upload.filename.c_str(),"execute_script")) {
      Web.upload_error=1;
      WSSend(500, CT_PLAIN, F("500: wrong filename"));
      return;
    }
    if (upload.totalSize>=glob_script_mem.script_size) {
      Web.upload_error=1;
      WSSend(500, CT_PLAIN, F("500: file to large"));
      return;
    }
    uplsize=0;

    sc_state = bitRead(Settings.rule_enabled, 0);
    bitWrite(Settings.rule_enabled,0,0);

  } else if(upload.status == UPLOAD_FILE_WRITE) {
    //AddLog_P(LOG_LEVEL_INFO, PSTR("HTP: upload write"));
    uint32_t csiz=upload.currentSize;
    uint32_t tsiz=glob_script_mem.script_size-1;
    if (uplsize<tsiz) {
      if (uplsize+csiz<tsiz) {
        memcpy(script_ex_ptr,upload.buf,csiz);
        script_ex_ptr+=csiz;
        uplsize+=csiz;
      } else {
        csiz=tsiz-uplsize;
        memcpy(script_ex_ptr,upload.buf,csiz);
        script_ex_ptr+=csiz;
        uplsize+=csiz;
      }
      //AddLog_P2(LOG_LEVEL_INFO, PSTR("HTP: write %d - %d"),csiz,uplsize);
    }

    //if (upload_file) upload_file.write(upload.buf,upload.currentSize);
  } else if(upload.status == UPLOAD_FILE_END) {
    AddLog_P(LOG_LEVEL_INFO, PSTR("HTP: upload close"));
    //if (upload_file) upload_file.close();
    if (Web.upload_error) {
      AddLog_P(LOG_LEVEL_INFO, PSTR("HTP: upload error"));
    } else {
      *script_ex_ptr=0;
      bitWrite(Settings.rule_enabled,0,sc_state);
      SaveScript();
      SaveScriptEnd();
    }
  } else {
    Web.upload_error=1;
    WSSend(500, CT_PLAIN, F("500: couldn't create file"));
  }
}

void ScriptExecuteUploadSuccess(void) {
  WSSend(200, CT_PLAIN, F("transfer OK"));
}


#ifdef USE_SCRIPT_FATFS

#if USE_LONG_FILE_NAMES>0
#undef REJCMPL
#define REJCMPL 6
#else
#undef REJCMPL
#define REJCMPL 8
#endif

uint8_t reject(char *name) {

  char *lcp = strrchr(name,'/');
  if (lcp) {
    name=lcp+1;
  }

  while (*name=='/') name++;
  if (*name=='_') return 1;
  if (*name=='.') return 1;

#ifndef ARDUINO_ESP8266_RELEASE_2_3_0
  if (!strncasecmp(name,"SPOTLI~1",REJCMPL)) return 1;
  if (!strncasecmp(name,"TRASHE~1",REJCMPL)) return 1;
  if (!strncasecmp(name,"FSEVEN~1",REJCMPL)) return 1;
  if (!strncasecmp(name,"SYSTEM~1",REJCMPL)) return 1;
#else
  if (!strcasecmp(name,"SPOTLI~1")) return 1;
  if (!strcasecmp(name,"TRASHE~1")) return 1;
  if (!strcasecmp(name,"FSEVEN~1")) return 1;
  if (!strcasecmp(name,"SYSTEM~1")) return 1;
#endif

  if (!strncasecmp(name,"System Volume",13)) return 1;
  return 0;
}

void ListDir(char *path, uint8_t depth) {
  char name[32];
  char npath[128];
  char format[12];
  sprintf(format,"%%-%ds",24-depth);

  File dir=fsp->open(path, FILE_READ);
  if (dir) {
    dir.rewindDirectory();
    if (strlen(path)>1) {
      snprintf_P(npath,sizeof(npath),PSTR("http://%s/upl?download=%s"),WiFi.localIP().toString().c_str(),path);
      for (uint8_t cnt=strlen(npath)-1;cnt>0;cnt--) {
        if (npath[cnt]=='/') {
          if (npath[cnt-1]=='=') npath[cnt+1]=0;
          else npath[cnt]=0;
          break;
        }
      }
      WSContentSend_P(HTTP_FORM_SDC_DIRd,npath,path,"..");
    }
    char *ep;
    while (true) {
      File entry=dir.openNextFile();
      if (!entry) {
        break;
      }
      // esp32 returns path here, shorten to filename
      ep=(char*)entry.name();
      if (*ep=='/') ep++;
      char *lcp = strrchr(ep,'/');
      if (lcp) {
        ep=lcp+1;
      }
      //AddLog_P2(LOG_LEVEL_INFO, PSTR("entry: %s"),ep);
      time_t tm=entry.getLastWrite();
      char tstr[24];
      strftime(tstr, 22, "%d-%m-%Y - %H:%M:%S ", localtime(&tm));

      char *pp=path;
      if (!*(pp+1)) pp++;
      char *cp=name;
      // osx formatted disks contain a lot of stuff we dont want
      if (reject((char*)ep)) goto fclose;

      for (uint8_t cnt=0;cnt<depth;cnt++) {
        *cp++='-';
      }

      sprintf(cp,format,ep);
      if (entry.isDirectory()) {
        snprintf_P(npath,sizeof(npath),HTTP_FORM_SDC_HREF,WiFi.localIP().toString().c_str(),pp,ep);
        WSContentSend_P(HTTP_FORM_SDC_DIRd,npath,ep,name);
        uint8_t plen=strlen(path);
        if (plen>1) {
          strcat(path,"/");
        }
        strcat(path,ep);
        ListDir(path,depth+4);
        path[plen]=0;
      } else {
          snprintf_P(npath,sizeof(npath),HTTP_FORM_SDC_HREF,WiFi.localIP().toString().c_str(),pp,ep);
          WSContentSend_P(HTTP_FORM_SDC_DIRb,npath,ep,name,tstr,entry.size());
      }
      fclose:
      entry.close();
    }
    dir.close();
  }
}

char path[48];

void Script_FileUploadConfiguration(void)
{
  uint8_t depth=0;
  strcpy(path,"/");

  if (!HttpCheckPriviledgedAccess()) { return; }

  if (Webserver->hasArg("download")) {
    String stmp = Webserver->arg("download");
    char *cp=(char*)stmp.c_str();
    if (DownloadFile(cp)) {
      // is directory
      strcpy(path,cp);
    }
  }

  void ScriptFileUploadSuccess(void) {
    WSContentStart_P(S_INFORMATION);
    WSContentSendStyle();
    WSContentSend_P(PSTR("<div style='text-align:center;'><b>" D_UPLOAD " <font color='#"));
    WSContentSend_P(PSTR("%06x'>" D_SUCCESSFUL "</font></b><br/>"), WebColor(COL_TEXT_SUCCESS));
    WSContentSend_P(PSTR("</div><br/>"));
    WSContentSend_P(PSTR("<p><form action='%s' method='get'><button>%s</button></form></p>"),"/upl",D_UPL_DONE);
    //WSContentSpaceButton(BUTTON_MAIN);
    WSContentStop();
  }

  WSContentStart_P(S_SCRIPT_FILE_UPLOAD);
  WSContentSendStyle();
  WSContentSend_P(HTTP_FORM_FILE_UPLOAD,D_SDCARD_DIR);
  WSContentSend_P(HTTP_FORM_FILE_UPG, D_SCRIPT_UPLOAD);
#ifdef SDCARD_DIR
  WSContentSend_P(HTTP_FORM_SDC_DIRa);
  if (glob_script_mem.script_sd_found) {
    ListDir(path,depth);
  }
  WSContentSend_P(HTTP_FORM_SDC_DIRc);
#endif
  WSContentSend_P(HTTP_FORM_FILE_UPGb);
  WSContentSpaceButton(BUTTON_CONFIGURATION);
  WSContentStop();
  Web.upload_error = 0;
}

File upload_file;


void script_upload(void) {

  //AddLog_P(LOG_LEVEL_INFO, PSTR("HTP: file upload"));

  HTTPUpload& upload = Webserver->upload();
  if (upload.status == UPLOAD_FILE_START) {
    char npath[48];
    sprintf(npath,"%s/%s",path,upload.filename.c_str());
    fsp->remove(npath);
    upload_file=fsp->open(npath,FILE_WRITE);
    if (!upload_file) Web.upload_error=1;
  } else if(upload.status == UPLOAD_FILE_WRITE) {
    if (upload_file) upload_file.write(upload.buf,upload.currentSize);
  } else if(upload.status == UPLOAD_FILE_END) {
    if (upload_file) upload_file.close();
    if (Web.upload_error) {
      AddLog_P(LOG_LEVEL_INFO, PSTR("HTP: upload error"));
    }
  } else {
    Web.upload_error=1;
    Webserver->send(500, "text/plain", "500: couldn't create file");
  }
}

uint8_t DownloadFile(char *file) {
  File download_file;
  WiFiClient download_Client;

    if (!fsp->exists(file)) {
      AddLog_P(LOG_LEVEL_INFO,PSTR("file not found"));
      return 0;
    }

    download_file=fsp->open(file,FILE_READ);
    if (!download_file) {
      AddLog_P(LOG_LEVEL_INFO,PSTR("could not open file"));
      return 0;
    }

    if (download_file.isDirectory()) {
      download_file.close();
      return 1;
    }

    uint32_t flen=download_file.size();

    download_Client = Webserver->client();
    Webserver->setContentLength(flen);

    char attachment[100];
    char *cp;
    for (uint8_t cnt=strlen(file); cnt>=0; cnt--) {
      if (file[cnt]=='/') {
        cp=&file[cnt+1];
        break;
      }
    }
    snprintf_P(attachment, sizeof(attachment), PSTR("attachment; filename=%s"),cp);
    Webserver->sendHeader(F("Content-Disposition"), attachment);
    WSSend(200, CT_STREAM, "");

    uint8_t buff[512];
    uint16_t bread;

    // transfer is about 150kb/s
    uint8_t cnt=0;
    while (download_file.available()) {
      bread=download_file.read(buff,sizeof(buff));
      uint16_t bw=download_Client.write((const char*)buff,bread);
      if (!bw) break;
      cnt++;
      if (cnt>7) {
        cnt=0;
        if (glob_script_mem.script_loglevel&0x80) {
          // this indeed multitasks, but is slower 50 kB/s
          loop();
        }
      }
    }
    download_file.close();
    download_Client.stop();
    return 0;
}

#endif


void HandleScriptTextareaConfiguration(void) {
  if (!HttpCheckPriviledgedAccess()) { return; }

  if (Webserver->hasArg("save")) {
    ScriptSaveSettings();
    HandleConfiguration();
    return;
  }
}


void HandleScriptConfiguration(void) {

    if (!HttpCheckPriviledgedAccess()) { return; }

    AddLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, S_CONFIGURE_SCRIPT);

#ifdef USE_SCRIPT_FATFS
    if (Webserver->hasArg("d1")) {
      DownloadFile(glob_script_mem.flink[0]);
    }
    if (Webserver->hasArg("d2")) {
      DownloadFile(glob_script_mem.flink[1]);
    }
    if (Webserver->hasArg("upl")) {
      Script_FileUploadConfiguration();
    }
#endif

    WSContentStart_P(S_CONFIGURE_SCRIPT);
    WSContentSendStyle();
    WSContentSend_P(HTTP_FORM_SCRIPT);


#ifdef xSCRIPT_STRIP_COMMENTS
    uint16_t ssize=glob_script_mem.script_size;
    if (bitRead(Settings.rule_enabled, 1)) ssize*=2;
    WSContentSend_P(HTTP_FORM_SCRIPT1,1,1,bitRead(Settings.rule_enabled,0) ? " checked" : "",ssize);
#else
    WSContentSend_P(HTTP_FORM_SCRIPT1,1,1,bitRead(Settings.rule_enabled,0) ? " checked" : "",glob_script_mem.script_size);
#endif

    // script is to larg for WSContentSend_P
    if (glob_script_mem.script_ram[0]) {
      _WSContentSend(glob_script_mem.script_ram);
    }
    WSContentSend_P(HTTP_FORM_SCRIPT1b);

#ifdef USE_SCRIPT_FATFS
    if (glob_script_mem.script_sd_found) {
      WSContentSend_P(HTTP_FORM_SCRIPT1d);
      if (glob_script_mem.flink[0][0]) WSContentSend_P(HTTP_FORM_SCRIPT1c,1,glob_script_mem.flink[0]);
      if (glob_script_mem.flink[1][0]) WSContentSend_P(HTTP_FORM_SCRIPT1c,2,glob_script_mem.flink[1]);
    }
#endif

    WSContentSend_P(HTTP_SCRIPT_FORM_END);
    WSContentSpaceButton(BUTTON_CONFIGURATION);
    WSContentStop();
}

void SaveScript(void) {

#ifdef EEP_SCRIPT_SIZE
  if (glob_script_mem.flags&1) {
    EEP_WRITE(0,EEP_SCRIPT_SIZE,glob_script_mem.script_ram);
  }
#endif // EEP_SCRIPT_SIZE

#ifdef USE_SCRIPT_FATFS
  if (glob_script_mem.flags&1) {
    fsp->remove(FAT_SCRIPT_NAME);
    File file=fsp->open(FAT_SCRIPT_NAME,FILE_WRITE);
    file.write((const uint8_t*)glob_script_mem.script_ram,FAT_SCRIPT_SIZE);
    file.close();
  }
#endif // USE_SCRIPT_FATFS

#ifdef LITTLEFS_SCRIPT_SIZE
  if (glob_script_mem.flags&1) {
    SaveFile("/script.txt",(uint8_t*)glob_script_mem.script_ram,LITTLEFS_SCRIPT_SIZE);
  }
#endif // LITTLEFS_SCRIPT_SIZE
}

void ScriptSaveSettings(void) {

  if (Webserver->hasArg("c1")) {
    bitWrite(Settings.rule_enabled,0,1);
  } else {
    bitWrite(Settings.rule_enabled,0,0);
  }


  String str = Webserver->arg("t1");

  if (*str.c_str()) {

    str.replace("\r\n","\n");
    str.replace("\r","\n");

#ifdef xSCRIPT_STRIP_COMMENTS
    if (bitRead(Settings.rule_enabled, 1)) {
      char *sp=(char*)str.c_str();
      char *sp1=sp;
      char *dp=sp;
      uint8_t flg=0;
      while (*sp) {
        while (*sp==' ') sp++;
        sp1=sp;
        sp=strchr(sp,'\n');
        if (!sp) {
          flg=1;
        } else {
          *sp=0;
        }
        if (*sp1!=';') {
          uint8_t slen=strlen(sp1);
          if (slen) {
            strcpy(dp,sp1);
            dp+=slen;
            *dp++='\n';
          }
        }
        if (flg) {
          *dp=0;
          break;
        }
        sp++;
      }
    }
#endif

    strlcpy(glob_script_mem.script_ram,str.c_str(), glob_script_mem.script_size);

    if (glob_script_mem.script_ram[0]!='>' && glob_script_mem.script_ram[1]!='D') {
      AddLog_P2(LOG_LEVEL_INFO, PSTR("script error: must start with >D"));
      bitWrite(Settings.rule_enabled, 0, 0);
    }

    SaveScript();

  }

  SaveScriptEnd();
}

void SaveScriptEnd(void) {
  if (glob_script_mem.script_mem) {
    Scripter_save_pvars();
    free(glob_script_mem.script_mem);
    glob_script_mem.script_mem=0;
    glob_script_mem.script_mem_size=0;
  }

#ifdef USE_UNISHOX_COMPRESSION
  //AddLog_P2(LOG_LEVEL_INFO,PSTR("in string: %s len = %d"),glob_script_mem.script_ram,strlen(glob_script_mem.script_ram));
  uint32_t len_compressed = SCRIPT_COMPRESS(glob_script_mem.script_ram, strlen(glob_script_mem.script_ram), Settings.rules[0], MAX_SCRIPT_SIZE-1);
  if (len_compressed > 0) {
    Settings.rules[0][len_compressed] = 0;
    AddLog_P2(LOG_LEVEL_INFO,PSTR("script compressed to %d bytes = %d %%"),len_compressed,len_compressed * 100 / strlen(glob_script_mem.script_ram));
  } else {
    AddLog_P2(LOG_LEVEL_INFO, PSTR("script compress error: %d"), len_compressed);
  }
#endif // USE_UNISHOX_COMPRESSION

  if (bitRead(Settings.rule_enabled, 0)) {
    int16_t res=Init_Scripter();
    if (res) {
      AddLog_P2(LOG_LEVEL_INFO, PSTR("script init error: %d"), res);
      return;
    }
    Run_Scripter(">B",2,0);
    fast_script=Run_Scripter(">F",-2,0);
  }
}

#endif


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


void Script_HueStatus(String *response, uint16_t hue_devs) {

  if (hue_script[hue_devs].type=='p') {
    *response+=FPSTR(SCRIPT_HUE_LIGHTS_STATUS_JSON2);
    response->replace("{j1",hue_script[hue_devs].name);
    response->replace("{j2", GetHueDeviceId(hue_devs));
    uint8_t pwr=glob_script_mem.fvars[hue_script[hue_devs].index[0]-1];
    response->replace("{state}", (pwr ? "true" : "false"));
    return;
  }

  *response+=FPSTR(SCRIPT_HUE_LIGHTS_STATUS_JSON1);
  uint8_t pwr=glob_script_mem.fvars[hue_script[hue_devs].index[0]-1];
  response->replace("{state}", (pwr ? "true" : "false"));
  String light_status = "";
  if (hue_script[hue_devs].index[1]>0) {
    // bri
    light_status += "\"bri\":";
    uint32_t bri=glob_script_mem.fvars[hue_script[hue_devs].index[1]-1];
    if (bri > 254)  bri = 254;
    if (bri < 1)    bri = 1;
    light_status += String(bri);
    light_status += ",";
  }
  if (hue_script[hue_devs].index[2]>0) {
    // hue
    uint32_t hue=glob_script_mem.fvars[hue_script[hue_devs].index[2]-1];
    //hue = changeUIntScale(hue, 0, 359, 0, 65535);
    light_status += "\"hue\":";
    light_status += String(hue);
    light_status += ",";
  }
  if (hue_script[hue_devs].index[3]>0) {
    // sat
    uint32_t sat=glob_script_mem.fvars[hue_script[hue_devs].index[3]-1] ;
    if (sat > 254)  sat = 254;
    if (sat < 1)    sat = 1;
    light_status += "\"sat\":";
    light_status += String(sat);
    light_status += ",";
  }
  if (hue_script[hue_devs].index[4]>0) {
    // ct
    uint32_t ct=glob_script_mem.fvars[hue_script[hue_devs].index[4]-1];
    light_status += "\"ct\":";
    light_status += String(ct);
    light_status += ",";
  }

  float temp;
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
  response->replace("{j1",hue_script[hue_devs].name);
  response->replace("{j2", GetHueDeviceId(hue_devs));

}

void Script_Check_Hue(String *response) {
  if (!bitRead(Settings.rule_enabled, 0)) return;

  uint8_t hue_script_found=Run_Scripter(">H",-2,0);
  if (hue_script_found!=99) return;

  char tmp[256];
  uint8_t hue_devs=0;
  uint8_t vindex=0;
  char *cp;
  char *lp=glob_script_mem.section_ptr+2;
  while (lp) {
    SCRIPT_SKIP_SPACES
    while (*lp==SCRIPT_EOL) {
     lp++;
    }
    if (!*lp || *lp=='#' || *lp=='>') {
        break;
    }
    if (*lp!=';') {
      // check this line
      Replace_Cmd_Vars(lp,1,tmp,sizeof(tmp));
      // check for hue defintions
      // NAME, TYPE , vars
      cp=tmp;
      cp=strchr(cp,',');
      if (!cp) break;
      *cp=0;
      // copy name
      strlcpy(hue_script[hue_devs].name,tmp,HUE_DEV_NSIZE);
      cp++;
      while (*cp==' ') cp++;
      // get type
      hue_script[hue_devs].type=*cp;

      for (vindex=0;vindex<HUE_DEV_MVNUM;vindex++) {
        hue_script[hue_devs].index[vindex]=0;
      }
      vindex=0;
      while (1) {
        cp=strchr(cp,',');
        if (!cp) break;
        // get vars, on,hue,sat,bri,ct,
        cp++;
        while (*cp==' ') cp++;

        vindex==0xff;
        if (!strncmp(cp,"on=",3)) {
          cp+=3;
          vindex=0;
        } else if (!strncmp(cp,"bri=",4)) {
          cp+=4;
          vindex=1;
        } else if (!strncmp(cp,"hue=",4)) {
          cp+=4;
          vindex=2;
        } else if (!strncmp(cp,"sat=",4)) {
          cp+=4;
          vindex=3;
        } else if (!strncmp(cp,"ct=",3)) {
          cp+=3;
          vindex=4;
        } else {
          // error
          vindex==0xff;
          break;
        }
        if (vindex!=0xff) {
          struct T_INDEX ind;
          uint8_t vtype;
          char vname[16];
          for (uint32_t cnt=0;cnt<sizeof(vname)-1;cnt++) {
            if (*cp==',' || *cp==0) {
              vname[cnt]=0;
              break;
            }
            vname[cnt]=*cp++;
          }
          isvar(vname,&vtype,&ind,0,0,0);
          if (vtype!=VAR_NV) {
            // found variable as result
            if (vtype==NUM_RES || (vtype&STYPE)==0) {
              hue_script[hue_devs].vindex[vindex]=ind.index;
              hue_script[hue_devs].index[vindex]=glob_script_mem.type[ind.index].index+1;
            } else {
            //  break;
            }
          }
        }
      }
      // append response
      if (response) {
        if (devices_present) {
          *response+=",\"";
        }
        else {
          if (hue_devs>0) *response+=",\"";
        }
        *response+=String(EncodeLightId(hue_devs+devices_present+1))+"\":";
        Script_HueStatus(response,hue_devs);
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
#if 0
  if (response) {
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Hue: %d"), hue_devs);
    toLog(">>>>");
    toLog(response->c_str());
    toLog(response->c_str()+LOGSZ);
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
void Script_Handle_Hue(String *path) {
  String response;
  int code = 200;
  uint16_t tmp = 0;
  uint16_t hue = 0;
  uint8_t  sat = 0;
  uint8_t  bri = 254;
  uint16_t ct = 0;
  bool resp = false;

  uint8_t device = DecodeLightId(atoi(path->c_str()));
  uint8_t index = device-devices_present-1;

  if (Webserver->args()) {
    response = "[";

    StaticJsonBuffer<400> jsonBuffer;
    JsonObject &hue_json = jsonBuffer.parseObject(Webserver->arg((Webserver->args())-1));
    if (hue_json.containsKey("on")) {

      response += FPSTR(sHUE_LIGHT_RESPONSE_JSON);
      response.replace("{id", String(EncodeLightId(device)));
      response.replace("{cm", "on");

      bool on = hue_json["on"];
      switch(on)
      {
        case false : glob_script_mem.fvars[hue_script[index].index[0]-1]=0;
                     response.replace("{re", "false");
                     break;
        case true  : glob_script_mem.fvars[hue_script[index].index[0]-1]=1;
                     response.replace("{re", "true");
                     break;
      }
      glob_script_mem.type[hue_script[index].vindex[0]].bits.changed=1;
      resp = true;
    }
    if (hue_json.containsKey("bri")) {             // Brightness is a scale from 1 (the minimum the light is capable of) to 254 (the maximum). Note: a brightness of 1 is not off.
      tmp = hue_json["bri"];
      bri=tmp;
      if (254 <= bri) { bri = 255; }
      if (resp) { response += ","; }
      response += FPSTR(sHUE_LIGHT_RESPONSE_JSON);
      response.replace("{id", String(EncodeLightId(device)));
      response.replace("{cm", "bri");
      response.replace("{re", String(tmp));
      glob_script_mem.fvars[hue_script[index].index[1]-1]=bri;
      glob_script_mem.type[hue_script[index].vindex[1]].bits.changed=1;
      resp = true;
    }
    if (hue_json.containsKey("xy")) {             // Saturation of the light. 254 is the most saturated (colored) and 0 is the least saturated (white).
      float x, y;
      x = hue_json["xy"][0];
      y = hue_json["xy"][1];
      const String &x_str = hue_json["xy"][0];
      const String &y_str = hue_json["xy"][1];
      uint8_t rr,gg,bb;
      LightStateClass::XyToRgb(x, y, &rr, &gg, &bb);
      LightStateClass::RgbToHsb(rr, gg, bb, &hue, &sat, nullptr);
      if (resp) { response += ","; }
      response += FPSTR(sHUE_LIGHT_RESPONSE_JSON);
      response.replace("{id", String(device));
      response.replace("{cm", "xy");
      response.replace("{re", "[" + x_str + "," + y_str + "]");
      glob_script_mem.fvars[hue_script[index].index[2]-1]=hue;
      glob_script_mem.type[hue_script[index].vindex[2]].bits.changed=1;
      glob_script_mem.fvars[hue_script[index].index[3]-1]=sat;
      glob_script_mem.type[hue_script[index].vindex[3]].bits.changed=1;
      resp = true;
    }

    if (hue_json.containsKey("hue")) {             // The hue value is a wrapping value between 0 and 65535. Both 0 and 65535 are red, 25500 is green and 46920 is blue.
      tmp = hue_json["hue"];
      //hue = changeUIntScale(tmp, 0, 65535, 0, 359);
      //tmp = changeUIntScale(hue, 0, 359, 0, 65535);
      hue=tmp;
      if (resp) { response += ","; }
      response += FPSTR(sHUE_LIGHT_RESPONSE_JSON);
      response.replace("{id", String(EncodeLightId(device)));
      response.replace("{cm", "hue");
      response.replace("{re", String(tmp));
      glob_script_mem.fvars[hue_script[index].index[2]-1]=hue;
      glob_script_mem.type[hue_script[index].vindex[2]].bits.changed=1;
      resp = true;
    }
    if (hue_json.containsKey("sat")) {             // Saturation of the light. 254 is the most saturated (colored) and 0 is the least saturated (white).
      tmp = hue_json["sat"];
      sat=tmp;
      if (254 <= sat) { sat = 255; }
      if (resp) { response += ","; }
      response += FPSTR(sHUE_LIGHT_RESPONSE_JSON);
      response.replace("{id", String(EncodeLightId(device)));
      response.replace("{cm", "sat");
      response.replace("{re", String(tmp));
      glob_script_mem.fvars[hue_script[index].index[3]-1]=sat;
      glob_script_mem.type[hue_script[index].vindex[3]].bits.changed=1;
      resp = true;
    }
    if (hue_json.containsKey("ct")) {  // Color temperature 153 (Cold) to 500 (Warm)
      ct = hue_json["ct"];
      if (resp) { response += ","; }
      response += FPSTR(sHUE_LIGHT_RESPONSE_JSON);
      response.replace("{id", String(EncodeLightId(device)));
      response.replace("{cm", "ct");
      response.replace("{re", String(ct));
      glob_script_mem.fvars[hue_script[index].index[4]-1]=ct;
      glob_script_mem.type[hue_script[index].vindex[4]].bits.changed=1;
      resp = true;
    }
    response += "]";

  } else {
    response = FPSTR(sHUE_ERROR_JSON);
  }
  AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_HTTP D_HUE " Result (%s)"), response.c_str());
  WSSend(code, CT_JSON, response);
  if (resp) {
    Run_Scripter(">E",2,0);
  }
}
#endif  // hue interface


#ifdef USE_SCRIPT_SUB_COMMAND
bool Script_SubCmd(void) {
  if (!bitRead(Settings.rule_enabled, 0)) return false;

  if (tasm_cmd_activ) return false;

  char command[CMDSZ];
  strlcpy(command,XdrvMailbox.topic,CMDSZ);
  uint32_t pl=XdrvMailbox.payload;
  char pld[64];
  strlcpy(pld,XdrvMailbox.data,sizeof(pld));

  char cmdbuff[128];
  char *cp=cmdbuff;
  *cp++='#';
  strcpy(cp,XdrvMailbox.topic);
  uint8_t tlen=strlen(XdrvMailbox.topic);
  cp+=tlen;
  if (XdrvMailbox.index > 0) {
    *cp++=XdrvMailbox.index|0x30;
    tlen++;
  }
  if ((XdrvMailbox.payload>0) || (XdrvMailbox.data_len>0)) {
    *cp++='(';
    strncpy(cp,XdrvMailbox.data,XdrvMailbox.data_len);
    cp+=XdrvMailbox.data_len;
    *cp++=')';
    *cp=0;
  }
  //toLog(cmdbuff);
  uint32_t res=Run_Scripter(cmdbuff,tlen+1,0);
  //AddLog_P2(LOG_LEVEL_INFO,">>%d",res);
  if (res) return false;
  else {
    if (pl>=0) {
      Response_P(S_JSON_COMMAND_NVALUE, command, pl);
    } else {
      Response_P(S_JSON_COMMAND_SVALUE, command, pld);
    }
  }
  return true;
}
#endif

void execute_script(char *script) {
  char *svd_sp=glob_script_mem.scriptptr;
  strcat(script,"\n#");
  glob_script_mem.scriptptr=script;
  Run_Scripter(">",1,0);
  glob_script_mem.scriptptr=svd_sp;
}
#define D_CMND_SCRIPT "Script"
#define D_CMND_SUBSCRIBE "Subscribe"
#define D_CMND_UNSUBSCRIBE "Unsubscribe"

enum ScriptCommands { CMND_SCRIPT,CMND_SUBSCRIBE, CMND_UNSUBSCRIBE };
const char kScriptCommands[] PROGMEM = D_CMND_SCRIPT "|" D_CMND_SUBSCRIBE "|" D_CMND_UNSUBSCRIBE;

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

    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 4)) {
      switch (XdrvMailbox.payload) {
        case 0: // Off
        case 1: // On
          bitWrite(Settings.rule_enabled, index -1, XdrvMailbox.payload);
          break;
#ifdef xSCRIPT_STRIP_COMMENTS
        case 2:
          bitWrite(Settings.rule_enabled, 1,0);
          break;
        case 3:
          bitWrite(Settings.rule_enabled, 1,1);
          break;
#endif
      }
    } else {
      if ('>' == XdrvMailbox.data[0]) {
        // execute script
        snprintf_P (mqtt_data, sizeof(mqtt_data), PSTR("{\"%s\":\"%s\"}"),command,XdrvMailbox.data);
        if (bitRead(Settings.rule_enabled, 0)) {
          for (uint8_t count=0; count<XdrvMailbox.data_len; count++) {
            if (XdrvMailbox.data[count]==';') XdrvMailbox.data[count]='\n';
          }
          execute_script(XdrvMailbox.data);
        }
      }
      return serviced;
    }
    snprintf_P (mqtt_data, sizeof(mqtt_data), PSTR("{\"%s\":\"%s\",\"Free\":%d}"),command, GetStateText(bitRead(Settings.rule_enabled,0)),glob_script_mem.script_size-strlen(glob_script_mem.script_ram));
#ifdef SUPPORT_MQTT_EVENT
  } else if (CMND_SUBSCRIBE == command_code) {			//MQTT Subscribe command. Subscribe <Event>, <Topic> [, <Key>]
      String result = ScriptSubscribe(XdrvMailbox.data, XdrvMailbox.data_len);
      Response_P(S_JSON_COMMAND_SVALUE, command, result.c_str());
    } else if (CMND_UNSUBSCRIBE == command_code) {			//MQTT Un-subscribe command. UnSubscribe <Event>
      String result = ScriptUnsubscribe(XdrvMailbox.data, XdrvMailbox.data_len);
      Response_P(S_JSON_COMMAND_SVALUE, command, result.c_str());
#endif        //SUPPORT_MQTT_EVENT
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

#endif



#ifdef SUPPORT_MQTT_EVENT

#ifndef MQTT_EVENT_MSIZE
#define MQTT_EVENT_MSIZE 256
#endif
#ifndef MQTT_EVENT_JSIZE
#define MQTT_EVENT_JSIZE 400
#endif

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
bool ScriptMqttData(void)
{
  bool serviced = false;
  //toLog(">>> 1");
  //toLog(XdrvMailbox.data);
  if (XdrvMailbox.data_len < 1 || XdrvMailbox.data_len > MQTT_EVENT_MSIZE) {
    return false;
  }
  String sTopic = XdrvMailbox.topic;
  String sData = XdrvMailbox.data;
  //AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Script: MQTT Topic %s, Event %s"), XdrvMailbox.topic, XdrvMailbox.data);
  MQTT_Subscription event_item;
  //Looking for matched topic
  for (uint32_t index = 0; index < subscriptions.size(); index++) {
    event_item = subscriptions.get(index);

    //AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Script: Match MQTT message Topic %s with subscription topic %s"), sTopic.c_str(), event_item.Topic.c_str());
    if (sTopic.startsWith(event_item.Topic)) {
      //This topic is subscribed by us, so serve it
      serviced = true;
      String value;
      String lkey;
      if (event_item.Key.length() == 0) {   //If did not specify Key
        value = sData;
      } else {      //If specified Key, need to parse Key/Value from JSON data
        StaticJsonBuffer<MQTT_EVENT_JSIZE> jsonBuf;
        JsonObject& jsonData = jsonBuf.parseObject(sData);
        String key1 = event_item.Key;
        String key2;
        if (!jsonData.success()) break;       //Failed to parse JSON data, ignore this message.
        int dot;
        if ((dot = key1.indexOf('.')) > 0) {
          key2 = key1.substring(dot+1);
          key1 = key1.substring(0, dot);
          lkey=key2;
          if (!jsonData[key1][key2].success()) break;   //Failed to get the key/value, ignore this message.
          value = (const char *)jsonData[key1][key2];
        } else {
          if (!jsonData[key1].success()) break;
          value = (const char *)jsonData[key1];
          lkey=key1;
        }
      }
      value.trim();
      char sbuffer[128];

      if (!strncmp(lkey.c_str(),"Epoch",5)) {
        uint32_t ep=atoi(value.c_str())-(uint32_t)EPOCH_OFFSET;
        snprintf_P(sbuffer, sizeof(sbuffer), PSTR(">%s=%d\n"), event_item.Event.c_str(),ep);
      } else {
        snprintf_P(sbuffer, sizeof(sbuffer), PSTR(">%s=\"%s\"\n"), event_item.Event.c_str(), value.c_str());
      }
      //toLog(sbuffer);
      execute_script(sbuffer);
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
    char parameters[data_len+1];
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
    //AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Script: Subscribe command with parameters: %s, %s, %s."), event_name.c_str(), topic.c_str(), key.c_str());
    //event_name.toUpperCase();
    if (event_name.length() > 0 && topic.length() > 0) {
      //Search all subscriptions
      for (uint32_t index=0; index < subscriptions.size(); index++) {
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
      //AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Script: New topic: %s."), topic.c_str());
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
    for (uint32_t index=0; index < subscriptions.size(); index++) {
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



#ifdef USE_SCRIPT_WEB_DISPLAY

void Script_Check_HTML_Setvars(void) {

  if (!HttpCheckPriviledgedAccess()) { return; }

  if (Webserver->hasArg("sv")) {
    String stmp = Webserver->arg("sv");
    char cmdbuf[64];
    memset(cmdbuf,0,sizeof(cmdbuf));
    char *cp=cmdbuf;
    *cp++='>';
    strncpy(cp,stmp.c_str(),sizeof(cmdbuf)-1);
    char *cp1=strchr(cp,'_');
    if (!cp1) return;
    *cp1=0;
    char vname[32];
    strncpy(vname,cp,sizeof(vname));
    *cp1='=';
    cp1++;

    struct T_INDEX ind;
    uint8_t vtype;
    isvar(vname,&vtype,&ind,0,0,0);
    if (vtype!=NUM_RES && vtype&STYPE) {
      // string type must insert quotes
      uint8_t tlen=strlen(cp1);
      memmove(cp1+1,cp1,tlen);
      *cp1='\"';
      *(cp1+tlen+1)='\"';
    }

    //toLog(cmdbuf);
    execute_script(cmdbuf);
    Run_Scripter(">E",2,0);
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
  "<div><center><label><b>%s</b><input type='checkbox' %s onchange='seva(%d,\"%s\")'></label></div>";

const char SCRIPT_MSG_TEXTINP[] PROGMEM =
  "<div><center><label><b>%s</b><input type='text'  value='%s' style='width:200px'  onfocusin='pr(0)' onfocusout='pr(1)' onchange='siva(value,\"%s\")'></label></div>";

const char SCRIPT_MSG_NUMINP[] PROGMEM =
  "<div><center><label><b>%s</b><input  min='%s' max='%s' step='%s' value='%s' type='number' style='width:200px' onfocusin='pr(0)' onfocusout='pr(1)' onchange='siva(value,\"%s\")'></label></div>";


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

//#define CHART_EXTRA_OPTIONS ",width:'640px',height:'480px'"
#define CHART_EXTRA_OPTIONS

const char SCRIPT_MSG_GTABLEb[] PROGMEM =
 "]);"
 "var options={%s" CHART_EXTRA_OPTIONS "};"
 "var chart=new google.visualization.%s(document.getElementById('chart%1d'));"
 "chart.draw(data,options);}"
 "google.charts.setOnLoadCallback(drawChart);</script>";

const char SCRIPT_MSG_GOPT1[] PROGMEM =
"title:'%s',isStacked:false";

const char SCRIPT_MSG_GAUGEOPT[] PROGMEM =
"max:%d,redFrom:%d,redTo:%d,yellowFrom:%d,yellowTo:%d";

const char SCRIPT_MSG_GOPT2[] PROGMEM =
"showRowNumber:true,sort:'disable',allowHtml:true,width:'100%%',height:'100%%',cssClassNames:cssc";

const char SCRIPT_MSG_GOPT3[] PROGMEM =
"title:'%s',isStacked:false,vAxes:{0:{maxValue:%d},1:{maxValue:%d}},series:{0:{targetAxisIndex:0},1:{targetAxisIndex:1}}%s";

const char SCRIPT_MSG_GOPT4[] PROGMEM =
//"hAxis:{minValue:new Date(0,1,1,0,0),maxValue:new Date(0,1,2,0,0),format:'HH:mm'}";
"hAxis:{minValue:new Date(0,1,1,0,0),maxValue:new Date(0,1,2,0,0),format:'HH:mm'},theme: 'maximized'";

const char SCRIPT_MSG_GOPT5[] PROGMEM =
"new Date(0,1,1,%d,%d)";

const char SCRIPT_MSG_GTE1[] PROGMEM = "'%s'";

#define GLIBS_MAIN 1<<0
#define GLIBS_TABLE 1<<1
#define GLIBS_GAUGE 1<<2
#define GLIBS_TIMELINE 1<<3

#define MAX_GARRAY 4

char *gc_get_arrays(char *lp, float **arrays, uint8_t *ranum, uint8_t *rentries, uint8_t *ipos) {
struct T_INDEX ind;
uint8_t vtype;
uint8 entries=0;
uint8_t cipos=0;

  uint8_t anum=0;
  while (anum<MAX_GARRAY) {
    if (*lp==')' || *lp==0) break;
    char *lp1=lp;
    float sysvar;
    lp=isvar(lp,&vtype,&ind,&sysvar,0,0);
    if (vtype!=VAR_NV) {
      SCRIPT_SKIP_SPACES
      uint8_t index=glob_script_mem.type[ind.index].index;
      if ((vtype&STYPE)==0) {
        // numeric result
        //Serial.printf("numeric %d - %d \n",ind.index,index);
        if (glob_script_mem.type[ind.index].bits.is_filter) {
          //Serial.printf("numeric array\n");
          uint8_t len=0;
          float *fa=Get_MFAddr(index,&len,&cipos);
          //Serial.printf(">> 2 %d\n",(uint32_t)*fa);
          if (fa && len>=entries) {
            if (!entries) {
              entries = len;
            }
            // add array to list
            arrays[anum]=fa;
            anum++;
          }
        } else {
          // single numeric
          arrays[anum]=&glob_script_mem.fvars[index];
          anum++;
          entries=1;
        }
      } else {
        lp=lp1;
        break;
      }
    }
  }
  //Serial.printf(">> %d - %d - %d\n",anum,entries,(uint32_t)*arrays[0]);
  *ranum=anum;
  *rentries=entries;
  *ipos=cipos;
  return lp;
}

char *gc_send_labels(char *lp,uint32_t anum) {
  WSContentSend_PD("[");
  for (uint32_t cnt=0; cnt<anum+1; cnt++) {
    char label[SCRIPT_MAXSSIZE];
    lp=GetStringResult(lp,OPER_EQU,label,0);
    SCRIPT_SKIP_SPACES
    WSContentSend_PD(SCRIPT_MSG_GTE1,label);
    //Serial.printf("labels %s\n",label);
    if (cnt<anum) { WSContentSend_PD(","); }
  }
  WSContentSend_PD("],");
  return lp;
}

#endif // USE_GOOGLE_CHARTS

void ScriptGetVarname(char *nbuf,char *sp, uint32_t blen) {
uint32_t cnt;
  for (cnt=0;cnt<blen-1;cnt++) {
    if (*sp==' ' || *sp==')') {
      break;
    }
    nbuf[cnt]=*sp++;
  }
  nbuf[cnt]=0;
}

void ScriptWebShow(char mc) {
  uint8_t web_script=Run_Scripter(">W",-2,0);
  if (web_script==99) {
    char tmp[256];
    uint8_t optflg=0;
    uint8_t chartindex=1;
    uint8_t google_libs=0;
    char *lp=glob_script_mem.section_ptr+2;
    while (lp) {
      while (*lp==SCRIPT_EOL) {
       lp++;
      }
      if (!*lp || *lp=='#' || *lp=='>') {
          break;
      }
      if (*lp!=';') {
        // send this line to web
        Replace_Cmd_Vars(lp,1,tmp,sizeof(tmp));
        char *lin=tmp;
        if (!mc && (*lin!='$')) {
          // normal web section
          if (*lin=='@') {
            lin++;
            optflg=1;
          } else {
            optflg=0;
          }
          // check for input elements
          if (!strncmp(lin,"sl(",3)) {
            // insert slider sl(min max var left mid right)
            char *lp=lin;
            float min;
            lp=GetNumericResult(lp+3,OPER_EQU,&min,0);
            SCRIPT_SKIP_SPACES
            // arg2
            float max;
            lp=GetNumericResult(lp,OPER_EQU,&max,0);
            SCRIPT_SKIP_SPACES
            float val;
            char *slp=lp;
            lp=GetNumericResult(lp,OPER_EQU,&val,0);
            SCRIPT_SKIP_SPACES

            char vname[16];
            ScriptGetVarname(vname,slp,sizeof(vname));

            char left[SCRIPT_MAXSSIZE];
            lp=GetStringResult(lp,OPER_EQU,left,0);
            SCRIPT_SKIP_SPACES
            char mid[SCRIPT_MAXSSIZE];
            lp=GetStringResult(lp,OPER_EQU,mid,0);
            SCRIPT_SKIP_SPACES
            char right[SCRIPT_MAXSSIZE];
            lp=GetStringResult(lp,OPER_EQU,right,0);
            SCRIPT_SKIP_SPACES

            WSContentSend_PD(SCRIPT_MSG_SLIDER,left,mid,right,(uint32_t)min,(uint32_t)max,(uint32_t)val,vname);

          } else if (!strncmp(lin,"ck(",3)) {
            char *lp=lin+3;
            char *slp=lp;
            float val;
            lp=GetNumericResult(lp,OPER_EQU,&val,0);
            SCRIPT_SKIP_SPACES

            char vname[16];
            ScriptGetVarname(vname,slp,sizeof(vname));

            char label[SCRIPT_MAXSSIZE];
            lp=GetStringResult(lp,OPER_EQU,label,0);
            const char *cp;
            uint8_t uval;
            if (val>0) {
              cp="checked='checked'";
              uval=0;
            } else {
              cp="";
              uval=1;
            }
            WSContentSend_PD(SCRIPT_MSG_CHKBOX,label,(char*)cp,uval,vname);

          } else if (!strncmp(lin,"bu(",3)) {
            char *lp=lin+3;
            uint8_t bcnt=0;
            char *found=lin;
            while (bcnt<4) {
              found=strstr(found,"bu(");
              if (!found) break;
              found+=3;
              bcnt++;
            }
            uint8_t proz=100/bcnt;
            if (!optflg && bcnt>1) proz-=2;
            if (optflg) WSContentSend_PD(SCRIPT_MSG_BUT_START_TBL);
            else WSContentSend_PD(SCRIPT_MSG_BUT_START);
            for (uint32_t cnt=0;cnt<bcnt;cnt++) {
              float val;
              char *slp=lp;
              lp=GetNumericResult(lp,OPER_EQU,&val,0);
              SCRIPT_SKIP_SPACES

              char vname[16];
              ScriptGetVarname(vname,slp,sizeof(vname));

              SCRIPT_SKIP_SPACES
              char ontxt[SCRIPT_MAXSSIZE];
              lp=GetStringResult(lp,OPER_EQU,ontxt,0);
              SCRIPT_SKIP_SPACES
              char offtxt[SCRIPT_MAXSSIZE];
              lp=GetStringResult(lp,OPER_EQU,offtxt,0);

              char *cp;
              uint8_t uval;
              if (val>0) {
                cp=ontxt;
                uval=0;
              } else {
                cp=offtxt;
                uval=1;
              }
              if (bcnt>1 && cnt==bcnt-1) {
                if (!optflg) proz+=2;
              }
              if (!optflg) {
                WSContentSend_PD(SCRIPT_MSG_BUTTONa,proz,uval,vname,cp);
              } else {
                WSContentSend_PD(SCRIPT_MSG_BUTTONa_TBL,proz,uval,vname,cp);
              }
              if (bcnt>1 && cnt<bcnt-1) {
                if (!optflg) WSContentSend_PD(SCRIPT_MSG_BUTTONb,2);
              }
              lp+=4;
            }
            if (optflg) WSContentSend_PD(SCRIPT_MSG_BUT_STOP_TBL);
            else WSContentSend_PD(SCRIPT_MSG_BUT_STOP);

          } else if (!strncmp(lin,"tx(",3)) {
            char *lp=lin+3;
            char *slp=lp;
            char str[SCRIPT_MAXSSIZE];
            lp=ForceStringVar(lp,str);
            SCRIPT_SKIP_SPACES
            char label[SCRIPT_MAXSSIZE];
            lp=GetStringResult(lp,OPER_EQU,label,0);

            char vname[16];
            ScriptGetVarname(vname,slp,sizeof(vname));

            WSContentSend_PD(SCRIPT_MSG_TEXTINP,label,str,vname);

          } else if (!strncmp(lin,"nm(",3)) {
            char *lp=lin;
            float min;
            lp=GetNumericResult(lp+3,OPER_EQU,&min,0);
            SCRIPT_SKIP_SPACES
            float max;
            lp=GetNumericResult(lp,OPER_EQU,&max,0);
            SCRIPT_SKIP_SPACES
            float step;
            lp=GetNumericResult(lp,OPER_EQU,&step,0);
            SCRIPT_SKIP_SPACES
            float val;
            char *slp=lp;
            lp=GetNumericResult(lp,OPER_EQU,&val,0);
            SCRIPT_SKIP_SPACES
            char vname[16];
            ScriptGetVarname(vname,slp,sizeof(vname));

            char label[SCRIPT_MAXSSIZE];
            lp=GetStringResult(lp,OPER_EQU,label,0);

            char vstr[16],minstr[16],maxstr[16],stepstr[16];
            dtostrfd(val,4,vstr);
            dtostrfd(min,4,minstr);
            dtostrfd(max,4,maxstr);
            dtostrfd(step,4,stepstr);
            WSContentSend_PD(SCRIPT_MSG_NUMINP,label,minstr,maxstr,stepstr,vstr,vname);

          } else {
            if (optflg) {
              WSContentSend_PD(PSTR("<div>%s</div>"),tmp);
            } else {
              WSContentSend_PD(PSTR("{s}%s{e}"),tmp);
            }
          }
          // end standard web interface
        } else {
          //  main section interface
          if (*lin==mc) {
#ifdef USE_GOOGLE_CHARTS
            lin++;
            char *lp;
            if (!strncmp(lin,"gc(",3)) {
                // get google table
              lp=lin+3;
              SCRIPT_SKIP_SPACES
              const char *type;
              const char *func;
              char options[312];
              uint8_t nanum=MAX_GARRAY;
              uint8_t y2f=0;
              char ctype;
              ctype=*lp;
              lp++;
              if (!(google_libs&GLIBS_MAIN)) {
                google_libs|=GLIBS_MAIN;
                WSContentSend_PD(SCRIPT_MSG_GTABLE);
              }
              switch (ctype) {
                  case 'l':
                    type=PSTR("LineChart");
                    break;
                  case 'b':
                    type=PSTR("BarChart");
                    break;
                  case 'p':
                    type=PSTR("PieChart");
                    break;
                  case 'g':
                    type=PSTR("Gauge");
                    if (!(google_libs&GLIBS_GAUGE)) {
                      google_libs|=GLIBS_GAUGE;
                      WSContentSend_PD(SCRIPT_MSG_GAUGE);
                    }
                    break;
                  case 't':
                    type=PSTR("Table");
                    if (!(google_libs&GLIBS_TABLE)) {
                      google_libs|=GLIBS_TABLE;
                      WSContentSend_PD(SCRIPT_MSG_TABLE);
                    }
                    break;
                  case 'T':
                    type=PSTR("Timeline");
                    if (!(google_libs&GLIBS_TIMELINE)) {
                      google_libs|=GLIBS_TIMELINE;
                      WSContentSend_PD(SCRIPT_MSG_TIMELINE);
                    }
                    break;
                  case 'h':
                    type=PSTR("Histogram");
                    break;
                  case 'c':
                    type=PSTR("ColumnChart");
                    break;
                  default:
                    // error
                    goto nextwebline;
                    break;
              }
              if (ctype=='l' && *lp=='f') {
                lp++;
                func=PSTR(",curveType:'function'");
              } else {
                func="";
              }
              if (*lp=='2') {
                lp++;
                nanum=2;
                y2f=1;
              }
              SCRIPT_SKIP_SPACES

              //Serial.printf("type %d\n",ctype);

              float *arrays[MAX_GARRAY];
              uint8_t anum=0;
              uint8 entries=0;
              uint8 ipos=0;
              lp=gc_get_arrays(lp, &arrays[0], &anum, &entries, &ipos);

              if (anum>nanum) {
                goto nextwebline;
              }
              // we know how many arrays and the number of entries
              //Serial.printf("arrays %d\n",anum);
              //Serial.printf("entries %d\n",entries);
              if (ctype=='T') {
                if (anum && !(entries&1)) {
                  WSContentSend_PD(SCRIPT_MSG_GTABLEa);
                  WSContentSend_PD(SCRIPT_MSG_GTABLEd);
                  char label[SCRIPT_MAXSSIZE];
                  lp=GetStringResult(lp,OPER_EQU,label,0);
                  SCRIPT_SKIP_SPACES
                  for (uint32_t ind=0; ind<anum; ind++) {
                    char lbl[16];
                    GetTextIndexed(lbl, sizeof(lbl), ind, label);
                    for (uint32_t cnt=0; cnt<entries; cnt+=2) {
                      WSContentSend_PD("['%s',",lbl);
                      float *fp=arrays[ind];
                      uint32_t time=fp[cnt];
                      WSContentSend_PD(SCRIPT_MSG_GOPT5,time/60,time%60);
                      WSContentSend_PD(",");
                      time=fp[cnt+1];
                      WSContentSend_PD(SCRIPT_MSG_GOPT5,time/60,time%60);
                      WSContentSend_PD("]");
                      if (cnt<entries-2) { WSContentSend_PD(","); }
                    }
                    if (ind<anum-1) { WSContentSend_PD(","); }
                  }
                  snprintf_P(options,sizeof(options),SCRIPT_MSG_GOPT4);
                }
              } else {
                // we need to fetch the labels now
                WSContentSend_PD(SCRIPT_MSG_GTABLEa);
                lp=gc_send_labels(lp,anum);

                // now we have to export the values
                // fetch label part only once in combo string
                char label[SCRIPT_MAXSSIZE];
                lp=GetStringResult(lp,OPER_EQU,label,0);
                SCRIPT_SKIP_SPACES

                int8_t todflg=-1;
                if (!strncmp(label,"cnt",3)) {
                  todflg=atoi(&label[3]);
                  if (todflg>=entries) todflg=entries-1;
                }

                uint32_t aind=ipos;
                if (aind>=entries) aind=entries-1;
                for (uint32_t cnt=0; cnt<entries; cnt++) {
                  WSContentSend_PD("['");
                  char lbl[16];
                  if (todflg>=0) {
                    sprintf(lbl,"%d",todflg);
                    todflg++;
                    if (todflg>=entries) {
                      todflg=0;
                    }
                  } else {
                    GetTextIndexed(lbl, sizeof(lbl), aind, label);
                  }
                  WSContentSend_PD(lbl);
                  WSContentSend_PD("',");
                  for (uint32_t ind=0; ind<anum; ind++) {
                    char acbuff[32];
                    float *fp=arrays[ind];
                    dtostrfd(fp[aind],glob_script_mem.script_dprec,acbuff);
                    WSContentSend_PD("%s",acbuff);
                    if (ind<anum-1) { WSContentSend_PD(","); }
                  }
                  WSContentSend_PD("]");
                  if (cnt<entries-1) { WSContentSend_PD(","); }
                  aind++;
                  if (aind>=entries) {
                    aind=0;
                  }
                }

                // get header
                char header[SCRIPT_MAXSSIZE];
                lp=GetStringResult(lp,OPER_EQU,header,0);
                SCRIPT_SKIP_SPACES

                switch (ctype) {
                  case 't':
                    snprintf_P(options,sizeof(options),SCRIPT_MSG_GOPT2);
                    break;
                  default:
                    snprintf_P(options,sizeof(options),SCRIPT_MSG_GOPT1,header);
                    break;
                }
                // check for 2 axis option
                if (y2f) {
                  // 2 y axes variant
                  SCRIPT_SKIP_SPACES
                  float max1;
                  lp=GetNumericResult(lp,OPER_EQU,&max1,0);
                  SCRIPT_SKIP_SPACES
                  float max2;
                  lp=GetNumericResult(lp,OPER_EQU,&max2,0);
                  SCRIPT_SKIP_SPACES
                  snprintf_P(options,sizeof(options),SCRIPT_MSG_GOPT3,header,(uint32_t)max1,(uint32_t)max2,func);
                }

                if (ctype=='g') {
                  float yellowFrom;
                  lp=GetNumericResult(lp,OPER_EQU,&yellowFrom,0);
                  SCRIPT_SKIP_SPACES
                  float redFrom;
                  lp=GetNumericResult(lp,OPER_EQU,&redFrom,0);
                  SCRIPT_SKIP_SPACES
                  float maxValue;
                  lp=GetNumericResult(lp,OPER_EQU,&maxValue,0);
                  SCRIPT_SKIP_SPACES
                  float redTo=maxValue;
                  float yellowTo=redFrom;
                  snprintf_P(options,sizeof(options),SCRIPT_MSG_GAUGEOPT,(uint32_t)maxValue,(uint32_t)redFrom,(uint32_t)redTo,
                  (uint32_t)yellowFrom,(uint32_t)yellowTo);
                }
              }
              WSContentSend_PD(SCRIPT_MSG_GTABLEb,options,type,chartindex);
              chartindex++;
            } else {
              WSContentSend_PD(PSTR("%s"),lin);
            }
#else
          } else {
          //  WSContentSend_PD(PSTR("%s"),lin);
#endif //USE_GOOGLE_CHARTS
          }
        }
      }
nextwebline:
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
#endif //USE_SCRIPT_WEB_DISPLAY


#ifdef USE_SENDMAIL

void script_send_email_body(void(*func)(char *)) {
uint8_t msect=Run_Scripter(">m",-2,0);
  if (msect==99) {
    char tmp[256];
    char *lp=glob_script_mem.section_ptr+2;
    while (lp) {
      while (*lp==SCRIPT_EOL) {
       lp++;
      }
      if (!*lp || *lp=='#' || *lp=='>') {
          break;
      }
      if (*lp!=';') {
        // send this line to smtp
        Replace_Cmd_Vars(lp,1,tmp,sizeof(tmp));
        //client->println(tmp);
        func(tmp);
      }
      if (*lp==SCRIPT_EOL) {
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
#endif

#ifdef USE_SCRIPT_JSON_EXPORT
void ScriptJsonAppend(void) {
  uint8_t web_script=Run_Scripter(">J",-2,0);
  if (web_script==99) {
    char tmp[256];
    char *lp=glob_script_mem.section_ptr+2;
    while (lp) {
      while (*lp==SCRIPT_EOL) {
       lp++;
      }
      if (!*lp || *lp=='#' || *lp=='>') {
          break;
      }
      if (*lp!=';') {
        // send this line to mqtt
        Replace_Cmd_Vars(lp,1,tmp,sizeof(tmp));
        ResponseAppend_P(PSTR("%s"),tmp);
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


bool RulesProcessEvent(char *json_event) {
  if (bitRead(Settings.rule_enabled, 0)) Run_Scripter(">E",2,json_event);
}

#ifdef ESP32
#ifdef USE_SCRIPT_TASK

#ifndef STASK_STACK
#define STASK_STACK 8192
#endif

#ifndef STASK_PRIO
#define STASK_PRIO 1
#endif

#if 1

struct ESP32_Task {
  uint16_t task_timer;
  TaskHandle_t task_t;
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
    delay(esp32_tasks[0].task_timer);
    if (bitRead(Settings.rule_enabled, 0)) {
      Run_Scripter(">t1",3,0);
    }
  }
}

void script_task2(void *arg) {
  //uint32_t lastms=millis();
  //uint32_t time;
  while (1) {
    //time=millis()-lastms;
    //lastms=millis();
    //time=esp32_tasks[1].task_timer-time;
    //if (time<esp32_tasks[1].task_timer) {delay(time); }
    //if (time<=esp32_tasks[1].task_timer) {vTaskDelay( pdMS_TO_TICKS( time ) ); }
    delay(esp32_tasks[1].task_timer);
    if (bitRead(Settings.rule_enabled, 0)) {
      Run_Scripter(">t2",3,0);
    }
  }
}
uint32_t scripter_create_task(uint32_t num, uint32_t time, uint32_t core) {
  //return 0;
  BaseType_t res = 0;
  if (core > 1) { core = 1; }
  if (num == 1) {
    if (esp32_tasks[0].task_t) { vTaskDelete(esp32_tasks[0].task_t); }
    res = xTaskCreatePinnedToCore(script_task1, "T1", STASK_STACK, NULL, STASK_PRIO, &esp32_tasks[0].task_t, core);
    esp32_tasks[0].task_timer = time;
  } else {
    if (esp32_tasks[1].task_t) { vTaskDelete(esp32_tasks[1].task_t); }
    res = xTaskCreatePinnedToCore(script_task2, "T2", STASK_STACK, NULL, STASK_PRIO, &esp32_tasks[1].task_t, core);
    esp32_tasks[1].task_timer = time;
  }
  return res;
}
#else

uint16_t task_timer1;
uint16_t task_timer2;
TaskHandle_t task_t1;
TaskHandle_t task_t2;

void script_task1(void *arg) {
  while (1) {
    delay(task_timer1);
    Run_Scripter(">t1",3,0);
  }
}

void script_task2(void *arg) {
  while (1) {
    delay(task_timer2);
    Run_Scripter(">t2",3,0);
  }
}

uint32_t scripter_create_task(uint32_t num, uint32_t time, uint32_t core) {
  //return 0;
  BaseType_t res = 0;
  if (core > 1) { core = 1; }
  if (num == 1) {
    if (task_t1) { vTaskDelete(task_t1); }
    res = xTaskCreatePinnedToCore(script_task1, "T1", STASK_STACK, NULL, STASK_PRIO, &task_t1, core);
    task_timer1 = time;
  } else {
    if (task_t2) { vTaskDelete(task_t2); }
    res = xTaskCreatePinnedToCore(script_task2, "T2", STASK_STACK, NULL, STASK_PRIO, &task_t2, core);
    task_timer2 = time;
  }
  return res;
}
#endif

#endif // USE_SCRIPT_TASK
#endif // ESP32
/*********************************************************************************************\
 * Interface
\*********************************************************************************************/
//const esp_partition_t *esp32_part;

bool Xdrv10(uint8_t function)
{
  bool result = false;
  char *sprt;

  switch (function) {
    case FUNC_PRE_INIT:
      // set defaults to rules memory
      //bitWrite(Settings.rule_enabled,0,0);
      glob_script_mem.script_ram=Settings.rules[0];
      glob_script_mem.script_size=MAX_SCRIPT_SIZE;
      glob_script_mem.flags=0;
      glob_script_mem.script_pram=(uint8_t*)Settings.script_pram[0];
      glob_script_mem.script_pram_size=PMEM_SIZE;

#ifdef USE_UNISHOX_COMPRESSION
      int32_t len_decompressed;
      sprt=(char*)calloc(UNISHOXRSIZE+8,1);
      if (!sprt) { break; }
      glob_script_mem.script_ram=sprt;
      glob_script_mem.script_size=UNISHOXRSIZE;
      len_decompressed = SCRIPT_DECOMPRESS(Settings.rules[0], strlen(Settings.rules[0]), glob_script_mem.script_ram, glob_script_mem.script_size);
      if (len_decompressed>0) glob_script_mem.script_ram[len_decompressed]=0;
      //AddLog_P2(LOG_LEVEL_INFO, PSTR("decompressed script len %d"),len_decompressed);
#endif // USE_UNISHOX_COMPRESSION

#ifdef USE_BUTTON_EVENT
      for (uint32_t cnt=0;cnt<MAX_KEYS;cnt++) {
        script_button[cnt]=-1;
      }
#endif

#ifdef EEP_SCRIPT_SIZE
      if (eeprom_init(EEP_SCRIPT_SIZE)) {
          // found 32kb eeprom
          char *script;
          script=(char*)calloc(EEP_SCRIPT_SIZE+4,1);
          if (!script) break;
          glob_script_mem.script_ram=script;
          glob_script_mem.script_size=EEP_SCRIPT_SIZE;
          EEP_READ(0,EEP_SCRIPT_SIZE,script);
          if (*script==0xff) {
            memset(script,EEP_SCRIPT_SIZE,0);
          }
          script[EEP_SCRIPT_SIZE-1]=0;
          // use rules storage for permanent vars
          glob_script_mem.script_pram=(uint8_t*)Settings.rules[0];
          glob_script_mem.script_pram_size=MAX_SCRIPT_SIZE;

          glob_script_mem.flags=1;
      }
#endif // EEP_SCRIPT_SIZE


#ifdef USE_SCRIPT_FATFS

#if USE_SCRIPT_FATFS>=0
      // fs on SD card
#ifdef ESP32
      if (PinUsed(GPIO_SPI_MOSI) && PinUsed(GPIO_SPI_MISO) && PinUsed(GPIO_SPI_CLK)) {
          SPI.begin(Pin(GPIO_SPI_CLK),Pin(GPIO_SPI_MISO),Pin(GPIO_SPI_MOSI), -1);
      }
#endif // ESP32
      fsp = &SD;
      if (SD.begin(USE_SCRIPT_FATFS)) {
#else
        // fs on flash
      fsp = &LittleFS;
      if (fsp->begin()) {
#endif // USE_SCRIPT_FATFS>=0

        //fsp->dateTimeCallback(dateTime);

        glob_script_mem.script_sd_found=1;
        char *script;
        script=(char*)calloc(FAT_SCRIPT_SIZE+4,1);
        if (!script) break;
        glob_script_mem.script_ram=script;
        glob_script_mem.script_size=FAT_SCRIPT_SIZE;
        if (fsp->exists(FAT_SCRIPT_NAME)) {
          File file=fsp->open(FAT_SCRIPT_NAME,FILE_READ);
          file.read((uint8_t*)script,FAT_SCRIPT_SIZE);
          file.close();
        }
        script[FAT_SCRIPT_SIZE-1]=0;
        // use rules storage for permanent vars
        glob_script_mem.script_pram=(uint8_t*)Settings.rules[0];
        glob_script_mem.script_pram_size=MAX_SCRIPT_SIZE;

        glob_script_mem.flags=1;

      } else {
        glob_script_mem.script_sd_found=0;
      }
#endif // USE_SCRIPT_FATFS


#ifdef LITTLEFS_SCRIPT_SIZE

#ifdef ESP32
    // spiffs on esp32
    fsp = &SPIFFS;
    //esp32_part = esp_partition_find_first(ESP_PARTITION_TYPE_DATA,ESP_PARTITION_SUBTYPE_DATA_SPIFFS,NULL);
    //Serial.printf("address %d - %d - %s\n",esp32_part->address,esp32_part->size, esp32_part->label);
#else
    // lfs on esp8266
    fsp = &LittleFS;
#endif
    char *script;
    script=(char*)calloc(LITTLEFS_SCRIPT_SIZE+4,1);
    if (!script) break;
    LoadFile("/script.txt",(uint8_t*)script,LITTLEFS_SCRIPT_SIZE);

    glob_script_mem.script_ram=script;
    glob_script_mem.script_size=LITTLEFS_SCRIPT_SIZE;
    script[LITTLEFS_SCRIPT_SIZE-1]=0;
    // use rules storage for permanent vars
    glob_script_mem.script_pram=(uint8_t*)Settings.rules[0];
    glob_script_mem.script_pram_size=MAX_SCRIPT_SIZE;
    glob_script_mem.flags=1;
#endif // LITTLEFS_SCRIPT_SIZE

    // a valid script MUST start with >D
    if (glob_script_mem.script_ram[0]!='>' && glob_script_mem.script_ram[1]!='D') {
      // clr all
      memset(glob_script_mem.script_ram,0,glob_script_mem.script_size);
      strcpy_P(glob_script_mem.script_ram, PSTR(">D\nscript error must start with >D"));
      bitWrite(Settings.rule_enabled, 0, 0);
    }

      // assure permanent memory is 4 byte aligned
      { uint32_t ptr=(uint32_t)glob_script_mem.script_pram;
      ptr&=0xfffffffc;
      ptr+=4;
      glob_script_mem.script_pram=(uint8_t*)ptr;
      glob_script_mem.script_pram_size-=4;
      }

      if (bitRead(Settings.rule_enabled, 0)) Init_Scripter();
      break;
    case FUNC_INIT:
      if (bitRead(Settings.rule_enabled, 0)) {
        Run_Scripter(">B",2,0);
        fast_script=Run_Scripter(">F",-2,0);
#if defined(USE_SCRIPT_HUE) && defined(USE_WEBSERVER) && defined(USE_EMULATION) && defined(USE_EMULATION_HUE) && defined(USE_LIGHT)
        Script_Check_Hue(0);
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
      if (bitRead(Settings.rule_enabled, 0)) Run_Scripter(">P",2,0);
#else
      if (bitRead(Settings.rule_enabled, 0)) Run_Scripter(">E",2,0);
#endif
      break;
    case FUNC_RULES_PROCESS:
      if (bitRead(Settings.rule_enabled, 0)) Run_Scripter(">E",2,mqtt_data);
      break;
#ifdef USE_WEBSERVER
    case FUNC_WEB_ADD_BUTTON:
      WSContentSend_P(HTTP_BTN_MENU_RULES);
      break;
#ifdef USE_SCRIPT_WEB_DISPLAY
    case FUNC_WEB_ADD_MAIN_BUTTON:
      if (bitRead(Settings.rule_enabled, 0)) {
        ScriptWebShow('$');
      }
      break;
#endif // USE_SCRIPT_WEB_DISPLAY
    case FUNC_WEB_ADD_HANDLER:
      Webserver->on("/" WEB_HANDLE_SCRIPT, HandleScriptConfiguration);
      Webserver->on("/ta",HTTP_POST, HandleScriptTextareaConfiguration);
      Webserver->on("/exs", HTTP_POST,[]() { Webserver->sendHeader("Location","/exs");Webserver->send(303);},script_upload_start);
      Webserver->on("/exs", HTTP_GET,ScriptExecuteUploadSuccess);

#ifdef USE_SCRIPT_FATFS
      Webserver->on("/u3", HTTP_POST,[]() { Webserver->sendHeader("Location","/u3");Webserver->send(303);},script_upload);
      Webserver->on("/u3", HTTP_GET,ScriptFileUploadSuccess);
      Webserver->on("/upl", HTTP_GET,Script_FileUploadConfiguration);
#endif
      break;
#endif // USE_WEBSERVER
    case FUNC_SAVE_BEFORE_RESTART:
      if (bitRead(Settings.rule_enabled, 0)) {
        Run_Scripter(">R",2,0);
        Scripter_save_pvars();
      }
      break;
#ifdef SUPPORT_MQTT_EVENT
    case FUNC_MQTT_DATA:
      if (bitRead(Settings.rule_enabled, 0)) {
        result = ScriptMqttData();
      }
      break;
#endif    //SUPPORT_MQTT_EVENT
#ifdef USE_SCRIPT_WEB_DISPLAY
    case FUNC_WEB_SENSOR:
      if (bitRead(Settings.rule_enabled, 0)) {
        ScriptWebShow(0);
      }
      break;
#endif //USE_SCRIPT_WEB_DISPLAY

#ifdef USE_SCRIPT_JSON_EXPORT
    case FUNC_JSON_APPEND:
      if (bitRead(Settings.rule_enabled, 0)) {
        ScriptJsonAppend();
      }
      break;
#endif //USE_SCRIPT_JSON_EXPORT

#ifdef USE_BUTTON_EVENT
    case FUNC_BUTTON_PRESSED:
      if (bitRead(Settings.rule_enabled, 0)) {
        if ((script_button[XdrvMailbox.index]&1)!=(XdrvMailbox.payload&1)) {
          script_button[XdrvMailbox.index]=XdrvMailbox.payload;
          Run_Scripter(">b",2,0);
        }
      }
      break;
#endif

  }
  return result;
}


#endif  // Do not USE_RULES
#endif  // USE_SCRIPT
