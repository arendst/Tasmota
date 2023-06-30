/* Solidification of Matter_Plugin_Device.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Device;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_Plugin_Device_init,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(node_label),
    /* K1   */  be_nested_str_weak(find),
    /* K2   */  be_nested_str_weak(name),
    /* K3   */  be_nested_str_weak(),
    /* K4   */  be_nested_str_weak(init),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x8C100701,  //  0000  GETMET	R4	R3	K1
      0x58180002,  //  0001  LDCONST	R6	K2
      0x581C0003,  //  0002  LDCONST	R7	K3
      0x7C100600,  //  0003  CALL	R4	3
      0x90020004,  //  0004  SETMBR	R0	K0	R4
      0x60100003,  //  0005  GETGBL	R4	G3
      0x5C140000,  //  0006  MOVE	R5	R0
      0x7C100200,  //  0007  CALL	R4	1
      0x8C100904,  //  0008  GETMET	R4	R4	K4
      0x5C180200,  //  0009  MOVE	R6	R1
      0x5C1C0400,  //  000A  MOVE	R7	R2
      0x5C200600,  //  000B  MOVE	R8	R3
      0x7C100800,  //  000C  CALL	R4	4
      0x80000000,  //  000D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_name
********************************************************************/
be_local_closure(Matter_Plugin_Device_set_name,   /* name */
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
    /* K0   */  be_nested_str_weak(node_label),
    /* K1   */  be_nested_str_weak(attribute_updated),
    }),
    be_str_weak(set_name),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x20080202,  //  0001  NE	R2	R1	R2
      0x780A0003,  //  0002  JMPF	R2	#0007
      0x8C080101,  //  0003  GETMET	R2	R0	K1
      0x54120038,  //  0004  LDINT	R4	57
      0x54160004,  //  0005  LDINT	R5	5
      0x7C080600,  //  0006  CALL	R2	3
      0x90020001,  //  0007  SETMBR	R0	K0	R1
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_name
********************************************************************/
be_local_closure(Matter_Plugin_Device_get_name,   /* name */
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
    /* K0   */  be_nested_str_weak(node_label),
    }),
    be_str_weak(get_name),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: invoke_request
