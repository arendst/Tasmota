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
be_define_local_const_str(init_str_0, "get_tasmota", 334356779, 11);
be_define_local_const_str(init_str_1, "i2c_enabled", 218388101, 11);
be_define_local_const_str(init_str_2, "addr", 1087856498, 4);
be_define_local_const_str(init_str_3, "wire", -212213352, 4);
be_define_local_const_str(init_str_4, "wire_scan", -1623691416, 9);
be_define_local_const_str(init_str_5, "function", -1630125495, 8);
be_define_local_const_str(init_str_6, "name", -1925595674, 4);
be_define_local_const_str(init_str_7, "I2C:", 813483371, 4);
be_define_local_const_str(init_str_8, "detected on bus", 1432002650, 15);
be_define_local_const_str(init_str_9, "bus", 1607822841, 3);

static const bvalue init_ktab[10] = {
  { { .s=be_local_const_str(init_str_0) }, BE_STRING},
  { { .s=be_local_const_str(init_str_1) }, BE_STRING},
  { { .s=be_local_const_str(init_str_2) }, BE_STRING},
  { { .s=be_local_const_str(init_str_3) }, BE_STRING},
  { { .s=be_local_const_str(init_str_4) }, BE_STRING},
  { { .s=be_local_const_str(init_str_5) }, BE_STRING},
  { { .s=be_local_const_str(init_str_6) }, BE_STRING},
  { { .s=be_local_const_str(init_str_7) }, BE_STRING},
  { { .s=be_local_const_str(init_str_8) }, BE_STRING},
  { { .s=be_local_const_str(init_str_9) }, BE_STRING},
};

static const uint32_t init_code[44] = {
  0x8C100100,  //  0000  GETMET	R4	R0	R256
  0x7C100200,  //  0001  CALL	R4	1
  0x4C140000,  //  0002  LDNIL	5
  0x20140605,  //  0003  NE	R5	R3	R5
  0x78160004,  //  0004  JMPF	R5	#000A
  0x8C140901,  //  0005  GETMET	R5	R4	R257
  0x5C1C0600,  //  0006  MOVE	R7	R3
  0x7C140400,  //  0007  CALL	R5	2
  0x74160000,  //  0008  JMPT	R5	#000A
  0x80000A00,  //  0009  RET	0	R5
  0x90020402,  //  000A  SETMBR	R0	R258	R2
  0x8C140904,  //  000B  GETMET	R5	R4	R260
  0x881C0102,  //  000C  GETMBR	R7	R0	R258
  0x7C140400,  //  000D  CALL	R5	2
  0x90020605,  //  000E  SETMBR	R0	R259	R5
  0x88140103,  //  000F  GETMBR	R5	R0	R259
  0x78160019,  //  0010  JMPF	R5	#002B
  0x60140015,  //  0011  GETGBL	R5	G21
  0x5C180200,  //  0012  MOVE	R6	R1
  0x7C140200,  //  0013  CALL	R5	1
  0x1C140B05,  //  0014  EQ	R5	R5	R261
  0x78160004,  //  0015  JMPF	R5	#001B
  0x5C140200,  //  0016  MOVE	R5	R1
  0x5C180000,  //  0017  MOVE	R6	R0
  0x7C140200,  //  0018  CALL	R5	1
  0x90020C05,  //  0019  SETMBR	R0	R262	R5
  0x70020000,  //  001A  JMP		#001C
  0x90020C01,  //  001B  SETMBR	R0	R262	R1
  0x88140106,  //  001C  GETMBR	R5	R0	R262
  0x4C180000,  //  001D  LDNIL	6
  0x1C140A06,  //  001E  EQ	R5	R5	R6
  0x78160001,  //  001F  JMPF	R5	#0022
  0x4C140000,  //  0020  LDNIL	5
  0x90020605,  //  0021  SETMBR	R0	R259	R5
  0x88140103,  //  0022  GETMBR	R5	R0	R259
  0x78160006,  //  0023  JMPF	R5	#002B
  0x6014000F,  //  0024  GETGBL	R5	G15
  0x58180007,  //  0025  LDCONST	R6	K7
  0x881C0106,  //  0026  GETMBR	R7	R0	R262
  0x58200008,  //  0027  LDCONST	R8	K8
  0x88240103,  //  0028  GETMBR	R9	R0	R259
  0x88241309,  //  0029  GETMBR	R9	R9	R265
  0x7C140800,  //  002A  CALL	R5	4
  0x80000000,  //  002B  RET	0	R0
};

be_define_local_proto(init, 10, 4, 1, 0, 0);
be_define_local_closure(init);

/*******************************************************************/

/********************************************************************
** Solidified function: write8
********************************************************************/

