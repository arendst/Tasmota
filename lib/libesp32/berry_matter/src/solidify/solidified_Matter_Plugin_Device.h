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
    17,                          /* nstack */
    4,                          /* argc */
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
    /* K6   */  be_nested_str_weak(set),
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
    ( &(const binstruction[264]) {  /* code */
      0xB8120000,  //  0000  GETNGBL	R4	K0
      0x88100901,  //  0001  GETMBR	R4	R4	K1
      0x88140502,  //  0002  GETMBR	R5	R2	K2
      0x88180503,  //  0003  GETMBR	R6	R2	K3
      0x1C1C0B04,  //  0004  EQ	R7	R5	K4
      0x781E0021,  //  0005  JMPF	R7	#0028
      0x1C1C0D05,  //  0006  EQ	R7	R6	K5
      0x781E0005,  //  0007  JMPF	R7	#000E
      0x8C1C0706,  //  0008  GETMET	R7	R3	K6
      0x88240907,  //  0009  GETMBR	R9	R4	K7
      0x58280005,  //  000A  LDCONST	R10	K5
      0x7C1C0600,  //  000B  CALL	R7	3
      0x80040E00,  //  000C  RET	1	R7
      0x70020018,  //  000D  JMP		#0027
      0x1C1C0D08,  //  000E  EQ	R7	R6	K8
      0x781E0005,  //  000F  JMPF	R7	#0016
      0x8C1C0706,  //  0010  GETMET	R7	R3	K6
      0x88240909,  //  0011  GETMBR	R9	R4	K9
      0x58280005,  //  0012  LDCONST	R10	K5
      0x7C1C0600,  //  0013  CALL	R7	3
      0x80040E00,  //  0014  RET	1	R7
      0x70020010,  //  0015  JMP		#0027
      0x541EFFFB,  //  0016  LDINT	R7	65532
      0x1C1C0C07,  //  0017  EQ	R7	R6	R7
      0x781E0005,  //  0018  JMPF	R7	#001F
      0x8C1C0706,  //  0019  GETMET	R7	R3	K6
      0x8824090A,  //  001A  GETMBR	R9	R4	K10
      0x58280005,  //  001B  LDCONST	R10	K5
      0x7C1C0600,  //  001C  CALL	R7	3
      0x80040E00,  //  001D  RET	1	R7
      0x70020007,  //  001E  JMP		#0027
      0x541EFFFC,  //  001F  LDINT	R7	65533
      0x1C1C0C07,  //  0020  EQ	R7	R6	R7
      0x781E0004,  //  0021  JMPF	R7	#0027
      0x8C1C0706,  //  0022  GETMET	R7	R3	K6
      0x8824090A,  //  0023  GETMBR	R9	R4	K10
      0x542A0003,  //  0024  LDINT	R10	4
      0x7C1C0600,  //  0025  CALL	R7	3
      0x80040E00,  //  0026  RET	1	R7
      0x700200DE,  //  0027  JMP		#0107
      0x541E0003,  //  0028  LDINT	R7	4
      0x1C1C0A07,  //  0029  EQ	R7	R5	R7
      0x781E0016,  //  002A  JMPF	R7	#0042
      0x1C1C0D05,  //  002B  EQ	R7	R6	K5
      0x781E0002,  //  002C  JMPF	R7	#0030
      0x4C1C0000,  //  002D  LDNIL	R7
      0x80040E00,  //  002E  RET	1	R7
      0x70020010,  //  002F  JMP		#0041
      0x541EFFFB,  //  0030  LDINT	R7	65532
      0x1C1C0C07,  //  0031  EQ	R7	R6	R7
      0x781E0005,  //  0032  JMPF	R7	#0039
      0x8C1C0706,  //  0033  GETMET	R7	R3	K6
      0x8824090A,  //  0034  GETMBR	R9	R4	K10
      0x58280005,  //  0035  LDCONST	R10	K5
      0x7C1C0600,  //  0036  CALL	R7	3
      0x80040E00,  //  0037  RET	1	R7
      0x70020007,  //  0038  JMP		#0041
      0x541EFFFC,  //  0039  LDINT	R7	65533
      0x1C1C0C07,  //  003A  EQ	R7	R6	R7
      0x781E0004,  //  003B  JMPF	R7	#0041
      0x8C1C0706,  //  003C  GETMET	R7	R3	K6
      0x8824090A,  //  003D  GETMBR	R9	R4	K10
      0x542A0003,  //  003E  LDINT	R10	4
      0x7C1C0600,  //  003F  CALL	R7	3
      0x80040E00,  //  0040  RET	1	R7
      0x700200C4,  //  0041  JMP		#0107
      0x541E0004,  //  0042  LDINT	R7	5
      0x1C1C0A07,  //  0043  EQ	R7	R5	R7
      0x781E0011,  //  0044  JMPF	R7	#0057
      0x541EFFFB,  //  0045  LDINT	R7	65532
      0x1C1C0C07,  //  0046  EQ	R7	R6	R7
      0x781E0005,  //  0047  JMPF	R7	#004E
      0x8C1C0706,  //  0048  GETMET	R7	R3	K6
      0x8824090A,  //  0049  GETMBR	R9	R4	K10
      0x58280005,  //  004A  LDCONST	R10	K5
      0x7C1C0600,  //  004B  CALL	R7	3
      0x80040E00,  //  004C  RET	1	R7
      0x70020007,  //  004D  JMP		#0056
      0x541EFFFC,  //  004E  LDINT	R7	65533
      0x1C1C0C07,  //  004F  EQ	R7	R6	R7
      0x781E0004,  //  0050  JMPF	R7	#0056
      0x8C1C0706,  //  0051  GETMET	R7	R3	K6
      0x8824090A,  //  0052  GETMBR	R9	R4	K10
      0x542A0003,  //  0053  LDINT	R10	4
      0x7C1C0600,  //  0054  CALL	R7	3
      0x80040E00,  //  0055  RET	1	R7
      0x700200AF,  //  0056  JMP		#0107
      0x541E001C,  //  0057  LDINT	R7	29
      0x1C1C0A07,  //  0058  EQ	R7	R5	R7
      0x781E003E,  //  0059  JMPF	R7	#0099
      0x1C1C0D05,  //  005A  EQ	R7	R6	K5
      0x781E0032,  //  005B  JMPF	R7	#008F
      0x8C1C090B,  //  005C  GETMET	R7	R4	K11
      0x7C1C0200,  //  005D  CALL	R7	1
      0x8820010C,  //  005E  GETMBR	R8	R0	K12
      0x60240010,  //  005F  GETGBL	R9	G16
      0x8C28110D,  //  0060  GETMET	R10	R8	K13
      0x7C280200,  //  0061  CALL	R10	1
      0x7C240200,  //  0062  CALL	R9	1
      0xA802000E,  //  0063  EXBLK	0	#0073
      0x5C281200,  //  0064  MOVE	R10	R9
      0x7C280000,  //  0065  CALL	R10	0
      0x8C2C0F0E,  //  0066  GETMET	R11	R7	K14
      0x7C2C0200,  //  0067  CALL	R11	1
      0x8C30170F,  //  0068  GETMET	R12	R11	K15
      0x58380005,  //  0069  LDCONST	R14	K5
      0x883C0907,  //  006A  GETMBR	R15	R4	K7
      0x5C401400,  //  006B  MOVE	R16	R10
      0x7C300800,  //  006C  CALL	R12	4
      0x8C30170F,  //  006D  GETMET	R12	R11	K15
      0x58380008,  //  006E  LDCONST	R14	K8
      0x883C0907,  //  006F  GETMBR	R15	R4	K7
      0x9440100A,  //  0070  GETIDX	R16	R8	R10
      0x7C300800,  //  0071  CALL	R12	4
      0x7001FFF0,  //  0072  JMP		#0064
      0x58240010,  //  0073  LDCONST	R9	K16
      0xAC240200,  //  0074  CATCH	R9	1	0
      0xB0080000,  //  0075  RAISE	2	R0	R0
      0x88240111,  //  0076  GETMBR	R9	R0	K17
      0x8C241312,  //  0077  GETMET	R9	R9	K18
      0x8C2C0313,  //  0078  GETMET	R11	R1	K19
      0x7C2C0200,  //  0079  CALL	R11	1
      0x7C240400,  //  007A  CALL	R9	2
      0x4C280000,  //  007B  LDNIL	R10
      0x1C24120A,  //  007C  EQ	R9	R9	R10
      0x7826000E,  //  007D  JMPF	R9	#008D
      0x88240114,  //  007E  GETMBR	R9	R0	K20
      0x88241315,  //  007F  GETMBR	R9	R9	K21
      0x7426000B,  //  0080  JMPT	R9	#008D
      0x8C240F0E,  //  0081  GETMET	R9	R7	K14
      0x7C240200,  //  0082  CALL	R9	1
      0x8C28130F,  //  0083  GETMET	R10	R9	K15
      0x58300005,  //  0084  LDCONST	R12	K5
      0x88340907,  //  0085  GETMBR	R13	R4	K7
      0x543A0012,  //  0086  LDINT	R14	19
      0x7C280800,  //  0087  CALL	R10	4
      0x8C28130F,  //  0088  GETMET	R10	R9	K15
      0x58300008,  //  0089  LDCONST	R12	K8
      0x88340907,  //  008A  GETMBR	R13	R4	K7
      0x58380008,  //  008B  LDCONST	R14	K8
      0x7C280800,  //  008C  CALL	R10	4
      0x80040E00,  //  008D  RET	1	R7
      0x70020008,  //  008E  JMP		#0098
      0x601C0003,  //  008F  GETGBL	R7	G3
      0x5C200000,  //  0090  MOVE	R8	R0
      0x7C1C0200,  //  0091  CALL	R7	1
      0x8C1C0F16,  //  0092  GETMET	R7	R7	K22
      0x5C240200,  //  0093  MOVE	R9	R1
      0x5C280400,  //  0094  MOVE	R10	R2
      0x5C2C0600,  //  0095  MOVE	R11	R3
      0x7C1C0800,  //  0096  CALL	R7	4
      0x80040E00,  //  0097  RET	1	R7
      0x7002006D,  //  0098  JMP		#0107
      0x541E0038,  //  0099  LDINT	R7	57
      0x1C1C0A07,  //  009A  EQ	R7	R5	R7
      0x781E0061,  //  009B  JMPF	R7	#00FE
      0xA41E2E00,  //  009C  IMPORT	R7	K23
      0x1C200D04,  //  009D  EQ	R8	R6	K4
      0x7822000A,  //  009E  JMPF	R8	#00AA
      0x8C200706,  //  009F  GETMET	R8	R3	K6
      0x88280918,  //  00A0  GETMBR	R10	R4	K24
      0xB82E3200,  //  00A1  GETNGBL	R11	K25
      0x8C2C171A,  //  00A2  GETMET	R11	R11	K26
      0x5834001B,  //  00A3  LDCONST	R13	K27
      0x50380200,  //  00A4  LDBOOL	R14	1	0
      0x7C2C0600,  //  00A5  CALL	R11	3
      0x942C171B,  //  00A6  GETIDX	R11	R11	K27
      0x7C200600,  //  00A7  CALL	R8	3
      0x80041000,  //  00A8  RET	1	R8
      0x70020052,  //  00A9  JMP		#00FD
      0x54220004,  //  00AA  LDINT	R8	5
      0x1C200C08,  //  00AB  EQ	R8	R6	R8
      0x78220006,  //  00AC  JMPF	R8	#00B4
      0x8C200706,  //  00AD  GETMET	R8	R3	K6
      0x88280918,  //  00AE  GETMBR	R10	R4	K24
      0x8C2C011C,  //  00AF  GETMET	R11	R0	K28
      0x7C2C0200,  //  00B0  CALL	R11	1
      0x7C200600,  //  00B1  CALL	R8	3
      0x80041000,  //  00B2  RET	1	R8
      0x70020048,  //  00B3  JMP		#00FD
      0x54220009,  //  00B4  LDINT	R8	10
      0x1C200C08,  //  00B5  EQ	R8	R6	R8
      0x78220015,  //  00B6  JMPF	R8	#00CD
      0xB8223200,  //  00B7  GETNGBL	R8	K25
      0x8C20111A,  //  00B8  GETMET	R8	R8	K26
      0x5828001D,  //  00B9  LDCONST	R10	K29
      0x502C0200,  //  00BA  LDBOOL	R11	1	0
      0x7C200600,  //  00BB  CALL	R8	3
      0x9420111E,  //  00BC  GETIDX	R8	R8	K30
      0x9420111F,  //  00BD  GETIDX	R8	R8	K31
      0x8C240F12,  //  00BE  GETMET	R9	R7	K18
      0x5C2C1000,  //  00BF  MOVE	R11	R8
      0x58300020,  //  00C0  LDCONST	R12	K32
      0x7C240600,  //  00C1  CALL	R9	3
      0x24281305,  //  00C2  GT	R10	R9	K5
      0x782A0002,  //  00C3  JMPF	R10	#00C7
      0x04281308,  //  00C4  SUB	R10	R9	K8
      0x402A0A0A,  //  00C5  CONNECT	R10	K5	R10
      0x9420100A,  //  00C6  GETIDX	R8	R8	R10
      0x8C280706,  //  00C7  GETMET	R10	R3	K6
      0x88300918,  //  00C8  GETMBR	R12	R4	K24
      0x5C341000,  //  00C9  MOVE	R13	R8
      0x7C280600,  //  00CA  CALL	R10	3
      0x80041400,  //  00CB  RET	1	R10
      0x7002002F,  //  00CC  JMP		#00FD
      0x5422000E,  //  00CD  LDINT	R8	15
      0x1C200C08,  //  00CE  EQ	R8	R6	R8
      0x7822000B,  //  00CF  JMPF	R8	#00DC
      0x8C200706,  //  00D0  GETMET	R8	R3	K6
      0x88280918,  //  00D1  GETMBR	R10	R4	K24
      0xB82E3200,  //  00D2  GETNGBL	R11	K25
      0x8C2C1721,  //  00D3  GETMET	R11	R11	K33
      0x7C2C0200,  //  00D4  CALL	R11	1
      0x8C2C1712,  //  00D5  GETMET	R11	R11	K18
      0x58340022,  //  00D6  LDCONST	R13	K34
      0x58380023,  //  00D7  LDCONST	R14	K35
      0x7C2C0600,  //  00D8  CALL	R11	3
      0x7C200600,  //  00D9  CALL	R8	3
      0x80041000,  //  00DA  RET	1	R8
      0x70020020,  //  00DB  JMP		#00FD
      0x54220010,  //  00DC  LDINT	R8	17
      0x1C200C08,  //  00DD  EQ	R8	R6	R8
      0x78220005,  //  00DE  JMPF	R8	#00E5
      0x8C200706,  //  00DF  GETMET	R8	R3	K6
      0x88280924,  //  00E0  GETMBR	R10	R4	K36
      0x582C0008,  //  00E1  LDCONST	R11	K8
      0x7C200600,  //  00E2  CALL	R8	3
      0x80041000,  //  00E3  RET	1	R8
      0x70020017,  //  00E4  JMP		#00FD
      0x54220011,  //  00E5  LDINT	R8	18
      0x1C200C08,  //  00E6  EQ	R8	R6	R8
      0x7822000B,  //  00E7  JMPF	R8	#00F4
      0x8C200706,  //  00E8  GETMET	R8	R3	K6
      0x88280918,  //  00E9  GETMBR	R10	R4	K24
      0xB82E3200,  //  00EA  GETNGBL	R11	K25
      0x8C2C1721,  //  00EB  GETMET	R11	R11	K33
      0x7C2C0200,  //  00EC  CALL	R11	1
      0x8C2C1712,  //  00ED  GETMET	R11	R11	K18
      0x58340022,  //  00EE  LDCONST	R13	K34
      0x58380023,  //  00EF  LDCONST	R14	K35
      0x7C2C0600,  //  00F0  CALL	R11	3
      0x7C200600,  //  00F1  CALL	R8	3
      0x80041000,  //  00F2  RET	1	R8
      0x70020008,  //  00F3  JMP		#00FD
      0x60200003,  //  00F4  GETGBL	R8	G3
      0x5C240000,  //  00F5  MOVE	R9	R0
      0x7C200200,  //  00F6  CALL	R8	1
      0x8C201116,  //  00F7  GETMET	R8	R8	K22
      0x5C280200,  //  00F8  MOVE	R10	R1
      0x5C2C0400,  //  00F9  MOVE	R11	R2
      0x5C300600,  //  00FA  MOVE	R12	R3
      0x7C200800,  //  00FB  CALL	R8	4
      0x80041000,  //  00FC  RET	1	R8
      0x70020008,  //  00FD  JMP		#0107
      0x601C0003,  //  00FE  GETGBL	R7	G3
      0x5C200000,  //  00FF  MOVE	R8	R0
      0x7C1C0200,  //  0100  CALL	R7	1
      0x8C1C0F16,  //  0101  GETMET	R7	R7	K22
      0x5C240200,  //  0102  MOVE	R9	R1
      0x5C280400,  //  0103  MOVE	R10	R2
      0x5C2C0600,  //  0104  MOVE	R11	R3
      0x7C1C0800,  //  0105  CALL	R7	4
      0x80040E00,  //  0106  RET	1	R7
      0x80000000,  //  0107  RET	0
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
