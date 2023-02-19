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
    14,                          /* nstack */
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
    /* K4   */  be_nested_str_weak(session),
    /* K5   */  be_const_int(3),
    /* K6   */  be_const_int(0),
    /* K7   */  be_const_int(1),
    /* K8   */  be_nested_str_weak(Matter_TLV_struct),
    /* K9   */  be_nested_str_weak(add_TLV),
    /* K10  */  be_nested_str_weak(U2),
    /* K11  */  be_nested_str_weak(onoff),
    /* K12  */  be_const_int(2),
    }),
    be_str_weak(invoke_request),
    &be_const_str_solidified,
    ( &(const binstruction[69]) {  /* code */
      0xB8120000,  //  0000  GETNGBL	R4	K0
      0x88100901,  //  0001  GETMBR	R4	R4	K1
      0x88140702,  //  0002  GETMBR	R5	R3	K2
      0x88180703,  //  0003  GETMBR	R6	R3	K3
      0x881C0304,  //  0004  GETMBR	R7	R1	K4
      0x1C200B05,  //  0005  EQ	R8	R5	K5
      0x78220016,  //  0006  JMPF	R8	#001E
      0x1C200D06,  //  0007  EQ	R8	R6	K6
      0x78220002,  //  0008  JMPF	R8	#000C
      0x50200200,  //  0009  LDBOOL	R8	1	0
      0x80041000,  //  000A  RET	1	R8
      0x70020010,  //  000B  JMP		#001D
      0x1C200D07,  //  000C  EQ	R8	R6	K7
      0x78220009,  //  000D  JMPF	R8	#0018
      0x8C200908,  //  000E  GETMET	R8	R4	K8
      0x7C200200,  //  000F  CALL	R8	1
      0x8C241109,  //  0010  GETMET	R9	R8	K9
      0x582C0006,  //  0011  LDCONST	R11	K6
      0x8830090A,  //  0012  GETMBR	R12	R4	K10
      0x58340006,  //  0013  LDCONST	R13	K6
      0x7C240800,  //  0014  CALL	R9	4
      0x900E0706,  //  0015  SETMBR	R3	K3	K6
      0x80041000,  //  0016  RET	1	R8
      0x70020004,  //  0017  JMP		#001D
      0x5422003F,  //  0018  LDINT	R8	64
      0x1C200C08,  //  0019  EQ	R8	R6	R8
      0x78220001,  //  001A  JMPF	R8	#001D
      0x50200200,  //  001B  LDBOOL	R8	1	0
      0x80041000,  //  001C  RET	1	R8
      0x70020025,  //  001D  JMP		#0044
      0x54220003,  //  001E  LDINT	R8	4
      0x1C200A08,  //  001F  EQ	R8	R5	R8
      0x78220002,  //  0020  JMPF	R8	#0024
      0x50200200,  //  0021  LDBOOL	R8	1	0
      0x80041000,  //  0022  RET	1	R8
      0x7002001F,  //  0023  JMP		#0044
      0x54220004,  //  0024  LDINT	R8	5
      0x1C200A08,  //  0025  EQ	R8	R5	R8
      0x78220002,  //  0026  JMPF	R8	#002A
      0x50200200,  //  0027  LDBOOL	R8	1	0
      0x80041000,  //  0028  RET	1	R8
      0x70020019,  //  0029  JMP		#0044
      0x54220005,  //  002A  LDINT	R8	6
      0x1C200A08,  //  002B  EQ	R8	R5	R8
      0x78220016,  //  002C  JMPF	R8	#0044
      0x1C200D06,  //  002D  EQ	R8	R6	K6
      0x78220004,  //  002E  JMPF	R8	#0034
      0x50200000,  //  002F  LDBOOL	R8	0	0
      0x90021608,  //  0030  SETMBR	R0	K11	R8
      0x50200200,  //  0031  LDBOOL	R8	1	0
      0x80041000,  //  0032  RET	1	R8
      0x7002000F,  //  0033  JMP		#0044
      0x1C200D07,  //  0034  EQ	R8	R6	K7
      0x78220004,  //  0035  JMPF	R8	#003B
      0x50200200,  //  0036  LDBOOL	R8	1	0
      0x90021608,  //  0037  SETMBR	R0	K11	R8
      0x50200200,  //  0038  LDBOOL	R8	1	0
      0x80041000,  //  0039  RET	1	R8
      0x70020008,  //  003A  JMP		#0044
      0x1C200D0C,  //  003B  EQ	R8	R6	K12
      0x78220006,  //  003C  JMPF	R8	#0044
      0x8820010B,  //  003D  GETMBR	R8	R0	K11
      0x78220000,  //  003E  JMPF	R8	#0040
      0x50200001,  //  003F  LDBOOL	R8	0	1
      0x50200200,  //  0040  LDBOOL	R8	1	0
      0x90021608,  //  0041  SETMBR	R0	K11	R8
      0x50200200,  //  0042  LDBOOL	R8	1	0
      0x80041000,  //  0043  RET	1	R8
      0x80000000,  //  0044  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_Plugin_OnOff_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(init),
    /* K1   */  be_nested_str_weak(endpoints),
    /* K2   */  be_nested_str_weak(ENDPOINTS),
    /* K3   */  be_nested_str_weak(clusters),
    /* K4   */  be_nested_str_weak(CLUSTERS),
    /* K5   */  be_nested_str_weak(onoff),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080500,  //  0003  GETMET	R2	R2	K0
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x88080102,  //  0006  GETMBR	R2	R0	K2
      0x90020202,  //  0007  SETMBR	R0	K1	R2
      0x88080104,  //  0008  GETMBR	R2	R0	K4
      0x90020602,  //  0009  SETMBR	R0	K3	R2
      0x50080000,  //  000A  LDBOOL	R2	0	0
      0x90020A02,  //  000B  SETMBR	R0	K5	R2
      0x80000000,  //  000C  RET	0
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
    ( &(const bvalue[21]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(TLV),
    /* K3   */  be_nested_str_weak(cluster),
    /* K4   */  be_nested_str_weak(attribute),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(Matter_TLV_array),
    /* K7   */  be_nested_str_weak(TYPES),
    /* K8   */  be_nested_str_weak(add_struct),
    /* K9   */  be_nested_str_weak(add_TLV),
    /* K10  */  be_nested_str_weak(U2),
    /* K11  */  be_const_int(1),
    /* K12  */  be_nested_str_weak(stop_iteration),
    /* K13  */  be_nested_str_weak(get_cluster_list),
    /* K14  */  be_nested_str_weak(U4),
    /* K15  */  be_const_int(2),
    /* K16  */  be_const_int(3),
    /* K17  */  be_nested_str_weak(create_TLV),
    /* K18  */  be_nested_str_weak(U1),
    /* K19  */  be_nested_str_weak(BOOL),
    /* K20  */  be_nested_str_weak(onoff),
    }),
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[113]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xB8120200,  //  0001  GETNGBL	R4	K1
      0x88100902,  //  0002  GETMBR	R4	R4	K2
      0x88140503,  //  0003  GETMBR	R5	R2	K3
      0x88180504,  //  0004  GETMBR	R6	R2	K4
      0x541E001C,  //  0005  LDINT	R7	29
      0x1C1C0A07,  //  0006  EQ	R7	R5	R7
      0x781E0042,  //  0007  JMPF	R7	#004B
      0x1C1C0D05,  //  0008  EQ	R7	R6	K5
      0x781E0019,  //  0009  JMPF	R7	#0024
      0x8C1C0906,  //  000A  GETMET	R7	R4	K6
      0x7C1C0200,  //  000B  CALL	R7	1
      0x60200010,  //  000C  GETGBL	R8	G16
      0x88240107,  //  000D  GETMBR	R9	R0	K7
      0x7C200200,  //  000E  CALL	R8	1
      0xA802000E,  //  000F  EXBLK	0	#001F
      0x5C241000,  //  0010  MOVE	R9	R8
      0x7C240000,  //  0011  CALL	R9	0
      0x8C280F08,  //  0012  GETMET	R10	R7	K8
      0x7C280200,  //  0013  CALL	R10	1
      0x8C2C1509,  //  0014  GETMET	R11	R10	K9
      0x58340005,  //  0015  LDCONST	R13	K5
      0x8838090A,  //  0016  GETMBR	R14	R4	K10
      0x5C3C1200,  //  0017  MOVE	R15	R9
      0x7C2C0800,  //  0018  CALL	R11	4
      0x8C2C1509,  //  0019  GETMET	R11	R10	K9
      0x5834000B,  //  001A  LDCONST	R13	K11
      0x8838090A,  //  001B  GETMBR	R14	R4	K10
      0x583C000B,  //  001C  LDCONST	R15	K11
      0x7C2C0800,  //  001D  CALL	R11	4
      0x7001FFF0,  //  001E  JMP		#0010
      0x5820000C,  //  001F  LDCONST	R8	K12
      0xAC200200,  //  0020  CATCH	R8	1	0
      0xB0080000,  //  0021  RAISE	2	R0	R0
      0x80040E00,  //  0022  RET	1	R7
      0x70020025,  //  0023  JMP		#004A
      0x1C1C0D0B,  //  0024  EQ	R7	R6	K11
      0x781E0013,  //  0025  JMPF	R7	#003A
      0x8C1C0906,  //  0026  GETMET	R7	R4	K6
      0x7C1C0200,  //  0027  CALL	R7	1
      0x60200010,  //  0028  GETGBL	R8	G16
      0x8C24010D,  //  0029  GETMET	R9	R0	K13
      0x7C240200,  //  002A  CALL	R9	1
      0x7C200200,  //  002B  CALL	R8	1
      0xA8020007,  //  002C  EXBLK	0	#0035
      0x5C241000,  //  002D  MOVE	R9	R8
      0x7C240000,  //  002E  CALL	R9	0
      0x8C280F09,  //  002F  GETMET	R10	R7	K9
      0x4C300000,  //  0030  LDNIL	R12
      0x8834090E,  //  0031  GETMBR	R13	R4	K14
      0x5C381200,  //  0032  MOVE	R14	R9
      0x7C280800,  //  0033  CALL	R10	4
      0x7001FFF7,  //  0034  JMP		#002D
      0x5820000C,  //  0035  LDCONST	R8	K12
      0xAC200200,  //  0036  CATCH	R8	1	0
      0xB0080000,  //  0037  RAISE	2	R0	R0
      0x80040E00,  //  0038  RET	1	R7
      0x7002000F,  //  0039  JMP		#004A
      0x1C1C0D0F,  //  003A  EQ	R7	R6	K15
      0x781E0008,  //  003B  JMPF	R7	#0045
      0x8C1C0906,  //  003C  GETMET	R7	R4	K6
      0x7C1C0200,  //  003D  CALL	R7	1
      0x8C200F09,  //  003E  GETMET	R8	R7	K9
      0x4C280000,  //  003F  LDNIL	R10
      0x882C090A,  //  0040  GETMBR	R11	R4	K10
      0x54320005,  //  0041  LDINT	R12	6
      0x7C200800,  //  0042  CALL	R8	4
      0x80040E00,  //  0043  RET	1	R7
      0x70020004,  //  0044  JMP		#004A
      0x1C1C0D10,  //  0045  EQ	R7	R6	K16
      0x781E0002,  //  0046  JMPF	R7	#004A
      0x8C1C0906,  //  0047  GETMET	R7	R4	K6
      0x7C1C0200,  //  0048  CALL	R7	1
      0x80040E00,  //  0049  RET	1	R7
      0x70020024,  //  004A  JMP		#0070
      0x1C1C0B10,  //  004B  EQ	R7	R5	K16
      0x781E000F,  //  004C  JMPF	R7	#005D
      0x1C1C0D05,  //  004D  EQ	R7	R6	K5
      0x781E0005,  //  004E  JMPF	R7	#0055
      0x8C1C0911,  //  004F  GETMET	R7	R4	K17
      0x8824090A,  //  0050  GETMBR	R9	R4	K10
      0x58280005,  //  0051  LDCONST	R10	K5
      0x7C1C0600,  //  0052  CALL	R7	3
      0x80040E00,  //  0053  RET	1	R7
      0x70020006,  //  0054  JMP		#005C
      0x1C1C0D0B,  //  0055  EQ	R7	R6	K11
      0x781E0004,  //  0056  JMPF	R7	#005C
      0x8C1C0911,  //  0057  GETMET	R7	R4	K17
      0x88240912,  //  0058  GETMBR	R9	R4	K18
      0x58280005,  //  0059  LDCONST	R10	K5
      0x7C1C0600,  //  005A  CALL	R7	3
      0x80040E00,  //  005B  RET	1	R7
      0x70020012,  //  005C  JMP		#0070
      0x541E0005,  //  005D  LDINT	R7	6
      0x1C1C0A07,  //  005E  EQ	R7	R5	R7
      0x781E000F,  //  005F  JMPF	R7	#0070
      0x1C1C0D05,  //  0060  EQ	R7	R6	K5
      0x781E0005,  //  0061  JMPF	R7	#0068
      0x8C1C0911,  //  0062  GETMET	R7	R4	K17
      0x88240913,  //  0063  GETMBR	R9	R4	K19
      0x88280114,  //  0064  GETMBR	R10	R0	K20
      0x7C1C0600,  //  0065  CALL	R7	3
      0x80040E00,  //  0066  RET	1	R7
      0x70020007,  //  0067  JMP		#0070
      0x541EFFFB,  //  0068  LDINT	R7	65532
      0x1C1C0C07,  //  0069  EQ	R7	R6	R7
      0x781E0004,  //  006A  JMPF	R7	#0070
      0x8C1C0911,  //  006B  GETMET	R7	R4	K17
      0x8824090E,  //  006C  GETMBR	R9	R4	K14
      0x58280005,  //  006D  LDCONST	R10	K5
      0x7C1C0600,  //  006E  CALL	R7	3
      0x80040E00,  //  006F  RET	1	R7
      0x80000000,  //  0070  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Plugin_OnOff
********************************************************************/
extern const bclass be_class_Matter_Plugin;
be_local_class(Matter_Plugin_OnOff,
    1,
    &be_class_Matter_Plugin,
    be_nested_map(7,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(invoke_request, 1), be_const_closure(Matter_Plugin_OnOff_invoke_request_closure) },
        { be_const_key_weak(read_attribute, 4), be_const_closure(Matter_Plugin_OnOff_read_attribute_closure) },
        { be_const_key_weak(init, -1), be_const_closure(Matter_Plugin_OnOff_init_closure) },
        { be_const_key_weak(TYPES, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(1,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(256),
    }))    ) } )) },
        { be_const_key_weak(CLUSTERS, 6), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(5,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(5, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(6,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(2),
        be_const_int(3),
        be_const_int(4),
        be_const_int(5),
    }))    ) } )) },
        { be_const_key_int(6, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(2,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(65532),
    }))    ) } )) },
        { be_const_key_int(29, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(4,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(2),
        be_const_int(3),
    }))    ) } )) },
        { be_const_key_int(3, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(2,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
    }))    ) } )) },
        { be_const_key_int(4, 2), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(1,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
    }))    ) } )) },
    }))    ) } )) },
        { be_const_key_weak(onoff, 2), be_const_var(0) },
        { be_const_key_weak(ENDPOINTS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(1,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(1),
    }))    ) } )) },
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
