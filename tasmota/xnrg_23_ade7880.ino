/*
  xnrg_23_ade7880.ino - ADE7880 energy sensor support for Tasmota

  SPDX-FileCopyrightText: 2022 Theo Arends and AndreKR

  SPDX-License-Identifier: GPL-3.0-only
*/

#ifdef USE_I2C
#ifdef USE_ENERGY_SENSOR
#ifdef USE_ADE7880
/*********************************************************************************************\
 * ADE7880 - Energy used in Shelly 3EM
 *
 * {"NAME":"Shelly 3EM","GPIO":[1,1,544,1,32,8065,0,0,640,8064,608,224,0,0],"FLAG":0,"BASE":18}
 *
 * Based on datasheet from https://www.analog.com/en/products/ade7880.html
 *
 * I2C Address: 0x38
\*********************************************************************************************/

#define XNRG_23                 23
#define XI2C_65                 65  // See I2CDEVICES.md

#define ADE7880_ADDR            0x38

enum Ade7880DspRegisters {
  ADE7880_AIGAIN = 0x4380,         // 0x4380  R/W  24  32 ZPSE  S   0x000000    Phase A current gain adjust.
  ADE7880_AVGAIN,                  // 0x4381  R/W  24  32 ZPSE  S   0x000000    Phase A voltage gain adjust.
  ADE7880_BIGAIN,                  // 0x4382  R/W  24  32 ZPSE  S   0x000000    Phase B current gain adjust.
  ADE7880_BVGAIN,                  // 0x4383  R/W  24  32 ZPSE  S   0x000000    Phase B voltage gain adjust.
  ADE7880_CIGAIN,                  // 0x4384  R/W  24  32 ZPSE  S   0x000000    Phase C current gain adjust.
  ADE7880_CVGAIN,                  // 0x4385  R/W  24  32 ZPSE  S   0x000000    Phase C voltage gain adjust.
  ADE7880_NIGAIN,                  // 0x4386  R/W  24  32 ZPSE  S   0x000000    Neutral current gain adjust.
  ADE7880_DICOEFF = 0x4388,        // 0x4388  R/W  24  32 ZPSE  S   0x0000000   Register used in the digital integrator algorithm. If the integrator is turned on, it must be set at 0xFF8000. In practice, it is transmitted as 0xFFF8000.
  ADE7880_APGAIN,                  // 0x4389  R/W  24  32 ZPSE  S   0x000000    Phase A power gain adjust.
  ADE7880_AWATTOS,                 // 0x438A  R/W  24  32 ZPSE  S   0x000000    Phase A total active power offset adjust.
  ADE7880_BPGAIN,                  // 0x438B  R/W  24  32 ZPSE  S   0x000000    Phase B power gain adjust.
  ADE7880_BWATTOS,                 // 0x438C  R/W  24  32 ZPSE  S   0x000000    Phase B total active power offset adjust.
  ADE7880_CPGAIN,                  // 0x438D  R/W  24  32 ZPSE  S   0x000000    Phase C power gain adjust.
  ADE7880_CWATTOS,                 // 0x438E  R/W  24  32 ZPSE  S   0x000000    Phase C total active power offset adjust.
  ADE7880_AIRMSOS,                 // 0x438F  R/W  24  32 ZPSE  S   0x000000    Phase A current rms offset.
  ADE7880_AVRMSOS,                 // 0x4390  R/W  24  32 ZPSE  S   0x000000    Phase A voltage rms offset.
  ADE7880_BIRMSOS,                 // 0x4391  R/W  24  32 ZPSE  S   0x000000    Phase B current rms offset.
  ADE7880_BVRMSOS,                 // 0x4392  R/W  24  32 ZPSE  S   0x000000    Phase B voltage rms offset.
  ADE7880_CIRMSOS,                 // 0x4393  R/W  24  32 ZPSE  S   0x000000    Phase C current rms offset.
  ADE7880_CVRMSOS,                 // 0x4394  R/W  24  32 ZPSE  S   0x000000    Phase C voltage rms offset.
  ADE7880_NIRMSOS                  // 0x4395  R/W  24  32 ZPSE  S   0x000000    Neutral current rms offset.

};

