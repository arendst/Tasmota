/* Solidification of Matter_IM.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
extern const bclass be_class_Matter_IM;
// compact class 'Matter_IM' ktab size: 209, total: 532 (saved 2584 bytes)
static const bvalue be_ktab_class_Matter_IM[209] = {
  /* K0   */  be_nested_str_weak(matter),
  /* K1   */  be_nested_str_weak(TimedRequestMessage),
  /* K2   */  be_nested_str_weak(from_TLV),
  /* K3   */  be_nested_str_weak(log),
  /* K4   */  be_nested_str_weak(MTR_X3A_X20_X3ECommand_X20_X20_X20_X28_X256i_X29_X20TimedRequest_X3D_X25i),
  /* K5   */  be_nested_str_weak(session),
  /* K6   */  be_nested_str_weak(local_session_id),
  /* K7   */  be_nested_str_weak(timeout),
  /* K8   */  be_const_int(3),
  /* K9   */  be_nested_str_weak(timed_exchanges),
  /* K10  */  be_nested_str_weak(exchange_id),
  /* K11  */  be_nested_str_weak(tasmota),
  /* K12  */  be_nested_str_weak(millis),
  /* K13  */  be_nested_str_weak(send_status),
  /* K14  */  be_const_int(0),
  /* K15  */  be_nested_str_weak(TLV),
  /* K16  */  be_nested_str_weak(get_attribute_name),
  /* K17  */  be_nested_str_weak(cluster),
  /* K18  */  be_nested_str_weak(attribute),
  /* K19  */  be_nested_str_weak(_X20_X28),
  /* K20  */  be_nested_str_weak(_X29),
  /* K21  */  be_nested_str_weak(),
  /* K22  */  be_nested_str_weak(status),
  /* K23  */  be_nested_str_weak(AttributeStatusIB),
  /* K24  */  be_nested_str_weak(path),
  /* K25  */  be_nested_str_weak(AttributePathIB),
  /* K26  */  be_nested_str_weak(StatusIB),
  /* K27  */  be_nested_str_weak(endpoint),
  /* K28  */  be_nested_str_weak(write_responses),
  /* K29  */  be_nested_str_weak(push),
  /* K30  */  be_nested_str_weak(MTR_X3A_X20_X3EWrite_Attr_X25s_X25s_X20_X2D_X20_X25s_X20STATUS_X3A_X200x_X2502X_X20_X25s),
  /* K31  */  be_nested_str_weak(SUCCESS),
  /* K32  */  be_const_int(2),
  /* K33  */  be_nested_str_weak(loglevel),
  /* K34  */  be_nested_str_weak(MTR_X3A_X20_X3EWrite_Attr_X25s_X25s_X20_X2D_X20IGNORED),
  /* K35  */  be_nested_str_weak(opcode),
  /* K36  */  be_nested_str_weak(read_request_solo),
  /* K37  */  be_nested_str_weak(from_raw),
  /* K38  */  be_nested_str_weak(raw),
  /* K39  */  be_nested_str_weak(app_payload_idx),
  /* K40  */  be_nested_str_weak(process_read_request_solo),
  /* K41  */  be_nested_str_weak(invoke_request_solo),
  /* K42  */  be_nested_str_weak(process_invoke_request_solo),
  /* K43  */  be_nested_str_weak(parse),
  /* K44  */  be_const_int(1),
  /* K45  */  be_nested_str_weak(process_status_response),
  /* K46  */  be_nested_str_weak(send_ack_now),
  /* K47  */  be_nested_str_weak(process_read_request_pull),
  /* K48  */  be_nested_str_weak(subscribe_request),
  /* K49  */  be_nested_str_weak(process_write_request),
  /* K50  */  be_nested_str_weak(process_invoke_request),
  /* K51  */  be_nested_str_weak(process_timed_request),
  /* K52  */  be_nested_str_weak(subs_shop),
  /* K53  */  be_nested_str_weak(every_50ms),
  /* K54  */  be_nested_str_weak(msg),
  /* K55  */  be_nested_str_weak(device),
  /* K56  */  be_nested_str_weak(resolve_attribute_read_solo),
  /* K57  */  be_nested_str_weak(read_attribute),
  /* K58  */  be_nested_str_weak(tlv_solo),
  /* K59  */  be_nested_str_weak(is_list),
  /* K60  */  be_nested_str_weak(is_array),
  /* K61  */  be_nested_str_weak(encode_len),
  /* K62  */  be_nested_str_weak(IM_ReportData_Pull),
  /* K63  */  be_nested_str_weak(MAX_MESSAGE),
  /* K64  */  be_nested_str_weak(MTR_X3A_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20Response_X20to_X20big_X2C_X20revert_X20to_X20non_X2Dsolo),
  /* K65  */  be_nested_str_weak(add),
  /* K66  */  be_nested_str_weak(attributedata2raw),
  /* K67  */  be_const_int(405077772),
  /* K68  */  be_nested_str_weak(attributestatus2raw),
  /* K69  */  be_nested_str_weak(MTR_X3A_X20_X3ERead_Attr_X20_X28_X256i_X29_X20_X25s_X20_X2D_X20IGNORED),
  /* K70  */  be_nested_str_weak(build_response),
  /* K71  */  be_nested_str_weak(message_handler),
  /* K72  */  be_nested_str_weak(clear),
  /* K73  */  be_nested_str_weak(encode_frame),
  /* K74  */  be_nested_str_weak(encrypt),
  /* K75  */  be_nested_str_weak(MTR_X3A_X20_X3Csnd_X20_X20_X20_X20_X20_X20_X20_X28_X256i_X29_X20id_X3D_X25i_X20exch_X3D_X25i_X20rack_X3D_X25s),
  /* K76  */  be_nested_str_weak(message_counter),
  /* K77  */  be_nested_str_weak(ack_message_counter),
  /* K78  */  be_nested_str_weak(send_response_frame),
  /* K79  */  be_nested_str_weak(to_str_val),
  /* K80  */  be_nested_str_weak(MTR_X3A_X20_X3ERead_Attr1_X28_X256i_X29_X20_X25s_X25s_X20_X2D_X20_X25s),
  /* K81  */  be_nested_str_weak(UNSUPPORTED_ATTRIBUTE),
  /* K82  */  be_nested_str_weak(MTR_X3A_X20_X3ERead_Attr1_X28_X256i_X29_X20_X25s_X25s_X20_X2D_X20STATUS_X3A_X200x_X2502X_X20_X25s),
  /* K83  */  be_nested_str_weak(MTR_X3A_X20_X3ERead_Attr1_X28_X256i_X29_X20_X25s_X25s_X20_X2D_X20IGNORED),
  /* K84  */  be_nested_str_weak(TimedRequest),
  /* K85  */  be_nested_str_weak(check_timed_request),
  /* K86  */  be_nested_str_weak(get_command_name),
  /* K87  */  be_nested_str_weak(command),
  /* K88  */  be_nested_str_weak(invoke_request),
  /* K89  */  be_nested_str_weak(command_fields),
  /* K90  */  be_nested_str_weak(_X28),
  /* K91  */  be_nested_str_weak(_X29_X20),
  /* K92  */  be_nested_str_weak(MTR_X3A_X20_X3ECommand1_X20_X20_X28_X256i_X29_X20_X25s_X20_X25s_X20_X25s),
  /* K93  */  be_const_int(354943030),
  /* K94  */  be_nested_str_weak(invokeresponse2raw),
  /* K95  */  be_nested_str_weak(MTR_X3A_X20_X3CReplied_X20_X20_X20_X28_X256i_X29_X20OK_X20exch_X3D_X25i),
  /* K96  */  be_nested_str_weak(MTR_X3A_X20_X3CReplied_X20_X20_X20_X28_X256i_X29_X20_X25s_X20_X25s),
  /* K97  */  be_nested_str_weak(MTR_X3A_X20_X3CReplied_X20_X20_X20_X28_X256i_X29_X20Status_X3D0x_X2502X_X20exch_X3D_X25i),
  /* K98  */  be_nested_str_weak(MTR_X3A_X20_Ignore_X20_X20_X20_X20_X28_X256i_X29_X20exch_X3D_X25i),
  /* K99  */  be_nested_str_weak(send_queue),
  /* K100 */  be_nested_str_weak(finished),
  /* K101 */  be_nested_str_weak(ready),
  /* K102 */  be_nested_str_weak(send_im),
  /* K103 */  be_nested_str_weak(remove_sendqueue_by_exchangeid),
  /* K104 */  be_nested_str_weak(resp),
  /* K105 */  be_nested_str_weak(SubscribeRequestMessage),
  /* K106 */  be_nested_str_weak(keep_subscriptions),
  /* K107 */  be_nested_str_weak(remove_by_session),
  /* K108 */  be_nested_str_weak(new_subscription),
  /* K109 */  be_nested_str_weak(attributes_requests),
  /* K110 */  be_nested_str_weak(Path),
  /* K111 */  be_nested_str_weak(stop_iteration),
  /* K112 */  be_nested_str_weak(MTR_X3A_X20_X3ESubscribe_X20_X28_X256i_X29_X20_X25s_X20_X28min_X3D_X25i_X2C_X20max_X3D_X25i_X2C_X20keep_X3D_X25i_X29_X20sub_X3D_X25i_X20fabric_filtered_X3D_X25s),
  /* K113 */  be_nested_str_weak(concat),
  /* K114 */  be_nested_str_weak(_X20),
  /* K115 */  be_nested_str_weak(min_interval),
  /* K116 */  be_nested_str_weak(max_interval),
  /* K117 */  be_nested_str_weak(subscription_id),
  /* K118 */  be_nested_str_weak(fabric_filtered),
  /* K119 */  be_nested_str_weak(process_read_or_subscribe_request_pull),
  /* K120 */  be_nested_str_weak(process_read_or_subscribe_request_event_pull),
  /* K121 */  be_nested_str_weak(set_event_generator_or_arr),
  /* K122 */  be_nested_str_weak(IM_SubscribeResponse_Pull),
  /* K123 */  be_nested_str_weak(tag_sub),
  /* K124 */  be_nested_str_weak(tlv2raw),
  /* K125 */  be_nested_str_weak(IM_Subscription_Shop),
  /* K126 */  be_nested_str_weak(ReadRequestMessage_solo),
  /* K127 */  be_nested_str_weak(InvokeRequestMessage_solo),
  /* K128 */  be_nested_str_weak(Matter_TLV_item),
  /* K129 */  be_nested_str_weak(findsubval),
  /* K130 */  be_nested_str_weak(find_sendqueue_by_exchangeid),
  /* K131 */  be_nested_str_weak(status_ok_received),
  /* K132 */  be_nested_str_weak(MTR_X3A_X20_X3EOK_X20_X20_X20_X20_X20_X20_X20_X20_X28_X256i_X29_X20exch_X3D_X25i_X20not_X20found),
  /* K133 */  be_nested_str_weak(MTR_X3A_X20_X3EStatus_X20_X20_X20_X20ERROR_X20_X3D_X200x_X2502X),
  /* K134 */  be_nested_str_weak(status_error_received),
  /* K135 */  be_nested_str_weak(event_requests),
  /* K136 */  be_nested_str_weak(get_node_id),
  /* K137 */  be_nested_str_weak(parse_event_filters_min_no),
  /* K138 */  be_nested_str_weak(event_filters),
  /* K139 */  be_nested_str_weak(EventGenerator),
  /* K140 */  be_nested_str_weak(start),
  /* K141 */  be_nested_str_weak(event),
  /* K142 */  be_nested_str_weak(get_event_name),
  /* K143 */  be_nested_str_weak(_X2502X),
  /* K144 */  be_nested_str_weak(_X2A_X2A),
  /* K145 */  be_nested_str_weak(_X2504X),
  /* K146 */  be_nested_str_weak(_X2A_X2A_X2A_X2A),
  /* K147 */  be_nested_str_weak(_X20_X28event_X3E_X25s_X29),
  /* K148 */  be_nested_str_weak(MTR_X3A_X20_X3ERead_Event_X28_X256i_X29_X20_X5B_X25s_X5D_X25s_X2F_X25s_X20_X25s_X25s),
  /* K149 */  be_const_class(be_class_Matter_IM),
  /* K150 */  be_nested_str_weak(int64),
  /* K151 */  be_nested_str_weak(toint64),
  /* K152 */  be_nested_str_weak(node),
  /* K153 */  be_nested_str_weak(tobytes),
  /* K154 */  be_nested_str_weak(MTR_X3A_X20node_id_X20filter_X20_X25s_X20doesn_X27t_X20match_X20_X25s),
  /* K155 */  be_nested_str_weak(tohex),
  /* K156 */  be_nested_str_weak(event_min),
  /* K157 */  be_nested_str_weak(get_exchangeid),
  /* K158 */  be_nested_str_weak(ack_received),
  /* K159 */  be_nested_str_weak(IM_Status),
  /* K160 */  be_nested_str_weak(MTR_X3A_X20_X3CSub_Alive_X20_X28_X256i_X29_X20sub_X3D_X25s),
  /* K161 */  be_nested_str_weak(is_keep_alive),
  /* K162 */  be_nested_str_weak(IM_SubscribedHeartbeat),
  /* K163 */  be_nested_str_weak(_message_handler),
  /* K164 */  be_nested_str_weak(send_enqueued),
  /* K165 */  be_nested_str_weak(ReadRequestMessage),
  /* K166 */  be_nested_str_weak(updates),
  /* K167 */  be_nested_str_weak(MTR_X3A_X20_X3CSub_Data_X20_X20_X28_X256i_X29_X20sub_X3D_X25i),
  /* K168 */  be_nested_str_weak(update_event_generator_array),
  /* K169 */  be_nested_str_weak(IM_ReportDataSubscribed_Pull),
  /* K170 */  be_nested_str_weak(find),
  /* K171 */  be_nested_str_weak(remove),
  /* K172 */  be_nested_str_weak(MTR_X3A_X20_X3ETimed_X20_X20_X20_X20_X20_X28_X256i_X29_X20TIMED_REQUEST_MISMATCH_X20no_X20prior_X20TimedRequest),
  /* K173 */  be_nested_str_weak(time_reached),
  /* K174 */  be_nested_str_weak(MTR_X3A_X20_X3ETimed_X20_X20_X20_X20_X20_X28_X256i_X29_X20TIMED_REQUEST_MISMATCH_X20timeout_X20expired),
  /* K175 */  be_nested_str_weak(expiration),
  /* K176 */  be_nested_str_weak(reached_timeout),
  /* K177 */  be_nested_str_weak(Matter_TLV_array),
  /* K178 */  be_nested_str_weak(val),
  /* K179 */  be_nested_str_weak(MTR_X3A_X20_X3ERead_Attr_X20_X28_X256i_X29_X20_X25s_X25s_X20_X2D_X20_X25s),
  /* K180 */  be_nested_str_weak(MTR_X3A_X20_X3ERead_Attr_X20_X28_X256i_X29_X20_X25s_X25s_X20_X2D_X20STATUS_X3A_X200x_X2502X_X20_X25s),
  /* K181 */  be_nested_str_weak(PathGenerator),
  /* K182 */  be_nested_str_weak(MTR_X3A_X20_X3ERead_Attr_X20_X28_X256i_X29_X20_X25s),
  /* K183 */  be_nested_str_weak(InvokeRequestMessage),
  /* K184 */  be_nested_str_weak(timed_request),
  /* K185 */  be_nested_str_weak(invoke_requests),
  /* K186 */  be_nested_str_weak(InvokeResponseMessage),
  /* K187 */  be_nested_str_weak(suppress_response),
  /* K188 */  be_nested_str_weak(invoke_responses),
  /* K189 */  be_nested_str_weak(command_path),
  /* K190 */  be_nested_str_weak(MTR_X3A_X20_X3ECommand_X20_X20_X20_X28_X256i_X29_X20_X25s_X20_X25s_X20_X25s),
  /* K191 */  be_nested_str_weak(IM_InvokeResponse),
  /* K192 */  be_nested_str_weak(WriteRequestMessage),
  /* K193 */  be_nested_str_weak(write_requests),
  /* K194 */  be_nested_str_weak(WriteResponseMessage),
  /* K195 */  be_nested_str_weak(data),
  /* K196 */  be_nested_str_weak(copy),
  /* K197 */  be_nested_str_weak(write_single_attribute_status_to_bytes),
  /* K198 */  be_nested_str_weak(MTR_X3A_X20Write_Attr_X20),
  /* K199 */  be_nested_str_weak(is_direct),
  /* K200 */  be_nested_str_weak(next_attribute),
  /* K201 */  be_nested_str_weak(get_pi),
  /* K202 */  be_nested_str_weak(write_attribute),
  /* K203 */  be_nested_str_weak(IM_WriteResponse),
  /* K204 */  be_nested_str_weak(expire_sendqueue),
  /* K205 */  be_nested_str_weak(keys),
  /* K206 */  be_nested_str_weak(path2raw),
  /* K207 */  be_nested_str_weak(send_encrypted_ack),
  /* K208 */  be_const_int(355795236),
};


extern const bclass be_class_Matter_IM;

