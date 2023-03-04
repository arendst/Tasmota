/* Solidification of Matter_IM_Data.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_IM_base;

/********************************************************************
** Solidified function: to_TLV_array
********************************************************************/
be_local_closure(Matter_IM_base_to_TLV_array,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(add_array),
    /* K1   */  be_nested_str_weak(add_obj),
    /* K2   */  be_nested_str_weak(stop_iteration),
    }),
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
be_local_closure(Matter_IM_base_tostring,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(_X3C),
    /* K1   */  be_nested_str_weak(_X3A),
    /* K2   */  be_nested_str_weak(matter),
    /* K3   */  be_nested_str_weak(inspect),
    /* K4   */  be_nested_str_weak(_X3E),
    }),
    be_str_weak(tostring),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x60040005,  //  0000  GETGBL	R1	G5
      0x5C080000,  //  0001  MOVE	R2	R0
      0x7C040200,  //  0002  CALL	R1	1
      0x00060001,  //  0003  ADD	R1	K0	R1
      0x00040301,  //  0004  ADD	R1	R1	K1
      0xB80A0400,  //  0005  GETNGBL	R2	K2
      0x8C080503,  //  0006  GETMET	R2	R2	K3
      0x5C100000,  //  0007  MOVE	R4	R0
      0x7C080400,  //  0008  CALL	R2	2
      0x00040202,  //  0009  ADD	R1	R1	R2
      0x00040304,  //  000A  ADD	R1	R1	K4
      0x80040200,  //  000B  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: from_TLV_array
********************************************************************/
be_local_closure(Matter_IM_base_from_TLV_array,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(push),
    /* K1   */  be_nested_str_weak(from_TLV),
    /* K2   */  be_nested_str_weak(stop_iteration),
    }),
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
      0x8C180700,  //  000D  GETMET	R6	R3	K0
      0x5C200400,  //  000E  MOVE	R8	R2
      0x7C200000,  //  000F  CALL	R8	0
      0x8C201101,  //  0010  GETMET	R8	R8	K1
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
        { be_const_key_weak(from_TLV_array, -1), be_const_closure(Matter_IM_base_from_TLV_array_closure) },
        { be_const_key_weak(tostring, -1), be_const_closure(Matter_IM_base_tostring_closure) },
        { be_const_key_weak(to_TLV_array, 0), be_const_closure(Matter_IM_base_to_TLV_array_closure) },
    })),
    be_str_weak(Matter_IM_base)
);
/*******************************************************************/

void be_load_Matter_IM_base_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_IM_base);
    be_setglobal(vm, "Matter_IM_base");
    be_pop(vm, 1);
}

extern const bclass be_class_Matter_IM_Message_base;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_IM_Message_base_init,   /* name */
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
        { be_const_key_weak(init, -1), be_const_closure(Matter_IM_Message_base_init_closure) },
        { be_const_key_weak(InteractionModelRevision, 0), be_const_var(0) },
    })),
    be_str_weak(Matter_IM_Message_base)
);
/*******************************************************************/

void be_load_Matter_IM_Message_base_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_IM_Message_base);
    be_setglobal(vm, "Matter_IM_Message_base");
    be_pop(vm, 1);
}

extern const bclass be_class_Matter_AttributePathIB;

/********************************************************************
** Solidified function: to_TLV
********************************************************************/
be_local_closure(Matter_AttributePathIB_to_TLV,   /* name */
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
be_local_closure(Matter_AttributePathIB_tostring,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[16]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(),
    /* K2   */  be_nested_str_weak(node),
    /* K3   */  be_nested_str_weak(format),
    /* K4   */  be_nested_str_weak(node_X3D_X25s_X20),
    /* K5   */  be_nested_str_weak(endpoint),
    /* K6   */  be_nested_str_weak(_X5B_X2502X_X5D),
    /* K7   */  be_nested_str_weak(_X5B_X2A_X2A_X5D),
    /* K8   */  be_nested_str_weak(cluster),
    /* K9   */  be_nested_str_weak(_X2504X_X2F),
    /* K10  */  be_nested_str_weak(_X2A_X2A_X2A_X2A_X2F),
    /* K11  */  be_nested_str_weak(attribute),
    /* K12  */  be_nested_str_weak(_X2504X),
    /* K13  */  be_nested_str_weak(_X2A_X2A_X2A_X2A),
    /* K14  */  be_nested_str_weak(Exception_X3E_X20),
    /* K15  */  be_nested_str_weak(_X2C_X20),
    }),
    be_str_weak(tostring),
    &be_const_str_solidified,
    ( &(const binstruction[62]) {  /* code */
      0xA802002D,  //  0000  EXBLK	0	#002F
      0xA4060000,  //  0001  IMPORT	R1	K0
      0x58080001,  //  0002  LDCONST	R2	K1
      0x880C0102,  //  0003  GETMBR	R3	R0	K2
      0x780E0004,  //  0004  JMPF	R3	#000A
      0x8C0C0303,  //  0005  GETMET	R3	R1	K3
      0x58140004,  //  0006  LDCONST	R5	K4
      0x88180102,  //  0007  GETMBR	R6	R0	K2
      0x7C0C0600,  //  0008  CALL	R3	3
      0x00080403,  //  0009  ADD	R2	R2	R3
      0x880C0105,  //  000A  GETMBR	R3	R0	K5
      0x4C100000,  //  000B  LDNIL	R4
      0x200C0604,  //  000C  NE	R3	R3	R4
      0x780E0004,  //  000D  JMPF	R3	#0013
      0x8C0C0303,  //  000E  GETMET	R3	R1	K3
      0x58140006,  //  000F  LDCONST	R5	K6
      0x88180105,  //  0010  GETMBR	R6	R0	K5
      0x7C0C0600,  //  0011  CALL	R3	3
      0x70020000,  //  0012  JMP		#0014
      0x580C0007,  //  0013  LDCONST	R3	K7
      0x00080403,  //  0014  ADD	R2	R2	R3
      0x880C0108,  //  0015  GETMBR	R3	R0	K8
      0x4C100000,  //  0016  LDNIL	R4
      0x200C0604,  //  0017  NE	R3	R3	R4
      0x780E0004,  //  0018  JMPF	R3	#001E
      0x8C0C0303,  //  0019  GETMET	R3	R1	K3
      0x58140009,  //  001A  LDCONST	R5	K9
      0x88180108,  //  001B  GETMBR	R6	R0	K8
      0x7C0C0600,  //  001C  CALL	R3	3
      0x70020000,  //  001D  JMP		#001F
      0x580C000A,  //  001E  LDCONST	R3	K10
      0x00080403,  //  001F  ADD	R2	R2	R3
      0x880C010B,  //  0020  GETMBR	R3	R0	K11
      0x4C100000,  //  0021  LDNIL	R4
      0x200C0604,  //  0022  NE	R3	R3	R4
      0x780E0004,  //  0023  JMPF	R3	#0029
      0x8C0C0303,  //  0024  GETMET	R3	R1	K3
      0x5814000C,  //  0025  LDCONST	R5	K12
      0x8818010B,  //  0026  GETMBR	R6	R0	K11
      0x7C0C0600,  //  0027  CALL	R3	3
      0x70020000,  //  0028  JMP		#002A
      0x580C000D,  //  0029  LDCONST	R3	K13
      0x00080403,  //  002A  ADD	R2	R2	R3
      0xA8040001,  //  002B  EXBLK	1	1
      0x80040400,  //  002C  RET	1	R2
      0xA8040001,  //  002D  EXBLK	1	1
      0x7002000D,  //  002E  JMP		#003D
      0xAC040002,  //  002F  CATCH	R1	0	2
      0x7002000A,  //  0030  JMP		#003C
      0x600C0008,  //  0031  GETGBL	R3	G8
      0x5C100200,  //  0032  MOVE	R4	R1
      0x7C0C0200,  //  0033  CALL	R3	1
      0x000E1C03,  //  0034  ADD	R3	K14	R3
      0x000C070F,  //  0035  ADD	R3	R3	K15
      0x60100008,  //  0036  GETGBL	R4	G8
      0x5C140400,  //  0037  MOVE	R5	R2
      0x7C100200,  //  0038  CALL	R4	1
      0x000C0604,  //  0039  ADD	R3	R3	R4
      0x80040600,  //  003A  RET	1	R3
      0x70020000,  //  003B  JMP		#003D
      0xB0080000,  //  003C  RAISE	2	R0	R0
      0x80000000,  //  003D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: from_TLV
********************************************************************/
be_local_closure(Matter_AttributePathIB_from_TLV,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[11]) {     /* constants */
    /* K0   */  be_nested_str_weak(tag_compression),
    /* K1   */  be_nested_str_weak(findsubval),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str_weak(node),
    /* K4   */  be_const_int(1),
    /* K5   */  be_nested_str_weak(endpoint),
    /* K6   */  be_const_int(2),
    /* K7   */  be_nested_str_weak(cluster),
    /* K8   */  be_const_int(3),
    /* K9   */  be_nested_str_weak(attribute),
    /* K10  */  be_nested_str_weak(list_index),
    }),
    be_str_weak(from_TLV),
    &be_const_str_solidified,
    ( &(const binstruction[30]) {  /* code */
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
      0x8C080301,  //  0019  GETMET	R2	R1	K1
      0x54120004,  //  001A  LDINT	R4	5
      0x7C080400,  //  001B  CALL	R2	2
      0x90021402,  //  001C  SETMBR	R0	K10	R2
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
        { be_const_key_weak(tostring, -1), be_const_closure(Matter_AttributePathIB_tostring_closure) },
        { be_const_key_weak(tag_compression, 0), be_const_var(0) },
        { be_const_key_weak(cluster, -1), be_const_var(3) },
        { be_const_key_weak(attribute, -1), be_const_var(4) },
        { be_const_key_weak(to_TLV, 2), be_const_closure(Matter_AttributePathIB_to_TLV_closure) },
        { be_const_key_weak(list_index, 3), be_const_var(5) },
        { be_const_key_weak(from_TLV, -1), be_const_closure(Matter_AttributePathIB_from_TLV_closure) },
        { be_const_key_weak(endpoint, 1), be_const_var(2) },
        { be_const_key_weak(node, -1), be_const_var(1) },
    })),
    be_str_weak(Matter_AttributePathIB)
);
/*******************************************************************/

