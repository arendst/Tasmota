/* Solidification of Matter_Fabric.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Fabric;

/********************************************************************
** Solidified function: counter_group_data_snd_next
********************************************************************/
be_local_closure(Matter_Fabric_counter_group_data_snd_next,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[15]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(_counter_group_data_snd_impl),
    /* K2   */  be_nested_str_weak(next),
    /* K3   */  be_nested_str_weak(tasmota),
    /* K4   */  be_nested_str_weak(log),
    /* K5   */  be_nested_str_weak(format),
    /* K6   */  be_nested_str_weak(MTR_X3A_X20_X2E_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20Counter_group_data_snd_X3D_X25i),
    /* K7   */  be_const_int(3),
    /* K8   */  be_nested_str_weak(matter),
    /* K9   */  be_nested_str_weak(Counter),
    /* K10  */  be_nested_str_weak(is_greater),
    /* K11  */  be_nested_str_weak(counter_group_data_snd),
    /* K12  */  be_nested_str_weak(_GROUP_SND_INCR),
    /* K13  */  be_nested_str_weak(does_persist),
    /* K14  */  be_nested_str_weak(save),
    }),
    be_str_weak(counter_group_data_snd_next),
    &be_const_str_solidified,
    ( &(const binstruction[28]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x88080101,  //  0001  GETMBR	R2	R0	K1
      0x8C080502,  //  0002  GETMET	R2	R2	K2
      0x7C080200,  //  0003  CALL	R2	1
      0xB80E0600,  //  0004  GETNGBL	R3	K3
      0x8C0C0704,  //  0005  GETMET	R3	R3	K4
      0x8C140305,  //  0006  GETMET	R5	R1	K5
      0x581C0006,  //  0007  LDCONST	R7	K6
      0x5C200400,  //  0008  MOVE	R8	R2
      0x7C140600,  //  0009  CALL	R5	3
      0x58180007,  //  000A  LDCONST	R6	K7
      0x7C0C0600,  //  000B  CALL	R3	3
      0xB80E1000,  //  000C  GETNGBL	R3	K8
      0x880C0709,  //  000D  GETMBR	R3	R3	K9
      0x8C0C070A,  //  000E  GETMET	R3	R3	K10
      0x5C140400,  //  000F  MOVE	R5	R2
      0x8818010B,  //  0010  GETMBR	R6	R0	K11
      0x7C0C0600,  //  0011  CALL	R3	3
      0x780E0007,  //  0012  JMPF	R3	#001B
      0x880C010C,  //  0013  GETMBR	R3	R0	K12
      0x000C0403,  //  0014  ADD	R3	R2	R3
      0x90021603,  //  0015  SETMBR	R0	K11	R3
      0x8C0C010D,  //  0016  GETMET	R3	R0	K13
      0x7C0C0200,  //  0017  CALL	R3	1
      0x780E0001,  //  0018  JMPF	R3	#001B
      0x8C0C010E,  //  0019  GETMET	R3	R0	K14
      0x7C0C0200,  //  001A  CALL	R3	1
      0x80040400,  //  001B  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_ca_pub
********************************************************************/
be_local_closure(Matter_Fabric_get_ca_pub,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(root_ca_certificate),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(TLV),
    /* K3   */  be_nested_str_weak(parse),
    /* K4   */  be_nested_str_weak(findsubval),
    }),
    be_str_weak(get_ca_pub),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x78060008,  //  0001  JMPF	R1	#000B
      0xB80A0200,  //  0002  GETNGBL	R2	K1
      0x88080502,  //  0003  GETMBR	R2	R2	K2
      0x8C080503,  //  0004  GETMET	R2	R2	K3
      0x5C100200,  //  0005  MOVE	R4	R1
      0x7C080400,  //  0006  CALL	R2	2
      0x8C0C0504,  //  0007  GETMET	R3	R2	K4
      0x54160008,  //  0008  LDINT	R5	9
      0x7C0C0400,  //  0009  CALL	R3	2
      0x80040600,  //  000A  RET	1	R3
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_device_id
********************************************************************/
be_local_closure(Matter_Fabric_get_device_id,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(device_id),
    }),
    be_str_weak(get_device_id),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: add_session
