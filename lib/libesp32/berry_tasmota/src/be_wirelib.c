/********************************************************************
 * Tasmota lib
 * 
 * To use: `import wire`
 * 
 * 2 wire communication - I2C
 *******************************************************************/
#include "be_constobj.h"

#ifdef USE_I2C

extern int b_wire_init(bvm *vm);

extern int b_wire_begintransmission(bvm *vm);
extern int b_wire_endtransmission(bvm *vm);
extern int b_wire_requestfrom(bvm *vm);
extern int b_wire_available(bvm *vm);
extern int b_wire_write(bvm *vm);
extern int b_wire_read(bvm *vm);

extern int b_wire_scan(bvm *vm);

extern int b_wire_validwrite(bvm *vm);
extern int b_wire_validread(bvm *vm);
extern int b_wire_detect(bvm *vm);
extern int b_wire_enabled(bvm *vm);

/********************************************************************
** Solidified function: write_bytes
********************************************************************/
be_local_closure(write_bytes,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    4,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str(_begin_transmission),
    /* K1   */  be_nested_str(_write),
    /* K2   */  be_nested_str(_end_transmission),
    }),
    &be_const_str_write_bytes,
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x8C100100,  //  0000  GETMET	R4	R0	K0
      0x5C180200,  //  0001  MOVE	R6	R1
      0x7C100400,  //  0002  CALL	R4	2
      0x8C100101,  //  0003  GETMET	R4	R0	K1
      0x5C180400,  //  0004  MOVE	R6	R2
      0x7C100400,  //  0005  CALL	R4	2
      0x8C100101,  //  0006  GETMET	R4	R0	K1
      0x5C180600,  //  0007  MOVE	R6	R3
      0x7C100400,  //  0008  CALL	R4	2
      0x8C100102,  //  0009  GETMET	R4	R0	K2
      0x7C100200,  //  000A  CALL	R4	1
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: read_bytes
********************************************************************/
be_local_closure(read_bytes,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    4,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str(_begin_transmission),
    /* K1   */  be_nested_str(_write),
    /* K2   */  be_nested_str(_end_transmission),
    /* K3   */  be_nested_str(_request_from),
    /* K4   */  be_nested_str(_available),
    /* K5   */  be_nested_str(_read),
    }),
    &be_const_str_read_bytes,
    &be_const_str_solidified,
    ( &(const binstruction[24]) {  /* code */
      0x8C100100,  //  0000  GETMET	R4	R0	K0
      0x5C180200,  //  0001  MOVE	R6	R1
      0x7C100400,  //  0002  CALL	R4	2
      0x8C100101,  //  0003  GETMET	R4	R0	K1
      0x5C180400,  //  0004  MOVE	R6	R2
      0x7C100400,  //  0005  CALL	R4	2
      0x8C100102,  //  0006  GETMET	R4	R0	K2
      0x50180000,  //  0007  LDBOOL	R6	0	0
      0x7C100400,  //  0008  CALL	R4	2
      0x8C100103,  //  0009  GETMET	R4	R0	K3
      0x5C180200,  //  000A  MOVE	R6	R1
      0x5C1C0600,  //  000B  MOVE	R7	R3
      0x7C100600,  //  000C  CALL	R4	3
      0x60100015,  //  000D  GETGBL	R4	G21
      0x5C140600,  //  000E  MOVE	R5	R3
      0x7C100200,  //  000F  CALL	R4	1
      0x8C140104,  //  0010  GETMET	R5	R0	K4
      0x7C140200,  //  0011  CALL	R5	1
      0x78160003,  //  0012  JMPF	R5	#0017
      0x8C140105,  //  0013  GETMET	R5	R0	K5
      0x7C140200,  //  0014  CALL	R5	1
      0x40140805,  //  0015  CONNECT	R5	R4	R5
      0x7001FFF8,  //  0016  JMP		#0010
      0x80040800,  //  0017  RET	1	R4
    })
  )
);
/*******************************************************************/


#include "be_fixed_be_class_tasmota_wire.h"

void be_load_wirelib(bvm *vm) {
    be_pushntvclass(vm, &be_class_tasmota_wire);
    be_setglobal(vm, "Wire");
    be_pop(vm, 1);
}
/* @const_object_info_begin

class be_class_tasmota_wire (scope: global, name: Wire) {
    bus, var

    init, func(b_wire_init)
    _begin_transmission, func(b_wire_begintransmission)
    _end_transmission, func(b_wire_endtransmission)
    _request_from, func(b_wire_requestfrom)
    _available, func(b_wire_available)
    _write, func(b_wire_write)
    _read, func(b_wire_read)
    scan, func(b_wire_scan)
    write, func(b_wire_validwrite)
    read, func(b_wire_validread)
    detect, func(b_wire_detect)
    enabled, func(b_wire_enabled)

    read_bytes, closure(read_bytes_closure)
    write_bytes, closure(write_bytes_closure)
}
@const_object_info_end */

#endif // USE_I2C