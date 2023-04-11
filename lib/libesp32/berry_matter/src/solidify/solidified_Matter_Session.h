/* Solidification of Matter_Session.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Fabric;

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
** Solidified function: log_new_fabric
********************************************************************/
be_local_closure(Matter_Fabric_log_new_fabric,   /* name */
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
    /* K4   */  be_nested_str_weak(MTR_X3A_X20_X2BFabric_X20_X20_X20_X20fab_X3D_X27_X25s_X27),
    /* K5   */  be_nested_str_weak(get_fabric_id),
    /* K6   */  be_nested_str_weak(copy),
    /* K7   */  be_nested_str_weak(reverse),
    /* K8   */  be_nested_str_weak(tohex),
    /* K9   */  be_const_int(2),
    }),
    be_str_weak(log_new_fabric),
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
    /* K9   */  be_const_int(2),
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
** Solidified class: Matter_Fabric
********************************************************************/
extern const bclass be_class_Matter_Expirable;
be_local_class(Matter_Fabric,
    20,
    &be_class_Matter_Expirable,
    be_nested_map(49,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(no_private_key, -1), be_const_var(5) },
        { be_const_key_weak(tojson, 8), be_const_closure(Matter_Fabric_tojson_closure) },
        { be_const_key_weak(counter_group_data_snd, -1), be_const_var(14) },
        { be_const_key_weak(fabric_label, -1), be_const_var(13) },
        { be_const_key_weak(_sessions, 34), be_const_var(4) },
        { be_const_key_weak(created, -1), be_const_var(1) },
        { be_const_key_weak(_GROUP_SND_INCR, -1), be_const_int(32) },
        { be_const_key_weak(get_admin_subject, -1), be_const_closure(Matter_Fabric_get_admin_subject_closure) },
        { be_const_key_weak(add_session, -1), be_const_closure(Matter_Fabric_add_session_closure) },
        { be_const_key_weak(get_old_recent_session, -1), be_const_closure(Matter_Fabric_get_old_recent_session_closure) },
        { be_const_key_weak(counter_group_data_snd_next, -1), be_const_closure(Matter_Fabric_counter_group_data_snd_next_closure) },
        { be_const_key_weak(ipk_epoch_key, -1), be_const_var(9) },
        { be_const_key_weak(get_device_id, -1), be_const_closure(Matter_Fabric_get_device_id_closure) },
        { be_const_key_weak(counter_group_ctrl_snd_next, -1), be_const_closure(Matter_Fabric_counter_group_ctrl_snd_next_closure) },
        { be_const_key_weak(_counter_group_ctrl_snd_impl, -1), be_const_var(17) },
        { be_const_key_weak(admin_subject, 3), be_const_var(18) },
        { be_const_key_weak(set_fabric_index, -1), be_const_closure(Matter_Fabric_set_fabric_index_closure) },
        { be_const_key_weak(get_fabric_compressed, 32), be_const_closure(Matter_Fabric_get_fabric_compressed_closure) },
        { be_const_key_weak(fabric_index, -1), be_const_var(2) },
        { be_const_key_weak(counter_group_ctrl_snd, 21), be_const_var(15) },
        { be_const_key_weak(log_new_fabric, -1), be_const_closure(Matter_Fabric_log_new_fabric_closure) },
        { be_const_key_weak(init, -1), be_const_closure(Matter_Fabric_init_closure) },
        { be_const_key_weak(get_fabric_id, -1), be_const_closure(Matter_Fabric_get_fabric_id_closure) },
        { be_const_key_weak(_store, 15), be_const_var(0) },
        { be_const_key_weak(admin_vendor, 20), be_const_var(19) },
        { be_const_key_weak(device_id, 40), be_const_var(12) },
        { be_const_key_weak(before_remove, 28), be_const_closure(Matter_Fabric_before_remove_closure) },
        { be_const_key_weak(fromjson, -1), be_const_static_closure(Matter_Fabric_fromjson_closure) },
        { be_const_key_weak(get_noc, 39), be_const_closure(Matter_Fabric_get_noc_closure) },
        { be_const_key_weak(get_fabric_index, 38), be_const_closure(Matter_Fabric_get_fabric_index_closure) },
        { be_const_key_weak(_MAX_CASE, -1), be_const_int(5) },
        { be_const_key_weak(get_ipk_epoch_key, -1), be_const_closure(Matter_Fabric_get_ipk_epoch_key_closure) },
        { be_const_key_weak(_GROUP_KEY, -1), be_nested_str_weak(GroupKey_X20v1_X2E0) },
        { be_const_key_weak(_counter_group_data_snd_impl, -1), be_const_var(16) },
        { be_const_key_weak(fabric_parent, -1), be_const_var(3) },
        { be_const_key_weak(fabric_id, -1), be_const_var(10) },
        { be_const_key_weak(icac, 35), be_const_var(8) },
        { be_const_key_weak(get_ca, -1), be_const_closure(Matter_Fabric_get_ca_closure) },
        { be_const_key_weak(get_ipk_group_key, 6), be_const_closure(Matter_Fabric_get_ipk_group_key_closure) },
        { be_const_key_weak(get_fabric_label, -1), be_const_closure(Matter_Fabric_get_fabric_label_closure) },
        { be_const_key_weak(get_oldest_session, -1), be_const_closure(Matter_Fabric_get_oldest_session_closure) },
        { be_const_key_weak(get_newest_session, 22), be_const_closure(Matter_Fabric_get_newest_session_closure) },
        { be_const_key_weak(get_admin_vendor, -1), be_const_closure(Matter_Fabric_get_admin_vendor_closure) },
        { be_const_key_weak(noc, -1), be_const_var(7) },
        { be_const_key_weak(hydrate_post, -1), be_const_closure(Matter_Fabric_hydrate_post_closure) },
        { be_const_key_weak(get_icac, 10), be_const_closure(Matter_Fabric_get_icac_closure) },
        { be_const_key_weak(fabric_compressed, -1), be_const_var(11) },
        { be_const_key_weak(get_ca_pub, -1), be_const_closure(Matter_Fabric_get_ca_pub_closure) },
        { be_const_key_weak(root_ca_certificate, -1), be_const_var(6) },
    })),
    be_str_weak(Matter_Fabric)
);
/*******************************************************************/

void be_load_Matter_Fabric_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Fabric);
    be_setglobal(vm, "Matter_Fabric");
    be_pop(vm, 1);
}

extern const bclass be_class_Matter_Session;

/********************************************************************
** Solidified function: before_remove
********************************************************************/
be_local_closure(Matter_Session_before_remove,   /* name */
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
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(log),
    /* K3   */  be_nested_str_weak(format),
    /* K4   */  be_nested_str_weak(MTR_X3A_X20_X2DSession_X20_X20_X20_X28_X256i_X29_X20_X28removed_X29),
    /* K5   */  be_nested_str_weak(local_session_id),
    /* K6   */  be_const_int(3),
    }),
    be_str_weak(before_remove),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xB80A0200,  //  0001  GETNGBL	R2	K1
      0x8C080502,  //  0002  GETMET	R2	R2	K2
      0x8C100303,  //  0003  GETMET	R4	R1	K3
      0x58180004,  //  0004  LDCONST	R6	K4
      0x881C0105,  //  0005  GETMBR	R7	R0	K5
      0x7C100600,  //  0006  CALL	R4	3
      0x58140006,  //  0007  LDCONST	R5	K6
      0x7C080600,  //  0008  CALL	R2	3
      0x80000000,  //  0009  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_fabric
********************************************************************/
be_local_closure(Matter_Session_get_fabric,   /* name */
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
    /* K0   */  be_nested_str_weak(_fabric),
    }),
    be_str_weak(get_fabric),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: persist_to_fabric
********************************************************************/
be_local_closure(Matter_Session_persist_to_fabric,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(_fabric),
    /* K1   */  be_nested_str_weak(add_session),
    }),
    be_str_weak(persist_to_fabric),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: counter_snd_next
********************************************************************/
be_local_closure(Matter_Session_counter_snd_next,   /* name */
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
    /* K1   */  be_nested_str_weak(_counter_snd_impl),
    /* K2   */  be_nested_str_weak(next),
    /* K3   */  be_nested_str_weak(tasmota),
    /* K4   */  be_nested_str_weak(log),
    /* K5   */  be_nested_str_weak(format),
    /* K6   */  be_nested_str_weak(MTR_X3A_X20_X2E_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20Counter_snd_X3D_X25i),
    /* K7   */  be_const_int(3),
    /* K8   */  be_nested_str_weak(matter),
    /* K9   */  be_nested_str_weak(Counter),
    /* K10  */  be_nested_str_weak(is_greater),
    /* K11  */  be_nested_str_weak(counter_snd),
    /* K12  */  be_nested_str_weak(_COUNTER_SND_INCR),
    /* K13  */  be_nested_str_weak(does_persist),
    /* K14  */  be_nested_str_weak(save),
    }),
    be_str_weak(counter_snd_next),
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
** Solidified function: get_ca
********************************************************************/
be_local_closure(Matter_Session_get_ca,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(_fabric),
    /* K1   */  be_nested_str_weak(root_ca_certificate),
    }),
    be_str_weak(get_ca),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x88040301,  //  0001  GETMBR	R1	R1	K1
      0x80040200,  //  0002  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_mode
