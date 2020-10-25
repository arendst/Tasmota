/*
OpenTherm.h - OpenTherm Library for the ESP8266/Arduino platform
https://github.com/ihormelnyk/OpenTherm
http://ihormelnyk.com/pages/OpenTherm
Licensed under MIT license
Copyright 2018, Ihor Melnyk

Frame Structure:
P MGS-TYPE SPARE DATA-ID  DATA-VALUE
0 000      0000  00000000 00000000 00000000
*/

#ifndef OpenTherm_h
#define OpenTherm_h

#include <stdint.h>
#include <Arduino.h>

enum OpenThermResponseStatus {
	NONE,
	SUCCESS,
	INVALID,
	TIMEOUT
};


enum OpenThermMessageType {
	/*  Master to Slave */
	READ_DATA       = B000,
	READ            = READ_DATA, // for backwared compatibility
	WRITE_DATA      = B001,
	WRITE           = WRITE_DATA, // for backwared compatibility
	INVALID_DATA    = B010,
	RESERVED        = B011,
	/* Slave to Master */
	READ_ACK        = B100,
	WRITE_ACK       = B101,
	DATA_INVALID    = B110,
	UNKNOWN_DATA_ID = B111
};

typedef OpenThermMessageType OpenThermRequestType; // for backwared compatibility

enum OpenThermMessageID {
	Status, // flag8 / flag8  Master and Slave Status flags.
	TSet, // f8.8  Control setpoint  ie CH  water temperature setpoint (°C)
	MConfigMMemberIDcode, // flag8 / u8  Master Configuration Flags /  Master MemberID Code
	SConfigSMemberIDcode, // flag8 / u8  Slave Configuration Flags /  Slave MemberID Code
	Command, // u8 / u8  Remote Command
	ASFflags, // / OEM-fault-code  flag8 / u8  Application-specific fault flags and OEM fault code
	RBPflags, // flag8 / flag8  Remote boiler parameter transfer-enable & read/write flags
	CoolingControl, // f8.8  Cooling control signal (%)
	TsetCH2, // f8.8  Control setpoint for 2e CH circuit (°C)
	TrOverride, // f8.8  Remote override room setpoint
	TSP, // u8 / u8  Number of Transparent-Slave-Parameters supported by slave
	TSPindexTSPvalue, // u8 / u8  Index number / Value of referred-to transparent slave parameter.
	FHBsize, // u8 / u8  Size of Fault-History-Buffer supported by slave
	FHBindexFHBvalue, // u8 / u8  Index number / Value of referred-to fault-history buffer entry.
	MaxRelModLevelSetting, // f8.8  Maximum relative modulation level setting (%)
	MaxCapacityMinModLevel, // u8 / u8  Maximum boiler capacity (kW) / Minimum boiler modulation level(%)
	TrSet, // f8.8  Room Setpoint (°C)
	RelModLevel, // f8.8  Relative Modulation Level (%)
	CHPressure, // f8.8  Water pressure in CH circuit  (bar)
	DHWFlowRate, // f8.8  Water flow rate in DHW circuit. (litres/minute)
	DayTime, // special / u8  Day of Week and Time of Day
	Date, // u8 / u8  Calendar date
	Year, // u16  Calendar year
	TrSetCH2, // f8.8  Room Setpoint for 2nd CH circuit (°C)
	Tr, // f8.8  Room temperature (°C)
	Tboiler, // f8.8  Boiler flow water temperature (°C)
	Tdhw, // f8.8  DHW temperature (°C)
	Toutside, // f8.8  Outside temperature (°C)
	Tret, // f8.8  Return water temperature (°C)
	Tstorage, // f8.8  Solar storage temperature (°C)
	Tcollector, // f8.8  Solar collector temperature (°C)
	TflowCH2, // f8.8  Flow water temperature CH2 circuit (°C)
	Tdhw2, // f8.8  Domestic hot water temperature 2 (°C)
	Texhaust, // s16  Boiler exhaust temperature (°C)
	TdhwSetUBTdhwSetLB = 48, // s8 / s8  DHW setpoint upper & lower bounds for adjustment  (°C)
	MaxTSetUBMaxTSetLB, // s8 / s8  Max CH water setpoint upper & lower bounds for adjustment  (°C)
	HcratioUBHcratioLB, // s8 / s8  OTC heat curve ratio upper & lower bounds for adjustment
	TdhwSet = 56, // f8.8  DHW setpoint (°C)    (Remote parameter 1)
	MaxTSet, // f8.8  Max CH water setpoint (°C)  (Remote parameters 2)
	Hcratio, // f8.8  OTC heat curve ratio (°C)  (Remote parameter 3)
	RemoteOverrideFunction = 100, // flag8 / -  Function of manual and program changes in master and remote room setpoint.
	OEMDiagnosticCode = 115, // u16  OEM-specific diagnostic/service code
	BurnerStarts, // u16  Number of starts burner
	CHPumpStarts, // u16  Number of starts CH pump
	DHWPumpValveStarts, // u16  Number of starts DHW pump/valve
	DHWBurnerStarts, // u16  Number of starts burner during DHW mode
	BurnerOperationHours, // u16  Number of hours that burner is in operation (i.e. flame on)
	CHPumpOperationHours, // u16  Number of hours that CH pump has been running
	DHWPumpValveOperationHours, // u16  Number of hours that DHW pump has been running or DHW valve has been opened
	DHWBurnerOperationHours, // u16  Number of hours that burner is in operation during DHW mode
	OpenThermVersionMaster, // f8.8  The implemented version of the OpenTherm Protocol Specification in the master.
	OpenThermVersionSlave, // f8.8  The implemented version of the OpenTherm Protocol Specification in the slave.
	MasterVersion, // u8 / u8  Master product version number and type
	SlaveVersion, // u8 / u8  Slave product version number and type
};

