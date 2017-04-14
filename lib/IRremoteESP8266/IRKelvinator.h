
#include <IRremoteESP8266.h>
#include <Arduino.h>

#define KELVINATOR_AUTO 0U
#define KELVINATOR_COOL 1U
#define KELVINATOR_DRY 2U
#define KELVINATOR_FAN 3U
#define KELVINATOR_HEAT 4U
#define KELVINATOR_MODE_MASK 0xF8U
#define KELVINATOR_POWER 8U
#define KELVINATOR_FAN_OFFSET 4U
#define KELVINATOR_BASIC_FAN_MAX 3U
#define KELVINATOR_BASIC_FAN_MASK uint8_t(0xFFU ^ (3U << KELVINATOR_FAN_OFFSET))
#define KELVINATOR_FAN_MASK uint8_t(0xFFU ^ (7U << KELVINATOR_FAN_OFFSET))
#define KELVINATOR_FAN_MAX 5U
#define KELVINATOR_VENT_SWING_OFFSET 6U
#define KELVINATOR_VENT_SWING uint8_t(1U << KELVINATOR_VENT_SWING_OFFSET)
#define KELVINATOR_VENT_SWING_V uint8_t(1U)
#define KELVINATOR_VENT_SWING_H uint8_t(1U << 4)
#define KELVINATOR_SLEEP_1_AND_3 uint8_t(1U << 7)
#define KELVINATOR_MIN_TEMP 16U  // 16C
#define KELVINATOR_MAX_TEMP 30U  // 30C
#define KELVINATOR_AUTO_TEMP 25U  // 25C
#define KELVINATOR_CHECKSUM_START 10U
#define KELVINATOR_QUIET_OFFSET 7U
#define KELVINATOR_QUIET uint8_t(1U << KELVINATOR_QUIET_OFFSET)
#define KELVINATOR_ION_FILTER_OFFSET 6U
#define KELVINATOR_ION_FILTER uint8_t(1U << KELVINATOR_ION_FILTER_OFFSET)
#define KELVINATOR_LIGHT_OFFSET 5U
#define KELVINATOR_LIGHT uint8_t(1U << KELVINATOR_LIGHT_OFFSET)
#define KELVINATOR_XFAN_OFFSET 7U
#define KELVINATOR_XFAN uint8_t(1U << KELVINATOR_XFAN_OFFSET)
#define KELVINATOR_TURBO_OFFSET 4U
#define KELVINATOR_TURBO uint8_t(1U << KELVINATOR_TURBO_OFFSET)


/*
	Kelvinator AC map

  (header mark and space)
  byte 0 = Basic Modes
    b2-0 = Modes
      Modes:
        000 = Auto (temp = 25C)
        001 = Cool
        010 = Dry (temp = 25C, but not shown)
        011 = Fan
        100 = Heat
    b3 = Power Status (1 = On, 0 = Off)
    b5-4 = Fan (Basic modes)
      Fan:
        00 = Auto
        01 = Fan 1
        10 = Fan 2
        11 = Fan 3 or higher (See byte 14)
    b6 = Vent swing (1 = On, 0 = Off) (See byte 4)
    b7 = Sleep Modes 1 & 3 (1 = On, 0 = Off)
  byte 1 = Temperature
    b3-0: Degrees C.
      0000 (0) = 16C
      0001 (1) = 17C
      0010 (2) = 18C
      ...
      1101 (13) = 29C
      1110 (14) = 30C
  byte 2 = Extras
    b3-0 = UNKNOWN, typically 0.
    b4 = Turbo Fan (1 = On, 0 = Off)
    b5 = Light (Display) (1 = On, 0 = Off)
    b6 = Ion Filter (1 = On, 0 = Off)
    b7 = X-Fan (Fan runs for a while after power off) (1 = On, 0 = Off)
  byte 3 = Section Indicator
    b3-0 = Unused (Typically 0)
    b5-4 = Unknown (possibly timer related) (Typically B01)
    b7-6 = End of command block (B01)
  (B010 marker and a gap of 20ms)
  byte 4 = Extended options
    b0 = Swing Vent Vertical (1 = On, 0 = Off)
    b4 = Swing Vent Horizontal (1 = On, 0 = Off)
  byte 5-6 = Timer related. Typically 0 except when timer in use.
  byte 7 = checksum
    b3-0 = Unknown (Used in Timer mode)
    b7-4 = checksum of the previous bytes (0-6)
  (gap of 40ms)
  (header mark and space)
  byte 8 = Repeat of byte 0
  byte 9 = Repeat of byte 1
  byte 10 = Repeat of byte 2
  byte 11 = Section Indicator
    b3-0 = Unused (Typically 0)
    b5-4 = Unknown (possibly timer related) (Typically B11)
    b7-6 = End of command block (B01)
  (B010 marker and a gap of 20ms)
  byte 12 = Extended options
    b0 = Sleep mode 2 (1 = On, 0=Off)
    b6-1 = Unknown (Used in Sleep Mode 3, Typically B000000)
    b7 = Quiet Mode (1 = On, 0=Off)
  byte 13 = Unknown (Sleep Mode 3 related, Typically 0x00)
  byte 14 = Fan control
    b3-0 = Unknown (Sleep Mode 3 related, Typically B0000)
    b6-4 = Fan speed
       B000 (0) = Automatic
       B001 (1) = Fan 1
       B010 (2) = Fan 2
       B011 (3) = Fan 3
       B100 (4) = Fan 4
       B101 (5) = Fan 5
  byte 15 = checksum
    b3-0 = Unknown (Typically B0000)
    b7-4 = checksum of the previous bytes (8-14)
*/

#define KELVINATOR_STATE_LENGTH 16

class IRKelvinatorAC
{
    public:
        IRKelvinatorAC(int pin);
        //: IRsend(pin){};

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
        void setSwingVertical(bool state);
        bool getSwingVertical();
        void setSwingHorizontal(bool state);
        bool getSwingHorizontal();
        void setQuiet(bool state);
        bool getQuiet();
        void setIonFilter(bool state);
        bool getIonFilter();
        void setLight(bool state);
        bool getLight();
        void setXFan(bool state);
        bool getXFan();
        void setTurbo(bool state);
        bool getTurbo();
        uint8_t* getRaw();


    private:
        // The state of the IR remote in IR code form.
        uint8_t remote_state[KELVINATOR_STATE_LENGTH];

        void checksum();
        void fixup();
        IRsend _irsend;
};
