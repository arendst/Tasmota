/*
  xsns_28_qmp6988->ino - QMP6988 temperature and pressure sensor support for Tasmota

  SPDX-FileCopyrightText: 2024 Theo Arends

  SPDX-License-Identifier: GPL-3.0-only
*/

#ifdef USE_I2C
#ifdef USE_QMP6988    
/*********************************************************************************************\
 * QMP6988 Temperature and Pressure sensor
 *
 * Source: M5Stack lib M5Unit-ENV, file QMP6988.cpp
 * 
 * I2C Address: 0x56 or 0x70 
\*********************************************************************************************/

#define XSNS_28                         28
#define XI2C_88                         88    // See I2CDEVICES.md

#define QMP6988_ADDRESS_L               0x70
#define QMP6988_ADDRESS_H               0x56

#define QMP6988_CHIP_ID                 0x5C

#define QMP6988_CHIP_ID_REG             0xD1
#define QMP6988_RESET_REG               0xE0  // Device reset register
#define QMP6988_DEVICE_STAT_REG         0xF3  // Device state register
#define QMP6988_CTRLMEAS_REG            0xF4  // Measurement Condition Control Register
// Data
#define QMP6988_PRESSURE_MSB_REG        0xF7  // Pressure MSB Register

// Compensation calculation
#define QMP6988_CALIBRATION_DATA_START  0xA0  // QMP6988 compensation coefficients
#define QMP6988_CALIBRATION_DATA_LENGTH 25

// Power mode
#define QMP6988_SLEEP_MODE              0x00
#define QMP6988_FORCED_MODE             0x01
#define QMP6988_NORMAL_MODE             0x03

#define QMP6988_CTRLMEAS_REG_MODE__POS  0
#define QMP6988_CTRLMEAS_REG_MODE__MSK  0x03
#define QMP6988_CTRLMEAS_REG_MODE__LEN  2

// Oversampling
#define QMP6988_OVERSAMPLING_SKIPPED    0x00
#define QMP6988_OVERSAMPLING_1X         0x01
#define QMP6988_OVERSAMPLING_2X         0x02
#define QMP6988_OVERSAMPLING_4X         0x03
#define QMP6988_OVERSAMPLING_8X         0x04
#define QMP6988_OVERSAMPLING_16X        0x05
#define QMP6988_OVERSAMPLING_32X        0x06
#define QMP6988_OVERSAMPLING_64X        0x07

#define QMP6988_CTRLMEAS_REG_OSRST__POS 5
#define QMP6988_CTRLMEAS_REG_OSRST__MSK 0xE0
#define QMP6988_CTRLMEAS_REG_OSRST__LEN 3

#define QMP6988_CTRLMEAS_REG_OSRSP__POS 2
#define QMP6988_CTRLMEAS_REG_OSRSP__MSK 0x1C
#define QMP6988_CTRLMEAS_REG_OSRSP__LEN 3

// filter
#define QMP6988_FILTERCOEFF_OFF         0x00
#define QMP6988_FILTERCOEFF_2           0x01
#define QMP6988_FILTERCOEFF_4           0x02
#define QMP6988_FILTERCOEFF_8           0x03
#define QMP6988_FILTERCOEFF_16          0x04
#define QMP6988_FILTERCOEFF_32          0x05

#define QMP6988_CONFIG_REG              0xF1  // IIR filter co-efficient setting Register
#define QMP6988_CONFIG_REG_FILTER__POS  0
#define QMP6988_CONFIG_REG_FILTER__MSK  0x07
#define QMP6988_CONFIG_REG_FILTER__LEN  3

#define SUBTRACTOR                      8388608  // pow(2,23)

typedef struct qmp6988_cali_data_t {
  int     COE_a0;
  int16_t COE_a1;
  int16_t COE_a2;
  int     COE_b00;
  int16_t COE_bt1;
  int16_t COE_bt2;
  int16_t COE_bp1;
  int16_t COE_b11;
  int16_t COE_bp2;
  int16_t COE_b12;
  int16_t COE_b21;
  int16_t COE_bp3;
} qmp6988_cali_data_t;

