/* Solidification of Matter_Session_Store.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
// compact class 'Matter_Session_Store' ktab size: 74, total: 155 (saved 648 bytes)
static const bvalue be_ktab_class_Matter_Session_Store[74] = {
  /* K0   */  be_const_int(0),
  /* K1   */  be_nested_str_weak(sessions),
  /* K2   */  be_nested_str_weak(remove),
  /* K3   */  be_const_int(1),
  /* K4   */  be_nested_str_weak(crypto),
  /* K5   */  be_nested_str_weak(random),
  /* K6   */  be_const_int(2),
  /* K7   */  be_nested_str_weak(get),
  /* K8   */  be_const_int(0),
  /* K9   */  be_nested_str_weak(get_session_by_local_session_id),
  /* K10  */  be_nested_str_weak(matter),
  /* K11  */  be_nested_str_weak(Expirable_list),
  /* K12  */  be_nested_str_weak(fabrics),
  /* K13  */  be_nested_str_weak(_FABRICS),
  /* K14  */  be_nested_str_weak(read),
  /* K15  */  be_nested_str_weak(close),
  /* K16  */  be_nested_str_weak(json),
  /* K17  */  be_nested_str_weak(load),
  /* K18  */  be_nested_str_weak(tasmota),
  /* K19  */  be_nested_str_weak(gc),
  /* K20  */  be_nested_str_weak(Fabric),
  /* K21  */  be_nested_str_weak(fromjson),
  /* K22  */  be_nested_str_weak(set_no_expiration),
  /* K23  */  be_nested_str_weak(set_persist),
  /* K24  */  be_nested_str_weak(find),
  /* K25  */  be_nested_str_weak(_sessions),
  /* K26  */  be_nested_str_weak(Session),
  /* K27  */  be_nested_str_weak(add_session),
  /* K28  */  be_nested_str_weak(stop_iteration),
  /* K29  */  be_nested_str_weak(push),
  /* K30  */  be_nested_str_weak(log),
  /* K31  */  be_nested_str_weak(MTR_X3A_X20Loaded_X20_X25i_X20fabric_X28s_X29),
  /* K32  */  be_nested_str_weak(io_error),
  /* K33  */  be_nested_str_weak(MTR_X3A_X20Session_Store_X3A_X3Aload_X20Exception_X3A),
  /* K34  */  be_nested_str_weak(_X7C),
  /* K35  */  be_nested_str_weak(get_device_id),
  /* K36  */  be_nested_str_weak(get_fabric_id),
  /* K37  */  be_nested_str_weak(active_fabrics),
  /* K38  */  be_nested_str_weak(get_fabric_index),
  /* K39  */  be_nested_str_weak(remove_expired),
  /* K40  */  be_nested_str_weak(persistables),
  /* K41  */  be_nested_str_weak(count_persistables),
  /* K42  */  be_nested_str_weak(MTR_X3A_X20session_X2Eresumption_id_X3D_X25s_X20vs_X20_X25s),
  /* K43  */  be_nested_str_weak(resumption_id),
  /* K44  */  be_nested_str_weak(shared_secret),
  /* K45  */  be_nested_str_weak(update),
  /* K46  */  be_nested_str_weak(set_expire_in_seconds),
  /* K47  */  be_nested_str_weak(get_session_by_source_node_id),
  /* K48  */  be_nested_str_weak(_source_node_id),
  /* K49  */  be_nested_str_weak(local_session_id),
  /* K50  */  be_nested_str_weak(device),
  /* K51  */  be_nested_str_weak(every_second),
  /* K52  */  be_nested_str_weak(fabric_id),
  /* K53  */  be_nested_str_weak(device_id),
  /* K54  */  be_nested_str_weak(value_error),
  /* K55  */  be_nested_str_weak(must_X20be_X20of_X20class_X20matter_X2EFabric),
  /* K56  */  be_nested_str_weak(remove_redundant_fabric),
  /* K57  */  be_nested_str_weak(fabric_index),
  /* K58  */  be_nested_str_weak(int),
  /* K59  */  be_nested_str_weak(path),
  /* K60  */  be_nested_str_weak(_FABRICS_TEMP),
  /* K61  */  be_nested_str_weak(w),
  /* K62  */  be_nested_str_weak(write),
  /* K63  */  be_nested_str_weak(_X5B),
  /* K64  */  be_nested_str_weak(_X2C),
  /* K65  */  be_nested_str_weak(writejson),
  /* K66  */  be_nested_str_weak(_X5D),
  /* K67  */  be_nested_str_weak(rename),
  /* K68  */  be_nested_str_weak(MTR_X3A_X20_X3DSaved_X20_X20_X20_X20_X20_X25s_X20fabric_X28s_X29_X20and_X20_X25s_X20session_X28s_X29),
  /* K69  */  be_nested_str_weak(event_fabrics_saved),
  /* K70  */  be_nested_str_weak(MTR_X3A_X20Saving_X20Fabrics_X20failed),
  /* K71  */  be_nested_str_weak(MTR_X3A_X20Session_Store_X3A_X3Asave_X20Exception_X3A),
  /* K72  */  be_nested_str_weak(remove_session),
  /* K73  */  be_nested_str_weak(_fabric),
};


extern const bclass be_class_Matter_Session_Store;