********************************************************************/
be_local_closure(Matter_Session_get_mode,   /* name */
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
    /* K0   */  be_nested_str_weak(mode),
    }),
    be_str_weak(get_mode),
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
be_local_closure(Matter_Session_tojson,   /* name */
  be_nested_proto(
    18,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[24]) {     /* constants */
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
    /* K15  */  be_nested_str_weak(instance),
    /* K16  */  be_nested_str_weak(format),
    /* K17  */  be_nested_str_weak(_X25s_X3A_X25s),
    /* K18  */  be_nested_str_weak(dump),
    /* K19  */  be_nested_str_weak(persist_post),
    /* K20  */  be_nested_str_weak(_X7B),
    /* K21  */  be_nested_str_weak(concat),
    /* K22  */  be_nested_str_weak(_X2C),
    /* K23  */  be_nested_str_weak(_X7D),
    }),
    be_str_weak(tojson),
    &be_const_str_solidified,
    ( &(const binstruction[96]) {  /* code */
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
      0xA8020027,  //  002C  EXBLK	0	#0055
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
      0x78260004,  //  003B  JMPF	R9	#0041
      0x8C24110E,  //  003C  GETMET	R9	R8	K14
      0x7C240200,  //  003D  CALL	R9	1
      0x00261A09,  //  003E  ADD	R9	K13	R9
      0x5C201200,  //  003F  MOVE	R8	R9
      0x70020005,  //  0040  JMP		#0047
      0x60240004,  //  0041  GETGBL	R9	G4
      0x5C281000,  //  0042  MOVE	R10	R8
      0x7C240200,  //  0043  CALL	R9	1
      0x1C24130F,  //  0044  EQ	R9	R9	K15
      0x78260000,  //  0045  JMPF	R9	#0047
      0x7001FFE5,  //  0046  JMP		#002D
      0x8C240B09,  //  0047  GETMET	R9	R5	K9
      0x8C2C0510,  //  0048  GETMET	R11	R2	K16
      0x58340011,  //  0049  LDCONST	R13	K17
      0x8C380312,  //  004A  GETMET	R14	R1	K18
      0x60400008,  //  004B  GETGBL	R16	G8
      0x5C440E00,  //  004C  MOVE	R17	R7
      0x7C400200,  //  004D  CALL	R16	1
      0x7C380400,  //  004E  CALL	R14	2
      0x8C3C0312,  //  004F  GETMET	R15	R1	K18
      0x5C441000,  //  0050  MOVE	R17	R8
      0x7C3C0400,  //  0051  CALL	R15	2
      0x7C2C0800,  //  0052  CALL	R11	4
      0x7C240400,  //  0053  CALL	R9	2
      0x7001FFD7,  //  0054  JMP		#002D
      0x5818000A,  //  0055  LDCONST	R6	K10
      0xAC180200,  //  0056  CATCH	R6	1	0
      0xB0080000,  //  0057  RAISE	2	R0	R0
      0x8C180113,  //  0058  GETMET	R6	R0	K19
      0x7C180200,  //  0059  CALL	R6	1
      0x8C180B15,  //  005A  GETMET	R6	R5	K21
      0x58200016,  //  005B  LDCONST	R8	K22
      0x7C180400,  //  005C  CALL	R6	2
      0x001A2806,  //  005D  ADD	R6	K20	R6
      0x00180D17,  //  005E  ADD	R6	R6	K23
      0x80040C00,  //  005F  RET	1	R6
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: assign_fabric_index
********************************************************************/
be_local_closure(Matter_Session_assign_fabric_index,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(_fabric),
    /* K1   */  be_nested_str_weak(get_fabric_index),
    /* K2   */  be_nested_str_weak(set_fabric_index),
    /* K3   */  be_nested_str_weak(_store),
    /* K4   */  be_nested_str_weak(next_fabric_idx),
    }),
    be_str_weak(assign_fabric_index),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x7C040200,  //  0002  CALL	R1	1
      0x4C080000,  //  0003  LDNIL	R2
      0x1C040202,  //  0004  EQ	R1	R1	R2
      0x78060005,  //  0005  JMPF	R1	#000C
      0x88040100,  //  0006  GETMBR	R1	R0	K0
      0x8C040302,  //  0007  GETMET	R1	R1	K2
      0x880C0103,  //  0008  GETMBR	R3	R0	K3
      0x8C0C0704,  //  0009  GETMET	R3	R3	K4
      0x7C0C0200,  //  000A  CALL	R3	1
      0x7C040400,  //  000B  CALL	R1	2
      0x80000000,  //  000C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: gen_CSR
********************************************************************/
be_local_closure(Matter_Session_gen_CSR,   /* name */
  be_nested_proto(
    15,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[12]) {     /* constants */
    /* K0   */  be_nested_str_weak(get_pk),
    /* K1   */  be_nested_str_weak(crypto),
    /* K2   */  be_nested_str_weak(EC_P256),
    /* K3   */  be_nested_str_weak(public_key),
    /* K4   */  be_nested_str_weak(3070020100300E310C300A060355040A0C034353523059301306072A8648CE3D020106082A8648CE3D030107034200),
    /* K5   */  be_nested_str_weak(A000),
    /* K6   */  be_nested_str_weak(300C06082A8648CE3D0403020500),
    /* K7   */  be_nested_str_weak(ecdsa_sign_sha256_asn1),
    /* K8   */  be_nested_str_weak(add),
    /* K9   */  be_const_int(3),
    /* K10  */  be_const_int(1),
    /* K11  */  be_const_int(0),
    }),
    be_str_weak(gen_CSR),
    &be_const_str_solidified,
    ( &(const binstruction[73]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0xA40A0200,  //  0002  IMPORT	R2	K1
      0x8C0C0502,  //  0003  GETMET	R3	R2	K2
      0x7C0C0200,  //  0004  CALL	R3	1
      0x8C100703,  //  0005  GETMET	R4	R3	K3
      0x5C180200,  //  0006  MOVE	R6	R1
      0x7C100400,  //  0007  CALL	R4	2
      0x60140015,  //  0008  GETGBL	R5	G21
      0x58180004,  //  0009  LDCONST	R6	K4
      0x7C140200,  //  000A  CALL	R5	1
      0x40180A04,  //  000B  CONNECT	R6	R5	R4
      0x60180015,  //  000C  GETGBL	R6	G21
      0x581C0005,  //  000D  LDCONST	R7	K5
      0x7C180200,  //  000E  CALL	R6	1
      0x40180A06,  //  000F  CONNECT	R6	R5	R6
      0x60180015,  //  0010  GETGBL	R6	G21
      0x581C0006,  //  0011  LDCONST	R7	K6
      0x7C180200,  //  0012  CALL	R6	1
      0x8C1C0707,  //  0013  GETMET	R7	R3	K7
      0x5C240200,  //  0014  MOVE	R9	R1
      0x5C280A00,  //  0015  MOVE	R10	R5
      0x7C1C0600,  //  0016  CALL	R7	3
      0x60200015,  //  0017  GETGBL	R8	G21
      0x5426007F,  //  0018  LDINT	R9	128
      0x7C200200,  //  0019  CALL	R8	1
      0x8C241108,  //  001A  GETMET	R9	R8	K8
      0x582C0009,  //  001B  LDCONST	R11	K9
      0x5830000A,  //  001C  LDCONST	R12	K10
      0x7C240600,  //  001D  CALL	R9	3
      0x8C241108,  //  001E  GETMET	R9	R8	K8
      0x602C000C,  //  001F  GETGBL	R11	G12
      0x5C300E00,  //  0020  MOVE	R12	R7
      0x7C2C0200,  //  0021  CALL	R11	1
      0x002C170A,  //  0022  ADD	R11	R11	K10
      0x5830000A,  //  0023  LDCONST	R12	K10
      0x7C240600,  //  0024  CALL	R9	3
      0x8C241108,  //  0025  GETMET	R9	R8	K8
      0x582C000B,  //  0026  LDCONST	R11	K11
      0x5830000A,  //  0027  LDCONST	R12	K10
      0x7C240600,  //  0028  CALL	R9	3
      0x40241007,  //  0029  CONNECT	R9	R8	R7
      0x6024000C,  //  002A  GETGBL	R9	G12
      0x5C280A00,  //  002B  MOVE	R10	R5
      0x7C240200,  //  002C  CALL	R9	1
      0x6028000C,  //  002D  GETGBL	R10	G12
      0x5C2C0C00,  //  002E  MOVE	R11	R6
      0x7C280200,  //  002F  CALL	R10	1
      0x0024120A,  //  0030  ADD	R9	R9	R10
      0x6028000C,  //  0031  GETGBL	R10	G12
      0x5C2C1000,  //  0032  MOVE	R11	R8
      0x7C280200,  //  0033  CALL	R10	1
      0x0024120A,  //  0034  ADD	R9	R9	R10
      0x60280015,  //  0035  GETGBL	R10	G21
      0x542E00CF,  //  0036  LDINT	R11	208
      0x7C280200,  //  0037  CALL	R10	1
      0x8C2C1508,  //  0038  GETMET	R11	R10	K8
      0x5436002F,  //  0039  LDINT	R13	48
      0x5838000A,  //  003A  LDCONST	R14	K10
      0x7C2C0600,  //  003B  CALL	R11	3
      0x8C2C1508,  //  003C  GETMET	R11	R10	K8
      0x54360080,  //  003D  LDINT	R13	129
      0x5838000A,  //  003E  LDCONST	R14	K10
      0x7C2C0600,  //  003F  CALL	R11	3
      0x8C2C1508,  //  0040  GETMET	R11	R10	K8
      0x543600FE,  //  0041  LDINT	R13	255
      0x2C34120D,  //  0042  AND	R13	R9	R13
      0x5838000A,  //  0043  LDCONST	R14	K10
      0x7C2C0600,  //  0044  CALL	R11	3
      0x402C1405,  //  0045  CONNECT	R11	R10	R5
      0x402C1406,  //  0046  CONNECT	R11	R10	R6
      0x402C1408,  //  0047  CONNECT	R11	R10	R8
      0x80041400,  //  0048  RET	1	R10
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: fabric_candidate
********************************************************************/
be_local_closure(Matter_Session_fabric_candidate,   /* name */
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
    /* K0   */  be_nested_str_weak(_fabric),
    /* K1   */  be_nested_str_weak(set_expire_in_seconds),
    /* K2   */  be_nested_str_weak(assign_fabric_index),
    /* K3   */  be_nested_str_weak(_store),
    /* K4   */  be_nested_str_weak(add_fabric),
    }),
    be_str_weak(fabric_candidate),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x540E0077,  //  0002  LDINT	R3	120
      0x7C040400,  //  0003  CALL	R1	2
      0x8C040102,  //  0004  GETMET	R1	R0	K2
      0x7C040200,  //  0005  CALL	R1	1
      0x88040103,  //  0006  GETMBR	R1	R0	K3
      0x8C040304,  //  0007  GETMET	R1	R1	K4
      0x880C0100,  //  0008  GETMBR	R3	R0	K0
      0x7C040400,  //  0009  CALL	R1	2
      0x80000000,  //  000A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: close
********************************************************************/
be_local_closure(Matter_Session_close,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[23]) {     /* constants */
    /* K0   */  be_nested_str_weak(local_session_id),
    /* K1   */  be_nested_str_weak(__future_local_session_id),
    /* K2   */  be_nested_str_weak(initiator_session_id),
    /* K3   */  be_nested_str_weak(__future_initiator_session_id),
    /* K4   */  be_nested_str_weak(_counter_rcv_impl),
    /* K5   */  be_nested_str_weak(reset),
    /* K6   */  be_nested_str_weak(_counter_snd_impl),
    /* K7   */  be_nested_str_weak(counter_rcv),
    /* K8   */  be_const_int(0),
    /* K9   */  be_nested_str_weak(counter_snd),
    /* K10  */  be_nested_str_weak(next),
    /* K11  */  be_nested_str_weak(i2rkey),
    /* K12  */  be_nested_str_weak(_i2r_privacy),
    /* K13  */  be_nested_str_weak(r2ikey),
    /* K14  */  be_nested_str_weak(attestation_challenge),
    /* K15  */  be_nested_str_weak(introspect),
    /* K16  */  be_nested_str_weak(members),
    /* K17  */  be_nested_str_weak(get),
    /* K18  */  be_nested_str_weak(function),
    /* K19  */  be_nested_str_weak(instance),
    /* K20  */  be_nested_str_weak(_),
    /* K21  */  be_const_int(1),
    /* K22  */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(close),
    &be_const_str_solidified,
    ( &(const binstruction[59]) {  /* code */
      0x88040101,  //  0000  GETMBR	R1	R0	K1
      0x90020001,  //  0001  SETMBR	R0	K0	R1
      0x88040103,  //  0002  GETMBR	R1	R0	K3
      0x90020401,  //  0003  SETMBR	R0	K2	R1
      0x88040104,  //  0004  GETMBR	R1	R0	K4
      0x8C040305,  //  0005  GETMET	R1	R1	K5
      0x7C040200,  //  0006  CALL	R1	1
      0x88040106,  //  0007  GETMBR	R1	R0	K6
      0x8C040305,  //  0008  GETMET	R1	R1	K5
      0x7C040200,  //  0009  CALL	R1	1
      0x90020F08,  //  000A  SETMBR	R0	K7	K8
      0x88040106,  //  000B  GETMBR	R1	R0	K6
      0x8C04030A,  //  000C  GETMET	R1	R1	K10
      0x7C040200,  //  000D  CALL	R1	1
      0x90021201,  //  000E  SETMBR	R0	K9	R1
      0x4C040000,  //  000F  LDNIL	R1
      0x90021601,  //  0010  SETMBR	R0	K11	R1
      0x4C040000,  //  0011  LDNIL	R1
      0x90021801,  //  0012  SETMBR	R0	K12	R1
      0x4C040000,  //  0013  LDNIL	R1
      0x90021A01,  //  0014  SETMBR	R0	K13	R1
      0x4C040000,  //  0015  LDNIL	R1
      0x90021C01,  //  0016  SETMBR	R0	K14	R1
      0xA4061E00,  //  0017  IMPORT	R1	K15
      0x60080010,  //  0018  GETGBL	R2	G16
      0x8C0C0310,  //  0019  GETMET	R3	R1	K16
      0x5C140000,  //  001A  MOVE	R5	R0
      0x7C0C0400,  //  001B  CALL	R3	2
      0x7C080200,  //  001C  CALL	R2	1
      0xA8020018,  //  001D  EXBLK	0	#0037
      0x5C0C0400,  //  001E  MOVE	R3	R2
      0x7C0C0000,  //  001F  CALL	R3	0
      0x8C100311,  //  0020  GETMET	R4	R1	K17
      0x5C180000,  //  0021  MOVE	R6	R0
      0x5C1C0600,  //  0022  MOVE	R7	R3
      0x7C100600,  //  0023  CALL	R4	3
      0x60140004,  //  0024  GETGBL	R5	G4
      0x5C180800,  //  0025  MOVE	R6	R4
      0x7C140200,  //  0026  CALL	R5	1
      0x20140B12,  //  0027  NE	R5	R5	K18
      0x7816000C,  //  0028  JMPF	R5	#0036
      0x60140004,  //  0029  GETGBL	R5	G4
      0x5C180800,  //  002A  MOVE	R6	R4
      0x7C140200,  //  002B  CALL	R5	1
      0x20140B13,  //  002C  NE	R5	R5	K19
      0x78160007,  //  002D  JMPF	R5	#0036
      0x94140708,  //  002E  GETIDX	R5	R3	K8
      0x1C140B14,  //  002F  EQ	R5	R5	K20
      0x78160004,  //  0030  JMPF	R5	#0036
      0x94140715,  //  0031  GETIDX	R5	R3	K21
      0x1C140B14,  //  0032  EQ	R5	R5	K20
      0x78160001,  //  0033  JMPF	R5	#0036
      0x4C140000,  //  0034  LDNIL	R5
      0x90000605,  //  0035  SETMBR	R0	R3	R5
      0x7001FFE6,  //  0036  JMP		#001E
      0x58080016,  //  0037  LDCONST	R2	K22
      0xAC080200,  //  0038  CATCH	R2	1	0
      0xB0080000,  //  0039  RAISE	2	R0	R0
      0x80000000,  //  003A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: hydrate_post
********************************************************************/
be_local_closure(Matter_Session_hydrate_post,   /* name */
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
    /* K0   */  be_nested_str_weak(_counter_snd_impl),
    /* K1   */  be_nested_str_weak(reset),
    /* K2   */  be_nested_str_weak(counter_snd),
    /* K3   */  be_nested_str_weak(_counter_rcv_impl),
    /* K4   */  be_nested_str_weak(counter_rcv),
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
** Solidified function: get_r2i
********************************************************************/
be_local_closure(Matter_Session_get_r2i,   /* name */
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
    /* K0   */  be_nested_str_weak(r2ikey),
    }),
    be_str_weak(get_r2i),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_mode
********************************************************************/
be_local_closure(Matter_Session_set_mode,   /* name */
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
    /* K0   */  be_nested_str_weak(mode),
    }),
    be_str_weak(set_mode),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x80000000,  //  0001  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_mode_CASE
********************************************************************/
be_local_closure(Matter_Session_set_mode_CASE,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(set_mode),
    /* K1   */  be_nested_str_weak(_CASE),
    }),
    be_str_weak(set_mode_CASE),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x880C0101,  //  0001  GETMBR	R3	R0	K1
      0x7C040400,  //  0002  CALL	R1	2
      0x80000000,  //  0003  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_fabric_label
********************************************************************/
be_local_closure(Matter_Session_set_fabric_label,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(_fabric),
    /* K2   */  be_nested_str_weak(fabric_label),
    }),
    be_str_weak(set_fabric_label),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x60080004,  //  0000  GETGBL	R2	G4
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x7C080200,  //  0002  CALL	R2	1
      0x1C080500,  //  0003  EQ	R2	R2	K0
      0x780A0001,  //  0004  JMPF	R2	#0007
      0x88080101,  //  0005  GETMBR	R2	R0	K1
      0x900A0401,  //  0006  SETMBR	R2	K2	R1
      0x80000000,  //  0007  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_fabric_label
********************************************************************/
be_local_closure(Matter_Session_get_fabric_label,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(_fabric),
    /* K1   */  be_nested_str_weak(fabric_label),
    }),
    be_str_weak(get_fabric_label),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x88040301,  //  0001  GETMBR	R1	R1	K1
      0x80040200,  //  0002  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: save
********************************************************************/
be_local_closure(Matter_Session_save,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(_store),
    /* K1   */  be_nested_str_weak(save_fabrics),
    }),
    be_str_weak(save),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x7C040200,  //  0002  CALL	R1	1
      0x80000000,  //  0003  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_ac
********************************************************************/
be_local_closure(Matter_Session_get_ac,   /* name */
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
    /* K0   */  be_nested_str_weak(attestation_challenge),
    }),
    be_str_weak(get_ac),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_device_id
********************************************************************/
be_local_closure(Matter_Session_get_device_id,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(_fabric),
    /* K1   */  be_nested_str_weak(device_id),
    }),
    be_str_weak(get_device_id),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x88040301,  //  0001  GETMBR	R1	R1	K1
      0x80040200,  //  0002  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_ipk_epoch_key
********************************************************************/
be_local_closure(Matter_Session_set_ipk_epoch_key,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(_fabric),
    /* K1   */  be_nested_str_weak(ipk_epoch_key),
    }),
    be_str_weak(set_ipk_epoch_key),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x900A0201,  //  0001  SETMBR	R2	K1	R1
      0x80000000,  //  0002  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: fromjson
