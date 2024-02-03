/* Solidification of Matter_Plugin_1_Aggregator.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Aggregator;

/********************************************************************
** Solidified function: read_attribute
********************************************************************/
be_local_closure(Matter_Plugin_Aggregator_read_attribute,   /* name */
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
    /* K11  */  be_const_int(2),
    /* K12  */  be_nested_str_weak(Matter_TLV_array),
    /* K13  */  be_nested_str_weak(add_TLV),
    /* K14  */  be_nested_str_weak(device),
    /* K15  */  be_nested_str_weak(get_active_endpoints),
    /* K16  */  be_nested_str_weak(AGGREGATOR_ENDPOINT),
    /* K17  */  be_nested_str_weak(stop_iteration),
    /* K18  */  be_nested_str_weak(read_attribute),
    }),
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[103]) {  /* code */
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
      0x7002003D,  //  0027  JMP		#0066
      0x541E001C,  //  0028  LDINT	R7	29
      0x1C1C0A07,  //  0029  EQ	R7	R5	R7
      0x781E0031,  //  002A  JMPF	R7	#005D
      0x1C1C0D0B,  //  002B  EQ	R7	R6	K11
      0x781E0008,  //  002C  JMPF	R7	#0036
      0x8C1C090C,  //  002D  GETMET	R7	R4	K12
      0x7C1C0200,  //  002E  CALL	R7	1
      0x8C200F0D,  //  002F  GETMET	R8	R7	K13
      0x4C280000,  //  0030  LDNIL	R10
      0x882C0907,  //  0031  GETMBR	R11	R4	K7
      0x5432001D,  //  0032  LDINT	R12	30
      0x7C200800,  //  0033  CALL	R8	4
      0x80040E00,  //  0034  RET	1	R7
      0x70020025,  //  0035  JMP		#005C
      0x1C1C0D04,  //  0036  EQ	R7	R6	K4
      0x781E001A,  //  0037  JMPF	R7	#0053
      0x8C1C090C,  //  0038  GETMET	R7	R4	K12
      0x7C1C0200,  //  0039  CALL	R7	1
      0x8820010E,  //  003A  GETMBR	R8	R0	K14
      0x8C20110F,  //  003B  GETMET	R8	R8	K15
      0x50280200,  //  003C  LDBOOL	R10	1	0
      0x7C200400,  //  003D  CALL	R8	2
      0x60240010,  //  003E  GETGBL	R9	G16
      0x5C281000,  //  003F  MOVE	R10	R8
      0x7C240200,  //  0040  CALL	R9	1
      0xA802000B,  //  0041  EXBLK	0	#004E
      0x5C281200,  //  0042  MOVE	R10	R9
      0x7C280000,  //  0043  CALL	R10	0
      0xB82E0000,  //  0044  GETNGBL	R11	K0
      0x882C1710,  //  0045  GETMBR	R11	R11	K16
      0x202C140B,  //  0046  NE	R11	R10	R11
      0x782E0004,  //  0047  JMPF	R11	#004D
      0x8C2C0F0D,  //  0048  GETMET	R11	R7	K13
      0x4C340000,  //  0049  LDNIL	R13
      0x88380907,  //  004A  GETMBR	R14	R4	K7
      0x5C3C1400,  //  004B  MOVE	R15	R10
      0x7C2C0800,  //  004C  CALL	R11	4
      0x7001FFF3,  //  004D  JMP		#0042
      0x58240011,  //  004E  LDCONST	R9	K17
      0xAC240200,  //  004F  CATCH	R9	1	0
      0xB0080000,  //  0050  RAISE	2	R0	R0
      0x80040E00,  //  0051  RET	1	R7
      0x70020008,  //  0052  JMP		#005C
      0x601C0003,  //  0053  GETGBL	R7	G3
      0x5C200000,  //  0054  MOVE	R8	R0
      0x7C1C0200,  //  0055  CALL	R7	1
      0x8C1C0F12,  //  0056  GETMET	R7	R7	K18
      0x5C240200,  //  0057  MOVE	R9	R1
      0x5C280400,  //  0058  MOVE	R10	R2
      0x5C2C0600,  //  0059  MOVE	R11	R3
      0x7C1C0800,  //  005A  CALL	R7	4
      0x80040E00,  //  005B  RET	1	R7
      0x70020008,  //  005C  JMP		#0066
      0x601C0003,  //  005D  GETGBL	R7	G3
      0x5C200000,  //  005E  MOVE	R8	R0
      0x7C1C0200,  //  005F  CALL	R7	1
      0x8C1C0F12,  //  0060  GETMET	R7	R7	K18
      0x5C240200,  //  0061  MOVE	R9	R1
      0x5C280400,  //  0062  MOVE	R10	R2
      0x5C2C0600,  //  0063  MOVE	R11	R3
      0x7C1C0800,  //  0064  CALL	R7	4
      0x80040E00,  //  0065  RET	1	R7
      0x80000000,  //  0066  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: invoke_request
********************************************************************/
be_local_closure(Matter_Plugin_Aggregator_invoke_request,   /* name */
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
    ( &(const binstruction[39]) {  /* code */
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
      0x70020008,  //  001C  JMP		#0026
      0x601C0003,  //  001D  GETGBL	R7	G3
      0x5C200000,  //  001E  MOVE	R8	R0
      0x7C1C0200,  //  001F  CALL	R7	1
      0x8C1C0F0A,  //  0020  GETMET	R7	R7	K10
      0x5C240200,  //  0021  MOVE	R9	R1
      0x5C280400,  //  0022  MOVE	R10	R2
      0x5C2C0600,  //  0023  MOVE	R11	R3
      0x7C1C0800,  //  0024  CALL	R7	4
      0x80040E00,  //  0025  RET	1	R7
      0x80000000,  //  0026  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Plugin_Aggregator
********************************************************************/
extern const bclass be_class_Matter_Plugin;
be_local_class(Matter_Plugin_Aggregator,
    0,
    &be_class_Matter_Plugin,
    be_nested_map(6,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(read_attribute, -1), be_const_closure(Matter_Plugin_Aggregator_read_attribute_closure) },
        { be_const_key_weak(TYPE, -1), be_nested_str_weak(aggregator) },
        { be_const_key_weak(TYPES, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(14, -1), be_const_int(1) },
    }))    ) } )) },
        { be_const_key_weak(DISPLAY_NAME, -1), be_nested_str_weak(Aggregator) },
        { be_const_key_weak(CLUSTERS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
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
        { be_const_key_int(3, 0), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(4,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
    }))    ) } )) },
        { be_const_key_weak(invoke_request, -1), be_const_closure(Matter_Plugin_Aggregator_invoke_request_closure) },
    })),
    be_str_weak(Matter_Plugin_Aggregator)
);
/*******************************************************************/

void be_load_Matter_Plugin_Aggregator_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Plugin_Aggregator);
    be_setglobal(vm, "Matter_Plugin_Aggregator");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
