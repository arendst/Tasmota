/********************************************************************
 * Tasmota lib
 *
 * To use: `import tasmota`
 *******************************************************************/
#include "be_constobj.h"
#include "be_ctypes.h"

extern struct TasmotaGlobal_t TasmotaGlobal;
extern struct TSettings * Settings;
extern const be_ctypes_structure_t be_tasmota_global_struct;
extern const be_ctypes_structure_t be_tasmota_settings_struct;

extern int l_getFreeHeap(bvm *vm);
extern int l_arch(bvm *vm);
extern int l_publish(bvm *vm);
extern int l_publish_result(bvm *vm);
extern int l_cmd(bvm *vm);
extern int l_getoption(bvm *vm);
extern int l_millis(bvm *vm);
extern int l_timereached(bvm *vm);
extern int l_rtc(bvm *vm);
extern int l_time_dump(bvm *vm);
extern int l_strftime(bvm *vm);
extern int l_strptime(bvm *vm);
extern int l_memory(bvm *vm);
extern int l_wifi(bvm *vm);
extern int l_eth(bvm *vm);
extern int l_yield(bvm *vm);
extern int l_delay(bvm *vm);
extern int l_scaleuint(bvm *vm);
extern int l_logInfo(bvm *vm);
extern int l_save(bvm *vm);
extern int t_random_byte(bvm *vm);

extern int l_read_sensors(bvm *vm);

extern int l_respCmnd(bvm *vm);
extern int l_respCmndStr(bvm *vm);
extern int l_respCmndDone(bvm *vm);
extern int l_respCmndError(bvm *vm);
extern int l_respCmndFailed(bvm *vm);
extern int l_resolveCmnd(bvm *vm);

extern int l_respAppend(bvm *vm);
extern int l_webSend(bvm *vm);
extern int l_webSendDecimal(bvm *vm);

extern int l_getlight(bvm *vm);
extern int l_setlight(bvm *vm);
extern int l_getpower(bvm *vm);
extern int l_setpower(bvm *vm);
extern int l_getswitch(bvm *vm);

extern int l_i2cenabled(bvm *vm);


/********************************************************************
** Solidified function: add_driver
********************************************************************/
be_local_closure(Tasmota_add_driver,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str(instance),
    /* K1   */  be_nested_str(value_error),
    /* K2   */  be_nested_str(instance_X20required),
    /* K3   */  be_nested_str(_drivers),
    /* K4   */  be_nested_str(push),
    }),
    &be_const_str_add_driver,
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x60080004,  //  0000  GETGBL	R2	G4
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x7C080200,  //  0002  CALL	R2	1
      0x20080500,  //  0003  NE	R2	R2	K0
      0x780A0000,  //  0004  JMPF	R2	#0006
      0xB0060302,  //  0005  RAISE	1	K1	K2
      0x88080103,  //  0006  GETMBR	R2	R0	K3
      0x780A0004,  //  0007  JMPF	R2	#000D
      0x88080103,  //  0008  GETMBR	R2	R0	K3
      0x8C080504,  //  0009  GETMET	R2	R2	K4
      0x5C100200,  //  000A  MOVE	R4	R1
      0x7C080400,  //  000B  CALL	R2	2
      0x70020003,  //  000C  JMP		#0011
      0x60080012,  //  000D  GETGBL	R2	G18
      0x7C080000,  //  000E  CALL	R2	0
      0x400C0401,  //  000F  CONNECT	R3	R2	R1
      0x90020602,  //  0010  SETMBR	R0	K3	R2
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: gc
********************************************************************/
be_local_closure(Tasmota_gc,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str(gc),
    /* K1   */  be_nested_str(collect),
    /* K2   */  be_nested_str(allocated),
    }),
    &be_const_str_gc,
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080301,  //  0001  GETMET	R2	R1	K1
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080302,  //  0003  GETMET	R2	R1	K2
      0x7C080200,  //  0004  CALL	R2	1
      0x80040400,  //  0005  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: find_op
