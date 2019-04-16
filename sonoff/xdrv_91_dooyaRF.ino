/*
  xdrv_91_rf443.ino - Veneta Blind RF Protocol support for Sonoff-Tasmota

  Copyright (C) 2019  Theo Arends

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

#ifdef USE_DOOYA

#define XDRV_91             91
/*********************************************************************************************\
 * constants
\*********************************************************************************************/
#define D_CMND_DOOYARF "dooya"

//The key to timing success is each bit regardless of True or False must be 1080 microsecond long in real time.

#define PREP_LONG 8550 //8550
#define START_HIGH 4725 //4725
#define START_LOW 1525 //1525
#define SHORT_BIT 350 //330 
#define LONG_BIT 730 //715

int last_delay =0;

void init_msg(){
  digitalWrite(pin[GPIO_DOOYARF_TX], LOW); //LOW
  delayMicroseconds(PREP_LONG - last_delay); 
  digitalWrite(pin[GPIO_DOOYARF_TX], HIGH);
  delayMicroseconds(START_HIGH);
  digitalWrite(pin[GPIO_DOOYARF_TX], LOW); //LOW
  delayMicroseconds(START_LOW);
}

void send_bit(int state){
  int high_time, low_time;
 
  if(state){ //True State
    high_time = LONG_BIT;
    low_time=SHORT_BIT;
  }else{ //False State
    high_time = SHORT_BIT;
    low_time=LONG_BIT;
  }
  //output to the transmiter 
  digitalWrite(pin[GPIO_DOOYARF_TX], HIGH); //HIGH
  delayMicroseconds(high_time);
  digitalWrite(pin[GPIO_DOOYARF_TX], LOW); //LOW
  delayMicroseconds(low_time);
  last_delay = low_time;
}

void assemble_bits(char chr){
    int revbin[4];
    int n = chr - 48; 
    int j;
    //assemble bit pattern
    if(n > 9){n-=7;} 
    for(j=0;j<4;j++){
       revbin[j]=(n % 2);
       n = n / 2; 
    }
    //reverse bit pattern
    for (int k = j - 1; k >= 0; k--){
      send_bit(revbin[k]); 
    }
    return;
}

void dooyaInit(void) {
  pinMode(pin[GPIO_DOOYARF_TX],OUTPUT);
  return;
}
//If the Blind number is 0 then all blinds with the same group will opperate
//together for the number is 1 or > it will only operate the individual blind
//the blind group is a group of blinds for a room or area which could operate together using the 0 blind operation.
  
//dooya {"group":"0x9F340E","blind":1,"action":up}
//dooya {"group":"0x9F340E","blind":1,"action":down}
//dooya {"group":"0x9F340E","blind":1,"action":stop}
//dooya {"group":"0x9F340E","blind":1,"action":prog}

//dooya {"group":"0x11340E","blind":1,"action":prog}
//dooya {"group":"0x11340E","blind":1,"action":down}

bool dooyaCommand(){
    char command [CMDSZ];
    bool serviced = false;
    char parm_uc[10];
    
    //copy from DRV05_irremote    
    char dataBufUc[XdrvMailbox.data_len]; 
    UpperCase(dataBufUc, XdrvMailbox.data);
    StaticJsonBuffer<128> jsonBuf;
    JsonObject &root = jsonBuf.parseObject(dataBufUc);

    if (!root.success()) {
        Response_P(S_JSON_COMMAND_SVALUE, command, D_JSON_INVALID_JSON);
    }else{
        char parm_uc[10];
        char responce[12];
              
        //Construct the RF Message.
        strcpy(responce,root[UpperCase_P(parm_uc, PSTR(D_JSON_DOOYARF_GROUP))]);
        strcat(responce,"B");
        strcat(responce,root[UpperCase_P(parm_uc, PSTR(D_JSON_DOOYARF_BLIND))]);
        const char *action = root[UpperCase_P(parm_uc, PSTR(D_JSON_DOOYARF_ACTION))];        

        if(!strcmp(action,UpperCase_P(parm_uc, PSTR(D_JSON_DOOYARF_ACTION_UP)))){
          strcat(responce,"11");
          serviced = true;
        }
        if(!strcmp(action,UpperCase_P(parm_uc, PSTR(D_JSON_DOOYARF_ACTION_DOWN)))){
          strcat(responce,"33");
          serviced = true;
        }
        if(!strcmp(action,UpperCase_P(parm_uc, PSTR(D_JSON_DOOYARF_ACTION_STOP)))){
          strcat(responce,"55");
          serviced = true;
        }
        if(!strcmp(action,UpperCase_P(parm_uc, PSTR(D_JSON_DOOYARF_ACTION_PROG)))){
          strcat(responce,"CC");
          serviced = true;
        }

        if(serviced){
          for(int h=0;h<10;h++){
            last_delay =0;
            init_msg();
            for(int i=2;i<12;i++){assemble_bits(responce[i]);}
          }
          Response_P(responce);
            
        }else{
          Serial.println(pin[GPIO_DOOYARF_TX]);
          serviced = true;
        }
    }
    return(serviced);
}
/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv91(uint8_t function)
{ 
  bool result = false;

  if (pin[GPIO_DOOYARF_TX] < 99) {
    switch (function) {
      case FUNC_PRE_INIT:
        dooyaInit();                             // init and start communication
        break;
      case FUNC_EVERY_50_MSECOND:
        break;
      case FUNC_EVERY_SECOND:
        break;
      case FUNC_JSON_APPEND:
        break;
      case FUNC_SAVE_BEFORE_RESTART:
        break;
      case FUNC_COMMAND:
        result = dooyaCommand();  // Return true on success
        break;
    }
  }
  return result; 
}

#endif  // USE_DOOYA
