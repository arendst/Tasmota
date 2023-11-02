/*
  xsns_83_neopool.ino - Sugar Valley NeoPool Control System Modbus support for Tasmota

  Copyright (C) 2022  Norbert Richter

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

#ifdef USE_NEOPOOL
/*********************************************************************************************\
 * Sugar Valley NeoPool electronic pool control and water treatment system, also known as brand
 *   Hidrolife (yellow case)
 *   Aquascenic (blue case)
 *   Oxilife (green case)
 *   Bionet (light blue case)
 *   Hidroniser (red case)
 *   UVScenic (lilca case)
 *   Station (orange case)
 *   Brilix
 *   Bayrol
 *   Hay
 *
 * Sugar Valley RS485 connector inside (DISPLAY/WIFI/EXTERN)
 * pins (from top to bottom):
 *
 *    RS485 MODBUS
 *      ___
 *   1 |*  |- +12V (internal power supply)
 *   2 |*  |- (not connected)
 *   3 |*  |- Modbus A+
 *   4 |*  |- Modbus B-
 *   5 |*__|- Modbus GND
 *
 *   RS485 Parameter: 19200 Baud / 1 Stopbit / Parity None
 *
 *   Channel connector DISPLAY is useless as long as the internal display is also connect,
 *   use WIFI or EXTERN instead.
 *
 * Hardware serial will be selected if GPIO1 = [NeoPool Rx] and GPIO3 = [NeoPool Tx]
\*********************************************************************************************/

#define XSNS_83                      83

#ifndef NEOPOOL_MODBUS_SPEED
#define NEOPOOL_MODBUS_SPEED         19200
#endif

#ifndef NEOPOOL_MODBUS_ADDRESS
#define NEOPOOL_MODBUS_ADDRESS       1      // Modbus address, "WIFI" uses 1, "EXTERN" defaults also 1
#endif


#define NEOPOOL_READ_REGISTER        0x04   // Function code used to read register
#define NEOPOOL_WRITE_REGISTER       0x10   // Function code used to write register
#define NEOPOOL_READ_TIMEOUT           25   // read data timeout in ms
#define NEOPOOL_DATA_TIMEOUT        30000   // directly read data register data discard timout in ms


// Pool LED RGB lights with different programs, the individual programs can be selected
// by switching them off and on again for a defined time when the LED is switched on.
// Default timings for LED light program step sequence (NPLight 3)
#define NEOPOOL_LIGHT_PRG_WAIT       30     // delay before start prg light if light was off (in ms)
#define NEOPOOL_LIGHT_PRG_DELAY      15     // default next light prg delay (in ms)
#define NEOPOOL_LIGHT_PRG_DELAY_MIN  5      // next light prg delay min (in ms)
#define NEOPOOL_LIGHT_PRG_DELAY_MAX  100    // next light prg delay max (in ms)


/*********************************************************************************************\
 * Sugar Valley Modbus Register (addresses marked with * are queried with each polling cycle)
 * (see https://downloads.vodnici.net/uploads/wpforo/attachments/69/171-Modbus-registers.pdf)
\*********************************************************************************************/
enum NeoPoolRegister {
                                          // addr    Unit   Description
                                          // ------  ------ ------------------------------------------------------------
  // MODBUS page (0x00xx) - undocumented - for internal use
  MBF_POWER_MODULE_VERSION = 0x0002,      // 0x0002         undocumented - power module version (MSB=Major, LSB=Minor)
  MBF_POWER_MODULE_NODEID = 0x0004,       // 0x0004         undocumented - power module Node ID (6 register 0x0004 - 0x0009)
  MBF_POWER_MODULE_REGISTER = 0x000C,     // 0x000C         undocumented - Writing an address in this register causes the power module register address to be read out into MBF_POWER_MODULE_DATA, see MBF_POWER_MODULE_REG_*
  MBF_POWER_MODULE_DATA = 0x000D,         // 0x000D         undocumented - power module data as requested in MBF_POWER_MODULE_REGISTER
  MBF_VOLT_24_36 = 0x0022,                // 0x0022*        undocumented - Current 24-36V line in mV
  MBF_VOLT_12 = 0x0023,                   // 0x0023*        undocumented - Current 12V line in mV
  MBF_VOLT_5 = 0x006A,                    // 0x006A*        undocumented - 5V line in mV / 0,62069
  MBF_AMP_4_20_MICRO = 0x0072,            // 0x0072*        undocumented - 2-40mA line in µA * 10 (1=0,01mA)

  // MEASURE page (0x01xx)
  MBF_ION_CURRENT = 0x0100,               // 0x0100*        Current measured in the ionization system
  MBF_HIDRO_CURRENT,                      // 0x0101*        Intensity level currently measured in the hydrolysissystem
  MBF_MEASURE_PH,                         // 0x0102* ph     Level measured in hundredths (700 = 7.00)
  MBF_MEASURE_RX,                         // 0x0103* mV     Redox level in mV
  MBF_MEASURE_CL,                         // 0x0104* ppm    Level measured in hundredths of chlorine ppm (100 = 1.00 ppm)
  MBF_MEASURE_CONDUCTIVITY,               // 0x0105* %      Level of conductivity measured in the water.
  MBF_MEASURE_TEMPERATURE,                // 0x0106* °C     Water temperature sensor (100 = 10.0°C)
  MBF_PH_STATUS,                          // 0x0107* mask   Status of the module control pH
  MBF_RX_STATUS,                          // 0x0108* mask   Status of the Rx-module
  MBF_CL_STATUS,                          // 0x0109* mask   Status of the Chlorine-module
  MBF_CD_STATUS,                          // 0x010A* mask   Status of the Conductivity-module
  MBF_ION_STATUS = 0x010C,                // 0x010C* mask   Status of the Ionization-module
  MBF_HIDRO_STATUS,                       // 0x010D* mask   Status of the Hydrolysis-module
  MBF_RELAY_STATE,                        // 0x010E* mask   Status of each configurable relays
  MBF_HIDRO_SWITCH_VALUE,                 // 0x010F*        INTERNAL - contains the opening of the hydrolysis PWM.
  MBF_NOTIFICATION,                       // 0x0110* mask   Reports whether a page of properties has changed since the last time it was consulted.
  MBF_HIDRO_VOLTAGE,                      // 0x0111         Reports on the stress applied to the hydrolysis cell. This register, together with that of MBF_HIDRO_CURRENT allows extrapolating the salinity of the water.

  // GLOBAL page (0x02xx)
  MBF_CELL_RUNTIME_LOW = 0x0206,          // 0x0206*        undocumented - cell runtime (32 bit) - low word
  MBF_CELL_RUNTIME_HIGH,                  // 0x0207*        undocumented - cell runtime (32 bit) - high word
  MBF_CELL_RUNTIME_PART_LOW,              // 0x0208*        undocumented - cell part runtime (32 bit) - low word
  MBF_CELL_RUNTIME_PART_HIGH,             // 0x0209*        undocumented - cell part runtime (32 bit) - high word
  MBF_CELL_BOOST = 0x020C,                // 0x020C* mask   undocumented - 0x0000 = Boost Off, 0x05A0 = Boost with redox ctrl, 0x85A0 = Boost without redox ctrl
  MBF_CELL_RUNTIME_POLA_LOW = 0x0214,     // 0x0214*        undocumented - cell runtime polarity A (32 bit) - low word
  MBF_CELL_RUNTIME_POLA_HIGH,             // 0x0215*        undocumented - cell runtime polarity A (32 bit) - high word
  MBF_CELL_RUNTIME_POLB_LOW,              // 0x0216*        undocumented - cell runtime polarity B (32 bit) - low word
  MBF_CELL_RUNTIME_POLB_HIGH,             // 0x0217*        undocumented - cell runtime polarity B (32 bit) - high word
  MBF_CELL_RUNTIME_POL_CHANGES_LOW,       // 0x0218*        undocumented - cell runtime polarity changes (32 bit) - low word
  MBF_CELL_RUNTIME_POL_CHANGES_HIGH,      // 0x0219*        undocumented - cell runtime polarity changes (32 bit) - high word
  MBF_HIDRO_MODULE_VERSION = 0x0280,      // 0x0280         undocumented - Hydrolysis module version
  MBF_HIDRO_MODULE_CONNECTIVITY = 0x0281, // 0x0281         undocumented - Hydrolysis module connection quality (in myriad: 0..10000)
  MBF_SET_COUNTDOWN_KEY_AUX_OFF = 0x0287, // 0x0287  mask   undocumented - switch AUX1-4 OFF - only for AUX which is assigned as AUX and set to MBV_PAR_CTIMER_COUNTDOWN_KEY_* mode  - see MBV_PAR_CTIMER_COUNTDOWN_KEY_AUX*
  MBF_SET_COUNTDOWN_KEY_AUX_ON,           // 0x0288  mask   undocumented - switch AUX1-4 ON  - only for AUX which is assigned as AUX and set to MBV_PAR_CTIMER_COUNTDOWN_KEY_* mode - see MBV_PAR_CTIMER_COUNTDOWN_KEY_AUX*
  MBF_SET_MANUAL_CTRL,                    // 0x0289         undocumented - write a 1 before manual control MBF_RELAY_STATE, after done write 0 and do MBF_EXEC
  MBF_ESCAPE = 0x0297,                    // 0x0297         undocumented - A write operation to this register is the same as using the ESC button on main screen - clears error
  MBF_SAVE_TO_EEPROM = 0x02F0,            // 0x02F0         A write operation to this register starts a EEPROM storage operation immediately. During the EEPROM storage procedure, the system may be unresponsive to MODBUS requests. The operation will last always less than 1 second.
  MBF_EXEC = 0x02F5,                      // 0x02F5         undocumented - immediately take over settings - a write operation to this register take over the previous written data

  // FACTORY page (0x03xx)
  MBF_PAR_VERSION = 0x0300,               // 0x0300*        Software version of the PowerBox (unused)
  MBF_PAR_MODEL,                          // 0x0301* mask   System model options
  MBF_PAR_SERNUM,                         // 0x0302*        Serial number of the PowerBox (unused)
  MBF_PAR_ION_NOM,                        // 0x0303*        Ionization maximum production level (DO NOT WRITE!)
  MBF_PAR_HIDRO_NOM = 0x0306,             // 0x0306*        Hydrolysis maximum production level. (DO NOT WRITE!) If the hydrolysis is set to work in percent mode, this value will be 100. If the hydrolysis module is set to work in g/h production, this module will contain the maximum amount of production in g/h units. (DO NOT WRITE!)
  MBF_PAR_SAL_AMPS = 0x030A,              // 0x030A         Current command in regulation for which we are going to measure voltage
  MBF_PAR_SAL_CELLK,                      // 0x030B         Specifies the relationship between the resistance obtained in the measurement process and its equivalence in g / l (grams per liter)
  MBF_PAR_SAL_TCOMP,                      // 0x030C         Specifies the deviation in temperature from the conductivity.
  MBF_PAR_HIDRO_MAX_VOLTAGE = 0x0322,     // 0x0322         Allows setting the maximum voltage value that can be reached with the hydrolysis current regulation. The value is specified in tenths of a volt. The default value of this register when the EEPROM is cleared is 80, which is equivalent to a maximum cell operating voltage of 8 volts.
  MBF_PAR_HIDRO_FLOW_SIGNAL,              // 0x0323         Allows to select the operation of the flow detection signal associated with the operation of the hydrolysis (see MBV_PAR_HIDRO_FLOW_SIGNAL*). The default value for this register is 0 (standard detection).
  MBF_PAR_HIDRO_MAX_PWM_STEP_UP,          // 0x0324         This register sets the PWM ramp up of the hydrolysis in pulses per duty cycle. This register makes it possible to adjust the rate at which the power delivered to the cell increases, allowing a gradual rise in power so that the operation of the switching source of the equipment is not saturated. Default 150
  MBF_PAR_HIDRO_MAX_PWM_STEP_DOWN,        // 0x0325         This register sets the PWM down ramp of the hydrolysis in pulses per duty cycle. This register allows adjusting the rate at which the power delivered to the cell decreases, allowing a gradual drop in power so that the switched source of the equipment is not disconnected due to lack of consumption. This gradual fall must be in accordance with the type of cell used, since said cell stores charge once the current stimulus has ceased. Default 20

  // INSTALLER page (0x04xx)
  MBF_PAR_ION_POL0 = 0x0400,              // 0x0400         Time in min the team must spend working on positive polarization in copper-silver ionization.
  MBF_PAR_ION_POL1,                       // 0x0401         Time in min the team must spend working on positive polarization in copper-silver ionization.
  MBF_PAR_ION_POL2,                       // 0x0402         Time in min the team must spend working on positive polarization in copper-silver ionization.
  MBF_PAR_HIDRO_ION_CAUDAL,               // 0x0403  mask   Bitmask register regulates the external control mode of ionization, hydrolysis and pumps.
  MBF_PAR_HIDRO_MODE,                     // 0x0404         Regulates the external control mode of hydrolysis from the modules of measure. 0: no control, 1: standard control (on / off), 2: with timed pump
  MBF_PAR_HIDRO_POL0,                     // 0x0405         Time must spend working on positive polarization in hydrolysis / electrolysis. Time is stored in minutes.
  MBF_PAR_HIDRO_POL1,                     // 0x0406         Time must spend working on positive polarization in hydrolysis / electrolysis. Time is stored in minutes.
  MBF_PAR_HIDRO_POL2,                     // 0x0407         Time must spend working on positive polarization in hydrolysis / electrolysis. Time is stored in minutes.
  MBF_PAR_TIME_LOW,                       // 0x0408*        System timestamp (32 bit unixtime) - low word
  MBF_PAR_TIME_HIGH,                      // 0x0409*        System timestamp (32 bit unixtime) - high word
  MBF_PAR_PH_ACID_RELAY_GPIO,             // 0x040A*        Relay number assigned to the acid pump function (only with pH module).
  MBF_PAR_PH_BASE_RELAY_GPIO,             // 0x040B*        Relay number assigned to the base pump function (only with pH module).
  MBF_PAR_RX_RELAY_GPIO,                  // 0x040C*        Relay number assigned to the Redox level regulation function. If the value is 0, there is no relay assigned, and therefore there is no pump function (ON / OFF should not be displayed)
  MBF_PAR_CL_RELAY_GPIO,                  // 0x040D*        Relay number assigned to the chlorine pump function (only with free chlorine measuring modules).
  MBF_PAR_CD_RELAY_GPIO,                  // 0x040E*        Relay number assigned to the conductivity (brine) pump function (only with conductivity measurement modules).
  MBF_PAR_TEMPERATURE_ACTIVE,             // 0x040F*        Indicates whether the equipment has a temperature measurement or not.
  MBF_PAR_LIGHTING_GPIO,                  // 0x0410*        Relay number assigned to the lighting function. 0: inactive.
  MBF_PAR_FILT_MODE,                      // 0x0411*        Filtration mode (see MBV_PAR_FILT_*)
  MBF_PAR_FILT_GPIO,                      // 0x0412*        Relay selected to perform the filtering function (by default it is relay 2). When this value is at zero, there is no relay assigned and therefore it is understood that the equipment does not control the filtration. In this case, the filter option does not appear in the user menu.
  MBF_PAR_FILT_MANUAL_STATE,              // 0x0413         Filtration status in manual mode (on = 1; off = 0)
  MBF_PAR_HEATING_MODE,                   // 0x0414         Heating mode. 0: the equipment is not heated. 1: the equipment is heating.
  MBF_PAR_HEATING_GPIO,                   // 0x0415         Relay selected to perform the heating function (by default it is relay 7). When this value is at zero, there is no relay assigned and therefore it is understood that the equipment does not control the heating. In this case, the filter modes associated with heating will not be displayed.
  MBF_PAR_HEATING_TEMP,                   // 0x0416         Heating setpoint temperature
  MBF_PAR_CLIMA_ONOFF,                    // 0x0417         Activation of the air conditioning mode (0 inactive; 1 active.
  MBF_PAR_SMART_TEMP_HIGH,                // 0x0418         Superior temperature of the Smart mode
  MBF_PAR_SMART_TEMP_LOW,                 // 0x0419         Lower temperature of the Smart mode
  MBF_PAR_SMART_ANTI_FREEZE,              // 0x041A         Antifreeze mode activated (1) or not (0). This adjustment is only available in the Smart filtration mode.
  MBF_PAR_SMART_INTERVAL_REDUCTION,       // 0x041B         This register is read-only and reports to the outside what percentage (0 to 100%) is being applied to the nominal filtration time. 100% means that the total programmed time is being filtered.
  MBF_PAR_INTELLIGENT_TEMP,               // 0x041C         Setpoint temperature for smart mode
  MBF_PAR_INTELLIGENT_FILT_MIN_TIME,      // 0x041D         Minimum filtration time in minutes
  MBF_PAR_INTELLIGENT_BONUS_TIME,         // 0x041E         Bonus time for the current set of intervals
  MBF_PAR_INTELLIGENT_TT_NEXT_INTERVAL,   // 0x041F         Time to next filtration interval. When it reaches 0 an interval is started and the number of seconds is reloaded for the next interval (2x3600)
  MBF_PAR_INTELLIGENT_INTERVALS,          // 0x0420         Number of started intervals. When it reaches 12 it is reset to 0 and the bonus time is reloaded with the value of MBF_PAR_INTELLIGENT_FILT_MIN_TIME
  MBF_PAR_FILTRATION_STATE,               // 0x0421         Filtration function state: 0 is off and 1 is on. The filtration state is regulated according to the MBF_PAR_FILT_MANUAL_STATE register if the filtration mode held in register MBF_PAR_FILT_MODE is set to FILT_MODE_MANUAL (0).
  MBF_PAR_HEATING_DELAY_TIME,             // 0x0422         Timer in seconds that counts up when the heating is to be enabled. Once this counter reaches 60 seconds, the heating is then enabled. This counter is for internal use only.
  MBF_PAR_FILTERING_TIME_LOW,             // 0x0423         32-bit value:
  MBF_PAR_FILTERING_TIME_HIGH,            // 0x0424         Internal timer for the intelligent filtering mode. It counts the filtering time done during a given day. This register is only for internal use and should not be modified by the user.
  MBF_PAR_INTELLIGENT_INTERVAL_TIME_LOW,  // 0x0425         32-bit value:
  MBF_PAR_INTELLIGENT_INTERVAL_TIME_HIGH, // 0x0426         Internal timer that counts the filtration interval assigned to the the intelligent mode. This register is only for internal use and should not be modified by the user.
  MBF_PAR_UV_MODE,                        // 0x0427         UV mode active or not - see MBV_PAR_UV_MODE*. To enable UV support for a given device, add the mask MBMSK_MODEL_UV to the MBF_PAR_MODEL register.
  MBF_PAR_UV_HIDE_WARN,                   // 0x0428  mask   Suppression for warning messages in the UV mode.
  MBF_PAR_UV_RELAY_GPIO,                  // 0x0429         Relay number assigned to the UV function.
  MBF_PAR_PH_PUMP_REP_TIME_ON,            // 0x042A  mask   Time that the pH pump will be turn on in the repetitive mode (see MBMSK_PH_PUMP_*). Contains a special time format, see desc for MBMSK_PH_PUMP_TIME.
  MBF_PAR_PH_PUMP_REP_TIME_OFF,           // 0x042B  mask   Time that the pH pump will be turn off in the repetitive mode. Contains a special time format, see desc for MBMSK_PH_PUMP_TIME, has no upper configuration bit 0x8000
  MBF_PAR_HIDRO_COVER_ENABLE,             // 0x042C  mask   Options for the hydrolysis/electrolysis module (see MBMSK_HIDRO_*)
  MBF_PAR_HIDRO_COVER_REDUCTION,          // 0x042D         Configured levels for the cover reduction and the hydrolysis shutdown temperature options: LSB = Percentage for the cover reduction, MSB = Temperature level for the hydrolysis shutdown (see MBMSK_HIDRO_*)
  MBF_PAR_PUMP_RELAY_TIME_OFF,            // 0x042E         Time level in minutes or seconds that the dosing pump must remain off when the temporized pump mode is selected. This time level register applies to all pumps except pH. Contains a special time format, see desc for MBMSK_PH_PUMP_TIME, has no upper configuration bit 0x8000
  MBF_PAR_PUMP_RELAY_TIME_ON,             // 0x042F         Time level in minutes or seconds that the dosing pump must remain on when the temporized pump mode is selected. This time level register applies to all pumps except pH. Contains a special time format, see desc for MBMSK_PH_PUMP_TIME, has no upper configuration bit 0x8000
  MBF_PAR_RELAY_PH,                       // 0x0430         Determine what pH regulation configuration the equipment has (see MBV_PAR_RELAY_PH_*)
  MBF_PAR_RELAY_MAX_TIME,                 // 0x0431         Maximum amount of time, in seconds, that a dosing pump can operate before rising an alarm signal. The behavior of the system when the dosing time is exceeded is regulated by the type of action stored in the MBF_PAR_RELAY_MODE register.
  MBF_PAR_RELAY_MODE,                     // 0x0432         Behavior of the system when the dosing time is exceeded (see MBMSK_PAR_RELAY_MODE_* and MBV_PAR_RELAY_MODE_*)
  MBF_PAR_RELAY_ACTIVATION_DELAY,         // 0x0433         Delay time in seconds for the pH pump when the measured pH value is outside the allowable pH setpoints. The system internally adds an extra time of 10 seconds to the value stored here. The pump starts the dosing operation once the condition of pH out of valid interval is maintained during the time specified in this register.
  MBF_PAR_TIMER_BLOCK_BASE,               // 0x0434         This block of 180 registers holds the configuration of the system timers. The system has a set of 12 fully configurable timers, each one assigned to a specific function, described below:
  MBF_PAR_TIMER_BLOCK_FILT_INT1 = 0x0434, // 0x0434         Filtration interval 1 (15 register - see MBV_TIMER_OFFMB_* for desc)
  MBF_PAR_TIMER_BLOCK_FILT_INT2 = 0x0443, // 0x0443         Filtration interval 2 (15 register - see MBV_TIMER_OFFMB_* for desc)
  MBF_PAR_TIMER_BLOCK_FILT_INT3 = 0x0452, // 0x0452         Filtration interval 3 (15 register - see MBV_TIMER_OFFMB_* for desc)
  MBF_PAR_TIMER_BLOCK_AUX1_INT2 = 0x0461, // 0x0461         Auxiliary relay 1 - 2. interval (15 register - see MBV_TIMER_OFFMB_* for desc)
  MBF_PAR_TIMER_BLOCK_LIGHT_INT = 0x0470, // 0x0470         Lighting interval (15 register - see MBV_TIMER_OFFMB_* for desc)
  MBF_PAR_TIMER_BLOCK_AUX2_INT2 = 0x047F, // 0x047F         Auxiliary relay 2 - 2. interval (15 register - see MBV_TIMER_OFFMB_* for desc)
  MBF_PAR_TIMER_BLOCK_AUX3_INT2 = 0x048E, // 0x048E         Auxiliary relay 3 - 2. interval (15 register - see MBV_TIMER_OFFMB_* for desc)
  MBF_PAR_TIMER_BLOCK_AUX4_INT2 = 0x049D, // 0x049D         Auxiliary relay 4 - 2. interval (15 register - see MBV_TIMER_OFFMB_* for desc)
  MBF_PAR_TIMER_BLOCK_AUX1_INT1 = 0x04AC, // 0x04AC         Auxiliary relay 1 - 1. interval (15 register - see MBV_TIMER_OFFMB_* for desc)
  MBF_PAR_TIMER_BLOCK_AUX2_INT1 = 0x04BB, // 0x04BB         Auxiliary relay 2 - 1. interval (15 register - see MBV_TIMER_OFFMB_* for desc)
  MBF_PAR_TIMER_BLOCK_AUX3_INT1 = 0x04CA, // 0x04CA         Auxiliary relay 3 - 1. interval (15 register - see MBV_TIMER_OFFMB_* for desc)
  MBF_PAR_TIMER_BLOCK_AUX4_INT1 = 0x04D9, // 0x04D9         Auxiliary relay 4 - 1. interval (15 register - see MBV_TIMER_OFFMB_* for desc)
  MBF_PAR_FILTVALVE_ENABLE = 0x04E8,      // 0x04E8         Filter cleaning functionality mode (0 = off, 1 = Besgo)
  MBF_PAR_FILTVALVE_MODE,                 // 0x04E9         Filter cleaning valve timing mode, possible modes: MBV_PAR_CTIMER_ENABLED, MBV_PAR_CTIMER_ALWAYS_ON, MBV_PAR_CTIMER_ALWAYS_OFF
  MBF_PAR_FILTVALVE_GPIO,                 // 0x04EA         Relay associated with the filter cleaning function. default AUX2 (value 5)
  MBF_PAR_FILTVALVE_START_LOW,            // 0x04EB         start timestamp of filter cleaning (32-bit)
  MBF_PAR_FILTVALVE_START_HIGH,           // 0x04EC           "
  MBF_PAR_FILTVALVE_PERIOD_MINUTES,       // 0x04ED         Period in minutes between cleaning actions. For example, if a value of 60 is stored in this registry, a cleanup action will occur every hour.
  MBF_PAR_FILTVALVE_INTERVAL,             // 0x04EE         Cleaning action duration in seconds.
  MBF_PAR_FILTVALVE_REMAINING,            // 0x04EF         Time remaining for the current cleaning action in seconds. If this register is 0, it means that there is no cleaning function running. When a cleanup function is started, the contents of the MBF_PAR_FILTVALVE_INTERVAL register are copied to this register, then decremented once per second. The display uses this log to track the progress of the cleaning function.
  MBF_ACTION_COPY_TO_RTC,                 // 0x04F0         A write (any value) forces the writing of the RTC time registers MBF_PAR_TIME_LOW (0x0408) and MBF_PAR_TIME_HIGH (0x0409) into the RTC internal microcontroller clock management registers.

