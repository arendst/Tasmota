/* Solidification of Matter_IM_Data.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
// compact class 'Matter_IM_base' ktab size: 10, total: 11 (saved 8 bytes)
static const bvalue be_ktab_class_Matter_IM_base[10] = {
  /* K0   */  be_nested_str_weak(add_array),
  /* K1   */  be_nested_str_weak(add_obj),
  /* K2   */  be_nested_str_weak(stop_iteration),
  /* K3   */  be_nested_str_weak(_X3C),
  /* K4   */  be_nested_str_weak(_X3A),
  /* K5   */  be_nested_str_weak(matter),
  /* K6   */  be_nested_str_weak(inspect),
  /* K7   */  be_nested_str_weak(_X3E),
  /* K8   */  be_nested_str_weak(push),
  /* K9   */  be_nested_str_weak(from_TLV),
};


extern const bclass be_class_Matter_IM_base;

/********************************************************************
** Solidified function: to_TLV_array
********************************************************************/
be_local_closure(class_Matter_IM_base_to_TLV_array,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_IM_base,     /* shared constants */
    be_str_weak(to_TLV_array),
    &be_const_str_solidified,
    ( &(const binstruction[23]) {  /* code */
      0x4C100000,  //  0000  LDNIL	R4
      0x1C100604,  //  0001  EQ	R4	R3	R4
      0x78120001,  //  0002  JMPF	R4	#0005
      0x4C100000,  //  0003  LDNIL	R4
      0x80040800,  //  0004  RET	1	R4
      0x8C100300,  //  0005  GETMET	R4	R1	K0
      0x5C180400,  //  0006  MOVE	R6	R2
      0x7C100400,  //  0007  CALL	R4	2
      0x60140010,  //  0008  GETGBL	R5	G16
      0x5C180600,  //  0009  MOVE	R6	R3
      0x7C140200,  //  000A  CALL	R5	1
      0xA8020006,  //  000B  EXBLK	0	#0013
      0x5C180A00,  //  000C  MOVE	R6	R5
      0x7C180000,  //  000D  CALL	R6	0
      0x8C1C0901,  //  000E  GETMET	R7	R4	K1
      0x4C240000,  //  000F  LDNIL	R9
      0x5C280C00,  //  0010  MOVE	R10	R6
      0x7C1C0600,  //  0011  CALL	R7	3
      0x7001FFF8,  //  0012  JMP		#000C
      0x58140002,  //  0013  LDCONST	R5	K2
      0xAC140200,  //  0014  CATCH	R5	1	0
      0xB0080000,  //  0015  RAISE	2	R0	R0
      0x80000000,  //  0016  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(class_Matter_IM_base_tostring,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_IM_base,     /* shared constants */
    be_str_weak(tostring),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x60040005,  //  0000  GETGBL	R1	G5
      0x5C080000,  //  0001  MOVE	R2	R0
      0x7C040200,  //  0002  CALL	R1	1
      0x00060601,  //  0003  ADD	R1	K3	R1
      0x00040304,  //  0004  ADD	R1	R1	K4
      0xB80A0A00,  //  0005  GETNGBL	R2	K5
      0x8C080506,  //  0006  GETMET	R2	R2	K6
      0x5C100000,  //  0007  MOVE	R4	R0
      0x7C080400,  //  0008  CALL	R2	2
      0x00040202,  //  0009  ADD	R1	R1	R2
      0x00040307,  //  000A  ADD	R1	R1	K7
      0x80040200,  //  000B  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: from_TLV_array
********************************************************************/
be_local_closure(class_Matter_IM_base_from_TLV_array,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_IM_base,     /* shared constants */
    be_str_weak(from_TLV_array),
    &be_const_str_solidified,
    ( &(const binstruction[25]) {  /* code */
      0x4C0C0000,  //  0000  LDNIL	R3
      0x1C0C0203,  //  0001  EQ	R3	R1	R3
      0x780E0001,  //  0002  JMPF	R3	#0005
      0x4C0C0000,  //  0003  LDNIL	R3
      0x80040600,  //  0004  RET	1	R3
      0x600C0012,  //  0005  GETGBL	R3	G18
      0x7C0C0000,  //  0006  CALL	R3	0
      0x60100010,  //  0007  GETGBL	R4	G16
      0x5C140200,  //  0008  MOVE	R5	R1
      0x7C100200,  //  0009  CALL	R4	1
      0xA8020009,  //  000A  EXBLK	0	#0015
      0x5C140800,  //  000B  MOVE	R5	R4
      0x7C140000,  //  000C  CALL	R5	0
      0x8C180708,  //  000D  GETMET	R6	R3	K8
      0x5C200400,  //  000E  MOVE	R8	R2
      0x7C200000,  //  000F  CALL	R8	0
      0x8C201109,  //  0010  GETMET	R8	R8	K9
      0x5C280A00,  //  0011  MOVE	R10	R5
      0x7C200400,  //  0012  CALL	R8	2
      0x7C180400,  //  0013  CALL	R6	2
      0x7001FFF5,  //  0014  JMP		#000B
      0x58100002,  //  0015  LDCONST	R4	K2
      0xAC100200,  //  0016  CATCH	R4	1	0
      0xB0080000,  //  0017  RAISE	2	R0	R0
      0x80040600,  //  0018  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_IM_base
********************************************************************/
be_local_class(Matter_IM_base,
    0,
    NULL,
    be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(from_TLV_array, -1), be_const_closure(class_Matter_IM_base_from_TLV_array_closure) },
        { be_const_key_weak(tostring, -1), be_const_closure(class_Matter_IM_base_tostring_closure) },
        { be_const_key_weak(to_TLV_array, 0), be_const_closure(class_Matter_IM_base_to_TLV_array_closure) },
    })),
    be_str_weak(Matter_IM_base)
);

extern const bclass be_class_Matter_IM_Message_base;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_Matter_IM_Message_base_init,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(InteractionModelRevision),
    /* K1   */  be_const_int(1),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x90020101,  //  0000  SETMBR	R0	K0	K1
      0x80000000,  //  0001  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_IM_Message_base
********************************************************************/
extern const bclass be_class_Matter_IM_base;
be_local_class(Matter_IM_Message_base,
    1,
    &be_class_Matter_IM_base,
    be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(init, -1), be_const_closure(class_Matter_IM_Message_base_init_closure) },
        { be_const_key_weak(InteractionModelRevision, 0), be_const_var(0) },
    })),
    be_str_weak(Matter_IM_Message_base)
);
// compact class 'Matter_AttributePathIB' ktab size: 29, total: 43 (saved 112 bytes)
static const bvalue be_ktab_class_Matter_AttributePathIB[29] = {
  /* K0   */  be_nested_str_weak(matter),
  /* K1   */  be_nested_str_weak(TLV),
  /* K2   */  be_nested_str_weak(Matter_TLV_list),
  /* K3   */  be_nested_str_weak(add_TLV),
  /* K4   */  be_const_int(0),
  /* K5   */  be_nested_str_weak(BOOL),
  /* K6   */  be_nested_str_weak(tag_compression),
  /* K7   */  be_const_int(1),
  /* K8   */  be_nested_str_weak(U8),
  /* K9   */  be_nested_str_weak(node),
  /* K10  */  be_const_int(2),
  /* K11  */  be_nested_str_weak(U2),
  /* K12  */  be_nested_str_weak(endpoint),
  /* K13  */  be_const_int(3),
  /* K14  */  be_nested_str_weak(U4),
  /* K15  */  be_nested_str_weak(cluster),
  /* K16  */  be_nested_str_weak(attribute),
  /* K17  */  be_nested_str_weak(list_index),
  /* K18  */  be_nested_str_weak(),
  /* K19  */  be_nested_str_weak(node_X3D_X25s_X20),
  /* K20  */  be_nested_str_weak(_X5B_X2502X_X5D),
  /* K21  */  be_nested_str_weak(_X5B_X2A_X2A_X5D),
  /* K22  */  be_nested_str_weak(_X2504X_X2F),
  /* K23  */  be_nested_str_weak(_X2A_X2A_X2A_X2A_X2F),
  /* K24  */  be_nested_str_weak(_X2504X),
  /* K25  */  be_nested_str_weak(_X2A_X2A_X2A_X2A),
  /* K26  */  be_nested_str_weak(Exception_X3E_X20),
  /* K27  */  be_nested_str_weak(_X2C_X20),
  /* K28  */  be_nested_str_weak(findsubval),
};


extern const bclass be_class_Matter_AttributePathIB;