void be_load_Matter_AttributePathIB_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_AttributePathIB);
    be_setglobal(vm, "Matter_AttributePathIB");
    be_pop(vm, 1);
}

extern const bclass be_class_Matter_ClusterPathIB;

/********************************************************************
** Solidified function: from_TLV
********************************************************************/
be_local_closure(Matter_ClusterPathIB_from_TLV,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(node),
    /* K1   */  be_nested_str_weak(findsubval),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str_weak(endpoint),
    /* K4   */  be_const_int(1),
    /* K5   */  be_nested_str_weak(cluster),
    /* K6   */  be_const_int(2),
    }),
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
be_local_closure(Matter_ClusterPathIB_to_TLV,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[13]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(TLV),
    /* K2   */  be_nested_str_weak(Matter_TLV_list),
    /* K3   */  be_nested_str_weak(add_TLV),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(U8),
    /* K6   */  be_nested_str_weak(node),
    /* K7   */  be_const_int(1),
    /* K8   */  be_nested_str_weak(U2),
    /* K9   */  be_nested_str_weak(endpoint),
    /* K10  */  be_const_int(2),
    /* K11  */  be_nested_str_weak(U4),
    /* K12  */  be_nested_str_weak(cluster),
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
      0x5814000A,  //  000F  LDCONST	R5	K10
      0x8818030B,  //  0010  GETMBR	R6	R1	K11
      0x881C010C,  //  0011  GETMBR	R7	R0	K12
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
        { be_const_key_weak(from_TLV, -1), be_const_closure(Matter_ClusterPathIB_from_TLV_closure) },
        { be_const_key_weak(node, -1), be_const_var(0) },
        { be_const_key_weak(cluster, -1), be_const_var(2) },
        { be_const_key_weak(endpoint, 0), be_const_var(1) },
        { be_const_key_weak(to_TLV, -1), be_const_closure(Matter_ClusterPathIB_to_TLV_closure) },
    })),
    be_str_weak(Matter_ClusterPathIB)
);
/*******************************************************************/

void be_load_Matter_ClusterPathIB_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_ClusterPathIB);
    be_setglobal(vm, "Matter_ClusterPathIB");
    be_pop(vm, 1);
}

extern const bclass be_class_Matter_DataVersionFilterIB;

/********************************************************************
** Solidified function: from_TLV
********************************************************************/
be_local_closure(Matter_DataVersionFilterIB_from_TLV,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(path),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(ClusterPathIB),
    /* K3   */  be_nested_str_weak(from_TLV),
    /* K4   */  be_nested_str_weak(findsub),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(data_version),
    /* K7   */  be_nested_str_weak(findsubval),
    /* K8   */  be_const_int(1),
    }),
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
be_local_closure(Matter_DataVersionFilterIB_to_TLV,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[10]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(TLV),
    /* K2   */  be_nested_str_weak(Matter_TLV_struct),
    /* K3   */  be_nested_str_weak(add_obj),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(path),
    /* K6   */  be_nested_str_weak(add_TLV),
    /* K7   */  be_const_int(1),
    /* K8   */  be_nested_str_weak(U4),
    /* K9   */  be_nested_str_weak(data_version),
    }),
    be_str_weak(to_TLV),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x88040301,  //  0001  GETMBR	R1	R1	K1
      0x8C080302,  //  0002  GETMET	R2	R1	K2
      0x7C080200,  //  0003  CALL	R2	1
      0x8C0C0503,  //  0004  GETMET	R3	R2	K3
      0x58140004,  //  0005  LDCONST	R5	K4
      0x88180105,  //  0006  GETMBR	R6	R0	K5
      0x7C0C0600,  //  0007  CALL	R3	3
      0x8C0C0506,  //  0008  GETMET	R3	R2	K6
      0x58140007,  //  0009  LDCONST	R5	K7
      0x88180308,  //  000A  GETMBR	R6	R1	K8
      0x881C0109,  //  000B  GETMBR	R7	R0	K9
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
        { be_const_key_weak(from_TLV, -1), be_const_closure(Matter_DataVersionFilterIB_from_TLV_closure) },
        { be_const_key_weak(to_TLV, -1), be_const_closure(Matter_DataVersionFilterIB_to_TLV_closure) },
        { be_const_key_weak(path, 3), be_const_var(0) },
        { be_const_key_weak(data_version, -1), be_const_var(1) },
    })),
    be_str_weak(Matter_DataVersionFilterIB)
);
/*******************************************************************/

void be_load_Matter_DataVersionFilterIB_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_DataVersionFilterIB);
    be_setglobal(vm, "Matter_DataVersionFilterIB");
    be_pop(vm, 1);
}

extern const bclass be_class_Matter_AttributeDataIB;

/********************************************************************
** Solidified function: from_TLV
********************************************************************/
be_local_closure(Matter_AttributeDataIB_from_TLV,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[11]) {     /* constants */
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
    }),
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
be_local_closure(Matter_AttributeDataIB_to_TLV,   /* name */
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
    /* K6   */  be_nested_str_weak(data_version),
    /* K7   */  be_nested_str_weak(add_obj),
    /* K8   */  be_const_int(1),
    /* K9   */  be_nested_str_weak(path),
    /* K10  */  be_const_int(2),
    /* K11  */  be_nested_str_weak(data),
    }),
    be_str_weak(to_TLV),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x88040301,  //  0001  GETMBR	R1	R1	K1
      0x8C080302,  //  0002  GETMET	R2	R1	K2
      0x7C080200,  //  0003  CALL	R2	1
      0x8C0C0503,  //  0004  GETMET	R3	R2	K3
      0x58140004,  //  0005  LDCONST	R5	K4
      0x88180305,  //  0006  GETMBR	R6	R1	K5
      0x881C0106,  //  0007  GETMBR	R7	R0	K6
      0x7C0C0800,  //  0008  CALL	R3	4
      0x8C0C0507,  //  0009  GETMET	R3	R2	K7
      0x58140008,  //  000A  LDCONST	R5	K8
      0x88180109,  //  000B  GETMBR	R6	R0	K9
      0x7C0C0600,  //  000C  CALL	R3	3
      0x8C0C0507,  //  000D  GETMET	R3	R2	K7
      0x5814000A,  //  000E  LDCONST	R5	K10
      0x8818010B,  //  000F  GETMBR	R6	R0	K11
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
        { be_const_key_weak(from_TLV, -1), be_const_closure(Matter_AttributeDataIB_from_TLV_closure) },
        { be_const_key_weak(path, 2), be_const_var(1) },
        { be_const_key_weak(to_TLV, -1), be_const_closure(Matter_AttributeDataIB_to_TLV_closure) },
    })),
    be_str_weak(Matter_AttributeDataIB)
);
/*******************************************************************/

void be_load_Matter_AttributeDataIB_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_AttributeDataIB);
    be_setglobal(vm, "Matter_AttributeDataIB");
    be_pop(vm, 1);
}

extern const bclass be_class_Matter_AttributeReportIB;

