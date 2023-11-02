/********************************************************************
 * Tasmota HASPmota solidified
 *******************************************************************/
#include "be_constobj.h"

#ifdef USE_LVGL
#ifdef USE_LVGL_HASPMOTA

extern const bclass be_class_lv_anim;
extern const bclass be_class_lv_arc;
extern const bclass be_class_lv_bar;
extern const bclass be_class_lv_btn;
extern const bclass be_class_lv_btnmatrix;
extern const bclass be_class_lv_canvas;
extern const bclass be_class_lv_chart;
extern const bclass be_class_lv_checkbox;
extern const bclass be_class_lv_color;
extern const bclass be_class_lv_colorwheel;
extern const bclass be_class_lv_disp;
extern const bclass be_class_lv_dropdown;
extern const bclass be_class_lv_font;
extern const bclass be_class_lv_group;
extern const bclass be_class_lv_img;
extern const bclass be_class_lv_imgbtn;
extern const bclass be_class_lv_indev;
extern const bclass be_class_lv_label;
extern const bclass be_class_lv_led;
extern const bclass be_class_lv_line;
extern const bclass be_class_lv_meter;
extern const bclass be_class_lv_msgbox;
extern const bclass be_class_lv_obj;
extern const bclass be_class_lv_roller;
extern const bclass be_class_lv_slider;
extern const bclass be_class_lv_spinbox;
extern const bclass be_class_lv_spinner;
extern const bclass be_class_lv_style;
extern const bclass be_class_lv_switch;
extern const bclass be_class_lv_table;
extern const bclass be_class_lv_textarea;
extern const bclass be_class_lv_theme;
extern const bclass be_class_lv_timer;
extern const bclass be_class_lv_qrcode;

extern const bclass be_class_lvh_page;

/********************************************************************
** Solidified function: get_obj
********************************************************************/
be_local_closure(lvh_page_get_obj,   /* name */
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
    /* K0   */  be_nested_str_weak(_obj_id),
    /* K1   */  be_nested_str_weak(find),
    }),
    be_str_weak(get_obj),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x80040400,  //  0004  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(lvh_page_init,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[16]) {     /* constants */
    /* K0   */  be_nested_str_weak(global),
    /* K1   */  be_nested_str_weak(_oh),
    /* K2   */  be_const_int(1),
    /* K3   */  be_nested_str_weak(_page_id),
    /* K4   */  be_nested_str_weak(_obj_id),
    /* K5   */  be_nested_str_weak(_lv_scr),
    /* K6   */  be_nested_str_weak(lv),
    /* K7   */  be_nested_str_weak(scr_act),
    /* K8   */  be_const_int(0),
    /* K9   */  be_nested_str_weak(layer_top),
    /* K10  */  be_nested_str_weak(obj),
    /* K11  */  be_nested_str_weak(get_style_bg_color),
    /* K12  */  be_nested_str_weak(set_style_bg_color),
    /* K13  */  be_nested_str_weak(lvh_scr),
    /* K14  */  be_nested_str_weak(p),
    /* K15  */  be_nested_str_weak(b0),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[66]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0x90020202,  //  0001  SETMBR	R0	K1	R2
      0x60100009,  //  0002  GETGBL	R4	G9
      0x5C140200,  //  0003  MOVE	R5	R1
      0x7C100200,  //  0004  CALL	R4	1
      0x5C040800,  //  0005  MOVE	R1	R4
      0x4C100000,  //  0006  LDNIL	R4
      0x1C100204,  //  0007  EQ	R4	R1	R4
      0x78120000,  //  0008  JMPF	R4	#000A
      0x58040002,  //  0009  LDCONST	R1	K2
      0x90020601,  //  000A  SETMBR	R0	K3	R1
      0x60100013,  //  000B  GETGBL	R4	G19
      0x7C100000,  //  000C  CALL	R4	0
      0x90020804,  //  000D  SETMBR	R0	K4	R4
      0x1C100302,  //  000E  EQ	R4	R1	K2
      0x78120004,  //  000F  JMPF	R4	#0015
      0xB8120C00,  //  0010  GETNGBL	R4	K6
      0x8C100907,  //  0011  GETMET	R4	R4	K7
      0x7C100200,  //  0012  CALL	R4	1
      0x90020A04,  //  0013  SETMBR	R0	K5	R4
      0x70020016,  //  0014  JMP		#002C
      0x1C100308,  //  0015  EQ	R4	R1	K8
      0x78120004,  //  0016  JMPF	R4	#001C
      0xB8120C00,  //  0017  GETNGBL	R4	K6
      0x8C100909,  //  0018  GETMET	R4	R4	K9
      0x7C100200,  //  0019  CALL	R4	1
      0x90020A04,  //  001A  SETMBR	R0	K5	R4
      0x7002000F,  //  001B  JMP		#002C
      0xB8120C00,  //  001C  GETNGBL	R4	K6
      0x8C10090A,  //  001D  GETMET	R4	R4	K10
      0x58180008,  //  001E  LDCONST	R6	K8
      0x7C100400,  //  001F  CALL	R4	2
      0x90020A04,  //  0020  SETMBR	R0	K5	R4
      0xB8120C00,  //  0021  GETNGBL	R4	K6
      0x8C100907,  //  0022  GETMET	R4	R4	K7
      0x7C100200,  //  0023  CALL	R4	1
      0x8C10090B,  //  0024  GETMET	R4	R4	K11
      0x58180008,  //  0025  LDCONST	R6	K8
      0x7C100400,  //  0026  CALL	R4	2
      0x88140105,  //  0027  GETMBR	R5	R0	K5
      0x8C140B0C,  //  0028  GETMET	R5	R5	K12
      0x5C1C0800,  //  0029  MOVE	R7	R4
      0x58200008,  //  002A  LDCONST	R8	K8
      0x7C140600,  //  002B  CALL	R5	3
      0x88100101,  //  002C  GETMBR	R4	R0	K1
      0x8810090D,  //  002D  GETMBR	R4	R4	K13
      0x5C140800,  //  002E  MOVE	R5	R4
      0x4C180000,  //  002F  LDNIL	R6
      0x5C1C0000,  //  0030  MOVE	R7	R0
      0x4C200000,  //  0031  LDNIL	R8
      0x88240105,  //  0032  GETMBR	R9	R0	K5
      0x7C140800,  //  0033  CALL	R5	4
      0x88180104,  //  0034  GETMBR	R6	R0	K4
      0x981A1005,  //  0035  SETIDX	R6	K8	R5
      0x60180008,  //  0036  GETGBL	R6	G8
      0x881C0103,  //  0037  GETMBR	R7	R0	K3
      0x7C180200,  //  0038  CALL	R6	1
      0x001A1C06,  //  0039  ADD	R6	K14	R6
      0x900C0C00,  //  003A  SETMBR	R3	R6	R0
      0x60180008,  //  003B  GETGBL	R6	G8
      0x881C0103,  //  003C  GETMBR	R7	R0	K3
      0x7C180200,  //  003D  CALL	R6	1
      0x001A1C06,  //  003E  ADD	R6	K14	R6
      0x00180D0F,  //  003F  ADD	R6	R6	K15
      0x900C0C05,  //  0040  SETMBR	R3	R6	R5
      0x80000000,  //  0041  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_scr
********************************************************************/
be_local_closure(lvh_page_get_scr,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(_lv_scr),
    }),
    be_str_weak(get_scr),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: show
********************************************************************/
be_local_closure(lvh_page_show,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
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
    ( &(const bvalue[18]) {     /* constants */
    /* K0   */  be_nested_str_weak(_lv_scr),
    /* K1   */  be_nested_str_weak(_p),
    /* K2   */  be_nested_str_weak(lv),
    /* K3   */  be_nested_str_weak(scr_act),
    /* K4   */  be_nested_str_weak(_oh),
    /* K5   */  be_nested_str_weak(page_dir_to),
    /* K6   */  be_nested_str_weak(id),
    /* K7   */  be_nested_str_weak(_X7B_X22hasp_X22_X3A_X7B_X22p_X25i_X22_X3A_X22out_X22_X7D_X7D),
    /* K8   */  be_nested_str_weak(lvh_page_cur_idx),
    /* K9   */  be_nested_str_weak(tasmota),
    /* K10  */  be_nested_str_weak(set_timer),
    /* K11  */  be_const_int(0),
    /* K12  */  be_nested_str_weak(_X7B_X22hasp_X22_X3A_X7B_X22p_X25i_X22_X3A_X22in_X22_X7D_X7D),
    /* K13  */  be_nested_str_weak(_page_id),
    /* K14  */  be_nested_str_weak(show_anim),
    /* K15  */  be_nested_str_weak(find),
    /* K16  */  be_nested_str_weak(SCR_LOAD_ANIM_NONE),
    /* K17  */  be_nested_str_weak(scr_load_anim),
    }),
    be_str_weak(show),
    &be_const_str_solidified,
    ( &(const binstruction[66]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x4C100000,  //  0001  LDNIL	R4
      0x1C0C0604,  //  0002  EQ	R3	R3	R4
      0x780E0001,  //  0003  JMPF	R3	#0006
      0x4C0C0000,  //  0004  LDNIL	R3
      0x80040600,  //  0005  RET	1	R3
      0x880C0100,  //  0006  GETMBR	R3	R0	K0
      0x880C0701,  //  0007  GETMBR	R3	R3	K1
      0xB8120400,  //  0008  GETNGBL	R4	K2
      0x8C100903,  //  0009  GETMET	R4	R4	K3
      0x7C100200,  //  000A  CALL	R4	1
      0x88100901,  //  000B  GETMBR	R4	R4	K1
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
      0x880C0104,  //  0016  GETMBR	R3	R0	K4
      0x8C0C0705,  //  0017  GETMET	R3	R3	K5
      0x8C140106,  //  0018  GETMET	R5	R0	K6
      0x7C140200,  //  0019  CALL	R5	1
      0x7C0C0400,  //  001A  CALL	R3	2
      0x5C040600,  //  001B  MOVE	R1	R3
      0x600C0018,  //  001C  GETGBL	R3	G24
      0x58100007,  //  001D  LDCONST	R4	K7
      0x88140104,  //  001E  GETMBR	R5	R0	K4
      0x88140B08,  //  001F  GETMBR	R5	R5	K8
      0x7C0C0400,  //  0020  CALL	R3	2
      0xB8121200,  //  0021  GETNGBL	R4	K9
      0x8C10090A,  //  0022  GETMET	R4	R4	K10
      0x5818000B,  //  0023  LDCONST	R6	K11
      0x841C0000,  //  0024  CLOSURE	R7	P0
      0x7C100600,  //  0025  CALL	R4	3
      0x60100018,  //  0026  GETGBL	R4	G24
      0x5814000C,  //  0027  LDCONST	R5	K12
      0x8818010D,  //  0028  GETMBR	R6	R0	K13
      0x7C100400,  //  0029  CALL	R4	2
      0xB8161200,  //  002A  GETNGBL	R5	K9
      0x8C140B0A,  //  002B  GETMET	R5	R5	K10
      0x581C000B,  //  002C  LDCONST	R7	K11
      0x84200001,  //  002D  CLOSURE	R8	P1
      0x7C140600,  //  002E  CALL	R5	3
      0x88140104,  //  002F  GETMBR	R5	R0	K4
      0x8818010D,  //  0030  GETMBR	R6	R0	K13
      0x90161006,  //  0031  SETMBR	R5	K8	R6
      0x8814010E,  //  0032  GETMBR	R5	R0	K14
      0x8C140B0F,  //  0033  GETMET	R5	R5	K15
      0x5C1C0200,  //  0034  MOVE	R7	R1
      0xB8220400,  //  0035  GETNGBL	R8	K2
      0x88201110,  //  0036  GETMBR	R8	R8	K16
      0x7C140600,  //  0037  CALL	R5	3
      0xB81A0400,  //  0038  GETNGBL	R6	K2
      0x8C180D11,  //  0039  GETMET	R6	R6	K17
      0x88200100,  //  003A  GETMBR	R8	R0	K0
      0x5C240A00,  //  003B  MOVE	R9	R5
      0x5C280400,  //  003C  MOVE	R10	R2
      0x582C000B,  //  003D  LDCONST	R11	K11
      0x50300000,  //  003E  LDBOOL	R12	0	0
      0x7C180C00,  //  003F  CALL	R6	6
      0xA0000000,  //  0040  CLOSE	R0
      0x80000000,  //  0041  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_obj
********************************************************************/
be_local_closure(lvh_page_set_obj,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(_obj_id),
    }),
    be_str_weak(set_obj),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x980C0202,  //  0001  SETIDX	R3	R1	R2
      0x80000000,  //  0002  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: id
********************************************************************/
be_local_closure(lvh_page_id,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(_page_id),
    }),
    be_str_weak(id),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
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
    be_nested_map(14,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(prev, 2), be_const_var(4) },
        { be_const_key_weak(_obj_id, -1), be_const_var(0) },
        { be_const_key_weak(show_anim, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(5,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(0, 3), be_const_int(0) },
        { be_const_key_int(1, -1), be_const_int(5) },
        { be_const_key_int(2, -1), be_const_int(8) },
        { be_const_key_int(-1, -1), be_const_int(6) },
        { be_const_key_int(-2, -1), be_const_int(7) },
    }))    ) } )) },
        { be_const_key_weak(_oh, -1), be_const_var(3) },
        { be_const_key_weak(id, -1), be_const_closure(lvh_page_id_closure) },
        { be_const_key_weak(init, -1), be_const_closure(lvh_page_init_closure) },
        { be_const_key_weak(next, 13), be_const_var(5) },
        { be_const_key_weak(_page_id, -1), be_const_var(1) },
        { be_const_key_weak(show, -1), be_const_closure(lvh_page_show_closure) },
        { be_const_key_weak(_lv_scr, -1), be_const_var(2) },
        { be_const_key_weak(back, 4), be_const_var(6) },
        { be_const_key_weak(get_obj, 12), be_const_closure(lvh_page_get_obj_closure) },
        { be_const_key_weak(set_obj, -1), be_const_closure(lvh_page_set_obj_closure) },
        { be_const_key_weak(get_scr, -1), be_const_closure(lvh_page_get_scr_closure) },
    })),
    be_str_weak(lvh_page)
);
/*******************************************************************/

void be_load_lvh_page_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_lvh_page);
    be_setglobal(vm, "lvh_page");
    be_pop(vm, 1);
}

extern const bclass be_class_lvh_obj;

/********************************************************************
** Solidified function: get_text_rule
********************************************************************/
be_local_closure(lvh_obj_get_text_rule,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(_text_rule),
    }),
    be_str_weak(get_text_rule),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_val_rule
********************************************************************/
be_local_closure(lvh_obj_set_val_rule,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
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
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(_val_rule),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(remove_rule),
    /* K3   */  be_nested_str_weak(add_rule),
    }),
    be_str_weak(set_val_rule),
    &be_const_str_solidified,
    ( &(const binstruction[21]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x4C0C0000,  //  0001  LDNIL	R3
      0x20080403,  //  0002  NE	R2	R2	R3
      0x780A0004,  //  0003  JMPF	R2	#0009
      0xB80A0200,  //  0004  GETNGBL	R2	K1
      0x8C080502,  //  0005  GETMET	R2	R2	K2
      0x88100100,  //  0006  GETMBR	R4	R0	K0
      0x5C140000,  //  0007  MOVE	R5	R0
      0x7C080600,  //  0008  CALL	R2	3
      0x60080008,  //  0009  GETGBL	R2	G8
      0x5C0C0200,  //  000A  MOVE	R3	R1
      0x7C080200,  //  000B  CALL	R2	1
      0x90020002,  //  000C  SETMBR	R0	K0	R2
      0xB80A0200,  //  000D  GETNGBL	R2	K1
      0x8C080503,  //  000E  GETMET	R2	R2	K3
      0x88100100,  //  000F  GETMBR	R4	R0	K0
      0x84140000,  //  0010  CLOSURE	R5	P0
      0x5C180000,  //  0011  MOVE	R6	R0
      0x7C080800,  //  0012  CALL	R2	4
      0xA0000000,  //  0013  CLOSE	R0
      0x80000000,  //  0014  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_value_str
********************************************************************/
be_local_closure(lvh_obj_set_value_str,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(set_text),
    }),
    be_str_weak(set_value_str),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x5C100200,  //  0001  MOVE	R4	R1
      0x7C080400,  //  0002  CALL	R2	2
      0x80000000,  //  0003  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_text_color
********************************************************************/
be_local_closure(lvh_obj_set_text_color,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(_lv_obj),
    /* K1   */  be_nested_str_weak(set_style_text_color),
    /* K2   */  be_nested_str_weak(parse_color),
    }),
    be_str_weak(set_text_color),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x8C0C0701,  //  0001  GETMET	R3	R3	K1
      0x8C140102,  //  0002  GETMET	R5	R0	K2
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
** Solidified function: set_align
********************************************************************/
be_local_closure(lvh_obj_set_align,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[13]) {     /* constants */
    /* K0   */  be_nested_str_weak(check_label),
    /* K1   */  be_const_int(0),
    /* K2   */  be_nested_str_weak(left),
    /* K3   */  be_nested_str_weak(lv),
    /* K4   */  be_nested_str_weak(TEXT_ALIGN_LEFT),
    /* K5   */  be_const_int(1),
    /* K6   */  be_nested_str_weak(center),
    /* K7   */  be_nested_str_weak(TEXT_ALIGN_CENTER),
    /* K8   */  be_const_int(2),
    /* K9   */  be_nested_str_weak(right),
    /* K10  */  be_nested_str_weak(TEXT_ALIGN_RIGHT),
    /* K11  */  be_nested_str_weak(_lv_label),
    /* K12  */  be_nested_str_weak(set_style_text_align),
    }),
    be_str_weak(set_align),
    &be_const_str_solidified,
    ( &(const binstruction[29]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x8C0C0100,  //  0001  GETMET	R3	R0	K0
      0x7C0C0200,  //  0002  CALL	R3	1
      0x1C0C0301,  //  0003  EQ	R3	R1	K1
      0x740E0001,  //  0004  JMPT	R3	#0007
      0x1C0C0302,  //  0005  EQ	R3	R1	K2
      0x780E0002,  //  0006  JMPF	R3	#000A
      0xB80E0600,  //  0007  GETNGBL	R3	K3
      0x88080704,  //  0008  GETMBR	R2	R3	K4
      0x7002000C,  //  0009  JMP		#0017
      0x1C0C0305,  //  000A  EQ	R3	R1	K5
      0x740E0001,  //  000B  JMPT	R3	#000E
      0x1C0C0306,  //  000C  EQ	R3	R1	K6
      0x780E0002,  //  000D  JMPF	R3	#0011
      0xB80E0600,  //  000E  GETNGBL	R3	K3
      0x88080707,  //  000F  GETMBR	R2	R3	K7
      0x70020005,  //  0010  JMP		#0017
      0x1C0C0308,  //  0011  EQ	R3	R1	K8
      0x740E0001,  //  0012  JMPT	R3	#0015
      0x1C0C0309,  //  0013  EQ	R3	R1	K9
      0x780E0001,  //  0014  JMPF	R3	#0017
      0xB80E0600,  //  0015  GETNGBL	R3	K3
      0x8808070A,  //  0016  GETMBR	R2	R3	K10
      0x880C010B,  //  0017  GETMBR	R3	R0	K11
      0x8C0C070C,  //  0018  GETMET	R3	R3	K12
      0x5C140400,  //  0019  MOVE	R5	R2
      0x58180001,  //  001A  LDCONST	R6	K1
      0x7C0C0600,  //  001B  CALL	R3	3
      0x80000000,  //  001C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_pad_bottom2
********************************************************************/
be_local_closure(lvh_obj_set_pad_bottom2,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(_lv_part2_selector),
    /* K1   */  be_nested_str_weak(_lv_obj),
    /* K2   */  be_nested_str_weak(set_style_pad_bottom),
    /* K3   */  be_nested_str_weak(lv),
    /* K4   */  be_nested_str_weak(STATE_DEFAULT),
    }),
    be_str_weak(set_pad_bottom2),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x4C0C0000,  //  0001  LDNIL	R3
      0x20080403,  //  0002  NE	R2	R2	R3
      0x780A0009,  //  0003  JMPF	R2	#000E
      0x88080101,  //  0004  GETMBR	R2	R0	K1
      0x8C080502,  //  0005  GETMET	R2	R2	K2
      0x60100009,  //  0006  GETGBL	R4	G9
      0x5C140200,  //  0007  MOVE	R5	R1
      0x7C100200,  //  0008  CALL	R4	1
      0x88140100,  //  0009  GETMBR	R5	R0	K0
      0xB81A0600,  //  000A  GETNGBL	R6	K3
      0x88180D04,  //  000B  GETMBR	R6	R6	K4
      0x30140A06,  //  000C  OR	R5	R5	R6
      0x7C080600,  //  000D  CALL	R2	3
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_action
********************************************************************/
be_local_closure(lvh_obj_set_action,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(_action),
    }),
    be_str_weak(set_action),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x60080008,  //  0000  GETGBL	R2	G8
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x7C080200,  //  0002  CALL	R2	1
      0x90020002,  //  0003  SETMBR	R0	K0	R2
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: event_cb
********************************************************************/
be_local_closure(lvh_obj_event_cb,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
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
        3,                          /* nstack */
        0,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 7),
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
    ( &(const bvalue[22]) {     /* constants */
    /* K0   */  be_nested_str_weak(_page),
    /* K1   */  be_nested_str_weak(_oh),
    /* K2   */  be_nested_str_weak(code),
    /* K3   */  be_nested_str_weak(action),
    /* K4   */  be_nested_str_weak(),
    /* K5   */  be_nested_str_weak(lv),
    /* K6   */  be_nested_str_weak(EVENT_CLICKED),
    /* K7   */  be_nested_str_weak(tasmota),
    /* K8   */  be_nested_str_weak(set_timer),
    /* K9   */  be_const_int(0),
    /* K10  */  be_nested_str_weak(_event_map),
    /* K11  */  be_nested_str_weak(find),
    /* K12  */  be_nested_str_weak(json),
    /* K13  */  be_nested_str_weak(EVENT_VALUE_CHANGED),
    /* K14  */  be_nested_str_weak(val),
    /* K15  */  be_nested_str_weak(_X2C_X22val_X22_X3A_X25s),
    /* K16  */  be_nested_str_weak(dump),
    /* K17  */  be_nested_str_weak(text),
    /* K18  */  be_nested_str_weak(_X2C_X22text_X22_X3A),
    /* K19  */  be_nested_str_weak(_X7B_X22hasp_X22_X3A_X7B_X22p_X25ib_X25i_X22_X3A_X7B_X22event_X22_X3A_X22_X25s_X22_X25s_X7D_X7D_X7D),
    /* K20  */  be_nested_str_weak(_page_id),
    /* K21  */  be_nested_str_weak(id),
    }),
    be_str_weak(event_cb),
    &be_const_str_solidified,
    ( &(const binstruction[72]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x88080501,  //  0001  GETMBR	R2	R2	K1
      0x880C0302,  //  0002  GETMBR	R3	R1	K2
      0x88100103,  //  0003  GETMBR	R4	R0	K3
      0x20100904,  //  0004  NE	R4	R4	K4
      0x78120008,  //  0005  JMPF	R4	#000F
      0xB8120A00,  //  0006  GETNGBL	R4	K5
      0x88100906,  //  0007  GETMBR	R4	R4	K6
      0x1C100604,  //  0008  EQ	R4	R3	R4
      0x78120004,  //  0009  JMPF	R4	#000F
      0xB8120E00,  //  000A  GETNGBL	R4	K7
      0x8C100908,  //  000B  GETMET	R4	R4	K8
      0x58180009,  //  000C  LDCONST	R6	K9
      0x841C0000,  //  000D  CLOSURE	R7	P0
      0x7C100600,  //  000E  CALL	R4	3
      0x8810010A,  //  000F  GETMBR	R4	R0	K10
      0x8C10090B,  //  0010  GETMET	R4	R4	K11
      0x5C180600,  //  0011  MOVE	R6	R3
      0x7C100400,  //  0012  CALL	R4	2
      0x4C140000,  //  0013  LDNIL	R5
      0x20140805,  //  0014  NE	R5	R4	R5
      0x7816002F,  //  0015  JMPF	R5	#0046
      0xA4161800,  //  0016  IMPORT	R5	K12
      0x58180004,  //  0017  LDCONST	R6	K4
      0x881C0302,  //  0018  GETMBR	R7	R1	K2
      0xB8220A00,  //  0019  GETNGBL	R8	K5
      0x8820110D,  //  001A  GETMBR	R8	R8	K13
      0x1C1C0E08,  //  001B  EQ	R7	R7	R8
      0x781E001A,  //  001C  JMPF	R7	#0038
      0xA8020015,  //  001D  EXBLK	0	#0034
      0x881C010E,  //  001E  GETMBR	R7	R0	K14
      0x4C200000,  //  001F  LDNIL	R8
      0x20200E08,  //  0020  NE	R8	R7	R8
      0x78220006,  //  0021  JMPF	R8	#0029
      0x60200018,  //  0022  GETGBL	R8	G24
      0x5824000F,  //  0023  LDCONST	R9	K15
      0x8C280B10,  //  0024  GETMET	R10	R5	K16
      0x5C300E00,  //  0025  MOVE	R12	R7
      0x7C280400,  //  0026  CALL	R10	2
      0x7C200400,  //  0027  CALL	R8	2
      0x5C181000,  //  0028  MOVE	R6	R8
      0x88200111,  //  0029  GETMBR	R8	R0	K17
      0x4C240000,  //  002A  LDNIL	R9
      0x20241009,  //  002B  NE	R9	R8	R9
      0x78260004,  //  002C  JMPF	R9	#0032
      0x00180D12,  //  002D  ADD	R6	R6	K18
      0x8C240B10,  //  002E  GETMET	R9	R5	K16
      0x5C2C1000,  //  002F  MOVE	R11	R8
      0x7C240400,  //  0030  CALL	R9	2
      0x00180C09,  //  0031  ADD	R6	R6	R9
      0xA8040001,  //  0032  EXBLK	1	1
      0x70020003,  //  0033  JMP		#0038
      0xAC1C0000,  //  0034  CATCH	R7	0	0
      0x70020000,  //  0035  JMP		#0037
      0x70020000,  //  0036  JMP		#0038
      0xB0080000,  //  0037  RAISE	2	R0	R0
      0x601C0018,  //  0038  GETGBL	R7	G24
      0x58200013,  //  0039  LDCONST	R8	K19
      0x88240100,  //  003A  GETMBR	R9	R0	K0
      0x88241314,  //  003B  GETMBR	R9	R9	K20
      0x88280115,  //  003C  GETMBR	R10	R0	K21
      0x5C2C0800,  //  003D  MOVE	R11	R4
      0x5C300C00,  //  003E  MOVE	R12	R6
      0x7C1C0A00,  //  003F  CALL	R7	5
      0xB8220E00,  //  0040  GETNGBL	R8	K7
      0x8C201108,  //  0041  GETMET	R8	R8	K8
      0x58280009,  //  0042  LDCONST	R10	K9
      0x842C0001,  //  0043  CLOSURE	R11	P1
      0x7C200600,  //  0044  CALL	R8	3
      0xA0140000,  //  0045  CLOSE	R5
      0xA0000000,  //  0046  CLOSE	R0
      0x80000000,  //  0047  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_pad_all2
********************************************************************/
be_local_closure(lvh_obj_set_pad_all2,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(_lv_part2_selector),
    /* K1   */  be_nested_str_weak(_lv_obj),
    /* K2   */  be_nested_str_weak(set_style_pad_all),
    /* K3   */  be_nested_str_weak(lv),
    /* K4   */  be_nested_str_weak(STATE_DEFAULT),
    }),
    be_str_weak(set_pad_all2),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x4C0C0000,  //  0001  LDNIL	R3
      0x20080403,  //  0002  NE	R2	R2	R3
      0x780A0009,  //  0003  JMPF	R2	#000E
      0x88080101,  //  0004  GETMBR	R2	R0	K1
      0x8C080502,  //  0005  GETMET	R2	R2	K2
      0x60100009,  //  0006  GETGBL	R4	G9
      0x5C140200,  //  0007  MOVE	R5	R1
      0x7C100200,  //  0008  CALL	R4	1
      0x88140100,  //  0009  GETMBR	R5	R0	K0
      0xB81A0600,  //  000A  GETNGBL	R6	K3
      0x88180D04,  //  000B  GETMBR	R6	R6	K4
      0x30140A06,  //  000C  OR	R5	R5	R6
      0x7C080600,  //  000D  CALL	R2	3
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: text_rule_matched
********************************************************************/
be_local_closure(lvh_obj_text_rule_matched,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(int),
    /* K1   */  be_nested_str_weak(_text_rule_function),
    /* K2   */  be_nested_str_weak(HSP_X3A_X20failed_X20to_X20run_X20self_X2E_text_rule_function_X20_X2D_X20_X25s_X20_X28_X25s_X29),
    /* K3   */  be_nested_str_weak(_text_rule_format),
    /* K4   */  be_nested_str_weak(string),
    /* K5   */  be_nested_str_weak(),
    /* K6   */  be_nested_str_weak(text),
    }),
    be_str_weak(text_rule_matched),
    &be_const_str_solidified,
    ( &(const binstruction[47]) {  /* code */
      0x60080004,  //  0000  GETGBL	R2	G4
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x7C080200,  //  0002  CALL	R2	1
      0x1C080500,  //  0003  EQ	R2	R2	K0
      0x780A0003,  //  0004  JMPF	R2	#0009
      0x6008000A,  //  0005  GETGBL	R2	G10
      0x5C0C0200,  //  0006  MOVE	R3	R1
      0x7C080200,  //  0007  CALL	R2	1
      0x5C040400,  //  0008  MOVE	R1	R2
      0x88080101,  //  0009  GETMBR	R2	R0	K1
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
      0x581C0002,  //  0018  LDCONST	R7	K2
      0x5C200600,  //  0019  MOVE	R8	R3
      0x5C240800,  //  001A  MOVE	R9	R4
      0x7C180600,  //  001B  CALL	R6	3
      0x7C140200,  //  001C  CALL	R5	1
      0x70020000,  //  001D  JMP		#001F
      0xB0080000,  //  001E  RAISE	2	R0	R0
      0x880C0103,  //  001F  GETMBR	R3	R0	K3
      0x60100004,  //  0020  GETGBL	R4	G4
      0x5C140600,  //  0021  MOVE	R5	R3
      0x7C100200,  //  0022  CALL	R4	1
      0x1C100904,  //  0023  EQ	R4	R4	K4
      0x78120005,  //  0024  JMPF	R4	#002B
      0x60100018,  //  0025  GETGBL	R4	G24
      0x5C140600,  //  0026  MOVE	R5	R3
      0x5C180200,  //  0027  MOVE	R6	R1
      0x7C100400,  //  0028  CALL	R4	2
      0x5C0C0800,  //  0029  MOVE	R3	R4
      0x70020000,  //  002A  JMP		#002C
      0x580C0005,  //  002B  LDCONST	R3	K5
      0x90020C03,  //  002C  SETMBR	R0	K6	R3
      0x50100000,  //  002D  LDBOOL	R4	0	0
      0x80040800,  //  002E  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_value_ofs_x
********************************************************************/
be_local_closure(lvh_obj_set_value_ofs_x,   /* name */
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
    /* K0   */  be_nested_str_weak(check_label),
    /* K1   */  be_nested_str_weak(_lv_label),
    /* K2   */  be_nested_str_weak(set_x),
    }),
    be_str_weak(set_value_ofs_x),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x7C080200,  //  0001  CALL	R2	1
      0x88080101,  //  0002  GETMBR	R2	R0	K1
      0x8C080502,  //  0003  GETMET	R2	R2	K2
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
** Solidified function: set_pad_right2
********************************************************************/
be_local_closure(lvh_obj_set_pad_right2,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(_lv_part2_selector),
    /* K1   */  be_nested_str_weak(_lv_obj),
    /* K2   */  be_nested_str_weak(set_style_pad_right),
    /* K3   */  be_nested_str_weak(lv),
    /* K4   */  be_nested_str_weak(STATE_DEFAULT),
    }),
    be_str_weak(set_pad_right2),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x4C0C0000,  //  0001  LDNIL	R3
      0x20080403,  //  0002  NE	R2	R2	R3
      0x780A0009,  //  0003  JMPF	R2	#000E
      0x88080101,  //  0004  GETMBR	R2	R0	K1
      0x8C080502,  //  0005  GETMET	R2	R2	K2
      0x60100009,  //  0006  GETGBL	R4	G9
      0x5C140200,  //  0007  MOVE	R5	R1
      0x7C100200,  //  0008  CALL	R4	1
      0x88140100,  //  0009  GETMBR	R5	R0	K0
      0xB81A0600,  //  000A  GETNGBL	R6	K3
      0x88180D04,  //  000B  GETMBR	R6	R6	K4
      0x30140A06,  //  000C  OR	R5	R5	R6
      0x7C080600,  //  000D  CALL	R2	3
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_align
********************************************************************/
be_local_closure(lvh_obj_get_align,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[10]) {     /* constants */
    /* K0   */  be_nested_str_weak(_lv_label),
    /* K1   */  be_nested_str_weak(get_style_text_align),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str_weak(lv),
    /* K4   */  be_nested_str_weak(TEXT_ALIGN_LEFT),
    /* K5   */  be_nested_str_weak(left),
    /* K6   */  be_nested_str_weak(TEXT_ALIGN_CENTER),
    /* K7   */  be_nested_str_weak(center),
    /* K8   */  be_nested_str_weak(TEXT_ALIGN_RIGHT),
    /* K9   */  be_nested_str_weak(right),
    }),
    be_str_weak(get_align),
    &be_const_str_solidified,
    ( &(const binstruction[32]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x4C080000,  //  0001  LDNIL	R2
      0x1C040202,  //  0002  EQ	R1	R1	R2
      0x78060001,  //  0003  JMPF	R1	#0006
      0x4C040000,  //  0004  LDNIL	R1
      0x80040200,  //  0005  RET	1	R1
      0x4C040000,  //  0006  LDNIL	R1
      0x88080100,  //  0007  GETMBR	R2	R0	K0
      0x8C080501,  //  0008  GETMET	R2	R2	K1
      0x58100002,  //  0009  LDCONST	R4	K2
      0x7C080400,  //  000A  CALL	R2	2
      0xB80A0600,  //  000B  GETNGBL	R2	K3
      0x88080504,  //  000C  GETMBR	R2	R2	K4
      0x1C080202,  //  000D  EQ	R2	R1	R2
      0x780A0001,  //  000E  JMPF	R2	#0011
      0x80060A00,  //  000F  RET	1	K5
      0x7002000D,  //  0010  JMP		#001F
      0xB80A0600,  //  0011  GETNGBL	R2	K3
      0x88080506,  //  0012  GETMBR	R2	R2	K6
      0x1C080202,  //  0013  EQ	R2	R1	R2
      0x780A0001,  //  0014  JMPF	R2	#0017
      0x80060E00,  //  0015  RET	1	K7
      0x70020007,  //  0016  JMP		#001F
      0xB80A0600,  //  0017  GETNGBL	R2	K3
      0x88080508,  //  0018  GETMBR	R2	R2	K8
      0x1C080202,  //  0019  EQ	R2	R1	R2
      0x780A0001,  //  001A  JMPF	R2	#001D
      0x80061200,  //  001B  RET	1	K9
      0x70020001,  //  001C  JMP		#001F
      0x4C080000,  //  001D  LDNIL	R2
      0x80040400,  //  001E  RET	1	R2
      0x80000000,  //  001F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_enabled
********************************************************************/
be_local_closure(lvh_obj_get_enabled,   /* name */
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
    /* K0   */  be_nested_str_weak(_lv_obj),
    /* K1   */  be_nested_str_weak(has_state),
    /* K2   */  be_nested_str_weak(lv),
    /* K3   */  be_nested_str_weak(STATE_DISABLED),
    }),
    be_str_weak(get_enabled),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0xB80E0400,  //  0002  GETNGBL	R3	K2
      0x880C0703,  //  0003  GETMBR	R3	R3	K3
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
** Solidified function: init
********************************************************************/
be_local_closure(lvh_obj_init,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    5,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(_page),
    /* K1   */  be_nested_str_weak(_lv_class),
    /* K2   */  be_nested_str_weak(_lv_obj),
    /* K3   */  be_nested_str_weak(post_init),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0x90020002,  //  0000  SETMBR	R0	K0	R2
      0x4C140000,  //  0001  LDNIL	R5
      0x1C140805,  //  0002  EQ	R5	R4	R5
      0x78160007,  //  0003  JMPF	R5	#000C
      0x88140101,  //  0004  GETMBR	R5	R0	K1
      0x78160005,  //  0005  JMPF	R5	#000C
      0x88140101,  //  0006  GETMBR	R5	R0	K1
      0x5C180A00,  //  0007  MOVE	R6	R5
      0x5C1C0200,  //  0008  MOVE	R7	R1
      0x7C180200,  //  0009  CALL	R6	1
      0x90020406,  //  000A  SETMBR	R0	K2	R6
      0x70020000,  //  000B  JMP		#000D
      0x90020404,  //  000C  SETMBR	R0	K2	R4
      0x8C140103,  //  000D  GETMET	R5	R0	K3
      0x7C140200,  //  000E  CALL	R5	1
      0x80000000,  //  000F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_text
********************************************************************/
be_local_closure(lvh_obj_get_text,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(_lv_label),
    /* K1   */  be_nested_str_weak(get_text),
    }),
    be_str_weak(get_text),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x4C080000,  //  0001  LDNIL	R2
      0x1C040202,  //  0002  EQ	R1	R1	R2
      0x78060001,  //  0003  JMPF	R1	#0006
      0x4C040000,  //  0004  LDNIL	R1
      0x80040200,  //  0005  RET	1	R1
      0x88040100,  //  0006  GETMBR	R1	R0	K0
      0x8C040301,  //  0007  GETMET	R1	R1	K1
      0x7C040200,  //  0008  CALL	R1	1
      0x80040200,  //  0009  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_line_width
********************************************************************/
be_local_closure(lvh_obj_get_line_width,   /* name */
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
    /* K0   */  be_nested_str_weak(_lv_obj),
    /* K1   */  be_nested_str_weak(get_style_line_width),
    }),
    be_str_weak(get_line_width),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x80040400,  //  0004  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_obj
