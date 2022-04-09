/********************************************************************
 * Tasmota OpenHASP solidified
 *******************************************************************/
#include "be_constobj.h"

#ifdef USE_LVGL
#ifdef USE_LVGL_OPENHASP

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
    /* K0   */  be_nested_str_literal("_obj_id"),
    /* K1   */  be_nested_str_literal("find"),
    }),
    be_str_literal("get_obj"),
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
    /* K0   */  be_nested_str_literal("global"),
    /* K1   */  be_nested_str_literal("_oh"),
    /* K2   */  be_const_int(1),
    /* K3   */  be_nested_str_literal("_page_id"),
    /* K4   */  be_nested_str_literal("_obj_id"),
    /* K5   */  be_nested_str_literal("_lv_scr"),
    /* K6   */  be_nested_str_literal("lv"),
    /* K7   */  be_nested_str_literal("scr_act"),
    /* K8   */  be_const_int(0),
    /* K9   */  be_nested_str_literal("layer_top"),
    /* K10  */  be_nested_str_literal("obj"),
    /* K11  */  be_nested_str_literal("get_style_bg_color"),
    /* K12  */  be_nested_str_literal("set_style_bg_color"),
    /* K13  */  be_nested_str_literal("lvh_scr"),
    /* K14  */  be_nested_str_literal("p"),
    /* K15  */  be_nested_str_literal("b0"),
    }),
    be_str_literal("init"),
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
    /* K0   */  be_nested_str_literal("_lv_scr"),
    }),
    be_str_literal("get_scr"),
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
    11,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[14]) {     /* constants */
    /* K0   */  be_nested_str_literal("_lv_scr"),
    /* K1   */  be_nested_str_literal("_p"),
    /* K2   */  be_nested_str_literal("lv"),
    /* K3   */  be_nested_str_literal("scr_act"),
    /* K4   */  be_nested_str_literal("_oh"),
    /* K5   */  be_nested_str_literal("page_dir_to"),
    /* K6   */  be_nested_str_literal("id"),
    /* K7   */  be_nested_str_literal("lvh_page_cur_idx"),
    /* K8   */  be_nested_str_literal("_page_id"),
    /* K9   */  be_nested_str_literal("show_anim"),
    /* K10  */  be_nested_str_literal("find"),
    /* K11  */  be_nested_str_literal("SCR_LOAD_ANIM_NONE"),
    /* K12  */  be_nested_str_literal("scr_load_anim"),
    /* K13  */  be_const_int(0),
    }),
    be_str_literal("show"),
    &be_const_str_solidified,
    ( &(const binstruction[46]) {  /* code */
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
      0x880C0104,  //  001C  GETMBR	R3	R0	K4
      0x88100108,  //  001D  GETMBR	R4	R0	K8
      0x900E0E04,  //  001E  SETMBR	R3	K7	R4
      0x880C0109,  //  001F  GETMBR	R3	R0	K9
      0x8C0C070A,  //  0020  GETMET	R3	R3	K10
      0x5C140200,  //  0021  MOVE	R5	R1
      0xB81A0400,  //  0022  GETNGBL	R6	K2
      0x88180D0B,  //  0023  GETMBR	R6	R6	K11
      0x7C0C0600,  //  0024  CALL	R3	3
      0xB8120400,  //  0025  GETNGBL	R4	K2
      0x8C10090C,  //  0026  GETMET	R4	R4	K12
      0x88180100,  //  0027  GETMBR	R6	R0	K0
      0x5C1C0600,  //  0028  MOVE	R7	R3
      0x5C200400,  //  0029  MOVE	R8	R2
      0x5824000D,  //  002A  LDCONST	R9	K13
      0x50280000,  //  002B  LDBOOL	R10	0	0
      0x7C100C00,  //  002C  CALL	R4	6
      0x80000000,  //  002D  RET	0
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
    /* K0   */  be_nested_str_literal("_obj_id"),
    }),
    be_str_literal("set_obj"),
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
    /* K0   */  be_nested_str_literal("_page_id"),
    }),
    be_str_literal("id"),
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
        { be_const_key_literal("prev", 2), be_const_var(4) },
        { be_const_key_literal("_obj_id", -1), be_const_var(0) },
        { be_const_key_literal("show_anim", -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(5,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(0, 3), be_const_int(0) },
        { be_const_key_int(1, -1), be_const_int(5) },
        { be_const_key_int(2, -1), be_const_int(8) },
        { be_const_key_int(-1, -1), be_const_int(6) },
        { be_const_key_int(-2, -1), be_const_int(7) },
    }))    ) } )) },
        { be_const_key_literal("_oh", -1), be_const_var(3) },
        { be_const_key_literal("id", -1), be_const_closure(lvh_page_id_closure) },
        { be_const_key_literal("init", -1), be_const_closure(lvh_page_init_closure) },
        { be_const_key_literal("next", 13), be_const_var(5) },
        { be_const_key_literal("_page_id", -1), be_const_var(1) },
        { be_const_key_literal("show", -1), be_const_closure(lvh_page_show_closure) },
        { be_const_key_literal("_lv_scr", -1), be_const_var(2) },
        { be_const_key_literal("back", 4), be_const_var(6) },
        { be_const_key_literal("get_obj", 12), be_const_closure(lvh_page_get_obj_closure) },
        { be_const_key_literal("set_obj", -1), be_const_closure(lvh_page_set_obj_closure) },
        { be_const_key_literal("get_scr", -1), be_const_closure(lvh_page_get_scr_closure) },
    })),
    be_str_literal("lvh_page")
);
/*******************************************************************/

