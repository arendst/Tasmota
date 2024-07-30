/* Solidification of Matter_zz_Device.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
extern const bclass be_class_Matter_Device;
// compact class 'Matter_Device' ktab size: 255, total: 533 (saved 2224 bytes)
static const bvalue be_ktab_class_Matter_Device[255] = {
  /* K0   */  be_nested_str_weak(udp_server),
  /* K1   */  be_nested_str_weak(received_ack),
  /* K2   */  be_nested_str_weak(started),
  /* K3   */  be_nested_str_weak(autoconf_device),
  /* K4   */  be_nested_str_weak(_start_udp),
  /* K5   */  be_nested_str_weak(UDP_PORT),
  /* K6   */  be_nested_str_weak(commissioning),
  /* K7   */  be_nested_str_weak(start_mdns_announce_hostnames),
  /* K8   */  be_nested_str_weak(sessions),
  /* K9   */  be_nested_str_weak(count_active_fabrics),
  /* K10  */  be_const_int(0),
  /* K11  */  be_nested_str_weak(plugins_persist),
  /* K12  */  be_nested_str_weak(save_param),
  /* K13  */  be_nested_str_weak(stop_basic_commissioning),
  /* K14  */  be_nested_str_weak(mdns_remove_op_discovery_all_fabrics),
  /* K15  */  be_const_class(be_class_Matter_Device),
  /* K16  */  be_nested_str_weak(),
  /* K17  */  be_nested_str_weak(k2l),
  /* K18  */  be_nested_str_weak(type),
  /* K19  */  be_nested_str_weak(_X20_X25s_X3A_X25s),
  /* K20  */  be_nested_str_weak(stop_iteration),
  /* K21  */  be_nested_str_weak(crypto),
  /* K22  */  be_nested_str_weak(tasmota),
  /* K23  */  be_nested_str_weak(get_option),
  /* K24  */  be_nested_str_weak(matter),
  /* K25  */  be_nested_str_weak(MATTER_OPTION),
  /* K26  */  be_nested_str_weak(UI),
  /* K27  */  be_nested_str_weak(profiler),
  /* K28  */  be_nested_str_weak(Profiler),
  /* K29  */  be_nested_str_weak(tick),
  /* K30  */  be_nested_str_weak(plugins),
  /* K31  */  be_nested_str_weak(plugins_config_remotes),
  /* K32  */  be_nested_str_weak(next_ep),
  /* K33  */  be_const_int(2),
  /* K34  */  be_nested_str_weak(ipv4only),
  /* K35  */  be_nested_str_weak(disable_bridge_mode),
  /* K36  */  be_nested_str_weak(load_param),
  /* K37  */  be_nested_str_weak(Session_Store),
  /* K38  */  be_nested_str_weak(load_fabrics),
  /* K39  */  be_nested_str_weak(message_handler),
  /* K40  */  be_nested_str_weak(MessageHandler),
  /* K41  */  be_nested_str_weak(events),
  /* K42  */  be_nested_str_weak(EventHandler),
  /* K43  */  be_nested_str_weak(ui),
  /* K44  */  be_nested_str_weak(wifi),
  /* K45  */  be_nested_str_weak(up),
  /* K46  */  be_nested_str_weak(eth),
  /* K47  */  be_nested_str_weak(start),
  /* K48  */  be_nested_str_weak(add_rule),
  /* K49  */  be_nested_str_weak(Wifi_X23Connected),
  /* K50  */  be_nested_str_weak(matter_start),
  /* K51  */  be_nested_str_weak(Eth_X23Connected),
  /* K52  */  be_nested_str_weak(Commissioning),
  /* K53  */  be_nested_str_weak(add_driver),
  /* K54  */  be_nested_str_weak(register_commands),
  /* K55  */  be_nested_str_weak(get_name),
  /* K56  */  be_const_int(1),
  /* K57  */  be_nested_str_weak(find),
  /* K58  */  be_nested_str_weak(log),
  /* K59  */  be_nested_str_weak(MTR_X3A_X20Starting_X20UDP_X20server_X20on_X20port_X3A_X20),
  /* K60  */  be_nested_str_weak(UDPServer),
  /* K61  */  be_nested_str_weak(MTR_X3A_X20removing_X20fabric_X20),
  /* K62  */  be_nested_str_weak(get_fabric_id),
  /* K63  */  be_nested_str_weak(copy),
  /* K64  */  be_nested_str_weak(reverse),
  /* K65  */  be_nested_str_weak(tohex),
  /* K66  */  be_nested_str_weak(im),
  /* K67  */  be_nested_str_weak(subs_shop),
  /* K68  */  be_nested_str_weak(remove_by_fabric),
  /* K69  */  be_nested_str_weak(mdns_remove_op_discovery),
  /* K70  */  be_nested_str_weak(remove_fabric),
  /* K71  */  be_nested_str_weak(save_fabrics),
  /* K72  */  be_nested_str_weak(endpoint),
  /* K73  */  be_nested_str_weak(cluster),
  /* K74  */  be_nested_str_weak(attribute),
  /* K75  */  be_nested_str_weak(find_plugin_by_endpoint),
  /* K76  */  be_nested_str_weak(status),
  /* K77  */  be_nested_str_weak(UNSUPPORTED_ENDPOINT),
  /* K78  */  be_nested_str_weak(contains_cluster),
  /* K79  */  be_nested_str_weak(UNSUPPORTED_CLUSTER),
  /* K80  */  be_nested_str_weak(contains_attribute),
  /* K81  */  be_nested_str_weak(UNSUPPORTED_ATTRIBUTE),
  /* K82  */  be_nested_str_weak(attribute_updated),
  /* K83  */  be_const_int(3),
  /* K84  */  be_nested_str_weak(AGGREGATOR_ENDPOINT),
  /* K85  */  be_nested_str_weak(json),
  /* K86  */  be_nested_str_weak(plugins_config),
  /* K87  */  be_nested_str_weak(autoconf_device_map),
  /* K88  */  be_nested_str_weak(adjust_next_ep),
  /* K89  */  be_nested_str_weak(MTR_X3A_X20autoconfig_X20_X3D_X20),
  /* K90  */  be_nested_str_weak(_instantiate_plugins_from_config),
  /* K91  */  be_nested_str_weak(read_sensors),
  /* K92  */  be_nested_str_weak(loglevel),
  /* K93  */  be_nested_str_weak(MTR_X3A_X20read_sensors_X3A_X20),
  /* K94  */  be_nested_str_weak(load),
  /* K95  */  be_nested_str_weak(parse_sensors),
  /* K96  */  be_nested_str_weak(MTR_X3A_X20unable_X20to_X20parse_X20read_sensors_X3A_X20),
  /* K97  */  be_nested_str_weak(get_endpoint),
  /* K98  */  be_nested_str_weak(push),
  /* K99  */  be_nested_str_weak(send_UDP),
  /* K100 */  be_nested_str_weak(remove_driver),
  /* K101 */  be_nested_str_weak(stop),
  /* K102 */  be_nested_str_weak(keys),
  /* K103 */  be_nested_str_weak(probe_sensor_time),
  /* K104 */  be_nested_str_weak(probe_sensor_timestamp),
  /* K105 */  be_nested_str_weak(jitter),
  /* K106 */  be_nested_str_weak(button_handler),
  /* K107 */  be_nested_str_weak(http_remotes),
  /* K108 */  be_nested_str_weak(contains),
  /* K109 */  be_nested_str_weak(get_timeout),
  /* K110 */  be_nested_str_weak(set_timeout),
  /* K111 */  be_nested_str_weak(HTTP_remote),
  /* K112 */  be_nested_str_weak(set_info),
  /* K113 */  be_nested_str_weak(k2l_num),
  /* K114 */  be_nested_str_weak(Plugin_Root),
  /* K115 */  be_nested_str_weak(MTR_X3A_X20Configuring_X20endpoints),
  /* K116 */  be_nested_str_weak(MTR_X3A_X20_X20_X20endpoint_X20_X3D_X20_X255i_X20type_X3A_X25s_X25s),
  /* K117 */  be_nested_str_weak(root),
  /* K118 */  be_nested_str_weak(Plugin_Aggregator),
  /* K119 */  be_nested_str_weak(aggregator),
  /* K120 */  be_nested_str_weak(MTR_X3A_X20no_X20class_X20name_X2C_X20skipping),
  /* K121 */  be_nested_str_weak(MTR_X3A_X20only_X20one_X20root_X20node_X20allowed),
  /* K122 */  be_nested_str_weak(plugins_classes),
  /* K123 */  be_nested_str_weak(MTR_X3A_X20unknown_X20class_X20name_X20_X27),
  /* K124 */  be_nested_str_weak(_X27_X20skipping),
  /* K125 */  be_nested_str_weak(conf_to_log),
  /* K126 */  be_nested_str_weak(MTR_X3A_X20Exception),
  /* K127 */  be_nested_str_weak(_X7C),
  /* K128 */  be_nested_str_weak(publish_result),
  /* K129 */  be_nested_str_weak(_X7B_X22Matter_X22_X3A_X7B_X22Initialized_X22_X3A1_X7D_X7D),
  /* K130 */  be_nested_str_weak(Matter),
  /* K131 */  be_nested_str_weak(remove),
  /* K132 */  be_nested_str_weak(DISPLAY_NAME),
  /* K133 */  be_nested_str_weak(introspect),
  /* K134 */  be_nested_str_weak(get),
  /* K135 */  be_nested_str_weak(http_remote),
  /* K136 */  be_nested_str_weak(MTR_X3A_X20remove_X20unused_X20remote_X3A_X20),
  /* K137 */  be_nested_str_weak(addr),
  /* K138 */  be_nested_str_weak(close),
  /* K139 */  be_nested_str_weak(MTR_X3A_X20invalid_X20entry_X20with_X20ep_X20_X270_X27),
  /* K140 */  be_nested_str_weak(MTR_X3A_X20endpoint_X20_X25s_X20collides_X20wit_X20aggregator_X2C_X20relocating_X20to_X20_X25s),
  /* K141 */  be_nested_str_weak(every_second),
  /* K142 */  be_nested_str_weak(Path),
  /* K143 */  be_nested_str_weak(attribute_updated_ctx),
  /* K144 */  be_nested_str_weak(read_sensors_scheduler),
  /* K145 */  be_nested_str_weak(every_250ms),
  /* K146 */  be_nested_str_weak(PathGenerator),
  /* K147 */  be_nested_str_weak(is_direct),
  /* K148 */  be_nested_str_weak(next_attribute),
  /* K149 */  be_nested_str_weak(get_pi),
  /* K150 */  be_nested_str_weak(START_ENDPOINT),
  /* K151 */  be_nested_str_weak(light),
  /* K152 */  be_nested_str_weak(channels),
  /* K153 */  be_nested_str_weak(light1),
  /* K154 */  be_nested_str_weak(light2),
  /* K155 */  be_nested_str_weak(light3),
  /* K156 */  be_nested_str_weak(cmd),
  /* K157 */  be_nested_str_weak(Status_X2013),
  /* K158 */  be_nested_str_weak(MTR_X3A_X20Status_X2013_X20_X3D_X20),
  /* K159 */  be_nested_str_weak(StatusSHT),
  /* K160 */  be_nested_str_weak(SHT),
  /* K161 */  be_nested_str_weak(MTR_X3A_X20_X27_X25s_X27_X20_X3D_X20_X25s),
  /* K162 */  be_nested_str_weak(Relay1),
  /* K163 */  be_nested_str_weak(Relay2),
  /* K164 */  be_nested_str_weak(MTR_X3A_X20relay1_X3D_X25s_X20relay2_X3D_X25s),
  /* K165 */  be_nested_str_weak(TiltConfig),
  /* K166 */  be_nested_str_weak(shutter_X2Btilt),
  /* K167 */  be_nested_str_weak(shutter),
  /* K168 */  be_nested_str_weak(get_power),
  /* K169 */  be_nested_str_weak(relay),
  /* K170 */  be_nested_str_weak(autoconf_sensors_list),
  /* K171 */  be_nested_str_weak(MTR_X3A_X20adding_X20endpoint_X20_X3D_X20_X25i_X20type_X3A_X25s_X25s),
  /* K172 */  be_nested_str_weak(signal_endpoints_changed),
  /* K173 */  be_nested_str_weak(MtrInfo_one),
  /* K174 */  be_nested_str_weak(int),
  /* K175 */  be_nested_str_weak(find_plugin_by_friendly_name),
  /* K176 */  be_nested_str_weak(resp_cmnd_done),
  /* K177 */  be_nested_str_weak(FILENAME),
  /* K178 */  be_nested_str_weak(read),
  /* K179 */  be_nested_str_weak(root_discriminator),
  /* K180 */  be_nested_str_weak(distinguish),
  /* K181 */  be_nested_str_weak(root_passcode),
  /* K182 */  be_nested_str_weak(passcode),
  /* K183 */  be_nested_str_weak(nextep),
  /* K184 */  be_nested_str_weak(config),
  /* K185 */  be_nested_str_weak(debug),
  /* K186 */  be_nested_str_weak(MTR_X3A_X20Load_config_X20_X3D_X20_X25s),
  /* K187 */  be_nested_str_weak(check_config_ep),
  /* K188 */  be_nested_str_weak(remotes),
  /* K189 */  be_nested_str_weak(MTR_X3A_X20load_remotes_X20_X3D_X20),
  /* K190 */  be_nested_str_weak(io_error),
  /* K191 */  be_nested_str_weak(MTR_X3A_X20load_param_X20Exception_X3A),
  /* K192 */  be_nested_str_weak(random),
  /* K193 */  be_nested_str_weak(generate_random_passcode),
  /* K194 */  be_nested_str_weak(resp_cmnd_str),
  /* K195 */  be_nested_str_weak(Invalid_X20JSON),
  /* K196 */  be_nested_str_weak(find_key_i),
  /* K197 */  be_nested_str_weak(Ep),
  /* K198 */  be_nested_str_weak(Name),
  /* K199 */  be_nested_str_weak(Invalid_X20_X27Ep_X27_X20attribute),
  /* K200 */  be_nested_str_weak(Invalid_X20Device),
  /* K201 */  be_nested_str_weak(VIRTUAL),
  /* K202 */  be_nested_str_weak(Device_X20is_X20not_X20virtual),
  /* K203 */  be_nested_str_weak(consolidate_update_commands),
  /* K204 */  be_nested_str_weak(find_list_i),
  /* K205 */  be_nested_str_weak(Invalid_X20attribute_X20_X27_X25s_X27),
  /* K206 */  be_nested_str_weak(update_virtual),
  /* K207 */  be_nested_str_weak(state_json),
  /* K208 */  be_nested_str_weak(_X7B_X22_X25s_X22_X3A_X25s_X7D),
  /* K209 */  be_nested_str_weak(resp_cmnd),
  /* K210 */  be_nested_str_weak(Missing_X20_X27Device_X27_X20attribute),
  /* K211 */  be_nested_str_weak(add_cmd),
  /* K212 */  be_nested_str_weak(MtrJoin),
  /* K213 */  be_nested_str_weak(MtrUpdate),
  /* K214 */  be_nested_str_weak(MtrInfo),
  /* K215 */  be_nested_str_weak(update_remotes_info),
  /* K216 */  be_nested_str_weak(_X7B_X22distinguish_X22_X3A_X25i_X2C_X22passcode_X22_X3A_X25i_X2C_X22ipv4only_X22_X3A_X25s_X2C_X22disable_bridge_mode_X22_X3A_X25s_X2C_X22nextep_X22_X3A_X25i),
  /* K217 */  be_nested_str_weak(true),
  /* K218 */  be_nested_str_weak(false),
  /* K219 */  be_nested_str_weak(_X2C_X22debug_X22_X3Atrue),
  /* K220 */  be_nested_str_weak(_X2C_X0A_X22config_X22_X3A),
  /* K221 */  be_nested_str_weak(dump),
  /* K222 */  be_nested_str_weak(_X2C_X0A_X22remotes_X22_X3A),
  /* K223 */  be_nested_str_weak(_X7D),
  /* K224 */  be_nested_str_weak(w),
  /* K225 */  be_nested_str_weak(write),
  /* K226 */  be_nested_str_weak(MTR_X3A_X20_X3DSaved_X20_X20_X20_X20_X20parameters_X25s),
  /* K227 */  be_nested_str_weak(_X20and_X20configuration),
  /* K228 */  be_nested_str_weak(MTR_X3A_X20Session_Store_X3A_X3Asave_X20Exception_X3A),
  /* K229 */  be_nested_str_weak(MTR_X3A_X20Cannot_X20remove_X20an_X20enpoint_X20not_X20configured_X3A_X20),
  /* K230 */  be_nested_str_weak(MTR_X3A_X20deleting_X20endpoint_X20_X3D_X20_X25i),
  /* K231 */  be_nested_str_weak(clean_remotes),
  /* K232 */  be_nested_str_weak(msg_received),
  /* K233 */  be_nested_str_weak(invoke_request),
  /* K234 */  be_nested_str_weak(time_reached),
  /* K235 */  be_nested_str_weak(_trigger_read_sensors),
  /* K236 */  be_nested_str_weak(millis),
  /* K237 */  be_nested_str_weak(start_root_basic_commissioning),
  /* K238 */  be_nested_str_weak(Temperature),
  /* K239 */  be_nested_str_weak(_X23Temperature),
  /* K240 */  be_nested_str_weak(temperature),
  /* K241 */  be_nested_str_weak(filter),
  /* K242 */  be_nested_str_weak(Pressure),
  /* K243 */  be_nested_str_weak(_X23Pressure),
  /* K244 */  be_nested_str_weak(pressure),
  /* K245 */  be_nested_str_weak(Illuminance),
  /* K246 */  be_nested_str_weak(_X23Illuminance),
  /* K247 */  be_nested_str_weak(illuminance),
  /* K248 */  be_nested_str_weak(Humidity),
  /* K249 */  be_nested_str_weak(_X23Humidity),
  /* K250 */  be_nested_str_weak(humidity),
  /* K251 */  be_nested_str_weak(_X7B_X22MtrInfo_X22_X3A_X25s_X7D),
  /* K252 */  be_nested_str_weak(every_50ms),
  /* K253 */  be_nested_str_weak(ARG),
  /* K254 */  be_nested_str_weak(get_info),
};


