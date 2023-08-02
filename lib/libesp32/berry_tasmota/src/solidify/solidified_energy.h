/* Solidification of energy.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(energy_init,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str(global),
    /* K1   */  be_nested_str(_energy),
    }),
    &be_const_str_init,
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x4C080000,  //  0001  LDNIL	R2
      0x90060202,  //  0002  SETMBR	R1	K1	R2
      0x80040000,  //  0003  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _deref
********************************************************************/
be_local_closure(energy__deref,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    0,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str(global),
    /* K1   */  be_nested_str(_energy),
    /* K2   */  be_nested_str(introspect),
    /* K3   */  be_nested_str(energy),
    /* K4   */  be_nested_str(_ptr),
    /* K5   */  be_nested_str(get),
    /* K6   */  be_const_int(0),
    /* K7   */  be_nested_str(energy_struct),
    /* K8   */  be_nested_str(toptr),
    }),
    &be_const_str__deref,
    &be_const_str_solidified,
    ( &(const binstruction[25]) {  /* code */
      0xB8020000,  //  0000  GETNGBL	R0	K0
      0x88000101,  //  0001  GETMBR	R0	R0	K1
      0x4C040000,  //  0002  LDNIL	R1
      0x1C000001,  //  0003  EQ	R0	R0	R1
      0x78020012,  //  0004  JMPF	R0	#0018
      0xA4020400,  //  0005  IMPORT	R0	K2
      0x60040015,  //  0006  GETGBL	R1	G21
      0xB80A0600,  //  0007  GETNGBL	R2	K3
      0x88080504,  //  0008  GETMBR	R2	R2	K4
      0x540E0003,  //  0009  LDINT	R3	4
      0x7C040400,  //  000A  CALL	R1	2
      0x8C040305,  //  000B  GETMET	R1	R1	K5
      0x580C0006,  //  000C  LDCONST	R3	K6
      0x54120003,  //  000D  LDINT	R4	4
      0x7C040600,  //  000E  CALL	R1	3
      0x20080306,  //  000F  NE	R2	R1	K6
      0x780A0006,  //  0010  JMPF	R2	#0018
      0xB80A0000,  //  0011  GETNGBL	R2	K0
      0xB80E0E00,  //  0012  GETNGBL	R3	K7
      0x8C100108,  //  0013  GETMET	R4	R0	K8
      0x5C180200,  //  0014  MOVE	R6	R1
      0x7C100400,  //  0015  CALL	R4	2
      0x7C0C0200,  //  0016  CALL	R3	1
      0x900A0203,  //  0017  SETMBR	R2	K1	R3
      0x80000000,  //  0018  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: read
********************************************************************/
be_local_closure(energy_read,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    0,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str(energy),
    /* K1   */  be_nested_str(_deref),
    /* K2   */  be_nested_str(global),
    /* K3   */  be_nested_str(_energy),
    /* K4   */  be_nested_str(tomap),
    }),
    &be_const_str_read,
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0xB8020000,  //  0000  GETNGBL	R0	K0
      0x8C000101,  //  0001  GETMET	R0	R0	K1
      0x7C000200,  //  0002  CALL	R0	1
      0xB8020400,  //  0003  GETNGBL	R0	K2
      0x88000103,  //  0004  GETMBR	R0	R0	K3
      0x78020003,  //  0005  JMPF	R0	#000A
      0xB8020600,  //  0006  GETNGBL	R0	K3
      0x8C000104,  //  0007  GETMET	R0	R0	K4
      0x7C000200,  //  0008  CALL	R0	1
      0x80040000,  //  0009  RET	1	R0
      0x80000000,  //  000A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: member
********************************************************************/
be_local_closure(energy_member,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str(energy),
    /* K1   */  be_nested_str(_deref),
    /* K2   */  be_nested_str(global),
    /* K3   */  be_nested_str(_energy),
    }),
    &be_const_str_member,
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x7C040200,  //  0002  CALL	R1	1
      0xB8060400,  //  0003  GETNGBL	R1	K2
      0x88040303,  //  0004  GETMBR	R1	R1	K3
      0x78060002,  //  0005  JMPF	R1	#0009
      0xB8060600,  //  0006  GETNGBL	R1	K3
      0x88040200,  //  0007  GETMBR	R1	R1	R0
      0x80040200,  //  0008  RET	1	R1
      0x80000000,  //  0009  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: setmember
********************************************************************/
be_local_closure(energy_setmember,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str(energy),
    /* K1   */  be_nested_str(_deref),
    /* K2   */  be_nested_str(global),
    /* K3   */  be_nested_str(_energy),
    }),
    &be_const_str_setmember,
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0xB80A0000,  //  0000  GETNGBL	R2	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x7C080200,  //  0002  CALL	R2	1
      0xB80A0400,  //  0003  GETNGBL	R2	K2
      0x88080503,  //  0004  GETMBR	R2	R2	K3
      0x780A0001,  //  0005  JMPF	R2	#0008
      0xB80A0600,  //  0006  GETNGBL	R2	K3
      0x90080001,  //  0007  SETMBR	R2	R0	R1
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/

/********************************************************************/
/* End of solidification */