enum Ade7880PowerQualityRegisters {
  ADE7880_IPEAK = 0xE500,          // 0xE500  R    32  32       U   N/A         Current peak register. See Figure 60 and Table 34 for details about its composition.
  ADE7880_VPEAK,                   // 0xE501  R    32  32       U   N/A         Voltage peak register. See Figure 60 and Table 35 for details about its composition.
  ADE7880_STATUS0,                 // 0xE502  R/W  32  32       U   N/A         Interrupt Status Register 0. See Table 36.
  ADE7880_STATUS1,                 // 0xE503  R/W  32  32       U   N/A         Interrupt Status Register 1. See Table 37.
  ADE7880_AIMAV,                   // 0xE504  R    20  32 ZP    U   N/A         Phase A current mean absolute value computed during PSM0 and PSM1 modes.
  ADE7880_BIMAV,                   // 0xE505  R    20  32 ZP    U   N/A         Phase B current mean absolute value computed during PSM0 and PSM1 modes.
  ADE7880_CIMAV,                   // 0xE506  R    20  32 ZP    U   N/A         Phase C current mean absolute value computed during PSM0 and PSM1 modes.
  ADE7880_OILVL,                   // 0xE507  R/W  24  32 ZP    U   0xFFFFFF    Overcurrent threshold.
  ADE7880_OVLVL,                   // 0xE508  R/W  24  32 ZP    U   0xFFFFFF    Overvoltage threshold.
  ADE7880_SAGLVL,                  // 0xE509  R/W  24  32 ZP    U   0x000000    Voltage SAG level threshold.
  ADE7880_MASK0,                   // 0xE50A  R/W  32  32       U   0x00000000  Interrupt Enable Register 0. See Table 38.
  ADE7880_MASK1,                   // 0xE50B  R/W  32  32       U   0x00000000  Interrupt Enable Register 1. See Table 39.
  ADE7880_IAWV,                    // 0xE50C  R    24  32 SE    S   N/A         Instantaneous value of Phase A current.
  ADE7880_IBWV,                    // 0xE50D  R    24  32 SE    S   N/A         Instantaneous value of Phase B current.
  ADE7880_ICWV,                    // 0xE50E  R    24  32 SE    S   N/A         Instantaneous value of Phase C current.
  ADE7880_INWV,                    // 0xE50F  R    24  32 SE    S   N/A         Instantaneous value of neutral current.
  ADE7880_VAWV,                    // 0xE510  R    24  32 SE    S   N/A         Instantaneous value of Phase A voltage.
  ADE7880_VBWV,                    // 0xE511  R    24  32 SE    S   N/A         Instantaneous value of Phase B voltage.
  ADE7880_VCWV,                    // 0xE512  R    24  32 SE    S   N/A         Instantaneous value of Phase C voltage.
  ADE7880_AWATT,                   // 0xE513  R    24  32 SE    S   N/A         Instantaneous value of Phase A total active power.
  ADE7880_BWATT,                   // 0xE514  R    24  32 SE    S   N/A         Instantaneous value of Phase B total active power.
  ADE7880_CWATT,                   // 0xE515  R    24  32 SE    S   N/A         Instantaneous value of Phase C total active power.
  ADE7880_AVA = 0xE519,            // 0xE519  R    24  32 SE    S   N/A         Instantaneous value of Phase A apparent power.
  ADE7880_BVA,                     // 0xE51A  R    24  32 SE    S   N/A         Instantaneous value of Phase B apparent power.
  ADE7880_CVA,                     // 0xE51B  R    24  32 SE    S   N/A         Instantaneous value of Phase C apparent power.
  ADE7880_CHECKSUM = 0xE51F,       // 0xE51F  R    32  32       U   0xAFFA63B9  Checksum verification. See the Checksum Register section for details.
  ADE7880_VNOM,                    // 0xE520  R/W  24  32 ZP    S   0x000000    Nominal phase voltage rms used in the alternative computation of the apparent power. When the VNOMxEN bit is set, the
                                   //                                           applied voltage input in the corresponding phase is ignored and all corresponding rms voltage instances are replaced by the value in the VNOM register.
  ADE7880_LAST_RWDATA32 = 0xE5FF,  // 0xE5FF  R    32  32       U   N/A         Contains the data from the last successful 32-bit register communication.
  ADE7880_PHSTATUS,                // 0xE600  R    16  16       U   N/A         Phase peak register. See Table 40.
  ADE7880_ANGLE0,                  // 0xE601  R    16  16       U   N/A         Time Delay 0. See the Time Interval Between Phases section for details.
  ADE7880_ANGLE1,                  // 0xE602  R    16  16       U   N/A         Time Delay 1. See the Time Interval Between Phases section for details.
  ADE7880_ANGLE2,                  // 0xE603  R    16  16       U   N/A         Time Delay 2. See the Time Interval Between Phases section for details.
  ADE7880_PHNOLOAD = 0xE608,       // 0xE608  R    16  16       U   N/A         Phase no load register. See Table 41.
  ADE7880_LINECYC = 0xE60C,        // 0xE60C  R/W  16  16       U   0xFFFF      Line cycle accumulation mode count.
  ADE7880_ZXTOUT,                  // 0xE60D  R/W  16  16       U   0xFFFF      Zero-crossing timeout count.
  ADE7880_COMPMODE,                // 0xE60E  R/W  16  16       U   0x01FF      Computation-mode register. See Table 42.
  ADE7880_Gain,                    // 0xE60F  R/W  16  16       U   0x0000      PGA gains at ADC inputs. See Table 43.
  ADE7880_CFMODE,                  // 0xE610  R/W  16  16       U   0x0EA0      CFx configuration register. See Table 44.
  ADE7880_CF1DEN,                  // 0xE611  R/W  16  16       U   0x0000      CF1 denominator.
  ADE7880_CF2DEN,                  // 0xE612  R/W  16  16       U   0x0000      CF2 denominator.
  ADE7880_CF3DEN,                  // 0xE613  R/W  16  16       U   0x0000      CF3 denominator.
  ADE7880_APHCAL,                  // 0xE614  R/W  10  16 ZP    S   0x0000      Phase calibration of Phase A. See Table 45.
  ADE7880_BPHCAL,                  // 0xE615  R/W  10  16 ZP    S   0x0000      Phase calibration of Phase B. See Table 45.
  ADE7880_CPHCAL,                  // 0xE616  R/W  10  16 ZP    S   0x0000      Phase calibration Phase of C. See Table 45.
  ADE7880_PHSIGN,                  // 0xE617  R    16  16       U   N/A         Power sign register. See Table 46.
  ADE7880_CONFIG,                  // 0xE618  R/W  16  16       U   0x0002      ADE7880 configuration register. See Table 47.
  ADE7880_MMODE = 0xE700,          // 0xE700  R/W  8   8        U   0x1C        Measurement mode register. See Table 48.
  ADE7880_ACCMODE,                 // 0xE701  R/W  8   8        U   0x80        Accumulation mode register. See Table 49.
  ADE7880_LCYCMODE,                // 0xE702  R/W  8   8        U   0x78        Line accumulation mode behavior. See Table 51.
  ADE7880_PEAKCYC,                 // 0xE703  R/W  8   8        U   0x00        Peak detection half line cycles.
  ADE7880_SAGCYC,                  // 0xE704  R/W  8   8        U   0x00        SAG detection half line cycles.
  ADE7880_CFCYC,                   // 0xE705  R/W  8   8        U   0x01        Number of CF pulses between two consecutive energy latches. See the Synchronizing Energy Registers with CFx Outputs section.
  ADE7880_HSDC_CFG,                // 0xE706  R/W  8   8        U   0x00        HSDC configuration register. See Table 52.
  ADE7880_Version,                 // 0xE707  R    8   8        U               Version of die.
  ADE7880_Reserved =  0xE7E4,      // 0xE7E4  R    8   8        U   0x08        This register must remain at this value for checksum functionality to work. If this register shows a different value while being read, reset the chip before working with the checksum feature.
  ADE7880_LAST_RWDATA8 = 0xE7FD,   // 0xE7FD  R    8   8        U   N/A         Contains the data from the last successful 8-bit register communication.
  ADE7880_FVRMS = 0xE880,          // 0xE880  R    24  32       S   N/A         The rms value of the fundamental component of the phase voltage.
  ADE7880_FIRMS,                   // 0xE881  R    24  32       S   N/A         The rms value of the fundamental component of the phase current
  ADE7880_FWATT,                   // 0xE882  R    24  32       S   N/A         The active power of the fundamental component.
  ADE7880_FVAR,                    // 0xE883  R    24  32       S   N/A         The reactive power of the fundamental component.
  ADE7880_FVA,                     // 0xE884  R    24  32       S   N/A         The apparent power of the fundamental component.
  ADE7880_FPF,                     // 0xE885  R    24  32       S   N/A         The power factor of the fundamental component.
  ADE7880_VTHD,                    // 0xE886  R    24  32       S   N/A         Total harmonic distortion of the phase voltage.
  ADE7880_ITHD,                    // 0xE887  R    24  32       S   N/A         Total harmonic distortion of the phase current.
  ADE7880_HXVRMS,                  // 0xE888  R    24  32       S   N/A         The rms value of the phase voltage harmonic X.
  ADE7880_HXIRMS,                  // 0xE889  R    24  32       S   N/A         The rms value of the phase current harmonic X.
  ADE7880_HXWATT,                  // 0xE88A  R    24  32       S   N/A         The active power of the harmonic X.
  ADE7880_HXVAR,                   // 0xE88B  R    24  32       S   N/A         The reactive power of the harmonic X.
  ADE7880_HXVA,                    // 0xE88C  R    24  32       S   N/A         The apparent power of the harmonic X.
  ADE7880_HXPF,                    // 0xE88D  R    24  32       S   N/A         The power factor of the harmonic X.
  ADE7880_HXVHD,                   // 0xE88E  R    24  32       S   N/A         Harmonic distortion of the phase voltage harmonic X relative to the fundamental.
  ADE7880_HXIHD,                   // 0xE88F  R    24  32       S   N/A         Harmonic distortion of the phase current harmonic X relative to the fundamental.
  ADE7880_HYVRMS,                  // 0xE890  R    24  32       S   N/A         The rms value of the phase voltage harmonic Y.
  ADE7880_HYIRMS,                  // 0xE891  R    24  32       S   N/A         The rms value of the phase current harmonic Y.
  ADE7880_HYWATT,                  // 0xE892  R    24  32       S   N/A         The active power of the harmonic Y.
  ADE7880_HYVAR,                   // 0xE893  R    24  32       S   N/A         The reactive power of the harmonic Y.
  ADE7880_HYVA,                    // 0xE894  R    24  32       S   N/A         The apparent power of the harmonic Y.
  ADE7880_HYPF,                    // 0xE895  R    24  32       S   N/A         The power factor of the harmonic Y.
  ADE7880_HYVHD,                   // 0xE896  R    24  32       S   N/A         Harmonic distortion of the phase voltage harmonic Y relative to the fundamental.
  ADE7880_HYIHD,                   // 0xE897  R    24  32       S   N/A         Harmonic distortion of the phase current harmonic Y relative to the fundamental.
  ADE7880_HZVRMS,                  // 0xE898  R    24  32       S   N/A         The rms value of the phase voltage harmonic Z.
  ADE7880_HZIRMS,                  // 0xE899  R    24  32       S   N/A         The rms value of the phase current harmonic Z.
  ADE7880_HZWATT,                  // 0xE89A  R    24  32       S   N/A         The active power of the harmonic Z.
  ADE7880_HZVAR,                   // 0xE89B  R    24  32       S   N/A         The reactive power of the harmonic Z.
  ADE7880_HZVA,                    // 0xE89C  R    24  32       S   N/A         The apparent power of the harmonic Z.
  ADE7880_HZPF,                    // 0xE89D  R    24  32       S   N/A         The power factor of the harmonic Z.
  ADE7880_HZVHD,                   // 0xE89E  R    24  32       S   N/A         Harmonic distortion of the phase voltage harmonic Z relative to the fundamental.
  ADE7880_HZIHD,                   // 0xE89F  R    24  32       S   N/A         Harmonic distortion of the phase current harmonic Z relative to the fundamental.
  ADE7880_HCONFIG = 0xE900,        // 0xE900  R/W  16  16       U   0x08        Harmonic Calculations Configuration register. See Table 54.
  ADE7880_APF = 0xE902,            // 0xE902  R    16  16       S   N/A         Phase A power factor.
  ADE7880_BPF,                     // 0xE903  R    16  16       S   N/A         Phase B power factor.
  ADE7880_CPF,                     // 0xE904  R    16  16       S   N/A         Phase C power factor.
  ADE7880_APERIOD,                 // 0xE905  R    16  16       U   N/A         Line period on Phase A voltage.
  ADE7880_BPERIOD,                 // 0xE906  R    16  16       U   N/A         Line period on Phase B voltage.
  ADE7880_CPERIOD,                 // 0xE907  R    16  16       U   N/A         Line period on Phase C voltage.
  ADE7880_APNOLOAD,                // 0xE908  R/W  16  16       U   0x0000      No load threshold in the total/ fundamental active power data paths. Do not write 0xFFFF to this register.
  ADE7880_VARNOLOAD,               // 0xE909  R/W  16  16       U   0x0000      No load threshold in the total/ fundamental reactive power data path. Do not write 0xFFFF to this register.
  ADE7880_VANOLOAD,                // 0xE90A  R/W  16  16       U   0x0000      No load threshold in the apparent power data path. Do not write 0xFFFF to this register.
  ADE7880_LAST_ADD = 0xE9FE,       // 0xE9FE  R    16  16       U   N/A         The address of the register successfully accessed during the last read/write operation.
  ADE7880_LAST_RWDATA16,           // 0xE9FF  R    16  16       U   N/A         Contains the data from the last successful 16-bit register communication.
  ADE7880_CONFIG3,                 // 0xEA00  R/W  8   8        U   0x01        Configuration register. See Table 53.
  ADE7880_LAST_OP,                 // 0xEA01  R    8   8        U   N/A         Indicates the type, read or write, of the last successful read/write operation.
  ADE7880_WTHR,                    // 0xEA02  R/W  8   8        U   0x03        Threshold used in phase total/fundamental active power data path.
  ADE7880_VARTHR,                  // 0xEA03  R/W  8   8        U   0x03        Threshold used in phase total/fundamental reactive power data path.
  ADE7880_VATHR,                   // 0xEA04  R/W  8   8        U   0x03        Threshold used in phase apparent power data path.
  ADE7880_HX = 0xEA08,             // 0xEA08  R/W  8   8        U   3           Selects an index of the harmonic monitored by the harmonic computations.
  ADE7880_HY,                      // 0xEA09  R/W  8   8        U   5           Selects an index of the harmonic monitored by the harmonic computations.
  ADE7880_HZ,                      // 0xEA0A  R/W  8   8        U   7           Selects an index of the harmonic monitored by the harmonic computations.
  ADE7880_LPOILVL = 0xEC00,        // 0xEC00  R/W  8   8        U   0x07        Overcurrent threshold used during PSM2 mode. See Table 55 in which the register is detailed.
  ADE7880_CONFIG2                  // 0xEC01  R/W  8   8        U   0x00        Configuration register used during PSM1 mode. See Table 56.
};

