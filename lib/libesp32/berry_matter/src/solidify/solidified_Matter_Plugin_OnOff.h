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
    12,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[15]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(TLV),
    /* K3   */  be_nested_str_weak(cluster),
    /* K4   */  be_nested_str_weak(attribute),
    /* K5   */  be_const_int(3),
    /* K6   */  be_const_int(0),
    /* K7   */  be_nested_str_weak(create_TLV),
    /* K8   */  be_nested_str_weak(U2),
    /* K9   */  be_const_int(1),
    /* K10  */  be_nested_str_weak(U1),
    /* K11  */  be_nested_str_weak(U4),
    /* K12  */  be_nested_str_weak(BOOL),
    /* K13  */  be_nested_str_weak(get_onoff),
    /* K14  */  be_nested_str_weak(read_attribute),
    }),
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[174]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xB8120200,  //  0001  GETNGBL	R4	K1
      0x88100902,  //  0002  GETMBR	R4	R4	K2
      0x88140503,  //  0003  GETMBR	R5	R2	K3
      0x88180504,  //  0004  GETMBR	R6	R2	K4
      0x1C1C0B05,  //  0005  EQ	R7	R5	K5
      0x781E0021,  //  0006  JMPF	R7	#0029
      0x1C1C0D06,  //  0007  EQ	R7	R6	K6
      0x781E0005,  //  0008  JMPF	R7	#000F
      0x8C1C0907,  //  0009  GETMET	R7	R4	K7
      0x88240908,  //  000A  GETMBR	R9	R4	K8
      0x58280006,  //  000B  LDCONST	R10	K6
      0x7C1C0600,  //  000C  CALL	R7	3
      0x80040E00,  //  000D  RET	1	R7
      0x70020018,  //  000E  JMP		#0028
      0x1C1C0D09,  //  000F  EQ	R7	R6	K9
      0x781E0005,  //  0010  JMPF	R7	#0017
      0x8C1C0907,  //  0011  GETMET	R7	R4	K7
      0x8824090A,  //  0012  GETMBR	R9	R4	K10
      0x58280006,  //  0013  LDCONST	R10	K6
      0x7C1C0600,  //  0014  CALL	R7	3
      0x80040E00,  //  0015  RET	1	R7
      0x70020010,  //  0016  JMP		#0028
      0x541EFFFB,  //  0017  LDINT	R7	65532
      0x1C1C0C07,  //  0018  EQ	R7	R6	R7
      0x781E0005,  //  0019  JMPF	R7	#0020
      0x8C1C0907,  //  001A  GETMET	R7	R4	K7
      0x8824090B,  //  001B  GETMBR	R9	R4	K11
      0x58280006,  //  001C  LDCONST	R10	K6
      0x7C1C0600,  //  001D  CALL	R7	3
      0x80040E00,  //  001E  RET	1	R7
      0x70020007,  //  001F  JMP		#0028
      0x541EFFFC,  //  0020  LDINT	R7	65533
      0x1C1C0C07,  //  0021  EQ	R7	R6	R7
      0x781E0004,  //  0022  JMPF	R7	#0028
      0x8C1C0907,  //  0023  GETMET	R7	R4	K7
      0x8824090B,  //  0024  GETMBR	R9	R4	K11
      0x542A0003,  //  0025  LDINT	R10	4
      0x7C1C0600,  //  0026  CALL	R7	3
      0x80040E00,  //  0027  RET	1	R7
      0x70020083,  //  0028  JMP		#00AD
      0x541E0003,  //  0029  LDINT	R7	4
      0x1C1C0A07,  //  002A  EQ	R7	R5	R7
      0x781E0016,  //  002B  JMPF	R7	#0043
      0x1C1C0D06,  //  002C  EQ	R7	R6	K6
      0x781E0002,  //  002D  JMPF	R7	#0031
      0x4C1C0000,  //  002E  LDNIL	R7
      0x80040E00,  //  002F  RET	1	R7
      0x70020010,  //  0030  JMP		#0042
      0x541EFFFB,  //  0031  LDINT	R7	65532
      0x1C1C0C07,  //  0032  EQ	R7	R6	R7
      0x781E0005,  //  0033  JMPF	R7	#003A
      0x8C1C0907,  //  0034  GETMET	R7	R4	K7
      0x8824090B,  //  0035  GETMBR	R9	R4	K11
      0x58280006,  //  0036  LDCONST	R10	K6
      0x7C1C0600,  //  0037  CALL	R7	3
      0x80040E00,  //  0038  RET	1	R7
      0x70020007,  //  0039  JMP		#0042
      0x541EFFFC,  //  003A  LDINT	R7	65533
      0x1C1C0C07,  //  003B  EQ	R7	R6	R7
      0x781E0004,  //  003C  JMPF	R7	#0042
      0x8C1C0907,  //  003D  GETMET	R7	R4	K7
      0x8824090B,  //  003E  GETMBR	R9	R4	K11
      0x542A0003,  //  003F  LDINT	R10	4
      0x7C1C0600,  //  0040  CALL	R7	3
      0x80040E00,  //  0041  RET	1	R7
      0x70020069,  //  0042  JMP		#00AD
      0x541E0004,  //  0043  LDINT	R7	5
      0x1C1C0A07,  //  0044  EQ	R7	R5	R7
      0x781E0011,  //  0045  JMPF	R7	#0058
      0x541EFFFB,  //  0046  LDINT	R7	65532
      0x1C1C0C07,  //  0047  EQ	R7	R6	R7
      0x781E0005,  //  0048  JMPF	R7	#004F
      0x8C1C0907,  //  0049  GETMET	R7	R4	K7
      0x8824090B,  //  004A  GETMBR	R9	R4	K11
      0x58280006,  //  004B  LDCONST	R10	K6
      0x7C1C0600,  //  004C  CALL	R7	3
      0x80040E00,  //  004D  RET	1	R7
      0x70020007,  //  004E  JMP		#0057
      0x541EFFFC,  //  004F  LDINT	R7	65533
      0x1C1C0C07,  //  0050  EQ	R7	R6	R7
      0x781E0004,  //  0051  JMPF	R7	#0057
      0x8C1C0907,  //  0052  GETMET	R7	R4	K7
      0x8824090B,  //  0053  GETMBR	R9	R4	K11
      0x542A0003,  //  0054  LDINT	R10	4
      0x7C1C0600,  //  0055  CALL	R7	3
      0x80040E00,  //  0056  RET	1	R7
      0x70020054,  //  0057  JMP		#00AD
      0x541E0005,  //  0058  LDINT	R7	6
      0x1C1C0A07,  //  0059  EQ	R7	R5	R7
      0x781E001A,  //  005A  JMPF	R7	#0076
      0x1C1C0D06,  //  005B  EQ	R7	R6	K6
      0x781E0006,  //  005C  JMPF	R7	#0064
      0x8C1C0907,  //  005D  GETMET	R7	R4	K7
      0x8824090C,  //  005E  GETMBR	R9	R4	K12
      0x8C28010D,  //  005F  GETMET	R10	R0	K13
      0x7C280200,  //  0060  CALL	R10	1
      0x7C1C0600,  //  0061  CALL	R7	3
      0x80040E00,  //  0062  RET	1	R7
      0x70020010,  //  0063  JMP		#0075
      0x541EFFFB,  //  0064  LDINT	R7	65532
      0x1C1C0C07,  //  0065  EQ	R7	R6	R7
      0x781E0005,  //  0066  JMPF	R7	#006D
      0x8C1C0907,  //  0067  GETMET	R7	R4	K7
      0x8824090B,  //  0068  GETMBR	R9	R4	K11
      0x58280006,  //  0069  LDCONST	R10	K6
      0x7C1C0600,  //  006A  CALL	R7	3
      0x80040E00,  //  006B  RET	1	R7
      0x70020007,  //  006C  JMP		#0075
      0x541EFFFC,  //  006D  LDINT	R7	65533
      0x1C1C0C07,  //  006E  EQ	R7	R6	R7
      0x781E0004,  //  006F  JMPF	R7	#0075
      0x8C1C0907,  //  0070  GETMET	R7	R4	K7
      0x8824090B,  //  0071  GETMBR	R9	R4	K11
      0x542A0003,  //  0072  LDINT	R10	4
      0x7C1C0600,  //  0073  CALL	R7	3
      0x80040E00,  //  0074  RET	1	R7
      0x70020036,  //  0075  JMP		#00AD
      0x541E0007,  //  0076  LDINT	R7	8
      0x1C1C0A07,  //  0077  EQ	R7	R5	R7
      0x781E002B,  //  0078  JMPF	R7	#00A5
      0x1C1C0D06,  //  0079  EQ	R7	R6	K6
      0x781E0005,  //  007A  JMPF	R7	#0081
      0x8C1C0907,  //  007B  GETMET	R7	R4	K7
      0x8824090A,  //  007C  GETMBR	R9	R4	K10
      0x542A0087,  //  007D  LDINT	R10	136
      0x7C1C0600,  //  007E  CALL	R7	3
      0x80040E00,  //  007F  RET	1	R7
      0x70020022,  //  0080  JMP		#00A4
      0x541E000E,  //  0081  LDINT	R7	15
      0x1C1C0C07,  //  0082  EQ	R7	R6	R7
      0x781E0005,  //  0083  JMPF	R7	#008A
      0x8C1C0907,  //  0084  GETMET	R7	R4	K7
      0x8824090A,  //  0085  GETMBR	R9	R4	K10
      0x58280006,  //  0086  LDCONST	R10	K6
      0x7C1C0600,  //  0087  CALL	R7	3
      0x80040E00,  //  0088  RET	1	R7
      0x70020019,  //  0089  JMP		#00A4
      0x541E000F,  //  008A  LDINT	R7	16
      0x1C1C0C07,  //  008B  EQ	R7	R6	R7
      0x781E0005,  //  008C  JMPF	R7	#0093
      0x8C1C0907,  //  008D  GETMET	R7	R4	K7
      0x8824090A,  //  008E  GETMBR	R9	R4	K10
      0x58280009,  //  008F  LDCONST	R10	K9
      0x7C1C0600,  //  0090  CALL	R7	3
      0x80040E00,  //  0091  RET	1	R7
      0x70020010,  //  0092  JMP		#00A4
      0x541EFFFB,  //  0093  LDINT	R7	65532
      0x1C1C0C07,  //  0094  EQ	R7	R6	R7
      0x781E0005,  //  0095  JMPF	R7	#009C
      0x8C1C0907,  //  0096  GETMET	R7	R4	K7
      0x8824090B,  //  0097  GETMBR	R9	R4	K11
      0x58280006,  //  0098  LDCONST	R10	K6
      0x7C1C0600,  //  0099  CALL	R7	3
      0x80040E00,  //  009A  RET	1	R7
      0x70020007,  //  009B  JMP		#00A4
      0x541EFFFC,  //  009C  LDINT	R7	65533
      0x1C1C0C07,  //  009D  EQ	R7	R6	R7
      0x781E0004,  //  009E  JMPF	R7	#00A4
      0x8C1C0907,  //  009F  GETMET	R7	R4	K7
      0x8824090B,  //  00A0  GETMBR	R9	R4	K11
      0x542A0003,  //  00A1  LDINT	R10	4
      0x7C1C0600,  //  00A2  CALL	R7	3
      0x80040E00,  //  00A3  RET	1	R7
      0x70020007,  //  00A4  JMP		#00AD
      0x601C0003,  //  00A5  GETGBL	R7	G3
      0x5C200000,  //  00A6  MOVE	R8	R0
      0x7C1C0200,  //  00A7  CALL	R7	1
      0x8C1C0F0E,  //  00A8  GETMET	R7	R7	K14
      0x5C240200,  //  00A9  MOVE	R9	R1
      0x5C280400,  //  00AA  MOVE	R10	R2
      0x7C1C0600,  //  00AB  CALL	R7	3
      0x80040E00,  //  00AC  RET	1	R7
      0x80000000,  //  00AD  RET	0
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
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(init),
    /* K1   */  be_nested_str_weak(get_onoff),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str_weak(tasmota_relay_index),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x60100003,  //  0000  GETGBL	R4	G3
      0x5C140000,  //  0001  MOVE	R5	R0
      0x7C100200,  //  0002  CALL	R4	1
      0x8C100900,  //  0003  GETMET	R4	R4	K0
      0x5C180200,  //  0004  MOVE	R6	R1
      0x5C1C0400,  //  0005  MOVE	R7	R2
      0x7C100600,  //  0006  CALL	R4	3
      0x8C100101,  //  0007  GETMET	R4	R0	K1
      0x7C100200,  //  0008  CALL	R4	1
      0x4C100000,  //  0009  LDNIL	R4
      0x1C100604,  //  000A  EQ	R4	R3	R4
      0x78120000,  //  000B  JMPF	R4	#000D
      0x580C0002,  //  000C  LDCONST	R3	K2
      0x90020603,  //  000D  SETMBR	R0	K3	R3
      0x80000000,  //  000E  RET	0
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
        be_const_map( *     be_nested_map(4,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(4, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(3,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
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
        { be_const_key_int(3, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(4,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
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