/********************************************************************
** Solidified function: remove_session
********************************************************************/
be_local_closure(class_Matter_Session_Store_remove_session,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Session_Store,     /* shared constants */
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
** Solidified function: gen_local_session_id
********************************************************************/
be_local_closure(class_Matter_Session_Store_gen_local_session_id,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Session_Store,     /* shared constants */
    be_str_weak(gen_local_session_id),
    &be_const_str_solidified,
    ( &(const binstruction[19]) {  /* code */
      0xA4060800,  //  0000  IMPORT	R1	K4
      0x50080200,  //  0001  LDBOOL	R2	1	0
      0x780A000E,  //  0002  JMPF	R2	#0012
      0x8C080305,  //  0003  GETMET	R2	R1	K5
      0x58100006,  //  0004  LDCONST	R4	K6
      0x7C080400,  //  0005  CALL	R2	2
      0x8C080507,  //  0006  GETMET	R2	R2	K7
      0x58100008,  //  0007  LDCONST	R4	K8
      0x58140006,  //  0008  LDCONST	R5	K6
      0x7C080600,  //  0009  CALL	R2	3
      0x8C0C0109,  //  000A  GETMET	R3	R0	K9
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
** Solidified function: load_fabrics
********************************************************************/
be_local_closure(class_Matter_Session_Store_load_fabrics,   /* name */
  be_nested_proto(
    16,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Session_Store,     /* shared constants */
    be_str_weak(load_fabrics),
    &be_const_str_solidified,
    ( &(const binstruction[115]) {  /* code */
      0xA802005E,  //  0000  EXBLK	0	#0060
      0xB8061400,  //  0001  GETNGBL	R1	K10
      0x8C04030B,  //  0002  GETMET	R1	R1	K11
      0x7C040200,  //  0003  CALL	R1	1
      0x90020201,  //  0004  SETMBR	R0	K1	R1
      0xB8061400,  //  0005  GETNGBL	R1	K10
      0x8C04030B,  //  0006  GETMET	R1	R1	K11
      0x7C040200,  //  0007  CALL	R1	1
      0x90021801,  //  0008  SETMBR	R0	K12	R1
      0x60040011,  //  0009  GETGBL	R1	G17
      0x8808010D,  //  000A  GETMBR	R2	R0	K13
      0x7C040200,  //  000B  CALL	R1	1
      0x8C08030E,  //  000C  GETMET	R2	R1	K14
      0x7C080200,  //  000D  CALL	R2	1
      0x8C0C030F,  //  000E  GETMET	R3	R1	K15
      0x7C0C0200,  //  000F  CALL	R3	1
      0xA40E2000,  //  0010  IMPORT	R3	K16
      0x8C100711,  //  0011  GETMET	R4	R3	K17
      0x5C180400,  //  0012  MOVE	R6	R2
      0x7C100400,  //  0013  CALL	R4	2
      0x4C080000,  //  0014  LDNIL	R2
      0xB8162400,  //  0015  GETNGBL	R5	K18
      0x8C140B13,  //  0016  GETMET	R5	R5	K19
      0x7C140200,  //  0017  CALL	R5	1
      0x60140010,  //  0018  GETGBL	R5	G16
      0x5C180800,  //  0019  MOVE	R6	R4
      0x7C140200,  //  001A  CALL	R5	1
      0xA8020035,  //  001B  EXBLK	0	#0052
      0x5C180A00,  //  001C  MOVE	R6	R5
      0x7C180000,  //  001D  CALL	R6	0
      0xB81E1400,  //  001E  GETNGBL	R7	K10
      0x881C0F14,  //  001F  GETMBR	R7	R7	K20
      0x8C1C0F15,  //  0020  GETMET	R7	R7	K21
      0x5C240000,  //  0021  MOVE	R9	R0
      0x5C280C00,  //  0022  MOVE	R10	R6
      0x7C1C0600,  //  0023  CALL	R7	3
      0x8C200F16,  //  0024  GETMET	R8	R7	K22
      0x7C200200,  //  0025  CALL	R8	1
      0x8C200F17,  //  0026  GETMET	R8	R7	K23
      0x50280200,  //  0027  LDBOOL	R10	1	0
      0x7C200400,  //  0028  CALL	R8	2
      0x8C200D18,  //  0029  GETMET	R8	R6	K24
      0x58280019,  //  002A  LDCONST	R10	K25
      0x602C0012,  //  002B  GETGBL	R11	G18
      0x7C2C0000,  //  002C  CALL	R11	0
      0x7C200600,  //  002D  CALL	R8	3
      0x60240010,  //  002E  GETGBL	R9	G16
      0x5C281000,  //  002F  MOVE	R10	R8
      0x7C240200,  //  0030  CALL	R9	1
      0xA8020017,  //  0031  EXBLK	0	#004A
      0x5C281200,  //  0032  MOVE	R10	R9
      0x7C280000,  //  0033  CALL	R10	0
      0xB82E1400,  //  0034  GETNGBL	R11	K10
      0x882C171A,  //  0035  GETMBR	R11	R11	K26
      0x8C2C1715,  //  0036  GETMET	R11	R11	K21
      0x5C340000,  //  0037  MOVE	R13	R0
      0x5C381400,  //  0038  MOVE	R14	R10
      0x5C3C0E00,  //  0039  MOVE	R15	R7
      0x7C2C0800,  //  003A  CALL	R11	4
      0x4C300000,  //  003B  LDNIL	R12
      0x2030160C,  //  003C  NE	R12	R11	R12
      0x7832000A,  //  003D  JMPF	R12	#0049
      0x8C301716,  //  003E  GETMET	R12	R11	K22
      0x7C300200,  //  003F  CALL	R12	1
      0x8C301717,  //  0040  GETMET	R12	R11	K23
      0x50380200,  //  0041  LDBOOL	R14	1	0
      0x7C300400,  //  0042  CALL	R12	2
      0x8C30011B,  //  0043  GETMET	R12	R0	K27
      0x5C381600,  //  0044  MOVE	R14	R11
      0x7C300400,  //  0045  CALL	R12	2
      0x8C300F1B,  //  0046  GETMET	R12	R7	K27
      0x5C381600,  //  0047  MOVE	R14	R11
      0x7C300400,  //  0048  CALL	R12	2
      0x7001FFE7,  //  0049  JMP		#0032
      0x5824001C,  //  004A  LDCONST	R9	K28
      0xAC240200,  //  004B  CATCH	R9	1	0
      0xB0080000,  //  004C  RAISE	2	R0	R0
      0x8824010C,  //  004D  GETMBR	R9	R0	K12
      0x8C24131D,  //  004E  GETMET	R9	R9	K29
      0x5C2C0E00,  //  004F  MOVE	R11	R7
      0x7C240400,  //  0050  CALL	R9	2
      0x7001FFC9,  //  0051  JMP		#001C
      0x5814001C,  //  0052  LDCONST	R5	K28
      0xAC140200,  //  0053  CATCH	R5	1	0
      0xB0080000,  //  0054  RAISE	2	R0	R0
      0xB8163C00,  //  0055  GETNGBL	R5	K30
      0x60180018,  //  0056  GETGBL	R6	G24
      0x581C001F,  //  0057  LDCONST	R7	K31
      0x6020000C,  //  0058  GETGBL	R8	G12
      0x8824010C,  //  0059  GETMBR	R9	R0	K12
      0x7C200200,  //  005A  CALL	R8	1
      0x7C180400,  //  005B  CALL	R6	2
      0x581C0006,  //  005C  LDCONST	R7	K6
      0x7C140400,  //  005D  CALL	R5	2
      0xA8040001,  //  005E  EXBLK	1	1
      0x70020011,  //  005F  JMP		#0072
      0xAC040002,  //  0060  CATCH	R1	0	2
      0x7002000E,  //  0061  JMP		#0071
      0x200C0320,  //  0062  NE	R3	R1	K32
      0x780E000B,  //  0063  JMPF	R3	#0070
      0xB80E3C00,  //  0064  GETNGBL	R3	K30
      0x60100008,  //  0065  GETGBL	R4	G8
      0x5C140200,  //  0066  MOVE	R5	R1
      0x7C100200,  //  0067  CALL	R4	1
      0x00124204,  //  0068  ADD	R4	K33	R4
      0x00100922,  //  0069  ADD	R4	R4	K34
      0x60140008,  //  006A  GETGBL	R5	G8
      0x5C180400,  //  006B  MOVE	R6	R2
      0x7C140200,  //  006C  CALL	R5	1
      0x00100805,  //  006D  ADD	R4	R4	R5
      0x58140006,  //  006E  LDCONST	R5	K6
      0x7C0C0400,  //  006F  CALL	R3	2
      0x70020000,  //  0070  JMP		#0072
      0xB0080000,  //  0071  RAISE	2	R0	R0
      0x80000000,  //  0072  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: find_children_fabrics
********************************************************************/
be_local_closure(class_Matter_Session_Store_find_children_fabrics,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
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
    1,                          /* has constants */
    &be_ktab_class_Matter_Session_Store,     /* shared constants */
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
** Solidified function: sessions_active
********************************************************************/
be_local_closure(class_Matter_Session_Store_sessions_active,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Session_Store,     /* shared constants */
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
      0x8C100723,  //  000A  GETMET	R4	R3	K35
      0x7C100200,  //  000B  CALL	R4	1
      0x78120005,  //  000C  JMPF	R4	#0013
      0x8C100724,  //  000D  GETMET	R4	R3	K36
      0x7C100200,  //  000E  CALL	R4	1
      0x78120002,  //  000F  JMPF	R4	#0013
      0x8C10031D,  //  0010  GETMET	R4	R1	K29
      0x5C180600,  //  0011  MOVE	R6	R3
      0x7C100400,  //  0012  CALL	R4	2
      0x00080503,  //  0013  ADD	R2	R2	K3
      0x7001FFED,  //  0014  JMP		#0003
      0x80040200,  //  0015  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: find_fabric_by_index
********************************************************************/
be_local_closure(class_Matter_Session_Store_find_fabric_by_index,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Session_Store,     /* shared constants */
    be_str_weak(find_fabric_by_index),
    &be_const_str_solidified,
    ( &(const binstruction[19]) {  /* code */
      0x60080010,  //  0000  GETGBL	R2	G16
      0x8C0C0125,  //  0001  GETMET	R3	R0	K37
      0x7C0C0200,  //  0002  CALL	R3	1
      0x7C080200,  //  0003  CALL	R2	1
      0xA8020008,  //  0004  EXBLK	0	#000E
      0x5C0C0400,  //  0005  MOVE	R3	R2
      0x7C0C0000,  //  0006  CALL	R3	0
      0x8C100726,  //  0007  GETMET	R4	R3	K38
      0x7C100200,  //  0008  CALL	R4	1
      0x1C100801,  //  0009  EQ	R4	R4	R1
      0x78120001,  //  000A  JMPF	R4	#000D
      0xA8040001,  //  000B  EXBLK	1	1
      0x80040600,  //  000C  RET	1	R3
      0x7001FFF6,  //  000D  JMP		#0005
      0x5808001C,  //  000E  LDCONST	R2	K28
      0xAC080200,  //  000F  CATCH	R2	1	0
      0xB0080000,  //  0010  RAISE	2	R0	R0
      0x4C080000,  //  0011  LDNIL	R2
      0x80040400,  //  0012  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: active_fabrics
********************************************************************/
be_local_closure(class_Matter_Session_Store_active_fabrics,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Session_Store,     /* shared constants */
    be_str_weak(active_fabrics),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x8C040127,  //  0000  GETMET	R1	R0	K39
      0x7C040200,  //  0001  CALL	R1	1
      0x8804010C,  //  0002  GETMBR	R1	R0	K12
      0x8C040328,  //  0003  GETMET	R1	R1	K40
      0x7C040200,  //  0004  CALL	R1	1
      0x80040200,  //  0005  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: create_fabric
********************************************************************/
be_local_closure(class_Matter_Session_Store_create_fabric,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Session_Store,     /* shared constants */
    be_str_weak(create_fabric),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0xB8061400,  //  0000  GETNGBL	R1	K10
      0x8C040314,  //  0001  GETMET	R1	R1	K20
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x80040200,  //  0004  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: count_active_fabrics
********************************************************************/
be_local_closure(class_Matter_Session_Store_count_active_fabrics,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Session_Store,     /* shared constants */
    be_str_weak(count_active_fabrics),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x8C040127,  //  0000  GETMET	R1	R0	K39
      0x7C040200,  //  0001  CALL	R1	1
      0x8804010C,  //  0002  GETMBR	R1	R0	K12
      0x4C080000,  //  0003  LDNIL	R2
      0x20040202,  //  0004  NE	R1	R1	R2
      0x78060004,  //  0005  JMPF	R1	#000B
      0x8804010C,  //  0006  GETMBR	R1	R0	K12
      0x8C040329,  //  0007  GETMET	R1	R1	K41
      0x7C040200,  //  0008  CALL	R1	1
      0x80040200,  //  0009  RET	1	R1
      0x70020000,  //  000A  JMP		#000C
      0x80060000,  //  000B  RET	1	K0
      0x80000000,  //  000C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: find_session_by_resumption_id
********************************************************************/
be_local_closure(class_Matter_Session_Store_find_session_by_resumption_id,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Session_Store,     /* shared constants */
    be_str_weak(find_session_by_resumption_id),
    &be_const_str_solidified,
    ( &(const binstruction[37]) {  /* code */
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
      0x78120018,  //  000A  JMPF	R4	#0024
      0x94100602,  //  000B  GETIDX	R4	R3	R2
      0xB8163C00,  //  000C  GETNGBL	R5	K30
      0x60180018,  //  000D  GETGBL	R6	G24
      0x581C002A,  //  000E  LDCONST	R7	K42
      0x60200008,  //  000F  GETGBL	R8	G8
      0x8824092B,  //  0010  GETMBR	R9	R4	K43
      0x7C200200,  //  0011  CALL	R8	1
      0x60240008,  //  0012  GETGBL	R9	G8
      0x5C280200,  //  0013  MOVE	R10	R1
      0x7C240200,  //  0014  CALL	R9	1
      0x7C180600,  //  0015  CALL	R6	3
      0x541E0003,  //  0016  LDINT	R7	4
      0x7C140400,  //  0017  CALL	R5	2
      0x8814092B,  //  0018  GETMBR	R5	R4	K43
      0x1C140A01,  //  0019  EQ	R5	R5	R1
      0x78160006,  //  001A  JMPF	R5	#0022
      0x8814092C,  //  001B  GETMBR	R5	R4	K44
      0x4C180000,  //  001C  LDNIL	R6
      0x20140A06,  //  001D  NE	R5	R5	R6
      0x78160002,  //  001E  JMPF	R5	#0022
      0x8C14092D,  //  001F  GETMET	R5	R4	K45
      0x7C140200,  //  0020  CALL	R5	1
      0x80040800,  //  0021  RET	1	R4
      0x00080503,  //  0022  ADD	R2	R2	K3
      0x7001FFE1,  //  0023  JMP		#0006
      0x80000000,  //  0024  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: add_session
********************************************************************/
be_local_closure(class_Matter_Session_Store_add_session,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Session_Store,     /* shared constants */
    be_str_weak(add_session),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x4C0C0000,  //  0000  LDNIL	R3
      0x200C0403,  //  0001  NE	R3	R2	R3
      0x780E0002,  //  0002  JMPF	R3	#0006
      0x8C0C032E,  //  0003  GETMET	R3	R1	K46
      0x5C140400,  //  0004  MOVE	R5	R2
      0x7C0C0400,  //  0005  CALL	R3	2
      0x880C0101,  //  0006  GETMBR	R3	R0	K1
      0x8C0C071D,  //  0007  GETMET	R3	R3	K29
      0x5C140200,  //  0008  MOVE	R5	R1
      0x7C0C0400,  //  0009  CALL	R3	2
      0x80000000,  //  000A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: find_session_source_id_unsecure
********************************************************************/
be_local_closure(class_Matter_Session_Store_find_session_source_id_unsecure,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Session_Store,     /* shared constants */
    be_str_weak(find_session_source_id_unsecure),
    &be_const_str_solidified,
    ( &(const binstruction[24]) {  /* code */
      0x8C0C012F,  //  0000  GETMET	R3	R0	K47
      0x5C140200,  //  0001  MOVE	R5	R1
      0x7C0C0400,  //  0002  CALL	R3	2
      0x4C100000,  //  0003  LDNIL	R4
      0x1C100604,  //  0004  EQ	R4	R3	R4
      0x7812000E,  //  0005  JMPF	R4	#0015
      0xB8121400,  //  0006  GETNGBL	R4	K10
      0x8C10091A,  //  0007  GETMET	R4	R4	K26
      0x5C180000,  //  0008  MOVE	R6	R0
      0x581C0000,  //  0009  LDCONST	R7	K0
      0x58200000,  //  000A  LDCONST	R8	K0
      0x7C100800,  //  000B  CALL	R4	4
      0x5C0C0800,  //  000C  MOVE	R3	R4
      0x900E6001,  //  000D  SETMBR	R3	K48	R1
      0x88100101,  //  000E  GETMBR	R4	R0	K1
      0x8C10091D,  //  000F  GETMET	R4	R4	K29
      0x5C180600,  //  0010  MOVE	R6	R3
      0x7C100400,  //  0011  CALL	R4	2
      0x8C10072E,  //  0012  GETMET	R4	R3	K46
      0x5C180400,  //  0013  MOVE	R6	R2
      0x7C100400,  //  0014  CALL	R4	2
      0x8C10072D,  //  0015  GETMET	R4	R3	K45
      0x7C100200,  //  0016  CALL	R4	1
      0x80040600,  //  0017  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: every_second
********************************************************************/
be_local_closure(class_Matter_Session_Store_every_second,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Session_Store,     /* shared constants */
    be_str_weak(every_second),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x8C040127,  //  0000  GETMET	R1	R0	K39
      0x7C040200,  //  0001  CALL	R1	1
      0x80000000,  //  0002  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_session_by_local_session_id
********************************************************************/
be_local_closure(class_Matter_Session_Store_get_session_by_local_session_id,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Session_Store,     /* shared constants */
    be_str_weak(get_session_by_local_session_id),
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0001,  //  0002  JMPF	R2	#0005
      0x4C080000,  //  0003  LDNIL	R2
      0x80040400,  //  0004  RET	1	R2
      0x6008000C,  //  0005  GETGBL	R2	G12
      0x880C0101,  //  0006  GETMBR	R3	R0	K1
      0x7C080200,  //  0007  CALL	R2	1
      0x580C0008,  //  0008  LDCONST	R3	K8
      0x88100101,  //  0009  GETMBR	R4	R0	K1
      0x14140602,  //  000A  LT	R5	R3	R2
      0x78160008,  //  000B  JMPF	R5	#0015
      0x94140803,  //  000C  GETIDX	R5	R4	R3
      0x88180B31,  //  000D  GETMBR	R6	R5	K49
      0x1C180C01,  //  000E  EQ	R6	R6	R1
      0x781A0002,  //  000F  JMPF	R6	#0013
      0x8C180B2D,  //  0010  GETMET	R6	R5	K45
      0x7C180200,  //  0011  CALL	R6	1
      0x80040A00,  //  0012  RET	1	R5
      0x000C0703,  //  0013  ADD	R3	R3	K3
      0x7001FFF4,  //  0014  JMP		#000A
      0x80000000,  //  0015  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_Matter_Session_Store_init,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Session_Store,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x90026401,  //  0000  SETMBR	R0	K50	R1
      0xB80A1400,  //  0001  GETNGBL	R2	K10
      0x8C08050B,  //  0002  GETMET	R2	R2	K11
      0x7C080200,  //  0003  CALL	R2	1
      0x90020202,  //  0004  SETMBR	R0	K1	R2
      0xB80A1400,  //  0005  GETNGBL	R2	K10
      0x8C08050B,  //  0006  GETMET	R2	R2	K11
      0x7C080200,  //  0007  CALL	R2	1
      0x90021802,  //  0008  SETMBR	R0	K12	R2
      0x80000000,  //  0009  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: remove_expired
********************************************************************/
be_local_closure(class_Matter_Session_Store_remove_expired,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Session_Store,     /* shared constants */
    be_str_weak(remove_expired),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x88040101,  //  0000  GETMBR	R1	R0	K1
      0x8C040333,  //  0001  GETMET	R1	R1	K51
      0x7C040200,  //  0002  CALL	R1	1
      0x8804010C,  //  0003  GETMBR	R1	R0	K12
      0x8C040333,  //  0004  GETMET	R1	R1	K51
      0x7C040200,  //  0005  CALL	R1	1
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: remove_redundant_fabric
********************************************************************/
be_local_closure(class_Matter_Session_Store_remove_redundant_fabric,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Session_Store,     /* shared constants */
    be_str_weak(remove_redundant_fabric),
    &be_const_str_solidified,
    ( &(const binstruction[26]) {  /* code */
      0x58080000,  //  0000  LDCONST	R2	K0
      0x600C000C,  //  0001  GETGBL	R3	G12
      0x8810010C,  //  0002  GETMBR	R4	R0	K12
      0x7C0C0200,  //  0003  CALL	R3	1
      0x140C0403,  //  0004  LT	R3	R2	R3
      0x780E0012,  //  0005  JMPF	R3	#0019
      0x880C010C,  //  0006  GETMBR	R3	R0	K12
      0x940C0602,  //  0007  GETIDX	R3	R3	R2
      0x20100601,  //  0008  NE	R4	R3	R1
      0x7812000C,  //  0009  JMPF	R4	#0017
      0x88100734,  //  000A  GETMBR	R4	R3	K52
      0x88140334,  //  000B  GETMBR	R5	R1	K52
      0x1C100805,  //  000C  EQ	R4	R4	R5
      0x78120008,  //  000D  JMPF	R4	#0017
      0x88100735,  //  000E  GETMBR	R4	R3	K53
      0x88140335,  //  000F  GETMBR	R5	R1	K53
      0x1C100805,  //  0010  EQ	R4	R4	R5
      0x78120004,  //  0011  JMPF	R4	#0017
      0x8810010C,  //  0012  GETMBR	R4	R0	K12
      0x8C100902,  //  0013  GETMET	R4	R4	K2
      0x5C180400,  //  0014  MOVE	R6	R2
      0x7C100400,  //  0015  CALL	R4	2
      0x70020000,  //  0016  JMP		#0018
      0x00080503,  //  0017  ADD	R2	R2	K3
      0x7001FFE7,  //  0018  JMP		#0001
      0x80000000,  //  0019  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: add_fabric
********************************************************************/
be_local_closure(class_Matter_Session_Store_add_fabric,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Session_Store,     /* shared constants */
    be_str_weak(add_fabric),
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0x6008000F,  //  0000  GETGBL	R2	G15
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0xB8121400,  //  0002  GETNGBL	R4	K10
      0x88100914,  //  0003  GETMBR	R4	R4	K20
      0x7C080400,  //  0004  CALL	R2	2
      0x740A0000,  //  0005  JMPT	R2	#0007
      0xB0066D37,  //  0006  RAISE	1	K54	K55
      0x8808010C,  //  0007  GETMBR	R2	R0	K12
      0x8C080518,  //  0008  GETMET	R2	R2	K24
      0x5C100200,  //  0009  MOVE	R4	R1
      0x7C080400,  //  000A  CALL	R2	2
      0x4C0C0000,  //  000B  LDNIL	R3
      0x1C080403,  //  000C  EQ	R2	R2	R3
      0x780A0006,  //  000D  JMPF	R2	#0015
      0x8C080138,  //  000E  GETMET	R2	R0	K56
      0x5C100200,  //  000F  MOVE	R4	R1
      0x7C080400,  //  0010  CALL	R2	2
      0x8808010C,  //  0011  GETMBR	R2	R0	K12
      0x8C08051D,  //  0012  GETMET	R2	R2	K29
      0x5C100200,  //  0013  MOVE	R4	R1
      0x7C080400,  //  0014  CALL	R2	2
      0x80000000,  //  0015  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_session_by_source_node_id
********************************************************************/
be_local_closure(class_Matter_Session_Store_get_session_by_source_node_id,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Session_Store,     /* shared constants */
    be_str_weak(get_session_by_source_node_id),
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0001,  //  0002  JMPF	R2	#0005
      0x4C080000,  //  0003  LDNIL	R2
      0x80040400,  //  0004  RET	1	R2
      0x6008000C,  //  0005  GETGBL	R2	G12
      0x880C0101,  //  0006  GETMBR	R3	R0	K1
      0x7C080200,  //  0007  CALL	R2	1
      0x580C0008,  //  0008  LDCONST	R3	K8
      0x88100101,  //  0009  GETMBR	R4	R0	K1
      0x14140602,  //  000A  LT	R5	R3	R2
      0x78160008,  //  000B  JMPF	R5	#0015
      0x94140803,  //  000C  GETIDX	R5	R4	R3
      0x88180B30,  //  000D  GETMBR	R6	R5	K48
      0x1C180C01,  //  000E  EQ	R6	R6	R1
      0x781A0002,  //  000F  JMPF	R6	#0013
      0x8C180B2D,  //  0010  GETMET	R6	R5	K45
      0x7C180200,  //  0011  CALL	R6	1
      0x80040A00,  //  0012  RET	1	R5
      0x000C0703,  //  0013  ADD	R3	R3	K3
      0x7001FFF4,  //  0014  JMP		#000A
      0x80000000,  //  0015  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: next_fabric_idx
********************************************************************/
be_local_closure(class_Matter_Session_Store_next_fabric_idx,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Session_Store,     /* shared constants */
    be_str_weak(next_fabric_idx),
    &be_const_str_solidified,
    ( &(const binstruction[25]) {  /* code */
      0x8C040127,  //  0000  GETMET	R1	R0	K39
      0x7C040200,  //  0001  CALL	R1	1
      0x58040003,  //  0002  LDCONST	R1	K3
      0x60080010,  //  0003  GETGBL	R2	G16
      0x8C0C0125,  //  0004  GETMET	R3	R0	K37
      0x7C0C0200,  //  0005  CALL	R3	1
      0x7C080200,  //  0006  CALL	R2	1
      0xA802000C,  //  0007  EXBLK	0	#0015
      0x5C0C0400,  //  0008  MOVE	R3	R2
      0x7C0C0000,  //  0009  CALL	R3	0
      0x88100739,  //  000A  GETMBR	R4	R3	K57
      0x60140004,  //  000B  GETGBL	R5	G4
      0x5C180800,  //  000C  MOVE	R6	R4
      0x7C140200,  //  000D  CALL	R5	1
      0x1C140B3A,  //  000E  EQ	R5	R5	K58
      0x78160003,  //  000F  JMPF	R5	#0014
      0x28140801,  //  0010  GE	R5	R4	R1
      0x78160001,  //  0011  JMPF	R5	#0014
      0x00140903,  //  0012  ADD	R5	R4	K3
      0x5C040A00,  //  0013  MOVE	R1	R5
      0x7001FFF2,  //  0014  JMP		#0008
      0x5808001C,  //  0015  LDCONST	R2	K28
      0xAC080200,  //  0016  CATCH	R2	1	0
      0xB0080000,  //  0017  RAISE	2	R0	R0
      0x80040200,  //  0018  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: save_fabrics
********************************************************************/
be_local_closure(class_Matter_Session_Store_save_fabrics,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Session_Store,     /* shared constants */
    be_str_weak(save_fabrics),
    &be_const_str_solidified,
    ( &(const binstruction[98]) {  /* code */
      0xA4062000,  //  0000  IMPORT	R1	K16
      0xA40A7600,  //  0001  IMPORT	R2	K59
      0xA802004D,  //  0002  EXBLK	0	#0051
      0x8C0C0127,  //  0003  GETMET	R3	R0	K39
      0x7C0C0200,  //  0004  CALL	R3	1
      0x580C0008,  //  0005  LDCONST	R3	K8
      0x58100008,  //  0006  LDCONST	R4	K8
      0x60140011,  //  0007  GETGBL	R5	G17
      0x8818013C,  //  0008  GETMBR	R6	R0	K60
      0x581C003D,  //  0009  LDCONST	R7	K61
      0x7C140400,  //  000A  CALL	R5	2
      0x8C180B3E,  //  000B  GETMET	R6	R5	K62
      0x5820003F,  //  000C  LDCONST	R8	K63
      0x7C180400,  //  000D  CALL	R6	2
      0x60180010,  //  000E  GETGBL	R6	G16
      0x881C010C,  //  000F  GETMBR	R7	R0	K12
      0x8C1C0F28,  //  0010  GETMET	R7	R7	K40
      0x7C1C0200,  //  0011  CALL	R7	1
      0x7C180200,  //  0012  CALL	R6	1
      0xA8020018,  //  0013  EXBLK	0	#002D
      0x5C1C0C00,  //  0014  MOVE	R7	R6
      0x7C1C0000,  //  0015  CALL	R7	0
      0x60200010,  //  0016  GETGBL	R8	G16
      0x88240F19,  //  0017  GETMBR	R9	R7	K25
      0x8C241328,  //  0018  GETMET	R9	R9	K40
      0x7C240200,  //  0019  CALL	R9	1
      0x7C200200,  //  001A  CALL	R8	1
      0xA8020003,  //  001B  EXBLK	0	#0020
      0x5C241000,  //  001C  MOVE	R9	R8
      0x7C240000,  //  001D  CALL	R9	0
      0x000C0703,  //  001E  ADD	R3	R3	K3
      0x7001FFFB,  //  001F  JMP		#001C
      0x5820001C,  //  0020  LDCONST	R8	K28
      0xAC200200,  //  0021  CATCH	R8	1	0
      0xB0080000,  //  0022  RAISE	2	R0	R0
      0x24200908,  //  0023  GT	R8	R4	K8
      0x78220002,  //  0024  JMPF	R8	#0028
      0x8C200B3E,  //  0025  GETMET	R8	R5	K62
      0x58280040,  //  0026  LDCONST	R10	K64
      0x7C200400,  //  0027  CALL	R8	2
      0x8C200F41,  //  0028  GETMET	R8	R7	K65
      0x5C280A00,  //  0029  MOVE	R10	R5
      0x7C200400,  //  002A  CALL	R8	2
      0x00100903,  //  002B  ADD	R4	R4	K3
      0x7001FFE6,  //  002C  JMP		#0014
      0x5818001C,  //  002D  LDCONST	R6	K28
      0xAC180200,  //  002E  CATCH	R6	1	0
      0xB0080000,  //  002F  RAISE	2	R0	R0
      0x8C180B3E,  //  0030  GETMET	R6	R5	K62
      0x58200042,  //  0031  LDCONST	R8	K66
      0x7C180400,  //  0032  CALL	R6	2
      0x8C180B0F,  //  0033  GETMET	R6	R5	K15
      0x7C180200,  //  0034  CALL	R6	1
      0x8C180502,  //  0035  GETMET	R6	R2	K2
      0x8820010D,  //  0036  GETMBR	R8	R0	K13
      0x7C180400,  //  0037  CALL	R6	2
      0x8C180543,  //  0038  GETMET	R6	R2	K67
      0x8820013C,  //  0039  GETMBR	R8	R0	K60
      0x8824010D,  //  003A  GETMBR	R9	R0	K13
      0x7C180600,  //  003B  CALL	R6	3
      0x781A000B,  //  003C  JMPF	R6	#0049
      0xB81A3C00,  //  003D  GETNGBL	R6	K30
      0x601C0018,  //  003E  GETGBL	R7	G24
      0x58200044,  //  003F  LDCONST	R8	K68
      0x5C240800,  //  0040  MOVE	R9	R4
      0x5C280600,  //  0041  MOVE	R10	R3
      0x7C1C0600,  //  0042  CALL	R7	3
      0x58200006,  //  0043  LDCONST	R8	K6
      0x7C180400,  //  0044  CALL	R6	2
      0x88180132,  //  0045  GETMBR	R6	R0	K50
      0x8C180D45,  //  0046  GETMET	R6	R6	K69
      0x7C180200,  //  0047  CALL	R6	1
      0x70020005,  //  0048  JMP		#004F
      0xB81A3C00,  //  0049  GETNGBL	R6	K30
      0x601C0018,  //  004A  GETGBL	R7	G24
      0x58200046,  //  004B  LDCONST	R8	K70
      0x7C1C0200,  //  004C  CALL	R7	1
      0x58200006,  //  004D  LDCONST	R8	K6
      0x7C180400,  //  004E  CALL	R6	2
      0xA8040001,  //  004F  EXBLK	1	1
      0x7002000F,  //  0050  JMP		#0061
      0xAC0C0002,  //  0051  CATCH	R3	0	2
      0x7002000C,  //  0052  JMP		#0060
      0xB8163C00,  //  0053  GETNGBL	R5	K30
      0x60180008,  //  0054  GETGBL	R6	G8
      0x5C1C0600,  //  0055  MOVE	R7	R3
      0x7C180200,  //  0056  CALL	R6	1
      0x001A8E06,  //  0057  ADD	R6	K71	R6
      0x00180D22,  //  0058  ADD	R6	R6	K34
      0x601C0008,  //  0059  GETGBL	R7	G8
      0x5C200800,  //  005A  MOVE	R8	R4
      0x7C1C0200,  //  005B  CALL	R7	1
      0x00180C07,  //  005C  ADD	R6	R6	R7
      0x581C0006,  //  005D  LDCONST	R7	K6
      0x7C140400,  //  005E  CALL	R5	2
      0x70020000,  //  005F  JMP		#0061
      0xB0080000,  //  0060  RAISE	2	R0	R0
      0x80000000,  //  0061  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: create_session
********************************************************************/
be_local_closure(class_Matter_Session_Store_create_session,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Session_Store,     /* shared constants */
    be_str_weak(create_session),
    &be_const_str_solidified,
    ( &(const binstruction[21]) {  /* code */
      0x8C0C0109,  //  0000  GETMET	R3	R0	K9
      0x5C140200,  //  0001  MOVE	R5	R1
      0x7C0C0400,  //  0002  CALL	R3	2
      0x4C100000,  //  0003  LDNIL	R4
      0x20100604,  //  0004  NE	R4	R3	R4
      0x78120002,  //  0005  JMPF	R4	#0009
      0x8C100148,  //  0006  GETMET	R4	R0	K72
      0x5C180600,  //  0007  MOVE	R6	R3
      0x7C100400,  //  0008  CALL	R4	2
      0xB8121400,  //  0009  GETNGBL	R4	K10
      0x8C10091A,  //  000A  GETMET	R4	R4	K26
      0x5C180000,  //  000B  MOVE	R6	R0
      0x5C1C0200,  //  000C  MOVE	R7	R1
      0x5C200400,  //  000D  MOVE	R8	R2
      0x7C100800,  //  000E  CALL	R4	4
      0x5C0C0800,  //  000F  MOVE	R3	R4
      0x88100101,  //  0010  GETMBR	R4	R0	K1
      0x8C10091D,  //  0011  GETMET	R4	R4	K29
      0x5C180600,  //  0012  MOVE	R6	R3
      0x7C100400,  //  0013  CALL	R4	2
      0x80040600,  //  0014  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: remove_fabric
********************************************************************/
be_local_closure(class_Matter_Session_Store_remove_fabric,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Session_Store,     /* shared constants */
    be_str_weak(remove_fabric),
    &be_const_str_solidified,
    ( &(const binstruction[34]) {  /* code */
      0x88080101,  //  0000  GETMBR	R2	R0	K1
      0x4C0C0000,  //  0001  LDNIL	R3
      0x20080403,  //  0002  NE	R2	R2	R3
      0x780A0011,  //  0003  JMPF	R2	#0016
      0x58080000,  //  0004  LDCONST	R2	K0
      0x600C000C,  //  0005  GETGBL	R3	G12
      0x88100101,  //  0006  GETMBR	R4	R0	K1
      0x7C0C0200,  //  0007  CALL	R3	1
      0x140C0403,  //  0008  LT	R3	R2	R3
      0x780E000B,  //  0009  JMPF	R3	#0016
      0x880C0101,  //  000A  GETMBR	R3	R0	K1
      0x940C0602,  //  000B  GETIDX	R3	R3	R2
      0x880C0749,  //  000C  GETMBR	R3	R3	K73
      0x1C0C0601,  //  000D  EQ	R3	R3	R1
      0x780E0004,  //  000E  JMPF	R3	#0014
      0x880C0101,  //  000F  GETMBR	R3	R0	K1
      0x8C0C0702,  //  0010  GETMET	R3	R3	K2
      0x5C140400,  //  0011  MOVE	R5	R2
      0x7C0C0400,  //  0012  CALL	R3	2
      0x70020000,  //  0013  JMP		#0015
      0x00080503,  //  0014  ADD	R2	R2	K3
      0x7001FFEE,  //  0015  JMP		#0005
      0x8808010C,  //  0016  GETMBR	R2	R0	K12
      0x4C0C0000,  //  0017  LDNIL	R3
      0x20080403,  //  0018  NE	R2	R2	R3
      0x780A0006,  //  0019  JMPF	R2	#0021
      0x8808010C,  //  001A  GETMBR	R2	R0	K12
      0x8C080502,  //  001B  GETMET	R2	R2	K2
      0x8810010C,  //  001C  GETMBR	R4	R0	K12
      0x8C100918,  //  001D  GETMET	R4	R4	K24
      0x5C180200,  //  001E  MOVE	R6	R1
      0x7C100400,  //  001F  CALL	R4	2
      0x7C080400,  //  0020  CALL	R2	2
      0x80000000,  //  0021  RET	0
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
    be_nested_map(28,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(sessions, -1), be_const_var(1) },
        { be_const_key_weak(remove_fabric, -1), be_const_closure(class_Matter_Session_Store_remove_fabric_closure) },
        { be_const_key_weak(remove_session, -1), be_const_closure(class_Matter_Session_Store_remove_session_closure) },
        { be_const_key_weak(active_fabrics, -1), be_const_closure(class_Matter_Session_Store_active_fabrics_closure) },
        { be_const_key_weak(load_fabrics, 18), be_const_closure(class_Matter_Session_Store_load_fabrics_closure) },
        { be_const_key_weak(fabrics, -1), be_const_var(2) },
        { be_const_key_weak(save_fabrics, -1), be_const_closure(class_Matter_Session_Store_save_fabrics_closure) },
        { be_const_key_weak(_FABRICS_TEMP, 22), be_nested_str_weak(_X2F_matter_fabrics_X2Etmp) },
        { be_const_key_weak(create_fabric, -1), be_const_closure(class_Matter_Session_Store_create_fabric_closure) },
        { be_const_key_weak(find_fabric_by_index, -1), be_const_closure(class_Matter_Session_Store_find_fabric_by_index_closure) },
        { be_const_key_weak(gen_local_session_id, 3), be_const_closure(class_Matter_Session_Store_gen_local_session_id_closure) },
        { be_const_key_weak(sessions_active, 8), be_const_closure(class_Matter_Session_Store_sessions_active_closure) },
        { be_const_key_weak(_FABRICS, -1), be_nested_str_weak(_X2F_matter_fabrics_X2Ejson) },
        { be_const_key_weak(get_session_by_source_node_id, -1), be_const_closure(class_Matter_Session_Store_get_session_by_source_node_id_closure) },
        { be_const_key_weak(count_active_fabrics, 23), be_const_closure(class_Matter_Session_Store_count_active_fabrics_closure) },
        { be_const_key_weak(add_session, 13), be_const_closure(class_Matter_Session_Store_add_session_closure) },
        { be_const_key_weak(find_session_source_id_unsecure, -1), be_const_closure(class_Matter_Session_Store_find_session_source_id_unsecure_closure) },
        { be_const_key_weak(every_second, -1), be_const_closure(class_Matter_Session_Store_every_second_closure) },
        { be_const_key_weak(add_fabric, -1), be_const_closure(class_Matter_Session_Store_add_fabric_closure) },
        { be_const_key_weak(init, -1), be_const_closure(class_Matter_Session_Store_init_closure) },
        { be_const_key_weak(remove_redundant_fabric, -1), be_const_closure(class_Matter_Session_Store_remove_redundant_fabric_closure) },
        { be_const_key_weak(get_session_by_local_session_id, 20), be_const_closure(class_Matter_Session_Store_get_session_by_local_session_id_closure) },
        { be_const_key_weak(remove_expired, -1), be_const_closure(class_Matter_Session_Store_remove_expired_closure) },
        { be_const_key_weak(find_session_by_resumption_id, -1), be_const_closure(class_Matter_Session_Store_find_session_by_resumption_id_closure) },
        { be_const_key_weak(next_fabric_idx, -1), be_const_closure(class_Matter_Session_Store_next_fabric_idx_closure) },
        { be_const_key_weak(find_children_fabrics, 6), be_const_closure(class_Matter_Session_Store_find_children_fabrics_closure) },
        { be_const_key_weak(create_session, -1), be_const_closure(class_Matter_Session_Store_create_session_closure) },
        { be_const_key_weak(device, 1), be_const_var(0) },
    })),
    be_str_weak(Matter_Session_Store)
);
/********************************************************************/
/* End of solidification */
