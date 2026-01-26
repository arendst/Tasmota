/*
  xdrv_119_i2c_ap33772s.ino - AP33772S USB PD Sink Controller support for Tasmota

  SPDX-FileCopyrightText: 2025 Theo Arends

  SPDX-License-Identifier: GPL-3.0-only
*/

#ifdef USE_I2C
#ifdef USE_AP33772S
/*********************************************************************************************\
 * AP33772S - USB Power Delivery (PD) Sink Controller
 *
 * SPR - Standard Power Range = PD3.0
 *       Fixed 3A: 5V, 9V, 12V, 15V, 20V (15-60W) and 5A: 20V (60-100W)
 *       PPS - Programmable Power Supply (3V3 to 21V in steps of 20mV (AP33772S = 100mV))
 * EPR - Extended Power Range = PD3.1
 *       Fixed 5A: 28V, 36V, 48V (100-240W)
 *       AVS - Adjustable Voltage Supply (15V to 28V or 48V (AP33772S = 28V) in steps of 100mV (AP33772S = 200mV))
 * PDO - Power Delivery Object or Power Data Object
 *
 * Datasheet https://www.diodes.com/datasheet/download/AP33772S.pdf
 * Library https://github.com/CentyLab/AP33772S-Cpp
 * Hardware https://github.com/CentyLab/RotoPD
 *
 * Tested using:
 * UGreen CD226 100W 4-Port USB GaN Fast Charger (PPS only)
 * Novoo NCEU120D-255C2 120W 3-Port USB GaN Fast Charger (PPS only)
 *
 * KOWSI KWS-X1 USB type C Voltage and Current monitor (PPS and AVS)
 *
 * I2C Address: 0x52
\*********************************************************************************************/

#define XDRV_119                  119
#define XI2C_93                   93    // See I2CDEVICES.md

//#define AP33772S_DEBUG

#define AP33772S_I2C_ADDR         0x52
#define AP33772S_MAX_PDO_ENTRIES  13    // Maximum number of PDO entries

#define AP33772S_CMD_STATUS       0x01  // Reset to 0 after every Read
#define AP33772S_CMD_MASK         0x02
#define AP33772S_CMD_OPMODE       0x03
#define AP33772S_CMD_CONFIG       0x04
#define AP33772S_CMD_PDCONFIG     0x05
#define AP33772S_CMD_SYSTEM       0x06

// Temperature setting register
#define AP33772S_CMD_TR25         0x0C 
#define AP33772S_CMD_TR50         0x0D 
#define AP33772S_CMD_TR75         0x0E
#define AP33772S_CMD_TR100        0x0F

// Power reading related
#define AP33772S_CMD_VOLTAGE      0x11
#define AP33772S_CMD_CURRENT      0x12
#define AP33772S_CMD_TEMP         0x13
#define AP33772S_CMD_VREQ         0x14
#define AP33772S_CMD_IREQ         0x15

#define AP33772S_CMD_VSELMIN      0x16  // Minimum Selection Voltage
#define AP33772S_CMD_UVPTHR       0x17
#define AP33772S_CMD_OVPTHR       0x18
#define AP33772S_CMD_OCPTHR       0x19
#define AP33772S_CMD_OTPTHR       0x1A
#define AP33772S_CMD_DRTHR        0x1B

#define AP33772S_CMD_SRCPDO       0x20

#define AP33772S_CMD_PD_REQMSG    0x31
#define AP33772S_CMD_PD_CMDMSG    0x32
#define AP33772S_CMD_PD_MSGRLT    0x33

typedef struct RDO_DATA_t {
  union {
    struct {
      unsigned int VOLTAGE_SEL: 8;   // Bits 7:0, Output Voltage Select
      unsigned int CURRENT_SEL: 4;   // Bits 11:8, Operating Current Select
      unsigned int PDO_INDEX: 4;     // Bits 15:12, Source PDO index select
    } REQMSG_Fields;
    struct {
      byte byte0;
      byte byte1;
    };
  unsigned long data;
  };
} RDO_DATA_t;

typedef struct SRC_SPRandEPR_PDO_Fields_t {
  union {
    struct {
      unsigned int voltage_max: 8;   // Bits 7:0, VOLTAGE_MAX field
      unsigned int peak_current: 2;  // Bits 9:8, PEAK_CURRENT field
      unsigned int current_max: 4;   // Bits 13:10, CURRENT_MAX field
      unsigned int type: 1;          // Bit 14, TYPE field
      unsigned int detect: 1;        // Bit 15, DETECT field
    } fixed;
    struct {
      unsigned int voltage_max: 8;   // Bits 7:0, VOLTAGE_MAX field
      unsigned int voltage_min: 2;   // Bits 9:8, VOLTAGE_MIN field
      unsigned int current_max: 4;   // Bits 13:10, CURRENT_MAX field
      unsigned int type: 1;          // Bit 14, TYPE field
      unsigned int detect: 1;        // Bit 15, DETECT field
    } pps;
    struct {
      unsigned int voltage_max: 8;   // Bits 7:0, VOLTAGE_MAX field
      unsigned int voltage_min: 2;   // Bits 9:8, VOLTAGE_MIN field
      unsigned int current_max: 4;   // Bits 13:10, CURRENT_MAX field
      unsigned int type: 1;          // Bit 14, TYPE field
      unsigned int detect: 1;        // Bit 15, DETECT field
    } avs;
    struct {
      uint8_t byte0;
      uint8_t byte1;
    };
  };
  unsigned long data;
} SRC_SPRandEPR_PDO_Fields_t;