********************************************************************/
be_local_closure(Matter_Fabric_add_session,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(_sessions),
    /* K1   */  be_nested_str_weak(find),
    /* K2   */  be_nested_str_weak(_MAX_CASE),
    /* K3   */  be_nested_str_weak(remove),
    /* K4   */  be_nested_str_weak(get_oldest_session),
    /* K5   */  be_nested_str_weak(push),
    }),
    be_str_weak(add_session),
    &be_const_str_solidified,
    ( &(const binstruction[27]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x4C0C0000,  //  0004  LDNIL	R3
      0x1C080403,  //  0005  EQ	R2	R2	R3
      0x780A0012,  //  0006  JMPF	R2	#001A
      0x6008000C,  //  0007  GETGBL	R2	G12
      0x880C0100,  //  0008  GETMBR	R3	R0	K0
      0x7C080200,  //  0009  CALL	R2	1
      0x880C0102,  //  000A  GETMBR	R3	R0	K2
      0x28080403,  //  000B  GE	R2	R2	R3
      0x780A0008,  //  000C  JMPF	R2	#0016
      0x88080100,  //  000D  GETMBR	R2	R0	K0
      0x8C080503,  //  000E  GETMET	R2	R2	K3
      0x88100100,  //  000F  GETMBR	R4	R0	K0
      0x8C100901,  //  0010  GETMET	R4	R4	K1
      0x8C180104,  //  0011  GETMET	R6	R0	K4
      0x7C180200,  //  0012  CALL	R6	1
      0x7C100400,  //  0013  CALL	R4	2
      0x7C080400,  //  0014  CALL	R2	2
      0x7001FFF0,  //  0015  JMP		#0007
      0x88080100,  //  0016  GETMBR	R2	R0	K0
      0x8C080505,  //  0017  GETMET	R2	R2	K5
      0x5C100200,  //  0018  MOVE	R4	R1
      0x7C080400,  //  0019  CALL	R2	2
      0x80000000,  //  001A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_oldest_session
********************************************************************/
be_local_closure(Matter_Fabric_get_oldest_session,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(get_old_recent_session),
    }),
    be_str_weak(get_oldest_session),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x500C0200,  //  0001  LDBOOL	R3	1	0
      0x7C040400,  //  0002  CALL	R1	2
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: fromjson
********************************************************************/
be_local_closure(Matter_Fabric_fromjson,   /* name */
  be_nested_proto(
    16,                          /* nstack */
    2,                          /* argc */
    4,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[18]) {     /* constants */
    /* K0   */  be_const_class(be_class_Matter_Fabric),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(introspect),
    /* K3   */  be_nested_str_weak(matter),
    /* K4   */  be_nested_str_weak(Fabric),
    /* K5   */  be_nested_str_weak(keys),
    /* K6   */  be_const_int(0),
    /* K7   */  be_nested_str_weak(_),
    /* K8   */  be_nested_str_weak(find),
    /* K9   */  be_nested_str_weak(0x),
    /* K10  */  be_nested_str_weak(set),
    /* K11  */  be_nested_str_weak(fromhex),
    /* K12  */  be_const_int(2),
    /* K13  */  be_const_int(2147483647),
    /* K14  */  be_nested_str_weak(_X24_X24),
    /* K15  */  be_nested_str_weak(fromb64),
    /* K16  */  be_nested_str_weak(stop_iteration),
    /* K17  */  be_nested_str_weak(hydrate_post),
    }),
    be_str_weak(fromjson),
    &be_const_str_solidified,
    ( &(const binstruction[76]) {  /* code */
      0x58080000,  //  0000  LDCONST	R2	K0
      0xA40E0200,  //  0001  IMPORT	R3	K1
      0xA4120400,  //  0002  IMPORT	R4	K2
      0xB8160600,  //  0003  GETNGBL	R5	K3
      0x8C140B04,  //  0004  GETMET	R5	R5	K4
      0x5C1C0000,  //  0005  MOVE	R7	R0
      0x7C140400,  //  0006  CALL	R5	2
      0x60180010,  //  0007  GETGBL	R6	G16
      0x8C1C0305,  //  0008  GETMET	R7	R1	K5
      0x7C1C0200,  //  0009  CALL	R7	1
      0x7C180200,  //  000A  CALL	R6	1
      0xA8020039,  //  000B  EXBLK	0	#0046
      0x5C1C0C00,  //  000C  MOVE	R7	R6
      0x7C1C0000,  //  000D  CALL	R7	0
      0x94200F06,  //  000E  GETIDX	R8	R7	K6
      0x1C201107,  //  000F  EQ	R8	R8	K7
      0x78220000,  //  0010  JMPF	R8	#0012
      0x7001FFF9,  //  0011  JMP		#000C
      0x94200207,  //  0012  GETIDX	R8	R1	R7
      0x60240004,  //  0013  GETGBL	R9	G4
      0x5C281000,  //  0014  MOVE	R10	R8
      0x7C240200,  //  0015  CALL	R9	1
      0x1C241301,  //  0016  EQ	R9	R9	K1
      0x78260027,  //  0017  JMPF	R9	#0040
      0x8C240708,  //  0018  GETMET	R9	R3	K8
      0x5C2C1000,  //  0019  MOVE	R11	R8
      0x58300009,  //  001A  LDCONST	R12	K9
      0x7C240600,  //  001B  CALL	R9	3
      0x1C241306,  //  001C  EQ	R9	R9	K6
      0x7826000A,  //  001D  JMPF	R9	#0029
      0x8C24090A,  //  001E  GETMET	R9	R4	K10
      0x5C2C0A00,  //  001F  MOVE	R11	R5
      0x5C300E00,  //  0020  MOVE	R12	R7
      0x60340015,  //  0021  GETGBL	R13	G21
      0x7C340000,  //  0022  CALL	R13	0
      0x8C341B0B,  //  0023  GETMET	R13	R13	K11
      0x403E190D,  //  0024  CONNECT	R15	K12	K13
      0x943C100F,  //  0025  GETIDX	R15	R8	R15
      0x7C340400,  //  0026  CALL	R13	2
      0x7C240800,  //  0027  CALL	R9	4
      0x70020015,  //  0028  JMP		#003F
      0x8C240708,  //  0029  GETMET	R9	R3	K8
      0x5C2C1000,  //  002A  MOVE	R11	R8
      0x5830000E,  //  002B  LDCONST	R12	K14
      0x7C240600,  //  002C  CALL	R9	3
      0x1C241306,  //  002D  EQ	R9	R9	K6
      0x7826000A,  //  002E  JMPF	R9	#003A
      0x8C24090A,  //  002F  GETMET	R9	R4	K10
      0x5C2C0A00,  //  0030  MOVE	R11	R5
      0x5C300E00,  //  0031  MOVE	R12	R7
      0x60340015,  //  0032  GETGBL	R13	G21
      0x7C340000,  //  0033  CALL	R13	0
      0x8C341B0F,  //  0034  GETMET	R13	R13	K15
      0x403E190D,  //  0035  CONNECT	R15	K12	K13
      0x943C100F,  //  0036  GETIDX	R15	R8	R15
      0x7C340400,  //  0037  CALL	R13	2
      0x7C240800,  //  0038  CALL	R9	4
      0x70020004,  //  0039  JMP		#003F
      0x8C24090A,  //  003A  GETMET	R9	R4	K10
      0x5C2C0A00,  //  003B  MOVE	R11	R5
      0x5C300E00,  //  003C  MOVE	R12	R7
      0x5C341000,  //  003D  MOVE	R13	R8
      0x7C240800,  //  003E  CALL	R9	4
      0x70020004,  //  003F  JMP		#0045
      0x8C24090A,  //  0040  GETMET	R9	R4	K10
      0x5C2C0A00,  //  0041  MOVE	R11	R5
      0x5C300E00,  //  0042  MOVE	R12	R7
      0x5C341000,  //  0043  MOVE	R13	R8
      0x7C240800,  //  0044  CALL	R9	4
      0x7001FFC5,  //  0045  JMP		#000C
      0x58180010,  //  0046  LDCONST	R6	K16
      0xAC180200,  //  0047  CATCH	R6	1	0
      0xB0080000,  //  0048  RAISE	2	R0	R0
      0x8C180B11,  //  0049  GETMET	R6	R5	K17
      0x7C180200,  //  004A  CALL	R6	1
      0x80040A00,  //  004B  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_fabric_index
********************************************************************/
be_local_closure(Matter_Fabric_set_fabric_index,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(fabric_index),
    }),
    be_str_weak(set_fabric_index),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x80000000,  //  0001  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_Fabric_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[18]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(_store),
    /* K2   */  be_nested_str_weak(_sessions),
    /* K3   */  be_nested_str_weak(matter),
    /* K4   */  be_nested_str_weak(Expirable_list),
    /* K5   */  be_nested_str_weak(fabric_label),
    /* K6   */  be_nested_str_weak(),
    /* K7   */  be_nested_str_weak(created),
    /* K8   */  be_nested_str_weak(tasmota),
    /* K9   */  be_nested_str_weak(rtc),
    /* K10  */  be_nested_str_weak(utc),
    /* K11  */  be_nested_str_weak(_counter_group_data_snd_impl),
    /* K12  */  be_nested_str_weak(Counter),
    /* K13  */  be_nested_str_weak(_counter_group_ctrl_snd_impl),
    /* K14  */  be_nested_str_weak(counter_group_data_snd),
    /* K15  */  be_nested_str_weak(next),
    /* K16  */  be_nested_str_weak(_GROUP_SND_INCR),
    /* K17  */  be_nested_str_weak(counter_group_ctrl_snd),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[33]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x90020201,  //  0001  SETMBR	R0	K1	R1
      0xB80E0600,  //  0002  GETNGBL	R3	K3
      0x8C0C0704,  //  0003  GETMET	R3	R3	K4
      0x7C0C0200,  //  0004  CALL	R3	1
      0x90020403,  //  0005  SETMBR	R0	K2	R3
      0x90020B06,  //  0006  SETMBR	R0	K5	K6
      0xB80E1000,  //  0007  GETNGBL	R3	K8
      0x8C0C0709,  //  0008  GETMET	R3	R3	K9
      0x7C0C0200,  //  0009  CALL	R3	1
      0x940C070A,  //  000A  GETIDX	R3	R3	K10
      0x90020E03,  //  000B  SETMBR	R0	K7	R3
      0xB80E0600,  //  000C  GETNGBL	R3	K3
      0x8C0C070C,  //  000D  GETMET	R3	R3	K12
      0x7C0C0200,  //  000E  CALL	R3	1
      0x90021603,  //  000F  SETMBR	R0	K11	R3
      0xB80E0600,  //  0010  GETNGBL	R3	K3
      0x8C0C070C,  //  0011  GETMET	R3	R3	K12
      0x7C0C0200,  //  0012  CALL	R3	1
      0x90021A03,  //  0013  SETMBR	R0	K13	R3
      0x880C010B,  //  0014  GETMBR	R3	R0	K11
      0x8C0C070F,  //  0015  GETMET	R3	R3	K15
      0x7C0C0200,  //  0016  CALL	R3	1
      0x88100110,  //  0017  GETMBR	R4	R0	K16
      0x000C0604,  //  0018  ADD	R3	R3	R4
      0x90021C03,  //  0019  SETMBR	R0	K14	R3
      0x880C010B,  //  001A  GETMBR	R3	R0	K11
      0x8C0C070F,  //  001B  GETMET	R3	R3	K15
      0x7C0C0200,  //  001C  CALL	R3	1
      0x88100110,  //  001D  GETMBR	R4	R0	K16
      0x000C0604,  //  001E  ADD	R3	R3	R4
      0x90022203,  //  001F  SETMBR	R0	K17	R3
      0x80000000,  //  0020  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: fabric_completed
********************************************************************/
be_local_closure(Matter_Fabric_fabric_completed,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(set_no_expiration),
    /* K1   */  be_nested_str_weak(set_persist),
    /* K2   */  be_nested_str_weak(assign_fabric_index),
    /* K3   */  be_nested_str_weak(_store),
    /* K4   */  be_nested_str_weak(add_fabric),
    }),
    be_str_weak(fabric_completed),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x8C040101,  //  0002  GETMET	R1	R0	K1
      0x500C0200,  //  0003  LDBOOL	R3	1	0
      0x7C040400,  //  0004  CALL	R1	2
      0x8C040102,  //  0005  GETMET	R1	R0	K2
      0x7C040200,  //  0006  CALL	R1	1
      0x88040103,  //  0007  GETMBR	R1	R0	K3
      0x8C040304,  //  0008  GETMET	R1	R1	K4
      0x5C0C0000,  //  0009  MOVE	R3	R0
      0x7C040400,  //  000A  CALL	R1	2
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: counter_group_ctrl_snd_next
********************************************************************/
be_local_closure(Matter_Fabric_counter_group_ctrl_snd_next,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[15]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(_counter_group_ctrl_snd_impl),
    /* K2   */  be_nested_str_weak(next),
    /* K3   */  be_nested_str_weak(tasmota),
    /* K4   */  be_nested_str_weak(log),
    /* K5   */  be_nested_str_weak(format),
    /* K6   */  be_nested_str_weak(MTR_X3A_X20_X2E_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20Counter_group_ctrl_snd_X3D_X25i),
    /* K7   */  be_const_int(3),
    /* K8   */  be_nested_str_weak(matter),
    /* K9   */  be_nested_str_weak(Counter),
    /* K10  */  be_nested_str_weak(is_greater),
    /* K11  */  be_nested_str_weak(counter_group_ctrl_snd),
    /* K12  */  be_nested_str_weak(_GROUP_SND_INCR),
    /* K13  */  be_nested_str_weak(does_persist),
    /* K14  */  be_nested_str_weak(save),
    }),
    be_str_weak(counter_group_ctrl_snd_next),
    &be_const_str_solidified,
    ( &(const binstruction[28]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x88080101,  //  0001  GETMBR	R2	R0	K1
      0x8C080502,  //  0002  GETMET	R2	R2	K2
      0x7C080200,  //  0003  CALL	R2	1
      0xB80E0600,  //  0004  GETNGBL	R3	K3
      0x8C0C0704,  //  0005  GETMET	R3	R3	K4
      0x8C140305,  //  0006  GETMET	R5	R1	K5
      0x581C0006,  //  0007  LDCONST	R7	K6
      0x5C200400,  //  0008  MOVE	R8	R2
      0x7C140600,  //  0009  CALL	R5	3
      0x58180007,  //  000A  LDCONST	R6	K7
      0x7C0C0600,  //  000B  CALL	R3	3
      0xB80E1000,  //  000C  GETNGBL	R3	K8
      0x880C0709,  //  000D  GETMBR	R3	R3	K9
      0x8C0C070A,  //  000E  GETMET	R3	R3	K10
      0x5C140400,  //  000F  MOVE	R5	R2
      0x8818010B,  //  0010  GETMBR	R6	R0	K11
      0x7C0C0600,  //  0011  CALL	R3	3
      0x780E0007,  //  0012  JMPF	R3	#001B
      0x880C010C,  //  0013  GETMBR	R3	R0	K12
      0x000C0403,  //  0014  ADD	R3	R2	R3
      0x90021603,  //  0015  SETMBR	R0	K11	R3
      0x8C0C010D,  //  0016  GETMET	R3	R0	K13
      0x7C0C0200,  //  0017  CALL	R3	1
      0x780E0001,  //  0018  JMPF	R3	#001B
      0x8C0C010E,  //  0019  GETMET	R3	R0	K14
      0x7C0C0200,  //  001A  CALL	R3	1
      0x80040400,  //  001B  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_ipk_epoch_key
********************************************************************/
be_local_closure(Matter_Fabric_get_ipk_epoch_key,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(ipk_epoch_key),
    }),
    be_str_weak(get_ipk_epoch_key),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tojson
********************************************************************/
be_local_closure(Matter_Fabric_tojson,   /* name */
  be_nested_proto(
    18,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[29]) {     /* constants */
    /* K0   */  be_nested_str_weak(json),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(introspect),
    /* K3   */  be_nested_str_weak(persist_pre),
    /* K4   */  be_nested_str_weak(members),
    /* K5   */  be_nested_str_weak(get),
    /* K6   */  be_nested_str_weak(function),
    /* K7   */  be_const_int(0),
    /* K8   */  be_nested_str_weak(_),
    /* K9   */  be_nested_str_weak(push),
    /* K10  */  be_nested_str_weak(stop_iteration),
    /* K11  */  be_nested_str_weak(matter),
    /* K12  */  be_nested_str_weak(sort),
    /* K13  */  be_nested_str_weak(_X24_X24),
    /* K14  */  be_nested_str_weak(tob64),
    /* K15  */  be_nested_str_weak(format),
    /* K16  */  be_nested_str_weak(_X25s_X3A_X25s),
    /* K17  */  be_nested_str_weak(dump),
    /* K18  */  be_nested_str_weak(_sessions),
    /* K19  */  be_nested_str_weak(persistables),
    /* K20  */  be_nested_str_weak(tojson),
    /* K21  */  be_nested_str_weak(_X5B),
    /* K22  */  be_nested_str_weak(concat),
    /* K23  */  be_nested_str_weak(_X2C),
    /* K24  */  be_nested_str_weak(_X5D),
    /* K25  */  be_nested_str_weak(_X22_sessions_X22_X3A),
    /* K26  */  be_nested_str_weak(persist_post),
    /* K27  */  be_nested_str_weak(_X7B),
    /* K28  */  be_nested_str_weak(_X7D),
    }),
    be_str_weak(tojson),
    &be_const_str_solidified,
    ( &(const binstruction[120]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0xA40E0400,  //  0002  IMPORT	R3	K2
      0x8C100103,  //  0003  GETMET	R4	R0	K3
      0x7C100200,  //  0004  CALL	R4	1
      0x60100012,  //  0005  GETGBL	R4	G18
      0x7C100000,  //  0006  CALL	R4	0
      0x60140010,  //  0007  GETGBL	R5	G16
      0x8C180704,  //  0008  GETMET	R6	R3	K4
      0x5C200000,  //  0009  MOVE	R8	R0
      0x7C180400,  //  000A  CALL	R6	2
      0x7C140200,  //  000B  CALL	R5	1
      0xA8020011,  //  000C  EXBLK	0	#001F
      0x5C180A00,  //  000D  MOVE	R6	R5
      0x7C180000,  //  000E  CALL	R6	0
      0x8C1C0705,  //  000F  GETMET	R7	R3	K5
      0x5C240000,  //  0010  MOVE	R9	R0
      0x5C280C00,  //  0011  MOVE	R10	R6
      0x7C1C0600,  //  0012  CALL	R7	3
      0x60200004,  //  0013  GETGBL	R8	G4
      0x5C240E00,  //  0014  MOVE	R9	R7
      0x7C200200,  //  0015  CALL	R8	1
      0x20201106,  //  0016  NE	R8	R8	K6
      0x78220005,  //  0017  JMPF	R8	#001E
      0x94200D07,  //  0018  GETIDX	R8	R6	K7
      0x20201108,  //  0019  NE	R8	R8	K8
      0x78220002,  //  001A  JMPF	R8	#001E
      0x8C200909,  //  001B  GETMET	R8	R4	K9
      0x5C280C00,  //  001C  MOVE	R10	R6
      0x7C200400,  //  001D  CALL	R8	2
      0x7001FFED,  //  001E  JMP		#000D
      0x5814000A,  //  001F  LDCONST	R5	K10
      0xAC140200,  //  0020  CATCH	R5	1	0
      0xB0080000,  //  0021  RAISE	2	R0	R0
      0xB8161600,  //  0022  GETNGBL	R5	K11
      0x8C140B0C,  //  0023  GETMET	R5	R5	K12
      0x5C1C0800,  //  0024  MOVE	R7	R4
      0x7C140400,  //  0025  CALL	R5	2
      0x5C100A00,  //  0026  MOVE	R4	R5
      0x60140012,  //  0027  GETGBL	R5	G18
      0x7C140000,  //  0028  CALL	R5	0
      0x60180010,  //  0029  GETGBL	R6	G16
      0x5C1C0800,  //  002A  MOVE	R7	R4
      0x7C180200,  //  002B  CALL	R6	1
      0xA8020020,  //  002C  EXBLK	0	#004E
      0x5C1C0C00,  //  002D  MOVE	R7	R6
      0x7C1C0000,  //  002E  CALL	R7	0
      0x8C200705,  //  002F  GETMET	R8	R3	K5
      0x5C280000,  //  0030  MOVE	R10	R0
      0x5C2C0E00,  //  0031  MOVE	R11	R7
      0x7C200600,  //  0032  CALL	R8	3
      0x4C240000,  //  0033  LDNIL	R9
      0x1C241009,  //  0034  EQ	R9	R8	R9
      0x78260000,  //  0035  JMPF	R9	#0037
      0x7001FFF5,  //  0036  JMP		#002D
      0x6024000F,  //  0037  GETGBL	R9	G15
      0x5C281000,  //  0038  MOVE	R10	R8
      0x602C0015,  //  0039  GETGBL	R11	G21
      0x7C240400,  //  003A  CALL	R9	2
      0x78260003,  //  003B  JMPF	R9	#0040
      0x8C24110E,  //  003C  GETMET	R9	R8	K14
      0x7C240200,  //  003D  CALL	R9	1
      0x00261A09,  //  003E  ADD	R9	K13	R9
      0x5C201200,  //  003F  MOVE	R8	R9
      0x8C240B09,  //  0040  GETMET	R9	R5	K9
      0x8C2C050F,  //  0041  GETMET	R11	R2	K15
      0x58340010,  //  0042  LDCONST	R13	K16
      0x8C380311,  //  0043  GETMET	R14	R1	K17
      0x60400008,  //  0044  GETGBL	R16	G8
      0x5C440E00,  //  0045  MOVE	R17	R7
      0x7C400200,  //  0046  CALL	R16	1
      0x7C380400,  //  0047  CALL	R14	2
      0x8C3C0311,  //  0048  GETMET	R15	R1	K17
      0x5C441000,  //  0049  MOVE	R17	R8
      0x7C3C0400,  //  004A  CALL	R15	2
      0x7C2C0800,  //  004B  CALL	R11	4
      0x7C240400,  //  004C  CALL	R9	2
      0x7001FFDE,  //  004D  JMP		#002D
      0x5818000A,  //  004E  LDCONST	R6	K10
      0xAC180200,  //  004F  CATCH	R6	1	0
      0xB0080000,  //  0050  RAISE	2	R0	R0
      0x60180012,  //  0051  GETGBL	R6	G18
      0x7C180000,  //  0052  CALL	R6	0
      0x601C0010,  //  0053  GETGBL	R7	G16
      0x88200112,  //  0054  GETMBR	R8	R0	K18
      0x8C201113,  //  0055  GETMET	R8	R8	K19
      0x7C200200,  //  0056  CALL	R8	1
      0x7C1C0200,  //  0057  CALL	R7	1
      0xA8020006,  //  0058  EXBLK	0	#0060
      0x5C200E00,  //  0059  MOVE	R8	R7
      0x7C200000,  //  005A  CALL	R8	0
      0x8C240D09,  //  005B  GETMET	R9	R6	K9
      0x8C2C1114,  //  005C  GETMET	R11	R8	K20
      0x7C2C0200,  //  005D  CALL	R11	1
      0x7C240400,  //  005E  CALL	R9	2
      0x7001FFF8,  //  005F  JMP		#0059
      0x581C000A,  //  0060  LDCONST	R7	K10
      0xAC1C0200,  //  0061  CATCH	R7	1	0
      0xB0080000,  //  0062  RAISE	2	R0	R0
      0x601C000C,  //  0063  GETGBL	R7	G12
      0x5C200C00,  //  0064  MOVE	R8	R6
      0x7C1C0200,  //  0065  CALL	R7	1
      0x241C0F07,  //  0066  GT	R7	R7	K7
      0x781E0007,  //  0067  JMPF	R7	#0070
      0x8C1C0D16,  //  0068  GETMET	R7	R6	K22
      0x58240017,  //  0069  LDCONST	R9	K23
      0x7C1C0400,  //  006A  CALL	R7	2
      0x001E2A07,  //  006B  ADD	R7	K21	R7
      0x001C0F18,  //  006C  ADD	R7	R7	K24
      0x8C200B09,  //  006D  GETMET	R8	R5	K9
      0x002A3207,  //  006E  ADD	R10	K25	R7
      0x7C200400,  //  006F  CALL	R8	2
      0x8C1C011A,  //  0070  GETMET	R7	R0	K26
      0x7C1C0200,  //  0071  CALL	R7	1
      0x8C1C0B16,  //  0072  GETMET	R7	R5	K22
      0x58240017,  //  0073  LDCONST	R9	K23
      0x7C1C0400,  //  0074  CALL	R7	2
      0x001E3607,  //  0075  ADD	R7	K27	R7
      0x001C0F1C,  //  0076  ADD	R7	R7	K28
      0x80040E00,  //  0077  RET	1	R7
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: assign_fabric_index
********************************************************************/
be_local_closure(Matter_Fabric_assign_fabric_index,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(get_fabric_index),
    /* K1   */  be_nested_str_weak(set_fabric_index),
    /* K2   */  be_nested_str_weak(_store),
    /* K3   */  be_nested_str_weak(next_fabric_idx),
    }),
    be_str_weak(assign_fabric_index),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x1C040202,  //  0003  EQ	R1	R1	R2
      0x78060004,  //  0004  JMPF	R1	#000A
      0x8C040101,  //  0005  GETMET	R1	R0	K1
      0x880C0102,  //  0006  GETMBR	R3	R0	K2
      0x8C0C0703,  //  0007  GETMET	R3	R3	K3
      0x7C0C0200,  //  0008  CALL	R3	1
      0x7C040400,  //  0009  CALL	R1	2
      0x80000000,  //  000A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_admin_subject
********************************************************************/
be_local_closure(Matter_Fabric_get_admin_subject,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(admin_subject),
    }),
    be_str_weak(get_admin_subject),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_fabric_compressed
********************************************************************/
be_local_closure(Matter_Fabric_get_fabric_compressed,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(fabric_compressed),
    }),
    be_str_weak(get_fabric_compressed),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: fabric_candidate
