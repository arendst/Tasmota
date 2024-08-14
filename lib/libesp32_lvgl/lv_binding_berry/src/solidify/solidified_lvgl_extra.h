/* Solidification of lvgl_extra.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_lv_str_arr;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_lv_str_arr_init,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(l),
    /* K1   */  be_nested_str_weak(init),
    /* K2   */  be_nested_str_weak(introspect),
    /* K3   */  be_nested_str_weak(add),
    /* K4   */  be_nested_str_weak(toptr),
    /* K5   */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[31]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x60080003,  //  0001  GETGBL	R2	G3
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C080200,  //  0003  CALL	R2	1
      0x8C080501,  //  0004  GETMET	R2	R2	K1
      0x6010000C,  //  0005  GETGBL	R4	G12
      0x5C140200,  //  0006  MOVE	R5	R1
      0x7C100200,  //  0007  CALL	R4	1
      0x54160003,  //  0008  LDINT	R5	4
      0x08100805,  //  0009  MUL	R4	R4	R5
      0x7C080400,  //  000A  CALL	R2	2
      0xA40A0400,  //  000B  IMPORT	R2	K2
      0x600C0010,  //  000C  GETGBL	R3	G16
      0x5C100200,  //  000D  MOVE	R4	R1
      0x7C0C0200,  //  000E  CALL	R3	1
      0xA802000A,  //  000F  EXBLK	0	#001B
      0x5C100600,  //  0010  MOVE	R4	R3
      0x7C100000,  //  0011  CALL	R4	0
      0x8C140103,  //  0012  GETMET	R5	R0	K3
      0x601C0009,  //  0013  GETGBL	R7	G9
      0x8C200504,  //  0014  GETMET	R8	R2	K4
      0x5C280800,  //  0015  MOVE	R10	R4
      0x7C200400,  //  0016  CALL	R8	2
      0x7C1C0200,  //  0017  CALL	R7	1
      0x54220003,  //  0018  LDINT	R8	4
      0x7C140600,  //  0019  CALL	R5	3
      0x7001FFF4,  //  001A  JMP		#0010
      0x580C0005,  //  001B  LDCONST	R3	K5
      0xAC0C0200,  //  001C  CATCH	R3	1	0
      0xB0080000,  //  001D  RAISE	2	R0	R0
      0x80000000,  //  001E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: lv_str_arr
********************************************************************/
extern const bclass be_class_bytes;
be_local_class(lv_str_arr,
    1,
    &be_class_bytes,
    be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(init, -1), be_const_closure(class_lv_str_arr_init_closure) },
        { be_const_key_weak(l, 0), be_const_var(0) },
    })),
    be_str_weak(lv_str_arr)
);

