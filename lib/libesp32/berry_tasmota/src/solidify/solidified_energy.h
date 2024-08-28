/* Solidification of energy.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
// compact class 'bytes_array' ktab size: 18, total: 34 (saved 128 bytes)
static const bvalue be_ktab_class_bytes_array[18] = {
  /* K0   */  be_nested_str(item_type),
  /* K1   */  be_nested_str(sz),
  /* K2   */  be_const_int(0),
  /* K3   */  be_nested_str(value_X20error),
  /* K4   */  be_nested_str(unsupported_X20item_type_X20number),
  /* K5   */  be_nested_str(item_size),
  /* K6   */  be_nested_str(init),
  /* K7   */  be_nested_str(_X5B),
  /* K8   */  be_nested_str(_X2C),
  /* K9   */  be_nested_str(getfloat),
  /* K10  */  be_nested_str(get),
  /* K11  */  be_const_int(1),
  /* K12  */  be_nested_str(_X5D),
  /* K13  */  be_nested_str(index_error),
  /* K14  */  be_nested_str(list_X20index_X20out_X20of_X20range),
  /* K15  */  be_nested_str(list_X20assignment_X20index_X20out_X20of_X20range),
  /* K16  */  be_nested_str(setfloat),
  /* K17  */  be_nested_str(set),
};