********************************************************************/
be_local_closure(Matter_Fabric_fabric_candidate,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(set_expire_in_seconds),
    /* K1   */  be_nested_str_weak(assign_fabric_index),
    /* K2   */  be_nested_str_weak(_store),
    /* K3   */  be_nested_str_weak(add_fabric),
    }),
    be_str_weak(fabric_candidate),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x540E0077,  //  0001  LDINT	R3	120
      0x7C040400,  //  0002  CALL	R1	2
      0x8C040101,  //  0003  GETMET	R1	R0	K1
      0x7C040200,  //  0004  CALL	R1	1
      0x88040102,  //  0005  GETMBR	R1	R0	K2
      0x8C040303,  //  0006  GETMET	R1	R1	K3
      0x5C0C0000,  //  0007  MOVE	R3	R0
      0x7C040400,  //  0008  CALL	R1	2
      0x80000000,  //  0009  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_fabric_label
********************************************************************/
be_local_closure(Matter_Fabric_get_fabric_label,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(fabric_label),
    }),
    be_str_weak(get_fabric_label),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_noc_icac
********************************************************************/
be_local_closure(Matter_Fabric_set_noc_icac,   /* name */
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
    /* K0   */  be_nested_str_weak(noc),
    /* K1   */  be_nested_str_weak(icac),
    }),
    be_str_weak(set_noc_icac),
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
** Solidified function: get_newest_session
********************************************************************/
be_local_closure(Matter_Fabric_get_newest_session,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(get_old_recent_session),
    }),
    be_str_weak(get_newest_session),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x500C0000,  //  0001  LDBOOL	R3	0	0
      0x7C040400,  //  0002  CALL	R1	2
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: before_remove
********************************************************************/
be_local_closure(Matter_Fabric_before_remove,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[10]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(log),
    /* K3   */  be_nested_str_weak(format),
    /* K4   */  be_nested_str_weak(MTR_X3A_X20_X2DFabric_X20_X20_X20_X20fab_X3D_X27_X25s_X27_X20_X28removed_X29),
    /* K5   */  be_nested_str_weak(get_fabric_id),
    /* K6   */  be_nested_str_weak(copy),
    /* K7   */  be_nested_str_weak(reverse),
    /* K8   */  be_nested_str_weak(tohex),
    /* K9   */  be_const_int(3),
    }),
    be_str_weak(before_remove),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xB80A0200,  //  0001  GETNGBL	R2	K1
      0x8C080502,  //  0002  GETMET	R2	R2	K2
      0x8C100303,  //  0003  GETMET	R4	R1	K3
      0x58180004,  //  0004  LDCONST	R6	K4
      0x8C1C0105,  //  0005  GETMET	R7	R0	K5
      0x7C1C0200,  //  0006  CALL	R7	1
      0x8C1C0F06,  //  0007  GETMET	R7	R7	K6
      0x7C1C0200,  //  0008  CALL	R7	1
      0x8C1C0F07,  //  0009  GETMET	R7	R7	K7
      0x7C1C0200,  //  000A  CALL	R7	1
      0x8C1C0F08,  //  000B  GETMET	R7	R7	K8
      0x7C1C0200,  //  000C  CALL	R7	1
      0x7C100600,  //  000D  CALL	R4	3
      0x58140009,  //  000E  LDCONST	R5	K9
      0x7C080600,  //  000F  CALL	R2	3
      0x80000000,  //  0010  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_fabric_device