struct Ade7880 {
  uint32_t current_rms[4] = { 0, 0, 0, 0 };


} Ade7880;

int Ade7880RegSize(uint16_t reg) {
  int size = 0;
  switch ((reg >> 8) & 0x0F) {
    case 0x03:  // 32-bit
    case 0x04:
    case 0x05:
    case 0x08:
      size++;
      size++;
    case 0x01:  // 16-bit
    case 0x02:
    case 0x06:
    case 0x09:
      size++;
    case 0x00:  // 8-bit
    case 0x07:
    case 0x0A:
    case 0x0B:
    case 0x0C:
      size++;
  }
  return size;
}

void Ade7880Write(uint16_t reg, uint32_t val) {
  int size = Ade7880RegSize(reg);
  if (size) {
    Wire.beginTransmission(ADE7880_ADDR);
    Wire.write((reg >> 8) & 0xFF);
    Wire.write(reg & 0xFF);
    while (size--) {
      Wire.write((val >> (8 * size)) & 0xFF);  // Write data, MSB first
    }
    Wire.endTransmission();
    delayMicroseconds(5);    // Bus-free time minimum 4.7us
  }
}

int32_t Ade7880Read(uint16_t reg) {
	uint32_t response = 0;

  int size = Ade7880RegSize(reg);
  if (size) {
    Wire.beginTransmission(ADE7880_ADDR);
    Wire.write((reg >> 8) & 0xFF);
    Wire.write(reg & 0xFF);
    Wire.endTransmission(0);
    Wire.requestFrom(ADE7880_ADDR, size);
    if (size <= Wire.available()) {
      for (uint32_t i = 0; i < size; i++) {
        response = response << 8 | Wire.read();   // receive DATA (MSB first)
      }
    }
  }
	return response;
}

