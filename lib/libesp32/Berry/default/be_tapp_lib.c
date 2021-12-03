/********************************************************************
 * Tasmota App manager
 * 
 * To use: `import tapp`
 * 
 *******************************************************************/
#include "be_constobj.h"

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Tapp_init,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_literal("tasmota"),
    /* K1   */  be_nested_str_literal("add_driver"),
    }),
    (be_nested_const_str("init", 380752755, 4)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[ 5]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: autoexec
********************************************************************/
be_local_closure(Tapp_autoexec,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[15]) {     /* constants */
    /* K0   */  be_nested_str_literal("path"),
    /* K1   */  be_nested_str_literal("string"),
    /* K2   */  be_nested_str_literal("listdir"),
    /* K3   */  be_nested_str_literal("/"),
    /* K4   */  be_nested_str_literal("find"),
    /* K5   */  be_nested_str_literal(".tapp"),
    /* K6   */  be_const_int(0),
    /* K7   */  be_nested_str_literal("tasmota"),
    /* K8   */  be_nested_str_literal("log"),
    /* K9   */  be_nested_str_literal("format"),
    /* K10  */  be_nested_str_literal("TAP: found Tasmota App '%s'"),
    /* K11  */  be_const_int(2),
    /* K12  */  be_nested_str_literal("load"),
    /* K13  */  be_nested_str_literal("#autoexec.be"),
    /* K14  */  be_nested_str_literal("stop_iteration"),
    }),
    (be_nested_const_str("autoexec", -618105405, 8)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[34]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0x8C0C0302,  //  0002  GETMET	R3	R1	K2
      0x58140003,  //  0003  LDCONST	R5	K3
      0x7C0C0400,  //  0004  CALL	R3	2
      0x60100010,  //  0005  GETGBL	R4	G16
      0x5C140600,  //  0006  MOVE	R5	R3
      0x7C100200,  //  0007  CALL	R4	1
      0xA8020014,  //  0008  EXBLK	0	#001E
      0x5C140800,  //  0009  MOVE	R5	R4
      0x7C140000,  //  000A  CALL	R5	0
      0x8C180504,  //  000B  GETMET	R6	R2	K4
      0x5C200A00,  //  000C  MOVE	R8	R5
      0x58240005,  //  000D  LDCONST	R9	K5
      0x7C180600,  //  000E  CALL	R6	3
      0x24180D06,  //  000F  GT	R6	R6	K6
      0x781A000B,  //  0010  JMPF	R6	#001D
      0xB81A0E00,  //  0011  GETNGBL	R6	K7
      0x8C180D08,  //  0012  GETMET	R6	R6	K8
      0x8C200509,  //  0013  GETMET	R8	R2	K9
      0x5828000A,  //  0014  LDCONST	R10	K10
      0x5C2C0A00,  //  0015  MOVE	R11	R5
      0x7C200600,  //  0016  CALL	R8	3
      0x5824000B,  //  0017  LDCONST	R9	K11
      0x7C180600,  //  0018  CALL	R6	3
      0xB81A0E00,  //  0019  GETNGBL	R6	K7
      0x8C180D0C,  //  001A  GETMET	R6	R6	K12
      0x00200B0D,  //  001B  ADD	R8	R5	K13
      0x7C180400,  //  001C  CALL	R6	2
      0x7001FFEA,  //  001D  JMP		#0009
      0x5810000E,  //  001E  LDCONST	R4	K14
      0xAC100200,  //  001F  CATCH	R4	1	0
      0xB0080000,  //  0020  RAISE	2	R0	R0
      0x80000000,  //  0021  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Tapp
********************************************************************/
be_local_class(Tapp,
    0,
    NULL,
    be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_nested_key("autoexec", -618105405, 8, -1), be_const_closure(Tapp_autoexec_closure) },
        { be_nested_key("init", 380752755, 4, 0), be_const_closure(Tapp_init_closure) },
    })),
    be_str_literal("Tapp")
);

/********************************************************************
** Solidified function: _anonymous_
********************************************************************/
be_local_closure(tapp__anonymous_,   /* name */
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
    /* K0   */  be_const_class(be_class_Tapp),
    }),
    (be_nested_const_str("_anonymous_", 1957281476, 11)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[ 5]) {  /* code */
      0x58040000,  //  0000  LDCONST	R1	K0
      0xB4000000,  //  0001  CLASS	K0
      0x5C080200,  //  0002  MOVE	R2	R1
      0x7C080000,  //  0003  CALL	R2	0
      0x80040400,  //  0004  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified module: tapp
********************************************************************/
be_local_module(tapp,
    "tapp",
    be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_nested_key("init", 380752755, 4, -1), be_const_closure(tapp__anonymous__closure) },
    }))
);
BE_EXPORT_VARIABLE be_define_const_native_module(tapp);
/********************************************************************/
