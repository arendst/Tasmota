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
be_local_closure(init,   /* name */
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
    /* K0   */  be_nested_string("tasmota", 424643812, 7),
    /* K1   */  be_nested_string("add_driver", 1654458371, 10),
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
be_local_closure(autoexec,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[13]) {     /* constants */
    /* K0   */  be_nested_string("path", -2071507658, 4),
    /* K1   */  be_nested_string("string", 398550328, 6),
    /* K2   */  be_nested_string("listdir", 2005220720, 7),
    /* K3   */  be_nested_string("/", 705468254, 1),
    /* K4   */  be_nested_string("find", -1108310694, 4),
    /* K5   */  be_nested_string(".tapp", 1363391594, 5),
    /* K6   */  be_const_int(0),
    /* K7   */  be_nested_string("format", -1180859054, 6),
    /* K8   */  be_nested_string("TAP: found Tasmota App '%s'", -1651814898, 27),
    /* K9   */  be_nested_string("tasmota", 424643812, 7),
    /* K10  */  be_nested_string("load", -435725847, 4),
    /* K11  */  be_nested_string("#autoexec.be", 1181757091, 12),
    /* K12  */  be_nested_string("stop_iteration", -121173395, 14),
    }),
    (be_nested_const_str("autoexec", -618105405, 8)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[32]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0x8C0C0302,  //  0002  GETMET	R3	R1	K2
      0x58140003,  //  0003  LDCONST	R5	K3
      0x7C0C0400,  //  0004  CALL	R3	2
      0x60100010,  //  0005  GETGBL	R4	G16
      0x5C140600,  //  0006  MOVE	R5	R3
      0x7C100200,  //  0007  CALL	R4	1
      0xA8020012,  //  0008  EXBLK	0	#001C
      0x5C140800,  //  0009  MOVE	R5	R4
      0x7C140000,  //  000A  CALL	R5	0
      0x8C180504,  //  000B  GETMET	R6	R2	K4
      0x5C200A00,  //  000C  MOVE	R8	R5
      0x58240005,  //  000D  LDCONST	R9	K5
      0x7C180600,  //  000E  CALL	R6	3
      0x24180D06,  //  000F  GT	R6	R6	K6
      0x781A0009,  //  0010  JMPF	R6	#001B
      0x60180001,  //  0011  GETGBL	R6	G1
      0x8C1C0507,  //  0012  GETMET	R7	R2	K7
      0x58240008,  //  0013  LDCONST	R9	K8
      0x5C280A00,  //  0014  MOVE	R10	R5
      0x7C1C0600,  //  0015  CALL	R7	3
      0x7C180200,  //  0016  CALL	R6	1
      0xB81A1200,  //  0017  GETNGBL	R6	K9
      0x8C180D0A,  //  0018  GETMET	R6	R6	K10
      0x00200B0B,  //  0019  ADD	R8	R5	K11
      0x7C180400,  //  001A  CALL	R6	2
      0x7001FFEC,  //  001B  JMP		#0009
      0x5810000C,  //  001C  LDCONST	R4	K12
      0xAC100200,  //  001D  CATCH	R4	1	0
      0xB0080000,  //  001E  RAISE	2	R0	R0
      0x80000000,  //  001F  RET	0
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
        { be_nested_key("init", 380752755, 4, -1), be_const_closure(init_closure) },
        { be_nested_key("autoexec", -618105405, 8, 0), be_const_closure(autoexec_closure) },
    })),
    (be_nested_const_str("Tapp", 2012315062, 4))
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
        { be_nested_key("init", 380752755, 4, -1), be_const_closure(_anonymous__closure) },
    }))
);
BE_EXPORT_VARIABLE be_define_const_native_module(tapp);
/********************************************************************/