********************************************************************/
be_local_closure(Matter_Session_fromjson,   /* name */
  be_nested_proto(
    17,                          /* nstack */
    3,                          /* argc */
    4,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[17]) {     /* constants */
    /* K0   */  be_const_class(be_class_Matter_Session),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(introspect),
    /* K3   */  be_nested_str_weak(matter),
    /* K4   */  be_nested_str_weak(Session),
    /* K5   */  be_nested_str_weak(keys),
    /* K6   */  be_nested_str_weak(find),
    /* K7   */  be_nested_str_weak(0x),
    /* K8   */  be_const_int(0),
    /* K9   */  be_nested_str_weak(set),
    /* K10  */  be_nested_str_weak(fromhex),
    /* K11  */  be_const_int(2),
    /* K12  */  be_const_int(2147483647),
    /* K13  */  be_nested_str_weak(_X24_X24),
    /* K14  */  be_nested_str_weak(fromb64),
    /* K15  */  be_nested_str_weak(stop_iteration),
    /* K16  */  be_nested_str_weak(hydrate_post),
    }),
    be_str_weak(fromjson),
    &be_const_str_solidified,
    ( &(const binstruction[75]) {  /* code */
      0x580C0000,  //  0000  LDCONST	R3	K0
      0xA4120200,  //  0001  IMPORT	R4	K1
      0xA4160400,  //  0002  IMPORT	R5	K2
      0xB81A0600,  //  0003  GETNGBL	R6	K3
      0x8C180D04,  //  0004  GETMET	R6	R6	K4
      0x5C200000,  //  0005  MOVE	R8	R0
      0x4C240000,  //  0006  LDNIL	R9
      0x4C280000,  //  0007  LDNIL	R10
      0x5C2C0400,  //  0008  MOVE	R11	R2
      0x7C180A00,  //  0009  CALL	R6	5
      0x601C0010,  //  000A  GETGBL	R7	G16
      0x8C200305,  //  000B  GETMET	R8	R1	K5
      0x7C200200,  //  000C  CALL	R8	1
      0x7C1C0200,  //  000D  CALL	R7	1
      0xA8020035,  //  000E  EXBLK	0	#0045
      0x5C200E00,  //  000F  MOVE	R8	R7
      0x7C200000,  //  0010  CALL	R8	0
      0x94240208,  //  0011  GETIDX	R9	R1	R8
      0x60280004,  //  0012  GETGBL	R10	G4
      0x5C2C1200,  //  0013  MOVE	R11	R9
      0x7C280200,  //  0014  CALL	R10	1
      0x1C281501,  //  0015  EQ	R10	R10	K1
      0x782A0027,  //  0016  JMPF	R10	#003F
      0x8C280906,  //  0017  GETMET	R10	R4	K6
      0x5C301200,  //  0018  MOVE	R12	R9
      0x58340007,  //  0019  LDCONST	R13	K7
      0x7C280600,  //  001A  CALL	R10	3
      0x1C281508,  //  001B  EQ	R10	R10	K8
      0x782A000A,  //  001C  JMPF	R10	#0028
      0x8C280B09,  //  001D  GETMET	R10	R5	K9
      0x5C300C00,  //  001E  MOVE	R12	R6
      0x5C341000,  //  001F  MOVE	R13	R8
      0x60380015,  //  0020  GETGBL	R14	G21
      0x7C380000,  //  0021  CALL	R14	0
      0x8C381D0A,  //  0022  GETMET	R14	R14	K10
      0x4042170C,  //  0023  CONNECT	R16	K11	K12
      0x94401210,  //  0024  GETIDX	R16	R9	R16
      0x7C380400,  //  0025  CALL	R14	2
      0x7C280800,  //  0026  CALL	R10	4
      0x70020015,  //  0027  JMP		#003E
      0x8C280906,  //  0028  GETMET	R10	R4	K6
      0x5C301200,  //  0029  MOVE	R12	R9
      0x5834000D,  //  002A  LDCONST	R13	K13
      0x7C280600,  //  002B  CALL	R10	3
      0x1C281508,  //  002C  EQ	R10	R10	K8
      0x782A000A,  //  002D  JMPF	R10	#0039
      0x8C280B09,  //  002E  GETMET	R10	R5	K9
      0x5C300C00,  //  002F  MOVE	R12	R6
      0x5C341000,  //  0030  MOVE	R13	R8
      0x60380015,  //  0031  GETGBL	R14	G21
      0x7C380000,  //  0032  CALL	R14	0
      0x8C381D0E,  //  0033  GETMET	R14	R14	K14
      0x4042170C,  //  0034  CONNECT	R16	K11	K12
      0x94401210,  //  0035  GETIDX	R16	R9	R16
      0x7C380400,  //  0036  CALL	R14	2
      0x7C280800,  //  0037  CALL	R10	4
      0x70020004,  //  0038  JMP		#003E
      0x8C280B09,  //  0039  GETMET	R10	R5	K9
      0x5C300C00,  //  003A  MOVE	R12	R6
      0x5C341000,  //  003B  MOVE	R13	R8
      0x5C381200,  //  003C  MOVE	R14	R9
      0x7C280800,  //  003D  CALL	R10	4
      0x70020004,  //  003E  JMP		#0044
      0x8C280B09,  //  003F  GETMET	R10	R5	K9
      0x5C300C00,  //  0040  MOVE	R12	R6
      0x5C341000,  //  0041  MOVE	R13	R8
      0x5C381200,  //  0042  MOVE	R14	R9
      0x7C280800,  //  0043  CALL	R10	4
      0x7001FFC9,  //  0044  JMP		#000F
      0x581C000F,  //  0045  LDCONST	R7	K15
      0xAC1C0200,  //  0046  CATCH	R7	1	0
      0xB0080000,  //  0047  RAISE	2	R0	R0
      0x8C1C0D10,  //  0048  GETMET	R7	R6	K16
      0x7C1C0200,  //  0049  CALL	R7	1
      0x80040C00,  //  004A  RET	1	R6
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_Session_init,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    5,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[24]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(_store),
    /* K2   */  be_nested_str_weak(mode),
    /* K3   */  be_const_int(0),
    /* K4   */  be_nested_str_weak(local_session_id),
    /* K5   */  be_nested_str_weak(initiator_session_id),
    /* K6   */  be_nested_str_weak(_counter_snd_impl),
    /* K7   */  be_nested_str_weak(matter),
    /* K8   */  be_nested_str_weak(Counter),
    /* K9   */  be_nested_str_weak(_counter_rcv_impl),
    /* K10  */  be_nested_str_weak(counter_rcv),
    /* K11  */  be_nested_str_weak(counter_snd),
    /* K12  */  be_nested_str_weak(next),
    /* K13  */  be_nested_str_weak(_COUNTER_SND_INCR),
    /* K14  */  be_nested_str_weak(_counter_insecure_rcv),
    /* K15  */  be_nested_str_weak(_counter_insecure_snd),
    /* K16  */  be_nested_str_weak(_breadcrumb),
    /* K17  */  be_nested_str_weak(_exchange_id),
    /* K18  */  be_nested_str_weak(random),
    /* K19  */  be_const_int(2),
    /* K20  */  be_nested_str_weak(get),
    /* K21  */  be_nested_str_weak(_fabric),
    /* K22  */  be_nested_str_weak(create_fabric),
    /* K23  */  be_nested_str_weak(update),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[47]) {  /* code */
      0xA4160000,  //  0000  IMPORT	R5	K0
      0x90020201,  //  0001  SETMBR	R0	K1	R1
      0x90020503,  //  0002  SETMBR	R0	K2	K3
      0x90020802,  //  0003  SETMBR	R0	K4	R2
      0x90020A03,  //  0004  SETMBR	R0	K5	R3
      0xB81A0E00,  //  0005  GETNGBL	R6	K7
      0x8C180D08,  //  0006  GETMET	R6	R6	K8
      0x7C180200,  //  0007  CALL	R6	1
      0x90020C06,  //  0008  SETMBR	R0	K6	R6
      0xB81A0E00,  //  0009  GETNGBL	R6	K7
      0x8C180D08,  //  000A  GETMET	R6	R6	K8
      0x7C180200,  //  000B  CALL	R6	1
      0x90021206,  //  000C  SETMBR	R0	K9	R6
      0x90021503,  //  000D  SETMBR	R0	K10	K3
      0x88180106,  //  000E  GETMBR	R6	R0	K6
      0x8C180D0C,  //  000F  GETMET	R6	R6	K12
      0x7C180200,  //  0010  CALL	R6	1
      0x881C010D,  //  0011  GETMBR	R7	R0	K13
      0x00180C07,  //  0012  ADD	R6	R6	R7
      0x90021606,  //  0013  SETMBR	R0	K11	R6
      0xB81A0E00,  //  0014  GETNGBL	R6	K7
      0x8C180D08,  //  0015  GETMET	R6	R6	K8
      0x7C180200,  //  0016  CALL	R6	1
      0x90021C06,  //  0017  SETMBR	R0	K14	R6
      0xB81A0E00,  //  0018  GETNGBL	R6	K7
      0x8C180D08,  //  0019  GETMET	R6	R6	K8
      0x7C180200,  //  001A  CALL	R6	1
      0x90021E06,  //  001B  SETMBR	R0	K15	R6
      0x90022103,  //  001C  SETMBR	R0	K16	K3
      0x8C180B12,  //  001D  GETMET	R6	R5	K18
      0x58200013,  //  001E  LDCONST	R8	K19
      0x7C180400,  //  001F  CALL	R6	2
      0x8C180D14,  //  0020  GETMET	R6	R6	K20
      0x58200003,  //  0021  LDCONST	R8	K3
      0x58240013,  //  0022  LDCONST	R9	K19
      0x7C180600,  //  0023  CALL	R6	3
      0x90022206,  //  0024  SETMBR	R0	K17	R6
      0x78120001,  //  0025  JMPF	R4	#0028
      0x5C180800,  //  0026  MOVE	R6	R4
      0x70020002,  //  0027  JMP		#002B
      0x88180101,  //  0028  GETMBR	R6	R0	K1
      0x8C180D16,  //  0029  GETMET	R6	R6	K22
      0x7C180200,  //  002A  CALL	R6	1
      0x90022A06,  //  002B  SETMBR	R0	K21	R6
      0x8C180117,  //  002C  GETMET	R6	R0	K23
      0x7C180200,  //  002D  CALL	R6	1
      0x80000000,  //  002E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update
********************************************************************/
be_local_closure(Matter_Session_update,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(last_used),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(rtc),
    /* K3   */  be_nested_str_weak(utc),
    }),
    be_str_weak(update),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0xB8060200,  //  0000  GETNGBL	R1	K1
      0x8C040302,  //  0001  GETMET	R1	R1	K2
      0x7C040200,  //  0002  CALL	R1	1
      0x94040303,  //  0003  GETIDX	R1	R1	K3
      0x90020001,  //  0004  SETMBR	R0	K0	R1
      0x80000000,  //  0005  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_PASE
********************************************************************/
be_local_closure(Matter_Session_is_PASE,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(mode),
    /* K1   */  be_nested_str_weak(_PASE),
    }),
    be_str_weak(is_PASE),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x88080101,  //  0001  GETMBR	R2	R0	K1
      0x1C040202,  //  0002  EQ	R1	R1	R2
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_i2r
********************************************************************/
be_local_closure(Matter_Session_get_i2r,   /* name */
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
    /* K0   */  be_nested_str_weak(i2rkey),
    }),
    be_str_weak(get_i2r),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_ipk_epoch_key
********************************************************************/
be_local_closure(Matter_Session_get_ipk_epoch_key,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(_fabric),
    /* K1   */  be_nested_str_weak(ipk_epoch_key),
    }),
    be_str_weak(get_ipk_epoch_key),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x88040301,  //  0001  GETMBR	R1	R1	K1
      0x80040200,  //  0002  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_admin_subject_vendor
********************************************************************/
be_local_closure(Matter_Session_set_admin_subject_vendor,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(_fabric),
    /* K1   */  be_nested_str_weak(admin_subject),
    /* K2   */  be_nested_str_weak(admin_vendor),
    }),
    be_str_weak(set_admin_subject_vendor),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x900E0201,  //  0001  SETMBR	R3	K1	R1
      0x880C0100,  //  0002  GETMBR	R3	R0	K0
      0x900E0402,  //  0003  SETMBR	R3	K2	R2
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_fabric_id
********************************************************************/
be_local_closure(Matter_Session_get_fabric_id,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(_fabric),
    /* K1   */  be_nested_str_weak(fabric_id),
    }),
    be_str_weak(get_fabric_id),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x88040301,  //  0001  GETMBR	R1	R1	K1
      0x80040200,  //  0002  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_ca
********************************************************************/
be_local_closure(Matter_Session_set_ca,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(_fabric),
    /* K1   */  be_nested_str_weak(root_ca_certificate),
    }),
    be_str_weak(set_ca),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x900A0201,  //  0001  SETMBR	R2	K1	R1
      0x80000000,  //  0002  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_fabric_device
********************************************************************/
be_local_closure(Matter_Session_set_fabric_device,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    5,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(_fabric),
    /* K1   */  be_nested_str_weak(fabric_id),
    /* K2   */  be_nested_str_weak(device_id),
    /* K3   */  be_nested_str_weak(fabric_compressed),
    /* K4   */  be_nested_str_weak(fabric_parent),
    /* K5   */  be_nested_str_weak(get_fabric_index),
    }),
    be_str_weak(set_fabric_device),
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0x88140100,  //  0000  GETMBR	R5	R0	K0
      0x90160201,  //  0001  SETMBR	R5	K1	R1
      0x88140100,  //  0002  GETMBR	R5	R0	K0
      0x90160402,  //  0003  SETMBR	R5	K2	R2
      0x88140100,  //  0004  GETMBR	R5	R0	K0
      0x90160603,  //  0005  SETMBR	R5	K3	R3
      0x88140100,  //  0006  GETMBR	R5	R0	K0
      0x4C180000,  //  0007  LDNIL	R6
      0x20180806,  //  0008  NE	R6	R4	R6
      0x781A0002,  //  0009  JMPF	R6	#000D
      0x8C180905,  //  000A  GETMET	R6	R4	K5
      0x7C180200,  //  000B  CALL	R6	1
      0x70020000,  //  000C  JMP		#000E
      0x4C180000,  //  000D  LDNIL	R6
      0x90160806,  //  000E  SETMBR	R5	K4	R6
      0x80000000,  //  000F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_icac