/********************************************************************
** Solidified function: to_TLV
********************************************************************/
be_local_closure(class_Matter_AttributePathIB_to_TLV,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_AttributePathIB,     /* shared constants */
    be_str_weak(to_TLV),
    &be_const_str_solidified,
    ( &(const binstruction[35]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x88040301,  //  0001  GETMBR	R1	R1	K1
      0x8C080302,  //  0002  GETMET	R2	R1	K2
      0x7C080200,  //  0003  CALL	R2	1
      0x8C0C0503,  //  0004  GETMET	R3	R2	K3
      0x58140004,  //  0005  LDCONST	R5	K4
      0x88180305,  //  0006  GETMBR	R6	R1	K5
      0x881C0106,  //  0007  GETMBR	R7	R0	K6
      0x7C0C0800,  //  0008  CALL	R3	4
      0x8C0C0503,  //  0009  GETMET	R3	R2	K3
      0x58140007,  //  000A  LDCONST	R5	K7
      0x88180308,  //  000B  GETMBR	R6	R1	K8
      0x881C0109,  //  000C  GETMBR	R7	R0	K9
      0x7C0C0800,  //  000D  CALL	R3	4
      0x8C0C0503,  //  000E  GETMET	R3	R2	K3
      0x5814000A,  //  000F  LDCONST	R5	K10
      0x8818030B,  //  0010  GETMBR	R6	R1	K11
      0x881C010C,  //  0011  GETMBR	R7	R0	K12
      0x7C0C0800,  //  0012  CALL	R3	4
      0x8C0C0503,  //  0013  GETMET	R3	R2	K3
      0x5814000D,  //  0014  LDCONST	R5	K13
      0x8818030E,  //  0015  GETMBR	R6	R1	K14
      0x881C010F,  //  0016  GETMBR	R7	R0	K15
      0x7C0C0800,  //  0017  CALL	R3	4
      0x8C0C0503,  //  0018  GETMET	R3	R2	K3
      0x54160003,  //  0019  LDINT	R5	4
      0x8818030E,  //  001A  GETMBR	R6	R1	K14
      0x881C0110,  //  001B  GETMBR	R7	R0	K16
      0x7C0C0800,  //  001C  CALL	R3	4
      0x8C0C0503,  //  001D  GETMET	R3	R2	K3
      0x54160004,  //  001E  LDINT	R5	5
      0x8818030B,  //  001F  GETMBR	R6	R1	K11
      0x881C0111,  //  0020  GETMBR	R7	R0	K17
      0x7C0C0800,  //  0021  CALL	R3	4
      0x80040400,  //  0022  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(class_Matter_AttributePathIB_tostring,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_AttributePathIB,     /* shared constants */
    be_str_weak(tostring),
    &be_const_str_solidified,
    ( &(const binstruction[61]) {  /* code */
      0xA802002C,  //  0000  EXBLK	0	#002E
      0x58040012,  //  0001  LDCONST	R1	K18
      0x88080109,  //  0002  GETMBR	R2	R0	K9
      0x780A0004,  //  0003  JMPF	R2	#0009
      0x60080018,  //  0004  GETGBL	R2	G24
      0x580C0013,  //  0005  LDCONST	R3	K19
      0x88100109,  //  0006  GETMBR	R4	R0	K9
      0x7C080400,  //  0007  CALL	R2	2
      0x00040202,  //  0008  ADD	R1	R1	R2
      0x8808010C,  //  0009  GETMBR	R2	R0	K12
      0x4C0C0000,  //  000A  LDNIL	R3
      0x20080403,  //  000B  NE	R2	R2	R3
      0x780A0004,  //  000C  JMPF	R2	#0012
      0x60080018,  //  000D  GETGBL	R2	G24
      0x580C0014,  //  000E  LDCONST	R3	K20
      0x8810010C,  //  000F  GETMBR	R4	R0	K12
      0x7C080400,  //  0010  CALL	R2	2
      0x70020000,  //  0011  JMP		#0013
      0x58080015,  //  0012  LDCONST	R2	K21
      0x00040202,  //  0013  ADD	R1	R1	R2
      0x8808010F,  //  0014  GETMBR	R2	R0	K15
      0x4C0C0000,  //  0015  LDNIL	R3
      0x20080403,  //  0016  NE	R2	R2	R3
      0x780A0004,  //  0017  JMPF	R2	#001D
      0x60080018,  //  0018  GETGBL	R2	G24
      0x580C0016,  //  0019  LDCONST	R3	K22
      0x8810010F,  //  001A  GETMBR	R4	R0	K15
      0x7C080400,  //  001B  CALL	R2	2
      0x70020000,  //  001C  JMP		#001E
      0x58080017,  //  001D  LDCONST	R2	K23
      0x00040202,  //  001E  ADD	R1	R1	R2
      0x88080110,  //  001F  GETMBR	R2	R0	K16
      0x4C0C0000,  //  0020  LDNIL	R3
      0x20080403,  //  0021  NE	R2	R2	R3
      0x780A0004,  //  0022  JMPF	R2	#0028
      0x60080018,  //  0023  GETGBL	R2	G24
      0x580C0018,  //  0024  LDCONST	R3	K24
      0x88100110,  //  0025  GETMBR	R4	R0	K16
      0x7C080400,  //  0026  CALL	R2	2
      0x70020000,  //  0027  JMP		#0029
      0x58080019,  //  0028  LDCONST	R2	K25
      0x00040202,  //  0029  ADD	R1	R1	R2
      0xA8040001,  //  002A  EXBLK	1	1
      0x80040200,  //  002B  RET	1	R1
      0xA8040001,  //  002C  EXBLK	1	1
      0x7002000D,  //  002D  JMP		#003C
      0xAC040002,  //  002E  CATCH	R1	0	2
      0x7002000A,  //  002F  JMP		#003B
      0x600C0008,  //  0030  GETGBL	R3	G8
      0x5C100200,  //  0031  MOVE	R4	R1
      0x7C0C0200,  //  0032  CALL	R3	1
      0x000E3403,  //  0033  ADD	R3	K26	R3
      0x000C071B,  //  0034  ADD	R3	R3	K27
      0x60100008,  //  0035  GETGBL	R4	G8
      0x5C140400,  //  0036  MOVE	R5	R2
      0x7C100200,  //  0037  CALL	R4	1
      0x000C0604,  //  0038  ADD	R3	R3	R4
      0x80040600,  //  0039  RET	1	R3
      0x70020000,  //  003A  JMP		#003C
      0xB0080000,  //  003B  RAISE	2	R0	R0
      0x80000000,  //  003C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: from_TLV
********************************************************************/
be_local_closure(class_Matter_AttributePathIB_from_TLV,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_AttributePathIB,     /* shared constants */
    be_str_weak(from_TLV),
    &be_const_str_solidified,
    ( &(const binstruction[30]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0001,  //  0002  JMPF	R2	#0005
      0x4C080000,  //  0003  LDNIL	R2
      0x80040400,  //  0004  RET	1	R2
      0x8C08031C,  //  0005  GETMET	R2	R1	K28
      0x58100004,  //  0006  LDCONST	R4	K4
      0x7C080400,  //  0007  CALL	R2	2
      0x90020C02,  //  0008  SETMBR	R0	K6	R2
      0x8C08031C,  //  0009  GETMET	R2	R1	K28
      0x58100007,  //  000A  LDCONST	R4	K7
      0x7C080400,  //  000B  CALL	R2	2
      0x90021202,  //  000C  SETMBR	R0	K9	R2
      0x8C08031C,  //  000D  GETMET	R2	R1	K28
      0x5810000A,  //  000E  LDCONST	R4	K10
      0x7C080400,  //  000F  CALL	R2	2
      0x90021802,  //  0010  SETMBR	R0	K12	R2
      0x8C08031C,  //  0011  GETMET	R2	R1	K28
      0x5810000D,  //  0012  LDCONST	R4	K13
      0x7C080400,  //  0013  CALL	R2	2
      0x90021E02,  //  0014  SETMBR	R0	K15	R2
      0x8C08031C,  //  0015  GETMET	R2	R1	K28
      0x54120003,  //  0016  LDINT	R4	4
      0x7C080400,  //  0017  CALL	R2	2
      0x90022002,  //  0018  SETMBR	R0	K16	R2
      0x8C08031C,  //  0019  GETMET	R2	R1	K28
      0x54120004,  //  001A  LDINT	R4	5
      0x7C080400,  //  001B  CALL	R2	2
      0x90022202,  //  001C  SETMBR	R0	K17	R2
      0x80040000,  //  001D  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_AttributePathIB
********************************************************************/
extern const bclass be_class_Matter_IM_base;
be_local_class(Matter_AttributePathIB,
    6,
    &be_class_Matter_IM_base,
    be_nested_map(9,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(tostring, -1), be_const_closure(class_Matter_AttributePathIB_tostring_closure) },
        { be_const_key_weak(tag_compression, 0), be_const_var(0) },
        { be_const_key_weak(cluster, -1), be_const_var(3) },
        { be_const_key_weak(attribute, -1), be_const_var(4) },
        { be_const_key_weak(to_TLV, 2), be_const_closure(class_Matter_AttributePathIB_to_TLV_closure) },
        { be_const_key_weak(list_index, 3), be_const_var(5) },
        { be_const_key_weak(from_TLV, -1), be_const_closure(class_Matter_AttributePathIB_from_TLV_closure) },
        { be_const_key_weak(endpoint, 1), be_const_var(2) },
        { be_const_key_weak(node, -1), be_const_var(1) },
    })),
    be_str_weak(Matter_AttributePathIB)
);
// compact class 'Matter_ClusterPathIB' ktab size: 14, total: 20 (saved 48 bytes)
static const bvalue be_ktab_class_Matter_ClusterPathIB[14] = {
  /* K0   */  be_nested_str_weak(node),
  /* K1   */  be_nested_str_weak(findsubval),
  /* K2   */  be_const_int(0),
  /* K3   */  be_nested_str_weak(endpoint),
  /* K4   */  be_const_int(1),
  /* K5   */  be_nested_str_weak(cluster),
  /* K6   */  be_const_int(2),
  /* K7   */  be_nested_str_weak(matter),
  /* K8   */  be_nested_str_weak(TLV),
  /* K9   */  be_nested_str_weak(Matter_TLV_list),
  /* K10  */  be_nested_str_weak(add_TLV),
  /* K11  */  be_nested_str_weak(U8),
  /* K12  */  be_nested_str_weak(U2),
  /* K13  */  be_nested_str_weak(U4),
};


extern const bclass be_class_Matter_ClusterPathIB;

/********************************************************************
** Solidified function: from_TLV
********************************************************************/
be_local_closure(class_Matter_ClusterPathIB_from_TLV,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_ClusterPathIB,     /* shared constants */
    be_str_weak(from_TLV),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0001,  //  0002  JMPF	R2	#0005
      0x4C080000,  //  0003  LDNIL	R2
      0x80040400,  //  0004  RET	1	R2
      0x8C080301,  //  0005  GETMET	R2	R1	K1
      0x58100002,  //  0006  LDCONST	R4	K2
      0x7C080400,  //  0007  CALL	R2	2
      0x90020002,  //  0008  SETMBR	R0	K0	R2
      0x8C080301,  //  0009  GETMET	R2	R1	K1
      0x58100004,  //  000A  LDCONST	R4	K4
      0x7C080400,  //  000B  CALL	R2	2
      0x90020602,  //  000C  SETMBR	R0	K3	R2
      0x8C080301,  //  000D  GETMET	R2	R1	K1
      0x58100006,  //  000E  LDCONST	R4	K6
      0x7C080400,  //  000F  CALL	R2	2
      0x90020A02,  //  0010  SETMBR	R0	K5	R2
      0x80040000,  //  0011  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: to_TLV
********************************************************************/
be_local_closure(class_Matter_ClusterPathIB_to_TLV,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_ClusterPathIB,     /* shared constants */
    be_str_weak(to_TLV),
    &be_const_str_solidified,
    ( &(const binstruction[20]) {  /* code */
      0xB8060E00,  //  0000  GETNGBL	R1	K7
      0x88040308,  //  0001  GETMBR	R1	R1	K8
      0x8C080309,  //  0002  GETMET	R2	R1	K9
      0x7C080200,  //  0003  CALL	R2	1
      0x8C0C050A,  //  0004  GETMET	R3	R2	K10
      0x58140002,  //  0005  LDCONST	R5	K2
      0x8818030B,  //  0006  GETMBR	R6	R1	K11
      0x881C0100,  //  0007  GETMBR	R7	R0	K0
      0x7C0C0800,  //  0008  CALL	R3	4
      0x8C0C050A,  //  0009  GETMET	R3	R2	K10
      0x58140004,  //  000A  LDCONST	R5	K4
      0x8818030C,  //  000B  GETMBR	R6	R1	K12
      0x881C0103,  //  000C  GETMBR	R7	R0	K3
      0x7C0C0800,  //  000D  CALL	R3	4
      0x8C0C050A,  //  000E  GETMET	R3	R2	K10
      0x58140006,  //  000F  LDCONST	R5	K6
      0x8818030D,  //  0010  GETMBR	R6	R1	K13
      0x881C0105,  //  0011  GETMBR	R7	R0	K5
      0x7C0C0800,  //  0012  CALL	R3	4
      0x80040400,  //  0013  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_ClusterPathIB
********************************************************************/
extern const bclass be_class_Matter_IM_base;
be_local_class(Matter_ClusterPathIB,
    3,
    &be_class_Matter_IM_base,
    be_nested_map(5,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(from_TLV, -1), be_const_closure(class_Matter_ClusterPathIB_from_TLV_closure) },
        { be_const_key_weak(node, -1), be_const_var(0) },
        { be_const_key_weak(cluster, -1), be_const_var(2) },
        { be_const_key_weak(endpoint, 0), be_const_var(1) },
        { be_const_key_weak(to_TLV, -1), be_const_closure(class_Matter_ClusterPathIB_to_TLV_closure) },
    })),
    be_str_weak(Matter_ClusterPathIB)
);
// compact class 'Matter_DataVersionFilterIB' ktab size: 14, total: 19 (saved 40 bytes)
static const bvalue be_ktab_class_Matter_DataVersionFilterIB[14] = {
  /* K0   */  be_nested_str_weak(path),
  /* K1   */  be_nested_str_weak(matter),
  /* K2   */  be_nested_str_weak(ClusterPathIB),
  /* K3   */  be_nested_str_weak(from_TLV),
  /* K4   */  be_nested_str_weak(findsub),
  /* K5   */  be_const_int(0),
  /* K6   */  be_nested_str_weak(data_version),
  /* K7   */  be_nested_str_weak(findsubval),
  /* K8   */  be_const_int(1),
  /* K9   */  be_nested_str_weak(TLV),
  /* K10  */  be_nested_str_weak(Matter_TLV_struct),
  /* K11  */  be_nested_str_weak(add_obj),
  /* K12  */  be_nested_str_weak(add_TLV),
  /* K13  */  be_nested_str_weak(U4),
};


extern const bclass be_class_Matter_DataVersionFilterIB;

/********************************************************************
** Solidified function: from_TLV
********************************************************************/
be_local_closure(class_Matter_DataVersionFilterIB_from_TLV,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_DataVersionFilterIB,     /* shared constants */
    be_str_weak(from_TLV),
    &be_const_str_solidified,
    ( &(const binstruction[19]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0001,  //  0002  JMPF	R2	#0005
      0x4C080000,  //  0003  LDNIL	R2
      0x80040400,  //  0004  RET	1	R2
      0xB80A0200,  //  0005  GETNGBL	R2	K1
      0x8C080502,  //  0006  GETMET	R2	R2	K2
      0x7C080200,  //  0007  CALL	R2	1
      0x8C080503,  //  0008  GETMET	R2	R2	K3
      0x8C100304,  //  0009  GETMET	R4	R1	K4
      0x58180005,  //  000A  LDCONST	R6	K5
      0x7C100400,  //  000B  CALL	R4	2
      0x7C080400,  //  000C  CALL	R2	2
      0x90020002,  //  000D  SETMBR	R0	K0	R2
      0x8C080307,  //  000E  GETMET	R2	R1	K7
      0x58100008,  //  000F  LDCONST	R4	K8
      0x7C080400,  //  0010  CALL	R2	2
      0x90020C02,  //  0011  SETMBR	R0	K6	R2
      0x80040000,  //  0012  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: to_TLV
********************************************************************/
be_local_closure(class_Matter_DataVersionFilterIB_to_TLV,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_DataVersionFilterIB,     /* shared constants */
    be_str_weak(to_TLV),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0xB8060200,  //  0000  GETNGBL	R1	K1
      0x88040309,  //  0001  GETMBR	R1	R1	K9
      0x8C08030A,  //  0002  GETMET	R2	R1	K10
      0x7C080200,  //  0003  CALL	R2	1
      0x8C0C050B,  //  0004  GETMET	R3	R2	K11
      0x58140005,  //  0005  LDCONST	R5	K5
      0x88180100,  //  0006  GETMBR	R6	R0	K0
      0x7C0C0600,  //  0007  CALL	R3	3
      0x8C0C050C,  //  0008  GETMET	R3	R2	K12
      0x58140008,  //  0009  LDCONST	R5	K8
      0x8818030D,  //  000A  GETMBR	R6	R1	K13
      0x881C0106,  //  000B  GETMBR	R7	R0	K6
      0x7C0C0800,  //  000C  CALL	R3	4
      0x80040400,  //  000D  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_DataVersionFilterIB
********************************************************************/
extern const bclass be_class_Matter_IM_base;
be_local_class(Matter_DataVersionFilterIB,
    2,
    &be_class_Matter_IM_base,
    be_nested_map(4,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(from_TLV, -1), be_const_closure(class_Matter_DataVersionFilterIB_from_TLV_closure) },
        { be_const_key_weak(to_TLV, -1), be_const_closure(class_Matter_DataVersionFilterIB_to_TLV_closure) },
        { be_const_key_weak(path, 3), be_const_var(0) },
        { be_const_key_weak(data_version, -1), be_const_var(1) },
    })),
    be_str_weak(Matter_DataVersionFilterIB)
);
// compact class 'Matter_AttributeDataIB' ktab size: 16, total: 23 (saved 56 bytes)
static const bvalue be_ktab_class_Matter_AttributeDataIB[16] = {
  /* K0   */  be_nested_str_weak(data_version),
  /* K1   */  be_nested_str_weak(findsubval),
  /* K2   */  be_const_int(0),
  /* K3   */  be_nested_str_weak(path),
  /* K4   */  be_nested_str_weak(matter),
  /* K5   */  be_nested_str_weak(AttributePathIB),
  /* K6   */  be_nested_str_weak(from_TLV),
  /* K7   */  be_nested_str_weak(findsub),
  /* K8   */  be_const_int(1),
  /* K9   */  be_nested_str_weak(data),
  /* K10  */  be_const_int(2),
  /* K11  */  be_nested_str_weak(TLV),
  /* K12  */  be_nested_str_weak(Matter_TLV_struct),
  /* K13  */  be_nested_str_weak(add_TLV),
  /* K14  */  be_nested_str_weak(U4),
  /* K15  */  be_nested_str_weak(add_obj),
};


extern const bclass be_class_Matter_AttributeDataIB;

/********************************************************************
** Solidified function: from_TLV
********************************************************************/
be_local_closure(class_Matter_AttributeDataIB_from_TLV,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_AttributeDataIB,     /* shared constants */
    be_str_weak(from_TLV),
    &be_const_str_solidified,
    ( &(const binstruction[23]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0001,  //  0002  JMPF	R2	#0005
      0x4C080000,  //  0003  LDNIL	R2
      0x80040400,  //  0004  RET	1	R2
      0x8C080301,  //  0005  GETMET	R2	R1	K1
      0x58100002,  //  0006  LDCONST	R4	K2
      0x7C080400,  //  0007  CALL	R2	2
      0x90020002,  //  0008  SETMBR	R0	K0	R2
      0xB80A0800,  //  0009  GETNGBL	R2	K4
      0x8C080505,  //  000A  GETMET	R2	R2	K5
      0x7C080200,  //  000B  CALL	R2	1
      0x8C080506,  //  000C  GETMET	R2	R2	K6
      0x8C100307,  //  000D  GETMET	R4	R1	K7
      0x58180008,  //  000E  LDCONST	R6	K8
      0x7C100400,  //  000F  CALL	R4	2
      0x7C080400,  //  0010  CALL	R2	2
      0x90020602,  //  0011  SETMBR	R0	K3	R2
      0x8C080301,  //  0012  GETMET	R2	R1	K1
      0x5810000A,  //  0013  LDCONST	R4	K10
      0x7C080400,  //  0014  CALL	R2	2
      0x90021202,  //  0015  SETMBR	R0	K9	R2
      0x80040000,  //  0016  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: to_TLV
********************************************************************/
be_local_closure(class_Matter_AttributeDataIB_to_TLV,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_AttributeDataIB,     /* shared constants */
    be_str_weak(to_TLV),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0xB8060800,  //  0000  GETNGBL	R1	K4
      0x8804030B,  //  0001  GETMBR	R1	R1	K11
      0x8C08030C,  //  0002  GETMET	R2	R1	K12
      0x7C080200,  //  0003  CALL	R2	1
      0x8C0C050D,  //  0004  GETMET	R3	R2	K13
      0x58140002,  //  0005  LDCONST	R5	K2
      0x8818030E,  //  0006  GETMBR	R6	R1	K14
      0x881C0100,  //  0007  GETMBR	R7	R0	K0
      0x7C0C0800,  //  0008  CALL	R3	4
      0x8C0C050F,  //  0009  GETMET	R3	R2	K15
      0x58140008,  //  000A  LDCONST	R5	K8
      0x88180103,  //  000B  GETMBR	R6	R0	K3
      0x7C0C0600,  //  000C  CALL	R3	3
      0x8C0C050F,  //  000D  GETMET	R3	R2	K15
      0x5814000A,  //  000E  LDCONST	R5	K10
      0x88180109,  //  000F  GETMBR	R6	R0	K9
      0x7C0C0600,  //  0010  CALL	R3	3
      0x80040400,  //  0011  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_AttributeDataIB
********************************************************************/
extern const bclass be_class_Matter_IM_base;
be_local_class(Matter_AttributeDataIB,
    3,
    &be_class_Matter_IM_base,
    be_nested_map(5,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(data_version, -1), be_const_var(0) },
        { be_const_key_weak(data, -1), be_const_var(2) },
        { be_const_key_weak(from_TLV, -1), be_const_closure(class_Matter_AttributeDataIB_from_TLV_closure) },
        { be_const_key_weak(path, 2), be_const_var(1) },
        { be_const_key_weak(to_TLV, -1), be_const_closure(class_Matter_AttributeDataIB_to_TLV_closure) },
    })),
    be_str_weak(Matter_AttributeDataIB)
);
// compact class 'Matter_AttributeReportIB' ktab size: 12, total: 17 (saved 40 bytes)
static const bvalue be_ktab_class_Matter_AttributeReportIB[12] = {
  /* K0   */  be_nested_str_weak(matter),
  /* K1   */  be_nested_str_weak(TLV),
  /* K2   */  be_nested_str_weak(Matter_TLV_struct),
  /* K3   */  be_nested_str_weak(add_obj),
  /* K4   */  be_const_int(0),
  /* K5   */  be_nested_str_weak(attribute_status),
  /* K6   */  be_const_int(1),
  /* K7   */  be_nested_str_weak(attribute_data),
  /* K8   */  be_nested_str_weak(AttributeStatusIB),
  /* K9   */  be_nested_str_weak(from_TLV),
  /* K10  */  be_nested_str_weak(findsub),
  /* K11  */  be_nested_str_weak(AttributeDataIB),
};


extern const bclass be_class_Matter_AttributeReportIB;

/********************************************************************
** Solidified function: to_TLV
********************************************************************/
be_local_closure(class_Matter_AttributeReportIB_to_TLV,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_AttributeReportIB,     /* shared constants */
    be_str_weak(to_TLV),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x88040301,  //  0001  GETMBR	R1	R1	K1
      0x8C080302,  //  0002  GETMET	R2	R1	K2
      0x7C080200,  //  0003  CALL	R2	1
      0x8C0C0503,  //  0004  GETMET	R3	R2	K3
      0x58140004,  //  0005  LDCONST	R5	K4
      0x88180105,  //  0006  GETMBR	R6	R0	K5
      0x7C0C0600,  //  0007  CALL	R3	3
      0x8C0C0503,  //  0008  GETMET	R3	R2	K3
      0x58140006,  //  0009  LDCONST	R5	K6
      0x88180107,  //  000A  GETMBR	R6	R0	K7
      0x7C0C0600,  //  000B  CALL	R3	3
      0x80040400,  //  000C  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: from_TLV
********************************************************************/
be_local_closure(class_Matter_AttributeReportIB_from_TLV,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_AttributeReportIB,     /* shared constants */
    be_str_weak(from_TLV),
    &be_const_str_solidified,
    ( &(const binstruction[24]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0001,  //  0002  JMPF	R2	#0005
      0x4C080000,  //  0003  LDNIL	R2
      0x80040400,  //  0004  RET	1	R2
      0xB80A0000,  //  0005  GETNGBL	R2	K0
      0x8C080508,  //  0006  GETMET	R2	R2	K8
      0x7C080200,  //  0007  CALL	R2	1
      0x8C080509,  //  0008  GETMET	R2	R2	K9
      0x8C10030A,  //  0009  GETMET	R4	R1	K10
      0x58180004,  //  000A  LDCONST	R6	K4
      0x7C100400,  //  000B  CALL	R4	2
      0x7C080400,  //  000C  CALL	R2	2
      0x90020A02,  //  000D  SETMBR	R0	K5	R2
      0xB80A0000,  //  000E  GETNGBL	R2	K0
      0x8C08050B,  //  000F  GETMET	R2	R2	K11
      0x7C080200,  //  0010  CALL	R2	1
      0x8C080509,  //  0011  GETMET	R2	R2	K9
      0x8C10030A,  //  0012  GETMET	R4	R1	K10
      0x58180006,  //  0013  LDCONST	R6	K6
      0x7C100400,  //  0014  CALL	R4	2
      0x7C080400,  //  0015  CALL	R2	2
      0x90020E02,  //  0016  SETMBR	R0	K7	R2
      0x80040000,  //  0017  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_AttributeReportIB
********************************************************************/
extern const bclass be_class_Matter_IM_base;
be_local_class(Matter_AttributeReportIB,
    2,
    &be_class_Matter_IM_base,
    be_nested_map(4,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(attribute_data, 3), be_const_var(1) },
        { be_const_key_weak(to_TLV, -1), be_const_closure(class_Matter_AttributeReportIB_to_TLV_closure) },
        { be_const_key_weak(attribute_status, -1), be_const_var(0) },
        { be_const_key_weak(from_TLV, -1), be_const_closure(class_Matter_AttributeReportIB_from_TLV_closure) },
    })),
    be_str_weak(Matter_AttributeReportIB)
);
// compact class 'Matter_EventFilterIB' ktab size: 10, total: 14 (saved 32 bytes)
static const bvalue be_ktab_class_Matter_EventFilterIB[10] = {
  /* K0   */  be_nested_str_weak(node),
  /* K1   */  be_nested_str_weak(findsubval),
  /* K2   */  be_const_int(0),
  /* K3   */  be_nested_str_weak(event_min),
  /* K4   */  be_const_int(1),
  /* K5   */  be_nested_str_weak(matter),
  /* K6   */  be_nested_str_weak(TLV),
  /* K7   */  be_nested_str_weak(Matter_TLV_struct),
  /* K8   */  be_nested_str_weak(add_TLV),
  /* K9   */  be_nested_str_weak(U8),
};


extern const bclass be_class_Matter_EventFilterIB;

/********************************************************************
** Solidified function: from_TLV
********************************************************************/
be_local_closure(class_Matter_EventFilterIB_from_TLV,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_EventFilterIB,     /* shared constants */
    be_str_weak(from_TLV),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0001,  //  0002  JMPF	R2	#0005
      0x4C080000,  //  0003  LDNIL	R2
      0x80040400,  //  0004  RET	1	R2
      0x8C080301,  //  0005  GETMET	R2	R1	K1
      0x58100002,  //  0006  LDCONST	R4	K2
      0x7C080400,  //  0007  CALL	R2	2
      0x90020002,  //  0008  SETMBR	R0	K0	R2
      0x8C080301,  //  0009  GETMET	R2	R1	K1
      0x58100004,  //  000A  LDCONST	R4	K4
      0x7C080400,  //  000B  CALL	R2	2
      0x90020602,  //  000C  SETMBR	R0	K3	R2
      0x80040000,  //  000D  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: to_TLV
********************************************************************/
be_local_closure(class_Matter_EventFilterIB_to_TLV,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_EventFilterIB,     /* shared constants */
    be_str_weak(to_TLV),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0xB8060A00,  //  0000  GETNGBL	R1	K5
      0x88040306,  //  0001  GETMBR	R1	R1	K6
      0x8C080307,  //  0002  GETMET	R2	R1	K7
      0x7C080200,  //  0003  CALL	R2	1
      0x8C0C0508,  //  0004  GETMET	R3	R2	K8
      0x58140002,  //  0005  LDCONST	R5	K2
      0x88180309,  //  0006  GETMBR	R6	R1	K9
      0x881C0100,  //  0007  GETMBR	R7	R0	K0
      0x7C0C0800,  //  0008  CALL	R3	4
      0x8C0C0508,  //  0009  GETMET	R3	R2	K8
      0x58140004,  //  000A  LDCONST	R5	K4
      0x88180309,  //  000B  GETMBR	R6	R1	K9
      0x881C0103,  //  000C  GETMBR	R7	R0	K3
      0x7C0C0800,  //  000D  CALL	R3	4
      0x80040400,  //  000E  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_EventFilterIB
********************************************************************/
extern const bclass be_class_Matter_IM_base;
be_local_class(Matter_EventFilterIB,
    2,
    &be_class_Matter_IM_base,
    be_nested_map(4,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(event_min, 3), be_const_var(1) },
        { be_const_key_weak(node, 2), be_const_var(0) },
        { be_const_key_weak(to_TLV, -1), be_const_closure(class_Matter_EventFilterIB_to_TLV_closure) },
        { be_const_key_weak(from_TLV, -1), be_const_closure(class_Matter_EventFilterIB_from_TLV_closure) },
    })),
    be_str_weak(Matter_EventFilterIB)
);
// compact class 'Matter_EventPathIB' ktab size: 18, total: 27 (saved 72 bytes)
static const bvalue be_ktab_class_Matter_EventPathIB[18] = {
  /* K0   */  be_nested_str_weak(node),
  /* K1   */  be_nested_str_weak(findsubval),
  /* K2   */  be_const_int(0),
  /* K3   */  be_nested_str_weak(endpoint),
  /* K4   */  be_const_int(1),
  /* K5   */  be_nested_str_weak(cluster),
  /* K6   */  be_const_int(2),
  /* K7   */  be_nested_str_weak(event),
  /* K8   */  be_const_int(3),
  /* K9   */  be_nested_str_weak(is_urgent),
  /* K10  */  be_nested_str_weak(matter),
  /* K11  */  be_nested_str_weak(TLV),
  /* K12  */  be_nested_str_weak(Matter_TLV_list),
  /* K13  */  be_nested_str_weak(add_TLV),
  /* K14  */  be_nested_str_weak(U8),
  /* K15  */  be_nested_str_weak(U2),
  /* K16  */  be_nested_str_weak(U4),
  /* K17  */  be_nested_str_weak(BOOL),
};


extern const bclass be_class_Matter_EventPathIB;

/********************************************************************
** Solidified function: from_TLV
********************************************************************/
be_local_closure(class_Matter_EventPathIB_from_TLV,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_EventPathIB,     /* shared constants */
    be_str_weak(from_TLV),
    &be_const_str_solidified,
    ( &(const binstruction[26]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0001,  //  0002  JMPF	R2	#0005
      0x4C080000,  //  0003  LDNIL	R2
      0x80040400,  //  0004  RET	1	R2
      0x8C080301,  //  0005  GETMET	R2	R1	K1
      0x58100002,  //  0006  LDCONST	R4	K2
      0x7C080400,  //  0007  CALL	R2	2
      0x90020002,  //  0008  SETMBR	R0	K0	R2
      0x8C080301,  //  0009  GETMET	R2	R1	K1
      0x58100004,  //  000A  LDCONST	R4	K4
      0x7C080400,  //  000B  CALL	R2	2
      0x90020602,  //  000C  SETMBR	R0	K3	R2
      0x8C080301,  //  000D  GETMET	R2	R1	K1
      0x58100006,  //  000E  LDCONST	R4	K6
      0x7C080400,  //  000F  CALL	R2	2
      0x90020A02,  //  0010  SETMBR	R0	K5	R2
      0x8C080301,  //  0011  GETMET	R2	R1	K1
      0x58100008,  //  0012  LDCONST	R4	K8
      0x7C080400,  //  0013  CALL	R2	2
      0x90020E02,  //  0014  SETMBR	R0	K7	R2
      0x8C080301,  //  0015  GETMET	R2	R1	K1
      0x54120003,  //  0016  LDINT	R4	4
      0x7C080400,  //  0017  CALL	R2	2
      0x90021202,  //  0018  SETMBR	R0	K9	R2
      0x80040000,  //  0019  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: to_TLV
********************************************************************/
be_local_closure(class_Matter_EventPathIB_to_TLV,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_EventPathIB,     /* shared constants */
    be_str_weak(to_TLV),
    &be_const_str_solidified,
    ( &(const binstruction[34]) {  /* code */
      0xB80A1400,  //  0000  GETNGBL	R2	K10
      0x8808050B,  //  0001  GETMBR	R2	R2	K11
      0x4C0C0000,  //  0002  LDNIL	R3
      0x1C0C0203,  //  0003  EQ	R3	R1	R3
      0x780E0002,  //  0004  JMPF	R3	#0008
      0x8C0C050C,  //  0005  GETMET	R3	R2	K12
      0x7C0C0200,  //  0006  CALL	R3	1
      0x5C040600,  //  0007  MOVE	R1	R3
      0x8C0C030D,  //  0008  GETMET	R3	R1	K13
      0x58140002,  //  0009  LDCONST	R5	K2
      0x8818050E,  //  000A  GETMBR	R6	R2	K14
      0x881C0100,  //  000B  GETMBR	R7	R0	K0
      0x7C0C0800,  //  000C  CALL	R3	4
      0x8C0C030D,  //  000D  GETMET	R3	R1	K13
      0x58140004,  //  000E  LDCONST	R5	K4
      0x8818050F,  //  000F  GETMBR	R6	R2	K15
      0x881C0103,  //  0010  GETMBR	R7	R0	K3
      0x7C0C0800,  //  0011  CALL	R3	4
      0x8C0C030D,  //  0012  GETMET	R3	R1	K13
      0x58140006,  //  0013  LDCONST	R5	K6
      0x88180510,  //  0014  GETMBR	R6	R2	K16
      0x881C0105,  //  0015  GETMBR	R7	R0	K5
      0x7C0C0800,  //  0016  CALL	R3	4
      0x8C0C030D,  //  0017  GETMET	R3	R1	K13
      0x58140008,  //  0018  LDCONST	R5	K8
      0x88180510,  //  0019  GETMBR	R6	R2	K16
      0x881C0107,  //  001A  GETMBR	R7	R0	K7
      0x7C0C0800,  //  001B  CALL	R3	4
      0x8C0C030D,  //  001C  GETMET	R3	R1	K13
      0x54160003,  //  001D  LDINT	R5	4
      0x88180511,  //  001E  GETMBR	R6	R2	K17
      0x881C0109,  //  001F  GETMBR	R7	R0	K9
      0x7C0C0800,  //  0020  CALL	R3	4
      0x80040200,  //  0021  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_EventPathIB
********************************************************************/
extern const bclass be_class_Matter_IM_base;
be_local_class(Matter_EventPathIB,
    5,
    &be_class_Matter_IM_base,
    be_nested_map(7,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(endpoint, 5), be_const_var(1) },
        { be_const_key_weak(from_TLV, -1), be_const_closure(class_Matter_EventPathIB_from_TLV_closure) },
        { be_const_key_weak(to_TLV, -1), be_const_closure(class_Matter_EventPathIB_to_TLV_closure) },
        { be_const_key_weak(is_urgent, -1), be_const_var(4) },
        { be_const_key_weak(event, -1), be_const_var(3) },
        { be_const_key_weak(cluster, 3), be_const_var(2) },
        { be_const_key_weak(node, -1), be_const_var(0) },
    })),
    be_str_weak(Matter_EventPathIB)
);
// compact class 'Matter_EventDataIB' ktab size: 25, total: 38 (saved 104 bytes)
static const bvalue be_ktab_class_Matter_EventDataIB[25] = {
  /* K0   */  be_nested_str_weak(path),
  /* K1   */  be_nested_str_weak(matter),
  /* K2   */  be_nested_str_weak(EventPathIB),
  /* K3   */  be_nested_str_weak(from_TLV),
  /* K4   */  be_nested_str_weak(findsub),
  /* K5   */  be_const_int(0),
  /* K6   */  be_nested_str_weak(event_number),
  /* K7   */  be_nested_str_weak(findsubval),
  /* K8   */  be_const_int(1),
  /* K9   */  be_nested_str_weak(priority),
  /* K10  */  be_const_int(2),
  /* K11  */  be_nested_str_weak(epoch_timestamp),
  /* K12  */  be_const_int(3),
  /* K13  */  be_nested_str_weak(system_timestamp),
  /* K14  */  be_nested_str_weak(delta_epoch_timestamp),
  /* K15  */  be_nested_str_weak(delta_system_timestamp),
  /* K16  */  be_nested_str_weak(data),
  /* K17  */  be_nested_str_weak(TLV),
  /* K18  */  be_nested_str_weak(Matter_TLV_struct),
  /* K19  */  be_nested_str_weak(to_TLV),
  /* K20  */  be_nested_str_weak(add_list),
  /* K21  */  be_nested_str_weak(add_TLV),
  /* K22  */  be_nested_str_weak(U8),
  /* K23  */  be_nested_str_weak(U1),
  /* K24  */  be_nested_str_weak(add_obj),
};


extern const bclass be_class_Matter_EventDataIB;

/********************************************************************
** Solidified function: from_TLV
********************************************************************/
be_local_closure(class_Matter_EventDataIB_from_TLV,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_EventDataIB,     /* shared constants */
    be_str_weak(from_TLV),
    &be_const_str_solidified,
    ( &(const binstruction[43]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0001,  //  0002  JMPF	R2	#0005
      0x4C080000,  //  0003  LDNIL	R2
      0x80040400,  //  0004  RET	1	R2
      0xB80A0200,  //  0005  GETNGBL	R2	K1
      0x8C080502,  //  0006  GETMET	R2	R2	K2
      0x7C080200,  //  0007  CALL	R2	1
      0x8C080503,  //  0008  GETMET	R2	R2	K3
      0x8C100304,  //  0009  GETMET	R4	R1	K4
      0x58180005,  //  000A  LDCONST	R6	K5
      0x7C100400,  //  000B  CALL	R4	2
      0x7C080400,  //  000C  CALL	R2	2
      0x90020002,  //  000D  SETMBR	R0	K0	R2
      0x8C080307,  //  000E  GETMET	R2	R1	K7
      0x58100008,  //  000F  LDCONST	R4	K8
      0x7C080400,  //  0010  CALL	R2	2
      0x90020C02,  //  0011  SETMBR	R0	K6	R2
      0x8C080307,  //  0012  GETMET	R2	R1	K7
      0x5810000A,  //  0013  LDCONST	R4	K10
      0x7C080400,  //  0014  CALL	R2	2
      0x90021202,  //  0015  SETMBR	R0	K9	R2
      0x8C080307,  //  0016  GETMET	R2	R1	K7
      0x5810000C,  //  0017  LDCONST	R4	K12
      0x7C080400,  //  0018  CALL	R2	2
      0x90021602,  //  0019  SETMBR	R0	K11	R2
      0x8C080307,  //  001A  GETMET	R2	R1	K7
      0x54120003,  //  001B  LDINT	R4	4
      0x7C080400,  //  001C  CALL	R2	2
      0x90021A02,  //  001D  SETMBR	R0	K13	R2
      0x8C080307,  //  001E  GETMET	R2	R1	K7
      0x54120004,  //  001F  LDINT	R4	5
      0x7C080400,  //  0020  CALL	R2	2
      0x90021C02,  //  0021  SETMBR	R0	K14	R2
      0x8C080307,  //  0022  GETMET	R2	R1	K7
      0x54120005,  //  0023  LDINT	R4	6
      0x7C080400,  //  0024  CALL	R2	2
      0x90021E02,  //  0025  SETMBR	R0	K15	R2
      0x8C080307,  //  0026  GETMET	R2	R1	K7
      0x54120006,  //  0027  LDINT	R4	7
      0x7C080400,  //  0028  CALL	R2	2
      0x90022002,  //  0029  SETMBR	R0	K16	R2
      0x80040000,  //  002A  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: to_TLV
********************************************************************/
be_local_closure(class_Matter_EventDataIB_to_TLV,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_EventDataIB,     /* shared constants */
    be_str_weak(to_TLV),
    &be_const_str_solidified,
    ( &(const binstruction[51]) {  /* code */
      0xB80A0200,  //  0000  GETNGBL	R2	K1
      0x88080511,  //  0001  GETMBR	R2	R2	K17
      0x4C0C0000,  //  0002  LDNIL	R3
      0x1C0C0203,  //  0003  EQ	R3	R1	R3
      0x780E0002,  //  0004  JMPF	R3	#0008
      0x8C0C0512,  //  0005  GETMET	R3	R2	K18
      0x7C0C0200,  //  0006  CALL	R3	1
      0x5C040600,  //  0007  MOVE	R1	R3
      0x880C0100,  //  0008  GETMBR	R3	R0	K0
      0x780E0005,  //  0009  JMPF	R3	#0010
      0x880C0100,  //  000A  GETMBR	R3	R0	K0
      0x8C0C0713,  //  000B  GETMET	R3	R3	K19
      0x8C140314,  //  000C  GETMET	R5	R1	K20
      0x581C0005,  //  000D  LDCONST	R7	K5
      0x7C140400,  //  000E  CALL	R5	2
      0x7C0C0400,  //  000F  CALL	R3	2
      0x8C0C0315,  //  0010  GETMET	R3	R1	K21
      0x58140008,  //  0011  LDCONST	R5	K8
      0x88180516,  //  0012  GETMBR	R6	R2	K22
      0x881C0106,  //  0013  GETMBR	R7	R0	K6
      0x7C0C0800,  //  0014  CALL	R3	4
      0x8C0C0315,  //  0015  GETMET	R3	R1	K21
      0x5814000A,  //  0016  LDCONST	R5	K10
      0x88180517,  //  0017  GETMBR	R6	R2	K23
      0x881C0109,  //  0018  GETMBR	R7	R0	K9
      0x7C0C0800,  //  0019  CALL	R3	4
      0x8C0C0315,  //  001A  GETMET	R3	R1	K21
      0x5814000C,  //  001B  LDCONST	R5	K12
      0x88180516,  //  001C  GETMBR	R6	R2	K22
      0x881C010B,  //  001D  GETMBR	R7	R0	K11
      0x7C0C0800,  //  001E  CALL	R3	4
      0x8C0C0315,  //  001F  GETMET	R3	R1	K21
      0x54160003,  //  0020  LDINT	R5	4
      0x88180516,  //  0021  GETMBR	R6	R2	K22
      0x881C010D,  //  0022  GETMBR	R7	R0	K13
      0x7C0C0800,  //  0023  CALL	R3	4
      0x8C0C0315,  //  0024  GETMET	R3	R1	K21
      0x54160004,  //  0025  LDINT	R5	5
      0x88180516,  //  0026  GETMBR	R6	R2	K22
      0x881C010E,  //  0027  GETMBR	R7	R0	K14
      0x7C0C0800,  //  0028  CALL	R3	4
      0x8C0C0315,  //  0029  GETMET	R3	R1	K21
      0x54160005,  //  002A  LDINT	R5	6
      0x88180516,  //  002B  GETMBR	R6	R2	K22
      0x881C010F,  //  002C  GETMBR	R7	R0	K15
      0x7C0C0800,  //  002D  CALL	R3	4
      0x8C0C0318,  //  002E  GETMET	R3	R1	K24
      0x54160006,  //  002F  LDINT	R5	7
      0x88180110,  //  0030  GETMBR	R6	R0	K16
      0x7C0C0600,  //  0031  CALL	R3	3
      0x80040200,  //  0032  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_EventDataIB
********************************************************************/
extern const bclass be_class_Matter_IM_base;
be_local_class(Matter_EventDataIB,
    8,
    &be_class_Matter_IM_base,
    be_nested_map(10,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(delta_system_timestamp, -1), be_const_var(6) },
        { be_const_key_weak(data, -1), be_const_var(7) },
        { be_const_key_weak(to_TLV, 6), be_const_closure(class_Matter_EventDataIB_to_TLV_closure) },
        { be_const_key_weak(from_TLV, -1), be_const_closure(class_Matter_EventDataIB_from_TLV_closure) },
        { be_const_key_weak(delta_epoch_timestamp, -1), be_const_var(5) },
        { be_const_key_weak(epoch_timestamp, -1), be_const_var(3) },
        { be_const_key_weak(system_timestamp, -1), be_const_var(4) },
        { be_const_key_weak(priority, 5), be_const_var(2) },
        { be_const_key_weak(path, 3), be_const_var(0) },
        { be_const_key_weak(event_number, 2), be_const_var(1) },
    })),
    be_str_weak(Matter_EventDataIB)
);
// compact class 'Matter_EventReportIB' ktab size: 12, total: 17 (saved 40 bytes)
static const bvalue be_ktab_class_Matter_EventReportIB[12] = {
  /* K0   */  be_nested_str_weak(matter),
  /* K1   */  be_nested_str_weak(TLV),
  /* K2   */  be_nested_str_weak(Matter_TLV_struct),
  /* K3   */  be_nested_str_weak(add_obj),
  /* K4   */  be_const_int(0),
  /* K5   */  be_nested_str_weak(event_status),
  /* K6   */  be_const_int(1),
  /* K7   */  be_nested_str_weak(event_data),
  /* K8   */  be_nested_str_weak(EventStatusIB),
  /* K9   */  be_nested_str_weak(from_TLV),
  /* K10  */  be_nested_str_weak(findsub),
  /* K11  */  be_nested_str_weak(EventDataIB),
};


extern const bclass be_class_Matter_EventReportIB;

/********************************************************************
** Solidified function: to_TLV
********************************************************************/
be_local_closure(class_Matter_EventReportIB_to_TLV,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_EventReportIB,     /* shared constants */
    be_str_weak(to_TLV),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x88040301,  //  0001  GETMBR	R1	R1	K1
      0x8C080302,  //  0002  GETMET	R2	R1	K2
      0x7C080200,  //  0003  CALL	R2	1
      0x8C0C0503,  //  0004  GETMET	R3	R2	K3
      0x58140004,  //  0005  LDCONST	R5	K4
      0x88180105,  //  0006  GETMBR	R6	R0	K5
      0x7C0C0600,  //  0007  CALL	R3	3
      0x8C0C0503,  //  0008  GETMET	R3	R2	K3
      0x58140006,  //  0009  LDCONST	R5	K6
      0x88180107,  //  000A  GETMBR	R6	R0	K7
      0x7C0C0600,  //  000B  CALL	R3	3
      0x80040400,  //  000C  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: from_TLV
********************************************************************/
be_local_closure(class_Matter_EventReportIB_from_TLV,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_EventReportIB,     /* shared constants */
    be_str_weak(from_TLV),
    &be_const_str_solidified,
    ( &(const binstruction[24]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0001,  //  0002  JMPF	R2	#0005
      0x4C080000,  //  0003  LDNIL	R2
      0x80040400,  //  0004  RET	1	R2
      0xB80A0000,  //  0005  GETNGBL	R2	K0
      0x8C080508,  //  0006  GETMET	R2	R2	K8
      0x7C080200,  //  0007  CALL	R2	1
      0x8C080509,  //  0008  GETMET	R2	R2	K9
      0x8C10030A,  //  0009  GETMET	R4	R1	K10
      0x58180004,  //  000A  LDCONST	R6	K4
      0x7C100400,  //  000B  CALL	R4	2
      0x7C080400,  //  000C  CALL	R2	2
      0x90020A02,  //  000D  SETMBR	R0	K5	R2
      0xB80A0000,  //  000E  GETNGBL	R2	K0
      0x8C08050B,  //  000F  GETMET	R2	R2	K11
      0x7C080200,  //  0010  CALL	R2	1
      0x8C080509,  //  0011  GETMET	R2	R2	K9
      0x8C10030A,  //  0012  GETMET	R4	R1	K10
      0x58180006,  //  0013  LDCONST	R6	K6
      0x7C100400,  //  0014  CALL	R4	2
      0x7C080400,  //  0015  CALL	R2	2
      0x90020E02,  //  0016  SETMBR	R0	K7	R2
      0x80040000,  //  0017  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_EventReportIB
********************************************************************/
extern const bclass be_class_Matter_IM_base;
be_local_class(Matter_EventReportIB,
    2,
    &be_class_Matter_IM_base,
    be_nested_map(4,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(event_data, 3), be_const_var(1) },
        { be_const_key_weak(to_TLV, -1), be_const_closure(class_Matter_EventReportIB_to_TLV_closure) },
        { be_const_key_weak(event_status, -1), be_const_var(0) },
        { be_const_key_weak(from_TLV, -1), be_const_closure(class_Matter_EventReportIB_from_TLV_closure) },
    })),
    be_str_weak(Matter_EventReportIB)
);
// compact class 'Matter_CommandPathIB' ktab size: 13, total: 19 (saved 48 bytes)
static const bvalue be_ktab_class_Matter_CommandPathIB[13] = {
  /* K0   */  be_nested_str_weak(endpoint),
  /* K1   */  be_nested_str_weak(findsubval),
  /* K2   */  be_const_int(0),
  /* K3   */  be_nested_str_weak(cluster),
  /* K4   */  be_const_int(1),
  /* K5   */  be_nested_str_weak(command),
  /* K6   */  be_const_int(2),
  /* K7   */  be_nested_str_weak(matter),
  /* K8   */  be_nested_str_weak(TLV),
  /* K9   */  be_nested_str_weak(Matter_TLV_list),
  /* K10  */  be_nested_str_weak(add_TLV),
  /* K11  */  be_nested_str_weak(U2),
  /* K12  */  be_nested_str_weak(U4),
};


extern const bclass be_class_Matter_CommandPathIB;

/********************************************************************
** Solidified function: from_TLV
********************************************************************/
be_local_closure(class_Matter_CommandPathIB_from_TLV,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_CommandPathIB,     /* shared constants */
    be_str_weak(from_TLV),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0001,  //  0002  JMPF	R2	#0005
      0x4C080000,  //  0003  LDNIL	R2
      0x80040400,  //  0004  RET	1	R2
      0x8C080301,  //  0005  GETMET	R2	R1	K1
      0x58100002,  //  0006  LDCONST	R4	K2
      0x7C080400,  //  0007  CALL	R2	2
      0x90020002,  //  0008  SETMBR	R0	K0	R2
      0x8C080301,  //  0009  GETMET	R2	R1	K1
      0x58100004,  //  000A  LDCONST	R4	K4
      0x7C080400,  //  000B  CALL	R2	2
      0x90020602,  //  000C  SETMBR	R0	K3	R2
      0x8C080301,  //  000D  GETMET	R2	R1	K1
      0x58100006,  //  000E  LDCONST	R4	K6
      0x7C080400,  //  000F  CALL	R2	2
      0x90020A02,  //  0010  SETMBR	R0	K5	R2
      0x80040000,  //  0011  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: to_TLV
********************************************************************/
be_local_closure(class_Matter_CommandPathIB_to_TLV,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_CommandPathIB,     /* shared constants */
    be_str_weak(to_TLV),
    &be_const_str_solidified,
    ( &(const binstruction[20]) {  /* code */
      0xB8060E00,  //  0000  GETNGBL	R1	K7
      0x88040308,  //  0001  GETMBR	R1	R1	K8
      0x8C080309,  //  0002  GETMET	R2	R1	K9
      0x7C080200,  //  0003  CALL	R2	1
      0x8C0C050A,  //  0004  GETMET	R3	R2	K10
      0x58140002,  //  0005  LDCONST	R5	K2
      0x8818030B,  //  0006  GETMBR	R6	R1	K11
      0x881C0100,  //  0007  GETMBR	R7	R0	K0
      0x7C0C0800,  //  0008  CALL	R3	4
      0x8C0C050A,  //  0009  GETMET	R3	R2	K10
      0x58140004,  //  000A  LDCONST	R5	K4
      0x8818030C,  //  000B  GETMBR	R6	R1	K12
      0x881C0103,  //  000C  GETMBR	R7	R0	K3
      0x7C0C0800,  //  000D  CALL	R3	4
      0x8C0C050A,  //  000E  GETMET	R3	R2	K10
      0x58140006,  //  000F  LDCONST	R5	K6
      0x8818030C,  //  0010  GETMBR	R6	R1	K12
      0x881C0105,  //  0011  GETMBR	R7	R0	K5
      0x7C0C0800,  //  0012  CALL	R3	4
      0x80040400,  //  0013  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_CommandPathIB
********************************************************************/
extern const bclass be_class_Matter_IM_base;
be_local_class(Matter_CommandPathIB,
    3,
    &be_class_Matter_IM_base,
    be_nested_map(5,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(endpoint, -1), be_const_var(0) },
        { be_const_key_weak(from_TLV, 0), be_const_closure(class_Matter_CommandPathIB_from_TLV_closure) },
        { be_const_key_weak(cluster, -1), be_const_var(1) },
        { be_const_key_weak(command, 1), be_const_var(2) },
        { be_const_key_weak(to_TLV, -1), be_const_closure(class_Matter_CommandPathIB_to_TLV_closure) },
    })),
    be_str_weak(Matter_CommandPathIB)
);
// compact class 'Matter_CommandDataIB' ktab size: 11, total: 16 (saved 40 bytes)
static const bvalue be_ktab_class_Matter_CommandDataIB[11] = {
  /* K0   */  be_nested_str_weak(matter),
  /* K1   */  be_nested_str_weak(TLV),
  /* K2   */  be_nested_str_weak(Matter_TLV_struct),
  /* K3   */  be_nested_str_weak(add_obj),
  /* K4   */  be_const_int(0),
  /* K5   */  be_nested_str_weak(command_path),
  /* K6   */  be_const_int(1),
  /* K7   */  be_nested_str_weak(command_fields),
  /* K8   */  be_nested_str_weak(CommandPathIB),
  /* K9   */  be_nested_str_weak(from_TLV),
  /* K10  */  be_nested_str_weak(findsub),
};


extern const bclass be_class_Matter_CommandDataIB;

/********************************************************************
** Solidified function: to_TLV
********************************************************************/
be_local_closure(class_Matter_CommandDataIB_to_TLV,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_CommandDataIB,     /* shared constants */
    be_str_weak(to_TLV),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x88040301,  //  0001  GETMBR	R1	R1	K1
      0x8C080302,  //  0002  GETMET	R2	R1	K2
      0x7C080200,  //  0003  CALL	R2	1
      0x8C0C0503,  //  0004  GETMET	R3	R2	K3
      0x58140004,  //  0005  LDCONST	R5	K4
      0x88180105,  //  0006  GETMBR	R6	R0	K5
      0x7C0C0600,  //  0007  CALL	R3	3
      0x8C0C0503,  //  0008  GETMET	R3	R2	K3
      0x58140006,  //  0009  LDCONST	R5	K6
      0x88180107,  //  000A  GETMBR	R6	R0	K7
      0x7C0C0600,  //  000B  CALL	R3	3
      0x80040400,  //  000C  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: from_TLV
********************************************************************/
be_local_closure(class_Matter_CommandDataIB_from_TLV,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_CommandDataIB,     /* shared constants */
    be_str_weak(from_TLV),
    &be_const_str_solidified,
    ( &(const binstruction[19]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0001,  //  0002  JMPF	R2	#0005
      0x4C080000,  //  0003  LDNIL	R2
      0x80040400,  //  0004  RET	1	R2
      0xB80A0000,  //  0005  GETNGBL	R2	K0
      0x8C080508,  //  0006  GETMET	R2	R2	K8
      0x7C080200,  //  0007  CALL	R2	1
      0x8C080509,  //  0008  GETMET	R2	R2	K9
      0x8C10030A,  //  0009  GETMET	R4	R1	K10
      0x58180004,  //  000A  LDCONST	R6	K4
      0x7C100400,  //  000B  CALL	R4	2
      0x7C080400,  //  000C  CALL	R2	2
      0x90020A02,  //  000D  SETMBR	R0	K5	R2
      0x8C08030A,  //  000E  GETMET	R2	R1	K10
      0x58100006,  //  000F  LDCONST	R4	K6
      0x7C080400,  //  0010  CALL	R2	2
      0x90020E02,  //  0011  SETMBR	R0	K7	R2
      0x80040000,  //  0012  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_CommandDataIB
********************************************************************/
extern const bclass be_class_Matter_IM_base;
be_local_class(Matter_CommandDataIB,
    2,
    &be_class_Matter_IM_base,
    be_nested_map(4,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(command_path, 3), be_const_var(0) },
        { be_const_key_weak(to_TLV, -1), be_const_closure(class_Matter_CommandDataIB_to_TLV_closure) },
        { be_const_key_weak(command_fields, -1), be_const_var(1) },
        { be_const_key_weak(from_TLV, -1), be_const_closure(class_Matter_CommandDataIB_from_TLV_closure) },
    })),
    be_str_weak(Matter_CommandDataIB)
);
// compact class 'Matter_InvokeResponseIB' ktab size: 12, total: 17 (saved 40 bytes)
static const bvalue be_ktab_class_Matter_InvokeResponseIB[12] = {
  /* K0   */  be_nested_str_weak(command),
  /* K1   */  be_nested_str_weak(matter),
  /* K2   */  be_nested_str_weak(CommandDataIB),
  /* K3   */  be_nested_str_weak(from_TLV),
  /* K4   */  be_nested_str_weak(findsub),
  /* K5   */  be_const_int(0),
  /* K6   */  be_nested_str_weak(status),
  /* K7   */  be_nested_str_weak(CommandStatusIB),
  /* K8   */  be_const_int(1),
  /* K9   */  be_nested_str_weak(TLV),
  /* K10  */  be_nested_str_weak(Matter_TLV_struct),
  /* K11  */  be_nested_str_weak(add_obj),
};


extern const bclass be_class_Matter_InvokeResponseIB;

/********************************************************************
** Solidified function: from_TLV
********************************************************************/
be_local_closure(class_Matter_InvokeResponseIB_from_TLV,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_InvokeResponseIB,     /* shared constants */
    be_str_weak(from_TLV),
    &be_const_str_solidified,
    ( &(const binstruction[24]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0001,  //  0002  JMPF	R2	#0005
      0x4C080000,  //  0003  LDNIL	R2
      0x80040400,  //  0004  RET	1	R2
      0xB80A0200,  //  0005  GETNGBL	R2	K1
      0x8C080502,  //  0006  GETMET	R2	R2	K2
      0x7C080200,  //  0007  CALL	R2	1
      0x8C080503,  //  0008  GETMET	R2	R2	K3
      0x8C100304,  //  0009  GETMET	R4	R1	K4
      0x58180005,  //  000A  LDCONST	R6	K5
      0x7C100400,  //  000B  CALL	R4	2
      0x7C080400,  //  000C  CALL	R2	2
      0x90020002,  //  000D  SETMBR	R0	K0	R2
      0xB80A0200,  //  000E  GETNGBL	R2	K1
      0x8C080507,  //  000F  GETMET	R2	R2	K7
      0x7C080200,  //  0010  CALL	R2	1
      0x8C080503,  //  0011  GETMET	R2	R2	K3
      0x8C100304,  //  0012  GETMET	R4	R1	K4
      0x58180008,  //  0013  LDCONST	R6	K8
      0x7C100400,  //  0014  CALL	R4	2
      0x7C080400,  //  0015  CALL	R2	2
      0x90020C02,  //  0016  SETMBR	R0	K6	R2
      0x80040000,  //  0017  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: to_TLV
********************************************************************/
be_local_closure(class_Matter_InvokeResponseIB_to_TLV,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_InvokeResponseIB,     /* shared constants */
    be_str_weak(to_TLV),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0xB8060200,  //  0000  GETNGBL	R1	K1
      0x88040309,  //  0001  GETMBR	R1	R1	K9
      0x8C08030A,  //  0002  GETMET	R2	R1	K10
      0x7C080200,  //  0003  CALL	R2	1
      0x8C0C050B,  //  0004  GETMET	R3	R2	K11
      0x58140005,  //  0005  LDCONST	R5	K5
      0x88180100,  //  0006  GETMBR	R6	R0	K0
      0x7C0C0600,  //  0007  CALL	R3	3
      0x8C0C050B,  //  0008  GETMET	R3	R2	K11
      0x58140008,  //  0009  LDCONST	R5	K8
      0x88180106,  //  000A  GETMBR	R6	R0	K6
      0x7C0C0600,  //  000B  CALL	R3	3
      0x80040400,  //  000C  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_InvokeResponseIB
********************************************************************/
extern const bclass be_class_Matter_IM_base;
be_local_class(Matter_InvokeResponseIB,
    2,
    &be_class_Matter_IM_base,
    be_nested_map(4,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(from_TLV, -1), be_const_closure(class_Matter_InvokeResponseIB_from_TLV_closure) },
        { be_const_key_weak(to_TLV, -1), be_const_closure(class_Matter_InvokeResponseIB_to_TLV_closure) },
        { be_const_key_weak(command, -1), be_const_var(0) },
        { be_const_key_weak(status, -1), be_const_var(1) },
    })),
    be_str_weak(Matter_InvokeResponseIB)
);
// compact class 'Matter_CommandStatusIB' ktab size: 12, total: 17 (saved 40 bytes)
static const bvalue be_ktab_class_Matter_CommandStatusIB[12] = {
  /* K0   */  be_nested_str_weak(matter),
  /* K1   */  be_nested_str_weak(TLV),
  /* K2   */  be_nested_str_weak(Matter_TLV_struct),
  /* K3   */  be_nested_str_weak(add_obj),
  /* K4   */  be_const_int(0),
  /* K5   */  be_nested_str_weak(command_path),
  /* K6   */  be_const_int(1),
  /* K7   */  be_nested_str_weak(status),
  /* K8   */  be_nested_str_weak(CommandPathIB),
  /* K9   */  be_nested_str_weak(from_TLV),
  /* K10  */  be_nested_str_weak(findsub),
  /* K11  */  be_nested_str_weak(StatusIB),
};


extern const bclass be_class_Matter_CommandStatusIB;

/********************************************************************
** Solidified function: to_TLV
********************************************************************/
be_local_closure(class_Matter_CommandStatusIB_to_TLV,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_CommandStatusIB,     /* shared constants */
    be_str_weak(to_TLV),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x88040301,  //  0001  GETMBR	R1	R1	K1
      0x8C080302,  //  0002  GETMET	R2	R1	K2
      0x7C080200,  //  0003  CALL	R2	1
      0x8C0C0503,  //  0004  GETMET	R3	R2	K3
      0x58140004,  //  0005  LDCONST	R5	K4
      0x88180105,  //  0006  GETMBR	R6	R0	K5
      0x7C0C0600,  //  0007  CALL	R3	3
      0x8C0C0503,  //  0008  GETMET	R3	R2	K3
      0x58140006,  //  0009  LDCONST	R5	K6
      0x88180107,  //  000A  GETMBR	R6	R0	K7
      0x7C0C0600,  //  000B  CALL	R3	3
      0x80040400,  //  000C  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: from_TLV
********************************************************************/
be_local_closure(class_Matter_CommandStatusIB_from_TLV,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_CommandStatusIB,     /* shared constants */
    be_str_weak(from_TLV),
    &be_const_str_solidified,
    ( &(const binstruction[24]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0001,  //  0002  JMPF	R2	#0005
      0x4C080000,  //  0003  LDNIL	R2
      0x80040400,  //  0004  RET	1	R2
      0xB80A0000,  //  0005  GETNGBL	R2	K0
      0x8C080508,  //  0006  GETMET	R2	R2	K8
      0x7C080200,  //  0007  CALL	R2	1
      0x8C080509,  //  0008  GETMET	R2	R2	K9
      0x8C10030A,  //  0009  GETMET	R4	R1	K10
      0x58180004,  //  000A  LDCONST	R6	K4
      0x7C100400,  //  000B  CALL	R4	2
      0x7C080400,  //  000C  CALL	R2	2
      0x90020A02,  //  000D  SETMBR	R0	K5	R2
      0xB80A0000,  //  000E  GETNGBL	R2	K0
      0x8C08050B,  //  000F  GETMET	R2	R2	K11
      0x7C080200,  //  0010  CALL	R2	1
      0x8C080509,  //  0011  GETMET	R2	R2	K9
      0x8C10030A,  //  0012  GETMET	R4	R1	K10
      0x58180006,  //  0013  LDCONST	R6	K6
      0x7C100400,  //  0014  CALL	R4	2
      0x7C080400,  //  0015  CALL	R2	2
      0x90020E02,  //  0016  SETMBR	R0	K7	R2
      0x80040000,  //  0017  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_CommandStatusIB
********************************************************************/
extern const bclass be_class_Matter_IM_base;
be_local_class(Matter_CommandStatusIB,
    2,
    &be_class_Matter_IM_base,
    be_nested_map(4,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(command_path, 2), be_const_var(0) },
        { be_const_key_weak(to_TLV, -1), be_const_closure(class_Matter_CommandStatusIB_to_TLV_closure) },
        { be_const_key_weak(from_TLV, -1), be_const_closure(class_Matter_CommandStatusIB_from_TLV_closure) },
        { be_const_key_weak(status, -1), be_const_var(1) },
    })),
    be_str_weak(Matter_CommandStatusIB)
);
// compact class 'Matter_EventStatusIB' ktab size: 12, total: 17 (saved 40 bytes)
static const bvalue be_ktab_class_Matter_EventStatusIB[12] = {
  /* K0   */  be_nested_str_weak(path),
  /* K1   */  be_nested_str_weak(matter),
  /* K2   */  be_nested_str_weak(EventPathIB),
  /* K3   */  be_nested_str_weak(from_TLV),
  /* K4   */  be_nested_str_weak(findsub),
  /* K5   */  be_const_int(0),
  /* K6   */  be_nested_str_weak(status),
  /* K7   */  be_nested_str_weak(StatusIB),
  /* K8   */  be_const_int(1),
  /* K9   */  be_nested_str_weak(TLV),
  /* K10  */  be_nested_str_weak(Matter_TLV_struct),
  /* K11  */  be_nested_str_weak(add_obj),
};


extern const bclass be_class_Matter_EventStatusIB;

/********************************************************************
** Solidified function: from_TLV
********************************************************************/
be_local_closure(class_Matter_EventStatusIB_from_TLV,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_EventStatusIB,     /* shared constants */
    be_str_weak(from_TLV),
    &be_const_str_solidified,
    ( &(const binstruction[24]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0001,  //  0002  JMPF	R2	#0005
      0x4C080000,  //  0003  LDNIL	R2
      0x80040400,  //  0004  RET	1	R2
      0xB80A0200,  //  0005  GETNGBL	R2	K1
      0x8C080502,  //  0006  GETMET	R2	R2	K2
      0x7C080200,  //  0007  CALL	R2	1
      0x8C080503,  //  0008  GETMET	R2	R2	K3
      0x8C100304,  //  0009  GETMET	R4	R1	K4
      0x58180005,  //  000A  LDCONST	R6	K5
      0x7C100400,  //  000B  CALL	R4	2
      0x7C080400,  //  000C  CALL	R2	2
      0x90020002,  //  000D  SETMBR	R0	K0	R2
      0xB80A0200,  //  000E  GETNGBL	R2	K1
      0x8C080507,  //  000F  GETMET	R2	R2	K7
      0x7C080200,  //  0010  CALL	R2	1
      0x8C080503,  //  0011  GETMET	R2	R2	K3
      0x8C100304,  //  0012  GETMET	R4	R1	K4
      0x58180008,  //  0013  LDCONST	R6	K8
      0x7C100400,  //  0014  CALL	R4	2
      0x7C080400,  //  0015  CALL	R2	2
      0x90020C02,  //  0016  SETMBR	R0	K6	R2
      0x80040000,  //  0017  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: to_TLV
********************************************************************/
be_local_closure(class_Matter_EventStatusIB_to_TLV,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_EventStatusIB,     /* shared constants */
    be_str_weak(to_TLV),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0xB8060200,  //  0000  GETNGBL	R1	K1
      0x88040309,  //  0001  GETMBR	R1	R1	K9
      0x8C08030A,  //  0002  GETMET	R2	R1	K10
      0x7C080200,  //  0003  CALL	R2	1
      0x8C0C050B,  //  0004  GETMET	R3	R2	K11
      0x58140005,  //  0005  LDCONST	R5	K5
      0x88180100,  //  0006  GETMBR	R6	R0	K0
      0x7C0C0600,  //  0007  CALL	R3	3
      0x8C0C050B,  //  0008  GETMET	R3	R2	K11
      0x58140008,  //  0009  LDCONST	R5	K8
      0x88180106,  //  000A  GETMBR	R6	R0	K6
      0x7C0C0600,  //  000B  CALL	R3	3
      0x80040400,  //  000C  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_EventStatusIB
********************************************************************/
extern const bclass be_class_Matter_IM_base;
be_local_class(Matter_EventStatusIB,
    2,
    &be_class_Matter_IM_base,
    be_nested_map(4,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(from_TLV, -1), be_const_closure(class_Matter_EventStatusIB_from_TLV_closure) },
        { be_const_key_weak(to_TLV, -1), be_const_closure(class_Matter_EventStatusIB_to_TLV_closure) },
        { be_const_key_weak(path, -1), be_const_var(0) },
        { be_const_key_weak(status, -1), be_const_var(1) },
    })),
    be_str_weak(Matter_EventStatusIB)
);
// compact class 'Matter_AttributeStatusIB' ktab size: 12, total: 17 (saved 40 bytes)
static const bvalue be_ktab_class_Matter_AttributeStatusIB[12] = {
  /* K0   */  be_nested_str_weak(path),
  /* K1   */  be_nested_str_weak(matter),
  /* K2   */  be_nested_str_weak(AttributePathIB),
  /* K3   */  be_nested_str_weak(from_TLV),
  /* K4   */  be_nested_str_weak(findsub),
  /* K5   */  be_const_int(0),
  /* K6   */  be_nested_str_weak(status),
  /* K7   */  be_nested_str_weak(StatusIB),
  /* K8   */  be_const_int(1),
  /* K9   */  be_nested_str_weak(TLV),
  /* K10  */  be_nested_str_weak(Matter_TLV_struct),
  /* K11  */  be_nested_str_weak(add_obj),
};


extern const bclass be_class_Matter_AttributeStatusIB;

/********************************************************************
** Solidified function: from_TLV
********************************************************************/
be_local_closure(class_Matter_AttributeStatusIB_from_TLV,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_AttributeStatusIB,     /* shared constants */
    be_str_weak(from_TLV),
    &be_const_str_solidified,
    ( &(const binstruction[24]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0001,  //  0002  JMPF	R2	#0005
      0x4C080000,  //  0003  LDNIL	R2
      0x80040400,  //  0004  RET	1	R2
      0xB80A0200,  //  0005  GETNGBL	R2	K1
      0x8C080502,  //  0006  GETMET	R2	R2	K2
      0x7C080200,  //  0007  CALL	R2	1
      0x8C080503,  //  0008  GETMET	R2	R2	K3
      0x8C100304,  //  0009  GETMET	R4	R1	K4
      0x58180005,  //  000A  LDCONST	R6	K5
      0x7C100400,  //  000B  CALL	R4	2
      0x7C080400,  //  000C  CALL	R2	2
      0x90020002,  //  000D  SETMBR	R0	K0	R2
      0xB80A0200,  //  000E  GETNGBL	R2	K1
      0x8C080507,  //  000F  GETMET	R2	R2	K7
      0x7C080200,  //  0010  CALL	R2	1
      0x8C080503,  //  0011  GETMET	R2	R2	K3
      0x8C100304,  //  0012  GETMET	R4	R1	K4
      0x58180008,  //  0013  LDCONST	R6	K8
      0x7C100400,  //  0014  CALL	R4	2
      0x7C080400,  //  0015  CALL	R2	2
      0x90020C02,  //  0016  SETMBR	R0	K6	R2
      0x80040000,  //  0017  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: to_TLV
********************************************************************/
be_local_closure(class_Matter_AttributeStatusIB_to_TLV,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_AttributeStatusIB,     /* shared constants */
    be_str_weak(to_TLV),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0xB8060200,  //  0000  GETNGBL	R1	K1
      0x88040309,  //  0001  GETMBR	R1	R1	K9
      0x8C08030A,  //  0002  GETMET	R2	R1	K10
      0x7C080200,  //  0003  CALL	R2	1
      0x8C0C050B,  //  0004  GETMET	R3	R2	K11
      0x58140005,  //  0005  LDCONST	R5	K5
      0x88180100,  //  0006  GETMBR	R6	R0	K0
      0x7C0C0600,  //  0007  CALL	R3	3
      0x8C0C050B,  //  0008  GETMET	R3	R2	K11
      0x58140008,  //  0009  LDCONST	R5	K8
      0x88180106,  //  000A  GETMBR	R6	R0	K6
      0x7C0C0600,  //  000B  CALL	R3	3
      0x80040400,  //  000C  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_AttributeStatusIB
********************************************************************/
extern const bclass be_class_Matter_IM_base;
be_local_class(Matter_AttributeStatusIB,
    2,
    &be_class_Matter_IM_base,
    be_nested_map(4,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(from_TLV, -1), be_const_closure(class_Matter_AttributeStatusIB_from_TLV_closure) },
        { be_const_key_weak(to_TLV, -1), be_const_closure(class_Matter_AttributeStatusIB_to_TLV_closure) },
        { be_const_key_weak(path, -1), be_const_var(0) },
        { be_const_key_weak(status, -1), be_const_var(1) },
    })),
    be_str_weak(Matter_AttributeStatusIB)
);
// compact class 'Matter_StatusIB' ktab size: 10, total: 14 (saved 32 bytes)
static const bvalue be_ktab_class_Matter_StatusIB[10] = {
  /* K0   */  be_nested_str_weak(matter),
  /* K1   */  be_nested_str_weak(TLV),
  /* K2   */  be_nested_str_weak(Matter_TLV_struct),
  /* K3   */  be_nested_str_weak(add_TLV),
  /* K4   */  be_const_int(0),
  /* K5   */  be_nested_str_weak(U2),
  /* K6   */  be_nested_str_weak(status),
  /* K7   */  be_const_int(1),
  /* K8   */  be_nested_str_weak(cluster_status),
  /* K9   */  be_nested_str_weak(findsubval),
};


extern const bclass be_class_Matter_StatusIB;

/********************************************************************
** Solidified function: to_TLV
********************************************************************/
be_local_closure(class_Matter_StatusIB_to_TLV,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_StatusIB,     /* shared constants */
    be_str_weak(to_TLV),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x88040301,  //  0001  GETMBR	R1	R1	K1
      0x8C080302,  //  0002  GETMET	R2	R1	K2
      0x7C080200,  //  0003  CALL	R2	1
      0x8C0C0503,  //  0004  GETMET	R3	R2	K3
      0x58140004,  //  0005  LDCONST	R5	K4
      0x88180305,  //  0006  GETMBR	R6	R1	K5
      0x881C0106,  //  0007  GETMBR	R7	R0	K6
      0x7C0C0800,  //  0008  CALL	R3	4
      0x8C0C0503,  //  0009  GETMET	R3	R2	K3
      0x58140007,  //  000A  LDCONST	R5	K7
      0x88180305,  //  000B  GETMBR	R6	R1	K5
      0x881C0108,  //  000C  GETMBR	R7	R0	K8
      0x7C0C0800,  //  000D  CALL	R3	4
      0x80040400,  //  000E  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: from_TLV
********************************************************************/
be_local_closure(class_Matter_StatusIB_from_TLV,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_StatusIB,     /* shared constants */
    be_str_weak(from_TLV),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0001,  //  0002  JMPF	R2	#0005
      0x4C080000,  //  0003  LDNIL	R2
      0x80040400,  //  0004  RET	1	R2
      0x8C080309,  //  0005  GETMET	R2	R1	K9
      0x58100004,  //  0006  LDCONST	R4	K4
      0x7C080400,  //  0007  CALL	R2	2
      0x90020C02,  //  0008  SETMBR	R0	K6	R2
      0x8C080309,  //  0009  GETMET	R2	R1	K9
      0x58100007,  //  000A  LDCONST	R4	K7
      0x7C080400,  //  000B  CALL	R2	2
      0x90021002,  //  000C  SETMBR	R0	K8	R2
      0x80040000,  //  000D  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_StatusIB
********************************************************************/
extern const bclass be_class_Matter_IM_base;
be_local_class(Matter_StatusIB,
    2,
    &be_class_Matter_IM_base,
    be_nested_map(4,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(cluster_status, 2), be_const_var(1) },
        { be_const_key_weak(to_TLV, -1), be_const_closure(class_Matter_StatusIB_to_TLV_closure) },
        { be_const_key_weak(from_TLV, -1), be_const_closure(class_Matter_StatusIB_from_TLV_closure) },
        { be_const_key_weak(status, -1), be_const_var(0) },
    })),
    be_str_weak(Matter_StatusIB)
);
// compact class 'Matter_StatusResponseMessage' ktab size: 10, total: 12 (saved 16 bytes)
static const bvalue be_ktab_class_Matter_StatusResponseMessage[10] = {
  /* K0   */  be_nested_str_weak(status),
  /* K1   */  be_nested_str_weak(findsubval),
  /* K2   */  be_const_int(0),
  /* K3   */  be_nested_str_weak(matter),
  /* K4   */  be_nested_str_weak(TLV),
  /* K5   */  be_nested_str_weak(Matter_TLV_struct),
  /* K6   */  be_nested_str_weak(add_TLV),
  /* K7   */  be_nested_str_weak(U4),
  /* K8   */  be_nested_str_weak(U1),
  /* K9   */  be_nested_str_weak(InteractionModelRevision),
};


extern const bclass be_class_Matter_StatusResponseMessage;

/********************************************************************
** Solidified function: from_TLV
********************************************************************/
be_local_closure(class_Matter_StatusResponseMessage_from_TLV,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_StatusResponseMessage,     /* shared constants */
    be_str_weak(from_TLV),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0001,  //  0002  JMPF	R2	#0005
      0x4C080000,  //  0003  LDNIL	R2
      0x80040400,  //  0004  RET	1	R2
      0x8C080301,  //  0005  GETMET	R2	R1	K1
      0x58100002,  //  0006  LDCONST	R4	K2
      0x7C080400,  //  0007  CALL	R2	2
      0x90020002,  //  0008  SETMBR	R0	K0	R2
      0x80040000,  //  0009  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: to_TLV
********************************************************************/
be_local_closure(class_Matter_StatusResponseMessage_to_TLV,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_StatusResponseMessage,     /* shared constants */
    be_str_weak(to_TLV),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0xB8060600,  //  0000  GETNGBL	R1	K3
      0x88040304,  //  0001  GETMBR	R1	R1	K4
      0x8C080305,  //  0002  GETMET	R2	R1	K5
      0x7C080200,  //  0003  CALL	R2	1
      0x8C0C0506,  //  0004  GETMET	R3	R2	K6
      0x58140002,  //  0005  LDCONST	R5	K2
      0x88180307,  //  0006  GETMBR	R6	R1	K7
      0x881C0100,  //  0007  GETMBR	R7	R0	K0
      0x7C0C0800,  //  0008  CALL	R3	4
      0x8C0C0506,  //  0009  GETMET	R3	R2	K6
      0x541600FE,  //  000A  LDINT	R5	255
      0x88180308,  //  000B  GETMBR	R6	R1	K8
      0x881C0109,  //  000C  GETMBR	R7	R0	K9
      0x7C0C0800,  //  000D  CALL	R3	4
      0x80040400,  //  000E  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_StatusResponseMessage
********************************************************************/
extern const bclass be_class_Matter_IM_Message_base;
be_local_class(Matter_StatusResponseMessage,
    1,
    &be_class_Matter_IM_Message_base,
    be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(from_TLV, -1), be_const_closure(class_Matter_StatusResponseMessage_from_TLV_closure) },
        { be_const_key_weak(to_TLV, 2), be_const_closure(class_Matter_StatusResponseMessage_to_TLV_closure) },
        { be_const_key_weak(status, -1), be_const_var(0) },
    })),
    be_str_weak(Matter_StatusResponseMessage)
);

extern const bclass be_class_Matter_ReadRequestMessage;

/********************************************************************
** Solidified function: from_TLV
********************************************************************/
be_local_closure(class_Matter_ReadRequestMessage_from_TLV,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[16]) {     /* constants */
    /* K0   */  be_nested_str_weak(attributes_requests),
    /* K1   */  be_nested_str_weak(from_TLV_array),
    /* K2   */  be_nested_str_weak(findsubval),
    /* K3   */  be_const_int(0),
    /* K4   */  be_nested_str_weak(matter),
    /* K5   */  be_nested_str_weak(AttributePathIB),
    /* K6   */  be_nested_str_weak(event_requests),
    /* K7   */  be_const_int(1),
    /* K8   */  be_nested_str_weak(EventPathIB),
    /* K9   */  be_nested_str_weak(event_filters),
    /* K10  */  be_const_int(2),
    /* K11  */  be_nested_str_weak(EventFilterIB),
    /* K12  */  be_nested_str_weak(fabric_filtered),
    /* K13  */  be_const_int(3),
    /* K14  */  be_nested_str_weak(data_version_filters),
    /* K15  */  be_nested_str_weak(DataVersionFilterIB),
    }),
    be_str_weak(from_TLV),
    &be_const_str_solidified,
    ( &(const binstruction[42]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0001,  //  0002  JMPF	R2	#0005
      0x4C080000,  //  0003  LDNIL	R2
      0x80040400,  //  0004  RET	1	R2
      0x8C080101,  //  0005  GETMET	R2	R0	K1
      0x8C100302,  //  0006  GETMET	R4	R1	K2
      0x58180003,  //  0007  LDCONST	R6	K3
      0x7C100400,  //  0008  CALL	R4	2
      0xB8160800,  //  0009  GETNGBL	R5	K4
      0x88140B05,  //  000A  GETMBR	R5	R5	K5
      0x7C080600,  //  000B  CALL	R2	3
      0x90020002,  //  000C  SETMBR	R0	K0	R2
      0x8C080101,  //  000D  GETMET	R2	R0	K1
      0x8C100302,  //  000E  GETMET	R4	R1	K2
      0x58180007,  //  000F  LDCONST	R6	K7
      0x7C100400,  //  0010  CALL	R4	2
      0xB8160800,  //  0011  GETNGBL	R5	K4
      0x88140B08,  //  0012  GETMBR	R5	R5	K8
      0x7C080600,  //  0013  CALL	R2	3
      0x90020C02,  //  0014  SETMBR	R0	K6	R2
      0x8C080101,  //  0015  GETMET	R2	R0	K1
      0x8C100302,  //  0016  GETMET	R4	R1	K2
      0x5818000A,  //  0017  LDCONST	R6	K10
      0x7C100400,  //  0018  CALL	R4	2
      0xB8160800,  //  0019  GETNGBL	R5	K4
      0x88140B0B,  //  001A  GETMBR	R5	R5	K11
      0x7C080600,  //  001B  CALL	R2	3
      0x90021202,  //  001C  SETMBR	R0	K9	R2
      0x8C080302,  //  001D  GETMET	R2	R1	K2
      0x5810000D,  //  001E  LDCONST	R4	K13
      0x7C080400,  //  001F  CALL	R2	2
      0x90021802,  //  0020  SETMBR	R0	K12	R2
      0x8C080101,  //  0021  GETMET	R2	R0	K1
      0x8C100302,  //  0022  GETMET	R4	R1	K2
      0x541A0003,  //  0023  LDINT	R6	4
      0x7C100400,  //  0024  CALL	R4	2
      0xB8160800,  //  0025  GETNGBL	R5	K4
      0x88140B0F,  //  0026  GETMBR	R5	R5	K15
      0x7C080600,  //  0027  CALL	R2	3
      0x90021C02,  //  0028  SETMBR	R0	K14	R2
      0x80040000,  //  0029  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_ReadRequestMessage
********************************************************************/
extern const bclass be_class_Matter_IM_Message_base;
be_local_class(Matter_ReadRequestMessage,
    5,
    &be_class_Matter_IM_Message_base,
    be_nested_map(6,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(from_TLV, -1), be_const_closure(class_Matter_ReadRequestMessage_from_TLV_closure) },
        { be_const_key_weak(event_filters, -1), be_const_var(2) },
        { be_const_key_weak(fabric_filtered, -1), be_const_var(3) },
        { be_const_key_weak(data_version_filters, -1), be_const_var(4) },
        { be_const_key_weak(event_requests, 3), be_const_var(1) },
        { be_const_key_weak(attributes_requests, -1), be_const_var(0) },
    })),
    be_str_weak(Matter_ReadRequestMessage)
);

extern const bclass be_class_Matter_ReadRequestMessage_solo;

/********************************************************************
** Solidified function: from_raw
********************************************************************/
be_local_closure(class_Matter_ReadRequestMessage_solo_from_raw,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[10]) {     /* constants */
    /* K0   */  be_nested_str_weak(reset),
    /* K1   */  be_nested_str_weak(get),
    /* K2   */  be_const_int(355860503),
    /* K3   */  be_const_int(1),
    /* K4   */  be_const_int(2),
    /* K5   */  be_const_int(3),
    /* K6   */  be_nested_str_weak(endpoint),
    /* K7   */  be_nested_str_weak(cluster),
    /* K8   */  be_nested_str_weak(attribute),
    /* K9   */  be_nested_str_weak(fabric_filtered),
    }),
    be_str_weak(from_raw),
    &be_const_str_solidified,
    ( &(const binstruction[131]) {  /* code */
      0x8C0C0100,  //  0000  GETMET	R3	R0	K0
      0x7C0C0200,  //  0001  CALL	R3	1
      0x600C000C,  //  0002  GETGBL	R3	G12
      0x5C100200,  //  0003  MOVE	R4	R1
      0x7C0C0200,  //  0004  CALL	R3	1
      0x4C100000,  //  0005  LDNIL	R4
      0x8C140301,  //  0006  GETMET	R5	R1	K1
      0x5C1C0400,  //  0007  MOVE	R7	R2
      0x5421FFFB,  //  0008  LDINT	R8	-4
      0x7C140600,  //  0009  CALL	R5	3
      0x20140B02,  //  000A  NE	R5	R5	K2
      0x78160001,  //  000B  JMPF	R5	#000E
      0x4C140000,  //  000C  LDNIL	R5
      0x80040A00,  //  000D  RET	1	R5
      0x54160003,  //  000E  LDINT	R5	4
      0x00080405,  //  000F  ADD	R2	R2	R5
      0x14140403,  //  0010  LT	R5	R2	R3
      0x7816002D,  //  0011  JMPF	R5	#0040
      0x8C140301,  //  0012  GETMET	R5	R1	K1
      0x001C0503,  //  0013  ADD	R7	R2	K3
      0x58200003,  //  0014  LDCONST	R8	K3
      0x7C140600,  //  0015  CALL	R5	3
      0x8C180301,  //  0016  GETMET	R6	R1	K1
      0x5C200400,  //  0017  MOVE	R8	R2
      0x58240003,  //  0018  LDCONST	R9	K3
      0x7C180600,  //  0019  CALL	R6	3
      0x541E0023,  //  001A  LDINT	R7	36
      0x1C1C0C07,  //  001B  EQ	R7	R6	R7
      0x781E0006,  //  001C  JMPF	R7	#0024
      0x8C1C0301,  //  001D  GETMET	R7	R1	K1
      0x00240504,  //  001E  ADD	R9	R2	K4
      0x58280003,  //  001F  LDCONST	R10	K3
      0x7C1C0600,  //  0020  CALL	R7	3
      0x5C100E00,  //  0021  MOVE	R4	R7
      0x00080505,  //  0022  ADD	R2	R2	K5
      0x7002000B,  //  0023  JMP		#0030
      0x541E0024,  //  0024  LDINT	R7	37
      0x1C1C0C07,  //  0025  EQ	R7	R6	R7
      0x781E0007,  //  0026  JMPF	R7	#002F
      0x8C1C0301,  //  0027  GETMET	R7	R1	K1
      0x00240504,  //  0028  ADD	R9	R2	K4
      0x58280004,  //  0029  LDCONST	R10	K4
      0x7C1C0600,  //  002A  CALL	R7	3
      0x5C100E00,  //  002B  MOVE	R4	R7
      0x541E0003,  //  002C  LDINT	R7	4
      0x00080407,  //  002D  ADD	R2	R2	R7
      0x70020000,  //  002E  JMP		#0030
      0x7002000F,  //  002F  JMP		#0040
      0x1C1C0B04,  //  0030  EQ	R7	R5	K4
      0x781E0001,  //  0031  JMPF	R7	#0034
      0x90020C04,  //  0032  SETMBR	R0	K6	R4
      0x7002000A,  //  0033  JMP		#003F
      0x1C1C0B05,  //  0034  EQ	R7	R5	K5
      0x781E0001,  //  0035  JMPF	R7	#0038
      0x90020E04,  //  0036  SETMBR	R0	K7	R4
      0x70020006,  //  0037  JMP		#003F
      0x541E0003,  //  0038  LDINT	R7	4
      0x1C1C0A07,  //  0039  EQ	R7	R5	R7
      0x781E0001,  //  003A  JMPF	R7	#003D
      0x90021004,  //  003B  SETMBR	R0	K8	R4
      0x70020001,  //  003C  JMP		#003F
      0x4C1C0000,  //  003D  LDNIL	R7
      0x80040E00,  //  003E  RET	1	R7
      0x7001FFCF,  //  003F  JMP		#0010
      0x8C140301,  //  0040  GETMET	R5	R1	K1
      0x5C1C0400,  //  0041  MOVE	R7	R2
      0x5421FFFD,  //  0042  LDINT	R8	-2
      0x7C140600,  //  0043  CALL	R5	3
      0x5C100A00,  //  0044  MOVE	R4	R5
      0x54161817,  //  0045  LDINT	R5	6168
      0x20140805,  //  0046  NE	R5	R4	R5
      0x78160001,  //  0047  JMPF	R5	#004A
      0x4C140000,  //  0048  LDNIL	R5
      0x80040A00,  //  0049  RET	1	R5
      0x00080504,  //  004A  ADD	R2	R2	K4
      0x8C140301,  //  004B  GETMET	R5	R1	K1
      0x5C1C0400,  //  004C  MOVE	R7	R2
      0x5421FFFD,  //  004D  LDINT	R8	-2
      0x7C140600,  //  004E  CALL	R5	3
      0x5C100A00,  //  004F  MOVE	R4	R5
      0x54162802,  //  0050  LDINT	R5	10243
      0x1C140805,  //  0051  EQ	R5	R4	R5
      0x78160003,  //  0052  JMPF	R5	#0057
      0x50140000,  //  0053  LDBOOL	R5	0	0
      0x90021205,  //  0054  SETMBR	R0	K9	R5
      0x00080504,  //  0055  ADD	R2	R2	K4
      0x70020005,  //  0056  JMP		#005D
      0x54162902,  //  0057  LDINT	R5	10499
      0x1C140805,  //  0058  EQ	R5	R4	R5
      0x78160002,  //  0059  JMPF	R5	#005D
      0x50140200,  //  005A  LDBOOL	R5	1	0
      0x90021205,  //  005B  SETMBR	R0	K9	R5
      0x00080504,  //  005C  ADD	R2	R2	K4
      0x8C140301,  //  005D  GETMET	R5	R1	K1
      0x5C1C0400,  //  005E  MOVE	R7	R2
      0x5421FFFD,  //  005F  LDINT	R8	-2
      0x7C140600,  //  0060  CALL	R5	3
      0x5C100A00,  //  0061  MOVE	R4	R5
      0x541624FE,  //  0062  LDINT	R5	9471
      0x1C140805,  //  0063  EQ	R5	R4	R5
      0x78160000,  //  0064  JMPF	R5	#0066
      0x00080505,  //  0065  ADD	R2	R2	K5
      0x8C140301,  //  0066  GETMET	R5	R1	K1
      0x5C1C0400,  //  0067  MOVE	R7	R2
      0x58200003,  //  0068  LDCONST	R8	K3
      0x7C140600,  //  0069  CALL	R5	3
      0x541A0017,  //  006A  LDINT	R6	24
      0x20140A06,  //  006B  NE	R5	R5	R6
      0x78160001,  //  006C  JMPF	R5	#006F
      0x4C140000,  //  006D  LDNIL	R5
      0x80040A00,  //  006E  RET	1	R5
      0x00080503,  //  006F  ADD	R2	R2	K3
      0x88140106,  //  0070  GETMBR	R5	R0	K6
      0x4C180000,  //  0071  LDNIL	R6
      0x1C140A06,  //  0072  EQ	R5	R5	R6
      0x7416000B,  //  0073  JMPT	R5	#0080
      0x88140107,  //  0074  GETMBR	R5	R0	K7
      0x4C180000,  //  0075  LDNIL	R6
      0x1C140A06,  //  0076  EQ	R5	R5	R6
      0x74160007,  //  0077  JMPT	R5	#0080
      0x88140108,  //  0078  GETMBR	R5	R0	K8
      0x4C180000,  //  0079  LDNIL	R6
      0x1C140A06,  //  007A  EQ	R5	R5	R6
      0x74160003,  //  007B  JMPT	R5	#0080
      0x88140109,  //  007C  GETMBR	R5	R0	K9
      0x4C180000,  //  007D  LDNIL	R6
      0x1C140A06,  //  007E  EQ	R5	R5	R6
      0x78160001,  //  007F  JMPF	R5	#0082
      0x4C140000,  //  0080  LDNIL	R5
      0x80040A00,  //  0081  RET	1	R5
      0x80040000,  //  0082  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_ReadRequestMessage_solo
********************************************************************/
extern const bclass be_class_Matter_Path;
be_local_class(Matter_ReadRequestMessage_solo,
    0,
    &be_class_Matter_Path,
    be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(from_raw, -1), be_const_closure(class_Matter_ReadRequestMessage_solo_from_raw_closure) },
    })),
    be_str_weak(Matter_ReadRequestMessage_solo)
);

extern const bclass be_class_Matter_ReportDataMessage;

/********************************************************************
** Solidified function: to_TLV
********************************************************************/
be_local_closure(class_Matter_ReportDataMessage_to_TLV,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[18]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(TLV),
    /* K2   */  be_nested_str_weak(Matter_TLV_struct),
    /* K3   */  be_nested_str_weak(add_TLV),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(U4),
    /* K6   */  be_nested_str_weak(subscription_id),
    /* K7   */  be_nested_str_weak(to_TLV_array),
    /* K8   */  be_const_int(1),
    /* K9   */  be_nested_str_weak(attribute_reports),
    /* K10  */  be_const_int(2),
    /* K11  */  be_nested_str_weak(event_reports),
    /* K12  */  be_const_int(3),
    /* K13  */  be_nested_str_weak(BOOL),
    /* K14  */  be_nested_str_weak(more_chunked_messages),
    /* K15  */  be_nested_str_weak(suppress_response),
    /* K16  */  be_nested_str_weak(U1),
    /* K17  */  be_nested_str_weak(InteractionModelRevision),
    }),
    be_str_weak(to_TLV),
    &be_const_str_solidified,
    ( &(const binstruction[35]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x88040301,  //  0001  GETMBR	R1	R1	K1
      0x8C080302,  //  0002  GETMET	R2	R1	K2
      0x7C080200,  //  0003  CALL	R2	1
      0x8C0C0503,  //  0004  GETMET	R3	R2	K3
      0x58140004,  //  0005  LDCONST	R5	K4
      0x88180305,  //  0006  GETMBR	R6	R1	K5
      0x881C0106,  //  0007  GETMBR	R7	R0	K6
      0x7C0C0800,  //  0008  CALL	R3	4
      0x8C0C0107,  //  0009  GETMET	R3	R0	K7
      0x5C140400,  //  000A  MOVE	R5	R2
      0x58180008,  //  000B  LDCONST	R6	K8
      0x881C0109,  //  000C  GETMBR	R7	R0	K9
      0x7C0C0800,  //  000D  CALL	R3	4
      0x8C0C0107,  //  000E  GETMET	R3	R0	K7
      0x5C140400,  //  000F  MOVE	R5	R2
      0x5818000A,  //  0010  LDCONST	R6	K10
      0x881C010B,  //  0011  GETMBR	R7	R0	K11
      0x7C0C0800,  //  0012  CALL	R3	4
      0x8C0C0503,  //  0013  GETMET	R3	R2	K3
      0x5814000C,  //  0014  LDCONST	R5	K12
      0x8818030D,  //  0015  GETMBR	R6	R1	K13
      0x881C010E,  //  0016  GETMBR	R7	R0	K14
      0x7C0C0800,  //  0017  CALL	R3	4
      0x8C0C0503,  //  0018  GETMET	R3	R2	K3
      0x54160003,  //  0019  LDINT	R5	4
      0x8818030D,  //  001A  GETMBR	R6	R1	K13
      0x881C010F,  //  001B  GETMBR	R7	R0	K15
      0x7C0C0800,  //  001C  CALL	R3	4
      0x8C0C0503,  //  001D  GETMET	R3	R2	K3
      0x541600FE,  //  001E  LDINT	R5	255
      0x88180310,  //  001F  GETMBR	R6	R1	K16
      0x881C0111,  //  0020  GETMBR	R7	R0	K17
      0x7C0C0800,  //  0021  CALL	R3	4
      0x80040400,  //  0022  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_ReportDataMessage
********************************************************************/
extern const bclass be_class_Matter_IM_Message_base;
be_local_class(Matter_ReportDataMessage,
    5,
    &be_class_Matter_IM_Message_base,
    be_nested_map(6,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(suppress_response, -1), be_const_var(4) },
        { be_const_key_weak(event_reports, 3), be_const_var(2) },
        { be_const_key_weak(subscription_id, -1), be_const_var(0) },
        { be_const_key_weak(to_TLV, -1), be_const_closure(class_Matter_ReportDataMessage_to_TLV_closure) },
        { be_const_key_weak(more_chunked_messages, -1), be_const_var(3) },
        { be_const_key_weak(attribute_reports, -1), be_const_var(1) },
    })),
    be_str_weak(Matter_ReportDataMessage)
);

extern const bclass be_class_Matter_SubscribeRequestMessage;

/********************************************************************
** Solidified function: from_TLV
********************************************************************/
be_local_closure(class_Matter_SubscribeRequestMessage_from_TLV,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[19]) {     /* constants */
    /* K0   */  be_nested_str_weak(keep_subscriptions),
    /* K1   */  be_nested_str_weak(findsubval),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str_weak(min_interval_floor),
    /* K4   */  be_const_int(1),
    /* K5   */  be_nested_str_weak(max_interval_ceiling),
    /* K6   */  be_const_int(2),
    /* K7   */  be_nested_str_weak(attributes_requests),
    /* K8   */  be_nested_str_weak(from_TLV_array),
    /* K9   */  be_const_int(3),
    /* K10  */  be_nested_str_weak(matter),
    /* K11  */  be_nested_str_weak(AttributePathIB),
    /* K12  */  be_nested_str_weak(event_requests),
    /* K13  */  be_nested_str_weak(EventPathIB),
    /* K14  */  be_nested_str_weak(event_filters),
    /* K15  */  be_nested_str_weak(EventFilterIB),
    /* K16  */  be_nested_str_weak(fabric_filtered),
    /* K17  */  be_nested_str_weak(data_version_filters),
    /* K18  */  be_nested_str_weak(DataVersionFilterIB),
    }),
    be_str_weak(from_TLV),
    &be_const_str_solidified,
    ( &(const binstruction[58]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0001,  //  0002  JMPF	R2	#0005
      0x4C080000,  //  0003  LDNIL	R2
      0x80040400,  //  0004  RET	1	R2
      0x8C080301,  //  0005  GETMET	R2	R1	K1
      0x58100002,  //  0006  LDCONST	R4	K2
      0x50140000,  //  0007  LDBOOL	R5	0	0
      0x7C080600,  //  0008  CALL	R2	3
      0x90020002,  //  0009  SETMBR	R0	K0	R2
      0x8C080301,  //  000A  GETMET	R2	R1	K1
      0x58100004,  //  000B  LDCONST	R4	K4
      0x58140002,  //  000C  LDCONST	R5	K2
      0x7C080600,  //  000D  CALL	R2	3
      0x90020602,  //  000E  SETMBR	R0	K3	R2
      0x8C080301,  //  000F  GETMET	R2	R1	K1
      0x58100006,  //  0010  LDCONST	R4	K6
      0x5416003B,  //  0011  LDINT	R5	60
      0x7C080600,  //  0012  CALL	R2	3
      0x90020A02,  //  0013  SETMBR	R0	K5	R2
      0x8C080108,  //  0014  GETMET	R2	R0	K8
      0x8C100301,  //  0015  GETMET	R4	R1	K1
      0x58180009,  //  0016  LDCONST	R6	K9
      0x7C100400,  //  0017  CALL	R4	2
      0xB8161400,  //  0018  GETNGBL	R5	K10
      0x88140B0B,  //  0019  GETMBR	R5	R5	K11
      0x7C080600,  //  001A  CALL	R2	3
      0x90020E02,  //  001B  SETMBR	R0	K7	R2
      0x8C080108,  //  001C  GETMET	R2	R0	K8
      0x8C100301,  //  001D  GETMET	R4	R1	K1
      0x541A0003,  //  001E  LDINT	R6	4
      0x7C100400,  //  001F  CALL	R4	2
      0xB8161400,  //  0020  GETNGBL	R5	K10
      0x88140B0D,  //  0021  GETMBR	R5	R5	K13
      0x7C080600,  //  0022  CALL	R2	3
      0x90021802,  //  0023  SETMBR	R0	K12	R2
      0x8C080108,  //  0024  GETMET	R2	R0	K8
      0x8C100301,  //  0025  GETMET	R4	R1	K1
      0x541A0004,  //  0026  LDINT	R6	5
      0x7C100400,  //  0027  CALL	R4	2
      0xB8161400,  //  0028  GETNGBL	R5	K10
      0x88140B0F,  //  0029  GETMBR	R5	R5	K15
      0x7C080600,  //  002A  CALL	R2	3
      0x90021C02,  //  002B  SETMBR	R0	K14	R2
      0x8C080301,  //  002C  GETMET	R2	R1	K1
      0x54120006,  //  002D  LDINT	R4	7
      0x50140000,  //  002E  LDBOOL	R5	0	0
      0x7C080600,  //  002F  CALL	R2	3
      0x90022002,  //  0030  SETMBR	R0	K16	R2
      0x8C080108,  //  0031  GETMET	R2	R0	K8
      0x8C100301,  //  0032  GETMET	R4	R1	K1
      0x541A0007,  //  0033  LDINT	R6	8
      0x7C100400,  //  0034  CALL	R4	2
      0xB8161400,  //  0035  GETNGBL	R5	K10
      0x88140B12,  //  0036  GETMBR	R5	R5	K18
      0x7C080600,  //  0037  CALL	R2	3
      0x90022202,  //  0038  SETMBR	R0	K17	R2
      0x80040000,  //  0039  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_SubscribeRequestMessage
********************************************************************/
extern const bclass be_class_Matter_IM_Message_base;
be_local_class(Matter_SubscribeRequestMessage,
    8,
    &be_class_Matter_IM_Message_base,
    be_nested_map(9,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(attributes_requests, -1), be_const_var(3) },
        { be_const_key_weak(event_requests, -1), be_const_var(4) },
        { be_const_key_weak(min_interval_floor, -1), be_const_var(1) },
        { be_const_key_weak(from_TLV, -1), be_const_closure(class_Matter_SubscribeRequestMessage_from_TLV_closure) },
        { be_const_key_weak(event_filters, -1), be_const_var(5) },
        { be_const_key_weak(fabric_filtered, 0), be_const_var(6) },
        { be_const_key_weak(keep_subscriptions, 3), be_const_var(0) },
        { be_const_key_weak(data_version_filters, -1), be_const_var(7) },
        { be_const_key_weak(max_interval_ceiling, -1), be_const_var(2) },
    })),
    be_str_weak(Matter_SubscribeRequestMessage)
);

extern const bclass be_class_Matter_SubscribeResponseMessage;

/********************************************************************
** Solidified function: to_TLV
********************************************************************/
be_local_closure(class_Matter_SubscribeResponseMessage_to_TLV,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[12]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(TLV),
    /* K2   */  be_nested_str_weak(Matter_TLV_struct),
    /* K3   */  be_nested_str_weak(add_TLV),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(U4),
    /* K6   */  be_nested_str_weak(subscription_id),
    /* K7   */  be_const_int(2),
    /* K8   */  be_nested_str_weak(U2),
    /* K9   */  be_nested_str_weak(max_interval),
    /* K10  */  be_nested_str_weak(U1),
    /* K11  */  be_nested_str_weak(InteractionModelRevision),
    }),
    be_str_weak(to_TLV),
    &be_const_str_solidified,
    ( &(const binstruction[20]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x88040301,  //  0001  GETMBR	R1	R1	K1
      0x8C080302,  //  0002  GETMET	R2	R1	K2
      0x7C080200,  //  0003  CALL	R2	1
      0x8C0C0503,  //  0004  GETMET	R3	R2	K3
      0x58140004,  //  0005  LDCONST	R5	K4
      0x88180305,  //  0006  GETMBR	R6	R1	K5
      0x881C0106,  //  0007  GETMBR	R7	R0	K6
      0x7C0C0800,  //  0008  CALL	R3	4
      0x8C0C0503,  //  0009  GETMET	R3	R2	K3
      0x58140007,  //  000A  LDCONST	R5	K7
      0x88180308,  //  000B  GETMBR	R6	R1	K8
      0x881C0109,  //  000C  GETMBR	R7	R0	K9
      0x7C0C0800,  //  000D  CALL	R3	4
      0x8C0C0503,  //  000E  GETMET	R3	R2	K3
      0x541600FE,  //  000F  LDINT	R5	255
      0x8818030A,  //  0010  GETMBR	R6	R1	K10
      0x881C010B,  //  0011  GETMBR	R7	R0	K11
      0x7C0C0800,  //  0012  CALL	R3	4
      0x80040400,  //  0013  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_SubscribeResponseMessage
********************************************************************/
extern const bclass be_class_Matter_IM_Message_base;
be_local_class(Matter_SubscribeResponseMessage,
    2,
    &be_class_Matter_IM_Message_base,
    be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(max_interval, -1), be_const_var(1) },
        { be_const_key_weak(to_TLV, -1), be_const_closure(class_Matter_SubscribeResponseMessage_to_TLV_closure) },
        { be_const_key_weak(subscription_id, -1), be_const_var(0) },
    })),
    be_str_weak(Matter_SubscribeResponseMessage)
);

extern const bclass be_class_Matter_WriteRequestMessage;

/********************************************************************
** Solidified function: from_TLV
********************************************************************/
be_local_closure(class_Matter_WriteRequestMessage_from_TLV,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[12]) {     /* constants */
    /* K0   */  be_nested_str_weak(suppress_response),
    /* K1   */  be_nested_str_weak(findsubval),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str_weak(timed_request),
    /* K4   */  be_const_int(1),
    /* K5   */  be_nested_str_weak(write_requests),
    /* K6   */  be_nested_str_weak(from_TLV_array),
    /* K7   */  be_const_int(2),
    /* K8   */  be_nested_str_weak(matter),
    /* K9   */  be_nested_str_weak(AttributeDataIB),
    /* K10  */  be_nested_str_weak(more_chunked_messages),
    /* K11  */  be_const_int(3),
    }),
    be_str_weak(from_TLV),
    &be_const_str_solidified,
    ( &(const binstruction[26]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0001,  //  0002  JMPF	R2	#0005
      0x4C080000,  //  0003  LDNIL	R2
      0x80040400,  //  0004  RET	1	R2
      0x8C080301,  //  0005  GETMET	R2	R1	K1
      0x58100002,  //  0006  LDCONST	R4	K2
      0x7C080400,  //  0007  CALL	R2	2
      0x90020002,  //  0008  SETMBR	R0	K0	R2
      0x8C080301,  //  0009  GETMET	R2	R1	K1
      0x58100004,  //  000A  LDCONST	R4	K4
      0x7C080400,  //  000B  CALL	R2	2
      0x90020602,  //  000C  SETMBR	R0	K3	R2
      0x8C080106,  //  000D  GETMET	R2	R0	K6
      0x8C100301,  //  000E  GETMET	R4	R1	K1
      0x58180007,  //  000F  LDCONST	R6	K7
      0x7C100400,  //  0010  CALL	R4	2
      0xB8161000,  //  0011  GETNGBL	R5	K8
      0x88140B09,  //  0012  GETMBR	R5	R5	K9
      0x7C080600,  //  0013  CALL	R2	3
      0x90020A02,  //  0014  SETMBR	R0	K5	R2
      0x8C080301,  //  0015  GETMET	R2	R1	K1
      0x5810000B,  //  0016  LDCONST	R4	K11
      0x7C080400,  //  0017  CALL	R2	2
      0x90021402,  //  0018  SETMBR	R0	K10	R2
      0x80040000,  //  0019  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_WriteRequestMessage
********************************************************************/
extern const bclass be_class_Matter_IM_Message_base;
be_local_class(Matter_WriteRequestMessage,
    4,
    &be_class_Matter_IM_Message_base,
    be_nested_map(5,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(write_requests, -1), be_const_var(2) },
        { be_const_key_weak(suppress_response, 2), be_const_var(0) },
        { be_const_key_weak(timed_request, -1), be_const_var(1) },
        { be_const_key_weak(from_TLV, -1), be_const_closure(class_Matter_WriteRequestMessage_from_TLV_closure) },
        { be_const_key_weak(more_chunked_messages, -1), be_const_var(3) },
    })),
    be_str_weak(Matter_WriteRequestMessage)
);

extern const bclass be_class_Matter_WriteResponseMessage;

/********************************************************************
** Solidified function: to_TLV
********************************************************************/
be_local_closure(class_Matter_WriteResponseMessage_to_TLV,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(TLV),
    /* K2   */  be_nested_str_weak(Matter_TLV_struct),
    /* K3   */  be_nested_str_weak(to_TLV_array),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(write_responses),
    /* K6   */  be_nested_str_weak(add_TLV),
    /* K7   */  be_nested_str_weak(U1),
    /* K8   */  be_nested_str_weak(InteractionModelRevision),
    }),
    be_str_weak(to_TLV),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x88040301,  //  0001  GETMBR	R1	R1	K1
      0x8C080302,  //  0002  GETMET	R2	R1	K2
      0x7C080200,  //  0003  CALL	R2	1
      0x8C0C0103,  //  0004  GETMET	R3	R0	K3
      0x5C140400,  //  0005  MOVE	R5	R2
      0x58180004,  //  0006  LDCONST	R6	K4
      0x881C0105,  //  0007  GETMBR	R7	R0	K5
      0x7C0C0800,  //  0008  CALL	R3	4
      0x8C0C0506,  //  0009  GETMET	R3	R2	K6
      0x541600FE,  //  000A  LDINT	R5	255
      0x88180307,  //  000B  GETMBR	R6	R1	K7
      0x881C0108,  //  000C  GETMBR	R7	R0	K8
      0x7C0C0800,  //  000D  CALL	R3	4
      0x80040400,  //  000E  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_WriteResponseMessage
********************************************************************/
extern const bclass be_class_Matter_IM_Message_base;
be_local_class(Matter_WriteResponseMessage,
    1,
    &be_class_Matter_IM_Message_base,
    be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(to_TLV, -1), be_const_closure(class_Matter_WriteResponseMessage_to_TLV_closure) },
        { be_const_key_weak(write_responses, 0), be_const_var(0) },
    })),
    be_str_weak(Matter_WriteResponseMessage)
);