typedef struct qmp6988_ik_data_t {
  int     a0;
  int     b00;
  int     a1;
  int     a2;
  int64_t bt1;
  int64_t bt2;
  int64_t bp1;
  int64_t b11;
  int64_t bp2;
  int64_t b12;
  int64_t b21;
  int64_t bp3;
} qmp6988_ik_data_t;

typedef struct qmp6988_data_t {
  qmp6988_cali_data_t qmp6988_cali;
  qmp6988_ik_data_t   ik;
  float               temperature;
  float               pressure;
  float               altitude;
  uint8_t             slave;
  uint8_t             chip_id;
  uint8_t             power_mode;
  uint8_t             address;
  uint8_t             bus;
} qmp6988_data_t;

qmp6988_data_t *Qmp6988 = nullptr;

/*********************************************************************************************/

bool QMP6988I2cReadBuffer(uint8_t reg, uint8_t *reg_data, uint16_t len) {
  bool ret = !I2cReadBuffer(Qmp6988->address, reg, reg_data, len, Qmp6988->bus);

//  AddLog(LOG_LEVEL_DEBUG, PSTR("QMP: Rd %02X '%*_H', Rslt %d"), reg, len, reg_data, ret);

  return ret;
}

bool QMP6988I2cWrite(uint8_t reg, uint32_t val) {
  bool ret = I2cWrite8(Qmp6988->address, reg, val, Qmp6988->bus);

//  AddLog(LOG_LEVEL_DEBUG, PSTR("QMP: Wr %02X '%02X', Rslt %d"), reg, val, ret);

  delay(20);
  return ret;
}

/*********************************************************************************************/

// DISABLE LOG
#define QMP6988_LOG(format...)
#define QMP6988_ERR(format...)

// ENABLE LOG
// #define QMP6988_LOG Serial.printf
// #define QMP6988_ERR Serial.printf

