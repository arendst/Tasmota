/*
  xdrv_89_esp32_dali.ino - DALI support for Tasmota

  Copyright (C) 2022  Andrei Kazmirtsuk aka eeak

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

  --------------------------------------------------------------------------------------------
  Version yyyymmdd  Action    Description
  --------------------------------------------------------------------------------------------
  0.0.0.1 20221027  publish - initial version
*/

#ifdef ESP32
#ifdef USE_DALI

/*********************************************************************************************\
 * DALI support for Tasmota
\*********************************************************************************************/

#define XDRV_89              89

#ifndef DALI_TIMER
    #define DALI_TIMER 0    // Default timer
#endif

#define BROADCAST_DP        0b11111110 // 0xFE
#define DALI_TOPIC          "DALI"

enum
{
    DALI_NO_ACTION,
    DALI_SENDING_DATA,
    DALI_RECEIVING_DATA,
    DALI_ERROR
};

// http and json defines
#define D_NAME_DALI "DALI"

const char S_JSON_DALI_COMMAND_NVALUE[] PROGMEM = "{\"" D_NAME_DALI "\":{\"%s\":%d}}";
const char kDALI_Commands[] PROGMEM  = D_CMND_DALI_POWER "|" D_CMND_DALI_DIMMER;

enum DALI_Commands {         // commands for Console
  CMND_DALI_PWR,
  CMND_DALI_DIM,
};

struct DALI {
    uint16_t send_dali_data;     // data to send to DALI bus
    uint16_t received_dali_data; // data received from DALI bus
    uint8_t flag;                // DALI status flag
    uint8_t bit_count;           // nr of rec/send bits
    uint16_t tick_count;         // nr of ticks of the timer
    bool former_val;             // bit value in previous tick of timer
    hw_timer_t *timer;           // hardware timer
} *Dali = nullptr;



/*********************************************************************************************\
 * DALI low level
\*********************************************************************************************/

/**
* @brief  This function handles hardware timer Handler.
* @param  None
* @retval None
*/
void IRAM_ATTR DALI_Tick_Handler(void)
{
    if (getDaliFlag() == DALI_RECEIVING_DATA)
    {
        receive_tick();
    }
    else if (getDaliFlag() == DALI_SENDING_DATA)
    {
        send_tick();
    }
}

/**
* @brief  This function enable data transfer start interrupt.
* @param  None
* @retval None
*/
void enableDaliRxInterrupt() {
    Dali->flag = DALI_NO_ACTION;
    timerAlarmDisable(Dali->timer);
    attachInterrupt(Pin(GPIO_DALI_RX), receiveDaliData, FALLING);
}

/**
* @brief  This function disable data transfer start interrupt.
* @param  None
* @retval None
*/
void disableRxInterrupt() {
    timerAlarmEnable(Dali->timer);
	detachInterrupt(Pin(GPIO_DALI_RX));
}

/**
* @brief   receiving flag status
* @param  None
* @retval uint8_t flag
*/
uint8_t getDaliFlag(void)
{
  return Dali->flag;
}

/**
* @brief   DALI data received callback
* @param  None
* @retval uint8_t flag
*/
void DataReceivedCallback() {
    AddLog(LOG_LEVEL_DEBUG, PSTR("DLI: Received: %d %d"), Dali->received_dali_data>>9, Dali->received_dali_data&0xff);
}

/*************** R E C E I V E * P R O C E D U R E S *******/

/**
* @brief  receive data from DALI bus
* @param  None
* @retval None
*/
void receiveDaliData()
{
    // null variables
    Dali->received_dali_data = 0;
    Dali->bit_count = 0;
    Dali->tick_count = 0;
    Dali->former_val = true;

    Dali->flag = DALI_RECEIVING_DATA;
    
	disableRxInterrupt();
}

/**
* @brief  Get state of DALIIN pin
* @param  None
* @retval bool status
*/
bool get_DALIIN(void)
{
    bool dali_read = digitalRead(Pin(GPIO_DALI_RX));
    return (false == DALI_IN_INVERT) ? dali_read : !dali_read;
}

/**
* @brief   receiving data from DALI bus
* @param  None
* @retval None
*
* |--------|----|---------------------------|----|
* 0        24   32                          160  176
*    wait   start      data                  stop
*/
void receive_tick(void)
{
    // four ticks per bit
    bool actual_val = get_DALIIN();
    Dali->tick_count++;
    
    // edge detected
    if(actual_val != Dali->former_val)
    {
        switch(Dali->bit_count)
        {
            case 0:
                if (Dali->tick_count > 2)
                {
                    Dali->tick_count = 0;
                    Dali->bit_count  = 1; // start bit
                }
                break;
            case 17:      // 1st stop bit
                if(Dali->tick_count > 6) { // stop bit error, no edge should exist
                    Dali->flag = DALI_ERROR;
				}
                break;
            default:      // other bits
                if(Dali->tick_count > 6)
                {
                    Dali->received_dali_data |= (actual_val << (16-Dali->bit_count));
                    Dali->bit_count++;
                    Dali->tick_count = 0;
                }
                break;
        }
    }else // voltage level stable
    {
        switch(Dali->bit_count)
        {
            case 0:
                if(Dali->tick_count==8) {	// too long start bit
                    Dali->flag = DALI_ERROR;
				}
                break;
            case 17:
                // First stop bit
                if (Dali->tick_count==8)
                {
                    if (actual_val==0) // wrong level of stop bit
                    {
                        Dali->flag = DALI_ERROR;
                    }
                    else
                    {
                        Dali->bit_count++;
                        Dali->tick_count = 0;
                    }
                }
                break;
            case 18:
                // Second stop bit
                if (Dali->tick_count==8)
                {
                    enableDaliRxInterrupt();
                    DataReceivedCallback();
					
                }
                break;
            default: // normal bits
                if(Dali->tick_count==10)
                { // too long delay before edge
                    Dali->flag = DALI_ERROR;
                }
                break;
        }
    }
    Dali->former_val = actual_val;
    if(getDaliFlag() == DALI_ERROR)
    {
		enableDaliRxInterrupt();
    }
}