void Ade7880Reset(void) {
  pinMode(16, OUTPUT);                          // Reset pin ADE7880
  digitalWrite(16, 0);
  delay(1);
  digitalWrite(16, 1);
  pinMode(16, INPUT);
}

void Ade7880Isr0(void) {
  // Init sequence about 100mS after reset
  uint32_t status1 = Ade7880Read(ADE7880_STATUS1);        // 0x01A08000

  uint32_t last_op = Ade7880Read(ADE7880_LAST_OP);        // 0x35 - Read
  uint32_t last_address = Ade7880Read(ADE7880_LAST_ADD);  // ADE7880_STATUS1

  Ade7880Write(ADE7880_CONFIG2, 0x02);                    // ADE7880_I2C_LOCK

//  Ade7953Write(0x102, 0x0004);    // Locking the communication interface (Clear bit COMM_LOCK), Enable HPF
//  Ade7953Write(0x0FE, 0x00AD);    // Unlock register 0x120
//  Ade7953Write(0x120, 0x0030);    // Configure optimum setting

}

void Ade7880Isr1(void) {
  // Poll sequence


}

void Ade7880EnergyEverySecond(void) {

}

void Ade7880DrvInit(void) {
  if (PinUsed(GPIO_ADE7880_IRQ) && PinUsed(GPIO_ADE7880_IRQ, 1)) {
    pinMode(Pin(GPIO_ADE7880_IRQ), INPUT);
    attachInterrupt(Pin(GPIO_ADE7880_IRQ), Ade7880Isr0, FALLING);

    pinMode(Pin(GPIO_ADE7880_IRQ, 1), INPUT);
    attachInterrupt(Pin(GPIO_ADE7880_IRQ, 1), Ade7880Isr1, FALLING);

    Ade7880Reset();

    delay(200);                                   // Need 200mS to init ADE7880
    if (I2cSetDevice(ADE7880_ADDR)) {
      I2cSetActiveFound(ADE7880_ADDR, "ADE7880");
      Energy.phase_count = 3;                     // Three phases
//      Energy.use_overtemp = true;                 // Use global temperature for overtemp detection

      TasmotaGlobal.energy_driver = XNRG_23;
    }
  }
}

bool Ade7880Command(void) {
  // Will need calibration for all three phases
  bool serviced = true;


  return serviced;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xnrg23(uint8_t function) {
  if (!I2cEnabled(XI2C_07)) { return false; }

  bool result = false;

  switch (function) {
    case FUNC_ENERGY_EVERY_SECOND:
      Ade7880EnergyEverySecond();
      break;
    case FUNC_COMMAND:
      result = Ade7880Command();
      break;
    case FUNC_PRE_INIT:
      Ade7880DrvInit();
      break;
  }
  return result;
}

#endif  // USE_ADE7880
#endif  // USE_ENERGY_SENSOR
#endif  // USE_I2C