void be_load_lvh_page_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_lvh_page);
    be_setglobal(vm, "lvh_page");
    be_pop(vm, 1);
}

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
    /* K0   */  be_nested_str_literal("_lv_obj"),
    /* K1   */  be_nested_str_literal("has_flag"),
    /* K2   */  be_nested_str_literal("lv"),
    /* K3   */  be_nested_str_literal("OBJ_FLAG_CLICKABLE"),
    }),
    be_str_literal("get_enabled"),
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
    /* K0   */  be_nested_str_literal("_page"),
    /* K1   */  be_nested_str_literal("_lv_class"),
    /* K2   */  be_nested_str_literal("_lv_obj"),
    /* K3   */  be_nested_str_literal("post_init"),
    }),
    be_str_literal("init"),
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
    /* K0   */  be_nested_str_literal("get_value_color"),
    }),
    be_str_literal("get_value_color"),
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
    /* K0   */  be_nested_str_literal("set_enabled"),
    }),
    be_str_literal("set_click"),
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
    /* K0   */  be_nested_str_literal("_text_rule"),
    }),
    be_str_literal("get_text_rule"),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
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
    /* K0   */  be_nested_str_literal("_lv_obj"),
    /* K1   */  be_nested_str_literal("add_flag"),
    /* K2   */  be_nested_str_literal("lv"),
    /* K3   */  be_nested_str_literal("OBJ_FLAG_HIDDEN"),
    /* K4   */  be_nested_str_literal("clear_flag"),
    }),
    be_str_literal("set_hidden"),
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
    /* K0   */  be_nested_str_literal("_text_rule_formula"),
    }),
    be_str_literal("get_text_rule_formula"),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
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
    /* K0   */  be_nested_str_literal("_lv_part2_selector"),
    /* K1   */  be_nested_str_literal("_lv_obj"),
    /* K2   */  be_nested_str_literal("set_style_pad_all"),
    /* K3   */  be_nested_str_literal("lv"),
    /* K4   */  be_nested_str_literal("STATE_DEFAULT"),
    }),
    be_str_literal("set_pad_all2"),
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
    13,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_literal("_text_rule_function"),
    /* K1   */  be_nested_str_literal("string"),
    /* K2   */  be_nested_str_literal("format"),
    /* K3   */  be_nested_str_literal("HSP: failed to run self._text_rule_function - %s (%s)"),
    /* K4   */  be_nested_str_literal("_text_rule_format"),
    /* K5   */  be_nested_str_literal(""),
    /* K6   */  be_nested_str_literal("text"),
    }),
    be_str_literal("text_rule_matched"),
    &be_const_str_solidified,
    ( &(const binstruction[43]) {  /* code */
      0x6008000A,  //  0000  GETGBL	R2	G10
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x7C080200,  //  0002  CALL	R2	1
      0x880C0100,  //  0003  GETMBR	R3	R0	K0
      0x4C100000,  //  0004  LDNIL	R4
      0x20100604,  //  0005  NE	R4	R3	R4
      0x78120012,  //  0006  JMPF	R4	#001A
      0xA8020005,  //  0007  EXBLK	0	#000E
      0x5C100600,  //  0008  MOVE	R4	R3
      0x5C140400,  //  0009  MOVE	R5	R2
      0x7C100200,  //  000A  CALL	R4	1
      0x5C080800,  //  000B  MOVE	R2	R4
      0xA8040001,  //  000C  EXBLK	1	1
      0x7002000B,  //  000D  JMP		#001A
      0xAC100002,  //  000E  CATCH	R4	0	2
      0x70020008,  //  000F  JMP		#0019
      0xA41A0200,  //  0010  IMPORT	R6	K1
      0x601C0001,  //  0011  GETGBL	R7	G1
      0x8C200D02,  //  0012  GETMET	R8	R6	K2
      0x58280003,  //  0013  LDCONST	R10	K3
      0x5C2C0800,  //  0014  MOVE	R11	R4
      0x5C300A00,  //  0015  MOVE	R12	R5
      0x7C200800,  //  0016  CALL	R8	4
      0x7C1C0200,  //  0017  CALL	R7	1
      0x70020000,  //  0018  JMP		#001A
      0xB0080000,  //  0019  RAISE	2	R0	R0
      0x88100104,  //  001A  GETMBR	R4	R0	K4
      0x60140004,  //  001B  GETGBL	R5	G4
      0x5C180800,  //  001C  MOVE	R6	R4
      0x7C140200,  //  001D  CALL	R5	1
      0x1C140B01,  //  001E  EQ	R5	R5	K1
      0x78160006,  //  001F  JMPF	R5	#0027
      0xA4160200,  //  0020  IMPORT	R5	K1
      0x8C180B02,  //  0021  GETMET	R6	R5	K2
      0x5C200800,  //  0022  MOVE	R8	R4
      0x5C240400,  //  0023  MOVE	R9	R2
      0x7C180600,  //  0024  CALL	R6	3
      0x5C100C00,  //  0025  MOVE	R4	R6
      0x70020000,  //  0026  JMP		#0028
      0x58100005,  //  0027  LDCONST	R4	K5
      0x90020C04,  //  0028  SETMBR	R0	K6	R4
      0x50140000,  //  0029  LDBOOL	R5	0	0
      0x80040A00,  //  002A  RET	1	R5
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
    /* K0   */  be_nested_str_literal("_lv_part2_selector"),
    /* K1   */  be_nested_str_literal("_lv_obj"),
    /* K2   */  be_nested_str_literal("set_style_pad_left"),
    /* K3   */  be_nested_str_literal("lv"),
    /* K4   */  be_nested_str_literal("STATE_DEFAULT"),
    }),
    be_str_literal("set_pad_left2"),
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
** Solidified function: setmember
********************************************************************/
be_local_closure(lvh_obj_setmember,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[16]) {     /* constants */
    /* K0   */  be_nested_str_literal("_attr_ignore"),
    /* K1   */  be_nested_str_literal("find"),
    /* K2   */  be_nested_str_literal("_attr_map"),
    /* K3   */  be_nested_str_literal("contains"),
    /* K4   */  be_nested_str_literal("string"),
    /* K5   */  be_nested_str_literal("introspect"),
    /* K6   */  be_nested_str_literal("get"),
    /* K7   */  be_nested_str_literal("_lv_obj"),
    /* K8   */  be_nested_str_literal("set_"),
    /* K9   */  be_nested_str_literal("is_color_attribute"),
    /* K10  */  be_nested_str_literal("parse_color"),
    /* K11  */  be_nested_str_literal("function"),
    /* K12  */  be_nested_str_literal("style_"),
    /* K13  */  be_const_int(0),
    /* K14  */  be_nested_str_literal("HSP: Could not find function set_"),
    /* K15  */  be_nested_str_literal("HSP: unknown attribute:"),
    }),
    be_str_literal("setmember"),
    &be_const_str_solidified,
    ( &(const binstruction[82]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x8C0C0701,  //  0001  GETMET	R3	R3	K1
      0x5C140200,  //  0002  MOVE	R5	R1
      0x7C0C0400,  //  0003  CALL	R3	2
      0x4C100000,  //  0004  LDNIL	R4
      0x200C0604,  //  0005  NE	R3	R3	R4
      0x780E0000,  //  0006  JMPF	R3	#0008
      0x80000600,  //  0007  RET	0
      0x880C0102,  //  0008  GETMBR	R3	R0	K2
      0x8C0C0703,  //  0009  GETMET	R3	R3	K3
      0x5C140200,  //  000A  MOVE	R5	R1
      0x7C0C0400,  //  000B  CALL	R3	2
      0x780E003F,  //  000C  JMPF	R3	#004D
      0xA40E0800,  //  000D  IMPORT	R3	K4
      0xA4120A00,  //  000E  IMPORT	R4	K5
      0x88140102,  //  000F  GETMBR	R5	R0	K2
      0x94140A01,  //  0010  GETIDX	R5	R5	R1
      0x7816002B,  //  0011  JMPF	R5	#003E
      0x8C180906,  //  0012  GETMET	R6	R4	K6
      0x88200107,  //  0013  GETMBR	R8	R0	K7
      0x00261005,  //  0014  ADD	R9	K8	R5
      0x7C180600,  //  0015  CALL	R6	3
      0x601C0004,  //  0016  GETGBL	R7	G4
      0x5C200A00,  //  0017  MOVE	R8	R5
      0x7C1C0200,  //  0018  CALL	R7	1
      0x1C1C0F04,  //  0019  EQ	R7	R7	K4
      0x781E0007,  //  001A  JMPF	R7	#0023
      0x8C1C0109,  //  001B  GETMET	R7	R0	K9
      0x5C240A00,  //  001C  MOVE	R9	R5
      0x7C1C0400,  //  001D  CALL	R7	2
      0x781E0003,  //  001E  JMPF	R7	#0023
      0x8C1C010A,  //  001F  GETMET	R7	R0	K10
      0x5C240400,  //  0020  MOVE	R9	R2
      0x7C1C0400,  //  0021  CALL	R7	2
      0x5C080E00,  //  0022  MOVE	R2	R7
      0x601C0004,  //  0023  GETGBL	R7	G4
      0x5C200C00,  //  0024  MOVE	R8	R6
      0x7C1C0200,  //  0025  CALL	R7	1
      0x1C1C0F0B,  //  0026  EQ	R7	R7	K11
      0x781E0011,  //  0027  JMPF	R7	#003A
      0x8C1C0701,  //  0028  GETMET	R7	R3	K1
      0x5C240A00,  //  0029  MOVE	R9	R5
      0x5828000C,  //  002A  LDCONST	R10	K12
      0x7C1C0600,  //  002B  CALL	R7	3
      0x1C1C0F0D,  //  002C  EQ	R7	R7	K13
      0x781E0005,  //  002D  JMPF	R7	#0034
      0x5C1C0C00,  //  002E  MOVE	R7	R6
      0x88200107,  //  002F  GETMBR	R8	R0	K7
      0x5C240400,  //  0030  MOVE	R9	R2
      0x5828000D,  //  0031  LDCONST	R10	K13
      0x7C1C0600,  //  0032  CALL	R7	3
      0x70020003,  //  0033  JMP		#0038
      0x5C1C0C00,  //  0034  MOVE	R7	R6
      0x88200107,  //  0035  GETMBR	R8	R0	K7
      0x5C240400,  //  0036  MOVE	R9	R2
      0x7C1C0400,  //  0037  CALL	R7	2
      0x80000E00,  //  0038  RET	0
      0x70020002,  //  0039  JMP		#003D
      0x601C0001,  //  003A  GETGBL	R7	G1
      0x00221C05,  //  003B  ADD	R8	K14	R5
      0x7C1C0200,  //  003C  CALL	R7	1
      0x7002000D,  //  003D  JMP		#004C
      0x8C180906,  //  003E  GETMET	R6	R4	K6
      0x5C200000,  //  003F  MOVE	R8	R0
      0x00261001,  //  0040  ADD	R9	K8	R1
      0x7C180600,  //  0041  CALL	R6	3
      0x601C0004,  //  0042  GETGBL	R7	G4
      0x5C200C00,  //  0043  MOVE	R8	R6
      0x7C1C0200,  //  0044  CALL	R7	1
      0x1C1C0F0B,  //  0045  EQ	R7	R7	K11
      0x781E0004,  //  0046  JMPF	R7	#004C
      0x5C1C0C00,  //  0047  MOVE	R7	R6
      0x5C200000,  //  0048  MOVE	R8	R0
      0x5C240400,  //  0049  MOVE	R9	R2
      0x7C1C0400,  //  004A  CALL	R7	2
      0x80000E00,  //  004B  RET	0
      0x70020003,  //  004C  JMP		#0051
      0x600C0001,  //  004D  GETGBL	R3	G1
      0x5810000F,  //  004E  LDCONST	R4	K15
      0x5C140200,  //  004F  MOVE	R5	R1
      0x7C0C0400,  //  0050  CALL	R3	2
      0x80000000,  //  0051  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_text_color
********************************************************************/
be_local_closure(lvh_obj_set_text_color,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_literal("_lv_obj"),
    /* K1   */  be_nested_str_literal("set_style_text_color"),
    /* K2   */  be_nested_str_literal("parse_color"),
    /* K3   */  be_const_int(0),
    }),
    be_str_literal("set_text_color"),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x8C100102,  //  0002  GETMET	R4	R0	K2
      0x5C180200,  //  0003  MOVE	R6	R1
      0x7C100400,  //  0004  CALL	R4	2
      0x58140003,  //  0005  LDCONST	R5	K3
      0x7C080600,  //  0006  CALL	R2	3
      0x80000000,  //  0007  RET	0
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
    /* K0   */  be_nested_str_literal("string"),
    /* K1   */  be_nested_str_literal("toupper"),
    /* K2   */  be_nested_str_literal("TRUE"),
    /* K3   */  be_nested_str_literal("FALSE"),
    /* K4   */  be_nested_str_literal("_lv_obj"),
    /* K5   */  be_nested_str_literal("add_state"),
    /* K6   */  be_nested_str_literal("lv"),
    /* K7   */  be_nested_str_literal("STATE_CHECKED"),
    /* K8   */  be_nested_str_literal("clear_state"),
    }),
    be_str_literal("set_toggle"),
    &be_const_str_solidified,
    ( &(const binstruction[26]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x8C0C0501,  //  0001  GETMET	R3	R2	K1
      0x60140008,  //  0002  GETGBL	R5	G8
      0x5C180200,  //  0003  MOVE	R6	R1
      0x7C140200,  //  0004  CALL	R5	1
      0x7C0C0400,  //  0005  CALL	R3	2
      0x5C040600,  //  0006  MOVE	R1	R3
      0x1C0C0302,  //  0007  EQ	R3	R1	K2
      0x780E0000,  //  0008  JMPF	R3	#000A
      0x50040200,  //  0009  LDBOOL	R1	1	0
      0x1C0C0303,  //  000A  EQ	R3	R1	K3
      0x780E0000,  //  000B  JMPF	R3	#000D
      0x50040000,  //  000C  LDBOOL	R1	0	0
      0x78060005,  //  000D  JMPF	R1	#0014
      0x880C0104,  //  000E  GETMBR	R3	R0	K4
      0x8C0C0705,  //  000F  GETMET	R3	R3	K5
      0xB8160C00,  //  0010  GETNGBL	R5	K6
      0x88140B07,  //  0011  GETMBR	R5	R5	K7
      0x7C0C0400,  //  0012  CALL	R3	2
      0x70020004,  //  0013  JMP		#0019
      0x880C0104,  //  0014  GETMBR	R3	R0	K4
      0x8C0C0708,  //  0015  GETMET	R3	R3	K8
      0xB8160C00,  //  0016  GETNGBL	R5	K6
      0x88140B07,  //  0017  GETMBR	R5	R5	K7
      0x7C0C0400,  //  0018  CALL	R3	2
      0x80000000,  //  0019  RET	0
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
    /* K0   */  be_nested_str_literal("_lv_obj"),
    /* K1   */  be_nested_str_literal("has_state"),
    /* K2   */  be_nested_str_literal("lv"),
    /* K3   */  be_nested_str_literal("STATE_CHECKED"),
    }),
    be_str_literal("get_toggle"),
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
    /* K0   */  be_nested_str_literal("check_label"),
    /* K1   */  be_nested_str_literal("_lv_label"),
    /* K2   */  be_nested_str_literal("set_text"),
    }),
    be_str_literal("set_text"),
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
** Solidified function: val_rule_matched
********************************************************************/
be_local_closure(lvh_obj_val_rule_matched,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_literal("_val_rule_function"),
    /* K1   */  be_nested_str_literal("string"),
    /* K2   */  be_nested_str_literal("format"),
    /* K3   */  be_nested_str_literal("HSP: failed to run self._val_rule_function - %s (%s)"),
    /* K4   */  be_nested_str_literal("val"),
    }),
    be_str_literal("val_rule_matched"),
    &be_const_str_solidified,
    ( &(const binstruction[32]) {  /* code */
      0x6008000A,  //  0000  GETGBL	R2	G10
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x7C080200,  //  0002  CALL	R2	1
      0x880C0100,  //  0003  GETMBR	R3	R0	K0
      0x4C100000,  //  0004  LDNIL	R4
      0x20100604,  //  0005  NE	R4	R3	R4
      0x78120012,  //  0006  JMPF	R4	#001A
      0xA8020005,  //  0007  EXBLK	0	#000E
      0x5C100600,  //  0008  MOVE	R4	R3
      0x5C140400,  //  0009  MOVE	R5	R2
      0x7C100200,  //  000A  CALL	R4	1
      0x5C080800,  //  000B  MOVE	R2	R4
      0xA8040001,  //  000C  EXBLK	1	1
      0x7002000B,  //  000D  JMP		#001A
      0xAC100002,  //  000E  CATCH	R4	0	2
      0x70020008,  //  000F  JMP		#0019
      0xA41A0200,  //  0010  IMPORT	R6	K1
      0x601C0001,  //  0011  GETGBL	R7	G1
      0x8C200D02,  //  0012  GETMET	R8	R6	K2
      0x58280003,  //  0013  LDCONST	R10	K3
      0x5C2C0800,  //  0014  MOVE	R11	R4
      0x5C300A00,  //  0015  MOVE	R12	R5
      0x7C200800,  //  0016  CALL	R8	4
      0x7C1C0200,  //  0017  CALL	R7	1
      0x70020000,  //  0018  JMP		#001A
      0xB0080000,  //  0019  RAISE	2	R0	R0
      0x60100009,  //  001A  GETGBL	R4	G9
      0x5C140400,  //  001B  MOVE	R5	R2
      0x7C100200,  //  001C  CALL	R4	1
      0x90020804,  //  001D  SETMBR	R0	K4	R4
      0x50100000,  //  001E  LDBOOL	R4	0	0
      0x80040800,  //  001F  RET	1	R4
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
    be_str_literal("get_mode"),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
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
    /* K0   */  be_nested_str_literal("get_text"),
    }),
    be_str_literal("get_value_str"),
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
    /* K0   */  be_nested_str_literal("_lv_part2_selector"),
    /* K1   */  be_nested_str_literal("_lv_obj"),
    /* K2   */  be_nested_str_literal("set_style_pad_bottom"),
    /* K3   */  be_nested_str_literal("lv"),
    /* K4   */  be_nested_str_literal("STATE_DEFAULT"),
    }),
    be_str_literal("set_pad_bottom2"),
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
    /* K0   */  be_nested_str_literal("_text_rule_format"),
    }),
    be_str_literal("set_text_rule_format"),
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
    /* K0   */  be_nested_str_literal("set_text_font"),
    }),
    be_str_literal("set_value_font"),
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
    be_str_literal("get_text_font"),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
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
    /* K0   */  be_nested_str_literal("_action"),
    }),
    be_str_literal("set_action"),
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
    be_str_literal("get_pad_all"),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
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
    /* K0   */  be_nested_str_literal("_lv_part2_selector"),
    /* K1   */  be_nested_str_literal("_lv_obj"),
    /* K2   */  be_nested_str_literal("get_style_radius"),
    /* K3   */  be_nested_str_literal("lv"),
    /* K4   */  be_nested_str_literal("STATE_DEFAULT"),
    }),
    be_str_literal("get_radius2"),
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
    /* K0   */  be_nested_str_literal("_lv_part2_selector"),
    /* K1   */  be_nested_str_literal("_lv_obj"),
    /* K2   */  be_nested_str_literal("set_style_pad_right"),
    /* K3   */  be_nested_str_literal("lv"),
    /* K4   */  be_nested_str_literal("STATE_DEFAULT"),
    }),
    be_str_literal("set_pad_right2"),
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
    /* K0   */  be_nested_str_literal("get_text_font"),
    }),
    be_str_literal("get_value_font"),
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
    /* K0   */  be_nested_str_literal("_lv_label"),
    /* K1   */  be_nested_str_literal("get_style_text_align"),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str_literal("lv"),
    /* K4   */  be_nested_str_literal("TEXT_ALIGN_LEFT"),
    /* K5   */  be_nested_str_literal("left"),
    /* K6   */  be_nested_str_literal("TEXT_ALIGN_CENTER"),
    /* K7   */  be_nested_str_literal("center"),
    /* K8   */  be_nested_str_literal("TEXT_ALIGN_RIGHT"),
    /* K9   */  be_nested_str_literal("right"),
    }),
    be_str_literal("get_align"),
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
** Solidified function: parse_color
********************************************************************/
be_local_closure(lvh_obj_parse_color,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
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
        /* K0   */  be_nested_str_literal("string"),
        /* K1   */  be_nested_str_literal("toupper"),
        /* K2   */  be_const_int(0),
        /* K3   */  be_const_int(1),
        /* K4   */  be_nested_str_literal("#"),
        /* K5   */  be_nested_str_literal("x"),
        /* K6   */  be_nested_str_literal("X"),
        /* K7   */  be_nested_str_literal("A"),
        /* K8   */  be_nested_str_literal("F"),
        /* K9   */  be_nested_str_literal("byte"),
        /* K10  */  be_nested_str_literal("0"),
        /* K11  */  be_nested_str_literal("9"),
        /* K12  */  be_nested_str_literal("stop_iteration"),
        }),
        be_str_literal("parse_hex"),
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
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_literal("#"),
    /* K2   */  be_nested_str_literal("lv"),
    /* K3   */  be_nested_str_literal("color"),
    /* K4   */  be_nested_str_literal("string"),
    /* K5   */  be_nested_str_literal("introspect"),
    /* K6   */  be_nested_str_literal("COLOR_"),
    /* K7   */  be_nested_str_literal("toupper"),
    /* K8   */  be_nested_str_literal("get"),
    }),
    be_str_literal("parse_color"),
    &be_const_str_solidified,
    ( &(const binstruction[39]) {  /* code */
      0x84040000,  //  0000  CLOSURE	R1	P0
      0x60080008,  //  0001  GETGBL	R2	G8
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C080200,  //  0003  CALL	R2	1
      0x5C000400,  //  0004  MOVE	R0	R2
      0x94080100,  //  0005  GETIDX	R2	R0	K0
      0x1C080501,  //  0006  EQ	R2	R2	K1
      0x780A0007,  //  0007  JMPF	R2	#0010
      0xB80A0400,  //  0008  GETNGBL	R2	K2
      0x8C080503,  //  0009  GETMET	R2	R2	K3
      0x5C100200,  //  000A  MOVE	R4	R1
      0x5C140000,  //  000B  MOVE	R5	R0
      0x7C100200,  //  000C  CALL	R4	1
      0x7C080400,  //  000D  CALL	R2	2
      0x80040400,  //  000E  RET	1	R2
      0x70020011,  //  000F  JMP		#0022
      0xA40A0800,  //  0010  IMPORT	R2	K4
      0xA40E0A00,  //  0011  IMPORT	R3	K5
      0x8C100507,  //  0012  GETMET	R4	R2	K7
      0x5C180000,  //  0013  MOVE	R6	R0
      0x7C100400,  //  0014  CALL	R4	2
      0x00120C04,  //  0015  ADD	R4	K6	R4
      0x8C140708,  //  0016  GETMET	R5	R3	K8
      0xB81E0400,  //  0017  GETNGBL	R7	K2
      0x5C200800,  //  0018  MOVE	R8	R4
      0x7C140600,  //  0019  CALL	R5	3
      0x4C180000,  //  001A  LDNIL	R6
      0x20180A06,  //  001B  NE	R6	R5	R6
      0x781A0004,  //  001C  JMPF	R6	#0022
      0xB81A0400,  //  001D  GETNGBL	R6	K2
      0x8C180D03,  //  001E  GETMET	R6	R6	K3
      0x5C200A00,  //  001F  MOVE	R8	R5
      0x7C180400,  //  0020  CALL	R6	2
      0x80040C00,  //  0021  RET	1	R6
      0xB80A0400,  //  0022  GETNGBL	R2	K2
      0x8C080503,  //  0023  GETMET	R2	R2	K3
      0x58100000,  //  0024  LDCONST	R4	K0
      0x7C080400,  //  0025  CALL	R2	2
      0x80040400,  //  0026  RET	1	R2
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
    /* K0   */  be_nested_str_literal("_lv_part2_selector"),
    /* K1   */  be_nested_str_literal("_lv_obj"),
    /* K2   */  be_nested_str_literal("get_style_pad_left"),
    /* K3   */  be_nested_str_literal("lv"),
    /* K4   */  be_nested_str_literal("STATE_DEFAULT"),
    }),
    be_str_literal("get_pad_left"),
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
    /* K0   */  be_nested_str_literal("_val_rule"),
    }),
    be_str_literal("get_val_rule"),
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
    /* K0   */  be_nested_str_literal("set_text_color"),
    }),
    be_str_literal("set_value_color"),
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
** Solidified function: set_text_rule_formula
********************************************************************/
be_local_closure(lvh_obj_set_text_rule_formula,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_literal("_text_rule_formula"),
    /* K1   */  be_nested_str_literal("return / val -> ("),
    /* K2   */  be_nested_str_literal(")"),
    /* K3   */  be_nested_str_literal("_text_rule_function"),
    /* K4   */  be_nested_str_literal("string"),
    /* K5   */  be_nested_str_literal("format"),
    /* K6   */  be_nested_str_literal("HSP: failed to compile '%s' - %s (%s)"),
    }),
    be_str_literal("set_text_rule_formula"),
    &be_const_str_solidified,
    ( &(const binstruction[30]) {  /* code */
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
      0x7002000C,  //  000F  JMP		#001D
      0xAC0C0002,  //  0010  CATCH	R3	0	2
      0x70020009,  //  0011  JMP		#001C
      0xA4160800,  //  0012  IMPORT	R5	K4
      0x60180001,  //  0013  GETGBL	R6	G1
      0x8C1C0B05,  //  0014  GETMET	R7	R5	K5
      0x58240006,  //  0015  LDCONST	R9	K6
      0x5C280400,  //  0016  MOVE	R10	R2
      0x5C2C0600,  //  0017  MOVE	R11	R3
      0x5C300800,  //  0018  MOVE	R12	R4
      0x7C1C0A00,  //  0019  CALL	R7	5
      0x7C180200,  //  001A  CALL	R6	1
      0x70020000,  //  001B  JMP		#001D
      0xB0080000,  //  001C  RAISE	2	R0	R0
      0x80000000,  //  001D  RET	0
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
    /* K0   */  be_nested_str_literal("_lv_obj"),
    }),
    be_str_literal("get_obj"),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_action
