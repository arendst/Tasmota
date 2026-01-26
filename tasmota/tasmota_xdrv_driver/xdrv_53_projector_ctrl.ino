/*
  xdrv_53_projector_ctrl.ino - LCD/DLP Projector Serial Control support for Tasmota

  Copyright (C) 2021  Jan Bubík <jbubik@centrum.cz>
  Written with the gifts I got from Jesus.

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

#ifdef USE_PROJECTOR_CTRL
/*********************************************************************************************\
 * LCD/DLP Projector Control via serial interface
 * https://www.sharpnecdisplays.eu/p/download/v/5e14a015e26cacae3ae64a422f7f8af4/cp/Products/Projectors/Shared/CommandLists/PDF-ExternalControlManual-english.pdf#page=5
 * https://www.optoma.co.uk/uploads/manuals/hd36-m-en-gb.pdf#page=56
 * http://global-download.acer.com/GDFiles/Document/RS232%20Command%20Table/RS232%20Command%20Table_Acer_1.0_A_A.zip?acerid=636791605984811687
\*********************************************************************************************/

#define XDRV_53			53

#if !defined(USE_PROJECTOR_CTRL_NEC) && !defined(USE_PROJECTOR_CTRL_OPTOMA) && !defined(USE_PROJECTOR_CTRL_ACER)
#define USE_PROJECTOR_CTRL_NEC                 // Use at least one projector
#endif

#define nitems(_a)		(sizeof((_a)) / sizeof((_a)[0]))
#define xxstr(s) xstr(s)
#define xstr(s) #s

enum projector_ctrl_dev_state_e : uint8_t {
	PROJECTOR_CTRL_DEV_UNKNOWN=0,
	PROJECTOR_CTRL_DEV_PWR_OFF,
	PROJECTOR_CTRL_DEV_PWR_ON
};

enum projector_ctrl_serial_state_e : uint8_t {
	PROJECTOR_CTRL_S_UNCONNECTED=0,
	PROJECTOR_CTRL_S_QRY_PWR,
	PROJECTOR_CTRL_S_QRY_TYPE,
	PROJECTOR_CTRL_S_IDLE,
	PROJECTOR_CTRL_S_PWR_ON,
	PROJECTOR_CTRL_S_PWR_OFF
};

enum projector_ctrl_serial_result_e : uint8_t {
	PROJECTOR_CTRL_R_UNKNOWN=0,
	PROJECTOR_CTRL_R_PASS,
	PROJECTOR_CTRL_R_FAIL
};

struct projector_ctrl_command_info_s {
	const enum projector_ctrl_serial_state_e command;
	const uint8_t  *send_codes;
	const uint8_t   send_len;
	const uint8_t   timeout_ticks;
	const uint8_t   pass_first_byte;
	const uint8_t   pass_len;
	const uint8_t   pass_value_offset;
	const uint8_t   pass_value_bytes;
	const uint8_t   fail_first_byte;
	const uint8_t   fail_len;
	const uint8_t   fail_value_offset;
	const uint8_t   fail_value_bytes;
} __packed;

#include "include/xdrv_53_projector_ctrl.h"

struct projector_ctrl_softc_s {
	TasmotaSerial	              *sc_serial;
	uint8_t                        sc_device;
	uint8_t	                       sc_ticks;
	enum projector_ctrl_dev_state_e     sc_dev_state;
	enum projector_ctrl_serial_state_e  sc_ser_state;
	enum projector_ctrl_serial_result_e sc_ser_result;
	enum projector_ctrl_serial_state_e  sc_ser_next_cmd;
	const struct projector_ctrl_command_info_s *sc_cmd_info;
	uint8_t			               sc_ser_sum;
	uint8_t			               sc_ser_len;
	uint32_t			           sc_ser_value;
} __packed;

static struct projector_ctrl_softc_s	*projector_ctrl_sc = nullptr;