  // USER page (0x05xx)  To make the modification of this register persistent, execute the EEPROM storage procedure described in global register MBF_SAVE_TO_EEPROM.
  MBF_PAR_ION = 0x0500,                   // 0x0500*        Ionization target production level. The value adjusted in this register must not exceed the value set in the MBF_PAR_ION_NOM factory register.
  MBF_PAR_ION_PR,                         // 0x0501*        Amount of time in minutes that the ionization must be activated each time that the filtration starts.
  MBF_PAR_HIDRO,                          // 0x0502*        Hydrolisis target production level. When the hydrolysis production is to be set in percent values, this value will contain the percent of production. If the hydrolysis module is set to work in g/h production, this module will contain the desired amount of production in g/h units. The value adjusted in this register must not exceed the value set in the MBF_PAR_HIDRO_NOM factory register.
  MBF_PAR_PH1 = 0x0504,                   // 0x0504*        Higher limit of the pH regulation system. The value set in this register is multiplied by 100. This means that if we want to set a value of 7.5, the numerical content that we must write in this register is 750. This register must be always higher than MBF_PAR_PH2.
  MBF_PAR_PH2,                            // 0x0505*        Lower limit of the pH regulation system. The value set in this register is multiplied by 100. This means that if we want to set a value of 7.0, the numerical content that we must write in this register is 700. This register must be always lower than MBF_PAR_PH1.
  MBF_PAR_RX1 = 0x0508,                   // 0x0508*        Set point for the redox regulation system. This value must be in the range of 0 to 1000.
  MBF_PAR_CL1 = 0x050A,                   // 0x050A*        Set point for the chlorine regulation system. The value stored in this register is multiplied by 100. This mean that if we want to set a value of 1.5 ppm, we will have to write a numerical value of 150. This value stored in this register must be in the range of 0 to 1000.
  MBF_PAR_FILTRATION_CONF = 0x050F,       // 0x050F* mask   undocumented - filtration type and speed, see MBMSK_PAR_FILTRATION_CONF_*
  MBF_PAR_FILTRATION_SPEED_FUNC = 0x0513, // 0x0513         undocumented - filtration speed function control
  MBF_PAR_FUNCTION_DEPENDENCY = 0x051B,   // 0x051B  mask   Specification for the dependency of different functions, such as heating, from external events like FL1 (see MBMSK_FCTDEP_HEATING/MBMSK_DEPENDENCY_*)

  // MISC page (0x06xx)
  MBF_PAR_UICFG_MACHINE = 0x0600,         // 0x0600*        Machine type (see MBV_PAR_MACH_* and  kNeoPoolMachineNames[])
  MBF_PAR_UICFG_LANGUAGE,                 // 0x0601*        Selected language (see MBV_PAR_LANG_*)
  MBF_PAR_UICFG_BACKLIGHT,                // 0x0602*        Display backlight (see MBV_PAR_BACKLIGHT_*)
  MBF_PAR_UICFG_SOUND,                    // 0x0603* mask   Audible alerts (see MBMSK_PAR_SOUND_*)
  MBF_PAR_UICFG_PASSWORD,                 // 0x0604*        System password encoded in BCD
  MBF_PAR_UICFG_VISUAL_OPTIONS,           // 0x0605* mask   Stores the different display options for the user interface menus (bitmask). Some bits allow you to hide options that are normally visible (bits 0 to 3) while other bits allow you to show options that are normally hidden (bits 9 to 15)
  MBF_PAR_UICFG_VISUAL_OPTIONS_EXT,       // 0x0606* mask   This register stores additional display options for the user interface menus, see MBMSK_VOE_*
  MBF_PAR_UICFG_MACH_VISUAL_STYLE,        // 0x0607* mask   This register is an expansion of register 0x0600 and 0x0605. The lower part of the register (8 bits LSB) is used to store the type of color selected when in register 0x600 has been specified that the machine is of type "generic". Colors and styles correspond to those listed in record 0x600 MBF_PAR_UICFG_MACHINE. The upper part (8-bit MSB) contains extra bits MBMSK_VS_FORCE_UNITS_GRH, MBMSK_VS_FORCE_UNITS_PERCENTAGE and MBMSK_ELECTROLISIS
  MBF_PAR_UICFG_MACH_NAME_BOLD_0,         // 0x0608         This set of 4 registers stores an ASCIIZ string of up to 8 characters that is used to specify the bold part of the title to be displayed at startup if the specified machine type is generic. Note: only lowercase letters (a-z) can be used.
  MBF_PAR_UICFG_MACH_NAME_BOLD_1,         // 0x0609
  MBF_PAR_UICFG_MACH_NAME_BOLD_2,         // 0x060A
  MBF_PAR_UICFG_MACH_NAME_BOLD_3,         // 0x060B
  MBF_PAR_UICFG_MACH_NAME_LIGHT_0,        // 0x060C         This set of 4 registers stores an ASCIIZ string of up to 8 characters that is used to specify the normal intensity part of the title that will be displayed at startup if the specified machine type is generic. Note: Only lowercase letters (a-z) can be used.
  MBF_PAR_UICFG_MACH_NAME_LIGHT_1,        // 0x060D
  MBF_PAR_UICFG_MACH_NAME_LIGHT_2,        // 0x060E
  MBF_PAR_UICFG_MACH_NAME_LIGHT_3,        // 0x060F
  MBF_PAR_UICFG_MACH_NAME_AUX1_0,         // 0x0610         Aux1 relay name: 5 registers ASCIIZ string of up to 10 characters
  MBF_PAR_UICFG_MACH_NAME_AUX1_1,         // 0x0611
  MBF_PAR_UICFG_MACH_NAME_AUX1_2,         // 0x0612
  MBF_PAR_UICFG_MACH_NAME_AUX1_3,         // 0x0613
  MBF_PAR_UICFG_MACH_NAME_AUX1_4,         // 0x0614
  MBF_PAR_UICFG_MACH_NAME_AUX2_0,         // 0x0615         Aux2 relay name: 5 registers ASCIIZ string of up to 10 characters
  MBF_PAR_UICFG_MACH_NAME_AUX2_1,         // 0x0616
  MBF_PAR_UICFG_MACH_NAME_AUX2_2,         // 0x0617
  MBF_PAR_UICFG_MACH_NAME_AUX2_3,         // 0x0618
  MBF_PAR_UICFG_MACH_NAME_AUX2_4,         // 0x0619
  MBF_PAR_UICFG_MACH_NAME_AUX3_0,         // 0x061A         Aux3 relay name: 5 registers ASCIIZ string of up to 10 characters
  MBF_PAR_UICFG_MACH_NAME_AUX3_1,         // 0x061B
  MBF_PAR_UICFG_MACH_NAME_AUX3_2,         // 0x061C
  MBF_PAR_UICFG_MACH_NAME_AUX3_3,         // 0x061D
  MBF_PAR_UICFG_MACH_NAME_AUX3_4,         // 0x061E
  MBF_PAR_UICFG_MACH_NAME_AUX4_0,         // 0x061F         Aux4 relay name: 5 registers ASCIIZ string of up to 10 characters
  MBF_PAR_UICFG_MACH_NAME_AUX4_1,         // 0x0620
  MBF_PAR_UICFG_MACH_NAME_AUX4_2,         // 0x0621
  MBF_PAR_UICFG_MACH_NAME_AUX4_3,         // 0x0622
  MBF_PAR_UICFG_MACH_NAME_AUX4_4,         // 0x0623
};

// Sugar Valley register constants and bit masks
enum NeoPoolConstAndBitMask {
  // MBF_PH_STATUS
  MBMSK_PH_STATUS_ALARM                   = 0x000F, // PH alarm. The possible alarm values are depending on the regulation model:
      // Valid alarm values for pH regulation with acid and base:
  MBV_PH_ACID_BASE_ALARM0 = 0,                      // no alarm
  MBV_PH_ACID_BASE_ALARM1 = 1,                      // pH too high; the pH value is 0.8 points higher than the setpoint (PH1 on acid systems, PH2 on base systems, PH1 on acid+base systems)
  MBV_PH_ACID_BASE_ALARM2 = 2,                      // pH too low: the pH value is 0.8 points lower than the set point value set in (PH1 on acid systems, PH2 on base systems, PH2 on acid+base systems)
  MBV_PH_ACID_BASE_ALARM3 = 3,                      // pH pump has exceeded the working time set by the MBF_PAR_RELAY_PH_MAX_TIME parameter and has stopped.
  MBV_PH_ACID_BASE_ALARM4 = 4,                      // pH higher than the set point (PH1 + 0.1 on acid systems, PH2 + 0.1 on base systems, PH1 on acid+base systems)
  MBV_PH_ACID_BASE_ALARM5 = 5,                      // pH lower than the set point  (PH1 - 0.3 on acid systems, PH2 - 0.3 on base systems, PH2 on acid+base systems)
  MBV_PH_ACID_BASE_ALARM6 = 6,                      // undocumented - tank level alarm

  MBMSK_PH_STATUS_CTRL_BY_FL              = 0x0400, // 10 Control status of the pH module by flow detection (if enabled by MBF_PAR_HIDRO_ION_CAUDAL)
  MBMSK_PH_STATUS_ACID_PUMP_ACTIVE        = 0x0800, // 11 Acid pH pump relay on (pump on)
  MBMSK_PH_STATUS_BASE_PUMP_ACTIVE        = 0x1000, // 12 Base pH Pump Relay On (Pump On)
  MBMSK_PH_STATUS_CTRL_ACTIVE             = 0x2000, // 13 Active pH control module and controlling pumps
  MBMSK_PH_STATUS_MEASURE_ACTIVE          = 0x4000, // 14 Active pH measurement module and making measurements. If this bit is at 1, the pH bar should be displayed.
  MBMSK_PH_STATUS_MODULE_PRESENT          = 0x8000, // 15 Detected pH measurement module

  // MBF_RX_STATUS
  MBMSK_RX_STATUS_RX_PUMP_ACTIVE          = 0x1000, // 12 Redox pump relay on (pump activated)
  MBMSK_RX_STATUS_CTRL_ACTIVE             = 0x2000, // 13 Active Redox control module and controlling pump
  MBMSK_RX_STATUS_MEASURE_ACTIVE          = 0x4000, // 14 Active Redox measurement module and performing measurements. If this bit is at 1, the Redox bar should be displayed on the screen.
  MBMSK_RX_STATUS_MODULE_PRESENT          = 0x8000, // 15 Redox measurement module detected in the system

  // MBF_CL_STATUS
  MBMSK_CL_STATUS_CHLORINE_FLOW           = 0x0008, //  3 Chlorine Probe Flow Sensor. This sensor is built into the probe itself and serves to detect whether there is water passing through the chlorine measurement probe. In case the sensor is at 0, the chlorine measurement will not be valid.
  MBMSK_CL_STATUS_CL_PUMP_ACTIVE          = 0x1000, // 12 Chlorine pump relay on (pump on)
  MBMSK_CL_STATUS_CTRL_ACTIVE             = 0x2000, // 13 Active chlorine control module and controlling pump
  MBMSK_CL_STATUS_MEASURE_ACTIVE          = 0x4000, // 14 Active chlorine measurement module and taking measurements. If this bit is 1, the chlorine bar should be displayed on the screen.
  MBMSK_CL_STATUS_MODULE_PRESENT          = 0x8000, // 15 Chlorine measurement module detected in the system

  // MBF_CD_STATUS
  MBMSK_CD_STATUS_RX_PUMP_ACTIVE          = 0x1000, // 12 Conductivity pump relay on (pump active)
  MBMSK_CD_STATUS_CTRL_ACTIVE             = 0x2000, // 13 Active conductivity control module and controlling pump
  MBMSK_CD_STATUS_MEASURE_ACTIVE          = 0x4000, // 14 Active conductivity measurement module and making measurements. If this bit is 1, the conditionality bar should be displayed on the screen.
  MBMSK_CD_STATUS_MODULE_PRESENT          = 0x8000, // 15 Conductivity measurement module detected in the system

  // MBF_ION_STATUS
  MBMSK_ION_STATUS_ON_TARGET              = 0x0001, //  0 On Target - the system has reached the set point.
  MBMSK_ION_STATUS_LOW                    = 0x0002, //  1 Low - Ionization cannot reach the set point.
  MBMSK_ION_STATUS_RESERVED               = 0x0004, //  2
  MBMSK_ION_STATUS_PROGTIME_EXCEEDED      = 0x0008, //  3 Pr off - The programmed ionization time has been exceeded
  MBMSK_ION_STATUS_POLOFF                 = 0x1000, // 12 Ion Pol off - Ionization in dead time
  MBMSK_ION_STATUS_POL1                   = 0x2000, // 13 Ion Pol 1 - Ionization working in polarization 1
  MBMSK_ION_STATUS_POL2                   = 0x4000, // 14 Ion Pol 2 - Ionization working in polarization 2

  // MBF_HIDRO_STATUS
  MBMSK_HIDRO_STATUS_ON_TARGET            = 0x0001, //  0 On Target - the system has reached the set point.
  MBMSK_HIDRO_STATUS_LOW                  = 0x0002, //  1 Low - Hydrolysis cannot reach the set point.
  MBMSK_HIDRO_STATUS_RESERVED             = 0x0004, //  2
  MBMSK_HIDRO_STATUS_FL1                  = 0x0008, //  3 Flow - Hydrolysis cell flow indicator (FL1)
  MBMSK_HIDRO_STATUS_COVER                = 0x0010, //  4 Cover - Cover input activated
  MBMSK_HIDRO_STATUS_MODULE_ACTIVE        = 0x0020, //  5 Active - Active Module hydrolysis (hidroEnable)
  MBMSK_HIDRO_STATUS_CTRL_ACTIVE          = 0x0040, //  6 Control - Hydrolysis module working with regulation (hydroControlEnable)
  MBMSK_HIDRO_STATUS_REDOX_ENABLED        = 0x0080, //  7 Redox enable - Activation of hydrolysis by the redox module
  MBMSK_HIDRO_STATUS_SHOCK_ENABLED        = 0x0100, //  8 Hydro shock enabled - Chlorine shock mode enabled
  MBMSK_HIDRO_STATUS_FL2                  = 0x0200, //  9 FL2 - Chlorine probe flow indicator, if present
  MBMSK_HIDRO_STATUS_ENABLED_BY_CHLORINE  = 0x0400, // 10 Cl enable - Activation of hydrolysis by the chlorine module
  MBMSK_HIDRO_STATUS_POLOFF               = 0x1000, // 12 Ion Pol off - Ionization in dead time
  MBMSK_HIDRO_STATUS_POL1                 = 0x2000, // 13 Ion Pol 1 - Ionization working in polarization 1
  MBMSK_HIDRO_STATUS_POL2                 = 0x4000, // 14 Ion Pol 2 - Ionization working in polarization 2

  // MBF_RELAY_STATE
  MBMSK_RELAY_STATE1                      = 0x0001, //  0 Relay 1 state (1 on; 0 off) (normally assigned to ph)
  MBMSK_RELAY_STATE2                      = 0x0002, //  1 Relay 2 state (1 on; 0 off) (normally assigned to filtering)
  MBMSK_RELAY_STATE3                      = 0x0004, //  2 Relay 3 status (1 on; 0 off) (normally assigned to lighting)
  MBMSK_RELAY_STATE4                      = 0x0008, //  3 Relay 4 status (1 on; 0 off)
  MBMSK_RELAY_STATE5                      = 0x0010, //  4 Relay 5 status (1 on; 0 off)
  MBMSK_RELAY_STATE6                      = 0x0020, //  5 Relay 6 status (1 on; 0 off)
  MBMSK_RELAY_STATE7                      = 0x0040, //  6 Relay 7 status (1 on; 0 off)
  MBMSK_RELAY_FILTSPEED_LOW               = 0x0100, //  8 Filtration low speed
  MBMSK_RELAY_FILTSPEED_MID               = 0x0200, //  9 Filtration mid speed
  MBMSK_RELAY_FILTSPEED_HIGH              = 0x0400, // 10 Filtration high speed
  MBMSK_RELAY_FILTSPEED                   = 0x0700, // Filtration current speed mask
  MBSHFT_RELAY_FILTSPEED                  = 8,      // Filtration current speed bit shift

  // MBF_NOTIFICATION
  MBMSK_NOTIF_MODBUS_CHANGED              = 0x0001, //  0 Modbus page changed
  MBMSK_NOTIF_GLOBAL_CHANGED              = 0x0002, //  1 Global page changed
  MBMSK_NOTIF_FACTORY_CHANGED             = 0x0004, //  2 Factory page changed
  MBMSK_NOTIF_INSTALLER_CHANGED           = 0x0008, //  3 Installer page changed
  MBMSK_NOTIF_USER_CHANGED                = 0x0010, //  4 User page changed
  MBMSK_NOTIF_MISC_CHANGED                = 0x0020, //  5 Misc page changed

  // MBF_CELL_BOOST
  MBMSK_CELL_BOOST_REDOX_CTL              = 0x8000, // undocumented - Disable redox ctrl
  MBMSK_CELL_BOOST_STATE                  = 0x0500, // undocumented - Boost
  MBMSK_CELL_BOOST_START                  = 0x00A0, // undocumented - Start boost

  // MBF_PAR_MODEL
  MBMSK_MODEL_ION                         = 0x0001, //  0 The equipment includes ionization control
  MBMSK_MODEL_HIDRO                       = 0x0002, //  1 The equipment includes hydrolysis or electrolysis
  MBMSK_MODEL_UV                          = 0x0004, //  2 The equipment includes disinfection control by ultraviolet lamp
  MBMSK_MODEL_SALINITY                    = 0x0008, //  3 The equipment includes measurement of salinity (Fanless equipment only)

  // MBF_PAR_HIDRO_FLOW_SIGNAL
  MBV_PAR_HIDRO_FLOW_SIGNAL_STD           = 0,      // Standard detection based on conduction between an auxiliary electrode and either of the two electrodes of the cell.
  MBV_PAR_HIDRO_FLOW_SIGNAL_ALWAYS_ON     = 1,      // Always connected. This value allows forcing the generation of the hydrolysis current even if no flow is detected in the sensor.
  MBV_PAR_HIDRO_FLOW_SIGNAL_PADDLE        = 2,      // Detection based on the paddle switch, associated with the FL1 input
  MBV_PAR_HIDRO_FLOW_SIGNAL_PADDLE_AND_STD= 3,      // Detection based on the paddle switch, associated with the FL1 input, and the standard detector. The system will understand that there is flow when both elements detect flow. If either one opens, the hydrolysis will stop.
  MBV_PAR_HIDRO_FLOW_SIGNAL_PADDLE_OR_STD = 4,      // Detection based on the paddle switch, associated with the FL1 input, or the standard detector. The system will understand that there is flow when either of the two elements detects flow. Hydrolysis will stop only if both detectors detect no flow.

  // MBF_PAR_HIDRO_ION_CAUDAL
  MBMSK_HIDRO_ION_CAUDAL_FL1_CTRL         = 0x0001, //  0 If the FL1 signal is detected to be inactive, the actuation of the different elements of the system is disabled.
  MBMSK_HIDRO_ION_CAUDAL_FL2_CTRL         = 0x0002, //  1 If the FL2 signal is detected to be inactive, the actuation of the different elements of the system is disabled.
  MBMSK_HIDRO_ION_CAUDAL_FULL_CL_HIDRO_CTRL=0x0004, //  2 If there is a chlorine module installed and it is detected that its flow sensor is inactive, the action of the different elements of the system is disabled.
  MBMSK_HIDRO_ION_CAUDAL_SLAVE            = 0x0008, //  3 The value of the slave input is taken and if it is inactive, the action of the different elements of the system is disabled.
  MBMSK_HIDRO_ION_CAUDAL_PADDLE_SWITCH    = 0x0010, //  4
  MBMSK_HIDRO_ION_CAUDAL_PADDLE_SWITCH_INV= 0x0020, //  5
  MBMSK_HIDRO_ION_CAUDAL_INVERSION        = 0x0080, //  7 This bit determines if active means open or closed for the input electrical signals, and allows to reverse the operation for example to implement a paddle switch that closes when there is no flow.