extern const bclass be_class_lv_int_arr;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_lv_int_arr_init,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(instance),
    /* K1   */  be_nested_str_weak(init),
    /* K2   */  be_nested_str_weak(add),
    /* K3   */  be_nested_str_weak(stop_iteration),
    /* K4   */  be_nested_str_weak(ptr),
    /* K5   */  be_nested_str_weak(int),
    /* K6   */  be_nested_str_weak(value_error),
    /* K7   */  be_nested_str_weak(argument_X20must_X20be_X20a_X20list_X20or_X20a_X20pointer_X2Bsize),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[58]) {  /* code */
      0x600C0004,  //  0000  GETGBL	R3	G4
      0x5C100200,  //  0001  MOVE	R4	R1
      0x7C0C0200,  //  0002  CALL	R3	1
      0x1C0C0700,  //  0003  EQ	R3	R3	K0
      0x780E001F,  //  0004  JMPF	R3	#0025
      0x600C000F,  //  0005  GETGBL	R3	G15
      0x5C100200,  //  0006  MOVE	R4	R1
      0x60140012,  //  0007  GETGBL	R5	G18
      0x7C0C0400,  //  0008  CALL	R3	2
      0x780E001A,  //  0009  JMPF	R3	#0025
      0x600C0003,  //  000A  GETGBL	R3	G3
      0x5C100000,  //  000B  MOVE	R4	R0
      0x7C0C0200,  //  000C  CALL	R3	1
      0x8C0C0701,  //  000D  GETMET	R3	R3	K1
      0x6014000C,  //  000E  GETGBL	R5	G12
      0x5C180200,  //  000F  MOVE	R6	R1
      0x7C140200,  //  0010  CALL	R5	1
      0x541A0003,  //  0011  LDINT	R6	4
      0x08140A06,  //  0012  MUL	R5	R5	R6
      0x7C0C0400,  //  0013  CALL	R3	2
      0x600C0010,  //  0014  GETGBL	R3	G16
      0x5C100200,  //  0015  MOVE	R4	R1
      0x7C0C0200,  //  0016  CALL	R3	1
      0xA8020008,  //  0017  EXBLK	0	#0021
      0x5C100600,  //  0018  MOVE	R4	R3
      0x7C100000,  //  0019  CALL	R4	0
      0x8C140102,  //  001A  GETMET	R5	R0	K2
      0x601C0009,  //  001B  GETGBL	R7	G9
      0x5C200800,  //  001C  MOVE	R8	R4
      0x7C1C0200,  //  001D  CALL	R7	1
      0x54220003,  //  001E  LDINT	R8	4
      0x7C140600,  //  001F  CALL	R5	3
      0x7001FFF6,  //  0020  JMP		#0018
      0x580C0003,  //  0021  LDCONST	R3	K3
      0xAC0C0200,  //  0022  CATCH	R3	1	0
      0xB0080000,  //  0023  RAISE	2	R0	R0
      0x70020013,  //  0024  JMP		#0039
      0x600C0004,  //  0025  GETGBL	R3	G4
      0x5C100200,  //  0026  MOVE	R4	R1
      0x7C0C0200,  //  0027  CALL	R3	1
      0x1C0C0704,  //  0028  EQ	R3	R3	K4
      0x780E000D,  //  0029  JMPF	R3	#0038
      0x600C0004,  //  002A  GETGBL	R3	G4
      0x5C100400,  //  002B  MOVE	R4	R2
      0x7C0C0200,  //  002C  CALL	R3	1
      0x1C0C0705,  //  002D  EQ	R3	R3	K5
      0x780E0008,  //  002E  JMPF	R3	#0038
      0x600C0003,  //  002F  GETGBL	R3	G3
      0x5C100000,  //  0030  MOVE	R4	R0
      0x7C0C0200,  //  0031  CALL	R3	1
      0x8C0C0701,  //  0032  GETMET	R3	R3	K1
      0x5C140200,  //  0033  MOVE	R5	R1
      0x541A0003,  //  0034  LDINT	R6	4
      0x08180406,  //  0035  MUL	R6	R2	R6
      0x7C0C0600,  //  0036  CALL	R3	3
      0x70020000,  //  0037  JMP		#0039
      0xB0060D07,  //  0038  RAISE	1	K6	K7
      0x80000000,  //  0039  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: item
********************************************************************/
be_local_closure(class_lv_int_arr_item,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(get),
    }),
    be_str_weak(item),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x54120003,  //  0001  LDINT	R4	4
      0x08100204,  //  0002  MUL	R4	R1	R4
      0x54160003,  //  0003  LDINT	R5	4
      0x7C080600,  //  0004  CALL	R2	3
      0x80040400,  //  0005  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: setitem
********************************************************************/
be_local_closure(class_lv_int_arr_setitem,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(set),
    }),
    be_str_weak(setitem),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x8C0C0100,  //  0000  GETMET	R3	R0	K0
      0x54160003,  //  0001  LDINT	R5	4
      0x08140205,  //  0002  MUL	R5	R1	R5
      0x5C180400,  //  0003  MOVE	R6	R2
      0x541E0003,  //  0004  LDINT	R7	4
      0x7C0C0800,  //  0005  CALL	R3	4
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: lv_int_arr
********************************************************************/
extern const bclass be_class_bytes;
be_local_class(lv_int_arr,
    0,
    &be_class_bytes,
    be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(init, -1), be_const_closure(class_lv_int_arr_init_closure) },
        { be_const_key_weak(item, -1), be_const_closure(class_lv_int_arr_item_closure) },
        { be_const_key_weak(setitem, -1), be_const_closure(class_lv_int_arr_setitem_closure) },
    })),
    be_str_weak(lv_int_arr)
);