typedef struct Ap33772s_t {
  SRC_SPRandEPR_PDO_Fields_t SRC_SPRandEPRpdoArray[AP33772S_MAX_PDO_ENTRIES]; 

  int voltage_PPS;
  int current_PPS;

  int voltage_AVS_byte;
  int current_AVS_byte;
  int index_AVS;

  int slider_voltage;
  int slider_current;

  uint8_t read_buffer[2];
  uint8_t write_buffer[2];
  int8_t index_PPS_user;
  int8_t index_AVS_user;
  bool output;
  uint8_t bus;
} Ap33772s_t;
Ap33772s_t* Ap33772s = nullptr;

/*********************************************************************************************/

#ifdef AP33772S_DEBUG

String AP33772S_DisplayCurrentRange(uint32_t current_max) {
  String result = "Invalid value";
  switch (current_max) {
    case 0:
      result = "0.00A ~ 1.24A (Less than)";
      break;
    case 1:
      result = "1.25A ~ 1.49A";
      break;
    case 2:
      result = "1.50A ~ 1.74A";
      break;
    case 3:
      result = "1.75A ~ 1.99A";
      break;
    case 4:
      result = "2.00A ~ 2.24A";
      break;
    case 5:
      result = "2.25A ~ 2.49A";
      break;
    case 6:
      result = "2.50A ~ 2.74A";
      break;
    case 7:
      result = "2.75A ~ 2.99A";
      break;
    case 8:
      result = "3.00A ~ 3.24A";
      break;
    case 9:
      result = "3.25A ~ 3.49A";
      break;
    case 10:
      result = "3.50A ~ 3.74A";
      break;
    case 11:
      result = "3.75A ~ 3.99A";
      break;
    case 12:
      result = "4.00A ~ 4.24A";
      break;
    case 13:
      result = "4.25A ~ 4.49A";
      break;
    case 14:
      result = "4.50A ~ 4.99A";
      break;
    case 15:
      result = "5.00A ~ (More than)";
      break;
    default:
      break;
  }
  return result;
}

String AP33772S_DisplayEPRVoltageMin(uint32_t current_max) {
  String result = "Invalid value";
  switch (current_max) {
    case 0:
      result = "Reserved";
      break;
    case 1:
      result = "15000mV ~";
      break;
    case 2:
      result = "15000mV < VOLTAGE_MIN ≤ 20000mV ";
      break;
    case 3:
      result = "others";
      break;
    default:
      break;
  }
  return result;
}

String AP33772S_DisplaySPRVoltageMin(uint32_t current_max) {
  String result = "Invalid value";
  switch (current_max) {
    case 0:
      result = "Reserved";
      break;
    case 1:
      result = "3300mV ~";
      break;
    case 2:
      result = "3300mV < VOLTAGE_MIN ≤ 5000mV ";
      break;
    case 3:
      result = "others";
      break;
    default:
      break;
  }
  return result;
}

/**
 * @brief Decode PDO information from SRC_SPRandEPR_PDO_Fields
 * @param pdoIndex feed from loop
 */
void AP33772S_DisplayPDOInfo(int pdoIndex) {
  // Determine if it's SPR or EPR based on pdoIndex
  bool isEPR = (pdoIndex >= 7 && pdoIndex <= 12);  // 1-6 for SPR, 7-12 for EPR
  // Check if both bytes are zero
  if (Ap33772s->SRC_SPRandEPRpdoArray[pdoIndex].byte0 == 0 && Ap33772s->SRC_SPRandEPRpdoArray[pdoIndex].byte1 == 0) {
    return;  // If both bytes are zero, exit the function
  }
  
  // Print the PDO type and index
  // Now, the individual fields can be accessed through the union in the struct
  if (Ap33772s->SRC_SPRandEPRpdoArray[pdoIndex].fixed.type == 0) {  // Fixed PDO
    // Print parsed values
    AddLog(LOG_LEVEL_DEBUG, PSTR("AP3: %s_PDO%d Fixed_PDO %dmV %s"),
      (pdoIndex <= 6) ? "SPR" : "EPR",
      pdoIndex+1,
      Ap33772s->SRC_SPRandEPRpdoArray[pdoIndex].fixed.voltage_max * (isEPR ? 200 : 100),                 // Voltage in 200mV units for EPR, 100mV for SPR
      AP33772S_DisplayCurrentRange(Ap33772s->SRC_SPRandEPRpdoArray[pdoIndex].fixed.current_max).c_str()  // Assuming displayCurrentRange function is available
    );
  } else {  // PPS or AVS PDO
    // Print parsed values
    AddLog(LOG_LEVEL_DEBUG, PSTR("AP3: %s_PDO%d %s_PDO %s %dmV %s"),
      (pdoIndex <= 6) ? "SPR" : "EPR",
      pdoIndex+1,
      (isEPR) ? "AVS" : "PPS",
      (isEPR) ? AP33772S_DisplayEPRVoltageMin(Ap33772s->SRC_SPRandEPRpdoArray[pdoIndex].avs.voltage_min).c_str() :  // Assuming displayVoltageMin function is available
                AP33772S_DisplaySPRVoltageMin(Ap33772s->SRC_SPRandEPRpdoArray[pdoIndex].pps.voltage_min).c_str(),   // Assuming displayVoltageMin function is available
      Ap33772s->SRC_SPRandEPRpdoArray[pdoIndex].fixed.voltage_max * (isEPR ? 200 : 100),                            // Maximum Voltage in 200mV units for EPR, 100mV for SPR
      AP33772S_DisplayCurrentRange(Ap33772s->SRC_SPRandEPRpdoArray[pdoIndex].fixed.current_max).c_str()             // Assuming displayCurrentRange function is available
    );

  }
}