extern const bclass be_class_Matter_Device;

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
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x80040400,  //  0004  RET	1	R2
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
    ( &(const binstruction[14]) {  /* code */
      0x88040102,  //  0000  GETMBR	R1	R0	K2
      0x78060000,  //  0001  JMPF	R1	#0003
      0x80000200,  //  0002  RET	0
      0x8C040103,  //  0003  GETMET	R1	R0	K3
      0x7C040200,  //  0004  CALL	R1	1
      0x8C040104,  //  0005  GETMET	R1	R0	K4
      0x880C0105,  //  0006  GETMBR	R3	R0	K5
      0x7C040400,  //  0007  CALL	R1	2
      0x88040106,  //  0008  GETMBR	R1	R0	K6
      0x8C040307,  //  0009  GETMET	R1	R1	K7
      0x7C040200,  //  000A  CALL	R1	1
      0x50040200,  //  000B  LDBOOL	R1	1	0
      0x90020401,  //  000C  SETMBR	R0	K2	R1
      0x80000000,  //  000D  RET	0
    })
  )
);
/*******************************************************************/


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
      0x88040108,  //  0000  GETMBR	R1	R0	K8
      0x8C040309,  //  0001  GETMET	R1	R1	K9
      0x7C040200,  //  0002  CALL	R1	1
      0x2404030A,  //  0003  GT	R1	R1	K10
      0x78060005,  //  0004  JMPF	R1	#000B
      0x8804010B,  //  0005  GETMBR	R1	R0	K11
      0x74060003,  //  0006  JMPT	R1	#000B
      0x50040200,  //  0007  LDBOOL	R1	1	0
      0x90021601,  //  0008  SETMBR	R0	K11	R1
      0x8C04010C,  //  0009  GETMET	R1	R0	K12
      0x7C040200,  //  000A  CALL	R1	1
      0x80000000,  //  000B  RET	0
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
    ( &(const binstruction[ 5]) {  /* code */
      0x8C04010D,  //  0000  GETMET	R1	R0	K13
      0x7C040200,  //  0001  CALL	R1	1
      0x8C04010E,  //  0002  GETMET	R1	R0	K14
      0x7C040200,  //  0003  CALL	R1	1
      0x80000000,  //  0004  RET	0
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
      0x5804000F,  //  0000  LDCONST	R1	K15
      0x58080010,  //  0001  LDCONST	R2	K16
      0x600C0010,  //  0002  GETGBL	R3	G16
      0x8C100311,  //  0003  GETMET	R4	R1	K17
      0x5C180000,  //  0004  MOVE	R6	R0
      0x7C100400,  //  0005  CALL	R4	2
      0x7C0C0200,  //  0006  CALL	R3	1
      0xA802000B,  //  0007  EXBLK	0	#0014
      0x5C100600,  //  0008  MOVE	R4	R3
      0x7C100000,  //  0009  CALL	R4	0
      0x1C140912,  //  000A  EQ	R5	R4	K18
      0x78160000,  //  000B  JMPF	R5	#000D
      0x7001FFFA,  //  000C  JMP		#0008
      0x60140018,  //  000D  GETGBL	R5	G24
      0x58180013,  //  000E  LDCONST	R6	K19
      0x5C1C0800,  //  000F  MOVE	R7	R4
      0x94200004,  //  0010  GETIDX	R8	R0	R4
      0x7C140600,  //  0011  CALL	R5	3
      0x00080405,  //  0012  ADD	R2	R2	R5
      0x7001FFF3,  //  0013  JMP		#0008
      0x580C0014,  //  0014  LDCONST	R3	K20
      0xAC0C0200,  //  0015  CATCH	R3	1	0
      0xB0080000,  //  0016  RAISE	2	R0	R0
      0x80040400,  //  0017  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_Matter_Device_init,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 2]) {
      be_nested_proto(
        4,                          /* nstack */
        0,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 5]) {     /* constants */
        /* K0   */  be_nested_str_weak(start),
        /* K1   */  be_nested_str_weak(tasmota),
        /* K2   */  be_nested_str_weak(remove_rule),
        /* K3   */  be_nested_str_weak(Wifi_X23Connected),
        /* K4   */  be_nested_str_weak(matter_start),
        }),
        be_str_weak(_anonymous_),
        &be_const_str_solidified,
        ( &(const binstruction[ 9]) {  /* code */
          0x68000000,  //  0000  GETUPV	R0	U0
          0x8C000100,  //  0001  GETMET	R0	R0	K0
          0x7C000200,  //  0002  CALL	R0	1
          0xB8020200,  //  0003  GETNGBL	R0	K1
          0x8C000102,  //  0004  GETMET	R0	R0	K2
          0x58080003,  //  0005  LDCONST	R2	K3
          0x580C0004,  //  0006  LDCONST	R3	K4
          0x7C000600,  //  0007  CALL	R0	3
          0x80000000,  //  0008  RET	0
        })
      ),
      be_nested_proto(
        4,                          /* nstack */
        0,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 5]) {     /* constants */
        /* K0   */  be_nested_str_weak(start),
        /* K1   */  be_nested_str_weak(tasmota),
        /* K2   */  be_nested_str_weak(remove_rule),
        /* K3   */  be_nested_str_weak(Eth_X23Connected),
        /* K4   */  be_nested_str_weak(matter_start),
        }),
        be_str_weak(_anonymous_),
        &be_const_str_solidified,
        ( &(const binstruction[ 9]) {  /* code */
          0x68000000,  //  0000  GETUPV	R0	U0
          0x8C000100,  //  0001  GETMET	R0	R0	K0
          0x7C000200,  //  0002  CALL	R0	1
          0xB8020200,  //  0003  GETNGBL	R0	K1
          0x8C000102,  //  0004  GETMET	R0	R0	K2
          0x58080003,  //  0005  LDCONST	R2	K3
          0x580C0004,  //  0006  LDCONST	R3	K4
          0x7C000600,  //  0007  CALL	R0	3
          0x80000000,  //  0008  RET	0
        })
      ),
    }),
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[105]) {  /* code */
      0xA4062A00,  //  0000  IMPORT	R1	K21
      0xB80A2C00,  //  0001  GETNGBL	R2	K22
      0x8C080517,  //  0002  GETMET	R2	R2	K23
      0xB8123000,  //  0003  GETNGBL	R4	K24
      0x88100919,  //  0004  GETMBR	R4	R4	K25
      0x7C080400,  //  0005  CALL	R2	2
      0x740A0004,  //  0006  JMPT	R2	#000C
      0xB80A3000,  //  0007  GETNGBL	R2	K24
      0x8C08051A,  //  0008  GETMET	R2	R2	K26
      0x5C100000,  //  0009  MOVE	R4	R0
      0x7C080400,  //  000A  CALL	R2	2
      0x80000400,  //  000B  RET	0
      0xB80A3000,  //  000C  GETNGBL	R2	K24
      0xB80E3000,  //  000D  GETNGBL	R3	K24
      0x8C0C071C,  //  000E  GETMET	R3	R3	K28
      0x7C0C0200,  //  000F  CALL	R3	1
      0x900A3603,  //  0010  SETMBR	R2	K27	R3
      0x50080000,  //  0011  LDBOOL	R2	0	0
      0x90020402,  //  0012  SETMBR	R0	K2	R2
      0x90023B0A,  //  0013  SETMBR	R0	K29	K10
      0x60080012,  //  0014  GETGBL	R2	G18
      0x7C080000,  //  0015  CALL	R2	0
      0x90023C02,  //  0016  SETMBR	R0	K30	R2
      0x50080000,  //  0017  LDBOOL	R2	0	0
      0x90021602,  //  0018  SETMBR	R0	K11	R2
      0x60080013,  //  0019  GETGBL	R2	G19
      0x7C080000,  //  001A  CALL	R2	0
      0x90023E02,  //  001B  SETMBR	R0	K31	R2
      0x90024121,  //  001C  SETMBR	R0	K32	K33
      0x50080000,  //  001D  LDBOOL	R2	0	0
      0x90024402,  //  001E  SETMBR	R0	K34	R2
      0x50080000,  //  001F  LDBOOL	R2	0	0
      0x90024602,  //  0020  SETMBR	R0	K35	R2
      0x8C080124,  //  0021  GETMET	R2	R0	K36
      0x7C080200,  //  0022  CALL	R2	1
      0xB80A3000,  //  0023  GETNGBL	R2	K24
      0x8C080525,  //  0024  GETMET	R2	R2	K37
      0x5C100000,  //  0025  MOVE	R4	R0
      0x7C080400,  //  0026  CALL	R2	2
      0x90021002,  //  0027  SETMBR	R0	K8	R2
      0x88080108,  //  0028  GETMBR	R2	R0	K8
      0x8C080526,  //  0029  GETMET	R2	R2	K38
      0x7C080200,  //  002A  CALL	R2	1
      0xB80A3000,  //  002B  GETNGBL	R2	K24
      0x8C080528,  //  002C  GETMET	R2	R2	K40
      0x5C100000,  //  002D  MOVE	R4	R0
      0x7C080400,  //  002E  CALL	R2	2
      0x90024E02,  //  002F  SETMBR	R0	K39	R2
      0xB80A3000,  //  0030  GETNGBL	R2	K24
      0x8C08052A,  //  0031  GETMET	R2	R2	K42
      0x5C100000,  //  0032  MOVE	R4	R0
      0x7C080400,  //  0033  CALL	R2	2
      0x90025202,  //  0034  SETMBR	R0	K41	R2
      0xB80A3000,  //  0035  GETNGBL	R2	K24
      0x8C08051A,  //  0036  GETMET	R2	R2	K26
      0x5C100000,  //  0037  MOVE	R4	R0
      0x7C080400,  //  0038  CALL	R2	2
      0x90025602,  //  0039  SETMBR	R0	K43	R2
      0xB80A2C00,  //  003A  GETNGBL	R2	K22
      0x8C08052C,  //  003B  GETMET	R2	R2	K44
      0x7C080200,  //  003C  CALL	R2	1
      0x9408052D,  //  003D  GETIDX	R2	R2	K45
      0x740A0004,  //  003E  JMPT	R2	#0044
      0xB80A2C00,  //  003F  GETNGBL	R2	K22
      0x8C08052E,  //  0040  GETMET	R2	R2	K46
      0x7C080200,  //  0041  CALL	R2	1
      0x9408052D,  //  0042  GETIDX	R2	R2	K45
      0x780A0001,  //  0043  JMPF	R2	#0046
      0x8C08012F,  //  0044  GETMET	R2	R0	K47
      0x7C080200,  //  0045  CALL	R2	1
      0xB80A2C00,  //  0046  GETNGBL	R2	K22
      0x8C08052C,  //  0047  GETMET	R2	R2	K44
      0x7C080200,  //  0048  CALL	R2	1
      0x9408052D,  //  0049  GETIDX	R2	R2	K45
      0x740A0005,  //  004A  JMPT	R2	#0051
      0xB80A2C00,  //  004B  GETNGBL	R2	K22
      0x8C080530,  //  004C  GETMET	R2	R2	K48
      0x58100031,  //  004D  LDCONST	R4	K49
      0x84140000,  //  004E  CLOSURE	R5	P0
      0x58180032,  //  004F  LDCONST	R6	K50
      0x7C080800,  //  0050  CALL	R2	4
      0xB80A2C00,  //  0051  GETNGBL	R2	K22
      0x8C08052E,  //  0052  GETMET	R2	R2	K46
      0x7C080200,  //  0053  CALL	R2	1
      0x9408052D,  //  0054  GETIDX	R2	R2	K45
      0x740A0005,  //  0055  JMPT	R2	#005C
      0xB80A2C00,  //  0056  GETNGBL	R2	K22
      0x8C080530,  //  0057  GETMET	R2	R2	K48
      0x58100033,  //  0058  LDCONST	R4	K51
      0x84140001,  //  0059  CLOSURE	R5	P1
      0x58180032,  //  005A  LDCONST	R6	K50
      0x7C080800,  //  005B  CALL	R2	4
      0xB80A3000,  //  005C  GETNGBL	R2	K24
      0x8C080534,  //  005D  GETMET	R2	R2	K52
      0x5C100000,  //  005E  MOVE	R4	R0
      0x7C080400,  //  005F  CALL	R2	2
      0x90020C02,  //  0060  SETMBR	R0	K6	R2
      0xB80A2C00,  //  0061  GETNGBL	R2	K22
      0x8C080535,  //  0062  GETMET	R2	R2	K53
      0x5C100000,  //  0063  MOVE	R4	R0
      0x7C080400,  //  0064  CALL	R2	2
      0x8C080136,  //  0065  GETMET	R2	R0	K54
      0x7C080200,  //  0066  CALL	R2	1
      0xA0000000,  //  0067  CLOSE	R0
      0x80000000,  //  0068  RET	0
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
      0x1C08050A,  //  0006  EQ	R2	R2	K10
      0x780A0001,  //  0007  JMPF	R2	#000A
      0x4C080000,  //  0008  LDNIL	R2
      0x80040400,  //  0009  RET	1	R2
      0x5808000A,  //  000A  LDCONST	R2	K10
      0x600C000C,  //  000B  GETGBL	R3	G12
      0x8810011E,  //  000C  GETMBR	R4	R0	K30
      0x7C0C0200,  //  000D  CALL	R3	1
      0x140C0403,  //  000E  LT	R3	R2	R3
      0x780E0010,  //  000F  JMPF	R3	#0021
      0x880C011E,  //  0010  GETMBR	R3	R0	K30
      0x940C0602,  //  0011  GETIDX	R3	R3	R2
      0x8C100737,  //  0012  GETMET	R4	R3	K55
      0x7C100200,  //  0013  CALL	R4	1
      0x4C140000,  //  0014  LDNIL	R5
      0x20140805,  //  0015  NE	R5	R4	R5
      0x78160007,  //  0016  JMPF	R5	#001F
      0x6014000C,  //  0017  GETGBL	R5	G12
      0x5C180800,  //  0018  MOVE	R6	R4
      0x7C140200,  //  0019  CALL	R5	1
      0x24140B0A,  //  001A  GT	R5	R5	K10
      0x78160002,  //  001B  JMPF	R5	#001F
      0x1C140801,  //  001C  EQ	R5	R4	R1
      0x78160000,  //  001D  JMPF	R5	#001F
      0x80040600,  //  001E  RET	1	R3
      0x00080538,  //  001F  ADD	R2	R2	K56
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
      0x8808011F,  //  0000  GETMBR	R2	R0	K31
      0x8C080539,  //  0001  GETMET	R2	R2	K57
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
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x780A0000,  //  0001  JMPF	R2	#0003
      0x80000400,  //  0002  RET	0
      0x4C080000,  //  0003  LDNIL	R2
      0x1C080202,  //  0004  EQ	R2	R1	R2
      0x780A0000,  //  0005  JMPF	R2	#0007
      0x540615A3,  //  0006  LDINT	R1	5540
      0xB80A7400,  //  0007  GETNGBL	R2	K58
      0x600C0008,  //  0008  GETGBL	R3	G8
      0x5C100200,  //  0009  MOVE	R4	R1
      0x7C0C0200,  //  000A  CALL	R3	1
      0x000E7603,  //  000B  ADD	R3	K59	R3
      0x58100021,  //  000C  LDCONST	R4	K33
      0x7C080400,  //  000D  CALL	R2	2
      0xB80A3000,  //  000E  GETNGBL	R2	K24
      0x8C08053C,  //  000F  GETMET	R2	R2	K60
      0x5C100000,  //  0010  MOVE	R4	R0
      0x58140010,  //  0011  LDCONST	R5	K16
      0x5C180200,  //  0012  MOVE	R6	R1
      0x7C080800,  //  0013  CALL	R2	4
      0x90020002,  //  0014  SETMBR	R0	K0	R2
      0x88080100,  //  0015  GETMBR	R2	R0	K0
      0x8C08052F,  //  0016  GETMET	R2	R2	K47
      0x84100000,  //  0017  CLOSURE	R4	P0
      0x7C080400,  //  0018  CALL	R2	2
      0xA0000000,  //  0019  CLOSE	R0
      0x80000000,  //  001A  RET	0
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
      0xB80A7400,  //  0003  GETNGBL	R2	K58
      0x8C0C033E,  //  0004  GETMET	R3	R1	K62
      0x7C0C0200,  //  0005  CALL	R3	1
      0x8C0C073F,  //  0006  GETMET	R3	R3	K63
      0x7C0C0200,  //  0007  CALL	R3	1
      0x8C0C0740,  //  0008  GETMET	R3	R3	K64
      0x7C0C0200,  //  0009  CALL	R3	1
      0x8C0C0741,  //  000A  GETMET	R3	R3	K65
      0x7C0C0200,  //  000B  CALL	R3	1
      0x000E7A03,  //  000C  ADD	R3	K61	R3
      0x58100021,  //  000D  LDCONST	R4	K33
      0x7C080400,  //  000E  CALL	R2	2
      0x88080127,  //  000F  GETMBR	R2	R0	K39
      0x88080542,  //  0010  GETMBR	R2	R2	K66
      0x88080543,  //  0011  GETMBR	R2	R2	K67
      0x8C080544,  //  0012  GETMET	R2	R2	K68
      0x5C100200,  //  0013  MOVE	R4	R1
      0x7C080400,  //  0014  CALL	R2	2
      0x88080106,  //  0015  GETMBR	R2	R0	K6
      0x8C080545,  //  0016  GETMET	R2	R2	K69
      0x5C100200,  //  0017  MOVE	R4	R1
      0x7C080400,  //  0018  CALL	R2	2
      0x88080108,  //  0019  GETMBR	R2	R0	K8
      0x8C080546,  //  001A  GETMET	R2	R2	K70
      0x5C100200,  //  001B  MOVE	R4	R1
      0x7C080400,  //  001C  CALL	R2	2
      0x88080108,  //  001D  GETMBR	R2	R0	K8
      0x8C080547,  //  001E  GETMET	R2	R2	K71
      0x7C080200,  //  001F  CALL	R2	1
      0x80000000,  //  0020  RET	0
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
      0x88080348,  //  0000  GETMBR	R2	R1	K72
      0x880C0349,  //  0001  GETMBR	R3	R1	K73
      0x8810034A,  //  0002  GETMBR	R4	R1	K74
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
      0x8C14014B,  //  000E  GETMET	R5	R0	K75
      0x5C1C0400,  //  000F  MOVE	R7	R2
      0x7C140400,  //  0010  CALL	R5	2
      0x4C180000,  //  0011  LDNIL	R6
      0x1C180A06,  //  0012  EQ	R6	R5	R6
      0x781A0005,  //  0013  JMPF	R6	#001A
      0xB81A3000,  //  0014  GETNGBL	R6	K24
      0x88180D4D,  //  0015  GETMBR	R6	R6	K77
      0x90069806,  //  0016  SETMBR	R1	K76	R6
      0x4C180000,  //  0017  LDNIL	R6
      0x80040C00,  //  0018  RET	1	R6
      0x70020013,  //  0019  JMP		#002E
      0x8C180B4E,  //  001A  GETMET	R6	R5	K78
      0x5C200600,  //  001B  MOVE	R8	R3
      0x7C180400,  //  001C  CALL	R6	2
      0x741A0005,  //  001D  JMPT	R6	#0024
      0xB81A3000,  //  001E  GETNGBL	R6	K24
      0x88180D4F,  //  001F  GETMBR	R6	R6	K79
      0x90069806,  //  0020  SETMBR	R1	K76	R6
      0x4C180000,  //  0021  LDNIL	R6
      0x80040C00,  //  0022  RET	1	R6
      0x70020009,  //  0023  JMP		#002E
      0x8C180B50,  //  0024  GETMET	R6	R5	K80
      0x5C200600,  //  0025  MOVE	R8	R3
      0x5C240800,  //  0026  MOVE	R9	R4
      0x7C180600,  //  0027  CALL	R6	3
      0x741A0004,  //  0028  JMPT	R6	#002E
      0xB81A3000,  //  0029  GETNGBL	R6	K24
      0x88180D51,  //  002A  GETMBR	R6	R6	K81
      0x90069806,  //  002B  SETMBR	R1	K76	R6
      0x4C180000,  //  002C  LDNIL	R6
      0x80040C00,  //  002D  RET	1	R6
      0x80040A00,  //  002E  RET	1	R5
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
      0x8C040152,  //  0000  GETMET	R1	R0	K82
      0x580C000A,  //  0001  LDCONST	R3	K10
      0x5412001C,  //  0002  LDINT	R4	29
      0x58140053,  //  0003  LDCONST	R5	K83
      0x50180000,  //  0004  LDBOOL	R6	0	0
      0x7C040A00,  //  0005  CALL	R1	5
      0x8C040152,  //  0006  GETMET	R1	R0	K82
      0xB80E3000,  //  0007  GETNGBL	R3	K24
      0x880C0754,  //  0008  GETMBR	R3	R3	K84
      0x5412001C,  //  0009  LDINT	R4	29
      0x58140053,  //  000A  LDCONST	R5	K83
      0x50180000,  //  000B  LDBOOL	R6	0	0
      0x7C040A00,  //  000C  CALL	R1	5
      0x80000000,  //  000D  RET	0
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
    ( &(const binstruction[39]) {  /* code */
      0xA406AA00,  //  0000  IMPORT	R1	K85
      0x6008000C,  //  0001  GETGBL	R2	G12
      0x880C011E,  //  0002  GETMBR	R3	R0	K30
      0x7C080200,  //  0003  CALL	R2	1
      0x2408050A,  //  0004  GT	R2	R2	K10
      0x780A0000,  //  0005  JMPF	R2	#0007
      0x80000400,  //  0006  RET	0
      0x8808010B,  //  0007  GETMBR	R2	R0	K11
      0x740A000E,  //  0008  JMPT	R2	#0018
      0x8C080157,  //  0009  GETMET	R2	R0	K87
      0x7C080200,  //  000A  CALL	R2	1
      0x9002AC02,  //  000B  SETMBR	R0	K86	R2
      0x60080013,  //  000C  GETGBL	R2	G19
      0x7C080000,  //  000D  CALL	R2	0
      0x90023E02,  //  000E  SETMBR	R0	K31	R2
      0x8C080158,  //  000F  GETMET	R2	R0	K88
      0x7C080200,  //  0010  CALL	R2	1
      0xB80A7400,  //  0011  GETNGBL	R2	K58
      0x600C0008,  //  0012  GETGBL	R3	G8
      0x88100156,  //  0013  GETMBR	R4	R0	K86
      0x7C0C0200,  //  0014  CALL	R3	1
      0x000EB203,  //  0015  ADD	R3	K89	R3
      0x58100053,  //  0016  LDCONST	R4	K83
      0x7C080400,  //  0017  CALL	R2	2
      0x8C08015A,  //  0018  GETMET	R2	R0	K90
      0x88100156,  //  0019  GETMBR	R4	R0	K86
      0x7C080400,  //  001A  CALL	R2	2
      0x8808010B,  //  001B  GETMBR	R2	R0	K11
      0x740A0008,  //  001C  JMPT	R2	#0026
      0x88080108,  //  001D  GETMBR	R2	R0	K8
      0x8C080509,  //  001E  GETMET	R2	R2	K9
      0x7C080200,  //  001F  CALL	R2	1
      0x2408050A,  //  0020  GT	R2	R2	K10
      0x780A0003,  //  0021  JMPF	R2	#0026
      0x50080200,  //  0022  LDBOOL	R2	1	0
      0x90021602,  //  0023  SETMBR	R0	K11	R2
      0x8C08010C,  //  0024  GETMET	R2	R0	K12
      0x7C080200,  //  0025  CALL	R2	1
      0x80000000,  //  0026  RET	0
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
      0xA406AA00,  //  0000  IMPORT	R1	K85
      0xB80A2C00,  //  0001  GETNGBL	R2	K22
      0x8C08055B,  //  0002  GETMET	R2	R2	K91
      0x7C080200,  //  0003  CALL	R2	1
      0xB80E2C00,  //  0004  GETNGBL	R3	K22
      0x8C0C075C,  //  0005  GETMET	R3	R3	K92
      0x58140053,  //  0006  LDCONST	R5	K83
      0x7C0C0400,  //  0007  CALL	R3	2
      0x780E0006,  //  0008  JMPF	R3	#0010
      0xB80E7400,  //  0009  GETNGBL	R3	K58
      0x60100008,  //  000A  GETGBL	R4	G8
      0x5C140400,  //  000B  MOVE	R5	R2
      0x7C100200,  //  000C  CALL	R4	1
      0x0012BA04,  //  000D  ADD	R4	K93	R4
      0x58140053,  //  000E  LDCONST	R5	K83
      0x7C0C0400,  //  000F  CALL	R3	2
      0x4C0C0000,  //  0010  LDNIL	R3
      0x1C0C0403,  //  0011  EQ	R3	R2	R3
      0x780E0000,  //  0012  JMPF	R3	#0014
      0x80000600,  //  0013  RET	0
      0x8C0C035E,  //  0014  GETMET	R3	R1	K94
      0x5C140400,  //  0015  MOVE	R5	R2
      0x7C0C0400,  //  0016  CALL	R3	2
      0x4C100000,  //  0017  LDNIL	R4
      0x20100604,  //  0018  NE	R4	R3	R4
      0x7812000D,  //  0019  JMPF	R4	#0028
      0x5810000A,  //  001A  LDCONST	R4	K10
      0x6014000C,  //  001B  GETGBL	R5	G12
      0x8818011E,  //  001C  GETMBR	R6	R0	K30
      0x7C140200,  //  001D  CALL	R5	1
      0x14140805,  //  001E  LT	R5	R4	R5
      0x78160006,  //  001F  JMPF	R5	#0027
      0x8814011E,  //  0020  GETMBR	R5	R0	K30
      0x94140A04,  //  0021  GETIDX	R5	R5	R4
      0x8C140B5F,  //  0022  GETMET	R5	R5	K95
      0x5C1C0600,  //  0023  MOVE	R7	R3
      0x7C140400,  //  0024  CALL	R5	2
      0x00100938,  //  0025  ADD	R4	R4	K56
      0x7001FFF3,  //  0026  JMP		#001B
      0x70020006,  //  0027  JMP		#002F
      0xB8127400,  //  0028  GETNGBL	R4	K58
      0x60140008,  //  0029  GETGBL	R5	G8
      0x5C180400,  //  002A  MOVE	R6	R2
      0x7C140200,  //  002B  CALL	R5	1
      0x0016C005,  //  002C  ADD	R5	K96	R5
      0x58180053,  //  002D  LDCONST	R6	K83
      0x7C100400,  //  002E  CALL	R4	2
      0x80000000,  //  002F  RET	0
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
      0x8810011E,  //  0003  GETMBR	R4	R0	K30
      0x7C0C0200,  //  0004  CALL	R3	1
      0xA8020011,  //  0005  EXBLK	0	#0018
      0x5C100600,  //  0006  MOVE	R4	R3
      0x7C100000,  //  0007  CALL	R4	0
      0x8C140961,  //  0008  GETMET	R5	R4	K97
      0x7C140200,  //  0009  CALL	R5	1
      0x78060002,  //  000A  JMPF	R1	#000E
      0x1C180B0A,  //  000B  EQ	R6	R5	K10
      0x781A0000,  //  000C  JMPF	R6	#000E
      0x7001FFF7,  //  000D  JMP		#0006
      0x8C180539,  //  000E  GETMET	R6	R2	K57
      0x5C200A00,  //  000F  MOVE	R8	R5
      0x7C180400,  //  0010  CALL	R6	2
      0x4C1C0000,  //  0011  LDNIL	R7
      0x1C180C07,  //  0012  EQ	R6	R6	R7
      0x781A0002,  //  0013  JMPF	R6	#0017
      0x8C180562,  //  0014  GETMET	R6	R2	K98
      0x5C200A00,  //  0015  MOVE	R8	R5
      0x7C180400,  //  0016  CALL	R6	2
      0x7001FFED,  //  0017  JMP		#0006
      0x580C0014,  //  0018  LDCONST	R3	K20
      0xAC0C0200,  //  0019  CATCH	R3	1	0
      0xB0080000,  //  001A  RAISE	2	R0	R0
      0x80040400,  //  001B  RET	1	R2
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
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080563,  //  0001  GETMET	R2	R2	K99
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x80040400,  //  0004  RET	1	R2
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
      0xB8062C00,  //  0000  GETNGBL	R1	K22
      0x8C040364,  //  0001  GETMET	R1	R1	K100
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x88040100,  //  0004  GETMBR	R1	R0	K0
      0x78060002,  //  0005  JMPF	R1	#0009
      0x88040100,  //  0006  GETMBR	R1	R0	K0
      0x8C040365,  //  0007  GETMET	R1	R1	K101
      0x7C040200,  //  0008  CALL	R1	1
      0x80000000,  //  0009  RET	0
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
      0x88080156,  //  0001  GETMBR	R2	R0	K86
      0x8C080566,  //  0002  GETMET	R2	R2	K102
      0x7C080200,  //  0003  CALL	R2	1
      0x7C040200,  //  0004  CALL	R1	1
      0xA802000A,  //  0005  EXBLK	0	#0011
      0x5C080200,  //  0006  MOVE	R2	R1
      0x7C080000,  //  0007  CALL	R2	0
      0x600C0009,  //  0008  GETGBL	R3	G9
      0x5C100400,  //  0009  MOVE	R4	R2
      0x7C0C0200,  //  000A  CALL	R3	1
      0x88100120,  //  000B  GETMBR	R4	R0	K32
      0x28100604,  //  000C  GE	R4	R3	R4
      0x78120001,  //  000D  JMPF	R4	#0010
      0x00100738,  //  000E  ADD	R4	R3	K56
      0x90024004,  //  000F  SETMBR	R0	K32	R4
      0x7001FFF4,  //  0010  JMP		#0006
      0x58040014,  //  0011  LDCONST	R1	K20
      0xAC040200,  //  0012  CATCH	R1	1	0
      0xB0080000,  //  0013  RAISE	2	R0	R0
      0x80000000,  //  0014  RET	0
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
      0x88080167,  //  0000  GETMBR	R2	R0	K103
      0x4C0C0000,  //  0001  LDNIL	R3
      0x1C080403,  //  0002  EQ	R2	R2	R3
      0x740A0002,  //  0003  JMPT	R2	#0007
      0x88080167,  //  0004  GETMBR	R2	R0	K103
      0x24080401,  //  0005  GT	R2	R2	R1
      0x780A0005,  //  0006  JMPF	R2	#000D
      0x9002CE01,  //  0007  SETMBR	R0	K103	R1
      0xB80A3000,  //  0008  GETNGBL	R2	K24
      0x8C080569,  //  0009  GETMET	R2	R2	K105
      0x5C100200,  //  000A  MOVE	R4	R1
      0x7C080400,  //  000B  CALL	R2	2
      0x9002D002,  //  000C  SETMBR	R0	K104	R2
      0x80000000,  //  000D  RET	0
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
      0x8C1C016A,  //  000E  GETMET	R7	R0	K106
      0x00240B38,  //  000F  ADD	R9	R5	K56
      0x20280604,  //  0010  NE	R10	R3	R4
      0x782A0001,  //  0011  JMPF	R10	#0014
      0x58280038,  //  0012  LDCONST	R10	K56
      0x70020000,  //  0013  JMP		#0015
      0x5828000A,  //  0014  LDCONST	R10	K10
      0x780E0001,  //  0015  JMPF	R3	#0018
      0x582C000A,  //  0016  LDCONST	R11	K10
      0x70020000,  //  0017  JMP		#0019
      0x582C0038,  //  0018  LDCONST	R11	K56
      0x5C300C00,  //  0019  MOVE	R12	R6
      0x7C1C0A00,  //  001A  CALL	R7	5
      0x80000000,  //  001B  RET	0
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
      0x880C016B,  //  0000  GETMBR	R3	R0	K107
      0x4C100000,  //  0001  LDNIL	R4
      0x1C0C0604,  //  0002  EQ	R3	R3	R4
      0x780E0002,  //  0003  JMPF	R3	#0007
      0x600C0013,  //  0004  GETGBL	R3	G19
      0x7C0C0000,  //  0005  CALL	R3	0
      0x9002D603,  //  0006  SETMBR	R0	K107	R3
      0x4C0C0000,  //  0007  LDNIL	R3
      0x8810016B,  //  0008  GETMBR	R4	R0	K107
      0x8C10096C,  //  0009  GETMET	R4	R4	K108
      0x5C180200,  //  000A  MOVE	R6	R1
      0x7C100400,  //  000B  CALL	R4	2
      0x78120009,  //  000C  JMPF	R4	#0017
      0x8810016B,  //  000D  GETMBR	R4	R0	K107
      0x940C0801,  //  000E  GETIDX	R3	R4	R1
      0x8C14076D,  //  000F  GETMET	R5	R3	K109
      0x7C140200,  //  0010  CALL	R5	1
      0x14140405,  //  0011  LT	R5	R2	R5
      0x78160002,  //  0012  JMPF	R5	#0016
      0x8C14076E,  //  0013  GETMET	R5	R3	K110
      0x5C1C0400,  //  0014  MOVE	R7	R2
      0x7C140400,  //  0015  CALL	R5	2
      0x70020011,  //  0016  JMP		#0029
      0xB8123000,  //  0017  GETNGBL	R4	K24
      0x8C10096F,  //  0018  GETMET	R4	R4	K111
      0x5C180000,  //  0019  MOVE	R6	R0
      0x5C1C0200,  //  001A  MOVE	R7	R1
      0x5C200400,  //  001B  MOVE	R8	R2
      0x7C100800,  //  001C  CALL	R4	4
      0x5C0C0800,  //  001D  MOVE	R3	R4
      0x8810011F,  //  001E  GETMBR	R4	R0	K31
      0x8C10096C,  //  001F  GETMET	R4	R4	K108
      0x5C180200,  //  0020  MOVE	R6	R1
      0x7C100400,  //  0021  CALL	R4	2
      0x78120003,  //  0022  JMPF	R4	#0027
      0x8C100770,  //  0023  GETMET	R4	R3	K112
      0x8818011F,  //  0024  GETMBR	R6	R0	K31
      0x94180C01,  //  0025  GETIDX	R6	R6	R1
      0x7C100400,  //  0026  CALL	R4	2
      0x8810016B,  //  0027  GETMBR	R4	R0	K107
      0x98100203,  //  0028  SETIDX	R4	R1	R3
      0x80040600,  //  0029  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _instantiate_plugins_from_config
********************************************************************/
be_local_closure(class_Matter_Device__instantiate_plugins_from_config,   /* name */
  be_nested_proto(
    17,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(_instantiate_plugins_from_config),
    &be_const_str_solidified,
    ( &(const binstruction[146]) {  /* code */
      0x8C080171,  //  0000  GETMET	R2	R0	K113
      0x5C100200,  //  0001  MOVE	R4	R1
      0x7C080400,  //  0002  CALL	R2	2
      0x880C011E,  //  0003  GETMBR	R3	R0	K30
      0x8C0C0762,  //  0004  GETMET	R3	R3	K98
      0xB8163000,  //  0005  GETNGBL	R5	K24
      0x8C140B72,  //  0006  GETMET	R5	R5	K114
      0x5C1C0000,  //  0007  MOVE	R7	R0
      0x5820000A,  //  0008  LDCONST	R8	K10
      0x60240013,  //  0009  GETGBL	R9	G19
      0x7C240000,  //  000A  CALL	R9	0
      0x7C140800,  //  000B  CALL	R5	4
      0x7C0C0400,  //  000C  CALL	R3	2
      0xB80E7400,  //  000D  GETNGBL	R3	K58
      0x58100073,  //  000E  LDCONST	R4	K115
      0x58140021,  //  000F  LDCONST	R5	K33
      0x7C0C0400,  //  0010  CALL	R3	2
      0xB80E7400,  //  0011  GETNGBL	R3	K58
      0x60100018,  //  0012  GETGBL	R4	G24
      0x58140074,  //  0013  LDCONST	R5	K116
      0x5818000A,  //  0014  LDCONST	R6	K10
      0x581C0075,  //  0015  LDCONST	R7	K117
      0x58200010,  //  0016  LDCONST	R8	K16
      0x7C100800,  //  0017  CALL	R4	4
      0x58140021,  //  0018  LDCONST	R5	K33
      0x7C0C0400,  //  0019  CALL	R3	2
      0x880C011E,  //  001A  GETMBR	R3	R0	K30
      0x8C0C0762,  //  001B  GETMET	R3	R3	K98
      0xB8163000,  //  001C  GETNGBL	R5	K24
      0x8C140B76,  //  001D  GETMET	R5	R5	K118
      0x5C1C0000,  //  001E  MOVE	R7	R0
      0xB8223000,  //  001F  GETNGBL	R8	K24
      0x88201154,  //  0020  GETMBR	R8	R8	K84
      0x60240013,  //  0021  GETGBL	R9	G19
      0x7C240000,  //  0022  CALL	R9	0
      0x7C140800,  //  0023  CALL	R5	4
      0x7C0C0400,  //  0024  CALL	R3	2
      0xB80E7400,  //  0025  GETNGBL	R3	K58
      0x60100018,  //  0026  GETGBL	R4	G24
      0x58140074,  //  0027  LDCONST	R5	K116
      0xB81A3000,  //  0028  GETNGBL	R6	K24
      0x88180D54,  //  0029  GETMBR	R6	R6	K84
      0x581C0077,  //  002A  LDCONST	R7	K119
      0x58200010,  //  002B  LDCONST	R8	K16
      0x7C100800,  //  002C  CALL	R4	4
      0x58140021,  //  002D  LDCONST	R5	K33
      0x7C0C0400,  //  002E  CALL	R3	2
      0x600C0010,  //  002F  GETGBL	R3	G16
      0x5C100400,  //  0030  MOVE	R4	R2
      0x7C0C0200,  //  0031  CALL	R3	1
      0xA8020055,  //  0032  EXBLK	0	#0089
      0x5C100600,  //  0033  MOVE	R4	R3
      0x7C100000,  //  0034  CALL	R4	0
      0x1C14090A,  //  0035  EQ	R5	R4	K10
      0x78160000,  //  0036  JMPF	R5	#0038
      0x7001FFFA,  //  0037  JMP		#0033
      0xA802003E,  //  0038  EXBLK	0	#0078
      0x60140008,  //  0039  GETGBL	R5	G8
      0x5C180800,  //  003A  MOVE	R6	R4
      0x7C140200,  //  003B  CALL	R5	1
      0x94140205,  //  003C  GETIDX	R5	R1	R5
      0x8C180B39,  //  003D  GETMET	R6	R5	K57
      0x58200012,  //  003E  LDCONST	R8	K18
      0x7C180400,  //  003F  CALL	R6	2
      0x4C1C0000,  //  0040  LDNIL	R7
      0x1C1C0C07,  //  0041  EQ	R7	R6	R7
      0x781E0005,  //  0042  JMPF	R7	#0049
      0xB81E7400,  //  0043  GETNGBL	R7	K58
      0x58200078,  //  0044  LDCONST	R8	K120
      0x58240053,  //  0045  LDCONST	R9	K83
      0x7C1C0400,  //  0046  CALL	R7	2
      0xA8040001,  //  0047  EXBLK	1	1
      0x7001FFE9,  //  0048  JMP		#0033
      0x1C1C0D75,  //  0049  EQ	R7	R6	K117
      0x781E0005,  //  004A  JMPF	R7	#0051
      0xB81E7400,  //  004B  GETNGBL	R7	K58
      0x58200079,  //  004C  LDCONST	R8	K121
      0x58240053,  //  004D  LDCONST	R9	K83
      0x7C1C0400,  //  004E  CALL	R7	2
      0xA8040001,  //  004F  EXBLK	1	1
      0x7001FFE1,  //  0050  JMP		#0033
      0x881C017A,  //  0051  GETMBR	R7	R0	K122
      0x8C1C0F39,  //  0052  GETMET	R7	R7	K57
      0x5C240C00,  //  0053  MOVE	R9	R6
      0x7C1C0400,  //  0054  CALL	R7	2
      0x4C200000,  //  0055  LDNIL	R8
      0x1C200E08,  //  0056  EQ	R8	R7	R8
      0x78220009,  //  0057  JMPF	R8	#0062
      0xB8227400,  //  0058  GETNGBL	R8	K58
      0x60240008,  //  0059  GETGBL	R9	G8
      0x5C280C00,  //  005A  MOVE	R10	R6
      0x7C240200,  //  005B  CALL	R9	1
      0x0026F609,  //  005C  ADD	R9	K123	R9
      0x0024137C,  //  005D  ADD	R9	R9	K124
      0x58280021,  //  005E  LDCONST	R10	K33
      0x7C200400,  //  005F  CALL	R8	2
      0xA8040001,  //  0060  EXBLK	1	1
      0x7001FFD0,  //  0061  JMP		#0033
      0x5C200E00,  //  0062  MOVE	R8	R7
      0x5C240000,  //  0063  MOVE	R9	R0
      0x5C280800,  //  0064  MOVE	R10	R4
      0x5C2C0A00,  //  0065  MOVE	R11	R5
      0x7C200600,  //  0066  CALL	R8	3
      0x8824011E,  //  0067  GETMBR	R9	R0	K30
      0x8C241362,  //  0068  GETMET	R9	R9	K98
      0x5C2C1000,  //  0069  MOVE	R11	R8
      0x7C240400,  //  006A  CALL	R9	2
      0xB8267400,  //  006B  GETNGBL	R9	K58
      0x60280018,  //  006C  GETGBL	R10	G24
      0x582C0074,  //  006D  LDCONST	R11	K116
      0x5C300800,  //  006E  MOVE	R12	R4
      0x5C340C00,  //  006F  MOVE	R13	R6
      0x8C38017D,  //  0070  GETMET	R14	R0	K125
      0x5C400A00,  //  0071  MOVE	R16	R5
      0x7C380400,  //  0072  CALL	R14	2
      0x7C280800,  //  0073  CALL	R10	4
      0x582C0021,  //  0074  LDCONST	R11	K33
      0x7C240400,  //  0075  CALL	R9	2
      0xA8040001,  //  0076  EXBLK	1	1
      0x7002000F,  //  0077  JMP		#0088
      0xAC140002,  //  0078  CATCH	R5	0	2
      0x7002000C,  //  0079  JMP		#0087
      0xB81E7400,  //  007A  GETNGBL	R7	K58
      0x60200008,  //  007B  GETGBL	R8	G8
      0x5C240A00,  //  007C  MOVE	R9	R5
      0x7C200200,  //  007D  CALL	R8	1
      0x0022FC08,  //  007E  ADD	R8	K126	R8
      0x0020117F,  //  007F  ADD	R8	R8	K127
      0x60240008,  //  0080  GETGBL	R9	G8
      0x5C280C00,  //  0081  MOVE	R10	R6
      0x7C240200,  //  0082  CALL	R9	1
      0x00201009,  //  0083  ADD	R8	R8	R9
      0x58240021,  //  0084  LDCONST	R9	K33
      0x7C1C0400,  //  0085  CALL	R7	2
      0x70020000,  //  0086  JMP		#0088
      0xB0080000,  //  0087  RAISE	2	R0	R0
      0x7001FFA9,  //  0088  JMP		#0033
      0x580C0014,  //  0089  LDCONST	R3	K20
      0xAC0C0200,  //  008A  CATCH	R3	1	0
      0xB0080000,  //  008B  RAISE	2	R0	R0
      0xB80E2C00,  //  008C  GETNGBL	R3	K22
      0x8C0C0780,  //  008D  GETMET	R3	R3	K128
      0x58140081,  //  008E  LDCONST	R5	K129
      0x58180082,  //  008F  LDCONST	R6	K130
      0x7C0C0600,  //  0090  CALL	R3	3
      0x80000000,  //  0091  RET	0
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
      0x5804000F,  //  0000  LDCONST	R1	K15
      0x60080010,  //  0001  GETGBL	R2	G16
      0x600C000C,  //  0002  GETGBL	R3	G12
      0x5C100000,  //  0003  MOVE	R4	R0
      0x7C0C0200,  //  0004  CALL	R3	1
      0x040C0738,  //  0005  SUB	R3	R3	K56
      0x400E7003,  //  0006  CONNECT	R3	K56	R3
      0x7C080200,  //  0007  CALL	R2	1
      0xA8020010,  //  0008  EXBLK	0	#001A
      0x5C0C0400,  //  0009  MOVE	R3	R2
      0x7C0C0000,  //  000A  CALL	R3	0
      0x94100003,  //  000B  GETIDX	R4	R0	R3
      0x5C140600,  //  000C  MOVE	R5	R3
      0x24180B0A,  //  000D  GT	R6	R5	K10
      0x781A0008,  //  000E  JMPF	R6	#0018
      0x04180B38,  //  000F  SUB	R6	R5	K56
      0x94180006,  //  0010  GETIDX	R6	R0	R6
      0x24180C04,  //  0011  GT	R6	R6	R4
      0x781A0004,  //  0012  JMPF	R6	#0018
      0x04180B38,  //  0013  SUB	R6	R5	K56
      0x94180006,  //  0014  GETIDX	R6	R0	R6
      0x98000A06,  //  0015  SETIDX	R0	R5	R6
      0x04140B38,  //  0016  SUB	R5	R5	K56
      0x7001FFF4,  //  0017  JMP		#000D
      0x98000A04,  //  0018  SETIDX	R0	R5	R4
      0x7001FFEE,  //  0019  JMP		#0009
      0x58080014,  //  001A  LDCONST	R2	K20
      0xAC080200,  //  001B  CATCH	R2	1	0
      0xB0080000,  //  001C  RAISE	2	R0	R0
      0x58080038,  //  001D  LDCONST	R2	K56
      0x600C000C,  //  001E  GETGBL	R3	G12
      0x5C100000,  //  001F  MOVE	R4	R0
      0x7C0C0200,  //  0020  CALL	R3	1
      0x180C0738,  //  0021  LE	R3	R3	K56
      0x780E0000,  //  0022  JMPF	R3	#0024
      0x80040000,  //  0023  RET	1	R0
      0x940C010A,  //  0024  GETIDX	R3	R0	K10
      0x6010000C,  //  0025  GETGBL	R4	G12
      0x5C140000,  //  0026  MOVE	R5	R0
      0x7C100200,  //  0027  CALL	R4	1
      0x14100404,  //  0028  LT	R4	R2	R4
      0x78120009,  //  0029  JMPF	R4	#0034
      0x94100002,  //  002A  GETIDX	R4	R0	R2
      0x1C100803,  //  002B  EQ	R4	R4	R3
      0x78120003,  //  002C  JMPF	R4	#0031
      0x8C100183,  //  002D  GETMET	R4	R0	K131
      0x5C180400,  //  002E  MOVE	R6	R2
      0x7C100400,  //  002F  CALL	R4	2
      0x70020001,  //  0030  JMP		#0033
      0x940C0002,  //  0031  GETIDX	R3	R0	R2
      0x00080538,  //  0032  ADD	R2	R2	K56
      0x7001FFF0,  //  0033  JMP		#0025
      0x80040000,  //  0034  RET	1	R0
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
      0x8808017A,  //  0000  GETMBR	R2	R0	K122
      0x8C080539,  //  0001  GETMET	R2	R2	K57
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x780A0001,  //  0004  JMPF	R2	#0007
      0x880C0584,  //  0005  GETMBR	R3	R2	K132
      0x70020000,  //  0006  JMP		#0008
      0x580C0010,  //  0007  LDCONST	R3	K16
      0x80040600,  //  0008  RET	1	R3
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
      0x5808000A,  //  0000  LDCONST	R2	K10
      0x600C000C,  //  0001  GETGBL	R3	G12
      0x8810011E,  //  0002  GETMBR	R4	R0	K30
      0x7C0C0200,  //  0003  CALL	R3	1
      0x140C0403,  //  0004  LT	R3	R2	R3
      0x780E0008,  //  0005  JMPF	R3	#000F
      0x880C011E,  //  0006  GETMBR	R3	R0	K30
      0x940C0602,  //  0007  GETIDX	R3	R3	R2
      0x8C100761,  //  0008  GETMET	R4	R3	K97
      0x7C100200,  //  0009  CALL	R4	1
      0x1C100801,  //  000A  EQ	R4	R4	R1
      0x78120000,  //  000B  JMPF	R4	#000D
      0x80040600,  //  000C  RET	1	R3
      0x00080538,  //  000D  ADD	R2	R2	K56
      0x7001FFF1,  //  000E  JMP		#0001
      0x4C0C0000,  //  000F  LDNIL	R3
      0x80040600,  //  0010  RET	1	R3
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
      0xA4070A00,  //  0000  IMPORT	R1	K133
      0x8808016B,  //  0001  GETMBR	R2	R0	K107
      0x780A004B,  //  0002  JMPF	R2	#004F
      0x60080013,  //  0003  GETGBL	R2	G19
      0x7C080000,  //  0004  CALL	R2	0
      0x600C0010,  //  0005  GETGBL	R3	G16
      0x8810016B,  //  0006  GETMBR	R4	R0	K107
      0x7C0C0200,  //  0007  CALL	R3	1
      0xA8020003,  //  0008  EXBLK	0	#000D
      0x5C100600,  //  0009  MOVE	R4	R3
      0x7C100000,  //  000A  CALL	R4	0
      0x9808090A,  //  000B  SETIDX	R2	R4	K10
      0x7001FFFB,  //  000C  JMP		#0009
      0x580C0014,  //  000D  LDCONST	R3	K20
      0xAC0C0200,  //  000E  CATCH	R3	1	0
      0xB0080000,  //  000F  RAISE	2	R0	R0
      0x600C0010,  //  0010  GETGBL	R3	G16
      0x8810011E,  //  0011  GETMBR	R4	R0	K30
      0x7C0C0200,  //  0012  CALL	R3	1
      0xA802000F,  //  0013  EXBLK	0	#0024
      0x5C100600,  //  0014  MOVE	R4	R3
      0x7C100000,  //  0015  CALL	R4	0
      0x8C140386,  //  0016  GETMET	R5	R1	K134
      0x5C1C0800,  //  0017  MOVE	R7	R4
      0x58200087,  //  0018  LDCONST	R8	K135
      0x7C140600,  //  0019  CALL	R5	3
      0x4C180000,  //  001A  LDNIL	R6
      0x20180A06,  //  001B  NE	R6	R5	R6
      0x781A0005,  //  001C  JMPF	R6	#0023
      0x8C180539,  //  001D  GETMET	R6	R2	K57
      0x5C200A00,  //  001E  MOVE	R8	R5
      0x5824000A,  //  001F  LDCONST	R9	K10
      0x7C180600,  //  0020  CALL	R6	3
      0x00180D38,  //  0021  ADD	R6	R6	K56
      0x98080A06,  //  0022  SETIDX	R2	R5	R6
      0x7001FFEF,  //  0023  JMP		#0014
      0x580C0014,  //  0024  LDCONST	R3	K20
      0xAC0C0200,  //  0025  CATCH	R3	1	0
      0xB0080000,  //  0026  RAISE	2	R0	R0
      0x600C0012,  //  0027  GETGBL	R3	G18
      0x7C0C0000,  //  0028  CALL	R3	0
      0x60100010,  //  0029  GETGBL	R4	G16
      0x8C140566,  //  002A  GETMET	R5	R2	K102
      0x7C140200,  //  002B  CALL	R5	1
      0x7C100200,  //  002C  CALL	R4	1
      0xA8020008,  //  002D  EXBLK	0	#0037
      0x5C140800,  //  002E  MOVE	R5	R4
      0x7C140000,  //  002F  CALL	R5	0
      0x94180405,  //  0030  GETIDX	R6	R2	R5
      0x1C180D0A,  //  0031  EQ	R6	R6	K10
      0x781A0002,  //  0032  JMPF	R6	#0036
      0x8C180762,  //  0033  GETMET	R6	R3	K98
      0x5C200A00,  //  0034  MOVE	R8	R5
      0x7C180400,  //  0035  CALL	R6	2
      0x7001FFF6,  //  0036  JMP		#002E
      0x58100014,  //  0037  LDCONST	R4	K20
      0xAC100200,  //  0038  CATCH	R4	1	0
      0xB0080000,  //  0039  RAISE	2	R0	R0
      0x60100010,  //  003A  GETGBL	R4	G16
      0x5C140600,  //  003B  MOVE	R5	R3
      0x7C100200,  //  003C  CALL	R4	1
      0xA802000D,  //  003D  EXBLK	0	#004C
      0x5C140800,  //  003E  MOVE	R5	R4
      0x7C140000,  //  003F  CALL	R5	0
      0xB81A7400,  //  0040  GETNGBL	R6	K58
      0x881C0B89,  //  0041  GETMBR	R7	R5	K137
      0x001F1007,  //  0042  ADD	R7	K136	R7
      0x58200053,  //  0043  LDCONST	R8	K83
      0x7C180400,  //  0044  CALL	R6	2
      0x8C180B8A,  //  0045  GETMET	R6	R5	K138
      0x7C180200,  //  0046  CALL	R6	1
      0x8818016B,  //  0047  GETMBR	R6	R0	K107
      0x8C180D83,  //  0048  GETMET	R6	R6	K131
      0x88200B89,  //  0049  GETMBR	R8	R5	K137
      0x7C180400,  //  004A  CALL	R6	2
      0x7001FFF1,  //  004B  JMP		#003E
      0x58100014,  //  004C  LDCONST	R4	K20
      0xAC100200,  //  004D  CATCH	R4	1	0
      0xB0080000,  //  004E  RAISE	2	R0	R0
      0x80000000,  //  004F  RET	0
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
    ( &(const binstruction[77]) {  /* code */
      0x50040000,  //  0000  LDBOOL	R1	0	0
      0x60080012,  //  0001  GETGBL	R2	G18
      0x7C080000,  //  0002  CALL	R2	0
      0x600C0010,  //  0003  GETGBL	R3	G16
      0x88100156,  //  0004  GETMBR	R4	R0	K86
      0x8C100966,  //  0005  GETMET	R4	R4	K102
      0x7C100200,  //  0006  CALL	R4	1
      0x7C0C0200,  //  0007  CALL	R3	1
      0xA8020007,  //  0008  EXBLK	0	#0011
      0x5C100600,  //  0009  MOVE	R4	R3
      0x7C100000,  //  000A  CALL	R4	0
      0x8C140562,  //  000B  GETMET	R5	R2	K98
      0x601C0009,  //  000C  GETGBL	R7	G9
      0x5C200800,  //  000D  MOVE	R8	R4
      0x7C1C0200,  //  000E  CALL	R7	1
      0x7C140400,  //  000F  CALL	R5	2
      0x7001FFF7,  //  0010  JMP		#0009
      0x580C0014,  //  0011  LDCONST	R3	K20
      0xAC0C0200,  //  0012  CATCH	R3	1	0
      0xB0080000,  //  0013  RAISE	2	R0	R0
      0x600C0010,  //  0014  GETGBL	R3	G16
      0x5C100400,  //  0015  MOVE	R4	R2
      0x7C0C0200,  //  0016  CALL	R3	1
      0xA8020030,  //  0017  EXBLK	0	#0049
      0x5C100600,  //  0018  MOVE	R4	R3
      0x7C100000,  //  0019  CALL	R4	0
      0x1C14090A,  //  001A  EQ	R5	R4	K10
      0x7816000B,  //  001B  JMPF	R5	#0028
      0xB8167400,  //  001C  GETNGBL	R5	K58
      0x5818008B,  //  001D  LDCONST	R6	K139
      0x581C0021,  //  001E  LDCONST	R7	K33
      0x7C140400,  //  001F  CALL	R5	2
      0x88140156,  //  0020  GETMBR	R5	R0	K86
      0x8C140B83,  //  0021  GETMET	R5	R5	K131
      0x601C0008,  //  0022  GETGBL	R7	G8
      0x5C200800,  //  0023  MOVE	R8	R4
      0x7C1C0200,  //  0024  CALL	R7	1
      0x7C140400,  //  0025  CALL	R5	2
      0x50040200,  //  0026  LDBOOL	R1	1	0
      0x7002001F,  //  0027  JMP		#0048
      0xB8163000,  //  0028  GETNGBL	R5	K24
      0x88140B54,  //  0029  GETMBR	R5	R5	K84
      0x1C140805,  //  002A  EQ	R5	R4	R5
      0x7816001B,  //  002B  JMPF	R5	#0048
      0x50040200,  //  002C  LDBOOL	R1	1	0
      0xB8167400,  //  002D  GETNGBL	R5	K58
      0x60180018,  //  002E  GETGBL	R6	G24
      0x581C008C,  //  002F  LDCONST	R7	K140
      0x5C200800,  //  0030  MOVE	R8	R4
      0x88240120,  //  0031  GETMBR	R9	R0	K32
      0x7C180600,  //  0032  CALL	R6	3
      0x581C0021,  //  0033  LDCONST	R7	K33
      0x7C140400,  //  0034  CALL	R5	2
      0x60140008,  //  0035  GETGBL	R5	G8
      0x88180120,  //  0036  GETMBR	R6	R0	K32
      0x7C140200,  //  0037  CALL	R5	1
      0x88180156,  //  0038  GETMBR	R6	R0	K86
      0x601C0008,  //  0039  GETGBL	R7	G8
      0x5C200800,  //  003A  MOVE	R8	R4
      0x7C1C0200,  //  003B  CALL	R7	1
      0x88200156,  //  003C  GETMBR	R8	R0	K86
      0x941C1007,  //  003D  GETIDX	R7	R8	R7
      0x98180A07,  //  003E  SETIDX	R6	R5	R7
      0x88140156,  //  003F  GETMBR	R5	R0	K86
      0x8C140B83,  //  0040  GETMET	R5	R5	K131
      0x601C0008,  //  0041  GETGBL	R7	G8
      0x5C200800,  //  0042  MOVE	R8	R4
      0x7C1C0200,  //  0043  CALL	R7	1
      0x7C140400,  //  0044  CALL	R5	2
      0x88140120,  //  0045  GETMBR	R5	R0	K32
      0x00140B38,  //  0046  ADD	R5	R5	K56
      0x90024005,  //  0047  SETMBR	R0	K32	R5
      0x7001FFCE,  //  0048  JMP		#0018
      0x580C0014,  //  0049  LDCONST	R3	K20
      0xAC0C0200,  //  004A  CATCH	R3	1	0
      0xB0080000,  //  004B  RAISE	2	R0	R0
      0x80040200,  //  004C  RET	1	R1
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
      0x88040108,  //  0000  GETMBR	R1	R0	K8
      0x8C04038D,  //  0001  GETMET	R1	R1	K141
      0x7C040200,  //  0002  CALL	R1	1
      0x88040127,  //  0003  GETMBR	R1	R0	K39
      0x8C04038D,  //  0004  GETMET	R1	R1	K141
      0x7C040200,  //  0005  CALL	R1	1
      0x88040129,  //  0006  GETMBR	R1	R0	K41
      0x8C04038D,  //  0007  GETMET	R1	R1	K141
      0x7C040200,  //  0008  CALL	R1	1
      0x88040106,  //  0009  GETMBR	R1	R0	K6
      0x8C04038D,  //  000A  GETMET	R1	R1	K141
      0x7C040200,  //  000B  CALL	R1	1
      0x80000000,  //  000C  RET	0
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
      0xB8163000,  //  0004  GETNGBL	R5	K24
      0x8C140B8E,  //  0005  GETMET	R5	R5	K142
      0x7C140200,  //  0006  CALL	R5	1
      0x90169001,  //  0007  SETMBR	R5	K72	R1
      0x90169202,  //  0008  SETMBR	R5	K73	R2
      0x90169403,  //  0009  SETMBR	R5	K74	R3
      0x88180127,  //  000A  GETMBR	R6	R0	K39
      0x88180D42,  //  000B  GETMBR	R6	R6	K66
      0x88180D43,  //  000C  GETMBR	R6	R6	K67
      0x8C180D8F,  //  000D  GETMET	R6	R6	K143
      0x5C200A00,  //  000E  MOVE	R8	R5
      0x5C240800,  //  000F  MOVE	R9	R4
      0x7C180600,  //  0010  CALL	R6	3
      0x80000000,  //  0011  RET	0
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
      0x5804000F,  //  0000  LDCONST	R1	K15
      0x60080012,  //  0001  GETGBL	R2	G18
      0x7C080000,  //  0002  CALL	R2	0
      0x4C0C0000,  //  0003  LDNIL	R3
      0x1C0C0003,  //  0004  EQ	R3	R0	R3
      0x780E0000,  //  0005  JMPF	R3	#0007
      0x80040400,  //  0006  RET	1	R2
      0x600C0010,  //  0007  GETGBL	R3	G16
      0x8C100166,  //  0008  GETMET	R4	R0	K102
      0x7C100200,  //  0009  CALL	R4	1
      0x7C0C0200,  //  000A  CALL	R3	1
      0xA8020007,  //  000B  EXBLK	0	#0014
      0x5C100600,  //  000C  MOVE	R4	R3
      0x7C100000,  //  000D  CALL	R4	0
      0x8C140562,  //  000E  GETMET	R5	R2	K98
      0x601C0009,  //  000F  GETGBL	R7	G9
      0x5C200800,  //  0010  MOVE	R8	R4
      0x7C1C0200,  //  0011  CALL	R7	1
      0x7C140400,  //  0012  CALL	R5	2
      0x7001FFF7,  //  0013  JMP		#000C
      0x580C0014,  //  0014  LDCONST	R3	K20
      0xAC0C0200,  //  0015  CATCH	R3	1	0
      0xB0080000,  //  0016  RAISE	2	R0	R0
      0x600C0010,  //  0017  GETGBL	R3	G16
      0x6010000C,  //  0018  GETGBL	R4	G12
      0x5C140400,  //  0019  MOVE	R5	R2
      0x7C100200,  //  001A  CALL	R4	1
      0x04100938,  //  001B  SUB	R4	R4	K56
      0x40127004,  //  001C  CONNECT	R4	K56	R4
      0x7C0C0200,  //  001D  CALL	R3	1
      0xA8020010,  //  001E  EXBLK	0	#0030
      0x5C100600,  //  001F  MOVE	R4	R3
      0x7C100000,  //  0020  CALL	R4	0
      0x94140404,  //  0021  GETIDX	R5	R2	R4
      0x5C180800,  //  0022  MOVE	R6	R4
      0x241C0D0A,  //  0023  GT	R7	R6	K10
      0x781E0008,  //  0024  JMPF	R7	#002E
      0x041C0D38,  //  0025  SUB	R7	R6	K56
      0x941C0407,  //  0026  GETIDX	R7	R2	R7
      0x241C0E05,  //  0027  GT	R7	R7	R5
      0x781E0004,  //  0028  JMPF	R7	#002E
      0x041C0D38,  //  0029  SUB	R7	R6	K56
      0x941C0407,  //  002A  GETIDX	R7	R2	R7
      0x98080C07,  //  002B  SETIDX	R2	R6	R7
      0x04180D38,  //  002C  SUB	R6	R6	K56
      0x7001FFF4,  //  002D  JMP		#0023
      0x98080C05,  //  002E  SETIDX	R2	R6	R5
      0x7001FFEE,  //  002F  JMP		#001F
      0x580C0014,  //  0030  LDCONST	R3	K20
      0xAC0C0200,  //  0031  CATCH	R3	1	0
      0xB0080000,  //  0032  RAISE	2	R0	R0
      0x80040400,  //  0033  RET	1	R2
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
      0x8C14016A,  //  0006  GETMET	R5	R0	K106
      0x001C0938,  //  0007  ADD	R7	R4	K56
      0x58200021,  //  0008  LDCONST	R8	K33
      0x5824000A,  //  0009  LDCONST	R9	K10
      0x5C280600,  //  000A  MOVE	R10	R3
      0x7C140A00,  //  000B  CALL	R5	5
      0x80000000,  //  000C  RET	0
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
      0x8C040190,  //  0000  GETMET	R1	R0	K144
      0x7C040200,  //  0001  CALL	R1	1
      0x5804000A,  //  0002  LDCONST	R1	K10
      0x6008000C,  //  0003  GETGBL	R2	G12
      0x880C011E,  //  0004  GETMBR	R3	R0	K30
      0x7C080200,  //  0005  CALL	R2	1
      0x14080202,  //  0006  LT	R2	R1	R2
      0x780A0005,  //  0007  JMPF	R2	#000E
      0x8808011E,  //  0008  GETMBR	R2	R0	K30
      0x94080401,  //  0009  GETIDX	R2	R2	R1
      0x8C080591,  //  000A  GETMET	R2	R2	K145
      0x7C080200,  //  000B  CALL	R2	1
      0x00040338,  //  000C  ADD	R1	R1	K56
      0x7001FFF4,  //  000D  JMP		#0003
      0x80000000,  //  000E  RET	0
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
      0x880C0348,  //  0000  GETMBR	R3	R1	K72
      0x88100349,  //  0001  GETMBR	R4	R1	K73
      0x8814034A,  //  0002  GETMBR	R5	R1	K74
      0xB81A3000,  //  0003  GETNGBL	R6	K24
      0x8C180D92,  //  0004  GETMET	R6	R6	K146
      0x5C200000,  //  0005  MOVE	R8	R0
      0x7C180400,  //  0006  CALL	R6	2
      0x8C1C0D2F,  //  0007  GETMET	R7	R6	K47
      0x5C240600,  //  0008  MOVE	R9	R3
      0x5C280800,  //  0009  MOVE	R10	R4
      0x5C2C0A00,  //  000A  MOVE	R11	R5
      0x7C1C0800,  //  000B  CALL	R7	4
      0x8C1C0D93,  //  000C  GETMET	R7	R6	K147
      0x7C1C0200,  //  000D  CALL	R7	1
      0x4C200000,  //  000E  LDNIL	R8
      0x8C240D94,  //  000F  GETMET	R9	R6	K148
      0x7C240200,  //  0010  CALL	R9	1
      0x5C201200,  //  0011  MOVE	R8	R9
      0x4C280000,  //  0012  LDNIL	R10
      0x2024120A,  //  0013  NE	R9	R9	R10
      0x78260005,  //  0014  JMPF	R9	#001B
      0x5C240400,  //  0015  MOVE	R9	R2
      0x8C280D95,  //  0016  GETMET	R10	R6	K149
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
** Solidified function: autoconf_device_map
********************************************************************/
be_local_closure(class_Matter_Device_autoconf_device_map,   /* name */
  be_nested_proto(
    19,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(autoconf_device_map),
    &be_const_str_solidified,
    ( &(const binstruction[262]) {  /* code */
      0xA406AA00,  //  0000  IMPORT	R1	K85
      0x60080013,  //  0001  GETGBL	R2	G19
      0x7C080000,  //  0002  CALL	R2	0
      0xB80E3000,  //  0003  GETNGBL	R3	K24
      0x880C0796,  //  0004  GETMBR	R3	R3	K150
      0x5810000A,  //  0005  LDCONST	R4	K10
      0xA4172E00,  //  0006  IMPORT	R5	K151
      0x8C180B86,  //  0007  GETMET	R6	R5	K134
      0x5820000A,  //  0008  LDCONST	R8	K10
      0x7C180400,  //  0009  CALL	R6	2
      0x4C1C0000,  //  000A  LDNIL	R7
      0x201C0C07,  //  000B  NE	R7	R6	R7
      0x781E0066,  //  000C  JMPF	R7	#0074
      0x601C000C,  //  000D  GETGBL	R7	G12
      0x8C200D39,  //  000E  GETMET	R8	R6	K57
      0x58280098,  //  000F  LDCONST	R10	K152
      0x582C0010,  //  0010  LDCONST	R11	K16
      0x7C200600,  //  0011  CALL	R8	3
      0x7C1C0200,  //  0012  CALL	R7	1
      0x58100038,  //  0013  LDCONST	R4	K56
      0x24200F0A,  //  0014  GT	R8	R7	K10
      0x7822005D,  //  0015  JMPF	R8	#0074
      0x1C200F38,  //  0016  EQ	R8	R7	K56
      0x7822001E,  //  0017  JMPF	R8	#0037
      0x60200008,  //  0018  GETGBL	R8	G8
      0x5C240600,  //  0019  MOVE	R9	R3
      0x7C200200,  //  001A  CALL	R8	1
      0x60240013,  //  001B  GETGBL	R9	G19
      0x7C240000,  //  001C  CALL	R9	0
      0x98262599,  //  001D  SETIDX	R9	K18	K153
      0x98081009,  //  001E  SETIDX	R2	R8	R9
      0x000C0738,  //  001F  ADD	R3	R3	K56
      0x58200038,  //  0020  LDCONST	R8	K56
      0x4C240000,  //  0021  LDNIL	R9
      0x8C280B86,  //  0022  GETMET	R10	R5	K134
      0x5C301000,  //  0023  MOVE	R12	R8
      0x7C280400,  //  0024  CALL	R10	2
      0x5C241400,  //  0025  MOVE	R9	R10
      0x4C2C0000,  //  0026  LDNIL	R11
      0x2028140B,  //  0027  NE	R10	R10	R11
      0x782A000C,  //  0028  JMPF	R10	#0036
      0x60280008,  //  0029  GETGBL	R10	G8
      0x5C2C0600,  //  002A  MOVE	R11	R3
      0x7C280200,  //  002B  CALL	R10	1
      0x602C0013,  //  002C  GETGBL	R11	G19
      0x7C2C0000,  //  002D  CALL	R11	0
      0x982E2599,  //  002E  SETIDX	R11	K18	K153
      0x00301138,  //  002F  ADD	R12	R8	K56
      0x982F2E0C,  //  0030  SETIDX	R11	K151	R12
      0x9808140B,  //  0031  SETIDX	R2	R10	R11
      0x000C0738,  //  0032  ADD	R3	R3	K56
      0x00100938,  //  0033  ADD	R4	R4	K56
      0x00201138,  //  0034  ADD	R8	R8	K56
      0x7001FFEB,  //  0035  JMP		#0022
      0x7002003C,  //  0036  JMP		#0074
      0x1C200F21,  //  0037  EQ	R8	R7	K33
      0x78220008,  //  0038  JMPF	R8	#0042
      0x60200008,  //  0039  GETGBL	R8	G8
      0x5C240600,  //  003A  MOVE	R9	R3
      0x7C200200,  //  003B  CALL	R8	1
      0x60240013,  //  003C  GETGBL	R9	G19
      0x7C240000,  //  003D  CALL	R9	0
      0x9826259A,  //  003E  SETIDX	R9	K18	K154
      0x98081009,  //  003F  SETIDX	R2	R8	R9
      0x000C0738,  //  0040  ADD	R3	R3	K56
      0x70020031,  //  0041  JMP		#0074
      0x1C200F53,  //  0042  EQ	R8	R7	K83
      0x7822002B,  //  0043  JMPF	R8	#0070
      0x60200008,  //  0044  GETGBL	R8	G8
      0x5C240600,  //  0045  MOVE	R9	R3
      0x7C200200,  //  0046  CALL	R8	1
      0x60240013,  //  0047  GETGBL	R9	G19
      0x7C240000,  //  0048  CALL	R9	0
      0x9826259B,  //  0049  SETIDX	R9	K18	K155
      0x98081009,  //  004A  SETIDX	R2	R8	R9
      0x000C0738,  //  004B  ADD	R3	R3	K56
      0x8C200B86,  //  004C  GETMET	R8	R5	K134
      0x58280038,  //  004D  LDCONST	R10	K56
      0x7C200400,  //  004E  CALL	R8	2
      0x4C240000,  //  004F  LDNIL	R9
      0x20241009,  //  0050  NE	R9	R8	R9
      0x7826001C,  //  0051  JMPF	R9	#006F
      0x6024000C,  //  0052  GETGBL	R9	G12
      0x8C281139,  //  0053  GETMET	R10	R8	K57
      0x58300098,  //  0054  LDCONST	R12	K152
      0x58340010,  //  0055  LDCONST	R13	K16
      0x7C280600,  //  0056  CALL	R10	3
      0x7C240200,  //  0057  CALL	R9	1
      0x1C281338,  //  0058  EQ	R10	R9	K56
      0x782A0009,  //  0059  JMPF	R10	#0064
      0x60280008,  //  005A  GETGBL	R10	G8
      0x5C2C0600,  //  005B  MOVE	R11	R3
      0x7C280200,  //  005C  CALL	R10	1
      0x602C0013,  //  005D  GETGBL	R11	G19
      0x7C2C0000,  //  005E  CALL	R11	0
      0x982E2599,  //  005F  SETIDX	R11	K18	K153
      0x9808140B,  //  0060  SETIDX	R2	R10	R11
      0x000C0738,  //  0061  ADD	R3	R3	K56
      0x00100938,  //  0062  ADD	R4	R4	K56
      0x7002000A,  //  0063  JMP		#006F
      0x1C281321,  //  0064  EQ	R10	R9	K33
      0x782A0008,  //  0065  JMPF	R10	#006F
      0x60280008,  //  0066  GETGBL	R10	G8
      0x5C2C0600,  //  0067  MOVE	R11	R3
      0x7C280200,  //  0068  CALL	R10	1
      0x602C0013,  //  0069  GETGBL	R11	G19
      0x7C2C0000,  //  006A  CALL	R11	0
      0x982E259A,  //  006B  SETIDX	R11	K18	K154
      0x9808140B,  //  006C  SETIDX	R2	R10	R11
      0x000C0738,  //  006D  ADD	R3	R3	K56
      0x00100938,  //  006E  ADD	R4	R4	K56
      0x70020003,  //  006F  JMP		#0074
      0x54220003,  //  0070  LDINT	R8	4
      0x1C200E08,  //  0071  EQ	R8	R7	R8
      0x78220000,  //  0072  JMPF	R8	#0074
      0x7001FFFF,  //  0073  JMP		#0074
      0xB81E2C00,  //  0074  GETNGBL	R7	K22
      0x8C1C0F9C,  //  0075  GETMET	R7	R7	K156
      0x5824009D,  //  0076  LDCONST	R9	K157
      0x50280200,  //  0077  LDBOOL	R10	1	0
      0x7C1C0600,  //  0078  CALL	R7	3
      0x60200012,  //  0079  GETGBL	R8	G18
      0x7C200000,  //  007A  CALL	R8	0
      0xB8267400,  //  007B  GETNGBL	R9	K58
      0x60280008,  //  007C  GETGBL	R10	G8
      0x5C2C0E00,  //  007D  MOVE	R11	R7
      0x7C280200,  //  007E  CALL	R10	1
      0x002B3C0A,  //  007F  ADD	R10	K158	R10
      0x582C0053,  //  0080  LDCONST	R11	K83
      0x7C240400,  //  0081  CALL	R9	2
      0x4C240000,  //  0082  LDNIL	R9
      0x20240E09,  //  0083  NE	R9	R7	R9
      0x7826004D,  //  0084  JMPF	R9	#00D3
      0x8C240F6C,  //  0085  GETMET	R9	R7	K108
      0x582C009F,  //  0086  LDCONST	R11	K159
      0x7C240400,  //  0087  CALL	R9	2
      0x78260049,  //  0088  JMPF	R9	#00D3
      0x941C0F9F,  //  0089  GETIDX	R7	R7	K159
      0x5824000A,  //  008A  LDCONST	R9	K10
      0x50280200,  //  008B  LDBOOL	R10	1	0
      0x782A0045,  //  008C  JMPF	R10	#00D3
      0x60280008,  //  008D  GETGBL	R10	G8
      0x5C2C1200,  //  008E  MOVE	R11	R9
      0x7C280200,  //  008F  CALL	R10	1
      0x002B400A,  //  0090  ADD	R10	K160	R10
      0x8C2C0F6C,  //  0091  GETMET	R11	R7	K108
      0x5C341400,  //  0092  MOVE	R13	R10
      0x7C2C0400,  //  0093  CALL	R11	2
      0x742E0000,  //  0094  JMPT	R11	#0096
      0x7002003C,  //  0095  JMP		#00D3
      0x942C0E0A,  //  0096  GETIDX	R11	R7	R10
      0xB8327400,  //  0097  GETNGBL	R12	K58
      0x60340018,  //  0098  GETGBL	R13	G24
      0x583800A1,  //  0099  LDCONST	R14	K161
      0x5C3C1400,  //  009A  MOVE	R15	R10
      0x60400008,  //  009B  GETGBL	R16	G8
      0x5C441600,  //  009C  MOVE	R17	R11
      0x7C400200,  //  009D  CALL	R16	1
      0x7C340600,  //  009E  CALL	R13	3
      0x58380053,  //  009F  LDCONST	R14	K83
      0x7C300400,  //  00A0  CALL	R12	2
      0x8C301739,  //  00A1  GETMET	R12	R11	K57
      0x583800A2,  //  00A2  LDCONST	R14	K162
      0x543DFFFE,  //  00A3  LDINT	R15	-1
      0x7C300600,  //  00A4  CALL	R12	3
      0x8C341739,  //  00A5  GETMET	R13	R11	K57
      0x583C00A3,  //  00A6  LDCONST	R15	K163
      0x5441FFFE,  //  00A7  LDINT	R16	-1
      0x7C340600,  //  00A8  CALL	R13	3
      0x2438190A,  //  00A9  GT	R14	R12	K10
      0x783A0002,  //  00AA  JMPF	R14	#00AE
      0x8C381162,  //  00AB  GETMET	R14	R8	K98
      0x04401938,  //  00AC  SUB	R16	R12	K56
      0x7C380400,  //  00AD  CALL	R14	2
      0x24381B0A,  //  00AE  GT	R14	R13	K10
      0x783A0002,  //  00AF  JMPF	R14	#00B3
      0x8C381162,  //  00B0  GETMET	R14	R8	K98
      0x04401B38,  //  00B1  SUB	R16	R13	K56
      0x7C380400,  //  00B2  CALL	R14	2
      0xB83A7400,  //  00B3  GETNGBL	R14	K58
      0x603C0018,  //  00B4  GETGBL	R15	G24
      0x584000A4,  //  00B5  LDCONST	R16	K164
      0x5C441800,  //  00B6  MOVE	R17	R12
      0x5C481A00,  //  00B7  MOVE	R18	R13
      0x7C3C0600,  //  00B8  CALL	R15	3
      0x58400053,  //  00B9  LDCONST	R16	K83
      0x7C380400,  //  00BA  CALL	R14	2
      0x8C381739,  //  00BB  GETMET	R14	R11	K57
      0x584000A5,  //  00BC  LDCONST	R16	K165
      0x7C380400,  //  00BD  CALL	R14	2
      0x783A0002,  //  00BE  JMPF	R14	#00C2
      0x943C1D21,  //  00BF  GETIDX	R15	R14	K33
      0x243C1F0A,  //  00C0  GT	R15	R15	K10
      0x743E0000,  //  00C1  JMPT	R15	#00C3
      0x503C0001,  //  00C2  LDBOOL	R15	0	1
      0x503C0200,  //  00C3  LDBOOL	R15	1	0
      0x60400008,  //  00C4  GETGBL	R16	G8
      0x5C440600,  //  00C5  MOVE	R17	R3
      0x7C400200,  //  00C6  CALL	R16	1
      0x60440013,  //  00C7  GETGBL	R17	G19
      0x7C440000,  //  00C8  CALL	R17	0
      0x783E0001,  //  00C9  JMPF	R15	#00CC
      0x584800A6,  //  00CA  LDCONST	R18	K166
      0x70020000,  //  00CB  JMP		#00CD
      0x584800A7,  //  00CC  LDCONST	R18	K167
      0x98462412,  //  00CD  SETIDX	R17	K18	R18
      0x98474E09,  //  00CE  SETIDX	R17	K167	R9
      0x98082011,  //  00CF  SETIDX	R2	R16	R17
      0x000C0738,  //  00D0  ADD	R3	R3	K56
      0x00241338,  //  00D1  ADD	R9	R9	K56
      0x7001FFB7,  //  00D2  JMP		#008B
      0x6024000C,  //  00D3  GETGBL	R9	G12
      0xB82A2C00,  //  00D4  GETNGBL	R10	K22
      0x8C2815A8,  //  00D5  GETMET	R10	R10	K168
      0x7C280200,  //  00D6  CALL	R10	1
      0x7C240200,  //  00D7  CALL	R9	1
      0x5828000A,  //  00D8  LDCONST	R10	K10
      0x04241204,  //  00D9  SUB	R9	R9	R4
      0x142C1409,  //  00DA  LT	R11	R10	R9
      0x782E0011,  //  00DB  JMPF	R11	#00EE
      0x8C2C1139,  //  00DC  GETMET	R11	R8	K57
      0x5C341400,  //  00DD  MOVE	R13	R10
      0x7C2C0400,  //  00DE  CALL	R11	2
      0x4C300000,  //  00DF  LDNIL	R12
      0x1C2C160C,  //  00E0  EQ	R11	R11	R12
      0x782E0009,  //  00E1  JMPF	R11	#00EC
      0x602C0008,  //  00E2  GETGBL	R11	G8
      0x5C300600,  //  00E3  MOVE	R12	R3
      0x7C2C0200,  //  00E4  CALL	R11	1
      0x60300013,  //  00E5  GETGBL	R12	G19
      0x7C300000,  //  00E6  CALL	R12	0
      0x983225A9,  //  00E7  SETIDX	R12	K18	K169
      0x00341538,  //  00E8  ADD	R13	R10	K56
      0x9833520D,  //  00E9  SETIDX	R12	K169	R13
      0x9808160C,  //  00EA  SETIDX	R2	R11	R12
      0x000C0738,  //  00EB  ADD	R3	R3	K56
      0x00281538,  //  00EC  ADD	R10	R10	K56
      0x7001FFEB,  //  00ED  JMP		#00DA
      0x8C2C035E,  //  00EE  GETMET	R11	R1	K94
      0xB8362C00,  //  00EF  GETNGBL	R13	K22
      0x8C341B5B,  //  00F0  GETMET	R13	R13	K91
      0x7C340200,  //  00F1  CALL	R13	1
      0x7C2C0400,  //  00F2  CALL	R11	2
      0x8C3001AA,  //  00F3  GETMET	R12	R0	K170
      0x5C381600,  //  00F4  MOVE	R14	R11
      0x7C300400,  //  00F5  CALL	R12	2
      0x60340010,  //  00F6  GETGBL	R13	G16
      0x5C381800,  //  00F7  MOVE	R14	R12
      0x7C340200,  //  00F8  CALL	R13	1
      0xA8020007,  //  00F9  EXBLK	0	#0102
      0x5C381A00,  //  00FA  MOVE	R14	R13
      0x7C380000,  //  00FB  CALL	R14	0
      0x603C0008,  //  00FC  GETGBL	R15	G8
      0x5C400600,  //  00FD  MOVE	R16	R3
      0x7C3C0200,  //  00FE  CALL	R15	1
      0x98081E0E,  //  00FF  SETIDX	R2	R15	R14
      0x000C0738,  //  0100  ADD	R3	R3	K56
      0x7001FFF7,  //  0101  JMP		#00FA
      0x58340014,  //  0102  LDCONST	R13	K20
      0xAC340200,  //  0103  CATCH	R13	1	0
      0xB0080000,  //  0104  RAISE	2	R0	R0
      0x80040400,  //  0105  RET	1	R2
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
      0x880C017A,  //  0000  GETMBR	R3	R0	K122
      0x8C0C0739,  //  0001  GETMET	R3	R3	K57
      0x5C140200,  //  0002  MOVE	R5	R1
      0x7C0C0400,  //  0003  CALL	R3	2
      0x4C100000,  //  0004  LDNIL	R4
      0x1C100604,  //  0005  EQ	R4	R3	R4
      0x78120008,  //  0006  JMPF	R4	#0010
      0xB8127400,  //  0007  GETNGBL	R4	K58
      0x60140008,  //  0008  GETGBL	R5	G8
      0x5C180200,  //  0009  MOVE	R6	R1
      0x7C140200,  //  000A  CALL	R5	1
      0x0016F605,  //  000B  ADD	R5	K123	R5
      0x00140B7C,  //  000C  ADD	R5	R5	K124
      0x58180053,  //  000D  LDCONST	R6	K83
      0x7C100400,  //  000E  CALL	R4	2
      0x80000800,  //  000F  RET	0
      0x88100120,  //  0010  GETMBR	R4	R0	K32
      0x60140008,  //  0011  GETGBL	R5	G8
      0x5C180800,  //  0012  MOVE	R6	R4
      0x7C140200,  //  0013  CALL	R5	1
      0x5C180600,  //  0014  MOVE	R6	R3
      0x5C1C0000,  //  0015  MOVE	R7	R0
      0x5C200800,  //  0016  MOVE	R8	R4
      0x5C240400,  //  0017  MOVE	R9	R2
      0x7C180600,  //  0018  CALL	R6	3
      0x881C011E,  //  0019  GETMBR	R7	R0	K30
      0x8C1C0F62,  //  001A  GETMET	R7	R7	K98
      0x5C240C00,  //  001B  MOVE	R9	R6
      0x7C1C0400,  //  001C  CALL	R7	2
      0x601C0013,  //  001D  GETGBL	R7	G19
      0x7C1C0000,  //  001E  CALL	R7	0
      0x981E2401,  //  001F  SETIDX	R7	K18	R1
      0x60200010,  //  0020  GETGBL	R8	G16
      0x8C240566,  //  0021  GETMET	R9	R2	K102
      0x7C240200,  //  0022  CALL	R9	1
      0x7C200200,  //  0023  CALL	R8	1
      0xA8020004,  //  0024  EXBLK	0	#002A
      0x5C241000,  //  0025  MOVE	R9	R8
      0x7C240000,  //  0026  CALL	R9	0
      0x94280409,  //  0027  GETIDX	R10	R2	R9
      0x981C120A,  //  0028  SETIDX	R7	R9	R10
      0x7001FFFA,  //  0029  JMP		#0025
      0x58200014,  //  002A  LDCONST	R8	K20
      0xAC200200,  //  002B  CATCH	R8	1	0
      0xB0080000,  //  002C  RAISE	2	R0	R0
      0xB8227400,  //  002D  GETNGBL	R8	K58
      0x60240018,  //  002E  GETGBL	R9	G24
      0x582800AB,  //  002F  LDCONST	R10	K171
      0x5C2C0800,  //  0030  MOVE	R11	R4
      0x5C300200,  //  0031  MOVE	R12	R1
      0x8C34017D,  //  0032  GETMET	R13	R0	K125
      0x5C3C0400,  //  0033  MOVE	R15	R2
      0x7C340400,  //  0034  CALL	R13	2
      0x7C240800,  //  0035  CALL	R9	4
      0x58280021,  //  0036  LDCONST	R10	K33
      0x7C200400,  //  0037  CALL	R8	2
      0x88200156,  //  0038  GETMBR	R8	R0	K86
      0x98200A07,  //  0039  SETIDX	R8	R5	R7
      0x50200200,  //  003A  LDBOOL	R8	1	0
      0x90021608,  //  003B  SETMBR	R0	K11	R8
      0x88200120,  //  003C  GETMBR	R8	R0	K32
      0x00201138,  //  003D  ADD	R8	R8	K56
      0x90024008,  //  003E  SETMBR	R0	K32	R8
      0x8C20010C,  //  003F  GETMET	R8	R0	K12
      0x7C200200,  //  0040  CALL	R8	1
      0x8C2001AC,  //  0041  GETMET	R8	R0	K172
      0x7C200200,  //  0042  CALL	R8	1
      0x80040800,  //  0043  RET	1	R4
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
      0x1C140710,  //  0000  EQ	R5	R3	K16
      0x7815FFFF,  //  0001  JMPF	R5	#0002
      0x1C140710,  //  0002  EQ	R5	R3	K16
      0x7816000D,  //  0003  JMPF	R5	#0012
      0x60140010,  //  0004  GETGBL	R5	G16
      0x8818011E,  //  0005  GETMBR	R6	R0	K30
      0x7C140200,  //  0006  CALL	R5	1
      0xA8020005,  //  0007  EXBLK	0	#000E
      0x5C180A00,  //  0008  MOVE	R6	R5
      0x7C180000,  //  0009  CALL	R6	0
      0x8C1C01AD,  //  000A  GETMET	R7	R0	K173
      0x88240D48,  //  000B  GETMBR	R9	R6	K72
      0x7C1C0400,  //  000C  CALL	R7	2
      0x7001FFF9,  //  000D  JMP		#0008
      0x58140014,  //  000E  LDCONST	R5	K20
      0xAC140200,  //  000F  CATCH	R5	1	0
      0xB0080000,  //  0010  RAISE	2	R0	R0
      0x70020011,  //  0011  JMP		#0024
      0x60140004,  //  0012  GETGBL	R5	G4
      0x5C180800,  //  0013  MOVE	R6	R4
      0x7C140200,  //  0014  CALL	R5	1
      0x1C140BAE,  //  0015  EQ	R5	R5	K174
      0x78160003,  //  0016  JMPF	R5	#001B
      0x8C1401AD,  //  0017  GETMET	R5	R0	K173
      0x5C1C0800,  //  0018  MOVE	R7	R4
      0x7C140400,  //  0019  CALL	R5	2
      0x70020008,  //  001A  JMP		#0024
      0x8C1401AF,  //  001B  GETMET	R5	R0	K175
      0x5C1C0600,  //  001C  MOVE	R7	R3
      0x7C140400,  //  001D  CALL	R5	2
      0x4C180000,  //  001E  LDNIL	R6
      0x20180A06,  //  001F  NE	R6	R5	R6
      0x781A0002,  //  0020  JMPF	R6	#0024
      0x8C1801AD,  //  0021  GETMET	R6	R0	K173
      0x88200B48,  //  0022  GETMBR	R8	R5	K72
      0x7C180400,  //  0023  CALL	R6	2
      0xB8162C00,  //  0024  GETNGBL	R5	K22
      0x8C140BB0,  //  0025  GETMET	R5	R5	K176
      0x7C140200,  //  0026  CALL	R5	1
      0x80000000,  //  0027  RET	0
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
    ( &(const binstruction[134]) {  /* code */
      0xA4062A00,  //  0000  IMPORT	R1	K21
      0x50080000,  //  0001  LDBOOL	R2	0	0
      0xA8020054,  //  0002  EXBLK	0	#0058
      0x600C0011,  //  0003  GETGBL	R3	G17
      0x881001B1,  //  0004  GETMBR	R4	R0	K177
      0x7C0C0200,  //  0005  CALL	R3	1
      0x8C1007B2,  //  0006  GETMET	R4	R3	K178
      0x7C100200,  //  0007  CALL	R4	1
      0x8C14078A,  //  0008  GETMET	R5	R3	K138
      0x7C140200,  //  0009  CALL	R5	1
      0xA416AA00,  //  000A  IMPORT	R5	K85
      0x8C180B5E,  //  000B  GETMET	R6	R5	K94
      0x5C200800,  //  000C  MOVE	R8	R4
      0x7C180400,  //  000D  CALL	R6	2
      0x8C1C0D39,  //  000E  GETMET	R7	R6	K57
      0x582400B4,  //  000F  LDCONST	R9	K180
      0x882801B3,  //  0010  GETMBR	R10	R0	K179
      0x7C1C0600,  //  0011  CALL	R7	3
      0x90036607,  //  0012  SETMBR	R0	K179	R7
      0x8C1C0D39,  //  0013  GETMET	R7	R6	K57
      0x582400B6,  //  0014  LDCONST	R9	K182
      0x882801B5,  //  0015  GETMBR	R10	R0	K181
      0x7C1C0600,  //  0016  CALL	R7	3
      0x90036A07,  //  0017  SETMBR	R0	K181	R7
      0x601C0017,  //  0018  GETGBL	R7	G23
      0x8C200D39,  //  0019  GETMET	R8	R6	K57
      0x58280022,  //  001A  LDCONST	R10	K34
      0x502C0000,  //  001B  LDBOOL	R11	0	0
      0x7C200600,  //  001C  CALL	R8	3
      0x7C1C0200,  //  001D  CALL	R7	1
      0x90024407,  //  001E  SETMBR	R0	K34	R7
      0x601C0017,  //  001F  GETGBL	R7	G23
      0x8C200D39,  //  0020  GETMET	R8	R6	K57
      0x58280023,  //  0021  LDCONST	R10	K35
      0x502C0000,  //  0022  LDBOOL	R11	0	0
      0x7C200600,  //  0023  CALL	R8	3
      0x7C1C0200,  //  0024  CALL	R7	1
      0x90024607,  //  0025  SETMBR	R0	K35	R7
      0x8C1C0D39,  //  0026  GETMET	R7	R6	K57
      0x582400B7,  //  0027  LDCONST	R9	K183
      0x88280120,  //  0028  GETMBR	R10	R0	K32
      0x7C1C0600,  //  0029  CALL	R7	3
      0x90024007,  //  002A  SETMBR	R0	K32	R7
      0x8C1C0D39,  //  002B  GETMET	R7	R6	K57
      0x582400B8,  //  002C  LDCONST	R9	K184
      0x7C1C0400,  //  002D  CALL	R7	2
      0x9002AC07,  //  002E  SETMBR	R0	K86	R7
      0x601C0017,  //  002F  GETGBL	R7	G23
      0x8C200D39,  //  0030  GETMET	R8	R6	K57
      0x582800B9,  //  0031  LDCONST	R10	K185
      0x7C200400,  //  0032  CALL	R8	2
      0x7C1C0200,  //  0033  CALL	R7	1
      0x90037207,  //  0034  SETMBR	R0	K185	R7
      0x881C0156,  //  0035  GETMBR	R7	R0	K86
      0x4C200000,  //  0036  LDNIL	R8
      0x201C0E08,  //  0037  NE	R7	R7	R8
      0x781E000D,  //  0038  JMPF	R7	#0047
      0xB81E7400,  //  0039  GETNGBL	R7	K58
      0x60200018,  //  003A  GETGBL	R8	G24
      0x582400BA,  //  003B  LDCONST	R9	K186
      0x88280156,  //  003C  GETMBR	R10	R0	K86
      0x7C200400,  //  003D  CALL	R8	2
      0x58240053,  //  003E  LDCONST	R9	K83
      0x7C1C0400,  //  003F  CALL	R7	2
      0x8C1C0158,  //  0040  GETMET	R7	R0	K88
      0x7C1C0200,  //  0041  CALL	R7	1
      0x8C1C01BB,  //  0042  GETMET	R7	R0	K187
      0x7C1C0200,  //  0043  CALL	R7	1
      0x5C080E00,  //  0044  MOVE	R2	R7
      0x501C0200,  //  0045  LDBOOL	R7	1	0
      0x90021607,  //  0046  SETMBR	R0	K11	R7
      0x8C1C0D39,  //  0047  GETMET	R7	R6	K57
      0x582400BC,  //  0048  LDCONST	R9	K188
      0x60280013,  //  0049  GETGBL	R10	G19
      0x7C280000,  //  004A  CALL	R10	0
      0x7C1C0600,  //  004B  CALL	R7	3
      0x90023E07,  //  004C  SETMBR	R0	K31	R7
      0x881C011F,  //  004D  GETMBR	R7	R0	K31
      0x781E0006,  //  004E  JMPF	R7	#0056
      0xB81E7400,  //  004F  GETNGBL	R7	K58
      0x60200008,  //  0050  GETGBL	R8	G8
      0x8824011F,  //  0051  GETMBR	R9	R0	K31
      0x7C200200,  //  0052  CALL	R8	1
      0x00237A08,  //  0053  ADD	R8	K189	R8
      0x58240053,  //  0054  LDCONST	R9	K83
      0x7C1C0400,  //  0055  CALL	R7	2
      0xA8040001,  //  0056  EXBLK	1	1
      0x70020011,  //  0057  JMP		#006A
      0xAC0C0002,  //  0058  CATCH	R3	0	2
      0x7002000E,  //  0059  JMP		#0069
      0x201407BE,  //  005A  NE	R5	R3	K190
      0x7816000B,  //  005B  JMPF	R5	#0068
      0xB8167400,  //  005C  GETNGBL	R5	K58
      0x60180008,  //  005D  GETGBL	R6	G8
      0x5C1C0600,  //  005E  MOVE	R7	R3
      0x7C180200,  //  005F  CALL	R6	1
      0x001B7E06,  //  0060  ADD	R6	K191	R6
      0x00180D7F,  //  0061  ADD	R6	R6	K127
      0x601C0008,  //  0062  GETGBL	R7	G8
      0x5C200800,  //  0063  MOVE	R8	R4
      0x7C1C0200,  //  0064  CALL	R7	1
      0x00180C07,  //  0065  ADD	R6	R6	R7
      0x581C0021,  //  0066  LDCONST	R7	K33
      0x7C140400,  //  0067  CALL	R5	2
      0x70020000,  //  0068  JMP		#006A
      0xB0080000,  //  0069  RAISE	2	R0	R0
      0x880C01B3,  //  006A  GETMBR	R3	R0	K179
      0x4C100000,  //  006B  LDNIL	R4
      0x1C0C0604,  //  006C  EQ	R3	R3	R4
      0x780E000A,  //  006D  JMPF	R3	#0079
      0x8C0C03C0,  //  006E  GETMET	R3	R1	K192
      0x58140021,  //  006F  LDCONST	R5	K33
      0x7C0C0400,  //  0070  CALL	R3	2
      0x8C0C0786,  //  0071  GETMET	R3	R3	K134
      0x5814000A,  //  0072  LDCONST	R5	K10
      0x58180021,  //  0073  LDCONST	R6	K33
      0x7C0C0600,  //  0074  CALL	R3	3
      0x54120FFE,  //  0075  LDINT	R4	4095
      0x2C0C0604,  //  0076  AND	R3	R3	R4
      0x90036603,  //  0077  SETMBR	R0	K179	R3
      0x50080200,  //  0078  LDBOOL	R2	1	0
      0x880C01B5,  //  0079  GETMBR	R3	R0	K181
      0x4C100000,  //  007A  LDNIL	R4
      0x1C0C0604,  //  007B  EQ	R3	R3	R4
      0x780E0004,  //  007C  JMPF	R3	#0082
      0x880C0106,  //  007D  GETMBR	R3	R0	K6
      0x8C0C07C1,  //  007E  GETMET	R3	R3	K193
      0x7C0C0200,  //  007F  CALL	R3	1
      0x90036A03,  //  0080  SETMBR	R0	K181	R3
      0x50080200,  //  0081  LDBOOL	R2	1	0
      0x780A0001,  //  0082  JMPF	R2	#0085
      0x8C0C010C,  //  0083  GETMET	R3	R0	K12
      0x7C0C0200,  //  0084  CALL	R3	1
      0x80000000,  //  0085  RET	0
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
      0xB8162C00,  //  0003  GETNGBL	R5	K22
      0x8C140BC2,  //  0004  GETMET	R5	R5	K194
      0x581C00C3,  //  0005  LDCONST	R7	K195
      0x7C140400,  //  0006  CALL	R5	2
      0x80040A00,  //  0007  RET	1	R5
      0xB8162C00,  //  0008  GETNGBL	R5	K22
      0x8C140BC4,  //  0009  GETMET	R5	R5	K196
      0x5C1C0800,  //  000A  MOVE	R7	R4
      0x582000C5,  //  000B  LDCONST	R8	K197
      0x7C140600,  //  000C  CALL	R5	3
      0xB81A2C00,  //  000D  GETNGBL	R6	K22
      0x8C180DC4,  //  000E  GETMET	R6	R6	K196
      0x5C200800,  //  000F  MOVE	R8	R4
      0x582400C6,  //  0010  LDCONST	R9	K198
      0x7C180600,  //  0011  CALL	R6	3
      0x74160000,  //  0012  JMPT	R5	#0014
      0x781A0064,  //  0013  JMPF	R6	#0079
      0x4C1C0000,  //  0014  LDNIL	R7
      0x78160010,  //  0015  JMPF	R5	#0027
      0x60200009,  //  0016  GETGBL	R8	G9
      0x94240805,  //  0017  GETIDX	R9	R4	R5
      0x7C200200,  //  0018  CALL	R8	1
      0x1824110A,  //  0019  LE	R9	R8	K10
      0x78260004,  //  001A  JMPF	R9	#0020
      0xB8262C00,  //  001B  GETNGBL	R9	K22
      0x8C2413C2,  //  001C  GETMET	R9	R9	K194
      0x582C00C7,  //  001D  LDCONST	R11	K199
      0x7C240400,  //  001E  CALL	R9	2
      0x80041200,  //  001F  RET	1	R9
      0x8C24014B,  //  0020  GETMET	R9	R0	K75
      0x5C2C1000,  //  0021  MOVE	R11	R8
      0x7C240400,  //  0022  CALL	R9	2
      0x5C1C1200,  //  0023  MOVE	R7	R9
      0x8C240983,  //  0024  GETMET	R9	R4	K131
      0x5C2C0A00,  //  0025  MOVE	R11	R5
      0x7C240400,  //  0026  CALL	R9	2
      0x781A0009,  //  0027  JMPF	R6	#0032
      0x4C200000,  //  0028  LDNIL	R8
      0x1C200E08,  //  0029  EQ	R8	R7	R8
      0x78220003,  //  002A  JMPF	R8	#002F
      0x8C2001AF,  //  002B  GETMET	R8	R0	K175
      0x94280806,  //  002C  GETIDX	R10	R4	R6
      0x7C200400,  //  002D  CALL	R8	2
      0x5C1C1000,  //  002E  MOVE	R7	R8
      0x8C200983,  //  002F  GETMET	R8	R4	K131
      0x5C280C00,  //  0030  MOVE	R10	R6
      0x7C200400,  //  0031  CALL	R8	2
      0x4C200000,  //  0032  LDNIL	R8
      0x1C200E08,  //  0033  EQ	R8	R7	R8
      0x78220004,  //  0034  JMPF	R8	#003A
      0xB8222C00,  //  0035  GETNGBL	R8	K22
      0x8C2011C2,  //  0036  GETMET	R8	R8	K194
      0x582800C8,  //  0037  LDCONST	R10	K200
      0x7C200400,  //  0038  CALL	R8	2
      0x80041000,  //  0039  RET	1	R8
      0x88200FC9,  //  003A  GETMBR	R8	R7	K201
      0x74220004,  //  003B  JMPT	R8	#0041
      0xB8222C00,  //  003C  GETNGBL	R8	K22
      0x8C2011C2,  //  003D  GETMET	R8	R8	K194
      0x582800CA,  //  003E  LDCONST	R10	K202
      0x7C200400,  //  003F  CALL	R8	2
      0x80041000,  //  0040  RET	1	R8
      0x8C200FCB,  //  0041  GETMET	R8	R7	K203
      0x7C200200,  //  0042  CALL	R8	1
      0x60240013,  //  0043  GETGBL	R9	G19
      0x7C240000,  //  0044  CALL	R9	0
      0x60280010,  //  0045  GETGBL	R10	G16
      0x8C2C0966,  //  0046  GETMET	R11	R4	K102
      0x7C2C0200,  //  0047  CALL	R11	1
      0x7C280200,  //  0048  CALL	R10	1
      0xA8020016,  //  0049  EXBLK	0	#0061
      0x5C2C1400,  //  004A  MOVE	R11	R10
      0x7C2C0000,  //  004B  CALL	R11	0
      0xB8322C00,  //  004C  GETNGBL	R12	K22
      0x8C3019CC,  //  004D  GETMET	R12	R12	K204
      0x5C381000,  //  004E  MOVE	R14	R8
      0x5C3C1600,  //  004F  MOVE	R15	R11
      0x7C300600,  //  0050  CALL	R12	3
      0x4C340000,  //  0051  LDNIL	R13
      0x1C34180D,  //  0052  EQ	R13	R12	R13
      0x78360008,  //  0053  JMPF	R13	#005D
      0xB8362C00,  //  0054  GETNGBL	R13	K22
      0x8C341BC2,  //  0055  GETMET	R13	R13	K194
      0x603C0018,  //  0056  GETGBL	R15	G24
      0x584000CD,  //  0057  LDCONST	R16	K205
      0x5C441600,  //  0058  MOVE	R17	R11
      0x7C3C0400,  //  0059  CALL	R15	2
      0x7C340400,  //  005A  CALL	R13	2
      0xA8040001,  //  005B  EXBLK	1	1
      0x80001A00,  //  005C  RET	0
      0x9434100C,  //  005D  GETIDX	R13	R8	R12
      0x9438080B,  //  005E  GETIDX	R14	R4	R11
      0x98241A0E,  //  005F  SETIDX	R9	R13	R14
      0x7001FFE8,  //  0060  JMP		#004A
      0x58280014,  //  0061  LDCONST	R10	K20
      0xAC280200,  //  0062  CATCH	R10	1	0
      0xB0080000,  //  0063  RAISE	2	R0	R0
      0x8C280FCE,  //  0064  GETMET	R10	R7	K206
      0x5C301200,  //  0065  MOVE	R12	R9
      0x7C280400,  //  0066  CALL	R10	2
      0x8C280FCF,  //  0067  GETMET	R10	R7	K207
      0x7C280200,  //  0068  CALL	R10	1
      0x782A000A,  //  0069  JMPF	R10	#0075
      0x602C0018,  //  006A  GETGBL	R11	G24
      0x583000D0,  //  006B  LDCONST	R12	K208
      0x5C340200,  //  006C  MOVE	R13	R1
      0x5C381400,  //  006D  MOVE	R14	R10
      0x7C2C0600,  //  006E  CALL	R11	3
      0xB8322C00,  //  006F  GETNGBL	R12	K22
      0x8C3019D1,  //  0070  GETMET	R12	R12	K209
      0x5C381600,  //  0071  MOVE	R14	R11
      0x7C300400,  //  0072  CALL	R12	2
      0x80041800,  //  0073  RET	1	R12
      0x70020003,  //  0074  JMP		#0079
      0xB82E2C00,  //  0075  GETNGBL	R11	K22
      0x8C2C17B0,  //  0076  GETMET	R11	R11	K176
      0x7C2C0200,  //  0077  CALL	R11	1
      0x80041600,  //  0078  RET	1	R11
      0xB81E2C00,  //  0079  GETNGBL	R7	K22
      0x8C1C0FC2,  //  007A  GETMET	R7	R7	K194
      0x582400D2,  //  007B  LDCONST	R9	K210
      0x7C1C0400,  //  007C  CALL	R7	2
      0x80000000,  //  007D  RET	0
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
      0xB8062C00,  //  0000  GETNGBL	R1	K22
      0x8C0403D3,  //  0001  GETMET	R1	R1	K211
      0x580C00D4,  //  0002  LDCONST	R3	K212
      0x84100000,  //  0003  CLOSURE	R4	P0
      0x7C040600,  //  0004  CALL	R1	3
      0xB8062C00,  //  0005  GETNGBL	R1	K22
      0x8C0403D3,  //  0006  GETMET	R1	R1	K211
      0x580C00D5,  //  0007  LDCONST	R3	K213
      0x84100001,  //  0008  CLOSURE	R4	P1
      0x7C040600,  //  0009  CALL	R1	3
      0xB8062C00,  //  000A  GETNGBL	R1	K22
      0x8C0403D3,  //  000B  GETMET	R1	R1	K211
      0x580C00D6,  //  000C  LDCONST	R3	K214
      0x84100002,  //  000D  CLOSURE	R4	P2
      0x7C040600,  //  000E  CALL	R1	3
      0xA0000000,  //  000F  CLOSE	R0
      0x80000000,  //  0010  RET	0
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
      0xA406AA00,  //  0000  IMPORT	R1	K85
      0x8C0801D7,  //  0001  GETMET	R2	R0	K215
      0x7C080200,  //  0002  CALL	R2	1
      0x60080018,  //  0003  GETGBL	R2	G24
      0x580C00D8,  //  0004  LDCONST	R3	K216
      0x881001B3,  //  0005  GETMBR	R4	R0	K179
      0x881401B5,  //  0006  GETMBR	R5	R0	K181
      0x88180122,  //  0007  GETMBR	R6	R0	K34
      0x781A0001,  //  0008  JMPF	R6	#000B
      0x581800D9,  //  0009  LDCONST	R6	K217
      0x70020000,  //  000A  JMP		#000C
      0x581800DA,  //  000B  LDCONST	R6	K218
      0x881C0123,  //  000C  GETMBR	R7	R0	K35
      0x781E0001,  //  000D  JMPF	R7	#0010
      0x581C00D9,  //  000E  LDCONST	R7	K217
      0x70020000,  //  000F  JMP		#0011
      0x581C00DA,  //  0010  LDCONST	R7	K218
      0x88200120,  //  0011  GETMBR	R8	R0	K32
      0x7C080C00,  //  0012  CALL	R2	6
      0x880C01B9,  //  0013  GETMBR	R3	R0	K185
      0x780E0000,  //  0014  JMPF	R3	#0016
      0x000805DB,  //  0015  ADD	R2	R2	K219
      0x880C010B,  //  0016  GETMBR	R3	R0	K11
      0x780E000E,  //  0017  JMPF	R3	#0027
      0x000805DC,  //  0018  ADD	R2	R2	K220
      0x8C0C03DD,  //  0019  GETMET	R3	R1	K221
      0x88140156,  //  001A  GETMBR	R5	R0	K86
      0x7C0C0400,  //  001B  CALL	R3	2
      0x00080403,  //  001C  ADD	R2	R2	R3
      0x600C000C,  //  001D  GETGBL	R3	G12
      0x8810011F,  //  001E  GETMBR	R4	R0	K31
      0x7C0C0200,  //  001F  CALL	R3	1
      0x240C070A,  //  0020  GT	R3	R3	K10
      0x780E0004,  //  0021  JMPF	R3	#0027
      0x000805DE,  //  0022  ADD	R2	R2	K222
      0x8C0C03DD,  //  0023  GETMET	R3	R1	K221
      0x8814011F,  //  0024  GETMBR	R5	R0	K31
      0x7C0C0400,  //  0025  CALL	R3	2
      0x00080403,  //  0026  ADD	R2	R2	R3
      0x000805DF,  //  0027  ADD	R2	R2	K223
      0xA8020017,  //  0028  EXBLK	0	#0041
      0x600C0011,  //  0029  GETGBL	R3	G17
      0x881001B1,  //  002A  GETMBR	R4	R0	K177
      0x581400E0,  //  002B  LDCONST	R5	K224
      0x7C0C0400,  //  002C  CALL	R3	2
      0x8C1007E1,  //  002D  GETMET	R4	R3	K225
      0x5C180400,  //  002E  MOVE	R6	R2
      0x7C100400,  //  002F  CALL	R4	2
      0x8C10078A,  //  0030  GETMET	R4	R3	K138
      0x7C100200,  //  0031  CALL	R4	1
      0xB8127400,  //  0032  GETNGBL	R4	K58
      0x60140018,  //  0033  GETGBL	R5	G24
      0x581800E2,  //  0034  LDCONST	R6	K226
      0x881C010B,  //  0035  GETMBR	R7	R0	K11
      0x781E0001,  //  0036  JMPF	R7	#0039
      0x581C00E3,  //  0037  LDCONST	R7	K227
      0x70020000,  //  0038  JMP		#003A
      0x581C0010,  //  0039  LDCONST	R7	K16
      0x7C140400,  //  003A  CALL	R5	2
      0x58180021,  //  003B  LDCONST	R6	K33
      0x7C100400,  //  003C  CALL	R4	2
      0xA8040001,  //  003D  EXBLK	1	1
      0x80040400,  //  003E  RET	1	R2
      0xA8040001,  //  003F  EXBLK	1	1
      0x70020010,  //  0040  JMP		#0052
      0xAC0C0002,  //  0041  CATCH	R3	0	2
      0x7002000D,  //  0042  JMP		#0051
      0xB8167400,  //  0043  GETNGBL	R5	K58
      0x60180008,  //  0044  GETGBL	R6	G8
      0x5C1C0600,  //  0045  MOVE	R7	R3
      0x7C180200,  //  0046  CALL	R6	1
      0x001BC806,  //  0047  ADD	R6	K228	R6
      0x00180D7F,  //  0048  ADD	R6	R6	K127
      0x601C0008,  //  0049  GETGBL	R7	G8
      0x5C200800,  //  004A  MOVE	R8	R4
      0x7C1C0200,  //  004B  CALL	R7	1
      0x00180C07,  //  004C  ADD	R6	R6	R7
      0x581C0021,  //  004D  LDCONST	R7	K33
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
      0xA40AAA00,  //  0000  IMPORT	R2	K85
      0x600C0008,  //  0001  GETGBL	R3	G8
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C0C0200,  //  0003  CALL	R3	1
      0x4C100000,  //  0004  LDNIL	R4
      0x4C140000,  //  0005  LDNIL	R5
      0x88180156,  //  0006  GETMBR	R6	R0	K86
      0x8C180D6C,  //  0007  GETMET	R6	R6	K108
      0x5C200600,  //  0008  MOVE	R8	R3
      0x7C180400,  //  0009  CALL	R6	2
      0x741A0004,  //  000A  JMPT	R6	#0010
      0xB81A7400,  //  000B  GETNGBL	R6	K58
      0x001FCA03,  //  000C  ADD	R7	K229	R3
      0x58200053,  //  000D  LDCONST	R8	K83
      0x7C180400,  //  000E  CALL	R6	2
      0x80000C00,  //  000F  RET	0
      0xB81A7400,  //  0010  GETNGBL	R6	K58
      0x601C0018,  //  0011  GETGBL	R7	G24
      0x582000E6,  //  0012  LDCONST	R8	K230
      0x5C240200,  //  0013  MOVE	R9	R1
      0x7C1C0400,  //  0014  CALL	R7	2
      0x58200021,  //  0015  LDCONST	R8	K33
      0x7C180400,  //  0016  CALL	R6	2
      0x88180156,  //  0017  GETMBR	R6	R0	K86
      0x8C180D83,  //  0018  GETMET	R6	R6	K131
      0x5C200600,  //  0019  MOVE	R8	R3
      0x7C180400,  //  001A  CALL	R6	2
      0x50180200,  //  001B  LDBOOL	R6	1	0
      0x90021606,  //  001C  SETMBR	R0	K11	R6
      0x5818000A,  //  001D  LDCONST	R6	K10
      0x601C000C,  //  001E  GETGBL	R7	G12
      0x8820011E,  //  001F  GETMBR	R8	R0	K30
      0x7C1C0200,  //  0020  CALL	R7	1
      0x141C0C07,  //  0021  LT	R7	R6	R7
      0x781E000D,  //  0022  JMPF	R7	#0031
      0x881C011E,  //  0023  GETMBR	R7	R0	K30
      0x941C0E06,  //  0024  GETIDX	R7	R7	R6
      0x8C1C0F61,  //  0025  GETMET	R7	R7	K97
      0x7C1C0200,  //  0026  CALL	R7	1
      0x1C1C0207,  //  0027  EQ	R7	R1	R7
      0x781E0005,  //  0028  JMPF	R7	#002F
      0x881C011E,  //  0029  GETMBR	R7	R0	K30
      0x8C1C0F83,  //  002A  GETMET	R7	R7	K131
      0x5C240C00,  //  002B  MOVE	R9	R6
      0x7C1C0400,  //  002C  CALL	R7	2
      0x70020002,  //  002D  JMP		#0031
      0x70020000,  //  002E  JMP		#0030
      0x00180D38,  //  002F  ADD	R6	R6	K56
      0x7001FFEC,  //  0030  JMP		#001E
      0x8C1C01E7,  //  0031  GETMET	R7	R0	K231
      0x7C1C0200,  //  0032  CALL	R7	1
      0x8C1C010C,  //  0033  GETMET	R7	R0	K12
      0x7C1C0200,  //  0034  CALL	R7	1
      0x8C1C01AC,  //  0035  GETMET	R7	R0	K172
      0x7C1C0200,  //  0036  CALL	R7	1
      0x80000000,  //  0037  RET	0
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
      0x88100127,  //  0000  GETMBR	R4	R0	K39
      0x8C1009E8,  //  0001  GETMET	R4	R4	K232
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
      0x5810000A,  //  0000  LDCONST	R4	K10
      0x88140748,  //  0001  GETMBR	R5	R3	K72
      0x6018000C,  //  0002  GETGBL	R6	G12
      0x881C011E,  //  0003  GETMBR	R7	R0	K30
      0x7C180200,  //  0004  CALL	R6	1
      0x14180806,  //  0005  LT	R6	R4	R6
      0x781A000C,  //  0006  JMPF	R6	#0014
      0x8818011E,  //  0007  GETMBR	R6	R0	K30
      0x94180C04,  //  0008  GETIDX	R6	R6	R4
      0x881C0D48,  //  0009  GETMBR	R7	R6	K72
      0x1C1C0E05,  //  000A  EQ	R7	R7	R5
      0x781E0005,  //  000B  JMPF	R7	#0012
      0x8C1C0DE9,  //  000C  GETMET	R7	R6	K233
      0x5C240200,  //  000D  MOVE	R9	R1
      0x5C280400,  //  000E  MOVE	R10	R2
      0x5C2C0600,  //  000F  MOVE	R11	R3
      0x7C1C0800,  //  0010  CALL	R7	4
      0x80040E00,  //  0011  RET	1	R7
      0x00100938,  //  0012  ADD	R4	R4	K56
      0x7001FFED,  //  0013  JMP		#0002
      0xB81A3000,  //  0014  GETNGBL	R6	K24
      0x88180D4D,  //  0015  GETMBR	R6	R6	K77
      0x900E9806,  //  0016  SETMBR	R3	K76	R6
      0x80000000,  //  0017  RET	0
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
      0x88040167,  //  0000  GETMBR	R1	R0	K103
      0x4C080000,  //  0001  LDNIL	R2
      0x1C040202,  //  0002  EQ	R1	R1	R2
      0x78060000,  //  0003  JMPF	R1	#0005
      0x80000200,  //  0004  RET	0
      0x88040168,  //  0005  GETMBR	R1	R0	K104
      0x1C04030A,  //  0006  EQ	R1	R1	K10
      0x74060004,  //  0007  JMPT	R1	#000D
      0xB8062C00,  //  0008  GETNGBL	R1	K22
      0x8C0403EA,  //  0009  GETMET	R1	R1	K234
      0x880C0168,  //  000A  GETMBR	R3	R0	K104
      0x7C040400,  //  000B  CALL	R1	2
      0x78060006,  //  000C  JMPF	R1	#0014
      0x8C0401EB,  //  000D  GETMET	R1	R0	K235
      0x7C040200,  //  000E  CALL	R1	1
      0xB8062C00,  //  000F  GETNGBL	R1	K22
      0x8C0403EC,  //  0010  GETMET	R1	R1	K236
      0x880C0167,  //  0011  GETMBR	R3	R0	K103
      0x7C040400,  //  0012  CALL	R1	2
      0x9002D001,  //  0013  SETMBR	R0	K104	R1
      0x80000000,  //  0014  RET	0
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
    ( &(const binstruction[14]) {  /* code */
      0x60140009,  //  0000  GETGBL	R5	G9
      0x5C180600,  //  0001  MOVE	R6	R3
      0x7C140200,  //  0002  CALL	R5	1
      0x78160003,  //  0003  JMPF	R5	#0008
      0x88180106,  //  0004  GETMBR	R6	R0	K6
      0x8C180DED,  //  0005  GETMET	R6	R6	K237
      0x7C180200,  //  0006  CALL	R6	1
      0x70020001,  //  0007  JMP		#000A
      0x8C18010D,  //  0008  GETMET	R6	R0	K13
      0x7C180200,  //  0009  CALL	R6	1
      0xB81A2C00,  //  000A  GETNGBL	R6	K22
      0x8C180DB0,  //  000B  GETMET	R6	R6	K176
      0x7C180200,  //  000C  CALL	R6	1
      0x80000000,  //  000D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: autoconf_sensors_list
********************************************************************/
be_local_closure(class_Matter_Device_autoconf_sensors_list,   /* name */
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
    be_str_weak(autoconf_sensors_list),
    &be_const_str_solidified,
    ( &(const binstruction[119]) {  /* code */
      0x60080012,  //  0000  GETGBL	R2	G18
      0x7C080000,  //  0001  CALL	R2	0
      0x600C0010,  //  0002  GETGBL	R3	G16
      0x8C100111,  //  0003  GETMET	R4	R0	K17
      0x5C180200,  //  0004  MOVE	R6	R1
      0x7C100400,  //  0005  CALL	R4	2
      0x7C0C0200,  //  0006  CALL	R3	1
      0xA8020013,  //  0007  EXBLK	0	#001C
      0x5C100600,  //  0008  MOVE	R4	R3
      0x7C100000,  //  0009  CALL	R4	0
      0x94140204,  //  000A  GETIDX	R5	R1	R4
      0x6018000F,  //  000B  GETGBL	R6	G15
      0x5C1C0A00,  //  000C  MOVE	R7	R5
      0x60200013,  //  000D  GETGBL	R8	G19
      0x7C180400,  //  000E  CALL	R6	2
      0x781A000A,  //  000F  JMPF	R6	#001B
      0x8C180B6C,  //  0010  GETMET	R6	R5	K108
      0x582000EE,  //  0011  LDCONST	R8	K238
      0x7C180400,  //  0012  CALL	R6	2
      0x781A0006,  //  0013  JMPF	R6	#001B
      0x001809EF,  //  0014  ADD	R6	R4	K239
      0x8C1C0562,  //  0015  GETMET	R7	R2	K98
      0x60240013,  //  0016  GETGBL	R9	G19
      0x7C240000,  //  0017  CALL	R9	0
      0x982625F0,  //  0018  SETIDX	R9	K18	K240
      0x9827E206,  //  0019  SETIDX	R9	K241	R6
      0x7C1C0400,  //  001A  CALL	R7	2
      0x7001FFEB,  //  001B  JMP		#0008
      0x580C0014,  //  001C  LDCONST	R3	K20
      0xAC0C0200,  //  001D  CATCH	R3	1	0
      0xB0080000,  //  001E  RAISE	2	R0	R0
      0x600C0010,  //  001F  GETGBL	R3	G16
      0x8C100111,  //  0020  GETMET	R4	R0	K17
      0x5C180200,  //  0021  MOVE	R6	R1
      0x7C100400,  //  0022  CALL	R4	2
      0x7C0C0200,  //  0023  CALL	R3	1
      0xA8020013,  //  0024  EXBLK	0	#0039
      0x5C100600,  //  0025  MOVE	R4	R3
      0x7C100000,  //  0026  CALL	R4	0
      0x94140204,  //  0027  GETIDX	R5	R1	R4
      0x6018000F,  //  0028  GETGBL	R6	G15
      0x5C1C0A00,  //  0029  MOVE	R7	R5
      0x60200013,  //  002A  GETGBL	R8	G19
      0x7C180400,  //  002B  CALL	R6	2
      0x781A000A,  //  002C  JMPF	R6	#0038
      0x8C180B6C,  //  002D  GETMET	R6	R5	K108
      0x582000F2,  //  002E  LDCONST	R8	K242
      0x7C180400,  //  002F  CALL	R6	2
      0x781A0006,  //  0030  JMPF	R6	#0038
      0x001809F3,  //  0031  ADD	R6	R4	K243
      0x8C1C0562,  //  0032  GETMET	R7	R2	K98
      0x60240013,  //  0033  GETGBL	R9	G19
      0x7C240000,  //  0034  CALL	R9	0
      0x982625F4,  //  0035  SETIDX	R9	K18	K244
      0x9827E206,  //  0036  SETIDX	R9	K241	R6
      0x7C1C0400,  //  0037  CALL	R7	2
      0x7001FFEB,  //  0038  JMP		#0025
      0x580C0014,  //  0039  LDCONST	R3	K20
      0xAC0C0200,  //  003A  CATCH	R3	1	0
      0xB0080000,  //  003B  RAISE	2	R0	R0
      0x600C0010,  //  003C  GETGBL	R3	G16
      0x8C100111,  //  003D  GETMET	R4	R0	K17
      0x5C180200,  //  003E  MOVE	R6	R1
      0x7C100400,  //  003F  CALL	R4	2
      0x7C0C0200,  //  0040  CALL	R3	1
      0xA8020013,  //  0041  EXBLK	0	#0056
      0x5C100600,  //  0042  MOVE	R4	R3
      0x7C100000,  //  0043  CALL	R4	0
      0x94140204,  //  0044  GETIDX	R5	R1	R4
      0x6018000F,  //  0045  GETGBL	R6	G15
      0x5C1C0A00,  //  0046  MOVE	R7	R5
      0x60200013,  //  0047  GETGBL	R8	G19
      0x7C180400,  //  0048  CALL	R6	2
      0x781A000A,  //  0049  JMPF	R6	#0055
      0x8C180B6C,  //  004A  GETMET	R6	R5	K108
      0x582000F5,  //  004B  LDCONST	R8	K245
      0x7C180400,  //  004C  CALL	R6	2
      0x781A0006,  //  004D  JMPF	R6	#0055
      0x001809F6,  //  004E  ADD	R6	R4	K246
      0x8C1C0562,  //  004F  GETMET	R7	R2	K98
      0x60240013,  //  0050  GETGBL	R9	G19
      0x7C240000,  //  0051  CALL	R9	0
      0x982625F7,  //  0052  SETIDX	R9	K18	K247
      0x9827E206,  //  0053  SETIDX	R9	K241	R6
      0x7C1C0400,  //  0054  CALL	R7	2
      0x7001FFEB,  //  0055  JMP		#0042
      0x580C0014,  //  0056  LDCONST	R3	K20
      0xAC0C0200,  //  0057  CATCH	R3	1	0
      0xB0080000,  //  0058  RAISE	2	R0	R0
      0x600C0010,  //  0059  GETGBL	R3	G16
      0x8C100111,  //  005A  GETMET	R4	R0	K17
      0x5C180200,  //  005B  MOVE	R6	R1
      0x7C100400,  //  005C  CALL	R4	2
      0x7C0C0200,  //  005D  CALL	R3	1
      0xA8020013,  //  005E  EXBLK	0	#0073
      0x5C100600,  //  005F  MOVE	R4	R3
      0x7C100000,  //  0060  CALL	R4	0
      0x94140204,  //  0061  GETIDX	R5	R1	R4
      0x6018000F,  //  0062  GETGBL	R6	G15
      0x5C1C0A00,  //  0063  MOVE	R7	R5
      0x60200013,  //  0064  GETGBL	R8	G19
      0x7C180400,  //  0065  CALL	R6	2
      0x781A000A,  //  0066  JMPF	R6	#0072
      0x8C180B6C,  //  0067  GETMET	R6	R5	K108
      0x582000F8,  //  0068  LDCONST	R8	K248
      0x7C180400,  //  0069  CALL	R6	2
      0x781A0006,  //  006A  JMPF	R6	#0072
      0x001809F9,  //  006B  ADD	R6	R4	K249
      0x8C1C0562,  //  006C  GETMET	R7	R2	K98
      0x60240013,  //  006D  GETGBL	R9	G19
      0x7C240000,  //  006E  CALL	R9	0
      0x982625FA,  //  006F  SETIDX	R9	K18	K250
      0x9827E206,  //  0070  SETIDX	R9	K241	R6
      0x7C1C0400,  //  0071  CALL	R7	2
      0x7001FFEB,  //  0072  JMP		#005F
      0x580C0014,  //  0073  LDCONST	R3	K20
      0xAC0C0200,  //  0074  CATCH	R3	1	0
      0xB0080000,  //  0075  RAISE	2	R0	R0
      0x80040400,  //  0076  RET	1	R2
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
      0x8C08014B,  //  0000  GETMET	R2	R0	K75
      0x5C100200,  //  0001  MOVE	R4	R1
      0x7C080400,  //  0002  CALL	R2	2
      0x4C0C0000,  //  0003  LDNIL	R3
      0x1C0C0403,  //  0004  EQ	R3	R2	R3
      0x780E0000,  //  0005  JMPF	R3	#0007
      0x80000600,  //  0006  RET	0
      0x8C0C05CF,  //  0007  GETMET	R3	R2	K207
      0x7C0C0200,  //  0008  CALL	R3	1
      0x780E0008,  //  0009  JMPF	R3	#0013
      0x60100018,  //  000A  GETGBL	R4	G24
      0x581400FB,  //  000B  LDCONST	R5	K251
      0x5C180600,  //  000C  MOVE	R6	R3
      0x7C100400,  //  000D  CALL	R4	2
      0xB8162C00,  //  000E  GETNGBL	R5	K22
      0x8C140B80,  //  000F  GETMET	R5	R5	K128
      0x5C1C0800,  //  0010  MOVE	R7	R4
      0x58200010,  //  0011  LDCONST	R8	K16
      0x7C140600,  //  0012  CALL	R5	3
      0x80000000,  //  0013  RET	0
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
      0x5814000A,  //  0000  LDCONST	R5	K10
      0xA41B0A00,  //  0001  IMPORT	R6	K133
      0x601C000C,  //  0002  GETGBL	R7	G12
      0x8820011E,  //  0003  GETMBR	R8	R0	K30
      0x7C1C0200,  //  0004  CALL	R7	1
      0x141C0A07,  //  0005  LT	R7	R5	R7
      0x781E0010,  //  0006  JMPF	R7	#0018
      0x881C011E,  //  0007  GETMBR	R7	R0	K30
      0x941C0E05,  //  0008  GETIDX	R7	R7	R5
      0x8C200D6C,  //  0009  GETMET	R8	R6	K108
      0x5C280E00,  //  000A  MOVE	R10	R7
      0x582C006A,  //  000B  LDCONST	R11	K106
      0x7C200600,  //  000C  CALL	R8	3
      0x78220007,  //  000D  JMPF	R8	#0016
      0x8820011E,  //  000E  GETMBR	R8	R0	K30
      0x94201005,  //  000F  GETIDX	R8	R8	R5
      0x8C20116A,  //  0010  GETMET	R8	R8	K106
      0x5C280200,  //  0011  MOVE	R10	R1
      0x5C2C0400,  //  0012  MOVE	R11	R2
      0x5C300600,  //  0013  MOVE	R12	R3
      0x5C340800,  //  0014  MOVE	R13	R4
      0x7C200A00,  //  0015  CALL	R8	5
      0x00140B38,  //  0016  ADD	R5	R5	K56
      0x7001FFE9,  //  0017  JMP		#0002
      0x80000000,  //  0018  RET	0
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
      0x8804011D,  //  0000  GETMBR	R1	R0	K29
      0x00040338,  //  0001  ADD	R1	R1	K56
      0x90023A01,  //  0002  SETMBR	R0	K29	R1
      0x88040127,  //  0003  GETMBR	R1	R0	K39
      0x8C0403FC,  //  0004  GETMET	R1	R1	K252
      0x7C040200,  //  0005  CALL	R1	1
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_plugin_class_arg
********************************************************************/
be_local_closure(class_Matter_Device_get_plugin_class_arg,   /* name */
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
    be_str_weak(get_plugin_class_arg),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x8808017A,  //  0000  GETMBR	R2	R0	K122
      0x8C080539,  //  0001  GETMET	R2	R2	K57
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x780A0001,  //  0004  JMPF	R2	#0007
      0x880C05FD,  //  0005  GETMBR	R3	R2	K253
      0x70020000,  //  0006  JMP		#0008
      0x580C0010,  //  0007  LDCONST	R3	K16
      0x80040600,  //  0008  RET	1	R3
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
      0x8808016B,  //  0002  GETMBR	R2	R0	K107
      0x4C0C0000,  //  0003  LDNIL	R3
      0x20080403,  //  0004  NE	R2	R2	R3
      0x780A0018,  //  0005  JMPF	R2	#001F
      0x60080010,  //  0006  GETGBL	R2	G16
      0x880C016B,  //  0007  GETMBR	R3	R0	K107
      0x8C0C0766,  //  0008  GETMET	R3	R3	K102
      0x7C0C0200,  //  0009  CALL	R3	1
      0x7C080200,  //  000A  CALL	R2	1
      0xA802000F,  //  000B  EXBLK	0	#001C
      0x5C0C0400,  //  000C  MOVE	R3	R2
      0x7C0C0000,  //  000D  CALL	R3	0
      0x8810016B,  //  000E  GETMBR	R4	R0	K107
      0x94100803,  //  000F  GETIDX	R4	R4	R3
      0x8C1009FE,  //  0010  GETMET	R4	R4	K254
      0x7C100200,  //  0011  CALL	R4	1
      0x4C140000,  //  0012  LDNIL	R5
      0x20140805,  //  0013  NE	R5	R4	R5
      0x78160005,  //  0014  JMPF	R5	#001B
      0x6014000C,  //  0015  GETGBL	R5	G12
      0x5C180800,  //  0016  MOVE	R6	R4
      0x7C140200,  //  0017  CALL	R5	1
      0x24140B0A,  //  0018  GT	R5	R5	K10
      0x78160000,  //  0019  JMPF	R5	#001B
      0x98040604,  //  001A  SETIDX	R1	R3	R4
      0x7001FFEF,  //  001B  JMP		#000C
      0x58080014,  //  001C  LDCONST	R2	K20
      0xAC080200,  //  001D  CATCH	R2	1	0
      0xB0080000,  //  001E  RAISE	2	R0	R0
      0x90023E01,  //  001F  SETMBR	R0	K31	R1
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
      0x5804000F,  //  0000  LDCONST	R1	K15
      0x60080012,  //  0001  GETGBL	R2	G18
      0x7C080000,  //  0002  CALL	R2	0
      0x4C0C0000,  //  0003  LDNIL	R3
      0x1C0C0003,  //  0004  EQ	R3	R0	R3
      0x780E0000,  //  0005  JMPF	R3	#0007
      0x80040400,  //  0006  RET	1	R2
      0x600C0010,  //  0007  GETGBL	R3	G16
      0x8C100166,  //  0008  GETMET	R4	R0	K102
      0x7C100200,  //  0009  CALL	R4	1
      0x7C0C0200,  //  000A  CALL	R3	1
      0xA8020005,  //  000B  EXBLK	0	#0012
      0x5C100600,  //  000C  MOVE	R4	R3
      0x7C100000,  //  000D  CALL	R4	0
      0x8C140562,  //  000E  GETMET	R5	R2	K98
      0x5C1C0800,  //  000F  MOVE	R7	R4
      0x7C140400,  //  0010  CALL	R5	2
      0x7001FFF9,  //  0011  JMP		#000C
      0x580C0014,  //  0012  LDCONST	R3	K20
      0xAC0C0200,  //  0013  CATCH	R3	1	0
      0xB0080000,  //  0014  RAISE	2	R0	R0
      0x600C0010,  //  0015  GETGBL	R3	G16
      0x6010000C,  //  0016  GETGBL	R4	G12
      0x5C140400,  //  0017  MOVE	R5	R2
      0x7C100200,  //  0018  CALL	R4	1
      0x04100938,  //  0019  SUB	R4	R4	K56
      0x40127004,  //  001A  CONNECT	R4	K56	R4
      0x7C0C0200,  //  001B  CALL	R3	1
      0xA8020010,  //  001C  EXBLK	0	#002E
      0x5C100600,  //  001D  MOVE	R4	R3
      0x7C100000,  //  001E  CALL	R4	0
      0x94140404,  //  001F  GETIDX	R5	R2	R4
      0x5C180800,  //  0020  MOVE	R6	R4
      0x241C0D0A,  //  0021  GT	R7	R6	K10
      0x781E0008,  //  0022  JMPF	R7	#002C
      0x041C0D38,  //  0023  SUB	R7	R6	K56
      0x941C0407,  //  0024  GETIDX	R7	R2	R7
      0x241C0E05,  //  0025  GT	R7	R7	R5
      0x781E0004,  //  0026  JMPF	R7	#002C
      0x041C0D38,  //  0027  SUB	R7	R6	K56
      0x941C0407,  //  0028  GETIDX	R7	R2	R7
      0x98080C07,  //  0029  SETIDX	R2	R6	R7
      0x04180D38,  //  002A  SUB	R6	R6	K56
      0x7001FFF4,  //  002B  JMP		#0021
      0x98080C05,  //  002C  SETIDX	R2	R6	R5
      0x7001FFEE,  //  002D  JMP		#001D
      0x580C0014,  //  002E  LDCONST	R3	K20
      0xAC0C0200,  //  002F  CATCH	R3	1	0
      0xB0080000,  //  0030  RAISE	2	R0	R0
      0x80040400,  //  0031  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Device
********************************************************************/
be_local_class(Matter_Device,
    22,
    NULL,
    be_nested_map(79,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(received_ack, -1), be_const_closure(class_Matter_Device_received_ack_closure) },
        { be_const_key_weak(_start_udp, -1), be_const_closure(class_Matter_Device__start_udp_closure) },
        { be_const_key_weak(plugins, -1), be_const_var(1) },
        { be_const_key_weak(msg_send, -1), be_const_closure(class_Matter_Device_msg_send_closure) },
        { be_const_key_weak(_instantiate_plugins_from_config, -1), be_const_closure(class_Matter_Device__instantiate_plugins_from_config_closure) },
        { be_const_key_weak(message_handler, -1), be_const_var(7) },
        { be_const_key_weak(event_fabrics_saved, -1), be_const_closure(class_Matter_Device_event_fabrics_saved_closure) },
        { be_const_key_weak(save_before_restart, -1), be_const_closure(class_Matter_Device_save_before_restart_closure) },
        { be_const_key_weak(probe_sensor_time, 1), be_const_var(20) },
        { be_const_key_weak(conf_to_log, 50), be_const_static_closure(class_Matter_Device_conf_to_log_closure) },
        { be_const_key_weak(init, -1), be_const_closure(class_Matter_Device_init_closure) },
        { be_const_key_weak(ui, -1), be_const_var(10) },
        { be_const_key_weak(root_passcode, -1), be_const_var(15) },
        { be_const_key_weak(every_50ms, 22), be_const_closure(class_Matter_Device_every_50ms_closure) },
        { be_const_key_weak(profiler, -1), be_const_var(6) },
        { be_const_key_weak(disable_bridge_mode, -1), be_const_var(17) },
        { be_const_key_weak(remove_fabric, -1), be_const_closure(class_Matter_Device_remove_fabric_closure) },
        { be_const_key_weak(resolve_attribute_read_solo, 66), be_const_closure(class_Matter_Device_resolve_attribute_read_solo_closure) },
        { be_const_key_weak(signal_endpoints_changed, -1), be_const_closure(class_Matter_Device_signal_endpoints_changed_closure) },
        { be_const_key_weak(autoconf_device, 69), be_const_closure(class_Matter_Device_autoconf_device_closure) },
        { be_const_key_weak(_trigger_read_sensors, -1), be_const_closure(class_Matter_Device__trigger_read_sensors_closure) },
        { be_const_key_weak(get_active_endpoints, -1), be_const_closure(class_Matter_Device_get_active_endpoints_closure) },
        { be_const_key_weak(button_handler, 37), be_const_closure(class_Matter_Device_button_handler_closure) },
        { be_const_key_weak(debug, 3), be_const_var(19) },
        { be_const_key_weak(stop, 43), be_const_closure(class_Matter_Device_stop_closure) },
        { be_const_key_weak(adjust_next_ep, -1), be_const_closure(class_Matter_Device_adjust_next_ep_closure) },
        { be_const_key_weak(add_read_sensors_schedule, 49), be_const_closure(class_Matter_Device_add_read_sensors_schedule_closure) },
        { be_const_key_weak(button_pressed, -1), be_const_closure(class_Matter_Device_button_pressed_closure) },
        { be_const_key_weak(register_http_remote, 53), be_const_closure(class_Matter_Device_register_http_remote_closure) },
        { be_const_key_weak(root_discriminator, -1), be_const_var(14) },
        { be_const_key_weak(start, 4), be_const_closure(class_Matter_Device_start_closure) },
        { be_const_key_weak(MtrInfo_one, -1), be_const_closure(class_Matter_Device_MtrInfo_one_closure) },
        { be_const_key_weak(autoconf_sensors_list, 40), be_const_closure(class_Matter_Device_autoconf_sensors_list_closure) },
        { be_const_key_weak(get_plugin_class_displayname, -1), be_const_closure(class_Matter_Device_get_plugin_class_displayname_closure) },
        { be_const_key_weak(plugins_config, -1), be_const_var(3) },
        { be_const_key_weak(http_remotes, 11), be_const_var(13) },
        { be_const_key_weak(get_plugin_remote_info, 58), be_const_closure(class_Matter_Device_get_plugin_remote_info_closure) },
        { be_const_key_weak(UDP_PORT, -1), be_const_int(5540) },
        { be_const_key_weak(tick, 46), be_const_var(11) },
        { be_const_key_weak(clean_remotes, 72), be_const_closure(class_Matter_Device_clean_remotes_closure) },
        { be_const_key_weak(plugins_classes, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(53,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(occupancy, -1), be_const_class(be_class_Matter_Plugin_Sensor_Occupancy) },
        { be_const_key_weak(pressure, -1), be_const_class(be_class_Matter_Plugin_Sensor_Pressure) },
        { be_const_key_weak(http_light2, -1), be_const_class(be_class_Matter_Plugin_Bridge_Light2) },
        { be_const_key_weak(v_contact, 36), be_const_class(be_class_Matter_Plugin_Virt_Sensor_Contact) },
        { be_const_key_weak(v_occupancy, -1), be_const_class(be_class_Matter_Plugin_Virt_Sensor_Occupancy) },
        { be_const_key_weak(flow, 8), be_const_class(be_class_Matter_Plugin_Sensor_Flow) },
        { be_const_key_weak(http_contact, -1), be_const_class(be_class_Matter_Plugin_Bridge_Sensor_Contact) },
        { be_const_key_weak(shutter, -1), be_const_class(be_class_Matter_Plugin_Shutter) },
        { be_const_key_weak(v_pressure, -1), be_const_class(be_class_Matter_Plugin_Virt_Sensor_Pressure) },
        { be_const_key_weak(http_rain, -1), be_const_class(be_class_Matter_Plugin_Bridge_Sensor_Rain) },
        { be_const_key_weak(http_pressure, 12), be_const_class(be_class_Matter_Plugin_Bridge_Sensor_Pressure) },
        { be_const_key_weak(v_light2, -1), be_const_class(be_class_Matter_Plugin_Virt_Light2) },
        { be_const_key_weak(v_flow, -1), be_const_class(be_class_Matter_Plugin_Virt_Sensor_Flow) },
        { be_const_key_weak(relay, -1), be_const_class(be_class_Matter_Plugin_OnOff) },
        { be_const_key_weak(v_illuminance, 29), be_const_class(be_class_Matter_Plugin_Virt_Sensor_Illuminance) },
        { be_const_key_weak(v_airquality, -1), be_const_class(be_class_Matter_Plugin_Virt_Sensor_Air_Quality) },
        { be_const_key_weak(temperature, 20), be_const_class(be_class_Matter_Plugin_Sensor_Temp) },
        { be_const_key_weak(waterleak, -1), be_const_class(be_class_Matter_Plugin_Sensor_Waterleak) },
        { be_const_key_weak(v_fan, -1), be_const_class(be_class_Matter_Plugin_Virt_Fan) },
        { be_const_key_weak(http_occupancy, 6), be_const_class(be_class_Matter_Plugin_Bridge_Sensor_Occupancy) },
        { be_const_key_weak(http_relay, -1), be_const_class(be_class_Matter_Plugin_Bridge_OnOff) },
        { be_const_key_weak(fan, -1), be_const_class(be_class_Matter_Plugin_Fan) },
        { be_const_key_weak(light1, -1), be_const_class(be_class_Matter_Plugin_Light1) },
        { be_const_key_weak(root, -1), be_const_class(be_class_Matter_Plugin_Root) },
        { be_const_key_weak(illuminance, -1), be_const_class(be_class_Matter_Plugin_Sensor_Illuminance) },
        { be_const_key_weak(v_temp, 15), be_const_class(be_class_Matter_Plugin_Virt_Sensor_Temp) },
        { be_const_key_weak(v_light1, -1), be_const_class(be_class_Matter_Plugin_Virt_Light1) },
        { be_const_key_weak(v_waterleak, -1), be_const_class(be_class_Matter_Plugin_Virt_Sensor_Waterleak) },
        { be_const_key_weak(v_relay, -1), be_const_class(be_class_Matter_Plugin_Virt_OnOff) },
        { be_const_key_weak(contact, 52), be_const_class(be_class_Matter_Plugin_Sensor_Contact) },
        { be_const_key_weak(light2, 27), be_const_class(be_class_Matter_Plugin_Light2) },
        { be_const_key_weak(http_light1, -1), be_const_class(be_class_Matter_Plugin_Bridge_Light1) },
        { be_const_key_weak(gensw_btn, -1), be_const_class(be_class_Matter_Plugin_Sensor_GenericSwitch_Btn) },
        { be_const_key_weak(onoff, -1), be_const_class(be_class_Matter_Plugin_Sensor_OnOff) },
        { be_const_key_weak(v_light0, -1), be_const_class(be_class_Matter_Plugin_Virt_Light0) },
        { be_const_key_weak(light3, -1), be_const_class(be_class_Matter_Plugin_Light3) },
        { be_const_key_weak(http_humidity, -1), be_const_class(be_class_Matter_Plugin_Bridge_Sensor_Humidity) },
        { be_const_key_weak(http_illuminance, -1), be_const_class(be_class_Matter_Plugin_Bridge_Sensor_Illuminance) },
        { be_const_key_weak(v_rain, 43), be_const_class(be_class_Matter_Plugin_Virt_Sensor_Rain) },
        { be_const_key_weak(http_light0, -1), be_const_class(be_class_Matter_Plugin_Bridge_Light0) },
        { be_const_key_weak(http_waterleak, 45), be_const_class(be_class_Matter_Plugin_Bridge_Sensor_Waterleak) },
        { be_const_key_weak(shutter_X2Btilt, -1), be_const_class(be_class_Matter_Plugin_ShutterTilt) },
        { be_const_key_weak(v_light3, -1), be_const_class(be_class_Matter_Plugin_Virt_Light3) },
        { be_const_key_weak(airquality, 35), be_const_class(be_class_Matter_Plugin_Sensor_Air_Quality) },
        { be_const_key_weak(http_flow, -1), be_const_class(be_class_Matter_Plugin_Bridge_Sensor_Flow) },
        { be_const_key_weak(humidity, -1), be_const_class(be_class_Matter_Plugin_Sensor_Humidity) },
        { be_const_key_weak(http_temperature, 41), be_const_class(be_class_Matter_Plugin_Bridge_Sensor_Temp) },
        { be_const_key_weak(http_light3, -1), be_const_class(be_class_Matter_Plugin_Bridge_Light3) },
        { be_const_key_weak(v_humidity, 1), be_const_class(be_class_Matter_Plugin_Virt_Sensor_Humidity) },
        { be_const_key_weak(http_airquality, -1), be_const_class(be_class_Matter_Plugin_Bridge_Sensor_Air_Quality) },
        { be_const_key_weak(aggregator, 11), be_const_class(be_class_Matter_Plugin_Aggregator) },
        { be_const_key_weak(rain, -1), be_const_class(be_class_Matter_Plugin_Sensor_Rain) },
        { be_const_key_weak(light0, -1), be_const_class(be_class_Matter_Plugin_Light0) },
    }))    ) } )) },
        { be_const_key_weak(udp_server, -1), be_const_var(5) },
        { be_const_key_weak(every_second, -1), be_const_closure(class_Matter_Device_every_second_closure) },
        { be_const_key_weak(MtrJoin, -1), be_const_closure(class_Matter_Device_MtrJoin_closure) },
        { be_const_key_weak(button_multi_pressed, -1), be_const_closure(class_Matter_Device_button_multi_pressed_closure) },
        { be_const_key_weak(k2l_num, -1), be_const_static_closure(class_Matter_Device_k2l_num_closure) },
        { be_const_key_weak(probe_sensor_timestamp, -1), be_const_var(21) },
        { be_const_key_weak(every_250ms, -1), be_const_closure(class_Matter_Device_every_250ms_closure) },
        { be_const_key_weak(process_attribute_expansion, -1), be_const_closure(class_Matter_Device_process_attribute_expansion_closure) },
        { be_const_key_weak(msg_received, 44), be_const_closure(class_Matter_Device_msg_received_closure) },
        { be_const_key_weak(events, -1), be_const_var(12) },
        { be_const_key_weak(VENDOR_ID, 63), be_const_int(65521) },
        { be_const_key_weak(MtrInfo, -1), be_const_closure(class_Matter_Device_MtrInfo_closure) },
        { be_const_key_weak(FILENAME, -1), be_nested_str_weak(_matter_device_X2Ejson) },
        { be_const_key_weak(ipv4only, -1), be_const_var(16) },
        { be_const_key_weak(commissioning, -1), be_const_var(8) },
        { be_const_key_weak(load_param, -1), be_const_closure(class_Matter_Device_load_param_closure) },
        { be_const_key_weak(sessions, 55), be_const_var(9) },
        { be_const_key_weak(find_plugin_by_endpoint, -1), be_const_closure(class_Matter_Device_find_plugin_by_endpoint_closure) },
        { be_const_key_weak(register_commands, -1), be_const_closure(class_Matter_Device_register_commands_closure) },
        { be_const_key_weak(next_ep, -1), be_const_var(18) },
        { be_const_key_weak(save_param, -1), be_const_closure(class_Matter_Device_save_param_closure) },
        { be_const_key_weak(bridge_remove_endpoint, -1), be_const_closure(class_Matter_Device_bridge_remove_endpoint_closure) },
        { be_const_key_weak(bridge_add_endpoint, 64), be_const_closure(class_Matter_Device_bridge_add_endpoint_closure) },
        { be_const_key_weak(autoconf_device_map, -1), be_const_closure(class_Matter_Device_autoconf_device_map_closure) },
        { be_const_key_weak(read_sensors_scheduler, -1), be_const_closure(class_Matter_Device_read_sensors_scheduler_closure) },
        { be_const_key_weak(attribute_updated, -1), be_const_closure(class_Matter_Device_attribute_updated_closure) },
        { be_const_key_weak(find_plugin_by_friendly_name, 41), be_const_closure(class_Matter_Device_find_plugin_by_friendly_name_closure) },
        { be_const_key_weak(plugins_config_remotes, -1), be_const_var(4) },
        { be_const_key_weak(plugins_persist, -1), be_const_var(2) },
        { be_const_key_weak(sort_distinct, 32), be_const_static_closure(class_Matter_Device_sort_distinct_closure) },
        { be_const_key_weak(started, 31), be_const_var(0) },
        { be_const_key_weak(check_config_ep, -1), be_const_closure(class_Matter_Device_check_config_ep_closure) },
        { be_const_key_weak(invoke_request, 14), be_const_closure(class_Matter_Device_invoke_request_closure) },
        { be_const_key_weak(MtrUpdate, 13), be_const_closure(class_Matter_Device_MtrUpdate_closure) },
        { be_const_key_weak(get_plugin_class_arg, -1), be_const_closure(class_Matter_Device_get_plugin_class_arg_closure) },
        { be_const_key_weak(PRODUCT_ID, -1), be_const_int(32768) },
        { be_const_key_weak(update_remotes_info, -1), be_const_closure(class_Matter_Device_update_remotes_info_closure) },
        { be_const_key_weak(k2l, -1), be_const_static_closure(class_Matter_Device_k2l_closure) },
    })),
    be_str_weak(Matter_Device)
);
/********************************************************************/
/* End of solidification */
