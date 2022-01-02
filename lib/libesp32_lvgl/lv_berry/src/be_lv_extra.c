/********************************************************************
 * Tasmota LVGL lv_signal_bars widget
 *******************************************************************/
#include "be_constobj.h"

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(lv_coord_arr_init,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str(instance),
    /* K1   */  be_nested_str(value_error),
    /* K2   */  be_nested_str(argument_X20must_X20be_X20a_X20list),
    /* K3   */  be_nested_str(init),
    /* K4   */  be_const_int(2),
    /* K5   */  be_nested_str(add),
    /* K6   */  be_nested_str(stop_iteration),
    }),
    &be_const_str_init,
    &be_const_str_solidified,
    ( &(const binstruction[37]) {  /* code */
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
      0x08100904,  //  0012  MUL	R4	R4	K4
      0x7C080400,  //  0013  CALL	R2	2
      0x60080010,  //  0014  GETGBL	R2	G16
      0x5C0C0200,  //  0015  MOVE	R3	R1
      0x7C080200,  //  0016  CALL	R2	1
      0xA8020008,  //  0017  EXBLK	0	#0021
      0x5C0C0400,  //  0018  MOVE	R3	R2
      0x7C0C0000,  //  0019  CALL	R3	0
      0x8C100105,  //  001A  GETMET	R4	R0	K5
      0x60180009,  //  001B  GETGBL	R6	G9
      0x5C1C0600,  //  001C  MOVE	R7	R3
      0x7C180200,  //  001D  CALL	R6	1
      0x581C0004,  //  001E  LDCONST	R7	K4
      0x7C100600,  //  001F  CALL	R4	3
      0x7001FFF6,  //  0020  JMP		#0018
      0x58080006,  //  0021  LDCONST	R2	K6
      0xAC080200,  //  0022  CATCH	R2	1	0
      0xB0080000,  //  0023  RAISE	2	R0	R0
      0x80000000,  //  0024  RET	0
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
    be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(init, -1), be_const_closure(lv_coord_arr_init_closure) },
    })),
    be_str_literal("lv_coord_arr")
);

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(lv_point_arr_init,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[12]) {     /* constants */
    /* K0   */  be_nested_str(instance),
    /* K1   */  be_nested_str(value_error),
    /* K2   */  be_nested_str(argument_X20must_X20be_X20a_X20list),
    /* K3   */  be_nested_str(init),
    /* K4   */  be_nested_str(lv),
    /* K5   */  be_nested_str(lv_point),
    /* K6   */  be_nested_str(elements_X20must_X20be_X20a_X20lv_point),
    /* K7   */  be_nested_str(add),
    /* K8   */  be_nested_str(x),
    /* K9   */  be_const_int(2),
    /* K10  */  be_nested_str(y),
    /* K11  */  be_nested_str(stop_iteration),
    }),
    &be_const_str_init,
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
      0x54160003,  //  0012  LDINT	R5	4
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
      0x581C0009,  //  0029  LDCONST	R7	K9
      0x7C100600,  //  002A  CALL	R4	3
      0x8C100107,  //  002B  GETMET	R4	R0	K7
      0x8818070A,  //  002C  GETMBR	R6	R3	K10
      0x581C0009,  //  002D  LDCONST	R7	K9
      0x7C100600,  //  002E  CALL	R4	3
      0x7001FFE8,  //  002F  JMP		#0019
      0x5808000B,  //  0030  LDCONST	R2	K11
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
        { be_const_key(init, -1), be_const_closure(lv_point_arr_init_closure) },
    })),
    be_str_literal("lv_point_arr")
);

/********************************************************************
** Solidified function: _anonymous_
********************************************************************/
be_local_closure(lv_extra__anonymous_,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str(global),
    /* K1   */  be_nested_str(lv),
    /* K2   */  be_nested_str(lv_coord_arr),
    /* K3   */  be_nested_str(lv_point_arr),
    /* K4   */  be_nested_str(coord_arr),
    /* K5   */  be_nested_str(point_arr),
    }),
    &be_const_str__anonymous_,
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x88080301,  //  0001  GETMBR	R2	R1	K1
      0x880C0102,  //  0002  GETMBR	R3	R0	K2
      0x900A0403,  //  0003  SETMBR	R2	K2	R3
      0x880C0103,  //  0004  GETMBR	R3	R0	K3
      0x900A0603,  //  0005  SETMBR	R2	K3	R3
      0x880C0102,  //  0006  GETMBR	R3	R0	K2
      0x900A0803,  //  0007  SETMBR	R2	K4	R3
      0x880C0103,  //  0008  GETMBR	R3	R0	K3
      0x900A0A03,  //  0009  SETMBR	R2	K5	R3
      0x80040000,  //  000A  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified module: lv_extra
********************************************************************/
be_local_module(lv_extra,
    "lv_extra",
    be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(init, -1), be_const_closure(lv_extra__anonymous__closure) },
        { be_const_key(lv_point_arr, -1), be_const_class(be_class_lv_point_arr) },
        { be_const_key(lv_coord_arr, 1), be_const_class(be_class_lv_coord_arr) },
    }))
);
BE_EXPORT_VARIABLE be_define_const_native_module(lv_extra);
/********************************************************************/