********************************************************************/
be_local_closure(Tasmota_find_op,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str(string),
    /* K1   */  be_nested_str(_X3D_X3C_X3E_X21),
    /* K2   */  be_nested_str(chars_in_string),
    /* K3   */  be_const_int(0),
    /* K4   */  be_nested_str(split),
    /* K5   */  be_const_int(1),
    }),
    &be_const_str_find_op,
    &be_const_str_solidified,
    ( &(const binstruction[42]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x580C0001,  //  0001  LDCONST	R3	K1
      0x8C100102,  //  0002  GETMET	R4	R0	K2
      0x5C180200,  //  0003  MOVE	R6	R1
      0x5C1C0600,  //  0004  MOVE	R7	R3
      0x7C100600,  //  0005  CALL	R4	3
      0x28140903,  //  0006  GE	R5	R4	K3
      0x78160019,  //  0007  JMPF	R5	#0022
      0x8C140504,  //  0008  GETMET	R5	R2	K4
      0x5C1C0200,  //  0009  MOVE	R7	R1
      0x5C200800,  //  000A  MOVE	R8	R4
      0x7C140600,  //  000B  CALL	R5	3
      0x94180B03,  //  000C  GETIDX	R6	R5	K3
      0x941C0B05,  //  000D  GETIDX	R7	R5	K5
      0x8C200102,  //  000E  GETMET	R8	R0	K2
      0x5C280E00,  //  000F  MOVE	R10	R7
      0x5C2C0600,  //  0010  MOVE	R11	R3
      0x50300200,  //  0011  LDBOOL	R12	1	0
      0x7C200800,  //  0012  CALL	R8	4
      0x5C101000,  //  0013  MOVE	R4	R8
      0x28200903,  //  0014  GE	R8	R4	K3
      0x7822000B,  //  0015  JMPF	R8	#0022
      0x8C200504,  //  0016  GETMET	R8	R2	K4
      0x5C280E00,  //  0017  MOVE	R10	R7
      0x5C2C0800,  //  0018  MOVE	R11	R4
      0x7C200600,  //  0019  CALL	R8	3
      0x94241103,  //  001A  GETIDX	R9	R8	K3
      0x94281105,  //  001B  GETIDX	R10	R8	K5
      0x602C0012,  //  001C  GETGBL	R11	G18
      0x7C2C0000,  //  001D  CALL	R11	0
      0x40301606,  //  001E  CONNECT	R12	R11	R6
      0x40301609,  //  001F  CONNECT	R12	R11	R9
      0x4030160A,  //  0020  CONNECT	R12	R11	R10
      0x80041600,  //  0021  RET	1	R11
      0x60140012,  //  0022  GETGBL	R5	G18
      0x7C140000,  //  0023  CALL	R5	0
      0x40180A01,  //  0024  CONNECT	R6	R5	R1
      0x4C180000,  //  0025  LDNIL	R6
      0x40180A06,  //  0026  CONNECT	R6	R5	R6
      0x4C180000,  //  0027  LDNIL	R6
      0x40180A06,  //  0028  CONNECT	R6	R5	R6
      0x80040A00,  //  0029  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: try_rule
********************************************************************/
be_local_closure(Tasmota_try_rule,   /* name */
  be_nested_proto(
    15,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[16]) {     /* constants */
    /* K0   */  be_nested_str(string),
    /* K1   */  be_nested_str(find_op),
    /* K2   */  be_nested_str(split),
    /* K3   */  be_const_int(0),
    /* K4   */  be_nested_str(_X23),
    /* K5   */  be_nested_str(find_key_i),
    /* K6   */  be_const_int(1),
    /* K7   */  be_const_int(2),
    /* K8   */  be_nested_str(_X3D_X3D),
    /* K9   */  be_nested_str(_X21_X3D_X3D),
    /* K10  */  be_nested_str(_X3D),
    /* K11  */  be_nested_str(_X21_X3D),
    /* K12  */  be_nested_str(_X3E),
    /* K13  */  be_nested_str(_X3E_X3D),
    /* K14  */  be_nested_str(_X3C),
    /* K15  */  be_nested_str(_X3C_X3D),
    }),
    &be_const_str_try_rule,
    &be_const_str_solidified,
    ( &(const binstruction[141]) {  /* code */
      0xA4120000,  //  0000  IMPORT	R4	K0
      0x8C140101,  //  0001  GETMET	R5	R0	K1
      0x5C1C0400,  //  0002  MOVE	R7	R2
      0x7C140400,  //  0003  CALL	R5	2
      0x5C180200,  //  0004  MOVE	R6	R1
      0x8C1C0902,  //  0005  GETMET	R7	R4	K2
      0x94240B03,  //  0006  GETIDX	R9	R5	K3
      0x58280004,  //  0007  LDCONST	R10	K4
      0x7C1C0600,  //  0008  CALL	R7	3
      0x58200003,  //  0009  LDCONST	R8	K3
      0x6024000C,  //  000A  GETGBL	R9	G12
      0x5C280E00,  //  000B  MOVE	R10	R7
      0x7C240200,  //  000C  CALL	R9	1
      0x14241009,  //  000D  LT	R9	R8	R9
      0x7826000C,  //  000E  JMPF	R9	#001C
      0x94240E08,  //  000F  GETIDX	R9	R7	R8
      0x8C280105,  //  0010  GETMET	R10	R0	K5
      0x5C300C00,  //  0011  MOVE	R12	R6
      0x5C341200,  //  0012  MOVE	R13	R9
      0x7C280600,  //  0013  CALL	R10	3
      0x4C2C0000,  //  0014  LDNIL	R11
      0x1C2C140B,  //  0015  EQ	R11	R10	R11
      0x782E0001,  //  0016  JMPF	R11	#0019
      0x502C0000,  //  0017  LDBOOL	R11	0	0
      0x80041600,  //  0018  RET	1	R11
      0x94180C0A,  //  0019  GETIDX	R6	R6	R10
      0x00201106,  //  001A  ADD	R8	R8	K6
      0x7001FFED,  //  001B  JMP		#000A
      0x94240B06,  //  001C  GETIDX	R9	R5	K6
      0x94280B07,  //  001D  GETIDX	R10	R5	K7
      0x78260066,  //  001E  JMPF	R9	#0086
      0x1C2C1308,  //  001F  EQ	R11	R9	K8
      0x782E000A,  //  0020  JMPF	R11	#002C
      0x602C0008,  //  0021  GETGBL	R11	G8
      0x5C300C00,  //  0022  MOVE	R12	R6
      0x7C2C0200,  //  0023  CALL	R11	1
      0x60300008,  //  0024  GETGBL	R12	G8
      0x5C341400,  //  0025  MOVE	R13	R10
      0x7C300200,  //  0026  CALL	R12	1
      0x202C160C,  //  0027  NE	R11	R11	R12
      0x782E0001,  //  0028  JMPF	R11	#002B
      0x502C0000,  //  0029  LDBOOL	R11	0	0
      0x80041600,  //  002A  RET	1	R11
      0x70020059,  //  002B  JMP		#0086
      0x1C2C1309,  //  002C  EQ	R11	R9	K9
      0x782E000A,  //  002D  JMPF	R11	#0039
      0x602C0008,  //  002E  GETGBL	R11	G8
      0x5C300C00,  //  002F  MOVE	R12	R6
      0x7C2C0200,  //  0030  CALL	R11	1
      0x60300008,  //  0031  GETGBL	R12	G8
      0x5C341400,  //  0032  MOVE	R13	R10
      0x7C300200,  //  0033  CALL	R12	1
      0x1C2C160C,  //  0034  EQ	R11	R11	R12
      0x782E0001,  //  0035  JMPF	R11	#0038
      0x502C0000,  //  0036  LDBOOL	R11	0	0
      0x80041600,  //  0037  RET	1	R11
      0x7002004C,  //  0038  JMP		#0086
      0x1C2C130A,  //  0039  EQ	R11	R9	K10
      0x782E000A,  //  003A  JMPF	R11	#0046
      0x602C000A,  //  003B  GETGBL	R11	G10
      0x5C300C00,  //  003C  MOVE	R12	R6
      0x7C2C0200,  //  003D  CALL	R11	1
      0x6030000A,  //  003E  GETGBL	R12	G10
      0x5C341400,  //  003F  MOVE	R13	R10
      0x7C300200,  //  0040  CALL	R12	1
      0x202C160C,  //  0041  NE	R11	R11	R12
      0x782E0001,  //  0042  JMPF	R11	#0045
      0x502C0000,  //  0043  LDBOOL	R11	0	0
      0x80041600,  //  0044  RET	1	R11
      0x7002003F,  //  0045  JMP		#0086
      0x1C2C130B,  //  0046  EQ	R11	R9	K11
      0x782E000A,  //  0047  JMPF	R11	#0053
      0x602C000A,  //  0048  GETGBL	R11	G10
      0x5C300C00,  //  0049  MOVE	R12	R6
      0x7C2C0200,  //  004A  CALL	R11	1
      0x6030000A,  //  004B  GETGBL	R12	G10
      0x5C341400,  //  004C  MOVE	R13	R10
      0x7C300200,  //  004D  CALL	R12	1
      0x1C2C160C,  //  004E  EQ	R11	R11	R12
      0x782E0001,  //  004F  JMPF	R11	#0052
      0x502C0000,  //  0050  LDBOOL	R11	0	0
      0x80041600,  //  0051  RET	1	R11
      0x70020032,  //  0052  JMP		#0086
      0x1C2C130C,  //  0053  EQ	R11	R9	K12
      0x782E000A,  //  0054  JMPF	R11	#0060
      0x602C000A,  //  0055  GETGBL	R11	G10
      0x5C300C00,  //  0056  MOVE	R12	R6
      0x7C2C0200,  //  0057  CALL	R11	1
      0x6030000A,  //  0058  GETGBL	R12	G10
      0x5C341400,  //  0059  MOVE	R13	R10
      0x7C300200,  //  005A  CALL	R12	1
      0x182C160C,  //  005B  LE	R11	R11	R12
      0x782E0001,  //  005C  JMPF	R11	#005F
      0x502C0000,  //  005D  LDBOOL	R11	0	0
      0x80041600,  //  005E  RET	1	R11
      0x70020025,  //  005F  JMP		#0086
      0x1C2C130D,  //  0060  EQ	R11	R9	K13
      0x782E000A,  //  0061  JMPF	R11	#006D
      0x602C000A,  //  0062  GETGBL	R11	G10
      0x5C300C00,  //  0063  MOVE	R12	R6
      0x7C2C0200,  //  0064  CALL	R11	1
      0x6030000A,  //  0065  GETGBL	R12	G10
      0x5C341400,  //  0066  MOVE	R13	R10
      0x7C300200,  //  0067  CALL	R12	1
      0x142C160C,  //  0068  LT	R11	R11	R12
      0x782E0001,  //  0069  JMPF	R11	#006C
      0x502C0000,  //  006A  LDBOOL	R11	0	0
      0x80041600,  //  006B  RET	1	R11
      0x70020018,  //  006C  JMP		#0086
      0x1C2C130E,  //  006D  EQ	R11	R9	K14
      0x782E000A,  //  006E  JMPF	R11	#007A
      0x602C000A,  //  006F  GETGBL	R11	G10
      0x5C300C00,  //  0070  MOVE	R12	R6
      0x7C2C0200,  //  0071  CALL	R11	1
      0x6030000A,  //  0072  GETGBL	R12	G10
      0x5C341400,  //  0073  MOVE	R13	R10
      0x7C300200,  //  0074  CALL	R12	1
      0x282C160C,  //  0075  GE	R11	R11	R12
      0x782E0001,  //  0076  JMPF	R11	#0079
      0x502C0000,  //  0077  LDBOOL	R11	0	0
      0x80041600,  //  0078  RET	1	R11
      0x7002000B,  //  0079  JMP		#0086
      0x1C2C130F,  //  007A  EQ	R11	R9	K15
      0x782E0009,  //  007B  JMPF	R11	#0086
      0x602C000A,  //  007C  GETGBL	R11	G10
      0x5C300C00,  //  007D  MOVE	R12	R6
      0x7C2C0200,  //  007E  CALL	R11	1
      0x6030000A,  //  007F  GETGBL	R12	G10
      0x5C341400,  //  0080  MOVE	R13	R10
      0x7C300200,  //  0081  CALL	R12	1
      0x242C160C,  //  0082  GT	R11	R11	R12
      0x782E0001,  //  0083  JMPF	R11	#0086
      0x502C0000,  //  0084  LDBOOL	R11	0	0
      0x80041600,  //  0085  RET	1	R11
      0x5C2C0600,  //  0086  MOVE	R11	R3
      0x5C300C00,  //  0087  MOVE	R12	R6
      0x94340B03,  //  0088  GETIDX	R13	R5	K3
      0x5C380200,  //  0089  MOVE	R14	R1
      0x7C2C0600,  //  008A  CALL	R11	3
      0x502C0200,  //  008B  LDBOOL	R11	1	0
      0x80041600,  //  008C  RET	1	R11
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: gen_cb
********************************************************************/
be_local_closure(Tasmota_gen_cb,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str(cb),
    /* K1   */  be_nested_str(gen_cb),
    }),
    &be_const_str_gen_cb,
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x8C0C0501,  //  0001  GETMET	R3	R2	K1
      0x5C140200,  //  0002  MOVE	R5	R1
      0x7C0C0400,  //  0003  CALL	R3	2
      0x80040600,  //  0004  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_light
********************************************************************/
be_local_closure(Tasmota_set_light,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str(tasmota_X2Eset_light_X28_X29_X20is_X20deprecated_X2C_X20use_X20light_X2Eset_X28_X29),
    /* K1   */  be_nested_str(light),
    /* K2   */  be_nested_str(set),
    }),
    &be_const_str_set_light,
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x600C0001,  //  0000  GETGBL	R3	G1
      0x58100000,  //  0001  LDCONST	R4	K0
      0x7C0C0200,  //  0002  CALL	R3	1
      0xA40E0200,  //  0003  IMPORT	R3	K1
      0x4C100000,  //  0004  LDNIL	R4
      0x20100404,  //  0005  NE	R4	R2	R4
      0x78120005,  //  0006  JMPF	R4	#000D
      0x8C100702,  //  0007  GETMET	R4	R3	K2
      0x5C180200,  //  0008  MOVE	R6	R1
      0x5C1C0400,  //  0009  MOVE	R7	R2
      0x7C100600,  //  000A  CALL	R4	3
      0x80040800,  //  000B  RET	1	R4
      0x70020003,  //  000C  JMP		#0011
      0x8C100702,  //  000D  GETMET	R4	R3	K2
      0x5C180200,  //  000E  MOVE	R6	R1
      0x7C100400,  //  000F  CALL	R4	2
      0x80040800,  //  0010  RET	1	R4
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: exec_tele
********************************************************************/
be_local_closure(Tasmota_exec_tele,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[12]) {     /* constants */
    /* K0   */  be_nested_str(_rules),
    /* K1   */  be_nested_str(json),
    /* K2   */  be_nested_str(load),
    /* K3   */  be_nested_str(log),
    /* K4   */  be_nested_str(BRY_X3A_X20ERROR_X2C_X20bad_X20json_X3A_X20),
    /* K5   */  be_const_int(3),
    /* K6   */  be_nested_str(Tele),
    /* K7   */  be_const_int(0),
    /* K8   */  be_nested_str(try_rule),
    /* K9   */  be_nested_str(k),
    /* K10  */  be_nested_str(v),
    /* K11  */  be_const_int(1),
    }),
    &be_const_str_exec_tele,
    &be_const_str_solidified,
    ( &(const binstruction[41]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x780A0024,  //  0001  JMPF	R2	#0027
      0xA40A0200,  //  0002  IMPORT	R2	K1
      0x8C0C0502,  //  0003  GETMET	R3	R2	K2
      0x5C140200,  //  0004  MOVE	R5	R1
      0x7C0C0400,  //  0005  CALL	R3	2
      0x50100000,  //  0006  LDBOOL	R4	0	0
      0x4C140000,  //  0007  LDNIL	R5
      0x1C140605,  //  0008  EQ	R5	R3	R5
      0x78160004,  //  0009  JMPF	R5	#000F
      0x8C140103,  //  000A  GETMET	R5	R0	K3
      0x001E0801,  //  000B  ADD	R7	K4	R1
      0x58200005,  //  000C  LDCONST	R8	K5
      0x7C140600,  //  000D  CALL	R5	3
      0x5C0C0200,  //  000E  MOVE	R3	R1
      0x60140013,  //  000F  GETGBL	R5	G19
      0x7C140000,  //  0010  CALL	R5	0
      0x98160C03,  //  0011  SETIDX	R5	K6	R3
      0x5C0C0A00,  //  0012  MOVE	R3	R5
      0x58140007,  //  0013  LDCONST	R5	K7
      0x6018000C,  //  0014  GETGBL	R6	G12
      0x881C0100,  //  0015  GETMBR	R7	R0	K0
      0x7C180200,  //  0016  CALL	R6	1
      0x14180A06,  //  0017  LT	R6	R5	R6
      0x781A000C,  //  0018  JMPF	R6	#0026
      0x88180100,  //  0019  GETMBR	R6	R0	K0
      0x94180C05,  //  001A  GETIDX	R6	R6	R5
      0x8C1C0108,  //  001B  GETMET	R7	R0	K8
      0x5C240600,  //  001C  MOVE	R9	R3
      0x88280D09,  //  001D  GETMBR	R10	R6	K9
      0x882C0D0A,  //  001E  GETMBR	R11	R6	K10
      0x7C1C0800,  //  001F  CALL	R7	4
      0x741E0001,  //  0020  JMPT	R7	#0023
      0x74120000,  //  0021  JMPT	R4	#0023
      0x50100001,  //  0022  LDBOOL	R4	0	1
      0x50100200,  //  0023  LDBOOL	R4	1	0
      0x00140B0B,  //  0024  ADD	R5	R5	K11
      0x7001FFED,  //  0025  JMP		#0014
      0x80040800,  //  0026  RET	1	R4
      0x50080000,  //  0027  LDBOOL	R2	0	0
      0x80040400,  //  0028  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: run_deferred
********************************************************************/
be_local_closure(Tasmota_run_deferred,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str(_timers),
    /* K1   */  be_const_int(0),
    /* K2   */  be_nested_str(size),
    /* K3   */  be_nested_str(time_reached),
    /* K4   */  be_nested_str(due),
    /* K5   */  be_nested_str(f),
    /* K6   */  be_nested_str(remove),
    /* K7   */  be_const_int(1),
    }),
    &be_const_str_run_deferred,
    &be_const_str_solidified,
    ( &(const binstruction[27]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x78060017,  //  0001  JMPF	R1	#001A
      0x58040001,  //  0002  LDCONST	R1	K1
      0x88080100,  //  0003  GETMBR	R2	R0	K0
      0x8C080502,  //  0004  GETMET	R2	R2	K2
      0x7C080200,  //  0005  CALL	R2	1
      0x14080202,  //  0006  LT	R2	R1	R2
      0x780A0011,  //  0007  JMPF	R2	#001A
      0x8C080103,  //  0008  GETMET	R2	R0	K3
      0x88100100,  //  0009  GETMBR	R4	R0	K0
      0x94100801,  //  000A  GETIDX	R4	R4	R1
      0x88100904,  //  000B  GETMBR	R4	R4	K4
      0x7C080400,  //  000C  CALL	R2	2
      0x780A0009,  //  000D  JMPF	R2	#0018
      0x88080100,  //  000E  GETMBR	R2	R0	K0
      0x94080401,  //  000F  GETIDX	R2	R2	R1
      0x88080505,  //  0010  GETMBR	R2	R2	K5
      0x880C0100,  //  0011  GETMBR	R3	R0	K0
      0x8C0C0706,  //  0012  GETMET	R3	R3	K6
      0x5C140200,  //  0013  MOVE	R5	R1
      0x7C0C0400,  //  0014  CALL	R3	2
      0x5C0C0400,  //  0015  MOVE	R3	R2
      0x7C0C0000,  //  0016  CALL	R3	0
      0x70020000,  //  0017  JMP		#0019
      0x00040307,  //  0018  ADD	R1	R1	K7
      0x7001FFE8,  //  0019  JMP		#0003
      0x80000000,  //  001A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: remove_driver
********************************************************************/
be_local_closure(Tasmota_remove_driver,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str(_drivers),
    /* K1   */  be_nested_str(find),
    /* K2   */  be_nested_str(pop),
    }),
    &be_const_str_remove_driver,
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x780A000A,  //  0001  JMPF	R2	#000D
      0x88080100,  //  0002  GETMBR	R2	R0	K0
      0x8C080501,  //  0003  GETMET	R2	R2	K1
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x4C0C0000,  //  0006  LDNIL	R3
      0x200C0403,  //  0007  NE	R3	R2	R3
      0x780E0003,  //  0008  JMPF	R3	#000D
      0x880C0100,  //  0009  GETMBR	R3	R0	K0
      0x8C0C0702,  //  000A  GETMET	R3	R3	K2
      0x5C140400,  //  000B  MOVE	R5	R2
      0x7C0C0400,  //  000C  CALL	R3	2
      0x80000000,  //  000D  RET	0
    })
  )
);
/*******************************************************************/

