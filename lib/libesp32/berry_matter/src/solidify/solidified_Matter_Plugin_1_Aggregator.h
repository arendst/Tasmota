/* Solidification of Matter_Plugin_1_Aggregator.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
// compact class 'Matter_Plugin_Aggregator' ktab size: 21, total: 29 (saved 64 bytes)
static const bvalue be_ktab_class_Matter_Plugin_Aggregator[21] = {
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
  /* K10  */  be_const_int(2),
  /* K11  */  be_nested_str_weak(Matter_TLV_array),
  /* K12  */  be_nested_str_weak(add_TLV),
  /* K13  */  be_nested_str_weak(device),
  /* K14  */  be_nested_str_weak(get_active_endpoints),
  /* K15  */  be_nested_str_weak(AGGREGATOR_ENDPOINT),
  /* K16  */  be_nested_str_weak(stop_iteration),
  /* K17  */  be_nested_str_weak(read_attribute),
  /* K18  */  be_nested_str_weak(command),
  /* K19  */  be_nested_str_weak(Matter_TLV_struct),
  /* K20  */  be_nested_str_weak(invoke_request),
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
    ( &(const binstruction[73]) {  /* code */
      0xB8120000,  //  0000  GETNGBL	R4	K0
      0x88100901,  //  0001  GETMBR	R4	R4	K1
      0x88140502,  //  0002  GETMBR	R5	R2	K2
      0x88180503,  //  0003  GETMBR	R6	R2	K3
      0x1C1C0B04,  //  0004  EQ	R7	R5	K4
      0x781E000F,  //  0005  JMPF	R7	#0016
      0x1C1C0D05,  //  0006  EQ	R7	R6	K5
      0x781E0005,  //  0007  JMPF	R7	#000E
      0x8C1C0706,  //  0008  GETMET	R7	R3	K6
      0x88240907,  //  0009  GETMBR	R9	R4	K7
      0x58280005,  //  000A  LDCONST	R10	K5
      0x7C1C0600,  //  000B  CALL	R7	3
      0x80040E00,  //  000C  RET	1	R7
      0x70020006,  //  000D  JMP		#0015
      0x1C1C0D08,  //  000E  EQ	R7	R6	K8
      0x781E0004,  //  000F  JMPF	R7	#0015
      0x8C1C0706,  //  0010  GETMET	R7	R3	K6
      0x88240909,  //  0011  GETMBR	R9	R4	K9
      0x58280005,  //  0012  LDCONST	R10	K5
      0x7C1C0600,  //  0013  CALL	R7	3
      0x80040E00,  //  0014  RET	1	R7
      0x70020029,  //  0015  JMP		#0040
      0x541E001C,  //  0016  LDINT	R7	29
      0x1C1C0A07,  //  0017  EQ	R7	R5	R7
      0x781E0026,  //  0018  JMPF	R7	#0040
      0x1C1C0D0A,  //  0019  EQ	R7	R6	K10
      0x781E0008,  //  001A  JMPF	R7	#0024
      0x8C1C090B,  //  001B  GETMET	R7	R4	K11
      0x7C1C0200,  //  001C  CALL	R7	1
      0x8C200F0C,  //  001D  GETMET	R8	R7	K12
      0x4C280000,  //  001E  LDNIL	R10
      0x882C0907,  //  001F  GETMBR	R11	R4	K7
      0x5432001D,  //  0020  LDINT	R12	30
      0x7C200800,  //  0021  CALL	R8	4
      0x80040E00,  //  0022  RET	1	R7
      0x7002001B,  //  0023  JMP		#0040
      0x1C1C0D04,  //  0024  EQ	R7	R6	K4
      0x781E0019,  //  0025  JMPF	R7	#0040
      0x8C1C090B,  //  0026  GETMET	R7	R4	K11
      0x7C1C0200,  //  0027  CALL	R7	1
      0x8820010D,  //  0028  GETMBR	R8	R0	K13
      0x8C20110E,  //  0029  GETMET	R8	R8	K14
      0x50280200,  //  002A  LDBOOL	R10	1	0
      0x7C200400,  //  002B  CALL	R8	2
      0x60240010,  //  002C  GETGBL	R9	G16
      0x5C281000,  //  002D  MOVE	R10	R8
      0x7C240200,  //  002E  CALL	R9	1
      0xA802000B,  //  002F  EXBLK	0	#003C
      0x5C281200,  //  0030  MOVE	R10	R9
      0x7C280000,  //  0031  CALL	R10	0
      0xB82E0000,  //  0032  GETNGBL	R11	K0
      0x882C170F,  //  0033  GETMBR	R11	R11	K15
      0x202C140B,  //  0034  NE	R11	R10	R11
      0x782E0004,  //  0035  JMPF	R11	#003B
      0x8C2C0F0C,  //  0036  GETMET	R11	R7	K12
      0x4C340000,  //  0037  LDNIL	R13
      0x88380907,  //  0038  GETMBR	R14	R4	K7
      0x5C3C1400,  //  0039  MOVE	R15	R10
      0x7C2C0800,  //  003A  CALL	R11	4
      0x7001FFF3,  //  003B  JMP		#0030
      0x58240010,  //  003C  LDCONST	R9	K16
      0xAC240200,  //  003D  CATCH	R9	1	0
      0xB0080000,  //  003E  RAISE	2	R0	R0
      0x80040E00,  //  003F  RET	1	R7
      0x601C0003,  //  0040  GETGBL	R7	G3
      0x5C200000,  //  0041  MOVE	R8	R0
      0x7C1C0200,  //  0042  CALL	R7	1
      0x8C1C0F11,  //  0043  GETMET	R7	R7	K17
      0x5C240200,  //  0044  MOVE	R9	R1
      0x5C280400,  //  0045  MOVE	R10	R2
      0x5C2C0600,  //  0046  MOVE	R11	R3
      0x7C1C0800,  //  0047  CALL	R7	4
      0x80040E00,  //  0048  RET	1	R7
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
      0x88180712,  //  0003  GETMBR	R6	R3	K18
      0x1C1C0B04,  //  0004  EQ	R7	R5	K4
      0x781E0016,  //  0005  JMPF	R7	#001D
      0x1C1C0D05,  //  0006  EQ	R7	R6	K5
      0x781E0002,  //  0007  JMPF	R7	#000B
      0x501C0200,  //  0008  LDBOOL	R7	1	0
      0x80040E00,  //  0009  RET	1	R7
      0x70020010,  //  000A  JMP		#001C
      0x1C1C0D08,  //  000B  EQ	R7	R6	K8
      0x781E0009,  //  000C  JMPF	R7	#0017
      0x8C1C0913,  //  000D  GETMET	R7	R4	K19
      0x7C1C0200,  //  000E  CALL	R7	1
      0x8C200F0C,  //  000F  GETMET	R8	R7	K12
      0x58280005,  //  0010  LDCONST	R10	K5
      0x882C0907,  //  0011  GETMBR	R11	R4	K7
      0x58300005,  //  0012  LDCONST	R12	K5
      0x7C200800,  //  0013  CALL	R8	4
      0x900E2505,  //  0014  SETMBR	R3	K18	K5
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
      0x8C1C0F14,  //  0020  GETMET	R7	R7	K20
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