extern const bclass be_class_Matter_TimedRequestMessage;

/********************************************************************
** Solidified function: from_TLV
********************************************************************/
be_local_closure(class_Matter_TimedRequestMessage_from_TLV,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(timeout),
    /* K1   */  be_nested_str_weak(findsubval),
    /* K2   */  be_const_int(0),
    }),
    be_str_weak(from_TLV),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0001,  //  0002  JMPF	R2	#0005
      0x4C080000,  //  0003  LDNIL	R2
      0x80040400,  //  0004  RET	1	R2
      0x8C080301,  //  0005  GETMET	R2	R1	K1
      0x58100002,  //  0006  LDCONST	R4	K2
      0x7C080400,  //  0007  CALL	R2	2
      0x90020002,  //  0008  SETMBR	R0	K0	R2
      0x80040000,  //  0009  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_TimedRequestMessage
********************************************************************/
extern const bclass be_class_Matter_IM_Message_base;
be_local_class(Matter_TimedRequestMessage,
    1,
    &be_class_Matter_IM_Message_base,
    be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(timeout, 1), be_const_var(0) },
        { be_const_key_weak(from_TLV, -1), be_const_closure(class_Matter_TimedRequestMessage_from_TLV_closure) },
    })),
    be_str_weak(Matter_TimedRequestMessage)
);