#endif  // AP33772S_DEBUG

/*********************************************************************************************/

void AP33772S_I2cRead(uint8_t cmdAddr, byte len) {
  // clear readBuffer
  for (uint32_t i = 0; i < 2; i++) {
    Ap33772s->read_buffer[i] = 0;
  }
  I2cReadBuffer(AP33772S_I2C_ADDR, cmdAddr, Ap33772s->read_buffer, len, Ap33772s->bus);
}

void AP33772S_I2cWrite(byte cmdAddr, byte len) {
  I2cWriteBuffer(AP33772S_I2C_ADDR, cmdAddr, Ap33772s->write_buffer, len, Ap33772s->bus);
}

/*********************************************************************************************/

/**
 * @brief take in current in mA unit
 * @return value from 0 to 15
 * @return -1 if there is an error
 */
int AP33772S_CurrentMap(int current) {
  // Check if the value is out of bounds
  if (current < 0 || current > 5000) {
    return -1; // Return -1 for invalid inputs
  }

  // If value is below 1250, return 0
  if (current < 1250) {
    return 0;
  }

  // Calculate the result for ranges above 1250
  return ((current - 1250) / 250) + 1;
}

/**
 * @brief Request fixed PDO voltage, work for both standard and EPR mode
 * @param pdoIndex index 1
 * @param max_current unit in mA
 */
void AP33772S_SetFixPDO(int pdoIndex, int max_current) {
  RDO_DATA_t rdoData;

  // Max current sanity check
  if (max_current <= 0) { return; }

  // For Fix voltage, only need to set PDO_INDEX and CURRENT_SEL
  // No need to change the selected voltage
  // handle the same in standard as well as EPR

  // PDO index need to be fixed type
  if (Ap33772s->SRC_SPRandEPRpdoArray[pdoIndex-1].fixed.type == 0) {
    // Now that we are in fix PDO mode
//    AddLog(LOG_LEVEL_DEBUG, PSTR("AP3: Type is fixed"));

    rdoData.REQMSG_Fields.PDO_INDEX = pdoIndex;  // Index 1

    // Serial.printf("You entered current: %dmA", max_current); //DEBUG
    // Serial.println(SRC_SPRandEPRpdoArray[pdoIndex].fixed.current_max); //DEBUG
    // Serial.println(currentMap(max_current)); //DEBUG
    if (AP33772S_CurrentMap(max_current) > Ap33772s->SRC_SPRandEPRpdoArray[pdoIndex-1].fixed.current_max) {
      AddLog(LOG_LEVEL_DEBUG, PSTR("AP3: Current not in range"));
      return; // Check if current setting is in range
    }

    rdoData.REQMSG_Fields.CURRENT_SEL = AP33772S_CurrentMap(max_current);
    // Note: For profile less than or equal to 3A power, CURRENT_SEL = 9 will not work.
    // rdoData.REQMSG_Fields.CURRENT_SEL = 9; 

    Ap33772s->write_buffer[0] = rdoData.byte0;   // Store the upper 8 bits
    Ap33772s->write_buffer[1] = rdoData.byte1;   // Store the lower 8 bits
    AP33772S_I2cWrite(AP33772S_CMD_PD_REQMSG, 2);
  }
}

/**
 * @brief Request PPS voltage
 * @param pdoIndex index 1
 * @param target_voltage unit in mV
 * @param max_current unit in mA
 * @bug only work if min PPS voltage is 3.3V
 */