/********************************************************************
** Solidified function: to_TLV
********************************************************************/
be_local_closure(Matter_AttributeReportIB_to_TLV,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(TLV),
    /* K2   */  be_nested_str_weak(Matter_TLV_struct),
    /* K3   */  be_nested_str_weak(add_obj),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(attribute_status),
    /* K6   */  be_const_int(1),
    /* K7   */  be_nested_str_weak(attribute_data),
    }),
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
be_local_closure(Matter_AttributeReportIB_from_TLV,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(attribute_status),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(AttributeStatusIB),
    /* K3   */  be_nested_str_weak(from_TLV),
    /* K4   */  be_nested_str_weak(findsub),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(attribute_data),
    /* K7   */  be_nested_str_weak(AttributeDataIB),
    /* K8   */  be_const_int(1),
    }),
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
** Solidified class: Matter_AttributeReportIB
********************************************************************/
extern const bclass be_class_Matter_IM_base;
be_local_class(Matter_AttributeReportIB,
    2,
    &be_class_Matter_IM_base,
    be_nested_map(4,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(attribute_data, 3), be_const_var(1) },
        { be_const_key_weak(to_TLV, -1), be_const_closure(Matter_AttributeReportIB_to_TLV_closure) },
        { be_const_key_weak(attribute_status, -1), be_const_var(0) },
        { be_const_key_weak(from_TLV, -1), be_const_closure(Matter_AttributeReportIB_from_TLV_closure) },
    })),
    be_str_weak(Matter_AttributeReportIB)
);
/*******************************************************************/

void be_load_Matter_AttributeReportIB_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_AttributeReportIB);
    be_setglobal(vm, "Matter_AttributeReportIB");
    be_pop(vm, 1);
}

extern const bclass be_class_Matter_EventFilterIB;

/********************************************************************
** Solidified function: from_TLV
********************************************************************/
be_local_closure(Matter_EventFilterIB_from_TLV,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(node),
    /* K1   */  be_nested_str_weak(findsubval),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str_weak(event_min),
    /* K4   */  be_const_int(1),
    }),
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
be_local_closure(Matter_EventFilterIB_to_TLV,   /* name */
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
    /* K3   */  be_nested_str_weak(add_TLV),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(U8),
    /* K6   */  be_nested_str_weak(node),
    /* K7   */  be_const_int(1),
    /* K8   */  be_nested_str_weak(event_min),
    }),
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
        { be_const_key_weak(to_TLV, -1), be_const_closure(Matter_EventFilterIB_to_TLV_closure) },
        { be_const_key_weak(from_TLV, -1), be_const_closure(Matter_EventFilterIB_from_TLV_closure) },
    })),
    be_str_weak(Matter_EventFilterIB)
);
/*******************************************************************/

void be_load_Matter_EventFilterIB_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_EventFilterIB);
    be_setglobal(vm, "Matter_EventFilterIB");
    be_pop(vm, 1);
}

extern const bclass be_class_Matter_EventPathIB;

/********************************************************************
** Solidified function: from_TLV
********************************************************************/
be_local_closure(Matter_EventPathIB_from_TLV,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[10]) {     /* constants */
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
be_local_closure(Matter_EventPathIB_to_TLV,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[17]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(TLV),
    /* K2   */  be_nested_str_weak(Matter_TLV_list),
    /* K3   */  be_nested_str_weak(add_TLV),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(U8),
    /* K6   */  be_nested_str_weak(node),
    /* K7   */  be_const_int(1),
    /* K8   */  be_nested_str_weak(U2),
    /* K9   */  be_nested_str_weak(endpoint),
    /* K10  */  be_const_int(2),
    /* K11  */  be_nested_str_weak(U4),
    /* K12  */  be_nested_str_weak(cluster),
    /* K13  */  be_const_int(3),
    /* K14  */  be_nested_str_weak(event),
    /* K15  */  be_nested_str_weak(BOOL),
    /* K16  */  be_nested_str_weak(is_urgent),
    }),
    be_str_weak(to_TLV),
    &be_const_str_solidified,
    ( &(const binstruction[34]) {  /* code */
      0xB80A0000,  //  0000  GETNGBL	R2	K0
      0x88080501,  //  0001  GETMBR	R2	R2	K1
      0x4C0C0000,  //  0002  LDNIL	R3
      0x1C0C0203,  //  0003  EQ	R3	R1	R3
      0x780E0002,  //  0004  JMPF	R3	#0008
      0x8C0C0502,  //  0005  GETMET	R3	R2	K2
      0x7C0C0200,  //  0006  CALL	R3	1
      0x5C040600,  //  0007  MOVE	R1	R3
      0x8C0C0303,  //  0008  GETMET	R3	R1	K3
      0x58140004,  //  0009  LDCONST	R5	K4
      0x88180505,  //  000A  GETMBR	R6	R2	K5
      0x881C0106,  //  000B  GETMBR	R7	R0	K6
      0x7C0C0800,  //  000C  CALL	R3	4
      0x8C0C0303,  //  000D  GETMET	R3	R1	K3
      0x58140007,  //  000E  LDCONST	R5	K7
      0x88180508,  //  000F  GETMBR	R6	R2	K8
      0x881C0109,  //  0010  GETMBR	R7	R0	K9
      0x7C0C0800,  //  0011  CALL	R3	4
      0x8C0C0303,  //  0012  GETMET	R3	R1	K3
      0x5814000A,  //  0013  LDCONST	R5	K10
      0x8818050B,  //  0014  GETMBR	R6	R2	K11
      0x881C010C,  //  0015  GETMBR	R7	R0	K12
      0x7C0C0800,  //  0016  CALL	R3	4
      0x8C0C0303,  //  0017  GETMET	R3	R1	K3
      0x5814000D,  //  0018  LDCONST	R5	K13
      0x8818050B,  //  0019  GETMBR	R6	R2	K11
      0x881C010E,  //  001A  GETMBR	R7	R0	K14
      0x7C0C0800,  //  001B  CALL	R3	4
      0x8C0C0303,  //  001C  GETMET	R3	R1	K3
      0x54160003,  //  001D  LDINT	R5	4
      0x8818050F,  //  001E  GETMBR	R6	R2	K15
      0x881C0110,  //  001F  GETMBR	R7	R0	K16
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
        { be_const_key_weak(from_TLV, -1), be_const_closure(Matter_EventPathIB_from_TLV_closure) },
        { be_const_key_weak(to_TLV, -1), be_const_closure(Matter_EventPathIB_to_TLV_closure) },
        { be_const_key_weak(is_urgent, -1), be_const_var(4) },
        { be_const_key_weak(event, -1), be_const_var(3) },
        { be_const_key_weak(cluster, 3), be_const_var(2) },
        { be_const_key_weak(node, -1), be_const_var(0) },
    })),
    be_str_weak(Matter_EventPathIB)
);
/*******************************************************************/

void be_load_Matter_EventPathIB_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_EventPathIB);
    be_setglobal(vm, "Matter_EventPathIB");
    be_pop(vm, 1);
}

extern const bclass be_class_Matter_EventDataIB;

