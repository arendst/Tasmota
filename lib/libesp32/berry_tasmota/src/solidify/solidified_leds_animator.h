/* Solidification of leds_animator.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Leds_animator;

/********************************************************************
** Solidified function: remove
********************************************************************/
be_local_closure(Leds_animator_remove,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(clear),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(remove_fast_loop),
    /* K3   */  be_nested_str_weak(fast_loop_cb),
    }),
    be_str_weak(remove),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0xB8060200,  //  0002  GETNGBL	R1	K1
      0x8C040302,  //  0003  GETMET	R1	R1	K2
      0x880C0103,  //  0004  GETMBR	R3	R0	K3
      0x7C040400,  //  0005  CALL	R1	2
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: add_background_animator
********************************************************************/
be_local_closure(Leds_animator_add_background_animator,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(set_cb),
    /* K1   */  be_nested_str_weak(set_back_color),
    /* K2   */  be_nested_str_weak(add_animator),
    }),
    be_str_weak(add_background_animator),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x8C080300,  //  0000  GETMET	R2	R1	K0
      0x5C100000,  //  0001  MOVE	R4	R0
      0x88140101,  //  0002  GETMBR	R5	R0	K1
      0x7C080600,  //  0003  CALL	R2	3
      0x8C080102,  //  0004  GETMET	R2	R0	K2
      0x5C100200,  //  0005  MOVE	R4	R1
      0x7C080400,  //  0006  CALL	R2	2
      0x80000000,  //  0007  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: clear
********************************************************************/
be_local_closure(Leds_animator_clear,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(stop),
    /* K1   */  be_nested_str_weak(strip),
    /* K2   */  be_nested_str_weak(clear),
    }),
    be_str_weak(clear),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x88040101,  //  0002  GETMBR	R1	R0	K1
      0x8C040302,  //  0003  GETMET	R1	R1	K2
      0x7C040200,  //  0004  CALL	R1	1
      0x80000000,  //  0005  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: add_animator
********************************************************************/
be_local_closure(Leds_animator_add_animator,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(animators),
    /* K1   */  be_nested_str_weak(push),
    }),
    be_str_weak(add_animator),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: animate
********************************************************************/
be_local_closure(Leds_animator_animate,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    0,                          /* has constants */
    NULL,                       /* no const */
    be_str_weak(animate),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_bri
********************************************************************/
be_local_closure(Leds_animator_get_bri,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(bri),
    }),
    be_str_weak(get_bri),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x80040400,  //  0001  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_bri
********************************************************************/
be_local_closure(Leds_animator_set_bri,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(bri),
    }),
    be_str_weak(set_bri),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x80000000,  //  0001  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_back_color
********************************************************************/
be_local_closure(Leds_animator_set_back_color,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(back_color),
    }),
    be_str_weak(set_back_color),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x80000000,  //  0001  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: stop
