/* Solidification of Matter_Plugin_1_Aggregator.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
// compact class 'Matter_Plugin_Aggregator' ktab size: 18, total: 25 (saved 56 bytes)
static const bvalue be_ktab_class_Matter_Plugin_Aggregator[18] = {
  /* K0   */  be_nested_str_weak(matter),
  /* K1   */  be_nested_str_weak(TLV),
  /* K2   */  be_nested_str_weak(cluster),
  /* K3   */  be_nested_str_weak(attribute),
  /* K4   */  be_const_int(3),
  /* K5   */  be_const_int(0),
  /* K6   */  be_nested_str_weak(set),
  /* K7   */  be_const_int(1),
  /* K8   */  be_const_int(2),
  /* K9   */  be_nested_str_weak(Matter_TLV_array),
  /* K10  */  be_nested_str_weak(add_TLV),
  /* K11  */  be_nested_str_weak(device),
  /* K12  */  be_nested_str_weak(get_active_endpoints),
  /* K13  */  be_nested_str_weak(stop_iteration),
  /* K14  */  be_nested_str_weak(read_attribute),
  /* K15  */  be_nested_str_weak(command),
  /* K16  */  be_nested_str_weak(Matter_TLV_struct),
  /* K17  */  be_nested_str_weak(invoke_request),
};


extern const bclass be_class_Matter_Plugin_Aggregator;

