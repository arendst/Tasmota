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
 * {"NAME":"Shelly 3EM","GPIO":[1,1,288,1,32,8065,0,0,640,8064,608,224,8096,0],"FLAG":0,"BASE":18}
 *
 * Based on datasheet from https://www.analog.com/en/products/ade7880.html Rev.C
 *
 * I2C Address: 0x38
 *********************************************************************************************
 * - ATTENTION Before installing Tasmota retrieve the calibration data from the Shelly 3EM:
 *   - Download file calib.dat from your configured shelly 3EM http://<shelly3em_ip_address>/calib.dat
 *   - Search your Shelly 3EM firmware dump for calib.dat
 * - Edit the file to become a single line. Notice the removal of unneeded data to make the string as short as possible:
 *   {"rms":{"current_a":3166385,"current_b":3125691,"current_c":3131983,"current_s":1756557,"voltage_a":-767262,"voltage_b":-763439,"voltage_c":-749854},"angles":{"angle0":180,"angle1":176,"angle2":176},"powers":{"totactive": {"a":-1345820,"b":-1347328,"c":-1351979}},"freq":0}
 * - Install Tasmota and use the above template.
 * - In addition to possible rules add a rule containing the calib.dat string from above like:
 *   rule3 on file#calib.dat do {"rms":{"current_a":3166385,"current_b":3125691,"current_c":3131983,"current_s":1756557,"voltage_a":-767262,"voltage_b":-763439,"voltage_c":-749854},"angles":{"angle0":180,"angle1":176,"angle2":176},"powers":{"totactive": {"a":-1345820,"b":-1347328,"c":-1351979}},"freq":0} endon
 * - Restart Tasmota and obeserve that the results seem calibrated as Tasmota now uses the information from calib.dat
\*********************************************************************************************/

#define XNRG_23                 23
#define XI2C_65                 65           // See I2CDEVICES.md

#define ADE7880_ADDR            0x38

/*********************************************************************************************/

#define ADE7880_MORE_REGS                    // Add Neutral Current information

//#define ADE7880_DEBUG
//#define ADE7880_PROFILING

#define ADE7880_WATCHDOG        5            // Allow x seconds of missed interrupts before reinit

// Default calibration parameters can be overridden by a rule as documented above.
#ifndef ADE7880_FREQ_INIT
#define ADE7880_FREQ_INIT       0            // Connected to networks with fundamental frequencies between 55 Hz and 66 Hz (1). Default 45 Hz to 55 Hz (0).
#endif
#ifndef ADE7880_AIGAIN_INIT
#define ADE7880_AIGAIN_INIT     3166385      // rms, current_a
#endif
#ifndef ADE7880_BIGAIN_INIT
#define ADE7880_BIGAIN_INIT     3125691      // rms, current_b
#endif
#ifndef ADE7880_CIGAIN_INIT
#define ADE7880_CIGAIN_INIT     3131983      // rms, current_c
#endif
#ifndef ADE7880_NIGAIN_INIT
#define ADE7880_NIGAIN_INIT     1756557      // rms, current_s !!
#endif
#ifndef ADE7880_AVGAIN_INIT
#define ADE7880_AVGAIN_INIT     -767262      // rms, voltage_a
#endif
#ifndef ADE7880_BVGAIN_INIT
#define ADE7880_BVGAIN_INIT     -763439      // rms, voltage_b
#endif
#ifndef ADE7880_CVGAIN_INIT
#define ADE7880_CVGAIN_INIT     -749854      // rms, voltage_c
#endif
#ifndef ADE7880_APHCAL_INIT
#define ADE7880_APHCAL_INIT     180          // angles, angle0
#endif
#ifndef ADE7880_BPHCAL_INIT
#define ADE7880_BPHCAL_INIT     176          // angles, angle1
#endif
#ifndef ADE7880_CPHCAL_INIT
#define ADE7880_CPHCAL_INIT     176          // angles, angle2
#endif
#ifndef ADE7880_APGAIN_INIT
#define ADE7880_APGAIN_INIT     -1345820     // powers, totactive, a
#endif
#ifndef ADE7880_BPGAIN_INIT
#define ADE7880_BPGAIN_INIT     -1347328     // powers, totactive, b
#endif
#ifndef ADE7880_CPGAIN_INIT
#define ADE7880_CPGAIN_INIT     -1351979     // powers, totactive, c
#endif