extern const bclass be_class_lv_point_arr;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_lv_point_arr_init,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[11]) {     /* constants */
    /* K0   */  be_nested_str_weak(instance),
    /* K1   */  be_nested_str_weak(value_error),
    /* K2   */  be_nested_str_weak(argument_X20must_X20be_X20a_X20list),
    /* K3   */  be_nested_str_weak(init),
    /* K4   */  be_nested_str_weak(lv),
    /* K5   */  be_nested_str_weak(lv_point),
    /* K6   */  be_nested_str_weak(elements_X20must_X20be_X20a_X20lv_point),
    /* K7   */  be_nested_str_weak(add),
    /* K8   */  be_nested_str_weak(x),
    /* K9   */  be_nested_str_weak(y),
    /* K10  */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[52]) {  /* code */
      0x60080004,  //  0000  GETGBL	R2	G4
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x7C080200,  //  0002  CALL	R2	1
      0x20080500,  //  0003  NE	R2	R2	K0
      0x740A0004,  //  0004  JMPT	R2	#000A
      0x6008000F,  //  0005  GETGBL	R2	G15
      0x5C0C0200,  //  0006  MOVE	R3	R1
      0x60100012,  //  0007  GETGBL	R4	G18
      0x7C080400,  //  0008  CALL	R2	2
      0x740A0000,  //  0009  JMPT	R2	#000B
      0xB0060302,  //  000A  RAISE	1	K1	K2
      0x60080003,  //  000B  GETGBL	R2	G3
      0x5C0C0000,  //  000C  MOVE	R3	R0
      0x7C080200,  //  000D  CALL	R2	1
      0x8C080503,  //  000E  GETMET	R2	R2	K3
      0x6010000C,  //  000F  GETGBL	R4	G12
      0x5C140200,  //  0010  MOVE	R5	R1
      0x7C100200,  //  0011  CALL	R4	1
      0x54160007,  //  0012  LDINT	R5	8
      0x08100805,  //  0013  MUL	R4	R4	R5
      0x7C080400,  //  0014  CALL	R2	2
      0x60080010,  //  0015  GETGBL	R2	G16
      0x5C0C0200,  //  0016  MOVE	R3	R1
      0x7C080200,  //  0017  CALL	R2	1
      0xA8020016,  //  0018  EXBLK	0	#0030
      0x5C0C0400,  //  0019  MOVE	R3	R2
      0x7C0C0000,  //  001A  CALL	R3	0
      0x60100004,  //  001B  GETGBL	R4	G4
      0x5C140600,  //  001C  MOVE	R5	R3
      0x7C100200,  //  001D  CALL	R4	1
      0x20100900,  //  001E  NE	R4	R4	K0
      0x74120005,  //  001F  JMPT	R4	#0026
      0x6010000F,  //  0020  GETGBL	R4	G15
      0x5C140600,  //  0021  MOVE	R5	R3
      0xB81A0800,  //  0022  GETNGBL	R6	K4
      0x88180D05,  //  0023  GETMBR	R6	R6	K5
      0x7C100400,  //  0024  CALL	R4	2
      0x74120000,  //  0025  JMPT	R4	#0027
      0xB0060306,  //  0026  RAISE	1	K1	K6
      0x8C100107,  //  0027  GETMET	R4	R0	K7
      0x88180708,  //  0028  GETMBR	R6	R3	K8
      0x541E0003,  //  0029  LDINT	R7	4
      0x7C100600,  //  002A  CALL	R4	3
      0x8C100107,  //  002B  GETMET	R4	R0	K7
      0x88180709,  //  002C  GETMBR	R6	R3	K9
      0x541E0003,  //  002D  LDINT	R7	4
      0x7C100600,  //  002E  CALL	R4	3
      0x7001FFE8,  //  002F  JMP		#0019
      0x5808000A,  //  0030  LDCONST	R2	K10
      0xAC080200,  //  0031  CATCH	R2	1	0
      0xB0080000,  //  0032  RAISE	2	R0	R0
      0x80000000,  //  0033  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: lv_point_arr
********************************************************************/
extern const bclass be_class_bytes;
be_local_class(lv_point_arr,
    0,
    &be_class_bytes,
    be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(init, -1), be_const_closure(class_lv_point_arr_init_closure) },
    })),
    be_str_weak(lv_point_arr)
);