void AP33772S_SetPPSPDO(int pdoIndex, int target_voltage, int max_current) {
  RDO_DATA_t rdoData;
  
  int voltage_min_decoded;
  // Sanity check include, check if the value is in EPR range (index < 8) and also AVS mode
  if ((pdoIndex < 8) && (Ap33772s->SRC_SPRandEPRpdoArray[pdoIndex-1].pps.type == 1)) {
//    AddLog(LOG_LEVEL_DEBUG, PSTR("AP3: Type is PPS"));
    // Now that we are in PPS mode

    rdoData.REQMSG_Fields.PDO_INDEX = pdoIndex;  // Index 1

    if (AP33772S_CurrentMap(max_current) > Ap33772s->SRC_SPRandEPRpdoArray[pdoIndex-1].pps.current_max) {
      AddLog(LOG_LEVEL_DEBUG, PSTR("AP3: PPS Current not in range"));
      return;                                    // Check if current setting is in range
    }

    // Decode voltage_min
    if (Ap33772s->SRC_SPRandEPRpdoArray[pdoIndex-1].pps.voltage_min > 0) {
      voltage_min_decoded = 3300;
    }

    if ((target_voltage < voltage_min_decoded) || 
        (target_voltage > Ap33772s->SRC_SPRandEPRpdoArray[pdoIndex-1].pps.voltage_max*100)) {
      AddLog(LOG_LEVEL_DEBUG, PSTR("AP3: PPS Voltage not in range"));
      return;                                    // Check if current setting is in range
    }

    rdoData.REQMSG_Fields.VOLTAGE_SEL = target_voltage/100;  // Output Voltage in 200mV units
    rdoData.REQMSG_Fields.CURRENT_SEL = AP33772S_CurrentMap(max_current);

    Ap33772s->write_buffer[0] = rdoData.byte0;   // Store the upper 8 bits
    Ap33772s->write_buffer[1] = rdoData.byte1;   // Store the lower 8 bits
    AP33772S_I2cWrite(AP33772S_CMD_PD_REQMSG, 2);
  }
}

/**
 * @brief Request AVS voltage
 * @param pdoIndex index 1
 * @param target_voltage unit in mV
 * @param max_current unit in mA
 * @bug only work if min AVS voltage is 15V, AVS max voltage is not capped at 30V
 */
void AP33772S_SetAVSPDO(int pdoIndex, int target_voltage, int max_current) {
  RDO_DATA_t rdoData;

  int voltage_min_decoded;
  // Sanity check include, check if the value is in EPR range (index >= 8) and also AVS mode
  if ((pdoIndex >= 8) && (Ap33772s->SRC_SPRandEPRpdoArray[pdoIndex-1].avs.type == 1)) {
//    AddLog(LOG_LEVEL_DEBUG, PSTR("AP3: Type is AVS"));
    // Now that we are in AVS mode

    // Decode voltage_min
    if (Ap33772s->SRC_SPRandEPRpdoArray[pdoIndex-1].avs.voltage_min > 0) {
      voltage_min_decoded = 15000;
    }

    rdoData.REQMSG_Fields.PDO_INDEX = pdoIndex;  // Index 1

    if (AP33772S_CurrentMap(max_current) > Ap33772s->SRC_SPRandEPRpdoArray[pdoIndex-1].avs.current_max) {
      AddLog(LOG_LEVEL_DEBUG, PSTR("AP3: AVS Current not in range."));
      return;                                    // Check if current setting is in range
    }

    // Decode voltage_min
    if (Ap33772s->SRC_SPRandEPRpdoArray[pdoIndex-1].avs.voltage_min > 0) {
      voltage_min_decoded = 15000;
    }

    if ((target_voltage < voltage_min_decoded) || 
        (target_voltage > Ap33772s->SRC_SPRandEPRpdoArray[pdoIndex-1].avs.voltage_max*200)) {
      AddLog(LOG_LEVEL_DEBUG, PSTR("AP3: AVS Voltage not in range"));
      return;                                    // Check if current setting is in range
    }

    rdoData.REQMSG_Fields.VOLTAGE_SEL = target_voltage/200;  // Output Voltage in 200mV units
    rdoData.REQMSG_Fields.CURRENT_SEL = AP33772S_CurrentMap(max_current);

    Ap33772s->write_buffer[0] = rdoData.byte0;   // Store the upper 8 bits
    Ap33772s->write_buffer[1] = rdoData.byte1;   // Store the lower 8 bits
    AP33772S_I2cWrite(AP33772S_CMD_PD_REQMSG, 2);

    Ap33772s->index_AVS = rdoData.REQMSG_Fields.PDO_INDEX;
    Ap33772s->voltage_AVS_byte = rdoData.REQMSG_Fields.VOLTAGE_SEL; 
    Ap33772s->current_AVS_byte = rdoData.REQMSG_Fields.CURRENT_SEL;
  }
  return;
}

/**
 * @brief Set resistance value of 10K NTC at 25C, 50C, 75C and 100C.
 *          Default is 10000, 4161, 1928, 974Ohm
 * @param TR25, TR50, TR75, TR100 unit in Ohm
 * @attention Blocking function due to long I2C write, min blocking time 15ms
 */