enum OpenThermStatus {
	NOT_INITIALIZED,
	READY,
	DELAY,
	REQUEST_SENDING,
	RESPONSE_WAITING,
	RESPONSE_START_BIT,
	RESPONSE_RECEIVING,
	RESPONSE_READY,
	RESPONSE_INVALID
};

class OpenTherm
{
public:
	OpenTherm(int inPin = 4, int outPin = 5, bool isSlave = false);
	volatile OpenThermStatus status;
	void begin(void(*handleInterruptCallback)(void));
	void begin(void(*handleInterruptCallback)(void), void(*processResponseCallback)(unsigned long, int));
	bool isReady();
	unsigned long sendRequest(unsigned long request);
	bool sendResponse(unsigned long request);
	bool sendRequestAync(unsigned long request);
	static unsigned long buildRequest(OpenThermMessageType type, OpenThermMessageID id, unsigned int data);
	static unsigned long buildResponse(OpenThermMessageType type, OpenThermMessageID id, unsigned int data);
	OpenThermResponseStatus getLastResponseStatus();
	const char *statusToString(OpenThermResponseStatus status);
	void handleInterrupt();
	void process();
	void end();

	static bool parity(unsigned long frame);
	OpenThermMessageType getMessageType(unsigned long message);
	OpenThermMessageID getDataID(unsigned long frame);
	const char *messageTypeToString(OpenThermMessageType message_type);
	bool isValidRequest(unsigned long request);
	bool isValidResponse(unsigned long response);

	//requests
	unsigned long buildSetBoilerStatusRequest(bool enableCentralHeating, bool enableHotWater = false, bool enableCooling = false, bool enableOutsideTemperatureCompensation = false, bool enableCentralHeating2 = false);
	unsigned long buildSetBoilerTemperatureRequest(float temperature);
	unsigned long buildGetBoilerTemperatureRequest();
	unsigned long buildSetHotWaterTemperatureRequest(float temperature);
	unsigned long buildSlaveConfigurationRequest();


	//responses
	static bool isFault(unsigned long response);
	static bool isCentralHeatingActive(unsigned long response);
	static bool isHotWaterActive(unsigned long response);
	static bool isFlameOn(unsigned long response);
	static bool isCoolingActive(unsigned long response);
	static bool isDiagnostic(unsigned long response);
	static uint16_t getUInt(const unsigned long response);
	static float getFloat(const unsigned long response);
	static unsigned int temperatureToData(float temperature);

	//basic requests
	unsigned long setBoilerStatus(bool enableCentralHeating, bool enableHotWater = false, bool enableCooling = false, bool enableOutsideTemperatureCompensation = false, bool enableCentralHeating2 = false);
	bool setBoilerTemperature(float temperature);
	bool setHotWaterTemperature(float temperature);
	float getBoilerTemperature();
    float getReturnTemperature();
    float getModulation();
    float getPressure();
    unsigned char getFault();
	unsigned long getSlaveConfiguration();

private:
	const int inPin;
	const int outPin;
	const bool isSlave;

	volatile unsigned long response;
	volatile OpenThermResponseStatus responseStatus;
	volatile unsigned long responseTimestamp;
	volatile byte responseBitIndex;

	int readState();
	void setActiveState();
	void setIdleState();
	void activateBoiler();

	void sendBit(bool high);
	void(*handleInterruptCallback)();
	void(*processResponseCallback)(unsigned long, int);
};

#ifndef ICACHE_RAM_ATTR
#define ICACHE_RAM_ATTR
#endif

#endif // OpenTherm_h