/********** Solidified proto: write8 */
be_define_local_const_str(write8_str_name, "write8", -1160975764, 6);
be_define_local_const_str(write8_str_source, "input", -103256197, 5);
be_define_local_const_str(write8_str_0, "wire", -212213352, 4);
be_define_local_const_str(write8_str_1, "write", -1104765092, 5);
be_define_local_const_str(write8_str_2, "addr", 1087856498, 4);

static const bvalue write8_ktab[4] = {
  { { .s=be_local_const_str(write8_str_0) }, BE_STRING},
  { { .s=be_local_const_str(write8_str_1) }, BE_STRING},
  { { .s=be_local_const_str(write8_str_2) }, BE_STRING},
  { { .i=1 }, BE_INT},
};

static const uint32_t write8_code[8] = {
  0x880C0100,  //  0000  GETMBR	R3	R0	R256
  0x8C0C0701,  //  0001  GETMET	R3	R3	R257
  0x88140102,  //  0002  GETMBR	R5	R0	R258
  0x5C180200,  //  0003  MOVE	R6	R1
  0x5C1C0400,  //  0004  MOVE	R7	R2
  0x58200003,  //  0005  LDCONST	R8	K3
  0x7C0C0A00,  //  0006  CALL	R3	5
  0x80040600,  //  0007  RET	1	R3
};

be_define_local_proto(write8, 9, 3, 1, 0, 0);
be_define_local_closure(write8);

/*******************************************************************/

/********************************************************************
** Solidified function: write_bit
********************************************************************/

/********** Solidified proto: write_bit */
be_define_local_const_str(write_bit_str_name, "write_bit", -1633976860, 9);
be_define_local_const_str(write_bit_str_source, "input", -103256197, 5);
be_define_local_const_str(write_bit_str_2, "write8", -1160975764, 6);
be_define_local_const_str(write_bit_str_3, "read8", -1492179129, 5);

static const bvalue write_bit_ktab[4] = {
  { { .i=0 }, BE_INT},
  { { .i=1 }, BE_INT},
  { { .s=be_local_const_str(write_bit_str_2) }, BE_STRING},
  { { .s=be_local_const_str(write_bit_str_3) }, BE_STRING},
};

static const uint32_t write_bit_code[26] = {
  0x14100500,  //  0000  LT	R4	R2	R256
  0x74120002,  //  0001  JMPT	R4	#0005
  0x54120006,  //  0002  LDINT	R4	7
  0x24100404,  //  0003  GT	R4	R2	R4
  0x78120000,  //  0004  JMPF	R4	#0006
  0x80000800,  //  0005  RET	0	R4
  0x38120202,  //  0006  SHL	R4	R257	R2
  0x780E0007,  //  0007  JMPF	R3	#0010
  0x8C140102,  //  0008  GETMET	R5	R0	R258
  0x5C1C0200,  //  0009  MOVE	R7	R1
  0x8C200103,  //  000A  GETMET	R8	R0	R259
  0x5C280200,  //  000B  MOVE	R10	R1
  0x7C200400,  //  000C  CALL	R8	2
  0x30201004,  //  000D  OR	R8	R8	R4
  0x7C140600,  //  000E  CALL	R5	3
  0x70020008,  //  000F  JMP		#0019
  0x8C140102,  //  0010  GETMET	R5	R0	R258
  0x5C1C0200,  //  0011  MOVE	R7	R1
  0x8C200103,  //  0012  GETMET	R8	R0	R259
  0x5C280200,  //  0013  MOVE	R10	R1
  0x7C200400,  //  0014  CALL	R8	2
  0x542600FE,  //  0015  LDINT	R9	255
  0x4241204,  //  0016  SUB	R9	R9	R4
  0x2C201009,  //  0017  AND	R8	R8	R9
  0x7C140600,  //  0018  CALL	R5	3
  0x80000000,  //  0019  RET	0	R0
};

be_define_local_proto(write_bit, 11, 4, 1, 0, 0);
be_define_local_closure(write_bit);

/*******************************************************************/


/********************************************************************
** Solidified function: read8
********************************************************************/

/********** Solidified proto: read8 */
be_define_local_const_str(read8_str_name, "read8", -1492179129, 5);
be_define_local_const_str(read8_str_source, "input", -103256197, 5);
be_define_local_const_str(read8_str_0, "wire", -212213352, 4);
be_define_local_const_str(read8_str_1, "read", -824204347, 4);
be_define_local_const_str(read8_str_2, "addr", 1087856498, 4);

static const bvalue read8_ktab[4] = {
  { { .s=be_local_const_str(read8_str_0) }, BE_STRING},
  { { .s=be_local_const_str(read8_str_1) }, BE_STRING},
  { { .s=be_local_const_str(read8_str_2) }, BE_STRING},
  { { .i=1 }, BE_INT},
};

