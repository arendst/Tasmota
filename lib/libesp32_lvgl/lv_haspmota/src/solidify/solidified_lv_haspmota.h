/* Solidification of lv_haspmota.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_lvh_root;

/********************************************************************
** Solidified function: get_text
********************************************************************/
extern const bclass be_class_lvh_root;
be_local_closure(class_lvh_root_get_text,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_root, 
    0,                          /* has constants */
    NULL,                       /* no const */
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
** Solidified function: get_text_rule_formula
********************************************************************/
extern const bclass be_class_lvh_root;
be_local_closure(class_lvh_root_get_text_rule_formula,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_root, 
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
** Solidified function: set_text_rule_formula
********************************************************************/
extern const bclass be_class_lvh_root;
be_local_closure(class_lvh_root_set_text_rule_formula,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_root, 
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
** Solidified function: get_text_rule
********************************************************************/
extern const bclass be_class_lvh_root;
be_local_closure(class_lvh_root_get_text_rule,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_root, 
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
** Solidified function: set_text
********************************************************************/
extern const bclass be_class_lvh_root;
be_local_closure(class_lvh_root_set_text,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_root, 
    0,                          /* has constants */
    NULL,                       /* no const */
    be_str_weak(set_text),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: val_rule_matched
********************************************************************/
extern const bclass be_class_lvh_root;
be_local_closure(class_lvh_root_val_rule_matched,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_root, 
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
** Solidified function: parse_font
********************************************************************/
extern const bclass be_class_lvh_root;
be_local_closure(class_lvh_root_parse_font,   /* name */
  be_nested_proto(
    15,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_root, 
    1,                          /* has constants */
    ( &(const bvalue[18]) {     /* constants */
    /* K0   */  be_nested_str_weak(int),
    /* K1   */  be_nested_str_weak(lv),
    /* K2   */  be_nested_str_weak(font_embedded),
    /* K3   */  be_nested_str_weak(robotocondensed),
    /* K4   */  be_nested_str_weak(montserrat),
    /* K5   */  be_nested_str_weak(HSP_X3A_X20Unsupported_X20font_X3A),
    /* K6   */  be_nested_str_weak(string),
    /* K7   */  be_nested_str_weak(split),
    /* K8   */  be_nested_str_weak(_X3A),
    /* K9   */  be_const_int(1),
    /* K10  */  be_nested_str_weak(_X2D),
    /* K11  */  be_const_int(0),
    /* K12  */  be_const_int(2),
    /* K13  */  be_nested_str_weak(concat),
    /* K14  */  be_nested_str_weak(endswith),
    /* K15  */  be_nested_str_weak(_X2Ettf),
    /* K16  */  be_nested_str_weak(load_freetype_font),
    /* K17  */  be_nested_str_weak(load_font),
    }),
    be_str_weak(parse_font),
    &be_const_str_solidified,
    ( &(const binstruction[148]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x600C0004,  //  0001  GETGBL	R3	G4
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C0C0200,  //  0003  CALL	R3	1
      0x1C0C0700,  //  0004  EQ	R3	R3	K0
      0x780E0020,  //  0005  JMPF	R3	#0027
      0xA8020007,  //  0006  EXBLK	0	#000F
      0xB80E0200,  //  0007  GETNGBL	R3	K1
      0x8C0C0702,  //  0008  GETMET	R3	R3	K2
      0x58140003,  //  0009  LDCONST	R5	K3
      0x5C180200,  //  000A  MOVE	R6	R1
      0x7C0C0600,  //  000B  CALL	R3	3
      0x5C080600,  //  000C  MOVE	R2	R3
      0xA8040001,  //  000D  EXBLK	1	1
      0x70020016,  //  000E  JMP		#0026
      0xAC0C0000,  //  000F  CATCH	R3	0	0
      0x70020013,  //  0010  JMP		#0025
      0xA8020007,  //  0011  EXBLK	0	#001A
      0xB80E0200,  //  0012  GETNGBL	R3	K1
      0x8C0C0702,  //  0013  GETMET	R3	R3	K2
      0x58140004,  //  0014  LDCONST	R5	K4
      0x5C180200,  //  0015  MOVE	R6	R1
      0x7C0C0600,  //  0016  CALL	R3	3
      0x5C080600,  //  0017  MOVE	R2	R3
      0xA8040001,  //  0018  EXBLK	1	1
      0x70020009,  //  0019  JMP		#0024
      0xAC0C0000,  //  001A  CATCH	R3	0	0
      0x70020006,  //  001B  JMP		#0023
      0x600C0001,  //  001C  GETGBL	R3	G1
      0x58100005,  //  001D  LDCONST	R4	K5
      0x5C140200,  //  001E  MOVE	R5	R1
      0x7C0C0400,  //  001F  CALL	R3	2
      0x4C0C0000,  //  0020  LDNIL	R3
      0x80040600,  //  0021  RET	1	R3
      0x70020000,  //  0022  JMP		#0024
      0xB0080000,  //  0023  RAISE	2	R0	R0
      0x70020000,  //  0024  JMP		#0026
      0xB0080000,  //  0025  RAISE	2	R0	R0
      0x70020062,  //  0026  JMP		#008A
      0x600C0004,  //  0027  GETGBL	R3	G4
      0x5C100200,  //  0028  MOVE	R4	R1
      0x7C0C0200,  //  0029  CALL	R3	1
      0x1C0C0706,  //  002A  EQ	R3	R3	K6
      0x780E005D,  //  002B  JMPF	R3	#008A
      0xA40E0C00,  //  002C  IMPORT	R3	K6
      0x8C100707,  //  002D  GETMET	R4	R3	K7
      0x5C180200,  //  002E  MOVE	R6	R1
      0x581C0008,  //  002F  LDCONST	R7	K8
      0x58200009,  //  0030  LDCONST	R8	K9
      0x7C100800,  //  0031  CALL	R4	4
      0x8C140707,  //  0032  GETMET	R5	R3	K7
      0x5C1C0200,  //  0033  MOVE	R7	R1
      0x5820000A,  //  0034  LDCONST	R8	K10
      0x7C140600,  //  0035  CALL	R5	3
      0x5C180200,  //  0036  MOVE	R6	R1
      0x581C000B,  //  0037  LDCONST	R7	K11
      0x50200000,  //  0038  LDBOOL	R8	0	0
      0x6024000C,  //  0039  GETGBL	R9	G12
      0x5C280800,  //  003A  MOVE	R10	R4
      0x7C240200,  //  003B  CALL	R9	1
      0x24241309,  //  003C  GT	R9	R9	K9
      0x78260003,  //  003D  JMPF	R9	#0042
      0x6024000C,  //  003E  GETGBL	R9	G12
      0x9428090B,  //  003F  GETIDX	R10	R4	K11
      0x7C240200,  //  0040  CALL	R9	1
      0x74260000,  //  0041  JMPT	R9	#0043
      0x50240001,  //  0042  LDBOOL	R9	0	1
      0x50240200,  //  0043  LDBOOL	R9	1	0
      0x6028000C,  //  0044  GETGBL	R10	G12
      0x5C2C0A00,  //  0045  MOVE	R11	R5
      0x7C280200,  //  0046  CALL	R10	1
      0x2828150C,  //  0047  GE	R10	R10	K12
      0x782A000B,  //  0048  JMPF	R10	#0055
      0x60280009,  //  0049  GETGBL	R10	G9
      0x542DFFFE,  //  004A  LDINT	R11	-1
      0x942C0A0B,  //  004B  GETIDX	R11	R5	R11
      0x7C280200,  //  004C  CALL	R10	1
      0x5C1C1400,  //  004D  MOVE	R7	R10
      0x5429FFFD,  //  004E  LDINT	R10	-2
      0x402A160A,  //  004F  CONNECT	R10	K11	R10
      0x94280A0A,  //  0050  GETIDX	R10	R5	R10
      0x8C28150D,  //  0051  GETMET	R10	R10	K13
      0x5830000A,  //  0052  LDCONST	R12	K10
      0x7C280400,  //  0053  CALL	R10	2
      0x5C181400,  //  0054  MOVE	R6	R10
      0x8C28070E,  //  0055  GETMET	R10	R3	K14
      0x5C300C00,  //  0056  MOVE	R12	R6
      0x5834000F,  //  0057  LDCONST	R13	K15
      0x50380200,  //  0058  LDBOOL	R14	1	0
      0x7C280800,  //  0059  CALL	R10	4
      0x782A0006,  //  005A  JMPF	R10	#0062
      0x8C280707,  //  005B  GETMET	R10	R3	K7
      0x5C300C00,  //  005C  MOVE	R12	R6
      0x58340008,  //  005D  LDCONST	R13	K8
      0x7C280600,  //  005E  CALL	R10	3
      0x542DFFFE,  //  005F  LDINT	R11	-1
      0x9418140B,  //  0060  GETIDX	R6	R10	R11
      0x50200200,  //  0061  LDBOOL	R8	1	0
      0x78220007,  //  0062  JMPF	R8	#006B
      0xB82A0200,  //  0063  GETNGBL	R10	K1
      0x8C281510,  //  0064  GETMET	R10	R10	K16
      0x5C300C00,  //  0065  MOVE	R12	R6
      0x5C340E00,  //  0066  MOVE	R13	R7
      0x5838000B,  //  0067  LDCONST	R14	K11
      0x7C280800,  //  0068  CALL	R10	4
      0x5C081400,  //  0069  MOVE	R2	R10
      0x7002001E,  //  006A  JMP		#008A
      0x78260005,  //  006B  JMPF	R9	#0072
      0xB82A0200,  //  006C  GETNGBL	R10	K1
      0x8C281511,  //  006D  GETMET	R10	R10	K17
      0x5C300200,  //  006E  MOVE	R12	R1
      0x7C280400,  //  006F  CALL	R10	2
      0x5C081400,  //  0070  MOVE	R2	R10
      0x70020017,  //  0071  JMP		#008A
      0x24280F0B,  //  0072  GT	R10	R7	K11
      0x782A0015,  //  0073  JMPF	R10	#008A
      0x6028000C,  //  0074  GETGBL	R10	G12
      0x5C2C0C00,  //  0075  MOVE	R11	R6
      0x7C280200,  //  0076  CALL	R10	1
      0x2428150B,  //  0077  GT	R10	R10	K11
      0x782A0010,  //  0078  JMPF	R10	#008A
      0xA8020007,  //  0079  EXBLK	0	#0082
      0xB82A0200,  //  007A  GETNGBL	R10	K1
      0x8C281502,  //  007B  GETMET	R10	R10	K2
      0x5C300C00,  //  007C  MOVE	R12	R6
      0x5C340E00,  //  007D  MOVE	R13	R7
      0x7C280600,  //  007E  CALL	R10	3
      0x5C081400,  //  007F  MOVE	R2	R10
      0xA8040001,  //  0080  EXBLK	1	1
      0x70020007,  //  0081  JMP		#008A
      0xAC280000,  //  0082  CATCH	R10	0	0
      0x70020004,  //  0083  JMP		#0089
      0x60280001,  //  0084  GETGBL	R10	G1
      0x582C0005,  //  0085  LDCONST	R11	K5
      0x5C300200,  //  0086  MOVE	R12	R1
      0x7C280400,  //  0087  CALL	R10	2
      0x70020000,  //  0088  JMP		#008A
      0xB0080000,  //  0089  RAISE	2	R0	R0
      0x4C0C0000,  //  008A  LDNIL	R3
      0x200C0403,  //  008B  NE	R3	R2	R3
      0x780E0001,  //  008C  JMPF	R3	#008F
      0x80040400,  //  008D  RET	1	R2
      0x70020003,  //  008E  JMP		#0093
      0x600C0001,  //  008F  GETGBL	R3	G1
      0x58100005,  //  0090  LDCONST	R4	K5
      0x5C140200,  //  0091  MOVE	R5	R1
      0x7C0C0400,  //  0092  CALL	R3	2
      0x80000000,  //  0093  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_val_rule_formula
********************************************************************/
extern const bclass be_class_lvh_root;
be_local_closure(class_lvh_root_get_val_rule_formula,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_root, 
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
** Solidified function: post_config
********************************************************************/
extern const bclass be_class_lvh_root;
be_local_closure(class_lvh_root_post_config,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_root, 
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(_val),
    /* K1   */  be_nested_str_weak(set_val),
    }),
    be_str_weak(post_config),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x4C080000,  //  0001  LDNIL	R2
      0x20040202,  //  0002  NE	R1	R1	R2
      0x78060002,  //  0003  JMPF	R1	#0007
      0x8C040101,  //  0004  GETMET	R1	R0	K1
      0x880C0100,  //  0005  GETMBR	R3	R0	K0
      0x7C040400,  //  0006  CALL	R1	2
      0x80000000,  //  0007  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: text_rule_matched
********************************************************************/
extern const bclass be_class_lvh_root;
be_local_closure(class_lvh_root_text_rule_matched,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_root, 
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
** Solidified function: get_meta
********************************************************************/
extern const bclass be_class_lvh_root;
be_local_closure(class_lvh_root_get_meta,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_root, 
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
** Solidified function: set_value_str
********************************************************************/
extern const bclass be_class_lvh_root;
be_local_closure(class_lvh_root_set_value_str,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_root, 
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
** Solidified function: init
********************************************************************/
extern const bclass be_class_lvh_root;
be_local_closure(class_lvh_root_init,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    6,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_root, 
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(_page),
    /* K1   */  be_nested_str_weak(_parent_lvh),
    /* K2   */  be_nested_str_weak(_lv_class),
    /* K3   */  be_nested_str_weak(_lv_obj),
    /* K4   */  be_nested_str_weak(post_init),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0x90020002,  //  0000  SETMBR	R0	K0	R2
      0x90020205,  //  0001  SETMBR	R0	K1	R5
      0x4C180000,  //  0002  LDNIL	R6
      0x1C180806,  //  0003  EQ	R6	R4	R6
      0x781A0007,  //  0004  JMPF	R6	#000D
      0x88180102,  //  0005  GETMBR	R6	R0	K2
      0x781A0005,  //  0006  JMPF	R6	#000D
      0x88180102,  //  0007  GETMBR	R6	R0	K2
      0x5C1C0C00,  //  0008  MOVE	R7	R6
      0x5C200200,  //  0009  MOVE	R8	R1
      0x7C1C0200,  //  000A  CALL	R7	1
      0x90020607,  //  000B  SETMBR	R0	K3	R7
      0x70020000,  //  000C  JMP		#000E
      0x90020604,  //  000D  SETMBR	R0	K3	R4
      0x8C180104,  //  000E  GETMET	R6	R0	K4
      0x7C180200,  //  000F  CALL	R6	1
      0x80000000,  //  0010  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_delete
********************************************************************/
extern const bclass be_class_lvh_root;
be_local_closure(class_lvh_root_get_delete,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_root, 
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(delete),
    }),
    be_str_weak(get_delete),
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
** Solidified function: remove_trailing_zeroes
********************************************************************/
extern const bclass be_class_lvh_root;
be_local_closure(class_lvh_root_remove_trailing_zeroes,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    4,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_root, 
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_const_class(be_class_lvh_root),
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
** Solidified function: set_val_rule
********************************************************************/
extern const bclass be_class_lvh_root;
be_local_closure(class_lvh_root_set_val_rule,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 2]) {
      be_nested_proto(
        4,                          /* nstack */
        1,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL, 
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
    &be_class_lvh_root, 
    }),
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(remove_val_rule),
    /* K1   */  be_nested_str_weak(_val_rule),
    /* K2   */  be_nested_str_weak(tasmota),
    /* K3   */  be_nested_str_weak(add_rule),
    }),
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
** Solidified function: get_val_rule
********************************************************************/
extern const bclass be_class_lvh_root;
be_local_closure(class_lvh_root_get_val_rule,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_root, 
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
** Solidified function: digits_to_style
********************************************************************/
extern const bclass be_class_lvh_root;
be_local_closure(class_lvh_root_digits_to_style,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_root, 
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
** Solidified function: set_text_rule_format
********************************************************************/
extern const bclass be_class_lvh_root;
be_local_closure(class_lvh_root_set_text_rule_format,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_root, 
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
** Solidified function: remove_val_rule
********************************************************************/
extern const bclass be_class_lvh_root;
be_local_closure(class_lvh_root_remove_val_rule,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_root, 
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(_val_rule),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(remove_rule),
    }),
    be_str_weak(remove_val_rule),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x4C080000,  //  0001  LDNIL	R2
      0x20040202,  //  0002  NE	R1	R1	R2
      0x78060004,  //  0003  JMPF	R1	#0009
      0xB8060200,  //  0004  GETNGBL	R1	K1
      0x8C040302,  //  0005  GETMET	R1	R1	K2
      0x880C0100,  //  0006  GETMBR	R3	R0	K0
      0x5C100000,  //  0007  MOVE	R4	R0
      0x7C040600,  //  0008  CALL	R1	3
      0x80000000,  //  0009  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: delete
********************************************************************/
extern const bclass be_class_lvh_root;
be_local_closure(class_lvh_root_delete,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_root, 
    0,                          /* has constants */
    NULL,                       /* no const */
    be_str_weak(delete),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_obj
********************************************************************/
extern const bclass be_class_lvh_root;
be_local_closure(class_lvh_root_get_obj,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_root, 
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
** Solidified function: is_color_attribute
********************************************************************/
extern const bclass be_class_lvh_root;
be_local_closure(class_lvh_root_is_color_attribute,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_root, 
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(endswith),
    /* K2   */  be_nested_str_weak(color),
    }),
    be_str_weak(is_color_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x8C0C0501,  //  0001  GETMET	R3	R2	K1
      0x60140008,  //  0002  GETGBL	R5	G8
      0x5C180200,  //  0003  MOVE	R6	R1
      0x7C140200,  //  0004  CALL	R5	1
      0x58180002,  //  0005  LDCONST	R6	K2
      0x7C0C0600,  //  0006  CALL	R3	3
      0x80040600,  //  0007  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_meta
********************************************************************/
extern const bclass be_class_lvh_root;
be_local_closure(class_lvh_root_set_meta,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_root, 
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
** Solidified function: set_delete
********************************************************************/
extern const bclass be_class_lvh_root;
be_local_closure(class_lvh_root_set_delete,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_root, 
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(type_error),
    /* K1   */  be_nested_str_weak(you_X20cannot_X20assign_X20to_X20_X27delete_X27),
    }),
    be_str_weak(set_delete),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0xB0060101,  //  0000  RAISE	1	K0	K1
      0x80000000,  //  0001  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_value_str
********************************************************************/
extern const bclass be_class_lvh_root;
be_local_closure(class_lvh_root_get_value_str,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_root, 
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
** Solidified function: parse_color
********************************************************************/
extern const bclass be_class_lvh_root;
be_local_closure(class_lvh_root_parse_color,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    1,                          /* argc */
    4,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 2]) {
      be_nested_proto(
        10,                          /* nstack */
        1,                          /* argc */
        0,                          /* varg */
        0,                          /* has upvals */
        NULL,                       /* no upvals */
        0,                          /* has sup protos */
        NULL, 
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
    &be_class_lvh_root, 
    }),
    1,                          /* has constants */
    ( &(const bvalue[10]) {     /* constants */
    /* K0   */  be_const_class(be_class_lvh_root),
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
** Solidified function: get_text_rule_format
********************************************************************/
extern const bclass be_class_lvh_root;
be_local_closure(class_lvh_root_get_text_rule_format,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_root, 
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
** Solidified function: remove_text_rule
********************************************************************/
extern const bclass be_class_lvh_root;
be_local_closure(class_lvh_root_remove_text_rule,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_root, 
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(_text_rule),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(remove_rule),
    }),
    be_str_weak(remove_text_rule),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x4C080000,  //  0001  LDNIL	R2
      0x20040202,  //  0002  NE	R1	R1	R2
      0x78060004,  //  0003  JMPF	R1	#0009
      0xB8060200,  //  0004  GETNGBL	R1	K1
      0x8C040302,  //  0005  GETMET	R1	R1	K2
      0x880C0100,  //  0006  GETMBR	R3	R0	K0
      0x5C100000,  //  0007  MOVE	R4	R0
      0x7C040600,  //  0008  CALL	R1	3
      0x80000000,  //  0009  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_val_rule_formula
********************************************************************/
extern const bclass be_class_lvh_root;
be_local_closure(class_lvh_root_set_val_rule_formula,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_root, 
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
** Solidified function: set_text_rule
********************************************************************/
extern const bclass be_class_lvh_root;
be_local_closure(class_lvh_root_set_text_rule,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 2]) {
      be_nested_proto(
        4,                          /* nstack */
        1,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL, 
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
    &be_class_lvh_root, 
    }),
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(remove_text_rule),
    /* K1   */  be_nested_str_weak(_text_rule),
    /* K2   */  be_nested_str_weak(tasmota),
    /* K3   */  be_nested_str_weak(add_rule),
    }),
    be_str_weak(set_text_rule),
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
** Solidified class: lvh_root
********************************************************************/
be_local_class(lvh_root,
    13,
    NULL,
    be_nested_map(49,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(_lv_obj, 18), be_const_var(1) },
        { be_const_key_weak(get_text, -1), be_const_closure(class_lvh_root_get_text_closure) },
        { be_const_key_weak(get_text_rule_formula, -1), be_const_closure(class_lvh_root_get_text_rule_formula_closure) },
        { be_const_key_weak(_val, -1), be_const_var(5) },
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
        { be_const_key_weak(remove_text_rule, -1), be_const_closure(class_lvh_root_remove_text_rule_closure) },
        { be_const_key_weak(_page, -1), be_const_var(2) },
        { be_const_key_weak(val_rule_matched, -1), be_const_closure(class_lvh_root_val_rule_matched_closure) },
        { be_const_key_weak(get_text_rule_format, 43), be_const_closure(class_lvh_root_get_text_rule_format_closure) },
        { be_const_key_weak(parse_font, -1), be_const_closure(class_lvh_root_parse_font_closure) },
        { be_const_key_weak(_val_rule_formula, 34), be_const_var(7) },
        { be_const_key_weak(_digit2state, 3), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(6,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(32),
        be_const_int(33),
        be_const_int(128),
        be_const_int(160),
    }))    ) } )) },
        { be_const_key_weak(_text_rule, -1), be_const_var(9) },
        { be_const_key_weak(get_val_rule_formula, 4), be_const_closure(class_lvh_root_get_val_rule_formula_closure) },
        { be_const_key_weak(_meta, -1), be_const_var(4) },
        { be_const_key_weak(text_rule_matched, -1), be_const_closure(class_lvh_root_text_rule_matched_closure) },
        { be_const_key_weak(_text_rule_formula, 10), be_const_var(10) },
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
        { be_const_key_weak(_text_rule_function, 44), be_const_var(11) },
        { be_const_key_weak(init, -1), be_const_closure(class_lvh_root_init_closure) },
        { be_const_key_weak(get_text_rule, 6), be_const_closure(class_lvh_root_get_text_rule_closure) },
        { be_const_key_weak(get_value_str, -1), be_const_closure(class_lvh_root_get_value_str_closure) },
        { be_const_key_weak(set_delete, -1), be_const_closure(class_lvh_root_set_delete_closure) },
        { be_const_key_weak(id, 12), be_const_var(0) },
        { be_const_key_weak(get_val_rule, -1), be_const_closure(class_lvh_root_get_val_rule_closure) },
        { be_const_key_weak(post_config, 8), be_const_closure(class_lvh_root_post_config_closure) },
        { be_const_key_weak(_val_rule_function, -1), be_const_var(8) },
        { be_const_key_weak(_attr_ignore, 40), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(9,
    ( (struct bvalue*) &(const bvalue[]) {
        be_nested_str_weak(tostring),
        be_nested_str_weak(obj),
        be_nested_str_weak(page),
        be_nested_str_weak(comment),
        be_nested_str_weak(parentid),
        be_nested_str_weak(prev),
        be_nested_str_weak(next),
        be_nested_str_weak(back),
        be_nested_str_weak(berry_run),
    }))    ) } )) },
        { be_const_key_weak(set_text_rule_format, -1), be_const_closure(class_lvh_root_set_text_rule_format_closure) },
        { be_const_key_weak(remove_val_rule, -1), be_const_closure(class_lvh_root_remove_val_rule_closure) },
        { be_const_key_weak(_text_rule_format, -1), be_const_var(12) },
        { be_const_key_weak(set_text_rule_formula, 23), be_const_closure(class_lvh_root_set_text_rule_formula_closure) },
        { be_const_key_weak(delete, -1), be_const_closure(class_lvh_root_delete_closure) },
        { be_const_key_weak(set_text, 26), be_const_closure(class_lvh_root_set_text_closure) },
        { be_const_key_weak(get_meta, -1), be_const_closure(class_lvh_root_get_meta_closure) },
        { be_const_key_weak(is_color_attribute, -1), be_const_closure(class_lvh_root_is_color_attribute_closure) },
        { be_const_key_weak(_val_rule, -1), be_const_var(6) },
        { be_const_key_weak(set_meta, -1), be_const_closure(class_lvh_root_set_meta_closure) },
        { be_const_key_weak(set_val_rule, 22), be_const_closure(class_lvh_root_set_val_rule_closure) },
        { be_const_key_weak(set_value_str, 21), be_const_closure(class_lvh_root_set_value_str_closure) },
        { be_const_key_weak(digits_to_style, -1), be_const_closure(class_lvh_root_digits_to_style_closure) },
        { be_const_key_weak(remove_trailing_zeroes, 14), be_const_static_closure(class_lvh_root_remove_trailing_zeroes_closure) },
        { be_const_key_weak(parse_color, -1), be_const_static_closure(class_lvh_root_parse_color_closure) },
        { be_const_key_weak(_parent_lvh, -1), be_const_var(3) },
        { be_const_key_weak(get_delete, -1), be_const_closure(class_lvh_root_get_delete_closure) },
        { be_const_key_weak(_lv_class, -1), be_const_nil() },
        { be_const_key_weak(get_obj, 5), be_const_closure(class_lvh_root_get_obj_closure) },
        { be_const_key_weak(set_val_rule_formula, -1), be_const_closure(class_lvh_root_set_val_rule_formula_closure) },
        { be_const_key_weak(set_text_rule, -1), be_const_closure(class_lvh_root_set_text_rule_closure) },
    })),
    be_str_weak(lvh_root)
);

extern const bclass be_class_lvh_obj;

/********************************************************************
** Solidified function: set_pad_bottom2
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_closure(class_lvh_obj_set_pad_bottom2,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_obj, 
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
** Solidified function: set_toggle
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_closure(class_lvh_obj_set_toggle,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_obj, 
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
** Solidified function: set_pad_left2
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_closure(class_lvh_obj_set_pad_left2,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_obj, 
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
** Solidified function: get_pad_top
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_closure(class_lvh_obj_get_pad_top,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_obj, 
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
** Solidified function: get_line_width
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_closure(class_lvh_obj_get_line_width,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_obj, 
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
** Solidified function: get_adjustable
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_closure(class_lvh_obj_get_adjustable,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_obj, 
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
** Solidified function: get_text
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_closure(class_lvh_obj_get_text,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_obj, 
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
** Solidified function: set_line_width
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_closure(class_lvh_obj_set_line_width,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_obj, 
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
** Solidified function: set_text
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_closure(class_lvh_obj_set_text,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_obj, 
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
** Solidified function: set_align
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_closure(class_lvh_obj_set_align,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_obj, 
    1,                          /* has constants */
    ( &(const bvalue[13]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(check_label),
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
    ( &(const binstruction[32]) {  /* code */
      0x4C0C0000,  //  0000  LDNIL	R3
      0x1C100500,  //  0001  EQ	R4	R2	K0
      0x78120000,  //  0002  JMPF	R4	#0004
      0x58080000,  //  0003  LDCONST	R2	K0
      0x8C100101,  //  0004  GETMET	R4	R0	K1
      0x7C100200,  //  0005  CALL	R4	1
      0x1C100300,  //  0006  EQ	R4	R1	K0
      0x74120001,  //  0007  JMPT	R4	#000A
      0x1C100302,  //  0008  EQ	R4	R1	K2
      0x78120002,  //  0009  JMPF	R4	#000D
      0xB8120600,  //  000A  GETNGBL	R4	K3
      0x880C0904,  //  000B  GETMBR	R3	R4	K4
      0x7002000C,  //  000C  JMP		#001A
      0x1C100305,  //  000D  EQ	R4	R1	K5
      0x74120001,  //  000E  JMPT	R4	#0011
      0x1C100306,  //  000F  EQ	R4	R1	K6
      0x78120002,  //  0010  JMPF	R4	#0014
      0xB8120600,  //  0011  GETNGBL	R4	K3
      0x880C0907,  //  0012  GETMBR	R3	R4	K7
      0x70020005,  //  0013  JMP		#001A
      0x1C100308,  //  0014  EQ	R4	R1	K8
      0x74120001,  //  0015  JMPT	R4	#0018
      0x1C100309,  //  0016  EQ	R4	R1	K9
      0x78120001,  //  0017  JMPF	R4	#001A
      0xB8120600,  //  0018  GETNGBL	R4	K3
      0x880C090A,  //  0019  GETMBR	R3	R4	K10
      0x8810010B,  //  001A  GETMBR	R4	R0	K11
      0x8C10090C,  //  001B  GETMET	R4	R4	K12
      0x5C180600,  //  001C  MOVE	R6	R3
      0x5C1C0400,  //  001D  MOVE	R7	R2
      0x7C100600,  //  001E  CALL	R4	3
      0x80000000,  //  001F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_value_color
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_closure(class_lvh_obj_get_value_color,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_obj, 
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
** Solidified function: get_pad_right
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_closure(class_lvh_obj_get_pad_right,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_obj, 
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
** Solidified function: get_text_color
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_closure(class_lvh_obj_get_text_color,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_obj, 
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
** Solidified function: get_action
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_closure(class_lvh_obj_get_action,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_obj, 
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
** Solidified function: get_value_ofs_y
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_closure(class_lvh_obj_get_value_ofs_y,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_obj, 
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
** Solidified function: set_pad_all2
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_closure(class_lvh_obj_set_pad_all2,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_obj, 
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
** Solidified function: check_label
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_closure(class_lvh_obj_check_label,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_obj, 
    1,                          /* has constants */
    ( &(const bvalue[10]) {     /* constants */
    /* K0   */  be_nested_str_weak(_lv_label),
    /* K1   */  be_nested_str_weak(introspect),
    /* K2   */  be_nested_str_weak(contains),
    /* K3   */  be_nested_str_weak(_lv_obj),
    /* K4   */  be_nested_str_weak(set_text),
    /* K5   */  be_nested_str_weak(lv),
    /* K6   */  be_nested_str_weak(label),
    /* K7   */  be_nested_str_weak(get_obj),
    /* K8   */  be_nested_str_weak(set_align),
    /* K9   */  be_nested_str_weak(ALIGN_CENTER),
    }),
    be_str_weak(check_label),
    &be_const_str_solidified,
    ( &(const binstruction[25]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x4C080000,  //  0001  LDNIL	R2
      0x1C040202,  //  0002  EQ	R1	R1	R2
      0x78060013,  //  0003  JMPF	R1	#0018
      0xA4060200,  //  0004  IMPORT	R1	K1
      0x8C080302,  //  0005  GETMET	R2	R1	K2
      0x88100103,  //  0006  GETMBR	R4	R0	K3
      0x58140004,  //  0007  LDCONST	R5	K4
      0x7C080600,  //  0008  CALL	R2	3
      0x780A0002,  //  0009  JMPF	R2	#000D
      0x88080103,  //  000A  GETMBR	R2	R0	K3
      0x90020002,  //  000B  SETMBR	R0	K0	R2
      0x7002000A,  //  000C  JMP		#0018
      0xB80A0A00,  //  000D  GETNGBL	R2	K5
      0x8C080506,  //  000E  GETMET	R2	R2	K6
      0x8C100107,  //  000F  GETMET	R4	R0	K7
      0x7C100200,  //  0010  CALL	R4	1
      0x7C080400,  //  0011  CALL	R2	2
      0x90020002,  //  0012  SETMBR	R0	K0	R2
      0x88080100,  //  0013  GETMBR	R2	R0	K0
      0x8C080508,  //  0014  GETMET	R2	R2	K8
      0xB8120A00,  //  0015  GETNGBL	R4	K5
      0x88100909,  //  0016  GETMBR	R4	R4	K9
      0x7C080400,  //  0017  CALL	R2	2
      0x80000000,  //  0018  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_value_color
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_closure(class_lvh_obj_set_value_color,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_obj, 
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
** Solidified function: get_flex_in_new_track
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_closure(class_lvh_obj_get_flex_in_new_track,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_obj, 
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(_lv_obj),
    /* K1   */  be_nested_str_weak(has_flag),
    /* K2   */  be_nested_str_weak(lv),
    /* K3   */  be_nested_str_weak(OBJ_FLAG_FLEX_IN_NEW_TRACK),
    }),
    be_str_weak(get_flex_in_new_track),
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
** Solidified function: delete
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_closure(class_lvh_obj_delete,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_obj, 
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(remove_val_rule),
    /* K1   */  be_nested_str_weak(remove_text_rule),
    /* K2   */  be_nested_str_weak(_lv_label),
    /* K3   */  be_nested_str_weak(del),
    /* K4   */  be_nested_str_weak(_lv_obj),
    /* K5   */  be_nested_str_weak(_page),
    /* K6   */  be_nested_str_weak(remove_obj),
    /* K7   */  be_nested_str_weak(id),
    }),
    be_str_weak(delete),
    &be_const_str_solidified,
    ( &(const binstruction[23]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x8C040101,  //  0002  GETMET	R1	R0	K1
      0x7C040200,  //  0003  CALL	R1	1
      0x88040102,  //  0004  GETMBR	R1	R0	K2
      0x78060004,  //  0005  JMPF	R1	#000B
      0x88040102,  //  0006  GETMBR	R1	R0	K2
      0x8C040303,  //  0007  GETMET	R1	R1	K3
      0x7C040200,  //  0008  CALL	R1	1
      0x4C040000,  //  0009  LDNIL	R1
      0x90020401,  //  000A  SETMBR	R0	K2	R1
      0x88040104,  //  000B  GETMBR	R1	R0	K4
      0x78060004,  //  000C  JMPF	R1	#0012
      0x88040104,  //  000D  GETMBR	R1	R0	K4
      0x8C040303,  //  000E  GETMET	R1	R1	K3
      0x7C040200,  //  000F  CALL	R1	1
      0x4C040000,  //  0010  LDNIL	R1
      0x90020801,  //  0011  SETMBR	R0	K4	R1
      0x88040105,  //  0012  GETMBR	R1	R0	K5
      0x8C040306,  //  0013  GETMET	R1	R1	K6
      0x880C0107,  //  0014  GETMBR	R3	R0	K7
      0x7C040400,  //  0015  CALL	R1	2
      0x80000000,  //  0016  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_value_ofs_x
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_closure(class_lvh_obj_get_value_ofs_x,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_obj, 
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
** Solidified function: set_pad_top2
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_closure(class_lvh_obj_set_pad_top2,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_obj, 
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
** Solidified function: set_click
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_closure(class_lvh_obj_set_click,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_obj, 
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
** Solidified function: set_adjustable
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_closure(class_lvh_obj_set_adjustable,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_obj, 
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
** Solidified function: event_cb
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_closure(class_lvh_obj_event_cb,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 3]) {
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
        NULL, 
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
        NULL, 
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
    &be_class_lvh_obj, 
    }),
    1,                          /* has constants */
    ( &(const bvalue[22]) {     /* constants */
    /* K0   */  be_nested_str_weak(_page),
    /* K1   */  be_nested_str_weak(_oh),
    /* K2   */  be_nested_str_weak(get_code),
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
      0x8C0C0302,  //  0002  GETMET	R3	R1	K2
      0x7C0C0200,  //  0003  CALL	R3	1
      0x88100103,  //  0004  GETMBR	R4	R0	K3
      0x20100904,  //  0005  NE	R4	R4	K4
      0x78120008,  //  0006  JMPF	R4	#0010
      0xB8120A00,  //  0007  GETNGBL	R4	K5
      0x88100906,  //  0008  GETMBR	R4	R4	K6
      0x1C100604,  //  0009  EQ	R4	R3	R4
      0x78120004,  //  000A  JMPF	R4	#0010
      0xB8120E00,  //  000B  GETNGBL	R4	K7
      0x8C100908,  //  000C  GETMET	R4	R4	K8
      0x58180009,  //  000D  LDCONST	R6	K9
      0x841C0000,  //  000E  CLOSURE	R7	P0
      0x7C100600,  //  000F  CALL	R4	3
      0x8810010A,  //  0010  GETMBR	R4	R0	K10
      0x8C10090B,  //  0011  GETMET	R4	R4	K11
      0x5C180600,  //  0012  MOVE	R6	R3
      0x7C100400,  //  0013  CALL	R4	2
      0x4C140000,  //  0014  LDNIL	R5
      0x20140805,  //  0015  NE	R5	R4	R5
      0x7816002E,  //  0016  JMPF	R5	#0046
      0xA4161800,  //  0017  IMPORT	R5	K12
      0x58180004,  //  0018  LDCONST	R6	K4
      0xB81E0A00,  //  0019  GETNGBL	R7	K5
      0x881C0F0D,  //  001A  GETMBR	R7	R7	K13
      0x1C1C0607,  //  001B  EQ	R7	R3	R7
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
** Solidified function: init
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_closure(class_lvh_obj_init,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    6,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_obj, 
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(init),
    }),
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
** Solidified function: get_enabled
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_closure(class_lvh_obj_get_enabled,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_obj, 
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
** Solidified function: get_click
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_closure(class_lvh_obj_get_click,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_obj, 
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
** Solidified function: get_label_mode
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_closure(class_lvh_obj_get_label_mode,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_obj, 
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(_lv_label),
    /* K1   */  be_nested_str_weak(get_long_mode),
    }),
    be_str_weak(get_label_mode),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x4C080000,  //  0001  LDNIL	R2
      0x20040202,  //  0002  NE	R1	R1	R2
      0x78060003,  //  0003  JMPF	R1	#0008
      0x88040100,  //  0004  GETMBR	R1	R0	K0
      0x8C040301,  //  0005  GETMET	R1	R1	K1
      0x7C040200,  //  0006  CALL	R1	1
      0x80040200,  //  0007  RET	1	R1
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_toggle
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_closure(class_lvh_obj_get_toggle,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_obj, 
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
** Solidified function: get_hidden
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_closure(class_lvh_obj_get_hidden,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_obj, 
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
** Solidified function: set_value_font
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_closure(class_lvh_obj_set_value_font,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_obj, 
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
** Solidified function: get_value_font
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_closure(class_lvh_obj_get_value_font,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_obj, 
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
** Solidified function: set_val
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_closure(class_lvh_obj_set_val,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_obj, 
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(introspect),
    /* K1   */  be_nested_str_weak(_val),
    /* K2   */  be_nested_str_weak(contains),
    /* K3   */  be_nested_str_weak(_lv_obj),
    /* K4   */  be_nested_str_weak(set_value),
    }),
    be_str_weak(set_val),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x90020201,  //  0001  SETMBR	R0	K1	R1
      0x8C0C0502,  //  0002  GETMET	R3	R2	K2
      0x88140103,  //  0003  GETMBR	R5	R0	K3
      0x58180004,  //  0004  LDCONST	R6	K4
      0x7C0C0600,  //  0005  CALL	R3	3
      0x780E0003,  //  0006  JMPF	R3	#000B
      0x880C0103,  //  0007  GETMBR	R3	R0	K3
      0x8C0C0704,  //  0008  GETMET	R3	R3	K4
      0x5C140200,  //  0009  MOVE	R5	R1
      0x7C0C0400,  //  000A  CALL	R3	2
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_text_color
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_closure(class_lvh_obj_set_text_color,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_obj, 
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
** Solidified function: set_action
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_closure(class_lvh_obj_set_action,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_obj, 
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
** Solidified function: get_radius2
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_closure(class_lvh_obj_get_radius2,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_obj, 
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
** Solidified function: set_pad_right2
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_closure(class_lvh_obj_set_pad_right2,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_obj, 
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
** Solidified function: set_label_mode
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_closure(class_lvh_obj_set_label_mode,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_obj, 
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
    be_str_weak(set_label_mode),
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
** Solidified function: setmember
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_closure(class_lvh_obj_setmember,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_obj, 
    1,                          /* has constants */
    ( &(const bvalue[24]) {     /* constants */
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
    /* K10  */  be_const_int(3),
    /* K11  */  be_nested_str_weak(byte),
    /* K12  */  be_const_int(2147483647),
    /* K13  */  be_nested_str_weak(digits_to_style),
    /* K14  */  be_nested_str_weak(_attr_ignore),
    /* K15  */  be_nested_str_weak(find),
    /* K16  */  be_nested_str_weak(get),
    /* K17  */  be_nested_str_weak(function),
    /* K18  */  be_nested_str_weak(_attr_map),
    /* K19  */  be_nested_str_weak(is_color_attribute),
    /* K20  */  be_nested_str_weak(parse_color),
    /* K21  */  be_nested_str_weak(_lv_obj),
    /* K22  */  be_nested_str_weak(set_style_),
    /* K23  */  be_nested_str_weak(HSP_X3A_X20unknown_X20attribute_X3A),
    }),
    be_str_weak(setmember),
    &be_const_str_solidified,
    ( &(const binstruction[152]) {  /* code */
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
      0x4C140000,  //  001C  LDNIL	R5
      0x6018000C,  //  001D  GETGBL	R6	G12
      0x5C1C0200,  //  001E  MOVE	R7	R1
      0x7C180200,  //  001F  CALL	R6	1
      0x28180D0A,  //  0020  GE	R6	R6	K10
      0x781A001F,  //  0021  JMPF	R6	#0042
      0x8C18070B,  //  0022  GETMET	R6	R3	K11
      0x5421FFFE,  //  0023  LDINT	R8	-1
      0x94200208,  //  0024  GETIDX	R8	R1	R8
      0x7C180400,  //  0025  CALL	R6	2
      0x8C1C070B,  //  0026  GETMET	R7	R3	K11
      0x5425FFFD,  //  0027  LDINT	R9	-2
      0x94240209,  //  0028  GETIDX	R9	R1	R9
      0x7C1C0400,  //  0029  CALL	R7	2
      0x5422002F,  //  002A  LDINT	R8	48
      0x28200C08,  //  002B  GE	R8	R6	R8
      0x78220014,  //  002C  JMPF	R8	#0042
      0x54220038,  //  002D  LDINT	R8	57
      0x18200C08,  //  002E  LE	R8	R6	R8
      0x78220011,  //  002F  JMPF	R8	#0042
      0x5422002F,  //  0030  LDINT	R8	48
      0x28200E08,  //  0031  GE	R8	R7	R8
      0x7822000E,  //  0032  JMPF	R8	#0042
      0x54220038,  //  0033  LDINT	R8	57
      0x18200E08,  //  0034  LE	R8	R7	R8
      0x7822000B,  //  0035  JMPF	R8	#0042
      0x60200009,  //  0036  GETGBL	R8	G9
      0x5425FFFD,  //  0037  LDINT	R9	-2
      0x4024130C,  //  0038  CONNECT	R9	R9	K12
      0x94240209,  //  0039  GETIDX	R9	R1	R9
      0x7C200200,  //  003A  CALL	R8	1
      0x5425FFFC,  //  003B  LDINT	R9	-3
      0x40260E09,  //  003C  CONNECT	R9	K7	R9
      0x94040209,  //  003D  GETIDX	R1	R1	R9
      0x8C24010D,  //  003E  GETMET	R9	R0	K13
      0x5C2C1000,  //  003F  MOVE	R11	R8
      0x7C240400,  //  0040  CALL	R9	2
      0x5C141200,  //  0041  MOVE	R5	R9
      0x8818010E,  //  0042  GETMBR	R6	R0	K14
      0x8C180D0F,  //  0043  GETMET	R6	R6	K15
      0x5C200200,  //  0044  MOVE	R8	R1
      0x7C180400,  //  0045  CALL	R6	2
      0x4C1C0000,  //  0046  LDNIL	R7
      0x20180C07,  //  0047  NE	R6	R6	R7
      0x781A0000,  //  0048  JMPF	R6	#004A
      0x80000C00,  //  0049  RET	0
      0x8C180910,  //  004A  GETMET	R6	R4	K16
      0x5C200000,  //  004B  MOVE	R8	R0
      0x00260601,  //  004C  ADD	R9	K3	R1
      0x7C180600,  //  004D  CALL	R6	3
      0x601C0004,  //  004E  GETGBL	R7	G4
      0x5C200C00,  //  004F  MOVE	R8	R6
      0x7C1C0200,  //  0050  CALL	R7	1
      0x1C1C0F11,  //  0051  EQ	R7	R7	K17
      0x781E000A,  //  0052  JMPF	R7	#005E
      0x5C1C0C00,  //  0053  MOVE	R7	R6
      0x5C200000,  //  0054  MOVE	R8	R0
      0x5C240400,  //  0055  MOVE	R9	R2
      0x4C280000,  //  0056  LDNIL	R10
      0x20280A0A,  //  0057  NE	R10	R5	R10
      0x782A0001,  //  0058  JMPF	R10	#005B
      0x5C280A00,  //  0059  MOVE	R10	R5
      0x70020000,  //  005A  JMP		#005C
      0x58280007,  //  005B  LDCONST	R10	K7
      0x7C1C0600,  //  005C  CALL	R7	3
      0x80000E00,  //  005D  RET	0
      0x881C0112,  //  005E  GETMBR	R7	R0	K18
      0x8C1C0F0F,  //  005F  GETMET	R7	R7	K15
      0x5C240200,  //  0060  MOVE	R9	R1
      0x5C280200,  //  0061  MOVE	R10	R1
      0x7C1C0600,  //  0062  CALL	R7	3
      0x5C040E00,  //  0063  MOVE	R1	R7
      0x8C1C0113,  //  0064  GETMET	R7	R0	K19
      0x5C240200,  //  0065  MOVE	R9	R1
      0x7C1C0400,  //  0066  CALL	R7	2
      0x781E0003,  //  0067  JMPF	R7	#006C
      0x8C1C0114,  //  0068  GETMET	R7	R0	K20
      0x5C240400,  //  0069  MOVE	R9	R2
      0x7C1C0400,  //  006A  CALL	R7	2
      0x5C080E00,  //  006B  MOVE	R2	R7
      0x4C1C0000,  //  006C  LDNIL	R7
      0x1C1C0A07,  //  006D  EQ	R7	R5	R7
      0x781E000E,  //  006E  JMPF	R7	#007E
      0x8C1C0910,  //  006F  GETMET	R7	R4	K16
      0x88240115,  //  0070  GETMBR	R9	R0	K21
      0x002A0601,  //  0071  ADD	R10	K3	R1
      0x7C1C0600,  //  0072  CALL	R7	3
      0x5C180E00,  //  0073  MOVE	R6	R7
      0x601C0004,  //  0074  GETGBL	R7	G4
      0x5C200C00,  //  0075  MOVE	R8	R6
      0x7C1C0200,  //  0076  CALL	R7	1
      0x1C1C0F11,  //  0077  EQ	R7	R7	K17
      0x781E0004,  //  0078  JMPF	R7	#007E
      0x5C1C0C00,  //  0079  MOVE	R7	R6
      0x88200115,  //  007A  GETMBR	R8	R0	K21
      0x5C240400,  //  007B  MOVE	R9	R2
      0x7C1C0400,  //  007C  CALL	R7	2
      0x80040E00,  //  007D  RET	1	R7
      0x8C1C0910,  //  007E  GETMET	R7	R4	K16
      0x88240115,  //  007F  GETMBR	R9	R0	K21
      0x002A2C01,  //  0080  ADD	R10	K22	R1
      0x7C1C0600,  //  0081  CALL	R7	3
      0x5C180E00,  //  0082  MOVE	R6	R7
      0x601C0004,  //  0083  GETGBL	R7	G4
      0x5C200C00,  //  0084  MOVE	R8	R6
      0x7C1C0200,  //  0085  CALL	R7	1
      0x1C1C0F11,  //  0086  EQ	R7	R7	K17
      0x781E000A,  //  0087  JMPF	R7	#0093
      0x5C1C0C00,  //  0088  MOVE	R7	R6
      0x88200115,  //  0089  GETMBR	R8	R0	K21
      0x5C240400,  //  008A  MOVE	R9	R2
      0x4C280000,  //  008B  LDNIL	R10
      0x20280A0A,  //  008C  NE	R10	R5	R10
      0x782A0001,  //  008D  JMPF	R10	#0090
      0x5C280A00,  //  008E  MOVE	R10	R5
      0x70020000,  //  008F  JMP		#0091
      0x58280007,  //  0090  LDCONST	R10	K7
      0x7C1C0600,  //  0091  CALL	R7	3
      0x80040E00,  //  0092  RET	1	R7
      0x601C0001,  //  0093  GETGBL	R7	G1
      0x58200017,  //  0094  LDCONST	R8	K23
      0x5C240200,  //  0095  MOVE	R9	R1
      0x7C1C0400,  //  0096  CALL	R7	2
      0x80000000,  //  0097  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_enabled
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_closure(class_lvh_obj_set_enabled,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_obj, 
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
** Solidified function: get_align
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_closure(class_lvh_obj_get_align,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_obj, 
    1,                          /* has constants */
    ( &(const bvalue[10]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(_lv_label),
    /* K2   */  be_nested_str_weak(get_style_text_align),
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
    ( &(const binstruction[33]) {  /* code */
      0x1C080300,  //  0000  EQ	R2	R1	K0
      0x780A0000,  //  0001  JMPF	R2	#0003
      0x58040000,  //  0002  LDCONST	R1	K0
      0x88080101,  //  0003  GETMBR	R2	R0	K1
      0x4C0C0000,  //  0004  LDNIL	R3
      0x1C080403,  //  0005  EQ	R2	R2	R3
      0x780A0001,  //  0006  JMPF	R2	#0009
      0x4C080000,  //  0007  LDNIL	R2
      0x80040400,  //  0008  RET	1	R2
      0x88080101,  //  0009  GETMBR	R2	R0	K1
      0x8C080502,  //  000A  GETMET	R2	R2	K2
      0x5C100200,  //  000B  MOVE	R4	R1
      0x7C080400,  //  000C  CALL	R2	2
      0xB80E0600,  //  000D  GETNGBL	R3	K3
      0x880C0704,  //  000E  GETMBR	R3	R3	K4
      0x1C0C0403,  //  000F  EQ	R3	R2	R3
      0x780E0001,  //  0010  JMPF	R3	#0013
      0x80060A00,  //  0011  RET	1	K5
      0x7002000C,  //  0012  JMP		#0020
      0xB80E0600,  //  0013  GETNGBL	R3	K3
      0x880C0706,  //  0014  GETMBR	R3	R3	K6
      0x1C0C0403,  //  0015  EQ	R3	R2	R3
      0x780E0001,  //  0016  JMPF	R3	#0019
      0x80060E00,  //  0017  RET	1	K7
      0x70020006,  //  0018  JMP		#0020
      0xB80E0600,  //  0019  GETNGBL	R3	K3
      0x880C0708,  //  001A  GETMBR	R3	R3	K8
      0x1C0C0403,  //  001B  EQ	R3	R2	R3
      0x780E0001,  //  001C  JMPF	R3	#001F
      0x80061200,  //  001D  RET	1	K9
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
extern const bclass be_class_lvh_obj;
be_local_closure(class_lvh_obj_get_pad_left,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_obj, 
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
** Solidified function: set_value_ofs_x
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_closure(class_lvh_obj_set_value_ofs_x,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_obj, 
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
** Solidified function: set_flex_in_new_track
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_closure(class_lvh_obj_set_flex_in_new_track,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_obj, 
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(_lv_obj),
    /* K1   */  be_nested_str_weak(add_flag),
    /* K2   */  be_nested_str_weak(lv),
    /* K3   */  be_nested_str_weak(OBJ_FLAG_FLEX_IN_NEW_TRACK),
    /* K4   */  be_nested_str_weak(clear_flag),
    }),
    be_str_weak(set_flex_in_new_track),
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
** Solidified function: member
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_closure(class_lvh_obj_member,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_obj, 
    1,                          /* has constants */
    ( &(const bvalue[18]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(introspect),
    /* K2   */  be_nested_str_weak(startswith),
    /* K3   */  be_nested_str_weak(set_),
    /* K4   */  be_nested_str_weak(get_),
    /* K5   */  be_const_int(3),
    /* K6   */  be_nested_str_weak(byte),
    /* K7   */  be_const_int(2147483647),
    /* K8   */  be_const_int(0),
    /* K9   */  be_nested_str_weak(digits_to_style),
    /* K10  */  be_nested_str_weak(_attr_ignore),
    /* K11  */  be_nested_str_weak(find),
    /* K12  */  be_nested_str_weak(get),
    /* K13  */  be_nested_str_weak(function),
    /* K14  */  be_nested_str_weak(_attr_map),
    /* K15  */  be_nested_str_weak(_lv_obj),
    /* K16  */  be_nested_str_weak(get_style_),
    /* K17  */  be_nested_str_weak(undefined),
    }),
    be_str_weak(member),
    &be_const_str_solidified,
    ( &(const binstruction[125]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0xA40E0200,  //  0001  IMPORT	R3	K1
      0x8C100502,  //  0002  GETMET	R4	R2	K2
      0x5C180200,  //  0003  MOVE	R6	R1
      0x581C0003,  //  0004  LDCONST	R7	K3
      0x7C100600,  //  0005  CALL	R4	3
      0x74120004,  //  0006  JMPT	R4	#000C
      0x8C100502,  //  0007  GETMET	R4	R2	K2
      0x5C180200,  //  0008  MOVE	R6	R1
      0x581C0004,  //  0009  LDCONST	R7	K4
      0x7C100600,  //  000A  CALL	R4	3
      0x78120000,  //  000B  JMPF	R4	#000D
      0x80000800,  //  000C  RET	0
      0x4C100000,  //  000D  LDNIL	R4
      0x6014000C,  //  000E  GETGBL	R5	G12
      0x5C180200,  //  000F  MOVE	R6	R1
      0x7C140200,  //  0010  CALL	R5	1
      0x28140B05,  //  0011  GE	R5	R5	K5
      0x7816001F,  //  0012  JMPF	R5	#0033
      0x8C140506,  //  0013  GETMET	R5	R2	K6
      0x541DFFFE,  //  0014  LDINT	R7	-1
      0x941C0207,  //  0015  GETIDX	R7	R1	R7
      0x7C140400,  //  0016  CALL	R5	2
      0x8C180506,  //  0017  GETMET	R6	R2	K6
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
      0x40201107,  //  0029  CONNECT	R8	R8	K7
      0x94200208,  //  002A  GETIDX	R8	R1	R8
      0x7C1C0200,  //  002B  CALL	R7	1
      0x5421FFFC,  //  002C  LDINT	R8	-3
      0x40221008,  //  002D  CONNECT	R8	K8	R8
      0x94040208,  //  002E  GETIDX	R1	R1	R8
      0x8C200109,  //  002F  GETMET	R8	R0	K9
      0x5C280E00,  //  0030  MOVE	R10	R7
      0x7C200400,  //  0031  CALL	R8	2
      0x5C101000,  //  0032  MOVE	R4	R8
      0x8814010A,  //  0033  GETMBR	R5	R0	K10
      0x8C140B0B,  //  0034  GETMET	R5	R5	K11
      0x5C1C0200,  //  0035  MOVE	R7	R1
      0x7C140400,  //  0036  CALL	R5	2
      0x4C180000,  //  0037  LDNIL	R6
      0x20140A06,  //  0038  NE	R5	R5	R6
      0x78160000,  //  0039  JMPF	R5	#003B
      0x80000A00,  //  003A  RET	0
      0x8C14070C,  //  003B  GETMET	R5	R3	K12
      0x5C1C0000,  //  003C  MOVE	R7	R0
      0x00220801,  //  003D  ADD	R8	K4	R1
      0x7C140600,  //  003E  CALL	R5	3
      0x60180004,  //  003F  GETGBL	R6	G4
      0x5C1C0A00,  //  0040  MOVE	R7	R5
      0x7C180200,  //  0041  CALL	R6	1
      0x1C180D0D,  //  0042  EQ	R6	R6	K13
      0x781A0009,  //  0043  JMPF	R6	#004E
      0x5C180A00,  //  0044  MOVE	R6	R5
      0x5C1C0000,  //  0045  MOVE	R7	R0
      0x4C200000,  //  0046  LDNIL	R8
      0x20200808,  //  0047  NE	R8	R4	R8
      0x78220001,  //  0048  JMPF	R8	#004B
      0x5C200800,  //  0049  MOVE	R8	R4
      0x70020000,  //  004A  JMP		#004C
      0x58200008,  //  004B  LDCONST	R8	K8
      0x7C180400,  //  004C  CALL	R6	2
      0x80040C00,  //  004D  RET	1	R6
      0x8818010E,  //  004E  GETMBR	R6	R0	K14
      0x8C180D0B,  //  004F  GETMET	R6	R6	K11
      0x5C200200,  //  0050  MOVE	R8	R1
      0x5C240200,  //  0051  MOVE	R9	R1
      0x7C180600,  //  0052  CALL	R6	3
      0x5C040C00,  //  0053  MOVE	R1	R6
      0x4C180000,  //  0054  LDNIL	R6
      0x1C180806,  //  0055  EQ	R6	R4	R6
      0x781A000D,  //  0056  JMPF	R6	#0065
      0x8C18070C,  //  0057  GETMET	R6	R3	K12
      0x8820010F,  //  0058  GETMBR	R8	R0	K15
      0x00260801,  //  0059  ADD	R9	K4	R1
      0x7C180600,  //  005A  CALL	R6	3
      0x5C140C00,  //  005B  MOVE	R5	R6
      0x60180004,  //  005C  GETGBL	R6	G4
      0x5C1C0A00,  //  005D  MOVE	R7	R5
      0x7C180200,  //  005E  CALL	R6	1
      0x1C180D0D,  //  005F  EQ	R6	R6	K13
      0x781A0003,  //  0060  JMPF	R6	#0065
      0x5C180A00,  //  0061  MOVE	R6	R5
      0x881C010F,  //  0062  GETMBR	R7	R0	K15
      0x7C180200,  //  0063  CALL	R6	1
      0x80040C00,  //  0064  RET	1	R6
      0x8C18070C,  //  0065  GETMET	R6	R3	K12
      0x8820010F,  //  0066  GETMBR	R8	R0	K15
      0x00262001,  //  0067  ADD	R9	K16	R1
      0x7C180600,  //  0068  CALL	R6	3
      0x5C140C00,  //  0069  MOVE	R5	R6
      0x60180004,  //  006A  GETGBL	R6	G4
      0x5C1C0A00,  //  006B  MOVE	R7	R5
      0x7C180200,  //  006C  CALL	R6	1
      0x1C180D0D,  //  006D  EQ	R6	R6	K13
      0x781A0009,  //  006E  JMPF	R6	#0079
      0x5C180A00,  //  006F  MOVE	R6	R5
      0x881C010F,  //  0070  GETMBR	R7	R0	K15
      0x4C200000,  //  0071  LDNIL	R8
      0x20200808,  //  0072  NE	R8	R4	R8
      0x78220001,  //  0073  JMPF	R8	#0076
      0x5C200800,  //  0074  MOVE	R8	R4
      0x70020000,  //  0075  JMP		#0077
      0x58200008,  //  0076  LDCONST	R8	K8
      0x7C180400,  //  0077  CALL	R6	2
      0x80040C00,  //  0078  RET	1	R6
      0x6018000B,  //  0079  GETGBL	R6	G11
      0x581C0011,  //  007A  LDCONST	R7	K17
      0x7C180200,  //  007B  CALL	R6	1
      0x80040C00,  //  007C  RET	1	R6
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_text_font
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_closure(class_lvh_obj_get_text_font,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_obj, 
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
** Solidified function: set_value_ofs_y
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_closure(class_lvh_obj_set_value_ofs_y,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_obj, 
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
** Solidified function: set_radius2
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_closure(class_lvh_obj_set_radius2,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_obj, 
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
** Solidified function: set_hidden
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_closure(class_lvh_obj_set_hidden,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_obj, 
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
** Solidified function: register_event_cb
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_closure(class_lvh_obj_register_event_cb,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_obj, 
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
** Solidified function: set_text_font
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_closure(class_lvh_obj_set_text_font,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_obj, 
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(parse_font),
    /* K1   */  be_nested_str_weak(_lv_obj),
    /* K2   */  be_nested_str_weak(set_style_text_font),
    /* K3   */  be_const_int(0),
    }),
    be_str_weak(set_text_font),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x5C100200,  //  0001  MOVE	R4	R1
      0x7C080400,  //  0002  CALL	R2	2
      0x4C0C0000,  //  0003  LDNIL	R3
      0x200C0403,  //  0004  NE	R3	R2	R3
      0x780E0004,  //  0005  JMPF	R3	#000B
      0x880C0101,  //  0006  GETMBR	R3	R0	K1
      0x8C0C0702,  //  0007  GETMET	R3	R3	K2
      0x5C140400,  //  0008  MOVE	R5	R2
      0x58180003,  //  0009  LDCONST	R6	K3
      0x7C0C0600,  //  000A  CALL	R3	3
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_pad_bottom
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_closure(class_lvh_obj_get_pad_bottom,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_obj, 
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
** Solidified function: get_pad_all
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_closure(class_lvh_obj_get_pad_all,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_obj, 
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
** Solidified function: get_val
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_closure(class_lvh_obj_get_val,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_obj, 
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
** Solidified function: post_init
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_closure(class_lvh_obj_post_init,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_obj, 
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
** Solidified class: lvh_obj
********************************************************************/
extern const bclass be_class_lvh_root;
be_local_class(lvh_obj,
    2,
    &be_class_lvh_root,
    be_nested_map(61,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(get_text, -1), be_const_closure(class_lvh_obj_get_text_closure) },
        { be_const_key_weak(set_pad_bottom2, 8), be_const_closure(class_lvh_obj_set_pad_bottom2_closure) },
        { be_const_key_weak(set_toggle, 29), be_const_closure(class_lvh_obj_set_toggle_closure) },
        { be_const_key_weak(set_pad_left2, 35), be_const_closure(class_lvh_obj_set_pad_left2_closure) },
        { be_const_key_weak(get_pad_top, 7), be_const_closure(class_lvh_obj_get_pad_top_closure) },
        { be_const_key_weak(get_line_width, -1), be_const_closure(class_lvh_obj_get_line_width_closure) },
        { be_const_key_weak(get_adjustable, -1), be_const_closure(class_lvh_obj_get_adjustable_closure) },
        { be_const_key_weak(check_label, 0), be_const_closure(class_lvh_obj_check_label_closure) },
        { be_const_key_weak(set_value_color, -1), be_const_closure(class_lvh_obj_set_value_color_closure) },
        { be_const_key_weak(set_text, -1), be_const_closure(class_lvh_obj_set_text_closure) },
        { be_const_key_weak(set_align, -1), be_const_closure(class_lvh_obj_set_align_closure) },
        { be_const_key_weak(get_value_color, 56), be_const_closure(class_lvh_obj_get_value_color_closure) },
        { be_const_key_weak(get_pad_bottom, -1), be_const_closure(class_lvh_obj_get_pad_bottom_closure) },
        { be_const_key_weak(set_text_font, 24), be_const_closure(class_lvh_obj_set_text_font_closure) },
        { be_const_key_weak(get_action, -1), be_const_closure(class_lvh_obj_get_action_closure) },
        { be_const_key_weak(get_value_ofs_y, -1), be_const_closure(class_lvh_obj_get_value_ofs_y_closure) },
        { be_const_key_weak(set_pad_all2, 37), be_const_closure(class_lvh_obj_set_pad_all2_closure) },
        { be_const_key_weak(register_event_cb, -1), be_const_closure(class_lvh_obj_register_event_cb_closure) },
        { be_const_key_weak(set_adjustable, -1), be_const_closure(class_lvh_obj_set_adjustable_closure) },
        { be_const_key_weak(get_flex_in_new_track, -1), be_const_closure(class_lvh_obj_get_flex_in_new_track_closure) },
        { be_const_key_weak(delete, 31), be_const_closure(class_lvh_obj_delete_closure) },
        { be_const_key_weak(set_radius2, -1), be_const_closure(class_lvh_obj_set_radius2_closure) },
        { be_const_key_weak(get_enabled, -1), be_const_closure(class_lvh_obj_get_enabled_closure) },
        { be_const_key_weak(_lv_class, -1), be_const_class(be_class_lv_obj) },
        { be_const_key_weak(member, -1), be_const_closure(class_lvh_obj_member_closure) },
        { be_const_key_weak(set_flex_in_new_track, -1), be_const_closure(class_lvh_obj_set_flex_in_new_track_closure) },
        { be_const_key_weak(event_cb, -1), be_const_closure(class_lvh_obj_event_cb_closure) },
        { be_const_key_weak(init, -1), be_const_closure(class_lvh_obj_init_closure) },
        { be_const_key_weak(_lv_label, 22), be_const_var(0) },
        { be_const_key_weak(_action, -1), be_const_var(1) },
        { be_const_key_weak(_lv_part2_selector, -1), be_const_nil() },
        { be_const_key_weak(set_value_font, 18), be_const_closure(class_lvh_obj_set_value_font_closure) },
        { be_const_key_weak(get_text_color, 40), be_const_closure(class_lvh_obj_get_text_color_closure) },
        { be_const_key_weak(set_click, 47), be_const_closure(class_lvh_obj_set_click_closure) },
        { be_const_key_weak(get_hidden, -1), be_const_closure(class_lvh_obj_get_hidden_closure) },
        { be_const_key_weak(get_align, 23), be_const_closure(class_lvh_obj_get_align_closure) },
        { be_const_key_weak(set_line_width, 45), be_const_closure(class_lvh_obj_set_line_width_closure) },
        { be_const_key_weak(set_val, 41), be_const_closure(class_lvh_obj_set_val_closure) },
        { be_const_key_weak(set_text_color, -1), be_const_closure(class_lvh_obj_set_text_color_closure) },
        { be_const_key_weak(set_action, -1), be_const_closure(class_lvh_obj_set_action_closure) },
        { be_const_key_weak(get_radius2, 28), be_const_closure(class_lvh_obj_get_radius2_closure) },
        { be_const_key_weak(get_label_mode, -1), be_const_closure(class_lvh_obj_get_label_mode_closure) },
        { be_const_key_weak(set_label_mode, -1), be_const_closure(class_lvh_obj_set_label_mode_closure) },
        { be_const_key_weak(setmember, -1), be_const_closure(class_lvh_obj_setmember_closure) },
        { be_const_key_weak(set_enabled, -1), be_const_closure(class_lvh_obj_set_enabled_closure) },
        { be_const_key_weak(get_value_font, -1), be_const_closure(class_lvh_obj_get_value_font_closure) },
        { be_const_key_weak(get_pad_left, -1), be_const_closure(class_lvh_obj_get_pad_left_closure) },
        { be_const_key_weak(get_toggle, -1), be_const_closure(class_lvh_obj_get_toggle_closure) },
        { be_const_key_weak(set_value_ofs_x, -1), be_const_closure(class_lvh_obj_set_value_ofs_x_closure) },
        { be_const_key_weak(_event_map, 25), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(7,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(7, -1), be_nested_str_weak(up) },
        { be_const_key_int(8, 4), be_nested_str_weak(release) },
        { be_const_key_int(1, -1), be_nested_str_weak(down) },
        { be_const_key_int(3, -1), be_nested_str_weak(lost) },
        { be_const_key_int(29, 2), be_nested_str_weak(changed) },
        { be_const_key_int(5, -1), be_nested_str_weak(long) },
        { be_const_key_int(6, -1), be_nested_str_weak(hold) },
    }))    ) } )) },
        { be_const_key_weak(set_pad_right2, 13), be_const_closure(class_lvh_obj_set_pad_right2_closure) },
        { be_const_key_weak(get_text_font, -1), be_const_closure(class_lvh_obj_get_text_font_closure) },
        { be_const_key_weak(set_value_ofs_y, -1), be_const_closure(class_lvh_obj_set_value_ofs_y_closure) },
        { be_const_key_weak(get_pad_right, 21), be_const_closure(class_lvh_obj_get_pad_right_closure) },
        { be_const_key_weak(set_hidden, -1), be_const_closure(class_lvh_obj_set_hidden_closure) },
        { be_const_key_weak(set_pad_top2, 17), be_const_closure(class_lvh_obj_set_pad_top2_closure) },
        { be_const_key_weak(get_value_ofs_x, -1), be_const_closure(class_lvh_obj_get_value_ofs_x_closure) },
        { be_const_key_weak(get_click, 12), be_const_closure(class_lvh_obj_get_click_closure) },
        { be_const_key_weak(get_pad_all, -1), be_const_closure(class_lvh_obj_get_pad_all_closure) },
        { be_const_key_weak(get_val, -1), be_const_closure(class_lvh_obj_get_val_closure) },
        { be_const_key_weak(post_init, -1), be_const_closure(class_lvh_obj_post_init_closure) },
    })),
    be_str_weak(lvh_obj)
);

extern const bclass be_class_lvh_fixed;

/********************************************************************
** Solidified function: post_init
********************************************************************/
extern const bclass be_class_lvh_fixed;
be_local_closure(class_lvh_fixed_post_init,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_fixed, 
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
    ( &(const binstruction[37]) {  /* code */
      0x60040003,  //  0000  GETGBL	R1	G3
      0x5C080000,  //  0001  MOVE	R2	R0
      0x7C040200,  //  0002  CALL	R1	1
      0x8C040300,  //  0003  GETMET	R1	R1	K0
      0x7C040200,  //  0004  CALL	R1	1
      0x88040101,  //  0005  GETMBR	R1	R0	K1
      0x8C080302,  //  0006  GETMET	R2	R1	K2
      0x58100003,  //  0007  LDCONST	R4	K3
      0x58140003,  //  0008  LDCONST	R5	K3
      0x7C080600,  //  0009  CALL	R2	3
      0x8C080304,  //  000A  GETMET	R2	R1	K4
      0x58100003,  //  000B  LDCONST	R4	K3
      0x58140003,  //  000C  LDCONST	R5	K3
      0x7C080600,  //  000D  CALL	R2	3
      0x8C080305,  //  000E  GETMET	R2	R1	K5
      0x58100003,  //  000F  LDCONST	R4	K3
      0x58140003,  //  0010  LDCONST	R5	K3
      0x7C080600,  //  0011  CALL	R2	3
      0x8C080306,  //  0012  GETMET	R2	R1	K6
      0x58100003,  //  0013  LDCONST	R4	K3
      0x58140003,  //  0014  LDCONST	R5	K3
      0x7C080600,  //  0015  CALL	R2	3
      0x8C080307,  //  0016  GETMET	R2	R1	K7
      0x58100003,  //  0017  LDCONST	R4	K3
      0x58140003,  //  0018  LDCONST	R5	K3
      0x7C080600,  //  0019  CALL	R2	3
      0x8C080308,  //  001A  GETMET	R2	R1	K8
      0xB8121200,  //  001B  GETNGBL	R4	K9
      0x8C10090A,  //  001C  GETMET	R4	R4	K10
      0x541A0063,  //  001D  LDINT	R6	100
      0x7C100400,  //  001E  CALL	R4	2
      0xB8161200,  //  001F  GETNGBL	R5	K9
      0x8C140B0A,  //  0020  GETMET	R5	R5	K10
      0x541E0063,  //  0021  LDINT	R7	100
      0x7C140400,  //  0022  CALL	R5	2
      0x7C080600,  //  0023  CALL	R2	3
      0x80000000,  //  0024  RET	0
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
extern const bclass be_class_lvh_flex;
be_local_closure(class_lvh_flex_post_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_flex, 
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
    ( &(const binstruction[11]) {  /* code */
      0x60040003,  //  0000  GETGBL	R1	G3
      0x5C080000,  //  0001  MOVE	R2	R0
      0x7C040200,  //  0002  CALL	R1	1
      0x8C040300,  //  0003  GETMET	R1	R1	K0
      0x7C040200,  //  0004  CALL	R1	1
      0x88040101,  //  0005  GETMBR	R1	R0	K1
      0x8C080302,  //  0006  GETMET	R2	R1	K2
      0xB8120600,  //  0007  GETNGBL	R4	K3
      0x88100904,  //  0008  GETMBR	R4	R4	K4
      0x7C080400,  //  0009  CALL	R2	2
      0x80000000,  //  000A  RET	0
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
    be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(post_init, -1), be_const_closure(class_lvh_flex_post_init_closure) },
    })),
    be_str_weak(lvh_flex)
);

extern const bclass be_class_lvh_label;

/********************************************************************
** Solidified function: post_init
********************************************************************/
extern const bclass be_class_lvh_label;
be_local_closure(class_lvh_label_post_init,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_label, 
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
        { be_const_key_weak(post_init, -1), be_const_closure(class_lvh_label_post_init_closure) },
        { be_const_key_weak(_lv_class, -1), be_const_class(be_class_lv_label) },
    })),
    be_str_weak(lvh_label)
);

extern const bclass be_class_lvh_arc;

/********************************************************************
** Solidified function: get_type
********************************************************************/
extern const bclass be_class_lvh_arc;
be_local_closure(class_lvh_arc_get_type,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_arc, 
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
** Solidified function: refresh_label_to_angle
********************************************************************/
extern const bclass be_class_lvh_arc;
be_local_closure(class_lvh_arc_refresh_label_to_angle,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_arc, 
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(_label_angle),
    /* K1   */  be_nested_str_weak(_lv_label),
    /* K2   */  be_nested_str_weak(_lv_obj),
    /* K3   */  be_nested_str_weak(rotate_obj_to_angle),
    }),
    be_str_weak(refresh_label_to_angle),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x4C080000,  //  0001  LDNIL	R2
      0x20040202,  //  0002  NE	R1	R1	R2
      0x78060008,  //  0003  JMPF	R1	#000D
      0x88040101,  //  0004  GETMBR	R1	R0	K1
      0x4C080000,  //  0005  LDNIL	R2
      0x20040202,  //  0006  NE	R1	R1	R2
      0x78060004,  //  0007  JMPF	R1	#000D
      0x88040102,  //  0008  GETMBR	R1	R0	K2
      0x8C040303,  //  0009  GETMET	R1	R1	K3
      0x880C0101,  //  000A  GETMBR	R3	R0	K1
      0x88100100,  //  000B  GETMBR	R4	R0	K0
      0x7C040600,  //  000C  CALL	R1	3
      0x80000000,  //  000D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_min
********************************************************************/
extern const bclass be_class_lvh_arc;
be_local_closure(class_lvh_arc_set_min,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_arc, 
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(_lv_obj),
    /* K1   */  be_nested_str_weak(set_range),
    /* K2   */  be_nested_str_weak(get_max),
    /* K3   */  be_nested_str_weak(refresh_label_to_angle),
    }),
    be_str_weak(set_min),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x60100009,  //  0002  GETGBL	R4	G9
      0x5C140200,  //  0003  MOVE	R5	R1
      0x7C100200,  //  0004  CALL	R4	1
      0x8C140102,  //  0005  GETMET	R5	R0	K2
      0x7C140200,  //  0006  CALL	R5	1
      0x7C080600,  //  0007  CALL	R2	3
      0x8C080103,  //  0008  GETMET	R2	R0	K3
      0x7C080200,  //  0009  CALL	R2	1
      0x80000000,  //  000A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_line_width1
********************************************************************/
extern const bclass be_class_lvh_arc;
be_local_closure(class_lvh_arc_get_line_width1,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_arc, 
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
** Solidified function: set_type
********************************************************************/
extern const bclass be_class_lvh_arc;
be_local_closure(class_lvh_arc_set_type,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_arc, 
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
** Solidified function: set_line_width1
********************************************************************/
extern const bclass be_class_lvh_arc;
be_local_closure(class_lvh_arc_set_line_width1,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_arc, 
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
extern const bclass be_class_lvh_arc;
be_local_closure(class_lvh_arc_get_min,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_arc, 
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
** Solidified function: set_line_width
********************************************************************/
extern const bclass be_class_lvh_arc;
be_local_closure(class_lvh_arc_set_line_width,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_arc, 
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
** Solidified function: get_max
********************************************************************/
extern const bclass be_class_lvh_arc;
be_local_closure(class_lvh_arc_get_max,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_arc, 
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
** Solidified function: set_label_to_angle
********************************************************************/
extern const bclass be_class_lvh_arc;
be_local_closure(class_lvh_arc_set_label_to_angle,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_arc, 
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(_label_angle),
    /* K1   */  be_nested_str_weak(refresh_label_to_angle),
    }),
    be_str_weak(set_label_to_angle),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x60080009,  //  0000  GETGBL	R2	G9
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
** Solidified function: set_val
********************************************************************/
extern const bclass be_class_lvh_arc;
be_local_closure(class_lvh_arc_set_val,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_arc, 
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(set_val),
    /* K1   */  be_nested_str_weak(refresh_label_to_angle),
    }),
    be_str_weak(set_val),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080500,  //  0003  GETMET	R2	R2	K0
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x8C080101,  //  0006  GETMET	R2	R0	K1
      0x7C080200,  //  0007  CALL	R2	1
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: post_config
********************************************************************/
extern const bclass be_class_lvh_arc;
be_local_closure(class_lvh_arc_post_config,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_arc, 
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(post_config),
    /* K1   */  be_nested_str_weak(refresh_label_to_angle),
    }),
    be_str_weak(post_config),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x60040003,  //  0000  GETGBL	R1	G3
      0x5C080000,  //  0001  MOVE	R2	R0
      0x7C040200,  //  0002  CALL	R1	1
      0x8C040300,  //  0003  GETMET	R1	R1	K0
      0x7C040200,  //  0004  CALL	R1	1
      0x8C040101,  //  0005  GETMET	R1	R0	K1
      0x7C040200,  //  0006  CALL	R1	1
      0x80000000,  //  0007  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_line_width
********************************************************************/
extern const bclass be_class_lvh_arc;
be_local_closure(class_lvh_arc_get_line_width,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_arc, 
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
** Solidified function: set_max
********************************************************************/
extern const bclass be_class_lvh_arc;
be_local_closure(class_lvh_arc_set_max,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_arc, 
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(_lv_obj),
    /* K1   */  be_nested_str_weak(set_range),
    /* K2   */  be_nested_str_weak(get_min),
    /* K3   */  be_nested_str_weak(refresh_label_to_angle),
    }),
    be_str_weak(set_max),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x8C100102,  //  0002  GETMET	R4	R0	K2
      0x7C100200,  //  0003  CALL	R4	1
      0x60140009,  //  0004  GETGBL	R5	G9
      0x5C180200,  //  0005  MOVE	R6	R1
      0x7C140200,  //  0006  CALL	R5	1
      0x7C080600,  //  0007  CALL	R2	3
      0x8C080103,  //  0008  GETMET	R2	R0	K3
      0x7C080200,  //  0009  CALL	R2	1
      0x80000000,  //  000A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_text
********************************************************************/
extern const bclass be_class_lvh_arc;
be_local_closure(class_lvh_arc_set_text,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_arc, 
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(set_text),
    /* K1   */  be_nested_str_weak(refresh_label_to_angle),
    }),
    be_str_weak(set_text),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080500,  //  0003  GETMET	R2	R2	K0
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x8C080101,  //  0006  GETMET	R2	R0	K1
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
extern const bclass be_class_lvh_switch;
be_local_closure(class_lvh_switch_set_val,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_switch, 
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
extern const bclass be_class_lvh_switch;
be_local_closure(class_lvh_switch_get_val,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_switch, 
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

extern const bclass be_class_lvh_spinner;

/********************************************************************
** Solidified function: set_angle
********************************************************************/
extern const bclass be_class_lvh_spinner;
be_local_closure(class_lvh_spinner_set_angle,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_spinner, 
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
extern const bclass be_class_lvh_spinner;
be_local_closure(class_lvh_spinner_init,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_spinner, 
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(_page),
    /* K1   */  be_nested_str_weak(find),
    /* K2   */  be_nested_str_weak(angle),
    /* K3   */  be_nested_str_weak(speed),
    /* K4   */  be_nested_str_weak(_lv_obj),
    /* K5   */  be_nested_str_weak(lv),
    /* K6   */  be_nested_str_weak(spinner),
    /* K7   */  be_nested_str_weak(set_anim_params),
    /* K8   */  be_nested_str_weak(post_init),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
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
      0x7C180400,  //  000C  CALL	R6	2
      0x90020806,  //  000D  SETMBR	R0	K4	R6
      0x88180104,  //  000E  GETMBR	R6	R0	K4
      0x8C180D07,  //  000F  GETMET	R6	R6	K7
      0x5C200A00,  //  0010  MOVE	R8	R5
      0x5C240800,  //  0011  MOVE	R9	R4
      0x7C180600,  //  0012  CALL	R6	3
      0x8C180108,  //  0013  GETMET	R6	R0	K8
      0x7C180200,  //  0014  CALL	R6	1
      0x80000000,  //  0015  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_speed
********************************************************************/
extern const bclass be_class_lvh_spinner;
be_local_closure(class_lvh_spinner_get_speed,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_spinner, 
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
extern const bclass be_class_lvh_spinner;
be_local_closure(class_lvh_spinner_get_angle,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_spinner, 
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
extern const bclass be_class_lvh_spinner;
be_local_closure(class_lvh_spinner_set_speed,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_spinner, 
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

extern const bclass be_class_lvh_img;

/********************************************************************
** Solidified function: set_src
********************************************************************/
extern const bclass be_class_lvh_img;
be_local_closure(class_lvh_img_set_src,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_img, 
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(tasmota_logo),
    /* K1   */  be_nested_str_weak(_lv_obj),
    /* K2   */  be_nested_str_weak(set_tasmota_logo),
    /* K3   */  be_nested_str_weak(set_src),
    }),
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
extern const bclass be_class_lvh_img;
be_local_closure(class_lvh_img_get_auto_size,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_img, 
    0,                          /* has constants */
    NULL,                       /* no const */
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
extern const bclass be_class_lvh_img;
be_local_closure(class_lvh_img_set_auto_size,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_img, 
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(_lv_obj),
    /* K1   */  be_nested_str_weak(set_inner_align),
    /* K2   */  be_nested_str_weak(lv),
    /* K3   */  be_nested_str_weak(IMAGE_ALIGN_STRETCH),
    }),
    be_str_weak(set_auto_size),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x78060004,  //  0000  JMPF	R1	#0006
      0x88080100,  //  0001  GETMBR	R2	R0	K0
      0x8C080501,  //  0002  GETMET	R2	R2	K1
      0xB8120400,  //  0003  GETNGBL	R4	K2
      0x88100903,  //  0004  GETMBR	R4	R4	K3
      0x7C080400,  //  0005  CALL	R2	2
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_angle
********************************************************************/
extern const bclass be_class_lvh_img;
be_local_closure(class_lvh_img_set_angle,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_img, 
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
extern const bclass be_class_lvh_img;
be_local_closure(class_lvh_img_get_angle,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_img, 
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
** Solidified function: set_raw
********************************************************************/
extern const bclass be_class_lvh_img;
be_local_closure(class_lvh_img_set_raw,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_img, 
    1,                          /* has constants */
    ( &(const bvalue[12]) {     /* constants */
    /* K0   */  be_nested_str_weak(_raw),
    /* K1   */  be_nested_str_weak(fromb64),
    /* K2   */  be_nested_str_weak(lv),
    /* K3   */  be_nested_str_weak(lv_image_dsc),
    /* K4   */  be_nested_str_weak(header_cf),
    /* K5   */  be_nested_str_weak(COLOR_FORMAT_RAW),
    /* K6   */  be_nested_str_weak(data_size),
    /* K7   */  be_nested_str_weak(data),
    /* K8   */  be_nested_str_weak(_buffer),
    /* K9   */  be_nested_str_weak(_imd_dsc),
    /* K10  */  be_nested_str_weak(_lv_obj),
    /* K11  */  be_nested_str_weak(set_src),
    }),
    be_str_weak(set_raw),
    &be_const_str_solidified,
    ( &(const binstruction[26]) {  /* code */
      0x60080015,  //  0000  GETGBL	R2	G21
      0x7C080000,  //  0001  CALL	R2	0
      0x8C080501,  //  0002  GETMET	R2	R2	K1
      0x5C100200,  //  0003  MOVE	R4	R1
      0x7C080400,  //  0004  CALL	R2	2
      0x90020002,  //  0005  SETMBR	R0	K0	R2
      0xB80A0400,  //  0006  GETNGBL	R2	K2
      0x8C080503,  //  0007  GETMET	R2	R2	K3
      0x7C080200,  //  0008  CALL	R2	1
      0xB80E0400,  //  0009  GETNGBL	R3	K2
      0x880C0705,  //  000A  GETMBR	R3	R3	K5
      0x900A0803,  //  000B  SETMBR	R2	K4	R3
      0x600C000C,  //  000C  GETGBL	R3	G12
      0x88100100,  //  000D  GETMBR	R4	R0	K0
      0x7C0C0200,  //  000E  CALL	R3	1
      0x900A0C03,  //  000F  SETMBR	R2	K6	R3
      0x880C0100,  //  0010  GETMBR	R3	R0	K0
      0x8C0C0708,  //  0011  GETMET	R3	R3	K8
      0x7C0C0200,  //  0012  CALL	R3	1
      0x900A0E03,  //  0013  SETMBR	R2	K7	R3
      0x90021202,  //  0014  SETMBR	R0	K9	R2
      0x880C010A,  //  0015  GETMBR	R3	R0	K10
      0x8C0C070B,  //  0016  GETMET	R3	R3	K11
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

extern const bclass be_class_lvh_qrcode;

/********************************************************************
** Solidified function: get_qr_dark_color
********************************************************************/
extern const bclass be_class_lvh_qrcode;
be_local_closure(class_lvh_qrcode_get_qr_dark_color,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_qrcode, 
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
** Solidified function: set_qr_text
********************************************************************/
extern const bclass be_class_lvh_qrcode;
be_local_closure(class_lvh_qrcode_set_qr_text,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_qrcode, 
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(qr_text),
    /* K1   */  be_nested_str_weak(_update),
    }),
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
extern const bclass be_class_lvh_qrcode;
be_local_closure(class_lvh_qrcode_set_light_color,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_qrcode, 
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(_lv_obj),
    /* K1   */  be_nested_str_weak(set_light_color),
    /* K2   */  be_nested_str_weak(parse_color),
    /* K3   */  be_nested_str_weak(_update),
    }),
    be_str_weak(set_light_color),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x8C100102,  //  0002  GETMET	R4	R0	K2
      0x5C180200,  //  0003  MOVE	R6	R1
      0x7C100400,  //  0004  CALL	R4	2
      0x7C080400,  //  0005  CALL	R2	2
      0x8C080103,  //  0006  GETMET	R2	R0	K3
      0x7C080200,  //  0007  CALL	R2	1
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_dark_color
********************************************************************/
extern const bclass be_class_lvh_qrcode;
be_local_closure(class_lvh_qrcode_get_dark_color,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_qrcode, 
    0,                          /* has constants */
    NULL,                       /* no const */
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
extern const bclass be_class_lvh_qrcode;
be_local_closure(class_lvh_qrcode_set_qr_light_color,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_qrcode, 
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(_lv_obj),
    /* K1   */  be_nested_str_weak(set_light_color),
    /* K2   */  be_nested_str_weak(parse_color),
    /* K3   */  be_nested_str_weak(_update),
    }),
    be_str_weak(set_qr_light_color),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x8C100102,  //  0002  GETMET	R4	R0	K2
      0x5C180200,  //  0003  MOVE	R6	R1
      0x7C100400,  //  0004  CALL	R4	2
      0x7C080400,  //  0005  CALL	R2	2
      0x8C080103,  //  0006  GETMET	R2	R0	K3
      0x7C080200,  //  0007  CALL	R2	1
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_qr_light_color
********************************************************************/
extern const bclass be_class_lvh_qrcode;
be_local_closure(class_lvh_qrcode_get_qr_light_color,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_qrcode, 
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
** Solidified function: set_qr_size
********************************************************************/
extern const bclass be_class_lvh_qrcode;
be_local_closure(class_lvh_qrcode_set_qr_size,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_qrcode, 
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(_lv_obj),
    /* K1   */  be_nested_str_weak(set_size),
    /* K2   */  be_nested_str_weak(_update),
    }),
    be_str_weak(set_qr_size),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x8C080102,  //  0004  GETMET	R2	R0	K2
      0x7C080200,  //  0005  CALL	R2	1
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_light_color
********************************************************************/
extern const bclass be_class_lvh_qrcode;
be_local_closure(class_lvh_qrcode_get_light_color,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_qrcode, 
    0,                          /* has constants */
    NULL,                       /* no const */
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
extern const bclass be_class_lvh_qrcode;
be_local_closure(class_lvh_qrcode_set_size,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_qrcode, 
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(_lv_obj),
    /* K1   */  be_nested_str_weak(set_size),
    /* K2   */  be_nested_str_weak(_update),
    }),
    be_str_weak(set_size),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x8C080102,  //  0004  GETMET	R2	R0	K2
      0x7C080200,  //  0005  CALL	R2	1
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_qr_dark_color
********************************************************************/
extern const bclass be_class_lvh_qrcode;
be_local_closure(class_lvh_qrcode_set_qr_dark_color,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_qrcode, 
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(_lv_obj),
    /* K1   */  be_nested_str_weak(set_dark_color),
    /* K2   */  be_nested_str_weak(parse_color),
    /* K3   */  be_nested_str_weak(_update),
    }),
    be_str_weak(set_qr_dark_color),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x8C100102,  //  0002  GETMET	R4	R0	K2
      0x5C180200,  //  0003  MOVE	R6	R1
      0x7C100400,  //  0004  CALL	R4	2
      0x7C080400,  //  0005  CALL	R2	2
      0x8C080103,  //  0006  GETMET	R2	R0	K3
      0x7C080200,  //  0007  CALL	R2	1
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_qr_text
********************************************************************/
extern const bclass be_class_lvh_qrcode;
be_local_closure(class_lvh_qrcode_get_qr_text,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_qrcode, 
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
** Solidified function: set_dark_color
********************************************************************/
extern const bclass be_class_lvh_qrcode;
be_local_closure(class_lvh_qrcode_set_dark_color,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_qrcode, 
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(_lv_obj),
    /* K1   */  be_nested_str_weak(set_dark_color),
    /* K2   */  be_nested_str_weak(parse_color),
    /* K3   */  be_nested_str_weak(_update),
    }),
    be_str_weak(set_dark_color),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x8C100102,  //  0002  GETMET	R4	R0	K2
      0x5C180200,  //  0003  MOVE	R6	R1
      0x7C100400,  //  0004  CALL	R4	2
      0x7C080400,  //  0005  CALL	R2	2
      0x8C080103,  //  0006  GETMET	R2	R0	K3
      0x7C080200,  //  0007  CALL	R2	1
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_size
********************************************************************/
extern const bclass be_class_lvh_qrcode;
be_local_closure(class_lvh_qrcode_get_size,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_qrcode, 
    0,                          /* has constants */
    NULL,                       /* no const */
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
extern const bclass be_class_lvh_qrcode;
be_local_closure(class_lvh_qrcode_post_config,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_qrcode, 
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(post_config),
    /* K1   */  be_nested_str_weak(_update),
    }),
    be_str_weak(post_config),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x60040003,  //  0000  GETGBL	R1	G3
      0x5C080000,  //  0001  MOVE	R2	R0
      0x7C040200,  //  0002  CALL	R1	1
      0x8C040300,  //  0003  GETMET	R1	R1	K0
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
extern const bclass be_class_lvh_qrcode;
be_local_closure(class_lvh_qrcode__update,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_qrcode, 
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(qr_text),
    /* K1   */  be_nested_str_weak(_lv_obj),
    /* K2   */  be_nested_str_weak(update),
    }),
    be_str_weak(_update),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x4C080000,  //  0001  LDNIL	R2
      0x20080202,  //  0002  NE	R2	R1	R2
      0x780A0006,  //  0003  JMPF	R2	#000B
      0x88080101,  //  0004  GETMBR	R2	R0	K1
      0x8C080502,  //  0005  GETMET	R2	R2	K2
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
extern const bclass be_class_lvh_qrcode;
be_local_closure(class_lvh_qrcode_get_qr_size,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_qrcode, 
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

extern const bclass be_class_lvh_slider;

/********************************************************************
** Solidified function: set_max
********************************************************************/
extern const bclass be_class_lvh_slider;
be_local_closure(class_lvh_slider_set_max,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_slider, 
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
** Solidified function: get_min
********************************************************************/
extern const bclass be_class_lvh_slider;
be_local_closure(class_lvh_slider_get_min,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_slider, 
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
** Solidified function: set_val
********************************************************************/
extern const bclass be_class_lvh_slider;
be_local_closure(class_lvh_slider_set_val,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_slider, 
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(_val),
    /* K1   */  be_nested_str_weak(_lv_obj),
    /* K2   */  be_nested_str_weak(set_value),
    /* K3   */  be_const_int(0),
    }),
    be_str_weak(set_val),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x88080101,  //  0001  GETMBR	R2	R0	K1
      0x8C080502,  //  0002  GETMET	R2	R2	K2
      0x5C100200,  //  0003  MOVE	R4	R1
      0x58140003,  //  0004  LDCONST	R5	K3
      0x7C080600,  //  0005  CALL	R2	3
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_max
********************************************************************/
extern const bclass be_class_lvh_slider;
be_local_closure(class_lvh_slider_get_max,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_slider, 
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
** Solidified function: set_min
********************************************************************/
extern const bclass be_class_lvh_slider;
be_local_closure(class_lvh_slider_set_min,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_slider, 
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

extern const bclass be_class_lvh_roller;

/********************************************************************
** Solidified function: set_options
********************************************************************/
extern const bclass be_class_lvh_roller;
be_local_closure(class_lvh_roller_set_options,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_roller, 
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
extern const bclass be_class_lvh_roller;
be_local_closure(class_lvh_roller_get_text,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_roller, 
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
extern const bclass be_class_lvh_roller;
be_local_closure(class_lvh_roller_get_options,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_roller, 
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
extern const bclass be_class_lvh_roller;
be_local_closure(class_lvh_roller_set_val,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_roller, 
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(_val),
    /* K1   */  be_nested_str_weak(_lv_obj),
    /* K2   */  be_nested_str_weak(set_selected),
    /* K3   */  be_const_int(0),
    }),
    be_str_weak(set_val),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x88080101,  //  0001  GETMBR	R2	R0	K1
      0x8C080502,  //  0002  GETMET	R2	R2	K2
      0x5C100200,  //  0003  MOVE	R4	R1
      0x58140003,  //  0004  LDCONST	R5	K3
      0x7C080600,  //  0005  CALL	R2	3
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_text
********************************************************************/
extern const bclass be_class_lvh_roller;
be_local_closure(class_lvh_roller_set_text,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_roller, 
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
extern const bclass be_class_lvh_roller;
be_local_closure(class_lvh_roller_get_val,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_roller, 
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

extern const bclass be_class_lvh_led;

/********************************************************************
** Solidified function: set_val
********************************************************************/
extern const bclass be_class_lvh_led;
be_local_closure(class_lvh_led_set_val,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_led, 
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(_val),
    /* K1   */  be_nested_str_weak(_lv_obj),
    /* K2   */  be_nested_str_weak(set_brightness),
    }),
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
extern const bclass be_class_lvh_led;
be_local_closure(class_lvh_led_get_color,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_led, 
    0,                          /* has constants */
    NULL,                       /* no const */
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
extern const bclass be_class_lvh_led;
be_local_closure(class_lvh_led_get_val,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_led, 
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(_lv_obj),
    /* K1   */  be_nested_str_weak(get_brightness),
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
** Solidified function: set_color
********************************************************************/
extern const bclass be_class_lvh_led;
be_local_closure(class_lvh_led_set_color,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_led, 
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(parse_color),
    /* K1   */  be_nested_str_weak(_lv_obj),
    /* K2   */  be_nested_str_weak(set_color),
    }),
    be_str_weak(set_color),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x5C100200,  //  0001  MOVE	R4	R1
      0x7C080400,  //  0002  CALL	R2	2
      0x880C0101,  //  0003  GETMBR	R3	R0	K1
      0x8C0C0702,  //  0004  GETMET	R3	R3	K2
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

extern const bclass be_class_lvh_dropdown;

/********************************************************************
** Solidified function: get_val
********************************************************************/
extern const bclass be_class_lvh_dropdown;
be_local_closure(class_lvh_dropdown_get_val,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_dropdown, 
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
** Solidified function: set_text
********************************************************************/
extern const bclass be_class_lvh_dropdown;
be_local_closure(class_lvh_dropdown_set_text,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_dropdown, 
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
** Solidified function: set_direction
********************************************************************/
extern const bclass be_class_lvh_dropdown;
be_local_closure(class_lvh_dropdown_set_direction,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_dropdown, 
    1,                          /* has constants */
    ( &(const bvalue[14]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_const_int(3),
    /* K2   */  be_nested_str_weak(_lv_obj),
    /* K3   */  be_nested_str_weak(set_dir),
    /* K4   */  be_nested_str_weak(_dir),
    /* K5   */  be_const_int(1),
    /* K6   */  be_nested_str_weak(_symbol),
    /* K7   */  be_nested_str_weak(lv),
    /* K8   */  be_nested_str_weak(SYMBOL_UP),
    /* K9   */  be_const_int(2),
    /* K10  */  be_nested_str_weak(SYMBOL_LEFT),
    /* K11  */  be_nested_str_weak(SYMBOL_RIGHT),
    /* K12  */  be_nested_str_weak(SYMBOL_DOWN),
    /* K13  */  be_nested_str_weak(set_symbol),
    }),
    be_str_weak(set_direction),
    &be_const_str_solidified,
    ( &(const binstruction[40]) {  /* code */
      0x60080009,  //  0000  GETGBL	R2	G9
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x7C080200,  //  0002  CALL	R2	1
      0x5C040400,  //  0003  MOVE	R1	R2
      0x14080300,  //  0004  LT	R2	R1	K0
      0x740A0001,  //  0005  JMPT	R2	#0008
      0x24080301,  //  0006  GT	R2	R1	K1
      0x780A0000,  //  0007  JMPF	R2	#0009
      0x58040000,  //  0008  LDCONST	R1	K0
      0x88080102,  //  0009  GETMBR	R2	R0	K2
      0x8C080503,  //  000A  GETMET	R2	R2	K3
      0x88100104,  //  000B  GETMBR	R4	R0	K4
      0x94100801,  //  000C  GETIDX	R4	R4	R1
      0x7C080400,  //  000D  CALL	R2	2
      0x1C080305,  //  000E  EQ	R2	R1	K5
      0x780A0003,  //  000F  JMPF	R2	#0014
      0xB80A0E00,  //  0010  GETNGBL	R2	K7
      0x88080508,  //  0011  GETMBR	R2	R2	K8
      0x90020C02,  //  0012  SETMBR	R0	K6	R2
      0x7002000E,  //  0013  JMP		#0023
      0x1C080309,  //  0014  EQ	R2	R1	K9
      0x780A0003,  //  0015  JMPF	R2	#001A
      0xB80A0E00,  //  0016  GETNGBL	R2	K7
      0x8808050A,  //  0017  GETMBR	R2	R2	K10
      0x90020C02,  //  0018  SETMBR	R0	K6	R2
      0x70020008,  //  0019  JMP		#0023
      0x1C080301,  //  001A  EQ	R2	R1	K1
      0x780A0003,  //  001B  JMPF	R2	#0020
      0xB80A0E00,  //  001C  GETNGBL	R2	K7
      0x8808050B,  //  001D  GETMBR	R2	R2	K11
      0x90020C02,  //  001E  SETMBR	R0	K6	R2
      0x70020002,  //  001F  JMP		#0023
      0xB80A0E00,  //  0020  GETNGBL	R2	K7
      0x8808050C,  //  0021  GETMBR	R2	R2	K12
      0x90020C02,  //  0022  SETMBR	R0	K6	R2
      0x88080102,  //  0023  GETMBR	R2	R0	K2
      0x8C08050D,  //  0024  GETMET	R2	R2	K13
      0x88100106,  //  0025  GETMBR	R4	R0	K6
      0x7C080400,  //  0026  CALL	R2	2
      0x80000000,  //  0027  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_direction
********************************************************************/
extern const bclass be_class_lvh_dropdown;
be_local_closure(class_lvh_dropdown_get_direction,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_dropdown, 
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
extern const bclass be_class_lvh_dropdown;
be_local_closure(class_lvh_dropdown_set_show_selected,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_dropdown, 
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
** Solidified function: set_val
********************************************************************/
extern const bclass be_class_lvh_dropdown;
be_local_closure(class_lvh_dropdown_set_val,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_dropdown, 
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(_val),
    /* K1   */  be_nested_str_weak(_lv_obj),
    /* K2   */  be_nested_str_weak(set_selected),
    }),
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
** Solidified function: get_text
********************************************************************/
extern const bclass be_class_lvh_dropdown;
be_local_closure(class_lvh_dropdown_get_text,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_dropdown, 
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
** Solidified function: get_show_selected
********************************************************************/
extern const bclass be_class_lvh_dropdown;
be_local_closure(class_lvh_dropdown_get_show_selected,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_dropdown, 
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
extern const bclass be_class_lvh_dropdown_list;
be_local_closure(class_lvh_dropdown_list_post_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_dropdown_list, 
    1,                          /* has constants */
    ( &(const bvalue[10]) {     /* constants */
    /* K0   */  be_nested_str_weak(_lv_obj),
    /* K1   */  be_nested_str_weak(_parent_lvh),
    /* K2   */  be_nested_str_weak(_page),
    /* K3   */  be_nested_str_weak(_oh),
    /* K4   */  be_nested_str_weak(lvh_dropdown),
    /* K5   */  be_nested_str_weak(lv),
    /* K6   */  be_nested_str_weak(list),
    /* K7   */  be_nested_str_weak(get_list),
    /* K8   */  be_nested_str_weak(_p),
    /* K9   */  be_nested_str_weak(HSP_X3A_X20_X27_dropdown_list_X27_X20should_X20have_X20a_X20parent_X20of_X20type_X20_X27dropdown_X27),
    }),
    be_str_weak(post_init),
    &be_const_str_solidified,
    ( &(const binstruction[23]) {  /* code */
      0x4C040000,  //  0000  LDNIL	R1
      0x90020001,  //  0001  SETMBR	R0	K0	R1
      0x6004000F,  //  0002  GETGBL	R1	G15
      0x88080101,  //  0003  GETMBR	R2	R0	K1
      0x880C0102,  //  0004  GETMBR	R3	R0	K2
      0x880C0703,  //  0005  GETMBR	R3	R3	K3
      0x880C0704,  //  0006  GETMBR	R3	R3	K4
      0x7C040400,  //  0007  CALL	R1	2
      0x78060009,  //  0008  JMPF	R1	#0013
      0xB8060A00,  //  0009  GETNGBL	R1	K5
      0x8C040306,  //  000A  GETMET	R1	R1	K6
      0x880C0101,  //  000B  GETMBR	R3	R0	K1
      0x880C0700,  //  000C  GETMBR	R3	R3	K0
      0x8C0C0707,  //  000D  GETMET	R3	R3	K7
      0x7C0C0200,  //  000E  CALL	R3	1
      0x880C0708,  //  000F  GETMBR	R3	R3	K8
      0x7C040400,  //  0010  CALL	R1	2
      0x90020001,  //  0011  SETMBR	R0	K0	R1
      0x70020002,  //  0012  JMP		#0016
      0x60040001,  //  0013  GETGBL	R1	G1
      0x58080009,  //  0014  LDCONST	R2	K9
      0x7C040200,  //  0015  CALL	R1	1
      0x80000000,  //  0016  RET	0
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

extern const bclass be_class_lvh_bar;

/********************************************************************
** Solidified function: get_max
********************************************************************/
extern const bclass be_class_lvh_bar;
be_local_closure(class_lvh_bar_get_max,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_bar, 
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
** Solidified function: set_max
********************************************************************/
extern const bclass be_class_lvh_bar;
be_local_closure(class_lvh_bar_set_max,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_bar, 
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(_lv_obj),
    /* K1   */  be_nested_str_weak(set_range),
    /* K2   */  be_nested_str_weak(get_min_value),
    }),
    be_str_weak(set_max),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x88100100,  //  0002  GETMBR	R4	R0	K0
      0x8C100902,  //  0003  GETMET	R4	R4	K2
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
extern const bclass be_class_lvh_bar;
be_local_closure(class_lvh_bar_post_init,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_bar, 
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(post_init),
    /* K1   */  be_nested_str_weak(_parent_lvh),
    /* K2   */  be_nested_str_weak(_page),
    /* K3   */  be_nested_str_weak(_oh),
    /* K4   */  be_nested_str_weak(lvh_scale),
    /* K5   */  be_nested_str_weak(_lv_obj),
    /* K6   */  be_nested_str_weak(get_range_min_value),
    /* K7   */  be_nested_str_weak(get_range_max_value),
    /* K8   */  be_nested_str_weak(set_range),
    }),
    be_str_weak(post_init),
    &be_const_str_solidified,
    ( &(const binstruction[26]) {  /* code */
      0x60040003,  //  0000  GETGBL	R1	G3
      0x5C080000,  //  0001  MOVE	R2	R0
      0x7C040200,  //  0002  CALL	R1	1
      0x8C040300,  //  0003  GETMET	R1	R1	K0
      0x7C040200,  //  0004  CALL	R1	1
      0x6004000F,  //  0005  GETGBL	R1	G15
      0x88080101,  //  0006  GETMBR	R2	R0	K1
      0x880C0102,  //  0007  GETMBR	R3	R0	K2
      0x880C0703,  //  0008  GETMBR	R3	R3	K3
      0x880C0704,  //  0009  GETMBR	R3	R3	K4
      0x7C040400,  //  000A  CALL	R1	2
      0x7806000C,  //  000B  JMPF	R1	#0019
      0x88040101,  //  000C  GETMBR	R1	R0	K1
      0x88040305,  //  000D  GETMBR	R1	R1	K5
      0x8C040306,  //  000E  GETMET	R1	R1	K6
      0x7C040200,  //  000F  CALL	R1	1
      0x88080101,  //  0010  GETMBR	R2	R0	K1
      0x88080505,  //  0011  GETMBR	R2	R2	K5
      0x8C080507,  //  0012  GETMET	R2	R2	K7
      0x7C080200,  //  0013  CALL	R2	1
      0x880C0105,  //  0014  GETMBR	R3	R0	K5
      0x8C0C0708,  //  0015  GETMET	R3	R3	K8
      0x5C140200,  //  0016  MOVE	R5	R1
      0x5C180400,  //  0017  MOVE	R6	R2
      0x7C0C0600,  //  0018  CALL	R3	3
      0x80000000,  //  0019  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_min
********************************************************************/
extern const bclass be_class_lvh_bar;
be_local_closure(class_lvh_bar_get_min,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_bar, 
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
** Solidified function: set_min
********************************************************************/
extern const bclass be_class_lvh_bar;
be_local_closure(class_lvh_bar_set_min,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_bar, 
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(_lv_obj),
    /* K1   */  be_nested_str_weak(set_range),
    /* K2   */  be_nested_str_weak(get_max_value),
    }),
    be_str_weak(set_min),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x60100009,  //  0002  GETGBL	R4	G9
      0x5C140200,  //  0003  MOVE	R5	R1
      0x7C100200,  //  0004  CALL	R4	1
      0x88140100,  //  0005  GETMBR	R5	R0	K0
      0x8C140B02,  //  0006  GETMET	R5	R5	K2
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
extern const bclass be_class_lvh_bar;
be_local_closure(class_lvh_bar_set_val,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_bar, 
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(_val),
    /* K1   */  be_nested_str_weak(_lv_obj),
    /* K2   */  be_nested_str_weak(set_value),
    /* K3   */  be_nested_str_weak(lv),
    /* K4   */  be_nested_str_weak(ANIM_OFF),
    }),
    be_str_weak(set_val),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x88080101,  //  0001  GETMBR	R2	R0	K1
      0x8C080502,  //  0002  GETMET	R2	R2	K2
      0x5C100200,  //  0003  MOVE	R4	R1
      0xB8160600,  //  0004  GETNGBL	R5	K3
      0x88140B04,  //  0005  GETMBR	R5	R5	K4
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
extern const bclass be_class_lvh_line;
be_local_closure(class_lvh_line_set_points,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_line, 
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
    /* K12  */  be_nested_str_weak(set_points),
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

extern const bclass be_class_lvh_scale;

/********************************************************************
** Solidified function: set_text_src
********************************************************************/
extern const bclass be_class_lvh_scale;
be_local_closure(class_lvh_scale_set_text_src,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_scale, 
    1,                          /* has constants */
    ( &(const bvalue[10]) {     /* constants */
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
    }),
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
extern const bclass be_class_lvh_scale;
be_local_closure(class_lvh_scale_get_max,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_scale, 
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(_lv_obj),
    /* K1   */  be_nested_str_weak(get_range_max_value),
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
** Solidified function: set_min
********************************************************************/
extern const bclass be_class_lvh_scale;
be_local_closure(class_lvh_scale_set_min,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_scale, 
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(_lv_obj),
    /* K1   */  be_nested_str_weak(set_range),
    /* K2   */  be_nested_str_weak(get_range_max_value),
    }),
    be_str_weak(set_min),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x60100009,  //  0002  GETGBL	R4	G9
      0x5C140200,  //  0003  MOVE	R5	R1
      0x7C100200,  //  0004  CALL	R4	1
      0x88140100,  //  0005  GETMBR	R5	R0	K0
      0x8C140B02,  //  0006  GETMET	R5	R5	K2
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
extern const bclass be_class_lvh_scale;
be_local_closure(class_lvh_scale_get_text_src,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_scale, 
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(_options),
    }),
    be_str_weak(get_text_src),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_max
********************************************************************/
extern const bclass be_class_lvh_scale;
be_local_closure(class_lvh_scale_set_max,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_scale, 
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(_lv_obj),
    /* K1   */  be_nested_str_weak(set_range),
    /* K2   */  be_nested_str_weak(get_range_min_value),
    }),
    be_str_weak(set_max),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x88100100,  //  0002  GETMBR	R4	R0	K0
      0x8C100902,  //  0003  GETMET	R4	R4	K2
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
extern const bclass be_class_lvh_scale;
be_local_closure(class_lvh_scale_get_min,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_scale, 
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(_lv_obj),
    /* K1   */  be_nested_str_weak(get_range_min_value),
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

extern const bclass be_class_lvh_scale_section;

/********************************************************************
** Solidified function: setmember
********************************************************************/
extern const bclass be_class_lvh_scale_section;
be_local_closure(class_lvh_scale_section_setmember,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_scale_section, 
    1,                          /* has constants */
    ( &(const bvalue[26]) {     /* constants */
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
    /* K10  */  be_const_int(3),
    /* K11  */  be_nested_str_weak(byte),
    /* K12  */  be_const_int(2147483647),
    /* K13  */  be_nested_str_weak(value_error),
    /* K14  */  be_nested_str_weak(only_X20modifiers_X20_X2710_X27_X20or_X20_X2730_X27_X20allowed),
    /* K15  */  be_nested_str_weak(_attr_ignore),
    /* K16  */  be_nested_str_weak(find),
    /* K17  */  be_nested_str_weak(_style),
    /* K18  */  be_nested_str_weak(_style10),
    /* K19  */  be_nested_str_weak(_style30),
    /* K20  */  be_nested_str_weak(get),
    /* K21  */  be_nested_str_weak(function),
    /* K22  */  be_nested_str_weak(is_color_attribute),
    /* K23  */  be_nested_str_weak(parse_color),
    /* K24  */  be_nested_str_weak(_X20for_X20),
    /* K25  */  be_nested_str_weak(HSP_X3A_X20Could_X20not_X20find_X20function_X20set_),
    }),
    be_str_weak(setmember),
    &be_const_str_solidified,
    ( &(const binstruction[141]) {  /* code */
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
      0x4C140000,  //  001C  LDNIL	R5
      0x6018000C,  //  001D  GETGBL	R6	G12
      0x5C1C0200,  //  001E  MOVE	R7	R1
      0x7C180200,  //  001F  CALL	R6	1
      0x28180D0A,  //  0020  GE	R6	R6	K10
      0x781A0023,  //  0021  JMPF	R6	#0046
      0x8C18070B,  //  0022  GETMET	R6	R3	K11
      0x5421FFFE,  //  0023  LDINT	R8	-1
      0x94200208,  //  0024  GETIDX	R8	R1	R8
      0x7C180400,  //  0025  CALL	R6	2
      0x8C1C070B,  //  0026  GETMET	R7	R3	K11
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
      0x4024130C,  //  0038  CONNECT	R9	R9	K12
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
      0xB0061B0E,  //  0045  RAISE	1	K13	K14
      0x8818010F,  //  0046  GETMBR	R6	R0	K15
      0x8C180D10,  //  0047  GETMET	R6	R6	K16
      0x5C200200,  //  0048  MOVE	R8	R1
      0x7C180400,  //  0049  CALL	R6	2
      0x4C1C0000,  //  004A  LDNIL	R7
      0x20180C07,  //  004B  NE	R6	R6	R7
      0x781A0000,  //  004C  JMPF	R6	#004E
      0x80000C00,  //  004D  RET	0
      0x88180111,  //  004E  GETMBR	R6	R0	K17
      0x541E0009,  //  004F  LDINT	R7	10
      0x1C1C0A07,  //  0050  EQ	R7	R5	R7
      0x781E0001,  //  0051  JMPF	R7	#0054
      0x88180112,  //  0052  GETMBR	R6	R0	K18
      0x70020003,  //  0053  JMP		#0058
      0x541E001D,  //  0054  LDINT	R7	30
      0x1C1C0A07,  //  0055  EQ	R7	R5	R7
      0x781E0000,  //  0056  JMPF	R7	#0058
      0x88180113,  //  0057  GETMBR	R6	R0	K19
      0x8C1C0914,  //  0058  GETMET	R7	R4	K20
      0x5C240000,  //  0059  MOVE	R9	R0
      0x002A0601,  //  005A  ADD	R10	K3	R1
      0x7C1C0600,  //  005B  CALL	R7	3
      0x60200004,  //  005C  GETGBL	R8	G4
      0x5C240E00,  //  005D  MOVE	R9	R7
      0x7C200200,  //  005E  CALL	R8	1
      0x1C201115,  //  005F  EQ	R8	R8	K21
      0x78220004,  //  0060  JMPF	R8	#0066
      0x5C200E00,  //  0061  MOVE	R8	R7
      0x5C240000,  //  0062  MOVE	R9	R0
      0x5C280400,  //  0063  MOVE	R10	R2
      0x7C200400,  //  0064  CALL	R8	2
      0x80001000,  //  0065  RET	0
      0x8C200914,  //  0066  GETMET	R8	R4	K20
      0x5C280C00,  //  0067  MOVE	R10	R6
      0x002E0601,  //  0068  ADD	R11	K3	R1
      0x7C200600,  //  0069  CALL	R8	3
      0x5C1C1000,  //  006A  MOVE	R7	R8
      0x60200004,  //  006B  GETGBL	R8	G4
      0x5C240E00,  //  006C  MOVE	R9	R7
      0x7C200200,  //  006D  CALL	R8	1
      0x1C201115,  //  006E  EQ	R8	R8	K21
      0x78220018,  //  006F  JMPF	R8	#0089
      0x8C200116,  //  0070  GETMET	R8	R0	K22
      0x5C280200,  //  0071  MOVE	R10	R1
      0x7C200400,  //  0072  CALL	R8	2
      0x78220003,  //  0073  JMPF	R8	#0078
      0x8C200117,  //  0074  GETMET	R8	R0	K23
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
      0x00281318,  //  0081  ADD	R10	R9	K24
      0x00281401,  //  0082  ADD	R10	R10	R1
      0xB004100A,  //  0083  RAISE	1	R8	R10
      0x70020000,  //  0084  JMP		#0086
      0xB0080000,  //  0085  RAISE	2	R0	R0
      0x4C200000,  //  0086  LDNIL	R8
      0x80041000,  //  0087  RET	1	R8
      0x70020002,  //  0088  JMP		#008C
      0x60200001,  //  0089  GETGBL	R8	G1
      0x00263201,  //  008A  ADD	R9	K25	R1
      0x7C200200,  //  008B  CALL	R8	1
      0x80000000,  //  008C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: post_init
********************************************************************/
extern const bclass be_class_lvh_scale_section;
be_local_closure(class_lvh_scale_section_post_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_scale_section, 
    1,                          /* has constants */
    ( &(const bvalue[19]) {     /* constants */
    /* K0   */  be_nested_str_weak(_lv_obj),
    /* K1   */  be_nested_str_weak(_min),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str_weak(_max),
    /* K4   */  be_nested_str_weak(_parent_lvh),
    /* K5   */  be_nested_str_weak(_page),
    /* K6   */  be_nested_str_weak(_oh),
    /* K7   */  be_nested_str_weak(lvh_scale),
    /* K8   */  be_nested_str_weak(add_section),
    /* K9   */  be_nested_str_weak(_style),
    /* K10  */  be_nested_str_weak(lv),
    /* K11  */  be_nested_str_weak(style),
    /* K12  */  be_nested_str_weak(set_style),
    /* K13  */  be_nested_str_weak(PART_MAIN),
    /* K14  */  be_nested_str_weak(_style10),
    /* K15  */  be_nested_str_weak(PART_INDICATOR),
    /* K16  */  be_nested_str_weak(_style30),
    /* K17  */  be_nested_str_weak(PART_ITEMS),
    /* K18  */  be_nested_str_weak(HSP_X3A_X20_X27scale_section_X27_X20should_X20have_X20a_X20parent_X20of_X20type_X20_X27scale_X27),
    }),
    be_str_weak(post_init),
    &be_const_str_solidified,
    ( &(const binstruction[51]) {  /* code */
      0x4C040000,  //  0000  LDNIL	R1
      0x90020001,  //  0001  SETMBR	R0	K0	R1
      0x90020302,  //  0002  SETMBR	R0	K1	K2
      0x90020702,  //  0003  SETMBR	R0	K3	K2
      0x6004000F,  //  0004  GETGBL	R1	G15
      0x88080104,  //  0005  GETMBR	R2	R0	K4
      0x880C0105,  //  0006  GETMBR	R3	R0	K5
      0x880C0706,  //  0007  GETMBR	R3	R3	K6
      0x880C0707,  //  0008  GETMBR	R3	R3	K7
      0x7C040400,  //  0009  CALL	R1	2
      0x78060023,  //  000A  JMPF	R1	#002F
      0x88040104,  //  000B  GETMBR	R1	R0	K4
      0x88040300,  //  000C  GETMBR	R1	R1	K0
      0x8C040308,  //  000D  GETMET	R1	R1	K8
      0x7C040200,  //  000E  CALL	R1	1
      0x90020001,  //  000F  SETMBR	R0	K0	R1
      0xB8061400,  //  0010  GETNGBL	R1	K10
      0x8C04030B,  //  0011  GETMET	R1	R1	K11
      0x7C040200,  //  0012  CALL	R1	1
      0x90021201,  //  0013  SETMBR	R0	K9	R1
      0x88040100,  //  0014  GETMBR	R1	R0	K0
      0x8C04030C,  //  0015  GETMET	R1	R1	K12
      0xB80E1400,  //  0016  GETNGBL	R3	K10
      0x880C070D,  //  0017  GETMBR	R3	R3	K13
      0x88100109,  //  0018  GETMBR	R4	R0	K9
      0x7C040600,  //  0019  CALL	R1	3
      0xB8061400,  //  001A  GETNGBL	R1	K10
      0x8C04030B,  //  001B  GETMET	R1	R1	K11
      0x7C040200,  //  001C  CALL	R1	1
      0x90021C01,  //  001D  SETMBR	R0	K14	R1
      0x88040100,  //  001E  GETMBR	R1	R0	K0
      0x8C04030C,  //  001F  GETMET	R1	R1	K12
      0xB80E1400,  //  0020  GETNGBL	R3	K10
      0x880C070F,  //  0021  GETMBR	R3	R3	K15
      0x8810010E,  //  0022  GETMBR	R4	R0	K14
      0x7C040600,  //  0023  CALL	R1	3
      0xB8061400,  //  0024  GETNGBL	R1	K10
      0x8C04030B,  //  0025  GETMET	R1	R1	K11
      0x7C040200,  //  0026  CALL	R1	1
      0x90022001,  //  0027  SETMBR	R0	K16	R1
      0x88040100,  //  0028  GETMBR	R1	R0	K0
      0x8C04030C,  //  0029  GETMET	R1	R1	K12
      0xB80E1400,  //  002A  GETNGBL	R3	K10
      0x880C0711,  //  002B  GETMBR	R3	R3	K17
      0x88100110,  //  002C  GETMBR	R4	R0	K16
      0x7C040600,  //  002D  CALL	R1	3
      0x70020002,  //  002E  JMP		#0032
      0x60040001,  //  002F  GETGBL	R1	G1
      0x58080012,  //  0030  LDCONST	R2	K18
      0x7C040200,  //  0031  CALL	R1	1
      0x80000000,  //  0032  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_min
********************************************************************/
extern const bclass be_class_lvh_scale_section;
be_local_closure(class_lvh_scale_section_set_min,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_scale_section, 
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(_max),
    /* K1   */  be_nested_str_weak(set_range),
    }),
    be_str_weak(set_min),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x60080009,  //  0000  GETGBL	R2	G9
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x7C080200,  //  0002  CALL	R2	1
      0x880C0100,  //  0003  GETMBR	R3	R0	K0
      0x14100602,  //  0004  LT	R4	R3	R2
      0x78120000,  //  0005  JMPF	R4	#0007
      0x5C0C0400,  //  0006  MOVE	R3	R2
      0x8C100101,  //  0007  GETMET	R4	R0	K1
      0x5C180400,  //  0008  MOVE	R6	R2
      0x5C1C0600,  //  0009  MOVE	R7	R3
      0x7C100600,  //  000A  CALL	R4	3
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_max
********************************************************************/
extern const bclass be_class_lvh_scale_section;
be_local_closure(class_lvh_scale_section_set_max,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_scale_section, 
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(_min),
    /* K1   */  be_nested_str_weak(set_range),
    }),
    be_str_weak(set_max),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x600C0009,  //  0001  GETGBL	R3	G9
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C0C0200,  //  0003  CALL	R3	1
      0x24100403,  //  0004  GT	R4	R2	R3
      0x78120000,  //  0005  JMPF	R4	#0007
      0x5C080600,  //  0006  MOVE	R2	R3
      0x8C100101,  //  0007  GETMET	R4	R0	K1
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
extern const bclass be_class_lvh_scale_section;
be_local_closure(class_lvh_scale_section_set_range,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_scale_section, 
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(_min),
    /* K1   */  be_nested_str_weak(_max),
    /* K2   */  be_nested_str_weak(_lv_obj),
    /* K3   */  be_nested_str_weak(set_range),
    }),
    be_str_weak(set_range),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x90020202,  //  0001  SETMBR	R0	K1	R2
      0x880C0102,  //  0002  GETMBR	R3	R0	K2
      0x8C0C0703,  //  0003  GETMET	R3	R3	K3
      0x5C140200,  //  0004  MOVE	R5	R1
      0x5C180400,  //  0005  MOVE	R6	R2
      0x7C0C0600,  //  0006  CALL	R3	3
      0x80000000,  //  0007  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: delete
********************************************************************/
extern const bclass be_class_lvh_scale_section;
be_local_closure(class_lvh_scale_section_delete,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_scale_section, 
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(_style),
    /* K1   */  be_nested_str_weak(del),
    /* K2   */  be_nested_str_weak(_style10),
    /* K3   */  be_nested_str_weak(_style30),
    }),
    be_str_weak(delete),
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
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
      0x80000000,  //  000F  RET	0
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
        { be_const_key_weak(setmember, -1), be_const_closure(class_lvh_scale_section_setmember_closure) },
        { be_const_key_weak(post_init, 3), be_const_closure(class_lvh_scale_section_post_init_closure) },
        { be_const_key_weak(_style10, -1), be_const_var(1) },
        { be_const_key_weak(_max, 11), be_const_var(4) },
        { be_const_key_weak(delete, -1), be_const_closure(class_lvh_scale_section_delete_closure) },
        { be_const_key_weak(_min, -1), be_const_var(3) },
        { be_const_key_weak(_style, 8), be_const_var(0) },
        { be_const_key_weak(_lv_class, -1), be_const_nil() },
        { be_const_key_weak(set_range, -1), be_const_closure(class_lvh_scale_section_set_range_closure) },
        { be_const_key_weak(set_max, 5), be_const_closure(class_lvh_scale_section_set_max_closure) },
        { be_const_key_weak(set_min, -1), be_const_closure(class_lvh_scale_section_set_min_closure) },
    })),
    be_str_weak(lvh_scale_section)
);

extern const bclass be_class_lvh_scale_line;

/********************************************************************
** Solidified function: set_val
********************************************************************/
extern const bclass be_class_lvh_scale_line;
be_local_closure(class_lvh_scale_line_set_val,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_scale_line, 
    1,                          /* has constants */
    ( &(const bvalue[13]) {     /* constants */
    /* K0   */  be_nested_str_weak(set_val),
    /* K1   */  be_nested_str_weak(_parent_lvh),
    /* K2   */  be_nested_str_weak(_lv_obj),
    /* K3   */  be_nested_str_weak(set_line_needle_value),
    /* K4   */  be_nested_str_weak(_needle_length),
    /* K5   */  be_nested_str_weak(_val),
    /* K6   */  be_nested_str_weak(get_points_num),
    /* K7   */  be_const_int(2),
    /* K8   */  be_nested_str_weak(get_points),
    /* K9   */  be_nested_str_weak(_lv_points),
    /* K10  */  be_nested_str_weak(setbytes),
    /* K11  */  be_const_int(0),
    /* K12  */  be_nested_str_weak(set_points),
    }),
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
extern const bclass be_class_lvh_scale_line;
be_local_closure(class_lvh_scale_line_post_init,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_scale_line, 
    1,                          /* has constants */
    ( &(const bvalue[12]) {     /* constants */
    /* K0   */  be_nested_str_weak(_parent_lvh),
    /* K1   */  be_nested_str_weak(_page),
    /* K2   */  be_nested_str_weak(_oh),
    /* K3   */  be_nested_str_weak(lvh_scale),
    /* K4   */  be_nested_str_weak(HSP_X3A_X20_X27scale_line_X27_X20should_X20have_X20a_X20parent_X20of_X20type_X20_X27scale_X27),
    /* K5   */  be_nested_str_weak(_needle_length),
    /* K6   */  be_const_int(0),
    /* K7   */  be_nested_str_weak(_lv_points),
    /* K8   */  be_nested_str_weak(lv),
    /* K9   */  be_nested_str_weak(point_arr),
    /* K10  */  be_nested_str_weak(point),
    /* K11  */  be_nested_str_weak(post_init),
    }),
    be_str_weak(post_init),
    &be_const_str_solidified,
    ( &(const binstruction[31]) {  /* code */
      0x6004000F,  //  0000  GETGBL	R1	G15
      0x88080100,  //  0001  GETMBR	R2	R0	K0
      0x880C0101,  //  0002  GETMBR	R3	R0	K1
      0x880C0702,  //  0003  GETMBR	R3	R3	K2
      0x880C0703,  //  0004  GETMBR	R3	R3	K3
      0x7C040400,  //  0005  CALL	R1	2
      0x74060002,  //  0006  JMPT	R1	#000A
      0x60040001,  //  0007  GETGBL	R1	G1
      0x58080004,  //  0008  LDCONST	R2	K4
      0x7C040200,  //  0009  CALL	R1	1
      0x90020B06,  //  000A  SETMBR	R0	K5	K6
      0xB8061000,  //  000B  GETNGBL	R1	K8
      0x8C040309,  //  000C  GETMET	R1	R1	K9
      0x600C0012,  //  000D  GETGBL	R3	G18
      0x7C0C0000,  //  000E  CALL	R3	0
      0xB8121000,  //  000F  GETNGBL	R4	K8
      0x8C10090A,  //  0010  GETMET	R4	R4	K10
      0x7C100200,  //  0011  CALL	R4	1
      0x40100604,  //  0012  CONNECT	R4	R3	R4
      0xB8121000,  //  0013  GETNGBL	R4	K8
      0x8C10090A,  //  0014  GETMET	R4	R4	K10
      0x7C100200,  //  0015  CALL	R4	1
      0x40100604,  //  0016  CONNECT	R4	R3	R4
      0x7C040400,  //  0017  CALL	R1	2
      0x90020E01,  //  0018  SETMBR	R0	K7	R1
      0x60040003,  //  0019  GETGBL	R1	G3
      0x5C080000,  //  001A  MOVE	R2	R0
      0x7C040200,  //  001B  CALL	R1	1
      0x8C04030B,  //  001C  GETMET	R1	R1	K11
      0x7C040200,  //  001D  CALL	R1	1
      0x80000000,  //  001E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_needle_length
********************************************************************/
extern const bclass be_class_lvh_scale_line;
be_local_closure(class_lvh_scale_line_get_needle_length,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_scale_line, 
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(_needle_length),
    }),
    be_str_weak(get_needle_length),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_needle_length
********************************************************************/
extern const bclass be_class_lvh_scale_line;
be_local_closure(class_lvh_scale_line_set_needle_length,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_scale_line, 
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(_needle_length),
    /* K1   */  be_nested_str_weak(_val),
    /* K2   */  be_nested_str_weak(set_val),
    }),
    be_str_weak(set_needle_length),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x60080009,  //  0000  GETGBL	R2	G9
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x7C080200,  //  0002  CALL	R2	1
      0x90020002,  //  0003  SETMBR	R0	K0	R2
      0x88080101,  //  0004  GETMBR	R2	R0	K1
      0x4C0C0000,  //  0005  LDNIL	R3
      0x20080403,  //  0006  NE	R2	R2	R3
      0x780A0002,  //  0007  JMPF	R2	#000B
      0x8C080102,  //  0008  GETMET	R2	R0	K2
      0x88100101,  //  0009  GETMBR	R4	R0	K1
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

extern const bclass be_class_lvh_spangroup;

/********************************************************************
** Solidified function: post_init
********************************************************************/
extern const bclass be_class_lvh_spangroup;
be_local_closure(class_lvh_spangroup_post_init,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_spangroup, 
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(_lv_obj),
    /* K1   */  be_nested_str_weak(set_mode),
    /* K2   */  be_nested_str_weak(lv),
    /* K3   */  be_nested_str_weak(SPAN_MODE_BREAK),
    /* K4   */  be_nested_str_weak(refr_mode),
    /* K5   */  be_nested_str_weak(post_init),
    }),
    be_str_weak(post_init),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0xB80E0400,  //  0002  GETNGBL	R3	K2
      0x880C0703,  //  0003  GETMBR	R3	R3	K3
      0x7C040400,  //  0004  CALL	R1	2
      0x88040100,  //  0005  GETMBR	R1	R0	K0
      0x8C040304,  //  0006  GETMET	R1	R1	K4
      0x7C040200,  //  0007  CALL	R1	1
      0x60040003,  //  0008  GETGBL	R1	G3
      0x5C080000,  //  0009  MOVE	R2	R0
      0x7C040200,  //  000A  CALL	R1	1
      0x8C040305,  //  000B  GETMET	R1	R1	K5
      0x7C040200,  //  000C  CALL	R1	1
      0x80000000,  //  000D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: refr_mode
********************************************************************/
extern const bclass be_class_lvh_spangroup;
be_local_closure(class_lvh_spangroup_refr_mode,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_spangroup, 
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(_lv_obj),
    /* K1   */  be_nested_str_weak(refr_mode),
    }),
    be_str_weak(refr_mode),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
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
        { be_const_key_weak(refr_mode, -1), be_const_closure(class_lvh_spangroup_refr_mode_closure) },
        { be_const_key_weak(_lv_class, -1), be_const_class(be_class_lv_spangroup) },
        { be_const_key_weak(post_init, 0), be_const_closure(class_lvh_spangroup_post_init_closure) },
    })),
    be_str_weak(lvh_spangroup)
);

extern const bclass be_class_lvh_span;

/********************************************************************
** Solidified function: setmember
********************************************************************/
extern const bclass be_class_lvh_span;
be_local_closure(class_lvh_span_setmember,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_span, 
    1,                          /* has constants */
    ( &(const bvalue[21]) {     /* constants */
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
    /* K18  */  be_nested_str_weak(refr_mode),
    /* K19  */  be_nested_str_weak(_X20for_X20),
    /* K20  */  be_nested_str_weak(HSP_X3A_X20Could_X20not_X20find_X20function_X20set_),
    }),
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
extern const bclass be_class_lvh_span;
be_local_closure(class_lvh_span_post_init,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_span, 
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(_lv_obj),
    /* K1   */  be_nested_str_weak(_parent_lvh),
    /* K2   */  be_nested_str_weak(_page),
    /* K3   */  be_nested_str_weak(_oh),
    /* K4   */  be_nested_str_weak(lvh_spangroup),
    /* K5   */  be_nested_str_weak(new_span),
    /* K6   */  be_nested_str_weak(_style),
    /* K7   */  be_nested_str_weak(get_style),
    }),
    be_str_weak(post_init),
    &be_const_str_solidified,
    ( &(const binstruction[19]) {  /* code */
      0x4C040000,  //  0000  LDNIL	R1
      0x90020001,  //  0001  SETMBR	R0	K0	R1
      0x6004000F,  //  0002  GETGBL	R1	G15
      0x88080101,  //  0003  GETMBR	R2	R0	K1
      0x880C0102,  //  0004  GETMBR	R3	R0	K2
      0x880C0703,  //  0005  GETMBR	R3	R3	K3
      0x880C0704,  //  0006  GETMBR	R3	R3	K4
      0x7C040400,  //  0007  CALL	R1	2
      0x78060008,  //  0008  JMPF	R1	#0012
      0x88040101,  //  0009  GETMBR	R1	R0	K1
      0x88040300,  //  000A  GETMBR	R1	R1	K0
      0x8C040305,  //  000B  GETMET	R1	R1	K5
      0x7C040200,  //  000C  CALL	R1	1
      0x90020001,  //  000D  SETMBR	R0	K0	R1
      0x88040100,  //  000E  GETMBR	R1	R0	K0
      0x8C040307,  //  000F  GETMET	R1	R1	K7
      0x7C040200,  //  0010  CALL	R1	1
      0x90020C01,  //  0011  SETMBR	R0	K6	R1
      0x80000000,  //  0012  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_text_font
********************************************************************/
extern const bclass be_class_lvh_span;
be_local_closure(class_lvh_span_set_text_font,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_span, 
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(parse_font),
    /* K1   */  be_nested_str_weak(_style),
    /* K2   */  be_nested_str_weak(set_text_font),
    /* K3   */  be_nested_str_weak(_parent_lvh),
    /* K4   */  be_nested_str_weak(refr_mode),
    }),
    be_str_weak(set_text_font),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x5C100200,  //  0001  MOVE	R4	R1
      0x7C080400,  //  0002  CALL	R2	2
      0x4C0C0000,  //  0003  LDNIL	R3
      0x200C0403,  //  0004  NE	R3	R2	R3
      0x780E0006,  //  0005  JMPF	R3	#000D
      0x880C0101,  //  0006  GETMBR	R3	R0	K1
      0x8C0C0702,  //  0007  GETMET	R3	R3	K2
      0x5C140400,  //  0008  MOVE	R5	R2
      0x7C0C0400,  //  0009  CALL	R3	2
      0x880C0103,  //  000A  GETMBR	R3	R0	K3
      0x8C0C0704,  //  000B  GETMET	R3	R3	K4
      0x7C0C0200,  //  000C  CALL	R3	1
      0x80000000,  //  000D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_text
********************************************************************/
extern const bclass be_class_lvh_span;
be_local_closure(class_lvh_span_set_text,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_span, 
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(_lv_obj),
    /* K1   */  be_nested_str_weak(set_text),
    }),
    be_str_weak(set_text),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
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

extern const bclass be_class_lvh_chart;

/********************************************************************
** Solidified function: get_y_min
********************************************************************/
extern const bclass be_class_lvh_chart;
be_local_closure(class_lvh_chart_get_y_min,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_chart, 
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(_y_min),
    }),
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
** Solidified function: add_point
********************************************************************/
extern const bclass be_class_lvh_chart;
be_local_closure(class_lvh_chart_add_point,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_chart, 
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(_lv_obj),
    /* K1   */  be_nested_str_weak(set_next_value),
    /* K2   */  be_nested_str_weak(_ser1),
    }),
    be_str_weak(add_point),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x88100102,  //  0002  GETMBR	R4	R0	K2
      0x5C140200,  //  0003  MOVE	R5	R1
      0x7C080600,  //  0004  CALL	R2	3
      0x80000000,  //  0005  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_val2
********************************************************************/
extern const bclass be_class_lvh_chart;
be_local_closure(class_lvh_chart_set_val2,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_chart, 
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(add_point2),
    }),
    be_str_weak(set_val2),
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
** Solidified function: set_y_min
********************************************************************/
extern const bclass be_class_lvh_chart;
be_local_closure(class_lvh_chart_set_y_min,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_chart, 
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(_y_min),
    /* K1   */  be_nested_str_weak(_lv_obj),
    /* K2   */  be_nested_str_weak(set_range),
    /* K3   */  be_nested_str_weak(lv),
    /* K4   */  be_nested_str_weak(CHART_AXIS_PRIMARY_Y),
    /* K5   */  be_nested_str_weak(_y_max),
    }),
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
extern const bclass be_class_lvh_chart;
be_local_closure(class_lvh_chart_add_point2,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_chart, 
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(_lv_obj),
    /* K1   */  be_nested_str_weak(set_next_value),
    /* K2   */  be_nested_str_weak(_ser2),
    }),
    be_str_weak(add_point2),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x88100102,  //  0002  GETMBR	R4	R0	K2
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
extern const bclass be_class_lvh_chart;
be_local_closure(class_lvh_chart_set_series1_color,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_chart, 
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(_lv_obj),
    /* K1   */  be_nested_str_weak(set_series_color),
    /* K2   */  be_nested_str_weak(_ser1),
    }),
    be_str_weak(set_series1_color),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x88100102,  //  0002  GETMBR	R4	R0	K2
      0x5C140200,  //  0003  MOVE	R5	R1
      0x7C080600,  //  0004  CALL	R2	3
      0x80000000,  //  0005  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_series2_color
********************************************************************/
extern const bclass be_class_lvh_chart;
be_local_closure(class_lvh_chart_set_series2_color,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_chart, 
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(_lv_obj),
    /* K1   */  be_nested_str_weak(set_series_color),
    /* K2   */  be_nested_str_weak(_ser2),
    }),
    be_str_weak(set_series2_color),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x88100102,  //  0002  GETMBR	R4	R0	K2
      0x5C140200,  //  0003  MOVE	R5	R1
      0x7C080600,  //  0004  CALL	R2	3
      0x80000000,  //  0005  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_y_max
********************************************************************/
extern const bclass be_class_lvh_chart;
be_local_closure(class_lvh_chart_set_y_max,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_chart, 
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(_y_max),
    /* K1   */  be_nested_str_weak(_lv_obj),
    /* K2   */  be_nested_str_weak(set_range),
    /* K3   */  be_nested_str_weak(lv),
    /* K4   */  be_nested_str_weak(CHART_AXIS_PRIMARY_Y),
    /* K5   */  be_nested_str_weak(_y_min),
    }),
    be_str_weak(set_y_max),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x88080101,  //  0001  GETMBR	R2	R0	K1
      0x8C080502,  //  0002  GETMET	R2	R2	K2
      0xB8120600,  //  0003  GETNGBL	R4	K3
      0x88100904,  //  0004  GETMBR	R4	R4	K4
      0x88140105,  //  0005  GETMBR	R5	R0	K5
      0x88180100,  //  0006  GETMBR	R6	R0	K0
      0x7C080800,  //  0007  CALL	R2	4
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: post_init
********************************************************************/
extern const bclass be_class_lvh_chart;
be_local_closure(class_lvh_chart_post_init,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_chart, 
    1,                          /* has constants */
    ( &(const bvalue[17]) {     /* constants */
    /* K0   */  be_nested_str_weak(_y_min),
    /* K1   */  be_const_int(0),
    /* K2   */  be_nested_str_weak(_y_max),
    /* K3   */  be_nested_str_weak(_h_div),
    /* K4   */  be_const_int(3),
    /* K5   */  be_nested_str_weak(_v_div),
    /* K6   */  be_nested_str_weak(_lv_obj),
    /* K7   */  be_nested_str_weak(set_update_mode),
    /* K8   */  be_nested_str_weak(lv),
    /* K9   */  be_nested_str_weak(CHART_UPDATE_MODE_SHIFT),
    /* K10  */  be_nested_str_weak(_ser1),
    /* K11  */  be_nested_str_weak(add_series),
    /* K12  */  be_nested_str_weak(color),
    /* K13  */  be_const_int(15615044),
    /* K14  */  be_nested_str_weak(CHART_AXIS_PRIMARY_Y),
    /* K15  */  be_nested_str_weak(_ser2),
    /* K16  */  be_const_int(4517444),
    }),
    be_str_weak(post_init),
    &be_const_str_solidified,
    ( &(const binstruction[32]) {  /* code */
      0x90020101,  //  0000  SETMBR	R0	K0	K1
      0x54060063,  //  0001  LDINT	R1	100
      0x90020401,  //  0002  SETMBR	R0	K2	R1
      0x90020704,  //  0003  SETMBR	R0	K3	K4
      0x54060004,  //  0004  LDINT	R1	5
      0x90020A01,  //  0005  SETMBR	R0	K5	R1
      0x88040106,  //  0006  GETMBR	R1	R0	K6
      0x8C040307,  //  0007  GETMET	R1	R1	K7
      0xB80E1000,  //  0008  GETNGBL	R3	K8
      0x880C0709,  //  0009  GETMBR	R3	R3	K9
      0x7C040400,  //  000A  CALL	R1	2
      0x88040106,  //  000B  GETMBR	R1	R0	K6
      0x8C04030B,  //  000C  GETMET	R1	R1	K11
      0xB80E1000,  //  000D  GETNGBL	R3	K8
      0x8C0C070C,  //  000E  GETMET	R3	R3	K12
      0x5814000D,  //  000F  LDCONST	R5	K13
      0x7C0C0400,  //  0010  CALL	R3	2
      0xB8121000,  //  0011  GETNGBL	R4	K8
      0x8810090E,  //  0012  GETMBR	R4	R4	K14
      0x7C040600,  //  0013  CALL	R1	3
      0x90021401,  //  0014  SETMBR	R0	K10	R1
      0x88040106,  //  0015  GETMBR	R1	R0	K6
      0x8C04030B,  //  0016  GETMET	R1	R1	K11
      0xB80E1000,  //  0017  GETNGBL	R3	K8
      0x8C0C070C,  //  0018  GETMET	R3	R3	K12
      0x58140010,  //  0019  LDCONST	R5	K16
      0x7C0C0400,  //  001A  CALL	R3	2
      0xB8121000,  //  001B  GETNGBL	R4	K8
      0x8810090E,  //  001C  GETMBR	R4	R4	K14
      0x7C040600,  //  001D  CALL	R1	3
      0x90021E01,  //  001E  SETMBR	R0	K15	R1
      0x80000000,  //  001F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_y_max
********************************************************************/
extern const bclass be_class_lvh_chart;
be_local_closure(class_lvh_chart_get_y_max,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_chart, 
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(_y_max),
    }),
    be_str_weak(get_y_max),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_val
********************************************************************/
extern const bclass be_class_lvh_chart;
be_local_closure(class_lvh_chart_set_val,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_chart, 
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(_val),
    /* K1   */  be_nested_str_weak(add_point),
    }),
    be_str_weak(set_val),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x8C080101,  //  0001  GETMET	R2	R0	K1
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_v_div_line_count
********************************************************************/
extern const bclass be_class_lvh_chart;
be_local_closure(class_lvh_chart_set_v_div_line_count,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_chart, 
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(_v_div),
    /* K1   */  be_nested_str_weak(_lv_obj),
    /* K2   */  be_nested_str_weak(set_div_line_count),
    /* K3   */  be_nested_str_weak(_h_div),
    }),
    be_str_weak(set_v_div_line_count),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x88080101,  //  0001  GETMBR	R2	R0	K1
      0x8C080502,  //  0002  GETMET	R2	R2	K2
      0x88100103,  //  0003  GETMBR	R4	R0	K3
      0x88140100,  //  0004  GETMBR	R5	R0	K0
      0x7C080600,  //  0005  CALL	R2	3
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_h_div_line_count
********************************************************************/
extern const bclass be_class_lvh_chart;
be_local_closure(class_lvh_chart_set_h_div_line_count,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_chart, 
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(_h_div),
    /* K1   */  be_nested_str_weak(_lv_obj),
    /* K2   */  be_nested_str_weak(set_div_line_count),
    /* K3   */  be_nested_str_weak(_v_div),
    }),
    be_str_weak(set_h_div_line_count),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x88080101,  //  0001  GETMBR	R2	R0	K1
      0x8C080502,  //  0002  GETMET	R2	R2	K2
      0x88100100,  //  0003  GETMBR	R4	R0	K0
      0x88140103,  //  0004  GETMBR	R5	R0	K3
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
    6,
    &be_class_lvh_obj,
    be_nested_map(20,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(get_y_min, -1), be_const_closure(class_lvh_chart_get_y_min_closure) },
        { be_const_key_weak(_h_div, 16), be_const_var(4) },
        { be_const_key_weak(set_v_div_line_count, -1), be_const_closure(class_lvh_chart_set_v_div_line_count_closure) },
        { be_const_key_weak(set_val2, -1), be_const_closure(class_lvh_chart_set_val2_closure) },
        { be_const_key_weak(set_y_min, -1), be_const_closure(class_lvh_chart_set_y_min_closure) },
        { be_const_key_weak(add_point2, -1), be_const_closure(class_lvh_chart_add_point2_closure) },
        { be_const_key_weak(_ser2, -1), be_const_var(1) },
        { be_const_key_weak(_ser1, 11), be_const_var(0) },
        { be_const_key_weak(_v_div, 18), be_const_var(5) },
        { be_const_key_weak(set_series2_color, -1), be_const_closure(class_lvh_chart_set_series2_color_closure) },
        { be_const_key_weak(set_y_max, 1), be_const_closure(class_lvh_chart_set_y_max_closure) },
        { be_const_key_weak(_lv_class, -1), be_const_class(be_class_lv_chart) },
        { be_const_key_weak(_y_max, -1), be_const_var(3) },
        { be_const_key_weak(get_y_max, -1), be_const_closure(class_lvh_chart_get_y_max_closure) },
        { be_const_key_weak(_y_min, 13), be_const_var(2) },
        { be_const_key_weak(set_val, -1), be_const_closure(class_lvh_chart_set_val_closure) },
        { be_const_key_weak(post_init, -1), be_const_closure(class_lvh_chart_post_init_closure) },
        { be_const_key_weak(add_point, 2), be_const_closure(class_lvh_chart_add_point_closure) },
        { be_const_key_weak(set_series1_color, 0), be_const_closure(class_lvh_chart_set_series1_color_closure) },
        { be_const_key_weak(set_h_div_line_count, -1), be_const_closure(class_lvh_chart_set_h_div_line_count_closure) },
    })),
    be_str_weak(lvh_chart)
);

extern const bclass be_class_lvh_btnmatrix;

/********************************************************************
** Solidified function: set_options
********************************************************************/
extern const bclass be_class_lvh_btnmatrix;
be_local_closure(class_lvh_btnmatrix_set_options,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_btnmatrix, 
    1,                          /* has constants */
    ( &(const bvalue[10]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(push),
    /* K2   */  be_nested_str_weak(),
    /* K3   */  be_nested_str_weak(_options),
    /* K4   */  be_nested_str_weak(_options_arr),
    /* K5   */  be_nested_str_weak(lv),
    /* K6   */  be_nested_str_weak(str_arr),
    /* K7   */  be_nested_str_weak(_lv_obj),
    /* K8   */  be_nested_str_weak(set_map),
    /* K9   */  be_nested_str_weak(HTP_X3A_X20_X27btnmatrix_X27_X20needs_X20_X27options_X27_X20to_X20be_X20a_X20list_X20of_X20strings),
    }),
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
** Solidified function: get_options
********************************************************************/
extern const bclass be_class_lvh_btnmatrix;
be_local_closure(class_lvh_btnmatrix_get_options,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_btnmatrix, 
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(_options),
    }),
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
** Solidified class: lvh_btnmatrix
********************************************************************/
extern const bclass be_class_lvh_obj;
be_local_class(lvh_btnmatrix,
    2,
    &be_class_lvh_obj,
    be_nested_map(5,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(_options, 1), be_const_var(0) },
        { be_const_key_weak(set_options, -1), be_const_closure(class_lvh_btnmatrix_set_options_closure) },
        { be_const_key_weak(_lv_class, -1), be_const_class(be_class_lv_buttonmatrix) },
        { be_const_key_weak(_options_arr, -1), be_const_var(1) },
        { be_const_key_weak(get_options, -1), be_const_closure(class_lvh_btnmatrix_get_options_closure) },
    })),
    be_str_weak(lvh_btnmatrix)
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

extern const bclass be_class_lvh_page;

/********************************************************************
** Solidified function: init
********************************************************************/
extern const bclass be_class_lvh_page;
be_local_closure(class_lvh_page_init,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_page, 
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
** Solidified function: show
********************************************************************/
extern const bclass be_class_lvh_page;
be_local_closure(class_lvh_page_show,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 3]) {
      be_nested_proto(
        3,                          /* nstack */
        0,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 3),
        }),
        0,                          /* has sup protos */
        NULL, 
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
        NULL, 
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
    &be_class_lvh_page, 
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
    /* K17  */  be_nested_str_weak(screen_load_anim),
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
** Solidified function: id
********************************************************************/
extern const bclass be_class_lvh_page;
be_local_closure(class_lvh_page_id,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_page, 
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
** Solidified function: get_scr
********************************************************************/
extern const bclass be_class_lvh_page;
be_local_closure(class_lvh_page_get_scr,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_page, 
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
** Solidified function: get_obj
********************************************************************/
extern const bclass be_class_lvh_page;
be_local_closure(class_lvh_page_get_obj,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_page, 
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
** Solidified function: add_obj
********************************************************************/
extern const bclass be_class_lvh_page;
be_local_closure(class_lvh_page_add_obj,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_page, 
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(_obj_id),
    /* K1   */  be_nested_str_weak(p_X25ib_X25i),
    /* K2   */  be_nested_str_weak(_page),
    /* K3   */  be_nested_str_weak(id),
    /* K4   */  be_nested_str_weak(global),
    }),
    be_str_weak(add_obj),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x980C0202,  //  0001  SETIDX	R3	R1	R2
      0x600C0018,  //  0002  GETGBL	R3	G24
      0x58100001,  //  0003  LDCONST	R4	K1
      0x88140502,  //  0004  GETMBR	R5	R2	K2
      0x8C140B03,  //  0005  GETMET	R5	R5	K3
      0x7C140200,  //  0006  CALL	R5	1
      0x5C180200,  //  0007  MOVE	R6	R1
      0x7C0C0600,  //  0008  CALL	R3	3
      0xB8120800,  //  0009  GETNGBL	R4	K4
      0x90100602,  //  000A  SETMBR	R4	R3	R2
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: remove_obj
********************************************************************/
extern const bclass be_class_lvh_page;
be_local_closure(class_lvh_page_remove_obj,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_lvh_page, 
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(_obj_id),
    /* K1   */  be_nested_str_weak(find),
    /* K2   */  be_nested_str_weak(remove),
    /* K3   */  be_nested_str_weak(p_X25ib_X25i),
    /* K4   */  be_nested_str_weak(_page),
    /* K5   */  be_nested_str_weak(id),
    /* K6   */  be_nested_str_weak(global),
    }),
    be_str_weak(remove_obj),
    &be_const_str_solidified,
    ( &(const binstruction[20]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x880C0100,  //  0004  GETMBR	R3	R0	K0
      0x8C0C0702,  //  0005  GETMET	R3	R3	K2
      0x5C140200,  //  0006  MOVE	R5	R1
      0x7C0C0400,  //  0007  CALL	R3	2
      0x780A0009,  //  0008  JMPF	R2	#0013
      0x600C0018,  //  0009  GETGBL	R3	G24
      0x58100003,  //  000A  LDCONST	R4	K3
      0x88140504,  //  000B  GETMBR	R5	R2	K4
      0x8C140B05,  //  000C  GETMET	R5	R5	K5
      0x7C140200,  //  000D  CALL	R5	1
      0x5C180200,  //  000E  MOVE	R6	R1
      0x7C0C0600,  //  000F  CALL	R3	3
      0xB8120C00,  //  0010  GETNGBL	R4	K6
      0x4C140000,  //  0011  LDNIL	R5
      0x90100605,  //  0012  SETMBR	R4	R3	R5
      0x80000000,  //  0013  RET	0
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
    be_nested_map(15,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(init, -1), be_const_closure(class_lvh_page_init_closure) },
        { be_const_key_weak(prev, 3), be_const_var(4) },
        { be_const_key_weak(next, -1), be_const_var(5) },
        { be_const_key_weak(back, 4), be_const_var(6) },
        { be_const_key_weak(remove_obj, 7), be_const_closure(class_lvh_page_remove_obj_closure) },
        { be_const_key_weak(show_anim, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(5,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(0, 3), be_const_int(0) },
        { be_const_key_int(1, -1), be_const_int(5) },
        { be_const_key_int(2, -1), be_const_int(8) },
        { be_const_key_int(-1, -1), be_const_int(6) },
        { be_const_key_int(-2, -1), be_const_int(7) },
    }))    ) } )) },
        { be_const_key_weak(id, 5), be_const_closure(class_lvh_page_id_closure) },
        { be_const_key_weak(_page_id, 11), be_const_var(1) },
        { be_const_key_weak(get_obj, -1), be_const_closure(class_lvh_page_get_obj_closure) },
        { be_const_key_weak(add_obj, -1), be_const_closure(class_lvh_page_add_obj_closure) },
        { be_const_key_weak(_obj_id, -1), be_const_var(0) },
        { be_const_key_weak(_lv_scr, 13), be_const_var(2) },
        { be_const_key_weak(_oh, -1), be_const_var(3) },
        { be_const_key_weak(get_scr, 14), be_const_closure(class_lvh_page_get_scr_closure) },
        { be_const_key_weak(show, -1), be_const_closure(class_lvh_page_show_closure) },
    })),
    be_str_weak(lvh_page)
);

extern const bclass be_class_HASPmota;

/********************************************************************
** Solidified function: _load
********************************************************************/
extern const bclass be_class_HASPmota;
be_local_closure(class_HASPmota__load,   /* name */
  be_nested_proto(
    15,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_HASPmota, 
    1,                          /* has constants */
    ( &(const bvalue[26]) {     /* constants */
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
    /* K14  */  be_nested_str_weak(tasmota),
    /* K15  */  be_nested_str_weak(loglevel),
    /* K16  */  be_nested_str_weak(log),
    /* K17  */  be_nested_str_weak(HSP_X3A_X20parsing_X20line_X20_X27_X25s_X27),
    /* K18  */  be_nested_str_weak(parse_page),
    /* K19  */  be_nested_str_weak(parse_obj),
    /* K20  */  be_nested_str_weak(tr),
    /* K21  */  be_nested_str_weak(_X20_X09),
    /* K22  */  be_nested_str_weak(),
    /* K23  */  be_nested_str_weak(HSP_X3A_X20invalid_X20JSON_X20line_X20_X27_X25s_X27),
    /* K24  */  be_const_int(2),
    /* K25  */  be_nested_str_weak(remove),
    }),
    be_str_weak(_load),
    &be_const_str_solidified,
    ( &(const binstruction[85]) {  /* code */
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
      0x78220034,  //  001C  JMPF	R8	#0052
      0x8C20070C,  //  001D  GETMET	R8	R3	K12
      0x94280F0B,  //  001E  GETIDX	R10	R7	K11
      0x7C200400,  //  001F  CALL	R8	2
      0x60240004,  //  0020  GETGBL	R9	G4
      0x5C281000,  //  0021  MOVE	R10	R8
      0x7C240200,  //  0022  CALL	R9	1
      0x1C24130D,  //  0023  EQ	R9	R9	K13
      0x78260016,  //  0024  JMPF	R9	#003C
      0xB8261C00,  //  0025  GETNGBL	R9	K14
      0x8C24130F,  //  0026  GETMET	R9	R9	K15
      0x542E0003,  //  0027  LDINT	R11	4
      0x7C240400,  //  0028  CALL	R9	2
      0x78260007,  //  0029  JMPF	R9	#0032
      0xB8261C00,  //  002A  GETNGBL	R9	K14
      0x8C241310,  //  002B  GETMET	R9	R9	K16
      0x602C0018,  //  002C  GETGBL	R11	G24
      0x58300011,  //  002D  LDCONST	R12	K17
      0x94340F0B,  //  002E  GETIDX	R13	R7	K11
      0x7C2C0400,  //  002F  CALL	R11	2
      0x54320003,  //  0030  LDINT	R12	4
      0x7C240600,  //  0031  CALL	R9	3
      0x8C240112,  //  0032  GETMET	R9	R0	K18
      0x5C2C1000,  //  0033  MOVE	R11	R8
      0x7C240400,  //  0034  CALL	R9	2
      0x8C240113,  //  0035  GETMET	R9	R0	K19
      0x5C2C1000,  //  0036  MOVE	R11	R8
      0x88300105,  //  0037  GETMBR	R12	R0	K5
      0x88340102,  //  0038  GETMBR	R13	R0	K2
      0x9430180D,  //  0039  GETIDX	R12	R12	R13
      0x7C240600,  //  003A  CALL	R9	3
      0x70020010,  //  003B  JMP		#004D
      0x6024000C,  //  003C  GETGBL	R9	G12
      0x8C280514,  //  003D  GETMET	R10	R2	K20
      0x94300F0B,  //  003E  GETIDX	R12	R7	K11
      0x58340015,  //  003F  LDCONST	R13	K21
      0x58380016,  //  0040  LDCONST	R14	K22
      0x7C280800,  //  0041  CALL	R10	4
      0x7C240200,  //  0042  CALL	R9	1
      0x2424130B,  //  0043  GT	R9	R9	K11
      0x78260007,  //  0044  JMPF	R9	#004D
      0xB8261C00,  //  0045  GETNGBL	R9	K14
      0x8C241310,  //  0046  GETMET	R9	R9	K16
      0x602C0018,  //  0047  GETGBL	R11	G24
      0x58300017,  //  0048  LDCONST	R12	K23
      0x94340F0B,  //  0049  GETIDX	R13	R7	K11
      0x7C2C0400,  //  004A  CALL	R11	2
      0x58300018,  //  004B  LDCONST	R12	K24
      0x7C240600,  //  004C  CALL	R9	3
      0x4C200000,  //  004D  LDNIL	R8
      0x8C240F19,  //  004E  GETMET	R9	R7	K25
      0x582C000B,  //  004F  LDCONST	R11	K11
      0x7C240400,  //  0050  CALL	R9	2
      0x7001FFC5,  //  0051  JMP		#0018
      0x4C1C0000,  //  0052  LDNIL	R7
      0x90020503,  //  0053  SETMBR	R0	K2	K3
      0x80000000,  //  0054  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: page_show
********************************************************************/
extern const bclass be_class_HASPmota;
be_local_closure(class_HASPmota_page_show,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_HASPmota, 
    1,                          /* has constants */
    ( &(const bvalue[11]) {     /* constants */
    /* K0   */  be_nested_str_weak(lvh_pages),
    /* K1   */  be_nested_str_weak(lvh_page_cur_idx),
    /* K2   */  be_nested_str_weak(pages_list_sorted),
    /* K3   */  be_const_int(1),
    /* K4   */  be_nested_str_weak(prev),
    /* K5   */  be_nested_str_weak(next),
    /* K6   */  be_nested_str_weak(back),
    /* K7   */  be_nested_str_weak(re_page_target),
    /* K8   */  be_nested_str_weak(match),
    /* K9   */  be_const_int(0),
    /* K10  */  be_nested_str_weak(show),
    }),
    be_str_weak(page_show),
    &be_const_str_solidified,
    ( &(const binstruction[68]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x880C0100,  //  0001  GETMBR	R3	R0	K0
      0x88100101,  //  0002  GETMBR	R4	R0	K1
      0x940C0604,  //  0003  GETIDX	R3	R3	R4
      0x8C100102,  //  0004  GETMET	R4	R0	K2
      0x88180101,  //  0005  GETMBR	R6	R0	K1
      0x7C100400,  //  0006  CALL	R4	2
      0x6014000C,  //  0007  GETGBL	R5	G12
      0x5C180800,  //  0008  MOVE	R6	R4
      0x7C140200,  //  0009  CALL	R5	1
      0x18140B03,  //  000A  LE	R5	R5	K3
      0x78160000,  //  000B  JMPF	R5	#000D
      0x80000A00,  //  000C  RET	0
      0x1C140304,  //  000D  EQ	R5	R1	K4
      0x78160009,  //  000E  JMPF	R5	#0019
      0x60140009,  //  000F  GETGBL	R5	G9
      0x88180704,  //  0010  GETMBR	R6	R3	K4
      0x7C140200,  //  0011  CALL	R5	1
      0x5C080A00,  //  0012  MOVE	R2	R5
      0x4C140000,  //  0013  LDNIL	R5
      0x1C140405,  //  0014  EQ	R5	R2	R5
      0x78160001,  //  0015  JMPF	R5	#0018
      0x5415FFFE,  //  0016  LDINT	R5	-1
      0x94080805,  //  0017  GETIDX	R2	R4	R5
      0x70020020,  //  0018  JMP		#003A
      0x1C140305,  //  0019  EQ	R5	R1	K5
      0x78160008,  //  001A  JMPF	R5	#0024
      0x60140009,  //  001B  GETGBL	R5	G9
      0x88180705,  //  001C  GETMBR	R6	R3	K5
      0x7C140200,  //  001D  CALL	R5	1
      0x5C080A00,  //  001E  MOVE	R2	R5
      0x4C140000,  //  001F  LDNIL	R5
      0x1C140405,  //  0020  EQ	R5	R2	R5
      0x78160000,  //  0021  JMPF	R5	#0023
      0x94080903,  //  0022  GETIDX	R2	R4	K3
      0x70020015,  //  0023  JMP		#003A
      0x1C140306,  //  0024  EQ	R5	R1	K6
      0x78160008,  //  0025  JMPF	R5	#002F
      0x60140009,  //  0026  GETGBL	R5	G9
      0x88180706,  //  0027  GETMBR	R6	R3	K6
      0x7C140200,  //  0028  CALL	R5	1
      0x5C080A00,  //  0029  MOVE	R2	R5
      0x4C140000,  //  002A  LDNIL	R5
      0x1C140405,  //  002B  EQ	R5	R2	R5
      0x78160000,  //  002C  JMPF	R5	#002E
      0x58080003,  //  002D  LDCONST	R2	K3
      0x7002000A,  //  002E  JMP		#003A
      0x88140107,  //  002F  GETMBR	R5	R0	K7
      0x8C140B08,  //  0030  GETMET	R5	R5	K8
      0x5C1C0200,  //  0031  MOVE	R7	R1
      0x7C140400,  //  0032  CALL	R5	2
      0x78160005,  //  0033  JMPF	R5	#003A
      0x60140009,  //  0034  GETGBL	R5	G9
      0x5419FFFE,  //  0035  LDINT	R6	-1
      0x401A0606,  //  0036  CONNECT	R6	K3	R6
      0x94180206,  //  0037  GETIDX	R6	R1	R6
      0x7C140200,  //  0038  CALL	R5	1
      0x5C080A00,  //  0039  MOVE	R2	R5
      0x4C140000,  //  003A  LDNIL	R5
      0x20140405,  //  003B  NE	R5	R2	R5
      0x78160005,  //  003C  JMPF	R5	#0043
      0x24140509,  //  003D  GT	R5	R2	K9
      0x78160003,  //  003E  JMPF	R5	#0043
      0x88140100,  //  003F  GETMBR	R5	R0	K0
      0x94140A02,  //  0040  GETIDX	R5	R5	R2
      0x8C140B0A,  //  0041  GETMET	R5	R5	K10
      0x7C140200,  //  0042  CALL	R5	1
      0x80000000,  //  0043  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
extern const bclass be_class_HASPmota;
be_local_closure(class_HASPmota_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_HASPmota, 
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(fix_lv_version),
    /* K1   */  be_nested_str_weak(re),
    /* K2   */  be_nested_str_weak(re_page_target),
    /* K3   */  be_nested_str_weak(compile),
    /* K4   */  be_nested_str_weak(p_X5Cd_X2B),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0xA4060200,  //  0002  IMPORT	R1	K1
      0x8C080303,  //  0003  GETMET	R2	R1	K3
      0x58100004,  //  0004  LDCONST	R4	K4
      0x7C080400,  //  0005  CALL	R2	2
      0x90020402,  //  0006  SETMBR	R0	K2	R2
      0x80000000,  //  0007  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: event_dispatch
********************************************************************/
extern const bclass be_class_HASPmota;
be_local_closure(class_HASPmota_event_dispatch,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_HASPmota, 
    1,                          /* has constants */
    ( &(const bvalue[11]) {     /* constants */
    /* K0   */  be_nested_str_weak(introspect),
    /* K1   */  be_nested_str_weak(toptr),
    /* K2   */  be_nested_str_weak(event),
    /* K3   */  be_nested_str_weak(_p),
    /* K4   */  be_nested_str_weak(lv),
    /* K5   */  be_nested_str_weak(lv_event),
    /* K6   */  be_nested_str_weak(get_user_data),
    /* K7   */  be_const_int(0),
    /* K8   */  be_nested_str_weak(fromptr),
    /* K9   */  be_nested_str_weak(instance),
    /* K10  */  be_nested_str_weak(event_cb),
    }),
    be_str_weak(event_dispatch),
    &be_const_str_solidified,
    ( &(const binstruction[34]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x8C0C0501,  //  0001  GETMET	R3	R2	K1
      0x5C140200,  //  0002  MOVE	R5	R1
      0x7C0C0400,  //  0003  CALL	R3	2
      0x88100102,  //  0004  GETMBR	R4	R0	K2
      0x78120002,  //  0005  JMPF	R4	#0009
      0x88100102,  //  0006  GETMBR	R4	R0	K2
      0x90120603,  //  0007  SETMBR	R4	K3	R3
      0x70020004,  //  0008  JMP		#000E
      0xB8120800,  //  0009  GETNGBL	R4	K4
      0x8C100905,  //  000A  GETMET	R4	R4	K5
      0x5C180600,  //  000B  MOVE	R6	R3
      0x7C100400,  //  000C  CALL	R4	2
      0x90020404,  //  000D  SETMBR	R0	K2	R4
      0x88100102,  //  000E  GETMBR	R4	R0	K2
      0x8C100906,  //  000F  GETMET	R4	R4	K6
      0x7C100200,  //  0010  CALL	R4	1
      0x60140009,  //  0011  GETGBL	R5	G9
      0x5C180800,  //  0012  MOVE	R6	R4
      0x7C140200,  //  0013  CALL	R5	1
      0x20140B07,  //  0014  NE	R5	R5	K7
      0x7816000A,  //  0015  JMPF	R5	#0021
      0x8C140508,  //  0016  GETMET	R5	R2	K8
      0x5C1C0800,  //  0017  MOVE	R7	R4
      0x7C140400,  //  0018  CALL	R5	2
      0x60180004,  //  0019  GETGBL	R6	G4
      0x5C1C0A00,  //  001A  MOVE	R7	R5
      0x7C180200,  //  001B  CALL	R6	1
      0x1C180D09,  //  001C  EQ	R6	R6	K9
      0x781A0002,  //  001D  JMPF	R6	#0021
      0x8C180B0A,  //  001E  GETMET	R6	R5	K10
      0x88200102,  //  001F  GETMBR	R8	R0	K2
      0x7C180400,  //  0020  CALL	R6	2
      0x80000000,  //  0021  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: pages_list_sorted
********************************************************************/
extern const bclass be_class_HASPmota;
be_local_closure(class_HASPmota_pages_list_sorted,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_HASPmota, 
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
extern const bclass be_class_HASPmota;
be_local_closure(class_HASPmota_get_page_cur,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_HASPmota, 
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
** Solidified function: fix_lv_version
********************************************************************/
extern const bclass be_class_HASPmota;
be_local_closure(class_HASPmota_fix_lv_version,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    0,                          /* argc */
    4,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_HASPmota, 
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_const_class(be_class_HASPmota),
    /* K1   */  be_nested_str_weak(introspect),
    /* K2   */  be_nested_str_weak(get),
    /* K3   */  be_nested_str_weak(lv),
    /* K4   */  be_nested_str_weak(version),
    /* K5   */  be_nested_str_weak(int),
    }),
    be_str_weak(fix_lv_version),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x58000000,  //  0000  LDCONST	R0	K0
      0xA4060200,  //  0001  IMPORT	R1	K1
      0x8C080302,  //  0002  GETMET	R2	R1	K2
      0xB8120600,  //  0003  GETNGBL	R4	K3
      0x58140004,  //  0004  LDCONST	R5	K4
      0x7C080600,  //  0005  CALL	R2	3
      0x600C0004,  //  0006  GETGBL	R3	G4
      0x5C100400,  //  0007  MOVE	R4	R2
      0x7C0C0200,  //  0008  CALL	R3	1
      0x200C0705,  //  0009  NE	R3	R3	K5
      0x780E0002,  //  000A  JMPF	R3	#000E
      0xB80E0600,  //  000B  GETNGBL	R3	K3
      0x54120007,  //  000C  LDINT	R4	8
      0x900E0804,  //  000D  SETMBR	R3	K4	R4
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: do_action
********************************************************************/
extern const bclass be_class_HASPmota;
be_local_closure(class_HASPmota_do_action,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_HASPmota, 
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(lv),
    /* K1   */  be_nested_str_weak(EVENT_CLICKED),
    /* K2   */  be_nested_str_weak(page_show),
    /* K3   */  be_nested_str_weak(_action),
    }),
    be_str_weak(do_action),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0xB80E0000,  //  0000  GETNGBL	R3	K0
      0x880C0701,  //  0001  GETMBR	R3	R3	K1
      0x200C0403,  //  0002  NE	R3	R2	R3
      0x780E0000,  //  0003  JMPF	R3	#0005
      0x80000600,  //  0004  RET	0
      0x8C0C0102,  //  0005  GETMET	R3	R0	K2
      0x88140303,  //  0006  GETMBR	R5	R1	K3
      0x7C0C0400,  //  0007  CALL	R3	2
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse
********************************************************************/
extern const bclass be_class_HASPmota;
be_local_closure(class_HASPmota_parse,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_HASPmota, 
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
** Solidified function: register_event
********************************************************************/
extern const bclass be_class_HASPmota;
be_local_closure(class_HASPmota_register_event,   /* name */
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
        1,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL, 
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
    &be_class_HASPmota, 
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
** Solidified function: page_dir_to
********************************************************************/
extern const bclass be_class_HASPmota;
be_local_closure(class_HASPmota_page_dir_to,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_HASPmota, 
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
** Solidified function: parse_page
********************************************************************/
extern const bclass be_class_HASPmota;
be_local_closure(class_HASPmota_parse_page,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_HASPmota, 
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
** Solidified function: sort
********************************************************************/
extern const bclass be_class_HASPmota;
be_local_closure(class_HASPmota_sort,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    4,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_HASPmota, 
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
** Solidified function: start
********************************************************************/
extern const bclass be_class_HASPmota;
be_local_closure(class_HASPmota_start,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_HASPmota, 
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
** Solidified function: parse_obj
********************************************************************/
extern const bclass be_class_HASPmota;
be_local_closure(class_HASPmota_parse_obj,   /* name */
  be_nested_proto(
    22,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_HASPmota, 
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
    /* K21  */  be_nested_str_weak(HSP_X3A_X20Cannot_X20find_X20object_X20of_X20type_X20),
    /* K22  */  be_nested_str_weak(add_obj),
    /* K23  */  be_nested_str_weak(function),
    /* K24  */  be_nested_str_weak(HSP_X3A_X20unable_X20to_X20run_X20berry_X20code_X20_X22_X25s_X22_X20_X2D_X20_X27_X25s_X27_X20_X2D_X20_X25s),
    /* K25  */  be_const_int(0),
    /* K26  */  be_nested_str_weak(HSP_X3A_X20cannot_X20specify_X20_X27obj_X27_X20for_X20_X27id_X27_X3A0),
    /* K27  */  be_nested_str_weak(keys),
    /* K28  */  be_nested_str_weak(stop_iteration),
    /* K29  */  be_nested_str_weak(post_config),
    }),
    be_str_weak(parse_obj),
    &be_const_str_solidified,
    ( &(const binstruction[215]) {  /* code */
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
      0x202C0D07,  //  002A  NE	R11	R6	K7
      0x782E006A,  //  002B  JMPF	R11	#0097
      0x4C2C0000,  //  002C  LDNIL	R11
      0x202C0A0B,  //  002D  NE	R11	R5	R11
      0x782E0067,  //  002E  JMPF	R11	#0097
      0x142C0B09,  //  002F  LT	R11	R5	K9
      0x742E0002,  //  0030  JMPT	R11	#0034
      0x542E00FD,  //  0031  LDINT	R11	254
      0x242C0A0B,  //  0032  GT	R11	R5	R11
      0x782E0008,  //  0033  JMPF	R11	#003D
      0x602C0001,  //  0034  GETGBL	R11	G1
      0x60300008,  //  0035  GETGBL	R12	G8
      0x5C340A00,  //  0036  MOVE	R13	R5
      0x7C300200,  //  0037  CALL	R12	1
      0x0032140C,  //  0038  ADD	R12	K10	R12
      0x0030190B,  //  0039  ADD	R12	R12	K11
      0x00301806,  //  003A  ADD	R12	R12	R6
      0x7C2C0200,  //  003B  CALL	R11	1
      0x80001600,  //  003C  RET	0
      0x4C2C0000,  //  003D  LDNIL	R11
      0x60300009,  //  003E  GETGBL	R12	G9
      0x8C340302,  //  003F  GETMET	R13	R1	K2
      0x583C000C,  //  0040  LDCONST	R15	K12
      0x7C340400,  //  0041  CALL	R13	2
      0x7C300200,  //  0042  CALL	R12	1
      0x4C340000,  //  0043  LDNIL	R13
      0x4C380000,  //  0044  LDNIL	R14
      0x2038180E,  //  0045  NE	R14	R12	R14
      0x783A0007,  //  0046  JMPF	R14	#004F
      0x8C38110D,  //  0047  GETMET	R14	R8	K13
      0x5C401800,  //  0048  MOVE	R16	R12
      0x7C380400,  //  0049  CALL	R14	2
      0x5C341C00,  //  004A  MOVE	R13	R14
      0x4C380000,  //  004B  LDNIL	R14
      0x20381A0E,  //  004C  NE	R14	R13	R14
      0x783A0000,  //  004D  JMPF	R14	#004F
      0x882C1B0E,  //  004E  GETMBR	R11	R13	K14
      0x4C380000,  //  004F  LDNIL	R14
      0x1C38160E,  //  0050  EQ	R14	R11	R14
      0x783A0002,  //  0051  JMPF	R14	#0055
      0x8C38110F,  //  0052  GETMET	R14	R8	K15
      0x7C380200,  //  0053  CALL	R14	1
      0x5C2C1C00,  //  0054  MOVE	R11	R14
      0x8C380910,  //  0055  GETMET	R14	R4	K16
      0x5C400000,  //  0056  MOVE	R16	R0
      0x00462206,  //  0057  ADD	R17	K17	R6
      0x7C380600,  //  0058  CALL	R14	3
      0x4C3C0000,  //  0059  LDNIL	R15
      0x4C400000,  //  005A  LDNIL	R16
      0x1C401C10,  //  005B  EQ	R16	R14	R16
      0x78420010,  //  005C  JMPF	R16	#006E
      0x8C400910,  //  005D  GETMET	R16	R4	K16
      0x5C480600,  //  005E  MOVE	R18	R3
      0x5C4C0C00,  //  005F  MOVE	R19	R6
      0x7C400600,  //  0060  CALL	R16	3
      0x4C440000,  //  0061  LDNIL	R17
      0x20442011,  //  0062  NE	R17	R16	R17
      0x78460009,  //  0063  JMPF	R17	#006E
      0x60440004,  //  0064  GETGBL	R17	G4
      0x5C482000,  //  0065  MOVE	R18	R16
      0x7C440200,  //  0066  CALL	R17	1
      0x1C442312,  //  0067  EQ	R17	R17	K18
      0x78460004,  //  0068  JMPF	R17	#006E
      0x5C442000,  //  0069  MOVE	R17	R16
      0x5C481600,  //  006A  MOVE	R18	R11
      0x7C440200,  //  006B  CALL	R17	1
      0x5C3C2200,  //  006C  MOVE	R15	R17
      0x88380113,  //  006D  GETMBR	R14	R0	K19
      0x4C400000,  //  006E  LDNIL	R16
      0x1C401C10,  //  006F  EQ	R16	R14	R16
      0x7842000F,  //  0070  JMPF	R16	#0081
      0x8C400914,  //  0071  GETMET	R16	R4	K20
      0x5C480C00,  //  0072  MOVE	R18	R6
      0x7C400400,  //  0073  CALL	R16	2
      0x4C440000,  //  0074  LDNIL	R17
      0x20442011,  //  0075  NE	R17	R16	R17
      0x78460009,  //  0076  JMPF	R17	#0081
      0x60440004,  //  0077  GETGBL	R17	G4
      0x5C482000,  //  0078  MOVE	R18	R16
      0x7C440200,  //  0079  CALL	R17	1
      0x1C442312,  //  007A  EQ	R17	R17	K18
      0x78460004,  //  007B  JMPF	R17	#0081
      0x5C442000,  //  007C  MOVE	R17	R16
      0x5C481600,  //  007D  MOVE	R18	R11
      0x7C440200,  //  007E  CALL	R17	1
      0x5C3C2200,  //  007F  MOVE	R15	R17
      0x88380113,  //  0080  GETMBR	R14	R0	K19
      0x4C400000,  //  0081  LDNIL	R16
      0x1C401C10,  //  0082  EQ	R16	R14	R16
      0x78420006,  //  0083  JMPF	R16	#008B
      0x60400001,  //  0084  GETGBL	R16	G1
      0x60440008,  //  0085  GETGBL	R17	G8
      0x5C480C00,  //  0086  MOVE	R18	R6
      0x7C440200,  //  0087  CALL	R17	1
      0x00462A11,  //  0088  ADD	R17	K21	R17
      0x7C400200,  //  0089  CALL	R16	1
      0x80002000,  //  008A  RET	0
      0x5C401C00,  //  008B  MOVE	R16	R14
      0x5C441600,  //  008C  MOVE	R17	R11
      0x5C480400,  //  008D  MOVE	R18	R2
      0x5C4C0200,  //  008E  MOVE	R19	R1
      0x5C501E00,  //  008F  MOVE	R20	R15
      0x5C541A00,  //  0090  MOVE	R21	R13
      0x7C400A00,  //  0091  CALL	R16	5
      0x5C1C2000,  //  0092  MOVE	R7	R16
      0x8C401116,  //  0093  GETMET	R16	R8	K22
      0x5C480A00,  //  0094  MOVE	R18	R5
      0x5C4C0E00,  //  0095  MOVE	R19	R7
      0x7C400600,  //  0096  CALL	R16	3
      0x4C2C0000,  //  0097  LDNIL	R11
      0x202C140B,  //  0098  NE	R11	R10	R11
      0x782E0018,  //  0099  JMPF	R11	#00B3
      0xA802000B,  //  009A  EXBLK	0	#00A7
      0x5C2C1400,  //  009B  MOVE	R11	R10
      0x7C2C0000,  //  009C  CALL	R11	0
      0x60300004,  //  009D  GETGBL	R12	G4
      0x5C341600,  //  009E  MOVE	R13	R11
      0x7C300200,  //  009F  CALL	R12	1
      0x1C301917,  //  00A0  EQ	R12	R12	K23
      0x78320002,  //  00A1  JMPF	R12	#00A5
      0x5C301600,  //  00A2  MOVE	R12	R11
      0x5C340E00,  //  00A3  MOVE	R13	R7
      0x7C300200,  //  00A4  CALL	R12	1
      0xA8040001,  //  00A5  EXBLK	1	1
      0x7002000B,  //  00A6  JMP		#00B3
      0xAC2C0002,  //  00A7  CATCH	R11	0	2
      0x70020008,  //  00A8  JMP		#00B2
      0x60340001,  //  00A9  GETGBL	R13	G1
      0x60380018,  //  00AA  GETGBL	R14	G24
      0x583C0018,  //  00AB  LDCONST	R15	K24
      0x5C401200,  //  00AC  MOVE	R16	R9
      0x5C441600,  //  00AD  MOVE	R17	R11
      0x5C481800,  //  00AE  MOVE	R18	R12
      0x7C380800,  //  00AF  CALL	R14	4
      0x7C340200,  //  00B0  CALL	R13	1
      0x70020000,  //  00B1  JMP		#00B3
      0xB0080000,  //  00B2  RAISE	2	R0	R0
      0x4C2C0000,  //  00B3  LDNIL	R11
      0x1C2C0A0B,  //  00B4  EQ	R11	R5	R11
      0x782E0000,  //  00B5  JMPF	R11	#00B7
      0x80001600,  //  00B6  RET	0
      0x1C2C0B19,  //  00B7  EQ	R11	R5	K25
      0x782E0005,  //  00B8  JMPF	R11	#00BF
      0x202C0D07,  //  00B9  NE	R11	R6	K7
      0x782E0003,  //  00BA  JMPF	R11	#00BF
      0x602C0001,  //  00BB  GETGBL	R11	G1
      0x5830001A,  //  00BC  LDCONST	R12	K26
      0x7C2C0200,  //  00BD  CALL	R11	1
      0x80001600,  //  00BE  RET	0
      0x1C2C0B19,  //  00BF  EQ	R11	R5	K25
      0x782E0005,  //  00C0  JMPF	R11	#00C7
      0x8C2C0105,  //  00C1  GETMET	R11	R0	K5
      0x7C2C0200,  //  00C2  CALL	R11	1
      0x8C2C170D,  //  00C3  GETMET	R11	R11	K13
      0x58340019,  //  00C4  LDCONST	R13	K25
      0x7C2C0400,  //  00C5  CALL	R11	2
      0x5C1C1600,  //  00C6  MOVE	R7	R11
      0x602C0010,  //  00C7  GETGBL	R11	G16
      0x8C30031B,  //  00C8  GETMET	R12	R1	K27
      0x7C300200,  //  00C9  CALL	R12	1
      0x7C2C0200,  //  00CA  CALL	R11	1
      0xA8020004,  //  00CB  EXBLK	0	#00D1
      0x5C301600,  //  00CC  MOVE	R12	R11
      0x7C300000,  //  00CD  CALL	R12	0
      0x9434020C,  //  00CE  GETIDX	R13	R1	R12
      0x901C180D,  //  00CF  SETMBR	R7	R12	R13
      0x7001FFFA,  //  00D0  JMP		#00CC
      0x582C001C,  //  00D1  LDCONST	R11	K28
      0xAC2C0200,  //  00D2  CATCH	R11	1	0
      0xB0080000,  //  00D3  RAISE	2	R0	R0
      0x8C2C0F1D,  //  00D4  GETMET	R11	R7	K29
      0x7C2C0200,  //  00D5  CALL	R11	1
      0x80000000,  //  00D6  RET	0
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
    be_nested_map(54,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(_load, -1), be_const_closure(class_HASPmota__load_closure) },
        { be_const_key_weak(lvh_switch, -1), be_const_class(be_class_lvh_switch) },
        { be_const_key_weak(lvh_scr, -1), be_const_class(be_class_lvh_scr) },
        { be_const_key_weak(lvh_root, 50), be_const_class(be_class_lvh_root) },
        { be_const_key_weak(lvh_spinner, -1), be_const_class(be_class_lvh_spinner) },
        { be_const_key_weak(dark, 49), be_const_var(0) },
        { be_const_key_weak(lvh_qrcode, -1), be_const_class(be_class_lvh_qrcode) },
        { be_const_key_weak(lvh_led, -1), be_const_class(be_class_lvh_led) },
        { be_const_key_weak(parse_obj, 32), be_const_closure(class_HASPmota_parse_obj_closure) },
        { be_const_key_weak(event_cb, 46), be_const_var(9) },
        { be_const_key_weak(lvh_scale_section, -1), be_const_class(be_class_lvh_scale_section) },
        { be_const_key_weak(lvh_span, -1), be_const_class(be_class_lvh_span) },
        { be_const_key_weak(lvh_scale, -1), be_const_class(be_class_lvh_scale) },
        { be_const_key_weak(r16, -1), be_const_var(4) },
        { be_const_key_weak(start, -1), be_const_closure(class_HASPmota_start_closure) },
        { be_const_key_weak(event, -1), be_const_var(8) },
        { be_const_key_weak(event_dispatch, 1), be_const_closure(class_HASPmota_event_dispatch_closure) },
        { be_const_key_weak(lvh_btnmatrix, -1), be_const_class(be_class_lvh_btnmatrix) },
        { be_const_key_weak(lvh_page, 35), be_const_class(be_class_lvh_page) },
        { be_const_key_weak(sort, -1), be_const_static_closure(class_HASPmota_sort_closure) },
        { be_const_key_weak(lvh_btn, 53), be_const_class(be_class_lvh_btn) },
        { be_const_key_weak(lvh_img, -1), be_const_class(be_class_lvh_img) },
        { be_const_key_weak(lvh_arc, -1), be_const_class(be_class_lvh_arc) },
        { be_const_key_weak(lvh_obj, 52), be_const_class(be_class_lvh_obj) },
        { be_const_key_weak(pages_list_sorted, -1), be_const_closure(class_HASPmota_pages_list_sorted_closure) },
        { be_const_key_weak(def_templ_name, -1), be_nested_str_weak(pages_X2Ejsonl) },
        { be_const_key_weak(get_page_cur, 8), be_const_closure(class_HASPmota_get_page_cur_closure) },
        { be_const_key_weak(lvh_slider, -1), be_const_class(be_class_lvh_slider) },
        { be_const_key_weak(lvh_pages, -1), be_const_var(5) },
        { be_const_key_weak(parse_page, -1), be_const_closure(class_HASPmota_parse_page_closure) },
        { be_const_key_weak(lvh_dropdown_list, 3), be_const_class(be_class_lvh_dropdown_list) },
        { be_const_key_weak(lvh_chart, -1), be_const_class(be_class_lvh_chart) },
        { be_const_key_weak(lvh_roller, -1), be_const_class(be_class_lvh_roller) },
        { be_const_key_weak(lvh_bar, -1), be_const_class(be_class_lvh_bar) },
        { be_const_key_weak(parse, -1), be_const_closure(class_HASPmota_parse_closure) },
        { be_const_key_weak(register_event, -1), be_const_closure(class_HASPmota_register_event_closure) },
        { be_const_key_weak(re_page_target, 31), be_const_var(7) },
        { be_const_key_weak(lvh_scale_line, -1), be_const_class(be_class_lvh_scale_line) },
        { be_const_key_weak(lvh_line, -1), be_const_class(be_class_lvh_line) },
        { be_const_key_weak(lvh_flex, 18), be_const_class(be_class_lvh_flex) },
        { be_const_key_weak(page_show, 38), be_const_closure(class_HASPmota_page_show_closure) },
        { be_const_key_weak(scr, -1), be_const_var(3) },
        { be_const_key_weak(page_dir_to, 11), be_const_closure(class_HASPmota_page_dir_to_closure) },
        { be_const_key_weak(vres, -1), be_const_var(2) },
        { be_const_key_weak(lvh_page_cur_idx, 29), be_const_var(6) },
        { be_const_key_weak(lvh_dropdown, 19), be_const_class(be_class_lvh_dropdown) },
        { be_const_key_weak(hres, -1), be_const_var(1) },
        { be_const_key_weak(lvh_spangroup, 14), be_const_class(be_class_lvh_spangroup) },
        { be_const_key_weak(lvh_fixed, 13), be_const_class(be_class_lvh_fixed) },
        { be_const_key_weak(do_action, -1), be_const_closure(class_HASPmota_do_action_closure) },
        { be_const_key_weak(fix_lv_version, -1), be_const_static_closure(class_HASPmota_fix_lv_version_closure) },
        { be_const_key_weak(init, 4), be_const_closure(class_HASPmota_init_closure) },
        { be_const_key_weak(lvh_checkbox, -1), be_const_class(be_class_lvh_checkbox) },
        { be_const_key_weak(lvh_label, -1), be_const_class(be_class_lvh_label) },
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
    NULL, 
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