********************************************************************/
be_local_closure(lvh_obj_get_obj,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(_lv_obj),
    }),
    be_str_weak(get_obj),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_val_rule_formula
********************************************************************/
be_local_closure(lvh_obj_set_val_rule_formula,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(_val_rule_formula),
    /* K1   */  be_nested_str_weak(return_X20_X2F_X20val_X20_X2D_X3E_X20_X28),
    /* K2   */  be_nested_str_weak(_X29),
    /* K3   */  be_nested_str_weak(_val_rule_function),
    /* K4   */  be_nested_str_weak(HSP_X3A_X20failed_X20to_X20compile_X20_X27_X25s_X27_X20_X2D_X20_X25s_X20_X28_X25s_X29),
    }),
    be_str_weak(set_val_rule_formula),
    &be_const_str_solidified,
    ( &(const binstruction[29]) {  /* code */
      0x60080008,  //  0000  GETGBL	R2	G8
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x7C080200,  //  0002  CALL	R2	1
      0x90020002,  //  0003  SETMBR	R0	K0	R2
      0x88080100,  //  0004  GETMBR	R2	R0	K0
      0x000A0202,  //  0005  ADD	R2	K1	R2
      0x00080502,  //  0006  ADD	R2	R2	K2
      0xA8020007,  //  0007  EXBLK	0	#0010
      0x600C000D,  //  0008  GETGBL	R3	G13
      0x5C100400,  //  0009  MOVE	R4	R2
      0x7C0C0200,  //  000A  CALL	R3	1
      0x5C100600,  //  000B  MOVE	R4	R3
      0x7C100000,  //  000C  CALL	R4	0
      0x90020604,  //  000D  SETMBR	R0	K3	R4
      0xA8040001,  //  000E  EXBLK	1	1
      0x7002000B,  //  000F  JMP		#001C
      0xAC0C0002,  //  0010  CATCH	R3	0	2
      0x70020008,  //  0011  JMP		#001B
      0x60140001,  //  0012  GETGBL	R5	G1
      0x60180018,  //  0013  GETGBL	R6	G24
      0x581C0004,  //  0014  LDCONST	R7	K4
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
** Solidified function: get_click
********************************************************************/
be_local_closure(lvh_obj_get_click,   /* name */
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
    /* K0   */  be_nested_str_weak(get_enabled),
    }),
    be_str_weak(get_click),
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
** Solidified function: set_value_font
********************************************************************/
be_local_closure(lvh_obj_set_value_font,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(set_text_font),
    }),
    be_str_weak(set_value_font),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x5C100200,  //  0001  MOVE	R4	R1
      0x7C080400,  //  0002  CALL	R2	2
      0x80000000,  //  0003  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_toggle
********************************************************************/
be_local_closure(lvh_obj_set_toggle,   /* name */
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
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(toupper),
    /* K2   */  be_nested_str_weak(TRUE),
    /* K3   */  be_nested_str_weak(FALSE),
    /* K4   */  be_nested_str_weak(_lv_obj),
    /* K5   */  be_nested_str_weak(add_state),
    /* K6   */  be_nested_str_weak(lv),
    /* K7   */  be_nested_str_weak(STATE_CHECKED),
    /* K8   */  be_nested_str_weak(clear_state),
    }),
    be_str_weak(set_toggle),
    &be_const_str_solidified,
    ( &(const binstruction[32]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x600C0004,  //  0001  GETGBL	R3	G4
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C0C0200,  //  0003  CALL	R3	1
      0x1C0C0700,  //  0004  EQ	R3	R3	K0
      0x780E000C,  //  0005  JMPF	R3	#0013
      0x8C0C0501,  //  0006  GETMET	R3	R2	K1
      0x60140008,  //  0007  GETGBL	R5	G8
      0x5C180200,  //  0008  MOVE	R6	R1
      0x7C140200,  //  0009  CALL	R5	1
      0x7C0C0400,  //  000A  CALL	R3	2
      0x5C040600,  //  000B  MOVE	R1	R3
      0x1C0C0302,  //  000C  EQ	R3	R1	K2
      0x780E0001,  //  000D  JMPF	R3	#0010
      0x50040200,  //  000E  LDBOOL	R1	1	0
      0x70020002,  //  000F  JMP		#0013
      0x1C0C0303,  //  0010  EQ	R3	R1	K3
      0x780E0000,  //  0011  JMPF	R3	#0013
      0x50040000,  //  0012  LDBOOL	R1	0	0
      0x78060005,  //  0013  JMPF	R1	#001A
      0x880C0104,  //  0014  GETMBR	R3	R0	K4
      0x8C0C0705,  //  0015  GETMET	R3	R3	K5
      0xB8160C00,  //  0016  GETNGBL	R5	K6
      0x88140B07,  //  0017  GETMBR	R5	R5	K7
      0x7C0C0400,  //  0018  CALL	R3	2
      0x70020004,  //  0019  JMP		#001F
      0x880C0104,  //  001A  GETMBR	R3	R0	K4
      0x8C0C0708,  //  001B  GETMET	R3	R3	K8
      0xB8160C00,  //  001C  GETNGBL	R5	K6
      0x88140B07,  //  001D  GETMBR	R5	R5	K7
      0x7C0C0400,  //  001E  CALL	R3	2
      0x80000000,  //  001F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_value_ofs_y
********************************************************************/
be_local_closure(lvh_obj_get_value_ofs_y,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(_lv_label),
    /* K1   */  be_nested_str_weak(get_y),
    }),
    be_str_weak(get_value_ofs_y),
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
** Solidified function: set_text_rule_formula
********************************************************************/
be_local_closure(lvh_obj_set_text_rule_formula,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(_text_rule_formula),
    /* K1   */  be_nested_str_weak(return_X20_X2F_X20val_X20_X2D_X3E_X20_X28),
    /* K2   */  be_nested_str_weak(_X29),
    /* K3   */  be_nested_str_weak(_text_rule_function),
    /* K4   */  be_nested_str_weak(HSP_X3A_X20failed_X20to_X20compile_X20_X27_X25s_X27_X20_X2D_X20_X25s_X20_X28_X25s_X29),
    }),
    be_str_weak(set_text_rule_formula),
    &be_const_str_solidified,
    ( &(const binstruction[29]) {  /* code */
      0x60080008,  //  0000  GETGBL	R2	G8
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x7C080200,  //  0002  CALL	R2	1
      0x90020002,  //  0003  SETMBR	R0	K0	R2
      0x88080100,  //  0004  GETMBR	R2	R0	K0
      0x000A0202,  //  0005  ADD	R2	K1	R2
      0x00080502,  //  0006  ADD	R2	R2	K2
      0xA8020007,  //  0007  EXBLK	0	#0010
      0x600C000D,  //  0008  GETGBL	R3	G13
      0x5C100400,  //  0009  MOVE	R4	R2
      0x7C0C0200,  //  000A  CALL	R3	1
      0x5C100600,  //  000B  MOVE	R4	R3
      0x7C100000,  //  000C  CALL	R4	0
      0x90020604,  //  000D  SETMBR	R0	K3	R4
      0xA8040001,  //  000E  EXBLK	1	1
      0x7002000B,  //  000F  JMP		#001C
      0xAC0C0002,  //  0010  CATCH	R3	0	2
      0x70020008,  //  0011  JMP		#001B
      0x60140001,  //  0012  GETGBL	R5	G1
      0x60180018,  //  0013  GETGBL	R6	G24
      0x581C0004,  //  0014  LDCONST	R7	K4
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
** Solidified function: register_event_cb
********************************************************************/
be_local_closure(lvh_obj_register_event_cb,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(_page),
    /* K1   */  be_nested_str_weak(_oh),
    /* K2   */  be_nested_str_weak(_event_map),
    /* K3   */  be_nested_str_weak(keys),
    /* K4   */  be_nested_str_weak(register_event),
    /* K5   */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(register_event_cb),
    &be_const_str_solidified,
    ( &(const binstruction[19]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x88040301,  //  0001  GETMBR	R1	R1	K1
      0x60080010,  //  0002  GETGBL	R2	G16
      0x880C0102,  //  0003  GETMBR	R3	R0	K2
      0x8C0C0703,  //  0004  GETMET	R3	R3	K3
      0x7C0C0200,  //  0005  CALL	R3	1
      0x7C080200,  //  0006  CALL	R2	1
      0xA8020006,  //  0007  EXBLK	0	#000F
      0x5C0C0400,  //  0008  MOVE	R3	R2
      0x7C0C0000,  //  0009  CALL	R3	0
      0x8C100304,  //  000A  GETMET	R4	R1	K4
      0x5C180000,  //  000B  MOVE	R6	R0
      0x5C1C0600,  //  000C  MOVE	R7	R3
      0x7C100600,  //  000D  CALL	R4	3
      0x7001FFF8,  //  000E  JMP		#0008
      0x58080005,  //  000F  LDCONST	R2	K5
      0xAC080200,  //  0010  CATCH	R2	1	0
      0xB0080000,  //  0011  RAISE	2	R0	R0
      0x80000000,  //  0012  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: digits_to_style
********************************************************************/
be_local_closure(lvh_obj_digits_to_style,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(_digit2part),
    /* K2   */  be_nested_str_weak(_digit2state),
    /* K3   */  be_nested_str_weak(invalid_X20style_X20suffix_X20_X2502i),
    /* K4   */  be_nested_str_weak(value_error),
    }),
    be_str_weak(digits_to_style),
    &be_const_str_solidified,
    ( &(const binstruction[44]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0000,  //  0002  JMPF	R2	#0004
      0x80060000,  //  0003  RET	1	K0
      0x540A0009,  //  0004  LDINT	R2	10
      0x0C080202,  //  0005  DIV	R2	R1	R2
      0x540E0009,  //  0006  LDINT	R3	10
      0x10080403,  //  0007  MOD	R2	R2	R3
      0x540E0009,  //  0008  LDINT	R3	10
      0x100C0203,  //  0009  MOD	R3	R1	R3
      0x58100000,  //  000A  LDCONST	R4	K0
      0x28140500,  //  000B  GE	R5	R2	K0
      0x78160008,  //  000C  JMPF	R5	#0016
      0x6014000C,  //  000D  GETGBL	R5	G12
      0x88180101,  //  000E  GETMBR	R6	R0	K1
      0x7C140200,  //  000F  CALL	R5	1
      0x14140405,  //  0010  LT	R5	R2	R5
      0x78160003,  //  0011  JMPF	R5	#0016
      0x88140101,  //  0012  GETMBR	R5	R0	K1
      0x94140A02,  //  0013  GETIDX	R5	R5	R2
      0x30100805,  //  0014  OR	R4	R4	R5
      0x70020000,  //  0015  JMP		#0017
      0x4C100000,  //  0016  LDNIL	R4
      0x28140700,  //  0017  GE	R5	R3	K0
      0x78160008,  //  0018  JMPF	R5	#0022
      0x6014000C,  //  0019  GETGBL	R5	G12
      0x88180102,  //  001A  GETMBR	R6	R0	K2
      0x7C140200,  //  001B  CALL	R5	1
      0x14140605,  //  001C  LT	R5	R3	R5
      0x78160003,  //  001D  JMPF	R5	#0022
      0x88140102,  //  001E  GETMBR	R5	R0	K2
      0x94140A03,  //  001F  GETIDX	R5	R5	R3
      0x30100805,  //  0020  OR	R4	R4	R5
      0x70020000,  //  0021  JMP		#0023
      0x4C100000,  //  0022  LDNIL	R4
      0x4C140000,  //  0023  LDNIL	R5
      0x1C140805,  //  0024  EQ	R5	R4	R5
      0x78160004,  //  0025  JMPF	R5	#002B
      0x60140018,  //  0026  GETGBL	R5	G24
      0x58180003,  //  0027  LDCONST	R6	K3
      0x5C1C0200,  //  0028  MOVE	R7	R1
      0x7C140400,  //  0029  CALL	R5	2
      0xB0060805,  //  002A  RAISE	1	K4	R5
      0x80040800,  //  002B  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_value_color
********************************************************************/
be_local_closure(lvh_obj_get_value_color,   /* name */
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
    /* K0   */  be_nested_str_weak(get_value_color),
    }),
    be_str_weak(get_value_color),
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
** Solidified function: get_text_color
********************************************************************/
be_local_closure(lvh_obj_get_text_color,   /* name */
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
    /* K0   */  be_nested_str_weak(_lv_obj),
    /* K1   */  be_nested_str_weak(get_style_text_color),
    }),
    be_str_weak(get_text_color),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x80040400,  //  0004  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_val
********************************************************************/
be_local_closure(lvh_obj_set_val,   /* name */
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
    /* K0   */  be_nested_str_weak(_lv_obj),
    /* K1   */  be_nested_str_weak(set_value),
    }),
    be_str_weak(set_val),
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
** Solidified function: setmember
********************************************************************/
be_local_closure(lvh_obj_setmember,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[22]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(introspect),
    /* K2   */  be_const_int(0),
    /* K3   */  be_const_int(3),
    /* K4   */  be_nested_str_weak(set_),
    /* K5   */  be_nested_str_weak(get_),
    /* K6   */  be_nested_str_weak(byte),
    /* K7   */  be_const_int(2147483647),
    /* K8   */  be_nested_str_weak(digits_to_style),
    /* K9   */  be_nested_str_weak(_attr_ignore),
    /* K10  */  be_nested_str_weak(find),
    /* K11  */  be_nested_str_weak(get),
    /* K12  */  be_nested_str_weak(function),
    /* K13  */  be_nested_str_weak(_attr_map),
    /* K14  */  be_nested_str_weak(contains),
    /* K15  */  be_nested_str_weak(_lv_obj),
    /* K16  */  be_nested_str_weak(is_color_attribute),
    /* K17  */  be_nested_str_weak(parse_color),
    /* K18  */  be_nested_str_weak(style_),
    /* K19  */  be_nested_str_weak(_X20for_X20),
    /* K20  */  be_nested_str_weak(HSP_X3A_X20Could_X20not_X20find_X20function_X20set_),
    /* K21  */  be_nested_str_weak(HSP_X3A_X20unknown_X20attribute_X3A),
    }),
    be_str_weak(setmember),
    &be_const_str_solidified,
    ( &(const binstruction[134]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xA4120200,  //  0001  IMPORT	R4	K1
      0x40160503,  //  0002  CONNECT	R5	K2	K3
      0x94140205,  //  0003  GETIDX	R5	R1	R5
      0x1C180B04,  //  0004  EQ	R6	R5	K4
      0x741A0001,  //  0005  JMPT	R6	#0008
      0x1C180B05,  //  0006  EQ	R6	R5	K5
      0x781A0000,  //  0007  JMPF	R6	#0009
      0x80000C00,  //  0008  RET	0
      0x58180002,  //  0009  LDCONST	R6	K2
      0x601C000C,  //  000A  GETGBL	R7	G12
      0x5C200200,  //  000B  MOVE	R8	R1
      0x7C1C0200,  //  000C  CALL	R7	1
      0x281C0F03,  //  000D  GE	R7	R7	K3
      0x781E0021,  //  000E  JMPF	R7	#0031
      0x8C1C0706,  //  000F  GETMET	R7	R3	K6
      0x5425FFFE,  //  0010  LDINT	R9	-1
      0x94240209,  //  0011  GETIDX	R9	R1	R9
      0x7C1C0400,  //  0012  CALL	R7	2
      0x8C200706,  //  0013  GETMET	R8	R3	K6
      0x5429FFFD,  //  0014  LDINT	R10	-2
      0x9428020A,  //  0015  GETIDX	R10	R1	R10
      0x7C200400,  //  0016  CALL	R8	2
      0x4C240000,  //  0017  LDNIL	R9
      0x542A002F,  //  0018  LDINT	R10	48
      0x28280E0A,  //  0019  GE	R10	R7	R10
      0x782A0011,  //  001A  JMPF	R10	#002D
      0x542A0038,  //  001B  LDINT	R10	57
      0x18280E0A,  //  001C  LE	R10	R7	R10
      0x782A000E,  //  001D  JMPF	R10	#002D
      0x542A002F,  //  001E  LDINT	R10	48
      0x2828100A,  //  001F  GE	R10	R8	R10
      0x782A000B,  //  0020  JMPF	R10	#002D
      0x542A0038,  //  0021  LDINT	R10	57
      0x1828100A,  //  0022  LE	R10	R8	R10
      0x782A0008,  //  0023  JMPF	R10	#002D
      0x60280009,  //  0024  GETGBL	R10	G9
      0x542DFFFD,  //  0025  LDINT	R11	-2
      0x402C1707,  //  0026  CONNECT	R11	R11	K7
      0x942C020B,  //  0027  GETIDX	R11	R1	R11
      0x7C280200,  //  0028  CALL	R10	1
      0x5C241400,  //  0029  MOVE	R9	R10
      0x5429FFFC,  //  002A  LDINT	R10	-3
      0x402A040A,  //  002B  CONNECT	R10	K2	R10
      0x9404020A,  //  002C  GETIDX	R1	R1	R10
      0x8C280108,  //  002D  GETMET	R10	R0	K8
      0x5C301200,  //  002E  MOVE	R12	R9
      0x7C280400,  //  002F  CALL	R10	2
      0x5C181400,  //  0030  MOVE	R6	R10
      0x881C0109,  //  0031  GETMBR	R7	R0	K9
      0x8C1C0F0A,  //  0032  GETMET	R7	R7	K10
      0x5C240200,  //  0033  MOVE	R9	R1
      0x7C1C0400,  //  0034  CALL	R7	2
      0x4C200000,  //  0035  LDNIL	R8
      0x201C0E08,  //  0036  NE	R7	R7	R8
      0x781E0000,  //  0037  JMPF	R7	#0039
      0x80000E00,  //  0038  RET	0
      0x8C1C090B,  //  0039  GETMET	R7	R4	K11
      0x5C240000,  //  003A  MOVE	R9	R0
      0x002A0801,  //  003B  ADD	R10	K4	R1
      0x7C1C0600,  //  003C  CALL	R7	3
      0x60200004,  //  003D  GETGBL	R8	G4
      0x5C240E00,  //  003E  MOVE	R9	R7
      0x7C200200,  //  003F  CALL	R8	1
      0x1C20110C,  //  0040  EQ	R8	R8	K12
      0x78220005,  //  0041  JMPF	R8	#0048
      0x5C200E00,  //  0042  MOVE	R8	R7
      0x5C240000,  //  0043  MOVE	R9	R0
      0x5C280400,  //  0044  MOVE	R10	R2
      0x5C2C0C00,  //  0045  MOVE	R11	R6
      0x7C200600,  //  0046  CALL	R8	3
      0x80001000,  //  0047  RET	0
      0x8820010D,  //  0048  GETMBR	R8	R0	K13
      0x8C20110E,  //  0049  GETMET	R8	R8	K14
      0x5C280200,  //  004A  MOVE	R10	R1
      0x7C200400,  //  004B  CALL	R8	2
      0x78220033,  //  004C  JMPF	R8	#0081
      0x8820010D,  //  004D  GETMBR	R8	R0	K13
      0x94201001,  //  004E  GETIDX	R8	R8	R1
      0x8C24090B,  //  004F  GETMET	R9	R4	K11
      0x882C010F,  //  0050  GETMBR	R11	R0	K15
      0x00320808,  //  0051  ADD	R12	K4	R8
      0x7C240600,  //  0052  CALL	R9	3
      0x5C1C1200,  //  0053  MOVE	R7	R9
      0x8C240110,  //  0054  GETMET	R9	R0	K16
      0x5C2C1000,  //  0055  MOVE	R11	R8
      0x7C240400,  //  0056  CALL	R9	2
      0x78260003,  //  0057  JMPF	R9	#005C
      0x8C240111,  //  0058  GETMET	R9	R0	K17
      0x5C2C0400,  //  0059  MOVE	R11	R2
      0x7C240400,  //  005A  CALL	R9	2
      0x5C081200,  //  005B  MOVE	R2	R9
      0x60240004,  //  005C  GETGBL	R9	G4
      0x5C280E00,  //  005D  MOVE	R10	R7
      0x7C240200,  //  005E  CALL	R9	1
      0x1C24130C,  //  005F  EQ	R9	R9	K12
      0x7826001B,  //  0060  JMPF	R9	#007D
      0xA8020011,  //  0061  EXBLK	0	#0074
      0x8C24070A,  //  0062  GETMET	R9	R3	K10
      0x5C2C1000,  //  0063  MOVE	R11	R8
      0x58300012,  //  0064  LDCONST	R12	K18
      0x7C240600,  //  0065  CALL	R9	3
      0x1C241302,  //  0066  EQ	R9	R9	K2
      0x78260005,  //  0067  JMPF	R9	#006E
      0x5C240E00,  //  0068  MOVE	R9	R7
      0x8828010F,  //  0069  GETMBR	R10	R0	K15
      0x5C2C0400,  //  006A  MOVE	R11	R2
      0x5C300C00,  //  006B  MOVE	R12	R6
      0x7C240600,  //  006C  CALL	R9	3
      0x70020003,  //  006D  JMP		#0072
      0x5C240E00,  //  006E  MOVE	R9	R7
      0x8828010F,  //  006F  GETMBR	R10	R0	K15
      0x5C2C0400,  //  0070  MOVE	R11	R2
      0x7C240400,  //  0071  CALL	R9	2
      0xA8040001,  //  0072  EXBLK	1	1
      0x70020006,  //  0073  JMP		#007B
      0xAC240002,  //  0074  CATCH	R9	0	2
      0x70020003,  //  0075  JMP		#007A
      0x002C1513,  //  0076  ADD	R11	R10	K19
      0x002C1601,  //  0077  ADD	R11	R11	R1
      0xB004120B,  //  0078  RAISE	1	R9	R11
      0x70020000,  //  0079  JMP		#007B
      0xB0080000,  //  007A  RAISE	2	R0	R0
      0x80001200,  //  007B  RET	0
      0x70020002,  //  007C  JMP		#0080
      0x60240001,  //  007D  GETGBL	R9	G1
      0x002A2808,  //  007E  ADD	R10	K20	R8
      0x7C240200,  //  007F  CALL	R9	1
      0x70020003,  //  0080  JMP		#0085
      0x60200001,  //  0081  GETGBL	R8	G1
      0x58240015,  //  0082  LDCONST	R9	K21
      0x5C280200,  //  0083  MOVE	R10	R1
      0x7C200400,  //  0084  CALL	R8	2
      0x80000000,  //  0085  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: check_label
********************************************************************/
be_local_closure(lvh_obj_check_label,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(_lv_label),
    /* K1   */  be_nested_str_weak(lv),
    /* K2   */  be_nested_str_weak(label),
    /* K3   */  be_nested_str_weak(get_obj),
    /* K4   */  be_nested_str_weak(set_align),
    /* K5   */  be_nested_str_weak(ALIGN_CENTER),
    }),
    be_str_weak(check_label),
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x4C080000,  //  0001  LDNIL	R2
      0x1C040202,  //  0002  EQ	R1	R1	R2
      0x7806000A,  //  0003  JMPF	R1	#000F
      0xB8060200,  //  0004  GETNGBL	R1	K1
      0x8C040302,  //  0005  GETMET	R1	R1	K2
      0x8C0C0103,  //  0006  GETMET	R3	R0	K3
      0x7C0C0200,  //  0007  CALL	R3	1
      0x7C040400,  //  0008  CALL	R1	2
      0x90020001,  //  0009  SETMBR	R0	K0	R1
      0x88040100,  //  000A  GETMBR	R1	R0	K0
      0x8C040304,  //  000B  GETMET	R1	R1	K4
      0xB80E0200,  //  000C  GETNGBL	R3	K1
      0x880C0705,  //  000D  GETMBR	R3	R3	K5
      0x7C040400,  //  000E  CALL	R1	2
      0x80000000,  //  000F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_mode
********************************************************************/
be_local_closure(lvh_obj_get_mode,   /* name */
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
    be_str_weak(get_mode),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_enabled
********************************************************************/
be_local_closure(lvh_obj_set_enabled,   /* name */
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
    /* K0   */  be_nested_str_weak(_lv_obj),
    /* K1   */  be_nested_str_weak(clear_state),
    /* K2   */  be_nested_str_weak(lv),
    /* K3   */  be_nested_str_weak(STATE_DISABLED),
    /* K4   */  be_nested_str_weak(add_state),
    }),
    be_str_weak(set_enabled),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x78060005,  //  0000  JMPF	R1	#0007
      0x88080100,  //  0001  GETMBR	R2	R0	K0
      0x8C080501,  //  0002  GETMET	R2	R2	K1
      0xB8120400,  //  0003  GETNGBL	R4	K2
      0x88100903,  //  0004  GETMBR	R4	R4	K3
      0x7C080400,  //  0005  CALL	R2	2
      0x70020004,  //  0006  JMP		#000C
      0x88080100,  //  0007  GETMBR	R2	R0	K0
      0x8C080504,  //  0008  GETMET	R2	R2	K4
      0xB8120400,  //  0009  GETNGBL	R4	K2
      0x88100903,  //  000A  GETMBR	R4	R4	K3
      0x7C080400,  //  000B  CALL	R2	2
      0x80000000,  //  000C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_pad_left2
********************************************************************/
be_local_closure(lvh_obj_set_pad_left2,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(_lv_part2_selector),
    /* K1   */  be_nested_str_weak(_lv_obj),
    /* K2   */  be_nested_str_weak(set_style_pad_left),
    /* K3   */  be_nested_str_weak(lv),
    /* K4   */  be_nested_str_weak(STATE_DEFAULT),
    }),
    be_str_weak(set_pad_left2),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x4C0C0000,  //  0001  LDNIL	R3
      0x20080403,  //  0002  NE	R2	R2	R3
      0x780A0009,  //  0003  JMPF	R2	#000E
      0x88080101,  //  0004  GETMBR	R2	R0	K1
      0x8C080502,  //  0005  GETMET	R2	R2	K2
      0x60100009,  //  0006  GETGBL	R4	G9
      0x5C140200,  //  0007  MOVE	R5	R1
      0x7C100200,  //  0008  CALL	R4	1
      0x88140100,  //  0009  GETMBR	R5	R0	K0
      0xB81A0600,  //  000A  GETNGBL	R6	K3
      0x88180D04,  //  000B  GETMBR	R6	R6	K4
      0x30140A06,  //  000C  OR	R5	R5	R6
      0x7C080600,  //  000D  CALL	R2	3
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_pad_bottom
********************************************************************/
be_local_closure(lvh_obj_get_pad_bottom,   /* name */
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
    /* K0   */  be_nested_str_weak(_lv_part2_selector),
    /* K1   */  be_nested_str_weak(_lv_obj),
    /* K2   */  be_nested_str_weak(get_style_pad_bottom),
    /* K3   */  be_nested_str_weak(lv),
    /* K4   */  be_nested_str_weak(STATE_DEFAULT),
    }),
    be_str_weak(get_pad_bottom),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x4C080000,  //  0001  LDNIL	R2
      0x20040202,  //  0002  NE	R1	R1	R2
      0x78060007,  //  0003  JMPF	R1	#000C
      0x88040101,  //  0004  GETMBR	R1	R0	K1
      0x8C040302,  //  0005  GETMET	R1	R1	K2
      0x880C0100,  //  0006  GETMBR	R3	R0	K0
      0xB8120600,  //  0007  GETNGBL	R4	K3
      0x88100904,  //  0008  GETMBR	R4	R4	K4
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
be_local_closure(lvh_obj_member,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[19]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(introspect),
    /* K2   */  be_const_int(0),
    /* K3   */  be_const_int(3),
    /* K4   */  be_nested_str_weak(set_),
    /* K5   */  be_nested_str_weak(get_),
    /* K6   */  be_nested_str_weak(byte),
    /* K7   */  be_const_int(2147483647),
    /* K8   */  be_nested_str_weak(digits_to_style),
    /* K9   */  be_nested_str_weak(_attr_ignore),
    /* K10  */  be_nested_str_weak(find),
    /* K11  */  be_nested_str_weak(get),
    /* K12  */  be_nested_str_weak(function),
    /* K13  */  be_nested_str_weak(_attr_map),
    /* K14  */  be_nested_str_weak(contains),
    /* K15  */  be_nested_str_weak(_lv_obj),
    /* K16  */  be_nested_str_weak(style_),
    /* K17  */  be_nested_str_weak(unknown_X20attribute_X20),
    /* K18  */  be_nested_str_weak(value_error),
    }),
    be_str_weak(member),
    &be_const_str_solidified,
    ( &(const binstruction[110]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0xA40E0200,  //  0001  IMPORT	R3	K1
      0x40120503,  //  0002  CONNECT	R4	K2	K3
      0x94100204,  //  0003  GETIDX	R4	R1	R4
      0x1C140904,  //  0004  EQ	R5	R4	K4
      0x74160001,  //  0005  JMPT	R5	#0008
      0x1C140905,  //  0006  EQ	R5	R4	K5
      0x78160000,  //  0007  JMPF	R5	#0009
      0x80000A00,  //  0008  RET	0
      0x58140002,  //  0009  LDCONST	R5	K2
      0x6018000C,  //  000A  GETGBL	R6	G12
      0x5C1C0200,  //  000B  MOVE	R7	R1
      0x7C180200,  //  000C  CALL	R6	1
      0x28180D03,  //  000D  GE	R6	R6	K3
      0x781A0021,  //  000E  JMPF	R6	#0031
      0x8C180506,  //  000F  GETMET	R6	R2	K6
      0x5421FFFE,  //  0010  LDINT	R8	-1
      0x94200208,  //  0011  GETIDX	R8	R1	R8
      0x7C180400,  //  0012  CALL	R6	2
      0x8C1C0506,  //  0013  GETMET	R7	R2	K6
      0x5425FFFD,  //  0014  LDINT	R9	-2
      0x94240209,  //  0015  GETIDX	R9	R1	R9
      0x7C1C0400,  //  0016  CALL	R7	2
      0x4C200000,  //  0017  LDNIL	R8
      0x5426002F,  //  0018  LDINT	R9	48
      0x28240C09,  //  0019  GE	R9	R6	R9
      0x78260011,  //  001A  JMPF	R9	#002D
      0x54260038,  //  001B  LDINT	R9	57
      0x18240C09,  //  001C  LE	R9	R6	R9
      0x7826000E,  //  001D  JMPF	R9	#002D
      0x5426002F,  //  001E  LDINT	R9	48
      0x28240E09,  //  001F  GE	R9	R7	R9
      0x7826000B,  //  0020  JMPF	R9	#002D
      0x54260038,  //  0021  LDINT	R9	57
      0x18240E09,  //  0022  LE	R9	R7	R9
      0x78260008,  //  0023  JMPF	R9	#002D
      0x60240009,  //  0024  GETGBL	R9	G9
      0x5429FFFD,  //  0025  LDINT	R10	-2
      0x40281507,  //  0026  CONNECT	R10	R10	K7
      0x9428020A,  //  0027  GETIDX	R10	R1	R10
      0x7C240200,  //  0028  CALL	R9	1
      0x5C201200,  //  0029  MOVE	R8	R9
      0x5425FFFC,  //  002A  LDINT	R9	-3
      0x40260409,  //  002B  CONNECT	R9	K2	R9
      0x94040209,  //  002C  GETIDX	R1	R1	R9
      0x8C240108,  //  002D  GETMET	R9	R0	K8
      0x5C2C1000,  //  002E  MOVE	R11	R8
      0x7C240400,  //  002F  CALL	R9	2
      0x5C141200,  //  0030  MOVE	R5	R9
      0x88180109,  //  0031  GETMBR	R6	R0	K9
      0x8C180D0A,  //  0032  GETMET	R6	R6	K10
      0x5C200200,  //  0033  MOVE	R8	R1
      0x7C180400,  //  0034  CALL	R6	2
      0x4C1C0000,  //  0035  LDNIL	R7
      0x20180C07,  //  0036  NE	R6	R6	R7
      0x781A0000,  //  0037  JMPF	R6	#0039
      0x80000C00,  //  0038  RET	0
      0x8C18070B,  //  0039  GETMET	R6	R3	K11
      0x5C200000,  //  003A  MOVE	R8	R0
      0x00260A01,  //  003B  ADD	R9	K5	R1
      0x7C180600,  //  003C  CALL	R6	3
      0x601C0004,  //  003D  GETGBL	R7	G4
      0x5C200C00,  //  003E  MOVE	R8	R6
      0x7C1C0200,  //  003F  CALL	R7	1
      0x1C1C0F0C,  //  0040  EQ	R7	R7	K12
      0x781E0004,  //  0041  JMPF	R7	#0047
      0x5C1C0C00,  //  0042  MOVE	R7	R6
      0x5C200000,  //  0043  MOVE	R8	R0
      0x5C240A00,  //  0044  MOVE	R9	R5
      0x7C1C0400,  //  0045  CALL	R7	2
      0x80040E00,  //  0046  RET	1	R7
      0x881C010D,  //  0047  GETMBR	R7	R0	K13
      0x8C1C0F0E,  //  0048  GETMET	R7	R7	K14
      0x5C240200,  //  0049  MOVE	R9	R1
      0x7C1C0400,  //  004A  CALL	R7	2
      0x781E001B,  //  004B  JMPF	R7	#0068
      0x881C010D,  //  004C  GETMBR	R7	R0	K13
      0x941C0E01,  //  004D  GETIDX	R7	R7	R1
      0x8C20070B,  //  004E  GETMET	R8	R3	K11
      0x8828010F,  //  004F  GETMBR	R10	R0	K15
      0x002E0A07,  //  0050  ADD	R11	K5	R7
      0x7C200600,  //  0051  CALL	R8	3
      0x5C181000,  //  0052  MOVE	R6	R8
      0x60200004,  //  0053  GETGBL	R8	G4
      0x5C240C00,  //  0054  MOVE	R9	R6
      0x7C200200,  //  0055  CALL	R8	1
      0x1C20110C,  //  0056  EQ	R8	R8	K12
      0x7822000F,  //  0057  JMPF	R8	#0068
      0x8C20050A,  //  0058  GETMET	R8	R2	K10
      0x5C280E00,  //  0059  MOVE	R10	R7
      0x582C0010,  //  005A  LDCONST	R11	K16
      0x7C200600,  //  005B  CALL	R8	3
      0x1C201102,  //  005C  EQ	R8	R8	K2
      0x78220005,  //  005D  JMPF	R8	#0064
      0x5C200C00,  //  005E  MOVE	R8	R6
      0x8824010F,  //  005F  GETMBR	R9	R0	K15
      0x5C280A00,  //  0060  MOVE	R10	R5
      0x7C200400,  //  0061  CALL	R8	2
      0x80041000,  //  0062  RET	1	R8
      0x70020003,  //  0063  JMP		#0068
      0x5C200C00,  //  0064  MOVE	R8	R6
      0x8824010F,  //  0065  GETMBR	R9	R0	K15
      0x7C200200,  //  0066  CALL	R8	1
      0x80041000,  //  0067  RET	1	R8
      0x601C0008,  //  0068  GETGBL	R7	G8
      0x5C200200,  //  0069  MOVE	R8	R1
      0x7C1C0200,  //  006A  CALL	R7	1
      0x001E2207,  //  006B  ADD	R7	K17	R7
      0xB0062407,  //  006C  RAISE	1	K18	R7
      0x80000000,  //  006D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_line_width
********************************************************************/
be_local_closure(lvh_obj_set_line_width,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(_lv_obj),
    /* K1   */  be_nested_str_weak(set_style_line_width),
    }),
    be_str_weak(set_line_width),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x8C0C0701,  //  0001  GETMET	R3	R3	K1
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
** Solidified function: get_text_rule_format
********************************************************************/
be_local_closure(lvh_obj_get_text_rule_format,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(_text_rule_format),
    }),
    be_str_weak(get_text_rule_format),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_text