enum Ade7880DspRegisters {
  // Register Name                    Addres  R/W  Bt  CommBln  Ty  Default     Description
  // ----------------------------     ------  ---  --  -------  --  ----------  --------------------------------------------------------------------
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
  ADE7880_NIRMSOS,                 // 0x4395  R/W  24  32 ZPSE  S   0x000000    Neutral current rms offset.
  ADE7880_HPGAIN = 0x4398,         // 0x4398  R/W  24  32 ZPSE  S   0x000000    Harmonic powers gain adjust.
  ADE7880_ISUMLVL,                 // 0x4399  R/W  24  32 ZPSE  S   0x000000    Threshold used in comparison between the sum of phase currents and the neutral current.
  ADE7880_VLEVEL = 0x439F,         // 0x439F  R/W  28  32 ZP    S   0x0000000   Register used in the algorithm that computes the fundamental active and reactive powers. Set this register according to Equation 22 for proper functioning of fundamental powers and harmonic computations.
  ADE7880_AFWATTOS = 0x43A2,       // 0x43A2  R/W  24  32 ZPSE  S   0x000000    Phase A fundamental active power offset adjust.
  ADE7880_BFWATTOS,                // 0x43A3  R/W  24  32 ZPSE  S   0x000000    Phase B fundamental active power offset adjust.
  ADE7880_CFWATTOS,                // 0x43A4  R/W  24  32 ZPSE  S   0x000000    Phase C fundamental active power offset adjust.
  ADE7880_AFVAROS,                 // 0x43A5  R/W  24  32 ZPSE  S   0x000000    Phase A fundamental reactive power offset adjust.
  ADE7880_BFVAROS,                 // 0x43A6  R/W  24  32 ZPSE  S   0x000000    Phase B fundamental reactive power offset adjust.
  ADE7880_CFVAROS,                 // 0x43A7  R/W  24  32 ZPSE  S   0x000000    Phase C fundamental reactive power offset adjust.
  ADE7880_AFIRMSOS,                // 0x43A8  R/W  24  32 ZPSE  S   0x000000    Phase A fundamental current rms offset.
  ADE7880_BFIRMSOS,                // 0x43A9  R/W  24  32 ZPSE  S   0x000000    Phase B fundamental current rms offset.
  ADE7880_CFIRMSOS,                // 0x43AA  R/W  24  32 ZPSE  S   0x000000    Phase C fundamental current rms offset.
  ADE7880_AFVRMSOS,                // 0x43AB  R/W  24  32 ZPSE  S   0x000000    Phase A fundamental voltage rms offset.
  ADE7880_BFVRMSOS,                // 0x43AC  R/W  24  32 ZPSE  S   0x000000    Phase B fundamental voltage rms offset.
  ADE7880_CFVRMSOS,                // 0x43AD  R/W  24  32 ZPSE  S   0x000000    Phase C fundamental voltage rms offset.
  ADE7880_HXWATTOS,                // 0x43AE  R/W  24  32 ZPSE  S   0x000000    Active power offset adjust on harmonic X (see Harmonics Calculations section for details).
  ADE7880_HYWATTOS,                // 0x43AF  R/W  24  32 ZPSE  S   0x000000    Active power offset adjust on harmonic Y (see Harmonics Calculations section for details).
  ADE7880_HZWATTOS,                // 0x43B0  R/W  24  32 ZPSE  S   0x000000    Active power offset adjust on harmonic Z (see Harmonics Calculations section for details).
  ADE7880_HXVAROS,                 // 0x43B1  R/W  24  32 ZPSE  S   0x000000    Active power offset adjust on harmonic X (see Harmonics Calculations section for details).
  ADE7880_HYVAROS,                 // 0x43B2  R/W  24  32 ZPSE  S   0x000000    Active power offset adjust on harmonic Y (see Harmonics Calculations section for details).
  ADE7880_HZVAROS,                 // 0x43B3  R/W  24  32 ZPSE  S   0x000000    Active power offset adjust on harmonic Z (see Harmonics Calculations section for details).
  ADE7880_HXIRMSOS,                // 0x43B4  R/W  24  32 ZPSE  S   0x000000    Current rms offset on harmonic X (see Harmonics Calculations section for details).
  ADE7880_HYIRMSOS,                // 0x43B5  R/W  24  32 ZPSE  S   0x000000    Current rms offset on harmonic Y (see Harmonics Calculations section for details).
  ADE7880_HZIRMSOS,                // 0x43B6  R/W  24  32 ZPSE  S   0x000000    Current rms offset on harmonic Z (see Harmonics Calculations section for details).
  ADE7880_HXVRMSOS,                // 0x43B7  R/W  24  32 ZPSE  S   0x000000    Voltage rms offset on harmonic X (see Harmonics Calculations section for details).
  ADE7880_HYVRMSOS,                // 0x43B8  R/W  24  32 ZPSE  S   0x000000    Voltage rms offset on harmonic Y (see Harmonics Calculations section for details).
  ADE7880_HZVRMSOS,                // 0x43B9  R/W  24  32 ZPSE  S   0x000000    Voltage rms offset on harmonic Z (see Harmonics Calculations section for details).
  ADE7880_AIRMS = 0x43C0,          // 0x43C0  R    24  32 ZP    S   N/A         Phase A current rms value.
  ADE7880_AVRMS,                   // 0x43C1  R    24  32 ZP    S   N/A         Phase A voltage rms value.
  ADE7880_BIRMS,                   // 0x43C2  R    24  32 ZP    S   N/A         Phase B current rms value.
  ADE7880_BVRMS,                   // 0x43C3  R    24  32 ZP    S   N/A         Phase B voltage rms value.
  ADE7880_CIRMS,                   // 0x43C4  R    24  32 ZP    S   N/A         Phase C current rms value.
  ADE7880_CVRMS,                   // 0x43C5  R    24  32 ZP    S   N/A         Phase C voltage rms value.
  ADE7880_NIRMS,                   // 0x43C6  R    24  32 ZP    S   N/A         Neutral current rms value.
  ADE7880_ISUM                     // 0x43C7  R    28  32 ZP    S   N/A         Sum of IAWV, IBWV and ICWV registers.
};