  // MBF_PAR_FILT_MODE
  MBV_PAR_FILT_MANUAL                     = 0,      // This mode allows to turn the filtration (and all other systems that depend on it) on and off manually.
  MBV_PAR_FILT_AUTO                       = 1,      // This mode allows filtering to be turned on and off according to the settings of the TIMER1, TIMER2 and TIMER3 timers.
  MBV_PAR_FILT_HEATING                    = 2,      // This mode is similar to the AUTO mode, but includes setting the temperature for the heating function. This mode is activated only if the MBF_PAR_HEATING_MODE register is at 1 and there is a heating relay assigned.
  MBV_PAR_FILT_SMART                      = 3,      // This filtration mode adjusts the pump operating times depending on the temperature. This mode is activated only if the MBF_PAR_TEMPERATURE_ACTIVE register is at 1.
  MBV_PAR_FILT_INTELLIGENT                = 4,      // This mode performs an intelligent filtration process in combination with the heating function. This mode is activated only if the MBF_PAR_HEATING_MODE register is at 1 and there is a heating relay assigned.
  MBV_PAR_FILT_BACKWASH                   = 13,     // This filter mode is started when the backwash operation is activated.

  // MBF_PAR_UV_MODE
  MBV_PAR_UV_MODE0                        = 0,      // UV is switched off and it will not turn on when filtration starts
  MBV_PAR_UV_MODE1                        = 1,      // UV is switched on and it will turn on when filtration starts. Time counter for the UV lamp will be incremented.

  // MBF_PAR_UV_HIDE_WARN
  MBMSK_UV_HIDE_WARN_CLEAN                = 0x0001, //  0
  MBMSK_UV_HIDE_WARN_REPLACE              = 0x0002, //  1

  // MBF_PAR_PH_PUMP_REP_TIME_ON
  MBMSK_PH_PUMP_TIME                      = 0x7FFF, // Time level for the pump: The time level has a special coding format. It can cover periods of 1 to 180 seconds with 1 second granularity and from 3 to 999 minutes with 1 minute granularity. f the value is set to 30 for example, a 30 second time will be considered. If we have the value 200, we will have an on time of (200-180+3) = 23 minutes.
  MBMSK_PH_PUMP_REPETITIVE                = 0x8000, // pH pump in repetitive mode (1)

  // MBF_PAR_HIDRO_COVER_ENABLE
  MBMSK_HIDRO_COVER_ENABLE                = 0x0001, // If enabled, the hydrolysis/electrolysis production will be reduced by a given percentage specified in the lower half of the MBF_PAR_HIDRO_COVER_REDUCTION register when the cover input is detected.
  MBMSK_HIDRO_TEMPERATURE_SHUTDOWN_ENABLE = 0x0002, // If enabled, the hydrolysis/electrolysis production will stop when the temperature falls below a given temperature threshold specified in higher part of the MBF_PAR_HIDRO_COVER_REDUCTION register.
  // MBF_PAR_HIDRO_COVER_REDUCTION
  MBMSK_HIDRO_COVER_REDUCTION             = 0x00FF, // Percentage for the cover reduction
  MBMSK_HIDRO_SHUTDOWN_TEMPERATURE        = 0xFF00, // Temperature level for the hydrolysis shutdown

  // MBF_PAR_RELAY_PH
  MBV_PAR_RELAY_PH_ACID_AND_BASE          = 0,      // The equipment works with an acid and base pump
  MBV_PAR_RELAY_PH_ACID_ONLY              = 1,      // The equipment works with acid pump only
  MBV_PAR_RELAY_PH_BASE_ONLY              = 2,      // The equipment works with base pump only

  //MBF_PAR_RELAY_MODE
  MBMSK_PAR_RELAY_MODE_PH                 = 0x0003, // Behavior for the pH module (MBV_PAR_RELAY_MODE_*)
  MBMSK_PAR_RELAY_MODE_RX                 = 0x000C, // Behavior for the Redox module (MBV_PAR_RELAY_MODE_*)
  MBMSK_PAR_RELAY_MODE_CL                 = 0x0030, // Behavior for the Chlorine module (MBV_PAR_RELAY_MODE_*)
  MBMSK_PAR_RELAY_MODE_CD                 = 0x00C0, // Behavior for the Conductivity module (MBV_PAR_RELAY_MODE_*)
  MBMSK_PAR_RELAY_MODE_TURBIDITY          = 0x0300, // Behavior for the Turbidity module (MBV_PAR_RELAY_MODE_*)
  MBV_PAR_RELAY_MODE_IGNORE               = 0,      // The system simply ignores the alarm and dosing continues.
  MBV_PAR_RELAY_MODE_SHOW_ONLY            = 1,      // The system only shows the alarm on screen, but the dosing continues.
  MBV_PAR_RELAY_MODE_SHOW_AND_STOP        = 2,      // The system shows the alarm on screen and stops the dosing pump

  // MBF_PAR_FILTRATION_CONF
  MBMSK_PAR_FILTRATION_CONF_TYPE          = 0x000F, // Filtration pump type, see MBV_PAR_FILTRATION_TYPE_*
  MBMSK_PAR_FILTRATION_CONF_DEF_SPEED     = 0x0070, // Filtration default speed, see MBV_PAR_FILTRATION_SPEED_*
  MBMSK_PAR_FILTRATION_CONF_INT1_SPEED    = 0x0380, // Filtration speed for timer interval 1, see MBV_PAR_FILTRATION_SPEED_*
  MBMSK_PAR_FILTRATION_CONF_INT2_SPEED    = 0x1C00, // Filtration speed for timer interval 2, see MBV_PAR_FILTRATION_SPEED_*
  MBMSK_PAR_FILTRATION_CONF_INT3_SPEED    = 0xE000, // Filtration speed for timer interval 3, see MBV_PAR_FILTRATION_SPEED_*
  MBSHFT_PAR_FILTRATION_CONF_TYPE         = 0,      // Filtration pump type bit shift
  MBSHFT_PAR_FILTRATION_CONF_DEF_SPEED    = 4,      // Filtration default speed bit shift
  MBSHFT_PAR_FILTRATION_CONF_INT1_SPEED   = 7,      // Filtration speed for timer interval 1 bit shift
  MBSHFT_PAR_FILTRATION_CONF_INT2_SPEED   = 10,     // Filtration speed for timer interval 2 bit shift
  MBSHFT_PAR_FILTRATION_CONF_INT3_SPEED   = 13,     // Filtration speed for timer interval 3 bit shift
  MBV_PAR_FILTRATION_TYPE_STANDARD        = 0,      // Standard (without speed control)
  MBV_PAR_FILTRATION_TYPE_HAYWARD         = 1,      // Variable speed B
  MBV_PAR_FILTRATION_TYPE_SPEED_B         = 2,      // Variable speed B
  MBV_PAR_FILTRATION_SPEED_SLOW           = 0,      // Speed Slow
  MBV_PAR_FILTRATION_SPEED_MEDIUM         = 1,      // Speed Medium
  MBV_PAR_FILTRATION_SPEED_FAST           = 2,      // Speed Fast

  // MBF_PAR_FUNCTION_DEPENDENCY
  MBMSK_FCTDEP_HEATING                    = 0x0007, // Heating function dependency:
  MBMSK_DEPENDENCY_FL1_PADDLE             = 0x0001,
  MBMSK_DEPENDENCY_FL2                    = 0x0002,
  MBMSK_DEPENDENCY_SLAVE                  = 0x0004,

  // MBF_PAR_UICFG_MACHINE
  MBV_PAR_MACH_NONE                       = 0,      // No machine assigned
  MBV_PAR_MACH_HIDROLIFE                  = 1,      // Hidrolife (yellow)
  MBV_PAR_MACH_AQUASCENIC                 = 2,      // Aquascenic (blue)
  MBV_PAR_MACH_OXILIFE                    = 3,      // Oxilife (green)
  MBV_PAR_MACH_BIONET                     = 4,      // Bionet (light blue)
  MBV_PAR_MACH_HIDRONISER                 = 5,      // Hidroniser (red)
  MBV_PAR_MACH_UVSCENIC                   = 6,      // UVScenic (lilac)
  MBV_PAR_MACH_STATION                    = 7,      // Station (orange)
  MBV_PAR_MACH_BRILIX                     = 8,      // Brilix
  MBV_PAR_MACH_GENERIC                    = 9,      // Generic
  MBV_PAR_MACH_BAYROL                     = 10,     // Bayrol
  MBV_PAR_MACH_HAY                        = 11,     // Hay

  // MBF_PAR_UICFG_LANGUAGE
  MBV_PAR_LANG_SPANISH                    = 0,
  MBV_PAR_LANG_ENGLISH                    = 1,
  MBV_PAR_LANG_FRENCH                     = 2,
  MBV_PAR_LANG_GERMAN                     = 3,
  MBV_PAR_LANG_ITALIAN                    = 4,
  MBV_PAR_LANG_TURKISH                    = 5,
  MBV_PAR_LANG_CZECH                      = 6,
  MBV_PAR_LANG_PORTUGUESE                 = 7,
  MBV_PAR_LANG_DUTCH                      = 8,
  MBV_PAR_LANG_POLISH                     = 9,
  MBV_PAR_LANG_HUNGARIAN                  = 10,
  MBV_PAR_LANG_RUSSIAN                    = 11,

  // MBF_PAR_UICFG_BACKLIGHT
  MBV_PAR_BACKLIGHT_15SEC                 = 0,      // Backlight off after 15 sec
  MBV_PAR_BACKLIGHT_30SEC                 = 1,      // Backlight off after 30 sec
  MBV_PAR_BACKLIGHT_60SEC                 = 2,      // Backlight off after 60 sec
  MBV_PAR_BACKLIGHT_5MIN                  = 3,      // Backlight off after 5 min
  MBV_PAR_BACKLIGHT_ON                    = 4,      // Backlight never turns off

  // MBF_PAR_TIMER_BLOCK_BASE
  MBV_TIMER_OFFMB_TIMER_ENABLE            = 0,      // Enables the timer function in the selected working mode, see MBF_PAR_CTIMER_*
  MBV_TIMER_OFFMB_TIMER_ON                = 1,      // Timer start (32-bit timestamp, LSB first)
  MBV_TIMER_OFFMB_TIMER_OFF               = 3,      // Timer stop (32-bit timestamp, LSB first) - not used
  MBV_TIMER_OFFMB_TIMER_PERIOD            = 5,      // Time in seconds between starting points (32-bit, LSB first), e.g. 86400 means daily
  MBV_TIMER_OFFMB_TIMER_INTERVAL          = 7,      // Time in seconds that the timer has to run when started (32-bit, LSB first)
  MBV_TIMER_OFFMB_TIMER_COUNTDOWN         = 9,      // Time remaining in seconds for the countdown mode (32-bit, LSB first)
  MBV_TIMER_OFFMB_TIMER_FUNCTION          = 11,     // Function assigned to this timer, see MBV_PAR_CTIMER_FCT_*
  MBV_TIMER_OFFMB_TIMER_WORK_TIME         = 13,     // Number of seconds that the timer has been operating
  // MBV_TIMER_OFFMB_TIMER_ENABLE working modes:
  MBV_PAR_CTIMER_DISABLE                  = 0,      // Timer disabled
  MBV_PAR_CTIMER_ENABLED                  = 1,      // Timer enabled and independent
  MBV_PAR_CTIMER_ENABLED_LINKED           = 2,      // Timer enabled and linked to relay from timer 0
  MBV_PAR_CTIMER_ALWAYS_ON                = 3,      // Relay assigned to this timer always on
  MBV_PAR_CTIMER_ALWAYS_OFF               = 4,      // Relay assigned to this timer always off
  MBV_PAR_CTIMER_COUNTDOWN_KEY_PLUS       = 0x0105, // Timer in countdown mode using + key
  MBV_PAR_CTIMER_COUNTDOWN_KEY_MINUS      = 0x0205, // Timer in countdown mode using - key
  MBV_PAR_CTIMER_COUNTDOWN_KEY_ARROWDOWN  = 0x0405, // Timer in countdown mode using arrow-down key

  // MBF_SET_COUNTDOWN_KEY_AUX_* mask:
  MBV_PAR_CTIMER_COUNTDOWN_KEY_AUX1       = 0x0100, // Aux1 ON/OFF, when MBV_TIMER_OFFMB_TIMER_ENABLE is set to MBV_PAR_CTIMER_COUNTDOWN_KEY_*
  MBV_PAR_CTIMER_COUNTDOWN_KEY_AUX2       = 0x0200, // Aux2 ON/OFF, when MBV_TIMER_OFFMB_TIMER_ENABLE is set to MBV_PAR_CTIMER_COUNTDOWN_KEY_*
  MBV_PAR_CTIMER_COUNTDOWN_KEY_AUX3       = 0x0400, // Aux3 ON/OFF, when MBV_TIMER_OFFMB_TIMER_ENABLE is set to MBV_PAR_CTIMER_COUNTDOWN_KEY_*
  MBV_PAR_CTIMER_COUNTDOWN_KEY_AUX4       = 0x0800, // Aux4 ON/OFF, when MBV_TIMER_OFFMB_TIMER_ENABLE is set to MBV_PAR_CTIMER_COUNTDOWN_KEY_*

  // MBV_TIMER_OFFMB_TIMER_FUNCTION codes:
  MBV_PAR_CTIMER_FCT_FILTRATION           = 0x0001, // Filtration function of the system
  MBV_PAR_CTIMER_FCT_LIGHTING             = 0x0002, // Lighting function of the system
  MBV_PAR_CTIMER_FCT_HEATING              = 0x0004, // Heating function of the system
  MBV_PAR_CTIMER_FCT_AUXREL1              = 0x0100, // Function assigned to relay 1 (Filtration)
  MBV_PAR_CTIMER_FCT_AUXREL2              = 0x0200, // Function assigned to relay 2 (Light)
  MBV_PAR_CTIMER_FCT_AUXREL3              = 0x0400, // Function assigned to relay 3 (Heating)
  MBV_PAR_CTIMER_FCT_AUXREL4              = 0x0800, // Function assigned to relay 4 (AUX1)
  MBV_PAR_CTIMER_FCT_AUXREL5              = 0x1000, // Function assigned to relay 5 (AUX2)
  MBV_PAR_CTIMER_FCT_AUXREL6              = 0x2000, // Function assigned to relay 6 (AUX3)
  MBV_PAR_CTIMER_FCT_AUXREL7              = 0x4000, // Function assigned to relay 7 (AUX4)

  // MBF_PAR_UICFG_SOUND
  MBMSK_PAR_SOUND_CLICK                   = 0x0001, //  0 Click sounds every time a key is pressed
  MBMSK_PAR_SOUND_POPUP                   = 0x0002, //  1 Sound plays each time a pop-up message appears
  MBMSK_PAR_SOUND_ALERTS                  = 0x0004, //  2 An alarm sounds when there is an alert on the equipment (AL3)
  MBMSK_PAR_SOUND_FILTRATION              = 0x0008, //  3 Audible warning every time the filtration is started

  // MBF_PAR_UICFG_VISUAL_OPTIONS
  MBMSK_HIDE_TEMPERATURE                  = 0x0001, //  0 Hide temperature measurement from main menu
  MBMSK_HIDE_FILTRATION                   = 0x0002, //  1 Hide filter status from main menu
  MBMSK_HIDE_LIGHTING                     = 0x0004, //  2 Hide lighting status from main menu
  MBMSK_HIDE_AUX_RELAYS                   = 0x0008, //  3 Hide auxiliary relay status from main menu.
  MBMSK_VO_HIDE_EXTRA_REGS                = 0x0010, //  4 Hide the option to adjust additional registers in the installer menu
  MBMSK_VO_HIDE_RELAY_CONFIG              = 0x0020, //  5 Hide the relay configuration option in the installer menu.
  MBMSK_VO_SLOW_FILTER_HIDRO_LEVEL        = 0x0040, //  6 This option enables the slow hydrolysis level filtering option when the pH module is installed. This is especially important when the acid / base dosing is done very close to the hydrolysis probe.
  MBMSK_VO_HIDE_SALINITY_MAIN_WINDOW      = 0x0080, //  7 Hides the salinity measurement from main screen.
  MBMSK_VO_SHOW_SPECIAL_REGS              = 0x0100, //  8 Displays the special register set configuration menu in the installer menu.
  MBMSK_SHOW_HID_SHUTDOWN_BY_TEMPERATURE  = 0x0200, //  9 Displays the option to turn off hydrolysis by temperature.
  MBMSK_SHOW_CELL_SELECTION               = 0x0400, // 10 Enables access to the cell selection menu from the service menu option of the configuration menu.
  MBMSK_SHOW_PUMP_TYPE                    = 0x0800, // 11 Displays the option for selecting the type of filtration pump (normal, three speeds, etc.).
  MBMSK_SHOW_QUICK_MENU                   = 0x1000, // 12 Displays the quick access menu instead of the conventional menu, when the SET key is pressed from the main display screen. Filtration (normal, three speeds, etc).
  MBMSK_SHOW_OXI_MAIN_DATA_SCREEN         = 0x2000, // 13 Displays main screen shown with a particular style called OXI
  MBMSK_SHOW_INSTALLER_MENU               = 0x4000, // 14 Shows access to the installer menu in the main menu without the need for a password.
  MBMSK_SHOW_FACTORY_MENU                 = 0x8000, // 15 Shows access to the factory menu in the main menu without the need for a password.

  // MBF_PAR_UICFG_VISUAL_OPTIONS_EXT
  MBMSK_VOE_SHOW_PNEUMATIC_VALVE          = 0x0001, //  0 Shows the pneumatic valve
  MBMSK_VOE_HIDE_AUX_REL_DEPENDENCY       = 0x0002, //  1 Hides the auxiliary relay dependency
  MBMSK_VOE_SHOW_BESGO_NAME               = 0x0004, //  2 Show “Besgo” instead of “Pneumatic” for the pneumatic valve titles.
  MBMSK_VOE_HIDE_SMART_INTEL_MODES        = 0x0008, //  3 Hide smart intelligent modes
  MBMSK_VOE_ENABLE_ADVANCED_CALIBRATION   = 0x0010, //  4 Enable advanced calibration
  MBMSK_VOE_SHOW_DOSING_PUMP_SCALING      = 0x0020, //  5 Show dosing pump scaling
  MBMSK_VOE_HIDE_MASTER_SLAVE_SEL         = 0x0030, //  6 Hide master slave selection
  MBMSK_VOE_ENABLE_ADVANCED_MASTER_SLAVE  = 0x0080, //  7 Enable advanced Master Slave
  MBMSK_VOE_SHOW_OVERTEMP_PROTECTION      = 0x0100, //  8 Show overtemperature protection
  MBMSK_VOE_SHOW_HYDRO_MODE_SEL           = 0x0200, //  9 Show hidrolysis mode selection
  MBMSK_VOE_SHOW_ADVANCED_EXT_CONTROL     = 0x0400, // 10 Show advanced external controls
  MBMSK_VOE_HIDE_BASIC_EXT_CONTROL        = 0x0800, // 11 Hide basic external controls
  MBMSK_VOE_HIDE_DIAGNOSTICS              = 0x1000, // 12 Hide diagnostics

  // MBF_PAR_UICFG_MACH_VISUAL_STYLE
  MBMSK_VS_FORCE_UNITS_GRH                = 0x2000, // 13 Display the hydrolysis/electrolysis in units of grams per hour (gr/h).
  MBMSK_VS_FORCE_UNITS_PERCENTAGE         = 0x4000, // 14 Display the hydrolysis/electrolysis in percentage units (%).
  MBMSK_ELECTROLISIS                      = 0x8000, // 15 Display the word electrolysis instead of hydrolysis in generic mode.

  // MBF_POWER_MODULE_REG_*
  MBV_POWER_MODULE_REG_INFO = 0,          // undocumented - set of 26-byte power module register stores an ASCIIZ string containing the subversion and timestamp of the module, e. g. ".57\nMay 26 2020\n01:08:10\n\0"
};

#include <TasmotaModbus.h>
TasmotaModbus *NeoPoolModbus;

// emulates a g/h system !!!! only for development purposes on % systems
// enables also cmnd 'NPgPerh [0|1]': 0 = disables emulation, 1 enables emulation
//#define NEOPOOL_EMULATE_GPERH 16          // Max g/h power of an emulated % system
#ifdef NEOPOOL_EMULATE_GPERH
bool neopool_system_gperh = false;        // emulation defaults off
#endif


#define NEOPOOL_RELAY_MAX           7     // Number of relais build-in

enum NeoPoolResult {
  NEOPOOL_RESULT_DEC = false,
  NEOPOOL_RESULT_HEX,
  NEOPOOL_RESULT_MAX
};
uint8_t neopool_result = NEOPOOL_RESULT_HEX;

bool neopool_active = false;
volatile bool neopool_poll = true;

uint8_t neopool_read_state = 0;
uint8_t neopool_send_retry = 0;
uint8_t neopool_failed_count = 0;
bool neopool_error = true;

uint16_t neopool_power_module_version;
uint16_t neopool_power_module_nodeid[6];

#define NEOPOOL_MAX_REPEAT_ON_ERROR 10
uint8_t neopool_repeat_on_error = 2;

uint16_t neopool_light_relay;
uint8_t neopool_light_prg_delay;
uint8_t neopoll_cmd_delay = 0;
void (* neopoll_cmd)(void) = nullptr;

// Modbus register set to read
// Defines blocks of register read once with a single read
struct {
  const uint16_t addr;
  const uint16_t cnt;
  uint16_t *data;
} NeoPoolReg[] = {
  // complete poll cycle needs 1750 ms to read complete register set
  {MBF_ION_CURRENT,       MBF_NOTIFICATION                  - MBF_ION_CURRENT       + 1, nullptr},
  {MBF_CELL_RUNTIME_LOW,  MBF_CELL_RUNTIME_POL_CHANGES_HIGH - MBF_CELL_RUNTIME_LOW  + 1, nullptr},
  {MBF_PAR_VERSION,       MBF_PAR_HIDRO_NOM                 - MBF_PAR_VERSION       + 1, nullptr},
  {MBF_PAR_TIME_LOW,      MBF_PAR_HEATING_GPIO              - MBF_PAR_TIME_LOW      + 1, nullptr},
  {MBF_PAR_ION,           MBF_PAR_FILTRATION_CONF           - MBF_PAR_ION           + 1, nullptr},
  {MBF_PAR_UICFG_MACHINE, MBF_PAR_UICFG_MACH_VISUAL_STYLE   - MBF_PAR_UICFG_MACHINE + 1, nullptr},
  {MBF_VOLT_24_36,        MBF_VOLT_12                       - MBF_VOLT_24_36        + 1, nullptr},
  {MBF_VOLT_5,            MBF_AMP_4_20_MICRO                - MBF_VOLT_5            + 1, nullptr}
};