void AP33772S_SetNTC(int TR25, int TR50, int TR75, int TR100) {
  Ap33772s->write_buffer[0] = TR25 & 0xff;
  Ap33772s->write_buffer[1] = (TR25 >> 8) & 0xff;
  AP33772S_I2cWrite(AP33772S_CMD_TR25, 2);
  delay(5);
  Ap33772s->write_buffer[0] = TR50 & 0xff;
  Ap33772s->write_buffer[1] = (TR50 >> 8) & 0xff;
  AP33772S_I2cWrite(AP33772S_CMD_TR50, 2);
  delay(5);
  Ap33772s->write_buffer[0] = TR75 & 0xff;
  Ap33772s->write_buffer[1] = (TR75 >> 8) & 0xff;
  AP33772S_I2cWrite(AP33772S_CMD_TR75, 2);
  delay(5);
  Ap33772s->write_buffer[0] = TR100 & 0xff;
  Ap33772s->write_buffer[1] = (TR100 >> 8) & 0xff;
  AP33772S_I2cWrite(AP33772S_CMD_TR100, 2);
}

/**
 * @brief Read NTC temperature
 * @return tempearture in C
 */
int AP33772S_ReadTemp(void) {
  AP33772S_I2cRead(AP33772S_CMD_TEMP, 1);
  return Ap33772s->read_buffer[0];               // I2C read return 1C/LSB
}

/**
 * @brief Read VBUS voltage
 * @return voltage in mV
 */
int AP33772S_ReadVoltage(void) {
  AP33772S_I2cRead(AP33772S_CMD_VOLTAGE, 2);
  return ((Ap33772s->read_buffer[1] << 8) | Ap33772s->read_buffer[0]) * 80;  // I2C read return 80mV/LSB
}

/**
 * @brief Read VBUS current
 * @return current in mA
 */
int AP33772S_ReadCurrent(void) {
  AP33772S_I2cRead(AP33772S_CMD_CURRENT, 1);
  return Ap33772s->read_buffer[0] * 24;          // I2C read return 24mA/LSB
}

/**
 * @brief Read VREQ The latest requested voltage negotiated with the source
 * @return voltage in mV
 */
int AP33772S_ReadVREQ(void) {
  AP33772S_I2cRead(AP33772S_CMD_VREQ, 1);
  return Ap33772s->read_buffer[0] * 50;          // I2C read return 50mV/LSB
}

/**
 * @brief Read IREQ The latest requested voltage negotiated with the source
 * @return current in mA
 */
int AP33772S_ReadIREQ(void) {
  AP33772S_I2cRead(AP33772S_CMD_IREQ, 1);
  return Ap33772s->read_buffer[0] * 10;          // I2C read return 10mA/LSB
}

/**
 * @brief Read VSELMIN register. The Minimum Selection Voltage
 * @return voltage in mV
 */
int AP33772S_ReadVSELMIN(void) {
  AP33772S_I2cRead(AP33772S_CMD_VSELMIN, 1);
  return Ap33772s->read_buffer[0] * 200;         // I2C read return 200mV/LSB
}

/**
 * @brief Set VSELMIN register. The Minimum Selection Voltage
 * @param voltage in mV
 */
void AP33772S_SetVSELMIN(int voltage) {
  Ap33772s->write_buffer[0] = voltage / 200;     // 200mV/LSB
  AP33772S_I2cWrite(AP33772S_CMD_VSELMIN, 1);
}

/**
 * @brief Read UVP Threshold, percentage(%) of VREQ
 * @return percentage, should only return 80%, 75%, or 70%. -1 for error
 */
int AP33772S_ReadUVPTHR(void) {
  AP33772S_I2cRead(AP33772S_CMD_UVPTHR, 1);
  switch(Ap33772s->read_buffer[0]) {
    case 1:
      return 80;
    case 2:
      return 75;
    case 3:
      return 70;
  }
  return -1;
}

/**
 * @brief Set UVP Threshold, percentage(%) of VREQ
 * @param value percentage. If 80% then value = 80
 */
void AP33772S_SetUVPTHR(int value) {
  if ((value >= 70) && (value <= 80)) {
    switch(value) {
      case 80:
        Ap33772s->write_buffer[0] = 1;
        break;
      case 75:
        Ap33772s->write_buffer[0] = 2;
        break;
      case 70:
        Ap33772s->write_buffer[0] = 3;
        break;
      default:
        return;                                  // Error
    }
    AP33772S_I2cWrite(AP33772S_CMD_UVPTHR, 1);
  }
}

/**
 * @brief Read OVP Threshold Voltage is the VREQ voltage plus OVPTHR offset voltage (mV)
 * @return voltage in mV
 */
int AP33772S_ReadOVPTHR(void) {
  AP33772S_I2cRead(AP33772S_CMD_OVPTHR, 1);
  return Ap33772s->read_buffer[0] * 80;          // I2C read return 80mV/LSB
}

/**
 * @brief Set OVP Threshold Voltage is the VREQ voltage plus OVPTHR offset voltage (mV)
 * @param voltage in mV
 */
void AP33772S_SetOVPTHR(int value) {
  Ap33772s->write_buffer[0] = value / 80;        // 80mV/LSB
  AP33772S_I2cWrite(AP33772S_CMD_OVPTHR, 1);
}