static const uint32_t read8_code[7] = {
  0x88080100,  //  0000  GETMBR	R2	R0	R256
  0x8C080501,  //  0001  GETMET	R2	R2	R257
  0x88100102,  //  0002  GETMBR	R4	R0	R258
  0x5C140200,  //  0003  MOVE	R5	R1
  0x58180003,  //  0004  LDCONST	R6	K3
  0x7C080800,  //  0005  CALL	R2	4
  0x80040400,  //  0006  RET	1	R2
};

be_define_local_proto(read8, 7, 2, 1, 0, 0);
be_define_local_closure(read8);

/*******************************************************************/

/********************************************************************
** Solidified function: read12
********************************************************************/

/********** Solidified proto: read12 */
be_define_local_const_str(read12_str_name, "read12", -3890326, 6);
be_define_local_const_str(read12_str_source, "input", -103256197, 5);
be_define_local_const_str(read12_str_0, "wire", -212213352, 4);
be_define_local_const_str(read12_str_1, "read_bytes", -718234123, 10);
be_define_local_const_str(read12_str_2, "addr", 1087856498, 4);

static const bvalue read12_ktab[6] = {
  { { .s=be_local_const_str(read12_str_0) }, BE_STRING},
  { { .s=be_local_const_str(read12_str_1) }, BE_STRING},
  { { .s=be_local_const_str(read12_str_2) }, BE_STRING},
  { { .i=2 }, BE_INT},
  { { .i=0 }, BE_INT},
  { { .i=1 }, BE_INT},
};

static const uint32_t read12_code[12] = {
  0x88080100,  //  0000  GETMBR	R2	R0	R256
  0x8C080501,  //  0001  GETMET	R2	R2	R257
  0x88100102,  //  0002  GETMBR	R4	R0	R258
  0x5C140200,  //  0003  MOVE	R5	R1
  0x58180003,  //  0004  LDCONST	R6	K3
  0x7C080800,  //  0005  CALL	R2	4
  0x940C0504,  //  0006  GETIDX	R3	R2	R260
  0x54120003,  //  0007  LDINT	R4	4
  0x380C0604,  //  0008  SHL	R3	R3	R4
  0x94100505,  //  0009  GETIDX	R4	R2	R261
  0xC0604,  //  000A  ADD	R3	R3	R4
  0x80040600,  //  000B  RET	1	R3
};

be_define_local_proto(read12, 7, 2, 1, 0, 0);
be_define_local_closure(read12);

/*******************************************************************/


/********************************************************************
** Solidified function: read13
********************************************************************/

/********** Solidified proto: read13 */
be_define_local_const_str(read13_str_name, "read13", 12887293, 6);
be_define_local_const_str(read13_str_source, "input", -103256197, 5);
be_define_local_const_str(read13_str_0, "wire", -212213352, 4);
be_define_local_const_str(read13_str_1, "read_bytes", -718234123, 10);
be_define_local_const_str(read13_str_2, "addr", 1087856498, 4);

static const bvalue read13_ktab[6] = {
  { { .s=be_local_const_str(read13_str_0) }, BE_STRING},
  { { .s=be_local_const_str(read13_str_1) }, BE_STRING},
  { { .s=be_local_const_str(read13_str_2) }, BE_STRING},
  { { .i=2 }, BE_INT},
  { { .i=0 }, BE_INT},
  { { .i=1 }, BE_INT},
};

static const uint32_t read13_code[12] = {
  0x88080100,  //  0000  GETMBR	R2	R0	R256
  0x8C080501,  //  0001  GETMET	R2	R2	R257
  0x88100102,  //  0002  GETMBR	R4	R0	R258
  0x5C140200,  //  0003  MOVE	R5	R1
  0x58180003,  //  0004  LDCONST	R6	K3
  0x7C080800,  //  0005  CALL	R2	4
  0x940C0504,  //  0006  GETIDX	R3	R2	R260
  0x54120004,  //  0007  LDINT	R4	5
  0x380C0604,  //  0008  SHL	R3	R3	R4
  0x94100505,  //  0009  GETIDX	R4	R2	R261
  0xC0604,  //  000A  ADD	R3	R3	R4
  0x80040600,  //  000B  RET	1	R3
};

be_define_local_proto(read13, 7, 2, 1, 0, 0);
be_define_local_closure(read13);

/*******************************************************************/


/********************************************************************
** Solidified function: read24
********************************************************************/

/********** Solidified proto: read24 */
be_define_local_const_str(read24_str_name, "read24", 1808533811, 6);
be_define_local_const_str(read24_str_source, "input", -103256197, 5);
be_define_local_const_str(read24_str_0, "wire", -212213352, 4);
be_define_local_const_str(read24_str_1, "read_bytes", -718234123, 10);
be_define_local_const_str(read24_str_2, "addr", 1087856498, 4);

