/********************************************************************
 * Tasmota lib
 *
 * To use: `import tasmota`
 *******************************************************************/
#include "be_constobj.h"

struct dummy_struct {};  // we need a struct name but don't need any meaningful content, we just take the address
extern struct TasmotaGlobal_t TasmotaGlobal;
extern struct dummy_struct be_tasmota_global_struct;

extern int l_getFreeHeap(bvm *vm);
extern int l_publish(bvm *vm);
extern int l_publish_result(bvm *vm);
extern int l_cmd(bvm *vm);
extern int l_get_cb(bvm *vm);
extern int l_getoption(bvm *vm);
extern int l_millis(bvm *vm);
extern int l_timereached(bvm *vm);
extern int l_rtc(bvm *vm);
extern int l_time_dump(bvm *vm);
extern int l_strftime(bvm *vm);
extern int l_memory(bvm *vm);
extern int l_wifi(bvm *vm);
extern int l_eth(bvm *vm);
extern int l_yield(bvm *vm);
extern int l_delay(bvm *vm);
extern int l_scaleuint(bvm *vm);
extern int l_logInfo(bvm *vm);
extern int l_save(bvm *vm);

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
** Solidified function: init
********************************************************************/
be_local_closure(init,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_string("global", 503252654, 6),
    /* K1   */  be_nested_string("ctypes_bytes_dyn", 915205307, 16),
    /* K2   */  be_nested_string("_global_addr", 533766721, 12),
    /* K3   */  be_nested_string("_global_def", 646007001, 11),
    }),
    (be_nested_const_str("init", 380752755, 4)),
    (be_nested_const_str("input", -103256197, 5)),
    ( &(const binstruction[ 6]) {  /* code */
      0xB8060200,  //  0000  GETNGBL	R1	K1
      0x88080102,  //  0001  GETMBR	R2	R0	K2
      0x880C0103,  //  0002  GETMBR	R3	R0	K3
      0x7C040400,  //  0003  CALL	R1	2
      0x90020001,  //  0004  SETMBR	R0	K0	R1
      0x80000000,  //  0005  RET	0
    })
  )
);
/*******************************************************************/

