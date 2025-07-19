/* Solidification of lv_haspmota.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
// compact class 'Antiburn' ktab size: 29, total: 40 (saved 88 bytes)
static const bvalue be_ktab_class_Antiburn[29] = {
  /* K0   */  be_nested_str_weak(running),
  /* K1   */  be_nested_str_weak(antiburn),
  /* K2   */  be_nested_str_weak(set_style_bg_color),
  /* K3   */  be_nested_str_weak(lv),
  /* K4   */  be_nested_str_weak(color_hex),
  /* K5   */  be_nested_str_weak(colors),
  /* K6   */  be_const_int(0),
  /* K7   */  be_nested_str_weak(tasmota),
  /* K8   */  be_nested_str_weak(set_timer),
  /* K9   */  be_nested_str_weak(stop),
  /* K10  */  be_nested_str_weak(set_style_bg_opa),
  /* K11  */  be_nested_str_weak(clear_flag),
  /* K12  */  be_nested_str_weak(OBJ_FLAG_CLICKABLE),
  /* K13  */  be_nested_str_weak(del),
  /* K14  */  be_nested_str_weak(start),
  /* K15  */  be_nested_str_weak(obj),
  /* K16  */  be_nested_str_weak(layer_sys),
  /* K17  */  be_nested_str_weak(set_style_radius),
  /* K18  */  be_nested_str_weak(set_style_border_width),
  /* K19  */  be_nested_str_weak(set_pos),
  /* K20  */  be_nested_str_weak(set_width),
  /* K21  */  be_nested_str_weak(get_hor_res),
  /* K22  */  be_nested_str_weak(set_height),
  /* K23  */  be_nested_str_weak(get_ver_res),
  /* K24  */  be_nested_str_weak(add_event_cb),
  /* K25  */  be_nested_str_weak(EVENT_PRESSED),
  /* K26  */  be_nested_str_weak(add_flag),
  /* K27  */  be_nested_str_weak(move_foreground),
  /* K28  */  be_nested_str_weak(cycle),
};


extern const bclass be_class_Antiburn;

/********************************************************************
** Solidified function: cycle
********************************************************************/
be_local_closure(class_Antiburn_cycle,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 1]) {
      be_nested_proto(
        3,                          /* nstack */
        0,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 2]) {  /* upvals */
          be_local_const_upval(1, 0),
          be_local_const_upval(1, 1),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 2]) {     /* constants */
        /* K0   */  be_nested_str_weak(cycle),
        /* K1   */  be_const_int(1),
        }),
        be_str_weak(_X3Clambda_X3E),
        &be_const_str_solidified,
        ( &(const binstruction[ 6]) {  /* code */
          0x68000000,  //  0000  GETUPV	R0	U0
          0x8C000100,  //  0001  GETMET	R0	R0	K0
          0x68080001,  //  0002  GETUPV	R2	U1
          0x00080501,  //  0003  ADD	R2	R2	K1
          0x7C000400,  //  0004  CALL	R0	2
          0x80040000,  //  0005  RET	1	R0
        })
      ),
    }),
    1,                          /* has constants */
    &be_ktab_class_Antiburn,     /* shared constants */
    be_str_weak(cycle),
    &be_const_str_solidified,
    ( &(const binstruction[32]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x780A0003,  //  0001  JMPF	R2	#0006
      0x88080101,  //  0002  GETMBR	R2	R0	K1
      0x4C0C0000,  //  0003  LDNIL	R3
      0x1C080403,  //  0004  EQ	R2	R2	R3
      0x780A0001,  //  0005  JMPF	R2	#0008
      0x4C080000,  //  0006  LDNIL	R2
      0x80040400,  //  0007  RET	1	R2
      0x540A001D,  //  0008  LDINT	R2	30
      0x14080202,  //  0009  LT	R2	R1	R2
      0x780A0010,  //  000A  JMPF	R2	#001C
      0x88080101,  //  000B  GETMBR	R2	R0	K1
      0x8C080502,  //  000C  GETMET	R2	R2	K2
      0xB8120600,  //  000D  GETNGBL	R4	K3
      0x8C100904,  //  000E  GETMET	R4	R4	K4
      0x541A0004,  //  000F  LDINT	R6	5
      0x10180206,  //  0010  MOD	R6	R1	R6
      0x881C0105,  //  0011  GETMBR	R7	R0	K5
      0x94180E06,  //  0012  GETIDX	R6	R7	R6
      0x7C100400,  //  0013  CALL	R4	2
      0x58140006,  //  0014  LDCONST	R5	K6
      0x7C080600,  //  0015  CALL	R2	3
      0xB80A0E00,  //  0016  GETNGBL	R2	K7
      0x8C080508,  //  0017  GETMET	R2	R2	K8
      0x541203E7,  //  0018  LDINT	R4	1000
      0x84140000,  //  0019  CLOSURE	R5	P0
      0x7C080600,  //  001A  CALL	R2	3
      0x70020001,  //  001B  JMP		#001E
      0x8C080109,  //  001C  GETMET	R2	R0	K9
      0x7C080200,  //  001D  CALL	R2	1
      0xA0000000,  //  001E  CLOSE	R0
      0x80000000,  //  001F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: stop
********************************************************************/
be_local_closure(class_Antiburn_stop,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Antiburn,     /* shared constants */
    be_str_weak(stop),
    &be_const_str_solidified,
    ( &(const binstruction[24]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x78060014,  //  0001  JMPF	R1	#0017
      0x88040101,  //  0002  GETMBR	R1	R0	K1
      0x4C080000,  //  0003  LDNIL	R2
      0x20040202,  //  0004  NE	R1	R1	R2
      0x78060010,  //  0005  JMPF	R1	#0017
      0x88040101,  //  0006  GETMBR	R1	R0	K1
      0x8C04030A,  //  0007  GETMET	R1	R1	K10
      0x580C0006,  //  0008  LDCONST	R3	K6
      0x58100006,  //  0009  LDCONST	R4	K6
      0x7C040600,  //  000A  CALL	R1	3
      0x88040101,  //  000B  GETMBR	R1	R0	K1
      0x8C04030B,  //  000C  GETMET	R1	R1	K11
      0xB80E0600,  //  000D  GETNGBL	R3	K3
      0x880C070C,  //  000E  GETMBR	R3	R3	K12
      0x7C040400,  //  000F  CALL	R1	2
      0x50040000,  //  0010  LDBOOL	R1	0	0
      0x90020001,  //  0011  SETMBR	R0	K0	R1
      0x88040101,  //  0012  GETMBR	R1	R0	K1
      0x8C04030D,  //  0013  GETMET	R1	R1	K13
      0x7C040200,  //  0014  CALL	R1	1
      0x4C040000,  //  0015  LDNIL	R1
      0x90020201,  //  0016  SETMBR	R0	K1	R1
      0x80000000,  //  0017  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_Antiburn_init,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Antiburn,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x50040000,  //  0000  LDBOOL	R1	0	0
      0x90020001,  //  0001  SETMBR	R0	K0	R1
      0x80000000,  //  0002  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start
********************************************************************/
be_local_closure(class_Antiburn_start,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
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
        /* K0   */  be_nested_str_weak(stop),
        }),
        be_str_weak(_X3Clambda_X3E),
        &be_const_str_solidified,
        ( &(const binstruction[ 4]) {  /* code */
          0x68000000,  //  0000  GETUPV	R0	U0
          0x8C000100,  //  0001  GETMET	R0	R0	K0
          0x7C000200,  //  0002  CALL	R0	1
          0x80040000,  //  0003  RET	1	R0
        })
      ),
    }),
    1,                          /* has constants */
    &be_ktab_class_Antiburn,     /* shared constants */
    be_str_weak(start),
    &be_const_str_solidified,
    ( &(const binstruction[70]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x78060001,  //  0001  JMPF	R1	#0004
      0x80000200,  //  0002  RET	0
      0x7002003F,  //  0003  JMP		#0044
      0xB8060600,  //  0004  GETNGBL	R1	K3
      0x8C04030E,  //  0005  GETMET	R1	R1	K14
      0x7C040200,  //  0006  CALL	R1	1
      0x88040101,  //  0007  GETMBR	R1	R0	K1
      0x4C080000,  //  0008  LDNIL	R2
      0x1C040202,  //  0009  EQ	R1	R1	R2
      0x78060026,  //  000A  JMPF	R1	#0032
      0xB8060600,  //  000B  GETNGBL	R1	K3
      0x8C04030F,  //  000C  GETMET	R1	R1	K15
      0xB80E0600,  //  000D  GETNGBL	R3	K3
      0x8C0C0710,  //  000E  GETMET	R3	R3	K16
      0x7C0C0200,  //  000F  CALL	R3	1
      0x7C040400,  //  0010  CALL	R1	2
      0x8C080311,  //  0011  GETMET	R2	R1	K17
      0x58100006,  //  0012  LDCONST	R4	K6
      0x58140006,  //  0013  LDCONST	R5	K6
      0x7C080600,  //  0014  CALL	R2	3
      0x8C080312,  //  0015  GETMET	R2	R1	K18
      0x58100006,  //  0016  LDCONST	R4	K6
      0x58140006,  //  0017  LDCONST	R5	K6
      0x7C080600,  //  0018  CALL	R2	3
      0x8C08030A,  //  0019  GETMET	R2	R1	K10
      0x541200FE,  //  001A  LDINT	R4	255
      0x58140006,  //  001B  LDCONST	R5	K6
      0x7C080600,  //  001C  CALL	R2	3
      0x8C080313,  //  001D  GETMET	R2	R1	K19
      0x58100006,  //  001E  LDCONST	R4	K6
      0x58140006,  //  001F  LDCONST	R5	K6
      0x7C080600,  //  0020  CALL	R2	3
      0x8C080314,  //  0021  GETMET	R2	R1	K20
      0xB8120600,  //  0022  GETNGBL	R4	K3
      0x8C100915,  //  0023  GETMET	R4	R4	K21
      0x7C100200,  //  0024  CALL	R4	1
      0x7C080400,  //  0025  CALL	R2	2
      0x8C080316,  //  0026  GETMET	R2	R1	K22
      0xB8120600,  //  0027  GETNGBL	R4	K3
      0x8C100917,  //  0028  GETMET	R4	R4	K23
      0x7C100200,  //  0029  CALL	R4	1
      0x7C080400,  //  002A  CALL	R2	2
      0x8C080318,  //  002B  GETMET	R2	R1	K24
      0x84100000,  //  002C  CLOSURE	R4	P0
      0xB8160600,  //  002D  GETNGBL	R5	K3
      0x88140B19,  //  002E  GETMBR	R5	R5	K25
      0x58180006,  //  002F  LDCONST	R6	K6
      0x7C080800,  //  0030  CALL	R2	4
      0x90020201,  //  0031  SETMBR	R0	K1	R1
      0x88040101,  //  0032  GETMBR	R1	R0	K1
      0x8C04030A,  //  0033  GETMET	R1	R1	K10
      0x540E00FE,  //  0034  LDINT	R3	255
      0x58100006,  //  0035  LDCONST	R4	K6
      0x7C040600,  //  0036  CALL	R1	3
      0x88040101,  //  0037  GETMBR	R1	R0	K1
      0x8C04031A,  //  0038  GETMET	R1	R1	K26
      0xB80E0600,  //  0039  GETNGBL	R3	K3
      0x880C070C,  //  003A  GETMBR	R3	R3	K12
      0x7C040400,  //  003B  CALL	R1	2
      0x88040101,  //  003C  GETMBR	R1	R0	K1
      0x8C04031B,  //  003D  GETMET	R1	R1	K27
      0x7C040200,  //  003E  CALL	R1	1
      0x50040200,  //  003F  LDBOOL	R1	1	0
      0x90020001,  //  0040  SETMBR	R0	K0	R1
      0x8C04011C,  //  0041  GETMET	R1	R0	K28
      0x580C0006,  //  0042  LDCONST	R3	K6
      0x7C040400,  //  0043  CALL	R1	2
      0xA0000000,  //  0044  CLOSE	R0
      0x80000000,  //  0045  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Antiburn
********************************************************************/
be_local_class(Antiburn,
    2,
    NULL,
    be_nested_map(7,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(antiburn, -1), be_const_var(0) },
        { be_const_key_weak(init, -1), be_const_closure(class_Antiburn_init_closure) },
        { be_const_key_weak(running, -1), be_const_var(1) },
        { be_const_key_weak(stop, -1), be_const_closure(class_Antiburn_stop_closure) },
        { be_const_key_weak(cycle, 0), be_const_closure(class_Antiburn_cycle_closure) },
        { be_const_key_weak(colors, 1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(5,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(16711680),
        be_const_int(65280),
        be_const_int(255),
        be_const_int(16777215),
    }))    ) } )) },
        { be_const_key_weak(start, -1), be_const_closure(class_Antiburn_start_closure) },
    })),
    be_str_weak(Antiburn)
);
// compact class 'DimmedPanel' ktab size: 19, total: 20 (saved 8 bytes)
static const bvalue be_ktab_class_DimmedPanel[19] = {
  /* K0   */  be_nested_str_weak(panel),
  /* K1   */  be_nested_str_weak(lv),
  /* K2   */  be_nested_str_weak(obj),
  /* K3   */  be_nested_str_weak(layer_sys),
  /* K4   */  be_nested_str_weak(set_style_radius),
  /* K5   */  be_const_int(0),
  /* K6   */  be_nested_str_weak(set_style_border_width),
  /* K7   */  be_nested_str_weak(set_style_bg_opa),
  /* K8   */  be_nested_str_weak(set_pos),
  /* K9   */  be_nested_str_weak(set_width),
  /* K10  */  be_nested_str_weak(get_hor_res),
  /* K11  */  be_nested_str_weak(set_height),
  /* K12  */  be_nested_str_weak(get_ver_res),
  /* K13  */  be_nested_str_weak(add_flag),
  /* K14  */  be_nested_str_weak(OBJ_FLAG_CLICKABLE),
  /* K15  */  be_nested_str_weak(add_event_cb),
  /* K16  */  be_nested_str_weak(EVENT_PRESSED),
  /* K17  */  be_nested_str_weak(move_foreground),
  /* K18  */  be_nested_str_weak(del),
};


extern const bclass be_class_DimmedPanel;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_DimmedPanel_init,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
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
        /* K0   */  be_nested_str_weak(stop),
        }),
        be_str_weak(_X3Clambda_X3E),
        &be_const_str_solidified,
        ( &(const binstruction[ 4]) {  /* code */
          0x68000000,  //  0000  GETUPV	R0	U0
          0x8C000100,  //  0001  GETMET	R0	R0	K0
          0x7C000200,  //  0002  CALL	R0	1
          0x80040000,  //  0003  RET	1	R0
        })
      ),
    }),
    1,                          /* has constants */
    &be_ktab_class_DimmedPanel,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[52]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x4C080000,  //  0001  LDNIL	R2
      0x1C040202,  //  0002  EQ	R1	R1	R2
      0x7806002A,  //  0003  JMPF	R1	#002F
      0xB8060200,  //  0004  GETNGBL	R1	K1
      0x8C040302,  //  0005  GETMET	R1	R1	K2
      0xB80E0200,  //  0006  GETNGBL	R3	K1
      0x8C0C0703,  //  0007  GETMET	R3	R3	K3
      0x7C0C0200,  //  0008  CALL	R3	1
      0x7C040400,  //  0009  CALL	R1	2
      0x8C080304,  //  000A  GETMET	R2	R1	K4
      0x58100005,  //  000B  LDCONST	R4	K5
      0x58140005,  //  000C  LDCONST	R5	K5
      0x7C080600,  //  000D  CALL	R2	3
      0x8C080306,  //  000E  GETMET	R2	R1	K6
      0x58100005,  //  000F  LDCONST	R4	K5
      0x58140005,  //  0010  LDCONST	R5	K5
      0x7C080600,  //  0011  CALL	R2	3
      0x8C080307,  //  0012  GETMET	R2	R1	K7
      0x58100005,  //  0013  LDCONST	R4	K5
      0x58140005,  //  0014  LDCONST	R5	K5
      0x7C080600,  //  0015  CALL	R2	3
      0x8C080308,  //  0016  GETMET	R2	R1	K8
      0x58100005,  //  0017  LDCONST	R4	K5
      0x58140005,  //  0018  LDCONST	R5	K5
      0x7C080600,  //  0019  CALL	R2	3
      0x8C080309,  //  001A  GETMET	R2	R1	K9
      0xB8120200,  //  001B  GETNGBL	R4	K1
      0x8C10090A,  //  001C  GETMET	R4	R4	K10
      0x7C100200,  //  001D  CALL	R4	1
      0x7C080400,  //  001E  CALL	R2	2
      0x8C08030B,  //  001F  GETMET	R2	R1	K11
      0xB8120200,  //  0020  GETNGBL	R4	K1
      0x8C10090C,  //  0021  GETMET	R4	R4	K12
      0x7C100200,  //  0022  CALL	R4	1
      0x7C080400,  //  0023  CALL	R2	2
      0x8C08030D,  //  0024  GETMET	R2	R1	K13
      0xB8120200,  //  0025  GETNGBL	R4	K1
      0x8810090E,  //  0026  GETMBR	R4	R4	K14
      0x7C080400,  //  0027  CALL	R2	2
      0x8C08030F,  //  0028  GETMET	R2	R1	K15
      0x84100000,  //  0029  CLOSURE	R4	P0
      0xB8160200,  //  002A  GETNGBL	R5	K1
      0x88140B10,  //  002B  GETMBR	R5	R5	K16
      0x58180005,  //  002C  LDCONST	R6	K5
      0x7C080800,  //  002D  CALL	R2	4
      0x90020001,  //  002E  SETMBR	R0	K0	R1
      0x88040100,  //  002F  GETMBR	R1	R0	K0
      0x8C040311,  //  0030  GETMET	R1	R1	K17
      0x7C040200,  //  0031  CALL	R1	1
      0xA0000000,  //  0032  CLOSE	R0
      0x80000000,  //  0033  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: stop
********************************************************************/
be_local_closure(class_DimmedPanel_stop,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DimmedPanel,     /* shared constants */
    be_str_weak(stop),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x4C080000,  //  0001  LDNIL	R2
      0x20040202,  //  0002  NE	R1	R1	R2
      0x78060004,  //  0003  JMPF	R1	#0009
      0x88040100,  //  0004  GETMBR	R1	R0	K0
      0x8C040312,  //  0005  GETMET	R1	R1	K18
      0x7C040200,  //  0006  CALL	R1	1
      0x4C040000,  //  0007  LDNIL	R1
      0x90020001,  //  0008  SETMBR	R0	K0	R1
      0x80000000,  //  0009  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: DimmedPanel
********************************************************************/
be_local_class(DimmedPanel,
    1,
    NULL,
    be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(panel, 1), be_const_var(0) },
        { be_const_key_weak(init, -1), be_const_closure(class_DimmedPanel_init_closure) },
        { be_const_key_weak(stop, -1), be_const_closure(class_DimmedPanel_stop_closure) },
    })),
    be_str_weak(DimmedPanel)
);
extern const bclass be_class_lvh_root;
// compact class 'lvh_root' ktab size: 70, total: 104 (saved 272 bytes)
static const bvalue be_ktab_class_lvh_root[70] = {
  /* K0   */  be_nested_str_weak(remove_val_rule),
  /* K1   */  be_nested_str_weak(_val_rule),
  /* K2   */  be_nested_str_weak(tasmota),
  /* K3   */  be_nested_str_weak(add_rule),
  /* K4   */  be_nested_str_weak(_val_rule_formula),
  /* K5   */  be_nested_str_weak(return_X20_X2F_X20val_X20_X2D_X3E_X20_X28),
  /* K6   */  be_nested_str_weak(_X29),
  /* K7   */  be_nested_str_weak(_val_rule_function),
  /* K8   */  be_nested_str_weak(HSP_X3A_X20failed_X20to_X20compile_X20_X27_X25s_X27_X20_X2D_X20_X25s_X20_X28_X25s_X29),
  /* K9   */  be_const_class(be_class_lvh_root),
  /* K10  */  be_const_int(0),
  /* K11  */  be_nested_str_weak(_X23),
  /* K12  */  be_nested_str_weak(lv),
  /* K13  */  be_nested_str_weak(color),
  /* K14  */  be_nested_str_weak(string),
  /* K15  */  be_nested_str_weak(introspect),
  /* K16  */  be_nested_str_weak(COLOR_),
  /* K17  */  be_nested_str_weak(toupper),
  /* K18  */  be_nested_str_weak(get),
  /* K19  */  be_nested_str_weak(_page),
  /* K20  */  be_nested_str_weak(remove_obj),
  /* K21  */  be_nested_str_weak(id),
  /* K22  */  be_nested_str_weak(int),
  /* K23  */  be_nested_str_weak(font_embedded),
  /* K24  */  be_nested_str_weak(robotocondensed),
  /* K25  */  be_nested_str_weak(path),
  /* K26  */  be_nested_str_weak(roboto_X2Ettf),
  /* K27  */  be_nested_str_weak(exists),
  /* K28  */  be_nested_str_weak(RobotoCondensed_X2DRegular_X2Ettf),
  /* K29  */  be_nested_str_weak(load_freetype_font),
  /* K30  */  be_nested_str_weak(HSP_X3A_X20_X27roboto_X2Ettf_X27_X20file_X20missing_X20for_X20size_X3A),
  /* K31  */  be_nested_str_weak(HSP_X3A_X20Unsupported_X20font_X3A),
  /* K32  */  be_nested_str_weak(split),
  /* K33  */  be_nested_str_weak(_X3A),
  /* K34  */  be_const_int(1),
  /* K35  */  be_nested_str_weak(_X2D),
  /* K36  */  be_const_int(2),
  /* K37  */  be_nested_str_weak(concat),
  /* K38  */  be_nested_str_weak(endswith),
  /* K39  */  be_nested_str_weak(_X2Ettf),
  /* K40  */  be_nested_str_weak(load_font),
  /* K41  */  be_nested_str_weak(_text_rule_formula),
  /* K42  */  be_nested_str_weak(HSP_X3A_X20failed_X20to_X20run_X20self_X2E_val_rule_function_X20_X2D_X20_X25s_X20_X28_X25s_X29),
  /* K43  */  be_nested_str_weak(val),
  /* K44  */  be_nested_str_weak(resize),
  /* K45  */  be_nested_str_weak(_text_rule),
  /* K46  */  be_nested_str_weak(remove_rule),
  /* K47  */  be_nested_str_weak(_tag),
  /* K48  */  be_nested_str_weak(_text_rule_format),
  /* K49  */  be_nested_str_weak(set_text),
  /* K50  */  be_nested_str_weak(_val),
  /* K51  */  be_nested_str_weak(set_val),
  /* K52  */  be_nested_str_weak(_parent_lvh),
  /* K53  */  be_nested_str_weak(_lv_class),
  /* K54  */  be_nested_str_weak(_lv_obj),
  /* K55  */  be_nested_str_weak(post_init),
  /* K56  */  be_nested_str_weak(_meta),
  /* K57  */  be_nested_str_weak(_text_rule_function),
  /* K58  */  be_nested_str_weak(HSP_X3A_X20failed_X20to_X20run_X20self_X2E_text_rule_function_X20_X2D_X20_X25s_X20_X28_X25s_X29),
  /* K59  */  be_nested_str_weak(),
  /* K60  */  be_nested_str_weak(text),
  /* K61  */  be_nested_str_weak(remove_text_rule),
  /* K62  */  be_nested_str_weak(_digit2part),
  /* K63  */  be_nested_str_weak(_digit2state),
  /* K64  */  be_nested_str_weak(invalid_X20style_X20suffix_X20_X2502i),
  /* K65  */  be_nested_str_weak(value_error),
  /* K66  */  be_nested_str_weak(get_text),
  /* K67  */  be_nested_str_weak(_delete),
  /* K68  */  be_nested_str_weak(type_error),
  /* K69  */  be_nested_str_weak(you_X20cannot_X20assign_X20to_X20_X27delete_X27),
};


extern const bclass be_class_lvh_root;

/********************************************************************
** Solidified function: set_val_rule
********************************************************************/
be_local_closure(class_lvh_root_set_val_rule,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
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
        /* K0   */  be_nested_str_weak(val_rule_matched),
        }),
        be_str_weak(_X3Clambda_X3E),
        &be_const_str_solidified,
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
    &be_ktab_class_lvh_root,     /* shared constants */
    be_str_weak(set_val_rule),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x7C080200,  //  0001  CALL	R2	1
      0x60080008,  //  0002  GETGBL	R2	G8
      0x5C0C0200,  //  0003  MOVE	R3	R1
      0x7C080200,  //  0004  CALL	R2	1
      0x90020202,  //  0005  SETMBR	R0	K1	R2
      0xB80A0400,  //  0006  GETNGBL	R2	K2
      0x8C080503,  //  0007  GETMET	R2	R2	K3
      0x88100101,  //  0008  GETMBR	R4	R0	K1
      0x84140000,  //  0009  CLOSURE	R5	P0
      0x5C180000,  //  000A  MOVE	R6	R0
      0x7C080800,  //  000B  CALL	R2	4
      0xA0000000,  //  000C  CLOSE	R0
      0x80000000,  //  000D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_val_rule_formula
********************************************************************/
be_local_closure(class_lvh_root_set_val_rule_formula,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_root,     /* shared constants */
    be_str_weak(set_val_rule_formula),
    &be_const_str_solidified,
    ( &(const binstruction[29]) {  /* code */
      0x60080008,  //  0000  GETGBL	R2	G8
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x7C080200,  //  0002  CALL	R2	1
      0x90020802,  //  0003  SETMBR	R0	K4	R2
      0x88080104,  //  0004  GETMBR	R2	R0	K4
      0x000A0A02,  //  0005  ADD	R2	K5	R2
      0x00080506,  //  0006  ADD	R2	R2	K6
      0xA8020007,  //  0007  EXBLK	0	#0010
      0x600C000D,  //  0008  GETGBL	R3	G13
      0x5C100400,  //  0009  MOVE	R4	R2
      0x7C0C0200,  //  000A  CALL	R3	1
      0x5C100600,  //  000B  MOVE	R4	R3
      0x7C100000,  //  000C  CALL	R4	0
      0x90020E04,  //  000D  SETMBR	R0	K7	R4
      0xA8040001,  //  000E  EXBLK	1	1
      0x7002000B,  //  000F  JMP		#001C
      0xAC0C0002,  //  0010  CATCH	R3	0	2
      0x70020008,  //  0011  JMP		#001B
      0x60140001,  //  0012  GETGBL	R5	G1
      0x60180018,  //  0013  GETGBL	R6	G24
      0x581C0008,  //  0014  LDCONST	R7	K8
      0x5C200400,  //  0015  MOVE	R8	R2
      0x5C240600,  //  0016  MOVE	R9	R3
      0x5C280800,  //  0017  MOVE	R10	R4
      0x7C180800,  //  0018  CALL	R6	4
      0x7C140200,  //  0019  CALL	R5	1
      0x70020000,  //  001A  JMP		#001C
      0xB0080000,  //  001B  RAISE	2	R0	R0
      0x80000000,  //  001C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_color
********************************************************************/
be_local_closure(class_lvh_root_parse_color,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    1,                          /* argc */
    12,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 1]) {
      be_nested_proto(
        10,                          /* nstack */
        1,                          /* argc */
        0,                          /* varg */
        0,                          /* has upvals */
        NULL,                       /* no upvals */
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[14]) {     /* constants */
        /* K0   */  be_nested_str_weak(string),
        /* K1   */  be_const_int(0),
        /* K2   */  be_const_int(1),
        /* K3   */  be_nested_str_weak(_X23),
        /* K4   */  be_nested_str_weak(x),
        /* K5   */  be_nested_str_weak(X),
        /* K6   */  be_nested_str_weak(A),
        /* K7   */  be_nested_str_weak(F),
        /* K8   */  be_nested_str_weak(byte),
        /* K9   */  be_nested_str_weak(a),
        /* K10  */  be_nested_str_weak(f),
        /* K11  */  be_nested_str_weak(0),
        /* K12  */  be_nested_str_weak(9),
        /* K13  */  be_nested_str_weak(stop_iteration),
        }),
        be_str_weak(parse_hex),
        &be_const_str_solidified,
        ( &(const binstruction[67]) {  /* code */
          0xA4060000,  //  0000  IMPORT	R1	K0
          0x58080001,  //  0001  LDCONST	R2	K1
          0x600C0010,  //  0002  GETGBL	R3	G16
          0x6010000C,  //  0003  GETGBL	R4	G12
          0x5C140000,  //  0004  MOVE	R5	R0
          0x7C100200,  //  0005  CALL	R4	1
          0x04100902,  //  0006  SUB	R4	R4	K2
          0x40120204,  //  0007  CONNECT	R4	K1	R4
          0x7C0C0200,  //  0008  CALL	R3	1
          0xA8020034,  //  0009  EXBLK	0	#003F
          0x5C100600,  //  000A  MOVE	R4	R3
          0x7C100000,  //  000B  CALL	R4	0
          0x94140004,  //  000C  GETIDX	R5	R0	R4
          0x1C180B03,  //  000D  EQ	R6	R5	K3
          0x781A0000,  //  000E  JMPF	R6	#0010
          0x7001FFF9,  //  000F  JMP		#000A
          0x1C180B04,  //  0010  EQ	R6	R5	K4
          0x741A0001,  //  0011  JMPT	R6	#0014
          0x1C180B05,  //  0012  EQ	R6	R5	K5
          0x781A0000,  //  0013  JMPF	R6	#0015
          0x7001FFF4,  //  0014  JMP		#000A
          0x28180B06,  //  0015  GE	R6	R5	K6
          0x781A000B,  //  0016  JMPF	R6	#0023
          0x18180B07,  //  0017  LE	R6	R5	K7
          0x781A0009,  //  0018  JMPF	R6	#0023
          0x541A0003,  //  0019  LDINT	R6	4
          0x38180406,  //  001A  SHL	R6	R2	R6
          0x8C1C0308,  //  001B  GETMET	R7	R1	K8
          0x5C240A00,  //  001C  MOVE	R9	R5
          0x7C1C0400,  //  001D  CALL	R7	2
          0x54220036,  //  001E  LDINT	R8	55
          0x041C0E08,  //  001F  SUB	R7	R7	R8
          0x30180C07,  //  0020  OR	R6	R6	R7
          0x5C080C00,  //  0021  MOVE	R2	R6
          0x7002001A,  //  0022  JMP		#003E
          0x28180B09,  //  0023  GE	R6	R5	K9
          0x781A000B,  //  0024  JMPF	R6	#0031
          0x18180B0A,  //  0025  LE	R6	R5	K10
          0x781A0009,  //  0026  JMPF	R6	#0031
          0x541A0003,  //  0027  LDINT	R6	4
          0x38180406,  //  0028  SHL	R6	R2	R6
          0x8C1C0308,  //  0029  GETMET	R7	R1	K8
          0x5C240A00,  //  002A  MOVE	R9	R5
          0x7C1C0400,  //  002B  CALL	R7	2
          0x54220056,  //  002C  LDINT	R8	87
          0x041C0E08,  //  002D  SUB	R7	R7	R8
          0x30180C07,  //  002E  OR	R6	R6	R7
          0x5C080C00,  //  002F  MOVE	R2	R6
          0x7002000C,  //  0030  JMP		#003E
          0x28180B0B,  //  0031  GE	R6	R5	K11
          0x781A000A,  //  0032  JMPF	R6	#003E
          0x18180B0C,  //  0033  LE	R6	R5	K12
          0x781A0008,  //  0034  JMPF	R6	#003E
          0x541A0003,  //  0035  LDINT	R6	4
          0x38180406,  //  0036  SHL	R6	R2	R6
          0x8C1C0308,  //  0037  GETMET	R7	R1	K8
          0x5C240A00,  //  0038  MOVE	R9	R5
          0x7C1C0400,  //  0039  CALL	R7	2
          0x5422002F,  //  003A  LDINT	R8	48
          0x041C0E08,  //  003B  SUB	R7	R7	R8
          0x30180C07,  //  003C  OR	R6	R6	R7
          0x5C080C00,  //  003D  MOVE	R2	R6
          0x7001FFCA,  //  003E  JMP		#000A
          0x580C000D,  //  003F  LDCONST	R3	K13
          0xAC0C0200,  //  0040  CATCH	R3	1	0
          0xB0080000,  //  0041  RAISE	2	R0	R0
          0x80040400,  //  0042  RET	1	R2
        })
      ),
    }),
    1,                          /* has constants */
    &be_ktab_class_lvh_root,     /* shared constants */
    be_str_weak(parse_color),
    &be_const_str_solidified,
    ( &(const binstruction[40]) {  /* code */
      0x58040009,  //  0000  LDCONST	R1	K9
      0x84080000,  //  0001  CLOSURE	R2	P0
      0x600C0008,  //  0002  GETGBL	R3	G8
      0x5C100000,  //  0003  MOVE	R4	R0
      0x7C0C0200,  //  0004  CALL	R3	1
      0x5C000600,  //  0005  MOVE	R0	R3
      0x940C010A,  //  0006  GETIDX	R3	R0	K10
      0x1C0C070B,  //  0007  EQ	R3	R3	K11
      0x780E0007,  //  0008  JMPF	R3	#0011
      0xB80E1800,  //  0009  GETNGBL	R3	K12
      0x8C0C070D,  //  000A  GETMET	R3	R3	K13
      0x5C140400,  //  000B  MOVE	R5	R2
      0x5C180000,  //  000C  MOVE	R6	R0
      0x7C140200,  //  000D  CALL	R5	1
      0x7C0C0400,  //  000E  CALL	R3	2
      0x80040600,  //  000F  RET	1	R3
      0x70020011,  //  0010  JMP		#0023
      0xA40E1C00,  //  0011  IMPORT	R3	K14
      0xA4121E00,  //  0012  IMPORT	R4	K15
      0x8C140711,  //  0013  GETMET	R5	R3	K17
      0x5C1C0000,  //  0014  MOVE	R7	R0
      0x7C140400,  //  0015  CALL	R5	2
      0x00162005,  //  0016  ADD	R5	K16	R5
      0x8C180912,  //  0017  GETMET	R6	R4	K18
      0xB8221800,  //  0018  GETNGBL	R8	K12
      0x5C240A00,  //  0019  MOVE	R9	R5
      0x7C180600,  //  001A  CALL	R6	3
      0x4C1C0000,  //  001B  LDNIL	R7
      0x201C0C07,  //  001C  NE	R7	R6	R7
      0x781E0004,  //  001D  JMPF	R7	#0023
      0xB81E1800,  //  001E  GETNGBL	R7	K12
      0x8C1C0F0D,  //  001F  GETMET	R7	R7	K13
      0x5C240C00,  //  0020  MOVE	R9	R6
      0x7C1C0400,  //  0021  CALL	R7	2
      0x80040E00,  //  0022  RET	1	R7
      0xB80E1800,  //  0023  GETNGBL	R3	K12
      0x8C0C070D,  //  0024  GETMET	R3	R3	K13
      0x5814000A,  //  0025  LDCONST	R5	K10
      0x7C0C0400,  //  0026  CALL	R3	2
      0x80040600,  //  0027  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _delete
********************************************************************/
be_local_closure(class_lvh_root__delete,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_root,     /* shared constants */
    be_str_weak(_delete),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x88040113,  //  0000  GETMBR	R1	R0	K19
      0x8C040314,  //  0001  GETMET	R1	R1	K20
      0x880C0115,  //  0002  GETMBR	R3	R0	K21
      0x7C040400,  //  0003  CALL	R1	2
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_font
********************************************************************/
be_local_closure(class_lvh_root_parse_font,   /* name */
  be_nested_proto(
    15,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_root,     /* shared constants */
    be_str_weak(parse_font),
    &be_const_str_solidified,
    ( &(const binstruction[172]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x600C0004,  //  0001  GETGBL	R3	G4
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C0C0200,  //  0003  CALL	R3	1
      0x1C0C0716,  //  0004  EQ	R3	R3	K22
      0x780E0038,  //  0005  JMPF	R3	#003F
      0xA8020007,  //  0006  EXBLK	0	#000F
      0xB80E1800,  //  0007  GETNGBL	R3	K12
      0x8C0C0717,  //  0008  GETMET	R3	R3	K23
      0x58140018,  //  0009  LDCONST	R5	K24
      0x5C180200,  //  000A  MOVE	R6	R1
      0x7C0C0600,  //  000B  CALL	R3	3
      0x5C080600,  //  000C  MOVE	R2	R3
      0xA8040001,  //  000D  EXBLK	1	1
      0x7002002E,  //  000E  JMP		#003E
      0xAC0C0000,  //  000F  CATCH	R3	0	0
      0x7002002B,  //  0010  JMP		#003D
      0xA40E3200,  //  0011  IMPORT	R3	K25
      0xA802001E,  //  0012  EXBLK	0	#0032
      0x5810001A,  //  0013  LDCONST	R4	K26
      0x8C14071B,  //  0014  GETMET	R5	R3	K27
      0x5C1C0800,  //  0015  MOVE	R7	R4
      0x7C140400,  //  0016  CALL	R5	2
      0x74160005,  //  0017  JMPT	R5	#001E
      0x5810001C,  //  0018  LDCONST	R4	K28
      0x8C14071B,  //  0019  GETMET	R5	R3	K27
      0x5C1C0800,  //  001A  MOVE	R7	R4
      0x7C140400,  //  001B  CALL	R5	2
      0x74160000,  //  001C  JMPT	R5	#001E
      0x4C100000,  //  001D  LDNIL	R4
      0x4C140000,  //  001E  LDNIL	R5
      0x20140805,  //  001F  NE	R5	R4	R5
      0x78160007,  //  0020  JMPF	R5	#0029
      0xB8161800,  //  0021  GETNGBL	R5	K12
      0x8C140B1D,  //  0022  GETMET	R5	R5	K29
      0x5C1C0800,  //  0023  MOVE	R7	R4
      0x5C200200,  //  0024  MOVE	R8	R1
      0x5824000A,  //  0025  LDCONST	R9	K10
      0x7C140800,  //  0026  CALL	R5	4
      0x5C080A00,  //  0027  MOVE	R2	R5
      0x70020006,  //  0028  JMP		#0030
      0x60140001,  //  0029  GETGBL	R5	G1
      0x5818001E,  //  002A  LDCONST	R6	K30
      0x5C1C0200,  //  002B  MOVE	R7	R1
      0x7C140400,  //  002C  CALL	R5	2
      0x4C140000,  //  002D  LDNIL	R5
      0xA8040001,  //  002E  EXBLK	1	1
      0x80040A00,  //  002F  RET	1	R5
      0xA8040001,  //  0030  EXBLK	1	1
      0x70020009,  //  0031  JMP		#003C
      0xAC100000,  //  0032  CATCH	R4	0	0
      0x70020006,  //  0033  JMP		#003B
      0x60100001,  //  0034  GETGBL	R4	G1
      0x5814001F,  //  0035  LDCONST	R5	K31
      0x5C180200,  //  0036  MOVE	R6	R1
      0x7C100400,  //  0037  CALL	R4	2
      0x4C100000,  //  0038  LDNIL	R4
      0x80040800,  //  0039  RET	1	R4
      0x70020000,  //  003A  JMP		#003C
      0xB0080000,  //  003B  RAISE	2	R0	R0
      0x70020000,  //  003C  JMP		#003E
      0xB0080000,  //  003D  RAISE	2	R0	R0
      0x70020062,  //  003E  JMP		#00A2
      0x600C0004,  //  003F  GETGBL	R3	G4
      0x5C100200,  //  0040  MOVE	R4	R1
      0x7C0C0200,  //  0041  CALL	R3	1
      0x1C0C070E,  //  0042  EQ	R3	R3	K14
      0x780E005D,  //  0043  JMPF	R3	#00A2
      0xA40E1C00,  //  0044  IMPORT	R3	K14
      0x8C100720,  //  0045  GETMET	R4	R3	K32
      0x5C180200,  //  0046  MOVE	R6	R1
      0x581C0021,  //  0047  LDCONST	R7	K33
      0x58200022,  //  0048  LDCONST	R8	K34
      0x7C100800,  //  0049  CALL	R4	4
      0x8C140720,  //  004A  GETMET	R5	R3	K32
      0x5C1C0200,  //  004B  MOVE	R7	R1
      0x58200023,  //  004C  LDCONST	R8	K35
      0x7C140600,  //  004D  CALL	R5	3
      0x5C180200,  //  004E  MOVE	R6	R1
      0x581C000A,  //  004F  LDCONST	R7	K10
      0x50200000,  //  0050  LDBOOL	R8	0	0
      0x6024000C,  //  0051  GETGBL	R9	G12
      0x5C280800,  //  0052  MOVE	R10	R4
      0x7C240200,  //  0053  CALL	R9	1
      0x24241322,  //  0054  GT	R9	R9	K34
      0x78260003,  //  0055  JMPF	R9	#005A
      0x6024000C,  //  0056  GETGBL	R9	G12
      0x9428090A,  //  0057  GETIDX	R10	R4	K10
      0x7C240200,  //  0058  CALL	R9	1
      0x74260000,  //  0059  JMPT	R9	#005B
      0x50240001,  //  005A  LDBOOL	R9	0	1
      0x50240200,  //  005B  LDBOOL	R9	1	0
      0x6028000C,  //  005C  GETGBL	R10	G12
      0x5C2C0A00,  //  005D  MOVE	R11	R5
      0x7C280200,  //  005E  CALL	R10	1
      0x28281524,  //  005F  GE	R10	R10	K36
      0x782A000B,  //  0060  JMPF	R10	#006D
      0x60280009,  //  0061  GETGBL	R10	G9
      0x542DFFFE,  //  0062  LDINT	R11	-1
      0x942C0A0B,  //  0063  GETIDX	R11	R5	R11
      0x7C280200,  //  0064  CALL	R10	1
      0x5C1C1400,  //  0065  MOVE	R7	R10
      0x5429FFFD,  //  0066  LDINT	R10	-2
      0x402A140A,  //  0067  CONNECT	R10	K10	R10
      0x94280A0A,  //  0068  GETIDX	R10	R5	R10
      0x8C281525,  //  0069  GETMET	R10	R10	K37
      0x58300023,  //  006A  LDCONST	R12	K35
      0x7C280400,  //  006B  CALL	R10	2
      0x5C181400,  //  006C  MOVE	R6	R10
      0x8C280726,  //  006D  GETMET	R10	R3	K38
      0x5C300C00,  //  006E  MOVE	R12	R6
      0x58340027,  //  006F  LDCONST	R13	K39
      0x50380200,  //  0070  LDBOOL	R14	1	0
      0x7C280800,  //  0071  CALL	R10	4
      0x782A0006,  //  0072  JMPF	R10	#007A
      0x8C280720,  //  0073  GETMET	R10	R3	K32
      0x5C300C00,  //  0074  MOVE	R12	R6
      0x58340021,  //  0075  LDCONST	R13	K33
      0x7C280600,  //  0076  CALL	R10	3
      0x542DFFFE,  //  0077  LDINT	R11	-1
      0x9418140B,  //  0078  GETIDX	R6	R10	R11
      0x50200200,  //  0079  LDBOOL	R8	1	0
      0x78220007,  //  007A  JMPF	R8	#0083
      0xB82A1800,  //  007B  GETNGBL	R10	K12
      0x8C28151D,  //  007C  GETMET	R10	R10	K29
      0x5C300C00,  //  007D  MOVE	R12	R6
      0x5C340E00,  //  007E  MOVE	R13	R7
      0x5838000A,  //  007F  LDCONST	R14	K10
      0x7C280800,  //  0080  CALL	R10	4
      0x5C081400,  //  0081  MOVE	R2	R10
      0x7002001E,  //  0082  JMP		#00A2
      0x78260005,  //  0083  JMPF	R9	#008A
      0xB82A1800,  //  0084  GETNGBL	R10	K12
      0x8C281528,  //  0085  GETMET	R10	R10	K40
      0x5C300200,  //  0086  MOVE	R12	R1
      0x7C280400,  //  0087  CALL	R10	2
      0x5C081400,  //  0088  MOVE	R2	R10
      0x70020017,  //  0089  JMP		#00A2
      0x24280F0A,  //  008A  GT	R10	R7	K10
      0x782A0015,  //  008B  JMPF	R10	#00A2
      0x6028000C,  //  008C  GETGBL	R10	G12
      0x5C2C0C00,  //  008D  MOVE	R11	R6
      0x7C280200,  //  008E  CALL	R10	1
      0x2428150A,  //  008F  GT	R10	R10	K10
      0x782A0010,  //  0090  JMPF	R10	#00A2
      0xA8020007,  //  0091  EXBLK	0	#009A
      0xB82A1800,  //  0092  GETNGBL	R10	K12
      0x8C281517,  //  0093  GETMET	R10	R10	K23
      0x5C300C00,  //  0094  MOVE	R12	R6
      0x5C340E00,  //  0095  MOVE	R13	R7
      0x7C280600,  //  0096  CALL	R10	3
      0x5C081400,  //  0097  MOVE	R2	R10
      0xA8040001,  //  0098  EXBLK	1	1
      0x70020007,  //  0099  JMP		#00A2
      0xAC280000,  //  009A  CATCH	R10	0	0
      0x70020004,  //  009B  JMP		#00A1
      0x60280001,  //  009C  GETGBL	R10	G1
      0x582C001F,  //  009D  LDCONST	R11	K31
      0x5C300200,  //  009E  MOVE	R12	R1
      0x7C280400,  //  009F  CALL	R10	2
      0x70020000,  //  00A0  JMP		#00A2
      0xB0080000,  //  00A1  RAISE	2	R0	R0
      0x4C0C0000,  //  00A2  LDNIL	R3
      0x200C0403,  //  00A3  NE	R3	R2	R3
      0x780E0001,  //  00A4  JMPF	R3	#00A7
      0x80040400,  //  00A5  RET	1	R2
      0x70020003,  //  00A6  JMP		#00AB
      0x600C0001,  //  00A7  GETGBL	R3	G1
      0x5810001F,  //  00A8  LDCONST	R4	K31
      0x5C140200,  //  00A9  MOVE	R5	R1
      0x7C0C0400,  //  00AA  CALL	R3	2
      0x80000000,  //  00AB  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_val_rule
********************************************************************/
be_local_closure(class_lvh_root_get_val_rule,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_root,     /* shared constants */
    be_str_weak(get_val_rule),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040101,  //  0000  GETMBR	R1	R0	K1
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_text_rule_formula
********************************************************************/
be_local_closure(class_lvh_root_get_text_rule_formula,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_root,     /* shared constants */
    be_str_weak(get_text_rule_formula),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040129,  //  0000  GETMBR	R1	R0	K41
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: val_rule_matched
********************************************************************/
be_local_closure(class_lvh_root_val_rule_matched,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_root,     /* shared constants */
    be_str_weak(val_rule_matched),
    &be_const_str_solidified,
    ( &(const binstruction[36]) {  /* code */
      0x6008000A,  //  0000  GETGBL	R2	G10
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x7C080200,  //  0002  CALL	R2	1
      0x4C0C0000,  //  0003  LDNIL	R3
      0x1C0C0403,  //  0004  EQ	R3	R2	R3
      0x780E0001,  //  0005  JMPF	R3	#0008
      0x500C0000,  //  0006  LDBOOL	R3	0	0
      0x80040600,  //  0007  RET	1	R3
      0x880C0107,  //  0008  GETMBR	R3	R0	K7
      0x4C100000,  //  0009  LDNIL	R4
      0x20100604,  //  000A  NE	R4	R3	R4
      0x78120011,  //  000B  JMPF	R4	#001E
      0xA8020005,  //  000C  EXBLK	0	#0013
      0x5C100600,  //  000D  MOVE	R4	R3
      0x5C140400,  //  000E  MOVE	R5	R2
      0x7C100200,  //  000F  CALL	R4	1
      0x5C080800,  //  0010  MOVE	R2	R4
      0xA8040001,  //  0011  EXBLK	1	1
      0x7002000A,  //  0012  JMP		#001E
      0xAC100002,  //  0013  CATCH	R4	0	2
      0x70020007,  //  0014  JMP		#001D
      0x60180001,  //  0015  GETGBL	R6	G1
      0x601C0018,  //  0016  GETGBL	R7	G24
      0x5820002A,  //  0017  LDCONST	R8	K42
      0x5C240800,  //  0018  MOVE	R9	R4
      0x5C280A00,  //  0019  MOVE	R10	R5
      0x7C1C0600,  //  001A  CALL	R7	3
      0x7C180200,  //  001B  CALL	R6	1
      0x70020000,  //  001C  JMP		#001E
      0xB0080000,  //  001D  RAISE	2	R0	R0
      0x60100009,  //  001E  GETGBL	R4	G9
      0x5C140400,  //  001F  MOVE	R5	R2
      0x7C100200,  //  0020  CALL	R4	1
      0x90025604,  //  0021  SETMBR	R0	K43	R4
      0x50100000,  //  0022  LDBOOL	R4	0	0
      0x80040800,  //  0023  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_text
********************************************************************/
be_local_closure(class_lvh_root_get_text,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_root,     /* shared constants */
    be_str_weak(get_text),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x4C040000,  //  0000  LDNIL	R1
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: remove_trailing_zeroes
********************************************************************/
be_local_closure(class_lvh_root_remove_trailing_zeroes,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    12,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_root,     /* shared constants */
    be_str_weak(remove_trailing_zeroes),
    &be_const_str_solidified,
    ( &(const binstruction[24]) {  /* code */
      0x58040009,  //  0000  LDCONST	R1	K9
      0x6008000C,  //  0001  GETGBL	R2	G12
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C080200,  //  0003  CALL	R2	1
      0x580C000A,  //  0004  LDCONST	R3	K10
      0x14100602,  //  0005  LT	R4	R3	R2
      0x78120007,  //  0006  JMPF	R4	#000F
      0x5411FFFE,  //  0007  LDINT	R4	-1
      0x04100803,  //  0008  SUB	R4	R4	R3
      0x94100004,  //  0009  GETIDX	R4	R0	R4
      0x2010090A,  //  000A  NE	R4	R4	K10
      0x78120000,  //  000B  JMPF	R4	#000D
      0x70020001,  //  000C  JMP		#000F
      0x000C0722,  //  000D  ADD	R3	R3	K34
      0x7001FFF5,  //  000E  JMP		#0005
      0x2410070A,  //  000F  GT	R4	R3	K10
      0x78120005,  //  0010  JMPF	R4	#0017
      0x8C10012C,  //  0011  GETMET	R4	R0	K44
      0x6018000C,  //  0012  GETGBL	R6	G12
      0x5C1C0000,  //  0013  MOVE	R7	R0
      0x7C180200,  //  0014  CALL	R6	1
      0x04180C03,  //  0015  SUB	R6	R6	R3
      0x7C100400,  //  0016  CALL	R4	2
      0x80040000,  //  0017  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: remove_text_rule
********************************************************************/
be_local_closure(class_lvh_root_remove_text_rule,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_root,     /* shared constants */
    be_str_weak(remove_text_rule),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x8804012D,  //  0000  GETMBR	R1	R0	K45
      0x4C080000,  //  0001  LDNIL	R2
      0x20040202,  //  0002  NE	R1	R1	R2
      0x78060004,  //  0003  JMPF	R1	#0009
      0xB8060400,  //  0004  GETNGBL	R1	K2
      0x8C04032E,  //  0005  GETMET	R1	R1	K46
      0x880C012D,  //  0006  GETMBR	R3	R0	K45
      0x5C100000,  //  0007  MOVE	R4	R0
      0x7C040600,  //  0008  CALL	R1	3
      0x80000000,  //  0009  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_tag
********************************************************************/
be_local_closure(class_lvh_root_set_tag,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_root,     /* shared constants */
    be_str_weak(set_tag),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x90025E01,  //  0000  SETMBR	R0	K47	R1
      0x80000000,  //  0001  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_text_rule_format
********************************************************************/
be_local_closure(class_lvh_root_set_text_rule_format,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_root,     /* shared constants */
    be_str_weak(set_text_rule_format),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x60080008,  //  0000  GETGBL	R2	G8
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x7C080200,  //  0002  CALL	R2	1
      0x90026002,  //  0003  SETMBR	R0	K48	R2
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_text_rule
********************************************************************/
be_local_closure(class_lvh_root_get_text_rule,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_root,     /* shared constants */
    be_str_weak(get_text_rule),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x8804012D,  //  0000  GETMBR	R1	R0	K45
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_value_str
********************************************************************/
be_local_closure(class_lvh_root_set_value_str,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_root,     /* shared constants */
    be_str_weak(set_value_str),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x8C080131,  //  0000  GETMET	R2	R0	K49
      0x5C100200,  //  0001  MOVE	R4	R1
      0x7C080400,  //  0002  CALL	R2	2
      0x80000000,  //  0003  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: post_config
********************************************************************/
be_local_closure(class_lvh_root_post_config,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_root,     /* shared constants */
    be_str_weak(post_config),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x88040132,  //  0000  GETMBR	R1	R0	K50
      0x4C080000,  //  0001  LDNIL	R2
      0x20040202,  //  0002  NE	R1	R1	R2
      0x78060002,  //  0003  JMPF	R1	#0007
      0x8C040133,  //  0004  GETMET	R1	R0	K51
      0x880C0132,  //  0005  GETMBR	R3	R0	K50
      0x7C040400,  //  0006  CALL	R1	2
      0x80000000,  //  0007  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_lvh_root_init,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    6,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_root,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x90022602,  //  0000  SETMBR	R0	K19	R2
      0x90026805,  //  0001  SETMBR	R0	K52	R5
      0x4C180000,  //  0002  LDNIL	R6
      0x1C180806,  //  0003  EQ	R6	R4	R6
      0x781A0007,  //  0004  JMPF	R6	#000D
      0x88180135,  //  0005  GETMBR	R6	R0	K53
      0x781A0005,  //  0006  JMPF	R6	#000D
      0x88180135,  //  0007  GETMBR	R6	R0	K53
      0x5C1C0C00,  //  0008  MOVE	R7	R6
      0x5C200200,  //  0009  MOVE	R8	R1
      0x7C1C0200,  //  000A  CALL	R7	1
      0x90026C07,  //  000B  SETMBR	R0	K54	R7
      0x70020000,  //  000C  JMP		#000E
      0x90026C04,  //  000D  SETMBR	R0	K54	R4
      0x8C180137,  //  000E  GETMET	R6	R0	K55
      0x5C200600,  //  000F  MOVE	R8	R3
      0x7C180400,  //  0010  CALL	R6	2
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_tag
********************************************************************/
be_local_closure(class_lvh_root_get_tag,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_root,     /* shared constants */
    be_str_weak(get_tag),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x8804012F,  //  0000  GETMBR	R1	R0	K47
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_meta
********************************************************************/
be_local_closure(class_lvh_root_set_meta,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_root,     /* shared constants */
    be_str_weak(set_meta),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x90027001,  //  0000  SETMBR	R0	K56	R1
      0x80000000,  //  0001  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: text_rule_matched
********************************************************************/
be_local_closure(class_lvh_root_text_rule_matched,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_root,     /* shared constants */
    be_str_weak(text_rule_matched),
    &be_const_str_solidified,
    ( &(const binstruction[47]) {  /* code */
      0x60080004,  //  0000  GETGBL	R2	G4
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x7C080200,  //  0002  CALL	R2	1
      0x1C080516,  //  0003  EQ	R2	R2	K22
      0x780A0003,  //  0004  JMPF	R2	#0009
      0x6008000A,  //  0005  GETGBL	R2	G10
      0x5C0C0200,  //  0006  MOVE	R3	R1
      0x7C080200,  //  0007  CALL	R2	1
      0x5C040400,  //  0008  MOVE	R1	R2
      0x88080139,  //  0009  GETMBR	R2	R0	K57
      0x4C0C0000,  //  000A  LDNIL	R3
      0x200C0403,  //  000B  NE	R3	R2	R3
      0x780E0011,  //  000C  JMPF	R3	#001F
      0xA8020005,  //  000D  EXBLK	0	#0014
      0x5C0C0400,  //  000E  MOVE	R3	R2
      0x5C100200,  //  000F  MOVE	R4	R1
      0x7C0C0200,  //  0010  CALL	R3	1
      0x5C040600,  //  0011  MOVE	R1	R3
      0xA8040001,  //  0012  EXBLK	1	1
      0x7002000A,  //  0013  JMP		#001F
      0xAC0C0002,  //  0014  CATCH	R3	0	2
      0x70020007,  //  0015  JMP		#001E
      0x60140001,  //  0016  GETGBL	R5	G1
      0x60180018,  //  0017  GETGBL	R6	G24
      0x581C003A,  //  0018  LDCONST	R7	K58
      0x5C200600,  //  0019  MOVE	R8	R3
      0x5C240800,  //  001A  MOVE	R9	R4
      0x7C180600,  //  001B  CALL	R6	3
      0x7C140200,  //  001C  CALL	R5	1
      0x70020000,  //  001D  JMP		#001F
      0xB0080000,  //  001E  RAISE	2	R0	R0
      0x880C0130,  //  001F  GETMBR	R3	R0	K48
      0x60100004,  //  0020  GETGBL	R4	G4
      0x5C140600,  //  0021  MOVE	R5	R3
      0x7C100200,  //  0022  CALL	R4	1
      0x1C10090E,  //  0023  EQ	R4	R4	K14
      0x78120005,  //  0024  JMPF	R4	#002B
      0x60100018,  //  0025  GETGBL	R4	G24
      0x5C140600,  //  0026  MOVE	R5	R3
      0x5C180200,  //  0027  MOVE	R6	R1
      0x7C100400,  //  0028  CALL	R4	2
      0x5C0C0800,  //  0029  MOVE	R3	R4
      0x70020000,  //  002A  JMP		#002C
      0x580C003B,  //  002B  LDCONST	R3	K59
      0x90027803,  //  002C  SETMBR	R0	K60	R3
      0x50100000,  //  002D  LDBOOL	R4	0	0
      0x80040800,  //  002E  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_meta
********************************************************************/
be_local_closure(class_lvh_root_get_meta,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_root,     /* shared constants */
    be_str_weak(get_meta),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040138,  //  0000  GETMBR	R1	R0	K56
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_text_rule_formula
********************************************************************/
be_local_closure(class_lvh_root_set_text_rule_formula,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_root,     /* shared constants */
    be_str_weak(set_text_rule_formula),
    &be_const_str_solidified,
    ( &(const binstruction[29]) {  /* code */
      0x60080008,  //  0000  GETGBL	R2	G8
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x7C080200,  //  0002  CALL	R2	1
      0x90025202,  //  0003  SETMBR	R0	K41	R2
      0x88080129,  //  0004  GETMBR	R2	R0	K41
      0x000A0A02,  //  0005  ADD	R2	K5	R2
      0x00080506,  //  0006  ADD	R2	R2	K6
      0xA8020007,  //  0007  EXBLK	0	#0010
      0x600C000D,  //  0008  GETGBL	R3	G13
      0x5C100400,  //  0009  MOVE	R4	R2
      0x7C0C0200,  //  000A  CALL	R3	1
      0x5C100600,  //  000B  MOVE	R4	R3
      0x7C100000,  //  000C  CALL	R4	0
      0x90027204,  //  000D  SETMBR	R0	K57	R4
      0xA8040001,  //  000E  EXBLK	1	1
      0x7002000B,  //  000F  JMP		#001C
      0xAC0C0002,  //  0010  CATCH	R3	0	2
      0x70020008,  //  0011  JMP		#001B
      0x60140001,  //  0012  GETGBL	R5	G1
      0x60180018,  //  0013  GETGBL	R6	G24
      0x581C0008,  //  0014  LDCONST	R7	K8
      0x5C200400,  //  0015  MOVE	R8	R2
      0x5C240600,  //  0016  MOVE	R9	R3
      0x5C280800,  //  0017  MOVE	R10	R4
      0x7C180800,  //  0018  CALL	R6	4
      0x7C140200,  //  0019  CALL	R5	1
      0x70020000,  //  001A  JMP		#001C
      0xB0080000,  //  001B  RAISE	2	R0	R0
      0x80000000,  //  001C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_text
********************************************************************/
be_local_closure(class_lvh_root_set_text,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_root,     /* shared constants */
    be_str_weak(set_text),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_text_rule
********************************************************************/
be_local_closure(class_lvh_root_set_text_rule,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
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
        /* K0   */  be_nested_str_weak(text_rule_matched),
        }),
        be_str_weak(_X3Clambda_X3E),
        &be_const_str_solidified,
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
    &be_ktab_class_lvh_root,     /* shared constants */
    be_str_weak(set_text_rule),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x8C08013D,  //  0000  GETMET	R2	R0	K61
      0x7C080200,  //  0001  CALL	R2	1
      0x60080008,  //  0002  GETGBL	R2	G8
      0x5C0C0200,  //  0003  MOVE	R3	R1
      0x7C080200,  //  0004  CALL	R2	1
      0x90025A02,  //  0005  SETMBR	R0	K45	R2
      0xB80A0400,  //  0006  GETNGBL	R2	K2
      0x8C080503,  //  0007  GETMET	R2	R2	K3
      0x8810012D,  //  0008  GETMBR	R4	R0	K45
      0x84140000,  //  0009  CLOSURE	R5	P0
      0x5C180000,  //  000A  MOVE	R6	R0
      0x7C080800,  //  000B  CALL	R2	4
      0xA0000000,  //  000C  CLOSE	R0
      0x80000000,  //  000D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: remove_val_rule
********************************************************************/
be_local_closure(class_lvh_root_remove_val_rule,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_root,     /* shared constants */
    be_str_weak(remove_val_rule),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x88040101,  //  0000  GETMBR	R1	R0	K1
      0x4C080000,  //  0001  LDNIL	R2
      0x20040202,  //  0002  NE	R1	R1	R2
      0x78060004,  //  0003  JMPF	R1	#0009
      0xB8060400,  //  0004  GETNGBL	R1	K2
      0x8C04032E,  //  0005  GETMET	R1	R1	K46
      0x880C0101,  //  0006  GETMBR	R3	R0	K1
      0x5C100000,  //  0007  MOVE	R4	R0
      0x7C040600,  //  0008  CALL	R1	3
      0x80000000,  //  0009  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_text_rule_format
********************************************************************/
be_local_closure(class_lvh_root_get_text_rule_format,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_root,     /* shared constants */
    be_str_weak(get_text_rule_format),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040130,  //  0000  GETMBR	R1	R0	K48
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_obj
********************************************************************/
be_local_closure(class_lvh_root_get_obj,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_root,     /* shared constants */
    be_str_weak(get_obj),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040136,  //  0000  GETMBR	R1	R0	K54
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_color_attribute
********************************************************************/
be_local_closure(class_lvh_root_is_color_attribute,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_root,     /* shared constants */
    be_str_weak(is_color_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0xA40A1C00,  //  0000  IMPORT	R2	K14
      0x8C0C0526,  //  0001  GETMET	R3	R2	K38
      0x60140008,  //  0002  GETGBL	R5	G8
      0x5C180200,  //  0003  MOVE	R6	R1
      0x7C140200,  //  0004  CALL	R5	1
      0x5818000D,  //  0005  LDCONST	R6	K13
      0x7C0C0600,  //  0006  CALL	R3	3
      0x80040600,  //  0007  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: digits_to_style
********************************************************************/
be_local_closure(class_lvh_root_digits_to_style,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_root,     /* shared constants */
    be_str_weak(digits_to_style),
    &be_const_str_solidified,
    ( &(const binstruction[44]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0000,  //  0002  JMPF	R2	#0004
      0x80061400,  //  0003  RET	1	K10
      0x540A0009,  //  0004  LDINT	R2	10
      0x0C080202,  //  0005  DIV	R2	R1	R2
      0x540E0009,  //  0006  LDINT	R3	10
      0x10080403,  //  0007  MOD	R2	R2	R3
      0x540E0009,  //  0008  LDINT	R3	10
      0x100C0203,  //  0009  MOD	R3	R1	R3
      0x5810000A,  //  000A  LDCONST	R4	K10
      0x2814050A,  //  000B  GE	R5	R2	K10
      0x78160008,  //  000C  JMPF	R5	#0016
      0x6014000C,  //  000D  GETGBL	R5	G12
      0x8818013E,  //  000E  GETMBR	R6	R0	K62
      0x7C140200,  //  000F  CALL	R5	1
      0x14140405,  //  0010  LT	R5	R2	R5
      0x78160003,  //  0011  JMPF	R5	#0016
      0x8814013E,  //  0012  GETMBR	R5	R0	K62
      0x94140A02,  //  0013  GETIDX	R5	R5	R2
      0x30100805,  //  0014  OR	R4	R4	R5
      0x70020000,  //  0015  JMP		#0017
      0x4C100000,  //  0016  LDNIL	R4
      0x2814070A,  //  0017  GE	R5	R3	K10
      0x78160008,  //  0018  JMPF	R5	#0022
      0x6014000C,  //  0019  GETGBL	R5	G12
      0x8818013F,  //  001A  GETMBR	R6	R0	K63
      0x7C140200,  //  001B  CALL	R5	1
      0x14140605,  //  001C  LT	R5	R3	R5
      0x78160003,  //  001D  JMPF	R5	#0022
      0x8814013F,  //  001E  GETMBR	R5	R0	K63
      0x94140A03,  //  001F  GETIDX	R5	R5	R3
      0x30100805,  //  0020  OR	R4	R4	R5
      0x70020000,  //  0021  JMP		#0023
      0x4C100000,  //  0022  LDNIL	R4
      0x4C140000,  //  0023  LDNIL	R5
      0x1C140805,  //  0024  EQ	R5	R4	R5
      0x78160004,  //  0025  JMPF	R5	#002B
      0x60140018,  //  0026  GETGBL	R5	G24
      0x58180040,  //  0027  LDCONST	R6	K64
      0x5C1C0200,  //  0028  MOVE	R7	R1
      0x7C140400,  //  0029  CALL	R5	2
      0xB0068205,  //  002A  RAISE	1	K65	R5
      0x80040800,  //  002B  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_value_str
********************************************************************/
be_local_closure(class_lvh_root_get_value_str,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_root,     /* shared constants */
    be_str_weak(get_value_str),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x8C040142,  //  0000  GETMET	R1	R0	K66
      0x7C040200,  //  0001  CALL	R1	1
      0x80040200,  //  0002  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_val_rule_formula
********************************************************************/
be_local_closure(class_lvh_root_get_val_rule_formula,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_root,     /* shared constants */
    be_str_weak(get_val_rule_formula),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040104,  //  0000  GETMBR	R1	R0	K4
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_delete
********************************************************************/
be_local_closure(class_lvh_root_get_delete,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 1]) {
      be_nested_proto(
        0,                          /* nstack */
        0,                          /* argc */
        0,                          /* varg */
        0,                          /* has upvals */
        NULL,                       /* no upvals */
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        0,                          /* has constants */
        NULL,                       /* no const */
        be_str_weak(_anonymous_),
        &be_const_str_solidified,
        ( &(const binstruction[ 1]) {  /* code */
          0x80000000,  //  0000  RET	0
        })
      ),
    }),
    1,                          /* has constants */
    &be_ktab_class_lvh_root,     /* shared constants */
    be_str_weak(get_delete),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x8C040143,  //  0000  GETMET	R1	R0	K67
      0x7C040200,  //  0001  CALL	R1	1
      0x84040000,  //  0002  CLOSURE	R1	P0
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_delete
********************************************************************/
be_local_closure(class_lvh_root_set_delete,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_root,     /* shared constants */
    be_str_weak(set_delete),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0xB0068945,  //  0000  RAISE	1	K68	K69
      0x80000000,  //  0001  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: lvh_root
********************************************************************/
be_local_class(lvh_root,
    14,
    NULL,
    be_nested_map(53,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(_tag, -1), be_const_var(5) },
        { be_const_key_weak(_text_rule_formula, 33), be_const_var(11) },
        { be_const_key_weak(set_val_rule_formula, -1), be_const_closure(class_lvh_root_set_val_rule_formula_closure) },
        { be_const_key_weak(set_val_rule, 0), be_const_closure(class_lvh_root_set_val_rule_closure) },
        { be_const_key_weak(parse_color, -1), be_const_static_closure(class_lvh_root_parse_color_closure) },
        { be_const_key_weak(_delete, -1), be_const_closure(class_lvh_root__delete_closure) },
        { be_const_key_weak(_lv_class, -1), be_const_nil() },
        { be_const_key_weak(get_val_rule, -1), be_const_closure(class_lvh_root_get_val_rule_closure) },
        { be_const_key_weak(get_delete, 24), be_const_closure(class_lvh_root_get_delete_closure) },
        { be_const_key_weak(_val_rule_formula, -1), be_const_var(8) },
        { be_const_key_weak(_val_rule_function, 28), be_const_var(9) },
        { be_const_key_weak(get_text_rule_formula, -1), be_const_closure(class_lvh_root_get_text_rule_formula_closure) },
        { be_const_key_weak(val_rule_matched, -1), be_const_closure(class_lvh_root_val_rule_matched_closure) },
        { be_const_key_weak(_text_rule_function, 48), be_const_var(12) },
        { be_const_key_weak(get_text, -1), be_const_closure(class_lvh_root_get_text_closure) },
        { be_const_key_weak(post_config, -1), be_const_closure(class_lvh_root_post_config_closure) },
        { be_const_key_weak(id, -1), be_const_var(0) },
        { be_const_key_weak(remove_trailing_zeroes, 27), be_const_static_closure(class_lvh_root_remove_trailing_zeroes_closure) },
        { be_const_key_weak(_val_rule, 37), be_const_var(7) },
        { be_const_key_weak(set_text_rule_format, -1), be_const_closure(class_lvh_root_set_text_rule_format_closure) },
        { be_const_key_weak(get_text_rule, -1), be_const_closure(class_lvh_root_get_text_rule_closure) },
        { be_const_key_weak(_attr_ignore, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(6,
    ( (struct bvalue*) &(const bvalue[]) {
        be_nested_str_weak(tostring),
        be_nested_str_weak(obj),
        be_nested_str_weak(page),
        be_nested_str_weak(comment),
        be_nested_str_weak(parentid),
        be_nested_str_weak(berry_run),
    }))    ) } )) },
        { be_const_key_weak(set_value_str, -1), be_const_closure(class_lvh_root_set_value_str_closure) },
        { be_const_key_weak(_text_rule_format, -1), be_const_var(13) },
        { be_const_key_weak(get_val_rule_formula, -1), be_const_closure(class_lvh_root_get_val_rule_formula_closure) },
        { be_const_key_weak(_lv_obj, -1), be_const_var(1) },
        { be_const_key_weak(digits_to_style, -1), be_const_closure(class_lvh_root_digits_to_style_closure) },
        { be_const_key_weak(remove_text_rule, -1), be_const_closure(class_lvh_root_remove_text_rule_closure) },
        { be_const_key_weak(_digit2state, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(6,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(32),
        be_const_int(33),
        be_const_int(128),
        be_const_int(160),
    }))    ) } )) },
        { be_const_key_weak(set_tag, 25), be_const_closure(class_lvh_root_set_tag_closure) },
        { be_const_key_weak(_EVENTS, -1), be_const_bytes_instance() },
        { be_const_key_weak(text_rule_matched, -1), be_const_closure(class_lvh_root_text_rule_matched_closure) },
        { be_const_key_weak(_attr_map, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(6,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(start_angle, -1), be_nested_str_weak(bg_start_angle) },
        { be_const_key_weak(h, -1), be_nested_str_weak(height) },
        { be_const_key_weak(start_angle1, 3), be_nested_str_weak(start_angle) },
        { be_const_key_weak(w, -1), be_nested_str_weak(width) },
        { be_const_key_weak(end_angle, -1), be_nested_str_weak(bg_end_angle) },
        { be_const_key_weak(end_angle1, 0), be_nested_str_weak(end_angle) },
    }))    ) } )) },
        { be_const_key_weak(_digit2part, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(10,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(131072),
        be_const_int(196608),
        be_const_int(327680),
        be_const_int(327680),
        be_const_int(262144),
        be_const_int(327680),
        be_const_int(393216),
        be_const_int(65536),
        be_const_int(524288),
    }))    ) } )) },
        { be_const_key_weak(set_text_rule, 47), be_const_closure(class_lvh_root_set_text_rule_closure) },
        { be_const_key_weak(set_meta, 18), be_const_closure(class_lvh_root_set_meta_closure) },
        { be_const_key_weak(_parent_lvh, -1), be_const_var(3) },
        { be_const_key_weak(_val, 32), be_const_var(6) },
        { be_const_key_weak(set_text, -1), be_const_closure(class_lvh_root_set_text_closure) },
        { be_const_key_weak(set_text_rule_formula, 36), be_const_closure(class_lvh_root_set_text_rule_formula_closure) },
        { be_const_key_weak(parse_font, 34), be_const_closure(class_lvh_root_parse_font_closure) },
        { be_const_key_weak(remove_val_rule, -1), be_const_closure(class_lvh_root_remove_val_rule_closure) },
        { be_const_key_weak(get_text_rule_format, -1), be_const_closure(class_lvh_root_get_text_rule_format_closure) },
        { be_const_key_weak(get_obj, -1), be_const_closure(class_lvh_root_get_obj_closure) },
        { be_const_key_weak(is_color_attribute, -1), be_const_closure(class_lvh_root_is_color_attribute_closure) },
        { be_const_key_weak(get_tag, 26), be_const_closure(class_lvh_root_get_tag_closure) },
        { be_const_key_weak(get_value_str, -1), be_const_closure(class_lvh_root_get_value_str_closure) },
        { be_const_key_weak(_text_rule, -1), be_const_var(10) },
        { be_const_key_weak(init, 15), be_const_closure(class_lvh_root_init_closure) },
        { be_const_key_weak(_meta, -1), be_const_var(4) },
        { be_const_key_weak(get_meta, 8), be_const_closure(class_lvh_root_get_meta_closure) },
        { be_const_key_weak(_page, 6), be_const_var(2) },
        { be_const_key_weak(set_delete, -1), be_const_closure(class_lvh_root_set_delete_closure) },
    })),
    be_str_weak(lvh_root)
);
// compact class 'lvh_obj' ktab size: 142, total: 288 (saved 1168 bytes)
static const bvalue be_ktab_class_lvh_obj[142] = {
  /* K0   */  be_nested_str_weak(init),
  /* K1   */  be_nested_str_weak(_lv_obj),
  /* K2   */  be_nested_str_weak(set_style_line_width),
  /* K3   */  be_nested_str_weak(_lv_part2_selector),
  /* K4   */  be_nested_str_weak(get_style_pad_right),
  /* K5   */  be_nested_str_weak(lv),
  /* K6   */  be_nested_str_weak(STATE_DEFAULT),
  /* K7   */  be_nested_str_weak(set_style_text_color),
  /* K8   */  be_nested_str_weak(parse_color),
  /* K9   */  be_nested_str_weak(remove_val_rule),
  /* K10  */  be_nested_str_weak(remove_text_rule),
  /* K11  */  be_nested_str_weak(_lv_label),
  /* K12  */  be_nested_str_weak(del),
  /* K13  */  be_nested_str_weak(_delete),
  /* K14  */  be_nested_str_weak(_page),
  /* K15  */  be_nested_str_weak(_hm),
  /* K16  */  be_nested_str_weak(_EVENTS),
  /* K17  */  be_const_int(0),
  /* K18  */  be_nested_str_weak(register_event),
  /* K19  */  be_const_int(1),
  /* K20  */  be_nested_str_weak(get_style_pad_bottom),
  /* K21  */  be_nested_str_weak(has_state),
  /* K22  */  be_nested_str_weak(STATE_DISABLED),
  /* K23  */  be_nested_str_weak(check_label),
  /* K24  */  be_nested_str_weak(set_text),
  /* K25  */  be_nested_str_weak(get_style_pad_top),
  /* K26  */  be_nested_str_weak(string),
  /* K27  */  be_nested_str_weak(introspect),
  /* K28  */  be_nested_str_weak(startswith),
  /* K29  */  be_nested_str_weak(set_),
  /* K30  */  be_nested_str_weak(get_),
  /* K31  */  be_const_int(3),
  /* K32  */  be_nested_str_weak(byte),
  /* K33  */  be_const_int(2147483647),
  /* K34  */  be_nested_str_weak(digits_to_style),
  /* K35  */  be_nested_str_weak(_attr_ignore),
  /* K36  */  be_nested_str_weak(find),
  /* K37  */  be_nested_str_weak(get),
  /* K38  */  be_nested_str_weak(function),
  /* K39  */  be_nested_str_weak(_attr_map),
  /* K40  */  be_nested_str_weak(get_style_),
  /* K41  */  be_nested_str_weak(undefined),
  /* K42  */  be_nested_str_weak(set_style_pad_left),
  /* K43  */  be_nested_str_weak(contains),
  /* K44  */  be_nested_str_weak(label),
  /* K45  */  be_nested_str_weak(get_obj),
  /* K46  */  be_nested_str_weak(set_align),
  /* K47  */  be_nested_str_weak(ALIGN_CENTER),
  /* K48  */  be_nested_str_weak(get_style_text_color),
  /* K49  */  be_nested_str_weak(register_event_cb),
  /* K50  */  be_nested_str_weak(clear_state),
  /* K51  */  be_nested_str_weak(add_state),
  /* K52  */  be_nested_str_weak(real),
  /* K53  */  be_nested_str_weak(math),
  /* K54  */  be_nested_str_weak(round),
  /* K55  */  be_nested_str_weak(endswith),
  /* K56  */  be_nested_str_weak(_X25),
  /* K57  */  be_nested_str_weak(pct),
  /* K58  */  be_nested_str_weak(is_color_attribute),
  /* K59  */  be_nested_str_weak(set_style_),
  /* K60  */  be_nested_str_weak(HSP_X3A_X20unknown_X20attribute_X3A),
  /* K61  */  be_nested_str_weak(toupper),
  /* K62  */  be_nested_str_weak(TRUE),
  /* K63  */  be_nested_str_weak(FALSE),
  /* K64  */  be_nested_str_weak(STATE_CHECKED),
  /* K65  */  be_nested_str_weak(set_style_pad_all),
  /* K66  */  be_nested_str_weak(get_x),
  /* K67  */  be_nested_str_weak(set_x),
  /* K68  */  be_nested_str_weak(_action),
  /* K69  */  be_nested_str_weak(),
  /* K70  */  be_nested_str_weak(has_flag),
  /* K71  */  be_nested_str_weak(OBJ_FLAG_FLEX_IN_NEW_TRACK),
  /* K72  */  be_nested_str_weak(add_flag),
  /* K73  */  be_nested_str_weak(clear_flag),
  /* K74  */  be_nested_str_weak(expand),
  /* K75  */  be_nested_str_weak(set_width),
  /* K76  */  be_nested_str_weak(SIZE_CONTENT),
  /* K77  */  be_nested_str_weak(break),
  /* K78  */  be_nested_str_weak(LABEL_LONG_WRAP),
  /* K79  */  be_nested_str_weak(dots),
  /* K80  */  be_nested_str_weak(LABEL_LONG_DOT),
  /* K81  */  be_nested_str_weak(scroll),
  /* K82  */  be_nested_str_weak(LABEL_LONG_SCROLL),
  /* K83  */  be_nested_str_weak(loop),
  /* K84  */  be_nested_str_weak(LABEL_LONG_SCROLL_CIRCULAR),
  /* K85  */  be_nested_str_weak(crop),
  /* K86  */  be_nested_str_weak(LABEL_LONG_CLIP),
  /* K87  */  be_nested_str_weak(set_long_mode),
  /* K88  */  be_nested_str_weak(set_style_pad_bottom),
  /* K89  */  be_nested_str_weak(_val),
  /* K90  */  be_nested_str_weak(set_value),
  /* K91  */  be_nested_str_weak(OBJ_FLAG_HIDDEN),
  /* K92  */  be_nested_str_weak(get_enabled),
  /* K93  */  be_nested_str_weak(get_text),
  /* K94  */  be_nested_str_weak(get_value_color),
  /* K95  */  be_nested_str_weak(get_long_mode),
  /* K96  */  be_nested_str_weak(OBJ_FLAG_CLICKABLE),
  /* K97  */  be_nested_str_weak(get_value),
  /* K98  */  be_nested_str_weak(parse_font),
  /* K99  */  be_nested_str_weak(set_style_text_font),
  /* K100 */  be_nested_str_weak(get_style_radius),
  /* K101 */  be_nested_str_weak(set_style_pad_top),
  /* K102 */  be_nested_str_weak(set_text_font),
  /* K103 */  be_nested_str_weak(set_enabled),
  /* K104 */  be_nested_str_weak(set_style_pad_right),
  /* K105 */  be_nested_str_weak(set_text_color),
  /* K106 */  be_nested_str_weak(get_code),
  /* K107 */  be_nested_str_weak(action),
  /* K108 */  be_nested_str_weak(EVENT_CLICKED),
  /* K109 */  be_nested_str_weak(tasmota),
  /* K110 */  be_nested_str_weak(defer),
  /* K111 */  be_nested_str_weak(_event_map),
  /* K112 */  be_nested_str_weak(json),
  /* K113 */  be_nested_str_weak(EVENT_VALUE_CHANGED),
  /* K114 */  be_nested_str_weak(val),
  /* K115 */  be_nested_str_weak(module),
  /* K116 */  be_nested_str_weak(_X2C_X22val_X22_X3A_X25s),
  /* K117 */  be_nested_str_weak(dump),
  /* K118 */  be_nested_str_weak(text),
  /* K119 */  be_nested_str_weak(_X2C_X22text_X22_X3A_X25s),
  /* K120 */  be_nested_str_weak(_tag),
  /* K121 */  be_nested_str_weak(_X2C_X22tag_X22_X3A_X25s),
  /* K122 */  be_nested_str_weak(get_sub_id),
  /* K123 */  be_nested_str_weak(_),
  /* K124 */  be_nested_str_weak(_X7B_X22hasp_X22_X3A_X7B_X22p_X25ib_X25i_X25s_X22_X3A_X7B_X22event_X22_X3A_X22_X25s_X22_X25s_X7D_X7D_X7D),
  /* K125 */  be_nested_str_weak(_page_id),
  /* K126 */  be_nested_str_weak(id),
  /* K127 */  be_nested_str_weak(get_style_line_width),
  /* K128 */  be_nested_str_weak(set_style_radius),
  /* K129 */  be_nested_str_weak(get_style_text_align),
  /* K130 */  be_nested_str_weak(TEXT_ALIGN_LEFT),
  /* K131 */  be_nested_str_weak(left),
  /* K132 */  be_nested_str_weak(TEXT_ALIGN_CENTER),
  /* K133 */  be_nested_str_weak(center),
  /* K134 */  be_nested_str_weak(TEXT_ALIGN_RIGHT),
  /* K135 */  be_nested_str_weak(right),
  /* K136 */  be_nested_str_weak(get_style_pad_left),
  /* K137 */  be_nested_str_weak(get_text_font),
  /* K138 */  be_nested_str_weak(get_y),
  /* K139 */  be_nested_str_weak(set_y),
  /* K140 */  be_const_int(2),
  /* K141 */  be_nested_str_weak(set_style_text_align),
};


extern const bclass be_class_lvh_obj;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_lvh_obj_init,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    6,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_obj,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x60180003,  //  0000  GETGBL	R6	G3
      0x5C1C0000,  //  0001  MOVE	R7	R0
      0x7C180200,  //  0002  CALL	R6	1
      0x8C180D00,  //  0003  GETMET	R6	R6	K0
      0x5C200200,  //  0004  MOVE	R8	R1
      0x5C240400,  //  0005  MOVE	R9	R2
      0x5C280600,  //  0006  MOVE	R10	R3
      0x5C2C0800,  //  0007  MOVE	R11	R4
      0x5C300A00,  //  0008  MOVE	R12	R5
      0x7C180C00,  //  0009  CALL	R6	6
      0x80000000,  //  000A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_text_font
********************************************************************/
be_local_closure(class_lvh_obj_get_text_font,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_obj,     /* shared constants */
    be_str_weak(get_text_font),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_pad_all
********************************************************************/
be_local_closure(class_lvh_obj_get_pad_all,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_obj,     /* shared constants */
    be_str_weak(get_pad_all),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_line_width
********************************************************************/
be_local_closure(class_lvh_obj_set_line_width,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_obj,     /* shared constants */
    be_str_weak(set_line_width),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x880C0101,  //  0000  GETMBR	R3	R0	K1
      0x8C0C0702,  //  0001  GETMET	R3	R3	K2
      0x60140009,  //  0002  GETGBL	R5	G9
      0x5C180200,  //  0003  MOVE	R6	R1
      0x7C140200,  //  0004  CALL	R5	1
      0x5C180400,  //  0005  MOVE	R6	R2
      0x7C0C0600,  //  0006  CALL	R3	3
      0x80000000,  //  0007  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_pad_right
********************************************************************/
be_local_closure(class_lvh_obj_get_pad_right,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_obj,     /* shared constants */
    be_str_weak(get_pad_right),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x88040103,  //  0000  GETMBR	R1	R0	K3
      0x4C080000,  //  0001  LDNIL	R2
      0x20040202,  //  0002  NE	R1	R1	R2
      0x78060007,  //  0003  JMPF	R1	#000C
      0x88040101,  //  0004  GETMBR	R1	R0	K1
      0x8C040304,  //  0005  GETMET	R1	R1	K4
      0x880C0103,  //  0006  GETMBR	R3	R0	K3
      0xB8120A00,  //  0007  GETNGBL	R4	K5
      0x88100906,  //  0008  GETMBR	R4	R4	K6
      0x300C0604,  //  0009  OR	R3	R3	R4
      0x7C040400,  //  000A  CALL	R1	2
      0x80040200,  //  000B  RET	1	R1
      0x80000000,  //  000C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_text_color
********************************************************************/
be_local_closure(class_lvh_obj_set_text_color,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_obj,     /* shared constants */
    be_str_weak(set_text_color),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x880C0101,  //  0000  GETMBR	R3	R0	K1
      0x8C0C0707,  //  0001  GETMET	R3	R3	K7
      0x8C140108,  //  0002  GETMET	R5	R0	K8
      0x5C1C0200,  //  0003  MOVE	R7	R1
      0x7C140400,  //  0004  CALL	R5	2
      0x5C180400,  //  0005  MOVE	R6	R2
      0x7C0C0600,  //  0006  CALL	R3	3
      0x80000000,  //  0007  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _delete
********************************************************************/
be_local_closure(class_lvh_obj__delete,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_obj,     /* shared constants */
    be_str_weak(_delete),
    &be_const_str_solidified,
    ( &(const binstruction[24]) {  /* code */
      0x8C040109,  //  0000  GETMET	R1	R0	K9
      0x7C040200,  //  0001  CALL	R1	1
      0x8C04010A,  //  0002  GETMET	R1	R0	K10
      0x7C040200,  //  0003  CALL	R1	1
      0x8804010B,  //  0004  GETMBR	R1	R0	K11
      0x78060004,  //  0005  JMPF	R1	#000B
      0x8804010B,  //  0006  GETMBR	R1	R0	K11
      0x8C04030C,  //  0007  GETMET	R1	R1	K12
      0x7C040200,  //  0008  CALL	R1	1
      0x4C040000,  //  0009  LDNIL	R1
      0x90021601,  //  000A  SETMBR	R0	K11	R1
      0x88040101,  //  000B  GETMBR	R1	R0	K1
      0x78060004,  //  000C  JMPF	R1	#0012
      0x88040101,  //  000D  GETMBR	R1	R0	K1
      0x8C04030C,  //  000E  GETMET	R1	R1	K12
      0x7C040200,  //  000F  CALL	R1	1
      0x4C040000,  //  0010  LDNIL	R1
      0x90020201,  //  0011  SETMBR	R0	K1	R1
      0x60040003,  //  0012  GETGBL	R1	G3
      0x5C080000,  //  0013  MOVE	R2	R0
      0x7C040200,  //  0014  CALL	R1	1
      0x8C04030D,  //  0015  GETMET	R1	R1	K13
      0x7C040200,  //  0016  CALL	R1	1
      0x80000000,  //  0017  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: register_event_cb
********************************************************************/
be_local_closure(class_lvh_obj_register_event_cb,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_obj,     /* shared constants */
    be_str_weak(register_event_cb),
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0x8804010E,  //  0000  GETMBR	R1	R0	K14
      0x8804030F,  //  0001  GETMBR	R1	R1	K15
      0x88080110,  //  0002  GETMBR	R2	R0	K16
      0x580C0011,  //  0003  LDCONST	R3	K17
      0x6010000C,  //  0004  GETGBL	R4	G12
      0x5C140400,  //  0005  MOVE	R5	R2
      0x7C100200,  //  0006  CALL	R4	1
      0x14100604,  //  0007  LT	R4	R3	R4
      0x78120005,  //  0008  JMPF	R4	#000F
      0x8C100312,  //  0009  GETMET	R4	R1	K18
      0x5C180000,  //  000A  MOVE	R6	R0
      0x941C0403,  //  000B  GETIDX	R7	R2	R3
      0x7C100600,  //  000C  CALL	R4	3
      0x000C0713,  //  000D  ADD	R3	R3	K19
      0x7001FFF4,  //  000E  JMP		#0004
      0x80000000,  //  000F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_pad_bottom
********************************************************************/
be_local_closure(class_lvh_obj_get_pad_bottom,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_obj,     /* shared constants */
    be_str_weak(get_pad_bottom),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x88040103,  //  0000  GETMBR	R1	R0	K3
      0x4C080000,  //  0001  LDNIL	R2
      0x20040202,  //  0002  NE	R1	R1	R2
      0x78060007,  //  0003  JMPF	R1	#000C
      0x88040101,  //  0004  GETMBR	R1	R0	K1
      0x8C040314,  //  0005  GETMET	R1	R1	K20
      0x880C0103,  //  0006  GETMBR	R3	R0	K3
      0xB8120A00,  //  0007  GETNGBL	R4	K5
      0x88100906,  //  0008  GETMBR	R4	R4	K6
      0x300C0604,  //  0009  OR	R3	R3	R4
      0x7C040400,  //  000A  CALL	R1	2
      0x80040200,  //  000B  RET	1	R1
      0x80000000,  //  000C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_enabled
********************************************************************/
be_local_closure(class_lvh_obj_get_enabled,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_obj,     /* shared constants */
    be_str_weak(get_enabled),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x88040101,  //  0000  GETMBR	R1	R0	K1
      0x8C040315,  //  0001  GETMET	R1	R1	K21
      0xB80E0A00,  //  0002  GETNGBL	R3	K5
      0x880C0716,  //  0003  GETMBR	R3	R3	K22
      0x7C040400,  //  0004  CALL	R1	2
      0x78060000,  //  0005  JMPF	R1	#0007
      0x50040001,  //  0006  LDBOOL	R1	0	1
      0x50040200,  //  0007  LDBOOL	R1	1	0
      0x80040200,  //  0008  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_text
********************************************************************/
be_local_closure(class_lvh_obj_set_text,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_obj,     /* shared constants */
    be_str_weak(set_text),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x8C080117,  //  0000  GETMET	R2	R0	K23
      0x7C080200,  //  0001  CALL	R2	1
      0x8808010B,  //  0002  GETMBR	R2	R0	K11
      0x8C080518,  //  0003  GETMET	R2	R2	K24
      0x60100008,  //  0004  GETGBL	R4	G8
      0x5C140200,  //  0005  MOVE	R5	R1
      0x7C100200,  //  0006  CALL	R4	1
      0x7C080400,  //  0007  CALL	R2	2
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_pad_top
********************************************************************/
be_local_closure(class_lvh_obj_get_pad_top,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_obj,     /* shared constants */
    be_str_weak(get_pad_top),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x88040103,  //  0000  GETMBR	R1	R0	K3
      0x4C080000,  //  0001  LDNIL	R2
      0x20040202,  //  0002  NE	R1	R1	R2
      0x78060007,  //  0003  JMPF	R1	#000C
      0x88040101,  //  0004  GETMBR	R1	R0	K1
      0x8C040319,  //  0005  GETMET	R1	R1	K25
      0x880C0103,  //  0006  GETMBR	R3	R0	K3
      0xB8120A00,  //  0007  GETNGBL	R4	K5
      0x88100906,  //  0008  GETMBR	R4	R4	K6
      0x300C0604,  //  0009  OR	R3	R3	R4
      0x7C040400,  //  000A  CALL	R1	2
      0x80040200,  //  000B  RET	1	R1
      0x80000000,  //  000C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: member
********************************************************************/
be_local_closure(class_lvh_obj_member,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_obj,     /* shared constants */
    be_str_weak(member),
    &be_const_str_solidified,
    ( &(const binstruction[125]) {  /* code */
      0xA40A3400,  //  0000  IMPORT	R2	K26
      0xA40E3600,  //  0001  IMPORT	R3	K27
      0x8C10051C,  //  0002  GETMET	R4	R2	K28
      0x5C180200,  //  0003  MOVE	R6	R1
      0x581C001D,  //  0004  LDCONST	R7	K29
      0x7C100600,  //  0005  CALL	R4	3
      0x74120004,  //  0006  JMPT	R4	#000C
      0x8C10051C,  //  0007  GETMET	R4	R2	K28
      0x5C180200,  //  0008  MOVE	R6	R1
      0x581C001E,  //  0009  LDCONST	R7	K30
      0x7C100600,  //  000A  CALL	R4	3
      0x78120000,  //  000B  JMPF	R4	#000D
      0x80000800,  //  000C  RET	0
      0x4C100000,  //  000D  LDNIL	R4
      0x6014000C,  //  000E  GETGBL	R5	G12
      0x5C180200,  //  000F  MOVE	R6	R1
      0x7C140200,  //  0010  CALL	R5	1
      0x28140B1F,  //  0011  GE	R5	R5	K31
      0x7816001F,  //  0012  JMPF	R5	#0033
      0x8C140520,  //  0013  GETMET	R5	R2	K32
      0x541DFFFE,  //  0014  LDINT	R7	-1
      0x941C0207,  //  0015  GETIDX	R7	R1	R7
      0x7C140400,  //  0016  CALL	R5	2
      0x8C180520,  //  0017  GETMET	R6	R2	K32
      0x5421FFFD,  //  0018  LDINT	R8	-2
      0x94200208,  //  0019  GETIDX	R8	R1	R8
      0x7C180400,  //  001A  CALL	R6	2
      0x541E002F,  //  001B  LDINT	R7	48
      0x281C0A07,  //  001C  GE	R7	R5	R7
      0x781E0014,  //  001D  JMPF	R7	#0033
      0x541E0038,  //  001E  LDINT	R7	57
      0x181C0A07,  //  001F  LE	R7	R5	R7
      0x781E0011,  //  0020  JMPF	R7	#0033
      0x541E002F,  //  0021  LDINT	R7	48
      0x281C0C07,  //  0022  GE	R7	R6	R7
      0x781E000E,  //  0023  JMPF	R7	#0033
      0x541E0038,  //  0024  LDINT	R7	57
      0x181C0C07,  //  0025  LE	R7	R6	R7
      0x781E000B,  //  0026  JMPF	R7	#0033
      0x601C0009,  //  0027  GETGBL	R7	G9
      0x5421FFFD,  //  0028  LDINT	R8	-2
      0x40201121,  //  0029  CONNECT	R8	R8	K33
      0x94200208,  //  002A  GETIDX	R8	R1	R8
      0x7C1C0200,  //  002B  CALL	R7	1
      0x5421FFFC,  //  002C  LDINT	R8	-3
      0x40222208,  //  002D  CONNECT	R8	K17	R8
      0x94040208,  //  002E  GETIDX	R1	R1	R8
      0x8C200122,  //  002F  GETMET	R8	R0	K34
      0x5C280E00,  //  0030  MOVE	R10	R7
      0x7C200400,  //  0031  CALL	R8	2
      0x5C101000,  //  0032  MOVE	R4	R8
      0x88140123,  //  0033  GETMBR	R5	R0	K35
      0x8C140B24,  //  0034  GETMET	R5	R5	K36
      0x5C1C0200,  //  0035  MOVE	R7	R1
      0x7C140400,  //  0036  CALL	R5	2
      0x4C180000,  //  0037  LDNIL	R6
      0x20140A06,  //  0038  NE	R5	R5	R6
      0x78160000,  //  0039  JMPF	R5	#003B
      0x80000A00,  //  003A  RET	0
      0x8C140725,  //  003B  GETMET	R5	R3	K37
      0x5C1C0000,  //  003C  MOVE	R7	R0
      0x00223C01,  //  003D  ADD	R8	K30	R1
      0x7C140600,  //  003E  CALL	R5	3
      0x60180004,  //  003F  GETGBL	R6	G4
      0x5C1C0A00,  //  0040  MOVE	R7	R5
      0x7C180200,  //  0041  CALL	R6	1
      0x1C180D26,  //  0042  EQ	R6	R6	K38
      0x781A0009,  //  0043  JMPF	R6	#004E
      0x5C180A00,  //  0044  MOVE	R6	R5
      0x5C1C0000,  //  0045  MOVE	R7	R0
      0x4C200000,  //  0046  LDNIL	R8
      0x20200808,  //  0047  NE	R8	R4	R8
      0x78220001,  //  0048  JMPF	R8	#004B
      0x5C200800,  //  0049  MOVE	R8	R4
      0x70020000,  //  004A  JMP		#004C
      0x58200011,  //  004B  LDCONST	R8	K17
      0x7C180400,  //  004C  CALL	R6	2
      0x80040C00,  //  004D  RET	1	R6
      0x88180127,  //  004E  GETMBR	R6	R0	K39
      0x8C180D24,  //  004F  GETMET	R6	R6	K36
      0x5C200200,  //  0050  MOVE	R8	R1
      0x5C240200,  //  0051  MOVE	R9	R1
      0x7C180600,  //  0052  CALL	R6	3
      0x5C040C00,  //  0053  MOVE	R1	R6
      0x4C180000,  //  0054  LDNIL	R6
      0x1C180806,  //  0055  EQ	R6	R4	R6
      0x781A000D,  //  0056  JMPF	R6	#0065
      0x8C180725,  //  0057  GETMET	R6	R3	K37
      0x88200101,  //  0058  GETMBR	R8	R0	K1
      0x00263C01,  //  0059  ADD	R9	K30	R1
      0x7C180600,  //  005A  CALL	R6	3
      0x5C140C00,  //  005B  MOVE	R5	R6
      0x60180004,  //  005C  GETGBL	R6	G4
      0x5C1C0A00,  //  005D  MOVE	R7	R5
      0x7C180200,  //  005E  CALL	R6	1
      0x1C180D26,  //  005F  EQ	R6	R6	K38
      0x781A0003,  //  0060  JMPF	R6	#0065
      0x5C180A00,  //  0061  MOVE	R6	R5
      0x881C0101,  //  0062  GETMBR	R7	R0	K1
      0x7C180200,  //  0063  CALL	R6	1
      0x80040C00,  //  0064  RET	1	R6
      0x8C180725,  //  0065  GETMET	R6	R3	K37
      0x88200101,  //  0066  GETMBR	R8	R0	K1
      0x00265001,  //  0067  ADD	R9	K40	R1
      0x7C180600,  //  0068  CALL	R6	3
      0x5C140C00,  //  0069  MOVE	R5	R6
      0x60180004,  //  006A  GETGBL	R6	G4
      0x5C1C0A00,  //  006B  MOVE	R7	R5
      0x7C180200,  //  006C  CALL	R6	1
      0x1C180D26,  //  006D  EQ	R6	R6	K38
      0x781A0009,  //  006E  JMPF	R6	#0079
      0x5C180A00,  //  006F  MOVE	R6	R5
      0x881C0101,  //  0070  GETMBR	R7	R0	K1
      0x4C200000,  //  0071  LDNIL	R8
      0x20200808,  //  0072  NE	R8	R4	R8
      0x78220001,  //  0073  JMPF	R8	#0076
      0x5C200800,  //  0074  MOVE	R8	R4
      0x70020000,  //  0075  JMP		#0077
      0x58200011,  //  0076  LDCONST	R8	K17
      0x7C180400,  //  0077  CALL	R6	2
      0x80040C00,  //  0078  RET	1	R6
      0x6018000B,  //  0079  GETGBL	R6	G11
      0x581C0029,  //  007A  LDCONST	R7	K41
      0x7C180200,  //  007B  CALL	R6	1
      0x80040C00,  //  007C  RET	1	R6
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_pad_left2
********************************************************************/
be_local_closure(class_lvh_obj_set_pad_left2,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_obj,     /* shared constants */
    be_str_weak(set_pad_left2),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x88080103,  //  0000  GETMBR	R2	R0	K3
      0x4C0C0000,  //  0001  LDNIL	R3
      0x20080403,  //  0002  NE	R2	R2	R3
      0x780A0009,  //  0003  JMPF	R2	#000E
      0x88080101,  //  0004  GETMBR	R2	R0	K1
      0x8C08052A,  //  0005  GETMET	R2	R2	K42
      0x60100009,  //  0006  GETGBL	R4	G9
      0x5C140200,  //  0007  MOVE	R5	R1
      0x7C100200,  //  0008  CALL	R4	1
      0x88140103,  //  0009  GETMBR	R5	R0	K3
      0xB81A0A00,  //  000A  GETNGBL	R6	K5
      0x88180D06,  //  000B  GETMBR	R6	R6	K6
      0x30140A06,  //  000C  OR	R5	R5	R6
      0x7C080600,  //  000D  CALL	R2	3
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: check_label
********************************************************************/
be_local_closure(class_lvh_obj_check_label,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_obj,     /* shared constants */
    be_str_weak(check_label),
    &be_const_str_solidified,
    ( &(const binstruction[25]) {  /* code */
      0x8804010B,  //  0000  GETMBR	R1	R0	K11
      0x4C080000,  //  0001  LDNIL	R2
      0x1C040202,  //  0002  EQ	R1	R1	R2
      0x78060013,  //  0003  JMPF	R1	#0018
      0xA4063600,  //  0004  IMPORT	R1	K27
      0x8C08032B,  //  0005  GETMET	R2	R1	K43
      0x88100101,  //  0006  GETMBR	R4	R0	K1
      0x58140018,  //  0007  LDCONST	R5	K24
      0x7C080600,  //  0008  CALL	R2	3
      0x780A0002,  //  0009  JMPF	R2	#000D
      0x88080101,  //  000A  GETMBR	R2	R0	K1
      0x90021602,  //  000B  SETMBR	R0	K11	R2
      0x7002000A,  //  000C  JMP		#0018
      0xB80A0A00,  //  000D  GETNGBL	R2	K5
      0x8C08052C,  //  000E  GETMET	R2	R2	K44
      0x8C10012D,  //  000F  GETMET	R4	R0	K45
      0x7C100200,  //  0010  CALL	R4	1
      0x7C080400,  //  0011  CALL	R2	2
      0x90021602,  //  0012  SETMBR	R0	K11	R2
      0x8808010B,  //  0013  GETMBR	R2	R0	K11
      0x8C08052E,  //  0014  GETMET	R2	R2	K46
      0xB8120A00,  //  0015  GETNGBL	R4	K5
      0x8810092F,  //  0016  GETMBR	R4	R4	K47
      0x7C080400,  //  0017  CALL	R2	2
      0x80000000,  //  0018  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_text_color
********************************************************************/
be_local_closure(class_lvh_obj_get_text_color,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_obj,     /* shared constants */
    be_str_weak(get_text_color),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x88080101,  //  0000  GETMBR	R2	R0	K1
      0x8C080530,  //  0001  GETMET	R2	R2	K48
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x80040400,  //  0004  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: post_init
********************************************************************/
be_local_closure(class_lvh_obj_post_init,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_obj,     /* shared constants */
    be_str_weak(post_init),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x8C080131,  //  0000  GETMET	R2	R0	K49
      0x7C080200,  //  0001  CALL	R2	1
      0x80000000,  //  0002  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_enabled
********************************************************************/
be_local_closure(class_lvh_obj_set_enabled,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_obj,     /* shared constants */
    be_str_weak(set_enabled),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x78060005,  //  0000  JMPF	R1	#0007
      0x88080101,  //  0001  GETMBR	R2	R0	K1
      0x8C080532,  //  0002  GETMET	R2	R2	K50
      0xB8120A00,  //  0003  GETNGBL	R4	K5
      0x88100916,  //  0004  GETMBR	R4	R4	K22
      0x7C080400,  //  0005  CALL	R2	2
      0x70020004,  //  0006  JMP		#000C
      0x88080101,  //  0007  GETMBR	R2	R0	K1
      0x8C080533,  //  0008  GETMET	R2	R2	K51
      0xB8120A00,  //  0009  GETNGBL	R4	K5
      0x88100916,  //  000A  GETMBR	R4	R4	K22
      0x7C080400,  //  000B  CALL	R2	2
      0x80000000,  //  000C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: setmember
********************************************************************/
be_local_closure(class_lvh_obj_setmember,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_obj,     /* shared constants */
    be_str_weak(setmember),
    &be_const_str_solidified,
    ( &(const binstruction[164]) {  /* code */
      0xA40E3400,  //  0000  IMPORT	R3	K26
      0xA4123600,  //  0001  IMPORT	R4	K27
      0x8C14071C,  //  0002  GETMET	R5	R3	K28
      0x5C1C0200,  //  0003  MOVE	R7	R1
      0x5820001D,  //  0004  LDCONST	R8	K29
      0x7C140600,  //  0005  CALL	R5	3
      0x74160004,  //  0006  JMPT	R5	#000C
      0x8C14071C,  //  0007  GETMET	R5	R3	K28
      0x5C1C0200,  //  0008  MOVE	R7	R1
      0x5820001E,  //  0009  LDCONST	R8	K30
      0x7C140600,  //  000A  CALL	R5	3
      0x78160000,  //  000B  JMPF	R5	#000D
      0x80000A00,  //  000C  RET	0
      0x60140004,  //  000D  GETGBL	R5	G4
      0x5C180400,  //  000E  MOVE	R6	R2
      0x7C140200,  //  000F  CALL	R5	1
      0x1C140B34,  //  0010  EQ	R5	R5	K52
      0x78160006,  //  0011  JMPF	R5	#0019
      0xA4166A00,  //  0012  IMPORT	R5	K53
      0x60180009,  //  0013  GETGBL	R6	G9
      0x8C1C0B36,  //  0014  GETMET	R7	R5	K54
      0x5C240400,  //  0015  MOVE	R9	R2
      0x7C1C0400,  //  0016  CALL	R7	2
      0x7C180200,  //  0017  CALL	R6	1
      0x5C080C00,  //  0018  MOVE	R2	R6
      0x8C140737,  //  0019  GETMET	R5	R3	K55
      0x5C1C0200,  //  001A  MOVE	R7	R1
      0x58200038,  //  001B  LDCONST	R8	K56
      0x7C140600,  //  001C  CALL	R5	3
      0x78160009,  //  001D  JMPF	R5	#0028
      0x5415FFFD,  //  001E  LDINT	R5	-2
      0x40162205,  //  001F  CONNECT	R5	K17	R5
      0x94040205,  //  0020  GETIDX	R1	R1	R5
      0xB8160A00,  //  0021  GETNGBL	R5	K5
      0x8C140B39,  //  0022  GETMET	R5	R5	K57
      0x601C0009,  //  0023  GETGBL	R7	G9
      0x5C200400,  //  0024  MOVE	R8	R2
      0x7C1C0200,  //  0025  CALL	R7	1
      0x7C140400,  //  0026  CALL	R5	2
      0x5C080A00,  //  0027  MOVE	R2	R5
      0x4C140000,  //  0028  LDNIL	R5
      0x6018000C,  //  0029  GETGBL	R6	G12
      0x5C1C0200,  //  002A  MOVE	R7	R1
      0x7C180200,  //  002B  CALL	R6	1
      0x28180D1F,  //  002C  GE	R6	R6	K31
      0x781A001F,  //  002D  JMPF	R6	#004E
      0x8C180720,  //  002E  GETMET	R6	R3	K32
      0x5421FFFE,  //  002F  LDINT	R8	-1
      0x94200208,  //  0030  GETIDX	R8	R1	R8
      0x7C180400,  //  0031  CALL	R6	2
      0x8C1C0720,  //  0032  GETMET	R7	R3	K32
      0x5425FFFD,  //  0033  LDINT	R9	-2
      0x94240209,  //  0034  GETIDX	R9	R1	R9
      0x7C1C0400,  //  0035  CALL	R7	2
      0x5422002F,  //  0036  LDINT	R8	48
      0x28200C08,  //  0037  GE	R8	R6	R8
      0x78220014,  //  0038  JMPF	R8	#004E
      0x54220038,  //  0039  LDINT	R8	57
      0x18200C08,  //  003A  LE	R8	R6	R8
      0x78220011,  //  003B  JMPF	R8	#004E
      0x5422002F,  //  003C  LDINT	R8	48
      0x28200E08,  //  003D  GE	R8	R7	R8
      0x7822000E,  //  003E  JMPF	R8	#004E
      0x54220038,  //  003F  LDINT	R8	57
      0x18200E08,  //  0040  LE	R8	R7	R8
      0x7822000B,  //  0041  JMPF	R8	#004E
      0x60200009,  //  0042  GETGBL	R8	G9
      0x5425FFFD,  //  0043  LDINT	R9	-2
      0x40241321,  //  0044  CONNECT	R9	R9	K33
      0x94240209,  //  0045  GETIDX	R9	R1	R9
      0x7C200200,  //  0046  CALL	R8	1
      0x5425FFFC,  //  0047  LDINT	R9	-3
      0x40262209,  //  0048  CONNECT	R9	K17	R9
      0x94040209,  //  0049  GETIDX	R1	R1	R9
      0x8C240122,  //  004A  GETMET	R9	R0	K34
      0x5C2C1000,  //  004B  MOVE	R11	R8
      0x7C240400,  //  004C  CALL	R9	2
      0x5C141200,  //  004D  MOVE	R5	R9
      0x88180123,  //  004E  GETMBR	R6	R0	K35
      0x8C180D24,  //  004F  GETMET	R6	R6	K36
      0x5C200200,  //  0050  MOVE	R8	R1
      0x7C180400,  //  0051  CALL	R6	2
      0x4C1C0000,  //  0052  LDNIL	R7
      0x20180C07,  //  0053  NE	R6	R6	R7
      0x781A0000,  //  0054  JMPF	R6	#0056
      0x80000C00,  //  0055  RET	0
      0x8C180925,  //  0056  GETMET	R6	R4	K37
      0x5C200000,  //  0057  MOVE	R8	R0
      0x00263A01,  //  0058  ADD	R9	K29	R1
      0x7C180600,  //  0059  CALL	R6	3
      0x601C0004,  //  005A  GETGBL	R7	G4
      0x5C200C00,  //  005B  MOVE	R8	R6
      0x7C1C0200,  //  005C  CALL	R7	1
      0x1C1C0F26,  //  005D  EQ	R7	R7	K38
      0x781E000A,  //  005E  JMPF	R7	#006A
      0x5C1C0C00,  //  005F  MOVE	R7	R6
      0x5C200000,  //  0060  MOVE	R8	R0
      0x5C240400,  //  0061  MOVE	R9	R2
      0x4C280000,  //  0062  LDNIL	R10
      0x20280A0A,  //  0063  NE	R10	R5	R10
      0x782A0001,  //  0064  JMPF	R10	#0067
      0x5C280A00,  //  0065  MOVE	R10	R5
      0x70020000,  //  0066  JMP		#0068
      0x58280011,  //  0067  LDCONST	R10	K17
      0x7C1C0600,  //  0068  CALL	R7	3
      0x80000E00,  //  0069  RET	0
      0x881C0127,  //  006A  GETMBR	R7	R0	K39
      0x8C1C0F24,  //  006B  GETMET	R7	R7	K36
      0x5C240200,  //  006C  MOVE	R9	R1
      0x5C280200,  //  006D  MOVE	R10	R1
      0x7C1C0600,  //  006E  CALL	R7	3
      0x5C040E00,  //  006F  MOVE	R1	R7
      0x8C1C013A,  //  0070  GETMET	R7	R0	K58
      0x5C240200,  //  0071  MOVE	R9	R1
      0x7C1C0400,  //  0072  CALL	R7	2
      0x781E0003,  //  0073  JMPF	R7	#0078
      0x8C1C0108,  //  0074  GETMET	R7	R0	K8
      0x5C240400,  //  0075  MOVE	R9	R2
      0x7C1C0400,  //  0076  CALL	R7	2
      0x5C080E00,  //  0077  MOVE	R2	R7
      0x4C1C0000,  //  0078  LDNIL	R7
      0x1C1C0A07,  //  0079  EQ	R7	R5	R7
      0x781E000E,  //  007A  JMPF	R7	#008A
      0x8C1C0925,  //  007B  GETMET	R7	R4	K37
      0x88240101,  //  007C  GETMBR	R9	R0	K1
      0x002A3A01,  //  007D  ADD	R10	K29	R1
      0x7C1C0600,  //  007E  CALL	R7	3
      0x5C180E00,  //  007F  MOVE	R6	R7
      0x601C0004,  //  0080  GETGBL	R7	G4
      0x5C200C00,  //  0081  MOVE	R8	R6
      0x7C1C0200,  //  0082  CALL	R7	1
      0x1C1C0F26,  //  0083  EQ	R7	R7	K38
      0x781E0004,  //  0084  JMPF	R7	#008A
      0x5C1C0C00,  //  0085  MOVE	R7	R6
      0x88200101,  //  0086  GETMBR	R8	R0	K1
      0x5C240400,  //  0087  MOVE	R9	R2
      0x7C1C0400,  //  0088  CALL	R7	2
      0x80040E00,  //  0089  RET	1	R7
      0x8C1C0925,  //  008A  GETMET	R7	R4	K37
      0x88240101,  //  008B  GETMBR	R9	R0	K1
      0x002A7601,  //  008C  ADD	R10	K59	R1
      0x7C1C0600,  //  008D  CALL	R7	3
      0x5C180E00,  //  008E  MOVE	R6	R7
      0x601C0004,  //  008F  GETGBL	R7	G4
      0x5C200C00,  //  0090  MOVE	R8	R6
      0x7C1C0200,  //  0091  CALL	R7	1
      0x1C1C0F26,  //  0092  EQ	R7	R7	K38
      0x781E000A,  //  0093  JMPF	R7	#009F
      0x5C1C0C00,  //  0094  MOVE	R7	R6
      0x88200101,  //  0095  GETMBR	R8	R0	K1
      0x5C240400,  //  0096  MOVE	R9	R2
      0x4C280000,  //  0097  LDNIL	R10
      0x20280A0A,  //  0098  NE	R10	R5	R10
      0x782A0001,  //  0099  JMPF	R10	#009C
      0x5C280A00,  //  009A  MOVE	R10	R5
      0x70020000,  //  009B  JMP		#009D
      0x58280011,  //  009C  LDCONST	R10	K17
      0x7C1C0600,  //  009D  CALL	R7	3
      0x80040E00,  //  009E  RET	1	R7
      0x601C0001,  //  009F  GETGBL	R7	G1
      0x5820003C,  //  00A0  LDCONST	R8	K60
      0x5C240200,  //  00A1  MOVE	R9	R1
      0x7C1C0400,  //  00A2  CALL	R7	2
      0x80000000,  //  00A3  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_toggle
********************************************************************/
be_local_closure(class_lvh_obj_set_toggle,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_obj,     /* shared constants */
    be_str_weak(set_toggle),
    &be_const_str_solidified,
    ( &(const binstruction[32]) {  /* code */
      0xA40A3400,  //  0000  IMPORT	R2	K26
      0x600C0004,  //  0001  GETGBL	R3	G4
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C0C0200,  //  0003  CALL	R3	1
      0x1C0C071A,  //  0004  EQ	R3	R3	K26
      0x780E000C,  //  0005  JMPF	R3	#0013
      0x8C0C053D,  //  0006  GETMET	R3	R2	K61
      0x60140008,  //  0007  GETGBL	R5	G8
      0x5C180200,  //  0008  MOVE	R6	R1
      0x7C140200,  //  0009  CALL	R5	1
      0x7C0C0400,  //  000A  CALL	R3	2
      0x5C040600,  //  000B  MOVE	R1	R3
      0x1C0C033E,  //  000C  EQ	R3	R1	K62
      0x780E0001,  //  000D  JMPF	R3	#0010
      0x50040200,  //  000E  LDBOOL	R1	1	0
      0x70020002,  //  000F  JMP		#0013
      0x1C0C033F,  //  0010  EQ	R3	R1	K63
      0x780E0000,  //  0011  JMPF	R3	#0013
      0x50040000,  //  0012  LDBOOL	R1	0	0
      0x78060005,  //  0013  JMPF	R1	#001A
      0x880C0101,  //  0014  GETMBR	R3	R0	K1
      0x8C0C0733,  //  0015  GETMET	R3	R3	K51
      0xB8160A00,  //  0016  GETNGBL	R5	K5
      0x88140B40,  //  0017  GETMBR	R5	R5	K64
      0x7C0C0400,  //  0018  CALL	R3	2
      0x70020004,  //  0019  JMP		#001F
      0x880C0101,  //  001A  GETMBR	R3	R0	K1
      0x8C0C0732,  //  001B  GETMET	R3	R3	K50
      0xB8160A00,  //  001C  GETNGBL	R5	K5
      0x88140B40,  //  001D  GETMBR	R5	R5	K64
      0x7C0C0400,  //  001E  CALL	R3	2
      0x80000000,  //  001F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_pad_all2
********************************************************************/
be_local_closure(class_lvh_obj_set_pad_all2,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_obj,     /* shared constants */
    be_str_weak(set_pad_all2),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x88080103,  //  0000  GETMBR	R2	R0	K3
      0x4C0C0000,  //  0001  LDNIL	R3
      0x20080403,  //  0002  NE	R2	R2	R3
      0x780A0009,  //  0003  JMPF	R2	#000E
      0x88080101,  //  0004  GETMBR	R2	R0	K1
      0x8C080541,  //  0005  GETMET	R2	R2	K65
      0x60100009,  //  0006  GETGBL	R4	G9
      0x5C140200,  //  0007  MOVE	R5	R1
      0x7C100200,  //  0008  CALL	R4	1
      0x88140103,  //  0009  GETMBR	R5	R0	K3
      0xB81A0A00,  //  000A  GETNGBL	R6	K5
      0x88180D06,  //  000B  GETMBR	R6	R6	K6
      0x30140A06,  //  000C  OR	R5	R5	R6
      0x7C080600,  //  000D  CALL	R2	3
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_value_ofs_x
********************************************************************/
be_local_closure(class_lvh_obj_get_value_ofs_x,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_obj,     /* shared constants */
    be_str_weak(get_value_ofs_x),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x8804010B,  //  0000  GETMBR	R1	R0	K11
      0x8C040342,  //  0001  GETMET	R1	R1	K66
      0x7C040200,  //  0002  CALL	R1	1
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_value_ofs_x
********************************************************************/
be_local_closure(class_lvh_obj_set_value_ofs_x,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_obj,     /* shared constants */
    be_str_weak(set_value_ofs_x),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x8C080117,  //  0000  GETMET	R2	R0	K23
      0x7C080200,  //  0001  CALL	R2	1
      0x8808010B,  //  0002  GETMBR	R2	R0	K11
      0x8C080543,  //  0003  GETMET	R2	R2	K67
      0x60100009,  //  0004  GETGBL	R4	G9
      0x5C140200,  //  0005  MOVE	R5	R1
      0x7C100200,  //  0006  CALL	R4	1
      0x7C080400,  //  0007  CALL	R2	2
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_action
********************************************************************/
be_local_closure(class_lvh_obj_set_action,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_obj,     /* shared constants */
    be_str_weak(set_action),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x60080008,  //  0000  GETGBL	R2	G8
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x7C080200,  //  0002  CALL	R2	1
      0x90028802,  //  0003  SETMBR	R0	K68	R2
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_action
********************************************************************/
be_local_closure(class_lvh_obj_get_action,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_obj,     /* shared constants */
    be_str_weak(get_action),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x88040144,  //  0000  GETMBR	R1	R0	K68
      0x78060001,  //  0001  JMPF	R1	#0004
      0x5C080200,  //  0002  MOVE	R2	R1
      0x70020000,  //  0003  JMP		#0005
      0x58080045,  //  0004  LDCONST	R2	K69
      0x80040400,  //  0005  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_flex_in_new_track
********************************************************************/
be_local_closure(class_lvh_obj_get_flex_in_new_track,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_obj,     /* shared constants */
    be_str_weak(get_flex_in_new_track),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x88040101,  //  0000  GETMBR	R1	R0	K1
      0x8C040346,  //  0001  GETMET	R1	R1	K70
      0xB80E0A00,  //  0002  GETNGBL	R3	K5
      0x880C0747,  //  0003  GETMBR	R3	R3	K71
      0x7C040400,  //  0004  CALL	R1	2
      0x80040200,  //  0005  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_flex_in_new_track
********************************************************************/
be_local_closure(class_lvh_obj_set_flex_in_new_track,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_obj,     /* shared constants */
    be_str_weak(set_flex_in_new_track),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x78060005,  //  0000  JMPF	R1	#0007
      0x88080101,  //  0001  GETMBR	R2	R0	K1
      0x8C080548,  //  0002  GETMET	R2	R2	K72
      0xB8120A00,  //  0003  GETNGBL	R4	K5
      0x88100947,  //  0004  GETMBR	R4	R4	K71
      0x7C080400,  //  0005  CALL	R2	2
      0x70020004,  //  0006  JMP		#000C
      0x88080101,  //  0007  GETMBR	R2	R0	K1
      0x8C080549,  //  0008  GETMET	R2	R2	K73
      0xB8120A00,  //  0009  GETNGBL	R4	K5
      0x88100947,  //  000A  GETMBR	R4	R4	K71
      0x7C080400,  //  000B  CALL	R2	2
      0x80000000,  //  000C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_label_mode
********************************************************************/
be_local_closure(class_lvh_obj_set_label_mode,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_obj,     /* shared constants */
    be_str_weak(set_label_mode),
    &be_const_str_solidified,
    ( &(const binstruction[43]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C0C034A,  //  0001  EQ	R3	R1	K74
      0x780E0005,  //  0002  JMPF	R3	#0009
      0x880C0101,  //  0003  GETMBR	R3	R0	K1
      0x8C0C074B,  //  0004  GETMET	R3	R3	K75
      0xB8160A00,  //  0005  GETNGBL	R5	K5
      0x88140B4C,  //  0006  GETMBR	R5	R5	K76
      0x7C0C0400,  //  0007  CALL	R3	2
      0x70020017,  //  0008  JMP		#0021
      0x1C0C034D,  //  0009  EQ	R3	R1	K77
      0x780E0002,  //  000A  JMPF	R3	#000E
      0xB80E0A00,  //  000B  GETNGBL	R3	K5
      0x8808074E,  //  000C  GETMBR	R2	R3	K78
      0x70020012,  //  000D  JMP		#0021
      0x1C0C034F,  //  000E  EQ	R3	R1	K79
      0x780E0002,  //  000F  JMPF	R3	#0013
      0xB80E0A00,  //  0010  GETNGBL	R3	K5
      0x88080750,  //  0011  GETMBR	R2	R3	K80
      0x7002000D,  //  0012  JMP		#0021
      0x1C0C0351,  //  0013  EQ	R3	R1	K81
      0x780E0002,  //  0014  JMPF	R3	#0018
      0xB80E0A00,  //  0015  GETNGBL	R3	K5
      0x88080752,  //  0016  GETMBR	R2	R3	K82
      0x70020008,  //  0017  JMP		#0021
      0x1C0C0353,  //  0018  EQ	R3	R1	K83
      0x780E0002,  //  0019  JMPF	R3	#001D
      0xB80E0A00,  //  001A  GETNGBL	R3	K5
      0x88080754,  //  001B  GETMBR	R2	R3	K84
      0x70020003,  //  001C  JMP		#0021
      0x1C0C0355,  //  001D  EQ	R3	R1	K85
      0x780E0001,  //  001E  JMPF	R3	#0021
      0xB80E0A00,  //  001F  GETNGBL	R3	K5
      0x88080756,  //  0020  GETMBR	R2	R3	K86
      0x4C0C0000,  //  0021  LDNIL	R3
      0x200C0403,  //  0022  NE	R3	R2	R3
      0x780E0005,  //  0023  JMPF	R3	#002A
      0x8C0C0117,  //  0024  GETMET	R3	R0	K23
      0x7C0C0200,  //  0025  CALL	R3	1
      0x880C010B,  //  0026  GETMBR	R3	R0	K11
      0x8C0C0757,  //  0027  GETMET	R3	R3	K87
      0x5C140400,  //  0028  MOVE	R5	R2
      0x7C0C0400,  //  0029  CALL	R3	2
      0x80000000,  //  002A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_sub_id
********************************************************************/
be_local_closure(class_lvh_obj_get_sub_id,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_obj,     /* shared constants */
    be_str_weak(get_sub_id),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x4C040000,  //  0000  LDNIL	R1
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_pad_bottom2
********************************************************************/
be_local_closure(class_lvh_obj_set_pad_bottom2,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_obj,     /* shared constants */
    be_str_weak(set_pad_bottom2),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x88080103,  //  0000  GETMBR	R2	R0	K3
      0x4C0C0000,  //  0001  LDNIL	R3
      0x20080403,  //  0002  NE	R2	R2	R3
      0x780A0009,  //  0003  JMPF	R2	#000E
      0x88080101,  //  0004  GETMBR	R2	R0	K1
      0x8C080558,  //  0005  GETMET	R2	R2	K88
      0x60100009,  //  0006  GETGBL	R4	G9
      0x5C140200,  //  0007  MOVE	R5	R1
      0x7C100200,  //  0008  CALL	R4	1
      0x88140103,  //  0009  GETMBR	R5	R0	K3
      0xB81A0A00,  //  000A  GETNGBL	R6	K5
      0x88180D06,  //  000B  GETMBR	R6	R6	K6
      0x30140A06,  //  000C  OR	R5	R5	R6
      0x7C080600,  //  000D  CALL	R2	3
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_val
********************************************************************/
be_local_closure(class_lvh_obj_set_val,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_obj,     /* shared constants */
    be_str_weak(set_val),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0xA40A3600,  //  0000  IMPORT	R2	K27
      0x9002B201,  //  0001  SETMBR	R0	K89	R1
      0x8C0C052B,  //  0002  GETMET	R3	R2	K43
      0x88140101,  //  0003  GETMBR	R5	R0	K1
      0x5818005A,  //  0004  LDCONST	R6	K90
      0x7C0C0600,  //  0005  CALL	R3	3
      0x780E0003,  //  0006  JMPF	R3	#000B
      0x880C0101,  //  0007  GETMBR	R3	R0	K1
      0x8C0C075A,  //  0008  GETMET	R3	R3	K90
      0x5C140200,  //  0009  MOVE	R5	R1
      0x7C0C0400,  //  000A  CALL	R3	2
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_hidden
********************************************************************/
be_local_closure(class_lvh_obj_get_hidden,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_obj,     /* shared constants */
    be_str_weak(get_hidden),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x88040101,  //  0000  GETMBR	R1	R0	K1
      0x8C040346,  //  0001  GETMET	R1	R1	K70
      0xB80E0A00,  //  0002  GETNGBL	R3	K5
      0x880C075B,  //  0003  GETMBR	R3	R3	K91
      0x7C040400,  //  0004  CALL	R1	2
      0x80040200,  //  0005  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_click
********************************************************************/
be_local_closure(class_lvh_obj_get_click,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_obj,     /* shared constants */
    be_str_weak(get_click),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x8C04015C,  //  0000  GETMET	R1	R0	K92
      0x7C040200,  //  0001  CALL	R1	1
      0x80040200,  //  0002  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_text
********************************************************************/
be_local_closure(class_lvh_obj_get_text,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_obj,     /* shared constants */
    be_str_weak(get_text),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x8804010B,  //  0000  GETMBR	R1	R0	K11
      0x4C080000,  //  0001  LDNIL	R2
      0x1C040202,  //  0002  EQ	R1	R1	R2
      0x78060001,  //  0003  JMPF	R1	#0006
      0x4C040000,  //  0004  LDNIL	R1
      0x80040200,  //  0005  RET	1	R1
      0x8804010B,  //  0006  GETMBR	R1	R0	K11
      0x8C04035D,  //  0007  GETMET	R1	R1	K93
      0x7C040200,  //  0008  CALL	R1	1
      0x80040200,  //  0009  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_value_color
********************************************************************/
be_local_closure(class_lvh_obj_get_value_color,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_obj,     /* shared constants */
    be_str_weak(get_value_color),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x8C04015E,  //  0000  GETMET	R1	R0	K94
      0x7C040200,  //  0001  CALL	R1	1
      0x80040200,  //  0002  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_label_mode
********************************************************************/
be_local_closure(class_lvh_obj_get_label_mode,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_obj,     /* shared constants */
    be_str_weak(get_label_mode),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x8804010B,  //  0000  GETMBR	R1	R0	K11
      0x4C080000,  //  0001  LDNIL	R2
      0x20040202,  //  0002  NE	R1	R1	R2
      0x78060003,  //  0003  JMPF	R1	#0008
      0x8804010B,  //  0004  GETMBR	R1	R0	K11
      0x8C04035F,  //  0005  GETMET	R1	R1	K95
      0x7C040200,  //  0006  CALL	R1	1
      0x80040200,  //  0007  RET	1	R1
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_adjustable
********************************************************************/
be_local_closure(class_lvh_obj_set_adjustable,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_obj,     /* shared constants */
    be_str_weak(set_adjustable),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x78060005,  //  0000  JMPF	R1	#0007
      0x88080101,  //  0001  GETMBR	R2	R0	K1
      0x8C080548,  //  0002  GETMET	R2	R2	K72
      0xB8120A00,  //  0003  GETNGBL	R4	K5
      0x88100960,  //  0004  GETMBR	R4	R4	K96
      0x7C080400,  //  0005  CALL	R2	2
      0x70020004,  //  0006  JMP		#000C
      0x88080101,  //  0007  GETMBR	R2	R0	K1
      0x8C080549,  //  0008  GETMET	R2	R2	K73
      0xB8120A00,  //  0009  GETNGBL	R4	K5
      0x88100960,  //  000A  GETMBR	R4	R4	K96
      0x7C080400,  //  000B  CALL	R2	2
      0x80000000,  //  000C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_val
********************************************************************/
be_local_closure(class_lvh_obj_get_val,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_obj,     /* shared constants */
    be_str_weak(get_val),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040101,  //  0000  GETMBR	R1	R0	K1
      0x8C040361,  //  0001  GETMET	R1	R1	K97
      0x7C040200,  //  0002  CALL	R1	1
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_text_font
********************************************************************/
be_local_closure(class_lvh_obj_set_text_font,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_obj,     /* shared constants */
    be_str_weak(set_text_font),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x8C080162,  //  0000  GETMET	R2	R0	K98
      0x5C100200,  //  0001  MOVE	R4	R1
      0x7C080400,  //  0002  CALL	R2	2
      0x4C0C0000,  //  0003  LDNIL	R3
      0x200C0403,  //  0004  NE	R3	R2	R3
      0x780E0004,  //  0005  JMPF	R3	#000B
      0x880C0101,  //  0006  GETMBR	R3	R0	K1
      0x8C0C0763,  //  0007  GETMET	R3	R3	K99
      0x5C140400,  //  0008  MOVE	R5	R2
      0x58180011,  //  0009  LDCONST	R6	K17
      0x7C0C0600,  //  000A  CALL	R3	3
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_radius2
********************************************************************/
be_local_closure(class_lvh_obj_get_radius2,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_obj,     /* shared constants */
    be_str_weak(get_radius2),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x88040103,  //  0000  GETMBR	R1	R0	K3
      0x4C080000,  //  0001  LDNIL	R2
      0x20040202,  //  0002  NE	R1	R1	R2
      0x78060007,  //  0003  JMPF	R1	#000C
      0x88040101,  //  0004  GETMBR	R1	R0	K1
      0x8C040364,  //  0005  GETMET	R1	R1	K100
      0x880C0103,  //  0006  GETMBR	R3	R0	K3
      0xB8120A00,  //  0007  GETNGBL	R4	K5
      0x88100906,  //  0008  GETMBR	R4	R4	K6
      0x300C0604,  //  0009  OR	R3	R3	R4
      0x7C040400,  //  000A  CALL	R1	2
      0x80040200,  //  000B  RET	1	R1
      0x80000000,  //  000C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_pad_top2
********************************************************************/
be_local_closure(class_lvh_obj_set_pad_top2,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_obj,     /* shared constants */
    be_str_weak(set_pad_top2),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x88080103,  //  0000  GETMBR	R2	R0	K3
      0x4C0C0000,  //  0001  LDNIL	R3
      0x20080403,  //  0002  NE	R2	R2	R3
      0x780A0009,  //  0003  JMPF	R2	#000E
      0x88080101,  //  0004  GETMBR	R2	R0	K1
      0x8C080565,  //  0005  GETMET	R2	R2	K101
      0x60100009,  //  0006  GETGBL	R4	G9
      0x5C140200,  //  0007  MOVE	R5	R1
      0x7C100200,  //  0008  CALL	R4	1
      0x88140103,  //  0009  GETMBR	R5	R0	K3
      0xB81A0A00,  //  000A  GETNGBL	R6	K5
      0x88180D06,  //  000B  GETMBR	R6	R6	K6
      0x30140A06,  //  000C  OR	R5	R5	R6
      0x7C080600,  //  000D  CALL	R2	3
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_value_font
********************************************************************/
be_local_closure(class_lvh_obj_set_value_font,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_obj,     /* shared constants */
    be_str_weak(set_value_font),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x8C080166,  //  0000  GETMET	R2	R0	K102
      0x5C100200,  //  0001  MOVE	R4	R1
      0x7C080400,  //  0002  CALL	R2	2
      0x80000000,  //  0003  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_click
********************************************************************/
be_local_closure(class_lvh_obj_set_click,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_obj,     /* shared constants */
    be_str_weak(set_click),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x8C080167,  //  0000  GETMET	R2	R0	K103
      0x5C100200,  //  0001  MOVE	R4	R1
      0x7C080400,  //  0002  CALL	R2	2
      0x80000000,  //  0003  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_pad_right2
********************************************************************/
be_local_closure(class_lvh_obj_set_pad_right2,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_obj,     /* shared constants */
    be_str_weak(set_pad_right2),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x88080103,  //  0000  GETMBR	R2	R0	K3
      0x4C0C0000,  //  0001  LDNIL	R3
      0x20080403,  //  0002  NE	R2	R2	R3
      0x780A0009,  //  0003  JMPF	R2	#000E
      0x88080101,  //  0004  GETMBR	R2	R0	K1
      0x8C080568,  //  0005  GETMET	R2	R2	K104
      0x60100009,  //  0006  GETGBL	R4	G9
      0x5C140200,  //  0007  MOVE	R5	R1
      0x7C100200,  //  0008  CALL	R4	1
      0x88140103,  //  0009  GETMBR	R5	R0	K3
      0xB81A0A00,  //  000A  GETNGBL	R6	K5
      0x88180D06,  //  000B  GETMBR	R6	R6	K6
      0x30140A06,  //  000C  OR	R5	R5	R6
      0x7C080600,  //  000D  CALL	R2	3
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_value_color
********************************************************************/
be_local_closure(class_lvh_obj_set_value_color,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_obj,     /* shared constants */
    be_str_weak(set_value_color),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x8C0C0169,  //  0000  GETMET	R3	R0	K105
      0x5C140200,  //  0001  MOVE	R5	R1
      0x5C180400,  //  0002  MOVE	R6	R2
      0x7C0C0600,  //  0003  CALL	R3	3
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_adjustable
********************************************************************/
be_local_closure(class_lvh_obj_get_adjustable,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_obj,     /* shared constants */
    be_str_weak(get_adjustable),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x88040101,  //  0000  GETMBR	R1	R0	K1
      0x8C040346,  //  0001  GETMET	R1	R1	K70
      0xB80E0A00,  //  0002  GETNGBL	R3	K5
      0x880C0760,  //  0003  GETMBR	R3	R3	K96
      0x7C040400,  //  0004  CALL	R1	2
      0x80040200,  //  0005  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: event_cb
********************************************************************/
be_local_closure(class_lvh_obj_event_cb,   /* name */
  be_nested_proto(
    16,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 2]) {
      be_nested_proto(
        4,                          /* nstack */
        0,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 3]) {  /* upvals */
          be_local_const_upval(1, 2),
          be_local_const_upval(1, 0),
          be_local_const_upval(1, 3),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 1]) {     /* constants */
        /* K0   */  be_nested_str_weak(do_action),
        }),
        be_str_weak(_X3Clambda_X3E),
        &be_const_str_solidified,
        ( &(const binstruction[ 6]) {  /* code */
          0x68000000,  //  0000  GETUPV	R0	U0
          0x8C000100,  //  0001  GETMET	R0	R0	K0
          0x68080001,  //  0002  GETUPV	R2	U1
          0x680C0002,  //  0003  GETUPV	R3	U2
          0x7C000600,  //  0004  CALL	R0	3
          0x80040000,  //  0005  RET	1	R0
        })
      ),
      be_nested_proto(
        5,                          /* nstack */
        0,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 9),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 4]) {     /* constants */
        /* K0   */  be_nested_str_weak(tasmota),
        /* K1   */  be_nested_str_weak(publish_rule),
        /* K2   */  be_nested_str_weak(log),
        /* K3   */  be_nested_str_weak(HSP_X3A_X20publish_X20_X25s),
        }),
        be_str_weak(_anonymous_),
        &be_const_str_solidified,
        ( &(const binstruction[13]) {  /* code */
          0xB8020000,  //  0000  GETNGBL	R0	K0
          0x8C000101,  //  0001  GETMET	R0	R0	K1
          0x68080000,  //  0002  GETUPV	R2	U0
          0x7C000400,  //  0003  CALL	R0	2
          0xB8020000,  //  0004  GETNGBL	R0	K0
          0x8C000102,  //  0005  GETMET	R0	R0	K2
          0x60080018,  //  0006  GETGBL	R2	G24
          0x580C0003,  //  0007  LDCONST	R3	K3
          0x68100000,  //  0008  GETUPV	R4	U0
          0x7C080400,  //  0009  CALL	R2	2
          0x540E0003,  //  000A  LDINT	R3	4
          0x7C000600,  //  000B  CALL	R0	3
          0x80000000,  //  000C  RET	0
        })
      ),
    }),
    1,                          /* has constants */
    &be_ktab_class_lvh_obj,     /* shared constants */
    be_str_weak(event_cb),
    &be_const_str_solidified,
    ( &(const binstruction[105]) {  /* code */
      0x8808010E,  //  0000  GETMBR	R2	R0	K14
      0x8808050F,  //  0001  GETMBR	R2	R2	K15
      0x8C0C036A,  //  0002  GETMET	R3	R1	K106
      0x7C0C0200,  //  0003  CALL	R3	1
      0x8810016B,  //  0004  GETMBR	R4	R0	K107
      0x20100945,  //  0005  NE	R4	R4	K69
      0x78120007,  //  0006  JMPF	R4	#000F
      0xB8120A00,  //  0007  GETNGBL	R4	K5
      0x8810096C,  //  0008  GETMBR	R4	R4	K108
      0x1C100604,  //  0009  EQ	R4	R3	R4
      0x78120003,  //  000A  JMPF	R4	#000F
      0xB812DA00,  //  000B  GETNGBL	R4	K109
      0x8C10096E,  //  000C  GETMET	R4	R4	K110
      0x84180000,  //  000D  CLOSURE	R6	P0
      0x7C100400,  //  000E  CALL	R4	2
      0x8810016F,  //  000F  GETMBR	R4	R0	K111
      0x8C100924,  //  0010  GETMET	R4	R4	K36
      0x5C180600,  //  0011  MOVE	R6	R3
      0x7C100400,  //  0012  CALL	R4	2
      0x4C140000,  //  0013  LDNIL	R5
      0x20140805,  //  0014  NE	R5	R4	R5
      0x78160050,  //  0015  JMPF	R5	#0067
      0xA416E000,  //  0016  IMPORT	R5	K112
      0x58180045,  //  0017  LDCONST	R6	K69
      0xB81E0A00,  //  0018  GETNGBL	R7	K5
      0x881C0F71,  //  0019  GETMBR	R7	R7	K113
      0x1C1C0607,  //  001A  EQ	R7	R3	R7
      0x781E0026,  //  001B  JMPF	R7	#0043
      0xA41E3600,  //  001C  IMPORT	R7	K27
      0x8C200F25,  //  001D  GETMET	R8	R7	K37
      0x5C280000,  //  001E  MOVE	R10	R0
      0x582C0072,  //  001F  LDCONST	R11	K114
      0x7C200600,  //  0020  CALL	R8	3
      0x4C240000,  //  0021  LDNIL	R9
      0x20241009,  //  0022  NE	R9	R8	R9
      0x7826000B,  //  0023  JMPF	R9	#0030
      0x60240004,  //  0024  GETGBL	R9	G4
      0x5C281000,  //  0025  MOVE	R10	R8
      0x7C240200,  //  0026  CALL	R9	1
      0x20241373,  //  0027  NE	R9	R9	K115
      0x78260006,  //  0028  JMPF	R9	#0030
      0x60240018,  //  0029  GETGBL	R9	G24
      0x58280074,  //  002A  LDCONST	R10	K116
      0x8C2C0B75,  //  002B  GETMET	R11	R5	K117
      0x5C341000,  //  002C  MOVE	R13	R8
      0x7C2C0400,  //  002D  CALL	R11	2
      0x7C240400,  //  002E  CALL	R9	2
      0x5C181200,  //  002F  MOVE	R6	R9
      0x8C240F25,  //  0030  GETMET	R9	R7	K37
      0x5C2C0000,  //  0031  MOVE	R11	R0
      0x58300076,  //  0032  LDCONST	R12	K118
      0x7C240600,  //  0033  CALL	R9	3
      0x4C280000,  //  0034  LDNIL	R10
      0x2028120A,  //  0035  NE	R10	R9	R10
      0x782A000B,  //  0036  JMPF	R10	#0043
      0x60280004,  //  0037  GETGBL	R10	G4
      0x5C2C1200,  //  0038  MOVE	R11	R9
      0x7C280200,  //  0039  CALL	R10	1
      0x20281573,  //  003A  NE	R10	R10	K115
      0x782A0006,  //  003B  JMPF	R10	#0043
      0x60280018,  //  003C  GETGBL	R10	G24
      0x582C0077,  //  003D  LDCONST	R11	K119
      0x8C300B75,  //  003E  GETMET	R12	R5	K117
      0x5C381200,  //  003F  MOVE	R14	R9
      0x7C300400,  //  0040  CALL	R12	2
      0x7C280400,  //  0041  CALL	R10	2
      0x00180C0A,  //  0042  ADD	R6	R6	R10
      0x881C0178,  //  0043  GETMBR	R7	R0	K120
      0x4C200000,  //  0044  LDNIL	R8
      0x201C0E08,  //  0045  NE	R7	R7	R8
      0x781E0006,  //  0046  JMPF	R7	#004E
      0x601C0018,  //  0047  GETGBL	R7	G24
      0x58200079,  //  0048  LDCONST	R8	K121
      0x8C240B75,  //  0049  GETMET	R9	R5	K117
      0x882C0178,  //  004A  GETMBR	R11	R0	K120
      0x7C240400,  //  004B  CALL	R9	2
      0x7C1C0400,  //  004C  CALL	R7	2
      0x00180C07,  //  004D  ADD	R6	R6	R7
      0x8C1C017A,  //  004E  GETMET	R7	R0	K122
      0x7C1C0200,  //  004F  CALL	R7	1
      0x4C200000,  //  0050  LDNIL	R8
      0x20200E08,  //  0051  NE	R8	R7	R8
      0x78220004,  //  0052  JMPF	R8	#0058
      0x60200008,  //  0053  GETGBL	R8	G8
      0x5C240E00,  //  0054  MOVE	R9	R7
      0x7C200200,  //  0055  CALL	R8	1
      0x0022F608,  //  0056  ADD	R8	K123	R8
      0x70020000,  //  0057  JMP		#0059
      0x58200045,  //  0058  LDCONST	R8	K69
      0x60240018,  //  0059  GETGBL	R9	G24
      0x5828007C,  //  005A  LDCONST	R10	K124
      0x882C010E,  //  005B  GETMBR	R11	R0	K14
      0x882C177D,  //  005C  GETMBR	R11	R11	K125
      0x8830017E,  //  005D  GETMBR	R12	R0	K126
      0x5C341000,  //  005E  MOVE	R13	R8
      0x5C380800,  //  005F  MOVE	R14	R4
      0x5C3C0C00,  //  0060  MOVE	R15	R6
      0x7C240C00,  //  0061  CALL	R9	6
      0xB82ADA00,  //  0062  GETNGBL	R10	K109
      0x8C28156E,  //  0063  GETMET	R10	R10	K110
      0x84300001,  //  0064  CLOSURE	R12	P1
      0x7C280400,  //  0065  CALL	R10	2
      0xA0140000,  //  0066  CLOSE	R5
      0xA0000000,  //  0067  CLOSE	R0
      0x80000000,  //  0068  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_line_width
********************************************************************/
be_local_closure(class_lvh_obj_get_line_width,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_obj,     /* shared constants */
    be_str_weak(get_line_width),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x88080101,  //  0000  GETMBR	R2	R0	K1
      0x8C08057F,  //  0001  GETMET	R2	R2	K127
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x80040400,  //  0004  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_radius2
********************************************************************/
be_local_closure(class_lvh_obj_set_radius2,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_obj,     /* shared constants */
    be_str_weak(set_radius2),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x88080103,  //  0000  GETMBR	R2	R0	K3
      0x4C0C0000,  //  0001  LDNIL	R3
      0x20080403,  //  0002  NE	R2	R2	R3
      0x780A0009,  //  0003  JMPF	R2	#000E
      0x88080101,  //  0004  GETMBR	R2	R0	K1
      0x8C080580,  //  0005  GETMET	R2	R2	K128
      0x60100009,  //  0006  GETGBL	R4	G9
      0x5C140200,  //  0007  MOVE	R5	R1
      0x7C100200,  //  0008  CALL	R4	1
      0x88140103,  //  0009  GETMBR	R5	R0	K3
      0xB81A0A00,  //  000A  GETNGBL	R6	K5
      0x88180D06,  //  000B  GETMBR	R6	R6	K6
      0x30140A06,  //  000C  OR	R5	R5	R6
      0x7C080600,  //  000D  CALL	R2	3
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_hidden
********************************************************************/
be_local_closure(class_lvh_obj_set_hidden,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_obj,     /* shared constants */
    be_str_weak(set_hidden),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x78060005,  //  0000  JMPF	R1	#0007
      0x88080101,  //  0001  GETMBR	R2	R0	K1
      0x8C080548,  //  0002  GETMET	R2	R2	K72
      0xB8120A00,  //  0003  GETNGBL	R4	K5
      0x8810095B,  //  0004  GETMBR	R4	R4	K91
      0x7C080400,  //  0005  CALL	R2	2
      0x70020004,  //  0006  JMP		#000C
      0x88080101,  //  0007  GETMBR	R2	R0	K1
      0x8C080549,  //  0008  GETMET	R2	R2	K73
      0xB8120A00,  //  0009  GETNGBL	R4	K5
      0x8810095B,  //  000A  GETMBR	R4	R4	K91
      0x7C080400,  //  000B  CALL	R2	2
      0x80000000,  //  000C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_align
********************************************************************/
be_local_closure(class_lvh_obj_get_align,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_obj,     /* shared constants */
    be_str_weak(get_align),
    &be_const_str_solidified,
    ( &(const binstruction[33]) {  /* code */
      0x1C080311,  //  0000  EQ	R2	R1	K17
      0x780A0000,  //  0001  JMPF	R2	#0003
      0x58040011,  //  0002  LDCONST	R1	K17
      0x8808010B,  //  0003  GETMBR	R2	R0	K11
      0x4C0C0000,  //  0004  LDNIL	R3
      0x1C080403,  //  0005  EQ	R2	R2	R3
      0x780A0001,  //  0006  JMPF	R2	#0009
      0x4C080000,  //  0007  LDNIL	R2
      0x80040400,  //  0008  RET	1	R2
      0x8808010B,  //  0009  GETMBR	R2	R0	K11
      0x8C080581,  //  000A  GETMET	R2	R2	K129
      0x5C100200,  //  000B  MOVE	R4	R1
      0x7C080400,  //  000C  CALL	R2	2
      0xB80E0A00,  //  000D  GETNGBL	R3	K5
      0x880C0782,  //  000E  GETMBR	R3	R3	K130
      0x1C0C0403,  //  000F  EQ	R3	R2	R3
      0x780E0001,  //  0010  JMPF	R3	#0013
      0x80070600,  //  0011  RET	1	K131
      0x7002000C,  //  0012  JMP		#0020
      0xB80E0A00,  //  0013  GETNGBL	R3	K5
      0x880C0784,  //  0014  GETMBR	R3	R3	K132
      0x1C0C0403,  //  0015  EQ	R3	R2	R3
      0x780E0001,  //  0016  JMPF	R3	#0019
      0x80070A00,  //  0017  RET	1	K133
      0x70020006,  //  0018  JMP		#0020
      0xB80E0A00,  //  0019  GETNGBL	R3	K5
      0x880C0786,  //  001A  GETMBR	R3	R3	K134
      0x1C0C0403,  //  001B  EQ	R3	R2	R3
      0x780E0001,  //  001C  JMPF	R3	#001F
      0x80070E00,  //  001D  RET	1	K135
      0x70020000,  //  001E  JMP		#0020
      0x80040400,  //  001F  RET	1	R2
      0x80000000,  //  0020  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_pad_left
********************************************************************/
be_local_closure(class_lvh_obj_get_pad_left,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_obj,     /* shared constants */
    be_str_weak(get_pad_left),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x88040103,  //  0000  GETMBR	R1	R0	K3
      0x4C080000,  //  0001  LDNIL	R2
      0x20040202,  //  0002  NE	R1	R1	R2
      0x78060007,  //  0003  JMPF	R1	#000C
      0x88040101,  //  0004  GETMBR	R1	R0	K1
      0x8C040388,  //  0005  GETMET	R1	R1	K136
      0x880C0103,  //  0006  GETMBR	R3	R0	K3
      0xB8120A00,  //  0007  GETNGBL	R4	K5
      0x88100906,  //  0008  GETMBR	R4	R4	K6
      0x300C0604,  //  0009  OR	R3	R3	R4
      0x7C040400,  //  000A  CALL	R1	2
      0x80040200,  //  000B  RET	1	R1
      0x80000000,  //  000C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_toggle
********************************************************************/
be_local_closure(class_lvh_obj_get_toggle,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_obj,     /* shared constants */
    be_str_weak(get_toggle),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x88040101,  //  0000  GETMBR	R1	R0	K1
      0x8C040315,  //  0001  GETMET	R1	R1	K21
      0xB80E0A00,  //  0002  GETNGBL	R3	K5
      0x880C0740,  //  0003  GETMBR	R3	R3	K64
      0x7C040400,  //  0004  CALL	R1	2
      0x80040200,  //  0005  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_value_font
********************************************************************/
be_local_closure(class_lvh_obj_get_value_font,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_obj,     /* shared constants */
    be_str_weak(get_value_font),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x8C040189,  //  0000  GETMET	R1	R0	K137
      0x7C040200,  //  0001  CALL	R1	1
      0x80040200,  //  0002  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_value_ofs_y
********************************************************************/
be_local_closure(class_lvh_obj_get_value_ofs_y,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_obj,     /* shared constants */
    be_str_weak(get_value_ofs_y),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x8804010B,  //  0000  GETMBR	R1	R0	K11
      0x8C04038A,  //  0001  GETMET	R1	R1	K138
      0x7C040200,  //  0002  CALL	R1	1
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_value_ofs_y
********************************************************************/
be_local_closure(class_lvh_obj_set_value_ofs_y,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_obj,     /* shared constants */
    be_str_weak(set_value_ofs_y),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x8C080117,  //  0000  GETMET	R2	R0	K23
      0x7C080200,  //  0001  CALL	R2	1
      0x8808010B,  //  0002  GETMBR	R2	R0	K11
      0x8C08058B,  //  0003  GETMET	R2	R2	K139
      0x60100009,  //  0004  GETGBL	R4	G9
      0x5C140200,  //  0005  MOVE	R5	R1
      0x7C100200,  //  0006  CALL	R4	1
      0x7C080400,  //  0007  CALL	R2	2
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_align
********************************************************************/
be_local_closure(class_lvh_obj_set_align,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_obj,     /* shared constants */
    be_str_weak(set_align),
    &be_const_str_solidified,
    ( &(const binstruction[32]) {  /* code */
      0x4C0C0000,  //  0000  LDNIL	R3
      0x1C100511,  //  0001  EQ	R4	R2	K17
      0x78120000,  //  0002  JMPF	R4	#0004
      0x58080011,  //  0003  LDCONST	R2	K17
      0x8C100117,  //  0004  GETMET	R4	R0	K23
      0x7C100200,  //  0005  CALL	R4	1
      0x1C100311,  //  0006  EQ	R4	R1	K17
      0x74120001,  //  0007  JMPT	R4	#000A
      0x1C100383,  //  0008  EQ	R4	R1	K131
      0x78120002,  //  0009  JMPF	R4	#000D
      0xB8120A00,  //  000A  GETNGBL	R4	K5
      0x880C0982,  //  000B  GETMBR	R3	R4	K130
      0x7002000C,  //  000C  JMP		#001A
      0x1C100313,  //  000D  EQ	R4	R1	K19
      0x74120001,  //  000E  JMPT	R4	#0011
      0x1C100385,  //  000F  EQ	R4	R1	K133
      0x78120002,  //  0010  JMPF	R4	#0014
      0xB8120A00,  //  0011  GETNGBL	R4	K5
      0x880C0984,  //  0012  GETMBR	R3	R4	K132
      0x70020005,  //  0013  JMP		#001A
      0x1C10038C,  //  0014  EQ	R4	R1	K140
      0x74120001,  //  0015  JMPT	R4	#0018
      0x1C100387,  //  0016  EQ	R4	R1	K135
      0x78120001,  //  0017  JMPF	R4	#001A
      0xB8120A00,  //  0018  GETNGBL	R4	K5
      0x880C0986,  //  0019  GETMBR	R3	R4	K134
      0x8810010B,  //  001A  GETMBR	R4	R0	K11
      0x8C10098D,  //  001B  GETMET	R4	R4	K141
      0x5C180600,  //  001C  MOVE	R6	R3
      0x5C1C0400,  //  001D  MOVE	R7	R2
      0x7C100600,  //  001E  CALL	R4	3
      0x80000000,  //  001F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: lvh_obj
********************************************************************/
extern const bclass be_class_lvh_root;
be_local_class(lvh_obj,
    2,
    &be_class_lvh_root,
    be_nested_map(63,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(_delete, -1), be_const_closure(class_lvh_obj__delete_closure) },
        { be_const_key_weak(get_action, -1), be_const_closure(class_lvh_obj_get_action_closure) },
        { be_const_key_weak(get_text_font, -1), be_const_closure(class_lvh_obj_get_text_font_closure) },
        { be_const_key_weak(get_pad_all, -1), be_const_closure(class_lvh_obj_get_pad_all_closure) },
        { be_const_key_weak(set_pad_bottom2, -1), be_const_closure(class_lvh_obj_set_pad_bottom2_closure) },
        { be_const_key_weak(get_value_font, -1), be_const_closure(class_lvh_obj_get_value_font_closure) },
        { be_const_key_weak(get_hidden, -1), be_const_closure(class_lvh_obj_get_hidden_closure) },
        { be_const_key_weak(_event_map, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(7,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(35, -1), be_nested_str_weak(changed) },
        { be_const_key_int(8, 5), be_nested_str_weak(long) },
        { be_const_key_int(9, -1), be_nested_str_weak(hold) },
        { be_const_key_int(10, 6), be_nested_str_weak(up) },
        { be_const_key_int(11, -1), be_nested_str_weak(release) },
        { be_const_key_int(1, -1), be_nested_str_weak(down) },
        { be_const_key_int(3, -1), be_nested_str_weak(lost) },
    }))    ) } )) },
        { be_const_key_weak(register_event_cb, -1), be_const_closure(class_lvh_obj_register_event_cb_closure) },
        { be_const_key_weak(get_pad_left, 11), be_const_closure(class_lvh_obj_get_pad_left_closure) },
        { be_const_key_weak(get_align, 12), be_const_closure(class_lvh_obj_get_align_closure) },
        { be_const_key_weak(get_value_color, -1), be_const_closure(class_lvh_obj_get_value_color_closure) },
        { be_const_key_weak(get_label_mode, -1), be_const_closure(class_lvh_obj_get_label_mode_closure) },
        { be_const_key_weak(member, -1), be_const_closure(class_lvh_obj_member_closure) },
        { be_const_key_weak(set_pad_left2, -1), be_const_closure(class_lvh_obj_set_pad_left2_closure) },
        { be_const_key_weak(check_label, -1), be_const_closure(class_lvh_obj_check_label_closure) },
        { be_const_key_weak(set_adjustable, 0), be_const_closure(class_lvh_obj_set_adjustable_closure) },
        { be_const_key_weak(post_init, -1), be_const_closure(class_lvh_obj_post_init_closure) },
        { be_const_key_weak(set_enabled, 35), be_const_closure(class_lvh_obj_set_enabled_closure) },
        { be_const_key_weak(setmember, -1), be_const_closure(class_lvh_obj_setmember_closure) },
        { be_const_key_weak(set_toggle, -1), be_const_closure(class_lvh_obj_set_toggle_closure) },
        { be_const_key_weak(_lv_part2_selector, -1), be_const_nil() },
        { be_const_key_weak(set_pad_all2, 6), be_const_closure(class_lvh_obj_set_pad_all2_closure) },
        { be_const_key_weak(get_value_ofs_x, -1), be_const_closure(class_lvh_obj_get_value_ofs_x_closure) },
        { be_const_key_weak(set_value_ofs_x, -1), be_const_closure(class_lvh_obj_set_value_ofs_x_closure) },
        { be_const_key_weak(set_action, -1), be_const_closure(class_lvh_obj_set_action_closure) },
        { be_const_key_weak(set_text, 1), be_const_closure(class_lvh_obj_set_text_closure) },
        { be_const_key_weak(get_flex_in_new_track, -1), be_const_closure(class_lvh_obj_get_flex_in_new_track_closure) },
        { be_const_key_weak(set_flex_in_new_track, -1), be_const_closure(class_lvh_obj_set_flex_in_new_track_closure) },
        { be_const_key_weak(_action, 56), be_const_var(1) },
        { be_const_key_weak(set_label_mode, 5), be_const_closure(class_lvh_obj_set_label_mode_closure) },
        { be_const_key_weak(get_sub_id, -1), be_const_closure(class_lvh_obj_get_sub_id_closure) },
        { be_const_key_weak(get_pad_top, 4), be_const_closure(class_lvh_obj_get_pad_top_closure) },
        { be_const_key_weak(init, 59), be_const_closure(class_lvh_obj_init_closure) },
        { be_const_key_weak(get_val, -1), be_const_closure(class_lvh_obj_get_val_closure) },
        { be_const_key_weak(event_cb, -1), be_const_closure(class_lvh_obj_event_cb_closure) },
        { be_const_key_weak(get_text, -1), be_const_closure(class_lvh_obj_get_text_closure) },
        { be_const_key_weak(_lv_label, -1), be_const_var(0) },
        { be_const_key_weak(get_pad_right, 9), be_const_closure(class_lvh_obj_get_pad_right_closure) },
        { be_const_key_weak(set_value_font, -1), be_const_closure(class_lvh_obj_set_value_font_closure) },
        { be_const_key_weak(_EVENTS, 16), be_const_bytes_instance(010A030B0823) },
        { be_const_key_weak(get_pad_bottom, 50), be_const_closure(class_lvh_obj_get_pad_bottom_closure) },
        { be_const_key_weak(set_text_font, -1), be_const_closure(class_lvh_obj_set_text_font_closure) },
        { be_const_key_weak(set_pad_right2, -1), be_const_closure(class_lvh_obj_set_pad_right2_closure) },
        { be_const_key_weak(set_pad_top2, -1), be_const_closure(class_lvh_obj_set_pad_top2_closure) },
        { be_const_key_weak(set_line_width, 39), be_const_closure(class_lvh_obj_set_line_width_closure) },
        { be_const_key_weak(set_click, -1), be_const_closure(class_lvh_obj_set_click_closure) },
        { be_const_key_weak(get_text_color, 43), be_const_closure(class_lvh_obj_get_text_color_closure) },
        { be_const_key_weak(set_value_color, -1), be_const_closure(class_lvh_obj_set_value_color_closure) },
        { be_const_key_weak(get_adjustable, -1), be_const_closure(class_lvh_obj_get_adjustable_closure) },
        { be_const_key_weak(get_radius2, 34), be_const_closure(class_lvh_obj_get_radius2_closure) },
        { be_const_key_weak(get_line_width, -1), be_const_closure(class_lvh_obj_get_line_width_closure) },
        { be_const_key_weak(_lv_class, -1), be_const_class(be_class_lv_obj) },
        { be_const_key_weak(set_radius2, -1), be_const_closure(class_lvh_obj_set_radius2_closure) },
        { be_const_key_weak(set_hidden, -1), be_const_closure(class_lvh_obj_set_hidden_closure) },
        { be_const_key_weak(set_text_color, 10), be_const_closure(class_lvh_obj_set_text_color_closure) },
        { be_const_key_weak(get_click, -1), be_const_closure(class_lvh_obj_get_click_closure) },
        { be_const_key_weak(get_enabled, 7), be_const_closure(class_lvh_obj_get_enabled_closure) },
        { be_const_key_weak(get_toggle, -1), be_const_closure(class_lvh_obj_get_toggle_closure) },
        { be_const_key_weak(set_val, -1), be_const_closure(class_lvh_obj_set_val_closure) },
        { be_const_key_weak(get_value_ofs_y, -1), be_const_closure(class_lvh_obj_get_value_ofs_y_closure) },
        { be_const_key_weak(set_value_ofs_y, -1), be_const_closure(class_lvh_obj_set_value_ofs_y_closure) },
        { be_const_key_weak(set_align, -1), be_const_closure(class_lvh_obj_set_align_closure) },
    })),
    be_str_weak(lvh_obj)
);

extern const bclass be_class_lvh_fixed;

/********************************************************************
** Solidified function: post_init
********************************************************************/
be_local_closure(class_lvh_fixed_post_init,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[11]) {     /* constants */
    /* K0   */  be_nested_str_weak(post_init),
    /* K1   */  be_nested_str_weak(_lv_obj),
    /* K2   */  be_nested_str_weak(set_style_pad_all),
    /* K3   */  be_const_int(0),
    /* K4   */  be_nested_str_weak(set_style_radius),
    /* K5   */  be_nested_str_weak(set_style_border_width),
    /* K6   */  be_nested_str_weak(set_style_margin_all),
    /* K7   */  be_nested_str_weak(set_style_bg_opa),
    /* K8   */  be_nested_str_weak(set_size),
    /* K9   */  be_nested_str_weak(lv),
    /* K10  */  be_nested_str_weak(pct),
    }),
    be_str_weak(post_init),
    &be_const_str_solidified,
    ( &(const binstruction[38]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080500,  //  0003  GETMET	R2	R2	K0
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x88080101,  //  0006  GETMBR	R2	R0	K1
      0x8C0C0502,  //  0007  GETMET	R3	R2	K2
      0x58140003,  //  0008  LDCONST	R5	K3
      0x58180003,  //  0009  LDCONST	R6	K3
      0x7C0C0600,  //  000A  CALL	R3	3
      0x8C0C0504,  //  000B  GETMET	R3	R2	K4
      0x58140003,  //  000C  LDCONST	R5	K3
      0x58180003,  //  000D  LDCONST	R6	K3
      0x7C0C0600,  //  000E  CALL	R3	3
      0x8C0C0505,  //  000F  GETMET	R3	R2	K5
      0x58140003,  //  0010  LDCONST	R5	K3
      0x58180003,  //  0011  LDCONST	R6	K3
      0x7C0C0600,  //  0012  CALL	R3	3
      0x8C0C0506,  //  0013  GETMET	R3	R2	K6
      0x58140003,  //  0014  LDCONST	R5	K3
      0x58180003,  //  0015  LDCONST	R6	K3
      0x7C0C0600,  //  0016  CALL	R3	3
      0x8C0C0507,  //  0017  GETMET	R3	R2	K7
      0x58140003,  //  0018  LDCONST	R5	K3
      0x58180003,  //  0019  LDCONST	R6	K3
      0x7C0C0600,  //  001A  CALL	R3	3
      0x8C0C0508,  //  001B  GETMET	R3	R2	K8
      0xB8161200,  //  001C  GETNGBL	R5	K9
      0x8C140B0A,  //  001D  GETMET	R5	R5	K10
      0x541E0063,  //  001E  LDINT	R7	100
      0x7C140400,  //  001F  CALL	R5	2
      0xB81A1200,  //  0020  GETNGBL	R6	K9
      0x8C180D0A,  //  0021  GETMET	R6	R6	K10
      0x54220063,  //  0022  LDINT	R8	100
      0x7C180400,  //  0023  CALL	R6	2
      0x7C0C0600,  //  0024  CALL	R3	3
      0x80000000,  //  0025  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: lvh_fixed
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_class(lvh_fixed,
    0,
    &be_class_lvh_obj,
    be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(post_init, -1), be_const_closure(class_lvh_fixed_post_init_closure) },
    })),
    be_str_weak(lvh_fixed)
);

extern const bclass be_class_lvh_flex;

/********************************************************************
** Solidified function: post_init
********************************************************************/
be_local_closure(class_lvh_flex_post_init,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(post_init),
    /* K1   */  be_nested_str_weak(_lv_obj),
    /* K2   */  be_nested_str_weak(set_flex_flow),
    /* K3   */  be_nested_str_weak(lv),
    /* K4   */  be_nested_str_weak(FLEX_FLOW_ROW),
    }),
    be_str_weak(post_init),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080500,  //  0003  GETMET	R2	R2	K0
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x88080101,  //  0006  GETMBR	R2	R0	K1
      0x8C0C0502,  //  0007  GETMET	R3	R2	K2
      0xB8160600,  //  0008  GETNGBL	R5	K3
      0x88140B04,  //  0009  GETMBR	R5	R5	K4
      0x7C0C0400,  //  000A  CALL	R3	2
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: lvh_flex
********************************************************************/
extern const bclass be_class_lvh_fixed;
be_local_class(lvh_flex,
    0,
    &be_class_lvh_fixed,
    be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(post_init, -1), be_const_closure(class_lvh_flex_post_init_closure) },
        { be_const_key_weak(_EVENTS, -1), be_const_bytes_instance() },
    })),
    be_str_weak(lvh_flex)
);

extern const bclass be_class_lvh_label;

/********************************************************************
** Solidified function: post_init
********************************************************************/
be_local_closure(class_lvh_label_post_init,   /* name */
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
    /* K0   */  be_nested_str_weak(_lv_label),
    /* K1   */  be_nested_str_weak(_lv_obj),
    /* K2   */  be_nested_str_weak(post_init),
    }),
    be_str_weak(post_init),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x88080101,  //  0000  GETMBR	R2	R0	K1
      0x90020002,  //  0001  SETMBR	R0	K0	R2
      0x60080003,  //  0002  GETGBL	R2	G3
      0x5C0C0000,  //  0003  MOVE	R3	R0
      0x7C080200,  //  0004  CALL	R2	1
      0x8C080502,  //  0005  GETMET	R2	R2	K2
      0x5C100200,  //  0006  MOVE	R4	R1
      0x7C080400,  //  0007  CALL	R2	2
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: lvh_label
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_class(lvh_label,
    0,
    &be_class_lvh_obj,
    be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(post_init, -1), be_const_closure(class_lvh_label_post_init_closure) },
        { be_const_key_weak(_lv_class, -1), be_const_class(be_class_lv_label) },
    })),
    be_str_weak(lvh_label)
);
// compact class 'lvh_arc' ktab size: 26, total: 49 (saved 184 bytes)
static const bvalue be_ktab_class_lvh_arc[26] = {
  /* K0   */  be_nested_str_weak(_lv_obj),
  /* K1   */  be_nested_str_weak(get_mode),
  /* K2   */  be_nested_str_weak(_label_angle),
  /* K3   */  be_nested_str_weak(_lv_label),
  /* K4   */  be_nested_str_weak(rotate_obj_to_angle),
  /* K5   */  be_nested_str_weak(set_range),
  /* K6   */  be_nested_str_weak(get_max),
  /* K7   */  be_nested_str_weak(refresh_label_to_angle),
  /* K8   */  be_nested_str_weak(get_arc_line_width),
  /* K9   */  be_nested_str_weak(lv),
  /* K10  */  be_nested_str_weak(PART_INDICATOR),
  /* K11  */  be_nested_str_weak(STATE_DEFAULT),
  /* K12  */  be_const_int(0),
  /* K13  */  be_nested_str_weak(ARC_MODE_NORMAL),
  /* K14  */  be_const_int(1),
  /* K15  */  be_nested_str_weak(ARC_MODE_REVERSE),
  /* K16  */  be_const_int(2),
  /* K17  */  be_nested_str_weak(ARC_MODE_SYMMETRICAL),
  /* K18  */  be_nested_str_weak(set_mode),
  /* K19  */  be_nested_str_weak(set_style_arc_width),
  /* K20  */  be_nested_str_weak(get_min_value),
  /* K21  */  be_nested_str_weak(get_max_value),
  /* K22  */  be_nested_str_weak(set_val),
  /* K23  */  be_nested_str_weak(post_config),
  /* K24  */  be_nested_str_weak(get_min),
  /* K25  */  be_nested_str_weak(set_text),
};


extern const bclass be_class_lvh_arc;

/********************************************************************
** Solidified function: get_type
********************************************************************/
be_local_closure(class_lvh_arc_get_type,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_arc,     /* shared constants */
    be_str_weak(get_type),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x7C040200,  //  0002  CALL	R1	1
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: refresh_label_to_angle
********************************************************************/
be_local_closure(class_lvh_arc_refresh_label_to_angle,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_arc,     /* shared constants */
    be_str_weak(refresh_label_to_angle),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x88040102,  //  0000  GETMBR	R1	R0	K2
      0x4C080000,  //  0001  LDNIL	R2
      0x20040202,  //  0002  NE	R1	R1	R2
      0x78060008,  //  0003  JMPF	R1	#000D
      0x88040103,  //  0004  GETMBR	R1	R0	K3
      0x4C080000,  //  0005  LDNIL	R2
      0x20040202,  //  0006  NE	R1	R1	R2
      0x78060004,  //  0007  JMPF	R1	#000D
      0x88040100,  //  0008  GETMBR	R1	R0	K0
      0x8C040304,  //  0009  GETMET	R1	R1	K4
      0x880C0103,  //  000A  GETMBR	R3	R0	K3
      0x88100102,  //  000B  GETMBR	R4	R0	K2
      0x7C040600,  //  000C  CALL	R1	3
      0x80000000,  //  000D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_min
********************************************************************/
be_local_closure(class_lvh_arc_set_min,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_arc,     /* shared constants */
    be_str_weak(set_min),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080505,  //  0001  GETMET	R2	R2	K5
      0x60100009,  //  0002  GETGBL	R4	G9
      0x5C140200,  //  0003  MOVE	R5	R1
      0x7C100200,  //  0004  CALL	R4	1
      0x8C140106,  //  0005  GETMET	R5	R0	K6
      0x7C140200,  //  0006  CALL	R5	1
      0x7C080600,  //  0007  CALL	R2	3
      0x8C080107,  //  0008  GETMET	R2	R0	K7
      0x7C080200,  //  0009  CALL	R2	1
      0x80000000,  //  000A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_line_width1
********************************************************************/
be_local_closure(class_lvh_arc_get_line_width1,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_arc,     /* shared constants */
    be_str_weak(get_line_width1),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040308,  //  0001  GETMET	R1	R1	K8
      0xB80E1200,  //  0002  GETNGBL	R3	K9
      0x880C070A,  //  0003  GETMBR	R3	R3	K10
      0xB8121200,  //  0004  GETNGBL	R4	K9
      0x8810090B,  //  0005  GETMBR	R4	R4	K11
      0x300C0604,  //  0006  OR	R3	R3	R4
      0x7C040400,  //  0007  CALL	R1	2
      0x80040200,  //  0008  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_type
********************************************************************/
be_local_closure(class_lvh_arc_set_type,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_arc,     /* shared constants */
    be_str_weak(set_type),
    &be_const_str_solidified,
    ( &(const binstruction[23]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C0C030C,  //  0001  EQ	R3	R1	K12
      0x780E0002,  //  0002  JMPF	R3	#0006
      0xB80E1200,  //  0003  GETNGBL	R3	K9
      0x8808070D,  //  0004  GETMBR	R2	R3	K13
      0x70020008,  //  0005  JMP		#000F
      0x1C0C030E,  //  0006  EQ	R3	R1	K14
      0x780E0002,  //  0007  JMPF	R3	#000B
      0xB80E1200,  //  0008  GETNGBL	R3	K9
      0x8808070F,  //  0009  GETMBR	R2	R3	K15
      0x70020003,  //  000A  JMP		#000F
      0x1C0C0310,  //  000B  EQ	R3	R1	K16
      0x780E0001,  //  000C  JMPF	R3	#000F
      0xB80E1200,  //  000D  GETNGBL	R3	K9
      0x88080711,  //  000E  GETMBR	R2	R3	K17
      0x4C0C0000,  //  000F  LDNIL	R3
      0x200C0403,  //  0010  NE	R3	R2	R3
      0x780E0003,  //  0011  JMPF	R3	#0016
      0x880C0100,  //  0012  GETMBR	R3	R0	K0
      0x8C0C0712,  //  0013  GETMET	R3	R3	K18
      0x5C140400,  //  0014  MOVE	R5	R2
      0x7C0C0400,  //  0015  CALL	R3	2
      0x80000000,  //  0016  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_line_width1
********************************************************************/
be_local_closure(class_lvh_arc_set_line_width1,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_arc,     /* shared constants */
    be_str_weak(set_line_width1),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080513,  //  0001  GETMET	R2	R2	K19
      0x60100009,  //  0002  GETGBL	R4	G9
      0x5C140200,  //  0003  MOVE	R5	R1
      0x7C100200,  //  0004  CALL	R4	1
      0xB8161200,  //  0005  GETNGBL	R5	K9
      0x88140B0A,  //  0006  GETMBR	R5	R5	K10
      0xB81A1200,  //  0007  GETNGBL	R6	K9
      0x88180D0B,  //  0008  GETMBR	R6	R6	K11
      0x30140A06,  //  0009  OR	R5	R5	R6
      0x7C080600,  //  000A  CALL	R2	3
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_min
********************************************************************/
be_local_closure(class_lvh_arc_get_min,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_arc,     /* shared constants */
    be_str_weak(get_min),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040314,  //  0001  GETMET	R1	R1	K20
      0x7C040200,  //  0002  CALL	R1	1
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_line_width
********************************************************************/
be_local_closure(class_lvh_arc_set_line_width,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_arc,     /* shared constants */
    be_str_weak(set_line_width),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x8C0C0713,  //  0001  GETMET	R3	R3	K19
      0x60140009,  //  0002  GETGBL	R5	G9
      0x5C180200,  //  0003  MOVE	R6	R1
      0x7C140200,  //  0004  CALL	R5	1
      0x5C180400,  //  0005  MOVE	R6	R2
      0x7C0C0600,  //  0006  CALL	R3	3
      0x80000000,  //  0007  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_max
********************************************************************/
be_local_closure(class_lvh_arc_get_max,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_arc,     /* shared constants */
    be_str_weak(get_max),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040315,  //  0001  GETMET	R1	R1	K21
      0x7C040200,  //  0002  CALL	R1	1
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_label_to_angle
********************************************************************/
be_local_closure(class_lvh_arc_set_label_to_angle,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_arc,     /* shared constants */
    be_str_weak(set_label_to_angle),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x60080009,  //  0000  GETGBL	R2	G9
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x7C080200,  //  0002  CALL	R2	1
      0x90020402,  //  0003  SETMBR	R0	K2	R2
      0x8C080107,  //  0004  GETMET	R2	R0	K7
      0x7C080200,  //  0005  CALL	R2	1
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_val
********************************************************************/
be_local_closure(class_lvh_arc_set_val,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_arc,     /* shared constants */
    be_str_weak(set_val),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080516,  //  0003  GETMET	R2	R2	K22
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x8C080107,  //  0006  GETMET	R2	R0	K7
      0x7C080200,  //  0007  CALL	R2	1
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: post_config
********************************************************************/
be_local_closure(class_lvh_arc_post_config,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_arc,     /* shared constants */
    be_str_weak(post_config),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x60040003,  //  0000  GETGBL	R1	G3
      0x5C080000,  //  0001  MOVE	R2	R0
      0x7C040200,  //  0002  CALL	R1	1
      0x8C040317,  //  0003  GETMET	R1	R1	K23
      0x7C040200,  //  0004  CALL	R1	1
      0x8C040107,  //  0005  GETMET	R1	R0	K7
      0x7C040200,  //  0006  CALL	R1	1
      0x80000000,  //  0007  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_line_width
********************************************************************/
be_local_closure(class_lvh_arc_get_line_width,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_arc,     /* shared constants */
    be_str_weak(get_line_width),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080508,  //  0001  GETMET	R2	R2	K8
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x80040400,  //  0004  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_max
********************************************************************/
be_local_closure(class_lvh_arc_set_max,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_arc,     /* shared constants */
    be_str_weak(set_max),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080505,  //  0001  GETMET	R2	R2	K5
      0x8C100118,  //  0002  GETMET	R4	R0	K24
      0x7C100200,  //  0003  CALL	R4	1
      0x60140009,  //  0004  GETGBL	R5	G9
      0x5C180200,  //  0005  MOVE	R6	R1
      0x7C140200,  //  0006  CALL	R5	1
      0x7C080600,  //  0007  CALL	R2	3
      0x8C080107,  //  0008  GETMET	R2	R0	K7
      0x7C080200,  //  0009  CALL	R2	1
      0x80000000,  //  000A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_text
********************************************************************/
be_local_closure(class_lvh_arc_set_text,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_arc,     /* shared constants */
    be_str_weak(set_text),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080519,  //  0003  GETMET	R2	R2	K25
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x8C080107,  //  0006  GETMET	R2	R0	K7
      0x7C080200,  //  0007  CALL	R2	1
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: lvh_arc
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_class(lvh_arc,
    1,
    &be_class_lvh_obj,
    be_nested_map(18,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(_label_angle, -1), be_const_var(0) },
        { be_const_key_weak(refresh_label_to_angle, -1), be_const_closure(class_lvh_arc_refresh_label_to_angle_closure) },
        { be_const_key_weak(set_max, -1), be_const_closure(class_lvh_arc_set_max_closure) },
        { be_const_key_weak(get_line_width, -1), be_const_closure(class_lvh_arc_get_line_width_closure) },
        { be_const_key_weak(set_min, 13), be_const_closure(class_lvh_arc_set_min_closure) },
        { be_const_key_weak(post_config, -1), be_const_closure(class_lvh_arc_post_config_closure) },
        { be_const_key_weak(set_type, 11), be_const_closure(class_lvh_arc_set_type_closure) },
        { be_const_key_weak(_lv_class, -1), be_const_class(be_class_lv_arc) },
        { be_const_key_weak(get_min, -1), be_const_closure(class_lvh_arc_get_min_closure) },
        { be_const_key_weak(set_line_width, -1), be_const_closure(class_lvh_arc_set_line_width_closure) },
        { be_const_key_weak(get_max, -1), be_const_closure(class_lvh_arc_get_max_closure) },
        { be_const_key_weak(set_line_width1, -1), be_const_closure(class_lvh_arc_set_line_width1_closure) },
        { be_const_key_weak(_lv_part2_selector, -1), be_const_int(196608) },
        { be_const_key_weak(set_label_to_angle, 0), be_const_closure(class_lvh_arc_set_label_to_angle_closure) },
        { be_const_key_weak(get_line_width1, 5), be_const_closure(class_lvh_arc_get_line_width1_closure) },
        { be_const_key_weak(set_val, 3), be_const_closure(class_lvh_arc_set_val_closure) },
        { be_const_key_weak(get_type, 2), be_const_closure(class_lvh_arc_get_type_closure) },
        { be_const_key_weak(set_text, -1), be_const_closure(class_lvh_arc_set_text_closure) },
    })),
    be_str_weak(lvh_arc)
);

extern const bclass be_class_lvh_switch;

/********************************************************************
** Solidified function: set_val
********************************************************************/
be_local_closure(class_lvh_switch_set_val,   /* name */
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
    /* K0   */  be_nested_str_weak(_val),
    /* K1   */  be_nested_str_weak(set_toggle),
    }),
    be_str_weak(set_val),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x8C080101,  //  0001  GETMET	R2	R0	K1
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x80040400,  //  0004  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_val
********************************************************************/
be_local_closure(class_lvh_switch_get_val,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(get_toggle),
    }),
    be_str_weak(get_val),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x80040200,  //  0002  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: lvh_switch
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_class(lvh_switch,
    0,
    &be_class_lvh_obj,
    be_nested_map(4,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(get_val, 1), be_const_closure(class_lvh_switch_get_val_closure) },
        { be_const_key_weak(set_val, -1), be_const_closure(class_lvh_switch_set_val_closure) },
        { be_const_key_weak(_lv_part2_selector, -1), be_const_int(196608) },
        { be_const_key_weak(_lv_class, 0), be_const_class(be_class_lv_switch) },
    })),
    be_str_weak(lvh_switch)
);
// compact class 'lvh_msgbox' ktab size: 46, total: 72 (saved 208 bytes)
static const bvalue be_ktab_class_lvh_msgbox[46] = {
  /* K0   */  be_nested_str_weak(contains),
  /* K1   */  be_nested_str_weak(options),
  /* K2   */  be_nested_str_weak(set_options),
  /* K3   */  be_nested_str_weak(remove),
  /* K4   */  be_nested_str_weak(post_init),
  /* K5   */  be_nested_str_weak(string),
  /* K6   */  be_nested_str_weak(startswith),
  /* K7   */  be_nested_str_weak(footer_),
  /* K8   */  be_nested_str_weak(_footer),
  /* K9   */  be_nested_str_weak(setmember),
  /* K10  */  be_const_int(2147483647),
  /* K11  */  be_nested_str_weak(header_),
  /* K12  */  be_nested_str_weak(_header),
  /* K13  */  be_nested_str_weak(title_),
  /* K14  */  be_nested_str_weak(_title),
  /* K15  */  be_nested_str_weak(content_),
  /* K16  */  be_nested_str_weak(_content),
  /* K17  */  be_nested_str_weak(buttons_),
  /* K18  */  be_nested_str_weak(_buttons),
  /* K19  */  be_nested_str_weak(stop_iteration),
  /* K20  */  be_nested_str_weak(_page),
  /* K21  */  be_nested_str_weak(_hm),
  /* K22  */  be_nested_str_weak(lvh_obj),
  /* K23  */  be_nested_str_weak(_lv_obj),
  /* K24  */  be_nested_str_weak(add_footer_button),
  /* K25  */  be_nested_str_weak(push),
  /* K26  */  be_nested_str_weak(_modal),
  /* K27  */  be_nested_str_weak(find),
  /* K28  */  be_nested_str_weak(modal),
  /* K29  */  be_nested_str_weak(lv),
  /* K30  */  be_nested_str_weak(msgbox),
  /* K31  */  be_const_int(0),
  /* K32  */  be_nested_str_weak(init),
  /* K33  */  be_nested_str_weak(text_align),
  /* K34  */  be_const_int(2),
  /* K35  */  be_nested_str_weak(bg_opa),
  /* K36  */  be_nested_str_weak(post_config),
  /* K37  */  be_nested_str_weak(get_header),
  /* K38  */  be_nested_str_weak(get_footer),
  /* K39  */  be_nested_str_weak(get_content),
  /* K40  */  be_nested_str_weak(get_title),
  /* K41  */  be_nested_str_weak(add_text),
  /* K42  */  be_nested_str_weak(member),
  /* K43  */  be_nested_str_weak(_add_button),
  /* K44  */  be_nested_str_weak(HTP_X3A_X20_X27msgbox_X27_X20needs_X20_X27options_X27_X20to_X20be_X20a_X20list_X20of_X20strings),
  /* K45  */  be_nested_str_weak(add_title),
};


extern const bclass be_class_lvh_msgbox;

/********************************************************************
** Solidified function: post_init
********************************************************************/
be_local_closure(class_lvh_msgbox_post_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_msgbox,     /* shared constants */
    be_str_weak(post_init),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0x8C080300,  //  0000  GETMET	R2	R1	K0
      0x58100001,  //  0001  LDCONST	R4	K1
      0x7C080400,  //  0002  CALL	R2	2
      0x780A0005,  //  0003  JMPF	R2	#000A
      0x8C080102,  //  0004  GETMET	R2	R0	K2
      0x94100301,  //  0005  GETIDX	R4	R1	K1
      0x7C080400,  //  0006  CALL	R2	2
      0x8C080303,  //  0007  GETMET	R2	R1	K3
      0x58100001,  //  0008  LDCONST	R4	K1
      0x7C080400,  //  0009  CALL	R2	2
      0x60080003,  //  000A  GETGBL	R2	G3
      0x5C0C0000,  //  000B  MOVE	R3	R0
      0x7C080200,  //  000C  CALL	R2	1
      0x8C080504,  //  000D  GETMET	R2	R2	K4
      0x5C100200,  //  000E  MOVE	R4	R1
      0x7C080400,  //  000F  CALL	R2	2
      0x80000000,  //  0010  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: setmember
********************************************************************/
be_local_closure(class_lvh_msgbox_setmember,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_msgbox,     /* shared constants */
    be_str_weak(setmember),
    &be_const_str_solidified,
    ( &(const binstruction[93]) {  /* code */
      0xA40E0A00,  //  0000  IMPORT	R3	K5
      0x8C100706,  //  0001  GETMET	R4	R3	K6
      0x5C180200,  //  0002  MOVE	R6	R1
      0x581C0007,  //  0003  LDCONST	R7	K7
      0x7C100600,  //  0004  CALL	R4	3
      0x78120009,  //  0005  JMPF	R4	#0010
      0x88100108,  //  0006  GETMBR	R4	R0	K8
      0x78120007,  //  0007  JMPF	R4	#0010
      0x88100108,  //  0008  GETMBR	R4	R0	K8
      0x8C100909,  //  0009  GETMET	R4	R4	K9
      0x541A0006,  //  000A  LDINT	R6	7
      0x40180D0A,  //  000B  CONNECT	R6	R6	K10
      0x94180206,  //  000C  GETIDX	R6	R1	R6
      0x5C1C0400,  //  000D  MOVE	R7	R2
      0x7C100600,  //  000E  CALL	R4	3
      0x7002004B,  //  000F  JMP		#005C
      0x8C100706,  //  0010  GETMET	R4	R3	K6
      0x5C180200,  //  0011  MOVE	R6	R1
      0x581C000B,  //  0012  LDCONST	R7	K11
      0x7C100600,  //  0013  CALL	R4	3
      0x78120009,  //  0014  JMPF	R4	#001F
      0x8810010C,  //  0015  GETMBR	R4	R0	K12
      0x78120007,  //  0016  JMPF	R4	#001F
      0x8810010C,  //  0017  GETMBR	R4	R0	K12
      0x8C100909,  //  0018  GETMET	R4	R4	K9
      0x541A0006,  //  0019  LDINT	R6	7
      0x40180D0A,  //  001A  CONNECT	R6	R6	K10
      0x94180206,  //  001B  GETIDX	R6	R1	R6
      0x5C1C0400,  //  001C  MOVE	R7	R2
      0x7C100600,  //  001D  CALL	R4	3
      0x7002003C,  //  001E  JMP		#005C
      0x8C100706,  //  001F  GETMET	R4	R3	K6
      0x5C180200,  //  0020  MOVE	R6	R1
      0x581C000D,  //  0021  LDCONST	R7	K13
      0x7C100600,  //  0022  CALL	R4	3
      0x78120009,  //  0023  JMPF	R4	#002E
      0x8810010E,  //  0024  GETMBR	R4	R0	K14
      0x78120007,  //  0025  JMPF	R4	#002E
      0x8810010E,  //  0026  GETMBR	R4	R0	K14
      0x8C100909,  //  0027  GETMET	R4	R4	K9
      0x541A0005,  //  0028  LDINT	R6	6
      0x40180D0A,  //  0029  CONNECT	R6	R6	K10
      0x94180206,  //  002A  GETIDX	R6	R1	R6
      0x5C1C0400,  //  002B  MOVE	R7	R2
      0x7C100600,  //  002C  CALL	R4	3
      0x7002002D,  //  002D  JMP		#005C
      0x8C100706,  //  002E  GETMET	R4	R3	K6
      0x5C180200,  //  002F  MOVE	R6	R1
      0x581C000F,  //  0030  LDCONST	R7	K15
      0x7C100600,  //  0031  CALL	R4	3
      0x78120009,  //  0032  JMPF	R4	#003D
      0x88100110,  //  0033  GETMBR	R4	R0	K16
      0x78120007,  //  0034  JMPF	R4	#003D
      0x88100110,  //  0035  GETMBR	R4	R0	K16
      0x8C100909,  //  0036  GETMET	R4	R4	K9
      0x541A0007,  //  0037  LDINT	R6	8
      0x40180D0A,  //  0038  CONNECT	R6	R6	K10
      0x94180206,  //  0039  GETIDX	R6	R1	R6
      0x5C1C0400,  //  003A  MOVE	R7	R2
      0x7C100600,  //  003B  CALL	R4	3
      0x7002001E,  //  003C  JMP		#005C
      0x8C100706,  //  003D  GETMET	R4	R3	K6
      0x5C180200,  //  003E  MOVE	R6	R1
      0x581C0011,  //  003F  LDCONST	R7	K17
      0x7C100600,  //  0040  CALL	R4	3
      0x78120012,  //  0041  JMPF	R4	#0055
      0x88100112,  //  0042  GETMBR	R4	R0	K18
      0x78120010,  //  0043  JMPF	R4	#0055
      0x60100010,  //  0044  GETGBL	R4	G16
      0x88140112,  //  0045  GETMBR	R5	R0	K18
      0x7C100200,  //  0046  CALL	R4	1
      0xA8020008,  //  0047  EXBLK	0	#0051
      0x5C140800,  //  0048  MOVE	R5	R4
      0x7C140000,  //  0049  CALL	R5	0
      0x8C180B09,  //  004A  GETMET	R6	R5	K9
      0x54220007,  //  004B  LDINT	R8	8
      0x4020110A,  //  004C  CONNECT	R8	R8	K10
      0x94200208,  //  004D  GETIDX	R8	R1	R8
      0x5C240400,  //  004E  MOVE	R9	R2
      0x7C180600,  //  004F  CALL	R6	3
      0x7001FFF6,  //  0050  JMP		#0048
      0x58100013,  //  0051  LDCONST	R4	K19
      0xAC100200,  //  0052  CATCH	R4	1	0
      0xB0080000,  //  0053  RAISE	2	R0	R0
      0x70020006,  //  0054  JMP		#005C
      0x60100003,  //  0055  GETGBL	R4	G3
      0x5C140000,  //  0056  MOVE	R5	R0
      0x7C100200,  //  0057  CALL	R4	1
      0x8C100909,  //  0058  GETMET	R4	R4	K9
      0x5C180200,  //  0059  MOVE	R6	R1
      0x5C1C0400,  //  005A  MOVE	R7	R2
      0x7C100600,  //  005B  CALL	R4	3
      0x80000000,  //  005C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _add_button
********************************************************************/
be_local_closure(class_lvh_msgbox__add_button,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_msgbox,     /* shared constants */
    be_str_weak(_add_button),
    &be_const_str_solidified,
    ( &(const binstruction[20]) {  /* code */
      0x88080114,  //  0000  GETMBR	R2	R0	K20
      0x88080515,  //  0001  GETMBR	R2	R2	K21
      0x88080516,  //  0002  GETMBR	R2	R2	K22
      0x880C0117,  //  0003  GETMBR	R3	R0	K23
      0x8C0C0718,  //  0004  GETMET	R3	R3	K24
      0x5C140200,  //  0005  MOVE	R5	R1
      0x7C0C0400,  //  0006  CALL	R3	2
      0x5C100400,  //  0007  MOVE	R4	R2
      0x4C140000,  //  0008  LDNIL	R5
      0x88180114,  //  0009  GETMBR	R6	R0	K20
      0x601C0013,  //  000A  GETGBL	R7	G19
      0x7C1C0000,  //  000B  CALL	R7	0
      0x5C200600,  //  000C  MOVE	R8	R3
      0x5C240000,  //  000D  MOVE	R9	R0
      0x7C100A00,  //  000E  CALL	R4	5
      0x88140112,  //  000F  GETMBR	R5	R0	K18
      0x8C140B19,  //  0010  GETMET	R5	R5	K25
      0x5C1C0800,  //  0011  MOVE	R7	R4
      0x7C140400,  //  0012  CALL	R5	2
      0x80000000,  //  0013  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_lvh_msgbox_init,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    6,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_msgbox,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[31]) {  /* code */
      0x60180012,  //  0000  GETGBL	R6	G18
      0x7C180000,  //  0001  CALL	R6	0
      0x90022406,  //  0002  SETMBR	R0	K18	R6
      0x60180017,  //  0003  GETGBL	R6	G23
      0x8C1C071B,  //  0004  GETMET	R7	R3	K27
      0x5824001C,  //  0005  LDCONST	R9	K28
      0x50280000,  //  0006  LDBOOL	R10	0	0
      0x7C1C0600,  //  0007  CALL	R7	3
      0x7C180200,  //  0008  CALL	R6	1
      0x90023406,  //  0009  SETMBR	R0	K26	R6
      0x8818011A,  //  000A  GETMBR	R6	R0	K26
      0x781A0004,  //  000B  JMPF	R6	#0011
      0xB81A3A00,  //  000C  GETNGBL	R6	K29
      0x8C180D1E,  //  000D  GETMET	R6	R6	K30
      0x5820001F,  //  000E  LDCONST	R8	K31
      0x7C180400,  //  000F  CALL	R6	2
      0x90022E06,  //  0010  SETMBR	R0	K23	R6
      0x60180003,  //  0011  GETGBL	R6	G3
      0x5C1C0000,  //  0012  MOVE	R7	R0
      0x7C180200,  //  0013  CALL	R6	1
      0x8C180D20,  //  0014  GETMET	R6	R6	K32
      0x5C200200,  //  0015  MOVE	R8	R1
      0x5C240400,  //  0016  MOVE	R9	R2
      0x5C280600,  //  0017  MOVE	R10	R3
      0x882C0117,  //  0018  GETMBR	R11	R0	K23
      0x5C300A00,  //  0019  MOVE	R12	R5
      0x7C180C00,  //  001A  CALL	R6	6
      0x90024322,  //  001B  SETMBR	R0	K33	K34
      0x541A00FE,  //  001C  LDINT	R6	255
      0x90024606,  //  001D  SETMBR	R0	K35	R6
      0x80000000,  //  001E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_options
********************************************************************/
be_local_closure(class_lvh_msgbox_get_options,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_msgbox,     /* shared constants */
    be_str_weak(get_options),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_title
********************************************************************/
be_local_closure(class_lvh_msgbox_get_title,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_msgbox,     /* shared constants */
    be_str_weak(get_title),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: register_event_cb
********************************************************************/
be_local_closure(class_lvh_msgbox_register_event_cb,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_msgbox,     /* shared constants */
    be_str_weak(register_event_cb),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: post_config
********************************************************************/
be_local_closure(class_lvh_msgbox_post_config,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 1]) {
      be_nested_proto(
        8,                          /* nstack */
        1,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 3]) {  /* upvals */
          be_local_const_upval(1, 2),
          be_local_const_upval(1, 1),
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 2]) {     /* constants */
        /* K0   */  be_nested_str_weak(_page),
        /* K1   */  be_nested_str_weak(value_error),
        }),
        be_str_weak(get_obj_safe),
        &be_const_str_solidified,
        ( &(const binstruction[25]) {  /* code */
          0xA802000F,  //  0000  EXBLK	0	#0011
          0x5C040000,  //  0001  MOVE	R1	R0
          0x68080000,  //  0002  GETUPV	R2	U0
          0x7C040200,  //  0003  CALL	R1	1
          0x68080001,  //  0004  GETUPV	R2	U1
          0x4C0C0000,  //  0005  LDNIL	R3
          0x68100002,  //  0006  GETUPV	R4	U2
          0x88100900,  //  0007  GETMBR	R4	R4	K0
          0x60140013,  //  0008  GETGBL	R5	G19
          0x7C140000,  //  0009  CALL	R5	0
          0x5C180200,  //  000A  MOVE	R6	R1
          0x681C0002,  //  000B  GETUPV	R7	U2
          0x7C080A00,  //  000C  CALL	R2	5
          0xA8040001,  //  000D  EXBLK	1	1
          0x80040400,  //  000E  RET	1	R2
          0xA8040001,  //  000F  EXBLK	1	1
          0x70020006,  //  0010  JMP		#0018
          0x58040001,  //  0011  LDCONST	R1	K1
          0xAC040200,  //  0012  CATCH	R1	1	0
          0x70020002,  //  0013  JMP		#0017
          0x4C040000,  //  0014  LDNIL	R1
          0x80040200,  //  0015  RET	1	R1
          0x70020000,  //  0016  JMP		#0018
          0xB0080000,  //  0017  RAISE	2	R0	R0
          0x80000000,  //  0018  RET	0
        })
      ),
    }),
    1,                          /* has constants */
    &be_ktab_class_lvh_msgbox,     /* shared constants */
    be_str_weak(post_config),
    &be_const_str_solidified,
    ( &(const binstruction[28]) {  /* code */
      0x88040114,  //  0000  GETMBR	R1	R0	K20
      0x88040315,  //  0001  GETMBR	R1	R1	K21
      0x88040316,  //  0002  GETMBR	R1	R1	K22
      0x88080117,  //  0003  GETMBR	R2	R0	K23
      0x840C0000,  //  0004  CLOSURE	R3	P0
      0x60100003,  //  0005  GETGBL	R4	G3
      0x5C140000,  //  0006  MOVE	R5	R0
      0x7C100200,  //  0007  CALL	R4	1
      0x8C100924,  //  0008  GETMET	R4	R4	K36
      0x7C100200,  //  0009  CALL	R4	1
      0x5C100600,  //  000A  MOVE	R4	R3
      0x88140525,  //  000B  GETMBR	R5	R2	K37
      0x7C100200,  //  000C  CALL	R4	1
      0x90021804,  //  000D  SETMBR	R0	K12	R4
      0x5C100600,  //  000E  MOVE	R4	R3
      0x88140526,  //  000F  GETMBR	R5	R2	K38
      0x7C100200,  //  0010  CALL	R4	1
      0x90021004,  //  0011  SETMBR	R0	K8	R4
      0x5C100600,  //  0012  MOVE	R4	R3
      0x88140527,  //  0013  GETMBR	R5	R2	K39
      0x7C100200,  //  0014  CALL	R4	1
      0x90022004,  //  0015  SETMBR	R0	K16	R4
      0x5C100600,  //  0016  MOVE	R4	R3
      0x88140528,  //  0017  GETMBR	R5	R2	K40
      0x7C100200,  //  0018  CALL	R4	1
      0x90021C04,  //  0019  SETMBR	R0	K14	R4
      0xA0000000,  //  001A  CLOSE	R0
      0x80000000,  //  001B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_text
********************************************************************/
be_local_closure(class_lvh_msgbox_set_text,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_msgbox,     /* shared constants */
    be_str_weak(set_text),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x88080117,  //  0000  GETMBR	R2	R0	K23
      0x8C080529,  //  0001  GETMET	R2	R2	K41
      0x60100008,  //  0002  GETGBL	R4	G8
      0x5C140200,  //  0003  MOVE	R5	R1
      0x7C100200,  //  0004  CALL	R4	1
      0x7C080400,  //  0005  CALL	R2	2
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: member
********************************************************************/
be_local_closure(class_lvh_msgbox_member,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_msgbox,     /* shared constants */
    be_str_weak(member),
    &be_const_str_solidified,
    ( &(const binstruction[69]) {  /* code */
      0xA40A0A00,  //  0000  IMPORT	R2	K5
      0x8C0C0506,  //  0001  GETMET	R3	R2	K6
      0x5C140200,  //  0002  MOVE	R5	R1
      0x58180007,  //  0003  LDCONST	R6	K7
      0x7C0C0600,  //  0004  CALL	R3	3
      0x780E0009,  //  0005  JMPF	R3	#0010
      0x880C0108,  //  0006  GETMBR	R3	R0	K8
      0x780E0007,  //  0007  JMPF	R3	#0010
      0x880C0108,  //  0008  GETMBR	R3	R0	K8
      0x8C0C072A,  //  0009  GETMET	R3	R3	K42
      0x54160006,  //  000A  LDINT	R5	7
      0x40140B0A,  //  000B  CONNECT	R5	R5	K10
      0x94140205,  //  000C  GETIDX	R5	R1	R5
      0x7C0C0400,  //  000D  CALL	R3	2
      0x80040600,  //  000E  RET	1	R3
      0x70020033,  //  000F  JMP		#0044
      0x8C0C0506,  //  0010  GETMET	R3	R2	K6
      0x5C140200,  //  0011  MOVE	R5	R1
      0x5818000B,  //  0012  LDCONST	R6	K11
      0x7C0C0600,  //  0013  CALL	R3	3
      0x780E0009,  //  0014  JMPF	R3	#001F
      0x880C010C,  //  0015  GETMBR	R3	R0	K12
      0x780E0007,  //  0016  JMPF	R3	#001F
      0x880C010C,  //  0017  GETMBR	R3	R0	K12
      0x8C0C072A,  //  0018  GETMET	R3	R3	K42
      0x54160006,  //  0019  LDINT	R5	7
      0x40140B0A,  //  001A  CONNECT	R5	R5	K10
      0x94140205,  //  001B  GETIDX	R5	R1	R5
      0x7C0C0400,  //  001C  CALL	R3	2
      0x80040600,  //  001D  RET	1	R3
      0x70020024,  //  001E  JMP		#0044
      0x8C0C0506,  //  001F  GETMET	R3	R2	K6
      0x5C140200,  //  0020  MOVE	R5	R1
      0x5818000D,  //  0021  LDCONST	R6	K13
      0x7C0C0600,  //  0022  CALL	R3	3
      0x780E0009,  //  0023  JMPF	R3	#002E
      0x880C010E,  //  0024  GETMBR	R3	R0	K14
      0x780E0007,  //  0025  JMPF	R3	#002E
      0x880C010E,  //  0026  GETMBR	R3	R0	K14
      0x8C0C072A,  //  0027  GETMET	R3	R3	K42
      0x54160005,  //  0028  LDINT	R5	6
      0x40140B0A,  //  0029  CONNECT	R5	R5	K10
      0x94140205,  //  002A  GETIDX	R5	R1	R5
      0x7C0C0400,  //  002B  CALL	R3	2
      0x80040600,  //  002C  RET	1	R3
      0x70020015,  //  002D  JMP		#0044
      0x8C0C0506,  //  002E  GETMET	R3	R2	K6
      0x5C140200,  //  002F  MOVE	R5	R1
      0x5818000F,  //  0030  LDCONST	R6	K15
      0x7C0C0600,  //  0031  CALL	R3	3
      0x780E0009,  //  0032  JMPF	R3	#003D
      0x880C0110,  //  0033  GETMBR	R3	R0	K16
      0x780E0007,  //  0034  JMPF	R3	#003D
      0x880C0110,  //  0035  GETMBR	R3	R0	K16
      0x8C0C072A,  //  0036  GETMET	R3	R3	K42
      0x54160007,  //  0037  LDINT	R5	8
      0x40140B0A,  //  0038  CONNECT	R5	R5	K10
      0x94140205,  //  0039  GETIDX	R5	R1	R5
      0x7C0C0400,  //  003A  CALL	R3	2
      0x80040600,  //  003B  RET	1	R3
      0x70020006,  //  003C  JMP		#0044
      0x600C0003,  //  003D  GETGBL	R3	G3
      0x5C100000,  //  003E  MOVE	R4	R0
      0x7C0C0200,  //  003F  CALL	R3	1
      0x8C0C072A,  //  0040  GETMET	R3	R3	K42
      0x5C140200,  //  0041  MOVE	R5	R1
      0x7C0C0400,  //  0042  CALL	R3	2
      0x80040600,  //  0043  RET	1	R3
      0x80000000,  //  0044  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_options
********************************************************************/
be_local_closure(class_lvh_msgbox_set_options,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_msgbox,     /* shared constants */
    be_str_weak(set_options),
    &be_const_str_solidified,
    ( &(const binstruction[28]) {  /* code */
      0x6008000F,  //  0000  GETGBL	R2	G15
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x60100012,  //  0002  GETGBL	R4	G18
      0x7C080400,  //  0003  CALL	R2	2
      0x780A0012,  //  0004  JMPF	R2	#0018
      0x6008000C,  //  0005  GETGBL	R2	G12
      0x5C0C0200,  //  0006  MOVE	R3	R1
      0x7C080200,  //  0007  CALL	R2	1
      0x2408051F,  //  0008  GT	R2	R2	K31
      0x780A000D,  //  0009  JMPF	R2	#0018
      0x60080010,  //  000A  GETGBL	R2	G16
      0x5C0C0200,  //  000B  MOVE	R3	R1
      0x7C080200,  //  000C  CALL	R2	1
      0xA8020005,  //  000D  EXBLK	0	#0014
      0x5C0C0400,  //  000E  MOVE	R3	R2
      0x7C0C0000,  //  000F  CALL	R3	0
      0x8C10012B,  //  0010  GETMET	R4	R0	K43
      0x5C180600,  //  0011  MOVE	R6	R3
      0x7C100400,  //  0012  CALL	R4	2
      0x7001FFF9,  //  0013  JMP		#000E
      0x58080013,  //  0014  LDCONST	R2	K19
      0xAC080200,  //  0015  CATCH	R2	1	0
      0xB0080000,  //  0016  RAISE	2	R0	R0
      0x70020002,  //  0017  JMP		#001B
      0x60080001,  //  0018  GETGBL	R2	G1
      0x580C002C,  //  0019  LDCONST	R3	K44
      0x7C080200,  //  001A  CALL	R2	1
      0x80000000,  //  001B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_title
********************************************************************/
be_local_closure(class_lvh_msgbox_set_title,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_msgbox,     /* shared constants */
    be_str_weak(set_title),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x88080117,  //  0000  GETMBR	R2	R0	K23
      0x8C08052D,  //  0001  GETMET	R2	R2	K45
      0x60100008,  //  0002  GETGBL	R4	G8
      0x5C140200,  //  0003  MOVE	R5	R1
      0x7C100200,  //  0004  CALL	R4	1
      0x7C080400,  //  0005  CALL	R2	2
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_text
********************************************************************/
be_local_closure(class_lvh_msgbox_get_text,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_msgbox,     /* shared constants */
    be_str_weak(get_text),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: lvh_msgbox
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_class(lvh_msgbox,
    6,
    &be_class_lvh_obj,
    be_nested_map(20,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(_lv_class, -1), be_const_class(be_class_lv_msgbox) },
        { be_const_key_weak(setmember, -1), be_const_closure(class_lvh_msgbox_setmember_closure) },
        { be_const_key_weak(init, -1), be_const_closure(class_lvh_msgbox_init_closure) },
        { be_const_key_weak(set_options, 17), be_const_closure(class_lvh_msgbox_set_options_closure) },
        { be_const_key_weak(member, 16), be_const_closure(class_lvh_msgbox_member_closure) },
        { be_const_key_weak(_footer, -1), be_const_var(2) },
        { be_const_key_weak(_add_button, -1), be_const_closure(class_lvh_msgbox__add_button_closure) },
        { be_const_key_weak(_modal, 0), be_const_var(0) },
        { be_const_key_weak(set_text, 2), be_const_closure(class_lvh_msgbox_set_text_closure) },
        { be_const_key_weak(post_config, -1), be_const_closure(class_lvh_msgbox_post_config_closure) },
        { be_const_key_weak(post_init, 3), be_const_closure(class_lvh_msgbox_post_init_closure) },
        { be_const_key_weak(_buttons, 4), be_const_var(5) },
        { be_const_key_weak(_title, -1), be_const_var(4) },
        { be_const_key_weak(register_event_cb, -1), be_const_closure(class_lvh_msgbox_register_event_cb_closure) },
        { be_const_key_weak(get_options, 9), be_const_closure(class_lvh_msgbox_get_options_closure) },
        { be_const_key_weak(_content, 8), be_const_var(3) },
        { be_const_key_weak(_header, -1), be_const_var(1) },
        { be_const_key_weak(get_title, -1), be_const_closure(class_lvh_msgbox_get_title_closure) },
        { be_const_key_weak(set_title, -1), be_const_closure(class_lvh_msgbox_set_title_closure) },
        { be_const_key_weak(get_text, -1), be_const_closure(class_lvh_msgbox_get_text_closure) },
    })),
    be_str_weak(lvh_msgbox)
);

extern const bclass be_class_lvh_spinner;

/********************************************************************
** Solidified function: set_angle
********************************************************************/
be_local_closure(class_lvh_spinner_set_angle,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    0,                          /* has constants */
    NULL,                       /* no const */
    be_str_weak(set_angle),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_lvh_spinner_init,   /* name */
  be_nested_proto(
    15,                          /* nstack */
    6,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(find),
    /* K1   */  be_nested_str_weak(angle),
    /* K2   */  be_nested_str_weak(speed),
    /* K3   */  be_nested_str_weak(_lv_obj),
    /* K4   */  be_nested_str_weak(lv),
    /* K5   */  be_nested_str_weak(spinner),
    /* K6   */  be_nested_str_weak(set_anim_params),
    /* K7   */  be_nested_str_weak(init),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[29]) {  /* code */
      0x8C180700,  //  0000  GETMET	R6	R3	K0
      0x58200001,  //  0001  LDCONST	R8	K1
      0x5426003B,  //  0002  LDINT	R9	60
      0x7C180600,  //  0003  CALL	R6	3
      0x8C1C0700,  //  0004  GETMET	R7	R3	K0
      0x58240002,  //  0005  LDCONST	R9	K2
      0x542A03E7,  //  0006  LDINT	R10	1000
      0x7C1C0600,  //  0007  CALL	R7	3
      0xB8220800,  //  0008  GETNGBL	R8	K4
      0x8C201105,  //  0009  GETMET	R8	R8	K5
      0x5C280200,  //  000A  MOVE	R10	R1
      0x7C200400,  //  000B  CALL	R8	2
      0x90020608,  //  000C  SETMBR	R0	K3	R8
      0x88200103,  //  000D  GETMBR	R8	R0	K3
      0x8C201106,  //  000E  GETMET	R8	R8	K6
      0x5C280E00,  //  000F  MOVE	R10	R7
      0x5C2C0C00,  //  0010  MOVE	R11	R6
      0x7C200600,  //  0011  CALL	R8	3
      0x60200003,  //  0012  GETGBL	R8	G3
      0x5C240000,  //  0013  MOVE	R9	R0
      0x7C200200,  //  0014  CALL	R8	1
      0x8C201107,  //  0015  GETMET	R8	R8	K7
      0x5C280200,  //  0016  MOVE	R10	R1
      0x5C2C0400,  //  0017  MOVE	R11	R2
      0x5C300600,  //  0018  MOVE	R12	R3
      0x88340103,  //  0019  GETMBR	R13	R0	K3
      0x5C380A00,  //  001A  MOVE	R14	R5
      0x7C200C00,  //  001B  CALL	R8	6
      0x80000000,  //  001C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_speed
********************************************************************/
be_local_closure(class_lvh_spinner_get_speed,   /* name */
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
    be_str_weak(get_speed),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_angle
********************************************************************/
be_local_closure(class_lvh_spinner_get_angle,   /* name */
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
    be_str_weak(get_angle),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_speed
********************************************************************/
be_local_closure(class_lvh_spinner_set_speed,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    0,                          /* has constants */
    NULL,                       /* no const */
    be_str_weak(set_speed),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: lvh_spinner
********************************************************************/
extern const bclass be_class_lvh_arc;
be_local_class(lvh_spinner,
    2,
    &be_class_lvh_arc,
    be_nested_map(8,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(set_speed, 4), be_const_closure(class_lvh_spinner_set_speed_closure) },
        { be_const_key_weak(_speed, -1), be_const_var(0) },
        { be_const_key_weak(get_angle, 6), be_const_closure(class_lvh_spinner_get_angle_closure) },
        { be_const_key_weak(init, 2), be_const_closure(class_lvh_spinner_init_closure) },
        { be_const_key_weak(_lv_class, -1), be_const_class(be_class_lv_spinner) },
        { be_const_key_weak(_angle, -1), be_const_var(1) },
        { be_const_key_weak(get_speed, -1), be_const_closure(class_lvh_spinner_get_speed_closure) },
        { be_const_key_weak(set_angle, 0), be_const_closure(class_lvh_spinner_set_angle_closure) },
    })),
    be_str_weak(lvh_spinner)
);
// compact class 'lvh_img' ktab size: 18, total: 24 (saved 48 bytes)
static const bvalue be_ktab_class_lvh_img[18] = {
  /* K0   */  be_nested_str_weak(tasmota_logo),
  /* K1   */  be_nested_str_weak(_lv_obj),
  /* K2   */  be_nested_str_weak(set_tasmota_logo),
  /* K3   */  be_nested_str_weak(set_src),
  /* K4   */  be_nested_str_weak(set_inner_align),
  /* K5   */  be_nested_str_weak(lv),
  /* K6   */  be_nested_str_weak(IMAGE_ALIGN_STRETCH),
  /* K7   */  be_nested_str_weak(set_angle),
  /* K8   */  be_nested_str_weak(get_angle),
  /* K9   */  be_nested_str_weak(_raw),
  /* K10  */  be_nested_str_weak(fromb64),
  /* K11  */  be_nested_str_weak(lv_image_dsc),
  /* K12  */  be_nested_str_weak(header_cf),
  /* K13  */  be_nested_str_weak(COLOR_FORMAT_RAW),
  /* K14  */  be_nested_str_weak(data_size),
  /* K15  */  be_nested_str_weak(data),
  /* K16  */  be_nested_str_weak(_buffer),
  /* K17  */  be_nested_str_weak(_imd_dsc),
};


extern const bclass be_class_lvh_img;

/********************************************************************
** Solidified function: set_src
********************************************************************/
be_local_closure(class_lvh_img_set_src,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_img,     /* shared constants */
    be_str_weak(set_src),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x1C080300,  //  0000  EQ	R2	R1	K0
      0x780A0003,  //  0001  JMPF	R2	#0006
      0x88080101,  //  0002  GETMBR	R2	R0	K1
      0x8C080502,  //  0003  GETMET	R2	R2	K2
      0x7C080200,  //  0004  CALL	R2	1
      0x70020003,  //  0005  JMP		#000A
      0x88080101,  //  0006  GETMBR	R2	R0	K1
      0x8C080503,  //  0007  GETMET	R2	R2	K3
      0x5C100200,  //  0008  MOVE	R4	R1
      0x7C080400,  //  0009  CALL	R2	2
      0x80000000,  //  000A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_auto_size
********************************************************************/
be_local_closure(class_lvh_img_get_auto_size,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_img,     /* shared constants */
    be_str_weak(get_auto_size),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_auto_size
********************************************************************/
be_local_closure(class_lvh_img_set_auto_size,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_img,     /* shared constants */
    be_str_weak(set_auto_size),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x78060004,  //  0000  JMPF	R1	#0006
      0x88080101,  //  0001  GETMBR	R2	R0	K1
      0x8C080504,  //  0002  GETMET	R2	R2	K4
      0xB8120A00,  //  0003  GETNGBL	R4	K5
      0x88100906,  //  0004  GETMBR	R4	R4	K6
      0x7C080400,  //  0005  CALL	R2	2
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_angle
********************************************************************/
be_local_closure(class_lvh_img_set_angle,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_img,     /* shared constants */
    be_str_weak(set_angle),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x60080009,  //  0000  GETGBL	R2	G9
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x7C080200,  //  0002  CALL	R2	1
      0x5C040400,  //  0003  MOVE	R1	R2
      0x88080101,  //  0004  GETMBR	R2	R0	K1
      0x8C080507,  //  0005  GETMET	R2	R2	K7
      0x5C100200,  //  0006  MOVE	R4	R1
      0x7C080400,  //  0007  CALL	R2	2
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_angle
********************************************************************/
be_local_closure(class_lvh_img_get_angle,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_img,     /* shared constants */
    be_str_weak(get_angle),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040101,  //  0000  GETMBR	R1	R0	K1
      0x8C040308,  //  0001  GETMET	R1	R1	K8
      0x7C040200,  //  0002  CALL	R1	1
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_raw
********************************************************************/
be_local_closure(class_lvh_img_set_raw,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_img,     /* shared constants */
    be_str_weak(set_raw),
    &be_const_str_solidified,
    ( &(const binstruction[26]) {  /* code */
      0x60080015,  //  0000  GETGBL	R2	G21
      0x7C080000,  //  0001  CALL	R2	0
      0x8C08050A,  //  0002  GETMET	R2	R2	K10
      0x5C100200,  //  0003  MOVE	R4	R1
      0x7C080400,  //  0004  CALL	R2	2
      0x90021202,  //  0005  SETMBR	R0	K9	R2
      0xB80A0A00,  //  0006  GETNGBL	R2	K5
      0x8C08050B,  //  0007  GETMET	R2	R2	K11
      0x7C080200,  //  0008  CALL	R2	1
      0xB80E0A00,  //  0009  GETNGBL	R3	K5
      0x880C070D,  //  000A  GETMBR	R3	R3	K13
      0x900A1803,  //  000B  SETMBR	R2	K12	R3
      0x600C000C,  //  000C  GETGBL	R3	G12
      0x88100109,  //  000D  GETMBR	R4	R0	K9
      0x7C0C0200,  //  000E  CALL	R3	1
      0x900A1C03,  //  000F  SETMBR	R2	K14	R3
      0x880C0109,  //  0010  GETMBR	R3	R0	K9
      0x8C0C0710,  //  0011  GETMET	R3	R3	K16
      0x7C0C0200,  //  0012  CALL	R3	1
      0x900A1E03,  //  0013  SETMBR	R2	K15	R3
      0x90022202,  //  0014  SETMBR	R0	K17	R2
      0x880C0101,  //  0015  GETMBR	R3	R0	K1
      0x8C0C0703,  //  0016  GETMET	R3	R3	K3
      0x5C140400,  //  0017  MOVE	R5	R2
      0x7C0C0400,  //  0018  CALL	R3	2
      0x80000000,  //  0019  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: lvh_img
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_class(lvh_img,
    2,
    &be_class_lvh_obj,
    be_nested_map(9,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(set_raw, -1), be_const_closure(class_lvh_img_set_raw_closure) },
        { be_const_key_weak(get_angle, 8), be_const_closure(class_lvh_img_get_angle_closure) },
        { be_const_key_weak(get_auto_size, -1), be_const_closure(class_lvh_img_get_auto_size_closure) },
        { be_const_key_weak(_raw, -1), be_const_var(0) },
        { be_const_key_weak(_imd_dsc, 5), be_const_var(1) },
        { be_const_key_weak(set_auto_size, -1), be_const_closure(class_lvh_img_set_auto_size_closure) },
        { be_const_key_weak(set_angle, 0), be_const_closure(class_lvh_img_set_angle_closure) },
        { be_const_key_weak(set_src, 1), be_const_closure(class_lvh_img_set_src_closure) },
        { be_const_key_weak(_lv_class, -1), be_const_class(be_class_lv_image) },
    })),
    be_str_weak(lvh_img)
);
// compact class 'lvh_qrcode' ktab size: 9, total: 29 (saved 160 bytes)
static const bvalue be_ktab_class_lvh_qrcode[9] = {
  /* K0   */  be_nested_str_weak(qr_text),
  /* K1   */  be_nested_str_weak(_update),
  /* K2   */  be_nested_str_weak(_lv_obj),
  /* K3   */  be_nested_str_weak(set_light_color),
  /* K4   */  be_nested_str_weak(parse_color),
  /* K5   */  be_nested_str_weak(set_size),
  /* K6   */  be_nested_str_weak(set_dark_color),
  /* K7   */  be_nested_str_weak(post_config),
  /* K8   */  be_nested_str_weak(update),
};


extern const bclass be_class_lvh_qrcode;

/********************************************************************
** Solidified function: get_qr_dark_color
********************************************************************/
be_local_closure(class_lvh_qrcode_get_qr_dark_color,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_qrcode,     /* shared constants */
    be_str_weak(get_qr_dark_color),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_qr_text
********************************************************************/
be_local_closure(class_lvh_qrcode_set_qr_text,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_qrcode,     /* shared constants */
    be_str_weak(set_qr_text),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x60080008,  //  0000  GETGBL	R2	G8
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x7C080200,  //  0002  CALL	R2	1
      0x90020002,  //  0003  SETMBR	R0	K0	R2
      0x8C080101,  //  0004  GETMET	R2	R0	K1
      0x7C080200,  //  0005  CALL	R2	1
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_light_color
********************************************************************/
be_local_closure(class_lvh_qrcode_set_light_color,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_qrcode,     /* shared constants */
    be_str_weak(set_light_color),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x88080102,  //  0000  GETMBR	R2	R0	K2
      0x8C080503,  //  0001  GETMET	R2	R2	K3
      0x8C100104,  //  0002  GETMET	R4	R0	K4
      0x5C180200,  //  0003  MOVE	R6	R1
      0x7C100400,  //  0004  CALL	R4	2
      0x7C080400,  //  0005  CALL	R2	2
      0x8C080101,  //  0006  GETMET	R2	R0	K1
      0x7C080200,  //  0007  CALL	R2	1
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_dark_color
********************************************************************/
be_local_closure(class_lvh_qrcode_get_dark_color,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_qrcode,     /* shared constants */
    be_str_weak(get_dark_color),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_qr_light_color
********************************************************************/
be_local_closure(class_lvh_qrcode_set_qr_light_color,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_qrcode,     /* shared constants */
    be_str_weak(set_qr_light_color),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x88080102,  //  0000  GETMBR	R2	R0	K2
      0x8C080503,  //  0001  GETMET	R2	R2	K3
      0x8C100104,  //  0002  GETMET	R4	R0	K4
      0x5C180200,  //  0003  MOVE	R6	R1
      0x7C100400,  //  0004  CALL	R4	2
      0x7C080400,  //  0005  CALL	R2	2
      0x8C080101,  //  0006  GETMET	R2	R0	K1
      0x7C080200,  //  0007  CALL	R2	1
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_qr_light_color
********************************************************************/
be_local_closure(class_lvh_qrcode_get_qr_light_color,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_qrcode,     /* shared constants */
    be_str_weak(get_qr_light_color),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_qr_size
********************************************************************/
be_local_closure(class_lvh_qrcode_set_qr_size,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_qrcode,     /* shared constants */
    be_str_weak(set_qr_size),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x88080102,  //  0000  GETMBR	R2	R0	K2
      0x8C080505,  //  0001  GETMET	R2	R2	K5
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x8C080101,  //  0004  GETMET	R2	R0	K1
      0x7C080200,  //  0005  CALL	R2	1
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_light_color
********************************************************************/
be_local_closure(class_lvh_qrcode_get_light_color,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_qrcode,     /* shared constants */
    be_str_weak(get_light_color),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_size
********************************************************************/
be_local_closure(class_lvh_qrcode_set_size,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_qrcode,     /* shared constants */
    be_str_weak(set_size),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x88080102,  //  0000  GETMBR	R2	R0	K2
      0x8C080505,  //  0001  GETMET	R2	R2	K5
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x8C080101,  //  0004  GETMET	R2	R0	K1
      0x7C080200,  //  0005  CALL	R2	1
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_qr_dark_color
********************************************************************/
be_local_closure(class_lvh_qrcode_set_qr_dark_color,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_qrcode,     /* shared constants */
    be_str_weak(set_qr_dark_color),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x88080102,  //  0000  GETMBR	R2	R0	K2
      0x8C080506,  //  0001  GETMET	R2	R2	K6
      0x8C100104,  //  0002  GETMET	R4	R0	K4
      0x5C180200,  //  0003  MOVE	R6	R1
      0x7C100400,  //  0004  CALL	R4	2
      0x7C080400,  //  0005  CALL	R2	2
      0x8C080101,  //  0006  GETMET	R2	R0	K1
      0x7C080200,  //  0007  CALL	R2	1
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_qr_text
********************************************************************/
be_local_closure(class_lvh_qrcode_get_qr_text,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_qrcode,     /* shared constants */
    be_str_weak(get_qr_text),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_dark_color
********************************************************************/
be_local_closure(class_lvh_qrcode_set_dark_color,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_qrcode,     /* shared constants */
    be_str_weak(set_dark_color),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x88080102,  //  0000  GETMBR	R2	R0	K2
      0x8C080506,  //  0001  GETMET	R2	R2	K6
      0x8C100104,  //  0002  GETMET	R4	R0	K4
      0x5C180200,  //  0003  MOVE	R6	R1
      0x7C100400,  //  0004  CALL	R4	2
      0x7C080400,  //  0005  CALL	R2	2
      0x8C080101,  //  0006  GETMET	R2	R0	K1
      0x7C080200,  //  0007  CALL	R2	1
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_size
********************************************************************/
be_local_closure(class_lvh_qrcode_get_size,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_qrcode,     /* shared constants */
    be_str_weak(get_size),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: post_config
********************************************************************/
be_local_closure(class_lvh_qrcode_post_config,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_qrcode,     /* shared constants */
    be_str_weak(post_config),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x60040003,  //  0000  GETGBL	R1	G3
      0x5C080000,  //  0001  MOVE	R2	R0
      0x7C040200,  //  0002  CALL	R1	1
      0x8C040307,  //  0003  GETMET	R1	R1	K7
      0x7C040200,  //  0004  CALL	R1	1
      0x8C040101,  //  0005  GETMET	R1	R0	K1
      0x7C040200,  //  0006  CALL	R1	1
      0x80000000,  //  0007  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _update
********************************************************************/
be_local_closure(class_lvh_qrcode__update,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_qrcode,     /* shared constants */
    be_str_weak(_update),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x4C080000,  //  0001  LDNIL	R2
      0x20080202,  //  0002  NE	R2	R1	R2
      0x780A0006,  //  0003  JMPF	R2	#000B
      0x88080102,  //  0004  GETMBR	R2	R0	K2
      0x8C080508,  //  0005  GETMET	R2	R2	K8
      0x5C100200,  //  0006  MOVE	R4	R1
      0x6014000C,  //  0007  GETGBL	R5	G12
      0x5C180200,  //  0008  MOVE	R6	R1
      0x7C140200,  //  0009  CALL	R5	1
      0x7C080600,  //  000A  CALL	R2	3
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_qr_size
********************************************************************/
be_local_closure(class_lvh_qrcode_get_qr_size,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_qrcode,     /* shared constants */
    be_str_weak(get_qr_size),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: lvh_qrcode
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_class(lvh_qrcode,
    1,
    &be_class_lvh_obj,
    be_nested_map(18,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(get_qr_dark_color, 5), be_const_closure(class_lvh_qrcode_get_qr_dark_color_closure) },
        { be_const_key_weak(get_qr_size, -1), be_const_closure(class_lvh_qrcode_get_qr_size_closure) },
        { be_const_key_weak(qr_text, 4), be_const_var(0) },
        { be_const_key_weak(_lv_class, -1), be_const_class(be_class_lv_qrcode) },
        { be_const_key_weak(set_qr_light_color, -1), be_const_closure(class_lvh_qrcode_set_qr_light_color_closure) },
        { be_const_key_weak(get_qr_light_color, -1), be_const_closure(class_lvh_qrcode_get_qr_light_color_closure) },
        { be_const_key_weak(get_size, -1), be_const_closure(class_lvh_qrcode_get_size_closure) },
        { be_const_key_weak(set_qr_size, 3), be_const_closure(class_lvh_qrcode_set_qr_size_closure) },
        { be_const_key_weak(get_light_color, -1), be_const_closure(class_lvh_qrcode_get_light_color_closure) },
        { be_const_key_weak(set_size, -1), be_const_closure(class_lvh_qrcode_set_size_closure) },
        { be_const_key_weak(set_qr_dark_color, -1), be_const_closure(class_lvh_qrcode_set_qr_dark_color_closure) },
        { be_const_key_weak(get_qr_text, 6), be_const_closure(class_lvh_qrcode_get_qr_text_closure) },
        { be_const_key_weak(set_light_color, 13), be_const_closure(class_lvh_qrcode_set_light_color_closure) },
        { be_const_key_weak(set_dark_color, 16), be_const_closure(class_lvh_qrcode_set_dark_color_closure) },
        { be_const_key_weak(post_config, -1), be_const_closure(class_lvh_qrcode_post_config_closure) },
        { be_const_key_weak(_update, -1), be_const_closure(class_lvh_qrcode__update_closure) },
        { be_const_key_weak(get_dark_color, -1), be_const_closure(class_lvh_qrcode_get_dark_color_closure) },
        { be_const_key_weak(set_qr_text, 1), be_const_closure(class_lvh_qrcode_set_qr_text_closure) },
    })),
    be_str_weak(lvh_qrcode)
);
// compact class 'lvh_slider' ktab size: 9, total: 14 (saved 40 bytes)
static const bvalue be_ktab_class_lvh_slider[9] = {
  /* K0   */  be_nested_str_weak(_lv_obj),
  /* K1   */  be_nested_str_weak(set_range),
  /* K2   */  be_nested_str_weak(get_min),
  /* K3   */  be_nested_str_weak(get_min_value),
  /* K4   */  be_nested_str_weak(_val),
  /* K5   */  be_nested_str_weak(set_value),
  /* K6   */  be_const_int(0),
  /* K7   */  be_nested_str_weak(get_max_value),
  /* K8   */  be_nested_str_weak(get_max),
};


extern const bclass be_class_lvh_slider;

/********************************************************************
** Solidified function: set_max
********************************************************************/
be_local_closure(class_lvh_slider_set_max,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_slider,     /* shared constants */
    be_str_weak(set_max),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x8C100102,  //  0002  GETMET	R4	R0	K2
      0x7C100200,  //  0003  CALL	R4	1
      0x60140009,  //  0004  GETGBL	R5	G9
      0x5C180200,  //  0005  MOVE	R6	R1
      0x7C140200,  //  0006  CALL	R5	1
      0x7C080600,  //  0007  CALL	R2	3
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_min
********************************************************************/
be_local_closure(class_lvh_slider_get_min,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_slider,     /* shared constants */
    be_str_weak(get_min),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040303,  //  0001  GETMET	R1	R1	K3
      0x7C040200,  //  0002  CALL	R1	1
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_val
********************************************************************/
be_local_closure(class_lvh_slider_set_val,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_slider,     /* shared constants */
    be_str_weak(set_val),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x90020801,  //  0000  SETMBR	R0	K4	R1
      0x88080100,  //  0001  GETMBR	R2	R0	K0
      0x8C080505,  //  0002  GETMET	R2	R2	K5
      0x5C100200,  //  0003  MOVE	R4	R1
      0x58140006,  //  0004  LDCONST	R5	K6
      0x7C080600,  //  0005  CALL	R2	3
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_max
********************************************************************/
be_local_closure(class_lvh_slider_get_max,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_slider,     /* shared constants */
    be_str_weak(get_max),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040307,  //  0001  GETMET	R1	R1	K7
      0x7C040200,  //  0002  CALL	R1	1
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_min
********************************************************************/
be_local_closure(class_lvh_slider_set_min,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_slider,     /* shared constants */
    be_str_weak(set_min),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x60100009,  //  0002  GETGBL	R4	G9
      0x5C140200,  //  0003  MOVE	R5	R1
      0x7C100200,  //  0004  CALL	R4	1
      0x8C140108,  //  0005  GETMET	R5	R0	K8
      0x7C140200,  //  0006  CALL	R5	1
      0x7C080600,  //  0007  CALL	R2	3
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: lvh_slider
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_class(lvh_slider,
    0,
    &be_class_lvh_obj,
    be_nested_map(6,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(set_min, 5), be_const_closure(class_lvh_slider_set_min_closure) },
        { be_const_key_weak(_lv_class, -1), be_const_class(be_class_lv_slider) },
        { be_const_key_weak(get_min, -1), be_const_closure(class_lvh_slider_get_min_closure) },
        { be_const_key_weak(set_val, -1), be_const_closure(class_lvh_slider_set_val_closure) },
        { be_const_key_weak(set_max, 0), be_const_closure(class_lvh_slider_set_max_closure) },
        { be_const_key_weak(get_max, -1), be_const_closure(class_lvh_slider_get_max_closure) },
    })),
    be_str_weak(lvh_slider)
);
// compact class 'lvh_roller' ktab size: 16, total: 20 (saved 32 bytes)
static const bvalue be_ktab_class_lvh_roller[16] = {
  /* K0   */  be_nested_str_weak(_lv_obj),
  /* K1   */  be_nested_str_weak(set_options),
  /* K2   */  be_nested_str_weak(lv),
  /* K3   */  be_nested_str_weak(ROLLER_MODE_NORMAL),
  /* K4   */  be_nested_str_weak(resize),
  /* K5   */  be_nested_str_weak(get_selected_str),
  /* K6   */  be_nested_str_weak(_buffer),
  /* K7   */  be_nested_str_weak(remove_trailing_zeroes),
  /* K8   */  be_nested_str_weak(asstring),
  /* K9   */  be_nested_str_weak(get_options),
  /* K10  */  be_nested_str_weak(_val),
  /* K11  */  be_nested_str_weak(set_selected),
  /* K12  */  be_const_int(0),
  /* K13  */  be_nested_str_weak(attribute_error),
  /* K14  */  be_nested_str_weak(set_text_X20unsupported_X20on_X20roller),
  /* K15  */  be_nested_str_weak(get_selected),
};


extern const bclass be_class_lvh_roller;

/********************************************************************
** Solidified function: set_options
********************************************************************/
be_local_closure(class_lvh_roller_set_options,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_roller,     /* shared constants */
    be_str_weak(set_options),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5C100200,  //  0002  MOVE	R4	R1
      0xB8160400,  //  0003  GETNGBL	R5	K2
      0x88140B03,  //  0004  GETMBR	R5	R5	K3
      0x7C080600,  //  0005  CALL	R2	3
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_text
********************************************************************/
be_local_closure(class_lvh_roller_get_text,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_roller,     /* shared constants */
    be_str_weak(get_text),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x60040015,  //  0000  GETGBL	R1	G21
      0x7C040000,  //  0001  CALL	R1	0
      0x8C040304,  //  0002  GETMET	R1	R1	K4
      0x540E00FF,  //  0003  LDINT	R3	256
      0x7C040400,  //  0004  CALL	R1	2
      0x88080100,  //  0005  GETMBR	R2	R0	K0
      0x8C080505,  //  0006  GETMET	R2	R2	K5
      0x8C100306,  //  0007  GETMET	R4	R1	K6
      0x7C100200,  //  0008  CALL	R4	1
      0x541600FF,  //  0009  LDINT	R5	256
      0x7C080600,  //  000A  CALL	R2	3
      0x8C080107,  //  000B  GETMET	R2	R0	K7
      0x5C100200,  //  000C  MOVE	R4	R1
      0x7C080400,  //  000D  CALL	R2	2
      0x5C040400,  //  000E  MOVE	R1	R2
      0x8C080308,  //  000F  GETMET	R2	R1	K8
      0x7C080200,  //  0010  CALL	R2	1
      0x80040400,  //  0011  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_options
********************************************************************/
be_local_closure(class_lvh_roller_get_options,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_roller,     /* shared constants */
    be_str_weak(get_options),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040309,  //  0001  GETMET	R1	R1	K9
      0x7C040200,  //  0002  CALL	R1	1
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_val
********************************************************************/
be_local_closure(class_lvh_roller_set_val,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_roller,     /* shared constants */
    be_str_weak(set_val),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x90021401,  //  0000  SETMBR	R0	K10	R1
      0x88080100,  //  0001  GETMBR	R2	R0	K0
      0x8C08050B,  //  0002  GETMET	R2	R2	K11
      0x5C100200,  //  0003  MOVE	R4	R1
      0x5814000C,  //  0004  LDCONST	R5	K12
      0x7C080600,  //  0005  CALL	R2	3
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_text
********************************************************************/
be_local_closure(class_lvh_roller_set_text,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_roller,     /* shared constants */
    be_str_weak(set_text),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0xB0061B0E,  //  0000  RAISE	1	K13	K14
      0x80000000,  //  0001  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_val
********************************************************************/
be_local_closure(class_lvh_roller_get_val,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_roller,     /* shared constants */
    be_str_weak(get_val),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C04030F,  //  0001  GETMET	R1	R1	K15
      0x7C040200,  //  0002  CALL	R1	1
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: lvh_roller
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_class(lvh_roller,
    0,
    &be_class_lvh_obj,
    be_nested_map(7,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(set_options, 2), be_const_closure(class_lvh_roller_set_options_closure) },
        { be_const_key_weak(get_text, -1), be_const_closure(class_lvh_roller_get_text_closure) },
        { be_const_key_weak(get_options, -1), be_const_closure(class_lvh_roller_get_options_closure) },
        { be_const_key_weak(_lv_class, -1), be_const_class(be_class_lv_roller) },
        { be_const_key_weak(set_text, -1), be_const_closure(class_lvh_roller_set_text_closure) },
        { be_const_key_weak(set_val, 4), be_const_closure(class_lvh_roller_set_val_closure) },
        { be_const_key_weak(get_val, -1), be_const_closure(class_lvh_roller_get_val_closure) },
    })),
    be_str_weak(lvh_roller)
);
// compact class 'lvh_led' ktab size: 6, total: 8 (saved 16 bytes)
static const bvalue be_ktab_class_lvh_led[6] = {
  /* K0   */  be_nested_str_weak(_val),
  /* K1   */  be_nested_str_weak(_lv_obj),
  /* K2   */  be_nested_str_weak(set_brightness),
  /* K3   */  be_nested_str_weak(get_brightness),
  /* K4   */  be_nested_str_weak(parse_color),
  /* K5   */  be_nested_str_weak(set_color),
};


extern const bclass be_class_lvh_led;

/********************************************************************
** Solidified function: set_val
********************************************************************/
be_local_closure(class_lvh_led_set_val,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_led,     /* shared constants */
    be_str_weak(set_val),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x88080101,  //  0001  GETMBR	R2	R0	K1
      0x8C080502,  //  0002  GETMET	R2	R2	K2
      0x5C100200,  //  0003  MOVE	R4	R1
      0x7C080400,  //  0004  CALL	R2	2
      0x80000000,  //  0005  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_color
********************************************************************/
be_local_closure(class_lvh_led_get_color,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_led,     /* shared constants */
    be_str_weak(get_color),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_val
********************************************************************/
be_local_closure(class_lvh_led_get_val,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_led,     /* shared constants */
    be_str_weak(get_val),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040101,  //  0000  GETMBR	R1	R0	K1
      0x8C040303,  //  0001  GETMET	R1	R1	K3
      0x7C040200,  //  0002  CALL	R1	1
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_color
********************************************************************/
be_local_closure(class_lvh_led_set_color,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_led,     /* shared constants */
    be_str_weak(set_color),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x8C080104,  //  0000  GETMET	R2	R0	K4
      0x5C100200,  //  0001  MOVE	R4	R1
      0x7C080400,  //  0002  CALL	R2	2
      0x880C0101,  //  0003  GETMBR	R3	R0	K1
      0x8C0C0705,  //  0004  GETMET	R3	R3	K5
      0x5C140400,  //  0005  MOVE	R5	R2
      0x7C0C0400,  //  0006  CALL	R3	2
      0x80000000,  //  0007  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: lvh_led
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_class(lvh_led,
    0,
    &be_class_lvh_obj,
    be_nested_map(5,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(set_val, -1), be_const_closure(class_lvh_led_set_val_closure) },
        { be_const_key_weak(get_color, 3), be_const_closure(class_lvh_led_get_color_closure) },
        { be_const_key_weak(_lv_class, -1), be_const_class(be_class_lv_led) },
        { be_const_key_weak(get_val, -1), be_const_closure(class_lvh_led_get_val_closure) },
        { be_const_key_weak(set_color, -1), be_const_closure(class_lvh_led_set_color_closure) },
    })),
    be_str_weak(lvh_led)
);
// compact class 'lvh_dropdown' ktab size: 25, total: 37 (saved 96 bytes)
static const bvalue be_ktab_class_lvh_dropdown[25] = {
  /* K0   */  be_nested_str_weak(_lv_obj),
  /* K1   */  be_nested_str_weak(get_selected),
  /* K2   */  be_nested_str_weak(set_text),
  /* K3   */  be_const_int(0),
  /* K4   */  be_const_int(3),
  /* K5   */  be_nested_str_weak(set_dir),
  /* K6   */  be_nested_str_weak(_dir),
  /* K7   */  be_const_int(1),
  /* K8   */  be_nested_str_weak(_symbol),
  /* K9   */  be_nested_str_weak(lv),
  /* K10  */  be_nested_str_weak(SYMBOL_UP),
  /* K11  */  be_const_int(2),
  /* K12  */  be_nested_str_weak(SYMBOL_LEFT),
  /* K13  */  be_nested_str_weak(SYMBOL_RIGHT),
  /* K14  */  be_nested_str_weak(SYMBOL_DOWN),
  /* K15  */  be_nested_str_weak(set_symbol),
  /* K16  */  be_nested_str_weak(get_dir),
  /* K17  */  be_nested_str_weak(_val),
  /* K18  */  be_nested_str_weak(set_selected),
  /* K19  */  be_nested_str_weak(get_text),
  /* K20  */  be_nested_str_weak(resize),
  /* K21  */  be_nested_str_weak(get_selected_str),
  /* K22  */  be_nested_str_weak(_buffer),
  /* K23  */  be_nested_str_weak(remove_trailing_zeroes),
  /* K24  */  be_nested_str_weak(asstring),
};


extern const bclass be_class_lvh_dropdown;

/********************************************************************
** Solidified function: get_val
********************************************************************/
be_local_closure(class_lvh_dropdown_get_val,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_dropdown,     /* shared constants */
    be_str_weak(get_val),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x7C040200,  //  0002  CALL	R1	1
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_text
********************************************************************/
be_local_closure(class_lvh_dropdown_set_text,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_dropdown,     /* shared constants */
    be_str_weak(set_text),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080502,  //  0001  GETMET	R2	R2	K2
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_direction
********************************************************************/
be_local_closure(class_lvh_dropdown_set_direction,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_dropdown,     /* shared constants */
    be_str_weak(set_direction),
    &be_const_str_solidified,
    ( &(const binstruction[40]) {  /* code */
      0x60080009,  //  0000  GETGBL	R2	G9
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x7C080200,  //  0002  CALL	R2	1
      0x5C040400,  //  0003  MOVE	R1	R2
      0x14080303,  //  0004  LT	R2	R1	K3
      0x740A0001,  //  0005  JMPT	R2	#0008
      0x24080304,  //  0006  GT	R2	R1	K4
      0x780A0000,  //  0007  JMPF	R2	#0009
      0x58040003,  //  0008  LDCONST	R1	K3
      0x88080100,  //  0009  GETMBR	R2	R0	K0
      0x8C080505,  //  000A  GETMET	R2	R2	K5
      0x88100106,  //  000B  GETMBR	R4	R0	K6
      0x94100801,  //  000C  GETIDX	R4	R4	R1
      0x7C080400,  //  000D  CALL	R2	2
      0x1C080307,  //  000E  EQ	R2	R1	K7
      0x780A0003,  //  000F  JMPF	R2	#0014
      0xB80A1200,  //  0010  GETNGBL	R2	K9
      0x8808050A,  //  0011  GETMBR	R2	R2	K10
      0x90021002,  //  0012  SETMBR	R0	K8	R2
      0x7002000E,  //  0013  JMP		#0023
      0x1C08030B,  //  0014  EQ	R2	R1	K11
      0x780A0003,  //  0015  JMPF	R2	#001A
      0xB80A1200,  //  0016  GETNGBL	R2	K9
      0x8808050C,  //  0017  GETMBR	R2	R2	K12
      0x90021002,  //  0018  SETMBR	R0	K8	R2
      0x70020008,  //  0019  JMP		#0023
      0x1C080304,  //  001A  EQ	R2	R1	K4
      0x780A0003,  //  001B  JMPF	R2	#0020
      0xB80A1200,  //  001C  GETNGBL	R2	K9
      0x8808050D,  //  001D  GETMBR	R2	R2	K13
      0x90021002,  //  001E  SETMBR	R0	K8	R2
      0x70020002,  //  001F  JMP		#0023
      0xB80A1200,  //  0020  GETNGBL	R2	K9
      0x8808050E,  //  0021  GETMBR	R2	R2	K14
      0x90021002,  //  0022  SETMBR	R0	K8	R2
      0x88080100,  //  0023  GETMBR	R2	R0	K0
      0x8C08050F,  //  0024  GETMET	R2	R2	K15
      0x88100108,  //  0025  GETMBR	R4	R0	K8
      0x7C080400,  //  0026  CALL	R2	2
      0x80000000,  //  0027  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_direction
********************************************************************/
be_local_closure(class_lvh_dropdown_get_direction,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_dropdown,     /* shared constants */
    be_str_weak(get_direction),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040310,  //  0001  GETMET	R1	R1	K16
      0x7C040200,  //  0002  CALL	R1	1
      0x58080003,  //  0003  LDCONST	R2	K3
      0x600C000C,  //  0004  GETGBL	R3	G12
      0x88100106,  //  0005  GETMBR	R4	R0	K6
      0x7C0C0200,  //  0006  CALL	R3	1
      0x140C0403,  //  0007  LT	R3	R2	R3
      0x780E0006,  //  0008  JMPF	R3	#0010
      0x880C0106,  //  0009  GETMBR	R3	R0	K6
      0x940C0602,  //  000A  GETIDX	R3	R3	R2
      0x1C0C0203,  //  000B  EQ	R3	R1	R3
      0x780E0000,  //  000C  JMPF	R3	#000E
      0x80040400,  //  000D  RET	1	R2
      0x00080507,  //  000E  ADD	R2	R2	K7
      0x7001FFF3,  //  000F  JMP		#0004
      0x540DFFFE,  //  0010  LDINT	R3	-1
      0x80040600,  //  0011  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_show_selected
********************************************************************/
be_local_closure(class_lvh_dropdown_set_show_selected,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_dropdown,     /* shared constants */
    be_str_weak(set_show_selected),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x78060003,  //  0000  JMPF	R1	#0005
      0x88080100,  //  0001  GETMBR	R2	R0	K0
      0x8C080502,  //  0002  GETMET	R2	R2	K2
      0x4C100000,  //  0003  LDNIL	R4
      0x7C080400,  //  0004  CALL	R2	2
      0x80000000,  //  0005  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_val
********************************************************************/
be_local_closure(class_lvh_dropdown_set_val,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_dropdown,     /* shared constants */
    be_str_weak(set_val),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x90022201,  //  0000  SETMBR	R0	K17	R1
      0x88080100,  //  0001  GETMBR	R2	R0	K0
      0x8C080512,  //  0002  GETMET	R2	R2	K18
      0x5C100200,  //  0003  MOVE	R4	R1
      0x7C080400,  //  0004  CALL	R2	2
      0x80000000,  //  0005  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_text
********************************************************************/
be_local_closure(class_lvh_dropdown_get_text,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_dropdown,     /* shared constants */
    be_str_weak(get_text),
    &be_const_str_solidified,
    ( &(const binstruction[27]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040313,  //  0001  GETMET	R1	R1	K19
      0x7C040200,  //  0002  CALL	R1	1
      0x4C080000,  //  0003  LDNIL	R2
      0x1C080202,  //  0004  EQ	R2	R1	R2
      0x780A0012,  //  0005  JMPF	R2	#0019
      0x60080015,  //  0006  GETGBL	R2	G21
      0x7C080000,  //  0007  CALL	R2	0
      0x8C080514,  //  0008  GETMET	R2	R2	K20
      0x541200FF,  //  0009  LDINT	R4	256
      0x7C080400,  //  000A  CALL	R2	2
      0x880C0100,  //  000B  GETMBR	R3	R0	K0
      0x8C0C0715,  //  000C  GETMET	R3	R3	K21
      0x8C140516,  //  000D  GETMET	R5	R2	K22
      0x7C140200,  //  000E  CALL	R5	1
      0x541A00FF,  //  000F  LDINT	R6	256
      0x7C0C0600,  //  0010  CALL	R3	3
      0x8C0C0117,  //  0011  GETMET	R3	R0	K23
      0x5C140400,  //  0012  MOVE	R5	R2
      0x7C0C0400,  //  0013  CALL	R3	2
      0x5C080600,  //  0014  MOVE	R2	R3
      0x8C0C0518,  //  0015  GETMET	R3	R2	K24
      0x7C0C0200,  //  0016  CALL	R3	1
      0x80040600,  //  0017  RET	1	R3
      0x70020000,  //  0018  JMP		#001A
      0x80040200,  //  0019  RET	1	R1
      0x80000000,  //  001A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_show_selected
********************************************************************/
be_local_closure(class_lvh_dropdown_get_show_selected,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_dropdown,     /* shared constants */
    be_str_weak(get_show_selected),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040313,  //  0001  GETMET	R1	R1	K19
      0x7C040200,  //  0002  CALL	R1	1
      0x4C080000,  //  0003  LDNIL	R2
      0x1C080202,  //  0004  EQ	R2	R1	R2
      0x80040400,  //  0005  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: lvh_dropdown
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_class(lvh_dropdown,
    1,
    &be_class_lvh_obj,
    be_nested_map(11,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(get_show_selected, -1), be_const_closure(class_lvh_dropdown_get_show_selected_closure) },
        { be_const_key_weak(get_text, 2), be_const_closure(class_lvh_dropdown_get_text_closure) },
        { be_const_key_weak(set_val, 7), be_const_closure(class_lvh_dropdown_set_val_closure) },
        { be_const_key_weak(_symbol, -1), be_const_var(0) },
        { be_const_key_weak(_lv_class, -1), be_const_class(be_class_lv_dropdown) },
        { be_const_key_weak(set_direction, -1), be_const_closure(class_lvh_dropdown_set_direction_closure) },
        { be_const_key_weak(get_val, 9), be_const_closure(class_lvh_dropdown_get_val_closure) },
        { be_const_key_weak(set_show_selected, -1), be_const_closure(class_lvh_dropdown_set_show_selected_closure) },
        { be_const_key_weak(_dir, 1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(4,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(8),
        be_const_int(4),
        be_const_int(1),
        be_const_int(2),
    }))    ) } )) },
        { be_const_key_weak(get_direction, -1), be_const_closure(class_lvh_dropdown_get_direction_closure) },
        { be_const_key_weak(set_text, 0), be_const_closure(class_lvh_dropdown_set_text_closure) },
    })),
    be_str_weak(lvh_dropdown)
);

extern const bclass be_class_lvh_dropdown_list;

/********************************************************************
** Solidified function: post_init
********************************************************************/
be_local_closure(class_lvh_dropdown_list_post_init,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[11]) {     /* constants */
    /* K0   */  be_nested_str_weak(_lv_obj),
    /* K1   */  be_nested_str_weak(_parent_lvh),
    /* K2   */  be_nested_str_weak(_page),
    /* K3   */  be_nested_str_weak(_hm),
    /* K4   */  be_nested_str_weak(lvh_dropdown),
    /* K5   */  be_nested_str_weak(lv),
    /* K6   */  be_nested_str_weak(list),
    /* K7   */  be_nested_str_weak(get_list),
    /* K8   */  be_nested_str_weak(_p),
    /* K9   */  be_nested_str_weak(HSP_X3A_X20_X27dropdown_list_X27_X20should_X20have_X20a_X20parent_X20of_X20type_X20_X27dropdown_X27),
    /* K10  */  be_nested_str_weak(post_init),
    }),
    be_str_weak(post_init),
    &be_const_str_solidified,
    ( &(const binstruction[29]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x90020002,  //  0001  SETMBR	R0	K0	R2
      0x6008000F,  //  0002  GETGBL	R2	G15
      0x880C0101,  //  0003  GETMBR	R3	R0	K1
      0x88100102,  //  0004  GETMBR	R4	R0	K2
      0x88100903,  //  0005  GETMBR	R4	R4	K3
      0x88100904,  //  0006  GETMBR	R4	R4	K4
      0x7C080400,  //  0007  CALL	R2	2
      0x780A0009,  //  0008  JMPF	R2	#0013
      0xB80A0A00,  //  0009  GETNGBL	R2	K5
      0x8C080506,  //  000A  GETMET	R2	R2	K6
      0x88100101,  //  000B  GETMBR	R4	R0	K1
      0x88100900,  //  000C  GETMBR	R4	R4	K0
      0x8C100907,  //  000D  GETMET	R4	R4	K7
      0x7C100200,  //  000E  CALL	R4	1
      0x88100908,  //  000F  GETMBR	R4	R4	K8
      0x7C080400,  //  0010  CALL	R2	2
      0x90020002,  //  0011  SETMBR	R0	K0	R2
      0x70020002,  //  0012  JMP		#0016
      0x60080001,  //  0013  GETGBL	R2	G1
      0x580C0009,  //  0014  LDCONST	R3	K9
      0x7C080200,  //  0015  CALL	R2	1
      0x60080003,  //  0016  GETGBL	R2	G3
      0x5C0C0000,  //  0017  MOVE	R3	R0
      0x7C080200,  //  0018  CALL	R2	1
      0x8C08050A,  //  0019  GETMET	R2	R2	K10
      0x5C100200,  //  001A  MOVE	R4	R1
      0x7C080400,  //  001B  CALL	R2	2
      0x80000000,  //  001C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: lvh_dropdown_list
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_class(lvh_dropdown_list,
    0,
    &be_class_lvh_obj,
    be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(post_init, -1), be_const_closure(class_lvh_dropdown_list_post_init_closure) },
        { be_const_key_weak(_lv_class, -1), be_const_nil() },
    })),
    be_str_weak(lvh_dropdown_list)
);
// compact class 'lvh_bar' ktab size: 15, total: 24 (saved 72 bytes)
static const bvalue be_ktab_class_lvh_bar[15] = {
  /* K0   */  be_nested_str_weak(_lv_obj),
  /* K1   */  be_nested_str_weak(get_max_value),
  /* K2   */  be_nested_str_weak(set_range),
  /* K3   */  be_nested_str_weak(get_min_value),
  /* K4   */  be_nested_str_weak(post_init),
  /* K5   */  be_nested_str_weak(_parent_lvh),
  /* K6   */  be_nested_str_weak(_page),
  /* K7   */  be_nested_str_weak(_hm),
  /* K8   */  be_nested_str_weak(lvh_scale),
  /* K9   */  be_nested_str_weak(get_range_min_value),
  /* K10  */  be_nested_str_weak(get_range_max_value),
  /* K11  */  be_nested_str_weak(_val),
  /* K12  */  be_nested_str_weak(set_value),
  /* K13  */  be_nested_str_weak(lv),
  /* K14  */  be_nested_str_weak(ANIM_OFF),
};


extern const bclass be_class_lvh_bar;

/********************************************************************
** Solidified function: get_max
********************************************************************/
be_local_closure(class_lvh_bar_get_max,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_bar,     /* shared constants */
    be_str_weak(get_max),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x7C040200,  //  0002  CALL	R1	1
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_max
********************************************************************/
be_local_closure(class_lvh_bar_set_max,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_bar,     /* shared constants */
    be_str_weak(set_max),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080502,  //  0001  GETMET	R2	R2	K2
      0x88100100,  //  0002  GETMBR	R4	R0	K0
      0x8C100903,  //  0003  GETMET	R4	R4	K3
      0x7C100200,  //  0004  CALL	R4	1
      0x60140009,  //  0005  GETGBL	R5	G9
      0x5C180200,  //  0006  MOVE	R6	R1
      0x7C140200,  //  0007  CALL	R5	1
      0x7C080600,  //  0008  CALL	R2	3
      0x80000000,  //  0009  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: post_init
********************************************************************/
be_local_closure(class_lvh_bar_post_init,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_bar,     /* shared constants */
    be_str_weak(post_init),
    &be_const_str_solidified,
    ( &(const binstruction[27]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080504,  //  0003  GETMET	R2	R2	K4
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x6008000F,  //  0006  GETGBL	R2	G15
      0x880C0105,  //  0007  GETMBR	R3	R0	K5
      0x88100106,  //  0008  GETMBR	R4	R0	K6
      0x88100907,  //  0009  GETMBR	R4	R4	K7
      0x88100908,  //  000A  GETMBR	R4	R4	K8
      0x7C080400,  //  000B  CALL	R2	2
      0x780A000C,  //  000C  JMPF	R2	#001A
      0x88080105,  //  000D  GETMBR	R2	R0	K5
      0x88080500,  //  000E  GETMBR	R2	R2	K0
      0x8C080509,  //  000F  GETMET	R2	R2	K9
      0x7C080200,  //  0010  CALL	R2	1
      0x880C0105,  //  0011  GETMBR	R3	R0	K5
      0x880C0700,  //  0012  GETMBR	R3	R3	K0
      0x8C0C070A,  //  0013  GETMET	R3	R3	K10
      0x7C0C0200,  //  0014  CALL	R3	1
      0x88100100,  //  0015  GETMBR	R4	R0	K0
      0x8C100902,  //  0016  GETMET	R4	R4	K2
      0x5C180400,  //  0017  MOVE	R6	R2
      0x5C1C0600,  //  0018  MOVE	R7	R3
      0x7C100600,  //  0019  CALL	R4	3
      0x80000000,  //  001A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_min
********************************************************************/
be_local_closure(class_lvh_bar_get_min,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_bar,     /* shared constants */
    be_str_weak(get_min),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040303,  //  0001  GETMET	R1	R1	K3
      0x7C040200,  //  0002  CALL	R1	1
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_min
********************************************************************/
be_local_closure(class_lvh_bar_set_min,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_bar,     /* shared constants */
    be_str_weak(set_min),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080502,  //  0001  GETMET	R2	R2	K2
      0x60100009,  //  0002  GETGBL	R4	G9
      0x5C140200,  //  0003  MOVE	R5	R1
      0x7C100200,  //  0004  CALL	R4	1
      0x88140100,  //  0005  GETMBR	R5	R0	K0
      0x8C140B01,  //  0006  GETMET	R5	R5	K1
      0x7C140200,  //  0007  CALL	R5	1
      0x7C080600,  //  0008  CALL	R2	3
      0x80000000,  //  0009  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_val
********************************************************************/
be_local_closure(class_lvh_bar_set_val,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_bar,     /* shared constants */
    be_str_weak(set_val),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x90021601,  //  0000  SETMBR	R0	K11	R1
      0x88080100,  //  0001  GETMBR	R2	R0	K0
      0x8C08050C,  //  0002  GETMET	R2	R2	K12
      0x5C100200,  //  0003  MOVE	R4	R1
      0xB8161A00,  //  0004  GETNGBL	R5	K13
      0x88140B0E,  //  0005  GETMBR	R5	R5	K14
      0x7C080600,  //  0006  CALL	R2	3
      0x80000000,  //  0007  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: lvh_bar
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_class(lvh_bar,
    0,
    &be_class_lvh_obj,
    be_nested_map(7,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(set_val, -1), be_const_closure(class_lvh_bar_set_val_closure) },
        { be_const_key_weak(post_init, 6), be_const_closure(class_lvh_bar_post_init_closure) },
        { be_const_key_weak(get_min, -1), be_const_closure(class_lvh_bar_get_min_closure) },
        { be_const_key_weak(_lv_class, 1), be_const_class(be_class_lv_bar) },
        { be_const_key_weak(get_max, 2), be_const_closure(class_lvh_bar_get_max_closure) },
        { be_const_key_weak(set_min, 0), be_const_closure(class_lvh_bar_set_min_closure) },
        { be_const_key_weak(set_max, -1), be_const_closure(class_lvh_bar_set_max_closure) },
    })),
    be_str_weak(lvh_bar)
);

extern const bclass be_class_lvh_line;

/********************************************************************
** Solidified function: set_points
********************************************************************/
be_local_closure(class_lvh_line_set_points,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[14]) {     /* constants */
    /* K0   */  be_const_int(2),
    /* K1   */  be_nested_str_weak(lv),
    /* K2   */  be_nested_str_weak(point),
    /* K3   */  be_nested_str_weak(x),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(y),
    /* K6   */  be_const_int(1),
    /* K7   */  be_nested_str_weak(push),
    /* K8   */  be_nested_str_weak(stop_iteration),
    /* K9   */  be_nested_str_weak(point_arr),
    /* K10  */  be_nested_str_weak(_lv_points),
    /* K11  */  be_nested_str_weak(_lv_obj),
    /* K12  */  be_nested_str_weak(set_points_mutable),
    /* K13  */  be_nested_str_weak(HSP_X3A_X20_X27line_X27_X20wrong_X20format_X20for_X20_X27points_X27_X20_X25s),
    }),
    be_str_weak(set_points),
    &be_const_str_solidified,
    ( &(const binstruction[61]) {  /* code */
      0x6008000F,  //  0000  GETGBL	R2	G15
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x60100012,  //  0002  GETGBL	R4	G18
      0x7C080400,  //  0003  CALL	R2	2
      0x780A0030,  //  0004  JMPF	R2	#0036
      0x60080012,  //  0005  GETGBL	R2	G18
      0x7C080000,  //  0006  CALL	R2	0
      0x600C0010,  //  0007  GETGBL	R3	G16
      0x5C100200,  //  0008  MOVE	R4	R1
      0x7C0C0200,  //  0009  CALL	R3	1
      0xA802001A,  //  000A  EXBLK	0	#0026
      0x5C100600,  //  000B  MOVE	R4	R3
      0x7C100000,  //  000C  CALL	R4	0
      0x6014000F,  //  000D  GETGBL	R5	G15
      0x5C180800,  //  000E  MOVE	R6	R4
      0x601C0012,  //  000F  GETGBL	R7	G18
      0x7C140400,  //  0010  CALL	R5	2
      0x78160012,  //  0011  JMPF	R5	#0025
      0x6014000C,  //  0012  GETGBL	R5	G12
      0x5C180800,  //  0013  MOVE	R6	R4
      0x7C140200,  //  0014  CALL	R5	1
      0x1C140B00,  //  0015  EQ	R5	R5	K0
      0x7816000D,  //  0016  JMPF	R5	#0025
      0xB8160200,  //  0017  GETNGBL	R5	K1
      0x8C140B02,  //  0018  GETMET	R5	R5	K2
      0x7C140200,  //  0019  CALL	R5	1
      0x60180009,  //  001A  GETGBL	R6	G9
      0x941C0904,  //  001B  GETIDX	R7	R4	K4
      0x7C180200,  //  001C  CALL	R6	1
      0x90160606,  //  001D  SETMBR	R5	K3	R6
      0x60180009,  //  001E  GETGBL	R6	G9
      0x941C0906,  //  001F  GETIDX	R7	R4	K6
      0x7C180200,  //  0020  CALL	R6	1
      0x90160A06,  //  0021  SETMBR	R5	K5	R6
      0x8C180507,  //  0022  GETMET	R6	R2	K7
      0x5C200A00,  //  0023  MOVE	R8	R5
      0x7C180400,  //  0024  CALL	R6	2
      0x7001FFE4,  //  0025  JMP		#000B
      0x580C0008,  //  0026  LDCONST	R3	K8
      0xAC0C0200,  //  0027  CATCH	R3	1	0
      0xB0080000,  //  0028  RAISE	2	R0	R0
      0xB80E0200,  //  0029  GETNGBL	R3	K1
      0x8C0C0709,  //  002A  GETMET	R3	R3	K9
      0x5C140400,  //  002B  MOVE	R5	R2
      0x7C0C0400,  //  002C  CALL	R3	2
      0x90021403,  //  002D  SETMBR	R0	K10	R3
      0x8810010B,  //  002E  GETMBR	R4	R0	K11
      0x8C10090C,  //  002F  GETMET	R4	R4	K12
      0x5C180600,  //  0030  MOVE	R6	R3
      0x601C000C,  //  0031  GETGBL	R7	G12
      0x5C200400,  //  0032  MOVE	R8	R2
      0x7C1C0200,  //  0033  CALL	R7	1
      0x7C100600,  //  0034  CALL	R4	3
      0x70020005,  //  0035  JMP		#003C
      0x60080001,  //  0036  GETGBL	R2	G1
      0x600C0018,  //  0037  GETGBL	R3	G24
      0x5810000D,  //  0038  LDCONST	R4	K13
      0x5C140200,  //  0039  MOVE	R5	R1
      0x7C0C0400,  //  003A  CALL	R3	2
      0x7C080200,  //  003B  CALL	R2	1
      0x80000000,  //  003C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: lvh_line
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_class(lvh_line,
    1,
    &be_class_lvh_obj,
    be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(_lv_points, -1), be_const_var(0) },
        { be_const_key_weak(_lv_class, -1), be_const_class(be_class_lv_line) },
        { be_const_key_weak(set_points, -1), be_const_closure(class_lvh_line_set_points_closure) },
    })),
    be_str_weak(lvh_line)
);
// compact class 'lvh_scale' ktab size: 13, total: 21 (saved 64 bytes)
static const bvalue be_ktab_class_lvh_scale[13] = {
  /* K0   */  be_const_int(0),
  /* K1   */  be_nested_str_weak(push),
  /* K2   */  be_nested_str_weak(),
  /* K3   */  be_nested_str_weak(_options),
  /* K4   */  be_nested_str_weak(_options_arr),
  /* K5   */  be_nested_str_weak(lv),
  /* K6   */  be_nested_str_weak(str_arr),
  /* K7   */  be_nested_str_weak(_lv_obj),
  /* K8   */  be_nested_str_weak(set_text_src),
  /* K9   */  be_nested_str_weak(HTP_X3A_X20_X27scale_X27_X20needs_X20_X27text_src_X27_X20to_X20be_X20a_X20list_X20of_X20strings),
  /* K10  */  be_nested_str_weak(get_range_max_value),
  /* K11  */  be_nested_str_weak(set_range),
  /* K12  */  be_nested_str_weak(get_range_min_value),
};


extern const bclass be_class_lvh_scale;

/********************************************************************
** Solidified function: set_text_src
********************************************************************/
be_local_closure(class_lvh_scale_set_text_src,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_scale,     /* shared constants */
    be_str_weak(set_text_src),
    &be_const_str_solidified,
    ( &(const binstruction[34]) {  /* code */
      0x6008000F,  //  0000  GETGBL	R2	G15
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x60100012,  //  0002  GETGBL	R4	G18
      0x7C080400,  //  0003  CALL	R2	2
      0x780A0018,  //  0004  JMPF	R2	#001E
      0x6008000C,  //  0005  GETGBL	R2	G12
      0x5C0C0200,  //  0006  MOVE	R3	R1
      0x7C080200,  //  0007  CALL	R2	1
      0x24080500,  //  0008  GT	R2	R2	K0
      0x780A0013,  //  0009  JMPF	R2	#001E
      0x6008000C,  //  000A  GETGBL	R2	G12
      0x540DFFFE,  //  000B  LDINT	R3	-1
      0x940C0203,  //  000C  GETIDX	R3	R1	R3
      0x7C080200,  //  000D  CALL	R2	1
      0x24080500,  //  000E  GT	R2	R2	K0
      0x780A0002,  //  000F  JMPF	R2	#0013
      0x8C080301,  //  0010  GETMET	R2	R1	K1
      0x58100002,  //  0011  LDCONST	R4	K2
      0x7C080400,  //  0012  CALL	R2	2
      0x90020601,  //  0013  SETMBR	R0	K3	R1
      0xB80A0A00,  //  0014  GETNGBL	R2	K5
      0x8C080506,  //  0015  GETMET	R2	R2	K6
      0x5C100200,  //  0016  MOVE	R4	R1
      0x7C080400,  //  0017  CALL	R2	2
      0x90020802,  //  0018  SETMBR	R0	K4	R2
      0x88080107,  //  0019  GETMBR	R2	R0	K7
      0x8C080508,  //  001A  GETMET	R2	R2	K8
      0x88100104,  //  001B  GETMBR	R4	R0	K4
      0x7C080400,  //  001C  CALL	R2	2
      0x70020002,  //  001D  JMP		#0021
      0x60080001,  //  001E  GETGBL	R2	G1
      0x580C0009,  //  001F  LDCONST	R3	K9
      0x7C080200,  //  0020  CALL	R2	1
      0x80000000,  //  0021  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_max
********************************************************************/
be_local_closure(class_lvh_scale_get_max,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_scale,     /* shared constants */
    be_str_weak(get_max),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040107,  //  0000  GETMBR	R1	R0	K7
      0x8C04030A,  //  0001  GETMET	R1	R1	K10
      0x7C040200,  //  0002  CALL	R1	1
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_min
********************************************************************/
be_local_closure(class_lvh_scale_set_min,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_scale,     /* shared constants */
    be_str_weak(set_min),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x88080107,  //  0000  GETMBR	R2	R0	K7
      0x8C08050B,  //  0001  GETMET	R2	R2	K11
      0x60100009,  //  0002  GETGBL	R4	G9
      0x5C140200,  //  0003  MOVE	R5	R1
      0x7C100200,  //  0004  CALL	R4	1
      0x88140107,  //  0005  GETMBR	R5	R0	K7
      0x8C140B0A,  //  0006  GETMET	R5	R5	K10
      0x7C140200,  //  0007  CALL	R5	1
      0x7C080600,  //  0008  CALL	R2	3
      0x80000000,  //  0009  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_text_src
********************************************************************/
be_local_closure(class_lvh_scale_get_text_src,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_scale,     /* shared constants */
    be_str_weak(get_text_src),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040103,  //  0000  GETMBR	R1	R0	K3
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_max
********************************************************************/
be_local_closure(class_lvh_scale_set_max,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_scale,     /* shared constants */
    be_str_weak(set_max),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x88080107,  //  0000  GETMBR	R2	R0	K7
      0x8C08050B,  //  0001  GETMET	R2	R2	K11
      0x88100107,  //  0002  GETMBR	R4	R0	K7
      0x8C10090C,  //  0003  GETMET	R4	R4	K12
      0x7C100200,  //  0004  CALL	R4	1
      0x60140009,  //  0005  GETGBL	R5	G9
      0x5C180200,  //  0006  MOVE	R6	R1
      0x7C140200,  //  0007  CALL	R5	1
      0x7C080600,  //  0008  CALL	R2	3
      0x80000000,  //  0009  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_min
********************************************************************/
be_local_closure(class_lvh_scale_get_min,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_scale,     /* shared constants */
    be_str_weak(get_min),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040107,  //  0000  GETMBR	R1	R0	K7
      0x8C04030C,  //  0001  GETMET	R1	R1	K12
      0x7C040200,  //  0002  CALL	R1	1
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: lvh_scale
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_class(lvh_scale,
    2,
    &be_class_lvh_obj,
    be_nested_map(9,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(_options_arr, -1), be_const_var(1) },
        { be_const_key_weak(get_max, 0), be_const_closure(class_lvh_scale_get_max_closure) },
        { be_const_key_weak(set_max, -1), be_const_closure(class_lvh_scale_set_max_closure) },
        { be_const_key_weak(get_text_src, -1), be_const_closure(class_lvh_scale_get_text_src_closure) },
        { be_const_key_weak(set_min, -1), be_const_closure(class_lvh_scale_set_min_closure) },
        { be_const_key_weak(_options, -1), be_const_var(0) },
        { be_const_key_weak(set_text_src, 3), be_const_closure(class_lvh_scale_set_text_src_closure) },
        { be_const_key_weak(_lv_class, 2), be_const_class(be_class_lv_scale) },
        { be_const_key_weak(get_min, -1), be_const_closure(class_lvh_scale_get_min_closure) },
    })),
    be_str_weak(lvh_scale)
);
// compact class 'lvh_scale_section' ktab size: 43, total: 58 (saved 120 bytes)
static const bvalue be_ktab_class_lvh_scale_section[43] = {
  /* K0   */  be_nested_str_weak(_style),
  /* K1   */  be_nested_str_weak(del),
  /* K2   */  be_nested_str_weak(_style10),
  /* K3   */  be_nested_str_weak(_style30),
  /* K4   */  be_nested_str_weak(_delete),
  /* K5   */  be_nested_str_weak(_lv_obj),
  /* K6   */  be_nested_str_weak(_min),
  /* K7   */  be_const_int(0),
  /* K8   */  be_nested_str_weak(_max),
  /* K9   */  be_nested_str_weak(_parent_lvh),
  /* K10  */  be_nested_str_weak(_page),
  /* K11  */  be_nested_str_weak(_hm),
  /* K12  */  be_nested_str_weak(lvh_scale),
  /* K13  */  be_nested_str_weak(add_section),
  /* K14  */  be_nested_str_weak(lv),
  /* K15  */  be_nested_str_weak(style),
  /* K16  */  be_nested_str_weak(set_style),
  /* K17  */  be_nested_str_weak(PART_MAIN),
  /* K18  */  be_nested_str_weak(PART_INDICATOR),
  /* K19  */  be_nested_str_weak(PART_ITEMS),
  /* K20  */  be_nested_str_weak(HSP_X3A_X20_X27scale_section_X27_X20should_X20have_X20a_X20parent_X20of_X20type_X20_X27scale_X27),
  /* K21  */  be_nested_str_weak(set_range),
  /* K22  */  be_nested_str_weak(string),
  /* K23  */  be_nested_str_weak(introspect),
  /* K24  */  be_nested_str_weak(startswith),
  /* K25  */  be_nested_str_weak(set_),
  /* K26  */  be_nested_str_weak(get_),
  /* K27  */  be_nested_str_weak(endswith),
  /* K28  */  be_nested_str_weak(_X25),
  /* K29  */  be_nested_str_weak(pct),
  /* K30  */  be_const_int(3),
  /* K31  */  be_nested_str_weak(byte),
  /* K32  */  be_const_int(2147483647),
  /* K33  */  be_nested_str_weak(value_error),
  /* K34  */  be_nested_str_weak(only_X20modifiers_X20_X2710_X27_X20or_X20_X2730_X27_X20allowed),
  /* K35  */  be_nested_str_weak(_attr_ignore),
  /* K36  */  be_nested_str_weak(find),
  /* K37  */  be_nested_str_weak(get),
  /* K38  */  be_nested_str_weak(function),
  /* K39  */  be_nested_str_weak(is_color_attribute),
  /* K40  */  be_nested_str_weak(parse_color),
  /* K41  */  be_nested_str_weak(_X20for_X20),
  /* K42  */  be_nested_str_weak(HSP_X3A_X20Could_X20not_X20find_X20function_X20set_),
};


extern const bclass be_class_lvh_scale_section;

/********************************************************************
** Solidified function: _delete
********************************************************************/
be_local_closure(class_lvh_scale_section__delete,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_scale_section,     /* shared constants */
    be_str_weak(_delete),
    &be_const_str_solidified,
    ( &(const binstruction[21]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x7C040200,  //  0002  CALL	R1	1
      0x4C040000,  //  0003  LDNIL	R1
      0x90020001,  //  0004  SETMBR	R0	K0	R1
      0x88040102,  //  0005  GETMBR	R1	R0	K2
      0x8C040301,  //  0006  GETMET	R1	R1	K1
      0x7C040200,  //  0007  CALL	R1	1
      0x4C040000,  //  0008  LDNIL	R1
      0x90020401,  //  0009  SETMBR	R0	K2	R1
      0x88040103,  //  000A  GETMBR	R1	R0	K3
      0x8C040301,  //  000B  GETMET	R1	R1	K1
      0x7C040200,  //  000C  CALL	R1	1
      0x4C040000,  //  000D  LDNIL	R1
      0x90020601,  //  000E  SETMBR	R0	K3	R1
      0x60040003,  //  000F  GETGBL	R1	G3
      0x5C080000,  //  0010  MOVE	R2	R0
      0x7C040200,  //  0011  CALL	R1	1
      0x8C040304,  //  0012  GETMET	R1	R1	K4
      0x7C040200,  //  0013  CALL	R1	1
      0x80000000,  //  0014  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: post_init
********************************************************************/
be_local_closure(class_lvh_scale_section_post_init,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_scale_section,     /* shared constants */
    be_str_weak(post_init),
    &be_const_str_solidified,
    ( &(const binstruction[51]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x90020A02,  //  0001  SETMBR	R0	K5	R2
      0x90020D07,  //  0002  SETMBR	R0	K6	K7
      0x90021107,  //  0003  SETMBR	R0	K8	K7
      0x6008000F,  //  0004  GETGBL	R2	G15
      0x880C0109,  //  0005  GETMBR	R3	R0	K9
      0x8810010A,  //  0006  GETMBR	R4	R0	K10
      0x8810090B,  //  0007  GETMBR	R4	R4	K11
      0x8810090C,  //  0008  GETMBR	R4	R4	K12
      0x7C080400,  //  0009  CALL	R2	2
      0x780A0023,  //  000A  JMPF	R2	#002F
      0x88080109,  //  000B  GETMBR	R2	R0	K9
      0x88080505,  //  000C  GETMBR	R2	R2	K5
      0x8C08050D,  //  000D  GETMET	R2	R2	K13
      0x7C080200,  //  000E  CALL	R2	1
      0x90020A02,  //  000F  SETMBR	R0	K5	R2
      0xB80A1C00,  //  0010  GETNGBL	R2	K14
      0x8C08050F,  //  0011  GETMET	R2	R2	K15
      0x7C080200,  //  0012  CALL	R2	1
      0x90020002,  //  0013  SETMBR	R0	K0	R2
      0x88080105,  //  0014  GETMBR	R2	R0	K5
      0x8C080510,  //  0015  GETMET	R2	R2	K16
      0xB8121C00,  //  0016  GETNGBL	R4	K14
      0x88100911,  //  0017  GETMBR	R4	R4	K17
      0x88140100,  //  0018  GETMBR	R5	R0	K0
      0x7C080600,  //  0019  CALL	R2	3
      0xB80A1C00,  //  001A  GETNGBL	R2	K14
      0x8C08050F,  //  001B  GETMET	R2	R2	K15
      0x7C080200,  //  001C  CALL	R2	1
      0x90020402,  //  001D  SETMBR	R0	K2	R2
      0x88080105,  //  001E  GETMBR	R2	R0	K5
      0x8C080510,  //  001F  GETMET	R2	R2	K16
      0xB8121C00,  //  0020  GETNGBL	R4	K14
      0x88100912,  //  0021  GETMBR	R4	R4	K18
      0x88140102,  //  0022  GETMBR	R5	R0	K2
      0x7C080600,  //  0023  CALL	R2	3
      0xB80A1C00,  //  0024  GETNGBL	R2	K14
      0x8C08050F,  //  0025  GETMET	R2	R2	K15
      0x7C080200,  //  0026  CALL	R2	1
      0x90020602,  //  0027  SETMBR	R0	K3	R2
      0x88080105,  //  0028  GETMBR	R2	R0	K5
      0x8C080510,  //  0029  GETMET	R2	R2	K16
      0xB8121C00,  //  002A  GETNGBL	R4	K14
      0x88100913,  //  002B  GETMBR	R4	R4	K19
      0x88140103,  //  002C  GETMBR	R5	R0	K3
      0x7C080600,  //  002D  CALL	R2	3
      0x70020002,  //  002E  JMP		#0032
      0x60080001,  //  002F  GETGBL	R2	G1
      0x580C0014,  //  0030  LDCONST	R3	K20
      0x7C080200,  //  0031  CALL	R2	1
      0x80000000,  //  0032  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_min
********************************************************************/
be_local_closure(class_lvh_scale_section_set_min,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_scale_section,     /* shared constants */
    be_str_weak(set_min),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x60080009,  //  0000  GETGBL	R2	G9
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x7C080200,  //  0002  CALL	R2	1
      0x880C0108,  //  0003  GETMBR	R3	R0	K8
      0x14100602,  //  0004  LT	R4	R3	R2
      0x78120000,  //  0005  JMPF	R4	#0007
      0x5C0C0400,  //  0006  MOVE	R3	R2
      0x8C100115,  //  0007  GETMET	R4	R0	K21
      0x5C180400,  //  0008  MOVE	R6	R2
      0x5C1C0600,  //  0009  MOVE	R7	R3
      0x7C100600,  //  000A  CALL	R4	3
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_range
********************************************************************/
be_local_closure(class_lvh_scale_section_set_range,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_scale_section,     /* shared constants */
    be_str_weak(set_range),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x90020C01,  //  0000  SETMBR	R0	K6	R1
      0x90021002,  //  0001  SETMBR	R0	K8	R2
      0x880C0105,  //  0002  GETMBR	R3	R0	K5
      0x8C0C0715,  //  0003  GETMET	R3	R3	K21
      0x5C140200,  //  0004  MOVE	R5	R1
      0x5C180400,  //  0005  MOVE	R6	R2
      0x7C0C0600,  //  0006  CALL	R3	3
      0x80000000,  //  0007  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_max
********************************************************************/
be_local_closure(class_lvh_scale_section_set_max,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_scale_section,     /* shared constants */
    be_str_weak(set_max),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x88080106,  //  0000  GETMBR	R2	R0	K6
      0x600C0009,  //  0001  GETGBL	R3	G9
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C0C0200,  //  0003  CALL	R3	1
      0x24100403,  //  0004  GT	R4	R2	R3
      0x78120000,  //  0005  JMPF	R4	#0007
      0x5C080600,  //  0006  MOVE	R2	R3
      0x8C100115,  //  0007  GETMET	R4	R0	K21
      0x5C180400,  //  0008  MOVE	R6	R2
      0x5C1C0600,  //  0009  MOVE	R7	R3
      0x7C100600,  //  000A  CALL	R4	3
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: setmember
********************************************************************/
be_local_closure(class_lvh_scale_section_setmember,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_scale_section,     /* shared constants */
    be_str_weak(setmember),
    &be_const_str_solidified,
    ( &(const binstruction[141]) {  /* code */
      0xA40E2C00,  //  0000  IMPORT	R3	K22
      0xA4122E00,  //  0001  IMPORT	R4	K23
      0x8C140718,  //  0002  GETMET	R5	R3	K24
      0x5C1C0200,  //  0003  MOVE	R7	R1
      0x58200019,  //  0004  LDCONST	R8	K25
      0x7C140600,  //  0005  CALL	R5	3
      0x74160004,  //  0006  JMPT	R5	#000C
      0x8C140718,  //  0007  GETMET	R5	R3	K24
      0x5C1C0200,  //  0008  MOVE	R7	R1
      0x5820001A,  //  0009  LDCONST	R8	K26
      0x7C140600,  //  000A  CALL	R5	3
      0x78160000,  //  000B  JMPF	R5	#000D
      0x80000A00,  //  000C  RET	0
      0x8C14071B,  //  000D  GETMET	R5	R3	K27
      0x5C1C0200,  //  000E  MOVE	R7	R1
      0x5820001C,  //  000F  LDCONST	R8	K28
      0x7C140600,  //  0010  CALL	R5	3
      0x78160009,  //  0011  JMPF	R5	#001C
      0x5415FFFD,  //  0012  LDINT	R5	-2
      0x40160E05,  //  0013  CONNECT	R5	K7	R5
      0x94040205,  //  0014  GETIDX	R1	R1	R5
      0xB8161C00,  //  0015  GETNGBL	R5	K14
      0x8C140B1D,  //  0016  GETMET	R5	R5	K29
      0x601C0009,  //  0017  GETGBL	R7	G9
      0x5C200400,  //  0018  MOVE	R8	R2
      0x7C1C0200,  //  0019  CALL	R7	1
      0x7C140400,  //  001A  CALL	R5	2
      0x5C080A00,  //  001B  MOVE	R2	R5
      0x4C140000,  //  001C  LDNIL	R5
      0x6018000C,  //  001D  GETGBL	R6	G12
      0x5C1C0200,  //  001E  MOVE	R7	R1
      0x7C180200,  //  001F  CALL	R6	1
      0x28180D1E,  //  0020  GE	R6	R6	K30
      0x781A0023,  //  0021  JMPF	R6	#0046
      0x8C18071F,  //  0022  GETMET	R6	R3	K31
      0x5421FFFE,  //  0023  LDINT	R8	-1
      0x94200208,  //  0024  GETIDX	R8	R1	R8
      0x7C180400,  //  0025  CALL	R6	2
      0x8C1C071F,  //  0026  GETMET	R7	R3	K31
      0x5425FFFD,  //  0027  LDINT	R9	-2
      0x94240209,  //  0028  GETIDX	R9	R1	R9
      0x7C1C0400,  //  0029  CALL	R7	2
      0x5422002F,  //  002A  LDINT	R8	48
      0x28200C08,  //  002B  GE	R8	R6	R8
      0x78220018,  //  002C  JMPF	R8	#0046
      0x54220038,  //  002D  LDINT	R8	57
      0x18200C08,  //  002E  LE	R8	R6	R8
      0x78220015,  //  002F  JMPF	R8	#0046
      0x5422002F,  //  0030  LDINT	R8	48
      0x28200E08,  //  0031  GE	R8	R7	R8
      0x78220012,  //  0032  JMPF	R8	#0046
      0x54220038,  //  0033  LDINT	R8	57
      0x18200E08,  //  0034  LE	R8	R7	R8
      0x7822000F,  //  0035  JMPF	R8	#0046
      0x60200009,  //  0036  GETGBL	R8	G9
      0x5425FFFD,  //  0037  LDINT	R9	-2
      0x40241320,  //  0038  CONNECT	R9	R9	K32
      0x94240209,  //  0039  GETIDX	R9	R1	R9
      0x7C200200,  //  003A  CALL	R8	1
      0x5C141000,  //  003B  MOVE	R5	R8
      0x5421FFFC,  //  003C  LDINT	R8	-3
      0x40220E08,  //  003D  CONNECT	R8	K7	R8
      0x94040208,  //  003E  GETIDX	R1	R1	R8
      0x54220009,  //  003F  LDINT	R8	10
      0x20200A08,  //  0040  NE	R8	R5	R8
      0x78220003,  //  0041  JMPF	R8	#0046
      0x5422001D,  //  0042  LDINT	R8	30
      0x20200A08,  //  0043  NE	R8	R5	R8
      0x78220000,  //  0044  JMPF	R8	#0046
      0xB0064322,  //  0045  RAISE	1	K33	K34
      0x88180123,  //  0046  GETMBR	R6	R0	K35
      0x8C180D24,  //  0047  GETMET	R6	R6	K36
      0x5C200200,  //  0048  MOVE	R8	R1
      0x7C180400,  //  0049  CALL	R6	2
      0x4C1C0000,  //  004A  LDNIL	R7
      0x20180C07,  //  004B  NE	R6	R6	R7
      0x781A0000,  //  004C  JMPF	R6	#004E
      0x80000C00,  //  004D  RET	0
      0x88180100,  //  004E  GETMBR	R6	R0	K0
      0x541E0009,  //  004F  LDINT	R7	10
      0x1C1C0A07,  //  0050  EQ	R7	R5	R7
      0x781E0001,  //  0051  JMPF	R7	#0054
      0x88180102,  //  0052  GETMBR	R6	R0	K2
      0x70020003,  //  0053  JMP		#0058
      0x541E001D,  //  0054  LDINT	R7	30
      0x1C1C0A07,  //  0055  EQ	R7	R5	R7
      0x781E0000,  //  0056  JMPF	R7	#0058
      0x88180103,  //  0057  GETMBR	R6	R0	K3
      0x8C1C0925,  //  0058  GETMET	R7	R4	K37
      0x5C240000,  //  0059  MOVE	R9	R0
      0x002A3201,  //  005A  ADD	R10	K25	R1
      0x7C1C0600,  //  005B  CALL	R7	3
      0x60200004,  //  005C  GETGBL	R8	G4
      0x5C240E00,  //  005D  MOVE	R9	R7
      0x7C200200,  //  005E  CALL	R8	1
      0x1C201126,  //  005F  EQ	R8	R8	K38
      0x78220004,  //  0060  JMPF	R8	#0066
      0x5C200E00,  //  0061  MOVE	R8	R7
      0x5C240000,  //  0062  MOVE	R9	R0
      0x5C280400,  //  0063  MOVE	R10	R2
      0x7C200400,  //  0064  CALL	R8	2
      0x80001000,  //  0065  RET	0
      0x8C200925,  //  0066  GETMET	R8	R4	K37
      0x5C280C00,  //  0067  MOVE	R10	R6
      0x002E3201,  //  0068  ADD	R11	K25	R1
      0x7C200600,  //  0069  CALL	R8	3
      0x5C1C1000,  //  006A  MOVE	R7	R8
      0x60200004,  //  006B  GETGBL	R8	G4
      0x5C240E00,  //  006C  MOVE	R9	R7
      0x7C200200,  //  006D  CALL	R8	1
      0x1C201126,  //  006E  EQ	R8	R8	K38
      0x78220018,  //  006F  JMPF	R8	#0089
      0x8C200127,  //  0070  GETMET	R8	R0	K39
      0x5C280200,  //  0071  MOVE	R10	R1
      0x7C200400,  //  0072  CALL	R8	2
      0x78220003,  //  0073  JMPF	R8	#0078
      0x8C200128,  //  0074  GETMET	R8	R0	K40
      0x5C280400,  //  0075  MOVE	R10	R2
      0x7C200400,  //  0076  CALL	R8	2
      0x5C081000,  //  0077  MOVE	R2	R8
      0xA8020005,  //  0078  EXBLK	0	#007F
      0x5C200E00,  //  0079  MOVE	R8	R7
      0x5C240C00,  //  007A  MOVE	R9	R6
      0x5C280400,  //  007B  MOVE	R10	R2
      0x7C200400,  //  007C  CALL	R8	2
      0xA8040001,  //  007D  EXBLK	1	1
      0x70020006,  //  007E  JMP		#0086
      0xAC200002,  //  007F  CATCH	R8	0	2
      0x70020003,  //  0080  JMP		#0085
      0x00281329,  //  0081  ADD	R10	R9	K41
      0x00281401,  //  0082  ADD	R10	R10	R1
      0xB004100A,  //  0083  RAISE	1	R8	R10
      0x70020000,  //  0084  JMP		#0086
      0xB0080000,  //  0085  RAISE	2	R0	R0
      0x4C200000,  //  0086  LDNIL	R8
      0x80041000,  //  0087  RET	1	R8
      0x70020002,  //  0088  JMP		#008C
      0x60200001,  //  0089  GETGBL	R8	G1
      0x00265401,  //  008A  ADD	R9	K42	R1
      0x7C200200,  //  008B  CALL	R8	1
      0x80000000,  //  008C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: lvh_scale_section
********************************************************************/
extern const bclass be_class_lvh_root;
be_local_class(lvh_scale_section,
    5,
    &be_class_lvh_root,
    be_nested_map(12,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(_style30, -1), be_const_var(2) },
        { be_const_key_weak(_delete, 3), be_const_closure(class_lvh_scale_section__delete_closure) },
        { be_const_key_weak(post_init, 5), be_const_closure(class_lvh_scale_section_post_init_closure) },
        { be_const_key_weak(setmember, -1), be_const_closure(class_lvh_scale_section_setmember_closure) },
        { be_const_key_weak(_max, 11), be_const_var(4) },
        { be_const_key_weak(_style10, -1), be_const_var(1) },
        { be_const_key_weak(_min, -1), be_const_var(3) },
        { be_const_key_weak(_style, 8), be_const_var(0) },
        { be_const_key_weak(_lv_class, -1), be_const_nil() },
        { be_const_key_weak(set_range, -1), be_const_closure(class_lvh_scale_section_set_range_closure) },
        { be_const_key_weak(set_max, -1), be_const_closure(class_lvh_scale_section_set_max_closure) },
        { be_const_key_weak(set_min, -1), be_const_closure(class_lvh_scale_section_set_min_closure) },
    })),
    be_str_weak(lvh_scale_section)
);
// compact class 'lvh_scale_line' ktab size: 21, total: 29 (saved 64 bytes)
static const bvalue be_ktab_class_lvh_scale_line[21] = {
  /* K0   */  be_nested_str_weak(set_val),
  /* K1   */  be_nested_str_weak(_parent_lvh),
  /* K2   */  be_nested_str_weak(_lv_obj),
  /* K3   */  be_nested_str_weak(set_line_needle_value),
  /* K4   */  be_nested_str_weak(_needle_length),
  /* K5   */  be_nested_str_weak(_val),
  /* K6   */  be_nested_str_weak(get_point_count),
  /* K7   */  be_const_int(2),
  /* K8   */  be_nested_str_weak(get_points),
  /* K9   */  be_nested_str_weak(_lv_points),
  /* K10  */  be_nested_str_weak(setbytes),
  /* K11  */  be_const_int(0),
  /* K12  */  be_nested_str_weak(set_points_mutable),
  /* K13  */  be_nested_str_weak(_page),
  /* K14  */  be_nested_str_weak(_hm),
  /* K15  */  be_nested_str_weak(lvh_scale),
  /* K16  */  be_nested_str_weak(HSP_X3A_X20_X27scale_line_X27_X20should_X20have_X20a_X20parent_X20of_X20type_X20_X27scale_X27),
  /* K17  */  be_nested_str_weak(lv),
  /* K18  */  be_nested_str_weak(point_arr),
  /* K19  */  be_nested_str_weak(point),
  /* K20  */  be_nested_str_weak(post_init),
};


extern const bclass be_class_lvh_scale_line;

/********************************************************************
** Solidified function: set_val
********************************************************************/
be_local_closure(class_lvh_scale_line_set_val,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_scale_line,     /* shared constants */
    be_str_weak(set_val),
    &be_const_str_solidified,
    ( &(const binstruction[37]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080500,  //  0003  GETMET	R2	R2	K0
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x88080101,  //  0006  GETMBR	R2	R0	K1
      0x88080502,  //  0007  GETMBR	R2	R2	K2
      0x8C080503,  //  0008  GETMET	R2	R2	K3
      0x88100102,  //  0009  GETMBR	R4	R0	K2
      0x88140104,  //  000A  GETMBR	R5	R0	K4
      0x88180105,  //  000B  GETMBR	R6	R0	K5
      0x7C080800,  //  000C  CALL	R2	4
      0x88080102,  //  000D  GETMBR	R2	R0	K2
      0x8C080506,  //  000E  GETMET	R2	R2	K6
      0x7C080200,  //  000F  CALL	R2	1
      0x1C080507,  //  0010  EQ	R2	R2	K7
      0x780A0011,  //  0011  JMPF	R2	#0024
      0x60080015,  //  0012  GETGBL	R2	G21
      0x880C0102,  //  0013  GETMBR	R3	R0	K2
      0x8C0C0708,  //  0014  GETMET	R3	R3	K8
      0x7C0C0200,  //  0015  CALL	R3	1
      0x6010000C,  //  0016  GETGBL	R4	G12
      0x88140109,  //  0017  GETMBR	R5	R0	K9
      0x7C100200,  //  0018  CALL	R4	1
      0x7C080400,  //  0019  CALL	R2	2
      0x880C0109,  //  001A  GETMBR	R3	R0	K9
      0x8C0C070A,  //  001B  GETMET	R3	R3	K10
      0x5814000B,  //  001C  LDCONST	R5	K11
      0x5C180400,  //  001D  MOVE	R6	R2
      0x7C0C0600,  //  001E  CALL	R3	3
      0x880C0102,  //  001F  GETMBR	R3	R0	K2
      0x8C0C070C,  //  0020  GETMET	R3	R3	K12
      0x88140109,  //  0021  GETMBR	R5	R0	K9
      0x58180007,  //  0022  LDCONST	R6	K7
      0x7C0C0600,  //  0023  CALL	R3	3
      0x80000000,  //  0024  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: post_init
********************************************************************/
be_local_closure(class_lvh_scale_line_post_init,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_scale_line,     /* shared constants */
    be_str_weak(post_init),
    &be_const_str_solidified,
    ( &(const binstruction[32]) {  /* code */
      0x6008000F,  //  0000  GETGBL	R2	G15
      0x880C0101,  //  0001  GETMBR	R3	R0	K1
      0x8810010D,  //  0002  GETMBR	R4	R0	K13
      0x8810090E,  //  0003  GETMBR	R4	R4	K14
      0x8810090F,  //  0004  GETMBR	R4	R4	K15
      0x7C080400,  //  0005  CALL	R2	2
      0x740A0002,  //  0006  JMPT	R2	#000A
      0x60080001,  //  0007  GETGBL	R2	G1
      0x580C0010,  //  0008  LDCONST	R3	K16
      0x7C080200,  //  0009  CALL	R2	1
      0x9002090B,  //  000A  SETMBR	R0	K4	K11
      0xB80A2200,  //  000B  GETNGBL	R2	K17
      0x8C080512,  //  000C  GETMET	R2	R2	K18
      0x60100012,  //  000D  GETGBL	R4	G18
      0x7C100000,  //  000E  CALL	R4	0
      0xB8162200,  //  000F  GETNGBL	R5	K17
      0x8C140B13,  //  0010  GETMET	R5	R5	K19
      0x7C140200,  //  0011  CALL	R5	1
      0x40140805,  //  0012  CONNECT	R5	R4	R5
      0xB8162200,  //  0013  GETNGBL	R5	K17
      0x8C140B13,  //  0014  GETMET	R5	R5	K19
      0x7C140200,  //  0015  CALL	R5	1
      0x40140805,  //  0016  CONNECT	R5	R4	R5
      0x7C080400,  //  0017  CALL	R2	2
      0x90021202,  //  0018  SETMBR	R0	K9	R2
      0x60080003,  //  0019  GETGBL	R2	G3
      0x5C0C0000,  //  001A  MOVE	R3	R0
      0x7C080200,  //  001B  CALL	R2	1
      0x8C080514,  //  001C  GETMET	R2	R2	K20
      0x5C100200,  //  001D  MOVE	R4	R1
      0x7C080400,  //  001E  CALL	R2	2
      0x80000000,  //  001F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_needle_length
********************************************************************/
be_local_closure(class_lvh_scale_line_get_needle_length,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_scale_line,     /* shared constants */
    be_str_weak(get_needle_length),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040104,  //  0000  GETMBR	R1	R0	K4
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_needle_length
********************************************************************/
be_local_closure(class_lvh_scale_line_set_needle_length,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_scale_line,     /* shared constants */
    be_str_weak(set_needle_length),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x60080009,  //  0000  GETGBL	R2	G9
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x7C080200,  //  0002  CALL	R2	1
      0x90020802,  //  0003  SETMBR	R0	K4	R2
      0x88080105,  //  0004  GETMBR	R2	R0	K5
      0x4C0C0000,  //  0005  LDNIL	R3
      0x20080403,  //  0006  NE	R2	R2	R3
      0x780A0002,  //  0007  JMPF	R2	#000B
      0x8C080100,  //  0008  GETMET	R2	R0	K0
      0x88100105,  //  0009  GETMBR	R4	R0	K5
      0x7C080400,  //  000A  CALL	R2	2
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: lvh_scale_line
********************************************************************/
extern const bclass be_class_lvh_line;
be_local_class(lvh_scale_line,
    1,
    &be_class_lvh_line,
    be_nested_map(5,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(set_val, 4), be_const_closure(class_lvh_scale_line_set_val_closure) },
        { be_const_key_weak(set_needle_length, -1), be_const_closure(class_lvh_scale_line_set_needle_length_closure) },
        { be_const_key_weak(get_needle_length, -1), be_const_closure(class_lvh_scale_line_get_needle_length_closure) },
        { be_const_key_weak(_needle_length, 1), be_const_var(0) },
        { be_const_key_weak(post_init, -1), be_const_closure(class_lvh_scale_line_post_init_closure) },
    })),
    be_str_weak(lvh_scale_line)
);
// compact class 'lvh_spangroup' ktab size: 6, total: 8 (saved 16 bytes)
static const bvalue be_ktab_class_lvh_spangroup[6] = {
  /* K0   */  be_nested_str_weak(_lv_obj),
  /* K1   */  be_nested_str_weak(set_mode),
  /* K2   */  be_nested_str_weak(lv),
  /* K3   */  be_nested_str_weak(SPAN_MODE_BREAK),
  /* K4   */  be_nested_str_weak(refresh),
  /* K5   */  be_nested_str_weak(post_init),
};


extern const bclass be_class_lvh_spangroup;

/********************************************************************
** Solidified function: post_init
********************************************************************/
be_local_closure(class_lvh_spangroup_post_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_spangroup,     /* shared constants */
    be_str_weak(post_init),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0xB8120400,  //  0002  GETNGBL	R4	K2
      0x88100903,  //  0003  GETMBR	R4	R4	K3
      0x7C080400,  //  0004  CALL	R2	2
      0x88080100,  //  0005  GETMBR	R2	R0	K0
      0x8C080504,  //  0006  GETMET	R2	R2	K4
      0x7C080200,  //  0007  CALL	R2	1
      0x60080003,  //  0008  GETGBL	R2	G3
      0x5C0C0000,  //  0009  MOVE	R3	R0
      0x7C080200,  //  000A  CALL	R2	1
      0x8C080505,  //  000B  GETMET	R2	R2	K5
      0x5C100200,  //  000C  MOVE	R4	R1
      0x7C080400,  //  000D  CALL	R2	2
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: refresh
********************************************************************/
be_local_closure(class_lvh_spangroup_refresh,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_spangroup,     /* shared constants */
    be_str_weak(refresh),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040304,  //  0001  GETMET	R1	R1	K4
      0x7C040200,  //  0002  CALL	R1	1
      0x80000000,  //  0003  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: lvh_spangroup
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_class(lvh_spangroup,
    0,
    &be_class_lvh_obj,
    be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(refresh, -1), be_const_closure(class_lvh_spangroup_refresh_closure) },
        { be_const_key_weak(_lv_class, -1), be_const_class(be_class_lv_spangroup) },
        { be_const_key_weak(post_init, 0), be_const_closure(class_lvh_spangroup_post_init_closure) },
    })),
    be_str_weak(lvh_spangroup)
);
// compact class 'lvh_span' ktab size: 31, total: 37 (saved 48 bytes)
static const bvalue be_ktab_class_lvh_span[31] = {
  /* K0   */  be_nested_str_weak(string),
  /* K1   */  be_nested_str_weak(introspect),
  /* K2   */  be_nested_str_weak(startswith),
  /* K3   */  be_nested_str_weak(set_),
  /* K4   */  be_nested_str_weak(get_),
  /* K5   */  be_nested_str_weak(endswith),
  /* K6   */  be_nested_str_weak(_X25),
  /* K7   */  be_const_int(0),
  /* K8   */  be_nested_str_weak(lv),
  /* K9   */  be_nested_str_weak(pct),
  /* K10  */  be_nested_str_weak(_attr_ignore),
  /* K11  */  be_nested_str_weak(find),
  /* K12  */  be_nested_str_weak(get),
  /* K13  */  be_nested_str_weak(function),
  /* K14  */  be_nested_str_weak(_style),
  /* K15  */  be_nested_str_weak(is_color_attribute),
  /* K16  */  be_nested_str_weak(parse_color),
  /* K17  */  be_nested_str_weak(_parent_lvh),
  /* K18  */  be_nested_str_weak(refresh),
  /* K19  */  be_nested_str_weak(_X20for_X20),
  /* K20  */  be_nested_str_weak(HSP_X3A_X20Could_X20not_X20find_X20function_X20set_),
  /* K21  */  be_nested_str_weak(_lv_obj),
  /* K22  */  be_nested_str_weak(_page),
  /* K23  */  be_nested_str_weak(_hm),
  /* K24  */  be_nested_str_weak(lvh_spangroup),
  /* K25  */  be_nested_str_weak(add_span),
  /* K26  */  be_nested_str_weak(get_style),
  /* K27  */  be_nested_str_weak(HSP_X3A_X20_X27span_X27_X20should_X20have_X20a_X20parent_X20of_X20type_X20_X27spangroup_X27),
  /* K28  */  be_nested_str_weak(parse_font),
  /* K29  */  be_nested_str_weak(set_text_font),
  /* K30  */  be_nested_str_weak(set_text),
};


extern const bclass be_class_lvh_span;

/********************************************************************
** Solidified function: setmember
********************************************************************/
be_local_closure(class_lvh_span_setmember,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_span,     /* shared constants */
    be_str_weak(setmember),
    &be_const_str_solidified,
    ( &(const binstruction[92]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xA4120200,  //  0001  IMPORT	R4	K1
      0x8C140702,  //  0002  GETMET	R5	R3	K2
      0x5C1C0200,  //  0003  MOVE	R7	R1
      0x58200003,  //  0004  LDCONST	R8	K3
      0x7C140600,  //  0005  CALL	R5	3
      0x74160004,  //  0006  JMPT	R5	#000C
      0x8C140702,  //  0007  GETMET	R5	R3	K2
      0x5C1C0200,  //  0008  MOVE	R7	R1
      0x58200004,  //  0009  LDCONST	R8	K4
      0x7C140600,  //  000A  CALL	R5	3
      0x78160000,  //  000B  JMPF	R5	#000D
      0x80000A00,  //  000C  RET	0
      0x8C140705,  //  000D  GETMET	R5	R3	K5
      0x5C1C0200,  //  000E  MOVE	R7	R1
      0x58200006,  //  000F  LDCONST	R8	K6
      0x7C140600,  //  0010  CALL	R5	3
      0x78160009,  //  0011  JMPF	R5	#001C
      0x5415FFFD,  //  0012  LDINT	R5	-2
      0x40160E05,  //  0013  CONNECT	R5	K7	R5
      0x94040205,  //  0014  GETIDX	R1	R1	R5
      0xB8161000,  //  0015  GETNGBL	R5	K8
      0x8C140B09,  //  0016  GETMET	R5	R5	K9
      0x601C0009,  //  0017  GETGBL	R7	G9
      0x5C200400,  //  0018  MOVE	R8	R2
      0x7C1C0200,  //  0019  CALL	R7	1
      0x7C140400,  //  001A  CALL	R5	2
      0x5C080A00,  //  001B  MOVE	R2	R5
      0x8814010A,  //  001C  GETMBR	R5	R0	K10
      0x8C140B0B,  //  001D  GETMET	R5	R5	K11
      0x5C1C0200,  //  001E  MOVE	R7	R1
      0x7C140400,  //  001F  CALL	R5	2
      0x4C180000,  //  0020  LDNIL	R6
      0x20140A06,  //  0021  NE	R5	R5	R6
      0x78160000,  //  0022  JMPF	R5	#0024
      0x80000A00,  //  0023  RET	0
      0x8C14090C,  //  0024  GETMET	R5	R4	K12
      0x5C1C0000,  //  0025  MOVE	R7	R0
      0x00220601,  //  0026  ADD	R8	K3	R1
      0x7C140600,  //  0027  CALL	R5	3
      0x60180004,  //  0028  GETGBL	R6	G4
      0x5C1C0A00,  //  0029  MOVE	R7	R5
      0x7C180200,  //  002A  CALL	R6	1
      0x1C180D0D,  //  002B  EQ	R6	R6	K13
      0x781A0004,  //  002C  JMPF	R6	#0032
      0x5C180A00,  //  002D  MOVE	R6	R5
      0x5C1C0000,  //  002E  MOVE	R7	R0
      0x5C200400,  //  002F  MOVE	R8	R2
      0x7C180400,  //  0030  CALL	R6	2
      0x80000C00,  //  0031  RET	0
      0x8C18090C,  //  0032  GETMET	R6	R4	K12
      0x8820010E,  //  0033  GETMBR	R8	R0	K14
      0x00260601,  //  0034  ADD	R9	K3	R1
      0x7C180600,  //  0035  CALL	R6	3
      0x5C140C00,  //  0036  MOVE	R5	R6
      0x60180004,  //  0037  GETGBL	R6	G4
      0x5C1C0A00,  //  0038  MOVE	R7	R5
      0x7C180200,  //  0039  CALL	R6	1
      0x1C180D0D,  //  003A  EQ	R6	R6	K13
      0x781A001B,  //  003B  JMPF	R6	#0058
      0x8C18010F,  //  003C  GETMET	R6	R0	K15
      0x5C200200,  //  003D  MOVE	R8	R1
      0x7C180400,  //  003E  CALL	R6	2
      0x781A0003,  //  003F  JMPF	R6	#0044
      0x8C180110,  //  0040  GETMET	R6	R0	K16
      0x5C200400,  //  0041  MOVE	R8	R2
      0x7C180400,  //  0042  CALL	R6	2
      0x5C080C00,  //  0043  MOVE	R2	R6
      0xA8020008,  //  0044  EXBLK	0	#004E
      0x5C180A00,  //  0045  MOVE	R6	R5
      0x881C010E,  //  0046  GETMBR	R7	R0	K14
      0x5C200400,  //  0047  MOVE	R8	R2
      0x7C180400,  //  0048  CALL	R6	2
      0x88180111,  //  0049  GETMBR	R6	R0	K17
      0x8C180D12,  //  004A  GETMET	R6	R6	K18
      0x7C180200,  //  004B  CALL	R6	1
      0xA8040001,  //  004C  EXBLK	1	1
      0x70020006,  //  004D  JMP		#0055
      0xAC180002,  //  004E  CATCH	R6	0	2
      0x70020003,  //  004F  JMP		#0054
      0x00200F13,  //  0050  ADD	R8	R7	K19
      0x00201001,  //  0051  ADD	R8	R8	R1
      0xB0040C08,  //  0052  RAISE	1	R6	R8
      0x70020000,  //  0053  JMP		#0055
      0xB0080000,  //  0054  RAISE	2	R0	R0
      0x4C180000,  //  0055  LDNIL	R6
      0x80040C00,  //  0056  RET	1	R6
      0x70020002,  //  0057  JMP		#005B
      0x60180001,  //  0058  GETGBL	R6	G1
      0x001E2801,  //  0059  ADD	R7	K20	R1
      0x7C180200,  //  005A  CALL	R6	1
      0x80000000,  //  005B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: post_init
********************************************************************/
be_local_closure(class_lvh_span_post_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_span,     /* shared constants */
    be_str_weak(post_init),
    &be_const_str_solidified,
    ( &(const binstruction[23]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x90022A02,  //  0001  SETMBR	R0	K21	R2
      0x6008000F,  //  0002  GETGBL	R2	G15
      0x880C0111,  //  0003  GETMBR	R3	R0	K17
      0x88100116,  //  0004  GETMBR	R4	R0	K22
      0x88100917,  //  0005  GETMBR	R4	R4	K23
      0x88100918,  //  0006  GETMBR	R4	R4	K24
      0x7C080400,  //  0007  CALL	R2	2
      0x780A0009,  //  0008  JMPF	R2	#0013
      0x88080111,  //  0009  GETMBR	R2	R0	K17
      0x88080515,  //  000A  GETMBR	R2	R2	K21
      0x8C080519,  //  000B  GETMET	R2	R2	K25
      0x7C080200,  //  000C  CALL	R2	1
      0x90022A02,  //  000D  SETMBR	R0	K21	R2
      0x88080115,  //  000E  GETMBR	R2	R0	K21
      0x8C08051A,  //  000F  GETMET	R2	R2	K26
      0x7C080200,  //  0010  CALL	R2	1
      0x90021C02,  //  0011  SETMBR	R0	K14	R2
      0x70020002,  //  0012  JMP		#0016
      0x60080001,  //  0013  GETGBL	R2	G1
      0x580C001B,  //  0014  LDCONST	R3	K27
      0x7C080200,  //  0015  CALL	R2	1
      0x80000000,  //  0016  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_text_font
********************************************************************/
be_local_closure(class_lvh_span_set_text_font,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_span,     /* shared constants */
    be_str_weak(set_text_font),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x8C08011C,  //  0000  GETMET	R2	R0	K28
      0x5C100200,  //  0001  MOVE	R4	R1
      0x7C080400,  //  0002  CALL	R2	2
      0x4C0C0000,  //  0003  LDNIL	R3
      0x200C0403,  //  0004  NE	R3	R2	R3
      0x780E0006,  //  0005  JMPF	R3	#000D
      0x880C010E,  //  0006  GETMBR	R3	R0	K14
      0x8C0C071D,  //  0007  GETMET	R3	R3	K29
      0x5C140400,  //  0008  MOVE	R5	R2
      0x7C0C0400,  //  0009  CALL	R3	2
      0x880C0111,  //  000A  GETMBR	R3	R0	K17
      0x8C0C0712,  //  000B  GETMET	R3	R3	K18
      0x7C0C0200,  //  000C  CALL	R3	1
      0x80000000,  //  000D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_text
********************************************************************/
be_local_closure(class_lvh_span_set_text,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_span,     /* shared constants */
    be_str_weak(set_text),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x88080115,  //  0000  GETMBR	R2	R0	K21
      0x8C08051E,  //  0001  GETMET	R2	R2	K30
      0x60100008,  //  0002  GETGBL	R4	G8
      0x5C140200,  //  0003  MOVE	R5	R1
      0x7C100200,  //  0004  CALL	R4	1
      0x7C080400,  //  0005  CALL	R2	2
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: lvh_span
********************************************************************/
extern const bclass be_class_lvh_root;
be_local_class(lvh_span,
    1,
    &be_class_lvh_root,
    be_nested_map(6,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(setmember, -1), be_const_closure(class_lvh_span_setmember_closure) },
        { be_const_key_weak(_lv_class, 4), be_const_nil() },
        { be_const_key_weak(post_init, -1), be_const_closure(class_lvh_span_post_init_closure) },
        { be_const_key_weak(set_text_font, -1), be_const_closure(class_lvh_span_set_text_font_closure) },
        { be_const_key_weak(_style, 0), be_const_var(0) },
        { be_const_key_weak(set_text, -1), be_const_closure(class_lvh_span_set_text_closure) },
    })),
    be_str_weak(lvh_span)
);
// compact class 'lvh_tabview' ktab size: 19, total: 28 (saved 72 bytes)
static const bvalue be_ktab_class_lvh_tabview[19] = {
  /* K0   */  be_nested_str_weak(_lv_obj),
  /* K1   */  be_nested_str_weak(get_tab_active),
  /* K2   */  be_nested_str_weak(_tab_list),
  /* K3   */  be_nested_str_weak(find),
  /* K4   */  be_nested_str_weak(post_init),
  /* K5   */  be_nested_str_weak(get_val),
  /* K6   */  be_const_int(0),
  /* K7   */  be_nested_str_weak(get_count),
  /* K8   */  be_nested_str_weak(get_text),
  /* K9   */  be_nested_str_weak(get_tab_count),
  /* K10  */  be_nested_str_weak(push),
  /* K11  */  be_nested_str_weak(tasmota),
  /* K12  */  be_nested_str_weak(defer),
  /* K13  */  be_const_int(1),
  /* K14  */  be_nested_str_weak(set_active),
  /* K15  */  be_nested_str_weak(lv),
  /* K16  */  be_nested_str_weak(ANIM_OFF),
  /* K17  */  be_nested_str_weak(_direction),
  /* K18  */  be_nested_str_weak(set_tab_bar_position),
};


extern const bclass be_class_lvh_tabview;

/********************************************************************
** Solidified function: get_val
********************************************************************/
be_local_closure(class_lvh_tabview_get_val,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_tabview,     /* shared constants */
    be_str_weak(get_val),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x7C040200,  //  0002  CALL	R1	1
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: find_tab
********************************************************************/
be_local_closure(class_lvh_tabview_find_tab,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_tabview,     /* shared constants */
    be_str_weak(find_tab),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x88080102,  //  0000  GETMBR	R2	R0	K2
      0x8C080503,  //  0001  GETMET	R2	R2	K3
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x80040400,  //  0004  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: post_init
********************************************************************/
be_local_closure(class_lvh_tabview_post_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_tabview,     /* shared constants */
    be_str_weak(post_init),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x60080012,  //  0000  GETGBL	R2	G18
      0x7C080000,  //  0001  CALL	R2	0
      0x90020402,  //  0002  SETMBR	R0	K2	R2
      0x60080003,  //  0003  GETGBL	R2	G3
      0x5C0C0000,  //  0004  MOVE	R3	R0
      0x7C080200,  //  0005  CALL	R2	1
      0x8C080504,  //  0006  GETMET	R2	R2	K4
      0x5C100200,  //  0007  MOVE	R4	R1
      0x7C080400,  //  0008  CALL	R2	2
      0x80000000,  //  0009  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_text
********************************************************************/
be_local_closure(class_lvh_tabview_get_text,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_tabview,     /* shared constants */
    be_str_weak(get_text),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0x8C040105,  //  0000  GETMET	R1	R0	K5
      0x7C040200,  //  0001  CALL	R1	1
      0x28080306,  //  0002  GE	R2	R1	K6
      0x780A0009,  //  0003  JMPF	R2	#000E
      0x8C080107,  //  0004  GETMET	R2	R0	K7
      0x7C080200,  //  0005  CALL	R2	1
      0x14080202,  //  0006  LT	R2	R1	R2
      0x780A0005,  //  0007  JMPF	R2	#000E
      0x88080102,  //  0008  GETMBR	R2	R0	K2
      0x94080401,  //  0009  GETIDX	R2	R2	R1
      0x8C080508,  //  000A  GETMET	R2	R2	K8
      0x7C080200,  //  000B  CALL	R2	1
      0x80040400,  //  000C  RET	1	R2
      0x70020001,  //  000D  JMP		#0010
      0x4C080000,  //  000E  LDNIL	R2
      0x80040400,  //  000F  RET	1	R2
      0x80000000,  //  0010  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_count
********************************************************************/
be_local_closure(class_lvh_tabview_get_count,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_tabview,     /* shared constants */
    be_str_weak(get_count),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040309,  //  0001  GETMET	R1	R1	K9
      0x7C040200,  //  0002  CALL	R1	1
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: push_tab
********************************************************************/
be_local_closure(class_lvh_tabview_push_tab,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_tabview,     /* shared constants */
    be_str_weak(push_tab),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x88080102,  //  0000  GETMBR	R2	R0	K2
      0x8C08050A,  //  0001  GETMET	R2	R2	K10
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_val
********************************************************************/
be_local_closure(class_lvh_tabview_set_val,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 1]) {
      be_nested_proto(
        4,                          /* nstack */
        0,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 2]) {  /* upvals */
          be_local_const_upval(1, 0),
          be_local_const_upval(1, 1),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 1]) {     /* constants */
        /* K0   */  be_nested_str_weak(set_val),
        }),
        be_str_weak(_anonymous_),
        &be_const_str_solidified,
        ( &(const binstruction[ 6]) {  /* code */
          0x68000000,  //  0000  GETUPV	R0	U0
          0x8C000100,  //  0001  GETMET	R0	R0	K0
          0x68080001,  //  0002  GETUPV	R2	U1
          0x500C0200,  //  0003  LDBOOL	R3	1	0
          0x7C000600,  //  0004  CALL	R0	3
          0x80000000,  //  0005  RET	0
        })
      ),
    }),
    1,                          /* has constants */
    &be_ktab_class_lvh_tabview,     /* shared constants */
    be_str_weak(set_val),
    &be_const_str_solidified,
    ( &(const binstruction[30]) {  /* code */
      0x8C0C0107,  //  0000  GETMET	R3	R0	K7
      0x7C0C0200,  //  0001  CALL	R3	1
      0x1C100706,  //  0002  EQ	R4	R3	K6
      0x78120006,  //  0003  JMPF	R4	#000B
      0x5C100400,  //  0004  MOVE	R4	R2
      0x74120003,  //  0005  JMPT	R4	#000A
      0xB8121600,  //  0006  GETNGBL	R4	K11
      0x8C10090C,  //  0007  GETMET	R4	R4	K12
      0x84180000,  //  0008  CLOSURE	R6	P0
      0x7C100400,  //  0009  CALL	R4	2
      0x70020010,  //  000A  JMP		#001C
      0x4C100000,  //  000B  LDNIL	R4
      0x1C100204,  //  000C  EQ	R4	R1	R4
      0x78120000,  //  000D  JMPF	R4	#000F
      0x58040006,  //  000E  LDCONST	R1	K6
      0x14100306,  //  000F  LT	R4	R1	K6
      0x78120000,  //  0010  JMPF	R4	#0012
      0x58040006,  //  0011  LDCONST	R1	K6
      0x28100203,  //  0012  GE	R4	R1	R3
      0x78120001,  //  0013  JMPF	R4	#0016
      0x0410070D,  //  0014  SUB	R4	R3	K13
      0x5C040800,  //  0015  MOVE	R1	R4
      0x88100100,  //  0016  GETMBR	R4	R0	K0
      0x8C10090E,  //  0017  GETMET	R4	R4	K14
      0x5C180200,  //  0018  MOVE	R6	R1
      0xB81E1E00,  //  0019  GETNGBL	R7	K15
      0x881C0F10,  //  001A  GETMBR	R7	R7	K16
      0x7C100600,  //  001B  CALL	R4	3
      0xA0000000,  //  001C  CLOSE	R0
      0x80000000,  //  001D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_btn_pos
********************************************************************/
be_local_closure(class_lvh_tabview_set_btn_pos,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_tabview,     /* shared constants */
    be_str_weak(set_btn_pos),
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0x60080009,  //  0000  GETGBL	R2	G9
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x7C080200,  //  0002  CALL	R2	1
      0x5C040400,  //  0003  MOVE	R1	R2
      0x4C080000,  //  0004  LDNIL	R2
      0x1C080202,  //  0005  EQ	R2	R1	R2
      0x740A0006,  //  0006  JMPT	R2	#000E
      0x14080306,  //  0007  LT	R2	R1	K6
      0x740A0004,  //  0008  JMPT	R2	#000E
      0x6008000C,  //  0009  GETGBL	R2	G12
      0x880C0111,  //  000A  GETMBR	R3	R0	K17
      0x7C080200,  //  000B  CALL	R2	1
      0x28080202,  //  000C  GE	R2	R1	R2
      0x780A0000,  //  000D  JMPF	R2	#000F
      0x58040006,  //  000E  LDCONST	R1	K6
      0x88080111,  //  000F  GETMBR	R2	R0	K17
      0x94080401,  //  0010  GETIDX	R2	R2	R1
      0x880C0100,  //  0011  GETMBR	R3	R0	K0
      0x8C0C0712,  //  0012  GETMET	R3	R3	K18
      0x5C140400,  //  0013  MOVE	R5	R2
      0x7C0C0400,  //  0014  CALL	R3	2
      0x80000000,  //  0015  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: lvh_tabview
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_class(lvh_tabview,
    1,
    &be_class_lvh_obj,
    be_nested_map(11,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(_lv_class, -1), be_const_class(be_class_lv_tabview) },
        { be_const_key_weak(set_btn_pos, 2), be_const_closure(class_lvh_tabview_set_btn_pos_closure) },
        { be_const_key_weak(set_val, -1), be_const_closure(class_lvh_tabview_set_val_closure) },
        { be_const_key_weak(_tab_list, -1), be_const_var(0) },
        { be_const_key_weak(find_tab, 9), be_const_closure(class_lvh_tabview_find_tab_closure) },
        { be_const_key_weak(get_count, -1), be_const_closure(class_lvh_tabview_get_count_closure) },
        { be_const_key_weak(get_val, 5), be_const_closure(class_lvh_tabview_get_val_closure) },
        { be_const_key_weak(push_tab, -1), be_const_closure(class_lvh_tabview_push_tab_closure) },
        { be_const_key_weak(get_text, 1), be_const_closure(class_lvh_tabview_get_text_closure) },
        { be_const_key_weak(post_init, 0), be_const_closure(class_lvh_tabview_post_init_closure) },
        { be_const_key_weak(_direction, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(5,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(4),
        be_const_int(8),
        be_const_int(1),
        be_const_int(2),
    }))    ) } )) },
    })),
    be_str_weak(lvh_tabview)
);

extern const bclass be_class_lvh_btn_tab;

/********************************************************************
** Solidified function: post_init
********************************************************************/
be_local_closure(class_lvh_btn_tab_post_init,   /* name */
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
    /* K0   */  be_nested_str_weak(_lv_obj),
    /* K1   */  be_nested_str_weak(set_style_radius),
    /* K2   */  be_const_int(0),
    }),
    be_str_weak(post_init),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x58100002,  //  0002  LDCONST	R4	K2
      0x58140002,  //  0003  LDCONST	R5	K2
      0x7C080600,  //  0004  CALL	R2	3
      0x80000000,  //  0005  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: lvh_btn_tab
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_class(lvh_btn_tab,
    0,
    &be_class_lvh_obj,
    be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(post_init, -1), be_const_closure(class_lvh_btn_tab_post_init_closure) },
        { be_const_key_weak(_lv_class, -1), be_const_class(be_class_lv_button) },
    })),
    be_str_weak(lvh_btn_tab)
);
// compact class 'lvh_tab' ktab size: 30, total: 38 (saved 64 bytes)
static const bvalue be_ktab_class_lvh_tab[30] = {
  /* K0   */  be_nested_str_weak(_lv_obj),
  /* K1   */  be_nested_str_weak(_parent_lvh),
  /* K2   */  be_nested_str_weak(_page),
  /* K3   */  be_nested_str_weak(_hm),
  /* K4   */  be_nested_str_weak(lvh_tabview),
  /* K5   */  be_nested_str_weak(_text),
  /* K6   */  be_nested_str_weak(add_tab),
  /* K7   */  be_nested_str_weak(get_tab_bar),
  /* K8   */  be_nested_str_weak(lv),
  /* K9   */  be_nested_str_weak(obj_class),
  /* K10  */  be_nested_str_weak(button),
  /* K11  */  be_nested_str_weak(_class),
  /* K12  */  be_nested_str_weak(get_child_count_by_type),
  /* K13  */  be_nested_str_weak(get_child_by_type),
  /* K14  */  be_const_int(1),
  /* K15  */  be_nested_str_weak(_btn),
  /* K16  */  be_nested_str_weak(lvh_btn_tab),
  /* K17  */  be_nested_str_weak(push_tab),
  /* K18  */  be_nested_str_weak(HSP_X3A_X20_X27tab_X27_X20requires_X20_X27text_X27_X20attribute),
  /* K19  */  be_nested_str_weak(HSP_X3A_X20_X27tab_X27_X20should_X20have_X20a_X20parent_X20of_X20type_X20_X27tabview_X27),
  /* K20  */  be_nested_str_weak(string),
  /* K21  */  be_nested_str_weak(startswith),
  /* K22  */  be_nested_str_weak(tab_),
  /* K23  */  be_nested_str_weak(setmember),
  /* K24  */  be_const_int(2147483647),
  /* K25  */  be_nested_str_weak(member),
  /* K26  */  be_nested_str_weak(set_text),
  /* K27  */  be_nested_str_weak(find),
  /* K28  */  be_nested_str_weak(text),
  /* K29  */  be_nested_str_weak(init),
};


extern const bclass be_class_lvh_tab;

/********************************************************************
** Solidified function: post_init
********************************************************************/
be_local_closure(class_lvh_tab_post_init,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_tab,     /* shared constants */
    be_str_weak(post_init),
    &be_const_str_solidified,
    ( &(const binstruction[58]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x90020002,  //  0001  SETMBR	R0	K0	R2
      0x6008000F,  //  0002  GETGBL	R2	G15
      0x880C0101,  //  0003  GETMBR	R3	R0	K1
      0x88100102,  //  0004  GETMBR	R4	R0	K2
      0x88100903,  //  0005  GETMBR	R4	R4	K3
      0x88100904,  //  0006  GETMBR	R4	R4	K4
      0x7C080400,  //  0007  CALL	R2	2
      0x780A002C,  //  0008  JMPF	R2	#0036
      0x88080105,  //  0009  GETMBR	R2	R0	K5
      0x4C0C0000,  //  000A  LDNIL	R3
      0x20080403,  //  000B  NE	R2	R2	R3
      0x780A0024,  //  000C  JMPF	R2	#0032
      0x88080101,  //  000D  GETMBR	R2	R0	K1
      0x88080500,  //  000E  GETMBR	R2	R2	K0
      0x8C080506,  //  000F  GETMET	R2	R2	K6
      0x88100105,  //  0010  GETMBR	R4	R0	K5
      0x7C080400,  //  0011  CALL	R2	2
      0x90020002,  //  0012  SETMBR	R0	K0	R2
      0x88080101,  //  0013  GETMBR	R2	R0	K1
      0x88080500,  //  0014  GETMBR	R2	R2	K0
      0x8C080507,  //  0015  GETMET	R2	R2	K7
      0x7C080200,  //  0016  CALL	R2	1
      0xB80E1000,  //  0017  GETNGBL	R3	K8
      0x8C0C0709,  //  0018  GETMET	R3	R3	K9
      0xB8161000,  //  0019  GETNGBL	R5	K8
      0x88140B0A,  //  001A  GETMBR	R5	R5	K10
      0x88140B0B,  //  001B  GETMBR	R5	R5	K11
      0x7C0C0400,  //  001C  CALL	R3	2
      0x8C10050C,  //  001D  GETMET	R4	R2	K12
      0x5C180600,  //  001E  MOVE	R6	R3
      0x7C100400,  //  001F  CALL	R4	2
      0x8C14050D,  //  0020  GETMET	R5	R2	K13
      0x041C090E,  //  0021  SUB	R7	R4	K14
      0x5C200600,  //  0022  MOVE	R8	R3
      0x7C140600,  //  0023  CALL	R5	3
      0x8C180110,  //  0024  GETMET	R6	R0	K16
      0x4C200000,  //  0025  LDNIL	R8
      0x88240102,  //  0026  GETMBR	R9	R0	K2
      0x60280013,  //  0027  GETGBL	R10	G19
      0x7C280000,  //  0028  CALL	R10	0
      0x5C2C0A00,  //  0029  MOVE	R11	R5
      0x5C300000,  //  002A  MOVE	R12	R0
      0x7C180C00,  //  002B  CALL	R6	6
      0x90021E06,  //  002C  SETMBR	R0	K15	R6
      0x88180101,  //  002D  GETMBR	R6	R0	K1
      0x8C180D11,  //  002E  GETMET	R6	R6	K17
      0x5C200000,  //  002F  MOVE	R8	R0
      0x7C180400,  //  0030  CALL	R6	2
      0x70020002,  //  0031  JMP		#0035
      0x60080001,  //  0032  GETGBL	R2	G1
      0x580C0012,  //  0033  LDCONST	R3	K18
      0x7C080200,  //  0034  CALL	R2	1
      0x70020002,  //  0035  JMP		#0039
      0x60080001,  //  0036  GETGBL	R2	G1
      0x580C0013,  //  0037  LDCONST	R3	K19
      0x7C080200,  //  0038  CALL	R2	1
      0x80000000,  //  0039  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: setmember
********************************************************************/
be_local_closure(class_lvh_tab_setmember,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_tab,     /* shared constants */
    be_str_weak(setmember),
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0xA40E2800,  //  0000  IMPORT	R3	K20
      0x8C100715,  //  0001  GETMET	R4	R3	K21
      0x5C180200,  //  0002  MOVE	R6	R1
      0x581C0016,  //  0003  LDCONST	R7	K22
      0x7C100600,  //  0004  CALL	R4	3
      0x78120007,  //  0005  JMPF	R4	#000E
      0x8810010F,  //  0006  GETMBR	R4	R0	K15
      0x8C100917,  //  0007  GETMET	R4	R4	K23
      0x541A0003,  //  0008  LDINT	R6	4
      0x40180D18,  //  0009  CONNECT	R6	R6	K24
      0x94180206,  //  000A  GETIDX	R6	R1	R6
      0x5C1C0400,  //  000B  MOVE	R7	R2
      0x7C100600,  //  000C  CALL	R4	3
      0x70020006,  //  000D  JMP		#0015
      0x60100003,  //  000E  GETGBL	R4	G3
      0x5C140000,  //  000F  MOVE	R5	R0
      0x7C100200,  //  0010  CALL	R4	1
      0x8C100917,  //  0011  GETMET	R4	R4	K23
      0x5C180200,  //  0012  MOVE	R6	R1
      0x5C1C0400,  //  0013  MOVE	R7	R2
      0x7C100600,  //  0014  CALL	R4	3
      0x80000000,  //  0015  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: member
********************************************************************/
be_local_closure(class_lvh_tab_member,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_tab,     /* shared constants */
    be_str_weak(member),
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0xA40A2800,  //  0000  IMPORT	R2	K20
      0x8C0C0515,  //  0001  GETMET	R3	R2	K21
      0x5C140200,  //  0002  MOVE	R5	R1
      0x58180016,  //  0003  LDCONST	R6	K22
      0x7C0C0600,  //  0004  CALL	R3	3
      0x780E0007,  //  0005  JMPF	R3	#000E
      0x880C010F,  //  0006  GETMBR	R3	R0	K15
      0x8C0C0719,  //  0007  GETMET	R3	R3	K25
      0x54160003,  //  0008  LDINT	R5	4
      0x40140B18,  //  0009  CONNECT	R5	R5	K24
      0x94140205,  //  000A  GETIDX	R5	R1	R5
      0x7C0C0400,  //  000B  CALL	R3	2
      0x80040600,  //  000C  RET	1	R3
      0x70020006,  //  000D  JMP		#0015
      0x600C0003,  //  000E  GETGBL	R3	G3
      0x5C100000,  //  000F  MOVE	R4	R0
      0x7C0C0200,  //  0010  CALL	R3	1
      0x8C0C0719,  //  0011  GETMET	R3	R3	K25
      0x5C140200,  //  0012  MOVE	R5	R1
      0x7C0C0400,  //  0013  CALL	R3	2
      0x80040600,  //  0014  RET	1	R3
      0x80000000,  //  0015  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_lvh_tab_init,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    6,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_tab,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0x8C18011A,  //  0000  GETMET	R6	R0	K26
      0x8C20071B,  //  0001  GETMET	R8	R3	K27
      0x5828001C,  //  0002  LDCONST	R10	K28
      0x7C200400,  //  0003  CALL	R8	2
      0x7C180400,  //  0004  CALL	R6	2
      0x60180003,  //  0005  GETGBL	R6	G3
      0x5C1C0000,  //  0006  MOVE	R7	R0
      0x7C180200,  //  0007  CALL	R6	1
      0x8C180D1D,  //  0008  GETMET	R6	R6	K29
      0x5C200200,  //  0009  MOVE	R8	R1
      0x5C240400,  //  000A  MOVE	R9	R2
      0x5C280600,  //  000B  MOVE	R10	R3
      0x5C2C0800,  //  000C  MOVE	R11	R4
      0x5C300A00,  //  000D  MOVE	R12	R5
      0x7C180C00,  //  000E  CALL	R6	6
      0x80000000,  //  000F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_text
********************************************************************/
be_local_closure(class_lvh_tab_set_text,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_tab,     /* shared constants */
    be_str_weak(set_text),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x60080008,  //  0000  GETGBL	R2	G8
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x7C080200,  //  0002  CALL	R2	1
      0x90020A02,  //  0003  SETMBR	R0	K5	R2
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_text
********************************************************************/
be_local_closure(class_lvh_tab_get_text,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_tab,     /* shared constants */
    be_str_weak(get_text),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040105,  //  0000  GETMBR	R1	R0	K5
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: lvh_tab
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_class(lvh_tab,
    2,
    &be_class_lvh_obj,
    be_nested_map(10,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(post_init, 3), be_const_closure(class_lvh_tab_post_init_closure) },
        { be_const_key_weak(setmember, 2), be_const_closure(class_lvh_tab_setmember_closure) },
        { be_const_key_weak(member, -1), be_const_closure(class_lvh_tab_member_closure) },
        { be_const_key_weak(lvh_btn_tab, -1), be_const_class(be_class_lvh_btn_tab) },
        { be_const_key_weak(_text, 8), be_const_var(0) },
        { be_const_key_weak(init, 4), be_const_closure(class_lvh_tab_init_closure) },
        { be_const_key_weak(_btn, -1), be_const_var(1) },
        { be_const_key_weak(_lv_class, -1), be_const_nil() },
        { be_const_key_weak(set_text, -1), be_const_closure(class_lvh_tab_set_text_closure) },
        { be_const_key_weak(get_text, -1), be_const_closure(class_lvh_tab_get_text_closure) },
    })),
    be_str_weak(lvh_tab)
);
// compact class 'lvh_chart' ktab size: 29, total: 74 (saved 360 bytes)
static const bvalue be_ktab_class_lvh_chart[29] = {
  /* K0   */  be_nested_str_weak(_y_min),
  /* K1   */  be_nested_str_weak(_lv_obj),
  /* K2   */  be_nested_str_weak(set_axis_range),
  /* K3   */  be_nested_str_weak(lv),
  /* K4   */  be_nested_str_weak(CHART_AXIS_PRIMARY_Y),
  /* K5   */  be_nested_str_weak(_y_max),
  /* K6   */  be_nested_str_weak(set_next_value),
  /* K7   */  be_nested_str_weak(_ser2),
  /* K8   */  be_nested_str_weak(set_series_color),
  /* K9   */  be_nested_str_weak(_ser1),
  /* K10  */  be_nested_str_weak(parse_color),
  /* K11  */  be_nested_str_weak(_y2_max),
  /* K12  */  be_nested_str_weak(CHART_AXIS_SECONDARY_Y),
  /* K13  */  be_nested_str_weak(_y2_min),
  /* K14  */  be_const_int(0),
  /* K15  */  be_nested_str_weak(_h_div),
  /* K16  */  be_const_int(3),
  /* K17  */  be_nested_str_weak(_v_div),
  /* K18  */  be_nested_str_weak(set_update_mode),
  /* K19  */  be_nested_str_weak(CHART_UPDATE_MODE_SHIFT),
  /* K20  */  be_nested_str_weak(add_series),
  /* K21  */  be_nested_str_weak(color),
  /* K22  */  be_const_int(15615044),
  /* K23  */  be_const_int(4517444),
  /* K24  */  be_nested_str_weak(post_init),
  /* K25  */  be_nested_str_weak(set_div_line_count),
  /* K26  */  be_nested_str_weak(add_point2),
  /* K27  */  be_nested_str_weak(_val),
  /* K28  */  be_nested_str_weak(add_point),
};


extern const bclass be_class_lvh_chart;

/********************************************************************
** Solidified function: set_y_min
********************************************************************/
be_local_closure(class_lvh_chart_set_y_min,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_chart,     /* shared constants */
    be_str_weak(set_y_min),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x88080101,  //  0001  GETMBR	R2	R0	K1
      0x8C080502,  //  0002  GETMET	R2	R2	K2
      0xB8120600,  //  0003  GETNGBL	R4	K3
      0x88100904,  //  0004  GETMBR	R4	R4	K4
      0x88140100,  //  0005  GETMBR	R5	R0	K0
      0x88180105,  //  0006  GETMBR	R6	R0	K5
      0x7C080800,  //  0007  CALL	R2	4
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: add_point2
********************************************************************/
be_local_closure(class_lvh_chart_add_point2,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_chart,     /* shared constants */
    be_str_weak(add_point2),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x88080101,  //  0000  GETMBR	R2	R0	K1
      0x8C080506,  //  0001  GETMET	R2	R2	K6
      0x88100107,  //  0002  GETMBR	R4	R0	K7
      0x5C140200,  //  0003  MOVE	R5	R1
      0x7C080600,  //  0004  CALL	R2	3
      0x80000000,  //  0005  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_series1_color
********************************************************************/
be_local_closure(class_lvh_chart_set_series1_color,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_chart,     /* shared constants */
    be_str_weak(set_series1_color),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x88080101,  //  0000  GETMBR	R2	R0	K1
      0x8C080508,  //  0001  GETMET	R2	R2	K8
      0x88100109,  //  0002  GETMBR	R4	R0	K9
      0x8C14010A,  //  0003  GETMET	R5	R0	K10
      0x5C1C0200,  //  0004  MOVE	R7	R1
      0x7C140400,  //  0005  CALL	R5	2
      0x7C080600,  //  0006  CALL	R2	3
      0x80000000,  //  0007  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_y_max
********************************************************************/
be_local_closure(class_lvh_chart_set_y_max,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_chart,     /* shared constants */
    be_str_weak(set_y_max),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x90020A01,  //  0000  SETMBR	R0	K5	R1
      0x88080101,  //  0001  GETMBR	R2	R0	K1
      0x8C080502,  //  0002  GETMET	R2	R2	K2
      0xB8120600,  //  0003  GETNGBL	R4	K3
      0x88100904,  //  0004  GETMBR	R4	R4	K4
      0x88140100,  //  0005  GETMBR	R5	R0	K0
      0x88180105,  //  0006  GETMBR	R6	R0	K5
      0x7C080800,  //  0007  CALL	R2	4
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_y2_max
********************************************************************/
be_local_closure(class_lvh_chart_set_y2_max,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_chart,     /* shared constants */
    be_str_weak(set_y2_max),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x90021601,  //  0000  SETMBR	R0	K11	R1
      0x88080101,  //  0001  GETMBR	R2	R0	K1
      0x8C080502,  //  0002  GETMET	R2	R2	K2
      0xB8120600,  //  0003  GETNGBL	R4	K3
      0x8810090C,  //  0004  GETMBR	R4	R4	K12
      0x8814010D,  //  0005  GETMBR	R5	R0	K13
      0x8818010B,  //  0006  GETMBR	R6	R0	K11
      0x7C080800,  //  0007  CALL	R2	4
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_y_min
********************************************************************/
be_local_closure(class_lvh_chart_get_y_min,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_chart,     /* shared constants */
    be_str_weak(get_y_min),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: post_init
********************************************************************/
be_local_closure(class_lvh_chart_post_init,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_chart,     /* shared constants */
    be_str_weak(post_init),
    &be_const_str_solidified,
    ( &(const binstruction[41]) {  /* code */
      0x9002010E,  //  0000  SETMBR	R0	K0	K14
      0x540A0063,  //  0001  LDINT	R2	100
      0x90020A02,  //  0002  SETMBR	R0	K5	R2
      0x90021B0E,  //  0003  SETMBR	R0	K13	K14
      0x540A0063,  //  0004  LDINT	R2	100
      0x90021602,  //  0005  SETMBR	R0	K11	R2
      0x90021F10,  //  0006  SETMBR	R0	K15	K16
      0x540A0004,  //  0007  LDINT	R2	5
      0x90022202,  //  0008  SETMBR	R0	K17	R2
      0x88080101,  //  0009  GETMBR	R2	R0	K1
      0x8C080512,  //  000A  GETMET	R2	R2	K18
      0xB8120600,  //  000B  GETNGBL	R4	K3
      0x88100913,  //  000C  GETMBR	R4	R4	K19
      0x7C080400,  //  000D  CALL	R2	2
      0x88080101,  //  000E  GETMBR	R2	R0	K1
      0x8C080514,  //  000F  GETMET	R2	R2	K20
      0xB8120600,  //  0010  GETNGBL	R4	K3
      0x8C100915,  //  0011  GETMET	R4	R4	K21
      0x58180016,  //  0012  LDCONST	R6	K22
      0x7C100400,  //  0013  CALL	R4	2
      0xB8160600,  //  0014  GETNGBL	R5	K3
      0x88140B04,  //  0015  GETMBR	R5	R5	K4
      0x7C080600,  //  0016  CALL	R2	3
      0x90021202,  //  0017  SETMBR	R0	K9	R2
      0x88080101,  //  0018  GETMBR	R2	R0	K1
      0x8C080514,  //  0019  GETMET	R2	R2	K20
      0xB8120600,  //  001A  GETNGBL	R4	K3
      0x8C100915,  //  001B  GETMET	R4	R4	K21
      0x58180017,  //  001C  LDCONST	R6	K23
      0x7C100400,  //  001D  CALL	R4	2
      0xB8160600,  //  001E  GETNGBL	R5	K3
      0x88140B0C,  //  001F  GETMBR	R5	R5	K12
      0x7C080600,  //  0020  CALL	R2	3
      0x90020E02,  //  0021  SETMBR	R0	K7	R2
      0x60080003,  //  0022  GETGBL	R2	G3
      0x5C0C0000,  //  0023  MOVE	R3	R0
      0x7C080200,  //  0024  CALL	R2	1
      0x8C080518,  //  0025  GETMET	R2	R2	K24
      0x5C100200,  //  0026  MOVE	R4	R1
      0x7C080400,  //  0027  CALL	R2	2
      0x80000000,  //  0028  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_y2_min
********************************************************************/
be_local_closure(class_lvh_chart_get_y2_min,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_chart,     /* shared constants */
    be_str_weak(get_y2_min),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x8804010D,  //  0000  GETMBR	R1	R0	K13
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_series2_color
********************************************************************/
be_local_closure(class_lvh_chart_set_series2_color,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_chart,     /* shared constants */
    be_str_weak(set_series2_color),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x88080101,  //  0000  GETMBR	R2	R0	K1
      0x8C080508,  //  0001  GETMET	R2	R2	K8
      0x88100107,  //  0002  GETMBR	R4	R0	K7
      0x8C14010A,  //  0003  GETMET	R5	R0	K10
      0x5C1C0200,  //  0004  MOVE	R7	R1
      0x7C140400,  //  0005  CALL	R5	2
      0x7C080600,  //  0006  CALL	R2	3
      0x80000000,  //  0007  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_y2_min
********************************************************************/
be_local_closure(class_lvh_chart_set_y2_min,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_chart,     /* shared constants */
    be_str_weak(set_y2_min),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x90021A01,  //  0000  SETMBR	R0	K13	R1
      0x88080101,  //  0001  GETMBR	R2	R0	K1
      0x8C080502,  //  0002  GETMET	R2	R2	K2
      0xB8120600,  //  0003  GETNGBL	R4	K3
      0x8810090C,  //  0004  GETMBR	R4	R4	K12
      0x8814010D,  //  0005  GETMBR	R5	R0	K13
      0x8818010B,  //  0006  GETMBR	R6	R0	K11
      0x7C080800,  //  0007  CALL	R2	4
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_h_div_line_count
********************************************************************/
be_local_closure(class_lvh_chart_set_h_div_line_count,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_chart,     /* shared constants */
    be_str_weak(set_h_div_line_count),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x90021E01,  //  0000  SETMBR	R0	K15	R1
      0x88080101,  //  0001  GETMBR	R2	R0	K1
      0x8C080519,  //  0002  GETMET	R2	R2	K25
      0x8810010F,  //  0003  GETMBR	R4	R0	K15
      0x88140111,  //  0004  GETMBR	R5	R0	K17
      0x7C080600,  //  0005  CALL	R2	3
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_y2_max
********************************************************************/
be_local_closure(class_lvh_chart_get_y2_max,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_chart,     /* shared constants */
    be_str_weak(get_y2_max),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x8804010B,  //  0000  GETMBR	R1	R0	K11
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_y_max
********************************************************************/
be_local_closure(class_lvh_chart_get_y_max,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_chart,     /* shared constants */
    be_str_weak(get_y_max),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040105,  //  0000  GETMBR	R1	R0	K5
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_val2
********************************************************************/
be_local_closure(class_lvh_chart_set_val2,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_chart,     /* shared constants */
    be_str_weak(set_val2),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x8C08011A,  //  0000  GETMET	R2	R0	K26
      0x5C100200,  //  0001  MOVE	R4	R1
      0x7C080400,  //  0002  CALL	R2	2
      0x80000000,  //  0003  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_val
********************************************************************/
be_local_closure(class_lvh_chart_set_val,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_chart,     /* shared constants */
    be_str_weak(set_val),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x90023601,  //  0000  SETMBR	R0	K27	R1
      0x8C08011C,  //  0001  GETMET	R2	R0	K28
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: add_point
********************************************************************/
be_local_closure(class_lvh_chart_add_point,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_chart,     /* shared constants */
    be_str_weak(add_point),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x88080101,  //  0000  GETMBR	R2	R0	K1
      0x8C080506,  //  0001  GETMET	R2	R2	K6
      0x88100109,  //  0002  GETMBR	R4	R0	K9
      0x5C140200,  //  0003  MOVE	R5	R1
      0x7C080600,  //  0004  CALL	R2	3
      0x80000000,  //  0005  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_v_div_line_count
********************************************************************/
be_local_closure(class_lvh_chart_set_v_div_line_count,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_chart,     /* shared constants */
    be_str_weak(set_v_div_line_count),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x90022201,  //  0000  SETMBR	R0	K17	R1
      0x88080101,  //  0001  GETMBR	R2	R0	K1
      0x8C080519,  //  0002  GETMET	R2	R2	K25
      0x8810010F,  //  0003  GETMBR	R4	R0	K15
      0x88140111,  //  0004  GETMBR	R5	R0	K17
      0x7C080600,  //  0005  CALL	R2	3
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: lvh_chart
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_class(lvh_chart,
    8,
    &be_class_lvh_obj,
    be_nested_map(26,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(set_y_min, -1), be_const_closure(class_lvh_chart_set_y_min_closure) },
        { be_const_key_weak(add_point2, -1), be_const_closure(class_lvh_chart_add_point2_closure) },
        { be_const_key_weak(_v_div, -1), be_const_var(7) },
        { be_const_key_weak(_ser1, -1), be_const_var(0) },
        { be_const_key_weak(_ser2, -1), be_const_var(1) },
        { be_const_key_weak(_y2_min, 6), be_const_var(4) },
        { be_const_key_weak(get_y2_max, -1), be_const_closure(class_lvh_chart_get_y2_max_closure) },
        { be_const_key_weak(set_v_div_line_count, -1), be_const_closure(class_lvh_chart_set_v_div_line_count_closure) },
        { be_const_key_weak(set_y_max, 24), be_const_closure(class_lvh_chart_set_y_max_closure) },
        { be_const_key_weak(_y2_max, -1), be_const_var(5) },
        { be_const_key_weak(_y_min, 2), be_const_var(2) },
        { be_const_key_weak(set_series2_color, -1), be_const_closure(class_lvh_chart_set_series2_color_closure) },
        { be_const_key_weak(set_y2_min, -1), be_const_closure(class_lvh_chart_set_y2_min_closure) },
        { be_const_key_weak(set_h_div_line_count, 7), be_const_closure(class_lvh_chart_set_h_div_line_count_closure) },
        { be_const_key_weak(_h_div, 5), be_const_var(6) },
        { be_const_key_weak(_y_max, 25), be_const_var(3) },
        { be_const_key_weak(set_series1_color, 22), be_const_closure(class_lvh_chart_set_series1_color_closure) },
        { be_const_key_weak(set_val2, 19), be_const_closure(class_lvh_chart_set_val2_closure) },
        { be_const_key_weak(get_y2_min, 9), be_const_closure(class_lvh_chart_get_y2_min_closure) },
        { be_const_key_weak(set_val, -1), be_const_closure(class_lvh_chart_set_val_closure) },
        { be_const_key_weak(set_y2_max, 15), be_const_closure(class_lvh_chart_set_y2_max_closure) },
        { be_const_key_weak(add_point, -1), be_const_closure(class_lvh_chart_add_point_closure) },
        { be_const_key_weak(get_y_max, -1), be_const_closure(class_lvh_chart_get_y_max_closure) },
        { be_const_key_weak(_lv_class, -1), be_const_class(be_class_lv_chart) },
        { be_const_key_weak(post_init, -1), be_const_closure(class_lvh_chart_post_init_closure) },
        { be_const_key_weak(get_y_min, -1), be_const_closure(class_lvh_chart_get_y_min_closure) },
    })),
    be_str_weak(lvh_chart)
);
// compact class 'lvh_btnmatrix' ktab size: 12, total: 15 (saved 24 bytes)
static const bvalue be_ktab_class_lvh_btnmatrix[12] = {
  /* K0   */  be_nested_str_weak(_options),
  /* K1   */  be_nested_str_weak(_lv_obj),
  /* K2   */  be_nested_str_weak(get_selected_button),
  /* K3   */  be_nested_str_weak(lv),
  /* K4   */  be_nested_str_weak(BUTTONMATRIX_BUTTON_NONE),
  /* K5   */  be_const_int(0),
  /* K6   */  be_nested_str_weak(push),
  /* K7   */  be_nested_str_weak(),
  /* K8   */  be_nested_str_weak(_options_arr),
  /* K9   */  be_nested_str_weak(str_arr),
  /* K10  */  be_nested_str_weak(set_map),
  /* K11  */  be_nested_str_weak(HTP_X3A_X20_X27btnmatrix_X27_X20needs_X20_X27options_X27_X20to_X20be_X20a_X20list_X20of_X20strings),
};


extern const bclass be_class_lvh_btnmatrix;

/********************************************************************
** Solidified function: get_options
********************************************************************/
be_local_closure(class_lvh_btnmatrix_get_options,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_btnmatrix,     /* shared constants */
    be_str_weak(get_options),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_sub_id
********************************************************************/
be_local_closure(class_lvh_btnmatrix_get_sub_id,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_btnmatrix,     /* shared constants */
    be_str_weak(get_sub_id),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x88040101,  //  0000  GETMBR	R1	R0	K1
      0x8C040302,  //  0001  GETMET	R1	R1	K2
      0x7C040200,  //  0002  CALL	R1	1
      0xB80A0600,  //  0003  GETNGBL	R2	K3
      0x88080504,  //  0004  GETMBR	R2	R2	K4
      0x20080202,  //  0005  NE	R2	R1	R2
      0x780A0001,  //  0006  JMPF	R2	#0009
      0x5C080200,  //  0007  MOVE	R2	R1
      0x70020000,  //  0008  JMP		#000A
      0x4C080000,  //  0009  LDNIL	R2
      0x80040400,  //  000A  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_options
********************************************************************/
be_local_closure(class_lvh_btnmatrix_set_options,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_btnmatrix,     /* shared constants */
    be_str_weak(set_options),
    &be_const_str_solidified,
    ( &(const binstruction[34]) {  /* code */
      0x6008000F,  //  0000  GETGBL	R2	G15
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x60100012,  //  0002  GETGBL	R4	G18
      0x7C080400,  //  0003  CALL	R2	2
      0x780A0018,  //  0004  JMPF	R2	#001E
      0x6008000C,  //  0005  GETGBL	R2	G12
      0x5C0C0200,  //  0006  MOVE	R3	R1
      0x7C080200,  //  0007  CALL	R2	1
      0x24080505,  //  0008  GT	R2	R2	K5
      0x780A0013,  //  0009  JMPF	R2	#001E
      0x6008000C,  //  000A  GETGBL	R2	G12
      0x540DFFFE,  //  000B  LDINT	R3	-1
      0x940C0203,  //  000C  GETIDX	R3	R1	R3
      0x7C080200,  //  000D  CALL	R2	1
      0x24080505,  //  000E  GT	R2	R2	K5
      0x780A0002,  //  000F  JMPF	R2	#0013
      0x8C080306,  //  0010  GETMET	R2	R1	K6
      0x58100007,  //  0011  LDCONST	R4	K7
      0x7C080400,  //  0012  CALL	R2	2
      0x90020001,  //  0013  SETMBR	R0	K0	R1
      0xB80A0600,  //  0014  GETNGBL	R2	K3
      0x8C080509,  //  0015  GETMET	R2	R2	K9
      0x5C100200,  //  0016  MOVE	R4	R1
      0x7C080400,  //  0017  CALL	R2	2
      0x90021002,  //  0018  SETMBR	R0	K8	R2
      0x88080101,  //  0019  GETMBR	R2	R0	K1
      0x8C08050A,  //  001A  GETMET	R2	R2	K10
      0x88100108,  //  001B  GETMBR	R4	R0	K8
      0x7C080400,  //  001C  CALL	R2	2
      0x70020002,  //  001D  JMP		#0021
      0x60080001,  //  001E  GETGBL	R2	G1
      0x580C000B,  //  001F  LDCONST	R3	K11
      0x7C080200,  //  0020  CALL	R2	1
      0x80000000,  //  0021  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_val
********************************************************************/
be_local_closure(class_lvh_btnmatrix_get_val,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_btnmatrix,     /* shared constants */
    be_str_weak(get_val),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x4C040000,  //  0000  LDNIL	R1
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: lvh_btnmatrix
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_class(lvh_btnmatrix,
    2,
    &be_class_lvh_obj,
    be_nested_map(7,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(_options, 2), be_const_var(0) },
        { be_const_key_weak(get_options, -1), be_const_closure(class_lvh_btnmatrix_get_options_closure) },
        { be_const_key_weak(set_options, 1), be_const_closure(class_lvh_btnmatrix_set_options_closure) },
        { be_const_key_weak(_lv_class, 5), be_const_class(be_class_lv_buttonmatrix) },
        { be_const_key_weak(_options_arr, -1), be_const_var(1) },
        { be_const_key_weak(get_sub_id, -1), be_const_closure(class_lvh_btnmatrix_get_sub_id_closure) },
        { be_const_key_weak(get_val, -1), be_const_closure(class_lvh_btnmatrix_get_val_closure) },
    })),
    be_str_weak(lvh_btnmatrix)
);
// compact class 'lvh_cpicker' ktab size: 21, total: 31 (saved 80 bytes)
static const bvalue be_ktab_class_lvh_cpicker[21] = {
  /* K0   */  be_nested_str_weak(_CW_MODES),
  /* K1   */  be_nested_str_weak(find),
  /* K2   */  be_nested_str_weak(_lv_obj),
  /* K3   */  be_nested_str_weak(set_mode),
  /* K4   */  be_nested_str_weak(unknown_X20color_X20mode_X20_X27_X25s_X27),
  /* K5   */  be_nested_str_weak(value_error),
  /* K6   */  be_nested_str_weak(get_rgb),
  /* K7   */  be_nested_str_weak(_X23_X2506X),
  /* K8   */  be_nested_str_weak(get_color_mode_fixed),
  /* K9   */  be_nested_str_weak(get_color_mode),
  /* K10  */  be_const_int(0),
  /* K11  */  be_nested_str_weak(unknown),
  /* K12  */  be_nested_str_weak(lv),
  /* K13  */  be_nested_str_weak(colorwheel),
  /* K14  */  be_nested_str_weak(init),
  /* K15  */  be_nested_str_weak(set_scale_width),
  /* K16  */  be_nested_str_weak(set_style_arc_width),
  /* K17  */  be_nested_str_weak(parse_color),
  /* K18  */  be_nested_str_weak(set_rgb),
  /* K19  */  be_nested_str_weak(set_mode_fixed),
  /* K20  */  be_nested_str_weak(get_style_arc_width),
};


extern const bclass be_class_lvh_cpicker;

/********************************************************************
** Solidified function: set_mode
********************************************************************/
be_local_closure(class_lvh_cpicker_set_mode,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_cpicker,     /* shared constants */
    be_str_weak(set_mode),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x4C0C0000,  //  0004  LDNIL	R3
      0x200C0403,  //  0005  NE	R3	R2	R3
      0x780E0004,  //  0006  JMPF	R3	#000C
      0x880C0102,  //  0007  GETMBR	R3	R0	K2
      0x8C0C0703,  //  0008  GETMET	R3	R3	K3
      0x5C140400,  //  0009  MOVE	R5	R2
      0x7C0C0400,  //  000A  CALL	R3	2
      0x70020004,  //  000B  JMP		#0011
      0x600C0018,  //  000C  GETGBL	R3	G24
      0x58100004,  //  000D  LDCONST	R4	K4
      0x5C140400,  //  000E  MOVE	R5	R2
      0x7C0C0400,  //  000F  CALL	R3	2
      0xB0060A03,  //  0010  RAISE	1	K5	R3
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_color
********************************************************************/
be_local_closure(class_lvh_cpicker_get_color,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_cpicker,     /* shared constants */
    be_str_weak(get_color),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x88040102,  //  0000  GETMBR	R1	R0	K2
      0x8C040306,  //  0001  GETMET	R1	R1	K6
      0x7C040200,  //  0002  CALL	R1	1
      0x60080018,  //  0003  GETGBL	R2	G24
      0x580C0007,  //  0004  LDCONST	R3	K7
      0x5C100200,  //  0005  MOVE	R4	R1
      0x7C080400,  //  0006  CALL	R2	2
      0x80040400,  //  0007  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_pad_inner
********************************************************************/
be_local_closure(class_lvh_cpicker_get_pad_inner,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_cpicker,     /* shared constants */
    be_str_weak(get_pad_inner),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_mode_fixed
********************************************************************/
be_local_closure(class_lvh_cpicker_get_mode_fixed,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_cpicker,     /* shared constants */
    be_str_weak(get_mode_fixed),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040102,  //  0000  GETMBR	R1	R0	K2
      0x8C040308,  //  0001  GETMET	R1	R1	K8
      0x7C040200,  //  0002  CALL	R1	1
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_mode
********************************************************************/
be_local_closure(class_lvh_cpicker_get_mode,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_cpicker,     /* shared constants */
    be_str_weak(get_mode),
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0x88040102,  //  0000  GETMBR	R1	R0	K2
      0x8C040309,  //  0001  GETMET	R1	R1	K9
      0x7C040200,  //  0002  CALL	R1	1
      0x2808030A,  //  0003  GE	R2	R1	K10
      0x780A0008,  //  0004  JMPF	R2	#000E
      0x6008000C,  //  0005  GETGBL	R2	G12
      0x880C0100,  //  0006  GETMBR	R3	R0	K0
      0x7C080200,  //  0007  CALL	R2	1
      0x14080202,  //  0008  LT	R2	R1	R2
      0x780A0003,  //  0009  JMPF	R2	#000E
      0x88080100,  //  000A  GETMBR	R2	R0	K0
      0x94080401,  //  000B  GETIDX	R2	R2	R1
      0x80040400,  //  000C  RET	1	R2
      0x70020000,  //  000D  JMP		#000F
      0x80061600,  //  000E  RET	1	K11
      0x80000000,  //  000F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_lvh_cpicker_init,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    6,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_cpicker,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[20]) {  /* code */
      0xB81A1800,  //  0000  GETNGBL	R6	K12
      0x8C180D0D,  //  0001  GETMET	R6	R6	K13
      0x5C200200,  //  0002  MOVE	R8	R1
      0x50240200,  //  0003  LDBOOL	R9	1	0
      0x7C180600,  //  0004  CALL	R6	3
      0x5C100C00,  //  0005  MOVE	R4	R6
      0x60180003,  //  0006  GETGBL	R6	G3
      0x5C1C0000,  //  0007  MOVE	R7	R0
      0x7C180200,  //  0008  CALL	R6	1
      0x8C180D0E,  //  0009  GETMET	R6	R6	K14
      0x5C200200,  //  000A  MOVE	R8	R1
      0x5C240400,  //  000B  MOVE	R9	R2
      0x5C280600,  //  000C  MOVE	R10	R3
      0x5C2C0800,  //  000D  MOVE	R11	R4
      0x5C300A00,  //  000E  MOVE	R12	R5
      0x7C180C00,  //  000F  CALL	R6	6
      0x8C18010F,  //  0010  GETMET	R6	R0	K15
      0x54220018,  //  0011  LDINT	R8	25
      0x7C180400,  //  0012  CALL	R6	2
      0x80000000,  //  0013  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_pad_inner
********************************************************************/
be_local_closure(class_lvh_cpicker_set_pad_inner,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_cpicker,     /* shared constants */
    be_str_weak(set_pad_inner),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_scale_width
********************************************************************/
be_local_closure(class_lvh_cpicker_set_scale_width,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_cpicker,     /* shared constants */
    be_str_weak(set_scale_width),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x88080102,  //  0000  GETMBR	R2	R0	K2
      0x8C080510,  //  0001  GETMET	R2	R2	K16
      0x60100009,  //  0002  GETGBL	R4	G9
      0x5C140200,  //  0003  MOVE	R5	R1
      0x7C100200,  //  0004  CALL	R4	1
      0x5814000A,  //  0005  LDCONST	R5	K10
      0x7C080600,  //  0006  CALL	R2	3
      0x80000000,  //  0007  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_color
********************************************************************/
be_local_closure(class_lvh_cpicker_set_color,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_cpicker,     /* shared constants */
    be_str_weak(set_color),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x8C080111,  //  0000  GETMET	R2	R0	K17
      0x5C100200,  //  0001  MOVE	R4	R1
      0x7C080400,  //  0002  CALL	R2	2
      0x880C0102,  //  0003  GETMBR	R3	R0	K2
      0x8C0C0712,  //  0004  GETMET	R3	R3	K18
      0x5C140400,  //  0005  MOVE	R5	R2
      0x7C0C0400,  //  0006  CALL	R3	2
      0x80000000,  //  0007  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_mode_fixed
********************************************************************/
be_local_closure(class_lvh_cpicker_set_mode_fixed,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_cpicker,     /* shared constants */
    be_str_weak(set_mode_fixed),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x60080017,  //  0000  GETGBL	R2	G23
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x7C080200,  //  0002  CALL	R2	1
      0x5C040400,  //  0003  MOVE	R1	R2
      0x88080102,  //  0004  GETMBR	R2	R0	K2
      0x8C080513,  //  0005  GETMET	R2	R2	K19
      0x5C100200,  //  0006  MOVE	R4	R1
      0x7C080400,  //  0007  CALL	R2	2
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_scale_width
********************************************************************/
be_local_closure(class_lvh_cpicker_get_scale_width,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_cpicker,     /* shared constants */
    be_str_weak(get_scale_width),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x88040102,  //  0000  GETMBR	R1	R0	K2
      0x8C040314,  //  0001  GETMET	R1	R1	K20
      0x580C000A,  //  0002  LDCONST	R3	K10
      0x7C040400,  //  0003  CALL	R1	2
      0x80040200,  //  0004  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: lvh_cpicker
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_class(lvh_cpicker,
    0,
    &be_class_lvh_obj,
    be_nested_map(13,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(get_scale_width, 9), be_const_closure(class_lvh_cpicker_get_scale_width_closure) },
        { be_const_key_weak(get_color, -1), be_const_closure(class_lvh_cpicker_get_color_closure) },
        { be_const_key_weak(get_pad_inner, -1), be_const_closure(class_lvh_cpicker_get_pad_inner_closure) },
        { be_const_key_weak(_CW_MODES, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(3,
    ( (struct bvalue*) &(const bvalue[]) {
        be_nested_str_weak(hue),
        be_nested_str_weak(saturation),
        be_nested_str_weak(value),
    }))    ) } )) },
        { be_const_key_weak(get_mode_fixed, -1), be_const_closure(class_lvh_cpicker_get_mode_fixed_closure) },
        { be_const_key_weak(get_mode, -1), be_const_closure(class_lvh_cpicker_get_mode_closure) },
        { be_const_key_weak(init, -1), be_const_closure(class_lvh_cpicker_init_closure) },
        { be_const_key_weak(set_color, 8), be_const_closure(class_lvh_cpicker_set_color_closure) },
        { be_const_key_weak(_lv_class, -1), be_const_class(be_class_lv_colorwheel) },
        { be_const_key_weak(set_pad_inner, 4), be_const_closure(class_lvh_cpicker_set_pad_inner_closure) },
        { be_const_key_weak(set_scale_width, 7), be_const_closure(class_lvh_cpicker_set_scale_width_closure) },
        { be_const_key_weak(set_mode_fixed, -1), be_const_closure(class_lvh_cpicker_set_mode_fixed_closure) },
        { be_const_key_weak(set_mode, 0), be_const_closure(class_lvh_cpicker_set_mode_closure) },
    })),
    be_str_weak(lvh_cpicker)
);

extern const bclass be_class_lvh_btn;

/********************************************************************
** Solidified class: lvh_btn
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_class(lvh_btn,
    0,
    &be_class_lvh_obj,
    be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(_lv_class, -1), be_const_class(be_class_lv_button) },
    })),
    be_str_weak(lvh_btn)
);

extern const bclass be_class_lvh_checkbox;

/********************************************************************
** Solidified function: set_val
********************************************************************/
be_local_closure(class_lvh_checkbox_set_val,   /* name */
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
    /* K0   */  be_nested_str_weak(_val),
    /* K1   */  be_nested_str_weak(set_toggle),
    }),
    be_str_weak(set_val),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x8C080101,  //  0001  GETMET	R2	R0	K1
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x80040400,  //  0004  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_val
********************************************************************/
be_local_closure(class_lvh_checkbox_get_val,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(get_toggle),
    }),
    be_str_weak(get_val),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x80040200,  //  0002  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: lvh_checkbox
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_class(lvh_checkbox,
    0,
    &be_class_lvh_obj,
    be_nested_map(4,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(get_val, 1), be_const_closure(class_lvh_checkbox_get_val_closure) },
        { be_const_key_weak(set_val, -1), be_const_closure(class_lvh_checkbox_set_val_closure) },
        { be_const_key_weak(_lv_part2_selector, -1), be_const_int(131072) },
        { be_const_key_weak(_lv_class, 0), be_const_class(be_class_lv_checkbox) },
    })),
    be_str_weak(lvh_checkbox)
);

extern const bclass be_class_lvh_scr;

/********************************************************************
** Solidified class: lvh_scr
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_class(lvh_scr,
    0,
    &be_class_lvh_obj,
    be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(_lv_class, -1), be_const_nil() },
    })),
    be_str_weak(lvh_scr)
);
// compact class 'lvh_page' ktab size: 45, total: 70 (saved 200 bytes)
static const bvalue be_ktab_class_lvh_page[45] = {
  /* K0   */  be_nested_str_weak(_clear),
  /* K1   */  be_nested_str_weak(string),
  /* K2   */  be_nested_str_weak(introspect),
  /* K3   */  be_nested_str_weak(startswith),
  /* K4   */  be_nested_str_weak(set_),
  /* K5   */  be_nested_str_weak(get_),
  /* K6   */  be_nested_str_weak(get),
  /* K7   */  be_nested_str_weak(function),
  /* K8   */  be_nested_str_weak(undefined),
  /* K9   */  be_nested_str_weak(_obj_id),
  /* K10  */  be_nested_str_weak(keys),
  /* K11  */  be_nested_str_weak(push),
  /* K12  */  be_nested_str_weak(stop_iteration),
  /* K13  */  be_const_int(0),
  /* K14  */  be_nested_str_weak(contains),
  /* K15  */  be_nested_str_weak(_delete),
  /* K16  */  be_const_int(1),
  /* K17  */  be_nested_str_weak(_lv_scr),
  /* K18  */  be_nested_str_weak(find),
  /* K19  */  be_nested_str_weak(remove),
  /* K20  */  be_nested_str_weak(p_X25ib_X25i),
  /* K21  */  be_nested_str_weak(_page),
  /* K22  */  be_nested_str_weak(id),
  /* K23  */  be_nested_str_weak(global),
  /* K24  */  be_nested_str_weak(_hm),
  /* K25  */  be_nested_str_weak(_page_id),
  /* K26  */  be_nested_str_weak(lv),
  /* K27  */  be_nested_str_weak(layer_top),
  /* K28  */  be_nested_str_weak(obj),
  /* K29  */  be_nested_str_weak(lvh_scr),
  /* K30  */  be_nested_str_weak(p_X25s),
  /* K31  */  be_nested_str_weak(p_X25sb0),
  /* K32  */  be_nested_str_weak(_p),
  /* K33  */  be_nested_str_weak(scr_act),
  /* K34  */  be_nested_str_weak(page_dir_to),
  /* K35  */  be_nested_str_weak(_X7B_X22hasp_X22_X3A_X7B_X22p_X25i_X22_X3A_X22out_X22_X7D_X7D),
  /* K36  */  be_nested_str_weak(lvh_page_cur_idx),
  /* K37  */  be_nested_str_weak(tasmota),
  /* K38  */  be_nested_str_weak(defer),
  /* K39  */  be_nested_str_weak(_X7B_X22hasp_X22_X3A_X7B_X22p_X25i_X22_X3A_X22in_X22_X7D_X7D),
  /* K40  */  be_nested_str_weak(screen_load),
  /* K41  */  be_nested_str_weak(show_anim),
  /* K42  */  be_nested_str_weak(SCR_LOAD_ANIM_NONE),
  /* K43  */  be_nested_str_weak(screen_load_anim),
  /* K44  */  be_nested_str_weak(_remove_page),
};


extern const bclass be_class_lvh_page;

/********************************************************************
** Solidified function: get_clear
********************************************************************/
be_local_closure(class_lvh_page_get_clear,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 1]) {
      be_nested_proto(
        0,                          /* nstack */
        0,                          /* argc */
        0,                          /* varg */
        0,                          /* has upvals */
        NULL,                       /* no upvals */
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        0,                          /* has constants */
        NULL,                       /* no const */
        be_str_weak(_anonymous_),
        &be_const_str_solidified,
        ( &(const binstruction[ 1]) {  /* code */
          0x80000000,  //  0000  RET	0
        })
      ),
    }),
    1,                          /* has constants */
    &be_ktab_class_lvh_page,     /* shared constants */
    be_str_weak(get_clear),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x84040000,  //  0002  CLOSURE	R1	P0
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: member
********************************************************************/
be_local_closure(class_lvh_page_member,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_page,     /* shared constants */
    be_str_weak(member),
    &be_const_str_solidified,
    ( &(const binstruction[30]) {  /* code */
      0xA40A0200,  //  0000  IMPORT	R2	K1
      0xA40E0400,  //  0001  IMPORT	R3	K2
      0x8C100503,  //  0002  GETMET	R4	R2	K3
      0x5C180200,  //  0003  MOVE	R6	R1
      0x581C0004,  //  0004  LDCONST	R7	K4
      0x7C100600,  //  0005  CALL	R4	3
      0x74120004,  //  0006  JMPT	R4	#000C
      0x8C100503,  //  0007  GETMET	R4	R2	K3
      0x5C180200,  //  0008  MOVE	R6	R1
      0x581C0005,  //  0009  LDCONST	R7	K5
      0x7C100600,  //  000A  CALL	R4	3
      0x78120000,  //  000B  JMPF	R4	#000D
      0x80000800,  //  000C  RET	0
      0x8C100706,  //  000D  GETMET	R4	R3	K6
      0x5C180000,  //  000E  MOVE	R6	R0
      0x001E0A01,  //  000F  ADD	R7	K5	R1
      0x7C100600,  //  0010  CALL	R4	3
      0x60140004,  //  0011  GETGBL	R5	G4
      0x5C180800,  //  0012  MOVE	R6	R4
      0x7C140200,  //  0013  CALL	R5	1
      0x1C140B07,  //  0014  EQ	R5	R5	K7
      0x78160003,  //  0015  JMPF	R5	#001A
      0x5C140800,  //  0016  MOVE	R5	R4
      0x5C180000,  //  0017  MOVE	R6	R0
      0x7C140200,  //  0018  CALL	R5	1
      0x80040A00,  //  0019  RET	1	R5
      0x6014000B,  //  001A  GETGBL	R5	G11
      0x58180008,  //  001B  LDCONST	R6	K8
      0x7C140200,  //  001C  CALL	R5	1
      0x80040A00,  //  001D  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _clear
********************************************************************/
be_local_closure(class_lvh_page__clear,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_page,     /* shared constants */
    be_str_weak(_clear),
    &be_const_str_solidified,
    ( &(const binstruction[41]) {  /* code */
      0x60040012,  //  0000  GETGBL	R1	G18
      0x7C040000,  //  0001  CALL	R1	0
      0x60080010,  //  0002  GETGBL	R2	G16
      0x880C0109,  //  0003  GETMBR	R3	R0	K9
      0x8C0C070A,  //  0004  GETMET	R3	R3	K10
      0x7C0C0200,  //  0005  CALL	R3	1
      0x7C080200,  //  0006  CALL	R2	1
      0xA8020005,  //  0007  EXBLK	0	#000E
      0x5C0C0400,  //  0008  MOVE	R3	R2
      0x7C0C0000,  //  0009  CALL	R3	0
      0x8C10030B,  //  000A  GETMET	R4	R1	K11
      0x5C180600,  //  000B  MOVE	R6	R3
      0x7C100400,  //  000C  CALL	R4	2
      0x7001FFF9,  //  000D  JMP		#0008
      0x5808000C,  //  000E  LDCONST	R2	K12
      0xAC080200,  //  000F  CATCH	R2	1	0
      0xB0080000,  //  0010  RAISE	2	R0	R0
      0x5808000D,  //  0011  LDCONST	R2	K13
      0x600C000C,  //  0012  GETGBL	R3	G12
      0x5C100200,  //  0013  MOVE	R4	R1
      0x7C0C0200,  //  0014  CALL	R3	1
      0x140C0403,  //  0015  LT	R3	R2	R3
      0x780E000D,  //  0016  JMPF	R3	#0025
      0x940C0202,  //  0017  GETIDX	R3	R1	R2
      0x2010070D,  //  0018  NE	R4	R3	K13
      0x78120008,  //  0019  JMPF	R4	#0023
      0x88100109,  //  001A  GETMBR	R4	R0	K9
      0x8C10090E,  //  001B  GETMET	R4	R4	K14
      0x5C180600,  //  001C  MOVE	R6	R3
      0x7C100400,  //  001D  CALL	R4	2
      0x78120003,  //  001E  JMPF	R4	#0023
      0x88100109,  //  001F  GETMBR	R4	R0	K9
      0x94100803,  //  0020  GETIDX	R4	R4	R3
      0x8C10090F,  //  0021  GETMET	R4	R4	K15
      0x7C100200,  //  0022  CALL	R4	1
      0x00080510,  //  0023  ADD	R2	R2	K16
      0x7001FFEC,  //  0024  JMP		#0012
      0x600C0013,  //  0025  GETGBL	R3	G19
      0x7C0C0000,  //  0026  CALL	R3	0
      0x90021203,  //  0027  SETMBR	R0	K9	R3
      0x80000000,  //  0028  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_scr
********************************************************************/
be_local_closure(class_lvh_page_get_scr,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_page,     /* shared constants */
    be_str_weak(get_scr),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040111,  //  0000  GETMBR	R1	R0	K17
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_delete
********************************************************************/
be_local_closure(class_lvh_page_get_delete,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 1]) {
      be_nested_proto(
        0,                          /* nstack */
        0,                          /* argc */
        0,                          /* varg */
        0,                          /* has upvals */
        NULL,                       /* no upvals */
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        0,                          /* has constants */
        NULL,                       /* no const */
        be_str_weak(_anonymous_),
        &be_const_str_solidified,
        ( &(const binstruction[ 1]) {  /* code */
          0x80000000,  //  0000  RET	0
        })
      ),
    }),
    1,                          /* has constants */
    &be_ktab_class_lvh_page,     /* shared constants */
    be_str_weak(get_delete),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x8C04010F,  //  0000  GETMET	R1	R0	K15
      0x7C040200,  //  0001  CALL	R1	1
      0x84040000,  //  0002  CLOSURE	R1	P0
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: remove_obj
********************************************************************/
be_local_closure(class_lvh_page_remove_obj,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_page,     /* shared constants */
    be_str_weak(remove_obj),
    &be_const_str_solidified,
    ( &(const binstruction[20]) {  /* code */
      0x88080109,  //  0000  GETMBR	R2	R0	K9
      0x8C080512,  //  0001  GETMET	R2	R2	K18
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x880C0109,  //  0004  GETMBR	R3	R0	K9
      0x8C0C0713,  //  0005  GETMET	R3	R3	K19
      0x5C140200,  //  0006  MOVE	R5	R1
      0x7C0C0400,  //  0007  CALL	R3	2
      0x780A0009,  //  0008  JMPF	R2	#0013
      0x600C0018,  //  0009  GETGBL	R3	G24
      0x58100014,  //  000A  LDCONST	R4	K20
      0x88140515,  //  000B  GETMBR	R5	R2	K21
      0x8C140B16,  //  000C  GETMET	R5	R5	K22
      0x7C140200,  //  000D  CALL	R5	1
      0x5C180200,  //  000E  MOVE	R6	R1
      0x7C0C0600,  //  000F  CALL	R3	3
      0xB8122E00,  //  0010  GETNGBL	R4	K23
      0x4C140000,  //  0011  LDNIL	R5
      0x90100605,  //  0012  SETMBR	R4	R3	R5
      0x80000000,  //  0013  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_obj
********************************************************************/
be_local_closure(class_lvh_page_get_obj,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_page,     /* shared constants */
    be_str_weak(get_obj),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x88080109,  //  0000  GETMBR	R2	R0	K9
      0x8C080512,  //  0001  GETMET	R2	R2	K18
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x80040400,  //  0004  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: add_obj
********************************************************************/
be_local_closure(class_lvh_page_add_obj,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_page,     /* shared constants */
    be_str_weak(add_obj),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x880C0109,  //  0000  GETMBR	R3	R0	K9
      0x980C0202,  //  0001  SETIDX	R3	R1	R2
      0x600C0018,  //  0002  GETGBL	R3	G24
      0x58100014,  //  0003  LDCONST	R4	K20
      0x88140515,  //  0004  GETMBR	R5	R2	K21
      0x8C140B16,  //  0005  GETMET	R5	R5	K22
      0x7C140200,  //  0006  CALL	R5	1
      0x5C180200,  //  0007  MOVE	R6	R1
      0x7C0C0600,  //  0008  CALL	R3	3
      0xB8122E00,  //  0009  GETNGBL	R4	K23
      0x90100602,  //  000A  SETMBR	R4	R3	R2
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_lvh_page_init,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_page,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[47]) {  /* code */
      0xA40E2E00,  //  0000  IMPORT	R3	K23
      0x90023002,  //  0001  SETMBR	R0	K24	R2
      0x60100009,  //  0002  GETGBL	R4	G9
      0x5C140200,  //  0003  MOVE	R5	R1
      0x7C100200,  //  0004  CALL	R4	1
      0x5C040800,  //  0005  MOVE	R1	R4
      0x4C100000,  //  0006  LDNIL	R4
      0x1C100204,  //  0007  EQ	R4	R1	R4
      0x78120000,  //  0008  JMPF	R4	#000A
      0x58040010,  //  0009  LDCONST	R1	K16
      0x90023201,  //  000A  SETMBR	R0	K25	R1
      0x60100013,  //  000B  GETGBL	R4	G19
      0x7C100000,  //  000C  CALL	R4	0
      0x90021204,  //  000D  SETMBR	R0	K9	R4
      0x1C10030D,  //  000E  EQ	R4	R1	K13
      0x78120004,  //  000F  JMPF	R4	#0015
      0xB8123400,  //  0010  GETNGBL	R4	K26
      0x8C10091B,  //  0011  GETMET	R4	R4	K27
      0x7C100200,  //  0012  CALL	R4	1
      0x90022204,  //  0013  SETMBR	R0	K17	R4
      0x70020004,  //  0014  JMP		#001A
      0xB8123400,  //  0015  GETNGBL	R4	K26
      0x8C10091C,  //  0016  GETMET	R4	R4	K28
      0x5818000D,  //  0017  LDCONST	R6	K13
      0x7C100400,  //  0018  CALL	R4	2
      0x90022204,  //  0019  SETMBR	R0	K17	R4
      0x88100118,  //  001A  GETMBR	R4	R0	K24
      0x8810091D,  //  001B  GETMBR	R4	R4	K29
      0x5C140800,  //  001C  MOVE	R5	R4
      0x4C180000,  //  001D  LDNIL	R6
      0x5C1C0000,  //  001E  MOVE	R7	R0
      0x4C200000,  //  001F  LDNIL	R8
      0x88240111,  //  0020  GETMBR	R9	R0	K17
      0x7C140800,  //  0021  CALL	R5	4
      0x88180109,  //  0022  GETMBR	R6	R0	K9
      0x981A1A05,  //  0023  SETIDX	R6	K13	R5
      0x60180018,  //  0024  GETGBL	R6	G24
      0x581C001E,  //  0025  LDCONST	R7	K30
      0x88200119,  //  0026  GETMBR	R8	R0	K25
      0x7C180400,  //  0027  CALL	R6	2
      0x900C0C00,  //  0028  SETMBR	R3	R6	R0
      0x60180018,  //  0029  GETGBL	R6	G24
      0x581C001F,  //  002A  LDCONST	R7	K31
      0x88200119,  //  002B  GETMBR	R8	R0	K25
      0x7C180400,  //  002C  CALL	R6	2
      0x900C0C05,  //  002D  SETMBR	R3	R6	R5
      0x80000000,  //  002E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: show
********************************************************************/
be_local_closure(class_lvh_page_show,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 2]) {
      be_nested_proto(
        3,                          /* nstack */
        0,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 3),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 2]) {     /* constants */
        /* K0   */  be_nested_str_weak(tasmota),
        /* K1   */  be_nested_str_weak(publish_rule),
        }),
        be_str_weak(_X3Clambda_X3E),
        &be_const_str_solidified,
        ( &(const binstruction[ 5]) {  /* code */
          0xB8020000,  //  0000  GETNGBL	R0	K0
          0x8C000101,  //  0001  GETMET	R0	R0	K1
          0x68080000,  //  0002  GETUPV	R2	U0
          0x7C000400,  //  0003  CALL	R0	2
          0x80040000,  //  0004  RET	1	R0
        })
      ),
      be_nested_proto(
        3,                          /* nstack */
        0,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 4),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 2]) {     /* constants */
        /* K0   */  be_nested_str_weak(tasmota),
        /* K1   */  be_nested_str_weak(publish_rule),
        }),
        be_str_weak(_X3Clambda_X3E),
        &be_const_str_solidified,
        ( &(const binstruction[ 5]) {  /* code */
          0xB8020000,  //  0000  GETNGBL	R0	K0
          0x8C000101,  //  0001  GETMET	R0	R0	K1
          0x68080000,  //  0002  GETUPV	R2	U0
          0x7C000400,  //  0003  CALL	R0	2
          0x80040000,  //  0004  RET	1	R0
        })
      ),
    }),
    1,                          /* has constants */
    &be_ktab_class_lvh_page,     /* shared constants */
    be_str_weak(show),
    &be_const_str_solidified,
    ( &(const binstruction[71]) {  /* code */
      0x880C0111,  //  0000  GETMBR	R3	R0	K17
      0x4C100000,  //  0001  LDNIL	R4
      0x1C0C0604,  //  0002  EQ	R3	R3	R4
      0x780E0001,  //  0003  JMPF	R3	#0006
      0x4C0C0000,  //  0004  LDNIL	R3
      0x80040600,  //  0005  RET	1	R3
      0x880C0111,  //  0006  GETMBR	R3	R0	K17
      0x880C0720,  //  0007  GETMBR	R3	R3	K32
      0xB8123400,  //  0008  GETNGBL	R4	K26
      0x8C100921,  //  0009  GETMET	R4	R4	K33
      0x7C100200,  //  000A  CALL	R4	1
      0x88100920,  //  000B  GETMBR	R4	R4	K32
      0x1C0C0604,  //  000C  EQ	R3	R3	R4
      0x780E0000,  //  000D  JMPF	R3	#000F
      0x80000600,  //  000E  RET	0
      0x4C0C0000,  //  000F  LDNIL	R3
      0x1C0C0403,  //  0010  EQ	R3	R2	R3
      0x780E0000,  //  0011  JMPF	R3	#0013
      0x540A01F3,  //  0012  LDINT	R2	500
      0x4C0C0000,  //  0013  LDNIL	R3
      0x1C0C0203,  //  0014  EQ	R3	R1	R3
      0x780E0005,  //  0015  JMPF	R3	#001C
      0x880C0118,  //  0016  GETMBR	R3	R0	K24
      0x8C0C0722,  //  0017  GETMET	R3	R3	K34
      0x8C140116,  //  0018  GETMET	R5	R0	K22
      0x7C140200,  //  0019  CALL	R5	1
      0x7C0C0400,  //  001A  CALL	R3	2
      0x5C040600,  //  001B  MOVE	R1	R3
      0x600C0018,  //  001C  GETGBL	R3	G24
      0x58100023,  //  001D  LDCONST	R4	K35
      0x88140118,  //  001E  GETMBR	R5	R0	K24
      0x88140B24,  //  001F  GETMBR	R5	R5	K36
      0x7C0C0400,  //  0020  CALL	R3	2
      0xB8124A00,  //  0021  GETNGBL	R4	K37
      0x8C100926,  //  0022  GETMET	R4	R4	K38
      0x84180000,  //  0023  CLOSURE	R6	P0
      0x7C100400,  //  0024  CALL	R4	2
      0x60100018,  //  0025  GETGBL	R4	G24
      0x58140027,  //  0026  LDCONST	R5	K39
      0x88180119,  //  0027  GETMBR	R6	R0	K25
      0x7C100400,  //  0028  CALL	R4	2
      0xB8164A00,  //  0029  GETNGBL	R5	K37
      0x8C140B26,  //  002A  GETMET	R5	R5	K38
      0x841C0001,  //  002B  CLOSURE	R7	P1
      0x7C140400,  //  002C  CALL	R5	2
      0x88140118,  //  002D  GETMBR	R5	R0	K24
      0x88180119,  //  002E  GETMBR	R6	R0	K25
      0x90164806,  //  002F  SETMBR	R5	K36	R6
      0x1C14030D,  //  0030  EQ	R5	R1	K13
      0x78160004,  //  0031  JMPF	R5	#0037
      0xB8163400,  //  0032  GETNGBL	R5	K26
      0x8C140B28,  //  0033  GETMET	R5	R5	K40
      0x881C0111,  //  0034  GETMBR	R7	R0	K17
      0x7C140400,  //  0035  CALL	R5	2
      0x7002000D,  //  0036  JMP		#0045
      0x88140129,  //  0037  GETMBR	R5	R0	K41
      0x8C140B12,  //  0038  GETMET	R5	R5	K18
      0x5C1C0200,  //  0039  MOVE	R7	R1
      0xB8223400,  //  003A  GETNGBL	R8	K26
      0x8820112A,  //  003B  GETMBR	R8	R8	K42
      0x7C140600,  //  003C  CALL	R5	3
      0xB81A3400,  //  003D  GETNGBL	R6	K26
      0x8C180D2B,  //  003E  GETMET	R6	R6	K43
      0x88200111,  //  003F  GETMBR	R8	R0	K17
      0x5C240A00,  //  0040  MOVE	R9	R5
      0x5C280400,  //  0041  MOVE	R10	R2
      0x582C000D,  //  0042  LDCONST	R11	K13
      0x50300000,  //  0043  LDBOOL	R12	0	0
      0x7C180C00,  //  0044  CALL	R6	6
      0xA0000000,  //  0045  CLOSE	R0
      0x80000000,  //  0046  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _delete
********************************************************************/
be_local_closure(class_lvh_page__delete,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_page,     /* shared constants */
    be_str_weak(_delete),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x88040118,  //  0000  GETMBR	R1	R0	K24
      0x8C04032C,  //  0001  GETMET	R1	R1	K44
      0x880C0119,  //  0002  GETMBR	R3	R0	K25
      0x7C040400,  //  0003  CALL	R1	2
      0x8C040100,  //  0004  GETMET	R1	R0	K0
      0x7C040200,  //  0005  CALL	R1	1
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: id
********************************************************************/
be_local_closure(class_lvh_page_id,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lvh_page,     /* shared constants */
    be_str_weak(id),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040119,  //  0000  GETMBR	R1	R0	K25
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: lvh_page
********************************************************************/
be_local_class(lvh_page,
    7,
    NULL,
    be_nested_map(20,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(show_anim, 1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(5,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(0, 3), be_const_int(0) },
        { be_const_key_int(1, -1), be_const_int(5) },
        { be_const_key_int(2, -1), be_const_int(8) },
        { be_const_key_int(-1, -1), be_const_int(6) },
        { be_const_key_int(-2, -1), be_const_int(7) },
    }))    ) } )) },
        { be_const_key_weak(id, 3), be_const_closure(class_lvh_page_id_closure) },
        { be_const_key_weak(_obj_id, -1), be_const_var(0) },
        { be_const_key_weak(show, 18), be_const_closure(class_lvh_page_show_closure) },
        { be_const_key_weak(_lv_scr, 19), be_const_var(2) },
        { be_const_key_weak(get_clear, 2), be_const_closure(class_lvh_page_get_clear_closure) },
        { be_const_key_weak(back, -1), be_const_var(6) },
        { be_const_key_weak(get_delete, -1), be_const_closure(class_lvh_page_get_delete_closure) },
        { be_const_key_weak(remove_obj, 4), be_const_closure(class_lvh_page_remove_obj_closure) },
        { be_const_key_weak(_hm, -1), be_const_var(3) },
        { be_const_key_weak(_page_id, 8), be_const_var(1) },
        { be_const_key_weak(member, 10), be_const_closure(class_lvh_page_member_closure) },
        { be_const_key_weak(next, -1), be_const_var(5) },
        { be_const_key_weak(get_obj, -1), be_const_closure(class_lvh_page_get_obj_closure) },
        { be_const_key_weak(add_obj, -1), be_const_closure(class_lvh_page_add_obj_closure) },
        { be_const_key_weak(init, -1), be_const_closure(class_lvh_page_init_closure) },
        { be_const_key_weak(prev, 0), be_const_var(4) },
        { be_const_key_weak(_delete, -1), be_const_closure(class_lvh_page__delete_closure) },
        { be_const_key_weak(get_scr, -1), be_const_closure(class_lvh_page_get_scr_closure) },
        { be_const_key_weak(_clear, -1), be_const_closure(class_lvh_page__clear_closure) },
    })),
    be_str_weak(lvh_page)
);
extern const bclass be_class_HASPmota;
// compact class 'HASPmota' ktab size: 151, total: 246 (saved 760 bytes)
static const bvalue be_ktab_class_HASPmota[151] = {
  /* K0   */  be_nested_str_weak(started),
  /* K1   */  be_nested_str_weak(string),
  /* K2   */  be_nested_str_weak(PAGES_JSONL),
  /* K3   */  be_nested_str_weak(path),
  /* K4   */  be_nested_str_weak(exists),
  /* K5   */  be_nested_str_weak(file_X20_X27),
  /* K6   */  be_nested_str_weak(_X27_X20not_X20found),
  /* K7   */  be_nested_str_weak(io_erorr),
  /* K8   */  be_nested_str_weak(lv),
  /* K9   */  be_nested_str_weak(start),
  /* K10  */  be_nested_str_weak(hres),
  /* K11  */  be_nested_str_weak(get_hor_res),
  /* K12  */  be_nested_str_weak(vres),
  /* K13  */  be_nested_str_weak(get_ver_res),
  /* K14  */  be_nested_str_weak(scr),
  /* K15  */  be_nested_str_weak(scr_act),
  /* K16  */  be_nested_str_weak(r12),
  /* K17  */  be_nested_str_weak(font_embedded),
  /* K18  */  be_nested_str_weak(robotocondensed),
  /* K19  */  be_nested_str_weak(montserrat),
  /* K20  */  be_nested_str_weak(r16),
  /* K21  */  be_nested_str_weak(r24),
  /* K22  */  be_nested_str_weak(lvh_root),
  /* K23  */  be_nested_str_weak(parse_color),
  /* K24  */  be_nested_str_weak(tasmota),
  /* K25  */  be_nested_str_weak(webcolor),
  /* K26  */  be_const_int(1),
  /* K27  */  be_const_int(2),
  /* K28  */  be_nested_str_weak(color_arr),
  /* K29  */  be_nested_str_weak(theme_haspmota_init),
  /* K30  */  be_const_int(0),
  /* K31  */  be_nested_str_weak(get_disp),
  /* K32  */  be_nested_str_weak(set_theme),
  /* K33  */  be_nested_str_weak(theme_apply),
  /* K34  */  be_nested_str_weak(layer_top),
  /* K35  */  be_nested_str_weak(set_style_bg_opa),
  /* K36  */  be_nested_str_weak(lvh_pages),
  /* K37  */  be_nested_str_weak(_load),
  /* K38  */  be_nested_str_weak(log),
  /* K39  */  be_nested_str_weak(HSP_X3A_X20HASPmota_X20initialized),
  /* K40  */  be_nested_str_weak(re),
  /* K41  */  be_nested_str_weak(get_page_cur),
  /* K42  */  be_nested_str_weak(pages_list_sorted),
  /* K43  */  be_nested_str_weak(lvh_page_cur_idx),
  /* K44  */  be_nested_str_weak(prev),
  /* K45  */  be_nested_str_weak(next),
  /* K46  */  be_nested_str_weak(back),
  /* K47  */  be_nested_str_weak(delete),
  /* K48  */  be_nested_str_weak(id),
  /* K49  */  be_nested_str_weak(match),
  /* K50  */  be_nested_str_weak(re_page_target),
  /* K51  */  be_nested_str_weak(show),
  /* K52  */  be_nested_str_weak(keys),
  /* K53  */  be_nested_str_weak(push),
  /* K54  */  be_nested_str_weak(stop_iteration),
  /* K55  */  be_nested_str_weak(sort),
  /* K56  */  be_nested_str_weak(find),
  /* K57  */  be_nested_str_weak(auto_dimming),
  /* K58  */  be_nested_str_weak(introspect),
  /* K59  */  be_nested_str_weak(toptr),
  /* K60  */  be_nested_str_weak(event),
  /* K61  */  be_nested_str_weak(_p),
  /* K62  */  be_nested_str_weak(lv_event),
  /* K63  */  be_nested_str_weak(get_user_data),
  /* K64  */  be_nested_str_weak(fromptr),
  /* K65  */  be_nested_str_weak(instance),
  /* K66  */  be_nested_str_weak(event_cb),
  /* K67  */  be_nested_str_weak(Antiburn),
  /* K68  */  be_nested_str_weak(json),
  /* K69  */  be_nested_str_weak(tell),
  /* K70  */  be_nested_str_weak(size),
  /* K71  */  be_nested_str_weak(readline),
  /* K72  */  be_nested_str_weak(_X23),
  /* K73  */  be_nested_str_weak(load),
  /* K74  */  be_nested_str_weak(loglevel),
  /* K75  */  be_nested_str_weak(endswith),
  /* K76  */  be_nested_str_weak(_X0A),
  /* K77  */  be_nested_str_weak(HSP_X3A_X20parsing_X20line_X20_X27_X25s_X27),
  /* K78  */  be_nested_str_weak(parse_page),
  /* K79  */  be_nested_str_weak(value_error),
  /* K80  */  be_nested_str_weak(no_X20page_X20_X27id_X27_X20defined),
  /* K81  */  be_nested_str_weak(parse_obj),
  /* K82  */  be_nested_str_weak(lvh_page_cur_idx_parsing),
  /* K83  */  be_nested_str_weak(tr),
  /* K84  */  be_nested_str_weak(_X20_X09),
  /* K85  */  be_nested_str_weak(),
  /* K86  */  be_nested_str_weak(HSP_X3A_X20invalid_X20JSON_X20line_X20_X27_X25s_X27),
  /* K87  */  be_nested_str_weak(close),
  /* K88  */  be_nested_str_weak(no_X20page_X20object_X20defined),
  /* K89  */  be_const_class(be_class_HASPmota),
  /* K90  */  be_nested_str_weak(get),
  /* K91  */  be_nested_str_weak(version),
  /* K92  */  be_nested_str_weak(int),
  /* K93  */  be_nested_str_weak(has),
  /* K94  */  be_nested_str_weak(page),
  /* K95  */  be_nested_str_weak(contains),
  /* K96  */  be_nested_str_weak(lvh_page),
  /* K97  */  be_nested_str_weak(get_page_cur_parsing),
  /* K98  */  be_nested_str_weak(remove),
  /* K99  */  be_nested_str_weak(_page_id),
  /* K100 */  be_nested_str_weak(dimming_time),
  /* K101 */  be_nested_str_weak(dimming_min),
  /* K102 */  be_nested_str_weak(dimming_duration),
  /* K103 */  be_nested_str_weak(antiburn_time),
  /* K104 */  be_nested_str_weak(antiburn),
  /* K105 */  be_nested_str_weak(unable_X20to_X20parse_X20JSON_X20line),
  /* K106 */  be_nested_str_weak(global),
  /* K107 */  be_nested_str_weak(obj),
  /* K108 */  be_nested_str_weak(HSP_X3A_X20invalid_X20_X27id_X27_X3A_X20_X25s_X20for_X20_X27obj_X27_X3A_X20_X25s),
  /* K109 */  be_nested_str_weak(get_obj),
  /* K110 */  be_nested_str_weak(parentid),
  /* K111 */  be_nested_str_weak(_lv_obj),
  /* K112 */  be_nested_str_weak(get_scr),
  /* K113 */  be_nested_str_weak(lvh_),
  /* K114 */  be_nested_str_weak(class),
  /* K115 */  be_nested_str_weak(lvh_obj),
  /* K116 */  be_nested_str_weak(module),
  /* K117 */  be_nested_str_weak(HSP_X3A_X20Cannot_X20find_X20object_X20of_X20type_X20_X25s),
  /* K118 */  be_nested_str_weak(add_obj),
  /* K119 */  be_nested_str_weak(HSP_X3A_X20cannot_X20specify_X20_X27obj_X27_X3A_X27_X25s_X27_X20for_X20_X27id_X27_X3A0),
  /* K120 */  be_nested_str_weak(post_config),
  /* K121 */  be_nested_str_weak(berry_run),
  /* K122 */  be_nested_str_weak(nil),
  /* K123 */  be_nested_str_weak(HSP_X3A_X20unable_X20to_X20compile_X20berry_X20code_X20_X22_X25s_X22_X20_X2D_X20_X27_X25s_X27_X20_X2D_X20_X25s),
  /* K124 */  be_nested_str_weak(function),
  /* K125 */  be_nested_str_weak(HSP_X3A_X20unable_X20to_X20run_X20berry_X20code_X20_X22_X25s_X22_X20_X2D_X20_X27_X25s_X27_X20_X2D_X20_X25s),
  /* K126 */  be_nested_str_weak(fix_lv_version),
  /* K127 */  be_nested_str_weak(compilebytes),
  /* K128 */  be_nested_str_weak(p_X5Cd_X2B),
  /* K129 */  be_nested_str_weak(DIMMING_DURATION),
  /* K130 */  be_nested_str_weak(dimming_state),
  /* K131 */  be_nested_str_weak(add_driver),
  /* K132 */  be_nested_str_weak(display),
  /* K133 */  be_nested_str_weak(disp),
  /* K134 */  be_nested_str_weak(get_inactive_time),
  /* K135 */  be_nested_str_weak(dimmed_panel),
  /* K136 */  be_nested_str_weak(stop),
  /* K137 */  be_nested_str_weak(dimmer),
  /* K138 */  be_nested_str_weak(off),
  /* K139 */  be_nested_str_weak(DimmedPanel),
  /* K140 */  be_nested_str_weak(short),
  /* K141 */  be_nested_str_weak(long),
  /* K142 */  be_nested_str_weak(_X7B_X22hasp_X22_X3A_X7B_X22p0b0_X22_X3A_X7B_X22idle_X22_X3A_X22_X25s_X22_X7D_X7D_X7D),
  /* K143 */  be_nested_str_weak(defer),
  /* K144 */  be_nested_str_weak(EVENT_CLICKED),
  /* K145 */  be_nested_str_weak(page_show),
  /* K146 */  be_nested_str_weak(_action),
  /* K147 */  be_nested_str_weak(p_X25s),
  /* K148 */  be_nested_str_weak(cb),
  /* K149 */  be_nested_str_weak(gen_cb),
  /* K150 */  be_nested_str_weak(add_event_cb),
};


extern const bclass be_class_HASPmota;

/********************************************************************
** Solidified function: start
********************************************************************/
be_local_closure(class_HASPmota_start,   /* name */
  be_nested_proto(
    23,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_HASPmota,     /* shared constants */
    be_str_weak(start),
    &be_const_str_solidified,
    ( &(const binstruction[223]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x780E0000,  //  0001  JMPF	R3	#0003
      0x80000600,  //  0002  RET	0
      0x4C0C0000,  //  0003  LDNIL	R3
      0x60100004,  //  0004  GETGBL	R4	G4
      0x5C140200,  //  0005  MOVE	R5	R1
      0x7C100200,  //  0006  CALL	R4	1
      0x1C100901,  //  0007  EQ	R4	R4	K1
      0x78120001,  //  0008  JMPF	R4	#000B
      0x5C0C0200,  //  0009  MOVE	R3	R1
      0x70020007,  //  000A  JMP		#0013
      0x60100004,  //  000B  GETGBL	R4	G4
      0x5C140400,  //  000C  MOVE	R5	R2
      0x7C100200,  //  000D  CALL	R4	1
      0x1C100901,  //  000E  EQ	R4	R4	K1
      0x78120001,  //  000F  JMPF	R4	#0012
      0x5C0C0400,  //  0010  MOVE	R3	R2
      0x70020000,  //  0011  JMP		#0013
      0x880C0102,  //  0012  GETMBR	R3	R0	K2
      0xA4120600,  //  0013  IMPORT	R4	K3
      0x8C140904,  //  0014  GETMET	R5	R4	K4
      0x5C1C0600,  //  0015  MOVE	R7	R3
      0x7C140400,  //  0016  CALL	R5	2
      0x74160002,  //  0017  JMPT	R5	#001B
      0x00160A03,  //  0018  ADD	R5	K5	R3
      0x00140B06,  //  0019  ADD	R5	R5	K6
      0xB0060E05,  //  001A  RAISE	1	K7	R5
      0xB8161000,  //  001B  GETNGBL	R5	K8
      0x8C140B09,  //  001C  GETMET	R5	R5	K9
      0x7C140200,  //  001D  CALL	R5	1
      0xB8161000,  //  001E  GETNGBL	R5	K8
      0x8C140B0B,  //  001F  GETMET	R5	R5	K11
      0x7C140200,  //  0020  CALL	R5	1
      0x90021405,  //  0021  SETMBR	R0	K10	R5
      0xB8161000,  //  0022  GETNGBL	R5	K8
      0x8C140B0D,  //  0023  GETMET	R5	R5	K13
      0x7C140200,  //  0024  CALL	R5	1
      0x90021805,  //  0025  SETMBR	R0	K12	R5
      0xB8161000,  //  0026  GETNGBL	R5	K8
      0x8C140B0F,  //  0027  GETMET	R5	R5	K15
      0x7C140200,  //  0028  CALL	R5	1
      0x90021C05,  //  0029  SETMBR	R0	K14	R5
      0xA8020007,  //  002A  EXBLK	0	#0033
      0xB8161000,  //  002B  GETNGBL	R5	K8
      0x8C140B11,  //  002C  GETMET	R5	R5	K17
      0x581C0012,  //  002D  LDCONST	R7	K18
      0x5422000B,  //  002E  LDINT	R8	12
      0x7C140600,  //  002F  CALL	R5	3
      0x90022005,  //  0030  SETMBR	R0	K16	R5
      0xA8040001,  //  0031  EXBLK	1	1
      0x70020009,  //  0032  JMP		#003D
      0xAC140000,  //  0033  CATCH	R5	0	0
      0x70020006,  //  0034  JMP		#003C
      0xB8161000,  //  0035  GETNGBL	R5	K8
      0x8C140B11,  //  0036  GETMET	R5	R5	K17
      0x581C0013,  //  0037  LDCONST	R7	K19
      0x54220009,  //  0038  LDINT	R8	10
      0x7C140600,  //  0039  CALL	R5	3
      0x90022005,  //  003A  SETMBR	R0	K16	R5
      0x70020000,  //  003B  JMP		#003D
      0xB0080000,  //  003C  RAISE	2	R0	R0
      0xA8020007,  //  003D  EXBLK	0	#0046
      0xB8161000,  //  003E  GETNGBL	R5	K8
      0x8C140B11,  //  003F  GETMET	R5	R5	K17
      0x581C0012,  //  0040  LDCONST	R7	K18
      0x5422000F,  //  0041  LDINT	R8	16
      0x7C140600,  //  0042  CALL	R5	3
      0x90022805,  //  0043  SETMBR	R0	K20	R5
      0xA8040001,  //  0044  EXBLK	1	1
      0x70020009,  //  0045  JMP		#0050
      0xAC140000,  //  0046  CATCH	R5	0	0
      0x70020006,  //  0047  JMP		#004F
      0xB8161000,  //  0048  GETNGBL	R5	K8
      0x8C140B11,  //  0049  GETMET	R5	R5	K17
      0x581C0013,  //  004A  LDCONST	R7	K19
      0x5422000D,  //  004B  LDINT	R8	14
      0x7C140600,  //  004C  CALL	R5	3
      0x90022805,  //  004D  SETMBR	R0	K20	R5
      0x70020000,  //  004E  JMP		#0050
      0xB0080000,  //  004F  RAISE	2	R0	R0
      0xA8020007,  //  0050  EXBLK	0	#0059
      0xB8161000,  //  0051  GETNGBL	R5	K8
      0x8C140B11,  //  0052  GETMET	R5	R5	K17
      0x581C0012,  //  0053  LDCONST	R7	K18
      0x54220017,  //  0054  LDINT	R8	24
      0x7C140600,  //  0055  CALL	R5	3
      0x90022A05,  //  0056  SETMBR	R0	K21	R5
      0xA8040001,  //  0057  EXBLK	1	1
      0x70020009,  //  0058  JMP		#0063
      0xAC140000,  //  0059  CATCH	R5	0	0
      0x70020006,  //  005A  JMP		#0062
      0xB8161000,  //  005B  GETNGBL	R5	K8
      0x8C140B11,  //  005C  GETMET	R5	R5	K17
      0x581C0013,  //  005D  LDCONST	R7	K19
      0x54220013,  //  005E  LDINT	R8	20
      0x7C140600,  //  005F  CALL	R5	3
      0x90022A05,  //  0060  SETMBR	R0	K21	R5
      0x70020000,  //  0061  JMP		#0063
      0xB0080000,  //  0062  RAISE	2	R0	R0
      0x88140116,  //  0063  GETMBR	R5	R0	K22
      0x8C140B17,  //  0064  GETMET	R5	R5	K23
      0xB81E3000,  //  0065  GETNGBL	R7	K24
      0x8C1C0F19,  //  0066  GETMET	R7	R7	K25
      0x54260009,  //  0067  LDINT	R9	10
      0x7C1C0400,  //  0068  CALL	R7	2
      0x7C140400,  //  0069  CALL	R5	2
      0x88180116,  //  006A  GETMBR	R6	R0	K22
      0x8C180D17,  //  006B  GETMET	R6	R6	K23
      0xB8223000,  //  006C  GETNGBL	R8	K24
      0x8C201119,  //  006D  GETMET	R8	R8	K25
      0x542A000A,  //  006E  LDINT	R10	11
      0x7C200400,  //  006F  CALL	R8	2
      0x7C180400,  //  0070  CALL	R6	2
      0x881C0116,  //  0071  GETMBR	R7	R0	K22
      0x8C1C0F17,  //  0072  GETMET	R7	R7	K23
      0xB8263000,  //  0073  GETNGBL	R9	K24
      0x8C241319,  //  0074  GETMET	R9	R9	K25
      0x582C001A,  //  0075  LDCONST	R11	K26
      0x7C240400,  //  0076  CALL	R9	2
      0x7C1C0400,  //  0077  CALL	R7	2
      0x88200116,  //  0078  GETMBR	R8	R0	K22
      0x8C201117,  //  0079  GETMET	R8	R8	K23
      0xB82A3000,  //  007A  GETNGBL	R10	K24
      0x8C281519,  //  007B  GETMET	R10	R10	K25
      0x54320008,  //  007C  LDINT	R12	9
      0x7C280400,  //  007D  CALL	R10	2
      0x7C200400,  //  007E  CALL	R8	2
      0x88240116,  //  007F  GETMBR	R9	R0	K22
      0x8C241317,  //  0080  GETMET	R9	R9	K23
      0xB82E3000,  //  0081  GETNGBL	R11	K24
      0x8C2C1719,  //  0082  GETMET	R11	R11	K25
      0x5834001B,  //  0083  LDCONST	R13	K27
      0x7C2C0400,  //  0084  CALL	R11	2
      0x7C240400,  //  0085  CALL	R9	2
      0x88280116,  //  0086  GETMBR	R10	R0	K22
      0x8C281517,  //  0087  GETMET	R10	R10	K23
      0xB8323000,  //  0088  GETNGBL	R12	K24
      0x8C301919,  //  0089  GETMET	R12	R12	K25
      0x5838001B,  //  008A  LDCONST	R14	K27
      0x7C300400,  //  008B  CALL	R12	2
      0x7C280400,  //  008C  CALL	R10	2
      0x882C0116,  //  008D  GETMBR	R11	R0	K22
      0x8C2C1717,  //  008E  GETMET	R11	R11	K23
      0xB8363000,  //  008F  GETNGBL	R13	K24
      0x8C341B19,  //  0090  GETMET	R13	R13	K25
      0x543E000B,  //  0091  LDINT	R15	12
      0x7C340400,  //  0092  CALL	R13	2
      0x7C2C0400,  //  0093  CALL	R11	2
      0x88300116,  //  0094  GETMBR	R12	R0	K22
      0x8C301917,  //  0095  GETMET	R12	R12	K23
      0xB83A3000,  //  0096  GETNGBL	R14	K24
      0x8C381D19,  //  0097  GETMET	R14	R14	K25
      0x5442000C,  //  0098  LDINT	R16	13
      0x7C380400,  //  0099  CALL	R14	2
      0x7C300400,  //  009A  CALL	R12	2
      0x88340116,  //  009B  GETMBR	R13	R0	K22
      0x8C341B17,  //  009C  GETMET	R13	R13	K23
      0xB83E3000,  //  009D  GETNGBL	R15	K24
      0x8C3C1F19,  //  009E  GETMET	R15	R15	K25
      0x5446000D,  //  009F  LDINT	R17	14
      0x7C3C0400,  //  00A0  CALL	R15	2
      0x7C340400,  //  00A1  CALL	R13	2
      0x88380116,  //  00A2  GETMBR	R14	R0	K22
      0x8C381D17,  //  00A3  GETMET	R14	R14	K23
      0xB8423000,  //  00A4  GETNGBL	R16	K24
      0x8C402119,  //  00A5  GETMET	R16	R16	K25
      0x544A000E,  //  00A6  LDINT	R18	15
      0x7C400400,  //  00A7  CALL	R16	2
      0x7C380400,  //  00A8  CALL	R14	2
      0xB83E1000,  //  00A9  GETNGBL	R15	K8
      0x8C3C1F1C,  //  00AA  GETMET	R15	R15	K28
      0x60440012,  //  00AB  GETGBL	R17	G18
      0x7C440000,  //  00AC  CALL	R17	0
      0x40482205,  //  00AD  CONNECT	R18	R17	R5
      0x40482206,  //  00AE  CONNECT	R18	R17	R6
      0x40482207,  //  00AF  CONNECT	R18	R17	R7
      0x40482208,  //  00B0  CONNECT	R18	R17	R8
      0x40482209,  //  00B1  CONNECT	R18	R17	R9
      0x4048220A,  //  00B2  CONNECT	R18	R17	R10
      0x4048220B,  //  00B3  CONNECT	R18	R17	R11
      0x4048220C,  //  00B4  CONNECT	R18	R17	R12
      0x4048220D,  //  00B5  CONNECT	R18	R17	R13
      0x4048220E,  //  00B6  CONNECT	R18	R17	R14
      0x7C3C0400,  //  00B7  CALL	R15	2
      0xB8421000,  //  00B8  GETNGBL	R16	K8
      0x8C40211D,  //  00B9  GETMET	R16	R16	K29
      0x5848001E,  //  00BA  LDCONST	R18	K30
      0x5C4C1E00,  //  00BB  MOVE	R19	R15
      0x88500110,  //  00BC  GETMBR	R20	R0	K16
      0x88540114,  //  00BD  GETMBR	R21	R0	K20
      0x88580115,  //  00BE  GETMBR	R22	R0	K21
      0x7C400C00,  //  00BF  CALL	R16	6
      0x8844010E,  //  00C0  GETMBR	R17	R0	K14
      0x8C44231F,  //  00C1  GETMET	R17	R17	K31
      0x7C440200,  //  00C2  CALL	R17	1
      0x8C442320,  //  00C3  GETMET	R17	R17	K32
      0x5C4C2000,  //  00C4  MOVE	R19	R16
      0x7C440400,  //  00C5  CALL	R17	2
      0xB8461000,  //  00C6  GETNGBL	R17	K8
      0x8C442321,  //  00C7  GETMET	R17	R17	K33
      0xB84E1000,  //  00C8  GETNGBL	R19	K8
      0x8C4C2722,  //  00C9  GETMET	R19	R19	K34
      0x7C4C0200,  //  00CA  CALL	R19	1
      0x7C440400,  //  00CB  CALL	R17	2
      0xB8461000,  //  00CC  GETNGBL	R17	K8
      0x8C442322,  //  00CD  GETMET	R17	R17	K34
      0x7C440200,  //  00CE  CALL	R17	1
      0x8C442323,  //  00CF  GETMET	R17	R17	K35
      0x584C001E,  //  00D0  LDCONST	R19	K30
      0x5850001E,  //  00D1  LDCONST	R20	K30
      0x7C440600,  //  00D2  CALL	R17	3
      0x60440013,  //  00D3  GETGBL	R17	G19
      0x7C440000,  //  00D4  CALL	R17	0
      0x90024811,  //  00D5  SETMBR	R0	K36	R17
      0x8C440125,  //  00D6  GETMET	R17	R0	K37
      0x5C4C0600,  //  00D7  MOVE	R19	R3
      0x7C440400,  //  00D8  CALL	R17	2
      0x50440200,  //  00D9  LDBOOL	R17	1	0
      0x90020011,  //  00DA  SETMBR	R0	K0	R17
      0xB8464C00,  //  00DB  GETNGBL	R17	K38
      0x58480027,  //  00DC  LDCONST	R18	K39
      0x7C440200,  //  00DD  CALL	R17	1
      0x80000000,  //  00DE  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: page_show
********************************************************************/
be_local_closure(class_HASPmota_page_show,   /* name */
  be_nested_proto(
    14,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 1]) {
      be_nested_proto(
        6,                          /* nstack */
        3,                          /* argc */
        0,                          /* varg */
        0,                          /* has upvals */
        NULL,                       /* no upvals */
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 1]) {     /* constants */
        /* K0   */  be_nested_str_weak(find),
        }),
        be_str_weak(to_page_resolve),
        &be_const_str_solidified,
        ( &(const binstruction[13]) {  /* code */
          0x4C0C0000,  //  0000  LDNIL	R3
          0x200C0003,  //  0001  NE	R3	R0	R3
          0x780E0007,  //  0002  JMPF	R3	#000B
          0x8C0C0500,  //  0003  GETMET	R3	R2	K0
          0x5C140000,  //  0004  MOVE	R5	R0
          0x7C0C0400,  //  0005  CALL	R3	2
          0x4C100000,  //  0006  LDNIL	R4
          0x200C0604,  //  0007  NE	R3	R3	R4
          0x780E0001,  //  0008  JMPF	R3	#000B
          0x80040000,  //  0009  RET	1	R0
          0x70020000,  //  000A  JMP		#000C
          0x80040200,  //  000B  RET	1	R1
          0x80000000,  //  000C  RET	0
        })
      ),
    }),
    1,                          /* has constants */
    &be_ktab_class_HASPmota,     /* shared constants */
    be_str_weak(page_show),
    &be_const_str_solidified,
    ( &(const binstruction[109]) {  /* code */
      0xA4125000,  //  0000  IMPORT	R4	K40
      0x84140000,  //  0001  CLOSURE	R5	P0
      0x4C180000,  //  0002  LDNIL	R6
      0x8C1C0129,  //  0003  GETMET	R7	R0	K41
      0x7C1C0200,  //  0004  CALL	R7	1
      0x8C20012A,  //  0005  GETMET	R8	R0	K42
      0x8828012B,  //  0006  GETMBR	R10	R0	K43
      0x7C200400,  //  0007  CALL	R8	2
      0x6024000C,  //  0008  GETGBL	R9	G12
      0x5C281000,  //  0009  MOVE	R10	R8
      0x7C240200,  //  000A  CALL	R9	1
      0x1824131A,  //  000B  LE	R9	R9	K26
      0x78260001,  //  000C  JMPF	R9	#000F
      0x4C240000,  //  000D  LDNIL	R9
      0x80041200,  //  000E  RET	1	R9
      0x1C24032C,  //  000F  EQ	R9	R1	K44
      0x78260009,  //  0010  JMPF	R9	#001B
      0x5C240A00,  //  0011  MOVE	R9	R5
      0x60280009,  //  0012  GETGBL	R10	G9
      0x882C0F2C,  //  0013  GETMBR	R11	R7	K44
      0x7C280200,  //  0014  CALL	R10	1
      0x542DFFFE,  //  0015  LDINT	R11	-1
      0x942C100B,  //  0016  GETIDX	R11	R8	R11
      0x5C301000,  //  0017  MOVE	R12	R8
      0x7C240600,  //  0018  CALL	R9	3
      0x5C181200,  //  0019  MOVE	R6	R9
      0x70020041,  //  001A  JMP		#005D
      0x1C24032D,  //  001B  EQ	R9	R1	K45
      0x78260008,  //  001C  JMPF	R9	#0026
      0x5C240A00,  //  001D  MOVE	R9	R5
      0x60280009,  //  001E  GETGBL	R10	G9
      0x882C0F2D,  //  001F  GETMBR	R11	R7	K45
      0x7C280200,  //  0020  CALL	R10	1
      0x942C111A,  //  0021  GETIDX	R11	R8	K26
      0x5C301000,  //  0022  MOVE	R12	R8
      0x7C240600,  //  0023  CALL	R9	3
      0x5C181200,  //  0024  MOVE	R6	R9
      0x70020036,  //  0025  JMP		#005D
      0x1C24032E,  //  0026  EQ	R9	R1	K46
      0x7826000B,  //  0027  JMPF	R9	#0034
      0x5C240A00,  //  0028  MOVE	R9	R5
      0x60280009,  //  0029  GETGBL	R10	G9
      0x882C0F2E,  //  002A  GETMBR	R11	R7	K46
      0x7C280200,  //  002B  CALL	R10	1
      0x8C2C012A,  //  002C  GETMET	R11	R0	K42
      0x4C340000,  //  002D  LDNIL	R13
      0x7C2C0400,  //  002E  CALL	R11	2
      0x942C171E,  //  002F  GETIDX	R11	R11	K30
      0x5C301000,  //  0030  MOVE	R12	R8
      0x7C240600,  //  0031  CALL	R9	3
      0x5C181200,  //  0032  MOVE	R6	R9
      0x70020028,  //  0033  JMP		#005D
      0x1C24032F,  //  0034  EQ	R9	R1	K47
      0x78260017,  //  0035  JMPF	R9	#004E
      0x5C240A00,  //  0036  MOVE	R9	R5
      0x60280009,  //  0037  GETGBL	R10	G9
      0x882C0F2E,  //  0038  GETMBR	R11	R7	K46
      0x7C280200,  //  0039  CALL	R10	1
      0x8C2C012A,  //  003A  GETMET	R11	R0	K42
      0x4C340000,  //  003B  LDNIL	R13
      0x7C2C0400,  //  003C  CALL	R11	2
      0x942C171E,  //  003D  GETIDX	R11	R11	K30
      0x5C301000,  //  003E  MOVE	R12	R8
      0x7C240600,  //  003F  CALL	R9	3
      0x5C181200,  //  0040  MOVE	R6	R9
      0x8C240F30,  //  0041  GETMET	R9	R7	K48
      0x7C240200,  //  0042  CALL	R9	1
      0x1C240C09,  //  0043  EQ	R9	R6	R9
      0x78260007,  //  0044  JMPF	R9	#004D
      0x5C240A00,  //  0045  MOVE	R9	R5
      0x60280009,  //  0046  GETGBL	R10	G9
      0x882C0F2D,  //  0047  GETMBR	R11	R7	K45
      0x7C280200,  //  0048  CALL	R10	1
      0x942C111A,  //  0049  GETIDX	R11	R8	K26
      0x5C301000,  //  004A  MOVE	R12	R8
      0x7C240600,  //  004B  CALL	R9	3
      0x5C181200,  //  004C  MOVE	R6	R9
      0x7002000E,  //  004D  JMP		#005D
      0x8C240931,  //  004E  GETMET	R9	R4	K49
      0x882C0132,  //  004F  GETMBR	R11	R0	K50
      0x5C300200,  //  0050  MOVE	R12	R1
      0x7C240600,  //  0051  CALL	R9	3
      0x78260009,  //  0052  JMPF	R9	#005D
      0x5C240A00,  //  0053  MOVE	R9	R5
      0x60280009,  //  0054  GETGBL	R10	G9
      0x542DFFFE,  //  0055  LDINT	R11	-1
      0x402E340B,  //  0056  CONNECT	R11	K26	R11
      0x942C020B,  //  0057  GETIDX	R11	R1	R11
      0x7C280200,  //  0058  CALL	R10	1
      0x4C2C0000,  //  0059  LDNIL	R11
      0x5C301000,  //  005A  MOVE	R12	R8
      0x7C240600,  //  005B  CALL	R9	3
      0x5C181200,  //  005C  MOVE	R6	R9
      0x4C240000,  //  005D  LDNIL	R9
      0x20240C09,  //  005E  NE	R9	R6	R9
      0x7826000B,  //  005F  JMPF	R9	#006C
      0x24240D1E,  //  0060  GT	R9	R6	K30
      0x78260009,  //  0061  JMPF	R9	#006C
      0x88240124,  //  0062  GETMBR	R9	R0	K36
      0x94241206,  //  0063  GETIDX	R9	R9	R6
      0x4C280000,  //  0064  LDNIL	R10
      0x2028120A,  //  0065  NE	R10	R9	R10
      0x782A0003,  //  0066  JMPF	R10	#006B
      0x8C281333,  //  0067  GETMET	R10	R9	K51
      0x5C300400,  //  0068  MOVE	R12	R2
      0x5C340600,  //  0069  MOVE	R13	R3
      0x7C280600,  //  006A  CALL	R10	3
      0x80041200,  //  006B  RET	1	R9
      0x80000000,  //  006C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: pages_list_sorted
********************************************************************/
be_local_closure(class_HASPmota_pages_list_sorted,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_HASPmota,     /* shared constants */
    be_str_weak(pages_list_sorted),
    &be_const_str_solidified,
    ( &(const binstruction[47]) {  /* code */
      0x60080012,  //  0000  GETGBL	R2	G18
      0x7C080000,  //  0001  CALL	R2	0
      0x1C0C031E,  //  0002  EQ	R3	R1	K30
      0x780E0000,  //  0003  JMPF	R3	#0005
      0x8804012B,  //  0004  GETMBR	R1	R0	K43
      0x600C0010,  //  0005  GETGBL	R3	G16
      0x88100124,  //  0006  GETMBR	R4	R0	K36
      0x8C100934,  //  0007  GETMET	R4	R4	K52
      0x7C100200,  //  0008  CALL	R4	1
      0x7C0C0200,  //  0009  CALL	R3	1
      0xA8020007,  //  000A  EXBLK	0	#0013
      0x5C100600,  //  000B  MOVE	R4	R3
      0x7C100000,  //  000C  CALL	R4	0
      0x2014091E,  //  000D  NE	R5	R4	K30
      0x78160002,  //  000E  JMPF	R5	#0012
      0x8C140535,  //  000F  GETMET	R5	R2	K53
      0x5C1C0800,  //  0010  MOVE	R7	R4
      0x7C140400,  //  0011  CALL	R5	2
      0x7001FFF7,  //  0012  JMP		#000B
      0x580C0036,  //  0013  LDCONST	R3	K54
      0xAC0C0200,  //  0014  CATCH	R3	1	0
      0xB0080000,  //  0015  RAISE	2	R0	R0
      0x8C0C0137,  //  0016  GETMET	R3	R0	K55
      0x5C140400,  //  0017  MOVE	R5	R2
      0x7C0C0400,  //  0018  CALL	R3	2
      0x5C080600,  //  0019  MOVE	R2	R3
      0x4C0C0000,  //  001A  LDNIL	R3
      0x1C0C0203,  //  001B  EQ	R3	R1	R3
      0x780E0000,  //  001C  JMPF	R3	#001E
      0x80040400,  //  001D  RET	1	R2
      0x600C000C,  //  001E  GETGBL	R3	G12
      0x5C100400,  //  001F  MOVE	R4	R2
      0x7C0C0200,  //  0020  CALL	R3	1
      0x00080402,  //  0021  ADD	R2	R2	R2
      0x8C100538,  //  0022  GETMET	R4	R2	K56
      0x5C180200,  //  0023  MOVE	R6	R1
      0x7C100400,  //  0024  CALL	R4	2
      0x4C140000,  //  0025  LDNIL	R5
      0x1C140805,  //  0026  EQ	R5	R4	R5
      0x78160001,  //  0027  JMPF	R5	#002A
      0x4C140000,  //  0028  LDNIL	R5
      0x80040A00,  //  0029  RET	1	R5
      0x00140803,  //  002A  ADD	R5	R4	R3
      0x04140B1A,  //  002B  SUB	R5	R5	K26
      0x40140805,  //  002C  CONNECT	R5	R4	R5
      0x94080405,  //  002D  GETIDX	R2	R2	R5
      0x80040400,  //  002E  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: every_100ms
********************************************************************/
be_local_closure(class_HASPmota_every_100ms,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_HASPmota,     /* shared constants */
    be_str_weak(every_100ms),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x8C040139,  //  0000  GETMET	R1	R0	K57
      0x7C040200,  //  0001  CALL	R1	1
      0x80000000,  //  0002  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: event_dispatch
********************************************************************/
be_local_closure(class_HASPmota_event_dispatch,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_HASPmota,     /* shared constants */
    be_str_weak(event_dispatch),
    &be_const_str_solidified,
    ( &(const binstruction[34]) {  /* code */
      0xA40A7400,  //  0000  IMPORT	R2	K58
      0x8C0C053B,  //  0001  GETMET	R3	R2	K59
      0x5C140200,  //  0002  MOVE	R5	R1
      0x7C0C0400,  //  0003  CALL	R3	2
      0x8810013C,  //  0004  GETMBR	R4	R0	K60
      0x78120002,  //  0005  JMPF	R4	#0009
      0x8810013C,  //  0006  GETMBR	R4	R0	K60
      0x90127A03,  //  0007  SETMBR	R4	K61	R3
      0x70020004,  //  0008  JMP		#000E
      0xB8121000,  //  0009  GETNGBL	R4	K8
      0x8C10093E,  //  000A  GETMET	R4	R4	K62
      0x5C180600,  //  000B  MOVE	R6	R3
      0x7C100400,  //  000C  CALL	R4	2
      0x90027804,  //  000D  SETMBR	R0	K60	R4
      0x8810013C,  //  000E  GETMBR	R4	R0	K60
      0x8C10093F,  //  000F  GETMET	R4	R4	K63
      0x7C100200,  //  0010  CALL	R4	1
      0x60140009,  //  0011  GETGBL	R5	G9
      0x5C180800,  //  0012  MOVE	R6	R4
      0x7C140200,  //  0013  CALL	R5	1
      0x20140B1E,  //  0014  NE	R5	R5	K30
      0x7816000A,  //  0015  JMPF	R5	#0021
      0x8C140540,  //  0016  GETMET	R5	R2	K64
      0x5C1C0800,  //  0017  MOVE	R7	R4
      0x7C140400,  //  0018  CALL	R5	2
      0x60180004,  //  0019  GETGBL	R6	G4
      0x5C1C0A00,  //  001A  MOVE	R7	R5
      0x7C180200,  //  001B  CALL	R6	1
      0x1C180D41,  //  001C  EQ	R6	R6	K65
      0x781A0002,  //  001D  JMPF	R6	#0021
      0x8C180B42,  //  001E  GETMET	R6	R5	K66
      0x8820013C,  //  001F  GETMBR	R8	R0	K60
      0x7C180400,  //  0020  CALL	R6	2
      0x80000000,  //  0021  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: antiburn
********************************************************************/
be_local_closure(class_HASPmota_antiburn,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_HASPmota,     /* shared constants */
    be_str_weak(antiburn),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x8C040143,  //  0000  GETMET	R1	R0	K67
      0x7C040200,  //  0001  CALL	R1	1
      0x8C040309,  //  0002  GETMET	R1	R1	K9
      0x7C040200,  //  0003  CALL	R1	1
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _load
********************************************************************/
be_local_closure(class_HASPmota__load,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_HASPmota,     /* shared constants */
    be_str_weak(_load),
    &be_const_str_solidified,
    ( &(const binstruction[106]) {  /* code */
      0xA40A0200,  //  0000  IMPORT	R2	K1
      0xA40E8800,  //  0001  IMPORT	R3	K68
      0x60100011,  //  0002  GETGBL	R4	G17
      0x5C140200,  //  0003  MOVE	R5	R1
      0x7C100200,  //  0004  CALL	R4	1
      0x8C140945,  //  0005  GETMET	R5	R4	K69
      0x7C140200,  //  0006  CALL	R5	1
      0x8C180946,  //  0007  GETMET	R6	R4	K70
      0x7C180200,  //  0008  CALL	R6	1
      0x14140A06,  //  0009  LT	R5	R5	R6
      0x78160049,  //  000A  JMPF	R5	#0055
      0x8C140947,  //  000B  GETMET	R5	R4	K71
      0x7C140200,  //  000C  CALL	R5	1
      0x6018000C,  //  000D  GETGBL	R6	G12
      0x5C1C0A00,  //  000E  MOVE	R7	R5
      0x7C180200,  //  000F  CALL	R6	1
      0x18180D1A,  //  0010  LE	R6	R6	K26
      0x741A0002,  //  0011  JMPT	R6	#0015
      0x94180B1E,  //  0012  GETIDX	R6	R5	K30
      0x1C180D48,  //  0013  EQ	R6	R6	K72
      0x781A0000,  //  0014  JMPF	R6	#0016
      0x7001FFEE,  //  0015  JMP		#0005
      0x8C180749,  //  0016  GETMET	R6	R3	K73
      0x5C200A00,  //  0017  MOVE	R8	R5
      0x7C180400,  //  0018  CALL	R6	2
      0x601C0004,  //  0019  GETGBL	R7	G4
      0x5C200C00,  //  001A  MOVE	R8	R6
      0x7C1C0200,  //  001B  CALL	R7	1
      0x1C1C0F41,  //  001C  EQ	R7	R7	K65
      0x781E0023,  //  001D  JMPF	R7	#0042
      0xB81E3000,  //  001E  GETNGBL	R7	K24
      0x8C1C0F4A,  //  001F  GETMET	R7	R7	K74
      0x54260003,  //  0020  LDINT	R9	4
      0x7C1C0400,  //  0021  CALL	R7	2
      0x781E000F,  //  0022  JMPF	R7	#0033
      0x8C1C054B,  //  0023  GETMET	R7	R2	K75
      0x5C240A00,  //  0024  MOVE	R9	R5
      0x5828004C,  //  0025  LDCONST	R10	K76
      0x7C1C0600,  //  0026  CALL	R7	3
      0x781E0002,  //  0027  JMPF	R7	#002B
      0x541DFFFD,  //  0028  LDINT	R7	-2
      0x401E3C07,  //  0029  CONNECT	R7	K30	R7
      0x94140A07,  //  002A  GETIDX	R5	R5	R7
      0xB81E3000,  //  002B  GETNGBL	R7	K24
      0x8C1C0F26,  //  002C  GETMET	R7	R7	K38
      0x60240018,  //  002D  GETGBL	R9	G24
      0x5828004D,  //  002E  LDCONST	R10	K77
      0x5C2C0A00,  //  002F  MOVE	R11	R5
      0x7C240400,  //  0030  CALL	R9	2
      0x542A0003,  //  0031  LDINT	R10	4
      0x7C1C0600,  //  0032  CALL	R7	3
      0x8C1C014E,  //  0033  GETMET	R7	R0	K78
      0x5C240C00,  //  0034  MOVE	R9	R6
      0x7C1C0400,  //  0035  CALL	R7	2
      0x881C0124,  //  0036  GETMBR	R7	R0	K36
      0x4C200000,  //  0037  LDNIL	R8
      0x1C1C0E08,  //  0038  EQ	R7	R7	R8
      0x781E0000,  //  0039  JMPF	R7	#003B
      0xB0069F50,  //  003A  RAISE	1	K79	K80
      0x8C1C0151,  //  003B  GETMET	R7	R0	K81
      0x5C240C00,  //  003C  MOVE	R9	R6
      0x88280124,  //  003D  GETMBR	R10	R0	K36
      0x882C0152,  //  003E  GETMBR	R11	R0	K82
      0x9428140B,  //  003F  GETIDX	R10	R10	R11
      0x7C1C0600,  //  0040  CALL	R7	3
      0x70020010,  //  0041  JMP		#0053
      0x601C000C,  //  0042  GETGBL	R7	G12
      0x8C200553,  //  0043  GETMET	R8	R2	K83
      0x5C280A00,  //  0044  MOVE	R10	R5
      0x582C0054,  //  0045  LDCONST	R11	K84
      0x58300055,  //  0046  LDCONST	R12	K85
      0x7C200800,  //  0047  CALL	R8	4
      0x7C1C0200,  //  0048  CALL	R7	1
      0x241C0F1E,  //  0049  GT	R7	R7	K30
      0x781E0007,  //  004A  JMPF	R7	#0053
      0xB81E3000,  //  004B  GETNGBL	R7	K24
      0x8C1C0F26,  //  004C  GETMET	R7	R7	K38
      0x60240018,  //  004D  GETGBL	R9	G24
      0x58280056,  //  004E  LDCONST	R10	K86
      0x5C2C0A00,  //  004F  MOVE	R11	R5
      0x7C240400,  //  0050  CALL	R9	2
      0x5828001B,  //  0051  LDCONST	R10	K27
      0x7C1C0600,  //  0052  CALL	R7	3
      0x4C180000,  //  0053  LDNIL	R6
      0x7001FFAF,  //  0054  JMP		#0005
      0x8C140957,  //  0055  GETMET	R5	R4	K87
      0x7C140200,  //  0056  CALL	R5	1
      0x8C14012A,  //  0057  GETMET	R5	R0	K42
      0x4C1C0000,  //  0058  LDNIL	R7
      0x7C140400,  //  0059  CALL	R5	2
      0x6018000C,  //  005A  GETGBL	R6	G12
      0x5C1C0A00,  //  005B  MOVE	R7	R5
      0x7C180200,  //  005C  CALL	R6	1
      0x1C180D1E,  //  005D  EQ	R6	R6	K30
      0x781A0000,  //  005E  JMPF	R6	#0060
      0xB0069F58,  //  005F  RAISE	1	K79	K88
      0x94180B1E,  //  0060  GETIDX	R6	R5	K30
      0x90025606,  //  0061  SETMBR	R0	K43	R6
      0x88180124,  //  0062  GETMBR	R6	R0	K36
      0x881C012B,  //  0063  GETMBR	R7	R0	K43
      0x94180C07,  //  0064  GETIDX	R6	R6	R7
      0x8C180D33,  //  0065  GETMET	R6	R6	K51
      0x5820001E,  //  0066  LDCONST	R8	K30
      0x5824001E,  //  0067  LDCONST	R9	K30
      0x7C180600,  //  0068  CALL	R6	3
      0x80000000,  //  0069  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: fix_lv_version
********************************************************************/
be_local_closure(class_HASPmota_fix_lv_version,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    0,                          /* argc */
    12,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_HASPmota,     /* shared constants */
    be_str_weak(fix_lv_version),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x58000059,  //  0000  LDCONST	R0	K89
      0xA4067400,  //  0001  IMPORT	R1	K58
      0x8C08035A,  //  0002  GETMET	R2	R1	K90
      0xB8121000,  //  0003  GETNGBL	R4	K8
      0x5814005B,  //  0004  LDCONST	R5	K91
      0x7C080600,  //  0005  CALL	R2	3
      0x600C0004,  //  0006  GETGBL	R3	G4
      0x5C100400,  //  0007  MOVE	R4	R2
      0x7C0C0200,  //  0008  CALL	R3	1
      0x200C075C,  //  0009  NE	R3	R3	K92
      0x780E0002,  //  000A  JMPF	R3	#000E
      0xB80E1000,  //  000B  GETNGBL	R3	K8
      0x54120007,  //  000C  LDINT	R4	8
      0x900EB604,  //  000D  SETMBR	R3	K91	R4
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: sort
********************************************************************/
be_local_closure(class_HASPmota_sort,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    12,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_HASPmota,     /* shared constants */
    be_str_weak(sort),
    &be_const_str_solidified,
    ( &(const binstruction[24]) {  /* code */
      0x58040059,  //  0000  LDCONST	R1	K89
      0x5808001E,  //  0001  LDCONST	R2	K30
      0x600C000C,  //  0002  GETGBL	R3	G12
      0x5C100000,  //  0003  MOVE	R4	R0
      0x7C0C0200,  //  0004  CALL	R3	1
      0x140C0403,  //  0005  LT	R3	R2	R3
      0x780E000F,  //  0006  JMPF	R3	#0017
      0x940C0002,  //  0007  GETIDX	R3	R0	R2
      0x5C100400,  //  0008  MOVE	R4	R2
      0x2414091E,  //  0009  GT	R5	R4	K30
      0x78160008,  //  000A  JMPF	R5	#0014
      0x0414091A,  //  000B  SUB	R5	R4	K26
      0x94140005,  //  000C  GETIDX	R5	R0	R5
      0x24140A03,  //  000D  GT	R5	R5	R3
      0x78160004,  //  000E  JMPF	R5	#0014
      0x0414091A,  //  000F  SUB	R5	R4	K26
      0x94140005,  //  0010  GETIDX	R5	R0	R5
      0x98000805,  //  0011  SETIDX	R0	R4	R5
      0x0410091A,  //  0012  SUB	R4	R4	K26
      0x7001FFF4,  //  0013  JMP		#0009
      0x98000803,  //  0014  SETIDX	R0	R4	R3
      0x0008051A,  //  0015  ADD	R2	R2	K26
      0x7001FFEA,  //  0016  JMP		#0002
      0x80040000,  //  0017  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: page_dir_to
********************************************************************/
be_local_closure(class_HASPmota_page_dir_to,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_HASPmota,     /* shared constants */
    be_str_weak(page_dir_to),
    &be_const_str_solidified,
    ( &(const binstruction[32]) {  /* code */
      0x8C08012A,  //  0000  GETMET	R2	R0	K42
      0x5810001E,  //  0001  LDCONST	R4	K30
      0x7C080400,  //  0002  CALL	R2	2
      0x4C0C0000,  //  0003  LDNIL	R3
      0x1C0C0403,  //  0004  EQ	R3	R2	R3
      0x780E0000,  //  0005  JMPF	R3	#0007
      0x80063C00,  //  0006  RET	1	K30
      0x600C000C,  //  0007  GETGBL	R3	G12
      0x5C100400,  //  0008  MOVE	R4	R2
      0x7C0C0200,  //  0009  CALL	R3	1
      0x1810071A,  //  000A  LE	R4	R3	K26
      0x78120000,  //  000B  JMPF	R4	#000D
      0x80063C00,  //  000C  RET	1	K30
      0x1C10071B,  //  000D  EQ	R4	R3	K27
      0x78120000,  //  000E  JMPF	R4	#0010
      0x80063400,  //  000F  RET	1	K26
      0x8C100538,  //  0010  GETMET	R4	R2	K56
      0x5C180200,  //  0011  MOVE	R6	R1
      0x7C100400,  //  0012  CALL	R4	2
      0x4C140000,  //  0013  LDNIL	R5
      0x1C140805,  //  0014  EQ	R5	R4	R5
      0x78160000,  //  0015  JMPF	R5	#0017
      0x80063C00,  //  0016  RET	1	K30
      0x0014071A,  //  0017  ADD	R5	R3	K26
      0x0C140B1B,  //  0018  DIV	R5	R5	K27
      0x18140805,  //  0019  LE	R5	R4	R5
      0x78160001,  //  001A  JMPF	R5	#001D
      0x80063400,  //  001B  RET	1	K26
      0x70020001,  //  001C  JMP		#001F
      0x5415FFFE,  //  001D  LDINT	R5	-1
      0x80040A00,  //  001E  RET	1	R5
      0x80000000,  //  001F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_page_cur_parsing
********************************************************************/
be_local_closure(class_HASPmota_get_page_cur_parsing,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_HASPmota,     /* shared constants */
    be_str_weak(get_page_cur_parsing),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040124,  //  0000  GETMBR	R1	R0	K36
      0x88080152,  //  0001  GETMBR	R2	R0	K82
      0x94040202,  //  0002  GETIDX	R1	R1	R2
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_page
********************************************************************/
be_local_closure(class_HASPmota_parse_page,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_HASPmota,     /* shared constants */
    be_str_weak(parse_page),
    &be_const_str_solidified,
    ( &(const binstruction[111]) {  /* code */
      0x8C08035D,  //  0000  GETMET	R2	R1	K93
      0x5810005E,  //  0001  LDCONST	R4	K94
      0x7C080400,  //  0002  CALL	R2	2
      0x780A0019,  //  0003  JMPF	R2	#001E
      0x60080004,  //  0004  GETGBL	R2	G4
      0x940C035E,  //  0005  GETIDX	R3	R1	K94
      0x7C080200,  //  0006  CALL	R2	1
      0x1C08055C,  //  0007  EQ	R2	R2	K92
      0x780A0014,  //  0008  JMPF	R2	#001E
      0x60080009,  //  0009  GETGBL	R2	G9
      0x940C035E,  //  000A  GETIDX	R3	R1	K94
      0x7C080200,  //  000B  CALL	R2	1
      0x9002A402,  //  000C  SETMBR	R0	K82	R2
      0x880C012B,  //  000D  GETMBR	R3	R0	K43
      0x4C100000,  //  000E  LDNIL	R4
      0x1C0C0604,  //  000F  EQ	R3	R3	R4
      0x780E0000,  //  0010  JMPF	R3	#0012
      0x90025602,  //  0011  SETMBR	R0	K43	R2
      0x880C0124,  //  0012  GETMBR	R3	R0	K36
      0x8C0C075F,  //  0013  GETMET	R3	R3	K95
      0x5C140400,  //  0014  MOVE	R5	R2
      0x7C0C0400,  //  0015  CALL	R3	2
      0x740E0006,  //  0016  JMPT	R3	#001E
      0x880C0160,  //  0017  GETMBR	R3	R0	K96
      0x88100124,  //  0018  GETMBR	R4	R0	K36
      0x5C140600,  //  0019  MOVE	R5	R3
      0x5C180400,  //  001A  MOVE	R6	R2
      0x5C1C0000,  //  001B  MOVE	R7	R0
      0x7C140400,  //  001C  CALL	R5	2
      0x98100405,  //  001D  SETIDX	R4	R2	R5
      0x8C080338,  //  001E  GETMET	R2	R1	K56
      0x58100030,  //  001F  LDCONST	R4	K48
      0x7C080400,  //  0020  CALL	R2	2
      0x1C08051E,  //  0021  EQ	R2	R2	K30
      0x780A004A,  //  0022  JMPF	R2	#006E
      0x8C080161,  //  0023  GETMET	R2	R0	K97
      0x7C080200,  //  0024  CALL	R2	1
      0x600C0009,  //  0025  GETGBL	R3	G9
      0x8C100338,  //  0026  GETMET	R4	R1	K56
      0x5818002C,  //  0027  LDCONST	R6	K44
      0x4C1C0000,  //  0028  LDNIL	R7
      0x7C100600,  //  0029  CALL	R4	3
      0x7C0C0200,  //  002A  CALL	R3	1
      0x900A5803,  //  002B  SETMBR	R2	K44	R3
      0x600C0009,  //  002C  GETGBL	R3	G9
      0x8C100338,  //  002D  GETMET	R4	R1	K56
      0x5818002D,  //  002E  LDCONST	R6	K45
      0x4C1C0000,  //  002F  LDNIL	R7
      0x7C100600,  //  0030  CALL	R4	3
      0x7C0C0200,  //  0031  CALL	R3	1
      0x900A5A03,  //  0032  SETMBR	R2	K45	R3
      0x600C0009,  //  0033  GETGBL	R3	G9
      0x8C100338,  //  0034  GETMET	R4	R1	K56
      0x5818002E,  //  0035  LDCONST	R6	K46
      0x4C1C0000,  //  0036  LDNIL	R7
      0x7C100600,  //  0037  CALL	R4	3
      0x7C0C0200,  //  0038  CALL	R3	1
      0x900A5C03,  //  0039  SETMBR	R2	K46	R3
      0x8C0C0362,  //  003A  GETMET	R3	R1	K98
      0x5814002C,  //  003B  LDCONST	R5	K44
      0x7C0C0400,  //  003C  CALL	R3	2
      0x8C0C0362,  //  003D  GETMET	R3	R1	K98
      0x5814002D,  //  003E  LDCONST	R5	K45
      0x7C0C0400,  //  003F  CALL	R3	2
      0x8C0C0362,  //  0040  GETMET	R3	R1	K98
      0x5814002E,  //  0041  LDCONST	R5	K46
      0x7C0C0400,  //  0042  CALL	R3	2
      0x880C0563,  //  0043  GETMBR	R3	R2	K99
      0x1C0C071E,  //  0044  EQ	R3	R3	K30
      0x780E0027,  //  0045  JMPF	R3	#006E
      0x600C0009,  //  0046  GETGBL	R3	G9
      0x8C100338,  //  0047  GETMET	R4	R1	K56
      0x58180064,  //  0048  LDCONST	R6	K100
      0x881C0164,  //  0049  GETMBR	R7	R0	K100
      0x7C100600,  //  004A  CALL	R4	3
      0x7C0C0200,  //  004B  CALL	R3	1
      0x9002C803,  //  004C  SETMBR	R0	K100	R3
      0x600C0009,  //  004D  GETGBL	R3	G9
      0x8C100338,  //  004E  GETMET	R4	R1	K56
      0x58180065,  //  004F  LDCONST	R6	K101
      0x881C0165,  //  0050  GETMBR	R7	R0	K101
      0x7C100600,  //  0051  CALL	R4	3
      0x7C0C0200,  //  0052  CALL	R3	1
      0x9002CA03,  //  0053  SETMBR	R0	K101	R3
      0x600C0009,  //  0054  GETGBL	R3	G9
      0x8C100338,  //  0055  GETMET	R4	R1	K56
      0x58180066,  //  0056  LDCONST	R6	K102
      0x881C0166,  //  0057  GETMBR	R7	R0	K102
      0x7C100600,  //  0058  CALL	R4	3
      0x7C0C0200,  //  0059  CALL	R3	1
      0x9002CC03,  //  005A  SETMBR	R0	K102	R3
      0x600C0009,  //  005B  GETGBL	R3	G9
      0x8C100338,  //  005C  GETMET	R4	R1	K56
      0x58180068,  //  005D  LDCONST	R6	K104
      0x881C0164,  //  005E  GETMBR	R7	R0	K100
      0x7C100600,  //  005F  CALL	R4	3
      0x7C0C0200,  //  0060  CALL	R3	1
      0x9002CE03,  //  0061  SETMBR	R0	K103	R3
      0x8C0C0362,  //  0062  GETMET	R3	R1	K98
      0x58140064,  //  0063  LDCONST	R5	K100
      0x7C0C0400,  //  0064  CALL	R3	2
      0x8C0C0362,  //  0065  GETMET	R3	R1	K98
      0x58140065,  //  0066  LDCONST	R5	K101
      0x7C0C0400,  //  0067  CALL	R3	2
      0x8C0C0362,  //  0068  GETMET	R3	R1	K98
      0x58140066,  //  0069  LDCONST	R5	K102
      0x7C0C0400,  //  006A  CALL	R3	2
      0x8C0C0362,  //  006B  GETMET	R3	R1	K98
      0x58140068,  //  006C  LDCONST	R5	K104
      0x7C0C0400,  //  006D  CALL	R3	2
      0x80000000,  //  006E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse
********************************************************************/
be_local_closure(class_HASPmota_parse,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_HASPmota,     /* shared constants */
    be_str_weak(parse),
    &be_const_str_solidified,
    ( &(const binstruction[23]) {  /* code */
      0xA40A8800,  //  0000  IMPORT	R2	K68
      0x8C0C0549,  //  0001  GETMET	R3	R2	K73
      0x5C140200,  //  0002  MOVE	R5	R1
      0x7C0C0400,  //  0003  CALL	R3	2
      0x60100004,  //  0004  GETGBL	R4	G4
      0x5C140600,  //  0005  MOVE	R5	R3
      0x7C100200,  //  0006  CALL	R4	1
      0x1C100941,  //  0007  EQ	R4	R4	K65
      0x7812000B,  //  0008  JMPF	R4	#0015
      0x8810012B,  //  0009  GETMBR	R4	R0	K43
      0x9002A404,  //  000A  SETMBR	R0	K82	R4
      0x8C10014E,  //  000B  GETMET	R4	R0	K78
      0x5C180600,  //  000C  MOVE	R6	R3
      0x7C100400,  //  000D  CALL	R4	2
      0x8C100151,  //  000E  GETMET	R4	R0	K81
      0x5C180600,  //  000F  MOVE	R6	R3
      0x881C0124,  //  0010  GETMBR	R7	R0	K36
      0x88200152,  //  0011  GETMBR	R8	R0	K82
      0x941C0E08,  //  0012  GETIDX	R7	R7	R8
      0x7C100600,  //  0013  CALL	R4	3
      0x70020000,  //  0014  JMP		#0016
      0xB0069F69,  //  0015  RAISE	1	K79	K105
      0x80000000,  //  0016  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_obj
********************************************************************/
be_local_closure(class_HASPmota_parse_obj,   /* name */
  be_nested_proto(
    20,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_HASPmota,     /* shared constants */
    be_str_weak(parse_obj),
    &be_const_str_solidified,
    ( &(const binstruction[239]) {  /* code */
      0xA40ED400,  //  0000  IMPORT	R3	K106
      0xA4127400,  //  0001  IMPORT	R4	K58
      0x60140009,  //  0002  GETGBL	R5	G9
      0x8C180338,  //  0003  GETMET	R6	R1	K56
      0x58200030,  //  0004  LDCONST	R8	K48
      0x7C180400,  //  0005  CALL	R6	2
      0x7C140200,  //  0006  CALL	R5	1
      0x8C180338,  //  0007  GETMET	R6	R1	K56
      0x5820006B,  //  0008  LDCONST	R8	K107
      0x7C180400,  //  0009  CALL	R6	2
      0x4C1C0000,  //  000A  LDNIL	R7
      0x201C0C07,  //  000B  NE	R7	R6	R7
      0x781E0003,  //  000C  JMPF	R7	#0011
      0x601C0008,  //  000D  GETGBL	R7	G8
      0x5C200C00,  //  000E  MOVE	R8	R6
      0x7C1C0200,  //  000F  CALL	R7	1
      0x70020000,  //  0010  JMP		#0012
      0x4C1C0000,  //  0011  LDNIL	R7
      0x5C180E00,  //  0012  MOVE	R6	R7
      0x8C1C0161,  //  0013  GETMET	R7	R0	K97
      0x7C1C0200,  //  0014  CALL	R7	1
      0x4C200000,  //  0015  LDNIL	R8
      0x20200A08,  //  0016  NE	R8	R5	R8
      0x78220011,  //  0017  JMPF	R8	#002A
      0x14200B1E,  //  0018  LT	R8	R5	K30
      0x74220002,  //  0019  JMPT	R8	#001D
      0x542200FD,  //  001A  LDINT	R8	254
      0x24200A08,  //  001B  GT	R8	R5	R8
      0x7822000C,  //  001C  JMPF	R8	#002A
      0x20200B1E,  //  001D  NE	R8	R5	K30
      0x74220002,  //  001E  JMPT	R8	#0022
      0x4C200000,  //  001F  LDNIL	R8
      0x1C200C08,  //  0020  EQ	R8	R6	R8
      0x78220007,  //  0021  JMPF	R8	#002A
      0x60200001,  //  0022  GETGBL	R8	G1
      0x60240018,  //  0023  GETGBL	R9	G24
      0x5828006C,  //  0024  LDCONST	R10	K108
      0x5C2C0A00,  //  0025  MOVE	R11	R5
      0x5C300C00,  //  0026  MOVE	R12	R6
      0x7C240600,  //  0027  CALL	R9	3
      0x7C200200,  //  0028  CALL	R8	1
      0x80001000,  //  0029  RET	0
      0x8C200F6D,  //  002A  GETMET	R8	R7	K109
      0x5C280A00,  //  002B  MOVE	R10	R5
      0x7C200400,  //  002C  CALL	R8	2
      0x4C240000,  //  002D  LDNIL	R9
      0x20240C09,  //  002E  NE	R9	R6	R9
      0x7826005F,  //  002F  JMPF	R9	#0090
      0x4C240000,  //  0030  LDNIL	R9
      0x20240A09,  //  0031  NE	R9	R5	R9
      0x7826005C,  //  0032  JMPF	R9	#0090
      0x4C240000,  //  0033  LDNIL	R9
      0x1C241009,  //  0034  EQ	R9	R8	R9
      0x78260059,  //  0035  JMPF	R9	#0090
      0x60240009,  //  0036  GETGBL	R9	G9
      0x8C280338,  //  0037  GETMET	R10	R1	K56
      0x5830006E,  //  0038  LDCONST	R12	K110
      0x7C280400,  //  0039  CALL	R10	2
      0x7C240200,  //  003A  CALL	R9	1
      0x4C280000,  //  003B  LDNIL	R10
      0x4C2C0000,  //  003C  LDNIL	R11
      0x4C300000,  //  003D  LDNIL	R12
      0x2030120C,  //  003E  NE	R12	R9	R12
      0x78320007,  //  003F  JMPF	R12	#0048
      0x8C300F6D,  //  0040  GETMET	R12	R7	K109
      0x5C381200,  //  0041  MOVE	R14	R9
      0x7C300400,  //  0042  CALL	R12	2
      0x5C281800,  //  0043  MOVE	R10	R12
      0x4C300000,  //  0044  LDNIL	R12
      0x2030140C,  //  0045  NE	R12	R10	R12
      0x78320000,  //  0046  JMPF	R12	#0048
      0x882C156F,  //  0047  GETMBR	R11	R10	K111
      0x4C300000,  //  0048  LDNIL	R12
      0x1C30160C,  //  0049  EQ	R12	R11	R12
      0x78320002,  //  004A  JMPF	R12	#004E
      0x8C300F70,  //  004B  GETMET	R12	R7	K112
      0x7C300200,  //  004C  CALL	R12	1
      0x5C2C1800,  //  004D  MOVE	R11	R12
      0x8C30095A,  //  004E  GETMET	R12	R4	K90
      0x5C380000,  //  004F  MOVE	R14	R0
      0x003EE206,  //  0050  ADD	R15	K113	R6
      0x7C300600,  //  0051  CALL	R12	3
      0x4C340000,  //  0052  LDNIL	R13
      0x4C380000,  //  0053  LDNIL	R14
      0x1C38180E,  //  0054  EQ	R14	R12	R14
      0x783A0010,  //  0055  JMPF	R14	#0067
      0x8C38095A,  //  0056  GETMET	R14	R4	K90
      0x5C400600,  //  0057  MOVE	R16	R3
      0x5C440C00,  //  0058  MOVE	R17	R6
      0x7C380600,  //  0059  CALL	R14	3
      0x4C3C0000,  //  005A  LDNIL	R15
      0x203C1C0F,  //  005B  NE	R15	R14	R15
      0x783E0009,  //  005C  JMPF	R15	#0067
      0x603C0004,  //  005D  GETGBL	R15	G4
      0x5C401C00,  //  005E  MOVE	R16	R14
      0x7C3C0200,  //  005F  CALL	R15	1
      0x1C3C1F72,  //  0060  EQ	R15	R15	K114
      0x783E0004,  //  0061  JMPF	R15	#0067
      0x5C3C1C00,  //  0062  MOVE	R15	R14
      0x5C401600,  //  0063  MOVE	R16	R11
      0x7C3C0200,  //  0064  CALL	R15	1
      0x5C341E00,  //  0065  MOVE	R13	R15
      0x88300173,  //  0066  GETMBR	R12	R0	K115
      0x4C380000,  //  0067  LDNIL	R14
      0x1C38180E,  //  0068  EQ	R14	R12	R14
      0x783A000F,  //  0069  JMPF	R14	#007A
      0x8C380974,  //  006A  GETMET	R14	R4	K116
      0x5C400C00,  //  006B  MOVE	R16	R6
      0x7C380400,  //  006C  CALL	R14	2
      0x4C3C0000,  //  006D  LDNIL	R15
      0x203C1C0F,  //  006E  NE	R15	R14	R15
      0x783E0009,  //  006F  JMPF	R15	#007A
      0x603C0004,  //  0070  GETGBL	R15	G4
      0x5C401C00,  //  0071  MOVE	R16	R14
      0x7C3C0200,  //  0072  CALL	R15	1
      0x1C3C1F72,  //  0073  EQ	R15	R15	K114
      0x783E0004,  //  0074  JMPF	R15	#007A
      0x5C3C1C00,  //  0075  MOVE	R15	R14
      0x5C401600,  //  0076  MOVE	R16	R11
      0x7C3C0200,  //  0077  CALL	R15	1
      0x5C341E00,  //  0078  MOVE	R13	R15
      0x88300173,  //  0079  GETMBR	R12	R0	K115
      0x4C380000,  //  007A  LDNIL	R14
      0x1C38180E,  //  007B  EQ	R14	R12	R14
      0x783A0006,  //  007C  JMPF	R14	#0084
      0x60380001,  //  007D  GETGBL	R14	G1
      0x603C0018,  //  007E  GETGBL	R15	G24
      0x58400075,  //  007F  LDCONST	R16	K117
      0x5C440C00,  //  0080  MOVE	R17	R6
      0x7C3C0400,  //  0081  CALL	R15	2
      0x7C380200,  //  0082  CALL	R14	1
      0x80001C00,  //  0083  RET	0
      0x5C381800,  //  0084  MOVE	R14	R12
      0x5C3C1600,  //  0085  MOVE	R15	R11
      0x5C400400,  //  0086  MOVE	R16	R2
      0x5C440200,  //  0087  MOVE	R17	R1
      0x5C481A00,  //  0088  MOVE	R18	R13
      0x5C4C1400,  //  0089  MOVE	R19	R10
      0x7C380A00,  //  008A  CALL	R14	5
      0x5C201C00,  //  008B  MOVE	R8	R14
      0x8C380F76,  //  008C  GETMET	R14	R7	K118
      0x5C400A00,  //  008D  MOVE	R16	R5
      0x5C441000,  //  008E  MOVE	R17	R8
      0x7C380600,  //  008F  CALL	R14	3
      0x1C240B1E,  //  0090  EQ	R9	R5	K30
      0x7826000F,  //  0091  JMPF	R9	#00A2
      0x4C240000,  //  0092  LDNIL	R9
      0x20240C09,  //  0093  NE	R9	R6	R9
      0x78260006,  //  0094  JMPF	R9	#009C
      0x60240001,  //  0095  GETGBL	R9	G1
      0x60280018,  //  0096  GETGBL	R10	G24
      0x582C0077,  //  0097  LDCONST	R11	K119
      0x5C300C00,  //  0098  MOVE	R12	R6
      0x7C280400,  //  0099  CALL	R10	2
      0x7C240200,  //  009A  CALL	R9	1
      0x80001200,  //  009B  RET	0
      0x8C240161,  //  009C  GETMET	R9	R0	K97
      0x7C240200,  //  009D  CALL	R9	1
      0x8C24136D,  //  009E  GETMET	R9	R9	K109
      0x582C001E,  //  009F  LDCONST	R11	K30
      0x7C240400,  //  00A0  CALL	R9	2
      0x5C201200,  //  00A1  MOVE	R8	R9
      0x4C240000,  //  00A2  LDNIL	R9
      0x20241009,  //  00A3  NE	R9	R8	R9
      0x7826000C,  //  00A4  JMPF	R9	#00B2
      0x60240010,  //  00A5  GETGBL	R9	G16
      0x8C280334,  //  00A6  GETMET	R10	R1	K52
      0x7C280200,  //  00A7  CALL	R10	1
      0x7C240200,  //  00A8  CALL	R9	1
      0xA8020004,  //  00A9  EXBLK	0	#00AF
      0x5C281200,  //  00AA  MOVE	R10	R9
      0x7C280000,  //  00AB  CALL	R10	0
      0x942C020A,  //  00AC  GETIDX	R11	R1	R10
      0x9020140B,  //  00AD  SETMBR	R8	R10	R11
      0x7001FFFA,  //  00AE  JMP		#00AA
      0x58240036,  //  00AF  LDCONST	R9	K54
      0xAC240200,  //  00B0  CATCH	R9	1	0
      0xB0080000,  //  00B1  RAISE	2	R0	R0
      0x4C240000,  //  00B2  LDNIL	R9
      0x20241009,  //  00B3  NE	R9	R8	R9
      0x78260001,  //  00B4  JMPF	R9	#00B7
      0x8C241178,  //  00B5  GETMET	R9	R8	K120
      0x7C240200,  //  00B6  CALL	R9	1
      0x4C240000,  //  00B7  LDNIL	R9
      0x60280008,  //  00B8  GETGBL	R10	G8
      0x8C2C0338,  //  00B9  GETMET	R11	R1	K56
      0x58340079,  //  00BA  LDCONST	R13	K121
      0x7C2C0400,  //  00BB  CALL	R11	2
      0x7C280200,  //  00BC  CALL	R10	1
      0x202C157A,  //  00BD  NE	R11	R10	K122
      0x782E0012,  //  00BE  JMPF	R11	#00D2
      0xA8020005,  //  00BF  EXBLK	0	#00C6
      0x602C000D,  //  00C0  GETGBL	R11	G13
      0x5C301400,  //  00C1  MOVE	R12	R10
      0x7C2C0200,  //  00C2  CALL	R11	1
      0x5C241600,  //  00C3  MOVE	R9	R11
      0xA8040001,  //  00C4  EXBLK	1	1
      0x7002000B,  //  00C5  JMP		#00D2
      0xAC2C0002,  //  00C6  CATCH	R11	0	2
      0x70020008,  //  00C7  JMP		#00D1
      0x60340001,  //  00C8  GETGBL	R13	G1
      0x60380018,  //  00C9  GETGBL	R14	G24
      0x583C007B,  //  00CA  LDCONST	R15	K123
      0x5C401400,  //  00CB  MOVE	R16	R10
      0x5C441600,  //  00CC  MOVE	R17	R11
      0x5C481800,  //  00CD  MOVE	R18	R12
      0x7C380800,  //  00CE  CALL	R14	4
      0x7C340200,  //  00CF  CALL	R13	1
      0x70020000,  //  00D0  JMP		#00D2
      0xB0080000,  //  00D1  RAISE	2	R0	R0
      0x4C2C0000,  //  00D2  LDNIL	R11
      0x202C120B,  //  00D3  NE	R11	R9	R11
      0x782E0018,  //  00D4  JMPF	R11	#00EE
      0xA802000B,  //  00D5  EXBLK	0	#00E2
      0x5C2C1200,  //  00D6  MOVE	R11	R9
      0x7C2C0000,  //  00D7  CALL	R11	0
      0x60300004,  //  00D8  GETGBL	R12	G4
      0x5C341600,  //  00D9  MOVE	R13	R11
      0x7C300200,  //  00DA  CALL	R12	1
      0x1C30197C,  //  00DB  EQ	R12	R12	K124
      0x78320002,  //  00DC  JMPF	R12	#00E0
      0x5C301600,  //  00DD  MOVE	R12	R11
      0x5C341000,  //  00DE  MOVE	R13	R8
      0x7C300200,  //  00DF  CALL	R12	1
      0xA8040001,  //  00E0  EXBLK	1	1
      0x7002000B,  //  00E1  JMP		#00EE
      0xAC2C0002,  //  00E2  CATCH	R11	0	2
      0x70020008,  //  00E3  JMP		#00ED
      0x60340001,  //  00E4  GETGBL	R13	G1
      0x60380018,  //  00E5  GETGBL	R14	G24
      0x583C007D,  //  00E6  LDCONST	R15	K125
      0x5C401400,  //  00E7  MOVE	R16	R10
      0x5C441600,  //  00E8  MOVE	R17	R11
      0x5C481800,  //  00E9  MOVE	R18	R12
      0x7C380800,  //  00EA  CALL	R14	4
      0x7C340200,  //  00EB  CALL	R13	1
      0x70020000,  //  00EC  JMP		#00EE
      0xB0080000,  //  00ED  RAISE	2	R0	R0
      0x80000000,  //  00EE  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_HASPmota_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_HASPmota,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[19]) {  /* code */
      0x8C04017E,  //  0000  GETMET	R1	R0	K126
      0x7C040200,  //  0001  CALL	R1	1
      0xA4065000,  //  0002  IMPORT	R1	K40
      0x8C08037F,  //  0003  GETMET	R2	R1	K127
      0x58100080,  //  0004  LDCONST	R4	K128
      0x7C080400,  //  0005  CALL	R2	2
      0x90026402,  //  0006  SETMBR	R0	K50	R2
      0x9002CF1E,  //  0007  SETMBR	R0	K103	K30
      0x9002C91E,  //  0008  SETMBR	R0	K100	K30
      0x9002CB1A,  //  0009  SETMBR	R0	K101	K26
      0x88080181,  //  000A  GETMBR	R2	R0	K129
      0x9002CC02,  //  000B  SETMBR	R0	K102	R2
      0x540A0063,  //  000C  LDINT	R2	100
      0x90030402,  //  000D  SETMBR	R0	K130	R2
      0xB80A3000,  //  000E  GETNGBL	R2	K24
      0x8C080583,  //  000F  GETMET	R2	R2	K131
      0x5C100000,  //  0010  MOVE	R4	R0
      0x7C080400,  //  0011  CALL	R2	2
      0x80000000,  //  0012  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: auto_dimming
********************************************************************/
be_local_closure(class_HASPmota_auto_dimming,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 1]) {
      be_nested_proto(
        5,                          /* nstack */
        0,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 5),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 4]) {     /* constants */
        /* K0   */  be_nested_str_weak(tasmota),
        /* K1   */  be_nested_str_weak(publish_rule),
        /* K2   */  be_nested_str_weak(log),
        /* K3   */  be_nested_str_weak(HSP_X3A_X20publish_X20_X25s),
        }),
        be_str_weak(_anonymous_),
        &be_const_str_solidified,
        ( &(const binstruction[13]) {  /* code */
          0xB8020000,  //  0000  GETNGBL	R0	K0
          0x8C000101,  //  0001  GETMET	R0	R0	K1
          0x68080000,  //  0002  GETUPV	R2	U0
          0x7C000400,  //  0003  CALL	R0	2
          0xB8020000,  //  0004  GETNGBL	R0	K0
          0x8C000102,  //  0005  GETMET	R0	R0	K2
          0x60080018,  //  0006  GETGBL	R2	G24
          0x580C0003,  //  0007  LDCONST	R3	K3
          0x68100000,  //  0008  GETUPV	R4	U0
          0x7C080400,  //  0009  CALL	R2	2
          0x540E0003,  //  000A  LDINT	R3	4
          0x7C000600,  //  000B  CALL	R0	3
          0x80000000,  //  000C  RET	0
        })
      ),
    }),
    1,                          /* has constants */
    &be_ktab_class_HASPmota,     /* shared constants */
    be_str_weak(auto_dimming),
    &be_const_str_solidified,
    ( &(const binstruction[104]) {  /* code */
      0x88040164,  //  0000  GETMBR	R1	R0	K100
      0x1804031E,  //  0001  LE	R1	R1	K30
      0x78060000,  //  0002  JMPF	R1	#0004
      0x80000200,  //  0003  RET	0
      0x4C040000,  //  0004  LDNIL	R1
      0xA40B0800,  //  0005  IMPORT	R2	K132
      0x880C0164,  //  0006  GETMBR	R3	R0	K100
      0x5412EA5F,  //  0007  LDINT	R4	60000
      0x080C0604,  //  0008  MUL	R3	R3	R4
      0xB8121000,  //  0009  GETNGBL	R4	K8
      0x8C100985,  //  000A  GETMET	R4	R4	K133
      0x7C100200,  //  000B  CALL	R4	1
      0x8C100986,  //  000C  GETMET	R4	R4	K134
      0x7C100200,  //  000D  CALL	R4	1
      0x14140803,  //  000E  LT	R5	R4	R3
      0x78160014,  //  000F  JMPF	R5	#0025
      0x88140187,  //  0010  GETMBR	R5	R0	K135
      0x4C180000,  //  0011  LDNIL	R6
      0x20140A06,  //  0012  NE	R5	R5	R6
      0x78160004,  //  0013  JMPF	R5	#0019
      0x88140187,  //  0014  GETMBR	R5	R0	K135
      0x8C140B88,  //  0015  GETMET	R5	R5	K136
      0x7C140200,  //  0016  CALL	R5	1
      0x4C140000,  //  0017  LDNIL	R5
      0x90030E05,  //  0018  SETMBR	R0	K135	R5
      0x88140182,  //  0019  GETMBR	R5	R0	K130
      0x541A0063,  //  001A  LDINT	R6	100
      0x14140A06,  //  001B  LT	R5	R5	R6
      0x78160006,  //  001C  JMPF	R5	#0024
      0x54160063,  //  001D  LDINT	R5	100
      0x90030405,  //  001E  SETMBR	R0	K130	R5
      0x8C140589,  //  001F  GETMET	R5	R2	K137
      0x8C1C0589,  //  0020  GETMET	R7	R2	K137
      0x7C1C0200,  //  0021  CALL	R7	1
      0x7C140400,  //  0022  CALL	R5	2
      0x5804008A,  //  0023  LDCONST	R1	K138
      0x70020035,  //  0024  JMP		#005B
      0x88140166,  //  0025  GETMBR	R5	R0	K102
      0x541A03E7,  //  0026  LDINT	R6	1000
      0x08140A06,  //  0027  MUL	R5	R5	R6
      0x00140605,  //  0028  ADD	R5	R3	R5
      0x14140805,  //  0029  LT	R5	R4	R5
      0x7816001F,  //  002A  JMPF	R5	#004B
      0x88140187,  //  002B  GETMBR	R5	R0	K135
      0x4C180000,  //  002C  LDNIL	R6
      0x1C140A06,  //  002D  EQ	R5	R5	R6
      0x78160002,  //  002E  JMPF	R5	#0032
      0x8C14018B,  //  002F  GETMET	R5	R0	K139
      0x7C140200,  //  0030  CALL	R5	1
      0x90030E05,  //  0031  SETMBR	R0	K135	R5
      0x88140182,  //  0032  GETMBR	R5	R0	K130
      0x541A0018,  //  0033  LDINT	R6	25
      0x24140A06,  //  0034  GT	R5	R5	R6
      0x78160013,  //  0035  JMPF	R5	#004A
      0x54160018,  //  0036  LDINT	R5	25
      0x90030405,  //  0037  SETMBR	R0	K130	R5
      0x8C140589,  //  0038  GETMET	R5	R2	K137
      0x7C140200,  //  0039  CALL	R5	1
      0x88180165,  //  003A  GETMBR	R6	R0	K101
      0x24180A06,  //  003B  GT	R6	R5	R6
      0x781A0002,  //  003C  JMPF	R6	#0040
      0x88180165,  //  003D  GETMBR	R6	R0	K101
      0x04180A06,  //  003E  SUB	R6	R5	R6
      0x70020000,  //  003F  JMP		#0041
      0x88180165,  //  0040  GETMBR	R6	R0	K101
      0x541E0003,  //  0041  LDINT	R7	4
      0x0C1C0C07,  //  0042  DIV	R7	R6	R7
      0x88200165,  //  0043  GETMBR	R8	R0	K101
      0x001C0E08,  //  0044  ADD	R7	R7	R8
      0x8C200589,  //  0045  GETMET	R8	R2	K137
      0x5C280E00,  //  0046  MOVE	R10	R7
      0x502C0200,  //  0047  LDBOOL	R11	1	0
      0x7C200600,  //  0048  CALL	R8	3
      0x5804008C,  //  0049  LDCONST	R1	K140
      0x7002000F,  //  004A  JMP		#005B
      0x88140187,  //  004B  GETMBR	R5	R0	K135
      0x4C180000,  //  004C  LDNIL	R6
      0x1C140A06,  //  004D  EQ	R5	R5	R6
      0x78160002,  //  004E  JMPF	R5	#0052
      0x8C14018B,  //  004F  GETMET	R5	R0	K139
      0x7C140200,  //  0050  CALL	R5	1
      0x90030E05,  //  0051  SETMBR	R0	K135	R5
      0x88140182,  //  0052  GETMBR	R5	R0	K130
      0x24140B1E,  //  0053  GT	R5	R5	K30
      0x78160005,  //  0054  JMPF	R5	#005B
      0x9003051E,  //  0055  SETMBR	R0	K130	K30
      0x8C140589,  //  0056  GETMET	R5	R2	K137
      0x581C001E,  //  0057  LDCONST	R7	K30
      0x50200200,  //  0058  LDBOOL	R8	1	0
      0x7C140600,  //  0059  CALL	R5	3
      0x5804008D,  //  005A  LDCONST	R1	K141
      0x4C140000,  //  005B  LDNIL	R5
      0x20140205,  //  005C  NE	R5	R1	R5
      0x78160008,  //  005D  JMPF	R5	#0067
      0x60140018,  //  005E  GETGBL	R5	G24
      0x5818008E,  //  005F  LDCONST	R6	K142
      0x5C1C0200,  //  0060  MOVE	R7	R1
      0x7C140400,  //  0061  CALL	R5	2
      0xB81A3000,  //  0062  GETNGBL	R6	K24
      0x8C180D8F,  //  0063  GETMET	R6	R6	K143
      0x84200000,  //  0064  CLOSURE	R8	P0
      0x7C180400,  //  0065  CALL	R6	2
      0xA0140000,  //  0066  CLOSE	R5
      0x80000000,  //  0067  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: do_action
********************************************************************/
be_local_closure(class_HASPmota_do_action,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_HASPmota,     /* shared constants */
    be_str_weak(do_action),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0xB80E1000,  //  0000  GETNGBL	R3	K8
      0x880C0790,  //  0001  GETMBR	R3	R3	K144
      0x200C0403,  //  0002  NE	R3	R2	R3
      0x780E0000,  //  0003  JMPF	R3	#0005
      0x80000600,  //  0004  RET	0
      0x8C0C0191,  //  0005  GETMET	R3	R0	K145
      0x88140392,  //  0006  GETMBR	R5	R1	K146
      0x7C0C0400,  //  0007  CALL	R3	2
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_page_cur
********************************************************************/
be_local_closure(class_HASPmota_get_page_cur,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_HASPmota,     /* shared constants */
    be_str_weak(get_page_cur),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040124,  //  0000  GETMBR	R1	R0	K36
      0x8808012B,  //  0001  GETMBR	R2	R0	K43
      0x94040202,  //  0002  GETIDX	R1	R1	R2
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _remove_page
********************************************************************/
be_local_closure(class_HASPmota__remove_page,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_HASPmota,     /* shared constants */
    be_str_weak(_remove_page),
    &be_const_str_solidified,
    ( &(const binstruction[37]) {  /* code */
      0x8C080129,  //  0000  GETMET	R2	R0	K41
      0x7C080200,  //  0001  CALL	R2	1
      0x8C080530,  //  0002  GETMET	R2	R2	K48
      0x7C080200,  //  0003  CALL	R2	1
      0x1C0C0202,  //  0004  EQ	R3	R1	R2
      0x780E0008,  //  0005  JMPF	R3	#000F
      0x8C0C0191,  //  0006  GETMET	R3	R0	K145
      0x5814002F,  //  0007  LDCONST	R5	K47
      0x5818001E,  //  0008  LDCONST	R6	K30
      0x581C001E,  //  0009  LDCONST	R7	K30
      0x7C0C0800,  //  000A  CALL	R3	4
      0x4C100000,  //  000B  LDNIL	R4
      0x1C100604,  //  000C  EQ	R4	R3	R4
      0x78120000,  //  000D  JMPF	R4	#000F
      0x80000800,  //  000E  RET	0
      0x880C0152,  //  000F  GETMBR	R3	R0	K82
      0x1C0C0601,  //  0010  EQ	R3	R3	R1
      0x780E0001,  //  0011  JMPF	R3	#0014
      0x880C012B,  //  0012  GETMBR	R3	R0	K43
      0x9002A403,  //  0013  SETMBR	R0	K82	R3
      0x880C0124,  //  0014  GETMBR	R3	R0	K36
      0x8C0C075F,  //  0015  GETMET	R3	R3	K95
      0x5C140200,  //  0016  MOVE	R5	R1
      0x7C0C0400,  //  0017  CALL	R3	2
      0x780E0003,  //  0018  JMPF	R3	#001D
      0x880C0124,  //  0019  GETMBR	R3	R0	K36
      0x8C0C0762,  //  001A  GETMET	R3	R3	K98
      0x5C140200,  //  001B  MOVE	R5	R1
      0x7C0C0400,  //  001C  CALL	R3	2
      0x600C0018,  //  001D  GETGBL	R3	G24
      0x58100093,  //  001E  LDCONST	R4	K147
      0x5C140200,  //  001F  MOVE	R5	R1
      0x7C0C0400,  //  0020  CALL	R3	2
      0xB812D400,  //  0021  GETNGBL	R4	K106
      0x4C140000,  //  0022  LDNIL	R5
      0x90100605,  //  0023  SETMBR	R4	R3	R5
      0x80000000,  //  0024  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_pages
********************************************************************/
be_local_closure(class_HASPmota_get_pages,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_HASPmota,     /* shared constants */
    be_str_weak(get_pages),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x8C04012A,  //  0000  GETMET	R1	R0	K42
      0x4C0C0000,  //  0001  LDNIL	R3
      0x7C040400,  //  0002  CALL	R1	2
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: register_event
********************************************************************/
be_local_closure(class_HASPmota_register_event,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
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
        /* K0   */  be_nested_str_weak(event_dispatch),
        }),
        be_str_weak(_X3Clambda_X3E),
        &be_const_str_solidified,
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
    &be_ktab_class_HASPmota,     /* shared constants */
    be_str_weak(register_event),
    &be_const_str_solidified,
    ( &(const binstruction[20]) {  /* code */
      0xA40F2800,  //  0000  IMPORT	R3	K148
      0xA4127400,  //  0001  IMPORT	R4	K58
      0x88140142,  //  0002  GETMBR	R5	R0	K66
      0x4C180000,  //  0003  LDNIL	R6
      0x1C140A06,  //  0004  EQ	R5	R5	R6
      0x78160003,  //  0005  JMPF	R5	#000A
      0x8C140795,  //  0006  GETMET	R5	R3	K149
      0x841C0000,  //  0007  CLOSURE	R7	P0
      0x7C140400,  //  0008  CALL	R5	2
      0x90028405,  //  0009  SETMBR	R0	K66	R5
      0x8814036F,  //  000A  GETMBR	R5	R1	K111
      0x8C180B96,  //  000B  GETMET	R6	R5	K150
      0x88200142,  //  000C  GETMBR	R8	R0	K66
      0x5C240400,  //  000D  MOVE	R9	R2
      0x8C28093B,  //  000E  GETMET	R10	R4	K59
      0x5C300200,  //  000F  MOVE	R12	R1
      0x7C280400,  //  0010  CALL	R10	2
      0x7C180800,  //  0011  CALL	R6	4
      0xA0000000,  //  0012  CLOSE	R0
      0x80000000,  //  0013  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: HASPmota
********************************************************************/
be_local_class(HASPmota,
    19,
    NULL,
    be_nested_map(76,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(lvh_img, 55), be_const_class(be_class_lvh_img) },
        { be_const_key_weak(lvh_arc, -1), be_const_class(be_class_lvh_arc) },
        { be_const_key_weak(page_show, 57), be_const_closure(class_HASPmota_page_show_closure) },
        { be_const_key_weak(start, 75), be_const_closure(class_HASPmota_start_closure) },
        { be_const_key_weak(pages_list_sorted, 73), be_const_closure(class_HASPmota_pages_list_sorted_closure) },
        { be_const_key_weak(event_cb, -1), be_const_var(12) },
        { be_const_key_weak(lvh_scr, -1), be_const_class(be_class_lvh_scr) },
        { be_const_key_weak(r12, -1), be_const_var(4) },
        { be_const_key_weak(every_100ms, -1), be_const_closure(class_HASPmota_every_100ms_closure) },
        { be_const_key_weak(register_event, -1), be_const_closure(class_HASPmota_register_event_closure) },
        { be_const_key_weak(lvh_pages, -1), be_const_var(7) },
        { be_const_key_weak(scr, -1), be_const_var(3) },
        { be_const_key_weak(lvh_label, -1), be_const_class(be_class_lvh_label) },
        { be_const_key_weak(DIMMING_DURATION, 5), be_const_int(30) },
        { be_const_key_weak(event_dispatch, -1), be_const_closure(class_HASPmota_event_dispatch_closure) },
        { be_const_key_weak(hres, -1), be_const_var(1) },
        { be_const_key_weak(lvh_scale, 70), be_const_class(be_class_lvh_scale) },
        { be_const_key_weak(lvh_btn, 43), be_const_class(be_class_lvh_btn) },
        { be_const_key_weak(lvh_switch, -1), be_const_class(be_class_lvh_switch) },
        { be_const_key_weak(lvh_scale_line, 72), be_const_class(be_class_lvh_scale_line) },
        { be_const_key_weak(antiburn, 44), be_const_closure(class_HASPmota_antiburn_closure) },
        { be_const_key_weak(dimming_min, -1), be_const_var(15) },
        { be_const_key_weak(lvh_obj, -1), be_const_class(be_class_lvh_obj) },
        { be_const_key_weak(lvh_page_cur_idx_parsing, -1), be_const_var(9) },
        { be_const_key_weak(lvh_page_cur_idx, 12), be_const_var(8) },
        { be_const_key_weak(lvh_tab, -1), be_const_class(be_class_lvh_tab) },
        { be_const_key_weak(lvh_chart, 52), be_const_class(be_class_lvh_chart) },
        { be_const_key_weak(lvh_bar, -1), be_const_class(be_class_lvh_bar) },
        { be_const_key_weak(lvh_scale_section, 49), be_const_class(be_class_lvh_scale_section) },
        { be_const_key_weak(lvh_cpicker, 65), be_const_class(be_class_lvh_cpicker) },
        { be_const_key_weak(lvh_root, 7), be_const_class(be_class_lvh_root) },
        { be_const_key_weak(r24, 22), be_const_var(6) },
        { be_const_key_weak(lvh_msgbox, 46), be_const_class(be_class_lvh_msgbox) },
        { be_const_key_weak(lvh_page, 0), be_const_class(be_class_lvh_page) },
        { be_const_key_weak(get_pages, -1), be_const_closure(class_HASPmota_get_pages_closure) },
        { be_const_key_weak(re_page_target, -1), be_const_var(10) },
        { be_const_key_weak(dimmed_panel, -1), be_const_var(18) },
        { be_const_key_weak(lvh_spangroup, -1), be_const_class(be_class_lvh_spangroup) },
        { be_const_key_weak(started, 35), be_const_var(0) },
        { be_const_key_weak(lvh_slider, -1), be_const_class(be_class_lvh_slider) },
        { be_const_key_weak(dimming_time, -1), be_const_var(14) },
        { be_const_key_weak(get_page_cur_parsing, -1), be_const_closure(class_HASPmota_get_page_cur_parsing_closure) },
        { be_const_key_weak(lvh_span, 45), be_const_class(be_class_lvh_span) },
        { be_const_key_weak(_load, -1), be_const_closure(class_HASPmota__load_closure) },
        { be_const_key_weak(lvh_line, -1), be_const_class(be_class_lvh_line) },
        { be_const_key_weak(parse_page, -1), be_const_closure(class_HASPmota_parse_page_closure) },
        { be_const_key_weak(lvh_fixed, 68), be_const_class(be_class_lvh_fixed) },
        { be_const_key_weak(lvh_btnmatrix, -1), be_const_class(be_class_lvh_btnmatrix) },
        { be_const_key_weak(parse, -1), be_const_closure(class_HASPmota_parse_closure) },
        { be_const_key_weak(lvh_tabview, -1), be_const_class(be_class_lvh_tabview) },
        { be_const_key_weak(parse_obj, -1), be_const_closure(class_HASPmota_parse_obj_closure) },
        { be_const_key_weak(init, -1), be_const_closure(class_HASPmota_init_closure) },
        { be_const_key_weak(PAGES_JSONL, 17), be_nested_str_weak(pages_X2Ejsonl) },
        { be_const_key_weak(lvh_flex, -1), be_const_class(be_class_lvh_flex) },
        { be_const_key_weak(dimming_state, -1), be_const_var(17) },
        { be_const_key_weak(vres, -1), be_const_var(2) },
        { be_const_key_weak(antiburn_time, -1), be_const_var(13) },
        { be_const_key_weak(auto_dimming, -1), be_const_closure(class_HASPmota_auto_dimming_closure) },
        { be_const_key_weak(get_page_cur, -1), be_const_closure(class_HASPmota_get_page_cur_closure) },
        { be_const_key_weak(event, -1), be_const_var(11) },
        { be_const_key_weak(lvh_dropdown_list, 34), be_const_class(be_class_lvh_dropdown_list) },
        { be_const_key_weak(DimmedPanel, -1), be_const_class(be_class_DimmedPanel) },
        { be_const_key_weak(r16, -1), be_const_var(5) },
        { be_const_key_weak(lvh_spinner, -1), be_const_class(be_class_lvh_spinner) },
        { be_const_key_weak(_remove_page, -1), be_const_closure(class_HASPmota__remove_page_closure) },
        { be_const_key_weak(sort, -1), be_const_static_closure(class_HASPmota_sort_closure) },
        { be_const_key_weak(lvh_roller, 18), be_const_class(be_class_lvh_roller) },
        { be_const_key_weak(do_action, 21), be_const_closure(class_HASPmota_do_action_closure) },
        { be_const_key_weak(Antiburn, -1), be_const_class(be_class_Antiburn) },
        { be_const_key_weak(dimming_duration, -1), be_const_var(16) },
        { be_const_key_weak(fix_lv_version, -1), be_const_static_closure(class_HASPmota_fix_lv_version_closure) },
        { be_const_key_weak(lvh_checkbox, 9), be_const_class(be_class_lvh_checkbox) },
        { be_const_key_weak(lvh_dropdown, -1), be_const_class(be_class_lvh_dropdown) },
        { be_const_key_weak(lvh_qrcode, 40), be_const_class(be_class_lvh_qrcode) },
        { be_const_key_weak(page_dir_to, 1), be_const_closure(class_HASPmota_page_dir_to_closure) },
        { be_const_key_weak(lvh_led, -1), be_const_class(be_class_lvh_led) },
    })),
    be_str_weak(HASPmota)
);

/********************************************************************
** Solidified function: _anonymous_
********************************************************************/
be_local_closure(_anonymous_,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(HASPmota),
    }),
    be_str_weak(_anonymous_),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x5C080200,  //  0001  MOVE	R2	R1
      0x7C080000,  //  0002  CALL	R2	0
      0x80040400,  //  0003  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified module: haspmota
********************************************************************/
be_local_module(haspmota,
    "haspmota",
    be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(HASPmota, -1), be_const_class(be_class_HASPmota) },
        { be_const_key_weak(init, -1), be_const_closure(_anonymous__closure) },
    }))
);
BE_EXPORT_VARIABLE be_define_const_native_module(haspmota);
/********************************************************************/
/********************************************************************/
/* End of solidification */
