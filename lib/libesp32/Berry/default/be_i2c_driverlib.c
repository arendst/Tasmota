/********************************************************************
 * Tasmota I2C_Driver class
 * 
 * To use: `d = I2C_Driver(addr, name)`
 * where:
 *      addr: I2C address of the device
 *      name: name of the I2C chip for logging
 * 
 *******************************************************************/
#include "be_constobj.h"

extern bclass* be_class_tasmota_driver;     // Parent class

/********************************************************************
** Solidified function: init
********************************************************************/

/********** Solidified proto: init */
be_define_local_const_str(init_str_name, "init", 380752755, 4);
be_define_local_const_str(init_str_source, "input", -103256197, 5);
be_define_local_const_str(init_str_0, "string", 398550328, 6);
be_define_local_const_str(init_str_1, "get_tasmota", 334356779, 11);
be_define_local_const_str(init_str_2, "addr", 1087856498, 4);
be_define_local_const_str(init_str_3, "wire", -212213352, 4);
be_define_local_const_str(init_str_4, "wire_scan", -1623691416, 9);
be_define_local_const_str(init_str_5, "name", -1925595674, 4);
be_define_local_const_str(init_str_6, "format", -1180859054, 6);
be_define_local_const_str(init_str_7, "I2C: %s detected on bus %d", 110398549, 26);
be_define_local_const_str(init_str_8, "bus", 1607822841, 3);

static const bvalue init_ktab[9] = {
  { { .s=be_local_const_str(init_str_0) }, BE_STRING},
  { { .s=be_local_const_str(init_str_1) }, BE_STRING},
  { { .s=be_local_const_str(init_str_2) }, BE_STRING},
  { { .s=be_local_const_str(init_str_3) }, BE_STRING},
  { { .s=be_local_const_str(init_str_4) }, BE_STRING},
  { { .s=be_local_const_str(init_str_5) }, BE_STRING},
  { { .s=be_local_const_str(init_str_6) }, BE_STRING},
  { { .s=be_local_const_str(init_str_7) }, BE_STRING},
  { { .s=be_local_const_str(init_str_8) }, BE_STRING},
};

static const uint32_t init_code[20] = {
  0xA40E0000,  //  0000  IMPORT R3  R256
  0x8C100101,  //  0001  GETMET R4  R0  R257
  0x7C100200,  //  0002  CALL R4  1
  0x90020401,  //  0003  SETMBR R0  R258  R1
  0x8C140904,  //  0004  GETMET R5  R4  R260
  0x881C0102,  //  0005  GETMBR R7  R0  R258
  0x7C140400,  //  0006  CALL R5  2
  0x90020605,  //  0007  SETMBR R0  R259  R5
  0x90020A02,  //  0008  SETMBR R0  R261  R2
  0x88140103,  //  0009  GETMBR R5  R0  R259
  0x78160007,  //  000A  JMPF R5  #0013
  0x6014000F,  //  000B  GETGBL R5  G15
  0x8C180706,  //  000C  GETMET R6  R3  R262
  0x58200007,  //  000D  LDCONST  R8  K7
  0x88240105,  //  000E  GETMBR R9  R0  R261
  0x88280103,  //  000F  GETMBR R10 R0  R259
  0x88281508,  //  0010  GETMBR R10 R10 R264
  0x7C180800,  //  0011  CALL R6  4
  0x7C140200,  //  0012  CALL R5  1
  0x80000000,  //  0013  RET  0 R0
};

be_define_local_proto(init, 11, 3, 1, 0, 0);
be_define_local_closure(init);

/*******************************************************************/



#if BE_USE_PRECOMPILED_OBJECT
#include "../generate/be_fixed_be_class_tasmota_i2c_driver.h"
#endif

void be_load_driver_i2c_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
    static const bnfuncinfo members[] = {
        { "wire", NULL },
        { "addr", NULL },
        { "name", NULL },

        { NULL, (bntvfunc) BE_CLOSURE }, /* mark section for berry closures */
        { "add_cmd", (bntvfunc) &init_closure },
        
        { NULL, NULL }
    };

    be_regclass(vm, "I2C_Driver", members);
    be_getglobal(vm, "I2C_Driver");
    be_getglobal(vm, "Driver");
    be_setsuper(vm, -2);
    be_pop(vm, 2);
#else
    be_pushntvclass(vm, &be_class_tasmota_i2c_driver);
    be_setglobal(vm, "I2C_Driver");
    be_pop(vm, 1);
#endif
}
/* @const_object_info_begin

class be_class_tasmota_i2c_driver (scope: global, name: I2C_Driver, super: be_class_tasmota_driver) {
    wire, var
    addr, var
    name, var

    add_cmd, closure(init_closure)
}
@const_object_info_end */