********************************************************************/
be_local_closure(lvh_obj_set_text,   /* name */
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
    /* K0   */  be_nested_str_weak(check_label),
    /* K1   */  be_nested_str_weak(_lv_label),
    /* K2   */  be_nested_str_weak(set_text),
    }),
    be_str_weak(set_text),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x7C080200,  //  0001  CALL	R2	1
      0x88080101,  //  0002  GETMBR	R2	R0	K1
      0x8C080502,  //  0003  GETMET	R2	R2	K2
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
** Solidified function: parse_color
********************************************************************/
be_local_closure(lvh_obj_parse_color,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    1,                          /* argc */
    4,                          /* varg */
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
        ( &(const bvalue[13]) {     /* constants */
        /* K0   */  be_nested_str_weak(string),
        /* K1   */  be_nested_str_weak(toupper),
        /* K2   */  be_const_int(0),
        /* K3   */  be_const_int(1),
        /* K4   */  be_nested_str_weak(_X23),
        /* K5   */  be_nested_str_weak(x),
        /* K6   */  be_nested_str_weak(X),
        /* K7   */  be_nested_str_weak(A),
        /* K8   */  be_nested_str_weak(F),
        /* K9   */  be_nested_str_weak(byte),
        /* K10  */  be_nested_str_weak(0),
        /* K11  */  be_nested_str_weak(9),
        /* K12  */  be_nested_str_weak(stop_iteration),
        }),
        be_str_weak(parse_hex),
        &be_const_str_solidified,
        ( &(const binstruction[57]) {  /* code */
          0xA4060000,  //  0000  IMPORT	R1	K0
          0x8C080301,  //  0001  GETMET	R2	R1	K1
          0x5C100000,  //  0002  MOVE	R4	R0
          0x7C080400,  //  0003  CALL	R2	2
          0x5C000400,  //  0004  MOVE	R0	R2
          0x58080002,  //  0005  LDCONST	R2	K2
          0x600C0010,  //  0006  GETGBL	R3	G16
          0x6010000C,  //  0007  GETGBL	R4	G12
          0x5C140000,  //  0008  MOVE	R5	R0
          0x7C100200,  //  0009  CALL	R4	1
          0x04100903,  //  000A  SUB	R4	R4	K3
          0x40120404,  //  000B  CONNECT	R4	K2	R4
          0x7C0C0200,  //  000C  CALL	R3	1
          0xA8020026,  //  000D  EXBLK	0	#0035
          0x5C100600,  //  000E  MOVE	R4	R3
          0x7C100000,  //  000F  CALL	R4	0
          0x94140004,  //  0010  GETIDX	R5	R0	R4
          0x1C180B04,  //  0011  EQ	R6	R5	K4
          0x781A0000,  //  0012  JMPF	R6	#0014
          0x7001FFF9,  //  0013  JMP		#000E
          0x1C180B05,  //  0014  EQ	R6	R5	K5
          0x741A0001,  //  0015  JMPT	R6	#0018
          0x1C180B06,  //  0016  EQ	R6	R5	K6
          0x781A0000,  //  0017  JMPF	R6	#0019
          0x7001FFF4,  //  0018  JMP		#000E
          0x28180B07,  //  0019  GE	R6	R5	K7
          0x781A000B,  //  001A  JMPF	R6	#0027
          0x18180B08,  //  001B  LE	R6	R5	K8
          0x781A0009,  //  001C  JMPF	R6	#0027
          0x541A0003,  //  001D  LDINT	R6	4
          0x38180406,  //  001E  SHL	R6	R2	R6
          0x8C1C0309,  //  001F  GETMET	R7	R1	K9
          0x5C240A00,  //  0020  MOVE	R9	R5
          0x7C1C0400,  //  0021  CALL	R7	2
          0x54220036,  //  0022  LDINT	R8	55
          0x041C0E08,  //  0023  SUB	R7	R7	R8
          0x30180C07,  //  0024  OR	R6	R6	R7
          0x5C080C00,  //  0025  MOVE	R2	R6
          0x7002000C,  //  0026  JMP		#0034
          0x28180B0A,  //  0027  GE	R6	R5	K10
          0x781A000A,  //  0028  JMPF	R6	#0034
          0x18180B0B,  //  0029  LE	R6	R5	K11
          0x781A0008,  //  002A  JMPF	R6	#0034
          0x541A0003,  //  002B  LDINT	R6	4
          0x38180406,  //  002C  SHL	R6	R2	R6
          0x8C1C0309,  //  002D  GETMET	R7	R1	K9
          0x5C240A00,  //  002E  MOVE	R9	R5
          0x7C1C0400,  //  002F  CALL	R7	2
          0x5422002F,  //  0030  LDINT	R8	48
          0x041C0E08,  //  0031  SUB	R7	R7	R8
          0x30180C07,  //  0032  OR	R6	R6	R7
          0x5C080C00,  //  0033  MOVE	R2	R6
          0x7001FFD8,  //  0034  JMP		#000E
          0x580C000C,  //  0035  LDCONST	R3	K12
          0xAC0C0200,  //  0036  CATCH	R3	1	0
          0xB0080000,  //  0037  RAISE	2	R0	R0
          0x80040400,  //  0038  RET	1	R2
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[10]) {     /* constants */
    /* K0   */  be_const_class(be_class_lvh_obj),
    /* K1   */  be_const_int(0),
    /* K2   */  be_nested_str_weak(_X23),
    /* K3   */  be_nested_str_weak(lv),
    /* K4   */  be_nested_str_weak(color),
    /* K5   */  be_nested_str_weak(string),
    /* K6   */  be_nested_str_weak(introspect),
    /* K7   */  be_nested_str_weak(COLOR_),
    /* K8   */  be_nested_str_weak(toupper),
    /* K9   */  be_nested_str_weak(get),
    }),
    be_str_weak(parse_color),
    &be_const_str_solidified,
    ( &(const binstruction[40]) {  /* code */
      0x58040000,  //  0000  LDCONST	R1	K0
      0x84080000,  //  0001  CLOSURE	R2	P0
      0x600C0008,  //  0002  GETGBL	R3	G8
      0x5C100000,  //  0003  MOVE	R4	R0
      0x7C0C0200,  //  0004  CALL	R3	1
      0x5C000600,  //  0005  MOVE	R0	R3
      0x940C0101,  //  0006  GETIDX	R3	R0	K1
      0x1C0C0702,  //  0007  EQ	R3	R3	K2
      0x780E0007,  //  0008  JMPF	R3	#0011
      0xB80E0600,  //  0009  GETNGBL	R3	K3
      0x8C0C0704,  //  000A  GETMET	R3	R3	K4
      0x5C140400,  //  000B  MOVE	R5	R2
      0x5C180000,  //  000C  MOVE	R6	R0
      0x7C140200,  //  000D  CALL	R5	1
      0x7C0C0400,  //  000E  CALL	R3	2
      0x80040600,  //  000F  RET	1	R3
      0x70020011,  //  0010  JMP		#0023
      0xA40E0A00,  //  0011  IMPORT	R3	K5
      0xA4120C00,  //  0012  IMPORT	R4	K6
      0x8C140708,  //  0013  GETMET	R5	R3	K8
      0x5C1C0000,  //  0014  MOVE	R7	R0
      0x7C140400,  //  0015  CALL	R5	2
      0x00160E05,  //  0016  ADD	R5	K7	R5
      0x8C180909,  //  0017  GETMET	R6	R4	K9
      0xB8220600,  //  0018  GETNGBL	R8	K3
      0x5C240A00,  //  0019  MOVE	R9	R5
      0x7C180600,  //  001A  CALL	R6	3
      0x4C1C0000,  //  001B  LDNIL	R7
      0x201C0C07,  //  001C  NE	R7	R6	R7
      0x781E0004,  //  001D  JMPF	R7	#0023
      0xB81E0600,  //  001E  GETNGBL	R7	K3
      0x8C1C0F04,  //  001F  GETMET	R7	R7	K4
      0x5C240C00,  //  0020  MOVE	R9	R6
      0x7C1C0400,  //  0021  CALL	R7	2
      0x80040E00,  //  0022  RET	1	R7
      0xB80E0600,  //  0023  GETNGBL	R3	K3
      0x8C0C0704,  //  0024  GETMET	R3	R3	K4
      0x58140001,  //  0025  LDCONST	R5	K1
      0x7C0C0400,  //  0026  CALL	R3	2
      0x80040600,  //  0027  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_val
********************************************************************/
be_local_closure(lvh_obj_get_val,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(_lv_obj),
    /* K1   */  be_nested_str_weak(get_value),
    }),
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
** Solidified function: get_pad_right
********************************************************************/
be_local_closure(lvh_obj_get_pad_right,   /* name */
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
    /* K0   */  be_nested_str_weak(_lv_part2_selector),
    /* K1   */  be_nested_str_weak(_lv_obj),
    /* K2   */  be_nested_str_weak(get_style_pad_right),
    /* K3   */  be_nested_str_weak(lv),
    /* K4   */  be_nested_str_weak(STATE_DEFAULT),
    }),
    be_str_weak(get_pad_right),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x4C080000,  //  0001  LDNIL	R2
      0x20040202,  //  0002  NE	R1	R1	R2
      0x78060007,  //  0003  JMPF	R1	#000C
      0x88040101,  //  0004  GETMBR	R1	R0	K1
      0x8C040302,  //  0005  GETMET	R1	R1	K2
      0x880C0100,  //  0006  GETMBR	R3	R0	K0
      0xB8120600,  //  0007  GETNGBL	R4	K3
      0x88100904,  //  0008  GETMBR	R4	R4	K4
      0x300C0604,  //  0009  OR	R3	R3	R4
      0x7C040400,  //  000A  CALL	R1	2
      0x80040200,  //  000B  RET	1	R1
      0x80000000,  //  000C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_value_font
********************************************************************/
be_local_closure(lvh_obj_get_value_font,   /* name */
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
    /* K0   */  be_nested_str_weak(get_text_font),
    }),
    be_str_weak(get_value_font),
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
** Solidified function: get_toggle
********************************************************************/
be_local_closure(lvh_obj_get_toggle,   /* name */
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
    /* K0   */  be_nested_str_weak(_lv_obj),
    /* K1   */  be_nested_str_weak(has_state),
    /* K2   */  be_nested_str_weak(lv),
    /* K3   */  be_nested_str_weak(STATE_CHECKED),
    }),
    be_str_weak(get_toggle),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0xB80E0400,  //  0002  GETNGBL	R3	K2
      0x880C0703,  //  0003  GETMBR	R3	R3	K3
      0x7C040400,  //  0004  CALL	R1	2
      0x80040200,  //  0005  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_meta
********************************************************************/
be_local_closure(lvh_obj_set_meta,   /* name */
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
    /* K0   */  be_nested_str_weak(_meta),
    }),
    be_str_weak(set_meta),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x80000000,  //  0001  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_text_rule_format
********************************************************************/
be_local_closure(lvh_obj_set_text_rule_format,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(_text_rule_format),
    }),
    be_str_weak(set_text_rule_format),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x60080008,  //  0000  GETGBL	R2	G8
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x7C080200,  //  0002  CALL	R2	1
      0x90020002,  //  0003  SETMBR	R0	K0	R2
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_value_ofs_x
********************************************************************/
be_local_closure(lvh_obj_get_value_ofs_x,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(_lv_label),
    /* K1   */  be_nested_str_weak(get_x),
    }),
    be_str_weak(get_value_ofs_x),
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
** Solidified function: is_color_attribute
********************************************************************/
be_local_closure(lvh_obj_is_color_attribute,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    4,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_const_class(be_class_lvh_obj),
    /* K1   */  be_nested_str_weak(re),
    /* K2   */  be_nested_str_weak(search),
    /* K3   */  be_nested_str_weak(color_X24),
    }),
    be_str_weak(is_color_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x58040000,  //  0000  LDCONST	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0x600C0017,  //  0002  GETGBL	R3	G23
      0x8C100502,  //  0003  GETMET	R4	R2	K2
      0x58180003,  //  0004  LDCONST	R6	K3
      0x601C0008,  //  0005  GETGBL	R7	G8
      0x5C200000,  //  0006  MOVE	R8	R0
      0x7C1C0200,  //  0007  CALL	R7	1
      0x7C100600,  //  0008  CALL	R4	3
      0x7C0C0200,  //  0009  CALL	R3	1
      0x80040600,  //  000A  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_text_rule_formula
********************************************************************/
be_local_closure(lvh_obj_get_text_rule_formula,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(_text_rule_formula),
    }),
    be_str_weak(get_text_rule_formula),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_value_color
********************************************************************/
be_local_closure(lvh_obj_set_value_color,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(set_text_color),
    }),
    be_str_weak(set_value_color),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x5C100200,  //  0001  MOVE	R4	R1
      0x7C080400,  //  0002  CALL	R2	2
      0x80000000,  //  0003  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_pad_top