static void
projector_ctrl_pre_init(void)
{
	struct projector_ctrl_softc_s *sc;
	int baudrate = PROJECTOR_CTRL_SERIAL_BAUDRATE;

	if (!PinUsed(GPIO_PROJECTOR_CTRL_TX) || !PinUsed(GPIO_PROJECTOR_CTRL_RX))
		return;

	sc = (struct projector_ctrl_softc_s *)malloc(sizeof(*sc));
	if (sc == NULL) {
		AddLog(LOG_LEVEL_ERROR, PSTR(PROJECTOR_CTRL_LOGNAME ": unable to allocate state"));
		return;
	}

	memset(sc, 0, sizeof(*sc));

	sc->sc_serial = new TasmotaSerial(Pin(GPIO_PROJECTOR_CTRL_RX),
	    Pin(GPIO_PROJECTOR_CTRL_TX), 2);

	if (!sc->sc_serial->begin(baudrate)) {
		AddLog(LOG_LEVEL_ERROR, PSTR(PROJECTOR_CTRL_LOGNAME ": unable to begin serial "
		    "(baudrate %d)"), baudrate);
		goto del;
	}

	if (sc->sc_serial->hardwareSerial()) {
		ClaimSerial();
		SetSerial(baudrate, TS_SERIAL_8N1);
	}
#ifdef ESP32
    AddLog(LOG_LEVEL_DEBUG, PSTR(PROJECTOR_CTRL_LOGNAME ": Serial UART%d"), sc->sc_serial->getUart());
#endif

  UpdateDevicesPresent(1); /* claim a POWER device slot */
	sc->sc_device = TasmotaGlobal.devices_present;

	AddLog(LOG_LEVEL_INFO, PSTR(PROJECTOR_CTRL_LOGNAME ": new RELAY%d, polling serial for Projector status"), sc->sc_device);

	projector_ctrl_sc = sc;
	return;
del:
	delete sc->sc_serial;
free:
	free(sc);
}



static void
projector_ctrl_write(struct projector_ctrl_softc_s *sc, const uint8_t *bytes, const size_t len)
{
	TasmotaSerial *serial;
	uint8_t cksum;
	size_t i;

	cksum = 0;
	serial = sc->sc_serial;

	for (i = 0; i < len; i++) {
		uint8_t b = bytes[i];
		serial->write(b);
		cksum += b;
	}
#ifdef USE_PROJECTOR_CTRL_NEC
	serial->write(cksum);
#ifdef DEBUG_PROJECTOR_CTRL
	char hex_b[(len + 1) * 2];
	AddLog(LOG_LEVEL_DEBUG,PSTR(PROJECTOR_CTRL_LOGNAME ": RAW bytes %s %02x"),
	    ToHex_P((uint8_t *)bytes, len, hex_b, sizeof(hex_b)), cksum);
#endif //DEBUG_PROJECTOR_CTRL
#else  //!USE_PROJECTOR_CTRL_NEC
#ifdef DEBUG_PROJECTOR_CTRL
	char hex_b[(len + 1) * 2];
	AddLog(LOG_LEVEL_DEBUG,PSTR(PROJECTOR_CTRL_LOGNAME ": RAW bytes %s"),
	    ToHex_P((uint8_t *)bytes, len, hex_b, sizeof(hex_b)));
#endif //DEBUG_PROJECTOR_CTRL
#endif //!USE_PROJECTOR_CTRL_NEC

	serial->flush();
	return;
}



static void
projector_ctrl_request(struct projector_ctrl_softc_s *sc, const uint8_t command)
{
	const struct projector_ctrl_command_info_s *e;
	size_t i;

	if ((sc->sc_dev_state!=PROJECTOR_CTRL_DEV_UNKNOWN)&&(sc->sc_ser_state!=PROJECTOR_CTRL_S_IDLE)) {
		if ((command!=PROJECTOR_CTRL_S_QRY_PWR)&&(command!=PROJECTOR_CTRL_S_QRY_TYPE)) {
		sc->sc_ser_next_cmd=(projector_ctrl_serial_state_e)command;
		AddLog(LOG_LEVEL_INFO, PSTR(PROJECTOR_CTRL_LOGNAME
			": Serial CMD %02x already running, enqueueing next (%02x)"), sc->sc_ser_state, command);
		};
		return;
	};

	for (i = 0; i < nitems(projector_ctrl_commands); i++) {
		e = &projector_ctrl_commands[i];
		if (command == e->command){
			sc->sc_cmd_info=e;
			sc->sc_ser_len=0;
			sc->sc_ser_result=PROJECTOR_CTRL_R_UNKNOWN;
			sc->sc_ser_state=(projector_ctrl_serial_state_e)command;
			sc->sc_ser_sum=0;
			sc->sc_ser_next_cmd=PROJECTOR_CTRL_S_UNCONNECTED;
			sc->sc_ticks=0;
#ifdef DEBUG_PROJECTOR_CTRL
			AddLog(LOG_LEVEL_DEBUG, PSTR(PROJECTOR_CTRL_LOGNAME
				": Sending CMD %02x"), command);
#endif //DEBUG_PROJECTOR_CTRL
			projector_ctrl_write(sc,e->send_codes,e->send_len);
			return;
		}
	};
#ifdef DEBUG_PROJECTOR_CTRL
	AddLog(LOG_LEVEL_DEBUG, PSTR(PROJECTOR_CTRL_LOGNAME
		": Undefined serial command %02x"), command);
#endif //DEBUG_PROJECTOR_CTRL
	return;
}