********************************************************************/
be_local_closure(Matter_Session_get_icac,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(_fabric),
    /* K1   */  be_nested_str_weak(icac),
    }),
    be_str_weak(get_icac),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x88040301,  //  0001  GETMBR	R1	R1	K1
      0x80040200,  //  0002  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_mode_PASE
********************************************************************/
be_local_closure(Matter_Session_set_mode_PASE,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(set_mode),
    /* K1   */  be_nested_str_weak(_PASE),
    }),
    be_str_weak(set_mode_PASE),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x880C0101,  //  0001  GETMBR	R3	R0	K1
      0x7C040400,  //  0002  CALL	R1	2
      0x80000000,  //  0003  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_admin_subject
********************************************************************/
be_local_closure(Matter_Session_get_admin_subject,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(_fabric),
    /* K1   */  be_nested_str_weak(admin_subject),
    }),
    be_str_weak(get_admin_subject),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x88040301,  //  0001  GETMBR	R1	R1	K1
      0x80040200,  //  0002  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_ipk_group_key
********************************************************************/
be_local_closure(Matter_Session_get_ipk_group_key,   /* name */
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
    /* K0   */  be_nested_str_weak(get_ipk_epoch_key),
    /* K1   */  be_nested_str_weak(get_fabric_compressed),
    /* K2   */  be_nested_str_weak(crypto),
    /* K3   */  be_nested_str_weak(HKDF_SHA256),
    /* K4   */  be_nested_str_weak(fromstring),
    /* K5   */  be_nested_str_weak(_GROUP_KEY),
    /* K6   */  be_nested_str_weak(derive),
    }),
    be_str_weak(get_ipk_group_key),
    &be_const_str_solidified,
    ( &(const binstruction[29]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x1C040202,  //  0003  EQ	R1	R1	R2
      0x74060004,  //  0004  JMPT	R1	#000A
      0x8C040101,  //  0005  GETMET	R1	R0	K1
      0x7C040200,  //  0006  CALL	R1	1
      0x4C080000,  //  0007  LDNIL	R2
      0x1C040202,  //  0008  EQ	R1	R1	R2
      0x78060001,  //  0009  JMPF	R1	#000C
      0x4C040000,  //  000A  LDNIL	R1
      0x80040200,  //  000B  RET	1	R1
      0xA4060400,  //  000C  IMPORT	R1	K2
      0x8C080303,  //  000D  GETMET	R2	R1	K3
      0x7C080200,  //  000E  CALL	R2	1
      0x600C0015,  //  000F  GETGBL	R3	G21
      0x7C0C0000,  //  0010  CALL	R3	0
      0x8C0C0704,  //  0011  GETMET	R3	R3	K4
      0x88140105,  //  0012  GETMBR	R5	R0	K5
      0x7C0C0400,  //  0013  CALL	R3	2
      0x8C100506,  //  0014  GETMET	R4	R2	K6
      0x8C180100,  //  0015  GETMET	R6	R0	K0
      0x7C180200,  //  0016  CALL	R6	1
      0x8C1C0101,  //  0017  GETMET	R7	R0	K1
      0x7C1C0200,  //  0018  CALL	R7	1
      0x5C200600,  //  0019  MOVE	R8	R3
      0x5426000F,  //  001A  LDINT	R9	16
      0x7C100A00,  //  001B  CALL	R4	5
      0x80040800,  //  001C  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_ca_pub
********************************************************************/
be_local_closure(Matter_Session_get_ca_pub,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(_fabric),
    /* K1   */  be_nested_str_weak(get_ca_pub),
    }),
    be_str_weak(get_ca_pub),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x7C040200,  //  0002  CALL	R1	1
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_CASE
********************************************************************/
be_local_closure(Matter_Session_is_CASE,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(mode),
    /* K1   */  be_nested_str_weak(_CASE),
    }),
    be_str_weak(is_CASE),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x88080101,  //  0001  GETMBR	R2	R0	K1
      0x1C040202,  //  0002  EQ	R1	R1	R2
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_fabric_compressed
********************************************************************/
be_local_closure(Matter_Session_get_fabric_compressed,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(_fabric),
    /* K1   */  be_nested_str_weak(fabric_compressed),
    }),
    be_str_weak(get_fabric_compressed),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x88040301,  //  0001  GETMBR	R1	R1	K1
      0x80040200,  //  0002  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: counter_rcv_validate
********************************************************************/
be_local_closure(Matter_Session_counter_rcv_validate,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(_counter_rcv_impl),
    /* K1   */  be_nested_str_weak(validate),
    /* K2   */  be_nested_str_weak(counter_rcv),
    /* K3   */  be_nested_str_weak(val),
    }),
    be_str_weak(counter_rcv_validate),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x8C0C0701,  //  0001  GETMET	R3	R3	K1
      0x5C140200,  //  0002  MOVE	R5	R1
      0x5C180400,  //  0003  MOVE	R6	R2
      0x7C0C0600,  //  0004  CALL	R3	3
      0x780E0003,  //  0005  JMPF	R3	#000A
      0x88100100,  //  0006  GETMBR	R4	R0	K0
      0x8C100903,  //  0007  GETMET	R4	R4	K3
      0x7C100200,  //  0008  CALL	R4	1
      0x90020404,  //  0009  SETMBR	R0	K2	R4
      0x80040600,  //  000A  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_noc
********************************************************************/
be_local_closure(Matter_Session_set_noc,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(_fabric),
    /* K1   */  be_nested_str_weak(noc),
    /* K2   */  be_nested_str_weak(icac),
    }),
    be_str_weak(set_noc),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x900E0201,  //  0001  SETMBR	R3	K1	R1
      0x880C0100,  //  0002  GETMBR	R3	R0	K0
      0x900E0402,  //  0003  SETMBR	R3	K2	R2
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_i2r_privacy
********************************************************************/
be_local_closure(Matter_Session_get_i2r_privacy,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(_i2r_privacy),
    /* K1   */  be_nested_str_weak(crypto),
    /* K2   */  be_nested_str_weak(HKDF_SHA256),
    /* K3   */  be_nested_str_weak(derive),
    /* K4   */  be_nested_str_weak(get_i2r),
    /* K5   */  be_nested_str_weak(fromstring),
    /* K6   */  be_nested_str_weak(PrivacyKey),
    }),
    be_str_weak(get_i2r_privacy),
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x4C080000,  //  0001  LDNIL	R2
      0x1C040202,  //  0002  EQ	R1	R1	R2
      0x7806000F,  //  0003  JMPF	R1	#0014
      0xA4060200,  //  0004  IMPORT	R1	K1
      0x8C080302,  //  0005  GETMET	R2	R1	K2
      0x7C080200,  //  0006  CALL	R2	1
      0x8C080503,  //  0007  GETMET	R2	R2	K3
      0x8C100104,  //  0008  GETMET	R4	R0	K4
      0x7C100200,  //  0009  CALL	R4	1
      0x60140015,  //  000A  GETGBL	R5	G21
      0x7C140000,  //  000B  CALL	R5	0
      0x60180015,  //  000C  GETGBL	R6	G21
      0x7C180000,  //  000D  CALL	R6	0
      0x8C180D05,  //  000E  GETMET	R6	R6	K5
      0x58200006,  //  000F  LDCONST	R8	K6
      0x7C180400,  //  0010  CALL	R6	2
      0x541E000F,  //  0011  LDINT	R7	16
      0x7C080A00,  //  0012  CALL	R2	5
      0x90020002,  //  0013  SETMBR	R0	K0	R2
      0x88040100,  //  0014  GETMBR	R1	R0	K0
      0x80040200,  //  0015  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_keys
********************************************************************/
be_local_closure(Matter_Session_set_keys,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    5,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(i2rkey),
    /* K1   */  be_nested_str_weak(_i2r_privacy),
    /* K2   */  be_nested_str_weak(r2ikey),
    /* K3   */  be_nested_str_weak(attestation_challenge),
    /* K4   */  be_nested_str_weak(created),
    }),
    be_str_weak(set_keys),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x4C140000,  //  0001  LDNIL	R5
      0x90020205,  //  0002  SETMBR	R0	K1	R5
      0x90020402,  //  0003  SETMBR	R0	K2	R2
      0x90020603,  //  0004  SETMBR	R0	K3	R3
      0x90020804,  //  0005  SETMBR	R0	K4	R4
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_admin_vendor
********************************************************************/
be_local_closure(Matter_Session_get_admin_vendor,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(_fabric),
    /* K1   */  be_nested_str_weak(admin_vendor),
    }),
    be_str_weak(get_admin_vendor),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x88040301,  //  0001  GETMBR	R1	R1	K1
      0x80040200,  //  0002  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: fabric_completed
********************************************************************/
be_local_closure(Matter_Session_fabric_completed,   /* name */
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
    /* K0   */  be_nested_str_weak(_fabric),
    /* K1   */  be_nested_str_weak(set_no_expiration),
    /* K2   */  be_nested_str_weak(set_persist),
    /* K3   */  be_nested_str_weak(assign_fabric_index),
    /* K4   */  be_nested_str_weak(_store),
    /* K5   */  be_nested_str_weak(add_fabric),
    }),
    be_str_weak(fabric_completed),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x7C040200,  //  0002  CALL	R1	1
      0x88040100,  //  0003  GETMBR	R1	R0	K0
      0x8C040302,  //  0004  GETMET	R1	R1	K2
      0x500C0200,  //  0005  LDBOOL	R3	1	0
      0x7C040400,  //  0006  CALL	R1	2
      0x8C040103,  //  0007  GETMET	R1	R0	K3
      0x7C040200,  //  0008  CALL	R1	1
      0x88040104,  //  0009  GETMBR	R1	R0	K4
      0x8C040305,  //  000A  GETMET	R1	R1	K5
      0x880C0100,  //  000B  GETMBR	R3	R0	K0
      0x7C040400,  //  000C  CALL	R1	2
      0x80000000,  //  000D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_pk
********************************************************************/
be_local_closure(Matter_Session_get_pk,   /* name */
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
    /* K0   */  be_nested_str_weak(_fabric),
    /* K1   */  be_nested_str_weak(no_private_key),
    /* K2   */  be_nested_str_weak(crypto),
    /* K3   */  be_nested_str_weak(random),
    }),
    be_str_weak(get_pk),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x88040301,  //  0001  GETMBR	R1	R1	K1
      0x74060005,  //  0002  JMPT	R1	#0009
      0xA4060400,  //  0003  IMPORT	R1	K2
      0x88080100,  //  0004  GETMBR	R2	R0	K0
      0x8C0C0303,  //  0005  GETMET	R3	R1	K3
      0x5416001F,  //  0006  LDINT	R5	32
      0x7C0C0400,  //  0007  CALL	R3	2
      0x900A0203,  //  0008  SETMBR	R2	K1	R3
      0x88040100,  //  0009  GETMBR	R1	R0	K0
      0x88040301,  //  000A  GETMBR	R1	R1	K1
      0x80040200,  //  000B  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_noc
********************************************************************/
be_local_closure(Matter_Session_get_noc,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(_fabric),
    /* K1   */  be_nested_str_weak(noc),
    }),
    be_str_weak(get_noc),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x88040301,  //  0001  GETMBR	R1	R1	K1
      0x80040200,  //  0002  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Session