int QMP6988GetCalibrationData(void) {
  int status = 0;
  // BITFIELDS temp_COE;
  uint8_t a_data_uint8_tr[QMP6988_CALIBRATION_DATA_LENGTH] = {0};
  int len;

  for (len = 0; len < QMP6988_CALIBRATION_DATA_LENGTH; len += 1) {
    status = QMP6988I2cReadBuffer(QMP6988_CALIBRATION_DATA_START + len, &a_data_uint8_tr[len], 1);
    if (status == 0) {
      QMP6988_LOG("qmp6988 read 0xA0 error!");
      return status;
    }
  }

  Qmp6988->qmp6988_cali.COE_a0 = (int)(((a_data_uint8_tr[18] << 12) | (a_data_uint8_tr[19] << 4) | (a_data_uint8_tr[24] & 0x0f)) << 12);
  Qmp6988->qmp6988_cali.COE_a0 = Qmp6988->qmp6988_cali.COE_a0 >> 12;

  Qmp6988->qmp6988_cali.COE_a1 = (int16_t)(((a_data_uint8_tr[20]) << 8) | a_data_uint8_tr[21]);
  Qmp6988->qmp6988_cali.COE_a2 = (int16_t)(((a_data_uint8_tr[22]) << 8) | a_data_uint8_tr[23]);

  Qmp6988->qmp6988_cali.COE_b00 = (int)(((a_data_uint8_tr[0] << 12) | (a_data_uint8_tr[1] << 4) | ((a_data_uint8_tr[24] & 0xf0) >> 4)) << 12);
  Qmp6988->qmp6988_cali.COE_b00 = Qmp6988->qmp6988_cali.COE_b00 >> 12;

  Qmp6988->qmp6988_cali.COE_bt1 = (int16_t)(((a_data_uint8_tr[2]) << 8) | a_data_uint8_tr[3]);
  Qmp6988->qmp6988_cali.COE_bt2 = (int16_t)(((a_data_uint8_tr[4]) << 8) | a_data_uint8_tr[5]);
  Qmp6988->qmp6988_cali.COE_bp1 = (int16_t)(((a_data_uint8_tr[6]) << 8) | a_data_uint8_tr[7]);
  Qmp6988->qmp6988_cali.COE_b11 = (int16_t)(((a_data_uint8_tr[8]) << 8) | a_data_uint8_tr[9]);
  Qmp6988->qmp6988_cali.COE_bp2 = (int16_t)(((a_data_uint8_tr[10]) << 8) | a_data_uint8_tr[11]);
  Qmp6988->qmp6988_cali.COE_b12 = (int16_t)(((a_data_uint8_tr[12]) << 8) | a_data_uint8_tr[13]);
  Qmp6988->qmp6988_cali.COE_b21 = (int16_t)(((a_data_uint8_tr[14]) << 8) | a_data_uint8_tr[15]);
  Qmp6988->qmp6988_cali.COE_bp3 = (int16_t)(((a_data_uint8_tr[16]) << 8) | a_data_uint8_tr[17]);

  QMP6988_LOG("<-----------calibration data-------------->\r\n");
  QMP6988_LOG("COE_a0[%d]	COE_a1[%d]	COE_a2[%d]	COE_b00[%d]\r\n",
              Qmp6988->qmp6988_cali.COE_a0, Qmp6988->qmp6988_cali.COE_a1,
              Qmp6988->qmp6988_cali.COE_a2, Qmp6988->qmp6988_cali.COE_b00);
  QMP6988_LOG("COE_bt1[%d]	COE_bt2[%d]	COE_bp1[%d]	COE_b11[%d]\r\n",
              Qmp6988->qmp6988_cali.COE_bt1, Qmp6988->qmp6988_cali.COE_bt2,
              Qmp6988->qmp6988_cali.COE_bp1, Qmp6988->qmp6988_cali.COE_b11);
  QMP6988_LOG("COE_bp2[%d]	COE_b12[%d]	COE_b21[%d]	COE_bp3[%d]\r\n",
              Qmp6988->qmp6988_cali.COE_bp2, Qmp6988->qmp6988_cali.COE_b12,
              Qmp6988->qmp6988_cali.COE_b21, Qmp6988->qmp6988_cali.COE_bp3);
  QMP6988_LOG("<-----------calibration data-------------->\r\n");

  Qmp6988->ik.a0  = Qmp6988->qmp6988_cali.COE_a0;                                   // 20Q4
  Qmp6988->ik.b00 = Qmp6988->qmp6988_cali.COE_b00;                                  // 20Q4

  Qmp6988->ik.a1 = 3608L * (int)Qmp6988->qmp6988_cali.COE_a1 - 1731677965L;         // 31Q23
  Qmp6988->ik.a2 = 16889L * (int)Qmp6988->qmp6988_cali.COE_a2 - 87619360L;          // 30Q47

  Qmp6988->ik.bt1 = 2982L * (int64_t)Qmp6988->qmp6988_cali.COE_bt1 + 107370906L;    // 28Q15
  Qmp6988->ik.bt2 = 329854L * (int64_t)Qmp6988->qmp6988_cali.COE_bt2 + 108083093L;  // 34Q38
  Qmp6988->ik.bp1 = 19923L * (int64_t)Qmp6988->qmp6988_cali.COE_bp1 + 1133836764L;  // 31Q20
  Qmp6988->ik.b11 = 2406L * (int64_t)Qmp6988->qmp6988_cali.COE_b11 + 118215883L;    // 28Q34
  Qmp6988->ik.bp2 = 3079L * (int64_t)Qmp6988->qmp6988_cali.COE_bp2 - 181579595L;    // 29Q43
  Qmp6988->ik.b12 = 6846L * (int64_t)Qmp6988->qmp6988_cali.COE_b12 + 85590281L;     // 29Q53
  Qmp6988->ik.b21 = 13836L * (int64_t)Qmp6988->qmp6988_cali.COE_b21 + 79333336L;    // 29Q60
  Qmp6988->ik.bp3 = 2915L * (int64_t)Qmp6988->qmp6988_cali.COE_bp3 + 157155561L;    // 28Q65

  QMP6988_LOG("<----------- int calibration data -------------->\r\n");
  QMP6988_LOG("a0[%d]	a1[%d] a2[%d] b00[%d]\r\n", Qmp6988->ik.a0,
              Qmp6988->ik.a1, Qmp6988->ik.a2, Qmp6988->ik.b00);
  QMP6988_LOG("bt1[%lld]	bt2[%lld]	bp1[%lld]	b11[%lld]\r\n",
              Qmp6988->ik.bt1, Qmp6988->ik.bt2, Qmp6988->ik.bp1, Qmp6988->ik.b11);
  QMP6988_LOG("bp2[%lld]	b12[%lld]	b21[%lld]	bp3[%lld]\r\n",
              Qmp6988->ik.bp2, Qmp6988->ik.b12, Qmp6988->ik.b21, Qmp6988->ik.bp3);
  QMP6988_LOG("<----------- int calibration data -------------->\r\n");

  return 1;
}