/********************************************************************
** Solidified function: from_TLV
********************************************************************/
be_local_closure(Matter_EventDataIB_from_TLV,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[17]) {     /* constants */
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
    }),
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
be_local_closure(Matter_EventDataIB_to_TLV,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[21]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(TLV),
    /* K2   */  be_nested_str_weak(Matter_TLV_struct),
    /* K3   */  be_nested_str_weak(path),
    /* K4   */  be_nested_str_weak(to_TLV),
    /* K5   */  be_nested_str_weak(add_list),
    /* K6   */  be_const_int(0),
    /* K7   */  be_nested_str_weak(add_TLV),
    /* K8   */  be_const_int(1),
    /* K9   */  be_nested_str_weak(U8),
    /* K10  */  be_nested_str_weak(event_number),
    /* K11  */  be_const_int(2),
    /* K12  */  be_nested_str_weak(U1),
    /* K13  */  be_nested_str_weak(priority),
    /* K14  */  be_const_int(3),
    /* K15  */  be_nested_str_weak(epoch_timestamp),
    /* K16  */  be_nested_str_weak(system_timestamp),
    /* K17  */  be_nested_str_weak(delta_epoch_timestamp),
    /* K18  */  be_nested_str_weak(delta_system_timestamp),
    /* K19  */  be_nested_str_weak(add_obj),
    /* K20  */  be_nested_str_weak(data),
    }),
    be_str_weak(to_TLV),
    &be_const_str_solidified,
    ( &(const binstruction[51]) {  /* code */
      0xB80A0000,  //  0000  GETNGBL	R2	K0
      0x88080501,  //  0001  GETMBR	R2	R2	K1
      0x4C0C0000,  //  0002  LDNIL	R3
      0x1C0C0203,  //  0003  EQ	R3	R1	R3
      0x780E0002,  //  0004  JMPF	R3	#0008
      0x8C0C0502,  //  0005  GETMET	R3	R2	K2
      0x7C0C0200,  //  0006  CALL	R3	1
      0x5C040600,  //  0007  MOVE	R1	R3
      0x880C0103,  //  0008  GETMBR	R3	R0	K3
      0x780E0005,  //  0009  JMPF	R3	#0010
      0x880C0103,  //  000A  GETMBR	R3	R0	K3
      0x8C0C0704,  //  000B  GETMET	R3	R3	K4
      0x8C140305,  //  000C  GETMET	R5	R1	K5
      0x581C0006,  //  000D  LDCONST	R7	K6
      0x7C140400,  //  000E  CALL	R5	2
      0x7C0C0400,  //  000F  CALL	R3	2
      0x8C0C0307,  //  0010  GETMET	R3	R1	K7
      0x58140008,  //  0011  LDCONST	R5	K8
      0x88180509,  //  0012  GETMBR	R6	R2	K9
      0x881C010A,  //  0013  GETMBR	R7	R0	K10
      0x7C0C0800,  //  0014  CALL	R3	4
      0x8C0C0307,  //  0015  GETMET	R3	R1	K7
      0x5814000B,  //  0016  LDCONST	R5	K11
      0x8818050C,  //  0017  GETMBR	R6	R2	K12
      0x881C010D,  //  0018  GETMBR	R7	R0	K13
      0x7C0C0800,  //  0019  CALL	R3	4
      0x8C0C0307,  //  001A  GETMET	R3	R1	K7
      0x5814000E,  //  001B  LDCONST	R5	K14
      0x88180509,  //  001C  GETMBR	R6	R2	K9
      0x881C010F,  //  001D  GETMBR	R7	R0	K15
      0x7C0C0800,  //  001E  CALL	R3	4
      0x8C0C0307,  //  001F  GETMET	R3	R1	K7
      0x54160003,  //  0020  LDINT	R5	4
      0x88180509,  //  0021  GETMBR	R6	R2	K9
      0x881C0110,  //  0022  GETMBR	R7	R0	K16
      0x7C0C0800,  //  0023  CALL	R3	4
      0x8C0C0307,  //  0024  GETMET	R3	R1	K7
      0x54160004,  //  0025  LDINT	R5	5
      0x88180509,  //  0026  GETMBR	R6	R2	K9
      0x881C0111,  //  0027  GETMBR	R7	R0	K17
      0x7C0C0800,  //  0028  CALL	R3	4
      0x8C0C0307,  //  0029  GETMET	R3	R1	K7
      0x54160005,  //  002A  LDINT	R5	6
      0x88180509,  //  002B  GETMBR	R6	R2	K9
      0x881C0112,  //  002C  GETMBR	R7	R0	K18
      0x7C0C0800,  //  002D  CALL	R3	4
      0x8C0C0313,  //  002E  GETMET	R3	R1	K19
      0x54160006,  //  002F  LDINT	R5	7
      0x88180114,  //  0030  GETMBR	R6	R0	K20
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
        { be_const_key_weak(to_TLV, 6), be_const_closure(Matter_EventDataIB_to_TLV_closure) },
        { be_const_key_weak(from_TLV, -1), be_const_closure(Matter_EventDataIB_from_TLV_closure) },
        { be_const_key_weak(delta_epoch_timestamp, -1), be_const_var(5) },
        { be_const_key_weak(epoch_timestamp, -1), be_const_var(3) },
        { be_const_key_weak(system_timestamp, -1), be_const_var(4) },
        { be_const_key_weak(priority, 5), be_const_var(2) },
        { be_const_key_weak(path, 3), be_const_var(0) },
        { be_const_key_weak(event_number, 2), be_const_var(1) },
    })),
    be_str_weak(Matter_EventDataIB)
);
/*******************************************************************/

void be_load_Matter_EventDataIB_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_EventDataIB);
    be_setglobal(vm, "Matter_EventDataIB");
    be_pop(vm, 1);
}

extern const bclass be_class_Matter_EventReportIB;

/********************************************************************
** Solidified function: to_TLV
********************************************************************/
be_local_closure(Matter_EventReportIB_to_TLV,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(TLV),
    /* K2   */  be_nested_str_weak(Matter_TLV_struct),
    /* K3   */  be_nested_str_weak(add_obj),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(event_status),
    /* K6   */  be_const_int(1),
    /* K7   */  be_nested_str_weak(event_data),
    }),
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
be_local_closure(Matter_EventReportIB_from_TLV,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(event_status),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(EventStatusIB),
    /* K3   */  be_nested_str_weak(from_TLV),
    /* K4   */  be_nested_str_weak(findsub),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(event_data),
    /* K7   */  be_nested_str_weak(EventDataIB),
    /* K8   */  be_const_int(1),
    }),
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
** Solidified class: Matter_EventReportIB
********************************************************************/
extern const bclass be_class_Matter_IM_base;
be_local_class(Matter_EventReportIB,
    2,
    &be_class_Matter_IM_base,
    be_nested_map(4,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(event_data, 3), be_const_var(1) },
        { be_const_key_weak(to_TLV, -1), be_const_closure(Matter_EventReportIB_to_TLV_closure) },
        { be_const_key_weak(event_status, -1), be_const_var(0) },
        { be_const_key_weak(from_TLV, -1), be_const_closure(Matter_EventReportIB_from_TLV_closure) },
    })),
    be_str_weak(Matter_EventReportIB)
);
/*******************************************************************/

void be_load_Matter_EventReportIB_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_EventReportIB);
    be_setglobal(vm, "Matter_EventReportIB");
    be_pop(vm, 1);
}

extern const bclass be_class_Matter_CommandPathIB;

/********************************************************************
** Solidified function: from_TLV
********************************************************************/
be_local_closure(Matter_CommandPathIB_from_TLV,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(endpoint),
    /* K1   */  be_nested_str_weak(findsubval),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str_weak(cluster),
    /* K4   */  be_const_int(1),
    /* K5   */  be_nested_str_weak(command),
    /* K6   */  be_const_int(2),
    }),
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
be_local_closure(Matter_CommandPathIB_to_TLV,   /* name */
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
    /* K2   */  be_nested_str_weak(Matter_TLV_list),
    /* K3   */  be_nested_str_weak(add_TLV),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(U2),
    /* K6   */  be_nested_str_weak(endpoint),
    /* K7   */  be_const_int(1),
    /* K8   */  be_nested_str_weak(U4),
    /* K9   */  be_nested_str_weak(cluster),
    /* K10  */  be_const_int(2),
    /* K11  */  be_nested_str_weak(command),
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
      0x5814000A,  //  000F  LDCONST	R5	K10
      0x88180308,  //  0010  GETMBR	R6	R1	K8
      0x881C010B,  //  0011  GETMBR	R7	R0	K11
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
        { be_const_key_weak(from_TLV, 0), be_const_closure(Matter_CommandPathIB_from_TLV_closure) },
        { be_const_key_weak(cluster, -1), be_const_var(1) },
        { be_const_key_weak(command, 1), be_const_var(2) },
        { be_const_key_weak(to_TLV, -1), be_const_closure(Matter_CommandPathIB_to_TLV_closure) },
    })),
    be_str_weak(Matter_CommandPathIB)
);
/*******************************************************************/

void be_load_Matter_CommandPathIB_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_CommandPathIB);
    be_setglobal(vm, "Matter_CommandPathIB");
    be_pop(vm, 1);
}

extern const bclass be_class_Matter_CommandDataIB;

/********************************************************************
** Solidified function: to_TLV
********************************************************************/
be_local_closure(Matter_CommandDataIB_to_TLV,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(TLV),
    /* K2   */  be_nested_str_weak(Matter_TLV_struct),
    /* K3   */  be_nested_str_weak(add_obj),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(command_path),
    /* K6   */  be_const_int(1),
    /* K7   */  be_nested_str_weak(command_fields),
    }),
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
be_local_closure(Matter_CommandDataIB_from_TLV,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(command_path),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(CommandPathIB),
    /* K3   */  be_nested_str_weak(from_TLV),
    /* K4   */  be_nested_str_weak(findsub),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(command_fields),
    /* K7   */  be_const_int(1),
    }),
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
      0x8C080304,  //  000E  GETMET	R2	R1	K4
      0x58100007,  //  000F  LDCONST	R4	K7
      0x7C080400,  //  0010  CALL	R2	2
      0x90020C02,  //  0011  SETMBR	R0	K6	R2
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
        { be_const_key_weak(to_TLV, -1), be_const_closure(Matter_CommandDataIB_to_TLV_closure) },
        { be_const_key_weak(command_fields, -1), be_const_var(1) },
        { be_const_key_weak(from_TLV, -1), be_const_closure(Matter_CommandDataIB_from_TLV_closure) },
    })),
    be_str_weak(Matter_CommandDataIB)
);
/*******************************************************************/

void be_load_Matter_CommandDataIB_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_CommandDataIB);
    be_setglobal(vm, "Matter_CommandDataIB");
    be_pop(vm, 1);
}

extern const bclass be_class_Matter_InvokeResponseIB;