typedef struct {
  uint16_t addr;
  uint16_t data;
  uint32_t ts;
} TNeoPoolData;
uint16_t NeoPoolDataCount = 0;
TNeoPoolData* NeoPoolData = nullptr;

// NeoPool modbus function errors
enum NeoPoolModbusCode {
  NEOPOOL_MODBUS_OK = 0,
  NEOPOOL_MODBUS_ERROR_RW_DATA,
  NEOPOOL_MODBUS_ERROR_TIMEOUT,
  NEOPOOL_MODBUS_ERROR_OUT_OF_MEM,
  NEOPOOL_MODBUS_ERROR_DEADLOCK
};

// NeoPool value resolutions
typedef struct {
  uint16_t ph : 2;
  uint16_t cl : 2;
  uint16_t ion : 2;
} NeoPoolResMBitfield;
NeoPoolResMBitfield neopool_resolution {
  .ph = 1,
  .cl = 1,
  .ion = 1
};


#define D_NEOPOOL_NAME "NeoPool"

#define D_NEOPOOL_JSON_FILTRATION_NONE        ""
#define D_NEOPOOL_JSON_FILTRATION_MANUAL      "Manual"
#define D_NEOPOOL_JSON_FILTRATION_AUTO        "Auto"
#define D_NEOPOOL_JSON_FILTRATION_HEATING     "Heating"
#define D_NEOPOOL_JSON_FILTRATION_SMART       "Smart"
#define D_NEOPOOL_JSON_FILTRATION_INTELLIGENT "Intelligent"
#define D_NEOPOOL_JSON_FILTRATION_BACKWASH    "Backwash"
#define D_NEOPOOL_JSON_MODULES                "Modules"
#define D_NEOPOOL_JSON_POWERUNIT              "Powerunit"
#define D_NEOPOOL_JSON_CHLORINE               "Chlorine"
#define D_NEOPOOL_JSON_CONDUCTIVITY           "Conductivity"
#define D_NEOPOOL_JSON_FILTRATION             "Filtration"
#define D_NEOPOOL_JSON_FILTRATION_MODE        "Mode"
#define D_NEOPOOL_JSON_FILTRATION_SPEED       "Speed"
#define D_NEOPOOL_JSON_HYDROLYSIS             "Hydrolysis"
#define D_NEOPOOL_JSON_PERCENT                "Percent"
#define D_NEOPOOL_JSON_CELL_RUNTIME           "Runtime"
#define D_NEOPOOL_JSON_CELL_RUNTIME_TOTAL     "Total"
#define D_NEOPOOL_JSON_CELL_RUNTIME_PART      "Part"
#define D_NEOPOOL_JSON_CELL_RUNTIME_POL1      "Pol1"
#define D_NEOPOOL_JSON_CELL_RUNTIME_POL2      "Pol2"
#define D_NEOPOOL_JSON_CELL_RUNTIME_CHANGES   "Changes"
#define D_NEOPOOL_JSON_IONIZATION             "Ionization"
#define D_NEOPOOL_JSON_LIGHT                  "Light"
#define D_NEOPOOL_JSON_LIGHT_MODE             "Mode"
#define D_NEOPOOL_JSON_REDOX                  "Redox"
#define D_NEOPOOL_JSON_RELAY                  "Relay"
#define D_NEOPOOL_JSON_RELAY_PH_ACID          "Acid"
#define D_NEOPOOL_JSON_RELAY_PH_BASE          "Base"
#define D_NEOPOOL_JSON_RELAY_RX               "Redox"
#define D_NEOPOOL_JSON_RELAY_CL               "Chlorine"
#define D_NEOPOOL_JSON_RELAY_CD               "Conductivity"
#define D_NEOPOOL_JSON_RELAY_HEATING          "Heating"
#define D_NEOPOOL_JSON_RELAY_UV               "UV"
#define D_NEOPOOL_JSON_RELAY_FILTVALVE        "Valve"
#define D_NEOPOOL_JSON_AUX                    "Aux"
#define D_NEOPOOL_JSON_STATE                  "State"
#define D_NEOPOOL_JSON_TYPE                   "Type"
#define D_NEOPOOL_JSON_UNIT                   "Unit"
#define D_NEOPOOL_JSON_COVER                  "Cover"
#define D_NEOPOOL_JSON_SHOCK                  "Boost"
#define D_NEOPOOL_JSON_LOW                    "Low"
#define D_NEOPOOL_JSON_SETPOINT               "Setpoint"
#define D_NEOPOOL_JSON_MIN                    "Min"
#define D_NEOPOOL_JSON_MAX                    "Max"
#define D_NEOPOOL_JSON_PHPUMP                 "Pump"
#define D_NEOPOOL_JSON_FLOW1                  "FL1"
#define D_NEOPOOL_JSON_TANK                   "Tank"
#define D_NEOPOOL_JSON_BIT                    "Bit"
#define D_NEOPOOL_JSON_NODE_ID                "NodeID"

const char kNeoPoolMachineNames[] PROGMEM =
  D_NEOPOOL_MACH_NONE "|"
  D_NEOPOOL_MACH_HIDROLIFE "|"
  D_NEOPOOL_MACH_AQUASCENIC "|"
  D_NEOPOOL_MACH_OXILIFE "|"
  D_NEOPOOL_MACH_BIONET "|"
  D_NEOPOOL_MACH_HIDRONISER "|"
  D_NEOPOOL_MACH_UVSCENIC "|"
  D_NEOPOOL_MACH_STATION "|"
  D_NEOPOOL_MACH_BRILIX "|"
  D_NEOPOOL_MACH_GENERIC "|"
  D_NEOPOOL_MACH_BAYROL "|"
  D_NEOPOOL_MACH_HAY
  ;

const char kNeoPoolFiltrationMode[] PROGMEM =
  D_NEOPOOL_FILTRATION_MANUAL "|"
  D_NEOPOOL_FILTRATION_AUTO "|"
  D_NEOPOOL_FILTRATION_HEATING "|"
  D_NEOPOOL_FILTRATION_SMART "|"
  D_NEOPOOL_FILTRATION_INTELLIGENT "|"
  D_NEOPOOL_FILTRATION_BACKWASH
  ;
const char kNeoPoolFiltrationModeCmnd[] PROGMEM =
  D_NEOPOOL_JSON_FILTRATION_MANUAL "|"
  D_NEOPOOL_JSON_FILTRATION_AUTO "|"
  D_NEOPOOL_JSON_FILTRATION_HEATING "|"
  D_NEOPOOL_JSON_FILTRATION_SMART "|"
  D_NEOPOOL_JSON_FILTRATION_INTELLIGENT "|"
  D_NEOPOOL_JSON_FILTRATION_BACKWASH
  ;
const uint8_t sNeoPoolFiltrationMode[] PROGMEM = {
  MBV_PAR_FILT_MANUAL,
  MBV_PAR_FILT_AUTO,
  MBV_PAR_FILT_HEATING,
  MBV_PAR_FILT_SMART,
  MBV_PAR_FILT_INTELLIGENT,
  MBV_PAR_FILT_BACKWASH };

const char kNeoPoolFiltrationSpeed[] PROGMEM =
  D_NEOPOOL_FILTRATION_NONE "|"
  D_NEOPOOL_FILTRATION_SLOW "|"
  D_NEOPOOL_FILTRATION_MEDIUM "|"
  D_NEOPOOL_FILTRATION_FAST
  ;

const char kNeoPoolpHAlarms[] PROGMEM =
  D_NEOPOOL_SETPOINT_OK "|"
  D_NEOPOOL_PH_HIGH "|"
  D_NEOPOOL_PH_LOW "|"
  D_NEOPOOL_PUMP_TIME_EXCEEDED
  ;

#define NEOPOOL_FMT_PH          "%*_f"
#define NEOPOOL_FMT_RX          "%d"
#define NEOPOOL_FMT_CL          "%*_f"
#define NEOPOOL_FMT_CD          "%d"
#define NEOPOOL_FMT_ION         "%*_f"
#define NEOPOOL_FMT_HIDRO       "%*_f"

#define D_NEOPOOL_UNIT_PERCENT  "%"
#define D_NEOPOOL_UNIT_GPERH    "g/h"

const char HTTP_SNS_NEOPOOL_TIME[]             PROGMEM = "{s}%s " D_NEOPOOL_TIME            "{m}%s"                                                               "{e}";
const char HTTP_SNS_NEOPOOL_VOLTAGE[]          PROGMEM = "{s}%s " D_VOLTAGE                 "{m}%*_f / %*_f / %*_f " D_UNIT_VOLT                                         "{e}";
const char HTTP_SNS_NEOPOOL_HYDROLYSIS[]       PROGMEM = "{s}%s " D_NEOPOOL_HYDROLYSIS      "{m}"  NEOPOOL_FMT_HIDRO  " %s ";
const char HTTP_SNS_NEOPOOL_PH[]               PROGMEM = "{s}%s " D_PH                      "{m}"  NEOPOOL_FMT_PH;
const char HTTP_SNS_NEOPOOL_REDOX[]            PROGMEM = "{s}%s " D_NEOPOOL_REDOX           "{m}"  NEOPOOL_FMT_RX     " "  D_UNIT_MILLIVOLT;
const char HTTP_SNS_NEOPOOL_PPM_CHLORINE[]     PROGMEM = "{s}%s " D_NEOPOOL_CHLORINE        "{m}"  NEOPOOL_FMT_CL     " "  D_UNIT_PARTS_PER_MILLION               "{e}";
const char HTTP_SNS_NEOPOOL_CONDUCTIVITY[]     PROGMEM = "{s}%s " D_NEOPOOL_CONDUCTIVITY    "{m}"  NEOPOOL_FMT_CD     " "  D_UNIT_PERCENT                         "{e}";
const char HTTP_SNS_NEOPOOL_IONIZATION[]       PROGMEM = "{s}%s " D_NEOPOOL_IONIZATION      "{m}"  NEOPOOL_FMT_ION    " "  "%s%s"                                 "{e}";
const char HTTP_SNS_NEOPOOL_FILT_MODE[]        PROGMEM = "{s}%s " D_NEOPOOL_FILT_MODE       "{m}%s"                                                               "{e}";
const char HTTP_SNS_NEOPOOL_RELAY[]            PROGMEM = "{s}%s " "%s"                      "{m}%s"                                                               "{e}";
const char HTTP_SNS_NEOPOOL_CELL_RUNTIME[]     PROGMEM = "{s}%s " D_NEOPOOL_CELL_RUNTIME    "{m}%s"                                                               "{e}";

const char HTTP_SNS_NEOPOOL_STATUS[]           PROGMEM = "<span style=\"background-color:%s;font-size:small;text-align:center;%s;\">&nbsp;%s&nbsp;</span>";
const char HTTP_SNS_NEOPOOL_STATUS_NORMAL[]    PROGMEM = "filter:invert(0.1)";
const char HTTP_SNS_NEOPOOL_STATUS_DISABLED[]  PROGMEM = "display: none";
const char HTTP_SNS_NEOPOOL_STATUS_INACTIVE[]  PROGMEM = "filter:opacity(0.15)";
const char HTTP_SNS_NEOPOOL_STATUS_ACTIVE[]    PROGMEM = "filter:invert(1)";


/*********************************************************************************************\
 * Commands
 *
 * NPFiltration {<state> {speed}}
 *            get/set manual filtration (state = 0..2, speed = 1..3)
 *            get filtration state if <state> is omitted, otherwise set new state
 *              0 - switch filtration pump off
 *              1 - switch filtration pump on
 *              2 - toggle filtration pump
 *            for non-standard filtration types additional speed control is possible
 *
 * NPFiltrationMode {<mode>}
 *            get/set filtration mode (mode = 0..4|13)
 *            get mode if <mode> is omitted, otherwise set new mode according:
 *              0 - Manual
 *              1 - Auto
 *              2 - Heating
 *              3 - Smart
 *              4 - Intelligent
 *             13 - Backwash
 *
 * NPFiltrationSpeed {<speed>}
 *            (only available for non-standard filtration types)
 *            get/set manual filtration speed (speed = 1..3)
 *            get filtration speed if <speed> is omitted, otherwise set new speed
 *              1 - low
 *              2 - mid
 *              3 - high
 *
 * NPTime {<time>}
 *            get/set system time
 *            get current time if <time> is omitted, otherwise set time according:
 *              0 - sync with Tasmota local time
 *              1 - sync with Tasmota utc time
 *            any other value of <time> will set time as epoch
 *
 * NPLight {<state> {delay}}
 *            get/set light (state = 0|1|2|3|4)
 *            get light state if <state> is omitted, otherwise set new state
 *              0 - switch light manual off
 *              1 - switch light manual on
 *              2 - toggle light
 *              3 - switch light to auto mode
 *              4 - switch to next program (for RGB-LED lights) (delay = 5..100)
 *                  prg change by switch light of for delay time then switch on
 *                  delay in ms from 0.5 - 10 sec
 *
 * NPpHMin {<ph>}
 *            (only available if pH module is installed)
 *            get/set pH lower limit (ph = 0..14)
 *            get current limit if <ph> is omitted, otherwise set
 *
 * NPpHMax {<ph>}
 *            (only available if pH module is installed)
 *            get/set pH upper limit (ph = 0..14)
 *            get current limit if <ph> is omitted, otherwise set
 *
 * NPpH {<ph>}
 *            (only available if pH module is installed)
 *            get/set pH upper limit (ph = 0..14)
 *            same as NPpHMax
 *
 * NPRedox {<setpoint>}
 *            (only available if redox module is installed)
 *            get/set redox set point in mV (setpoint = 0..100
 *            get current set point if <setpoint> is omitted, otherwise set
 *
 * NPHydrolysis {<level> {%}}
 *            (only available if hydrolysis/electrolysis control is present)
 *            get/set hydrolysis/electrolysis level
 *            get current level if <level> is omitted, otherwise set:
 *            0..100 in % for systems configured to %
 *            0..<max> in g/h for systems configured for g/h (<max> depends by MBF_PAR_HIDRO_NOM register value)
 *            <level> can specified in % on all systems by appending the % sign to the value
 *
 * NPIonization {<level>}
 *            (only available if ionization control is present)
 *            get/set ionization target production level (level = 0..x, the upper limit of the range may vary depending on the MBF_PAR_ION_NOM register)
 *            get current level if <level> is omitted, otherwise set
 *
 * NPChlorine {<setpoint>}
 *            (only available if free chlorine probe detector is installed)
 *            get/set chlorine set point in ppm (setpoint = 0..10)
 *            get current set point if <setpoint> is omitted, otherwise set
 *
 * NPControl
 *            Show information about system controls
 *
 * NPSave
 *            write data permanently into EEPROM
 *
 * NPExec
 *            immediately take over changed data (without writing to EEPROM)
 *
 * NPEscape
 *            clears possible errors (like pump exceeded time etc.)
 *
 * NPResult {<format>}
 *            get/set addr/data result format read/write commands (format = 0|1):
 *            get output format if <format> is omitted, otherwise
 *              0 - output as decimal numbers
 *              1 - output as hexadecimal strings (default)
 *
 * NPOnError {<repeat>}
 *            get/set auto-repeat Modbus read/write commands on error (repeat = 0..10):
 *            get auto-repeat setting if <repeat> is omitted, otherwise
 *              0     - disable auto-repeat on read/write error
 *              1..10 - repeat commands n times until ok
 *
 * NPPHRes {<digits>}
 * NPCLRes {<digits>}
 * NPIonRes {<digits>}
 *            get/set number of digits in results for PH, CL and ION values
 *
 *
 * NPRead <addr> {<cnt>}
 * NPReadL <addr> {<cnt>}
 *            read 16/32-bit register (cnt = 1..30|1..15), cnt = 1 if omitted
 *            NPRead read 16-bit register
 *            NPReadL read 32-bit register
 *
 * NPWrite <addr> <data> {<data>...}
 * NPWriteL <addr> <data> {<data>...}
 *            NPWrite write 16-bit register (data = 0..65535), <data> max 10 times
 *            NPWriteL write 32-bit register (data = 0..4294967295), <data> max 10 times
 *
 * NPBit <addr> <bit> {<data>}
 * NPBitL <addr> <bit> {<data>}
 *            read/write register bit (bit = 0..15, data = 0|1)
 *            read if <data> is omitted, otherwise set <bit> to new <data>
 *
 * Examples:
 *
 * Get/Set filtration mode
 *    NPFiltrationMode
 *    RESULT = {"NPFiltrationmode":"Manual"}
 *    NPFiltrationMode 1
 *    {"NPFiltrationmode":"Auto"}
 *
 * Switch light relay on
 *    NPLight 1
 *    RESULT = {"NPLight":"ON"}
 *
 * Read Heating setpoint temperature MBF_PAR_HEATING_TEMP
 *    Backlog NPResult 0;NPRead 0x416
 *    RESULT = {"NPResult":0}
 *    RESULT = {"NPRead":{"Address":1046,"Data":28}}
 *
 * Read system time MBF_PAR_TIME_* as 32-bit register using decimal output
 *    Backlog NPResult 0;NPReadL 0x408
 *    RESULT = {"NPResult":0}
 *    RESULT = {"NPReadL":{"Address":1032,"Data":1612124540}}
 *
 * Enable temperature module by setting MBF_PAR_TEMPERATURE_ACTIVE and set it permanently into EEPROM
 *    Backlog NPWrite 0x40F,1;NPSave
 *    RESULT = {"NPWrite":{"Address":"0x040F","Data":"0x0001"}}
 *    RESULT = {"NPSave":"Done"}
 *
 * Hide auxiliary relay display from main menu by setting bit 3 of MBF_PAR_UICFG_VISUAL_OPTIONS
 *    NPBit 0x605,3,1
 *    RESULT = {"NPBit":{"Address":"0x0605","Data":"0x08C8"}}
 *
 * Read Filtration interval 1-3 settings
 *    Backlog NPResult 0;NPRead 0x434;NPReadL 0x435,7;NPRead 0x443;NPReadL 0x444,7;NPRead 0x452;NPReadL 0x0453,7
 *    RESULT = {"NPResult":0}
 *    RESULT = {"NPRead":{"Address":1076,"Data":1}}
 *    RESULT = {"NPReadL":{"Address":1077,"Data":[28800,0,86400,14400,0,1,0]}}
 *    RESULT = {"NPRead":{"Address":1091,"Data":1}}
 *    RESULT = {"NPReadL":{"Address":1092,"Data":[43200,0,86400,21600,0,1,0]}}
 *    RESULT = {"NPRead":{"Address":1106,"Data":1}}
 *    RESULT = {"NPReadL":{"Address":1107,"Data":[0,0,86400,0,0,1,0]}} *
 *
 * Set filtration interval 1 to daily 9:00 - 12:30 (9:00: 3600 * 9 ≙ 32400 / 12:30 ≙ 3,5h = 12600)
 *    NPWriteL 0x435,32400 0 86400 12600
 *    RESULT = {"NPWriteL":{"Address":1077,"Data":[32400,0,86400,12600]}}
 *
 *********************************************************************************************/

#define D_PRFX_NEOPOOL "NP"
#define D_CMND_NP_RESULT "Result"
#define D_CMND_NP_READ "Read"
#define D_CMND_NP_READL "ReadL"
#define D_CMND_NP_WRITE "Write"
#define D_CMND_NP_WRITEL "WriteL"
#define D_CMND_NP_BIT "Bit"
#define D_CMND_NP_BITL "BitL"
#define D_CMND_NP_FILTRATION "Filtration"
#define D_CMND_NP_FILTRATIONMODE "Filtrationmode"
#define D_CMND_NP_FILTRATIONSPEED "Filtrationspeed"
#define D_CMND_NP_TIME "Time"
#define D_CMND_NP_LIGHT "Light"
#define D_CMND_NP_PHMIN "pHMin"
#define D_CMND_NP_PHMAX "pHMax"
#define D_CMND_NP_PH "pH"
#define D_CMND_NP_REDOX "Redox"
#define D_CMND_NP_HYDROLYSIS "Hydrolysis"
#define D_CMND_NP_IONIZATION "Ionization"
#define D_CMND_NP_CHLORINE "Chlorine"
#define D_CMND_NP_CONTROL "Control"
#define D_CMND_NP_SAVE "Save"
#define D_CMND_NP_EXEC "Exec"
#define D_CMND_NP_ESCAPE "Escape"
#define D_CMND_NP_ONERROR "OnError"
#define D_CMND_NP_PHRES "PHRes"
#define D_CMND_NP_CLRES "CLRes"
#define D_CMND_NP_IONRES "IONRes"
#ifdef NEOPOOL_EMULATE_GPERH
  #define D_CMND_NP_GPERH "gPerh"
#endif

const char kNPCommands[] PROGMEM =  D_PRFX_NEOPOOL "|"  // Prefix
  D_CMND_NP_RESULT "|"
  D_CMND_NP_READ  "|"
  D_CMND_NP_READL  "|"
  D_CMND_NP_WRITE  "|"
  D_CMND_NP_WRITEL  "|"
  D_CMND_NP_BIT "|"
  D_CMND_NP_BITL "|"
  D_CMND_NP_FILTRATION "|"
  D_CMND_NP_FILTRATIONMODE "|"
  D_CMND_NP_FILTRATIONSPEED "|"
  D_CMND_NP_TIME "|"
  D_CMND_NP_LIGHT "|"
  D_CMND_NP_PHMIN "|"
  D_CMND_NP_PHMAX "|"
  D_CMND_NP_PH "|"
  D_CMND_NP_REDOX "|"
  D_CMND_NP_HYDROLYSIS "|"
  D_CMND_NP_IONIZATION "|"
  D_CMND_NP_CHLORINE "|"
  D_CMND_NP_CONTROL "|"
  D_CMND_NP_SAVE "|"
  D_CMND_NP_EXEC "|"
  D_CMND_NP_ESCAPE "|"
  D_CMND_NP_ONERROR "|"
  D_CMND_NP_PHRES "|"
  D_CMND_NP_CLRES "|"
  D_CMND_NP_IONRES
#ifdef NEOPOOL_EMULATE_GPERH
   "|" D_CMND_NP_GPERH
#endif
  ;

void (* const NPCommand[])(void) PROGMEM = {
  &CmndNeopoolResult,
  &CmndNeopoolReadReg,
  &CmndNeopoolReadReg,
  &CmndNeopoolWriteReg,
  &CmndNeopoolWriteReg,
  &CmndNeopoolBit,
  &CmndNeopoolBit,
  &CmndNeopoolFiltration,
  &CmndNeopoolFiltrationMode,
  &CmndNeopoolFiltrationSpeed,
  &CmndNeopoolTime,
  &CmndNeopoolLight,
  &CmndNeopoolpHMin,
  &CmndNeopoolpHMax,
  &CmndNeopoolpHMax,
  &CmndNeopoolRedox,
  &CmndNeopoolHydrolysis,
  &CmndNeopoolIonization,
  &CmndNeopoolChlorine,
  &CmndNeopoolControl,
  &CmndNeopoolSave,
  &CmndNeopoolExec,
  &CmndNeopoolEscape,
  &CmndNeopoolOnError,
  &CmndNeopoolPHRes,
  &CmndNeopoolCLRes,
  &CmndNeopoolIONRes
#ifdef NEOPOOL_EMULATE_GPERH
  ,&CmndNeopoolgPerh
#endif
};



