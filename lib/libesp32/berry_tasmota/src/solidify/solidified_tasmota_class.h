/* Solidification of tasmota_class.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
extern const bclass be_class_Tasmota;
// compact class 'Tasmota' ktab size: 183, total: 371 (saved 1504 bytes)
static const bvalue be_ktab_class_Tasmota[183] = {
  /* K0   */  be_nested_str(json),
  /* K1   */  be_nested_str(string),
  /* K2   */  be_nested_str(tasmota),
  /* K3   */  be_nested_str(wd),
  /* K4   */  be_const_int(0),
  /* K5   */  be_nested_str(_X2F),
  /* K6   */  be_nested_str(_X23),
  /* K7   */  be_nested_str(),
  /* K8   */  be_nested_str(manifest_X2Ejson),
  /* K9   */  be_nested_str(read),
  /* K10  */  be_nested_str(close),
  /* K11  */  be_nested_str(load),
  /* K12  */  be_nested_str(find),
  /* K13  */  be_nested_str(name),
  /* K14  */  be_nested_str(description),
  /* K15  */  be_nested_str(version),
  /* K16  */  be_nested_str(min_tasmota),
  /* K17  */  be_nested_str(autorun),
  /* K18  */  be_nested_str(endswith),
  /* K19  */  be_nested_str(_X2Etapp),
  /* K20  */  be_nested_str(log),
  /* K21  */  be_nested_str(BRY_X3A_X20error_X20_X25s_X20_X25s_X20when_X20reading_X20_X27manifest_X2Ejson_X27_X20in_X20_X27_X25s_X27),
  /* K22  */  be_nested_str(_ccmd),
  /* K23  */  be_nested_str(find_key_i),
  /* K24  */  be_nested_str(resolvecmnd),
  /* K25  */  be_const_class(be_class_Tasmota),
  /* K26  */  be_nested_str(check_not_method),
  /* K27  */  be_nested_str(is_network_up),
  /* K28  */  be_nested_str(_wnu),
  /* K29  */  be_nested_str(push),
  /* K30  */  be_nested_str(_X2Ebe),
  /* K31  */  be_nested_str(BRY_X3A_X20file_X20_X27_X25s_X27_X20does_X20not_X20have_X20_X27_X2Ebe_X27_X20extension),
  /* K32  */  be_nested_str(BRY_X3A_X20cannot_X20compile_X20file_X20in_X20read_X2Donly_X20archive),
  /* K33  */  be_nested_str(file),
  /* K34  */  be_nested_str(BRY_X3A_X20empty_X20compiled_X20file),
  /* K35  */  be_nested_str(BRY_X3A_X20failed_X20to_X20load_X20_X27_X25s_X27_X20_X28_X25s_X20_X2D_X20_X25s_X29),
  /* K36  */  be_nested_str(c),
  /* K37  */  be_nested_str(save),
  /* K38  */  be_nested_str(BRY_X3A_X20could_X20not_X20save_X20compiled_X20file_X20_X25s_X20_X28_X25s_X29),
  /* K39  */  be_nested_str(_fl),
  /* K40  */  be_nested_str(function),
  /* K41  */  be_nested_str(value_error),
  /* K42  */  be_nested_str(argument_X20must_X20be_X20a_X20function),
  /* K43  */  be_nested_str(global),
  /* K44  */  be_nested_str(fast_loop_enabled),
  /* K45  */  be_const_int(1),
  /* K46  */  be_nested_str(_timers),
  /* K47  */  be_nested_str(size),
  /* K48  */  be_nested_str(id),
  /* K49  */  be_nested_str(remove),
  /* K50  */  be_nested_str(wifi),
  /* K51  */  be_nested_str(up),
  /* K52  */  be_nested_str(eth),
  /* K53  */  be_nested_str(toupper),
  /* K54  */  be_nested_str(tasmota_X2Eset_light_X28_X29_X20is_X20deprecated_X2C_X20use_X20light_X2Eset_X28_X29),
  /* K55  */  be_nested_str(light),
  /* K56  */  be_nested_str(set),
  /* K57  */  be_nested_str(_crons),
  /* K58  */  be_nested_str(ccronexpr),
  /* K59  */  be_nested_str(now),
  /* K60  */  be_nested_str(trig),
  /* K61  */  be_nested_str(next),
  /* K62  */  be_nested_str(time_reached),
  /* K63  */  be_nested_str(f),
  /* K64  */  be_nested_str(cmd_res),
  /* K65  */  be_nested_str(maxlog_level),
  /* K66  */  be_const_int(2),
  /* K67  */  be_nested_str(_cmd),
  /* K68  */  be_nested_str(keys),
  /* K69  */  be_nested_str(_X3F),
  /* K70  */  be_nested_str(stop_iteration),
  /* K71  */  be_nested_str(_rules),
  /* K72  */  be_nested_str(rule),
  /* K73  */  be_nested_str(_drivers),
  /* K74  */  be_nested_str(pop),
  /* K75  */  be_nested_str(_ext),
  /* K76  */  be_nested_str(remove_by_value),
  /* K77  */  be_nested_str(the_X20second_X20argument_X20is_X20not_X20a_X20function),
  /* K78  */  be_nested_str(Trigger),
  /* K79  */  be_nested_str(BRY_X3A_X20ERROR_X2C_X20bad_X20json_X3A_X20),
  /* K80  */  be_const_int(3),
  /* K81  */  be_nested_str(Tele),
  /* K82  */  be_nested_str(try_rule),
  /* K83  */  be_nested_str(cb),
  /* K84  */  be_nested_str(gen_cb),
  /* K85  */  be_nested_str(BRY_X3A_X20Exception_X3E_X20run_network_up_X20_X27_X25s_X27_X20_X2D_X20_X25s),
  /* K86  */  be_nested_str(run_deferred),
  /* K87  */  be_nested_str(instance),
  /* K88  */  be_nested_str(instance_X20and_X20name_X20required),
  /* K89  */  be_nested_str(sortedmap),
  /* K90  */  be_nested_str(contains),
  /* K91  */  be_nested_str(BRY_X3A_X20Extension_X20_X27_X25s_X27_X20already_X20registered),
  /* K92  */  be_nested_str(i2c_enabled),
  /* K93  */  be_nested_str(wire1),
  /* K94  */  be_nested_str(enabled),
  /* K95  */  be_nested_str(detect),
  /* K96  */  be_nested_str(wire2),
  /* K97  */  be_nested_str(split),
  /* K98  */  be_nested_str(index_X2Ehtml),
  /* K99  */  be_nested_str(webclient),
  /* K100 */  be_nested_str(set_follow_redirects),
  /* K101 */  be_nested_str(begin),
  /* K102 */  be_nested_str(GET),
  /* K103 */  be_nested_str(status_X3A_X20),
  /* K104 */  be_nested_str(connection_error),
  /* K105 */  be_nested_str(write_file),
  /* K106 */  be_nested_str(BRY_X3A_X20Fetched_X20),
  /* K107 */  be_nested_str(_defer),
  /* K108 */  be_nested_str(deferred_ready),
  /* K109 */  be_nested_str(ctypes_bytes_dyn),
  /* K110 */  be_nested_str(_global_addr),
  /* K111 */  be_nested_str(_global_def),
  /* K112 */  be_nested_str(introspect),
  /* K113 */  be_nested_str(_settings_ptr),
  /* K114 */  be_nested_str(get),
  /* K115 */  be_nested_str(settings),
  /* K116 */  be_nested_str(toptr),
  /* K117 */  be_nested_str(_settings_def),
  /* K118 */  be_nested_str(_debug_present),
  /* K119 */  be_nested_str(debug),
  /* K120 */  be_nested_str(add_cmd),
  /* K121 */  be_nested_str(UrlFetch),
  /* K122 */  be_nested_str(unload),
  /* K123 */  be_nested_str(remove_driver),
  /* K124 */  be_nested_str(gc),
  /* K125 */  be_nested_str(tasmota_X2Eget_light_X28_X29_X20is_X20deprecated_X2C_X20use_X20light_X2Eget_X28_X29),
  /* K126 */  be_nested_str(path),
  /* K127 */  be_nested_str(startswith),
  /* K128 */  be_nested_str(_X2Etapp_),
  /* K129 */  be_nested_str(_X23autoexec),
  /* K130 */  be_const_int(2147483647),
  /* K131 */  be_nested_str(_X2E),
  /* K132 */  be_nested_str(_X2Ebec),
  /* K133 */  be_nested_str(BRY_X3A_X20file_X20extension_X20is_X20not_X20_X27_X2Ebe_X27_X20nor_X20_X27_X2Ebec_X27),
  /* K134 */  be_nested_str(exists),
  /* K135 */  be_nested_str(BRY_X3A_X20corrupt_X20bytecode_X20_X27_X25s_X27),
  /* K136 */  be_nested_str(BRY_X3A_X20bytecode_X20has_X20wrong_X20version_X20_X27_X25s_X27_X20_X28_X25s_X29),
  /* K137 */  be_nested_str(remove_rule),
  /* K138 */  be_nested_str(Rule_Matcher),
  /* K139 */  be_nested_str(parse),
  /* K140 */  be_nested_str(type_error),
  /* K141 */  be_nested_str(BRY_X3A_X20argument_X20must_X20be_X20a_X20function),
  /* K142 */  be_nested_str(ismethod),
  /* K143 */  be_nested_str(BRY_X3A_X20method_X20not_X20allowed_X2C_X20use_X20a_X20closure_X20like_X20_X27_X2F_X20args_X20_X2D_X3E_X20obj_X2Efunc_X28args_X29_X27),
  /* K144 */  be_nested_str(add_rule),
  /* K145 */  be_nested_str(o),
  /* K146 */  be_nested_str(millis),
  /* K147 */  be_nested_str(collect),
  /* K148 */  be_nested_str(allocated),
  /* K149 */  be_nested_str(_find_op),
  /* K150 */  be_nested_str(http),
  /* K151 */  be_nested_str(resp_cmnd_str),
  /* K152 */  be_nested_str(URL_X20must_X20start_X20with_X20_X27http_X28s_X29_X27),
  /* K153 */  be_nested_str(urlfetch),
  /* K154 */  be_nested_str(resp_cmnd_failed),
  /* K155 */  be_nested_str(resp_cmnd_done),
  /* K156 */  be_nested_str(scale_uint),
  /* K157 */  be_nested_str(time_dump),
  /* K158 */  be_nested_str(_X2504d_X2D_X2502d_X2D_X2502dT_X2502d_X3A_X2502d_X3A_X2502d),
  /* K159 */  be_nested_str(year),
  /* K160 */  be_nested_str(month),
  /* K161 */  be_nested_str(day),
  /* K162 */  be_nested_str(hour),
  /* K163 */  be_nested_str(min),
  /* K164 */  be_nested_str(sec),
  /* K165 */  be_nested_str(match),
  /* K166 */  be_nested_str(trigger),
  /* K167 */  be_nested_str(every_50ms),
  /* K168 */  be_nested_str(run_network_up),
  /* K169 */  be_nested_str(run_timers),
  /* K170 */  be_nested_str(every_250ms),
  /* K171 */  be_nested_str(run_cron),
  /* K172 */  be_nested_str(mqtt_data),
  /* K173 */  be_nested_str(cmd),
  /* K174 */  be_nested_str(exec_cmd),
  /* K175 */  be_nested_str(tele),
  /* K176 */  be_nested_str(exec_tele),
  /* K177 */  be_nested_str(exec_rules),
  /* K178 */  be_nested_str(BRY_X3A_X20Exception_X3E_X20_X27_X25s_X27_X20_X2D_X20_X25s),
  /* K179 */  be_nested_str(traceback),
  /* K180 */  be_nested_str(save_before_restart),
  /* K181 */  be_nested_str(persist),
  /* K182 */  be_nested_str(instance_X20required),
};


extern const bclass be_class_Tasmota;