int16_t QMP6988ConvTx02e(qmp6988_ik_data_t* ik, int dt);
int16_t QMP6988ConvTx02e(qmp6988_ik_data_t* ik, int dt) {
  int16_t ret;
  int64_t wk1, wk2;

  // wk1: 60Q4 // bit size
  wk1 = ((int64_t)ik->a1 * (int64_t)dt);         // 31Q23+24-1=54 (54Q23)
  wk2 = ((int64_t)ik->a2 * (int64_t)dt) >> 14;   // 30Q47+24-1=53 (39Q33)
  wk2 = (wk2 * (int64_t)dt) >> 10;               // 39Q33+24-1=62 (52Q23)
  wk2 = ((wk1 + wk2) / 32767) >> 19;             // 54,52->55Q23 (20Q04)
  ret = (int16_t)((ik->a0 + wk2) >> 4);          // 21Q4 -> 17Q0
  return ret;
}

int QMP6988GetPressure02e(qmp6988_ik_data_t* ik, int dp, int16_t tx);
int QMP6988GetPressure02e(qmp6988_ik_data_t* ik, int dp, int16_t tx) {
  int ret;
  int64_t wk1, wk2, wk3;

  // wk1 = 48Q16 // bit size
  wk1 = ((int64_t)ik->bt1 * (int64_t)tx);        // 28Q15+16-1=43 (43Q15)
  wk2 = ((int64_t)ik->bp1 * (int64_t)dp) >> 5;   // 31Q20+24-1=54 (49Q15)
  wk1 += wk2;                                    // 43,49->50Q15
  wk2 = ((int64_t)ik->bt2 * (int64_t)tx) >> 1;   // 34Q38+16-1=49 (48Q37)
  wk2 = (wk2 * (int64_t)tx) >> 8;                // 48Q37+16-1=63 (55Q29)
  wk3 = wk2;                                     // 55Q29
  wk2 = ((int64_t)ik->b11 * (int64_t)tx) >> 4;   // 28Q34+16-1=43 (39Q30)
  wk2 = (wk2 * (int64_t)dp) >> 1;                // 39Q30+24-1=62 (61Q29)
  wk3 += wk2;                                    // 55,61->62Q29
  wk2 = ((int64_t)ik->bp2 * (int64_t)dp) >> 13;  // 29Q43+24-1=52 (39Q30)
  wk2 = (wk2 * (int64_t)dp) >> 1;                // 39Q30+24-1=62 (61Q29)
  wk3 += wk2;                                    // 62,61->63Q29
  wk1 += wk3 >> 14;                              // Q29 >> 14 -> Q15
  wk2 = ((int64_t)ik->b12 * (int64_t)tx);        // 29Q53+16-1=45 (45Q53)
  wk2 = (wk2 * (int64_t)tx) >> 22;               // 45Q53+16-1=61 (39Q31)
  wk2 = (wk2 * (int64_t)dp) >> 1;                // 39Q31+24-1=62 (61Q30)
  wk3 = wk2;                                     // 61Q30
  wk2 = ((int64_t)ik->b21 * (int64_t)tx) >> 6;   // 29Q60+16-1=45 (39Q54)
  wk2 = (wk2 * (int64_t)dp) >> 23;               // 39Q54+24-1=62 (39Q31)
  wk2 = (wk2 * (int64_t)dp) >> 1;                // 39Q31+24-1=62 (61Q20)
  wk3 += wk2;                                    // 61,61->62Q30
  wk2 = ((int64_t)ik->bp3 * (int64_t)dp) >> 12;  // 28Q65+24-1=51 (39Q53)
  wk2 = (wk2 * (int64_t)dp) >> 23;               // 39Q53+24-1=62 (39Q30)
  wk2 = (wk2 * (int64_t)dp);                     // 39Q30+24-1=62 (62Q30)
  wk3 += wk2;                                    // 62,62->63Q30
  wk1 += wk3 >> 15;                              // Q30 >> 15 = Q15
  wk1 /= 32767L;
  wk1 >>= 11;                                    // Q15 >> 7 = Q4
  wk1 += ik->b00;                                // Q4 + 20Q4
  // wk1 >>= 4;                                    // 28Q4 -> 24Q0
  ret = (int)wk1;
  return ret;
}