********************************************************************/
be_local_closure(Matter_Fabric_set_fabric_device,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    5,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(fabric_id),
    /* K1   */  be_nested_str_weak(device_id),
    /* K2   */  be_nested_str_weak(fabric_compressed),
    /* K3   */  be_nested_str_weak(fabric_parent),
    /* K4   */  be_nested_str_weak(get_fabric_index),
    }),
    be_str_weak(set_fabric_device),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x90020202,  //  0001  SETMBR	R0	K1	R2
      0x90020403,  //  0002  SETMBR	R0	K2	R3
      0x4C140000,  //  0003  LDNIL	R5
      0x20140805,  //  0004  NE	R5	R4	R5
      0x78160002,  //  0005  JMPF	R5	#0009
      0x8C140904,  //  0006  GETMET	R5	R4	K4
      0x7C140200,  //  0007  CALL	R5	1
      0x70020000,  //  0008  JMP		#000A
      0x4C140000,  //  0009  LDNIL	R5
      0x90020605,  //  000A  SETMBR	R0	K3	R5
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_ipk_epoch_key
********************************************************************/
be_local_closure(Matter_Fabric_set_ipk_epoch_key,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(ipk_epoch_key),
    }),
    be_str_weak(set_ipk_epoch_key),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x80000000,  //  0001  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_ipk_group_key