static uint8_t
projector_ctrl_parse(struct projector_ctrl_softc_s *sc, const uint8_t byte)
{
	enum projector_ctrl_serial_state_e nstate;
	const struct projector_ctrl_command_info_s *cmd;

	nstate = sc->sc_ser_state;

	switch (nstate) {
	case PROJECTOR_CTRL_S_IDLE:
	case PROJECTOR_CTRL_S_UNCONNECTED:
#ifdef DEBUG_PROJECTOR_CTRL
		AddLog(LOG_LEVEL_DEBUG, PSTR(PROJECTOR_CTRL_LOGNAME
			   ": Spurious input in state %02x, got %02x, going UNCONNECTED"), nstate, byte);
#endif //DEBUG_PROJECTOR_CTRL
		return(PROJECTOR_CTRL_S_UNCONNECTED);

	default:
		cmd=sc->sc_cmd_info;
		sc->sc_ser_len++;
		if (sc->sc_ser_len==1) {
			if (byte==cmd->pass_first_byte){
				sc->sc_ser_result=PROJECTOR_CTRL_R_PASS;
#ifdef DEBUG_PROJECTOR_CTRL
				AddLog(LOG_LEVEL_DEBUG, PSTR(PROJECTOR_CTRL_LOGNAME
				   ": CMD %02x PASS, 1st byte %02x"), nstate, byte);
#endif //DEBUG_PROJECTOR_CTRL
			}else if (byte==cmd->fail_first_byte){
				sc->sc_ser_result=PROJECTOR_CTRL_R_FAIL;
#ifdef DEBUG_PROJECTOR_CTRL
				AddLog(LOG_LEVEL_DEBUG, PSTR(PROJECTOR_CTRL_LOGNAME
				   ": CMD %02x FAIL, 1st byte %02x"), nstate, byte);
#endif //DEBUG_PROJECTOR_CTRL
			}else{
#ifdef DEBUG_PROJECTOR_CTRL
				AddLog(LOG_LEVEL_DEBUG, PSTR(PROJECTOR_CTRL_LOGNAME
				   ": CMD %02x UNKNOWN, 1st byte %02x, going UNCONNECTED"), nstate, byte);
#endif //DEBUG_PROJECTOR_CTRL
				return(PROJECTOR_CTRL_S_UNCONNECTED);
			};
		};
		if (sc->sc_ser_result==PROJECTOR_CTRL_R_PASS){
			if (sc->sc_ser_len==(cmd->pass_value_offset+1))
				sc->sc_ser_value=byte;
			if ((sc->sc_ser_len>(cmd->pass_value_offset+1))&&(sc->sc_ser_len<=(cmd->pass_value_offset+cmd->pass_value_bytes)))
				sc->sc_ser_value=(sc->sc_ser_value<<8)|byte;
			if (sc->sc_ser_len==cmd->pass_len){
#ifdef USE_PROJECTOR_CTRL_NEC
				if(sc->sc_ser_sum!=byte){
#ifdef DEBUG_PROJECTOR_CTRL
					AddLog(LOG_LEVEL_DEBUG, PSTR(PROJECTOR_CTRL_LOGNAME
						": Failed cksum for CMD %02x. Got %02x bytes, computed cksum: %02x, recevied cksum: %02x, going UNCONNECTED"),
						nstate, sc->sc_ser_len, sc->sc_ser_sum, byte);
#endif	//DEBUG_PROJECTOR_CTRL
					nstate=PROJECTOR_CTRL_S_UNCONNECTED;
				} else
#endif	//USE_PROJECTOR_CTRL_NEC
				{
#ifdef DEBUG_PROJECTOR_CTRL
					AddLog(LOG_LEVEL_DEBUG, PSTR(PROJECTOR_CTRL_LOGNAME
						": CMD %02x PASS, got %02x bytes, retval %02x, going IDLE"), nstate, sc->sc_ser_len, sc->sc_ser_value);
#endif //DEBUG_PROJECTOR_CTRL
					nstate=PROJECTOR_CTRL_S_IDLE;
				};
			};
		};

		if (sc->sc_ser_result==PROJECTOR_CTRL_R_FAIL) {
			if (sc->sc_ser_len==(cmd->fail_value_offset+1))
				sc->sc_ser_value=byte;
			if ((sc->sc_ser_len>(cmd->fail_value_offset+1))&&(sc->sc_ser_len<=(cmd->fail_value_offset+cmd->fail_value_bytes)))
				sc->sc_ser_value=(sc->sc_ser_value<<8)|byte;
			if(sc->sc_ser_len==cmd->fail_len){
#ifdef USE_PROJECTOR_CTRL_NEC
				if(sc->sc_ser_sum!=byte){
#ifdef DEBUG_PROJECTOR_CTRL
					AddLog(LOG_LEVEL_DEBUG, PSTR(PROJECTOR_CTRL_LOGNAME
						": Failed cksum for CMD %02x. Got %02x bytes, computed cksum: %02x, receied cksum: %02x, going UNCONNECTED"),
						nstate, sc->sc_ser_len, sc->sc_ser_sum, byte);
#endif	//DEBUG_PROJECTOR_CTRL
					nstate=PROJECTOR_CTRL_S_UNCONNECTED;
				} else
#endif	//USE_PROJECTOR_CTRL_NEC
				{
#ifdef DEBUG_PROJECTOR_CTRL
					AddLog(LOG_LEVEL_DEBUG, PSTR(PROJECTOR_CTRL_LOGNAME
						": CMD %02x FAIL, got %02x bytes, retval %02x, going idle"), nstate, sc->sc_ser_len, sc->sc_ser_value);
#endif //DEBUG_PROJECTOR_CTRL
					nstate=PROJECTOR_CTRL_S_IDLE;
				};
			};
		};

#ifdef USE_PROJECTOR_CTRL_NEC
		sc->sc_ser_sum += byte;
#endif //USE_PROJECTOR_CTRL_NEC

		break;
	}
	return (nstate);
}



