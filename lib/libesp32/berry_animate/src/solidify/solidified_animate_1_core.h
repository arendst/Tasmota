/* Solidification of animate_1_core.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Animate_core;

/********************************************************************
** Solidified function: clear
********************************************************************/
be_local_closure(Animate_core_clear,   /* name */
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
** Solidified function: set_strip_bri
********************************************************************/
be_local_closure(Animate_core_set_strip_bri,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(strip),
    /* K1   */  be_nested_str_weak(set_bri),
    /* K2   */  be_nested_str_weak(tasmota),
    /* K3   */  be_nested_str_weak(scale_uint),
    /* K4   */  be_nested_str_weak(bri),
    /* K5   */  be_const_int(0),
    }),
    be_str_weak(set_strip_bri),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0xB80E0400,  //  0002  GETNGBL	R3	K2
      0x8C0C0703,  //  0003  GETMET	R3	R3	K3
      0x88140104,  //  0004  GETMBR	R5	R0	K4
      0x58180005,  //  0005  LDCONST	R6	K5
      0x541E0063,  //  0006  LDINT	R7	100
      0x58200005,  //  0007  LDCONST	R8	K5
      0x542600FE,  //  0008  LDINT	R9	255
      0x7C0C0C00,  //  0009  CALL	R3	6
      0x7C040400,  //  000A  CALL	R1	2
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: remove_painter
********************************************************************/
be_local_closure(Animate_core_remove_painter,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(painters),
    /* K1   */  be_nested_str_weak(remove),
    /* K2   */  be_nested_str_weak(find),
    /* K3   */  be_nested_str_weak(clear),
    }),
    be_str_weak(remove_painter),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x4C0C0000,  //  0001  LDNIL	R3
      0x200C0203,  //  0002  NE	R3	R1	R3
      0x780E0005,  //  0003  JMPF	R3	#000A
      0x8C0C0501,  //  0004  GETMET	R3	R2	K1
      0x8C140502,  //  0005  GETMET	R5	R2	K2
      0x5C1C0200,  //  0006  MOVE	R7	R1
      0x7C140400,  //  0007  CALL	R5	2
      0x7C0C0400,  //  0008  CALL	R3	2
      0x70020001,  //  0009  JMP		#000C
      0x8C0C0503,  //  000A  GETMET	R3	R2	K3
      0x7C0C0200,  //  000B  CALL	R3	1
      0x80000000,  //  000C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: stop