/*************** S E N D * P R O C E D U R E S *************/

/**
* @brief   Set value to the DALIOUT pin
* @param  bool
* @retval None
*/
void set_DALIOUT(bool pin_value)
{
    digitalWrite(Pin(GPIO_DALI_TX), pin_value == DALI_OUT_INVERT ? LOW : HIGH);
}

/**
* @brief   gets state of the DALIOUT pin
* @param   None
* @retval bool state of the DALIOUT pin
*/
bool get_DALIOUT(void)
{
    bool dali_read = digitalRead(Pin(GPIO_DALI_TX));
    return (false == DALI_OUT_INVERT) ? dali_read : !dali_read;
}

/**
* @brief   Send data to DALI bus
* @param   byteToSend
* @retval None
*/
void sendDaliData(uint8_t firstByte, uint8_t secondByte)
{
	Dali->send_dali_data = firstByte << 8;
	Dali->send_dali_data += secondByte & 0xff;
	Dali->bit_count = 0;
	Dali->tick_count = 0;

    Dali->flag = DALI_SENDING_DATA;

	disableRxInterrupt();
}

/**
* @brief   DALI protocol physical layer for slave device
* @param   None
* @retval  None
*
* |--------|----|---------------------------|----|
* 0        24   32                          160  176
*    wait   start      data                  stop
*/
void send_tick(void)
{
	// access to the routine just every 4 ticks = every half bit
	if ((Dali->tick_count & 0x03) == 0)
	{
		if (Dali->tick_count < 160)
		{
			// settling time between forward and backward frame
			if (Dali->tick_count < 24)
			{
				Dali->tick_count++;
				return;
			}

			// start of the start bit
			if (Dali->tick_count == 24)
			{
				//   GPIOB->ODR ^= GPIO_ODR_7;
				set_DALIOUT(false);
				Dali->tick_count++;
				return;
			}

			// edge of the start bit
			// 28 ticks = 28/9600 = 2,92ms = delay between forward and backward message frame
			if (Dali->tick_count == 28)
			{
				set_DALIOUT(true);
				Dali->tick_count++;
				return;
			}

			// bit value (edge) selection
			bool bit_value = (bool)((Dali->send_dali_data >> (15 - Dali->bit_count)) & 0x01);

			// Every half bit -> Manchester coding
			if (!((Dali->tick_count - 24) & 0x0007))
			{									// div by 8
				if (get_DALIOUT() == bit_value) // former value of bit = new value of bit
					set_DALIOUT((bool)(1 - bit_value));
			}

			// Generate edge for actual bit
			if (!((Dali->tick_count - 28) & 0x0007))
			{
				set_DALIOUT(bit_value);
				Dali->bit_count++;
			}
		}
		else
		{ // end of data byte, start of stop bits
			if (Dali->tick_count == 160)
			{
				set_DALIOUT(true); // start of stop bit
			}

			// end of stop bits, no settling time
			if (Dali->tick_count == 176)
			{
				enableDaliRxInterrupt();
			}
		}
	}
	Dali->tick_count++;

	return;
}

/***********************************************************/

void DaliPreInit() {
    if (!PinUsed(GPIO_DALI_TX) || !PinUsed(GPIO_DALI_RX)) { return; }
    AddLog(LOG_LEVEL_INFO, PSTR("DLI: Init - RX-pin: %d, TX-pin: %d"), Pin(GPIO_DALI_RX), Pin(GPIO_DALI_TX));
    // pinMode(LED, OUTPUT);
	pinMode(Pin(GPIO_DALI_TX), OUTPUT);
	digitalWrite(Pin(GPIO_DALI_TX), HIGH);
	pinMode(Pin(GPIO_DALI_RX), INPUT);

	Dali = (DALI*)calloc(1,sizeof(DALI));
    if (!Dali) {
        AddLog(LOG_LEVEL_INFO, PSTR("DLI: Memory allocation error"));
        return;
    }
     Dali->timer = timerBegin(DALI_TIMER, 13, true);
    timerAttachInterrupt(Dali->timer, &DALI_Tick_Handler, true);
    timerAlarmWrite(Dali->timer, 641, true);
    
    attachInterrupt(Pin(GPIO_DALI_RX), receiveDaliData, FALLING);
	enableDaliRxInterrupt();
}