********************************************************************/
be_local_closure(Leds_animator_stop,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(running),
    /* K1   */  be_nested_str_weak(animators),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str_weak(stop),
    /* K4   */  be_const_int(1),
    /* K5   */  be_nested_str_weak(tasmota),
    /* K6   */  be_nested_str_weak(remove_fast_loop),
    /* K7   */  be_nested_str_weak(fast_loop_cb),
    }),
    be_str_weak(stop),
    &be_const_str_solidified,
    ( &(const binstruction[19]) {  /* code */
      0x50040000,  //  0000  LDBOOL	R1	0	0
      0x90020001,  //  0001  SETMBR	R0	K0	R1
      0x88040101,  //  0002  GETMBR	R1	R0	K1
      0x58080002,  //  0003  LDCONST	R2	K2
      0x600C000C,  //  0004  GETGBL	R3	G12
      0x5C100200,  //  0005  MOVE	R4	R1
      0x7C0C0200,  //  0006  CALL	R3	1
      0x140C0403,  //  0007  LT	R3	R2	R3
      0x780E0004,  //  0008  JMPF	R3	#000E
      0x940C0202,  //  0009  GETIDX	R3	R1	R2
      0x8C0C0703,  //  000A  GETMET	R3	R3	K3
      0x7C0C0200,  //  000B  CALL	R3	1
      0x00080504,  //  000C  ADD	R2	R2	K4
      0x7001FFF5,  //  000D  JMP		#0004
      0xB80E0A00,  //  000E  GETNGBL	R3	K5
      0x8C0C0706,  //  000F  GETMET	R3	R3	K6
      0x88140107,  //  0010  GETMBR	R5	R0	K7
      0x7C0C0400,  //  0011  CALL	R3	2
      0x80000000,  //  0012  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_cb
********************************************************************/
be_local_closure(Leds_animator_set_cb,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(animate_object),
    /* K1   */  be_nested_str_weak(animate_method),
    }),
    be_str_weak(set_cb),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x90020202,  //  0001  SETMBR	R0	K1	R2
      0x80000000,  //  0002  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Leds_animator_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 1]) {
      be_nested_proto(
        2,                          /* nstack */
        0,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 1]) {     /* constants */
        /* K0   */  be_nested_str_weak(fast_loop),
        }),
        be_str_weak(_anonymous_),
        &be_const_str_solidified,
        ( &(const binstruction[ 4]) {  /* code */
          0x68000000,  //  0000  GETUPV	R0	U0
          0x8C000100,  //  0001  GETMET	R0	R0	K0
          0x7C000200,  //  0002  CALL	R0	1
          0x80000000,  //  0003  RET	0
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[13]) {     /* constants */
    /* K0   */  be_nested_str_weak(strip),
    /* K1   */  be_nested_str_weak(bri),
    /* K2   */  be_nested_str_weak(running),
    /* K3   */  be_nested_str_weak(pixel_count),
    /* K4   */  be_nested_str_weak(animators),
    /* K5   */  be_nested_str_weak(painters),
    /* K6   */  be_nested_str_weak(clear),
    /* K7   */  be_nested_str_weak(frame),
    /* K8   */  be_nested_str_weak(Leds_frame),
    /* K9   */  be_nested_str_weak(layer),
    /* K10  */  be_nested_str_weak(fast_loop_cb),
    /* K11  */  be_nested_str_weak(back_color),
    /* K12  */  be_const_int(0),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[32]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x4C0C0000,  //  0001  LDNIL	R3
      0x1C0C0403,  //  0002  EQ	R3	R2	R3
      0x780E0000,  //  0003  JMPF	R3	#0005
      0x540A0031,  //  0004  LDINT	R2	50
      0x90020202,  //  0005  SETMBR	R0	K1	R2
      0x500C0000,  //  0006  LDBOOL	R3	0	0
      0x90020403,  //  0007  SETMBR	R0	K2	R3
      0x8C0C0303,  //  0008  GETMET	R3	R1	K3
      0x7C0C0200,  //  0009  CALL	R3	1
      0x90020603,  //  000A  SETMBR	R0	K3	R3
      0x600C0012,  //  000B  GETGBL	R3	G18
      0x7C0C0000,  //  000C  CALL	R3	0
      0x90020803,  //  000D  SETMBR	R0	K4	R3
      0x600C0012,  //  000E  GETGBL	R3	G18
      0x7C0C0000,  //  000F  CALL	R3	0
      0x90020A03,  //  0010  SETMBR	R0	K5	R3
      0x8C0C0106,  //  0011  GETMET	R3	R0	K6
      0x7C0C0200,  //  0012  CALL	R3	1
      0xB80E1000,  //  0013  GETNGBL	R3	K8
      0x88100103,  //  0014  GETMBR	R4	R0	K3
      0x7C0C0200,  //  0015  CALL	R3	1
      0x90020E03,  //  0016  SETMBR	R0	K7	R3
      0xB80E1000,  //  0017  GETNGBL	R3	K8
      0x88100103,  //  0018  GETMBR	R4	R0	K3
      0x7C0C0200,  //  0019  CALL	R3	1
      0x90021203,  //  001A  SETMBR	R0	K9	R3
      0x840C0000,  //  001B  CLOSURE	R3	P0
      0x90021403,  //  001C  SETMBR	R0	K10	R3
      0x9002170C,  //  001D  SETMBR	R0	K11	K12
      0xA0000000,  //  001E  CLOSE	R0
      0x80000000,  //  001F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: add_painter
********************************************************************/
be_local_closure(Leds_animator_add_painter,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(painters),
    /* K1   */  be_nested_str_weak(push),
    }),
    be_str_weak(add_painter),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: fast_loop
********************************************************************/
be_local_closure(Leds_animator_fast_loop,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[28]) {     /* constants */
    /* K0   */  be_nested_str_weak(running),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(time_reached),
    /* K3   */  be_nested_str_weak(fast_loop_next),
    /* K4   */  be_nested_str_weak(strip),
    /* K5   */  be_nested_str_weak(can_show),
    /* K6   */  be_nested_str_weak(frame),
    /* K7   */  be_nested_str_weak(fill_pixels),
    /* K8   */  be_nested_str_weak(back_color),
    /* K9   */  be_const_int(0),
    /* K10  */  be_nested_str_weak(millis),
    /* K11  */  be_nested_str_weak(FAST_LOOP_MIN),
    /* K12  */  be_nested_str_weak(animators),
    /* K13  */  be_nested_str_weak(animate),
    /* K14  */  be_const_int(1),
    /* K15  */  be_nested_str_weak(layer),
    /* K16  */  be_nested_str_weak(painters),
    /* K17  */  be_const_int(-16777216),
    /* K18  */  be_nested_str_weak(paint),
    /* K19  */  be_nested_str_weak(blend_pixels),
    /* K20  */  be_nested_str_weak(animate_object),
    /* K21  */  be_nested_str_weak(animate_method),
    /* K22  */  be_nested_str_weak(paste_pixels),
    /* K23  */  be_nested_str_weak(pixels_buffer),
    /* K24  */  be_nested_str_weak(bri),
    /* K25  */  be_nested_str_weak(gamma),
    /* K26  */  be_nested_str_weak(dirty),
    /* K27  */  be_nested_str_weak(show),
    }),
    be_str_weak(fast_loop),
    &be_const_str_solidified,
    ( &(const binstruction[81]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x7806004D,  //  0001  JMPF	R1	#0050
      0xB8060200,  //  0002  GETNGBL	R1	K1
      0x8C040302,  //  0003  GETMET	R1	R1	K2
      0x880C0103,  //  0004  GETMBR	R3	R0	K3
      0x7C040400,  //  0005  CALL	R1	2
      0x78060048,  //  0006  JMPF	R1	#0050
      0x88040104,  //  0007  GETMBR	R1	R0	K4
      0x8C040305,  //  0008  GETMET	R1	R1	K5
      0x7C040200,  //  0009  CALL	R1	1
      0x78060044,  //  000A  JMPF	R1	#0050
      0x88040106,  //  000B  GETMBR	R1	R0	K6
      0x8C040307,  //  000C  GETMET	R1	R1	K7
      0x880C0108,  //  000D  GETMBR	R3	R0	K8
      0x7C040400,  //  000E  CALL	R1	2
      0x58040009,  //  000F  LDCONST	R1	K9
      0xB80A0200,  //  0010  GETNGBL	R2	K1
      0x8C08050A,  //  0011  GETMET	R2	R2	K10
      0x7C080200,  //  0012  CALL	R2	1
      0x880C010B,  //  0013  GETMBR	R3	R0	K11
      0x000C0403,  //  0014  ADD	R3	R2	R3
      0x90020603,  //  0015  SETMBR	R0	K3	R3
      0x600C000C,  //  0016  GETGBL	R3	G12
      0x8810010C,  //  0017  GETMBR	R4	R0	K12
      0x7C0C0200,  //  0018  CALL	R3	1
      0x140C0203,  //  0019  LT	R3	R1	R3
      0x780E0006,  //  001A  JMPF	R3	#0022
      0x880C010C,  //  001B  GETMBR	R3	R0	K12
      0x940C0601,  //  001C  GETIDX	R3	R3	R1
      0x8C0C070D,  //  001D  GETMET	R3	R3	K13
      0x5C140400,  //  001E  MOVE	R5	R2
      0x7C0C0400,  //  001F  CALL	R3	2
      0x0004030E,  //  0020  ADD	R1	R1	K14
      0x7001FFF3,  //  0021  JMP		#0016
      0x58040009,  //  0022  LDCONST	R1	K9
      0x880C0106,  //  0023  GETMBR	R3	R0	K6
      0x8810010F,  //  0024  GETMBR	R4	R0	K15
      0x6014000C,  //  0025  GETGBL	R5	G12
      0x88180110,  //  0026  GETMBR	R6	R0	K16
      0x7C140200,  //  0027  CALL	R5	1
      0x14140205,  //  0028  LT	R5	R1	R5
      0x7816000D,  //  0029  JMPF	R5	#0038
      0x8C140907,  //  002A  GETMET	R5	R4	K7
      0x581C0011,  //  002B  LDCONST	R7	K17
      0x7C140400,  //  002C  CALL	R5	2
      0x88140110,  //  002D  GETMBR	R5	R0	K16
      0x94140A01,  //  002E  GETIDX	R5	R5	R1
      0x8C140B12,  //  002F  GETMET	R5	R5	K18
      0x5C1C0800,  //  0030  MOVE	R7	R4
      0x7C140400,  //  0031  CALL	R5	2
      0x78160002,  //  0032  JMPF	R5	#0036
      0x8C140713,  //  0033  GETMET	R5	R3	K19
      0x5C1C0800,  //  0034  MOVE	R7	R4
      0x7C140400,  //  0035  CALL	R5	2
      0x0004030E,  //  0036  ADD	R1	R1	K14
      0x7001FFEC,  //  0037  JMP		#0025
      0x88140114,  //  0038  GETMBR	R5	R0	K20
      0x88180115,  //  0039  GETMBR	R6	R0	K21
      0x78160003,  //  003A  JMPF	R5	#003F
      0x781A0002,  //  003B  JMPF	R6	#003F
      0x5C1C0C00,  //  003C  MOVE	R7	R6
      0x5C200A00,  //  003D  MOVE	R8	R5
      0x7C1C0200,  //  003E  CALL	R7	1
      0x8C1C010D,  //  003F  GETMET	R7	R0	K13
      0x7C1C0200,  //  0040  CALL	R7	1
      0x881C0106,  //  0041  GETMBR	R7	R0	K6
      0x8C1C0F16,  //  0042  GETMET	R7	R7	K22
      0x88240104,  //  0043  GETMBR	R9	R0	K4
      0x8C241317,  //  0044  GETMET	R9	R9	K23
      0x7C240200,  //  0045  CALL	R9	1
      0x88280118,  //  0046  GETMBR	R10	R0	K24
      0x882C0104,  //  0047  GETMBR	R11	R0	K4
      0x882C1719,  //  0048  GETMBR	R11	R11	K25
      0x7C1C0800,  //  0049  CALL	R7	4
      0x881C0104,  //  004A  GETMBR	R7	R0	K4
      0x8C1C0F1A,  //  004B  GETMET	R7	R7	K26
      0x7C1C0200,  //  004C  CALL	R7	1
      0x881C0104,  //  004D  GETMBR	R7	R0	K4
      0x8C1C0F1B,  //  004E  GETMET	R7	R7	K27
      0x7C1C0200,  //  004F  CALL	R7	1
      0x80000000,  //  0050  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start
********************************************************************/
be_local_closure(Leds_animator_start,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(running),
    /* K1   */  be_nested_str_weak(animators),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str_weak(start),
    /* K4   */  be_const_int(1),
    /* K5   */  be_nested_str_weak(fast_loop_next),
    /* K6   */  be_nested_str_weak(tasmota),
    /* K7   */  be_nested_str_weak(add_fast_loop),
    /* K8   */  be_nested_str_weak(fast_loop_cb),
    }),
    be_str_weak(start),
    &be_const_str_solidified,
    ( &(const binstruction[20]) {  /* code */
      0x50040200,  //  0000  LDBOOL	R1	1	0
      0x90020001,  //  0001  SETMBR	R0	K0	R1
      0x88040101,  //  0002  GETMBR	R1	R0	K1
      0x58080002,  //  0003  LDCONST	R2	K2
      0x600C000C,  //  0004  GETGBL	R3	G12
      0x5C100200,  //  0005  MOVE	R4	R1
      0x7C0C0200,  //  0006  CALL	R3	1
      0x140C0403,  //  0007  LT	R3	R2	R3
      0x780E0004,  //  0008  JMPF	R3	#000E
      0x940C0202,  //  0009  GETIDX	R3	R1	R2
      0x8C0C0703,  //  000A  GETMET	R3	R3	K3
      0x7C0C0200,  //  000B  CALL	R3	1
      0x00080504,  //  000C  ADD	R2	R2	K4
      0x7001FFF5,  //  000D  JMP		#0004
      0x90020B02,  //  000E  SETMBR	R0	K5	K2
      0xB80E0C00,  //  000F  GETNGBL	R3	K6
      0x8C0C0707,  //  0010  GETMET	R3	R3	K7
      0x88140108,  //  0011  GETMBR	R5	R0	K8
      0x7C0C0400,  //  0012  CALL	R3	2
      0x80000000,  //  0013  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Leds_animator
********************************************************************/
be_local_class(Leds_animator,
    13,
    NULL,
    be_nested_map(28,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(bri, 23), be_const_var(2) },
        { be_const_key_weak(remove, -1), be_const_closure(Leds_animator_remove_closure) },
        { be_const_key_weak(start, 21), be_const_closure(Leds_animator_start_closure) },
        { be_const_key_weak(fast_loop, -1), be_const_closure(Leds_animator_fast_loop_closure) },
        { be_const_key_weak(add_animator, -1), be_const_closure(Leds_animator_add_animator_closure) },
        { be_const_key_weak(FAST_LOOP_MIN, 8), be_const_int(20) },
        { be_const_key_weak(animate, -1), be_const_closure(Leds_animator_animate_closure) },
        { be_const_key_weak(pixel_count, -1), be_const_var(1) },
        { be_const_key_weak(animators, -1), be_const_var(4) },
        { be_const_key_weak(back_color, 11), be_const_var(12) },
        { be_const_key_weak(fast_loop_cb, 12), be_const_var(6) },
        { be_const_key_weak(frame, -1), be_const_var(10) },
        { be_const_key_weak(animate_object, -1), be_const_var(8) },
        { be_const_key_weak(stop, -1), be_const_closure(Leds_animator_stop_closure) },
        { be_const_key_weak(init, -1), be_const_closure(Leds_animator_init_closure) },
        { be_const_key_weak(set_cb, 13), be_const_closure(Leds_animator_set_cb_closure) },
        { be_const_key_weak(running, -1), be_const_var(3) },
        { be_const_key_weak(strip, 15), be_const_var(0) },
        { be_const_key_weak(fast_loop_next, -1), be_const_var(7) },
        { be_const_key_weak(set_back_color, 14), be_const_closure(Leds_animator_set_back_color_closure) },
        { be_const_key_weak(add_painter, -1), be_const_closure(Leds_animator_add_painter_closure) },
        { be_const_key_weak(set_bri, 24), be_const_closure(Leds_animator_set_bri_closure) },
        { be_const_key_weak(layer, 9), be_const_var(11) },
        { be_const_key_weak(animate_method, 6), be_const_var(9) },
        { be_const_key_weak(get_bri, -1), be_const_closure(Leds_animator_get_bri_closure) },
        { be_const_key_weak(painters, -1), be_const_var(5) },
        { be_const_key_weak(clear, 3), be_const_closure(Leds_animator_clear_closure) },
        { be_const_key_weak(add_background_animator, 2), be_const_closure(Leds_animator_add_background_animator_closure) },
    })),
    be_str_weak(Leds_animator)
);
/*******************************************************************/

void be_load_Leds_animator_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Leds_animator);
    be_setglobal(vm, "Leds_animator");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