********************************************************************/
be_local_closure(Matter_Plugin_Device_invoke_request,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[11]) {     /* constants */
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
    /* K10  */  be_nested_str_weak(invoke_request),
    }),
    be_str_weak(invoke_request),
    &be_const_str_solidified,
    ( &(const binstruction[51]) {  /* code */
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
      0x70020014,  //  001C  JMP		#0032
      0x541E0003,  //  001D  LDINT	R7	4
      0x1C1C0A07,  //  001E  EQ	R7	R5	R7
      0x781E0002,  //  001F  JMPF	R7	#0023
      0x501C0200,  //  0020  LDBOOL	R7	1	0
      0x80040E00,  //  0021  RET	1	R7
      0x7002000E,  //  0022  JMP		#0032
      0x541E0004,  //  0023  LDINT	R7	5
      0x1C1C0A07,  //  0024  EQ	R7	R5	R7
      0x781E0002,  //  0025  JMPF	R7	#0029
      0x501C0200,  //  0026  LDBOOL	R7	1	0
      0x80040E00,  //  0027  RET	1	R7
      0x70020008,  //  0028  JMP		#0032
      0x601C0003,  //  0029  GETGBL	R7	G3
      0x5C200000,  //  002A  MOVE	R8	R0
      0x7C1C0200,  //  002B  CALL	R7	1
      0x8C1C0F0A,  //  002C  GETMET	R7	R7	K10
      0x5C240200,  //  002D  MOVE	R9	R1
      0x5C280400,  //  002E  MOVE	R10	R2
      0x5C2C0600,  //  002F  MOVE	R11	R3
      0x7C1C0800,  //  0030  CALL	R7	4
      0x80040E00,  //  0031  RET	1	R7
      0x80000000,  //  0032  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: read_attribute
********************************************************************/
be_local_closure(Matter_Plugin_Device_read_attribute,   /* name */
  be_nested_proto(
    16,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[37]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(TLV),
    /* K2   */  be_nested_str_weak(cluster),
    /* K3   */  be_nested_str_weak(attribute),
    /* K4   */  be_const_int(3),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(create_TLV),
    /* K7   */  be_nested_str_weak(U2),
    /* K8   */  be_const_int(1),
    /* K9   */  be_nested_str_weak(U1),
    /* K10  */  be_nested_str_weak(U4),
    /* K11  */  be_nested_str_weak(Matter_TLV_array),
    /* K12  */  be_nested_str_weak(TYPES),
    /* K13  */  be_nested_str_weak(keys),
    /* K14  */  be_nested_str_weak(add_struct),
    /* K15  */  be_nested_str_weak(add_TLV),
    /* K16  */  be_nested_str_weak(stop_iteration),
    /* K17  */  be_nested_str_weak(NON_BRIDGE_VENDOR),
    /* K18  */  be_nested_str_weak(find),
    /* K19  */  be_nested_str_weak(get_admin_vendor),
    /* K20  */  be_nested_str_weak(device),
    /* K21  */  be_nested_str_weak(disable_bridge_mode),
    /* K22  */  be_nested_str_weak(read_attribute),
    /* K23  */  be_nested_str_weak(string),
    /* K24  */  be_nested_str_weak(UTF1),
    /* K25  */  be_nested_str_weak(tasmota),
    /* K26  */  be_nested_str_weak(cmd),
    /* K27  */  be_nested_str_weak(DeviceName),
    /* K28  */  be_nested_str_weak(get_name),
    /* K29  */  be_nested_str_weak(Status_X202),
    /* K30  */  be_nested_str_weak(StatusFWR),
    /* K31  */  be_nested_str_weak(Version),
    /* K32  */  be_nested_str_weak(_X28),
    /* K33  */  be_nested_str_weak(wifi),
    /* K34  */  be_nested_str_weak(mac),
    /* K35  */  be_nested_str_weak(),
    /* K36  */  be_nested_str_weak(BOOL),
    }),
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[261]) {  /* code */
      0xB80E0000,  //  0000  GETNGBL	R3	K0
      0x880C0701,  //  0001  GETMBR	R3	R3	K1
      0x88100502,  //  0002  GETMBR	R4	R2	K2
      0x88140503,  //  0003  GETMBR	R5	R2	K3
      0x1C180904,  //  0004  EQ	R6	R4	K4
      0x781A0021,  //  0005  JMPF	R6	#0028
      0x1C180B05,  //  0006  EQ	R6	R5	K5
      0x781A0005,  //  0007  JMPF	R6	#000E
      0x8C180706,  //  0008  GETMET	R6	R3	K6
      0x88200707,  //  0009  GETMBR	R8	R3	K7
      0x58240005,  //  000A  LDCONST	R9	K5
      0x7C180600,  //  000B  CALL	R6	3
      0x80040C00,  //  000C  RET	1	R6
      0x70020018,  //  000D  JMP		#0027
      0x1C180B08,  //  000E  EQ	R6	R5	K8
      0x781A0005,  //  000F  JMPF	R6	#0016
      0x8C180706,  //  0010  GETMET	R6	R3	K6
      0x88200709,  //  0011  GETMBR	R8	R3	K9
      0x58240005,  //  0012  LDCONST	R9	K5
      0x7C180600,  //  0013  CALL	R6	3
      0x80040C00,  //  0014  RET	1	R6
      0x70020010,  //  0015  JMP		#0027
      0x541AFFFB,  //  0016  LDINT	R6	65532
      0x1C180A06,  //  0017  EQ	R6	R5	R6
      0x781A0005,  //  0018  JMPF	R6	#001F
      0x8C180706,  //  0019  GETMET	R6	R3	K6
      0x8820070A,  //  001A  GETMBR	R8	R3	K10
      0x58240005,  //  001B  LDCONST	R9	K5
      0x7C180600,  //  001C  CALL	R6	3
      0x80040C00,  //  001D  RET	1	R6
      0x70020007,  //  001E  JMP		#0027
      0x541AFFFC,  //  001F  LDINT	R6	65533
      0x1C180A06,  //  0020  EQ	R6	R5	R6
      0x781A0004,  //  0021  JMPF	R6	#0027
      0x8C180706,  //  0022  GETMET	R6	R3	K6
      0x8820070A,  //  0023  GETMBR	R8	R3	K10
      0x54260003,  //  0024  LDINT	R9	4
      0x7C180600,  //  0025  CALL	R6	3
      0x80040C00,  //  0026  RET	1	R6
      0x700200DB,  //  0027  JMP		#0104
      0x541A0003,  //  0028  LDINT	R6	4
      0x1C180806,  //  0029  EQ	R6	R4	R6
      0x781A0016,  //  002A  JMPF	R6	#0042
      0x1C180B05,  //  002B  EQ	R6	R5	K5
      0x781A0002,  //  002C  JMPF	R6	#0030
      0x4C180000,  //  002D  LDNIL	R6
      0x80040C00,  //  002E  RET	1	R6
      0x70020010,  //  002F  JMP		#0041
      0x541AFFFB,  //  0030  LDINT	R6	65532
      0x1C180A06,  //  0031  EQ	R6	R5	R6
      0x781A0005,  //  0032  JMPF	R6	#0039
      0x8C180706,  //  0033  GETMET	R6	R3	K6
      0x8820070A,  //  0034  GETMBR	R8	R3	K10
      0x58240005,  //  0035  LDCONST	R9	K5
      0x7C180600,  //  0036  CALL	R6	3
      0x80040C00,  //  0037  RET	1	R6
      0x70020007,  //  0038  JMP		#0041
      0x541AFFFC,  //  0039  LDINT	R6	65533
      0x1C180A06,  //  003A  EQ	R6	R5	R6
      0x781A0004,  //  003B  JMPF	R6	#0041
      0x8C180706,  //  003C  GETMET	R6	R3	K6
      0x8820070A,  //  003D  GETMBR	R8	R3	K10
      0x54260003,  //  003E  LDINT	R9	4
      0x7C180600,  //  003F  CALL	R6	3
      0x80040C00,  //  0040  RET	1	R6
      0x700200C1,  //  0041  JMP		#0104
      0x541A0004,  //  0042  LDINT	R6	5
      0x1C180806,  //  0043  EQ	R6	R4	R6
      0x781A0011,  //  0044  JMPF	R6	#0057
      0x541AFFFB,  //  0045  LDINT	R6	65532
      0x1C180A06,  //  0046  EQ	R6	R5	R6
      0x781A0005,  //  0047  JMPF	R6	#004E
      0x8C180706,  //  0048  GETMET	R6	R3	K6
      0x8820070A,  //  0049  GETMBR	R8	R3	K10
      0x58240005,  //  004A  LDCONST	R9	K5
      0x7C180600,  //  004B  CALL	R6	3
      0x80040C00,  //  004C  RET	1	R6
      0x70020007,  //  004D  JMP		#0056
      0x541AFFFC,  //  004E  LDINT	R6	65533
      0x1C180A06,  //  004F  EQ	R6	R5	R6
      0x781A0004,  //  0050  JMPF	R6	#0056
      0x8C180706,  //  0051  GETMET	R6	R3	K6
      0x8820070A,  //  0052  GETMBR	R8	R3	K10
      0x54260003,  //  0053  LDINT	R9	4
      0x7C180600,  //  0054  CALL	R6	3
      0x80040C00,  //  0055  RET	1	R6
      0x700200AC,  //  0056  JMP		#0104
      0x541A001C,  //  0057  LDINT	R6	29
      0x1C180806,  //  0058  EQ	R6	R4	R6
      0x781A003D,  //  0059  JMPF	R6	#0098
      0x1C180B05,  //  005A  EQ	R6	R5	K5
      0x781A0032,  //  005B  JMPF	R6	#008F
      0x8C18070B,  //  005C  GETMET	R6	R3	K11
      0x7C180200,  //  005D  CALL	R6	1
      0x881C010C,  //  005E  GETMBR	R7	R0	K12
      0x60200010,  //  005F  GETGBL	R8	G16
      0x8C240F0D,  //  0060  GETMET	R9	R7	K13
      0x7C240200,  //  0061  CALL	R9	1
      0x7C200200,  //  0062  CALL	R8	1
      0xA802000E,  //  0063  EXBLK	0	#0073
      0x5C241000,  //  0064  MOVE	R9	R8
      0x7C240000,  //  0065  CALL	R9	0
      0x8C280D0E,  //  0066  GETMET	R10	R6	K14
      0x7C280200,  //  0067  CALL	R10	1
      0x8C2C150F,  //  0068  GETMET	R11	R10	K15
      0x58340005,  //  0069  LDCONST	R13	K5
      0x88380707,  //  006A  GETMBR	R14	R3	K7
      0x5C3C1200,  //  006B  MOVE	R15	R9
      0x7C2C0800,  //  006C  CALL	R11	4
      0x8C2C150F,  //  006D  GETMET	R11	R10	K15
      0x58340008,  //  006E  LDCONST	R13	K8
      0x88380707,  //  006F  GETMBR	R14	R3	K7
      0x943C0E09,  //  0070  GETIDX	R15	R7	R9
      0x7C2C0800,  //  0071  CALL	R11	4
      0x7001FFF0,  //  0072  JMP		#0064
      0x58200010,  //  0073  LDCONST	R8	K16
      0xAC200200,  //  0074  CATCH	R8	1	0
      0xB0080000,  //  0075  RAISE	2	R0	R0
      0x88200111,  //  0076  GETMBR	R8	R0	K17
      0x8C201112,  //  0077  GETMET	R8	R8	K18
      0x8C280313,  //  0078  GETMET	R10	R1	K19
      0x7C280200,  //  0079  CALL	R10	1
      0x7C200400,  //  007A  CALL	R8	2
      0x4C240000,  //  007B  LDNIL	R9
      0x1C201009,  //  007C  EQ	R8	R8	R9
      0x7822000E,  //  007D  JMPF	R8	#008D
      0x88200114,  //  007E  GETMBR	R8	R0	K20
      0x88201115,  //  007F  GETMBR	R8	R8	K21
      0x7422000B,  //  0080  JMPT	R8	#008D
      0x8C200D0E,  //  0081  GETMET	R8	R6	K14
      0x7C200200,  //  0082  CALL	R8	1
      0x8C24110F,  //  0083  GETMET	R9	R8	K15
      0x582C0005,  //  0084  LDCONST	R11	K5
      0x88300707,  //  0085  GETMBR	R12	R3	K7
      0x54360012,  //  0086  LDINT	R13	19
      0x7C240800,  //  0087  CALL	R9	4
      0x8C24110F,  //  0088  GETMET	R9	R8	K15
      0x582C0008,  //  0089  LDCONST	R11	K8
      0x88300707,  //  008A  GETMBR	R12	R3	K7
      0x58340008,  //  008B  LDCONST	R13	K8
      0x7C240800,  //  008C  CALL	R9	4
      0x80040C00,  //  008D  RET	1	R6
      0x70020007,  //  008E  JMP		#0097
      0x60180003,  //  008F  GETGBL	R6	G3
      0x5C1C0000,  //  0090  MOVE	R7	R0
      0x7C180200,  //  0091  CALL	R6	1
      0x8C180D16,  //  0092  GETMET	R6	R6	K22
      0x5C200200,  //  0093  MOVE	R8	R1
      0x5C240400,  //  0094  MOVE	R9	R2
      0x7C180600,  //  0095  CALL	R6	3
      0x80040C00,  //  0096  RET	1	R6
      0x7002006B,  //  0097  JMP		#0104
      0x541A0038,  //  0098  LDINT	R6	57
      0x1C180806,  //  0099  EQ	R6	R4	R6
      0x781A0060,  //  009A  JMPF	R6	#00FC
      0xA41A2E00,  //  009B  IMPORT	R6	K23
      0x1C1C0B04,  //  009C  EQ	R7	R5	K4
      0x781E000A,  //  009D  JMPF	R7	#00A9
      0x8C1C0706,  //  009E  GETMET	R7	R3	K6
      0x88240718,  //  009F  GETMBR	R9	R3	K24
      0xB82A3200,  //  00A0  GETNGBL	R10	K25
      0x8C28151A,  //  00A1  GETMET	R10	R10	K26
      0x5830001B,  //  00A2  LDCONST	R12	K27
      0x50340200,  //  00A3  LDBOOL	R13	1	0
      0x7C280600,  //  00A4  CALL	R10	3
      0x9428151B,  //  00A5  GETIDX	R10	R10	K27
      0x7C1C0600,  //  00A6  CALL	R7	3
      0x80040E00,  //  00A7  RET	1	R7
      0x70020051,  //  00A8  JMP		#00FB
      0x541E0004,  //  00A9  LDINT	R7	5
      0x1C1C0A07,  //  00AA  EQ	R7	R5	R7
      0x781E0006,  //  00AB  JMPF	R7	#00B3
      0x8C1C0706,  //  00AC  GETMET	R7	R3	K6
      0x88240718,  //  00AD  GETMBR	R9	R3	K24
      0x8C28011C,  //  00AE  GETMET	R10	R0	K28
      0x7C280200,  //  00AF  CALL	R10	1
      0x7C1C0600,  //  00B0  CALL	R7	3
      0x80040E00,  //  00B1  RET	1	R7
      0x70020047,  //  00B2  JMP		#00FB
      0x541E0009,  //  00B3  LDINT	R7	10
      0x1C1C0A07,  //  00B4  EQ	R7	R5	R7
      0x781E0015,  //  00B5  JMPF	R7	#00CC
      0xB81E3200,  //  00B6  GETNGBL	R7	K25
      0x8C1C0F1A,  //  00B7  GETMET	R7	R7	K26
      0x5824001D,  //  00B8  LDCONST	R9	K29
      0x50280200,  //  00B9  LDBOOL	R10	1	0
      0x7C1C0600,  //  00BA  CALL	R7	3
      0x941C0F1E,  //  00BB  GETIDX	R7	R7	K30
      0x941C0F1F,  //  00BC  GETIDX	R7	R7	K31
      0x8C200D12,  //  00BD  GETMET	R8	R6	K18
      0x5C280E00,  //  00BE  MOVE	R10	R7
      0x582C0020,  //  00BF  LDCONST	R11	K32
      0x7C200600,  //  00C0  CALL	R8	3
      0x24241105,  //  00C1  GT	R9	R8	K5
      0x78260002,  //  00C2  JMPF	R9	#00C6
      0x04241108,  //  00C3  SUB	R9	R8	K8
      0x40260A09,  //  00C4  CONNECT	R9	K5	R9
      0x941C0E09,  //  00C5  GETIDX	R7	R7	R9
      0x8C240706,  //  00C6  GETMET	R9	R3	K6
      0x882C0718,  //  00C7  GETMBR	R11	R3	K24
      0x5C300E00,  //  00C8  MOVE	R12	R7
      0x7C240600,  //  00C9  CALL	R9	3
      0x80041200,  //  00CA  RET	1	R9
      0x7002002E,  //  00CB  JMP		#00FB
      0x541E000E,  //  00CC  LDINT	R7	15
      0x1C1C0A07,  //  00CD  EQ	R7	R5	R7
      0x781E000B,  //  00CE  JMPF	R7	#00DB
      0x8C1C0706,  //  00CF  GETMET	R7	R3	K6
      0x88240718,  //  00D0  GETMBR	R9	R3	K24
      0xB82A3200,  //  00D1  GETNGBL	R10	K25
      0x8C281521,  //  00D2  GETMET	R10	R10	K33
      0x7C280200,  //  00D3  CALL	R10	1
      0x8C281512,  //  00D4  GETMET	R10	R10	K18
      0x58300022,  //  00D5  LDCONST	R12	K34
      0x58340023,  //  00D6  LDCONST	R13	K35
      0x7C280600,  //  00D7  CALL	R10	3
      0x7C1C0600,  //  00D8  CALL	R7	3
      0x80040E00,  //  00D9  RET	1	R7
      0x7002001F,  //  00DA  JMP		#00FB
      0x541E0010,  //  00DB  LDINT	R7	17
      0x1C1C0A07,  //  00DC  EQ	R7	R5	R7
      0x781E0005,  //  00DD  JMPF	R7	#00E4
      0x8C1C0706,  //  00DE  GETMET	R7	R3	K6
      0x88240724,  //  00DF  GETMBR	R9	R3	K36
      0x58280008,  //  00E0  LDCONST	R10	K8
      0x7C1C0600,  //  00E1  CALL	R7	3
      0x80040E00,  //  00E2  RET	1	R7
      0x70020016,  //  00E3  JMP		#00FB
      0x541E0011,  //  00E4  LDINT	R7	18
      0x1C1C0A07,  //  00E5  EQ	R7	R5	R7
      0x781E000B,  //  00E6  JMPF	R7	#00F3
      0x8C1C0706,  //  00E7  GETMET	R7	R3	K6
      0x88240718,  //  00E8  GETMBR	R9	R3	K24
      0xB82A3200,  //  00E9  GETNGBL	R10	K25
      0x8C281521,  //  00EA  GETMET	R10	R10	K33
      0x7C280200,  //  00EB  CALL	R10	1
      0x8C281512,  //  00EC  GETMET	R10	R10	K18
      0x58300022,  //  00ED  LDCONST	R12	K34
      0x58340023,  //  00EE  LDCONST	R13	K35
      0x7C280600,  //  00EF  CALL	R10	3
      0x7C1C0600,  //  00F0  CALL	R7	3
      0x80040E00,  //  00F1  RET	1	R7
      0x70020007,  //  00F2  JMP		#00FB
      0x601C0003,  //  00F3  GETGBL	R7	G3
      0x5C200000,  //  00F4  MOVE	R8	R0
      0x7C1C0200,  //  00F5  CALL	R7	1
      0x8C1C0F16,  //  00F6  GETMET	R7	R7	K22
      0x5C240200,  //  00F7  MOVE	R9	R1
      0x5C280400,  //  00F8  MOVE	R10	R2
      0x7C1C0600,  //  00F9  CALL	R7	3
      0x80040E00,  //  00FA  RET	1	R7
      0x70020007,  //  00FB  JMP		#0104
      0x60180003,  //  00FC  GETGBL	R6	G3
      0x5C1C0000,  //  00FD  MOVE	R7	R0
      0x7C180200,  //  00FE  CALL	R6	1
      0x8C180D16,  //  00FF  GETMET	R6	R6	K22
      0x5C200200,  //  0100  MOVE	R8	R1
      0x5C240400,  //  0101  MOVE	R9	R2
      0x7C180600,  //  0102  CALL	R6	3
      0x80040C00,  //  0103  RET	1	R6
      0x80000000,  //  0104  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Plugin_Device
********************************************************************/
extern const bclass be_class_Matter_Plugin;
be_local_class(Matter_Plugin_Device,
    1,
    &be_class_Matter_Plugin,
    be_nested_map(9,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(NON_BRIDGE_VENDOR, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(2,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(4631),
        be_const_int(4993),
    }))    ) } )) },
        { be_const_key_weak(invoke_request, -1), be_const_closure(Matter_Plugin_Device_invoke_request_closure) },
        { be_const_key_weak(TYPES, 0), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(19, -1), be_const_int(1) },
    }))    ) } )) },
        { be_const_key_weak(read_attribute, -1), be_const_closure(Matter_Plugin_Device_read_attribute_closure) },
        { be_const_key_weak(set_name, 8), be_const_closure(Matter_Plugin_Device_set_name_closure) },
        { be_const_key_weak(get_name, 1), be_const_closure(Matter_Plugin_Device_get_name_closure) },
        { be_const_key_weak(init, 3), be_const_closure(Matter_Plugin_Device_init_closure) },
        { be_const_key_weak(node_label, -1), be_const_var(0) },
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
        { be_const_key_int(57, 2), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(6,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(3),
        be_const_int(5),
        be_const_int(10),
        be_const_int(15),
        be_const_int(17),
        be_const_int(18),
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
        { be_const_key_int(3, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(4,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
    }))    ) } )) },
    })),
    be_str_weak(Matter_Plugin_Device)
);
/*******************************************************************/

void be_load_Matter_Plugin_Device_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Plugin_Device);
    be_setglobal(vm, "Matter_Plugin_Device");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