int AP33772S_ReadOCPTHR(void) {
  AP33772S_I2cRead(AP33772S_CMD_OCPTHR, 1);
  return Ap33772s->read_buffer[0] * 50;          // I2C read return 50mA/LSB
}

void AP33772S_SetOCPTHR(int value) {
  Ap33772s->write_buffer[0] = value/50;          // 50mA/LSB
  AP33772S_I2cWrite(AP33772S_CMD_OCPTHR, 1);
}

int AP33772S_ReadOTPTHR(void) {
  AP33772S_I2cRead(AP33772S_CMD_OTPTHR, 1);
  return Ap33772s->read_buffer[0];               // I2C read return 1C/LSB
}

void AP33772S_SetOTPTHR(int value) {
  Ap33772s->write_buffer[0] = value;             // 1C/LSB
  AP33772S_I2cWrite(AP33772S_CMD_OTPTHR, 1);
}

int AP33772S_ReadDRTHR(void) {
  AP33772S_I2cRead(AP33772S_CMD_DRTHR, 1);
  return Ap33772s->read_buffer[0];               // I2C read return 1C/LSB
}

void AP33772S_SetDRTHR(int value) {
  Ap33772s->write_buffer[0] = value;             // 1C/LSB
  AP33772S_I2cWrite(AP33772S_CMD_DRTHR, 1);
}

/**
 * @brief Turn on/off the NMOS switch
 * @param flag 0 or 1 for OFF/ON
 * @return 1 if flag make sense
 * @bug can add code to check Vout voltage to ensure on or off, worry about settle time required for VOUT
 */
bool AP33772S_SetOutput(uint8_t flag) {
  switch(flag) {
    case 0:
      Ap33772s->write_buffer[0] = 0b00010001;    // Turn off
      AP33772S_I2cWrite(AP33772S_CMD_SYSTEM, 1);
      return true;
      break; //Sanity
    case 1:
      Ap33772s->write_buffer[0] = 0b00010010;    // Turn on
      AP33772S_I2cWrite(AP33772S_CMD_SYSTEM, 1);
      return true;
      break; //Sanity
    default:
      return false;                              // Error, dont know the input
  }
}

/*********************************************************************************************/

void AP33772S_Init(void) {
  uint32_t bus;
  for (bus = 0; bus < 2; bus++) {
    if (!I2cSetDevice(AP33772S_I2C_ADDR, bus)) {
      continue;
    }
    uint8_t data[26];
    delay(100);                                  // We expect only one chip

    if (!I2cReadBuffer(AP33772S_I2C_ADDR, AP33772S_CMD_SRCPDO, data, sizeof(data), bus)) {
      Ap33772s = (Ap33772s_t*)calloc(sizeof(Ap33772s_t), 1);  // Need calloc to reset registers to 0/false
      if (nullptr == Ap33772s) { return; }
      Ap33772s->bus = bus;
      I2cSetActiveFound(AP33772S_I2C_ADDR, "AP33772S", Ap33772s->bus);

      Ap33772s->index_PPS_user = -1;
      Ap33772s->index_AVS_user = -1;
      // Store the bytes in the array of structs
      for (uint32_t i = 0; i < sizeof(data); i += 2) {
        uint32_t pdo_index = (i / 2);            // Calculate the PDO index
        Ap33772s->SRC_SPRandEPRpdoArray[pdo_index].byte0 = data[i];
        Ap33772s->SRC_SPRandEPRpdoArray[pdo_index].byte1 = data[i + 1];

        // @brief Search through the list of profile and look for PPS, AVS
        // @bug If system has 2 PPS profiles. The index only show the last one. Use displayPDOInfo() to check.
        if ((pdo_index < 7) && (Ap33772s->SRC_SPRandEPRpdoArray[pdo_index].pps.type == 1)) {
          Ap33772s->index_PPS_user = pdo_index +1;
          AddLog(LOG_LEVEL_DEBUG, PSTR("AP3: Found PPS profile %d"), Ap33772s->index_PPS_user);
        }
        else if ((pdo_index >= 7) && (Ap33772s->SRC_SPRandEPRpdoArray[pdo_index].avs.type == 1)) {
          Ap33772s->index_AVS_user = pdo_index +1;
          AddLog(LOG_LEVEL_DEBUG, PSTR("AP3: Found AVS profile %d"), Ap33772s->index_AVS_user);
        }
#ifdef AP33772S_DEBUG
        AP33772S_DisplayPDOInfo(pdo_index);
#endif  // AP33772S_DEBUG
      }

      AP33772S_SetOutput(0);         // Default output OFF
      Ap33772s->voltage_PPS = 5000;  // Default 5V
      Ap33772s->current_PPS = 3000;  // Default 3A
      AP33772S_SetPPSPDO(Ap33772s->index_PPS_user, Ap33772s->voltage_PPS, Ap33772s->current_PPS);
      break;
    }
  }
}