extern const bclass be_class_Matter_InvokeRequestMessage;

/********************************************************************
** Solidified function: from_TLV
********************************************************************/
be_local_closure(class_Matter_InvokeRequestMessage_from_TLV,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[10]) {     /* constants */
    /* K0   */  be_nested_str_weak(suppress_response),
    /* K1   */  be_nested_str_weak(findsubval),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str_weak(timed_request),
    /* K4   */  be_const_int(1),
    /* K5   */  be_nested_str_weak(invoke_requests),
    /* K6   */  be_nested_str_weak(from_TLV_array),
    /* K7   */  be_const_int(2),
    /* K8   */  be_nested_str_weak(matter),
    /* K9   */  be_nested_str_weak(CommandDataIB),
    }),
    be_str_weak(from_TLV),
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0001,  //  0002  JMPF	R2	#0005
      0x4C080000,  //  0003  LDNIL	R2
      0x80040400,  //  0004  RET	1	R2
      0x8C080301,  //  0005  GETMET	R2	R1	K1
      0x58100002,  //  0006  LDCONST	R4	K2
      0x7C080400,  //  0007  CALL	R2	2
      0x90020002,  //  0008  SETMBR	R0	K0	R2
      0x8C080301,  //  0009  GETMET	R2	R1	K1
      0x58100004,  //  000A  LDCONST	R4	K4
      0x7C080400,  //  000B  CALL	R2	2
      0x90020602,  //  000C  SETMBR	R0	K3	R2
      0x8C080106,  //  000D  GETMET	R2	R0	K6
      0x8C100301,  //  000E  GETMET	R4	R1	K1
      0x58180007,  //  000F  LDCONST	R6	K7
      0x7C100400,  //  0010  CALL	R4	2
      0xB8161000,  //  0011  GETNGBL	R5	K8
      0x88140B09,  //  0012  GETMBR	R5	R5	K9
      0x7C080600,  //  0013  CALL	R2	3
      0x90020A02,  //  0014  SETMBR	R0	K5	R2
      0x80040000,  //  0015  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_InvokeRequestMessage
