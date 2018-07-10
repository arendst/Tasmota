/*firmware of multichannel gas sensor v2.0*3
  write by loovee
  2016-11-6

  Factory adc value of 3 channels:
  NH3 = 860
  CO  = 950
  NO2 = 155

  Default address is 0x04
*/

#include <Wire.h>
#include <EEPROM.h>

#define DEFAULT_I2C_ADDR 0x04
#define DBG                     1
// EEPROM ADDRESS
#define ADDR_IS_SET             0           // if this is the first time to run, if 1126, set 
#define ADDR_FACTORY_ADC_NH3    2
#define ADDR_FACTORY_ADC_CO     4
#define ADDR_FACTORY_ADC_NO2    6

#define ADDR_USER_ADC_HN3       8
#define ADDR_USER_ADC_CO        10
#define ADDR_USER_ADC_NO2       12
#define ADDR_IF_CALI            14          // IF USER HAD CALI
#define ADDR_I2C_ADDRESS        20
// I2C COMMAND

#define CMD_ADC_RES0            1           // NH3
#define CMD_ADC_RES1            2           // CO
#define CMD_ADC_RES2            3           // NO2
#define CMD_ADC_RESALL          4           // ALL CHANNEL
#define CMD_CHANGE_I2C          5           // CHANGE I2C
#define CMD_READ_EEPROM         6           // READ EEPROM VALUE, RETURN UNSIGNED INT
#define CMD_SET_R0_ADC          7           // SET R0 ADC VALUE
#define CMD_GET_R0_ADC          8           // GET R0 ADC VALUE
#define CMD_GET_R0_ADC_FACTORY  9           // GET FACTORY R0 ADC VALUE
#define CMD_CONTROL_LED         10
#define CMD_CONTROL_PWR         11

// EEPROM READ AND WRITE - UNSIGNED INT
void eeprom_write(int addr, unsigned int value)
{
    EEPROM.write(addr, value>>8);
    EEPROM.write(addr+1, value&0xff);
}

unsigned int eeprom_read(int addr)
{
    unsigned int r = EEPROM.read(addr);
    r <<= 8;
    r+= EEPROM.read(addr+1);
    return r;
}

const int pin_pwr = 8;
const int pin_led = 9;

const int pin_NH3   = A0;           // RES0
const int pin_CO    = A1;           // RES1
const int pin_NO2   = A2;           // RES2

unsigned char i2c_address = 0;

#define LED_ON()    digitalWrite(pin_led, LOW)
#define LED_OFF()   digitalWrite(pin_led, HIGH)

void factory_init()
{
#if DBG
    Serial.print("FACTORY: ");
#endif
    if(1126 != eeprom_read(ADDR_IS_SET))            // IF FACTORY SET
    {
#if DBG
        Serial.println("YES");
#endif
        eeprom_write(ADDR_IS_SET, 1126);
        eeprom_write(ADDR_FACTORY_ADC_NH3, 860);
        eeprom_write(ADDR_FACTORY_ADC_CO, 950);
        eeprom_write(ADDR_FACTORY_ADC_NO2, 155);
        eeprom_write(ADDR_USER_ADC_HN3, 860);
        eeprom_write(ADDR_USER_ADC_CO, 950);
        eeprom_write(ADDR_USER_ADC_NO2, 155);
        eeprom_write(ADDR_IF_CALI, 0);
        eeprom_write(ADDR_I2C_ADDRESS, DEFAULT_I2C_ADDR);
    }
#if DBG
    else Serial.println("NO");
#endif
}

int getAnalog(int pin)
{
    long sum = 0;
    for(int i=0; i<64; i++)
    {
        sum += analogRead(pin);
    }
    return sum>>6;
}

unsigned int ADC_RES0 = 0;
unsigned int ADC_RES1 = 0;
unsigned int ADC_RES2 = 0;

unsigned char raw_adc[6];

void updateValue()
{
    static unsigned long timer_s = millis();
    if(millis()-timer_s < 1000)return;
    timer_s = millis();
    
    ADC_RES0 = getAnalog(pin_NH3);
    ADC_RES1 = getAnalog(pin_CO);
    ADC_RES2 = getAnalog(pin_NO2);
    
    raw_adc[0] = ADC_RES0>>8;
    raw_adc[1] = ADC_RES0;
    raw_adc[2] = ADC_RES1>>8;
    raw_adc[3] = ADC_RES1;
    raw_adc[4] = ADC_RES2>>8;
    raw_adc[5] = ADC_RES2;
}

