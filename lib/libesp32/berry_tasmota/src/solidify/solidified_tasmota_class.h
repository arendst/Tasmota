/* Solidification of tasmota_class.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
extern const bclass be_class_Tasmota;
// compact class 'Tasmota' ktab size: 158, total: 304 (saved 1168 bytes)
static const bvalue be_ktab_class_Tasmota[158] = {
  /* K0   */  be_nested_str(i2c_enabled),
  /* K1   */  be_nested_str(wire1),
  /* K2   */  be_nested_str(enabled),
  /* K3   */  be_nested_str(detect),
  /* K4   */  be_nested_str(wire2),
  /* K5   */  be_nested_str(_crons),
  /* K6   */  be_const_int(0),
  /* K7   */  be_nested_str(size),
  /* K8   */  be_nested_str(id),
  /* K9   */  be_nested_str(trig),
  /* K10  */  be_const_int(1),
  /* K11  */  be_nested_str(introspect),
  /* K12  */  be_nested_str(function),
  /* K13  */  be_nested_str(type_error),
  /* K14  */  be_nested_str(BRY_X3A_X20argument_X20must_X20be_X20a_X20function),
  /* K15  */  be_nested_str(ismethod),
  /* K16  */  be_nested_str(BRY_X3A_X20method_X20not_X20allowed_X2C_X20use_X20a_X20closure_X20like_X20_X27_X2F_X20args_X20_X2D_X3E_X20obj_X2Efunc_X28args_X29_X27),
  /* K17  */  be_nested_str(string),
  /* K18  */  be_nested_str(toupper),
  /* K19  */  be_nested_str(time_dump),
  /* K20  */  be_nested_str(_X2504d_X2D_X2502d_X2D_X2502dT_X2502d_X3A_X2502d_X3A_X2502d),
  /* K21  */  be_nested_str(year),
  /* K22  */  be_nested_str(month),
  /* K23  */  be_nested_str(day),
  /* K24  */  be_nested_str(hour),
  /* K25  */  be_nested_str(min),
  /* K26  */  be_nested_str(sec),
  /* K27  */  be_nested_str(match),
  /* K28  */  be_nested_str(trigger),
  /* K29  */  be_nested_str(_drivers),
  /* K30  */  be_nested_str(find),
  /* K31  */  be_nested_str(pop),
  /* K32  */  be_nested_str(_rules),
  /* K33  */  be_nested_str(json),
  /* K34  */  be_nested_str(load),
  /* K35  */  be_nested_str(log),
  /* K36  */  be_nested_str(BRY_X3A_X20ERROR_X2C_X20bad_X20json_X3A_X20),
  /* K37  */  be_const_int(3),
  /* K38  */  be_nested_str(Tele),
  /* K39  */  be_nested_str(try_rule),
  /* K40  */  be_nested_str(f),
  /* K41  */  be_nested_str(ccronexpr),
  /* K42  */  be_nested_str(now),
  /* K43  */  be_nested_str(next),
  /* K44  */  be_nested_str(time_reached),
  /* K45  */  be_nested_str(_defer),
  /* K46  */  be_nested_str(push),
  /* K47  */  be_nested_str(tasmota),
  /* K48  */  be_nested_str(global),
  /* K49  */  be_nested_str(deferred_ready),
  /* K50  */  be_nested_str(scale_uint),
  /* K51  */  be_const_int(2),
  /* K52  */  be_nested_str(_fl),
  /* K53  */  be_nested_str(remove),
  /* K54  */  be_nested_str(check_not_method),
  /* K55  */  be_nested_str(remove_rule),
  /* K56  */  be_nested_str(Trigger),
  /* K57  */  be_nested_str(Rule_Matcher),
  /* K58  */  be_nested_str(parse),
  /* K59  */  be_nested_str(value_error),
  /* K60  */  be_nested_str(the_X20second_X20argument_X20is_X20not_X20a_X20function),
  /* K61  */  be_nested_str(ctypes_bytes_dyn),
  /* K62  */  be_nested_str(_global_addr),
  /* K63  */  be_nested_str(_global_def),
  /* K64  */  be_nested_str(_settings_ptr),
  /* K65  */  be_nested_str(get),
  /* K66  */  be_nested_str(settings),
  /* K67  */  be_nested_str(toptr),
  /* K68  */  be_nested_str(_settings_def),
  /* K69  */  be_nested_str(wd),
  /* K70  */  be_nested_str(),
  /* K71  */  be_nested_str(_debug_present),
  /* K72  */  be_nested_str(contains),
  /* K73  */  be_nested_str(debug),
  /* K74  */  be_nested_str(add_cmd),
  /* K75  */  be_nested_str(UrlFetch),
  /* K76  */  be_nested_str(_timers),
  /* K77  */  be_nested_str(millis),
  /* K78  */  be_nested_str(split),
  /* K79  */  be_nested_str(_X2F),
  /* K80  */  be_nested_str(index_X2Ehtml),
  /* K81  */  be_nested_str(webclient),
  /* K82  */  be_nested_str(set_follow_redirects),
  /* K83  */  be_nested_str(begin),
  /* K84  */  be_nested_str(GET),
  /* K85  */  be_nested_str(status_X3A_X20),
  /* K86  */  be_nested_str(connection_error),
  /* K87  */  be_nested_str(write_file),
  /* K88  */  be_nested_str(close),
  /* K89  */  be_nested_str(BRY_X3A_X20Fetched_X20),
  /* K90  */  be_nested_str(rule),
  /* K91  */  be_nested_str(instance),
  /* K92  */  be_nested_str(instance_X20required),
  /* K93  */  be_nested_str(cmd_res),
  /* K94  */  be_nested_str(o),
  /* K95  */  be_const_class(be_class_Tasmota),
  /* K96  */  be_nested_str(gc),
  /* K97  */  be_nested_str(collect),
  /* K98  */  be_nested_str(allocated),
  /* K99  */  be_nested_str(add_rule),
  /* K100 */  be_nested_str(every_50ms),
  /* K101 */  be_nested_str(run_timers),
  /* K102 */  be_nested_str(every_250ms),
  /* K103 */  be_nested_str(run_cron),
  /* K104 */  be_nested_str(mqtt_data),
  /* K105 */  be_nested_str(cmd),
  /* K106 */  be_nested_str(exec_cmd),
  /* K107 */  be_nested_str(tele),
  /* K108 */  be_nested_str(exec_tele),
  /* K109 */  be_nested_str(exec_rules),
  /* K110 */  be_nested_str(BRY_X3A_X20Exception_X3E_X20_X27_X25s_X27_X20_X2D_X20_X25s),
  /* K111 */  be_nested_str(traceback),
  /* K112 */  be_nested_str(save_before_restart),
  /* K113 */  be_nested_str(persist),
  /* K114 */  be_nested_str(save),
  /* K115 */  be_nested_str(keys),
  /* K116 */  be_nested_str(_X3F),
  /* K117 */  be_nested_str(stop_iteration),
  /* K118 */  be_nested_str(tasmota_X2Eget_light_X28_X29_X20is_X20deprecated_X2C_X20use_X20light_X2Eget_X28_X29),
  /* K119 */  be_nested_str(light),
  /* K120 */  be_nested_str(argument_X20must_X20be_X20a_X20function),
  /* K121 */  be_nested_str(fast_loop_enabled),
  /* K122 */  be_nested_str(maxlog_level),
  /* K123 */  be_nested_str(_cmd),
  /* K124 */  be_nested_str(run_deferred),
  /* K125 */  be_nested_str(_ccmd),
  /* K126 */  be_nested_str(find_key_i),
  /* K127 */  be_nested_str(resolvecmnd),
  /* K128 */  be_nested_str(_find_op),
  /* K129 */  be_const_int(2147483647),
  /* K130 */  be_nested_str(path),
  /* K131 */  be_nested_str(startswith),
  /* K132 */  be_nested_str(_X23),
  /* K133 */  be_nested_str(_X2E),
  /* K134 */  be_nested_str(_X2Ebe),
  /* K135 */  be_nested_str(endswith),
  /* K136 */  be_nested_str(_X2Ebec),
  /* K137 */  be_nested_str(c),
  /* K138 */  be_nested_str(BRY_X3A_X20file_X20extension_X20is_X20not_X20_X27_X2Ebe_X27_X20nor_X20_X27_X2Ebec_X27),
  /* K139 */  be_nested_str(exists),
  /* K140 */  be_nested_str(BRY_X3A_X20corrupt_X20bytecode_X20_X27_X25s_X27),
  /* K141 */  be_nested_str(BRY_X3A_X20bytecode_X20has_X20wrong_X20version_X20_X27_X25s_X27_X20_X28_X25s_X29),
  /* K142 */  be_nested_str(BRY_X3A_X20file_X20_X27_X25s_X27_X20does_X20not_X20have_X20_X27_X2Ebe_X27_X20extension),
  /* K143 */  be_nested_str(BRY_X3A_X20cannot_X20compile_X20file_X20in_X20read_X2Donly_X20archive),
  /* K144 */  be_nested_str(file),
  /* K145 */  be_nested_str(BRY_X3A_X20empty_X20compiled_X20file),
  /* K146 */  be_nested_str(BRY_X3A_X20failed_X20to_X20load_X20_X27_X25s_X27_X20_X28_X25s_X20_X2D_X20_X25s_X29),
  /* K147 */  be_nested_str(BRY_X3A_X20could_X20not_X20save_X20compiled_X20file_X20_X25s_X20_X28_X25s_X29),
  /* K148 */  be_nested_str(tasmota_X2Eset_light_X28_X29_X20is_X20deprecated_X2C_X20use_X20light_X2Eset_X28_X29),
  /* K149 */  be_nested_str(set),
  /* K150 */  be_nested_str(cb),
  /* K151 */  be_nested_str(gen_cb),
  /* K152 */  be_nested_str(http),
  /* K153 */  be_nested_str(resp_cmnd_str),
  /* K154 */  be_nested_str(URL_X20must_X20start_X20with_X20_X27http_X28s_X29_X27),
  /* K155 */  be_nested_str(urlfetch),
  /* K156 */  be_nested_str(resp_cmnd_failed),
  /* K157 */  be_nested_str(resp_cmnd_done),
};


extern const bclass be_class_Tasmota;