enum Ade7880InternalDspRegisters {
  ADE7880_Run = 0xE228             // 0xE228  R/W  16  16       U   0x0000      Run register starts and stops the DSP. See the Digital Signal Processor section for more details.
};

enum Ade7880BillableRegisters {
  ADE7880_AWATTHR = 0xE400,        // 0xE400  R    32  32       S   0x00000000  Phase A total active energy accumulation.
  ADE7880_BWATTHR,                 // 0xE401  R    32  32       S   0x00000000  Phase B total active energy accumulation.
  ADE7880_CWATTHR,                 // 0xE402  R    32  32       S   0x00000000  Phase C total active energy accumulation.
  ADE7880_AFWATTHR,                // 0xE403  R    32  32       S   0x00000000  Phase A fundamental active energy accumulation.
  ADE7880_BFWATTHR,                // 0xE404  R    32  32       S   0x00000000  Phase B fundamental active energy accumulation.
  ADE7880_CFWATTHR,                // 0xE405  R    32  32       S   0x00000000  Phase C fundamental active energy accumulation.
  ADE7880_AFVARHR = 0xE409,        // 0xE409  R    32  32       S   0x00000000  Phase A fundamental reactive energy accumulation.
  ADE7880_BFVARHR,                 // 0xE40A  R    32  32       S   0x00000000  Phase B fundamental reactive energy accumulation.
  ADE7880_CFVARHR,                 // 0xE40B  R    32  32       S   0x00000000  Phase C fundamental reactive energy accumulation.
  ADE7880_AVAHR,                   // 0xE40C  R    32  32       S   0x00000000  Phase A apparent energy accumulation.
  ADE7880_BVAHR,                   // 0xE40D  R    32  32       S   0x00000000  Phase B apparent energy accumulation.
  ADE7880_CVAHR                    // 0xE40E  R    32  32       S   0x00000000  Phase C apparent energy accumulation.
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
  ADE7880_DSPWP_SET = 0xE7E3,      // 0xE7E3  W    8   8        U   0x00        Write protect DSP (0x80) or enable write (0x00). See page 40.
  ADE7880_Reserved,                // 0xE7E4  R    8   8        U   0x08        This register must remain at this value for checksum functionality to work. If this register shows a different value while being read, reset the chip before working with the checksum feature.
  ADE7880_LAST_RWDATA8 = 0xE7FD,   // 0xE7FD  R    8   8        U   N/A         Contains the data from the last successful 8-bit register communication.
  ADE7880_DSPWP_SEL,               // 0xE7FE  W    8   8        U   0xAD        Select DSP writeprotect. See page 40.
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
  float neutral_current;
  int32_t calib_current[4];
  int32_t calib_voltage[3];
  int32_t calib_acpower[3];
  uint16_t calib_angle[3];
  bool calib_frequency;
  bool irq0_state;
  uint8_t cycle_count;
  uint8_t watchdog;
} Ade7880;

/*********************************************************************************************/

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
#ifdef ADE7880_DEBUG
    char log_format[100];
    snprintf_P(log_format, sizeof(log_format), PSTR("A78: Wr 0x%%04X 0x%%0%dX (%%d)"), size << 1);
    AddLog(LOG_LEVEL_DEBUG_MORE, log_format, reg, val, val);
#endif  // ADE7880_DEBUG
    Wire.beginTransmission(ADE7880_ADDR);
    Wire.write((reg >> 8) & 0xFF);
    Wire.write(reg & 0xFF);
    while (size--) {
      Wire.write((val >> (8 * size)) & 0xFF);                              // Write data, MSB first
    }
    Wire.endTransmission();
    delayMicroseconds(5);                                                  // Bus-free time minimum 4.7us
  }
}

bool Ade7880VerifyWrite(uint16_t reg) {
  uint32_t error = 0;
  error += (0xCA != Ade7880Read(ADE7880_LAST_OP));                         // Indicates the type, read (0x35) or write (0xCA), of the last successful read/write operation.
  error += (reg != Ade7880Read(ADE7880_LAST_ADD));                         // The address of the register successfully accessed during the last read/write operation.
  if (error) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("A78: Write verify error"));
    return false;
  }
  return true;
}

bool Ade7880WriteVerify(uint16_t reg, uint32_t val) {
  Ade7880Write(reg, val);
  return Ade7880VerifyWrite(reg);
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
        response = response << 8 | Wire.read();                            // receive DATA (MSB first)
      }
    }