static void
projector_ctrl_loop(struct projector_ctrl_softc_s *sc)
{
	TasmotaSerial *serial;
	uint8_t      oldstate;
	serial = sc->sc_serial;

	while (serial->available()) {
		yield();
        oldstate = sc->sc_ser_state;
		switch (sc->sc_ser_state = (projector_ctrl_serial_state_e)projector_ctrl_parse(sc, serial->read())) {
			case PROJECTOR_CTRL_S_UNCONNECTED:
				if (sc->sc_dev_state!=PROJECTOR_CTRL_DEV_UNKNOWN){
					sc->sc_dev_state=PROJECTOR_CTRL_DEV_UNKNOWN;
					AddLog(LOG_LEVEL_INFO,PSTR(PROJECTOR_CTRL_LOGNAME ": DISCONNECTED(unexpected input)"));
				}
				break;
			case PROJECTOR_CTRL_S_IDLE:
				if ((oldstate==PROJECTOR_CTRL_S_QRY_PWR)&&(sc->sc_ser_result==PROJECTOR_CTRL_R_PASS)){
					if(sc->sc_dev_state==PROJECTOR_CTRL_DEV_UNKNOWN){
						AddLog(LOG_LEVEL_INFO,PSTR(PROJECTOR_CTRL_LOGNAME ": CONNECTED"));
					};
					if((  (sc->sc_ser_value==PROJECTOR_CTRL_QRYPWR_ON)
					    ||(sc->sc_ser_value==PROJECTOR_CTRL_QRYPWR_COOLING)
					    ||(sc->sc_ser_value==PROJECTOR_CTRL_QRYPWR_STARTING)
					    ||(sc->sc_ser_value==PROJECTOR_CTRL_QRYPWR_WARMING)
					   )&&(sc->sc_dev_state!=PROJECTOR_CTRL_DEV_PWR_ON)){
						sc->sc_dev_state=PROJECTOR_CTRL_DEV_PWR_ON;
						ExecuteCommandPower(sc->sc_device, POWER_ON, SRC_IGNORE);
					};
					if((  (sc->sc_ser_value!=PROJECTOR_CTRL_QRYPWR_ON)
					    &&(sc->sc_ser_value!=PROJECTOR_CTRL_QRYPWR_COOLING)
					    &&(sc->sc_ser_value!=PROJECTOR_CTRL_QRYPWR_STARTING)
					    &&(sc->sc_ser_value!=PROJECTOR_CTRL_QRYPWR_WARMING)
					   )&&(sc->sc_dev_state!=PROJECTOR_CTRL_DEV_PWR_OFF)){
						sc->sc_dev_state=PROJECTOR_CTRL_DEV_PWR_OFF;
						ExecuteCommandPower(sc->sc_device, POWER_OFF, SRC_IGNORE);
					};
				};
				if ((oldstate==PROJECTOR_CTRL_S_PWR_ON)&&(sc->sc_ser_result==PROJECTOR_CTRL_R_PASS))
					sc->sc_dev_state=PROJECTOR_CTRL_DEV_PWR_ON;
				if ((oldstate==PROJECTOR_CTRL_S_PWR_OFF)&&(sc->sc_ser_result==PROJECTOR_CTRL_R_PASS))
					sc->sc_dev_state=PROJECTOR_CTRL_DEV_PWR_OFF;
				if(sc->sc_ser_next_cmd!=PROJECTOR_CTRL_S_UNCONNECTED){
					oldstate=sc->sc_ser_next_cmd;
					sc->sc_ser_next_cmd=PROJECTOR_CTRL_S_UNCONNECTED;
					projector_ctrl_request(sc,oldstate);
				};
				break;
		};
	}
}