/********************************************************************
** Solidified function: fast_loop
********************************************************************/
be_local_closure(Tasmota_fast_loop,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str(_fl),
    /* K1   */  be_const_int(0),
    /* K2   */  be_const_int(1),
    }),
    &be_const_str_fast_loop,
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x5C080200,  //  0001  MOVE	R2	R1
      0x740A0000,  //  0002  JMPT	R2	#0004
      0x80000400,  //  0003  RET	0
      0x58080001,  //  0004  LDCONST	R2	K1
      0x600C000C,  //  0005  GETGBL	R3	G12
      0x5C100200,  //  0006  MOVE	R4	R1
      0x7C0C0200,  //  0007  CALL	R3	1
      0x140C0403,  //  0008  LT	R3	R2	R3
      0x780E0003,  //  0009  JMPF	R3	#000E
      0x940C0202,  //  000A  GETIDX	R3	R1	R2
      0x7C0C0000,  //  000B  CALL	R3	0
      0x00080502,  //  000C  ADD	R2	R2	K2
      0x7001FFF6,  //  000D  JMP		#0005
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: add_fast_loop
********************************************************************/
be_local_closure(Tasmota_add_fast_loop,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str(check_not_method),
    /* K1   */  be_nested_str(_fl),
    /* K2   */  be_nested_str(function),
    /* K3   */  be_nested_str(value_error),
    /* K4   */  be_nested_str(argument_X20must_X20be_X20a_X20function),
    /* K5   */  be_nested_str(global),
    /* K6   */  be_nested_str(fast_loop_enabled),
    /* K7   */  be_const_int(1),
    /* K8   */  be_nested_str(push),
    }),
    &be_const_str_add_fast_loop,
    &be_const_str_solidified,
    ( &(const binstruction[21]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x5C100200,  //  0001  MOVE	R4	R1
      0x7C080400,  //  0002  CALL	R2	2
      0x88080101,  //  0003  GETMBR	R2	R0	K1
      0x740A0002,  //  0004  JMPT	R2	#0008
      0x60080012,  //  0005  GETGBL	R2	G18
      0x7C080000,  //  0006  CALL	R2	0
      0x90020202,  //  0007  SETMBR	R0	K1	R2
      0x60080004,  //  0008  GETGBL	R2	G4
      0x5C0C0200,  //  0009  MOVE	R3	R1
      0x7C080200,  //  000A  CALL	R2	1
      0x20080502,  //  000B  NE	R2	R2	K2
      0x780A0000,  //  000C  JMPF	R2	#000E
      0xB0060704,  //  000D  RAISE	1	K3	K4
      0x88080105,  //  000E  GETMBR	R2	R0	K5
      0x900A0D07,  //  000F  SETMBR	R2	K6	K7
      0x88080101,  //  0010  GETMBR	R2	R0	K1
      0x8C080508,  //  0011  GETMET	R2	R2	K8
      0x5C100200,  //  0012  MOVE	R4	R1
      0x7C080400,  //  0013  CALL	R2	2
      0x80000000,  //  0014  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: event
********************************************************************/
be_local_closure(Tasmota_event,   /* name */
  be_nested_proto(
    20,                          /* nstack */
    6,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[23]) {     /* constants */
    /* K0   */  be_nested_str(introspect),
    /* K1   */  be_nested_str(string),
    /* K2   */  be_nested_str(every_50ms),
    /* K3   */  be_nested_str(run_deferred),
    /* K4   */  be_nested_str(cmd),
    /* K5   */  be_nested_str(exec_cmd),
    /* K6   */  be_nested_str(tele),
    /* K7   */  be_nested_str(exec_tele),
    /* K8   */  be_nested_str(rule),
    /* K9   */  be_nested_str(exec_rules),
    /* K10  */  be_nested_str(gc),
    /* K11  */  be_nested_str(_drivers),
    /* K12  */  be_const_int(0),
    /* K13  */  be_nested_str(get),
    /* K14  */  be_nested_str(function),
    /* K15  */  be_nested_str(format),
    /* K16  */  be_nested_str(BRY_X3A_X20Exception_X3E_X20_X27_X25s_X27_X20_X2D_X20_X25s),
    /* K17  */  be_nested_str(debug),
    /* K18  */  be_nested_str(traceback),
    /* K19  */  be_const_int(1),
    /* K20  */  be_nested_str(save_before_restart),
    /* K21  */  be_nested_str(persist),
    /* K22  */  be_nested_str(save),
    }),
    &be_const_str_event,
    &be_const_str_solidified,
    ( &(const binstruction[91]) {  /* code */
      0xA41A0000,  //  0000  IMPORT	R6	K0
      0xA41E0200,  //  0001  IMPORT	R7	K1
      0x1C200302,  //  0002  EQ	R8	R1	K2
      0x78220001,  //  0003  JMPF	R8	#0006
      0x8C200103,  //  0004  GETMET	R8	R0	K3
      0x7C200200,  //  0005  CALL	R8	1
      0x50200000,  //  0006  LDBOOL	R8	0	0
      0x1C240304,  //  0007  EQ	R9	R1	K4
      0x78260006,  //  0008  JMPF	R9	#0010
      0x8C240105,  //  0009  GETMET	R9	R0	K5
      0x5C2C0400,  //  000A  MOVE	R11	R2
      0x5C300600,  //  000B  MOVE	R12	R3
      0x5C340800,  //  000C  MOVE	R13	R4
      0x7C240800,  //  000D  CALL	R9	4
      0x80041200,  //  000E  RET	1	R9
      0x70020044,  //  000F  JMP		#0055
      0x1C240306,  //  0010  EQ	R9	R1	K6
      0x78260004,  //  0011  JMPF	R9	#0017
      0x8C240107,  //  0012  GETMET	R9	R0	K7
      0x5C2C0800,  //  0013  MOVE	R11	R4
      0x7C240400,  //  0014  CALL	R9	2
      0x80041200,  //  0015  RET	1	R9
      0x7002003D,  //  0016  JMP		#0055
      0x1C240308,  //  0017  EQ	R9	R1	K8
      0x78260004,  //  0018  JMPF	R9	#001E
      0x8C240109,  //  0019  GETMET	R9	R0	K9
      0x5C2C0800,  //  001A  MOVE	R11	R4
      0x7C240400,  //  001B  CALL	R9	2
      0x80041200,  //  001C  RET	1	R9
      0x70020036,  //  001D  JMP		#0055
      0x1C24030A,  //  001E  EQ	R9	R1	K10
      0x78260003,  //  001F  JMPF	R9	#0024
      0x8C24010A,  //  0020  GETMET	R9	R0	K10
      0x7C240200,  //  0021  CALL	R9	1
      0x80041200,  //  0022  RET	1	R9
      0x70020030,  //  0023  JMP		#0055
      0x8824010B,  //  0024  GETMBR	R9	R0	K11
      0x7826002E,  //  0025  JMPF	R9	#0055
      0x5824000C,  //  0026  LDCONST	R9	K12
      0x6028000C,  //  0027  GETGBL	R10	G12
      0x882C010B,  //  0028  GETMBR	R11	R0	K11
      0x7C280200,  //  0029  CALL	R10	1
      0x1428120A,  //  002A  LT	R10	R9	R10
      0x782A0028,  //  002B  JMPF	R10	#0055
      0x8828010B,  //  002C  GETMBR	R10	R0	K11
      0x94281409,  //  002D  GETIDX	R10	R10	R9
      0x8C2C0D0D,  //  002E  GETMET	R11	R6	K13
      0x5C341400,  //  002F  MOVE	R13	R10
      0x5C380200,  //  0030  MOVE	R14	R1
      0x7C2C0600,  //  0031  CALL	R11	3
      0x60300004,  //  0032  GETGBL	R12	G4
      0x5C341600,  //  0033  MOVE	R13	R11
      0x7C300200,  //  0034  CALL	R12	1
      0x1C30190E,  //  0035  EQ	R12	R12	K14
      0x7832001B,  //  0036  JMPF	R12	#0053
      0xA802000C,  //  0037  EXBLK	0	#0045
      0x5C301600,  //  0038  MOVE	R12	R11
      0x5C341400,  //  0039  MOVE	R13	R10
      0x5C380400,  //  003A  MOVE	R14	R2
      0x5C3C0600,  //  003B  MOVE	R15	R3
      0x5C400800,  //  003C  MOVE	R16	R4
      0x5C440A00,  //  003D  MOVE	R17	R5
      0x7C300A00,  //  003E  CALL	R12	5
      0x5C201800,  //  003F  MOVE	R8	R12
      0x78220001,  //  0040  JMPF	R8	#0043
      0xA8040001,  //  0041  EXBLK	1	1
      0x70020011,  //  0042  JMP		#0055
      0xA8040001,  //  0043  EXBLK	1	1
      0x7002000D,  //  0044  JMP		#0053
      0xAC300002,  //  0045  CATCH	R12	0	2
      0x7002000A,  //  0046  JMP		#0052
      0x60380001,  //  0047  GETGBL	R14	G1
      0x8C3C0F0F,  //  0048  GETMET	R15	R7	K15
      0x58440010,  //  0049  LDCONST	R17	K16
      0x5C481800,  //  004A  MOVE	R18	R12
      0x5C4C1A00,  //  004B  MOVE	R19	R13
      0x7C3C0800,  //  004C  CALL	R15	4
      0x7C380200,  //  004D  CALL	R14	1
      0xA43A2200,  //  004E  IMPORT	R14	K17
      0x8C3C1D12,  //  004F  GETMET	R15	R14	K18
      0x7C3C0200,  //  0050  CALL	R15	1
      0x70020000,  //  0051  JMP		#0053
      0xB0080000,  //  0052  RAISE	2	R0	R0
      0x00241313,  //  0053  ADD	R9	R9	K19
      0x7001FFD1,  //  0054  JMP		#0027
      0x1C240314,  //  0055  EQ	R9	R1	K20
      0x78260002,  //  0056  JMPF	R9	#005A
      0xA4262A00,  //  0057  IMPORT	R9	K21
      0x8C281316,  //  0058  GETMET	R10	R9	K22
      0x7C280200,  //  0059  CALL	R10	1
      0x80041000,  //  005A  RET	1	R8
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: find_key_i
********************************************************************/
be_local_closure(Tasmota_find_key_i,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str(string),
    /* K1   */  be_nested_str(toupper),
    /* K2   */  be_nested_str(keys),
    /* K3   */  be_nested_str(_X3F),
    /* K4   */  be_nested_str(stop_iteration),
    }),
    &be_const_str_find_key_i,
    &be_const_str_solidified,
    ( &(const binstruction[30]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0x8C100701,  //  0001  GETMET	R4	R3	K1
      0x5C180400,  //  0002  MOVE	R6	R2
      0x7C100400,  //  0003  CALL	R4	2
      0x6014000F,  //  0004  GETGBL	R5	G15
      0x5C180200,  //  0005  MOVE	R6	R1
      0x601C0013,  //  0006  GETGBL	R7	G19
      0x7C140400,  //  0007  CALL	R5	2
      0x78160013,  //  0008  JMPF	R5	#001D
      0x60140010,  //  0009  GETGBL	R5	G16
      0x8C180302,  //  000A  GETMET	R6	R1	K2
      0x7C180200,  //  000B  CALL	R6	1
      0x7C140200,  //  000C  CALL	R5	1
      0xA802000B,  //  000D  EXBLK	0	#001A
      0x5C180A00,  //  000E  MOVE	R6	R5
      0x7C180000,  //  000F  CALL	R6	0
      0x8C1C0701,  //  0010  GETMET	R7	R3	K1
      0x5C240C00,  //  0011  MOVE	R9	R6
      0x7C1C0400,  //  0012  CALL	R7	2
      0x1C1C0E04,  //  0013  EQ	R7	R7	R4
      0x741E0001,  //  0014  JMPT	R7	#0017
      0x1C1C0503,  //  0015  EQ	R7	R2	K3
      0x781E0001,  //  0016  JMPF	R7	#0019
      0xA8040001,  //  0017  EXBLK	1	1
      0x80040C00,  //  0018  RET	1	R6
      0x7001FFF3,  //  0019  JMP		#000E
      0x58140004,  //  001A  LDCONST	R5	K4
      0xAC140200,  //  001B  CATCH	R5	1	0
      0xB0080000,  //  001C  RAISE	2	R0	R0
      0x80000000,  //  001D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: wire_scan
********************************************************************/
be_local_closure(Tasmota_wire_scan,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str(i2c_enabled),
    /* K1   */  be_nested_str(wire1),
    /* K2   */  be_nested_str(enabled),
    /* K3   */  be_nested_str(detect),
    /* K4   */  be_nested_str(wire2),
    }),
    &be_const_str_wire_scan,
    &be_const_str_solidified,
    ( &(const binstruction[33]) {  /* code */
      0x4C0C0000,  //  0000  LDNIL	R3
      0x200C0403,  //  0001  NE	R3	R2	R3
      0x780E0005,  //  0002  JMPF	R3	#0009
      0x8C0C0100,  //  0003  GETMET	R3	R0	K0
      0x5C140400,  //  0004  MOVE	R5	R2
      0x7C0C0400,  //  0005  CALL	R3	2
      0x740E0001,  //  0006  JMPT	R3	#0009
      0x4C0C0000,  //  0007  LDNIL	R3
      0x80040600,  //  0008  RET	1	R3
      0x880C0101,  //  0009  GETMBR	R3	R0	K1
      0x8C0C0702,  //  000A  GETMET	R3	R3	K2
      0x7C0C0200,  //  000B  CALL	R3	1
      0x780E0006,  //  000C  JMPF	R3	#0014
      0x880C0101,  //  000D  GETMBR	R3	R0	K1
      0x8C0C0703,  //  000E  GETMET	R3	R3	K3
      0x5C140200,  //  000F  MOVE	R5	R1
      0x7C0C0400,  //  0010  CALL	R3	2
      0x780E0001,  //  0011  JMPF	R3	#0014
      0x880C0101,  //  0012  GETMBR	R3	R0	K1
      0x80040600,  //  0013  RET	1	R3
      0x880C0104,  //  0014  GETMBR	R3	R0	K4
      0x8C0C0702,  //  0015  GETMET	R3	R3	K2
      0x7C0C0200,  //  0016  CALL	R3	1
      0x780E0006,  //  0017  JMPF	R3	#001F
      0x880C0104,  //  0018  GETMBR	R3	R0	K4
      0x8C0C0703,  //  0019  GETMET	R3	R3	K3
      0x5C140200,  //  001A  MOVE	R5	R1
      0x7C0C0400,  //  001B  CALL	R3	2
      0x780E0001,  //  001C  JMPF	R3	#001F
      0x880C0104,  //  001D  GETMBR	R3	R0	K4
      0x80040600,  //  001E  RET	1	R3
      0x4C0C0000,  //  001F  LDNIL	R3
      0x80040600,  //  0020  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Tasmota_init,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[15]) {     /* constants */
    /* K0   */  be_nested_str(global),
    /* K1   */  be_nested_str(ctypes_bytes_dyn),
    /* K2   */  be_nested_str(_global_addr),
    /* K3   */  be_nested_str(_global_def),
    /* K4   */  be_nested_str(introspect),
    /* K5   */  be_nested_str(_settings_ptr),
    /* K6   */  be_nested_str(get),
    /* K7   */  be_const_int(0),
    /* K8   */  be_nested_str(settings),
    /* K9   */  be_nested_str(toptr),
    /* K10  */  be_nested_str(_settings_def),
    /* K11  */  be_nested_str(wd),
    /* K12  */  be_nested_str(),
    /* K13  */  be_nested_str(_debug_present),
    /* K14  */  be_nested_str(debug),
    }),
    &be_const_str_init,
    &be_const_str_solidified,
    ( &(const binstruction[36]) {  /* code */
      0xB8060200,  //  0000  GETNGBL	R1	K1
      0x88080102,  //  0001  GETMBR	R2	R0	K2
      0x880C0103,  //  0002  GETMBR	R3	R0	K3
      0x7C040400,  //  0003  CALL	R1	2
      0x90020001,  //  0004  SETMBR	R0	K0	R1
      0xA4060800,  //  0005  IMPORT	R1	K4
      0x60080015,  //  0006  GETGBL	R2	G21
      0x880C0105,  //  0007  GETMBR	R3	R0	K5
      0x54120003,  //  0008  LDINT	R4	4
      0x7C080400,  //  0009  CALL	R2	2
      0x8C080506,  //  000A  GETMET	R2	R2	K6
      0x58100007,  //  000B  LDCONST	R4	K7
      0x54160003,  //  000C  LDINT	R5	4
      0x7C080600,  //  000D  CALL	R2	3
      0x780A0006,  //  000E  JMPF	R2	#0016
      0xB80E0200,  //  000F  GETNGBL	R3	K1
      0x8C100309,  //  0010  GETMET	R4	R1	K9
      0x5C180400,  //  0011  MOVE	R6	R2
      0x7C100400,  //  0012  CALL	R4	2
      0x8814010A,  //  0013  GETMBR	R5	R0	K10
      0x7C0C0400,  //  0014  CALL	R3	2
      0x90021003,  //  0015  SETMBR	R0	K8	R3
      0x9002170C,  //  0016  SETMBR	R0	K11	K12
      0x500C0000,  //  0017  LDBOOL	R3	0	0
      0x90021A03,  //  0018  SETMBR	R0	K13	R3
      0xA8020004,  //  0019  EXBLK	0	#001F
      0xA40E1C00,  //  001A  IMPORT	R3	K14
      0x50100200,  //  001B  LDBOOL	R4	1	0
      0x90021A04,  //  001C  SETMBR	R0	K13	R4
      0xA8040001,  //  001D  EXBLK	1	1
      0x70020003,  //  001E  JMP		#0023
      0xAC0C0000,  //  001F  CATCH	R3	0	0
      0x70020000,  //  0020  JMP		#0022
      0x70020000,  //  0021  JMP		#0023
      0xB0080000,  //  0022  RAISE	2	R0	R0
      0x80000000,  //  0023  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: time_str
********************************************************************/
be_local_closure(Tasmota_time_str,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[10]) {     /* constants */
    /* K0   */  be_nested_str(string),
    /* K1   */  be_nested_str(time_dump),
    /* K2   */  be_nested_str(format),
    /* K3   */  be_nested_str(_X2504d_X2D_X2502d_X2D_X2502dT_X2502d_X3A_X2502d_X3A_X2502d),
    /* K4   */  be_nested_str(year),
    /* K5   */  be_nested_str(month),
    /* K6   */  be_nested_str(day),
    /* K7   */  be_nested_str(hour),
    /* K8   */  be_nested_str(min),
    /* K9   */  be_nested_str(sec),
    }),
    &be_const_str_time_str,
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x8C0C0101,  //  0001  GETMET	R3	R0	K1
      0x5C140200,  //  0002  MOVE	R5	R1
      0x7C0C0400,  //  0003  CALL	R3	2
      0x8C100502,  //  0004  GETMET	R4	R2	K2
      0x58180003,  //  0005  LDCONST	R6	K3
      0x941C0704,  //  0006  GETIDX	R7	R3	K4
      0x94200705,  //  0007  GETIDX	R8	R3	K5
      0x94240706,  //  0008  GETIDX	R9	R3	K6
      0x94280707,  //  0009  GETIDX	R10	R3	K7
      0x942C0708,  //  000A  GETIDX	R11	R3	K8
      0x94300709,  //  000B  GETIDX	R12	R3	K9
      0x7C101000,  //  000C  CALL	R4	8
      0x80040800,  //  000D  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: remove_rule
********************************************************************/
be_local_closure(Tasmota_remove_rule,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str(_rules),
    /* K1   */  be_const_int(0),
    /* K2   */  be_nested_str(k),
    /* K3   */  be_nested_str(remove),
    /* K4   */  be_const_int(1),
    }),
    &be_const_str_remove_rule,
    &be_const_str_solidified,
    ( &(const binstruction[21]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x780A0011,  //  0001  JMPF	R2	#0014
      0x58080001,  //  0002  LDCONST	R2	K1
      0x600C000C,  //  0003  GETGBL	R3	G12
      0x88100100,  //  0004  GETMBR	R4	R0	K0
      0x7C0C0200,  //  0005  CALL	R3	1
      0x140C0403,  //  0006  LT	R3	R2	R3
      0x780E000B,  //  0007  JMPF	R3	#0014
      0x880C0100,  //  0008  GETMBR	R3	R0	K0
      0x940C0602,  //  0009  GETIDX	R3	R3	R2
      0x880C0702,  //  000A  GETMBR	R3	R3	K2
      0x1C0C0601,  //  000B  EQ	R3	R3	R1
      0x780E0004,  //  000C  JMPF	R3	#0012
      0x880C0100,  //  000D  GETMBR	R3	R0	K0
      0x8C0C0703,  //  000E  GETMET	R3	R3	K3
      0x5C140400,  //  000F  MOVE	R5	R2
      0x7C0C0400,  //  0010  CALL	R3	2
      0x70020000,  //  0011  JMP		#0013
      0x00080504,  //  0012  ADD	R2	R2	K4
      0x7001FFEE,  //  0013  JMP		#0003
      0x80000000,  //  0014  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: load
********************************************************************/
be_local_closure(Tasmota_load,   /* name */
  be_nested_proto(
    21,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 2]) {
      be_nested_proto(
        6,                          /* nstack */
        1,                          /* argc */
        2,                          /* varg */
        0,                          /* has upvals */
        NULL,                       /* no upvals */
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 4]) {     /* constants */
        /* K0   */  be_nested_str(sys),
        /* K1   */  be_nested_str(path),
        /* K2   */  be_nested_str(find),
        /* K3   */  be_nested_str(push),
        }),
        &be_const_str_push_path,
        &be_const_str_solidified,
        ( &(const binstruction[13]) {  /* code */
          0xA4060000,  //  0000  IMPORT	R1	K0
          0x8C080301,  //  0001  GETMET	R2	R1	K1
          0x7C080200,  //  0002  CALL	R2	1
          0x8C0C0502,  //  0003  GETMET	R3	R2	K2
          0x5C140000,  //  0004  MOVE	R5	R0
          0x7C0C0400,  //  0005  CALL	R3	2
          0x4C100000,  //  0006  LDNIL	R4
          0x1C0C0604,  //  0007  EQ	R3	R3	R4
          0x780E0002,  //  0008  JMPF	R3	#000C
          0x8C0C0503,  //  0009  GETMET	R3	R2	K3
          0x5C140000,  //  000A  MOVE	R5	R0
          0x7C0C0400,  //  000B  CALL	R3	2
          0x80000000,  //  000C  RET	0
        })
      ),
      be_nested_proto(
        7,                          /* nstack */
        1,                          /* argc */
        2,                          /* varg */
        0,                          /* has upvals */
        NULL,                       /* no upvals */
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 4]) {     /* constants */
        /* K0   */  be_nested_str(sys),
        /* K1   */  be_nested_str(path),
        /* K2   */  be_nested_str(find),
        /* K3   */  be_nested_str(remove),
        }),
        &be_const_str_pop_path,
        &be_const_str_solidified,
        ( &(const binstruction[13]) {  /* code */
          0xA4060000,  //  0000  IMPORT	R1	K0
          0x8C080301,  //  0001  GETMET	R2	R1	K1
          0x7C080200,  //  0002  CALL	R2	1
          0x8C0C0502,  //  0003  GETMET	R3	R2	K2
          0x5C140000,  //  0004  MOVE	R5	R0
          0x7C0C0400,  //  0005  CALL	R3	2
          0x4C100000,  //  0006  LDNIL	R4
          0x20100604,  //  0007  NE	R4	R3	R4
          0x78120002,  //  0008  JMPF	R4	#000C
          0x8C100503,  //  0009  GETMET	R4	R2	K3
          0x5C180600,  //  000A  MOVE	R6	R3
          0x7C100400,  //  000B  CALL	R4	2
          0x80000000,  //  000C  RET	0
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[21]) {     /* constants */
    /* K0   */  be_nested_str(string),
    /* K1   */  be_nested_str(path),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str(_X2F),
    /* K4   */  be_nested_str(split),
    /* K5   */  be_nested_str(_X23),
    /* K6   */  be_const_int(1),
    /* K7   */  be_nested_str(find),
    /* K8   */  be_nested_str(_X2E),
    /* K9   */  be_nested_str(_X2Ebe),
    /* K10  */  be_nested_str(_X2Ebec),
    /* K11  */  be_nested_str(io_error),
    /* K12  */  be_nested_str(file_X20extension_X20is_X20not_X20_X27_X2Ebe_X27_X20or_X20_X27_X2Ebec_X27),
    /* K13  */  be_nested_str(last_modified),
    /* K14  */  be_nested_str(c),
    /* K15  */  be_nested_str(wd),
    /* K16  */  be_nested_str(),
    /* K17  */  be_nested_str(file),
    /* K18  */  be_nested_str(save),
    /* K19  */  be_nested_str(format),
    /* K20  */  be_nested_str(BRY_X3A_X20could_X20not_X20save_X20compiled_X20file_X20_X25s_X20_X28_X25s_X29),
    }),
    &be_const_str_load,
    &be_const_str_solidified,
    ( &(const binstruction[121]) {  /* code */
      0x84080000,  //  0000  CLOSURE	R2	P0
      0x840C0001,  //  0001  CLOSURE	R3	P1
      0xA4120000,  //  0002  IMPORT	R4	K0
      0xA4160200,  //  0003  IMPORT	R5	K1
      0x6018000C,  //  0004  GETGBL	R6	G12
      0x5C1C0200,  //  0005  MOVE	R7	R1
      0x7C180200,  //  0006  CALL	R6	1
      0x1C180D02,  //  0007  EQ	R6	R6	K2
      0x781A0001,  //  0008  JMPF	R6	#000B
      0x50180000,  //  0009  LDBOOL	R6	0	0
      0x80040C00,  //  000A  RET	1	R6
      0x94180302,  //  000B  GETIDX	R6	R1	K2
      0x20180D03,  //  000C  NE	R6	R6	K3
      0x781A0000,  //  000D  JMPF	R6	#000F
      0x00060601,  //  000E  ADD	R1	K3	R1
      0x8C180904,  //  000F  GETMET	R6	R4	K4
      0x5C200200,  //  0010  MOVE	R8	R1
      0x58240005,  //  0011  LDCONST	R9	K5
      0x7C180600,  //  0012  CALL	R6	3
      0x941C0D02,  //  0013  GETIDX	R7	R6	K2
      0x5421FFFE,  //  0014  LDINT	R8	-1
      0x94200C08,  //  0015  GETIDX	R8	R6	R8
      0x6024000C,  //  0016  GETGBL	R9	G12
      0x5C280C00,  //  0017  MOVE	R10	R6
      0x7C240200,  //  0018  CALL	R9	1
      0x24241306,  //  0019  GT	R9	R9	K6
      0x8C280907,  //  001A  GETMET	R10	R4	K7
      0x5C301000,  //  001B  MOVE	R12	R8
      0x58340008,  //  001C  LDCONST	R13	K8
      0x7C280600,  //  001D  CALL	R10	3
      0x14281502,  //  001E  LT	R10	R10	K2
      0x782A0001,  //  001F  JMPF	R10	#0022
      0x00040309,  //  0020  ADD	R1	R1	K9
      0x00201109,  //  0021  ADD	R8	R8	K9
      0x5429FFFC,  //  0022  LDINT	R10	-3
      0x542DFFFE,  //  0023  LDINT	R11	-1
      0x4028140B,  //  0024  CONNECT	R10	R10	R11
      0x9428100A,  //  0025  GETIDX	R10	R8	R10
      0x1C281509,  //  0026  EQ	R10	R10	K9
      0x542DFFFB,  //  0027  LDINT	R11	-4
      0x5431FFFE,  //  0028  LDINT	R12	-1
      0x402C160C,  //  0029  CONNECT	R11	R11	R12
      0x942C100B,  //  002A  GETIDX	R11	R8	R11
      0x1C2C170A,  //  002B  EQ	R11	R11	K10
      0x5C301400,  //  002C  MOVE	R12	R10
      0x74320002,  //  002D  JMPT	R12	#0031
      0x5C301600,  //  002E  MOVE	R12	R11
      0x74320000,  //  002F  JMPT	R12	#0031
      0xB006170C,  //  0030  RAISE	1	K11	K12
      0x8C300B0D,  //  0031  GETMET	R12	R5	K13
      0x5C380E00,  //  0032  MOVE	R14	R7
      0x7C300400,  //  0033  CALL	R12	2
      0x782E0005,  //  0034  JMPF	R11	#003B
      0x4C340000,  //  0035  LDNIL	R13
      0x1C34180D,  //  0036  EQ	R13	R12	R13
      0x78360001,  //  0037  JMPF	R13	#003A
      0x50340000,  //  0038  LDBOOL	R13	0	0
      0x80041A00,  //  0039  RET	1	R13
      0x70020013,  //  003A  JMP		#004F
      0x8C340B0D,  //  003B  GETMET	R13	R5	K13
      0x003C030E,  //  003C  ADD	R15	R1	K14
      0x7C340400,  //  003D  CALL	R13	2
      0x4C380000,  //  003E  LDNIL	R14
      0x1C38180E,  //  003F  EQ	R14	R12	R14
      0x783A0004,  //  0040  JMPF	R14	#0046
      0x4C380000,  //  0041  LDNIL	R14
      0x1C381A0E,  //  0042  EQ	R14	R13	R14
      0x783A0001,  //  0043  JMPF	R14	#0046
      0x50380000,  //  0044  LDBOOL	R14	0	0
      0x80041C00,  //  0045  RET	1	R14
      0x4C380000,  //  0046  LDNIL	R14
      0x20381A0E,  //  0047  NE	R14	R13	R14
      0x783A0005,  //  0048  JMPF	R14	#004F
      0x4C380000,  //  0049  LDNIL	R14
      0x1C38180E,  //  004A  EQ	R14	R12	R14
      0x743A0001,  //  004B  JMPT	R14	#004E
      0x28381A0C,  //  004C  GE	R14	R13	R12
      0x783A0000,  //  004D  JMPF	R14	#004F
      0x502C0200,  //  004E  LDBOOL	R11	1	0
      0x78260005,  //  004F  JMPF	R9	#0056
      0x00340F05,  //  0050  ADD	R13	R7	K5
      0x90021E0D,  //  0051  SETMBR	R0	K15	R13
      0x5C340400,  //  0052  MOVE	R13	R2
      0x8838010F,  //  0053  GETMBR	R14	R0	K15
      0x7C340200,  //  0054  CALL	R13	1
      0x70020000,  //  0055  JMP		#0057
      0x90021F10,  //  0056  SETMBR	R0	K15	K16
      0x6034000D,  //  0057  GETGBL	R13	G13
      0x5C380200,  //  0058  MOVE	R14	R1
      0x583C0011,  //  0059  LDCONST	R15	K17
      0x7C340400,  //  005A  CALL	R13	2
      0x5C381600,  //  005B  MOVE	R14	R11
      0x743A0013,  //  005C  JMPT	R14	#0071
      0x5C381200,  //  005D  MOVE	R14	R9
      0x743A0011,  //  005E  JMPT	R14	#0071
      0xA8020005,  //  005F  EXBLK	0	#0066
      0x8C380112,  //  0060  GETMET	R14	R0	K18
      0x0040030E,  //  0061  ADD	R16	R1	K14
      0x5C441A00,  //  0062  MOVE	R17	R13
      0x7C380600,  //  0063  CALL	R14	3
      0xA8040001,  //  0064  EXBLK	1	1
      0x7002000A,  //  0065  JMP		#0071
      0xAC380001,  //  0066  CATCH	R14	0	1
      0x70020007,  //  0067  JMP		#0070
      0x603C0001,  //  0068  GETGBL	R15	G1
      0x8C400913,  //  0069  GETMET	R16	R4	K19
      0x58480014,  //  006A  LDCONST	R18	K20
      0x004C030E,  //  006B  ADD	R19	R1	K14
      0x5C501C00,  //  006C  MOVE	R20	R14
      0x7C400800,  //  006D  CALL	R16	4
      0x7C3C0200,  //  006E  CALL	R15	1
      0x70020000,  //  006F  JMP		#0071
      0xB0080000,  //  0070  RAISE	2	R0	R0
      0x5C381A00,  //  0071  MOVE	R14	R13
      0x7C380000,  //  0072  CALL	R14	0
      0x78260002,  //  0073  JMPF	R9	#0077
      0x5C380600,  //  0074  MOVE	R14	R3
      0x003C0F05,  //  0075  ADD	R15	R7	K5
      0x7C380200,  //  0076  CALL	R14	1
      0x50380200,  //  0077  LDBOOL	R14	1	0
      0x80041C00,  //  0078  RET	1	R14
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: chars_in_string
********************************************************************/
be_local_closure(Tasmota_chars_in_string,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_const_int(1),
    }),
    &be_const_str_chars_in_string,
    &be_const_str_solidified,
    ( &(const binstruction[31]) {  /* code */
      0x780E0001,  //  0000  JMPF	R3	#0003
      0x50100200,  //  0001  LDBOOL	R4	1	0
      0x70020000,  //  0002  JMP		#0004
      0x50100000,  //  0003  LDBOOL	R4	0	0
      0x58140000,  //  0004  LDCONST	R5	K0
      0x6018000C,  //  0005  GETGBL	R6	G12
      0x5C1C0200,  //  0006  MOVE	R7	R1
      0x7C180200,  //  0007  CALL	R6	1
      0x14180A06,  //  0008  LT	R6	R5	R6
      0x781A0012,  //  0009  JMPF	R6	#001D
      0x50180000,  //  000A  LDBOOL	R6	0	0
      0x581C0000,  //  000B  LDCONST	R7	K0
      0x6020000C,  //  000C  GETGBL	R8	G12
      0x5C240400,  //  000D  MOVE	R9	R2
      0x7C200200,  //  000E  CALL	R8	1
      0x14200E08,  //  000F  LT	R8	R7	R8
      0x78220006,  //  0010  JMPF	R8	#0018
      0x94200205,  //  0011  GETIDX	R8	R1	R5
      0x94240407,  //  0012  GETIDX	R9	R2	R7
      0x1C201009,  //  0013  EQ	R8	R8	R9
      0x78220000,  //  0014  JMPF	R8	#0016
      0x50180200,  //  0015  LDBOOL	R6	1	0
      0x001C0F01,  //  0016  ADD	R7	R7	K1
      0x7001FFF3,  //  0017  JMP		#000C
      0x20200806,  //  0018  NE	R8	R4	R6
      0x78220000,  //  0019  JMPF	R8	#001B
      0x80040A00,  //  001A  RET	1	R5
      0x00140B01,  //  001B  ADD	R5	R5	K1
      0x7001FFE7,  //  001C  JMP		#0005
      0x5419FFFE,  //  001D  LDINT	R6	-1
      0x80040C00,  //  001E  RET	1	R6
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: cmd
********************************************************************/
be_local_closure(Tasmota_cmd,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str(cmd_res),
    /* K1   */  be_nested_str(_cmd),
    }),
    &be_const_str_cmd,
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x50080200,  //  0000  LDBOOL	R2	1	0
      0x90020002,  //  0001  SETMBR	R0	K0	R2
      0x8C080101,  //  0002  GETMET	R2	R0	K1
      0x5C100200,  //  0003  MOVE	R4	R1
      0x7C080400,  //  0004  CALL	R2	2
      0x4C080000,  //  0005  LDNIL	R2
      0x880C0100,  //  0006  GETMBR	R3	R0	K0
      0x50100200,  //  0007  LDBOOL	R4	1	0
      0x200C0604,  //  0008  NE	R3	R3	R4
      0x780E0000,  //  0009  JMPF	R3	#000B
      0x88080100,  //  000A  GETMBR	R2	R0	K0
      0x4C0C0000,  //  000B  LDNIL	R3
      0x90020003,  //  000C  SETMBR	R0	K0	R3
      0x80040400,  //  000D  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: add_cmd
********************************************************************/
be_local_closure(Tasmota_add_cmd,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str(check_not_method),
    /* K1   */  be_nested_str(_ccmd),
    /* K2   */  be_nested_str(function),
    /* K3   */  be_nested_str(value_error),
    /* K4   */  be_nested_str(the_X20second_X20argument_X20is_X20not_X20a_X20function),
    }),
    &be_const_str_add_cmd,
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x8C0C0100,  //  0000  GETMET	R3	R0	K0
      0x5C140400,  //  0001  MOVE	R5	R2
      0x7C0C0400,  //  0002  CALL	R3	2
      0x880C0101,  //  0003  GETMBR	R3	R0	K1
      0x740E0002,  //  0004  JMPT	R3	#0008
      0x600C0013,  //  0005  GETGBL	R3	G19
      0x7C0C0000,  //  0006  CALL	R3	0
      0x90020203,  //  0007  SETMBR	R0	K1	R3
      0x600C0004,  //  0008  GETGBL	R3	G4
      0x5C100400,  //  0009  MOVE	R4	R2
      0x7C0C0200,  //  000A  CALL	R3	1
      0x1C0C0702,  //  000B  EQ	R3	R3	K2
      0x780E0002,  //  000C  JMPF	R3	#0010
      0x880C0101,  //  000D  GETMBR	R3	R0	K1
      0x980C0202,  //  000E  SETIDX	R3	R1	R2
      0x70020000,  //  000F  JMP		#0011
      0xB0060704,  //  0010  RAISE	1	K3	K4
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: add_rule
********************************************************************/
be_local_closure(Tasmota_add_rule,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str(check_not_method),
    /* K1   */  be_nested_str(_rules),
    /* K2   */  be_nested_str(function),
    /* K3   */  be_nested_str(push),
    /* K4   */  be_nested_str(kv),
    /* K5   */  be_nested_str(value_error),
    /* K6   */  be_nested_str(the_X20second_X20argument_X20is_X20not_X20a_X20function),
    }),
    &be_const_str_add_rule,
    &be_const_str_solidified,
    ( &(const binstruction[23]) {  /* code */
      0x8C0C0100,  //  0000  GETMET	R3	R0	K0
      0x5C140400,  //  0001  MOVE	R5	R2
      0x7C0C0400,  //  0002  CALL	R3	2
      0x880C0101,  //  0003  GETMBR	R3	R0	K1
      0x740E0002,  //  0004  JMPT	R3	#0008
      0x600C0012,  //  0005  GETGBL	R3	G18
      0x7C0C0000,  //  0006  CALL	R3	0
      0x90020203,  //  0007  SETMBR	R0	K1	R3
      0x600C0004,  //  0008  GETGBL	R3	G4
      0x5C100400,  //  0009  MOVE	R4	R2
      0x7C0C0200,  //  000A  CALL	R3	1
      0x1C0C0702,  //  000B  EQ	R3	R3	K2
      0x780E0007,  //  000C  JMPF	R3	#0015
      0x880C0101,  //  000D  GETMBR	R3	R0	K1
      0x8C0C0703,  //  000E  GETMET	R3	R3	K3
      0x8C140104,  //  000F  GETMET	R5	R0	K4
      0x5C1C0200,  //  0010  MOVE	R7	R1
      0x5C200400,  //  0011  MOVE	R8	R2
      0x7C140600,  //  0012  CALL	R5	3
      0x7C0C0400,  //  0013  CALL	R3	2
      0x70020000,  //  0014  JMP		#0016
      0xB0060B06,  //  0015  RAISE	1	K5	K6
      0x80000000,  //  0016  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: exec_rules
********************************************************************/
be_local_closure(Tasmota_exec_rules,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[12]) {     /* constants */
    /* K0   */  be_nested_str(_rules),
    /* K1   */  be_nested_str(cmd_res),
    /* K2   */  be_nested_str(json),
    /* K3   */  be_nested_str(load),
    /* K4   */  be_nested_str(log),
    /* K5   */  be_nested_str(BRY_X3A_X20ERROR_X2C_X20bad_X20json_X3A_X20),
    /* K6   */  be_const_int(3),
    /* K7   */  be_const_int(0),
    /* K8   */  be_nested_str(try_rule),
    /* K9   */  be_nested_str(k),
    /* K10  */  be_nested_str(v),
    /* K11  */  be_const_int(1),
    }),
    &be_const_str_exec_rules,
    &be_const_str_solidified,
    ( &(const binstruction[48]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x740A0003,  //  0001  JMPT	R2	#0006
      0x88080101,  //  0002  GETMBR	R2	R0	K1
      0x4C0C0000,  //  0003  LDNIL	R3
      0x20080403,  //  0004  NE	R2	R2	R3
      0x780A0027,  //  0005  JMPF	R2	#002E
      0xA40A0400,  //  0006  IMPORT	R2	K2
      0x8C0C0503,  //  0007  GETMET	R3	R2	K3
      0x5C140200,  //  0008  MOVE	R5	R1
      0x7C0C0400,  //  0009  CALL	R3	2
      0x50100000,  //  000A  LDBOOL	R4	0	0
      0x4C140000,  //  000B  LDNIL	R5
      0x1C140605,  //  000C  EQ	R5	R3	R5
      0x78160004,  //  000D  JMPF	R5	#0013
      0x8C140104,  //  000E  GETMET	R5	R0	K4
      0x001E0A01,  //  000F  ADD	R7	K5	R1
      0x58200006,  //  0010  LDCONST	R8	K6
      0x7C140600,  //  0011  CALL	R5	3
      0x5C0C0200,  //  0012  MOVE	R3	R1
      0x88140101,  //  0013  GETMBR	R5	R0	K1
      0x4C180000,  //  0014  LDNIL	R6
      0x20140A06,  //  0015  NE	R5	R5	R6
      0x78160000,  //  0016  JMPF	R5	#0018
      0x90020203,  //  0017  SETMBR	R0	K1	R3
      0x88140100,  //  0018  GETMBR	R5	R0	K0
      0x78160012,  //  0019  JMPF	R5	#002D
      0x58140007,  //  001A  LDCONST	R5	K7
      0x6018000C,  //  001B  GETGBL	R6	G12
      0x881C0100,  //  001C  GETMBR	R7	R0	K0
      0x7C180200,  //  001D  CALL	R6	1
      0x14180A06,  //  001E  LT	R6	R5	R6
      0x781A000C,  //  001F  JMPF	R6	#002D
      0x88180100,  //  0020  GETMBR	R6	R0	K0
      0x94180C05,  //  0021  GETIDX	R6	R6	R5
      0x8C1C0108,  //  0022  GETMET	R7	R0	K8
      0x5C240600,  //  0023  MOVE	R9	R3
      0x88280D09,  //  0024  GETMBR	R10	R6	K9
      0x882C0D0A,  //  0025  GETMBR	R11	R6	K10
      0x7C1C0800,  //  0026  CALL	R7	4
      0x741E0001,  //  0027  JMPT	R7	#002A
      0x74120000,  //  0028  JMPT	R4	#002A
      0x50100001,  //  0029  LDBOOL	R4	0	1
      0x50100200,  //  002A  LDBOOL	R4	1	0
      0x00140B0B,  //  002B  ADD	R5	R5	K11
      0x7001FFED,  //  002C  JMP		#001B
      0x80040800,  //  002D  RET	1	R4
      0x50080000,  //  002E  LDBOOL	R2	0	0
      0x80040400,  //  002F  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: hs2rgb
********************************************************************/
be_local_closure(Tasmota_hs2rgb,   /* name */
  be_nested_proto(
    17,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str(tasmota),
    /* K2   */  be_nested_str(scale_uint),
    /* K3   */  be_const_int(1),
    /* K4   */  be_const_int(2),
    /* K5   */  be_const_int(3),
    }),
    &be_const_str_hs2rgb,
    &be_const_str_solidified,
    ( &(const binstruction[68]) {  /* code */
      0x4C0C0000,  //  0000  LDNIL	R3
      0x1C0C0403,  //  0001  EQ	R3	R2	R3
      0x780E0000,  //  0002  JMPF	R3	#0004
      0x540A00FE,  //  0003  LDINT	R2	255
      0x540E00FE,  //  0004  LDINT	R3	255
      0x541200FE,  //  0005  LDINT	R4	255
      0x541600FE,  //  0006  LDINT	R5	255
      0x541A0167,  //  0007  LDINT	R6	360
      0x10040206,  //  0008  MOD	R1	R1	R6
      0x24180500,  //  0009  GT	R6	R2	K0
      0x781A0031,  //  000A  JMPF	R6	#003D
      0x541A003B,  //  000B  LDINT	R6	60
      0x0C180206,  //  000C  DIV	R6	R1	R6
      0x541E003B,  //  000D  LDINT	R7	60
      0x101C0207,  //  000E  MOD	R7	R1	R7
      0x542200FE,  //  000F  LDINT	R8	255
      0x04201002,  //  0010  SUB	R8	R8	R2
      0xB8260200,  //  0011  GETNGBL	R9	K1
      0x8C241302,  //  0012  GETMET	R9	R9	K2
      0x5C2C0E00,  //  0013  MOVE	R11	R7
      0x58300000,  //  0014  LDCONST	R12	K0
      0x5436003B,  //  0015  LDINT	R13	60
      0x543A00FE,  //  0016  LDINT	R14	255
      0x5C3C1000,  //  0017  MOVE	R15	R8
      0x7C240C00,  //  0018  CALL	R9	6
      0xB82A0200,  //  0019  GETNGBL	R10	K1
      0x8C281502,  //  001A  GETMET	R10	R10	K2
      0x5C300E00,  //  001B  MOVE	R12	R7
      0x58340000,  //  001C  LDCONST	R13	K0
      0x543A003B,  //  001D  LDINT	R14	60
      0x5C3C1000,  //  001E  MOVE	R15	R8
      0x544200FE,  //  001F  LDINT	R16	255
      0x7C280C00,  //  0020  CALL	R10	6
      0x1C2C0D00,  //  0021  EQ	R11	R6	K0
      0x782E0002,  //  0022  JMPF	R11	#0026
      0x5C141400,  //  0023  MOVE	R5	R10
      0x5C101000,  //  0024  MOVE	R4	R8
      0x70020016,  //  0025  JMP		#003D
      0x1C2C0D03,  //  0026  EQ	R11	R6	K3
      0x782E0002,  //  0027  JMPF	R11	#002B
      0x5C0C1200,  //  0028  MOVE	R3	R9
      0x5C101000,  //  0029  MOVE	R4	R8
      0x70020011,  //  002A  JMP		#003D
      0x1C2C0D04,  //  002B  EQ	R11	R6	K4
      0x782E0002,  //  002C  JMPF	R11	#0030
      0x5C0C1000,  //  002D  MOVE	R3	R8
      0x5C101400,  //  002E  MOVE	R4	R10
      0x7002000C,  //  002F  JMP		#003D
      0x1C2C0D05,  //  0030  EQ	R11	R6	K5
      0x782E0002,  //  0031  JMPF	R11	#0035
      0x5C0C1000,  //  0032  MOVE	R3	R8
      0x5C141200,  //  0033  MOVE	R5	R9
      0x70020007,  //  0034  JMP		#003D
      0x542E0003,  //  0035  LDINT	R11	4
      0x1C2C0C0B,  //  0036  EQ	R11	R6	R11
      0x782E0002,  //  0037  JMPF	R11	#003B
      0x5C0C1400,  //  0038  MOVE	R3	R10
      0x5C141000,  //  0039  MOVE	R5	R8
      0x70020001,  //  003A  JMP		#003D
      0x5C141000,  //  003B  MOVE	R5	R8
      0x5C101200,  //  003C  MOVE	R4	R9
      0x541A000F,  //  003D  LDINT	R6	16
      0x38180606,  //  003E  SHL	R6	R3	R6
      0x541E0007,  //  003F  LDINT	R7	8
      0x381C0A07,  //  0040  SHL	R7	R5	R7
      0x30180C07,  //  0041  OR	R6	R6	R7
      0x30180C04,  //  0042  OR	R6	R6	R4
      0x80040C00,  //  0043  RET	1	R6
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(KV_init,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str(k),
    /* K1   */  be_nested_str(v),
    }),
    &be_const_str_init,
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x90020202,  //  0001  SETMBR	R0	K1	R2
      0x80000000,  //  0002  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: KV
********************************************************************/
be_local_class(KV,
    2,
    NULL,
    be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(k, 2), be_const_var(0) },
        { be_const_key(v, -1), be_const_var(1) },
        { be_const_key(init, -1), be_const_closure(KV_init_closure) },
    })),
    be_str_literal("KV")
);

/********************************************************************
** Solidified function: kv
********************************************************************/
be_local_closure(Tasmota_kv,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_const_class(be_class_KV),
    }),
    &be_const_str_kv,
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x580C0000,  //  0000  LDCONST	R3	K0
      0xB4000000,  //  0001  CLASS	K0
      0x5C100600,  //  0002  MOVE	R4	R3
      0x5C140200,  //  0003  MOVE	R5	R1
      0x5C180400,  //  0004  MOVE	R6	R2
      0x7C100400,  //  0005  CALL	R4	2
      0x80040800,  //  0006  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: remove_cmd
********************************************************************/
be_local_closure(Tasmota_remove_cmd,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str(_ccmd),
    /* K1   */  be_nested_str(remove),
    }),
    &be_const_str_remove_cmd,
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x780A0003,  //  0001  JMPF	R2	#0006
      0x88080100,  //  0002  GETMBR	R2	R0	K0
      0x8C080501,  //  0003  GETMET	R2	R2	K1
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_timer
********************************************************************/
be_local_closure(Tasmota_set_timer,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str(check_not_method),
    /* K1   */  be_nested_str(_timers),
    /* K2   */  be_nested_str(push),
    /* K3   */  be_nested_str(Timer),
    /* K4   */  be_nested_str(millis),
    }),
    &be_const_str_set_timer,
    &be_const_str_solidified,
    ( &(const binstruction[19]) {  /* code */
      0x8C100100,  //  0000  GETMET	R4	R0	K0
      0x5C180400,  //  0001  MOVE	R6	R2
      0x7C100400,  //  0002  CALL	R4	2
      0x88100101,  //  0003  GETMBR	R4	R0	K1
      0x74120002,  //  0004  JMPT	R4	#0008
      0x60100012,  //  0005  GETGBL	R4	G18
      0x7C100000,  //  0006  CALL	R4	0
      0x90020204,  //  0007  SETMBR	R0	K1	R4
      0x88100101,  //  0008  GETMBR	R4	R0	K1
      0x8C100902,  //  0009  GETMET	R4	R4	K2
      0xB81A0600,  //  000A  GETNGBL	R6	K3
      0x8C1C0104,  //  000B  GETMET	R7	R0	K4
      0x5C240200,  //  000C  MOVE	R9	R1
      0x7C1C0400,  //  000D  CALL	R7	2
      0x5C200400,  //  000E  MOVE	R8	R2
      0x5C240600,  //  000F  MOVE	R9	R3
      0x7C180600,  //  0010  CALL	R6	3
      0x7C100400,  //  0011  CALL	R4	2
      0x80000000,  //  0012  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: remove_timer
********************************************************************/
be_local_closure(Tasmota_remove_timer,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str(tasmota),
    /* K1   */  be_nested_str(_timers),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str(size),
    /* K4   */  be_nested_str(id),
    /* K5   */  be_nested_str(remove),
    /* K6   */  be_const_int(1),
    }),
    &be_const_str_remove_timer,
    &be_const_str_solidified,
    ( &(const binstruction[23]) {  /* code */
      0xB80A0000,  //  0000  GETNGBL	R2	K0
      0x88080501,  //  0001  GETMBR	R2	R2	K1
      0x780A0012,  //  0002  JMPF	R2	#0016
      0x58080002,  //  0003  LDCONST	R2	K2
      0xB80E0000,  //  0004  GETNGBL	R3	K0
      0x880C0701,  //  0005  GETMBR	R3	R3	K1
      0x8C0C0703,  //  0006  GETMET	R3	R3	K3
      0x7C0C0200,  //  0007  CALL	R3	1
      0x140C0403,  //  0008  LT	R3	R2	R3
      0x780E000B,  //  0009  JMPF	R3	#0016
      0x880C0101,  //  000A  GETMBR	R3	R0	K1
      0x940C0602,  //  000B  GETIDX	R3	R3	R2
      0x880C0704,  //  000C  GETMBR	R3	R3	K4
      0x1C0C0601,  //  000D  EQ	R3	R3	R1
      0x780E0004,  //  000E  JMPF	R3	#0014
      0x880C0101,  //  000F  GETMBR	R3	R0	K1
      0x8C0C0705,  //  0010  GETMET	R3	R3	K5
      0x5C140400,  //  0011  MOVE	R5	R2
      0x7C0C0400,  //  0012  CALL	R3	2
      0x70020000,  //  0013  JMP		#0015
      0x00080506,  //  0014  ADD	R2	R2	K6
      0x7001FFED,  //  0015  JMP		#0004
      0x80000000,  //  0016  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: exec_cmd
********************************************************************/
be_local_closure(Tasmota_exec_cmd,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str(_ccmd),
    /* K1   */  be_nested_str(json),
    /* K2   */  be_nested_str(load),
    /* K3   */  be_nested_str(find_key_i),
    /* K4   */  be_nested_str(resolvecmnd),
    }),
    &be_const_str_exec_cmd,
    &be_const_str_solidified,
    ( &(const binstruction[27]) {  /* code */
      0x88100100,  //  0000  GETMBR	R4	R0	K0
      0x78120016,  //  0001  JMPF	R4	#0019
      0xA4120200,  //  0002  IMPORT	R4	K1
      0x8C140902,  //  0003  GETMET	R5	R4	K2
      0x5C1C0600,  //  0004  MOVE	R7	R3
      0x7C140400,  //  0005  CALL	R5	2
      0x8C180103,  //  0006  GETMET	R6	R0	K3
      0x88200100,  //  0007  GETMBR	R8	R0	K0
      0x5C240200,  //  0008  MOVE	R9	R1
      0x7C180600,  //  0009  CALL	R6	3
      0x4C1C0000,  //  000A  LDNIL	R7
      0x201C0C07,  //  000B  NE	R7	R6	R7
      0x781E000B,  //  000C  JMPF	R7	#0019
      0x8C1C0104,  //  000D  GETMET	R7	R0	K4
      0x5C240C00,  //  000E  MOVE	R9	R6
      0x7C1C0400,  //  000F  CALL	R7	2
      0x881C0100,  //  0010  GETMBR	R7	R0	K0
      0x941C0E06,  //  0011  GETIDX	R7	R7	R6
      0x5C200C00,  //  0012  MOVE	R8	R6
      0x5C240400,  //  0013  MOVE	R9	R2
      0x5C280600,  //  0014  MOVE	R10	R3
      0x5C2C0A00,  //  0015  MOVE	R11	R5
      0x7C1C0800,  //  0016  CALL	R7	4
      0x501C0200,  //  0017  LDBOOL	R7	1	0
      0x80040E00,  //  0018  RET	1	R7
      0x50100000,  //  0019  LDBOOL	R4	0	0
      0x80040800,  //  001A  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_light
********************************************************************/
be_local_closure(Tasmota_get_light,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str(tasmota_X2Eget_light_X28_X29_X20is_X20deprecated_X2C_X20use_X20light_X2Eget_X28_X29),
    /* K1   */  be_nested_str(light),
    /* K2   */  be_nested_str(get),
    }),
    &be_const_str_get_light,
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0x60080001,  //  0000  GETGBL	R2	G1
      0x580C0000,  //  0001  LDCONST	R3	K0
      0x7C080200,  //  0002  CALL	R2	1
      0xA40A0200,  //  0003  IMPORT	R2	K1
      0x4C0C0000,  //  0004  LDNIL	R3
      0x200C0203,  //  0005  NE	R3	R1	R3
      0x780E0004,  //  0006  JMPF	R3	#000C
      0x8C0C0502,  //  0007  GETMET	R3	R2	K2
      0x5C140200,  //  0008  MOVE	R5	R1
      0x7C0C0400,  //  0009  CALL	R3	2
      0x80040600,  //  000A  RET	1	R3
      0x70020002,  //  000B  JMP		#000F
      0x8C0C0502,  //  000C  GETMET	R3	R2	K2
      0x7C0C0200,  //  000D  CALL	R3	1
      0x80040600,  //  000E  RET	1	R3
      0x80000000,  //  000F  RET	0
    })
  )
);
/*******************************************************************/

