/*
  xdrv_10_scripter.ino - script support for Sonoff-Tasmota

  Copyright (C) 2019  Gerhard Mutz and Theo Arends

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

uses about 14,2 k of flash
more stack could be needed for sendmail  => -D CONT_STACKSIZE=4800 = +0.8k stack -0.8k heap


to do
optimize code for space

remarks
goal is fast execution time, minimal use of ram and intuitive syntax
therefore =>
case sensitive cmds and vars (lowercase uses time and code)
no math hierarchy  (costs ram and execution time, better group with brackets, anyhow better readable for beginners)
(will probably make math hierarchy an ifdefed option)
keywords if then else endif, or, and are better readable for beginners (others may use {})

\*********************************************************************************************/

#define XDRV_10             10

#define SCRIPT_DEBUG 0

#define MAXVARS 50
#define MAXNVARS 45
#define MAXSVARS 5
#define MAXFILT 5
#define SCRIPT_SVARSIZE 20
#define SCRIPT_MAXSSIZE 48
#define SCRIPT_EOL '\n'
#define SCRIPT_FLOAT_PRECISION 2
#define SCRIPT_MAXPERM (MAX_RULE_MEMS*10)-4/sizeof(float)


enum {OPER_EQU=1,OPER_PLS,OPER_MIN,OPER_MUL,OPER_DIV,OPER_PLSEQU,OPER_MINEQU,OPER_MULEQU,OPER_DIVEQU,OPER_EQUEQU,OPER_NOTEQU,OPER_GRTEQU,OPER_LOWEQU,OPER_GRT,OPER_LOW,OPER_PERC,OPER_XOR,OPER_AND,OPER_OR,OPER_ANDEQU,OPER_OREQU,OPER_XOREQU,OPER_PERCEQU};
enum {SCRIPT_LOGLEVEL=1,SCRIPT_TELEPERIOD};

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
    uint8_t numvars;
    void *script_mem;
    uint16_t script_mem_size;
    uint8_t script_dprec;
    uint8_t var_not_found;
    uint8_t glob_error;
    uint8_t max_ssize;
    uint8_t script_loglevel;
} glob_script_mem;

uint8_t tasm_cmd_activ=0;

uint32_t script_lastmillis;

char *GetNumericResult(char *lp,uint8_t lastop,float *fp,JsonObject *jo);

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
  if (bitRead(Settings.rule_enabled, 0)) Run_Scripter(">T",2, mqtt_data);
}

#define SCRIPT_SKIP_SPACES while (*lp==' ' || *lp=='\t') lp++;
#define SCRIPT_SKIP_EOL while (*lp==SCRIPT_EOL) lp++;

// allocates all variable and presets them
int16_t Init_Scripter(char *script) {
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
                    fv=CharToDouble(op);
                    fvalues[nvars]=fv;
                    vtypes[vars].bits.is_string=0;
                    if (!vtypes[vars].bits.is_filter) vtypes[vars].index=nvars;
                    nvars++;
                    if (nvars>MAXNVARS) {
                      return -1;
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
    }

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
    uint32_t lptr=(uint32_t)Settings.mems[0];
    lptr&=0xfffffffc;
    float *fp=(float*)lptr;
    fp++;
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


#if SCRIPT_DEBUG>0
    ClaimSerial();
    SetSerialBaudrate(9600);
#endif

    // store start of actual program here
    glob_script_mem.scriptptr=lp-1;
    return 0;

}

#define NUM_RES 0xfe
#define STR_RES 0xfd
#define VAR_NV 0xff

#define NTYPE 0
#define STYPE 0x80


//Settings.seriallog_level
//Settings.weblog_level