********************************************************************/
be_local_closure(lvh_obj_get_action,   /* name */
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
    /* K0   */  be_nested_str_literal("_action"),
    }),
    be_str_literal("get_action"),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
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
    /* K0   */  be_nested_str_literal("_lv_obj"),
    /* K1   */  be_nested_str_literal("add_flag"),
    /* K2   */  be_nested_str_literal("lv"),
    /* K3   */  be_nested_str_literal("OBJ_FLAG_CLICKABLE"),
    /* K4   */  be_nested_str_literal("clear_flag"),
    }),
    be_str_literal("set_enabled"),
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
    /* K0   */  be_nested_str_literal("_lv_label"),
    /* K1   */  be_nested_str_literal("lv"),
    /* K2   */  be_nested_str_literal("label"),
    /* K3   */  be_nested_str_literal("get_obj"),
    /* K4   */  be_nested_str_literal("set_align"),
    /* K5   */  be_nested_str_literal("ALIGN_CENTER"),
    }),
    be_str_literal("check_label"),
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
** Solidified function: register_event_cb
********************************************************************/
be_local_closure(lvh_obj_register_event_cb,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 1]) {
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
        /* K0   */  be_nested_str_literal("event_cb"),
        }),
        be_str_literal("<lambda>"),
        &be_const_str_solidified,
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
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_literal("_event_map"),
    /* K1   */  be_nested_str_literal("keys"),
    /* K2   */  be_nested_str_literal("_lv_obj"),
    /* K3   */  be_nested_str_literal("add_event_cb"),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_literal("stop_iteration"),
    }),
    be_str_literal("register_event_cb"),
    &be_const_str_solidified,
    ( &(const binstruction[21]) {  /* code */
      0x84040000,  //  0000  CLOSURE	R1	P0
      0x60080010,  //  0001  GETGBL	R2	G16
      0x880C0100,  //  0002  GETMBR	R3	R0	K0
      0x8C0C0701,  //  0003  GETMET	R3	R3	K1
      0x7C0C0200,  //  0004  CALL	R3	1
      0x7C080200,  //  0005  CALL	R2	1
      0xA8020008,  //  0006  EXBLK	0	#0010
      0x5C0C0400,  //  0007  MOVE	R3	R2
      0x7C0C0000,  //  0008  CALL	R3	0
      0x88100102,  //  0009  GETMBR	R4	R0	K2
      0x8C100903,  //  000A  GETMET	R4	R4	K3
      0x5C180200,  //  000B  MOVE	R6	R1
      0x5C1C0600,  //  000C  MOVE	R7	R3
      0x58200004,  //  000D  LDCONST	R8	K4
      0x7C100800,  //  000E  CALL	R4	4
      0x7001FFF6,  //  000F  JMP		#0007
      0x58080005,  //  0010  LDCONST	R2	K5
      0xAC080200,  //  0011  CATCH	R2	1	0
      0xB0080000,  //  0012  RAISE	2	R0	R0
      0xA0000000,  //  0013  CLOSE	R0
      0x80000000,  //  0014  RET	0
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
    /* K0   */  be_nested_str_literal("_text_rule_format"),
    }),
    be_str_literal("get_text_rule_format"),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
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
    /* K0   */  be_nested_str_literal("set_text"),
    }),
    be_str_literal("set_value_str"),
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
    /* K0   */  be_nested_str_literal("register_event_cb"),
    }),
    be_str_literal("post_init"),
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
    /* K0   */  be_nested_str_literal("_lv_obj"),
    /* K1   */  be_nested_str_literal("has_flag"),
    /* K2   */  be_nested_str_literal("lv"),
    /* K3   */  be_nested_str_literal("OBJ_FLAG_CLICKABLE"),
    }),
    be_str_literal("get_adjustable"),
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
    /* K0   */  be_nested_str_literal("_lv_obj"),
    /* K1   */  be_nested_str_literal("has_flag"),
    /* K2   */  be_nested_str_literal("lv"),
    /* K3   */  be_nested_str_literal("OBJ_FLAG_HIDDEN"),
    }),
    be_str_literal("get_hidden"),
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
    /* K0   */  be_nested_str_literal("_lv_label"),
    /* K1   */  be_nested_str_literal("get_x"),
    }),
    be_str_literal("get_value_ofs_x"),
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
    /* K0   */  be_nested_str_literal("_lv_part2_selector"),
    /* K1   */  be_nested_str_literal("_lv_obj"),
    /* K2   */  be_nested_str_literal("set_style_radius"),
    /* K3   */  be_nested_str_literal("lv"),
    /* K4   */  be_nested_str_literal("STATE_DEFAULT"),
    }),
    be_str_literal("set_radius2"),
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
** Solidified function: get_line_width
********************************************************************/
be_local_closure(lvh_obj_get_line_width,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_literal("_lv_obj"),
    /* K1   */  be_nested_str_literal("get_style_line_width"),
    /* K2   */  be_const_int(0),
    }),
    be_str_literal("get_line_width"),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x580C0002,  //  0002  LDCONST	R3	K2
      0x7C040400,  //  0003  CALL	R1	2
      0x80040200,  //  0004  RET	1	R1
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
    /* K0   */  be_nested_str_literal("get_enabled"),
    }),
    be_str_literal("get_click"),
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
    /* K0   */  be_nested_str_literal("check_label"),
    /* K1   */  be_nested_str_literal("_lv_label"),
    /* K2   */  be_nested_str_literal("set_x"),
    }),
    be_str_literal("set_value_ofs_x"),
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
    /* K0   */  be_nested_str_literal("_lv_part2_selector"),
    /* K1   */  be_nested_str_literal("_lv_obj"),
    /* K2   */  be_nested_str_literal("get_style_pad_right"),
    /* K3   */  be_nested_str_literal("lv"),
    /* K4   */  be_nested_str_literal("STATE_DEFAULT"),
    }),
    be_str_literal("get_pad_right"),
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
    /* K0   */  be_nested_str_literal("_lv_part2_selector"),
    /* K1   */  be_nested_str_literal("_lv_obj"),
    /* K2   */  be_nested_str_literal("get_style_pad_bottom"),
    /* K3   */  be_nested_str_literal("lv"),
    /* K4   */  be_nested_str_literal("STATE_DEFAULT"),
    }),
    be_str_literal("get_pad_bottom"),
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
    /* K0   */  be_nested_str_literal("_val_rule_formula"),
    }),
    be_str_literal("get_val_rule_formula"),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: member