********************************************************************/
extern const bclass be_class_Matter_IM_Message_base;
be_local_class(Matter_InvokeRequestMessage,
    3,
    &be_class_Matter_IM_Message_base,
    be_nested_map(4,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(suppress_response, 1), be_const_var(0) },
        { be_const_key_weak(from_TLV, 3), be_const_closure(class_Matter_InvokeRequestMessage_from_TLV_closure) },
        { be_const_key_weak(timed_request, -1), be_const_var(1) },
        { be_const_key_weak(invoke_requests, -1), be_const_var(2) },
    })),
    be_str_weak(Matter_InvokeRequestMessage)
);
// compact class 'Matter_InvokeRequestMessage_solo' ktab size: 17, total: 20 (saved 24 bytes)
static const bvalue be_ktab_class_Matter_InvokeRequestMessage_solo[17] = {
  /* K0   */  be_nested_str_weak(reset),
  /* K1   */  be_nested_str_weak(SuppressResponse),
  /* K2   */  be_nested_str_weak(TimedRequest),
  /* K3   */  be_nested_str_weak(command_fields),
  /* K4   */  be_nested_str_weak(get),
  /* K5   */  be_const_int(1),
  /* K6   */  be_const_int(2),
  /* K7   */  be_const_int(3),
  /* K8   */  be_const_int(0),
  /* K9   */  be_nested_str_weak(endpoint),
  /* K10  */  be_nested_str_weak(cluster),
  /* K11  */  be_nested_str_weak(command),
  /* K12  */  be_nested_str_weak(matter),
  /* K13  */  be_nested_str_weak(TLV),
  /* K14  */  be_nested_str_weak(parse),
  /* K15  */  be_nested_str_weak(next_idx),
  /* K16  */  be_const_int(620691736),
};


