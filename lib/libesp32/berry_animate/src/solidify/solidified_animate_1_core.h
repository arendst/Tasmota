/* Solidification of animate_1_core.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
// compact class 'Animate_core' ktab size: 48, total: 98 (saved 400 bytes)
static const bvalue be_ktab_class_Animate_core[48] = {
  /* K0   */  be_nested_str_weak(stop),
  /* K1   */  be_nested_str_weak(strip),
  /* K2   */  be_nested_str_weak(clear),
  /* K3   */  be_nested_str_weak(set_bri),
  /* K4   */  be_nested_str_weak(tasmota),
  /* K5   */  be_nested_str_weak(scale_uint),
  /* K6   */  be_nested_str_weak(bri),
  /* K7   */  be_const_int(0),
  /* K8   */  be_nested_str_weak(painters),
  /* K9   */  be_nested_str_weak(remove),
  /* K10  */  be_nested_str_weak(find),
  /* K11  */  be_nested_str_weak(running),
  /* K12  */  be_nested_str_weak(animators),
  /* K13  */  be_const_int(1),
  /* K14  */  be_nested_str_weak(remove_fast_loop),
  /* K15  */  be_nested_str_weak(fast_loop_cb),
  /* K16  */  be_nested_str_weak(set_strip_bri),
  /* K17  */  be_nested_str_weak(push),
  /* K18  */  be_nested_str_weak(time_reached),
  /* K19  */  be_nested_str_weak(fast_loop_next),
  /* K20  */  be_nested_str_weak(can_show),
  /* K21  */  be_nested_str_weak(frame),
  /* K22  */  be_nested_str_weak(fill_pixels),
  /* K23  */  be_nested_str_weak(back_color),
  /* K24  */  be_nested_str_weak(millis),
  /* K25  */  be_nested_str_weak(FAST_LOOP_MIN),
  /* K26  */  be_nested_str_weak(animate),
  /* K27  */  be_nested_str_weak(layer),
  /* K28  */  be_const_int(-16777216),
  /* K29  */  be_nested_str_weak(paint),
  /* K30  */  be_nested_str_weak(blend_pixels),
  /* K31  */  be_nested_str_weak(obj),
  /* K32  */  be_nested_str_weak(mth),
  /* K33  */  be_nested_str_weak(paste_pixels),
  /* K34  */  be_nested_str_weak(pixels_buffer),
  /* K35  */  be_nested_str_weak(get_bri),
  /* K36  */  be_nested_str_weak(get_gamma),
  /* K37  */  be_nested_str_weak(dirty),
  /* K38  */  be_nested_str_weak(show),
  /* K39  */  be_nested_str_weak(global),
  /* K40  */  be_nested_str_weak(_cur_anim),
  /* K41  */  be_nested_str_weak(pixel_count),
  /* K42  */  be_nested_str_weak(set_current),
  /* K43  */  be_nested_str_weak(set_cb),
  /* K44  */  be_nested_str_weak(set_back_color),
  /* K45  */  be_nested_str_weak(add_animator),
  /* K46  */  be_nested_str_weak(start),
  /* K47  */  be_nested_str_weak(add_fast_loop),
};


extern const bclass be_class_Animate_core;