/*********************************************************************************************/

void NeoPoolPoll(void)              // Poll modbus register
{
  if (!neopool_poll) {
    return;
  };

  if (neopoll_cmd_delay) {
    neopoll_cmd_delay--;
    if (0 == neopoll_cmd_delay && nullptr != neopoll_cmd) {
      void (* do_cmd)(void) = neopoll_cmd;
      neopoll_cmd = nullptr;
      do_cmd();
    }
  }

  bool data_ready = NeoPoolModbus->ReceiveReady();

  if (data_ready && nullptr != NeoPoolReg[neopool_read_state].data) {
    uint8_t *buffer = (uint8_t *)malloc(5+(NeoPoolReg[neopool_read_state].cnt)*2);

    if (nullptr != buffer) {
      uint8_t error = NeoPoolModbus->ReceiveBuffer(buffer, NeoPoolReg[neopool_read_state].cnt);  // cnt x 16bit register

      if (0 == error) {
        neopool_failed_count = 0;
        neopool_error = false;
        for (uint32_t i = 0; i < NeoPoolReg[neopool_read_state].cnt; i++) {
          NeoPoolReg[neopool_read_state].data[i] = (buffer[i*2+3] << 8) | buffer[i*2+4];
        }
      }
#ifdef DEBUG_TASMOTA_SENSOR
      else {
        AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("NEO: modbus receive error %d"), error);
      }
#endif  // DEBUG_TASMOTA_SENSOR
      free(buffer);
    }
#ifdef DEBUG_TASMOTA_SENSOR
    else {
        AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("NEO: modbus block 0x%04X - 0x%04X skipped"), NeoPoolReg[neopool_read_state].addr, NeoPoolReg[neopool_read_state].addr+NeoPoolReg[neopool_read_state].cnt);
    }
#endif  // DEBUG_TASMOTA_SENSOR

    ++neopool_read_state %= nitems(NeoPoolReg);
  }

  if (nullptr != NeoPoolReg[neopool_read_state].data) {
    if (0 == neopool_send_retry || data_ready) {
      neopool_send_retry = SENSOR_MAX_MISS;   // controller sometimes takes long time to answer
#ifdef DEBUG_TASMOTA_SENSOR
        AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("NEO: modbus send(%d, %d, 0x%04X, %d)"), NEOPOOL_MODBUS_ADDRESS, NEOPOOL_READ_REGISTER, NeoPoolReg[neopool_read_state].addr, NeoPoolReg[neopool_read_state].cnt);
#endif  // DEBUG_TASMOTA_SENSOR
        NeoPoolModbus->Send(NEOPOOL_MODBUS_ADDRESS, NEOPOOL_READ_REGISTER, NeoPoolReg[neopool_read_state].addr, NeoPoolReg[neopool_read_state].cnt);
    } else {
      if (1 == neopool_send_retry) {
        neopool_failed_count++;
        if (neopool_failed_count > 2) {
          neopool_failed_count = 0;
          neopool_error = true;
          NeoPoolInitData();
        }
      }
      neopool_send_retry--;
    }
  }
}



/*********************************************************************************************/

void NeoPoolInit(void) {
  neopool_active = false;
  if (PinUsed(GPIO_NEOPOOL_RX) && PinUsed(GPIO_NEOPOOL_TX)) {
    NeoPoolModbus = new TasmotaModbus(Pin(GPIO_NEOPOOL_RX), Pin(GPIO_NEOPOOL_TX));
    uint8_t result = NeoPoolModbus->Begin(NEOPOOL_MODBUS_SPEED);
    if (result) {
      if (2 == result) {
          ClaimSerial();
      }
      if (NeoPoolInitData()) {  // Claims heap space
        neopool_active = true;
      }
    }
  }
}


bool NeoPoolInitData(void)
{
  bool result = false;

  neopool_error = true;
  neopool_power_module_version = 0;
  memset(neopool_power_module_nodeid, 0, sizeof(neopool_power_module_nodeid));
  for (uint32_t i = 0; i < nitems(NeoPoolReg); i++) {
    if (nullptr == NeoPoolReg[i].data) {
      NeoPoolReg[i].data = (uint16_t *)malloc(sizeof(uint16_t)*NeoPoolReg[i].cnt);
      if (nullptr != NeoPoolReg[i].data) {
        memset(NeoPoolReg[i].data, 0, sizeof(uint16_t)*NeoPoolReg[i].cnt);
#ifdef DEBUG_TASMOTA_SENSOR
        AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("NEO: Init - addr 0x%04x cnt %d data %p"), NeoPoolReg[i].addr, NeoPoolReg[i].cnt, NeoPoolReg[i].data);
#endif  // DEBUG_TASMOTA_SENSOR
        result = true;
      }
#ifdef DEBUG_TASMOTA_SENSOR
      else {
        AddLog(LOG_LEVEL_DEBUG, PSTR("NEO: Init - out of memory"));
      }
#endif  // DEBUG_TASMOTA_SENSOR
    }
  }
  return result;
}



/*********************************************************************************************/

#ifdef DEBUG_TASMOTA_SENSOR
void NeoPoolLogRW(const char *name, uint16_t addr, uint16_t *data, uint16_t cnt)
{
  char *log_data = (char *)malloc(cnt*7+1);
  *log_data = 0;
  for (uint32_t i = 0; i < cnt; i++) {
    char h[8];
    snprintf_P(h, sizeof(h), PSTR("%s0x%04X"), i ? PSTR(",") : PSTR(""), data[i]);
    strncat(log_data, h, cnt*7+1);
  }
  AddLog(LOG_LEVEL_DEBUG, PSTR("NEO: %s(0x%04X, %d) = [%s]"), name, addr, cnt, log_data);
  free(log_data);
}
#endif  // DEBUG_TASMOTA_SENSOR


void NeoPool250msSetStatus(bool status)
{
  neopool_poll = status;

  if (!status) {
    // clear rec buffer from possible prev periodical communication
    uint32_t timeoutMS = millis() + 100 * NEOPOOL_READ_TIMEOUT; // Max delay before we timeout
    while (NeoPoolModbus->available() && millis() < timeoutMS) {
      NeoPoolModbus->read();
      SleepDelay(0);
    }

  }
}


uint8_t NeoPoolReadRegisterData(uint16_t addr, uint16_t *data, uint16_t cnt)
{
  bool data_ready;
  uint32_t timeoutMS;
  uint16_t *origin = data;

  NeoPool250msSetStatus(false);
  *data = 0;

  NeoPoolModbus->Send(NEOPOOL_MODBUS_ADDRESS, NEOPOOL_READ_REGISTER, addr, cnt);
  timeoutMS = millis() + cnt * NEOPOOL_READ_TIMEOUT; // Max delay before we timeout
  while (!(data_ready = NeoPoolModbus->ReceiveReady()) && millis() < timeoutMS) { delay(1); }
  if (data_ready) {
    uint8_t *buffer = (uint8_t*)malloc(5+cnt*2);
    if (buffer != nullptr) {
      uint8_t error = NeoPoolModbus->ReceiveBuffer(buffer, cnt);
      if (error) {
#ifdef DEBUG_TASMOTA_SENSOR
        AddLog(LOG_LEVEL_DEBUG, PSTR("NEO: addr 0x%04X read data error %d"), addr, error);
#endif  // DEBUG_TASMOTA_SENSOR
        NeoPool250msSetStatus(true);
        free(buffer);
        return NEOPOOL_MODBUS_ERROR_RW_DATA;
      }
      for(uint64_t i = 0; i < cnt; i++) {
        *data++ = (buffer[i*2+3] << 8) | buffer[i*2+4];
      }
      NeoPool250msSetStatus(true);
      delay(2);
      free(buffer);
#ifdef DEBUG_TASMOTA_SENSOR
      NeoPoolLogRW("NeoPoolReadRegister", addr, origin, cnt);
#endif  // DEBUG_TASMOTA_SENSOR
      return NEOPOOL_MODBUS_OK;
    }
#ifdef DEBUG_TASMOTA_SENSOR
    AddLog(LOG_LEVEL_DEBUG, PSTR("NEO: addr 0x%04X read out of memory"), addr);
#endif  // DEBUG_TASMOTA_SENSOR
    return NEOPOOL_MODBUS_ERROR_OUT_OF_MEM;
  }
#ifdef DEBUG_TASMOTA_SENSOR
  AddLog(LOG_LEVEL_DEBUG, PSTR("NEO: addr 0x%04X read data timeout"), addr);
#endif  // DEBUG_TASMOTA_SENSOR
  NeoPool250msSetStatus(true);
  return NEOPOOL_MODBUS_ERROR_TIMEOUT;
}


uint8_t NeoPoolWriteRegisterData(uint16_t addr, uint16_t *data, uint16_t cnt)
{
  uint8_t *frame;
  uint32_t numbytes;
  bool data_ready;
  uint32_t timeoutMS;

#ifdef DEBUG_TASMOTA_SENSOR
  NeoPoolLogRW("NeoPoolWriteRegister", addr, data, cnt);
#endif  // DEBUG_TASMOTA_SENSOR
  NeoPool250msSetStatus(false);
  numbytes = 7+cnt*2;
  frame = (uint8_t*)malloc(numbytes+2);
  if (nullptr == frame) {
#ifdef DEBUG_TASMOTA_SENSOR
    AddLog(LOG_LEVEL_DEBUG, PSTR("NEO: addr 0x%04X write out of memory"), addr);
#endif  // DEBUG_TASMOTA_SENSOR
    return NEOPOOL_MODBUS_ERROR_OUT_OF_MEM;
  }

  // Function 16 (10hex) Write Multiple Registers
  // Header
  frame[0] = NEOPOOL_MODBUS_ADDRESS;
  frame[1] = NEOPOOL_WRITE_REGISTER;
  frame[2] = (uint8_t)(addr >> 8);          // addr MSB
  frame[3] = (uint8_t)(addr);               // addr LSB
  frame[4] = (uint8_t)(cnt >> 8);           // register quantity MSB
  frame[5] = (uint8_t)(cnt);                // register quantity LSB
  frame[6] = (uint8_t)(cnt * 2);            // byte count
  for (uint32_t i = 0; i < cnt; i++) {
#ifdef NEOPOOL_EMULATE_GPERH
    frame[7+i*2] = (uint8_t)(NeoPoolEmulateSetData(addr + i, data[i]) >> 8); // data MSB
    frame[8+i*2] = (uint8_t)(NeoPoolEmulateSetData(addr + i, data[i]));      // data LSB
#else
    frame[7+i*2] = (uint8_t)(data[i] >> 8); // data MSB
    frame[8+i*2] = (uint8_t)(data[i]);      // data LSB
#endif
  }
  uint16_t crc = NeoPoolModbus->CalculateCRC(frame, numbytes);
  frame[numbytes] = (uint8_t)(crc);
  frame[numbytes+1] = (uint8_t)(crc >> 8);

  NeoPoolModbus->flush();
  NeoPoolModbus->write(frame, numbytes+2);

  timeoutMS = millis() + 1 * NEOPOOL_READ_TIMEOUT; // Max delay before we timeout
  while (!(data_ready = NeoPoolModbus->ReceiveReady()) && millis() < timeoutMS) { delay(1); }
  free(frame);
  if (data_ready) {
    uint8_t buffer[9];
    uint8_t error = NeoPoolModbus->ReceiveBuffer(buffer, 1);
    if (0 != error && 9 != error) { // ReceiveBuffer can't handle 0x10 code result
#ifdef DEBUG_TASMOTA_SENSOR
      AddLog(LOG_LEVEL_DEBUG, PSTR("NEO: addr 0x%04X write data response error %d"), addr, error);
#endif  // DEBUG_TASMOTA_SENSOR
      NeoPool250msSetStatus(true);
      return NEOPOOL_MODBUS_ERROR_RW_DATA;
    }
    if (9 == error) {
      // clear buffer before we leave
      while (NeoPoolModbus->available()) {
        NeoPoolModbus->read();
      }
    }
    NeoPool250msSetStatus(true);
    delay(2);
    if (MBF_SAVE_TO_EEPROM == addr) {
        // EEPROM write can take some time, wait until device is ready
        timeoutMS = millis() + 1000;  // Max delay for save eeprom cmnd
        uint16_t tmp;
        while (NEOPOOL_MODBUS_OK != NeoPoolReadRegister(MBF_NOTIFICATION, &tmp, 1) && millis() < timeoutMS);
    }
    return NEOPOOL_MODBUS_OK;
  }
#ifdef DEBUG_TASMOTA_SENSOR
  AddLog(LOG_LEVEL_DEBUG, PSTR("NEO: addr 0x%04X write data response timeout"), addr);
#endif  // DEBUG_TASMOTA_SENSOR
  NeoPool250msSetStatus(true);
  return NEOPOOL_MODBUS_ERROR_TIMEOUT;
}


uint8_t NeoPoolReadRegisterRaw(uint16_t addr, uint16_t *data, uint16_t cnt)
{
  uint8_t repeat = neopool_repeat_on_error;
  uint8_t result;
  do {
    result = NeoPoolReadRegisterData(addr, data, cnt);
    SleepDelay(0);
  } while(repeat-- > 0 && NEOPOOL_MODBUS_OK != result);
  return result;
}

uint8_t NeoPoolReadRegister(uint16_t addr, uint16_t *data, uint16_t cnt)
{
  uint8_t result = NeoPoolReadRegisterRaw(addr, data, cnt);
#ifdef NEOPOOL_EMULATE_GPERH
  if (NEOPOOL_MODBUS_OK == result) {
    for (uint32_t i = 0; i < cnt; i++) {
      data[i] = NeoPoolEmulateGetData(addr + i, data[i]);
    }
  }
#endif
  return result;
}


uint8_t NeoPoolWriteRegister(uint16_t addr, uint16_t *data, uint16_t cnt)
{
  uint8_t repeat = neopool_repeat_on_error;
  uint8_t result;

  do {
    result = NeoPoolWriteRegisterData(addr, data, cnt);
    SleepDelay(0);
  } while(repeat-- > 0 && NEOPOOL_MODBUS_OK != result);
  return result;
}


uint8_t NeoPoolWriteRegisterWord(uint16_t addr, uint16_t data)
{
  return NeoPoolWriteRegister(addr, &data, 1);
}


uint16_t NeoPoolGetDataTO(uint16_t addr, int32_t timeout)
{
  uint16_t data;
  bool datavalid = false;
  uint16_t i;

  for (i = 0; !datavalid && i < nitems(NeoPoolReg); i++) {
    if (nullptr != NeoPoolReg[i].data && addr >= NeoPoolReg[i].addr && addr < NeoPoolReg[i].addr+NeoPoolReg[i].cnt) {
      data = NeoPoolReg[i].data[addr - NeoPoolReg[i].addr];
      datavalid = true;
    }
  }

  if (!datavalid) {
    if (timeout < 0) {
      timeout = NEOPOOL_DATA_TIMEOUT;
    }
    // search in temportary data array
    for (i = 0; !datavalid && i < NeoPoolDataCount; i++) {
      if (nullptr != NeoPoolData && addr == NeoPoolData[i].addr) {
        if (millis() < NeoPoolData[i].ts) {
          data = NeoPoolData[i].data;
        } else {
          NeoPoolReadRegisterRaw(addr, &data, 1);
          NeoPoolData[i].data = data;
          NeoPoolData[i].ts = millis() + timeout;
        }
        datavalid = true;
      }
    }

    if (!datavalid) {
      NeoPoolReadRegisterRaw(addr, &data, 1);
      datavalid = true;
      if (nullptr == NeoPoolData) {
        NeoPoolDataCount = 1;
        NeoPoolData = (TNeoPoolData*)malloc(sizeof(TNeoPoolData) * NeoPoolDataCount);
      } else {
        NeoPoolDataCount++;
        NeoPoolData = (TNeoPoolData*)realloc(NeoPoolData, sizeof(TNeoPoolData) * NeoPoolDataCount);
      }
      if (nullptr != NeoPoolData) {
        NeoPoolData[NeoPoolDataCount-1].addr = addr;
        NeoPoolData[NeoPoolDataCount-1].data = data;
        NeoPoolData[NeoPoolDataCount-1].ts = millis() + timeout;
      } else {
        NeoPoolDataCount = 0;
      }
    }
  }
#ifdef NEOPOOL_EMULATE_GPERH
  return NeoPoolEmulateGetData(addr, data);
#else
  return data;
#endif
}


uint16_t NeoPoolGetData(uint16_t addr)
{
  return NeoPoolGetDataTO(addr, -1);
}


uint32_t NeoPoolGetDataLong(uint16_t addr)
{
  return ((uint32_t)NeoPoolGetData(addr) + ((uint32_t)NeoPoolGetData(addr+1) << 16));
}


#ifdef NEOPOOL_EMULATE_GPERH
uint16_t NeoPoolEmulateGetData(uint16_t addr, uint16_t data)
{
  if (neopool_system_gperh) {
    // emulate g/h device
    switch(addr) {
      case MBF_PAR_UICFG_MACH_VISUAL_STYLE:
          data &= ~((MBMSK_VS_FORCE_UNITS_GRH | MBMSK_VS_FORCE_UNITS_PERCENTAGE));
          data |= MBMSK_VS_FORCE_UNITS_GRH;
          break;
      case MBF_HIDRO_CURRENT:
      case MBF_PAR_HIDRO:
      case MBF_PAR_HIDRO_NOM:
          // [0..1000] -> [0(NEOPOOL_EMULATE_GPERH * 10)
          data = data * NEOPOOL_EMULATE_GPERH / 100;
          break;
    }
  }
  return data;
}

uint16_t NeoPoolEmulateSetData(uint16_t addr, uint16_t data)
{
  if (neopool_system_gperh) {
    // emulate g/h device
    switch(addr) {
      case MBF_PAR_HIDRO:
          // [0..(NEOPOOL_EMULATE_GPERH * 10)] -> [0..1000]
          data = (data * 100) / NEOPOOL_EMULATE_GPERH;
          break;
    }
  }
  return data;
}
#endif

uint32_t NeoPoolGetFiltrationSpeed()
{
  switch((NeoPoolGetData(MBF_RELAY_STATE) & MBMSK_RELAY_FILTSPEED) >> MBSHFT_RELAY_FILTSPEED) {
    case 1:
      return 1;
    case 2:
      return 2;
    case 4:
      return 3;
  }
  switch((NeoPoolGetData(MBF_PAR_FILTRATION_CONF) & MBMSK_PAR_FILTRATION_CONF_DEF_SPEED) >> MBSHFT_PAR_FILTRATION_CONF_DEF_SPEED) {
    case MBV_PAR_FILTRATION_SPEED_SLOW:
      return 1;
    case MBV_PAR_FILTRATION_SPEED_MEDIUM:
      return 2;
    case MBV_PAR_FILTRATION_SPEED_FAST:
      return 3;
  }
  return 0;
}


bool NeoPoolIsHydrolysis(void)
{
  return (((NeoPoolGetData(MBF_PAR_MODEL) & MBMSK_MODEL_HIDRO)) ||
          (NeoPoolGetData(MBF_HIDRO_STATUS) & (MBMSK_HIDRO_STATUS_CTRL_ACTIVE | MBMSK_HIDRO_STATUS_CTRL_ACTIVE)));
}

bool NeoPoolIsHydrolysisInPercent(void)
{
  return !(MBMSK_VS_FORCE_UNITS_GRH == (NeoPoolGetData(MBF_PAR_UICFG_MACH_VISUAL_STYLE) & (MBMSK_VS_FORCE_UNITS_GRH | MBMSK_VS_FORCE_UNITS_PERCENTAGE)));
}

bool NeoPoolIspHModule(void)
{
  return (NeoPoolGetData(MBF_PH_STATUS) & MBMSK_PH_STATUS_MEASURE_ACTIVE);
}


bool NeoPoolIsRedox(void)
{
  return (NeoPoolGetData(MBF_RX_STATUS) & MBMSK_RX_STATUS_MEASURE_ACTIVE);
}


bool NeoPoolIsChlorine(void)
{
  return (NeoPoolGetData(MBF_CL_STATUS) & MBMSK_CL_STATUS_MEASURE_ACTIVE);
}


bool NeoPoolIsConductivity(void)
{
  return (NeoPoolGetData(MBF_CD_STATUS) & MBMSK_CD_STATUS_MEASURE_ACTIVE);
}


bool NeoPoolIsIonization(void)
{
  return (NeoPoolGetData(MBF_PAR_MODEL) & MBMSK_MODEL_ION);
}


/*********************************************************************************************/
void NeoPoolAppendModules(void)
{
  ResponseAppend_P(PSTR("\""  D_NEOPOOL_JSON_MODULES  "\":"));
  ResponseAppend_P(PSTR("{\""  D_JSON_PH  "\":%d"), NeoPoolIspHModule());
  ResponseAppend_P(PSTR(",\""  D_NEOPOOL_JSON_REDOX  "\":%d"), NeoPoolIsRedox());
  ResponseAppend_P(PSTR(",\""  D_NEOPOOL_JSON_HYDROLYSIS  "\":%d"), NeoPoolIsHydrolysis());
  ResponseAppend_P(PSTR(",\""  D_NEOPOOL_JSON_CHLORINE  "\":%d"), NeoPoolIsChlorine());
  ResponseAppend_P(PSTR(",\""  D_NEOPOOL_JSON_CONDUCTIVITY  "\":%d"), NeoPoolIsConductivity());
  ResponseAppend_P(PSTR(",\""  D_NEOPOOL_JSON_IONIZATION  "\":%d"), NeoPoolIsIonization());
  ResponseJsonEnd();
}