/********************************************************************
** Solidified function: add_driver
********************************************************************/
be_local_closure(add_driver,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_string("_drivers", -1034638311, 8),
    /* K1   */  be_nested_string("push", -2022703139, 4),
    }),
    (be_nested_const_str("add_driver", 1654458371, 10)),
    (be_nested_const_str("Tasmota.be", 825809411, 10)),
    ( &(const binstruction[12]) {  /* code */
      0x88080100,  //  0000  GETMBR R2  R0  K0
      0x780A0004,  //  0001  JMPF R2  #0007
      0x88080100,  //  0002  GETMBR R2  R0  K0
      0x8C080501,  //  0003  GETMET R2  R2  K1
      0x5C100200,  //  0004  MOVE R4  R1
      0x7C080400,  //  0005  CALL R2  2
      0x70020003,  //  0006  JMP    #000B
      0x60080012,  //  0007  GETGBL R2  G18
      0x7C080000,  //  0008  CALL R2  0
      0x400C0401,  //  0009  CONNECT  R3  R2  R1
      0x90020002,  //  000A  SETMBR R0  K0  R2
      0x80000000,  //  000B  RET  0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: gen_cb
********************************************************************/
be_local_closure(gen_cb,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_string("_cb", -251666929, 3),
    /* K1   */  be_const_int(0),
    /* K2   */  be_nested_string("find", -1108310694, 4),
    /* K3   */  be_nested_string("_get_cb", 1448849122, 7),
    /* K4   */  be_nested_string("stop_iteration", -121173395, 14),
    /* K5   */  be_nested_string("internal_error", -1775809127, 14),
    /* K6   */  be_nested_string("No callback available", 633786138, 21),
    }),
    (be_nested_const_str("gen_cb", -1049739745, 6)),
    (be_nested_const_str("Tasmota.be", 825809411, 10)),
    ( &(const binstruction[34]) {  /* code */
      0x88080100,  //  0000  GETMBR R2  R0  K0
      0x4C0C0000,  //  0001  LDNIL  R3
      0x1C080403,  //  0002  EQ R2  R2  R3
      0x780A0002,  //  0003  JMPF R2  #0007
      0x60080013,  //  0004  GETGBL R2  G19
      0x7C080000,  //  0005  CALL R2  0
      0x90020002,  //  0006  SETMBR R0  K0  R2
      0x60080010,  //  0007  GETGBL R2  G16
      0x540E0012,  //  0008  LDINT  R3  19
      0x400E0203,  //  0009  CONNECT  R3  K1  R3
      0x7C080200,  //  000A  CALL R2  1
      0xA8020010,  //  000B  EXBLK  0 #001D
      0x5C0C0400,  //  000C  MOVE R3  R2
      0x7C0C0000,  //  000D  CALL R3  0
      0x88100100,  //  000E  GETMBR R4  R0  K0
      0x8C100902,  //  000F  GETMET R4  R4  K2
      0x5C180600,  //  0010  MOVE R6  R3
      0x7C100400,  //  0011  CALL R4  2
      0x4C140000,  //  0012  LDNIL  R5
      0x1C100805,  //  0013  EQ R4  R4  R5
      0x78120006,  //  0014  JMPF R4  #001C
      0x88100100,  //  0015  GETMBR R4  R0  K0
      0x98100601,  //  0016  SETIDX R4  R3  R1
      0x8C100103,  //  0017  GETMET R4  R0  K3
      0x5C180600,  //  0018  MOVE R6  R3
      0x7C100400,  //  0019  CALL R4  2
      0xA8040001,  //  001A  EXBLK  1 1
      0x80040800,  //  001B  RET  1 R4
      0x7001FFEE,  //  001C  JMP    #000C
      0x58080004,  //  001D  LDCONST  R2  K4
      0xAC080200,  //  001E  CATCH  R2  1 0
      0xB0080000,  //  001F  RAISE  2 R0  R0
      0xB0060B06,  //  0020  RAISE  1 K5  K6
      0x80000000,  //  0021  RET  0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_light
********************************************************************/
be_local_closure(set_light,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_string("tasmota.set_light() is deprecated, use light.set()", 2124937871, 50),
    /* K1   */  be_nested_string("light", -493019601, 5),
    /* K2   */  be_nested_string("set", -970520829, 3),
    }),
    (be_nested_const_str("set_light", -1118891144, 9)),
    (be_nested_const_str("Tasmota.be", 825809411, 10)),
    ( &(const binstruction[18]) {  /* code */
      0x600C0001,  //  0000  GETGBL R3  G1
      0x58100000,  //  0001  LDCONST  R4  K0
      0x7C0C0200,  //  0002  CALL R3  1
      0xA40E0200,  //  0003  IMPORT R3  K1
      0x4C100000,  //  0004  LDNIL  R4
      0x20100404,  //  0005  NE R4  R2  R4
      0x78120005,  //  0006  JMPF R4  #000D
      0x8C100702,  //  0007  GETMET R4  R3  K2
      0x5C180200,  //  0008  MOVE R6  R1
      0x5C1C0400,  //  0009  MOVE R7  R2
      0x7C100600,  //  000A  CALL R4  3
      0x80040800,  //  000B  RET  1 R4
      0x70020003,  //  000C  JMP    #0011
      0x8C100702,  //  000D  GETMET R4  R3  K2
      0x5C180200,  //  000E  MOVE R6  R1
      0x7C100400,  //  000F  CALL R4  2
      0x80040800,  //  0010  RET  1 R4
      0x80000000,  //  0011  RET  0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: remove_rule
********************************************************************/
be_local_closure(remove_rule,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_string("_rules", -28750191, 6),
    /* K1   */  be_nested_string("remove", -611183107, 6),
    }),
    (be_nested_const_str("remove_rule", -838755968, 11)),
    (be_nested_const_str("Tasmota.be", 825809411, 10)),
    ( &(const binstruction[ 7]) {  /* code */
      0x88080100,  //  0000  GETMBR R2  R0  K0
      0x780A0003,  //  0001  JMPF R2  #0006
      0x88080100,  //  0002  GETMBR R2  R0  K0
      0x8C080501,  //  0003  GETMET R2  R2  K1
      0x5C100200,  //  0004  MOVE R4  R1
      0x7C080400,  //  0005  CALL R2  2
      0x80000000,  //  0006  RET  0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: add_cmd
********************************************************************/
be_local_closure(add_cmd,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    3,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_string("_ccmd", -2131545883, 5),
    /* K1   */  be_nested_string("function", -1630125495, 8),
    /* K2   */  be_nested_string("value_error", 773297791, 11),
    /* K3   */  be_nested_string("the second argument is not a function", -340392827, 37),
    }),
    (be_nested_const_str("add_cmd", -933336417, 7)),
    (be_nested_const_str("Tasmota.be", 825809411, 10)),
    ( &(const binstruction[15]) {  /* code */
      0x880C0100,  //  0000  GETMBR R3  R0  K0
      0x740E0002,  //  0001  JMPT R3  #0005
      0x600C0013,  //  0002  GETGBL R3  G19
      0x7C0C0000,  //  0003  CALL R3  0
      0x90020003,  //  0004  SETMBR R0  K0  R3
      0x600C0004,  //  0005  GETGBL R3  G4
      0x5C100400,  //  0006  MOVE R4  R2
      0x7C0C0200,  //  0007  CALL R3  1
      0x1C0C0701,  //  0008  EQ R3  R3  K1
      0x780E0002,  //  0009  JMPF R3  #000D
      0x880C0100,  //  000A  GETMBR R3  R0  K0
      0x980C0202,  //  000B  SETIDX R3  R1  R2
      0x70020000,  //  000C  JMP    #000E
      0xB0060503,  //  000D  RAISE  1 K2  K3
      0x80000000,  //  000E  RET  0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: wire_scan
********************************************************************/
be_local_closure(wire_scan,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    3,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_string("i2c_enabled", 218388101, 11),
    /* K1   */  be_nested_string("wire1", -1082245877, 5),
    /* K2   */  be_nested_string("detect", 8884370, 6),
    /* K3   */  be_nested_string("wire2", -1065468258, 5),
    }),
    (be_nested_const_str("wire_scan", -1623691416, 9)),
    (be_nested_const_str("Tasmota.be", 825809411, 10)),
    ( &(const binstruction[25]) {  /* code */
      0x4C0C0000,  //  0000  LDNIL  R3
      0x200C0403,  //  0001  NE R3  R2  R3
      0x780E0005,  //  0002  JMPF R3  #0009
      0x8C0C0100,  //  0003  GETMET R3  R0  K0
      0x5C140400,  //  0004  MOVE R5  R2
      0x7C0C0400,  //  0005  CALL R3  2
      0x740E0001,  //  0006  JMPT R3  #0009
      0x4C0C0000,  //  0007  LDNIL  R3
      0x80040600,  //  0008  RET  1 R3
      0x880C0101,  //  0009  GETMBR R3  R0  K1
      0x8C0C0702,  //  000A  GETMET R3  R3  K2
      0x5C140200,  //  000B  MOVE R5  R1
      0x7C0C0400,  //  000C  CALL R3  2
      0x780E0001,  //  000D  JMPF R3  #0010
      0x880C0101,  //  000E  GETMBR R3  R0  K1
      0x80040600,  //  000F  RET  1 R3
      0x880C0103,  //  0010  GETMBR R3  R0  K3
      0x8C0C0702,  //  0011  GETMET R3  R3  K2
      0x5C140200,  //  0012  MOVE R5  R1
      0x7C0C0400,  //  0013  CALL R3  2
      0x780E0001,  //  0014  JMPF R3  #0017
      0x880C0103,  //  0015  GETMBR R3  R0  K3
      0x80040600,  //  0016  RET  1 R3
      0x4C0C0000,  //  0017  LDNIL  R3
      0x80040600,  //  0018  RET  1 R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: find_key_i
********************************************************************/
be_local_closure(find_key_i,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    3,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_string("string", 398550328, 6),
    /* K1   */  be_nested_string("toupper", -602983720, 7),
    /* K2   */  be_nested_string("keys", -112588595, 4),
    /* K3   */  be_nested_string("?", 973910158, 1),
    /* K4   */  be_nested_string("stop_iteration", -121173395, 14),
    }),
    (be_nested_const_str("find_key_i", 850136726, 10)),
    (be_nested_const_str("input", -103256197, 5)),
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
** Solidified function: chars_in_string
********************************************************************/
be_local_closure(chars_in_string,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    4,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_const_int(1),
    }),
    (be_nested_const_str("chars_in_string", -1146182164, 15)),
    (be_nested_const_str("input", -103256197, 5)),
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
** Solidified function: set_timer
********************************************************************/
be_local_closure(set_timer,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    4,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_string("_timers", -1694866380, 7),
    /* K1   */  be_nested_string("push", -2022703139, 4),
    /* K2   */  be_nested_string("Timer", -346839614, 5),
    /* K3   */  be_nested_string("millis", 1214679063, 6),
    }),
    (be_nested_const_str("set_timer", 2135414533, 9)),
    (be_nested_const_str("Tasmota.be", 825809411, 10)),
    ( &(const binstruction[16]) {  /* code */
      0x88100100,  //  0000  GETMBR R4  R0  K0
      0x74120002,  //  0001  JMPT R4  #0005
      0x60100012,  //  0002  GETGBL R4  G18
      0x7C100000,  //  0003  CALL R4  0
      0x90020004,  //  0004  SETMBR R0  K0  R4
      0x88100100,  //  0005  GETMBR R4  R0  K0
      0x8C100901,  //  0006  GETMET R4  R4  K1
      0xB81A0400,  //  0007  GETNGBL  R6  K2
      0x8C1C0103,  //  0008  GETMET R7  R0  K3
      0x5C240200,  //  0009  MOVE R9  R1
      0x7C1C0400,  //  000A  CALL R7  2
      0x5C200400,  //  000B  MOVE R8  R2
      0x5C240600,  //  000C  MOVE R9  R3
      0x7C180600,  //  000D  CALL R6  3
      0x7C100400,  //  000E  CALL R4  2
      0x80000000,  //  000F  RET  0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: add_rule
********************************************************************/
be_local_closure(add_rule,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    3,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_string("_rules", -28750191, 6),
    /* K1   */  be_nested_string("function", -1630125495, 8),
    /* K2   */  be_nested_string("value_error", 773297791, 11),
    /* K3   */  be_nested_string("the second argument is not a function", -340392827, 37),
    }),
    (be_nested_const_str("add_rule", 596540743, 8)),
    (be_nested_const_str("Tasmota.be", 825809411, 10)),
    ( &(const binstruction[15]) {  /* code */
      0x880C0100,  //  0000  GETMBR R3  R0  K0
      0x740E0002,  //  0001  JMPT R3  #0005
      0x600C0013,  //  0002  GETGBL R3  G19
      0x7C0C0000,  //  0003  CALL R3  0
      0x90020003,  //  0004  SETMBR R0  K0  R3
      0x600C0004,  //  0005  GETGBL R3  G4
      0x5C100400,  //  0006  MOVE R4  R2
      0x7C0C0200,  //  0007  CALL R3  1
      0x1C0C0701,  //  0008  EQ R3  R3  K1
      0x780E0002,  //  0009  JMPF R3  #000D
      0x880C0100,  //  000A  GETMBR R3  R0  K0
      0x980C0202,  //  000B  SETIDX R3  R1  R2
      0x70020000,  //  000C  JMP    #000E
      0xB0060503,  //  000D  RAISE  1 K2  K3
      0x80000000,  //  000E  RET  0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: run_deferred
********************************************************************/
be_local_closure(run_deferred,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_string("_timers", -1694866380, 7),
    /* K1   */  be_const_int(0),
    /* K2   */  be_nested_string("size", 597743964, 4),
    /* K3   */  be_nested_string("time_reached", 2075136773, 12),
    /* K4   */  be_nested_string("due", -399437003, 3),
    /* K5   */  be_nested_string("f", -485742695, 1),
    /* K6   */  be_nested_string("remove", -611183107, 6),
    /* K7   */  be_const_int(1),
    }),
    (be_nested_const_str("run_deferred", 371594696, 12)),
    (be_nested_const_str("Tasmota.be", 825809411, 10)),
    ( &(const binstruction[27]) {  /* code */
      0x88040100,  //  0000  GETMBR R1  R0  K0
      0x78060017,  //  0001  JMPF R1  #001A
      0x58040001,  //  0002  LDCONST  R1  K1
      0x88080100,  //  0003  GETMBR R2  R0  K0
      0x8C080502,  //  0004  GETMET R2  R2  K2
      0x7C080200,  //  0005  CALL R2  1
      0x14080202,  //  0006  LT R2  R1  R2
      0x780A0011,  //  0007  JMPF R2  #001A
      0x8C080103,  //  0008  GETMET R2  R0  K3
      0x88100100,  //  0009  GETMBR R4  R0  K0
      0x94100801,  //  000A  GETIDX R4  R4  R1
      0x88100904,  //  000B  GETMBR R4  R4  K4
      0x7C080400,  //  000C  CALL R2  2
      0x780A0009,  //  000D  JMPF R2  #0018
      0x88080100,  //  000E  GETMBR R2  R0  K0
      0x94080401,  //  000F  GETIDX R2  R2  R1
      0x88080505,  //  0010  GETMBR R2  R2  K5
      0x880C0100,  //  0011  GETMBR R3  R0  K0
      0x8C0C0706,  //  0012  GETMET R3  R3  K6
      0x5C140200,  //  0013  MOVE R5  R1
      0x7C0C0400,  //  0014  CALL R3  2
      0x5C0C0400,  //  0015  MOVE R3  R2
      0x7C0C0000,  //  0016  CALL R3  0
      0x70020000,  //  0017  JMP    #0019
      0x00040307,  //  0018  ADD  R1  R1  K7
      0x7001FFE8,  //  0019  JMP    #0003
      0x80000000,  //  001A  RET  0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: cmd
********************************************************************/
be_local_closure(cmd,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_string("cmd_res", 921166762, 7),
    /* K1   */  be_nested_string("_cmd", -875145154, 4),
    }),
    (be_nested_const_str("cmd", -158181397, 3)),
    (be_nested_const_str("tasmota.be", 1128870755, 10)),
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
** Solidified function: time_str
********************************************************************/
be_local_closure(time_str,   /* name */
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
    /* K1   */  be_nested_string("time_dump", -964556549, 9),
    /* K2   */  be_nested_string("format", -1180859054, 6),
    /* K3   */  be_nested_string("%04d-%02d-%02dT%02d:%02d:%02d", -869438695, 29),
    /* K4   */  be_nested_string("year", -1367388900, 4),
    /* K5   */  be_nested_string("month", -696646139, 5),
    /* K6   */  be_nested_string("day", -464576003, 3),
    /* K7   */  be_nested_string("hour", -1241306097, 4),
    /* K8   */  be_nested_string("min", -913357481, 3),
    /* K9   */  be_nested_string("sec", -1155074638, 3),
    }),
    (be_nested_const_str("time_str", -1681139684, 8)),
    (be_nested_const_str("Tasmota.be", 825809411, 10)),
    ( &(const binstruction[14]) {  /* code */
      0xA40A0000,  //  0000  IMPORT R2  K0
      0x8C0C0101,  //  0001  GETMET R3  R0  K1
      0x5C140200,  //  0002  MOVE R5  R1
      0x7C0C0400,  //  0003  CALL R3  2
      0x8C100502,  //  0004  GETMET R4  R2  K2
      0x58180003,  //  0005  LDCONST  R6  K3
      0x941C0704,  //  0006  GETIDX R7  R3  K4
      0x94200705,  //  0007  GETIDX R8  R3  K5
      0x94240706,  //  0008  GETIDX R9  R3  K6
      0x94280707,  //  0009  GETIDX R10 R3  K7
      0x942C0708,  //  000A  GETIDX R11 R3  K8
      0x94300709,  //  000B  GETIDX R12 R3  K9
      0x7C101000,  //  000C  CALL R4  8
      0x80040800,  //  000D  RET  1 R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: load
********************************************************************/
be_local_closure(load,   /* name */
  be_nested_proto(
    15,                          /* nstack */
    2,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[18]) {     /* constants */
    /* K0   */  be_nested_string("string", 398550328, 6),
    /* K1   */  be_nested_string("path", -2071507658, 4),
    /* K2   */  be_nested_string("find", -1108310694, 4),
    /* K3   */  be_nested_string(".", 722245873, 1),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_string(".be", 1325797348, 3),
    /* K6   */  be_nested_string("split", -2017972765, 5),
    /* K7   */  be_const_int(1),
    /* K8   */  be_nested_string("be", 942383232, 2),
    /* K9   */  be_nested_string("bec", 1336821081, 3),
    /* K10  */  be_nested_string("io_error", 1970281036, 8),
    /* K11  */  be_nested_string("file extension is not '.be' or '.bec'", -1199247657, 37),
    /* K12  */  be_nested_string("c", -435409838, 1),
    /* K13  */  be_nested_string("last_modified", 772177145, 13),
    /* K14  */  be_nested_string("file", -1427482813, 4),
    /* K15  */  be_nested_string("save", -855671224, 4),
    /* K16  */  be_nested_string("format", -1180859054, 6),
    /* K17  */  be_nested_string("BRY: could not save compiled file %s (%s)", 736659787, 41),
    }),
    (be_nested_const_str("load", -435725847, 4)),
    (be_nested_const_str("input", -103256197, 5)),
    ( &(const binstruction[87]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0xA40E0200,  //  0001  IMPORT	R3	K1
      0x8C100502,  //  0002  GETMET	R4	R2	K2
      0x5C180200,  //  0003  MOVE	R6	R1
      0x581C0003,  //  0004  LDCONST	R7	K3
      0x7C100600,  //  0005  CALL	R4	3
      0x14100904,  //  0006  LT	R4	R4	K4
      0x78120000,  //  0007  JMPF	R4	#0009
      0x00040305,  //  0008  ADD	R1	R1	K5
      0x8C100506,  //  0009  GETMET	R4	R2	K6
      0x5C180200,  //  000A  MOVE	R6	R1
      0x581C0003,  //  000B  LDCONST	R7	K3
      0x7C100600,  //  000C  CALL	R4	3
      0x6014000C,  //  000D  GETGBL	R5	G12
      0x5C180800,  //  000E  MOVE	R6	R4
      0x7C140200,  //  000F  CALL	R5	1
      0x18140B07,  //  0010  LE	R5	R5	K7
      0x74160007,  //  0011  JMPT	R5	#001A
      0x5415FFFE,  //  0012  LDINT	R5	-1
      0x94140805,  //  0013  GETIDX	R5	R4	R5
      0x20140B08,  //  0014  NE	R5	R5	K8
      0x78160004,  //  0015  JMPF	R5	#001B
      0x5415FFFE,  //  0016  LDINT	R5	-1
      0x94140805,  //  0017  GETIDX	R5	R4	R5
      0x20140B09,  //  0018  NE	R5	R5	K9
      0x78160000,  //  0019  JMPF	R5	#001B
      0xB006150B,  //  001A  RAISE	1	K10	K11
      0x5415FFFE,  //  001B  LDINT	R5	-1
      0x94140205,  //  001C  GETIDX	R5	R1	R5
      0x1C140B0C,  //  001D  EQ	R5	R5	K12
      0x8C18070D,  //  001E  GETMET	R6	R3	K13
      0x5C200200,  //  001F  MOVE	R8	R1
      0x7C180400,  //  0020  CALL	R6	2
      0x78160005,  //  0021  JMPF	R5	#0028
      0x4C1C0000,  //  0022  LDNIL	R7
      0x1C1C0C07,  //  0023  EQ	R7	R6	R7
      0x781E0001,  //  0024  JMPF	R7	#0027
      0x501C0000,  //  0025  LDBOOL	R7	0	0
      0x80040E00,  //  0026  RET	1	R7
      0x70020013,  //  0027  JMP		#003C
      0x8C1C070D,  //  0028  GETMET	R7	R3	K13
      0x0024030C,  //  0029  ADD	R9	R1	K12
      0x7C1C0400,  //  002A  CALL	R7	2
      0x4C200000,  //  002B  LDNIL	R8
      0x1C200C08,  //  002C  EQ	R8	R6	R8
      0x78220004,  //  002D  JMPF	R8	#0033
      0x4C200000,  //  002E  LDNIL	R8
      0x1C200E08,  //  002F  EQ	R8	R7	R8
      0x78220001,  //  0030  JMPF	R8	#0033
      0x50200000,  //  0031  LDBOOL	R8	0	0
      0x80041000,  //  0032  RET	1	R8
      0x4C200000,  //  0033  LDNIL	R8
      0x20200E08,  //  0034  NE	R8	R7	R8
      0x78220005,  //  0035  JMPF	R8	#003C
      0x4C200000,  //  0036  LDNIL	R8
      0x1C200C08,  //  0037  EQ	R8	R6	R8
      0x74220001,  //  0038  JMPT	R8	#003B
      0x28200E06,  //  0039  GE	R8	R7	R6
      0x78220000,  //  003A  JMPF	R8	#003C
      0x50140200,  //  003B  LDBOOL	R5	1	0
      0x601C000D,  //  003C  GETGBL	R7	G13
      0x5C200200,  //  003D  MOVE	R8	R1
      0x5824000E,  //  003E  LDCONST	R9	K14
      0x7C1C0400,  //  003F  CALL	R7	2
      0x74160011,  //  0040  JMPT	R5	#0053
      0xA8020005,  //  0041  EXBLK	0	#0048
      0x8C20010F,  //  0042  GETMET	R8	R0	K15
      0x0028030C,  //  0043  ADD	R10	R1	K12
      0x5C2C0E00,  //  0044  MOVE	R11	R7
      0x7C200600,  //  0045  CALL	R8	3
      0xA8040001,  //  0046  EXBLK	1	1
      0x7002000A,  //  0047  JMP		#0053
      0xAC200001,  //  0048  CATCH	R8	0	1
      0x70020007,  //  0049  JMP		#0052
      0x60240001,  //  004A  GETGBL	R9	G1
      0x8C280510,  //  004B  GETMET	R10	R2	K16
      0x58300011,  //  004C  LDCONST	R12	K17
      0x0034030C,  //  004D  ADD	R13	R1	K12
      0x5C381000,  //  004E  MOVE	R14	R8
      0x7C280800,  //  004F  CALL	R10	4
      0x7C240200,  //  0050  CALL	R9	1
      0x70020000,  //  0051  JMP		#0053
      0xB0080000,  //  0052  RAISE	2	R0	R0
      0x5C200E00,  //  0053  MOVE	R8	R7
      0x7C200000,  //  0054  CALL	R8	0
      0x50200200,  //  0055  LDBOOL	R8	1	0
      0x80041000,  //  0056  RET	1	R8
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: find_op
********************************************************************/
be_local_closure(find_op,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    2,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_string("string", 398550328, 6),
    /* K1   */  be_nested_string("=<>!", -1630497019, 4),
    /* K2   */  be_nested_string("chars_in_string", -1146182164, 15),
    /* K3   */  be_const_int(0),
    /* K4   */  be_nested_string("split", -2017972765, 5),
    /* K5   */  be_const_int(1),
    }),
    (be_nested_const_str("find_op", -528253920, 7)),
    (be_nested_const_str("Tasmota.be", 825809411, 10)),
    ( &(const binstruction[42]) {  /* code */
      0xA40A0000,  //  0000  IMPORT R2  K0
      0x580C0001,  //  0001  LDCONST  R3  K1
      0x8C100102,  //  0002  GETMET R4  R0  K2
      0x5C180200,  //  0003  MOVE R6  R1
      0x5C1C0600,  //  0004  MOVE R7  R3
      0x7C100600,  //  0005  CALL R4  3
      0x28140903,  //  0006  GE R5  R4  K3
      0x78160019,  //  0007  JMPF R5  #0022
      0x8C140504,  //  0008  GETMET R5  R2  K4
      0x5C1C0200,  //  0009  MOVE R7  R1
      0x5C200800,  //  000A  MOVE R8  R4
      0x7C140600,  //  000B  CALL R5  3
      0x94180B03,  //  000C  GETIDX R6  R5  K3
      0x941C0B05,  //  000D  GETIDX R7  R5  K5
      0x8C200102,  //  000E  GETMET R8  R0  K2
      0x5C280E00,  //  000F  MOVE R10 R7
      0x5C2C0600,  //  0010  MOVE R11 R3
      0x50300200,  //  0011  LDBOOL R12 1 0
      0x7C200800,  //  0012  CALL R8  4
      0x5C101000,  //  0013  MOVE R4  R8
      0x28200903,  //  0014  GE R8  R4  K3
      0x7822000B,  //  0015  JMPF R8  #0022
      0x8C200504,  //  0016  GETMET R8  R2  K4
      0x5C280E00,  //  0017  MOVE R10 R7
      0x5C2C0800,  //  0018  MOVE R11 R4
      0x7C200600,  //  0019  CALL R8  3
      0x94241103,  //  001A  GETIDX R9  R8  K3
      0x94281105,  //  001B  GETIDX R10 R8  K5
      0x602C0012,  //  001C  GETGBL R11 G18
      0x7C2C0000,  //  001D  CALL R11 0
      0x40301606,  //  001E  CONNECT  R12 R11 R6
      0x40301609,  //  001F  CONNECT  R12 R11 R9
      0x4030160A,  //  0020  CONNECT  R12 R11 R10
      0x80041600,  //  0021  RET  1 R11
      0x60140012,  //  0022  GETGBL R5  G18
      0x7C140000,  //  0023  CALL R5  0
      0x40180A01,  //  0024  CONNECT  R6  R5  R1
      0x4C180000,  //  0025  LDNIL  R6
      0x40180A06,  //  0026  CONNECT  R6  R5  R6
      0x4C180000,  //  0027  LDNIL  R6
      0x40180A06,  //  0028  CONNECT  R6  R5  R6
      0x80040A00,  //  0029  RET  1 R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: remove_timer
********************************************************************/
be_local_closure(remove_timer,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_string("tasmota", 424643812, 7),
    /* K1   */  be_nested_string("_timers", -1694866380, 7),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_string("size", 597743964, 4),
    /* K4   */  be_nested_string("id", 926444256, 2),
    /* K5   */  be_nested_string("remove", -611183107, 6),
    /* K6   */  be_const_int(1),
    }),
    (be_nested_const_str("remove_timer", -153495081, 12)),
    (be_nested_const_str("Tasmota.be", 825809411, 10)),
    ( &(const binstruction[23]) {  /* code */
      0xB80A0000,  //  0000  GETNGBL  R2  K0
      0x88080501,  //  0001  GETMBR R2  R2  K1
      0x780A0012,  //  0002  JMPF R2  #0016
      0x58080002,  //  0003  LDCONST  R2  K2
      0xB80E0000,  //  0004  GETNGBL  R3  K0
      0x880C0701,  //  0005  GETMBR R3  R3  K1
      0x8C0C0703,  //  0006  GETMET R3  R3  K3
      0x7C0C0200,  //  0007  CALL R3  1
      0x140C0403,  //  0008  LT R3  R2  R3
      0x780E000B,  //  0009  JMPF R3  #0016
      0x880C0101,  //  000A  GETMBR R3  R0  K1
      0x940C0602,  //  000B  GETIDX R3  R3  R2
      0x880C0704,  //  000C  GETMBR R3  R3  K4
      0x1C0C0601,  //  000D  EQ R3  R3  R1
      0x780E0004,  //  000E  JMPF R3  #0014
      0x880C0101,  //  000F  GETMBR R3  R0  K1
      0x8C0C0705,  //  0010  GETMET R3  R3  K5
      0x5C140400,  //  0011  MOVE R5  R2
      0x7C0C0400,  //  0012  CALL R3  2
      0x70020000,  //  0013  JMP    #0015
      0x00080506,  //  0014  ADD  R2  R2  K6
      0x7001FFED,  //  0015  JMP    #0004
      0x80000000,  //  0016  RET  0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_light
********************************************************************/
be_local_closure(get_light,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_string("tasmota.get_light() is deprecated, use light.get()", -769213649, 50),
    /* K1   */  be_nested_string("light", -493019601, 5),
    /* K2   */  be_nested_string("get", 1410115415, 3),
    }),
    (be_nested_const_str("get_light", 381930476, 9)),
    (be_nested_const_str("Tasmota.be", 825809411, 10)),
    ( &(const binstruction[16]) {  /* code */
      0x60080001,  //  0000  GETGBL R2  G1
      0x580C0000,  //  0001  LDCONST  R3  K0
      0x7C080200,  //  0002  CALL R2  1
      0xA40A0200,  //  0003  IMPORT R2  K1
      0x4C0C0000,  //  0004  LDNIL  R3
      0x200C0203,  //  0005  NE R3  R1  R3
      0x780E0004,  //  0006  JMPF R3  #000C
      0x8C0C0502,  //  0007  GETMET R3  R2  K2
      0x5C140200,  //  0008  MOVE R5  R1
      0x7C0C0400,  //  0009  CALL R3  2
      0x80040600,  //  000A  RET  1 R3
      0x70020002,  //  000B  JMP    #000F
      0x8C0C0502,  //  000C  GETMET R3  R2  K2
      0x7C0C0200,  //  000D  CALL R3  1
      0x80040600,  //  000E  RET  1 R3
      0x80000000,  //  000F  RET  0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: exec_rules
********************************************************************/
be_local_closure(exec_rules,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    2,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 1]) {
      be_nested_proto(
        8,                          /* nstack */
        3,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 2]) {  /* upvals */
          be_local_const_upval(1, 0),
          be_local_const_upval(1, 3),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 1]) {     /* constants */
        /* K0   */  be_nested_string("try_rule", 1986449405, 8),
        }),
        (be_nested_const_str("<lambda>", 607256038, 8)),
        (be_nested_const_str("tasmota.be", 1128870755, 10)),
        ( &(const binstruction[11]) {  /* code */
          0x680C0000,  //  0000  GETUPV	R3	U0
          0x8C0C0700,  //  0001  GETMET	R3	R3	K0
          0x68140001,  //  0002  GETUPV	R5	U1
          0x5C180000,  //  0003  MOVE	R6	R0
          0x5C1C0200,  //  0004  MOVE	R7	R1
          0x7C0C0800,  //  0005  CALL	R3	4
          0x740E0001,  //  0006  JMPT	R3	#0009
          0x740A0000,  //  0007  JMPT	R2	#0009
          0x50080001,  //  0008  LDBOOL	R2	0	1
          0x50080200,  //  0009  LDBOOL	R2	1	0
          0x80040400,  //  000A  RET	1	R2
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_string("_rules", -28750191, 6),
    /* K1   */  be_nested_string("cmd_res", 921166762, 7),
    /* K2   */  be_nested_string("json", 916562499, 4),
    /* K3   */  be_nested_string("load", -435725847, 4),
    /* K4   */  be_nested_string("log", 1062293841, 3),
    /* K5   */  be_nested_string("BRY: ERROR, bad json: ", -1579831487, 22),
    /* K6   */  be_const_int(3),
    /* K7   */  be_nested_string("reduce", 2002030311, 6),
    /* K8   */  be_nested_string("stop_iteration", -121173395, 14),
    }),
    (be_nested_const_str("exec_rules", 1445221092, 10)),
    (be_nested_const_str("tasmota.be", 1128870755, 10)),
    ( &(const binstruction[47]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x740A0003,  //  0001  JMPT	R2	#0006
      0x88080101,  //  0002  GETMBR	R2	R0	K1
      0x4C0C0000,  //  0003  LDNIL	R3
      0x20080403,  //  0004  NE	R2	R2	R3
      0x780A0025,  //  0005  JMPF	R2	#002C
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
      0x7816000E,  //  0019  JMPF	R5	#0029
      0xA8020008,  //  001A  EXBLK	0	#0024
      0x88140100,  //  001B  GETMBR	R5	R0	K0
      0x8C140B07,  //  001C  GETMET	R5	R5	K7
      0x841C0000,  //  001D  CLOSURE	R7	P0
      0x4C200000,  //  001E  LDNIL	R8
      0x50240000,  //  001F  LDBOOL	R9	0	0
      0x7C140800,  //  0020  CALL	R5	4
      0x5C100A00,  //  0021  MOVE	R4	R5
      0xA8040001,  //  0022  EXBLK	1	1
      0x70020004,  //  0023  JMP		#0029
      0x58140008,  //  0024  LDCONST	R5	K8
      0xAC140200,  //  0025  CATCH	R5	1	0
      0x70020000,  //  0026  JMP		#0028
      0x70020000,  //  0027  JMP		#0029
      0xB0080000,  //  0028  RAISE	2	R0	R0
      0xA0000000,  //  0029  CLOSE	R0
      0x80040800,  //  002A  RET	1	R4
      0xA0080000,  //  002B  CLOSE	R2
      0x50080000,  //  002C  LDBOOL	R2	0	0
      0xA0000000,  //  002D  CLOSE	R0
      0x80040400,  //  002E  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: remove_driver
********************************************************************/
be_local_closure(remove_driver,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_string("_drivers", -1034638311, 8),
    /* K1   */  be_nested_string("find", -1108310694, 4),
    /* K2   */  be_nested_string("pop", 1362321360, 3),
    }),
    (be_nested_const_str("remove_driver", 1030243768, 13)),
    (be_nested_const_str("Tasmota.be", 825809411, 10)),
    ( &(const binstruction[14]) {  /* code */
      0x88080100,  //  0000  GETMBR R2  R0  K0
      0x780A000A,  //  0001  JMPF R2  #000D
      0x88080100,  //  0002  GETMBR R2  R0  K0
      0x8C080501,  //  0003  GETMET R2  R2  K1
      0x5C100200,  //  0004  MOVE R4  R1
      0x7C080400,  //  0005  CALL R2  2
      0x4C0C0000,  //  0006  LDNIL  R3
      0x200C0403,  //  0007  NE R3  R2  R3
      0x780E0003,  //  0008  JMPF R3  #000D
      0x880C0100,  //  0009  GETMBR R3  R0  K0
      0x8C0C0702,  //  000A  GETMET R3  R3  K2
      0x5C140400,  //  000B  MOVE R5  R2
      0x7C0C0400,  //  000C  CALL R3  2
      0x80000000,  //  000D  RET  0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: try_rule
********************************************************************/
be_local_closure(try_rule,   /* name */
  be_nested_proto(
    15,                          /* nstack */
    4,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[16]) {     /* constants */
    /* K0   */  be_nested_string("string", 398550328, 6),
    /* K1   */  be_nested_string("find_op", -528253920, 7),
    /* K2   */  be_nested_string("split", -2017972765, 5),
    /* K3   */  be_const_int(0),
    /* K4   */  be_nested_string("#", 638357778, 1),
    /* K5   */  be_nested_string("find_key_i", 850136726, 10),
    /* K6   */  be_const_int(1),
    /* K7   */  be_const_int(2),
    /* K8   */  be_nested_string("==", -1863000881, 2),
    /* K9   */  be_nested_string("!==", 559817114, 3),
    /* K10  */  be_nested_string("=", 940354920, 1),
    /* K11  */  be_nested_string("!=", -1866252285, 2),
    /* K12  */  be_nested_string(">", 990687777, 1),
    /* K13  */  be_nested_string(">=", 284975636, 2),
    /* K14  */  be_nested_string("<", 957132539, 1),
    /* K15  */  be_nested_string("<=", -1795743310, 2),
    }),
    (be_nested_const_str("try_rule", 1986449405, 8)),
    (be_nested_const_str("input", -103256197, 5)),
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
** Solidified function: cb_dispatch
********************************************************************/
be_local_closure(cb_dispatch,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    6,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_string("_cb", -251666929, 3),
    /* K1   */  be_const_int(0),
    /* K2   */  be_nested_string("find", -1108310694, 4),
    }),
    (be_nested_const_str("cb_dispatch", 1741510499, 11)),
    (be_nested_const_str("Tasmota.be", 825809411, 10)),
    ( &(const binstruction[20]) {  /* code */
      0x88180100,  //  0000  GETMBR R6  R0  K0
      0x4C1C0000,  //  0001  LDNIL  R7
      0x1C180C07,  //  0002  EQ R6  R6  R7
      0x781A0000,  //  0003  JMPF R6  #0005
      0x80060200,  //  0004  RET  1 K1
      0x88180100,  //  0005  GETMBR R6  R0  K0
      0x8C180D02,  //  0006  GETMET R6  R6  K2
      0x5C200200,  //  0007  MOVE R8  R1
      0x7C180400,  //  0008  CALL R6  2
      0x4C1C0000,  //  0009  LDNIL  R7
      0x201C0C07,  //  000A  NE R7  R6  R7
      0x781E0006,  //  000B  JMPF R7  #0013
      0x5C1C0C00,  //  000C  MOVE R7  R6
      0x5C200400,  //  000D  MOVE R8  R2
      0x5C240600,  //  000E  MOVE R9  R3
      0x5C280800,  //  000F  MOVE R10 R4
      0x5C2C0A00,  //  0010  MOVE R11 R5
      0x7C1C0800,  //  0011  CALL R7  4
      0x80040E00,  //  0012  RET  1 R7
      0x80060200,  //  0013  RET  1 K1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: gc
********************************************************************/
be_local_closure(gc,   /* name */
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
    /* K0   */  be_nested_string("gc", 1042313471, 2),
    /* K1   */  be_nested_string("collect", -1895928271, 7),
    /* K2   */  be_nested_string("allocated", 429986098, 9),
    }),
    (be_nested_const_str("gc", 1042313471, 2)),
    (be_nested_const_str("Tasmota.be", 825809411, 10)),
    ( &(const binstruction[ 6]) {  /* code */
      0xA4060000,  //  0000  IMPORT R1  K0
      0x8C080301,  //  0001  GETMET R2  R1  K1
      0x7C080200,  //  0002  CALL R2  1
      0x8C080302,  //  0003  GETMET R2  R1  K2
      0x7C080200,  //  0004  CALL R2  1
      0x80040400,  //  0005  RET  1 R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: event
********************************************************************/
be_local_closure(event,   /* name */
  be_nested_proto(
    21,                          /* nstack */
    6,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[21]) {     /* constants */
    /* K0   */  be_nested_string("introspect", 164638290, 10),
    /* K1   */  be_nested_string("debug", 1483009432, 5),
    /* K2   */  be_nested_string("string", 398550328, 6),
    /* K3   */  be_nested_string("every_50ms", -1911083288, 10),
    /* K4   */  be_nested_string("run_deferred", 371594696, 12),
    /* K5   */  be_nested_string("cmd", -158181397, 3),
    /* K6   */  be_nested_string("exec_cmd", 493567399, 8),
    /* K7   */  be_nested_string("rule", -64077613, 4),
    /* K8   */  be_nested_string("exec_rules", 1445221092, 10),
    /* K9   */  be_nested_string("gc", 1042313471, 2),
    /* K10  */  be_nested_string("_drivers", -1034638311, 8),
    /* K11  */  be_const_int(0),
    /* K12  */  be_nested_string("get", 1410115415, 3),
    /* K13  */  be_nested_string("function", -1630125495, 8),
    /* K14  */  be_nested_string("format", -1180859054, 6),
    /* K15  */  be_nested_string("BRY: Exception> '%s' - %s", -2047976332, 25),
    /* K16  */  be_nested_string("traceback", -909779187, 9),
    /* K17  */  be_const_int(1),
    /* K18  */  be_nested_string("save_before_restart", 1253239338, 19),
    /* K19  */  be_nested_string("persist", -377883517, 7),
    /* K20  */  be_nested_string("save", -855671224, 4),
    }),
    (be_nested_const_str("event", -30355297, 5)),
    (be_nested_const_str("input", -103256197, 5)),
    ( &(const binstruction[84]) {  /* code */
      0xA41A0000,  //  0000  IMPORT	R6	K0
      0xA41E0200,  //  0001  IMPORT	R7	K1
      0xA4220400,  //  0002  IMPORT	R8	K2
      0x1C240303,  //  0003  EQ	R9	R1	K3
      0x78260001,  //  0004  JMPF	R9	#0007
      0x8C240104,  //  0005  GETMET	R9	R0	K4
      0x7C240200,  //  0006  CALL	R9	1
      0x50240000,  //  0007  LDBOOL	R9	0	0
      0x1C280305,  //  0008  EQ	R10	R1	K5
      0x782A0006,  //  0009  JMPF	R10	#0011
      0x8C280106,  //  000A  GETMET	R10	R0	K6
      0x5C300400,  //  000B  MOVE	R12	R2
      0x5C340600,  //  000C  MOVE	R13	R3
      0x5C380800,  //  000D  MOVE	R14	R4
      0x7C280800,  //  000E  CALL	R10	4
      0x80041400,  //  000F  RET	1	R10
      0x7002003C,  //  0010  JMP		#004E
      0x1C280307,  //  0011  EQ	R10	R1	K7
      0x782A0004,  //  0012  JMPF	R10	#0018
      0x8C280108,  //  0013  GETMET	R10	R0	K8
      0x5C300800,  //  0014  MOVE	R12	R4
      0x7C280400,  //  0015  CALL	R10	2
      0x80041400,  //  0016  RET	1	R10
      0x70020035,  //  0017  JMP		#004E
      0x1C280309,  //  0018  EQ	R10	R1	K9
      0x782A0003,  //  0019  JMPF	R10	#001E
      0x8C280109,  //  001A  GETMET	R10	R0	K9
      0x7C280200,  //  001B  CALL	R10	1
      0x80041400,  //  001C  RET	1	R10
      0x7002002F,  //  001D  JMP		#004E
      0x8828010A,  //  001E  GETMBR	R10	R0	K10
      0x782A002D,  //  001F  JMPF	R10	#004E
      0x5828000B,  //  0020  LDCONST	R10	K11
      0x602C000C,  //  0021  GETGBL	R11	G12
      0x8830010A,  //  0022  GETMBR	R12	R0	K10
      0x7C2C0200,  //  0023  CALL	R11	1
      0x142C140B,  //  0024  LT	R11	R10	R11
      0x782E0027,  //  0025  JMPF	R11	#004E
      0x882C010A,  //  0026  GETMBR	R11	R0	K10
      0x942C160A,  //  0027  GETIDX	R11	R11	R10
      0x8C300D0C,  //  0028  GETMET	R12	R6	K12
      0x5C381600,  //  0029  MOVE	R14	R11
      0x5C3C0200,  //  002A  MOVE	R15	R1
      0x7C300600,  //  002B  CALL	R12	3
      0x60340004,  //  002C  GETGBL	R13	G4
      0x5C381800,  //  002D  MOVE	R14	R12
      0x7C340200,  //  002E  CALL	R13	1
      0x1C341B0D,  //  002F  EQ	R13	R13	K13
      0x7836001A,  //  0030  JMPF	R13	#004C
      0xA802000C,  //  0031  EXBLK	0	#003F
      0x5C341800,  //  0032  MOVE	R13	R12
      0x5C381600,  //  0033  MOVE	R14	R11
      0x5C3C0400,  //  0034  MOVE	R15	R2
      0x5C400600,  //  0035  MOVE	R16	R3
      0x5C440800,  //  0036  MOVE	R17	R4
      0x5C480A00,  //  0037  MOVE	R18	R5
      0x7C340A00,  //  0038  CALL	R13	5
      0x5C241A00,  //  0039  MOVE	R9	R13
      0x78260001,  //  003A  JMPF	R9	#003D
      0xA8040001,  //  003B  EXBLK	1	1
      0x70020010,  //  003C  JMP		#004E
      0xA8040001,  //  003D  EXBLK	1	1
      0x7002000C,  //  003E  JMP		#004C
      0xAC340002,  //  003F  CATCH	R13	0	2
      0x70020009,  //  0040  JMP		#004B
      0x603C0001,  //  0041  GETGBL	R15	G1
      0x8C40110E,  //  0042  GETMET	R16	R8	K14
      0x5848000F,  //  0043  LDCONST	R18	K15
      0x5C4C1A00,  //  0044  MOVE	R19	R13
      0x5C501C00,  //  0045  MOVE	R20	R14
      0x7C400800,  //  0046  CALL	R16	4
      0x7C3C0200,  //  0047  CALL	R15	1
      0x8C3C0F10,  //  0048  GETMET	R15	R7	K16
      0x7C3C0200,  //  0049  CALL	R15	1
      0x70020000,  //  004A  JMP		#004C
      0xB0080000,  //  004B  RAISE	2	R0	R0
      0x00281511,  //  004C  ADD	R10	R10	K17
      0x7001FFD2,  //  004D  JMP		#0021
      0x1C280312,  //  004E  EQ	R10	R1	K18
      0x782A0002,  //  004F  JMPF	R10	#0053
      0xA42A2600,  //  0050  IMPORT	R10	K19
      0x8C2C1514,  //  0051  GETMET	R11	R10	K20
      0x7C2C0200,  //  0052  CALL	R11	1
      0x80041200,  //  0053  RET	1	R9
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: remove_cmd
********************************************************************/
be_local_closure(remove_cmd,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_string("_ccmd", -2131545883, 5),
    /* K1   */  be_nested_string("remove", -611183107, 6),
    }),
    (be_nested_const_str("remove_cmd", -462651594, 10)),
    (be_nested_const_str("Tasmota.be", 825809411, 10)),
    ( &(const binstruction[ 7]) {  /* code */
      0x88080100,  //  0000  GETMBR R2  R0  K0
      0x780A0003,  //  0001  JMPF R2  #0006
      0x88080100,  //  0002  GETMBR R2  R0  K0
      0x8C080501,  //  0003  GETMET R2  R2  K1
      0x5C100200,  //  0004  MOVE R4  R1
      0x7C080400,  //  0005  CALL R2  2
      0x80000000,  //  0006  RET  0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: exec_cmd
********************************************************************/
be_local_closure(exec_cmd,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    4,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_string("_ccmd", -2131545883, 5),
    /* K1   */  be_nested_string("json", 916562499, 4),
    /* K2   */  be_nested_string("load", -435725847, 4),
    /* K3   */  be_nested_string("find_key_i", 850136726, 10),
    /* K4   */  be_nested_string("resolvecmnd", 993361485, 11),
    }),
    (be_nested_const_str("exec_cmd", 493567399, 8)),
    (be_nested_const_str("Tasmota.be", 825809411, 10)),
    ( &(const binstruction[27]) {  /* code */
      0x88100100,  //  0000  GETMBR R4  R0  K0
      0x78120016,  //  0001  JMPF R4  #0019
      0xA4120200,  //  0002  IMPORT R4  K1
      0x8C140902,  //  0003  GETMET R5  R4  K2
      0x5C1C0600,  //  0004  MOVE R7  R3
      0x7C140400,  //  0005  CALL R5  2
      0x8C180103,  //  0006  GETMET R6  R0  K3
      0x88200100,  //  0007  GETMBR R8  R0  K0
      0x5C240200,  //  0008  MOVE R9  R1
      0x7C180600,  //  0009  CALL R6  3
      0x4C1C0000,  //  000A  LDNIL  R7
      0x201C0C07,  //  000B  NE R7  R6  R7
      0x781E000B,  //  000C  JMPF R7  #0019
      0x8C1C0104,  //  000D  GETMET R7  R0  K4
      0x5C240C00,  //  000E  MOVE R9  R6
      0x7C1C0400,  //  000F  CALL R7  2
      0x881C0100,  //  0010  GETMBR R7  R0  K0
      0x941C0E06,  //  0011  GETIDX R7  R7  R6
      0x5C200C00,  //  0012  MOVE R8  R6
      0x5C240400,  //  0013  MOVE R9  R2
      0x5C280600,  //  0014  MOVE R10 R3
      0x5C2C0A00,  //  0015  MOVE R11 R5
      0x7C1C0800,  //  0016  CALL R7  4
      0x501C0200,  //  0017  LDBOOL R7  1 0
      0x80040E00,  //  0018  RET  1 R7
      0x50100000,  //  0019  LDBOOL R4  0 0
      0x80040800,  //  001A  RET  1 R4
    })
  )
);
/*******************************************************************/


#include "../generate/be_fixed_be_class_tasmota.h"


// Class definition
void be_load_tasmota_ntvlib(bvm *vm)
{
    be_pushntvclass(vm, &be_class_tasmota);
    be_setglobal(vm, "Tasmota");
    be_pop(vm, 1);
}

/* @const_object_info_begin
class be_class_tasmota (scope: global, name: Tasmota) {
    _rules, var
    _timers, var
    _ccmd, var
    _drivers, var
    _cb, var
    wire1, var
    wire2, var
    global, var
    cmd_res, var

    _global_def, comptr(&be_tasmota_global_struct)
    _global_addr, comptr(&TasmotaGlobal)

    init, closure(init_closure)

    get_free_heap, func(l_getFreeHeap)
    publish, func(l_publish)
    publish_result, func(l_publish_result)
    _cmd, func(l_cmd)
    _get_cb, func(l_get_cb)
    get_option, func(l_getoption)
    millis, func(l_millis)
    time_reached, func(l_timereached)
    rtc, func(l_rtc)
    time_dump, func(l_time_dump)
    strftime, func(l_strftime)
    memory, func(l_memory)
    wifi, func(l_wifi)
    eth, func(l_eth)
    yield, func(l_yield)
    delay, func(l_delay)
    scale_uint, func(l_scaleuint)
    log, func(l_logInfo)
    save, func(l_save)

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

    cmd, closure(cmd_closure)
    chars_in_string, closure(chars_in_string_closure)
    find_key_i, closure(find_key_i_closure)
    find_op, closure(find_op_closure)
    add_rule, closure(add_rule_closure)
    remove_rule, closure(remove_rule_closure)
    try_rule, closure(try_rule_closure)
    exec_rules, closure(exec_rules_closure)
    set_timer, closure(set_timer_closure)
    run_deferred, closure(run_deferred_closure)
    remove_timer, closure(remove_timer_closure)
    add_cmd, closure(add_cmd_closure)
    remove_cmd, closure(remove_cmd_closure)
    exec_cmd, closure(exec_cmd_closure)
    gc, closure(gc_closure)
    event, closure(event_closure)
    add_driver, closure(add_driver_closure)
    remove_driver, closure(remove_driver_closure)
    load, closure(load_closure)
    wire_scan, closure(wire_scan_closure)
    time_str, closure(time_str_closure)

    cb_dispatch, closure(cb_dispatch_closure)
    gen_cb, closure(gen_cb_closure)

    get_light, closure(get_light_closure)
    set_light, closure(set_light_closure)
}
@const_object_info_end */