/********************************************************************
** Solidified function: clear
********************************************************************/
be_local_closure(class_Animate_core_clear,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Animate_core,     /* shared constants */
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
be_local_closure(class_Animate_core_set_strip_bri,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Animate_core,     /* shared constants */
    be_str_weak(set_strip_bri),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x88040101,  //  0000  GETMBR	R1	R0	K1
      0x8C040303,  //  0001  GETMET	R1	R1	K3
      0xB80E0800,  //  0002  GETNGBL	R3	K4
      0x8C0C0705,  //  0003  GETMET	R3	R3	K5
      0x88140106,  //  0004  GETMBR	R5	R0	K6
      0x58180007,  //  0005  LDCONST	R6	K7
      0x541E0063,  //  0006  LDINT	R7	100
      0x58200007,  //  0007  LDCONST	R8	K7
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
be_local_closure(class_Animate_core_remove_painter,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Animate_core,     /* shared constants */
    be_str_weak(remove_painter),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x88080108,  //  0000  GETMBR	R2	R0	K8
      0x4C0C0000,  //  0001  LDNIL	R3
      0x200C0203,  //  0002  NE	R3	R1	R3
      0x780E0005,  //  0003  JMPF	R3	#000A
      0x8C0C0509,  //  0004  GETMET	R3	R2	K9
      0x8C14050A,  //  0005  GETMET	R5	R2	K10
      0x5C1C0200,  //  0006  MOVE	R7	R1
      0x7C140400,  //  0007  CALL	R5	2
      0x7C0C0400,  //  0008  CALL	R3	2
      0x70020001,  //  0009  JMP		#000C
      0x8C0C0502,  //  000A  GETMET	R3	R2	K2
      0x7C0C0200,  //  000B  CALL	R3	1
      0x80000000,  //  000C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: stop
********************************************************************/
be_local_closure(class_Animate_core_stop,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Animate_core,     /* shared constants */
    be_str_weak(stop),
    &be_const_str_solidified,
    ( &(const binstruction[19]) {  /* code */
      0x50040000,  //  0000  LDBOOL	R1	0	0
      0x90021601,  //  0001  SETMBR	R0	K11	R1
      0x8804010C,  //  0002  GETMBR	R1	R0	K12
      0x58080007,  //  0003  LDCONST	R2	K7
      0x600C000C,  //  0004  GETGBL	R3	G12
      0x5C100200,  //  0005  MOVE	R4	R1
      0x7C0C0200,  //  0006  CALL	R3	1
      0x140C0403,  //  0007  LT	R3	R2	R3
      0x780E0004,  //  0008  JMPF	R3	#000E
      0x940C0202,  //  0009  GETIDX	R3	R1	R2
      0x8C0C0700,  //  000A  GETMET	R3	R3	K0
      0x7C0C0200,  //  000B  CALL	R3	1
      0x0008050D,  //  000C  ADD	R2	R2	K13
      0x7001FFF5,  //  000D  JMP		#0004
      0xB80E0800,  //  000E  GETNGBL	R3	K4
      0x8C0C070E,  //  000F  GETMET	R3	R3	K14
      0x8814010F,  //  0010  GETMBR	R5	R0	K15
      0x7C0C0400,  //  0011  CALL	R3	2
      0x80000000,  //  0012  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_bri
********************************************************************/
be_local_closure(class_Animate_core_get_bri,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Animate_core,     /* shared constants */
    be_str_weak(get_bri),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88080106,  //  0000  GETMBR	R2	R0	K6
      0x80040400,  //  0001  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_bri
********************************************************************/
be_local_closure(class_Animate_core_set_bri,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Animate_core,     /* shared constants */
    be_str_weak(set_bri),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x90020C01,  //  0000  SETMBR	R0	K6	R1
      0x8C080110,  //  0001  GETMET	R2	R0	K16
      0x7C080200,  //  0002  CALL	R2	1
      0x80000000,  //  0003  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: add_painter
********************************************************************/
be_local_closure(class_Animate_core_add_painter,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Animate_core,     /* shared constants */
    be_str_weak(add_painter),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x88080108,  //  0000  GETMBR	R2	R0	K8
      0x8C08050A,  //  0001  GETMET	R2	R2	K10
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x4C0C0000,  //  0004  LDNIL	R3
      0x1C080403,  //  0005  EQ	R2	R2	R3
      0x780A0003,  //  0006  JMPF	R2	#000B
      0x88080108,  //  0007  GETMBR	R2	R0	K8
      0x8C080511,  //  0008  GETMET	R2	R2	K17
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
be_local_closure(class_Animate_core_fast_loop,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Animate_core,     /* shared constants */
    be_str_weak(fast_loop),
    &be_const_str_solidified,
    ( &(const binstruction[84]) {  /* code */
      0x8804010B,  //  0000  GETMBR	R1	R0	K11
      0x78060050,  //  0001  JMPF	R1	#0053
      0xB8060800,  //  0002  GETNGBL	R1	K4
      0x8C040312,  //  0003  GETMET	R1	R1	K18
      0x880C0113,  //  0004  GETMBR	R3	R0	K19
      0x7C040400,  //  0005  CALL	R1	2
      0x7806004B,  //  0006  JMPF	R1	#0053
      0x88040101,  //  0007  GETMBR	R1	R0	K1
      0x8C040314,  //  0008  GETMET	R1	R1	K20
      0x7C040200,  //  0009  CALL	R1	1
      0x78060047,  //  000A  JMPF	R1	#0053
      0x88040115,  //  000B  GETMBR	R1	R0	K21
      0x8C040316,  //  000C  GETMET	R1	R1	K22
      0x880C0117,  //  000D  GETMBR	R3	R0	K23
      0x7C040400,  //  000E  CALL	R1	2
      0x58040007,  //  000F  LDCONST	R1	K7
      0xB80A0800,  //  0010  GETNGBL	R2	K4
      0x8C080518,  //  0011  GETMET	R2	R2	K24
      0x7C080200,  //  0012  CALL	R2	1
      0x880C0119,  //  0013  GETMBR	R3	R0	K25
      0x000C0403,  //  0014  ADD	R3	R2	R3
      0x90022603,  //  0015  SETMBR	R0	K19	R3
      0x600C000C,  //  0016  GETGBL	R3	G12
      0x8810010C,  //  0017  GETMBR	R4	R0	K12
      0x7C0C0200,  //  0018  CALL	R3	1
      0x140C0203,  //  0019  LT	R3	R1	R3
      0x780E0006,  //  001A  JMPF	R3	#0022
      0x880C010C,  //  001B  GETMBR	R3	R0	K12
      0x940C0601,  //  001C  GETIDX	R3	R3	R1
      0x8C0C071A,  //  001D  GETMET	R3	R3	K26
      0x5C140400,  //  001E  MOVE	R5	R2
      0x7C0C0400,  //  001F  CALL	R3	2
      0x0004030D,  //  0020  ADD	R1	R1	K13
      0x7001FFF3,  //  0021  JMP		#0016
      0x58040007,  //  0022  LDCONST	R1	K7
      0x880C0115,  //  0023  GETMBR	R3	R0	K21
      0x8810011B,  //  0024  GETMBR	R4	R0	K27
      0x6014000C,  //  0025  GETGBL	R5	G12
      0x88180108,  //  0026  GETMBR	R6	R0	K8
      0x7C140200,  //  0027  CALL	R5	1
      0x14140205,  //  0028  LT	R5	R1	R5
      0x7816000D,  //  0029  JMPF	R5	#0038
      0x8C140916,  //  002A  GETMET	R5	R4	K22
      0x581C001C,  //  002B  LDCONST	R7	K28
      0x7C140400,  //  002C  CALL	R5	2
      0x88140108,  //  002D  GETMBR	R5	R0	K8
      0x94140A01,  //  002E  GETIDX	R5	R5	R1
      0x8C140B1D,  //  002F  GETMET	R5	R5	K29
      0x5C1C0800,  //  0030  MOVE	R7	R4
      0x7C140400,  //  0031  CALL	R5	2
      0x78160002,  //  0032  JMPF	R5	#0036
      0x8C14071E,  //  0033  GETMET	R5	R3	K30
      0x5C1C0800,  //  0034  MOVE	R7	R4
      0x7C140400,  //  0035  CALL	R5	2
      0x0004030D,  //  0036  ADD	R1	R1	K13
      0x7001FFEC,  //  0037  JMP		#0025
      0x8814011F,  //  0038  GETMBR	R5	R0	K31
      0x88180120,  //  0039  GETMBR	R6	R0	K32
      0x78160003,  //  003A  JMPF	R5	#003F
      0x781A0002,  //  003B  JMPF	R6	#003F
      0x5C1C0C00,  //  003C  MOVE	R7	R6
      0x5C200A00,  //  003D  MOVE	R8	R5
      0x7C1C0200,  //  003E  CALL	R7	1
      0x8C1C011A,  //  003F  GETMET	R7	R0	K26
      0x7C1C0200,  //  0040  CALL	R7	1
      0x881C0115,  //  0041  GETMBR	R7	R0	K21
      0x8C1C0F21,  //  0042  GETMET	R7	R7	K33
      0x88240101,  //  0043  GETMBR	R9	R0	K1
      0x8C241322,  //  0044  GETMET	R9	R9	K34
      0x7C240200,  //  0045  CALL	R9	1
      0x88280101,  //  0046  GETMBR	R10	R0	K1
      0x8C281523,  //  0047  GETMET	R10	R10	K35
      0x7C280200,  //  0048  CALL	R10	1
      0x882C0101,  //  0049  GETMBR	R11	R0	K1
      0x8C2C1724,  //  004A  GETMET	R11	R11	K36
      0x7C2C0200,  //  004B  CALL	R11	1
      0x7C1C0800,  //  004C  CALL	R7	4
      0x881C0101,  //  004D  GETMBR	R7	R0	K1
      0x8C1C0F25,  //  004E  GETMET	R7	R7	K37
      0x7C1C0200,  //  004F  CALL	R7	1
      0x881C0101,  //  0050  GETMBR	R7	R0	K1
      0x8C1C0F26,  //  0051  GETMET	R7	R7	K38
      0x7C1C0200,  //  0052  CALL	R7	1
      0x80000000,  //  0053  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: remove_animator
********************************************************************/
be_local_closure(class_Animate_core_remove_animator,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Animate_core,     /* shared constants */
    be_str_weak(remove_animator),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x8808010C,  //  0000  GETMBR	R2	R0	K12
      0x4C0C0000,  //  0001  LDNIL	R3
      0x200C0203,  //  0002  NE	R3	R1	R3
      0x780E0005,  //  0003  JMPF	R3	#000A
      0x8C0C0509,  //  0004  GETMET	R3	R2	K9
      0x8C14050A,  //  0005  GETMET	R5	R2	K10
      0x5C1C0200,  //  0006  MOVE	R7	R1
      0x7C140400,  //  0007  CALL	R5	2
      0x7C0C0400,  //  0008  CALL	R3	2
      0x70020001,  //  0009  JMP		#000C
      0x8C0C0502,  //  000A  GETMET	R3	R2	K2
      0x7C0C0200,  //  000B  CALL	R3	1
      0x80000000,  //  000C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: animate
********************************************************************/
be_local_closure(class_Animate_core_animate,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Animate_core,     /* shared constants */
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
be_local_closure(class_Animate_core_set_current,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Animate_core,     /* shared constants */
    be_str_weak(set_current),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0xB8064E00,  //  0000  GETNGBL	R1	K39
      0x90065000,  //  0001  SETMBR	R1	K40	R0
      0x80000000,  //  0002  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_Animate_core_init,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
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
    &be_ktab_class_Animate_core,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[37]) {  /* code */
      0xA40E3400,  //  0000  IMPORT	R3	K26
      0x90020201,  //  0001  SETMBR	R0	K1	R1
      0x4C100000,  //  0002  LDNIL	R4
      0x1C100404,  //  0003  EQ	R4	R2	R4
      0x78120000,  //  0004  JMPF	R4	#0006
      0x540A0031,  //  0005  LDINT	R2	50
      0x90020C02,  //  0006  SETMBR	R0	K6	R2
      0x8C100110,  //  0007  GETMET	R4	R0	K16
      0x7C100200,  //  0008  CALL	R4	1
      0x50100000,  //  0009  LDBOOL	R4	0	0
      0x90021604,  //  000A  SETMBR	R0	K11	R4
      0x8C100329,  //  000B  GETMET	R4	R1	K41
      0x7C100200,  //  000C  CALL	R4	1
      0x90025204,  //  000D  SETMBR	R0	K41	R4
      0x60100012,  //  000E  GETGBL	R4	G18
      0x7C100000,  //  000F  CALL	R4	0
      0x90021804,  //  0010  SETMBR	R0	K12	R4
      0x60100012,  //  0011  GETGBL	R4	G18
      0x7C100000,  //  0012  CALL	R4	0
      0x90021004,  //  0013  SETMBR	R0	K8	R4
      0x8C100102,  //  0014  GETMET	R4	R0	K2
      0x7C100200,  //  0015  CALL	R4	1
      0x8C100715,  //  0016  GETMET	R4	R3	K21
      0x88180129,  //  0017  GETMBR	R6	R0	K41
      0x7C100400,  //  0018  CALL	R4	2
      0x90022A04,  //  0019  SETMBR	R0	K21	R4
      0x8C100715,  //  001A  GETMET	R4	R3	K21
      0x88180129,  //  001B  GETMBR	R6	R0	K41
      0x7C100400,  //  001C  CALL	R4	2
      0x90023604,  //  001D  SETMBR	R0	K27	R4
      0x84100000,  //  001E  CLOSURE	R4	P0
      0x90021E04,  //  001F  SETMBR	R0	K15	R4
      0x90022F07,  //  0020  SETMBR	R0	K23	K7
      0x8C10012A,  //  0021  GETMET	R4	R0	K42
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
be_local_closure(class_Animate_core_set_cb,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Animate_core,     /* shared constants */
    be_str_weak(set_cb),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x90023E01,  //  0000  SETMBR	R0	K31	R1
      0x90024002,  //  0001  SETMBR	R0	K32	R2
      0x80000000,  //  0002  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_back_color
********************************************************************/
be_local_closure(class_Animate_core_set_back_color,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Animate_core,     /* shared constants */
    be_str_weak(set_back_color),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x90022E01,  //  0000  SETMBR	R0	K23	R1
      0x80000000,  //  0001  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: add_background_animator
********************************************************************/
be_local_closure(class_Animate_core_add_background_animator,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Animate_core,     /* shared constants */
    be_str_weak(add_background_animator),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x8C08032B,  //  0000  GETMET	R2	R1	K43
      0x5C100000,  //  0001  MOVE	R4	R0
      0x8814012C,  //  0002  GETMBR	R5	R0	K44
      0x7C080600,  //  0003  CALL	R2	3
      0x8C08012D,  //  0004  GETMET	R2	R0	K45
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
be_local_closure(class_Animate_core_add_animator,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Animate_core,     /* shared constants */
    be_str_weak(add_animator),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x8808010C,  //  0000  GETMBR	R2	R0	K12
      0x8C08050A,  //  0001  GETMET	R2	R2	K10
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x4C0C0000,  //  0004  LDNIL	R3
      0x1C080403,  //  0005  EQ	R2	R2	R3
      0x780A0003,  //  0006  JMPF	R2	#000B
      0x8808010C,  //  0007  GETMBR	R2	R0	K12
      0x8C080511,  //  0008  GETMET	R2	R2	K17
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
be_local_closure(class_Animate_core_remove,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Animate_core,     /* shared constants */
    be_str_weak(remove),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x8C040102,  //  0000  GETMET	R1	R0	K2
      0x7C040200,  //  0001  CALL	R1	1
      0xB8060800,  //  0002  GETNGBL	R1	K4
      0x8C04030E,  //  0003  GETMET	R1	R1	K14
      0x880C010F,  //  0004  GETMBR	R3	R0	K15
      0x7C040400,  //  0005  CALL	R1	2
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start
********************************************************************/
be_local_closure(class_Animate_core_start,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Animate_core,     /* shared constants */
    be_str_weak(start),
    &be_const_str_solidified,
    ( &(const binstruction[20]) {  /* code */
      0x50040200,  //  0000  LDBOOL	R1	1	0
      0x90021601,  //  0001  SETMBR	R0	K11	R1
      0x8804010C,  //  0002  GETMBR	R1	R0	K12
      0x58080007,  //  0003  LDCONST	R2	K7
      0x600C000C,  //  0004  GETGBL	R3	G12
      0x5C100200,  //  0005  MOVE	R4	R1
      0x7C0C0200,  //  0006  CALL	R3	1
      0x140C0403,  //  0007  LT	R3	R2	R3
      0x780E0004,  //  0008  JMPF	R3	#000E
      0x940C0202,  //  0009  GETIDX	R3	R1	R2
      0x8C0C072E,  //  000A  GETMET	R3	R3	K46
      0x7C0C0200,  //  000B  CALL	R3	1
      0x0008050D,  //  000C  ADD	R2	R2	K13
      0x7001FFF5,  //  000D  JMP		#0004
      0x90022707,  //  000E  SETMBR	R0	K19	K7
      0xB80E0800,  //  000F  GETNGBL	R3	K4
      0x8C0C072F,  //  0010  GETMET	R3	R3	K47
      0x8814010F,  //  0011  GETMBR	R5	R0	K15
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
        { be_const_key_weak(set_strip_bri, -1), be_const_closure(class_Animate_core_set_strip_bri_closure) },
        { be_const_key_weak(animators, -1), be_const_var(4) },
        { be_const_key_weak(clear, 0), be_const_closure(class_Animate_core_clear_closure) },
        { be_const_key_weak(remove, -1), be_const_closure(class_Animate_core_remove_closure) },
        { be_const_key_weak(mth, -1), be_const_var(9) },
        { be_const_key_weak(stop, 1), be_const_closure(class_Animate_core_stop_closure) },
        { be_const_key_weak(fast_loop_cb, 30), be_const_var(6) },
        { be_const_key_weak(get_bri, -1), be_const_closure(class_Animate_core_get_bri_closure) },
        { be_const_key_weak(add_animator, -1), be_const_closure(class_Animate_core_add_animator_closure) },
        { be_const_key_weak(add_background_animator, -1), be_const_closure(class_Animate_core_add_background_animator_closure) },
        { be_const_key_weak(fast_loop_next, -1), be_const_var(7) },
        { be_const_key_weak(remove_animator, -1), be_const_closure(class_Animate_core_remove_animator_closure) },
        { be_const_key_weak(add_painter, 28), be_const_closure(class_Animate_core_add_painter_closure) },
        { be_const_key_weak(FAST_LOOP_MIN, -1), be_const_int(20) },
        { be_const_key_weak(fast_loop, -1), be_const_closure(class_Animate_core_fast_loop_closure) },
        { be_const_key_weak(set_back_color, 11), be_const_closure(class_Animate_core_set_back_color_closure) },
        { be_const_key_weak(animate, 8), be_const_closure(class_Animate_core_animate_closure) },
        { be_const_key_weak(strip, 24), be_const_var(0) },
        { be_const_key_weak(layer, -1), be_const_var(11) },
        { be_const_key_weak(init, -1), be_const_closure(class_Animate_core_init_closure) },
        { be_const_key_weak(bri, -1), be_const_var(2) },
        { be_const_key_weak(set_cb, 13), be_const_closure(class_Animate_core_set_cb_closure) },
        { be_const_key_weak(back_color, 18), be_const_var(12) },
        { be_const_key_weak(pixel_count, 15), be_const_var(1) },
        { be_const_key_weak(set_current, -1), be_const_closure(class_Animate_core_set_current_closure) },
        { be_const_key_weak(painters, -1), be_const_var(5) },
        { be_const_key_weak(obj, 10), be_const_var(8) },
        { be_const_key_weak(set_bri, 9), be_const_closure(class_Animate_core_set_bri_closure) },
        { be_const_key_weak(running, -1), be_const_var(3) },
        { be_const_key_weak(remove_painter, 3), be_const_closure(class_Animate_core_remove_painter_closure) },
        { be_const_key_weak(frame, -1), be_const_var(10) },
        { be_const_key_weak(start, -1), be_const_closure(class_Animate_core_start_closure) },
    })),
    be_str_weak(Animate_core)
);
/********************************************************************/
/* End of solidification */