void NeoPoolShow(bool json)
{
  char neopool_type[60];
  char stemp[200];
  float fvalue;

  if (neopool_error) {
    return;
  }

  GetTextIndexed(neopool_type, sizeof(neopool_type), NeoPoolGetData(MBF_PAR_UICFG_MACHINE), kNeoPoolMachineNames);
  *stemp = 0;

  if (json) {
    ResponseAppend_P(PSTR(",\""  D_NEOPOOL_NAME  "\":{"));

    // Time
    ResponseAppend_P(PSTR("\""  D_JSON_TIME  "\":\"%s\","), GetDT(NeoPoolGetDataLong(MBF_PAR_TIME_LOW)).c_str());

    // Type
    ResponseAppend_P(PSTR("\""  D_JSON_TYPE  "\":\"%s\","), neopool_type);

    // Modules
    NeoPoolAppendModules();

    // Temperature
    if (NeoPoolGetData(MBF_PAR_TEMPERATURE_ACTIVE)) {
      fvalue = ConvertTemp((float)NeoPoolGetData(MBF_MEASURE_TEMPERATURE)/10);
      ResponseAppend_P(PSTR(",\""  D_TEMPERATURE  "\":%*_f"), Settings->flag2.temperature_resolution, &fvalue);
    }

    // Voltage
    {
      float f5volt = (float)NeoPoolGetData(MBF_VOLT_5)/620.69;
      float f12volt = (float)NeoPoolGetData(MBF_VOLT_12)/1000;
      float f24_36volt = (float)NeoPoolGetData(MBF_VOLT_24_36)/1000;
      float f420mA = (float)NeoPoolGetData(MBF_AMP_4_20_MICRO)/100;
      ResponseAppend_P(PSTR(",\""  D_NEOPOOL_JSON_POWERUNIT  "\":{"));
      if (neopool_power_module_version ||
          NEOPOOL_MODBUS_OK == NeoPoolReadRegister(MBF_POWER_MODULE_VERSION, &neopool_power_module_version, 1)) {
        ResponseAppend_P(PSTR("\""  D_JSON_VERSION  "\":\"V%d.%d\","),
          (neopool_power_module_version >> 8),
          neopool_power_module_version & 0xff
        );
      }
      if (neopool_power_module_nodeid[0] ||
          NEOPOOL_MODBUS_OK == NeoPoolReadRegister(MBF_POWER_MODULE_NODEID, neopool_power_module_nodeid, nitems(neopool_power_module_nodeid))) {
        if (Settings->flag6.neopool_outputsensitive) {
          ResponseAppend_P(PSTR("\""  D_NEOPOOL_JSON_NODE_ID  "\":\"%04X %04X %04X %04X %04X %04X\","),
            neopool_power_module_nodeid[0],
            neopool_power_module_nodeid[1],
            neopool_power_module_nodeid[2],
            neopool_power_module_nodeid[3],
            neopool_power_module_nodeid[4],
            neopool_power_module_nodeid[5]
          );
        }
        else {
          ResponseAppend_P(PSTR("\""  D_NEOPOOL_JSON_NODE_ID  "\":\"XXXX XXXX XXXX XXXX XXXX %04X\","),
            neopool_power_module_nodeid[5]
          );
        }
      }
      ResponseAppend_P(PSTR("\"5V\":%*_f,\"12V\":%*_f,\"24-30V\":%*_f,\"4-20mA\":%*_f}"),
        Settings->flag2.voltage_resolution, &f5volt,
        Settings->flag2.voltage_resolution, &f12volt,
        Settings->flag2.voltage_resolution, &f24_36volt,
        Settings->flag2.current_resolution, &f420mA);
    }

    // pH
    if (NeoPoolIspHModule()) {
      fvalue = (float)NeoPoolGetData(MBF_MEASURE_PH)/100;
      ResponseAppend_P(PSTR(",\""  D_PH  "\":{\""  D_JSON_DATA  "\":"  NEOPOOL_FMT_PH), neopool_resolution.ph, &fvalue);

      // S1
      float fphmin = (float)NeoPoolGetData(MBF_PAR_PH2)/100;
      ResponseAppend_P(PSTR(",\""  D_NEOPOOL_JSON_MIN  "\":"  NEOPOOL_FMT_PH), neopool_resolution.ph, &fphmin);
      float fphmax = (float)NeoPoolGetData(MBF_PAR_PH1)/100;
      ResponseAppend_P(PSTR(",\""  D_NEOPOOL_JSON_MAX  "\":"  NEOPOOL_FMT_PH), neopool_resolution.ph, &fphmax);

      // S2
      ResponseAppend_P(PSTR(",\""  D_NEOPOOL_JSON_STATE  "\":%d"), (NeoPoolGetData(MBF_PH_STATUS) & MBMSK_PH_STATUS_ALARM));

      // S3
      int phpump = 0;
      if (NeoPoolGetData(MBF_PH_STATUS) & MBMSK_PH_STATUS_CTRL_ACTIVE) {
        if (NeoPoolGetData(MBF_PH_STATUS) & (MBMSK_PH_STATUS_ACID_PUMP_ACTIVE | MBMSK_PH_STATUS_BASE_PUMP_ACTIVE)) {
          phpump = 1;
        } else {
          phpump = 2;
        }
      }
      ResponseAppend_P(PSTR(",\""  D_NEOPOOL_JSON_PHPUMP  "\":%d"), phpump);

      // S4
      ResponseAppend_P(PSTR(",\""  D_NEOPOOL_JSON_FLOW1  "\":%d"), (NeoPoolGetData(MBF_PH_STATUS) & MBMSK_PH_STATUS_CTRL_BY_FL) ? 0 : 1);

      ResponseAppend_P(PSTR(",\""  D_NEOPOOL_JSON_TANK  "\":%d"), (MBV_PH_ACID_BASE_ALARM6 == (NeoPoolGetData(MBF_PH_STATUS) & MBMSK_PH_STATUS_ALARM)) ? 0 : 1);

      ResponseJsonEnd();
    }

    // Redox
    if (NeoPoolIsRedox()) {
      ResponseAppend_P(PSTR(",\""  D_NEOPOOL_JSON_REDOX  "\":{"));
      ResponseAppend_P(PSTR("\""  D_JSON_DATA  "\":"  NEOPOOL_FMT_RX), NeoPoolGetData(MBF_MEASURE_RX));
      ResponseAppend_P(PSTR(",\""  D_NEOPOOL_JSON_SETPOINT  "\":"  NEOPOOL_FMT_RX), NeoPoolGetData(MBF_PAR_RX1));
      ResponseJsonEnd();
    }

    // Chlorine
    if (NeoPoolIsChlorine()) {
      ResponseAppend_P(PSTR(",\""  D_NEOPOOL_JSON_CHLORINE  "\":{"));
      fvalue = (float)NeoPoolGetData(MBF_MEASURE_CL)/100;
      ResponseAppend_P(PSTR("\""  D_JSON_DATA  "\":"  NEOPOOL_FMT_CL), neopool_resolution.cl, &fvalue);
      fvalue = (float)NeoPoolGetData(MBF_PAR_CL1)/100;
      ResponseAppend_P(PSTR(",\""  D_NEOPOOL_JSON_SETPOINT  "\":"  NEOPOOL_FMT_CL), neopool_resolution.cl, &fvalue);
      ResponseJsonEnd();
    }

    // Conductivity
    if (NeoPoolIsConductivity()) {
      ResponseAppend_P(PSTR(",\""  D_NEOPOOL_CONDUCTIVITY  "\":" NEOPOOL_FMT_CD), NeoPoolGetData(MBF_MEASURE_CONDUCTIVITY));
    }

    // Ionization
    if (NeoPoolIsIonization()) {
      ResponseAppend_P(PSTR(",\""  D_NEOPOOL_JSON_IONIZATION  "\":{"));
      fvalue = (float)NeoPoolGetData(MBF_ION_CURRENT);
      ResponseAppend_P(PSTR("\""  D_JSON_DATA  "\":"  NEOPOOL_FMT_ION), neopool_resolution.ion, &fvalue);
      fvalue = (float)NeoPoolGetData(MBF_PAR_ION);
      ResponseAppend_P(PSTR(",\""  D_NEOPOOL_JSON_SETPOINT  "\":"  NEOPOOL_FMT_ION), neopool_resolution.ion, &fvalue);
      fvalue = (float)NeoPoolGetData(MBF_PAR_ION_NOM);
      ResponseAppend_P(PSTR(",\""  D_NEOPOOL_JSON_MAX  "\":"  NEOPOOL_FMT_ION), neopool_resolution.ion, &fvalue);
      ResponseJsonEnd();
    }

    // Hydrolysis
    if (NeoPoolIsHydrolysis()) {
      int decimals = NeoPoolIsHydrolysisInPercent() ? 0 : 1;
      ResponseAppend_P(PSTR(",\""  D_NEOPOOL_JSON_HYDROLYSIS  "\":{"));
      uint16_t data = NeoPoolGetData(MBF_HIDRO_CURRENT);
      fvalue = (float)data / 10;
      ResponseAppend_P(PSTR( "\""  D_JSON_DATA  "\":"  NEOPOOL_FMT_HIDRO), decimals, &fvalue);
      ResponseAppend_P(PSTR(",\""  D_NEOPOOL_JSON_UNIT  "\":\"%s\""), NeoPoolIsHydrolysisInPercent() ? PSTR(D_NEOPOOL_UNIT_PERCENT) : PSTR(D_NEOPOOL_UNIT_GPERH));
      uint16_t setpoint = NeoPoolGetData(MBF_PAR_HIDRO);
      fvalue = (float)setpoint / 10;
      ResponseAppend_P(PSTR(",\""  D_NEOPOOL_JSON_SETPOINT  "\":"  NEOPOOL_FMT_HIDRO), decimals, &fvalue);
      uint16_t max = NeoPoolGetData(MBF_PAR_HIDRO_NOM);
      fvalue = (float)max / 10;
      ResponseAppend_P(PSTR(",\""  D_NEOPOOL_JSON_MAX  "\":"  NEOPOOL_FMT_HIDRO), decimals, &fvalue);

      ResponseAppend_P(PSTR(",\""  D_NEOPOOL_JSON_PERCENT  "\":{"));
      ResponseAppend_P(PSTR( "\""  D_JSON_DATA  "\":%d"), data * 100 / max);
      ResponseAppend_P(PSTR(",\""  D_NEOPOOL_JSON_SETPOINT  "\":%d"), setpoint * 100 / max);
      ResponseJsonEnd();

      ResponseAppend_P(PSTR(",\""  D_NEOPOOL_JSON_CELL_RUNTIME  "\":{"));
      ResponseAppend_P(PSTR( "\""  D_NEOPOOL_JSON_CELL_RUNTIME_TOTAL  "\":\"%s\""), GetDuration(NeoPoolGetDataLong(MBF_CELL_RUNTIME_LOW)).c_str());
      ResponseAppend_P(PSTR(",\""  D_NEOPOOL_JSON_CELL_RUNTIME_PART  "\":\"%s\""), GetDuration(NeoPoolGetDataLong(MBF_CELL_RUNTIME_PART_LOW)).c_str());
      ResponseAppend_P(PSTR(",\""  D_NEOPOOL_JSON_CELL_RUNTIME_POL1  "\":\"%s\""), GetDuration(NeoPoolGetDataLong(MBF_CELL_RUNTIME_POLA_LOW)).c_str());
      ResponseAppend_P(PSTR(",\""  D_NEOPOOL_JSON_CELL_RUNTIME_POL2  "\":\"%s\""), GetDuration(NeoPoolGetDataLong(MBF_CELL_RUNTIME_POLB_LOW)).c_str());
      ResponseAppend_P(PSTR(",\""  D_NEOPOOL_JSON_CELL_RUNTIME_CHANGES  "\":%ld"), NeoPoolGetDataLong(MBF_CELL_RUNTIME_POL_CHANGES_LOW));
      ResponseJsonEnd();

      // S1
      const char *state = PSTR("");
      if (0 == (NeoPoolGetData(MBF_HIDRO_STATUS) & MBMSK_HIDRO_STATUS_MODULE_ACTIVE)) {
        state = PSTR(D_NEOPOOL_STATUS_OFF);
      } else if (0 == (NeoPoolGetData(MBF_HIDRO_STATUS) & MBMSK_HIDRO_STATUS_FL1)) {
        state = PSTR(D_NEOPOOL_STATUS_FLOW);
      } else if (NeoPoolGetData(MBF_HIDRO_STATUS) & MBMSK_HIDRO_STATUS_POL1) {
        state = PSTR(D_NEOPOOL_POLARIZATION "1");
      } else if (NeoPoolGetData(MBF_HIDRO_STATUS) & MBMSK_HIDRO_STATUS_POL2) {
        state = PSTR(D_NEOPOOL_POLARIZATION "2");
      } else {
        state = PSTR(D_NEOPOOL_STATUS_OFF);
      }
      ResponseAppend_P(PSTR(",\""  D_NEOPOOL_JSON_STATE  "\":\"%s\""), state);
      // S2
      ResponseAppend_P(PSTR(",\""  D_NEOPOOL_JSON_COVER  "\":%d"), (NeoPoolGetData(MBF_HIDRO_STATUS) & MBMSK_HIDRO_STATUS_COVER) ? 1 : 0 );
      // S3
      ResponseAppend_P(PSTR(",\""  D_NEOPOOL_JSON_SHOCK  "\":%d"), (NeoPoolGetData(MBF_HIDRO_STATUS) & MBMSK_HIDRO_STATUS_SHOCK_ENABLED) ? ((NeoPoolGetData(MBF_CELL_BOOST) & MBMSK_CELL_BOOST_REDOX_CTL) ? 1 : 2) : 0 );
      // S4
      ResponseAppend_P(PSTR(",\""  D_NEOPOOL_JSON_LOW  "\":%d"), (NeoPoolGetData(MBF_HIDRO_STATUS) & MBMSK_HIDRO_STATUS_LOW) ? 1 : 0 );

      ResponseJsonEnd();
    }

    // Filtration
    if (0 != NeoPoolGetData(MBF_PAR_FILT_GPIO)) {
      ResponseAppend_P(PSTR(",\""  D_NEOPOOL_JSON_FILTRATION  "\":"));
      ResponseAppend_P(PSTR("{\""  D_NEOPOOL_JSON_STATE  "\":%d"), (NeoPoolGetData(MBF_RELAY_STATE) >> (NeoPoolGetData(MBF_PAR_FILT_GPIO)-1)) & 1);
      uint16_t speed = NeoPoolGetFiltrationSpeed();
      if (speed) {
        ResponseAppend_P(PSTR(",\""  D_NEOPOOL_JSON_FILTRATION_SPEED  "\":%d"), (speed < 3) ? speed : 3);
      }
      ResponseAppend_P(PSTR(",\""  D_NEOPOOL_JSON_FILTRATION_MODE  "\":%d}"), NeoPoolGetData(MBF_PAR_FILT_MODE));
    }

    // Light
    if (0 != NeoPoolGetData(MBF_PAR_LIGHTING_GPIO)) {
      ResponseAppend_P(PSTR(",\""  D_JSON_LIGHT  "\":%d"), (NeoPoolGetData(MBF_RELAY_STATE) >> (NeoPoolGetData(MBF_PAR_LIGHTING_GPIO)-1)) & 1);
    }

    // Relays
    ResponseAppend_P(PSTR(",\""  D_NEOPOOL_JSON_RELAY  "\":{\""  D_NEOPOOL_JSON_STATE  "\":["));
    for(uint16_t i = 0; i < NEOPOOL_RELAY_MAX; i++) {
      ResponseAppend_P(PSTR("%s%d"), i ? PSTR(",") : PSTR(""), (NeoPoolGetData(MBF_RELAY_STATE) >> i) & 1);
    }
    ResponseAppend_P(PSTR("]"));
    ResponseAppend_P(PSTR(",\""  D_NEOPOOL_JSON_AUX  "\":["));
    for(uint16_t i = 3; i < NEOPOOL_RELAY_MAX; i++) {
      ResponseAppend_P(PSTR("%s%d"), i > 3 ? PSTR(",") : PSTR(""), (NeoPoolGetData(MBF_RELAY_STATE) >> i) & 1);
    }
    ResponseAppend_P(PSTR("]"));
    if (0 != NeoPoolGetData(MBF_PAR_PH_ACID_RELAY_GPIO)) {
      ResponseAppend_P(PSTR(",\""  D_NEOPOOL_JSON_RELAY_PH_ACID  "\":%d"), (NeoPoolGetData(MBF_RELAY_STATE) >> (NeoPoolGetData(MBF_PAR_PH_ACID_RELAY_GPIO)-1)) & 1);
    }
    if (0 != NeoPoolGetData(MBF_PAR_PH_BASE_RELAY_GPIO)) {
      ResponseAppend_P(PSTR(",\""  D_NEOPOOL_JSON_RELAY_PH_BASE  "\":%d"), (NeoPoolGetData(MBF_RELAY_STATE) >> (NeoPoolGetData(MBF_PAR_PH_BASE_RELAY_GPIO)-1)) & 1);
    }
    if (0 != NeoPoolGetData(MBF_PAR_RX_RELAY_GPIO)) {
      ResponseAppend_P(PSTR(",\""  D_NEOPOOL_JSON_RELAY_RX  "\":%d"), (NeoPoolGetData(MBF_RELAY_STATE) >> (NeoPoolGetData(MBF_PAR_RX_RELAY_GPIO)-1)) & 1);
    }
    if (0 != NeoPoolGetData(MBF_PAR_CL_RELAY_GPIO)) {
      ResponseAppend_P(PSTR(",\""  D_NEOPOOL_JSON_RELAY_CL  "\":%d"), (NeoPoolGetData(MBF_RELAY_STATE) >> (NeoPoolGetData(MBF_PAR_CL_RELAY_GPIO)-1)) & 1);
    }
    if (0 != NeoPoolGetData(MBF_PAR_CD_RELAY_GPIO)) {
      ResponseAppend_P(PSTR(",\""  D_NEOPOOL_JSON_RELAY_CD " \":%d"), (NeoPoolGetData(MBF_RELAY_STATE) >> NeoPoolGetData(MBF_PAR_CD_RELAY_GPIO)) & 1);
    }
    if (0 != NeoPoolGetData(MBF_PAR_HEATING_GPIO)) {
      ResponseAppend_P(PSTR(",\""  D_NEOPOOL_JSON_RELAY_HEATING " \":%d"), (NeoPoolGetData(MBF_RELAY_STATE) >> NeoPoolGetData(MBF_PAR_HEATING_GPIO)) & 1);
    }
    if (0 != NeoPoolGetData(MBF_PAR_UV_RELAY_GPIO)) {
      ResponseAppend_P(PSTR(",\""  D_NEOPOOL_JSON_RELAY_UV " \":%d"), (NeoPoolGetData(MBF_RELAY_STATE) >> NeoPoolGetData(MBF_PAR_UV_RELAY_GPIO)) & 1);
    }
    if (0 != NeoPoolGetData(MBF_PAR_FILTVALVE_GPIO)) {
      ResponseAppend_P(PSTR(",\""  D_NEOPOOL_JSON_RELAY_FILTVALVE " \":%d"), (NeoPoolGetData(MBF_RELAY_STATE) >> NeoPoolGetData(MBF_PAR_FILTVALVE_GPIO)) & 1);
    }

    ResponseJsonEndEnd();

#ifdef USE_WEBSERVER
  } else {
    char bg_color[10];
    snprintf_P(bg_color, sizeof(bg_color), PSTR("#%02x%02x%02x"),
      Settings->web_color[COL_BACKGROUND][0],  // R
      Settings->web_color[COL_BACKGROUND][1],  // G
      Settings->web_color[COL_BACKGROUND][2]   // B
      );

    // Time
    String nptime = GetDT(NeoPoolGetDataLong(MBF_PAR_TIME_LOW));
    WSContentSend_PD(HTTP_SNS_NEOPOOL_TIME, neopool_type, nptime.substring(0, nptime.length()-3).c_str());

    // Temperature
    if (NeoPoolGetData(MBF_PAR_TEMPERATURE_ACTIVE)) {
      fvalue = ConvertTemp((float)NeoPoolGetData(MBF_MEASURE_TEMPERATURE)/10);
      WSContentSend_PD(HTTP_SNS_F_TEMP, neopool_type, Settings->flag2.temperature_resolution, &fvalue, TempUnit());
    }

    // Voltage
    {
      float f5volt = (float)NeoPoolGetData(MBF_VOLT_5)/620.69;
      float f12volt = (float)NeoPoolGetData(MBF_VOLT_12)/1000;
      float f24_36volt = (float)NeoPoolGetData(MBF_VOLT_24_36)/1000;
      WSContentSend_PD(HTTP_SNS_NEOPOOL_VOLTAGE, neopool_type,
        Settings->flag2.voltage_resolution, &f5volt,
        Settings->flag2.voltage_resolution, &f12volt,
        Settings->flag2.voltage_resolution, &f24_36volt);
    }

    // Hydrolysis
    if (NeoPoolIsHydrolysis()) {
      // Data
      int decimals = NeoPoolIsHydrolysisInPercent() ? 0 : 1;
      fvalue = (float)NeoPoolGetData(MBF_HIDRO_CURRENT)/10;
      WSContentSend_PD(HTTP_SNS_NEOPOOL_HYDROLYSIS, neopool_type, decimals, &fvalue,
        NeoPoolIsHydrolysisInPercent() ? PSTR(D_NEOPOOL_UNIT_PERCENT) : PSTR(D_NEOPOOL_UNIT_GPERH));

      // S1
      float fhidromax = (float)NeoPoolGetData(MBF_PAR_HIDRO)/10;
      ext_snprintf_P(stemp, sizeof(stemp), PSTR(NEOPOOL_FMT_HIDRO " %s"), decimals, &fhidromax,
        NeoPoolIsHydrolysisInPercent() ? PSTR(D_NEOPOOL_UNIT_PERCENT) : PSTR(D_NEOPOOL_UNIT_GPERH));
      WSContentSend_PD(HTTP_SNS_NEOPOOL_STATUS, bg_color, HTTP_SNS_NEOPOOL_STATUS_INACTIVE, stemp);
      WSContentSend_PD(PSTR(" "));

      // S2
      if (0 == (NeoPoolGetData(MBF_HIDRO_STATUS) & MBMSK_HIDRO_STATUS_MODULE_ACTIVE)) {
        WSContentSend_PD(HTTP_SNS_NEOPOOL_STATUS, bg_color, HTTP_SNS_NEOPOOL_STATUS_NORMAL, PSTR(D_NEOPOOL_STATUS_OFF));
      } else if (0 == (NeoPoolGetData(MBF_HIDRO_STATUS) & MBMSK_HIDRO_STATUS_FL1)) {
        WSContentSend_PD(HTTP_SNS_NEOPOOL_STATUS, bg_color, HTTP_SNS_NEOPOOL_STATUS_ACTIVE, PSTR(D_NEOPOOL_STATUS_FLOW));
      } else if (NeoPoolGetData(MBF_HIDRO_STATUS) & MBMSK_HIDRO_STATUS_POL1) {
        WSContentSend_PD(HTTP_SNS_NEOPOOL_STATUS, bg_color, HTTP_SNS_NEOPOOL_STATUS_NORMAL, PSTR(D_NEOPOOL_POLARIZATION "1"));
      } else if (NeoPoolGetData(MBF_HIDRO_STATUS) & MBMSK_HIDRO_STATUS_POL2) {
        WSContentSend_PD(HTTP_SNS_NEOPOOL_STATUS, bg_color, HTTP_SNS_NEOPOOL_STATUS_NORMAL, PSTR(D_NEOPOOL_POLARIZATION "2"));
      } else {
        WSContentSend_PD(HTTP_SNS_NEOPOOL_STATUS, bg_color, HTTP_SNS_NEOPOOL_STATUS_NORMAL, PSTR(D_NEOPOOL_STATUS_OFF));
      }
      WSContentSend_PD(PSTR(" "));
      // S3
      if (NeoPoolGetData(MBF_HIDRO_STATUS) & MBMSK_HIDRO_STATUS_COVER) {
        WSContentSend_PD(HTTP_SNS_NEOPOOL_STATUS, bg_color, HTTP_SNS_NEOPOOL_STATUS_ACTIVE, PSTR(D_NEOPOOL_COVER));
      } else  {
        WSContentSend_PD(HTTP_SNS_NEOPOOL_STATUS, bg_color, HTTP_SNS_NEOPOOL_STATUS_DISABLED, PSTR(D_NEOPOOL_COVER));
      }
      WSContentSend_PD(PSTR(" "));
      // S4
      if (NeoPoolGetData(MBF_HIDRO_STATUS) & MBMSK_HIDRO_STATUS_SHOCK_ENABLED) {
        if ((NeoPoolGetData(MBF_CELL_BOOST) & MBMSK_CELL_BOOST_REDOX_CTL) == 0) {
          WSContentSend_PD(HTTP_SNS_NEOPOOL_STATUS, bg_color, HTTP_SNS_NEOPOOL_STATUS_ACTIVE, PSTR(D_NEOPOOL_SHOCK "+" D_NEOPOOL_REDOX));
        } else {
          WSContentSend_PD(HTTP_SNS_NEOPOOL_STATUS, bg_color, HTTP_SNS_NEOPOOL_STATUS_ACTIVE, PSTR(D_NEOPOOL_SHOCK));
        }
      } else  {
        WSContentSend_PD(HTTP_SNS_NEOPOOL_STATUS, bg_color, HTTP_SNS_NEOPOOL_STATUS_DISABLED, PSTR(D_NEOPOOL_SHOCK));
      }
      WSContentSend_PD(PSTR(" "));
      // S5
      if (NeoPoolGetData(MBF_HIDRO_STATUS) & MBMSK_HIDRO_STATUS_LOW) {
        WSContentSend_PD(HTTP_SNS_NEOPOOL_STATUS, bg_color, HTTP_SNS_NEOPOOL_STATUS_ACTIVE, PSTR(D_NEOPOOL_LOW));
      } else  {
        WSContentSend_PD(HTTP_SNS_NEOPOOL_STATUS, bg_color, HTTP_SNS_NEOPOOL_STATUS_DISABLED, PSTR(D_NEOPOOL_LOW));
      }

      WSContentSend_PD(PSTR("{e}"));
    }

    // pH
    if (NeoPoolIspHModule()) {
      // Data
      fvalue = (float)NeoPoolGetData(MBF_MEASURE_PH)/100;
      WSContentSend_PD(HTTP_SNS_NEOPOOL_PH, neopool_type, neopool_resolution.ph, &fvalue);
      WSContentSend_PD(PSTR("&nbsp;"));
      // S1
      float fphmax = (float)NeoPoolGetData(MBF_PAR_PH1)/100;
      ext_snprintf_P(stemp, sizeof(stemp), PSTR(NEOPOOL_FMT_PH), neopool_resolution.ph, &fphmax);
      WSContentSend_PD(HTTP_SNS_NEOPOOL_STATUS, bg_color,
        (((uint16_t)(fvalue*10) > (uint16_t)(fphmax*10)) ? HTTP_SNS_NEOPOOL_STATUS_ACTIVE : HTTP_SNS_NEOPOOL_STATUS_INACTIVE), stemp);
      WSContentSend_PD(PSTR(" "));
      // S2
      if ((NeoPoolGetData(MBF_PH_STATUS) & MBMSK_PH_STATUS_ALARM) >=1 && (NeoPoolGetData(MBF_PH_STATUS) & MBMSK_PH_STATUS_ALARM) <= 3) {
        GetTextIndexed(stemp, sizeof(stemp), NeoPoolGetData(MBF_PH_STATUS) & MBMSK_PH_STATUS_ALARM, kNeoPoolpHAlarms);
        WSContentSend_PD(HTTP_SNS_NEOPOOL_STATUS, bg_color, HTTP_SNS_NEOPOOL_STATUS_ACTIVE, stemp);
      }
      WSContentSend_PD(PSTR(" "));
      // S3
      if (NeoPoolGetData(MBF_PH_STATUS) & MBMSK_PH_STATUS_CTRL_ACTIVE) {
        if (MBV_PH_ACID_BASE_ALARM6 == (NeoPoolGetData(MBF_PH_STATUS) & MBMSK_PH_STATUS_ALARM)) {
          WSContentSend_PD(HTTP_SNS_NEOPOOL_STATUS, bg_color, HTTP_SNS_NEOPOOL_STATUS_ACTIVE, PSTR(D_NEOPOOL_STATUS_TANK));
        } else if (NeoPoolGetData(MBF_PH_STATUS) & (MBMSK_PH_STATUS_ACID_PUMP_ACTIVE | MBMSK_PH_STATUS_BASE_PUMP_ACTIVE)) {
          WSContentSend_PD(HTTP_SNS_NEOPOOL_STATUS, bg_color, HTTP_SNS_NEOPOOL_STATUS_ACTIVE, PSTR(D_NEOPOOL_STATUS_ON));
        } else if (MBV_PH_ACID_BASE_ALARM0 != (NeoPoolGetData(MBF_PH_STATUS) & MBMSK_PH_STATUS_ALARM)) {
          WSContentSend_PD(HTTP_SNS_NEOPOOL_STATUS, bg_color, HTTP_SNS_NEOPOOL_STATUS_NORMAL, PSTR(D_NEOPOOL_STATUS_WAIT));
        }
      } else {
          WSContentSend_PD(HTTP_SNS_NEOPOOL_STATUS, bg_color, HTTP_SNS_NEOPOOL_STATUS_DISABLED, PSTR(D_NEOPOOL_STATUS_OFF));
      }
      WSContentSend_PD(PSTR(" "));
      // S4
      if (0 == (NeoPoolGetData(MBF_PH_STATUS) & MBMSK_PH_STATUS_CTRL_BY_FL)) {
        WSContentSend_PD(HTTP_SNS_NEOPOOL_STATUS, bg_color, HTTP_SNS_NEOPOOL_STATUS_ACTIVE, PSTR(D_NEOPOOL_FLOW1));
      } else  {
        WSContentSend_PD(HTTP_SNS_NEOPOOL_STATUS, bg_color, HTTP_SNS_NEOPOOL_STATUS_DISABLED, PSTR(D_NEOPOOL_FLOW1));
      }
      WSContentSend_PD(PSTR("{e}"));
    }

    // Redox
    // Status/Alarm: S1 S2
    // S1: 0
    // S2: FL1
    if (NeoPoolIsRedox()) {
      WSContentSend_PD(HTTP_SNS_NEOPOOL_REDOX, neopool_type, NeoPoolGetData(MBF_MEASURE_RX));
      WSContentSend_PD(PSTR("&nbsp;"));
      // S1
      ext_snprintf_P(stemp, sizeof(stemp), PSTR(NEOPOOL_FMT_RX " "  D_UNIT_MILLIVOLT), NeoPoolGetData(MBF_PAR_RX1));
      WSContentSend_PD(HTTP_SNS_NEOPOOL_STATUS, bg_color,
        (NeoPoolGetData(MBF_HIDRO_CURRENT) ? HTTP_SNS_NEOPOOL_STATUS_ACTIVE : HTTP_SNS_NEOPOOL_STATUS_INACTIVE),
        stemp);
      WSContentSend_PD(PSTR("{e}"));
    }

    // Chlorine
    if (NeoPoolIsChlorine()) {
      fvalue = (float)NeoPoolGetData(MBF_MEASURE_CL)/100;
      WSContentSend_PD(HTTP_SNS_NEOPOOL_PPM_CHLORINE, neopool_type, neopool_resolution.ph, &fvalue);
    }

    // Conductivity
    if (NeoPoolIsConductivity()) {
      WSContentSend_PD(HTTP_SNS_NEOPOOL_CONDUCTIVITY, neopool_type, NeoPoolGetData(MBF_MEASURE_CONDUCTIVITY));
    }

    // Ionization
    if (NeoPoolIsIonization()) {
      char spol[100];
      snprintf_P(spol, sizeof(spol), PSTR(" "  D_NEOPOOL_POLARIZATION  "%d"), (NeoPoolGetData(MBF_ION_STATUS) & (MBMSK_ION_STATUS_POL1 | MBMSK_ION_STATUS_POL2)) >> 13);
      snprintf_P(stemp, sizeof(stemp), PSTR("%s%s%s"),
        (NeoPoolGetData(MBF_ION_STATUS) & (MBMSK_ION_STATUS_POL1 | MBMSK_ION_STATUS_POL2)) ? spol : PSTR(""),
        NeoPoolGetData(MBF_ION_STATUS) & MBMSK_ION_STATUS_ON_TARGET ? PSTR(" " D_NEOPOOL_SETPOINT_OK) : PSTR(""),
        NeoPoolGetData(MBF_ION_STATUS) & MBMSK_ION_STATUS_PROGTIME_EXCEEDED ? PSTR(" " D_NEOPOOL_PR_OFF) : PSTR("")
        );
      fvalue = (float)NeoPoolGetData(MBF_ION_CURRENT);
      WSContentSend_PD(HTTP_SNS_NEOPOOL_IONIZATION, neopool_type,
        neopool_resolution.ion, &fvalue,
        stemp,
        NeoPoolGetData(MBF_ION_STATUS) & MBMSK_ION_STATUS_LOW ? PSTR(" " D_NEOPOOL_LOW) : PSTR("")
      );
    }

    // Filtration mode
    GetTextIndexed(stemp, sizeof(stemp), NeoPoolGetData(MBF_PAR_FILT_MODE) <= MBV_PAR_FILT_INTELLIGENT ? NeoPoolGetData(MBF_PAR_FILT_MODE) : nitems(kNeoPoolFiltrationMode)-1, kNeoPoolFiltrationMode);
    WSContentSend_PD(HTTP_SNS_NEOPOOL_FILT_MODE, neopool_type, stemp);

    // Relays
    for (uint32_t i = 0; i < NEOPOOL_RELAY_MAX; i++) {
      char sdesc[24];
      memset(sdesc, 0, nitems(sdesc));
      memset(stemp, 0, nitems(stemp));
      if        (0 != NeoPoolGetData(MBF_PAR_PH_ACID_RELAY_GPIO) && i == NeoPoolGetData(MBF_PAR_PH_ACID_RELAY_GPIO)-1) {
        strncpy_P(sdesc, PSTR(D_NEOPOOL_RELAY_PH_ACID), sizeof(sdesc));
      } else if (0 != NeoPoolGetData(MBF_PAR_PH_BASE_RELAY_GPIO) && i == NeoPoolGetData(MBF_PAR_PH_BASE_RELAY_GPIO)-1) {
        strncpy_P(sdesc, PSTR(D_NEOPOOL_RELAY_PH_BASE), sizeof(sdesc));
      } else if (0 != NeoPoolGetData(MBF_PAR_RX_RELAY_GPIO)      && i == NeoPoolGetData(MBF_PAR_RX_RELAY_GPIO)-1) {
        strncpy_P(sdesc, PSTR(D_NEOPOOL_RELAY_RX), sizeof(sdesc));
      } else if (0 != NeoPoolGetData(MBF_PAR_CL_RELAY_GPIO)      && i == NeoPoolGetData(MBF_PAR_CL_RELAY_GPIO)-1) {
        strncpy_P(sdesc, PSTR(D_NEOPOOL_RELAY_CL), sizeof(sdesc));
      } else if (0 != NeoPoolGetData(MBF_PAR_CD_RELAY_GPIO)      && i == NeoPoolGetData(MBF_PAR_CD_RELAY_GPIO)-1) {
        strncpy_P(sdesc, PSTR(D_NEOPOOL_RELAY_CD), sizeof(sdesc));
      } else if (0 != NeoPoolGetData(MBF_PAR_FILT_GPIO)          && i == NeoPoolGetData(MBF_PAR_FILT_GPIO)-1) {
          char smotorspeed[32];
          strncpy_P(sdesc, PSTR(D_NEOPOOL_RELAY_FILTRATION), sizeof(sdesc));
          GetTextIndexed(smotorspeed, sizeof(smotorspeed), NeoPoolGetFiltrationSpeed(), kNeoPoolFiltrationSpeed);
          snprintf_P(stemp, sizeof(stemp), PSTR("%s%s%s%s"), ((NeoPoolGetData(MBF_RELAY_STATE) & (1<<i))?D_ON:D_OFF), *smotorspeed ? PSTR(" (") : PSTR(""), smotorspeed,  *smotorspeed ? PSTR(")") : PSTR(""));
      } else if (0 != NeoPoolGetData(MBF_PAR_LIGHTING_GPIO) && i == NeoPoolGetData(MBF_PAR_LIGHTING_GPIO)-1) {
          strncpy_P(sdesc, PSTR(D_NEOPOOL_RELAY_LIGHT), sizeof(sdesc));
      } else if (0 != NeoPoolGetData(MBF_PAR_HEATING_GPIO) && i == NeoPoolGetData(MBF_PAR_HEATING_GPIO)-1) {
          strncpy_P(sdesc, PSTR(D_NEOPOOL_RELAY_HEATING), sizeof(sdesc));
      } else if (0 != NeoPoolGetData(MBF_PAR_UV_RELAY_GPIO) && i == NeoPoolGetData(MBF_PAR_UV_RELAY_GPIO)-1) {
          strncpy_P(sdesc, PSTR(D_NEOPOOL_RELAY_UV), sizeof(sdesc));
      } else if (0 != NeoPoolGetData(MBF_PAR_FILTVALVE_GPIO) && i == NeoPoolGetData(MBF_PAR_FILTVALVE_GPIO)-1) {
          strncpy_P(sdesc, PSTR(D_NEOPOOL_RELAY_VALVE), sizeof(sdesc));
      } else if (i > 2) {
          // Aux
          char sname[(MBF_PAR_UICFG_MACH_NAME_AUX2_0 - MBF_PAR_UICFG_MACH_NAME_AUX1_0) * 2 + 1];
          uint16_t base = MBF_PAR_UICFG_MACH_NAME_AUX1_0 + ((i - 3) * (MBF_PAR_UICFG_MACH_NAME_AUX2_0 - MBF_PAR_UICFG_MACH_NAME_AUX1_0));
          for (uint16_t k = 0; k < (MBF_PAR_UICFG_MACH_NAME_AUX2_0 - MBF_PAR_UICFG_MACH_NAME_AUX1_0); k++) {
            uint16_t data = NeoPoolGetData(base + k);
            sname[k*2] = (char)(data >> 8);
            sname[k*2 + 1] = (char)(data & 0xFF);
          }
          if (*sname) {
            snprintf_P(sdesc, sizeof(sdesc), PSTR(D_NEOPOOL_RELAY_AUX  " %d (%s)"), i-2, sname);
          } else {
            snprintf_P(sdesc, sizeof(sdesc), PSTR(D_NEOPOOL_RELAY_AUX  " %d"), i-2);
          }
      } else {
          // unassigned relay
          snprintf_P(sdesc, sizeof(sdesc), PSTR(D_NEOPOOL_RELAY  " %d"), i+1);
      }

      WSContentSend_PD(HTTP_SNS_NEOPOOL_RELAY, neopool_type, sdesc,
        '\0' == *stemp ? ((NeoPoolGetData(MBF_RELAY_STATE) & (1<<i))?PSTR(D_ON):PSTR(D_OFF)) : stemp);
    }

    {
      // Cell runtime
      WSContentSend_PD(HTTP_SNS_NEOPOOL_CELL_RUNTIME, neopool_type,
        GetDuration(NeoPoolGetDataLong(MBF_CELL_RUNTIME_LOW)).c_str());
    }

#endif  // USE_WEBSERVER
  }
}