#ifdef ADE7880_DEBUG
    if ((reg != ADE7880_LAST_OP) && (reg != ADE7880_LAST_ADD)) {
      char log_format[100];
      snprintf_P(log_format, sizeof(log_format), PSTR("A78: Rd 0x%%04X 0x%%0%dX (%%d)"), size << 1);
      AddLog(LOG_LEVEL_DEBUG_MORE, log_format, reg, response, response);
    }
#endif  // ADE7880_DEBUG
  }
	return response;
}

int32_t Ade7880ReadVerify(uint16_t reg) {
  int32_t result = Ade7880Read(reg);
  uint32_t error = 0;
  error += (0x35 != Ade7880Read(ADE7880_LAST_OP));                         // Indicates the type, read (0x35) or write (0xCA), of the last successful read/write operation.
  error += (reg != Ade7880Read(ADE7880_LAST_ADD));                         // The address of the register successfully accessed during the last read/write operation.
  if (error) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("A78: Read verify error"));
  }
  return result;
}

/*********************************************************************************************/

bool Ade7880Init(void) {
  // Init sequence about 6mS after reset - See page 40 (takes 68ms)
#ifdef ADE7880_PROFILING
  uint32_t start = millis();
#endif  // ADE7880_PROFILING

  uint32_t status1 = Ade7880ReadVerify(ADE7880_STATUS1);                   // 0xE503 - 0x01A08000
  if (bitRead(status1, 15)) {                                              // RSTDONE
    // Power on or Reset
    Ade7880WriteVerify(ADE7880_CONFIG2, 0x02);                             // 0xEC01 - ADE7880_I2C_LOCK
    Ade7880WriteVerify(ADE7880_STATUS1, 0x3FFE8930);                       // 0xE503 - Acknowledge RSTDONE - Reset IRQ1 line
    status1 = Ade7880ReadVerify(ADE7880_STATUS1);                          // 0xE503 - 0x01A00007
  } else {
    return false;
  }
  uint8_t version = Ade7880ReadVerify(ADE7880_Version);                    // 0xE707 - 0x01
  Ade7880WriteVerify(ADE7880_Gain, 0x0000);                                // 0xE60F - Gain register set to 1 for current, and voltage
  if (Ade7880.calib_frequency) {
    Ade7880WriteVerify(ADE7880_COMPMODE, 0x41FF);                          // 0xE60E - Connected to networks with fundamental frequencies between 55 Hz and 66 Hz. Default is 45 Hz and 55 Hz.
  }

  for (uint32_t phase = 0; phase < 3; phase++) {
    Ade7880WriteVerify(ADE7880_AVGAIN + (phase * 2), Ade7880.calib_voltage[phase]);  // 0x4381
    Ade7880WriteVerify(ADE7880_AIGAIN + (phase * 2), Ade7880.calib_current[phase]);  // 0x4380
    Ade7880WriteVerify(ADE7880_APGAIN + (phase * 2), Ade7880.calib_acpower[phase]);  // 0x4389
    Ade7880WriteVerify(ADE7880_APHCAL + phase, Ade7880.calib_angle[phase]);          // 0xE614
  }
  Ade7880WriteVerify(ADE7880_NIGAIN, Ade7880.calib_current[3]);            // 0x4386
  Ade7880WriteVerify(ADE7880_NIGAIN, Ade7880.calib_current[3]);            // 0x4386 - Write last data memory RAM three times (page 40)
  Ade7880WriteVerify(ADE7880_NIGAIN, Ade7880.calib_current[3]);            // 0x4386

  bool error = false;
  for (uint32_t phase = 0; phase < 3; phase++) {
    if (Ade7880ReadVerify(ADE7880_AVGAIN + (phase * 2)) != (Ade7880.calib_voltage[phase] & 0x0FFFFFFF)) { error = true; }
    else if (Ade7880ReadVerify(ADE7880_AIGAIN + (phase * 2)) != (Ade7880.calib_current[phase] & 0x0FFFFFFF)) { error = true; }
    else if (Ade7880ReadVerify(ADE7880_APGAIN + (phase * 2)) != (Ade7880.calib_acpower[phase] & 0x0FFFFFFF)) { error = true; }
    else if (Ade7880ReadVerify(ADE7880_APHCAL + phase) != (Ade7880.calib_angle[phase] & 0x00FF)) { error = true; }
  }
  if (Ade7880ReadVerify(ADE7880_NIGAIN) != (Ade7880.calib_current[3] & 0x0FFFFFFF)) { error = true; }
  if (error) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("A78: Error initializing parameters"));
    return false;
  }

  if (!Ade7880WriteVerify(ADE7880_LCYCMODE, 0x09)) {                       // 0xE702 - Line cycle accumulation mode
                                                                           // - Watt-hour accumulation registers (AWATTHR, BWATTHR, CWATTHR, AFWATTHR, BFWATTHR, and CFWATTHR) are placed into line cycle accumulation mode.
                                                                           // - Phase A is selected for zero-crossings counts in the line cycle accumulation mode.
    AddLog(LOG_LEVEL_DEBUG, PSTR("A78: Error setting LCYCMODE register"));
    return false;
  }
  uint32_t line_cycle = (Ade7880.calib_frequency) ? 120 : 100;             // Either 60Hz or 50Hz to have 1 second interrupts
  if (!Ade7880WriteVerify(ADE7880_LINECYC, line_cycle)) {                  // 0xE60C - = 100
    AddLog(LOG_LEVEL_DEBUG, PSTR("A78: Error setting LINECYC register"));
    return false;
  }
  Ade7880WriteVerify(ADE7880_MASK0, 0x00000020);                           // 0xE50A - IRQ0 at end of an integration over an integer number of half line cycles set in the LINECYC register.
  if (!Ade7880VerifyWrite(ADE7880_MASK0)) {                                // 0xE50A
    AddLog(LOG_LEVEL_DEBUG, PSTR("A78: Error setting MASK0 register"));
    return false;
  }
  Ade7880Write(ADE7880_MASK0, 0x00000020);                                 // 0xE50A
  Ade7880Write(ADE7880_DSPWP_SEL, 0xAD);                                   // 0xE7FE - Select DSP write protection
  Ade7880Write(ADE7880_DSPWP_SET, 0x80);                                   // 0xE7E3 - Write protect DSP area
  Ade7880WriteVerify(ADE7880_Run, 0x0201);                                 // 0xE228 - Start DSP