********************************************************************/
be_local_closure(lvh_obj_get_pad_top,   /* name */
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
    /* K0   */  be_nested_str_weak(_lv_part2_selector),
    /* K1   */  be_nested_str_weak(_lv_obj),
    /* K2   */  be_nested_str_weak(get_style_pad_top),
    /* K3   */  be_nested_str_weak(lv),
    /* K4   */  be_nested_str_weak(STATE_DEFAULT),
    }),
    be_str_weak(get_pad_top),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x4C080000,  //  0001  LDNIL	R2
      0x20040202,  //  0002  NE	R1	R1	R2
      0x78060007,  //  0003  JMPF	R1	#000C
      0x88040101,  //  0004  GETMBR	R1	R0	K1
      0x8C040302,  //  0005  GETMET	R1	R1	K2
      0x880C0100,  //  0006  GETMBR	R3	R0	K0
      0xB8120600,  //  0007  GETNGBL	R4	K3
      0x88100904,  //  0008  GETMBR	R4	R4	K4
      0x300C0604,  //  0009  OR	R3	R3	R4
      0x7C040400,  //  000A  CALL	R1	2
      0x80040200,  //  000B  RET	1	R1
      0x80000000,  //  000C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_hidden
********************************************************************/
be_local_closure(lvh_obj_get_hidden,   /* name */
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
    /* K0   */  be_nested_str_weak(_lv_obj),
    /* K1   */  be_nested_str_weak(has_flag),
    /* K2   */  be_nested_str_weak(lv),
    /* K3   */  be_nested_str_weak(OBJ_FLAG_HIDDEN),
    }),
    be_str_weak(get_hidden),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0xB80E0400,  //  0002  GETNGBL	R3	K2
      0x880C0703,  //  0003  GETMBR	R3	R3	K3
      0x7C040400,  //  0004  CALL	R1	2
      0x80040200,  //  0005  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_pad_left
********************************************************************/
be_local_closure(lvh_obj_get_pad_left,   /* name */
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
    /* K0   */  be_nested_str_weak(_lv_part2_selector),
    /* K1   */  be_nested_str_weak(_lv_obj),
    /* K2   */  be_nested_str_weak(get_style_pad_left),
    /* K3   */  be_nested_str_weak(lv),
    /* K4   */  be_nested_str_weak(STATE_DEFAULT),
    }),
    be_str_weak(get_pad_left),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x4C080000,  //  0001  LDNIL	R2
      0x20040202,  //  0002  NE	R1	R1	R2
      0x78060007,  //  0003  JMPF	R1	#000C
      0x88040101,  //  0004  GETMBR	R1	R0	K1
      0x8C040302,  //  0005  GETMET	R1	R1	K2
      0x880C0100,  //  0006  GETMBR	R3	R0	K0
      0xB8120600,  //  0007  GETNGBL	R4	K3
      0x88100904,  //  0008  GETMBR	R4	R4	K4
      0x300C0604,  //  0009  OR	R3	R3	R4
      0x7C040400,  //  000A  CALL	R1	2
      0x80040200,  //  000B  RET	1	R1
      0x80000000,  //  000C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_radius2
********************************************************************/
be_local_closure(lvh_obj_get_radius2,   /* name */
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
    /* K0   */  be_nested_str_weak(_lv_part2_selector),
    /* K1   */  be_nested_str_weak(_lv_obj),
    /* K2   */  be_nested_str_weak(get_style_radius),
    /* K3   */  be_nested_str_weak(lv),
    /* K4   */  be_nested_str_weak(STATE_DEFAULT),
    }),
    be_str_weak(get_radius2),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x4C080000,  //  0001  LDNIL	R2
      0x20040202,  //  0002  NE	R1	R1	R2
      0x78060007,  //  0003  JMPF	R1	#000C
      0x88040101,  //  0004  GETMBR	R1	R0	K1
      0x8C040302,  //  0005  GETMET	R1	R1	K2
      0x880C0100,  //  0006  GETMBR	R3	R0	K0
      0xB8120600,  //  0007  GETNGBL	R4	K3
      0x88100904,  //  0008  GETMBR	R4	R4	K4
      0x300C0604,  //  0009  OR	R3	R3	R4
      0x7C040400,  //  000A  CALL	R1	2
      0x80040200,  //  000B  RET	1	R1
      0x80000000,  //  000C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_pad_all
********************************************************************/
be_local_closure(lvh_obj_get_pad_all,   /* name */
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
    be_str_weak(get_pad_all),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_click
********************************************************************/
be_local_closure(lvh_obj_set_click,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(set_enabled),
    }),
    be_str_weak(set_click),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x5C100200,  //  0001  MOVE	R4	R1
      0x7C080400,  //  0002  CALL	R2	2
      0x80000000,  //  0003  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_pad_top2
********************************************************************/
be_local_closure(lvh_obj_set_pad_top2,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(_lv_part2_selector),
    /* K1   */  be_nested_str_weak(_lv_obj),
    /* K2   */  be_nested_str_weak(set_style_pad_top),
    /* K3   */  be_nested_str_weak(lv),
    /* K4   */  be_nested_str_weak(STATE_DEFAULT),
    }),
    be_str_weak(set_pad_top2),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x4C0C0000,  //  0001  LDNIL	R3
      0x20080403,  //  0002  NE	R2	R2	R3
      0x780A0009,  //  0003  JMPF	R2	#000E
      0x88080101,  //  0004  GETMBR	R2	R0	K1
      0x8C080502,  //  0005  GETMET	R2	R2	K2
      0x60100009,  //  0006  GETGBL	R4	G9
      0x5C140200,  //  0007  MOVE	R5	R1
      0x7C100200,  //  0008  CALL	R4	1
      0x88140100,  //  0009  GETMBR	R5	R0	K0
      0xB81A0600,  //  000A  GETNGBL	R6	K3
      0x88180D04,  //  000B  GETMBR	R6	R6	K4
      0x30140A06,  //  000C  OR	R5	R5	R6
      0x7C080600,  //  000D  CALL	R2	3
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_action
********************************************************************/
be_local_closure(lvh_obj_get_action,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(_action),
    /* K1   */  be_nested_str_weak(),
    }),
    be_str_weak(get_action),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x78060001,  //  0001  JMPF	R1	#0004
      0x5C080200,  //  0002  MOVE	R2	R1
      0x70020000,  //  0003  JMP		#0005
      0x58080001,  //  0004  LDCONST	R2	K1
      0x80040400,  //  0005  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_adjustable
********************************************************************/
be_local_closure(lvh_obj_set_adjustable,   /* name */
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
    /* K0   */  be_nested_str_weak(_lv_obj),
    /* K1   */  be_nested_str_weak(add_flag),
    /* K2   */  be_nested_str_weak(lv),
    /* K3   */  be_nested_str_weak(OBJ_FLAG_CLICKABLE),
    /* K4   */  be_nested_str_weak(clear_flag),
    }),
    be_str_weak(set_adjustable),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x78060005,  //  0000  JMPF	R1	#0007
      0x88080100,  //  0001  GETMBR	R2	R0	K0
      0x8C080501,  //  0002  GETMET	R2	R2	K1
      0xB8120400,  //  0003  GETNGBL	R4	K2
      0x88100903,  //  0004  GETMBR	R4	R4	K3
      0x7C080400,  //  0005  CALL	R2	2
      0x70020004,  //  0006  JMP		#000C
      0x88080100,  //  0007  GETMBR	R2	R0	K0
      0x8C080504,  //  0008  GETMET	R2	R2	K4
      0xB8120400,  //  0009  GETNGBL	R4	K2
      0x88100903,  //  000A  GETMBR	R4	R4	K3
      0x7C080400,  //  000B  CALL	R2	2
      0x80000000,  //  000C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_value_ofs_y
********************************************************************/
be_local_closure(lvh_obj_set_value_ofs_y,   /* name */
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
    /* K0   */  be_nested_str_weak(check_label),
    /* K1   */  be_nested_str_weak(_lv_label),
    /* K2   */  be_nested_str_weak(set_y),
    }),
    be_str_weak(set_value_ofs_y),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x7C080200,  //  0001  CALL	R2	1
      0x88080101,  //  0002  GETMBR	R2	R0	K1
      0x8C080502,  //  0003  GETMET	R2	R2	K2
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
** Solidified function: set_hidden
********************************************************************/
be_local_closure(lvh_obj_set_hidden,   /* name */
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
    /* K0   */  be_nested_str_weak(_lv_obj),
    /* K1   */  be_nested_str_weak(add_flag),
    /* K2   */  be_nested_str_weak(lv),
    /* K3   */  be_nested_str_weak(OBJ_FLAG_HIDDEN),
    /* K4   */  be_nested_str_weak(clear_flag),
    }),
    be_str_weak(set_hidden),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x78060005,  //  0000  JMPF	R1	#0007
      0x88080100,  //  0001  GETMBR	R2	R0	K0
      0x8C080501,  //  0002  GETMET	R2	R2	K1
      0xB8120400,  //  0003  GETNGBL	R4	K2
      0x88100903,  //  0004  GETMBR	R4	R4	K3
      0x7C080400,  //  0005  CALL	R2	2
      0x70020004,  //  0006  JMP		#000C
      0x88080100,  //  0007  GETMBR	R2	R0	K0
      0x8C080504,  //  0008  GETMET	R2	R2	K4
      0xB8120400,  //  0009  GETNGBL	R4	K2
      0x88100903,  //  000A  GETMBR	R4	R4	K3
      0x7C080400,  //  000B  CALL	R2	2
      0x80000000,  //  000C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_value_str
********************************************************************/
be_local_closure(lvh_obj_get_value_str,   /* name */
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
    /* K0   */  be_nested_str_weak(get_text),
    }),
    be_str_weak(get_value_str),
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
** Solidified function: get_text_font
********************************************************************/
be_local_closure(lvh_obj_get_text_font,   /* name */
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
    be_str_weak(get_text_font),
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
be_local_closure(lvh_obj_set_text_rule,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
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
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(_text_rule),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(remove_rule),
    /* K3   */  be_nested_str_weak(add_rule),
    }),
    be_str_weak(set_text_rule),
    &be_const_str_solidified,
    ( &(const binstruction[21]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x4C0C0000,  //  0001  LDNIL	R3
      0x20080403,  //  0002  NE	R2	R2	R3
      0x780A0004,  //  0003  JMPF	R2	#0009
      0xB80A0200,  //  0004  GETNGBL	R2	K1
      0x8C080502,  //  0005  GETMET	R2	R2	K2
      0x88100100,  //  0006  GETMBR	R4	R0	K0
      0x5C140000,  //  0007  MOVE	R5	R0
      0x7C080600,  //  0008  CALL	R2	3
      0x60080008,  //  0009  GETGBL	R2	G8
      0x5C0C0200,  //  000A  MOVE	R3	R1
      0x7C080200,  //  000B  CALL	R2	1
      0x90020002,  //  000C  SETMBR	R0	K0	R2
      0xB80A0200,  //  000D  GETNGBL	R2	K1
      0x8C080503,  //  000E  GETMET	R2	R2	K3
      0x88100100,  //  000F  GETMBR	R4	R0	K0
      0x84140000,  //  0010  CLOSURE	R5	P0
      0x5C180000,  //  0011  MOVE	R6	R0
      0x7C080800,  //  0012  CALL	R2	4
      0xA0000000,  //  0013  CLOSE	R0
      0x80000000,  //  0014  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_meta
********************************************************************/
be_local_closure(lvh_obj_get_meta,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(_meta),
    }),
    be_str_weak(get_meta),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_val_rule
********************************************************************/
be_local_closure(lvh_obj_get_val_rule,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(_val_rule),
    }),
    be_str_weak(get_val_rule),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_text_font
********************************************************************/
be_local_closure(lvh_obj_set_text_font,   /* name */
  be_nested_proto(
    16,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[21]) {     /* constants */
    /* K0   */  be_nested_str_weak(int),
    /* K1   */  be_nested_str_weak(lv),
    /* K2   */  be_nested_str_weak(font_embedded),
    /* K3   */  be_nested_str_weak(robotocondensed),
    /* K4   */  be_nested_str_weak(montserrat),
    /* K5   */  be_nested_str_weak(string),
    /* K6   */  be_nested_str_weak(re),
    /* K7   */  be_nested_str_weak(split),
    /* K8   */  be_nested_str_weak(_X3A),
    /* K9   */  be_const_int(1),
    /* K10  */  be_nested_str_weak(_X2D),
    /* K11  */  be_const_int(0),
    /* K12  */  be_const_int(2),
    /* K13  */  be_nested_str_weak(concat),
    /* K14  */  be_nested_str_weak(match),
    /* K15  */  be_nested_str_weak(_X2E_X2A_X5C_X2Ettf_X24),
    /* K16  */  be_nested_str_weak(load_freetype_font),
    /* K17  */  be_nested_str_weak(load_font),
    /* K18  */  be_nested_str_weak(_lv_obj),
    /* K19  */  be_nested_str_weak(set_style_text_font),
    /* K20  */  be_nested_str_weak(HSP_X3A_X20Unsupported_X20font_X3A),
    }),
    be_str_weak(set_text_font),
    &be_const_str_solidified,
    ( &(const binstruction[143]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x600C0004,  //  0001  GETGBL	R3	G4
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C0C0200,  //  0003  CALL	R3	1
      0x1C0C0700,  //  0004  EQ	R3	R3	K0
      0x780E001B,  //  0005  JMPF	R3	#0022
      0xA8020007,  //  0006  EXBLK	0	#000F
      0xB80E0200,  //  0007  GETNGBL	R3	K1
      0x8C0C0702,  //  0008  GETMET	R3	R3	K2
      0x58140003,  //  0009  LDCONST	R5	K3
      0x5C180200,  //  000A  MOVE	R6	R1
      0x7C0C0600,  //  000B  CALL	R3	3
      0x5C080600,  //  000C  MOVE	R2	R3
      0xA8040001,  //  000D  EXBLK	1	1
      0x70020011,  //  000E  JMP		#0021
      0xAC0C0000,  //  000F  CATCH	R3	0	0
      0x7002000E,  //  0010  JMP		#0020
      0xA8020007,  //  0011  EXBLK	0	#001A
      0xB80E0200,  //  0012  GETNGBL	R3	K1
      0x8C0C0702,  //  0013  GETMET	R3	R3	K2
      0x58140004,  //  0014  LDCONST	R5	K4
      0x5C180200,  //  0015  MOVE	R6	R1
      0x7C0C0600,  //  0016  CALL	R3	3
      0x5C080600,  //  0017  MOVE	R2	R3
      0xA8040001,  //  0018  EXBLK	1	1
      0x70020004,  //  0019  JMP		#001F
      0xAC0C0000,  //  001A  CATCH	R3	0	0
      0x70020001,  //  001B  JMP		#001E
      0x80000600,  //  001C  RET	0
      0x70020000,  //  001D  JMP		#001F
      0xB0080000,  //  001E  RAISE	2	R0	R0
      0x70020000,  //  001F  JMP		#0021
      0xB0080000,  //  0020  RAISE	2	R0	R0
      0x7002005E,  //  0021  JMP		#0081
      0x600C0004,  //  0022  GETGBL	R3	G4
      0x5C100200,  //  0023  MOVE	R4	R1
      0x7C0C0200,  //  0024  CALL	R3	1
      0x1C0C0705,  //  0025  EQ	R3	R3	K5
      0x780E0059,  //  0026  JMPF	R3	#0081
      0xA40E0A00,  //  0027  IMPORT	R3	K5
      0xA4120C00,  //  0028  IMPORT	R4	K6
      0x8C140707,  //  0029  GETMET	R5	R3	K7
      0x5C1C0200,  //  002A  MOVE	R7	R1
      0x58200008,  //  002B  LDCONST	R8	K8
      0x58240009,  //  002C  LDCONST	R9	K9
      0x7C140800,  //  002D  CALL	R5	4
      0x8C180707,  //  002E  GETMET	R6	R3	K7
      0x5C200200,  //  002F  MOVE	R8	R1
      0x5824000A,  //  0030  LDCONST	R9	K10
      0x7C180600,  //  0031  CALL	R6	3
      0x5C1C0200,  //  0032  MOVE	R7	R1
      0x5820000B,  //  0033  LDCONST	R8	K11
      0x50240000,  //  0034  LDBOOL	R9	0	0
      0x6028000C,  //  0035  GETGBL	R10	G12
      0x5C2C0A00,  //  0036  MOVE	R11	R5
      0x7C280200,  //  0037  CALL	R10	1
      0x24281509,  //  0038  GT	R10	R10	K9
      0x782A0003,  //  0039  JMPF	R10	#003E
      0x6028000C,  //  003A  GETGBL	R10	G12
      0x942C0B0B,  //  003B  GETIDX	R11	R5	K11
      0x7C280200,  //  003C  CALL	R10	1
      0x742A0000,  //  003D  JMPT	R10	#003F
      0x50280001,  //  003E  LDBOOL	R10	0	1
      0x50280200,  //  003F  LDBOOL	R10	1	0
      0x602C000C,  //  0040  GETGBL	R11	G12
      0x5C300C00,  //  0041  MOVE	R12	R6
      0x7C2C0200,  //  0042  CALL	R11	1
      0x282C170C,  //  0043  GE	R11	R11	K12
      0x782E000B,  //  0044  JMPF	R11	#0051
      0x602C0009,  //  0045  GETGBL	R11	G9
      0x5431FFFE,  //  0046  LDINT	R12	-1
      0x94300C0C,  //  0047  GETIDX	R12	R6	R12
      0x7C2C0200,  //  0048  CALL	R11	1
      0x5C201600,  //  0049  MOVE	R8	R11
      0x542DFFFD,  //  004A  LDINT	R11	-2
      0x402E160B,  //  004B  CONNECT	R11	K11	R11
      0x942C0C0B,  //  004C  GETIDX	R11	R6	R11
      0x8C2C170D,  //  004D  GETMET	R11	R11	K13
      0x5834000A,  //  004E  LDCONST	R13	K10
      0x7C2C0400,  //  004F  CALL	R11	2
      0x5C1C1600,  //  0050  MOVE	R7	R11
      0x8C2C090E,  //  0051  GETMET	R11	R4	K14
      0x5834000F,  //  0052  LDCONST	R13	K15
      0x5C380E00,  //  0053  MOVE	R14	R7
      0x7C2C0600,  //  0054  CALL	R11	3
      0x782E0006,  //  0055  JMPF	R11	#005D
      0x8C2C0707,  //  0056  GETMET	R11	R3	K7
      0x5C340E00,  //  0057  MOVE	R13	R7
      0x58380008,  //  0058  LDCONST	R14	K8
      0x7C2C0600,  //  0059  CALL	R11	3
      0x5431FFFE,  //  005A  LDINT	R12	-1
      0x941C160C,  //  005B  GETIDX	R7	R11	R12
      0x50240200,  //  005C  LDBOOL	R9	1	0
      0x78260007,  //  005D  JMPF	R9	#0066
      0xB82E0200,  //  005E  GETNGBL	R11	K1
      0x8C2C1710,  //  005F  GETMET	R11	R11	K16
      0x5C340E00,  //  0060  MOVE	R13	R7
      0x5C381000,  //  0061  MOVE	R14	R8
      0x583C000B,  //  0062  LDCONST	R15	K11
      0x7C2C0800,  //  0063  CALL	R11	4
      0x5C081600,  //  0064  MOVE	R2	R11
      0x7002001A,  //  0065  JMP		#0081
      0x782A0005,  //  0066  JMPF	R10	#006D
      0xB82E0200,  //  0067  GETNGBL	R11	K1
      0x8C2C1711,  //  0068  GETMET	R11	R11	K17
      0x5C340200,  //  0069  MOVE	R13	R1
      0x7C2C0400,  //  006A  CALL	R11	2
      0x5C081600,  //  006B  MOVE	R2	R11
      0x70020013,  //  006C  JMP		#0081
      0x242C110B,  //  006D  GT	R11	R8	K11
      0x782E0011,  //  006E  JMPF	R11	#0081
      0x602C000C,  //  006F  GETGBL	R11	G12
      0x5C300E00,  //  0070  MOVE	R12	R7
      0x7C2C0200,  //  0071  CALL	R11	1
      0x242C170B,  //  0072  GT	R11	R11	K11
      0x782E000C,  //  0073  JMPF	R11	#0081
      0xA8020007,  //  0074  EXBLK	0	#007D
      0xB82E0200,  //  0075  GETNGBL	R11	K1
      0x8C2C1702,  //  0076  GETMET	R11	R11	K2
      0x5C340E00,  //  0077  MOVE	R13	R7
      0x5C381000,  //  0078  MOVE	R14	R8
      0x7C2C0600,  //  0079  CALL	R11	3
      0x5C081600,  //  007A  MOVE	R2	R11
      0xA8040001,  //  007B  EXBLK	1	1
      0x70020003,  //  007C  JMP		#0081
      0xAC2C0000,  //  007D  CATCH	R11	0	0
      0x70020000,  //  007E  JMP		#0080
      0x70020000,  //  007F  JMP		#0081
      0xB0080000,  //  0080  RAISE	2	R0	R0
      0x4C0C0000,  //  0081  LDNIL	R3
      0x200C0403,  //  0082  NE	R3	R2	R3
      0x780E0005,  //  0083  JMPF	R3	#008A
      0x880C0112,  //  0084  GETMBR	R3	R0	K18
      0x8C0C0713,  //  0085  GETMET	R3	R3	K19
      0x5C140400,  //  0086  MOVE	R5	R2
      0x5818000B,  //  0087  LDCONST	R6	K11
      0x7C0C0600,  //  0088  CALL	R3	3
      0x70020003,  //  0089  JMP		#008E
      0x600C0001,  //  008A  GETGBL	R3	G1
      0x58100014,  //  008B  LDCONST	R4	K20
      0x5C140200,  //  008C  MOVE	R5	R1
      0x7C0C0400,  //  008D  CALL	R3	2
      0x80000000,  //  008E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_radius2
********************************************************************/
be_local_closure(lvh_obj_set_radius2,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(_lv_part2_selector),
    /* K1   */  be_nested_str_weak(_lv_obj),
    /* K2   */  be_nested_str_weak(set_style_radius),
    /* K3   */  be_nested_str_weak(lv),
    /* K4   */  be_nested_str_weak(STATE_DEFAULT),
    }),
    be_str_weak(set_radius2),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x4C0C0000,  //  0001  LDNIL	R3
      0x20080403,  //  0002  NE	R2	R2	R3
      0x780A0009,  //  0003  JMPF	R2	#000E
      0x88080101,  //  0004  GETMBR	R2	R0	K1
      0x8C080502,  //  0005  GETMET	R2	R2	K2
      0x60100009,  //  0006  GETGBL	R4	G9
      0x5C140200,  //  0007  MOVE	R5	R1
      0x7C100200,  //  0008  CALL	R4	1
      0x88140100,  //  0009  GETMBR	R5	R0	K0
      0xB81A0600,  //  000A  GETNGBL	R6	K3
      0x88180D04,  //  000B  GETMBR	R6	R6	K4
      0x30140A06,  //  000C  OR	R5	R5	R6
      0x7C080600,  //  000D  CALL	R2	3
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_adjustable
********************************************************************/
be_local_closure(lvh_obj_get_adjustable,   /* name */
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
    /* K0   */  be_nested_str_weak(_lv_obj),
    /* K1   */  be_nested_str_weak(has_flag),
    /* K2   */  be_nested_str_weak(lv),
    /* K3   */  be_nested_str_weak(OBJ_FLAG_CLICKABLE),
    }),
    be_str_weak(get_adjustable),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0xB80E0400,  //  0002  GETNGBL	R3	K2
      0x880C0703,  //  0003  GETMBR	R3	R3	K3
      0x7C040400,  //  0004  CALL	R1	2
      0x80040200,  //  0005  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_mode
********************************************************************/
be_local_closure(lvh_obj_set_mode,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[18]) {     /* constants */
    /* K0   */  be_nested_str_weak(expand),
    /* K1   */  be_nested_str_weak(_lv_obj),
    /* K2   */  be_nested_str_weak(set_width),
    /* K3   */  be_nested_str_weak(lv),
    /* K4   */  be_nested_str_weak(SIZE_CONTENT),
    /* K5   */  be_nested_str_weak(break),
    /* K6   */  be_nested_str_weak(LABEL_LONG_WRAP),
    /* K7   */  be_nested_str_weak(dots),
    /* K8   */  be_nested_str_weak(LABEL_LONG_DOT),
    /* K9   */  be_nested_str_weak(scroll),
    /* K10  */  be_nested_str_weak(LABEL_LONG_SCROLL),
    /* K11  */  be_nested_str_weak(loop),
    /* K12  */  be_nested_str_weak(LABEL_LONG_SCROLL_CIRCULAR),
    /* K13  */  be_nested_str_weak(crop),
    /* K14  */  be_nested_str_weak(LABEL_LONG_CLIP),
    /* K15  */  be_nested_str_weak(check_label),
    /* K16  */  be_nested_str_weak(_lv_label),
    /* K17  */  be_nested_str_weak(set_long_mode),
    }),
    be_str_weak(set_mode),
    &be_const_str_solidified,
    ( &(const binstruction[43]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C0C0300,  //  0001  EQ	R3	R1	K0
      0x780E0005,  //  0002  JMPF	R3	#0009
      0x880C0101,  //  0003  GETMBR	R3	R0	K1
      0x8C0C0702,  //  0004  GETMET	R3	R3	K2
      0xB8160600,  //  0005  GETNGBL	R5	K3
      0x88140B04,  //  0006  GETMBR	R5	R5	K4
      0x7C0C0400,  //  0007  CALL	R3	2
      0x70020017,  //  0008  JMP		#0021
      0x1C0C0305,  //  0009  EQ	R3	R1	K5
      0x780E0002,  //  000A  JMPF	R3	#000E
      0xB80E0600,  //  000B  GETNGBL	R3	K3
      0x88080706,  //  000C  GETMBR	R2	R3	K6
      0x70020012,  //  000D  JMP		#0021
      0x1C0C0307,  //  000E  EQ	R3	R1	K7
      0x780E0002,  //  000F  JMPF	R3	#0013
      0xB80E0600,  //  0010  GETNGBL	R3	K3
      0x88080708,  //  0011  GETMBR	R2	R3	K8
      0x7002000D,  //  0012  JMP		#0021
      0x1C0C0309,  //  0013  EQ	R3	R1	K9
      0x780E0002,  //  0014  JMPF	R3	#0018
      0xB80E0600,  //  0015  GETNGBL	R3	K3
      0x8808070A,  //  0016  GETMBR	R2	R3	K10
      0x70020008,  //  0017  JMP		#0021
      0x1C0C030B,  //  0018  EQ	R3	R1	K11
      0x780E0002,  //  0019  JMPF	R3	#001D
      0xB80E0600,  //  001A  GETNGBL	R3	K3
      0x8808070C,  //  001B  GETMBR	R2	R3	K12
      0x70020003,  //  001C  JMP		#0021
      0x1C0C030D,  //  001D  EQ	R3	R1	K13
      0x780E0001,  //  001E  JMPF	R3	#0021
      0xB80E0600,  //  001F  GETNGBL	R3	K3
      0x8808070E,  //  0020  GETMBR	R2	R3	K14
      0x4C0C0000,  //  0021  LDNIL	R3
      0x200C0403,  //  0022  NE	R3	R2	R3
      0x780E0005,  //  0023  JMPF	R3	#002A
      0x8C0C010F,  //  0024  GETMET	R3	R0	K15
      0x7C0C0200,  //  0025  CALL	R3	1
      0x880C0110,  //  0026  GETMBR	R3	R0	K16
      0x8C0C0711,  //  0027  GETMET	R3	R3	K17
      0x5C140400,  //  0028  MOVE	R5	R2
      0x7C0C0400,  //  0029  CALL	R3	2
      0x80000000,  //  002A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_val_rule_formula
********************************************************************/
be_local_closure(lvh_obj_get_val_rule_formula,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(_val_rule_formula),
    }),
    be_str_weak(get_val_rule_formula),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: remove_trailing_zeroes
********************************************************************/
be_local_closure(lvh_obj_remove_trailing_zeroes,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    4,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_const_class(be_class_lvh_obj),
    /* K1   */  be_const_int(0),
    /* K2   */  be_const_int(1),
    /* K3   */  be_nested_str_weak(resize),
    }),
    be_str_weak(remove_trailing_zeroes),
    &be_const_str_solidified,
    ( &(const binstruction[24]) {  /* code */
      0x58040000,  //  0000  LDCONST	R1	K0
      0x6008000C,  //  0001  GETGBL	R2	G12
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C080200,  //  0003  CALL	R2	1
      0x580C0001,  //  0004  LDCONST	R3	K1
      0x14100602,  //  0005  LT	R4	R3	R2
      0x78120007,  //  0006  JMPF	R4	#000F
      0x5411FFFE,  //  0007  LDINT	R4	-1
      0x04100803,  //  0008  SUB	R4	R4	R3
      0x94100004,  //  0009  GETIDX	R4	R0	R4
      0x20100901,  //  000A  NE	R4	R4	K1
      0x78120000,  //  000B  JMPF	R4	#000D
      0x70020001,  //  000C  JMP		#000F
      0x000C0702,  //  000D  ADD	R3	R3	K2
      0x7001FFF5,  //  000E  JMP		#0005
      0x24100701,  //  000F  GT	R4	R3	K1
      0x78120005,  //  0010  JMPF	R4	#0017
      0x8C100103,  //  0011  GETMET	R4	R0	K3
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
** Solidified function: post_init
********************************************************************/
be_local_closure(lvh_obj_post_init,   /* name */
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
    /* K0   */  be_nested_str_weak(register_event_cb),
    }),
    be_str_weak(post_init),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x80000000,  //  0002  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: val_rule_matched
********************************************************************/
be_local_closure(lvh_obj_val_rule_matched,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(_val_rule_function),
    /* K1   */  be_nested_str_weak(HSP_X3A_X20failed_X20to_X20run_X20self_X2E_val_rule_function_X20_X2D_X20_X25s_X20_X28_X25s_X29),
    /* K2   */  be_nested_str_weak(val),
    }),
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
      0x880C0100,  //  0008  GETMBR	R3	R0	K0
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
      0x58200001,  //  0017  LDCONST	R8	K1
      0x5C240800,  //  0018  MOVE	R9	R4
      0x5C280A00,  //  0019  MOVE	R10	R5
      0x7C1C0600,  //  001A  CALL	R7	3
      0x7C180200,  //  001B  CALL	R6	1
      0x70020000,  //  001C  JMP		#001E
      0xB0080000,  //  001D  RAISE	2	R0	R0
      0x60100009,  //  001E  GETGBL	R4	G9
      0x5C140400,  //  001F  MOVE	R5	R2
      0x7C100200,  //  0020  CALL	R4	1
      0x90020404,  //  0021  SETMBR	R0	K2	R4
      0x50100000,  //  0022  LDBOOL	R4	0	0
      0x80040800,  //  0023  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: lvh_obj