void DaliPwr(uint8_t val){
    sendDaliData(BROADCAST_DP, val);
}

bool DaliCmd(void)
{
    char command[CMDSZ];
    uint8_t name_len = strlen(D_NAME_DALI);
    if (!strncasecmp_P(XdrvMailbox.topic, PSTR(D_NAME_DALI), name_len))
    {
        uint32_t command_code = GetCommandCode(command, sizeof(command), XdrvMailbox.topic + name_len, kDALI_Commands);
        switch (command_code)
        {
        case CMND_DALI_PWR:
            if (XdrvMailbox.data_len)
            {
                if (254 >= XdrvMailbox.payload)
                {
                    DaliPwr(XdrvMailbox.payload);
                }
            }
            Response_P(S_JSON_DALI_COMMAND_NVALUE, command, XdrvMailbox.payload);
            break;
        default:
            return false;
        }
        return true;
    }
    else
    {
        return false;
    }
}


bool DaliMqtt()
{
    char stopic[TOPSZ];
    strncpy(stopic, XdrvMailbox.topic, TOPSZ);
    XdrvMailbox.topic[TOPSZ - 1] = 0;

    char *items[10];
    char *p = stopic;
    int cnt = 0;
    do
    {
        items[cnt] = strtok(p, "/");
        cnt++;
        p = nullptr;
    } while (items[cnt - 1]);
    cnt--; // repreents the number of items

    if (cnt < 3)
    { // not for us?
        AddLog(LOG_LEVEL_INFO,PSTR("cnt: %d < 3"), cnt);
        return false;
    }

    int DALIindex = 0;
    int ADRindex = 0;
    int CMDindex = 0;
    uint8_t DALIaddr = BROADCAST_DP;
    if (strcasecmp_P(items[cnt - 3], PSTR(DALI_TOPIC)) != 0)
    {
        if (strcasecmp_P(items[cnt - 2], PSTR(DALI_TOPIC)) != 0)
        {
            if (strcasecmp_P(items[cnt - 1], PSTR(DALI_TOPIC)) != 0)
            {
                return false; // not for us
            }
            else
            {
                if (true == DaliJsonParse()) { return true; }
            }
        }
        else
        {
            DALIindex = cnt - 2;
            CMDindex = cnt - 1;
        }
    }
    else
    {
        DALIindex = cnt - 3;
        CMDindex = cnt - 2;
        ADRindex = cnt - 1;
        DALIaddr = ((int)CharToFloat(items[ADRindex]))  << 1;
        
    }

    uint8_t level;
    uint8_t value = (uint8_t)CharToFloat(XdrvMailbox.data);
    if (strcasecmp_P(items[CMDindex], PSTR("percent")) == 0) {
        float percent = (float)(254 * value * 0.01);
        level = (uint8_t)percent;
    }
    else if (strcasecmp_P(items[CMDindex], PSTR("level")) == 0) {
        level = value;
    }
    else {
        AddLog(LOG_LEVEL_INFO,PSTR("command not recognized: %s"), items[CMDindex]);
        return false; // not for us
    }

    AddLog(LOG_LEVEL_INFO,PSTR("Dali value %d on address %d"), value, DALIaddr);
    sendDaliData(DALIaddr, level);

    return true;
}

bool DaliJsonParse()
{
    bool served = false;
    JsonParser parser((char *)XdrvMailbox.data);
    JsonParserObject root = parser.getRootObject();
    if (root)
    {
        int DALIindex = 0;
        int ADRindex = 0;
        int8_t DALIdim = -1;
        uint8_t DALIaddr = BROADCAST_DP;

        JsonParserToken val = root[PSTR("cmd")];
        if (val)
        {
            uint8_t cmd = val.getUInt();
            val = root[PSTR("addr")];
            if (val)
            {
                uint8_t addr = val.getUInt();
                AddLog(LOG_LEVEL_DEBUG, PSTR("DLI: cmd = %d, addr = %d"), cmd, addr);
                sendDaliData(addr, cmd);
                return true;
            }
            else
            {
                return false;
            }
        }
        val = root[PSTR("addr")];
        if (val)
        {
            uint8_t addr = val.getUInt();
            if ((addr >= 0) && (addr < 64))
                DALIaddr = addr  << 1;
        }
        val = root[PSTR("dim")];
        if (val)
        {
            uint8_t dim = val.getUInt();
            if (dim < 255)
                DALIdim = dim;
        }

        sendDaliData(DALIaddr, DALIdim);
        served = true;
    }
    
    return served;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv89(uint32_t function)
{
    bool result = false;

    if (FUNC_INIT == function)
    {
        DaliPreInit();
    }
    else if (Dali)
    {
        switch (function)
        {
        case FUNC_MQTT_DATA:
            result = DaliMqtt();
            break;
        case FUNC_COMMAND:
            result = DaliCmd();
            break;
        }
    }
    return result;
}

#endif  // USE_DALI
#endif  // ESP32