/**
  * Some charger will disconnect with sink if no refresh request is sent within 1s
  * Spamming request to keep power on
*/
void AP33772S_KeepAlive(void) {
  if (Ap33772s->output) {
    AP33772S_SetPPSPDO(Ap33772s->index_PPS_user, Ap33772s->voltage_PPS, Ap33772s->current_PPS);
  }
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

const char kAP33772SCommands[] PROGMEM = "PD|" // Prefix
  "Output|Voltage|Current";

void (* const AP33772SCommands[])(void) PROGMEM = {
  &CmndPDOutput, &CmndPDVoltage, &CmndPDCurrent };

void CmndPDOutput(void) {
  // PDOutput    - Show output state
  // PDOutput 0  - Turn output off
  // PDOutput 1  - Turn output on
  // PDOutput 2  - Toggle output
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 2)) {
    switch (XdrvMailbox.payload) {
    case 0: // Off
      Ap33772s->output = 0;
      break;
    case 1: // On
      Ap33772s->output = 1;
      break;
    case 2: // Toggle
      Ap33772s->output ^= 1;
      break;
    }
    AP33772S_SetOutput(Ap33772s->output);
  }
  ResponseCmndChar(GetStateText(Ap33772s->output));
}

void CmndPDVoltage(void) {
  // PDVoltage       - Show PPS voltage
  // PDVoltage 5000  - Set PPS voltage to 5000 mV or 5V
  if ((XdrvMailbox.payload >= 3300) && (XdrvMailbox.payload <= 20000)) {
    Ap33772s->voltage_PPS = XdrvMailbox.payload;
    if (Ap33772s->current_PPS < 1250) {
      Ap33772s->current_PPS = 1250;              // Default 1.25A
    }
    AP33772S_SetPPSPDO(Ap33772s->index_PPS_user, Ap33772s->voltage_PPS, Ap33772s->current_PPS);
  }
  uint32_t voltage = AP33772S_ReadVoltage();
  ResponseCmndNumber(voltage);
}

void CmndPDCurrent(void) {
  // PDCurrent       - Show max PPS current
  // PDCurrent 2000  - Set max PPS current to 2000mA or 2A
  if ((XdrvMailbox.payload >= 1250) && (XdrvMailbox.payload <= 5000)) {
    Ap33772s->current_PPS = XdrvMailbox.payload;
    if (Ap33772s->voltage_PPS < 3300) {
      Ap33772s->voltage_PPS = 5000;              // Default 5V
    }
    AP33772S_SetPPSPDO(Ap33772s->index_PPS_user, Ap33772s->voltage_PPS, Ap33772s->current_PPS);
  }
  uint32_t current = AP33772S_ReadCurrent();
  ResponseCmndNumber(current);
}

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

#ifdef USE_WEBSERVER
const char HTTP_MSG_SLIDER_AP33772S[] PROGMEM =
  "<tr>"
  "<td colspan='%d' style='width:%d%%'>"
  "<div id='d119%d' class='r' style='background-image:linear-gradient(to right,#000,#fff);font-size:10px;'>"
  "<span id='s119%d'></span> %s"
  "<input id='i119%d' type='range' min='%d' max='%d' value='%d' onchange='lc(\"i\",119%d,value)'>"
  "</div>"
  "</td>"
  "<script>"
    "eb('s119%d').innerHTML=eb('i119%d').value/1000;"
    "eb('i119%d').oninput=function(){"
      "eb('s119%d').innerHTML=this.value/1000;"
    "}"
  "</script>";

const char HTTP_MSG_BUTTON_AP33772S[] PROGMEM =
  "<td style='width:15%%'><button id='k119' style='height:46px;background:#%06x;' onclick='la(\"&k119=1\");'>PD</button></td>";

void AP33772S_WebAddMainSlider(void) {
  WSContentSend_P(HTTP_TABLE100);

  Ap33772s->slider_voltage = Ap33772s->voltage_PPS;
  WSContentSend_P(HTTP_MSG_SLIDER_AP33772S,  // Voltage
    2, 100,                      // colspan, width
    1,                           // d1191 - Unique HTML id (not used)
    1,                           // s1191 - Unique span HTML id - Used for label updates
    "V",                         // label
    1,                           // i1191 - Unique range HTML id - Used for slider updates
    3300, 20000,                 // Range 3V3 to 20V
    Ap33772s->slider_voltage,    // Range value
    1,                           // i1191 - Value id
    1, 1, 1, 1                   // s1191, i1191, i1191, s1191 - Script ids
    );
  WSContentSend_P(PSTR("</tr>"));

  Ap33772s->slider_current = Ap33772s->current_PPS;
  WSContentSend_P(HTTP_MSG_SLIDER_AP33772S,  // Current
    1, 85,                       // colspan, width
    2,                           // d1192 - Unique HTML id (not used)
    2,                           // s1192 - Unique span HTML id - Used for label updates
    "A max",                     // label
    2,                           // i1192 - Unique range HTML id - Used for slider updates
    1250, 5000,                  // Range 1.25A to 5A
    Ap33772s->slider_current,    // Range value
    2,                           // i1192 - Value id
    2, 2, 2, 2                   // s1192, i1192, i1192, s1192 - Script ids
    );
  WSContentSend_P(HTTP_MSG_BUTTON_AP33772S,  // Output
    WebColor((Ap33772s->output) ? COL_BUTTON : COL_BUTTON_OFF));
  WSContentSend_P(PSTR("</tr>"));

  WSContentSend_P(PSTR("</table>"));
}