********************************************************************/
be_local_class(lvh_obj,
    13,
    NULL,
    be_nested_map(94,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(val_rule_matched, 9), be_const_closure(lvh_obj_val_rule_matched_closure) },
        { be_const_key_weak(post_init, 18), be_const_closure(lvh_obj_post_init_closure) },
        { be_const_key_weak(set_val_rule, -1), be_const_closure(lvh_obj_set_val_rule_closure) },
        { be_const_key_weak(remove_trailing_zeroes, -1), be_const_static_closure(lvh_obj_remove_trailing_zeroes_closure) },
        { be_const_key_weak(_action, -1), be_const_var(4) },
        { be_const_key_weak(id, -1), be_const_var(0) },
        { be_const_key_weak(get_val_rule_formula, -1), be_const_closure(lvh_obj_get_val_rule_formula_closure) },
        { be_const_key_weak(set_mode, 80), be_const_closure(lvh_obj_set_mode_closure) },
        { be_const_key_weak(get_adjustable, -1), be_const_closure(lvh_obj_get_adjustable_closure) },
        { be_const_key_weak(set_radius2, -1), be_const_closure(lvh_obj_set_radius2_closure) },
        { be_const_key_weak(set_text_font, -1), be_const_closure(lvh_obj_set_text_font_closure) },
        { be_const_key_weak(set_pad_all2, -1), be_const_closure(lvh_obj_set_pad_all2_closure) },
        { be_const_key_weak(text_rule_matched, -1), be_const_closure(lvh_obj_text_rule_matched_closure) },
        { be_const_key_weak(set_value_ofs_x, -1), be_const_closure(lvh_obj_set_value_ofs_x_closure) },
        { be_const_key_weak(_text_rule, 45), be_const_var(9) },
        { be_const_key_weak(_page, -1), be_const_var(3) },
        { be_const_key_weak(get_val_rule, 73), be_const_closure(lvh_obj_get_val_rule_closure) },
        { be_const_key_weak(get_text_color, -1), be_const_closure(lvh_obj_get_text_color_closure) },
        { be_const_key_weak(set_text_rule, -1), be_const_closure(lvh_obj_set_text_rule_closure) },
        { be_const_key_weak(get_enabled, -1), be_const_closure(lvh_obj_get_enabled_closure) },
        { be_const_key_weak(_attr_ignore, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(10,
    ( (struct bvalue*) &(const bvalue[]) {
        be_nested_str_weak(tostring),
        be_nested_str_weak(obj),
        be_nested_str_weak(page),
        be_nested_str_weak(comment),
        be_nested_str_weak(parentid),
        be_nested_str_weak(auto_size),
        be_nested_str_weak(prev),
        be_nested_str_weak(next),
        be_nested_str_weak(back),
        be_nested_str_weak(berry_run),
    }))    ) } )) },
        { be_const_key_weak(init, -1), be_const_closure(lvh_obj_init_closure) },
        { be_const_key_weak(_text_rule_format, -1), be_const_var(12) },
        { be_const_key_weak(get_text, -1), be_const_closure(lvh_obj_get_text_closure) },
        { be_const_key_weak(get_text_font, -1), be_const_closure(lvh_obj_get_text_font_closure) },
        { be_const_key_weak(_lv_class, 87), be_const_class(be_class_lv_obj) },
        { be_const_key_weak(get_value_str, -1), be_const_closure(lvh_obj_get_value_str_closure) },
        { be_const_key_weak(set_align, 42), be_const_closure(lvh_obj_set_align_closure) },
        { be_const_key_weak(set_val_rule_formula, -1), be_const_closure(lvh_obj_set_val_rule_formula_closure) },
        { be_const_key_weak(_text_rule_function, 30), be_const_var(11) },
        { be_const_key_weak(set_enabled, 79), be_const_closure(lvh_obj_set_enabled_closure) },
        { be_const_key_weak(set_value_font, -1), be_const_closure(lvh_obj_set_value_font_closure) },
        { be_const_key_weak(set_toggle, -1), be_const_closure(lvh_obj_set_toggle_closure) },
        { be_const_key_weak(_val_rule_function, -1), be_const_var(8) },
        { be_const_key_weak(get_value_ofs_y, -1), be_const_closure(lvh_obj_get_value_ofs_y_closure) },
        { be_const_key_weak(_text_rule_formula, 88), be_const_var(10) },
        { be_const_key_weak(set_pad_left2, 93), be_const_closure(lvh_obj_set_pad_left2_closure) },
        { be_const_key_weak(digits_to_style, -1), be_const_closure(lvh_obj_digits_to_style_closure) },
        { be_const_key_weak(get_action, -1), be_const_closure(lvh_obj_get_action_closure) },
        { be_const_key_weak(get_obj, 84), be_const_closure(lvh_obj_get_obj_closure) },
        { be_const_key_weak(_attr_map, 50), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(26,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(rotation, -1), be_nested_str_weak(rotation) },
        { be_const_key_weak(bg_opa, -1), be_nested_str_weak(style_bg_opa) },
        { be_const_key_weak(end_angle, 3), be_nested_str_weak(bg_end_angle) },
        { be_const_key_weak(w, 18), be_nested_str_weak(width) },
        { be_const_key_weak(src, 6), be_nested_str_weak(src) },
        { be_const_key_weak(x, -1), be_nested_str_weak(x) },
        { be_const_key_weak(border_color, -1), be_nested_str_weak(style_border_color) },
        { be_const_key_weak(bg_grad_color, 0), be_nested_str_weak(style_bg_grad_color) },
        { be_const_key_weak(pad_all, -1), be_nested_str_weak(style_pad_all) },
        { be_const_key_weak(bg_color, 20), be_nested_str_weak(style_bg_color) },
        { be_const_key_weak(y, 8), be_nested_str_weak(y) },
        { be_const_key_weak(line_color, -1), be_nested_str_weak(style_line_color) },
        { be_const_key_weak(image_recolor_opa, 10), be_nested_str_weak(style_img_recolor_opa) },
        { be_const_key_weak(start_angle, 5), be_nested_str_weak(bg_start_angle) },
        { be_const_key_weak(end_angle1, 11), be_nested_str_weak(end_angle) },
        { be_const_key_weak(border_side, -1), be_nested_str_weak(style_border_side) },
        { be_const_key_weak(pad_bottom, 9), be_nested_str_weak(style_pad_bottom) },
        { be_const_key_weak(radius, 4), be_nested_str_weak(style_radius) },
        { be_const_key_weak(pad_left, -1), be_nested_str_weak(style_pad_left) },
        { be_const_key_weak(image_recolor, -1), be_nested_str_weak(style_img_recolor) },
        { be_const_key_weak(border_width, -1), be_nested_str_weak(style_border_width) },
        { be_const_key_weak(pad_right, -1), be_nested_str_weak(style_pad_right) },
        { be_const_key_weak(start_angle1, -1), be_nested_str_weak(start_angle) },
        { be_const_key_weak(h, -1), be_nested_str_weak(height) },
        { be_const_key_weak(pad_top, 16), be_nested_str_weak(style_pad_top) },
        { be_const_key_weak(bg_grad_dir, 14), be_nested_str_weak(style_bg_grad_dir) },
    }))    ) } )) },
        { be_const_key_weak(register_event_cb, 17), be_const_closure(lvh_obj_register_event_cb_closure) },
        { be_const_key_weak(set_pad_top2, 61), be_const_closure(lvh_obj_set_pad_top2_closure) },
        { be_const_key_weak(event_cb, 7), be_const_closure(lvh_obj_event_cb_closure) },
        { be_const_key_weak(check_label, -1), be_const_closure(lvh_obj_check_label_closure) },
        { be_const_key_weak(_lv_part2_selector, -1), be_const_nil() },
        { be_const_key_weak(_digit2part, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(10,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(131072),
        be_const_int(196608),
        be_const_int(327680),
        be_const_int(327680),
        be_const_int(262144),
        be_const_int(393216),
        be_const_int(458752),
        be_const_int(65536),
        be_const_int(524288),
    }))    ) } )) },
        { be_const_key_weak(get_val, 75), be_const_closure(lvh_obj_get_val_closure) },
        { be_const_key_weak(get_pad_all, -1), be_const_closure(lvh_obj_get_pad_all_closure) },
        { be_const_key_weak(get_pad_bottom, 66), be_const_closure(lvh_obj_get_pad_bottom_closure) },
        { be_const_key_weak(get_toggle, -1), be_const_closure(lvh_obj_get_toggle_closure) },
        { be_const_key_weak(set_line_width, -1), be_const_closure(lvh_obj_set_line_width_closure) },
        { be_const_key_weak(get_text_rule_format, -1), be_const_closure(lvh_obj_get_text_rule_format_closure) },
        { be_const_key_weak(set_pad_bottom2, 74), be_const_closure(lvh_obj_set_pad_bottom2_closure) },
        { be_const_key_weak(set_action, 16), be_const_closure(lvh_obj_set_action_closure) },
        { be_const_key_weak(set_text_color, 47), be_const_closure(lvh_obj_set_text_color_closure) },
        { be_const_key_weak(get_pad_right, -1), be_const_closure(lvh_obj_get_pad_right_closure) },
        { be_const_key_weak(get_value_font, -1), be_const_closure(lvh_obj_get_value_font_closure) },
        { be_const_key_weak(get_text_rule, 40), be_const_closure(lvh_obj_get_text_rule_closure) },
        { be_const_key_weak(set_meta, 8), be_const_closure(lvh_obj_set_meta_closure) },
        { be_const_key_weak(set_text_rule_format, -1), be_const_closure(lvh_obj_set_text_rule_format_closure) },
        { be_const_key_weak(get_line_width, -1), be_const_closure(lvh_obj_get_line_width_closure) },
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
        { be_const_key_weak(get_value_ofs_x, -1), be_const_closure(lvh_obj_get_value_ofs_x_closure) },
        { be_const_key_weak(is_color_attribute, -1), be_const_static_closure(lvh_obj_is_color_attribute_closure) },
        { be_const_key_weak(get_text_rule_formula, 26), be_const_closure(lvh_obj_get_text_rule_formula_closure) },
        { be_const_key_weak(set_value_color, -1), be_const_closure(lvh_obj_set_value_color_closure) },
        { be_const_key_weak(get_pad_top, -1), be_const_closure(lvh_obj_get_pad_top_closure) },
        { be_const_key_weak(get_hidden, -1), be_const_closure(lvh_obj_get_hidden_closure) },
        { be_const_key_weak(get_pad_left, -1), be_const_closure(lvh_obj_get_pad_left_closure) },
        { be_const_key_weak(get_radius2, -1), be_const_closure(lvh_obj_get_radius2_closure) },
        { be_const_key_weak(set_value_str, 48), be_const_closure(lvh_obj_set_value_str_closure) },
        { be_const_key_weak(set_click, -1), be_const_closure(lvh_obj_set_click_closure) },
        { be_const_key_weak(parse_color, 82), be_const_static_closure(lvh_obj_parse_color_closure) },
        { be_const_key_weak(set_text, 36), be_const_closure(lvh_obj_set_text_closure) },
        { be_const_key_weak(member, -1), be_const_closure(lvh_obj_member_closure) },
        { be_const_key_weak(set_pad_right2, 38), be_const_closure(lvh_obj_set_pad_right2_closure) },
        { be_const_key_weak(set_adjustable, -1), be_const_closure(lvh_obj_set_adjustable_closure) },
        { be_const_key_weak(set_value_ofs_y, -1), be_const_closure(lvh_obj_set_value_ofs_y_closure) },
        { be_const_key_weak(get_mode, 28), be_const_closure(lvh_obj_get_mode_closure) },
        { be_const_key_weak(setmember, -1), be_const_closure(lvh_obj_setmember_closure) },
        { be_const_key_weak(_val_rule_formula, 24), be_const_var(7) },
        { be_const_key_weak(get_click, -1), be_const_closure(lvh_obj_get_click_closure) },
        { be_const_key_weak(get_meta, -1), be_const_closure(lvh_obj_get_meta_closure) },
        { be_const_key_weak(get_value_color, -1), be_const_closure(lvh_obj_get_value_color_closure) },
        { be_const_key_weak(_meta, 10), be_const_var(5) },
        { be_const_key_weak(_val_rule, 0), be_const_var(6) },
        { be_const_key_weak(_lv_label, -1), be_const_var(2) },
        { be_const_key_weak(set_text_rule_formula, -1), be_const_closure(lvh_obj_set_text_rule_formula_closure) },
        { be_const_key_weak(set_val, 6), be_const_closure(lvh_obj_set_val_closure) },
        { be_const_key_weak(set_hidden, 5), be_const_closure(lvh_obj_set_hidden_closure) },
        { be_const_key_weak(_event_map, 3), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(7,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(28, 2), be_nested_str_weak(changed) },
        { be_const_key_int(8, 4), be_nested_str_weak(release) },
        { be_const_key_int(7, -1), be_nested_str_weak(up) },
        { be_const_key_int(3, -1), be_nested_str_weak(lost) },
        { be_const_key_int(1, -1), be_nested_str_weak(down) },
        { be_const_key_int(5, -1), be_nested_str_weak(long) },
        { be_const_key_int(6, -1), be_nested_str_weak(hold) },
    }))    ) } )) },
        { be_const_key_weak(_lv_obj, 1), be_const_var(1) },
        { be_const_key_weak(get_align, -1), be_const_closure(lvh_obj_get_align_closure) },
    })),
    be_str_weak(lvh_obj)
);
/*******************************************************************/

void be_load_lvh_obj_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_lvh_obj);
    be_setglobal(vm, "lvh_obj");
    be_pop(vm, 1);
}

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
/*******************************************************************/

void be_load_lvh_scr_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_lvh_scr);
    be_setglobal(vm, "lvh_scr");
    be_pop(vm, 1);
}

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
        { be_const_key_weak(_lv_class, -1), be_const_class(be_class_lv_btn) },
    })),
    be_str_weak(lvh_btn)
);
/*******************************************************************/

void be_load_lvh_btn_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_lvh_btn);
    be_setglobal(vm, "lvh_btn");
    be_pop(vm, 1);
}

extern const bclass be_class_lvh_switch;

/********************************************************************
** Solidified function: set_val
********************************************************************/
be_local_closure(lvh_switch_set_val,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(set_toggle),
    }),
    be_str_weak(set_val),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x5C100200,  //  0001  MOVE	R4	R1
      0x7C080400,  //  0002  CALL	R2	2
      0x80040400,  //  0003  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_val
********************************************************************/
be_local_closure(lvh_switch_get_val,   /* name */
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
        { be_const_key_weak(get_val, 1), be_const_closure(lvh_switch_get_val_closure) },
        { be_const_key_weak(set_val, -1), be_const_closure(lvh_switch_set_val_closure) },
        { be_const_key_weak(_lv_part2_selector, -1), be_const_int(196608) },
        { be_const_key_weak(_lv_class, 0), be_const_class(be_class_lv_switch) },
    })),
    be_str_weak(lvh_switch)
);
/*******************************************************************/

void be_load_lvh_switch_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_lvh_switch);
    be_setglobal(vm, "lvh_switch");
    be_pop(vm, 1);
}

extern const bclass be_class_lvh_checkbox;

/********************************************************************
** Solidified class: lvh_checkbox
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_class(lvh_checkbox,
    0,
    &be_class_lvh_obj,
    be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(_lv_class, -1), be_const_class(be_class_lv_checkbox) },
    })),
    be_str_weak(lvh_checkbox)
);
/*******************************************************************/

void be_load_lvh_checkbox_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_lvh_checkbox);
    be_setglobal(vm, "lvh_checkbox");
    be_pop(vm, 1);
}

extern const bclass be_class_lvh_label;

/********************************************************************
** Solidified function: post_init
********************************************************************/
be_local_closure(lvh_label_post_init,   /* name */
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
    /* K0   */  be_nested_str_weak(_lv_label),
    /* K1   */  be_nested_str_weak(_lv_obj),
    /* K2   */  be_nested_str_weak(post_init),
    }),
    be_str_weak(post_init),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x88040101,  //  0000  GETMBR	R1	R0	K1
      0x90020001,  //  0001  SETMBR	R0	K0	R1
      0x60040003,  //  0002  GETGBL	R1	G3
      0x5C080000,  //  0003  MOVE	R2	R0
      0x7C040200,  //  0004  CALL	R1	1
      0x8C040302,  //  0005  GETMET	R1	R1	K2
      0x7C040200,  //  0006  CALL	R1	1
      0x80000000,  //  0007  RET	0
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
        { be_const_key_weak(post_init, -1), be_const_closure(lvh_label_post_init_closure) },
        { be_const_key_weak(_lv_class, -1), be_const_class(be_class_lv_label) },
    })),
    be_str_weak(lvh_label)
);
/*******************************************************************/

void be_load_lvh_label_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_lvh_label);
    be_setglobal(vm, "lvh_label");
    be_pop(vm, 1);
}

extern const bclass be_class_lvh_spinner;

/********************************************************************
** Solidified function: set_angle
********************************************************************/
be_local_closure(lvh_spinner_set_angle,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(_anim_end),
    /* K1   */  be_nested_str_weak(start_value),
    /* K2   */  be_nested_str_weak(end_value),
    }),
    be_str_weak(set_angle),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x60080009,  //  0000  GETGBL	R2	G9
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x7C080200,  //  0002  CALL	R2	1
      0x5C040400,  //  0003  MOVE	R1	R2
      0x88080100,  //  0004  GETMBR	R2	R0	K0
      0x900A0201,  //  0005  SETMBR	R2	K1	R1
      0x88080100,  //  0006  GETMBR	R2	R0	K0
      0x540E0167,  //  0007  LDINT	R3	360
      0x000C0203,  //  0008  ADD	R3	R1	R3
      0x900A0403,  //  0009  SETMBR	R2	K2	R3
      0x80000000,  //  000A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(lvh_spinner_init,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[15]) {     /* constants */
    /* K0   */  be_nested_str_weak(_page),
    /* K1   */  be_nested_str_weak(find),
    /* K2   */  be_nested_str_weak(angle),
    /* K3   */  be_nested_str_weak(speed),
    /* K4   */  be_nested_str_weak(_lv_obj),
    /* K5   */  be_nested_str_weak(lv),
    /* K6   */  be_nested_str_weak(spinner),
    /* K7   */  be_nested_str_weak(post_init),
    /* K8   */  be_nested_str_weak(anim_get),
    /* K9   */  be_nested_str_weak(_arc_anim_start_angle),
    /* K10  */  be_nested_str_weak(_arc_anim_end_angle),
    /* K11  */  be_nested_str_weak(_anim_start),
    /* K12  */  be_nested_str_weak(anim_ntv),
    /* K13  */  be_nested_str_weak(_p),
    /* K14  */  be_nested_str_weak(_anim_end),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[41]) {  /* code */
      0x90020002,  //  0000  SETMBR	R0	K0	R2
      0x8C100701,  //  0001  GETMET	R4	R3	K1
      0x58180002,  //  0002  LDCONST	R6	K2
      0x541E003B,  //  0003  LDINT	R7	60
      0x7C100600,  //  0004  CALL	R4	3
      0x8C140701,  //  0005  GETMET	R5	R3	K1
      0x581C0003,  //  0006  LDCONST	R7	K3
      0x542203E7,  //  0007  LDINT	R8	1000
      0x7C140600,  //  0008  CALL	R5	3
      0xB81A0A00,  //  0009  GETNGBL	R6	K5
      0x8C180D06,  //  000A  GETMET	R6	R6	K6
      0x5C200200,  //  000B  MOVE	R8	R1
      0x5C240A00,  //  000C  MOVE	R9	R5
      0x5C280800,  //  000D  MOVE	R10	R4
      0x7C180800,  //  000E  CALL	R6	4
      0x90020806,  //  000F  SETMBR	R0	K4	R6
      0x8C180107,  //  0010  GETMET	R6	R0	K7
      0x7C180200,  //  0011  CALL	R6	1
      0xB81A0A00,  //  0012  GETNGBL	R6	K5
      0x8C180D08,  //  0013  GETMET	R6	R6	K8
      0x88200104,  //  0014  GETMBR	R8	R0	K4
      0x88240104,  //  0015  GETMBR	R9	R0	K4
      0x88241309,  //  0016  GETMBR	R9	R9	K9
      0x7C180600,  //  0017  CALL	R6	3
      0xB81E0A00,  //  0018  GETNGBL	R7	K5
      0x8C1C0F08,  //  0019  GETMET	R7	R7	K8
      0x88240104,  //  001A  GETMBR	R9	R0	K4
      0x88280104,  //  001B  GETMBR	R10	R0	K4
      0x8828150A,  //  001C  GETMBR	R10	R10	K10
      0x7C1C0600,  //  001D  CALL	R7	3
      0xB8220A00,  //  001E  GETNGBL	R8	K5
      0x8C20110C,  //  001F  GETMET	R8	R8	K12
      0x88280D0D,  //  0020  GETMBR	R10	R6	K13
      0x7C200400,  //  0021  CALL	R8	2
      0x90021608,  //  0022  SETMBR	R0	K11	R8
      0xB8220A00,  //  0023  GETNGBL	R8	K5
      0x8C20110C,  //  0024  GETMET	R8	R8	K12
      0x88280F0D,  //  0025  GETMBR	R10	R7	K13
      0x7C200400,  //  0026  CALL	R8	2
      0x90021C08,  //  0027  SETMBR	R0	K14	R8
      0x80000000,  //  0028  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_angle
********************************************************************/
be_local_closure(lvh_spinner_get_angle,   /* name */
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
    /* K0   */  be_nested_str_weak(_anim_end),
    /* K1   */  be_nested_str_weak(start_value),
    /* K2   */  be_nested_str_weak(_anim_start),
    }),
    be_str_weak(get_angle),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x88040301,  //  0001  GETMBR	R1	R1	K1
      0x88080102,  //  0002  GETMBR	R2	R0	K2
      0x88080501,  //  0003  GETMBR	R2	R2	K1
      0x04040202,  //  0004  SUB	R1	R1	R2
      0x80040200,  //  0005  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_speed
********************************************************************/
be_local_closure(lvh_spinner_get_speed,   /* name */
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
    /* K0   */  be_nested_str_weak(_anim_start),
    /* K1   */  be_nested_str_weak(time),
    }),
    be_str_weak(get_speed),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x88040301,  //  0001  GETMBR	R1	R1	K1
      0x80040200,  //  0002  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_speed
********************************************************************/
be_local_closure(lvh_spinner_set_speed,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(_anim_start),
    /* K1   */  be_nested_str_weak(time),
    /* K2   */  be_nested_str_weak(_anim_end),
    }),
    be_str_weak(set_speed),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x60080009,  //  0000  GETGBL	R2	G9
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x7C080200,  //  0002  CALL	R2	1
      0x5C040400,  //  0003  MOVE	R1	R2
      0x88080100,  //  0004  GETMBR	R2	R0	K0
      0x900A0201,  //  0005  SETMBR	R2	K1	R1
      0x88080102,  //  0006  GETMBR	R2	R0	K2
      0x900A0201,  //  0007  SETMBR	R2	K1	R1
      0x80000000,  //  0008  RET	0
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
        { be_const_key_weak(set_speed, 1), be_const_closure(lvh_spinner_set_speed_closure) },
        { be_const_key_weak(_lv_class, -1), be_const_class(be_class_lv_spinner) },
        { be_const_key_weak(get_speed, 6), be_const_closure(lvh_spinner_get_speed_closure) },
        { be_const_key_weak(init, 2), be_const_closure(lvh_spinner_init_closure) },
        { be_const_key_weak(_anim_start, -1), be_const_var(0) },
        { be_const_key_weak(_anim_end, -1), be_const_var(1) },
        { be_const_key_weak(get_angle, -1), be_const_closure(lvh_spinner_get_angle_closure) },
        { be_const_key_weak(set_angle, 0), be_const_closure(lvh_spinner_set_angle_closure) },
    })),
    be_str_weak(lvh_spinner)
);
/*******************************************************************/

void be_load_lvh_spinner_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_lvh_spinner);
    be_setglobal(vm, "lvh_spinner");
    be_pop(vm, 1);
}

extern const bclass be_class_lvh_line;

/********************************************************************
** Solidified class: lvh_line
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_class(lvh_line,
    0,
    &be_class_lvh_obj,
    be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(_lv_class, -1), be_const_class(be_class_lv_line) },
    })),
    be_str_weak(lvh_line)
);
/*******************************************************************/

void be_load_lvh_line_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_lvh_line);
    be_setglobal(vm, "lvh_line");
    be_pop(vm, 1);
}

extern const bclass be_class_lvh_img;

/********************************************************************
** Solidified function: set_angle
********************************************************************/
be_local_closure(lvh_img_set_angle,   /* name */
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
    /* K0   */  be_nested_str_weak(_lv_obj),
    /* K1   */  be_nested_str_weak(set_angle),
    }),
    be_str_weak(set_angle),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x60080009,  //  0000  GETGBL	R2	G9
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x7C080200,  //  0002  CALL	R2	1
      0x5C040400,  //  0003  MOVE	R1	R2
      0x88080100,  //  0004  GETMBR	R2	R0	K0
      0x8C080501,  //  0005  GETMET	R2	R2	K1
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
be_local_closure(lvh_img_get_angle,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(_lv_obj),
    /* K1   */  be_nested_str_weak(get_angle),
    }),
    be_str_weak(get_angle),
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
** Solidified class: lvh_img
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_class(lvh_img,
    0,
    &be_class_lvh_obj,
    be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(set_angle, -1), be_const_closure(lvh_img_set_angle_closure) },
        { be_const_key_weak(get_angle, 2), be_const_closure(lvh_img_get_angle_closure) },
        { be_const_key_weak(_lv_class, -1), be_const_class(be_class_lv_img) },
    })),
    be_str_weak(lvh_img)
);
/*******************************************************************/

void be_load_lvh_img_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_lvh_img);
    be_setglobal(vm, "lvh_img");
    be_pop(vm, 1);
}

extern const bclass be_class_lvh_roller;