/********************************************************************
** Solidified function: process_timed_request
********************************************************************/
be_local_closure(class_Matter_IM_process_timed_request,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_IM,     /* shared constants */
    be_str_weak(process_timed_request),
    &be_const_str_solidified,
    ( &(const binstruction[29]) {  /* code */
      0xB80E0000,  //  0000  GETNGBL	R3	K0
      0x8C0C0701,  //  0001  GETMET	R3	R3	K1
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C0702,  //  0003  GETMET	R3	R3	K2
      0x5C140400,  //  0004  MOVE	R5	R2
      0x7C0C0400,  //  0005  CALL	R3	2
      0xB8120600,  //  0006  GETNGBL	R4	K3
      0x60140018,  //  0007  GETGBL	R5	G24
      0x58180004,  //  0008  LDCONST	R6	K4
      0x881C0305,  //  0009  GETMBR	R7	R1	K5
      0x881C0F06,  //  000A  GETMBR	R7	R7	K6
      0x88200707,  //  000B  GETMBR	R8	R3	K7
      0x7C140600,  //  000C  CALL	R5	3
      0x58180008,  //  000D  LDCONST	R6	K8
      0x7C100400,  //  000E  CALL	R4	2
      0x88100109,  //  000F  GETMBR	R4	R0	K9
      0x8814030A,  //  0010  GETMBR	R5	R1	K10
      0xB81A1600,  //  0011  GETNGBL	R6	K11
      0x8C180D0C,  //  0012  GETMET	R6	R6	K12
      0x7C180200,  //  0013  CALL	R6	1
      0x881C0707,  //  0014  GETMBR	R7	R3	K7
      0x00180C07,  //  0015  ADD	R6	R6	R7
      0x98100A06,  //  0016  SETIDX	R4	R5	R6
      0x8C10010D,  //  0017  GETMET	R4	R0	K13
      0x5C180200,  //  0018  MOVE	R6	R1
      0x581C000E,  //  0019  LDCONST	R7	K14
      0x7C100600,  //  001A  CALL	R4	3
      0x50100200,  //  001B  LDBOOL	R4	1	0
      0x80040800,  //  001C  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: write_single_attribute_status_to_bytes
********************************************************************/
be_local_closure(class_Matter_IM_write_single_attribute_status_to_bytes,   /* name */
  be_nested_proto(
    15,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_IM,     /* shared constants */
    be_str_weak(write_single_attribute_status_to_bytes),
    &be_const_str_solidified,
    ( &(const binstruction[84]) {  /* code */
      0xB8120000,  //  0000  GETNGBL	R4	K0
      0x8810090F,  //  0001  GETMBR	R4	R4	K15
      0xB8160000,  //  0002  GETNGBL	R5	K0
      0x8C140B10,  //  0003  GETMET	R5	R5	K16
      0x881C0511,  //  0004  GETMBR	R7	R2	K17
      0x88200512,  //  0005  GETMBR	R8	R2	K18
      0x7C140600,  //  0006  CALL	R5	3
      0x78160002,  //  0007  JMPF	R5	#000B
      0x001A2605,  //  0008  ADD	R6	K19	R5
      0x00180D14,  //  0009  ADD	R6	R6	K20
      0x70020000,  //  000A  JMP		#000C
      0x58180015,  //  000B  LDCONST	R6	K21
      0x5C140C00,  //  000C  MOVE	R5	R6
      0x88180516,  //  000D  GETMBR	R6	R2	K22
      0x4C1C0000,  //  000E  LDNIL	R7
      0x20180C07,  //  000F  NE	R6	R6	R7
      0x781A0032,  //  0010  JMPF	R6	#0044
      0xB81A0000,  //  0011  GETNGBL	R6	K0
      0x8C180D17,  //  0012  GETMET	R6	R6	K23
      0x7C180200,  //  0013  CALL	R6	1
      0xB81E0000,  //  0014  GETNGBL	R7	K0
      0x8C1C0F19,  //  0015  GETMET	R7	R7	K25
      0x7C1C0200,  //  0016  CALL	R7	1
      0x901A3007,  //  0017  SETMBR	R6	K24	R7
      0xB81E0000,  //  0018  GETNGBL	R7	K0
      0x8C1C0F1A,  //  0019  GETMET	R7	R7	K26
      0x7C1C0200,  //  001A  CALL	R7	1
      0x901A2C07,  //  001B  SETMBR	R6	K22	R7
      0x881C0D18,  //  001C  GETMBR	R7	R6	K24
      0x8820051B,  //  001D  GETMBR	R8	R2	K27
      0x901E3608,  //  001E  SETMBR	R7	K27	R8
      0x881C0D18,  //  001F  GETMBR	R7	R6	K24
      0x88200511,  //  0020  GETMBR	R8	R2	K17
      0x901E2208,  //  0021  SETMBR	R7	K17	R8
      0x881C0D18,  //  0022  GETMBR	R7	R6	K24
      0x88200512,  //  0023  GETMBR	R8	R2	K18
      0x901E2408,  //  0024  SETMBR	R7	K18	R8
      0x881C0D16,  //  0025  GETMBR	R7	R6	K22
      0x88200516,  //  0026  GETMBR	R8	R2	K22
      0x901E2C08,  //  0027  SETMBR	R7	K22	R8
      0x881C031C,  //  0028  GETMBR	R7	R1	K28
      0x8C1C0F1D,  //  0029  GETMET	R7	R7	K29
      0x5C240C00,  //  002A  MOVE	R9	R6
      0x7C1C0400,  //  002B  CALL	R7	2
      0xB81E0600,  //  002C  GETNGBL	R7	K3
      0x60200018,  //  002D  GETGBL	R8	G24
      0x5824001E,  //  002E  LDCONST	R9	K30
      0x60280008,  //  002F  GETGBL	R10	G8
      0x5C2C0400,  //  0030  MOVE	R11	R2
      0x7C280200,  //  0031  CALL	R10	1
      0x5C2C0A00,  //  0032  MOVE	R11	R5
      0x5C300600,  //  0033  MOVE	R12	R3
      0x88340516,  //  0034  GETMBR	R13	R2	K22
      0x88380516,  //  0035  GETMBR	R14	R2	K22
      0x1C381D0E,  //  0036  EQ	R14	R14	K14
      0x783A0001,  //  0037  JMPF	R14	#003A
      0x5838001F,  //  0038  LDCONST	R14	K31
      0x70020000,  //  0039  JMP		#003B
      0x58380015,  //  003A  LDCONST	R14	K21
      0x7C200C00,  //  003B  CALL	R8	6
      0x8824051B,  //  003C  GETMBR	R9	R2	K27
      0x2024130E,  //  003D  NE	R9	R9	K14
      0x78260001,  //  003E  JMPF	R9	#0041
      0x58240020,  //  003F  LDCONST	R9	K32
      0x70020000,  //  0040  JMP		#0042
      0x58240008,  //  0041  LDCONST	R9	K8
      0x7C1C0400,  //  0042  CALL	R7	2
      0x7002000E,  //  0043  JMP		#0053
      0xB81A1600,  //  0044  GETNGBL	R6	K11
      0x8C180D21,  //  0045  GETMET	R6	R6	K33
      0x58200008,  //  0046  LDCONST	R8	K8
      0x7C180400,  //  0047  CALL	R6	2
      0x781A0009,  //  0048  JMPF	R6	#0053
      0xB81A0600,  //  0049  GETNGBL	R6	K3
      0x601C0018,  //  004A  GETGBL	R7	G24
      0x58200022,  //  004B  LDCONST	R8	K34
      0x60240008,  //  004C  GETGBL	R9	G8
      0x5C280400,  //  004D  MOVE	R10	R2
      0x7C240200,  //  004E  CALL	R9	1
      0x5C280A00,  //  004F  MOVE	R10	R5
      0x7C1C0600,  //  0050  CALL	R7	3
      0x58200008,  //  0051  LDCONST	R8	K8
      0x7C180400,  //  0052  CALL	R6	2
      0x80000000,  //  0053  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_incoming
********************************************************************/
be_local_closure(class_Matter_IM_process_incoming,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_IM,     /* shared constants */
    be_str_weak(process_incoming),
    &be_const_str_solidified,
    ( &(const binstruction[124]) {  /* code */
      0x88080323,  //  0000  GETMBR	R2	R1	K35
      0x1C0C0520,  //  0001  EQ	R3	R2	K32
      0x780E000D,  //  0002  JMPF	R3	#0011
      0x880C0124,  //  0003  GETMBR	R3	R0	K36
      0x8C0C0725,  //  0004  GETMET	R3	R3	K37
      0x88140326,  //  0005  GETMBR	R5	R1	K38
      0x88180327,  //  0006  GETMBR	R6	R1	K39
      0x7C0C0600,  //  0007  CALL	R3	3
      0x4C100000,  //  0008  LDNIL	R4
      0x20100604,  //  0009  NE	R4	R3	R4
      0x78120004,  //  000A  JMPF	R4	#0010
      0x8C100128,  //  000B  GETMET	R4	R0	K40
      0x5C180200,  //  000C  MOVE	R6	R1
      0x5C1C0600,  //  000D  MOVE	R7	R3
      0x7C100600,  //  000E  CALL	R4	3
      0x80040800,  //  000F  RET	1	R4
      0x7002000F,  //  0010  JMP		#0021
      0x540E0007,  //  0011  LDINT	R3	8
      0x1C0C0403,  //  0012  EQ	R3	R2	R3
      0x780E000C,  //  0013  JMPF	R3	#0021
      0x880C0129,  //  0014  GETMBR	R3	R0	K41
      0x8C0C0725,  //  0015  GETMET	R3	R3	K37
      0x88140326,  //  0016  GETMBR	R5	R1	K38
      0x88180327,  //  0017  GETMBR	R6	R1	K39
      0x7C0C0600,  //  0018  CALL	R3	3
      0x4C100000,  //  0019  LDNIL	R4
      0x20100604,  //  001A  NE	R4	R3	R4
      0x78120004,  //  001B  JMPF	R4	#0021
      0x8C10012A,  //  001C  GETMET	R4	R0	K42
      0x5C180200,  //  001D  MOVE	R6	R1
      0x5C1C0600,  //  001E  MOVE	R7	R3
      0x7C100600,  //  001F  CALL	R4	3
      0x80040800,  //  0020  RET	1	R4
      0xB80E0000,  //  0021  GETNGBL	R3	K0
      0x880C070F,  //  0022  GETMBR	R3	R3	K15
      0x8C0C072B,  //  0023  GETMET	R3	R3	K43
      0x88140326,  //  0024  GETMBR	R5	R1	K38
      0x88180327,  //  0025  GETMBR	R6	R1	K39
      0x7C0C0600,  //  0026  CALL	R3	3
      0x1C10052C,  //  0027  EQ	R4	R2	K44
      0x78120005,  //  0028  JMPF	R4	#002F
      0x8C10012D,  //  0029  GETMET	R4	R0	K45
      0x5C180200,  //  002A  MOVE	R6	R1
      0x5C1C0600,  //  002B  MOVE	R7	R3
      0x7C100600,  //  002C  CALL	R4	3
      0x80040800,  //  002D  RET	1	R4
      0x7002004A,  //  002E  JMP		#007A
      0x1C100520,  //  002F  EQ	R4	R2	K32
      0x78120008,  //  0030  JMPF	R4	#003A
      0x8C10012E,  //  0031  GETMET	R4	R0	K46
      0x5C180200,  //  0032  MOVE	R6	R1
      0x7C100400,  //  0033  CALL	R4	2
      0x8C10012F,  //  0034  GETMET	R4	R0	K47
      0x5C180200,  //  0035  MOVE	R6	R1
      0x5C1C0600,  //  0036  MOVE	R7	R3
      0x7C100600,  //  0037  CALL	R4	3
      0x80040800,  //  0038  RET	1	R4
      0x7002003F,  //  0039  JMP		#007A
      0x1C100508,  //  003A  EQ	R4	R2	K8
      0x78120008,  //  003B  JMPF	R4	#0045
      0x8C10012E,  //  003C  GETMET	R4	R0	K46
      0x5C180200,  //  003D  MOVE	R6	R1
      0x7C100400,  //  003E  CALL	R4	2
      0x8C100130,  //  003F  GETMET	R4	R0	K48
      0x5C180200,  //  0040  MOVE	R6	R1
      0x5C1C0600,  //  0041  MOVE	R7	R3
      0x7C100600,  //  0042  CALL	R4	3
      0x80040800,  //  0043  RET	1	R4
      0x70020034,  //  0044  JMP		#007A
      0x54120003,  //  0045  LDINT	R4	4
      0x1C100404,  //  0046  EQ	R4	R2	R4
      0x78120002,  //  0047  JMPF	R4	#004B
      0x50100000,  //  0048  LDBOOL	R4	0	0
      0x80040800,  //  0049  RET	1	R4
      0x7002002E,  //  004A  JMP		#007A
      0x54120004,  //  004B  LDINT	R4	5
      0x1C100404,  //  004C  EQ	R4	R2	R4
      0x78120002,  //  004D  JMPF	R4	#0051
      0x50100000,  //  004E  LDBOOL	R4	0	0
      0x80040800,  //  004F  RET	1	R4
      0x70020028,  //  0050  JMP		#007A
      0x54120005,  //  0051  LDINT	R4	6
      0x1C100404,  //  0052  EQ	R4	R2	R4
      0x78120008,  //  0053  JMPF	R4	#005D
      0x8C10012E,  //  0054  GETMET	R4	R0	K46
      0x5C180200,  //  0055  MOVE	R6	R1
      0x7C100400,  //  0056  CALL	R4	2
      0x8C100131,  //  0057  GETMET	R4	R0	K49
      0x5C180200,  //  0058  MOVE	R6	R1
      0x5C1C0600,  //  0059  MOVE	R7	R3
      0x7C100600,  //  005A  CALL	R4	3
      0x80040800,  //  005B  RET	1	R4
      0x7002001C,  //  005C  JMP		#007A
      0x54120006,  //  005D  LDINT	R4	7
      0x1C100404,  //  005E  EQ	R4	R2	R4
      0x78120002,  //  005F  JMPF	R4	#0063
      0x50100000,  //  0060  LDBOOL	R4	0	0
      0x80040800,  //  0061  RET	1	R4
      0x70020016,  //  0062  JMP		#007A
      0x54120007,  //  0063  LDINT	R4	8
      0x1C100404,  //  0064  EQ	R4	R2	R4
      0x78120005,  //  0065  JMPF	R4	#006C
      0x8C100132,  //  0066  GETMET	R4	R0	K50
      0x5C180200,  //  0067  MOVE	R6	R1
      0x5C1C0600,  //  0068  MOVE	R7	R3
      0x7C100600,  //  0069  CALL	R4	3
      0x80040800,  //  006A  RET	1	R4
      0x7002000D,  //  006B  JMP		#007A
      0x54120008,  //  006C  LDINT	R4	9
      0x1C100404,  //  006D  EQ	R4	R2	R4
      0x78120002,  //  006E  JMPF	R4	#0072
      0x50100000,  //  006F  LDBOOL	R4	0	0
      0x80040800,  //  0070  RET	1	R4
      0x70020007,  //  0071  JMP		#007A
      0x54120009,  //  0072  LDINT	R4	10
      0x1C100404,  //  0073  EQ	R4	R2	R4
      0x78120004,  //  0074  JMPF	R4	#007A
      0x8C100133,  //  0075  GETMET	R4	R0	K51
      0x5C180200,  //  0076  MOVE	R6	R1
      0x5C1C0600,  //  0077  MOVE	R7	R3
      0x7C100600,  //  0078  CALL	R4	3
      0x80040800,  //  0079  RET	1	R4
      0x50100000,  //  007A  LDBOOL	R4	0	0
      0x80040800,  //  007B  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: every_50ms
********************************************************************/
be_local_closure(class_Matter_IM_every_50ms,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_IM,     /* shared constants */
    be_str_weak(every_50ms),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040134,  //  0000  GETMBR	R1	R0	K52
      0x8C040335,  //  0001  GETMET	R1	R1	K53
      0x7C040200,  //  0002  CALL	R1	1
      0x80000000,  //  0003  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_read_request_solo
********************************************************************/
be_local_closure(class_Matter_IM_process_read_request_solo,   /* name */
  be_nested_proto(
    19,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_IM,     /* shared constants */
    be_str_weak(process_read_request_solo),
    &be_const_str_solidified,
    ( &(const binstruction[239]) {  /* code */
      0x540E007F,  //  0000  LDINT	R3	128
      0x900A2C03,  //  0001  SETMBR	R2	K22	R3
      0x900A6C01,  //  0002  SETMBR	R2	K54	R1
      0x880C0137,  //  0003  GETMBR	R3	R0	K55
      0x8C0C0738,  //  0004  GETMET	R3	R3	K56
      0x5C140400,  //  0005  MOVE	R5	R2
      0x7C0C0400,  //  0006  CALL	R3	2
      0x4C100000,  //  0007  LDNIL	R4
      0x4C140000,  //  0008  LDNIL	R5
      0x4C180000,  //  0009  LDNIL	R6
      0x20180606,  //  000A  NE	R6	R3	R6
      0x781A0007,  //  000B  JMPF	R6	#0014
      0x541A0085,  //  000C  LDINT	R6	134
      0x900A2C06,  //  000D  SETMBR	R2	K22	R6
      0x8C180739,  //  000E  GETMET	R6	R3	K57
      0x88200305,  //  000F  GETMBR	R8	R1	K5
      0x5C240400,  //  0010  MOVE	R9	R2
      0x8828013A,  //  0011  GETMBR	R10	R0	K58
      0x7C180800,  //  0012  CALL	R6	4
      0x5C100C00,  //  0013  MOVE	R4	R6
      0x4C180000,  //  0014  LDNIL	R6
      0x20180806,  //  0015  NE	R6	R4	R6
      0x781A0036,  //  0016  JMPF	R6	#004E
      0x8818093B,  //  0017  GETMBR	R6	R4	K59
      0x741A0001,  //  0018  JMPT	R6	#001B
      0x8818093C,  //  0019  GETMBR	R6	R4	K60
      0x781A0018,  //  001A  JMPF	R6	#0034
      0x8C18093D,  //  001B  GETMET	R6	R4	K61
      0x7C180200,  //  001C  CALL	R6	1
      0xB81E0000,  //  001D  GETNGBL	R7	K0
      0x881C0F3E,  //  001E  GETMBR	R7	R7	K62
      0x881C0F3F,  //  001F  GETMBR	R7	R7	K63
      0x24180C07,  //  0020  GT	R6	R6	R7
      0x781A0011,  //  0021  JMPF	R6	#0034
      0x4C100000,  //  0022  LDNIL	R4
      0xB81A0600,  //  0023  GETNGBL	R6	K3
      0x601C0018,  //  0024  GETGBL	R7	G24
      0x58200040,  //  0025  LDCONST	R8	K64
      0x7C1C0200,  //  0026  CALL	R7	1
      0x58200008,  //  0027  LDCONST	R8	K8
      0x7C180400,  //  0028  CALL	R6	2
      0xB81A0000,  //  0029  GETNGBL	R6	K0
      0x88180D0F,  //  002A  GETMBR	R6	R6	K15
      0x8C180D2B,  //  002B  GETMET	R6	R6	K43
      0x88200326,  //  002C  GETMBR	R8	R1	K38
      0x88240327,  //  002D  GETMBR	R9	R1	K39
      0x7C180600,  //  002E  CALL	R6	3
      0x8C1C012F,  //  002F  GETMET	R7	R0	K47
      0x5C240200,  //  0030  MOVE	R9	R1
      0x5C280C00,  //  0031  MOVE	R10	R6
      0x7C1C0600,  //  0032  CALL	R7	3
      0x80040E00,  //  0033  RET	1	R7
      0x60180015,  //  0034  GETGBL	R6	G21
      0x541E002F,  //  0035  LDINT	R7	48
      0x7C180200,  //  0036  CALL	R6	1
      0x5C140C00,  //  0037  MOVE	R5	R6
      0x8C180B41,  //  0038  GETMET	R6	R5	K65
      0x54220014,  //  0039  LDINT	R8	21
      0x5824002C,  //  003A  LDCONST	R9	K44
      0x7C180600,  //  003B  CALL	R6	3
      0x8C180B41,  //  003C  GETMET	R6	R5	K65
      0x54223600,  //  003D  LDINT	R8	13825
      0x5425FFFD,  //  003E  LDINT	R9	-2
      0x7C180600,  //  003F  CALL	R6	3
      0x8C180142,  //  0040  GETMET	R6	R0	K66
      0x5C200A00,  //  0041  MOVE	R8	R5
      0x5C240400,  //  0042  MOVE	R9	R2
      0x5C280800,  //  0043  MOVE	R10	R4
      0x7C180800,  //  0044  CALL	R6	4
      0x8C180B41,  //  0045  GETMET	R6	R5	K65
      0x58200043,  //  0046  LDCONST	R8	K67
      0x5425FFFB,  //  0047  LDINT	R9	-4
      0x7C180600,  //  0048  CALL	R6	3
      0x8C180B41,  //  0049  GETMET	R6	R5	K65
      0x54220017,  //  004A  LDINT	R8	24
      0x5824002C,  //  004B  LDCONST	R9	K44
      0x7C180600,  //  004C  CALL	R6	3
      0x70020028,  //  004D  JMP		#0077
      0x88180516,  //  004E  GETMBR	R6	R2	K22
      0x4C1C0000,  //  004F  LDNIL	R7
      0x20180C07,  //  0050  NE	R6	R6	R7
      0x781A0019,  //  0051  JMPF	R6	#006C
      0x60180015,  //  0052  GETGBL	R6	G21
      0x541E002F,  //  0053  LDINT	R7	48
      0x7C180200,  //  0054  CALL	R6	1
      0x5C140C00,  //  0055  MOVE	R5	R6
      0x8C180B41,  //  0056  GETMET	R6	R5	K65
      0x54220014,  //  0057  LDINT	R8	21
      0x5824002C,  //  0058  LDCONST	R9	K44
      0x7C180600,  //  0059  CALL	R6	3
      0x8C180B41,  //  005A  GETMET	R6	R5	K65
      0x54223600,  //  005B  LDINT	R8	13825
      0x5425FFFD,  //  005C  LDINT	R9	-2
      0x7C180600,  //  005D  CALL	R6	3
      0x8C180144,  //  005E  GETMET	R6	R0	K68
      0x5C200A00,  //  005F  MOVE	R8	R5
      0x5C240400,  //  0060  MOVE	R9	R2
      0x88280516,  //  0061  GETMBR	R10	R2	K22
      0x7C180800,  //  0062  CALL	R6	4
      0x8C180B41,  //  0063  GETMET	R6	R5	K65
      0x58200043,  //  0064  LDCONST	R8	K67
      0x5425FFFB,  //  0065  LDINT	R9	-4
      0x7C180600,  //  0066  CALL	R6	3
      0x8C180B41,  //  0067  GETMET	R6	R5	K65
      0x54220017,  //  0068  LDINT	R8	24
      0x5824002C,  //  0069  LDCONST	R9	K44
      0x7C180600,  //  006A  CALL	R6	3
      0x7002000A,  //  006B  JMP		#0077
      0xB81A0600,  //  006C  GETNGBL	R6	K3
      0x601C0018,  //  006D  GETGBL	R7	G24
      0x58200045,  //  006E  LDCONST	R8	K69
      0x88240305,  //  006F  GETMBR	R9	R1	K5
      0x88241306,  //  0070  GETMBR	R9	R9	K6
      0x5C280400,  //  0071  MOVE	R10	R2
      0x7C1C0600,  //  0072  CALL	R7	3
      0x58200008,  //  0073  LDCONST	R8	K8
      0x7C180400,  //  0074  CALL	R6	2
      0x50180000,  //  0075  LDBOOL	R6	0	0
      0x80040C00,  //  0076  RET	1	R6
      0x8C180346,  //  0077  GETMET	R6	R1	K70
      0x54220004,  //  0078  LDINT	R8	5
      0x50240200,  //  0079  LDBOOL	R9	1	0
      0x7C180600,  //  007A  CALL	R6	3
      0x881C0137,  //  007B  GETMBR	R7	R0	K55
      0x881C0F47,  //  007C  GETMBR	R7	R7	K71
      0x88200326,  //  007D  GETMBR	R8	R1	K38
      0x8C241148,  //  007E  GETMET	R9	R8	K72
      0x7C240200,  //  007F  CALL	R9	1
      0x8C240D49,  //  0080  GETMET	R9	R6	K73
      0x5C2C0A00,  //  0081  MOVE	R11	R5
      0x5C301000,  //  0082  MOVE	R12	R8
      0x7C240600,  //  0083  CALL	R9	3
      0x8C240D4A,  //  0084  GETMET	R9	R6	K74
      0x7C240200,  //  0085  CALL	R9	1
      0xB8261600,  //  0086  GETNGBL	R9	K11
      0x8C241321,  //  0087  GETMET	R9	R9	K33
      0x542E0003,  //  0088  LDINT	R11	4
      0x7C240400,  //  0089  CALL	R9	2
      0x7826000A,  //  008A  JMPF	R9	#0096
      0xB8260600,  //  008B  GETNGBL	R9	K3
      0x60280018,  //  008C  GETGBL	R10	G24
      0x582C004B,  //  008D  LDCONST	R11	K75
      0x88300D05,  //  008E  GETMBR	R12	R6	K5
      0x88301906,  //  008F  GETMBR	R12	R12	K6
      0x88340D4C,  //  0090  GETMBR	R13	R6	K76
      0x88380D0A,  //  0091  GETMBR	R14	R6	K10
      0x883C0D4D,  //  0092  GETMBR	R15	R6	K77
      0x7C280A00,  //  0093  CALL	R10	5
      0x542E0003,  //  0094  LDINT	R11	4
      0x7C240400,  //  0095  CALL	R9	2
      0x8C240F4E,  //  0096  GETMET	R9	R7	K78
      0x5C2C0C00,  //  0097  MOVE	R11	R6
      0x7C240400,  //  0098  CALL	R9	2
      0x4C240000,  //  0099  LDNIL	R9
      0xB82A1600,  //  009A  GETNGBL	R10	K11
      0x8C281521,  //  009B  GETMET	R10	R10	K33
      0x58300008,  //  009C  LDCONST	R12	K8
      0x7C280400,  //  009D  CALL	R10	2
      0x782A000B,  //  009E  JMPF	R10	#00AB
      0xB82A0000,  //  009F  GETNGBL	R10	K0
      0x8C281510,  //  00A0  GETMET	R10	R10	K16
      0x88300511,  //  00A1  GETMBR	R12	R2	K17
      0x88340512,  //  00A2  GETMBR	R13	R2	K18
      0x7C280600,  //  00A3  CALL	R10	3
      0x5C241400,  //  00A4  MOVE	R9	R10
      0x78260002,  //  00A5  JMPF	R9	#00A9
      0x002A2609,  //  00A6  ADD	R10	K19	R9
      0x00281514,  //  00A7  ADD	R10	R10	K20
      0x70020000,  //  00A8  JMP		#00AA
      0x58280015,  //  00A9  LDCONST	R10	K21
      0x5C241400,  //  00AA  MOVE	R9	R10
      0x4C280000,  //  00AB  LDNIL	R10
      0x2028080A,  //  00AC  NE	R10	R4	R10
      0x782A0012,  //  00AD  JMPF	R10	#00C1
      0xB82A1600,  //  00AE  GETNGBL	R10	K11
      0x8C281521,  //  00AF  GETMET	R10	R10	K33
      0x58300008,  //  00B0  LDCONST	R12	K8
      0x7C280400,  //  00B1  CALL	R10	2
      0x782A000C,  //  00B2  JMPF	R10	#00C0
      0x8C28094F,  //  00B3  GETMET	R10	R4	K79
      0x7C280200,  //  00B4  CALL	R10	1
      0xB82E0600,  //  00B5  GETNGBL	R11	K3
      0x60300018,  //  00B6  GETGBL	R12	G24
      0x58340050,  //  00B7  LDCONST	R13	K80
      0x88380305,  //  00B8  GETMBR	R14	R1	K5
      0x88381D06,  //  00B9  GETMBR	R14	R14	K6
      0x5C3C0400,  //  00BA  MOVE	R15	R2
      0x5C401200,  //  00BB  MOVE	R16	R9
      0x5C441400,  //  00BC  MOVE	R17	R10
      0x7C300A00,  //  00BD  CALL	R12	5
      0x58340008,  //  00BE  LDCONST	R13	K8
      0x7C2C0400,  //  00BF  CALL	R11	2
      0x7002002B,  //  00C0  JMP		#00ED
      0x88280516,  //  00C1  GETMBR	R10	R2	K22
      0x4C2C0000,  //  00C2  LDNIL	R11
      0x2028140B,  //  00C3  NE	R10	R10	R11
      0x782A0018,  //  00C4  JMPF	R10	#00DE
      0x88280516,  //  00C5  GETMBR	R10	R2	K22
      0x542E0085,  //  00C6  LDINT	R11	134
      0x1C28140B,  //  00C7  EQ	R10	R10	R11
      0x782A0001,  //  00C8  JMPF	R10	#00CB
      0x58280051,  //  00C9  LDCONST	R10	K81
      0x70020000,  //  00CA  JMP		#00CC
      0x58280015,  //  00CB  LDCONST	R10	K21
      0xB82E1600,  //  00CC  GETNGBL	R11	K11
      0x8C2C1721,  //  00CD  GETMET	R11	R11	K33
      0x58340008,  //  00CE  LDCONST	R13	K8
      0x7C2C0400,  //  00CF  CALL	R11	2
      0x782E000B,  //  00D0  JMPF	R11	#00DD
      0xB82E0600,  //  00D1  GETNGBL	R11	K3
      0x60300018,  //  00D2  GETGBL	R12	G24
      0x58340052,  //  00D3  LDCONST	R13	K82
      0x88380305,  //  00D4  GETMBR	R14	R1	K5
      0x88381D06,  //  00D5  GETMBR	R14	R14	K6
      0x5C3C0400,  //  00D6  MOVE	R15	R2
      0x5C401200,  //  00D7  MOVE	R16	R9
      0x88440516,  //  00D8  GETMBR	R17	R2	K22
      0x5C481400,  //  00D9  MOVE	R18	R10
      0x7C300C00,  //  00DA  CALL	R12	6
      0x58340008,  //  00DB  LDCONST	R13	K8
      0x7C2C0400,  //  00DC  CALL	R11	2
      0x7002000E,  //  00DD  JMP		#00ED
      0xB82A1600,  //  00DE  GETNGBL	R10	K11
      0x8C281521,  //  00DF  GETMET	R10	R10	K33
      0x58300008,  //  00E0  LDCONST	R12	K8
      0x7C280400,  //  00E1  CALL	R10	2
      0x782A0009,  //  00E2  JMPF	R10	#00ED
      0xB82A0600,  //  00E3  GETNGBL	R10	K3
      0x602C0018,  //  00E4  GETGBL	R11	G24
      0x58300053,  //  00E5  LDCONST	R12	K83
      0x88340305,  //  00E6  GETMBR	R13	R1	K5
      0x88341B06,  //  00E7  GETMBR	R13	R13	K6
      0x5C380400,  //  00E8  MOVE	R14	R2
      0x5C3C1200,  //  00E9  MOVE	R15	R9
      0x7C2C0800,  //  00EA  CALL	R11	4
      0x58300008,  //  00EB  LDCONST	R12	K8
      0x7C280400,  //  00EC  CALL	R10	2
      0x50280200,  //  00ED  LDBOOL	R10	1	0
      0x80041400,  //  00EE  RET	1	R10
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_invoke_request_solo
********************************************************************/
be_local_closure(class_Matter_IM_process_invoke_request_solo,   /* name */
  be_nested_proto(
    15,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_IM,     /* shared constants */
    be_str_weak(process_invoke_request_solo),
    &be_const_str_solidified,
    ( &(const binstruction[196]) {  /* code */
      0x900A6C01,  //  0000  SETMBR	R2	K54	R1
      0x880C0554,  //  0001  GETMBR	R3	R2	K84
      0x780E000A,  //  0002  JMPF	R3	#000E
      0x8C0C0155,  //  0003  GETMET	R3	R0	K85
      0x5C140200,  //  0004  MOVE	R5	R1
      0x50180200,  //  0005  LDBOOL	R6	1	0
      0x7C0C0600,  //  0006  CALL	R3	3
      0x740E0005,  //  0007  JMPT	R3	#000E
      0x8C0C010D,  //  0008  GETMET	R3	R0	K13
      0x5C140200,  //  0009  MOVE	R5	R1
      0x541A00C8,  //  000A  LDINT	R6	201
      0x7C0C0600,  //  000B  CALL	R3	3
      0x500C0200,  //  000C  LDBOOL	R3	1	0
      0x80040600,  //  000D  RET	1	R3
      0x540E0080,  //  000E  LDINT	R3	129
      0x900A2C03,  //  000F  SETMBR	R2	K22	R3
      0xB80E0000,  //  0010  GETNGBL	R3	K0
      0x8C0C0756,  //  0011  GETMET	R3	R3	K86
      0x88140511,  //  0012  GETMBR	R5	R2	K17
      0x88180557,  //  0013  GETMBR	R6	R2	K87
      0x7C0C0600,  //  0014  CALL	R3	3
      0x60100008,  //  0015  GETGBL	R4	G8
      0x5C140400,  //  0016  MOVE	R5	R2
      0x7C100200,  //  0017  CALL	R4	1
      0x88140137,  //  0018  GETMBR	R5	R0	K55
      0x8C140B58,  //  0019  GETMET	R5	R5	K88
      0x881C0305,  //  001A  GETMBR	R7	R1	K5
      0x88200559,  //  001B  GETMBR	R8	R2	K89
      0x5C240400,  //  001C  MOVE	R9	R2
      0x7C140800,  //  001D  CALL	R5	4
      0x88180503,  //  001E  GETMBR	R6	R2	K3
      0x4C1C0000,  //  001F  LDNIL	R7
      0x20180C07,  //  0020  NE	R6	R6	R7
      0x781A0005,  //  0021  JMPF	R6	#0028
      0x60180008,  //  0022  GETGBL	R6	G8
      0x881C0503,  //  0023  GETMBR	R7	R2	K3
      0x7C180200,  //  0024  CALL	R6	1
      0x001AB406,  //  0025  ADD	R6	K90	R6
      0x00180D5B,  //  0026  ADD	R6	R6	K91
      0x70020000,  //  0027  JMP		#0029
      0x58180015,  //  0028  LDCONST	R6	K21
      0xB81E1600,  //  0029  GETNGBL	R7	K11
      0x8C1C0F21,  //  002A  GETMET	R7	R7	K33
      0x58240008,  //  002B  LDCONST	R9	K8
      0x7C1C0400,  //  002C  CALL	R7	2
      0x781E000D,  //  002D  JMPF	R7	#003C
      0xB81E0600,  //  002E  GETNGBL	R7	K3
      0x60200018,  //  002F  GETGBL	R8	G24
      0x5824005C,  //  0030  LDCONST	R9	K92
      0x88280305,  //  0031  GETMBR	R10	R1	K5
      0x88281506,  //  0032  GETMBR	R10	R10	K6
      0x5C2C0800,  //  0033  MOVE	R11	R4
      0x780E0001,  //  0034  JMPF	R3	#0037
      0x5C300600,  //  0035  MOVE	R12	R3
      0x70020000,  //  0036  JMP		#0038
      0x58300015,  //  0037  LDCONST	R12	K21
      0x5C340C00,  //  0038  MOVE	R13	R6
      0x7C200A00,  //  0039  CALL	R8	5
      0x58240008,  //  003A  LDCONST	R9	K8
      0x7C1C0400,  //  003B  CALL	R7	2
      0x4C1C0000,  //  003C  LDNIL	R7
      0x900A0607,  //  003D  SETMBR	R2	K3	R7
      0x601C0015,  //  003E  GETGBL	R7	G21
      0x5422002F,  //  003F  LDINT	R8	48
      0x7C1C0200,  //  0040  CALL	R7	1
      0x8C200F41,  //  0041  GETMET	R8	R7	K65
      0x5828005D,  //  0042  LDCONST	R10	K93
      0x542DFFFB,  //  0043  LDINT	R11	-4
      0x7C200600,  //  0044  CALL	R8	3
      0x8C200F41,  //  0045  GETMET	R8	R7	K65
      0x5828002C,  //  0046  LDCONST	R10	K44
      0x582C002C,  //  0047  LDCONST	R11	K44
      0x7C200600,  //  0048  CALL	R8	3
      0x50200200,  //  0049  LDBOOL	R8	1	0
      0x1C200A08,  //  004A  EQ	R8	R5	R8
      0x74220002,  //  004B  JMPT	R8	#004F
      0x88200516,  //  004C  GETMBR	R8	R2	K22
      0x1C20110E,  //  004D  EQ	R8	R8	K14
      0x78220014,  //  004E  JMPF	R8	#0064
      0x900A2D0E,  //  004F  SETMBR	R2	K22	K14
      0x8C20015E,  //  0050  GETMET	R8	R0	K94
      0x5C280E00,  //  0051  MOVE	R10	R7
      0x5C2C0400,  //  0052  MOVE	R11	R2
      0x4C300000,  //  0053  LDNIL	R12
      0x7C200800,  //  0054  CALL	R8	4
      0xB8221600,  //  0055  GETNGBL	R8	K11
      0x8C201121,  //  0056  GETMET	R8	R8	K33
      0x58280008,  //  0057  LDCONST	R10	K8
      0x7C200400,  //  0058  CALL	R8	2
      0x78220008,  //  0059  JMPF	R8	#0063
      0xB8220600,  //  005A  GETNGBL	R8	K3
      0x60240018,  //  005B  GETGBL	R9	G24
      0x5828005F,  //  005C  LDCONST	R10	K95
      0x882C0305,  //  005D  GETMBR	R11	R1	K5
      0x882C1706,  //  005E  GETMBR	R11	R11	K6
      0x8830030A,  //  005F  GETMBR	R12	R1	K10
      0x7C240600,  //  0060  CALL	R9	3
      0x58280008,  //  0061  LDCONST	R10	K8
      0x7C200400,  //  0062  CALL	R8	2
      0x70020043,  //  0063  JMP		#00A8
      0x4C200000,  //  0064  LDNIL	R8
      0x20200A08,  //  0065  NE	R8	R5	R8
      0x78220017,  //  0066  JMPF	R8	#007F
      0x8C20015E,  //  0067  GETMET	R8	R0	K94
      0x5C280E00,  //  0068  MOVE	R10	R7
      0x5C2C0400,  //  0069  MOVE	R11	R2
      0x5C300A00,  //  006A  MOVE	R12	R5
      0x7C200800,  //  006B  CALL	R8	4
      0x5C200600,  //  006C  MOVE	R8	R3
      0x74220000,  //  006D  JMPT	R8	#006F
      0x580C0015,  //  006E  LDCONST	R3	K21
      0xB8221600,  //  006F  GETNGBL	R8	K11
      0x8C201121,  //  0070  GETMET	R8	R8	K33
      0x58280008,  //  0071  LDCONST	R10	K8
      0x7C200400,  //  0072  CALL	R8	2
      0x78220009,  //  0073  JMPF	R8	#007E
      0xB8220600,  //  0074  GETNGBL	R8	K3
      0x60240018,  //  0075  GETGBL	R9	G24
      0x58280060,  //  0076  LDCONST	R10	K96
      0x882C0305,  //  0077  GETMBR	R11	R1	K5
      0x882C1706,  //  0078  GETMBR	R11	R11	K6
      0x5C300400,  //  0079  MOVE	R12	R2
      0x5C340600,  //  007A  MOVE	R13	R3
      0x7C240800,  //  007B  CALL	R9	4
      0x58280008,  //  007C  LDCONST	R10	K8
      0x7C200400,  //  007D  CALL	R8	2
      0x70020028,  //  007E  JMP		#00A8
      0x88200516,  //  007F  GETMBR	R8	R2	K22
      0x4C240000,  //  0080  LDNIL	R9
      0x20201009,  //  0081  NE	R8	R8	R9
      0x78220014,  //  0082  JMPF	R8	#0098
      0x8C20015E,  //  0083  GETMET	R8	R0	K94
      0x5C280E00,  //  0084  MOVE	R10	R7
      0x5C2C0400,  //  0085  MOVE	R11	R2
      0x4C300000,  //  0086  LDNIL	R12
      0x7C200800,  //  0087  CALL	R8	4
      0xB8221600,  //  0088  GETNGBL	R8	K11
      0x8C201121,  //  0089  GETMET	R8	R8	K33
      0x58280008,  //  008A  LDCONST	R10	K8
      0x7C200400,  //  008B  CALL	R8	2
      0x78220009,  //  008C  JMPF	R8	#0097
      0xB8220600,  //  008D  GETNGBL	R8	K3
      0x60240018,  //  008E  GETGBL	R9	G24
      0x58280061,  //  008F  LDCONST	R10	K97
      0x882C0305,  //  0090  GETMBR	R11	R1	K5
      0x882C1706,  //  0091  GETMBR	R11	R11	K6
      0x88300516,  //  0092  GETMBR	R12	R2	K22
      0x8834030A,  //  0093  GETMBR	R13	R1	K10
      0x7C240800,  //  0094  CALL	R9	4
      0x58280008,  //  0095  LDCONST	R10	K8
      0x7C200400,  //  0096  CALL	R8	2
      0x7002000F,  //  0097  JMP		#00A8
      0xB8221600,  //  0098  GETNGBL	R8	K11
      0x8C201121,  //  0099  GETMET	R8	R8	K33
      0x58280008,  //  009A  LDCONST	R10	K8
      0x7C200400,  //  009B  CALL	R8	2
      0x78220008,  //  009C  JMPF	R8	#00A6
      0xB8220600,  //  009D  GETNGBL	R8	K3
      0x60240018,  //  009E  GETGBL	R9	G24
      0x58280062,  //  009F  LDCONST	R10	K98
      0x882C0305,  //  00A0  GETMBR	R11	R1	K5
      0x882C1706,  //  00A1  GETMBR	R11	R11	K6
      0x8830030A,  //  00A2  GETMBR	R12	R1	K10
      0x7C240600,  //  00A3  CALL	R9	3
      0x58280008,  //  00A4  LDCONST	R10	K8
      0x7C200400,  //  00A5  CALL	R8	2
      0x50200000,  //  00A6  LDBOOL	R8	0	0
      0x80041000,  //  00A7  RET	1	R8
      0x8C200F41,  //  00A8  GETMET	R8	R7	K65
      0x58280043,  //  00A9  LDCONST	R10	K67
      0x542DFFFB,  //  00AA  LDINT	R11	-4
      0x7C200600,  //  00AB  CALL	R8	3
      0x8C200F41,  //  00AC  GETMET	R8	R7	K65
      0x542A0017,  //  00AD  LDINT	R10	24
      0x582C002C,  //  00AE  LDCONST	R11	K44
      0x7C200600,  //  00AF  CALL	R8	3
      0x8C200346,  //  00B0  GETMET	R8	R1	K70
      0x542A0008,  //  00B1  LDINT	R10	9
      0x502C0200,  //  00B2  LDBOOL	R11	1	0
      0x7C200600,  //  00B3  CALL	R8	3
      0x88240137,  //  00B4  GETMBR	R9	R0	K55
      0x88241347,  //  00B5  GETMBR	R9	R9	K71
      0x88280326,  //  00B6  GETMBR	R10	R1	K38
      0x8C2C1548,  //  00B7  GETMET	R11	R10	K72
      0x7C2C0200,  //  00B8  CALL	R11	1
      0x8C2C1149,  //  00B9  GETMET	R11	R8	K73
      0x5C340E00,  //  00BA  MOVE	R13	R7
      0x5C381400,  //  00BB  MOVE	R14	R10
      0x7C2C0600,  //  00BC  CALL	R11	3
      0x8C2C114A,  //  00BD  GETMET	R11	R8	K74
      0x7C2C0200,  //  00BE  CALL	R11	1
      0x8C2C134E,  //  00BF  GETMET	R11	R9	K78
      0x5C341000,  //  00C0  MOVE	R13	R8
      0x7C2C0400,  //  00C1  CALL	R11	2
      0x502C0200,  //  00C2  LDBOOL	R11	1	0
      0x80041600,  //  00C3  RET	1	R11
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: send_enqueued
********************************************************************/
be_local_closure(class_Matter_IM_send_enqueued,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_IM,     /* shared constants */
    be_str_weak(send_enqueued),
    &be_const_str_solidified,
    ( &(const binstruction[25]) {  /* code */
      0x5808000E,  //  0000  LDCONST	R2	K14
      0x600C000C,  //  0001  GETGBL	R3	G12
      0x88100163,  //  0002  GETMBR	R4	R0	K99
      0x7C0C0200,  //  0003  CALL	R3	1
      0x140C0403,  //  0004  LT	R3	R2	R3
      0x780E0011,  //  0005  JMPF	R3	#0018
      0x880C0163,  //  0006  GETMBR	R3	R0	K99
      0x940C0602,  //  0007  GETIDX	R3	R3	R2
      0x88100764,  //  0008  GETMBR	R4	R3	K100
      0x74120004,  //  0009  JMPT	R4	#000F
      0x88100765,  //  000A  GETMBR	R4	R3	K101
      0x78120002,  //  000B  JMPF	R4	#000F
      0x8C100766,  //  000C  GETMET	R4	R3	K102
      0x5C180200,  //  000D  MOVE	R6	R1
      0x7C100400,  //  000E  CALL	R4	2
      0x88100764,  //  000F  GETMBR	R4	R3	K100
      0x78120004,  //  0010  JMPF	R4	#0016
      0x8C100167,  //  0011  GETMET	R4	R0	K103
      0x88180768,  //  0012  GETMBR	R6	R3	K104
      0x88180D0A,  //  0013  GETMBR	R6	R6	K10
      0x7C100400,  //  0014  CALL	R4	2
      0x70020000,  //  0015  JMP		#0017
      0x0008052C,  //  0016  ADD	R2	R2	K44
      0x7001FFE8,  //  0017  JMP		#0001
      0x80000000,  //  0018  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: subscribe_request
********************************************************************/
be_local_closure(class_Matter_IM_subscribe_request,   /* name */
  be_nested_proto(
    17,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_IM,     /* shared constants */
    be_str_weak(subscribe_request),
    &be_const_str_solidified,
    ( &(const binstruction[96]) {  /* code */
      0xB80E0000,  //  0000  GETNGBL	R3	K0
      0x8C0C0769,  //  0001  GETMET	R3	R3	K105
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C0702,  //  0003  GETMET	R3	R3	K2
      0x5C140400,  //  0004  MOVE	R5	R2
      0x7C0C0400,  //  0005  CALL	R3	2
      0x8810076A,  //  0006  GETMBR	R4	R3	K106
      0x74120003,  //  0007  JMPT	R4	#000C
      0x88100134,  //  0008  GETMBR	R4	R0	K52
      0x8C10096B,  //  0009  GETMET	R4	R4	K107
      0x88180305,  //  000A  GETMBR	R6	R1	K5
      0x7C100400,  //  000B  CALL	R4	2
      0x88100134,  //  000C  GETMBR	R4	R0	K52
      0x8C10096C,  //  000D  GETMET	R4	R4	K108
      0x88180305,  //  000E  GETMBR	R6	R1	K5
      0x5C1C0600,  //  000F  MOVE	R7	R3
      0x7C100600,  //  0010  CALL	R4	3
      0xB8161600,  //  0011  GETNGBL	R5	K11
      0x8C140B21,  //  0012  GETMET	R5	R5	K33
      0x581C0008,  //  0013  LDCONST	R7	K8
      0x7C140400,  //  0014  CALL	R5	2
      0x78160032,  //  0015  JMPF	R5	#0049
      0x8814076D,  //  0016  GETMBR	R5	R3	K109
      0x4C180000,  //  0017  LDNIL	R6
      0x20140A06,  //  0018  NE	R5	R5	R6
      0x7816002E,  //  0019  JMPF	R5	#0049
      0x60140012,  //  001A  GETGBL	R5	G18
      0x7C140000,  //  001B  CALL	R5	0
      0xB81A0000,  //  001C  GETNGBL	R6	K0
      0x8C180D6E,  //  001D  GETMET	R6	R6	K110
      0x7C180200,  //  001E  CALL	R6	1
      0x901A6C01,  //  001F  SETMBR	R6	K54	R1
      0x601C0010,  //  0020  GETGBL	R7	G16
      0x8820076D,  //  0021  GETMBR	R8	R3	K109
      0x7C1C0200,  //  0022  CALL	R7	1
      0xA802000D,  //  0023  EXBLK	0	#0032
      0x5C200E00,  //  0024  MOVE	R8	R7
      0x7C200000,  //  0025  CALL	R8	0
      0x8824111B,  //  0026  GETMBR	R9	R8	K27
      0x901A3609,  //  0027  SETMBR	R6	K27	R9
      0x88241111,  //  0028  GETMBR	R9	R8	K17
      0x901A2209,  //  0029  SETMBR	R6	K17	R9
      0x88241112,  //  002A  GETMBR	R9	R8	K18
      0x901A2409,  //  002B  SETMBR	R6	K18	R9
      0x8C240B1D,  //  002C  GETMET	R9	R5	K29
      0x602C0008,  //  002D  GETGBL	R11	G8
      0x5C300C00,  //  002E  MOVE	R12	R6
      0x7C2C0200,  //  002F  CALL	R11	1
      0x7C240400,  //  0030  CALL	R9	2
      0x7001FFF1,  //  0031  JMP		#0024
      0x581C006F,  //  0032  LDCONST	R7	K111
      0xAC1C0200,  //  0033  CATCH	R7	1	0
      0xB0080000,  //  0034  RAISE	2	R0	R0
      0xB81E0600,  //  0035  GETNGBL	R7	K3
      0x60200018,  //  0036  GETGBL	R8	G24
      0x58240070,  //  0037  LDCONST	R9	K112
      0x88280305,  //  0038  GETMBR	R10	R1	K5
      0x88281506,  //  0039  GETMBR	R10	R10	K6
      0x8C2C0B71,  //  003A  GETMET	R11	R5	K113
      0x58340072,  //  003B  LDCONST	R13	K114
      0x7C2C0400,  //  003C  CALL	R11	2
      0x88300973,  //  003D  GETMBR	R12	R4	K115
      0x88340974,  //  003E  GETMBR	R13	R4	K116
      0x8838076A,  //  003F  GETMBR	R14	R3	K106
      0x783A0001,  //  0040  JMPF	R14	#0043
      0x5838002C,  //  0041  LDCONST	R14	K44
      0x70020000,  //  0042  JMP		#0044
      0x5838000E,  //  0043  LDCONST	R14	K14
      0x883C0975,  //  0044  GETMBR	R15	R4	K117
      0x88400776,  //  0045  GETMBR	R16	R3	K118
      0x7C201000,  //  0046  CALL	R8	8
      0x58240008,  //  0047  LDCONST	R9	K8
      0x7C1C0400,  //  0048  CALL	R7	2
      0x8C140177,  //  0049  GETMET	R5	R0	K119
      0x5C1C0600,  //  004A  MOVE	R7	R3
      0x5C200200,  //  004B  MOVE	R8	R1
      0x7C140600,  //  004C  CALL	R5	3
      0x8C180178,  //  004D  GETMET	R6	R0	K120
      0x5C200600,  //  004E  MOVE	R8	R3
      0x5C240200,  //  004F  MOVE	R9	R1
      0x7C180600,  //  0050  CALL	R6	3
      0x8C1C0979,  //  0051  GETMET	R7	R4	K121
      0x5C240C00,  //  0052  MOVE	R9	R6
      0x7C1C0400,  //  0053  CALL	R7	2
      0x881C0163,  //  0054  GETMBR	R7	R0	K99
      0x8C1C0F1D,  //  0055  GETMET	R7	R7	K29
      0xB8260000,  //  0056  GETNGBL	R9	K0
      0x8C24137A,  //  0057  GETMET	R9	R9	K122
      0x5C2C0200,  //  0058  MOVE	R11	R1
      0x5C300A00,  //  0059  MOVE	R12	R5
      0x5C340C00,  //  005A  MOVE	R13	R6
      0x5C380800,  //  005B  MOVE	R14	R4
      0x7C240A00,  //  005C  CALL	R9	5
      0x7C1C0400,  //  005D  CALL	R7	2
      0x501C0200,  //  005E  LDBOOL	R7	1	0
      0x80040E00,  //  005F  RET	1	R7
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: invokeresponse2raw
********************************************************************/
be_local_closure(class_Matter_IM_invokeresponse2raw,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_IM,     /* shared constants */
    be_str_weak(invokeresponse2raw),
    &be_const_str_solidified,
    ( &(const binstruction[147]) {  /* code */
      0x8C100341,  //  0000  GETMET	R4	R1	K65
      0x541A0014,  //  0001  LDINT	R6	21
      0x581C002C,  //  0002  LDCONST	R7	K44
      0x7C100600,  //  0003  CALL	R4	3
      0x4C100000,  //  0004  LDNIL	R4
      0x1C100604,  //  0005  EQ	R4	R3	R4
      0x78120004,  //  0006  JMPF	R4	#000C
      0x8C100341,  //  0007  GETMET	R4	R1	K65
      0x541A3500,  //  0008  LDINT	R6	13569
      0x541DFFFD,  //  0009  LDINT	R7	-2
      0x7C100600,  //  000A  CALL	R4	3
      0x70020003,  //  000B  JMP		#0010
      0x8C100341,  //  000C  GETMET	R4	R1	K65
      0x541A34FF,  //  000D  LDINT	R6	13568
      0x541DFFFD,  //  000E  LDINT	R7	-2
      0x7C100600,  //  000F  CALL	R4	3
      0x8C100341,  //  0010  GETMET	R4	R1	K65
      0x541A36FF,  //  0011  LDINT	R6	14080
      0x541DFFFD,  //  0012  LDINT	R7	-2
      0x7C100600,  //  0013  CALL	R4	3
      0x8810051B,  //  0014  GETMBR	R4	R2	K27
      0x541600FE,  //  0015  LDINT	R5	255
      0x18100805,  //  0016  LE	R4	R4	R5
      0x78120008,  //  0017  JMPF	R4	#0021
      0x8C100341,  //  0018  GETMET	R4	R1	K65
      0x541A23FF,  //  0019  LDINT	R6	9216
      0x541DFFFD,  //  001A  LDINT	R7	-2
      0x7C100600,  //  001B  CALL	R4	3
      0x8C100341,  //  001C  GETMET	R4	R1	K65
      0x8818051B,  //  001D  GETMBR	R6	R2	K27
      0x581C002C,  //  001E  LDCONST	R7	K44
      0x7C100600,  //  001F  CALL	R4	3
      0x70020007,  //  0020  JMP		#0029
      0x8C100341,  //  0021  GETMET	R4	R1	K65
      0x541A24FF,  //  0022  LDINT	R6	9472
      0x541DFFFD,  //  0023  LDINT	R7	-2
      0x7C100600,  //  0024  CALL	R4	3
      0x8C100341,  //  0025  GETMET	R4	R1	K65
      0x8818051B,  //  0026  GETMBR	R6	R2	K27
      0x581C0020,  //  0027  LDCONST	R7	K32
      0x7C100600,  //  0028  CALL	R4	3
      0x88100511,  //  0029  GETMBR	R4	R2	K17
      0x541600FE,  //  002A  LDINT	R5	255
      0x18100805,  //  002B  LE	R4	R4	R5
      0x78120008,  //  002C  JMPF	R4	#0036
      0x8C100341,  //  002D  GETMET	R4	R1	K65
      0x541A2400,  //  002E  LDINT	R6	9217
      0x541DFFFD,  //  002F  LDINT	R7	-2
      0x7C100600,  //  0030  CALL	R4	3
      0x8C100341,  //  0031  GETMET	R4	R1	K65
      0x88180511,  //  0032  GETMBR	R6	R2	K17
      0x581C002C,  //  0033  LDCONST	R7	K44
      0x7C100600,  //  0034  CALL	R4	3
      0x70020014,  //  0035  JMP		#004B
      0x88100511,  //  0036  GETMBR	R4	R2	K17
      0x5416FFFE,  //  0037  LDINT	R5	65535
      0x18100805,  //  0038  LE	R4	R4	R5
      0x78120008,  //  0039  JMPF	R4	#0043
      0x8C100341,  //  003A  GETMET	R4	R1	K65
      0x541A2500,  //  003B  LDINT	R6	9473
      0x541DFFFD,  //  003C  LDINT	R7	-2
      0x7C100600,  //  003D  CALL	R4	3
      0x8C100341,  //  003E  GETMET	R4	R1	K65
      0x88180511,  //  003F  GETMBR	R6	R2	K17
      0x581C0020,  //  0040  LDCONST	R7	K32
      0x7C100600,  //  0041  CALL	R4	3
      0x70020007,  //  0042  JMP		#004B
      0x8C100341,  //  0043  GETMET	R4	R1	K65
      0x541A2600,  //  0044  LDINT	R6	9729
      0x541DFFFD,  //  0045  LDINT	R7	-2
      0x7C100600,  //  0046  CALL	R4	3
      0x8C100341,  //  0047  GETMET	R4	R1	K65
      0x88180511,  //  0048  GETMBR	R6	R2	K17
      0x541E0003,  //  0049  LDINT	R7	4
      0x7C100600,  //  004A  CALL	R4	3
      0x88100557,  //  004B  GETMBR	R4	R2	K87
      0x541600FE,  //  004C  LDINT	R5	255
      0x18100805,  //  004D  LE	R4	R4	R5
      0x78120008,  //  004E  JMPF	R4	#0058
      0x8C100341,  //  004F  GETMET	R4	R1	K65
      0x541A2401,  //  0050  LDINT	R6	9218
      0x541DFFFD,  //  0051  LDINT	R7	-2
      0x7C100600,  //  0052  CALL	R4	3
      0x8C100341,  //  0053  GETMET	R4	R1	K65
      0x88180557,  //  0054  GETMBR	R6	R2	K87
      0x581C002C,  //  0055  LDCONST	R7	K44
      0x7C100600,  //  0056  CALL	R4	3
      0x70020014,  //  0057  JMP		#006D
      0x88100557,  //  0058  GETMBR	R4	R2	K87
      0x5416FFFE,  //  0059  LDINT	R5	65535
      0x18100805,  //  005A  LE	R4	R4	R5
      0x78120008,  //  005B  JMPF	R4	#0065
      0x8C100341,  //  005C  GETMET	R4	R1	K65
      0x541A2501,  //  005D  LDINT	R6	9474
      0x541DFFFD,  //  005E  LDINT	R7	-2
      0x7C100600,  //  005F  CALL	R4	3
      0x8C100341,  //  0060  GETMET	R4	R1	K65
      0x88180557,  //  0061  GETMBR	R6	R2	K87
      0x581C0020,  //  0062  LDCONST	R7	K32
      0x7C100600,  //  0063  CALL	R4	3
      0x70020007,  //  0064  JMP		#006D
      0x8C100341,  //  0065  GETMET	R4	R1	K65
      0x541A2601,  //  0066  LDINT	R6	9730
      0x541DFFFD,  //  0067  LDINT	R7	-2
      0x7C100600,  //  0068  CALL	R4	3
      0x8C100341,  //  0069  GETMET	R4	R1	K65
      0x88180557,  //  006A  GETMBR	R6	R2	K87
      0x541E0003,  //  006B  LDINT	R7	4
      0x7C100600,  //  006C  CALL	R4	3
      0x8C100341,  //  006D  GETMET	R4	R1	K65
      0x541A0017,  //  006E  LDINT	R6	24
      0x581C002C,  //  006F  LDCONST	R7	K44
      0x7C100600,  //  0070  CALL	R4	3
      0x4C100000,  //  0071  LDNIL	R4
      0x1C100604,  //  0072  EQ	R4	R3	R4
      0x78120015,  //  0073  JMPF	R4	#008A
      0x88100516,  //  0074  GETMBR	R4	R2	K22
      0x4C140000,  //  0075  LDNIL	R5
      0x1C140805,  //  0076  EQ	R5	R4	R5
      0x78160000,  //  0077  JMPF	R5	#0079
      0x5810000E,  //  0078  LDCONST	R4	K14
      0x8C140341,  //  0079  GETMET	R5	R1	K65
      0x541E3500,  //  007A  LDINT	R7	13569
      0x5421FFFD,  //  007B  LDINT	R8	-2
      0x7C140600,  //  007C  CALL	R5	3
      0x8C140341,  //  007D  GETMET	R5	R1	K65
      0x541E23FF,  //  007E  LDINT	R7	9216
      0x5421FFFD,  //  007F  LDINT	R8	-2
      0x7C140600,  //  0080  CALL	R5	3
      0x8C140341,  //  0081  GETMET	R5	R1	K65
      0x881C0516,  //  0082  GETMBR	R7	R2	K22
      0x5820002C,  //  0083  LDCONST	R8	K44
      0x7C140600,  //  0084  CALL	R5	3
      0x8C140341,  //  0085  GETMET	R5	R1	K65
      0x541E0017,  //  0086  LDINT	R7	24
      0x5820002C,  //  0087  LDCONST	R8	K44
      0x7C140600,  //  0088  CALL	R5	3
      0x70020003,  //  0089  JMP		#008E
      0x900EF72C,  //  008A  SETMBR	R3	K123	K44
      0x8C10077C,  //  008B  GETMET	R4	R3	K124
      0x5C180200,  //  008C  MOVE	R6	R1
      0x7C100400,  //  008D  CALL	R4	2
      0x8C100341,  //  008E  GETMET	R4	R1	K65
      0x541A1817,  //  008F  LDINT	R6	6168
      0x541DFFFD,  //  0090  LDINT	R7	-2
      0x7C100600,  //  0091  CALL	R4	3
      0x80000000,  //  0092  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_Matter_IM_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_IM,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[26]) {  /* code */
      0x90026E01,  //  0000  SETMBR	R0	K55	R1
      0x60080012,  //  0001  GETGBL	R2	G18
      0x7C080000,  //  0002  CALL	R2	0
      0x9002C602,  //  0003  SETMBR	R0	K99	R2
      0xB80A0000,  //  0004  GETNGBL	R2	K0
      0x8C08057D,  //  0005  GETMET	R2	R2	K125
      0x5C100000,  //  0006  MOVE	R4	R0
      0x7C080400,  //  0007  CALL	R2	2
      0x90026802,  //  0008  SETMBR	R0	K52	R2
      0xB80A0000,  //  0009  GETNGBL	R2	K0
      0x8C08057E,  //  000A  GETMET	R2	R2	K126
      0x7C080200,  //  000B  CALL	R2	1
      0x90024802,  //  000C  SETMBR	R0	K36	R2
      0xB80A0000,  //  000D  GETNGBL	R2	K0
      0x8C08057F,  //  000E  GETMET	R2	R2	K127
      0x7C080200,  //  000F  CALL	R2	1
      0x90025202,  //  0010  SETMBR	R0	K41	R2
      0xB80A0000,  //  0011  GETNGBL	R2	K0
      0x8808050F,  //  0012  GETMBR	R2	R2	K15
      0x8C080580,  //  0013  GETMET	R2	R2	K128
      0x7C080200,  //  0014  CALL	R2	1
      0x90027402,  //  0015  SETMBR	R0	K58	R2
      0x60080013,  //  0016  GETGBL	R2	G19
      0x7C080000,  //  0017  CALL	R2	0
      0x90021202,  //  0018  SETMBR	R0	K9	R2
      0x80000000,  //  0019  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_status_response
********************************************************************/
be_local_closure(class_Matter_IM_process_status_response,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_IM,     /* shared constants */
    be_str_weak(process_status_response),
    &be_const_str_solidified,
    ( &(const binstruction[41]) {  /* code */
      0x8C0C0581,  //  0000  GETMET	R3	R2	K129
      0x5814000E,  //  0001  LDCONST	R5	K14
      0x541A00FE,  //  0002  LDINT	R6	255
      0x7C0C0600,  //  0003  CALL	R3	3
      0x8C100182,  //  0004  GETMET	R4	R0	K130
      0x8818030A,  //  0005  GETMBR	R6	R1	K10
      0x7C100400,  //  0006  CALL	R4	2
      0x1C14070E,  //  0007  EQ	R5	R3	K14
      0x7816000F,  //  0008  JMPF	R5	#0019
      0x78120004,  //  0009  JMPF	R4	#000F
      0x8C140983,  //  000A  GETMET	R5	R4	K131
      0x5C1C0200,  //  000B  MOVE	R7	R1
      0x7C140400,  //  000C  CALL	R5	2
      0x80040A00,  //  000D  RET	1	R5
      0x70020008,  //  000E  JMP		#0018
      0xB8160600,  //  000F  GETNGBL	R5	K3
      0x60180018,  //  0010  GETGBL	R6	G24
      0x581C0084,  //  0011  LDCONST	R7	K132
      0x88200305,  //  0012  GETMBR	R8	R1	K5
      0x88201106,  //  0013  GETMBR	R8	R8	K6
      0x8824030A,  //  0014  GETMBR	R9	R1	K10
      0x7C180600,  //  0015  CALL	R6	3
      0x541E0003,  //  0016  LDINT	R7	4
      0x7C140400,  //  0017  CALL	R5	2
      0x7002000D,  //  0018  JMP		#0027
      0xB8160600,  //  0019  GETNGBL	R5	K3
      0x60180018,  //  001A  GETGBL	R6	G24
      0x581C0085,  //  001B  LDCONST	R7	K133
      0x5C200600,  //  001C  MOVE	R8	R3
      0x7C180400,  //  001D  CALL	R6	2
      0x581C0008,  //  001E  LDCONST	R7	K8
      0x7C140400,  //  001F  CALL	R5	2
      0x78120005,  //  0020  JMPF	R4	#0027
      0x8C140986,  //  0021  GETMET	R5	R4	K134
      0x5C1C0200,  //  0022  MOVE	R7	R1
      0x7C140400,  //  0023  CALL	R5	2
      0x8C140167,  //  0024  GETMET	R5	R0	K103
      0x881C030A,  //  0025  GETMBR	R7	R1	K10
      0x7C140400,  //  0026  CALL	R5	2
      0x50140000,  //  0027  LDBOOL	R5	0	0
      0x80040A00,  //  0028  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_read_or_subscribe_request_event_pull
********************************************************************/
be_local_closure(class_Matter_IM_process_read_or_subscribe_request_event_pull,   /* name */
  be_nested_proto(
    24,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_IM,     /* shared constants */
    be_str_weak(process_read_or_subscribe_request_event_pull),
    &be_const_str_solidified,
    ( &(const binstruction[140]) {  /* code */
      0x880C0387,  //  0000  GETMBR	R3	R1	K135
      0x4C100000,  //  0001  LDNIL	R4
      0x200C0604,  //  0002  NE	R3	R3	R4
      0x780E0085,  //  0003  JMPF	R3	#008A
      0x4C0C0000,  //  0004  LDNIL	R3
      0x4C100000,  //  0005  LDNIL	R4
      0x20100404,  //  0006  NE	R4	R2	R4
      0x78120002,  //  0007  JMPF	R4	#000B
      0x8C100588,  //  0008  GETMET	R4	R2	K136
      0x7C100200,  //  0009  CALL	R4	1
      0x70020000,  //  000A  JMP		#000C
      0x4C100000,  //  000B  LDNIL	R4
      0x88140387,  //  000C  GETMBR	R5	R1	K135
      0x78160003,  //  000D  JMPF	R5	#0012
      0x6014000C,  //  000E  GETGBL	R5	G12
      0x88180387,  //  000F  GETMBR	R6	R1	K135
      0x7C140200,  //  0010  CALL	R5	1
      0x70020000,  //  0011  JMP		#0013
      0x5814000E,  //  0012  LDCONST	R5	K14
      0x24180B2C,  //  0013  GT	R6	R5	K44
      0x781A0002,  //  0014  JMPF	R6	#0018
      0x60180012,  //  0015  GETGBL	R6	G18
      0x7C180000,  //  0016  CALL	R6	0
      0x5C0C0C00,  //  0017  MOVE	R3	R6
      0x8C180189,  //  0018  GETMET	R6	R0	K137
      0x8820038A,  //  0019  GETMBR	R8	R1	K138
      0x5C240800,  //  001A  MOVE	R9	R4
      0x7C180600,  //  001B  CALL	R6	3
      0x881C0387,  //  001C  GETMBR	R7	R1	K135
      0x781E006A,  //  001D  JMPF	R7	#0089
      0x601C0010,  //  001E  GETGBL	R7	G16
      0x88200387,  //  001F  GETMBR	R8	R1	K135
      0x7C1C0200,  //  0020  CALL	R7	1
      0xA8020063,  //  0021  EXBLK	0	#0086
      0x5C200E00,  //  0022  MOVE	R8	R7
      0x7C200000,  //  0023  CALL	R8	0
      0xB8260000,  //  0024  GETNGBL	R9	K0
      0x8C24138B,  //  0025  GETMET	R9	R9	K139
      0x882C0137,  //  0026  GETMBR	R11	R0	K55
      0x7C240400,  //  0027  CALL	R9	2
      0x8C28138C,  //  0028  GETMET	R10	R9	K140
      0x8830111B,  //  0029  GETMBR	R12	R8	K27
      0x88341111,  //  002A  GETMBR	R13	R8	K17
      0x8838118D,  //  002B  GETMBR	R14	R8	K141
      0x5C3C0C00,  //  002C  MOVE	R15	R6
      0x7C280A00,  //  002D  CALL	R10	5
      0x24280B2C,  //  002E  GT	R10	R5	K44
      0x782A0003,  //  002F  JMPF	R10	#0034
      0x8C28071D,  //  0030  GETMET	R10	R3	K29
      0x5C301200,  //  0031  MOVE	R12	R9
      0x7C280400,  //  0032  CALL	R10	2
      0x70020000,  //  0033  JMP		#0035
      0x5C0C1200,  //  0034  MOVE	R3	R9
      0xB82A1600,  //  0035  GETNGBL	R10	K11
      0x8C281521,  //  0036  GETMET	R10	R10	K33
      0x58300008,  //  0037  LDCONST	R12	K8
      0x7C280400,  //  0038  CALL	R10	2
      0x782A004A,  //  0039  JMPF	R10	#0085
      0x58280015,  //  003A  LDCONST	R10	K21
      0x882C1111,  //  003B  GETMBR	R11	R8	K17
      0x4C300000,  //  003C  LDNIL	R12
      0x202C160C,  //  003D  NE	R11	R11	R12
      0x782E0011,  //  003E  JMPF	R11	#0051
      0x882C118D,  //  003F  GETMBR	R11	R8	K141
      0x4C300000,  //  0040  LDNIL	R12
      0x202C160C,  //  0041  NE	R11	R11	R12
      0x782E000D,  //  0042  JMPF	R11	#0051
      0xB82E0000,  //  0043  GETNGBL	R11	K0
      0x8C2C178E,  //  0044  GETMET	R11	R11	K142
      0x88341111,  //  0045  GETMBR	R13	R8	K17
      0x8838118D,  //  0046  GETMBR	R14	R8	K141
      0x7C2C0600,  //  0047  CALL	R11	3
      0x5C281600,  //  0048  MOVE	R10	R11
      0x4C2C0000,  //  0049  LDNIL	R11
      0x202C140B,  //  004A  NE	R11	R10	R11
      0x782E0002,  //  004B  JMPF	R11	#004F
      0x002EB40A,  //  004C  ADD	R11	K90	R10
      0x002C175B,  //  004D  ADD	R11	R11	K91
      0x70020000,  //  004E  JMP		#0050
      0x582C0015,  //  004F  LDCONST	R11	K21
      0x5C281600,  //  0050  MOVE	R10	R11
      0x882C111B,  //  0051  GETMBR	R11	R8	K27
      0x4C300000,  //  0052  LDNIL	R12
      0x202C160C,  //  0053  NE	R11	R11	R12
      0x782E0004,  //  0054  JMPF	R11	#005A
      0x602C0018,  //  0055  GETGBL	R11	G24
      0x5830008F,  //  0056  LDCONST	R12	K143
      0x8834111B,  //  0057  GETMBR	R13	R8	K27
      0x7C2C0400,  //  0058  CALL	R11	2
      0x70020000,  //  0059  JMP		#005B
      0x582C0090,  //  005A  LDCONST	R11	K144
      0x88301111,  //  005B  GETMBR	R12	R8	K17
      0x4C340000,  //  005C  LDNIL	R13
      0x2030180D,  //  005D  NE	R12	R12	R13
      0x78320004,  //  005E  JMPF	R12	#0064
      0x60300018,  //  005F  GETGBL	R12	G24
      0x58340091,  //  0060  LDCONST	R13	K145
      0x88381111,  //  0061  GETMBR	R14	R8	K17
      0x7C300400,  //  0062  CALL	R12	2
      0x70020000,  //  0063  JMP		#0065
      0x58300092,  //  0064  LDCONST	R12	K146
      0x8834118D,  //  0065  GETMBR	R13	R8	K141
      0x4C380000,  //  0066  LDNIL	R14
      0x20341A0E,  //  0067  NE	R13	R13	R14
      0x78360004,  //  0068  JMPF	R13	#006E
      0x60340018,  //  0069  GETGBL	R13	G24
      0x5838008F,  //  006A  LDCONST	R14	K143
      0x883C118D,  //  006B  GETMBR	R15	R8	K141
      0x7C340400,  //  006C  CALL	R13	2
      0x70020000,  //  006D  JMP		#006F
      0x58340090,  //  006E  LDCONST	R13	K144
      0x4C380000,  //  006F  LDNIL	R14
      0x20380C0E,  //  0070  NE	R14	R6	R14
      0x783A0004,  //  0071  JMPF	R14	#0077
      0x60380018,  //  0072  GETGBL	R14	G24
      0x583C0093,  //  0073  LDCONST	R15	K147
      0x5C400C00,  //  0074  MOVE	R16	R6
      0x7C380400,  //  0075  CALL	R14	2
      0x70020000,  //  0076  JMP		#0078
      0x58380015,  //  0077  LDCONST	R14	K21
      0xB83E0600,  //  0078  GETNGBL	R15	K3
      0x60400018,  //  0079  GETGBL	R16	G24
      0x58440094,  //  007A  LDCONST	R17	K148
      0x88480505,  //  007B  GETMBR	R18	R2	K5
      0x88482506,  //  007C  GETMBR	R18	R18	K6
      0x5C4C1600,  //  007D  MOVE	R19	R11
      0x5C501800,  //  007E  MOVE	R20	R12
      0x5C541A00,  //  007F  MOVE	R21	R13
      0x5C581400,  //  0080  MOVE	R22	R10
      0x5C5C1C00,  //  0081  MOVE	R23	R14
      0x7C400E00,  //  0082  CALL	R16	7
      0x58440008,  //  0083  LDCONST	R17	K8
      0x7C3C0400,  //  0084  CALL	R15	2
      0x7001FF9B,  //  0085  JMP		#0022
      0x581C006F,  //  0086  LDCONST	R7	K111
      0xAC1C0200,  //  0087  CATCH	R7	1	0
      0xB0080000,  //  0088  RAISE	2	R0	R0
      0x80040600,  //  0089  RET	1	R3
      0x4C0C0000,  //  008A  LDNIL	R3
      0x80040600,  //  008B  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_event_filters_min_no
********************************************************************/
be_local_closure(class_Matter_IM_parse_event_filters_min_no,   /* name */
  be_nested_proto(
    14,                          /* nstack */
    2,                          /* argc */
    12,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_IM,     /* shared constants */
    be_str_weak(parse_event_filters_min_no),
    &be_const_str_solidified,
    ( &(const binstruction[50]) {  /* code */
      0x58080095,  //  0000  LDCONST	R2	K149
      0x4C0C0000,  //  0001  LDNIL	R3
      0x4C100000,  //  0002  LDNIL	R4
      0x20100004,  //  0003  NE	R4	R0	R4
      0x7812002B,  //  0004  JMPF	R4	#0031
      0x60100010,  //  0005  GETGBL	R4	G16
      0x5C140000,  //  0006  MOVE	R5	R0
      0x7C100200,  //  0007  CALL	R4	1
      0xA8020024,  //  0008  EXBLK	0	#002E
      0x5C140800,  //  0009  MOVE	R5	R4
      0x7C140000,  //  000A  CALL	R5	0
      0xB81B2C00,  //  000B  GETNGBL	R6	K150
      0x8C180D97,  //  000C  GETMET	R6	R6	K151
      0x88200B98,  //  000D  GETMBR	R8	R5	K152
      0x7C180400,  //  000E  CALL	R6	2
      0x781A0012,  //  000F  JMPF	R6	#0023
      0x78060011,  //  0010  JMPF	R1	#0023
      0x881C0B98,  //  0011  GETMBR	R7	R5	K152
      0x8C1C0F99,  //  0012  GETMET	R7	R7	K153
      0x7C1C0200,  //  0013  CALL	R7	1
      0x201C0E01,  //  0014  NE	R7	R7	R1
      0x781E000C,  //  0015  JMPF	R7	#0023
      0xB81E1600,  //  0016  GETNGBL	R7	K11
      0x8C1C0F03,  //  0017  GETMET	R7	R7	K3
      0x60240018,  //  0018  GETGBL	R9	G24
      0x5828009A,  //  0019  LDCONST	R10	K154
      0x8C2C0D99,  //  001A  GETMET	R11	R6	K153
      0x7C2C0200,  //  001B  CALL	R11	1
      0x8C2C179B,  //  001C  GETMET	R11	R11	K155
      0x7C2C0200,  //  001D  CALL	R11	1
      0x8C30039B,  //  001E  GETMET	R12	R1	K155
      0x7C300200,  //  001F  CALL	R12	1
      0x7C240600,  //  0020  CALL	R9	3
      0x7C1C0400,  //  0021  CALL	R7	2
      0x7001FFE5,  //  0022  JMP		#0009
      0xB81F2C00,  //  0023  GETNGBL	R7	K150
      0x8C1C0F97,  //  0024  GETMET	R7	R7	K151
      0x88240B9C,  //  0025  GETMBR	R9	R5	K156
      0x7C1C0400,  //  0026  CALL	R7	2
      0x4C200000,  //  0027  LDNIL	R8
      0x1C200608,  //  0028  EQ	R8	R3	R8
      0x74220001,  //  0029  JMPT	R8	#002C
      0x14200607,  //  002A  LT	R8	R3	R7
      0x78220000,  //  002B  JMPF	R8	#002D
      0x5C0C0E00,  //  002C  MOVE	R3	R7
      0x7001FFDA,  //  002D  JMP		#0009
      0x5810006F,  //  002E  LDCONST	R4	K111
      0xAC100200,  //  002F  CATCH	R4	1	0
      0xB0080000,  //  0030  RAISE	2	R0	R0
      0x80040600,  //  0031  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: path2raw
********************************************************************/
be_local_closure(class_Matter_IM_path2raw,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    5,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_IM,     /* shared constants */
    be_str_weak(path2raw),
    &be_const_str_solidified,
    ( &(const binstruction[107]) {  /* code */
      0x8C140341,  //  0000  GETMET	R5	R1	K65
      0x541E0036,  //  0001  LDINT	R7	55
      0x5820002C,  //  0002  LDCONST	R8	K44
      0x7C140600,  //  0003  CALL	R5	3
      0x8C140341,  //  0004  GETMET	R5	R1	K65
      0x5C1C0600,  //  0005  MOVE	R7	R3
      0x5820002C,  //  0006  LDCONST	R8	K44
      0x7C140600,  //  0007  CALL	R5	3
      0x8814051B,  //  0008  GETMBR	R5	R2	K27
      0x541A00FE,  //  0009  LDINT	R6	255
      0x18140A06,  //  000A  LE	R5	R5	R6
      0x78160008,  //  000B  JMPF	R5	#0015
      0x8C140341,  //  000C  GETMET	R5	R1	K65
      0x541E2401,  //  000D  LDINT	R7	9218
      0x5421FFFD,  //  000E  LDINT	R8	-2
      0x7C140600,  //  000F  CALL	R5	3
      0x8C140341,  //  0010  GETMET	R5	R1	K65
      0x881C051B,  //  0011  GETMBR	R7	R2	K27
      0x5820002C,  //  0012  LDCONST	R8	K44
      0x7C140600,  //  0013  CALL	R5	3
      0x70020007,  //  0014  JMP		#001D
      0x8C140341,  //  0015  GETMET	R5	R1	K65
      0x541E2501,  //  0016  LDINT	R7	9474
      0x5421FFFD,  //  0017  LDINT	R8	-2
      0x7C140600,  //  0018  CALL	R5	3
      0x8C140341,  //  0019  GETMET	R5	R1	K65
      0x881C051B,  //  001A  GETMBR	R7	R2	K27
      0x58200020,  //  001B  LDCONST	R8	K32
      0x7C140600,  //  001C  CALL	R5	3
      0x88140511,  //  001D  GETMBR	R5	R2	K17
      0x541A00FE,  //  001E  LDINT	R6	255
      0x18140A06,  //  001F  LE	R5	R5	R6
      0x78160008,  //  0020  JMPF	R5	#002A
      0x8C140341,  //  0021  GETMET	R5	R1	K65
      0x541E2402,  //  0022  LDINT	R7	9219
      0x5421FFFD,  //  0023  LDINT	R8	-2
      0x7C140600,  //  0024  CALL	R5	3
      0x8C140341,  //  0025  GETMET	R5	R1	K65
      0x881C0511,  //  0026  GETMBR	R7	R2	K17
      0x5820002C,  //  0027  LDCONST	R8	K44
      0x7C140600,  //  0028  CALL	R5	3
      0x70020014,  //  0029  JMP		#003F
      0x88140511,  //  002A  GETMBR	R5	R2	K17
      0x541AFFFE,  //  002B  LDINT	R6	65535
      0x18140A06,  //  002C  LE	R5	R5	R6
      0x78160008,  //  002D  JMPF	R5	#0037
      0x8C140341,  //  002E  GETMET	R5	R1	K65
      0x541E2502,  //  002F  LDINT	R7	9475
      0x5421FFFD,  //  0030  LDINT	R8	-2
      0x7C140600,  //  0031  CALL	R5	3
      0x8C140341,  //  0032  GETMET	R5	R1	K65
      0x881C0511,  //  0033  GETMBR	R7	R2	K17
      0x58200020,  //  0034  LDCONST	R8	K32
      0x7C140600,  //  0035  CALL	R5	3
      0x70020007,  //  0036  JMP		#003F
      0x8C140341,  //  0037  GETMET	R5	R1	K65
      0x541E2602,  //  0038  LDINT	R7	9731
      0x5421FFFD,  //  0039  LDINT	R8	-2
      0x7C140600,  //  003A  CALL	R5	3
      0x8C140341,  //  003B  GETMET	R5	R1	K65
      0x881C0511,  //  003C  GETMBR	R7	R2	K17
      0x54220003,  //  003D  LDINT	R8	4
      0x7C140600,  //  003E  CALL	R5	3
      0x88140512,  //  003F  GETMBR	R5	R2	K18
      0x541A00FE,  //  0040  LDINT	R6	255
      0x18140A06,  //  0041  LE	R5	R5	R6
      0x78160008,  //  0042  JMPF	R5	#004C
      0x8C140341,  //  0043  GETMET	R5	R1	K65
      0x541E2403,  //  0044  LDINT	R7	9220
      0x5421FFFD,  //  0045  LDINT	R8	-2
      0x7C140600,  //  0046  CALL	R5	3
      0x8C140341,  //  0047  GETMET	R5	R1	K65
      0x881C0512,  //  0048  GETMBR	R7	R2	K18
      0x5820002C,  //  0049  LDCONST	R8	K44
      0x7C140600,  //  004A  CALL	R5	3
      0x70020014,  //  004B  JMP		#0061
      0x88140512,  //  004C  GETMBR	R5	R2	K18
      0x541AFFFE,  //  004D  LDINT	R6	65535
      0x18140A06,  //  004E  LE	R5	R5	R6
      0x78160008,  //  004F  JMPF	R5	#0059
      0x8C140341,  //  0050  GETMET	R5	R1	K65
      0x541E2503,  //  0051  LDINT	R7	9476
      0x5421FFFD,  //  0052  LDINT	R8	-2
      0x7C140600,  //  0053  CALL	R5	3
      0x8C140341,  //  0054  GETMET	R5	R1	K65
      0x881C0512,  //  0055  GETMBR	R7	R2	K18
      0x58200020,  //  0056  LDCONST	R8	K32
      0x7C140600,  //  0057  CALL	R5	3
      0x70020007,  //  0058  JMP		#0061
      0x8C140341,  //  0059  GETMET	R5	R1	K65
      0x541E2603,  //  005A  LDINT	R7	9732
      0x5421FFFD,  //  005B  LDINT	R8	-2
      0x7C140600,  //  005C  CALL	R5	3
      0x8C140341,  //  005D  GETMET	R5	R1	K65
      0x881C0512,  //  005E  GETMBR	R7	R2	K18
      0x54220003,  //  005F  LDINT	R8	4
      0x7C140600,  //  0060  CALL	R5	3
      0x78120003,  //  0061  JMPF	R4	#0066
      0x8C140341,  //  0062  GETMET	R5	R1	K65
      0x541E3404,  //  0063  LDINT	R7	13317
      0x5421FFFD,  //  0064  LDINT	R8	-2
      0x7C140600,  //  0065  CALL	R5	3
      0x8C140341,  //  0066  GETMET	R5	R1	K65
      0x541E0017,  //  0067  LDINT	R7	24
      0x5820002C,  //  0068  LDCONST	R8	K44
      0x7C140600,  //  0069  CALL	R5	3
      0x80000000,  //  006A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: find_sendqueue_by_exchangeid
********************************************************************/
be_local_closure(class_Matter_IM_find_sendqueue_by_exchangeid,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_IM,     /* shared constants */
    be_str_weak(find_sendqueue_by_exchangeid),
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0001,  //  0002  JMPF	R2	#0005
      0x4C080000,  //  0003  LDNIL	R2
      0x80040400,  //  0004  RET	1	R2
      0x5808000E,  //  0005  LDCONST	R2	K14
      0x600C000C,  //  0006  GETGBL	R3	G12
      0x88100163,  //  0007  GETMBR	R4	R0	K99
      0x7C0C0200,  //  0008  CALL	R3	1
      0x140C0403,  //  0009  LT	R3	R2	R3
      0x780E0008,  //  000A  JMPF	R3	#0014
      0x880C0163,  //  000B  GETMBR	R3	R0	K99
      0x940C0602,  //  000C  GETIDX	R3	R3	R2
      0x8C10079D,  //  000D  GETMET	R4	R3	K157
      0x7C100200,  //  000E  CALL	R4	1
      0x1C100801,  //  000F  EQ	R4	R4	R1
      0x78120000,  //  0010  JMPF	R4	#0012
      0x80040600,  //  0011  RET	1	R3
      0x0008052C,  //  0012  ADD	R2	R2	K44
      0x7001FFF1,  //  0013  JMP		#0006
      0x4C0C0000,  //  0014  LDNIL	R3
      0x80040600,  //  0015  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_incoming_ack
********************************************************************/
be_local_closure(class_Matter_IM_process_incoming_ack,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_IM,     /* shared constants */
    be_str_weak(process_incoming_ack),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x8C080182,  //  0000  GETMET	R2	R0	K130
      0x8810030A,  //  0001  GETMBR	R4	R1	K10
      0x7C080400,  //  0002  CALL	R2	2
      0x780A0008,  //  0003  JMPF	R2	#000D
      0x8C0C059E,  //  0004  GETMET	R3	R2	K158
      0x5C140200,  //  0005  MOVE	R5	R1
      0x7C0C0400,  //  0006  CALL	R3	2
      0x88100564,  //  0007  GETMBR	R4	R2	K100
      0x78120002,  //  0008  JMPF	R4	#000C
      0x8C100167,  //  0009  GETMET	R4	R0	K103
      0x8818030A,  //  000A  GETMBR	R6	R1	K10
      0x7C100400,  //  000B  CALL	R4	2
      0x80040600,  //  000C  RET	1	R3
      0x500C0000,  //  000D  LDBOOL	R3	0	0
      0x80040600,  //  000E  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: send_status
********************************************************************/
be_local_closure(class_Matter_IM_send_status,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_IM,     /* shared constants */
    be_str_weak(send_status),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x880C0163,  //  0000  GETMBR	R3	R0	K99
      0x8C0C071D,  //  0001  GETMET	R3	R3	K29
      0xB8160000,  //  0002  GETNGBL	R5	K0
      0x8C140B9F,  //  0003  GETMET	R5	R5	K159
      0x5C1C0200,  //  0004  MOVE	R7	R1
      0x5C200400,  //  0005  MOVE	R8	R2
      0x7C140600,  //  0006  CALL	R5	3
      0x7C0C0400,  //  0007  CALL	R3	2
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: send_subscribe_heartbeat
********************************************************************/
be_local_closure(class_Matter_IM_send_subscribe_heartbeat,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_IM,     /* shared constants */
    be_str_weak(send_subscribe_heartbeat),
    &be_const_str_solidified,
    ( &(const binstruction[29]) {  /* code */
      0x88080305,  //  0000  GETMBR	R2	R1	K5
      0xB80E1600,  //  0001  GETNGBL	R3	K11
      0x8C0C0721,  //  0002  GETMET	R3	R3	K33
      0x58140008,  //  0003  LDCONST	R5	K8
      0x7C0C0400,  //  0004  CALL	R3	2
      0x780E0007,  //  0005  JMPF	R3	#000E
      0xB80E0600,  //  0006  GETNGBL	R3	K3
      0x60100018,  //  0007  GETGBL	R4	G24
      0x581400A0,  //  0008  LDCONST	R5	K160
      0x88180506,  //  0009  GETMBR	R6	R2	K6
      0x881C0375,  //  000A  GETMBR	R7	R1	K117
      0x7C100600,  //  000B  CALL	R4	3
      0x58140008,  //  000C  LDCONST	R5	K8
      0x7C0C0400,  //  000D  CALL	R3	2
      0x500C0200,  //  000E  LDBOOL	R3	1	0
      0x90074203,  //  000F  SETMBR	R1	K161	R3
      0x880C0163,  //  0010  GETMBR	R3	R0	K99
      0x8C0C071D,  //  0011  GETMET	R3	R3	K29
      0xB8160000,  //  0012  GETNGBL	R5	K0
      0x8C140BA2,  //  0013  GETMET	R5	R5	K162
      0x881C05A3,  //  0014  GETMBR	R7	R2	K163
      0x5C200400,  //  0015  MOVE	R8	R2
      0x5C240200,  //  0016  MOVE	R9	R1
      0x7C140800,  //  0017  CALL	R5	4
      0x7C0C0400,  //  0018  CALL	R3	2
      0x8C0C01A4,  //  0019  GETMET	R3	R0	K164
      0x881405A3,  //  001A  GETMBR	R5	R2	K163
      0x7C0C0400,  //  001B  CALL	R3	2
      0x80000000,  //  001C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: send_subscribe_update
********************************************************************/
be_local_closure(class_Matter_IM_send_subscribe_update,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_IM,     /* shared constants */
    be_str_weak(send_subscribe_update),
    &be_const_str_solidified,
    ( &(const binstruction[64]) {  /* code */
      0x88080305,  //  0000  GETMBR	R2	R1	K5
      0xB80E0000,  //  0001  GETNGBL	R3	K0
      0x8C0C07A5,  //  0002  GETMET	R3	R3	K165
      0x7C0C0200,  //  0003  CALL	R3	1
      0x50100000,  //  0004  LDBOOL	R4	0	0
      0x900EEC04,  //  0005  SETMBR	R3	K118	R4
      0x60100012,  //  0006  GETGBL	R4	G18
      0x7C100000,  //  0007  CALL	R4	0
      0x900EDA04,  //  0008  SETMBR	R3	K109	R4
      0x60100010,  //  0009  GETGBL	R4	G16
      0x881403A6,  //  000A  GETMBR	R5	R1	K166
      0x7C100200,  //  000B  CALL	R4	1
      0xA802000F,  //  000C  EXBLK	0	#001D
      0x5C140800,  //  000D  MOVE	R5	R4
      0x7C140000,  //  000E  CALL	R5	0
      0xB81A0000,  //  000F  GETNGBL	R6	K0
      0x8C180D19,  //  0010  GETMET	R6	R6	K25
      0x7C180200,  //  0011  CALL	R6	1
      0x881C0B1B,  //  0012  GETMBR	R7	R5	K27
      0x901A3607,  //  0013  SETMBR	R6	K27	R7
      0x881C0B11,  //  0014  GETMBR	R7	R5	K17
      0x901A2207,  //  0015  SETMBR	R6	K17	R7
      0x881C0B12,  //  0016  GETMBR	R7	R5	K18
      0x901A2407,  //  0017  SETMBR	R6	K18	R7
      0x881C076D,  //  0018  GETMBR	R7	R3	K109
      0x8C1C0F1D,  //  0019  GETMET	R7	R7	K29
      0x5C240C00,  //  001A  MOVE	R9	R6
      0x7C1C0400,  //  001B  CALL	R7	2
      0x7001FFEF,  //  001C  JMP		#000D
      0x5810006F,  //  001D  LDCONST	R4	K111
      0xAC100200,  //  001E  CATCH	R4	1	0
      0xB0080000,  //  001F  RAISE	2	R0	R0
      0xB8120600,  //  0020  GETNGBL	R4	K3
      0x60140018,  //  0021  GETGBL	R5	G24
      0x581800A7,  //  0022  LDCONST	R6	K167
      0x881C0506,  //  0023  GETMBR	R7	R2	K6
      0x88200375,  //  0024  GETMBR	R8	R1	K117
      0x7C140600,  //  0025  CALL	R5	3
      0x58180008,  //  0026  LDCONST	R6	K8
      0x7C100400,  //  0027  CALL	R4	2
      0x50100000,  //  0028  LDBOOL	R4	0	0
      0x90074204,  //  0029  SETMBR	R1	K161	R4
      0x8C100177,  //  002A  GETMET	R4	R0	K119
      0x5C180600,  //  002B  MOVE	R6	R3
      0x4C1C0000,  //  002C  LDNIL	R7
      0x7C100600,  //  002D  CALL	R4	3
      0x8C1403A8,  //  002E  GETMET	R5	R1	K168
      0x7C140200,  //  002F  CALL	R5	1
      0xB81A0000,  //  0030  GETNGBL	R6	K0
      0x8C180DA9,  //  0031  GETMET	R6	R6	K169
      0x882005A3,  //  0032  GETMBR	R8	R2	K163
      0x5C240400,  //  0033  MOVE	R9	R2
      0x5C280800,  //  0034  MOVE	R10	R4
      0x5C2C0A00,  //  0035  MOVE	R11	R5
      0x5C300200,  //  0036  MOVE	R12	R1
      0x7C180C00,  //  0037  CALL	R6	6
      0x881C0163,  //  0038  GETMBR	R7	R0	K99
      0x8C1C0F1D,  //  0039  GETMET	R7	R7	K29
      0x5C240C00,  //  003A  MOVE	R9	R6
      0x7C1C0400,  //  003B  CALL	R7	2
      0x8C1C01A4,  //  003C  GETMET	R7	R0	K164
      0x882405A3,  //  003D  GETMBR	R9	R2	K163
      0x7C1C0400,  //  003E  CALL	R7	2
      0x80000000,  //  003F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: check_timed_request
********************************************************************/
be_local_closure(class_Matter_IM_check_timed_request,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_IM,     /* shared constants */
    be_str_weak(check_timed_request),
    &be_const_str_solidified,
    ( &(const binstruction[42]) {  /* code */
      0x5C0C0400,  //  0000  MOVE	R3	R2
      0x740E0001,  //  0001  JMPT	R3	#0004
      0x500C0200,  //  0002  LDBOOL	R3	1	0
      0x80040600,  //  0003  RET	1	R3
      0x880C0109,  //  0004  GETMBR	R3	R0	K9
      0x8C0C07AA,  //  0005  GETMET	R3	R3	K170
      0x8814030A,  //  0006  GETMBR	R5	R1	K10
      0x7C0C0400,  //  0007  CALL	R3	2
      0x88100109,  //  0008  GETMBR	R4	R0	K9
      0x8C1009AB,  //  0009  GETMET	R4	R4	K171
      0x8818030A,  //  000A  GETMBR	R6	R1	K10
      0x7C100400,  //  000B  CALL	R4	2
      0x4C100000,  //  000C  LDNIL	R4
      0x1C100604,  //  000D  EQ	R4	R3	R4
      0x78120009,  //  000E  JMPF	R4	#0019
      0xB8120600,  //  000F  GETNGBL	R4	K3
      0x60140018,  //  0010  GETGBL	R5	G24
      0x581800AC,  //  0011  LDCONST	R6	K172
      0x881C0305,  //  0012  GETMBR	R7	R1	K5
      0x881C0F06,  //  0013  GETMBR	R7	R7	K6
      0x7C140400,  //  0014  CALL	R5	2
      0x58180008,  //  0015  LDCONST	R6	K8
      0x7C100400,  //  0016  CALL	R4	2
      0x50100000,  //  0017  LDBOOL	R4	0	0
      0x80040800,  //  0018  RET	1	R4
      0xB8121600,  //  0019  GETNGBL	R4	K11
      0x8C1009AD,  //  001A  GETMET	R4	R4	K173
      0x5C180600,  //  001B  MOVE	R6	R3
      0x7C100400,  //  001C  CALL	R4	2
      0x78120009,  //  001D  JMPF	R4	#0028
      0xB8120600,  //  001E  GETNGBL	R4	K3
      0x60140018,  //  001F  GETGBL	R5	G24
      0x581800AE,  //  0020  LDCONST	R6	K174
      0x881C0305,  //  0021  GETMBR	R7	R1	K5
      0x881C0F06,  //  0022  GETMBR	R7	R7	K6
      0x7C140400,  //  0023  CALL	R5	2
      0x58180008,  //  0024  LDCONST	R6	K8
      0x7C100400,  //  0025  CALL	R4	2
      0x50100000,  //  0026  LDBOOL	R4	0	0
      0x80040800,  //  0027  RET	1	R4
      0x50100200,  //  0028  LDBOOL	R4	1	0
      0x80040800,  //  0029  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: expire_sendqueue
********************************************************************/
be_local_closure(class_Matter_IM_expire_sendqueue,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_IM,     /* shared constants */
    be_str_weak(expire_sendqueue),
    &be_const_str_solidified,
    ( &(const binstruction[24]) {  /* code */
      0x5804000E,  //  0000  LDCONST	R1	K14
      0x6008000C,  //  0001  GETGBL	R2	G12
      0x880C0163,  //  0002  GETMBR	R3	R0	K99
      0x7C080200,  //  0003  CALL	R2	1
      0x14080202,  //  0004  LT	R2	R1	R2
      0x780A000F,  //  0005  JMPF	R2	#0016
      0x88080163,  //  0006  GETMBR	R2	R0	K99
      0x94080401,  //  0007  GETIDX	R2	R2	R1
      0xB80E1600,  //  0008  GETNGBL	R3	K11
      0x8C0C07AD,  //  0009  GETMET	R3	R3	K173
      0x881405AF,  //  000A  GETMBR	R5	R2	K175
      0x7C0C0400,  //  000B  CALL	R3	2
      0x780E0006,  //  000C  JMPF	R3	#0014
      0x8C0C05B0,  //  000D  GETMET	R3	R2	K176
      0x7C0C0200,  //  000E  CALL	R3	1
      0x880C0163,  //  000F  GETMBR	R3	R0	K99
      0x8C0C07AB,  //  0010  GETMET	R3	R3	K171
      0x5C140200,  //  0011  MOVE	R5	R1
      0x7C0C0400,  //  0012  CALL	R3	2
      0x70020000,  //  0013  JMP		#0015
      0x0004032C,  //  0014  ADD	R1	R1	K44
      0x7001FFEA,  //  0015  JMP		#0001
      0x4C080000,  //  0016  LDNIL	R2
      0x80040400,  //  0017  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: read_single_attribute_to_bytes
********************************************************************/
be_local_closure(class_Matter_IM_read_single_attribute_to_bytes,   /* name */
  be_nested_proto(
    21,                          /* nstack */
    5,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_IM,     /* shared constants */
    be_str_weak(read_single_attribute_to_bytes),
    &be_const_str_solidified,
    ( &(const binstruction[164]) {  /* code */
      0xB8160000,  //  0000  GETNGBL	R5	K0
      0x88140B0F,  //  0001  GETMBR	R5	R5	K15
      0x4C180000,  //  0002  LDNIL	R6
      0xB81E1600,  //  0003  GETNGBL	R7	K11
      0x8C1C0F21,  //  0004  GETMET	R7	R7	K33
      0x58240008,  //  0005  LDCONST	R9	K8
      0x7C1C0400,  //  0006  CALL	R7	2
      0x781E000B,  //  0007  JMPF	R7	#0014
      0xB81E0000,  //  0008  GETNGBL	R7	K0
      0x8C1C0F10,  //  0009  GETMET	R7	R7	K16
      0x88240511,  //  000A  GETMBR	R9	R2	K17
      0x88280512,  //  000B  GETMBR	R10	R2	K18
      0x7C1C0600,  //  000C  CALL	R7	3
      0x5C180E00,  //  000D  MOVE	R6	R7
      0x781A0002,  //  000E  JMPF	R6	#0012
      0x001E2606,  //  000F  ADD	R7	K19	R6
      0x001C0F14,  //  0010  ADD	R7	R7	K20
      0x70020000,  //  0011  JMP		#0013
      0x581C0015,  //  0012  LDCONST	R7	K21
      0x5C180E00,  //  0013  MOVE	R6	R7
      0x881C0516,  //  0014  GETMBR	R7	R2	K22
      0x4C200000,  //  0015  LDNIL	R8
      0x201C0E08,  //  0016  NE	R7	R7	R8
      0x4C200000,  //  0017  LDNIL	R8
      0x4C240000,  //  0018  LDNIL	R9
      0x4C280000,  //  0019  LDNIL	R10
      0x2028020A,  //  001A  NE	R10	R1	R10
      0x782A0005,  //  001B  JMPF	R10	#0022
      0x8C280339,  //  001C  GETMET	R10	R1	K57
      0x5C300600,  //  001D  MOVE	R12	R3
      0x5C340400,  //  001E  MOVE	R13	R2
      0x8838013A,  //  001F  GETMBR	R14	R0	K58
      0x7C280800,  //  0020  CALL	R10	4
      0x5C201400,  //  0021  MOVE	R8	R10
      0x4C280000,  //  0022  LDNIL	R10
      0x2028100A,  //  0023  NE	R10	R8	R10
      0x782A0057,  //  0024  JMPF	R10	#007D
      0x58280015,  //  0025  LDCONST	R10	K21
      0xB82E1600,  //  0026  GETNGBL	R11	K11
      0x8C2C1721,  //  0027  GETMET	R11	R11	K33
      0x58340008,  //  0028  LDCONST	R13	K8
      0x7C2C0400,  //  0029  CALL	R11	2
      0x782E0003,  //  002A  JMPF	R11	#002F
      0x78120002,  //  002B  JMPF	R4	#002F
      0x8C2C114F,  //  002C  GETMET	R11	R8	K79
      0x7C2C0200,  //  002D  CALL	R11	1
      0x5C281600,  //  002E  MOVE	R10	R11
      0x882C113B,  //  002F  GETMBR	R11	R8	K59
      0x742E0001,  //  0030  JMPT	R11	#0033
      0x882C113C,  //  0031  GETMBR	R11	R8	K60
      0x782E002F,  //  0032  JMPF	R11	#0063
      0x8C2C113D,  //  0033  GETMET	R11	R8	K61
      0x7C2C0200,  //  0034  CALL	R11	1
      0xB8320000,  //  0035  GETNGBL	R12	K0
      0x8830193E,  //  0036  GETMBR	R12	R12	K62
      0x8830193F,  //  0037  GETMBR	R12	R12	K63
      0x242C160C,  //  0038  GT	R11	R11	R12
      0x782E0028,  //  0039  JMPF	R11	#0063
      0x602C0012,  //  003A  GETGBL	R11	G18
      0x7C2C0000,  //  003B  CALL	R11	0
      0x5C241600,  //  003C  MOVE	R9	R11
      0x602C0015,  //  003D  GETGBL	R11	G21
      0x5432002F,  //  003E  LDINT	R12	48
      0x7C2C0200,  //  003F  CALL	R11	1
      0x8C300BB1,  //  0040  GETMET	R12	R5	K177
      0x7C300200,  //  0041  CALL	R12	1
      0x8C340142,  //  0042  GETMET	R13	R0	K66
      0x5C3C1600,  //  0043  MOVE	R15	R11
      0x5C400400,  //  0044  MOVE	R16	R2
      0x5C441800,  //  0045  MOVE	R17	R12
      0x50480000,  //  0046  LDBOOL	R18	0	0
      0x7C340A00,  //  0047  CALL	R13	5
      0x8C34131D,  //  0048  GETMET	R13	R9	K29
      0x5C3C1600,  //  0049  MOVE	R15	R11
      0x7C340400,  //  004A  CALL	R13	2
      0x60340010,  //  004B  GETGBL	R13	G16
      0x883811B2,  //  004C  GETMBR	R14	R8	K178
      0x7C340200,  //  004D  CALL	R13	1
      0xA802000F,  //  004E  EXBLK	0	#005F
      0x5C381A00,  //  004F  MOVE	R14	R13
      0x7C380000,  //  0050  CALL	R14	0
      0x603C0015,  //  0051  GETGBL	R15	G21
      0x5442002F,  //  0052  LDINT	R16	48
      0x7C3C0200,  //  0053  CALL	R15	1
      0x5C2C1E00,  //  0054  MOVE	R11	R15
      0x8C3C0142,  //  0055  GETMET	R15	R0	K66
      0x5C441600,  //  0056  MOVE	R17	R11
      0x5C480400,  //  0057  MOVE	R18	R2
      0x5C4C1C00,  //  0058  MOVE	R19	R14
      0x50500200,  //  0059  LDBOOL	R20	1	0
      0x7C3C0A00,  //  005A  CALL	R15	5
      0x8C3C131D,  //  005B  GETMET	R15	R9	K29
      0x5C441600,  //  005C  MOVE	R17	R11
      0x7C3C0400,  //  005D  CALL	R15	2
      0x7001FFEF,  //  005E  JMP		#004F
      0x5834006F,  //  005F  LDCONST	R13	K111
      0xAC340200,  //  0060  CATCH	R13	1	0
      0xB0080000,  //  0061  RAISE	2	R0	R0
      0x70020008,  //  0062  JMP		#006C
      0x602C0015,  //  0063  GETGBL	R11	G21
      0x5432002F,  //  0064  LDINT	R12	48
      0x7C2C0200,  //  0065  CALL	R11	1
      0x5C241600,  //  0066  MOVE	R9	R11
      0x8C2C0142,  //  0067  GETMET	R11	R0	K66
      0x5C341200,  //  0068  MOVE	R13	R9
      0x5C380400,  //  0069  MOVE	R14	R2
      0x5C3C1000,  //  006A  MOVE	R15	R8
      0x7C2C0800,  //  006B  CALL	R11	4
      0xB82E1600,  //  006C  GETNGBL	R11	K11
      0x8C2C1721,  //  006D  GETMET	R11	R11	K33
      0x58340008,  //  006E  LDCONST	R13	K8
      0x7C2C0400,  //  006F  CALL	R11	2
      0x782E000A,  //  0070  JMPF	R11	#007C
      0x78120009,  //  0071  JMPF	R4	#007C
      0xB82E0600,  //  0072  GETNGBL	R11	K3
      0x60300018,  //  0073  GETGBL	R12	G24
      0x583400B3,  //  0074  LDCONST	R13	K179
      0x88380706,  //  0075  GETMBR	R14	R3	K6
      0x5C3C0400,  //  0076  MOVE	R15	R2
      0x5C400C00,  //  0077  MOVE	R16	R6
      0x5C441400,  //  0078  MOVE	R17	R10
      0x7C300A00,  //  0079  CALL	R12	5
      0x58340008,  //  007A  LDCONST	R13	K8
      0x7C2C0400,  //  007B  CALL	R11	2
      0x70020025,  //  007C  JMP		#00A3
      0x88280516,  //  007D  GETMBR	R10	R2	K22
      0x4C2C0000,  //  007E  LDNIL	R11
      0x2028140B,  //  007F  NE	R10	R10	R11
      0x782A0021,  //  0080  JMPF	R10	#00A3
      0x781E0020,  //  0081  JMPF	R7	#00A3
      0x60280015,  //  0082  GETGBL	R10	G21
      0x542E002F,  //  0083  LDINT	R11	48
      0x7C280200,  //  0084  CALL	R10	1
      0x5C241400,  //  0085  MOVE	R9	R10
      0x8C280144,  //  0086  GETMET	R10	R0	K68
      0x5C301200,  //  0087  MOVE	R12	R9
      0x5C340400,  //  0088  MOVE	R13	R2
      0x88380516,  //  0089  GETMBR	R14	R2	K22
      0x7C280800,  //  008A  CALL	R10	4
      0xB82A1600,  //  008B  GETNGBL	R10	K11
      0x8C281521,  //  008C  GETMET	R10	R10	K33
      0x58300008,  //  008D  LDCONST	R12	K8
      0x7C280400,  //  008E  CALL	R10	2
      0x782A0012,  //  008F  JMPF	R10	#00A3
      0xB82A0600,  //  0090  GETNGBL	R10	K3
      0x602C0018,  //  0091  GETGBL	R11	G24
      0x583000B4,  //  0092  LDCONST	R12	K180
      0x88340706,  //  0093  GETMBR	R13	R3	K6
      0x60380008,  //  0094  GETGBL	R14	G8
      0x5C3C0400,  //  0095  MOVE	R15	R2
      0x7C380200,  //  0096  CALL	R14	1
      0x5C3C0C00,  //  0097  MOVE	R15	R6
      0x88400516,  //  0098  GETMBR	R16	R2	K22
      0x88440516,  //  0099  GETMBR	R17	R2	K22
      0x544A0085,  //  009A  LDINT	R18	134
      0x1C442212,  //  009B  EQ	R17	R17	R18
      0x78460001,  //  009C  JMPF	R17	#009F
      0x58440051,  //  009D  LDCONST	R17	K81
      0x70020000,  //  009E  JMP		#00A0
      0x58440015,  //  009F  LDCONST	R17	K21
      0x7C2C0C00,  //  00A0  CALL	R11	6
      0x58300008,  //  00A1  LDCONST	R12	K8
      0x7C280400,  //  00A2  CALL	R10	2
      0x80041200,  //  00A3  RET	1	R9
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_read_or_subscribe_request_pull
********************************************************************/
be_local_closure(class_Matter_IM_process_read_or_subscribe_request_pull,   /* name */
  be_nested_proto(
    16,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_IM,     /* shared constants */
    be_str_weak(process_read_or_subscribe_request_pull),
    &be_const_str_solidified,
    ( &(const binstruction[115]) {  /* code */
      0x880C036D,  //  0000  GETMBR	R3	R1	K109
      0x4C100000,  //  0001  LDNIL	R4
      0x200C0604,  //  0002  NE	R3	R3	R4
      0x780E006C,  //  0003  JMPF	R3	#0071
      0x4C0C0000,  //  0004  LDNIL	R3
      0x8810036D,  //  0005  GETMBR	R4	R1	K109
      0x78120003,  //  0006  JMPF	R4	#000B
      0x6010000C,  //  0007  GETGBL	R4	G12
      0x8814036D,  //  0008  GETMBR	R5	R1	K109
      0x7C100200,  //  0009  CALL	R4	1
      0x70020000,  //  000A  JMP		#000C
      0x5810000E,  //  000B  LDCONST	R4	K14
      0x2414092C,  //  000C  GT	R5	R4	K44
      0x78160002,  //  000D  JMPF	R5	#0011
      0x60140012,  //  000E  GETGBL	R5	G18
      0x7C140000,  //  000F  CALL	R5	0
      0x5C0C0A00,  //  0010  MOVE	R3	R5
      0x60140010,  //  0011  GETGBL	R5	G16
      0x8818036D,  //  0012  GETMBR	R6	R1	K109
      0x7C140200,  //  0013  CALL	R5	1
      0xA8020057,  //  0014  EXBLK	0	#006D
      0x5C180A00,  //  0015  MOVE	R6	R5
      0x7C180000,  //  0016  CALL	R6	0
      0xB81E0000,  //  0017  GETNGBL	R7	K0
      0x8C1C0FB5,  //  0018  GETMET	R7	R7	K181
      0x88240137,  //  0019  GETMBR	R9	R0	K55
      0x7C1C0400,  //  001A  CALL	R7	2
      0x8C200F8C,  //  001B  GETMET	R8	R7	K140
      0x88280D1B,  //  001C  GETMBR	R10	R6	K27
      0x882C0D11,  //  001D  GETMBR	R11	R6	K17
      0x88300D12,  //  001E  GETMBR	R12	R6	K18
      0x88340376,  //  001F  GETMBR	R13	R1	K118
      0x7C200A00,  //  0020  CALL	R8	5
      0x2420092C,  //  0021  GT	R8	R4	K44
      0x78220003,  //  0022  JMPF	R8	#0027
      0x8C20071D,  //  0023  GETMET	R8	R3	K29
      0x5C280E00,  //  0024  MOVE	R10	R7
      0x7C200400,  //  0025  CALL	R8	2
      0x70020000,  //  0026  JMP		#0028
      0x5C0C0E00,  //  0027  MOVE	R3	R7
      0xB8221600,  //  0028  GETNGBL	R8	K11
      0x8C201121,  //  0029  GETMET	R8	R8	K33
      0x58280008,  //  002A  LDCONST	R10	K8
      0x7C200400,  //  002B  CALL	R8	2
      0x7822003E,  //  002C  JMPF	R8	#006C
      0x88200D1B,  //  002D  GETMBR	R8	R6	K27
      0x4C240000,  //  002E  LDNIL	R9
      0x1C201009,  //  002F  EQ	R8	R8	R9
      0x74220007,  //  0030  JMPT	R8	#0039
      0x88200D11,  //  0031  GETMBR	R8	R6	K17
      0x4C240000,  //  0032  LDNIL	R9
      0x1C201009,  //  0033  EQ	R8	R8	R9
      0x74220003,  //  0034  JMPT	R8	#0039
      0x88200D12,  //  0035  GETMBR	R8	R6	K18
      0x4C240000,  //  0036  LDNIL	R9
      0x1C201009,  //  0037  EQ	R8	R8	R9
      0x78220032,  //  0038  JMPF	R8	#006C
      0xB8220000,  //  0039  GETNGBL	R8	K0
      0x8C20116E,  //  003A  GETMET	R8	R8	K110
      0x7C200200,  //  003B  CALL	R8	1
      0x88240D1B,  //  003C  GETMBR	R9	R6	K27
      0x90223609,  //  003D  SETMBR	R8	K27	R9
      0x88240D11,  //  003E  GETMBR	R9	R6	K17
      0x90222209,  //  003F  SETMBR	R8	K17	R9
      0x88240D12,  //  0040  GETMBR	R9	R6	K18
      0x90222409,  //  0041  SETMBR	R8	K18	R9
      0x88240376,  //  0042  GETMBR	R9	R1	K118
      0x9022EC09,  //  0043  SETMBR	R8	K118	R9
      0x60240008,  //  0044  GETGBL	R9	G8
      0x5C281000,  //  0045  MOVE	R10	R8
      0x7C240200,  //  0046  CALL	R9	1
      0x88280D11,  //  0047  GETMBR	R10	R6	K17
      0x4C2C0000,  //  0048  LDNIL	R11
      0x2028140B,  //  0049  NE	R10	R10	R11
      0x782A0017,  //  004A  JMPF	R10	#0063
      0x88280D12,  //  004B  GETMBR	R10	R6	K18
      0x4C2C0000,  //  004C  LDNIL	R11
      0x2028140B,  //  004D  NE	R10	R10	R11
      0x782A0013,  //  004E  JMPF	R10	#0063
      0xB82A0000,  //  004F  GETNGBL	R10	K0
      0x8C281510,  //  0050  GETMET	R10	R10	K16
      0x88300D11,  //  0051  GETMBR	R12	R6	K17
      0x88340D12,  //  0052  GETMBR	R13	R6	K18
      0x7C280600,  //  0053  CALL	R10	3
      0xB82E0600,  //  0054  GETNGBL	R11	K3
      0x60300018,  //  0055  GETGBL	R12	G24
      0x583400B6,  //  0056  LDCONST	R13	K182
      0x88380505,  //  0057  GETMBR	R14	R2	K5
      0x88381D06,  //  0058  GETMBR	R14	R14	K6
      0x782A0002,  //  0059  JMPF	R10	#005D
      0x003E260A,  //  005A  ADD	R15	K19	R10
      0x003C1F14,  //  005B  ADD	R15	R15	K20
      0x70020000,  //  005C  JMP		#005E
      0x583C0015,  //  005D  LDCONST	R15	K21
      0x003C120F,  //  005E  ADD	R15	R9	R15
      0x7C300600,  //  005F  CALL	R12	3
      0x58340008,  //  0060  LDCONST	R13	K8
      0x7C2C0400,  //  0061  CALL	R11	2
      0x70020008,  //  0062  JMP		#006C
      0xB82A0600,  //  0063  GETNGBL	R10	K3
      0x602C0018,  //  0064  GETGBL	R11	G24
      0x583000B6,  //  0065  LDCONST	R12	K182
      0x88340505,  //  0066  GETMBR	R13	R2	K5
      0x88341B06,  //  0067  GETMBR	R13	R13	K6
      0x5C381200,  //  0068  MOVE	R14	R9
      0x7C2C0600,  //  0069  CALL	R11	3
      0x58300008,  //  006A  LDCONST	R12	K8
      0x7C280400,  //  006B  CALL	R10	2
      0x7001FFA7,  //  006C  JMP		#0015
      0x5814006F,  //  006D  LDCONST	R5	K111
      0xAC140200,  //  006E  CATCH	R5	1	0
      0xB0080000,  //  006F  RAISE	2	R0	R0
      0x80040600,  //  0070  RET	1	R3
      0x4C0C0000,  //  0071  LDNIL	R3
      0x80040600,  //  0072  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_invoke_request
********************************************************************/
be_local_closure(class_Matter_IM_process_invoke_request,   /* name */
  be_nested_proto(
    19,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_IM,     /* shared constants */
    be_str_weak(process_invoke_request),
    &be_const_str_solidified,
    ( &(const binstruction[228]) {  /* code */
      0xB80E0000,  //  0000  GETNGBL	R3	K0
      0x8C0C076E,  //  0001  GETMET	R3	R3	K110
      0x7C0C0200,  //  0002  CALL	R3	1
      0x900E6C01,  //  0003  SETMBR	R3	K54	R1
      0xB8120000,  //  0004  GETNGBL	R4	K0
      0x8C1009B7,  //  0005  GETMET	R4	R4	K183
      0x7C100200,  //  0006  CALL	R4	1
      0x8C100902,  //  0007  GETMET	R4	R4	K2
      0x5C180400,  //  0008  MOVE	R6	R2
      0x7C100400,  //  0009  CALL	R4	2
      0x8C140155,  //  000A  GETMET	R5	R0	K85
      0x5C1C0200,  //  000B  MOVE	R7	R1
      0x882009B8,  //  000C  GETMBR	R8	R4	K184
      0x7C140600,  //  000D  CALL	R5	3
      0x74160005,  //  000E  JMPT	R5	#0015
      0x8C14010D,  //  000F  GETMET	R5	R0	K13
      0x5C1C0200,  //  0010  MOVE	R7	R1
      0x542200C8,  //  0011  LDINT	R8	201
      0x7C140600,  //  0012  CALL	R5	3
      0x50140200,  //  0013  LDBOOL	R5	1	0
      0x80040A00,  //  0014  RET	1	R5
      0x881409B9,  //  0015  GETMBR	R5	R4	K185
      0x4C180000,  //  0016  LDNIL	R6
      0x20140A06,  //  0017  NE	R5	R5	R6
      0x781600C9,  //  0018  JMPF	R5	#00E3
      0xB8160000,  //  0019  GETNGBL	R5	K0
      0x8C140BBA,  //  001A  GETMET	R5	R5	K186
      0x7C140200,  //  001B  CALL	R5	1
      0x50180000,  //  001C  LDBOOL	R6	0	0
      0x90177606,  //  001D  SETMBR	R5	K187	R6
      0x60180012,  //  001E  GETGBL	R6	G18
      0x7C180000,  //  001F  CALL	R6	0
      0x90177806,  //  0020  SETMBR	R5	K188	R6
      0x60180010,  //  0021  GETGBL	R6	G16
      0x881C09B9,  //  0022  GETMBR	R7	R4	K185
      0x7C180200,  //  0023  CALL	R6	1
      0xA80200A8,  //  0024  EXBLK	0	#00CE
      0x5C1C0C00,  //  0025  MOVE	R7	R6
      0x7C1C0000,  //  0026  CALL	R7	0
      0x88200FBD,  //  0027  GETMBR	R8	R7	K189
      0x8820111B,  //  0028  GETMBR	R8	R8	K27
      0x900E3608,  //  0029  SETMBR	R3	K27	R8
      0x88200FBD,  //  002A  GETMBR	R8	R7	K189
      0x88201111,  //  002B  GETMBR	R8	R8	K17
      0x900E2208,  //  002C  SETMBR	R3	K17	R8
      0x88200FBD,  //  002D  GETMBR	R8	R7	K189
      0x88201157,  //  002E  GETMBR	R8	R8	K87
      0x900EAE08,  //  002F  SETMBR	R3	K87	R8
      0x54220080,  //  0030  LDINT	R8	129
      0x900E2C08,  //  0031  SETMBR	R3	K22	R8
      0xB8220000,  //  0032  GETNGBL	R8	K0
      0x8C201156,  //  0033  GETMET	R8	R8	K86
      0x88280711,  //  0034  GETMBR	R10	R3	K17
      0x882C0757,  //  0035  GETMBR	R11	R3	K87
      0x7C200600,  //  0036  CALL	R8	3
      0x60240008,  //  0037  GETGBL	R9	G8
      0x5C280600,  //  0038  MOVE	R10	R3
      0x7C240200,  //  0039  CALL	R9	1
      0x88280137,  //  003A  GETMBR	R10	R0	K55
      0x8C281558,  //  003B  GETMET	R10	R10	K88
      0x88300305,  //  003C  GETMBR	R12	R1	K5
      0x88340F59,  //  003D  GETMBR	R13	R7	K89
      0x5C380600,  //  003E  MOVE	R14	R3
      0x7C280800,  //  003F  CALL	R10	4
      0x882C0703,  //  0040  GETMBR	R11	R3	K3
      0x4C300000,  //  0041  LDNIL	R12
      0x202C160C,  //  0042  NE	R11	R11	R12
      0x782E0005,  //  0043  JMPF	R11	#004A
      0x602C0008,  //  0044  GETGBL	R11	G8
      0x88300703,  //  0045  GETMBR	R12	R3	K3
      0x7C2C0200,  //  0046  CALL	R11	1
      0x002EB40B,  //  0047  ADD	R11	K90	R11
      0x002C175B,  //  0048  ADD	R11	R11	K91
      0x70020000,  //  0049  JMP		#004B
      0x582C0015,  //  004A  LDCONST	R11	K21
      0xB8320600,  //  004B  GETNGBL	R12	K3
      0x60340018,  //  004C  GETGBL	R13	G24
      0x583800BE,  //  004D  LDCONST	R14	K190
      0x883C0305,  //  004E  GETMBR	R15	R1	K5
      0x883C1F06,  //  004F  GETMBR	R15	R15	K6
      0x5C401200,  //  0050  MOVE	R16	R9
      0x78220001,  //  0051  JMPF	R8	#0054
      0x5C441000,  //  0052  MOVE	R17	R8
      0x70020000,  //  0053  JMP		#0055
      0x58440015,  //  0054  LDCONST	R17	K21
      0x5C481600,  //  0055  MOVE	R18	R11
      0x7C340A00,  //  0056  CALL	R13	5
      0x58380008,  //  0057  LDCONST	R14	K8
      0x7C300400,  //  0058  CALL	R12	2
      0x4C300000,  //  0059  LDNIL	R12
      0x900E060C,  //  005A  SETMBR	R3	K3	R12
      0x60300015,  //  005B  GETGBL	R12	G21
      0x5436001F,  //  005C  LDINT	R13	32
      0x7C300200,  //  005D  CALL	R12	1
      0x50340200,  //  005E  LDBOOL	R13	1	0
      0x1C34140D,  //  005F  EQ	R13	R10	R13
      0x74360002,  //  0060  JMPT	R13	#0064
      0x88340716,  //  0061  GETMBR	R13	R3	K22
      0x1C341B0E,  //  0062  EQ	R13	R13	K14
      0x78360018,  //  0063  JMPF	R13	#007D
      0x900E2D0E,  //  0064  SETMBR	R3	K22	K14
      0x8C34015E,  //  0065  GETMET	R13	R0	K94
      0x5C3C1800,  //  0066  MOVE	R15	R12
      0x5C400600,  //  0067  MOVE	R16	R3
      0x4C440000,  //  0068  LDNIL	R17
      0x7C340800,  //  0069  CALL	R13	4
      0x88340BBC,  //  006A  GETMBR	R13	R5	K188
      0x8C341B1D,  //  006B  GETMET	R13	R13	K29
      0x5C3C1800,  //  006C  MOVE	R15	R12
      0x7C340400,  //  006D  CALL	R13	2
      0xB8361600,  //  006E  GETNGBL	R13	K11
      0x8C341B21,  //  006F  GETMET	R13	R13	K33
      0x583C0008,  //  0070  LDCONST	R15	K8
      0x7C340400,  //  0071  CALL	R13	2
      0x78360008,  //  0072  JMPF	R13	#007C
      0xB8360600,  //  0073  GETNGBL	R13	K3
      0x60380018,  //  0074  GETGBL	R14	G24
      0x583C005F,  //  0075  LDCONST	R15	K95
      0x88400305,  //  0076  GETMBR	R16	R1	K5
      0x88402106,  //  0077  GETMBR	R16	R16	K6
      0x8844030A,  //  0078  GETMBR	R17	R1	K10
      0x7C380600,  //  0079  CALL	R14	3
      0x583C0008,  //  007A  LDCONST	R15	K8
      0x7C340400,  //  007B  CALL	R13	2
      0x7002004F,  //  007C  JMP		#00CD
      0x4C340000,  //  007D  LDNIL	R13
      0x2034140D,  //  007E  NE	R13	R10	R13
      0x78360021,  //  007F  JMPF	R13	#00A2
      0x8C34015E,  //  0080  GETMET	R13	R0	K94
      0x5C3C1800,  //  0081  MOVE	R15	R12
      0x5C400600,  //  0082  MOVE	R16	R3
      0x5C441400,  //  0083  MOVE	R17	R10
      0x7C340800,  //  0084  CALL	R13	4
      0x88340BBC,  //  0085  GETMBR	R13	R5	K188
      0x8C341B1D,  //  0086  GETMET	R13	R13	K29
      0x5C3C1800,  //  0087  MOVE	R15	R12
      0x7C340400,  //  0088  CALL	R13	2
      0xB8360000,  //  0089  GETNGBL	R13	K0
      0x8C341B56,  //  008A  GETMET	R13	R13	K86
      0x883C0711,  //  008B  GETMBR	R15	R3	K17
      0x88400757,  //  008C  GETMBR	R16	R3	K87
      0x7C340600,  //  008D  CALL	R13	3
      0x5C201A00,  //  008E  MOVE	R8	R13
      0x5C341000,  //  008F  MOVE	R13	R8
      0x74360000,  //  0090  JMPT	R13	#0092
      0x58200015,  //  0091  LDCONST	R8	K21
      0xB8361600,  //  0092  GETNGBL	R13	K11
      0x8C341B21,  //  0093  GETMET	R13	R13	K33
      0x583C0008,  //  0094  LDCONST	R15	K8
      0x7C340400,  //  0095  CALL	R13	2
      0x78360009,  //  0096  JMPF	R13	#00A1
      0xB8360600,  //  0097  GETNGBL	R13	K3
      0x60380018,  //  0098  GETGBL	R14	G24
      0x583C0060,  //  0099  LDCONST	R15	K96
      0x88400305,  //  009A  GETMBR	R16	R1	K5
      0x88402106,  //  009B  GETMBR	R16	R16	K6
      0x5C440600,  //  009C  MOVE	R17	R3
      0x5C481000,  //  009D  MOVE	R18	R8
      0x7C380800,  //  009E  CALL	R14	4
      0x583C0008,  //  009F  LDCONST	R15	K8
      0x7C340400,  //  00A0  CALL	R13	2
      0x7002002A,  //  00A1  JMP		#00CD
      0x88340716,  //  00A2  GETMBR	R13	R3	K22
      0x4C380000,  //  00A3  LDNIL	R14
      0x20341A0E,  //  00A4  NE	R13	R13	R14
      0x78360018,  //  00A5  JMPF	R13	#00BF
      0x8C34015E,  //  00A6  GETMET	R13	R0	K94
      0x5C3C1800,  //  00A7  MOVE	R15	R12
      0x5C400600,  //  00A8  MOVE	R16	R3
      0x4C440000,  //  00A9  LDNIL	R17
      0x7C340800,  //  00AA  CALL	R13	4
      0x88340BBC,  //  00AB  GETMBR	R13	R5	K188
      0x8C341B1D,  //  00AC  GETMET	R13	R13	K29
      0x5C3C1800,  //  00AD  MOVE	R15	R12
      0x7C340400,  //  00AE  CALL	R13	2
      0xB8361600,  //  00AF  GETNGBL	R13	K11
      0x8C341B21,  //  00B0  GETMET	R13	R13	K33
      0x583C0008,  //  00B1  LDCONST	R15	K8
      0x7C340400,  //  00B2  CALL	R13	2
      0x78360009,  //  00B3  JMPF	R13	#00BE
      0xB8360600,  //  00B4  GETNGBL	R13	K3
      0x60380018,  //  00B5  GETGBL	R14	G24
      0x583C0061,  //  00B6  LDCONST	R15	K97
      0x88400305,  //  00B7  GETMBR	R16	R1	K5
      0x88402106,  //  00B8  GETMBR	R16	R16	K6
      0x88440716,  //  00B9  GETMBR	R17	R3	K22
      0x8848030A,  //  00BA  GETMBR	R18	R1	K10
      0x7C380800,  //  00BB  CALL	R14	4
      0x583C0008,  //  00BC  LDCONST	R15	K8
      0x7C340400,  //  00BD  CALL	R13	2
      0x7002000D,  //  00BE  JMP		#00CD
      0xB8361600,  //  00BF  GETNGBL	R13	K11
      0x8C341B21,  //  00C0  GETMET	R13	R13	K33
      0x583C0008,  //  00C1  LDCONST	R15	K8
      0x7C340400,  //  00C2  CALL	R13	2
      0x78360008,  //  00C3  JMPF	R13	#00CD
      0xB8360600,  //  00C4  GETNGBL	R13	K3
      0x60380018,  //  00C5  GETGBL	R14	G24
      0x583C0062,  //  00C6  LDCONST	R15	K98
      0x88400305,  //  00C7  GETMBR	R16	R1	K5
      0x88402106,  //  00C8  GETMBR	R16	R16	K6
      0x8844030A,  //  00C9  GETMBR	R17	R1	K10
      0x7C380600,  //  00CA  CALL	R14	3
      0x583C0008,  //  00CB  LDCONST	R15	K8
      0x7C340400,  //  00CC  CALL	R13	2
      0x7001FF56,  //  00CD  JMP		#0025
      0x5818006F,  //  00CE  LDCONST	R6	K111
      0xAC180200,  //  00CF  CATCH	R6	1	0
      0xB0080000,  //  00D0  RAISE	2	R0	R0
      0x6018000C,  //  00D1  GETGBL	R6	G12
      0x881C0BBC,  //  00D2  GETMBR	R7	R5	K188
      0x7C180200,  //  00D3  CALL	R6	1
      0x24180D0E,  //  00D4  GT	R6	R6	K14
      0x781A0008,  //  00D5  JMPF	R6	#00DF
      0x88180163,  //  00D6  GETMBR	R6	R0	K99
      0x8C180D1D,  //  00D7  GETMET	R6	R6	K29
      0xB8220000,  //  00D8  GETNGBL	R8	K0
      0x8C2011BF,  //  00D9  GETMET	R8	R8	K191
      0x5C280200,  //  00DA  MOVE	R10	R1
      0x5C2C0A00,  //  00DB  MOVE	R11	R5
      0x7C200600,  //  00DC  CALL	R8	3
      0x7C180400,  //  00DD  CALL	R6	2
      0x70020001,  //  00DE  JMP		#00E1
      0x50180000,  //  00DF  LDBOOL	R6	0	0
      0x80040C00,  //  00E0  RET	1	R6
      0x50180200,  //  00E1  LDBOOL	R6	1	0
      0x80040C00,  //  00E2  RET	1	R6
      0x80000000,  //  00E3  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: remove_sendqueue_by_exchangeid
********************************************************************/
be_local_closure(class_Matter_IM_remove_sendqueue_by_exchangeid,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_IM,     /* shared constants */
    be_str_weak(remove_sendqueue_by_exchangeid),
    &be_const_str_solidified,
    ( &(const binstruction[24]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0000,  //  0002  JMPF	R2	#0004
      0x80000400,  //  0003  RET	0
      0x5808000E,  //  0004  LDCONST	R2	K14
      0x600C000C,  //  0005  GETGBL	R3	G12
      0x88100163,  //  0006  GETMBR	R4	R0	K99
      0x7C0C0200,  //  0007  CALL	R3	1
      0x140C0403,  //  0008  LT	R3	R2	R3
      0x780E000C,  //  0009  JMPF	R3	#0017
      0x880C0163,  //  000A  GETMBR	R3	R0	K99
      0x940C0602,  //  000B  GETIDX	R3	R3	R2
      0x8C0C079D,  //  000C  GETMET	R3	R3	K157
      0x7C0C0200,  //  000D  CALL	R3	1
      0x1C0C0601,  //  000E  EQ	R3	R3	R1
      0x780E0004,  //  000F  JMPF	R3	#0015
      0x880C0163,  //  0010  GETMBR	R3	R0	K99
      0x8C0C07AB,  //  0011  GETMET	R3	R3	K171
      0x5C140400,  //  0012  MOVE	R5	R2
      0x7C0C0400,  //  0013  CALL	R3	2
      0x70020000,  //  0014  JMP		#0016
      0x0008052C,  //  0015  ADD	R2	R2	K44
      0x7001FFED,  //  0016  JMP		#0005
      0x80000000,  //  0017  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_read_request_pull
********************************************************************/
be_local_closure(class_Matter_IM_process_read_request_pull,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_IM,     /* shared constants */
    be_str_weak(process_read_request_pull),
    &be_const_str_solidified,
    ( &(const binstruction[25]) {  /* code */
      0xB80E0000,  //  0000  GETNGBL	R3	K0
      0x8C0C07A5,  //  0001  GETMET	R3	R3	K165
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C0702,  //  0003  GETMET	R3	R3	K2
      0x5C140400,  //  0004  MOVE	R5	R2
      0x7C0C0400,  //  0005  CALL	R3	2
      0x8C100177,  //  0006  GETMET	R4	R0	K119
      0x5C180600,  //  0007  MOVE	R6	R3
      0x5C1C0200,  //  0008  MOVE	R7	R1
      0x7C100600,  //  0009  CALL	R4	3
      0x8C140178,  //  000A  GETMET	R5	R0	K120
      0x5C1C0600,  //  000B  MOVE	R7	R3
      0x5C200200,  //  000C  MOVE	R8	R1
      0x7C140600,  //  000D  CALL	R5	3
      0x88180163,  //  000E  GETMBR	R6	R0	K99
      0x8C180D1D,  //  000F  GETMET	R6	R6	K29
      0xB8220000,  //  0010  GETNGBL	R8	K0
      0x8C20113E,  //  0011  GETMET	R8	R8	K62
      0x5C280200,  //  0012  MOVE	R10	R1
      0x5C2C0800,  //  0013  MOVE	R11	R4
      0x5C300A00,  //  0014  MOVE	R12	R5
      0x7C200800,  //  0015  CALL	R8	4
      0x7C180400,  //  0016  CALL	R6	2
      0x50180200,  //  0017  LDBOOL	R6	1	0
      0x80040C00,  //  0018  RET	1	R6
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_write_request
********************************************************************/
be_local_closure(class_Matter_IM_process_write_request,   /* name */
  be_nested_proto(
    22,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_IM,     /* shared constants */
    be_str_weak(process_write_request),
    &be_const_str_solidified,
    ( &(const binstruction[153]) {  /* code */
      0xB80E0000,  //  0000  GETNGBL	R3	K0
      0x8C0C07C0,  //  0001  GETMET	R3	R3	K192
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C0702,  //  0003  GETMET	R3	R3	K2
      0x5C140400,  //  0004  MOVE	R5	R2
      0x7C0C0400,  //  0005  CALL	R3	2
      0xB8120000,  //  0006  GETNGBL	R4	K0
      0x8C10096E,  //  0007  GETMET	R4	R4	K110
      0x7C100200,  //  0008  CALL	R4	1
      0x881407BB,  //  0009  GETMBR	R5	R3	K187
      0x881807B8,  //  000A  GETMBR	R6	R3	K184
      0x8C1C0155,  //  000B  GETMET	R7	R0	K85
      0x5C240200,  //  000C  MOVE	R9	R1
      0x5C280C00,  //  000D  MOVE	R10	R6
      0x7C1C0600,  //  000E  CALL	R7	3
      0x741E0005,  //  000F  JMPT	R7	#0016
      0x8C1C010D,  //  0010  GETMET	R7	R0	K13
      0x5C240200,  //  0011  MOVE	R9	R1
      0x542A00C8,  //  0012  LDINT	R10	201
      0x7C1C0600,  //  0013  CALL	R7	3
      0x501C0200,  //  0014  LDBOOL	R7	1	0
      0x80040E00,  //  0015  RET	1	R7
      0x881C07C1,  //  0016  GETMBR	R7	R3	K193
      0x4C200000,  //  0017  LDNIL	R8
      0x201C0E08,  //  0018  NE	R7	R7	R8
      0x781E007C,  //  0019  JMPF	R7	#0097
      0xB81E0000,  //  001A  GETNGBL	R7	K0
      0x8C1C0FC2,  //  001B  GETMET	R7	R7	K194
      0x7C1C0200,  //  001C  CALL	R7	1
      0x60200012,  //  001D  GETGBL	R8	G18
      0x7C200000,  //  001E  CALL	R8	0
      0x901E3808,  //  001F  SETMBR	R7	K28	R8
      0xB8220000,  //  0020  GETNGBL	R8	K0
      0x8C2011B5,  //  0021  GETMET	R8	R8	K181
      0x88280137,  //  0022  GETMBR	R10	R0	K55
      0x7C200400,  //  0023  CALL	R8	2
      0x60240010,  //  0024  GETGBL	R9	G16
      0x882807C1,  //  0025  GETMBR	R10	R3	K193
      0x7C240200,  //  0026  CALL	R9	1
      0xA8020061,  //  0027  EXBLK	0	#008A
      0x5C281200,  //  0028  MOVE	R10	R9
      0x7C280000,  //  0029  CALL	R10	0
      0x882C1518,  //  002A  GETMBR	R11	R10	K24
      0x883015C3,  //  002B  GETMBR	R12	R10	K195
      0x8C3409C4,  //  002C  GETMET	R13	R4	K196
      0x5C3C1600,  //  002D  MOVE	R15	R11
      0x7C340400,  //  002E  CALL	R13	2
      0x88341711,  //  002F  GETMBR	R13	R11	K17
      0x4C380000,  //  0030  LDNIL	R14
      0x1C341A0E,  //  0031  EQ	R13	R13	R14
      0x74360003,  //  0032  JMPT	R13	#0037
      0x88341712,  //  0033  GETMBR	R13	R11	K18
      0x4C380000,  //  0034  LDNIL	R14
      0x1C341A0E,  //  0035  EQ	R13	R13	R14
      0x78360007,  //  0036  JMPF	R13	#003F
      0x5436007F,  //  0037  LDINT	R13	128
      0x90122C0D,  //  0038  SETMBR	R4	K22	R13
      0x8C3401C5,  //  0039  GETMET	R13	R0	K197
      0x5C3C0E00,  //  003A  MOVE	R15	R7
      0x5C400800,  //  003B  MOVE	R16	R4
      0x4C440000,  //  003C  LDNIL	R17
      0x7C340800,  //  003D  CALL	R13	4
      0x7001FFE8,  //  003E  JMP		#0028
      0x8834171B,  //  003F  GETMBR	R13	R11	K27
      0x4C380000,  //  0040  LDNIL	R14
      0x1C341A0E,  //  0041  EQ	R13	R13	R14
      0x78360016,  //  0042  JMPF	R13	#005A
      0xB8361600,  //  0043  GETNGBL	R13	K11
      0x8C341B21,  //  0044  GETMET	R13	R13	K33
      0x583C0008,  //  0045  LDCONST	R15	K8
      0x7C340400,  //  0046  CALL	R13	2
      0x78360011,  //  0047  JMPF	R13	#005A
      0xB8360000,  //  0048  GETNGBL	R13	K0
      0x8C341B10,  //  0049  GETMET	R13	R13	K16
      0x883C1711,  //  004A  GETMBR	R15	R11	K17
      0x88401712,  //  004B  GETMBR	R16	R11	K18
      0x7C340600,  //  004C  CALL	R13	3
      0xB83A0600,  //  004D  GETNGBL	R14	K3
      0x603C0008,  //  004E  GETGBL	R15	G8
      0x5C400800,  //  004F  MOVE	R16	R4
      0x7C3C0200,  //  0050  CALL	R15	1
      0x003F8C0F,  //  0051  ADD	R15	K198	R15
      0x78360002,  //  0052  JMPF	R13	#0056
      0x0042260D,  //  0053  ADD	R16	K19	R13
      0x00402114,  //  0054  ADD	R16	R16	K20
      0x70020000,  //  0055  JMP		#0057
      0x58400015,  //  0056  LDCONST	R16	K21
      0x003C1E10,  //  0057  ADD	R15	R15	R16
      0x58400008,  //  0058  LDCONST	R16	K8
      0x7C380400,  //  0059  CALL	R14	2
      0x8C34118C,  //  005A  GETMET	R13	R8	K140
      0x883C171B,  //  005B  GETMBR	R15	R11	K27
      0x88401711,  //  005C  GETMBR	R16	R11	K17
      0x88441712,  //  005D  GETMBR	R17	R11	K18
      0x7C340800,  //  005E  CALL	R13	4
      0x8C3411C7,  //  005F  GETMET	R13	R8	K199
      0x7C340200,  //  0060  CALL	R13	1
      0x4C380000,  //  0061  LDNIL	R14
      0x8C3C11C8,  //  0062  GETMET	R15	R8	K200
      0x7C3C0200,  //  0063  CALL	R15	1
      0x5C381E00,  //  0064  MOVE	R14	R15
      0x783E0022,  //  0065  JMPF	R15	#0089
      0x903A6C01,  //  0066  SETMBR	R14	K54	R1
      0x883C1D16,  //  0067  GETMBR	R15	R14	K22
      0x4C400000,  //  0068  LDNIL	R16
      0x203C1E10,  //  0069  NE	R15	R15	R16
      0x783E0007,  //  006A  JMPF	R15	#0073
      0x4C3C0000,  //  006B  LDNIL	R15
      0x903A2C0F,  //  006C  SETMBR	R14	K22	R15
      0x8C3C01C5,  //  006D  GETMET	R15	R0	K197
      0x5C440E00,  //  006E  MOVE	R17	R7
      0x5C481C00,  //  006F  MOVE	R18	R14
      0x5C4C1800,  //  0070  MOVE	R19	R12
      0x7C3C0800,  //  0071  CALL	R15	4
      0x70020014,  //  0072  JMP		#0088
      0x8C3C11C9,  //  0073  GETMET	R15	R8	K201
      0x7C3C0200,  //  0074  CALL	R15	1
      0x54420087,  //  0075  LDINT	R16	136
      0x903A2C10,  //  0076  SETMBR	R14	K22	R16
      0x4C400000,  //  0077  LDNIL	R16
      0x20401E10,  //  0078  NE	R16	R15	R16
      0x78420005,  //  0079  JMPF	R16	#0080
      0x8C401FCA,  //  007A  GETMET	R16	R15	K202
      0x88480305,  //  007B  GETMBR	R18	R1	K5
      0x5C4C1C00,  //  007C  MOVE	R19	R14
      0x5C501800,  //  007D  MOVE	R20	R12
      0x7C400800,  //  007E  CALL	R16	4
      0x70020000,  //  007F  JMP		#0081
      0x4C400000,  //  0080  LDNIL	R16
      0x78420000,  //  0081  JMPF	R16	#0083
      0x903A2D0E,  //  0082  SETMBR	R14	K22	K14
      0x8C4401C5,  //  0083  GETMET	R17	R0	K197
      0x5C4C0E00,  //  0084  MOVE	R19	R7
      0x5C501C00,  //  0085  MOVE	R20	R14
      0x5C541800,  //  0086  MOVE	R21	R12
      0x7C440800,  //  0087  CALL	R17	4
      0x7001FFD8,  //  0088  JMP		#0062
      0x7001FF9D,  //  0089  JMP		#0028
      0x5824006F,  //  008A  LDCONST	R9	K111
      0xAC240200,  //  008B  CATCH	R9	1	0
      0xB0080000,  //  008C  RAISE	2	R0	R0
      0x5C240A00,  //  008D  MOVE	R9	R5
      0x74260007,  //  008E  JMPT	R9	#0097
      0x88240163,  //  008F  GETMBR	R9	R0	K99
      0x8C24131D,  //  0090  GETMET	R9	R9	K29
      0xB82E0000,  //  0091  GETNGBL	R11	K0
      0x8C2C17CB,  //  0092  GETMET	R11	R11	K203
      0x5C340200,  //  0093  MOVE	R13	R1
      0x5C380E00,  //  0094  MOVE	R14	R7
      0x7C2C0600,  //  0095  CALL	R11	3
      0x7C240400,  //  0096  CALL	R9	2
      0x501C0200,  //  0097  LDBOOL	R7	1	0
      0x80040E00,  //  0098  RET	1	R7
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: every_second
********************************************************************/
be_local_closure(class_Matter_IM_every_second,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_IM,     /* shared constants */
    be_str_weak(every_second),
    &be_const_str_solidified,
    ( &(const binstruction[45]) {  /* code */
      0x8C0401CC,  //  0000  GETMET	R1	R0	K204
      0x7C040200,  //  0001  CALL	R1	1
      0x6004000C,  //  0002  GETGBL	R1	G12
      0x88080109,  //  0003  GETMBR	R2	R0	K9
      0x7C040200,  //  0004  CALL	R1	1
      0x2404030E,  //  0005  GT	R1	R1	K14
      0x78060024,  //  0006  JMPF	R1	#002C
      0x60040012,  //  0007  GETGBL	R1	G18
      0x7C040000,  //  0008  CALL	R1	0
      0x60080010,  //  0009  GETGBL	R2	G16
      0x880C0109,  //  000A  GETMBR	R3	R0	K9
      0x8C0C07CD,  //  000B  GETMET	R3	R3	K205
      0x7C0C0200,  //  000C  CALL	R3	1
      0x7C080200,  //  000D  CALL	R2	1
      0xA802000B,  //  000E  EXBLK	0	#001B
      0x5C0C0400,  //  000F  MOVE	R3	R2
      0x7C0C0000,  //  0010  CALL	R3	0
      0xB8121600,  //  0011  GETNGBL	R4	K11
      0x8C1009AD,  //  0012  GETMET	R4	R4	K173
      0x88180109,  //  0013  GETMBR	R6	R0	K9
      0x94180C03,  //  0014  GETIDX	R6	R6	R3
      0x7C100400,  //  0015  CALL	R4	2
      0x78120002,  //  0016  JMPF	R4	#001A
      0x8C10031D,  //  0017  GETMET	R4	R1	K29
      0x5C180600,  //  0018  MOVE	R6	R3
      0x7C100400,  //  0019  CALL	R4	2
      0x7001FFF3,  //  001A  JMP		#000F
      0x5808006F,  //  001B  LDCONST	R2	K111
      0xAC080200,  //  001C  CATCH	R2	1	0
      0xB0080000,  //  001D  RAISE	2	R0	R0
      0x60080010,  //  001E  GETGBL	R2	G16
      0x5C0C0200,  //  001F  MOVE	R3	R1
      0x7C080200,  //  0020  CALL	R2	1
      0xA8020006,  //  0021  EXBLK	0	#0029
      0x5C0C0400,  //  0022  MOVE	R3	R2
      0x7C0C0000,  //  0023  CALL	R3	0
      0x88100109,  //  0024  GETMBR	R4	R0	K9
      0x8C1009AB,  //  0025  GETMET	R4	R4	K171
      0x5C180600,  //  0026  MOVE	R6	R3
      0x7C100400,  //  0027  CALL	R4	2
      0x7001FFF8,  //  0028  JMP		#0022
      0x5808006F,  //  0029  LDCONST	R2	K111
      0xAC080200,  //  002A  CATCH	R2	1	0
      0xB0080000,  //  002B  RAISE	2	R0	R0
      0x80000000,  //  002C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: attributestatus2raw
********************************************************************/
be_local_closure(class_Matter_IM_attributestatus2raw,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_IM,     /* shared constants */
    be_str_weak(attributestatus2raw),
    &be_const_str_solidified,
    ( &(const binstruction[47]) {  /* code */
      0x8C100341,  //  0000  GETMET	R4	R1	K65
      0x541A0014,  //  0001  LDINT	R6	21
      0x581C002C,  //  0002  LDCONST	R7	K44
      0x7C100600,  //  0003  CALL	R4	3
      0x8C100341,  //  0004  GETMET	R4	R1	K65
      0x541A34FF,  //  0005  LDINT	R6	13568
      0x541DFFFD,  //  0006  LDINT	R7	-2
      0x7C100600,  //  0007  CALL	R4	3
      0x8C1001CE,  //  0008  GETMET	R4	R0	K206
      0x5C180200,  //  0009  MOVE	R6	R1
      0x5C1C0400,  //  000A  MOVE	R7	R2
      0x5820000E,  //  000B  LDCONST	R8	K14
      0x7C100800,  //  000C  CALL	R4	4
      0x8C100341,  //  000D  GETMET	R4	R1	K65
      0x541A3500,  //  000E  LDINT	R6	13569
      0x541DFFFD,  //  000F  LDINT	R7	-2
      0x7C100600,  //  0010  CALL	R4	3
      0x88100516,  //  0011  GETMBR	R4	R2	K22
      0x541600FE,  //  0012  LDINT	R5	255
      0x18100805,  //  0013  LE	R4	R4	R5
      0x78120008,  //  0014  JMPF	R4	#001E
      0x8C100341,  //  0015  GETMET	R4	R1	K65
      0x541A23FF,  //  0016  LDINT	R6	9216
      0x541DFFFD,  //  0017  LDINT	R7	-2
      0x7C100600,  //  0018  CALL	R4	3
      0x8C100341,  //  0019  GETMET	R4	R1	K65
      0x88180516,  //  001A  GETMBR	R6	R2	K22
      0x581C002C,  //  001B  LDCONST	R7	K44
      0x7C100600,  //  001C  CALL	R4	3
      0x70020007,  //  001D  JMP		#0026
      0x8C100341,  //  001E  GETMET	R4	R1	K65
      0x541A24FF,  //  001F  LDINT	R6	9472
      0x541DFFFD,  //  0020  LDINT	R7	-2
      0x7C100600,  //  0021  CALL	R4	3
      0x8C100341,  //  0022  GETMET	R4	R1	K65
      0x88180516,  //  0023  GETMBR	R6	R2	K22
      0x581C0020,  //  0024  LDCONST	R7	K32
      0x7C100600,  //  0025  CALL	R4	3
      0x8C100341,  //  0026  GETMET	R4	R1	K65
      0x541A1817,  //  0027  LDINT	R6	6168
      0x541DFFFD,  //  0028  LDINT	R7	-2
      0x7C100600,  //  0029  CALL	R4	3
      0x8C100341,  //  002A  GETMET	R4	R1	K65
      0x541A0017,  //  002B  LDINT	R6	24
      0x581C002C,  //  002C  LDCONST	R7	K44
      0x7C100600,  //  002D  CALL	R4	3
      0x80000000,  //  002E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: send_ack_now
********************************************************************/
be_local_closure(class_Matter_IM_send_ack_now,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_IM,     /* shared constants */
    be_str_weak(send_ack_now),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0000,  //  0002  JMPF	R2	#0004
      0x80000400,  //  0003  RET	0
      0x88080305,  //  0004  GETMBR	R2	R1	K5
      0x880805A3,  //  0005  GETMBR	R2	R2	K163
      0x8C0805CF,  //  0006  GETMET	R2	R2	K207
      0x5C100200,  //  0007  MOVE	R4	R1
      0x50140000,  //  0008  LDBOOL	R5	0	0
      0x7C080600,  //  0009  CALL	R2	3
      0x80000000,  //  000A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: attributedata2raw
********************************************************************/
be_local_closure(class_Matter_IM_attributedata2raw,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    5,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_IM,     /* shared constants */
    be_str_weak(attributedata2raw),
    &be_const_str_solidified,
    ( &(const binstruction[23]) {  /* code */
      0x8C140341,  //  0000  GETMET	R5	R1	K65
      0x581C00D0,  //  0001  LDCONST	R7	K208
      0x5421FFFB,  //  0002  LDINT	R8	-4
      0x7C140600,  //  0003  CALL	R5	3
      0x8C140341,  //  0004  GETMET	R5	R1	K65
      0x581C002C,  //  0005  LDCONST	R7	K44
      0x5421FFFD,  //  0006  LDINT	R8	-2
      0x7C140600,  //  0007  CALL	R5	3
      0x8C1401CE,  //  0008  GETMET	R5	R0	K206
      0x5C1C0200,  //  0009  MOVE	R7	R1
      0x5C200400,  //  000A  MOVE	R8	R2
      0x5824002C,  //  000B  LDCONST	R9	K44
      0x5C280800,  //  000C  MOVE	R10	R4
      0x7C140A00,  //  000D  CALL	R5	5
      0x900EF720,  //  000E  SETMBR	R3	K123	K32
      0x8C14077C,  //  000F  GETMET	R5	R3	K124
      0x5C1C0200,  //  0010  MOVE	R7	R1
      0x7C140400,  //  0011  CALL	R5	2
      0x8C140341,  //  0012  GETMET	R5	R1	K65
      0x541E1817,  //  0013  LDINT	R7	6168
      0x5421FFFD,  //  0014  LDINT	R8	-2
      0x7C140600,  //  0015  CALL	R5	3
      0x80000000,  //  0016  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_IM
********************************************************************/
be_local_class(Matter_IM,
    7,
    NULL,
    be_nested_map(38,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(process_timed_request, -1), be_const_closure(class_Matter_IM_process_timed_request_closure) },
        { be_const_key_weak(write_single_attribute_status_to_bytes, -1), be_const_closure(class_Matter_IM_write_single_attribute_status_to_bytes_closure) },
        { be_const_key_weak(process_incoming, -1), be_const_closure(class_Matter_IM_process_incoming_closure) },
        { be_const_key_weak(invoke_request_solo, 4), be_const_var(4) },
        { be_const_key_weak(attributedata2raw, 10), be_const_closure(class_Matter_IM_attributedata2raw_closure) },
        { be_const_key_weak(send_ack_now, -1), be_const_closure(class_Matter_IM_send_ack_now_closure) },
        { be_const_key_weak(timed_exchanges, 2), be_const_var(6) },
        { be_const_key_weak(process_invoke_request_solo, -1), be_const_closure(class_Matter_IM_process_invoke_request_solo_closure) },
        { be_const_key_weak(send_enqueued, -1), be_const_closure(class_Matter_IM_send_enqueued_closure) },
        { be_const_key_weak(subscribe_request, 36), be_const_closure(class_Matter_IM_subscribe_request_closure) },
        { be_const_key_weak(attributestatus2raw, -1), be_const_closure(class_Matter_IM_attributestatus2raw_closure) },
        { be_const_key_weak(every_second, 32), be_const_closure(class_Matter_IM_every_second_closure) },
        { be_const_key_weak(process_read_or_subscribe_request_event_pull, -1), be_const_closure(class_Matter_IM_process_read_or_subscribe_request_event_pull_closure) },
        { be_const_key_weak(init, 12), be_const_closure(class_Matter_IM_init_closure) },
        { be_const_key_weak(parse_event_filters_min_no, -1), be_const_static_closure(class_Matter_IM_parse_event_filters_min_no_closure) },
        { be_const_key_weak(device, 6), be_const_var(0) },
        { be_const_key_weak(path2raw, -1), be_const_closure(class_Matter_IM_path2raw_closure) },
        { be_const_key_weak(send_subscribe_heartbeat, -1), be_const_closure(class_Matter_IM_send_subscribe_heartbeat_closure) },
        { be_const_key_weak(remove_sendqueue_by_exchangeid, -1), be_const_closure(class_Matter_IM_remove_sendqueue_by_exchangeid_closure) },
        { be_const_key_weak(process_incoming_ack, -1), be_const_closure(class_Matter_IM_process_incoming_ack_closure) },
        { be_const_key_weak(process_read_or_subscribe_request_pull, -1), be_const_closure(class_Matter_IM_process_read_or_subscribe_request_pull_closure) },
        { be_const_key_weak(process_read_request_solo, 17), be_const_closure(class_Matter_IM_process_read_request_solo_closure) },
        { be_const_key_weak(send_subscribe_update, -1), be_const_closure(class_Matter_IM_send_subscribe_update_closure) },
        { be_const_key_weak(check_timed_request, -1), be_const_closure(class_Matter_IM_check_timed_request_closure) },
        { be_const_key_weak(expire_sendqueue, -1), be_const_closure(class_Matter_IM_expire_sendqueue_closure) },
        { be_const_key_weak(read_single_attribute_to_bytes, -1), be_const_closure(class_Matter_IM_read_single_attribute_to_bytes_closure) },
        { be_const_key_weak(every_50ms, 20), be_const_closure(class_Matter_IM_every_50ms_closure) },
        { be_const_key_weak(process_invoke_request, -1), be_const_closure(class_Matter_IM_process_invoke_request_closure) },
        { be_const_key_weak(invokeresponse2raw, 18), be_const_closure(class_Matter_IM_invokeresponse2raw_closure) },
        { be_const_key_weak(process_read_request_pull, -1), be_const_closure(class_Matter_IM_process_read_request_pull_closure) },
        { be_const_key_weak(process_write_request, -1), be_const_closure(class_Matter_IM_process_write_request_closure) },
        { be_const_key_weak(find_sendqueue_by_exchangeid, 11), be_const_closure(class_Matter_IM_find_sendqueue_by_exchangeid_closure) },
        { be_const_key_weak(subs_shop, -1), be_const_var(1) },
        { be_const_key_weak(read_request_solo, -1), be_const_var(3) },
        { be_const_key_weak(send_status, 5), be_const_closure(class_Matter_IM_send_status_closure) },
        { be_const_key_weak(send_queue, 3), be_const_var(2) },
        { be_const_key_weak(process_status_response, -1), be_const_closure(class_Matter_IM_process_status_response_closure) },
        { be_const_key_weak(tlv_solo, -1), be_const_var(5) },
    })),
    be_str_weak(Matter_IM)
);
/********************************************************************/
/* End of solidification */