********************************************************************/
be_local_closure(Matter_Fabric_get_ipk_group_key,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(ipk_epoch_key),
    /* K1   */  be_nested_str_weak(fabric_compressed),
    /* K2   */  be_nested_str_weak(crypto),
    /* K3   */  be_nested_str_weak(HKDF_SHA256),
    /* K4   */  be_nested_str_weak(fromstring),
    /* K5   */  be_nested_str_weak(_GROUP_KEY),
    /* K6   */  be_nested_str_weak(derive),
    }),
    be_str_weak(get_ipk_group_key),
    &be_const_str_solidified,
    ( &(const binstruction[25]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x4C080000,  //  0001  LDNIL	R2
      0x1C040202,  //  0002  EQ	R1	R1	R2
      0x74060003,  //  0003  JMPT	R1	#0008
      0x88040101,  //  0004  GETMBR	R1	R0	K1
      0x4C080000,  //  0005  LDNIL	R2
      0x1C040202,  //  0006  EQ	R1	R1	R2
      0x78060001,  //  0007  JMPF	R1	#000A
      0x4C040000,  //  0008  LDNIL	R1
      0x80040200,  //  0009  RET	1	R1
      0xA4060400,  //  000A  IMPORT	R1	K2
      0x8C080303,  //  000B  GETMET	R2	R1	K3
      0x7C080200,  //  000C  CALL	R2	1
      0x600C0015,  //  000D  GETGBL	R3	G21
      0x7C0C0000,  //  000E  CALL	R3	0
      0x8C0C0704,  //  000F  GETMET	R3	R3	K4
      0x88140105,  //  0010  GETMBR	R5	R0	K5
      0x7C0C0400,  //  0011  CALL	R3	2
      0x8C100506,  //  0012  GETMET	R4	R2	K6
      0x88180100,  //  0013  GETMBR	R6	R0	K0
      0x881C0101,  //  0014  GETMBR	R7	R0	K1
      0x5C200600,  //  0015  MOVE	R8	R3
      0x5426000F,  //  0016  LDINT	R9	16
      0x7C100A00,  //  0017  CALL	R4	5
      0x80040800,  //  0018  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: hydrate_post
********************************************************************/
be_local_closure(Matter_Fabric_hydrate_post,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(_counter_group_data_snd_impl),
    /* K1   */  be_nested_str_weak(reset),
    /* K2   */  be_nested_str_weak(counter_group_data_snd),
    /* K3   */  be_nested_str_weak(_counter_group_ctrl_snd_impl),
    /* K4   */  be_nested_str_weak(counter_group_ctrl_snd),
    /* K5   */  be_nested_str_weak(val),
    }),
    be_str_weak(hydrate_post),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x880C0102,  //  0002  GETMBR	R3	R0	K2
      0x7C040400,  //  0003  CALL	R1	2
      0x88040103,  //  0004  GETMBR	R1	R0	K3
      0x8C040301,  //  0005  GETMET	R1	R1	K1
      0x880C0104,  //  0006  GETMBR	R3	R0	K4
      0x7C040400,  //  0007  CALL	R1	2
      0x88040100,  //  0008  GETMBR	R1	R0	K0
      0x8C040305,  //  0009  GETMET	R1	R1	K5
      0x7C040200,  //  000A  CALL	R1	1
      0x90020401,  //  000B  SETMBR	R0	K2	R1
      0x88040103,  //  000C  GETMBR	R1	R0	K3
      0x8C040305,  //  000D  GETMET	R1	R1	K5
      0x7C040200,  //  000E  CALL	R1	1
      0x90020801,  //  000F  SETMBR	R0	K4	R1
      0x80000000,  //  0010  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_admin_vendor_name
********************************************************************/
be_local_closure(Matter_Fabric_get_admin_vendor_name,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(admin_vendor),
    /* K1   */  be_nested_str_weak(),
    /* K2   */  be_nested_str_weak(matter),
    /* K3   */  be_nested_str_weak(get_vendor_name),
    /* K4   */  be_nested_str_weak(string),
    /* K5   */  be_nested_str_weak(format),
    /* K6   */  be_nested_str_weak(0x_X2504X),
    }),
    be_str_weak(get_admin_vendor_name),
    &be_const_str_solidified,
    ( &(const binstruction[21]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x4C080000,  //  0001  LDNIL	R2
      0x1C080202,  //  0002  EQ	R2	R1	R2
      0x780A0000,  //  0003  JMPF	R2	#0005
      0x80060200,  //  0004  RET	1	K1
      0xB80A0400,  //  0005  GETNGBL	R2	K2
      0x8C080503,  //  0006  GETMET	R2	R2	K3
      0x5C100200,  //  0007  MOVE	R4	R1
      0x7C080400,  //  0008  CALL	R2	2
      0x4C0C0000,  //  0009  LDNIL	R3
      0x200C0403,  //  000A  NE	R3	R2	R3
      0x780E0001,  //  000B  JMPF	R3	#000E
      0x80040400,  //  000C  RET	1	R2
      0x70020005,  //  000D  JMP		#0014
      0xA40E0800,  //  000E  IMPORT	R3	K4
      0x8C100705,  //  000F  GETMET	R4	R3	K5
      0x58180006,  //  0010  LDCONST	R6	K6
      0x5C1C0200,  //  0011  MOVE	R7	R1
      0x7C100600,  //  0012  CALL	R4	3
      0x80040800,  //  0013  RET	1	R4
      0x80000000,  //  0014  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_pk
********************************************************************/
be_local_closure(Matter_Fabric_set_pk,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(no_private_key),
    }),
    be_str_weak(set_pk),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x80000000,  //  0001  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_icac
********************************************************************/
be_local_closure(Matter_Fabric_get_icac,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(icac),
    }),
    be_str_weak(get_icac),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_admin_subject_vendor