/********************************************************************
** Solidified function: set_options
********************************************************************/
be_local_closure(lvh_roller_set_options,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(_lv_obj),
    /* K1   */  be_nested_str_weak(set_options),
    /* K2   */  be_nested_str_weak(lv),
    /* K3   */  be_nested_str_weak(ROLLER_MODE_NORMAL),
    }),
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
be_local_closure(lvh_roller_get_text,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(resize),
    /* K1   */  be_nested_str_weak(_lv_obj),
    /* K2   */  be_nested_str_weak(get_selected_str),
    /* K3   */  be_nested_str_weak(_buffer),
    /* K4   */  be_nested_str_weak(remove_trailing_zeroes),
    /* K5   */  be_nested_str_weak(asstring),
    }),
    be_str_weak(get_text),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x60040015,  //  0000  GETGBL	R1	G21
      0x7C040000,  //  0001  CALL	R1	0
      0x8C040300,  //  0002  GETMET	R1	R1	K0
      0x540E00FF,  //  0003  LDINT	R3	256
      0x7C040400,  //  0004  CALL	R1	2
      0x88080101,  //  0005  GETMBR	R2	R0	K1
      0x8C080502,  //  0006  GETMET	R2	R2	K2
      0x8C100303,  //  0007  GETMET	R4	R1	K3
      0x7C100200,  //  0008  CALL	R4	1
      0x541600FF,  //  0009  LDINT	R5	256
      0x7C080600,  //  000A  CALL	R2	3
      0x8C080104,  //  000B  GETMET	R2	R0	K4
      0x5C100200,  //  000C  MOVE	R4	R1
      0x7C080400,  //  000D  CALL	R2	2
      0x5C040400,  //  000E  MOVE	R1	R2
      0x8C080305,  //  000F  GETMET	R2	R1	K5
      0x7C080200,  //  0010  CALL	R2	1
      0x80040400,  //  0011  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_options
********************************************************************/
be_local_closure(lvh_roller_get_options,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(_lv_obj),
    /* K1   */  be_nested_str_weak(get_options),
    }),
    be_str_weak(get_options),
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
** Solidified function: set_val
********************************************************************/
be_local_closure(lvh_roller_set_val,   /* name */
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
    /* K1   */  be_nested_str_weak(set_selected),
    /* K2   */  be_const_int(0),
    }),
    be_str_weak(set_val),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5C100200,  //  0002  MOVE	R4	R1
      0x58140002,  //  0003  LDCONST	R5	K2
      0x7C080600,  //  0004  CALL	R2	3
      0x80000000,  //  0005  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_text
********************************************************************/
be_local_closure(lvh_roller_set_text,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(attribute_error),
    /* K1   */  be_nested_str_weak(set_text_X20unsupported_X20on_X20roller),
    }),
    be_str_weak(set_text),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0xB0060101,  //  0000  RAISE	1	K0	K1
      0x80000000,  //  0001  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_val
********************************************************************/
be_local_closure(lvh_roller_get_val,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(_lv_obj),
    /* K1   */  be_nested_str_weak(get_selected),
    }),
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
** Solidified class: lvh_roller
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_class(lvh_roller,
    0,
    &be_class_lvh_obj,
    be_nested_map(7,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(set_options, 2), be_const_closure(lvh_roller_set_options_closure) },
        { be_const_key_weak(get_text, -1), be_const_closure(lvh_roller_get_text_closure) },
        { be_const_key_weak(get_options, -1), be_const_closure(lvh_roller_get_options_closure) },
        { be_const_key_weak(_lv_class, -1), be_const_class(be_class_lv_roller) },
        { be_const_key_weak(set_text, -1), be_const_closure(lvh_roller_set_text_closure) },
        { be_const_key_weak(set_val, 4), be_const_closure(lvh_roller_set_val_closure) },
        { be_const_key_weak(get_val, -1), be_const_closure(lvh_roller_get_val_closure) },
    })),
    be_str_weak(lvh_roller)
);
/*******************************************************************/

void be_load_lvh_roller_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_lvh_roller);
    be_setglobal(vm, "lvh_roller");
    be_pop(vm, 1);
}

extern const bclass be_class_lvh_btnmatrix;

/********************************************************************
** Solidified class: lvh_btnmatrix
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_class(lvh_btnmatrix,
    0,
    &be_class_lvh_obj,
    be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(_lv_class, -1), be_const_class(be_class_lv_btnmatrix) },
    })),
    be_str_weak(lvh_btnmatrix)
);
/*******************************************************************/

void be_load_lvh_btnmatrix_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_lvh_btnmatrix);
    be_setglobal(vm, "lvh_btnmatrix");
    be_pop(vm, 1);
}

extern const bclass be_class_lvh_bar;

/********************************************************************
** Solidified class: lvh_bar
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_class(lvh_bar,
    0,
    &be_class_lvh_obj,
    be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(_lv_class, -1), be_const_class(be_class_lv_bar) },
    })),
    be_str_weak(lvh_bar)
);
/*******************************************************************/

void be_load_lvh_bar_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_lvh_bar);
    be_setglobal(vm, "lvh_bar");
    be_pop(vm, 1);
}

extern const bclass be_class_lvh_slider;

/********************************************************************
** Solidified function: set_val
********************************************************************/
be_local_closure(lvh_slider_set_val,   /* name */
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
    /* K1   */  be_nested_str_weak(set_value),
    /* K2   */  be_const_int(0),
    }),
    be_str_weak(set_val),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5C100200,  //  0002  MOVE	R4	R1
      0x58140002,  //  0003  LDCONST	R5	K2
      0x7C080600,  //  0004  CALL	R2	3
      0x80000000,  //  0005  RET	0
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
    be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(set_val, -1), be_const_closure(lvh_slider_set_val_closure) },
        { be_const_key_weak(_lv_class, 0), be_const_class(be_class_lv_slider) },
    })),
    be_str_weak(lvh_slider)
);
/*******************************************************************/

void be_load_lvh_slider_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_lvh_slider);
    be_setglobal(vm, "lvh_slider");
    be_pop(vm, 1);
}

extern const bclass be_class_lvh_arc;

/********************************************************************
** Solidified function: set_line_width1
********************************************************************/
be_local_closure(lvh_arc_set_line_width1,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(_lv_obj),
    /* K1   */  be_nested_str_weak(set_style_arc_width),
    /* K2   */  be_nested_str_weak(lv),
    /* K3   */  be_nested_str_weak(PART_INDICATOR),
    /* K4   */  be_nested_str_weak(STATE_DEFAULT),
    }),
    be_str_weak(set_line_width1),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x60100009,  //  0002  GETGBL	R4	G9
      0x5C140200,  //  0003  MOVE	R5	R1
      0x7C100200,  //  0004  CALL	R4	1
      0xB8160400,  //  0005  GETNGBL	R5	K2
      0x88140B03,  //  0006  GETMBR	R5	R5	K3
      0xB81A0400,  //  0007  GETNGBL	R6	K2
      0x88180D04,  //  0008  GETMBR	R6	R6	K4
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
be_local_closure(lvh_arc_get_min,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(_lv_obj),
    /* K1   */  be_nested_str_weak(get_min_value),
    }),
    be_str_weak(get_min),
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
** Solidified function: get_max
********************************************************************/
be_local_closure(lvh_arc_get_max,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(_lv_obj),
    /* K1   */  be_nested_str_weak(get_max_value),
    }),
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
** Solidified function: get_line_width
********************************************************************/
be_local_closure(lvh_arc_get_line_width,   /* name */
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
    /* K0   */  be_nested_str_weak(_lv_obj),
    /* K1   */  be_nested_str_weak(get_arc_line_width),
    }),
    be_str_weak(get_line_width),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x80040400,  //  0004  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_type
********************************************************************/
be_local_closure(lvh_arc_get_type,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(_lv_obj),
    /* K1   */  be_nested_str_weak(get_mode),
    }),
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
** Solidified function: set_min
********************************************************************/
be_local_closure(lvh_arc_set_min,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(_lv_obj),
    /* K1   */  be_nested_str_weak(set_range),
    /* K2   */  be_nested_str_weak(get_max),
    }),
    be_str_weak(set_min),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x60100009,  //  0002  GETGBL	R4	G9
      0x5C140200,  //  0003  MOVE	R5	R1
      0x7C100200,  //  0004  CALL	R4	1
      0x8C140102,  //  0005  GETMET	R5	R0	K2
      0x7C140200,  //  0006  CALL	R5	1
      0x7C080600,  //  0007  CALL	R2	3
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_line_width1
********************************************************************/
be_local_closure(lvh_arc_get_line_width1,   /* name */
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
    /* K0   */  be_nested_str_weak(_lv_obj),
    /* K1   */  be_nested_str_weak(get_arc_line_width),
    /* K2   */  be_nested_str_weak(lv),
    /* K3   */  be_nested_str_weak(PART_INDICATOR),
    /* K4   */  be_nested_str_weak(STATE_DEFAULT),
    }),
    be_str_weak(get_line_width1),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0xB80E0400,  //  0002  GETNGBL	R3	K2
      0x880C0703,  //  0003  GETMBR	R3	R3	K3
      0xB8120400,  //  0004  GETNGBL	R4	K2
      0x88100904,  //  0005  GETMBR	R4	R4	K4
      0x300C0604,  //  0006  OR	R3	R3	R4
      0x7C040400,  //  0007  CALL	R1	2
      0x80040200,  //  0008  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_line_width
********************************************************************/
be_local_closure(lvh_arc_set_line_width,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(_lv_obj),
    /* K1   */  be_nested_str_weak(set_style_arc_width),
    }),
    be_str_weak(set_line_width),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x8C0C0701,  //  0001  GETMET	R3	R3	K1
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
** Solidified function: set_max
********************************************************************/
be_local_closure(lvh_arc_set_max,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(_lv_obj),
    /* K1   */  be_nested_str_weak(set_range),
    /* K2   */  be_nested_str_weak(get_min),
    }),
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
** Solidified function: set_type
********************************************************************/
be_local_closure(lvh_arc_set_type,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(lv),
    /* K2   */  be_nested_str_weak(ARC_MODE_NORMAL),
    /* K3   */  be_const_int(1),
    /* K4   */  be_nested_str_weak(ARC_MODE_REVERSE),
    /* K5   */  be_const_int(2),
    /* K6   */  be_nested_str_weak(ARC_MODE_SYMMETRICAL),
    /* K7   */  be_nested_str_weak(_lv_obj),
    /* K8   */  be_nested_str_weak(set_mode),
    }),
    be_str_weak(set_type),
    &be_const_str_solidified,
    ( &(const binstruction[23]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C0C0300,  //  0001  EQ	R3	R1	K0
      0x780E0002,  //  0002  JMPF	R3	#0006
      0xB80E0200,  //  0003  GETNGBL	R3	K1
      0x88080702,  //  0004  GETMBR	R2	R3	K2
      0x70020008,  //  0005  JMP		#000F
      0x1C0C0303,  //  0006  EQ	R3	R1	K3
      0x780E0002,  //  0007  JMPF	R3	#000B
      0xB80E0200,  //  0008  GETNGBL	R3	K1
      0x88080704,  //  0009  GETMBR	R2	R3	K4
      0x70020003,  //  000A  JMP		#000F
      0x1C0C0305,  //  000B  EQ	R3	R1	K5
      0x780E0001,  //  000C  JMPF	R3	#000F
      0xB80E0200,  //  000D  GETNGBL	R3	K1
      0x88080706,  //  000E  GETMBR	R2	R3	K6
      0x4C0C0000,  //  000F  LDNIL	R3
      0x200C0403,  //  0010  NE	R3	R2	R3
      0x780E0003,  //  0011  JMPF	R3	#0016
      0x880C0107,  //  0012  GETMBR	R3	R0	K7
      0x8C0C0708,  //  0013  GETMET	R3	R3	K8
      0x5C140400,  //  0014  MOVE	R5	R2
      0x7C0C0400,  //  0015  CALL	R3	2
      0x80000000,  //  0016  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: lvh_arc
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_class(lvh_arc,
    0,
    &be_class_lvh_obj,
    be_nested_map(12,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(set_line_width1, 1), be_const_closure(lvh_arc_set_line_width1_closure) },
        { be_const_key_weak(set_type, -1), be_const_closure(lvh_arc_set_type_closure) },
        { be_const_key_weak(set_max, -1), be_const_closure(lvh_arc_set_max_closure) },
        { be_const_key_weak(set_line_width, -1), be_const_closure(lvh_arc_set_line_width_closure) },
        { be_const_key_weak(get_type, 11), be_const_closure(lvh_arc_get_type_closure) },
        { be_const_key_weak(get_line_width1, -1), be_const_closure(lvh_arc_get_line_width1_closure) },
        { be_const_key_weak(_lv_part2_selector, -1), be_const_int(196608) },
        { be_const_key_weak(_lv_class, -1), be_const_class(be_class_lv_arc) },
        { be_const_key_weak(get_min, 5), be_const_closure(lvh_arc_get_min_closure) },
        { be_const_key_weak(get_line_width, 3), be_const_closure(lvh_arc_get_line_width_closure) },
        { be_const_key_weak(get_max, 2), be_const_closure(lvh_arc_get_max_closure) },
        { be_const_key_weak(set_min, -1), be_const_closure(lvh_arc_set_min_closure) },
    })),
    be_str_weak(lvh_arc)
);
/*******************************************************************/

void be_load_lvh_arc_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_lvh_arc);
    be_setglobal(vm, "lvh_arc");
    be_pop(vm, 1);
}

extern const bclass be_class_lvh_textarea;

/********************************************************************
** Solidified class: lvh_textarea
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_class(lvh_textarea,
    0,
    &be_class_lvh_obj,
    be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(_lv_class, -1), be_const_class(be_class_lv_textarea) },
    })),
    be_str_weak(lvh_textarea)
);
/*******************************************************************/

void be_load_lvh_textarea_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_lvh_textarea);
    be_setglobal(vm, "lvh_textarea");
    be_pop(vm, 1);
}

extern const bclass be_class_lvh_dropdown;

/********************************************************************
** Solidified function: get_val
********************************************************************/
be_local_closure(lvh_dropdown_get_val,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(_lv_obj),
    /* K1   */  be_nested_str_weak(get_selected),
    }),
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
** Solidified function: set_direction
********************************************************************/
be_local_closure(lvh_dropdown_set_direction,   /* name */
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
    /* K1   */  be_nested_str_weak(set_dir),
    /* K2   */  be_nested_str_weak(_dir),
    }),
    be_str_weak(set_direction),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x60100009,  //  0002  GETGBL	R4	G9
      0x5C140200,  //  0003  MOVE	R5	R1
      0x7C100200,  //  0004  CALL	R4	1
      0x88140102,  //  0005  GETMBR	R5	R0	K2
      0x94100A04,  //  0006  GETIDX	R4	R5	R4
      0x7C080400,  //  0007  CALL	R2	2
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_options
********************************************************************/
be_local_closure(lvh_dropdown_set_options,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(_lv_obj),
    /* K1   */  be_nested_str_weak(set_options),
    /* K2   */  be_nested_str_weak(lv),
    /* K3   */  be_nested_str_weak(ROLLER_MODE_NORMAL),
    }),
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
** Solidified function: set_val
********************************************************************/
be_local_closure(lvh_dropdown_set_val,   /* name */
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
    /* K1   */  be_nested_str_weak(set_selected),
    /* K2   */  be_const_int(0),
    }),
    be_str_weak(set_val),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5C100200,  //  0002  MOVE	R4	R1
      0x58140002,  //  0003  LDCONST	R5	K2
      0x7C080600,  //  0004  CALL	R2	3
      0x80000000,  //  0005  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_text
********************************************************************/
be_local_closure(lvh_dropdown_get_text,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(_lv_obj),
    /* K1   */  be_nested_str_weak(get_text),
    /* K2   */  be_nested_str_weak(resize),
    /* K3   */  be_nested_str_weak(get_selected_str),
    /* K4   */  be_nested_str_weak(_buffer),
    /* K5   */  be_nested_str_weak(remove_trailing_zeroes),
    /* K6   */  be_nested_str_weak(asstring),
    }),
    be_str_weak(get_text),
    &be_const_str_solidified,
    ( &(const binstruction[27]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x7C040200,  //  0002  CALL	R1	1
      0x4C080000,  //  0003  LDNIL	R2
      0x1C080202,  //  0004  EQ	R2	R1	R2
      0x780A0012,  //  0005  JMPF	R2	#0019
      0x60080015,  //  0006  GETGBL	R2	G21
      0x7C080000,  //  0007  CALL	R2	0
      0x8C080502,  //  0008  GETMET	R2	R2	K2
      0x541200FF,  //  0009  LDINT	R4	256
      0x7C080400,  //  000A  CALL	R2	2
      0x880C0100,  //  000B  GETMBR	R3	R0	K0
      0x8C0C0703,  //  000C  GETMET	R3	R3	K3
      0x8C140504,  //  000D  GETMET	R5	R2	K4
      0x7C140200,  //  000E  CALL	R5	1
      0x541A00FF,  //  000F  LDINT	R6	256
      0x7C0C0600,  //  0010  CALL	R3	3
      0x8C0C0105,  //  0011  GETMET	R3	R0	K5
      0x5C140400,  //  0012  MOVE	R5	R2
      0x7C0C0400,  //  0013  CALL	R3	2
      0x5C080600,  //  0014  MOVE	R2	R3
      0x8C0C0506,  //  0015  GETMET	R3	R2	K6
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
** Solidified function: get_direction
********************************************************************/
be_local_closure(lvh_dropdown_get_direction,   /* name */
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
    /* K0   */  be_nested_str_weak(_lv_obj),
    /* K1   */  be_nested_str_weak(get_dir),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str_weak(_dir),
    /* K4   */  be_const_int(1),
    }),
    be_str_weak(get_direction),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x7C040200,  //  0002  CALL	R1	1
      0x58080002,  //  0003  LDCONST	R2	K2
      0x600C000C,  //  0004  GETGBL	R3	G12
      0x88100103,  //  0005  GETMBR	R4	R0	K3
      0x7C0C0200,  //  0006  CALL	R3	1
      0x140C0403,  //  0007  LT	R3	R2	R3
      0x780E0006,  //  0008  JMPF	R3	#0010
      0x880C0103,  //  0009  GETMBR	R3	R0	K3
      0x940C0602,  //  000A  GETIDX	R3	R3	R2
      0x1C0C0203,  //  000B  EQ	R3	R1	R3
      0x780E0000,  //  000C  JMPF	R3	#000E
      0x80040400,  //  000D  RET	1	R2
      0x00080504,  //  000E  ADD	R2	R2	K4
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
be_local_closure(lvh_dropdown_set_show_selected,   /* name */
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
    /* K0   */  be_nested_str_weak(_lv_obj),
    /* K1   */  be_nested_str_weak(set_text),
    }),
    be_str_weak(set_show_selected),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x78060003,  //  0000  JMPF	R1	#0005
      0x88080100,  //  0001  GETMBR	R2	R0	K0
      0x8C080501,  //  0002  GETMET	R2	R2	K1
      0x4C100000,  //  0003  LDNIL	R4
      0x7C080400,  //  0004  CALL	R2	2
      0x80000000,  //  0005  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_text
********************************************************************/
be_local_closure(lvh_dropdown_set_text,   /* name */
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
    /* K0   */  be_nested_str_weak(_lv_obj),
    /* K1   */  be_nested_str_weak(set_text),
    }),
    be_str_weak(set_text),
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
** Solidified function: get_options
********************************************************************/
be_local_closure(lvh_dropdown_get_options,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(_lv_obj),
    /* K1   */  be_nested_str_weak(get_options),
    }),
    be_str_weak(get_options),
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
** Solidified function: get_show_selected
********************************************************************/
be_local_closure(lvh_dropdown_get_show_selected,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(_lv_obj),
    /* K1   */  be_nested_str_weak(get_text),
    }),
    be_str_weak(get_show_selected),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
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
    0,
    &be_class_lvh_obj,
    be_nested_map(12,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(get_show_selected, 8), be_const_closure(lvh_dropdown_get_show_selected_closure) },
        { be_const_key_weak(set_direction, -1), be_const_closure(lvh_dropdown_set_direction_closure) },
        { be_const_key_weak(set_options, -1), be_const_closure(lvh_dropdown_set_options_closure) },
        { be_const_key_weak(set_val, 4), be_const_closure(lvh_dropdown_set_val_closure) },
        { be_const_key_weak(get_text, -1), be_const_closure(lvh_dropdown_get_text_closure) },
        { be_const_key_weak(get_direction, 9), be_const_closure(lvh_dropdown_get_direction_closure) },
        { be_const_key_weak(_lv_class, -1), be_const_class(be_class_lv_dropdown) },
        { be_const_key_weak(set_show_selected, 6), be_const_closure(lvh_dropdown_set_show_selected_closure) },
        { be_const_key_weak(set_text, -1), be_const_closure(lvh_dropdown_set_text_closure) },
        { be_const_key_weak(_dir, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(4,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(8),
        be_const_int(4),
        be_const_int(1),
        be_const_int(2),
    }))    ) } )) },
        { be_const_key_weak(get_options, -1), be_const_closure(lvh_dropdown_get_options_closure) },
        { be_const_key_weak(get_val, 0), be_const_closure(lvh_dropdown_get_val_closure) },
    })),
    be_str_weak(lvh_dropdown)
);
/*******************************************************************/

void be_load_lvh_dropdown_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_lvh_dropdown);
    be_setglobal(vm, "lvh_dropdown");
    be_pop(vm, 1);
}

extern const bclass be_class_lvh_qrcode;

