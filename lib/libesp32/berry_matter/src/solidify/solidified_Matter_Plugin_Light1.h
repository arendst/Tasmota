/* Solidification of Matter_Plugin_Light1.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Light1;

/********************************************************************
** Solidified function: update_shadow
********************************************************************/
be_local_closure(Matter_Plugin_Light1_update_shadow,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[10]) {     /* constants */
    /* K0   */  be_nested_str_weak(light),
    /* K1   */  be_nested_str_weak(get),
    /* K2   */  be_nested_str_weak(find),
    /* K3   */  be_nested_str_weak(bri),
    /* K4   */  be_nested_str_weak(tasmota),
    /* K5   */  be_nested_str_weak(scale_uint),
    /* K6   */  be_const_int(0),
    /* K7   */  be_nested_str_weak(shadow_bri),
    /* K8   */  be_nested_str_weak(attribute_updated),
    /* K9   */  be_nested_str_weak(update_shadow),
    }),
    be_str_weak(update_shadow),
    &be_const_str_solidified,
    ( &(const binstruction[33]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080301,  //  0001  GETMET	R2	R1	K1
      0x7C080200,  //  0002  CALL	R2	1
      0x8C0C0502,  //  0003  GETMET	R3	R2	K2
      0x58140003,  //  0004  LDCONST	R5	K3
      0x4C180000,  //  0005  LDNIL	R6
      0x7C0C0600,  //  0006  CALL	R3	3
      0x4C100000,  //  0007  LDNIL	R4
      0x20100604,  //  0008  NE	R4	R3	R4
      0x78120010,  //  0009  JMPF	R4	#001B
      0xB8120800,  //  000A  GETNGBL	R4	K4
      0x8C100905,  //  000B  GETMET	R4	R4	K5
      0x5C180600,  //  000C  MOVE	R6	R3
      0x581C0006,  //  000D  LDCONST	R7	K6
      0x542200FE,  //  000E  LDINT	R8	255
      0x58240006,  //  000F  LDCONST	R9	K6
      0x542A00FD,  //  0010  LDINT	R10	254
      0x7C100C00,  //  0011  CALL	R4	6
      0x5C0C0800,  //  0012  MOVE	R3	R4
      0x88100107,  //  0013  GETMBR	R4	R0	K7
      0x20100604,  //  0014  NE	R4	R3	R4
      0x78120004,  //  0015  JMPF	R4	#001B
      0x8C100108,  //  0016  GETMET	R4	R0	K8
      0x541A0007,  //  0017  LDINT	R6	8
      0x581C0006,  //  0018  LDCONST	R7	K6
      0x7C100600,  //  0019  CALL	R4	3
      0x90020E03,  //  001A  SETMBR	R0	K7	R3
      0x60100003,  //  001B  GETGBL	R4	G3
      0x5C140000,  //  001C  MOVE	R5	R0
      0x7C100200,  //  001D  CALL	R4	1
      0x8C100909,  //  001E  GETMET	R4	R4	K9
      0x7C100200,  //  001F  CALL	R4	1
      0x80000000,  //  0020  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: invoke_request
********************************************************************/
be_local_closure(Matter_Plugin_Light1_invoke_request,   /* name */
  be_nested_proto(
    16,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[20]) {     /* constants */
    /* K0   */  be_nested_str_weak(light),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(TLV),
    /* K3   */  be_nested_str_weak(cluster),
    /* K4   */  be_nested_str_weak(command),
    /* K5   */  be_nested_str_weak(update_shadow_lazy),
    /* K6   */  be_const_int(0),
    /* K7   */  be_nested_str_weak(findsubval),
    /* K8   */  be_nested_str_weak(tasmota),
    /* K9   */  be_nested_str_weak(scale_uint),
    /* K10  */  be_nested_str_weak(set),
    /* K11  */  be_nested_str_weak(bri),
    /* K12  */  be_nested_str_weak(update_shadow),
    /* K13  */  be_nested_str_weak(log),
    /* K14  */  be_nested_str_weak(bri_X3A),
    /* K15  */  be_const_int(1),
    /* K16  */  be_const_int(2),
    /* K17  */  be_const_int(3),
    /* K18  */  be_nested_str_weak(power),
    /* K19  */  be_nested_str_weak(invoke_request),
    }),
    be_str_weak(invoke_request),
    &be_const_str_solidified,
    ( &(const binstruction[112]) {  /* code */
      0xA4120000,  //  0000  IMPORT	R4	K0
      0xB8160200,  //  0001  GETNGBL	R5	K1
      0x88140B02,  //  0002  GETMBR	R5	R5	K2
      0x88180703,  //  0003  GETMBR	R6	R3	K3
      0x881C0704,  //  0004  GETMBR	R7	R3	K4
      0x54220007,  //  0005  LDINT	R8	8
      0x1C200C08,  //  0006  EQ	R8	R6	R8
      0x7822005D,  //  0007  JMPF	R8	#0066
      0x8C200105,  //  0008  GETMET	R8	R0	K5
      0x7C200200,  //  0009  CALL	R8	1
      0x1C200F06,  //  000A  EQ	R8	R7	K6
      0x78220019,  //  000B  JMPF	R8	#0026
      0x8C200507,  //  000C  GETMET	R8	R2	K7
      0x58280006,  //  000D  LDCONST	R10	K6
      0x7C200400,  //  000E  CALL	R8	2
      0xB8261000,  //  000F  GETNGBL	R9	K8
      0x8C241309,  //  0010  GETMET	R9	R9	K9
      0x5C2C1000,  //  0011  MOVE	R11	R8
      0x58300006,  //  0012  LDCONST	R12	K6
      0x543600FD,  //  0013  LDINT	R13	254
      0x58380006,  //  0014  LDCONST	R14	K6
      0x543E00FE,  //  0015  LDINT	R15	255
      0x7C240C00,  //  0016  CALL	R9	6
      0x8C28090A,  //  0017  GETMET	R10	R4	K10
      0x60300013,  //  0018  GETGBL	R12	G19
      0x7C300000,  //  0019  CALL	R12	0
      0x98321609,  //  001A  SETIDX	R12	K11	R9
      0x7C280400,  //  001B  CALL	R10	2
      0x8C28010C,  //  001C  GETMET	R10	R0	K12
      0x7C280200,  //  001D  CALL	R10	1
      0x60280008,  //  001E  GETGBL	R10	G8
      0x5C2C1000,  //  001F  MOVE	R11	R8
      0x7C280200,  //  0020  CALL	R10	1
      0x002A1C0A,  //  0021  ADD	R10	K14	R10
      0x900E1A0A,  //  0022  SETMBR	R3	K13	R10
      0x50280200,  //  0023  LDBOOL	R10	1	0
      0x80041400,  //  0024  RET	1	R10
      0x7002003E,  //  0025  JMP		#0065
      0x1C200F0F,  //  0026  EQ	R8	R7	K15
      0x78220002,  //  0027  JMPF	R8	#002B
      0x50200200,  //  0028  LDBOOL	R8	1	0
      0x80041000,  //  0029  RET	1	R8
      0x70020039,  //  002A  JMP		#0065
      0x1C200F10,  //  002B  EQ	R8	R7	K16
      0x78220002,  //  002C  JMPF	R8	#0030
      0x50200200,  //  002D  LDBOOL	R8	1	0
      0x80041000,  //  002E  RET	1	R8
      0x70020034,  //  002F  JMP		#0065
      0x1C200F11,  //  0030  EQ	R8	R7	K17
      0x78220002,  //  0031  JMPF	R8	#0035
      0x50200200,  //  0032  LDBOOL	R8	1	0
      0x80041000,  //  0033  RET	1	R8
      0x7002002F,  //  0034  JMP		#0065
      0x54220003,  //  0035  LDINT	R8	4
      0x1C200E08,  //  0036  EQ	R8	R7	R8
      0x7822001B,  //  0037  JMPF	R8	#0054
      0x8C200507,  //  0038  GETMET	R8	R2	K7
      0x58280006,  //  0039  LDCONST	R10	K6
      0x7C200400,  //  003A  CALL	R8	2
      0xB8261000,  //  003B  GETNGBL	R9	K8
      0x8C241309,  //  003C  GETMET	R9	R9	K9
      0x5C2C1000,  //  003D  MOVE	R11	R8
      0x58300006,  //  003E  LDCONST	R12	K6
      0x543600FD,  //  003F  LDINT	R13	254
      0x58380006,  //  0040  LDCONST	R14	K6
      0x543E00FE,  //  0041  LDINT	R15	255
      0x7C240C00,  //  0042  CALL	R9	6
      0x24281306,  //  0043  GT	R10	R9	K6
      0x8C2C090A,  //  0044  GETMET	R11	R4	K10
      0x60340013,  //  0045  GETGBL	R13	G19
      0x7C340000,  //  0046  CALL	R13	0
      0x98361609,  //  0047  SETIDX	R13	K11	R9
      0x9836240A,  //  0048  SETIDX	R13	K18	R10
      0x7C2C0400,  //  0049  CALL	R11	2
      0x8C2C010C,  //  004A  GETMET	R11	R0	K12
      0x7C2C0200,  //  004B  CALL	R11	1
      0x602C0008,  //  004C  GETGBL	R11	G8
      0x5C301000,  //  004D  MOVE	R12	R8
      0x7C2C0200,  //  004E  CALL	R11	1
      0x002E1C0B,  //  004F  ADD	R11	K14	R11
      0x900E1A0B,  //  0050  SETMBR	R3	K13	R11
      0x502C0200,  //  0051  LDBOOL	R11	1	0
      0x80041600,  //  0052  RET	1	R11
      0x70020010,  //  0053  JMP		#0065
      0x54220004,  //  0054  LDINT	R8	5
      0x1C200E08,  //  0055  EQ	R8	R7	R8
      0x78220002,  //  0056  JMPF	R8	#005A
      0x50200200,  //  0057  LDBOOL	R8	1	0
      0x80041000,  //  0058  RET	1	R8
      0x7002000A,  //  0059  JMP		#0065
      0x54220005,  //  005A  LDINT	R8	6
      0x1C200E08,  //  005B  EQ	R8	R7	R8
      0x78220002,  //  005C  JMPF	R8	#0060
      0x50200200,  //  005D  LDBOOL	R8	1	0
      0x80041000,  //  005E  RET	1	R8
      0x70020004,  //  005F  JMP		#0065
      0x54220006,  //  0060  LDINT	R8	7
      0x1C200E08,  //  0061  EQ	R8	R7	R8
      0x78220001,  //  0062  JMPF	R8	#0065
      0x50200200,  //  0063  LDBOOL	R8	1	0
      0x80041000,  //  0064  RET	1	R8
      0x70020008,  //  0065  JMP		#006F
      0x60200003,  //  0066  GETGBL	R8	G3
      0x5C240000,  //  0067  MOVE	R9	R0
      0x7C200200,  //  0068  CALL	R8	1
      0x8C201113,  //  0069  GETMET	R8	R8	K19
      0x5C280200,  //  006A  MOVE	R10	R1
      0x5C2C0400,  //  006B  MOVE	R11	R2
      0x5C300600,  //  006C  MOVE	R12	R3
      0x7C200800,  //  006D  CALL	R8	4
      0x80041000,  //  006E  RET	1	R8
      0x80000000,  //  006F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: read_attribute
********************************************************************/
be_local_closure(Matter_Plugin_Light1_read_attribute,   /* name */
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
    /* K5   */  be_nested_str_weak(update_shadow_lazy),
    /* K6   */  be_const_int(0),
    /* K7   */  be_nested_str_weak(create_TLV),
    /* K8   */  be_nested_str_weak(U1),
    /* K9   */  be_nested_str_weak(shadow_bri),
    /* K10  */  be_const_int(2),
    /* K11  */  be_const_int(3),
    /* K12  */  be_nested_str_weak(U4),
    /* K13  */  be_const_int(1),
    /* K14  */  be_nested_str_weak(read_attribute),
    }),
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[79]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xB8120200,  //  0001  GETNGBL	R4	K1
      0x88100902,  //  0002  GETMBR	R4	R4	K2
      0x88140503,  //  0003  GETMBR	R5	R2	K3
      0x88180504,  //  0004  GETMBR	R6	R2	K4
      0x541E0007,  //  0005  LDINT	R7	8
      0x1C1C0A07,  //  0006  EQ	R7	R5	R7
      0x781E003D,  //  0007  JMPF	R7	#0046
      0x8C1C0105,  //  0008  GETMET	R7	R0	K5
      0x7C1C0200,  //  0009  CALL	R7	1
      0x1C1C0D06,  //  000A  EQ	R7	R6	K6
      0x781E0005,  //  000B  JMPF	R7	#0012
      0x8C1C0907,  //  000C  GETMET	R7	R4	K7
      0x88240908,  //  000D  GETMBR	R9	R4	K8
      0x88280109,  //  000E  GETMBR	R10	R0	K9
      0x7C1C0600,  //  000F  CALL	R7	3
      0x80040E00,  //  0010  RET	1	R7
      0x70020032,  //  0011  JMP		#0045
      0x1C1C0D0A,  //  0012  EQ	R7	R6	K10
      0x781E0005,  //  0013  JMPF	R7	#001A
      0x8C1C0907,  //  0014  GETMET	R7	R4	K7
      0x88240908,  //  0015  GETMBR	R9	R4	K8
      0x58280006,  //  0016  LDCONST	R10	K6
      0x7C1C0600,  //  0017  CALL	R7	3
      0x80040E00,  //  0018  RET	1	R7
      0x7002002A,  //  0019  JMP		#0045
      0x1C1C0D0B,  //  001A  EQ	R7	R6	K11
      0x781E0005,  //  001B  JMPF	R7	#0022
      0x8C1C0907,  //  001C  GETMET	R7	R4	K7
      0x88240908,  //  001D  GETMBR	R9	R4	K8
      0x542A00FD,  //  001E  LDINT	R10	254
      0x7C1C0600,  //  001F  CALL	R7	3
      0x80040E00,  //  0020  RET	1	R7
      0x70020022,  //  0021  JMP		#0045
      0x541E000E,  //  0022  LDINT	R7	15
      0x1C1C0C07,  //  0023  EQ	R7	R6	R7
      0x781E0005,  //  0024  JMPF	R7	#002B
      0x8C1C0907,  //  0025  GETMET	R7	R4	K7
      0x88240908,  //  0026  GETMBR	R9	R4	K8
      0x58280006,  //  0027  LDCONST	R10	K6
      0x7C1C0600,  //  0028  CALL	R7	3
      0x80040E00,  //  0029  RET	1	R7
      0x70020019,  //  002A  JMP		#0045
      0x541E0010,  //  002B  LDINT	R7	17
      0x1C1C0C07,  //  002C  EQ	R7	R6	R7
      0x781E0005,  //  002D  JMPF	R7	#0034
      0x8C1C0907,  //  002E  GETMET	R7	R4	K7
      0x88240908,  //  002F  GETMBR	R9	R4	K8
      0x88280109,  //  0030  GETMBR	R10	R0	K9
      0x7C1C0600,  //  0031  CALL	R7	3
      0x80040E00,  //  0032  RET	1	R7
      0x70020010,  //  0033  JMP		#0045
      0x541EFFFB,  //  0034  LDINT	R7	65532
      0x1C1C0C07,  //  0035  EQ	R7	R6	R7
      0x781E0005,  //  0036  JMPF	R7	#003D
      0x8C1C0907,  //  0037  GETMET	R7	R4	K7
      0x8824090C,  //  0038  GETMBR	R9	R4	K12
      0x5828000D,  //  0039  LDCONST	R10	K13
      0x7C1C0600,  //  003A  CALL	R7	3
      0x80040E00,  //  003B  RET	1	R7
      0x70020007,  //  003C  JMP		#0045
      0x541EFFFC,  //  003D  LDINT	R7	65533
      0x1C1C0C07,  //  003E  EQ	R7	R6	R7
      0x781E0004,  //  003F  JMPF	R7	#0045
      0x8C1C0907,  //  0040  GETMET	R7	R4	K7
      0x8824090C,  //  0041  GETMBR	R9	R4	K12
      0x542A0004,  //  0042  LDINT	R10	5
      0x7C1C0600,  //  0043  CALL	R7	3
      0x80040E00,  //  0044  RET	1	R7
      0x70020007,  //  0045  JMP		#004E
      0x601C0003,  //  0046  GETGBL	R7	G3
      0x5C200000,  //  0047  MOVE	R8	R0
      0x7C1C0200,  //  0048  CALL	R7	1
      0x8C1C0F0E,  //  0049  GETMET	R7	R7	K14
      0x5C240200,  //  004A  MOVE	R9	R1
      0x5C280400,  //  004B  MOVE	R10	R2
      0x7C1C0600,  //  004C  CALL	R7	3
      0x80040E00,  //  004D  RET	1	R7
      0x80000000,  //  004E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_Plugin_Light1_init,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(init),
    /* K1   */  be_nested_str_weak(shadow_bri),
    /* K2   */  be_const_int(0),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x60100003,  //  0000  GETGBL	R4	G3
      0x5C140000,  //  0001  MOVE	R5	R0
      0x7C100200,  //  0002  CALL	R4	1
      0x8C100900,  //  0003  GETMET	R4	R4	K0
      0x5C180200,  //  0004  MOVE	R6	R1
      0x5C1C0400,  //  0005  MOVE	R7	R2
      0x5C200600,  //  0006  MOVE	R8	R3
      0x7C100800,  //  0007  CALL	R4	4
      0x90020302,  //  0008  SETMBR	R0	K1	K2
      0x80000000,  //  0009  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Plugin_Light1
********************************************************************/
extern const bclass be_class_Matter_Plugin_Light0;
be_local_class(Matter_Plugin_Light1,
    1,
    &be_class_Matter_Plugin_Light0,
    be_nested_map(9,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(init, -1), be_const_closure(Matter_Plugin_Light1_init_closure) },
        { be_const_key_weak(TYPE, -1), be_nested_str_weak(light1) },
        { be_const_key_weak(TYPES, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(257, -1), be_const_int(2) },
    }))    ) } )) },
        { be_const_key_weak(NAME, -1), be_nested_str_weak(Light_X201_X20Dimmer) },
        { be_const_key_weak(CLUSTERS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(8, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(7,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(2),
        be_const_int(3),
        be_const_int(15),
        be_const_int(17),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
    }))    ) } )) },
        { be_const_key_weak(invoke_request, -1), be_const_closure(Matter_Plugin_Light1_invoke_request_closure) },
        { be_const_key_weak(read_attribute, 0), be_const_closure(Matter_Plugin_Light1_read_attribute_closure) },
        { be_const_key_weak(update_shadow, 8), be_const_closure(Matter_Plugin_Light1_update_shadow_closure) },
        { be_const_key_weak(shadow_bri, -1), be_const_var(0) },
    })),
    be_str_weak(Matter_Plugin_Light1)
);
/*******************************************************************/

void be_load_Matter_Plugin_Light1_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Plugin_Light1);
    be_setglobal(vm, "Matter_Plugin_Light1");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