/********************************************************************
** Solidified function: check_not_method
********************************************************************/
be_local_closure(Tasmota_check_not_method,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str(introspect),
    /* K1   */  be_nested_str(function),
    /* K2   */  be_nested_str(type_error),
    /* K3   */  be_nested_str(BRY_X3A_X20argument_X20must_X20be_X20a_X20function),
    /* K4   */  be_nested_str(ismethod),
    /* K5   */  be_nested_str(BRY_X3A_X20method_X20not_X20allowed_X2C_X20use_X20a_X20closure_X20like_X20_X27_X2F_X20args_X20_X2D_X3E_X20obj_X2Efunc_X28args_X29_X27),
    }),
    &be_const_str_check_not_method,
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x600C0004,  //  0001  GETGBL	R3	G4
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C0C0200,  //  0003  CALL	R3	1
      0x200C0701,  //  0004  NE	R3	R3	K1
      0x780E0000,  //  0005  JMPF	R3	#0007
      0xB0060503,  //  0006  RAISE	1	K2	K3
      0x8C0C0504,  //  0007  GETMET	R3	R2	K4
      0x5C140200,  //  0008  MOVE	R5	R1
      0x7C0C0400,  //  0009  CALL	R3	2
      0x50100200,  //  000A  LDBOOL	R4	1	0
      0x1C0C0604,  //  000B  EQ	R3	R3	R4
      0x780E0000,  //  000C  JMPF	R3	#000E
      0xB0060505,  //  000D  RAISE	1	K2	K5
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/