/********************************************************************
** Solidified function: from_TLV
********************************************************************/
be_local_closure(Matter_InvokeResponseIB_from_TLV,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(command),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(CommandDataIB),
    /* K3   */  be_nested_str_weak(from_TLV),
    /* K4   */  be_nested_str_weak(findsub),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(status),
    /* K7   */  be_nested_str_weak(CommandStatusIB),
    /* K8   */  be_const_int(1),
    }),
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
be_local_closure(Matter_InvokeResponseIB_to_TLV,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(TLV),
    /* K2   */  be_nested_str_weak(Matter_TLV_struct),
    /* K3   */  be_nested_str_weak(add_obj),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(command),
    /* K6   */  be_const_int(1),
    /* K7   */  be_nested_str_weak(status),
    }),
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
** Solidified class: Matter_InvokeResponseIB
********************************************************************/
extern const bclass be_class_Matter_IM_base;
be_local_class(Matter_InvokeResponseIB,
    2,
    &be_class_Matter_IM_base,
    be_nested_map(4,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(from_TLV, -1), be_const_closure(Matter_InvokeResponseIB_from_TLV_closure) },
        { be_const_key_weak(to_TLV, -1), be_const_closure(Matter_InvokeResponseIB_to_TLV_closure) },
        { be_const_key_weak(command, -1), be_const_var(0) },
        { be_const_key_weak(status, -1), be_const_var(1) },
    })),
    be_str_weak(Matter_InvokeResponseIB)
);
/*******************************************************************/

void be_load_Matter_InvokeResponseIB_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_InvokeResponseIB);
    be_setglobal(vm, "Matter_InvokeResponseIB");
    be_pop(vm, 1);
}

extern const bclass be_class_Matter_CommandStatusIB;

/********************************************************************
** Solidified function: to_TLV
********************************************************************/
be_local_closure(Matter_CommandStatusIB_to_TLV,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(TLV),
    /* K2   */  be_nested_str_weak(Matter_TLV_struct),
    /* K3   */  be_nested_str_weak(add_obj),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(command_path),
    /* K6   */  be_const_int(1),
    /* K7   */  be_nested_str_weak(status),
    }),
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
be_local_closure(Matter_CommandStatusIB_from_TLV,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(command_path),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(CommandPathIB),
    /* K3   */  be_nested_str_weak(from_TLV),
    /* K4   */  be_nested_str_weak(findsub),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(status),
    /* K7   */  be_nested_str_weak(StatusIB),
    /* K8   */  be_const_int(1),
    }),
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
** Solidified class: Matter_CommandStatusIB
********************************************************************/
extern const bclass be_class_Matter_IM_base;
be_local_class(Matter_CommandStatusIB,
    2,
    &be_class_Matter_IM_base,
    be_nested_map(4,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(command_path, 2), be_const_var(0) },
        { be_const_key_weak(to_TLV, -1), be_const_closure(Matter_CommandStatusIB_to_TLV_closure) },
        { be_const_key_weak(from_TLV, -1), be_const_closure(Matter_CommandStatusIB_from_TLV_closure) },
        { be_const_key_weak(status, -1), be_const_var(1) },
    })),
    be_str_weak(Matter_CommandStatusIB)
);
/*******************************************************************/

void be_load_Matter_CommandStatusIB_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_CommandStatusIB);
    be_setglobal(vm, "Matter_CommandStatusIB");
    be_pop(vm, 1);
}

extern const bclass be_class_Matter_EventStatusIB;

/********************************************************************
** Solidified function: from_TLV
********************************************************************/
be_local_closure(Matter_EventStatusIB_from_TLV,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(path),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(EventPathIB),
    /* K3   */  be_nested_str_weak(from_TLV),
    /* K4   */  be_nested_str_weak(findsub),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(status),
    /* K7   */  be_nested_str_weak(StatusIB),
    /* K8   */  be_const_int(1),
    }),
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
be_local_closure(Matter_EventStatusIB_to_TLV,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(TLV),
    /* K2   */  be_nested_str_weak(Matter_TLV_struct),
    /* K3   */  be_nested_str_weak(add_obj),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(path),
    /* K6   */  be_const_int(1),
    /* K7   */  be_nested_str_weak(status),
    }),
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
** Solidified class: Matter_EventStatusIB
********************************************************************/
extern const bclass be_class_Matter_IM_base;
be_local_class(Matter_EventStatusIB,
    2,
    &be_class_Matter_IM_base,
    be_nested_map(4,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(from_TLV, -1), be_const_closure(Matter_EventStatusIB_from_TLV_closure) },
        { be_const_key_weak(to_TLV, -1), be_const_closure(Matter_EventStatusIB_to_TLV_closure) },
        { be_const_key_weak(path, -1), be_const_var(0) },
        { be_const_key_weak(status, -1), be_const_var(1) },
    })),
    be_str_weak(Matter_EventStatusIB)
);
/*******************************************************************/

void be_load_Matter_EventStatusIB_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_EventStatusIB);
    be_setglobal(vm, "Matter_EventStatusIB");
    be_pop(vm, 1);
}

extern const bclass be_class_Matter_AttributeStatusIB;

/********************************************************************
** Solidified function: from_TLV
********************************************************************/
be_local_closure(Matter_AttributeStatusIB_from_TLV,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(path),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(AttributePathIB),
    /* K3   */  be_nested_str_weak(from_TLV),
    /* K4   */  be_nested_str_weak(findsub),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(status),
    /* K7   */  be_nested_str_weak(StatusIB),
    /* K8   */  be_const_int(1),
    }),
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
be_local_closure(Matter_AttributeStatusIB_to_TLV,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(TLV),
    /* K2   */  be_nested_str_weak(Matter_TLV_struct),
    /* K3   */  be_nested_str_weak(add_obj),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(path),
    /* K6   */  be_const_int(1),
    /* K7   */  be_nested_str_weak(status),
    }),
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
** Solidified class: Matter_AttributeStatusIB
********************************************************************/
extern const bclass be_class_Matter_IM_base;
be_local_class(Matter_AttributeStatusIB,
    2,
    &be_class_Matter_IM_base,
    be_nested_map(4,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(from_TLV, -1), be_const_closure(Matter_AttributeStatusIB_from_TLV_closure) },
        { be_const_key_weak(to_TLV, -1), be_const_closure(Matter_AttributeStatusIB_to_TLV_closure) },
        { be_const_key_weak(path, -1), be_const_var(0) },
        { be_const_key_weak(status, -1), be_const_var(1) },
    })),
    be_str_weak(Matter_AttributeStatusIB)
);
/*******************************************************************/

void be_load_Matter_AttributeStatusIB_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_AttributeStatusIB);
    be_setglobal(vm, "Matter_AttributeStatusIB");
    be_pop(vm, 1);
}

extern const bclass be_class_Matter_StatusIB;

/********************************************************************
** Solidified function: to_TLV
********************************************************************/
be_local_closure(Matter_StatusIB_to_TLV,   /* name */
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
    /* K3   */  be_nested_str_weak(add_TLV),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(U2),
    /* K6   */  be_nested_str_weak(status),
    /* K7   */  be_const_int(1),
    /* K8   */  be_nested_str_weak(cluster_status),
    }),
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
be_local_closure(Matter_StatusIB_from_TLV,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(status),
    /* K1   */  be_nested_str_weak(findsubval),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str_weak(cluster_status),
    /* K4   */  be_const_int(1),
    }),
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
** Solidified class: Matter_StatusIB
********************************************************************/
extern const bclass be_class_Matter_IM_base;
be_local_class(Matter_StatusIB,
    2,
    &be_class_Matter_IM_base,
    be_nested_map(4,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(cluster_status, 2), be_const_var(1) },
        { be_const_key_weak(to_TLV, -1), be_const_closure(Matter_StatusIB_to_TLV_closure) },
        { be_const_key_weak(from_TLV, -1), be_const_closure(Matter_StatusIB_from_TLV_closure) },
        { be_const_key_weak(status, -1), be_const_var(0) },
    })),
    be_str_weak(Matter_StatusIB)
);
/*******************************************************************/

void be_load_Matter_StatusIB_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_StatusIB);
    be_setglobal(vm, "Matter_StatusIB");
    be_pop(vm, 1);
}

extern const bclass be_class_Matter_StatusResponseMessage;

/********************************************************************
** Solidified function: from_TLV
********************************************************************/
be_local_closure(Matter_StatusResponseMessage_from_TLV,   /* name */
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
    /* K0   */  be_nested_str_weak(status),
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
** Solidified function: to_TLV
********************************************************************/
be_local_closure(Matter_StatusResponseMessage_to_TLV,   /* name */
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
    /* K3   */  be_nested_str_weak(add_TLV),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(U4),
    /* K6   */  be_nested_str_weak(status),
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
      0x8C0C0503,  //  0004  GETMET	R3	R2	K3
      0x58140004,  //  0005  LDCONST	R5	K4
      0x88180305,  //  0006  GETMBR	R6	R1	K5
      0x881C0106,  //  0007  GETMBR	R7	R0	K6
      0x7C0C0800,  //  0008  CALL	R3	4
      0x8C0C0503,  //  0009  GETMET	R3	R2	K3
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
** Solidified class: Matter_StatusResponseMessage
********************************************************************/
extern const bclass be_class_Matter_IM_Message_base;
be_local_class(Matter_StatusResponseMessage,
    1,
    &be_class_Matter_IM_Message_base,
    be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(from_TLV, -1), be_const_closure(Matter_StatusResponseMessage_from_TLV_closure) },
        { be_const_key_weak(to_TLV, 2), be_const_closure(Matter_StatusResponseMessage_to_TLV_closure) },
        { be_const_key_weak(status, -1), be_const_var(0) },
    })),
    be_str_weak(Matter_StatusResponseMessage)
);
/*******************************************************************/