********************************************************************/
be_local_closure(lvh_obj_member,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[14]) {     /* constants */
    /* K0   */  be_nested_str_literal("string"),
    /* K1   */  be_nested_str_literal("_attr_ignore"),
    /* K2   */  be_nested_str_literal("find"),
    /* K3   */  be_nested_str_literal("_attr_map"),
    /* K4   */  be_nested_str_literal("contains"),
    /* K5   */  be_nested_str_literal("introspect"),
    /* K6   */  be_nested_str_literal("get"),
    /* K7   */  be_nested_str_literal("get_"),
    /* K8   */  be_nested_str_literal("function"),
    /* K9   */  be_nested_str_literal("_lv_obj"),
    /* K10  */  be_nested_str_literal("style_"),
    /* K11  */  be_const_int(0),
    /* K12  */  be_nested_str_literal("unknown attribute "),
    /* K13  */  be_nested_str_literal("value_error"),
    }),
    be_str_literal("member"),
    &be_const_str_solidified,
    ( &(const binstruction[65]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x880C0101,  //  0001  GETMBR	R3	R0	K1
      0x8C0C0702,  //  0002  GETMET	R3	R3	K2
      0x5C140200,  //  0003  MOVE	R5	R1
      0x7C0C0400,  //  0004  CALL	R3	2
      0x4C100000,  //  0005  LDNIL	R4
      0x200C0604,  //  0006  NE	R3	R3	R4
      0x780E0000,  //  0007  JMPF	R3	#0009
      0x80000600,  //  0008  RET	0
      0x880C0103,  //  0009  GETMBR	R3	R0	K3
      0x8C0C0704,  //  000A  GETMET	R3	R3	K4
      0x5C140200,  //  000B  MOVE	R5	R1
      0x7C0C0400,  //  000C  CALL	R3	2
      0x780E002C,  //  000D  JMPF	R3	#003B
      0xA40E0A00,  //  000E  IMPORT	R3	K5
      0x88100103,  //  000F  GETMBR	R4	R0	K3
      0x94100801,  //  0010  GETIDX	R4	R4	R1
      0x4C140000,  //  0011  LDNIL	R5
      0x1C140805,  //  0012  EQ	R5	R4	R5
      0x7816000D,  //  0013  JMPF	R5	#0022
      0x8C140706,  //  0014  GETMET	R5	R3	K6
      0x5C1C0000,  //  0015  MOVE	R7	R0
      0x00220E01,  //  0016  ADD	R8	K7	R1
      0x7C140600,  //  0017  CALL	R5	3
      0x60180004,  //  0018  GETGBL	R6	G4
      0x5C1C0A00,  //  0019  MOVE	R7	R5
      0x7C180200,  //  001A  CALL	R6	1
      0x1C180D08,  //  001B  EQ	R6	R6	K8
      0x781A0003,  //  001C  JMPF	R6	#0021
      0x5C180A00,  //  001D  MOVE	R6	R5
      0x5C1C0000,  //  001E  MOVE	R7	R0
      0x7C180200,  //  001F  CALL	R6	1
      0x80040C00,  //  0020  RET	1	R6
      0x70020018,  //  0021  JMP		#003B
      0x8C140706,  //  0022  GETMET	R5	R3	K6
      0x881C0109,  //  0023  GETMBR	R7	R0	K9
      0x00220E04,  //  0024  ADD	R8	K7	R4
      0x7C140600,  //  0025  CALL	R5	3
      0x60180004,  //  0026  GETGBL	R6	G4
      0x5C1C0A00,  //  0027  MOVE	R7	R5
      0x7C180200,  //  0028  CALL	R6	1
      0x1C180D08,  //  0029  EQ	R6	R6	K8
      0x781A000F,  //  002A  JMPF	R6	#003B
      0x8C180502,  //  002B  GETMET	R6	R2	K2
      0x5C200800,  //  002C  MOVE	R8	R4
      0x5824000A,  //  002D  LDCONST	R9	K10
      0x7C180600,  //  002E  CALL	R6	3
      0x1C180D0B,  //  002F  EQ	R6	R6	K11
      0x781A0005,  //  0030  JMPF	R6	#0037
      0x5C180A00,  //  0031  MOVE	R6	R5
      0x881C0109,  //  0032  GETMBR	R7	R0	K9
      0x5820000B,  //  0033  LDCONST	R8	K11
      0x7C180400,  //  0034  CALL	R6	2
      0x80040C00,  //  0035  RET	1	R6
      0x70020003,  //  0036  JMP		#003B
      0x5C180A00,  //  0037  MOVE	R6	R5
      0x881C0109,  //  0038  GETMBR	R7	R0	K9
      0x7C180200,  //  0039  CALL	R6	1
      0x80040C00,  //  003A  RET	1	R6
      0x600C0008,  //  003B  GETGBL	R3	G8
      0x5C100200,  //  003C  MOVE	R4	R1
      0x7C0C0200,  //  003D  CALL	R3	1
      0x000E1803,  //  003E  ADD	R3	K12	R3
      0xB0061A03,  //  003F  RAISE	1	K13	R3
      0x80000000,  //  0040  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_line_width
********************************************************************/
be_local_closure(lvh_obj_set_line_width,   /* name */
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
    /* K0   */  be_nested_str_literal("_lv_obj"),
    /* K1   */  be_nested_str_literal("set_style_line_width"),
    /* K2   */  be_const_int(0),
    }),
    be_str_literal("set_line_width"),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x60100009,  //  0002  GETGBL	R4	G9
      0x5C140200,  //  0003  MOVE	R5	R1
      0x7C100200,  //  0004  CALL	R4	1
      0x58140002,  //  0005  LDCONST	R5	K2
      0x7C080600,  //  0006  CALL	R2	3
      0x80000000,  //  0007  RET	0
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
    /* K0   */  be_nested_str_literal("_lv_part2_selector"),
    /* K1   */  be_nested_str_literal("_lv_obj"),
    /* K2   */  be_nested_str_literal("set_style_pad_top"),
    /* K3   */  be_nested_str_literal("lv"),
    /* K4   */  be_nested_str_literal("STATE_DEFAULT"),
    }),
    be_str_literal("set_pad_top2"),
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
    /* K0   */  be_nested_str_literal("_lv_label"),
    /* K1   */  be_nested_str_literal("get_text"),
    }),
    be_str_literal("get_text"),
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
    /* K0   */  be_nested_str_literal("_lv_obj"),
    /* K1   */  be_nested_str_literal("add_flag"),
    /* K2   */  be_nested_str_literal("lv"),
    /* K3   */  be_nested_str_literal("OBJ_FLAG_CLICKABLE"),
    /* K4   */  be_nested_str_literal("clear_flag"),
    }),
    be_str_literal("set_adjustable"),
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
** Solidified function: set_text_font
********************************************************************/
be_local_closure(lvh_obj_set_text_font,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[14]) {     /* constants */
    /* K0   */  be_nested_str_literal("int"),
    /* K1   */  be_nested_str_literal("lv"),
    /* K2   */  be_nested_str_literal("font_embedded"),
    /* K3   */  be_nested_str_literal("robotocondensed"),
    /* K4   */  be_nested_str_literal("montserrat"),
    /* K5   */  be_nested_str_literal("string"),
    /* K6   */  be_nested_str_literal("split"),
    /* K7   */  be_nested_str_literal("-"),
    /* K8   */  be_const_int(2),
    /* K9   */  be_const_int(0),
    /* K10  */  be_nested_str_literal("concat"),
    /* K11  */  be_nested_str_literal("_lv_obj"),
    /* K12  */  be_nested_str_literal("set_style_text_font"),
    /* K13  */  be_nested_str_literal("HSP: Unsupported font:"),
    }),
    be_str_literal("set_text_font"),
    &be_const_str_solidified,
    ( &(const binstruction[93]) {  /* code */
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
      0x7002002C,  //  0021  JMP		#004F
      0x600C0004,  //  0022  GETGBL	R3	G4
      0x5C100200,  //  0023  MOVE	R4	R1
      0x7C0C0200,  //  0024  CALL	R3	1
      0x1C0C0705,  //  0025  EQ	R3	R3	K5
      0x780E0027,  //  0026  JMPF	R3	#004F
      0xA40E0A00,  //  0027  IMPORT	R3	K5
      0x8C100706,  //  0028  GETMET	R4	R3	K6
      0x5C180200,  //  0029  MOVE	R6	R1
      0x581C0007,  //  002A  LDCONST	R7	K7
      0x7C100600,  //  002B  CALL	R4	3
      0x6014000C,  //  002C  GETGBL	R5	G12
      0x5C180800,  //  002D  MOVE	R6	R4
      0x7C140200,  //  002E  CALL	R5	1
      0x28140B08,  //  002F  GE	R5	R5	K8
      0x7816001D,  //  0030  JMPF	R5	#004F
      0x60140009,  //  0031  GETGBL	R5	G9
      0x5419FFFE,  //  0032  LDINT	R6	-1
      0x94180806,  //  0033  GETIDX	R6	R4	R6
      0x7C140200,  //  0034  CALL	R5	1
      0x5419FFFD,  //  0035  LDINT	R6	-2
      0x401A1206,  //  0036  CONNECT	R6	K9	R6
      0x94180806,  //  0037  GETIDX	R6	R4	R6
      0x8C180D0A,  //  0038  GETMET	R6	R6	K10
      0x58200007,  //  0039  LDCONST	R8	K7
      0x7C180400,  //  003A  CALL	R6	2
      0x241C0B09,  //  003B  GT	R7	R5	K9
      0x781E0011,  //  003C  JMPF	R7	#004F
      0x601C000C,  //  003D  GETGBL	R7	G12
      0x5C200C00,  //  003E  MOVE	R8	R6
      0x7C1C0200,  //  003F  CALL	R7	1
      0x241C0F09,  //  0040  GT	R7	R7	K9
      0x781E000C,  //  0041  JMPF	R7	#004F
      0xA8020007,  //  0042  EXBLK	0	#004B
      0xB81E0200,  //  0043  GETNGBL	R7	K1
      0x8C1C0F02,  //  0044  GETMET	R7	R7	K2
      0x5C240C00,  //  0045  MOVE	R9	R6
      0x5C280A00,  //  0046  MOVE	R10	R5
      0x7C1C0600,  //  0047  CALL	R7	3
      0x5C080E00,  //  0048  MOVE	R2	R7
      0xA8040001,  //  0049  EXBLK	1	1
      0x70020003,  //  004A  JMP		#004F
      0xAC1C0000,  //  004B  CATCH	R7	0	0
      0x70020000,  //  004C  JMP		#004E
      0x70020000,  //  004D  JMP		#004F
      0xB0080000,  //  004E  RAISE	2	R0	R0
      0x4C0C0000,  //  004F  LDNIL	R3
      0x200C0403,  //  0050  NE	R3	R2	R3
      0x780E0005,  //  0051  JMPF	R3	#0058
      0x880C010B,  //  0052  GETMBR	R3	R0	K11
      0x8C0C070C,  //  0053  GETMET	R3	R3	K12
      0x5C140400,  //  0054  MOVE	R5	R2
      0x58180009,  //  0055  LDCONST	R6	K9
      0x7C0C0600,  //  0056  CALL	R3	3
      0x70020003,  //  0057  JMP		#005C
      0x600C0001,  //  0058  GETGBL	R3	G1
      0x5810000D,  //  0059  LDCONST	R4	K13
      0x5C140200,  //  005A  MOVE	R5	R1
      0x7C0C0400,  //  005B  CALL	R3	2
      0x80000000,  //  005C  RET	0
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
    /* K0   */  be_nested_str_literal("_lv_label"),
    /* K1   */  be_nested_str_literal("get_y"),
    }),
    be_str_literal("get_value_ofs_y"),
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
    /* K0   */  be_nested_str_literal("_lv_part2_selector"),
    /* K1   */  be_nested_str_literal("_lv_obj"),
    /* K2   */  be_nested_str_literal("get_style_pad_top"),
    /* K3   */  be_nested_str_literal("lv"),
    /* K4   */  be_nested_str_literal("STATE_DEFAULT"),
    }),
    be_str_literal("get_pad_top"),
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
** Solidified function: is_color_attribute
********************************************************************/
be_local_closure(lvh_obj_is_color_attribute,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_literal("re"),
    /* K1   */  be_nested_str_literal("search"),
    /* K2   */  be_nested_str_literal("color$"),
    }),
    be_str_literal("is_color_attribute"),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x60080017,  //  0001  GETGBL	R2	G23
      0x8C0C0301,  //  0002  GETMET	R3	R1	K1
      0x58140002,  //  0003  LDCONST	R5	K2
      0x60180008,  //  0004  GETGBL	R6	G8
      0x5C1C0000,  //  0005  MOVE	R7	R0
      0x7C180200,  //  0006  CALL	R6	1
      0x7C0C0600,  //  0007  CALL	R3	3
      0x7C080200,  //  0008  CALL	R2	1
      0x80040400,  //  0009  RET	1	R2
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
    /* K0   */  be_nested_str_literal("check_label"),
    /* K1   */  be_nested_str_literal("_lv_label"),
    /* K2   */  be_nested_str_literal("set_y"),
    }),
    be_str_literal("set_value_ofs_y"),
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
    /* K0   */  be_nested_str_literal("expand"),
    /* K1   */  be_nested_str_literal("_lv_obj"),
    /* K2   */  be_nested_str_literal("set_width"),
    /* K3   */  be_nested_str_literal("lv"),
    /* K4   */  be_nested_str_literal("SIZE_CONTENT"),
    /* K5   */  be_nested_str_literal("break"),
    /* K6   */  be_nested_str_literal("LABEL_LONG_WRAP"),
    /* K7   */  be_nested_str_literal("dots"),
    /* K8   */  be_nested_str_literal("LABEL_LONG_DOT"),
    /* K9   */  be_nested_str_literal("scroll"),
    /* K10  */  be_nested_str_literal("LABEL_LONG_SCROLL"),
    /* K11  */  be_nested_str_literal("loop"),
    /* K12  */  be_nested_str_literal("LABEL_LONG_SCROLL_CIRCULAR"),
    /* K13  */  be_nested_str_literal("crop"),
    /* K14  */  be_nested_str_literal("LABEL_LONG_CLIP"),
    /* K15  */  be_nested_str_literal("check_label"),
    /* K16  */  be_nested_str_literal("_lv_label"),
    /* K17  */  be_nested_str_literal("set_long_mode"),
    }),
    be_str_literal("set_mode"),
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
** Solidified function: set_val_rule_formula
********************************************************************/
be_local_closure(lvh_obj_set_val_rule_formula,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_literal("_val_rule_formula"),
    /* K1   */  be_nested_str_literal("return / val -> ("),
    /* K2   */  be_nested_str_literal(")"),
    /* K3   */  be_nested_str_literal("_val_rule_function"),
    /* K4   */  be_nested_str_literal("string"),
    /* K5   */  be_nested_str_literal("format"),
    /* K6   */  be_nested_str_literal("HSP: failed to compile '%s' - %s (%s)"),
    }),
    be_str_literal("set_val_rule_formula"),
    &be_const_str_solidified,
    ( &(const binstruction[30]) {  /* code */
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
      0x7002000C,  //  000F  JMP		#001D
      0xAC0C0002,  //  0010  CATCH	R3	0	2
      0x70020009,  //  0011  JMP		#001C
      0xA4160800,  //  0012  IMPORT	R5	K4
      0x60180001,  //  0013  GETGBL	R6	G1
      0x8C1C0B05,  //  0014  GETMET	R7	R5	K5
      0x58240006,  //  0015  LDCONST	R9	K6
      0x5C280400,  //  0016  MOVE	R10	R2
      0x5C2C0600,  //  0017  MOVE	R11	R3
      0x5C300800,  //  0018  MOVE	R12	R4
      0x7C1C0A00,  //  0019  CALL	R7	5
      0x7C180200,  //  001A  CALL	R6	1
      0x70020000,  //  001B  JMP		#001D
      0xB0080000,  //  001C  RAISE	2	R0	R0
      0x80000000,  //  001D  RET	0
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
        /* K0   */  be_nested_str_literal("text_rule_matched"),
        }),
        be_str_literal("<lambda>"),
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
    /* K0   */  be_nested_str_literal("_text_rule"),
    /* K1   */  be_nested_str_literal("tasmota"),
    /* K2   */  be_nested_str_literal("remove_rule"),
    /* K3   */  be_nested_str_literal("add_rule"),
    }),
    be_str_literal("set_text_rule"),
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
** Solidified function: get_text_color
********************************************************************/
be_local_closure(lvh_obj_get_text_color,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_literal("_lv_obj"),
    /* K1   */  be_nested_str_literal("get_style_text_color"),
    /* K2   */  be_const_int(0),
    }),
    be_str_literal("get_text_color"),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x580C0002,  //  0002  LDCONST	R3	K2
      0x7C040400,  //  0003  CALL	R1	2
      0x80040200,  //  0004  RET	1	R1
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
    3,                          /* argc */
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
          be_local_const_upval(1, 3),
          be_local_const_upval(1, 0),
          be_local_const_upval(1, 4),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 1]) {     /* constants */
        /* K0   */  be_nested_str_literal("do_action"),
        }),
        be_str_literal("<lambda>"),
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
        /* K0   */  be_nested_str_literal("tasmota"),
        /* K1   */  be_nested_str_literal("publish_rule"),
        }),
        be_str_literal("<lambda>"),
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
    ( &(const bvalue[16]) {     /* constants */
    /* K0   */  be_nested_str_literal("_page"),
    /* K1   */  be_nested_str_literal("_oh"),
    /* K2   */  be_nested_str_literal("code"),
    /* K3   */  be_nested_str_literal("action"),
    /* K4   */  be_nested_str_literal("lv"),
    /* K5   */  be_nested_str_literal("EVENT_CLICKED"),
    /* K6   */  be_nested_str_literal("tasmota"),
    /* K7   */  be_nested_str_literal("set_timer"),
    /* K8   */  be_const_int(0),
    /* K9   */  be_nested_str_literal("_event_map"),
    /* K10  */  be_nested_str_literal("find"),
    /* K11  */  be_nested_str_literal("string"),
    /* K12  */  be_nested_str_literal("format"),
    /* K13  */  be_nested_str_literal("{\"hasp\":{\"p%ib%i\":\"%s\"}}"),
    /* K14  */  be_nested_str_literal("_page_id"),
    /* K15  */  be_nested_str_literal("id"),
    }),
    be_str_literal("event_cb"),
    &be_const_str_solidified,
    ( &(const binstruction[39]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x880C0701,  //  0001  GETMBR	R3	R3	K1
      0x88100502,  //  0002  GETMBR	R4	R2	K2
      0x88140103,  //  0003  GETMBR	R5	R0	K3
      0x4C180000,  //  0004  LDNIL	R6
      0x20140A06,  //  0005  NE	R5	R5	R6
      0x78160008,  //  0006  JMPF	R5	#0010
      0xB8160800,  //  0007  GETNGBL	R5	K4
      0x88140B05,  //  0008  GETMBR	R5	R5	K5
      0x1C140805,  //  0009  EQ	R5	R4	R5
      0x78160004,  //  000A  JMPF	R5	#0010
      0xB8160C00,  //  000B  GETNGBL	R5	K6
      0x8C140B07,  //  000C  GETMET	R5	R5	K7
      0x581C0008,  //  000D  LDCONST	R7	K8
      0x84200000,  //  000E  CLOSURE	R8	P0
      0x7C140600,  //  000F  CALL	R5	3
      0x88140109,  //  0010  GETMBR	R5	R0	K9
      0x8C140B0A,  //  0011  GETMET	R5	R5	K10
      0x5C1C0800,  //  0012  MOVE	R7	R4
      0x7C140400,  //  0013  CALL	R5	2
      0x4C180000,  //  0014  LDNIL	R6
      0x20180A06,  //  0015  NE	R6	R5	R6
      0x781A000D,  //  0016  JMPF	R6	#0025
      0xA41A1600,  //  0017  IMPORT	R6	K11
      0x8C1C0D0C,  //  0018  GETMET	R7	R6	K12
      0x5824000D,  //  0019  LDCONST	R9	K13
      0x88280100,  //  001A  GETMBR	R10	R0	K0
      0x8828150E,  //  001B  GETMBR	R10	R10	K14
      0x882C010F,  //  001C  GETMBR	R11	R0	K15
      0x5C300A00,  //  001D  MOVE	R12	R5
      0x7C1C0A00,  //  001E  CALL	R7	5
      0xB8220C00,  //  001F  GETNGBL	R8	K6
      0x8C201107,  //  0020  GETMET	R8	R8	K7
      0x58280008,  //  0021  LDCONST	R10	K8
      0x842C0001,  //  0022  CLOSURE	R11	P1
      0x7C200600,  //  0023  CALL	R8	3
      0xA0180000,  //  0024  CLOSE	R6
      0xA0000000,  //  0025  CLOSE	R0
      0x80000000,  //  0026  RET	0
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
    /* K0   */  be_nested_str_literal("check_label"),
    /* K1   */  be_const_int(0),
    /* K2   */  be_nested_str_literal("left"),
    /* K3   */  be_nested_str_literal("lv"),
    /* K4   */  be_nested_str_literal("TEXT_ALIGN_LEFT"),
    /* K5   */  be_const_int(1),
    /* K6   */  be_nested_str_literal("center"),
    /* K7   */  be_nested_str_literal("TEXT_ALIGN_CENTER"),
    /* K8   */  be_const_int(2),
    /* K9   */  be_nested_str_literal("right"),
    /* K10  */  be_nested_str_literal("TEXT_ALIGN_RIGHT"),
    /* K11  */  be_nested_str_literal("_lv_label"),
    /* K12  */  be_nested_str_literal("set_style_text_align"),
    }),
    be_str_literal("set_align"),
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
        /* K0   */  be_nested_str_literal("val_rule_matched"),
        }),
        be_str_literal("<lambda>"),
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
    /* K0   */  be_nested_str_literal("_val_rule"),
    /* K1   */  be_nested_str_literal("tasmota"),
    /* K2   */  be_nested_str_literal("remove_rule"),
    /* K3   */  be_nested_str_literal("add_rule"),
    }),
    be_str_literal("set_val_rule"),
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
** Solidified class: lvh_obj
********************************************************************/
be_local_class(lvh_obj,
    12,
    NULL,
    be_nested_map(85,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_literal("get_enabled", -1), be_const_closure(lvh_obj_get_enabled_closure) },
        { be_const_key_literal("set_val_rule", -1), be_const_closure(lvh_obj_set_val_rule_closure) },
        { be_const_key_literal("get_value_color", -1), be_const_closure(lvh_obj_get_value_color_closure) },
        { be_const_key_literal("set_click", 29), be_const_closure(lvh_obj_set_click_closure) },
        { be_const_key_literal("set_align", -1), be_const_closure(lvh_obj_set_align_closure) },
        { be_const_key_literal("set_hidden", 38), be_const_closure(lvh_obj_set_hidden_closure) },
        { be_const_key_literal("get_mode", -1), be_const_closure(lvh_obj_get_mode_closure) },
        { be_const_key_literal("get_text_rule_formula", 50), be_const_closure(lvh_obj_get_text_rule_formula_closure) },
        { be_const_key_literal("set_pad_all2", -1), be_const_closure(lvh_obj_set_pad_all2_closure) },
        { be_const_key_literal("text_rule_matched", -1), be_const_closure(lvh_obj_text_rule_matched_closure) },
        { be_const_key_literal("_val_rule", -1), be_const_var(5) },
        { be_const_key_literal("setmember", 63), be_const_closure(lvh_obj_setmember_closure) },
        { be_const_key_literal("get_text_font", -1), be_const_closure(lvh_obj_get_text_font_closure) },
        { be_const_key_literal("set_toggle", -1), be_const_closure(lvh_obj_set_toggle_closure) },
        { be_const_key_literal("get_toggle", 6), be_const_closure(lvh_obj_get_toggle_closure) },
        { be_const_key_literal("init", 31), be_const_closure(lvh_obj_init_closure) },
        { be_const_key_literal("id", -1), be_const_var(0) },
        { be_const_key_literal("val_rule_matched", -1), be_const_closure(lvh_obj_val_rule_matched_closure) },
        { be_const_key_literal("set_text_rule", 76), be_const_closure(lvh_obj_set_text_rule_closure) },
        { be_const_key_literal("get_value_str", 59), be_const_closure(lvh_obj_get_value_str_closure) },
        { be_const_key_literal("set_val_rule_formula", -1), be_const_closure(lvh_obj_set_val_rule_formula_closure) },
        { be_const_key_literal("set_value_color", -1), be_const_closure(lvh_obj_set_value_color_closure) },
        { be_const_key_literal("_lv_class", -1), be_const_class(be_class_lv_obj) },
        { be_const_key_literal("_text_rule_format", -1), be_const_var(11) },
        { be_const_key_literal("_val_rule_formula", 12), be_const_var(6) },
        { be_const_key_literal("set_action", -1), be_const_closure(lvh_obj_set_action_closure) },
        { be_const_key_literal("set_text_color", 79), be_const_closure(lvh_obj_set_text_color_closure) },
        { be_const_key_literal("_lv_part2_selector", -1), be_const_nil() },
        { be_const_key_literal("set_value_font", 71), be_const_closure(lvh_obj_set_value_font_closure) },
        { be_const_key_literal("get_obj", -1), be_const_closure(lvh_obj_get_obj_closure) },
        { be_const_key_literal("get_value_font", -1), be_const_closure(lvh_obj_get_value_font_closure) },
        { be_const_key_literal("set_text", -1), be_const_closure(lvh_obj_set_text_closure) },
        { be_const_key_literal("post_init", -1), be_const_closure(lvh_obj_post_init_closure) },
        { be_const_key_literal("_action", -1), be_const_var(4) },
        { be_const_key_literal("get_val_rule", 61), be_const_closure(lvh_obj_get_val_rule_closure) },
        { be_const_key_literal("get_align", 21), be_const_closure(lvh_obj_get_align_closure) },
        { be_const_key_literal("get_pad_top", 60), be_const_closure(lvh_obj_get_pad_top_closure) },
        { be_const_key_literal("parse_color", 18), be_const_static_closure(lvh_obj_parse_color_closure) },
        { be_const_key_literal("set_text_rule_format", -1), be_const_closure(lvh_obj_set_text_rule_format_closure) },
        { be_const_key_literal("get_action", -1), be_const_closure(lvh_obj_get_action_closure) },
        { be_const_key_literal("get_line_width", -1), be_const_closure(lvh_obj_get_line_width_closure) },
        { be_const_key_literal("check_label", -1), be_const_closure(lvh_obj_check_label_closure) },
        { be_const_key_literal("set_value_ofs_x", -1), be_const_closure(lvh_obj_set_value_ofs_x_closure) },
        { be_const_key_literal("get_text_rule_format", -1), be_const_closure(lvh_obj_get_text_rule_format_closure) },
        { be_const_key_literal("set_value_str", -1), be_const_closure(lvh_obj_set_value_str_closure) },
        { be_const_key_literal("get_pad_left", 32), be_const_closure(lvh_obj_get_pad_left_closure) },
        { be_const_key_literal("get_adjustable", -1), be_const_closure(lvh_obj_get_adjustable_closure) },
        { be_const_key_literal("_page", -1), be_const_var(3) },
        { be_const_key_literal("get_value_ofs_x", -1), be_const_closure(lvh_obj_get_value_ofs_x_closure) },
        { be_const_key_literal("set_radius2", 36), be_const_closure(lvh_obj_set_radius2_closure) },
        { be_const_key_literal("_attr_ignore", -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(10,
    ( (struct bvalue*) &(const bvalue[]) {
        be_nested_str_literal("tostring"),
        be_nested_str_literal("obj"),
        be_nested_str_literal("page"),
        be_nested_str_literal("comment"),
        be_nested_str_literal("parentid"),
        be_nested_str_literal("auto_size"),
        be_nested_str_literal("prev"),
        be_nested_str_literal("next"),
        be_nested_str_literal("back"),
        be_nested_str_literal("berry_run"),
    }))    ) } )) },
        { be_const_key_literal("set_pad_bottom2", 40), be_const_closure(lvh_obj_set_pad_bottom2_closure) },
        { be_const_key_literal("get_click", -1), be_const_closure(lvh_obj_get_click_closure) },
        { be_const_key_literal("register_event_cb", 42), be_const_closure(lvh_obj_register_event_cb_closure) },
        { be_const_key_literal("get_pad_right", -1), be_const_closure(lvh_obj_get_pad_right_closure) },
        { be_const_key_literal("get_pad_bottom", -1), be_const_closure(lvh_obj_get_pad_bottom_closure) },
        { be_const_key_literal("_lv_obj", -1), be_const_var(1) },
        { be_const_key_literal("get_val_rule_formula", -1), be_const_closure(lvh_obj_get_val_rule_formula_closure) },
        { be_const_key_literal("_lv_label", 57), be_const_var(2) },
        { be_const_key_literal("get_text", 62), be_const_closure(lvh_obj_get_text_closure) },
        { be_const_key_literal("_text_rule_formula", -1), be_const_var(9) },
        { be_const_key_literal("set_pad_top2", 55), be_const_closure(lvh_obj_set_pad_top2_closure) },
        { be_const_key_literal("set_line_width", -1), be_const_closure(lvh_obj_set_line_width_closure) },
        { be_const_key_literal("member", -1), be_const_closure(lvh_obj_member_closure) },
        { be_const_key_literal("_text_rule_function", -1), be_const_var(10) },
        { be_const_key_literal("set_adjustable", -1), be_const_closure(lvh_obj_set_adjustable_closure) },
        { be_const_key_literal("set_text_font", 23), be_const_closure(lvh_obj_set_text_font_closure) },
        { be_const_key_literal("set_pad_left2", 47), be_const_closure(lvh_obj_set_pad_left2_closure) },
        { be_const_key_literal("_attr_map", -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(60,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_literal("action", 54), be_const_nil() },
        { be_const_key_literal("pad_bottom", -1), be_nested_str_literal("style_pad_bottom") },
        { be_const_key_literal("min", 21), be_const_nil() },
        { be_const_key_literal("pad_right2", 58), be_const_nil() },
        { be_const_key_literal("angle", 1), be_const_nil() },
        { be_const_key_literal("val", 23), be_nested_str_literal("value") },
        { be_const_key_literal("text_color", 32), be_const_nil() },
        { be_const_key_literal("hidden", -1), be_const_nil() },
        { be_const_key_literal("speed", -1), be_const_nil() },
        { be_const_key_literal("image_recolor", -1), be_nested_str_literal("style_img_recolor") },
        { be_const_key_literal("value_str", -1), be_const_nil() },
        { be_const_key_literal("line_color", 20), be_nested_str_literal("style_line_color") },
        { be_const_key_literal("line_width", -1), be_const_nil() },
        { be_const_key_literal("toggle", 7), be_const_nil() },
        { be_const_key_literal("pad_all", -1), be_nested_str_literal("style_pad_all") },
        { be_const_key_literal("x", -1), be_nested_str_literal("x") },
        { be_const_key_literal("pad_left2", 5), be_const_nil() },
        { be_const_key_literal("radius", 30), be_nested_str_literal("style_radius") },
        { be_const_key_literal("mode", 48), be_const_nil() },
        { be_const_key_literal("val_rule", 17), be_const_nil() },
        { be_const_key_literal("bg_grad_color", 45), be_nested_str_literal("style_bg_grad_color") },
        { be_const_key_literal("value_ofs_y", 51), be_const_nil() },
        { be_const_key_literal("end_angle", -1), be_nested_str_literal("bg_end_angle") },
        { be_const_key_literal("y", -1), be_nested_str_literal("y") },
        { be_const_key_literal("image_recolor_opa", -1), be_nested_str_literal("style_img_recolor_opa") },
        { be_const_key_literal("text_rule_format", -1), be_const_nil() },
        { be_const_key_literal("value_color", 39), be_const_nil() },
        { be_const_key_literal("asjustable", 0), be_const_nil() },
        { be_const_key_literal("pad_all2", -1), be_const_nil() },
        { be_const_key_literal("max", -1), be_const_nil() },
        { be_const_key_literal("bg_opa", 53), be_nested_str_literal("style_bg_opa") },
        { be_const_key_literal("h", -1), be_nested_str_literal("height") },
        { be_const_key_literal("pad_left", 57), be_nested_str_literal("style_pad_left") },
        { be_const_key_literal("type", -1), be_const_nil() },
        { be_const_key_literal("text", -1), be_const_nil() },
        { be_const_key_literal("rotation", 2), be_nested_str_literal("rotation") },
        { be_const_key_literal("value_ofs_x", -1), be_const_nil() },
        { be_const_key_literal("src", -1), be_nested_str_literal("src") },
        { be_const_key_literal("start_angle1", -1), be_nested_str_literal("start_angle") },
        { be_const_key_literal("w", 43), be_nested_str_literal("width") },
        { be_const_key_literal("pad_top2", -1), be_const_nil() },
        { be_const_key_literal("end_angle1", 9), be_nested_str_literal("end_angle") },
        { be_const_key_literal("enabled", -1), be_const_nil() },
        { be_const_key_literal("align", -1), be_const_nil() },
        { be_const_key_literal("border_width", -1), be_nested_str_literal("style_border_width") },
        { be_const_key_literal("start_angle", -1), be_nested_str_literal("bg_start_angle") },
        { be_const_key_literal("val_rule_formula", 52), be_const_nil() },
        { be_const_key_literal("click", -1), be_const_nil() },
        { be_const_key_literal("bg_color", -1), be_nested_str_literal("style_bg_color") },
        { be_const_key_literal("text_rule", -1), be_const_nil() },
        { be_const_key_literal("pad_top", -1), be_nested_str_literal("style_pad_top") },
        { be_const_key_literal("line_width1", -1), be_const_nil() },
        { be_const_key_literal("pad_bottom2", 55), be_const_nil() },
        { be_const_key_literal("radius2", -1), be_const_nil() },
        { be_const_key_literal("bg_grad_dir", -1), be_nested_str_literal("style_bg_grad_dir") },
        { be_const_key_literal("text_font", -1), be_const_nil() },
        { be_const_key_literal("text_rule_formula", -1), be_const_nil() },
        { be_const_key_literal("value_font", -1), be_const_nil() },
        { be_const_key_literal("border_side", -1), be_nested_str_literal("style_border_side") },
        { be_const_key_literal("pad_right", -1), be_nested_str_literal("style_pad_right") },
    }))    ) } )) },
        { be_const_key_literal("get_value_ofs_y", -1), be_const_closure(lvh_obj_get_value_ofs_y_closure) },
        { be_const_key_literal("_text_rule", -1), be_const_var(8) },
        { be_const_key_literal("get_hidden", 78), be_const_closure(lvh_obj_get_hidden_closure) },
        { be_const_key_literal("set_pad_right2", 33), be_const_closure(lvh_obj_set_pad_right2_closure) },
        { be_const_key_literal("is_color_attribute", 20), be_const_static_closure(lvh_obj_is_color_attribute_closure) },
        { be_const_key_literal("set_value_ofs_y", -1), be_const_closure(lvh_obj_set_value_ofs_y_closure) },
        { be_const_key_literal("set_mode", -1), be_const_closure(lvh_obj_set_mode_closure) },
        { be_const_key_literal("set_text_rule_formula", -1), be_const_closure(lvh_obj_set_text_rule_formula_closure) },
        { be_const_key_literal("_event_map", -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(7,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(28, 2), be_nested_str_literal("changed") },
        { be_const_key_int(8, 4), be_nested_str_literal("release") },
        { be_const_key_int(7, -1), be_nested_str_literal("up") },
        { be_const_key_int(3, -1), be_nested_str_literal("lost") },
        { be_const_key_int(1, -1), be_nested_str_literal("down") },
        { be_const_key_int(5, -1), be_nested_str_literal("long") },
        { be_const_key_int(6, -1), be_nested_str_literal("hold") },
    }))    ) } )) },
        { be_const_key_literal("get_radius2", -1), be_const_closure(lvh_obj_get_radius2_closure) },
        { be_const_key_literal("get_pad_all", -1), be_const_closure(lvh_obj_get_pad_all_closure) },
        { be_const_key_literal("get_text_color", -1), be_const_closure(lvh_obj_get_text_color_closure) },
        { be_const_key_literal("_val_rule_function", 10), be_const_var(7) },
        { be_const_key_literal("event_cb", -1), be_const_closure(lvh_obj_event_cb_closure) },
        { be_const_key_literal("get_text_rule", 4), be_const_closure(lvh_obj_get_text_rule_closure) },
        { be_const_key_literal("set_enabled", 1), be_const_closure(lvh_obj_set_enabled_closure) },
    })),
    be_str_literal("lvh_obj")
);
/*******************************************************************/