#include "be_fixed_be_class_tasmota.h"


// Class definition
void be_load_tasmota_ntvlib(bvm *vm)
{
    be_pushntvclass(vm, &be_class_tasmota);
    be_setglobal(vm, "Tasmota");
    be_pop(vm, 1);
}

/* @const_object_info_begin
class be_class_tasmota (scope: global, name: Tasmota) {
    _fl, var
    _rules, var
    _timers, var
    _ccmd, var
    _drivers, var
    wire1, var
    wire2, var
    global, var
    settings, var
    cmd_res, var
    wd, var
    _debug_present, var

    _global_def, comptr(&be_tasmota_global_struct)
    _settings_def, comptr(&be_tasmota_settings_struct)
    _global_addr, comptr(&TasmotaGlobal)
    _settings_ptr, comptr(&Settings)

    init, closure(Tasmota_init_closure)
    kv, closure(Tasmota_kv_closure)

    get_free_heap, func(l_getFreeHeap)
    arch, func(l_arch)
    publish, func(l_publish)
    publish_result, func(l_publish_result)
    _cmd, func(l_cmd)
    get_option, func(l_getoption)
    millis, func(l_millis)
    time_reached, func(l_timereached)
    rtc, func(l_rtc)
    time_dump, func(l_time_dump)
    strftime, func(l_strftime)
    strptime, func(l_strptime)
    memory, func(l_memory)
    wifi, func(l_wifi)
    eth, func(l_eth)
    yield, func(l_yield)
    delay, func(l_delay)
    scale_uint, func(l_scaleuint)
    log, func(l_logInfo)
    save, func(l_save)

    read_sensors, func(l_read_sensors)

    resp_cmnd, func(l_respCmnd)
    resp_cmnd_str, func(l_respCmndStr)
    resp_cmnd_done, func(l_respCmndDone)
    resp_cmnd_error, func(l_respCmndError)
    resp_cmnd_failed, func(l_respCmndFailed)
    resolvecmnd, func(l_resolveCmnd)

    response_append, func(l_respAppend)
    web_send, func(l_webSend)
    web_send_decimal, func(l_webSendDecimal)

    get_power, func(l_getpower)
    set_power, func(l_setpower)
    get_switch, func(l_getswitch)

    i2c_enabled, func(l_i2cenabled)

    fast_loop, closure(Tasmota_fast_loop_closure)
    add_fast_loop, closure(Tasmota_add_fast_loop_closure)
    cmd, closure(Tasmota_cmd_closure)
    chars_in_string, closure(Tasmota_chars_in_string_closure)
    find_key_i, closure(Tasmota_find_key_i_closure)
    find_op, closure(Tasmota_find_op_closure)
    add_rule, closure(Tasmota_add_rule_closure)
    remove_rule, closure(Tasmota_remove_rule_closure)
    try_rule, closure(Tasmota_try_rule_closure)
    exec_rules, closure(Tasmota_exec_rules_closure)
    exec_tele, closure(Tasmota_exec_tele_closure)
    set_timer, closure(Tasmota_set_timer_closure)
    run_deferred, closure(Tasmota_run_deferred_closure)
    remove_timer, closure(Tasmota_remove_timer_closure)
    add_cmd, closure(Tasmota_add_cmd_closure)
    remove_cmd, closure(Tasmota_remove_cmd_closure)
    exec_cmd, closure(Tasmota_exec_cmd_closure)
    gc, closure(Tasmota_gc_closure)
    event, closure(Tasmota_event_closure)
    add_driver, closure(Tasmota_add_driver_closure)
    remove_driver, closure(Tasmota_remove_driver_closure)
    load, closure(Tasmota_load_closure)
    wire_scan, closure(Tasmota_wire_scan_closure)
    time_str, closure(Tasmota_time_str_closure)

    check_not_method, closure(Tasmota_check_not_method_closure)

    hs2rgb, closure(Tasmota_hs2rgb_closure)

    gen_cb, closure(Tasmota_gen_cb_closure)

    get_light, closure(Tasmota_get_light_closure)
    set_light, closure(Tasmota_set_light_closure)
}
@const_object_info_end */