static const bvalue read24_ktab[7] = {
  { { .s=be_local_const_str(read24_str_0) }, BE_STRING},
  { { .s=be_local_const_str(read24_str_1) }, BE_STRING},
  { { .s=be_local_const_str(read24_str_2) }, BE_STRING},
  { { .i=3 }, BE_INT},
  { { .i=0 }, BE_INT},
  { { .i=1 }, BE_INT},
  { { .i=2 }, BE_INT},
};

static const uint32_t read24_code[16] = {
  0x88080100,  //  0000  GETMBR	R2	R0	R256
  0x8C080501,  //  0001  GETMET	R2	R2	R257
  0x88100102,  //  0002  GETMBR	R4	R0	R258
  0x5C140200,  //  0003  MOVE	R5	R1
  0x58180003,  //  0004  LDCONST	R6	K3
  0x7C080800,  //  0005  CALL	R2	4
  0x940C0504,  //  0006  GETIDX	R3	R2	R260
  0x5412000F,  //  0007  LDINT	R4	16
  0x380C0604,  //  0008  SHL	R3	R3	R4
  0x94100505,  //  0009  GETIDX	R4	R2	R261
  0x54160007,  //  000A  LDINT	R5	8
  0x38100805,  //  000B  SHL	R4	R4	R5
  0xC0604,  //  000C  ADD	R3	R3	R4
  0x94100506,  //  000D  GETIDX	R4	R2	R262
  0xC0604,  //  000E  ADD	R3	R3	R4
  0x80040600,  //  000F  RET	1	R3
};

be_define_local_proto(read24, 7, 2, 1, 0, 0);
be_define_local_closure(read24);

/*******************************************************************/

/********************************************************************
** Solidified function: read32
********************************************************************/

/********** Solidified proto: read32 */
be_define_local_const_str(read32_str_name, "read32", 1741276240, 6);
be_define_local_const_str(read32_str_source, "input", -103256197, 5);
be_define_local_const_str(read32_str_0, "wire", -212213352, 4);
be_define_local_const_str(read32_str_1, "read_bytes", -718234123, 10);
be_define_local_const_str(read32_str_2, "addr", 1087856498, 4);

static const bvalue read32_ktab[7] = {
  { { .s=be_local_const_str(read32_str_0) }, BE_STRING},
  { { .s=be_local_const_str(read32_str_1) }, BE_STRING},
  { { .s=be_local_const_str(read32_str_2) }, BE_STRING},
  { { .i=0 }, BE_INT},
  { { .i=1 }, BE_INT},
  { { .i=2 }, BE_INT},
  { { .i=3 }, BE_INT},
};

static const uint32_t read32_code[20] = {
  0x88080100,  //  0000  GETMBR	R2	R0	R256
  0x8C080501,  //  0001  GETMET	R2	R2	R257
  0x88100102,  //  0002  GETMBR	R4	R0	R258
  0x5C140200,  //  0003  MOVE	R5	R1
  0x541A0003,  //  0004  LDINT	R6	4
  0x7C080800,  //  0005  CALL	R2	4
  0x940C0503,  //  0006  GETIDX	R3	R2	R259
  0x54120017,  //  0007  LDINT	R4	24
  0x380C0604,  //  0008  SHL	R3	R3	R4
  0x94100504,  //  0009  GETIDX	R4	R2	R260
  0x5416000F,  //  000A  LDINT	R5	16
  0x38100805,  //  000B  SHL	R4	R4	R5
  0xC0604,  //  000C  ADD	R3	R3	R4
  0x94100505,  //  000D  GETIDX	R4	R2	R261
  0x54160007,  //  000E  LDINT	R5	8
  0x38100805,  //  000F  SHL	R4	R4	R5
  0xC0604,  //  0010  ADD	R3	R3	R4
  0x94100506,  //  0011  GETIDX	R4	R2	R262
  0xC0604,  //  0012  ADD	R3	R3	R4
  0x80040600,  //  0013  RET	1	R3
};

be_define_local_proto(read32, 7, 2, 1, 0, 0);
be_define_local_closure(read32);

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
        { "init", (bntvfunc) &init_closure },
        { "write8", (bntvfunc) &write8_closure },
        { "write_bit", (bntvfunc) &write_bit_closure },
        { "read8", (bntvfunc) &read8_closure },
        { "read12", (bntvfunc) &read12_closure },
        { "read13", (bntvfunc) &read13_closure },
        { "read24", (bntvfunc) &read24_closure },
        { "read32", (bntvfunc) &read32_closure },
        
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

    init, closure(init_closure)
    write8, closure(write8_closure)
    write_bit, closure(write_bit_closure)
    read8, closure(read8_closure)
    read12, closure(read12_closure)
    read13, closure(read13_closure)
    read24, closure(read24_closure)
    read32, closure(read32_closure)
}
@const_object_info_end */