void be_load_lvh_obj_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_lvh_obj);
    be_setglobal(vm, "lvh_obj");
    be_pop(vm, 1);
}

/********************************************************************
** Solidified class: lvh_scr
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_class(lvh_scr,
    0,
    &be_class_lvh_obj,
    be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_literal("_lv_class", -1), be_const_nil() },
    })),
    be_str_literal("lvh_scr")
);
/*******************************************************************/

void be_load_lvh_scr_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_lvh_scr);
    be_setglobal(vm, "lvh_scr");
    be_pop(vm, 1);
}

/********************************************************************
** Solidified class: lvh_btn
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_class(lvh_btn,
    0,
    &be_class_lvh_obj,
    be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_literal("_lv_class", -1), be_const_class(be_class_lv_btn) },
    })),
    be_str_literal("lvh_btn")
);
/*******************************************************************/

void be_load_lvh_btn_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_lvh_btn);
    be_setglobal(vm, "lvh_btn");
    be_pop(vm, 1);
}

/********************************************************************
** Solidified class: lvh_switch
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_class(lvh_switch,
    0,
    &be_class_lvh_obj,
    be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_literal("_lv_part2_selector", -1), be_const_int(196608) },
        { be_const_key_literal("_lv_class", -1), be_const_class(be_class_lv_switch) },
    })),
    be_str_literal("lvh_switch")
);
/*******************************************************************/