********************************************************************/
extern const bclass be_class_Matter_Expirable;
be_local_class(Matter_Session,
    31,
    &be_class_Matter_Expirable,
    be_nested_map(81,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(_counter_insecure_snd, -1), be_const_var(19) },
        { be_const_key_weak(_source_node_id, -1), be_const_var(7) },
        { be_const_key_weak(get_fabric, -1), be_const_closure(Matter_Session_get_fabric_closure) },
        { be_const_key_weak(_i2r_privacy, -1), be_const_var(22) },
        { be_const_key_weak(get_pk, -1), be_const_closure(Matter_Session_get_pk_closure) },
        { be_const_key_weak(counter_snd_next, -1), be_const_closure(Matter_Session_counter_snd_next_closure) },
        { be_const_key_weak(attestation_challenge, -1), be_const_var(23) },
        { be_const_key_weak(close, -1), be_const_closure(Matter_Session_close_closure) },
        { be_const_key_weak(before_remove, 0), be_const_closure(Matter_Session_before_remove_closure) },
        { be_const_key_weak(initiator_session_id, 69), be_const_var(4) },
        { be_const_key_weak(_exchange_id, 33), be_const_var(14) },
        { be_const_key_weak(get_ca, -1), be_const_closure(Matter_Session_get_ca_closure) },
        { be_const_key_weak(save, -1), be_const_closure(Matter_Session_save_closure) },
        { be_const_key_weak(get_admin_vendor, -1), be_const_closure(Matter_Session_get_admin_vendor_closure) },
        { be_const_key_weak(tojson, -1), be_const_closure(Matter_Session_tojson_closure) },
        { be_const_key_weak(assign_fabric_index, -1), be_const_closure(Matter_Session_assign_fabric_index_closure) },
        { be_const_key_weak(gen_CSR, -1), be_const_closure(Matter_Session_gen_CSR_closure) },
        { be_const_key_weak(peer_node_id, -1), be_const_var(24) },
        { be_const_key_weak(fabric_candidate, 74), be_const_closure(Matter_Session_fabric_candidate_closure) },
        { be_const_key_weak(_GROUP_KEY, -1), be_nested_str_weak(GroupKey_X20v1_X2E0) },
        { be_const_key_weak(resumption_id, -1), be_const_var(26) },
        { be_const_key_weak(_fabric, 7), be_const_var(2) },
        { be_const_key_weak(set_keys, -1), be_const_closure(Matter_Session_set_keys_closure) },
        { be_const_key_weak(get_i2r_privacy, 40), be_const_closure(Matter_Session_get_i2r_privacy_closure) },
        { be_const_key_weak(hydrate_post, 46), be_const_closure(Matter_Session_hydrate_post_closure) },
        { be_const_key_weak(set_ipk_epoch_key, 79), be_const_closure(Matter_Session_set_ipk_epoch_key_closure) },
        { be_const_key_weak(set_mode, 35), be_const_closure(Matter_Session_set_mode_closure) },
        { be_const_key_weak(__future_local_session_id, 12), be_const_var(9) },
        { be_const_key_weak(set_mode_CASE, 70), be_const_closure(Matter_Session_set_mode_CASE_closure) },
        { be_const_key_weak(set_fabric_label, -1), be_const_closure(Matter_Session_set_fabric_label_closure) },
        { be_const_key_weak(get_fabric_label, 44), be_const_closure(Matter_Session_get_fabric_label_closure) },
        { be_const_key_weak(init, -1), be_const_closure(Matter_Session_init_closure) },
        { be_const_key_weak(update, -1), be_const_closure(Matter_Session_update_closure) },
        { be_const_key_weak(get_fabric_compressed, -1), be_const_closure(Matter_Session_get_fabric_compressed_closure) },
        { be_const_key_weak(get_ac, 32), be_const_closure(Matter_Session_get_ac_closure) },
        { be_const_key_weak(_CASE, 41), be_const_int(2) },
        { be_const_key_weak(get_device_id, -1), be_const_closure(Matter_Session_get_device_id_closure) },
        { be_const_key_weak(persist_to_fabric, 25), be_const_closure(Matter_Session_persist_to_fabric_closure) },
        { be_const_key_weak(fromjson, -1), be_const_static_closure(Matter_Session_fromjson_closure) },
        { be_const_key_weak(local_session_id, 4), be_const_var(3) },
        { be_const_key_weak(set_admin_subject_vendor, -1), be_const_closure(Matter_Session_set_admin_subject_vendor_closure) },
        { be_const_key_weak(get_fabric_id, -1), be_const_closure(Matter_Session_get_fabric_id_closure) },
        { be_const_key_weak(is_PASE, -1), be_const_closure(Matter_Session_is_PASE_closure) },
        { be_const_key_weak(get_icac, -1), be_const_closure(Matter_Session_get_icac_closure) },
        { be_const_key_weak(shared_secret, -1), be_const_var(27) },
        { be_const_key_weak(_breadcrumb, -1), be_const_var(25) },
        { be_const_key_weak(get_ca_pub, 31), be_const_closure(Matter_Session_get_ca_pub_closure) },
        { be_const_key_weak(_PASE, -1), be_const_int(1) },
        { be_const_key_weak(get_ipk_epoch_key, -1), be_const_closure(Matter_Session_get_ipk_epoch_key_closure) },
        { be_const_key_weak(get_i2r, 23), be_const_closure(Matter_Session_get_i2r_closure) },
        { be_const_key_weak(r2ikey, 65), be_const_var(21) },
        { be_const_key_weak(__Msg1, -1), be_const_var(28) },
        { be_const_key_weak(counter_rcv, -1), be_const_var(10) },
        { be_const_key_weak(_port, -1), be_const_var(16) },
        { be_const_key_weak(set_ca, -1), be_const_closure(Matter_Session_set_ca_closure) },
        { be_const_key_weak(_COUNTER_SND_INCR, -1), be_const_int(1024) },
        { be_const_key_weak(set_fabric_device, -1), be_const_closure(Matter_Session_set_fabric_device_closure) },
        { be_const_key_weak(get_r2i, 43), be_const_closure(Matter_Session_get_r2i_closure) },
        { be_const_key_weak(get_admin_subject, -1), be_const_closure(Matter_Session_get_admin_subject_closure) },
        { be_const_key_weak(__Msg2, -1), be_const_var(29) },
        { be_const_key_weak(_counter_insecure_rcv, 64), be_const_var(18) },
        { be_const_key_weak(_message_handler, 58), be_const_var(17) },
        { be_const_key_weak(get_ipk_group_key, -1), be_const_closure(Matter_Session_get_ipk_group_key_closure) },
        { be_const_key_weak(_ip, 53), be_const_var(15) },
        { be_const_key_weak(set_mode_PASE, -1), be_const_closure(Matter_Session_set_mode_PASE_closure) },
        { be_const_key_weak(_store, -1), be_const_var(0) },
        { be_const_key_weak(mode, -1), be_const_var(1) },
        { be_const_key_weak(is_CASE, -1), be_const_closure(Matter_Session_is_CASE_closure) },
        { be_const_key_weak(__future_initiator_session_id, -1), be_const_var(8) },
        { be_const_key_weak(i2rkey, -1), be_const_var(20) },
        { be_const_key_weak(counter_snd, -1), be_const_var(11) },
        { be_const_key_weak(counter_rcv_validate, -1), be_const_closure(Matter_Session_counter_rcv_validate_closure) },
        { be_const_key_weak(last_used, -1), be_const_var(6) },
        { be_const_key_weak(set_noc, -1), be_const_closure(Matter_Session_set_noc_closure) },
        { be_const_key_weak(_counter_snd_impl, -1), be_const_var(13) },
        { be_const_key_weak(_counter_rcv_impl, 22), be_const_var(12) },
        { be_const_key_weak(get_mode, 13), be_const_closure(Matter_Session_get_mode_closure) },
        { be_const_key_weak(__chunked_attr_reports, -1), be_const_var(30) },
        { be_const_key_weak(fabric_completed, -1), be_const_closure(Matter_Session_fabric_completed_closure) },
        { be_const_key_weak(created, -1), be_const_var(5) },
        { be_const_key_weak(get_noc, -1), be_const_closure(Matter_Session_get_noc_closure) },
    })),
    be_str_weak(Matter_Session)
);
/*******************************************************************/

void be_load_Matter_Session_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Session);
    be_setglobal(vm, "Matter_Session");
    be_pop(vm, 1);
}

extern const bclass be_class_Matter_Session_Store;