extern const bclass be_class_bytes_array;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_bytes_array_init,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_bytes_array,     /* shared constants */
    &be_const_str_init,
    &be_const_str_solidified,
    ( &(const binstruction[35]) {  /* code */
      0x90020002,  //  0000  SETMBR	R0	K0	R2
      0x90020203,  //  0001  SETMBR	R0	K1	R3
      0x4C100000,  //  0002  LDNIL	R4
      0x5415FFFB,  //  0003  LDINT	R5	-4
      0x28140405,  //  0004  GE	R5	R2	R5
      0x7816000B,  //  0005  JMPF	R5	#0012
      0x54160003,  //  0006  LDINT	R5	4
      0x18140405,  //  0007  LE	R5	R2	R5
      0x78160008,  //  0008  JMPF	R5	#0012
      0x20140502,  //  0009  NE	R5	R2	K2
      0x78160006,  //  000A  JMPF	R5	#0012
      0x24140502,  //  000B  GT	R5	R2	K2
      0x78160001,  //  000C  JMPF	R5	#000F
      0x5C140400,  //  000D  MOVE	R5	R2
      0x70020000,  //  000E  JMP		#0010
      0x44140400,  //  000F  NEG	R5	R2
      0x5C100A00,  //  0010  MOVE	R4	R5
      0x70020005,  //  0011  JMP		#0018
      0x54160004,  //  0012  LDINT	R5	5
      0x1C140405,  //  0013  EQ	R5	R2	R5
      0x78160001,  //  0014  JMPF	R5	#0017
      0x54120003,  //  0015  LDINT	R4	4
      0x70020000,  //  0016  JMP		#0018
      0xB0060704,  //  0017  RAISE	1	K3	K4
      0x90020A04,  //  0018  SETMBR	R0	K5	R4
      0x60140003,  //  0019  GETGBL	R5	G3
      0x5C180000,  //  001A  MOVE	R6	R0
      0x7C140200,  //  001B  CALL	R5	1
      0x8C140B06,  //  001C  GETMET	R5	R5	K6
      0x5C1C0200,  //  001D  MOVE	R7	R1
      0x88200105,  //  001E  GETMBR	R8	R0	K5
      0x88240101,  //  001F  GETMBR	R9	R0	K1
      0x08201009,  //  0020  MUL	R8	R8	R9
      0x7C140600,  //  0021  CALL	R5	3
      0x80000000,  //  0022  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: size
********************************************************************/
be_local_closure(class_bytes_array_size,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_bytes_array,     /* shared constants */
    &be_const_str_size,
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040101,  //  0000  GETMBR	R1	R0	K1
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(class_bytes_array_tostring,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_bytes_array,     /* shared constants */
    &be_const_str_tostring,
    &be_const_str_solidified,
    ( &(const binstruction[32]) {  /* code */
      0x58040007,  //  0000  LDCONST	R1	K7
      0x58080002,  //  0001  LDCONST	R2	K2
      0x880C0101,  //  0002  GETMBR	R3	R0	K1
      0x140C0403,  //  0003  LT	R3	R2	R3
      0x780E0018,  //  0004  JMPF	R3	#001E
      0x240C0502,  //  0005  GT	R3	R2	K2
      0x780E0000,  //  0006  JMPF	R3	#0008
      0x00040308,  //  0007  ADD	R1	R1	K8
      0x880C0100,  //  0008  GETMBR	R3	R0	K0
      0x54120004,  //  0009  LDINT	R4	5
      0x1C0C0604,  //  000A  EQ	R3	R3	R4
      0x780E0007,  //  000B  JMPF	R3	#0014
      0x600C0008,  //  000C  GETGBL	R3	G8
      0x8C100109,  //  000D  GETMET	R4	R0	K9
      0x88180105,  //  000E  GETMBR	R6	R0	K5
      0x08180406,  //  000F  MUL	R6	R2	R6
      0x7C100400,  //  0010  CALL	R4	2
      0x7C0C0200,  //  0011  CALL	R3	1
      0x00040203,  //  0012  ADD	R1	R1	R3
      0x70020007,  //  0013  JMP		#001C
      0x600C0008,  //  0014  GETGBL	R3	G8
      0x8C10010A,  //  0015  GETMET	R4	R0	K10
      0x88180105,  //  0016  GETMBR	R6	R0	K5
      0x08180406,  //  0017  MUL	R6	R2	R6
      0x881C0100,  //  0018  GETMBR	R7	R0	K0
      0x7C100600,  //  0019  CALL	R4	3
      0x7C0C0200,  //  001A  CALL	R3	1
      0x00040203,  //  001B  ADD	R1	R1	R3
      0x0008050B,  //  001C  ADD	R2	R2	K11
      0x7001FFE3,  //  001D  JMP		#0002
      0x0004030C,  //  001E  ADD	R1	R1	K12
      0x80040200,  //  001F  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: item
********************************************************************/
be_local_closure(class_bytes_array_item,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_bytes_array,     /* shared constants */
    &be_const_str_item,
    &be_const_str_solidified,
    ( &(const binstruction[27]) {  /* code */
      0x14080302,  //  0000  LT	R2	R1	K2
      0x780A0001,  //  0001  JMPF	R2	#0004
      0x88080101,  //  0002  GETMBR	R2	R0	K1
      0x00040202,  //  0003  ADD	R1	R1	R2
      0x14080302,  //  0004  LT	R2	R1	K2
      0x740A0002,  //  0005  JMPT	R2	#0009
      0x88080101,  //  0006  GETMBR	R2	R0	K1
      0x28080202,  //  0007  GE	R2	R1	R2
      0x780A0000,  //  0008  JMPF	R2	#000A
      0xB0061B0E,  //  0009  RAISE	1	K13	K14
      0x88080100,  //  000A  GETMBR	R2	R0	K0
      0x540E0004,  //  000B  LDINT	R3	5
      0x1C080403,  //  000C  EQ	R2	R2	R3
      0x780A0005,  //  000D  JMPF	R2	#0014
      0x8C080109,  //  000E  GETMET	R2	R0	K9
      0x88100105,  //  000F  GETMBR	R4	R0	K5
      0x08100204,  //  0010  MUL	R4	R1	R4
      0x7C080400,  //  0011  CALL	R2	2
      0x80040400,  //  0012  RET	1	R2
      0x70020005,  //  0013  JMP		#001A
      0x8C08010A,  //  0014  GETMET	R2	R0	K10
      0x88100105,  //  0015  GETMBR	R4	R0	K5
      0x08100204,  //  0016  MUL	R4	R1	R4
      0x88140100,  //  0017  GETMBR	R5	R0	K0
      0x7C080600,  //  0018  CALL	R2	3
      0x80040400,  //  0019  RET	1	R2
      0x80000000,  //  001A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: setitem
********************************************************************/
be_local_closure(class_bytes_array_setitem,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_bytes_array,     /* shared constants */
    &be_const_str_setitem,
    &be_const_str_solidified,
    ( &(const binstruction[29]) {  /* code */
      0x140C0302,  //  0000  LT	R3	R1	K2
      0x780E0001,  //  0001  JMPF	R3	#0004
      0x880C0101,  //  0002  GETMBR	R3	R0	K1
      0x00040203,  //  0003  ADD	R1	R1	R3
      0x140C0302,  //  0004  LT	R3	R1	K2
      0x740E0002,  //  0005  JMPT	R3	#0009
      0x880C0101,  //  0006  GETMBR	R3	R0	K1
      0x280C0203,  //  0007  GE	R3	R1	R3
      0x780E0000,  //  0008  JMPF	R3	#000A
      0xB0061B0F,  //  0009  RAISE	1	K13	K15
      0x880C0100,  //  000A  GETMBR	R3	R0	K0
      0x54120004,  //  000B  LDINT	R4	5
      0x1C0C0604,  //  000C  EQ	R3	R3	R4
      0x780E0006,  //  000D  JMPF	R3	#0015
      0x8C0C0110,  //  000E  GETMET	R3	R0	K16
      0x88140105,  //  000F  GETMBR	R5	R0	K5
      0x08140205,  //  0010  MUL	R5	R1	R5
      0x5C180400,  //  0011  MOVE	R6	R2
      0x7C0C0600,  //  0012  CALL	R3	3
      0x80040600,  //  0013  RET	1	R3
      0x70020006,  //  0014  JMP		#001C
      0x8C0C0111,  //  0015  GETMET	R3	R0	K17
      0x88140105,  //  0016  GETMBR	R5	R0	K5
      0x08140205,  //  0017  MUL	R5	R1	R5
      0x88180100,  //  0018  GETMBR	R6	R0	K0
      0x5C1C0400,  //  0019  MOVE	R7	R2
      0x7C0C0800,  //  001A  CALL	R3	4
      0x80040600,  //  001B  RET	1	R3
      0x80000000,  //  001C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: bytes_array
********************************************************************/
extern const bclass be_class_bytes;
be_local_class(bytes_array,
    3,
    &be_class_bytes,
    be_nested_map(8,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(sz, -1), be_const_var(1) },
        { be_const_key(setitem, -1), be_const_closure(class_bytes_array_setitem_closure) },
        { be_const_key(item_size, -1), be_const_var(2) },
        { be_const_key(init, 7), be_const_closure(class_bytes_array_init_closure) },
        { be_const_key(size, 1), be_const_closure(class_bytes_array_size_closure) },
        { be_const_key(tostring, -1), be_const_closure(class_bytes_array_tostring_closure) },
        { be_const_key(item, -1), be_const_closure(class_bytes_array_item_closure) },
        { be_const_key(item_type, -1), be_const_var(0) },
    })),
    (bstring*) &be_const_str_bytes_array
);

extern const bclass be_class_energy_phases_float;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_energy_phases_float_init,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str(energy),
    /* K1   */  be_nested_str(init),
    /* K2   */  be_nested_str(_phases),
    }),
    &be_const_str_init,
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x600C0003,  //  0001  GETGBL	R3	G3
      0x5C100000,  //  0002  MOVE	R4	R0
      0x7C0C0200,  //  0003  CALL	R3	1
      0x8C0C0701,  //  0004  GETMET	R3	R3	K1
      0x5C140200,  //  0005  MOVE	R5	R1
      0x541A0004,  //  0006  LDINT	R6	5
      0x881C0502,  //  0007  GETMBR	R7	R2	K2
      0x7C0C0800,  //  0008  CALL	R3	4
      0x80000000,  //  0009  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: energy_phases_float
********************************************************************/
extern const bclass be_class_bytes_array;
be_local_class(energy_phases_float,
    0,
    &be_class_bytes_array,
    be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(init, -1), be_const_closure(class_energy_phases_float_init_closure) },
    })),
    (bstring*) &be_const_str_energy_phases_float
);

extern const bclass be_class_energy_phases_int32;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_energy_phases_int32_init,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str(energy),
    /* K1   */  be_nested_str(init),
    /* K2   */  be_nested_str(_phases),
    }),
    &be_const_str_init,
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x600C0003,  //  0001  GETGBL	R3	G3
      0x5C100000,  //  0002  MOVE	R4	R0
      0x7C0C0200,  //  0003  CALL	R3	1
      0x8C0C0701,  //  0004  GETMET	R3	R3	K1
      0x5C140200,  //  0005  MOVE	R5	R1
      0x541A0003,  //  0006  LDINT	R6	4
      0x881C0502,  //  0007  GETMBR	R7	R2	K2
      0x7C0C0800,  //  0008  CALL	R3	4
      0x80000000,  //  0009  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: energy_phases_int32
********************************************************************/
extern const bclass be_class_bytes_array;
be_local_class(energy_phases_int32,
    0,
    &be_class_bytes_array,
    be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(init, -1), be_const_closure(class_energy_phases_int32_init_closure) },
    })),
    (bstring*) &be_const_str_energy_phases_int32
);