void be_load_lvh_switch_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_lvh_switch);
    be_setglobal(vm, "lvh_switch");
    be_pop(vm, 1);
}

/********************************************************************
** Solidified class: lvh_checkbox
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_class(lvh_checkbox,
    0,
    &be_class_lvh_obj,
    be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_literal("_lv_class", -1), be_const_class(be_class_lv_checkbox) },
    })),
    be_str_literal("lvh_checkbox")
);
/*******************************************************************/

void be_load_lvh_checkbox_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_lvh_checkbox);
    be_setglobal(vm, "lvh_checkbox");
    be_pop(vm, 1);
}

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
    /* K0   */  be_nested_str_literal("_lv_label"),
    /* K1   */  be_nested_str_literal("_lv_obj"),
    /* K2   */  be_nested_str_literal("post_init"),
    }),
    be_str_literal("post_init"),
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
        { be_const_key_literal("post_init", -1), be_const_closure(lvh_label_post_init_closure) },
        { be_const_key_literal("_lv_class", -1), be_const_class(be_class_lv_label) },
    })),
    be_str_literal("lvh_label")
);
/*******************************************************************/

void be_load_lvh_label_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_lvh_label);
    be_setglobal(vm, "lvh_label");
    be_pop(vm, 1);
}

/********************************************************************
** Solidified function: get_speed
********************************************************************/
be_local_closure(lvh_spinner_get_speed,   /* name */
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
    be_str_literal("get_speed"),
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
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_literal("_page"),
    /* K1   */  be_nested_str_literal("find"),
    /* K2   */  be_nested_str_literal("angle"),
    /* K3   */  be_nested_str_literal("speed"),
    /* K4   */  be_nested_str_literal("_lv_obj"),
    /* K5   */  be_nested_str_literal("lv"),
    /* K6   */  be_nested_str_literal("spinner"),
    /* K7   */  be_nested_str_literal("post_init"),
    }),
    be_str_literal("init"),
    &be_const_str_solidified,
    ( &(const binstruction[19]) {  /* code */
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
      0x80000000,  //  0012  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_speed
********************************************************************/
be_local_closure(lvh_spinner_set_speed,   /* name */
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
    be_str_literal("set_speed"),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_angle
********************************************************************/
be_local_closure(lvh_spinner_set_angle,   /* name */
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
    be_str_literal("set_angle"),
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
be_local_closure(lvh_spinner_get_angle,   /* name */
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
    be_str_literal("get_angle"),
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
    0,
    &be_class_lvh_arc,
    be_nested_map(6,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_literal("get_angle", 5), be_const_closure(lvh_spinner_get_angle_closure) },
        { be_const_key_literal("_lv_class", 0), be_const_class(be_class_lv_spinner) },
        { be_const_key_literal("set_angle", 4), be_const_closure(lvh_spinner_set_angle_closure) },
        { be_const_key_literal("init", 2), be_const_closure(lvh_spinner_init_closure) },
        { be_const_key_literal("set_speed", -1), be_const_closure(lvh_spinner_set_speed_closure) },
        { be_const_key_literal("get_speed", -1), be_const_closure(lvh_spinner_get_speed_closure) },
    })),
    be_str_literal("lvh_spinner")
);
/*******************************************************************/

void be_load_lvh_spinner_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_lvh_spinner);
    be_setglobal(vm, "lvh_spinner");
    be_pop(vm, 1);
}

/********************************************************************
** Solidified class: lvh_line
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_class(lvh_line,
    0,
    &be_class_lvh_obj,
    be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_literal("_lv_class", -1), be_const_class(be_class_lv_line) },
    })),
    be_str_literal("lvh_line")
);
/*******************************************************************/

void be_load_lvh_line_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_lvh_line);
    be_setglobal(vm, "lvh_line");
    be_pop(vm, 1);
}

/********************************************************************
** Solidified class: lvh_img
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_class(lvh_img,
    0,
    &be_class_lvh_obj,
    be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_literal("_lv_class", -1), be_const_class(be_class_lv_img) },
    })),
    be_str_literal("lvh_img")
);
/*******************************************************************/

void be_load_lvh_img_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_lvh_img);
    be_setglobal(vm, "lvh_img");
    be_pop(vm, 1);
}

/********************************************************************
** Solidified class: lvh_roller
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_class(lvh_roller,
    0,
    &be_class_lvh_obj,
    be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_literal("_lv_class", -1), be_const_class(be_class_lv_roller) },
    })),
    be_str_literal("lvh_roller")
);
/*******************************************************************/

void be_load_lvh_roller_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_lvh_roller);
    be_setglobal(vm, "lvh_roller");
    be_pop(vm, 1);
}

/********************************************************************
** Solidified class: lvh_btnmatrix
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_class(lvh_btnmatrix,
    0,
    &be_class_lvh_obj,
    be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_literal("_lv_class", -1), be_const_class(be_class_lv_btnmatrix) },
    })),
    be_str_literal("lvh_btnmatrix")
);
/*******************************************************************/

void be_load_lvh_btnmatrix_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_lvh_btnmatrix);
    be_setglobal(vm, "lvh_btnmatrix");
    be_pop(vm, 1);
}

/********************************************************************
** Solidified class: lvh_bar
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_class(lvh_bar,
    0,
    &be_class_lvh_obj,
    be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_literal("_lv_class", -1), be_const_class(be_class_lv_bar) },
    })),
    be_str_literal("lvh_bar")
);
/*******************************************************************/

void be_load_lvh_bar_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_lvh_bar);
    be_setglobal(vm, "lvh_bar");
    be_pop(vm, 1);
}

/********************************************************************
** Solidified class: lvh_slider
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_class(lvh_slider,
    0,
    &be_class_lvh_obj,
    be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_literal("_lv_class", -1), be_const_class(be_class_lv_slider) },
    })),
    be_str_literal("lvh_slider")
);
/*******************************************************************/

void be_load_lvh_slider_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_lvh_slider);
    be_setglobal(vm, "lvh_slider");
    be_pop(vm, 1);
}

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
    /* K0   */  be_nested_str_literal("_lv_obj"),
    /* K1   */  be_nested_str_literal("set_style_arc_width"),
    /* K2   */  be_nested_str_literal("lv"),
    /* K3   */  be_nested_str_literal("PART_INDICATOR"),
    /* K4   */  be_nested_str_literal("STATE_DEFAULT"),
    }),
    be_str_literal("set_line_width1"),
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
    /* K0   */  be_nested_str_literal("_lv_obj"),
    /* K1   */  be_nested_str_literal("get_min_value"),
    }),
    be_str_literal("get_min"),
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
    /* K0   */  be_nested_str_literal("_lv_obj"),
    /* K1   */  be_nested_str_literal("get_max_value"),
    }),
    be_str_literal("get_max"),
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
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_literal("_lv_obj"),
    /* K1   */  be_nested_str_literal("get_arc_line_width"),
    /* K2   */  be_const_int(0),
    }),
    be_str_literal("get_line_width"),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x580C0002,  //  0002  LDCONST	R3	K2
      0x7C040400,  //  0003  CALL	R1	2
      0x80040200,  //  0004  RET	1	R1
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
    /* K0   */  be_nested_str_literal("_lv_obj"),
    /* K1   */  be_nested_str_literal("get_mode"),
    }),
    be_str_literal("get_type"),
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
    /* K0   */  be_nested_str_literal("_lv_obj"),
    /* K1   */  be_nested_str_literal("set_range"),
    /* K2   */  be_nested_str_literal("get_max"),
    }),
    be_str_literal("set_min"),
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
    /* K0   */  be_nested_str_literal("_lv_obj"),
    /* K1   */  be_nested_str_literal("get_arc_line_width"),
    /* K2   */  be_nested_str_literal("lv"),
    /* K3   */  be_nested_str_literal("PART_INDICATOR"),
    /* K4   */  be_nested_str_literal("STATE_DEFAULT"),
    }),
    be_str_literal("get_line_width1"),
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
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_literal("_lv_obj"),
    /* K1   */  be_nested_str_literal("set_style_arc_width"),
    /* K2   */  be_const_int(0),
    }),
    be_str_literal("set_line_width"),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x60100009,  //  0002  GETGBL	R4	G9
      0x5C140200,  //  0003  MOVE	R5	R1
      0x7C100200,  //  0004  CALL	R4	1
      0x58140002,  //  0005  LDCONST	R5	K2
      0x7C080600,  //  0006  CALL	R2	3
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
    /* K0   */  be_nested_str_literal("_lv_obj"),
    /* K1   */  be_nested_str_literal("set_range"),
    /* K2   */  be_nested_str_literal("get_min"),
    }),
    be_str_literal("set_max"),
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
    /* K1   */  be_nested_str_literal("lv"),
    /* K2   */  be_nested_str_literal("ARC_MODE_NORMAL"),
    /* K3   */  be_const_int(1),
    /* K4   */  be_nested_str_literal("ARC_MODE_REVERSE"),
    /* K5   */  be_const_int(2),
    /* K6   */  be_nested_str_literal("ARC_MODE_SYMMETRICAL"),
    /* K7   */  be_nested_str_literal("_lv_obj"),
    /* K8   */  be_nested_str_literal("set_mode"),
    }),
    be_str_literal("set_type"),
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
        { be_const_key_literal("set_line_width1", 1), be_const_closure(lvh_arc_set_line_width1_closure) },
        { be_const_key_literal("set_type", -1), be_const_closure(lvh_arc_set_type_closure) },
        { be_const_key_literal("set_max", -1), be_const_closure(lvh_arc_set_max_closure) },
        { be_const_key_literal("set_line_width", -1), be_const_closure(lvh_arc_set_line_width_closure) },
        { be_const_key_literal("get_type", 11), be_const_closure(lvh_arc_get_type_closure) },
        { be_const_key_literal("get_line_width1", -1), be_const_closure(lvh_arc_get_line_width1_closure) },
        { be_const_key_literal("_lv_part2_selector", -1), be_const_int(196608) },
        { be_const_key_literal("_lv_class", -1), be_const_class(be_class_lv_arc) },
        { be_const_key_literal("get_min", 5), be_const_closure(lvh_arc_get_min_closure) },
        { be_const_key_literal("get_line_width", 3), be_const_closure(lvh_arc_get_line_width_closure) },
        { be_const_key_literal("get_max", 2), be_const_closure(lvh_arc_get_max_closure) },
        { be_const_key_literal("set_min", -1), be_const_closure(lvh_arc_set_min_closure) },
    })),
    be_str_literal("lvh_arc")
);
/*******************************************************************/

void be_load_lvh_arc_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_lvh_arc);
    be_setglobal(vm, "lvh_arc");
    be_pop(vm, 1);
}

/********************************************************************
** Solidified class: lvh_textarea
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_class(lvh_textarea,
    0,
    &be_class_lvh_obj,
    be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_literal("_lv_class", -1), be_const_class(be_class_lv_textarea) },
    })),
    be_str_literal("lvh_textarea")
);
/*******************************************************************/

void be_load_lvh_textarea_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_lvh_textarea);
    be_setglobal(vm, "lvh_textarea");
    be_pop(vm, 1);
}

/********************************************************************
** Solidified class: lvh_dropdown
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_class(lvh_dropdown,
    0,
    &be_class_lvh_obj,
    be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_literal("_lv_class", -1), be_const_class(be_class_lv_dropdown) },
    })),
    be_str_literal("lvh_dropdown")
);
/*******************************************************************/

void be_load_lvh_dropdown_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_lvh_dropdown);
    be_setglobal(vm, "lvh_dropdown");
    be_pop(vm, 1);
}