/*-------------------------------------------------------------------------------------------*/

void AP33772S_WebUpdateButtonSliderState(void) {
  WSContentSend_P(PSTR("</table>"));           // Terminate current {t}
  WSContentSend_P(HTTP_MSG_EXEC_JAVASCRIPT);   // "<img style='display:none;' src onerror=\""
  if (Ap33772s->voltage_PPS != Ap33772s->slider_voltage) {
    if (WebUpdateSliderTime()) {
      Ap33772s->slider_voltage = Ap33772s->voltage_PPS;
    }
    WSContentSend_P(PSTR("eb('i1191').value=%d;eb('s1191').innerHTML=%d/1000;"),
      Ap33772s->voltage_PPS, Ap33772s->voltage_PPS);
  }
  if (Ap33772s->current_PPS != Ap33772s->slider_current) {
    if (WebUpdateSliderTime()) {
      Ap33772s->slider_current = Ap33772s->current_PPS;
    }
    WSContentSend_P(PSTR("eb('i1192').value=%d;eb('s1192').innerHTML=%d/1000;"),
      Ap33772s->current_PPS, Ap33772s->current_PPS);
  }
  WSContentSend_P(PSTR("eb('k119').style='height:46px;background:#%06x;'"),
    WebColor((Ap33772s->output) ? COL_BUTTON : COL_BUTTON_OFF));
  WSContentSend_P(PSTR("\">{t}"));             // Restart {t} = <table style='width:100%'>
}

/*********************************************************************************************/

void AP33772S_WebGetArg(void) {
  char tmp[8];                                 // WebGetArg numbers only
  char svalue[32];                             // Command and number parameter

  WebGetArg("i1191", tmp, sizeof(tmp));
  if (strlen(tmp)) {
    snprintf_P(svalue, sizeof(svalue), PSTR("PDVoltage %s"), tmp);
    ExecuteWebCommand(svalue);
  }
  WebGetArg("i1192", tmp, sizeof(tmp));
  if (strlen(tmp)) {
    snprintf_P(svalue, sizeof(svalue), PSTR("PDCurrent %s"), tmp);
    ExecuteWebCommand(svalue);
  }
  WebGetArg(PSTR("k119"), tmp, sizeof(tmp));
  if (strlen(tmp)) {
    snprintf_P(svalue, sizeof(svalue), PSTR("PDOutput 2"));
    ExecuteWebCommand(svalue);
  }
}
#endif  // USE_WEBSERVER

/*********************************************************************************************/

void AP33772S_Show(bool json) {
  float f_voltage = (float)AP33772S_ReadVoltage() / 1000;
  float f_current = (float)AP33772S_ReadCurrent() / 1000;
  float f_power = f_voltage * f_current;
  float f_temperature = ConvertTemp(AP33772S_ReadTemp());

  if (json) {
    ResponseAppend_P(",\"AP33772S\":{\"" D_JSON_VOLTAGE "\":%*_f,\"" D_JSON_CURRENT "\":%*_f,\"" D_JSON_POWERUSAGE "\":%*_f,\"" D_JSON_TEMPERATURE "\":%*_f}",
      Settings->flag2.voltage_resolution, &f_voltage,
      Settings->flag2.current_resolution, &f_current,
      Settings->flag2.wattage_resolution, &f_power,
      Settings->flag2.temperature_resolution, &f_temperature);
#ifdef USE_WEBSERVER
  } else {
    WSContentSend_Voltage("AP33772S", f_voltage);
    WSContentSend_Current("AP33772S", f_current);
    WSContentSend_PD(HTTP_SNS_F_POWER, "AP33772S", Settings->flag2.wattage_resolution, &f_power);
    WSContentSend_Temp("AP33772S", f_temperature);
    AP33772S_WebUpdateButtonSliderState();
#endif  // USE_WEBSERVER
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv119(uint32_t function) {
  if (!I2cEnabled(XI2C_93)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    AP33772S_Init();
  }
  else if (Ap33772s) {
    switch (function) {
      case FUNC_EVERY_250_MSECOND:
        AP33772S_KeepAlive();
        break;
      case FUNC_JSON_APPEND:
        AP33772S_Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        AP33772S_Show(0);
        break;
      case FUNC_WEB_ADD_MAIN_BUTTON:
        AP33772S_WebAddMainSlider();
        break;
      case FUNC_WEB_GET_ARG:
        AP33772S_WebGetArg();
        break;
#endif  // USE_WEBSERVER
      case FUNC_COMMAND:
        result = DecodeCommand(kAP33772SCommands, AP33772SCommands);
        break;
      case FUNC_ACTIVE:
        result = true;
        break;
    }
  }
  return result;
}

#endif  // USE_AP33772S
#endif  // USE_I2C