/*********************************************************************************************\
 * Command implementation
\*********************************************************************************************/

void NeopoolReadWriteResponse(uint16_t addr, uint16_t *data, uint16_t cnt, bool fbits32, int16_t bit)
{
  const char *data_fmt;
  uint32_t ldata;

  Response_P(PSTR("{\"%s\":{\""  D_JSON_ADDRESS  "\":"), XdrvMailbox.command);
  ResponseAppend_P(NEOPOOL_RESULT_HEX == neopool_result ? PSTR("\"0x%04X\"") : PSTR("%d"), addr);
  ResponseAppend_P(PSTR(",\""  D_JSON_DATA  "\":"));

  data_fmt = PSTR("%ld");
  if (NEOPOOL_RESULT_HEX == neopool_result) {
    data_fmt = fbits32 ? PSTR("\"0x%08X\"") : PSTR("\"0x%04X\"");
  }
  ldata = (uint32_t)data[0];
  if (fbits32) {
    ldata |= (uint32_t)data[1] << 16;
  }
  if ( cnt > 1 ) {
    char sdel[2] = {0};
    ResponseAppend_P(PSTR("["));
    for(uint16_t i=0; i<cnt; i++) {
      ResponseAppend_P(PSTR("%s"), sdel);
      ldata = (uint32_t)data[(fbits32+1)*i];
      if (fbits32) {
        ldata |= (uint32_t)data[(fbits32+1)*i+1] << 16;
      }
      ResponseAppend_P(data_fmt, ldata);
      *sdel = ',';
    }
    ResponseAppend_P(PSTR("]"));
  } else {
    ResponseAppend_P(data_fmt, ldata);
  }
  if (bit >= 0) {
    ResponseAppend_P(PSTR(",\""  D_NEOPOOL_JSON_BIT  "%d\":%ld"), bit, (ldata>>bit) & 1);
  }
  ResponseJsonEndEnd();
}


void NeopoolCmndError(void)
{
  Response_P(PSTR("{\""  D_JSON_COMMAND  "\":\""  D_JSON_ERROR  "\"}"));
}


void NeopoolResponseError(void)
{
  ResponseCmndChar(PSTR(D_JSON_ERROR));
}


void CmndNeopoolResult(void)
{
  if (XdrvMailbox.data_len && XdrvMailbox.payload >= 0 && XdrvMailbox.payload < NEOPOOL_RESULT_MAX) {
    neopool_result = XdrvMailbox.payload;
  }
  ResponseCmndNumber(neopool_result);
}


void CmndNeopoolReadReg(void)
{
  uint16_t addr, data[30] = { 0 }, cnt = 1;
  uint32_t value[2] = { 0 };
  uint32_t params_cnt = ParseParameters(nitems(value), value);
  bool fbits32 = !strcasecmp_P(XdrvMailbox.command, PSTR(D_PRFX_NEOPOOL  D_CMND_NP_READL));

  cnt = 1;
  if (2 == params_cnt) {
    cnt = value[1];
  }
  if (params_cnt && cnt < (fbits32 ? (nitems(data)/2) : nitems(data))) {
    addr = value[0];
    if (NEOPOOL_MODBUS_OK != NeoPoolReadRegister(addr, data, fbits32 ? (cnt*2) : cnt)) {
      NeopoolResponseError();
      return;
    }
  }
  NeopoolReadWriteResponse(addr, data, cnt, fbits32, -1);
}


void CmndNeopoolWriteReg(void)
{
  uint16_t addr, data[20] = { 0 }, cnt;
  uint32_t value[(nitems(data)/2)+1] = { 0 };
  uint32_t params_cnt = ParseParameters(nitems(value), value);
  bool fbits32 = !strcasecmp_P(XdrvMailbox.command, PSTR(D_PRFX_NEOPOOL  D_CMND_NP_WRITEL));

  if (params_cnt > 1) {
    addr = value[0];
    cnt = params_cnt-1;
    for (uint32_t i = 0; i < cnt; i++) {
      if (fbits32) {
        data[i*2] = value[i+1];       // LSB
        data[i*2+1] = value[i+1]>>16; // MSB
      } else {
        data[i] = value[i+1];
      }
    }
    if (NEOPOOL_MODBUS_OK != NeoPoolWriteRegister(addr, data, fbits32 ? cnt*2 : cnt)) {
      NeopoolResponseError();
      return;
    }
  }
  if (NEOPOOL_MODBUS_OK != NeoPoolReadRegister(addr, data, fbits32 ? (cnt*2) : cnt)) {
    NeopoolResponseError();
    return;
  }
  NeopoolReadWriteResponse(addr, data, cnt, fbits32, -1);
}


void CmndNeopoolBit(void)
{
  uint16_t addr, data;
  uint16_t bit;
  uint32_t value[3] = { 0 };
  uint32_t params_cnt = ParseParameters(nitems(value), value);
  bool fbits32 = !strcasecmp_P(XdrvMailbox.command, PSTR(D_PRFX_NEOPOOL  D_CMND_NP_BITL));
  uint16_t tempdata[2];

  if (params_cnt >= 2) {
    addr = value[0];
    bit = (uint16_t)value[1];

    if (bit >= 0 && bit < 16<<fbits32) {
      if (3 == params_cnt) {
        data = value[2];
        if (data >=0 && data <= 1 && NEOPOOL_MODBUS_OK == NeoPoolReadRegister(addr, tempdata, 1<<fbits32)) {
          if (fbits32) {
            uint32_t tempdata32 = tempdata[0] | (tempdata[1]<<16);
            tempdata32 &= ~(1<<bit);
            tempdata32 |= data<<bit;
            tempdata[0] = (uint16_t)tempdata32;
            tempdata[1] = (uint16_t)(tempdata32>>16);
          } else {
            tempdata[0] &= ~(1<<bit);
            tempdata[0] |= (data<<bit);
          }
          if (NEOPOOL_MODBUS_OK != NeoPoolWriteRegister(addr, tempdata, 1<<fbits32)) {
            NeopoolResponseError();
            return;
          }
        } else {
          NeopoolResponseError();
          return;
        }
      }
      if (NEOPOOL_MODBUS_OK != NeoPoolReadRegister(addr, &data, 1)) {
        NeopoolResponseError();
        return;
      }
      NeopoolReadWriteResponse(addr, &data, 1, fbits32, bit);
      return;
    }

  }
  NeopoolCmndError();
}


void CmndNeopoolFiltrationRes(uint16_t data)
{
  uint16_t speed = NeoPoolGetFiltrationSpeed();
  if (speed) {
    Response_P(PSTR("{\"%s\":\"%s\",\""  D_NEOPOOL_JSON_FILTRATION_SPEED  "\":\"%d\"}"),
      XdrvMailbox.command,
      GetStateText(data),
      (speed < 3) ? speed : 3);
  } else {
    ResponseCmndStateText(data);
  }
}