********************************************************************/
be_local_closure(Matter_Fabric_set_admin_subject_vendor,   /* name */
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
    /* K0   */  be_nested_str_weak(admin_subject),
    /* K1   */  be_nested_str_weak(admin_vendor),
    }),
    be_str_weak(set_admin_subject_vendor),
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
** Solidified function: log_new_fabric
********************************************************************/
be_local_closure(Matter_Fabric_log_new_fabric,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[11]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(log),
    /* K3   */  be_nested_str_weak(format),
    /* K4   */  be_nested_str_weak(MTR_X3A_X20_X2BFabric_X20_X20_X20_X20fab_X3D_X27_X25s_X27_X20vendorid_X3D_X25s),
    /* K5   */  be_nested_str_weak(get_fabric_id),
    /* K6   */  be_nested_str_weak(copy),
    /* K7   */  be_nested_str_weak(reverse),
    /* K8   */  be_nested_str_weak(tohex),
    /* K9   */  be_nested_str_weak(get_admin_vendor_name),
    /* K10  */  be_const_int(3),
    }),
    be_str_weak(log_new_fabric),
    &be_const_str_solidified,
    ( &(const binstruction[19]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xB80A0200,  //  0001  GETNGBL	R2	K1
      0x8C080502,  //  0002  GETMET	R2	R2	K2
      0x8C100303,  //  0003  GETMET	R4	R1	K3
      0x58180004,  //  0004  LDCONST	R6	K4
      0x8C1C0105,  //  0005  GETMET	R7	R0	K5
      0x7C1C0200,  //  0006  CALL	R7	1
      0x8C1C0F06,  //  0007  GETMET	R7	R7	K6
      0x7C1C0200,  //  0008  CALL	R7	1
      0x8C1C0F07,  //  0009  GETMET	R7	R7	K7
      0x7C1C0200,  //  000A  CALL	R7	1
      0x8C1C0F08,  //  000B  GETMET	R7	R7	K8
      0x7C1C0200,  //  000C  CALL	R7	1
      0x8C200109,  //  000D  GETMET	R8	R0	K9
      0x7C200200,  //  000E  CALL	R8	1
      0x7C100800,  //  000F  CALL	R4	4
      0x5814000A,  //  0010  LDCONST	R5	K10
      0x7C080600,  //  0011  CALL	R2	3
      0x80000000,  //  0012  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_pk
********************************************************************/
be_local_closure(Matter_Fabric_get_pk,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(no_private_key),
    }),
    be_str_weak(get_pk),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_admin_vendor
********************************************************************/
be_local_closure(Matter_Fabric_get_admin_vendor,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(admin_vendor),
    }),
    be_str_weak(get_admin_vendor),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_fabric_id
