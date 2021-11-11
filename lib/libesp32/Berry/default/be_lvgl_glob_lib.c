/********************************************************************
 * Tasmota LVGL lv_signal_bars widget
 *******************************************************************/
#include "be_constobj.h"

#ifdef USE_LVGL

#include "lvgl.h"

/********************************************************************
** Solidified function: get_object_from_ptr
********************************************************************/
be_local_closure(get_object_from_ptr,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_string("cb_obj", 1195696482, 6),
    /* K1   */  be_nested_string("find", -1108310694, 4),
    }),
    (be_nested_const_str("get_object_from_ptr", -1949948095, 19)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[10]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x4C0C0000,  //  0001  LDNIL	R3
      0x20080403,  //  0002  NE	R2	R2	R3
      0x780A0004,  //  0003  JMPF	R2	#0009
      0x88080100,  //  0004  GETMBR	R2	R0	K0
      0x8C080501,  //  0005  GETMET	R2	R2	K1
      0x5C100200,  //  0006  MOVE	R4	R1
      0x7C080400,  //  0007  CALL	R2	2
      0x80040400,  //  0008  RET	1	R2
      0x80000000,  //  0009  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: widget_event_impl
********************************************************************/
be_local_closure(widget_event_impl,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    3,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_string("introspect", 164638290, 10),
    /* K1   */  be_nested_string("lv", 1529997255, 2),
    /* K2   */  be_nested_string("lv_obj_class", -255311002, 12),
    /* K3   */  be_nested_string("lv_event", -1860877328, 8),
    /* K4   */  be_nested_string("target", 845187144, 6),
    /* K5   */  be_nested_string("get_object_from_ptr", -1949948095, 19),
    /* K6   */  be_nested_string("instance", 193386898, 8),
    /* K7   */  be_nested_string("get", 1410115415, 3),
    /* K8   */  be_nested_string("widget_event", 1951408186, 12),
    }),
    (be_nested_const_str("widget_event_impl", -2116536735, 17)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[30]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xB8120200,  //  0001  GETNGBL	R4	K1
      0x8C100902,  //  0002  GETMET	R4	R4	K2
      0x5C180200,  //  0003  MOVE	R6	R1
      0x7C100400,  //  0004  CALL	R4	2
      0xB8160200,  //  0005  GETNGBL	R5	K1
      0x8C140B03,  //  0006  GETMET	R5	R5	K3
      0x5C1C0400,  //  0007  MOVE	R7	R2
      0x7C140400,  //  0008  CALL	R5	2
      0x88180B04,  //  0009  GETMBR	R6	R5	K4
      0x8C1C0105,  //  000A  GETMET	R7	R0	K5
      0x60240009,  //  000B  GETGBL	R9	G9
      0x5C280C00,  //  000C  MOVE	R10	R6
      0x7C240200,  //  000D  CALL	R9	1
      0x7C1C0400,  //  000E  CALL	R7	2
      0x60200004,  //  000F  GETGBL	R8	G4
      0x5C240E00,  //  0010  MOVE	R9	R7
      0x7C200200,  //  0011  CALL	R8	1
      0x1C201106,  //  0012  EQ	R8	R8	K6
      0x78220008,  //  0013  JMPF	R8	#001D
      0x8C200707,  //  0014  GETMET	R8	R3	K7
      0x5C280E00,  //  0015  MOVE	R10	R7
      0x582C0008,  //  0016  LDCONST	R11	K8
      0x7C200600,  //  0017  CALL	R8	3
      0x78220003,  //  0018  JMPF	R8	#001D
      0x8C200F08,  //  0019  GETMET	R8	R7	K8
      0x5C280800,  //  001A  MOVE	R10	R4
      0x5C2C0A00,  //  001B  MOVE	R11	R5
      0x7C200600,  //  001C  CALL	R8	3
      0x80000000,  //  001D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: lvgl_event_dispatch
********************************************************************/
be_local_closure(lvgl_event_dispatch,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    2,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_string("introspect", 164638290, 10),
    /* K1   */  be_nested_string("lv", 1529997255, 2),
    /* K2   */  be_nested_string("lv_event", -1860877328, 8),
    /* K3   */  be_nested_string("toptr", -915119842, 5),
    /* K4   */  be_nested_string("target", 845187144, 6),
    /* K5   */  be_nested_string("cb_event_closure", -466699971, 16),
    /* K6   */  be_nested_string("get_object_from_ptr", -1949948095, 19),
    }),
    (be_nested_const_str("lvgl_event_dispatch", 2104396622, 19)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[20]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0xB80E0200,  //  0001  GETNGBL	R3	K1
      0x8C0C0702,  //  0002  GETMET	R3	R3	K2
      0x8C140503,  //  0003  GETMET	R5	R2	K3
      0x5C1C0200,  //  0004  MOVE	R7	R1
      0x7C140400,  //  0005  CALL	R5	2
      0x7C0C0400,  //  0006  CALL	R3	2
      0x60100009,  //  0007  GETGBL	R4	G9
      0x88140704,  //  0008  GETMBR	R5	R3	K4
      0x7C100200,  //  0009  CALL	R4	1
      0x88140105,  //  000A  GETMBR	R5	R0	K5
      0x94140A04,  //  000B  GETIDX	R5	R5	R4
      0x8C180106,  //  000C  GETMET	R6	R0	K6
      0x5C200800,  //  000D  MOVE	R8	R4
      0x7C180400,  //  000E  CALL	R6	2
      0x5C1C0A00,  //  000F  MOVE	R7	R5
      0x5C200C00,  //  0010  MOVE	R8	R6
      0x5C240600,  //  0011  MOVE	R9	R3
      0x7C1C0400,  //  0012  CALL	R7	2
      0x80000000,  //  0013  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: widget_dtor_impl
********************************************************************/
be_local_closure(widget_dtor_impl,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    3,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_string("introspect", 164638290, 10),
    /* K1   */  be_nested_string("lv", 1529997255, 2),
    /* K2   */  be_nested_string("lv_obj_class", -255311002, 12),
    /* K3   */  be_nested_string("get_object_from_ptr", -1949948095, 19),
    /* K4   */  be_nested_string("instance", 193386898, 8),
    /* K5   */  be_nested_string("get", 1410115415, 3),
    /* K6   */  be_nested_string("widget_destructor", -87578951, 17),
    }),
    (be_nested_const_str("widget_dtor_impl", 520430610, 16)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[22]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xB8120200,  //  0001  GETNGBL	R4	K1
      0x8C100902,  //  0002  GETMET	R4	R4	K2
      0x5C180200,  //  0003  MOVE	R6	R1
      0x7C100400,  //  0004  CALL	R4	2
      0x8C140103,  //  0005  GETMET	R5	R0	K3
      0x5C1C0400,  //  0006  MOVE	R7	R2
      0x7C140400,  //  0007  CALL	R5	2
      0x60180004,  //  0008  GETGBL	R6	G4
      0x5C1C0A00,  //  0009  MOVE	R7	R5
      0x7C180200,  //  000A  CALL	R6	1
      0x1C180D04,  //  000B  EQ	R6	R6	K4
      0x781A0007,  //  000C  JMPF	R6	#0015
      0x8C180705,  //  000D  GETMET	R6	R3	K5
      0x5C200A00,  //  000E  MOVE	R8	R5
      0x58240006,  //  000F  LDCONST	R9	K6
      0x7C180600,  //  0010  CALL	R6	3
      0x781A0002,  //  0011  JMPF	R6	#0015
      0x8C180B06,  //  0012  GETMET	R6	R5	K6
      0x5C200800,  //  0013  MOVE	R8	R4
      0x7C180400,  //  0014  CALL	R6	2
      0x80000000,  //  0015  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: register_obj
********************************************************************/
be_local_closure(register_obj,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_string("cb_obj", 1195696482, 6),
    /* K1   */  be_nested_string("_p", 1594591802, 2),
    }),
    (be_nested_const_str("register_obj", -312352526, 12)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[13]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x4C0C0000,  //  0001  LDNIL	R3
      0x1C080403,  //  0002  EQ	R2	R2	R3
      0x780A0002,  //  0003  JMPF	R2	#0007
      0x60080013,  //  0004  GETGBL	R2	G19
      0x7C080000,  //  0005  CALL	R2	0
      0x90020002,  //  0006  SETMBR	R0	K0	R2
      0x60080009,  //  0007  GETGBL	R2	G9
      0x880C0301,  //  0008  GETMBR	R3	R1	K1
      0x7C080200,  //  0009  CALL	R2	1
      0x880C0100,  //  000A  GETMBR	R3	R0	K0
      0x980C0401,  //  000B  SETIDX	R3	R2	R1
      0x80000000,  //  000C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: gen_cb
********************************************************************/
be_local_closure(gen_cb,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    5,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 1]) {
      be_nested_proto(
        4,                          /* nstack */
        1,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 1]) {     /* constants */
        /* K0   */  be_nested_string("lvgl_event_dispatch", 2104396622, 19),
        }),
        (be_nested_const_str("<lambda>", 607256038, 8)),
        ((bstring*) &be_const_str_input),
        ( &(const binstruction[ 5]) {  /* code */
          0x68040000,  //  0000  GETUPV	R1	U0
          0x8C040300,  //  0001  GETMET	R1	R1	K0
          0x5C0C0000,  //  0002  MOVE	R3	R0
          0x7C040400,  //  0003  CALL	R1	2
          0x80040200,  //  0004  RET	1	R1
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_string("lv_event_cb", -1814236280, 11),
    /* K1   */  be_nested_string("cb_event_closure", -466699971, 16),
    /* K2   */  be_nested_string("event_cb", -1166269279, 8),
    /* K3   */  be_nested_string("tasmota", 424643812, 7),
    /* K4   */  be_nested_string("gen_cb", -1049739745, 6),
    /* K5   */  be_nested_string("register_obj", -312352526, 12),
    /* K6   */  be_nested_string("null_cb", -1961430836, 7),
    /* K7   */  be_nested_string("cb_do_nothing", 1488730702, 13),
    }),
    (be_nested_const_str("gen_cb", -1049739745, 6)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[41]) {  /* code */
      0x1C140300,  //  0000  EQ	R5	R1	K0
      0x78160018,  //  0001  JMPF	R5	#001B
      0x88140101,  //  0002  GETMBR	R5	R0	K1
      0x4C180000,  //  0003  LDNIL	R6
      0x1C140A06,  //  0004  EQ	R5	R5	R6
      0x78160002,  //  0005  JMPF	R5	#0009
      0x60140013,  //  0006  GETGBL	R5	G19
      0x7C140000,  //  0007  CALL	R5	0
      0x90020205,  //  0008  SETMBR	R0	K1	R5
      0x88140102,  //  0009  GETMBR	R5	R0	K2
      0x4C180000,  //  000A  LDNIL	R6
      0x1C140A06,  //  000B  EQ	R5	R5	R6
      0x78160004,  //  000C  JMPF	R5	#0012
      0xB8160600,  //  000D  GETNGBL	R5	K3
      0x8C140B04,  //  000E  GETMET	R5	R5	K4
      0x841C0000,  //  000F  CLOSURE	R7	P0
      0x7C140400,  //  0010  CALL	R5	2
      0x90020405,  //  0011  SETMBR	R0	K2	R5
      0x8C140105,  //  0012  GETMET	R5	R0	K5
      0x5C1C0600,  //  0013  MOVE	R7	R3
      0x7C140400,  //  0014  CALL	R5	2
      0x88140101,  //  0015  GETMBR	R5	R0	K1
      0x98140802,  //  0016  SETIDX	R5	R4	R2
      0x88140102,  //  0017  GETMBR	R5	R0	K2
      0xA0000000,  //  0018  CLOSE	R0
      0x80040A00,  //  0019  RET	1	R5
      0x7002000B,  //  001A  JMP		#0027
      0x88140106,  //  001B  GETMBR	R5	R0	K6
      0x4C180000,  //  001C  LDNIL	R6
      0x1C140A06,  //  001D  EQ	R5	R5	R6
      0x78160004,  //  001E  JMPF	R5	#0024
      0xB8160600,  //  001F  GETNGBL	R5	K3
      0x8C140B04,  //  0020  GETMET	R5	R5	K4
      0x881C0107,  //  0021  GETMBR	R7	R0	K7
      0x7C140400,  //  0022  CALL	R5	2
      0x90020C05,  //  0023  SETMBR	R0	K6	R5
      0x88140106,  //  0024  GETMBR	R5	R0	K6
      0xA0000000,  //  0025  CLOSE	R0
      0x80040A00,  //  0026  RET	1	R5
      0xA0000000,  //  0027  CLOSE	R0
      0x80000000,  //  0028  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: deregister_obj
********************************************************************/
be_local_closure(deregister_obj,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_string("cb_obj", 1195696482, 6),
    /* K1   */  be_nested_string("remove", -611183107, 6),
    /* K2   */  be_nested_string("cb_event_closure", -466699971, 16),
    }),
    (be_nested_const_str("deregister_obj", -385000303, 14)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[17]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x4C0C0000,  //  0001  LDNIL	R3
      0x20080403,  //  0002  NE	R2	R2	R3
      0x780A0003,  //  0003  JMPF	R2	#0008
      0x88080100,  //  0004  GETMBR	R2	R0	K0
      0x8C080501,  //  0005  GETMET	R2	R2	K1
      0x5C100200,  //  0006  MOVE	R4	R1
      0x7C080400,  //  0007  CALL	R2	2
      0x88080102,  //  0008  GETMBR	R2	R0	K2
      0x4C0C0000,  //  0009  LDNIL	R3
      0x20080403,  //  000A  NE	R2	R2	R3
      0x780A0003,  //  000B  JMPF	R2	#0010
      0x88080102,  //  000C  GETMBR	R2	R0	K2
      0x8C080501,  //  000D  GETMET	R2	R2	K1
      0x5C100200,  //  000E  MOVE	R4	R1
      0x7C080400,  //  000F  CALL	R2	2
      0x80000000,  //  0010  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: widget_cb
********************************************************************/
be_local_closure(widget_cb,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 3]) {
      be_nested_proto(
        6,                          /* nstack */
        2,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 1]) {     /* constants */
        /* K0   */  be_nested_string("widget_ctor_impl", 194252479, 16),
        }),
        (be_nested_const_str("<lambda>", 607256038, 8)),
        ((bstring*) &be_const_str_input),
        ( &(const binstruction[ 6]) {  /* code */
          0x68080000,  //  0000  GETUPV	R2	U0
          0x8C080500,  //  0001  GETMET	R2	R2	K0
          0x5C100000,  //  0002  MOVE	R4	R0
          0x5C140200,  //  0003  MOVE	R5	R1
          0x7C080600,  //  0004  CALL	R2	3
          0x80040400,  //  0005  RET	1	R2
        })
      ),
      be_nested_proto(
        6,                          /* nstack */
        2,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 1]) {     /* constants */
        /* K0   */  be_nested_string("widget_dtor_impl", 520430610, 16),
        }),
        (be_nested_const_str("<lambda>", 607256038, 8)),
        ((bstring*) &be_const_str_input),
        ( &(const binstruction[ 6]) {  /* code */
          0x68080000,  //  0000  GETUPV	R2	U0
          0x8C080500,  //  0001  GETMET	R2	R2	K0
          0x5C100000,  //  0002  MOVE	R4	R0
          0x5C140200,  //  0003  MOVE	R5	R1
          0x7C080600,  //  0004  CALL	R2	3
          0x80040400,  //  0005  RET	1	R2
        })
      ),
      be_nested_proto(
        6,                          /* nstack */
        2,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 1]) {     /* constants */
        /* K0   */  be_nested_string("widget_event_impl", -2116536735, 17),
        }),
        (be_nested_const_str("<lambda>", 607256038, 8)),
        ((bstring*) &be_const_str_input),
        ( &(const binstruction[ 6]) {  /* code */
          0x68080000,  //  0000  GETUPV	R2	U0
          0x8C080500,  //  0001  GETMET	R2	R2	K0
          0x5C100000,  //  0002  MOVE	R4	R0
          0x5C140200,  //  0003  MOVE	R5	R1
          0x7C080600,  //  0004  CALL	R2	3
          0x80040400,  //  0005  RET	1	R2
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[15]) {     /* constants */
    /* K0   */  be_nested_string("widget_ctor_cb", 876007560, 14),
    /* K1   */  be_nested_string("tasmota", 424643812, 7),
    /* K2   */  be_nested_string("gen_cb", -1049739745, 6),
    /* K3   */  be_nested_string("widget_dtor_cb", -1143421451, 14),
    /* K4   */  be_nested_string("widget_event_cb", 1508466754, 15),
    /* K5   */  be_nested_string("widget_struct_default", 781673633, 21),
    /* K6   */  be_nested_string("lv", 1529997255, 2),
    /* K7   */  be_nested_string("lv_obj_class", -255311002, 12),
    /* K8   */  be_nested_string("lv_obj", -37134147, 6),
    /* K9   */  be_nested_string("_class", -1562820946, 6),
    /* K10  */  be_nested_string("copy", -446502332, 4),
    /* K11  */  be_nested_string("base_class", 1107737279, 10),
    /* K12  */  be_nested_string("constructor_cb", -1805861999, 14),
    /* K13  */  be_nested_string("destructor_cb", 1930283190, 13),
    /* K14  */  be_nested_string("event_cb", -1166269279, 8),
    }),
    (be_nested_const_str("widget_cb", -1531384241, 9)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[56]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x4C080000,  //  0001  LDNIL	R2
      0x1C040202,  //  0002  EQ	R1	R1	R2
      0x78060004,  //  0003  JMPF	R1	#0009
      0xB8060200,  //  0004  GETNGBL	R1	K1
      0x8C040302,  //  0005  GETMET	R1	R1	K2
      0x840C0000,  //  0006  CLOSURE	R3	P0
      0x7C040400,  //  0007  CALL	R1	2
      0x90020001,  //  0008  SETMBR	R0	K0	R1
      0x88040103,  //  0009  GETMBR	R1	R0	K3
      0x4C080000,  //  000A  LDNIL	R2
      0x1C040202,  //  000B  EQ	R1	R1	R2
      0x78060004,  //  000C  JMPF	R1	#0012
      0xB8060200,  //  000D  GETNGBL	R1	K1
      0x8C040302,  //  000E  GETMET	R1	R1	K2
      0x840C0001,  //  000F  CLOSURE	R3	P1
      0x7C040400,  //  0010  CALL	R1	2
      0x90020601,  //  0011  SETMBR	R0	K3	R1
      0x88040104,  //  0012  GETMBR	R1	R0	K4
      0x4C080000,  //  0013  LDNIL	R2
      0x1C040202,  //  0014  EQ	R1	R1	R2
      0x78060004,  //  0015  JMPF	R1	#001B
      0xB8060200,  //  0016  GETNGBL	R1	K1
      0x8C040302,  //  0017  GETMET	R1	R1	K2
      0x840C0002,  //  0018  CLOSURE	R3	P2
      0x7C040400,  //  0019  CALL	R1	2
      0x90020801,  //  001A  SETMBR	R0	K4	R1
      0x88040105,  //  001B  GETMBR	R1	R0	K5
      0x4C080000,  //  001C  LDNIL	R2
      0x1C040202,  //  001D  EQ	R1	R1	R2
      0x78060016,  //  001E  JMPF	R1	#0036
      0xB8060C00,  //  001F  GETNGBL	R1	K6
      0x8C040307,  //  0020  GETMET	R1	R1	K7
      0xB80E0C00,  //  0021  GETNGBL	R3	K6
      0x880C0708,  //  0022  GETMBR	R3	R3	K8
      0x880C0709,  //  0023  GETMBR	R3	R3	K9
      0x7C040400,  //  0024  CALL	R1	2
      0x8C04030A,  //  0025  GETMET	R1	R1	K10
      0x7C040200,  //  0026  CALL	R1	1
      0x90020A01,  //  0027  SETMBR	R0	K5	R1
      0x88040105,  //  0028  GETMBR	R1	R0	K5
      0xB80A0C00,  //  0029  GETNGBL	R2	K6
      0x88080508,  //  002A  GETMBR	R2	R2	K8
      0x88080509,  //  002B  GETMBR	R2	R2	K9
      0x90061602,  //  002C  SETMBR	R1	K11	R2
      0x88040105,  //  002D  GETMBR	R1	R0	K5
      0x88080100,  //  002E  GETMBR	R2	R0	K0
      0x90061802,  //  002F  SETMBR	R1	K12	R2
      0x88040105,  //  0030  GETMBR	R1	R0	K5
      0x88080103,  //  0031  GETMBR	R2	R0	K3
      0x90061A02,  //  0032  SETMBR	R1	K13	R2
      0x88040105,  //  0033  GETMBR	R1	R0	K5
      0x88080104,  //  0034  GETMBR	R2	R0	K4
      0x90061C02,  //  0035  SETMBR	R1	K14	R2
      0xA0000000,  //  0036  CLOSE	R0
      0x80000000,  //  0037  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _anonymous_
********************************************************************/
be_local_closure(_anonymous_,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    0,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_string("LVG: call to unsupported callback", 504176819, 33),
    }),
    (be_nested_const_str("_anonymous_", 1957281476, 11)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[ 4]) {  /* code */
      0x60000001,  //  0000  GETGBL	R0	G1
      0x58040000,  //  0001  LDCONST	R1	K0
      0x7C000200,  //  0002  CALL	R0	1
      0x80000000,  //  0003  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: create_custom_widget
********************************************************************/
be_local_closure(create_custom_widget,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    3,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[27]) {     /* constants */
    /* K0   */  be_nested_string("introspect", 164638290, 10),
    /* K1   */  be_nested_string("lv", 1529997255, 2),
    /* K2   */  be_nested_string("lv_obj", -37134147, 6),
    /* K3   */  be_nested_string("value_error", 773297791, 11),
    /* K4   */  be_nested_string("arg must be a subclass of lv_obj", 1641882079, 32),
    /* K5   */  be_nested_string("widget_struct_by_class", -488593454, 22),
    /* K6   */  be_nested_string("find", -1108310694, 4),
    /* K7   */  be_nested_string("widget_cb", -1531384241, 9),
    /* K8   */  be_nested_string("widget_struct_default", 781673633, 21),
    /* K9   */  be_nested_string("copy", -446502332, 4),
    /* K10  */  be_nested_string("base_class", 1107737279, 10),
    /* K11  */  be_nested_string("_class", -1562820946, 6),
    /* K12  */  be_nested_string("get", 1410115415, 3),
    /* K13  */  be_nested_string("widget_width_def", -308888434, 16),
    /* K14  */  be_nested_string("width_def", 1143717879, 9),
    /* K15  */  be_nested_string("widget_height_def", -1163299483, 17),
    /* K16  */  be_nested_string("height_def", -1946728458, 10),
    /* K17  */  be_nested_string("widget_editable", -473174010, 15),
    /* K18  */  be_nested_string("editable", 60532369, 8),
    /* K19  */  be_nested_string("widget_group_def", 1246968785, 16),
    /* K20  */  be_nested_string("group_def", 1524213328, 9),
    /* K21  */  be_nested_string("widget_instance_size", 2055354779, 20),
    /* K22  */  be_nested_string("instance_size", -14697778, 13),
    /* K23  */  be_nested_string("obj_class_create_obj", -990576664, 20),
    /* K24  */  be_nested_string("_p", 1594591802, 2),
    /* K25  */  be_nested_string("register_obj", -312352526, 12),
    /* K26  */  be_nested_string("class_init_obj", 178410604, 14),
    }),
    (be_nested_const_str("create_custom_widget", 1140594778, 20)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[86]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0x6010000F,  //  0001  GETGBL	R4	G15
      0x5C140200,  //  0002  MOVE	R5	R1
      0xB81A0200,  //  0003  GETNGBL	R6	K1
      0x88180D02,  //  0004  GETMBR	R6	R6	K2
      0x7C100400,  //  0005  CALL	R4	2
      0x74120000,  //  0006  JMPT	R4	#0008
      0xB0060704,  //  0007  RAISE	1	K3	K4
      0x88100105,  //  0008  GETMBR	R4	R0	K5
      0x4C140000,  //  0009  LDNIL	R5
      0x1C100805,  //  000A  EQ	R4	R4	R5
      0x78120002,  //  000B  JMPF	R4	#000F
      0x60100013,  //  000C  GETGBL	R4	G19
      0x7C100000,  //  000D  CALL	R4	0
      0x90020A04,  //  000E  SETMBR	R0	K5	R4
      0x60100005,  //  000F  GETGBL	R4	G5
      0x5C140200,  //  0010  MOVE	R5	R1
      0x7C100200,  //  0011  CALL	R4	1
      0x88140105,  //  0012  GETMBR	R5	R0	K5
      0x8C140B06,  //  0013  GETMET	R5	R5	K6
      0x5C1C0800,  //  0014  MOVE	R7	R4
      0x7C140400,  //  0015  CALL	R5	2
      0x4C180000,  //  0016  LDNIL	R6
      0x1C180A06,  //  0017  EQ	R6	R5	R6
      0x781A002F,  //  0018  JMPF	R6	#0049
      0x8C180107,  //  0019  GETMET	R6	R0	K7
      0x7C180200,  //  001A  CALL	R6	1
      0x88180108,  //  001B  GETMBR	R6	R0	K8
      0x8C180D09,  //  001C  GETMET	R6	R6	K9
      0x7C180200,  //  001D  CALL	R6	1
      0x5C140C00,  //  001E  MOVE	R5	R6
      0x60180003,  //  001F  GETGBL	R6	G3
      0x5C1C0200,  //  0020  MOVE	R7	R1
      0x7C180200,  //  0021  CALL	R6	1
      0x88180D0B,  //  0022  GETMBR	R6	R6	K11
      0x90161406,  //  0023  SETMBR	R5	K10	R6
      0x8C18070C,  //  0024  GETMET	R6	R3	K12
      0x5C200200,  //  0025  MOVE	R8	R1
      0x5824000D,  //  0026  LDCONST	R9	K13
      0x7C180600,  //  0027  CALL	R6	3
      0x781A0001,  //  0028  JMPF	R6	#002B
      0x8818030D,  //  0029  GETMBR	R6	R1	K13
      0x90161C06,  //  002A  SETMBR	R5	K14	R6
      0x8C18070C,  //  002B  GETMET	R6	R3	K12
      0x5C200200,  //  002C  MOVE	R8	R1
      0x5824000F,  //  002D  LDCONST	R9	K15
      0x7C180600,  //  002E  CALL	R6	3
      0x781A0001,  //  002F  JMPF	R6	#0032
      0x8818030F,  //  0030  GETMBR	R6	R1	K15
      0x90162006,  //  0031  SETMBR	R5	K16	R6
      0x8C18070C,  //  0032  GETMET	R6	R3	K12
      0x5C200200,  //  0033  MOVE	R8	R1
      0x58240011,  //  0034  LDCONST	R9	K17
      0x7C180600,  //  0035  CALL	R6	3
      0x781A0001,  //  0036  JMPF	R6	#0039
      0x88180311,  //  0037  GETMBR	R6	R1	K17
      0x90162406,  //  0038  SETMBR	R5	K18	R6
      0x8C18070C,  //  0039  GETMET	R6	R3	K12
      0x5C200200,  //  003A  MOVE	R8	R1
      0x58240013,  //  003B  LDCONST	R9	K19
      0x7C180600,  //  003C  CALL	R6	3
      0x781A0001,  //  003D  JMPF	R6	#0040
      0x88180313,  //  003E  GETMBR	R6	R1	K19
      0x90162806,  //  003F  SETMBR	R5	K20	R6
      0x8C18070C,  //  0040  GETMET	R6	R3	K12
      0x5C200200,  //  0041  MOVE	R8	R1
      0x58240015,  //  0042  LDCONST	R9	K21
      0x7C180600,  //  0043  CALL	R6	3
      0x781A0001,  //  0044  JMPF	R6	#0047
      0x88180315,  //  0045  GETMBR	R6	R1	K21
      0x90162C06,  //  0046  SETMBR	R5	K22	R6
      0x88180105,  //  0047  GETMBR	R6	R0	K5
      0x98180805,  //  0048  SETIDX	R6	R4	R5
      0xB81A0200,  //  0049  GETNGBL	R6	K1
      0x8C180D17,  //  004A  GETMET	R6	R6	K23
      0x5C200A00,  //  004B  MOVE	R8	R5
      0x5C240400,  //  004C  MOVE	R9	R2
      0x7C180600,  //  004D  CALL	R6	3
      0x881C0D18,  //  004E  GETMBR	R7	R6	K24
      0x90063007,  //  004F  SETMBR	R1	K24	R7
      0x8C1C0119,  //  0050  GETMET	R7	R0	K25
      0x5C240200,  //  0051  MOVE	R9	R1
      0x7C1C0400,  //  0052  CALL	R7	2
      0x8C1C031A,  //  0053  GETMET	R7	R1	K26
      0x7C1C0200,  //  0054  CALL	R7	1
      0x80000000,  //  0055  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: widget_ctor_impl
********************************************************************/
be_local_closure(widget_ctor_impl,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    3,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_string("introspect", 164638290, 10),
    /* K1   */  be_nested_string("lv", 1529997255, 2),
    /* K2   */  be_nested_string("lv_obj_class", -255311002, 12),
    /* K3   */  be_nested_string("get_object_from_ptr", -1949948095, 19),
    /* K4   */  be_nested_string("cb_obj", 1195696482, 6),
    /* K5   */  be_nested_string("find", -1108310694, 4),
    /* K6   */  be_nested_string("instance", 193386898, 8),
    /* K7   */  be_nested_string("get", 1410115415, 3),
    /* K8   */  be_nested_string("widget_constructor", -1751181362, 18),
    }),
    (be_nested_const_str("widget_ctor_impl", 194252479, 16)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[29]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xB8120200,  //  0001  GETNGBL	R4	K1
      0x8C100902,  //  0002  GETMET	R4	R4	K2
      0x5C180200,  //  0003  MOVE	R6	R1
      0x7C100400,  //  0004  CALL	R4	2
      0x8C140103,  //  0005  GETMET	R5	R0	K3
      0x5C1C0400,  //  0006  MOVE	R7	R2
      0x7C140400,  //  0007  CALL	R5	2
      0x88180104,  //  0008  GETMBR	R6	R0	K4
      0x8C180D05,  //  0009  GETMET	R6	R6	K5
      0x5C200A00,  //  000A  MOVE	R8	R5
      0x7C180400,  //  000B  CALL	R6	2
      0x781A0001,  //  000C  JMPF	R6	#000F
      0x88180104,  //  000D  GETMBR	R6	R0	K4
      0x94140C05,  //  000E  GETIDX	R5	R6	R5
      0x60180004,  //  000F  GETGBL	R6	G4
      0x5C1C0A00,  //  0010  MOVE	R7	R5
      0x7C180200,  //  0011  CALL	R6	1
      0x1C180D06,  //  0012  EQ	R6	R6	K6
      0x781A0007,  //  0013  JMPF	R6	#001C
      0x8C180707,  //  0014  GETMET	R6	R3	K7
      0x5C200A00,  //  0015  MOVE	R8	R5
      0x58240008,  //  0016  LDCONST	R9	K8
      0x7C180600,  //  0017  CALL	R6	3
      0x781A0002,  //  0018  JMPF	R6	#001C
      0x8C180B08,  //  0019  GETMET	R6	R5	K8
      0x5C200800,  //  001A  MOVE	R8	R4
      0x7C180400,  //  001B  CALL	R6	2
      0x80000000,  //  001C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: LVGL_glob
********************************************************************/
be_local_class(LVGL_glob,
    9,
    NULL,
    be_nested_map(20,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_nested_key("widget_ctor_cb", 876007560, 14, 9), be_const_var(4) },
        { be_nested_key("get_object_from_ptr", -1949948095, 19, 4), be_const_closure(get_object_from_ptr_closure) },
        { be_nested_key("cb_obj", 1195696482, 6, 7), be_const_var(0) },
        { be_nested_key("widget_struct_by_class", -488593454, 22, -1), be_const_var(8) },
        { be_nested_key("widget_event_impl", -2116536735, 17, -1), be_const_closure(widget_event_impl_closure) },
        { be_nested_key("widget_dtor_cb", -1143421451, 14, 6), be_const_var(5) },
        { be_nested_key("cb_event_closure", -466699971, 16, -1), be_const_var(1) },
        { be_nested_key("lvgl_event_dispatch", 2104396622, 19, 16), be_const_closure(lvgl_event_dispatch_closure) },
        { be_nested_key("widget_dtor_impl", 520430610, 16, -1), be_const_closure(widget_dtor_impl_closure) },
        { be_nested_key("null_cb", -1961430836, 7, -1), be_const_var(3) },
        { be_nested_key("register_obj", -312352526, 12, 8), be_const_closure(register_obj_closure) },
        { be_nested_key("gen_cb", -1049739745, 6, -1), be_const_closure(gen_cb_closure) },
        { be_nested_key("widget_struct_default", 781673633, 21, -1), be_const_var(7) },
        { be_nested_key("deregister_obj", -385000303, 14, 12), be_const_closure(deregister_obj_closure) },
        { be_nested_key("widget_event_cb", 1508466754, 15, -1), be_const_var(6) },
        { be_nested_key("widget_cb", -1531384241, 9, -1), be_const_closure(widget_cb_closure) },
        { be_nested_key("cb_do_nothing", 1488730702, 13, 3), be_const_closure(_anonymous__closure) },
        { be_nested_key("event_cb", -1166269279, 8, -1), be_const_var(2) },
        { be_nested_key("create_custom_widget", 1140594778, 20, -1), be_const_closure(create_custom_widget_closure) },
        { be_nested_key("widget_ctor_impl", 194252479, 16, -1), be_const_closure(widget_ctor_impl_closure) },
    })),
    (be_nested_const_str("LVGL_glob", 315437079, 9))
);
/*******************************************************************/

void be_load_LVGL_glob_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_LVGL_glob);
    be_setglobal(vm, "LVGL_glob");
    be_pop(vm, 1);
}

#endif // USE_LVGL