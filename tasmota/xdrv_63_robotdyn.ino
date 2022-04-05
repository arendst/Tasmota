/*
  xdrv_63_robotdyn.ino -support for robotdin 4 AC channel dimmer with zero crossing

  SPDX-FileCopyrightText: 2022 Benoit Lecuppe

  SPDX-License-Identifier: GPL-3.0-only
*/
//ifdef VSCODE_DEV
  #define USE_LIGHT
  #define USE_ROBOTDYN_DIMMER
//endif
#ifdef USE_LIGHT
#ifdef USE_ROBOTDYN_DIMMER
/*********************************************************************************************\
 * RobotDyn AC Power dimmer controller WI-FI 
 * ZeroCross GPIO5
 * DIMMER1   GPIO4
 * DIMMER2  GPIO13
 * DIMMER3  GPIO14
 * DIMMER4  GPIO12
 * MCU ESP8266
 * FLASH 4MB
 * AC-IN 90-240V
 * DC - VDD 3.3V
 * RDB abreviation mean RoBotDyn
\*********************************************************************************************/

#define XDRV_63                 63

extern "C" int startWaveform(uint8_t pin, uint32_t timeHighUS, uint32_t timeLowUS, uint32_t runTimeUS = 0, int8_t alignPhase = -1, uint32_t phaseOffsetUS = 0, bool autoPwm = false);
extern "C" int stopWaveform(uint8_t pin);


void rbdInit(void){
  // RobotDyn AC dimmer Init
  AddLog(LOG_LEVEL_INFO, PSTR("========== RBD INIT rdbInit() "));
  // init ZeroCross detection. One pin must be attach to "ZC Pulse"
  if (!PinUsed(GPIO_ZEROCROSS))  AddLog(LOG_LEVEL_ERROR, PSTR("========== RBD INIT ERROR: on pin mist be set to ZC Pulse in template"));
  ZeroCrossInit(0);

}


// wait until time. time in µs from board start
// delay microsseconds give time from système (not shure)
// if time < 10, then time is the number of 10ms iteration delay
uint32_t rbdWait(uint32_t time,uint32_t offset){   
  uint32_t now , i, max = 1000 ;
  if (time < 1) max = 1;
  for(i = 0;i<max; i++ ){
    now = micros();
    if( now + offset >= time && time > 0) break;
    yield();   
    delayMicroseconds(10);
    if(time == 0) break;
  }
  now = micros();
  //AddLog(LOG_LEVEL_INFO, PSTR("========== RDB rbdWait(time); now %dµs - time %dµs = %dµs; loop %d"), now, time, now - time, i);
  return micros();
}

// get next Zero-crossing (all args in µs)
// next : last zerocross calculated
// cycle : time of cycle 
// offset : time to do somme calc before zero-crossing
uint32_t rbdGetNext(uint32_t next,uint32_t cycle,uint32_t offset) {
  uint32_t now = micros() , i ;
  for(i = 0;i<1000; i++ ){
    if( next  >= now + offset) break;
    next += cycle;
  }
  //AddLog(LOG_LEVEL_INFO, PSTR("========== RDB getNext(next); next %dµs - now %dµs = %dµs; loop %d"), next, now, next-now, i);
  return next;
}

typedef struct  {uint32_t  time, cycle, last, lastCycle, next, now,n,pin,start,started;} RDBTRACE;
RDBTRACE rbdt[100];