void be_load_Matter_StatusResponseMessage_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_StatusResponseMessage);
    be_setglobal(vm, "Matter_StatusResponseMessage");
    be_pop(vm, 1);
}

extern const bclass be_class_Matter_ReadRequestMessage;

/********************************************************************
** Solidified function: from_TLV
********************************************************************/
be_local_closure(Matter_ReadRequestMessage_from_TLV,   /* name */
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
** Solidified function: to_TLV
********************************************************************/
be_local_closure(Matter_ReadRequestMessage_to_TLV,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[17]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(TLV),
    /* K2   */  be_nested_str_weak(Matter_TLV_struct),
    /* K3   */  be_nested_str_weak(to_TLV_array),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(attributes_requests),
    /* K6   */  be_const_int(1),
    /* K7   */  be_nested_str_weak(event_requests),
    /* K8   */  be_const_int(2),
    /* K9   */  be_nested_str_weak(event_filters),
    /* K10  */  be_nested_str_weak(add_TLV),
    /* K11  */  be_const_int(3),
    /* K12  */  be_nested_str_weak(BOOL),
    /* K13  */  be_nested_str_weak(fabric_filtered),
    /* K14  */  be_nested_str_weak(data_version_filters),
    /* K15  */  be_nested_str_weak(U1),
    /* K16  */  be_nested_str_weak(InteractionModelRevision),
    }),
    be_str_weak(to_TLV),
    &be_const_str_solidified,
    ( &(const binstruction[35]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x88040301,  //  0001  GETMBR	R1	R1	K1
      0x8C080302,  //  0002  GETMET	R2	R1	K2
      0x7C080200,  //  0003  CALL	R2	1
      0x8C0C0103,  //  0004  GETMET	R3	R0	K3
      0x5C140400,  //  0005  MOVE	R5	R2
      0x58180004,  //  0006  LDCONST	R6	K4
      0x881C0105,  //  0007  GETMBR	R7	R0	K5
      0x7C0C0800,  //  0008  CALL	R3	4
      0x8C0C0103,  //  0009  GETMET	R3	R0	K3
      0x5C140400,  //  000A  MOVE	R5	R2
      0x58180006,  //  000B  LDCONST	R6	K6
      0x881C0107,  //  000C  GETMBR	R7	R0	K7
      0x7C0C0800,  //  000D  CALL	R3	4
      0x8C0C0103,  //  000E  GETMET	R3	R0	K3
      0x5C140400,  //  000F  MOVE	R5	R2
      0x58180008,  //  0010  LDCONST	R6	K8
      0x881C0109,  //  0011  GETMBR	R7	R0	K9
      0x7C0C0800,  //  0012  CALL	R3	4
      0x8C0C050A,  //  0013  GETMET	R3	R2	K10
      0x5814000B,  //  0014  LDCONST	R5	K11
      0x8818030C,  //  0015  GETMBR	R6	R1	K12
      0x881C010D,  //  0016  GETMBR	R7	R0	K13
      0x7C0C0800,  //  0017  CALL	R3	4
      0x8C0C0103,  //  0018  GETMET	R3	R0	K3
      0x5C140400,  //  0019  MOVE	R5	R2
      0x541A0003,  //  001A  LDINT	R6	4
      0x881C010E,  //  001B  GETMBR	R7	R0	K14
      0x7C0C0800,  //  001C  CALL	R3	4
      0x8C0C050A,  //  001D  GETMET	R3	R2	K10
      0x541600FE,  //  001E  LDINT	R5	255
      0x8818030F,  //  001F  GETMBR	R6	R1	K15
      0x881C0110,  //  0020  GETMBR	R7	R0	K16
      0x7C0C0800,  //  0021  CALL	R3	4
      0x80040400,  //  0022  RET	1	R2
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
    be_nested_map(7,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(event_filters, -1), be_const_var(2) },
        { be_const_key_weak(fabric_filtered, 3), be_const_var(3) },
        { be_const_key_weak(to_TLV, -1), be_const_closure(Matter_ReadRequestMessage_to_TLV_closure) },
        { be_const_key_weak(from_TLV, -1), be_const_closure(Matter_ReadRequestMessage_from_TLV_closure) },
        { be_const_key_weak(data_version_filters, -1), be_const_var(4) },
        { be_const_key_weak(attributes_requests, -1), be_const_var(0) },
        { be_const_key_weak(event_requests, 0), be_const_var(1) },
    })),
    be_str_weak(Matter_ReadRequestMessage)
);
/*******************************************************************/

void be_load_Matter_ReadRequestMessage_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_ReadRequestMessage);
    be_setglobal(vm, "Matter_ReadRequestMessage");
    be_pop(vm, 1);
}

extern const bclass be_class_Matter_ReportDataMessage;

/********************************************************************
** Solidified function: from_TLV
********************************************************************/
be_local_closure(Matter_ReportDataMessage_from_TLV,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[14]) {     /* constants */
    /* K0   */  be_nested_str_weak(subscription_id),
    /* K1   */  be_nested_str_weak(findsubval),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str_weak(attribute_reports),
    /* K4   */  be_nested_str_weak(from_TLV_array),
    /* K5   */  be_const_int(1),
    /* K6   */  be_nested_str_weak(matter),
    /* K7   */  be_nested_str_weak(AttributeReportIB),
    /* K8   */  be_nested_str_weak(event_reports),
    /* K9   */  be_const_int(2),
    /* K10  */  be_nested_str_weak(EventReportIB),
    /* K11  */  be_nested_str_weak(more_chunked_messages),
    /* K12  */  be_const_int(3),
    /* K13  */  be_nested_str_weak(suppress_response),
    }),
    be_str_weak(from_TLV),
    &be_const_str_solidified,
    ( &(const binstruction[34]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0001,  //  0002  JMPF	R2	#0005
      0x4C080000,  //  0003  LDNIL	R2
      0x80040400,  //  0004  RET	1	R2
      0x8C080301,  //  0005  GETMET	R2	R1	K1
      0x58100002,  //  0006  LDCONST	R4	K2
      0x7C080400,  //  0007  CALL	R2	2
      0x90020002,  //  0008  SETMBR	R0	K0	R2
      0x8C080104,  //  0009  GETMET	R2	R0	K4
      0x8C100301,  //  000A  GETMET	R4	R1	K1
      0x58180005,  //  000B  LDCONST	R6	K5
      0x7C100400,  //  000C  CALL	R4	2
      0xB8160C00,  //  000D  GETNGBL	R5	K6
      0x88140B07,  //  000E  GETMBR	R5	R5	K7
      0x7C080600,  //  000F  CALL	R2	3
      0x90020602,  //  0010  SETMBR	R0	K3	R2
      0x8C080104,  //  0011  GETMET	R2	R0	K4
      0x8C100301,  //  0012  GETMET	R4	R1	K1
      0x58180009,  //  0013  LDCONST	R6	K9
      0x7C100400,  //  0014  CALL	R4	2
      0xB8160C00,  //  0015  GETNGBL	R5	K6
      0x88140B0A,  //  0016  GETMBR	R5	R5	K10
      0x7C080600,  //  0017  CALL	R2	3
      0x90021002,  //  0018  SETMBR	R0	K8	R2
      0x8C080301,  //  0019  GETMET	R2	R1	K1
      0x5810000C,  //  001A  LDCONST	R4	K12
      0x7C080400,  //  001B  CALL	R2	2
      0x90021602,  //  001C  SETMBR	R0	K11	R2
      0x8C080301,  //  001D  GETMET	R2	R1	K1
      0x54120003,  //  001E  LDINT	R4	4
      0x7C080400,  //  001F  CALL	R2	2
      0x90021A02,  //  0020  SETMBR	R0	K13	R2
      0x80040000,  //  0021  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: to_TLV
********************************************************************/
be_local_closure(Matter_ReportDataMessage_to_TLV,   /* name */
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
    be_nested_map(7,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(attribute_reports, -1), be_const_var(1) },
        { be_const_key_weak(from_TLV, -1), be_const_closure(Matter_ReportDataMessage_from_TLV_closure) },
        { be_const_key_weak(to_TLV, -1), be_const_closure(Matter_ReportDataMessage_to_TLV_closure) },
        { be_const_key_weak(more_chunked_messages, -1), be_const_var(3) },
        { be_const_key_weak(event_reports, -1), be_const_var(2) },
        { be_const_key_weak(suppress_response, -1), be_const_var(4) },
        { be_const_key_weak(subscription_id, 5), be_const_var(0) },
    })),
    be_str_weak(Matter_ReportDataMessage)
);
/*******************************************************************/