void CmndNeopoolFiltration(void)
{
  uint16_t addr = MBF_PAR_FILT_MANUAL_STATE;
  uint16_t data;
  uint16_t filtration_conf;
  uint32_t value[2] = { 0 };
  uint32_t params_cnt = ParseParameters(nitems(value), value);

  if (XdrvMailbox.data_len) {
    if (NEOPOOL_MODBUS_OK != NeoPoolReadRegister(MBF_PAR_FILTRATION_CONF, &filtration_conf, 1)) {
      NeopoolResponseError();
      return;
    }
    if (params_cnt > 2 || (params_cnt > 1 && (MBV_PAR_FILTRATION_TYPE_STANDARD == (filtration_conf & MBMSK_PAR_FILTRATION_CONF_TYPE)))) {
        // no speed control for standard filtration types
        NeopoolCmndError();
        return;
    }
    if (params_cnt > 1) {
      if (value[1] >= 1 && value[1] <= 3) {
        // Set filtration speed first
        NeoPoolWriteRegisterWord(MBF_PAR_FILTRATION_CONF,
          (filtration_conf & ~MBMSK_PAR_FILTRATION_CONF_DEF_SPEED) | ((value[1] - 1) << MBSHFT_PAR_FILTRATION_CONF_DEF_SPEED));
        NeoPoolWriteRegisterWord(MBF_EXEC, 1);
      } else {
          NeopoolCmndError();
          return;
      }
    }
    if (value[0] >= 0 && value[0] <= 2) {
      // Set MBF_PAR_FILT_MODE
      if (NEOPOOL_MODBUS_OK != NeoPoolWriteRegisterWord(MBF_PAR_FILT_MODE, MBV_PAR_FILT_MANUAL)) {
        NeopoolResponseError();
        return;
      }
      if (2 == value[0]) {
        if (NEOPOOL_MODBUS_OK != NeoPoolReadRegister(MBF_PAR_FILTRATION_STATE, &data, 1)) {
          NeopoolResponseError();
          return;
        }
        value[0] = data ? 0 : 1;
      }
      // Set filtration mode to manual
      if (NEOPOOL_MODBUS_OK != NeoPoolWriteRegisterWord(MBF_PAR_FILT_MANUAL_STATE, value[0])) {
        NeopoolResponseError();
        return;
      }
      CmndNeopoolFiltrationRes(value[0]);
      return;
    } else {
      NeopoolCmndError();
      return;
    }
  }
  if (NEOPOOL_MODBUS_OK != NeoPoolReadRegister(MBF_PAR_FILTRATION_STATE, &data, 1)) {
    NeopoolResponseError();
    return;
  }
  CmndNeopoolFiltrationRes(data);
}


void CmndNeopoolFiltrationMode(void)
{
  uint16_t addr = MBF_PAR_FILT_MODE;
  uint16_t data;
  char stemp[80];

  if (XdrvMailbox.data_len) {
    char command[CMDSZ];
    int mode = GetCommandCode(command, sizeof(command), XdrvMailbox.data, kNeoPoolFiltrationModeCmnd);
    if (mode >= 0) {
      XdrvMailbox.payload = pgm_read_byte(sNeoPoolFiltrationMode + mode);
    }
    if ((XdrvMailbox.payload >= MBV_PAR_FILT_MANUAL && XdrvMailbox.payload <= MBV_PAR_FILT_INTELLIGENT) ||
        MBV_PAR_FILT_BACKWASH == XdrvMailbox.payload) {
        // Set MBF_PAR_FILT_MODE
        if (NEOPOOL_MODBUS_OK != NeoPoolWriteRegisterWord(addr, XdrvMailbox.payload)) {
          NeopoolResponseError();
          return;
        }
    } else {
      NeopoolCmndError();
      return;
    }
  }
  if (NEOPOOL_MODBUS_OK != NeoPoolReadRegister(addr, &data, 1)) {
    NeopoolResponseError();
    return;
  }
  ResponseCmndChar(GetTextIndexed(stemp, sizeof(stemp), data < MBV_PAR_FILT_INTELLIGENT ? data : nitems(kNeoPoolFiltrationModeCmnd)-1, kNeoPoolFiltrationMode));
}


void CmndNeopoolFiltrationSpeed(void)
{
  uint16_t speed;
  uint16_t filtration_conf;

  if (NEOPOOL_MODBUS_OK != NeoPoolReadRegister(MBF_PAR_FILTRATION_CONF, &filtration_conf, 1)) {
    NeopoolResponseError();
    return;
  }
  if (MBV_PAR_FILTRATION_TYPE_STANDARD == (filtration_conf & MBMSK_PAR_FILTRATION_CONF_TYPE)) {
      // no speed control for standard filtration types
      NeopoolCmndError();
      return;
  }

  speed = NeoPoolGetFiltrationSpeed();
  if (XdrvMailbox.data_len) {
    if (XdrvMailbox.payload >= 1 && XdrvMailbox.payload <= 3) {
      speed = XdrvMailbox.payload;
      // Set filtration speed
      if (NEOPOOL_MODBUS_OK != NeoPoolWriteRegisterWord(MBF_PAR_FILTRATION_CONF,
          (filtration_conf & ~MBMSK_PAR_FILTRATION_CONF_DEF_SPEED) | ((speed - 1) << MBSHFT_PAR_FILTRATION_CONF_DEF_SPEED))) {
          NeopoolResponseError();
          return;
        }
      NeoPoolWriteRegisterWord(MBF_EXEC, 1);
    } else {
        NeopoolCmndError();
        return;
    }
  }
  ResponseCmndNumber(speed);
}


void CmndNeopoolTime(void)
{
  uint16_t data[2];
  uint32_t np_time;

  if (XdrvMailbox.data_len) {
    np_time = XdrvMailbox.payload;
    if (0 == XdrvMailbox.payload) {
      np_time = Rtc.local_time;
    }
    if (1 == XdrvMailbox.payload) {
      np_time = Rtc.utc_time;
    }
#ifdef DEBUG_TASMOTA_SENSOR
    AddLog(LOG_LEVEL_DEBUG, PSTR("NEO: set time to %ld"), np_time);
#endif  // DEBUG_TASMOTA_SENSOR
    data[0] = np_time;
    data[1] = np_time>>16;
    NeoPoolWriteRegister(MBF_PAR_TIME_LOW, data, 2);
    NeoPoolWriteRegisterWord(MBF_ACTION_COPY_TO_RTC, 1);
  }
  if (NEOPOOL_MODBUS_OK != NeoPoolReadRegister(MBF_PAR_TIME_LOW, data, 2)) {
    NeopoolResponseError();
    return;
  }
  np_time = (uint32_t)data[0] | ((uint32_t)data[1] << 16);
#ifdef DEBUG_TASMOTA_SENSOR
  AddLog(LOG_LEVEL_DEBUG, PSTR("NEO: time read %ld"), np_time);
#endif  // DEBUG_TASMOTA_SENSOR
  ResponseCmndChar(GetDT(NeoPoolGetDataLong(MBF_PAR_TIME_LOW)).c_str());
}


void CmndNeopoolLight(void)
{
  uint16_t data, set;
  uint16_t timer_val[] = {MBV_PAR_CTIMER_ALWAYS_OFF, MBV_PAR_CTIMER_ALWAYS_ON, POWER_TOGGLE, MBV_PAR_CTIMER_ENABLED};
  uint32_t value[2] = { 0 };
  uint32_t params_cnt = ParseParameters(nitems(value), value);

  if (NEOPOOL_MODBUS_OK != NeoPoolReadRegister(MBF_PAR_LIGHTING_GPIO, &neopool_light_relay, 1) ||
      NEOPOOL_MODBUS_OK != NeoPoolReadRegister(MBF_RELAY_STATE, &data, 1)) {
    NeopoolResponseError();
    return;
  }
  if (neopool_light_relay >= 1 && neopool_light_relay <= NEOPOOL_RELAY_MAX) {
    // get/set light
    if (1 == params_cnt && XdrvMailbox.payload >= 0 && XdrvMailbox.payload < nitems(timer_val)) {
      if (POWER_TOGGLE == timer_val[XdrvMailbox.payload]) {
        XdrvMailbox.payload = ((data >>= (neopool_light_relay - 1)) & 1) ? POWER_OFF : POWER_ON;
      }
      NeoPoolWriteRegisterWord(MBF_PAR_TIMER_BLOCK_LIGHT_INT + MBV_TIMER_OFFMB_TIMER_ENABLE, timer_val[XdrvMailbox.payload]);
      NeoPoolWriteRegisterWord(MBF_EXEC, 1);
      // data >>= (neopool_light_relay - 1);
      ResponseCmndStateText(XdrvMailbox.payload);
      return;
    }
    // set next light program
    if (nitems(timer_val) == XdrvMailbox.payload) {
      if (NEOPOOL_MODBUS_OK != NeoPoolWriteRegisterWord(MBF_SET_MANUAL_CTRL, 1)) {
        NeopoolResponseError();
        return;
      }
      if (params_cnt > 1) {
        if (value[1] >= NEOPOOL_LIGHT_PRG_DELAY_MIN and value[1] <= NEOPOOL_LIGHT_PRG_DELAY_MAX) {
          // use given delay
          neopool_light_prg_delay = value[1];
        } else {
          NeopoolCmndError();
          return;
        }
      } else {
        // use default delay
        neopool_light_prg_delay = NEOPOOL_LIGHT_PRG_DELAY;
      }
      if (data & 1<<(neopool_light_relay-1)) {
        // light already on, start programming immediately
        CmndNeopoolLightPrgWaitStart();
      } else {
        // light currently off: must first switched on and wait for inital prg delay NEOPOOL_LIGHT_PRG_WAIT
        data |= 1<<(neopool_light_relay-1);
        if (NEOPOOL_MODBUS_OK != NeoPoolWriteRegister(MBF_RELAY_STATE, &data, 1)) {
          NeopoolResponseError();
          return;
        }
        neopoll_cmd = &CmndNeopoolLightPrgWaitStart;
        neopoll_cmd_delay = NEOPOOL_LIGHT_PRG_WAIT * 4 / 10;
      }
      ResponseCmndDone();
      return;
    }
    if (0 == params_cnt) {
      ResponseCmndStateText((data >>= (neopool_light_relay - 1)) & 1);
      return;
    }

  }
  NeopoolCmndError();
}


void CmndNeopoolLightPrgWaitStart(void)
{
  uint16_t data;

  // start prg sequence with light off
  if (NEOPOOL_MODBUS_OK == NeoPoolReadRegister(MBF_RELAY_STATE, &data, 1)) {
    if (NEOPOOL_MODBUS_OK == NeoPoolWriteRegisterWord(MBF_RELAY_STATE, data & ~(1<<(neopool_light_relay-1)))) {
      neopoll_cmd = &CmndNeopoolLightPrgEnd;
      // then wait for given prg sequence delay
      neopoll_cmd_delay = neopool_light_prg_delay * 4 / 10;
    }
  }
}


void CmndNeopoolLightPrgEnd(void)
{
  // exit manual ctrl
  NeoPoolWriteRegisterWord(MBF_SET_MANUAL_CTRL, 0);
  // switch light on to finish prg sequence
  NeoPoolWriteRegisterWord(MBF_PAR_TIMER_BLOCK_LIGHT_INT + MBV_TIMER_OFFMB_TIMER_ENABLE, MBV_PAR_CTIMER_ALWAYS_ON);
  NeoPoolWriteRegisterWord(MBF_EXEC, 1);
}


bool CmndNeopoolSetParam(uint16_t reg, uint16_t data, uint16_t factor, float min, float max)
{
  if (data >= min*(float)factor && data <= max*(float)factor) {
    if (NEOPOOL_MODBUS_OK != NeoPoolWriteRegisterWord(reg, data) ||
        NEOPOOL_MODBUS_OK != NeoPoolWriteRegisterWord(MBF_EXEC, 1) ||
        NEOPOOL_MODBUS_OK != NeoPoolWriteRegisterWord(MBF_SAVE_TO_EEPROM, 1)) {
      NeopoolResponseError();
      return false;
    } else {
      return true;
    }
  } else {
    return false;
  }
  return true;
}


bool CmndNeopoolSetParam(uint16_t reg, uint16_t factor, float min, float max)
{
  uint16_t data;

  if (XdrvMailbox.data_len) {
    return CmndNeopoolSetParam(reg, (int)(CharToFloat(XdrvMailbox.data) * (float)factor), factor, min, max);
  }
  return true;
}


void CmndNeopoolGetParam(uint16_t reg, uint16_t factor, uint16_t decimals, const char* unit)
{
  uint16_t data;

  if (NEOPOOL_MODBUS_OK != NeoPoolReadRegister(reg, &data, 1)) {
      NeopoolResponseError();
      return;
  }

  float fvalue = (float)(data) / (float)factor;
  if (nullptr == unit) {
    ResponseCmndFloat(fvalue, decimals);
  }
  else {
    Response_P(PSTR("{\"%s\":%*_f,\""  D_NEOPOOL_JSON_UNIT  "\":\"%s\"}"), XdrvMailbox.command, decimals, &fvalue, unit);
  }
}


void CmndNeopoolGetParam(uint16_t reg, uint16_t factor, uint16_t decimals)
{
  CmndNeopoolGetParam(reg, factor, decimals, nullptr);
}


void CmndNeopoolpHMin(void)
{
  if (NeoPoolIspHModule()) {
    uint16_t data;
    // read pH max
    if (NEOPOOL_MODBUS_OK != NeoPoolReadRegister(MBF_PAR_PH1, &data, 1)) {
        NeopoolResponseError();
        return;
    }
    if (CmndNeopoolSetParam(MBF_PAR_PH2, 100, 0, (float)data/100)) {
      CmndNeopoolGetParam(MBF_PAR_PH2, 100, neopool_resolution.ph);
    }
  } else {
    NeopoolCmndError();
  }
}


void CmndNeopoolpHMax(void)
{
  if (NeoPoolIspHModule()) {
    uint16_t data;
    // read pH min
    if (NEOPOOL_MODBUS_OK != NeoPoolReadRegister(MBF_PAR_PH2, &data, 1)) {
        NeopoolResponseError();
        return;
    }
    if (CmndNeopoolSetParam(MBF_PAR_PH1, 100, (float)data/100, 14)) {
      CmndNeopoolGetParam(MBF_PAR_PH1, 100, neopool_resolution.ph);
    }
  } else {
    NeopoolCmndError();
  }
}


void CmndNeopoolRedox(void)
{
  if (NeoPoolIsRedox()) {
    if (CmndNeopoolSetParam(MBF_PAR_RX1, 1, 0, 1000)) {
      CmndNeopoolGetParam(MBF_PAR_RX1, 1, 0);
    }
  } else {
    NeopoolCmndError();
  }
}


void CmndNeopoolHydrolysisSet(uint16_t max, const char* unit)
{
  // set value in percent or g/h
  if (CmndNeopoolSetParam(MBF_PAR_HIDRO, 10, 0, (float)max/10)) {
    CmndNeopoolGetParam(MBF_PAR_HIDRO, 10, NeoPoolIsHydrolysisInPercent() ? 0 : 1, unit);
  } else {
    NeopoolCmndError();
  }
}


void CmndNeopoolHydrolysis(void)
{
  if (NeoPoolIsHydrolysis()) {

    if (XdrvMailbox.data_len) {

      uint16_t max;
      // read hydrolysis maximum production level
      if (NEOPOOL_MODBUS_OK != NeoPoolReadRegister(MBF_PAR_HIDRO_NOM, &max, 1)) {
          NeopoolResponseError();
          return;
      }
      //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("NEO: NPHydrolysis - MBF_PAR_HIDRO_NOM = %d"), max);

      // only set if param is given
      if (NeoPoolIsHydrolysisInPercent()) {

        // % system
        //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("NEO: NPHydrolysis - %% system, always set value in %%"));
        // always set value in %
        CmndNeopoolHydrolysisSet(max, PSTR(D_NEOPOOL_UNIT_PERCENT));

      } else {

        // g/h system
        TrimSpace(XdrvMailbox.data);
        //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("NEO: NPHydrolysis - g/h system, param = '%s'"), XdrvMailbox.data);
        bool ispercent = false;
        if ('%' == *(XdrvMailbox.data + strlen(XdrvMailbox.data) - 1)) {
          ispercent = true;
          *(XdrvMailbox.data + strlen(XdrvMailbox.data) - 1) = '\0';
        }
        //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("NEO: NPHydrolysis - g/h system, param = '%s', using percent: %s"), XdrvMailbox.data, ispercent ? PSTR("true") : PSTR("false"));
        if (*XdrvMailbox.data) {

          // only set if param is given (without the possible %)
          if (ispercent) {
            // set value in precent based on max g/h
            //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("NEO: NPHydrolysis - g/h system, set value in %% = %d"), max * TextToInt(XdrvMailbox.data) / 100);
            // max * (float)% / 100
            if (CmndNeopoolSetParam(MBF_PAR_HIDRO, (int)((float)max * CharToFloat(XdrvMailbox.data) / 100), 1, 0, max)) {
              CmndNeopoolGetParam(MBF_PAR_HIDRO, 10, 1, PSTR(D_NEOPOOL_UNIT_GPERH));
            } else {
              NeopoolCmndError();
            }
          }
          else {
            // set value in g/h
            //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("NEO: NPHydrolysis - g/h system, set value in g/h"));
            CmndNeopoolHydrolysisSet(max, PSTR(D_NEOPOOL_UNIT_GPERH));
          }

        } // XdrvMailbox.data

      } // g/h system

    } // XdrvMailbox.data_len
    else {
      CmndNeopoolGetParam(MBF_PAR_HIDRO, 10, 0, NeoPoolIsHydrolysisInPercent() ? PSTR(D_NEOPOOL_UNIT_PERCENT) : PSTR(D_NEOPOOL_UNIT_GPERH));
    }

  } // NeoPoolIsHydrolysis()
}


void CmndNeopoolIonization(void)
{
  if (NeoPoolIsIonization()) {
    uint16_t data;
    // read ionization maximum production level
    if (NEOPOOL_MODBUS_OK != NeoPoolReadRegister(MBF_PAR_ION_NOM, &data, 1)) {
        NeopoolResponseError();
        return;
    }
    if (CmndNeopoolSetParam(MBF_PAR_ION, 1, 0, (float)data)) {
      CmndNeopoolGetParam(MBF_PAR_ION, 1, neopool_resolution.ion);
    }
  } else {
    NeopoolCmndError();
  }
}


void CmndNeopoolChlorine(void)
{
  if (NeoPoolIsChlorine()) {
    if (CmndNeopoolSetParam(MBF_PAR_CL1, 100, 0, 10)) {
      CmndNeopoolGetParam(MBF_PAR_CL1, 100, neopool_resolution.cl);
    }
  } else {
    NeopoolCmndError();
  }
}


void CmndNeopoolControl(void)
{
  Response_P(PSTR("{"));
  NeoPoolAppendModules();

  ResponseAppend_P(PSTR(",\""  D_NEOPOOL_JSON_RELAY  "\":{"));
  ResponseAppend_P(PSTR( "\""  D_NEOPOOL_JSON_RELAY_PH_ACID  "\":%d"), NeoPoolGetData(MBF_PAR_PH_ACID_RELAY_GPIO));
  ResponseAppend_P(PSTR(",\""  D_NEOPOOL_JSON_RELAY_PH_BASE  "\":%d"), NeoPoolGetData(MBF_PAR_PH_BASE_RELAY_GPIO));
  ResponseAppend_P(PSTR(",\""  D_NEOPOOL_JSON_RELAY_RX  "\":%d"), NeoPoolGetData(MBF_PAR_RX_RELAY_GPIO));
  ResponseAppend_P(PSTR(",\""  D_NEOPOOL_JSON_RELAY_CL  "\":%d"), NeoPoolGetData(MBF_PAR_CL_RELAY_GPIO));
  ResponseAppend_P(PSTR(",\""  D_NEOPOOL_JSON_RELAY_CD  "\":%d"), NeoPoolGetData(MBF_PAR_CD_RELAY_GPIO));
  ResponseAppend_P(PSTR(",\""  D_NEOPOOL_JSON_RELAY_HEATING  "\":%d"), NeoPoolGetData(MBF_PAR_HEATING_GPIO));
  ResponseAppend_P(PSTR(",\""  D_NEOPOOL_JSON_RELAY_UV  "\":%d"), NeoPoolGetData(MBF_PAR_UV_RELAY_GPIO));
  ResponseAppend_P(PSTR(",\""  D_NEOPOOL_JSON_RELAY_FILTVALVE  "\":%d"), NeoPoolGetData(MBF_PAR_FILTVALVE_GPIO));
  ResponseJsonEndEnd();
}


void CmndNeopoolSave(void)
{
  if (NEOPOOL_MODBUS_OK == NeoPoolWriteRegisterWord(MBF_SAVE_TO_EEPROM, 1)) {
    ResponseCmndDone();
  } else {
    NeopoolResponseError();
  }
}


void CmndNeopoolExec(void)
{
  if (NEOPOOL_MODBUS_OK == NeoPoolWriteRegisterWord(MBF_EXEC, 1)) {
    ResponseCmndDone();
  } else {
    NeopoolResponseError();
  }
}


void CmndNeopoolEscape(void)
{
  if (NEOPOOL_MODBUS_OK == NeoPoolWriteRegisterWord(MBF_ESCAPE, 1)) {
    ResponseCmndDone();
  } else {
    NeopoolResponseError();
  }
}


void CmndNeopoolOnError(void)
{
  if (XdrvMailbox.data_len && XdrvMailbox.payload >= 0 && XdrvMailbox.payload <= NEOPOOL_MAX_REPEAT_ON_ERROR) {
     neopool_repeat_on_error = XdrvMailbox.payload;
  }
  ResponseCmndNumber(neopool_repeat_on_error);
}


void CmndNeopoolPHRes(void)
{
  if (XdrvMailbox.data_len && XdrvMailbox.payload >= 0 && XdrvMailbox.payload <= 3) {
    neopool_resolution.ph = XdrvMailbox.payload;
  }
  ResponseCmndNumber(neopool_resolution.ph);
}


void CmndNeopoolCLRes(void)
{
  if (XdrvMailbox.data_len && XdrvMailbox.payload >= 0 && XdrvMailbox.payload <= 3) {
    neopool_resolution.cl = XdrvMailbox.payload;
  }
  ResponseCmndNumber(neopool_resolution.cl);
}


void CmndNeopoolIONRes(void)
{
  if (XdrvMailbox.data_len && XdrvMailbox.payload >= 0 && XdrvMailbox.payload <= 3) {
    neopool_resolution.ion = XdrvMailbox.payload;
  }
  ResponseCmndNumber(neopool_resolution.ion);
}


#ifdef NEOPOOL_EMULATE_GPERH
void CmndNeopoolgPerh(void)
{
  if (XdrvMailbox.data_len && XdrvMailbox.payload >= 0 && XdrvMailbox.payload <= 1) {
    neopool_system_gperh = XdrvMailbox.payload;
  }
  ResponseCmndNumber(neopool_system_gperh);
}
#endif


/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns83(uint32_t function)
{
  bool result = false;

  if (FUNC_INIT == function) {
    NeoPoolInit();
  } else if (neopool_active) {
    switch (function) {
      case FUNC_EVERY_250_MSECOND:
        NeoPoolPoll();
        break;
      case FUNC_COMMAND:
        result = DecodeCommand(kNPCommands, NPCommand);
        break;
      case FUNC_JSON_APPEND:
        NeoPoolShow(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        NeoPoolShow(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_NEOPOOL