/********************************************************************
** Solidified function: _anonymous_
********************************************************************/
be_local_closure(_anonymous_,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[12]) {     /* constants */
    /* K0   */  be_nested_str_weak(global),
    /* K1   */  be_nested_str_weak(lv),
    /* K2   */  be_nested_str_weak(lv_coord_arr),
    /* K3   */  be_nested_str_weak(lv_int_arr),
    /* K4   */  be_nested_str_weak(lv_point_arr),
    /* K5   */  be_nested_str_weak(coord_arr),
    /* K6   */  be_nested_str_weak(int_arr),
    /* K7   */  be_nested_str_weak(point_arr),
    /* K8   */  be_nested_str_weak(style_prop_arr),
    /* K9   */  be_nested_str_weak(lv_style_prop_arr),
    /* K10  */  be_nested_str_weak(str_arr),
    /* K11  */  be_nested_str_weak(lv_str_arr),
    }),
    be_str_weak(_anonymous_),
    &be_const_str_solidified,
    ( &(const binstruction[19]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x88080301,  //  0001  GETMBR	R2	R1	K1
      0x880C0102,  //  0002  GETMBR	R3	R0	K2
      0x900A0403,  //  0003  SETMBR	R2	K2	R3
      0x880C0103,  //  0004  GETMBR	R3	R0	K3
      0x900A0603,  //  0005  SETMBR	R2	K3	R3
      0x880C0104,  //  0006  GETMBR	R3	R0	K4
      0x900A0803,  //  0007  SETMBR	R2	K4	R3
      0x880C0102,  //  0008  GETMBR	R3	R0	K2
      0x900A0A03,  //  0009  SETMBR	R2	K5	R3
      0x880C0103,  //  000A  GETMBR	R3	R0	K3
      0x900A0C03,  //  000B  SETMBR	R2	K6	R3
      0x880C0104,  //  000C  GETMBR	R3	R0	K4
      0x900A0E03,  //  000D  SETMBR	R2	K7	R3
      0x880C0109,  //  000E  GETMBR	R3	R0	K9
      0x900A1003,  //  000F  SETMBR	R2	K8	R3
      0x880C010B,  //  0010  GETMBR	R3	R0	K11
      0x900A1403,  //  0011  SETMBR	R2	K10	R3
      0x80040000,  //  0012  RET	1	R0
    })
  )
);
/*******************************************************************/


