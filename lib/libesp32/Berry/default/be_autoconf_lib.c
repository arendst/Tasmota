/********************************************************************
 * Tasmota lib
 * 
 * To use: `import autoconf`
 * 
 *******************************************************************/
#include "be_constobj.h"


/********************************************************************
** Solidified function: page_autoconf_ctl
********************************************************************/
be_local_closure(page_autoconf_ctl,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[41]) {     /* constants */
    /* K0   */  be_nested_string("webserver", 1572454038, 9),
    /* K1   */  be_nested_string("string", 398550328, 6),
    /* K2   */  be_nested_string("path", -2071507658, 4),
    /* K3   */  be_nested_string("check_privileged_access", -602033328, 23),
    /* K4   */  be_nested_string("has_arg", 424878688, 7),
    /* K5   */  be_nested_string("reapply", -516027964, 7),
    /* K6   */  be_nested_string("tasmota", 424643812, 7),
    /* K7   */  be_nested_string("log", 1062293841, 3),
    /* K8   */  be_nested_string("CFG: removing first time marker", 2125556683, 31),
    /* K9   */  be_const_int(2),
    /* K10  */  be_nested_string("clear_first_time", 632769909, 16),
    /* K11  */  be_nested_string("redirect", 389758641, 8),
    /* K12  */  be_nested_string("/?rst=", 580074707, 6),
    /* K13  */  be_nested_string("zip", -1417514060, 3),
    /* K14  */  be_nested_string("CFG: removing autoconf files", -280262326, 28),
    /* K15  */  be_nested_string("delete_all_configs", -1912899718, 18),
    /* K16  */  be_nested_string("arg", 1047474471, 3),
    /* K17  */  be_nested_string("reset", 1695364032, 5),
    /* K18  */  be_nested_string("format", -1180859054, 6),
    /* K19  */  be_nested_string("https://raw.githubusercontent.com/tasmota/autoconf/main/%s/%s.autoconf", -1551440987, 70),
    /* K20  */  be_nested_string("arch", -1342162999, 4),
    /* K21  */  be_nested_string("CFG: downloading '%s'", 589480701, 21),
    /* K22  */  be_nested_string("%s.autoconf", -734583772, 11),
    /* K23  */  be_nested_string("webclient", -218578150, 9),
    /* K24  */  be_nested_string("begin", 1748273790, 5),
    /* K25  */  be_nested_string("GET", -1763262857, 3),
    /* K26  */  be_nested_string("return code=%i", 2127454401, 14),
    /* K27  */  be_nested_string("connection_error", 1358926260, 16),
    /* K28  */  be_nested_string("write_file", -1117308417, 10),
    /* K29  */  be_nested_string("close", 667630371, 5),
    /* K30  */  be_nested_string("value_error", 773297791, 11),
    /* K31  */  be_nested_string("Unknown command", 1830905432, 15),
    /* K32  */  be_nested_string("CFG: Exception> '%s' - %s", 1228874553, 25),
    /* K33  */  be_nested_string("content_start", -1357458227, 13),
    /* K34  */  be_nested_string("Parameter error", -454925258, 15),
    /* K35  */  be_nested_string("content_send_style", 1087907647, 18),
    /* K36  */  be_nested_string("content_send", 1673733649, 12),
    /* K37  */  be_nested_string("<p style='width:340px;'><b>Exception:</b><br>'%s'<br>%s</p>", -42402214, 59),
    /* K38  */  be_nested_string("content_button", 1956476087, 14),
    /* K39  */  be_nested_string("BUTTON_CONFIGURATION", 70820856, 20),
    /* K40  */  be_nested_string("content_stop", 658554751, 12),
    }),
    (be_nested_const_str("page_autoconf_ctl", -1841585800, 17)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[117]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0xA40E0400,  //  0002  IMPORT	R3	K2
      0x8C100303,  //  0003  GETMET	R4	R1	K3
      0x7C100200,  //  0004  CALL	R4	1
      0x74120001,  //  0005  JMPT	R4	#0008
      0x4C100000,  //  0006  LDNIL	R4
      0x80040800,  //  0007  RET	1	R4
      0xA802004E,  //  0008  EXBLK	0	#0058
      0x8C100304,  //  0009  GETMET	R4	R1	K4
      0x58180005,  //  000A  LDCONST	R6	K5
      0x7C100400,  //  000B  CALL	R4	2
      0x7812000A,  //  000C  JMPF	R4	#0018
      0xB8120C00,  //  000D  GETNGBL	R4	K6
      0x8C100907,  //  000E  GETMET	R4	R4	K7
      0x58180008,  //  000F  LDCONST	R6	K8
      0x581C0009,  //  0010  LDCONST	R7	K9
      0x7C100600,  //  0011  CALL	R4	3
      0x8C10010A,  //  0012  GETMET	R4	R0	K10
      0x7C100200,  //  0013  CALL	R4	1
      0x8C10030B,  //  0014  GETMET	R4	R1	K11
      0x5818000C,  //  0015  LDCONST	R6	K12
      0x7C100400,  //  0016  CALL	R4	2
      0x7002003D,  //  0017  JMP		#0056
      0x8C100304,  //  0018  GETMET	R4	R1	K4
      0x5818000D,  //  0019  LDCONST	R6	K13
      0x7C100400,  //  001A  CALL	R4	2
      0x78120038,  //  001B  JMPF	R4	#0055
      0xB8120C00,  //  001C  GETNGBL	R4	K6
      0x8C100907,  //  001D  GETMET	R4	R4	K7
      0x5818000E,  //  001E  LDCONST	R6	K14
      0x581C0009,  //  001F  LDCONST	R7	K9
      0x7C100600,  //  0020  CALL	R4	3
      0x8C10010F,  //  0021  GETMET	R4	R0	K15
      0x7C100200,  //  0022  CALL	R4	1
      0x8C100310,  //  0023  GETMET	R4	R1	K16
      0x5818000D,  //  0024  LDCONST	R6	K13
      0x7C100400,  //  0025  CALL	R4	2
      0x20140911,  //  0026  NE	R5	R4	K17
      0x78160026,  //  0027  JMPF	R5	#004F
      0x8C140512,  //  0028  GETMET	R5	R2	K18
      0x581C0013,  //  0029  LDCONST	R7	K19
      0xB8220C00,  //  002A  GETNGBL	R8	K6
      0x8C201114,  //  002B  GETMET	R8	R8	K20
      0x7C200200,  //  002C  CALL	R8	1
      0x5C240800,  //  002D  MOVE	R9	R4
      0x7C140800,  //  002E  CALL	R5	4
      0xB81A0C00,  //  002F  GETNGBL	R6	K6
      0x8C180D07,  //  0030  GETMET	R6	R6	K7
      0x8C200512,  //  0031  GETMET	R8	R2	K18
      0x58280015,  //  0032  LDCONST	R10	K21
      0x5C2C0A00,  //  0033  MOVE	R11	R5
      0x7C200600,  //  0034  CALL	R8	3
      0x58240009,  //  0035  LDCONST	R9	K9
      0x7C180600,  //  0036  CALL	R6	3
      0x8C180512,  //  0037  GETMET	R6	R2	K18
      0x58200016,  //  0038  LDCONST	R8	K22
      0x5C240800,  //  0039  MOVE	R9	R4
      0x7C180600,  //  003A  CALL	R6	3
      0xB81E2E00,  //  003B  GETNGBL	R7	K23
      0x7C1C0000,  //  003C  CALL	R7	0
      0x8C200F18,  //  003D  GETMET	R8	R7	K24
      0x5C280A00,  //  003E  MOVE	R10	R5
      0x7C200400,  //  003F  CALL	R8	2
      0x8C200F19,  //  0040  GETMET	R8	R7	K25
      0x7C200200,  //  0041  CALL	R8	1
      0x542600C7,  //  0042  LDINT	R9	200
      0x20241009,  //  0043  NE	R9	R8	R9
      0x78260004,  //  0044  JMPF	R9	#004A
      0x8C240512,  //  0045  GETMET	R9	R2	K18
      0x582C001A,  //  0046  LDCONST	R11	K26
      0x5C301000,  //  0047  MOVE	R12	R8
      0x7C240600,  //  0048  CALL	R9	3
      0xB0063609,  //  0049  RAISE	1	K27	R9
      0x8C240F1C,  //  004A  GETMET	R9	R7	K28
      0x5C2C0C00,  //  004B  MOVE	R11	R6
      0x7C240400,  //  004C  CALL	R9	2
      0x8C240F1D,  //  004D  GETMET	R9	R7	K29
      0x7C240200,  //  004E  CALL	R9	1
      0x8C14010A,  //  004F  GETMET	R5	R0	K10
      0x7C140200,  //  0050  CALL	R5	1
      0x8C14030B,  //  0051  GETMET	R5	R1	K11
      0x581C000C,  //  0052  LDCONST	R7	K12
      0x7C140400,  //  0053  CALL	R5	2
      0x70020000,  //  0054  JMP		#0056
      0xB0063D1F,  //  0055  RAISE	1	K30	K31
      0xA8040001,  //  0056  EXBLK	1	1
      0x7002001B,  //  0057  JMP		#0074
      0xAC100002,  //  0058  CATCH	R4	0	2
      0x70020018,  //  0059  JMP		#0073
      0x60180001,  //  005A  GETGBL	R6	G1
      0x8C1C0512,  //  005B  GETMET	R7	R2	K18
      0x58240020,  //  005C  LDCONST	R9	K32
      0x5C280800,  //  005D  MOVE	R10	R4
      0x5C2C0A00,  //  005E  MOVE	R11	R5
      0x7C1C0800,  //  005F  CALL	R7	4
      0x7C180200,  //  0060  CALL	R6	1
      0x8C180321,  //  0061  GETMET	R6	R1	K33
      0x58200022,  //  0062  LDCONST	R8	K34
      0x7C180400,  //  0063  CALL	R6	2
      0x8C180323,  //  0064  GETMET	R6	R1	K35
      0x7C180200,  //  0065  CALL	R6	1
      0x8C180324,  //  0066  GETMET	R6	R1	K36
      0x8C200512,  //  0067  GETMET	R8	R2	K18
      0x58280025,  //  0068  LDCONST	R10	K37
      0x5C2C0800,  //  0069  MOVE	R11	R4
      0x5C300A00,  //  006A  MOVE	R12	R5
      0x7C200800,  //  006B  CALL	R8	4
      0x7C180400,  //  006C  CALL	R6	2
      0x8C180326,  //  006D  GETMET	R6	R1	K38
      0x88200327,  //  006E  GETMBR	R8	R1	K39
      0x7C180400,  //  006F  CALL	R6	2
      0x8C180328,  //  0070  GETMET	R6	R1	K40
      0x7C180200,  //  0071  CALL	R6	1
      0x70020000,  //  0072  JMP		#0074
      0xB0080000,  //  0073  RAISE	2	R0	R0
      0x80000000,  //  0074  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: autoexec
********************************************************************/
be_local_closure(autoexec,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[32]) {     /* constants */
    /* K0   */  be_nested_string("_archive", -290407892, 8),
    /* K1   */  be_nested_string("path", -2071507658, 4),
    /* K2   */  be_nested_string("#init.bat", -997372219, 9),
    /* K3   */  be_nested_string("is_first_time", 275242384, 13),
    /* K4   */  be_nested_string("exists", 1002329533, 6),
    /* K5   */  be_nested_string("set_first_time", -1183719746, 14),
    /* K6   */  be_nested_string("run_bat", -1758063998, 7),
    /* K7   */  be_nested_string("tasmota", 424643812, 7),
    /* K8   */  be_nested_string("log", 1062293841, 3),
    /* K9   */  be_nested_string("CFG: 'init.bat' done, restarting", 1569670677, 32),
    /* K10  */  be_const_int(2),
    /* K11  */  be_nested_string("cmd", -158181397, 3),
    /* K12  */  be_nested_string("Restart 1", -790511441, 9),
    /* K13  */  be_nested_string("#display.ini", 182218220, 12),
    /* K14  */  be_nested_string("gpio", -1656812038, 4),
    /* K15  */  be_nested_string("pin_used", -261112684, 8),
    /* K16  */  be_nested_string("OPTION_A", 1133299440, 8),
    /* K17  */  be_nested_string("display.ini", -1648793295, 11),
    /* K18  */  be_nested_string("CFG: skipping 'display.ini' because already present in file-system", -329418032, 66),
    /* K19  */  be_nested_string("display", 1164572437, 7),
    /* K20  */  be_nested_string("r", -150190315, 1),
    /* K21  */  be_nested_string("read", -824204347, 4),
    /* K22  */  be_nested_string("close", 667630371, 5),
    /* K23  */  be_nested_string("start", 1697318111, 5),
    /* K24  */  be_nested_string("#autoexec.bat", -912076799, 13),
    /* K25  */  be_nested_string("CFG: running ", -1816632762, 13),
    /* K26  */  be_const_int(3),
    /* K27  */  be_nested_string("CFG: ran  ", -715396824, 10),
    /* K28  */  be_nested_string("#autoexec.be", 1181757091, 12),
    /* K29  */  be_nested_string("CFG: loading ", -284605793, 13),
    /* K30  */  be_nested_string("load", -435725847, 4),
    /* K31  */  be_nested_string("CFG: loaded  ", -584693758, 13),
    }),
    (be_nested_const_str("autoexec", -618105405, 8)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[107]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x4C080000,  //  0001  LDNIL	R2
      0x1C040202,  //  0002  EQ	R1	R1	R2
      0x78060000,  //  0003  JMPF	R1	#0005
      0x80000200,  //  0004  RET	0
      0xA4060200,  //  0005  IMPORT	R1	K1
      0x88080100,  //  0006  GETMBR	R2	R0	K0
      0x00080502,  //  0007  ADD	R2	R2	K2
      0x8C0C0103,  //  0008  GETMET	R3	R0	K3
      0x7C0C0200,  //  0009  CALL	R3	1
      0x780E0012,  //  000A  JMPF	R3	#001E
      0x8C0C0304,  //  000B  GETMET	R3	R1	K4
      0x5C140400,  //  000C  MOVE	R5	R2
      0x7C0C0400,  //  000D  CALL	R3	2
      0x780E000E,  //  000E  JMPF	R3	#001E
      0x8C0C0105,  //  000F  GETMET	R3	R0	K5
      0x7C0C0200,  //  0010  CALL	R3	1
      0x8C0C0106,  //  0011  GETMET	R3	R0	K6
      0x5C140400,  //  0012  MOVE	R5	R2
      0x7C0C0400,  //  0013  CALL	R3	2
      0xB80E0E00,  //  0014  GETNGBL	R3	K7
      0x8C0C0708,  //  0015  GETMET	R3	R3	K8
      0x58140009,  //  0016  LDCONST	R5	K9
      0x5818000A,  //  0017  LDCONST	R6	K10
      0x7C0C0600,  //  0018  CALL	R3	3
      0xB80E0E00,  //  0019  GETNGBL	R3	K7
      0x8C0C070B,  //  001A  GETMET	R3	R3	K11
      0x5814000C,  //  001B  LDCONST	R5	K12
      0x7C0C0400,  //  001C  CALL	R3	2
      0x80000600,  //  001D  RET	0
      0x880C0100,  //  001E  GETMBR	R3	R0	K0
      0x000C070D,  //  001F  ADD	R3	R3	K13
      0x5C080600,  //  0020  MOVE	R2	R3
      0xB80E1C00,  //  0021  GETNGBL	R3	K14
      0x8C0C070F,  //  0022  GETMET	R3	R3	K15
      0xB8161C00,  //  0023  GETNGBL	R5	K14
      0x88140B10,  //  0024  GETMBR	R5	R5	K16
      0x5818000A,  //  0025  LDCONST	R6	K10
      0x7C0C0600,  //  0026  CALL	R3	3
      0x780E0019,  //  0027  JMPF	R3	#0042
      0x8C0C0304,  //  0028  GETMET	R3	R1	K4
      0x5C140400,  //  0029  MOVE	R5	R2
      0x7C0C0400,  //  002A  CALL	R3	2
      0x780E0015,  //  002B  JMPF	R3	#0042
      0x8C0C0304,  //  002C  GETMET	R3	R1	K4
      0x58140011,  //  002D  LDCONST	R5	K17
      0x7C0C0400,  //  002E  CALL	R3	2
      0x780E0005,  //  002F  JMPF	R3	#0036
      0xB80E0E00,  //  0030  GETNGBL	R3	K7
      0x8C0C0708,  //  0031  GETMET	R3	R3	K8
      0x58140012,  //  0032  LDCONST	R5	K18
      0x5818000A,  //  0033  LDCONST	R6	K10
      0x7C0C0600,  //  0034  CALL	R3	3
      0x7002000B,  //  0035  JMP		#0042
      0xA40E2600,  //  0036  IMPORT	R3	K19
      0x60100011,  //  0037  GETGBL	R4	G17
      0x5C140400,  //  0038  MOVE	R5	R2
      0x58180014,  //  0039  LDCONST	R6	K20
      0x7C100400,  //  003A  CALL	R4	2
      0x8C140915,  //  003B  GETMET	R5	R4	K21
      0x7C140200,  //  003C  CALL	R5	1
      0x8C180916,  //  003D  GETMET	R6	R4	K22
      0x7C180200,  //  003E  CALL	R6	1
      0x8C180717,  //  003F  GETMET	R6	R3	K23
      0x5C200A00,  //  0040  MOVE	R8	R5
      0x7C180400,  //  0041  CALL	R6	2
      0x880C0100,  //  0042  GETMBR	R3	R0	K0
      0x000C0718,  //  0043  ADD	R3	R3	K24
      0x5C080600,  //  0044  MOVE	R2	R3
      0x8C0C0304,  //  0045  GETMET	R3	R1	K4
      0x5C140400,  //  0046  MOVE	R5	R2
      0x7C0C0400,  //  0047  CALL	R3	2
      0x780E000C,  //  0048  JMPF	R3	#0056
      0xB80E0E00,  //  0049  GETNGBL	R3	K7
      0x8C0C0708,  //  004A  GETMET	R3	R3	K8
      0x00163202,  //  004B  ADD	R5	K25	R2
      0x5818001A,  //  004C  LDCONST	R6	K26
      0x7C0C0600,  //  004D  CALL	R3	3
      0x8C0C0106,  //  004E  GETMET	R3	R0	K6
      0x5C140400,  //  004F  MOVE	R5	R2
      0x7C0C0400,  //  0050  CALL	R3	2
      0xB80E0E00,  //  0051  GETNGBL	R3	K7
      0x8C0C0708,  //  0052  GETMET	R3	R3	K8
      0x00163602,  //  0053  ADD	R5	K27	R2
      0x5818001A,  //  0054  LDCONST	R6	K26
      0x7C0C0600,  //  0055  CALL	R3	3
      0x880C0100,  //  0056  GETMBR	R3	R0	K0
      0x000C071C,  //  0057  ADD	R3	R3	K28
      0x5C080600,  //  0058  MOVE	R2	R3
      0x8C0C0304,  //  0059  GETMET	R3	R1	K4
      0x5C140400,  //  005A  MOVE	R5	R2
      0x7C0C0400,  //  005B  CALL	R3	2
      0x780E000C,  //  005C  JMPF	R3	#006A
      0xB80E0E00,  //  005D  GETNGBL	R3	K7
      0x8C0C0708,  //  005E  GETMET	R3	R3	K8
      0x00163A02,  //  005F  ADD	R5	K29	R2
      0x5818001A,  //  0060  LDCONST	R6	K26
      0x7C0C0600,  //  0061  CALL	R3	3
      0xB80E3C00,  //  0062  GETNGBL	R3	K30
      0x5C100400,  //  0063  MOVE	R4	R2
      0x7C0C0200,  //  0064  CALL	R3	1
      0xB80E0E00,  //  0065  GETNGBL	R3	K7
      0x8C0C0708,  //  0066  GETMET	R3	R3	K8
      0x00163E02,  //  0067  ADD	R5	K31	R2
      0x5818001A,  //  0068  LDCONST	R6	K26
      0x7C0C0600,  //  0069  CALL	R3	3
      0x80000000,  //  006A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: run_bat
********************************************************************/
be_local_closure(run_bat,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    2,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[10]) {     /* constants */
    /* K0   */  be_nested_string("string", 398550328, 6),
    /* K1   */  be_nested_string("r", -150190315, 1),
    /* K2   */  be_nested_string("readline", 1212709927, 8),
    /* K3   */  be_const_int(0),
    /* K4   */  be_nested_string("\n", 252472541, 1),
    /* K5   */  be_nested_string("tasmota", 424643812, 7),
    /* K6   */  be_nested_string("cmd", -158181397, 3),
    /* K7   */  be_nested_string("close", 667630371, 5),
    /* K8   */  be_nested_string("format", -1180859054, 6),
    /* K9   */  be_nested_string("CFG: could not run %s (%s - %s)", 1428829580, 31),
    }),
    (be_nested_const_str("run_bat", -1758063998, 7)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[54]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x4C0C0000,  //  0001  LDNIL	R3
      0xA8020023,  //  0002  EXBLK	0	#0027
      0x60100011,  //  0003  GETGBL	R4	G17
      0x5C140200,  //  0004  MOVE	R5	R1
      0x58180001,  //  0005  LDCONST	R6	K1
      0x7C100400,  //  0006  CALL	R4	2
      0x5C0C0800,  //  0007  MOVE	R3	R4
      0x50100200,  //  0008  LDBOOL	R4	1	0
      0x78120018,  //  0009  JMPF	R4	#0023
      0x8C100702,  //  000A  GETMET	R4	R3	K2
      0x7C100200,  //  000B  CALL	R4	1
      0x6014000C,  //  000C  GETGBL	R5	G12
      0x5C180800,  //  000D  MOVE	R6	R4
      0x7C140200,  //  000E  CALL	R5	1
      0x1C140B03,  //  000F  EQ	R5	R5	K3
      0x78160000,  //  0010  JMPF	R5	#0012
      0x70020010,  //  0011  JMP		#0023
      0x5415FFFE,  //  0012  LDINT	R5	-1
      0x94140805,  //  0013  GETIDX	R5	R4	R5
      0x1C140B04,  //  0014  EQ	R5	R5	K4
      0x78160002,  //  0015  JMPF	R5	#0019
      0x5415FFFD,  //  0016  LDINT	R5	-2
      0x40160605,  //  0017  CONNECT	R5	K3	R5
      0x94100805,  //  0018  GETIDX	R4	R4	R5
      0x6014000C,  //  0019  GETGBL	R5	G12
      0x5C180800,  //  001A  MOVE	R6	R4
      0x7C140200,  //  001B  CALL	R5	1
      0x24140B03,  //  001C  GT	R5	R5	K3
      0x78160003,  //  001D  JMPF	R5	#0022
      0xB8160A00,  //  001E  GETNGBL	R5	K5
      0x8C140B06,  //  001F  GETMET	R5	R5	K6
      0x5C1C0800,  //  0020  MOVE	R7	R4
      0x7C140400,  //  0021  CALL	R5	2
      0x7001FFE4,  //  0022  JMP		#0008
      0x8C100707,  //  0023  GETMET	R4	R3	K7
      0x7C100200,  //  0024  CALL	R4	1
      0xA8040001,  //  0025  EXBLK	1	1
      0x7002000D,  //  0026  JMP		#0035
      0xAC100002,  //  0027  CATCH	R4	0	2
      0x7002000A,  //  0028  JMP		#0034
      0x60180001,  //  0029  GETGBL	R6	G1
      0x8C1C0508,  //  002A  GETMET	R7	R2	K8
      0x58240009,  //  002B  LDCONST	R9	K9
      0x5C280200,  //  002C  MOVE	R10	R1
      0x5C2C0800,  //  002D  MOVE	R11	R4
      0x5C300A00,  //  002E  MOVE	R12	R5
      0x7C1C0A00,  //  002F  CALL	R7	5
      0x7C180200,  //  0030  CALL	R6	1
      0x8C180707,  //  0031  GETMET	R6	R3	K7
      0x7C180200,  //  0032  CALL	R6	1
      0x70020000,  //  0033  JMP		#0035
      0xB0080000,  //  0034  RAISE	2	R0	R0
      0x80000000,  //  0035  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: page_autoconf_mgr
********************************************************************/
be_local_closure(page_autoconf_mgr,   /* name */
  be_nested_proto(
    19,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[39]) {     /* constants */
    /* K0   */  be_nested_string("webserver", 1572454038, 9),
    /* K1   */  be_nested_string("string", 398550328, 6),
    /* K2   */  be_nested_string("check_privileged_access", -602033328, 23),
    /* K3   */  be_nested_string("content_start", -1357458227, 13),
    /* K4   */  be_nested_string("Auto-configuration", 1665006109, 18),
    /* K5   */  be_nested_string("content_send_style", 1087907647, 18),
    /* K6   */  be_nested_string("content_send", 1673733649, 12),
    /* K7   */  be_nested_string("<p><small>&nbsp;(This feature requires an internet connection)</small></p>", -1575700810, 74),
    /* K8   */  be_nested_string("get_current_module_path", -1088293888, 23),
    /* K9   */  be_nested_string("tr", 1195724803, 2),
    /* K10  */  be_nested_string("get_current_module_name", -1915696556, 23),
    /* K11  */  be_nested_string("_", -636741266, 1),
    /* K12  */  be_nested_string(" ", 621580159, 1),
    /* K13  */  be_nested_string("_error", 1132109656, 6),
    /* K14  */  be_nested_string("&lt;Error: apply new or remove&gt;", -1439459347, 34),
    /* K15  */  be_nested_string("&lt;None&gt;", -1692801798, 12),
    /* K16  */  be_nested_string("<fieldset><style>.bdis{background:#888;}.bdis:hover{background:#888;}</style>", 842307168, 77),
    /* K17  */  be_nested_string("format", -1180859054, 6),
    /* K18  */  be_nested_string("<legend><b title='Autoconfiguration'>&nbsp;Current auto-configuration</b></legend>", -82466516, 82),
    /* K19  */  be_nested_string("<p>Current configuration: </p><p><b>%s</b></p>", -179311535, 46),
    /* K20  */  be_nested_string("<p><form id=reapply style='display: block;' action='/ac' method='post' ", 546993478, 71),
    /* K21  */  be_nested_string("onsubmit='return confirm(\"This will cause a restart.\");'>", 232646018, 57),
    /* K22  */  be_nested_string("<button name='reapply' class='button bgrn'>Re-apply current configuration</button>", -1147033080, 82),
    /* K23  */  be_nested_string("</form></p>", -748395557, 11),
    /* K24  */  be_nested_string("<p></p></fieldset><p></p>", 2052843416, 25),
    /* K25  */  be_nested_string("<legend><b title='New autoconf'>&nbsp;Select new auto-configuration</b></legend>", 1926223891, 80),
    /* K26  */  be_nested_string("<p><form id=zip style='display: block;' action='/ac' method='post' ", -261345130, 67),
    /* K27  */  be_nested_string("onsubmit='return confirm(\"This will change the current configuration and cause a restart.\");'>", -502554737, 94),
    /* K28  */  be_nested_string("<label>Choose a device configuration:</label><br>", 1336654704, 49),
    /* K29  */  be_nested_string("<select name='zip'>", -47042760, 19),
    /* K30  */  be_nested_string("load_templates", -781097163, 14),
    /* K31  */  be_nested_string("<option value='reset'>&lt;Remove autoconf&gt;</option>", -300347541, 54),
    /* K32  */  be_nested_string("<option value='%s'>%s</option>", 510303524, 30),
    /* K33  */  be_nested_string("stop_iteration", -121173395, 14),
    /* K34  */  be_nested_string("</select><p></p>", 1863865923, 16),
    /* K35  */  be_nested_string("<button name='zipapply' class='button bgrn'>Apply configuration</button>", 1205771629, 72),
    /* K36  */  be_nested_string("content_button", 1956476087, 14),
    /* K37  */  be_nested_string("BUTTON_CONFIGURATION", 70820856, 20),
    /* K38  */  be_nested_string("content_stop", 658554751, 12),
    }),
    (be_nested_const_str("page_autoconf_mgr", -651030265, 17)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[124]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0x8C0C0302,  //  0002  GETMET	R3	R1	K2
      0x7C0C0200,  //  0003  CALL	R3	1
      0x740E0001,  //  0004  JMPT	R3	#0007
      0x4C0C0000,  //  0005  LDNIL	R3
      0x80040600,  //  0006  RET	1	R3
      0x8C0C0303,  //  0007  GETMET	R3	R1	K3
      0x58140004,  //  0008  LDCONST	R5	K4
      0x7C0C0400,  //  0009  CALL	R3	2
      0x8C0C0305,  //  000A  GETMET	R3	R1	K5
      0x7C0C0200,  //  000B  CALL	R3	1
      0x8C0C0306,  //  000C  GETMET	R3	R1	K6
      0x58140007,  //  000D  LDCONST	R5	K7
      0x7C0C0400,  //  000E  CALL	R3	2
      0x8C0C0108,  //  000F  GETMET	R3	R0	K8
      0x7C0C0200,  //  0010  CALL	R3	1
      0x780E0006,  //  0011  JMPF	R3	#0019
      0x8C100509,  //  0012  GETMET	R4	R2	K9
      0x8C18010A,  //  0013  GETMET	R6	R0	K10
      0x7C180200,  //  0014  CALL	R6	1
      0x581C000B,  //  0015  LDCONST	R7	K11
      0x5820000C,  //  0016  LDCONST	R8	K12
      0x7C100800,  //  0017  CALL	R4	4
      0x70020004,  //  0018  JMP		#001E
      0x8810010D,  //  0019  GETMBR	R4	R0	K13
      0x78120001,  //  001A  JMPF	R4	#001D
      0x5810000E,  //  001B  LDCONST	R4	K14
      0x70020000,  //  001C  JMP		#001E
      0x5810000F,  //  001D  LDCONST	R4	K15
      0x8C140306,  //  001E  GETMET	R5	R1	K6
      0x581C0010,  //  001F  LDCONST	R7	K16
      0x7C140400,  //  0020  CALL	R5	2
      0x8C140306,  //  0021  GETMET	R5	R1	K6
      0x8C1C0511,  //  0022  GETMET	R7	R2	K17
      0x58240012,  //  0023  LDCONST	R9	K18
      0x7C1C0400,  //  0024  CALL	R7	2
      0x7C140400,  //  0025  CALL	R5	2
      0x8C140306,  //  0026  GETMET	R5	R1	K6
      0x8C1C0511,  //  0027  GETMET	R7	R2	K17
      0x58240013,  //  0028  LDCONST	R9	K19
      0x5C280800,  //  0029  MOVE	R10	R4
      0x7C1C0600,  //  002A  CALL	R7	3
      0x7C140400,  //  002B  CALL	R5	2
      0x780E000B,  //  002C  JMPF	R3	#0039
      0x8C140306,  //  002D  GETMET	R5	R1	K6
      0x581C0014,  //  002E  LDCONST	R7	K20
      0x7C140400,  //  002F  CALL	R5	2
      0x8C140306,  //  0030  GETMET	R5	R1	K6
      0x581C0015,  //  0031  LDCONST	R7	K21
      0x7C140400,  //  0032  CALL	R5	2
      0x8C140306,  //  0033  GETMET	R5	R1	K6
      0x581C0016,  //  0034  LDCONST	R7	K22
      0x7C140400,  //  0035  CALL	R5	2
      0x8C140306,  //  0036  GETMET	R5	R1	K6
      0x581C0017,  //  0037  LDCONST	R7	K23
      0x7C140400,  //  0038  CALL	R5	2
      0x8C140306,  //  0039  GETMET	R5	R1	K6
      0x581C0018,  //  003A  LDCONST	R7	K24
      0x7C140400,  //  003B  CALL	R5	2
      0x8C140306,  //  003C  GETMET	R5	R1	K6
      0x581C0010,  //  003D  LDCONST	R7	K16
      0x7C140400,  //  003E  CALL	R5	2
      0x8C140306,  //  003F  GETMET	R5	R1	K6
      0x8C1C0511,  //  0040  GETMET	R7	R2	K17
      0x58240019,  //  0041  LDCONST	R9	K25
      0x7C1C0400,  //  0042  CALL	R7	2
      0x7C140400,  //  0043  CALL	R5	2
      0x8C140306,  //  0044  GETMET	R5	R1	K6
      0x581C001A,  //  0045  LDCONST	R7	K26
      0x7C140400,  //  0046  CALL	R5	2
      0x8C140306,  //  0047  GETMET	R5	R1	K6
      0x581C001B,  //  0048  LDCONST	R7	K27
      0x7C140400,  //  0049  CALL	R5	2
      0x8C140306,  //  004A  GETMET	R5	R1	K6
      0x581C001C,  //  004B  LDCONST	R7	K28
      0x7C140400,  //  004C  CALL	R5	2
      0x8C140306,  //  004D  GETMET	R5	R1	K6
      0x581C001D,  //  004E  LDCONST	R7	K29
      0x7C140400,  //  004F  CALL	R5	2
      0x8C14011E,  //  0050  GETMET	R5	R0	K30
      0x7C140200,  //  0051  CALL	R5	1
      0x8C180306,  //  0052  GETMET	R6	R1	K6
      0x5820001F,  //  0053  LDCONST	R8	K31
      0x7C180400,  //  0054  CALL	R6	2
      0x60180010,  //  0055  GETGBL	R6	G16
      0x5C1C0A00,  //  0056  MOVE	R7	R5
      0x7C180200,  //  0057  CALL	R6	1
      0xA802000D,  //  0058  EXBLK	0	#0067
      0x5C1C0C00,  //  0059  MOVE	R7	R6
      0x7C1C0000,  //  005A  CALL	R7	0
      0x8C200306,  //  005B  GETMET	R8	R1	K6
      0x8C280511,  //  005C  GETMET	R10	R2	K17
      0x58300020,  //  005D  LDCONST	R12	K32
      0x5C340E00,  //  005E  MOVE	R13	R7
      0x8C380509,  //  005F  GETMET	R14	R2	K9
      0x5C400E00,  //  0060  MOVE	R16	R7
      0x5844000B,  //  0061  LDCONST	R17	K11
      0x5848000C,  //  0062  LDCONST	R18	K12
      0x7C380800,  //  0063  CALL	R14	4
      0x7C280800,  //  0064  CALL	R10	4
      0x7C200400,  //  0065  CALL	R8	2
      0x7001FFF1,  //  0066  JMP		#0059
      0x58180021,  //  0067  LDCONST	R6	K33
      0xAC180200,  //  0068  CATCH	R6	1	0
      0xB0080000,  //  0069  RAISE	2	R0	R0
      0x8C180306,  //  006A  GETMET	R6	R1	K6
      0x58200022,  //  006B  LDCONST	R8	K34
      0x7C180400,  //  006C  CALL	R6	2
      0x8C180306,  //  006D  GETMET	R6	R1	K6
      0x58200023,  //  006E  LDCONST	R8	K35
      0x7C180400,  //  006F  CALL	R6	2
      0x8C180306,  //  0070  GETMET	R6	R1	K6
      0x58200017,  //  0071  LDCONST	R8	K23
      0x7C180400,  //  0072  CALL	R6	2
      0x8C180306,  //  0073  GETMET	R6	R1	K6
      0x58200018,  //  0074  LDCONST	R8	K24
      0x7C180400,  //  0075  CALL	R6	2
      0x8C180324,  //  0076  GETMET	R6	R1	K36
      0x88200325,  //  0077  GETMBR	R8	R1	K37
      0x7C180400,  //  0078  CALL	R6	2
      0x8C180326,  //  0079  GETMET	R6	R1	K38
      0x7C180200,  //  007A  CALL	R6	1
      0x80000000,  //  007B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_current_module_name
********************************************************************/
be_local_closure(get_current_module_name,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_string("_archive", -290407892, 8),
    /* K1   */  be_const_int(0),
    }),
    (be_nested_const_str("get_current_module_name", -1915696556, 23)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[ 5]) {  /* code */
      0x5405FFF5,  //  0000  LDINT	R1	-10
      0x40060201,  //  0001  CONNECT	R1	K1	R1
      0x88080100,  //  0002  GETMBR	R2	R0	K0
      0x94040401,  //  0003  GETIDX	R1	R2	R1
      0x80040200,  //  0004  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: delete_all_configs
********************************************************************/
be_local_closure(delete_all_configs,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_string("path", -2071507658, 4),
    /* K1   */  be_nested_string("string", 398550328, 6),
    /* K2   */  be_nested_string("listdir", 2005220720, 7),
    /* K3   */  be_nested_string("/", 705468254, 1),
    /* K4   */  be_nested_string("find", -1108310694, 4),
    /* K5   */  be_nested_string(".autoconf", -1770288208, 9),
    /* K6   */  be_const_int(0),
    /* K7   */  be_nested_string("remove", -611183107, 6),
    /* K8   */  be_nested_string("stop_iteration", -121173395, 14),
    }),
    (be_nested_const_str("delete_all_configs", -1912899718, 18)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[25]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0x8C0C0302,  //  0002  GETMET	R3	R1	K2
      0x58140003,  //  0003  LDCONST	R5	K3
      0x7C0C0400,  //  0004  CALL	R3	2
      0x60100010,  //  0005  GETGBL	R4	G16
      0x5C140600,  //  0006  MOVE	R5	R3
      0x7C100200,  //  0007  CALL	R4	1
      0xA802000B,  //  0008  EXBLK	0	#0015
      0x5C140800,  //  0009  MOVE	R5	R4
      0x7C140000,  //  000A  CALL	R5	0
      0x8C180504,  //  000B  GETMET	R6	R2	K4
      0x5C200A00,  //  000C  MOVE	R8	R5
      0x58240005,  //  000D  LDCONST	R9	K5
      0x7C180600,  //  000E  CALL	R6	3
      0x24180D06,  //  000F  GT	R6	R6	K6
      0x781A0002,  //  0010  JMPF	R6	#0014
      0x8C180307,  //  0011  GETMET	R6	R1	K7
      0x5C200A00,  //  0012  MOVE	R8	R5
      0x7C180400,  //  0013  CALL	R6	2
      0x7001FFF3,  //  0014  JMP		#0009
      0x58100008,  //  0015  LDCONST	R4	K8
      0xAC100200,  //  0016  CATCH	R4	1	0
      0xB0080000,  //  0017  RAISE	2	R0	R0
      0x80000000,  //  0018  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_first_time
********************************************************************/
be_local_closure(set_first_time,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_string("/.autoconf", -2082892903, 10),
    /* K1   */  be_nested_string("w", -234078410, 1),
    /* K2   */  be_nested_string("close", 667630371, 5),
    }),
    (be_nested_const_str("set_first_time", -1183719746, 14)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[ 7]) {  /* code */
      0x60040011,  //  0000  GETGBL	R1	G17
      0x58080000,  //  0001  LDCONST	R2	K0
      0x580C0001,  //  0002  LDCONST	R3	K1
      0x7C040400,  //  0003  CALL	R1	2
      0x8C080302,  //  0004  GETMET	R2	R1	K2
      0x7C080200,  //  0005  CALL	R2	1
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: load_templates
********************************************************************/
be_local_closure(load_templates,   /* name */
  be_nested_proto(
    15,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[21]) {     /* constants */
    /* K0   */  be_nested_string("string", 398550328, 6),
    /* K1   */  be_nested_string("json", 916562499, 4),
    /* K2   */  be_nested_string("format", -1180859054, 6),
    /* K3   */  be_nested_string("https://raw.githubusercontent.com/tasmota/autoconf/main/%s_manifest.json", -637415251, 72),
    /* K4   */  be_nested_string("tasmota", 424643812, 7),
    /* K5   */  be_nested_string("arch", -1342162999, 4),
    /* K6   */  be_nested_string("log", 1062293841, 3),
    /* K7   */  be_nested_string("CFG: loading '%s'", -2009661199, 17),
    /* K8   */  be_const_int(3),
    /* K9   */  be_nested_string("webclient", -218578150, 9),
    /* K10  */  be_nested_string("begin", 1748273790, 5),
    /* K11  */  be_nested_string("GET", -1763262857, 3),
    /* K12  */  be_nested_string("CFG: return_code=%i", 2059897320, 19),
    /* K13  */  be_const_int(2),
    /* K14  */  be_nested_string("get_string", -99119327, 10),
    /* K15  */  be_nested_string("close", 667630371, 5),
    /* K16  */  be_nested_string("load", -435725847, 4),
    /* K17  */  be_nested_string("CFG: loaded '%s'", 1699028828, 16),
    /* K18  */  be_nested_string("find", -1108310694, 4),
    /* K19  */  be_nested_string("files", 1055342736, 5),
    /* K20  */  be_nested_string("CFG: exception '%s' - '%s'", -199559383, 26),
    }),
    (be_nested_const_str("load_templates", -781097163, 14)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[86]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0xA8020042,  //  0002  EXBLK	0	#0046
      0x8C0C0302,  //  0003  GETMET	R3	R1	K2
      0x58140003,  //  0004  LDCONST	R5	K3
      0xB81A0800,  //  0005  GETNGBL	R6	K4
      0x8C180D05,  //  0006  GETMET	R6	R6	K5
      0x7C180200,  //  0007  CALL	R6	1
      0x7C0C0600,  //  0008  CALL	R3	3
      0xB8120800,  //  0009  GETNGBL	R4	K4
      0x8C100906,  //  000A  GETMET	R4	R4	K6
      0x8C180302,  //  000B  GETMET	R6	R1	K2
      0x58200007,  //  000C  LDCONST	R8	K7
      0x5C240600,  //  000D  MOVE	R9	R3
      0x7C180600,  //  000E  CALL	R6	3
      0x581C0008,  //  000F  LDCONST	R7	K8
      0x7C100600,  //  0010  CALL	R4	3
      0xB8121200,  //  0011  GETNGBL	R4	K9
      0x7C100000,  //  0012  CALL	R4	0
      0x8C14090A,  //  0013  GETMET	R5	R4	K10
      0x5C1C0600,  //  0014  MOVE	R7	R3
      0x7C140400,  //  0015  CALL	R5	2
      0x8C14090B,  //  0016  GETMET	R5	R4	K11
      0x7C140200,  //  0017  CALL	R5	1
      0x541A00C7,  //  0018  LDINT	R6	200
      0x20180A06,  //  0019  NE	R6	R5	R6
      0x781A000A,  //  001A  JMPF	R6	#0026
      0xB81A0800,  //  001B  GETNGBL	R6	K4
      0x8C180D06,  //  001C  GETMET	R6	R6	K6
      0x8C200302,  //  001D  GETMET	R8	R1	K2
      0x5828000C,  //  001E  LDCONST	R10	K12
      0x5C2C0A00,  //  001F  MOVE	R11	R5
      0x7C200600,  //  0020  CALL	R8	3
      0x5824000D,  //  0021  LDCONST	R9	K13
      0x7C180600,  //  0022  CALL	R6	3
      0x4C180000,  //  0023  LDNIL	R6
      0xA8040001,  //  0024  EXBLK	1	1
      0x80040C00,  //  0025  RET	1	R6
      0x8C18090E,  //  0026  GETMET	R6	R4	K14
      0x7C180200,  //  0027  CALL	R6	1
      0x8C1C090F,  //  0028  GETMET	R7	R4	K15
      0x7C1C0200,  //  0029  CALL	R7	1
      0x8C1C0510,  //  002A  GETMET	R7	R2	K16
      0x5C240C00,  //  002B  MOVE	R9	R6
      0x7C1C0400,  //  002C  CALL	R7	2
      0xB8220800,  //  002D  GETNGBL	R8	K4
      0x8C201106,  //  002E  GETMET	R8	R8	K6
      0x8C280302,  //  002F  GETMET	R10	R1	K2
      0x58300011,  //  0030  LDCONST	R12	K17
      0x60340008,  //  0031  GETGBL	R13	G8
      0x5C380E00,  //  0032  MOVE	R14	R7
      0x7C340200,  //  0033  CALL	R13	1
      0x7C280600,  //  0034  CALL	R10	3
      0x582C0008,  //  0035  LDCONST	R11	K8
      0x7C200600,  //  0036  CALL	R8	3
      0x8C200F12,  //  0037  GETMET	R8	R7	K18
      0x58280013,  //  0038  LDCONST	R10	K19
      0x7C200400,  //  0039  CALL	R8	2
      0x6024000F,  //  003A  GETGBL	R9	G15
      0x5C281000,  //  003B  MOVE	R10	R8
      0x602C0012,  //  003C  GETGBL	R11	G18
      0x7C240400,  //  003D  CALL	R9	2
      0x78260001,  //  003E  JMPF	R9	#0041
      0xA8040001,  //  003F  EXBLK	1	1
      0x80041000,  //  0040  RET	1	R8
      0x4C240000,  //  0041  LDNIL	R9
      0xA8040001,  //  0042  EXBLK	1	1
      0x80041200,  //  0043  RET	1	R9
      0xA8040001,  //  0044  EXBLK	1	1
      0x7002000E,  //  0045  JMP		#0055
      0xAC0C0002,  //  0046  CATCH	R3	0	2
      0x7002000B,  //  0047  JMP		#0054
      0xB8160800,  //  0048  GETNGBL	R5	K4
      0x8C140B06,  //  0049  GETMET	R5	R5	K6
      0x8C1C0302,  //  004A  GETMET	R7	R1	K2
      0x58240014,  //  004B  LDCONST	R9	K20
      0x5C280600,  //  004C  MOVE	R10	R3
      0x5C2C0800,  //  004D  MOVE	R11	R4
      0x7C1C0800,  //  004E  CALL	R7	4
      0x5820000D,  //  004F  LDCONST	R8	K13
      0x7C140600,  //  0050  CALL	R5	3
      0x4C140000,  //  0051  LDNIL	R5
      0x80040A00,  //  0052  RET	1	R5
      0x70020000,  //  0053  JMP		#0055
      0xB0080000,  //  0054  RAISE	2	R0	R0
      0x80000000,  //  0055  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: web_add_config_button
********************************************************************/
be_local_closure(web_add_config_button,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_string("webserver", 1572454038, 9),
    /* K1   */  be_nested_string("content_send", 1673733649, 12),
    /* K2   */  be_nested_string("<p><form id=ac action='ac' style='display: block;' method='get'><button>&#129668; Auto-configuration</button></form></p>", 452285201, 120),
    }),
    (be_nested_const_str("web_add_config_button", 639674325, 21)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[ 5]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080301,  //  0001  GETMET	R2	R1	K1
      0x58100002,  //  0002  LDCONST	R4	K2
      0x7C080400,  //  0003  CALL	R2	2
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_first_time
********************************************************************/
be_local_closure(is_first_time,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_string("path", -2071507658, 4),
    /* K1   */  be_nested_string("exists", 1002329533, 6),
    /* K2   */  be_nested_string("/.autoconf", -2082892903, 10),
    }),
    (be_nested_const_str("is_first_time", 275242384, 13)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[ 8]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080301,  //  0001  GETMET	R2	R1	K1
      0x58100002,  //  0002  LDCONST	R4	K2
      0x7C080400,  //  0003  CALL	R2	2
      0x780A0000,  //  0004  JMPF	R2	#0006
      0x50080001,  //  0005  LDBOOL	R2	0	1
      0x50080200,  //  0006  LDBOOL	R2	1	0
      0x80040400,  //  0007  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(init,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[17]) {     /* constants */
    /* K0   */  be_nested_str_literal("path"),
    /* K1   */  be_nested_str_literal("string"),
    /* K2   */  be_nested_str_literal("listdir"),
    /* K3   */  be_nested_str_literal("/"),
    /* K4   */  be_nested_str_literal("tasmota"),
    /* K5   */  be_nested_str_literal("add_driver"),
    /* K6   */  be_const_int(0),
    /* K7   */  be_nested_str_literal("find"),
    /* K8   */  be_nested_str_literal(".autoconf"),
    /* K9   */  be_nested_str_literal("format"),
    /* K10  */  be_nested_str_literal("CFG: multiple autoconf files found, aborting ('%s' + '%s')"),
    /* K11  */  be_nested_str_literal("_error"),
    /* K12  */  be_const_int(1),
    /* K13  */  be_nested_str_literal("log"),
    /* K14  */  be_nested_str_literal("CFG: no '*.autoconf' file found"),
    /* K15  */  be_const_int(2),
    /* K16  */  be_nested_str_literal("_archive"),
    }),
    (be_nested_const_str("init", 380752755, 4)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[51]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0x8C0C0302,  //  0002  GETMET	R3	R1	K2
      0x58140003,  //  0003  LDCONST	R5	K3
      0x7C0C0400,  //  0004  CALL	R3	2
      0x4C100000,  //  0005  LDNIL	R4
      0xB8160800,  //  0006  GETNGBL	R5	K4
      0x8C140B05,  //  0007  GETMET	R5	R5	K5
      0x5C1C0000,  //  0008  MOVE	R7	R0
      0x7C140400,  //  0009  CALL	R5	2
      0x58140006,  //  000A  LDCONST	R5	K6
      0x6018000C,  //  000B  GETGBL	R6	G12
      0x5C1C0600,  //  000C  MOVE	R7	R3
      0x7C180200,  //  000D  CALL	R6	1
      0x14180A06,  //  000E  LT	R6	R5	R6
      0x781A0016,  //  000F  JMPF	R6	#0027
      0x8C180507,  //  0010  GETMET	R6	R2	K7
      0x94200605,  //  0011  GETIDX	R8	R3	R5
      0x58240008,  //  0012  LDCONST	R9	K8
      0x7C180600,  //  0013  CALL	R6	3
      0x24180D06,  //  0014  GT	R6	R6	K6
      0x781A000E,  //  0015  JMPF	R6	#0025
      0x4C180000,  //  0016  LDNIL	R6
      0x20180806,  //  0017  NE	R6	R4	R6
      0x781A000A,  //  0018  JMPF	R6	#0024
      0x60180001,  //  0019  GETGBL	R6	G1
      0x8C1C0509,  //  001A  GETMET	R7	R2	K9
      0x5824000A,  //  001B  LDCONST	R9	K10
      0x5C280800,  //  001C  MOVE	R10	R4
      0x942C0605,  //  001D  GETIDX	R11	R3	R5
      0x7C1C0800,  //  001E  CALL	R7	4
      0x7C180200,  //  001F  CALL	R6	1
      0x50180200,  //  0020  LDBOOL	R6	1	0
      0x90021606,  //  0021  SETMBR	R0	K11	R6
      0x4C180000,  //  0022  LDNIL	R6
      0x80040C00,  //  0023  RET	1	R6
      0x94100605,  //  0024  GETIDX	R4	R3	R5
      0x00140B0C,  //  0025  ADD	R5	R5	K12
      0x7001FFE3,  //  0026  JMP		#000B
      0x4C180000,  //  0027  LDNIL	R6
      0x1C180806,  //  0028  EQ	R6	R4	R6
      0x781A0006,  //  0029  JMPF	R6	#0031
      0xB81A0800,  //  002A  GETNGBL	R6	K4
      0x8C180D0D,  //  002B  GETMET	R6	R6	K13
      0x5820000E,  //  002C  LDCONST	R8	K14
      0x5824000F,  //  002D  LDCONST	R9	K15
      0x7C180600,  //  002E  CALL	R6	3
      0x4C180000,  //  002F  LDNIL	R6
      0x80040C00,  //  0030  RET	1	R6
      0x90022004,  //  0031  SETMBR	R0	K16	R4
      0x80000000,  //  0032  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: preinit
********************************************************************/
be_local_closure(preinit,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[10]) {     /* constants */
    /* K0   */  be_nested_string("_archive", -290407892, 8),
    /* K1   */  be_nested_string("path", -2071507658, 4),
    /* K2   */  be_nested_string("#preinit.be", 687035716, 11),
    /* K3   */  be_nested_string("exists", 1002329533, 6),
    /* K4   */  be_nested_string("tasmota", 424643812, 7),
    /* K5   */  be_nested_string("log", 1062293841, 3),
    /* K6   */  be_nested_string("CFG: loading ", -284605793, 13),
    /* K7   */  be_const_int(3),
    /* K8   */  be_nested_string("load", -435725847, 4),
    /* K9   */  be_nested_string("CFG: loaded  ", -584693758, 13),
    }),
    (be_nested_const_str("preinit", -1572960196, 7)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[26]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x4C080000,  //  0001  LDNIL	R2
      0x1C040202,  //  0002  EQ	R1	R1	R2
      0x78060000,  //  0003  JMPF	R1	#0005
      0x80000200,  //  0004  RET	0
      0xA4060200,  //  0005  IMPORT	R1	K1
      0x88080100,  //  0006  GETMBR	R2	R0	K0
      0x00080502,  //  0007  ADD	R2	R2	K2
      0x8C0C0303,  //  0008  GETMET	R3	R1	K3
      0x5C140400,  //  0009  MOVE	R5	R2
      0x7C0C0400,  //  000A  CALL	R3	2
      0x780E000C,  //  000B  JMPF	R3	#0019
      0xB80E0800,  //  000C  GETNGBL	R3	K4
      0x8C0C0705,  //  000D  GETMET	R3	R3	K5
      0x00160C02,  //  000E  ADD	R5	K6	R2
      0x58180007,  //  000F  LDCONST	R6	K7
      0x7C0C0600,  //  0010  CALL	R3	3
      0xB80E1000,  //  0011  GETNGBL	R3	K8
      0x5C100400,  //  0012  MOVE	R4	R2
      0x7C0C0200,  //  0013  CALL	R3	1
      0xB80E0800,  //  0014  GETNGBL	R3	K4
      0x8C0C0705,  //  0015  GETMET	R3	R3	K5
      0x00161202,  //  0016  ADD	R5	K9	R2
      0x58180007,  //  0017  LDCONST	R6	K7
      0x7C0C0600,  //  0018  CALL	R3	3
      0x80000000,  //  0019  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: reset
********************************************************************/
be_local_closure(reset,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[13]) {     /* constants */
    /* K0   */  be_nested_string("path", -2071507658, 4),
    /* K1   */  be_nested_string("string", 398550328, 6),
    /* K2   */  be_nested_string("listdir", 2005220720, 7),
    /* K3   */  be_nested_string("/", 705468254, 1),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_string("find", -1108310694, 4),
    /* K6   */  be_nested_string(".autoconf", -1770288208, 9),
    /* K7   */  be_nested_string("remove", -611183107, 6),
    /* K8   */  be_nested_string("format", -1180859054, 6),
    /* K9   */  be_nested_string("CFG: removed file '%s'", 2048602473, 22),
    /* K10  */  be_const_int(1),
    /* K11  */  be_nested_string("_archive", -290407892, 8),
    /* K12  */  be_nested_string("_error", 1132109656, 6),
    }),
    (be_nested_const_str("reset", 1695364032, 5)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[35]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0x8C0C0302,  //  0002  GETMET	R3	R1	K2
      0x58140003,  //  0003  LDCONST	R5	K3
      0x7C0C0400,  //  0004  CALL	R3	2
      0x4C100000,  //  0005  LDNIL	R4
      0x58140004,  //  0006  LDCONST	R5	K4
      0x6018000C,  //  0007  GETGBL	R6	G12
      0x5C1C0600,  //  0008  MOVE	R7	R3
      0x7C180200,  //  0009  CALL	R6	1
      0x14180A06,  //  000A  LT	R6	R5	R6
      0x781A0011,  //  000B  JMPF	R6	#001E
      0x94180605,  //  000C  GETIDX	R6	R3	R5
      0x8C1C0505,  //  000D  GETMET	R7	R2	K5
      0x5C240C00,  //  000E  MOVE	R9	R6
      0x58280006,  //  000F  LDCONST	R10	K6
      0x7C1C0600,  //  0010  CALL	R7	3
      0x241C0F04,  //  0011  GT	R7	R7	K4
      0x781E0008,  //  0012  JMPF	R7	#001C
      0x8C1C0307,  //  0013  GETMET	R7	R1	K7
      0x5C240C00,  //  0014  MOVE	R9	R6
      0x7C1C0400,  //  0015  CALL	R7	2
      0x601C0001,  //  0016  GETGBL	R7	G1
      0x8C200508,  //  0017  GETMET	R8	R2	K8
      0x58280009,  //  0018  LDCONST	R10	K9
      0x5C2C0C00,  //  0019  MOVE	R11	R6
      0x7C200600,  //  001A  CALL	R8	3
      0x7C1C0200,  //  001B  CALL	R7	1
      0x00140B0A,  //  001C  ADD	R5	R5	K10
      0x7001FFE8,  //  001D  JMP		#0007
      0x4C180000,  //  001E  LDNIL	R6
      0x90021606,  //  001F  SETMBR	R0	K11	R6
      0x4C180000,  //  0020  LDNIL	R6
      0x90021806,  //  0021  SETMBR	R0	K12	R6
      0x80000000,  //  0022  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: web_add_handler
********************************************************************/
be_local_closure(web_add_handler,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 2]) {
      be_nested_proto(
        2,                          /* nstack */
        0,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 1]) {     /* constants */
        /* K0   */  be_nested_string("page_autoconf_mgr", -651030265, 17),
        }),
        (be_nested_const_str("<lambda>", 607256038, 8)),
        ((bstring*) &be_const_str_input),
        ( &(const binstruction[ 4]) {  /* code */
          0x68000000,  //  0000  GETUPV	R0	U0
          0x8C000100,  //  0001  GETMET	R0	R0	K0
          0x7C000200,  //  0002  CALL	R0	1
          0x80040000,  //  0003  RET	1	R0
        })
      ),
      be_nested_proto(
        2,                          /* nstack */
        0,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 1]) {     /* constants */
        /* K0   */  be_nested_string("page_autoconf_ctl", -1841585800, 17),
        }),
        (be_nested_const_str("<lambda>", 607256038, 8)),
        ((bstring*) &be_const_str_input),
        ( &(const binstruction[ 4]) {  /* code */
          0x68000000,  //  0000  GETUPV	R0	U0
          0x8C000100,  //  0001  GETMET	R0	R0	K0
          0x7C000200,  //  0002  CALL	R0	1
          0x80040000,  //  0003  RET	1	R0
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_string("webserver", 1572454038, 9),
    /* K1   */  be_nested_string("on", 1630810064, 2),
    /* K2   */  be_nested_string("/ac", -390315318, 3),
    /* K3   */  be_nested_string("HTTP_GET", 1722467738, 8),
    /* K4   */  be_nested_string("HTTP_POST", 1999554144, 9),
    }),
    (be_nested_const_str("web_add_handler", -304792334, 15)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[13]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080301,  //  0001  GETMET	R2	R1	K1
      0x58100002,  //  0002  LDCONST	R4	K2
      0x84140000,  //  0003  CLOSURE	R5	P0
      0x88180303,  //  0004  GETMBR	R6	R1	K3
      0x7C080800,  //  0005  CALL	R2	4
      0x8C080301,  //  0006  GETMET	R2	R1	K1
      0x58100002,  //  0007  LDCONST	R4	K2
      0x84140001,  //  0008  CLOSURE	R5	P1
      0x88180304,  //  0009  GETMBR	R6	R1	K4
      0x7C080800,  //  000A  CALL	R2	4
      0xA0000000,  //  000B  CLOSE	R0
      0x80000000,  //  000C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: clear_first_time
********************************************************************/
be_local_closure(clear_first_time,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_string("path", -2071507658, 4),
    /* K1   */  be_nested_string("remove", -611183107, 6),
    /* K2   */  be_nested_string("/.autoconf", -2082892903, 10),
    }),
    (be_nested_const_str("clear_first_time", 632769909, 16)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[ 5]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080301,  //  0001  GETMET	R2	R1	K1
      0x58100002,  //  0002  LDCONST	R4	K2
      0x7C080400,  //  0003  CALL	R2	2
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_current_module_path
********************************************************************/
be_local_closure(get_current_module_path,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_string("_archive", -290407892, 8),
    }),
    (be_nested_const_str("get_current_module_path", -1088293888, 23)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Autoconf
********************************************************************/
be_local_class(Autoconf,
    2,
    NULL,
    be_nested_map(18,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_nested_key("page_autoconf_ctl", -1841585800, 17, -1), be_const_closure(page_autoconf_ctl_closure) },
        { be_nested_key("autoexec", -618105405, 8, -1), be_const_closure(autoexec_closure) },
        { be_nested_key("run_bat", -1758063998, 7, 6), be_const_closure(run_bat_closure) },
        { be_nested_key("page_autoconf_mgr", -651030265, 17, -1), be_const_closure(page_autoconf_mgr_closure) },
        { be_nested_key("get_current_module_name", -1915696556, 23, -1), be_const_closure(get_current_module_name_closure) },
        { be_nested_key("delete_all_configs", -1912899718, 18, 13), be_const_closure(delete_all_configs_closure) },
        { be_nested_key("set_first_time", -1183719746, 14, -1), be_const_closure(set_first_time_closure) },
        { be_nested_key("load_templates", -781097163, 14, -1), be_const_closure(load_templates_closure) },
        { be_nested_key("_archive", -290407892, 8, -1), be_const_var(0) },
        { be_nested_key("web_add_config_button", 639674325, 21, -1), be_const_closure(web_add_config_button_closure) },
        { be_nested_key("is_first_time", 275242384, 13, -1), be_const_closure(is_first_time_closure) },
        { be_nested_key("init", 380752755, 4, -1), be_const_closure(init_closure) },
        { be_nested_key("preinit", -1572960196, 7, 5), be_const_closure(preinit_closure) },
        { be_nested_key("reset", 1695364032, 5, 17), be_const_closure(reset_closure) },
        { be_nested_key("web_add_handler", -304792334, 15, 4), be_const_closure(web_add_handler_closure) },
        { be_nested_key("clear_first_time", 632769909, 16, 11), be_const_closure(clear_first_time_closure) },
        { be_nested_key("_error", 1132109656, 6, -1), be_const_var(1) },
        { be_nested_key("get_current_module_path", -1088293888, 23, -1), be_const_closure(get_current_module_path_closure) },
    })),
    (be_nested_const_str("Autoconf", 984011268, 8))
);

/********************************************************************
** Solidified function: _anonymous_
********************************************************************/
be_local_closure(_anonymous_,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_const_class(be_class_Autoconf),
    }),
    (be_nested_const_str("_anonymous_", 1957281476, 11)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[ 5]) {  /* code */
      0x58040000,  //  0000  LDCONST	R1	K0
      0xB4000000,  //  0001  CLASS	K0
      0x5C080200,  //  0002  MOVE	R2	R1
      0x7C080000,  //  0003  CALL	R2	0
      0x80040400,  //  0004  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified module: autoconf
********************************************************************/
be_local_module(autoconf,
    "autoconf",
    be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_nested_key("init", 380752755, 4, -1), be_const_closure(_anonymous__closure) },
    }))
);
BE_EXPORT_VARIABLE be_define_const_native_module(autoconf);
/********************************************************************/