/********************************************************************
** Solidified function: _anonymous_
********************************************************************/
be_local_closure(openhasp__anonymous_,   /* name */
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
    /* K0   */  be_nested_str_literal("OpenHASP"),
    }),
    be_str_literal("_anonymous_"),
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
** Solidified function: get_page_cur
********************************************************************/
be_local_closure(OpenHASP_get_page_cur,   /* name */
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
    /* K0   */  be_nested_str_literal("lvh_pages"),
    /* K1   */  be_nested_str_literal("lvh_page_cur_idx"),
    }),
    be_str_literal("get_page_cur"),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040101,  //  0000  GETMBR	R1	R0	K1
      0x88080100,  //  0001  GETMBR	R2	R0	K0
      0x94040401,  //  0002  GETIDX	R1	R2	R1
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start
********************************************************************/
be_local_closure(OpenHASP_start,   /* name */
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
    /* K0   */  be_nested_str_literal("path"),
    /* K1   */  be_nested_str_literal("def_templ_name"),
    /* K2   */  be_nested_str_literal("exists"),
    /* K3   */  be_nested_str_literal("file '"),
    /* K4   */  be_nested_str_literal("' not found"),
    /* K5   */  be_nested_str_literal("io_erorr"),
    /* K6   */  be_nested_str_literal("lv"),
    /* K7   */  be_nested_str_literal("start"),
    /* K8   */  be_nested_str_literal("dark"),
    /* K9   */  be_nested_str_literal("hres"),
    /* K10  */  be_nested_str_literal("get_hor_res"),
    /* K11  */  be_nested_str_literal("vres"),
    /* K12  */  be_nested_str_literal("get_ver_res"),
    /* K13  */  be_nested_str_literal("scr"),
    /* K14  */  be_nested_str_literal("scr_act"),
    /* K15  */  be_nested_str_literal("r16"),
    /* K16  */  be_nested_str_literal("font_embedded"),
    /* K17  */  be_nested_str_literal("robotocondensed"),
    /* K18  */  be_nested_str_literal("montserrat"),
    /* K19  */  be_nested_str_literal("theme_openhasp_init"),
    /* K20  */  be_const_int(0),
    /* K21  */  be_nested_str_literal("color"),
    /* K22  */  be_const_int(16711935),
    /* K23  */  be_const_int(3158064),
    /* K24  */  be_nested_str_literal("get_disp"),
    /* K25  */  be_nested_str_literal("set_theme"),
    /* K26  */  be_nested_str_literal("set_style_bg_color"),
    /* K27  */  be_const_int(16777215),
    /* K28  */  be_nested_str_literal("theme_apply"),
    /* K29  */  be_nested_str_literal("layer_top"),
    /* K30  */  be_nested_str_literal("set_style_bg_opa"),
    /* K31  */  be_nested_str_literal("lvh_pages"),
    /* K32  */  be_nested_str_literal("_load"),
    }),
    be_str_literal("start"),
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
** Solidified function: pages_list_sorted
********************************************************************/
be_local_closure(OpenHASP_pages_list_sorted,   /* name */
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
    /* K1   */  be_nested_str_literal("lvh_page_cur_idx"),
    /* K2   */  be_nested_str_literal("lvh_pages"),
    /* K3   */  be_nested_str_literal("keys"),
    /* K4   */  be_nested_str_literal("push"),
    /* K5   */  be_nested_str_literal("stop_iteration"),
    /* K6   */  be_nested_str_literal("sort"),
    /* K7   */  be_nested_str_literal("find"),
    /* K8   */  be_const_int(1),
    }),
    be_str_literal("pages_list_sorted"),
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
** Solidified function: parse
********************************************************************/
be_local_closure(OpenHASP_parse,   /* name */
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
    /* K0   */  be_nested_str_literal("json"),
    /* K1   */  be_nested_str_literal("load"),
    /* K2   */  be_nested_str_literal("instance"),
    /* K3   */  be_nested_str_literal("parse_page"),
    /* K4   */  be_nested_str_literal("parse_obj"),
    /* K5   */  be_nested_str_literal("lvh_pages"),
    /* K6   */  be_nested_str_literal("lvh_page_cur_idx"),
    /* K7   */  be_nested_str_literal("value_error"),
    /* K8   */  be_nested_str_literal("unable to parse JSON line"),
    }),
    be_str_literal("parse"),
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
      0x881C0106,  //  000E  GETMBR	R7	R0	K6
      0x88200105,  //  000F  GETMBR	R8	R0	K5
      0x941C1007,  //  0010  GETIDX	R7	R8	R7
      0x7C100600,  //  0011  CALL	R4	3
      0x70020000,  //  0012  JMP		#0014
      0xB0060F08,  //  0013  RAISE	1	K7	K8
      0x80000000,  //  0014  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_obj