void setup()
{
#if DBG
    Serial.begin(115200);
#endif
    pinMode(pin_pwr, OUTPUT);
    digitalWrite(pin_pwr, HIGH);
    pinMode(pin_led, OUTPUT);

    factory_init();
    i2c_address = eeprom_read(ADDR_I2C_ADDRESS);
#if DBG
    Serial.print("i2d address = 0x");
    Serial.println(i2c_address, HEX);
#endif
    for(int i=0; i<5; i++)
    {
        digitalWrite(pin_led, LOW);
        delay(100);
        digitalWrite(pin_led, HIGH);
        delay(100);
    }

    Wire.begin(i2c_address);           // join i2c bus with address
    Wire.onReceive(receiveCallback);        // register receive callback
    Wire.onRequest(requestCallback);        // register request callback
}

void loop()
{
    updateValue();
}

unsigned char recvCmd = 0;
unsigned char recvDta = 0;
unsigned char recvDtaStr[10];

void receiveCallback(int dtaCount)
{
    if(dtaCount == 1)
    {
        recvCmd  = Wire.read();
    }
    else if(dtaCount == 2)     // set i2c address
    {
        recvCmd = Wire.read();
        recvDta = Wire.read();

        if(CMD_CHANGE_I2C == recvCmd)
        {
            i2c_address = recvDta;
            eeprom_write(ADDR_I2C_ADDRESS, i2c_address);
            Wire.begin(i2c_address);
        }
        else if(CMD_CONTROL_LED == recvCmd)
        {
            if(0 == recvDta)LED_OFF();
            else if(1 == recvDta)LED_ON();
        }
        else if(CMD_CONTROL_PWR == recvCmd)
        {
            if(0 == recvDta)digitalWrite(pin_pwr, LOW);
            else if(1 == recvDta)digitalWrite(pin_pwr, HIGH);
        }
    }
    else if(dtaCount == 7)      // set ADC value
    {
        recvCmd = Wire.read();
        unsigned int dta[3];
        
        for(int i=0; i<3; i++)
        {
            dta[i] = Wire.read();
            dta[i] <<= 8;
            dta[i] += Wire.read();
        }
       
        if(recvCmd == CMD_SET_R0_ADC)
        {
            eeprom_write(ADDR_USER_ADC_HN3, dta[0]);
            eeprom_write(ADDR_USER_ADC_CO, dta[1]);
            eeprom_write(ADDR_USER_ADC_NO2, dta[2]);
        }
    }
}

unsigned char rcDta[10];
void requestCallback()
{
    switch(recvCmd)
    {
        case CMD_ADC_RES0:                      // NH3
        Wire.write(&raw_adc[0], 2);             // HIGH FIRST

        break;
        
        case CMD_ADC_RES1:                      // CO
        Wire.write(&raw_adc[2], 2);             // HIGH FIRST
        break;
        
        case CMD_ADC_RES2:                      // NO2
        Wire.write(&raw_adc[4], 2);             // HIGH FIRST
        break;
        
        case CMD_ADC_RESALL:
        Wire.write(raw_adc, 6);
        break;
        
        case CMD_READ_EEPROM:
        rcDta[0] = EEPROM.read(recvDta);
        rcDta[1] = EEPROM.read(recvDta+1);
        Wire.write(rcDta, 2);
        break;
        
        case CMD_GET_R0_ADC:
        
        rcDta[0] = EEPROM.read(ADDR_USER_ADC_HN3);
        rcDta[1] = EEPROM.read(ADDR_USER_ADC_HN3+1);
        rcDta[2] = EEPROM.read(ADDR_USER_ADC_CO);
        rcDta[3] = EEPROM.read(ADDR_USER_ADC_CO+1);
        rcDta[4] = EEPROM.read(ADDR_USER_ADC_NO2);
        rcDta[5] = EEPROM.read(ADDR_USER_ADC_NO2+1);
        
        Wire.write(rcDta, 6);
        
        break;
        
        case CMD_GET_R0_ADC_FACTORY:
        
        rcDta[0] = EEPROM.read(ADDR_FACTORY_ADC_NH3);
        rcDta[1] = EEPROM.read(ADDR_FACTORY_ADC_NH3+1);
        rcDta[2] = EEPROM.read(ADDR_FACTORY_ADC_CO);
        rcDta[3] = EEPROM.read(ADDR_FACTORY_ADC_CO+1);
        rcDta[4] = EEPROM.read(ADDR_FACTORY_ADC_NO2);
        rcDta[5] = EEPROM.read(ADDR_FACTORY_ADC_NO2+1);
        
        Wire.write(rcDta, 6);
        break;
        
        default:;
    }
}

// END FILE
