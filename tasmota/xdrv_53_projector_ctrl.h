#ifndef _XDRV53_PROJECTOR_CTRL_H_
#define _XDRV53_PROJECTOR_CTRL_H_

//define RELAY that is forced ON while projector is running
#ifndef PROJECTOR_CTRL_PWR_BY_RELAY
#define PROJECTOR_CTRL_PWR_BY_RELAY 1
#endif //PROJECTOR_CTRL_PWR_BY_RELAY

#define PROJECTOR_CTRL_SERIAL_TIMEOUT 10 //up to 254 seconds

#ifndef PROJECTOR_CTRL_SERIAL_BAUDRATE
#define PROJECTOR_CTRL_SERIAL_BAUDRATE 9600
#endif //PROJECTOR_CTRL_SERIAL_BAUDRATE

#ifdef USE_PROJECTOR_CTRL_NEC
/* see the serial codes in
 * https://www.sharpnecdisplays.eu/p/download/v/5e14a015e26cacae3ae64a422f7f8af4/cp/Products/Projectors/Shared/CommandLists/PDF-ExternalControlManual-english.pdf#page=5
 * tested with NEC V300W
 * final cksum byte is appended automatically
 */
#define PROJECTOR_CTRL_LOGNAME	"DLP[NEC]"
static const uint8_t projector_ctrl_msg_qry_typ[] = { 0x00, 0xBF, 0x00, 0x00, 0x01, 0x00 }; //page81
static const uint8_t projector_ctrl_msg_qry_pwr[] = { 0x00, 0xBF, 0x00, 0x00, 0x01, 0x02 }; //page83
static const uint8_t projector_ctrl_msg_pwr_on[] = { 0x02, 0x00, 0x00, 0x00, 0x00 };        //page15
static const uint8_t projector_ctrl_msg_pwr_off[] = { 0x02, 0x01, 0x00, 0x00, 0x00 };       //page16
static const struct projector_ctrl_command_info_s projector_ctrl_commands[] = {
	{PROJECTOR_CTRL_S_QRY_TYPE, &projector_ctrl_msg_qry_typ[0], sizeof(projector_ctrl_msg_qry_typ),
								PROJECTOR_CTRL_SERIAL_TIMEOUT, 0x20, 22, 8, 4, 0xA0, 8, 5, 2},
	{PROJECTOR_CTRL_S_QRY_PWR,  &projector_ctrl_msg_qry_pwr[0], sizeof(projector_ctrl_msg_qry_pwr),
								PROJECTOR_CTRL_SERIAL_TIMEOUT, 0x20, 22, 6, 1, 0xA0, 8, 5, 2},
	{PROJECTOR_CTRL_S_PWR_ON,   &projector_ctrl_msg_pwr_on[0], sizeof(projector_ctrl_msg_pwr_on),
								PROJECTOR_CTRL_SERIAL_TIMEOUT, 0x22, 6, 0, 1, 0xA2, 8, 5, 2},
	{PROJECTOR_CTRL_S_PWR_OFF,  &projector_ctrl_msg_pwr_off[0], sizeof(projector_ctrl_msg_pwr_off),
								PROJECTOR_CTRL_SERIAL_TIMEOUT, 0x22, 6, 0, 1, 0xA2, 8, 5, 2}
};
#define PROJECTOR_CTRL_QRYPWR_ON      0x04
#define PROJECTOR_CTRL_QRYPWR_COOLING 0x05
#define PROJECTOR_CTRL_QRYPWR_STARTING 0x01 //undocumented state, seen on V300W
#define PROJECTOR_CTRL_QRYPWR_WARMING 0x02  //undocumented state, seen on V300W


#elif defined(USE_PROJECTOR_CTRL_OPTOMA)
/* see the serial codes in
 * https://www.optoma.co.uk/uploads/manuals/hd36-m-en-gb.pdf#page=56
 * not really tested with OPTOMA devices
 */
#define PROJECTOR_CTRL_LOGNAME	"DLP[OPTOMA]"
static const uint8_t projector_ctrl_msg_qry_typ[] = { 0x7e, 0x30, 0x30, 0x31, 0x32, 0x32, 0x20, 0x31, 0x0d }; //page59
static const uint8_t projector_ctrl_msg_qry_pwr[] = { 0x7e, 0x30, 0x30, 0x31, 0x32, 0x34, 0x20, 0x31, 0x0d }; //page59
static const uint8_t projector_ctrl_msg_pwr_on[] = { 0x7e, 0x30, 0x30, 0x30, 0x30, 0x20, 0x31, 0x0d };        //page56
static const uint8_t projector_ctrl_msg_pwr_off[] = { 0x7e, 0x30, 0x30, 0x30, 0x30, 0x20, 0x30, 0x0d };       //page56
static const struct projector_ctrl_command_info_s projector_ctrl_commands[] = {
	{PROJECTOR_CTRL_S_QRY_TYPE, &projector_ctrl_msg_qry_typ[0], sizeof(projector_ctrl_msg_qry_typ),
								PROJECTOR_CTRL_SERIAL_TIMEOUT, 'O', 6, 2, 4, 'I', 5, 4, 1},
	{PROJECTOR_CTRL_S_QRY_PWR,  &projector_ctrl_msg_qry_pwr[0], sizeof(projector_ctrl_msg_qry_pwr),
								PROJECTOR_CTRL_SERIAL_TIMEOUT, 'O', 3, 2, 1, 'I', 5, 4, 1},
	{PROJECTOR_CTRL_S_PWR_ON,   &projector_ctrl_msg_pwr_on[0], sizeof(projector_ctrl_msg_pwr_on),
								PROJECTOR_CTRL_SERIAL_TIMEOUT, 'P', 1, 0, 1, 'F', 1, 0, 1},
	{PROJECTOR_CTRL_S_PWR_OFF,  &projector_ctrl_msg_pwr_off[0], sizeof(projector_ctrl_msg_pwr_off),
								PROJECTOR_CTRL_SERIAL_TIMEOUT, 'P', 1, 0, 1, 'F', 1, 0, 1}
};
#define PROJECTOR_CTRL_QRYPWR_ON      0x31
#define PROJECTOR_CTRL_QRYPWR_COOLING 0x31 //placebo
#define PROJECTOR_CTRL_QRYPWR_STARTING 0x31 //placebo
#define PROJECTOR_CTRL_QRYPWR_WARMING 0x31 //placebo


#else
#error USE_PROJECTOR_CTRL: No projector type defined
#endif


#endif //_XDRV53_PROJECTOR_CTRL_H_