********************************************************************/
be_local_closure(OpenHASP_parse_obj,   /* name */
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
    /* K0   */  be_nested_str_literal("global"),
    /* K1   */  be_nested_str_literal("string"),
    /* K2   */  be_nested_str_literal("introspect"),
    /* K3   */  be_nested_str_literal("find"),
    /* K4   */  be_nested_str_literal("id"),
    /* K5   */  be_nested_str_literal("obj"),
    /* K6   */  be_nested_str_literal("get_page_cur"),
    /* K7   */  be_nested_str_literal("berry_run"),
    /* K8   */  be_nested_str_literal("nil"),
    /* K9   */  be_nested_str_literal("format"),
    /* K10  */  be_nested_str_literal("HSP: unable to run berry code \"%s\" - '%s' - %s"),
    /* K11  */  be_const_int(1),
    /* K12  */  be_nested_str_literal("HSP: invalid 'id': "),
    /* K13  */  be_nested_str_literal(" for 'obj':"),
    /* K14  */  be_nested_str_literal("parentid"),
    /* K15  */  be_nested_str_literal("get_obj"),
    /* K16  */  be_nested_str_literal("_lv_obj"),
    /* K17  */  be_nested_str_literal("get_scr"),
    /* K18  */  be_nested_str_literal("get"),
    /* K19  */  be_nested_str_literal("lvh_"),
    /* K20  */  be_nested_str_literal("class"),
    /* K21  */  be_nested_str_literal("lvh_obj"),
    /* K22  */  be_nested_str_literal("module"),
    /* K23  */  be_nested_str_literal("HSP: cannot find object of type "),
    /* K24  */  be_nested_str_literal("set_obj"),
    /* K25  */  be_nested_str_literal("p%ib%i"),
    /* K26  */  be_const_int(0),
    /* K27  */  be_nested_str_literal("HSP: cannot specify 'obj' for 'id':0"),
    /* K28  */  be_nested_str_literal("keys"),
    /* K29  */  be_nested_str_literal("stop_iteration"),
    }),
    be_str_literal("parse_obj"),
    &be_const_str_solidified,
    ( &(const binstruction[190]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xA4120200,  //  0001  IMPORT	R4	K1
      0xA4160400,  //  0002  IMPORT	R5	K2
      0x60180009,  //  0003  GETGBL	R6	G9
      0x8C1C0303,  //  0004  GETMET	R7	R1	K3
      0x58240004,  //  0005  LDCONST	R9	K4
      0x7C1C0400,  //  0006  CALL	R7	2
      0x7C180200,  //  0007  CALL	R6	1
      0x601C0008,  //  0008  GETGBL	R7	G8
      0x8C200303,  //  0009  GETMET	R8	R1	K3
      0x58280005,  //  000A  LDCONST	R10	K5
      0x7C200400,  //  000B  CALL	R8	2
      0x7C1C0200,  //  000C  CALL	R7	1
      0x4C200000,  //  000D  LDNIL	R8
      0x8C240106,  //  000E  GETMET	R9	R0	K6
      0x7C240200,  //  000F  CALL	R9	1
      0x60280008,  //  0010  GETGBL	R10	G8
      0x8C2C0303,  //  0011  GETMET	R11	R1	K3
      0x58340007,  //  0012  LDCONST	R13	K7
      0x7C2C0400,  //  0013  CALL	R11	2
      0x7C280200,  //  0014  CALL	R10	1
      0x202C1508,  //  0015  NE	R11	R10	K8
      0x782E0013,  //  0016  JMPF	R11	#002B
      0xA8020006,  //  0017  EXBLK	0	#001F
      0x602C000D,  //  0018  GETGBL	R11	G13
      0x5C301400,  //  0019  MOVE	R12	R10
      0x7C2C0200,  //  001A  CALL	R11	1
      0x5C301600,  //  001B  MOVE	R12	R11
      0x7C300000,  //  001C  CALL	R12	0
      0xA8040001,  //  001D  EXBLK	1	1
      0x7002000B,  //  001E  JMP		#002B
      0xAC2C0002,  //  001F  CATCH	R11	0	2
      0x70020008,  //  0020  JMP		#002A
      0x60340001,  //  0021  GETGBL	R13	G1
      0x8C380909,  //  0022  GETMET	R14	R4	K9
      0x5840000A,  //  0023  LDCONST	R16	K10
      0x5C441400,  //  0024  MOVE	R17	R10
      0x5C481600,  //  0025  MOVE	R18	R11
      0x5C4C1800,  //  0026  MOVE	R19	R12
      0x7C380A00,  //  0027  CALL	R14	5
      0x7C340200,  //  0028  CALL	R13	1
      0x70020000,  //  0029  JMP		#002B
      0xB0080000,  //  002A  RAISE	2	R0	R0
      0x4C2C0000,  //  002B  LDNIL	R11
      0x1C2C0C0B,  //  002C  EQ	R11	R6	R11
      0x782E0000,  //  002D  JMPF	R11	#002F
      0x80001600,  //  002E  RET	0
      0x202C0F08,  //  002F  NE	R11	R7	K8
      0x782E006E,  //  0030  JMPF	R11	#00A0
      0x4C2C0000,  //  0031  LDNIL	R11
      0x202C0C0B,  //  0032  NE	R11	R6	R11
      0x782E006B,  //  0033  JMPF	R11	#00A0
      0x142C0D0B,  //  0034  LT	R11	R6	K11
      0x742E0002,  //  0035  JMPT	R11	#0039
      0x542E00FD,  //  0036  LDINT	R11	254
      0x242C0C0B,  //  0037  GT	R11	R6	R11
      0x782E0008,  //  0038  JMPF	R11	#0042
      0x602C0001,  //  0039  GETGBL	R11	G1
      0x60300008,  //  003A  GETGBL	R12	G8
      0x5C340C00,  //  003B  MOVE	R13	R6
      0x7C300200,  //  003C  CALL	R12	1
      0x0032180C,  //  003D  ADD	R12	K12	R12
      0x0030190D,  //  003E  ADD	R12	R12	K13
      0x00301807,  //  003F  ADD	R12	R12	R7
      0x7C2C0200,  //  0040  CALL	R11	1
      0x80001600,  //  0041  RET	0
      0x4C2C0000,  //  0042  LDNIL	R11
      0x60300009,  //  0043  GETGBL	R12	G9
      0x8C340303,  //  0044  GETMET	R13	R1	K3
      0x583C000E,  //  0045  LDCONST	R15	K14
      0x7C340400,  //  0046  CALL	R13	2
      0x7C300200,  //  0047  CALL	R12	1
      0x4C340000,  //  0048  LDNIL	R13
      0x2034180D,  //  0049  NE	R13	R12	R13
      0x78360006,  //  004A  JMPF	R13	#0052
      0x8C34130F,  //  004B  GETMET	R13	R9	K15
      0x5C3C1800,  //  004C  MOVE	R15	R12
      0x7C340400,  //  004D  CALL	R13	2
      0x4C380000,  //  004E  LDNIL	R14
      0x20381A0E,  //  004F  NE	R14	R13	R14
      0x783A0000,  //  0050  JMPF	R14	#0052
      0x882C1B10,  //  0051  GETMBR	R11	R13	K16
      0x4C340000,  //  0052  LDNIL	R13
      0x1C34160D,  //  0053  EQ	R13	R11	R13
      0x78360002,  //  0054  JMPF	R13	#0058
      0x8C341311,  //  0055  GETMET	R13	R9	K17
      0x7C340200,  //  0056  CALL	R13	1
      0x5C2C1A00,  //  0057  MOVE	R11	R13
      0x8C340B12,  //  0058  GETMET	R13	R5	K18
      0x5C3C0000,  //  0059  MOVE	R15	R0
      0x00422607,  //  005A  ADD	R16	K19	R7
      0x7C340600,  //  005B  CALL	R13	3
      0x4C380000,  //  005C  LDNIL	R14
      0x4C3C0000,  //  005D  LDNIL	R15
      0x1C3C1A0F,  //  005E  EQ	R15	R13	R15
      0x783E0010,  //  005F  JMPF	R15	#0071
      0x8C3C0B12,  //  0060  GETMET	R15	R5	K18
      0x5C440600,  //  0061  MOVE	R17	R3
      0x5C480E00,  //  0062  MOVE	R18	R7
      0x7C3C0600,  //  0063  CALL	R15	3
      0x4C400000,  //  0064  LDNIL	R16
      0x20401E10,  //  0065  NE	R16	R15	R16
      0x78420009,  //  0066  JMPF	R16	#0071
      0x60400004,  //  0067  GETGBL	R16	G4
      0x5C441E00,  //  0068  MOVE	R17	R15
      0x7C400200,  //  0069  CALL	R16	1
      0x1C402114,  //  006A  EQ	R16	R16	K20
      0x78420004,  //  006B  JMPF	R16	#0071
      0x5C401E00,  //  006C  MOVE	R16	R15
      0x5C441600,  //  006D  MOVE	R17	R11
      0x7C400200,  //  006E  CALL	R16	1
      0x5C382000,  //  006F  MOVE	R14	R16
      0x88340115,  //  0070  GETMBR	R13	R0	K21
      0x4C3C0000,  //  0071  LDNIL	R15
      0x1C3C1A0F,  //  0072  EQ	R15	R13	R15
      0x783E000F,  //  0073  JMPF	R15	#0084
      0x8C3C0B16,  //  0074  GETMET	R15	R5	K22
      0x5C440E00,  //  0075  MOVE	R17	R7
      0x7C3C0400,  //  0076  CALL	R15	2
      0x4C400000,  //  0077  LDNIL	R16
      0x20401E10,  //  0078  NE	R16	R15	R16
      0x78420009,  //  0079  JMPF	R16	#0084
      0x60400004,  //  007A  GETGBL	R16	G4
      0x5C441E00,  //  007B  MOVE	R17	R15
      0x7C400200,  //  007C  CALL	R16	1
      0x1C402114,  //  007D  EQ	R16	R16	K20
      0x78420004,  //  007E  JMPF	R16	#0084
      0x5C401E00,  //  007F  MOVE	R16	R15
      0x5C441600,  //  0080  MOVE	R17	R11
      0x7C400200,  //  0081  CALL	R16	1
      0x5C382000,  //  0082  MOVE	R14	R16
      0x88340115,  //  0083  GETMBR	R13	R0	K21
      0x4C3C0000,  //  0084  LDNIL	R15
      0x1C3C1A0F,  //  0085  EQ	R15	R13	R15
      0x783E0006,  //  0086  JMPF	R15	#008E
      0x603C0001,  //  0087  GETGBL	R15	G1
      0x60400008,  //  0088  GETGBL	R16	G8
      0x5C440E00,  //  0089  MOVE	R17	R7
      0x7C400200,  //  008A  CALL	R16	1
      0x00422E10,  //  008B  ADD	R16	K23	R16
      0x7C3C0200,  //  008C  CALL	R15	1
      0x80001E00,  //  008D  RET	0
      0x5C3C1A00,  //  008E  MOVE	R15	R13
      0x5C401600,  //  008F  MOVE	R16	R11
      0x5C440400,  //  0090  MOVE	R17	R2
      0x5C480200,  //  0091  MOVE	R18	R1
      0x5C4C1C00,  //  0092  MOVE	R19	R14
      0x7C3C0800,  //  0093  CALL	R15	4
      0x5C201E00,  //  0094  MOVE	R8	R15
      0x8C3C1318,  //  0095  GETMET	R15	R9	K24
      0x5C440C00,  //  0096  MOVE	R17	R6
      0x5C481000,  //  0097  MOVE	R18	R8
      0x7C3C0600,  //  0098  CALL	R15	3
      0x8C3C0909,  //  0099  GETMET	R15	R4	K9
      0x58440019,  //  009A  LDCONST	R17	K25
      0x8C481304,  //  009B  GETMET	R18	R9	K4
      0x7C480200,  //  009C  CALL	R18	1
      0x5C4C0C00,  //  009D  MOVE	R19	R6
      0x7C3C0800,  //  009E  CALL	R15	4
      0x900C1E08,  //  009F  SETMBR	R3	R15	R8
      0x1C2C0D1A,  //  00A0  EQ	R11	R6	K26
      0x782E0005,  //  00A1  JMPF	R11	#00A8
      0x202C0F08,  //  00A2  NE	R11	R7	K8
      0x782E0003,  //  00A3  JMPF	R11	#00A8
      0x602C0001,  //  00A4  GETGBL	R11	G1
      0x5830001B,  //  00A5  LDCONST	R12	K27
      0x7C2C0200,  //  00A6  CALL	R11	1
      0x80001600,  //  00A7  RET	0
      0x1C2C0D1A,  //  00A8  EQ	R11	R6	K26
      0x782E0005,  //  00A9  JMPF	R11	#00B0
      0x8C2C0106,  //  00AA  GETMET	R11	R0	K6
      0x7C2C0200,  //  00AB  CALL	R11	1
      0x8C2C170F,  //  00AC  GETMET	R11	R11	K15
      0x5834001A,  //  00AD  LDCONST	R13	K26
      0x7C2C0400,  //  00AE  CALL	R11	2
      0x5C201600,  //  00AF  MOVE	R8	R11
      0x602C0010,  //  00B0  GETGBL	R11	G16
      0x8C30031C,  //  00B1  GETMET	R12	R1	K28
      0x7C300200,  //  00B2  CALL	R12	1
      0x7C2C0200,  //  00B3  CALL	R11	1
      0xA8020004,  //  00B4  EXBLK	0	#00BA
      0x5C301600,  //  00B5  MOVE	R12	R11
      0x7C300000,  //  00B6  CALL	R12	0
      0x9434020C,  //  00B7  GETIDX	R13	R1	R12
      0x9020180D,  //  00B8  SETMBR	R8	R12	R13
      0x7001FFFA,  //  00B9  JMP		#00B5
      0x582C001D,  //  00BA  LDCONST	R11	K29
      0xAC2C0200,  //  00BB  CATCH	R11	1	0
      0xB0080000,  //  00BC  RAISE	2	R0	R0
      0x80000000,  //  00BD  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(OpenHASP_init,   /* name */
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
    /* K0   */  be_nested_str_literal("re"),
    /* K1   */  be_nested_str_literal("re_page_target"),
    /* K2   */  be_nested_str_literal("compile"),
    /* K3   */  be_nested_str_literal("p\\d+"),
    }),
    be_str_literal("init"),
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
** Solidified function: deinit
********************************************************************/
be_local_closure(OpenHASP_deinit,   /* name */
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
    /* K0   */  be_nested_str_literal("_val_rule"),
    /* K1   */  be_nested_str_literal("tasmota"),
    /* K2   */  be_nested_str_literal("remove_rule"),
    /* K3   */  be_nested_str_literal("_text_rule"),
    }),
    be_str_literal("deinit"),
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
** Solidified function: do_action
********************************************************************/
be_local_closure(OpenHASP_do_action,   /* name */
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
    /* K0   */  be_nested_str_literal("lv"),
    /* K1   */  be_nested_str_literal("EVENT_CLICKED"),
    /* K2   */  be_nested_str_literal("_action"),
    /* K3   */  be_nested_str_literal("lvh_pages"),
    /* K4   */  be_nested_str_literal("lvh_page_cur_idx"),
    /* K5   */  be_nested_str_literal("pages_list_sorted"),
    /* K6   */  be_const_int(1),
    /* K7   */  be_nested_str_literal("prev"),
    /* K8   */  be_nested_str_literal("next"),
    /* K9   */  be_nested_str_literal("back"),
    /* K10  */  be_nested_str_literal("re_page_target"),
    /* K11  */  be_nested_str_literal("match"),
    /* K12  */  be_const_int(0),
    /* K13  */  be_nested_str_literal("show"),
    }),
    be_str_literal("do_action"),
    &be_const_str_solidified,
    ( &(const binstruction[74]) {  /* code */
      0xB80E0000,  //  0000  GETNGBL	R3	K0
      0x880C0701,  //  0001  GETMBR	R3	R3	K1
      0x200C0403,  //  0002  NE	R3	R2	R3
      0x780E0000,  //  0003  JMPF	R3	#0005
      0x80000600,  //  0004  RET	0
      0x880C0302,  //  0005  GETMBR	R3	R1	K2
      0x88100104,  //  0006  GETMBR	R4	R0	K4
      0x88140103,  //  0007  GETMBR	R5	R0	K3
      0x94100A04,  //  0008  GETIDX	R4	R5	R4
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
** Solidified function: _load
********************************************************************/
be_local_closure(OpenHASP__load,   /* name */
  be_nested_proto(
    15,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[16]) {     /* constants */
    /* K0   */  be_nested_str_literal("string"),
    /* K1   */  be_nested_str_literal("json"),
    /* K2   */  be_nested_str_literal("lvh_page_cur_idx"),
    /* K3   */  be_const_int(1),
    /* K4   */  be_nested_str_literal("lvh_page"),
    /* K5   */  be_nested_str_literal("lvh_pages"),
    /* K6   */  be_nested_str_literal("r"),
    /* K7   */  be_nested_str_literal("split"),
    /* K8   */  be_nested_str_literal("read"),
    /* K9   */  be_nested_str_literal("\n"),
    /* K10  */  be_nested_str_literal("close"),
    /* K11  */  be_nested_str_literal("load"),
    /* K12  */  be_nested_str_literal("instance"),
    /* K13  */  be_nested_str_literal("parse_page"),
    /* K14  */  be_nested_str_literal("parse_obj"),
    /* K15  */  be_nested_str_literal("stop_iteration"),
    }),
    be_str_literal("_load"),
    &be_const_str_solidified,
    ( &(const binstruction[50]) {  /* code */
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
      0x8C180507,  //  000E  GETMET	R6	R2	K7
      0x8C200B08,  //  000F  GETMET	R8	R5	K8
      0x7C200200,  //  0010  CALL	R8	1
      0x58240009,  //  0011  LDCONST	R9	K9
      0x7C180600,  //  0012  CALL	R6	3
      0x8C1C0B0A,  //  0013  GETMET	R7	R5	K10
      0x7C1C0200,  //  0014  CALL	R7	1
      0x601C0010,  //  0015  GETGBL	R7	G16
      0x5C200C00,  //  0016  MOVE	R8	R6
      0x7C1C0200,  //  0017  CALL	R7	1
      0xA8020013,  //  0018  EXBLK	0	#002D
      0x5C200E00,  //  0019  MOVE	R8	R7
      0x7C200000,  //  001A  CALL	R8	0
      0x8C24070B,  //  001B  GETMET	R9	R3	K11
      0x5C2C1000,  //  001C  MOVE	R11	R8
      0x7C240400,  //  001D  CALL	R9	2
      0x60280004,  //  001E  GETGBL	R10	G4
      0x5C2C1200,  //  001F  MOVE	R11	R9
      0x7C280200,  //  0020  CALL	R10	1
      0x1C28150C,  //  0021  EQ	R10	R10	K12
      0x782A0008,  //  0022  JMPF	R10	#002C
      0x8C28010D,  //  0023  GETMET	R10	R0	K13
      0x5C301200,  //  0024  MOVE	R12	R9
      0x7C280400,  //  0025  CALL	R10	2
      0x8C28010E,  //  0026  GETMET	R10	R0	K14
      0x5C301200,  //  0027  MOVE	R12	R9
      0x88340102,  //  0028  GETMBR	R13	R0	K2
      0x88380105,  //  0029  GETMBR	R14	R0	K5
      0x94341C0D,  //  002A  GETIDX	R13	R14	R13
      0x7C280600,  //  002B  CALL	R10	3
      0x7001FFEB,  //  002C  JMP		#0019
      0x581C000F,  //  002D  LDCONST	R7	K15
      0xAC1C0200,  //  002E  CATCH	R7	1	0
      0xB0080000,  //  002F  RAISE	2	R0	R0
      0x90020503,  //  0030  SETMBR	R0	K2	K3
      0x80000000,  //  0031  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: sort
********************************************************************/
be_local_closure(OpenHASP_sort,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_const_int(1),
    /* K1   */  be_const_int(0),
    /* K2   */  be_nested_str_literal("stop_iteration"),
    }),
    be_str_literal("sort"),
    &be_const_str_solidified,
    ( &(const binstruction[29]) {  /* code */
      0x60040010,  //  0000  GETGBL	R1	G16
      0x6008000C,  //  0001  GETGBL	R2	G12
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C080200,  //  0003  CALL	R2	1
      0x04080500,  //  0004  SUB	R2	R2	K0
      0x400A0002,  //  0005  CONNECT	R2	K0	R2
      0x7C040200,  //  0006  CALL	R1	1
      0xA8020010,  //  0007  EXBLK	0	#0019
      0x5C080200,  //  0008  MOVE	R2	R1
      0x7C080000,  //  0009  CALL	R2	0
      0x940C0002,  //  000A  GETIDX	R3	R0	R2
      0x5C100400,  //  000B  MOVE	R4	R2
      0x24140901,  //  000C  GT	R5	R4	K1
      0x78160008,  //  000D  JMPF	R5	#0017
      0x04140900,  //  000E  SUB	R5	R4	K0
      0x94140005,  //  000F  GETIDX	R5	R0	R5
      0x24140A03,  //  0010  GT	R5	R5	R3
      0x78160004,  //  0011  JMPF	R5	#0017
      0x04140900,  //  0012  SUB	R5	R4	K0
      0x94140005,  //  0013  GETIDX	R5	R0	R5
      0x98000805,  //  0014  SETIDX	R0	R4	R5
      0x04100900,  //  0015  SUB	R4	R4	K0
      0x7001FFF4,  //  0016  JMP		#000C
      0x98000803,  //  0017  SETIDX	R0	R4	R3
      0x7001FFEE,  //  0018  JMP		#0008
      0x58040002,  //  0019  LDCONST	R1	K2
      0xAC040200,  //  001A  CATCH	R1	1	0
      0xB0080000,  //  001B  RAISE	2	R0	R0
      0x80040000,  //  001C  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_page
********************************************************************/
be_local_closure(OpenHASP_parse_page,   /* name */
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
    /* K0   */  be_nested_str_literal("has"),
    /* K1   */  be_nested_str_literal("page"),
    /* K2   */  be_nested_str_literal("int"),
    /* K3   */  be_nested_str_literal("lvh_page_cur_idx"),
    /* K4   */  be_nested_str_literal("lvh_pages"),
    /* K5   */  be_nested_str_literal("contains"),
    /* K6   */  be_nested_str_literal("lvh_page"),
    /* K7   */  be_nested_str_literal("find"),
    /* K8   */  be_nested_str_literal("id"),
    /* K9   */  be_const_int(0),
    /* K10  */  be_nested_str_literal("get_page_cur"),
    /* K11  */  be_nested_str_literal("prev"),
    /* K12  */  be_nested_str_literal("next"),
    /* K13  */  be_nested_str_literal("back"),
    }),
    be_str_literal("parse_page"),
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
** Solidified function: page_dir_to
********************************************************************/
be_local_closure(OpenHASP_page_dir_to,   /* name */
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
    /* K0   */  be_nested_str_literal("pages_list_sorted"),
    /* K1   */  be_const_int(0),
    /* K2   */  be_const_int(1),
    /* K3   */  be_const_int(2),
    /* K4   */  be_nested_str_literal("find"),
    }),
    be_str_literal("page_dir_to"),
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
** Solidified class: OpenHASP
********************************************************************/
be_local_class(OpenHASP,
    8,
    NULL,
    be_nested_map(38,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_literal("re_page_target", -1), be_const_var(7) },
        { be_const_key_literal("lvh_slider", -1), be_const_class(be_class_lvh_slider) },
        { be_const_key_literal("lvh_checkbox", -1), be_const_class(be_class_lvh_checkbox) },
        { be_const_key_literal("start", -1), be_const_closure(OpenHASP_start_closure) },
        { be_const_key_literal("pages_list_sorted", 8), be_const_closure(OpenHASP_pages_list_sorted_closure) },
        { be_const_key_literal("page_dir_to", -1), be_const_closure(OpenHASP_page_dir_to_closure) },
        { be_const_key_literal("lvh_scr", -1), be_const_class(be_class_lvh_scr) },
        { be_const_key_literal("lvh_btnmatrix", -1), be_const_class(be_class_lvh_btnmatrix) },
        { be_const_key_literal("parse_page", -1), be_const_closure(OpenHASP_parse_page_closure) },
        { be_const_key_literal("dark", 7), be_const_var(0) },
        { be_const_key_literal("parse", 17), be_const_closure(OpenHASP_parse_closure) },
        { be_const_key_literal("scr", -1), be_const_var(3) },
        { be_const_key_literal("parse_obj", -1), be_const_closure(OpenHASP_parse_obj_closure) },
        { be_const_key_literal("init", -1), be_const_closure(OpenHASP_init_closure) },
        { be_const_key_literal("lvh_img", 21), be_const_class(be_class_lvh_img) },
        { be_const_key_literal("hres", -1), be_const_var(1) },
        { be_const_key_literal("deinit", -1), be_const_closure(OpenHASP_deinit_closure) },
        { be_const_key_literal("lvh_pages", -1), be_const_var(5) },
        { be_const_key_literal("r16", 34), be_const_var(4) },
        { be_const_key_literal("lvh_dropdown", -1), be_const_class(be_class_lvh_dropdown) },
        { be_const_key_literal("get_page_cur", 35), be_const_closure(OpenHASP_get_page_cur_closure) },
        { be_const_key_literal("vres", 2), be_const_var(2) },
        { be_const_key_literal("sort", -1), be_const_static_closure(OpenHASP_sort_closure) },
        { be_const_key_literal("lvh_roller", -1), be_const_class(be_class_lvh_roller) },
        { be_const_key_literal("lvh_label", 18), be_const_class(be_class_lvh_label) },
        { be_const_key_literal("lvh_spinner", -1), be_const_class(be_class_lvh_spinner) },
        { be_const_key_literal("lvh_btn", 30), be_const_class(be_class_lvh_btn) },
        { be_const_key_literal("lvh_bar", -1), be_const_class(be_class_lvh_bar) },
        { be_const_key_literal("lvh_switch", 23), be_const_class(be_class_lvh_switch) },
        { be_const_key_literal("do_action", 22), be_const_closure(OpenHASP_do_action_closure) },
        { be_const_key_literal("_load", -1), be_const_closure(OpenHASP__load_closure) },
        { be_const_key_literal("lvh_obj", -1), be_const_class(be_class_lvh_obj) },
        { be_const_key_literal("lvh_textarea", -1), be_const_class(be_class_lvh_textarea) },
        { be_const_key_literal("lvh_page", 14), be_const_class(be_class_lvh_page) },
        { be_const_key_literal("lvh_page_cur_idx", -1), be_const_var(6) },
        { be_const_key_literal("lvh_line", -1), be_const_class(be_class_lvh_line) },
        { be_const_key_literal("lvh_arc", 5), be_const_class(be_class_lvh_arc) },
        { be_const_key_literal("def_templ_name", -1), be_nested_str_literal("pages.jsonl") },
    })),
    be_str_literal("OpenHASP")
);

/********************************************************************
** Solidified module: openhasp
********************************************************************/
be_local_module(openhasp,
    "openhasp",
    be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_literal("OpenHASP", -1), be_const_class(be_class_OpenHASP) },
        { be_const_key_literal("init", 0), be_const_closure(openhasp__anonymous__closure) },
    }))
);
BE_EXPORT_VARIABLE be_define_const_native_module(openhasp);
/********************************************************************/


#endif // USE_LVGL_OPENHASP
#endif // USE_LVGL