#ifdef ADE7880_PROFILING
  AddLog(LOG_LEVEL_DEBUG, PSTR("A78: Init done in %d ms"), millis() - start);
#endif  // ADE7880_PROFILING

  return true;
}

bool Ade7880SetCalibrate(void) {

#ifdef ADE7880_PROFILING
    uint32_t start = millis();
#endif  // ADE7880_PROFILING

  int pin_reset = Pin(GPIO_RESET);
#ifdef ESP8266
  if (-1 == pin_reset) { pin_reset = 16; }                                 // Reset pin ADE7880 in Shelly 3EM
#endif
  if (pin_reset >= 0) {
    pinMode(pin_reset, OUTPUT);
    digitalWrite(pin_reset, 0);
    delay(1);
    digitalWrite(pin_reset, 1);
    pinMode(pin_reset, INPUT);
  }

  Ade7880.cycle_count = 2;                                                 // Skip first two cycles

  uint32_t timeout = millis() + 100;                                       // Should be reset within 10 ms
  while (!TimeReached(timeout)) {                                          // Wait up to 100 ms
    if (!digitalRead(Pin(GPIO_ADE7880_IRQ, 1))) {

#ifdef ADE7880_PROFILING
      AddLog(LOG_LEVEL_DEBUG, PSTR("A78: Reset in %d ms"), millis() - start);
#endif  // ADE7880_PROFILING

      if (Ade7880Init()) {
        Ade7880.watchdog = 0;
        return true;
      }
    }
  }
  return false;
}

/*********************************************************************************************/