/********************************************************************
** Solidified function: get_qr_size
********************************************************************/
be_local_closure(lvh_qrcode_get_qr_size,   /* name */
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
    be_str_weak(get_qr_size),
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
be_local_closure(lvh_qrcode_set_qr_size,   /* name */
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
    be_str_weak(set_qr_size),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_qr_light_color
********************************************************************/
be_local_closure(lvh_qrcode_get_qr_light_color,   /* name */
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
    be_str_weak(get_qr_light_color),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_qr_dark_color
********************************************************************/
be_local_closure(lvh_qrcode_get_qr_dark_color,   /* name */
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
    be_str_weak(get_qr_dark_color),
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
be_local_closure(lvh_qrcode_init,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[12]) {     /* constants */
    /* K0   */  be_nested_str_weak(_page),
    /* K1   */  be_nested_str_weak(find),
    /* K2   */  be_nested_str_weak(qr_size),
    /* K3   */  be_nested_str_weak(parse_color),
    /* K4   */  be_nested_str_weak(qr_dark_color),
    /* K5   */  be_nested_str_weak(_X23000000),
    /* K6   */  be_nested_str_weak(qr_light_color),
    /* K7   */  be_nested_str_weak(_X23FFFFFF),
    /* K8   */  be_nested_str_weak(_lv_obj),
    /* K9   */  be_nested_str_weak(lv),
    /* K10  */  be_nested_str_weak(qrcode),
    /* K11  */  be_nested_str_weak(post_init),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[28]) {  /* code */
      0x90020002,  //  0000  SETMBR	R0	K0	R2
      0x8C100701,  //  0001  GETMET	R4	R3	K1
      0x58180002,  //  0002  LDCONST	R6	K2
      0x541E0063,  //  0003  LDINT	R7	100
      0x7C100600,  //  0004  CALL	R4	3
      0x8C140103,  //  0005  GETMET	R5	R0	K3
      0x8C1C0701,  //  0006  GETMET	R7	R3	K1
      0x58240004,  //  0007  LDCONST	R9	K4
      0x58280005,  //  0008  LDCONST	R10	K5
      0x7C1C0600,  //  0009  CALL	R7	3
      0x7C140400,  //  000A  CALL	R5	2
      0x8C180103,  //  000B  GETMET	R6	R0	K3
      0x8C200701,  //  000C  GETMET	R8	R3	K1
      0x58280006,  //  000D  LDCONST	R10	K6
      0x582C0007,  //  000E  LDCONST	R11	K7
      0x7C200600,  //  000F  CALL	R8	3
      0x7C180400,  //  0010  CALL	R6	2
      0xB81E1200,  //  0011  GETNGBL	R7	K9
      0x8C1C0F0A,  //  0012  GETMET	R7	R7	K10
      0x5C240200,  //  0013  MOVE	R9	R1
      0x5C280800,  //  0014  MOVE	R10	R4
      0x5C2C0A00,  //  0015  MOVE	R11	R5
      0x5C300C00,  //  0016  MOVE	R12	R6
      0x7C1C0A00,  //  0017  CALL	R7	5
      0x90021007,  //  0018  SETMBR	R0	K8	R7
      0x8C1C010B,  //  0019  GETMET	R7	R0	K11
      0x7C1C0200,  //  001A  CALL	R7	1
      0x80000000,  //  001B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_qr_light_color
********************************************************************/
be_local_closure(lvh_qrcode_set_qr_light_color,   /* name */
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
    be_str_weak(set_qr_light_color),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_qr_text
********************************************************************/
be_local_closure(lvh_qrcode_get_qr_text,   /* name */
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
    be_str_weak(get_qr_text),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_qr_dark_color
********************************************************************/
be_local_closure(lvh_qrcode_set_qr_dark_color,   /* name */
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
    be_str_weak(set_qr_dark_color),
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
be_local_closure(lvh_qrcode_set_qr_text,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(_lv_obj),
    /* K1   */  be_nested_str_weak(update),
    }),
    be_str_weak(set_qr_text),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x60080008,  //  0000  GETGBL	R2	G8
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x7C080200,  //  0002  CALL	R2	1
      0x5C040400,  //  0003  MOVE	R1	R2
      0x88080100,  //  0004  GETMBR	R2	R0	K0
      0x8C080501,  //  0005  GETMET	R2	R2	K1
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
** Solidified class: lvh_qrcode
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_class(lvh_qrcode,
    0,
    &be_class_lvh_obj,
    be_nested_map(10,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(set_qr_text, -1), be_const_closure(lvh_qrcode_set_qr_text_closure) },
        { be_const_key_weak(set_qr_size, -1), be_const_closure(lvh_qrcode_set_qr_size_closure) },
        { be_const_key_weak(get_qr_light_color, 6), be_const_closure(lvh_qrcode_get_qr_light_color_closure) },
        { be_const_key_weak(set_qr_dark_color, -1), be_const_closure(lvh_qrcode_set_qr_dark_color_closure) },
        { be_const_key_weak(get_qr_text, -1), be_const_closure(lvh_qrcode_get_qr_text_closure) },
        { be_const_key_weak(init, -1), be_const_closure(lvh_qrcode_init_closure) },
        { be_const_key_weak(set_qr_light_color, -1), be_const_closure(lvh_qrcode_set_qr_light_color_closure) },
        { be_const_key_weak(_lv_class, 4), be_const_class(be_class_lv_qrcode) },
        { be_const_key_weak(get_qr_dark_color, 3), be_const_closure(lvh_qrcode_get_qr_dark_color_closure) },
        { be_const_key_weak(get_qr_size, 0), be_const_closure(lvh_qrcode_get_qr_size_closure) },
    })),
    be_str_weak(lvh_qrcode)
);
/*******************************************************************/

void be_load_lvh_qrcode_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_lvh_qrcode);
    be_setglobal(vm, "lvh_qrcode");
    be_pop(vm, 1);
}

extern const bclass be_class_HASPmota;

/********************************************************************
** Solidified function: do_action
********************************************************************/
be_local_closure(HASPmota_do_action,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[14]) {     /* constants */
    /* K0   */  be_nested_str_weak(lv),
    /* K1   */  be_nested_str_weak(EVENT_CLICKED),
    /* K2   */  be_nested_str_weak(_action),
    /* K3   */  be_nested_str_weak(lvh_pages),
    /* K4   */  be_nested_str_weak(lvh_page_cur_idx),
    /* K5   */  be_nested_str_weak(pages_list_sorted),
    /* K6   */  be_const_int(1),
    /* K7   */  be_nested_str_weak(prev),
    /* K8   */  be_nested_str_weak(next),
    /* K9   */  be_nested_str_weak(back),
    /* K10  */  be_nested_str_weak(re_page_target),
    /* K11  */  be_nested_str_weak(match),
    /* K12  */  be_const_int(0),
    /* K13  */  be_nested_str_weak(show),
    }),
    be_str_weak(do_action),
    &be_const_str_solidified,
    ( &(const binstruction[74]) {  /* code */
      0xB80E0000,  //  0000  GETNGBL	R3	K0
      0x880C0701,  //  0001  GETMBR	R3	R3	K1
      0x200C0403,  //  0002  NE	R3	R2	R3
      0x780E0000,  //  0003  JMPF	R3	#0005
      0x80000600,  //  0004  RET	0
      0x880C0302,  //  0005  GETMBR	R3	R1	K2
      0x88100103,  //  0006  GETMBR	R4	R0	K3
      0x88140104,  //  0007  GETMBR	R5	R0	K4
      0x94100805,  //  0008  GETIDX	R4	R4	R5
      0x4C140000,  //  0009  LDNIL	R5
      0x8C180105,  //  000A  GETMET	R6	R0	K5
      0x88200104,  //  000B  GETMBR	R8	R0	K4
      0x7C180400,  //  000C  CALL	R6	2
      0x601C000C,  //  000D  GETGBL	R7	G12
      0x5C200C00,  //  000E  MOVE	R8	R6
      0x7C1C0200,  //  000F  CALL	R7	1
      0x181C0F06,  //  0010  LE	R7	R7	K6
      0x781E0000,  //  0011  JMPF	R7	#0013
      0x80000E00,  //  0012  RET	0
      0x1C1C0707,  //  0013  EQ	R7	R3	K7
      0x781E0009,  //  0014  JMPF	R7	#001F
      0x601C0009,  //  0015  GETGBL	R7	G9
      0x88200907,  //  0016  GETMBR	R8	R4	K7
      0x7C1C0200,  //  0017  CALL	R7	1
      0x5C140E00,  //  0018  MOVE	R5	R7
      0x4C1C0000,  //  0019  LDNIL	R7
      0x1C1C0A07,  //  001A  EQ	R7	R5	R7
      0x781E0001,  //  001B  JMPF	R7	#001E
      0x541DFFFE,  //  001C  LDINT	R7	-1
      0x94140C07,  //  001D  GETIDX	R5	R6	R7
      0x70020020,  //  001E  JMP		#0040
      0x1C1C0708,  //  001F  EQ	R7	R3	K8
      0x781E0008,  //  0020  JMPF	R7	#002A
      0x601C0009,  //  0021  GETGBL	R7	G9
      0x88200908,  //  0022  GETMBR	R8	R4	K8
      0x7C1C0200,  //  0023  CALL	R7	1
      0x5C140E00,  //  0024  MOVE	R5	R7
      0x4C1C0000,  //  0025  LDNIL	R7
      0x1C1C0A07,  //  0026  EQ	R7	R5	R7
      0x781E0000,  //  0027  JMPF	R7	#0029
      0x94140D06,  //  0028  GETIDX	R5	R6	K6
      0x70020015,  //  0029  JMP		#0040
      0x1C1C0709,  //  002A  EQ	R7	R3	K9
      0x781E0008,  //  002B  JMPF	R7	#0035
      0x601C0009,  //  002C  GETGBL	R7	G9
      0x88200909,  //  002D  GETMBR	R8	R4	K9
      0x7C1C0200,  //  002E  CALL	R7	1
      0x5C140E00,  //  002F  MOVE	R5	R7
      0x4C1C0000,  //  0030  LDNIL	R7
      0x1C1C0A07,  //  0031  EQ	R7	R5	R7
      0x781E0000,  //  0032  JMPF	R7	#0034
      0x58140006,  //  0033  LDCONST	R5	K6
      0x7002000A,  //  0034  JMP		#0040
      0x881C010A,  //  0035  GETMBR	R7	R0	K10
      0x8C1C0F0B,  //  0036  GETMET	R7	R7	K11
      0x5C240600,  //  0037  MOVE	R9	R3
      0x7C1C0400,  //  0038  CALL	R7	2
      0x781E0005,  //  0039  JMPF	R7	#0040
      0x601C0009,  //  003A  GETGBL	R7	G9
      0x5421FFFE,  //  003B  LDINT	R8	-1
      0x40220C08,  //  003C  CONNECT	R8	K6	R8
      0x94200608,  //  003D  GETIDX	R8	R3	R8
      0x7C1C0200,  //  003E  CALL	R7	1
      0x5C140E00,  //  003F  MOVE	R5	R7
      0x4C1C0000,  //  0040  LDNIL	R7
      0x201C0A07,  //  0041  NE	R7	R5	R7
      0x781E0005,  //  0042  JMPF	R7	#0049
      0x241C0B0C,  //  0043  GT	R7	R5	K12
      0x781E0003,  //  0044  JMPF	R7	#0049
      0x881C0103,  //  0045  GETMBR	R7	R0	K3
      0x941C0E05,  //  0046  GETIDX	R7	R7	R5
      0x8C1C0F0D,  //  0047  GETMET	R7	R7	K13
      0x7C1C0200,  //  0048  CALL	R7	1
      0x80000000,  //  0049  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: pages_list_sorted
********************************************************************/
be_local_closure(HASPmota_pages_list_sorted,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(lvh_page_cur_idx),
    /* K2   */  be_nested_str_weak(lvh_pages),
    /* K3   */  be_nested_str_weak(keys),
    /* K4   */  be_nested_str_weak(push),
    /* K5   */  be_nested_str_weak(stop_iteration),
    /* K6   */  be_nested_str_weak(sort),
    /* K7   */  be_nested_str_weak(find),
    /* K8   */  be_const_int(1),
    }),
    be_str_weak(pages_list_sorted),
    &be_const_str_solidified,
    ( &(const binstruction[47]) {  /* code */
      0x60080012,  //  0000  GETGBL	R2	G18
      0x7C080000,  //  0001  CALL	R2	0
      0x1C0C0300,  //  0002  EQ	R3	R1	K0
      0x780E0000,  //  0003  JMPF	R3	#0005
      0x88040101,  //  0004  GETMBR	R1	R0	K1
      0x600C0010,  //  0005  GETGBL	R3	G16
      0x88100102,  //  0006  GETMBR	R4	R0	K2
      0x8C100903,  //  0007  GETMET	R4	R4	K3
      0x7C100200,  //  0008  CALL	R4	1
      0x7C0C0200,  //  0009  CALL	R3	1
      0xA8020007,  //  000A  EXBLK	0	#0013
      0x5C100600,  //  000B  MOVE	R4	R3
      0x7C100000,  //  000C  CALL	R4	0
      0x20140900,  //  000D  NE	R5	R4	K0
      0x78160002,  //  000E  JMPF	R5	#0012
      0x8C140504,  //  000F  GETMET	R5	R2	K4
      0x5C1C0800,  //  0010  MOVE	R7	R4
      0x7C140400,  //  0011  CALL	R5	2
      0x7001FFF7,  //  0012  JMP		#000B
      0x580C0005,  //  0013  LDCONST	R3	K5
      0xAC0C0200,  //  0014  CATCH	R3	1	0
      0xB0080000,  //  0015  RAISE	2	R0	R0
      0x8C0C0106,  //  0016  GETMET	R3	R0	K6
      0x5C140400,  //  0017  MOVE	R5	R2
      0x7C0C0400,  //  0018  CALL	R3	2
      0x5C080600,  //  0019  MOVE	R2	R3
      0x4C0C0000,  //  001A  LDNIL	R3
      0x1C0C0203,  //  001B  EQ	R3	R1	R3
      0x780E0000,  //  001C  JMPF	R3	#001E
      0x80040200,  //  001D  RET	1	R1
      0x600C000C,  //  001E  GETGBL	R3	G12
      0x5C100400,  //  001F  MOVE	R4	R2
      0x7C0C0200,  //  0020  CALL	R3	1
      0x00080402,  //  0021  ADD	R2	R2	R2
      0x8C100507,  //  0022  GETMET	R4	R2	K7
      0x5C180200,  //  0023  MOVE	R6	R1
      0x7C100400,  //  0024  CALL	R4	2
      0x4C140000,  //  0025  LDNIL	R5
      0x1C140805,  //  0026  EQ	R5	R4	R5
      0x78160001,  //  0027  JMPF	R5	#002A
      0x4C140000,  //  0028  LDNIL	R5
      0x80040A00,  //  0029  RET	1	R5
      0x00140803,  //  002A  ADD	R5	R4	R3
      0x04140B08,  //  002B  SUB	R5	R5	K8
      0x40140805,  //  002C  CONNECT	R5	R4	R5
      0x94080405,  //  002D  GETIDX	R2	R2	R5
      0x80040400,  //  002E  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_page_cur
********************************************************************/
be_local_closure(HASPmota_get_page_cur,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(lvh_pages),
    /* K1   */  be_nested_str_weak(lvh_page_cur_idx),
    }),
    be_str_weak(get_page_cur),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x88080101,  //  0001  GETMBR	R2	R0	K1
      0x94040202,  //  0002  GETIDX	R1	R1	R2
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start
********************************************************************/
be_local_closure(HASPmota_start,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[33]) {     /* constants */
    /* K0   */  be_nested_str_weak(path),
    /* K1   */  be_nested_str_weak(def_templ_name),
    /* K2   */  be_nested_str_weak(exists),
    /* K3   */  be_nested_str_weak(file_X20_X27),
    /* K4   */  be_nested_str_weak(_X27_X20not_X20found),
    /* K5   */  be_nested_str_weak(io_erorr),
    /* K6   */  be_nested_str_weak(lv),
    /* K7   */  be_nested_str_weak(start),
    /* K8   */  be_nested_str_weak(dark),
    /* K9   */  be_nested_str_weak(hres),
    /* K10  */  be_nested_str_weak(get_hor_res),
    /* K11  */  be_nested_str_weak(vres),
    /* K12  */  be_nested_str_weak(get_ver_res),
    /* K13  */  be_nested_str_weak(scr),
    /* K14  */  be_nested_str_weak(scr_act),
    /* K15  */  be_nested_str_weak(r16),
    /* K16  */  be_nested_str_weak(font_embedded),
    /* K17  */  be_nested_str_weak(robotocondensed),
    /* K18  */  be_nested_str_weak(montserrat),
    /* K19  */  be_nested_str_weak(theme_haspmota_init),
    /* K20  */  be_const_int(0),
    /* K21  */  be_nested_str_weak(color),
    /* K22  */  be_const_int(16711935),
    /* K23  */  be_const_int(3158064),
    /* K24  */  be_nested_str_weak(get_disp),
    /* K25  */  be_nested_str_weak(set_theme),
    /* K26  */  be_nested_str_weak(set_style_bg_color),
    /* K27  */  be_const_int(16777215),
    /* K28  */  be_nested_str_weak(theme_apply),
    /* K29  */  be_nested_str_weak(layer_top),
    /* K30  */  be_nested_str_weak(set_style_bg_opa),
    /* K31  */  be_nested_str_weak(lvh_pages),
    /* K32  */  be_nested_str_weak(_load),
    }),
    be_str_weak(start),
    &be_const_str_solidified,
    ( &(const binstruction[105]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0x4C100000,  //  0001  LDNIL	R4
      0x1C100404,  //  0002  EQ	R4	R2	R4
      0x78120000,  //  0003  JMPF	R4	#0005
      0x88080101,  //  0004  GETMBR	R2	R0	K1
      0x8C100702,  //  0005  GETMET	R4	R3	K2
      0x5C180400,  //  0006  MOVE	R6	R2
      0x7C100400,  //  0007  CALL	R4	2
      0x74120002,  //  0008  JMPT	R4	#000C
      0x00120602,  //  0009  ADD	R4	K3	R2
      0x00100904,  //  000A  ADD	R4	R4	K4
      0xB0060A04,  //  000B  RAISE	1	K5	R4
      0xB8120C00,  //  000C  GETNGBL	R4	K6
      0x8C100907,  //  000D  GETMET	R4	R4	K7
      0x7C100200,  //  000E  CALL	R4	1
      0x60100017,  //  000F  GETGBL	R4	G23
      0x5C140200,  //  0010  MOVE	R5	R1
      0x7C100200,  //  0011  CALL	R4	1
      0x90021004,  //  0012  SETMBR	R0	K8	R4
      0xB8120C00,  //  0013  GETNGBL	R4	K6
      0x8C10090A,  //  0014  GETMET	R4	R4	K10
      0x7C100200,  //  0015  CALL	R4	1
      0x90021204,  //  0016  SETMBR	R0	K9	R4
      0xB8120C00,  //  0017  GETNGBL	R4	K6
      0x8C10090C,  //  0018  GETMET	R4	R4	K12
      0x7C100200,  //  0019  CALL	R4	1
      0x90021604,  //  001A  SETMBR	R0	K11	R4
      0xB8120C00,  //  001B  GETNGBL	R4	K6
      0x8C10090E,  //  001C  GETMET	R4	R4	K14
      0x7C100200,  //  001D  CALL	R4	1
      0x90021A04,  //  001E  SETMBR	R0	K13	R4
      0xA8020007,  //  001F  EXBLK	0	#0028
      0xB8120C00,  //  0020  GETNGBL	R4	K6
      0x8C100910,  //  0021  GETMET	R4	R4	K16
      0x58180011,  //  0022  LDCONST	R6	K17
      0x541E000F,  //  0023  LDINT	R7	16
      0x7C100600,  //  0024  CALL	R4	3
      0x90021E04,  //  0025  SETMBR	R0	K15	R4
      0xA8040001,  //  0026  EXBLK	1	1
      0x70020009,  //  0027  JMP		#0032
      0xAC100000,  //  0028  CATCH	R4	0	0
      0x70020006,  //  0029  JMP		#0031
      0xB8120C00,  //  002A  GETNGBL	R4	K6
      0x8C100910,  //  002B  GETMET	R4	R4	K16
      0x58180012,  //  002C  LDCONST	R6	K18
      0x541E000D,  //  002D  LDINT	R7	14
      0x7C100600,  //  002E  CALL	R4	3
      0x90021E04,  //  002F  SETMBR	R0	K15	R4
      0x70020000,  //  0030  JMP		#0032
      0xB0080000,  //  0031  RAISE	2	R0	R0
      0xB8120C00,  //  0032  GETNGBL	R4	K6
      0x8C100913,  //  0033  GETMET	R4	R4	K19
      0x58180014,  //  0034  LDCONST	R6	K20
      0xB81E0C00,  //  0035  GETNGBL	R7	K6
      0x8C1C0F15,  //  0036  GETMET	R7	R7	K21
      0x58240016,  //  0037  LDCONST	R9	K22
      0x7C1C0400,  //  0038  CALL	R7	2
      0xB8220C00,  //  0039  GETNGBL	R8	K6
      0x8C201115,  //  003A  GETMET	R8	R8	K21
      0x58280017,  //  003B  LDCONST	R10	K23
      0x7C200400,  //  003C  CALL	R8	2
      0x88240108,  //  003D  GETMBR	R9	R0	K8
      0x8828010F,  //  003E  GETMBR	R10	R0	K15
      0x7C100C00,  //  003F  CALL	R4	6
      0x8814010D,  //  0040  GETMBR	R5	R0	K13
      0x8C140B18,  //  0041  GETMET	R5	R5	K24
      0x7C140200,  //  0042  CALL	R5	1
      0x8C140B19,  //  0043  GETMET	R5	R5	K25
      0x5C1C0800,  //  0044  MOVE	R7	R4
      0x7C140400,  //  0045  CALL	R5	2
      0x8814010D,  //  0046  GETMBR	R5	R0	K13
      0x8C140B1A,  //  0047  GETMET	R5	R5	K26
      0x881C0108,  //  0048  GETMBR	R7	R0	K8
      0x781E0004,  //  0049  JMPF	R7	#004F
      0xB81E0C00,  //  004A  GETNGBL	R7	K6
      0x8C1C0F15,  //  004B  GETMET	R7	R7	K21
      0x58240014,  //  004C  LDCONST	R9	K20
      0x7C1C0400,  //  004D  CALL	R7	2
      0x70020003,  //  004E  JMP		#0053
      0xB81E0C00,  //  004F  GETNGBL	R7	K6
      0x8C1C0F15,  //  0050  GETMET	R7	R7	K21
      0x5824001B,  //  0051  LDCONST	R9	K27
      0x7C1C0400,  //  0052  CALL	R7	2
      0x58200014,  //  0053  LDCONST	R8	K20
      0x7C140600,  //  0054  CALL	R5	3
      0xB8160C00,  //  0055  GETNGBL	R5	K6
      0x8C140B1C,  //  0056  GETMET	R5	R5	K28
      0xB81E0C00,  //  0057  GETNGBL	R7	K6
      0x8C1C0F1D,  //  0058  GETMET	R7	R7	K29
      0x7C1C0200,  //  0059  CALL	R7	1
      0x7C140400,  //  005A  CALL	R5	2
      0xB8160C00,  //  005B  GETNGBL	R5	K6
      0x8C140B1D,  //  005C  GETMET	R5	R5	K29
      0x7C140200,  //  005D  CALL	R5	1
      0x8C140B1E,  //  005E  GETMET	R5	R5	K30
      0x581C0014,  //  005F  LDCONST	R7	K20
      0x58200014,  //  0060  LDCONST	R8	K20
      0x7C140600,  //  0061  CALL	R5	3
      0x60140013,  //  0062  GETGBL	R5	G19
      0x7C140000,  //  0063  CALL	R5	0
      0x90023E05,  //  0064  SETMBR	R0	K31	R5
      0x8C140120,  //  0065  GETMET	R5	R0	K32
      0x5C1C0400,  //  0066  MOVE	R7	R2
      0x7C140400,  //  0067  CALL	R5	2
      0x80000000,  //  0068  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(HASPmota_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(re),
    /* K1   */  be_nested_str_weak(re_page_target),
    /* K2   */  be_nested_str_weak(compile),
    /* K3   */  be_nested_str_weak(p_X5Cd_X2B),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080302,  //  0001  GETMET	R2	R1	K2
      0x58100003,  //  0002  LDCONST	R4	K3
      0x7C080400,  //  0003  CALL	R2	2
      0x90020202,  //  0004  SETMBR	R0	K1	R2
      0x80000000,  //  0005  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: register_event
********************************************************************/
be_local_closure(HASPmota_register_event,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
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
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(cb),
    /* K1   */  be_nested_str_weak(introspect),
    /* K2   */  be_nested_str_weak(event_cb),
    /* K3   */  be_nested_str_weak(gen_cb),
    /* K4   */  be_nested_str_weak(_lv_obj),
    /* K5   */  be_nested_str_weak(add_event_cb),
    /* K6   */  be_nested_str_weak(toptr),
    }),
    be_str_weak(register_event),
    &be_const_str_solidified,
    ( &(const binstruction[20]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xA4120200,  //  0001  IMPORT	R4	K1
      0x88140102,  //  0002  GETMBR	R5	R0	K2
      0x4C180000,  //  0003  LDNIL	R6
      0x1C140A06,  //  0004  EQ	R5	R5	R6
      0x78160003,  //  0005  JMPF	R5	#000A
      0x8C140703,  //  0006  GETMET	R5	R3	K3
      0x841C0000,  //  0007  CLOSURE	R7	P0
      0x7C140400,  //  0008  CALL	R5	2
      0x90020405,  //  0009  SETMBR	R0	K2	R5
      0x88140304,  //  000A  GETMBR	R5	R1	K4
      0x8C180B05,  //  000B  GETMET	R6	R5	K5
      0x88200102,  //  000C  GETMBR	R8	R0	K2
      0x5C240400,  //  000D  MOVE	R9	R2
      0x8C280906,  //  000E  GETMET	R10	R4	K6
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
** Solidified function: parse_page
********************************************************************/
be_local_closure(HASPmota_parse_page,   /* name */
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
    /* K0   */  be_nested_str_weak(has),
    /* K1   */  be_nested_str_weak(page),
    /* K2   */  be_nested_str_weak(int),
    /* K3   */  be_nested_str_weak(lvh_page_cur_idx),
    /* K4   */  be_nested_str_weak(lvh_pages),
    /* K5   */  be_nested_str_weak(contains),
    /* K6   */  be_nested_str_weak(lvh_page),
    /* K7   */  be_nested_str_weak(find),
    /* K8   */  be_nested_str_weak(id),
    /* K9   */  be_const_int(0),
    /* K10  */  be_nested_str_weak(get_page_cur),
    /* K11  */  be_nested_str_weak(prev),
    /* K12  */  be_nested_str_weak(next),
    /* K13  */  be_nested_str_weak(back),
    }),
    be_str_weak(parse_page),
    &be_const_str_solidified,
    ( &(const binstruction[54]) {  /* code */
      0x8C080300,  //  0000  GETMET	R2	R1	K0
      0x58100001,  //  0001  LDCONST	R4	K1
      0x7C080400,  //  0002  CALL	R2	2
      0x780A0030,  //  0003  JMPF	R2	#0035
      0x60080004,  //  0004  GETGBL	R2	G4
      0x940C0301,  //  0005  GETIDX	R3	R1	K1
      0x7C080200,  //  0006  CALL	R2	1
      0x1C080502,  //  0007  EQ	R2	R2	K2
      0x780A002B,  //  0008  JMPF	R2	#0035
      0x60080009,  //  0009  GETGBL	R2	G9
      0x940C0301,  //  000A  GETIDX	R3	R1	K1
      0x7C080200,  //  000B  CALL	R2	1
      0x90020602,  //  000C  SETMBR	R0	K3	R2
      0x880C0104,  //  000D  GETMBR	R3	R0	K4
      0x8C0C0705,  //  000E  GETMET	R3	R3	K5
      0x5C140400,  //  000F  MOVE	R5	R2
      0x7C0C0400,  //  0010  CALL	R3	2
      0x740E0006,  //  0011  JMPT	R3	#0019
      0x880C0106,  //  0012  GETMBR	R3	R0	K6
      0x88100104,  //  0013  GETMBR	R4	R0	K4
      0x5C140600,  //  0014  MOVE	R5	R3
      0x5C180400,  //  0015  MOVE	R6	R2
      0x5C1C0000,  //  0016  MOVE	R7	R0
      0x7C140400,  //  0017  CALL	R5	2
      0x98100405,  //  0018  SETIDX	R4	R2	R5
      0x8C0C0307,  //  0019  GETMET	R3	R1	K7
      0x58140008,  //  001A  LDCONST	R5	K8
      0x7C0C0400,  //  001B  CALL	R3	2
      0x1C0C0709,  //  001C  EQ	R3	R3	K9
      0x780E0016,  //  001D  JMPF	R3	#0035
      0x8C0C010A,  //  001E  GETMET	R3	R0	K10
      0x7C0C0200,  //  001F  CALL	R3	1
      0x60100009,  //  0020  GETGBL	R4	G9
      0x8C140307,  //  0021  GETMET	R5	R1	K7
      0x581C000B,  //  0022  LDCONST	R7	K11
      0x4C200000,  //  0023  LDNIL	R8
      0x7C140600,  //  0024  CALL	R5	3
      0x7C100200,  //  0025  CALL	R4	1
      0x900E1604,  //  0026  SETMBR	R3	K11	R4
      0x60100009,  //  0027  GETGBL	R4	G9
      0x8C140307,  //  0028  GETMET	R5	R1	K7
      0x581C000C,  //  0029  LDCONST	R7	K12
      0x4C200000,  //  002A  LDNIL	R8
      0x7C140600,  //  002B  CALL	R5	3
      0x7C100200,  //  002C  CALL	R4	1
      0x900E1804,  //  002D  SETMBR	R3	K12	R4
      0x60100009,  //  002E  GETGBL	R4	G9
      0x8C140307,  //  002F  GETMET	R5	R1	K7
      0x581C000D,  //  0030  LDCONST	R7	K13
      0x4C200000,  //  0031  LDNIL	R8
      0x7C140600,  //  0032  CALL	R5	3
      0x7C100200,  //  0033  CALL	R4	1
      0x900E1A04,  //  0034  SETMBR	R3	K13	R4
      0x80000000,  //  0035  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: deinit
********************************************************************/
be_local_closure(HASPmota_deinit,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(_val_rule),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(remove_rule),
    /* K3   */  be_nested_str_weak(_text_rule),
    }),
    be_str_weak(deinit),
    &be_const_str_solidified,
    ( &(const binstruction[19]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x4C080000,  //  0001  LDNIL	R2
      0x20040202,  //  0002  NE	R1	R1	R2
      0x78060004,  //  0003  JMPF	R1	#0009
      0xB8060200,  //  0004  GETNGBL	R1	K1
      0x8C040302,  //  0005  GETMET	R1	R1	K2
      0x880C0100,  //  0006  GETMBR	R3	R0	K0
      0x5C100000,  //  0007  MOVE	R4	R0
      0x7C040600,  //  0008  CALL	R1	3
      0x88040103,  //  0009  GETMBR	R1	R0	K3
      0x4C080000,  //  000A  LDNIL	R2
      0x20040202,  //  000B  NE	R1	R1	R2
      0x78060004,  //  000C  JMPF	R1	#0012
      0xB8060200,  //  000D  GETNGBL	R1	K1
      0x8C040302,  //  000E  GETMET	R1	R1	K2
      0x880C0103,  //  000F  GETMBR	R3	R0	K3
      0x5C100000,  //  0010  MOVE	R4	R0
      0x7C040600,  //  0011  CALL	R1	3
      0x80000000,  //  0012  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: page_dir_to
********************************************************************/
be_local_closure(HASPmota_page_dir_to,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(pages_list_sorted),
    /* K1   */  be_const_int(0),
    /* K2   */  be_const_int(1),
    /* K3   */  be_const_int(2),
    /* K4   */  be_nested_str_weak(find),
    }),
    be_str_weak(page_dir_to),
    &be_const_str_solidified,
    ( &(const binstruction[32]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x58100001,  //  0001  LDCONST	R4	K1
      0x7C080400,  //  0002  CALL	R2	2
      0x4C0C0000,  //  0003  LDNIL	R3
      0x1C0C0403,  //  0004  EQ	R3	R2	R3
      0x780E0000,  //  0005  JMPF	R3	#0007
      0x80060200,  //  0006  RET	1	K1
      0x600C000C,  //  0007  GETGBL	R3	G12
      0x5C100400,  //  0008  MOVE	R4	R2
      0x7C0C0200,  //  0009  CALL	R3	1
      0x18100702,  //  000A  LE	R4	R3	K2
      0x78120000,  //  000B  JMPF	R4	#000D
      0x80060200,  //  000C  RET	1	K1
      0x1C100703,  //  000D  EQ	R4	R3	K3
      0x78120000,  //  000E  JMPF	R4	#0010
      0x80060400,  //  000F  RET	1	K2
      0x8C100504,  //  0010  GETMET	R4	R2	K4
      0x5C180200,  //  0011  MOVE	R6	R1
      0x7C100400,  //  0012  CALL	R4	2
      0x4C140000,  //  0013  LDNIL	R5
      0x1C140805,  //  0014  EQ	R5	R4	R5
      0x78160000,  //  0015  JMPF	R5	#0017
      0x80060200,  //  0016  RET	1	K1
      0x00140702,  //  0017  ADD	R5	R3	K2
      0x0C140B03,  //  0018  DIV	R5	R5	K3
      0x18140805,  //  0019  LE	R5	R4	R5
      0x78160001,  //  001A  JMPF	R5	#001D
      0x80060400,  //  001B  RET	1	K2
      0x70020001,  //  001C  JMP		#001F
      0x5415FFFE,  //  001D  LDINT	R5	-1
      0x80040A00,  //  001E  RET	1	R5
      0x80000000,  //  001F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _load
********************************************************************/
be_local_closure(HASPmota__load,   /* name */
  be_nested_proto(
    14,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[17]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(json),
    /* K2   */  be_nested_str_weak(lvh_page_cur_idx),
    /* K3   */  be_const_int(1),
    /* K4   */  be_nested_str_weak(lvh_page),
    /* K5   */  be_nested_str_weak(lvh_pages),
    /* K6   */  be_nested_str_weak(r),
    /* K7   */  be_nested_str_weak(read),
    /* K8   */  be_nested_str_weak(close),
    /* K9   */  be_nested_str_weak(split),
    /* K10  */  be_nested_str_weak(_X0A),
    /* K11  */  be_const_int(0),
    /* K12  */  be_nested_str_weak(load),
    /* K13  */  be_nested_str_weak(instance),
    /* K14  */  be_nested_str_weak(parse_page),
    /* K15  */  be_nested_str_weak(parse_obj),
    /* K16  */  be_nested_str_weak(remove),
    }),
    be_str_weak(_load),
    &be_const_str_solidified,
    ( &(const binstruction[54]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0xA40E0200,  //  0001  IMPORT	R3	K1
      0x90020503,  //  0002  SETMBR	R0	K2	K3
      0x88100104,  //  0003  GETMBR	R4	R0	K4
      0x88140105,  //  0004  GETMBR	R5	R0	K5
      0x5C180800,  //  0005  MOVE	R6	R4
      0x581C0003,  //  0006  LDCONST	R7	K3
      0x5C200000,  //  0007  MOVE	R8	R0
      0x7C180400,  //  0008  CALL	R6	2
      0x98160606,  //  0009  SETIDX	R5	K3	R6
      0x60140011,  //  000A  GETGBL	R5	G17
      0x5C180200,  //  000B  MOVE	R6	R1
      0x581C0006,  //  000C  LDCONST	R7	K6
      0x7C140400,  //  000D  CALL	R5	2
      0x8C180B07,  //  000E  GETMET	R6	R5	K7
      0x7C180200,  //  000F  CALL	R6	1
      0x8C1C0B08,  //  0010  GETMET	R7	R5	K8
      0x7C1C0200,  //  0011  CALL	R7	1
      0x8C1C0509,  //  0012  GETMET	R7	R2	K9
      0x5C240C00,  //  0013  MOVE	R9	R6
      0x5828000A,  //  0014  LDCONST	R10	K10
      0x7C1C0600,  //  0015  CALL	R7	3
      0x4C140000,  //  0016  LDNIL	R5
      0x4C180000,  //  0017  LDNIL	R6
      0x6020000C,  //  0018  GETGBL	R8	G12
      0x5C240E00,  //  0019  MOVE	R9	R7
      0x7C200200,  //  001A  CALL	R8	1
      0x2420110B,  //  001B  GT	R8	R8	K11
      0x78220015,  //  001C  JMPF	R8	#0033
      0x8C20070C,  //  001D  GETMET	R8	R3	K12
      0x94280F0B,  //  001E  GETIDX	R10	R7	K11
      0x7C200400,  //  001F  CALL	R8	2
      0x60240004,  //  0020  GETGBL	R9	G4
      0x5C281000,  //  0021  MOVE	R10	R8
      0x7C240200,  //  0022  CALL	R9	1
      0x1C24130D,  //  0023  EQ	R9	R9	K13
      0x78260008,  //  0024  JMPF	R9	#002E
      0x8C24010E,  //  0025  GETMET	R9	R0	K14
      0x5C2C1000,  //  0026  MOVE	R11	R8
      0x7C240400,  //  0027  CALL	R9	2
      0x8C24010F,  //  0028  GETMET	R9	R0	K15
      0x5C2C1000,  //  0029  MOVE	R11	R8
      0x88300105,  //  002A  GETMBR	R12	R0	K5
      0x88340102,  //  002B  GETMBR	R13	R0	K2
      0x9430180D,  //  002C  GETIDX	R12	R12	R13
      0x7C240600,  //  002D  CALL	R9	3
      0x4C200000,  //  002E  LDNIL	R8
      0x8C240F10,  //  002F  GETMET	R9	R7	K16
      0x582C000B,  //  0030  LDCONST	R11	K11
      0x7C240400,  //  0031  CALL	R9	2
      0x7001FFE4,  //  0032  JMP		#0018
      0x4C1C0000,  //  0033  LDNIL	R7
      0x90020503,  //  0034  SETMBR	R0	K2	K3
      0x80000000,  //  0035  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse
********************************************************************/
be_local_closure(HASPmota_parse,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(json),
    /* K1   */  be_nested_str_weak(load),
    /* K2   */  be_nested_str_weak(instance),
    /* K3   */  be_nested_str_weak(parse_page),
    /* K4   */  be_nested_str_weak(parse_obj),
    /* K5   */  be_nested_str_weak(lvh_pages),
    /* K6   */  be_nested_str_weak(lvh_page_cur_idx),
    /* K7   */  be_nested_str_weak(value_error),
    /* K8   */  be_nested_str_weak(unable_X20to_X20parse_X20JSON_X20line),
    }),
    be_str_weak(parse),
    &be_const_str_solidified,
    ( &(const binstruction[21]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x8C0C0501,  //  0001  GETMET	R3	R2	K1
      0x5C140200,  //  0002  MOVE	R5	R1
      0x7C0C0400,  //  0003  CALL	R3	2
      0x60100004,  //  0004  GETGBL	R4	G4
      0x5C140600,  //  0005  MOVE	R5	R3
      0x7C100200,  //  0006  CALL	R4	1
      0x1C100902,  //  0007  EQ	R4	R4	K2
      0x78120009,  //  0008  JMPF	R4	#0013
      0x8C100103,  //  0009  GETMET	R4	R0	K3
      0x5C180600,  //  000A  MOVE	R6	R3
      0x7C100400,  //  000B  CALL	R4	2
      0x8C100104,  //  000C  GETMET	R4	R0	K4
      0x5C180600,  //  000D  MOVE	R6	R3
      0x881C0105,  //  000E  GETMBR	R7	R0	K5
      0x88200106,  //  000F  GETMBR	R8	R0	K6
      0x941C0E08,  //  0010  GETIDX	R7	R7	R8
      0x7C100600,  //  0011  CALL	R4	3
      0x70020000,  //  0012  JMP		#0014
      0xB0060F08,  //  0013  RAISE	1	K7	K8
      0x80000000,  //  0014  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: sort
********************************************************************/
be_local_closure(HASPmota_sort,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    4,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_const_class(be_class_HASPmota),
    /* K1   */  be_const_int(1),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(sort),
    &be_const_str_solidified,
    ( &(const binstruction[30]) {  /* code */
      0x58040000,  //  0000  LDCONST	R1	K0
      0x60080010,  //  0001  GETGBL	R2	G16
      0x600C000C,  //  0002  GETGBL	R3	G12
      0x5C100000,  //  0003  MOVE	R4	R0
      0x7C0C0200,  //  0004  CALL	R3	1
      0x040C0701,  //  0005  SUB	R3	R3	K1
      0x400E0203,  //  0006  CONNECT	R3	K1	R3
      0x7C080200,  //  0007  CALL	R2	1
      0xA8020010,  //  0008  EXBLK	0	#001A
      0x5C0C0400,  //  0009  MOVE	R3	R2
      0x7C0C0000,  //  000A  CALL	R3	0
      0x94100003,  //  000B  GETIDX	R4	R0	R3
      0x5C140600,  //  000C  MOVE	R5	R3
      0x24180B02,  //  000D  GT	R6	R5	K2
      0x781A0008,  //  000E  JMPF	R6	#0018
      0x04180B01,  //  000F  SUB	R6	R5	K1
      0x94180006,  //  0010  GETIDX	R6	R0	R6
      0x24180C04,  //  0011  GT	R6	R6	R4
      0x781A0004,  //  0012  JMPF	R6	#0018
      0x04180B01,  //  0013  SUB	R6	R5	K1
      0x94180006,  //  0014  GETIDX	R6	R0	R6
      0x98000A06,  //  0015  SETIDX	R0	R5	R6
      0x04140B01,  //  0016  SUB	R5	R5	K1
      0x7001FFF4,  //  0017  JMP		#000D
      0x98000A04,  //  0018  SETIDX	R0	R5	R4
      0x7001FFEE,  //  0019  JMP		#0009
      0x58080003,  //  001A  LDCONST	R2	K3
      0xAC080200,  //  001B  CATCH	R2	1	0
      0xB0080000,  //  001C  RAISE	2	R0	R0
      0x80040000,  //  001D  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_obj
********************************************************************/
be_local_closure(HASPmota_parse_obj,   /* name */
  be_nested_proto(
    20,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[30]) {     /* constants */
    /* K0   */  be_nested_str_weak(global),
    /* K1   */  be_nested_str_weak(introspect),
    /* K2   */  be_nested_str_weak(find),
    /* K3   */  be_nested_str_weak(id),
    /* K4   */  be_nested_str_weak(obj),
    /* K5   */  be_nested_str_weak(get_page_cur),
    /* K6   */  be_nested_str_weak(berry_run),
    /* K7   */  be_nested_str_weak(nil),
    /* K8   */  be_nested_str_weak(HSP_X3A_X20unable_X20to_X20compile_X20berry_X20code_X20_X22_X25s_X22_X20_X2D_X20_X27_X25s_X27_X20_X2D_X20_X25s),
    /* K9   */  be_const_int(1),
    /* K10  */  be_nested_str_weak(HSP_X3A_X20invalid_X20_X27id_X27_X3A_X20),
    /* K11  */  be_nested_str_weak(_X20for_X20_X27obj_X27_X3A),
    /* K12  */  be_nested_str_weak(parentid),
    /* K13  */  be_nested_str_weak(get_obj),
    /* K14  */  be_nested_str_weak(_lv_obj),
    /* K15  */  be_nested_str_weak(get_scr),
    /* K16  */  be_nested_str_weak(get),
    /* K17  */  be_nested_str_weak(lvh_),
    /* K18  */  be_nested_str_weak(class),
    /* K19  */  be_nested_str_weak(lvh_obj),
    /* K20  */  be_nested_str_weak(module),
    /* K21  */  be_nested_str_weak(HSP_X3A_X20cannot_X20find_X20object_X20of_X20type_X20),
    /* K22  */  be_nested_str_weak(set_obj),
    /* K23  */  be_nested_str_weak(p_X25ib_X25i),
    /* K24  */  be_nested_str_weak(function),
    /* K25  */  be_nested_str_weak(HSP_X3A_X20unable_X20to_X20run_X20berry_X20code_X20_X22_X25s_X22_X20_X2D_X20_X27_X25s_X27_X20_X2D_X20_X25s),
    /* K26  */  be_const_int(0),
    /* K27  */  be_nested_str_weak(HSP_X3A_X20cannot_X20specify_X20_X27obj_X27_X20for_X20_X27id_X27_X3A0),
    /* K28  */  be_nested_str_weak(keys),
    /* K29  */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(parse_obj),
    &be_const_str_solidified,
    ( &(const binstruction[217]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xA4120200,  //  0001  IMPORT	R4	K1
      0x60140009,  //  0002  GETGBL	R5	G9
      0x8C180302,  //  0003  GETMET	R6	R1	K2
      0x58200003,  //  0004  LDCONST	R8	K3
      0x7C180400,  //  0005  CALL	R6	2
      0x7C140200,  //  0006  CALL	R5	1
      0x60180008,  //  0007  GETGBL	R6	G8
      0x8C1C0302,  //  0008  GETMET	R7	R1	K2
      0x58240004,  //  0009  LDCONST	R9	K4
      0x7C1C0400,  //  000A  CALL	R7	2
      0x7C180200,  //  000B  CALL	R6	1
      0x4C1C0000,  //  000C  LDNIL	R7
      0x8C200105,  //  000D  GETMET	R8	R0	K5
      0x7C200200,  //  000E  CALL	R8	1
      0x60240008,  //  000F  GETGBL	R9	G8
      0x8C280302,  //  0010  GETMET	R10	R1	K2
      0x58300006,  //  0011  LDCONST	R12	K6
      0x7C280400,  //  0012  CALL	R10	2
      0x7C240200,  //  0013  CALL	R9	1
      0x4C280000,  //  0014  LDNIL	R10
      0x202C1307,  //  0015  NE	R11	R9	K7
      0x782E0012,  //  0016  JMPF	R11	#002A
      0xA8020005,  //  0017  EXBLK	0	#001E
      0x602C000D,  //  0018  GETGBL	R11	G13
      0x5C301200,  //  0019  MOVE	R12	R9
      0x7C2C0200,  //  001A  CALL	R11	1
      0x5C281600,  //  001B  MOVE	R10	R11
      0xA8040001,  //  001C  EXBLK	1	1
      0x7002000B,  //  001D  JMP		#002A
      0xAC2C0002,  //  001E  CATCH	R11	0	2
      0x70020008,  //  001F  JMP		#0029
      0x60340001,  //  0020  GETGBL	R13	G1
      0x60380018,  //  0021  GETGBL	R14	G24
      0x583C0008,  //  0022  LDCONST	R15	K8
      0x5C401200,  //  0023  MOVE	R16	R9
      0x5C441600,  //  0024  MOVE	R17	R11
      0x5C481800,  //  0025  MOVE	R18	R12
      0x7C380800,  //  0026  CALL	R14	4
      0x7C340200,  //  0027  CALL	R13	1
      0x70020000,  //  0028  JMP		#002A
      0xB0080000,  //  0029  RAISE	2	R0	R0
      0x4C2C0000,  //  002A  LDNIL	R11
      0x1C2C0A0B,  //  002B  EQ	R11	R5	R11
      0x782E0000,  //  002C  JMPF	R11	#002E
      0x80001600,  //  002D  RET	0
      0x202C0D07,  //  002E  NE	R11	R6	K7
      0x782E006E,  //  002F  JMPF	R11	#009F
      0x4C2C0000,  //  0030  LDNIL	R11
      0x202C0A0B,  //  0031  NE	R11	R5	R11
      0x782E006B,  //  0032  JMPF	R11	#009F
      0x142C0B09,  //  0033  LT	R11	R5	K9
      0x742E0002,  //  0034  JMPT	R11	#0038
      0x542E00FD,  //  0035  LDINT	R11	254
      0x242C0A0B,  //  0036  GT	R11	R5	R11
      0x782E0008,  //  0037  JMPF	R11	#0041
      0x602C0001,  //  0038  GETGBL	R11	G1
      0x60300008,  //  0039  GETGBL	R12	G8
      0x5C340A00,  //  003A  MOVE	R13	R5
      0x7C300200,  //  003B  CALL	R12	1
      0x0032140C,  //  003C  ADD	R12	K10	R12
      0x0030190B,  //  003D  ADD	R12	R12	K11
      0x00301806,  //  003E  ADD	R12	R12	R6
      0x7C2C0200,  //  003F  CALL	R11	1
      0x80001600,  //  0040  RET	0
      0x4C2C0000,  //  0041  LDNIL	R11
      0x60300009,  //  0042  GETGBL	R12	G9
      0x8C340302,  //  0043  GETMET	R13	R1	K2
      0x583C000C,  //  0044  LDCONST	R15	K12
      0x7C340400,  //  0045  CALL	R13	2
      0x7C300200,  //  0046  CALL	R12	1
      0x4C340000,  //  0047  LDNIL	R13
      0x2034180D,  //  0048  NE	R13	R12	R13
      0x78360006,  //  0049  JMPF	R13	#0051
      0x8C34110D,  //  004A  GETMET	R13	R8	K13
      0x5C3C1800,  //  004B  MOVE	R15	R12
      0x7C340400,  //  004C  CALL	R13	2
      0x4C380000,  //  004D  LDNIL	R14
      0x20381A0E,  //  004E  NE	R14	R13	R14
      0x783A0000,  //  004F  JMPF	R14	#0051
      0x882C1B0E,  //  0050  GETMBR	R11	R13	K14
      0x4C340000,  //  0051  LDNIL	R13
      0x1C34160D,  //  0052  EQ	R13	R11	R13
      0x78360002,  //  0053  JMPF	R13	#0057
      0x8C34110F,  //  0054  GETMET	R13	R8	K15
      0x7C340200,  //  0055  CALL	R13	1
      0x5C2C1A00,  //  0056  MOVE	R11	R13
      0x8C340910,  //  0057  GETMET	R13	R4	K16
      0x5C3C0000,  //  0058  MOVE	R15	R0
      0x00422206,  //  0059  ADD	R16	K17	R6
      0x7C340600,  //  005A  CALL	R13	3
      0x4C380000,  //  005B  LDNIL	R14
      0x4C3C0000,  //  005C  LDNIL	R15
      0x1C3C1A0F,  //  005D  EQ	R15	R13	R15
      0x783E0010,  //  005E  JMPF	R15	#0070
      0x8C3C0910,  //  005F  GETMET	R15	R4	K16
      0x5C440600,  //  0060  MOVE	R17	R3
      0x5C480C00,  //  0061  MOVE	R18	R6
      0x7C3C0600,  //  0062  CALL	R15	3
      0x4C400000,  //  0063  LDNIL	R16
      0x20401E10,  //  0064  NE	R16	R15	R16
      0x78420009,  //  0065  JMPF	R16	#0070
      0x60400004,  //  0066  GETGBL	R16	G4
      0x5C441E00,  //  0067  MOVE	R17	R15
      0x7C400200,  //  0068  CALL	R16	1
      0x1C402112,  //  0069  EQ	R16	R16	K18
      0x78420004,  //  006A  JMPF	R16	#0070
      0x5C401E00,  //  006B  MOVE	R16	R15
      0x5C441600,  //  006C  MOVE	R17	R11
      0x7C400200,  //  006D  CALL	R16	1
      0x5C382000,  //  006E  MOVE	R14	R16
      0x88340113,  //  006F  GETMBR	R13	R0	K19
      0x4C3C0000,  //  0070  LDNIL	R15
      0x1C3C1A0F,  //  0071  EQ	R15	R13	R15
      0x783E000F,  //  0072  JMPF	R15	#0083
      0x8C3C0914,  //  0073  GETMET	R15	R4	K20
      0x5C440C00,  //  0074  MOVE	R17	R6
      0x7C3C0400,  //  0075  CALL	R15	2
      0x4C400000,  //  0076  LDNIL	R16
      0x20401E10,  //  0077  NE	R16	R15	R16
      0x78420009,  //  0078  JMPF	R16	#0083
      0x60400004,  //  0079  GETGBL	R16	G4
      0x5C441E00,  //  007A  MOVE	R17	R15
      0x7C400200,  //  007B  CALL	R16	1
      0x1C402112,  //  007C  EQ	R16	R16	K18
      0x78420004,  //  007D  JMPF	R16	#0083
      0x5C401E00,  //  007E  MOVE	R16	R15
      0x5C441600,  //  007F  MOVE	R17	R11
      0x7C400200,  //  0080  CALL	R16	1
      0x5C382000,  //  0081  MOVE	R14	R16
      0x88340113,  //  0082  GETMBR	R13	R0	K19
      0x4C3C0000,  //  0083  LDNIL	R15
      0x1C3C1A0F,  //  0084  EQ	R15	R13	R15
      0x783E0006,  //  0085  JMPF	R15	#008D
      0x603C0001,  //  0086  GETGBL	R15	G1
      0x60400008,  //  0087  GETGBL	R16	G8
      0x5C440C00,  //  0088  MOVE	R17	R6
      0x7C400200,  //  0089  CALL	R16	1
      0x00422A10,  //  008A  ADD	R16	K21	R16
      0x7C3C0200,  //  008B  CALL	R15	1
      0x80001E00,  //  008C  RET	0
      0x5C3C1A00,  //  008D  MOVE	R15	R13
      0x5C401600,  //  008E  MOVE	R16	R11
      0x5C440400,  //  008F  MOVE	R17	R2
      0x5C480200,  //  0090  MOVE	R18	R1
      0x5C4C1C00,  //  0091  MOVE	R19	R14
      0x7C3C0800,  //  0092  CALL	R15	4
      0x5C1C1E00,  //  0093  MOVE	R7	R15
      0x8C3C1116,  //  0094  GETMET	R15	R8	K22
      0x5C440A00,  //  0095  MOVE	R17	R5
      0x5C480E00,  //  0096  MOVE	R18	R7
      0x7C3C0600,  //  0097  CALL	R15	3
      0x603C0018,  //  0098  GETGBL	R15	G24
      0x58400017,  //  0099  LDCONST	R16	K23
      0x8C441103,  //  009A  GETMET	R17	R8	K3
      0x7C440200,  //  009B  CALL	R17	1
      0x5C480A00,  //  009C  MOVE	R18	R5
      0x7C3C0600,  //  009D  CALL	R15	3
      0x900C1E07,  //  009E  SETMBR	R3	R15	R7
      0x4C2C0000,  //  009F  LDNIL	R11
      0x202C140B,  //  00A0  NE	R11	R10	R11
      0x782E0018,  //  00A1  JMPF	R11	#00BB
      0xA802000B,  //  00A2  EXBLK	0	#00AF
      0x5C2C1400,  //  00A3  MOVE	R11	R10
      0x7C2C0000,  //  00A4  CALL	R11	0
      0x60300004,  //  00A5  GETGBL	R12	G4
      0x5C341600,  //  00A6  MOVE	R13	R11
      0x7C300200,  //  00A7  CALL	R12	1
      0x1C301918,  //  00A8  EQ	R12	R12	K24
      0x78320002,  //  00A9  JMPF	R12	#00AD
      0x5C301600,  //  00AA  MOVE	R12	R11
      0x5C340E00,  //  00AB  MOVE	R13	R7
      0x7C300200,  //  00AC  CALL	R12	1
      0xA8040001,  //  00AD  EXBLK	1	1
      0x7002000B,  //  00AE  JMP		#00BB
      0xAC2C0002,  //  00AF  CATCH	R11	0	2
      0x70020008,  //  00B0  JMP		#00BA
      0x60340001,  //  00B1  GETGBL	R13	G1
      0x60380018,  //  00B2  GETGBL	R14	G24
      0x583C0019,  //  00B3  LDCONST	R15	K25
      0x5C401200,  //  00B4  MOVE	R16	R9
      0x5C441600,  //  00B5  MOVE	R17	R11
      0x5C481800,  //  00B6  MOVE	R18	R12
      0x7C380800,  //  00B7  CALL	R14	4
      0x7C340200,  //  00B8  CALL	R13	1
      0x70020000,  //  00B9  JMP		#00BB
      0xB0080000,  //  00BA  RAISE	2	R0	R0
      0x1C2C0B1A,  //  00BB  EQ	R11	R5	K26
      0x782E0005,  //  00BC  JMPF	R11	#00C3
      0x202C0D07,  //  00BD  NE	R11	R6	K7
      0x782E0003,  //  00BE  JMPF	R11	#00C3
      0x602C0001,  //  00BF  GETGBL	R11	G1
      0x5830001B,  //  00C0  LDCONST	R12	K27
      0x7C2C0200,  //  00C1  CALL	R11	1
      0x80001600,  //  00C2  RET	0
      0x1C2C0B1A,  //  00C3  EQ	R11	R5	K26
      0x782E0005,  //  00C4  JMPF	R11	#00CB
      0x8C2C0105,  //  00C5  GETMET	R11	R0	K5
      0x7C2C0200,  //  00C6  CALL	R11	1
      0x8C2C170D,  //  00C7  GETMET	R11	R11	K13
      0x5834001A,  //  00C8  LDCONST	R13	K26
      0x7C2C0400,  //  00C9  CALL	R11	2
      0x5C1C1600,  //  00CA  MOVE	R7	R11
      0x602C0010,  //  00CB  GETGBL	R11	G16
      0x8C30031C,  //  00CC  GETMET	R12	R1	K28
      0x7C300200,  //  00CD  CALL	R12	1
      0x7C2C0200,  //  00CE  CALL	R11	1
      0xA8020004,  //  00CF  EXBLK	0	#00D5
      0x5C301600,  //  00D0  MOVE	R12	R11
      0x7C300000,  //  00D1  CALL	R12	0
      0x9434020C,  //  00D2  GETIDX	R13	R1	R12
      0x901C180D,  //  00D3  SETMBR	R7	R12	R13
      0x7001FFFA,  //  00D4  JMP		#00D0
      0x582C001D,  //  00D5  LDCONST	R11	K29
      0xAC2C0200,  //  00D6  CATCH	R11	1	0
      0xB0080000,  //  00D7  RAISE	2	R0	R0
      0x80000000,  //  00D8  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: event_dispatch
********************************************************************/
be_local_closure(HASPmota_event_dispatch,   /* name */
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
    /* K0   */  be_nested_str_weak(introspect),
    /* K1   */  be_nested_str_weak(toptr),
    /* K2   */  be_nested_str_weak(event),
    /* K3   */  be_nested_str_weak(_change_buffer),
    /* K4   */  be_nested_str_weak(lv),
    /* K5   */  be_nested_str_weak(lv_event),
    /* K6   */  be_nested_str_weak(user_data),
    /* K7   */  be_const_int(0),
    /* K8   */  be_nested_str_weak(fromptr),
    /* K9   */  be_nested_str_weak(instance),
    /* K10  */  be_nested_str_weak(event_cb),
    }),
    be_str_weak(event_dispatch),
    &be_const_str_solidified,
    ( &(const binstruction[35]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x8C0C0501,  //  0001  GETMET	R3	R2	K1
      0x5C140200,  //  0002  MOVE	R5	R1
      0x7C0C0400,  //  0003  CALL	R3	2
      0x88100102,  //  0004  GETMBR	R4	R0	K2
      0x78120004,  //  0005  JMPF	R4	#000B
      0x88100102,  //  0006  GETMBR	R4	R0	K2
      0x8C100903,  //  0007  GETMET	R4	R4	K3
      0x5C180600,  //  0008  MOVE	R6	R3
      0x7C100400,  //  0009  CALL	R4	2
      0x70020004,  //  000A  JMP		#0010
      0xB8120800,  //  000B  GETNGBL	R4	K4
      0x8C100905,  //  000C  GETMET	R4	R4	K5
      0x5C180600,  //  000D  MOVE	R6	R3
      0x7C100400,  //  000E  CALL	R4	2
      0x90020404,  //  000F  SETMBR	R0	K2	R4
      0x88100102,  //  0010  GETMBR	R4	R0	K2
      0x88100906,  //  0011  GETMBR	R4	R4	K6
      0x60140009,  //  0012  GETGBL	R5	G9
      0x5C180800,  //  0013  MOVE	R6	R4
      0x7C140200,  //  0014  CALL	R5	1
      0x20140B07,  //  0015  NE	R5	R5	K7
      0x7816000A,  //  0016  JMPF	R5	#0022
      0x8C140508,  //  0017  GETMET	R5	R2	K8
      0x5C1C0800,  //  0018  MOVE	R7	R4
      0x7C140400,  //  0019  CALL	R5	2
      0x60180004,  //  001A  GETGBL	R6	G4
      0x5C1C0A00,  //  001B  MOVE	R7	R5
      0x7C180200,  //  001C  CALL	R6	1
      0x1C180D09,  //  001D  EQ	R6	R6	K9
      0x781A0002,  //  001E  JMPF	R6	#0022
      0x8C180B0A,  //  001F  GETMET	R6	R5	K10
      0x88200102,  //  0020  GETMBR	R8	R0	K2
      0x7C180400,  //  0021  CALL	R6	2
      0x80000000,  //  0022  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: HASPmota
********************************************************************/
be_local_class(HASPmota,
    10,
    NULL,
    be_nested_map(43,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(lvh_textarea, 41), be_const_class(be_class_lvh_textarea) },
        { be_const_key_weak(scr, -1), be_const_var(3) },
        { be_const_key_weak(event_dispatch, 16), be_const_closure(HASPmota_event_dispatch_closure) },
        { be_const_key_weak(lvh_bar, -1), be_const_class(be_class_lvh_bar) },
        { be_const_key_weak(lvh_img, -1), be_const_class(be_class_lvh_img) },
        { be_const_key_weak(event, -1), be_const_var(8) },
        { be_const_key_weak(do_action, 33), be_const_closure(HASPmota_do_action_closure) },
        { be_const_key_weak(pages_list_sorted, -1), be_const_closure(HASPmota_pages_list_sorted_closure) },
        { be_const_key_weak(lvh_btnmatrix, 42), be_const_class(be_class_lvh_btnmatrix) },
        { be_const_key_weak(start, -1), be_const_closure(HASPmota_start_closure) },
        { be_const_key_weak(init, 5), be_const_closure(HASPmota_init_closure) },
        { be_const_key_weak(lvh_scr, -1), be_const_class(be_class_lvh_scr) },
        { be_const_key_weak(parse_page, -1), be_const_closure(HASPmota_parse_page_closure) },
        { be_const_key_weak(register_event, 24), be_const_closure(HASPmota_register_event_closure) },
        { be_const_key_weak(lvh_roller, 34), be_const_class(be_class_lvh_roller) },
        { be_const_key_weak(lvh_page, 11), be_const_class(be_class_lvh_page) },
        { be_const_key_weak(lvh_page_cur_idx, -1), be_const_var(6) },
        { be_const_key_weak(lvh_arc, 12), be_const_class(be_class_lvh_arc) },
        { be_const_key_weak(hres, 38), be_const_var(1) },
        { be_const_key_weak(dark, -1), be_const_var(0) },
        { be_const_key_weak(lvh_label, 37), be_const_class(be_class_lvh_label) },
        { be_const_key_weak(deinit, 14), be_const_closure(HASPmota_deinit_closure) },
        { be_const_key_weak(page_dir_to, -1), be_const_closure(HASPmota_page_dir_to_closure) },
        { be_const_key_weak(parse_obj, -1), be_const_closure(HASPmota_parse_obj_closure) },
        { be_const_key_weak(lvh_spinner, -1), be_const_class(be_class_lvh_spinner) },
        { be_const_key_weak(_load, -1), be_const_closure(HASPmota__load_closure) },
        { be_const_key_weak(lvh_qrcode, -1), be_const_class(be_class_lvh_qrcode) },
        { be_const_key_weak(parse, -1), be_const_closure(HASPmota_parse_closure) },
        { be_const_key_weak(lvh_pages, 2), be_const_var(5) },
        { be_const_key_weak(lvh_line, -1), be_const_class(be_class_lvh_line) },
        { be_const_key_weak(lvh_switch, -1), be_const_class(be_class_lvh_switch) },
        { be_const_key_weak(re_page_target, -1), be_const_var(7) },
        { be_const_key_weak(r16, -1), be_const_var(4) },
        { be_const_key_weak(def_templ_name, -1), be_nested_str_weak(pages_X2Ejsonl) },
        { be_const_key_weak(lvh_obj, 19), be_const_class(be_class_lvh_obj) },
        { be_const_key_weak(sort, 23), be_const_static_closure(HASPmota_sort_closure) },
        { be_const_key_weak(lvh_dropdown, -1), be_const_class(be_class_lvh_dropdown) },
        { be_const_key_weak(lvh_checkbox, -1), be_const_class(be_class_lvh_checkbox) },
        { be_const_key_weak(lvh_slider, -1), be_const_class(be_class_lvh_slider) },
        { be_const_key_weak(vres, -1), be_const_var(2) },
        { be_const_key_weak(lvh_btn, -1), be_const_class(be_class_lvh_btn) },
        { be_const_key_weak(event_cb, -1), be_const_var(9) },
        { be_const_key_weak(get_page_cur, -1), be_const_closure(HASPmota_get_page_cur_closure) },
    })),
    be_str_weak(HASPmota)
);

/********************************************************************
** Solidified function: _anonymous_
********************************************************************/
be_local_closure(haspmota__anonymous_,   /* name */
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
        { be_const_key_weak(init, -1), be_const_closure(haspmota__anonymous__closure) },
    }))
);
BE_EXPORT_VARIABLE be_define_const_native_module(haspmota);
/********************************************************************/


#endif // USE_LVGL_HASPMOTA
#endif // USE_LVGL