extern const bclass be_class_Matter_InvokeRequestMessage_solo;

/********************************************************************
** Solidified function: reset
********************************************************************/
be_local_closure(class_Matter_InvokeRequestMessage_solo_reset,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_InvokeRequestMessage_solo,     /* shared constants */
    be_str_weak(reset),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x4C040000,  //  0000  LDNIL	R1
      0x60080003,  //  0001  GETGBL	R2	G3
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C080200,  //  0003  CALL	R2	1
      0x8C080500,  //  0004  GETMET	R2	R2	K0
      0x7C080200,  //  0005  CALL	R2	1
      0x90020201,  //  0006  SETMBR	R0	K1	R1
      0x90020401,  //  0007  SETMBR	R0	K2	R1
      0x90020601,  //  0008  SETMBR	R0	K3	R1
      0x80000000,  //  0009  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: from_raw
********************************************************************/
be_local_closure(class_Matter_InvokeRequestMessage_solo_from_raw,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_InvokeRequestMessage_solo,     /* shared constants */
    be_str_weak(from_raw),
    &be_const_str_solidified,
    ( &(const binstruction[170]) {  /* code */
      0x8C0C0100,  //  0000  GETMET	R3	R0	K0
      0x7C0C0200,  //  0001  CALL	R3	1
      0x600C000C,  //  0002  GETGBL	R3	G12
      0x5C100200,  //  0003  MOVE	R4	R1
      0x7C0C0200,  //  0004  CALL	R3	1
      0x4C100000,  //  0005  LDNIL	R4
      0x8C140304,  //  0006  GETMET	R5	R1	K4
      0x5C1C0400,  //  0007  MOVE	R7	R2
      0x58200005,  //  0008  LDCONST	R8	K5
      0x7C140600,  //  0009  CALL	R5	3
      0x541A0014,  //  000A  LDINT	R6	21
      0x20140A06,  //  000B  NE	R5	R5	R6
      0x78160001,  //  000C  JMPF	R5	#000F
      0x4C140000,  //  000D  LDNIL	R5
      0x80040A00,  //  000E  RET	1	R5
      0x00080505,  //  000F  ADD	R2	R2	K5
      0x8C140304,  //  0010  GETMET	R5	R1	K4
      0x5C1C0400,  //  0011  MOVE	R7	R2
      0x5421FFFD,  //  0012  LDINT	R8	-2
      0x7C140600,  //  0013  CALL	R5	3
      0x5C100A00,  //  0014  MOVE	R4	R5
      0x541627FF,  //  0015  LDINT	R5	10240
      0x1C140805,  //  0016  EQ	R5	R4	R5
      0x74160002,  //  0017  JMPT	R5	#001B
      0x541628FF,  //  0018  LDINT	R5	10496
      0x1C140805,  //  0019  EQ	R5	R4	R5
      0x78160003,  //  001A  JMPF	R5	#001F
      0x541628FF,  //  001B  LDINT	R5	10496
      0x1C140805,  //  001C  EQ	R5	R4	R5
      0x90020205,  //  001D  SETMBR	R0	K1	R5
      0x00080506,  //  001E  ADD	R2	R2	K6
      0x8C140304,  //  001F  GETMET	R5	R1	K4
      0x5C1C0400,  //  0020  MOVE	R7	R2
      0x5421FFFD,  //  0021  LDINT	R8	-2
      0x7C140600,  //  0022  CALL	R5	3
      0x5C100A00,  //  0023  MOVE	R4	R5
      0x54162800,  //  0024  LDINT	R5	10241
      0x1C140805,  //  0025  EQ	R5	R4	R5
      0x74160002,  //  0026  JMPT	R5	#002A
      0x54162900,  //  0027  LDINT	R5	10497
      0x1C140805,  //  0028  EQ	R5	R4	R5
      0x78160003,  //  0029  JMPF	R5	#002E
      0x54162900,  //  002A  LDINT	R5	10497
      0x1C140805,  //  002B  EQ	R5	R4	R5
      0x90020205,  //  002C  SETMBR	R0	K1	R5
      0x00080506,  //  002D  ADD	R2	R2	K6
      0x8C140304,  //  002E  GETMET	R5	R1	K4
      0x5C1C0400,  //  002F  MOVE	R7	R2
      0x5421FFFD,  //  0030  LDINT	R8	-2
      0x7C140600,  //  0031  CALL	R5	3
      0x541A3601,  //  0032  LDINT	R6	13826
      0x20140A06,  //  0033  NE	R5	R5	R6
      0x78160001,  //  0034  JMPF	R5	#0037
      0x4C140000,  //  0035  LDNIL	R5
      0x80040A00,  //  0036  RET	1	R5
      0x00080506,  //  0037  ADD	R2	R2	K6
      0x8C140304,  //  0038  GETMET	R5	R1	K4
      0x5C1C0400,  //  0039  MOVE	R7	R2
      0x58200005,  //  003A  LDCONST	R8	K5
      0x7C140600,  //  003B  CALL	R5	3
      0x541A0014,  //  003C  LDINT	R6	21
      0x20140A06,  //  003D  NE	R5	R5	R6
      0x78160001,  //  003E  JMPF	R5	#0041
      0x4C140000,  //  003F  LDNIL	R5
      0x80040A00,  //  0040  RET	1	R5
      0x00080505,  //  0041  ADD	R2	R2	K5
      0x8C140304,  //  0042  GETMET	R5	R1	K4
      0x5C1C0400,  //  0043  MOVE	R7	R2
      0x5421FFFD,  //  0044  LDINT	R8	-2
      0x7C140600,  //  0045  CALL	R5	3
      0x541A36FF,  //  0046  LDINT	R6	14080
      0x20140A06,  //  0047  NE	R5	R5	R6
      0x78160001,  //  0048  JMPF	R5	#004B
      0x4C140000,  //  0049  LDNIL	R5
      0x80040A00,  //  004A  RET	1	R5
      0x00080506,  //  004B  ADD	R2	R2	K6
      0x14140403,  //  004C  LT	R5	R2	R3
      0x7816002C,  //  004D  JMPF	R5	#007B
      0x8C140304,  //  004E  GETMET	R5	R1	K4
      0x001C0505,  //  004F  ADD	R7	R2	K5
      0x58200005,  //  0050  LDCONST	R8	K5
      0x7C140600,  //  0051  CALL	R5	3
      0x8C180304,  //  0052  GETMET	R6	R1	K4
      0x5C200400,  //  0053  MOVE	R8	R2
      0x58240005,  //  0054  LDCONST	R9	K5
      0x7C180600,  //  0055  CALL	R6	3
      0x541E0023,  //  0056  LDINT	R7	36
      0x1C1C0C07,  //  0057  EQ	R7	R6	R7
      0x781E0006,  //  0058  JMPF	R7	#0060
      0x8C1C0304,  //  0059  GETMET	R7	R1	K4
      0x00240506,  //  005A  ADD	R9	R2	K6
      0x58280005,  //  005B  LDCONST	R10	K5
      0x7C1C0600,  //  005C  CALL	R7	3
      0x5C100E00,  //  005D  MOVE	R4	R7
      0x00080507,  //  005E  ADD	R2	R2	K7
      0x7002000B,  //  005F  JMP		#006C
      0x541E0024,  //  0060  LDINT	R7	37
      0x1C1C0C07,  //  0061  EQ	R7	R6	R7
      0x781E0007,  //  0062  JMPF	R7	#006B
      0x8C1C0304,  //  0063  GETMET	R7	R1	K4
      0x00240506,  //  0064  ADD	R9	R2	K6
      0x58280006,  //  0065  LDCONST	R10	K6
      0x7C1C0600,  //  0066  CALL	R7	3
      0x5C100E00,  //  0067  MOVE	R4	R7
      0x541E0003,  //  0068  LDINT	R7	4
      0x00080407,  //  0069  ADD	R2	R2	R7
      0x70020000,  //  006A  JMP		#006C
      0x7002000E,  //  006B  JMP		#007B
      0x1C1C0B08,  //  006C  EQ	R7	R5	K8
      0x781E0001,  //  006D  JMPF	R7	#0070
      0x90021204,  //  006E  SETMBR	R0	K9	R4
      0x70020009,  //  006F  JMP		#007A
      0x1C1C0B05,  //  0070  EQ	R7	R5	K5
      0x781E0001,  //  0071  JMPF	R7	#0074
      0x90021404,  //  0072  SETMBR	R0	K10	R4
      0x70020005,  //  0073  JMP		#007A
      0x1C1C0B06,  //  0074  EQ	R7	R5	K6
      0x781E0001,  //  0075  JMPF	R7	#0078
      0x90021604,  //  0076  SETMBR	R0	K11	R4
      0x70020001,  //  0077  JMP		#007A
      0x4C1C0000,  //  0078  LDNIL	R7
      0x80040E00,  //  0079  RET	1	R7
      0x7001FFD0,  //  007A  JMP		#004C
      0x8C140304,  //  007B  GETMET	R5	R1	K4
      0x5C1C0400,  //  007C  MOVE	R7	R2
      0x58200005,  //  007D  LDCONST	R8	K5
      0x7C140600,  //  007E  CALL	R5	3
      0x541A0017,  //  007F  LDINT	R6	24
      0x20140A06,  //  0080  NE	R5	R5	R6
      0x78160001,  //  0081  JMPF	R5	#0084
      0x4C140000,  //  0082  LDNIL	R5
      0x80040A00,  //  0083  RET	1	R5
      0x00080505,  //  0084  ADD	R2	R2	K5
      0x8C140304,  //  0085  GETMET	R5	R1	K4
      0x5C1C0400,  //  0086  MOVE	R7	R2
      0x5421FFFD,  //  0087  LDINT	R8	-2
      0x7C140600,  //  0088  CALL	R5	3
      0x541A3500,  //  0089  LDINT	R6	13569
      0x20140A06,  //  008A  NE	R5	R5	R6
      0x78160001,  //  008B  JMPF	R5	#008E
      0x4C140000,  //  008C  LDNIL	R5
      0x80040A00,  //  008D  RET	1	R5
      0xB8161800,  //  008E  GETNGBL	R5	K12
      0x88140B0D,  //  008F  GETMBR	R5	R5	K13
      0x8C140B0E,  //  0090  GETMET	R5	R5	K14
      0x5C1C0200,  //  0091  MOVE	R7	R1
      0x5C200400,  //  0092  MOVE	R8	R2
      0x7C140600,  //  0093  CALL	R5	3
      0x90020605,  //  0094  SETMBR	R0	K3	R5
      0x88140103,  //  0095  GETMBR	R5	R0	K3
      0x88080B0F,  //  0096  GETMBR	R2	R5	K15
      0x8C180304,  //  0097  GETMET	R6	R1	K4
      0x5C200400,  //  0098  MOVE	R8	R2
      0x5425FFFD,  //  0099  LDINT	R9	-2
      0x7C180600,  //  009A  CALL	R6	3
      0x541E1817,  //  009B  LDINT	R7	6168
      0x20180C07,  //  009C  NE	R6	R6	R7
      0x781A0001,  //  009D  JMPF	R6	#00A0
      0x4C180000,  //  009E  LDNIL	R6
      0x80040C00,  //  009F  RET	1	R6
      0x00080506,  //  00A0  ADD	R2	R2	K6
      0x8C140304,  //  00A1  GETMET	R5	R1	K4
      0x5C1C0400,  //  00A2  MOVE	R7	R2
      0x5421FFFB,  //  00A3  LDINT	R8	-4
      0x7C140600,  //  00A4  CALL	R5	3
      0x20140B10,  //  00A5  NE	R5	R5	K16
      0x78160001,  //  00A6  JMPF	R5	#00A9
      0x4C140000,  //  00A7  LDNIL	R5
      0x80040A00,  //  00A8  RET	1	R5
      0x80040000,  //  00A9  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_InvokeRequestMessage_solo
********************************************************************/
extern const bclass be_class_Matter_Path;
be_local_class(Matter_InvokeRequestMessage_solo,
    3,
    &be_class_Matter_Path,
    be_nested_map(5,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(SuppressResponse, 4), be_const_var(0) },
        { be_const_key_weak(command_fields, 3), be_const_var(2) },
        { be_const_key_weak(reset, -1), be_const_closure(class_Matter_InvokeRequestMessage_solo_reset_closure) },
        { be_const_key_weak(from_raw, -1), be_const_closure(class_Matter_InvokeRequestMessage_solo_from_raw_closure) },
        { be_const_key_weak(TimedRequest, -1), be_const_var(1) },
    })),
    be_str_weak(Matter_InvokeRequestMessage_solo)
);

extern const bclass be_class_Matter_InvokeResponseMessage;

/********************************************************************
** Solidified function: to_TLV
********************************************************************/
be_local_closure(class_Matter_InvokeResponseMessage_to_TLV,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[12]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(TLV),
    /* K2   */  be_nested_str_weak(Matter_TLV_struct),
    /* K3   */  be_nested_str_weak(add_TLV),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(BOOL),
    /* K6   */  be_nested_str_weak(suppress_response),
    /* K7   */  be_nested_str_weak(to_TLV_array),
    /* K8   */  be_const_int(1),
    /* K9   */  be_nested_str_weak(invoke_responses),
    /* K10  */  be_nested_str_weak(U1),
    /* K11  */  be_nested_str_weak(InteractionModelRevision),
    }),
    be_str_weak(to_TLV),
    &be_const_str_solidified,
    ( &(const binstruction[20]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x88040301,  //  0001  GETMBR	R1	R1	K1
      0x8C080302,  //  0002  GETMET	R2	R1	K2
      0x7C080200,  //  0003  CALL	R2	1
      0x8C0C0503,  //  0004  GETMET	R3	R2	K3
      0x58140004,  //  0005  LDCONST	R5	K4
      0x88180305,  //  0006  GETMBR	R6	R1	K5
      0x881C0106,  //  0007  GETMBR	R7	R0	K6
      0x7C0C0800,  //  0008  CALL	R3	4
      0x8C0C0107,  //  0009  GETMET	R3	R0	K7
      0x5C140400,  //  000A  MOVE	R5	R2
      0x58180008,  //  000B  LDCONST	R6	K8
      0x881C0109,  //  000C  GETMBR	R7	R0	K9
      0x7C0C0800,  //  000D  CALL	R3	4
      0x8C0C0503,  //  000E  GETMET	R3	R2	K3
      0x541600FE,  //  000F  LDINT	R5	255
      0x8818030A,  //  0010  GETMBR	R6	R1	K10
      0x881C010B,  //  0011  GETMBR	R7	R0	K11
      0x7C0C0800,  //  0012  CALL	R3	4
      0x80040400,  //  0013  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_InvokeResponseMessage
********************************************************************/
extern const bclass be_class_Matter_IM_Message_base;
be_local_class(Matter_InvokeResponseMessage,
    2,
    &be_class_Matter_IM_Message_base,
    be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(suppress_response, -1), be_const_var(0) },
        { be_const_key_weak(to_TLV, -1), be_const_closure(class_Matter_InvokeResponseMessage_to_TLV_closure) },
        { be_const_key_weak(invoke_responses, -1), be_const_var(1) },
    })),
    be_str_weak(Matter_InvokeResponseMessage)
);
/********************************************************************/
/* End of solidification */
