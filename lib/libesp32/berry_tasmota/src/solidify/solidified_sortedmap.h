/* Solidification of sortedmap.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
// compact class 'sortedmap' ktab size: 22, total: 56 (saved 272 bytes)
static const bvalue be_ktab_class_sortedmap[22] = {
  /* K0   */  be_nested_str(_data),
  /* K1   */  be_nested_str(size),
  /* K2   */  be_nested_str(iter),
  /* K3   */  be_nested_str(_keys),
  /* K4   */  be_nested_str(contains),
  /* K5   */  be_nested_str(remove),
  /* K6   */  be_nested_str(find),
  /* K7   */  be_nested_str(string),
  /* K8   */  be_nested_str(_X7B),
  /* K9   */  be_const_int(0),
  /* K10  */  be_const_int(1),
  /* K11  */  be_nested_str(_X2C_X20),
  /* K12  */  be_nested_str(format),
  /* K13  */  be_nested_str(_X27_X25s_X27_X3A_X20),
  /* K14  */  be_nested_str(_X25s_X3A_X20),
  /* K15  */  be_nested_str(_X27_X25s_X27),
  /* K16  */  be_nested_str(stop_iteration),
  /* K17  */  be_nested_str(_X7D),
  /* K18  */  be_nested_str(_find_insert_position),
  /* K19  */  be_nested_str(insert),
  /* K20  */  be_const_int(2),
  /* K21  */  be_nested_str(push),
};


extern const bclass be_class_sortedmap;