// this is the main function
// call every 50ms by tasmota
// resync PWM generator to zerocross
// calc most probalily next zero-crossing, and next time to send pulse to triac.
// wait this moment and start new wave form to the pin whose linked to triac trigger.
// don't use AddLog in the midlle of this function
// use struct rbdT to log data and print a the end
void rbdSyncPWM(void) {
/* DEBUG */ uint32_t d = 0; // debug point
  // retrieve data from interrupt
  static uint32_t last, next, cycle, halfCycle;
  uint32_t lastTime = TasmotaGlobal.zc_time; 
  uint32_t lastCycle = TasmotaGlobal.zc_interval;  

  const uint32_t MINTime = 20000, MAXTime = UINT32_MAX - MINTime, MINCycle = 8000, DEFCycle = 10000, MAXCycle = 12000, MINLast = 10000000 ;
 
  // WAIT 10s after card start
  if(!cycle && lastTime < MINLast) {
      AddLog(LOG_LEVEL_INFO, PSTR("====== RBD 0 WAIT %ds"),(MINLast - lastTime)/1000000);
      return;
    }

  // REMOVE BORDER OVERFLOW
  if((MINTime < lastTime && lastTime < MAXTime ) == false) {
    AddLog(LOG_LEVEL_INFO, PSTR("====== RBD 1 SKIP OVERFLOW %dµs"),lastCycle);
    return;
  }

  // REMOVE BAD CYCLE
  if( (MINCycle < lastCycle && lastCycle < MAXCycle) == false) {
    AddLog(LOG_LEVEL_INFO, PSTR("====== RBD 2 BAD CYCLE %dµs"),lastCycle);
    return;
  }

  // FIRST TIME INIT
  if( !cycle) {
    cycle = DEFCycle; // lastCycle ; // AT 50Hz one periode = 200ms one cycle per half periode = 100ms
    halfCycle = (cycle + 1) / 2;
    last = lastTime; // last ZC get by interrupt
    AddLog(LOG_LEVEL_INFO, PSTR("====== RBD 3 INIT last %dµs, lastcycle %dµs"),last,lastCycle);
    return;
  } 

  // CALC lastCycle(s) time
  uint32_t time = lastTime - last; //time between last ZC calcl and Curent ZC
  uint32_t n = ( time + halfCycle) / cycle ; // rounding number of Cycle ()
  lastCycle = (time + n/2)/n ;
  last = lastTime;
  /* DEBUG 4 */ rbdt[d = 0] = { .time = time, .cycle=cycle, .last=last, .lastCycle=lastCycle, .next=next, .now=0,.n=n,.pin=0,.start=0,.started=0};

  // FIND next ~= last the nearest caclulated zero-crossing 
  // if 'last' has exceeded UINT32_MAX 'next' will do the same
  uint32_t min = last - halfCycle, max = last + halfCycle; 
  while(next < min || max < next)  next += cycle; 
  /* DEBUG 5 */ rbdt[d = 1] = { .time = time, .cycle=cycle, .last=last, .lastCycle=lastCycle, .next=next, .now=0,.n=0,.pin=0,.start=0,.started=0};

  // AVERAGE cycle and next zero crossing, carref
  cycle = (49 * cycle + lastCycle +25) / 50; // calc round average
  next  = last;// (uint32_t) ((9 * (uint64_t)next + (uint64_t)last + 5) / 10); // calc round avarage, pay attention to ocerflow so use uint64
  /* DEBUG 6 */ rbdt[d = 2] = { .time = time, .cycle=cycle, .last=last, .lastCycle=lastCycle, .next=next, .now=0,.n=0,.pin=0,.start=0,.started=0};
  
  // PWM time cycle calc
  const uint32_t high = DEFCycle / 256;
  uint32_t low = cycle - high;

  // GET next zero-crossing
  next = rbdGetNext(next, cycle, 150); // get next cycle , 100µs to do calc and stop old PWM
  /* DEBUG 7 */ rbdt[d = 3] = { .time = time, .cycle=cycle, .last=last, .lastCycle=lastCycle, .next=next, .now=micros(),.n=0,.pin=0,.start=0,.started=0};
  
  // WAIT next zero-crossing
  uint32_t now = rbdWait( next,100 ); // wait until 100µs before next zero-crossing
  /* DEBUG 7 */ rbdt[d = 4] = { .time = time, .cycle=cycle, .last=last, .lastCycle=lastCycle, .next=next, .now=micros(),.n=0,.pin=0,.start=0,.started=0};
  
  n = 0; // light index
  struct PWM { int pin;uint32_t start;} pwm[MAX_PWMS], *p = pwm;
  // ZERO-CROSSING stop PWM wave for resync and recalc start point for each PWM
  for (uint32_t i = 0; i < MAX_PWMS;i++) { //for each light
    p->pin = Pin(GPIO_PWM1, i);
    if( p->pin < 0) continue;
    p->start = next  + (cycle * (900 - (Light.fade_running ? Light.fade_cur_10[i] : Light.fade_start_10[i]))  ) / 1024; // calc start clock
    //stopWaveform(p->pin); // stop actual PWM
    /* DEBUG 8 to 12 */ rbdt[d = 5+n] = { .time = time, .cycle=cycle, .last=last, .lastCycle=lastCycle, .next=next, .now=micros(),.n=n,.pin=p->pin,.start=p->start,.started=0};
    p++;
    n++; // number of light to start
  }
  now = micros();
  // START RESYNC triac
  uint32_t k = 0;
  bool watchdog = false;
  for (uint32_t i = 0,j=0; i < n;){ // n ligths are to start 
    for (p = pwm; p < pwm + n; p++) { //for each light
      j++;
      if( p->pin < 0 ) continue ; // light already processed 
      if( p->start > micros()) continue; // start this light latter
      if( micros() > p->start  + 300) { p->start += cycle;continue;} // to late maybe an interruption take long time
      stopWaveform(p->pin); // stop actual PWM
      if( micros() > p->start  + 300) { p->start += cycle;continue;} // to late maybe an interruption take long time
      startWaveform(p->pin, high, low, 0, -1, 0, true); // start the light NOW!
      /* DEBUG 13 to 18 */ rbdt[d = 10+k++] = { .time =(uint32_t)(p-pwm), .cycle=j, .last=last, .lastCycle=lastCycle, .next=next, .now=micros(),.n=i,.pin=p->pin,.start=p->start,.started=0};
      p->pin = -1; // mark the light as processed
      i++; // one more light  started
      }
    if( k > 100) break; //watchdog
    now = rbdWait(0,0); // wait 10µS minimum
  }
 return; // LOG DEBUG
  RDBTRACE *t;
    AddLog(LOG_LEVEL_INFO, PSTR(""));
    t = &rbdt[0]; AddLog(LOG_LEVEL_INFO, PSTR("====== RBD  4 CALC   time %8dµs / cycle  %dµs = n %3d, lastCycle = %dµS  After last interupt"),
                                                                    t->time,     t->cycle, t->n,     t->lastCycle);
    t = &rbdt[1]; AddLog(LOG_LEVEL_INFO, PSTR("=======RBD  5 FIND   last %dµs  next %dµs delta %d After find nearest next"),
                                                                t-> last, t->next, t->next - t->last );
    t = &rbdt[2]; AddLog(LOG_LEVEL_INFO, PSTR("=======RBD  6 AVG    last %dµs  next %dµs delta %d, cycle = %dµS  After average next and cycle"),
                                                                t-> last, t->next, t->next - t->last, t->cycle );
    t = &rbdt[3]; AddLog(LOG_LEVEL_INFO, PSTR("=======RBD  7 GET  ZC now %dµs  next %dµs lag %4d, After calc next ZC"),
                                                                t-> now, t->next, t->now - t->next );
    t = &rbdt[4]; AddLog(LOG_LEVEL_INFO, PSTR("=======RBD  8 WAIT ZC now %dµs  next %dµs lag %4d, After ZC"),
                                                                t-> now, t->next, t->now - t->next );

  return;
  for(d = 5; d < 5 + MAX_PWMS;d++) {
    t = &rbdt[d]; AddLog(LOG_LEVEL_INFO, PSTR("=======RBD %2d STOP    now %dµs start %dµs lag %d, pin %d  After stop ZC"),
                                                               d+5, t-> now, t->start, t->now - t->start, t->pin );
  }
  for(d = 10; d < 10 + k; d++) {
    t = &rbdt[d]; AddLog(LOG_LEVEL_INFO, PSTR("=======RBD %2d START   now %dµs start %dµs lag %d, pin %d, j=%2d, p=%d    After start"),
                                                        d+10, t-> now, t->start, t->now - t->start, t->pin, t->cycle, t->time );
  }
  return;
}