void be_load_Matter_ReportDataMessage_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_ReportDataMessage);
    be_setglobal(vm, "Matter_ReportDataMessage");
    be_pop(vm, 1);
}

extern const bclass be_class_Matter_SubscribeRequestMessage;

/********************************************************************
** Solidified function: from_TLV
********************************************************************/
be_local_closure(Matter_SubscribeRequestMessage_from_TLV,   /* name */
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
** Solidified function: to_TLV
********************************************************************/
be_local_closure(Matter_SubscribeRequestMessage_to_TLV,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[21]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(TLV),
    /* K2   */  be_nested_str_weak(Matter_TLV_struct),
    /* K3   */  be_nested_str_weak(add_TLV),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(BOOL),
    /* K6   */  be_nested_str_weak(keep_subscriptions),
    /* K7   */  be_const_int(1),
    /* K8   */  be_nested_str_weak(U2),
    /* K9   */  be_nested_str_weak(min_interval_floor),
    /* K10  */  be_const_int(2),
    /* K11  */  be_nested_str_weak(max_interval_ceiling),
    /* K12  */  be_nested_str_weak(to_TLV_array),
    /* K13  */  be_const_int(3),
    /* K14  */  be_nested_str_weak(attributes_requests),
    /* K15  */  be_nested_str_weak(event_requests),
    /* K16  */  be_nested_str_weak(event_filters),
    /* K17  */  be_nested_str_weak(fabric_filtered),
    /* K18  */  be_nested_str_weak(data_version_filters),
    /* K19  */  be_nested_str_weak(U1),
    /* K20  */  be_nested_str_weak(InteractionModelRevision),
    }),
    be_str_weak(to_TLV),
    &be_const_str_solidified,
    ( &(const binstruction[50]) {  /* code */
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
      0x88180308,  //  0010  GETMBR	R6	R1	K8
      0x881C010B,  //  0011  GETMBR	R7	R0	K11
      0x7C0C0800,  //  0012  CALL	R3	4
      0x8C0C010C,  //  0013  GETMET	R3	R0	K12
      0x5C140400,  //  0014  MOVE	R5	R2
      0x5818000D,  //  0015  LDCONST	R6	K13
      0x881C010E,  //  0016  GETMBR	R7	R0	K14
      0x7C0C0800,  //  0017  CALL	R3	4
      0x8C0C010C,  //  0018  GETMET	R3	R0	K12
      0x5C140400,  //  0019  MOVE	R5	R2
      0x541A0003,  //  001A  LDINT	R6	4
      0x881C010F,  //  001B  GETMBR	R7	R0	K15
      0x7C0C0800,  //  001C  CALL	R3	4
      0x8C0C010C,  //  001D  GETMET	R3	R0	K12
      0x5C140400,  //  001E  MOVE	R5	R2
      0x541A0004,  //  001F  LDINT	R6	5
      0x881C0110,  //  0020  GETMBR	R7	R0	K16
      0x7C0C0800,  //  0021  CALL	R3	4
      0x8C0C0503,  //  0022  GETMET	R3	R2	K3
      0x54160006,  //  0023  LDINT	R5	7
      0x88180305,  //  0024  GETMBR	R6	R1	K5
      0x881C0111,  //  0025  GETMBR	R7	R0	K17
      0x7C0C0800,  //  0026  CALL	R3	4
      0x8C0C010C,  //  0027  GETMET	R3	R0	K12
      0x5C140400,  //  0028  MOVE	R5	R2
      0x541A0007,  //  0029  LDINT	R6	8
      0x881C0112,  //  002A  GETMBR	R7	R0	K18
      0x7C0C0800,  //  002B  CALL	R3	4
      0x8C0C0503,  //  002C  GETMET	R3	R2	K3
      0x541600FE,  //  002D  LDINT	R5	255
      0x88180313,  //  002E  GETMBR	R6	R1	K19
      0x881C0114,  //  002F  GETMBR	R7	R0	K20
      0x7C0C0800,  //  0030  CALL	R3	4
      0x80040400,  //  0031  RET	1	R2
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
    be_nested_map(10,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(to_TLV, -1), be_const_closure(Matter_SubscribeRequestMessage_to_TLV_closure) },
        { be_const_key_weak(attributes_requests, 7), be_const_var(3) },
        { be_const_key_weak(fabric_filtered, 6), be_const_var(6) },
        { be_const_key_weak(min_interval_floor, -1), be_const_var(1) },
        { be_const_key_weak(data_version_filters, -1), be_const_var(7) },
        { be_const_key_weak(max_interval_ceiling, -1), be_const_var(2) },
        { be_const_key_weak(event_requests, 3), be_const_var(4) },
        { be_const_key_weak(event_filters, -1), be_const_var(5) },
        { be_const_key_weak(from_TLV, -1), be_const_closure(Matter_SubscribeRequestMessage_from_TLV_closure) },
        { be_const_key_weak(keep_subscriptions, 0), be_const_var(0) },
    })),
    be_str_weak(Matter_SubscribeRequestMessage)
);
/*******************************************************************/

void be_load_Matter_SubscribeRequestMessage_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_SubscribeRequestMessage);
    be_setglobal(vm, "Matter_SubscribeRequestMessage");
    be_pop(vm, 1);
}

extern const bclass be_class_Matter_SubscribeResponseMessage;

/********************************************************************
** Solidified function: to_TLV
********************************************************************/
be_local_closure(Matter_SubscribeResponseMessage_to_TLV,   /* name */
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
** Solidified function: from_TLV
********************************************************************/
be_local_closure(Matter_SubscribeResponseMessage_from_TLV,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(subscription_id),
    /* K1   */  be_nested_str_weak(findsubval),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str_weak(max_interval),
    /* K4   */  be_const_int(2),
    }),
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
** Solidified class: Matter_SubscribeResponseMessage
********************************************************************/
extern const bclass be_class_Matter_IM_Message_base;
be_local_class(Matter_SubscribeResponseMessage,
    2,
    &be_class_Matter_IM_Message_base,
    be_nested_map(4,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(subscription_id, 2), be_const_var(0) },
        { be_const_key_weak(to_TLV, -1), be_const_closure(Matter_SubscribeResponseMessage_to_TLV_closure) },
        { be_const_key_weak(from_TLV, -1), be_const_closure(Matter_SubscribeResponseMessage_from_TLV_closure) },
        { be_const_key_weak(max_interval, -1), be_const_var(1) },
    })),
    be_str_weak(Matter_SubscribeResponseMessage)
);
/*******************************************************************/

void be_load_Matter_SubscribeResponseMessage_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_SubscribeResponseMessage);
    be_setglobal(vm, "Matter_SubscribeResponseMessage");
    be_pop(vm, 1);
}

extern const bclass be_class_Matter_WriteRequestMessage;

/********************************************************************
** Solidified function: to_TLV
********************************************************************/
be_local_closure(Matter_WriteRequestMessage_to_TLV,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[16]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(TLV),
    /* K2   */  be_nested_str_weak(Matter_TLV_struct),
    /* K3   */  be_nested_str_weak(add_TLV),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(BOOL),
    /* K6   */  be_nested_str_weak(suppress_response),
    /* K7   */  be_const_int(1),
    /* K8   */  be_nested_str_weak(timed_request),
    /* K9   */  be_nested_str_weak(to_TLV_array),
    /* K10  */  be_const_int(2),
    /* K11  */  be_nested_str_weak(write_requests),
    /* K12  */  be_const_int(3),
    /* K13  */  be_nested_str_weak(more_chunked_messages),
    /* K14  */  be_nested_str_weak(U1),
    /* K15  */  be_nested_str_weak(InteractionModelRevision),
    }),
    be_str_weak(to_TLV),
    &be_const_str_solidified,
    ( &(const binstruction[30]) {  /* code */
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
      0x8C0C0109,  //  000E  GETMET	R3	R0	K9
      0x5C140400,  //  000F  MOVE	R5	R2
      0x5818000A,  //  0010  LDCONST	R6	K10
      0x881C010B,  //  0011  GETMBR	R7	R0	K11
      0x7C0C0800,  //  0012  CALL	R3	4
      0x8C0C0503,  //  0013  GETMET	R3	R2	K3
      0x5814000C,  //  0014  LDCONST	R5	K12
      0x88180305,  //  0015  GETMBR	R6	R1	K5
      0x881C010D,  //  0016  GETMBR	R7	R0	K13
      0x7C0C0800,  //  0017  CALL	R3	4
      0x8C0C0503,  //  0018  GETMET	R3	R2	K3
      0x541600FE,  //  0019  LDINT	R5	255
      0x8818030E,  //  001A  GETMBR	R6	R1	K14
      0x881C010F,  //  001B  GETMBR	R7	R0	K15
      0x7C0C0800,  //  001C  CALL	R3	4
      0x80040400,  //  001D  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: from_TLV
********************************************************************/
be_local_closure(Matter_WriteRequestMessage_from_TLV,   /* name */
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
    be_nested_map(6,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(suppress_response, 5), be_const_var(0) },
        { be_const_key_weak(to_TLV, -1), be_const_closure(Matter_WriteRequestMessage_to_TLV_closure) },
        { be_const_key_weak(timed_request, -1), be_const_var(1) },
        { be_const_key_weak(write_requests, -1), be_const_var(2) },
        { be_const_key_weak(more_chunked_messages, -1), be_const_var(3) },
        { be_const_key_weak(from_TLV, -1), be_const_closure(Matter_WriteRequestMessage_from_TLV_closure) },
    })),
    be_str_weak(Matter_WriteRequestMessage)
);
/*******************************************************************/

