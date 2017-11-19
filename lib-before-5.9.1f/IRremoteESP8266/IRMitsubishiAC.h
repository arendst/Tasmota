
#include <IRremoteESP8266.h>
#include <Arduino.h>

#define MITSUBISHI_AC_AUTO 0x20U
#define MITSUBISHI_AC_COOL 0x18U
#define MITSUBISHI_AC_DRY 0x10U
#define MITSUBISHI_AC_HEAT 0x08U
#define MITSUBISHI_AC_POWER 0x20U
#define MITSUBISHI_AC_FAN_AUTO 0U
#define MITSUBISHI_AC_FAN_MAX 5U
#define MITSUBISHI_AC_FAN_SILENT 6U
#define MITSUBISHI_AC_MIN_TEMP 16U  // 16C
#define MITSUBISHI_AC_MAX_TEMP 31U  // 31C
#define MITSUBISHI_AC_VANE_AUTO 0U
#define MITSUBISHI_AC_VANE_AUTO_MOVE 7U
#define MITSUBISHI_AC_STATE_LENGTH 18

class IRMitsubishiAC
{
    public:
        IRMitsubishiAC(int pin);

        void stateReset();
        void send();

        void begin();
        void on();
        void off();
        void setPower(bool state);
        bool getPower();
        void setTemp(uint8_t temp);
        uint8_t getTemp();
        void setFan(uint8_t fan);
        uint8_t getFan();
        void setMode(uint8_t mode);
        uint8_t getMode();
        void setVane(uint8_t mode);
        uint8_t getVane();
        uint8_t* getRaw();


    private:
        // The state of the IR remote in IR code form.
        // Known good state obtained from:
        //   https://github.com/r45635/HVAC-IR-Control/blob/master/HVAC_ESP8266/HVAC_ESP8266.ino#L108
        uint8_t known_good_state[MITSUBISHI_AC_STATE_LENGTH] = { 0x23, 0xCB, 0x26, 0x01, 0x00, 0x20, 0x08, 0x06, 0x30, 0x45, 0x67, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F };
        uint8_t remote_state[MITSUBISHI_AC_STATE_LENGTH];

        void checksum();
        IRsend _irsend;
};