void Ade7880Cycle(void) {
  // Cycle sequence (takes 55ms)
#ifdef ADE7880_PROFILING
  uint32_t start = millis();
#endif  // ADE7880_PROFILING

  uint32_t status0 = Ade7880ReadVerify(ADE7880_STATUS0);                   // 0xE502 - 0x000FEFE0
/*
  if (!bitRead(status0, 5)) {                                              // LENERGY
    AddLog(LOG_LEVEL_DEBUG, PSTR("A78: Unexpected ISR0 0x%08X"), status0);
    return;
  } else {
    Ade7880WriteVerify(ADE7880_STATUS0, 0x00000020);                       // 0xE502 - Acknowledge LENERGY - Reset IRQ0 line
    status0 = Ade7880ReadVerify(ADE7880_STATUS0);                          // 0xE502 - 0x000FEFC0
  }
*/
  if (!bitRead(status0, 5)) {                                              // LENERGY
    AddLog(LOG_LEVEL_DEBUG, PSTR("A78: Unexpected ISR0 0x%08X"), status0);
  }
  Ade7880WriteVerify(ADE7880_STATUS0, 0x00000020);                         // 0xE502 - Acknowledge LENERGY - Reset IRQ0 line
  status0 = Ade7880ReadVerify(ADE7880_STATUS0);                            // 0xE502 - 0x000FEFC0

  if (Ade7880.cycle_count) {                                               // Allow calibration stabilization
    Ade7880.cycle_count--;
    return;                                                                // Skip first cycles
  }
  Ade7880.neutral_current = (float)Ade7880ReadVerify(ADE7880_NIRMS) / 100000;                // 0x43C6
  for (uint32_t phase = 0; phase < 3; phase++) {
    Energy->data_valid[phase] = 0;
    Energy->voltage[phase] = (float)Ade7880ReadVerify(ADE7880_AVRMS + (phase * 2)) / 10000;   // 0x43C1 - 0x0024CC94 = 241.1668 V
    Energy->current[phase] = (float)Ade7880ReadVerify(ADE7880_AIRMS + (phase * 2)) / 100000;  // 0x43C0 - 0x00002D6D = 0.11629 A
    Energy->active_power[phase] = (float)Ade7880ReadVerify(ADE7880_AWATT + phase) / 100;      // 0xE513 - 0xFFFFF524 = -27.79 W
    Energy->apparent_power[phase] = (float)Ade7880ReadVerify(ADE7880_AVA + phase) / 100;      // 0xE519 - 0xFFFFF50D
    Energy->frequency[phase] = 256000.0f / Ade7880ReadVerify(ADE7880_APERIOD + phase);        // 0xE905 - Page 34 and based on ADE7880_FREQ_INIT
    // Suppose constant load during period of 100/120 periods as set by ADE7880_LINECYC disregards load change inbetween.
    // ADE7880_AWATT = 6713 = 67,13 W
    // 67,13 * 1000 / 36 = 1864 deca micro Wh (0.01864Wh)
//    Energy->kWhtoday_delta[phase] += Energy->active_power[phase] * 1000 / 36;

    // By measuring load 1024000 times/second load change in 100/120 periods can be accounted for.
    // ADE7880_AWATT = 6713 = 67,13 W
    // ADE7880_AWATTHR = 273
    // AWATT multiplier is 16 (Figure 77)
    // ADE7880_WTHR = 3 (default)
    // Active power accumulation rate is 1.024MHz (Page 49)
    // 1024000 * 16 * ADE7880_AWATT / ADE7880_WTHR * 0x8000000 = ADE7880_AWATTHR
    // 1024000 * 16 * 6713          / 3            * 134217728 = 273
    // 16384000     * 6713          / 402653184                = 273
    // 273 * 402653184 / 16384000 = 6709 = 67,09W * 1000 / 36 = 1863 deca micro Wh (Tasmota needs deca micro Wh)
    // 273 * 402653184 / 16384 = 6709248 = 67092,48W / 3600 = 1863 deca micro Wh
    // 273 * 24576 = 6709248 / 3600 = 1863 deca micro Wh
    int32_t active_energy = Ade7880ReadVerify(ADE7880_AWATTHR + phase);    // 0xE400 - 0xFFFFFF8F = -0.112
    Energy->kWhtoday_delta[phase] += active_energy * 24576 / 3600;          // Using int32_t allows loads up to 87kW (0x7FFFFFFF / 24576)
  }
  EnergyUpdateToday();

#ifdef ADE7880_PROFILING
  AddLog(LOG_LEVEL_DEBUG, PSTR("A78: Cycle in %d ms"), millis() - start);
#endif  // ADE7880_PROFILING
}

void Ade7880Service0(void) {
  // Poll sequence
  Ade7880Cycle();
  Ade7880.watchdog = 0;
  Ade7880.irq0_state = 0;
}

static void IRAM_ATTR Ade7880Isr0(void) {
  // Poll sequence
  if (!Ade7880.irq0_state) {
    Ade7880.irq0_state = 1;
  }
}

void Ade7880Watchdog(void) {
  // Reinit if interrupt stopped
  Ade7880.watchdog++;
  if (Ade7880.watchdog > ADE7880_WATCHDOG) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("A78: Reinit"));
    Ade7880SetCalibrate();
  }
}

/*********************************************************************************************/

bool Ade7880SetDefaults(const char* json) {
  // {"rms":{"current_a":3166385,"current_b":3125691,"current_c":3131983,"current_s":1756557,"voltage_a":-767262,"voltage_b":-763439,"voltage_c":-749854},"angles":{"angle0":180,"angle1":176,"angle2":176},"powers":{"totactive": {"a":-1345820,"b":-1347328,"c":-1351979}},"freq":0}
  uint32_t len = strlen(json) +1;
  if (len < 7) { return false; }          // Too short

  char json_buffer[len];
  memcpy(json_buffer, json, len);         // Keep original safe
  JsonParser parser(json_buffer);
  JsonParserObject root = parser.getRootObject();
  if (!root) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("A78: Invalid JSON"));
    return false;
  }

  // All parameters are optional allowing for partial changes
  JsonParserToken val = root[PSTR("freq")];
  if (val) { Ade7880.calib_frequency = val.getUInt(); }
  JsonParserObject rms = root[PSTR("rms")].getObject();
  if (rms) {
    val = rms[PSTR("current_a")];
    if (val) { Ade7880.calib_current[0] = val.getInt(); }
    val = rms[PSTR("current_b")];
    if (val) { Ade7880.calib_current[1] = val.getInt(); }
    val = rms[PSTR("current_c")];
    if (val) { Ade7880.calib_current[2] = val.getInt(); }
    val = rms[PSTR("current_s")];
    if (val) { Ade7880.calib_current[3] = val.getInt(); }
    val = rms[PSTR("voltage_a")];
    if (val) { Ade7880.calib_voltage[0] = val.getInt(); }
    val = rms[PSTR("voltage_b")];
    if (val) { Ade7880.calib_voltage[1] = val.getInt(); }
    val = rms[PSTR("voltage_c")];
    if (val) { Ade7880.calib_voltage[2] = val.getInt(); }
  }
  JsonParserObject angles = root[PSTR("angles")].getObject();
  if (angles) {
    val = angles[PSTR("angle0")];
    if (val) { Ade7880.calib_angle[0] = val.getUInt(); }
    val = angles[PSTR("angle1")];
    if (val) { Ade7880.calib_angle[1] = val.getUInt(); }
    val = angles[PSTR("angle2")];
    if (val) { Ade7880.calib_angle[2] = val.getUInt(); }
  }
  JsonParserObject powers = root[PSTR("powers")].getObject();
  if (powers) {
    JsonParserObject totactive = powers[PSTR("totactive")].getObject();
    if (totactive) {
      val = totactive[PSTR("a")];
      if (val) { Ade7880.calib_acpower[0] = val.getInt(); }
      val = totactive[PSTR("b")];
      if (val) { Ade7880.calib_acpower[1] = val.getInt(); }
      val = totactive[PSTR("c")];
      if (val) { Ade7880.calib_acpower[2] = val.getInt(); }
    }
  }
  return true;
}