void QMP6988Reset(void) {
  uint8_t ret = QMP6988I2cWrite(QMP6988_RESET_REG, 0xe6);
  if (0 == ret) {
    QMP6988_LOG("reset fail!!! \r\n");
  }
  QMP6988I2cWrite(QMP6988_RESET_REG, 0x00);
}

void QMP6988SetpPowermode(int power_mode) {

  QMP6988_LOG("qmp_set_powermode %d \r\n", power_mode);

  Qmp6988->power_mode = power_mode;
  uint8_t data;
  QMP6988I2cReadBuffer(QMP6988_CTRLMEAS_REG, &data, 1);
  data = data & 0xfc;
  if (power_mode == QMP6988_SLEEP_MODE) {
    data |= 0x00;
  } else if (power_mode == QMP6988_FORCED_MODE) {
    data |= 0x01;
  } else if (power_mode == QMP6988_NORMAL_MODE) {
    data |= 0x03;
  }
  QMP6988I2cWrite(QMP6988_CTRLMEAS_REG, data);

  QMP6988_LOG("qmp_set_powermode 0xf4=0x%x \r\n", data);
}

void QMP6988SetFilter(unsigned char filter) {
  uint8_t data = (filter & 0x03);
  QMP6988I2cWrite(QMP6988_CONFIG_REG, data);
}

void QMP6988SetOversamplingP(unsigned char oversampling_p) {
  uint8_t data;
  QMP6988I2cReadBuffer(QMP6988_CTRLMEAS_REG, &data, 1);
  data &= 0xe3;
  data |= (oversampling_p << 2);
  QMP6988I2cWrite(QMP6988_CTRLMEAS_REG, data);
}

void QMP6988SetOversamplingT(unsigned char oversampling_t) {
  uint8_t data;
  QMP6988I2cReadBuffer(QMP6988_CTRLMEAS_REG, &data, 1);
  data &= 0x1f;
  data |= (oversampling_t << 5);
  QMP6988I2cWrite(QMP6988_CTRLMEAS_REG, data);
}

bool QMP6988ValidChip(void) {
  uint8_t data;
  QMP6988I2cReadBuffer(QMP6988_CHIP_ID_REG, &data, 1);

  QMP6988_LOG("chip id=0x%x \r\n", data);

  return (QMP6988_CHIP_ID == data);
}

/*********************************************************************************************/

void QMP6988CalcPressureAndTemperature(void) {
  uint8_t a_data_uint8_tr[6] = {0};
  uint8_t err = QMP6988I2cReadBuffer(QMP6988_PRESSURE_MSB_REG, a_data_uint8_tr, 6);
  if (err == 0) {
    QMP6988_LOG("qmp6988 read press raw error! \r\n");
    return;
  }
  uint32_t T_read = (uint32_t)((((uint32_t)(a_data_uint8_tr[3])) << 16) | (((uint16_t)(a_data_uint8_tr[4])) << 8) | (a_data_uint8_tr[5]));
  int T_raw  = (int)(T_read - SUBTRACTOR);
  int T_int = QMP6988ConvTx02e(&(Qmp6988->ik), T_raw);

  uint32_t P_read = (uint32_t)((((uint32_t)(a_data_uint8_tr[0])) << 16) | (((uint16_t)(a_data_uint8_tr[1])) << 8) | (a_data_uint8_tr[2]));
  int P_raw  = (int)(P_read - SUBTRACTOR);
  int P_int = QMP6988GetPressure02e(&(Qmp6988->ik), P_raw, T_int);

  Qmp6988->temperature = (float)T_int / 256.0f;
  Qmp6988->pressure    = (float)P_int / 1600.0f;
}

