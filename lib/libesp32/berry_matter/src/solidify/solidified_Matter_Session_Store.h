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
    16,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[27]) {     /* constants */
    /* K0   */  be_nested_str_weak(sessions),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(Expirable_list),
    /* K3   */  be_nested_str_weak(fabrics),
    /* K4   */  be_nested_str_weak(_FABRICS),
    /* K5   */  be_nested_str_weak(read),
    /* K6   */  be_nested_str_weak(close),
    /* K7   */  be_nested_str_weak(json),
    /* K8   */  be_nested_str_weak(load),
    /* K9   */  be_nested_str_weak(tasmota),
    /* K10  */  be_nested_str_weak(gc),
    /* K11  */  be_nested_str_weak(Fabric),
    /* K12  */  be_nested_str_weak(fromjson),
    /* K13  */  be_nested_str_weak(set_no_expiration),
    /* K14  */  be_nested_str_weak(set_persist),
    /* K15  */  be_nested_str_weak(find),
    /* K16  */  be_nested_str_weak(_sessions),
    /* K17  */  be_nested_str_weak(Session),
    /* K18  */  be_nested_str_weak(add_session),
    /* K19  */  be_nested_str_weak(stop_iteration),
    /* K20  */  be_nested_str_weak(push),
    /* K21  */  be_nested_str_weak(log),
    /* K22  */  be_nested_str_weak(MTR_X3A_X20Loaded_X20_X25i_X20fabric_X28s_X29),
    /* K23  */  be_const_int(2),
    /* K24  */  be_nested_str_weak(io_error),
    /* K25  */  be_nested_str_weak(MTR_X3A_X20Session_Store_X3A_X3Aload_X20Exception_X3A),
    /* K26  */  be_nested_str_weak(_X7C),
    }),
    be_str_weak(load_fabrics),
    &be_const_str_solidified,
    ( &(const binstruction[117]) {  /* code */
      0xA802005F,  //  0000  EXBLK	0	#0061
      0xB8060200,  //  0001  GETNGBL	R1	K1
      0x8C040302,  //  0002  GETMET	R1	R1	K2
      0x7C040200,  //  0003  CALL	R1	1
      0x90020001,  //  0004  SETMBR	R0	K0	R1
      0xB8060200,  //  0005  GETNGBL	R1	K1
      0x8C040302,  //  0006  GETMET	R1	R1	K2
      0x7C040200,  //  0007  CALL	R1	1
      0x90020601,  //  0008  SETMBR	R0	K3	R1
      0x60040011,  //  0009  GETGBL	R1	G17
      0x88080104,  //  000A  GETMBR	R2	R0	K4
      0x7C040200,  //  000B  CALL	R1	1
      0x8C080305,  //  000C  GETMET	R2	R1	K5
      0x7C080200,  //  000D  CALL	R2	1
      0x8C0C0306,  //  000E  GETMET	R3	R1	K6
      0x7C0C0200,  //  000F  CALL	R3	1
      0xA40E0E00,  //  0010  IMPORT	R3	K7
      0x8C100708,  //  0011  GETMET	R4	R3	K8
      0x5C180400,  //  0012  MOVE	R6	R2
      0x7C100400,  //  0013  CALL	R4	2
      0x4C080000,  //  0014  LDNIL	R2
      0xB8161200,  //  0015  GETNGBL	R5	K9
      0x8C140B0A,  //  0016  GETMET	R5	R5	K10
      0x7C140200,  //  0017  CALL	R5	1
      0x60140010,  //  0018  GETGBL	R5	G16
      0x5C180800,  //  0019  MOVE	R6	R4
      0x7C140200,  //  001A  CALL	R5	1
      0xA8020035,  //  001B  EXBLK	0	#0052
      0x5C180A00,  //  001C  MOVE	R6	R5
      0x7C180000,  //  001D  CALL	R6	0
      0xB81E0200,  //  001E  GETNGBL	R7	K1
      0x881C0F0B,  //  001F  GETMBR	R7	R7	K11
      0x8C1C0F0C,  //  0020  GETMET	R7	R7	K12
      0x5C240000,  //  0021  MOVE	R9	R0
      0x5C280C00,  //  0022  MOVE	R10	R6
      0x7C1C0600,  //  0023  CALL	R7	3
      0x8C200F0D,  //  0024  GETMET	R8	R7	K13
      0x7C200200,  //  0025  CALL	R8	1
      0x8C200F0E,  //  0026  GETMET	R8	R7	K14
      0x50280200,  //  0027  LDBOOL	R10	1	0
      0x7C200400,  //  0028  CALL	R8	2
      0x8C200D0F,  //  0029  GETMET	R8	R6	K15
      0x58280010,  //  002A  LDCONST	R10	K16
      0x602C0012,  //  002B  GETGBL	R11	G18
      0x7C2C0000,  //  002C  CALL	R11	0
      0x7C200600,  //  002D  CALL	R8	3
      0x60240010,  //  002E  GETGBL	R9	G16
      0x5C281000,  //  002F  MOVE	R10	R8
      0x7C240200,  //  0030  CALL	R9	1
      0xA8020017,  //  0031  EXBLK	0	#004A
      0x5C281200,  //  0032  MOVE	R10	R9
      0x7C280000,  //  0033  CALL	R10	0
      0xB82E0200,  //  0034  GETNGBL	R11	K1
      0x882C1711,  //  0035  GETMBR	R11	R11	K17
      0x8C2C170C,  //  0036  GETMET	R11	R11	K12
      0x5C340000,  //  0037  MOVE	R13	R0
      0x5C381400,  //  0038  MOVE	R14	R10
      0x5C3C0E00,  //  0039  MOVE	R15	R7
      0x7C2C0800,  //  003A  CALL	R11	4
      0x4C300000,  //  003B  LDNIL	R12
      0x2030160C,  //  003C  NE	R12	R11	R12
      0x7832000A,  //  003D  JMPF	R12	#0049
      0x8C30170D,  //  003E  GETMET	R12	R11	K13
      0x7C300200,  //  003F  CALL	R12	1
      0x8C30170E,  //  0040  GETMET	R12	R11	K14
      0x50380200,  //  0041  LDBOOL	R14	1	0
      0x7C300400,  //  0042  CALL	R12	2
      0x8C300112,  //  0043  GETMET	R12	R0	K18
      0x5C381600,  //  0044  MOVE	R14	R11
      0x7C300400,  //  0045  CALL	R12	2
      0x8C300F12,  //  0046  GETMET	R12	R7	K18
      0x5C381600,  //  0047  MOVE	R14	R11
      0x7C300400,  //  0048  CALL	R12	2
      0x7001FFE7,  //  0049  JMP		#0032
      0x58240013,  //  004A  LDCONST	R9	K19
      0xAC240200,  //  004B  CATCH	R9	1	0
      0xB0080000,  //  004C  RAISE	2	R0	R0
      0x88240103,  //  004D  GETMBR	R9	R0	K3
      0x8C241314,  //  004E  GETMET	R9	R9	K20
      0x5C2C0E00,  //  004F  MOVE	R11	R7
      0x7C240400,  //  0050  CALL	R9	2
      0x7001FFC9,  //  0051  JMP		#001C
      0x58140013,  //  0052  LDCONST	R5	K19
      0xAC140200,  //  0053  CATCH	R5	1	0
      0xB0080000,  //  0054  RAISE	2	R0	R0
      0xB8161200,  //  0055  GETNGBL	R5	K9
      0x8C140B15,  //  0056  GETMET	R5	R5	K21
      0x601C0018,  //  0057  GETGBL	R7	G24
      0x58200016,  //  0058  LDCONST	R8	K22
      0x6024000C,  //  0059  GETGBL	R9	G12
      0x88280103,  //  005A  GETMBR	R10	R0	K3
      0x7C240200,  //  005B  CALL	R9	1
      0x7C1C0400,  //  005C  CALL	R7	2
      0x58200017,  //  005D  LDCONST	R8	K23
      0x7C140600,  //  005E  CALL	R5	3
      0xA8040001,  //  005F  EXBLK	1	1
      0x70020012,  //  0060  JMP		#0074
      0xAC040002,  //  0061  CATCH	R1	0	2
      0x7002000F,  //  0062  JMP		#0073
      0x200C0318,  //  0063  NE	R3	R1	K24
      0x780E000C,  //  0064  JMPF	R3	#0072
      0xB80E1200,  //  0065  GETNGBL	R3	K9
      0x8C0C0715,  //  0066  GETMET	R3	R3	K21
      0x60140008,  //  0067  GETGBL	R5	G8
      0x5C180200,  //  0068  MOVE	R6	R1
      0x7C140200,  //  0069  CALL	R5	1
      0x00163205,  //  006A  ADD	R5	K25	R5
      0x00140B1A,  //  006B  ADD	R5	R5	K26
      0x60180008,  //  006C  GETGBL	R6	G8
      0x5C1C0400,  //  006D  MOVE	R7	R2
      0x7C180200,  //  006E  CALL	R6	1
      0x00140A06,  //  006F  ADD	R5	R5	R6
      0x58180017,  //  0070  LDCONST	R6	K23
      0x7C0C0600,  //  0071  CALL	R3	3
      0x70020000,  //  0072  JMP		#0074
      0xB0080000,  //  0073  RAISE	2	R0	R0
      0x80000000,  //  0074  RET	0
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
    12,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(sessions),
    /* K2   */  be_nested_str_weak(tasmota),
    /* K3   */  be_nested_str_weak(log),
    /* K4   */  be_nested_str_weak(MTR_X3A_X20session_X2Eresumption_id_X3D_X25s_X20vs_X20_X25s),
    /* K5   */  be_nested_str_weak(resumption_id),
    /* K6   */  be_nested_str_weak(shared_secret),
    /* K7   */  be_nested_str_weak(update),
    /* K8   */  be_const_int(1),
    }),
    be_str_weak(find_session_by_resumption_id),
    &be_const_str_solidified,
    ( &(const binstruction[38]) {  /* code */
      0x5C080200,  //  0000  MOVE	R2	R1
      0x740A0001,  //  0001  JMPT	R2	#0004
      0x4C080000,  //  0002  LDNIL	R2
      0x80040400,  //  0003  RET	1	R2
      0x58080000,  //  0004  LDCONST	R2	K0
      0x880C0101,  //  0005  GETMBR	R3	R0	K1
      0x6010000C,  //  0006  GETGBL	R4	G12
      0x5C140600,  //  0007  MOVE	R5	R3
      0x7C100200,  //  0008  CALL	R4	1
      0x14100404,  //  0009  LT	R4	R2	R4
      0x78120019,  //  000A  JMPF	R4	#0025
      0x94100602,  //  000B  GETIDX	R4	R3	R2
      0xB8160400,  //  000C  GETNGBL	R5	K2
      0x8C140B03,  //  000D  GETMET	R5	R5	K3
      0x601C0018,  //  000E  GETGBL	R7	G24
      0x58200004,  //  000F  LDCONST	R8	K4
      0x60240008,  //  0010  GETGBL	R9	G8
      0x88280905,  //  0011  GETMBR	R10	R4	K5
      0x7C240200,  //  0012  CALL	R9	1
      0x60280008,  //  0013  GETGBL	R10	G8
      0x5C2C0200,  //  0014  MOVE	R11	R1
      0x7C280200,  //  0015  CALL	R10	1
      0x7C1C0600,  //  0016  CALL	R7	3
      0x54220003,  //  0017  LDINT	R8	4
      0x7C140600,  //  0018  CALL	R5	3
      0x88140905,  //  0019  GETMBR	R5	R4	K5
      0x1C140A01,  //  001A  EQ	R5	R5	R1
      0x78160006,  //  001B  JMPF	R5	#0023
      0x88140906,  //  001C  GETMBR	R5	R4	K6
      0x4C180000,  //  001D  LDNIL	R6
      0x20140A06,  //  001E  NE	R5	R5	R6
      0x78160002,  //  001F  JMPF	R5	#0023
      0x8C140907,  //  0020  GETMET	R5	R4	K7
      0x7C140200,  //  0021  CALL	R5	1
      0x80040800,  //  0022  RET	1	R4
      0x00080508,  //  0023  ADD	R2	R2	K8
      0x7001FFE0,  //  0024  JMP		#0006
      0x80000000,  //  0025  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: save_fabrics
********************************************************************/
be_local_closure(Matter_Session_Store_save_fabrics,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[24]) {     /* constants */
    /* K0   */  be_nested_str_weak(json),
    /* K1   */  be_nested_str_weak(remove_expired),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str_weak(_FABRICS),
    /* K4   */  be_nested_str_weak(w),
    /* K5   */  be_nested_str_weak(write),
    /* K6   */  be_nested_str_weak(_X5B),
    /* K7   */  be_nested_str_weak(fabrics),
    /* K8   */  be_nested_str_weak(persistables),
    /* K9   */  be_nested_str_weak(_sessions),
    /* K10  */  be_const_int(1),
    /* K11  */  be_nested_str_weak(stop_iteration),
    /* K12  */  be_nested_str_weak(_X2C),
    /* K13  */  be_nested_str_weak(tojson),
    /* K14  */  be_nested_str_weak(_X5D),
    /* K15  */  be_nested_str_weak(close),
    /* K16  */  be_nested_str_weak(tasmota),
    /* K17  */  be_nested_str_weak(log),
    /* K18  */  be_nested_str_weak(MTR_X3A_X20_X3DSaved_X20_X20_X20_X20_X20_X25s_X20fabric_X28s_X29_X20and_X20_X25s_X20session_X28s_X29),
    /* K19  */  be_const_int(2),
    /* K20  */  be_nested_str_weak(device),
    /* K21  */  be_nested_str_weak(event_fabrics_saved),
    /* K22  */  be_nested_str_weak(MTR_X3A_X20Session_Store_X3A_X3Asave_X20Exception_X3A),
    /* K23  */  be_nested_str_weak(_X7C),
    }),
    be_str_weak(save_fabrics),
    &be_const_str_solidified,
    ( &(const binstruction[86]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA8020041,  //  0001  EXBLK	0	#0044
      0x8C080101,  //  0002  GETMET	R2	R0	K1
      0x7C080200,  //  0003  CALL	R2	1
      0x58080002,  //  0004  LDCONST	R2	K2
      0x580C0002,  //  0005  LDCONST	R3	K2
      0x60100011,  //  0006  GETGBL	R4	G17
      0x88140103,  //  0007  GETMBR	R5	R0	K3
      0x58180004,  //  0008  LDCONST	R6	K4
      0x7C100400,  //  0009  CALL	R4	2
      0x8C140905,  //  000A  GETMET	R5	R4	K5
      0x581C0006,  //  000B  LDCONST	R7	K6
      0x7C140400,  //  000C  CALL	R5	2
      0x60140010,  //  000D  GETGBL	R5	G16
      0x88180107,  //  000E  GETMBR	R6	R0	K7
      0x8C180D08,  //  000F  GETMET	R6	R6	K8
      0x7C180200,  //  0010  CALL	R6	1
      0x7C140200,  //  0011  CALL	R5	1
      0xA802001A,  //  0012  EXBLK	0	#002E
      0x5C180A00,  //  0013  MOVE	R6	R5
      0x7C180000,  //  0014  CALL	R6	0
      0x601C0010,  //  0015  GETGBL	R7	G16
      0x88200D09,  //  0016  GETMBR	R8	R6	K9
      0x8C201108,  //  0017  GETMET	R8	R8	K8
      0x7C200200,  //  0018  CALL	R8	1
      0x7C1C0200,  //  0019  CALL	R7	1
      0xA8020003,  //  001A  EXBLK	0	#001F
      0x5C200E00,  //  001B  MOVE	R8	R7
      0x7C200000,  //  001C  CALL	R8	0
      0x0008050A,  //  001D  ADD	R2	R2	K10
      0x7001FFFB,  //  001E  JMP		#001B
      0x581C000B,  //  001F  LDCONST	R7	K11
      0xAC1C0200,  //  0020  CATCH	R7	1	0
      0xB0080000,  //  0021  RAISE	2	R0	R0
      0x241C0702,  //  0022  GT	R7	R3	K2
      0x781E0002,  //  0023  JMPF	R7	#0027
      0x8C1C0905,  //  0024  GETMET	R7	R4	K5
      0x5824000C,  //  0025  LDCONST	R9	K12
      0x7C1C0400,  //  0026  CALL	R7	2
      0x8C1C0D0D,  //  0027  GETMET	R7	R6	K13
      0x7C1C0200,  //  0028  CALL	R7	1
      0x8C200905,  //  0029  GETMET	R8	R4	K5
      0x5C280E00,  //  002A  MOVE	R10	R7
      0x7C200400,  //  002B  CALL	R8	2
      0x000C070A,  //  002C  ADD	R3	R3	K10
      0x7001FFE4,  //  002D  JMP		#0013
      0x5814000B,  //  002E  LDCONST	R5	K11
      0xAC140200,  //  002F  CATCH	R5	1	0
      0xB0080000,  //  0030  RAISE	2	R0	R0
      0x8C140905,  //  0031  GETMET	R5	R4	K5
      0x581C000E,  //  0032  LDCONST	R7	K14
      0x7C140400,  //  0033  CALL	R5	2
      0x8C14090F,  //  0034  GETMET	R5	R4	K15
      0x7C140200,  //  0035  CALL	R5	1
      0xB8162000,  //  0036  GETNGBL	R5	K16
      0x8C140B11,  //  0037  GETMET	R5	R5	K17
      0x601C0018,  //  0038  GETGBL	R7	G24
      0x58200012,  //  0039  LDCONST	R8	K18
      0x5C240600,  //  003A  MOVE	R9	R3
      0x5C280400,  //  003B  MOVE	R10	R2
      0x7C1C0600,  //  003C  CALL	R7	3
      0x58200013,  //  003D  LDCONST	R8	K19
      0x7C140600,  //  003E  CALL	R5	3
      0x88140114,  //  003F  GETMBR	R5	R0	K20
      0x8C140B15,  //  0040  GETMET	R5	R5	K21
      0x7C140200,  //  0041  CALL	R5	1
      0xA8040001,  //  0042  EXBLK	1	1
      0x70020010,  //  0043  JMP		#0055
      0xAC080002,  //  0044  CATCH	R2	0	2
      0x7002000D,  //  0045  JMP		#0054
      0xB8122000,  //  0046  GETNGBL	R4	K16
      0x8C100911,  //  0047  GETMET	R4	R4	K17
      0x60180008,  //  0048  GETGBL	R6	G8
      0x5C1C0400,  //  0049  MOVE	R7	R2
      0x7C180200,  //  004A  CALL	R6	1
      0x001A2C06,  //  004B  ADD	R6	K22	R6
      0x00180D17,  //  004C  ADD	R6	R6	K23
      0x601C0008,  //  004D  GETGBL	R7	G8
      0x5C200600,  //  004E  MOVE	R8	R3
      0x7C1C0200,  //  004F  CALL	R7	1
      0x00180C07,  //  0050  ADD	R6	R6	R7
      0x581C0013,  //  0051  LDCONST	R7	K19
      0x7C100600,  //  0052  CALL	R4	3
      0x70020000,  //  0053  JMP		#0055
      0xB0080000,  //  0054  RAISE	2	R0	R0
      0x80000000,  //  0055  RET	0
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