void Ade7880Defaults(void) {
  Ade7880.calib_frequency = ADE7880_FREQ_INIT;
  Ade7880.calib_current[0] = ADE7880_AIGAIN_INIT;
  Ade7880.calib_current[1] = ADE7880_BIGAIN_INIT;
  Ade7880.calib_current[2] = ADE7880_CIGAIN_INIT;
  Ade7880.calib_current[3] = ADE7880_NIGAIN_INIT;
  Ade7880.calib_voltage[0] = ADE7880_AVGAIN_INIT;
  Ade7880.calib_voltage[1] = ADE7880_BVGAIN_INIT;
  Ade7880.calib_voltage[2] = ADE7880_CVGAIN_INIT;
  Ade7880.calib_acpower[0] = ADE7880_APGAIN_INIT;
  Ade7880.calib_acpower[1] = ADE7880_BPGAIN_INIT;
  Ade7880.calib_acpower[2] = ADE7880_CPGAIN_INIT;
  Ade7880.calib_angle[0] = ADE7880_APHCAL_INIT;
  Ade7880.calib_angle[1] = ADE7880_BPHCAL_INIT;
  Ade7880.calib_angle[2] = ADE7880_CPHCAL_INIT;

  String calib = "";
#ifdef USE_UFILESYS
  calib = TfsLoadString("/calib.dat");
#endif  // USE_UFILESYS
#ifdef USE_RULES
  // rule3 on file#calib.dat do {"rms":{"current_a":3166385,"current_b":3125691,"current_c":3131983,"current_s":1756557,"voltage_a":-767262,"voltage_b":-763439,"voltage_c":-749854},"angles":{"angle0":180,"angle1":176,"angle2":176},"powers":{"totactive": {"a":-1345820,"b":-1347328,"c":-1351979}},"freq":0} endon
  if (!calib.length()) {
    calib = RuleLoadFile("CALIB.DAT");
  }
#endif  // USE_RULES
  if (calib.length()) {
//    AddLog(LOG_LEVEL_DEBUG, PSTR("A78: File '%s'"), calib.c_str());
    Ade7880SetDefaults(calib.c_str());
  }
}

void Ade7880DrvInit(void) {
  if (PinUsed(GPIO_ADE7880_IRQ) && PinUsed(GPIO_ADE7880_IRQ, 1)) {
    if (I2cSetDevice(ADE7880_ADDR)) {
      I2cSetActiveFound(ADE7880_ADDR, "ADE7880");

      pinMode(Pin(GPIO_ADE7880_IRQ), INPUT);
      attachInterrupt(Pin(GPIO_ADE7880_IRQ), Ade7880Isr0, FALLING);
      Ade7880.irq0_state = 0;
      pinMode(Pin(GPIO_ADE7880_IRQ, 1), INPUT);

      Ade7880Defaults();

      if (Ade7880SetCalibrate()) {
        Energy->phase_count = 3;                                            // Three phases
//        Settings->flag5.energy_phase = 1;                                  // SetOption129 - (Energy) Show phase information
//        Energy->use_overtemp = true;                                        // Use global temperature for overtemp detection
        Energy->local_energy_active_export = true;
        TasmotaGlobal.energy_driver = XNRG_23;
      }
    }
  }
}