static void
projector_ctrl_connect(struct projector_ctrl_softc_s *sc)
{
	projector_ctrl_request(sc,PROJECTOR_CTRL_S_QRY_PWR);
}



static void
projector_ctrl_tick(struct projector_ctrl_softc_s *sc)
{
	if(sc->sc_ser_state==PROJECTOR_CTRL_S_IDLE){
		switch (TasmotaGlobal.uptime&0xf) {
			case 0:
				projector_ctrl_request(sc,PROJECTOR_CTRL_S_QRY_PWR);
				break;
			case 8:
				projector_ctrl_request(sc,PROJECTOR_CTRL_S_QRY_TYPE);
				break;
		};
	}else if(sc->sc_ticks > sc->sc_cmd_info->timeout_ticks){
		//current CMD has ran out of time, drop connection
		AddLog(LOG_LEVEL_INFO,PSTR(PROJECTOR_CTRL_LOGNAME ": DISCONNECTED(timeout)"));
		sc->sc_dev_state=PROJECTOR_CTRL_DEV_UNKNOWN;
		sc->sc_ser_state=PROJECTOR_CTRL_S_UNCONNECTED;
	};
}



static bool
projector_ctrl_set_power(struct projector_ctrl_softc_s *sc)
{
	if (TasmotaGlobal.active_device==PROJECTOR_CTRL_PWR_BY_RELAY){
		if ((sc->sc_dev_state == PROJECTOR_CTRL_DEV_PWR_ON) && (0==bitRead(XdrvMailbox.index, PROJECTOR_CTRL_PWR_BY_RELAY -1))) {
			TasmotaGlobal.power = bitSet(TasmotaGlobal.power,PROJECTOR_CTRL_PWR_BY_RELAY -1);
			AddLog(LOG_LEVEL_INFO,PSTR(PROJECTOR_CTRL_LOGNAME ": Keep RELAY" xxstr(PROJECTOR_CTRL_PWR_BY_RELAY) " ON"));
		} else {
			return(false);
		};
	} else if (TasmotaGlobal.active_device==sc->sc_device){
		if (bitRead(XdrvMailbox.index, sc->sc_device -1)) {
			switch (sc->sc_dev_state) {
			case PROJECTOR_CTRL_DEV_UNKNOWN:
				TasmotaGlobal.power = bitClear(TasmotaGlobal.power,sc->sc_device -1);
				break;
			case PROJECTOR_CTRL_DEV_PWR_OFF:
				projector_ctrl_request(sc,PROJECTOR_CTRL_S_PWR_ON);
				break;
			};
		}else{
			if (sc->sc_dev_state == PROJECTOR_CTRL_DEV_PWR_ON)
				projector_ctrl_request(sc,PROJECTOR_CTRL_S_PWR_OFF);
		};
	} else {
		return(false);
	};
	return (true);
}


/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv53(uint32_t function) {
	bool result;
	struct projector_ctrl_softc_s *sc;

	result = false;
	sc = projector_ctrl_sc;

	switch (function) {
	case FUNC_PRE_INIT:
		projector_ctrl_pre_init();
		return (false);
	}

	if (sc == NULL)
		return (false);

	switch (function) {
	case FUNC_LOOP:
		projector_ctrl_loop(sc);
		break;

	case FUNC_EVERY_SECOND:
		sc->sc_ticks++;
		if (sc->sc_dev_state!=PROJECTOR_CTRL_DEV_UNKNOWN)
			projector_ctrl_tick(sc);
		else if ((TasmotaGlobal.uptime&0x7)==0) //each 8 seconds
			projector_ctrl_connect(sc);
		break;

	case FUNC_SET_DEVICE_POWER:
		result = projector_ctrl_set_power(sc);
		break;

    case FUNC_ACTIVE:
        result = true;
        break;

	}

	return (result);
}

#endif  // USE_PROJECTOR_CTRL