// commande "RDBTest"
void rbdCmndTest(void)
{
  AddLog(LOG_LEVEL_INFO, PSTR("========== RBD rdbCmndTest(void)"));
   ResponseClear();
}

/*********************************************************************************************\
 * Driver Interface
\*********************************************************************************************/

// this is the dictionnary of commands for RobotDyn
// first table contain the list of commandes and the seconde is the fonctions to call in this module 
const char kRBDCommands[] PROGMEM = "|RBDTest" ;
void (* const RBDCommands[])(void) PROGMEM = {&rbdCmndTest };
// This function is ethe interface (the entry point for all action require by tastota) with tasmota program

bool Xdrv63(uint8_t function) {

  bool result = false;
  switch(function) {
    case FUNC_INIT : // module must init
      rbdInit();
      break;
    case FUNC_EVERY_50_MSECOND: // FUNC_EVERY_SECOND: // entry point to do somethin every 50ms. in our case resync the PWM on ze-cross
      rbdSyncPWM();
      break;
    case FUNC_COMMAND: // command define by this module or another aend by tasmota 

      AddLog(LOG_LEVEL_INFO, PSTR("========== RBD COMMAND"));
      //decodeCommand is a tasmota function that decode command and call the right fonction.
      // i just implement a command to test module
      

      result = DecodeCommand(kRBDCommands, RBDCommands,nullptr);
      break;
  }
  return result;
}

#endif  // USE_ROBOTDYN_DIMMER
#endif  // USE_LIGHTd