float Get_MFilter(uint8_t index) {
  uint8_t *mp=(uint8_t*)glob_script_mem.mfilt;
  for (uint8_t count=0; count<MAXFILT; count++) {
    struct M_FILT *mflp=(struct M_FILT*)mp;
    if (count==index) {
      if (mflp->numvals&0x80) {
        // moving average
        return mflp->maccu/(mflp->numvals&0x7f);
      } else {
        // median, sort array
        float tbuff[mflp->numvals],tmp;
        uint8_t flag;
        memmove(tbuff,mflp->rbuff,sizeof(tbuff));
        for (uint8_t ocnt=0; ocnt<mflp->numvals; ocnt++) {
          flag=0;
          for (uint8_t count=0; count<mflp->numvals-1; count++) {
            if (tbuff[count]>tbuff[count+1]) {
              tmp=tbuff[count];
              tbuff[count]=tbuff[count+1];
              tbuff[count+1]=tmp;
              flag=1;
            }
          }
          if (!flag) break;
        }
        return mflp->rbuff[mflp->numvals/2];
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

  float tbuff[MEDIAN_SIZE],tmp;
  uint8_t flag;
  mf->buffer[mf->index]=in;
  mf->index++;
  if (mf->index>=MEDIAN_SIZE) mf->index=0;
  // sort list and take median
  memmove(tbuff,mf->buffer,sizeof(tbuff));
  for (uint8_t ocnt=0; ocnt<MEDIAN_SIZE; ocnt++) {
    flag=0;
    for (uint8_t count=0; count<MEDIAN_SIZE-1; count++) {
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
}


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
        if (fp) *fp=CharToDouble(lp);
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
        if (sp) *sp++=*lp;
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
    for (count=0; count<glob_script_mem.numvars; count++) {
        char *cp=glob_script_mem.glob_vnp+glob_script_mem.vnp_offset[count];
        uint8_t slen=strlen(cp);

        if (slen==len && *cp==vname[0]) {
            if (!strncmp(cp,vname,len)) {
                uint8_t index=vtp[count].index;
                *tind=vtp[count];
                tind->index=count; // overwrite with global var index
                if (vtp[count].bits.is_string==0) {
                    *vtype=NTYPE|index;
                    if (vtp[count].bits.is_filter) {
                      fvar=Get_MFilter(index);
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
        if (subtype) {
          *subtype=0;
          subtype++;
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
              } else if (!strncmp(str_value,"OFF",3)) {
                if (fp) *fp=0;
              } else {
                *vtype=STR_RES;
                tind->bits.constant=1;
                tind->bits.is_string=1;
                if (sp) strlcpy(sp,str_value,SCRIPT_MAXSSIZE);
                return lp+len;
              }
            } else {
              if (fp) *fp=CharToDouble((char*)str_value);
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
      case 'b':
        if (!strncmp(vname,"boot",4)) {
          if (rules_flag.system_boot) {
            rules_flag.system_boot=0;
            fvar=1;
          }
          goto exit;
        }
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
        break;
      case 'd':
        if (!strncmp(vname,"day",3)) {
          fvar=RtcTime.day_of_month;
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
          if (sp) strlcpy(sp,Settings.mqtt_grptopic,glob_script_mem.max_ssize);
          goto strexit;
        }
        break;
      case 'h':
        if (!strncmp(vname,"hours",5)) {
          fvar=RtcTime.hour;
          goto exit;
        }
        if (!strncmp(vname,"heap",4)) {
          fvar=ESP.getFreeHeap();
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
          fvar=pin[(uint8_t)fvar];
          // skip ] bracket
          len++;
          goto exit;
        }
        if (!strncmp(vname,"pd[",3)) {
          GetNumericResult(vname+3,OPER_EQU,&fvar,0);
          uint8_t gpiopin=fvar;
          for (uint8_t i=0;i<GPIO_SENSOR_END;i++) {
            if (pin[i]==gpiopin) {
              fvar=i;
              // skip ] bracket
              len++;
              goto exit;
            }
          }
          fvar=999;
          goto exit;
        }
        if (!strncmp(vname,"prefix1",7)) {
          if (sp) strlcpy(sp,Settings.mqtt_prefix[0],glob_script_mem.max_ssize);
          goto strexit;
        }
        if (!strncmp(vname,"prefix2",7)) {
          if (sp) strlcpy(sp,Settings.mqtt_prefix[1],glob_script_mem.max_ssize);
          goto strexit;
        }
        if (!strncmp(vname,"prefix3",7)) {
          if (sp) strlcpy(sp,Settings.mqtt_prefix[2],glob_script_mem.max_ssize);
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
          fvar=FastPrecisePow(fvar1,fvar2);
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
        break;
      case 'r':
        if (!strncmp(vname,"ram",3)) {
          fvar=glob_script_mem.script_mem_size+(MAX_RULE_SETS*MAX_RULE_SIZE)+(MAX_RULE_MEMS*10);
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
          fvar=SwitchLastState((uint8_t)fvar);
          // skip ] bracket
          len++;
          goto exit;
        }
        if (!strncmp(vname,"stack",5)) {
          fvar=GetStack();
          goto exit;
        }
        if (!strncmp(vname,"slen",4)) {
          fvar=strlen(Settings.rules[0]);
          goto exit;
        }
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
          if (sp) strlcpy(sp,Settings.mqtt_topic,glob_script_mem.max_ssize);
          goto strexit;
        }
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


#if defined(ARDUINO_ESP8266_RELEASE_2_3_0) || defined(ARDUINO_ESP8266_RELEASE_2_4_0) || defined(ARDUINO_ESP8266_RELEASE_2_4_1)
// All version before core 2.4.2
// https://github.com/esp8266/Arduino/issues/2557
extern "C" {
#include <cont.h>
  extern cont_t g_cont;
}
uint16_t GetStack(void) {
  register uint32_t *sp asm("a1");
  return (4 * (sp - g_pcont.stack));
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

char *GetStringResult(char *lp,uint8_t lastop,char *cp,JsonObject *jo) {
  uint8_t operand=0;
  uint8_t vtype;
  char *slp;
  struct T_INDEX ind;
  char str[SCRIPT_MAXSSIZE],str1[SCRIPT_MAXSSIZE];
  while (1) {
    lp=isvar(lp,&vtype,&ind,0,str1,jo);
    switch (lastop) {
        case OPER_EQU:
          strlcpy(str,str1,sizeof(str));
          break;
        case OPER_PLS:
          strlcat(str,str1,sizeof(str));
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
                fvar=fmod(fvar,fvar1);
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


// replace vars in cmd %var%
void Replace_Cmd_Vars(char *srcbuf,char *dstbuf,uint16_t dstsize) {
    char *cp;
    uint16_t count;
    uint8_t vtype;
    float fvar;
    cp=srcbuf;
    struct T_INDEX ind;
    char string[SCRIPT_MAXSSIZE];
    for (count=0;count<dstsize;count++) {
        if (*cp=='%') {
            cp++;
            if (*cp=='%') {
              dstbuf[count]=*cp++;
            } else {
              //char *scp=cp;
              cp=isvar(cp,&vtype,&ind,&fvar,string,0);
              if (vtype!=VAR_NV) {
                // found variable as result
                if (vtype==NUM_RES || (vtype&STYPE)==0) {
                    // numeric result
                    dtostrfd(fvar,glob_script_mem.script_dprec,string);
                } else {
                    // string result
                }
                strcpy(&dstbuf[count],string);
                count+=strlen(string)-1;
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
            dstbuf[count]=*cp;
            if (*cp==0) {
                break;
            }
            cp++;
        }
    }
}

void toLog(const char *str) {
  if (!str) return;
  snprintf_P(log_data, sizeof(log_data), PSTR("%s"),str);
  AddLog(LOG_LEVEL_INFO);
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

#define IF_NEST 8
// execute section of scripter
int16_t Run_Scripter(const char *type, uint8_t tlen, char *js) {
    uint8_t vtype=0,sindex,xflg,floop=0,globvindex;
    struct T_INDEX ind;
    uint8_t operand,lastop,numeric=1,if_state[IF_NEST],if_result[IF_NEST],and_or,ifstck=0,s_ifstck=0;
    if_state[ifstck]=0;
    if_result[ifstck]=0;
    char cmpstr[SCRIPT_MAXSSIZE];

    if (tasm_cmd_activ) return 0;


    float *dfvar,*cv_count,cv_max,cv_inc;
    char *cv_ptr;
    float fvar=0,fvar1,sysvar,swvar;
    uint8_t section=0,sysv_type=0,swflg=0;

    if (!glob_script_mem.scriptptr) {
      return -99;
    }

    DynamicJsonBuffer jsonBuffer; // on heap
    JsonObject &jobj=jsonBuffer.parseObject(js);
    JsonObject  *jo;
    if (js) jo=&jobj;
    else jo=0;

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
                section=0;
                break;
            }
            if (*lp=='#') {
                section=0;
                break;
            }
            glob_script_mem.var_not_found=0;

#if SCRIPT_DEBUG>0
            char tbuff[128];
            sprintf(tbuff,"stack=%d,state=%d,cmpres=%d line: ",ifstck,if_state[ifstck],if_result[ifstck]);
            toLogEOL(tbuff,lp);
#endif

            if (!strncmp(lp,"if",2)) {
                lp+=2;
                if (if_state[ifstck]>0) {
                  if (ifstck<IF_NEST-1) ifstck++;
                }
                if_state[ifstck]=1;
                and_or=0;
            } else if (!strncmp(lp,"then",4) && if_state[ifstck]==1) {
                lp+=4;
                if_state[ifstck]=2;
            } else if (!strncmp(lp,"else",4) && if_state[ifstck]==2) {
                lp+=4;
                if_state[ifstck]=3;
            } else if (!strncmp(lp,"endif",5) && if_state[ifstck]>=2) {
                lp+=5;
                if_state[ifstck]=0;
                if (ifstck>0) {
                  ifstck--;
                }
                s_ifstck=ifstck; // >>>>>
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
                  lp+=4;
                  iselse=1;
                  break;
                }
                lp++;
              }
              if (!iselse) {
                lp=slp;
                // endif
                if_state[ifstck]=0;
                if (ifstck>0) {
                  ifstck--;
                }
                s_ifstck=ifstck; // >>>>>
              }
            }

            if (!strncmp(lp,"for",3)) {
              // start for next loop, fetch 3 params
              // simple implementation, zero loop count not supported
              lp+=3;
              SCRIPT_SKIP_SPACES
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
                  SCRIPT_SKIP_EOL
                  cv_ptr=lp;
                  floop=1;
              } else {
                      // error
                  toLogEOL("for error",lp);
              }
            } else if (!strncmp(lp,"next",4) && floop>0) {
              // for next loop
              *cv_count+=cv_inc;
              if (*cv_count<=cv_max) {
                lp=cv_ptr;
              } else {
                lp+=4;
                floop=0;
              }
            }

            if (!strncmp(lp,"switch",6)) {
              lp+=6;
              SCRIPT_SKIP_SPACES
              lp=GetNumericResult(lp,OPER_EQU,&swvar,0);
              swflg=1;
            } else if (!strncmp(lp,"case",4) && swflg>0) {
              lp+=4;
              SCRIPT_SKIP_SPACES
              float cvar;
              lp=GetNumericResult(lp,OPER_EQU,&cvar,0);
              if (swvar!=cvar) {
                swflg=2;
              } else {
                swflg=1;
              }
            } else if (!strncmp(lp,"ends",4) && swflg>0) {
              lp+=4;
              swflg=0;
            }

            if (swflg==2) goto next_line;


            SCRIPT_SKIP_SPACES
            //SCRIPT_SKIP_EOL
            if (*lp==SCRIPT_EOL) {
             goto next_line;
            }
            //toLogN(lp,16);
            if (if_state[s_ifstck]==3 && if_result[s_ifstck]) goto next_line;
            if (if_state[s_ifstck]==2 && !if_result[s_ifstck]) goto next_line;

            s_ifstck=ifstck;

            if (!strncmp(lp,"break",5)) {
              if (floop) {
                // should break loop
                floop=0;
              } else {
                section=0;
              }
              break;
            } else if (!strncmp(lp,"dprec",5)) {
              lp+=5;
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
              lp=GetNumericResult(lp,OPER_EQU,&fvar,0);
              int8_t mode=fvar;
              pinMode(pinnr,mode&1);
              goto next_line;
            } else if (!strncmp(lp,"spin(",5)) {
              lp+=5;
              // set pin mode
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

            else if (!strncmp(lp,"=>",2)) {
                // execute cmd
                lp+=2;
                SCRIPT_SKIP_SPACES
                #define SCRIPT_CMDMEM 512
                char *cmdmem=(char*)malloc(SCRIPT_CMDMEM);
                if (cmdmem) {
                  char *cmd=cmdmem;
                  short count;
                  for (count=0; count<SCRIPT_CMDMEM/2-1; count++) {
                    //if (*lp=='\r' || *lp=='\n' || *lp=='}') {
                    if (!*lp || *lp=='\r' || *lp=='\n') {
                        cmd[count]=0;
                        break;
                    }
                    cmd[count]=*lp++;
                  }
                  //snprintf_P(log_data, sizeof(log_data), tmp);
                  //AddLog(LOG_LEVEL_INFO);
                  // replace vars in cmd
                  char *tmp=cmdmem+SCRIPT_CMDMEM/2;
                  Replace_Cmd_Vars(cmd,tmp,SCRIPT_CMDMEM/2);
                  //toSLog(tmp);

                  if (!strncmp(tmp,"print",5)) {
                    toLog(&tmp[5]);
                  } else {
                    snprintf_P(log_data, sizeof(log_data), PSTR("Script: performs \"%s\""), tmp);
                    AddLog(glob_script_mem.script_loglevel);
                    tasm_cmd_activ=1;
                    ExecuteCommand((char*)tmp, SRC_RULE);
                    tasm_cmd_activ=0;
                  }
                  if (cmdmem) free(cmdmem);
                }
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
                Run_Scripter(slp,plen,0);
                lp=slp;
                goto next_line;
            }

            // check for variable result
            if (if_state[s_ifstck]==1) {
              // compare
              dfvar=&fvar;
              glob_script_mem.glob_error=0;
              char *slp=lp;
              numeric=1;
              lp=GetNumericResult(lp,OPER_EQU,dfvar,0);
              if (glob_script_mem.glob_error==1) {
                // was string, not number
                lp=slp;
                numeric=0;
                // get the string
                lp=isvar(lp,&vtype,&ind,0,cmpstr,0);
              }
            } else {
              lp=isvar(lp,&vtype,&ind,&sysvar,0,0);
              if (vtype!=VAR_NV) {
                  // found variable as result
                  globvindex=ind.index; // save destination var index here
                  uint8_t index=glob_script_mem.type[ind.index].index;
                  if ((vtype&STYPE)==0) {
                      // numeric result
                      if (ind.bits.settable) {
                        dfvar=&sysvar;
                        sysv_type=ind.index;
                      } else {
                        dfvar=&glob_script_mem.fvars[index];
                        sysv_type=0;
                      }
                      numeric=1;
                  } else {
                      // string result
                      numeric=0;
                      sindex=index;
                  }
              }
            }
            // evaluate operand
            lp=getop(lp,&lastop);
            if (if_state[s_ifstck]==1) {
              if (numeric) {
                uint8_t res=0;
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

                if (!and_or) {
                    if_result[s_ifstck]=res;
                } else if (and_or==1) {
                    if_result[s_ifstck]|=res;
                } else {
                    if_result[s_ifstck]&=res;
                }
#if SCRIPT_DEBUG>0
                char tbuff[128];
                sprintf(tbuff,"p1=%d,p2=%d,cmpres=%d line: ",(int32_t)*dfvar,(int32_t)fvar1,if_result[s_ifstck]);
                toLogEOL(tbuff,lp);
#endif

              } else {
                // compare string
                char str[SCRIPT_MAXSSIZE];
                lp=GetStringResult(lp,OPER_EQU,str,0);
                if (lastop==OPER_EQUEQU || lastop==OPER_NOTEQU) {
                  uint8_t res=0;
                  res=strcmp(cmpstr,str);
                  if (lastop==OPER_EQUEQU) res=!res;
                  if (!and_or) {
                      if_result[s_ifstck]=res;
                  } else if (and_or==1) {
                      if_result[s_ifstck]|=res;
                  } else {
                      if_result[s_ifstck]&=res;
                  }
                }
              }
              SCRIPT_SKIP_SPACES
              if (*lp=='{' && if_state[ifstck]==1) {
                lp+=1; // then
                if_state[ifstck]=2;
              }
              goto next_line;
            } else {
              if (numeric) {
                char *slp=lp;
                glob_script_mem.glob_error=0;
                lp=GetNumericResult(lp,OPER_EQU,&fvar,jo);
                if (glob_script_mem.glob_error==1) {
                  // mismatch was string, not number
                  // get the string and convert to number
                  lp=isvar(slp,&vtype,&ind,0,cmpstr,jo);
                  fvar=CharToDouble(cmpstr);
                }
                switch (lastop) {
                    case OPER_EQU:
                        if (glob_script_mem.var_not_found) {
                          if (!js) toLog("var not found\n");
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
                        *dfvar=fmod(*dfvar,fvar);
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
                  Set_MFilter(glob_script_mem.type[globvindex].index,*dfvar);
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
                char str[SCRIPT_MAXSSIZE];
                char *slp=lp;
                lp=GetStringResult(lp,OPER_EQU,str,jo);
                if (!js && glob_script_mem.var_not_found) {
                  // mismatch
                  lp=GetNumericResult(slp,OPER_EQU,&fvar,0);
                  dtostrfd(fvar,6,str);
                  glob_script_mem.var_not_found=0;
                }

                if (!glob_script_mem.var_not_found) {
                  // var was changed
                  glob_script_mem.type[globvindex].bits.changed=1;
                  if (lastop==OPER_EQU) {
                    strlcpy(glob_script_mem.glob_snp+(sindex*glob_script_mem.max_ssize),str,glob_script_mem.max_ssize);
                  } else if (lastop==OPER_PLSEQU) {
                    strlcat(glob_script_mem.glob_snp+(sindex*glob_script_mem.max_ssize),str,glob_script_mem.max_ssize);
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
            // decode line
            if (*lp=='>' && tlen==1) {
              // called from cmdline
              lp++;
              section=1;
              goto startline;
            }
            if (!strncmp(lp,type,tlen)) {
                // found section
                section=1;
                // check for subroutine
                if (*type=='#') {
                  // check for parameter
                  type+=tlen;
                  if (*type=='(') {
                    float fparam;
                    numeric=1;
                    glob_script_mem.glob_error=0;
                    GetNumericResult((char*)type,OPER_EQU,&fparam,0);
                    if (glob_script_mem.glob_error==1) {
                      // was string, not number
                      numeric=0;
                      // get the string
                      GetStringResult((char*)type+1,OPER_EQU,cmpstr,0);
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
                              *dfvar=CharToDouble(cmpstr);
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
          if (!lp) break;
          lp++;
        }
    }
}

uint8_t script_xsns_index = 0;


void ScripterEvery100ms(void)
{
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
}

//mems[MAX_RULE_MEMS] is 50 bytes in 6.5
// can hold 11 floats or floats + strings
// should report overflow later
void Scripter_save_pvars(void) {
  uint32_t lptr=(uint32_t)Settings.mems[0];
  int16_t mlen=0;
  lptr&=0xfffffffc;
  float *fp=(float*)lptr;
  fp++;
  mlen+=sizeof(float);
  struct T_INDEX *vtp=glob_script_mem.type;
  for (uint8_t count=0; count<glob_script_mem.numvars; count++) {
    if (vtp[count].bits.is_permanent && !vtp[count].bits.is_string) {
      uint8_t index=vtp[count].index;
      mlen+=sizeof(float);
      if (mlen>MAX_RULE_MEMS*10) {
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
      if (mlen>MAX_RULE_MEMS*10) {
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
#define D_CONFIGURE_SCRIPT "Edit script"
#define D_RULEVARS "edit script"

const char S_CONFIGURE_SCRIPT[] PROGMEM = D_CONFIGURE_SCRIPT;

const char HTTP_BTN_MENU_RULES[] PROGMEM =
  "<p><form action='" WEB_HANDLE_SCRIPT "' method='get'><button>" D_CONFIGURE_SCRIPT "</button></form></p>";


const char HTTP_FORM_SCRIPT[] PROGMEM =
    "<fieldset><legend><b>&nbsp;" D_RULEVARS "&nbsp;</b></legend>"
    "<form method='post' action='" WEB_HANDLE_SCRIPT "'>";

const char HTTP_FORM_SCRIPT1[] PROGMEM =
    "<br/><input style='width:10%%;' id='c%d' name='c%d' type='checkbox'%s><b>script enable</b><br/>"
    "<br><textarea  id='t%1d' name='t%d' rows='8' cols='80' maxlength='%d' style='font-size: 12pt' >";

const char HTTP_FORM_SCRIPT1b[] PROGMEM =
    "</textarea>";

void HandleScriptConfiguration(void)
{
    if (!HttpCheckPriviledgedAccess()) { return; }

    AddLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, S_CONFIGURE_SCRIPT);

    if (WebServer->hasArg("save")) {
      ScriptSaveSettings();
      HandleConfiguration();
      return;
    }

    WSContentStart_P(S_CONFIGURE_SCRIPT);
    WSContentSendStyle();
    WSContentSend_P(HTTP_FORM_SCRIPT);
    WSContentSend_P(HTTP_FORM_SCRIPT1,1,1,bitRead(Settings.rule_enabled,0) ? " checked" : "",1,1,MAX_RULE_SIZE*3);

    // script is to larg for WSContentSend_P
    if (Settings.rules[0][0]) {
      _WSContentSend(Settings.rules[0]);
    }
    WSContentSend_P(HTTP_FORM_SCRIPT1b);
    WSContentSend_P(HTTP_FORM_END);
    WSContentSpaceButton(BUTTON_CONFIGURATION);
    WSContentStop();
  }


void strrepl_inplace(char *str, const char *a, const char *b) {
  for (char *cursor=str; (cursor=strstr(cursor,a)) != NULL;) {
    memmove(cursor+strlen(b),cursor+strlen(a),strlen(cursor)-strlen(a)+1);
    for (int i=0; b[i]!='\0'; i++) {
      cursor[i] = b[i];
    }
    cursor += strlen(b);
  }
}

#define MAX_SCRIPT_SIZE MAX_RULE_SIZE*3

void ScriptSaveSettings(void) {

  if (WebServer->hasArg("c1")) {
    bitWrite(Settings.rule_enabled,0,1);
  } else {
    bitWrite(Settings.rule_enabled,0,0);
  }

  String str = WebServer->arg("t1");

  if (*str.c_str()) {
#if 1
    strrepl_inplace((char*)str.c_str(),"\r\n","\n");
    strrepl_inplace((char*)str.c_str(),"\r","\n");
#else
    str.replace("\r\n","\n");
    str.replace("\r","\n");
#endif
    strlcpy(Settings.rules[0],str.c_str(), MAX_RULE_SIZE*3);
  }

  if (glob_script_mem.script_mem) {
    Scripter_save_pvars();
    free(glob_script_mem.script_mem);
    glob_script_mem.script_mem=0;
    glob_script_mem.script_mem_size=0;
  }

  if (bitRead(Settings.rule_enabled, 0)) {
    int16_t res=Init_Scripter(Settings.rules[0]);
    if (res) {
      snprintf_P(log_data, sizeof(log_data), PSTR("script init error: %d"),res);
      AddLog(LOG_LEVEL_INFO);
      return;
    }
    Run_Scripter(">B",2,0);
  }
}

#endif

void execute_script(char *script) {
char *svd_sp=glob_script_mem.scriptptr;
  strcat(script,"\n#");
  glob_script_mem.scriptptr=script;
  Run_Scripter(">",1,0);
  glob_script_mem.scriptptr=svd_sp;
  Scripter_save_pvars();
}

enum ScriptCommands { CMND_SCRIPT };
const char kScriptCommands[] PROGMEM = "Script";

bool ScriptCommand(void) {
  char command[CMDSZ];
  bool serviced = true;
  uint8_t index = XdrvMailbox.index;

  int command_code = GetCommandCode(command, sizeof(command), XdrvMailbox.topic, kScriptCommands);
  if (-1 == command_code) {
    serviced = false;  // Unknown command
  }
  else if ((CMND_SCRIPT == command_code) && (index > 0)) {

    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 2)) {
      switch (XdrvMailbox.payload) {
        case 0: // Off
        case 1: // On
        bitWrite(Settings.rule_enabled, index -1, XdrvMailbox.payload);
      }
    } else {
      if ('>' == XdrvMailbox.data[0]) {
        // execute script
        for (uint8_t count=0; count<XdrvMailbox.data_len; count++) {
          if (XdrvMailbox.data[count]==';') XdrvMailbox.data[count]='\n';
        }
        execute_script(XdrvMailbox.data);
        /*
        for (uint8_t count=0; count<XdrvMailbox.data_len; count++) {
          if (XdrvMailbox.data[count]=='\n') XdrvMailbox.data[count]=';';
        }*/
      }
    }
    snprintf_P (mqtt_data, sizeof(mqtt_data), PSTR("{\"%s\":\"%s\",\"Free\":%d}"),command, GetStateText(bitRead(Settings.rule_enabled,0)),MAX_RULE_SIZE*3-strlen(Settings.rules[0]));
  } else serviced = false;

  return serviced;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv10(byte function)
{
  bool result = false;

  switch (function) {
    case FUNC_PRE_INIT:
      if (bitRead(Settings.rule_enabled, 0)) Init_Scripter(Settings.rules[0]);
      break;
    case FUNC_INIT:
      if (bitRead(Settings.rule_enabled, 0)) Run_Scripter(">B",2,0);
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
    case FUNC_RULES_PROCESS:
      if (bitRead(Settings.rule_enabled, 0)) Run_Scripter(">E",2,mqtt_data);
      break;
#ifdef USE_WEBSERVER
    case FUNC_WEB_ADD_BUTTON:
      WSContentSend_P(HTTP_BTN_MENU_RULES);
      break;
    case FUNC_WEB_ADD_HANDLER:
      WebServer->on("/" WEB_HANDLE_SCRIPT, HandleScriptConfiguration);
      break;
#endif // USE_WEBSERVER
    case FUNC_SAVE_BEFORE_RESTART:
      if (bitRead(Settings.rule_enabled, 0)) {
        Run_Scripter(">R",2,0);
        Scripter_save_pvars();
      }
      break;
  }
  return result;
}

#endif  // Do not USE_RULES
#endif  // USE_SCRIPT