/********************************************************************
** Solidified function: read_attribute
********************************************************************/
be_local_closure(class_Matter_Plugin_Aggregator_read_attribute,   /* name */
  be_nested_proto(
    16,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Aggregator,     /* shared constants */
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[71]) {  /* code */
      0xB8120000,  //  0000  GETNGBL	R4	K0
      0x88100901,  //  0001  GETMBR	R4	R4	K1
      0x88140502,  //  0002  GETMBR	R5	R2	K2
      0x88180503,  //  0003  GETMBR	R6	R2	K3
      0x1C1C0B04,  //  0004  EQ	R7	R5	K4
      0x781E000F,  //  0005  JMPF	R7	#0016
      0x1C1C0D05,  //  0006  EQ	R7	R6	K5
      0x781E0005,  //  0007  JMPF	R7	#000E
      0x8C1C0706,  //  0008  GETMET	R7	R3	K6
      0x54260004,  //  0009  LDINT	R9	5
      0x58280005,  //  000A  LDCONST	R10	K5
      0x7C1C0600,  //  000B  CALL	R7	3
      0x80040E00,  //  000C  RET	1	R7
      0x70020006,  //  000D  JMP		#0015
      0x1C1C0D07,  //  000E  EQ	R7	R6	K7
      0x781E0004,  //  000F  JMPF	R7	#0015
      0x8C1C0706,  //  0010  GETMET	R7	R3	K6
      0x54260003,  //  0011  LDINT	R9	4
      0x58280005,  //  0012  LDCONST	R10	K5
      0x7C1C0600,  //  0013  CALL	R7	3
      0x80040E00,  //  0014  RET	1	R7
      0x70020027,  //  0015  JMP		#003E
      0x541E001C,  //  0016  LDINT	R7	29
      0x1C1C0A07,  //  0017  EQ	R7	R5	R7
      0x781E0024,  //  0018  JMPF	R7	#003E
      0x1C1C0D08,  //  0019  EQ	R7	R6	K8
      0x781E0008,  //  001A  JMPF	R7	#0024
      0x8C1C0909,  //  001B  GETMET	R7	R4	K9
      0x7C1C0200,  //  001C  CALL	R7	1
      0x8C200F0A,  //  001D  GETMET	R8	R7	K10
      0x4C280000,  //  001E  LDNIL	R10
      0x542E0004,  //  001F  LDINT	R11	5
      0x5432001D,  //  0020  LDINT	R12	30
      0x7C200800,  //  0021  CALL	R8	4
      0x80040E00,  //  0022  RET	1	R7
      0x70020019,  //  0023  JMP		#003E
      0x1C1C0D04,  //  0024  EQ	R7	R6	K4
      0x781E0017,  //  0025  JMPF	R7	#003E
      0x8C1C0909,  //  0026  GETMET	R7	R4	K9
      0x7C1C0200,  //  0027  CALL	R7	1
      0x8820010B,  //  0028  GETMBR	R8	R0	K11
      0x8C20110C,  //  0029  GETMET	R8	R8	K12
      0x50280200,  //  002A  LDBOOL	R10	1	0
      0x7C200400,  //  002B  CALL	R8	2
      0x60240010,  //  002C  GETGBL	R9	G16
      0x5C281000,  //  002D  MOVE	R10	R8
      0x7C240200,  //  002E  CALL	R9	1
      0xA8020009,  //  002F  EXBLK	0	#003A
      0x5C281200,  //  0030  MOVE	R10	R9
      0x7C280000,  //  0031  CALL	R10	0
      0x202C1507,  //  0032  NE	R11	R10	K7
      0x782E0004,  //  0033  JMPF	R11	#0039
      0x8C2C0F0A,  //  0034  GETMET	R11	R7	K10
      0x4C340000,  //  0035  LDNIL	R13
      0x543A0004,  //  0036  LDINT	R14	5
      0x5C3C1400,  //  0037  MOVE	R15	R10
      0x7C2C0800,  //  0038  CALL	R11	4
      0x7001FFF5,  //  0039  JMP		#0030
      0x5824000D,  //  003A  LDCONST	R9	K13
      0xAC240200,  //  003B  CATCH	R9	1	0
      0xB0080000,  //  003C  RAISE	2	R0	R0
      0x80040E00,  //  003D  RET	1	R7
      0x601C0003,  //  003E  GETGBL	R7	G3
      0x5C200000,  //  003F  MOVE	R8	R0
      0x7C1C0200,  //  0040  CALL	R7	1
      0x8C1C0F0E,  //  0041  GETMET	R7	R7	K14
      0x5C240200,  //  0042  MOVE	R9	R1
      0x5C280400,  //  0043  MOVE	R10	R2
      0x5C2C0600,  //  0044  MOVE	R11	R3
      0x7C1C0800,  //  0045  CALL	R7	4
      0x80040E00,  //  0046  RET	1	R7
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: invoke_request
********************************************************************/
be_local_closure(class_Matter_Plugin_Aggregator_invoke_request,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Aggregator,     /* shared constants */
    be_str_weak(invoke_request),
    &be_const_str_solidified,
    ( &(const binstruction[39]) {  /* code */
      0xB8120000,  //  0000  GETNGBL	R4	K0
      0x88100901,  //  0001  GETMBR	R4	R4	K1
      0x88140702,  //  0002  GETMBR	R5	R3	K2
      0x8818070F,  //  0003  GETMBR	R6	R3	K15
      0x1C1C0B04,  //  0004  EQ	R7	R5	K4
      0x781E0016,  //  0005  JMPF	R7	#001D
      0x1C1C0D05,  //  0006  EQ	R7	R6	K5
      0x781E0002,  //  0007  JMPF	R7	#000B
      0x501C0200,  //  0008  LDBOOL	R7	1	0
      0x80040E00,  //  0009  RET	1	R7
      0x70020010,  //  000A  JMP		#001C
      0x1C1C0D07,  //  000B  EQ	R7	R6	K7
      0x781E0009,  //  000C  JMPF	R7	#0017
      0x8C1C0910,  //  000D  GETMET	R7	R4	K16
      0x7C1C0200,  //  000E  CALL	R7	1
      0x8C200F0A,  //  000F  GETMET	R8	R7	K10
      0x58280005,  //  0010  LDCONST	R10	K5
      0x542E0004,  //  0011  LDINT	R11	5
      0x58300005,  //  0012  LDCONST	R12	K5
      0x7C200800,  //  0013  CALL	R8	4
      0x900E1F05,  //  0014  SETMBR	R3	K15	K5
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
      0x8C1C0F11,  //  0020  GETMET	R7	R7	K17
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
        { be_const_key_weak(read_attribute, -1), be_const_closure(class_Matter_Plugin_Aggregator_read_attribute_closure) },
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
        { be_const_key_int(29, -1), be_const_bytes_instance(0000000100020003FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(3, 0), be_const_bytes_instance(00000001FFF8FFF9FFFAFFFBFFFCFFFD) },
    }))    ) } )) },
        { be_const_key_weak(invoke_request, -1), be_const_closure(class_Matter_Plugin_Aggregator_invoke_request_closure) },
    })),
    be_str_weak(Matter_Plugin_Aggregator)
);
/********************************************************************/
/* End of solidification */
