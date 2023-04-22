/* Solidification of Matter_Session_Store.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Session_Store;

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
** Solidified function: init
********************************************************************/
be_local_closure(Matter_Session_Store_init,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(device),
    /* K1   */  be_nested_str_weak(sessions),
    /* K2   */  be_nested_str_weak(matter),
    /* K3   */  be_nested_str_weak(Expirable_list),
    /* K4   */  be_nested_str_weak(fabrics),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0xB80A0400,  //  0001  GETNGBL	R2	K2
      0x8C080503,  //  0002  GETMET	R2	R2	K3
      0x7C080200,  //  0003  CALL	R2	1
      0x90020202,  //  0004  SETMBR	R0	K1	R2
      0xB80A0400,  //  0005  GETNGBL	R2	K2
      0x8C080503,  //  0006  GETMET	R2	R2	K3
      0x7C080200,  //  0007  CALL	R2	1
      0x90020802,  //  0008  SETMBR	R0	K4	R2
      0x80000000,  //  0009  RET	0
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
    ( &(const bvalue[13]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_const_int(0),
    /* K2   */  be_nested_str_weak(sessions),
    /* K3   */  be_nested_str_weak(tasmota),
    /* K4   */  be_nested_str_weak(log),
    /* K5   */  be_nested_str_weak(format),
    /* K6   */  be_nested_str_weak(MTR_X3A_X20session_X2Eresumption_id_X3D_X25s_X20vs_X20_X25s),
    /* K7   */  be_nested_str_weak(resumption_id),
    /* K8   */  be_const_int(3),
    /* K9   */  be_nested_str_weak(shared_secret),
    /* K10  */  be_nested_str_weak(MTR_X3A_X20session_X2Eshared_secret_X3D_X25s),
    /* K11  */  be_nested_str_weak(update),
    /* K12  */  be_const_int(1),
    }),
    be_str_weak(find_session_by_resumption_id),
    &be_const_str_solidified,
    ( &(const binstruction[49]) {  /* code */
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
      0x78160023,  //  000B  JMPF	R5	#0030
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
      0x58240008,  //  0018  LDCONST	R9	K8
      0x7C180600,  //  0019  CALL	R6	3
      0x88180B07,  //  001A  GETMBR	R6	R5	K7
      0x1C180C01,  //  001B  EQ	R6	R6	R1
      0x781A0010,  //  001C  JMPF	R6	#002E
      0x88180B09,  //  001D  GETMBR	R6	R5	K9
      0x4C1C0000,  //  001E  LDNIL	R7
      0x20180C07,  //  001F  NE	R6	R6	R7
      0x781A000C,  //  0020  JMPF	R6	#002E
      0xB81A0600,  //  0021  GETNGBL	R6	K3
      0x8C180D04,  //  0022  GETMET	R6	R6	K4
      0x8C200505,  //  0023  GETMET	R8	R2	K5
      0x5828000A,  //  0024  LDCONST	R10	K10
      0x602C0008,  //  0025  GETGBL	R11	G8
      0x88300B09,  //  0026  GETMBR	R12	R5	K9
      0x7C2C0200,  //  0027  CALL	R11	1
      0x7C200600,  //  0028  CALL	R8	3
      0x58240008,  //  0029  LDCONST	R9	K8
      0x7C180600,  //  002A  CALL	R6	3
      0x8C180B0B,  //  002B  GETMET	R6	R5	K11
      0x7C180200,  //  002C  CALL	R6	1
      0x80040A00,  //  002D  RET	1	R5
      0x000C070C,  //  002E  ADD	R3	R3	K12
      0x7001FFD6,  //  002F  JMP		#0007
      0x80000000,  //  0030  RET	0
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
    ( &(const bvalue[28]) {     /* constants */
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
    /* K24  */  be_nested_str_weak(device),
    /* K25  */  be_nested_str_weak(event_fabrics_saved),
    /* K26  */  be_nested_str_weak(MTR_X3A_X20Session_Store_X3A_X3Asave_X20Exception_X3A),
    /* K27  */  be_nested_str_weak(_X7C),
    }),
    be_str_weak(save_fabrics),
    &be_const_str_solidified,
    ( &(const binstruction[87]) {  /* code */
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
      0xA8020017,  //  002C  EXBLK	0	#0045
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
      0x881C0118,  //  0040  GETMBR	R7	R0	K24
      0x8C1C0F19,  //  0041  GETMET	R7	R7	K25
      0x7C1C0200,  //  0042  CALL	R7	1
      0xA8040001,  //  0043  EXBLK	1	1
      0x70020010,  //  0044  JMP		#0056
      0xAC140002,  //  0045  CATCH	R5	0	2
      0x7002000D,  //  0046  JMP		#0055
      0xB81E2600,  //  0047  GETNGBL	R7	K19
      0x8C1C0F14,  //  0048  GETMET	R7	R7	K20
      0x60240008,  //  0049  GETGBL	R9	G8
      0x5C280A00,  //  004A  MOVE	R10	R5
      0x7C240200,  //  004B  CALL	R9	1
      0x00263409,  //  004C  ADD	R9	K26	R9
      0x0024131B,  //  004D  ADD	R9	R9	K27
      0x60280008,  //  004E  GETGBL	R10	G8
      0x5C2C0C00,  //  004F  MOVE	R11	R6
      0x7C280200,  //  0050  CALL	R10	1
      0x0024120A,  //  0051  ADD	R9	R9	R10
      0x58280017,  //  0052  LDCONST	R10	K23
      0x7C1C0600,  //  0053  CALL	R7	3
      0x70020000,  //  0054  JMP		#0056
      0xB0080000,  //  0055  RAISE	2	R0	R0
      0x80000000,  //  0056  RET	0
    })
  )
);
/*******************************************************************/


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
** Solidified class: Matter_Session_Store
********************************************************************/
be_local_class(Matter_Session_Store,
    3,
    NULL,
    be_nested_map(27,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(gen_local_session_id, -1), be_const_closure(Matter_Session_Store_gen_local_session_id_closure) },
        { be_const_key_weak(create_fabric, -1), be_const_closure(Matter_Session_Store_create_fabric_closure) },
        { be_const_key_weak(get_session_by_source_node_id, -1), be_const_closure(Matter_Session_Store_get_session_by_source_node_id_closure) },
        { be_const_key_weak(save_fabrics, -1), be_const_closure(Matter_Session_Store_save_fabrics_closure) },
        { be_const_key_weak(active_fabrics, 10), be_const_closure(Matter_Session_Store_active_fabrics_closure) },
        { be_const_key_weak(find_session_by_resumption_id, 20), be_const_closure(Matter_Session_Store_find_session_by_resumption_id_closure) },
        { be_const_key_weak(sessions_active, -1), be_const_closure(Matter_Session_Store_sessions_active_closure) },
        { be_const_key_weak(sessions, -1), be_const_var(1) },
        { be_const_key_weak(fabrics, -1), be_const_var(2) },
        { be_const_key_weak(load_fabrics, 6), be_const_closure(Matter_Session_Store_load_fabrics_closure) },
        { be_const_key_weak(find_session_source_id_unsecure, 23), be_const_closure(Matter_Session_Store_find_session_source_id_unsecure_closure) },
        { be_const_key_weak(_FABRICS, -1), be_nested_str_weak(_matter_fabrics_X2Ejson) },
        { be_const_key_weak(remove_session, -1), be_const_closure(Matter_Session_Store_remove_session_closure) },
        { be_const_key_weak(remove_fabric, 22), be_const_closure(Matter_Session_Store_remove_fabric_closure) },
        { be_const_key_weak(find_fabric_by_index, 19), be_const_closure(Matter_Session_Store_find_fabric_by_index_closure) },
        { be_const_key_weak(remove_expired, -1), be_const_closure(Matter_Session_Store_remove_expired_closure) },
        { be_const_key_weak(add_session, -1), be_const_closure(Matter_Session_Store_add_session_closure) },
        { be_const_key_weak(remove_redundant_fabric, 11), be_const_closure(Matter_Session_Store_remove_redundant_fabric_closure) },
        { be_const_key_weak(create_session, -1), be_const_closure(Matter_Session_Store_create_session_closure) },
        { be_const_key_weak(get_session_by_local_session_id, -1), be_const_closure(Matter_Session_Store_get_session_by_local_session_id_closure) },
        { be_const_key_weak(init, -1), be_const_closure(Matter_Session_Store_init_closure) },
        { be_const_key_weak(every_second, -1), be_const_closure(Matter_Session_Store_every_second_closure) },
        { be_const_key_weak(add_fabric, -1), be_const_closure(Matter_Session_Store_add_fabric_closure) },
        { be_const_key_weak(count_active_fabrics, -1), be_const_closure(Matter_Session_Store_count_active_fabrics_closure) },
        { be_const_key_weak(device, 5), be_const_var(0) },
        { be_const_key_weak(next_fabric_idx, 3), be_const_closure(Matter_Session_Store_next_fabric_idx_closure) },
        { be_const_key_weak(find_children_fabrics, 0), be_const_closure(Matter_Session_Store_find_children_fabrics_closure) },
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