/********************************************************************
** Solidified function: size
********************************************************************/
be_local_closure(class_sortedmap_size,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_sortedmap,     /* shared constants */
    &be_const_str_size,
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
** Solidified function: iter
********************************************************************/
be_local_closure(class_sortedmap_iter,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_sortedmap,     /* shared constants */
    &be_const_str_iter,
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040302,  //  0001  GETMET	R1	R1	K2
      0x7C040200,  //  0002  CALL	R1	1
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_by_index
********************************************************************/
be_local_closure(class_sortedmap_get_by_index,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_sortedmap,     /* shared constants */
    &be_const_str_get_by_index,
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x88080103,  //  0000  GETMBR	R2	R0	K3
      0x880C0100,  //  0001  GETMBR	R3	R0	K0
      0x94080401,  //  0002  GETIDX	R2	R2	R1
      0x94080602,  //  0003  GETIDX	R2	R3	R2
      0x80040400,  //  0004  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: remove
********************************************************************/
be_local_closure(class_sortedmap_remove,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_sortedmap,     /* shared constants */
    &be_const_str_remove,
    &be_const_str_solidified,
    ( &(const binstruction[24]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080504,  //  0001  GETMET	R2	R2	K4
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x780A0010,  //  0004  JMPF	R2	#0016
      0x88080100,  //  0005  GETMBR	R2	R0	K0
      0x8C080505,  //  0006  GETMET	R2	R2	K5
      0x5C100200,  //  0007  MOVE	R4	R1
      0x7C080400,  //  0008  CALL	R2	2
      0x88080103,  //  0009  GETMBR	R2	R0	K3
      0x8C080506,  //  000A  GETMET	R2	R2	K6
      0x5C100200,  //  000B  MOVE	R4	R1
      0x7C080400,  //  000C  CALL	R2	2
      0x4C0C0000,  //  000D  LDNIL	R3
      0x200C0403,  //  000E  NE	R3	R2	R3
      0x780E0003,  //  000F  JMPF	R3	#0014
      0x880C0103,  //  0010  GETMBR	R3	R0	K3
      0x8C0C0705,  //  0011  GETMET	R3	R3	K5
      0x5C140400,  //  0012  MOVE	R5	R2
      0x7C0C0400,  //  0013  CALL	R3	2
      0x500C0200,  //  0014  LDBOOL	R3	1	0
      0x80040600,  //  0015  RET	1	R3
      0x50080000,  //  0016  LDBOOL	R2	0	0
      0x80040400,  //  0017  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: contains
********************************************************************/
be_local_closure(class_sortedmap_contains,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_sortedmap,     /* shared constants */
    &be_const_str_contains,
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080504,  //  0001  GETMET	R2	R2	K4
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x80040400,  //  0004  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: keys
********************************************************************/
be_local_closure(class_sortedmap_keys,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_sortedmap,     /* shared constants */
    &be_const_str_keys,
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040103,  //  0000  GETMBR	R1	R0	K3
      0x8C040302,  //  0001  GETMET	R1	R1	K2
      0x7C040200,  //  0002  CALL	R1	1
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(class_sortedmap_tostring,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_sortedmap,     /* shared constants */
    &be_const_str_tostring,
    &be_const_str_solidified,
    ( &(const binstruction[60]) {  /* code */
      0xA4060E00,  //  0000  IMPORT	R1	K7
      0x58080008,  //  0001  LDCONST	R2	K8
      0x500C0200,  //  0002  LDBOOL	R3	1	0
      0x60100010,  //  0003  GETGBL	R4	G16
      0x88140103,  //  0004  GETMBR	R5	R0	K3
      0x8C140B01,  //  0005  GETMET	R5	R5	K1
      0x7C140200,  //  0006  CALL	R5	1
      0x04140B0A,  //  0007  SUB	R5	R5	K10
      0x40161205,  //  0008  CONNECT	R5	K9	R5
      0x7C100200,  //  0009  CALL	R4	1
      0xA802002B,  //  000A  EXBLK	0	#0037
      0x5C140800,  //  000B  MOVE	R5	R4
      0x7C140000,  //  000C  CALL	R5	0
      0x88180103,  //  000D  GETMBR	R6	R0	K3
      0x94180C05,  //  000E  GETIDX	R6	R6	R5
      0x881C0100,  //  000F  GETMBR	R7	R0	K0
      0x941C0E06,  //  0010  GETIDX	R7	R7	R6
      0x5C200600,  //  0011  MOVE	R8	R3
      0x74220000,  //  0012  JMPT	R8	#0014
      0x0008050B,  //  0013  ADD	R2	R2	K11
      0x500C0000,  //  0014  LDBOOL	R3	0	0
      0x60200004,  //  0015  GETGBL	R8	G4
      0x5C240C00,  //  0016  MOVE	R9	R6
      0x7C200200,  //  0017  CALL	R8	1
      0x1C201107,  //  0018  EQ	R8	R8	K7
      0x78220005,  //  0019  JMPF	R8	#0020
      0x8C20030C,  //  001A  GETMET	R8	R1	K12
      0x5828000D,  //  001B  LDCONST	R10	K13
      0x5C2C0C00,  //  001C  MOVE	R11	R6
      0x7C200600,  //  001D  CALL	R8	3
      0x00080408,  //  001E  ADD	R2	R2	R8
      0x70020006,  //  001F  JMP		#0027
      0x8C20030C,  //  0020  GETMET	R8	R1	K12
      0x5828000E,  //  0021  LDCONST	R10	K14
      0x602C0008,  //  0022  GETGBL	R11	G8
      0x5C300C00,  //  0023  MOVE	R12	R6
      0x7C2C0200,  //  0024  CALL	R11	1
      0x7C200600,  //  0025  CALL	R8	3
      0x00080408,  //  0026  ADD	R2	R2	R8
      0x60200004,  //  0027  GETGBL	R8	G4
      0x5C240E00,  //  0028  MOVE	R9	R7
      0x7C200200,  //  0029  CALL	R8	1
      0x1C201107,  //  002A  EQ	R8	R8	K7
      0x78220005,  //  002B  JMPF	R8	#0032
      0x8C20030C,  //  002C  GETMET	R8	R1	K12
      0x5828000F,  //  002D  LDCONST	R10	K15
      0x5C2C0E00,  //  002E  MOVE	R11	R7
      0x7C200600,  //  002F  CALL	R8	3
      0x00080408,  //  0030  ADD	R2	R2	R8
      0x70020003,  //  0031  JMP		#0036
      0x60200008,  //  0032  GETGBL	R8	G8
      0x5C240E00,  //  0033  MOVE	R9	R7
      0x7C200200,  //  0034  CALL	R8	1
      0x00080408,  //  0035  ADD	R2	R2	R8
      0x7001FFD3,  //  0036  JMP		#000B
      0x58100010,  //  0037  LDCONST	R4	K16
      0xAC100200,  //  0038  CATCH	R4	1	0
      0xB0080000,  //  0039  RAISE	2	R0	R0
      0x00080511,  //  003A  ADD	R2	R2	K17
      0x80040400,  //  003B  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: insert
********************************************************************/
be_local_closure(class_sortedmap_insert,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_sortedmap,     /* shared constants */
    &be_const_str_insert,
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x8C0C0704,  //  0001  GETMET	R3	R3	K4
      0x5C140200,  //  0002  MOVE	R5	R1
      0x7C0C0400,  //  0003  CALL	R3	2
      0x780E0000,  //  0004  JMPF	R3	#0006
      0x500C0001,  //  0005  LDBOOL	R3	0	1
      0x500C0200,  //  0006  LDBOOL	R3	1	0
      0x88100100,  //  0007  GETMBR	R4	R0	K0
      0x98100202,  //  0008  SETIDX	R4	R1	R2
      0x780E0009,  //  0009  JMPF	R3	#0014
      0x8C100112,  //  000A  GETMET	R4	R0	K18
      0x5C180200,  //  000B  MOVE	R6	R1
      0x7C100400,  //  000C  CALL	R4	2
      0x88140103,  //  000D  GETMBR	R5	R0	K3
      0x8C140B13,  //  000E  GETMET	R5	R5	K19
      0x5C1C0800,  //  000F  MOVE	R7	R4
      0x5C200200,  //  0010  MOVE	R8	R1
      0x7C140600,  //  0011  CALL	R5	3
      0x50140200,  //  0012  LDBOOL	R5	1	0
      0x80040A00,  //  0013  RET	1	R5
      0x50100000,  //  0014  LDBOOL	R4	0	0
      0x80040800,  //  0015  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _find_insert_position
********************************************************************/
be_local_closure(class_sortedmap__find_insert_position,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_sortedmap,     /* shared constants */
    &be_const_str__find_insert_position,
    &be_const_str_solidified,
    ( &(const binstruction[41]) {  /* code */
      0x58080009,  //  0000  LDCONST	R2	K9
      0x880C0103,  //  0001  GETMBR	R3	R0	K3
      0x8C0C0701,  //  0002  GETMET	R3	R3	K1
      0x7C0C0200,  //  0003  CALL	R3	1
      0x040C070A,  //  0004  SUB	R3	R3	K10
      0x18100403,  //  0005  LE	R4	R2	R3
      0x78120020,  //  0006  JMPF	R4	#0028
      0x60100009,  //  0007  GETGBL	R4	G9
      0x00140403,  //  0008  ADD	R5	R2	R3
      0x0C140B14,  //  0009  DIV	R5	R5	K20
      0x7C100200,  //  000A  CALL	R4	1
      0x88140103,  //  000B  GETMBR	R5	R0	K3
      0x94140A04,  //  000C  GETIDX	R5	R5	R4
      0x4C180000,  //  000D  LDNIL	R6
      0x601C0004,  //  000E  GETGBL	R7	G4
      0x5C200200,  //  000F  MOVE	R8	R1
      0x7C1C0200,  //  0010  CALL	R7	1
      0x60200004,  //  0011  GETGBL	R8	G4
      0x5C240A00,  //  0012  MOVE	R9	R5
      0x7C200200,  //  0013  CALL	R8	1
      0x1C1C0E08,  //  0014  EQ	R7	R7	R8
      0x781E0002,  //  0015  JMPF	R7	#0019
      0x241C0205,  //  0016  GT	R7	R1	R5
      0x5C180E00,  //  0017  MOVE	R6	R7
      0x70020007,  //  0018  JMP		#0021
      0x601C0008,  //  0019  GETGBL	R7	G8
      0x5C200200,  //  001A  MOVE	R8	R1
      0x7C1C0200,  //  001B  CALL	R7	1
      0x60200008,  //  001C  GETGBL	R8	G8
      0x5C240A00,  //  001D  MOVE	R9	R5
      0x7C200200,  //  001E  CALL	R8	1
      0x241C0E08,  //  001F  GT	R7	R7	R8
      0x5C180E00,  //  0020  MOVE	R6	R7
      0x781A0002,  //  0021  JMPF	R6	#0025
      0x001C090A,  //  0022  ADD	R7	R4	K10
      0x5C080E00,  //  0023  MOVE	R2	R7
      0x70020001,  //  0024  JMP		#0027
      0x041C090A,  //  0025  SUB	R7	R4	K10
      0x5C0C0E00,  //  0026  MOVE	R3	R7
      0x7001FFDC,  //  0027  JMP		#0005
      0x80040400,  //  0028  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_keys
********************************************************************/
be_local_closure(class_sortedmap_get_keys,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_sortedmap,     /* shared constants */
    &be_const_str_get_keys,
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040103,  //  0000  GETMBR	R1	R0	K3
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: item
********************************************************************/
be_local_closure(class_sortedmap_item,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_sortedmap,     /* shared constants */
    &be_const_str_item,
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x94080401,  //  0001  GETIDX	R2	R2	R1
      0x80040400,  //  0002  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: setitem
********************************************************************/
be_local_closure(class_sortedmap_setitem,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_sortedmap,     /* shared constants */
    &be_const_str_setitem,
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x8C0C0113,  //  0000  GETMET	R3	R0	K19
      0x5C140200,  //  0001  MOVE	R5	R1
      0x5C180400,  //  0002  MOVE	R6	R2
      0x7C0C0600,  //  0003  CALL	R3	3
      0x80040600,  //  0004  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: remove_by_value
********************************************************************/
be_local_closure(class_sortedmap_remove_by_value,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_sortedmap,     /* shared constants */
    &be_const_str_remove_by_value,
    &be_const_str_solidified,
    ( &(const binstruction[41]) {  /* code */
      0x60080012,  //  0000  GETGBL	R2	G18
      0x7C080000,  //  0001  CALL	R2	0
      0x600C0010,  //  0002  GETGBL	R3	G16
      0x88100103,  //  0003  GETMBR	R4	R0	K3
      0x8C100901,  //  0004  GETMET	R4	R4	K1
      0x7C100200,  //  0005  CALL	R4	1
      0x0410090A,  //  0006  SUB	R4	R4	K10
      0x40121204,  //  0007  CONNECT	R4	K9	R4
      0x7C0C0200,  //  0008  CALL	R3	1
      0xA802000B,  //  0009  EXBLK	0	#0016
      0x5C100600,  //  000A  MOVE	R4	R3
      0x7C100000,  //  000B  CALL	R4	0
      0x88140103,  //  000C  GETMBR	R5	R0	K3
      0x94140A04,  //  000D  GETIDX	R5	R5	R4
      0x88180100,  //  000E  GETMBR	R6	R0	K0
      0x94180C05,  //  000F  GETIDX	R6	R6	R5
      0x1C180C01,  //  0010  EQ	R6	R6	R1
      0x781A0002,  //  0011  JMPF	R6	#0015
      0x8C180515,  //  0012  GETMET	R6	R2	K21
      0x5C200A00,  //  0013  MOVE	R8	R5
      0x7C180400,  //  0014  CALL	R6	2
      0x7001FFF3,  //  0015  JMP		#000A
      0x580C0010,  //  0016  LDCONST	R3	K16
      0xAC0C0200,  //  0017  CATCH	R3	1	0
      0xB0080000,  //  0018  RAISE	2	R0	R0
      0x580C0009,  //  0019  LDCONST	R3	K9
      0x60100010,  //  001A  GETGBL	R4	G16
      0x5C140400,  //  001B  MOVE	R5	R2
      0x7C100200,  //  001C  CALL	R4	1
      0xA8020006,  //  001D  EXBLK	0	#0025
      0x5C140800,  //  001E  MOVE	R5	R4
      0x7C140000,  //  001F  CALL	R5	0
      0x8C180105,  //  0020  GETMET	R6	R0	K5
      0x5C200A00,  //  0021  MOVE	R8	R5
      0x7C180400,  //  0022  CALL	R6	2
      0x000C070A,  //  0023  ADD	R3	R3	K10
      0x7001FFF8,  //  0024  JMP		#001E
      0x58100010,  //  0025  LDCONST	R4	K16
      0xAC100200,  //  0026  CATCH	R4	1	0
      0xB0080000,  //  0027  RAISE	2	R0	R0
      0x80040600,  //  0028  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: clear
********************************************************************/
be_local_closure(class_sortedmap_clear,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_sortedmap,     /* shared constants */
    &be_const_str_clear,
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x60040013,  //  0000  GETGBL	R1	G19
      0x7C040000,  //  0001  CALL	R1	0
      0x90020001,  //  0002  SETMBR	R0	K0	R1
      0x60040012,  //  0003  GETGBL	R1	G18
      0x7C040000,  //  0004  CALL	R1	0
      0x90020601,  //  0005  SETMBR	R0	K3	R1
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_sortedmap_init,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_sortedmap,     /* shared constants */
    &be_const_str_init,
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x60040013,  //  0000  GETGBL	R1	G19
      0x7C040000,  //  0001  CALL	R1	0
      0x90020001,  //  0002  SETMBR	R0	K0	R1
      0x60040012,  //  0003  GETGBL	R1	G18
      0x7C040000,  //  0004  CALL	R1	0
      0x90020601,  //  0005  SETMBR	R0	K3	R1
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: find
********************************************************************/
be_local_closure(class_sortedmap_find,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_sortedmap,     /* shared constants */
    &be_const_str_find,
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x8C0C0706,  //  0001  GETMET	R3	R3	K6
      0x5C140200,  //  0002  MOVE	R5	R1
      0x5C180400,  //  0003  MOVE	R6	R2
      0x7C0C0600,  //  0004  CALL	R3	3
      0x80040600,  //  0005  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: sortedmap
********************************************************************/
be_local_class(sortedmap,
    2,
    NULL,
    be_nested_map(18,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(size, 1), be_const_closure(class_sortedmap_size_closure) },
        { be_const_key(_data, -1), be_const_var(0) },
        { be_const_key(item, -1), be_const_closure(class_sortedmap_item_closure) },
        { be_const_key(remove, -1), be_const_closure(class_sortedmap_remove_closure) },
        { be_const_key(contains, 2), be_const_closure(class_sortedmap_contains_closure) },
        { be_const_key(keys, -1), be_const_closure(class_sortedmap_keys_closure) },
        { be_const_key(get_by_index, 17), be_const_closure(class_sortedmap_get_by_index_closure) },
        { be_const_key(tostring, -1), be_const_closure(class_sortedmap_tostring_closure) },
        { be_const_key(insert, -1), be_const_closure(class_sortedmap_insert_closure) },
        { be_const_key(_find_insert_position, -1), be_const_closure(class_sortedmap__find_insert_position_closure) },
        { be_const_key(get_keys, -1), be_const_closure(class_sortedmap_get_keys_closure) },
        { be_const_key(remove_by_value, -1), be_const_closure(class_sortedmap_remove_by_value_closure) },
        { be_const_key(clear, -1), be_const_closure(class_sortedmap_clear_closure) },
        { be_const_key(iter, 11), be_const_closure(class_sortedmap_iter_closure) },
        { be_const_key(setitem, 12), be_const_closure(class_sortedmap_setitem_closure) },
        { be_const_key(init, -1), be_const_closure(class_sortedmap_init_closure) },
        { be_const_key(find, -1), be_const_closure(class_sortedmap_find_closure) },
        { be_const_key(_keys, -1), be_const_var(1) },
    })),
    (bstring*) &be_const_str_sortedmap
);
/********************************************************************/
/* End of solidification */
