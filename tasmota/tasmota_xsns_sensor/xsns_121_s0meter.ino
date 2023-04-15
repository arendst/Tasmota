#ifdef USE_S0_FLOW_METER
#define XSNS_121             121

struct tS0Flow {
  uint32_t pintime;
  uint32_t cnttime;
  uint32_t pincnt;
  uint32_t old_time;
  uint32_t* usage;
  float  rate;
  float* resolution;  // cubic meter per pulse
  uint8_t* type;
  int id;
  uint16_t* low_debounce;
  uint16_t* high_debounce;
  int pin; 
  uint8_t pinstate;
};
tS0Flow s0flow[4];

/*********************************************************************************************/
void S0FlowEvery250ms(int id)
{   tS0Flow* s=&s0flow[id];
    if(s->pincnt>0)
    { uint32_t td=s->cnttime-s->old_time;
      if(td>0)   // not if overflown
      { if(*s->type==1)  
            s->rate=(60000000.0*(float)(s->pincnt))/(float)(td)* (*(s->resolution)); // liter/min
        if(*s->type==2)  
            s->rate=(3600000.0*(float)(s->pincnt))/(float)(td)* (*(s->resolution)); // m3/h
      }
      *s->usage+=s->pincnt;
      s->old_time=s->cnttime;
      ETS_GPIO_INTR_DISABLE();
      s->pincnt=0;
      ETS_GPIO_INTR_ENABLE();
    }
    else
    { float r=0.0;
      uint32_t td=millis()-s->cnttime;
      if(*s->type==1)  
        r=(60000000.0)/(float)(td)* (*(s->resolution)); // liter/min
      if(*s->type==2)  
        r=(3600000.0)/(float)(td)* (*(s->resolution)); // m3/h
      if(*(s->resolution) <0.005 && r<s->rate*0.8) 
        s->rate=r;
      if(*(s->resolution) >=0.005 && r<s->rate*0.1) 
        s->rate=r;
      
      if(td>300000)   // if longer than 5min no pulse
        s->rate=0; 
    }  
}

void ICACHE_RAM_ATTR S0FlowIsrArg(void *arg) {
#if defined(ESP8266)
  volatile uint32_t gpio_status = GPIO_REG_READ(GPIO_STATUS_ADDRESS);
  GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, gpio_status);
#endif
  tS0Flow* s = static_cast<tS0Flow*>(arg);
  uint8_t p = digitalRead(s->pin);
  if(s->pinstate !=p)
  { 
    uint32_t t = millis();
    if(p==1)
    { if((t-s->pintime)>*s->high_debounce)  // debounce ms
      {
        s->pintime  = t;
        s->pinstate = p;
      }
    }
    else // p==0
    { if((t-s->pintime)>*s->low_debounce)  // debounce ms
      {
        s->pintime  = t;
        s->cnttime  = t;
        s->pinstate = p;
        s->pincnt++;
      }
    }
  }
}

void S0FlowShow(boolean json,int id)
{
  tS0Flow* s=&s0flow[id];
  const char *type = *s->type==1?D_WATER:D_GAS;
  char type_str[20];
  sprintf(type_str,"%s%d",type,id+1);

  char usage[16];
  char rate[16];
  float u=*s->resolution;
  u*=*s->usage;
  dtostrfd(u, 3, usage);
  dtostrfd(s->rate, 3, rate);

  if (json)
  {
    ResponseAppend_P(JSON_SNS_GNGPM, type_str, usage, rate);
  }
#ifdef USE_WEBSERVER
  else
  {
    WSContentSend_PD(HTTP_SNS_CUBIC_METER, type_str, usage);
    if(*s->type==1)
        WSContentSend_PD(HTTP_SNS_LPM, type_str, rate);
    if(*s->type==2)
        WSContentSend_PD(HTTP_SNS_CPH, type_str, rate);
    }
#endif  // USE_WEBSERVER
}

void S0Flow_StateTextJson(int id)
{
  tS0Flow* s=&s0flow[id];
  char str_r[20];
  char str_u[20];
  const char *type = *s->type==1?D_WATER:D_GAS;
  float u=*s->resolution;
  u*=*s->usage;
  dtostrfd(u, 3, str_u);
  dtostrfd(*s->resolution, 4, str_r);
  Response_P(PSTR("{\"%s%d\":{\"%s\":%s,\"Resolution\":%s,\"Low-Debounce\":%d,\"High-Debounce\":%d}}"),type,id+1,D_TOTAL_USAGE,str_u,str_r,*s->low_debounce,*s->high_debounce);
}

