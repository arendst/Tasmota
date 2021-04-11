/********************************************************************
 * Tasmota lib
 * 
 * To use: `d = Driver()`
 * 
 *******************************************************************/
#include "be_object.h"
#include "be_string.h"
#include "be_gc.h"

extern int d_getTasmotaGlob(bvm *vm);


/********************************************************************
  "class Driver2 : Driver "
    "def add_cmd(c, f) "
      "var tasmota = self.get_tasmota() "
      "tasmota.add_cmd(c, / cmd, idx, payload, payload_json -> f(self, cmd, idx, payload, payload_json)) "
    "end "
  "end "
  "Driver = Driver2 "
********************************************************************/
/********************************************************************
** Solidified function: add_cmd
********************************************************************/

/********** Solidified proto: add_cmd_0 */
static const bupvaldesc add_cmd_0_upvals[2] = {
  be_local_const_upval(1, 2),
  be_local_const_upval(1, 0),
};

be_define_local_const_str(add_cmd_0_str_name, "add_cmd_0", 607256038, 0, 8, 0);
be_define_local_const_str(add_cmd_0_str_source, "input", -103256197, 0, 5, 0);

static const uint32_t add_cmd_0_code[8] = {
  0x68100000,  //  0000  GETUPV	R4	U0
  0x68140001,  //  0001  GETUPV	R5	U1
  0x5C180000,  //  0002  MOVE	R6	R0
  0x5C1C0200,  //  0003  MOVE	R7	R1
  0x5C200400,  //  0004  MOVE	R8	R2
  0x5C240600,  //  0005  MOVE	R9	R3
  0x7C100A00,  //  0006  CALL	R4	5
  0x80040800,  //  0007  RET	1	R4
};

be_define_local_proto(add_cmd_0, 10, 4, 0, 0, 1);

/********** Solidified proto: add_cmd */
static const bproto *add_cmd_subproto[1] = {
  &add_cmd_0_proto,
};

be_define_local_const_str(add_cmd_str_name, "add_cmd", -933336417, 0, 7, 0);
be_define_local_const_str(add_cmd_str_source, "input", -103256197, 0, 5, 0);
be_define_local_const_str(add_cmd_str_0, "get_tasmota", 334356779, 0, 11, 0);
be_define_local_const_str(add_cmd_str_1, "add_cmd", -933336417, 0, 7, 0);

static const bvalue add_cmd_ktab[2] = {
  { { .s=be_local_const_str(add_cmd_str_0) }, BE_STRING},
  { { .s=be_local_const_str(add_cmd_str_1) }, BE_STRING},
};

static const uint32_t add_cmd_code[8] = {
  0x8C0C0100,  //  0000  GETMET	R3	R0	R256
  0x7C0C0200,  //  0001  CALL	R3	1
  0x8C100701,  //  0002  GETMET	R4	R3	R257
  0x5C180200,  //  0003  MOVE	R6	R1
  0x841C0000,  //  0004  CLOSURE	R7	P0
  0x7C100600,  //  0005  CALL	R4	3
  0xA0000000,  //  0006  CLOSE	0
  0x80000000,  //  0007  RET	0	R0
};

be_define_local_proto(add_cmd, 8, 3, 1, 1, 0);
be_define_local_closure(add_cmd);

/*******************************************************************/

// #if !BE_USE_PRECOMPILED_OBJECT
#if 1           // TODO we will do pre-compiled later
void be_load_driverlib(bvm *vm)
{
    static const bnfuncinfo members[] = {
        { "every_second", NULL },
        { "every_100ms", NULL },
        { "web_add_button", NULL },
        { "web_add_main_button", NULL },
        { "save_before_restart", NULL },
        { "web_sensor", NULL },
        { "json_append", NULL },
        { "button_pressed", NULL },

        { "get_tasmota", d_getTasmotaGlob },

        { NULL, (bntvfunc) BE_CLOSURE }, /* mark section for berry closures */
        { "add_cmd", (bntvfunc) &add_cmd_closure },
        
        { NULL, NULL }
    };
    be_regclass(vm, "Driver", members);
}
#else
/* @const_object_info_begin
module tasmota (scope: global, depend: 1) {
    get_free_heap, func(l_getFreeHeap)
}
@const_object_info_end */
#include "../generate/be_fixed_tasmota.h"
#endif
