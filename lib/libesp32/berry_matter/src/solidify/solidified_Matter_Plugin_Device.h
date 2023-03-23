/* Solidification of Matter_Plugin_Device.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Device;

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
    ( &(const binstruction[45]) {  /* code */
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
      0x7002000E,  //  001C  JMP		#002C
      0x541E0003,  //  001D  LDINT	R7	4
      0x1C1C0A07,  //  001E  EQ	R7	R5	R7
      0x781E0002,  //  001F  JMPF	R7	#0023
      0x501C0200,  //  0020  LDBOOL	R7	1	0
      0x80040E00,  //  0021  RET	1	R7
      0x70020008,  //  0022  JMP		#002C
      0x601C0003,  //  0023  GETGBL	R7	G3
      0x5C200000,  //  0024  MOVE	R8	R0
      0x7C1C0200,  //  0025  CALL	R7	1
      0x8C1C0F0A,  //  0026  GETMET	R7	R7	K10
      0x5C240200,  //  0027  MOVE	R9	R1
      0x5C280400,  //  0028  MOVE	R10	R2
      0x5C2C0600,  //  0029  MOVE	R11	R3
      0x7C1C0800,  //  002A  CALL	R7	4
      0x80040E00,  //  002B  RET	1	R7
      0x80000000,  //  002C  RET	0
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
    ( &(const bvalue[21]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(TLV),
    /* K3   */  be_nested_str_weak(cluster),
    /* K4   */  be_nested_str_weak(attribute),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(Matter_TLV_array),
    /* K7   */  be_nested_str_weak(TYPES),
    /* K8   */  be_nested_str_weak(keys),
    /* K9   */  be_nested_str_weak(add_struct),
    /* K10  */  be_nested_str_weak(add_TLV),
    /* K11  */  be_nested_str_weak(U2),
    /* K12  */  be_const_int(1),
    /* K13  */  be_nested_str_weak(stop_iteration),
    /* K14  */  be_nested_str_weak(get_cluster_list),
    /* K15  */  be_nested_str_weak(U4),
    /* K16  */  be_const_int(2),
    /* K17  */  be_const_int(3),
    /* K18  */  be_nested_str_weak(create_TLV),
    /* K19  */  be_nested_str_weak(U1),
    /* K20  */  be_nested_str_weak(read_attribute),
    }),
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[167]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xB8120200,  //  0001  GETNGBL	R4	K1
      0x88100902,  //  0002  GETMBR	R4	R4	K2
      0x88140503,  //  0003  GETMBR	R5	R2	K3
      0x88180504,  //  0004  GETMBR	R6	R2	K4
      0x541E001C,  //  0005  LDINT	R7	29
      0x1C1C0A07,  //  0006  EQ	R7	R5	R7
      0x781E0057,  //  0007  JMPF	R7	#0060
      0x1C1C0D05,  //  0008  EQ	R7	R6	K5
      0x781E001C,  //  0009  JMPF	R7	#0027
      0x8C1C0906,  //  000A  GETMET	R7	R4	K6
      0x7C1C0200,  //  000B  CALL	R7	1
      0x60200010,  //  000C  GETGBL	R8	G16
      0x88240107,  //  000D  GETMBR	R9	R0	K7
      0x8C241308,  //  000E  GETMET	R9	R9	K8
      0x7C240200,  //  000F  CALL	R9	1
      0x7C200200,  //  0010  CALL	R8	1
      0xA802000F,  //  0011  EXBLK	0	#0022
      0x5C241000,  //  0012  MOVE	R9	R8
      0x7C240000,  //  0013  CALL	R9	0
      0x8C280F09,  //  0014  GETMET	R10	R7	K9
      0x7C280200,  //  0015  CALL	R10	1
      0x8C2C150A,  //  0016  GETMET	R11	R10	K10
      0x58340005,  //  0017  LDCONST	R13	K5
      0x8838090B,  //  0018  GETMBR	R14	R4	K11
      0x5C3C1200,  //  0019  MOVE	R15	R9
      0x7C2C0800,  //  001A  CALL	R11	4
      0x8C2C150A,  //  001B  GETMET	R11	R10	K10
      0x5834000C,  //  001C  LDCONST	R13	K12
      0x8838090B,  //  001D  GETMBR	R14	R4	K11
      0x883C0107,  //  001E  GETMBR	R15	R0	K7
      0x943C1E09,  //  001F  GETIDX	R15	R15	R9
      0x7C2C0800,  //  0020  CALL	R11	4
      0x7001FFEF,  //  0021  JMP		#0012
      0x5820000D,  //  0022  LDCONST	R8	K13
      0xAC200200,  //  0023  CATCH	R8	1	0
      0xB0080000,  //  0024  RAISE	2	R0	R0
      0x80040E00,  //  0025  RET	1	R7
      0x70020037,  //  0026  JMP		#005F
      0x1C1C0D0C,  //  0027  EQ	R7	R6	K12
      0x781E0013,  //  0028  JMPF	R7	#003D
      0x8C1C0906,  //  0029  GETMET	R7	R4	K6
      0x7C1C0200,  //  002A  CALL	R7	1
      0x60200010,  //  002B  GETGBL	R8	G16
      0x8C24010E,  //  002C  GETMET	R9	R0	K14
      0x7C240200,  //  002D  CALL	R9	1
      0x7C200200,  //  002E  CALL	R8	1
      0xA8020007,  //  002F  EXBLK	0	#0038
      0x5C241000,  //  0030  MOVE	R9	R8
      0x7C240000,  //  0031  CALL	R9	0
      0x8C280F0A,  //  0032  GETMET	R10	R7	K10
      0x4C300000,  //  0033  LDNIL	R12
      0x8834090F,  //  0034  GETMBR	R13	R4	K15
      0x5C381200,  //  0035  MOVE	R14	R9
      0x7C280800,  //  0036  CALL	R10	4
      0x7001FFF7,  //  0037  JMP		#0030
      0x5820000D,  //  0038  LDCONST	R8	K13
      0xAC200200,  //  0039  CATCH	R8	1	0
      0xB0080000,  //  003A  RAISE	2	R0	R0
      0x80040E00,  //  003B  RET	1	R7
      0x70020021,  //  003C  JMP		#005F
      0x1C1C0D10,  //  003D  EQ	R7	R6	K16
      0x781E0008,  //  003E  JMPF	R7	#0048
      0x8C1C0906,  //  003F  GETMET	R7	R4	K6
      0x7C1C0200,  //  0040  CALL	R7	1
      0x8C200F0A,  //  0041  GETMET	R8	R7	K10
      0x4C280000,  //  0042  LDNIL	R10
      0x882C090B,  //  0043  GETMBR	R11	R4	K11
      0x54320005,  //  0044  LDINT	R12	6
      0x7C200800,  //  0045  CALL	R8	4
      0x80040E00,  //  0046  RET	1	R7
      0x70020016,  //  0047  JMP		#005F
      0x1C1C0D11,  //  0048  EQ	R7	R6	K17
      0x781E0003,  //  0049  JMPF	R7	#004E
      0x8C1C0906,  //  004A  GETMET	R7	R4	K6
      0x7C1C0200,  //  004B  CALL	R7	1
      0x80040E00,  //  004C  RET	1	R7
      0x70020010,  //  004D  JMP		#005F
      0x541EFFFB,  //  004E  LDINT	R7	65532
      0x1C1C0C07,  //  004F  EQ	R7	R6	R7
      0x781E0005,  //  0050  JMPF	R7	#0057
      0x8C1C0912,  //  0051  GETMET	R7	R4	K18
      0x8824090F,  //  0052  GETMBR	R9	R4	K15
      0x58280005,  //  0053  LDCONST	R10	K5
      0x7C1C0600,  //  0054  CALL	R7	3
      0x80040E00,  //  0055  RET	1	R7
      0x70020007,  //  0056  JMP		#005F
      0x541EFFFC,  //  0057  LDINT	R7	65533
      0x1C1C0C07,  //  0058  EQ	R7	R6	R7
      0x781E0004,  //  0059  JMPF	R7	#005F
      0x8C1C0912,  //  005A  GETMET	R7	R4	K18
      0x8824090F,  //  005B  GETMBR	R9	R4	K15
      0x5828000C,  //  005C  LDCONST	R10	K12
      0x7C1C0600,  //  005D  CALL	R7	3
      0x80040E00,  //  005E  RET	1	R7
      0x70020045,  //  005F  JMP		#00A6
      0x1C1C0B11,  //  0060  EQ	R7	R5	K17
      0x781E0021,  //  0061  JMPF	R7	#0084
      0x1C1C0D05,  //  0062  EQ	R7	R6	K5
      0x781E0005,  //  0063  JMPF	R7	#006A
      0x8C1C0912,  //  0064  GETMET	R7	R4	K18
      0x8824090B,  //  0065  GETMBR	R9	R4	K11
      0x58280005,  //  0066  LDCONST	R10	K5
      0x7C1C0600,  //  0067  CALL	R7	3
      0x80040E00,  //  0068  RET	1	R7
      0x70020018,  //  0069  JMP		#0083
      0x1C1C0D0C,  //  006A  EQ	R7	R6	K12
      0x781E0005,  //  006B  JMPF	R7	#0072
      0x8C1C0912,  //  006C  GETMET	R7	R4	K18
      0x88240913,  //  006D  GETMBR	R9	R4	K19
      0x58280005,  //  006E  LDCONST	R10	K5
      0x7C1C0600,  //  006F  CALL	R7	3
      0x80040E00,  //  0070  RET	1	R7
      0x70020010,  //  0071  JMP		#0083
      0x541EFFFB,  //  0072  LDINT	R7	65532
      0x1C1C0C07,  //  0073  EQ	R7	R6	R7
      0x781E0005,  //  0074  JMPF	R7	#007B
      0x8C1C0912,  //  0075  GETMET	R7	R4	K18
      0x8824090F,  //  0076  GETMBR	R9	R4	K15
      0x58280005,  //  0077  LDCONST	R10	K5
      0x7C1C0600,  //  0078  CALL	R7	3
      0x80040E00,  //  0079  RET	1	R7
      0x70020007,  //  007A  JMP		#0083
      0x541EFFFC,  //  007B  LDINT	R7	65533
      0x1C1C0C07,  //  007C  EQ	R7	R6	R7
      0x781E0004,  //  007D  JMPF	R7	#0083
      0x8C1C0912,  //  007E  GETMET	R7	R4	K18
      0x8824090F,  //  007F  GETMBR	R9	R4	K15
      0x542A0003,  //  0080  LDINT	R10	4
      0x7C1C0600,  //  0081  CALL	R7	3
      0x80040E00,  //  0082  RET	1	R7
      0x70020021,  //  0083  JMP		#00A6
      0x541E0003,  //  0084  LDINT	R7	4
      0x1C1C0A07,  //  0085  EQ	R7	R5	R7
      0x781E0016,  //  0086  JMPF	R7	#009E
      0x1C1C0D05,  //  0087  EQ	R7	R6	K5
      0x781E0002,  //  0088  JMPF	R7	#008C
      0x4C1C0000,  //  0089  LDNIL	R7
      0x80040E00,  //  008A  RET	1	R7
      0x70020010,  //  008B  JMP		#009D
      0x541EFFFB,  //  008C  LDINT	R7	65532
      0x1C1C0C07,  //  008D  EQ	R7	R6	R7
      0x781E0005,  //  008E  JMPF	R7	#0095
      0x8C1C0912,  //  008F  GETMET	R7	R4	K18
      0x8824090F,  //  0090  GETMBR	R9	R4	K15
      0x58280005,  //  0091  LDCONST	R10	K5
      0x7C1C0600,  //  0092  CALL	R7	3
      0x80040E00,  //  0093  RET	1	R7
      0x70020007,  //  0094  JMP		#009D
      0x541EFFFC,  //  0095  LDINT	R7	65533
      0x1C1C0C07,  //  0096  EQ	R7	R6	R7
      0x781E0004,  //  0097  JMPF	R7	#009D
      0x8C1C0912,  //  0098  GETMET	R7	R4	K18
      0x8824090F,  //  0099  GETMBR	R9	R4	K15
      0x542A0003,  //  009A  LDINT	R10	4
      0x7C1C0600,  //  009B  CALL	R7	3
      0x80040E00,  //  009C  RET	1	R7
      0x70020007,  //  009D  JMP		#00A6
      0x601C0003,  //  009E  GETGBL	R7	G3
      0x5C200000,  //  009F  MOVE	R8	R0
      0x7C1C0200,  //  00A0  CALL	R7	1
      0x8C1C0F14,  //  00A1  GETMET	R7	R7	K20
      0x5C240200,  //  00A2  MOVE	R9	R1
      0x5C280400,  //  00A3  MOVE	R10	R2
      0x7C1C0600,  //  00A4  CALL	R7	3
      0x80040E00,  //  00A5  RET	1	R7
      0x80000000,  //  00A6  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_Plugin_Device_init,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(init),
    /* K1   */  be_nested_str_weak(clusters),
    /* K2   */  be_nested_str_weak(CLUSTERS),
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
      0x7C100600,  //  0006  CALL	R4	3
      0x88100102,  //  0007  GETMBR	R4	R0	K2
      0x90020204,  //  0008  SETMBR	R0	K1	R4
      0x80000000,  //  0009  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Plugin_Device
********************************************************************/
extern const bclass be_class_Matter_Plugin;
be_local_class(Matter_Plugin_Device,
    0,
    &be_class_Matter_Plugin,
    be_nested_map(5,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(TYPES, 3), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(0, -1), be_const_int(0) },
    }))    ) } )) },
        { be_const_key_weak(read_attribute, -1), be_const_closure(Matter_Plugin_Device_read_attribute_closure) },
        { be_const_key_weak(invoke_request, 1), be_const_closure(Matter_Plugin_Device_invoke_request_closure) },
        { be_const_key_weak(init, -1), be_const_closure(Matter_Plugin_Device_init_closure) },
        { be_const_key_weak(CLUSTERS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(3, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(4,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(4, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(3,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
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
