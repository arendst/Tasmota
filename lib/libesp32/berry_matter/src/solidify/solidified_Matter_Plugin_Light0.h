/* Solidification of Matter_Plugin_Light0.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Light0;

/********************************************************************
** Solidified function: read_attribute
********************************************************************/
be_local_closure(Matter_Plugin_Light0_read_attribute,   /* name */
  be_nested_proto(
    11,                          /* nstack */
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
    /* K13  */  be_nested_str_weak(shadow_onoff),
    /* K14  */  be_nested_str_weak(read_attribute),
    }),
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[126]) {  /* code */
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
      0x70020053,  //  0028  JMP		#007D
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
      0x70020039,  //  0042  JMP		#007D
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
      0x70020024,  //  0057  JMP		#007D
      0x541E0005,  //  0058  LDINT	R7	6
      0x1C1C0A07,  //  0059  EQ	R7	R5	R7
      0x781E0019,  //  005A  JMPF	R7	#0075
      0x1C1C0D06,  //  005B  EQ	R7	R6	K6
      0x781E0005,  //  005C  JMPF	R7	#0063
      0x8C1C0907,  //  005D  GETMET	R7	R4	K7
      0x8824090C,  //  005E  GETMBR	R9	R4	K12
      0x8828010D,  //  005F  GETMBR	R10	R0	K13
      0x7C1C0600,  //  0060  CALL	R7	3
      0x80040E00,  //  0061  RET	1	R7
      0x70020010,  //  0062  JMP		#0074
      0x541EFFFB,  //  0063  LDINT	R7	65532
      0x1C1C0C07,  //  0064  EQ	R7	R6	R7
      0x781E0005,  //  0065  JMPF	R7	#006C
      0x8C1C0907,  //  0066  GETMET	R7	R4	K7
      0x8824090B,  //  0067  GETMBR	R9	R4	K11
      0x58280006,  //  0068  LDCONST	R10	K6
      0x7C1C0600,  //  0069  CALL	R7	3
      0x80040E00,  //  006A  RET	1	R7
      0x70020007,  //  006B  JMP		#0074
      0x541EFFFC,  //  006C  LDINT	R7	65533
      0x1C1C0C07,  //  006D  EQ	R7	R6	R7
      0x781E0004,  //  006E  JMPF	R7	#0074
      0x8C1C0907,  //  006F  GETMET	R7	R4	K7
      0x8824090B,  //  0070  GETMBR	R9	R4	K11
      0x542A0003,  //  0071  LDINT	R10	4
      0x7C1C0600,  //  0072  CALL	R7	3
      0x80040E00,  //  0073  RET	1	R7
      0x70020007,  //  0074  JMP		#007D
      0x601C0003,  //  0075  GETGBL	R7	G3
      0x5C200000,  //  0076  MOVE	R8	R0
      0x7C1C0200,  //  0077  CALL	R7	1
      0x8C1C0F0E,  //  0078  GETMET	R7	R7	K14
      0x5C240200,  //  0079  MOVE	R9	R1
      0x5C280400,  //  007A  MOVE	R10	R2
      0x7C1C0600,  //  007B  CALL	R7	3
      0x80040E00,  //  007C  RET	1	R7
      0x80000000,  //  007D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update_shadow
********************************************************************/
be_local_closure(Matter_Plugin_Light0_update_shadow,   /* name */
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
    /* K0   */  be_nested_str_weak(light),
    /* K1   */  be_nested_str_weak(get),
    /* K2   */  be_nested_str_weak(find),
    /* K3   */  be_nested_str_weak(power),
    /* K4   */  be_nested_str_weak(shadow_onoff),
    /* K5   */  be_nested_str_weak(attribute_updated),
    /* K6   */  be_const_int(0),
    }),
    be_str_weak(update_shadow),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080301,  //  0001  GETMET	R2	R1	K1
      0x7C080200,  //  0002  CALL	R2	1
      0x8C0C0502,  //  0003  GETMET	R3	R2	K2
      0x58140003,  //  0004  LDCONST	R5	K3
      0x4C180000,  //  0005  LDNIL	R6
      0x7C0C0600,  //  0006  CALL	R3	3
      0x88100104,  //  0007  GETMBR	R4	R0	K4
      0x20100604,  //  0008  NE	R4	R3	R4
      0x78120005,  //  0009  JMPF	R4	#0010
      0x8C100105,  //  000A  GETMET	R4	R0	K5
      0x4C180000,  //  000B  LDNIL	R6
      0x541E0005,  //  000C  LDINT	R7	6
      0x58200006,  //  000D  LDCONST	R8	K6
      0x7C100800,  //  000E  CALL	R4	4
      0x90020803,  //  000F  SETMBR	R0	K4	R3
      0x80000000,  //  0010  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: invoke_request