bool Ade7880Command(void) {
  bool serviced = false;

  if (CMND_ENERGYCONFIG == Energy->command_code) {
    // Non-pesistent settings
    // EnergyConfig {"rms":{"current_a":3166385,"current_b":3125691,"current_c":3131983,"current_s":1756557,"voltage_a":-767262,"voltage_b":-763439,"voltage_c":-749854},"angles":{"angle0":180,"angle1":176,"angle2":176},"powers":{"totactive": {"a":-1345820,"b":-1347328,"c":-1351979}},"freq":0}
    // EnergyConfig {"rms":{"voltage_c":-549854}}
    // EnergyConfig {"freq":0}
    if (XdrvMailbox.data_len) {
#ifdef ADE7880_DEBUG
      if ('1' == XdrvMailbox.data[0]) {
        // EnergyConfig 1 - Dump DSP data memory (0x4380..0x43B9)
        char data[600] = { 0 };
        for (uint32_t i = 0; i < 57; i++) {
          int32_t value = Ade7880Read(ADE7880_AIGAIN + i);
//          snprintf_P(data, sizeof(data), PSTR("%s%s%08X"), data, (i)?",":"", value);
          if (bitRead(value, 27)) { value |= 0xF0000000; }  // Make 24-bit negative (ZPSE)
          snprintf_P(data, sizeof(data), PSTR("%s%s%d"), data, (i)?",":"", value);
        }
        AddLog(LOG_LEVEL_DEBUG, PSTR("A78: DSP Regs 0x4380..B9 '%s'"), data);
        return true;
      }
      if ('2' == XdrvMailbox.data[0]) {
        // EnergyConfig 2 - Dump DSP UI data memory (0x43C0..0x43C7)
        char data[600] = { 0 };
        for (uint32_t i = 0; i < 8; i++) {
          int32_t value = Ade7880Read(ADE7880_AIRMS + i);
          snprintf_P(data, sizeof(data), PSTR("%s%s%08X"), data, (i)?",":"", value);
//          if (7 == i) {
//            if (bitRead(value, 27)) { value |= 0xF0000000; }  // Make 28-bit negative (ZP)
//          } else {
//            if (bitRead(value, 23)) { value |= 0xFF000000; }  // Make 24-bit negative (ZP)
//          }
//          snprintf_P(data, sizeof(data), PSTR("%s%s%d"), data, (i)?",":"", value);
        }
        AddLog(LOG_LEVEL_DEBUG, PSTR("A78: DSP Regs 0x43C0..C7 '%s'"), data);
        return true;
      }
#endif  // ADE7880_DEBUG
      Ade7880Defaults();                      // Load defaults
      if (Ade7880SetDefaults(XdrvMailbox.data)) {
        bool status = Ade7880SetCalibrate();
        AddLog(LOG_LEVEL_DEBUG, PSTR("A78: Re-init status %d"), status);
      }
    }
    Response_P(PSTR("{\"%s\":{"
                    "\"rms\":{\"current_a\":%d,\"current_b\":%d,\"current_c\":%d,\"current_s\":%d,\"voltage_a\":%d,\"voltage_b\":%d,\"voltage_c\":%d},"
                    "\"angles\":{\"angle0\":%d,\"angle1\":%d,\"angle2\":%d},"
                    "\"powers\":{\"totactive\":{\"a\":%d,\"b\":%d,\"c\":%d}},\"freq\":%d}}"),
                    XdrvMailbox.command,
                    Ade7880.calib_current[0], Ade7880.calib_current[1], Ade7880.calib_current[2], Ade7880.calib_current[3],
                    Ade7880.calib_voltage[0], Ade7880.calib_voltage[1], Ade7880.calib_voltage[2],
                    Ade7880.calib_angle[0], Ade7880.calib_angle[1], Ade7880.calib_angle[2],
                    Ade7880.calib_acpower[0], Ade7880.calib_acpower[1], Ade7880.calib_acpower[2], Ade7880.calib_frequency);
    serviced = true;
  }

  return serviced;
}

/*********************************************************************************************\
 * Show
\*********************************************************************************************/

#ifdef ADE7880_MORE_REGS
void Ade7880Show(bool json) {
  if (json) {
    ResponseAppend_P(PSTR(",\"" D_JSON_CURRENT_NEUTRAL "\":%s"), EnergyFmt(&Ade7880.neutral_current, Settings->flag2.current_resolution, 1));
#ifdef USE_WEBSERVER
  } else {
    WSContentSend_PD(HTTP_SNS_CURRENT_N, WebEnergyFmt(&Ade7880.neutral_current, Settings->flag2.current_resolution, 1));
#endif  // USE_WEBSERVER
  }
}
#endif  // ADE7880_MORE_REGS

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xnrg23(uint32_t function) {
  if (!I2cEnabled(XI2C_65)) { return false; }

  bool result = false;

  switch (function) {
    case FUNC_LOOP:
      if (Ade7880.irq0_state) { Ade7880Service0(); }
      break;
    case FUNC_EVERY_SECOND:
      Ade7880Watchdog();
      break;
#ifdef ADE7880_MORE_REGS
    case FUNC_JSON_APPEND:
      Ade7880Show(1);
      break;
#ifdef USE_WEBSERVER
    case FUNC_WEB_COL_SENSOR:
      Ade7880Show(0);
      break;
#endif  // USE_WEBSERVER
#endif  // ADE7880_MORE_REGS
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