/********************************************************************
** Solidified function: wire_scan
********************************************************************/
be_local_closure(class_Tasmota_wire_scan,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Tasmota,     /* shared constants */
    &be_const_str_wire_scan,
    &be_const_str_solidified,
    ( &(const binstruction[33]) {  /* code */
      0x4C0C0000,  //  0000  LDNIL	R3
      0x200C0403,  //  0001  NE	R3	R2	R3
      0x780E0005,  //  0002  JMPF	R3	#0009
      0x8C0C0100,  //  0003  GETMET	R3	R0	K0
      0x5C140400,  //  0004  MOVE	R5	R2
      0x7C0C0400,  //  0005  CALL	R3	2
      0x740E0001,  //  0006  JMPT	R3	#0009
      0x4C0C0000,  //  0007  LDNIL	R3
      0x80040600,  //  0008  RET	1	R3
      0x880C0101,  //  0009  GETMBR	R3	R0	K1
      0x8C0C0702,  //  000A  GETMET	R3	R3	K2
      0x7C0C0200,  //  000B  CALL	R3	1
      0x780E0006,  //  000C  JMPF	R3	#0014
      0x880C0101,  //  000D  GETMBR	R3	R0	K1
      0x8C0C0703,  //  000E  GETMET	R3	R3	K3
      0x5C140200,  //  000F  MOVE	R5	R1
      0x7C0C0400,  //  0010  CALL	R3	2
      0x780E0001,  //  0011  JMPF	R3	#0014
      0x880C0101,  //  0012  GETMBR	R3	R0	K1
      0x80040600,  //  0013  RET	1	R3
      0x880C0104,  //  0014  GETMBR	R3	R0	K4
      0x8C0C0702,  //  0015  GETMET	R3	R3	K2
      0x7C0C0200,  //  0016  CALL	R3	1
      0x780E0006,  //  0017  JMPF	R3	#001F
      0x880C0104,  //  0018  GETMBR	R3	R0	K4
      0x8C0C0703,  //  0019  GETMET	R3	R3	K3
      0x5C140200,  //  001A  MOVE	R5	R1
      0x7C0C0400,  //  001B  CALL	R3	2
      0x780E0001,  //  001C  JMPF	R3	#001F
      0x880C0104,  //  001D  GETMBR	R3	R0	K4
      0x80040600,  //  001E  RET	1	R3
      0x4C0C0000,  //  001F  LDNIL	R3
      0x80040600,  //  0020  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: next_cron
********************************************************************/
be_local_closure(class_Tasmota_next_cron,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Tasmota,     /* shared constants */
    &be_const_str_next_cron,
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0x88080105,  //  0000  GETMBR	R2	R0	K5
      0x780A000D,  //  0001  JMPF	R2	#0010
      0x580C0006,  //  0002  LDCONST	R3	K6
      0x8C100507,  //  0003  GETMET	R4	R2	K7
      0x7C100200,  //  0004  CALL	R4	1
      0x14100604,  //  0005  LT	R4	R3	R4
      0x78120008,  //  0006  JMPF	R4	#0010
      0x94100403,  //  0007  GETIDX	R4	R2	R3
      0x88100908,  //  0008  GETMBR	R4	R4	K8
      0x1C100801,  //  0009  EQ	R4	R4	R1
      0x78120002,  //  000A  JMPF	R4	#000E
      0x94100403,  //  000B  GETIDX	R4	R2	R3
      0x88100909,  //  000C  GETMBR	R4	R4	K9
      0x80040800,  //  000D  RET	1	R4
      0x000C070A,  //  000E  ADD	R3	R3	K10
      0x7001FFF2,  //  000F  JMP		#0003
      0x80000000,  //  0010  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: check_not_method
********************************************************************/
be_local_closure(class_Tasmota_check_not_method,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Tasmota,     /* shared constants */
    &be_const_str_check_not_method,
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0xA40A1600,  //  0000  IMPORT	R2	K11
      0x600C0004,  //  0001  GETGBL	R3	G4
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C0C0200,  //  0003  CALL	R3	1
      0x200C070C,  //  0004  NE	R3	R3	K12
      0x780E0000,  //  0005  JMPF	R3	#0007
      0xB0061B0E,  //  0006  RAISE	1	K13	K14
      0x8C0C050F,  //  0007  GETMET	R3	R2	K15
      0x5C140200,  //  0008  MOVE	R5	R1
      0x7C0C0400,  //  0009  CALL	R3	2
      0x50100200,  //  000A  LDBOOL	R4	1	0
      0x1C0C0604,  //  000B  EQ	R3	R3	R4
      0x780E0000,  //  000C  JMPF	R3	#000E
      0xB0061B10,  //  000D  RAISE	1	K13	K16
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: find_list_i
********************************************************************/
be_local_closure(class_Tasmota_find_list_i,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Tasmota,     /* shared constants */
    &be_const_str_find_list_i,
    &be_const_str_solidified,
    ( &(const binstruction[20]) {  /* code */
      0xA40E2200,  //  0000  IMPORT	R3	K17
      0x58100006,  //  0001  LDCONST	R4	K6
      0x8C140712,  //  0002  GETMET	R5	R3	K18
      0x5C1C0400,  //  0003  MOVE	R7	R2
      0x7C140400,  //  0004  CALL	R5	2
      0x6018000C,  //  0005  GETGBL	R6	G12
      0x5C1C0200,  //  0006  MOVE	R7	R1
      0x7C180200,  //  0007  CALL	R6	1
      0x14180806,  //  0008  LT	R6	R4	R6
      0x781A0007,  //  0009  JMPF	R6	#0012
      0x8C180712,  //  000A  GETMET	R6	R3	K18
      0x94200204,  //  000B  GETIDX	R8	R1	R4
      0x7C180400,  //  000C  CALL	R6	2
      0x1C180C05,  //  000D  EQ	R6	R6	R5
      0x781A0000,  //  000E  JMPF	R6	#0010
      0x80040800,  //  000F  RET	1	R4
      0x0010090A,  //  0010  ADD	R4	R4	K10
      0x7001FFF2,  //  0011  JMP		#0005
      0x4C180000,  //  0012  LDNIL	R6
      0x80040C00,  //  0013  RET	1	R6
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: time_str
********************************************************************/
be_local_closure(class_Tasmota_time_str,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Tasmota,     /* shared constants */
    &be_const_str_time_str,
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x8C080113,  //  0000  GETMET	R2	R0	K19
      0x5C100200,  //  0001  MOVE	R4	R1
      0x7C080400,  //  0002  CALL	R2	2
      0x600C0018,  //  0003  GETGBL	R3	G24
      0x58100014,  //  0004  LDCONST	R4	K20
      0x94140515,  //  0005  GETIDX	R5	R2	K21
      0x94180516,  //  0006  GETIDX	R6	R2	K22
      0x941C0517,  //  0007  GETIDX	R7	R2	K23
      0x94200518,  //  0008  GETIDX	R8	R2	K24
      0x94240519,  //  0009  GETIDX	R9	R2	K25
      0x9428051A,  //  000A  GETIDX	R10	R2	K26
      0x7C0C0E00,  //  000B  CALL	R3	7
      0x80040600,  //  000C  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: try_rule
********************************************************************/
be_local_closure(class_Tasmota_try_rule,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Tasmota,     /* shared constants */
    &be_const_str_try_rule,
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x8C10051B,  //  0000  GETMET	R4	R2	K27
      0x5C180200,  //  0001  MOVE	R6	R1
      0x7C100400,  //  0002  CALL	R4	2
      0x4C140000,  //  0003  LDNIL	R5
      0x20140805,  //  0004  NE	R5	R4	R5
      0x78160009,  //  0005  JMPF	R5	#0010
      0x4C140000,  //  0006  LDNIL	R5
      0x20140605,  //  0007  NE	R5	R3	R5
      0x78160004,  //  0008  JMPF	R5	#000E
      0x5C140600,  //  0009  MOVE	R5	R3
      0x5C180800,  //  000A  MOVE	R6	R4
      0x881C051C,  //  000B  GETMBR	R7	R2	K28
      0x5C200200,  //  000C  MOVE	R8	R1
      0x7C140600,  //  000D  CALL	R5	3
      0x50140200,  //  000E  LDBOOL	R5	1	0
      0x80040A00,  //  000F  RET	1	R5
      0x50140000,  //  0010  LDBOOL	R5	0	0
      0x80040A00,  //  0011  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: remove_driver
********************************************************************/
be_local_closure(class_Tasmota_remove_driver,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Tasmota,     /* shared constants */
    &be_const_str_remove_driver,
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x8808011D,  //  0000  GETMBR	R2	R0	K29
      0x780A000A,  //  0001  JMPF	R2	#000D
      0x8808011D,  //  0002  GETMBR	R2	R0	K29
      0x8C08051E,  //  0003  GETMET	R2	R2	K30
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x4C0C0000,  //  0006  LDNIL	R3
      0x200C0403,  //  0007  NE	R3	R2	R3
      0x780E0003,  //  0008  JMPF	R3	#000D
      0x880C011D,  //  0009  GETMBR	R3	R0	K29
      0x8C0C071F,  //  000A  GETMET	R3	R3	K31
      0x5C140400,  //  000B  MOVE	R5	R2
      0x7C0C0400,  //  000C  CALL	R3	2
      0x80000000,  //  000D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: exec_tele
********************************************************************/
be_local_closure(class_Tasmota_exec_tele,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Tasmota,     /* shared constants */
    &be_const_str_exec_tele,
    &be_const_str_solidified,
    ( &(const binstruction[41]) {  /* code */
      0x88080120,  //  0000  GETMBR	R2	R0	K32
      0x780A0024,  //  0001  JMPF	R2	#0027
      0xA40A4200,  //  0002  IMPORT	R2	K33
      0x8C0C0522,  //  0003  GETMET	R3	R2	K34
      0x5C140200,  //  0004  MOVE	R5	R1
      0x7C0C0400,  //  0005  CALL	R3	2
      0x50100000,  //  0006  LDBOOL	R4	0	0
      0x4C140000,  //  0007  LDNIL	R5
      0x1C140605,  //  0008  EQ	R5	R3	R5
      0x78160004,  //  0009  JMPF	R5	#000F
      0x8C140123,  //  000A  GETMET	R5	R0	K35
      0x001E4801,  //  000B  ADD	R7	K36	R1
      0x58200025,  //  000C  LDCONST	R8	K37
      0x7C140600,  //  000D  CALL	R5	3
      0x5C0C0200,  //  000E  MOVE	R3	R1
      0x60140013,  //  000F  GETGBL	R5	G19
      0x7C140000,  //  0010  CALL	R5	0
      0x98164C03,  //  0011  SETIDX	R5	K38	R3
      0x5C0C0A00,  //  0012  MOVE	R3	R5
      0x58140006,  //  0013  LDCONST	R5	K6
      0x6018000C,  //  0014  GETGBL	R6	G12
      0x881C0120,  //  0015  GETMBR	R7	R0	K32
      0x7C180200,  //  0016  CALL	R6	1
      0x14180A06,  //  0017  LT	R6	R5	R6
      0x781A000C,  //  0018  JMPF	R6	#0026
      0x88180120,  //  0019  GETMBR	R6	R0	K32
      0x94180C05,  //  001A  GETIDX	R6	R6	R5
      0x8C1C0127,  //  001B  GETMET	R7	R0	K39
      0x5C240600,  //  001C  MOVE	R9	R3
      0x88280D09,  //  001D  GETMBR	R10	R6	K9
      0x882C0D28,  //  001E  GETMBR	R11	R6	K40
      0x7C1C0800,  //  001F  CALL	R7	4
      0x741E0001,  //  0020  JMPT	R7	#0023
      0x74120000,  //  0021  JMPT	R4	#0023
      0x50100001,  //  0022  LDBOOL	R4	0	1
      0x50100200,  //  0023  LDBOOL	R4	1	0
      0x00140B0A,  //  0024  ADD	R5	R5	K10
      0x7001FFED,  //  0025  JMP		#0014
      0x80040800,  //  0026  RET	1	R4
      0x50080000,  //  0027  LDBOOL	R2	0	0
      0x80040400,  //  0028  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: run_cron
********************************************************************/
be_local_closure(class_Tasmota_run_cron,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Tasmota,     /* shared constants */
    &be_const_str_run_cron,
    &be_const_str_solidified,
    ( &(const binstruction[34]) {  /* code */
      0x88040105,  //  0000  GETMBR	R1	R0	K5
      0x7806001E,  //  0001  JMPF	R1	#0021
      0x58040006,  //  0002  LDCONST	R1	K6
      0xB80A5200,  //  0003  GETNGBL	R2	K41
      0x8C08052A,  //  0004  GETMET	R2	R2	K42
      0x7C080200,  //  0005  CALL	R2	1
      0x880C0105,  //  0006  GETMBR	R3	R0	K5
      0x8C0C0707,  //  0007  GETMET	R3	R3	K7
      0x7C0C0200,  //  0008  CALL	R3	1
      0x140C0203,  //  0009  LT	R3	R1	R3
      0x780E0015,  //  000A  JMPF	R3	#0021
      0x880C0105,  //  000B  GETMBR	R3	R0	K5
      0x940C0601,  //  000C  GETIDX	R3	R3	R1
      0x88100709,  //  000D  GETMBR	R4	R3	K9
      0x1C100906,  //  000E  EQ	R4	R4	K6
      0x78120003,  //  000F  JMPF	R4	#0014
      0x8C10072B,  //  0010  GETMET	R4	R3	K43
      0x7C100200,  //  0011  CALL	R4	1
      0x900E1204,  //  0012  SETMBR	R3	K9	R4
      0x7002000A,  //  0013  JMP		#001F
      0x8C10072C,  //  0014  GETMET	R4	R3	K44
      0x7C100200,  //  0015  CALL	R4	1
      0x78120007,  //  0016  JMPF	R4	#001F
      0x88100728,  //  0017  GETMBR	R4	R3	K40
      0x8C14072B,  //  0018  GETMET	R5	R3	K43
      0x7C140200,  //  0019  CALL	R5	1
      0x900E1205,  //  001A  SETMBR	R3	K9	R5
      0x5C180800,  //  001B  MOVE	R6	R4
      0x5C1C0400,  //  001C  MOVE	R7	R2
      0x5C200A00,  //  001D  MOVE	R8	R5
      0x7C180400,  //  001E  CALL	R6	2
      0x0004030A,  //  001F  ADD	R1	R1	K10
      0x7001FFE4,  //  0020  JMP		#0006
      0x80000000,  //  0021  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: defer
********************************************************************/
be_local_closure(class_Tasmota_defer,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Tasmota,     /* shared constants */
    &be_const_str_defer,
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x8808012D,  //  0000  GETMBR	R2	R0	K45
      0x4C0C0000,  //  0001  LDNIL	R3
      0x1C080403,  //  0002  EQ	R2	R2	R3
      0x780A0002,  //  0003  JMPF	R2	#0007
      0x60080012,  //  0004  GETGBL	R2	G18
      0x7C080000,  //  0005  CALL	R2	0
      0x90025A02,  //  0006  SETMBR	R0	K45	R2
      0x8808012D,  //  0007  GETMBR	R2	R0	K45
      0x8C08052E,  //  0008  GETMET	R2	R2	K46
      0x5C100200,  //  0009  MOVE	R4	R1
      0x7C080400,  //  000A  CALL	R2	2
      0xB80A5E00,  //  000B  GETNGBL	R2	K47
      0x88080530,  //  000C  GETMBR	R2	R2	K48
      0x900A630A,  //  000D  SETMBR	R2	K49	K10
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: hs2rgb
********************************************************************/
be_local_closure(class_Tasmota_hs2rgb,   /* name */
  be_nested_proto(
    17,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Tasmota,     /* shared constants */
    &be_const_str_hs2rgb,
    &be_const_str_solidified,
    ( &(const binstruction[68]) {  /* code */
      0x4C0C0000,  //  0000  LDNIL	R3
      0x1C0C0403,  //  0001  EQ	R3	R2	R3
      0x780E0000,  //  0002  JMPF	R3	#0004
      0x540A00FE,  //  0003  LDINT	R2	255
      0x540E00FE,  //  0004  LDINT	R3	255
      0x541200FE,  //  0005  LDINT	R4	255
      0x541600FE,  //  0006  LDINT	R5	255
      0x541A0167,  //  0007  LDINT	R6	360
      0x10040206,  //  0008  MOD	R1	R1	R6
      0x24180506,  //  0009  GT	R6	R2	K6
      0x781A0031,  //  000A  JMPF	R6	#003D
      0x541A003B,  //  000B  LDINT	R6	60
      0x0C180206,  //  000C  DIV	R6	R1	R6
      0x541E003B,  //  000D  LDINT	R7	60
      0x101C0207,  //  000E  MOD	R7	R1	R7
      0x542200FE,  //  000F  LDINT	R8	255
      0x04201002,  //  0010  SUB	R8	R8	R2
      0xB8265E00,  //  0011  GETNGBL	R9	K47
      0x8C241332,  //  0012  GETMET	R9	R9	K50
      0x5C2C0E00,  //  0013  MOVE	R11	R7
      0x58300006,  //  0014  LDCONST	R12	K6
      0x5436003B,  //  0015  LDINT	R13	60
      0x543A00FE,  //  0016  LDINT	R14	255
      0x5C3C1000,  //  0017  MOVE	R15	R8
      0x7C240C00,  //  0018  CALL	R9	6
      0xB82A5E00,  //  0019  GETNGBL	R10	K47
      0x8C281532,  //  001A  GETMET	R10	R10	K50
      0x5C300E00,  //  001B  MOVE	R12	R7
      0x58340006,  //  001C  LDCONST	R13	K6
      0x543A003B,  //  001D  LDINT	R14	60
      0x5C3C1000,  //  001E  MOVE	R15	R8
      0x544200FE,  //  001F  LDINT	R16	255
      0x7C280C00,  //  0020  CALL	R10	6
      0x1C2C0D06,  //  0021  EQ	R11	R6	K6
      0x782E0002,  //  0022  JMPF	R11	#0026
      0x5C141400,  //  0023  MOVE	R5	R10
      0x5C101000,  //  0024  MOVE	R4	R8
      0x70020016,  //  0025  JMP		#003D
      0x1C2C0D0A,  //  0026  EQ	R11	R6	K10
      0x782E0002,  //  0027  JMPF	R11	#002B
      0x5C0C1200,  //  0028  MOVE	R3	R9
      0x5C101000,  //  0029  MOVE	R4	R8
      0x70020011,  //  002A  JMP		#003D
      0x1C2C0D33,  //  002B  EQ	R11	R6	K51
      0x782E0002,  //  002C  JMPF	R11	#0030
      0x5C0C1000,  //  002D  MOVE	R3	R8
      0x5C101400,  //  002E  MOVE	R4	R10
      0x7002000C,  //  002F  JMP		#003D
      0x1C2C0D25,  //  0030  EQ	R11	R6	K37
      0x782E0002,  //  0031  JMPF	R11	#0035
      0x5C0C1000,  //  0032  MOVE	R3	R8
      0x5C141200,  //  0033  MOVE	R5	R9
      0x70020007,  //  0034  JMP		#003D
      0x542E0003,  //  0035  LDINT	R11	4
      0x1C2C0C0B,  //  0036  EQ	R11	R6	R11
      0x782E0002,  //  0037  JMPF	R11	#003B
      0x5C0C1400,  //  0038  MOVE	R3	R10
      0x5C141000,  //  0039  MOVE	R5	R8
      0x70020001,  //  003A  JMP		#003D
      0x5C141000,  //  003B  MOVE	R5	R8
      0x5C101200,  //  003C  MOVE	R4	R9
      0x541A000F,  //  003D  LDINT	R6	16
      0x38180606,  //  003E  SHL	R6	R3	R6
      0x541E0007,  //  003F  LDINT	R7	8
      0x381C0A07,  //  0040  SHL	R7	R5	R7
      0x30180C07,  //  0041  OR	R6	R6	R7
      0x30180C04,  //  0042  OR	R6	R6	R4
      0x80040C00,  //  0043  RET	1	R6
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: remove_fast_loop
********************************************************************/
be_local_closure(class_Tasmota_remove_fast_loop,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Tasmota,     /* shared constants */
    &be_const_str_remove_fast_loop,
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x88080134,  //  0000  GETMBR	R2	R0	K52
      0x740A0000,  //  0001  JMPT	R2	#0003
      0x80000400,  //  0002  RET	0
      0x88080134,  //  0003  GETMBR	R2	R0	K52
      0x8C08051E,  //  0004  GETMET	R2	R2	K30
      0x5C100200,  //  0005  MOVE	R4	R1
      0x7C080400,  //  0006  CALL	R2	2
      0x4C0C0000,  //  0007  LDNIL	R3
      0x200C0403,  //  0008  NE	R3	R2	R3
      0x780E0003,  //  0009  JMPF	R3	#000E
      0x880C0134,  //  000A  GETMBR	R3	R0	K52
      0x8C0C0735,  //  000B  GETMET	R3	R3	K53
      0x5C140400,  //  000C  MOVE	R5	R2
      0x7C0C0400,  //  000D  CALL	R3	2
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: add_rule
********************************************************************/
be_local_closure(class_Tasmota_add_rule,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    5,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Tasmota,     /* shared constants */
    &be_const_str_add_rule,
    &be_const_str_solidified,
    ( &(const binstruction[37]) {  /* code */
      0x8C140136,  //  0000  GETMET	R5	R0	K54
      0x5C1C0400,  //  0001  MOVE	R7	R2
      0x7C140400,  //  0002  CALL	R5	2
      0x88140120,  //  0003  GETMBR	R5	R0	K32
      0x4C180000,  //  0004  LDNIL	R6
      0x1C140A06,  //  0005  EQ	R5	R5	R6
      0x78160002,  //  0006  JMPF	R5	#000A
      0x60140012,  //  0007  GETGBL	R5	G18
      0x7C140000,  //  0008  CALL	R5	0
      0x90024005,  //  0009  SETMBR	R0	K32	R5
      0x60140004,  //  000A  GETGBL	R5	G4
      0x5C180400,  //  000B  MOVE	R6	R2
      0x7C140200,  //  000C  CALL	R5	1
      0x1C140B0C,  //  000D  EQ	R5	R5	K12
      0x78160013,  //  000E  JMPF	R5	#0023
      0x4C140000,  //  000F  LDNIL	R5
      0x20140605,  //  0010  NE	R5	R3	R5
      0x78160003,  //  0011  JMPF	R5	#0016
      0x8C140137,  //  0012  GETMET	R5	R0	K55
      0x5C1C0200,  //  0013  MOVE	R7	R1
      0x5C200600,  //  0014  MOVE	R8	R3
      0x7C140600,  //  0015  CALL	R5	3
      0x88140120,  //  0016  GETMBR	R5	R0	K32
      0x8C140B2E,  //  0017  GETMET	R5	R5	K46
      0xB81E7000,  //  0018  GETNGBL	R7	K56
      0x88200139,  //  0019  GETMBR	R8	R0	K57
      0x8C20113A,  //  001A  GETMET	R8	R8	K58
      0x5C280200,  //  001B  MOVE	R10	R1
      0x7C200400,  //  001C  CALL	R8	2
      0x5C240400,  //  001D  MOVE	R9	R2
      0x5C280600,  //  001E  MOVE	R10	R3
      0x5C2C0800,  //  001F  MOVE	R11	R4
      0x7C1C0800,  //  0020  CALL	R7	4
      0x7C140400,  //  0021  CALL	R5	2
      0x70020000,  //  0022  JMP		#0024
      0xB006773C,  //  0023  RAISE	1	K59	K60
      0x80000000,  //  0024  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_Tasmota_init,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 1]) {
      be_nested_proto(
        10,                          /* nstack */
        4,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 1]) {     /* constants */
        /* K0   */  be_nested_str(urlfetch_cmd),
        }),
        &be_const_str__anonymous_,
        &be_const_str_solidified,
        ( &(const binstruction[ 8]) {  /* code */
          0x68100000,  //  0000  GETUPV	R4	U0
          0x8C100900,  //  0001  GETMET	R4	R4	K0
          0x5C180000,  //  0002  MOVE	R6	R0
          0x5C1C0200,  //  0003  MOVE	R7	R1
          0x5C200400,  //  0004  MOVE	R8	R2
          0x5C240600,  //  0005  MOVE	R9	R3
          0x7C100A00,  //  0006  CALL	R4	5
          0x80000000,  //  0007  RET	0
        })
      ),
    }),
    1,                          /* has constants */
    &be_ktab_class_Tasmota,     /* shared constants */
    &be_const_str_init,
    &be_const_str_solidified,
    ( &(const binstruction[34]) {  /* code */
      0xB8067A00,  //  0000  GETNGBL	R1	K61
      0x8808013E,  //  0001  GETMBR	R2	R0	K62
      0x880C013F,  //  0002  GETMBR	R3	R0	K63
      0x7C040400,  //  0003  CALL	R1	2
      0x90026001,  //  0004  SETMBR	R0	K48	R1
      0xA4061600,  //  0005  IMPORT	R1	K11
      0x60080015,  //  0006  GETGBL	R2	G21
      0x880C0140,  //  0007  GETMBR	R3	R0	K64
      0x54120003,  //  0008  LDINT	R4	4
      0x7C080400,  //  0009  CALL	R2	2
      0x8C080541,  //  000A  GETMET	R2	R2	K65
      0x58100006,  //  000B  LDCONST	R4	K6
      0x54160003,  //  000C  LDINT	R5	4
      0x7C080600,  //  000D  CALL	R2	3
      0x780A0006,  //  000E  JMPF	R2	#0016
      0xB80E7A00,  //  000F  GETNGBL	R3	K61
      0x8C100343,  //  0010  GETMET	R4	R1	K67
      0x5C180400,  //  0011  MOVE	R6	R2
      0x7C100400,  //  0012  CALL	R4	2
      0x88140144,  //  0013  GETMBR	R5	R0	K68
      0x7C0C0400,  //  0014  CALL	R3	2
      0x90028403,  //  0015  SETMBR	R0	K66	R3
      0x90028B46,  //  0016  SETMBR	R0	K69	K70
      0xB80E6000,  //  0017  GETNGBL	R3	K48
      0x8C0C0748,  //  0018  GETMET	R3	R3	K72
      0x58140049,  //  0019  LDCONST	R5	K73
      0x7C0C0400,  //  001A  CALL	R3	2
      0x90028E03,  //  001B  SETMBR	R0	K71	R3
      0x8C0C014A,  //  001C  GETMET	R3	R0	K74
      0x5814004B,  //  001D  LDCONST	R5	K75
      0x84180000,  //  001E  CLOSURE	R6	P0
      0x7C0C0600,  //  001F  CALL	R3	3
      0xA0000000,  //  0020  CLOSE	R0
      0x80000000,  //  0021  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: add_cron
********************************************************************/
be_local_closure(class_Tasmota_add_cron,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Tasmota,     /* shared constants */
    &be_const_str_add_cron,
    &be_const_str_solidified,
    ( &(const binstruction[27]) {  /* code */
      0x8C100136,  //  0000  GETMET	R4	R0	K54
      0x5C180400,  //  0001  MOVE	R6	R2
      0x7C100400,  //  0002  CALL	R4	2
      0x88100105,  //  0003  GETMBR	R4	R0	K5
      0x4C140000,  //  0004  LDNIL	R5
      0x1C100805,  //  0005  EQ	R4	R4	R5
      0x78120002,  //  0006  JMPF	R4	#000A
      0x60100012,  //  0007  GETGBL	R4	G18
      0x7C100000,  //  0008  CALL	R4	0
      0x90020A04,  //  0009  SETMBR	R0	K5	R4
      0xB8125200,  //  000A  GETNGBL	R4	K41
      0x60140008,  //  000B  GETGBL	R5	G8
      0x5C180200,  //  000C  MOVE	R6	R1
      0x7C140200,  //  000D  CALL	R5	1
      0x7C100200,  //  000E  CALL	R4	1
      0x8C14092B,  //  000F  GETMET	R5	R4	K43
      0x7C140200,  //  0010  CALL	R5	1
      0x88180105,  //  0011  GETMBR	R6	R0	K5
      0x8C180D2E,  //  0012  GETMET	R6	R6	K46
      0xB8227000,  //  0013  GETNGBL	R8	K56
      0x5C240A00,  //  0014  MOVE	R9	R5
      0x5C280400,  //  0015  MOVE	R10	R2
      0x5C2C0600,  //  0016  MOVE	R11	R3
      0x5C300800,  //  0017  MOVE	R12	R4
      0x7C200800,  //  0018  CALL	R8	4
      0x7C180400,  //  0019  CALL	R6	2
      0x80000000,  //  001A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_timer
********************************************************************/
be_local_closure(class_Tasmota_set_timer,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Tasmota,     /* shared constants */
    &be_const_str_set_timer,
    &be_const_str_solidified,
    ( &(const binstruction[21]) {  /* code */
      0x8C100136,  //  0000  GETMET	R4	R0	K54
      0x5C180400,  //  0001  MOVE	R6	R2
      0x7C100400,  //  0002  CALL	R4	2
      0x8810014C,  //  0003  GETMBR	R4	R0	K76
      0x4C140000,  //  0004  LDNIL	R5
      0x1C100805,  //  0005  EQ	R4	R4	R5
      0x78120002,  //  0006  JMPF	R4	#000A
      0x60100012,  //  0007  GETGBL	R4	G18
      0x7C100000,  //  0008  CALL	R4	0
      0x90029804,  //  0009  SETMBR	R0	K76	R4
      0x8810014C,  //  000A  GETMBR	R4	R0	K76
      0x8C10092E,  //  000B  GETMET	R4	R4	K46
      0xB81A7000,  //  000C  GETNGBL	R6	K56
      0x8C1C014D,  //  000D  GETMET	R7	R0	K77
      0x5C240200,  //  000E  MOVE	R9	R1
      0x7C1C0400,  //  000F  CALL	R7	2
      0x5C200400,  //  0010  MOVE	R8	R2
      0x5C240600,  //  0011  MOVE	R9	R3
      0x7C180600,  //  0012  CALL	R6	3
      0x7C100400,  //  0013  CALL	R4	2
      0x80000000,  //  0014  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: urlfetch
********************************************************************/
be_local_closure(class_Tasmota_urlfetch,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Tasmota,     /* shared constants */
    &be_const_str_urlfetch,
    &be_const_str_solidified,
    ( &(const binstruction[48]) {  /* code */
      0x4C0C0000,  //  0000  LDNIL	R3
      0x1C0C0403,  //  0001  EQ	R3	R2	R3
      0x780E000D,  //  0002  JMPF	R3	#0011
      0xA40E2200,  //  0003  IMPORT	R3	K17
      0x8C10074E,  //  0004  GETMET	R4	R3	K78
      0x5C180200,  //  0005  MOVE	R6	R1
      0x581C004F,  //  0006  LDCONST	R7	K79
      0x7C100600,  //  0007  CALL	R4	3
      0x8C10091F,  //  0008  GETMET	R4	R4	K31
      0x7C100200,  //  0009  CALL	R4	1
      0x5C080800,  //  000A  MOVE	R2	R4
      0x6010000C,  //  000B  GETGBL	R4	G12
      0x5C140400,  //  000C  MOVE	R5	R2
      0x7C100200,  //  000D  CALL	R4	1
      0x1C100906,  //  000E  EQ	R4	R4	K6
      0x78120000,  //  000F  JMPF	R4	#0011
      0x58080050,  //  0010  LDCONST	R2	K80
      0xB80EA200,  //  0011  GETNGBL	R3	K81
      0x7C0C0000,  //  0012  CALL	R3	0
      0x8C100752,  //  0013  GETMET	R4	R3	K82
      0x50180200,  //  0014  LDBOOL	R6	1	0
      0x7C100400,  //  0015  CALL	R4	2
      0x8C100753,  //  0016  GETMET	R4	R3	K83
      0x5C180200,  //  0017  MOVE	R6	R1
      0x7C100400,  //  0018  CALL	R4	2
      0x8C100754,  //  0019  GETMET	R4	R3	K84
      0x7C100200,  //  001A  CALL	R4	1
      0x541600C7,  //  001B  LDINT	R5	200
      0x20140805,  //  001C  NE	R5	R4	R5
      0x78160004,  //  001D  JMPF	R5	#0023
      0x60140008,  //  001E  GETGBL	R5	G8
      0x5C180800,  //  001F  MOVE	R6	R4
      0x7C140200,  //  0020  CALL	R5	1
      0x0016AA05,  //  0021  ADD	R5	K85	R5
      0xB006AC05,  //  0022  RAISE	1	K86	R5
      0x8C140757,  //  0023  GETMET	R5	R3	K87
      0x5C1C0400,  //  0024  MOVE	R7	R2
      0x7C140400,  //  0025  CALL	R5	2
      0x8C180758,  //  0026  GETMET	R6	R3	K88
      0x7C180200,  //  0027  CALL	R6	1
      0x8C180123,  //  0028  GETMET	R6	R0	K35
      0x60200008,  //  0029  GETGBL	R8	G8
      0x5C240A00,  //  002A  MOVE	R9	R5
      0x7C200200,  //  002B  CALL	R8	1
      0x0022B208,  //  002C  ADD	R8	K89	R8
      0x58240025,  //  002D  LDCONST	R9	K37
      0x7C180600,  //  002E  CALL	R6	3
      0x80040800,  //  002F  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: remove_rule
********************************************************************/
be_local_closure(class_Tasmota_remove_rule,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Tasmota,     /* shared constants */
    &be_const_str_remove_rule,
    &be_const_str_solidified,
    ( &(const binstruction[27]) {  /* code */
      0x880C0120,  //  0000  GETMBR	R3	R0	K32
      0x780E0017,  //  0001  JMPF	R3	#001A
      0x580C0006,  //  0002  LDCONST	R3	K6
      0x6010000C,  //  0003  GETGBL	R4	G12
      0x88140120,  //  0004  GETMBR	R5	R0	K32
      0x7C100200,  //  0005  CALL	R4	1
      0x14100604,  //  0006  LT	R4	R3	R4
      0x78120011,  //  0007  JMPF	R4	#001A
      0x88100120,  //  0008  GETMBR	R4	R0	K32
      0x94100803,  //  0009  GETIDX	R4	R4	R3
      0x88100909,  //  000A  GETMBR	R4	R4	K9
      0x8810095A,  //  000B  GETMBR	R4	R4	K90
      0x1C100801,  //  000C  EQ	R4	R4	R1
      0x78120009,  //  000D  JMPF	R4	#0018
      0x88100120,  //  000E  GETMBR	R4	R0	K32
      0x94100803,  //  000F  GETIDX	R4	R4	R3
      0x88100908,  //  0010  GETMBR	R4	R4	K8
      0x1C100802,  //  0011  EQ	R4	R4	R2
      0x78120004,  //  0012  JMPF	R4	#0018
      0x88100120,  //  0013  GETMBR	R4	R0	K32
      0x8C100935,  //  0014  GETMET	R4	R4	K53
      0x5C180600,  //  0015  MOVE	R6	R3
      0x7C100400,  //  0016  CALL	R4	2
      0x70020000,  //  0017  JMP		#0019
      0x000C070A,  //  0018  ADD	R3	R3	K10
      0x7001FFE8,  //  0019  JMP		#0003
      0x80000000,  //  001A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: add_driver
********************************************************************/
be_local_closure(class_Tasmota_add_driver,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Tasmota,     /* shared constants */
    &be_const_str_add_driver,
    &be_const_str_solidified,
    ( &(const binstruction[25]) {  /* code */
      0x60080004,  //  0000  GETGBL	R2	G4
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x7C080200,  //  0002  CALL	R2	1
      0x2008055B,  //  0003  NE	R2	R2	K91
      0x780A0000,  //  0004  JMPF	R2	#0006
      0xB006775C,  //  0005  RAISE	1	K59	K92
      0x8808011D,  //  0006  GETMBR	R2	R0	K29
      0x780A000B,  //  0007  JMPF	R2	#0014
      0x8808011D,  //  0008  GETMBR	R2	R0	K29
      0x8C08051E,  //  0009  GETMET	R2	R2	K30
      0x5C100200,  //  000A  MOVE	R4	R1
      0x7C080400,  //  000B  CALL	R2	2
      0x4C0C0000,  //  000C  LDNIL	R3
      0x1C080403,  //  000D  EQ	R2	R2	R3
      0x780A0003,  //  000E  JMPF	R2	#0013
      0x8808011D,  //  000F  GETMBR	R2	R0	K29
      0x8C08052E,  //  0010  GETMET	R2	R2	K46
      0x5C100200,  //  0011  MOVE	R4	R1
      0x7C080400,  //  0012  CALL	R2	2
      0x70020003,  //  0013  JMP		#0018
      0x60080012,  //  0014  GETGBL	R2	G18
      0x7C080000,  //  0015  CALL	R2	0
      0x400C0401,  //  0016  CONNECT	R3	R2	R1
      0x90023A02,  //  0017  SETMBR	R0	K29	R2
      0x80000000,  //  0018  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: exec_rules
********************************************************************/
be_local_closure(class_Tasmota_exec_rules,   /* name */
  be_nested_proto(
    14,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Tasmota,     /* shared constants */
    &be_const_str_exec_rules,
    &be_const_str_solidified,
    ( &(const binstruction[60]) {  /* code */
      0x880C015D,  //  0000  GETMBR	R3	R0	K93
      0x88100120,  //  0001  GETMBR	R4	R0	K32
      0x74120002,  //  0002  JMPT	R4	#0006
      0x4C100000,  //  0003  LDNIL	R4
      0x20100604,  //  0004  NE	R4	R3	R4
      0x78120033,  //  0005  JMPF	R4	#003A
      0xA4124200,  //  0006  IMPORT	R4	K33
      0x4C140000,  //  0007  LDNIL	R5
      0x9002BA05,  //  0008  SETMBR	R0	K93	R5
      0x50140000,  //  0009  LDBOOL	R5	0	0
      0x8C180922,  //  000A  GETMET	R6	R4	K34
      0x5C200200,  //  000B  MOVE	R8	R1
      0x7C180400,  //  000C  CALL	R6	2
      0x4C1C0000,  //  000D  LDNIL	R7
      0x1C1C0C07,  //  000E  EQ	R7	R6	R7
      0x781E0004,  //  000F  JMPF	R7	#0015
      0x8C1C0123,  //  0010  GETMET	R7	R0	K35
      0x00264801,  //  0011  ADD	R9	K36	R1
      0x58280025,  //  0012  LDCONST	R10	K37
      0x7C1C0600,  //  0013  CALL	R7	3
      0x5C180200,  //  0014  MOVE	R6	R1
      0x780A001E,  //  0015  JMPF	R2	#0035
      0x881C0120,  //  0016  GETMBR	R7	R0	K32
      0x781E001C,  //  0017  JMPF	R7	#0035
      0x581C0006,  //  0018  LDCONST	R7	K6
      0x6020000C,  //  0019  GETGBL	R8	G12
      0x88240120,  //  001A  GETMBR	R9	R0	K32
      0x7C200200,  //  001B  CALL	R8	1
      0x14200E08,  //  001C  LT	R8	R7	R8
      0x78220016,  //  001D  JMPF	R8	#0035
      0x88200120,  //  001E  GETMBR	R8	R0	K32
      0x94201007,  //  001F  GETIDX	R8	R8	R7
      0x8C240127,  //  0020  GETMET	R9	R0	K39
      0x5C2C0C00,  //  0021  MOVE	R11	R6
      0x88301109,  //  0022  GETMBR	R12	R8	K9
      0x88341128,  //  0023  GETMBR	R13	R8	K40
      0x7C240800,  //  0024  CALL	R9	4
      0x74160001,  //  0025  JMPT	R5	#0028
      0x74260000,  //  0026  JMPT	R9	#0028
      0x50140001,  //  0027  LDBOOL	R5	0	1
      0x50140200,  //  0028  LDBOOL	R5	1	0
      0x78260008,  //  0029  JMPF	R9	#0033
      0x8828115E,  //  002A  GETMBR	R10	R8	K94
      0x502C0200,  //  002B  LDBOOL	R11	1	0
      0x1C28140B,  //  002C  EQ	R10	R10	R11
      0x782A0004,  //  002D  JMPF	R10	#0033
      0x88280120,  //  002E  GETMBR	R10	R0	K32
      0x8C281535,  //  002F  GETMET	R10	R10	K53
      0x5C300E00,  //  0030  MOVE	R12	R7
      0x7C280400,  //  0031  CALL	R10	2
      0x70020000,  //  0032  JMP		#0034
      0x001C0F0A,  //  0033  ADD	R7	R7	K10
      0x7001FFE3,  //  0034  JMP		#0019
      0x4C1C0000,  //  0035  LDNIL	R7
      0x201C0607,  //  0036  NE	R7	R3	R7
      0x781E0000,  //  0037  JMPF	R7	#0039
      0x9002BA06,  //  0038  SETMBR	R0	K93	R6
      0x80040A00,  //  0039  RET	1	R5
      0x50100000,  //  003A  LDBOOL	R4	0	0
      0x80040800,  //  003B  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: int
********************************************************************/
be_local_closure(class_Tasmota_int,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    3,                          /* argc */
    12,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Tasmota,     /* shared constants */
    &be_const_str_int,
    &be_const_str_solidified,
    ( &(const binstruction[46]) {  /* code */
      0x580C005F,  //  0000  LDCONST	R3	K95
      0x60100009,  //  0001  GETGBL	R4	G9
      0x5C140000,  //  0002  MOVE	R5	R0
      0x7C100200,  //  0003  CALL	R4	1
      0x5C000800,  //  0004  MOVE	R0	R4
      0x4C100000,  //  0005  LDNIL	R4
      0x1C100204,  //  0006  EQ	R4	R1	R4
      0x78120003,  //  0007  JMPF	R4	#000C
      0x4C100000,  //  0008  LDNIL	R4
      0x1C100404,  //  0009  EQ	R4	R2	R4
      0x78120000,  //  000A  JMPF	R4	#000C
      0x80040000,  //  000B  RET	1	R0
      0x60100009,  //  000C  GETGBL	R4	G9
      0x5C140200,  //  000D  MOVE	R5	R1
      0x7C100200,  //  000E  CALL	R4	1
      0x5C040800,  //  000F  MOVE	R1	R4
      0x60100009,  //  0010  GETGBL	R4	G9
      0x5C140400,  //  0011  MOVE	R5	R2
      0x7C100200,  //  0012  CALL	R4	1
      0x5C080800,  //  0013  MOVE	R2	R4
      0x4C100000,  //  0014  LDNIL	R4
      0x20100204,  //  0015  NE	R4	R1	R4
      0x78120006,  //  0016  JMPF	R4	#001E
      0x4C100000,  //  0017  LDNIL	R4
      0x20100404,  //  0018  NE	R4	R2	R4
      0x78120003,  //  0019  JMPF	R4	#001E
      0x4C100000,  //  001A  LDNIL	R4
      0x1C100004,  //  001B  EQ	R4	R0	R4
      0x78120000,  //  001C  JMPF	R4	#001E
      0x80040200,  //  001D  RET	1	R1
      0x4C100000,  //  001E  LDNIL	R4
      0x20100004,  //  001F  NE	R4	R0	R4
      0x7812000B,  //  0020  JMPF	R4	#002D
      0x4C100000,  //  0021  LDNIL	R4
      0x20100204,  //  0022  NE	R4	R1	R4
      0x78120002,  //  0023  JMPF	R4	#0027
      0x14100001,  //  0024  LT	R4	R0	R1
      0x78120000,  //  0025  JMPF	R4	#0027
      0x80040200,  //  0026  RET	1	R1
      0x4C100000,  //  0027  LDNIL	R4
      0x20100404,  //  0028  NE	R4	R2	R4
      0x78120002,  //  0029  JMPF	R4	#002D
      0x24100002,  //  002A  GT	R4	R0	R2
      0x78120000,  //  002B  JMPF	R4	#002D
      0x80040400,  //  002C  RET	1	R2
      0x80040000,  //  002D  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: gc
********************************************************************/
be_local_closure(class_Tasmota_gc,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Tasmota,     /* shared constants */
    &be_const_str_gc,
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0xA406C000,  //  0000  IMPORT	R1	K96
      0x8C080361,  //  0001  GETMET	R2	R1	K97
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080362,  //  0003  GETMET	R2	R1	K98
      0x7C080200,  //  0004  CALL	R2	1
      0x80040400,  //  0005  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: run_deferred
********************************************************************/
be_local_closure(class_Tasmota_run_deferred,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Tasmota,     /* shared constants */
    &be_const_str_run_deferred,
    &be_const_str_solidified,
    ( &(const binstruction[26]) {  /* code */
      0x8804012D,  //  0000  GETMBR	R1	R0	K45
      0x78060016,  //  0001  JMPF	R1	#0019
      0x6004000C,  //  0002  GETGBL	R1	G12
      0x8808012D,  //  0003  GETMBR	R2	R0	K45
      0x7C040200,  //  0004  CALL	R1	1
      0x24080306,  //  0005  GT	R2	R1	K6
      0x780A0009,  //  0006  JMPF	R2	#0011
      0x8808012D,  //  0007  GETMBR	R2	R0	K45
      0x94080506,  //  0008  GETIDX	R2	R2	K6
      0x880C012D,  //  0009  GETMBR	R3	R0	K45
      0x8C0C0735,  //  000A  GETMET	R3	R3	K53
      0x58140006,  //  000B  LDCONST	R5	K6
      0x7C0C0400,  //  000C  CALL	R3	2
      0x0404030A,  //  000D  SUB	R1	R1	K10
      0x5C0C0400,  //  000E  MOVE	R3	R2
      0x7C0C0000,  //  000F  CALL	R3	0
      0x7001FFF3,  //  0010  JMP		#0005
      0x6008000C,  //  0011  GETGBL	R2	G12
      0x880C012D,  //  0012  GETMBR	R3	R0	K45
      0x7C080200,  //  0013  CALL	R2	1
      0x1C080506,  //  0014  EQ	R2	R2	K6
      0x780A0002,  //  0015  JMPF	R2	#0019
      0xB80A5E00,  //  0016  GETNGBL	R2	K47
      0x88080530,  //  0017  GETMBR	R2	R2	K48
      0x900A6306,  //  0018  SETMBR	R2	K49	K6
      0x80000000,  //  0019  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: add_rule_once
********************************************************************/
be_local_closure(class_Tasmota_add_rule_once,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Tasmota,     /* shared constants */
    &be_const_str_add_rule_once,
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x8C100163,  //  0000  GETMET	R4	R0	K99
      0x5C180200,  //  0001  MOVE	R6	R1
      0x5C1C0400,  //  0002  MOVE	R7	R2
      0x5C200600,  //  0003  MOVE	R8	R3
      0x50240200,  //  0004  LDBOOL	R9	1	0
      0x7C100A00,  //  0005  CALL	R4	5
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: remove_cron
********************************************************************/
be_local_closure(class_Tasmota_remove_cron,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Tasmota,     /* shared constants */
    &be_const_str_remove_cron,
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x88080105,  //  0000  GETMBR	R2	R0	K5
      0x780A000E,  //  0001  JMPF	R2	#0011
      0x580C0006,  //  0002  LDCONST	R3	K6
      0x8C100507,  //  0003  GETMET	R4	R2	K7
      0x7C100200,  //  0004  CALL	R4	1
      0x14100604,  //  0005  LT	R4	R3	R4
      0x78120009,  //  0006  JMPF	R4	#0011
      0x94100403,  //  0007  GETIDX	R4	R2	R3
      0x88100908,  //  0008  GETMBR	R4	R4	K8
      0x1C100801,  //  0009  EQ	R4	R4	R1
      0x78120003,  //  000A  JMPF	R4	#000F
      0x8C100535,  //  000B  GETMET	R4	R2	K53
      0x5C180600,  //  000C  MOVE	R6	R3
      0x7C100400,  //  000D  CALL	R4	2
      0x70020000,  //  000E  JMP		#0010
      0x000C070A,  //  000F  ADD	R3	R3	K10
      0x7001FFF1,  //  0010  JMP		#0003
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: fast_loop
********************************************************************/
be_local_closure(class_Tasmota_fast_loop,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Tasmota,     /* shared constants */
    &be_const_str_fast_loop,
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x88040134,  //  0000  GETMBR	R1	R0	K52
      0x5C080200,  //  0001  MOVE	R2	R1
      0x740A0000,  //  0002  JMPT	R2	#0004
      0x80000400,  //  0003  RET	0
      0x58080006,  //  0004  LDCONST	R2	K6
      0x600C000C,  //  0005  GETGBL	R3	G12
      0x5C100200,  //  0006  MOVE	R4	R1
      0x7C0C0200,  //  0007  CALL	R3	1
      0x140C0403,  //  0008  LT	R3	R2	R3
      0x780E0003,  //  0009  JMPF	R3	#000E
      0x940C0202,  //  000A  GETIDX	R3	R1	R2
      0x7C0C0000,  //  000B  CALL	R3	0
      0x0008050A,  //  000C  ADD	R2	R2	K10
      0x7001FFF6,  //  000D  JMP		#0005
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: event
********************************************************************/
be_local_closure(class_Tasmota_event,   /* name */
  be_nested_proto(
    19,                          /* nstack */
    6,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Tasmota,     /* shared constants */
    &be_const_str_event,
    &be_const_str_solidified,
    ( &(const binstruction[108]) {  /* code */
      0xA41A1600,  //  0000  IMPORT	R6	K11
      0x1C1C0364,  //  0001  EQ	R7	R1	K100
      0x781E0001,  //  0002  JMPF	R7	#0005
      0x8C1C0165,  //  0003  GETMET	R7	R0	K101
      0x7C1C0200,  //  0004  CALL	R7	1
      0x1C1C0366,  //  0005  EQ	R7	R1	K102
      0x781E0001,  //  0006  JMPF	R7	#0009
      0x8C1C0167,  //  0007  GETMET	R7	R0	K103
      0x7C1C0200,  //  0008  CALL	R7	1
      0x501C0000,  //  0009  LDBOOL	R7	0	0
      0x50200000,  //  000A  LDBOOL	R8	0	0
      0x1C240368,  //  000B  EQ	R9	R1	K104
      0x78260000,  //  000C  JMPF	R9	#000E
      0x50200200,  //  000D  LDBOOL	R8	1	0
      0x1C240369,  //  000E  EQ	R9	R1	K105
      0x78260006,  //  000F  JMPF	R9	#0017
      0x8C24016A,  //  0010  GETMET	R9	R0	K106
      0x5C2C0400,  //  0011  MOVE	R11	R2
      0x5C300600,  //  0012  MOVE	R12	R3
      0x5C340800,  //  0013  MOVE	R13	R4
      0x7C240800,  //  0014  CALL	R9	4
      0x80041200,  //  0015  RET	1	R9
      0x7002004E,  //  0016  JMP		#0066
      0x1C24036B,  //  0017  EQ	R9	R1	K107
      0x78260004,  //  0018  JMPF	R9	#001E
      0x8C24016C,  //  0019  GETMET	R9	R0	K108
      0x5C2C0800,  //  001A  MOVE	R11	R4
      0x7C240400,  //  001B  CALL	R9	2
      0x80041200,  //  001C  RET	1	R9
      0x70020047,  //  001D  JMP		#0066
      0x1C24035A,  //  001E  EQ	R9	R1	K90
      0x78260007,  //  001F  JMPF	R9	#0028
      0x8C24016D,  //  0020  GETMET	R9	R0	K109
      0x5C2C0800,  //  0021  MOVE	R11	R4
      0x60300017,  //  0022  GETGBL	R12	G23
      0x5C340600,  //  0023  MOVE	R13	R3
      0x7C300200,  //  0024  CALL	R12	1
      0x7C240600,  //  0025  CALL	R9	3
      0x80041200,  //  0026  RET	1	R9
      0x7002003D,  //  0027  JMP		#0066
      0x1C240360,  //  0028  EQ	R9	R1	K96
      0x78260003,  //  0029  JMPF	R9	#002E
      0x8C240160,  //  002A  GETMET	R9	R0	K96
      0x7C240200,  //  002B  CALL	R9	1
      0x80041200,  //  002C  RET	1	R9
      0x70020037,  //  002D  JMP		#0066
      0x8824011D,  //  002E  GETMBR	R9	R0	K29
      0x78260035,  //  002F  JMPF	R9	#0066
      0x58240006,  //  0030  LDCONST	R9	K6
      0x6028000C,  //  0031  GETGBL	R10	G12
      0x882C011D,  //  0032  GETMBR	R11	R0	K29
      0x7C280200,  //  0033  CALL	R10	1
      0x1428120A,  //  0034  LT	R10	R9	R10
      0x782A002F,  //  0035  JMPF	R10	#0066
      0x8828011D,  //  0036  GETMBR	R10	R0	K29
      0x94281409,  //  0037  GETIDX	R10	R10	R9
      0x8C2C0D41,  //  0038  GETMET	R11	R6	K65
      0x5C341400,  //  0039  MOVE	R13	R10
      0x5C380200,  //  003A  MOVE	R14	R1
      0x7C2C0600,  //  003B  CALL	R11	3
      0x60300004,  //  003C  GETGBL	R12	G4
      0x5C341600,  //  003D  MOVE	R13	R11
      0x7C300200,  //  003E  CALL	R12	1
      0x1C30190C,  //  003F  EQ	R12	R12	K12
      0x78320022,  //  0040  JMPF	R12	#0064
      0xA8020011,  //  0041  EXBLK	0	#0054
      0x5C301600,  //  0042  MOVE	R12	R11
      0x5C341400,  //  0043  MOVE	R13	R10
      0x5C380400,  //  0044  MOVE	R14	R2
      0x5C3C0600,  //  0045  MOVE	R15	R3
      0x5C400800,  //  0046  MOVE	R16	R4
      0x5C440A00,  //  0047  MOVE	R17	R5
      0x7C300A00,  //  0048  CALL	R12	5
      0x74320001,  //  0049  JMPT	R12	#004C
      0x741E0000,  //  004A  JMPT	R7	#004C
      0x501C0001,  //  004B  LDBOOL	R7	0	1
      0x501C0200,  //  004C  LDBOOL	R7	1	0
      0x781E0003,  //  004D  JMPF	R7	#0052
      0x5C301000,  //  004E  MOVE	R12	R8
      0x74320001,  //  004F  JMPT	R12	#0052
      0xA8040001,  //  0050  EXBLK	1	1
      0x70020013,  //  0051  JMP		#0066
      0xA8040001,  //  0052  EXBLK	1	1
      0x7002000F,  //  0053  JMP		#0064
      0xAC300002,  //  0054  CATCH	R12	0	2
      0x7002000C,  //  0055  JMP		#0063
      0x60380001,  //  0056  GETGBL	R14	G1
      0x603C0018,  //  0057  GETGBL	R15	G24
      0x5840006E,  //  0058  LDCONST	R16	K110
      0x5C441800,  //  0059  MOVE	R17	R12
      0x5C481A00,  //  005A  MOVE	R18	R13
      0x7C3C0600,  //  005B  CALL	R15	3
      0x7C380200,  //  005C  CALL	R14	1
      0x88380147,  //  005D  GETMBR	R14	R0	K71
      0x783A0002,  //  005E  JMPF	R14	#0062
      0xA43A9200,  //  005F  IMPORT	R14	K73
      0x8C3C1D6F,  //  0060  GETMET	R15	R14	K111
      0x7C3C0200,  //  0061  CALL	R15	1
      0x70020000,  //  0062  JMP		#0064
      0xB0080000,  //  0063  RAISE	2	R0	R0
      0x0024130A,  //  0064  ADD	R9	R9	K10
      0x7001FFCA,  //  0065  JMP		#0031
      0x1C240370,  //  0066  EQ	R9	R1	K112
      0x78260002,  //  0067  JMPF	R9	#006B
      0xA426E200,  //  0068  IMPORT	R9	K113
      0x8C281372,  //  0069  GETMET	R10	R9	K114
      0x7C280200,  //  006A  CALL	R10	1
      0x80040E00,  //  006B  RET	1	R7
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: find_key_i
********************************************************************/
be_local_closure(class_Tasmota_find_key_i,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Tasmota,     /* shared constants */
    &be_const_str_find_key_i,
    &be_const_str_solidified,
    ( &(const binstruction[30]) {  /* code */
      0xA40E2200,  //  0000  IMPORT	R3	K17
      0x8C100712,  //  0001  GETMET	R4	R3	K18
      0x5C180400,  //  0002  MOVE	R6	R2
      0x7C100400,  //  0003  CALL	R4	2
      0x6014000F,  //  0004  GETGBL	R5	G15
      0x5C180200,  //  0005  MOVE	R6	R1
      0x601C0013,  //  0006  GETGBL	R7	G19
      0x7C140400,  //  0007  CALL	R5	2
      0x78160013,  //  0008  JMPF	R5	#001D
      0x60140010,  //  0009  GETGBL	R5	G16
      0x8C180373,  //  000A  GETMET	R6	R1	K115
      0x7C180200,  //  000B  CALL	R6	1
      0x7C140200,  //  000C  CALL	R5	1
      0xA802000B,  //  000D  EXBLK	0	#001A
      0x5C180A00,  //  000E  MOVE	R6	R5
      0x7C180000,  //  000F  CALL	R6	0
      0x8C1C0712,  //  0010  GETMET	R7	R3	K18
      0x5C240C00,  //  0011  MOVE	R9	R6
      0x7C1C0400,  //  0012  CALL	R7	2
      0x1C1C0E04,  //  0013  EQ	R7	R7	R4
      0x741E0001,  //  0014  JMPT	R7	#0017
      0x1C1C0574,  //  0015  EQ	R7	R2	K116
      0x781E0001,  //  0016  JMPF	R7	#0019
      0xA8040001,  //  0017  EXBLK	1	1
      0x80040C00,  //  0018  RET	1	R6
      0x7001FFF3,  //  0019  JMP		#000E
      0x58140075,  //  001A  LDCONST	R5	K117
      0xAC140200,  //  001B  CATCH	R5	1	0
      0xB0080000,  //  001C  RAISE	2	R0	R0
      0x80000000,  //  001D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_light
********************************************************************/
be_local_closure(class_Tasmota_get_light,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Tasmota,     /* shared constants */
    &be_const_str_get_light,
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0x60080001,  //  0000  GETGBL	R2	G1
      0x580C0076,  //  0001  LDCONST	R3	K118
      0x7C080200,  //  0002  CALL	R2	1
      0xA40AEE00,  //  0003  IMPORT	R2	K119
      0x4C0C0000,  //  0004  LDNIL	R3
      0x200C0203,  //  0005  NE	R3	R1	R3
      0x780E0004,  //  0006  JMPF	R3	#000C
      0x8C0C0541,  //  0007  GETMET	R3	R2	K65
      0x5C140200,  //  0008  MOVE	R5	R1
      0x7C0C0400,  //  0009  CALL	R3	2
      0x80040600,  //  000A  RET	1	R3
      0x70020002,  //  000B  JMP		#000F
      0x8C0C0541,  //  000C  GETMET	R3	R2	K65
      0x7C0C0200,  //  000D  CALL	R3	1
      0x80040600,  //  000E  RET	1	R3
      0x80000000,  //  000F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: add_fast_loop
********************************************************************/
be_local_closure(class_Tasmota_add_fast_loop,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Tasmota,     /* shared constants */
    &be_const_str_add_fast_loop,
    &be_const_str_solidified,
    ( &(const binstruction[23]) {  /* code */
      0x8C080136,  //  0000  GETMET	R2	R0	K54
      0x5C100200,  //  0001  MOVE	R4	R1
      0x7C080400,  //  0002  CALL	R2	2
      0x88080134,  //  0003  GETMBR	R2	R0	K52
      0x4C0C0000,  //  0004  LDNIL	R3
      0x1C080403,  //  0005  EQ	R2	R2	R3
      0x780A0002,  //  0006  JMPF	R2	#000A
      0x60080012,  //  0007  GETGBL	R2	G18
      0x7C080000,  //  0008  CALL	R2	0
      0x90026802,  //  0009  SETMBR	R0	K52	R2
      0x60080004,  //  000A  GETGBL	R2	G4
      0x5C0C0200,  //  000B  MOVE	R3	R1
      0x7C080200,  //  000C  CALL	R2	1
      0x2008050C,  //  000D  NE	R2	R2	K12
      0x780A0000,  //  000E  JMPF	R2	#0010
      0xB0067778,  //  000F  RAISE	1	K59	K120
      0x88080130,  //  0010  GETMBR	R2	R0	K48
      0x900AF30A,  //  0011  SETMBR	R2	K121	K10
      0x88080134,  //  0012  GETMBR	R2	R0	K52
      0x8C08052E,  //  0013  GETMET	R2	R2	K46
      0x5C100200,  //  0014  MOVE	R4	R1
      0x7C080400,  //  0015  CALL	R2	2
      0x80000000,  //  0016  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: cmd
********************************************************************/
be_local_closure(class_Tasmota_cmd,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Tasmota,     /* shared constants */
    &be_const_str_cmd,
    &be_const_str_solidified,
    ( &(const binstruction[27]) {  /* code */
      0x880C015D,  //  0000  GETMBR	R3	R0	K93
      0x50100200,  //  0001  LDBOOL	R4	1	0
      0x9002BA04,  //  0002  SETMBR	R0	K93	R4
      0xB8125E00,  //  0003  GETNGBL	R4	K47
      0x88100930,  //  0004  GETMBR	R4	R4	K48
      0x8810097A,  //  0005  GETMBR	R4	R4	K122
      0x780A0004,  //  0006  JMPF	R2	#000C
      0x28140933,  //  0007  GE	R5	R4	K51
      0x78160002,  //  0008  JMPF	R5	#000C
      0xB8165E00,  //  0009  GETNGBL	R5	K47
      0x88140B30,  //  000A  GETMBR	R5	R5	K48
      0x9016F50A,  //  000B  SETMBR	R5	K122	K10
      0x8C14017B,  //  000C  GETMET	R5	R0	K123
      0x5C1C0200,  //  000D  MOVE	R7	R1
      0x7C140400,  //  000E  CALL	R5	2
      0x4C140000,  //  000F  LDNIL	R5
      0x8818015D,  //  0010  GETMBR	R6	R0	K93
      0x501C0200,  //  0011  LDBOOL	R7	1	0
      0x20180C07,  //  0012  NE	R6	R6	R7
      0x781A0000,  //  0013  JMPF	R6	#0015
      0x8814015D,  //  0014  GETMBR	R5	R0	K93
      0x9002BA03,  //  0015  SETMBR	R0	K93	R3
      0x780A0002,  //  0016  JMPF	R2	#001A
      0xB81A5E00,  //  0017  GETNGBL	R6	K47
      0x88180D30,  //  0018  GETMBR	R6	R6	K48
      0x901AF404,  //  0019  SETMBR	R6	K122	R4
      0x80040A00,  //  001A  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: run_timers
********************************************************************/
be_local_closure(class_Tasmota_run_timers,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Tasmota,     /* shared constants */
    &be_const_str_run_timers,
    &be_const_str_solidified,
    ( &(const binstruction[27]) {  /* code */
      0x8C04017C,  //  0000  GETMET	R1	R0	K124
      0x7C040200,  //  0001  CALL	R1	1
      0x8804014C,  //  0002  GETMBR	R1	R0	K76
      0x78060015,  //  0003  JMPF	R1	#001A
      0x58040006,  //  0004  LDCONST	R1	K6
      0x8808014C,  //  0005  GETMBR	R2	R0	K76
      0x8C080507,  //  0006  GETMET	R2	R2	K7
      0x7C080200,  //  0007  CALL	R2	1
      0x14080202,  //  0008  LT	R2	R1	R2
      0x780A000F,  //  0009  JMPF	R2	#001A
      0x8808014C,  //  000A  GETMBR	R2	R0	K76
      0x94080401,  //  000B  GETIDX	R2	R2	R1
      0x8C0C012C,  //  000C  GETMET	R3	R0	K44
      0x88140509,  //  000D  GETMBR	R5	R2	K9
      0x7C0C0400,  //  000E  CALL	R3	2
      0x780E0007,  //  000F  JMPF	R3	#0018
      0x880C0528,  //  0010  GETMBR	R3	R2	K40
      0x8810014C,  //  0011  GETMBR	R4	R0	K76
      0x8C100935,  //  0012  GETMET	R4	R4	K53
      0x5C180200,  //  0013  MOVE	R6	R1
      0x7C100400,  //  0014  CALL	R4	2
      0x5C100600,  //  0015  MOVE	R4	R3
      0x7C100000,  //  0016  CALL	R4	0
      0x70020000,  //  0017  JMP		#0019
      0x0004030A,  //  0018  ADD	R1	R1	K10
      0x7001FFEA,  //  0019  JMP		#0005
      0x80000000,  //  001A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: exec_cmd
********************************************************************/
be_local_closure(class_Tasmota_exec_cmd,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Tasmota,     /* shared constants */
    &be_const_str_exec_cmd,
    &be_const_str_solidified,
    ( &(const binstruction[27]) {  /* code */
      0x8810017D,  //  0000  GETMBR	R4	R0	K125
      0x78120016,  //  0001  JMPF	R4	#0019
      0xA4124200,  //  0002  IMPORT	R4	K33
      0x8C140922,  //  0003  GETMET	R5	R4	K34
      0x5C1C0600,  //  0004  MOVE	R7	R3
      0x7C140400,  //  0005  CALL	R5	2
      0x8C18017E,  //  0006  GETMET	R6	R0	K126
      0x8820017D,  //  0007  GETMBR	R8	R0	K125
      0x5C240200,  //  0008  MOVE	R9	R1
      0x7C180600,  //  0009  CALL	R6	3
      0x4C1C0000,  //  000A  LDNIL	R7
      0x201C0C07,  //  000B  NE	R7	R6	R7
      0x781E000B,  //  000C  JMPF	R7	#0019
      0x8C1C017F,  //  000D  GETMET	R7	R0	K127
      0x5C240C00,  //  000E  MOVE	R9	R6
      0x7C1C0400,  //  000F  CALL	R7	2
      0x881C017D,  //  0010  GETMBR	R7	R0	K125
      0x941C0E06,  //  0011  GETIDX	R7	R7	R6
      0x5C200C00,  //  0012  MOVE	R8	R6
      0x5C240400,  //  0013  MOVE	R9	R2
      0x5C280600,  //  0014  MOVE	R10	R3
      0x5C2C0A00,  //  0015  MOVE	R11	R5
      0x7C1C0800,  //  0016  CALL	R7	4
      0x501C0200,  //  0017  LDBOOL	R7	1	0
      0x80040E00,  //  0018  RET	1	R7
      0x50100000,  //  0019  LDBOOL	R4	0	0
      0x80040800,  //  001A  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: add_cmd
********************************************************************/
be_local_closure(class_Tasmota_add_cmd,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Tasmota,     /* shared constants */
    &be_const_str_add_cmd,
    &be_const_str_solidified,
    ( &(const binstruction[20]) {  /* code */
      0x8C0C0136,  //  0000  GETMET	R3	R0	K54
      0x5C140400,  //  0001  MOVE	R5	R2
      0x7C0C0400,  //  0002  CALL	R3	2
      0x880C017D,  //  0003  GETMBR	R3	R0	K125
      0x4C100000,  //  0004  LDNIL	R4
      0x1C0C0604,  //  0005  EQ	R3	R3	R4
      0x780E0002,  //  0006  JMPF	R3	#000A
      0x600C0013,  //  0007  GETGBL	R3	G19
      0x7C0C0000,  //  0008  CALL	R3	0
      0x9002FA03,  //  0009  SETMBR	R0	K125	R3
      0x600C0004,  //  000A  GETGBL	R3	G4
      0x5C100400,  //  000B  MOVE	R4	R2
      0x7C0C0200,  //  000C  CALL	R3	1
      0x1C0C070C,  //  000D  EQ	R3	R3	K12
      0x780E0002,  //  000E  JMPF	R3	#0012
      0x880C017D,  //  000F  GETMBR	R3	R0	K125
      0x980C0202,  //  0010  SETIDX	R3	R1	R2
      0x70020000,  //  0011  JMP		#0013
      0xB006773C,  //  0012  RAISE	1	K59	K60
      0x80000000,  //  0013  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: find_op
********************************************************************/
be_local_closure(class_Tasmota_find_op,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Tasmota,     /* shared constants */
    &be_const_str_find_op,
    &be_const_str_solidified,
    ( &(const binstruction[31]) {  /* code */
      0x8C080180,  //  0000  GETMET	R2	R0	K128
      0x5C100200,  //  0001  MOVE	R4	R1
      0x7C080400,  //  0002  CALL	R2	2
      0x280C0506,  //  0003  GE	R3	R2	K6
      0x780E0011,  //  0004  JMPF	R3	#0017
      0x540E7FFE,  //  0005  LDINT	R3	32767
      0x2C0C0403,  //  0006  AND	R3	R2	R3
      0x5412000F,  //  0007  LDINT	R4	16
      0x3C100404,  //  0008  SHR	R4	R2	R4
      0x60140012,  //  0009  GETGBL	R5	G18
      0x7C140000,  //  000A  CALL	R5	0
      0x0418070A,  //  000B  SUB	R6	R3	K10
      0x401A0C06,  //  000C  CONNECT	R6	K6	R6
      0x94180206,  //  000D  GETIDX	R6	R1	R6
      0x40180A06,  //  000E  CONNECT	R6	R5	R6
      0x0418090A,  //  000F  SUB	R6	R4	K10
      0x40180606,  //  0010  CONNECT	R6	R3	R6
      0x94180206,  //  0011  GETIDX	R6	R1	R6
      0x40180A06,  //  0012  CONNECT	R6	R5	R6
      0x40180981,  //  0013  CONNECT	R6	R4	K129
      0x94180206,  //  0014  GETIDX	R6	R1	R6
      0x40180A06,  //  0015  CONNECT	R6	R5	R6
      0x80040A00,  //  0016  RET	1	R5
      0x600C0012,  //  0017  GETGBL	R3	G18
      0x7C0C0000,  //  0018  CALL	R3	0
      0x40100601,  //  0019  CONNECT	R4	R3	R1
      0x4C100000,  //  001A  LDNIL	R4
      0x40100604,  //  001B  CONNECT	R4	R3	R4
      0x4C100000,  //  001C  LDNIL	R4
      0x40100604,  //  001D  CONNECT	R4	R3	R4
      0x80040600,  //  001E  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: load
********************************************************************/
be_local_closure(class_Tasmota_load,   /* name */
  be_nested_proto(
    26,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 6]) {
      be_nested_proto(
        6,                          /* nstack */
        1,                          /* argc */
        0,                          /* varg */
        0,                          /* has upvals */
        NULL,                       /* no upvals */
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 4]) {     /* constants */
        /* K0   */  be_nested_str(sys),
        /* K1   */  be_nested_str(path),
        /* K2   */  be_nested_str(find),
        /* K3   */  be_nested_str(push),
        }),
        &be_const_str_push_path,
        &be_const_str_solidified,
        ( &(const binstruction[13]) {  /* code */
          0xA4060000,  //  0000  IMPORT	R1	K0
          0x8C080301,  //  0001  GETMET	R2	R1	K1
          0x7C080200,  //  0002  CALL	R2	1
          0x8C0C0502,  //  0003  GETMET	R3	R2	K2
          0x5C140000,  //  0004  MOVE	R5	R0
          0x7C0C0400,  //  0005  CALL	R3	2
          0x4C100000,  //  0006  LDNIL	R4
          0x1C0C0604,  //  0007  EQ	R3	R3	R4
          0x780E0002,  //  0008  JMPF	R3	#000C
          0x8C0C0503,  //  0009  GETMET	R3	R2	K3
          0x5C140000,  //  000A  MOVE	R5	R0
          0x7C0C0400,  //  000B  CALL	R3	2
          0x80000000,  //  000C  RET	0
        })
      ),
      be_nested_proto(
        7,                          /* nstack */
        1,                          /* argc */
        0,                          /* varg */
        0,                          /* has upvals */
        NULL,                       /* no upvals */
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 4]) {     /* constants */
        /* K0   */  be_nested_str(sys),
        /* K1   */  be_nested_str(path),
        /* K2   */  be_nested_str(find),
        /* K3   */  be_nested_str(remove),
        }),
        &be_const_str_pop_path,
        &be_const_str_solidified,
        ( &(const binstruction[13]) {  /* code */
          0xA4060000,  //  0000  IMPORT	R1	K0
          0x8C080301,  //  0001  GETMET	R2	R1	K1
          0x7C080200,  //  0002  CALL	R2	1
          0x8C0C0502,  //  0003  GETMET	R3	R2	K2
          0x5C140000,  //  0004  MOVE	R5	R0
          0x7C0C0400,  //  0005  CALL	R3	2
          0x4C100000,  //  0006  LDNIL	R4
          0x20100604,  //  0007  NE	R4	R3	R4
          0x78120002,  //  0008  JMPF	R4	#000C
          0x8C100503,  //  0009  GETMET	R4	R2	K3
          0x5C180600,  //  000A  MOVE	R6	R3
          0x7C100400,  //  000B  CALL	R4	2
          0x80000000,  //  000C  RET	0
        })
      ),
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
        /* K0   */  be_nested_str(r),
        /* K1   */  be_nested_str(readbytes),
        /* K2   */  be_const_int(3),
        /* K3   */  be_const_int(1),
        /* K4   */  be_nested_str(BECDFE),
        /* K5   */  be_const_int(0),
        /* K6   */  be_nested_str(close),
        /* K7   */  be_nested_str(BRY_X3A_X20failed_X20to_X20load_X20compiled_X20_X27_X25s_X27_X20_X28_X25s_X29),
        }),
        &be_const_str_try_get_bec_version,
        &be_const_str_solidified,
        ( &(const binstruction[43]) {  /* code */
          0x4C040000,  //  0000  LDNIL	R1
          0xA8020016,  //  0001  EXBLK	0	#0019
          0x60080011,  //  0002  GETGBL	R2	G17
          0x5C0C0000,  //  0003  MOVE	R3	R0
          0x58100000,  //  0004  LDCONST	R4	K0
          0x7C080400,  //  0005  CALL	R2	2
          0x5C040400,  //  0006  MOVE	R1	R2
          0x8C080301,  //  0007  GETMET	R2	R1	K1
          0x58100002,  //  0008  LDCONST	R4	K2
          0x7C080400,  //  0009  CALL	R2	2
          0x8C0C0301,  //  000A  GETMET	R3	R1	K1
          0x58140003,  //  000B  LDCONST	R5	K3
          0x7C0C0400,  //  000C  CALL	R3	2
          0x60100015,  //  000D  GETGBL	R4	G21
          0x58140004,  //  000E  LDCONST	R5	K4
          0x7C100200,  //  000F  CALL	R4	1
          0x1C100404,  //  0010  EQ	R4	R2	R4
          0x78120002,  //  0011  JMPF	R4	#0015
          0x94100705,  //  0012  GETIDX	R4	R3	K5
          0xA8040001,  //  0013  EXBLK	1	1
          0x80040800,  //  0014  RET	1	R4
          0x8C100306,  //  0015  GETMET	R4	R1	K6
          0x7C100200,  //  0016  CALL	R4	1
          0xA8040001,  //  0017  EXBLK	1	1
          0x7002000F,  //  0018  JMP		#0029
          0xAC080001,  //  0019  CATCH	R2	0	1
          0x7002000C,  //  001A  JMP		#0028
          0x4C0C0000,  //  001B  LDNIL	R3
          0x200C0203,  //  001C  NE	R3	R1	R3
          0x780E0001,  //  001D  JMPF	R3	#0020
          0x8C0C0306,  //  001E  GETMET	R3	R1	K6
          0x7C0C0200,  //  001F  CALL	R3	1
          0x600C0001,  //  0020  GETGBL	R3	G1
          0x60100018,  //  0021  GETGBL	R4	G24
          0x58140007,  //  0022  LDCONST	R5	K7
          0x5C180000,  //  0023  MOVE	R6	R0
          0x5C1C0400,  //  0024  MOVE	R7	R2
          0x7C100600,  //  0025  CALL	R4	3
          0x7C0C0200,  //  0026  CALL	R3	1
          0x70020000,  //  0027  JMP		#0029
          0xB0080000,  //  0028  RAISE	2	R0	R0
          0x4C080000,  //  0029  LDNIL	R2
          0x80040400,  //  002A  RET	1	R2
        })
      ),
      be_nested_proto(
        5,                          /* nstack */
        1,                          /* argc */
        0,                          /* varg */
        0,                          /* has upvals */
        NULL,                       /* no upvals */
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 2]) {     /* constants */
        /* K0   */  be_nested_str(path),
        /* K1   */  be_nested_str(remove),
        }),
        &be_const_str_try_remove_file,
        &be_const_str_solidified,
        ( &(const binstruction[15]) {  /* code */
          0xA4060000,  //  0000  IMPORT	R1	K0
          0xA8020006,  //  0001  EXBLK	0	#0009
          0x8C080301,  //  0002  GETMET	R2	R1	K1
          0x5C100000,  //  0003  MOVE	R4	R0
          0x7C080400,  //  0004  CALL	R2	2
          0xA8040001,  //  0005  EXBLK	1	1
          0x80040400,  //  0006  RET	1	R2
          0xA8040001,  //  0007  EXBLK	1	1
          0x70020003,  //  0008  JMP		#000D
          0xAC080000,  //  0009  CATCH	R2	0	0
          0x70020000,  //  000A  JMP		#000C
          0x70020000,  //  000B  JMP		#000D
          0xB0080000,  //  000C  RAISE	2	R0	R0
          0x50080000,  //  000D  LDBOOL	R2	0	0
          0x80040400,  //  000E  RET	1	R2
        })
      ),
      be_nested_proto(
        9,                          /* nstack */
        1,                          /* argc */
        0,                          /* varg */
        0,                          /* has upvals */
        NULL,                       /* no upvals */
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 2]) {     /* constants */
        /* K0   */  be_nested_str(file),
        /* K1   */  be_nested_str(BRY_X3A_X20failed_X20to_X20load_X20_X27_X25s_X27_X20_X28_X25s_X20_X2D_X20_X25s_X29),
        }),
        &be_const_str_try_compile,
        &be_const_str_solidified,
        ( &(const binstruction[23]) {  /* code */
          0xA8020007,  //  0000  EXBLK	0	#0009
          0x6004000D,  //  0001  GETGBL	R1	G13
          0x5C080000,  //  0002  MOVE	R2	R0
          0x580C0000,  //  0003  LDCONST	R3	K0
          0x7C040400,  //  0004  CALL	R1	2
          0xA8040001,  //  0005  EXBLK	1	1
          0x80040200,  //  0006  RET	1	R1
          0xA8040001,  //  0007  EXBLK	1	1
          0x7002000B,  //  0008  JMP		#0015
          0xAC040002,  //  0009  CATCH	R1	0	2
          0x70020008,  //  000A  JMP		#0014
          0x600C0001,  //  000B  GETGBL	R3	G1
          0x60100018,  //  000C  GETGBL	R4	G24
          0x58140001,  //  000D  LDCONST	R5	K1
          0x5C180000,  //  000E  MOVE	R6	R0
          0x5C1C0200,  //  000F  MOVE	R7	R1
          0x5C200400,  //  0010  MOVE	R8	R2
          0x7C100800,  //  0011  CALL	R4	4
          0x7C0C0200,  //  0012  CALL	R3	1
          0x70020000,  //  0013  JMP		#0015
          0xB0080000,  //  0014  RAISE	2	R0	R0
          0x4C040000,  //  0015  LDNIL	R1
          0x80040200,  //  0016  RET	1	R1
        })
      ),
      be_nested_proto(
        8,                          /* nstack */
        1,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 4]) {     /* constants */
        /* K0   */  be_nested_str(BRY_X3A_X20failed_X20to_X20run_X20compiled_X20code_X20_X28_X25s_X20_X2D_X20_X25s_X29),
        /* K1   */  be_nested_str(_debug_present),
        /* K2   */  be_nested_str(debug),
        /* K3   */  be_nested_str(traceback),
        }),
        &be_const_str_try_run_compiled,
        &be_const_str_solidified,
        ( &(const binstruction[30]) {  /* code */
          0x4C040000,  //  0000  LDNIL	R1
          0x20040001,  //  0001  NE	R1	R0	R1
          0x78060018,  //  0002  JMPF	R1	#001C
          0xA8020006,  //  0003  EXBLK	0	#000B
          0x5C040000,  //  0004  MOVE	R1	R0
          0x7C040000,  //  0005  CALL	R1	0
          0x50040200,  //  0006  LDBOOL	R1	1	0
          0xA8040001,  //  0007  EXBLK	1	1
          0x80040200,  //  0008  RET	1	R1
          0xA8040001,  //  0009  EXBLK	1	1
          0x70020010,  //  000A  JMP		#001C
          0xAC040002,  //  000B  CATCH	R1	0	2
          0x7002000D,  //  000C  JMP		#001B
          0x600C0001,  //  000D  GETGBL	R3	G1
          0x60100018,  //  000E  GETGBL	R4	G24
          0x58140000,  //  000F  LDCONST	R5	K0
          0x5C180200,  //  0010  MOVE	R6	R1
          0x5C1C0400,  //  0011  MOVE	R7	R2
          0x7C100600,  //  0012  CALL	R4	3
          0x7C0C0200,  //  0013  CALL	R3	1
          0x680C0000,  //  0014  GETUPV	R3	U0
          0x880C0701,  //  0015  GETMBR	R3	R3	K1
          0x780E0002,  //  0016  JMPF	R3	#001A
          0xA40E0400,  //  0017  IMPORT	R3	K2
          0x8C100703,  //  0018  GETMET	R4	R3	K3
          0x7C100200,  //  0019  CALL	R4	1
          0x70020000,  //  001A  JMP		#001C
          0xB0080000,  //  001B  RAISE	2	R0	R0
          0x50040000,  //  001C  LDBOOL	R1	0	0
          0x80040200,  //  001D  RET	1	R1
        })
      ),
    }),
    1,                          /* has constants */
    &be_ktab_class_Tasmota,     /* shared constants */
    &be_const_str_load,
    &be_const_str_solidified,
    ( &(const binstruction[164]) {  /* code */
      0x84080000,  //  0000  CLOSURE	R2	P0
      0x840C0001,  //  0001  CLOSURE	R3	P1
      0x84100002,  //  0002  CLOSURE	R4	P2
      0x84140003,  //  0003  CLOSURE	R5	P3
      0x84180004,  //  0004  CLOSURE	R6	P4
      0x841C0005,  //  0005  CLOSURE	R7	P5
      0xA4222200,  //  0006  IMPORT	R8	K17
      0xA4270400,  //  0007  IMPORT	R9	K130
      0x6028000C,  //  0008  GETGBL	R10	G12
      0x5C2C0200,  //  0009  MOVE	R11	R1
      0x7C280200,  //  000A  CALL	R10	1
      0x1C281506,  //  000B  EQ	R10	R10	K6
      0x782A0002,  //  000C  JMPF	R10	#0010
      0x50280000,  //  000D  LDBOOL	R10	0	0
      0xA0000000,  //  000E  CLOSE	R0
      0x80041400,  //  000F  RET	1	R10
      0x8C281183,  //  0010  GETMET	R10	R8	K131
      0x5C300200,  //  0011  MOVE	R12	R1
      0x5834004F,  //  0012  LDCONST	R13	K79
      0x7C280600,  //  0013  CALL	R10	3
      0x742A0000,  //  0014  JMPT	R10	#0016
      0x00069E01,  //  0015  ADD	R1	K79	R1
      0x8C28111E,  //  0016  GETMET	R10	R8	K30
      0x5C300200,  //  0017  MOVE	R12	R1
      0x58340084,  //  0018  LDCONST	R13	K132
      0x7C280600,  //  0019  CALL	R10	3
      0x242C1506,  //  001A  GT	R11	R10	K6
      0x782E0003,  //  001B  JMPF	R11	#0020
      0x0430150A,  //  001C  SUB	R12	R10	K10
      0x40320C0C,  //  001D  CONNECT	R12	K6	R12
      0x9430020C,  //  001E  GETIDX	R12	R1	R12
      0x70020000,  //  001F  JMP		#0021
      0x5C300200,  //  0020  MOVE	R12	R1
      0x782E0003,  //  0021  JMPF	R11	#0026
      0x0034150A,  //  0022  ADD	R13	R10	K10
      0x40341B81,  //  0023  CONNECT	R13	R13	K129
      0x9434020D,  //  0024  GETIDX	R13	R1	R13
      0x70020000,  //  0025  JMP		#0027
      0x5C340200,  //  0026  MOVE	R13	R1
      0x8C38111E,  //  0027  GETMET	R14	R8	K30
      0x5C401A00,  //  0028  MOVE	R16	R13
      0x58440085,  //  0029  LDCONST	R17	K133
      0x7C380600,  //  002A  CALL	R14	3
      0x14381D06,  //  002B  LT	R14	R14	K6
      0x783A0001,  //  002C  JMPF	R14	#002F
      0x00040386,  //  002D  ADD	R1	R1	K134
      0x00341B86,  //  002E  ADD	R13	R13	K134
      0x8C381187,  //  002F  GETMET	R14	R8	K135
      0x5C401A00,  //  0030  MOVE	R16	R13
      0x58440086,  //  0031  LDCONST	R17	K134
      0x7C380600,  //  0032  CALL	R14	3
      0x8C3C1187,  //  0033  GETMET	R15	R8	K135
      0x5C441A00,  //  0034  MOVE	R17	R13
      0x58480088,  //  0035  LDCONST	R18	K136
      0x7C3C0600,  //  0036  CALL	R15	3
      0x783E0001,  //  0037  JMPF	R15	#003A
      0x5C400200,  //  0038  MOVE	R16	R1
      0x70020000,  //  0039  JMP		#003B
      0x00400389,  //  003A  ADD	R16	R1	K137
      0x5C441C00,  //  003B  MOVE	R17	R14
      0x74460007,  //  003C  JMPT	R17	#0045
      0x5C441E00,  //  003D  MOVE	R17	R15
      0x74460005,  //  003E  JMPT	R17	#0045
      0x60440001,  //  003F  GETGBL	R17	G1
      0x5848008A,  //  0040  LDCONST	R18	K138
      0x7C440200,  //  0041  CALL	R17	1
      0x50440000,  //  0042  LDBOOL	R17	0	0
      0xA0000000,  //  0043  CLOSE	R0
      0x80042200,  //  0044  RET	1	R17
      0x50440000,  //  0045  LDBOOL	R17	0	0
      0x783E0008,  //  0046  JMPF	R15	#0050
      0x8C48138B,  //  0047  GETMET	R18	R9	K139
      0x5C502000,  //  0048  MOVE	R20	R16
      0x7C480400,  //  0049  CALL	R18	2
      0x744A0002,  //  004A  JMPT	R18	#004E
      0x50480000,  //  004B  LDBOOL	R18	0	0
      0xA0000000,  //  004C  CLOSE	R0
      0x80042400,  //  004D  RET	1	R18
      0x50440200,  //  004E  LDBOOL	R17	1	0
      0x70020014,  //  004F  JMP		#0065
      0x8C48138B,  //  0050  GETMET	R18	R9	K139
      0x5C500200,  //  0051  MOVE	R20	R1
      0x7C480400,  //  0052  CALL	R18	2
      0x784A0007,  //  0053  JMPF	R18	#005C
      0x8C48138B,  //  0054  GETMET	R18	R9	K139
      0x5C502000,  //  0055  MOVE	R20	R16
      0x7C480400,  //  0056  CALL	R18	2
      0x784A0002,  //  0057  JMPF	R18	#005B
      0x5C480A00,  //  0058  MOVE	R18	R5
      0x5C4C2000,  //  0059  MOVE	R19	R16
      0x7C480200,  //  005A  CALL	R18	1
      0x70020008,  //  005B  JMP		#0065
      0x8C48138B,  //  005C  GETMET	R18	R9	K139
      0x5C502000,  //  005D  MOVE	R20	R16
      0x7C480400,  //  005E  CALL	R18	2
      0x784A0001,  //  005F  JMPF	R18	#0062
      0x50440200,  //  0060  LDBOOL	R17	1	0
      0x70020002,  //  0061  JMP		#0065
      0x50480000,  //  0062  LDBOOL	R18	0	0
      0xA0000000,  //  0063  CLOSE	R0
      0x80042400,  //  0064  RET	1	R18
      0x782E0005,  //  0065  JMPF	R11	#006C
      0x00481984,  //  0066  ADD	R18	R12	K132
      0x90028A12,  //  0067  SETMBR	R0	K69	R18
      0x5C480400,  //  0068  MOVE	R18	R2
      0x884C0145,  //  0069  GETMBR	R19	R0	K69
      0x7C480200,  //  006A  CALL	R18	1
      0x70020000,  //  006B  JMP		#006D
      0x90028B46,  //  006C  SETMBR	R0	K69	K70
      0x4C480000,  //  006D  LDNIL	R18
      0x78460025,  //  006E  JMPF	R17	#0095
      0x5C4C0800,  //  006F  MOVE	R19	R4
      0x5C502000,  //  0070  MOVE	R20	R16
      0x7C4C0200,  //  0071  CALL	R19	1
      0x50500200,  //  0072  LDBOOL	R20	1	0
      0x4C540000,  //  0073  LDNIL	R21
      0x1C542615,  //  0074  EQ	R21	R19	R21
      0x78560007,  //  0075  JMPF	R21	#007E
      0x60540001,  //  0076  GETGBL	R21	G1
      0x60580018,  //  0077  GETGBL	R22	G24
      0x585C008C,  //  0078  LDCONST	R23	K140
      0x5C602000,  //  0079  MOVE	R24	R16
      0x7C580400,  //  007A  CALL	R22	2
      0x7C540200,  //  007B  CALL	R21	1
      0x50500000,  //  007C  LDBOOL	R20	0	0
      0x7002000A,  //  007D  JMP		#0089
      0x54560003,  //  007E  LDINT	R21	4
      0x20542615,  //  007F  NE	R21	R19	R21
      0x78560007,  //  0080  JMPF	R21	#0089
      0x60540001,  //  0081  GETGBL	R21	G1
      0x60580018,  //  0082  GETGBL	R22	G24
      0x585C008D,  //  0083  LDCONST	R23	K141
      0x5C602000,  //  0084  MOVE	R24	R16
      0x5C642600,  //  0085  MOVE	R25	R19
      0x7C580600,  //  0086  CALL	R22	3
      0x7C540200,  //  0087  CALL	R21	1
      0x50500000,  //  0088  LDBOOL	R20	0	0
      0x78520003,  //  0089  JMPF	R20	#008E
      0x5C540C00,  //  008A  MOVE	R21	R6
      0x5C582000,  //  008B  MOVE	R22	R16
      0x7C540200,  //  008C  CALL	R21	1
      0x5C482A00,  //  008D  MOVE	R18	R21
      0x4C540000,  //  008E  LDNIL	R21
      0x1C542415,  //  008F  EQ	R21	R18	R21
      0x78560003,  //  0090  JMPF	R21	#0095
      0x5C540A00,  //  0091  MOVE	R21	R5
      0x5C582000,  //  0092  MOVE	R22	R16
      0x7C540200,  //  0093  CALL	R21	1
      0x50440000,  //  0094  LDBOOL	R17	0	0
      0x5C4C2200,  //  0095  MOVE	R19	R17
      0x744E0003,  //  0096  JMPT	R19	#009B
      0x5C4C0C00,  //  0097  MOVE	R19	R6
      0x5C500200,  //  0098  MOVE	R20	R1
      0x7C4C0200,  //  0099  CALL	R19	1
      0x5C482600,  //  009A  MOVE	R18	R19
      0x5C4C0E00,  //  009B  MOVE	R19	R7
      0x5C502400,  //  009C  MOVE	R20	R18
      0x7C4C0200,  //  009D  CALL	R19	1
      0x782E0002,  //  009E  JMPF	R11	#00A2
      0x5C500600,  //  009F  MOVE	R20	R3
      0x00541984,  //  00A0  ADD	R21	R12	K132
      0x7C500200,  //  00A1  CALL	R20	1
      0xA0000000,  //  00A2  CLOSE	R0
      0x80042600,  //  00A3  RET	1	R19
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: compile
********************************************************************/
be_local_closure(class_Tasmota_compile,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Tasmota,     /* shared constants */
    &be_const_str_compile,
    &be_const_str_solidified,
    ( &(const binstruction[84]) {  /* code */
      0xA40A2200,  //  0000  IMPORT	R2	K17
      0x8C0C0587,  //  0001  GETMET	R3	R2	K135
      0x5C140200,  //  0002  MOVE	R5	R1
      0x58180086,  //  0003  LDCONST	R6	K134
      0x7C0C0600,  //  0004  CALL	R3	3
      0x740E0007,  //  0005  JMPT	R3	#000E
      0x600C0001,  //  0006  GETGBL	R3	G1
      0x60100018,  //  0007  GETGBL	R4	G24
      0x5814008E,  //  0008  LDCONST	R5	K142
      0x5C180200,  //  0009  MOVE	R6	R1
      0x7C100400,  //  000A  CALL	R4	2
      0x7C0C0200,  //  000B  CALL	R3	1
      0x500C0000,  //  000C  LDBOOL	R3	0	0
      0x80040600,  //  000D  RET	1	R3
      0x8C0C051E,  //  000E  GETMET	R3	R2	K30
      0x5C140200,  //  000F  MOVE	R5	R1
      0x58180084,  //  0010  LDCONST	R6	K132
      0x7C0C0600,  //  0011  CALL	R3	3
      0x240C0706,  //  0012  GT	R3	R3	K6
      0x780E0006,  //  0013  JMPF	R3	#001B
      0x600C0001,  //  0014  GETGBL	R3	G1
      0x60100018,  //  0015  GETGBL	R4	G24
      0x5814008F,  //  0016  LDCONST	R5	K143
      0x7C100200,  //  0017  CALL	R4	1
      0x7C0C0200,  //  0018  CALL	R3	1
      0x500C0000,  //  0019  LDBOOL	R3	0	0
      0x80040600,  //  001A  RET	1	R3
      0x4C0C0000,  //  001B  LDNIL	R3
      0xA8020011,  //  001C  EXBLK	0	#002F
      0x6010000D,  //  001D  GETGBL	R4	G13
      0x5C140200,  //  001E  MOVE	R5	R1
      0x58180090,  //  001F  LDCONST	R6	K144
      0x7C100400,  //  0020  CALL	R4	2
      0x5C0C0800,  //  0021  MOVE	R3	R4
      0x4C100000,  //  0022  LDNIL	R4
      0x1C100604,  //  0023  EQ	R4	R3	R4
      0x78120007,  //  0024  JMPF	R4	#002D
      0x60100001,  //  0025  GETGBL	R4	G1
      0x60140018,  //  0026  GETGBL	R5	G24
      0x58180091,  //  0027  LDCONST	R6	K145
      0x7C140200,  //  0028  CALL	R5	1
      0x7C100200,  //  0029  CALL	R4	1
      0x50100000,  //  002A  LDBOOL	R4	0	0
      0xA8040001,  //  002B  EXBLK	1	1
      0x80040800,  //  002C  RET	1	R4
      0xA8040001,  //  002D  EXBLK	1	1
      0x7002000D,  //  002E  JMP		#003D
      0xAC100002,  //  002F  CATCH	R4	0	2
      0x7002000A,  //  0030  JMP		#003C
      0x60180001,  //  0031  GETGBL	R6	G1
      0x601C0018,  //  0032  GETGBL	R7	G24
      0x58200092,  //  0033  LDCONST	R8	K146
      0x5C240200,  //  0034  MOVE	R9	R1
      0x5C280800,  //  0035  MOVE	R10	R4
      0x5C2C0A00,  //  0036  MOVE	R11	R5
      0x7C1C0800,  //  0037  CALL	R7	4
      0x7C180200,  //  0038  CALL	R6	1
      0x50180000,  //  0039  LDBOOL	R6	0	0
      0x80040C00,  //  003A  RET	1	R6
      0x70020000,  //  003B  JMP		#003D
      0xB0080000,  //  003C  RAISE	2	R0	R0
      0x00100389,  //  003D  ADD	R4	R1	K137
      0xA8020005,  //  003E  EXBLK	0	#0045
      0x8C140172,  //  003F  GETMET	R5	R0	K114
      0x5C1C0800,  //  0040  MOVE	R7	R4
      0x5C200600,  //  0041  MOVE	R8	R3
      0x7C140600,  //  0042  CALL	R5	3
      0xA8040001,  //  0043  EXBLK	1	1
      0x7002000C,  //  0044  JMP		#0052
      0xAC140001,  //  0045  CATCH	R5	0	1
      0x70020009,  //  0046  JMP		#0051
      0x60180001,  //  0047  GETGBL	R6	G1
      0x601C0018,  //  0048  GETGBL	R7	G24
      0x58200093,  //  0049  LDCONST	R8	K147
      0x5C240800,  //  004A  MOVE	R9	R4
      0x5C280A00,  //  004B  MOVE	R10	R5
      0x7C1C0600,  //  004C  CALL	R7	3
      0x7C180200,  //  004D  CALL	R6	1
      0x50180000,  //  004E  LDBOOL	R6	0	0
      0x80040C00,  //  004F  RET	1	R6
      0x70020000,  //  0050  JMP		#0052
      0xB0080000,  //  0051  RAISE	2	R0	R0
      0x50140200,  //  0052  LDBOOL	R5	1	0
      0x80040A00,  //  0053  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_light
********************************************************************/
be_local_closure(class_Tasmota_set_light,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Tasmota,     /* shared constants */
    &be_const_str_set_light,
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x600C0001,  //  0000  GETGBL	R3	G1
      0x58100094,  //  0001  LDCONST	R4	K148
      0x7C0C0200,  //  0002  CALL	R3	1
      0xA40EEE00,  //  0003  IMPORT	R3	K119
      0x4C100000,  //  0004  LDNIL	R4
      0x20100404,  //  0005  NE	R4	R2	R4
      0x78120005,  //  0006  JMPF	R4	#000D
      0x8C100795,  //  0007  GETMET	R4	R3	K149
      0x5C180200,  //  0008  MOVE	R6	R1
      0x5C1C0400,  //  0009  MOVE	R7	R2
      0x7C100600,  //  000A  CALL	R4	3
      0x80040800,  //  000B  RET	1	R4
      0x70020003,  //  000C  JMP		#0011
      0x8C100795,  //  000D  GETMET	R4	R3	K149
      0x5C180200,  //  000E  MOVE	R6	R1
      0x7C100400,  //  000F  CALL	R4	2
      0x80040800,  //  0010  RET	1	R4
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: gen_cb
********************************************************************/
be_local_closure(class_Tasmota_gen_cb,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Tasmota,     /* shared constants */
    &be_const_str_gen_cb,
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0xA40B2C00,  //  0000  IMPORT	R2	K150
      0x8C0C0597,  //  0001  GETMET	R3	R2	K151
      0x5C140200,  //  0002  MOVE	R5	R1
      0x7C0C0400,  //  0003  CALL	R3	2
      0x80040600,  //  0004  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: urlfetch_cmd
********************************************************************/
be_local_closure(class_Tasmota_urlfetch_cmd,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    5,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Tasmota,     /* shared constants */
    &be_const_str_urlfetch_cmd,
    &be_const_str_solidified,
    ( &(const binstruction[34]) {  /* code */
      0xA4162200,  //  0000  IMPORT	R5	K17
      0x8C180B1E,  //  0001  GETMET	R6	R5	K30
      0x5C200600,  //  0002  MOVE	R8	R3
      0x58240098,  //  0003  LDCONST	R9	K152
      0x7C180600,  //  0004  CALL	R6	3
      0x20180D06,  //  0005  NE	R6	R6	K6
      0x781A0003,  //  0006  JMPF	R6	#000B
      0x8C180199,  //  0007  GETMET	R6	R0	K153
      0x5820009A,  //  0008  LDCONST	R8	K154
      0x7C180400,  //  0009  CALL	R6	2
      0x80000C00,  //  000A  RET	0
      0xA802000A,  //  000B  EXBLK	0	#0017
      0x8C18019B,  //  000C  GETMET	R6	R0	K155
      0x5C200600,  //  000D  MOVE	R8	R3
      0x7C180400,  //  000E  CALL	R6	2
      0x141C0D06,  //  000F  LT	R7	R6	K6
      0x781E0003,  //  0010  JMPF	R7	#0015
      0x8C1C019C,  //  0011  GETMET	R7	R0	K156
      0x7C1C0200,  //  0012  CALL	R7	1
      0xA8040001,  //  0013  EXBLK	1	1
      0x80000E00,  //  0014  RET	0
      0xA8040001,  //  0015  EXBLK	1	1
      0x70020006,  //  0016  JMP		#001E
      0xAC180002,  //  0017  CATCH	R6	0	2
      0x70020003,  //  0018  JMP		#001D
      0x8C20019C,  //  0019  GETMET	R8	R0	K156
      0x7C200200,  //  001A  CALL	R8	1
      0x80001000,  //  001B  RET	0
      0x70020000,  //  001C  JMP		#001E
      0xB0080000,  //  001D  RAISE	2	R0	R0
      0xB81A5E00,  //  001E  GETNGBL	R6	K47
      0x8C180D9D,  //  001F  GETMET	R6	R6	K157
      0x7C180200,  //  0020  CALL	R6	1
      0x80000000,  //  0021  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: remove_cmd
********************************************************************/
be_local_closure(class_Tasmota_remove_cmd,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Tasmota,     /* shared constants */
    &be_const_str_remove_cmd,
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x8808017D,  //  0000  GETMBR	R2	R0	K125
      0x780A0003,  //  0001  JMPF	R2	#0006
      0x8808017D,  //  0002  GETMBR	R2	R0	K125
      0x8C080535,  //  0003  GETMET	R2	R2	K53
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: remove_timer
********************************************************************/
be_local_closure(class_Tasmota_remove_timer,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Tasmota,     /* shared constants */
    &be_const_str_remove_timer,
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x8808014C,  //  0000  GETMBR	R2	R0	K76
      0x780A000E,  //  0001  JMPF	R2	#0011
      0x580C0006,  //  0002  LDCONST	R3	K6
      0x8C100507,  //  0003  GETMET	R4	R2	K7
      0x7C100200,  //  0004  CALL	R4	1
      0x14100604,  //  0005  LT	R4	R3	R4
      0x78120009,  //  0006  JMPF	R4	#0011
      0x94100403,  //  0007  GETIDX	R4	R2	R3
      0x88100908,  //  0008  GETMBR	R4	R4	K8
      0x1C100801,  //  0009  EQ	R4	R4	R1
      0x78120003,  //  000A  JMPF	R4	#000F
      0x8C100535,  //  000B  GETMET	R4	R2	K53
      0x5C180600,  //  000C  MOVE	R6	R3
      0x7C100400,  //  000D  CALL	R4	2
      0x70020000,  //  000E  JMP		#0010
      0x000C070A,  //  000F  ADD	R3	R3	K10
      0x7001FFF1,  //  0010  JMP		#0003
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Tasmota
********************************************************************/
be_local_class(Tasmota,
    14,
    NULL,
    be_nested_map(56,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(wire_scan, 29), be_const_closure(class_Tasmota_wire_scan_closure) },
        { be_const_key(next_cron, -1), be_const_closure(class_Tasmota_next_cron_closure) },
        { be_const_key(remove_timer, -1), be_const_closure(class_Tasmota_remove_timer_closure) },
        { be_const_key(find_list_i, -1), be_const_closure(class_Tasmota_find_list_i_closure) },
        { be_const_key(time_str, 40), be_const_closure(class_Tasmota_time_str_closure) },
        { be_const_key(remove_cmd, 41), be_const_closure(class_Tasmota_remove_cmd_closure) },
        { be_const_key(wd, 12), be_const_var(12) },
        { be_const_key(urlfetch_cmd, 52), be_const_closure(class_Tasmota_urlfetch_cmd_closure) },
        { be_const_key(remove_driver, -1), be_const_closure(class_Tasmota_remove_driver_closure) },
        { be_const_key(exec_tele, -1), be_const_closure(class_Tasmota_exec_tele_closure) },
        { be_const_key(run_cron, -1), be_const_closure(class_Tasmota_run_cron_closure) },
        { be_const_key(set_light, -1), be_const_closure(class_Tasmota_set_light_closure) },
        { be_const_key(compile, -1), be_const_closure(class_Tasmota_compile_closure) },
        { be_const_key(hs2rgb, 16), be_const_closure(class_Tasmota_hs2rgb_closure) },
        { be_const_key(exec_rules, -1), be_const_closure(class_Tasmota_exec_rules_closure) },
        { be_const_key(add_rule, -1), be_const_closure(class_Tasmota_add_rule_closure) },
        { be_const_key(_ccmd, -1), be_const_var(5) },
        { be_const_key(_rules, -1), be_const_var(1) },
        { be_const_key(int, -1), be_const_static_closure(class_Tasmota_int_closure) },
        { be_const_key(init, 53), be_const_closure(class_Tasmota_init_closure) },
        { be_const_key(exec_cmd, 46), be_const_closure(class_Tasmota_exec_cmd_closure) },
        { be_const_key(add_cron, 7), be_const_closure(class_Tasmota_add_cron_closure) },
        { be_const_key(_defer, -1), be_const_var(3) },
        { be_const_key(run_timers, -1), be_const_closure(class_Tasmota_run_timers_closure) },
        { be_const_key(remove_rule, -1), be_const_closure(class_Tasmota_remove_rule_closure) },
        { be_const_key(_drivers, -1), be_const_var(6) },
        { be_const_key(urlfetch, 51), be_const_closure(class_Tasmota_urlfetch_closure) },
        { be_const_key(_crons, 50), be_const_var(4) },
        { be_const_key(_timers, 14), be_const_var(2) },
        { be_const_key(settings, -1), be_const_var(11) },
        { be_const_key(global, 18), be_const_var(10) },
        { be_const_key(check_not_method, 20), be_const_closure(class_Tasmota_check_not_method_closure) },
        { be_const_key(run_deferred, 11), be_const_closure(class_Tasmota_run_deferred_closure) },
        { be_const_key(add_rule_once, -1), be_const_closure(class_Tasmota_add_rule_once_closure) },
        { be_const_key(remove_cron, -1), be_const_closure(class_Tasmota_remove_cron_closure) },
        { be_const_key(defer, 10), be_const_closure(class_Tasmota_defer_closure) },
        { be_const_key(event, -1), be_const_closure(class_Tasmota_event_closure) },
        { be_const_key(_debug_present, -1), be_const_var(13) },
        { be_const_key(wire2, -1), be_const_var(8) },
        { be_const_key(remove_fast_loop, 36), be_const_closure(class_Tasmota_remove_fast_loop_closure) },
        { be_const_key(get_light, -1), be_const_closure(class_Tasmota_get_light_closure) },
        { be_const_key(find_key_i, -1), be_const_closure(class_Tasmota_find_key_i_closure) },
        { be_const_key(add_fast_loop, -1), be_const_closure(class_Tasmota_add_fast_loop_closure) },
        { be_const_key(cmd, -1), be_const_closure(class_Tasmota_cmd_closure) },
        { be_const_key(_fl, -1), be_const_var(0) },
        { be_const_key(try_rule, 23), be_const_closure(class_Tasmota_try_rule_closure) },
        { be_const_key(gc, -1), be_const_closure(class_Tasmota_gc_closure) },
        { be_const_key(add_cmd, -1), be_const_closure(class_Tasmota_add_cmd_closure) },
        { be_const_key(find_op, -1), be_const_closure(class_Tasmota_find_op_closure) },
        { be_const_key(load, -1), be_const_closure(class_Tasmota_load_closure) },
        { be_const_key(add_driver, 35), be_const_closure(class_Tasmota_add_driver_closure) },
        { be_const_key(cmd_res, -1), be_const_var(9) },
        { be_const_key(set_timer, -1), be_const_closure(class_Tasmota_set_timer_closure) },
        { be_const_key(wire1, -1), be_const_var(7) },
        { be_const_key(fast_loop, 5), be_const_closure(class_Tasmota_fast_loop_closure) },
        { be_const_key(gen_cb, 2), be_const_closure(class_Tasmota_gen_cb_closure) },
    })),
    (bstring*) &be_const_str_Tasmota
);
/********************************************************************/
/* End of solidification */