extern const bclass be_class_lv_style_prop_arr;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_lv_style_prop_arr_init,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(instance),
    /* K1   */  be_nested_str_weak(value_error),
    /* K2   */  be_nested_str_weak(argument_X20must_X20be_X20a_X20list),
    /* K3   */  be_nested_str_weak(init),
    /* K4   */  be_nested_str_weak(add),
    /* K5   */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[38]) {  /* code */
      0x60080004,  //  0000  GETGBL	R2	G4
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x7C080200,  //  0002  CALL	R2	1
      0x20080500,  //  0003  NE	R2	R2	K0
      0x740A0004,  //  0004  JMPT	R2	#000A
      0x6008000F,  //  0005  GETGBL	R2	G15
      0x5C0C0200,  //  0006  MOVE	R3	R1
      0x60100012,  //  0007  GETGBL	R4	G18
      0x7C080400,  //  0008  CALL	R2	2
      0x740A0000,  //  0009  JMPT	R2	#000B
      0xB0060302,  //  000A  RAISE	1	K1	K2
      0x60080003,  //  000B  GETGBL	R2	G3
      0x5C0C0000,  //  000C  MOVE	R3	R0
      0x7C080200,  //  000D  CALL	R2	1
      0x8C080503,  //  000E  GETMET	R2	R2	K3
      0x6010000C,  //  000F  GETGBL	R4	G12
      0x5C140200,  //  0010  MOVE	R5	R1
      0x7C100200,  //  0011  CALL	R4	1
      0x54160003,  //  0012  LDINT	R5	4
      0x08100805,  //  0013  MUL	R4	R4	R5
      0x7C080400,  //  0014  CALL	R2	2
      0x60080010,  //  0015  GETGBL	R2	G16
      0x5C0C0200,  //  0016  MOVE	R3	R1
      0x7C080200,  //  0017  CALL	R2	1
      0xA8020008,  //  0018  EXBLK	0	#0022
      0x5C0C0400,  //  0019  MOVE	R3	R2
      0x7C0C0000,  //  001A  CALL	R3	0
      0x8C100104,  //  001B  GETMET	R4	R0	K4
      0x60180009,  //  001C  GETGBL	R6	G9
      0x5C1C0600,  //  001D  MOVE	R7	R3
      0x7C180200,  //  001E  CALL	R6	1
      0x541E0003,  //  001F  LDINT	R7	4
      0x7C100600,  //  0020  CALL	R4	3
      0x7001FFF6,  //  0021  JMP		#0019
      0x58080005,  //  0022  LDCONST	R2	K5
      0xAC080200,  //  0023  CATCH	R2	1	0
      0xB0080000,  //  0024  RAISE	2	R0	R0
      0x80000000,  //  0025  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: lv_style_prop_arr
********************************************************************/
extern const bclass be_class_bytes;
be_local_class(lv_style_prop_arr,
    0,
    &be_class_bytes,
    be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(init, -1), be_const_closure(class_lv_style_prop_arr_init_closure) },
    })),
    be_str_weak(lv_style_prop_arr)
);

extern const bclass be_class_lv_coord_arr;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_lv_coord_arr_init,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(instance),
    /* K1   */  be_nested_str_weak(init),
    /* K2   */  be_nested_str_weak(add),
    /* K3   */  be_nested_str_weak(stop_iteration),
    /* K4   */  be_nested_str_weak(ptr),
    /* K5   */  be_nested_str_weak(int),
    /* K6   */  be_nested_str_weak(value_error),
    /* K7   */  be_nested_str_weak(argument_X20must_X20be_X20a_X20list_X20or_X20a_X20pointer_X2Bsize),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[58]) {  /* code */
      0x600C0004,  //  0000  GETGBL	R3	G4
      0x5C100200,  //  0001  MOVE	R4	R1
      0x7C0C0200,  //  0002  CALL	R3	1
      0x1C0C0700,  //  0003  EQ	R3	R3	K0
      0x780E001F,  //  0004  JMPF	R3	#0025
      0x600C000F,  //  0005  GETGBL	R3	G15
      0x5C100200,  //  0006  MOVE	R4	R1
      0x60140012,  //  0007  GETGBL	R5	G18
      0x7C0C0400,  //  0008  CALL	R3	2
      0x780E001A,  //  0009  JMPF	R3	#0025
      0x600C0003,  //  000A  GETGBL	R3	G3
      0x5C100000,  //  000B  MOVE	R4	R0
      0x7C0C0200,  //  000C  CALL	R3	1
      0x8C0C0701,  //  000D  GETMET	R3	R3	K1
      0x6014000C,  //  000E  GETGBL	R5	G12
      0x5C180200,  //  000F  MOVE	R6	R1
      0x7C140200,  //  0010  CALL	R5	1
      0x541A0003,  //  0011  LDINT	R6	4
      0x08140A06,  //  0012  MUL	R5	R5	R6
      0x7C0C0400,  //  0013  CALL	R3	2
      0x600C0010,  //  0014  GETGBL	R3	G16
      0x5C100200,  //  0015  MOVE	R4	R1
      0x7C0C0200,  //  0016  CALL	R3	1
      0xA8020008,  //  0017  EXBLK	0	#0021
      0x5C100600,  //  0018  MOVE	R4	R3
      0x7C100000,  //  0019  CALL	R4	0
      0x8C140102,  //  001A  GETMET	R5	R0	K2
      0x601C0009,  //  001B  GETGBL	R7	G9
      0x5C200800,  //  001C  MOVE	R8	R4
      0x7C1C0200,  //  001D  CALL	R7	1
      0x54220003,  //  001E  LDINT	R8	4
      0x7C140600,  //  001F  CALL	R5	3
      0x7001FFF6,  //  0020  JMP		#0018
      0x580C0003,  //  0021  LDCONST	R3	K3
      0xAC0C0200,  //  0022  CATCH	R3	1	0
      0xB0080000,  //  0023  RAISE	2	R0	R0
      0x70020013,  //  0024  JMP		#0039
      0x600C0004,  //  0025  GETGBL	R3	G4
      0x5C100200,  //  0026  MOVE	R4	R1
      0x7C0C0200,  //  0027  CALL	R3	1
      0x1C0C0704,  //  0028  EQ	R3	R3	K4
      0x780E000D,  //  0029  JMPF	R3	#0038
      0x600C0004,  //  002A  GETGBL	R3	G4
      0x5C100400,  //  002B  MOVE	R4	R2
      0x7C0C0200,  //  002C  CALL	R3	1
      0x1C0C0705,  //  002D  EQ	R3	R3	K5
      0x780E0008,  //  002E  JMPF	R3	#0038
      0x600C0003,  //  002F  GETGBL	R3	G3
      0x5C100000,  //  0030  MOVE	R4	R0
      0x7C0C0200,  //  0031  CALL	R3	1
      0x8C0C0701,  //  0032  GETMET	R3	R3	K1
      0x5C140200,  //  0033  MOVE	R5	R1
      0x541A0003,  //  0034  LDINT	R6	4
      0x08180406,  //  0035  MUL	R6	R2	R6
      0x7C0C0600,  //  0036  CALL	R3	3
      0x70020000,  //  0037  JMP		#0039
      0xB0060D07,  //  0038  RAISE	1	K6	K7
      0x80000000,  //  0039  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: item
********************************************************************/
be_local_closure(class_lv_coord_arr_item,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(get),
    }),
    be_str_weak(item),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x54120003,  //  0001  LDINT	R4	4
      0x08100204,  //  0002  MUL	R4	R1	R4
      0x54160003,  //  0003  LDINT	R5	4
      0x7C080600,  //  0004  CALL	R2	3
      0x80040400,  //  0005  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: setitem
