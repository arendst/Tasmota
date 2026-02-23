/* Solidification of Matter_zz_Device.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
extern const bclass be_class_Matter_Device;
// compact class 'Matter_Device' ktab size: 210, total: 449 (saved 1912 bytes)
static const bvalue be_ktab_class_Matter_Device[210] = {
  /* K0   */  be_nested_str_weak(sessions),
  /* K1   */  be_nested_str_weak(count_active_fabrics),
  /* K2   */  be_const_int(0),
  /* K3   */  be_nested_str_weak(plugins_persist),
  /* K4   */  be_nested_str_weak(save_param),
  /* K5   */  be_nested_str_weak(json),
  /* K6   */  be_nested_str_weak(tasmota),
  /* K7   */  be_nested_str_weak(read_sensors),
  /* K8   */  be_nested_str_weak(loglevel),
  /* K9   */  be_const_int(3),
  /* K10  */  be_nested_str_weak(log),
  /* K11  */  be_nested_str_weak(MTR_X3A_X20read_sensors_X3A_X20),
  /* K12  */  be_nested_str_weak(load),
  /* K13  */  be_nested_str_weak(plugins),
  /* K14  */  be_nested_str_weak(parse_sensors),
  /* K15  */  be_const_int(1),
  /* K16  */  be_nested_str_weak(MTR_X3A_X20unable_X20to_X20parse_X20read_sensors_X3A_X20),
  /* K17  */  be_nested_str_weak(endpoint),
  /* K18  */  be_nested_str_weak(invoke_request),
  /* K19  */  be_nested_str_weak(status),
  /* K20  */  be_nested_str_weak(matter),
  /* K21  */  be_nested_str_weak(UNSUPPORTED_ENDPOINT),
  /* K22  */  be_nested_str_weak(probe_sensor_time),
  /* K23  */  be_nested_str_weak(probe_sensor_timestamp),
  /* K24  */  be_nested_str_weak(jitter),
  /* K25  */  be_nested_str_weak(attribute_updated),
  /* K26  */  be_nested_str_weak(AGGREGATOR_ENDPOINT),
  /* K27  */  be_nested_str_weak(get_name),
  /* K28  */  be_nested_str_weak(plugins_config_remotes),
  /* K29  */  be_nested_str_weak(find),
  /* K30  */  be_const_class(be_class_Matter_Device),
  /* K31  */  be_nested_str_weak(keys),
  /* K32  */  be_nested_str_weak(push),
  /* K33  */  be_nested_str_weak(stop_iteration),
  /* K34  */  be_nested_str_weak(tick),
  /* K35  */  be_nested_str_weak(message_handler),
  /* K36  */  be_nested_str_weak(every_50ms),
  /* K37  */  be_nested_str_weak(),
  /* K38  */  be_nested_str_weak(k2l),
  /* K39  */  be_nested_str_weak(type),
  /* K40  */  be_nested_str_weak(_X20_X25s_X3A_X25s),
  /* K41  */  be_nested_str_weak(button_handler),
  /* K42  */  be_const_int(2),
  /* K43  */  be_nested_str_weak(crypto),
  /* K44  */  be_nested_str_weak(get_option),
  /* K45  */  be_nested_str_weak(MATTER_OPTION),
  /* K46  */  be_nested_str_weak(ui),
  /* K47  */  be_nested_str_weak(UI),
  /* K48  */  be_nested_str_weak(profiler),
  /* K49  */  be_nested_str_weak(Profiler),
  /* K50  */  be_nested_str_weak(next_ep),
  /* K51  */  be_nested_str_weak(EP),
  /* K52  */  be_nested_str_weak(ipv4only),
  /* K53  */  be_nested_str_weak(disable_bridge_mode),
  /* K54  */  be_nested_str_weak(commissioning),
  /* K55  */  be_nested_str_weak(Commissioning),
  /* K56  */  be_nested_str_weak(load_param),
  /* K57  */  be_nested_str_weak(Session_Store),
  /* K58  */  be_nested_str_weak(load_fabrics),
  /* K59  */  be_nested_str_weak(MessageHandler),
  /* K60  */  be_nested_str_weak(events),
  /* K61  */  be_nested_str_weak(EventHandler),
  /* K62  */  be_nested_str_weak(zigbee),
  /* K63  */  be_nested_str_weak(init_zigbee),
  /* K64  */  be_nested_str_weak(when_network_up),
  /* K65  */  be_nested_str_weak(init_basic_commissioning),
  /* K66  */  be_nested_str_weak(add_driver),
  /* K67  */  be_nested_str_weak(register_commands),
  /* K68  */  be_nested_str_weak(remove),
  /* K69  */  be_nested_str_weak(MTR_X3A_X20removing_X20fabric_X20),
  /* K70  */  be_nested_str_weak(get_fabric_id),
  /* K71  */  be_nested_str_weak(copy),
  /* K72  */  be_nested_str_weak(reverse),
  /* K73  */  be_nested_str_weak(tohex),
  /* K74  */  be_nested_str_weak(im),
  /* K75  */  be_nested_str_weak(subs_shop),
  /* K76  */  be_nested_str_weak(remove_by_fabric),
  /* K77  */  be_nested_str_weak(mdns_remove_op_discovery),
  /* K78  */  be_nested_str_weak(remove_fabric),
  /* K79  */  be_nested_str_weak(save_fabrics),
  /* K80  */  be_nested_str_weak(plugins_config),
  /* K81  */  be_nested_str_weak(contains),
  /* K82  */  be_nested_str_weak(MTR_X3A_X20Cannot_X20remove_X20an_X20enpoint_X20not_X20configured_X3A_X20),
  /* K83  */  be_nested_str_weak(MTR_X3A_X20deleting_X20endpoint_X20_X3D_X20_X25i),
  /* K84  */  be_nested_str_weak(get_endpoint),
  /* K85  */  be_nested_str_weak(clean_remotes),
  /* K86  */  be_nested_str_weak(signal_endpoints_changed),
  /* K87  */  be_nested_str_weak(read_sensors_scheduler),
  /* K88  */  be_nested_str_weak(every_250ms),
  /* K89  */  be_nested_str_weak(autoconf_device),
  /* K90  */  be_nested_str_weak(_start_udp),
  /* K91  */  be_nested_str_weak(UDP_PORT),
  /* K92  */  be_nested_str_weak(start_mdns_announce_hostnames),
  /* K93  */  be_nested_str_weak(add_cmd),
  /* K94  */  be_nested_str_weak(MtrJoin),
  /* K95  */  be_nested_str_weak(MtrUpdate),
  /* K96  */  be_nested_str_weak(MtrInfo),
  /* K97  */  be_nested_str_weak(MtrInfo_one),
  /* K98  */  be_nested_str_weak(int),
  /* K99  */  be_nested_str_weak(find_plugin_by_friendly_name),
  /* K100 */  be_nested_str_weak(resp_cmnd_done),
  /* K101 */  be_nested_str_weak(msg_received),
  /* K102 */  be_nested_str_weak(FILENAME),
  /* K103 */  be_nested_str_weak(read),
  /* K104 */  be_nested_str_weak(close),
  /* K105 */  be_nested_str_weak(root_discriminator),
  /* K106 */  be_nested_str_weak(distinguish),
  /* K107 */  be_nested_str_weak(root_passcode),
  /* K108 */  be_nested_str_weak(passcode),
  /* K109 */  be_nested_str_weak(nextep),
  /* K110 */  be_nested_str_weak(config),
  /* K111 */  be_nested_str_weak(debug),
  /* K112 */  be_nested_str_weak(MTR_X3A_X20Load_config_X20_X3D_X20_X25s),
  /* K113 */  be_nested_str_weak(adjust_next_ep),
  /* K114 */  be_nested_str_weak(check_config_ep),
  /* K115 */  be_nested_str_weak(remotes),
  /* K116 */  be_nested_str_weak(MTR_X3A_X20load_remotes_X20_X3D_X20),
  /* K117 */  be_nested_str_weak(io_error),
  /* K118 */  be_nested_str_weak(MTR_X3A_X20load_param_X20Exception_X3A),
  /* K119 */  be_nested_str_weak(_X7C),
  /* K120 */  be_nested_str_weak(random),
  /* K121 */  be_nested_str_weak(get),
  /* K122 */  be_nested_str_weak(generate_random_passcode),
  /* K123 */  be_nested_str_weak(http_remotes),
  /* K124 */  be_nested_str_weak(get_timeout),
  /* K125 */  be_nested_str_weak(set_timeout),
  /* K126 */  be_nested_str_weak(HTTP_remote),
  /* K127 */  be_nested_str_weak(set_info),
  /* K128 */  be_nested_str_weak(start_root_basic_commissioning),
  /* K129 */  be_nested_str_weak(stop_basic_commissioning),
  /* K130 */  be_nested_str_weak(autoconf),
  /* K131 */  be_nested_str_weak(Autoconf),
  /* K132 */  be_nested_str_weak(autoconf_device_map),
  /* K133 */  be_nested_str_weak(MTR_X3A_X20autoconfig_X20_X3D_X20),
  /* K134 */  be_nested_str_weak(instantiate_plugins_from_config),
  /* K135 */  be_nested_str_weak(introspect),
  /* K136 */  be_nested_str_weak(module),
  /* K137 */  be_nested_str_weak(matter_zigbee),
  /* K138 */  be_nested_str_weak(udp_server),
  /* K139 */  be_nested_str_weak(MTR_X3A_X20Starting_X20UDP_X20server_X20on_X20port_X3A_X20),
  /* K140 */  be_nested_str_weak(UDPServer),
  /* K141 */  be_nested_str_weak(start),
  /* K142 */  be_nested_str_weak(mdns_remove_op_discovery_all_fabrics),
  /* K143 */  be_nested_str_weak(Path),
  /* K144 */  be_nested_str_weak(cluster),
  /* K145 */  be_nested_str_weak(attribute),
  /* K146 */  be_nested_str_weak(attribute_updated_ctx),
  /* K147 */  be_nested_str_weak(received_ack),
  /* K148 */  be_nested_str_weak(update_remotes_info),
  /* K149 */  be_nested_str_weak(_X7B_X22distinguish_X22_X3A_X25i_X2C_X22passcode_X22_X3A_X25i_X2C_X22ipv4only_X22_X3A_X25s_X2C_X22disable_bridge_mode_X22_X3A_X25s_X2C_X22nextep_X22_X3A_X25i),
  /* K150 */  be_nested_str_weak(true),
  /* K151 */  be_nested_str_weak(false),
  /* K152 */  be_nested_str_weak(_X2C_X22debug_X22_X3Atrue),
  /* K153 */  be_nested_str_weak(_X2C_X0A_X22config_X22_X3A),
  /* K154 */  be_nested_str_weak(dump),
  /* K155 */  be_nested_str_weak(_X2C_X0A_X22remotes_X22_X3A),
  /* K156 */  be_nested_str_weak(_X7D),
  /* K157 */  be_nested_str_weak(w),
  /* K158 */  be_nested_str_weak(write),
  /* K159 */  be_nested_str_weak(MTR_X3A_X20_X3DSaved_X20_X20_X20_X20_X20parameters_X25s),
  /* K160 */  be_nested_str_weak(_X20and_X20configuration),
  /* K161 */  be_nested_str_weak(MTR_X3A_X20Session_Store_X3A_X3Asave_X20Exception_X3A),
  /* K162 */  be_nested_str_weak(get_info),
  /* K163 */  be_nested_str_weak(every_second),
  /* K164 */  be_nested_str_weak(remove_driver),
  /* K165 */  be_nested_str_weak(stop),
  /* K166 */  be_nested_str_weak(plugins_classes),
  /* K167 */  be_nested_str_weak(DISPLAY_NAME),
  /* K168 */  be_nested_str_weak(find_plugin_by_endpoint),
  /* K169 */  be_nested_str_weak(state_json),
  /* K170 */  be_nested_str_weak(_X7B_X22MtrInfo_X22_X3A_X25s_X7D),
  /* K171 */  be_nested_str_weak(publish_result),
  /* K172 */  be_nested_str_weak(MTR_X3A_X20unknown_X20class_X20name_X20_X27),
  /* K173 */  be_nested_str_weak(_X27_X20skipping),
  /* K174 */  be_nested_str_weak(MTR_X3A_X20adding_X20endpoint_X20_X3D_X20_X25i_X20type_X3A_X25s_X25s),
  /* K175 */  be_nested_str_weak(conf_to_log),
  /* K176 */  be_nested_str_weak(contains_cluster),
  /* K177 */  be_nested_str_weak(UNSUPPORTED_CLUSTER),
  /* K178 */  be_nested_str_weak(contains_attribute),
  /* K179 */  be_nested_str_weak(UNSUPPORTED_ATTRIBUTE),
  /* K180 */  be_nested_str_weak(send_UDP),
  /* K181 */  be_nested_str_weak(http_remote),
  /* K182 */  be_nested_str_weak(MTR_X3A_X20remove_X20unused_X20remote_X3A_X20),
  /* K183 */  be_nested_str_weak(addr),
  /* K184 */  be_nested_str_weak(resp_cmnd_str),
  /* K185 */  be_nested_str_weak(Invalid_X20JSON),
  /* K186 */  be_nested_str_weak(find_key_i),
  /* K187 */  be_nested_str_weak(Ep),
  /* K188 */  be_nested_str_weak(Name),
  /* K189 */  be_nested_str_weak(Invalid_X20_X27Ep_X27_X20attribute),
  /* K190 */  be_nested_str_weak(Invalid_X20Device),
  /* K191 */  be_nested_str_weak(VIRTUAL),
  /* K192 */  be_nested_str_weak(Device_X20is_X20not_X20virtual),
  /* K193 */  be_nested_str_weak(consolidate_update_commands),
  /* K194 */  be_nested_str_weak(find_list_i),
  /* K195 */  be_nested_str_weak(Invalid_X20attribute_X20_X27_X25s_X27),
  /* K196 */  be_nested_str_weak(update_virtual),
  /* K197 */  be_nested_str_weak(_X7B_X22_X25s_X22_X3A_X25s_X7D),
  /* K198 */  be_nested_str_weak(resp_cmnd),
  /* K199 */  be_nested_str_weak(Missing_X20_X27Device_X27_X20attribute),
  /* K200 */  be_nested_str_weak(time_reached),
  /* K201 */  be_nested_str_weak(_trigger_read_sensors),
  /* K202 */  be_nested_str_weak(millis),
  /* K203 */  be_nested_str_weak(Matter_Zigbee_Mapper),
  /* K204 */  be_nested_str_weak(PathGenerator),
  /* K205 */  be_nested_str_weak(is_direct),
  /* K206 */  be_nested_str_weak(next_attribute),
  /* K207 */  be_nested_str_weak(get_pi),
  /* K208 */  be_nested_str_weak(MTR_X3A_X20endpoint_X20_X25s_X20collides_X20wit_X20aggregator_X2C_X20relocating_X20to_X20_X25s),
  /* K209 */  be_nested_str_weak(is_zigbee_present),
};


extern const bclass be_class_Matter_Device;

