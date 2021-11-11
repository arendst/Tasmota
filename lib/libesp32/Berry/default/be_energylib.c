/********************************************************************
 * Tasmota lib
 * 
 * To use: `import power`
 * 
 * read power values
 *******************************************************************/
#include "be_constobj.h"

#ifdef USE_ENERGY_SENSOR

extern struct ENERGY Energy;
/*

def init(m)
    import global
    global._energy = energy_struct(m._ptr)
    return m
end

def read()
    return _energy.tomap()
end

def member(k)
    return _energy.(k)
end

def setmember(k, v)
    _energy.(k) = v
end

import solidify
solidify.dump(m.init)
*/

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
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_string("global", 503252654, 6),
    /* K1   */  be_nested_string("_energy", 535372070, 7),
    /* K2   */  be_nested_string("energy_struct", 1655792843, 13),
    /* K3   */  be_nested_string("_ptr", 306235816, 4),
    }),
    ((bstring*) &be_const_str_init),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[ 6]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xB80A0400,  //  0001  GETNGBL	R2	K2
      0x880C0103,  //  0002  GETMBR	R3	R0	K3
      0x7C080200,  //  0003  CALL	R2	1
      0x90060202,  //  0004  SETMBR	R1	K1	R2
      0x80040000,  //  0005  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: read
********************************************************************/
be_local_closure(read,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    0,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_string("_energy", 535372070, 7),
    /* K1   */  be_nested_string("tomap", 612167626, 5),
    }),
    (be_nested_const_str("<anonymous>", 1160973142, 11)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[ 4]) {  /* code */
      0xB8020000,  //  0000  GETNGBL	R0	K0
      0x8C000101,  //  0001  GETMET	R0	R0	K1
      0x7C000200,  //  0002  CALL	R0	1
      0x80040000,  //  0003  RET	1	R0
    })
  )
);
/*******************************************************************/

/********************************************************************
** Solidified function: member
********************************************************************/
be_local_closure(member,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_string("_energy", 535372070, 7),
    }),
    (be_nested_const_str("member", 719708611, 6)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[ 3]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x88040200,  //  0001  GETMBR	R1	R1	R0
      0x80040200,  //  0002  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: setmember
********************************************************************/
be_local_closure(setmember,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    2,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_string("_energy", 535372070, 7),
    }),
    (be_nested_const_str("setmember", 1432909441, 9)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[ 3]) {  /* code */
      0xB80A0000,  //  0000  GETNGBL	R2	K0
      0x90080001,  //  0001  SETMBR	R2	R0	R1
      0x80000000,  //  0002  RET	0
    })
  )
);
/*******************************************************************/


/* @const_object_info_begin
module energy (scope: global) {
    _ptr, comptr(&Energy)
    init, closure(init_closure)

    read, closure(read_closure)
    member, closure(member_closure)
    setmember, closure(setmember_closure)
}
@const_object_info_end */
#include "../generate/be_fixed_energy.h"

#endif // USE_ENERGY_SENSOR