********************************************************************/
be_local_closure(class_lv_coord_arr_setitem,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(set),
    }),
    be_str_weak(setitem),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x8C0C0100,  //  0000  GETMET	R3	R0	K0
      0x54160003,  //  0001  LDINT	R5	4
      0x08140205,  //  0002  MUL	R5	R1	R5
      0x5C180400,  //  0003  MOVE	R6	R2
      0x541E0003,  //  0004  LDINT	R7	4
      0x7C0C0800,  //  0005  CALL	R3	4
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: lv_coord_arr
********************************************************************/
extern const bclass be_class_bytes;
be_local_class(lv_coord_arr,
    0,
    &be_class_bytes,
    be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(init, -1), be_const_closure(class_lv_coord_arr_init_closure) },
        { be_const_key_weak(item, -1), be_const_closure(class_lv_coord_arr_item_closure) },
        { be_const_key_weak(setitem, -1), be_const_closure(class_lv_coord_arr_setitem_closure) },
    })),
    be_str_weak(lv_coord_arr)
);

/********************************************************************
** Solidified module: lv_extra
********************************************************************/
be_local_module(lv_extra,
    "lv_extra",
    be_nested_map(6,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(lv_coord_arr, 4), be_const_class(be_class_lv_coord_arr) },
        { be_const_key_weak(lv_int_arr, -1), be_const_class(be_class_lv_int_arr) },
        { be_const_key_weak(lv_point_arr, -1), be_const_class(be_class_lv_point_arr) },
        { be_const_key_weak(init, -1), be_const_closure(_anonymous__closure) },
        { be_const_key_weak(lv_style_prop_arr, -1), be_const_class(be_class_lv_style_prop_arr) },
        { be_const_key_weak(lv_str_arr, 0), be_const_class(be_class_lv_str_arr) },
    }))
);
BE_EXPORT_VARIABLE be_define_const_native_module(lv_extra);
/********************************************************************/
/********************************************************************/
/* End of solidification */