********************************************************************/
be_local_closure(Animate_core_stop,   /* name */
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
** Solidified function: get_bri
********************************************************************/
be_local_closure(Animate_core_get_bri,   /* name */
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
be_local_closure(Animate_core_set_bri,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(bri),
    /* K1   */  be_nested_str_weak(set_strip_bri),
    }),
    be_str_weak(set_bri),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x8C080101,  //  0001  GETMET	R2	R0	K1
      0x7C080200,  //  0002  CALL	R2	1
      0x80000000,  //  0003  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: add_painter
********************************************************************/
be_local_closure(Animate_core_add_painter,   /* name */
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
    /* K0   */  be_nested_str_weak(painters),
    /* K1   */  be_nested_str_weak(find),
    /* K2   */  be_nested_str_weak(push),
    }),
    be_str_weak(add_painter),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x4C0C0000,  //  0004  LDNIL	R3
      0x1C080403,  //  0005  EQ	R2	R2	R3
      0x780A0003,  //  0006  JMPF	R2	#000B
      0x88080100,  //  0007  GETMBR	R2	R0	K0
      0x8C080502,  //  0008  GETMET	R2	R2	K2
      0x5C100200,  //  0009  MOVE	R4	R1
      0x7C080400,  //  000A  CALL	R2	2
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: fast_loop
********************************************************************/
be_local_closure(Animate_core_fast_loop,   /* name */
  be_nested_proto(
    13,                          /* nstack */
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
    /* K20  */  be_nested_str_weak(obj),
    /* K21  */  be_nested_str_weak(mth),
    /* K22  */  be_nested_str_weak(paste_pixels),
    /* K23  */  be_nested_str_weak(pixels_buffer),
    /* K24  */  be_nested_str_weak(get_bri),
    /* K25  */  be_nested_str_weak(get_gamma),
    /* K26  */  be_nested_str_weak(dirty),
    /* K27  */  be_nested_str_weak(show),
    }),
    be_str_weak(fast_loop),
    &be_const_str_solidified,
    ( &(const binstruction[84]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x78060050,  //  0001  JMPF	R1	#0053
      0xB8060200,  //  0002  GETNGBL	R1	K1
      0x8C040302,  //  0003  GETMET	R1	R1	K2
      0x880C0103,  //  0004  GETMBR	R3	R0	K3
      0x7C040400,  //  0005  CALL	R1	2
      0x7806004B,  //  0006  JMPF	R1	#0053
      0x88040104,  //  0007  GETMBR	R1	R0	K4
      0x8C040305,  //  0008  GETMET	R1	R1	K5
      0x7C040200,  //  0009  CALL	R1	1
      0x78060047,  //  000A  JMPF	R1	#0053
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
      0x88280104,  //  0046  GETMBR	R10	R0	K4
      0x8C281518,  //  0047  GETMET	R10	R10	K24
      0x7C280200,  //  0048  CALL	R10	1
      0x882C0104,  //  0049  GETMBR	R11	R0	K4
      0x8C2C1719,  //  004A  GETMET	R11	R11	K25
      0x7C2C0200,  //  004B  CALL	R11	1
      0x7C1C0800,  //  004C  CALL	R7	4
      0x881C0104,  //  004D  GETMBR	R7	R0	K4
      0x8C1C0F1A,  //  004E  GETMET	R7	R7	K26
      0x7C1C0200,  //  004F  CALL	R7	1
      0x881C0104,  //  0050  GETMBR	R7	R0	K4
      0x8C1C0F1B,  //  0051  GETMET	R7	R7	K27
      0x7C1C0200,  //  0052  CALL	R7	1
      0x80000000,  //  0053  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: remove_animator
********************************************************************/
be_local_closure(Animate_core_remove_animator,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(animators),
    /* K1   */  be_nested_str_weak(remove),
    /* K2   */  be_nested_str_weak(find),
    /* K3   */  be_nested_str_weak(clear),
    }),
    be_str_weak(remove_animator),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x4C0C0000,  //  0001  LDNIL	R3
      0x200C0203,  //  0002  NE	R3	R1	R3
      0x780E0005,  //  0003  JMPF	R3	#000A
      0x8C0C0501,  //  0004  GETMET	R3	R2	K1
      0x8C140502,  //  0005  GETMET	R5	R2	K2
      0x5C1C0200,  //  0006  MOVE	R7	R1
      0x7C140400,  //  0007  CALL	R5	2
      0x7C0C0400,  //  0008  CALL	R3	2
      0x70020001,  //  0009  JMP		#000C
      0x8C0C0503,  //  000A  GETMET	R3	R2	K3
      0x7C0C0200,  //  000B  CALL	R3	1
      0x80000000,  //  000C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: animate
********************************************************************/
be_local_closure(Animate_core_animate,   /* name */
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
** Solidified function: set_current
********************************************************************/
be_local_closure(Animate_core_set_current,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(global),
    /* K1   */  be_nested_str_weak(_cur_anim),
    }),
    be_str_weak(set_current),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x90060200,  //  0001  SETMBR	R1	K1	R0
      0x80000000,  //  0002  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Animate_core_init,   /* name */
  be_nested_proto(
    7,                          /* nstack */
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
    ( &(const bvalue[15]) {     /* constants */
    /* K0   */  be_nested_str_weak(animate),
    /* K1   */  be_nested_str_weak(strip),
    /* K2   */  be_nested_str_weak(bri),
    /* K3   */  be_nested_str_weak(set_strip_bri),
    /* K4   */  be_nested_str_weak(running),
    /* K5   */  be_nested_str_weak(pixel_count),
    /* K6   */  be_nested_str_weak(animators),
    /* K7   */  be_nested_str_weak(painters),
    /* K8   */  be_nested_str_weak(clear),
    /* K9   */  be_nested_str_weak(frame),
    /* K10  */  be_nested_str_weak(layer),
    /* K11  */  be_nested_str_weak(fast_loop_cb),
    /* K12  */  be_nested_str_weak(back_color),
    /* K13  */  be_const_int(0),
    /* K14  */  be_nested_str_weak(set_current),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[37]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0x90020201,  //  0001  SETMBR	R0	K1	R1
      0x4C100000,  //  0002  LDNIL	R4
      0x1C100404,  //  0003  EQ	R4	R2	R4
      0x78120000,  //  0004  JMPF	R4	#0006
      0x540A0031,  //  0005  LDINT	R2	50
      0x90020402,  //  0006  SETMBR	R0	K2	R2
      0x8C100103,  //  0007  GETMET	R4	R0	K3
      0x7C100200,  //  0008  CALL	R4	1
      0x50100000,  //  0009  LDBOOL	R4	0	0
      0x90020804,  //  000A  SETMBR	R0	K4	R4
      0x8C100305,  //  000B  GETMET	R4	R1	K5
      0x7C100200,  //  000C  CALL	R4	1
      0x90020A04,  //  000D  SETMBR	R0	K5	R4
      0x60100012,  //  000E  GETGBL	R4	G18
      0x7C100000,  //  000F  CALL	R4	0
      0x90020C04,  //  0010  SETMBR	R0	K6	R4
      0x60100012,  //  0011  GETGBL	R4	G18
      0x7C100000,  //  0012  CALL	R4	0
      0x90020E04,  //  0013  SETMBR	R0	K7	R4
      0x8C100108,  //  0014  GETMET	R4	R0	K8
      0x7C100200,  //  0015  CALL	R4	1
      0x8C100709,  //  0016  GETMET	R4	R3	K9
      0x88180105,  //  0017  GETMBR	R6	R0	K5
      0x7C100400,  //  0018  CALL	R4	2
      0x90021204,  //  0019  SETMBR	R0	K9	R4
      0x8C100709,  //  001A  GETMET	R4	R3	K9
      0x88180105,  //  001B  GETMBR	R6	R0	K5
      0x7C100400,  //  001C  CALL	R4	2
      0x90021404,  //  001D  SETMBR	R0	K10	R4
      0x84100000,  //  001E  CLOSURE	R4	P0
      0x90021604,  //  001F  SETMBR	R0	K11	R4
      0x9002190D,  //  0020  SETMBR	R0	K12	K13
      0x8C10010E,  //  0021  GETMET	R4	R0	K14
      0x7C100200,  //  0022  CALL	R4	1
      0xA0000000,  //  0023  CLOSE	R0
      0x80000000,  //  0024  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_cb
********************************************************************/
be_local_closure(Animate_core_set_cb,   /* name */
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
    /* K0   */  be_nested_str_weak(obj),
    /* K1   */  be_nested_str_weak(mth),
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
** Solidified function: set_back_color
********************************************************************/
be_local_closure(Animate_core_set_back_color,   /* name */
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
** Solidified function: add_background_animator
********************************************************************/
be_local_closure(Animate_core_add_background_animator,   /* name */
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
** Solidified function: add_animator
********************************************************************/
be_local_closure(Animate_core_add_animator,   /* name */
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
    /* K0   */  be_nested_str_weak(animators),
    /* K1   */  be_nested_str_weak(find),
    /* K2   */  be_nested_str_weak(push),
    }),
    be_str_weak(add_animator),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x4C0C0000,  //  0004  LDNIL	R3
      0x1C080403,  //  0005  EQ	R2	R2	R3
      0x780A0003,  //  0006  JMPF	R2	#000B
      0x88080100,  //  0007  GETMBR	R2	R0	K0
      0x8C080502,  //  0008  GETMET	R2	R2	K2
      0x5C100200,  //  0009  MOVE	R4	R1
      0x7C080400,  //  000A  CALL	R2	2
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: remove
********************************************************************/
be_local_closure(Animate_core_remove,   /* name */
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
** Solidified function: start
********************************************************************/
be_local_closure(Animate_core_start,   /* name */
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
** Solidified class: Animate_core
********************************************************************/
be_local_class(Animate_core,
    13,
    NULL,
    be_nested_map(32,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(set_strip_bri, -1), be_const_closure(Animate_core_set_strip_bri_closure) },
        { be_const_key_weak(animators, -1), be_const_var(4) },
        { be_const_key_weak(clear, 0), be_const_closure(Animate_core_clear_closure) },
        { be_const_key_weak(remove, -1), be_const_closure(Animate_core_remove_closure) },
        { be_const_key_weak(mth, -1), be_const_var(9) },
        { be_const_key_weak(stop, 1), be_const_closure(Animate_core_stop_closure) },
        { be_const_key_weak(fast_loop_cb, 30), be_const_var(6) },
        { be_const_key_weak(get_bri, -1), be_const_closure(Animate_core_get_bri_closure) },
        { be_const_key_weak(add_animator, -1), be_const_closure(Animate_core_add_animator_closure) },
        { be_const_key_weak(add_background_animator, -1), be_const_closure(Animate_core_add_background_animator_closure) },
        { be_const_key_weak(fast_loop_next, -1), be_const_var(7) },
        { be_const_key_weak(remove_animator, -1), be_const_closure(Animate_core_remove_animator_closure) },
        { be_const_key_weak(add_painter, 28), be_const_closure(Animate_core_add_painter_closure) },
        { be_const_key_weak(FAST_LOOP_MIN, -1), be_const_int(20) },
        { be_const_key_weak(fast_loop, -1), be_const_closure(Animate_core_fast_loop_closure) },
        { be_const_key_weak(set_back_color, 11), be_const_closure(Animate_core_set_back_color_closure) },
        { be_const_key_weak(animate, 8), be_const_closure(Animate_core_animate_closure) },
        { be_const_key_weak(strip, 24), be_const_var(0) },
        { be_const_key_weak(layer, -1), be_const_var(11) },
        { be_const_key_weak(init, -1), be_const_closure(Animate_core_init_closure) },
        { be_const_key_weak(bri, -1), be_const_var(2) },
        { be_const_key_weak(set_cb, 13), be_const_closure(Animate_core_set_cb_closure) },
        { be_const_key_weak(back_color, 18), be_const_var(12) },
        { be_const_key_weak(pixel_count, 15), be_const_var(1) },
        { be_const_key_weak(set_current, -1), be_const_closure(Animate_core_set_current_closure) },
        { be_const_key_weak(painters, -1), be_const_var(5) },
        { be_const_key_weak(obj, 10), be_const_var(8) },
        { be_const_key_weak(set_bri, 9), be_const_closure(Animate_core_set_bri_closure) },
        { be_const_key_weak(running, -1), be_const_var(3) },
        { be_const_key_weak(remove_painter, 3), be_const_closure(Animate_core_remove_painter_closure) },
        { be_const_key_weak(frame, -1), be_const_var(10) },
        { be_const_key_weak(start, -1), be_const_closure(Animate_core_start_closure) },
    })),
    be_str_weak(Animate_core)
);
/*******************************************************************/

void be_load_Animate_core_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Animate_core);
    be_setglobal(vm, "Animate_core");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