/********************************************************************
** Solidified function: event_fabrics_saved
********************************************************************/
be_local_closure(class_Matter_Device_event_fabrics_saved,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(event_fabrics_saved),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x7C040200,  //  0002  CALL	R1	1
      0x24040302,  //  0003  GT	R1	R1	K2
      0x78060005,  //  0004  JMPF	R1	#000B
      0x88040103,  //  0005  GETMBR	R1	R0	K3
      0x74060003,  //  0006  JMPT	R1	#000B
      0x50040200,  //  0007  LDBOOL	R1	1	0
      0x90020601,  //  0008  SETMBR	R0	K3	R1
      0x8C040104,  //  0009  GETMET	R1	R0	K4
      0x7C040200,  //  000A  CALL	R1	1
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _trigger_read_sensors
********************************************************************/
be_local_closure(class_Matter_Device__trigger_read_sensors,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(_trigger_read_sensors),
    &be_const_str_solidified,
    ( &(const binstruction[48]) {  /* code */
      0xA4060A00,  //  0000  IMPORT	R1	K5
      0xB80A0C00,  //  0001  GETNGBL	R2	K6
      0x8C080507,  //  0002  GETMET	R2	R2	K7
      0x7C080200,  //  0003  CALL	R2	1
      0xB80E0C00,  //  0004  GETNGBL	R3	K6
      0x8C0C0708,  //  0005  GETMET	R3	R3	K8
      0x58140009,  //  0006  LDCONST	R5	K9
      0x7C0C0400,  //  0007  CALL	R3	2
      0x780E0006,  //  0008  JMPF	R3	#0010
      0xB80E1400,  //  0009  GETNGBL	R3	K10
      0x60100008,  //  000A  GETGBL	R4	G8
      0x5C140400,  //  000B  MOVE	R5	R2
      0x7C100200,  //  000C  CALL	R4	1
      0x00121604,  //  000D  ADD	R4	K11	R4
      0x58140009,  //  000E  LDCONST	R5	K9
      0x7C0C0400,  //  000F  CALL	R3	2
      0x4C0C0000,  //  0010  LDNIL	R3
      0x1C0C0403,  //  0011  EQ	R3	R2	R3
      0x780E0000,  //  0012  JMPF	R3	#0014
      0x80000600,  //  0013  RET	0
      0x8C0C030C,  //  0014  GETMET	R3	R1	K12
      0x5C140400,  //  0015  MOVE	R5	R2
      0x7C0C0400,  //  0016  CALL	R3	2
      0x4C100000,  //  0017  LDNIL	R4
      0x20100604,  //  0018  NE	R4	R3	R4
      0x7812000D,  //  0019  JMPF	R4	#0028
      0x58100002,  //  001A  LDCONST	R4	K2
      0x6014000C,  //  001B  GETGBL	R5	G12
      0x8818010D,  //  001C  GETMBR	R6	R0	K13
      0x7C140200,  //  001D  CALL	R5	1
      0x14140805,  //  001E  LT	R5	R4	R5
      0x78160006,  //  001F  JMPF	R5	#0027
      0x8814010D,  //  0020  GETMBR	R5	R0	K13
      0x94140A04,  //  0021  GETIDX	R5	R5	R4
      0x8C140B0E,  //  0022  GETMET	R5	R5	K14
      0x5C1C0600,  //  0023  MOVE	R7	R3
      0x7C140400,  //  0024  CALL	R5	2
      0x0010090F,  //  0025  ADD	R4	R4	K15
      0x7001FFF3,  //  0026  JMP		#001B
      0x70020006,  //  0027  JMP		#002F
      0xB8121400,  //  0028  GETNGBL	R4	K10
      0x60140008,  //  0029  GETGBL	R5	G8
      0x5C180400,  //  002A  MOVE	R6	R2
      0x7C140200,  //  002B  CALL	R5	1
      0x00162005,  //  002C  ADD	R5	K16	R5
      0x58180009,  //  002D  LDCONST	R6	K9
      0x7C100400,  //  002E  CALL	R4	2
      0x80000000,  //  002F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: invoke_request
********************************************************************/
be_local_closure(class_Matter_Device_invoke_request,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(invoke_request),
    &be_const_str_solidified,
    ( &(const binstruction[24]) {  /* code */
      0x58100002,  //  0000  LDCONST	R4	K2
      0x88140711,  //  0001  GETMBR	R5	R3	K17
      0x6018000C,  //  0002  GETGBL	R6	G12
      0x881C010D,  //  0003  GETMBR	R7	R0	K13
      0x7C180200,  //  0004  CALL	R6	1
      0x14180806,  //  0005  LT	R6	R4	R6
      0x781A000C,  //  0006  JMPF	R6	#0014
      0x8818010D,  //  0007  GETMBR	R6	R0	K13
      0x94180C04,  //  0008  GETIDX	R6	R6	R4
      0x881C0D11,  //  0009  GETMBR	R7	R6	K17
      0x1C1C0E05,  //  000A  EQ	R7	R7	R5
      0x781E0005,  //  000B  JMPF	R7	#0012
      0x8C1C0D12,  //  000C  GETMET	R7	R6	K18
      0x5C240200,  //  000D  MOVE	R9	R1
      0x5C280400,  //  000E  MOVE	R10	R2
      0x5C2C0600,  //  000F  MOVE	R11	R3
      0x7C1C0800,  //  0010  CALL	R7	4
      0x80040E00,  //  0011  RET	1	R7
      0x0010090F,  //  0012  ADD	R4	R4	K15
      0x7001FFED,  //  0013  JMP		#0002
      0xB81A2800,  //  0014  GETNGBL	R6	K20
      0x88180D15,  //  0015  GETMBR	R6	R6	K21
      0x900E2606,  //  0016  SETMBR	R3	K19	R6
      0x80000000,  //  0017  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: add_read_sensors_schedule
********************************************************************/
be_local_closure(class_Matter_Device_add_read_sensors_schedule,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(add_read_sensors_schedule),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x88080116,  //  0000  GETMBR	R2	R0	K22
      0x4C0C0000,  //  0001  LDNIL	R3
      0x1C080403,  //  0002  EQ	R2	R2	R3
      0x740A0002,  //  0003  JMPT	R2	#0007
      0x88080116,  //  0004  GETMBR	R2	R0	K22
      0x24080401,  //  0005  GT	R2	R2	R1
      0x780A0005,  //  0006  JMPF	R2	#000D
      0x90022C01,  //  0007  SETMBR	R0	K22	R1
      0xB80A2800,  //  0008  GETNGBL	R2	K20
      0x8C080518,  //  0009  GETMET	R2	R2	K24
      0x5C100200,  //  000A  MOVE	R4	R1
      0x7C080400,  //  000B  CALL	R2	2
      0x90022E02,  //  000C  SETMBR	R0	K23	R2
      0x80000000,  //  000D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: signal_endpoints_changed
********************************************************************/
be_local_closure(class_Matter_Device_signal_endpoints_changed,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(signal_endpoints_changed),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x8C040119,  //  0000  GETMET	R1	R0	K25
      0x580C0002,  //  0001  LDCONST	R3	K2
      0x5412001C,  //  0002  LDINT	R4	29
      0x58140009,  //  0003  LDCONST	R5	K9
      0x50180000,  //  0004  LDBOOL	R6	0	0
      0x7C040A00,  //  0005  CALL	R1	5
      0x8C040119,  //  0006  GETMET	R1	R0	K25
      0xB80E2800,  //  0007  GETNGBL	R3	K20
      0x880C071A,  //  0008  GETMBR	R3	R3	K26
      0x5412001C,  //  0009  LDINT	R4	29
      0x58140009,  //  000A  LDCONST	R5	K9
      0x50180000,  //  000B  LDBOOL	R6	0	0
      0x7C040A00,  //  000C  CALL	R1	5
      0x80000000,  //  000D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: find_plugin_by_friendly_name
********************************************************************/
be_local_closure(class_Matter_Device_find_plugin_by_friendly_name,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(find_plugin_by_friendly_name),
    &be_const_str_solidified,
    ( &(const binstruction[35]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x740A0004,  //  0002  JMPT	R2	#0008
      0x6008000C,  //  0003  GETGBL	R2	G12
      0x5C0C0200,  //  0004  MOVE	R3	R1
      0x7C080200,  //  0005  CALL	R2	1
      0x1C080502,  //  0006  EQ	R2	R2	K2
      0x780A0001,  //  0007  JMPF	R2	#000A
      0x4C080000,  //  0008  LDNIL	R2
      0x80040400,  //  0009  RET	1	R2
      0x58080002,  //  000A  LDCONST	R2	K2
      0x600C000C,  //  000B  GETGBL	R3	G12
      0x8810010D,  //  000C  GETMBR	R4	R0	K13
      0x7C0C0200,  //  000D  CALL	R3	1
      0x140C0403,  //  000E  LT	R3	R2	R3
      0x780E0010,  //  000F  JMPF	R3	#0021
      0x880C010D,  //  0010  GETMBR	R3	R0	K13
      0x940C0602,  //  0011  GETIDX	R3	R3	R2
      0x8C10071B,  //  0012  GETMET	R4	R3	K27
      0x7C100200,  //  0013  CALL	R4	1
      0x4C140000,  //  0014  LDNIL	R5
      0x20140805,  //  0015  NE	R5	R4	R5
      0x78160007,  //  0016  JMPF	R5	#001F
      0x6014000C,  //  0017  GETGBL	R5	G12
      0x5C180800,  //  0018  MOVE	R6	R4
      0x7C140200,  //  0019  CALL	R5	1
      0x24140B02,  //  001A  GT	R5	R5	K2
      0x78160002,  //  001B  JMPF	R5	#001F
      0x1C140801,  //  001C  EQ	R5	R4	R1
      0x78160000,  //  001D  JMPF	R5	#001F
      0x80040600,  //  001E  RET	1	R3
      0x0008050F,  //  001F  ADD	R2	R2	K15
      0x7001FFE9,  //  0020  JMP		#000B
      0x4C0C0000,  //  0021  LDNIL	R3
      0x80040600,  //  0022  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_plugin_remote_info
********************************************************************/
be_local_closure(class_Matter_Device_get_plugin_remote_info,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(get_plugin_remote_info),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x8808011C,  //  0000  GETMBR	R2	R0	K28
      0x8C08051D,  //  0001  GETMET	R2	R2	K29
      0x5C100200,  //  0002  MOVE	R4	R1
      0x60140013,  //  0003  GETGBL	R5	G19
      0x7C140000,  //  0004  CALL	R5	0
      0x7C080600,  //  0005  CALL	R2	3
      0x80040400,  //  0006  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: k2l_num
********************************************************************/
be_local_closure(class_Matter_Device_k2l_num,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    12,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(k2l_num),
    &be_const_str_solidified,
    ( &(const binstruction[52]) {  /* code */
      0x5804001E,  //  0000  LDCONST	R1	K30
      0x60080012,  //  0001  GETGBL	R2	G18
      0x7C080000,  //  0002  CALL	R2	0
      0x4C0C0000,  //  0003  LDNIL	R3
      0x1C0C0003,  //  0004  EQ	R3	R0	R3
      0x780E0000,  //  0005  JMPF	R3	#0007
      0x80040400,  //  0006  RET	1	R2
      0x600C0010,  //  0007  GETGBL	R3	G16
      0x8C10011F,  //  0008  GETMET	R4	R0	K31
      0x7C100200,  //  0009  CALL	R4	1
      0x7C0C0200,  //  000A  CALL	R3	1
      0xA8020007,  //  000B  EXBLK	0	#0014
      0x5C100600,  //  000C  MOVE	R4	R3
      0x7C100000,  //  000D  CALL	R4	0
      0x8C140520,  //  000E  GETMET	R5	R2	K32
      0x601C0009,  //  000F  GETGBL	R7	G9
      0x5C200800,  //  0010  MOVE	R8	R4
      0x7C1C0200,  //  0011  CALL	R7	1
      0x7C140400,  //  0012  CALL	R5	2
      0x7001FFF7,  //  0013  JMP		#000C
      0x580C0021,  //  0014  LDCONST	R3	K33
      0xAC0C0200,  //  0015  CATCH	R3	1	0
      0xB0080000,  //  0016  RAISE	2	R0	R0
      0x600C0010,  //  0017  GETGBL	R3	G16
      0x6010000C,  //  0018  GETGBL	R4	G12
      0x5C140400,  //  0019  MOVE	R5	R2
      0x7C100200,  //  001A  CALL	R4	1
      0x0410090F,  //  001B  SUB	R4	R4	K15
      0x40121E04,  //  001C  CONNECT	R4	K15	R4
      0x7C0C0200,  //  001D  CALL	R3	1
      0xA8020010,  //  001E  EXBLK	0	#0030
      0x5C100600,  //  001F  MOVE	R4	R3
      0x7C100000,  //  0020  CALL	R4	0
      0x94140404,  //  0021  GETIDX	R5	R2	R4
      0x5C180800,  //  0022  MOVE	R6	R4
      0x241C0D02,  //  0023  GT	R7	R6	K2
      0x781E0008,  //  0024  JMPF	R7	#002E
      0x041C0D0F,  //  0025  SUB	R7	R6	K15
      0x941C0407,  //  0026  GETIDX	R7	R2	R7
      0x241C0E05,  //  0027  GT	R7	R7	R5
      0x781E0004,  //  0028  JMPF	R7	#002E
      0x041C0D0F,  //  0029  SUB	R7	R6	K15
      0x941C0407,  //  002A  GETIDX	R7	R2	R7
      0x98080C07,  //  002B  SETIDX	R2	R6	R7
      0x04180D0F,  //  002C  SUB	R6	R6	K15
      0x7001FFF4,  //  002D  JMP		#0023
      0x98080C05,  //  002E  SETIDX	R2	R6	R5
      0x7001FFEE,  //  002F  JMP		#001F
      0x580C0021,  //  0030  LDCONST	R3	K33
      0xAC0C0200,  //  0031  CATCH	R3	1	0
      0xB0080000,  //  0032  RAISE	2	R0	R0
      0x80040400,  //  0033  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: every_50ms
********************************************************************/
be_local_closure(class_Matter_Device_every_50ms,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(every_50ms),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x88040122,  //  0000  GETMBR	R1	R0	K34
      0x0004030F,  //  0001  ADD	R1	R1	K15
      0x90024401,  //  0002  SETMBR	R0	K34	R1
      0x88040123,  //  0003  GETMBR	R1	R0	K35
      0x8C040324,  //  0004  GETMET	R1	R1	K36
      0x7C040200,  //  0005  CALL	R1	1
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: conf_to_log
********************************************************************/
be_local_closure(class_Matter_Device_conf_to_log,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    12,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(conf_to_log),
    &be_const_str_solidified,
    ( &(const binstruction[24]) {  /* code */
      0x5804001E,  //  0000  LDCONST	R1	K30
      0x58080025,  //  0001  LDCONST	R2	K37
      0x600C0010,  //  0002  GETGBL	R3	G16
      0x8C100326,  //  0003  GETMET	R4	R1	K38
      0x5C180000,  //  0004  MOVE	R6	R0
      0x7C100400,  //  0005  CALL	R4	2
      0x7C0C0200,  //  0006  CALL	R3	1
      0xA802000B,  //  0007  EXBLK	0	#0014
      0x5C100600,  //  0008  MOVE	R4	R3
      0x7C100000,  //  0009  CALL	R4	0
      0x1C140927,  //  000A  EQ	R5	R4	K39
      0x78160000,  //  000B  JMPF	R5	#000D
      0x7001FFFA,  //  000C  JMP		#0008
      0x60140018,  //  000D  GETGBL	R5	G24
      0x58180028,  //  000E  LDCONST	R6	K40
      0x5C1C0800,  //  000F  MOVE	R7	R4
      0x94200004,  //  0010  GETIDX	R8	R0	R4
      0x7C140600,  //  0011  CALL	R5	3
      0x00080405,  //  0012  ADD	R2	R2	R5
      0x7001FFF3,  //  0013  JMP		#0008
      0x580C0021,  //  0014  LDCONST	R3	K33
      0xAC0C0200,  //  0015  CATCH	R3	1	0
      0xB0080000,  //  0016  RAISE	2	R0	R0
      0x80040400,  //  0017  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: button_multi_pressed
********************************************************************/
be_local_closure(class_Matter_Device_button_multi_pressed,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(button_multi_pressed),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x540E0007,  //  0000  LDINT	R3	8
      0x3C0C0403,  //  0001  SHR	R3	R2	R3
      0x541200FE,  //  0002  LDINT	R4	255
      0x2C0C0604,  //  0003  AND	R3	R3	R4
      0x541200FE,  //  0004  LDINT	R4	255
      0x2C100404,  //  0005  AND	R4	R2	R4
      0x8C140129,  //  0006  GETMET	R5	R0	K41
      0x001C090F,  //  0007  ADD	R7	R4	K15
      0x5820002A,  //  0008  LDCONST	R8	K42
      0x58240002,  //  0009  LDCONST	R9	K2
      0x5C280600,  //  000A  MOVE	R10	R3
      0x7C140A00,  //  000B  CALL	R5	5
      0x80000000,  //  000C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_Matter_Device_init,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 1]) {
      be_nested_proto(
        2,                          /* nstack */
        0,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 1]) {     /* constants */
        /* K0   */  be_nested_str_weak(start),
        }),
        be_str_weak(_anonymous_),
        &be_const_str_solidified,
        ( &(const binstruction[ 4]) {  /* code */
          0x68000000,  //  0000  GETUPV	R0	U0
          0x8C000100,  //  0001  GETMET	R0	R0	K0
          0x7C000200,  //  0002  CALL	R0	1
          0x80000000,  //  0003  RET	0
        })
      ),
    }),
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[83]) {  /* code */
      0xA4065600,  //  0000  IMPORT	R1	K43
      0xB80A0C00,  //  0001  GETNGBL	R2	K6
      0x8C08052C,  //  0002  GETMET	R2	R2	K44
      0xB8122800,  //  0003  GETNGBL	R4	K20
      0x8810092D,  //  0004  GETMBR	R4	R4	K45
      0x7C080400,  //  0005  CALL	R2	2
      0x740A0006,  //  0006  JMPT	R2	#000E
      0xB80A2800,  //  0007  GETNGBL	R2	K20
      0x8C08052F,  //  0008  GETMET	R2	R2	K47
      0x5C100000,  //  0009  MOVE	R4	R0
      0x50140000,  //  000A  LDBOOL	R5	0	0
      0x7C080600,  //  000B  CALL	R2	3
      0x90025C02,  //  000C  SETMBR	R0	K46	R2
      0x80000400,  //  000D  RET	0
      0xB80A2800,  //  000E  GETNGBL	R2	K20
      0xB80E2800,  //  000F  GETNGBL	R3	K20
      0x8C0C0731,  //  0010  GETMET	R3	R3	K49
      0x7C0C0200,  //  0011  CALL	R3	1
      0x900A6003,  //  0012  SETMBR	R2	K48	R3
      0x90024502,  //  0013  SETMBR	R0	K34	K2
      0x60080012,  //  0014  GETGBL	R2	G18
      0x7C080000,  //  0015  CALL	R2	0
      0x90021A02,  //  0016  SETMBR	R0	K13	R2
      0x50080000,  //  0017  LDBOOL	R2	0	0
      0x90020602,  //  0018  SETMBR	R0	K3	R2
      0x60080013,  //  0019  GETGBL	R2	G19
      0x7C080000,  //  001A  CALL	R2	0
      0x90023802,  //  001B  SETMBR	R0	K28	R2
      0x88080133,  //  001C  GETMBR	R2	R0	K51
      0x90026402,  //  001D  SETMBR	R0	K50	R2
      0x50080000,  //  001E  LDBOOL	R2	0	0
      0x90026802,  //  001F  SETMBR	R0	K52	R2
      0x50080000,  //  0020  LDBOOL	R2	0	0
      0x90026A02,  //  0021  SETMBR	R0	K53	R2
      0xB80A2800,  //  0022  GETNGBL	R2	K20
      0x8C080537,  //  0023  GETMET	R2	R2	K55
      0x5C100000,  //  0024  MOVE	R4	R0
      0x7C080400,  //  0025  CALL	R2	2
      0x90026C02,  //  0026  SETMBR	R0	K54	R2
      0x8C080138,  //  0027  GETMET	R2	R0	K56
      0x7C080200,  //  0028  CALL	R2	1
      0xB80A2800,  //  0029  GETNGBL	R2	K20
      0x8C080539,  //  002A  GETMET	R2	R2	K57
      0x5C100000,  //  002B  MOVE	R4	R0
      0x7C080400,  //  002C  CALL	R2	2
      0x90020002,  //  002D  SETMBR	R0	K0	R2
      0x88080100,  //  002E  GETMBR	R2	R0	K0
      0x8C08053A,  //  002F  GETMET	R2	R2	K58
      0x7C080200,  //  0030  CALL	R2	1
      0xB80A2800,  //  0031  GETNGBL	R2	K20
      0x8C08053B,  //  0032  GETMET	R2	R2	K59
      0x5C100000,  //  0033  MOVE	R4	R0
      0x7C080400,  //  0034  CALL	R2	2
      0x90024602,  //  0035  SETMBR	R0	K35	R2
      0xB80A2800,  //  0036  GETNGBL	R2	K20
      0x8C08053D,  //  0037  GETMET	R2	R2	K61
      0x5C100000,  //  0038  MOVE	R4	R0
      0x7C080400,  //  0039  CALL	R2	2
      0x90027802,  //  003A  SETMBR	R0	K60	R2
      0x8C08013F,  //  003B  GETMET	R2	R0	K63
      0x7C080200,  //  003C  CALL	R2	1
      0x90027C02,  //  003D  SETMBR	R0	K62	R2
      0xB80A2800,  //  003E  GETNGBL	R2	K20
      0x8C08052F,  //  003F  GETMET	R2	R2	K47
      0x5C100000,  //  0040  MOVE	R4	R0
      0x50140200,  //  0041  LDBOOL	R5	1	0
      0x7C080600,  //  0042  CALL	R2	3
      0x90025C02,  //  0043  SETMBR	R0	K46	R2
      0xB80A0C00,  //  0044  GETNGBL	R2	K6
      0x8C080540,  //  0045  GETMET	R2	R2	K64
      0x84100000,  //  0046  CLOSURE	R4	P0
      0x7C080400,  //  0047  CALL	R2	2
      0x88080136,  //  0048  GETMBR	R2	R0	K54
      0x8C080541,  //  0049  GETMET	R2	R2	K65
      0x7C080200,  //  004A  CALL	R2	1
      0xB80A0C00,  //  004B  GETNGBL	R2	K6
      0x8C080542,  //  004C  GETMET	R2	R2	K66
      0x5C100000,  //  004D  MOVE	R4	R0
      0x7C080400,  //  004E  CALL	R2	2
      0x8C080143,  //  004F  GETMET	R2	R0	K67
      0x7C080200,  //  0050  CALL	R2	1
      0xA0000000,  //  0051  CLOSE	R0
      0x80000000,  //  0052  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: sort_distinct
********************************************************************/
be_local_closure(class_Matter_Device_sort_distinct,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    12,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(sort_distinct),
    &be_const_str_solidified,
    ( &(const binstruction[53]) {  /* code */
      0x5804001E,  //  0000  LDCONST	R1	K30
      0x60080010,  //  0001  GETGBL	R2	G16
      0x600C000C,  //  0002  GETGBL	R3	G12
      0x5C100000,  //  0003  MOVE	R4	R0
      0x7C0C0200,  //  0004  CALL	R3	1
      0x040C070F,  //  0005  SUB	R3	R3	K15
      0x400E1E03,  //  0006  CONNECT	R3	K15	R3
      0x7C080200,  //  0007  CALL	R2	1
      0xA8020010,  //  0008  EXBLK	0	#001A
      0x5C0C0400,  //  0009  MOVE	R3	R2
      0x7C0C0000,  //  000A  CALL	R3	0
      0x94100003,  //  000B  GETIDX	R4	R0	R3
      0x5C140600,  //  000C  MOVE	R5	R3
      0x24180B02,  //  000D  GT	R6	R5	K2
      0x781A0008,  //  000E  JMPF	R6	#0018
      0x04180B0F,  //  000F  SUB	R6	R5	K15
      0x94180006,  //  0010  GETIDX	R6	R0	R6
      0x24180C04,  //  0011  GT	R6	R6	R4
      0x781A0004,  //  0012  JMPF	R6	#0018
      0x04180B0F,  //  0013  SUB	R6	R5	K15
      0x94180006,  //  0014  GETIDX	R6	R0	R6
      0x98000A06,  //  0015  SETIDX	R0	R5	R6
      0x04140B0F,  //  0016  SUB	R5	R5	K15
      0x7001FFF4,  //  0017  JMP		#000D
      0x98000A04,  //  0018  SETIDX	R0	R5	R4
      0x7001FFEE,  //  0019  JMP		#0009
      0x58080021,  //  001A  LDCONST	R2	K33
      0xAC080200,  //  001B  CATCH	R2	1	0
      0xB0080000,  //  001C  RAISE	2	R0	R0
      0x5808000F,  //  001D  LDCONST	R2	K15
      0x600C000C,  //  001E  GETGBL	R3	G12
      0x5C100000,  //  001F  MOVE	R4	R0
      0x7C0C0200,  //  0020  CALL	R3	1
      0x180C070F,  //  0021  LE	R3	R3	K15
      0x780E0000,  //  0022  JMPF	R3	#0024
      0x80040000,  //  0023  RET	1	R0
      0x940C0102,  //  0024  GETIDX	R3	R0	K2
      0x6010000C,  //  0025  GETGBL	R4	G12
      0x5C140000,  //  0026  MOVE	R5	R0
      0x7C100200,  //  0027  CALL	R4	1
      0x14100404,  //  0028  LT	R4	R2	R4
      0x78120009,  //  0029  JMPF	R4	#0034
      0x94100002,  //  002A  GETIDX	R4	R0	R2
      0x1C100803,  //  002B  EQ	R4	R4	R3
      0x78120003,  //  002C  JMPF	R4	#0031
      0x8C100144,  //  002D  GETMET	R4	R0	K68
      0x5C180400,  //  002E  MOVE	R6	R2
      0x7C100400,  //  002F  CALL	R4	2
      0x70020001,  //  0030  JMP		#0033
      0x940C0002,  //  0031  GETIDX	R3	R0	R2
      0x0008050F,  //  0032  ADD	R2	R2	K15
      0x7001FFF0,  //  0033  JMP		#0025
      0x80040000,  //  0034  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: remove_fabric
********************************************************************/
be_local_closure(class_Matter_Device_remove_fabric,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(remove_fabric),
    &be_const_str_solidified,
    ( &(const binstruction[33]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x20080202,  //  0001  NE	R2	R1	R2
      0x780A0019,  //  0002  JMPF	R2	#001D
      0xB80A1400,  //  0003  GETNGBL	R2	K10
      0x8C0C0346,  //  0004  GETMET	R3	R1	K70
      0x7C0C0200,  //  0005  CALL	R3	1
      0x8C0C0747,  //  0006  GETMET	R3	R3	K71
      0x7C0C0200,  //  0007  CALL	R3	1
      0x8C0C0748,  //  0008  GETMET	R3	R3	K72
      0x7C0C0200,  //  0009  CALL	R3	1
      0x8C0C0749,  //  000A  GETMET	R3	R3	K73
      0x7C0C0200,  //  000B  CALL	R3	1
      0x000E8A03,  //  000C  ADD	R3	K69	R3
      0x5810002A,  //  000D  LDCONST	R4	K42
      0x7C080400,  //  000E  CALL	R2	2
      0x88080123,  //  000F  GETMBR	R2	R0	K35
      0x8808054A,  //  0010  GETMBR	R2	R2	K74
      0x8808054B,  //  0011  GETMBR	R2	R2	K75
      0x8C08054C,  //  0012  GETMET	R2	R2	K76
      0x5C100200,  //  0013  MOVE	R4	R1
      0x7C080400,  //  0014  CALL	R2	2
      0x88080136,  //  0015  GETMBR	R2	R0	K54
      0x8C08054D,  //  0016  GETMET	R2	R2	K77
      0x5C100200,  //  0017  MOVE	R4	R1
      0x7C080400,  //  0018  CALL	R2	2
      0x88080100,  //  0019  GETMBR	R2	R0	K0
      0x8C08054E,  //  001A  GETMET	R2	R2	K78
      0x5C100200,  //  001B  MOVE	R4	R1
      0x7C080400,  //  001C  CALL	R2	2
      0x88080100,  //  001D  GETMBR	R2	R0	K0
      0x8C08054F,  //  001E  GETMET	R2	R2	K79
      0x7C080200,  //  001F  CALL	R2	1
      0x80000000,  //  0020  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: bridge_remove_endpoint
********************************************************************/
be_local_closure(class_Matter_Device_bridge_remove_endpoint,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(bridge_remove_endpoint),
    &be_const_str_solidified,
    ( &(const binstruction[56]) {  /* code */
      0xA40A0A00,  //  0000  IMPORT	R2	K5
      0x600C0008,  //  0001  GETGBL	R3	G8
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C0C0200,  //  0003  CALL	R3	1
      0x4C100000,  //  0004  LDNIL	R4
      0x4C140000,  //  0005  LDNIL	R5
      0x88180150,  //  0006  GETMBR	R6	R0	K80
      0x8C180D51,  //  0007  GETMET	R6	R6	K81
      0x5C200600,  //  0008  MOVE	R8	R3
      0x7C180400,  //  0009  CALL	R6	2
      0x741A0004,  //  000A  JMPT	R6	#0010
      0xB81A1400,  //  000B  GETNGBL	R6	K10
      0x001EA403,  //  000C  ADD	R7	K82	R3
      0x58200009,  //  000D  LDCONST	R8	K9
      0x7C180400,  //  000E  CALL	R6	2
      0x80000C00,  //  000F  RET	0
      0xB81A1400,  //  0010  GETNGBL	R6	K10
      0x601C0018,  //  0011  GETGBL	R7	G24
      0x58200053,  //  0012  LDCONST	R8	K83
      0x5C240200,  //  0013  MOVE	R9	R1
      0x7C1C0400,  //  0014  CALL	R7	2
      0x5820002A,  //  0015  LDCONST	R8	K42
      0x7C180400,  //  0016  CALL	R6	2
      0x88180150,  //  0017  GETMBR	R6	R0	K80
      0x8C180D44,  //  0018  GETMET	R6	R6	K68
      0x5C200600,  //  0019  MOVE	R8	R3
      0x7C180400,  //  001A  CALL	R6	2
      0x50180200,  //  001B  LDBOOL	R6	1	0
      0x90020606,  //  001C  SETMBR	R0	K3	R6
      0x58180002,  //  001D  LDCONST	R6	K2
      0x601C000C,  //  001E  GETGBL	R7	G12
      0x8820010D,  //  001F  GETMBR	R8	R0	K13
      0x7C1C0200,  //  0020  CALL	R7	1
      0x141C0C07,  //  0021  LT	R7	R6	R7
      0x781E000D,  //  0022  JMPF	R7	#0031
      0x881C010D,  //  0023  GETMBR	R7	R0	K13
      0x941C0E06,  //  0024  GETIDX	R7	R7	R6
      0x8C1C0F54,  //  0025  GETMET	R7	R7	K84
      0x7C1C0200,  //  0026  CALL	R7	1
      0x1C1C0207,  //  0027  EQ	R7	R1	R7
      0x781E0005,  //  0028  JMPF	R7	#002F
      0x881C010D,  //  0029  GETMBR	R7	R0	K13
      0x8C1C0F44,  //  002A  GETMET	R7	R7	K68
      0x5C240C00,  //  002B  MOVE	R9	R6
      0x7C1C0400,  //  002C  CALL	R7	2
      0x70020002,  //  002D  JMP		#0031
      0x70020000,  //  002E  JMP		#0030
      0x00180D0F,  //  002F  ADD	R6	R6	K15
      0x7001FFEC,  //  0030  JMP		#001E
      0x8C1C0155,  //  0031  GETMET	R7	R0	K85
      0x7C1C0200,  //  0032  CALL	R7	1
      0x8C1C0104,  //  0033  GETMET	R7	R0	K4
      0x7C1C0200,  //  0034  CALL	R7	1
      0x8C1C0156,  //  0035  GETMET	R7	R0	K86
      0x7C1C0200,  //  0036  CALL	R7	1
      0x80000000,  //  0037  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: every_250ms
********************************************************************/
be_local_closure(class_Matter_Device_every_250ms,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(every_250ms),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x8C040157,  //  0000  GETMET	R1	R0	K87
      0x7C040200,  //  0001  CALL	R1	1
      0x58040002,  //  0002  LDCONST	R1	K2
      0x6008000C,  //  0003  GETGBL	R2	G12
      0x880C010D,  //  0004  GETMBR	R3	R0	K13
      0x7C080200,  //  0005  CALL	R2	1
      0x14080202,  //  0006  LT	R2	R1	R2
      0x780A0005,  //  0007  JMPF	R2	#000E
      0x8808010D,  //  0008  GETMBR	R2	R0	K13
      0x94080401,  //  0009  GETIDX	R2	R2	R1
      0x8C080558,  //  000A  GETMET	R2	R2	K88
      0x7C080200,  //  000B  CALL	R2	1
      0x0004030F,  //  000C  ADD	R1	R1	K15
      0x7001FFF4,  //  000D  JMP		#0003
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start
********************************************************************/
be_local_closure(class_Matter_Device_start,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(start),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x8C040159,  //  0000  GETMET	R1	R0	K89
      0x7C040200,  //  0001  CALL	R1	1
      0x8C04015A,  //  0002  GETMET	R1	R0	K90
      0x880C015B,  //  0003  GETMBR	R3	R0	K91
      0x7C040400,  //  0004  CALL	R1	2
      0x88040136,  //  0005  GETMBR	R1	R0	K54
      0x8C04035C,  //  0006  GETMET	R1	R1	K92
      0x7C040200,  //  0007  CALL	R1	1
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: register_commands
********************************************************************/
be_local_closure(class_Matter_Device_register_commands,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 3]) {
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
        /* K0   */  be_nested_str_weak(MtrJoin),
        }),
        be_str_weak(_X3Clambda_X3E),
        &be_const_str_solidified,
        ( &(const binstruction[ 8]) {  /* code */
          0x68100000,  //  0000  GETUPV	R4	U0
          0x8C100900,  //  0001  GETMET	R4	R4	K0
          0x5C180000,  //  0002  MOVE	R6	R0
          0x5C1C0200,  //  0003  MOVE	R7	R1
          0x5C200400,  //  0004  MOVE	R8	R2
          0x5C240600,  //  0005  MOVE	R9	R3
          0x7C100A00,  //  0006  CALL	R4	5
          0x80040800,  //  0007  RET	1	R4
        })
      ),
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
        /* K0   */  be_nested_str_weak(MtrUpdate),
        }),
        be_str_weak(_X3Clambda_X3E),
        &be_const_str_solidified,
        ( &(const binstruction[ 8]) {  /* code */
          0x68100000,  //  0000  GETUPV	R4	U0
          0x8C100900,  //  0001  GETMET	R4	R4	K0
          0x5C180000,  //  0002  MOVE	R6	R0
          0x5C1C0200,  //  0003  MOVE	R7	R1
          0x5C200400,  //  0004  MOVE	R8	R2
          0x5C240600,  //  0005  MOVE	R9	R3
          0x7C100A00,  //  0006  CALL	R4	5
          0x80040800,  //  0007  RET	1	R4
        })
      ),
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
        /* K0   */  be_nested_str_weak(MtrInfo),
        }),
        be_str_weak(_X3Clambda_X3E),
        &be_const_str_solidified,
        ( &(const binstruction[ 8]) {  /* code */
          0x68100000,  //  0000  GETUPV	R4	U0
          0x8C100900,  //  0001  GETMET	R4	R4	K0
          0x5C180000,  //  0002  MOVE	R6	R0
          0x5C1C0200,  //  0003  MOVE	R7	R1
          0x5C200400,  //  0004  MOVE	R8	R2
          0x5C240600,  //  0005  MOVE	R9	R3
          0x7C100A00,  //  0006  CALL	R4	5
          0x80040800,  //  0007  RET	1	R4
        })
      ),
    }),
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(register_commands),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0xB8060C00,  //  0000  GETNGBL	R1	K6
      0x8C04035D,  //  0001  GETMET	R1	R1	K93
      0x580C005E,  //  0002  LDCONST	R3	K94
      0x84100000,  //  0003  CLOSURE	R4	P0
      0x7C040600,  //  0004  CALL	R1	3
      0xB8060C00,  //  0005  GETNGBL	R1	K6
      0x8C04035D,  //  0006  GETMET	R1	R1	K93
      0x580C005F,  //  0007  LDCONST	R3	K95
      0x84100001,  //  0008  CLOSURE	R4	P1
      0x7C040600,  //  0009  CALL	R1	3
      0xB8060C00,  //  000A  GETNGBL	R1	K6
      0x8C04035D,  //  000B  GETMET	R1	R1	K93
      0x580C0060,  //  000C  LDCONST	R3	K96
      0x84100002,  //  000D  CLOSURE	R4	P2
      0x7C040600,  //  000E  CALL	R1	3
      0xA0000000,  //  000F  CLOSE	R0
      0x80000000,  //  0010  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: MtrInfo
********************************************************************/
be_local_closure(class_Matter_Device_MtrInfo,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    5,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(MtrInfo),
    &be_const_str_solidified,
    ( &(const binstruction[40]) {  /* code */
      0x1C140725,  //  0000  EQ	R5	R3	K37
      0x7815FFFF,  //  0001  JMPF	R5	#0002
      0x1C140725,  //  0002  EQ	R5	R3	K37
      0x7816000D,  //  0003  JMPF	R5	#0012
      0x60140010,  //  0004  GETGBL	R5	G16
      0x8818010D,  //  0005  GETMBR	R6	R0	K13
      0x7C140200,  //  0006  CALL	R5	1
      0xA8020005,  //  0007  EXBLK	0	#000E
      0x5C180A00,  //  0008  MOVE	R6	R5
      0x7C180000,  //  0009  CALL	R6	0
      0x8C1C0161,  //  000A  GETMET	R7	R0	K97
      0x88240D11,  //  000B  GETMBR	R9	R6	K17
      0x7C1C0400,  //  000C  CALL	R7	2
      0x7001FFF9,  //  000D  JMP		#0008
      0x58140021,  //  000E  LDCONST	R5	K33
      0xAC140200,  //  000F  CATCH	R5	1	0
      0xB0080000,  //  0010  RAISE	2	R0	R0
      0x70020011,  //  0011  JMP		#0024
      0x60140004,  //  0012  GETGBL	R5	G4
      0x5C180800,  //  0013  MOVE	R6	R4
      0x7C140200,  //  0014  CALL	R5	1
      0x1C140B62,  //  0015  EQ	R5	R5	K98
      0x78160003,  //  0016  JMPF	R5	#001B
      0x8C140161,  //  0017  GETMET	R5	R0	K97
      0x5C1C0800,  //  0018  MOVE	R7	R4
      0x7C140400,  //  0019  CALL	R5	2
      0x70020008,  //  001A  JMP		#0024
      0x8C140163,  //  001B  GETMET	R5	R0	K99
      0x5C1C0600,  //  001C  MOVE	R7	R3
      0x7C140400,  //  001D  CALL	R5	2
      0x4C180000,  //  001E  LDNIL	R6
      0x20180A06,  //  001F  NE	R6	R5	R6
      0x781A0002,  //  0020  JMPF	R6	#0024
      0x8C180161,  //  0021  GETMET	R6	R0	K97
      0x88200B11,  //  0022  GETMBR	R8	R5	K17
      0x7C180400,  //  0023  CALL	R6	2
      0xB8160C00,  //  0024  GETNGBL	R5	K6
      0x8C140B64,  //  0025  GETMET	R5	R5	K100
      0x7C140200,  //  0026  CALL	R5	1
      0x80000000,  //  0027  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: msg_received
********************************************************************/
be_local_closure(class_Matter_Device_msg_received,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(msg_received),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x88100123,  //  0000  GETMBR	R4	R0	K35
      0x8C100965,  //  0001  GETMET	R4	R4	K101
      0x5C180200,  //  0002  MOVE	R6	R1
      0x5C1C0400,  //  0003  MOVE	R7	R2
      0x5C200600,  //  0004  MOVE	R8	R3
      0x7C100800,  //  0005  CALL	R4	4
      0x80040800,  //  0006  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: load_param
********************************************************************/
be_local_closure(class_Matter_Device_load_param,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(load_param),
    &be_const_str_solidified,
    ( &(const binstruction[136]) {  /* code */
      0xA4065600,  //  0000  IMPORT	R1	K43
      0x50080000,  //  0001  LDBOOL	R2	0	0
      0xA8020056,  //  0002  EXBLK	0	#005A
      0x600C0011,  //  0003  GETGBL	R3	G17
      0x88100166,  //  0004  GETMBR	R4	R0	K102
      0x7C0C0200,  //  0005  CALL	R3	1
      0x8C100767,  //  0006  GETMET	R4	R3	K103
      0x7C100200,  //  0007  CALL	R4	1
      0x8C140768,  //  0008  GETMET	R5	R3	K104
      0x7C140200,  //  0009  CALL	R5	1
      0xA4160A00,  //  000A  IMPORT	R5	K5
      0x8C180B0C,  //  000B  GETMET	R6	R5	K12
      0x5C200800,  //  000C  MOVE	R8	R4
      0x7C180400,  //  000D  CALL	R6	2
      0x8C1C0D1D,  //  000E  GETMET	R7	R6	K29
      0x5824006A,  //  000F  LDCONST	R9	K106
      0x88280169,  //  0010  GETMBR	R10	R0	K105
      0x7C1C0600,  //  0011  CALL	R7	3
      0x9002D207,  //  0012  SETMBR	R0	K105	R7
      0x8C1C0D1D,  //  0013  GETMET	R7	R6	K29
      0x5824006C,  //  0014  LDCONST	R9	K108
      0x8828016B,  //  0015  GETMBR	R10	R0	K107
      0x7C1C0600,  //  0016  CALL	R7	3
      0x9002D607,  //  0017  SETMBR	R0	K107	R7
      0x601C0017,  //  0018  GETGBL	R7	G23
      0x8C200D1D,  //  0019  GETMET	R8	R6	K29
      0x58280034,  //  001A  LDCONST	R10	K52
      0x502C0000,  //  001B  LDBOOL	R11	0	0
      0x7C200600,  //  001C  CALL	R8	3
      0x7C1C0200,  //  001D  CALL	R7	1
      0x90026807,  //  001E  SETMBR	R0	K52	R7
      0x601C0017,  //  001F  GETGBL	R7	G23
      0x8C200D1D,  //  0020  GETMET	R8	R6	K29
      0x58280035,  //  0021  LDCONST	R10	K53
      0x502C0000,  //  0022  LDBOOL	R11	0	0
      0x7C200600,  //  0023  CALL	R8	3
      0x7C1C0200,  //  0024  CALL	R7	1
      0x90026A07,  //  0025  SETMBR	R0	K53	R7
      0x8C1C0D1D,  //  0026  GETMET	R7	R6	K29
      0x5824006D,  //  0027  LDCONST	R9	K109
      0x88280132,  //  0028  GETMBR	R10	R0	K50
      0x7C1C0600,  //  0029  CALL	R7	3
      0x90026407,  //  002A  SETMBR	R0	K50	R7
      0x8C1C0D1D,  //  002B  GETMET	R7	R6	K29
      0x5824006E,  //  002C  LDCONST	R9	K110
      0x60280013,  //  002D  GETGBL	R10	G19
      0x7C280000,  //  002E  CALL	R10	0
      0x7C1C0600,  //  002F  CALL	R7	3
      0x9002A007,  //  0030  SETMBR	R0	K80	R7
      0x601C0017,  //  0031  GETGBL	R7	G23
      0x8C200D1D,  //  0032  GETMET	R8	R6	K29
      0x5828006F,  //  0033  LDCONST	R10	K111
      0x7C200400,  //  0034  CALL	R8	2
      0x7C1C0200,  //  0035  CALL	R7	1
      0x9002DE07,  //  0036  SETMBR	R0	K111	R7
      0x881C0150,  //  0037  GETMBR	R7	R0	K80
      0x4C200000,  //  0038  LDNIL	R8
      0x201C0E08,  //  0039  NE	R7	R7	R8
      0x781E000D,  //  003A  JMPF	R7	#0049
      0xB81E1400,  //  003B  GETNGBL	R7	K10
      0x60200018,  //  003C  GETGBL	R8	G24
      0x58240070,  //  003D  LDCONST	R9	K112
      0x88280150,  //  003E  GETMBR	R10	R0	K80
      0x7C200400,  //  003F  CALL	R8	2
      0x58240009,  //  0040  LDCONST	R9	K9
      0x7C1C0400,  //  0041  CALL	R7	2
      0x8C1C0171,  //  0042  GETMET	R7	R0	K113
      0x7C1C0200,  //  0043  CALL	R7	1
      0x8C1C0172,  //  0044  GETMET	R7	R0	K114
      0x7C1C0200,  //  0045  CALL	R7	1
      0x5C080E00,  //  0046  MOVE	R2	R7
      0x501C0200,  //  0047  LDBOOL	R7	1	0
      0x90020607,  //  0048  SETMBR	R0	K3	R7
      0x8C1C0D1D,  //  0049  GETMET	R7	R6	K29
      0x58240073,  //  004A  LDCONST	R9	K115
      0x60280013,  //  004B  GETGBL	R10	G19
      0x7C280000,  //  004C  CALL	R10	0
      0x7C1C0600,  //  004D  CALL	R7	3
      0x90023807,  //  004E  SETMBR	R0	K28	R7
      0x881C011C,  //  004F  GETMBR	R7	R0	K28
      0x781E0006,  //  0050  JMPF	R7	#0058
      0xB81E1400,  //  0051  GETNGBL	R7	K10
      0x60200008,  //  0052  GETGBL	R8	G8
      0x8824011C,  //  0053  GETMBR	R9	R0	K28
      0x7C200200,  //  0054  CALL	R8	1
      0x0022E808,  //  0055  ADD	R8	K116	R8
      0x58240009,  //  0056  LDCONST	R9	K9
      0x7C1C0400,  //  0057  CALL	R7	2
      0xA8040001,  //  0058  EXBLK	1	1
      0x70020011,  //  0059  JMP		#006C
      0xAC0C0002,  //  005A  CATCH	R3	0	2
      0x7002000E,  //  005B  JMP		#006B
      0x20140775,  //  005C  NE	R5	R3	K117
      0x7816000B,  //  005D  JMPF	R5	#006A
      0xB8161400,  //  005E  GETNGBL	R5	K10
      0x60180008,  //  005F  GETGBL	R6	G8
      0x5C1C0600,  //  0060  MOVE	R7	R3
      0x7C180200,  //  0061  CALL	R6	1
      0x001AEC06,  //  0062  ADD	R6	K118	R6
      0x00180D77,  //  0063  ADD	R6	R6	K119
      0x601C0008,  //  0064  GETGBL	R7	G8
      0x5C200800,  //  0065  MOVE	R8	R4
      0x7C1C0200,  //  0066  CALL	R7	1
      0x00180C07,  //  0067  ADD	R6	R6	R7
      0x581C002A,  //  0068  LDCONST	R7	K42
      0x7C140400,  //  0069  CALL	R5	2
      0x70020000,  //  006A  JMP		#006C
      0xB0080000,  //  006B  RAISE	2	R0	R0
      0x880C0169,  //  006C  GETMBR	R3	R0	K105
      0x4C100000,  //  006D  LDNIL	R4
      0x1C0C0604,  //  006E  EQ	R3	R3	R4
      0x780E000A,  //  006F  JMPF	R3	#007B
      0x8C0C0378,  //  0070  GETMET	R3	R1	K120
      0x5814002A,  //  0071  LDCONST	R5	K42
      0x7C0C0400,  //  0072  CALL	R3	2
      0x8C0C0779,  //  0073  GETMET	R3	R3	K121
      0x58140002,  //  0074  LDCONST	R5	K2
      0x5818002A,  //  0075  LDCONST	R6	K42
      0x7C0C0600,  //  0076  CALL	R3	3
      0x54120FFE,  //  0077  LDINT	R4	4095
      0x2C0C0604,  //  0078  AND	R3	R3	R4
      0x9002D203,  //  0079  SETMBR	R0	K105	R3
      0x50080200,  //  007A  LDBOOL	R2	1	0
      0x880C016B,  //  007B  GETMBR	R3	R0	K107
      0x4C100000,  //  007C  LDNIL	R4
      0x1C0C0604,  //  007D  EQ	R3	R3	R4
      0x780E0004,  //  007E  JMPF	R3	#0084
      0x880C0136,  //  007F  GETMBR	R3	R0	K54
      0x8C0C077A,  //  0080  GETMET	R3	R3	K122
      0x7C0C0200,  //  0081  CALL	R3	1
      0x9002D603,  //  0082  SETMBR	R0	K107	R3
      0x50080200,  //  0083  LDBOOL	R2	1	0
      0x780A0001,  //  0084  JMPF	R2	#0087
      0x8C0C0104,  //  0085  GETMET	R3	R0	K4
      0x7C0C0200,  //  0086  CALL	R3	1
      0x80000000,  //  0087  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: register_http_remote
********************************************************************/
be_local_closure(class_Matter_Device_register_http_remote,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(register_http_remote),
    &be_const_str_solidified,
    ( &(const binstruction[42]) {  /* code */
      0x880C017B,  //  0000  GETMBR	R3	R0	K123
      0x4C100000,  //  0001  LDNIL	R4
      0x1C0C0604,  //  0002  EQ	R3	R3	R4
      0x780E0002,  //  0003  JMPF	R3	#0007
      0x600C0013,  //  0004  GETGBL	R3	G19
      0x7C0C0000,  //  0005  CALL	R3	0
      0x9002F603,  //  0006  SETMBR	R0	K123	R3
      0x4C0C0000,  //  0007  LDNIL	R3
      0x8810017B,  //  0008  GETMBR	R4	R0	K123
      0x8C100951,  //  0009  GETMET	R4	R4	K81
      0x5C180200,  //  000A  MOVE	R6	R1
      0x7C100400,  //  000B  CALL	R4	2
      0x78120009,  //  000C  JMPF	R4	#0017
      0x8810017B,  //  000D  GETMBR	R4	R0	K123
      0x940C0801,  //  000E  GETIDX	R3	R4	R1
      0x8C10077C,  //  000F  GETMET	R4	R3	K124
      0x7C100200,  //  0010  CALL	R4	1
      0x14100404,  //  0011  LT	R4	R2	R4
      0x78120002,  //  0012  JMPF	R4	#0016
      0x8C10077D,  //  0013  GETMET	R4	R3	K125
      0x5C180400,  //  0014  MOVE	R6	R2
      0x7C100400,  //  0015  CALL	R4	2
      0x70020011,  //  0016  JMP		#0029
      0xB8122800,  //  0017  GETNGBL	R4	K20
      0x8C10097E,  //  0018  GETMET	R4	R4	K126
      0x5C180000,  //  0019  MOVE	R6	R0
      0x5C1C0200,  //  001A  MOVE	R7	R1
      0x5C200400,  //  001B  MOVE	R8	R2
      0x7C100800,  //  001C  CALL	R4	4
      0x5C0C0800,  //  001D  MOVE	R3	R4
      0x8810011C,  //  001E  GETMBR	R4	R0	K28
      0x8C100951,  //  001F  GETMET	R4	R4	K81
      0x5C180200,  //  0020  MOVE	R6	R1
      0x7C100400,  //  0021  CALL	R4	2
      0x78120003,  //  0022  JMPF	R4	#0027
      0x8C10077F,  //  0023  GETMET	R4	R3	K127
      0x8818011C,  //  0024  GETMBR	R6	R0	K28
      0x94180C01,  //  0025  GETIDX	R6	R6	R1
      0x7C100400,  //  0026  CALL	R4	2
      0x8810017B,  //  0027  GETMBR	R4	R0	K123
      0x98100203,  //  0028  SETIDX	R4	R1	R3
      0x80040600,  //  0029  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: reset_param
********************************************************************/
be_local_closure(class_Matter_Device_reset_param,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(reset_param),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x50040000,  //  0000  LDBOOL	R1	0	0
      0x90020601,  //  0001  SETMBR	R0	K3	R1
      0x88040133,  //  0002  GETMBR	R1	R0	K51
      0x90026401,  //  0003  SETMBR	R0	K50	R1
      0x8C040104,  //  0004  GETMET	R1	R0	K4
      0x7C040200,  //  0005  CALL	R1	1
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: MtrJoin
********************************************************************/
be_local_closure(class_Matter_Device_MtrJoin,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    5,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(MtrJoin),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x60140009,  //  0000  GETGBL	R5	G9
      0x5C180600,  //  0001  MOVE	R6	R3
      0x7C140200,  //  0002  CALL	R5	1
      0x78160003,  //  0003  JMPF	R5	#0008
      0x88180136,  //  0004  GETMBR	R6	R0	K54
      0x8C180D80,  //  0005  GETMET	R6	R6	K128
      0x7C180200,  //  0006  CALL	R6	1
      0x70020002,  //  0007  JMP		#000B
      0x88180136,  //  0008  GETMBR	R6	R0	K54
      0x8C180D81,  //  0009  GETMET	R6	R6	K129
      0x7C180200,  //  000A  CALL	R6	1
      0xB81A0C00,  //  000B  GETNGBL	R6	K6
      0x8C180D64,  //  000C  GETMET	R6	R6	K100
      0x7C180200,  //  000D  CALL	R6	1
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: autoconf_device
********************************************************************/
be_local_closure(class_Matter_Device_autoconf_device,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(autoconf_device),
    &be_const_str_solidified,
    ( &(const binstruction[50]) {  /* code */
      0xA4060A00,  //  0000  IMPORT	R1	K5
      0x6008000C,  //  0001  GETGBL	R2	G12
      0x880C010D,  //  0002  GETMBR	R3	R0	K13
      0x7C080200,  //  0003  CALL	R2	1
      0x24080502,  //  0004  GT	R2	R2	K2
      0x780A0000,  //  0005  JMPF	R2	#0007
      0x80000400,  //  0006  RET	0
      0x88080182,  //  0007  GETMBR	R2	R0	K130
      0x4C0C0000,  //  0008  LDNIL	R3
      0x1C080403,  //  0009  EQ	R2	R2	R3
      0x780A0004,  //  000A  JMPF	R2	#0010
      0xB80A2800,  //  000B  GETNGBL	R2	K20
      0x8C080583,  //  000C  GETMET	R2	R2	K131
      0x5C100000,  //  000D  MOVE	R4	R0
      0x7C080400,  //  000E  CALL	R2	2
      0x90030402,  //  000F  SETMBR	R0	K130	R2
      0x88080103,  //  0010  GETMBR	R2	R0	K3
      0x740A000F,  //  0011  JMPT	R2	#0022
      0x88080182,  //  0012  GETMBR	R2	R0	K130
      0x8C080584,  //  0013  GETMET	R2	R2	K132
      0x7C080200,  //  0014  CALL	R2	1
      0x9002A002,  //  0015  SETMBR	R0	K80	R2
      0x60080013,  //  0016  GETGBL	R2	G19
      0x7C080000,  //  0017  CALL	R2	0
      0x90023802,  //  0018  SETMBR	R0	K28	R2
      0x8C080171,  //  0019  GETMET	R2	R0	K113
      0x7C080200,  //  001A  CALL	R2	1
      0xB80A1400,  //  001B  GETNGBL	R2	K10
      0x600C0008,  //  001C  GETGBL	R3	G8
      0x88100150,  //  001D  GETMBR	R4	R0	K80
      0x7C0C0200,  //  001E  CALL	R3	1
      0x000F0A03,  //  001F  ADD	R3	K133	R3
      0x58100009,  //  0020  LDCONST	R4	K9
      0x7C080400,  //  0021  CALL	R2	2
      0x88080182,  //  0022  GETMBR	R2	R0	K130
      0x8C080586,  //  0023  GETMET	R2	R2	K134
      0x88100150,  //  0024  GETMBR	R4	R0	K80
      0x7C080400,  //  0025  CALL	R2	2
      0x88080103,  //  0026  GETMBR	R2	R0	K3
      0x740A0008,  //  0027  JMPT	R2	#0031
      0x88080100,  //  0028  GETMBR	R2	R0	K0
      0x8C080501,  //  0029  GETMET	R2	R2	K1
      0x7C080200,  //  002A  CALL	R2	1
      0x24080502,  //  002B  GT	R2	R2	K2
      0x780A0003,  //  002C  JMPF	R2	#0031
      0x50080200,  //  002D  LDBOOL	R2	1	0
      0x90020602,  //  002E  SETMBR	R0	K3	R2
      0x8C080104,  //  002F  GETMET	R2	R0	K4
      0x7C080200,  //  0030  CALL	R2	1
      0x80000000,  //  0031  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_zigbee_present
********************************************************************/
be_local_closure(class_Matter_Device_is_zigbee_present,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(is_zigbee_present),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0xA4070E00,  //  0000  IMPORT	R1	K135
      0x8C080388,  //  0001  GETMET	R2	R1	K136
      0x58100089,  //  0002  LDCONST	R4	K137
      0x7C080400,  //  0003  CALL	R2	2
      0x4C0C0000,  //  0004  LDNIL	R3
      0x20080403,  //  0005  NE	R2	R2	R3
      0x80040400,  //  0006  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _start_udp
********************************************************************/
be_local_closure(class_Matter_Device__start_udp,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 1]) {
      be_nested_proto(
        8,                          /* nstack */
        3,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 1]) {     /* constants */
        /* K0   */  be_nested_str_weak(msg_received),
        }),
        be_str_weak(_X3Clambda_X3E),
        &be_const_str_solidified,
        ( &(const binstruction[ 7]) {  /* code */
          0x680C0000,  //  0000  GETUPV	R3	U0
          0x8C0C0700,  //  0001  GETMET	R3	R3	K0
          0x5C140000,  //  0002  MOVE	R5	R0
          0x5C180200,  //  0003  MOVE	R6	R1
          0x5C1C0400,  //  0004  MOVE	R7	R2
          0x7C0C0800,  //  0005  CALL	R3	4
          0x80040600,  //  0006  RET	1	R3
        })
      ),
    }),
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(_start_udp),
    &be_const_str_solidified,
    ( &(const binstruction[27]) {  /* code */
      0x8808018A,  //  0000  GETMBR	R2	R0	K138
      0x780A0000,  //  0001  JMPF	R2	#0003
      0x80000400,  //  0002  RET	0
      0x4C080000,  //  0003  LDNIL	R2
      0x1C080202,  //  0004  EQ	R2	R1	R2
      0x780A0000,  //  0005  JMPF	R2	#0007
      0x540615A3,  //  0006  LDINT	R1	5540
      0xB80A1400,  //  0007  GETNGBL	R2	K10
      0x600C0008,  //  0008  GETGBL	R3	G8
      0x5C100200,  //  0009  MOVE	R4	R1
      0x7C0C0200,  //  000A  CALL	R3	1
      0x000F1603,  //  000B  ADD	R3	K139	R3
      0x5810002A,  //  000C  LDCONST	R4	K42
      0x7C080400,  //  000D  CALL	R2	2
      0xB80A2800,  //  000E  GETNGBL	R2	K20
      0x8C08058C,  //  000F  GETMET	R2	R2	K140
      0x5C100000,  //  0010  MOVE	R4	R0
      0x58140025,  //  0011  LDCONST	R5	K37
      0x5C180200,  //  0012  MOVE	R6	R1
      0x7C080800,  //  0013  CALL	R2	4
      0x90031402,  //  0014  SETMBR	R0	K138	R2
      0x8808018A,  //  0015  GETMBR	R2	R0	K138
      0x8C08058D,  //  0016  GETMET	R2	R2	K141
      0x84100000,  //  0017  CLOSURE	R4	P0
      0x7C080400,  //  0018  CALL	R2	2
      0xA0000000,  //  0019  CLOSE	R0
      0x80000000,  //  001A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_active_endpoints
********************************************************************/
be_local_closure(class_Matter_Device_get_active_endpoints,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(get_active_endpoints),
    &be_const_str_solidified,
    ( &(const binstruction[28]) {  /* code */
      0x60080012,  //  0000  GETGBL	R2	G18
      0x7C080000,  //  0001  CALL	R2	0
      0x600C0010,  //  0002  GETGBL	R3	G16
      0x8810010D,  //  0003  GETMBR	R4	R0	K13
      0x7C0C0200,  //  0004  CALL	R3	1
      0xA8020011,  //  0005  EXBLK	0	#0018
      0x5C100600,  //  0006  MOVE	R4	R3
      0x7C100000,  //  0007  CALL	R4	0
      0x8C140954,  //  0008  GETMET	R5	R4	K84
      0x7C140200,  //  0009  CALL	R5	1
      0x78060002,  //  000A  JMPF	R1	#000E
      0x1C180B02,  //  000B  EQ	R6	R5	K2
      0x781A0000,  //  000C  JMPF	R6	#000E
      0x7001FFF7,  //  000D  JMP		#0006
      0x8C18051D,  //  000E  GETMET	R6	R2	K29
      0x5C200A00,  //  000F  MOVE	R8	R5
      0x7C180400,  //  0010  CALL	R6	2
      0x4C1C0000,  //  0011  LDNIL	R7
      0x1C180C07,  //  0012  EQ	R6	R6	R7
      0x781A0002,  //  0013  JMPF	R6	#0017
      0x8C180520,  //  0014  GETMET	R6	R2	K32
      0x5C200A00,  //  0015  MOVE	R8	R5
      0x7C180400,  //  0016  CALL	R6	2
      0x7001FFED,  //  0017  JMP		#0006
      0x580C0021,  //  0018  LDCONST	R3	K33
      0xAC0C0200,  //  0019  CATCH	R3	1	0
      0xB0080000,  //  001A  RAISE	2	R0	R0
      0x80040400,  //  001B  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: save_before_restart
********************************************************************/
be_local_closure(class_Matter_Device_save_before_restart,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(save_before_restart),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x88040136,  //  0000  GETMBR	R1	R0	K54
      0x8C040381,  //  0001  GETMET	R1	R1	K129
      0x7C040200,  //  0002  CALL	R1	1
      0x88040136,  //  0003  GETMBR	R1	R0	K54
      0x8C04038E,  //  0004  GETMET	R1	R1	K142
      0x7C040200,  //  0005  CALL	R1	1
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: attribute_updated
********************************************************************/
be_local_closure(class_Matter_Device_attribute_updated,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    5,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(attribute_updated),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x4C140000,  //  0000  LDNIL	R5
      0x1C140805,  //  0001  EQ	R5	R4	R5
      0x78160000,  //  0002  JMPF	R5	#0004
      0x50100000,  //  0003  LDBOOL	R4	0	0
      0xB8162800,  //  0004  GETNGBL	R5	K20
      0x8C140B8F,  //  0005  GETMET	R5	R5	K143
      0x7C140200,  //  0006  CALL	R5	1
      0x90162201,  //  0007  SETMBR	R5	K17	R1
      0x90172002,  //  0008  SETMBR	R5	K144	R2
      0x90172203,  //  0009  SETMBR	R5	K145	R3
      0x88180123,  //  000A  GETMBR	R6	R0	K35
      0x88180D4A,  //  000B  GETMBR	R6	R6	K74
      0x88180D4B,  //  000C  GETMBR	R6	R6	K75
      0x8C180D92,  //  000D  GETMET	R6	R6	K146
      0x5C200A00,  //  000E  MOVE	R8	R5
      0x5C240800,  //  000F  MOVE	R9	R4
      0x7C180600,  //  0010  CALL	R6	3
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: received_ack
********************************************************************/
be_local_closure(class_Matter_Device_received_ack,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(received_ack),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x8808018A,  //  0000  GETMBR	R2	R0	K138
      0x8C080593,  //  0001  GETMET	R2	R2	K147
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x80040400,  //  0004  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: save_param
********************************************************************/
be_local_closure(class_Matter_Device_save_param,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(save_param),
    &be_const_str_solidified,
    ( &(const binstruction[83]) {  /* code */
      0xA4060A00,  //  0000  IMPORT	R1	K5
      0x8C080194,  //  0001  GETMET	R2	R0	K148
      0x7C080200,  //  0002  CALL	R2	1
      0x60080018,  //  0003  GETGBL	R2	G24
      0x580C0095,  //  0004  LDCONST	R3	K149
      0x88100169,  //  0005  GETMBR	R4	R0	K105
      0x8814016B,  //  0006  GETMBR	R5	R0	K107
      0x88180134,  //  0007  GETMBR	R6	R0	K52
      0x781A0001,  //  0008  JMPF	R6	#000B
      0x58180096,  //  0009  LDCONST	R6	K150
      0x70020000,  //  000A  JMP		#000C
      0x58180097,  //  000B  LDCONST	R6	K151
      0x881C0135,  //  000C  GETMBR	R7	R0	K53
      0x781E0001,  //  000D  JMPF	R7	#0010
      0x581C0096,  //  000E  LDCONST	R7	K150
      0x70020000,  //  000F  JMP		#0011
      0x581C0097,  //  0010  LDCONST	R7	K151
      0x88200132,  //  0011  GETMBR	R8	R0	K50
      0x7C080C00,  //  0012  CALL	R2	6
      0x880C016F,  //  0013  GETMBR	R3	R0	K111
      0x780E0000,  //  0014  JMPF	R3	#0016
      0x00080598,  //  0015  ADD	R2	R2	K152
      0x880C0103,  //  0016  GETMBR	R3	R0	K3
      0x780E000E,  //  0017  JMPF	R3	#0027
      0x00080599,  //  0018  ADD	R2	R2	K153
      0x8C0C039A,  //  0019  GETMET	R3	R1	K154
      0x88140150,  //  001A  GETMBR	R5	R0	K80
      0x7C0C0400,  //  001B  CALL	R3	2
      0x00080403,  //  001C  ADD	R2	R2	R3
      0x600C000C,  //  001D  GETGBL	R3	G12
      0x8810011C,  //  001E  GETMBR	R4	R0	K28
      0x7C0C0200,  //  001F  CALL	R3	1
      0x240C0702,  //  0020  GT	R3	R3	K2
      0x780E0004,  //  0021  JMPF	R3	#0027
      0x0008059B,  //  0022  ADD	R2	R2	K155
      0x8C0C039A,  //  0023  GETMET	R3	R1	K154
      0x8814011C,  //  0024  GETMBR	R5	R0	K28
      0x7C0C0400,  //  0025  CALL	R3	2
      0x00080403,  //  0026  ADD	R2	R2	R3
      0x0008059C,  //  0027  ADD	R2	R2	K156
      0xA8020017,  //  0028  EXBLK	0	#0041
      0x600C0011,  //  0029  GETGBL	R3	G17
      0x88100166,  //  002A  GETMBR	R4	R0	K102
      0x5814009D,  //  002B  LDCONST	R5	K157
      0x7C0C0400,  //  002C  CALL	R3	2
      0x8C10079E,  //  002D  GETMET	R4	R3	K158
      0x5C180400,  //  002E  MOVE	R6	R2
      0x7C100400,  //  002F  CALL	R4	2
      0x8C100768,  //  0030  GETMET	R4	R3	K104
      0x7C100200,  //  0031  CALL	R4	1
      0xB8121400,  //  0032  GETNGBL	R4	K10
      0x60140018,  //  0033  GETGBL	R5	G24
      0x5818009F,  //  0034  LDCONST	R6	K159
      0x881C0103,  //  0035  GETMBR	R7	R0	K3
      0x781E0001,  //  0036  JMPF	R7	#0039
      0x581C00A0,  //  0037  LDCONST	R7	K160
      0x70020000,  //  0038  JMP		#003A
      0x581C0025,  //  0039  LDCONST	R7	K37
      0x7C140400,  //  003A  CALL	R5	2
      0x5818002A,  //  003B  LDCONST	R6	K42
      0x7C100400,  //  003C  CALL	R4	2
      0xA8040001,  //  003D  EXBLK	1	1
      0x80040400,  //  003E  RET	1	R2
      0xA8040001,  //  003F  EXBLK	1	1
      0x70020010,  //  0040  JMP		#0052
      0xAC0C0002,  //  0041  CATCH	R3	0	2
      0x7002000D,  //  0042  JMP		#0051
      0xB8161400,  //  0043  GETNGBL	R5	K10
      0x60180008,  //  0044  GETGBL	R6	G8
      0x5C1C0600,  //  0045  MOVE	R7	R3
      0x7C180200,  //  0046  CALL	R6	1
      0x001B4206,  //  0047  ADD	R6	K161	R6
      0x00180D77,  //  0048  ADD	R6	R6	K119
      0x601C0008,  //  0049  GETGBL	R7	G8
      0x5C200800,  //  004A  MOVE	R8	R4
      0x7C1C0200,  //  004B  CALL	R7	1
      0x00180C07,  //  004C  ADD	R6	R6	R7
      0x581C002A,  //  004D  LDCONST	R7	K42
      0x7C140400,  //  004E  CALL	R5	2
      0x80040400,  //  004F  RET	1	R2
      0x70020000,  //  0050  JMP		#0052
      0xB0080000,  //  0051  RAISE	2	R0	R0
      0x80000000,  //  0052  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: button_handler
********************************************************************/
be_local_closure(class_Matter_Device_button_handler,   /* name */
  be_nested_proto(
    14,                          /* nstack */
    5,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(button_handler),
    &be_const_str_solidified,
    ( &(const binstruction[25]) {  /* code */
      0x58140002,  //  0000  LDCONST	R5	K2
      0xA41B0E00,  //  0001  IMPORT	R6	K135
      0x601C000C,  //  0002  GETGBL	R7	G12
      0x8820010D,  //  0003  GETMBR	R8	R0	K13
      0x7C1C0200,  //  0004  CALL	R7	1
      0x141C0A07,  //  0005  LT	R7	R5	R7
      0x781E0010,  //  0006  JMPF	R7	#0018
      0x881C010D,  //  0007  GETMBR	R7	R0	K13
      0x941C0E05,  //  0008  GETIDX	R7	R7	R5
      0x8C200D51,  //  0009  GETMET	R8	R6	K81
      0x5C280E00,  //  000A  MOVE	R10	R7
      0x582C0029,  //  000B  LDCONST	R11	K41
      0x7C200600,  //  000C  CALL	R8	3
      0x78220007,  //  000D  JMPF	R8	#0016
      0x8820010D,  //  000E  GETMBR	R8	R0	K13
      0x94201005,  //  000F  GETIDX	R8	R8	R5
      0x8C201129,  //  0010  GETMET	R8	R8	K41
      0x5C280200,  //  0011  MOVE	R10	R1
      0x5C2C0400,  //  0012  MOVE	R11	R2
      0x5C300600,  //  0013  MOVE	R12	R3
      0x5C340800,  //  0014  MOVE	R13	R4
      0x7C200A00,  //  0015  CALL	R8	5
      0x00140B0F,  //  0016  ADD	R5	R5	K15
      0x7001FFE9,  //  0017  JMP		#0002
      0x80000000,  //  0018  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update_remotes_info
********************************************************************/
be_local_closure(class_Matter_Device_update_remotes_info,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(update_remotes_info),
    &be_const_str_solidified,
    ( &(const binstruction[33]) {  /* code */
      0x60040013,  //  0000  GETGBL	R1	G19
      0x7C040000,  //  0001  CALL	R1	0
      0x8808017B,  //  0002  GETMBR	R2	R0	K123
      0x4C0C0000,  //  0003  LDNIL	R3
      0x20080403,  //  0004  NE	R2	R2	R3
      0x780A0018,  //  0005  JMPF	R2	#001F
      0x60080010,  //  0006  GETGBL	R2	G16
      0x880C017B,  //  0007  GETMBR	R3	R0	K123
      0x8C0C071F,  //  0008  GETMET	R3	R3	K31
      0x7C0C0200,  //  0009  CALL	R3	1
      0x7C080200,  //  000A  CALL	R2	1
      0xA802000F,  //  000B  EXBLK	0	#001C
      0x5C0C0400,  //  000C  MOVE	R3	R2
      0x7C0C0000,  //  000D  CALL	R3	0
      0x8810017B,  //  000E  GETMBR	R4	R0	K123
      0x94100803,  //  000F  GETIDX	R4	R4	R3
      0x8C1009A2,  //  0010  GETMET	R4	R4	K162
      0x7C100200,  //  0011  CALL	R4	1
      0x4C140000,  //  0012  LDNIL	R5
      0x20140805,  //  0013  NE	R5	R4	R5
      0x78160005,  //  0014  JMPF	R5	#001B
      0x6014000C,  //  0015  GETGBL	R5	G12
      0x5C180800,  //  0016  MOVE	R6	R4
      0x7C140200,  //  0017  CALL	R5	1
      0x24140B02,  //  0018  GT	R5	R5	K2
      0x78160000,  //  0019  JMPF	R5	#001B
      0x98040604,  //  001A  SETIDX	R1	R3	R4
      0x7001FFEF,  //  001B  JMP		#000C
      0x58080021,  //  001C  LDCONST	R2	K33
      0xAC080200,  //  001D  CATCH	R2	1	0
      0xB0080000,  //  001E  RAISE	2	R0	R0
      0x90023801,  //  001F  SETMBR	R0	K28	R1
      0x80040200,  //  0020  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: k2l
********************************************************************/
be_local_closure(class_Matter_Device_k2l,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    12,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(k2l),
    &be_const_str_solidified,
    ( &(const binstruction[50]) {  /* code */
      0x5804001E,  //  0000  LDCONST	R1	K30
      0x60080012,  //  0001  GETGBL	R2	G18
      0x7C080000,  //  0002  CALL	R2	0
      0x4C0C0000,  //  0003  LDNIL	R3
      0x1C0C0003,  //  0004  EQ	R3	R0	R3
      0x780E0000,  //  0005  JMPF	R3	#0007
      0x80040400,  //  0006  RET	1	R2
      0x600C0010,  //  0007  GETGBL	R3	G16
      0x8C10011F,  //  0008  GETMET	R4	R0	K31
      0x7C100200,  //  0009  CALL	R4	1
      0x7C0C0200,  //  000A  CALL	R3	1
      0xA8020005,  //  000B  EXBLK	0	#0012
      0x5C100600,  //  000C  MOVE	R4	R3
      0x7C100000,  //  000D  CALL	R4	0
      0x8C140520,  //  000E  GETMET	R5	R2	K32
      0x5C1C0800,  //  000F  MOVE	R7	R4
      0x7C140400,  //  0010  CALL	R5	2
      0x7001FFF9,  //  0011  JMP		#000C
      0x580C0021,  //  0012  LDCONST	R3	K33
      0xAC0C0200,  //  0013  CATCH	R3	1	0
      0xB0080000,  //  0014  RAISE	2	R0	R0
      0x600C0010,  //  0015  GETGBL	R3	G16
      0x6010000C,  //  0016  GETGBL	R4	G12
      0x5C140400,  //  0017  MOVE	R5	R2
      0x7C100200,  //  0018  CALL	R4	1
      0x0410090F,  //  0019  SUB	R4	R4	K15
      0x40121E04,  //  001A  CONNECT	R4	K15	R4
      0x7C0C0200,  //  001B  CALL	R3	1
      0xA8020010,  //  001C  EXBLK	0	#002E
      0x5C100600,  //  001D  MOVE	R4	R3
      0x7C100000,  //  001E  CALL	R4	0
      0x94140404,  //  001F  GETIDX	R5	R2	R4
      0x5C180800,  //  0020  MOVE	R6	R4
      0x241C0D02,  //  0021  GT	R7	R6	K2
      0x781E0008,  //  0022  JMPF	R7	#002C
      0x041C0D0F,  //  0023  SUB	R7	R6	K15
      0x941C0407,  //  0024  GETIDX	R7	R2	R7
      0x241C0E05,  //  0025  GT	R7	R7	R5
      0x781E0004,  //  0026  JMPF	R7	#002C
      0x041C0D0F,  //  0027  SUB	R7	R6	K15
      0x941C0407,  //  0028  GETIDX	R7	R2	R7
      0x98080C07,  //  0029  SETIDX	R2	R6	R7
      0x04180D0F,  //  002A  SUB	R6	R6	K15
      0x7001FFF4,  //  002B  JMP		#0021
      0x98080C05,  //  002C  SETIDX	R2	R6	R5
      0x7001FFEE,  //  002D  JMP		#001D
      0x580C0021,  //  002E  LDCONST	R3	K33
      0xAC0C0200,  //  002F  CATCH	R3	1	0
      0xB0080000,  //  0030  RAISE	2	R0	R0
      0x80040400,  //  0031  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: every_second
********************************************************************/
be_local_closure(class_Matter_Device_every_second,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(every_second),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C0403A3,  //  0001  GETMET	R1	R1	K163
      0x7C040200,  //  0002  CALL	R1	1
      0x88040123,  //  0003  GETMBR	R1	R0	K35
      0x8C0403A3,  //  0004  GETMET	R1	R1	K163
      0x7C040200,  //  0005  CALL	R1	1
      0x8804013C,  //  0006  GETMBR	R1	R0	K60
      0x8C0403A3,  //  0007  GETMET	R1	R1	K163
      0x7C040200,  //  0008  CALL	R1	1
      0x88040136,  //  0009  GETMBR	R1	R0	K54
      0x8C0403A3,  //  000A  GETMET	R1	R1	K163
      0x7C040200,  //  000B  CALL	R1	1
      0x80000000,  //  000C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: adjust_next_ep
********************************************************************/
be_local_closure(class_Matter_Device_adjust_next_ep,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(adjust_next_ep),
    &be_const_str_solidified,
    ( &(const binstruction[21]) {  /* code */
      0x60040010,  //  0000  GETGBL	R1	G16
      0x88080150,  //  0001  GETMBR	R2	R0	K80
      0x8C08051F,  //  0002  GETMET	R2	R2	K31
      0x7C080200,  //  0003  CALL	R2	1
      0x7C040200,  //  0004  CALL	R1	1
      0xA802000A,  //  0005  EXBLK	0	#0011
      0x5C080200,  //  0006  MOVE	R2	R1
      0x7C080000,  //  0007  CALL	R2	0
      0x600C0009,  //  0008  GETGBL	R3	G9
      0x5C100400,  //  0009  MOVE	R4	R2
      0x7C0C0200,  //  000A  CALL	R3	1
      0x88100132,  //  000B  GETMBR	R4	R0	K50
      0x28100604,  //  000C  GE	R4	R3	R4
      0x78120001,  //  000D  JMPF	R4	#0010
      0x0010070F,  //  000E  ADD	R4	R3	K15
      0x90026404,  //  000F  SETMBR	R0	K50	R4
      0x7001FFF4,  //  0010  JMP		#0006
      0x58040021,  //  0011  LDCONST	R1	K33
      0xAC040200,  //  0012  CATCH	R1	1	0
      0xB0080000,  //  0013  RAISE	2	R0	R0
      0x80000000,  //  0014  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: stop
********************************************************************/
be_local_closure(class_Matter_Device_stop,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(stop),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0xB8060C00,  //  0000  GETNGBL	R1	K6
      0x8C0403A4,  //  0001  GETMET	R1	R1	K164
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x8804018A,  //  0004  GETMBR	R1	R0	K138
      0x78060002,  //  0005  JMPF	R1	#0009
      0x8804018A,  //  0006  GETMBR	R1	R0	K138
      0x8C0403A5,  //  0007  GETMET	R1	R1	K165
      0x7C040200,  //  0008  CALL	R1	1
      0x80000000,  //  0009  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_plugin_class_displayname
********************************************************************/
be_local_closure(class_Matter_Device_get_plugin_class_displayname,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(get_plugin_class_displayname),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x880801A6,  //  0000  GETMBR	R2	R0	K166
      0x8C08051D,  //  0001  GETMET	R2	R2	K29
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x780A0001,  //  0004  JMPF	R2	#0007
      0x880C05A7,  //  0005  GETMBR	R3	R2	K167
      0x70020000,  //  0006  JMP		#0008
      0x580C0025,  //  0007  LDCONST	R3	K37
      0x80040600,  //  0008  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: MtrInfo_one
********************************************************************/
be_local_closure(class_Matter_Device_MtrInfo_one,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(MtrInfo_one),
    &be_const_str_solidified,
    ( &(const binstruction[20]) {  /* code */
      0x8C0801A8,  //  0000  GETMET	R2	R0	K168
      0x5C100200,  //  0001  MOVE	R4	R1
      0x7C080400,  //  0002  CALL	R2	2
      0x4C0C0000,  //  0003  LDNIL	R3
      0x1C0C0403,  //  0004  EQ	R3	R2	R3
      0x780E0000,  //  0005  JMPF	R3	#0007
      0x80000600,  //  0006  RET	0
      0x8C0C05A9,  //  0007  GETMET	R3	R2	K169
      0x7C0C0200,  //  0008  CALL	R3	1
      0x780E0008,  //  0009  JMPF	R3	#0013
      0x60100018,  //  000A  GETGBL	R4	G24
      0x581400AA,  //  000B  LDCONST	R5	K170
      0x5C180600,  //  000C  MOVE	R6	R3
      0x7C100400,  //  000D  CALL	R4	2
      0xB8160C00,  //  000E  GETNGBL	R5	K6
      0x8C140BAB,  //  000F  GETMET	R5	R5	K171
      0x5C1C0800,  //  0010  MOVE	R7	R4
      0x58200025,  //  0011  LDCONST	R8	K37
      0x7C140600,  //  0012  CALL	R5	3
      0x80000000,  //  0013  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: bridge_add_endpoint
********************************************************************/
be_local_closure(class_Matter_Device_bridge_add_endpoint,   /* name */
  be_nested_proto(
    16,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(bridge_add_endpoint),
    &be_const_str_solidified,
    ( &(const binstruction[68]) {  /* code */
      0x880C01A6,  //  0000  GETMBR	R3	R0	K166
      0x8C0C071D,  //  0001  GETMET	R3	R3	K29
      0x5C140200,  //  0002  MOVE	R5	R1
      0x7C0C0400,  //  0003  CALL	R3	2
      0x4C100000,  //  0004  LDNIL	R4
      0x1C100604,  //  0005  EQ	R4	R3	R4
      0x78120008,  //  0006  JMPF	R4	#0010
      0xB8121400,  //  0007  GETNGBL	R4	K10
      0x60140008,  //  0008  GETGBL	R5	G8
      0x5C180200,  //  0009  MOVE	R6	R1
      0x7C140200,  //  000A  CALL	R5	1
      0x00175805,  //  000B  ADD	R5	K172	R5
      0x00140BAD,  //  000C  ADD	R5	R5	K173
      0x58180009,  //  000D  LDCONST	R6	K9
      0x7C100400,  //  000E  CALL	R4	2
      0x80000800,  //  000F  RET	0
      0x88100132,  //  0010  GETMBR	R4	R0	K50
      0x60140008,  //  0011  GETGBL	R5	G8
      0x5C180800,  //  0012  MOVE	R6	R4
      0x7C140200,  //  0013  CALL	R5	1
      0x5C180600,  //  0014  MOVE	R6	R3
      0x5C1C0000,  //  0015  MOVE	R7	R0
      0x5C200800,  //  0016  MOVE	R8	R4
      0x5C240400,  //  0017  MOVE	R9	R2
      0x7C180600,  //  0018  CALL	R6	3
      0x881C010D,  //  0019  GETMBR	R7	R0	K13
      0x8C1C0F20,  //  001A  GETMET	R7	R7	K32
      0x5C240C00,  //  001B  MOVE	R9	R6
      0x7C1C0400,  //  001C  CALL	R7	2
      0x601C0013,  //  001D  GETGBL	R7	G19
      0x7C1C0000,  //  001E  CALL	R7	0
      0x981E4E01,  //  001F  SETIDX	R7	K39	R1
      0x60200010,  //  0020  GETGBL	R8	G16
      0x8C24051F,  //  0021  GETMET	R9	R2	K31
      0x7C240200,  //  0022  CALL	R9	1
      0x7C200200,  //  0023  CALL	R8	1
      0xA8020004,  //  0024  EXBLK	0	#002A
      0x5C241000,  //  0025  MOVE	R9	R8
      0x7C240000,  //  0026  CALL	R9	0
      0x94280409,  //  0027  GETIDX	R10	R2	R9
      0x981C120A,  //  0028  SETIDX	R7	R9	R10
      0x7001FFFA,  //  0029  JMP		#0025
      0x58200021,  //  002A  LDCONST	R8	K33
      0xAC200200,  //  002B  CATCH	R8	1	0
      0xB0080000,  //  002C  RAISE	2	R0	R0
      0xB8221400,  //  002D  GETNGBL	R8	K10
      0x60240018,  //  002E  GETGBL	R9	G24
      0x582800AE,  //  002F  LDCONST	R10	K174
      0x5C2C0800,  //  0030  MOVE	R11	R4
      0x5C300200,  //  0031  MOVE	R12	R1
      0x8C3401AF,  //  0032  GETMET	R13	R0	K175
      0x5C3C0400,  //  0033  MOVE	R15	R2
      0x7C340400,  //  0034  CALL	R13	2
      0x7C240800,  //  0035  CALL	R9	4
      0x5828002A,  //  0036  LDCONST	R10	K42
      0x7C200400,  //  0037  CALL	R8	2
      0x88200150,  //  0038  GETMBR	R8	R0	K80
      0x98200A07,  //  0039  SETIDX	R8	R5	R7
      0x50200200,  //  003A  LDBOOL	R8	1	0
      0x90020608,  //  003B  SETMBR	R0	K3	R8
      0x88200132,  //  003C  GETMBR	R8	R0	K50
      0x0020110F,  //  003D  ADD	R8	R8	K15
      0x90026408,  //  003E  SETMBR	R0	K50	R8
      0x8C200104,  //  003F  GETMET	R8	R0	K4
      0x7C200200,  //  0040  CALL	R8	1
      0x8C200156,  //  0041  GETMET	R8	R0	K86
      0x7C200200,  //  0042  CALL	R8	1
      0x80040800,  //  0043  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: resolve_attribute_read_solo
********************************************************************/
be_local_closure(class_Matter_Device_resolve_attribute_read_solo,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(resolve_attribute_read_solo),
    &be_const_str_solidified,
    ( &(const binstruction[47]) {  /* code */
      0x88080311,  //  0000  GETMBR	R2	R1	K17
      0x880C0390,  //  0001  GETMBR	R3	R1	K144
      0x88100391,  //  0002  GETMBR	R4	R1	K145
      0x4C140000,  //  0003  LDNIL	R5
      0x1C140405,  //  0004  EQ	R5	R2	R5
      0x74160005,  //  0005  JMPT	R5	#000C
      0x4C140000,  //  0006  LDNIL	R5
      0x1C140605,  //  0007  EQ	R5	R3	R5
      0x74160002,  //  0008  JMPT	R5	#000C
      0x4C140000,  //  0009  LDNIL	R5
      0x1C140805,  //  000A  EQ	R5	R4	R5
      0x78160001,  //  000B  JMPF	R5	#000E
      0x4C140000,  //  000C  LDNIL	R5
      0x80040A00,  //  000D  RET	1	R5
      0x8C1401A8,  //  000E  GETMET	R5	R0	K168
      0x5C1C0400,  //  000F  MOVE	R7	R2
      0x7C140400,  //  0010  CALL	R5	2
      0x4C180000,  //  0011  LDNIL	R6
      0x1C180A06,  //  0012  EQ	R6	R5	R6
      0x781A0005,  //  0013  JMPF	R6	#001A
      0xB81A2800,  //  0014  GETNGBL	R6	K20
      0x88180D15,  //  0015  GETMBR	R6	R6	K21
      0x90062606,  //  0016  SETMBR	R1	K19	R6
      0x4C180000,  //  0017  LDNIL	R6
      0x80040C00,  //  0018  RET	1	R6
      0x70020013,  //  0019  JMP		#002E
      0x8C180BB0,  //  001A  GETMET	R6	R5	K176
      0x5C200600,  //  001B  MOVE	R8	R3
      0x7C180400,  //  001C  CALL	R6	2
      0x741A0005,  //  001D  JMPT	R6	#0024
      0xB81A2800,  //  001E  GETNGBL	R6	K20
      0x88180DB1,  //  001F  GETMBR	R6	R6	K177
      0x90062606,  //  0020  SETMBR	R1	K19	R6
      0x4C180000,  //  0021  LDNIL	R6
      0x80040C00,  //  0022  RET	1	R6
      0x70020009,  //  0023  JMP		#002E
      0x8C180BB2,  //  0024  GETMET	R6	R5	K178
      0x5C200600,  //  0025  MOVE	R8	R3
      0x5C240800,  //  0026  MOVE	R9	R4
      0x7C180600,  //  0027  CALL	R6	3
      0x741A0004,  //  0028  JMPT	R6	#002E
      0xB81A2800,  //  0029  GETNGBL	R6	K20
      0x88180DB3,  //  002A  GETMBR	R6	R6	K179
      0x90062606,  //  002B  SETMBR	R1	K19	R6
      0x4C180000,  //  002C  LDNIL	R6
      0x80040C00,  //  002D  RET	1	R6
      0x80040A00,  //  002E  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: msg_send
********************************************************************/
be_local_closure(class_Matter_Device_msg_send,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(msg_send),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x8808018A,  //  0000  GETMBR	R2	R0	K138
      0x8C0805B4,  //  0001  GETMET	R2	R2	K180
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x80040400,  //  0004  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: clean_remotes
********************************************************************/
be_local_closure(class_Matter_Device_clean_remotes,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(clean_remotes),
    &be_const_str_solidified,
    ( &(const binstruction[80]) {  /* code */
      0xA4070E00,  //  0000  IMPORT	R1	K135
      0x8808017B,  //  0001  GETMBR	R2	R0	K123
      0x780A004B,  //  0002  JMPF	R2	#004F
      0x60080013,  //  0003  GETGBL	R2	G19
      0x7C080000,  //  0004  CALL	R2	0
      0x600C0010,  //  0005  GETGBL	R3	G16
      0x8810017B,  //  0006  GETMBR	R4	R0	K123
      0x7C0C0200,  //  0007  CALL	R3	1
      0xA8020003,  //  0008  EXBLK	0	#000D
      0x5C100600,  //  0009  MOVE	R4	R3
      0x7C100000,  //  000A  CALL	R4	0
      0x98080902,  //  000B  SETIDX	R2	R4	K2
      0x7001FFFB,  //  000C  JMP		#0009
      0x580C0021,  //  000D  LDCONST	R3	K33
      0xAC0C0200,  //  000E  CATCH	R3	1	0
      0xB0080000,  //  000F  RAISE	2	R0	R0
      0x600C0010,  //  0010  GETGBL	R3	G16
      0x8810010D,  //  0011  GETMBR	R4	R0	K13
      0x7C0C0200,  //  0012  CALL	R3	1
      0xA802000F,  //  0013  EXBLK	0	#0024
      0x5C100600,  //  0014  MOVE	R4	R3
      0x7C100000,  //  0015  CALL	R4	0
      0x8C140379,  //  0016  GETMET	R5	R1	K121
      0x5C1C0800,  //  0017  MOVE	R7	R4
      0x582000B5,  //  0018  LDCONST	R8	K181
      0x7C140600,  //  0019  CALL	R5	3
      0x4C180000,  //  001A  LDNIL	R6
      0x20180A06,  //  001B  NE	R6	R5	R6
      0x781A0005,  //  001C  JMPF	R6	#0023
      0x8C18051D,  //  001D  GETMET	R6	R2	K29
      0x5C200A00,  //  001E  MOVE	R8	R5
      0x58240002,  //  001F  LDCONST	R9	K2
      0x7C180600,  //  0020  CALL	R6	3
      0x00180D0F,  //  0021  ADD	R6	R6	K15
      0x98080A06,  //  0022  SETIDX	R2	R5	R6
      0x7001FFEF,  //  0023  JMP		#0014
      0x580C0021,  //  0024  LDCONST	R3	K33
      0xAC0C0200,  //  0025  CATCH	R3	1	0
      0xB0080000,  //  0026  RAISE	2	R0	R0
      0x600C0012,  //  0027  GETGBL	R3	G18
      0x7C0C0000,  //  0028  CALL	R3	0
      0x60100010,  //  0029  GETGBL	R4	G16
      0x8C14051F,  //  002A  GETMET	R5	R2	K31
      0x7C140200,  //  002B  CALL	R5	1
      0x7C100200,  //  002C  CALL	R4	1
      0xA8020008,  //  002D  EXBLK	0	#0037
      0x5C140800,  //  002E  MOVE	R5	R4
      0x7C140000,  //  002F  CALL	R5	0
      0x94180405,  //  0030  GETIDX	R6	R2	R5
      0x1C180D02,  //  0031  EQ	R6	R6	K2
      0x781A0002,  //  0032  JMPF	R6	#0036
      0x8C180720,  //  0033  GETMET	R6	R3	K32
      0x5C200A00,  //  0034  MOVE	R8	R5
      0x7C180400,  //  0035  CALL	R6	2
      0x7001FFF6,  //  0036  JMP		#002E
      0x58100021,  //  0037  LDCONST	R4	K33
      0xAC100200,  //  0038  CATCH	R4	1	0
      0xB0080000,  //  0039  RAISE	2	R0	R0
      0x60100010,  //  003A  GETGBL	R4	G16
      0x5C140600,  //  003B  MOVE	R5	R3
      0x7C100200,  //  003C  CALL	R4	1
      0xA802000D,  //  003D  EXBLK	0	#004C
      0x5C140800,  //  003E  MOVE	R5	R4
      0x7C140000,  //  003F  CALL	R5	0
      0xB81A1400,  //  0040  GETNGBL	R6	K10
      0x881C0BB7,  //  0041  GETMBR	R7	R5	K183
      0x001F6C07,  //  0042  ADD	R7	K182	R7
      0x58200009,  //  0043  LDCONST	R8	K9
      0x7C180400,  //  0044  CALL	R6	2
      0x8C180B68,  //  0045  GETMET	R6	R5	K104
      0x7C180200,  //  0046  CALL	R6	1
      0x8818017B,  //  0047  GETMBR	R6	R0	K123
      0x8C180D44,  //  0048  GETMET	R6	R6	K68
      0x88200BB7,  //  0049  GETMBR	R8	R5	K183
      0x7C180400,  //  004A  CALL	R6	2
      0x7001FFF1,  //  004B  JMP		#003E
      0x58100021,  //  004C  LDCONST	R4	K33
      0xAC100200,  //  004D  CATCH	R4	1	0
      0xB0080000,  //  004E  RAISE	2	R0	R0
      0x80000000,  //  004F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: MtrUpdate
********************************************************************/
be_local_closure(class_Matter_Device_MtrUpdate,   /* name */
  be_nested_proto(
    18,                          /* nstack */
    5,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(MtrUpdate),
    &be_const_str_solidified,
    ( &(const binstruction[126]) {  /* code */
      0x4C140000,  //  0000  LDNIL	R5
      0x1C140805,  //  0001  EQ	R5	R4	R5
      0x78160004,  //  0002  JMPF	R5	#0008
      0xB8160C00,  //  0003  GETNGBL	R5	K6
      0x8C140BB8,  //  0004  GETMET	R5	R5	K184
      0x581C00B9,  //  0005  LDCONST	R7	K185
      0x7C140400,  //  0006  CALL	R5	2
      0x80040A00,  //  0007  RET	1	R5
      0xB8160C00,  //  0008  GETNGBL	R5	K6
      0x8C140BBA,  //  0009  GETMET	R5	R5	K186
      0x5C1C0800,  //  000A  MOVE	R7	R4
      0x582000BB,  //  000B  LDCONST	R8	K187
      0x7C140600,  //  000C  CALL	R5	3
      0xB81A0C00,  //  000D  GETNGBL	R6	K6
      0x8C180DBA,  //  000E  GETMET	R6	R6	K186
      0x5C200800,  //  000F  MOVE	R8	R4
      0x582400BC,  //  0010  LDCONST	R9	K188
      0x7C180600,  //  0011  CALL	R6	3
      0x74160000,  //  0012  JMPT	R5	#0014
      0x781A0064,  //  0013  JMPF	R6	#0079
      0x4C1C0000,  //  0014  LDNIL	R7
      0x78160010,  //  0015  JMPF	R5	#0027
      0x60200009,  //  0016  GETGBL	R8	G9
      0x94240805,  //  0017  GETIDX	R9	R4	R5
      0x7C200200,  //  0018  CALL	R8	1
      0x18241102,  //  0019  LE	R9	R8	K2
      0x78260004,  //  001A  JMPF	R9	#0020
      0xB8260C00,  //  001B  GETNGBL	R9	K6
      0x8C2413B8,  //  001C  GETMET	R9	R9	K184
      0x582C00BD,  //  001D  LDCONST	R11	K189
      0x7C240400,  //  001E  CALL	R9	2
      0x80041200,  //  001F  RET	1	R9
      0x8C2401A8,  //  0020  GETMET	R9	R0	K168
      0x5C2C1000,  //  0021  MOVE	R11	R8
      0x7C240400,  //  0022  CALL	R9	2
      0x5C1C1200,  //  0023  MOVE	R7	R9
      0x8C240944,  //  0024  GETMET	R9	R4	K68
      0x5C2C0A00,  //  0025  MOVE	R11	R5
      0x7C240400,  //  0026  CALL	R9	2
      0x781A0009,  //  0027  JMPF	R6	#0032
      0x4C200000,  //  0028  LDNIL	R8
      0x1C200E08,  //  0029  EQ	R8	R7	R8
      0x78220003,  //  002A  JMPF	R8	#002F
      0x8C200163,  //  002B  GETMET	R8	R0	K99
      0x94280806,  //  002C  GETIDX	R10	R4	R6
      0x7C200400,  //  002D  CALL	R8	2
      0x5C1C1000,  //  002E  MOVE	R7	R8
      0x8C200944,  //  002F  GETMET	R8	R4	K68
      0x5C280C00,  //  0030  MOVE	R10	R6
      0x7C200400,  //  0031  CALL	R8	2
      0x4C200000,  //  0032  LDNIL	R8
      0x1C200E08,  //  0033  EQ	R8	R7	R8
      0x78220004,  //  0034  JMPF	R8	#003A
      0xB8220C00,  //  0035  GETNGBL	R8	K6
      0x8C2011B8,  //  0036  GETMET	R8	R8	K184
      0x582800BE,  //  0037  LDCONST	R10	K190
      0x7C200400,  //  0038  CALL	R8	2
      0x80041000,  //  0039  RET	1	R8
      0x88200FBF,  //  003A  GETMBR	R8	R7	K191
      0x74220004,  //  003B  JMPT	R8	#0041
      0xB8220C00,  //  003C  GETNGBL	R8	K6
      0x8C2011B8,  //  003D  GETMET	R8	R8	K184
      0x582800C0,  //  003E  LDCONST	R10	K192
      0x7C200400,  //  003F  CALL	R8	2
      0x80041000,  //  0040  RET	1	R8
      0x8C200FC1,  //  0041  GETMET	R8	R7	K193
      0x7C200200,  //  0042  CALL	R8	1
      0x60240013,  //  0043  GETGBL	R9	G19
      0x7C240000,  //  0044  CALL	R9	0
      0x60280010,  //  0045  GETGBL	R10	G16
      0x8C2C091F,  //  0046  GETMET	R11	R4	K31
      0x7C2C0200,  //  0047  CALL	R11	1
      0x7C280200,  //  0048  CALL	R10	1
      0xA8020016,  //  0049  EXBLK	0	#0061
      0x5C2C1400,  //  004A  MOVE	R11	R10
      0x7C2C0000,  //  004B  CALL	R11	0
      0xB8320C00,  //  004C  GETNGBL	R12	K6
      0x8C3019C2,  //  004D  GETMET	R12	R12	K194
      0x5C381000,  //  004E  MOVE	R14	R8
      0x5C3C1600,  //  004F  MOVE	R15	R11
      0x7C300600,  //  0050  CALL	R12	3
      0x4C340000,  //  0051  LDNIL	R13
      0x1C34180D,  //  0052  EQ	R13	R12	R13
      0x78360008,  //  0053  JMPF	R13	#005D
      0xB8360C00,  //  0054  GETNGBL	R13	K6
      0x8C341BB8,  //  0055  GETMET	R13	R13	K184
      0x603C0018,  //  0056  GETGBL	R15	G24
      0x584000C3,  //  0057  LDCONST	R16	K195
      0x5C441600,  //  0058  MOVE	R17	R11
      0x7C3C0400,  //  0059  CALL	R15	2
      0x7C340400,  //  005A  CALL	R13	2
      0xA8040001,  //  005B  EXBLK	1	1
      0x80001A00,  //  005C  RET	0
      0x9434100C,  //  005D  GETIDX	R13	R8	R12
      0x9438080B,  //  005E  GETIDX	R14	R4	R11
      0x98241A0E,  //  005F  SETIDX	R9	R13	R14
      0x7001FFE8,  //  0060  JMP		#004A
      0x58280021,  //  0061  LDCONST	R10	K33
      0xAC280200,  //  0062  CATCH	R10	1	0
      0xB0080000,  //  0063  RAISE	2	R0	R0
      0x8C280FC4,  //  0064  GETMET	R10	R7	K196
      0x5C301200,  //  0065  MOVE	R12	R9
      0x7C280400,  //  0066  CALL	R10	2
      0x8C280FA9,  //  0067  GETMET	R10	R7	K169
      0x7C280200,  //  0068  CALL	R10	1
      0x782A000A,  //  0069  JMPF	R10	#0075
      0x602C0018,  //  006A  GETGBL	R11	G24
      0x583000C5,  //  006B  LDCONST	R12	K197
      0x5C340200,  //  006C  MOVE	R13	R1
      0x5C381400,  //  006D  MOVE	R14	R10
      0x7C2C0600,  //  006E  CALL	R11	3
      0xB8320C00,  //  006F  GETNGBL	R12	K6
      0x8C3019C6,  //  0070  GETMET	R12	R12	K198
      0x5C381600,  //  0071  MOVE	R14	R11
      0x7C300400,  //  0072  CALL	R12	2
      0x80041800,  //  0073  RET	1	R12
      0x70020003,  //  0074  JMP		#0079
      0xB82E0C00,  //  0075  GETNGBL	R11	K6
      0x8C2C1764,  //  0076  GETMET	R11	R11	K100
      0x7C2C0200,  //  0077  CALL	R11	1
      0x80041600,  //  0078  RET	1	R11
      0xB81E0C00,  //  0079  GETNGBL	R7	K6
      0x8C1C0FB8,  //  007A  GETMET	R7	R7	K184
      0x582400C7,  //  007B  LDCONST	R9	K199
      0x7C1C0400,  //  007C  CALL	R7	2
      0x80000000,  //  007D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: button_pressed
********************************************************************/
be_local_closure(class_Matter_Device_button_pressed,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(button_pressed),
    &be_const_str_solidified,
    ( &(const binstruction[28]) {  /* code */
      0x540E000F,  //  0000  LDINT	R3	16
      0x3C0C0403,  //  0001  SHR	R3	R2	R3
      0x541200FE,  //  0002  LDINT	R4	255
      0x2C0C0604,  //  0003  AND	R3	R3	R4
      0x54120007,  //  0004  LDINT	R4	8
      0x3C100404,  //  0005  SHR	R4	R2	R4
      0x541600FE,  //  0006  LDINT	R5	255
      0x2C100805,  //  0007  AND	R4	R4	R5
      0x541600FE,  //  0008  LDINT	R5	255
      0x2C140405,  //  0009  AND	R5	R2	R5
      0x541A0017,  //  000A  LDINT	R6	24
      0x3C180406,  //  000B  SHR	R6	R2	R6
      0x541E00FE,  //  000C  LDINT	R7	255
      0x2C180C07,  //  000D  AND	R6	R6	R7
      0x8C1C0129,  //  000E  GETMET	R7	R0	K41
      0x00240B0F,  //  000F  ADD	R9	R5	K15
      0x20280604,  //  0010  NE	R10	R3	R4
      0x782A0001,  //  0011  JMPF	R10	#0014
      0x5828000F,  //  0012  LDCONST	R10	K15
      0x70020000,  //  0013  JMP		#0015
      0x58280002,  //  0014  LDCONST	R10	K2
      0x780E0001,  //  0015  JMPF	R3	#0018
      0x582C0002,  //  0016  LDCONST	R11	K2
      0x70020000,  //  0017  JMP		#0019
      0x582C000F,  //  0018  LDCONST	R11	K15
      0x5C300C00,  //  0019  MOVE	R12	R6
      0x7C1C0A00,  //  001A  CALL	R7	5
      0x80000000,  //  001B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: read_sensors_scheduler
********************************************************************/
be_local_closure(class_Matter_Device_read_sensors_scheduler,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(read_sensors_scheduler),
    &be_const_str_solidified,
    ( &(const binstruction[21]) {  /* code */
      0x88040116,  //  0000  GETMBR	R1	R0	K22
      0x4C080000,  //  0001  LDNIL	R2
      0x1C040202,  //  0002  EQ	R1	R1	R2
      0x78060000,  //  0003  JMPF	R1	#0005
      0x80000200,  //  0004  RET	0
      0x88040117,  //  0005  GETMBR	R1	R0	K23
      0x1C040302,  //  0006  EQ	R1	R1	K2
      0x74060004,  //  0007  JMPT	R1	#000D
      0xB8060C00,  //  0008  GETNGBL	R1	K6
      0x8C0403C8,  //  0009  GETMET	R1	R1	K200
      0x880C0117,  //  000A  GETMBR	R3	R0	K23
      0x7C040400,  //  000B  CALL	R1	2
      0x78060006,  //  000C  JMPF	R1	#0014
      0x8C0401C9,  //  000D  GETMET	R1	R0	K201
      0x7C040200,  //  000E  CALL	R1	1
      0xB8060C00,  //  000F  GETNGBL	R1	K6
      0x8C0403CA,  //  0010  GETMET	R1	R1	K202
      0x880C0116,  //  0011  GETMBR	R3	R0	K22
      0x7C040400,  //  0012  CALL	R1	2
      0x90022E01,  //  0013  SETMBR	R0	K23	R1
      0x80000000,  //  0014  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: create_zb_mapper
********************************************************************/
be_local_closure(class_Matter_Device_create_zb_mapper,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(create_zb_mapper),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x8808013E,  //  0000  GETMBR	R2	R0	K62
      0x780A0004,  //  0001  JMPF	R2	#0007
      0x8808013E,  //  0002  GETMBR	R2	R0	K62
      0x8C0805CB,  //  0003  GETMET	R2	R2	K203
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x80040400,  //  0006  RET	1	R2
      0x80000000,  //  0007  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_attribute_expansion
********************************************************************/
be_local_closure(class_Matter_Device_process_attribute_expansion,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(process_attribute_expansion),
    &be_const_str_solidified,
    ( &(const binstruction[28]) {  /* code */
      0x880C0311,  //  0000  GETMBR	R3	R1	K17
      0x88100390,  //  0001  GETMBR	R4	R1	K144
      0x88140391,  //  0002  GETMBR	R5	R1	K145
      0xB81A2800,  //  0003  GETNGBL	R6	K20
      0x8C180DCC,  //  0004  GETMET	R6	R6	K204
      0x5C200000,  //  0005  MOVE	R8	R0
      0x7C180400,  //  0006  CALL	R6	2
      0x8C1C0D8D,  //  0007  GETMET	R7	R6	K141
      0x5C240600,  //  0008  MOVE	R9	R3
      0x5C280800,  //  0009  MOVE	R10	R4
      0x5C2C0A00,  //  000A  MOVE	R11	R5
      0x7C1C0800,  //  000B  CALL	R7	4
      0x8C1C0DCD,  //  000C  GETMET	R7	R6	K205
      0x7C1C0200,  //  000D  CALL	R7	1
      0x4C200000,  //  000E  LDNIL	R8
      0x8C240DCE,  //  000F  GETMET	R9	R6	K206
      0x7C240200,  //  0010  CALL	R9	1
      0x5C201200,  //  0011  MOVE	R8	R9
      0x4C280000,  //  0012  LDNIL	R10
      0x2024120A,  //  0013  NE	R9	R9	R10
      0x78260005,  //  0014  JMPF	R9	#001B
      0x5C240400,  //  0015  MOVE	R9	R2
      0x8C280DCF,  //  0016  GETMET	R10	R6	K207
      0x7C280200,  //  0017  CALL	R10	1
      0x5C2C1000,  //  0018  MOVE	R11	R8
      0x7C240400,  //  0019  CALL	R9	2
      0x7001FFF3,  //  001A  JMP		#000F
      0x80000000,  //  001B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: check_config_ep
********************************************************************/
be_local_closure(class_Matter_Device_check_config_ep,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(check_config_ep),
    &be_const_str_solidified,
    ( &(const binstruction[63]) {  /* code */
      0x50040000,  //  0000  LDBOOL	R1	0	0
      0x60080012,  //  0001  GETGBL	R2	G18
      0x7C080000,  //  0002  CALL	R2	0
      0x600C0010,  //  0003  GETGBL	R3	G16
      0x88100150,  //  0004  GETMBR	R4	R0	K80
      0x8C10091F,  //  0005  GETMET	R4	R4	K31
      0x7C100200,  //  0006  CALL	R4	1
      0x7C0C0200,  //  0007  CALL	R3	1
      0xA8020007,  //  0008  EXBLK	0	#0011
      0x5C100600,  //  0009  MOVE	R4	R3
      0x7C100000,  //  000A  CALL	R4	0
      0x8C140520,  //  000B  GETMET	R5	R2	K32
      0x601C0009,  //  000C  GETGBL	R7	G9
      0x5C200800,  //  000D  MOVE	R8	R4
      0x7C1C0200,  //  000E  CALL	R7	1
      0x7C140400,  //  000F  CALL	R5	2
      0x7001FFF7,  //  0010  JMP		#0009
      0x580C0021,  //  0011  LDCONST	R3	K33
      0xAC0C0200,  //  0012  CATCH	R3	1	0
      0xB0080000,  //  0013  RAISE	2	R0	R0
      0x600C0010,  //  0014  GETGBL	R3	G16
      0x5C100400,  //  0015  MOVE	R4	R2
      0x7C0C0200,  //  0016  CALL	R3	1
      0xA8020022,  //  0017  EXBLK	0	#003B
      0x5C100600,  //  0018  MOVE	R4	R3
      0x7C100000,  //  0019  CALL	R4	0
      0xB8162800,  //  001A  GETNGBL	R5	K20
      0x88140B1A,  //  001B  GETMBR	R5	R5	K26
      0x1C140805,  //  001C  EQ	R5	R4	R5
      0x7816001B,  //  001D  JMPF	R5	#003A
      0x50040200,  //  001E  LDBOOL	R1	1	0
      0xB8161400,  //  001F  GETNGBL	R5	K10
      0x60180018,  //  0020  GETGBL	R6	G24
      0x581C00D0,  //  0021  LDCONST	R7	K208
      0x5C200800,  //  0022  MOVE	R8	R4
      0x88240132,  //  0023  GETMBR	R9	R0	K50
      0x7C180600,  //  0024  CALL	R6	3
      0x581C002A,  //  0025  LDCONST	R7	K42
      0x7C140400,  //  0026  CALL	R5	2
      0x60140008,  //  0027  GETGBL	R5	G8
      0x88180132,  //  0028  GETMBR	R6	R0	K50
      0x7C140200,  //  0029  CALL	R5	1
      0x88180150,  //  002A  GETMBR	R6	R0	K80
      0x601C0008,  //  002B  GETGBL	R7	G8
      0x5C200800,  //  002C  MOVE	R8	R4
      0x7C1C0200,  //  002D  CALL	R7	1
      0x88200150,  //  002E  GETMBR	R8	R0	K80
      0x941C1007,  //  002F  GETIDX	R7	R8	R7
      0x98180A07,  //  0030  SETIDX	R6	R5	R7
      0x88140150,  //  0031  GETMBR	R5	R0	K80
      0x8C140B44,  //  0032  GETMET	R5	R5	K68
      0x601C0008,  //  0033  GETGBL	R7	G8
      0x5C200800,  //  0034  MOVE	R8	R4
      0x7C1C0200,  //  0035  CALL	R7	1
      0x7C140400,  //  0036  CALL	R5	2
      0x88140132,  //  0037  GETMBR	R5	R0	K50
      0x00140B0F,  //  0038  ADD	R5	R5	K15
      0x90026405,  //  0039  SETMBR	R0	K50	R5
      0x7001FFDC,  //  003A  JMP		#0018
      0x580C0021,  //  003B  LDCONST	R3	K33
      0xAC0C0200,  //  003C  CATCH	R3	1	0
      0xB0080000,  //  003D  RAISE	2	R0	R0
      0x80040200,  //  003E  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init_zigbee
********************************************************************/
be_local_closure(class_Matter_Device_init_zigbee,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(init_zigbee),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x8C0401D1,  //  0000  GETMET	R1	R0	K209
      0x7C040200,  //  0001  CALL	R1	1
      0x78060004,  //  0002  JMPF	R1	#0008
      0xA4071200,  //  0003  IMPORT	R1	K137
      0x5C080200,  //  0004  MOVE	R2	R1
      0x5C0C0000,  //  0005  MOVE	R3	R0
      0x7C080200,  //  0006  CALL	R2	1
      0x80040400,  //  0007  RET	1	R2
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: find_plugin_by_endpoint
********************************************************************/
be_local_closure(class_Matter_Device_find_plugin_by_endpoint,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(find_plugin_by_endpoint),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0x58080002,  //  0000  LDCONST	R2	K2
      0x600C000C,  //  0001  GETGBL	R3	G12
      0x8810010D,  //  0002  GETMBR	R4	R0	K13
      0x7C0C0200,  //  0003  CALL	R3	1
      0x140C0403,  //  0004  LT	R3	R2	R3
      0x780E0008,  //  0005  JMPF	R3	#000F
      0x880C010D,  //  0006  GETMBR	R3	R0	K13
      0x940C0602,  //  0007  GETIDX	R3	R3	R2
      0x8C100754,  //  0008  GETMET	R4	R3	K84
      0x7C100200,  //  0009  CALL	R4	1
      0x1C100801,  //  000A  EQ	R4	R4	R1
      0x78120000,  //  000B  JMPF	R4	#000D
      0x80040600,  //  000C  RET	1	R3
      0x0008050F,  //  000D  ADD	R2	R2	K15
      0x7001FFF1,  //  000E  JMP		#0001
      0x4C0C0000,  //  000F  LDNIL	R3
      0x80040600,  //  0010  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Device
********************************************************************/
be_local_class(Matter_Device,
    24,
    NULL,
    be_nested_map(82,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(event_fabrics_saved, 81), be_const_closure(class_Matter_Device_event_fabrics_saved_closure) },
        { be_const_key_weak(_trigger_read_sensors, -1), be_const_closure(class_Matter_Device__trigger_read_sensors_closure) },
        { be_const_key_weak(plugins_config, -1), be_const_var(3) },
        { be_const_key_weak(invoke_request, -1), be_const_closure(class_Matter_Device_invoke_request_closure) },
        { be_const_key_weak(find_plugin_by_endpoint, 8), be_const_closure(class_Matter_Device_find_plugin_by_endpoint_closure) },
        { be_const_key_weak(add_read_sensors_schedule, 30), be_const_closure(class_Matter_Device_add_read_sensors_schedule_closure) },
        { be_const_key_weak(init_zigbee, 43), be_const_closure(class_Matter_Device_init_zigbee_closure) },
        { be_const_key_weak(plugins, -1), be_const_var(1) },
        { be_const_key_weak(check_config_ep, 39), be_const_closure(class_Matter_Device_check_config_ep_closure) },
        { be_const_key_weak(signal_endpoints_changed, -1), be_const_closure(class_Matter_Device_signal_endpoints_changed_closure) },
        { be_const_key_weak(udp_server, -1), be_const_var(5) },
        { be_const_key_weak(PRODUCT_ID, -1), be_const_int(32768) },
        { be_const_key_weak(disable_bridge_mode, -1), be_const_var(19) },
        { be_const_key_weak(VENDOR_ID, -1), be_const_int(65521) },
        { be_const_key_weak(init, -1), be_const_closure(class_Matter_Device_init_closure) },
        { be_const_key_weak(k2l_num, -1), be_const_static_closure(class_Matter_Device_k2l_num_closure) },
        { be_const_key_weak(every_50ms, 34), be_const_closure(class_Matter_Device_every_50ms_closure) },
        { be_const_key_weak(process_attribute_expansion, 49), be_const_closure(class_Matter_Device_process_attribute_expansion_closure) },
        { be_const_key_weak(events, 22), be_const_var(14) },
        { be_const_key_weak(create_zb_mapper, -1), be_const_closure(class_Matter_Device_create_zb_mapper_closure) },
        { be_const_key_weak(profiler, -1), be_const_var(6) },
        { be_const_key_weak(zigbee, 66), be_const_var(11) },
        { be_const_key_weak(root_passcode, -1), be_const_var(17) },
        { be_const_key_weak(get_plugin_remote_info, 14), be_const_closure(class_Matter_Device_get_plugin_remote_info_closure) },
        { be_const_key_weak(button_pressed, -1), be_const_closure(class_Matter_Device_button_pressed_closure) },
        { be_const_key_weak(remove_fabric, -1), be_const_closure(class_Matter_Device_remove_fabric_closure) },
        { be_const_key_weak(sessions, -1), be_const_var(10) },
        { be_const_key_weak(bridge_remove_endpoint, 51), be_const_closure(class_Matter_Device_bridge_remove_endpoint_closure) },
        { be_const_key_weak(every_250ms, 56), be_const_closure(class_Matter_Device_every_250ms_closure) },
        { be_const_key_weak(start, -1), be_const_closure(class_Matter_Device_start_closure) },
        { be_const_key_weak(autoconf_device, -1), be_const_closure(class_Matter_Device_autoconf_device_closure) },
        { be_const_key_weak(register_commands, -1), be_const_closure(class_Matter_Device_register_commands_closure) },
        { be_const_key_weak(MtrInfo, -1), be_const_closure(class_Matter_Device_MtrInfo_closure) },
        { be_const_key_weak(is_zigbee_present, -1), be_const_closure(class_Matter_Device_is_zigbee_present_closure) },
        { be_const_key_weak(conf_to_log, -1), be_const_static_closure(class_Matter_Device_conf_to_log_closure) },
        { be_const_key_weak(load_param, -1), be_const_closure(class_Matter_Device_load_param_closure) },
        { be_const_key_weak(sort_distinct, 74), be_const_static_closure(class_Matter_Device_sort_distinct_closure) },
        { be_const_key_weak(http_remotes, -1), be_const_var(15) },
        { be_const_key_weak(reset_param, -1), be_const_closure(class_Matter_Device_reset_param_closure) },
        { be_const_key_weak(probe_sensor_time, -1), be_const_var(22) },
        { be_const_key_weak(tick, 73), be_const_var(13) },
        { be_const_key_weak(received_ack, -1), be_const_closure(class_Matter_Device_received_ack_closure) },
        { be_const_key_weak(plugins_config_remotes, 54), be_const_var(4) },
        { be_const_key_weak(bridge_add_endpoint, 79), be_const_closure(class_Matter_Device_bridge_add_endpoint_closure) },
        { be_const_key_weak(started, -1), be_const_var(0) },
        { be_const_key_weak(get_active_endpoints, -1), be_const_closure(class_Matter_Device_get_active_endpoints_closure) },
        { be_const_key_weak(save_before_restart, -1), be_const_closure(class_Matter_Device_save_before_restart_closure) },
        { be_const_key_weak(attribute_updated, -1), be_const_closure(class_Matter_Device_attribute_updated_closure) },
        { be_const_key_weak(EP, 19), be_const_int(2) },
        { be_const_key_weak(MtrInfo_one, -1), be_const_closure(class_Matter_Device_MtrInfo_one_closure) },
        { be_const_key_weak(plugins_persist, 41), be_const_var(2) },
        { be_const_key_weak(probe_sensor_timestamp, -1), be_const_var(23) },
        { be_const_key_weak(button_multi_pressed, 55), be_const_closure(class_Matter_Device_button_multi_pressed_closure) },
        { be_const_key_weak(msg_received, -1), be_const_closure(class_Matter_Device_msg_received_closure) },
        { be_const_key_weak(plugins_classes, 33), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(60,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(gensw_btn, -1), be_const_class(be_class_Matter_Plugin_Sensor_GenericSwitch_Btn) },
        { be_const_key_weak(contact, -1), be_const_class(be_class_Matter_Plugin_Sensor_Contact) },
        { be_const_key_weak(z_light0, 26), be_const_class(be_class_Matter_Plugin_Zigbee_Light0) },
        { be_const_key_weak(v_rain, -1), be_const_class(be_class_Matter_Plugin_Virt_Sensor_Rain) },
        { be_const_key_weak(z_temp, 16), be_const_class(be_class_Matter_Plugin_Zigbee_Temperature) },
        { be_const_key_weak(http_pressure, 25), be_const_class(be_class_Matter_Plugin_Bridge_Sensor_Pressure) },
        { be_const_key_weak(v_light2, -1), be_const_class(be_class_Matter_Plugin_Virt_Light2) },
        { be_const_key_weak(v_fan, -1), be_const_class(be_class_Matter_Plugin_Virt_Fan) },
        { be_const_key_weak(v_temp, -1), be_const_class(be_class_Matter_Plugin_Virt_Sensor_Temp) },
        { be_const_key_weak(light1, -1), be_const_class(be_class_Matter_Plugin_Light1) },
        { be_const_key_weak(pressure, 45), be_const_class(be_class_Matter_Plugin_Sensor_Pressure) },
        { be_const_key_weak(v_occupancy, -1), be_const_class(be_class_Matter_Plugin_Virt_Sensor_Occupancy) },
        { be_const_key_weak(v_humidity, 34), be_const_class(be_class_Matter_Plugin_Virt_Sensor_Humidity) },
        { be_const_key_weak(waterleak, 27), be_const_class(be_class_Matter_Plugin_Sensor_Waterleak) },
        { be_const_key_weak(fan, -1), be_const_class(be_class_Matter_Plugin_Fan) },
        { be_const_key_weak(http_occupancy, 12), be_const_class(be_class_Matter_Plugin_Bridge_Sensor_Occupancy) },
        { be_const_key_weak(http_temperature, -1), be_const_class(be_class_Matter_Plugin_Bridge_Sensor_Temp) },
        { be_const_key_weak(light0, 54), be_const_class(be_class_Matter_Plugin_Light0) },
        { be_const_key_weak(http_contact, 9), be_const_class(be_class_Matter_Plugin_Bridge_Sensor_Contact) },
        { be_const_key_weak(http_airquality, 40), be_const_class(be_class_Matter_Plugin_Bridge_Sensor_Air_Quality) },
        { be_const_key_weak(light3, 33), be_const_class(be_class_Matter_Plugin_Light3) },
        { be_const_key_weak(v_waterleak, 41), be_const_class(be_class_Matter_Plugin_Virt_Sensor_Waterleak) },
        { be_const_key_weak(occupancy, -1), be_const_class(be_class_Matter_Plugin_Sensor_Occupancy) },
        { be_const_key_weak(v_light1, -1), be_const_class(be_class_Matter_Plugin_Virt_Light1) },
        { be_const_key_weak(humidity, -1), be_const_class(be_class_Matter_Plugin_Sensor_Humidity) },
        { be_const_key_weak(flow, -1), be_const_class(be_class_Matter_Plugin_Sensor_Flow) },
        { be_const_key_weak(v_contact, -1), be_const_class(be_class_Matter_Plugin_Virt_Sensor_Contact) },
        { be_const_key_weak(rain, -1), be_const_class(be_class_Matter_Plugin_Sensor_Rain) },
        { be_const_key_weak(aggregator, -1), be_const_class(be_class_Matter_Plugin_Aggregator) },
        { be_const_key_weak(http_light1, -1), be_const_class(be_class_Matter_Plugin_Bridge_Light1) },
        { be_const_key_weak(relay, 21), be_const_class(be_class_Matter_Plugin_OnOff) },
        { be_const_key_weak(http_light3, -1), be_const_class(be_class_Matter_Plugin_Bridge_Light3) },
        { be_const_key_weak(http_light2, 3), be_const_class(be_class_Matter_Plugin_Bridge_Light2) },
        { be_const_key_weak(airquality, 36), be_const_class(be_class_Matter_Plugin_Sensor_Air_Quality) },
        { be_const_key_weak(v_airquality, 44), be_const_class(be_class_Matter_Plugin_Virt_Sensor_Air_Quality) },
        { be_const_key_weak(http_illuminance, 1), be_const_class(be_class_Matter_Plugin_Bridge_Sensor_Illuminance) },
        { be_const_key_weak(shutter_X2Btilt, -1), be_const_class(be_class_Matter_Plugin_ShutterTilt) },
        { be_const_key_weak(temperature, -1), be_const_class(be_class_Matter_Plugin_Sensor_Temp) },
        { be_const_key_weak(shutter, -1), be_const_class(be_class_Matter_Plugin_Shutter) },
        { be_const_key_weak(z_pressure, -1), be_const_class(be_class_Matter_Plugin_Zigbee_Pressure) },
        { be_const_key_weak(light2, -1), be_const_class(be_class_Matter_Plugin_Light2) },
        { be_const_key_weak(http_light0, -1), be_const_class(be_class_Matter_Plugin_Bridge_Light0) },
        { be_const_key_weak(http_flow, -1), be_const_class(be_class_Matter_Plugin_Bridge_Sensor_Flow) },
        { be_const_key_weak(http_relay, -1), be_const_class(be_class_Matter_Plugin_Bridge_OnOff) },
        { be_const_key_weak(z_humidity, -1), be_const_class(be_class_Matter_Plugin_Zigbee_Humidity) },
        { be_const_key_weak(v_flow, -1), be_const_class(be_class_Matter_Plugin_Virt_Sensor_Flow) },
        { be_const_key_weak(http_waterleak, -1), be_const_class(be_class_Matter_Plugin_Bridge_Sensor_Waterleak) },
        { be_const_key_weak(v_relay, 13), be_const_class(be_class_Matter_Plugin_Virt_OnOff) },
        { be_const_key_weak(z_occupancy, 59), be_const_class(be_class_Matter_Plugin_Zigbee_Occupancy) },
        { be_const_key_weak(z_light1, 23), be_const_class(be_class_Matter_Plugin_Zigbee_Light1) },
        { be_const_key_weak(v_light0, 2), be_const_class(be_class_Matter_Plugin_Virt_Light0) },
        { be_const_key_weak(v_illuminance, 48), be_const_class(be_class_Matter_Plugin_Virt_Sensor_Illuminance) },
        { be_const_key_weak(z_light2, 6), be_const_class(be_class_Matter_Plugin_Zigbee_Light2) },
        { be_const_key_weak(root, 39), be_const_class(be_class_Matter_Plugin_Root) },
        { be_const_key_weak(onoff, -1), be_const_class(be_class_Matter_Plugin_Sensor_OnOff) },
        { be_const_key_weak(http_humidity, -1), be_const_class(be_class_Matter_Plugin_Bridge_Sensor_Humidity) },
        { be_const_key_weak(http_rain, -1), be_const_class(be_class_Matter_Plugin_Bridge_Sensor_Rain) },
        { be_const_key_weak(v_pressure, -1), be_const_class(be_class_Matter_Plugin_Virt_Sensor_Pressure) },
        { be_const_key_weak(illuminance, -1), be_const_class(be_class_Matter_Plugin_Sensor_Illuminance) },
        { be_const_key_weak(v_light3, -1), be_const_class(be_class_Matter_Plugin_Virt_Light3) },
    }))    ) } )) },
        { be_const_key_weak(autoconf, 65), be_const_var(9) },
        { be_const_key_weak(adjust_next_ep, 60), be_const_closure(class_Matter_Device_adjust_next_ep_closure) },
        { be_const_key_weak(UDP_PORT, -1), be_const_int(5540) },
        { be_const_key_weak(next_ep, 57), be_const_var(20) },
        { be_const_key_weak(every_second, -1), be_const_closure(class_Matter_Device_every_second_closure) },
        { be_const_key_weak(k2l, -1), be_const_static_closure(class_Matter_Device_k2l_closure) },
        { be_const_key_weak(stop, -1), be_const_closure(class_Matter_Device_stop_closure) },
        { be_const_key_weak(get_plugin_class_displayname, -1), be_const_closure(class_Matter_Device_get_plugin_class_displayname_closure) },
        { be_const_key_weak(message_handler, -1), be_const_var(7) },
        { be_const_key_weak(commissioning, -1), be_const_var(8) },
        { be_const_key_weak(save_param, -1), be_const_closure(class_Matter_Device_save_param_closure) },
        { be_const_key_weak(ui, -1), be_const_var(12) },
        { be_const_key_weak(resolve_attribute_read_solo, -1), be_const_closure(class_Matter_Device_resolve_attribute_read_solo_closure) },
        { be_const_key_weak(button_handler, 4), be_const_closure(class_Matter_Device_button_handler_closure) },
        { be_const_key_weak(msg_send, -1), be_const_closure(class_Matter_Device_msg_send_closure) },
        { be_const_key_weak(clean_remotes, -1), be_const_closure(class_Matter_Device_clean_remotes_closure) },
        { be_const_key_weak(MtrUpdate, -1), be_const_closure(class_Matter_Device_MtrUpdate_closure) },
        { be_const_key_weak(debug, 24), be_const_var(21) },
        { be_const_key_weak(MtrJoin, 53), be_const_closure(class_Matter_Device_MtrJoin_closure) },
        { be_const_key_weak(register_http_remote, -1), be_const_closure(class_Matter_Device_register_http_remote_closure) },
        { be_const_key_weak(update_remotes_info, 17), be_const_closure(class_Matter_Device_update_remotes_info_closure) },
        { be_const_key_weak(FILENAME, -1), be_nested_str_weak(_matter_device_X2Ejson) },
        { be_const_key_weak(read_sensors_scheduler, 12), be_const_closure(class_Matter_Device_read_sensors_scheduler_closure) },
        { be_const_key_weak(_start_udp, 11), be_const_closure(class_Matter_Device__start_udp_closure) },
        { be_const_key_weak(ipv4only, -1), be_const_var(18) },
        { be_const_key_weak(find_plugin_by_friendly_name, 6), be_const_closure(class_Matter_Device_find_plugin_by_friendly_name_closure) },
        { be_const_key_weak(root_discriminator, -1), be_const_var(16) },
    })),
    be_str_weak(Matter_Device)
);
/********************************************************************/
/* End of solidification */