void be_load_Matter_WriteRequestMessage_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_WriteRequestMessage);
    be_setglobal(vm, "Matter_WriteRequestMessage");
    be_pop(vm, 1);
}

extern const bclass be_class_Matter_WriteResponseMessage;

/********************************************************************
** Solidified function: from_TLV
********************************************************************/
be_local_closure(Matter_WriteResponseMessage_from_TLV,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(write_requests),
    /* K1   */  be_nested_str_weak(from_TLV_array),
    /* K2   */  be_nested_str_weak(findsubval),
    /* K3   */  be_const_int(0),
    /* K4   */  be_nested_str_weak(matter),
    /* K5   */  be_nested_str_weak(AttributeStatusIB),
    }),
    be_str_weak(from_TLV),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
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
      0x80040000,  //  000D  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: to_TLV
********************************************************************/
be_local_closure(Matter_WriteResponseMessage_to_TLV,   /* name */
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
    be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(from_TLV, 2), be_const_closure(Matter_WriteResponseMessage_from_TLV_closure) },
        { be_const_key_weak(to_TLV, -1), be_const_closure(Matter_WriteResponseMessage_to_TLV_closure) },
        { be_const_key_weak(write_responses, -1), be_const_var(0) },
    })),
    be_str_weak(Matter_WriteResponseMessage)
);
/*******************************************************************/

void be_load_Matter_WriteResponseMessage_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_WriteResponseMessage);
    be_setglobal(vm, "Matter_WriteResponseMessage");
    be_pop(vm, 1);
}

extern const bclass be_class_Matter_TimedRequestMessage;

/********************************************************************
** Solidified function: from_TLV
********************************************************************/
be_local_closure(Matter_TimedRequestMessage_from_TLV,   /* name */
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
** Solidified function: to_TLV
********************************************************************/
be_local_closure(Matter_TimedRequestMessage_to_TLV,   /* name */
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
    /* K3   */  be_nested_str_weak(add_TLV),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(U2),
    /* K6   */  be_nested_str_weak(timeout),
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
      0x8C0C0503,  //  0004  GETMET	R3	R2	K3
      0x58140004,  //  0005  LDCONST	R5	K4
      0x88180305,  //  0006  GETMBR	R6	R1	K5
      0x881C0106,  //  0007  GETMBR	R7	R0	K6
      0x7C0C0800,  //  0008  CALL	R3	4
      0x8C0C0503,  //  0009  GETMET	R3	R2	K3
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
** Solidified class: Matter_TimedRequestMessage
********************************************************************/
extern const bclass be_class_Matter_IM_Message_base;
be_local_class(Matter_TimedRequestMessage,
    1,
    &be_class_Matter_IM_Message_base,
    be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(from_TLV, -1), be_const_closure(Matter_TimedRequestMessage_from_TLV_closure) },
        { be_const_key_weak(timeout, 2), be_const_var(0) },
        { be_const_key_weak(to_TLV, -1), be_const_closure(Matter_TimedRequestMessage_to_TLV_closure) },
    })),
    be_str_weak(Matter_TimedRequestMessage)
);
/*******************************************************************/

void be_load_Matter_TimedRequestMessage_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_TimedRequestMessage);
    be_setglobal(vm, "Matter_TimedRequestMessage");
    be_pop(vm, 1);
}

extern const bclass be_class_Matter_InvokeRequestMessage;

/********************************************************************
** Solidified function: from_TLV
********************************************************************/
be_local_closure(Matter_InvokeRequestMessage_from_TLV,   /* name */
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
** Solidified function: to_TLV
********************************************************************/
be_local_closure(Matter_InvokeRequestMessage_to_TLV,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[14]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(TLV),
    /* K2   */  be_nested_str_weak(Matter_TLV_struct),
    /* K3   */  be_nested_str_weak(add_TLV),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(BOOL),
    /* K6   */  be_nested_str_weak(suppress_response),
    /* K7   */  be_const_int(1),
    /* K8   */  be_nested_str_weak(timed_request),
    /* K9   */  be_nested_str_weak(to_TLV_array),
    /* K10  */  be_const_int(2),
    /* K11  */  be_nested_str_weak(invoke_requests),
    /* K12  */  be_nested_str_weak(U1),
    /* K13  */  be_nested_str_weak(InteractionModelRevision),
    }),
    be_str_weak(to_TLV),
    &be_const_str_solidified,
    ( &(const binstruction[25]) {  /* code */
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
      0x8C0C0109,  //  000E  GETMET	R3	R0	K9
      0x5C140400,  //  000F  MOVE	R5	R2
      0x5818000A,  //  0010  LDCONST	R6	K10
      0x881C010B,  //  0011  GETMBR	R7	R0	K11
      0x7C0C0800,  //  0012  CALL	R3	4
      0x8C0C0503,  //  0013  GETMET	R3	R2	K3
      0x541600FE,  //  0014  LDINT	R5	255
      0x8818030C,  //  0015  GETMBR	R6	R1	K12
      0x881C010D,  //  0016  GETMBR	R7	R0	K13
      0x7C0C0800,  //  0017  CALL	R3	4
      0x80040400,  //  0018  RET	1	R2
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
    be_nested_map(5,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(invoke_requests, -1), be_const_var(2) },
        { be_const_key_weak(suppress_response, 2), be_const_var(0) },
        { be_const_key_weak(timed_request, -1), be_const_var(1) },
        { be_const_key_weak(from_TLV, -1), be_const_closure(Matter_InvokeRequestMessage_from_TLV_closure) },
        { be_const_key_weak(to_TLV, -1), be_const_closure(Matter_InvokeRequestMessage_to_TLV_closure) },
    })),
    be_str_weak(Matter_InvokeRequestMessage)
);
/*******************************************************************/

void be_load_Matter_InvokeRequestMessage_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_InvokeRequestMessage);
    be_setglobal(vm, "Matter_InvokeRequestMessage");
    be_pop(vm, 1);
}

extern const bclass be_class_Matter_InvokeResponseMessage;

/********************************************************************
** Solidified function: to_TLV
********************************************************************/
be_local_closure(Matter_InvokeResponseMessage_to_TLV,   /* name */
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
** Solidified function: from_TLV
********************************************************************/
be_local_closure(Matter_InvokeResponseMessage_from_TLV,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(suppress_response),
    /* K1   */  be_nested_str_weak(findsubval),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str_weak(invoke_responses),
    /* K4   */  be_nested_str_weak(from_TLV_array),
    /* K5   */  be_const_int(1),
    /* K6   */  be_nested_str_weak(matter),
    /* K7   */  be_nested_str_weak(InvokeResponseIB),
    }),
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
      0x8C080104,  //  0009  GETMET	R2	R0	K4
      0x8C100301,  //  000A  GETMET	R4	R1	K1
      0x58180005,  //  000B  LDCONST	R6	K5
      0x7C100400,  //  000C  CALL	R4	2
      0xB8160C00,  //  000D  GETNGBL	R5	K6
      0x88140B07,  //  000E  GETMBR	R5	R5	K7
      0x7C080600,  //  000F  CALL	R2	3
      0x90020602,  //  0010  SETMBR	R0	K3	R2
      0x80040000,  //  0011  RET	1	R0
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
    be_nested_map(4,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(suppress_response, 3), be_const_var(0) },
        { be_const_key_weak(to_TLV, -1), be_const_closure(Matter_InvokeResponseMessage_to_TLV_closure) },
        { be_const_key_weak(invoke_responses, -1), be_const_var(1) },
        { be_const_key_weak(from_TLV, -1), be_const_closure(Matter_InvokeResponseMessage_from_TLV_closure) },
    })),
    be_str_weak(Matter_InvokeResponseMessage)
);
/*******************************************************************/

void be_load_Matter_InvokeResponseMessage_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_InvokeResponseMessage);
    be_setglobal(vm, "Matter_InvokeResponseMessage");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