********************************************************************/
be_local_closure(Matter_Fabric_get_fabric_id,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(fabric_id),
    }),
    be_str_weak(get_fabric_id),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_ca
********************************************************************/
be_local_closure(Matter_Fabric_set_ca,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(root_ca_certificate),
    }),
    be_str_weak(set_ca),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x80000000,  //  0001  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_noc
********************************************************************/
be_local_closure(Matter_Fabric_get_noc,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(noc),
    }),
    be_str_weak(get_noc),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_fabric_index
********************************************************************/
be_local_closure(Matter_Fabric_get_fabric_index,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(fabric_index),
    }),
    be_str_weak(get_fabric_index),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_ca
********************************************************************/
be_local_closure(Matter_Fabric_get_ca,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(root_ca_certificate),
    }),
    be_str_weak(get_ca),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_old_recent_session
********************************************************************/
be_local_closure(Matter_Fabric_get_old_recent_session,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(_sessions),
    /* K1   */  be_const_int(0),
    /* K2   */  be_nested_str_weak(last_used),
    /* K3   */  be_const_int(1),
    }),
    be_str_weak(get_old_recent_session),
    &be_const_str_solidified,
    ( &(const binstruction[30]) {  /* code */
      0x6008000C,  //  0000  GETGBL	R2	G12
      0x880C0100,  //  0001  GETMBR	R3	R0	K0
      0x7C080200,  //  0002  CALL	R2	1
      0x1C080501,  //  0003  EQ	R2	R2	K1
      0x780A0001,  //  0004  JMPF	R2	#0007
      0x4C080000,  //  0005  LDNIL	R2
      0x80040400,  //  0006  RET	1	R2
      0x88080100,  //  0007  GETMBR	R2	R0	K0
      0x94080501,  //  0008  GETIDX	R2	R2	K1
      0x880C0502,  //  0009  GETMBR	R3	R2	K2
      0x58100003,  //  000A  LDCONST	R4	K3
      0x6014000C,  //  000B  GETGBL	R5	G12
      0x88180100,  //  000C  GETMBR	R6	R0	K0
      0x7C140200,  //  000D  CALL	R5	1
      0x14140805,  //  000E  LT	R5	R4	R5
      0x7816000C,  //  000F  JMPF	R5	#001D
      0x88140100,  //  0010  GETMBR	R5	R0	K0
      0x94140A04,  //  0011  GETIDX	R5	R5	R4
      0x88140B02,  //  0012  GETMBR	R5	R5	K2
      0x78060001,  //  0013  JMPF	R1	#0016
      0x14180A03,  //  0014  LT	R6	R5	R3
      0x70020000,  //  0015  JMP		#0017
      0x24180A03,  //  0016  GT	R6	R5	R3
      0x781A0002,  //  0017  JMPF	R6	#001B
      0x88180100,  //  0018  GETMBR	R6	R0	K0
      0x94080C04,  //  0019  GETIDX	R2	R6	R4
      0x5C0C0A00,  //  001A  MOVE	R3	R5
      0x00100903,  //  001B  ADD	R4	R4	K3
      0x7001FFED,  //  001C  JMP		#000B
      0x80040400,  //  001D  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Fabric
********************************************************************/
extern const bclass be_class_Matter_Expirable;
be_local_class(Matter_Fabric,
    20,
    &be_class_Matter_Expirable,
    be_nested_map(60,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(fabric_index, -1), be_const_var(2) },
        { be_const_key_weak(fromjson, -1), be_const_static_closure(Matter_Fabric_fromjson_closure) },
        { be_const_key_weak(root_ca_certificate, -1), be_const_var(6) },
        { be_const_key_weak(ipk_epoch_key, -1), be_const_var(9) },
        { be_const_key_weak(get_ca, 34), be_const_closure(Matter_Fabric_get_ca_closure) },
        { be_const_key_weak(noc, -1), be_const_var(7) },
        { be_const_key_weak(get_device_id, -1), be_const_closure(Matter_Fabric_get_device_id_closure) },
        { be_const_key_weak(add_session, -1), be_const_closure(Matter_Fabric_add_session_closure) },
        { be_const_key_weak(get_oldest_session, 50), be_const_closure(Matter_Fabric_get_oldest_session_closure) },
        { be_const_key_weak(fabric_parent, -1), be_const_var(3) },
        { be_const_key_weak(no_private_key, -1), be_const_var(5) },
        { be_const_key_weak(admin_vendor, 1), be_const_var(19) },
        { be_const_key_weak(fabric_id, -1), be_const_var(10) },
        { be_const_key_weak(device_id, -1), be_const_var(12) },
        { be_const_key_weak(set_fabric_index, -1), be_const_closure(Matter_Fabric_set_fabric_index_closure) },
        { be_const_key_weak(init, -1), be_const_closure(Matter_Fabric_init_closure) },
        { be_const_key_weak(counter_group_ctrl_snd, -1), be_const_var(15) },
        { be_const_key_weak(fabric_label, -1), be_const_var(13) },
        { be_const_key_weak(fabric_completed, -1), be_const_closure(Matter_Fabric_fabric_completed_closure) },
        { be_const_key_weak(created, -1), be_const_var(1) },
        { be_const_key_weak(fabric_compressed, -1), be_const_var(11) },
        { be_const_key_weak(icac, -1), be_const_var(8) },
        { be_const_key_weak(_counter_group_data_snd_impl, 2), be_const_var(16) },
        { be_const_key_weak(_store, 57), be_const_var(0) },
        { be_const_key_weak(get_ca_pub, 32), be_const_closure(Matter_Fabric_get_ca_pub_closure) },
        { be_const_key_weak(_GROUP_KEY, 16), be_nested_str_weak(GroupKey_X20v1_X2E0) },
        { be_const_key_weak(tojson, -1), be_const_closure(Matter_Fabric_tojson_closure) },
        { be_const_key_weak(assign_fabric_index, -1), be_const_closure(Matter_Fabric_assign_fabric_index_closure) },
        { be_const_key_weak(get_admin_subject, -1), be_const_closure(Matter_Fabric_get_admin_subject_closure) },
        { be_const_key_weak(_MAX_CASE, -1), be_const_int(5) },
        { be_const_key_weak(fabric_candidate, -1), be_const_closure(Matter_Fabric_fabric_candidate_closure) },
        { be_const_key_weak(_GROUP_SND_INCR, 36), be_const_int(32) },
        { be_const_key_weak(get_fabric_label, 55), be_const_closure(Matter_Fabric_get_fabric_label_closure) },
        { be_const_key_weak(set_noc_icac, -1), be_const_closure(Matter_Fabric_set_noc_icac_closure) },
        { be_const_key_weak(get_noc, 52), be_const_closure(Matter_Fabric_get_noc_closure) },
        { be_const_key_weak(before_remove, -1), be_const_closure(Matter_Fabric_before_remove_closure) },
        { be_const_key_weak(get_admin_vendor_name, -1), be_const_closure(Matter_Fabric_get_admin_vendor_name_closure) },
        { be_const_key_weak(set_ca, -1), be_const_closure(Matter_Fabric_set_ca_closure) },
        { be_const_key_weak(get_ipk_group_key, -1), be_const_closure(Matter_Fabric_get_ipk_group_key_closure) },
        { be_const_key_weak(hydrate_post, -1), be_const_closure(Matter_Fabric_hydrate_post_closure) },
        { be_const_key_weak(set_ipk_epoch_key, 31), be_const_closure(Matter_Fabric_set_ipk_epoch_key_closure) },
        { be_const_key_weak(set_pk, -1), be_const_closure(Matter_Fabric_set_pk_closure) },
        { be_const_key_weak(set_admin_subject_vendor, -1), be_const_closure(Matter_Fabric_set_admin_subject_vendor_closure) },
        { be_const_key_weak(get_fabric_compressed, 42), be_const_closure(Matter_Fabric_get_fabric_compressed_closure) },
        { be_const_key_weak(log_new_fabric, -1), be_const_closure(Matter_Fabric_log_new_fabric_closure) },
        { be_const_key_weak(get_pk, -1), be_const_closure(Matter_Fabric_get_pk_closure) },
        { be_const_key_weak(get_admin_vendor, -1), be_const_closure(Matter_Fabric_get_admin_vendor_closure) },
        { be_const_key_weak(get_fabric_id, -1), be_const_closure(Matter_Fabric_get_fabric_id_closure) },
        { be_const_key_weak(get_icac, 37), be_const_closure(Matter_Fabric_get_icac_closure) },
        { be_const_key_weak(_counter_group_ctrl_snd_impl, -1), be_const_var(17) },
        { be_const_key_weak(set_fabric_device, -1), be_const_closure(Matter_Fabric_set_fabric_device_closure) },
        { be_const_key_weak(admin_subject, 5), be_const_var(18) },
        { be_const_key_weak(counter_group_data_snd, -1), be_const_var(14) },
        { be_const_key_weak(get_newest_session, 29), be_const_closure(Matter_Fabric_get_newest_session_closure) },
        { be_const_key_weak(get_fabric_index, -1), be_const_closure(Matter_Fabric_get_fabric_index_closure) },
        { be_const_key_weak(get_ipk_epoch_key, -1), be_const_closure(Matter_Fabric_get_ipk_epoch_key_closure) },
        { be_const_key_weak(counter_group_data_snd_next, 4), be_const_closure(Matter_Fabric_counter_group_data_snd_next_closure) },
        { be_const_key_weak(counter_group_ctrl_snd_next, -1), be_const_closure(Matter_Fabric_counter_group_ctrl_snd_next_closure) },
        { be_const_key_weak(get_old_recent_session, -1), be_const_closure(Matter_Fabric_get_old_recent_session_closure) },
        { be_const_key_weak(_sessions, 0), be_const_var(4) },
    })),
    be_str_weak(Matter_Fabric)
);
/*******************************************************************/

void be_load_Matter_Fabric_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Fabric);
    be_setglobal(vm, "Matter_Fabric");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