/********************************************************************
** Solidified function: read_extension_manifest
********************************************************************/
be_local_closure(class_Tasmota_read_extension_manifest,   /* name */
  be_nested_proto(
    15,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Tasmota,     /* shared constants */
    &be_const_str_read_extension_manifest,
    &be_const_str_solidified,
    ( &(const binstruction[100]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0xA802004C,  //  0002  EXBLK	0	#0050
      0xA4120000,  //  0003  IMPORT	R4	K0
      0xA4160200,  //  0004  IMPORT	R5	K1
      0x4C180000,  //  0005  LDNIL	R6
      0x1C180606,  //  0006  EQ	R6	R3	R6
      0x781A0001,  //  0007  JMPF	R6	#000A
      0xB81A0400,  //  0008  GETNGBL	R6	K2
      0x880C0D03,  //  0009  GETMBR	R3	R6	K3
      0x6018000C,  //  000A  GETGBL	R6	G12
      0x5C1C0600,  //  000B  MOVE	R7	R3
      0x7C180200,  //  000C  CALL	R6	1
      0x24180D04,  //  000D  GT	R6	R6	K4
      0x781A0009,  //  000E  JMPF	R6	#0019
      0x5419FFFE,  //  000F  LDINT	R6	-1
      0x94180606,  //  0010  GETIDX	R6	R3	R6
      0x20180D05,  //  0011  NE	R6	R6	K5
      0x781A0005,  //  0012  JMPF	R6	#0019
      0x5419FFFE,  //  0013  LDINT	R6	-1
      0x94180606,  //  0014  GETIDX	R6	R3	R6
      0x20180D06,  //  0015  NE	R6	R6	K6
      0x781A0001,  //  0016  JMPF	R6	#0019
      0x58180006,  //  0017  LDCONST	R6	K6
      0x70020000,  //  0018  JMP		#001A
      0x58180007,  //  0019  LDCONST	R6	K7
      0x601C0011,  //  001A  GETGBL	R7	G17
      0x00200606,  //  001B  ADD	R8	R3	R6
      0x00201108,  //  001C  ADD	R8	R8	K8
      0x7C1C0200,  //  001D  CALL	R7	1
      0x5C080E00,  //  001E  MOVE	R2	R7
      0x8C1C0509,  //  001F  GETMET	R7	R2	K9
      0x7C1C0200,  //  0020  CALL	R7	1
      0x8C20050A,  //  0021  GETMET	R8	R2	K10
      0x7C200200,  //  0022  CALL	R8	1
      0x8C20090B,  //  0023  GETMET	R8	R4	K11
      0x5C280E00,  //  0024  MOVE	R10	R7
      0x7C200400,  //  0025  CALL	R8	2
      0x8C24110C,  //  0026  GETMET	R9	R8	K12
      0x582C000D,  //  0027  LDCONST	R11	K13
      0x7C240400,  //  0028  CALL	R9	2
      0x78260020,  //  0029  JMPF	R9	#004B
      0x60280008,  //  002A  GETGBL	R10	G8
      0x942C110D,  //  002B  GETIDX	R11	R8	K13
      0x7C280200,  //  002C  CALL	R10	1
      0x98221A0A,  //  002D  SETIDX	R8	K13	R10
      0x60280008,  //  002E  GETGBL	R10	G8
      0x8C2C110C,  //  002F  GETMET	R11	R8	K12
      0x5834000E,  //  0030  LDCONST	R13	K14
      0x58380007,  //  0031  LDCONST	R14	K7
      0x7C2C0600,  //  0032  CALL	R11	3
      0x7C280200,  //  0033  CALL	R10	1
      0x98221C0A,  //  0034  SETIDX	R8	K14	R10
      0x60280009,  //  0035  GETGBL	R10	G9
      0x8C2C110C,  //  0036  GETMET	R11	R8	K12
      0x5834000F,  //  0037  LDCONST	R13	K15
      0x58380004,  //  0038  LDCONST	R14	K4
      0x7C2C0600,  //  0039  CALL	R11	3
      0x7C280200,  //  003A  CALL	R10	1
      0x98221E0A,  //  003B  SETIDX	R8	K15	R10
      0x60280009,  //  003C  GETGBL	R10	G9
      0x8C2C110C,  //  003D  GETMET	R11	R8	K12
      0x58340010,  //  003E  LDCONST	R13	K16
      0x58380004,  //  003F  LDCONST	R14	K4
      0x7C2C0600,  //  0040  CALL	R11	3
      0x7C280200,  //  0041  CALL	R10	1
      0x9822200A,  //  0042  SETIDX	R8	K16	R10
      0x8C280B12,  //  0043  GETMET	R10	R5	K18
      0x5C300600,  //  0044  MOVE	R12	R3
      0x58340013,  //  0045  LDCONST	R13	K19
      0x7C280600,  //  0046  CALL	R10	3
      0x9822220A,  //  0047  SETIDX	R8	K17	R10
      0xA8040001,  //  0048  EXBLK	1	1
      0x80041000,  //  0049  RET	1	R8
      0x70020002,  //  004A  JMP		#004E
      0x4C280000,  //  004B  LDNIL	R10
      0xA8040001,  //  004C  EXBLK	1	1
      0x80041400,  //  004D  RET	1	R10
      0xA8040001,  //  004E  EXBLK	1	1
      0x70020012,  //  004F  JMP		#0063
      0xAC100002,  //  0050  CATCH	R4	0	2
      0x7002000F,  //  0051  JMP		#0062
      0xB81A2800,  //  0052  GETNGBL	R6	K20
      0x601C0018,  //  0053  GETGBL	R7	G24
      0x58200015,  //  0054  LDCONST	R8	K21
      0x5C240800,  //  0055  MOVE	R9	R4
      0x5C280A00,  //  0056  MOVE	R10	R5
      0x5C2C0600,  //  0057  MOVE	R11	R3
      0x7C1C0800,  //  0058  CALL	R7	4
      0x7C180200,  //  0059  CALL	R6	1
      0x4C180000,  //  005A  LDNIL	R6
      0x20180406,  //  005B  NE	R6	R2	R6
      0x781A0001,  //  005C  JMPF	R6	#005F
      0x8C18050A,  //  005D  GETMET	R6	R2	K10
      0x7C180200,  //  005E  CALL	R6	1
      0x4C180000,  //  005F  LDNIL	R6
      0x80040C00,  //  0060  RET	1	R6
      0x70020000,  //  0061  JMP		#0063
      0xB0080000,  //  0062  RAISE	2	R0	R0
      0x80000000,  //  0063  RET	0
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
      0x88100116,  //  0000  GETMBR	R4	R0	K22
      0x78120016,  //  0001  JMPF	R4	#0019
      0x8C100117,  //  0002  GETMET	R4	R0	K23
      0x88180116,  //  0003  GETMBR	R6	R0	K22
      0x5C1C0200,  //  0004  MOVE	R7	R1
      0x7C100600,  //  0005  CALL	R4	3
      0x4C140000,  //  0006  LDNIL	R5
      0x20140805,  //  0007  NE	R5	R4	R5
      0x7816000F,  //  0008  JMPF	R5	#0019
      0xA4160000,  //  0009  IMPORT	R5	K0
      0x8C180B0B,  //  000A  GETMET	R6	R5	K11
      0x5C200600,  //  000B  MOVE	R8	R3
      0x7C180400,  //  000C  CALL	R6	2
      0x8C1C0118,  //  000D  GETMET	R7	R0	K24
      0x5C240800,  //  000E  MOVE	R9	R4
      0x7C1C0400,  //  000F  CALL	R7	2
      0x881C0116,  //  0010  GETMBR	R7	R0	K22
      0x941C0E04,  //  0011  GETIDX	R7	R7	R4
      0x5C200800,  //  0012  MOVE	R8	R4
      0x5C240400,  //  0013  MOVE	R9	R2
      0x5C280600,  //  0014  MOVE	R10	R3
      0x5C2C0C00,  //  0015  MOVE	R11	R6
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
      0x580C0019,  //  0000  LDCONST	R3	K25
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
** Solidified function: when_network_up
********************************************************************/
be_local_closure(class_Tasmota_when_network_up,   /* name */
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
    &be_const_str_when_network_up,
    &be_const_str_solidified,
    ( &(const binstruction[23]) {  /* code */
      0x8C08011A,  //  0000  GETMET	R2	R0	K26
      0x5C100200,  //  0001  MOVE	R4	R1
      0x7C080400,  //  0002  CALL	R2	2
      0x8C08011B,  //  0003  GETMET	R2	R0	K27
      0x7C080200,  //  0004  CALL	R2	1
      0x780A0002,  //  0005  JMPF	R2	#0009
      0x5C080200,  //  0006  MOVE	R2	R1
      0x7C080000,  //  0007  CALL	R2	0
      0x7002000C,  //  0008  JMP		#0016
      0x8808011C,  //  0009  GETMBR	R2	R0	K28
      0x4C0C0000,  //  000A  LDNIL	R3
      0x1C080403,  //  000B  EQ	R2	R2	R3
      0x780A0004,  //  000C  JMPF	R2	#0012
      0x60080012,  //  000D  GETGBL	R2	G18
      0x7C080000,  //  000E  CALL	R2	0
      0x400C0401,  //  000F  CONNECT	R3	R2	R1
      0x90023802,  //  0010  SETMBR	R0	K28	R2
      0x70020003,  //  0011  JMP		#0016
      0x8808011C,  //  0012  GETMBR	R2	R0	K28
      0x8C08051D,  //  0013  GETMET	R2	R2	K29
      0x5C100200,  //  0014  MOVE	R4	R1
      0x7C080400,  //  0015  CALL	R2	2
      0x80000000,  //  0016  RET	0
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
    ( &(const binstruction[85]) {  /* code */
      0xA40A0200,  //  0000  IMPORT	R2	K1
      0x8C0C0512,  //  0001  GETMET	R3	R2	K18
      0x5C140200,  //  0002  MOVE	R5	R1
      0x5818001E,  //  0003  LDCONST	R6	K30
      0x7C0C0600,  //  0004  CALL	R3	3
      0x740E0007,  //  0005  JMPT	R3	#000E
      0x600C0001,  //  0006  GETGBL	R3	G1
      0x60100018,  //  0007  GETGBL	R4	G24
      0x5814001F,  //  0008  LDCONST	R5	K31
      0x5C180200,  //  0009  MOVE	R6	R1
      0x7C100400,  //  000A  CALL	R4	2
      0x7C0C0200,  //  000B  CALL	R3	1
      0x500C0000,  //  000C  LDBOOL	R3	0	0
      0x80040600,  //  000D  RET	1	R3
      0x8C0C050C,  //  000E  GETMET	R3	R2	K12
      0x5C140200,  //  000F  MOVE	R5	R1
      0x58180006,  //  0010  LDCONST	R6	K6
      0x7C0C0600,  //  0011  CALL	R3	3
      0x240C0704,  //  0012  GT	R3	R3	K4
      0x780E0006,  //  0013  JMPF	R3	#001B
      0x600C0001,  //  0014  GETGBL	R3	G1
      0x60100018,  //  0015  GETGBL	R4	G24
      0x58140020,  //  0016  LDCONST	R5	K32
      0x7C100200,  //  0017  CALL	R4	1
      0x7C0C0200,  //  0018  CALL	R3	1
      0x500C0000,  //  0019  LDBOOL	R3	0	0
      0x80040600,  //  001A  RET	1	R3
      0x4C0C0000,  //  001B  LDNIL	R3
      0xA8020012,  //  001C  EXBLK	0	#0030
      0x6010000D,  //  001D  GETGBL	R4	G13
      0x5C140200,  //  001E  MOVE	R5	R1
      0x58180021,  //  001F  LDCONST	R6	K33
      0x501C0200,  //  0020  LDBOOL	R7	1	0
      0x7C100600,  //  0021  CALL	R4	3
      0x5C0C0800,  //  0022  MOVE	R3	R4
      0x4C100000,  //  0023  LDNIL	R4
      0x1C100604,  //  0024  EQ	R4	R3	R4
      0x78120007,  //  0025  JMPF	R4	#002E
      0x60100001,  //  0026  GETGBL	R4	G1
      0x60140018,  //  0027  GETGBL	R5	G24
      0x58180022,  //  0028  LDCONST	R6	K34
      0x7C140200,  //  0029  CALL	R5	1
      0x7C100200,  //  002A  CALL	R4	1
      0x50100000,  //  002B  LDBOOL	R4	0	0
      0xA8040001,  //  002C  EXBLK	1	1
      0x80040800,  //  002D  RET	1	R4
      0xA8040001,  //  002E  EXBLK	1	1
      0x7002000D,  //  002F  JMP		#003E
      0xAC100002,  //  0030  CATCH	R4	0	2
      0x7002000A,  //  0031  JMP		#003D
      0x60180001,  //  0032  GETGBL	R6	G1
      0x601C0018,  //  0033  GETGBL	R7	G24
      0x58200023,  //  0034  LDCONST	R8	K35
      0x5C240200,  //  0035  MOVE	R9	R1
      0x5C280800,  //  0036  MOVE	R10	R4
      0x5C2C0A00,  //  0037  MOVE	R11	R5
      0x7C1C0800,  //  0038  CALL	R7	4
      0x7C180200,  //  0039  CALL	R6	1
      0x50180000,  //  003A  LDBOOL	R6	0	0
      0x80040C00,  //  003B  RET	1	R6
      0x70020000,  //  003C  JMP		#003E
      0xB0080000,  //  003D  RAISE	2	R0	R0
      0x00100324,  //  003E  ADD	R4	R1	K36
      0xA8020005,  //  003F  EXBLK	0	#0046
      0x8C140125,  //  0040  GETMET	R5	R0	K37
      0x5C1C0800,  //  0041  MOVE	R7	R4
      0x5C200600,  //  0042  MOVE	R8	R3
      0x7C140600,  //  0043  CALL	R5	3
      0xA8040001,  //  0044  EXBLK	1	1
      0x7002000C,  //  0045  JMP		#0053
      0xAC140001,  //  0046  CATCH	R5	0	1
      0x70020009,  //  0047  JMP		#0052
      0x60180001,  //  0048  GETGBL	R6	G1
      0x601C0018,  //  0049  GETGBL	R7	G24
      0x58200026,  //  004A  LDCONST	R8	K38
      0x5C240800,  //  004B  MOVE	R9	R4
      0x5C280A00,  //  004C  MOVE	R10	R5
      0x7C1C0600,  //  004D  CALL	R7	3
      0x7C180200,  //  004E  CALL	R6	1
      0x50180000,  //  004F  LDBOOL	R6	0	0
      0x80040C00,  //  0050  RET	1	R6
      0x70020000,  //  0051  JMP		#0053
      0xB0080000,  //  0052  RAISE	2	R0	R0
      0x50140200,  //  0053  LDBOOL	R5	1	0
      0x80040A00,  //  0054  RET	1	R5
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
      0x8C08011A,  //  0000  GETMET	R2	R0	K26
      0x5C100200,  //  0001  MOVE	R4	R1
      0x7C080400,  //  0002  CALL	R2	2
      0x88080127,  //  0003  GETMBR	R2	R0	K39
      0x4C0C0000,  //  0004  LDNIL	R3
      0x1C080403,  //  0005  EQ	R2	R2	R3
      0x780A0002,  //  0006  JMPF	R2	#000A
      0x60080012,  //  0007  GETGBL	R2	G18
      0x7C080000,  //  0008  CALL	R2	0
      0x90024E02,  //  0009  SETMBR	R0	K39	R2
      0x60080004,  //  000A  GETGBL	R2	G4
      0x5C0C0200,  //  000B  MOVE	R3	R1
      0x7C080200,  //  000C  CALL	R2	1
      0x20080528,  //  000D  NE	R2	R2	K40
      0x780A0000,  //  000E  JMPF	R2	#0010
      0xB006532A,  //  000F  RAISE	1	K41	K42
      0x8808012B,  //  0010  GETMBR	R2	R0	K43
      0x900A592D,  //  0011  SETMBR	R2	K44	K45
      0x88080127,  //  0012  GETMBR	R2	R0	K39
      0x8C08051D,  //  0013  GETMET	R2	R2	K29
      0x5C100200,  //  0014  MOVE	R4	R1
      0x7C080400,  //  0015  CALL	R2	2
      0x80000000,  //  0016  RET	0
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
      0x8808012E,  //  0000  GETMBR	R2	R0	K46
      0x780A000E,  //  0001  JMPF	R2	#0011
      0x580C0004,  //  0002  LDCONST	R3	K4
      0x8C10052F,  //  0003  GETMET	R4	R2	K47
      0x7C100200,  //  0004  CALL	R4	1
      0x14100604,  //  0005  LT	R4	R3	R4
      0x78120009,  //  0006  JMPF	R4	#0011
      0x94100403,  //  0007  GETIDX	R4	R2	R3
      0x88100930,  //  0008  GETMBR	R4	R4	K48
      0x1C100801,  //  0009  EQ	R4	R4	R1
      0x78120003,  //  000A  JMPF	R4	#000F
      0x8C100531,  //  000B  GETMET	R4	R2	K49
      0x5C180600,  //  000C  MOVE	R6	R3
      0x7C100400,  //  000D  CALL	R4	2
      0x70020000,  //  000E  JMP		#0010
      0x000C072D,  //  000F  ADD	R3	R3	K45
      0x7001FFF1,  //  0010  JMP		#0003
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_network_up
********************************************************************/
be_local_closure(class_Tasmota_is_network_up,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Tasmota,     /* shared constants */
    &be_const_str_is_network_up,
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0xB8060400,  //  0000  GETNGBL	R1	K2
      0x8C040332,  //  0001  GETMET	R1	R1	K50
      0x7C040200,  //  0002  CALL	R1	1
      0x94040333,  //  0003  GETIDX	R1	R1	K51
      0x74060005,  //  0004  JMPT	R1	#000B
      0xB8060400,  //  0005  GETNGBL	R1	K2
      0x8C040334,  //  0006  GETMET	R1	R1	K52
      0x7C040200,  //  0007  CALL	R1	1
      0x94040333,  //  0008  GETIDX	R1	R1	K51
      0x74060000,  //  0009  JMPT	R1	#000B
      0x50040001,  //  000A  LDBOOL	R1	0	1
      0x50040200,  //  000B  LDBOOL	R1	1	0
      0x80040200,  //  000C  RET	1	R1
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
      0xA40E0200,  //  0000  IMPORT	R3	K1
      0x58100004,  //  0001  LDCONST	R4	K4
      0x8C140735,  //  0002  GETMET	R5	R3	K53
      0x5C1C0400,  //  0003  MOVE	R7	R2
      0x7C140400,  //  0004  CALL	R5	2
      0x6018000C,  //  0005  GETGBL	R6	G12
      0x5C1C0200,  //  0006  MOVE	R7	R1
      0x7C180200,  //  0007  CALL	R6	1
      0x14180806,  //  0008  LT	R6	R4	R6
      0x781A0007,  //  0009  JMPF	R6	#0012
      0x8C180735,  //  000A  GETMET	R6	R3	K53
      0x94200204,  //  000B  GETIDX	R8	R1	R4
      0x7C180400,  //  000C  CALL	R6	2
      0x1C180C05,  //  000D  EQ	R6	R6	R5
      0x781A0000,  //  000E  JMPF	R6	#0010
      0x80040800,  //  000F  RET	1	R4
      0x0010092D,  //  0010  ADD	R4	R4	K45
      0x7001FFF2,  //  0011  JMP		#0005
      0x4C180000,  //  0012  LDNIL	R6
      0x80040C00,  //  0013  RET	1	R6
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
      0x58100036,  //  0001  LDCONST	R4	K54
      0x7C0C0200,  //  0002  CALL	R3	1
      0xA40E6E00,  //  0003  IMPORT	R3	K55
      0x4C100000,  //  0004  LDNIL	R4
      0x20100404,  //  0005  NE	R4	R2	R4
      0x78120005,  //  0006  JMPF	R4	#000D
      0x8C100738,  //  0007  GETMET	R4	R3	K56
      0x5C180200,  //  0008  MOVE	R6	R1
      0x5C1C0400,  //  0009  MOVE	R7	R2
      0x7C100600,  //  000A  CALL	R4	3
      0x80040800,  //  000B  RET	1	R4
      0x70020003,  //  000C  JMP		#0011
      0x8C100738,  //  000D  GETMET	R4	R3	K56
      0x5C180200,  //  000E  MOVE	R6	R1
      0x7C100400,  //  000F  CALL	R4	2
      0x80040800,  //  0010  RET	1	R4
      0x80000000,  //  0011  RET	0
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
      0x88040139,  //  0000  GETMBR	R1	R0	K57
      0x7806001E,  //  0001  JMPF	R1	#0021
      0x58040004,  //  0002  LDCONST	R1	K4
      0xB80A7400,  //  0003  GETNGBL	R2	K58
      0x8C08053B,  //  0004  GETMET	R2	R2	K59
      0x7C080200,  //  0005  CALL	R2	1
      0x880C0139,  //  0006  GETMBR	R3	R0	K57
      0x8C0C072F,  //  0007  GETMET	R3	R3	K47
      0x7C0C0200,  //  0008  CALL	R3	1
      0x140C0203,  //  0009  LT	R3	R1	R3
      0x780E0015,  //  000A  JMPF	R3	#0021
      0x880C0139,  //  000B  GETMBR	R3	R0	K57
      0x940C0601,  //  000C  GETIDX	R3	R3	R1
      0x8810073C,  //  000D  GETMBR	R4	R3	K60
      0x1C100904,  //  000E  EQ	R4	R4	K4
      0x78120003,  //  000F  JMPF	R4	#0014
      0x8C10073D,  //  0010  GETMET	R4	R3	K61
      0x7C100200,  //  0011  CALL	R4	1
      0x900E7804,  //  0012  SETMBR	R3	K60	R4
      0x7002000A,  //  0013  JMP		#001F
      0x8C10073E,  //  0014  GETMET	R4	R3	K62
      0x7C100200,  //  0015  CALL	R4	1
      0x78120007,  //  0016  JMPF	R4	#001F
      0x8810073F,  //  0017  GETMBR	R4	R3	K63
      0x8C14073D,  //  0018  GETMET	R5	R3	K61
      0x7C140200,  //  0019  CALL	R5	1
      0x900E7805,  //  001A  SETMBR	R3	K60	R5
      0x5C180800,  //  001B  MOVE	R6	R4
      0x5C1C0400,  //  001C  MOVE	R7	R2
      0x5C200A00,  //  001D  MOVE	R8	R5
      0x7C180400,  //  001E  CALL	R6	2
      0x0004032D,  //  001F  ADD	R1	R1	K45
      0x7001FFE4,  //  0020  JMP		#0006
      0x80000000,  //  0021  RET	0
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
      0x88080139,  //  0000  GETMBR	R2	R0	K57
      0x780A000E,  //  0001  JMPF	R2	#0011
      0x580C0004,  //  0002  LDCONST	R3	K4
      0x8C10052F,  //  0003  GETMET	R4	R2	K47
      0x7C100200,  //  0004  CALL	R4	1
      0x14100604,  //  0005  LT	R4	R3	R4
      0x78120009,  //  0006  JMPF	R4	#0011
      0x94100403,  //  0007  GETIDX	R4	R2	R3
      0x88100930,  //  0008  GETMBR	R4	R4	K48
      0x1C100801,  //  0009  EQ	R4	R4	R1
      0x78120003,  //  000A  JMPF	R4	#000F
      0x8C100531,  //  000B  GETMET	R4	R2	K49
      0x5C180600,  //  000C  MOVE	R6	R3
      0x7C100400,  //  000D  CALL	R4	2
      0x70020000,  //  000E  JMP		#0010
      0x000C072D,  //  000F  ADD	R3	R3	K45
      0x7001FFF1,  //  0010  JMP		#0003
      0x80000000,  //  0011  RET	0
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
      0x880C0140,  //  0000  GETMBR	R3	R0	K64
      0x50100200,  //  0001  LDBOOL	R4	1	0
      0x90028004,  //  0002  SETMBR	R0	K64	R4
      0xB8120400,  //  0003  GETNGBL	R4	K2
      0x8810092B,  //  0004  GETMBR	R4	R4	K43
      0x88100941,  //  0005  GETMBR	R4	R4	K65
      0x780A0004,  //  0006  JMPF	R2	#000C
      0x28140942,  //  0007  GE	R5	R4	K66
      0x78160002,  //  0008  JMPF	R5	#000C
      0xB8160400,  //  0009  GETNGBL	R5	K2
      0x88140B2B,  //  000A  GETMBR	R5	R5	K43
      0x9016832D,  //  000B  SETMBR	R5	K65	K45
      0x8C140143,  //  000C  GETMET	R5	R0	K67
      0x5C1C0200,  //  000D  MOVE	R7	R1
      0x7C140400,  //  000E  CALL	R5	2
      0x4C140000,  //  000F  LDNIL	R5
      0x88180140,  //  0010  GETMBR	R6	R0	K64
      0x501C0200,  //  0011  LDBOOL	R7	1	0
      0x20180C07,  //  0012  NE	R6	R6	R7
      0x781A0000,  //  0013  JMPF	R6	#0015
      0x88140140,  //  0014  GETMBR	R5	R0	K64
      0x90028003,  //  0015  SETMBR	R0	K64	R3
      0x780A0002,  //  0016  JMPF	R2	#001A
      0xB81A0400,  //  0017  GETNGBL	R6	K2
      0x88180D2B,  //  0018  GETMBR	R6	R6	K43
      0x901A8204,  //  0019  SETMBR	R6	K65	R4
      0x80040A00,  //  001A  RET	1	R5
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
      0xA40E0200,  //  0000  IMPORT	R3	K1
      0x8C100735,  //  0001  GETMET	R4	R3	K53
      0x5C180400,  //  0002  MOVE	R6	R2
      0x7C100400,  //  0003  CALL	R4	2
      0x6014000F,  //  0004  GETGBL	R5	G15
      0x5C180200,  //  0005  MOVE	R6	R1
      0x601C0013,  //  0006  GETGBL	R7	G19
      0x7C140400,  //  0007  CALL	R5	2
      0x78160013,  //  0008  JMPF	R5	#001D
      0x60140010,  //  0009  GETGBL	R5	G16
      0x8C180344,  //  000A  GETMET	R6	R1	K68
      0x7C180200,  //  000B  CALL	R6	1
      0x7C140200,  //  000C  CALL	R5	1
      0xA802000B,  //  000D  EXBLK	0	#001A
      0x5C180A00,  //  000E  MOVE	R6	R5
      0x7C180000,  //  000F  CALL	R6	0
      0x8C1C0735,  //  0010  GETMET	R7	R3	K53
      0x5C240C00,  //  0011  MOVE	R9	R6
      0x7C1C0400,  //  0012  CALL	R7	2
      0x1C1C0E04,  //  0013  EQ	R7	R7	R4
      0x741E0001,  //  0014  JMPT	R7	#0017
      0x1C1C0545,  //  0015  EQ	R7	R2	K69
      0x781E0001,  //  0016  JMPF	R7	#0019
      0xA8040001,  //  0017  EXBLK	1	1
      0x80040C00,  //  0018  RET	1	R6
      0x7001FFF3,  //  0019  JMP		#000E
      0x58140046,  //  001A  LDCONST	R5	K70
      0xAC140200,  //  001B  CATCH	R5	1	0
      0xB0080000,  //  001C  RAISE	2	R0	R0
      0x80000000,  //  001D  RET	0
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
      0x880C0147,  //  0000  GETMBR	R3	R0	K71
      0x780E0017,  //  0001  JMPF	R3	#001A
      0x580C0004,  //  0002  LDCONST	R3	K4
      0x6010000C,  //  0003  GETGBL	R4	G12
      0x88140147,  //  0004  GETMBR	R5	R0	K71
      0x7C100200,  //  0005  CALL	R4	1
      0x14100604,  //  0006  LT	R4	R3	R4
      0x78120011,  //  0007  JMPF	R4	#001A
      0x88100147,  //  0008  GETMBR	R4	R0	K71
      0x94100803,  //  0009  GETIDX	R4	R4	R3
      0x8810093C,  //  000A  GETMBR	R4	R4	K60
      0x88100948,  //  000B  GETMBR	R4	R4	K72
      0x1C100801,  //  000C  EQ	R4	R4	R1
      0x78120009,  //  000D  JMPF	R4	#0018
      0x88100147,  //  000E  GETMBR	R4	R0	K71
      0x94100803,  //  000F  GETIDX	R4	R4	R3
      0x88100930,  //  0010  GETMBR	R4	R4	K48
      0x1C100802,  //  0011  EQ	R4	R4	R2
      0x78120004,  //  0012  JMPF	R4	#0018
      0x88100147,  //  0013  GETMBR	R4	R0	K71
      0x8C100931,  //  0014  GETMET	R4	R4	K49
      0x5C180600,  //  0015  MOVE	R6	R3
      0x7C100400,  //  0016  CALL	R4	2
      0x70020000,  //  0017  JMP		#0019
      0x000C072D,  //  0018  ADD	R3	R3	K45
      0x7001FFE8,  //  0019  JMP		#0003
      0x80000000,  //  001A  RET	0
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
    ( &(const binstruction[20]) {  /* code */
      0x88080149,  //  0000  GETMBR	R2	R0	K73
      0x780A000A,  //  0001  JMPF	R2	#000D
      0x88080149,  //  0002  GETMBR	R2	R0	K73
      0x8C08050C,  //  0003  GETMET	R2	R2	K12
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x4C0C0000,  //  0006  LDNIL	R3
      0x200C0403,  //  0007  NE	R3	R2	R3
      0x780E0003,  //  0008  JMPF	R3	#000D
      0x880C0149,  //  0009  GETMBR	R3	R0	K73
      0x8C0C074A,  //  000A  GETMET	R3	R3	K74
      0x5C140400,  //  000B  MOVE	R5	R2
      0x7C0C0400,  //  000C  CALL	R3	2
      0x8808014B,  //  000D  GETMBR	R2	R0	K75
      0x780A0003,  //  000E  JMPF	R2	#0013
      0x8808014B,  //  000F  GETMBR	R2	R0	K75
      0x8C08054C,  //  0010  GETMET	R2	R2	K76
      0x5C100200,  //  0011  MOVE	R4	R1
      0x7C080400,  //  0012  CALL	R2	2
      0x80000000,  //  0013  RET	0
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
      0x8C0C011A,  //  0000  GETMET	R3	R0	K26
      0x5C140400,  //  0001  MOVE	R5	R2
      0x7C0C0400,  //  0002  CALL	R3	2
      0x880C0116,  //  0003  GETMBR	R3	R0	K22
      0x4C100000,  //  0004  LDNIL	R4
      0x1C0C0604,  //  0005  EQ	R3	R3	R4
      0x780E0002,  //  0006  JMPF	R3	#000A
      0x600C0013,  //  0007  GETGBL	R3	G19
      0x7C0C0000,  //  0008  CALL	R3	0
      0x90022C03,  //  0009  SETMBR	R0	K22	R3
      0x600C0004,  //  000A  GETGBL	R3	G4
      0x5C100400,  //  000B  MOVE	R4	R2
      0x7C0C0200,  //  000C  CALL	R3	1
      0x1C0C0728,  //  000D  EQ	R3	R3	K40
      0x780E0002,  //  000E  JMPF	R3	#0012
      0x880C0116,  //  000F  GETMBR	R3	R0	K22
      0x980C0202,  //  0010  SETIDX	R3	R1	R2
      0x70020000,  //  0011  JMP		#0013
      0xB006534D,  //  0012  RAISE	1	K41	K77
      0x80000000,  //  0013  RET	0
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
      0x88080127,  //  0000  GETMBR	R2	R0	K39
      0x740A0000,  //  0001  JMPT	R2	#0003
      0x80000400,  //  0002  RET	0
      0x88080127,  //  0003  GETMBR	R2	R0	K39
      0x8C08050C,  //  0004  GETMET	R2	R2	K12
      0x5C100200,  //  0005  MOVE	R4	R1
      0x7C080400,  //  0006  CALL	R2	2
      0x4C0C0000,  //  0007  LDNIL	R3
      0x200C0403,  //  0008  NE	R3	R2	R3
      0x780E0003,  //  0009  JMPF	R3	#000E
      0x880C0127,  //  000A  GETMBR	R3	R0	K39
      0x8C0C0731,  //  000B  GETMET	R3	R3	K49
      0x5C140400,  //  000C  MOVE	R5	R2
      0x7C0C0400,  //  000D  CALL	R3	2
      0x80000000,  //  000E  RET	0
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
      0x8C10011A,  //  0000  GETMET	R4	R0	K26
      0x5C180400,  //  0001  MOVE	R6	R2
      0x7C100400,  //  0002  CALL	R4	2
      0x88100139,  //  0003  GETMBR	R4	R0	K57
      0x4C140000,  //  0004  LDNIL	R5
      0x1C100805,  //  0005  EQ	R4	R4	R5
      0x78120002,  //  0006  JMPF	R4	#000A
      0x60100012,  //  0007  GETGBL	R4	G18
      0x7C100000,  //  0008  CALL	R4	0
      0x90027204,  //  0009  SETMBR	R0	K57	R4
      0xB8127400,  //  000A  GETNGBL	R4	K58
      0x60140008,  //  000B  GETGBL	R5	G8
      0x5C180200,  //  000C  MOVE	R6	R1
      0x7C140200,  //  000D  CALL	R5	1
      0x7C100200,  //  000E  CALL	R4	1
      0x8C14093D,  //  000F  GETMET	R5	R4	K61
      0x7C140200,  //  0010  CALL	R5	1
      0x88180139,  //  0011  GETMBR	R6	R0	K57
      0x8C180D1D,  //  0012  GETMET	R6	R6	K29
      0xB8229C00,  //  0013  GETNGBL	R8	K78
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
      0x88080147,  //  0000  GETMBR	R2	R0	K71
      0x780A0024,  //  0001  JMPF	R2	#0027
      0xA40A0000,  //  0002  IMPORT	R2	K0
      0x8C0C050B,  //  0003  GETMET	R3	R2	K11
      0x5C140200,  //  0004  MOVE	R5	R1
      0x7C0C0400,  //  0005  CALL	R3	2
      0x50100000,  //  0006  LDBOOL	R4	0	0
      0x4C140000,  //  0007  LDNIL	R5
      0x1C140605,  //  0008  EQ	R5	R3	R5
      0x78160004,  //  0009  JMPF	R5	#000F
      0x8C140114,  //  000A  GETMET	R5	R0	K20
      0x001E9E01,  //  000B  ADD	R7	K79	R1
      0x58200050,  //  000C  LDCONST	R8	K80
      0x7C140600,  //  000D  CALL	R5	3
      0x5C0C0200,  //  000E  MOVE	R3	R1
      0x60140013,  //  000F  GETGBL	R5	G19
      0x7C140000,  //  0010  CALL	R5	0
      0x9816A203,  //  0011  SETIDX	R5	K81	R3
      0x5C0C0A00,  //  0012  MOVE	R3	R5
      0x58140004,  //  0013  LDCONST	R5	K4
      0x6018000C,  //  0014  GETGBL	R6	G12
      0x881C0147,  //  0015  GETMBR	R7	R0	K71
      0x7C180200,  //  0016  CALL	R6	1
      0x14180A06,  //  0017  LT	R6	R5	R6
      0x781A000C,  //  0018  JMPF	R6	#0026
      0x88180147,  //  0019  GETMBR	R6	R0	K71
      0x94180C05,  //  001A  GETIDX	R6	R6	R5
      0x8C1C0152,  //  001B  GETMET	R7	R0	K82
      0x5C240600,  //  001C  MOVE	R9	R3
      0x88280D3C,  //  001D  GETMBR	R10	R6	K60
      0x882C0D3F,  //  001E  GETMBR	R11	R6	K63
      0x7C1C0800,  //  001F  CALL	R7	4
      0x741E0001,  //  0020  JMPT	R7	#0023
      0x74120000,  //  0021  JMPT	R4	#0023
      0x50100001,  //  0022  LDBOOL	R4	0	1
      0x50100200,  //  0023  LDBOOL	R4	1	0
      0x00140B2D,  //  0024  ADD	R5	R5	K45
      0x7001FFED,  //  0025  JMP		#0014
      0x80040800,  //  0026  RET	1	R4
      0x50080000,  //  0027  LDBOOL	R2	0	0
      0x80040400,  //  0028  RET	1	R2
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
      0xA40AA600,  //  0000  IMPORT	R2	K83
      0x8C0C0554,  //  0001  GETMET	R3	R2	K84
      0x5C140200,  //  0002  MOVE	R5	R1
      0x7C0C0400,  //  0003  CALL	R3	2
      0x80040600,  //  0004  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: run_network_up
********************************************************************/
be_local_closure(class_Tasmota_run_network_up,   /* name */
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
    &be_const_str_run_network_up,
    &be_const_str_solidified,
    ( &(const binstruction[39]) {  /* code */
      0x8804011C,  //  0000  GETMBR	R1	R0	K28
      0x4C080000,  //  0001  LDNIL	R2
      0x1C040202,  //  0002  EQ	R1	R1	R2
      0x78060000,  //  0003  JMPF	R1	#0005
      0x80000200,  //  0004  RET	0
      0x8C04011B,  //  0005  GETMET	R1	R0	K27
      0x7C040200,  //  0006  CALL	R1	1
      0x7806001D,  //  0007  JMPF	R1	#0026
      0x6004000C,  //  0008  GETGBL	R1	G12
      0x8808011C,  //  0009  GETMBR	R2	R0	K28
      0x7C040200,  //  000A  CALL	R1	1
      0x24040304,  //  000B  GT	R1	R1	K4
      0x78060016,  //  000C  JMPF	R1	#0024
      0x8804011C,  //  000D  GETMBR	R1	R0	K28
      0x94040304,  //  000E  GETIDX	R1	R1	K4
      0x8808011C,  //  000F  GETMBR	R2	R0	K28
      0x8C080531,  //  0010  GETMET	R2	R2	K49
      0x58100004,  //  0011  LDCONST	R4	K4
      0x7C080400,  //  0012  CALL	R2	2
      0xA8020003,  //  0013  EXBLK	0	#0018
      0x5C080200,  //  0014  MOVE	R2	R1
      0x7C080000,  //  0015  CALL	R2	0
      0xA8040001,  //  0016  EXBLK	1	1
      0x7002000A,  //  0017  JMP		#0023
      0xAC080002,  //  0018  CATCH	R2	0	2
      0x70020007,  //  0019  JMP		#0022
      0x60100001,  //  001A  GETGBL	R4	G1
      0x60140018,  //  001B  GETGBL	R5	G24
      0x58180055,  //  001C  LDCONST	R6	K85
      0x5C1C0400,  //  001D  MOVE	R7	R2
      0x5C200600,  //  001E  MOVE	R8	R3
      0x7C140600,  //  001F  CALL	R5	3
      0x7C100200,  //  0020  CALL	R4	1
      0x70020000,  //  0021  JMP		#0023
      0xB0080000,  //  0022  RAISE	2	R0	R0
      0x7001FFE3,  //  0023  JMP		#0008
      0x4C040000,  //  0024  LDNIL	R1
      0x90023801,  //  0025  SETMBR	R0	K28	R1
      0x80000000,  //  0026  RET	0
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
      0x8C040156,  //  0000  GETMET	R1	R0	K86
      0x7C040200,  //  0001  CALL	R1	1
      0x8804012E,  //  0002  GETMBR	R1	R0	K46
      0x78060015,  //  0003  JMPF	R1	#001A
      0x58040004,  //  0004  LDCONST	R1	K4
      0x8808012E,  //  0005  GETMBR	R2	R0	K46
      0x8C08052F,  //  0006  GETMET	R2	R2	K47
      0x7C080200,  //  0007  CALL	R2	1
      0x14080202,  //  0008  LT	R2	R1	R2
      0x780A000F,  //  0009  JMPF	R2	#001A
      0x8808012E,  //  000A  GETMBR	R2	R0	K46
      0x94080401,  //  000B  GETIDX	R2	R2	R1
      0x8C0C013E,  //  000C  GETMET	R3	R0	K62
      0x8814053C,  //  000D  GETMBR	R5	R2	K60
      0x7C0C0400,  //  000E  CALL	R3	2
      0x780E0007,  //  000F  JMPF	R3	#0018
      0x880C053F,  //  0010  GETMBR	R3	R2	K63
      0x8810012E,  //  0011  GETMBR	R4	R0	K46
      0x8C100931,  //  0012  GETMET	R4	R4	K49
      0x5C180200,  //  0013  MOVE	R6	R1
      0x7C100400,  //  0014  CALL	R4	2
      0x5C100600,  //  0015  MOVE	R4	R3
      0x7C100000,  //  0016  CALL	R4	0
      0x70020000,  //  0017  JMP		#0019
      0x0004032D,  //  0018  ADD	R1	R1	K45
      0x7001FFEA,  //  0019  JMP		#0005
      0x80000000,  //  001A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: add_extension
********************************************************************/
be_local_closure(class_Tasmota_add_extension,   /* name */
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
    &be_const_str_add_extension,
    &be_const_str_solidified,
    ( &(const binstruction[51]) {  /* code */
      0x4C0C0000,  //  0000  LDNIL	R3
      0x1C0C0403,  //  0001  EQ	R3	R2	R3
      0x780E0001,  //  0002  JMPF	R3	#0005
      0xB80E0400,  //  0003  GETNGBL	R3	K2
      0x88080703,  //  0004  GETMBR	R2	R3	K3
      0x600C0004,  //  0005  GETGBL	R3	G4
      0x5C100200,  //  0006  MOVE	R4	R1
      0x7C0C0200,  //  0007  CALL	R3	1
      0x200C0757,  //  0008  NE	R3	R3	K87
      0x740E0004,  //  0009  JMPT	R3	#000F
      0x600C0004,  //  000A  GETGBL	R3	G4
      0x5C100400,  //  000B  MOVE	R4	R2
      0x7C0C0200,  //  000C  CALL	R3	1
      0x200C0701,  //  000D  NE	R3	R3	K1
      0x780E0000,  //  000E  JMPF	R3	#0010
      0xB0065358,  //  000F  RAISE	1	K41	K88
      0x4C0C0000,  //  0010  LDNIL	R3
      0x200C0403,  //  0011  NE	R3	R2	R3
      0x780E001E,  //  0012  JMPF	R3	#0032
      0xA40E0200,  //  0013  IMPORT	R3	K1
      0x8810014B,  //  0014  GETMBR	R4	R0	K75
      0x4C140000,  //  0015  LDNIL	R5
      0x1C100805,  //  0016  EQ	R4	R4	R5
      0x78120002,  //  0017  JMPF	R4	#001B
      0xB812B200,  //  0018  GETNGBL	R4	K89
      0x7C100000,  //  0019  CALL	R4	0
      0x90029604,  //  001A  SETMBR	R0	K75	R4
      0x8C100712,  //  001B  GETMET	R4	R3	K18
      0x5C180400,  //  001C  MOVE	R6	R2
      0x581C0006,  //  001D  LDCONST	R7	K6
      0x7C100600,  //  001E  CALL	R4	3
      0x78120002,  //  001F  JMPF	R4	#0023
      0x5411FFFD,  //  0020  LDINT	R4	-2
      0x40120804,  //  0021  CONNECT	R4	K4	R4
      0x94080404,  //  0022  GETIDX	R2	R2	R4
      0x8810014B,  //  0023  GETMBR	R4	R0	K75
      0x8C10095A,  //  0024  GETMET	R4	R4	K90
      0x5C180400,  //  0025  MOVE	R6	R2
      0x7C100400,  //  0026  CALL	R4	2
      0x78120007,  //  0027  JMPF	R4	#0030
      0xB8122800,  //  0028  GETNGBL	R4	K20
      0x60140018,  //  0029  GETGBL	R5	G24
      0x5818005B,  //  002A  LDCONST	R6	K91
      0x5C1C0400,  //  002B  MOVE	R7	R2
      0x7C140400,  //  002C  CALL	R5	2
      0x58180050,  //  002D  LDCONST	R6	K80
      0x7C100400,  //  002E  CALL	R4	2
      0x70020001,  //  002F  JMP		#0032
      0x8810014B,  //  0030  GETMBR	R4	R0	K75
      0x98100401,  //  0031  SETIDX	R4	R2	R1
      0x80000000,  //  0032  RET	0
    })
  )
);
/*******************************************************************/


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
      0x8C0C015C,  //  0003  GETMET	R3	R0	K92
      0x5C140400,  //  0004  MOVE	R5	R2
      0x7C0C0400,  //  0005  CALL	R3	2
      0x740E0001,  //  0006  JMPT	R3	#0009
      0x4C0C0000,  //  0007  LDNIL	R3
      0x80040600,  //  0008  RET	1	R3
      0x880C015D,  //  0009  GETMBR	R3	R0	K93
      0x8C0C075E,  //  000A  GETMET	R3	R3	K94
      0x7C0C0200,  //  000B  CALL	R3	1
      0x780E0006,  //  000C  JMPF	R3	#0014
      0x880C015D,  //  000D  GETMBR	R3	R0	K93
      0x8C0C075F,  //  000E  GETMET	R3	R3	K95
      0x5C140200,  //  000F  MOVE	R5	R1
      0x7C0C0400,  //  0010  CALL	R3	2
      0x780E0001,  //  0011  JMPF	R3	#0014
      0x880C015D,  //  0012  GETMBR	R3	R0	K93
      0x80040600,  //  0013  RET	1	R3
      0x880C0160,  //  0014  GETMBR	R3	R0	K96
      0x8C0C075E,  //  0015  GETMET	R3	R3	K94
      0x7C0C0200,  //  0016  CALL	R3	1
      0x780E0006,  //  0017  JMPF	R3	#001F
      0x880C0160,  //  0018  GETMBR	R3	R0	K96
      0x8C0C075F,  //  0019  GETMET	R3	R3	K95
      0x5C140200,  //  001A  MOVE	R5	R1
      0x7C0C0400,  //  001B  CALL	R3	2
      0x780E0001,  //  001C  JMPF	R3	#001F
      0x880C0160,  //  001D  GETMBR	R3	R0	K96
      0x80040600,  //  001E  RET	1	R3
      0x4C0C0000,  //  001F  LDNIL	R3
      0x80040600,  //  0020  RET	1	R3
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
      0xA40E0200,  //  0003  IMPORT	R3	K1
      0x8C100761,  //  0004  GETMET	R4	R3	K97
      0x5C180200,  //  0005  MOVE	R6	R1
      0x581C0005,  //  0006  LDCONST	R7	K5
      0x7C100600,  //  0007  CALL	R4	3
      0x8C10094A,  //  0008  GETMET	R4	R4	K74
      0x7C100200,  //  0009  CALL	R4	1
      0x5C080800,  //  000A  MOVE	R2	R4
      0x6010000C,  //  000B  GETGBL	R4	G12
      0x5C140400,  //  000C  MOVE	R5	R2
      0x7C100200,  //  000D  CALL	R4	1
      0x1C100904,  //  000E  EQ	R4	R4	K4
      0x78120000,  //  000F  JMPF	R4	#0011
      0x58080062,  //  0010  LDCONST	R2	K98
      0xB80EC600,  //  0011  GETNGBL	R3	K99
      0x7C0C0000,  //  0012  CALL	R3	0
      0x8C100764,  //  0013  GETMET	R4	R3	K100
      0x50180200,  //  0014  LDBOOL	R6	1	0
      0x7C100400,  //  0015  CALL	R4	2
      0x8C100765,  //  0016  GETMET	R4	R3	K101
      0x5C180200,  //  0017  MOVE	R6	R1
      0x7C100400,  //  0018  CALL	R4	2
      0x8C100766,  //  0019  GETMET	R4	R3	K102
      0x7C100200,  //  001A  CALL	R4	1
      0x541600C7,  //  001B  LDINT	R5	200
      0x20140805,  //  001C  NE	R5	R4	R5
      0x78160004,  //  001D  JMPF	R5	#0023
      0x60140008,  //  001E  GETGBL	R5	G8
      0x5C180800,  //  001F  MOVE	R6	R4
      0x7C140200,  //  0020  CALL	R5	1
      0x0016CE05,  //  0021  ADD	R5	K103	R5
      0xB006D005,  //  0022  RAISE	1	K104	R5
      0x8C140769,  //  0023  GETMET	R5	R3	K105
      0x5C1C0400,  //  0024  MOVE	R7	R2
      0x7C140400,  //  0025  CALL	R5	2
      0x8C18070A,  //  0026  GETMET	R6	R3	K10
      0x7C180200,  //  0027  CALL	R6	1
      0x8C180114,  //  0028  GETMET	R6	R0	K20
      0x60200008,  //  0029  GETGBL	R8	G8
      0x5C240A00,  //  002A  MOVE	R9	R5
      0x7C200200,  //  002B  CALL	R8	1
      0x0022D408,  //  002C  ADD	R8	K106	R8
      0x58240050,  //  002D  LDCONST	R9	K80
      0x7C180600,  //  002E  CALL	R6	3
      0x80040800,  //  002F  RET	1	R4
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
      0x8804016B,  //  0000  GETMBR	R1	R0	K107
      0x78060016,  //  0001  JMPF	R1	#0019
      0x6004000C,  //  0002  GETGBL	R1	G12
      0x8808016B,  //  0003  GETMBR	R2	R0	K107
      0x7C040200,  //  0004  CALL	R1	1
      0x24080304,  //  0005  GT	R2	R1	K4
      0x780A0009,  //  0006  JMPF	R2	#0011
      0x8808016B,  //  0007  GETMBR	R2	R0	K107
      0x94080504,  //  0008  GETIDX	R2	R2	K4
      0x880C016B,  //  0009  GETMBR	R3	R0	K107
      0x8C0C0731,  //  000A  GETMET	R3	R3	K49
      0x58140004,  //  000B  LDCONST	R5	K4
      0x7C0C0400,  //  000C  CALL	R3	2
      0x0404032D,  //  000D  SUB	R1	R1	K45
      0x5C0C0400,  //  000E  MOVE	R3	R2
      0x7C0C0000,  //  000F  CALL	R3	0
      0x7001FFF3,  //  0010  JMP		#0005
      0x6008000C,  //  0011  GETGBL	R2	G12
      0x880C016B,  //  0012  GETMBR	R3	R0	K107
      0x7C080200,  //  0013  CALL	R2	1
      0x1C080504,  //  0014  EQ	R2	R2	K4
      0x780A0002,  //  0015  JMPF	R2	#0019
      0xB80A0400,  //  0016  GETNGBL	R2	K2
      0x8808052B,  //  0017  GETMBR	R2	R2	K43
      0x900AD904,  //  0018  SETMBR	R2	K108	K4
      0x80000000,  //  0019  RET	0
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
      0xB806DA00,  //  0000  GETNGBL	R1	K109
      0x8808016E,  //  0001  GETMBR	R2	R0	K110
      0x880C016F,  //  0002  GETMBR	R3	R0	K111
      0x7C040400,  //  0003  CALL	R1	2
      0x90025601,  //  0004  SETMBR	R0	K43	R1
      0xA406E000,  //  0005  IMPORT	R1	K112
      0x60080015,  //  0006  GETGBL	R2	G21
      0x880C0171,  //  0007  GETMBR	R3	R0	K113
      0x54120003,  //  0008  LDINT	R4	4
      0x7C080400,  //  0009  CALL	R2	2
      0x8C080572,  //  000A  GETMET	R2	R2	K114
      0x58100004,  //  000B  LDCONST	R4	K4
      0x54160003,  //  000C  LDINT	R5	4
      0x7C080600,  //  000D  CALL	R2	3
      0x780A0006,  //  000E  JMPF	R2	#0016
      0xB80EDA00,  //  000F  GETNGBL	R3	K109
      0x8C100374,  //  0010  GETMET	R4	R1	K116
      0x5C180400,  //  0011  MOVE	R6	R2
      0x7C100400,  //  0012  CALL	R4	2
      0x88140175,  //  0013  GETMBR	R5	R0	K117
      0x7C0C0400,  //  0014  CALL	R3	2
      0x9002E603,  //  0015  SETMBR	R0	K115	R3
      0x90020707,  //  0016  SETMBR	R0	K3	K7
      0xB80E5600,  //  0017  GETNGBL	R3	K43
      0x8C0C075A,  //  0018  GETMET	R3	R3	K90
      0x58140077,  //  0019  LDCONST	R5	K119
      0x7C0C0400,  //  001A  CALL	R3	2
      0x9002EC03,  //  001B  SETMBR	R0	K118	R3
      0x8C0C0178,  //  001C  GETMET	R3	R0	K120
      0x58140079,  //  001D  LDCONST	R5	K121
      0x84180000,  //  001E  CLOSURE	R6	P0
      0x7C0C0600,  //  001F  CALL	R3	3
      0xA0000000,  //  0020  CLOSE	R0
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
      0x8808016B,  //  0000  GETMBR	R2	R0	K107
      0x4C0C0000,  //  0001  LDNIL	R3
      0x1C080403,  //  0002  EQ	R2	R2	R3
      0x780A0002,  //  0003  JMPF	R2	#0007
      0x60080012,  //  0004  GETGBL	R2	G18
      0x7C080000,  //  0005  CALL	R2	0
      0x9002D602,  //  0006  SETMBR	R0	K107	R2
      0x8808016B,  //  0007  GETMBR	R2	R0	K107
      0x8C08051D,  //  0008  GETMET	R2	R2	K29
      0x5C100200,  //  0009  MOVE	R4	R1
      0x7C080400,  //  000A  CALL	R2	2
      0xB80A0400,  //  000B  GETNGBL	R2	K2
      0x8808052B,  //  000C  GETMBR	R2	R2	K43
      0x900AD92D,  //  000D  SETMBR	R2	K108	K45
      0x80000000,  //  000E  RET	0
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
      0x88080139,  //  0000  GETMBR	R2	R0	K57
      0x780A000D,  //  0001  JMPF	R2	#0010
      0x580C0004,  //  0002  LDCONST	R3	K4
      0x8C10052F,  //  0003  GETMET	R4	R2	K47
      0x7C100200,  //  0004  CALL	R4	1
      0x14100604,  //  0005  LT	R4	R3	R4
      0x78120008,  //  0006  JMPF	R4	#0010
      0x94100403,  //  0007  GETIDX	R4	R2	R3
      0x88100930,  //  0008  GETMBR	R4	R4	K48
      0x1C100801,  //  0009  EQ	R4	R4	R1
      0x78120002,  //  000A  JMPF	R4	#000E
      0x94100403,  //  000B  GETIDX	R4	R2	R3
      0x8810093C,  //  000C  GETMBR	R4	R4	K60
      0x80040800,  //  000D  RET	1	R4
      0x000C072D,  //  000E  ADD	R3	R3	K45
      0x7001FFF2,  //  000F  JMP		#0003
      0x80000000,  //  0010  RET	0
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
      0x88040127,  //  0000  GETMBR	R1	R0	K39
      0x5C080200,  //  0001  MOVE	R2	R1
      0x740A0000,  //  0002  JMPT	R2	#0004
      0x80000400,  //  0003  RET	0
      0x58080004,  //  0004  LDCONST	R2	K4
      0x600C000C,  //  0005  GETGBL	R3	G12
      0x5C100200,  //  0006  MOVE	R4	R1
      0x7C0C0200,  //  0007  CALL	R3	1
      0x140C0403,  //  0008  LT	R3	R2	R3
      0x780E0003,  //  0009  JMPF	R3	#000E
      0x940C0202,  //  000A  GETIDX	R3	R1	R2
      0x7C0C0000,  //  000B  CALL	R3	0
      0x0008052D,  //  000C  ADD	R2	R2	K45
      0x7001FFF6,  //  000D  JMP		#0005
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: unload_extension
********************************************************************/
be_local_closure(class_Tasmota_unload_extension,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Tasmota,     /* shared constants */
    &be_const_str_unload_extension,
    &be_const_str_solidified,
    ( &(const binstruction[44]) {  /* code */
      0x8808014B,  //  0000  GETMBR	R2	R0	K75
      0x4C0C0000,  //  0001  LDNIL	R3
      0x1C080403,  //  0002  EQ	R2	R2	R3
      0x780A0001,  //  0003  JMPF	R2	#0006
      0x50080000,  //  0004  LDBOOL	R2	0	0
      0x80040400,  //  0005  RET	1	R2
      0x5C080200,  //  0006  MOVE	R2	R1
      0x600C0004,  //  0007  GETGBL	R3	G4
      0x5C100200,  //  0008  MOVE	R4	R1
      0x7C0C0200,  //  0009  CALL	R3	1
      0x1C0C0701,  //  000A  EQ	R3	R3	K1
      0x780E0004,  //  000B  JMPF	R3	#0011
      0x880C014B,  //  000C  GETMBR	R3	R0	K75
      0x8C0C070C,  //  000D  GETMET	R3	R3	K12
      0x5C140200,  //  000E  MOVE	R5	R1
      0x7C0C0400,  //  000F  CALL	R3	2
      0x5C080600,  //  0010  MOVE	R2	R3
      0x600C0004,  //  0011  GETGBL	R3	G4
      0x5C100400,  //  0012  MOVE	R4	R2
      0x7C0C0200,  //  0013  CALL	R3	1
      0x1C0C0757,  //  0014  EQ	R3	R3	K87
      0x780E0012,  //  0015  JMPF	R3	#0029
      0xA40EE000,  //  0016  IMPORT	R3	K112
      0x8C10075A,  //  0017  GETMET	R4	R3	K90
      0x5C180400,  //  0018  MOVE	R6	R2
      0x581C007A,  //  0019  LDCONST	R7	K122
      0x7C100600,  //  001A  CALL	R4	3
      0x78120001,  //  001B  JMPF	R4	#001E
      0x8C10057A,  //  001C  GETMET	R4	R2	K122
      0x7C100200,  //  001D  CALL	R4	1
      0x8C10017B,  //  001E  GETMET	R4	R0	K123
      0x5C180400,  //  001F  MOVE	R6	R2
      0x7C100400,  //  0020  CALL	R4	2
      0x4C040000,  //  0021  LDNIL	R1
      0x4C080000,  //  0022  LDNIL	R2
      0xB8120400,  //  0023  GETNGBL	R4	K2
      0x8C10097C,  //  0024  GETMET	R4	R4	K124
      0x7C100200,  //  0025  CALL	R4	1
      0x50100200,  //  0026  LDBOOL	R4	1	0
      0x80040800,  //  0027  RET	1	R4
      0x70020001,  //  0028  JMP		#002B
      0x500C0000,  //  0029  LDBOOL	R3	0	0
      0x80040600,  //  002A  RET	1	R3
      0x80000000,  //  002B  RET	0
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
      0x580C007D,  //  0001  LDCONST	R3	K125
      0x7C080200,  //  0002  CALL	R2	1
      0xA40A6E00,  //  0003  IMPORT	R2	K55
      0x4C0C0000,  //  0004  LDNIL	R3
      0x200C0203,  //  0005  NE	R3	R1	R3
      0x780E0004,  //  0006  JMPF	R3	#000C
      0x8C0C0572,  //  0007  GETMET	R3	R2	K114
      0x5C140200,  //  0008  MOVE	R5	R1
      0x7C0C0400,  //  0009  CALL	R3	2
      0x80040600,  //  000A  RET	1	R3
      0x70020002,  //  000B  JMP		#000F
      0x8C0C0572,  //  000C  GETMET	R3	R2	K114
      0x7C0C0200,  //  000D  CALL	R3	1
      0x80040600,  //  000E  RET	1	R3
      0x80000000,  //  000F  RET	0
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
    ( &(const binstruction[176]) {  /* code */
      0x84080000,  //  0000  CLOSURE	R2	P0
      0x840C0001,  //  0001  CLOSURE	R3	P1
      0x84100002,  //  0002  CLOSURE	R4	P2
      0x84140003,  //  0003  CLOSURE	R5	P3
      0x84180004,  //  0004  CLOSURE	R6	P4
      0x841C0005,  //  0005  CLOSURE	R7	P5
      0xA4220200,  //  0006  IMPORT	R8	K1
      0xA426FC00,  //  0007  IMPORT	R9	K126
      0x6028000C,  //  0008  GETGBL	R10	G12
      0x5C2C0200,  //  0009  MOVE	R11	R1
      0x7C280200,  //  000A  CALL	R10	1
      0x1C281504,  //  000B  EQ	R10	R10	K4
      0x782A0002,  //  000C  JMPF	R10	#0010
      0x50280000,  //  000D  LDBOOL	R10	0	0
      0xA0000000,  //  000E  CLOSE	R0
      0x80041400,  //  000F  RET	1	R10
      0x8C28117F,  //  0010  GETMET	R10	R8	K127
      0x5C300200,  //  0011  MOVE	R12	R1
      0x58340005,  //  0012  LDCONST	R13	K5
      0x7C280600,  //  0013  CALL	R10	3
      0x742A0000,  //  0014  JMPT	R10	#0016
      0x00060A01,  //  0015  ADD	R1	K5	R1
      0x8C281112,  //  0016  GETMET	R10	R8	K18
      0x5C300200,  //  0017  MOVE	R12	R1
      0x58340013,  //  0018  LDCONST	R13	K19
      0x7C280600,  //  0019  CALL	R10	3
      0x742A0004,  //  001A  JMPT	R10	#0020
      0x8C281112,  //  001B  GETMET	R10	R8	K18
      0x5C300200,  //  001C  MOVE	R12	R1
      0x58340080,  //  001D  LDCONST	R13	K128
      0x7C280600,  //  001E  CALL	R10	3
      0x782A0000,  //  001F  JMPF	R10	#0021
      0x00040381,  //  0020  ADD	R1	R1	K129
      0x8C28110C,  //  0021  GETMET	R10	R8	K12
      0x5C300200,  //  0022  MOVE	R12	R1
      0x58340006,  //  0023  LDCONST	R13	K6
      0x7C280600,  //  0024  CALL	R10	3
      0x242C1504,  //  0025  GT	R11	R10	K4
      0x782E0003,  //  0026  JMPF	R11	#002B
      0x0430152D,  //  0027  SUB	R12	R10	K45
      0x4032080C,  //  0028  CONNECT	R12	K4	R12
      0x9430020C,  //  0029  GETIDX	R12	R1	R12
      0x70020000,  //  002A  JMP		#002C
      0x5C300200,  //  002B  MOVE	R12	R1
      0x782E0003,  //  002C  JMPF	R11	#0031
      0x0034152D,  //  002D  ADD	R13	R10	K45
      0x40341B82,  //  002E  CONNECT	R13	R13	K130
      0x9434020D,  //  002F  GETIDX	R13	R1	R13
      0x70020000,  //  0030  JMP		#0032
      0x5C340200,  //  0031  MOVE	R13	R1
      0x8C38110C,  //  0032  GETMET	R14	R8	K12
      0x5C401A00,  //  0033  MOVE	R16	R13
      0x58440083,  //  0034  LDCONST	R17	K131
      0x7C380600,  //  0035  CALL	R14	3
      0x14381D04,  //  0036  LT	R14	R14	K4
      0x783A0001,  //  0037  JMPF	R14	#003A
      0x0004031E,  //  0038  ADD	R1	R1	K30
      0x00341B1E,  //  0039  ADD	R13	R13	K30
      0x8C381112,  //  003A  GETMET	R14	R8	K18
      0x5C401A00,  //  003B  MOVE	R16	R13
      0x5844001E,  //  003C  LDCONST	R17	K30
      0x7C380600,  //  003D  CALL	R14	3
      0x8C3C1112,  //  003E  GETMET	R15	R8	K18
      0x5C441A00,  //  003F  MOVE	R17	R13
      0x58480084,  //  0040  LDCONST	R18	K132
      0x7C3C0600,  //  0041  CALL	R15	3
      0x783E0001,  //  0042  JMPF	R15	#0045
      0x5C400200,  //  0043  MOVE	R16	R1
      0x70020000,  //  0044  JMP		#0046
      0x00400324,  //  0045  ADD	R16	R1	K36
      0x5C441C00,  //  0046  MOVE	R17	R14
      0x74460007,  //  0047  JMPT	R17	#0050
      0x5C441E00,  //  0048  MOVE	R17	R15
      0x74460005,  //  0049  JMPT	R17	#0050
      0x60440001,  //  004A  GETGBL	R17	G1
      0x58480085,  //  004B  LDCONST	R18	K133
      0x7C440200,  //  004C  CALL	R17	1
      0x50440000,  //  004D  LDBOOL	R17	0	0
      0xA0000000,  //  004E  CLOSE	R0
      0x80042200,  //  004F  RET	1	R17
      0x50440000,  //  0050  LDBOOL	R17	0	0
      0x783E0008,  //  0051  JMPF	R15	#005B
      0x8C481386,  //  0052  GETMET	R18	R9	K134
      0x5C502000,  //  0053  MOVE	R20	R16
      0x7C480400,  //  0054  CALL	R18	2
      0x744A0002,  //  0055  JMPT	R18	#0059
      0x50480000,  //  0056  LDBOOL	R18	0	0
      0xA0000000,  //  0057  CLOSE	R0
      0x80042400,  //  0058  RET	1	R18
      0x50440200,  //  0059  LDBOOL	R17	1	0
      0x70020014,  //  005A  JMP		#0070
      0x8C481386,  //  005B  GETMET	R18	R9	K134
      0x5C500200,  //  005C  MOVE	R20	R1
      0x7C480400,  //  005D  CALL	R18	2
      0x784A0007,  //  005E  JMPF	R18	#0067
      0x8C481386,  //  005F  GETMET	R18	R9	K134
      0x5C502000,  //  0060  MOVE	R20	R16
      0x7C480400,  //  0061  CALL	R18	2
      0x784A0002,  //  0062  JMPF	R18	#0066
      0x5C480A00,  //  0063  MOVE	R18	R5
      0x5C4C2000,  //  0064  MOVE	R19	R16
      0x7C480200,  //  0065  CALL	R18	1
      0x70020008,  //  0066  JMP		#0070
      0x8C481386,  //  0067  GETMET	R18	R9	K134
      0x5C502000,  //  0068  MOVE	R20	R16
      0x7C480400,  //  0069  CALL	R18	2
      0x784A0001,  //  006A  JMPF	R18	#006D
      0x50440200,  //  006B  LDBOOL	R17	1	0
      0x70020002,  //  006C  JMP		#0070
      0x50480000,  //  006D  LDBOOL	R18	0	0
      0xA0000000,  //  006E  CLOSE	R0
      0x80042400,  //  006F  RET	1	R18
      0x782E0005,  //  0070  JMPF	R11	#0077
      0x00481906,  //  0071  ADD	R18	R12	K6
      0x90020612,  //  0072  SETMBR	R0	K3	R18
      0x5C480400,  //  0073  MOVE	R18	R2
      0x884C0103,  //  0074  GETMBR	R19	R0	K3
      0x7C480200,  //  0075  CALL	R18	1
      0x70020000,  //  0076  JMP		#0078
      0x90020707,  //  0077  SETMBR	R0	K3	K7
      0x4C480000,  //  0078  LDNIL	R18
      0x78460025,  //  0079  JMPF	R17	#00A0
      0x5C4C0800,  //  007A  MOVE	R19	R4
      0x5C502000,  //  007B  MOVE	R20	R16
      0x7C4C0200,  //  007C  CALL	R19	1
      0x50500200,  //  007D  LDBOOL	R20	1	0
      0x4C540000,  //  007E  LDNIL	R21
      0x1C542615,  //  007F  EQ	R21	R19	R21
      0x78560007,  //  0080  JMPF	R21	#0089
      0x60540001,  //  0081  GETGBL	R21	G1
      0x60580018,  //  0082  GETGBL	R22	G24
      0x585C0087,  //  0083  LDCONST	R23	K135
      0x5C602000,  //  0084  MOVE	R24	R16
      0x7C580400,  //  0085  CALL	R22	2
      0x7C540200,  //  0086  CALL	R21	1
      0x50500000,  //  0087  LDBOOL	R20	0	0
      0x7002000A,  //  0088  JMP		#0094
      0x54560003,  //  0089  LDINT	R21	4
      0x20542615,  //  008A  NE	R21	R19	R21
      0x78560007,  //  008B  JMPF	R21	#0094
      0x60540001,  //  008C  GETGBL	R21	G1
      0x60580018,  //  008D  GETGBL	R22	G24
      0x585C0088,  //  008E  LDCONST	R23	K136
      0x5C602000,  //  008F  MOVE	R24	R16
      0x5C642600,  //  0090  MOVE	R25	R19
      0x7C580600,  //  0091  CALL	R22	3
      0x7C540200,  //  0092  CALL	R21	1
      0x50500000,  //  0093  LDBOOL	R20	0	0
      0x78520003,  //  0094  JMPF	R20	#0099
      0x5C540C00,  //  0095  MOVE	R21	R6
      0x5C582000,  //  0096  MOVE	R22	R16
      0x7C540200,  //  0097  CALL	R21	1
      0x5C482A00,  //  0098  MOVE	R18	R21
      0x4C540000,  //  0099  LDNIL	R21
      0x1C542415,  //  009A  EQ	R21	R18	R21
      0x78560003,  //  009B  JMPF	R21	#00A0
      0x5C540A00,  //  009C  MOVE	R21	R5
      0x5C582000,  //  009D  MOVE	R22	R16
      0x7C540200,  //  009E  CALL	R21	1
      0x50440000,  //  009F  LDBOOL	R17	0	0
      0x5C4C2200,  //  00A0  MOVE	R19	R17
      0x744E0003,  //  00A1  JMPT	R19	#00A6
      0x5C4C0C00,  //  00A2  MOVE	R19	R6
      0x5C500200,  //  00A3  MOVE	R20	R1
      0x7C4C0200,  //  00A4  CALL	R19	1
      0x5C482600,  //  00A5  MOVE	R18	R19
      0x5C4C0E00,  //  00A6  MOVE	R19	R7
      0x5C502400,  //  00A7  MOVE	R20	R18
      0x7C4C0200,  //  00A8  CALL	R19	1
      0x782E0003,  //  00A9  JMPF	R11	#00AE
      0x5C500600,  //  00AA  MOVE	R20	R3
      0x00541906,  //  00AB  ADD	R21	R12	K6
      0x7C500200,  //  00AC  CALL	R20	1
      0x90020707,  //  00AD  SETMBR	R0	K3	K7
      0xA0000000,  //  00AE  CLOSE	R0
      0x80042600,  //  00AF  RET	1	R19
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
      0x8C14011A,  //  0000  GETMET	R5	R0	K26
      0x5C1C0400,  //  0001  MOVE	R7	R2
      0x7C140400,  //  0002  CALL	R5	2
      0x88140147,  //  0003  GETMBR	R5	R0	K71
      0x4C180000,  //  0004  LDNIL	R6
      0x1C140A06,  //  0005  EQ	R5	R5	R6
      0x78160002,  //  0006  JMPF	R5	#000A
      0x60140012,  //  0007  GETGBL	R5	G18
      0x7C140000,  //  0008  CALL	R5	0
      0x90028E05,  //  0009  SETMBR	R0	K71	R5
      0x60140004,  //  000A  GETGBL	R5	G4
      0x5C180400,  //  000B  MOVE	R6	R2
      0x7C140200,  //  000C  CALL	R5	1
      0x1C140B28,  //  000D  EQ	R5	R5	K40
      0x78160013,  //  000E  JMPF	R5	#0023
      0x4C140000,  //  000F  LDNIL	R5
      0x20140605,  //  0010  NE	R5	R3	R5
      0x78160003,  //  0011  JMPF	R5	#0016
      0x8C140189,  //  0012  GETMET	R5	R0	K137
      0x5C1C0200,  //  0013  MOVE	R7	R1
      0x5C200600,  //  0014  MOVE	R8	R3
      0x7C140600,  //  0015  CALL	R5	3
      0x88140147,  //  0016  GETMBR	R5	R0	K71
      0x8C140B1D,  //  0017  GETMET	R5	R5	K29
      0xB81E9C00,  //  0018  GETNGBL	R7	K78
      0x8820018A,  //  0019  GETMBR	R8	R0	K138
      0x8C20118B,  //  001A  GETMET	R8	R8	K139
      0x5C280200,  //  001B  MOVE	R10	R1
      0x7C200400,  //  001C  CALL	R8	2
      0x5C240400,  //  001D  MOVE	R9	R2
      0x5C280600,  //  001E  MOVE	R10	R3
      0x5C2C0800,  //  001F  MOVE	R11	R4
      0x7C1C0800,  //  0020  CALL	R7	4
      0x7C140400,  //  0021  CALL	R5	2
      0x70020000,  //  0022  JMP		#0024
      0xB006534D,  //  0023  RAISE	1	K41	K77
      0x80000000,  //  0024  RET	0
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
      0xA40AE000,  //  0000  IMPORT	R2	K112
      0x600C0004,  //  0001  GETGBL	R3	G4
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C0C0200,  //  0003  CALL	R3	1
      0x200C0728,  //  0004  NE	R3	R3	K40
      0x780E0000,  //  0005  JMPF	R3	#0007
      0xB007198D,  //  0006  RAISE	1	K140	K141
      0x8C0C058E,  //  0007  GETMET	R3	R2	K142
      0x5C140200,  //  0008  MOVE	R5	R1
      0x7C0C0400,  //  0009  CALL	R3	2
      0x50100200,  //  000A  LDBOOL	R4	1	0
      0x1C0C0604,  //  000B  EQ	R3	R3	R4
      0x780E0000,  //  000C  JMPF	R3	#000E
      0xB007198F,  //  000D  RAISE	1	K140	K143
      0x80000000,  //  000E  RET	0
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
      0x8C100190,  //  0000  GETMET	R4	R0	K144
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
      0x880C0140,  //  0000  GETMBR	R3	R0	K64
      0x88100147,  //  0001  GETMBR	R4	R0	K71
      0x74120002,  //  0002  JMPT	R4	#0006
      0x4C100000,  //  0003  LDNIL	R4
      0x20100604,  //  0004  NE	R4	R3	R4
      0x78120033,  //  0005  JMPF	R4	#003A
      0xA4120000,  //  0006  IMPORT	R4	K0
      0x4C140000,  //  0007  LDNIL	R5
      0x90028005,  //  0008  SETMBR	R0	K64	R5
      0x50140000,  //  0009  LDBOOL	R5	0	0
      0x8C18090B,  //  000A  GETMET	R6	R4	K11
      0x5C200200,  //  000B  MOVE	R8	R1
      0x7C180400,  //  000C  CALL	R6	2
      0x4C1C0000,  //  000D  LDNIL	R7
      0x1C1C0C07,  //  000E  EQ	R7	R6	R7
      0x781E0004,  //  000F  JMPF	R7	#0015
      0x8C1C0114,  //  0010  GETMET	R7	R0	K20
      0x00269E01,  //  0011  ADD	R9	K79	R1
      0x58280050,  //  0012  LDCONST	R10	K80
      0x7C1C0600,  //  0013  CALL	R7	3
      0x5C180200,  //  0014  MOVE	R6	R1
      0x780A001E,  //  0015  JMPF	R2	#0035
      0x881C0147,  //  0016  GETMBR	R7	R0	K71
      0x781E001C,  //  0017  JMPF	R7	#0035
      0x581C0004,  //  0018  LDCONST	R7	K4
      0x6020000C,  //  0019  GETGBL	R8	G12
      0x88240147,  //  001A  GETMBR	R9	R0	K71
      0x7C200200,  //  001B  CALL	R8	1
      0x14200E08,  //  001C  LT	R8	R7	R8
      0x78220016,  //  001D  JMPF	R8	#0035
      0x88200147,  //  001E  GETMBR	R8	R0	K71
      0x94201007,  //  001F  GETIDX	R8	R8	R7
      0x8C240152,  //  0020  GETMET	R9	R0	K82
      0x5C2C0C00,  //  0021  MOVE	R11	R6
      0x8830113C,  //  0022  GETMBR	R12	R8	K60
      0x8834113F,  //  0023  GETMBR	R13	R8	K63
      0x7C240800,  //  0024  CALL	R9	4
      0x74160001,  //  0025  JMPT	R5	#0028
      0x74260000,  //  0026  JMPT	R9	#0028
      0x50140001,  //  0027  LDBOOL	R5	0	1
      0x50140200,  //  0028  LDBOOL	R5	1	0
      0x78260008,  //  0029  JMPF	R9	#0033
      0x88281191,  //  002A  GETMBR	R10	R8	K145
      0x502C0200,  //  002B  LDBOOL	R11	1	0
      0x1C28140B,  //  002C  EQ	R10	R10	R11
      0x782A0004,  //  002D  JMPF	R10	#0033
      0x88280147,  //  002E  GETMBR	R10	R0	K71
      0x8C281531,  //  002F  GETMET	R10	R10	K49
      0x5C300E00,  //  0030  MOVE	R12	R7
      0x7C280400,  //  0031  CALL	R10	2
      0x70020000,  //  0032  JMP		#0034
      0x001C0F2D,  //  0033  ADD	R7	R7	K45
      0x7001FFE3,  //  0034  JMP		#0019
      0x4C1C0000,  //  0035  LDNIL	R7
      0x201C0607,  //  0036  NE	R7	R3	R7
      0x781E0000,  //  0037  JMPF	R7	#0039
      0x90028006,  //  0038  SETMBR	R0	K64	R6
      0x80040A00,  //  0039  RET	1	R5
      0x50100000,  //  003A  LDBOOL	R4	0	0
      0x80040800,  //  003B  RET	1	R4
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
      0x8C10011A,  //  0000  GETMET	R4	R0	K26
      0x5C180400,  //  0001  MOVE	R6	R2
      0x7C100400,  //  0002  CALL	R4	2
      0x8810012E,  //  0003  GETMBR	R4	R0	K46
      0x4C140000,  //  0004  LDNIL	R5
      0x1C100805,  //  0005  EQ	R4	R4	R5
      0x78120002,  //  0006  JMPF	R4	#000A
      0x60100012,  //  0007  GETGBL	R4	G18
      0x7C100000,  //  0008  CALL	R4	0
      0x90025C04,  //  0009  SETMBR	R0	K46	R4
      0x8810012E,  //  000A  GETMBR	R4	R0	K46
      0x8C10091D,  //  000B  GETMET	R4	R4	K29
      0xB81A9C00,  //  000C  GETNGBL	R6	K78
      0x8C1C0192,  //  000D  GETMET	R7	R0	K146
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
      0xA406F800,  //  0000  IMPORT	R1	K124
      0x8C080393,  //  0001  GETMET	R2	R1	K147
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080394,  //  0003  GETMET	R2	R1	K148
      0x7C080200,  //  0004  CALL	R2	1
      0x80040400,  //  0005  RET	1	R2
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
      0x8C080195,  //  0000  GETMET	R2	R0	K149
      0x5C100200,  //  0001  MOVE	R4	R1
      0x7C080400,  //  0002  CALL	R2	2
      0x280C0504,  //  0003  GE	R3	R2	K4
      0x780E0011,  //  0004  JMPF	R3	#0017
      0x540E7FFE,  //  0005  LDINT	R3	32767
      0x2C0C0403,  //  0006  AND	R3	R2	R3
      0x5412000F,  //  0007  LDINT	R4	16
      0x3C100404,  //  0008  SHR	R4	R2	R4
      0x60140012,  //  0009  GETGBL	R5	G18
      0x7C140000,  //  000A  CALL	R5	0
      0x0418072D,  //  000B  SUB	R6	R3	K45
      0x401A0806,  //  000C  CONNECT	R6	K4	R6
      0x94180206,  //  000D  GETIDX	R6	R1	R6
      0x40180A06,  //  000E  CONNECT	R6	R5	R6
      0x0418092D,  //  000F  SUB	R6	R4	K45
      0x40180606,  //  0010  CONNECT	R6	R3	R6
      0x94180206,  //  0011  GETIDX	R6	R1	R6
      0x40180A06,  //  0012  CONNECT	R6	R5	R6
      0x40180982,  //  0013  CONNECT	R6	R4	K130
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
      0xA4160200,  //  0000  IMPORT	R5	K1
      0x8C180B0C,  //  0001  GETMET	R6	R5	K12
      0x5C200600,  //  0002  MOVE	R8	R3
      0x58240096,  //  0003  LDCONST	R9	K150
      0x7C180600,  //  0004  CALL	R6	3
      0x20180D04,  //  0005  NE	R6	R6	K4
      0x781A0003,  //  0006  JMPF	R6	#000B
      0x8C180197,  //  0007  GETMET	R6	R0	K151
      0x58200098,  //  0008  LDCONST	R8	K152
      0x7C180400,  //  0009  CALL	R6	2
      0x80000C00,  //  000A  RET	0
      0xA802000A,  //  000B  EXBLK	0	#0017
      0x8C180199,  //  000C  GETMET	R6	R0	K153
      0x5C200600,  //  000D  MOVE	R8	R3
      0x7C180400,  //  000E  CALL	R6	2
      0x141C0D04,  //  000F  LT	R7	R6	K4
      0x781E0003,  //  0010  JMPF	R7	#0015
      0x8C1C019A,  //  0011  GETMET	R7	R0	K154
      0x7C1C0200,  //  0012  CALL	R7	1
      0xA8040001,  //  0013  EXBLK	1	1
      0x80000E00,  //  0014  RET	0
      0xA8040001,  //  0015  EXBLK	1	1
      0x70020006,  //  0016  JMP		#001E
      0xAC180002,  //  0017  CATCH	R6	0	2
      0x70020003,  //  0018  JMP		#001D
      0x8C20019A,  //  0019  GETMET	R8	R0	K154
      0x7C200200,  //  001A  CALL	R8	1
      0x80001000,  //  001B  RET	0
      0x70020000,  //  001C  JMP		#001E
      0xB0080000,  //  001D  RAISE	2	R0	R0
      0xB81A0400,  //  001E  GETNGBL	R6	K2
      0x8C180D9B,  //  001F  GETMET	R6	R6	K155
      0x7C180200,  //  0020  CALL	R6	1
      0x80000000,  //  0021  RET	0
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
      0x24180504,  //  0009  GT	R6	R2	K4
      0x781A0031,  //  000A  JMPF	R6	#003D
      0x541A003B,  //  000B  LDINT	R6	60
      0x0C180206,  //  000C  DIV	R6	R1	R6
      0x541E003B,  //  000D  LDINT	R7	60
      0x101C0207,  //  000E  MOD	R7	R1	R7
      0x542200FE,  //  000F  LDINT	R8	255
      0x04201002,  //  0010  SUB	R8	R8	R2
      0xB8260400,  //  0011  GETNGBL	R9	K2
      0x8C24139C,  //  0012  GETMET	R9	R9	K156
      0x5C2C0E00,  //  0013  MOVE	R11	R7
      0x58300004,  //  0014  LDCONST	R12	K4
      0x5436003B,  //  0015  LDINT	R13	60
      0x543A00FE,  //  0016  LDINT	R14	255
      0x5C3C1000,  //  0017  MOVE	R15	R8
      0x7C240C00,  //  0018  CALL	R9	6
      0xB82A0400,  //  0019  GETNGBL	R10	K2
      0x8C28159C,  //  001A  GETMET	R10	R10	K156
      0x5C300E00,  //  001B  MOVE	R12	R7
      0x58340004,  //  001C  LDCONST	R13	K4
      0x543A003B,  //  001D  LDINT	R14	60
      0x5C3C1000,  //  001E  MOVE	R15	R8
      0x544200FE,  //  001F  LDINT	R16	255
      0x7C280C00,  //  0020  CALL	R10	6
      0x1C2C0D04,  //  0021  EQ	R11	R6	K4
      0x782E0002,  //  0022  JMPF	R11	#0026
      0x5C141400,  //  0023  MOVE	R5	R10
      0x5C101000,  //  0024  MOVE	R4	R8
      0x70020016,  //  0025  JMP		#003D
      0x1C2C0D2D,  //  0026  EQ	R11	R6	K45
      0x782E0002,  //  0027  JMPF	R11	#002B
      0x5C0C1200,  //  0028  MOVE	R3	R9
      0x5C101000,  //  0029  MOVE	R4	R8
      0x70020011,  //  002A  JMP		#003D
      0x1C2C0D42,  //  002B  EQ	R11	R6	K66
      0x782E0002,  //  002C  JMPF	R11	#0030
      0x5C0C1000,  //  002D  MOVE	R3	R8
      0x5C101400,  //  002E  MOVE	R4	R10
      0x7002000C,  //  002F  JMP		#003D
      0x1C2C0D50,  //  0030  EQ	R11	R6	K80
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
      0x8C08019D,  //  0000  GETMET	R2	R0	K157
      0x5C100200,  //  0001  MOVE	R4	R1
      0x7C080400,  //  0002  CALL	R2	2
      0x600C0018,  //  0003  GETGBL	R3	G24
      0x5810009E,  //  0004  LDCONST	R4	K158
      0x9414059F,  //  0005  GETIDX	R5	R2	K159
      0x941805A0,  //  0006  GETIDX	R6	R2	K160
      0x941C05A1,  //  0007  GETIDX	R7	R2	K161
      0x942005A2,  //  0008  GETIDX	R8	R2	K162
      0x942405A3,  //  0009  GETIDX	R9	R2	K163
      0x942805A4,  //  000A  GETIDX	R10	R2	K164
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
      0x8C1005A5,  //  0000  GETMET	R4	R2	K165
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
      0x881C05A6,  //  000B  GETMBR	R7	R2	K166
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
    ( &(const binstruction[112]) {  /* code */
      0x1C1803A7,  //  0000  EQ	R6	R1	K167
      0x781A0005,  //  0001  JMPF	R6	#0008
      0x8818011C,  //  0002  GETMBR	R6	R0	K28
      0x781A0001,  //  0003  JMPF	R6	#0006
      0x8C1801A8,  //  0004  GETMET	R6	R0	K168
      0x7C180200,  //  0005  CALL	R6	1
      0x8C1801A9,  //  0006  GETMET	R6	R0	K169
      0x7C180200,  //  0007  CALL	R6	1
      0x1C1803AA,  //  0008  EQ	R6	R1	K170
      0x781A0001,  //  0009  JMPF	R6	#000C
      0x8C1801AB,  //  000A  GETMET	R6	R0	K171
      0x7C180200,  //  000B  CALL	R6	1
      0x50180000,  //  000C  LDBOOL	R6	0	0
      0x501C0000,  //  000D  LDBOOL	R7	0	0
      0x1C2003AC,  //  000E  EQ	R8	R1	K172
      0x78220000,  //  000F  JMPF	R8	#0011
      0x501C0200,  //  0010  LDBOOL	R7	1	0
      0x1C2003AD,  //  0011  EQ	R8	R1	K173
      0x78220006,  //  0012  JMPF	R8	#001A
      0x8C2001AE,  //  0013  GETMET	R8	R0	K174
      0x5C280400,  //  0014  MOVE	R10	R2
      0x5C2C0600,  //  0015  MOVE	R11	R3
      0x5C300800,  //  0016  MOVE	R12	R4
      0x7C200800,  //  0017  CALL	R8	4
      0x80041000,  //  0018  RET	1	R8
      0x7002004F,  //  0019  JMP		#006A
      0x1C2003AF,  //  001A  EQ	R8	R1	K175
      0x78220004,  //  001B  JMPF	R8	#0021
      0x8C2001B0,  //  001C  GETMET	R8	R0	K176
      0x5C280800,  //  001D  MOVE	R10	R4
      0x7C200400,  //  001E  CALL	R8	2
      0x80041000,  //  001F  RET	1	R8
      0x70020048,  //  0020  JMP		#006A
      0x1C200348,  //  0021  EQ	R8	R1	K72
      0x78220007,  //  0022  JMPF	R8	#002B
      0x8C2001B1,  //  0023  GETMET	R8	R0	K177
      0x5C280800,  //  0024  MOVE	R10	R4
      0x602C0017,  //  0025  GETGBL	R11	G23
      0x5C300600,  //  0026  MOVE	R12	R3
      0x7C2C0200,  //  0027  CALL	R11	1
      0x7C200600,  //  0028  CALL	R8	3
      0x80041000,  //  0029  RET	1	R8
      0x7002003E,  //  002A  JMP		#006A
      0x1C20037C,  //  002B  EQ	R8	R1	K124
      0x78220003,  //  002C  JMPF	R8	#0031
      0x8C20017C,  //  002D  GETMET	R8	R0	K124
      0x7C200200,  //  002E  CALL	R8	1
      0x80041000,  //  002F  RET	1	R8
      0x70020038,  //  0030  JMP		#006A
      0x88200149,  //  0031  GETMBR	R8	R0	K73
      0x78220036,  //  0032  JMPF	R8	#006A
      0xA422E000,  //  0033  IMPORT	R8	K112
      0x58240004,  //  0034  LDCONST	R9	K4
      0x6028000C,  //  0035  GETGBL	R10	G12
      0x882C0149,  //  0036  GETMBR	R11	R0	K73
      0x7C280200,  //  0037  CALL	R10	1
      0x1428120A,  //  0038  LT	R10	R9	R10
      0x782A002F,  //  0039  JMPF	R10	#006A
      0x88280149,  //  003A  GETMBR	R10	R0	K73
      0x94281409,  //  003B  GETIDX	R10	R10	R9
      0x8C2C1172,  //  003C  GETMET	R11	R8	K114
      0x5C341400,  //  003D  MOVE	R13	R10
      0x5C380200,  //  003E  MOVE	R14	R1
      0x7C2C0600,  //  003F  CALL	R11	3
      0x60300004,  //  0040  GETGBL	R12	G4
      0x5C341600,  //  0041  MOVE	R13	R11
      0x7C300200,  //  0042  CALL	R12	1
      0x1C301928,  //  0043  EQ	R12	R12	K40
      0x78320022,  //  0044  JMPF	R12	#0068
      0xA8020011,  //  0045  EXBLK	0	#0058
      0x5C301600,  //  0046  MOVE	R12	R11
      0x5C341400,  //  0047  MOVE	R13	R10
      0x5C380400,  //  0048  MOVE	R14	R2
      0x5C3C0600,  //  0049  MOVE	R15	R3
      0x5C400800,  //  004A  MOVE	R16	R4
      0x5C440A00,  //  004B  MOVE	R17	R5
      0x7C300A00,  //  004C  CALL	R12	5
      0x74320001,  //  004D  JMPT	R12	#0050
      0x741A0000,  //  004E  JMPT	R6	#0050
      0x50180001,  //  004F  LDBOOL	R6	0	1
      0x50180200,  //  0050  LDBOOL	R6	1	0
      0x781A0003,  //  0051  JMPF	R6	#0056
      0x5C300E00,  //  0052  MOVE	R12	R7
      0x74320001,  //  0053  JMPT	R12	#0056
      0xA8040001,  //  0054  EXBLK	1	1
      0x70020013,  //  0055  JMP		#006A
      0xA8040001,  //  0056  EXBLK	1	1
      0x7002000F,  //  0057  JMP		#0068
      0xAC300002,  //  0058  CATCH	R12	0	2
      0x7002000C,  //  0059  JMP		#0067
      0x60380001,  //  005A  GETGBL	R14	G1
      0x603C0018,  //  005B  GETGBL	R15	G24
      0x584000B2,  //  005C  LDCONST	R16	K178
      0x5C441800,  //  005D  MOVE	R17	R12
      0x5C481A00,  //  005E  MOVE	R18	R13
      0x7C3C0600,  //  005F  CALL	R15	3
      0x7C380200,  //  0060  CALL	R14	1
      0x88380176,  //  0061  GETMBR	R14	R0	K118
      0x783A0002,  //  0062  JMPF	R14	#0066
      0xA43AEE00,  //  0063  IMPORT	R14	K119
      0x8C3C1DB3,  //  0064  GETMET	R15	R14	K179
      0x7C3C0200,  //  0065  CALL	R15	1
      0x70020000,  //  0066  JMP		#0068
      0xB0080000,  //  0067  RAISE	2	R0	R0
      0x0024132D,  //  0068  ADD	R9	R9	K45
      0x7001FFCA,  //  0069  JMP		#0035
      0x1C2003B4,  //  006A  EQ	R8	R1	K180
      0x78220002,  //  006B  JMPF	R8	#006F
      0xA4236A00,  //  006C  IMPORT	R8	K181
      0x8C241125,  //  006D  GETMET	R9	R8	K37
      0x7C240200,  //  006E  CALL	R9	1
      0x80040C00,  //  006F  RET	1	R6
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
      0x88080116,  //  0000  GETMBR	R2	R0	K22
      0x780A0003,  //  0001  JMPF	R2	#0006
      0x88080116,  //  0002  GETMBR	R2	R0	K22
      0x8C080531,  //  0003  GETMET	R2	R2	K49
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x80000000,  //  0006  RET	0
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
      0x20080557,  //  0003  NE	R2	R2	K87
      0x780A0000,  //  0004  JMPF	R2	#0006
      0xB00653B6,  //  0005  RAISE	1	K41	K182
      0x88080149,  //  0006  GETMBR	R2	R0	K73
      0x780A000B,  //  0007  JMPF	R2	#0014
      0x88080149,  //  0008  GETMBR	R2	R0	K73
      0x8C08050C,  //  0009  GETMET	R2	R2	K12
      0x5C100200,  //  000A  MOVE	R4	R1
      0x7C080400,  //  000B  CALL	R2	2
      0x4C0C0000,  //  000C  LDNIL	R3
      0x1C080403,  //  000D  EQ	R2	R2	R3
      0x780A0003,  //  000E  JMPF	R2	#0013
      0x88080149,  //  000F  GETMBR	R2	R0	K73
      0x8C08051D,  //  0010  GETMET	R2	R2	K29
      0x5C100200,  //  0011  MOVE	R4	R1
      0x7C080400,  //  0012  CALL	R2	2
      0x70020003,  //  0013  JMP		#0018
      0x60080012,  //  0014  GETGBL	R2	G18
      0x7C080000,  //  0015  CALL	R2	0
      0x400C0401,  //  0016  CONNECT	R3	R2	R1
      0x90029202,  //  0017  SETMBR	R0	K73	R2
      0x80000000,  //  0018  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Tasmota
********************************************************************/
be_local_class(Tasmota,
    15,
    NULL,
    be_nested_map(63,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(read_extension_manifest, -1), be_const_closure(class_Tasmota_read_extension_manifest_closure) },
        { be_const_key(add_driver, 58), be_const_closure(class_Tasmota_add_driver_closure) },
        { be_const_key(int, -1), be_const_static_closure(class_Tasmota_int_closure) },
        { be_const_key(find_list_i, -1), be_const_closure(class_Tasmota_find_list_i_closure) },
        { be_const_key(when_network_up, -1), be_const_closure(class_Tasmota_when_network_up_closure) },
        { be_const_key(wire1, 8), be_const_var(8) },
        { be_const_key(compile, -1), be_const_closure(class_Tasmota_compile_closure) },
        { be_const_key(add_fast_loop, 24), be_const_closure(class_Tasmota_add_fast_loop_closure) },
        { be_const_key(is_network_up, -1), be_const_closure(class_Tasmota_is_network_up_closure) },
        { be_const_key(remove_cron, -1), be_const_closure(class_Tasmota_remove_cron_closure) },
        { be_const_key(exec_cmd, 3), be_const_closure(class_Tasmota_exec_cmd_closure) },
        { be_const_key(set_light, 12), be_const_closure(class_Tasmota_set_light_closure) },
        { be_const_key(time_str, 57), be_const_closure(class_Tasmota_time_str_closure) },
        { be_const_key(remove_timer, 20), be_const_closure(class_Tasmota_remove_timer_closure) },
        { be_const_key(hs2rgb, 41), be_const_closure(class_Tasmota_hs2rgb_closure) },
        { be_const_key(cmd, -1), be_const_closure(class_Tasmota_cmd_closure) },
        { be_const_key(exec_tele, -1), be_const_closure(class_Tasmota_exec_tele_closure) },
        { be_const_key(remove_rule, 30), be_const_closure(class_Tasmota_remove_rule_closure) },
        { be_const_key(wd, -1), be_const_var(13) },
        { be_const_key(add_cmd, -1), be_const_closure(class_Tasmota_add_cmd_closure) },
        { be_const_key(find_op, 9), be_const_closure(class_Tasmota_find_op_closure) },
        { be_const_key(add_cron, 37), be_const_closure(class_Tasmota_add_cron_closure) },
        { be_const_key(_defer, -1), be_const_var(3) },
        { be_const_key(global, 16), be_const_var(11) },
        { be_const_key(settings, 54), be_const_var(12) },
        { be_const_key(run_network_up, -1), be_const_closure(class_Tasmota_run_network_up_closure) },
        { be_const_key(_rules, -1), be_const_var(1) },
        { be_const_key(gen_cb, 14), be_const_closure(class_Tasmota_gen_cb_closure) },
        { be_const_key(wire_scan, -1), be_const_closure(class_Tasmota_wire_scan_closure) },
        { be_const_key(add_extension, -1), be_const_closure(class_Tasmota_add_extension_closure) },
        { be_const_key(gc, -1), be_const_closure(class_Tasmota_gc_closure) },
        { be_const_key(wire2, -1), be_const_var(9) },
        { be_const_key(run_deferred, 48), be_const_closure(class_Tasmota_run_deferred_closure) },
        { be_const_key(init, -1), be_const_closure(class_Tasmota_init_closure) },
        { be_const_key(defer, -1), be_const_closure(class_Tasmota_defer_closure) },
        { be_const_key(set_timer, -1), be_const_closure(class_Tasmota_set_timer_closure) },
        { be_const_key(next_cron, -1), be_const_closure(class_Tasmota_next_cron_closure) },
        { be_const_key(exec_rules, -1), be_const_closure(class_Tasmota_exec_rules_closure) },
        { be_const_key(unload_extension, -1), be_const_closure(class_Tasmota_unload_extension_closure) },
        { be_const_key(add_rule_once, 50), be_const_closure(class_Tasmota_add_rule_once_closure) },
        { be_const_key(cmd_res, -1), be_const_var(10) },
        { be_const_key(_ccmd, 29), be_const_var(5) },
        { be_const_key(add_rule, -1), be_const_closure(class_Tasmota_add_rule_closure) },
        { be_const_key(remove_driver, 42), be_const_closure(class_Tasmota_remove_driver_closure) },
        { be_const_key(_fl, -1), be_const_var(0) },
        { be_const_key(check_not_method, -1), be_const_closure(class_Tasmota_check_not_method_closure) },
        { be_const_key(_drivers, -1), be_const_var(6) },
        { be_const_key(find_key_i, 39), be_const_closure(class_Tasmota_find_key_i_closure) },
        { be_const_key(get_light, -1), be_const_closure(class_Tasmota_get_light_closure) },
        { be_const_key(load, 35), be_const_closure(class_Tasmota_load_closure) },
        { be_const_key(fast_loop, 53), be_const_closure(class_Tasmota_fast_loop_closure) },
        { be_const_key(_debug_present, -1), be_const_var(14) },
        { be_const_key(run_timers, 26), be_const_closure(class_Tasmota_run_timers_closure) },
        { be_const_key(urlfetch, -1), be_const_closure(class_Tasmota_urlfetch_closure) },
        { be_const_key(_timers, -1), be_const_var(2) },
        { be_const_key(_wnu, 18), be_const_var(7) },
        { be_const_key(urlfetch_cmd, -1), be_const_closure(class_Tasmota_urlfetch_cmd_closure) },
        { be_const_key(remove_fast_loop, -1), be_const_closure(class_Tasmota_remove_fast_loop_closure) },
        { be_const_key(_crons, -1), be_const_var(4) },
        { be_const_key(try_rule, -1), be_const_closure(class_Tasmota_try_rule_closure) },
        { be_const_key(event, -1), be_const_closure(class_Tasmota_event_closure) },
        { be_const_key(remove_cmd, -1), be_const_closure(class_Tasmota_remove_cmd_closure) },
        { be_const_key(run_cron, 1), be_const_closure(class_Tasmota_run_cron_closure) },
    })),
    (bstring*) &be_const_str_Tasmota
);
/********************************************************************/
/* End of solidification */
