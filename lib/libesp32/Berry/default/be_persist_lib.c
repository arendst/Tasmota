/********************************************************************
 * Tasmota lib
 * 
 * To use: `import power`
 * 
 * read power values
 *******************************************************************/
#include "be_constobj.h"

/********************************************************************
** Solidified function: setmember
********************************************************************/
be_local_closure(setmember,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    3,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_string("p", -183745553, 1),
    /* K1   */  be_nested_string("dirty", -1627386213, 5),
    }),
    (be_nested_const_str("setmember", 1432909441, 9)),
    (be_nested_const_str("stdin", -1529146723, 5)),
    ( &(const binstruction[ 5]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x980C0202,  //  0001  SETIDX	R3	R1	R2
      0x500C0200,  //  0002  LDBOOL	R3	1	0
      0x90020203,  //  0003  SETMBR	R0	K1	R3
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: save
********************************************************************/
be_local_closure(save,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_string("json", 916562499, 4),
    /* K1   */  be_nested_string("filename", 1058081160, 8),
    /* K2   */  be_nested_string("w", -234078410, 1),
    /* K3   */  be_nested_string("write", -1104765092, 5),
    /* K4   */  be_nested_string("dump", -631966073, 4),
    /* K5   */  be_nested_string("p", -183745553, 1),
    /* K6   */  be_nested_string("close", 667630371, 5),
    /* K7   */  be_nested_string("dirty", -1627386213, 5),
    }),
    (be_nested_const_str("save", -855671224, 4)),
    (be_nested_const_str("stdin", -1529146723, 5)),
    ( &(const binstruction[30]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x4C080000,  //  0001  LDNIL	R2
      0xA802000D,  //  0002  EXBLK	0	#0011
      0x600C0011,  //  0003  GETGBL	R3	G17
      0x88100101,  //  0004  GETMBR	R4	R0	K1
      0x58140002,  //  0005  LDCONST	R5	K2
      0x7C0C0400,  //  0006  CALL	R3	2
      0x5C080600,  //  0007  MOVE	R2	R3
      0x8C0C0503,  //  0008  GETMET	R3	R2	K3
      0x8C140304,  //  0009  GETMET	R5	R1	K4
      0x881C0105,  //  000A  GETMBR	R7	R0	K5
      0x7C140400,  //  000B  CALL	R5	2
      0x7C0C0400,  //  000C  CALL	R3	2
      0x8C0C0506,  //  000D  GETMET	R3	R2	K6
      0x7C0C0200,  //  000E  CALL	R3	1
      0xA8040001,  //  000F  EXBLK	1	1
      0x70020009,  //  0010  JMP		#001B
      0xAC0C0002,  //  0011  CATCH	R3	0	2
      0x70020006,  //  0012  JMP		#001A
      0x4C140000,  //  0013  LDNIL	R5
      0x20140405,  //  0014  NE	R5	R2	R5
      0x78160001,  //  0015  JMPF	R5	#0018
      0x8C140506,  //  0016  GETMET	R5	R2	K6
      0x7C140200,  //  0017  CALL	R5	1
      0xB0040604,  //  0018  RAISE	1	R3	R4
      0x70020000,  //  0019  JMP		#001B
      0xB0080000,  //  001A  RAISE	2	R0	R0
      0x500C0000,  //  001B  LDBOOL	R3	0	0
      0x90020E03,  //  001C  SETMBR	R0	K7	R3
      0x80000000,  //  001D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(init,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_string("filename", 1058081160, 8),
    /* K1   */  be_nested_string("_persist.json", 2008425138, 13),
    /* K2   */  be_nested_string("p", -183745553, 1),
    /* K3   */  be_nested_string("copy", -446502332, 4),
    /* K4   */  be_nested_string("load", -435725847, 4),
    /* K5   */  be_nested_string("dirty", -1627386213, 5),
    }),
    (be_nested_const_str("init", 380752755, 4)),
    (be_nested_const_str("stdin", -1529146723, 5)),
    ( &(const binstruction[20]) {  /* code */
      0x90020101,  //  0000  SETMBR	R0	K0	K1
      0x6008000F,  //  0001  GETGBL	R2	G15
      0x5C0C0200,  //  0002  MOVE	R3	R1
      0x60100013,  //  0003  GETGBL	R4	G19
      0x7C080400,  //  0004  CALL	R2	2
      0x780A0003,  //  0005  JMPF	R2	#000A
      0x8C080303,  //  0006  GETMET	R2	R1	K3
      0x7C080200,  //  0007  CALL	R2	1
      0x90020402,  //  0008  SETMBR	R0	K2	R2
      0x70020002,  //  0009  JMP		#000D
      0x60080013,  //  000A  GETGBL	R2	G19
      0x7C080000,  //  000B  CALL	R2	0
      0x90020402,  //  000C  SETMBR	R0	K2	R2
      0x8C080104,  //  000D  GETMET	R2	R0	K4
      0x88100102,  //  000E  GETMBR	R4	R0	K2
      0x88140100,  //  000F  GETMBR	R5	R0	K0
      0x7C080600,  //  0010  CALL	R2	3
      0x50080000,  //  0011  LDBOOL	R2	0	0
      0x90020A02,  //  0012  SETMBR	R0	K5	R2
      0x80000000,  //  0013  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: zero
********************************************************************/
be_local_closure(zero,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_string("p", -183745553, 1),
    /* K1   */  be_nested_string("dirty", -1627386213, 5),
    }),
    (be_nested_const_str("zero", -1955600541, 4)),
    (be_nested_const_str("stdin", -1529146723, 5)),
    ( &(const binstruction[ 6]) {  /* code */
      0x60040013,  //  0000  GETGBL	R1	G19
      0x7C040000,  //  0001  CALL	R1	0
      0x90020001,  //  0002  SETMBR	R0	K0	R1
      0x50040200,  //  0003  LDBOOL	R1	1	0
      0x90020201,  //  0004  SETMBR	R0	K1	R1
      0x80000000,  //  0005  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(tostring,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_string("string", 398550328, 6),
    /* K1   */  be_nested_string("format", -1180859054, 6),
    /* K2   */  be_nested_string("<instance: %s(%s)>", 1553432777, 18),
    /* K3   */  be_nested_string("p", -183745553, 1),
    }),
    (be_nested_const_str("tostring", -1995258651, 8)),
    (be_nested_const_str("stdin", -1529146723, 5)),
    ( &(const binstruction[11]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080301,  //  0001  GETMET	R2	R1	K1
      0x58100002,  //  0002  LDCONST	R4	K2
      0x60140005,  //  0003  GETGBL	R5	G5
      0x5C180000,  //  0004  MOVE	R6	R0
      0x7C140200,  //  0005  CALL	R5	1
      0x60180008,  //  0006  GETGBL	R6	G8
      0x881C0103,  //  0007  GETMBR	R7	R0	K3
      0x7C180200,  //  0008  CALL	R6	1
      0x7C080800,  //  0009  CALL	R2	4
      0x80040400,  //  000A  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: member
********************************************************************/
be_local_closure(member,   /* name */
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
    /* K0   */  be_nested_string("p", -183745553, 1),
    /* K1   */  be_nested_string("find", -1108310694, 4),
    }),
    (be_nested_const_str("member", 719708611, 6)),
    (be_nested_const_str("stdin", -1529146723, 5)),
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
** Solidified function: load
********************************************************************/
be_local_closure(load,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[10]) {     /* constants */
    /* K0   */  be_nested_string("json", 916562499, 4),
    /* K1   */  be_nested_string("path", -2071507658, 4),
    /* K2   */  be_nested_string("exists", 1002329533, 6),
    /* K3   */  be_nested_string("filename", 1058081160, 8),
    /* K4   */  be_nested_string("r", -150190315, 1),
    /* K5   */  be_nested_string("load", -435725847, 4),
    /* K6   */  be_nested_string("read", -824204347, 4),
    /* K7   */  be_nested_string("close", 667630371, 5),
    /* K8   */  be_nested_string("p", -183745553, 1),
    /* K9   */  be_nested_string("dirty", -1627386213, 5),
    }),
    (be_nested_const_str("load", -435725847, 4)),
    (be_nested_const_str("stdin", -1529146723, 5)),
    ( &(const binstruction[37]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0x4C0C0000,  //  0002  LDNIL	R3
      0x4C100000,  //  0003  LDNIL	R4
      0x8C140502,  //  0004  GETMET	R5	R2	K2
      0x881C0103,  //  0005  GETMBR	R7	R0	K3
      0x7C140400,  //  0006  CALL	R5	2
      0x7816001B,  //  0007  JMPF	R5	#0024
      0xA802000D,  //  0008  EXBLK	0	#0017
      0x60140011,  //  0009  GETGBL	R5	G17
      0x88180103,  //  000A  GETMBR	R6	R0	K3
      0x581C0004,  //  000B  LDCONST	R7	K4
      0x7C140400,  //  000C  CALL	R5	2
      0x5C0C0A00,  //  000D  MOVE	R3	R5
      0x8C140305,  //  000E  GETMET	R5	R1	K5
      0x8C1C0706,  //  000F  GETMET	R7	R3	K6
      0x7C1C0200,  //  0010  CALL	R7	1
      0x7C140400,  //  0011  CALL	R5	2
      0x5C100A00,  //  0012  MOVE	R4	R5
      0x8C140707,  //  0013  GETMET	R5	R3	K7
      0x7C140200,  //  0014  CALL	R5	1
      0xA8040001,  //  0015  EXBLK	1	1
      0x70020009,  //  0016  JMP		#0021
      0xAC140002,  //  0017  CATCH	R5	0	2
      0x70020006,  //  0018  JMP		#0020
      0x4C1C0000,  //  0019  LDNIL	R7
      0x201C0607,  //  001A  NE	R7	R3	R7
      0x781E0001,  //  001B  JMPF	R7	#001E
      0x8C1C0707,  //  001C  GETMET	R7	R3	K7
      0x7C1C0200,  //  001D  CALL	R7	1
      0xB0040A06,  //  001E  RAISE	1	R5	R6
      0x70020000,  //  001F  JMP		#0021
      0xB0080000,  //  0020  RAISE	2	R0	R0
      0x90021004,  //  0021  SETMBR	R0	K8	R4
      0x50140000,  //  0022  LDBOOL	R5	0	0
      0x90021205,  //  0023  SETMBR	R0	K9	R5
      0x80000000,  //  0024  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Persist
********************************************************************/
be_local_class(Persist,
    3,
    NULL,
    be_nested_map(10,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_nested_key("filename", 1058081160, 8, -1), be_const_var(0) },
        { be_nested_key("setmember", 1432909441, 9, 7), be_const_closure(setmember_closure) },
        { be_nested_key("save", -855671224, 4, -1), be_const_closure(save_closure) },
        { be_nested_key("dirty", -1627386213, 5, 8), be_const_var(2) },
        { be_nested_key("init", 380752755, 4, -1), be_const_closure(init_closure) },
        { be_nested_key("zero", -1955600541, 4, 6), be_const_closure(zero_closure) },
        { be_nested_key("tostring", -1995258651, 8, 4), be_const_closure(tostring_closure) },
        { be_nested_key("member", 719708611, 6, -1), be_const_closure(member_closure) },
        { be_nested_key("p", -183745553, 1, -1), be_const_var(1) },
        { be_nested_key("load", -435725847, 4, -1), be_const_closure(load_closure) },
    })),
    (be_nested_const_str("Persist", 1456346147, 7))
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
    /* K0   */  be_const_class(be_class_Persist),
    }),
    (be_nested_const_str("_anonymous_", 1957281476, 11)),
    (be_nested_const_str("stdin", -1529146723, 5)),
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

/* @const_object_info_begin
module persist (scope: global) {
    init, closure(_anonymous__closure)
}
@const_object_info_end */
#include "../generate/be_fixed_persist.h"