********************************************************************/
be_local_closure(Matter_Plugin_Light0_invoke_request,   /* name */
  be_nested_proto(
    14,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[16]) {     /* constants */
    /* K0   */  be_nested_str_weak(light),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(TLV),
    /* K3   */  be_nested_str_weak(cluster),
    /* K4   */  be_nested_str_weak(command),
    /* K5   */  be_const_int(3),
    /* K6   */  be_const_int(0),
    /* K7   */  be_const_int(1),
    /* K8   */  be_nested_str_weak(Matter_TLV_struct),
    /* K9   */  be_nested_str_weak(add_TLV),
    /* K10  */  be_nested_str_weak(U2),
    /* K11  */  be_nested_str_weak(set),
    /* K12  */  be_nested_str_weak(power),
    /* K13  */  be_nested_str_weak(update_shadow),
    /* K14  */  be_const_int(2),
    /* K15  */  be_nested_str_weak(shadow_onoff),
    }),
    be_str_weak(invoke_request),
    &be_const_str_solidified,
    ( &(const binstruction[87]) {  /* code */
      0xA4120000,  //  0000  IMPORT	R4	K0
      0xB8160200,  //  0001  GETNGBL	R5	K1
      0x88140B02,  //  0002  GETMBR	R5	R5	K2
      0x88180703,  //  0003  GETMBR	R6	R3	K3
      0x881C0704,  //  0004  GETMBR	R7	R3	K4
      0x1C200D05,  //  0005  EQ	R8	R6	K5
      0x78220016,  //  0006  JMPF	R8	#001E
      0x1C200F06,  //  0007  EQ	R8	R7	K6
      0x78220002,  //  0008  JMPF	R8	#000C
      0x50200200,  //  0009  LDBOOL	R8	1	0
      0x80041000,  //  000A  RET	1	R8
      0x70020010,  //  000B  JMP		#001D
      0x1C200F07,  //  000C  EQ	R8	R7	K7
      0x78220009,  //  000D  JMPF	R8	#0018
      0x8C200B08,  //  000E  GETMET	R8	R5	K8
      0x7C200200,  //  000F  CALL	R8	1
      0x8C241109,  //  0010  GETMET	R9	R8	K9
      0x582C0006,  //  0011  LDCONST	R11	K6
      0x88300B0A,  //  0012  GETMBR	R12	R5	K10
      0x58340006,  //  0013  LDCONST	R13	K6
      0x7C240800,  //  0014  CALL	R9	4
      0x900E0906,  //  0015  SETMBR	R3	K4	K6
      0x80041000,  //  0016  RET	1	R8
      0x70020004,  //  0017  JMP		#001D
      0x5422003F,  //  0018  LDINT	R8	64
      0x1C200E08,  //  0019  EQ	R8	R7	R8
      0x78220001,  //  001A  JMPF	R8	#001D
      0x50200200,  //  001B  LDBOOL	R8	1	0
      0x80041000,  //  001C  RET	1	R8
      0x70020037,  //  001D  JMP		#0056
      0x54220003,  //  001E  LDINT	R8	4
      0x1C200C08,  //  001F  EQ	R8	R6	R8
      0x78220002,  //  0020  JMPF	R8	#0024
      0x50200200,  //  0021  LDBOOL	R8	1	0
      0x80041000,  //  0022  RET	1	R8
      0x70020031,  //  0023  JMP		#0056
      0x54220004,  //  0024  LDINT	R8	5
      0x1C200C08,  //  0025  EQ	R8	R6	R8
      0x78220002,  //  0026  JMPF	R8	#002A
      0x50200200,  //  0027  LDBOOL	R8	1	0
      0x80041000,  //  0028  RET	1	R8
      0x7002002B,  //  0029  JMP		#0056
      0x54220005,  //  002A  LDINT	R8	6
      0x1C200C08,  //  002B  EQ	R8	R6	R8
      0x78220028,  //  002C  JMPF	R8	#0056
      0x1C200F06,  //  002D  EQ	R8	R7	K6
      0x7822000A,  //  002E  JMPF	R8	#003A
      0x8C20090B,  //  002F  GETMET	R8	R4	K11
      0x60280013,  //  0030  GETGBL	R10	G19
      0x7C280000,  //  0031  CALL	R10	0
      0x502C0000,  //  0032  LDBOOL	R11	0	0
      0x982A180B,  //  0033  SETIDX	R10	K12	R11
      0x7C200400,  //  0034  CALL	R8	2
      0x8C20010D,  //  0035  GETMET	R8	R0	K13
      0x7C200200,  //  0036  CALL	R8	1
      0x50200200,  //  0037  LDBOOL	R8	1	0
      0x80041000,  //  0038  RET	1	R8
      0x7002001B,  //  0039  JMP		#0056
      0x1C200F07,  //  003A  EQ	R8	R7	K7
      0x7822000A,  //  003B  JMPF	R8	#0047
      0x8C20090B,  //  003C  GETMET	R8	R4	K11
      0x60280013,  //  003D  GETGBL	R10	G19
      0x7C280000,  //  003E  CALL	R10	0
      0x502C0200,  //  003F  LDBOOL	R11	1	0
      0x982A180B,  //  0040  SETIDX	R10	K12	R11
      0x7C200400,  //  0041  CALL	R8	2
      0x8C20010D,  //  0042  GETMET	R8	R0	K13
      0x7C200200,  //  0043  CALL	R8	1
      0x50200200,  //  0044  LDBOOL	R8	1	0
      0x80041000,  //  0045  RET	1	R8
      0x7002000E,  //  0046  JMP		#0056
      0x1C200F0E,  //  0047  EQ	R8	R7	K14
      0x7822000C,  //  0048  JMPF	R8	#0056
      0x8C20090B,  //  0049  GETMET	R8	R4	K11
      0x60280013,  //  004A  GETGBL	R10	G19
      0x7C280000,  //  004B  CALL	R10	0
      0x882C010F,  //  004C  GETMBR	R11	R0	K15
      0x782E0000,  //  004D  JMPF	R11	#004F
      0x502C0001,  //  004E  LDBOOL	R11	0	1
      0x502C0200,  //  004F  LDBOOL	R11	1	0
      0x982A180B,  //  0050  SETIDX	R10	K12	R11
      0x7C200400,  //  0051  CALL	R8	2
      0x8C20010D,  //  0052  GETMET	R8	R0	K13
      0x7C200200,  //  0053  CALL	R8	1
      0x50200200,  //  0054  LDBOOL	R8	1	0
      0x80041000,  //  0055  RET	1	R8
      0x80000000,  //  0056  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_Plugin_Light0_init,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(init),
    /* K1   */  be_nested_str_weak(shadow_onoff),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x600C0003,  //  0000  GETGBL	R3	G3
      0x5C100000,  //  0001  MOVE	R4	R0
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C0700,  //  0003  GETMET	R3	R3	K0
      0x5C140200,  //  0004  MOVE	R5	R1
      0x5C180400,  //  0005  MOVE	R6	R2
      0x7C0C0600,  //  0006  CALL	R3	3
      0x500C0000,  //  0007  LDBOOL	R3	0	0
      0x90020203,  //  0008  SETMBR	R0	K1	R3
      0x80000000,  //  0009  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Plugin_Light0
********************************************************************/
extern const bclass be_class_Matter_Plugin;
be_local_class(Matter_Plugin_Light0,
    1,
    &be_class_Matter_Plugin,
    be_nested_map(7,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(read_attribute, 4), be_const_closure(Matter_Plugin_Light0_read_attribute_closure) },
        { be_const_key_weak(update_shadow, -1), be_const_closure(Matter_Plugin_Light0_update_shadow_closure) },
        { be_const_key_weak(CLUSTERS, 1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
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
        { be_const_key_weak(TYPES, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(256, -1), be_const_int(2) },
    }))    ) } )) },
        { be_const_key_weak(invoke_request, 2), be_const_closure(Matter_Plugin_Light0_invoke_request_closure) },
        { be_const_key_weak(init, -1), be_const_closure(Matter_Plugin_Light0_init_closure) },
        { be_const_key_weak(shadow_onoff, -1), be_const_var(0) },
    })),
    be_str_weak(Matter_Plugin_Light0)
);
/*******************************************************************/

void be_load_Matter_Plugin_Light0_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Plugin_Light0);
    be_setglobal(vm, "Matter_Plugin_Light0");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