extern const bclass be_class_energy_phases_uint8;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_energy_phases_uint8_init,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str(energy),
    /* K1   */  be_nested_str(init),
    /* K2   */  be_const_int(1),
    /* K3   */  be_nested_str(_phases),
    }),
    &be_const_str_init,
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x600C0003,  //  0001  GETGBL	R3	G3
      0x5C100000,  //  0002  MOVE	R4	R0
      0x7C0C0200,  //  0003  CALL	R3	1
      0x8C0C0701,  //  0004  GETMET	R3	R3	K1
      0x5C140200,  //  0005  MOVE	R5	R1
      0x58180002,  //  0006  LDCONST	R6	K2
      0x881C0503,  //  0007  GETMBR	R7	R2	K3
      0x7C0C0800,  //  0008  CALL	R3	4
      0x80000000,  //  0009  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: energy_phases_uint8
********************************************************************/
extern const bclass be_class_bytes_array;
be_local_class(energy_phases_uint8,
    0,
    &be_class_bytes_array,
    be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(init, -1), be_const_closure(class_energy_phases_uint8_init_closure) },
    })),
    (bstring*) &be_const_str_energy_phases_uint8
);

extern const bclass be_class_energy_phases_uint16;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_energy_phases_uint16_init,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str(energy),
    /* K1   */  be_nested_str(init),
    /* K2   */  be_const_int(2),
    /* K3   */  be_nested_str(_phases),
    }),
    &be_const_str_init,
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x600C0003,  //  0001  GETGBL	R3	G3
      0x5C100000,  //  0002  MOVE	R4	R0
      0x7C0C0200,  //  0003  CALL	R3	1
      0x8C0C0701,  //  0004  GETMET	R3	R3	K1
      0x5C140200,  //  0005  MOVE	R5	R1
      0x58180002,  //  0006  LDCONST	R6	K2
      0x881C0503,  //  0007  GETMBR	R7	R2	K3
      0x7C0C0800,  //  0008  CALL	R3	4
      0x80000000,  //  0009  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: energy_phases_uint16
********************************************************************/
extern const bclass be_class_bytes_array;
be_local_class(energy_phases_uint16,
    0,
    &be_class_bytes_array,
    be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(init, -1), be_const_closure(class_energy_phases_uint16_init_closure) },
    })),
    (bstring*) &be_const_str_energy_phases_uint16
);

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(module_energy_init,   /* name */
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
be_local_closure(module_energy__deref,   /* name */
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
be_local_closure(module_energy_read,   /* name */
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
be_local_closure(module_energy_member,   /* name */
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
be_local_closure(module_energy_setmember,   /* name */
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


/********************************************************************
** Solidified function: tomap
********************************************************************/
be_local_closure(module_energy_tomap,   /* name */
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
    &be_const_str_tomap,
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

/********************************************************************/
/* End of solidification */