bool S0Flow_Cmd(int id,int cmd,double val)
{
  char val_str[20];
  dtostrf(val, 9, 3, val_str);
  //AddLog_P2(LOG_LEVEL_ERROR, PSTR("id %d / cmd %d / val %s"),id,cmd,val_str);
  if(id<0 || id>3) return false;
  if(cmd<0 || cmd>5) return false;
  if(cmd==0) {};  // show values
  if(cmd==1) *s0flow[id].usage=val / *s0flow[id].resolution;
  if(cmd==2) *s0flow[id].resolution=val;
  if(cmd==3) *s0flow[id].type=val;
  if(cmd==4) *s0flow[id].low_debounce=val;
  if(cmd==5) *s0flow[id].high_debounce=val;
  S0Flow_StateTextJson(id);
  return true;
}

bool S0Flow_Command(int id)
{
  tS0Flow* s=&s0flow[id];
  char sub_string1[XdrvMailbox.data_len +1];
  char sub_string2[XdrvMailbox.data_len +1];

  //AddLog_P2(LOG_LEVEL_ERROR, PSTR("id=%d / payload=%d"),id,XdrvMailbox.payload);
  if(XdrvMailbox.payload-1!=id) return false; 

  for (uint32_t ca = 0; ca < XdrvMailbox.data_len; ca++) {
    if ((' ' == XdrvMailbox.data[ca]) || ('=' == XdrvMailbox.data[ca])) { XdrvMailbox.data[ca] = ','; }
  }
  //AddLog_P2(LOG_LEVEL_ERROR, PSTR("payload %d / data %s / cmdcode %d"),XdrvMailbox.payload,XdrvMailbox.data,XdrvMailbox.command_code);
  if (strstr(XdrvMailbox.data, ",") != nullptr)
  { char* sub1=subStr(sub_string1, XdrvMailbox.data, ",", 2);
    char* sub2=subStr(sub_string2, XdrvMailbox.data, ",", 3);
    int cmd=0;
    double  val=0;
    if(sub1!=nullptr && sub2!=nullptr)
    { cmd=atoi(sub1);
      val=strtod(sub2,nullptr);
    }
    return S0Flow_Cmd(id,  cmd, val ); 
  }
  S0Flow_StateTextJson(id);
  return true;
}

void S0FlowInit(int id)
{
    bool init=false;
    if (PinUsed(GPIO_S0_FLOW_METER1,id)) {    
        s0flow[id].pin=Pin(GPIO_S0_FLOW_METER1,id);
        init=true;
    }
 
    if(init) {
        s0flow[id].pintime=0;
        s0flow[id].pincnt=0;
        s0flow[id].rate=0;
        s0flow[id].id=id;
        s0flow[id].usage=   (uint32_t*)&(Settings->s0flow_usage[id]);
        s0flow[id].resolution= (float*)&(Settings->s0flow_resolution[id]);
        s0flow[id].type=               &(Settings->s0flow_type[id]);
        s0flow[id].low_debounce=       &(Settings->s0flow_low_debounce[id]);
        s0flow[id].high_debounce=      &(Settings->s0flow_high_debounce[id]);
        pinMode(s0flow[id].pin, INPUT_PULLUP);
        attachInterruptArg(s0flow[id].pin, S0FlowIsrArg, &s0flow[id], CHANGE);
    }
}


/*********************************************************************************************\
 * Interface
\*********************************************************************************************/
bool Xsns121(uint32_t function)
{
  // If we don't have pins assigned give up quickly.
  int id;
  for(id=0;id<4;id++)
  { 
    if (!PinUsed(GPIO_S0_FLOW_METER1,id) ) { continue; }
      
    switch (function)
    {
        case FUNC_INIT:
          S0FlowInit(id);
          break;
        case FUNC_EVERY_250_MSECOND:
          S0FlowEvery250ms(id);
          break;
        case FUNC_JSON_APPEND:
          S0FlowShow(1,id);
          break;
        case FUNC_COMMAND_SENSOR:
          if (XSNS_121 == XdrvMailbox.index) {
            bool ret=S0Flow_Command(id);
            //AddLog_P2(LOG_LEVEL_ERROR, PSTR("id=%d / ret=%d"),id,ret);
            if(ret==true) return true;
          }
          break;
#ifdef USE_WEBSERVER
        case FUNC_WEB_SENSOR:
          S0FlowShow(0,id);
          break;
#endif  // USE_WEBSERVER
    }
  }
  return false;
}

#endif  // USE_S0_FLOW_METER