/********************************************************************
** Solidified function: gen_local_session_id
********************************************************************/
be_local_closure(Matter_Session_Store_gen_local_session_id,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(random),
    /* K2   */  be_const_int(2),
    /* K3   */  be_nested_str_weak(get),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(get_session_by_local_session_id),
    }),
    be_str_weak(gen_local_session_id),
    &be_const_str_solidified,
    ( &(const binstruction[19]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x50080200,  //  0001  LDBOOL	R2	1	0
      0x780A000E,  //  0002  JMPF	R2	#0012
      0x8C080301,  //  0003  GETMET	R2	R1	K1
      0x58100002,  //  0004  LDCONST	R4	K2
      0x7C080400,  //  0005  CALL	R2	2
      0x8C080503,  //  0006  GETMET	R2	R2	K3
      0x58100004,  //  0007  LDCONST	R4	K4
      0x58140002,  //  0008  LDCONST	R5	K2
      0x7C080600,  //  0009  CALL	R2	3
      0x8C0C0105,  //  000A  GETMET	R3	R0	K5
      0x5C140400,  //  000B  MOVE	R5	R2
      0x7C0C0400,  //  000C  CALL	R3	2
      0x4C100000,  //  000D  LDNIL	R4
      0x1C0C0604,  //  000E  EQ	R3	R3	R4
      0x780E0000,  //  000F  JMPF	R3	#0011
      0x80040400,  //  0010  RET	1	R2
      0x7001FFEE,  //  0011  JMP		#0001
      0x80000000,  //  0012  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: remove_redundant_fabric
********************************************************************/
be_local_closure(Matter_Session_Store_remove_redundant_fabric,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(fabrics),
    /* K2   */  be_nested_str_weak(fabric_id),
    /* K3   */  be_nested_str_weak(device_id),
    /* K4   */  be_nested_str_weak(remove),
    /* K5   */  be_const_int(1),
    }),
    be_str_weak(remove_redundant_fabric),
    &be_const_str_solidified,
    ( &(const binstruction[26]) {  /* code */
      0x58080000,  //  0000  LDCONST	R2	K0
      0x600C000C,  //  0001  GETGBL	R3	G12
      0x88100101,  //  0002  GETMBR	R4	R0	K1
      0x7C0C0200,  //  0003  CALL	R3	1
      0x140C0403,  //  0004  LT	R3	R2	R3
      0x780E0012,  //  0005  JMPF	R3	#0019
      0x880C0101,  //  0006  GETMBR	R3	R0	K1
      0x940C0602,  //  0007  GETIDX	R3	R3	R2
      0x20100601,  //  0008  NE	R4	R3	R1
      0x7812000C,  //  0009  JMPF	R4	#0017
      0x88100702,  //  000A  GETMBR	R4	R3	K2
      0x88140302,  //  000B  GETMBR	R5	R1	K2
      0x1C100805,  //  000C  EQ	R4	R4	R5
      0x78120008,  //  000D  JMPF	R4	#0017
      0x88100703,  //  000E  GETMBR	R4	R3	K3
      0x88140303,  //  000F  GETMBR	R5	R1	K3
      0x1C100805,  //  0010  EQ	R4	R4	R5
      0x78120004,  //  0011  JMPF	R4	#0017
      0x88100101,  //  0012  GETMBR	R4	R0	K1
      0x8C100904,  //  0013  GETMET	R4	R4	K4
      0x5C180400,  //  0014  MOVE	R6	R2
      0x7C100400,  //  0015  CALL	R4	2
      0x70020000,  //  0016  JMP		#0018
      0x00080505,  //  0017  ADD	R2	R2	K5
      0x7001FFE7,  //  0018  JMP		#0001
      0x80000000,  //  0019  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: find_session_by_resumption_id
********************************************************************/
be_local_closure(Matter_Session_Store_find_session_by_resumption_id,   /* name */
  be_nested_proto(
    14,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[12]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_const_int(0),
    /* K2   */  be_nested_str_weak(sessions),
    /* K3   */  be_nested_str_weak(tasmota),
    /* K4   */  be_nested_str_weak(log),
    /* K5   */  be_nested_str_weak(format),
    /* K6   */  be_nested_str_weak(MTR_X3A_X20session_X2Eresumption_id_X3D_X25s_X20vs_X20_X25s),
    /* K7   */  be_nested_str_weak(resumption_id),
    /* K8   */  be_nested_str_weak(shared_secret),
    /* K9   */  be_nested_str_weak(MTR_X3A_X20session_X2Eshared_secret_X3D_X25s),
    /* K10  */  be_nested_str_weak(update),
    /* K11  */  be_const_int(1),
    }),
    be_str_weak(find_session_by_resumption_id),
    &be_const_str_solidified,
    ( &(const binstruction[47]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x740E0001,  //  0002  JMPT	R3	#0005
      0x4C0C0000,  //  0003  LDNIL	R3
      0x80040600,  //  0004  RET	1	R3
      0x580C0001,  //  0005  LDCONST	R3	K1
      0x88100102,  //  0006  GETMBR	R4	R0	K2
      0x6014000C,  //  0007  GETGBL	R5	G12
      0x5C180800,  //  0008  MOVE	R6	R4
      0x7C140200,  //  0009  CALL	R5	1
      0x14140605,  //  000A  LT	R5	R3	R5
      0x78160021,  //  000B  JMPF	R5	#002E
      0x94140803,  //  000C  GETIDX	R5	R4	R3
      0xB81A0600,  //  000D  GETNGBL	R6	K3
      0x8C180D04,  //  000E  GETMET	R6	R6	K4
      0x8C200505,  //  000F  GETMET	R8	R2	K5
      0x58280006,  //  0010  LDCONST	R10	K6
      0x602C0008,  //  0011  GETGBL	R11	G8
      0x88300B07,  //  0012  GETMBR	R12	R5	K7
      0x7C2C0200,  //  0013  CALL	R11	1
      0x60300008,  //  0014  GETGBL	R12	G8
      0x5C340200,  //  0015  MOVE	R13	R1
      0x7C300200,  //  0016  CALL	R12	1
      0x7C200800,  //  0017  CALL	R8	4
      0x7C180400,  //  0018  CALL	R6	2
      0x88180B07,  //  0019  GETMBR	R6	R5	K7
      0x1C180C01,  //  001A  EQ	R6	R6	R1
      0x781A000F,  //  001B  JMPF	R6	#002C
      0x88180B08,  //  001C  GETMBR	R6	R5	K8
      0x4C1C0000,  //  001D  LDNIL	R7
      0x20180C07,  //  001E  NE	R6	R6	R7
      0x781A000B,  //  001F  JMPF	R6	#002C
      0xB81A0600,  //  0020  GETNGBL	R6	K3
      0x8C180D04,  //  0021  GETMET	R6	R6	K4
      0x8C200505,  //  0022  GETMET	R8	R2	K5
      0x58280009,  //  0023  LDCONST	R10	K9
      0x602C0008,  //  0024  GETGBL	R11	G8
      0x88300B08,  //  0025  GETMBR	R12	R5	K8
      0x7C2C0200,  //  0026  CALL	R11	1
      0x7C200600,  //  0027  CALL	R8	3
      0x7C180400,  //  0028  CALL	R6	2
      0x8C180B0A,  //  0029  GETMET	R6	R5	K10
      0x7C180200,  //  002A  CALL	R6	1
      0x80040A00,  //  002B  RET	1	R5
      0x000C070B,  //  002C  ADD	R3	R3	K11
      0x7001FFD8,  //  002D  JMP		#0007
      0x80000000,  //  002E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: remove_fabric
********************************************************************/
be_local_closure(Matter_Session_Store_remove_fabric,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(sessions),
    /* K2   */  be_nested_str_weak(_fabric),
    /* K3   */  be_nested_str_weak(remove),
    /* K4   */  be_const_int(1),
    /* K5   */  be_nested_str_weak(fabrics),
    /* K6   */  be_nested_str_weak(find),
    }),
    be_str_weak(remove_fabric),
    &be_const_str_solidified,
    ( &(const binstruction[26]) {  /* code */
      0x58080000,  //  0000  LDCONST	R2	K0
      0x600C000C,  //  0001  GETGBL	R3	G12
      0x88100101,  //  0002  GETMBR	R4	R0	K1
      0x7C0C0200,  //  0003  CALL	R3	1
      0x140C0403,  //  0004  LT	R3	R2	R3
      0x780E000B,  //  0005  JMPF	R3	#0012
      0x880C0101,  //  0006  GETMBR	R3	R0	K1
      0x940C0602,  //  0007  GETIDX	R3	R3	R2
      0x880C0702,  //  0008  GETMBR	R3	R3	K2
      0x1C0C0601,  //  0009  EQ	R3	R3	R1
      0x780E0004,  //  000A  JMPF	R3	#0010
      0x880C0101,  //  000B  GETMBR	R3	R0	K1
      0x8C0C0703,  //  000C  GETMET	R3	R3	K3
      0x5C140400,  //  000D  MOVE	R5	R2
      0x7C0C0400,  //  000E  CALL	R3	2
      0x70020000,  //  000F  JMP		#0011
      0x00080504,  //  0010  ADD	R2	R2	K4
      0x7001FFEE,  //  0011  JMP		#0001
      0x880C0105,  //  0012  GETMBR	R3	R0	K5
      0x8C0C0703,  //  0013  GETMET	R3	R3	K3
      0x88140105,  //  0014  GETMBR	R5	R0	K5
      0x8C140B06,  //  0015  GETMET	R5	R5	K6
      0x5C1C0200,  //  0016  MOVE	R7	R1
      0x7C140400,  //  0017  CALL	R5	2
      0x7C0C0400,  //  0018  CALL	R3	2
      0x80000000,  //  0019  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: find_session_source_id_unsecure
********************************************************************/
be_local_closure(Matter_Session_Store_find_session_source_id_unsecure,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(get_session_by_source_node_id),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(Session),
    /* K3   */  be_const_int(0),
    /* K4   */  be_nested_str_weak(_source_node_id),
    /* K5   */  be_nested_str_weak(sessions),
    /* K6   */  be_nested_str_weak(push),
    /* K7   */  be_nested_str_weak(set_expire_in_seconds),
    /* K8   */  be_nested_str_weak(update),
    }),
    be_str_weak(find_session_source_id_unsecure),
    &be_const_str_solidified,
    ( &(const binstruction[24]) {  /* code */
      0x8C0C0100,  //  0000  GETMET	R3	R0	K0
      0x5C140200,  //  0001  MOVE	R5	R1
      0x7C0C0400,  //  0002  CALL	R3	2
      0x4C100000,  //  0003  LDNIL	R4
      0x1C100604,  //  0004  EQ	R4	R3	R4
      0x7812000E,  //  0005  JMPF	R4	#0015
      0xB8120200,  //  0006  GETNGBL	R4	K1
      0x8C100902,  //  0007  GETMET	R4	R4	K2
      0x5C180000,  //  0008  MOVE	R6	R0
      0x581C0003,  //  0009  LDCONST	R7	K3
      0x58200003,  //  000A  LDCONST	R8	K3
      0x7C100800,  //  000B  CALL	R4	4
      0x5C0C0800,  //  000C  MOVE	R3	R4
      0x900E0801,  //  000D  SETMBR	R3	K4	R1
      0x88100105,  //  000E  GETMBR	R4	R0	K5
      0x8C100906,  //  000F  GETMET	R4	R4	K6
      0x5C180600,  //  0010  MOVE	R6	R3
      0x7C100400,  //  0011  CALL	R4	2
      0x8C100707,  //  0012  GETMET	R4	R3	K7
      0x5C180400,  //  0013  MOVE	R6	R2
      0x7C100400,  //  0014  CALL	R4	2
      0x8C100708,  //  0015  GETMET	R4	R3	K8
      0x7C100200,  //  0016  CALL	R4	1
      0x80040600,  //  0017  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_session_by_source_node_id
********************************************************************/
be_local_closure(Matter_Session_Store_get_session_by_source_node_id,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(sessions),
    /* K1   */  be_const_int(0),
    /* K2   */  be_nested_str_weak(_source_node_id),
    /* K3   */  be_nested_str_weak(update),
    /* K4   */  be_const_int(1),
    }),
    be_str_weak(get_session_by_source_node_id),
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0001,  //  0002  JMPF	R2	#0005
      0x4C080000,  //  0003  LDNIL	R2
      0x80040400,  //  0004  RET	1	R2
      0x6008000C,  //  0005  GETGBL	R2	G12
      0x880C0100,  //  0006  GETMBR	R3	R0	K0
      0x7C080200,  //  0007  CALL	R2	1
      0x580C0001,  //  0008  LDCONST	R3	K1
      0x88100100,  //  0009  GETMBR	R4	R0	K0
      0x14140602,  //  000A  LT	R5	R3	R2
      0x78160008,  //  000B  JMPF	R5	#0015
      0x94140803,  //  000C  GETIDX	R5	R4	R3
      0x88180B02,  //  000D  GETMBR	R6	R5	K2
      0x1C180C01,  //  000E  EQ	R6	R6	R1
      0x781A0002,  //  000F  JMPF	R6	#0013
      0x8C180B03,  //  0010  GETMET	R6	R5	K3
      0x7C180200,  //  0011  CALL	R6	1
      0x80040A00,  //  0012  RET	1	R5
      0x000C0704,  //  0013  ADD	R3	R3	K4
      0x7001FFF4,  //  0014  JMP		#000A
      0x80000000,  //  0015  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: add_session
********************************************************************/
be_local_closure(Matter_Session_Store_add_session,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(set_expire_in_seconds),
    /* K1   */  be_nested_str_weak(sessions),
    /* K2   */  be_nested_str_weak(push),
    }),
    be_str_weak(add_session),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x4C0C0000,  //  0000  LDNIL	R3
      0x200C0403,  //  0001  NE	R3	R2	R3
      0x780E0002,  //  0002  JMPF	R3	#0006
      0x8C0C0300,  //  0003  GETMET	R3	R1	K0
      0x5C140400,  //  0004  MOVE	R5	R2
      0x7C0C0400,  //  0005  CALL	R3	2
      0x880C0101,  //  0006  GETMBR	R3	R0	K1
      0x8C0C0702,  //  0007  GETMET	R3	R3	K2
      0x5C140200,  //  0008  MOVE	R5	R1
      0x7C0C0400,  //  0009  CALL	R3	2
      0x80000000,  //  000A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: count_active_fabrics
********************************************************************/
be_local_closure(Matter_Session_Store_count_active_fabrics,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(remove_expired),
    /* K1   */  be_nested_str_weak(fabrics),
    /* K2   */  be_nested_str_weak(count_persistables),
    }),
    be_str_weak(count_active_fabrics),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x88040101,  //  0002  GETMBR	R1	R0	K1
      0x8C040302,  //  0003  GETMET	R1	R1	K2
      0x7C040200,  //  0004  CALL	R1	1
      0x80040200,  //  0005  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: remove_expired
********************************************************************/
be_local_closure(Matter_Session_Store_remove_expired,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(sessions),
    /* K1   */  be_nested_str_weak(every_second),
    /* K2   */  be_nested_str_weak(fabrics),
    }),
    be_str_weak(remove_expired),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x7C040200,  //  0002  CALL	R1	1
      0x88040102,  //  0003  GETMBR	R1	R0	K2
      0x8C040301,  //  0004  GETMET	R1	R1	K1
      0x7C040200,  //  0005  CALL	R1	1
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: find_children_fabrics
********************************************************************/
be_local_closure(Matter_Session_Store_find_children_fabrics,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 1]) {
      be_nested_proto(
        7,                          /* nstack */
        1,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 3]) {  /* upvals */
          be_local_const_upval(1, 0),
          be_local_const_upval(1, 2),
          be_local_const_upval(1, 3),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 6]) {     /* constants */
        /* K0   */  be_nested_str_weak(active_fabrics),
        /* K1   */  be_nested_str_weak(fabric_parent),
        /* K2   */  be_nested_str_weak(find),
        /* K3   */  be_nested_str_weak(fabric_index),
        /* K4   */  be_nested_str_weak(push),
        /* K5   */  be_nested_str_weak(stop_iteration),
        }),
        be_str_weak(find_children_fabrics_inner),
        &be_const_str_solidified,
        ( &(const binstruction[30]) {  /* code */
          0x60040010,  //  0000  GETGBL	R1	G16
          0x68080000,  //  0001  GETUPV	R2	U0
          0x8C080500,  //  0002  GETMET	R2	R2	K0
          0x7C080200,  //  0003  CALL	R2	1
          0x7C040200,  //  0004  CALL	R1	1
          0xA8020013,  //  0005  EXBLK	0	#001A
          0x5C080200,  //  0006  MOVE	R2	R1
          0x7C080000,  //  0007  CALL	R2	0
          0x880C0501,  //  0008  GETMBR	R3	R2	K1
          0x1C0C0600,  //  0009  EQ	R3	R3	R0
          0x780E000D,  //  000A  JMPF	R3	#0019
          0x680C0001,  //  000B  GETUPV	R3	U1
          0x8C0C0702,  //  000C  GETMET	R3	R3	K2
          0x7C0C0200,  //  000D  CALL	R3	1
          0x4C100000,  //  000E  LDNIL	R4
          0x1C0C0604,  //  000F  EQ	R3	R3	R4
          0x780E0007,  //  0010  JMPF	R3	#0019
          0x880C0503,  //  0011  GETMBR	R3	R2	K3
          0x68100001,  //  0012  GETUPV	R4	U1
          0x8C100904,  //  0013  GETMET	R4	R4	K4
          0x5C180600,  //  0014  MOVE	R6	R3
          0x7C100400,  //  0015  CALL	R4	2
          0x68100002,  //  0016  GETUPV	R4	U2
          0x5C140600,  //  0017  MOVE	R5	R3
          0x7C100200,  //  0018  CALL	R4	1
          0x7001FFEB,  //  0019  JMP		#0006
          0x58040005,  //  001A  LDCONST	R1	K5
          0xAC040200,  //  001B  CATCH	R1	1	0
          0xB0080000,  //  001C  RAISE	2	R0	R0
          0x80000000,  //  001D  RET	0
        })
      ),
    }),
    0,                          /* has constants */
    NULL,                       /* no const */
    be_str_weak(find_children_fabrics),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0002,  //  0002  JMPF	R2	#0006
      0x60080012,  //  0003  GETGBL	R2	G18
      0x7C080000,  //  0004  CALL	R2	0
      0x80040400,  //  0005  RET	1	R2
      0x60080012,  //  0006  GETGBL	R2	G18
      0x7C080000,  //  0007  CALL	R2	0
      0x400C0401,  //  0008  CONNECT	R3	R2	R1
      0x840C0000,  //  0009  CLOSURE	R3	P0
      0x5C100600,  //  000A  MOVE	R4	R3
      0x5C140200,  //  000B  MOVE	R5	R1
      0x7C100200,  //  000C  CALL	R4	1
      0xA0000000,  //  000D  CLOSE	R0
      0x80040400,  //  000E  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: find_fabric_by_index
