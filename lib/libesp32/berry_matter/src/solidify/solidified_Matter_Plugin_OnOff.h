/* Solidification of Matter_Plugin_OnOff.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_OnOff;

/********************************************************************
** Solidified function: invoke_request
********************************************************************/
be_local_closure(Matter_Plugin_OnOff_invoke_request,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[13]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(TLV),
    /* K2   */  be_nested_str_weak(cluster),
    /* K3   */  be_nested_str_weak(command),
    /* K4   */  be_const_int(3),
    /* K5   */  be_const_int(0),
    /* K6   */  be_const_int(1),
    /* K7   */  be_nested_str_weak(Matter_TLV_struct),
    /* K8   */  be_nested_str_weak(add_TLV),
    /* K9   */  be_nested_str_weak(U2),
    /* K10  */  be_nested_str_weak(set_onoff),
    /* K11  */  be_const_int(2),
    /* K12  */  be_nested_str_weak(get_onoff),
    }),
    be_str_weak(invoke_request),
    &be_const_str_solidified,
    ( &(const binstruction[119]) {  /* code */
      0xB8120000,  //  0000  GETNGBL	R4	K0
      0x88100901,  //  0001  GETMBR	R4	R4	K1
      0x88140702,  //  0002  GETMBR	R5	R3	K2
      0x88180703,  //  0003  GETMBR	R6	R3	K3
      0x1C1C0B04,  //  0004  EQ	R7	R5	K4
      0x781E0016,  //  0005  JMPF	R7	#001D
      0x1C1C0D05,  //  0006  EQ	R7	R6	K5
      0x781E0002,  //  0007  JMPF	R7	#000B
      0x501C0200,  //  0008  LDBOOL	R7	1	0
      0x80040E00,  //  0009  RET	1	R7
      0x70020010,  //  000A  JMP		#001C
      0x1C1C0D06,  //  000B  EQ	R7	R6	K6
      0x781E0009,  //  000C  JMPF	R7	#0017
      0x8C1C0907,  //  000D  GETMET	R7	R4	K7
      0x7C1C0200,  //  000E  CALL	R7	1
      0x8C200F08,  //  000F  GETMET	R8	R7	K8
      0x58280005,  //  0010  LDCONST	R10	K5
      0x882C0909,  //  0011  GETMBR	R11	R4	K9
      0x58300005,  //  0012  LDCONST	R12	K5
      0x7C200800,  //  0013  CALL	R8	4
      0x900E0705,  //  0014  SETMBR	R3	K3	K5
      0x80040E00,  //  0015  RET	1	R7
      0x70020004,  //  0016  JMP		#001C
      0x541E003F,  //  0017  LDINT	R7	64
      0x1C1C0C07,  //  0018  EQ	R7	R6	R7
      0x781E0001,  //  0019  JMPF	R7	#001C
      0x501C0200,  //  001A  LDBOOL	R7	1	0
      0x80040E00,  //  001B  RET	1	R7
      0x70020058,  //  001C  JMP		#0076
      0x541E0003,  //  001D  LDINT	R7	4
      0x1C1C0A07,  //  001E  EQ	R7	R5	R7
      0x781E0002,  //  001F  JMPF	R7	#0023
      0x501C0200,  //  0020  LDBOOL	R7	1	0
      0x80040E00,  //  0021  RET	1	R7
      0x70020052,  //  0022  JMP		#0076
      0x541E0004,  //  0023  LDINT	R7	5
      0x1C1C0A07,  //  0024  EQ	R7	R5	R7
      0x781E0002,  //  0025  JMPF	R7	#0029
      0x501C0200,  //  0026  LDBOOL	R7	1	0
      0x80040E00,  //  0027  RET	1	R7
      0x7002004C,  //  0028  JMP		#0076
      0x541E0005,  //  0029  LDINT	R7	6
      0x1C1C0A07,  //  002A  EQ	R7	R5	R7
      0x781E001B,  //  002B  JMPF	R7	#0048
      0x1C1C0D05,  //  002C  EQ	R7	R6	K5
      0x781E0005,  //  002D  JMPF	R7	#0034
      0x8C1C010A,  //  002E  GETMET	R7	R0	K10
      0x50240000,  //  002F  LDBOOL	R9	0	0
      0x7C1C0400,  //  0030  CALL	R7	2
      0x501C0200,  //  0031  LDBOOL	R7	1	0
      0x80040E00,  //  0032  RET	1	R7
      0x70020012,  //  0033  JMP		#0047
      0x1C1C0D06,  //  0034  EQ	R7	R6	K6
      0x781E0005,  //  0035  JMPF	R7	#003C
      0x8C1C010A,  //  0036  GETMET	R7	R0	K10
      0x50240200,  //  0037  LDBOOL	R9	1	0
      0x7C1C0400,  //  0038  CALL	R7	2
      0x501C0200,  //  0039  LDBOOL	R7	1	0
      0x80040E00,  //  003A  RET	1	R7
      0x7002000A,  //  003B  JMP		#0047
      0x1C1C0D0B,  //  003C  EQ	R7	R6	K11
      0x781E0008,  //  003D  JMPF	R7	#0047
      0x8C1C010A,  //  003E  GETMET	R7	R0	K10
      0x8C24010C,  //  003F  GETMET	R9	R0	K12
      0x7C240200,  //  0040  CALL	R9	1
      0x78260000,  //  0041  JMPF	R9	#0043
      0x50240001,  //  0042  LDBOOL	R9	0	1
      0x50240200,  //  0043  LDBOOL	R9	1	0
      0x7C1C0400,  //  0044  CALL	R7	2
      0x501C0200,  //  0045  LDBOOL	R7	1	0
      0x80040E00,  //  0046  RET	1	R7
      0x7002002D,  //  0047  JMP		#0076
      0x541E0007,  //  0048  LDINT	R7	8
      0x1C1C0A07,  //  0049  EQ	R7	R5	R7
      0x781E002A,  //  004A  JMPF	R7	#0076
      0x1C1C0D05,  //  004B  EQ	R7	R6	K5
      0x781E0002,  //  004C  JMPF	R7	#0050
      0x501C0200,  //  004D  LDBOOL	R7	1	0
      0x80040E00,  //  004E  RET	1	R7
      0x70020025,  //  004F  JMP		#0076
      0x1C1C0D06,  //  0050  EQ	R7	R6	K6
      0x781E0002,  //  0051  JMPF	R7	#0055
      0x501C0200,  //  0052  LDBOOL	R7	1	0
      0x80040E00,  //  0053  RET	1	R7
      0x70020020,  //  0054  JMP		#0076
      0x1C1C0D0B,  //  0055  EQ	R7	R6	K11
      0x781E0002,  //  0056  JMPF	R7	#005A
      0x501C0200,  //  0057  LDBOOL	R7	1	0
      0x80040E00,  //  0058  RET	1	R7
      0x7002001B,  //  0059  JMP		#0076
      0x1C1C0D04,  //  005A  EQ	R7	R6	K4
      0x781E0002,  //  005B  JMPF	R7	#005F
      0x501C0200,  //  005C  LDBOOL	R7	1	0
      0x80040E00,  //  005D  RET	1	R7
      0x70020016,  //  005E  JMP		#0076
      0x541E0003,  //  005F  LDINT	R7	4
      0x1C1C0C07,  //  0060  EQ	R7	R6	R7
      0x781E0002,  //  0061  JMPF	R7	#0065
      0x501C0200,  //  0062  LDBOOL	R7	1	0
      0x80040E00,  //  0063  RET	1	R7
      0x70020010,  //  0064  JMP		#0076
      0x541E0004,  //  0065  LDINT	R7	5
      0x1C1C0C07,  //  0066  EQ	R7	R6	R7
      0x781E0002,  //  0067  JMPF	R7	#006B
      0x501C0200,  //  0068  LDBOOL	R7	1	0
      0x80040E00,  //  0069  RET	1	R7
      0x7002000A,  //  006A  JMP		#0076
      0x541E0005,  //  006B  LDINT	R7	6
      0x1C1C0C07,  //  006C  EQ	R7	R6	R7
      0x781E0002,  //  006D  JMPF	R7	#0071
      0x501C0200,  //  006E  LDBOOL	R7	1	0
      0x80040E00,  //  006F  RET	1	R7
      0x70020004,  //  0070  JMP		#0076
      0x541E0006,  //  0071  LDINT	R7	7
      0x1C1C0C07,  //  0072  EQ	R7	R6	R7
      0x781E0001,  //  0073  JMPF	R7	#0076
      0x501C0200,  //  0074  LDBOOL	R7	1	0
      0x80040E00,  //  0075  RET	1	R7
      0x80000000,  //  0076  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_onoff
********************************************************************/
be_local_closure(Matter_Plugin_OnOff_get_onoff,   /* name */
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
    /* K0   */  be_nested_str_weak(tasmota),
    /* K1   */  be_nested_str_weak(get_power),
    /* K2   */  be_nested_str_weak(tasmota_relay_index),
    /* K3   */  be_nested_str_weak(shadow_onoff),
    /* K4   */  be_nested_str_weak(onoff_changed),
    }),
    be_str_weak(get_onoff),
    &be_const_str_solidified,
    ( &(const binstruction[28]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x880C0102,  //  0002  GETMBR	R3	R0	K2
      0x7C040400,  //  0003  CALL	R1	2
      0x4C080000,  //  0004  LDNIL	R2
      0x20080202,  //  0005  NE	R2	R1	R2
      0x780A000C,  //  0006  JMPF	R2	#0014
      0x88080103,  //  0007  GETMBR	R2	R0	K3
      0x4C0C0000,  //  0008  LDNIL	R3
      0x20080403,  //  0009  NE	R2	R2	R3
      0x780A0007,  //  000A  JMPF	R2	#0013
      0x88080103,  //  000B  GETMBR	R2	R0	K3
      0x600C0017,  //  000C  GETGBL	R3	G23
      0x5C100200,  //  000D  MOVE	R4	R1
      0x7C0C0200,  //  000E  CALL	R3	1
      0x20080403,  //  000F  NE	R2	R2	R3
      0x780A0001,  //  0010  JMPF	R2	#0013
      0x8C080104,  //  0011  GETMET	R2	R0	K4
      0x7C080200,  //  0012  CALL	R2	1
      0x90020601,  //  0013  SETMBR	R0	K3	R1
      0x88080103,  //  0014  GETMBR	R2	R0	K3
      0x4C0C0000,  //  0015  LDNIL	R3
      0x1C080403,  //  0016  EQ	R2	R2	R3
      0x780A0001,  //  0017  JMPF	R2	#001A
      0x50080000,  //  0018  LDBOOL	R2	0	0
      0x90020602,  //  0019  SETMBR	R0	K3	R2
      0x88080103,  //  001A  GETMBR	R2	R0	K3
      0x80040400,  //  001B  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: onoff_changed
********************************************************************/
be_local_closure(Matter_Plugin_OnOff_onoff_changed,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(attribute_updated),
    /* K1   */  be_const_int(0),
    }),
    be_str_weak(onoff_changed),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x4C0C0000,  //  0001  LDNIL	R3
      0x54120005,  //  0002  LDINT	R4	6
      0x58140001,  //  0003  LDCONST	R5	K1
      0x7C040800,  //  0004  CALL	R1	4
      0x80000000,  //  0005  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_onoff
********************************************************************/
be_local_closure(Matter_Plugin_OnOff_set_onoff,   /* name */
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
    /* K0   */  be_nested_str_weak(tasmota),
    /* K1   */  be_nested_str_weak(set_power),
    /* K2   */  be_nested_str_weak(tasmota_relay_index),
    /* K3   */  be_nested_str_weak(get_onoff),
    }),
    be_str_weak(set_onoff),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0xB80A0000,  //  0000  GETNGBL	R2	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x88100102,  //  0002  GETMBR	R4	R0	K2
      0x60140017,  //  0003  GETGBL	R5	G23
      0x5C180200,  //  0004  MOVE	R6	R1
      0x7C140200,  //  0005  CALL	R5	1
      0x7C080600,  //  0006  CALL	R2	3
      0x8C080103,  //  0007  GETMET	R2	R0	K3
      0x7C080200,  //  0008  CALL	R2	1
      0x80000000,  //  0009  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: read_attribute
********************************************************************/
be_local_closure(Matter_Plugin_OnOff_read_attribute,   /* name */
  be_nested_proto(
    16,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[22]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(TLV),
    /* K3   */  be_nested_str_weak(cluster),
    /* K4   */  be_nested_str_weak(attribute),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(Matter_TLV_array),
    /* K7   */  be_nested_str_weak(TYPES),
    /* K8   */  be_nested_str_weak(keys),
    /* K9   */  be_nested_str_weak(add_struct),
    /* K10  */  be_nested_str_weak(add_TLV),
    /* K11  */  be_nested_str_weak(U2),
    /* K12  */  be_const_int(1),
    /* K13  */  be_nested_str_weak(stop_iteration),
    /* K14  */  be_nested_str_weak(get_cluster_list),
    /* K15  */  be_nested_str_weak(U4),
    /* K16  */  be_const_int(2),
    /* K17  */  be_const_int(3),
    /* K18  */  be_nested_str_weak(create_TLV),
    /* K19  */  be_nested_str_weak(U1),
    /* K20  */  be_nested_str_weak(BOOL),
    /* K21  */  be_nested_str_weak(get_onoff),
    }),
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[256]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xB8120200,  //  0001  GETNGBL	R4	K1
      0x88100902,  //  0002  GETMBR	R4	R4	K2
      0x88140503,  //  0003  GETMBR	R5	R2	K3
      0x88180504,  //  0004  GETMBR	R6	R2	K4
      0x541E001C,  //  0005  LDINT	R7	29
      0x1C1C0A07,  //  0006  EQ	R7	R5	R7
      0x781E0057,  //  0007  JMPF	R7	#0060
      0x1C1C0D05,  //  0008  EQ	R7	R6	K5
      0x781E001C,  //  0009  JMPF	R7	#0027
      0x8C1C0906,  //  000A  GETMET	R7	R4	K6
      0x7C1C0200,  //  000B  CALL	R7	1
      0x60200010,  //  000C  GETGBL	R8	G16
      0x88240107,  //  000D  GETMBR	R9	R0	K7
      0x8C241308,  //  000E  GETMET	R9	R9	K8
      0x7C240200,  //  000F  CALL	R9	1
      0x7C200200,  //  0010  CALL	R8	1
      0xA802000F,  //  0011  EXBLK	0	#0022
      0x5C241000,  //  0012  MOVE	R9	R8
      0x7C240000,  //  0013  CALL	R9	0
      0x8C280F09,  //  0014  GETMET	R10	R7	K9
      0x7C280200,  //  0015  CALL	R10	1
      0x8C2C150A,  //  0016  GETMET	R11	R10	K10
      0x58340005,  //  0017  LDCONST	R13	K5
      0x8838090B,  //  0018  GETMBR	R14	R4	K11
      0x5C3C1200,  //  0019  MOVE	R15	R9
      0x7C2C0800,  //  001A  CALL	R11	4
      0x8C2C150A,  //  001B  GETMET	R11	R10	K10
      0x5834000C,  //  001C  LDCONST	R13	K12
      0x8838090B,  //  001D  GETMBR	R14	R4	K11
      0x883C0107,  //  001E  GETMBR	R15	R0	K7
      0x943C1E09,  //  001F  GETIDX	R15	R15	R9
      0x7C2C0800,  //  0020  CALL	R11	4
      0x7001FFEF,  //  0021  JMP		#0012
      0x5820000D,  //  0022  LDCONST	R8	K13
      0xAC200200,  //  0023  CATCH	R8	1	0
      0xB0080000,  //  0024  RAISE	2	R0	R0
      0x80040E00,  //  0025  RET	1	R7
      0x70020037,  //  0026  JMP		#005F
      0x1C1C0D0C,  //  0027  EQ	R7	R6	K12
      0x781E0013,  //  0028  JMPF	R7	#003D
      0x8C1C0906,  //  0029  GETMET	R7	R4	K6
      0x7C1C0200,  //  002A  CALL	R7	1
      0x60200010,  //  002B  GETGBL	R8	G16
      0x8C24010E,  //  002C  GETMET	R9	R0	K14
      0x7C240200,  //  002D  CALL	R9	1
      0x7C200200,  //  002E  CALL	R8	1
      0xA8020007,  //  002F  EXBLK	0	#0038
      0x5C241000,  //  0030  MOVE	R9	R8
      0x7C240000,  //  0031  CALL	R9	0
      0x8C280F0A,  //  0032  GETMET	R10	R7	K10
      0x4C300000,  //  0033  LDNIL	R12
      0x8834090F,  //  0034  GETMBR	R13	R4	K15
      0x5C381200,  //  0035  MOVE	R14	R9
      0x7C280800,  //  0036  CALL	R10	4
      0x7001FFF7,  //  0037  JMP		#0030
      0x5820000D,  //  0038  LDCONST	R8	K13
      0xAC200200,  //  0039  CATCH	R8	1	0
      0xB0080000,  //  003A  RAISE	2	R0	R0
      0x80040E00,  //  003B  RET	1	R7
      0x70020021,  //  003C  JMP		#005F
      0x1C1C0D10,  //  003D  EQ	R7	R6	K16
      0x781E0008,  //  003E  JMPF	R7	#0048
      0x8C1C0906,  //  003F  GETMET	R7	R4	K6
      0x7C1C0200,  //  0040  CALL	R7	1
      0x8C200F0A,  //  0041  GETMET	R8	R7	K10
      0x4C280000,  //  0042  LDNIL	R10
      0x882C090B,  //  0043  GETMBR	R11	R4	K11
      0x54320005,  //  0044  LDINT	R12	6
      0x7C200800,  //  0045  CALL	R8	4
      0x80040E00,  //  0046  RET	1	R7
      0x70020016,  //  0047  JMP		#005F
      0x1C1C0D11,  //  0048  EQ	R7	R6	K17
      0x781E0003,  //  0049  JMPF	R7	#004E
      0x8C1C0906,  //  004A  GETMET	R7	R4	K6
      0x7C1C0200,  //  004B  CALL	R7	1
      0x80040E00,  //  004C  RET	1	R7
      0x70020010,  //  004D  JMP		#005F
      0x541EFFFB,  //  004E  LDINT	R7	65532
      0x1C1C0C07,  //  004F  EQ	R7	R6	R7
      0x781E0005,  //  0050  JMPF	R7	#0057
      0x8C1C0912,  //  0051  GETMET	R7	R4	K18
      0x8824090F,  //  0052  GETMBR	R9	R4	K15
      0x58280005,  //  0053  LDCONST	R10	K5
      0x7C1C0600,  //  0054  CALL	R7	3
      0x80040E00,  //  0055  RET	1	R7
      0x70020007,  //  0056  JMP		#005F
      0x541EFFFC,  //  0057  LDINT	R7	65533
      0x1C1C0C07,  //  0058  EQ	R7	R6	R7
      0x781E0004,  //  0059  JMPF	R7	#005F
      0x8C1C0912,  //  005A  GETMET	R7	R4	K18
      0x8824090F,  //  005B  GETMBR	R9	R4	K15
      0x5828000C,  //  005C  LDCONST	R10	K12
      0x7C1C0600,  //  005D  CALL	R7	3
      0x80040E00,  //  005E  RET	1	R7
      0x7002009E,  //  005F  JMP		#00FF
      0x1C1C0B11,  //  0060  EQ	R7	R5	K17
      0x781E0021,  //  0061  JMPF	R7	#0084
      0x1C1C0D05,  //  0062  EQ	R7	R6	K5
      0x781E0005,  //  0063  JMPF	R7	#006A
      0x8C1C0912,  //  0064  GETMET	R7	R4	K18
      0x8824090B,  //  0065  GETMBR	R9	R4	K11
      0x58280005,  //  0066  LDCONST	R10	K5
      0x7C1C0600,  //  0067  CALL	R7	3
      0x80040E00,  //  0068  RET	1	R7
      0x70020018,  //  0069  JMP		#0083
      0x1C1C0D0C,  //  006A  EQ	R7	R6	K12
      0x781E0005,  //  006B  JMPF	R7	#0072
      0x8C1C0912,  //  006C  GETMET	R7	R4	K18
      0x88240913,  //  006D  GETMBR	R9	R4	K19
      0x58280005,  //  006E  LDCONST	R10	K5
      0x7C1C0600,  //  006F  CALL	R7	3
      0x80040E00,  //  0070  RET	1	R7
      0x70020010,  //  0071  JMP		#0083
      0x541EFFFB,  //  0072  LDINT	R7	65532
      0x1C1C0C07,  //  0073  EQ	R7	R6	R7
      0x781E0005,  //  0074  JMPF	R7	#007B
      0x8C1C0912,  //  0075  GETMET	R7	R4	K18
      0x8824090F,  //  0076  GETMBR	R9	R4	K15
      0x58280005,  //  0077  LDCONST	R10	K5
      0x7C1C0600,  //  0078  CALL	R7	3
      0x80040E00,  //  0079  RET	1	R7
      0x70020007,  //  007A  JMP		#0083
      0x541EFFFC,  //  007B  LDINT	R7	65533
      0x1C1C0C07,  //  007C  EQ	R7	R6	R7
      0x781E0004,  //  007D  JMPF	R7	#0083
      0x8C1C0912,  //  007E  GETMET	R7	R4	K18
      0x8824090F,  //  007F  GETMBR	R9	R4	K15
      0x542A0003,  //  0080  LDINT	R10	4
      0x7C1C0600,  //  0081  CALL	R7	3
      0x80040E00,  //  0082  RET	1	R7
      0x7002007A,  //  0083  JMP		#00FF
      0x541E0003,  //  0084  LDINT	R7	4
      0x1C1C0A07,  //  0085  EQ	R7	R5	R7
      0x781E0016,  //  0086  JMPF	R7	#009E
      0x1C1C0D05,  //  0087  EQ	R7	R6	K5
      0x781E0002,  //  0088  JMPF	R7	#008C
      0x4C1C0000,  //  0089  LDNIL	R7
      0x80040E00,  //  008A  RET	1	R7
      0x70020010,  //  008B  JMP		#009D
      0x541EFFFB,  //  008C  LDINT	R7	65532
      0x1C1C0C07,  //  008D  EQ	R7	R6	R7
      0x781E0005,  //  008E  JMPF	R7	#0095
      0x8C1C0912,  //  008F  GETMET	R7	R4	K18
      0x8824090F,  //  0090  GETMBR	R9	R4	K15
      0x58280005,  //  0091  LDCONST	R10	K5
      0x7C1C0600,  //  0092  CALL	R7	3
      0x80040E00,  //  0093  RET	1	R7
      0x70020007,  //  0094  JMP		#009D
      0x541EFFFC,  //  0095  LDINT	R7	65533
      0x1C1C0C07,  //  0096  EQ	R7	R6	R7
      0x781E0004,  //  0097  JMPF	R7	#009D
      0x8C1C0912,  //  0098  GETMET	R7	R4	K18
      0x8824090F,  //  0099  GETMBR	R9	R4	K15
      0x542A0003,  //  009A  LDINT	R10	4
      0x7C1C0600,  //  009B  CALL	R7	3
      0x80040E00,  //  009C  RET	1	R7
      0x70020060,  //  009D  JMP		#00FF
      0x541E0004,  //  009E  LDINT	R7	5
      0x1C1C0A07,  //  009F  EQ	R7	R5	R7
      0x781E0011,  //  00A0  JMPF	R7	#00B3
      0x541EFFFB,  //  00A1  LDINT	R7	65532
      0x1C1C0C07,  //  00A2  EQ	R7	R6	R7
      0x781E0005,  //  00A3  JMPF	R7	#00AA
      0x8C1C0912,  //  00A4  GETMET	R7	R4	K18
      0x8824090F,  //  00A5  GETMBR	R9	R4	K15
      0x58280005,  //  00A6  LDCONST	R10	K5
      0x7C1C0600,  //  00A7  CALL	R7	3
      0x80040E00,  //  00A8  RET	1	R7
      0x70020007,  //  00A9  JMP		#00B2
      0x541EFFFC,  //  00AA  LDINT	R7	65533
      0x1C1C0C07,  //  00AB  EQ	R7	R6	R7
      0x781E0004,  //  00AC  JMPF	R7	#00B2
      0x8C1C0912,  //  00AD  GETMET	R7	R4	K18
      0x8824090F,  //  00AE  GETMBR	R9	R4	K15
      0x542A0003,  //  00AF  LDINT	R10	4
      0x7C1C0600,  //  00B0  CALL	R7	3
      0x80040E00,  //  00B1  RET	1	R7
      0x7002004B,  //  00B2  JMP		#00FF
      0x541E0005,  //  00B3  LDINT	R7	6
      0x1C1C0A07,  //  00B4  EQ	R7	R5	R7
      0x781E001A,  //  00B5  JMPF	R7	#00D1
      0x1C1C0D05,  //  00B6  EQ	R7	R6	K5
      0x781E0006,  //  00B7  JMPF	R7	#00BF
      0x8C1C0912,  //  00B8  GETMET	R7	R4	K18
      0x88240914,  //  00B9  GETMBR	R9	R4	K20
      0x8C280115,  //  00BA  GETMET	R10	R0	K21
      0x7C280200,  //  00BB  CALL	R10	1
      0x7C1C0600,  //  00BC  CALL	R7	3
      0x80040E00,  //  00BD  RET	1	R7
      0x70020010,  //  00BE  JMP		#00D0
      0x541EFFFB,  //  00BF  LDINT	R7	65532
      0x1C1C0C07,  //  00C0  EQ	R7	R6	R7
      0x781E0005,  //  00C1  JMPF	R7	#00C8
      0x8C1C0912,  //  00C2  GETMET	R7	R4	K18
      0x8824090F,  //  00C3  GETMBR	R9	R4	K15
      0x58280005,  //  00C4  LDCONST	R10	K5
      0x7C1C0600,  //  00C5  CALL	R7	3
      0x80040E00,  //  00C6  RET	1	R7
      0x70020007,  //  00C7  JMP		#00D0
      0x541EFFFC,  //  00C8  LDINT	R7	65533
      0x1C1C0C07,  //  00C9  EQ	R7	R6	R7
      0x781E0004,  //  00CA  JMPF	R7	#00D0
      0x8C1C0912,  //  00CB  GETMET	R7	R4	K18
      0x8824090F,  //  00CC  GETMBR	R9	R4	K15
      0x542A0003,  //  00CD  LDINT	R10	4
      0x7C1C0600,  //  00CE  CALL	R7	3
      0x80040E00,  //  00CF  RET	1	R7
      0x7002002D,  //  00D0  JMP		#00FF
      0x541E0007,  //  00D1  LDINT	R7	8
      0x1C1C0A07,  //  00D2  EQ	R7	R5	R7
      0x781E002A,  //  00D3  JMPF	R7	#00FF
      0x1C1C0D05,  //  00D4  EQ	R7	R6	K5
      0x781E0005,  //  00D5  JMPF	R7	#00DC
      0x8C1C0912,  //  00D6  GETMET	R7	R4	K18
      0x88240913,  //  00D7  GETMBR	R9	R4	K19
      0x542A0087,  //  00D8  LDINT	R10	136
      0x7C1C0600,  //  00D9  CALL	R7	3
      0x80040E00,  //  00DA  RET	1	R7
      0x70020022,  //  00DB  JMP		#00FF
      0x541E000E,  //  00DC  LDINT	R7	15
      0x1C1C0C07,  //  00DD  EQ	R7	R6	R7
      0x781E0005,  //  00DE  JMPF	R7	#00E5
      0x8C1C0912,  //  00DF  GETMET	R7	R4	K18
      0x88240913,  //  00E0  GETMBR	R9	R4	K19
      0x58280005,  //  00E1  LDCONST	R10	K5
      0x7C1C0600,  //  00E2  CALL	R7	3
      0x80040E00,  //  00E3  RET	1	R7
      0x70020019,  //  00E4  JMP		#00FF
      0x541E000F,  //  00E5  LDINT	R7	16
      0x1C1C0C07,  //  00E6  EQ	R7	R6	R7
      0x781E0005,  //  00E7  JMPF	R7	#00EE
      0x8C1C0912,  //  00E8  GETMET	R7	R4	K18
      0x88240913,  //  00E9  GETMBR	R9	R4	K19
      0x5828000C,  //  00EA  LDCONST	R10	K12
      0x7C1C0600,  //  00EB  CALL	R7	3
      0x80040E00,  //  00EC  RET	1	R7
      0x70020010,  //  00ED  JMP		#00FF
      0x541EFFFB,  //  00EE  LDINT	R7	65532
      0x1C1C0C07,  //  00EF  EQ	R7	R6	R7
      0x781E0005,  //  00F0  JMPF	R7	#00F7
      0x8C1C0912,  //  00F1  GETMET	R7	R4	K18
      0x8824090F,  //  00F2  GETMBR	R9	R4	K15
      0x58280005,  //  00F3  LDCONST	R10	K5
      0x7C1C0600,  //  00F4  CALL	R7	3
      0x80040E00,  //  00F5  RET	1	R7
      0x70020007,  //  00F6  JMP		#00FF
      0x541EFFFC,  //  00F7  LDINT	R7	65533
      0x1C1C0C07,  //  00F8  EQ	R7	R6	R7
      0x781E0004,  //  00F9  JMPF	R7	#00FF
      0x8C1C0912,  //  00FA  GETMET	R7	R4	K18
      0x8824090F,  //  00FB  GETMBR	R9	R4	K15
      0x542A0003,  //  00FC  LDINT	R10	4
      0x7C1C0600,  //  00FD  CALL	R7	3
      0x80040E00,  //  00FE  RET	1	R7
      0x80000000,  //  00FF  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_Plugin_OnOff_init,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(init),
    /* K1   */  be_nested_str_weak(clusters),
    /* K2   */  be_nested_str_weak(CLUSTERS),
    /* K3   */  be_nested_str_weak(get_onoff),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(tasmota_relay_index),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0x60100003,  //  0000  GETGBL	R4	G3
      0x5C140000,  //  0001  MOVE	R5	R0
      0x7C100200,  //  0002  CALL	R4	1
      0x8C100900,  //  0003  GETMET	R4	R4	K0
      0x5C180200,  //  0004  MOVE	R6	R1
      0x5C1C0400,  //  0005  MOVE	R7	R2
      0x7C100600,  //  0006  CALL	R4	3
      0x88100102,  //  0007  GETMBR	R4	R0	K2
      0x90020204,  //  0008  SETMBR	R0	K1	R4
      0x8C100103,  //  0009  GETMET	R4	R0	K3
      0x7C100200,  //  000A  CALL	R4	1
      0x4C100000,  //  000B  LDNIL	R4
      0x1C100604,  //  000C  EQ	R4	R3	R4
      0x78120000,  //  000D  JMPF	R4	#000F
      0x580C0004,  //  000E  LDCONST	R3	K4
      0x90020A03,  //  000F  SETMBR	R0	K5	R3
      0x80000000,  //  0010  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: every_second
********************************************************************/
be_local_closure(Matter_Plugin_OnOff_every_second,   /* name */
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
    /* K0   */  be_nested_str_weak(get_onoff),
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
** Solidified class: Matter_Plugin_OnOff
********************************************************************/
extern const bclass be_class_Matter_Plugin;
be_local_class(Matter_Plugin_OnOff,
    2,
    &be_class_Matter_Plugin,
    be_nested_map(11,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(every_second, -1), be_const_closure(Matter_Plugin_OnOff_every_second_closure) },
        { be_const_key_weak(get_onoff, 8), be_const_closure(Matter_Plugin_OnOff_get_onoff_closure) },
        { be_const_key_weak(init, -1), be_const_closure(Matter_Plugin_OnOff_init_closure) },
        { be_const_key_weak(shadow_onoff, 5), be_const_var(1) },
        { be_const_key_weak(TYPES, 7), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(266, -1), be_const_int(2) },
    }))    ) } )) },
        { be_const_key_weak(read_attribute, -1), be_const_closure(Matter_Plugin_OnOff_read_attribute_closure) },
        { be_const_key_weak(CLUSTERS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(5,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(5, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(8,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(2),
        be_const_int(3),
        be_const_int(4),
        be_const_int(5),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(6, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(3,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(29, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(6,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(2),
        be_const_int(3),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(3, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(4,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(4, 2), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(3,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
    }))    ) } )) },
        { be_const_key_weak(set_onoff, -1), be_const_closure(Matter_Plugin_OnOff_set_onoff_closure) },
        { be_const_key_weak(onoff_changed, -1), be_const_closure(Matter_Plugin_OnOff_onoff_changed_closure) },
        { be_const_key_weak(tasmota_relay_index, 2), be_const_var(0) },
        { be_const_key_weak(invoke_request, 0), be_const_closure(Matter_Plugin_OnOff_invoke_request_closure) },
    })),
    be_str_weak(Matter_Plugin_OnOff)
);
/*******************************************************************/

void be_load_Matter_Plugin_OnOff_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Plugin_OnOff);
    be_setglobal(vm, "Matter_Plugin_OnOff");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
