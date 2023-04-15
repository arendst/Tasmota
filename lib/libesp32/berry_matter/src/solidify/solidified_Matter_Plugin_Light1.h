/* Solidification of Matter_Plugin_Light1.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Light1;

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
    ( &(const bvalue[19]) {     /* constants */
    /* K0   */  be_nested_str_weak(light),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(TLV),
    /* K3   */  be_nested_str_weak(cluster),
    /* K4   */  be_nested_str_weak(command),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(findsubval),
    /* K7   */  be_nested_str_weak(tasmota),
    /* K8   */  be_nested_str_weak(scale_uint),
    /* K9   */  be_nested_str_weak(set),
    /* K10  */  be_nested_str_weak(bri),
    /* K11  */  be_nested_str_weak(update_shadow),
    /* K12  */  be_nested_str_weak(log),
    /* K13  */  be_nested_str_weak(bri_X3A),
    /* K14  */  be_const_int(1),
    /* K15  */  be_const_int(2),
    /* K16  */  be_const_int(3),
    /* K17  */  be_nested_str_weak(power),
    /* K18  */  be_nested_str_weak(invoke_request),
    }),
    be_str_weak(invoke_request),
    &be_const_str_solidified,
    ( &(const binstruction[110]) {  /* code */
      0xA4120000,  //  0000  IMPORT	R4	K0
      0xB8160200,  //  0001  GETNGBL	R5	K1
      0x88140B02,  //  0002  GETMBR	R5	R5	K2
      0x88180703,  //  0003  GETMBR	R6	R3	K3
      0x881C0704,  //  0004  GETMBR	R7	R3	K4
      0x54220007,  //  0005  LDINT	R8	8
      0x1C200C08,  //  0006  EQ	R8	R6	R8
      0x7822005B,  //  0007  JMPF	R8	#0064
      0x1C200F05,  //  0008  EQ	R8	R7	K5
      0x78220019,  //  0009  JMPF	R8	#0024
      0x8C200506,  //  000A  GETMET	R8	R2	K6
      0x58280005,  //  000B  LDCONST	R10	K5
      0x7C200400,  //  000C  CALL	R8	2
      0xB8260E00,  //  000D  GETNGBL	R9	K7
      0x8C241308,  //  000E  GETMET	R9	R9	K8
      0x5C2C1000,  //  000F  MOVE	R11	R8
      0x58300005,  //  0010  LDCONST	R12	K5
      0x543600FD,  //  0011  LDINT	R13	254
      0x58380005,  //  0012  LDCONST	R14	K5
      0x543E00FE,  //  0013  LDINT	R15	255
      0x7C240C00,  //  0014  CALL	R9	6
      0x8C280909,  //  0015  GETMET	R10	R4	K9
      0x60300013,  //  0016  GETGBL	R12	G19
      0x7C300000,  //  0017  CALL	R12	0
      0x98321409,  //  0018  SETIDX	R12	K10	R9
      0x7C280400,  //  0019  CALL	R10	2
      0x8C28010B,  //  001A  GETMET	R10	R0	K11
      0x7C280200,  //  001B  CALL	R10	1
      0x60280008,  //  001C  GETGBL	R10	G8
      0x5C2C1000,  //  001D  MOVE	R11	R8
      0x7C280200,  //  001E  CALL	R10	1
      0x002A1A0A,  //  001F  ADD	R10	K13	R10
      0x900E180A,  //  0020  SETMBR	R3	K12	R10
      0x50280200,  //  0021  LDBOOL	R10	1	0
      0x80041400,  //  0022  RET	1	R10
      0x7002003E,  //  0023  JMP		#0063
      0x1C200F0E,  //  0024  EQ	R8	R7	K14
      0x78220002,  //  0025  JMPF	R8	#0029
      0x50200200,  //  0026  LDBOOL	R8	1	0
      0x80041000,  //  0027  RET	1	R8
      0x70020039,  //  0028  JMP		#0063
      0x1C200F0F,  //  0029  EQ	R8	R7	K15
      0x78220002,  //  002A  JMPF	R8	#002E
      0x50200200,  //  002B  LDBOOL	R8	1	0
      0x80041000,  //  002C  RET	1	R8
      0x70020034,  //  002D  JMP		#0063
      0x1C200F10,  //  002E  EQ	R8	R7	K16
      0x78220002,  //  002F  JMPF	R8	#0033
      0x50200200,  //  0030  LDBOOL	R8	1	0
      0x80041000,  //  0031  RET	1	R8
      0x7002002F,  //  0032  JMP		#0063
      0x54220003,  //  0033  LDINT	R8	4
      0x1C200E08,  //  0034  EQ	R8	R7	R8
      0x7822001B,  //  0035  JMPF	R8	#0052
      0x8C200506,  //  0036  GETMET	R8	R2	K6
      0x58280005,  //  0037  LDCONST	R10	K5
      0x7C200400,  //  0038  CALL	R8	2
      0xB8260E00,  //  0039  GETNGBL	R9	K7
      0x8C241308,  //  003A  GETMET	R9	R9	K8
      0x5C2C1000,  //  003B  MOVE	R11	R8
      0x58300005,  //  003C  LDCONST	R12	K5
      0x543600FD,  //  003D  LDINT	R13	254
      0x58380005,  //  003E  LDCONST	R14	K5
      0x543E00FE,  //  003F  LDINT	R15	255
      0x7C240C00,  //  0040  CALL	R9	6
      0x24281305,  //  0041  GT	R10	R9	K5
      0x8C2C0909,  //  0042  GETMET	R11	R4	K9
      0x60340013,  //  0043  GETGBL	R13	G19
      0x7C340000,  //  0044  CALL	R13	0
      0x98361409,  //  0045  SETIDX	R13	K10	R9
      0x9836220A,  //  0046  SETIDX	R13	K17	R10
      0x7C2C0400,  //  0047  CALL	R11	2
      0x8C2C010B,  //  0048  GETMET	R11	R0	K11
      0x7C2C0200,  //  0049  CALL	R11	1
      0x602C0008,  //  004A  GETGBL	R11	G8
      0x5C301000,  //  004B  MOVE	R12	R8
      0x7C2C0200,  //  004C  CALL	R11	1
      0x002E1A0B,  //  004D  ADD	R11	K13	R11
      0x900E180B,  //  004E  SETMBR	R3	K12	R11
      0x502C0200,  //  004F  LDBOOL	R11	1	0
      0x80041600,  //  0050  RET	1	R11
      0x70020010,  //  0051  JMP		#0063
      0x54220004,  //  0052  LDINT	R8	5
      0x1C200E08,  //  0053  EQ	R8	R7	R8
      0x78220002,  //  0054  JMPF	R8	#0058
      0x50200200,  //  0055  LDBOOL	R8	1	0
      0x80041000,  //  0056  RET	1	R8
      0x7002000A,  //  0057  JMP		#0063
      0x54220005,  //  0058  LDINT	R8	6
      0x1C200E08,  //  0059  EQ	R8	R7	R8
      0x78220002,  //  005A  JMPF	R8	#005E
      0x50200200,  //  005B  LDBOOL	R8	1	0
      0x80041000,  //  005C  RET	1	R8
      0x70020004,  //  005D  JMP		#0063
      0x54220006,  //  005E  LDINT	R8	7
      0x1C200E08,  //  005F  EQ	R8	R7	R8
      0x78220001,  //  0060  JMPF	R8	#0063
      0x50200200,  //  0061  LDBOOL	R8	1	0
      0x80041000,  //  0062  RET	1	R8
      0x70020008,  //  0063  JMP		#006D
      0x60200003,  //  0064  GETGBL	R8	G3
      0x5C240000,  //  0065  MOVE	R9	R0
      0x7C200200,  //  0066  CALL	R8	1
      0x8C201112,  //  0067  GETMET	R8	R8	K18
      0x5C280200,  //  0068  MOVE	R10	R1
      0x5C2C0400,  //  0069  MOVE	R11	R2
      0x5C300600,  //  006A  MOVE	R12	R3
      0x7C200800,  //  006B  CALL	R8	4
      0x80041000,  //  006C  RET	1	R8
      0x80000000,  //  006D  RET	0
    })
  )
);
/*******************************************************************/


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
    ( &(const binstruction[36]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080301,  //  0001  GETMET	R2	R1	K1
      0x7C080200,  //  0002  CALL	R2	1
      0x8C0C0502,  //  0003  GETMET	R3	R2	K2
      0x58140003,  //  0004  LDCONST	R5	K3
      0x4C180000,  //  0005  LDNIL	R6
      0x7C0C0600,  //  0006  CALL	R3	3
      0x4C100000,  //  0007  LDNIL	R4
      0x20100604,  //  0008  NE	R4	R3	R4
      0x78120009,  //  0009  JMPF	R4	#0014
      0xB8120800,  //  000A  GETNGBL	R4	K4
      0x8C100905,  //  000B  GETMET	R4	R4	K5
      0x5C180600,  //  000C  MOVE	R6	R3
      0x581C0006,  //  000D  LDCONST	R7	K6
      0x542200FE,  //  000E  LDINT	R8	255
      0x58240006,  //  000F  LDCONST	R9	K6
      0x542A00FD,  //  0010  LDINT	R10	254
      0x7C100C00,  //  0011  CALL	R4	6
      0x5C0C0800,  //  0012  MOVE	R3	R4
      0x70020000,  //  0013  JMP		#0015
      0x880C0107,  //  0014  GETMBR	R3	R0	K7
      0x88100107,  //  0015  GETMBR	R4	R0	K7
      0x20100604,  //  0016  NE	R4	R3	R4
      0x78120005,  //  0017  JMPF	R4	#001E
      0x8C100108,  //  0018  GETMET	R4	R0	K8
      0x4C180000,  //  0019  LDNIL	R6
      0x541E0007,  //  001A  LDINT	R7	8
      0x58200006,  //  001B  LDCONST	R8	K6
      0x7C100800,  //  001C  CALL	R4	4
      0x90020E03,  //  001D  SETMBR	R0	K7	R3
      0x60100003,  //  001E  GETGBL	R4	G3
      0x5C140000,  //  001F  MOVE	R5	R0
      0x7C100200,  //  0020  CALL	R4	1
      0x8C100909,  //  0021  GETMET	R4	R4	K9
      0x7C100200,  //  0022  CALL	R4	1
      0x80000000,  //  0023  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_Plugin_Light1_init,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
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
    ( &(const binstruction[ 9]) {  /* code */
      0x600C0003,  //  0000  GETGBL	R3	G3
      0x5C100000,  //  0001  MOVE	R4	R0
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C0700,  //  0003  GETMET	R3	R3	K0
      0x5C140200,  //  0004  MOVE	R5	R1
      0x5C180400,  //  0005  MOVE	R6	R2
      0x7C0C0600,  //  0006  CALL	R3	3
      0x90020302,  //  0007  SETMBR	R0	K1	K2
      0x80000000,  //  0008  RET	0
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
    ( &(const bvalue[14]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(TLV),
    /* K3   */  be_nested_str_weak(cluster),
    /* K4   */  be_nested_str_weak(attribute),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(create_TLV),
    /* K7   */  be_nested_str_weak(U1),
    /* K8   */  be_nested_str_weak(shadow_bri),
    /* K9   */  be_const_int(2),
    /* K10  */  be_const_int(3),
    /* K11  */  be_nested_str_weak(U4),
    /* K12  */  be_const_int(1),
    /* K13  */  be_nested_str_weak(read_attribute),
    }),
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[77]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xB8120200,  //  0001  GETNGBL	R4	K1
      0x88100902,  //  0002  GETMBR	R4	R4	K2
      0x88140503,  //  0003  GETMBR	R5	R2	K3
      0x88180504,  //  0004  GETMBR	R6	R2	K4
      0x541E0007,  //  0005  LDINT	R7	8
      0x1C1C0A07,  //  0006  EQ	R7	R5	R7
      0x781E003B,  //  0007  JMPF	R7	#0044
      0x1C1C0D05,  //  0008  EQ	R7	R6	K5
      0x781E0005,  //  0009  JMPF	R7	#0010
      0x8C1C0906,  //  000A  GETMET	R7	R4	K6
      0x88240907,  //  000B  GETMBR	R9	R4	K7
      0x88280108,  //  000C  GETMBR	R10	R0	K8
      0x7C1C0600,  //  000D  CALL	R7	3
      0x80040E00,  //  000E  RET	1	R7
      0x70020032,  //  000F  JMP		#0043
      0x1C1C0D09,  //  0010  EQ	R7	R6	K9
      0x781E0005,  //  0011  JMPF	R7	#0018
      0x8C1C0906,  //  0012  GETMET	R7	R4	K6
      0x88240907,  //  0013  GETMBR	R9	R4	K7
      0x58280005,  //  0014  LDCONST	R10	K5
      0x7C1C0600,  //  0015  CALL	R7	3
      0x80040E00,  //  0016  RET	1	R7
      0x7002002A,  //  0017  JMP		#0043
      0x1C1C0D0A,  //  0018  EQ	R7	R6	K10
      0x781E0005,  //  0019  JMPF	R7	#0020
      0x8C1C0906,  //  001A  GETMET	R7	R4	K6
      0x88240907,  //  001B  GETMBR	R9	R4	K7
      0x542A00FD,  //  001C  LDINT	R10	254
      0x7C1C0600,  //  001D  CALL	R7	3
      0x80040E00,  //  001E  RET	1	R7
      0x70020022,  //  001F  JMP		#0043
      0x541E000E,  //  0020  LDINT	R7	15
      0x1C1C0C07,  //  0021  EQ	R7	R6	R7
      0x781E0005,  //  0022  JMPF	R7	#0029
      0x8C1C0906,  //  0023  GETMET	R7	R4	K6
      0x88240907,  //  0024  GETMBR	R9	R4	K7
      0x58280005,  //  0025  LDCONST	R10	K5
      0x7C1C0600,  //  0026  CALL	R7	3
      0x80040E00,  //  0027  RET	1	R7
      0x70020019,  //  0028  JMP		#0043
      0x541E0010,  //  0029  LDINT	R7	17
      0x1C1C0C07,  //  002A  EQ	R7	R6	R7
      0x781E0005,  //  002B  JMPF	R7	#0032
      0x8C1C0906,  //  002C  GETMET	R7	R4	K6
      0x88240907,  //  002D  GETMBR	R9	R4	K7
      0x88280108,  //  002E  GETMBR	R10	R0	K8
      0x7C1C0600,  //  002F  CALL	R7	3
      0x80040E00,  //  0030  RET	1	R7
      0x70020010,  //  0031  JMP		#0043
      0x541EFFFB,  //  0032  LDINT	R7	65532
      0x1C1C0C07,  //  0033  EQ	R7	R6	R7
      0x781E0005,  //  0034  JMPF	R7	#003B
      0x8C1C0906,  //  0035  GETMET	R7	R4	K6
      0x8824090B,  //  0036  GETMBR	R9	R4	K11
      0x5828000C,  //  0037  LDCONST	R10	K12
      0x7C1C0600,  //  0038  CALL	R7	3
      0x80040E00,  //  0039  RET	1	R7
      0x70020007,  //  003A  JMP		#0043
      0x541EFFFC,  //  003B  LDINT	R7	65533
      0x1C1C0C07,  //  003C  EQ	R7	R6	R7
      0x781E0004,  //  003D  JMPF	R7	#0043
      0x8C1C0906,  //  003E  GETMET	R7	R4	K6
      0x8824090B,  //  003F  GETMBR	R9	R4	K11
      0x542A0004,  //  0040  LDINT	R10	5
      0x7C1C0600,  //  0041  CALL	R7	3
      0x80040E00,  //  0042  RET	1	R7
      0x70020007,  //  0043  JMP		#004C
      0x601C0003,  //  0044  GETGBL	R7	G3
      0x5C200000,  //  0045  MOVE	R8	R0
      0x7C1C0200,  //  0046  CALL	R7	1
      0x8C1C0F0D,  //  0047  GETMET	R7	R7	K13
      0x5C240200,  //  0048  MOVE	R9	R1
      0x5C280400,  //  0049  MOVE	R10	R2
      0x7C1C0600,  //  004A  CALL	R7	3
      0x80040E00,  //  004B  RET	1	R7
      0x80000000,  //  004C  RET	0
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
    be_nested_map(7,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(invoke_request, 1), be_const_closure(Matter_Plugin_Light1_invoke_request_closure) },
        { be_const_key_weak(read_attribute, 4), be_const_closure(Matter_Plugin_Light1_read_attribute_closure) },
        { be_const_key_weak(shadow_bri, -1), be_const_var(0) },
        { be_const_key_weak(TYPES, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(257, -1), be_const_int(2) },
    }))    ) } )) },
        { be_const_key_weak(update_shadow, 6), be_const_closure(Matter_Plugin_Light1_update_shadow_closure) },
        { be_const_key_weak(init, -1), be_const_closure(Matter_Plugin_Light1_init_closure) },
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