********************************************************************/
be_local_closure(Matter_Session_Store_find_fabric_by_index,   /* name */
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
    /* K0   */  be_nested_str_weak(active_fabrics),
    /* K1   */  be_nested_str_weak(get_fabric_index),
    /* K2   */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(find_fabric_by_index),
    &be_const_str_solidified,
    ( &(const binstruction[19]) {  /* code */
      0x60080010,  //  0000  GETGBL	R2	G16
      0x8C0C0100,  //  0001  GETMET	R3	R0	K0
      0x7C0C0200,  //  0002  CALL	R3	1
      0x7C080200,  //  0003  CALL	R2	1
      0xA8020008,  //  0004  EXBLK	0	#000E
      0x5C0C0400,  //  0005  MOVE	R3	R2
      0x7C0C0000,  //  0006  CALL	R3	0
      0x8C100701,  //  0007  GETMET	R4	R3	K1
      0x7C100200,  //  0008  CALL	R4	1
      0x1C100801,  //  0009  EQ	R4	R4	R1
      0x78120001,  //  000A  JMPF	R4	#000D
      0xA8040001,  //  000B  EXBLK	1	1
      0x80040600,  //  000C  RET	1	R3
      0x7001FFF6,  //  000D  JMP		#0005
      0x58080002,  //  000E  LDCONST	R2	K2
      0xAC080200,  //  000F  CATCH	R2	1	0
      0xB0080000,  //  0010  RAISE	2	R0	R0
      0x4C080000,  //  0011  LDNIL	R2
      0x80040400,  //  0012  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: next_fabric_idx
********************************************************************/
be_local_closure(Matter_Session_Store_next_fabric_idx,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(remove_expired),
    /* K1   */  be_const_int(1),
    /* K2   */  be_nested_str_weak(active_fabrics),
    /* K3   */  be_nested_str_weak(fabric_index),
    /* K4   */  be_nested_str_weak(int),
    /* K5   */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(next_fabric_idx),
    &be_const_str_solidified,
    ( &(const binstruction[25]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x58040001,  //  0002  LDCONST	R1	K1
      0x60080010,  //  0003  GETGBL	R2	G16
      0x8C0C0102,  //  0004  GETMET	R3	R0	K2
      0x7C0C0200,  //  0005  CALL	R3	1
      0x7C080200,  //  0006  CALL	R2	1
      0xA802000C,  //  0007  EXBLK	0	#0015
      0x5C0C0400,  //  0008  MOVE	R3	R2
      0x7C0C0000,  //  0009  CALL	R3	0
      0x88100703,  //  000A  GETMBR	R4	R3	K3
      0x60140004,  //  000B  GETGBL	R5	G4
      0x5C180800,  //  000C  MOVE	R6	R4
      0x7C140200,  //  000D  CALL	R5	1
      0x1C140B04,  //  000E  EQ	R5	R5	K4
      0x78160003,  //  000F  JMPF	R5	#0014
      0x28140801,  //  0010  GE	R5	R4	R1
      0x78160001,  //  0011  JMPF	R5	#0014
      0x00140901,  //  0012  ADD	R5	R4	K1
      0x5C040A00,  //  0013  MOVE	R1	R5
      0x7001FFF2,  //  0014  JMP		#0008
      0x58080005,  //  0015  LDCONST	R2	K5
      0xAC080200,  //  0016  CATCH	R2	1	0
      0xB0080000,  //  0017  RAISE	2	R0	R0
      0x80040200,  //  0018  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_session_by_local_session_id
********************************************************************/
be_local_closure(Matter_Session_Store_get_session_by_local_session_id,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(sessions),
    /* K1   */  be_const_int(0),
    /* K2   */  be_nested_str_weak(local_session_id),
    /* K3   */  be_nested_str_weak(update),
    /* K4   */  be_const_int(1),
    }),
    be_str_weak(get_session_by_local_session_id),
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0001,  //  0002  JMPF	R2	#0005
      0x4C080000,  //  0003  LDNIL	R2
      0x80040400,  //  0004  RET	1	R2
      0x6008000C,  //  0005  GETGBL	R2	G12
      0x880C0100,  //  0006  GETMBR	R3	R0	K0
      0x7C080200,  //  0007  CALL	R2	1
      0x580C0001,  //  0008  LDCONST	R3	K1
      0x88100100,  //  0009  GETMBR	R4	R0	K0
      0x14140602,  //  000A  LT	R5	R3	R2
      0x78160008,  //  000B  JMPF	R5	#0015
      0x94140803,  //  000C  GETIDX	R5	R4	R3
      0x88180B02,  //  000D  GETMBR	R6	R5	K2
      0x1C180C01,  //  000E  EQ	R6	R6	R1
      0x781A0002,  //  000F  JMPF	R6	#0013
      0x8C180B03,  //  0010  GETMET	R6	R5	K3
      0x7C180200,  //  0011  CALL	R6	1
      0x80040A00,  //  0012  RET	1	R5
      0x000C0704,  //  0013  ADD	R3	R3	K4
      0x7001FFF4,  //  0014  JMP		#000A
      0x80000000,  //  0015  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: active_fabrics
********************************************************************/
be_local_closure(Matter_Session_Store_active_fabrics,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(remove_expired),
    /* K1   */  be_nested_str_weak(fabrics),
    /* K2   */  be_nested_str_weak(persistables),
    }),
    be_str_weak(active_fabrics),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x88040101,  //  0002  GETMBR	R1	R0	K1
      0x8C040302,  //  0003  GETMET	R1	R1	K2
      0x7C040200,  //  0004  CALL	R1	1
      0x80040200,  //  0005  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: every_second
********************************************************************/
be_local_closure(Matter_Session_Store_every_second,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(remove_expired),
    }),
    be_str_weak(every_second),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x80000000,  //  0002  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: remove_session
********************************************************************/
be_local_closure(Matter_Session_Store_remove_session,   /* name */
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
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(sessions),
    /* K2   */  be_nested_str_weak(remove),
    /* K3   */  be_const_int(1),
    }),
    be_str_weak(remove_session),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0x58080000,  //  0000  LDCONST	R2	K0
      0x880C0101,  //  0001  GETMBR	R3	R0	K1
      0x6010000C,  //  0002  GETGBL	R4	G12
      0x88140101,  //  0003  GETMBR	R5	R0	K1
      0x7C100200,  //  0004  CALL	R4	1
      0x14100404,  //  0005  LT	R4	R2	R4
      0x78120008,  //  0006  JMPF	R4	#0010
      0x94100602,  //  0007  GETIDX	R4	R3	R2
      0x1C100801,  //  0008  EQ	R4	R4	R1
      0x78120003,  //  0009  JMPF	R4	#000E
      0x8C100702,  //  000A  GETMET	R4	R3	K2
      0x5C180400,  //  000B  MOVE	R6	R2
      0x7C100400,  //  000C  CALL	R4	2
      0x70020000,  //  000D  JMP		#000F
      0x00080503,  //  000E  ADD	R2	R2	K3
      0x7001FFF1,  //  000F  JMP		#0002
      0x80000000,  //  0010  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_Session_Store_init,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(sessions),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(Expirable_list),
    /* K3   */  be_nested_str_weak(fabrics),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0xB8060200,  //  0000  GETNGBL	R1	K1
      0x8C040302,  //  0001  GETMET	R1	R1	K2
      0x7C040200,  //  0002  CALL	R1	1
      0x90020001,  //  0003  SETMBR	R0	K0	R1
      0xB8060200,  //  0004  GETNGBL	R1	K1
      0x8C040302,  //  0005  GETMET	R1	R1	K2
      0x7C040200,  //  0006  CALL	R1	1
      0x90020601,  //  0007  SETMBR	R0	K3	R1
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: create_session
********************************************************************/
be_local_closure(Matter_Session_Store_create_session,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(get_session_by_local_session_id),
    /* K1   */  be_nested_str_weak(remove_session),
    /* K2   */  be_nested_str_weak(matter),
    /* K3   */  be_nested_str_weak(Session),
    /* K4   */  be_nested_str_weak(sessions),
    /* K5   */  be_nested_str_weak(push),
    }),
    be_str_weak(create_session),
    &be_const_str_solidified,
    ( &(const binstruction[21]) {  /* code */
      0x8C0C0100,  //  0000  GETMET	R3	R0	K0
      0x5C140200,  //  0001  MOVE	R5	R1
      0x7C0C0400,  //  0002  CALL	R3	2
      0x4C100000,  //  0003  LDNIL	R4
      0x20100604,  //  0004  NE	R4	R3	R4
      0x78120002,  //  0005  JMPF	R4	#0009
      0x8C100101,  //  0006  GETMET	R4	R0	K1
      0x5C180600,  //  0007  MOVE	R6	R3
      0x7C100400,  //  0008  CALL	R4	2
      0xB8120400,  //  0009  GETNGBL	R4	K2
      0x8C100903,  //  000A  GETMET	R4	R4	K3
      0x5C180000,  //  000B  MOVE	R6	R0
      0x5C1C0200,  //  000C  MOVE	R7	R1
      0x5C200400,  //  000D  MOVE	R8	R2
      0x7C100800,  //  000E  CALL	R4	4
      0x5C0C0800,  //  000F  MOVE	R3	R4
      0x88100104,  //  0010  GETMBR	R4	R0	K4
      0x8C100905,  //  0011  GETMET	R4	R4	K5
      0x5C180600,  //  0012  MOVE	R6	R3
      0x7C100400,  //  0013  CALL	R4	2
      0x80040600,  //  0014  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: save_fabrics
********************************************************************/
be_local_closure(Matter_Session_Store_save_fabrics,   /* name */
  be_nested_proto(
    14,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[26]) {     /* constants */
    /* K0   */  be_nested_str_weak(json),
    /* K1   */  be_nested_str_weak(remove_expired),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str_weak(fabrics),
    /* K4   */  be_nested_str_weak(persistables),
    /* K5   */  be_nested_str_weak(_sessions),
    /* K6   */  be_const_int(1),
    /* K7   */  be_nested_str_weak(stop_iteration),
    /* K8   */  be_nested_str_weak(push),
    /* K9   */  be_nested_str_weak(tojson),
    /* K10  */  be_nested_str_weak(_X5B),
    /* K11  */  be_nested_str_weak(concat),
    /* K12  */  be_nested_str_weak(_X2C),
    /* K13  */  be_nested_str_weak(_X5D),
    /* K14  */  be_nested_str_weak(string),
    /* K15  */  be_nested_str_weak(_FABRICS),
    /* K16  */  be_nested_str_weak(w),
    /* K17  */  be_nested_str_weak(write),
    /* K18  */  be_nested_str_weak(close),
    /* K19  */  be_nested_str_weak(tasmota),
    /* K20  */  be_nested_str_weak(log),
    /* K21  */  be_nested_str_weak(format),
    /* K22  */  be_nested_str_weak(MTR_X3A_X20_X3DSaved_X20_X20_X20_X20_X20_X25i_X20fabric_X28s_X29_X20and_X20_X25i_X20session_X28s_X29),
    /* K23  */  be_const_int(2),
    /* K24  */  be_nested_str_weak(MTR_X3A_X20Session_Store_X3A_X3Asave_X20Exception_X3A),
    /* K25  */  be_nested_str_weak(_X7C),
    }),
    be_str_weak(save_fabrics),
    &be_const_str_solidified,
    ( &(const binstruction[84]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080101,  //  0001  GETMET	R2	R0	K1
      0x7C080200,  //  0002  CALL	R2	1
      0x58080002,  //  0003  LDCONST	R2	K2
      0x600C0012,  //  0004  GETGBL	R3	G18
      0x7C0C0000,  //  0005  CALL	R3	0
      0x60100010,  //  0006  GETGBL	R4	G16
      0x88140103,  //  0007  GETMBR	R5	R0	K3
      0x8C140B04,  //  0008  GETMET	R5	R5	K4
      0x7C140200,  //  0009  CALL	R5	1
      0x7C100200,  //  000A  CALL	R4	1
      0xA8020013,  //  000B  EXBLK	0	#0020
      0x5C140800,  //  000C  MOVE	R5	R4
      0x7C140000,  //  000D  CALL	R5	0
      0x60180010,  //  000E  GETGBL	R6	G16
      0x881C0B05,  //  000F  GETMBR	R7	R5	K5
      0x8C1C0F04,  //  0010  GETMET	R7	R7	K4
      0x7C1C0200,  //  0011  CALL	R7	1
      0x7C180200,  //  0012  CALL	R6	1
      0xA8020003,  //  0013  EXBLK	0	#0018
      0x5C1C0C00,  //  0014  MOVE	R7	R6
      0x7C1C0000,  //  0015  CALL	R7	0
      0x00080506,  //  0016  ADD	R2	R2	K6
      0x7001FFFB,  //  0017  JMP		#0014
      0x58180007,  //  0018  LDCONST	R6	K7
      0xAC180200,  //  0019  CATCH	R6	1	0
      0xB0080000,  //  001A  RAISE	2	R0	R0
      0x8C180708,  //  001B  GETMET	R6	R3	K8
      0x8C200B09,  //  001C  GETMET	R8	R5	K9
      0x7C200200,  //  001D  CALL	R8	1
      0x7C180400,  //  001E  CALL	R6	2
      0x7001FFEB,  //  001F  JMP		#000C
      0x58100007,  //  0020  LDCONST	R4	K7
      0xAC100200,  //  0021  CATCH	R4	1	0
      0xB0080000,  //  0022  RAISE	2	R0	R0
      0x6010000C,  //  0023  GETGBL	R4	G12
      0x5C140600,  //  0024  MOVE	R5	R3
      0x7C100200,  //  0025  CALL	R4	1
      0x8C14070B,  //  0026  GETMET	R5	R3	K11
      0x581C000C,  //  0027  LDCONST	R7	K12
      0x7C140400,  //  0028  CALL	R5	2
      0x00161405,  //  0029  ADD	R5	K10	R5
      0x00140B0D,  //  002A  ADD	R5	R5	K13
      0x5C0C0A00,  //  002B  MOVE	R3	R5
      0xA8020014,  //  002C  EXBLK	0	#0042
      0xA4161C00,  //  002D  IMPORT	R5	K14
      0x60180011,  //  002E  GETGBL	R6	G17
      0x881C010F,  //  002F  GETMBR	R7	R0	K15
      0x58200010,  //  0030  LDCONST	R8	K16
      0x7C180400,  //  0031  CALL	R6	2
      0x8C1C0D11,  //  0032  GETMET	R7	R6	K17
      0x5C240600,  //  0033  MOVE	R9	R3
      0x7C1C0400,  //  0034  CALL	R7	2
      0x8C1C0D12,  //  0035  GETMET	R7	R6	K18
      0x7C1C0200,  //  0036  CALL	R7	1
      0xB81E2600,  //  0037  GETNGBL	R7	K19
      0x8C1C0F14,  //  0038  GETMET	R7	R7	K20
      0x8C240B15,  //  0039  GETMET	R9	R5	K21
      0x582C0016,  //  003A  LDCONST	R11	K22
      0x5C300800,  //  003B  MOVE	R12	R4
      0x5C340400,  //  003C  MOVE	R13	R2
      0x7C240800,  //  003D  CALL	R9	4
      0x58280017,  //  003E  LDCONST	R10	K23
      0x7C1C0600,  //  003F  CALL	R7	3
      0xA8040001,  //  0040  EXBLK	1	1
      0x70020010,  //  0041  JMP		#0053
      0xAC140002,  //  0042  CATCH	R5	0	2
      0x7002000D,  //  0043  JMP		#0052
      0xB81E2600,  //  0044  GETNGBL	R7	K19
      0x8C1C0F14,  //  0045  GETMET	R7	R7	K20
      0x60240008,  //  0046  GETGBL	R9	G8
      0x5C280A00,  //  0047  MOVE	R10	R5
      0x7C240200,  //  0048  CALL	R9	1
      0x00263009,  //  0049  ADD	R9	K24	R9
      0x00241319,  //  004A  ADD	R9	R9	K25
      0x60280008,  //  004B  GETGBL	R10	G8
      0x5C2C0C00,  //  004C  MOVE	R11	R6
      0x7C280200,  //  004D  CALL	R10	1
      0x0024120A,  //  004E  ADD	R9	R9	R10
      0x58280017,  //  004F  LDCONST	R10	K23
      0x7C1C0600,  //  0050  CALL	R7	3
      0x70020000,  //  0051  JMP		#0053
      0xB0080000,  //  0052  RAISE	2	R0	R0
      0x80000000,  //  0053  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: load_fabrics
********************************************************************/
be_local_closure(Matter_Session_Store_load_fabrics,   /* name */
  be_nested_proto(
    17,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[29]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(sessions),
    /* K2   */  be_nested_str_weak(matter),
    /* K3   */  be_nested_str_weak(Expirable_list),
    /* K4   */  be_nested_str_weak(fabrics),
    /* K5   */  be_nested_str_weak(_FABRICS),
    /* K6   */  be_nested_str_weak(read),
    /* K7   */  be_nested_str_weak(close),
    /* K8   */  be_nested_str_weak(json),
    /* K9   */  be_nested_str_weak(load),
    /* K10  */  be_nested_str_weak(tasmota),
    /* K11  */  be_nested_str_weak(gc),
    /* K12  */  be_nested_str_weak(Fabric),
    /* K13  */  be_nested_str_weak(fromjson),
    /* K14  */  be_nested_str_weak(set_no_expiration),
    /* K15  */  be_nested_str_weak(set_persist),
    /* K16  */  be_nested_str_weak(find),
    /* K17  */  be_nested_str_weak(_sessions),
    /* K18  */  be_nested_str_weak(Session),
    /* K19  */  be_nested_str_weak(add_session),
    /* K20  */  be_nested_str_weak(stop_iteration),
    /* K21  */  be_nested_str_weak(push),
    /* K22  */  be_nested_str_weak(log),
    /* K23  */  be_nested_str_weak(format),
    /* K24  */  be_nested_str_weak(MTR_X3A_X20Loaded_X20_X25i_X20fabric_X28s_X29),
    /* K25  */  be_const_int(2),
    /* K26  */  be_nested_str_weak(io_error),
    /* K27  */  be_nested_str_weak(MTR_X3A_X20Session_Store_X3A_X3Aload_X20Exception_X3A),
    /* K28  */  be_nested_str_weak(_X7C),
    }),
    be_str_weak(load_fabrics),
    &be_const_str_solidified,
    ( &(const binstruction[118]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA802005F,  //  0001  EXBLK	0	#0062
      0xB80A0400,  //  0002  GETNGBL	R2	K2
      0x8C080503,  //  0003  GETMET	R2	R2	K3
      0x7C080200,  //  0004  CALL	R2	1
      0x90020202,  //  0005  SETMBR	R0	K1	R2
      0xB80A0400,  //  0006  GETNGBL	R2	K2
      0x8C080503,  //  0007  GETMET	R2	R2	K3
      0x7C080200,  //  0008  CALL	R2	1
      0x90020802,  //  0009  SETMBR	R0	K4	R2
      0x60080011,  //  000A  GETGBL	R2	G17
      0x880C0105,  //  000B  GETMBR	R3	R0	K5
      0x7C080200,  //  000C  CALL	R2	1
      0x8C0C0506,  //  000D  GETMET	R3	R2	K6
      0x7C0C0200,  //  000E  CALL	R3	1
      0x8C100507,  //  000F  GETMET	R4	R2	K7
      0x7C100200,  //  0010  CALL	R4	1
      0xA4121000,  //  0011  IMPORT	R4	K8
      0x8C140909,  //  0012  GETMET	R5	R4	K9
      0x5C1C0600,  //  0013  MOVE	R7	R3
      0x7C140400,  //  0014  CALL	R5	2
      0x4C0C0000,  //  0015  LDNIL	R3
      0xB81A1400,  //  0016  GETNGBL	R6	K10
      0x8C180D0B,  //  0017  GETMET	R6	R6	K11
      0x7C180200,  //  0018  CALL	R6	1
      0x60180010,  //  0019  GETGBL	R6	G16
      0x5C1C0A00,  //  001A  MOVE	R7	R5
      0x7C180200,  //  001B  CALL	R6	1
      0xA8020035,  //  001C  EXBLK	0	#0053
      0x5C1C0C00,  //  001D  MOVE	R7	R6
      0x7C1C0000,  //  001E  CALL	R7	0
      0xB8220400,  //  001F  GETNGBL	R8	K2
      0x8820110C,  //  0020  GETMBR	R8	R8	K12
      0x8C20110D,  //  0021  GETMET	R8	R8	K13
      0x5C280000,  //  0022  MOVE	R10	R0
      0x5C2C0E00,  //  0023  MOVE	R11	R7
      0x7C200600,  //  0024  CALL	R8	3
      0x8C24110E,  //  0025  GETMET	R9	R8	K14
      0x7C240200,  //  0026  CALL	R9	1
      0x8C24110F,  //  0027  GETMET	R9	R8	K15
      0x502C0200,  //  0028  LDBOOL	R11	1	0
      0x7C240400,  //  0029  CALL	R9	2
      0x8C240F10,  //  002A  GETMET	R9	R7	K16
      0x582C0011,  //  002B  LDCONST	R11	K17
      0x60300012,  //  002C  GETGBL	R12	G18
      0x7C300000,  //  002D  CALL	R12	0
      0x7C240600,  //  002E  CALL	R9	3
      0x60280010,  //  002F  GETGBL	R10	G16
      0x5C2C1200,  //  0030  MOVE	R11	R9
      0x7C280200,  //  0031  CALL	R10	1
      0xA8020017,  //  0032  EXBLK	0	#004B
      0x5C2C1400,  //  0033  MOVE	R11	R10
      0x7C2C0000,  //  0034  CALL	R11	0
      0xB8320400,  //  0035  GETNGBL	R12	K2
      0x88301912,  //  0036  GETMBR	R12	R12	K18
      0x8C30190D,  //  0037  GETMET	R12	R12	K13
      0x5C380000,  //  0038  MOVE	R14	R0
      0x5C3C1600,  //  0039  MOVE	R15	R11
      0x5C401000,  //  003A  MOVE	R16	R8
      0x7C300800,  //  003B  CALL	R12	4
      0x4C340000,  //  003C  LDNIL	R13
      0x2034180D,  //  003D  NE	R13	R12	R13
      0x7836000A,  //  003E  JMPF	R13	#004A
      0x8C34190E,  //  003F  GETMET	R13	R12	K14
      0x7C340200,  //  0040  CALL	R13	1
      0x8C34190F,  //  0041  GETMET	R13	R12	K15
      0x503C0200,  //  0042  LDBOOL	R15	1	0
      0x7C340400,  //  0043  CALL	R13	2
      0x8C340113,  //  0044  GETMET	R13	R0	K19
      0x5C3C1800,  //  0045  MOVE	R15	R12
      0x7C340400,  //  0046  CALL	R13	2
      0x8C341113,  //  0047  GETMET	R13	R8	K19
      0x5C3C1800,  //  0048  MOVE	R15	R12
      0x7C340400,  //  0049  CALL	R13	2
      0x7001FFE7,  //  004A  JMP		#0033
      0x58280014,  //  004B  LDCONST	R10	K20
      0xAC280200,  //  004C  CATCH	R10	1	0
      0xB0080000,  //  004D  RAISE	2	R0	R0
      0x88280104,  //  004E  GETMBR	R10	R0	K4
      0x8C281515,  //  004F  GETMET	R10	R10	K21
      0x5C301000,  //  0050  MOVE	R12	R8
      0x7C280400,  //  0051  CALL	R10	2
      0x7001FFC9,  //  0052  JMP		#001D
      0x58180014,  //  0053  LDCONST	R6	K20
      0xAC180200,  //  0054  CATCH	R6	1	0
      0xB0080000,  //  0055  RAISE	2	R0	R0
      0xB81A1400,  //  0056  GETNGBL	R6	K10
      0x8C180D16,  //  0057  GETMET	R6	R6	K22
      0x8C200317,  //  0058  GETMET	R8	R1	K23
      0x58280018,  //  0059  LDCONST	R10	K24
      0x602C000C,  //  005A  GETGBL	R11	G12
      0x88300104,  //  005B  GETMBR	R12	R0	K4
      0x7C2C0200,  //  005C  CALL	R11	1
      0x7C200600,  //  005D  CALL	R8	3
      0x58240019,  //  005E  LDCONST	R9	K25
      0x7C180600,  //  005F  CALL	R6	3
      0xA8040001,  //  0060  EXBLK	1	1
      0x70020012,  //  0061  JMP		#0075
      0xAC080002,  //  0062  CATCH	R2	0	2
      0x7002000F,  //  0063  JMP		#0074
      0x2010051A,  //  0064  NE	R4	R2	K26
      0x7812000C,  //  0065  JMPF	R4	#0073
      0xB8121400,  //  0066  GETNGBL	R4	K10
      0x8C100916,  //  0067  GETMET	R4	R4	K22
      0x60180008,  //  0068  GETGBL	R6	G8
      0x5C1C0400,  //  0069  MOVE	R7	R2
      0x7C180200,  //  006A  CALL	R6	1
      0x001A3606,  //  006B  ADD	R6	K27	R6
      0x00180D1C,  //  006C  ADD	R6	R6	K28
      0x601C0008,  //  006D  GETGBL	R7	G8
      0x5C200600,  //  006E  MOVE	R8	R3
      0x7C1C0200,  //  006F  CALL	R7	1
      0x00180C07,  //  0070  ADD	R6	R6	R7
      0x581C0019,  //  0071  LDCONST	R7	K25
      0x7C100600,  //  0072  CALL	R4	3
      0x70020000,  //  0073  JMP		#0075
      0xB0080000,  //  0074  RAISE	2	R0	R0
      0x80000000,  //  0075  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: sessions_active
********************************************************************/
be_local_closure(Matter_Session_Store_sessions_active,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(sessions),
    /* K2   */  be_nested_str_weak(get_device_id),
    /* K3   */  be_nested_str_weak(get_fabric_id),
    /* K4   */  be_nested_str_weak(push),
    /* K5   */  be_const_int(1),
    }),
    be_str_weak(sessions_active),
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0x60040012,  //  0000  GETGBL	R1	G18
      0x7C040000,  //  0001  CALL	R1	0
      0x58080000,  //  0002  LDCONST	R2	K0
      0x600C000C,  //  0003  GETGBL	R3	G12
      0x88100101,  //  0004  GETMBR	R4	R0	K1
      0x7C0C0200,  //  0005  CALL	R3	1
      0x140C0403,  //  0006  LT	R3	R2	R3
      0x780E000C,  //  0007  JMPF	R3	#0015
      0x880C0101,  //  0008  GETMBR	R3	R0	K1
      0x940C0602,  //  0009  GETIDX	R3	R3	R2
      0x8C100702,  //  000A  GETMET	R4	R3	K2
      0x7C100200,  //  000B  CALL	R4	1
      0x78120005,  //  000C  JMPF	R4	#0013
      0x8C100703,  //  000D  GETMET	R4	R3	K3
      0x7C100200,  //  000E  CALL	R4	1
      0x78120002,  //  000F  JMPF	R4	#0013
      0x8C100304,  //  0010  GETMET	R4	R1	K4
      0x5C180600,  //  0011  MOVE	R6	R3
      0x7C100400,  //  0012  CALL	R4	2
      0x00080505,  //  0013  ADD	R2	R2	K5
      0x7001FFED,  //  0014  JMP		#0003
      0x80040200,  //  0015  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: create_fabric
********************************************************************/
be_local_closure(Matter_Session_Store_create_fabric,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(Fabric),
    }),
    be_str_weak(create_fabric),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x80040200,  //  0004  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: add_fabric
********************************************************************/
be_local_closure(Matter_Session_Store_add_fabric,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(Fabric),
    /* K2   */  be_nested_str_weak(value_error),
    /* K3   */  be_nested_str_weak(must_X20be_X20of_X20class_X20matter_X2EFabric),
    /* K4   */  be_nested_str_weak(fabrics),
    /* K5   */  be_nested_str_weak(find),
    /* K6   */  be_nested_str_weak(remove_redundant_fabric),
    /* K7   */  be_nested_str_weak(push),
    }),
    be_str_weak(add_fabric),
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0x6008000F,  //  0000  GETGBL	R2	G15
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0xB8120000,  //  0002  GETNGBL	R4	K0
      0x88100901,  //  0003  GETMBR	R4	R4	K1
      0x7C080400,  //  0004  CALL	R2	2
      0x740A0000,  //  0005  JMPT	R2	#0007
      0xB0060503,  //  0006  RAISE	1	K2	K3
      0x88080104,  //  0007  GETMBR	R2	R0	K4
      0x8C080505,  //  0008  GETMET	R2	R2	K5
      0x5C100200,  //  0009  MOVE	R4	R1
      0x7C080400,  //  000A  CALL	R2	2
      0x4C0C0000,  //  000B  LDNIL	R3
      0x1C080403,  //  000C  EQ	R2	R2	R3
      0x780A0006,  //  000D  JMPF	R2	#0015
      0x8C080106,  //  000E  GETMET	R2	R0	K6
      0x5C100200,  //  000F  MOVE	R4	R1
      0x7C080400,  //  0010  CALL	R2	2
      0x88080104,  //  0011  GETMBR	R2	R0	K4
      0x8C080507,  //  0012  GETMET	R2	R2	K7
      0x5C100200,  //  0013  MOVE	R4	R1
      0x7C080400,  //  0014  CALL	R2	2
      0x80000000,  //  0015  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Session_Store
********************************************************************/
be_local_class(Matter_Session_Store,
    2,
    NULL,
    be_nested_map(26,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(gen_local_session_id, 25), be_const_closure(Matter_Session_Store_gen_local_session_id_closure) },
        { be_const_key_weak(remove_redundant_fabric, 15), be_const_closure(Matter_Session_Store_remove_redundant_fabric_closure) },
        { be_const_key_weak(find_session_by_resumption_id, -1), be_const_closure(Matter_Session_Store_find_session_by_resumption_id_closure) },
        { be_const_key_weak(fabrics, 5), be_const_var(1) },
        { be_const_key_weak(add_fabric, -1), be_const_closure(Matter_Session_Store_add_fabric_closure) },
        { be_const_key_weak(create_fabric, -1), be_const_closure(Matter_Session_Store_create_fabric_closure) },
        { be_const_key_weak(sessions_active, -1), be_const_closure(Matter_Session_Store_sessions_active_closure) },
        { be_const_key_weak(add_session, -1), be_const_closure(Matter_Session_Store_add_session_closure) },
        { be_const_key_weak(count_active_fabrics, -1), be_const_closure(Matter_Session_Store_count_active_fabrics_closure) },
        { be_const_key_weak(remove_expired, 17), be_const_closure(Matter_Session_Store_remove_expired_closure) },
        { be_const_key_weak(_FABRICS, -1), be_nested_str_weak(_matter_fabrics_X2Ejson) },
        { be_const_key_weak(find_fabric_by_index, -1), be_const_closure(Matter_Session_Store_find_fabric_by_index_closure) },
        { be_const_key_weak(sessions, 4), be_const_var(0) },
        { be_const_key_weak(get_session_by_source_node_id, 11), be_const_closure(Matter_Session_Store_get_session_by_source_node_id_closure) },
        { be_const_key_weak(next_fabric_idx, -1), be_const_closure(Matter_Session_Store_next_fabric_idx_closure) },
        { be_const_key_weak(get_session_by_local_session_id, 24), be_const_closure(Matter_Session_Store_get_session_by_local_session_id_closure) },
        { be_const_key_weak(active_fabrics, -1), be_const_closure(Matter_Session_Store_active_fabrics_closure) },
        { be_const_key_weak(every_second, -1), be_const_closure(Matter_Session_Store_every_second_closure) },
        { be_const_key_weak(remove_session, -1), be_const_closure(Matter_Session_Store_remove_session_closure) },
        { be_const_key_weak(init, -1), be_const_closure(Matter_Session_Store_init_closure) },
        { be_const_key_weak(create_session, -1), be_const_closure(Matter_Session_Store_create_session_closure) },
        { be_const_key_weak(save_fabrics, -1), be_const_closure(Matter_Session_Store_save_fabrics_closure) },
        { be_const_key_weak(load_fabrics, -1), be_const_closure(Matter_Session_Store_load_fabrics_closure) },
        { be_const_key_weak(remove_fabric, 6), be_const_closure(Matter_Session_Store_remove_fabric_closure) },
        { be_const_key_weak(find_children_fabrics, -1), be_const_closure(Matter_Session_Store_find_children_fabrics_closure) },
        { be_const_key_weak(find_session_source_id_unsecure, -1), be_const_closure(Matter_Session_Store_find_session_source_id_unsecure_closure) },
    })),
    be_str_weak(Matter_Session_Store)
);
/*******************************************************************/

void be_load_Matter_Session_Store_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Session_Store);
    be_setglobal(vm, "Matter_Session_Store");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
