
#pragma once

#include "bma423.h"
#include "i2c_bus.h"


enum {
    DIRECTION_TOP_EDGE        = 0,
    DIRECTION_BOTTOM_EDGE     = 1,
    DIRECTION_LEFT_EDGE       = 2,
    DIRECTION_RIGHT_EDGE      = 3,
    DIRECTION_DISP_UP         = 4,
    DIRECTION_DISP_DOWN       = 5
} ;
typedef struct bma4_dev Bma;
typedef struct bma4_accel Accel;
typedef struct bma4_accel_config Acfg;

class BMA
{
public:
    BMA(I2CBus &bus);
    ~BMA();
    bool begin();
    void reset();
    uint8_t direction();
    float temperature();
    void enableAccel();

    void disalbeIrq();
    void enableIrq();
    void attachInterrupt();
    uint32_t getCounter();
    bool isStepCounter();
    bool isDoubleClick();
    bool readInterrupt();
    bool isTilt();
    bool isActivity();
    bool isAnyNoMotion();
    bool getAccel(Accel &acc);
    uint8_t getIrqStatus();
    const char * getActivity();

    bool set_remap_axes(struct bma423_axes_remap *remap_data);
    bool enableStepCountInterrupt(bool en = true);
    bool enableTiltInterrupt(bool en = true);
    bool enableWakeupInterrupt(bool en = true);
    bool enableAnyNoMotionInterrupt(bool en = true);
    bool enableActivityInterrupt(bool en = true);

private:
    static uint16_t read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *read_data, uint16_t len);
    static uint16_t write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *read_data, uint16_t len);

    uint16_t config();
    Bma _dev;
    static bma4_com_fptr_t _read;
    static bma4_com_fptr_t _write;
    static I2CBus *_bus;
    bool _irqRead = false;
    uint16_t _irqStatus;

};