void Qmp6988Detect(void) {
  Qmp6988 = (qmp6988_data_t*)calloc(sizeof(qmp6988_data_t), 1);  // Need calloc to reset registers to 0/false
  if (nullptr == Qmp6988) { return; }

  const uint8_t addresses[2] = { QMP6988_ADDRESS_L, QMP6988_ADDRESS_H };
  for (uint32_t bus = 0; bus < 2; bus++) {
    for (uint32_t i = 0; i < sizeof(addresses); i++) {
      if (!I2cSetDevice(addresses[i], bus)) { continue; }
      Qmp6988->address = addresses[i];
      Qmp6988->bus = bus;
      if (QMP6988ValidChip()) {
        QMP6988Reset();
        QMP6988GetCalibrationData();
        QMP6988SetpPowermode(QMP6988_NORMAL_MODE);
        QMP6988SetFilter(QMP6988_FILTERCOEFF_4);
        QMP6988SetOversamplingP(QMP6988_OVERSAMPLING_8X);
        QMP6988SetOversamplingT(QMP6988_OVERSAMPLING_1X);
        I2cSetActiveFound(Qmp6988->address, "QMP6988", Qmp6988->bus);
        return;
      }
    }
  }

  free(Qmp6988);
  Qmp6988 = nullptr;
}

void Qmp6988Show(bool json) {
  float qmp_temperature = ConvertTemp(Qmp6988->temperature);
  float qmp_pressure = ConvertPressure(Qmp6988->pressure);
  float qmp_sealevel = ConvertPressureForSeaLevel(Qmp6988->pressure);

  char pressure[33];
  dtostrfd(qmp_pressure, Settings->flag2.pressure_resolution, pressure);
  char sea_pressure[33];
  dtostrfd(qmp_sealevel, Settings->flag2.pressure_resolution, sea_pressure);

  if (json) {
    ResponseAppend_P(PSTR(",\"QMP6988\":{\"" D_JSON_TEMPERATURE "\":%*_f,\"" D_JSON_PRESSURE "\":%s"),
      Settings->flag2.temperature_resolution, &qmp_temperature, pressure);
    if (Settings->altitude != 0) {
      ResponseAppend_P(PSTR(",\"" D_JSON_PRESSUREATSEALEVEL "\":%s"), sea_pressure);
    }
    ResponseJsonEnd();
#ifdef USE_DOMOTICZ
    // Domoticz and knx only support one temp sensor
    if ((0 == TasmotaGlobal.tele_period)) {
      DomoticzFloatSensor(DZ_TEMP, qmp_temperature);
    }
#endif // USE_DOMOTICZ
#ifdef USE_WEBSERVER
  } else {
    WSContentSend_Temp("QMP6988", qmp_temperature);
    WSContentSend_PD(HTTP_SNS_PRESSURE, "QMP6988", pressure, PressureUnit().c_str());
    if (Settings->altitude != 0) {
      WSContentSend_PD(HTTP_SNS_SEAPRESSURE, "QMP6988", sea_pressure, PressureUnit().c_str());
    }
#endif // USE_WEBSERVER
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns28(uint32_t function) {
  if (!I2cEnabled(XI2C_88)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    Qmp6988Detect();
  }
  else if (Qmp6988) {
    switch (function) {
      case FUNC_EVERY_SECOND:
        QMP6988CalcPressureAndTemperature();
        break;
      case FUNC_JSON_APPEND:
        Qmp6988Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        Qmp6988Show(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_QMP6988
#endif  // USE_I2C
