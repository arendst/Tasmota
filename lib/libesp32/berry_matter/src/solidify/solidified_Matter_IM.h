/* Solidification of Matter_IM.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
extern const bclass be_class_Matter_IM;
// compact class 'Matter_IM' ktab size: 213, total: 544 (saved 2648 bytes)
static const bvalue be_ktab_class_Matter_IM[213] = {
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
  /* K14  */  be_nested_str_weak(SUCCESS),
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
  /* K31  */  be_const_int(0),
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
  /* K54  */  be_nested_str_weak(INVALID_ACTION),
  /* K55  */  be_nested_str_weak(msg),
  /* K56  */  be_nested_str_weak(device),
  /* K57  */  be_nested_str_weak(resolve_attribute_read_solo),
  /* K58  */  be_nested_str_weak(UNSUPPORTED_ATTRIBUTE),
  /* K59  */  be_nested_str_weak(read_attribute),
  /* K60  */  be_nested_str_weak(tlv_solo),
  /* K61  */  be_nested_str_weak(is_list),
  /* K62  */  be_nested_str_weak(is_array),
  /* K63  */  be_nested_str_weak(encode_len),
  /* K64  */  be_nested_str_weak(IM_ReportData_Pull),
  /* K65  */  be_nested_str_weak(MAX_MESSAGE),
  /* K66  */  be_nested_str_weak(MTR_X3A_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20Response_X20to_X20big_X2C_X20revert_X20to_X20non_X2Dsolo),
  /* K67  */  be_nested_str_weak(add),
  /* K68  */  be_nested_str_weak(attributedata2raw),
  /* K69  */  be_const_int(405077772),
  /* K70  */  be_nested_str_weak(attributestatus2raw),
  /* K71  */  be_nested_str_weak(MTR_X3A_X20_X3ERead_Attr_X20_X28_X256i_X29_X20_X25s_X20_X2D_X20IGNORED),
  /* K72  */  be_nested_str_weak(build_response),
  /* K73  */  be_nested_str_weak(message_handler),
  /* K74  */  be_nested_str_weak(clear),
  /* K75  */  be_nested_str_weak(encode_frame),
  /* K76  */  be_nested_str_weak(encrypt),
  /* K77  */  be_nested_str_weak(MTR_X3A_X20_X3Csnd_X20_X20_X20_X20_X20_X20_X20_X28_X256i_X29_X20id_X3D_X25i_X20exch_X3D_X25i_X20rack_X3D_X25s),
  /* K78  */  be_nested_str_weak(message_counter),
  /* K79  */  be_nested_str_weak(ack_message_counter),
  /* K80  */  be_nested_str_weak(send_response_frame),
  /* K81  */  be_nested_str_weak(to_str_val),
  /* K82  */  be_nested_str_weak(MTR_X3A_X20_X3ERead_Attr1_X28_X256i_X29_X20_X25s_X25s_X20_X2D_X20_X25s),
  /* K83  */  be_nested_str_weak(MTR_X3A_X20_X3ERead_Attr1_X28_X256i_X29_X20_X25s_X25s_X20_X2D_X20STATUS_X3A_X200x_X2502X_X20_X25s),
  /* K84  */  be_nested_str_weak(MTR_X3A_X20_X3ERead_Attr1_X28_X256i_X29_X20_X25s_X25s_X20_X2D_X20IGNORED),
  /* K85  */  be_nested_str_weak(TimedRequest),
  /* K86  */  be_nested_str_weak(check_timed_request),
  /* K87  */  be_nested_str_weak(TIMED_REQUEST_MISMATCH),
  /* K88  */  be_nested_str_weak(UNSUPPORTED_COMMAND),
  /* K89  */  be_nested_str_weak(get_command_name),
  /* K90  */  be_nested_str_weak(command),
  /* K91  */  be_nested_str_weak(invoke_request),
  /* K92  */  be_nested_str_weak(command_fields),
  /* K93  */  be_nested_str_weak(_X28),
  /* K94  */  be_nested_str_weak(_X29_X20),
  /* K95  */  be_nested_str_weak(MTR_X3A_X20_X3ECommand1_X20_X20_X28_X256i_X29_X20_X25s_X20_X25s_X20_X25s),
  /* K96  */  be_const_int(354943030),
  /* K97  */  be_nested_str_weak(invokeresponse2raw),
  /* K98  */  be_nested_str_weak(MTR_X3A_X20_X3CReplied_X20_X20_X20_X28_X256i_X29_X20OK_X20exch_X3D_X25i),
  /* K99  */  be_nested_str_weak(MTR_X3A_X20_X3CReplied_X20_X20_X20_X28_X256i_X29_X20_X25s_X20_X25s),
  /* K100 */  be_nested_str_weak(MTR_X3A_X20_X3CReplied_X20_X20_X20_X28_X256i_X29_X20Status_X3D0x_X2502X_X20exch_X3D_X25i),
  /* K101 */  be_nested_str_weak(MTR_X3A_X20_Ignore_X20_X20_X20_X20_X28_X256i_X29_X20exch_X3D_X25i),
  /* K102 */  be_nested_str_weak(send_queue),
  /* K103 */  be_nested_str_weak(finished),
  /* K104 */  be_nested_str_weak(ready),
  /* K105 */  be_nested_str_weak(send_im),
  /* K106 */  be_nested_str_weak(remove_sendqueue_by_exchangeid),
  /* K107 */  be_nested_str_weak(resp),
  /* K108 */  be_nested_str_weak(SubscribeRequestMessage),
  /* K109 */  be_nested_str_weak(keep_subscriptions),
  /* K110 */  be_nested_str_weak(remove_by_session),
  /* K111 */  be_nested_str_weak(new_subscription),
  /* K112 */  be_nested_str_weak(attributes_requests),
  /* K113 */  be_nested_str_weak(Path),
  /* K114 */  be_nested_str_weak(stop_iteration),
  /* K115 */  be_nested_str_weak(MTR_X3A_X20_X3ESubscribe_X20_X28_X256i_X29_X20_X25s_X20_X28min_X3D_X25i_X2C_X20max_X3D_X25i_X2C_X20keep_X3D_X25i_X29_X20sub_X3D_X25i_X20fabric_filtered_X3D_X25s),
  /* K116 */  be_nested_str_weak(concat),
  /* K117 */  be_nested_str_weak(_X20),
  /* K118 */  be_nested_str_weak(min_interval),
  /* K119 */  be_nested_str_weak(max_interval),
  /* K120 */  be_nested_str_weak(subscription_id),
  /* K121 */  be_nested_str_weak(fabric_filtered),
  /* K122 */  be_nested_str_weak(process_read_or_subscribe_request_pull),
  /* K123 */  be_nested_str_weak(process_read_or_subscribe_request_event_pull),
  /* K124 */  be_nested_str_weak(set_event_generator_or_arr),
  /* K125 */  be_nested_str_weak(IM_SubscribeResponse_Pull),
  /* K126 */  be_nested_str_weak(tag_sub),
  /* K127 */  be_nested_str_weak(tlv2raw),
  /* K128 */  be_nested_str_weak(IM_Subscription_Shop),
  /* K129 */  be_nested_str_weak(ReadRequestMessage_solo),
  /* K130 */  be_nested_str_weak(InvokeRequestMessage_solo),
  /* K131 */  be_nested_str_weak(Matter_TLV_item),
  /* K132 */  be_nested_str_weak(findsubval),
  /* K133 */  be_nested_str_weak(find_sendqueue_by_exchangeid),
  /* K134 */  be_nested_str_weak(status_ok_received),
  /* K135 */  be_nested_str_weak(MTR_X3A_X20_X3EOK_X20_X20_X20_X20_X20_X20_X20_X20_X28_X256i_X29_X20exch_X3D_X25i_X20not_X20found),
  /* K136 */  be_nested_str_weak(MTR_X3A_X20_X3EStatus_X20_X20_X20_X20ERROR_X20_X3D_X200x_X2502X),
  /* K137 */  be_nested_str_weak(status_error_received),
  /* K138 */  be_nested_str_weak(event_requests),
  /* K139 */  be_nested_str_weak(get_node_id),
  /* K140 */  be_nested_str_weak(parse_event_filters_min_no),
  /* K141 */  be_nested_str_weak(event_filters),
  /* K142 */  be_nested_str_weak(EventGenerator),
  /* K143 */  be_nested_str_weak(start),
  /* K144 */  be_nested_str_weak(event),
  /* K145 */  be_nested_str_weak(get_event_name),
  /* K146 */  be_nested_str_weak(_X2502X),
  /* K147 */  be_nested_str_weak(_X2A_X2A),
  /* K148 */  be_nested_str_weak(_X2504X),
  /* K149 */  be_nested_str_weak(_X2A_X2A_X2A_X2A),
  /* K150 */  be_nested_str_weak(_X20_X28event_X3E_X25s_X29),
  /* K151 */  be_nested_str_weak(MTR_X3A_X20_X3ERead_Event_X28_X256i_X29_X20_X5B_X25s_X5D_X25s_X2F_X25s_X20_X25s_X25s),
  /* K152 */  be_const_class(be_class_Matter_IM),
  /* K153 */  be_nested_str_weak(int64),
  /* K154 */  be_nested_str_weak(toint64),
  /* K155 */  be_nested_str_weak(node),
  /* K156 */  be_nested_str_weak(tobytes),
  /* K157 */  be_nested_str_weak(MTR_X3A_X20node_id_X20filter_X20_X25s_X20doesn_X27t_X20match_X20_X25s),
  /* K158 */  be_nested_str_weak(tohex),
  /* K159 */  be_nested_str_weak(event_min),
  /* K160 */  be_nested_str_weak(get_exchangeid),
  /* K161 */  be_nested_str_weak(ack_received),
  /* K162 */  be_nested_str_weak(IM_Status),
  /* K163 */  be_nested_str_weak(MTR_X3A_X20_X3CSub_Alive_X20_X28_X256i_X29_X20sub_X3D_X25s),
  /* K164 */  be_nested_str_weak(is_keep_alive),
  /* K165 */  be_nested_str_weak(IM_SubscribedHeartbeat),
  /* K166 */  be_nested_str_weak(_message_handler),
  /* K167 */  be_nested_str_weak(send_enqueued),
  /* K168 */  be_nested_str_weak(ReadRequestMessage),
  /* K169 */  be_nested_str_weak(updates),
  /* K170 */  be_nested_str_weak(MTR_X3A_X20_X3CSub_Data_X20_X20_X28_X256i_X29_X20sub_X3D_X25i),
  /* K171 */  be_nested_str_weak(update_event_generator_array),
  /* K172 */  be_nested_str_weak(IM_ReportDataSubscribed_Pull),
  /* K173 */  be_nested_str_weak(find),
  /* K174 */  be_nested_str_weak(remove),
  /* K175 */  be_nested_str_weak(MTR_X3A_X20_X3ETimed_X20_X20_X20_X20_X20_X28_X256i_X29_X20TIMED_REQUEST_MISMATCH_X20no_X20prior_X20TimedRequest),
  /* K176 */  be_nested_str_weak(time_reached),
  /* K177 */  be_nested_str_weak(MTR_X3A_X20_X3ETimed_X20_X20_X20_X20_X20_X28_X256i_X29_X20TIMED_REQUEST_MISMATCH_X20timeout_X20expired),
  /* K178 */  be_nested_str_weak(expiration),
  /* K179 */  be_nested_str_weak(reached_timeout),
  /* K180 */  be_nested_str_weak(Matter_TLV_array),
  /* K181 */  be_nested_str_weak(val),
  /* K182 */  be_nested_str_weak(MTR_X3A_X20_X3ERead_Attr_X20_X28_X256i_X29_X20_X25s_X25s_X20_X2D_X20_X25s),
  /* K183 */  be_nested_str_weak(MTR_X3A_X20_X3ERead_Attr_X20_X28_X256i_X29_X20_X25s_X25s_X20_X2D_X20STATUS_X3A_X200x_X2502X_X20_X25s),
  /* K184 */  be_nested_str_weak(PathGenerator),
  /* K185 */  be_nested_str_weak(MTR_X3A_X20_X3ERead_Attr_X20_X28_X256i_X29_X20_X25s),
  /* K186 */  be_nested_str_weak(InvokeRequestMessage),
  /* K187 */  be_nested_str_weak(timed_request),
  /* K188 */  be_nested_str_weak(invoke_requests),
  /* K189 */  be_nested_str_weak(InvokeResponseMessage),
  /* K190 */  be_nested_str_weak(suppress_response),
  /* K191 */  be_nested_str_weak(invoke_responses),
  /* K192 */  be_nested_str_weak(command_path),
  /* K193 */  be_nested_str_weak(MTR_X3A_X20_X3ECommand_X20_X20_X20_X28_X256i_X29_X20_X25s_X20_X25s_X20_X25s),
  /* K194 */  be_nested_str_weak(IM_InvokeResponse),
  /* K195 */  be_nested_str_weak(WriteRequestMessage),
  /* K196 */  be_nested_str_weak(write_requests),
  /* K197 */  be_nested_str_weak(WriteResponseMessage),
  /* K198 */  be_nested_str_weak(data),
  /* K199 */  be_nested_str_weak(copy),
  /* K200 */  be_nested_str_weak(write_single_attribute_status_to_bytes),
  /* K201 */  be_nested_str_weak(MTR_X3A_X20Write_Attr_X20),
  /* K202 */  be_nested_str_weak(is_direct),
  /* K203 */  be_nested_str_weak(next_attribute),
  /* K204 */  be_nested_str_weak(get_pi),
  /* K205 */  be_nested_str_weak(UNSUPPORTED_WRITE),
  /* K206 */  be_nested_str_weak(write_attribute),
  /* K207 */  be_nested_str_weak(IM_WriteResponse),
  /* K208 */  be_nested_str_weak(expire_sendqueue),
  /* K209 */  be_nested_str_weak(keys),
  /* K210 */  be_nested_str_weak(path2raw),
  /* K211 */  be_nested_str_weak(send_encrypted_ack),
  /* K212 */  be_const_int(355795236),
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
    ( &(const binstruction[30]) {  /* code */
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
      0xB81E0000,  //  0019  GETNGBL	R7	K0
      0x881C0F0E,  //  001A  GETMBR	R7	R7	K14
      0x7C100600,  //  001B  CALL	R4	3
      0x50100200,  //  001C  LDBOOL	R4	1	0
      0x80040800,  //  001D  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: write_single_attribute_status_to_bytes
********************************************************************/
be_local_closure(class_Matter_IM_write_single_attribute_status_to_bytes,   /* name */
  be_nested_proto(
    16,                          /* nstack */
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
    ( &(const binstruction[86]) {  /* code */
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
      0x781A0034,  //  0010  JMPF	R6	#0046
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
      0xB83E0000,  //  0036  GETNGBL	R15	K0
      0x883C1F0E,  //  0037  GETMBR	R15	R15	K14
      0x1C381C0F,  //  0038  EQ	R14	R14	R15
      0x783A0001,  //  0039  JMPF	R14	#003C
      0x5838000E,  //  003A  LDCONST	R14	K14
      0x70020000,  //  003B  JMP		#003D
      0x58380015,  //  003C  LDCONST	R14	K21
      0x7C200C00,  //  003D  CALL	R8	6
      0x8824051B,  //  003E  GETMBR	R9	R2	K27
      0x2024131F,  //  003F  NE	R9	R9	K31
      0x78260001,  //  0040  JMPF	R9	#0043
      0x58240020,  //  0041  LDCONST	R9	K32
      0x70020000,  //  0042  JMP		#0044
      0x58240008,  //  0043  LDCONST	R9	K8
      0x7C1C0400,  //  0044  CALL	R7	2
      0x7002000E,  //  0045  JMP		#0055
      0xB81A1600,  //  0046  GETNGBL	R6	K11
      0x8C180D21,  //  0047  GETMET	R6	R6	K33
      0x58200008,  //  0048  LDCONST	R8	K8
      0x7C180400,  //  0049  CALL	R6	2
      0x781A0009,  //  004A  JMPF	R6	#0055
      0xB81A0600,  //  004B  GETNGBL	R6	K3
      0x601C0018,  //  004C  GETGBL	R7	G24
      0x58200022,  //  004D  LDCONST	R8	K34
      0x60240008,  //  004E  GETGBL	R9	G8
      0x5C280400,  //  004F  MOVE	R10	R2
      0x7C240200,  //  0050  CALL	R9	1
      0x5C280A00,  //  0051  MOVE	R10	R5
      0x7C1C0600,  //  0052  CALL	R7	3
      0x58200008,  //  0053  LDCONST	R8	K8
      0x7C180400,  //  0054  CALL	R6	2
      0x80000000,  //  0055  RET	0
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
    ( &(const binstruction[242]) {  /* code */
      0xB80E0000,  //  0000  GETNGBL	R3	K0
      0x880C0736,  //  0001  GETMBR	R3	R3	K54
      0x900A2C03,  //  0002  SETMBR	R2	K22	R3
      0x900A6E01,  //  0003  SETMBR	R2	K55	R1
      0x880C0138,  //  0004  GETMBR	R3	R0	K56
      0x8C0C0739,  //  0005  GETMET	R3	R3	K57
      0x5C140400,  //  0006  MOVE	R5	R2
      0x7C0C0400,  //  0007  CALL	R3	2
      0x4C100000,  //  0008  LDNIL	R4
      0x4C140000,  //  0009  LDNIL	R5
      0x4C180000,  //  000A  LDNIL	R6
      0x20180606,  //  000B  NE	R6	R3	R6
      0x781A0008,  //  000C  JMPF	R6	#0016
      0xB81A0000,  //  000D  GETNGBL	R6	K0
      0x88180D3A,  //  000E  GETMBR	R6	R6	K58
      0x900A2C06,  //  000F  SETMBR	R2	K22	R6
      0x8C18073B,  //  0010  GETMET	R6	R3	K59
      0x88200305,  //  0011  GETMBR	R8	R1	K5
      0x5C240400,  //  0012  MOVE	R9	R2
      0x8828013C,  //  0013  GETMBR	R10	R0	K60
      0x7C180800,  //  0014  CALL	R6	4
      0x5C100C00,  //  0015  MOVE	R4	R6
      0x4C180000,  //  0016  LDNIL	R6
      0x20180806,  //  0017  NE	R6	R4	R6
      0x781A0036,  //  0018  JMPF	R6	#0050
      0x8818093D,  //  0019  GETMBR	R6	R4	K61
      0x741A0001,  //  001A  JMPT	R6	#001D
      0x8818093E,  //  001B  GETMBR	R6	R4	K62
      0x781A0018,  //  001C  JMPF	R6	#0036
      0x8C18093F,  //  001D  GETMET	R6	R4	K63
      0x7C180200,  //  001E  CALL	R6	1
      0xB81E0000,  //  001F  GETNGBL	R7	K0
      0x881C0F40,  //  0020  GETMBR	R7	R7	K64
      0x881C0F41,  //  0021  GETMBR	R7	R7	K65
      0x24180C07,  //  0022  GT	R6	R6	R7
      0x781A0011,  //  0023  JMPF	R6	#0036
      0x4C100000,  //  0024  LDNIL	R4
      0xB81A0600,  //  0025  GETNGBL	R6	K3
      0x601C0018,  //  0026  GETGBL	R7	G24
      0x58200042,  //  0027  LDCONST	R8	K66
      0x7C1C0200,  //  0028  CALL	R7	1
      0x58200008,  //  0029  LDCONST	R8	K8
      0x7C180400,  //  002A  CALL	R6	2
      0xB81A0000,  //  002B  GETNGBL	R6	K0
      0x88180D0F,  //  002C  GETMBR	R6	R6	K15
      0x8C180D2B,  //  002D  GETMET	R6	R6	K43
      0x88200326,  //  002E  GETMBR	R8	R1	K38
      0x88240327,  //  002F  GETMBR	R9	R1	K39
      0x7C180600,  //  0030  CALL	R6	3
      0x8C1C012F,  //  0031  GETMET	R7	R0	K47
      0x5C240200,  //  0032  MOVE	R9	R1
      0x5C280C00,  //  0033  MOVE	R10	R6
      0x7C1C0600,  //  0034  CALL	R7	3
      0x80040E00,  //  0035  RET	1	R7
      0x60180015,  //  0036  GETGBL	R6	G21
      0x541E002F,  //  0037  LDINT	R7	48
      0x7C180200,  //  0038  CALL	R6	1
      0x5C140C00,  //  0039  MOVE	R5	R6
      0x8C180B43,  //  003A  GETMET	R6	R5	K67
      0x54220014,  //  003B  LDINT	R8	21
      0x5824002C,  //  003C  LDCONST	R9	K44
      0x7C180600,  //  003D  CALL	R6	3
      0x8C180B43,  //  003E  GETMET	R6	R5	K67
      0x54223600,  //  003F  LDINT	R8	13825
      0x5425FFFD,  //  0040  LDINT	R9	-2
      0x7C180600,  //  0041  CALL	R6	3
      0x8C180144,  //  0042  GETMET	R6	R0	K68
      0x5C200A00,  //  0043  MOVE	R8	R5
      0x5C240400,  //  0044  MOVE	R9	R2
      0x5C280800,  //  0045  MOVE	R10	R4
      0x7C180800,  //  0046  CALL	R6	4
      0x8C180B43,  //  0047  GETMET	R6	R5	K67
      0x58200045,  //  0048  LDCONST	R8	K69
      0x5425FFFB,  //  0049  LDINT	R9	-4
      0x7C180600,  //  004A  CALL	R6	3
      0x8C180B43,  //  004B  GETMET	R6	R5	K67
      0x54220017,  //  004C  LDINT	R8	24
      0x5824002C,  //  004D  LDCONST	R9	K44
      0x7C180600,  //  004E  CALL	R6	3
      0x70020028,  //  004F  JMP		#0079
      0x88180516,  //  0050  GETMBR	R6	R2	K22
      0x4C1C0000,  //  0051  LDNIL	R7
      0x20180C07,  //  0052  NE	R6	R6	R7
      0x781A0019,  //  0053  JMPF	R6	#006E
      0x60180015,  //  0054  GETGBL	R6	G21
      0x541E002F,  //  0055  LDINT	R7	48
      0x7C180200,  //  0056  CALL	R6	1
      0x5C140C00,  //  0057  MOVE	R5	R6
      0x8C180B43,  //  0058  GETMET	R6	R5	K67
      0x54220014,  //  0059  LDINT	R8	21
      0x5824002C,  //  005A  LDCONST	R9	K44
      0x7C180600,  //  005B  CALL	R6	3
      0x8C180B43,  //  005C  GETMET	R6	R5	K67
      0x54223600,  //  005D  LDINT	R8	13825
      0x5425FFFD,  //  005E  LDINT	R9	-2
      0x7C180600,  //  005F  CALL	R6	3
      0x8C180146,  //  0060  GETMET	R6	R0	K70
      0x5C200A00,  //  0061  MOVE	R8	R5
      0x5C240400,  //  0062  MOVE	R9	R2
      0x88280516,  //  0063  GETMBR	R10	R2	K22
      0x7C180800,  //  0064  CALL	R6	4
      0x8C180B43,  //  0065  GETMET	R6	R5	K67
      0x58200045,  //  0066  LDCONST	R8	K69
      0x5425FFFB,  //  0067  LDINT	R9	-4
      0x7C180600,  //  0068  CALL	R6	3
      0x8C180B43,  //  0069  GETMET	R6	R5	K67
      0x54220017,  //  006A  LDINT	R8	24
      0x5824002C,  //  006B  LDCONST	R9	K44
      0x7C180600,  //  006C  CALL	R6	3
      0x7002000A,  //  006D  JMP		#0079
      0xB81A0600,  //  006E  GETNGBL	R6	K3
      0x601C0018,  //  006F  GETGBL	R7	G24
      0x58200047,  //  0070  LDCONST	R8	K71
      0x88240305,  //  0071  GETMBR	R9	R1	K5
      0x88241306,  //  0072  GETMBR	R9	R9	K6
      0x5C280400,  //  0073  MOVE	R10	R2
      0x7C1C0600,  //  0074  CALL	R7	3
      0x58200008,  //  0075  LDCONST	R8	K8
      0x7C180400,  //  0076  CALL	R6	2
      0x50180000,  //  0077  LDBOOL	R6	0	0
      0x80040C00,  //  0078  RET	1	R6
      0x8C180348,  //  0079  GETMET	R6	R1	K72
      0x54220004,  //  007A  LDINT	R8	5
      0x50240200,  //  007B  LDBOOL	R9	1	0
      0x7C180600,  //  007C  CALL	R6	3
      0x881C0138,  //  007D  GETMBR	R7	R0	K56
      0x881C0F49,  //  007E  GETMBR	R7	R7	K73
      0x88200326,  //  007F  GETMBR	R8	R1	K38
      0x8C24114A,  //  0080  GETMET	R9	R8	K74
      0x7C240200,  //  0081  CALL	R9	1
      0x8C240D4B,  //  0082  GETMET	R9	R6	K75
      0x5C2C0A00,  //  0083  MOVE	R11	R5
      0x5C301000,  //  0084  MOVE	R12	R8
      0x7C240600,  //  0085  CALL	R9	3
      0x8C240D4C,  //  0086  GETMET	R9	R6	K76
      0x7C240200,  //  0087  CALL	R9	1
      0xB8261600,  //  0088  GETNGBL	R9	K11
      0x8C241321,  //  0089  GETMET	R9	R9	K33
      0x542E0003,  //  008A  LDINT	R11	4
      0x7C240400,  //  008B  CALL	R9	2
      0x7826000A,  //  008C  JMPF	R9	#0098
      0xB8260600,  //  008D  GETNGBL	R9	K3
      0x60280018,  //  008E  GETGBL	R10	G24
      0x582C004D,  //  008F  LDCONST	R11	K77
      0x88300D05,  //  0090  GETMBR	R12	R6	K5
      0x88301906,  //  0091  GETMBR	R12	R12	K6
      0x88340D4E,  //  0092  GETMBR	R13	R6	K78
      0x88380D0A,  //  0093  GETMBR	R14	R6	K10
      0x883C0D4F,  //  0094  GETMBR	R15	R6	K79
      0x7C280A00,  //  0095  CALL	R10	5
      0x542E0003,  //  0096  LDINT	R11	4
      0x7C240400,  //  0097  CALL	R9	2
      0x8C240F50,  //  0098  GETMET	R9	R7	K80
      0x5C2C0C00,  //  0099  MOVE	R11	R6
      0x7C240400,  //  009A  CALL	R9	2
      0x4C240000,  //  009B  LDNIL	R9
      0xB82A1600,  //  009C  GETNGBL	R10	K11
      0x8C281521,  //  009D  GETMET	R10	R10	K33
      0x58300008,  //  009E  LDCONST	R12	K8
      0x7C280400,  //  009F  CALL	R10	2
      0x782A000B,  //  00A0  JMPF	R10	#00AD
      0xB82A0000,  //  00A1  GETNGBL	R10	K0
      0x8C281510,  //  00A2  GETMET	R10	R10	K16
      0x88300511,  //  00A3  GETMBR	R12	R2	K17
      0x88340512,  //  00A4  GETMBR	R13	R2	K18
      0x7C280600,  //  00A5  CALL	R10	3
      0x5C241400,  //  00A6  MOVE	R9	R10
      0x78260002,  //  00A7  JMPF	R9	#00AB
      0x002A2609,  //  00A8  ADD	R10	K19	R9
      0x00281514,  //  00A9  ADD	R10	R10	K20
      0x70020000,  //  00AA  JMP		#00AC
      0x58280015,  //  00AB  LDCONST	R10	K21
      0x5C241400,  //  00AC  MOVE	R9	R10
      0x4C280000,  //  00AD  LDNIL	R10
      0x2028080A,  //  00AE  NE	R10	R4	R10
      0x782A0012,  //  00AF  JMPF	R10	#00C3
      0xB82A1600,  //  00B0  GETNGBL	R10	K11
      0x8C281521,  //  00B1  GETMET	R10	R10	K33
      0x58300008,  //  00B2  LDCONST	R12	K8
      0x7C280400,  //  00B3  CALL	R10	2
      0x782A000C,  //  00B4  JMPF	R10	#00C2
      0x8C280951,  //  00B5  GETMET	R10	R4	K81
      0x7C280200,  //  00B6  CALL	R10	1
      0xB82E0600,  //  00B7  GETNGBL	R11	K3
      0x60300018,  //  00B8  GETGBL	R12	G24
      0x58340052,  //  00B9  LDCONST	R13	K82
      0x88380305,  //  00BA  GETMBR	R14	R1	K5
      0x88381D06,  //  00BB  GETMBR	R14	R14	K6
      0x5C3C0400,  //  00BC  MOVE	R15	R2
      0x5C401200,  //  00BD  MOVE	R16	R9
      0x5C441400,  //  00BE  MOVE	R17	R10
      0x7C300A00,  //  00BF  CALL	R12	5
      0x58340008,  //  00C0  LDCONST	R13	K8
      0x7C2C0400,  //  00C1  CALL	R11	2
      0x7002002C,  //  00C2  JMP		#00F0
      0x88280516,  //  00C3  GETMBR	R10	R2	K22
      0x4C2C0000,  //  00C4  LDNIL	R11
      0x2028140B,  //  00C5  NE	R10	R10	R11
      0x782A0019,  //  00C6  JMPF	R10	#00E1
      0x88280516,  //  00C7  GETMBR	R10	R2	K22
      0xB82E0000,  //  00C8  GETNGBL	R11	K0
      0x882C173A,  //  00C9  GETMBR	R11	R11	K58
      0x1C28140B,  //  00CA  EQ	R10	R10	R11
      0x782A0001,  //  00CB  JMPF	R10	#00CE
      0x5828003A,  //  00CC  LDCONST	R10	K58
      0x70020000,  //  00CD  JMP		#00CF
      0x58280015,  //  00CE  LDCONST	R10	K21
      0xB82E1600,  //  00CF  GETNGBL	R11	K11
      0x8C2C1721,  //  00D0  GETMET	R11	R11	K33
      0x58340008,  //  00D1  LDCONST	R13	K8
      0x7C2C0400,  //  00D2  CALL	R11	2
      0x782E000B,  //  00D3  JMPF	R11	#00E0
      0xB82E0600,  //  00D4  GETNGBL	R11	K3
      0x60300018,  //  00D5  GETGBL	R12	G24
      0x58340053,  //  00D6  LDCONST	R13	K83
      0x88380305,  //  00D7  GETMBR	R14	R1	K5
      0x88381D06,  //  00D8  GETMBR	R14	R14	K6
      0x5C3C0400,  //  00D9  MOVE	R15	R2
      0x5C401200,  //  00DA  MOVE	R16	R9
      0x88440516,  //  00DB  GETMBR	R17	R2	K22
      0x5C481400,  //  00DC  MOVE	R18	R10
      0x7C300C00,  //  00DD  CALL	R12	6
      0x58340008,  //  00DE  LDCONST	R13	K8
      0x7C2C0400,  //  00DF  CALL	R11	2
      0x7002000E,  //  00E0  JMP		#00F0
      0xB82A1600,  //  00E1  GETNGBL	R10	K11
      0x8C281521,  //  00E2  GETMET	R10	R10	K33
      0x58300008,  //  00E3  LDCONST	R12	K8
      0x7C280400,  //  00E4  CALL	R10	2
      0x782A0009,  //  00E5  JMPF	R10	#00F0
      0xB82A0600,  //  00E6  GETNGBL	R10	K3
      0x602C0018,  //  00E7  GETGBL	R11	G24
      0x58300054,  //  00E8  LDCONST	R12	K84
      0x88340305,  //  00E9  GETMBR	R13	R1	K5
      0x88341B06,  //  00EA  GETMBR	R13	R13	K6
      0x5C380400,  //  00EB  MOVE	R14	R2
      0x5C3C1200,  //  00EC  MOVE	R15	R9
      0x7C2C0800,  //  00ED  CALL	R11	4
      0x58300008,  //  00EE  LDCONST	R12	K8
      0x7C280400,  //  00EF  CALL	R10	2
      0x50280200,  //  00F0  LDBOOL	R10	1	0
      0x80041400,  //  00F1  RET	1	R10
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
    ( &(const binstruction[202]) {  /* code */
      0x900A6E01,  //  0000  SETMBR	R2	K55	R1
      0x880C0555,  //  0001  GETMBR	R3	R2	K85
      0x780E000B,  //  0002  JMPF	R3	#000F
      0x8C0C0156,  //  0003  GETMET	R3	R0	K86
      0x5C140200,  //  0004  MOVE	R5	R1
      0x50180200,  //  0005  LDBOOL	R6	1	0
      0x7C0C0600,  //  0006  CALL	R3	3
      0x740E0006,  //  0007  JMPT	R3	#000F
      0x8C0C010D,  //  0008  GETMET	R3	R0	K13
      0x5C140200,  //  0009  MOVE	R5	R1
      0xB81A0000,  //  000A  GETNGBL	R6	K0
      0x88180D57,  //  000B  GETMBR	R6	R6	K87
      0x7C0C0600,  //  000C  CALL	R3	3
      0x500C0200,  //  000D  LDBOOL	R3	1	0
      0x80040600,  //  000E  RET	1	R3
      0xB80E0000,  //  000F  GETNGBL	R3	K0
      0x880C0758,  //  0010  GETMBR	R3	R3	K88
      0x900A2C03,  //  0011  SETMBR	R2	K22	R3
      0xB80E0000,  //  0012  GETNGBL	R3	K0
      0x8C0C0759,  //  0013  GETMET	R3	R3	K89
      0x88140511,  //  0014  GETMBR	R5	R2	K17
      0x8818055A,  //  0015  GETMBR	R6	R2	K90
      0x7C0C0600,  //  0016  CALL	R3	3
      0x60100008,  //  0017  GETGBL	R4	G8
      0x5C140400,  //  0018  MOVE	R5	R2
      0x7C100200,  //  0019  CALL	R4	1
      0x88140138,  //  001A  GETMBR	R5	R0	K56
      0x8C140B5B,  //  001B  GETMET	R5	R5	K91
      0x881C0305,  //  001C  GETMBR	R7	R1	K5
      0x8820055C,  //  001D  GETMBR	R8	R2	K92
      0x5C240400,  //  001E  MOVE	R9	R2
      0x7C140800,  //  001F  CALL	R5	4
      0x88180503,  //  0020  GETMBR	R6	R2	K3
      0x4C1C0000,  //  0021  LDNIL	R7
      0x20180C07,  //  0022  NE	R6	R6	R7
      0x781A0005,  //  0023  JMPF	R6	#002A
      0x60180008,  //  0024  GETGBL	R6	G8
      0x881C0503,  //  0025  GETMBR	R7	R2	K3
      0x7C180200,  //  0026  CALL	R6	1
      0x001ABA06,  //  0027  ADD	R6	K93	R6
      0x00180D5E,  //  0028  ADD	R6	R6	K94
      0x70020000,  //  0029  JMP		#002B
      0x58180015,  //  002A  LDCONST	R6	K21
      0xB81E1600,  //  002B  GETNGBL	R7	K11
      0x8C1C0F21,  //  002C  GETMET	R7	R7	K33
      0x58240008,  //  002D  LDCONST	R9	K8
      0x7C1C0400,  //  002E  CALL	R7	2
      0x781E000D,  //  002F  JMPF	R7	#003E
      0xB81E0600,  //  0030  GETNGBL	R7	K3
      0x60200018,  //  0031  GETGBL	R8	G24
      0x5824005F,  //  0032  LDCONST	R9	K95
      0x88280305,  //  0033  GETMBR	R10	R1	K5
      0x88281506,  //  0034  GETMBR	R10	R10	K6
      0x5C2C0800,  //  0035  MOVE	R11	R4
      0x780E0001,  //  0036  JMPF	R3	#0039
      0x5C300600,  //  0037  MOVE	R12	R3
      0x70020000,  //  0038  JMP		#003A
      0x58300015,  //  0039  LDCONST	R12	K21
      0x5C340C00,  //  003A  MOVE	R13	R6
      0x7C200A00,  //  003B  CALL	R8	5
      0x58240008,  //  003C  LDCONST	R9	K8
      0x7C1C0400,  //  003D  CALL	R7	2
      0x4C1C0000,  //  003E  LDNIL	R7
      0x900A0607,  //  003F  SETMBR	R2	K3	R7
      0x601C0015,  //  0040  GETGBL	R7	G21
      0x5422002F,  //  0041  LDINT	R8	48
      0x7C1C0200,  //  0042  CALL	R7	1
      0x8C200F43,  //  0043  GETMET	R8	R7	K67
      0x58280060,  //  0044  LDCONST	R10	K96
      0x542DFFFB,  //  0045  LDINT	R11	-4
      0x7C200600,  //  0046  CALL	R8	3
      0x8C200F43,  //  0047  GETMET	R8	R7	K67
      0x5828002C,  //  0048  LDCONST	R10	K44
      0x582C002C,  //  0049  LDCONST	R11	K44
      0x7C200600,  //  004A  CALL	R8	3
      0x50200200,  //  004B  LDBOOL	R8	1	0
      0x1C200A08,  //  004C  EQ	R8	R5	R8
      0x74220004,  //  004D  JMPT	R8	#0053
      0x88200516,  //  004E  GETMBR	R8	R2	K22
      0xB8260000,  //  004F  GETNGBL	R9	K0
      0x8824130E,  //  0050  GETMBR	R9	R9	K14
      0x1C201009,  //  0051  EQ	R8	R8	R9
      0x78220016,  //  0052  JMPF	R8	#006A
      0xB8220000,  //  0053  GETNGBL	R8	K0
      0x8820110E,  //  0054  GETMBR	R8	R8	K14
      0x900A2C08,  //  0055  SETMBR	R2	K22	R8
      0x8C200161,  //  0056  GETMET	R8	R0	K97
      0x5C280E00,  //  0057  MOVE	R10	R7
      0x5C2C0400,  //  0058  MOVE	R11	R2
      0x4C300000,  //  0059  LDNIL	R12
      0x7C200800,  //  005A  CALL	R8	4
      0xB8221600,  //  005B  GETNGBL	R8	K11
      0x8C201121,  //  005C  GETMET	R8	R8	K33
      0x58280008,  //  005D  LDCONST	R10	K8
      0x7C200400,  //  005E  CALL	R8	2
      0x78220008,  //  005F  JMPF	R8	#0069
      0xB8220600,  //  0060  GETNGBL	R8	K3
      0x60240018,  //  0061  GETGBL	R9	G24
      0x58280062,  //  0062  LDCONST	R10	K98
      0x882C0305,  //  0063  GETMBR	R11	R1	K5
      0x882C1706,  //  0064  GETMBR	R11	R11	K6
      0x8830030A,  //  0065  GETMBR	R12	R1	K10
      0x7C240600,  //  0066  CALL	R9	3
      0x58280008,  //  0067  LDCONST	R10	K8
      0x7C200400,  //  0068  CALL	R8	2
      0x70020043,  //  0069  JMP		#00AE
      0x4C200000,  //  006A  LDNIL	R8
      0x20200A08,  //  006B  NE	R8	R5	R8
      0x78220017,  //  006C  JMPF	R8	#0085
      0x8C200161,  //  006D  GETMET	R8	R0	K97
      0x5C280E00,  //  006E  MOVE	R10	R7
      0x5C2C0400,  //  006F  MOVE	R11	R2
      0x5C300A00,  //  0070  MOVE	R12	R5
      0x7C200800,  //  0071  CALL	R8	4
      0x5C200600,  //  0072  MOVE	R8	R3
      0x74220000,  //  0073  JMPT	R8	#0075
      0x580C0015,  //  0074  LDCONST	R3	K21
      0xB8221600,  //  0075  GETNGBL	R8	K11
      0x8C201121,  //  0076  GETMET	R8	R8	K33
      0x58280008,  //  0077  LDCONST	R10	K8
      0x7C200400,  //  0078  CALL	R8	2
      0x78220009,  //  0079  JMPF	R8	#0084
      0xB8220600,  //  007A  GETNGBL	R8	K3
      0x60240018,  //  007B  GETGBL	R9	G24
      0x58280063,  //  007C  LDCONST	R10	K99
      0x882C0305,  //  007D  GETMBR	R11	R1	K5
      0x882C1706,  //  007E  GETMBR	R11	R11	K6
      0x5C300400,  //  007F  MOVE	R12	R2
      0x5C340600,  //  0080  MOVE	R13	R3
      0x7C240800,  //  0081  CALL	R9	4
      0x58280008,  //  0082  LDCONST	R10	K8
      0x7C200400,  //  0083  CALL	R8	2
      0x70020028,  //  0084  JMP		#00AE
      0x88200516,  //  0085  GETMBR	R8	R2	K22
      0x4C240000,  //  0086  LDNIL	R9
      0x20201009,  //  0087  NE	R8	R8	R9
      0x78220014,  //  0088  JMPF	R8	#009E
      0x8C200161,  //  0089  GETMET	R8	R0	K97
      0x5C280E00,  //  008A  MOVE	R10	R7
      0x5C2C0400,  //  008B  MOVE	R11	R2
      0x4C300000,  //  008C  LDNIL	R12
      0x7C200800,  //  008D  CALL	R8	4
      0xB8221600,  //  008E  GETNGBL	R8	K11
      0x8C201121,  //  008F  GETMET	R8	R8	K33
      0x58280008,  //  0090  LDCONST	R10	K8
      0x7C200400,  //  0091  CALL	R8	2
      0x78220009,  //  0092  JMPF	R8	#009D
      0xB8220600,  //  0093  GETNGBL	R8	K3
      0x60240018,  //  0094  GETGBL	R9	G24
      0x58280064,  //  0095  LDCONST	R10	K100
      0x882C0305,  //  0096  GETMBR	R11	R1	K5
      0x882C1706,  //  0097  GETMBR	R11	R11	K6
      0x88300516,  //  0098  GETMBR	R12	R2	K22
      0x8834030A,  //  0099  GETMBR	R13	R1	K10
      0x7C240800,  //  009A  CALL	R9	4
      0x58280008,  //  009B  LDCONST	R10	K8
      0x7C200400,  //  009C  CALL	R8	2
      0x7002000F,  //  009D  JMP		#00AE
      0xB8221600,  //  009E  GETNGBL	R8	K11
      0x8C201121,  //  009F  GETMET	R8	R8	K33
      0x58280008,  //  00A0  LDCONST	R10	K8
      0x7C200400,  //  00A1  CALL	R8	2
      0x78220008,  //  00A2  JMPF	R8	#00AC
      0xB8220600,  //  00A3  GETNGBL	R8	K3
      0x60240018,  //  00A4  GETGBL	R9	G24
      0x58280065,  //  00A5  LDCONST	R10	K101
      0x882C0305,  //  00A6  GETMBR	R11	R1	K5
      0x882C1706,  //  00A7  GETMBR	R11	R11	K6
      0x8830030A,  //  00A8  GETMBR	R12	R1	K10
      0x7C240600,  //  00A9  CALL	R9	3
      0x58280008,  //  00AA  LDCONST	R10	K8
      0x7C200400,  //  00AB  CALL	R8	2
      0x50200000,  //  00AC  LDBOOL	R8	0	0
      0x80041000,  //  00AD  RET	1	R8
      0x8C200F43,  //  00AE  GETMET	R8	R7	K67
      0x58280045,  //  00AF  LDCONST	R10	K69
      0x542DFFFB,  //  00B0  LDINT	R11	-4
      0x7C200600,  //  00B1  CALL	R8	3
      0x8C200F43,  //  00B2  GETMET	R8	R7	K67
      0x542A0017,  //  00B3  LDINT	R10	24
      0x582C002C,  //  00B4  LDCONST	R11	K44
      0x7C200600,  //  00B5  CALL	R8	3
      0x8C200348,  //  00B6  GETMET	R8	R1	K72
      0x542A0008,  //  00B7  LDINT	R10	9
      0x502C0200,  //  00B8  LDBOOL	R11	1	0
      0x7C200600,  //  00B9  CALL	R8	3
      0x88240138,  //  00BA  GETMBR	R9	R0	K56
      0x88241349,  //  00BB  GETMBR	R9	R9	K73
      0x88280326,  //  00BC  GETMBR	R10	R1	K38
      0x8C2C154A,  //  00BD  GETMET	R11	R10	K74
      0x7C2C0200,  //  00BE  CALL	R11	1
      0x8C2C114B,  //  00BF  GETMET	R11	R8	K75
      0x5C340E00,  //  00C0  MOVE	R13	R7
      0x5C381400,  //  00C1  MOVE	R14	R10
      0x7C2C0600,  //  00C2  CALL	R11	3
      0x8C2C114C,  //  00C3  GETMET	R11	R8	K76
      0x7C2C0200,  //  00C4  CALL	R11	1
      0x8C2C1350,  //  00C5  GETMET	R11	R9	K80
      0x5C341000,  //  00C6  MOVE	R13	R8
      0x7C2C0400,  //  00C7  CALL	R11	2
      0x502C0200,  //  00C8  LDBOOL	R11	1	0
      0x80041600,  //  00C9  RET	1	R11
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
      0x5808001F,  //  0000  LDCONST	R2	K31
      0x600C000C,  //  0001  GETGBL	R3	G12
      0x88100166,  //  0002  GETMBR	R4	R0	K102
      0x7C0C0200,  //  0003  CALL	R3	1
      0x140C0403,  //  0004  LT	R3	R2	R3
      0x780E0011,  //  0005  JMPF	R3	#0018
      0x880C0166,  //  0006  GETMBR	R3	R0	K102
      0x940C0602,  //  0007  GETIDX	R3	R3	R2
      0x88100767,  //  0008  GETMBR	R4	R3	K103
      0x74120004,  //  0009  JMPT	R4	#000F
      0x88100768,  //  000A  GETMBR	R4	R3	K104
      0x78120002,  //  000B  JMPF	R4	#000F
      0x8C100769,  //  000C  GETMET	R4	R3	K105
      0x5C180200,  //  000D  MOVE	R6	R1
      0x7C100400,  //  000E  CALL	R4	2
      0x88100767,  //  000F  GETMBR	R4	R3	K103
      0x78120004,  //  0010  JMPF	R4	#0016
      0x8C10016A,  //  0011  GETMET	R4	R0	K106
      0x8818076B,  //  0012  GETMBR	R6	R3	K107
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
      0x8C0C076C,  //  0001  GETMET	R3	R3	K108
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C0702,  //  0003  GETMET	R3	R3	K2
      0x5C140400,  //  0004  MOVE	R5	R2
      0x7C0C0400,  //  0005  CALL	R3	2
      0x8810076D,  //  0006  GETMBR	R4	R3	K109
      0x74120003,  //  0007  JMPT	R4	#000C
      0x88100134,  //  0008  GETMBR	R4	R0	K52
      0x8C10096E,  //  0009  GETMET	R4	R4	K110
      0x88180305,  //  000A  GETMBR	R6	R1	K5
      0x7C100400,  //  000B  CALL	R4	2
      0x88100134,  //  000C  GETMBR	R4	R0	K52
      0x8C10096F,  //  000D  GETMET	R4	R4	K111
      0x88180305,  //  000E  GETMBR	R6	R1	K5
      0x5C1C0600,  //  000F  MOVE	R7	R3
      0x7C100600,  //  0010  CALL	R4	3
      0xB8161600,  //  0011  GETNGBL	R5	K11
      0x8C140B21,  //  0012  GETMET	R5	R5	K33
      0x581C0008,  //  0013  LDCONST	R7	K8
      0x7C140400,  //  0014  CALL	R5	2
      0x78160032,  //  0015  JMPF	R5	#0049
      0x88140770,  //  0016  GETMBR	R5	R3	K112
      0x4C180000,  //  0017  LDNIL	R6
      0x20140A06,  //  0018  NE	R5	R5	R6
      0x7816002E,  //  0019  JMPF	R5	#0049
      0x60140012,  //  001A  GETGBL	R5	G18
      0x7C140000,  //  001B  CALL	R5	0
      0xB81A0000,  //  001C  GETNGBL	R6	K0
      0x8C180D71,  //  001D  GETMET	R6	R6	K113
      0x7C180200,  //  001E  CALL	R6	1
      0x901A6E01,  //  001F  SETMBR	R6	K55	R1
      0x601C0010,  //  0020  GETGBL	R7	G16
      0x88200770,  //  0021  GETMBR	R8	R3	K112
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
      0x581C0072,  //  0032  LDCONST	R7	K114
      0xAC1C0200,  //  0033  CATCH	R7	1	0
      0xB0080000,  //  0034  RAISE	2	R0	R0
      0xB81E0600,  //  0035  GETNGBL	R7	K3
      0x60200018,  //  0036  GETGBL	R8	G24
      0x58240073,  //  0037  LDCONST	R9	K115
      0x88280305,  //  0038  GETMBR	R10	R1	K5
      0x88281506,  //  0039  GETMBR	R10	R10	K6
      0x8C2C0B74,  //  003A  GETMET	R11	R5	K116
      0x58340075,  //  003B  LDCONST	R13	K117
      0x7C2C0400,  //  003C  CALL	R11	2
      0x88300976,  //  003D  GETMBR	R12	R4	K118
      0x88340977,  //  003E  GETMBR	R13	R4	K119
      0x8838076D,  //  003F  GETMBR	R14	R3	K109
      0x783A0001,  //  0040  JMPF	R14	#0043
      0x5838002C,  //  0041  LDCONST	R14	K44
      0x70020000,  //  0042  JMP		#0044
      0x5838001F,  //  0043  LDCONST	R14	K31
      0x883C0978,  //  0044  GETMBR	R15	R4	K120
      0x88400779,  //  0045  GETMBR	R16	R3	K121
      0x7C201000,  //  0046  CALL	R8	8
      0x58240008,  //  0047  LDCONST	R9	K8
      0x7C1C0400,  //  0048  CALL	R7	2
      0x8C14017A,  //  0049  GETMET	R5	R0	K122
      0x5C1C0600,  //  004A  MOVE	R7	R3
      0x5C200200,  //  004B  MOVE	R8	R1
      0x7C140600,  //  004C  CALL	R5	3
      0x8C18017B,  //  004D  GETMET	R6	R0	K123
      0x5C200600,  //  004E  MOVE	R8	R3
      0x5C240200,  //  004F  MOVE	R9	R1
      0x7C180600,  //  0050  CALL	R6	3
      0x8C1C097C,  //  0051  GETMET	R7	R4	K124
      0x5C240C00,  //  0052  MOVE	R9	R6
      0x7C1C0400,  //  0053  CALL	R7	2
      0x881C0166,  //  0054  GETMBR	R7	R0	K102
      0x8C1C0F1D,  //  0055  GETMET	R7	R7	K29
      0xB8260000,  //  0056  GETNGBL	R9	K0
      0x8C24137D,  //  0057  GETMET	R9	R9	K125
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
    ( &(const binstruction[148]) {  /* code */
      0x8C100343,  //  0000  GETMET	R4	R1	K67
      0x541A0014,  //  0001  LDINT	R6	21
      0x581C002C,  //  0002  LDCONST	R7	K44
      0x7C100600,  //  0003  CALL	R4	3
      0x4C100000,  //  0004  LDNIL	R4
      0x1C100604,  //  0005  EQ	R4	R3	R4
      0x78120004,  //  0006  JMPF	R4	#000C
      0x8C100343,  //  0007  GETMET	R4	R1	K67
      0x541A3500,  //  0008  LDINT	R6	13569
      0x541DFFFD,  //  0009  LDINT	R7	-2
      0x7C100600,  //  000A  CALL	R4	3
      0x70020003,  //  000B  JMP		#0010
      0x8C100343,  //  000C  GETMET	R4	R1	K67
      0x541A34FF,  //  000D  LDINT	R6	13568
      0x541DFFFD,  //  000E  LDINT	R7	-2
      0x7C100600,  //  000F  CALL	R4	3
      0x8C100343,  //  0010  GETMET	R4	R1	K67
      0x541A36FF,  //  0011  LDINT	R6	14080
      0x541DFFFD,  //  0012  LDINT	R7	-2
      0x7C100600,  //  0013  CALL	R4	3
      0x8810051B,  //  0014  GETMBR	R4	R2	K27
      0x541600FE,  //  0015  LDINT	R5	255
      0x18100805,  //  0016  LE	R4	R4	R5
      0x78120008,  //  0017  JMPF	R4	#0021
      0x8C100343,  //  0018  GETMET	R4	R1	K67
      0x541A23FF,  //  0019  LDINT	R6	9216
      0x541DFFFD,  //  001A  LDINT	R7	-2
      0x7C100600,  //  001B  CALL	R4	3
      0x8C100343,  //  001C  GETMET	R4	R1	K67
      0x8818051B,  //  001D  GETMBR	R6	R2	K27
      0x581C002C,  //  001E  LDCONST	R7	K44
      0x7C100600,  //  001F  CALL	R4	3
      0x70020007,  //  0020  JMP		#0029
      0x8C100343,  //  0021  GETMET	R4	R1	K67
      0x541A24FF,  //  0022  LDINT	R6	9472
      0x541DFFFD,  //  0023  LDINT	R7	-2
      0x7C100600,  //  0024  CALL	R4	3
      0x8C100343,  //  0025  GETMET	R4	R1	K67
      0x8818051B,  //  0026  GETMBR	R6	R2	K27
      0x581C0020,  //  0027  LDCONST	R7	K32
      0x7C100600,  //  0028  CALL	R4	3
      0x88100511,  //  0029  GETMBR	R4	R2	K17
      0x541600FE,  //  002A  LDINT	R5	255
      0x18100805,  //  002B  LE	R4	R4	R5
      0x78120008,  //  002C  JMPF	R4	#0036
      0x8C100343,  //  002D  GETMET	R4	R1	K67
      0x541A2400,  //  002E  LDINT	R6	9217
      0x541DFFFD,  //  002F  LDINT	R7	-2
      0x7C100600,  //  0030  CALL	R4	3
      0x8C100343,  //  0031  GETMET	R4	R1	K67
      0x88180511,  //  0032  GETMBR	R6	R2	K17
      0x581C002C,  //  0033  LDCONST	R7	K44
      0x7C100600,  //  0034  CALL	R4	3
      0x70020014,  //  0035  JMP		#004B
      0x88100511,  //  0036  GETMBR	R4	R2	K17
      0x5416FFFE,  //  0037  LDINT	R5	65535
      0x18100805,  //  0038  LE	R4	R4	R5
      0x78120008,  //  0039  JMPF	R4	#0043
      0x8C100343,  //  003A  GETMET	R4	R1	K67
      0x541A2500,  //  003B  LDINT	R6	9473
      0x541DFFFD,  //  003C  LDINT	R7	-2
      0x7C100600,  //  003D  CALL	R4	3
      0x8C100343,  //  003E  GETMET	R4	R1	K67
      0x88180511,  //  003F  GETMBR	R6	R2	K17
      0x581C0020,  //  0040  LDCONST	R7	K32
      0x7C100600,  //  0041  CALL	R4	3
      0x70020007,  //  0042  JMP		#004B
      0x8C100343,  //  0043  GETMET	R4	R1	K67
      0x541A2600,  //  0044  LDINT	R6	9729
      0x541DFFFD,  //  0045  LDINT	R7	-2
      0x7C100600,  //  0046  CALL	R4	3
      0x8C100343,  //  0047  GETMET	R4	R1	K67
      0x88180511,  //  0048  GETMBR	R6	R2	K17
      0x541E0003,  //  0049  LDINT	R7	4
      0x7C100600,  //  004A  CALL	R4	3
      0x8810055A,  //  004B  GETMBR	R4	R2	K90
      0x541600FE,  //  004C  LDINT	R5	255
      0x18100805,  //  004D  LE	R4	R4	R5
      0x78120008,  //  004E  JMPF	R4	#0058
      0x8C100343,  //  004F  GETMET	R4	R1	K67
      0x541A2401,  //  0050  LDINT	R6	9218
      0x541DFFFD,  //  0051  LDINT	R7	-2
      0x7C100600,  //  0052  CALL	R4	3
      0x8C100343,  //  0053  GETMET	R4	R1	K67
      0x8818055A,  //  0054  GETMBR	R6	R2	K90
      0x581C002C,  //  0055  LDCONST	R7	K44
      0x7C100600,  //  0056  CALL	R4	3
      0x70020014,  //  0057  JMP		#006D
      0x8810055A,  //  0058  GETMBR	R4	R2	K90
      0x5416FFFE,  //  0059  LDINT	R5	65535
      0x18100805,  //  005A  LE	R4	R4	R5
      0x78120008,  //  005B  JMPF	R4	#0065
      0x8C100343,  //  005C  GETMET	R4	R1	K67
      0x541A2501,  //  005D  LDINT	R6	9474
      0x541DFFFD,  //  005E  LDINT	R7	-2
      0x7C100600,  //  005F  CALL	R4	3
      0x8C100343,  //  0060  GETMET	R4	R1	K67
      0x8818055A,  //  0061  GETMBR	R6	R2	K90
      0x581C0020,  //  0062  LDCONST	R7	K32
      0x7C100600,  //  0063  CALL	R4	3
      0x70020007,  //  0064  JMP		#006D
      0x8C100343,  //  0065  GETMET	R4	R1	K67
      0x541A2601,  //  0066  LDINT	R6	9730
      0x541DFFFD,  //  0067  LDINT	R7	-2
      0x7C100600,  //  0068  CALL	R4	3
      0x8C100343,  //  0069  GETMET	R4	R1	K67
      0x8818055A,  //  006A  GETMBR	R6	R2	K90
      0x541E0003,  //  006B  LDINT	R7	4
      0x7C100600,  //  006C  CALL	R4	3
      0x8C100343,  //  006D  GETMET	R4	R1	K67
      0x541A0017,  //  006E  LDINT	R6	24
      0x581C002C,  //  006F  LDCONST	R7	K44
      0x7C100600,  //  0070  CALL	R4	3
      0x4C100000,  //  0071  LDNIL	R4
      0x1C100604,  //  0072  EQ	R4	R3	R4
      0x78120016,  //  0073  JMPF	R4	#008B
      0x88100516,  //  0074  GETMBR	R4	R2	K22
      0x4C140000,  //  0075  LDNIL	R5
      0x1C140805,  //  0076  EQ	R5	R4	R5
      0x78160001,  //  0077  JMPF	R5	#007A
      0xB8160000,  //  0078  GETNGBL	R5	K0
      0x88100B0E,  //  0079  GETMBR	R4	R5	K14
      0x8C140343,  //  007A  GETMET	R5	R1	K67
      0x541E3500,  //  007B  LDINT	R7	13569
      0x5421FFFD,  //  007C  LDINT	R8	-2
      0x7C140600,  //  007D  CALL	R5	3
      0x8C140343,  //  007E  GETMET	R5	R1	K67
      0x541E23FF,  //  007F  LDINT	R7	9216
      0x5421FFFD,  //  0080  LDINT	R8	-2
      0x7C140600,  //  0081  CALL	R5	3
      0x8C140343,  //  0082  GETMET	R5	R1	K67
      0x881C0516,  //  0083  GETMBR	R7	R2	K22
      0x5820002C,  //  0084  LDCONST	R8	K44
      0x7C140600,  //  0085  CALL	R5	3
      0x8C140343,  //  0086  GETMET	R5	R1	K67
      0x541E0017,  //  0087  LDINT	R7	24
      0x5820002C,  //  0088  LDCONST	R8	K44
      0x7C140600,  //  0089  CALL	R5	3
      0x70020003,  //  008A  JMP		#008F
      0x900EFD2C,  //  008B  SETMBR	R3	K126	K44
      0x8C10077F,  //  008C  GETMET	R4	R3	K127
      0x5C180200,  //  008D  MOVE	R6	R1
      0x7C100400,  //  008E  CALL	R4	2
      0x8C100343,  //  008F  GETMET	R4	R1	K67
      0x541A1817,  //  0090  LDINT	R6	6168
      0x541DFFFD,  //  0091  LDINT	R7	-2
      0x7C100600,  //  0092  CALL	R4	3
      0x80000000,  //  0093  RET	0
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
      0x90027001,  //  0000  SETMBR	R0	K56	R1
      0x60080012,  //  0001  GETGBL	R2	G18
      0x7C080000,  //  0002  CALL	R2	0
      0x9002CC02,  //  0003  SETMBR	R0	K102	R2
      0xB80A0000,  //  0004  GETNGBL	R2	K0
      0x8C080580,  //  0005  GETMET	R2	R2	K128
      0x5C100000,  //  0006  MOVE	R4	R0
      0x7C080400,  //  0007  CALL	R2	2
      0x90026802,  //  0008  SETMBR	R0	K52	R2
      0xB80A0000,  //  0009  GETNGBL	R2	K0
      0x8C080581,  //  000A  GETMET	R2	R2	K129
      0x7C080200,  //  000B  CALL	R2	1
      0x90024802,  //  000C  SETMBR	R0	K36	R2
      0xB80A0000,  //  000D  GETNGBL	R2	K0
      0x8C080582,  //  000E  GETMET	R2	R2	K130
      0x7C080200,  //  000F  CALL	R2	1
      0x90025202,  //  0010  SETMBR	R0	K41	R2
      0xB80A0000,  //  0011  GETNGBL	R2	K0
      0x8808050F,  //  0012  GETMBR	R2	R2	K15
      0x8C080583,  //  0013  GETMET	R2	R2	K131
      0x7C080200,  //  0014  CALL	R2	1
      0x90027802,  //  0015  SETMBR	R0	K60	R2
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
    ( &(const binstruction[43]) {  /* code */
      0x8C0C0584,  //  0000  GETMET	R3	R2	K132
      0x5814001F,  //  0001  LDCONST	R5	K31
      0x541A00FE,  //  0002  LDINT	R6	255
      0x7C0C0600,  //  0003  CALL	R3	3
      0x8C100185,  //  0004  GETMET	R4	R0	K133
      0x8818030A,  //  0005  GETMBR	R6	R1	K10
      0x7C100400,  //  0006  CALL	R4	2
      0xB8160000,  //  0007  GETNGBL	R5	K0
      0x88140B0E,  //  0008  GETMBR	R5	R5	K14
      0x1C140605,  //  0009  EQ	R5	R3	R5
      0x7816000F,  //  000A  JMPF	R5	#001B
      0x78120004,  //  000B  JMPF	R4	#0011
      0x8C140986,  //  000C  GETMET	R5	R4	K134
      0x5C1C0200,  //  000D  MOVE	R7	R1
      0x7C140400,  //  000E  CALL	R5	2
      0x80040A00,  //  000F  RET	1	R5
      0x70020008,  //  0010  JMP		#001A
      0xB8160600,  //  0011  GETNGBL	R5	K3
      0x60180018,  //  0012  GETGBL	R6	G24
      0x581C0087,  //  0013  LDCONST	R7	K135
      0x88200305,  //  0014  GETMBR	R8	R1	K5
      0x88201106,  //  0015  GETMBR	R8	R8	K6
      0x8824030A,  //  0016  GETMBR	R9	R1	K10
      0x7C180600,  //  0017  CALL	R6	3
      0x541E0003,  //  0018  LDINT	R7	4
      0x7C140400,  //  0019  CALL	R5	2
      0x7002000D,  //  001A  JMP		#0029
      0xB8160600,  //  001B  GETNGBL	R5	K3
      0x60180018,  //  001C  GETGBL	R6	G24
      0x581C0088,  //  001D  LDCONST	R7	K136
      0x5C200600,  //  001E  MOVE	R8	R3
      0x7C180400,  //  001F  CALL	R6	2
      0x581C0008,  //  0020  LDCONST	R7	K8
      0x7C140400,  //  0021  CALL	R5	2
      0x78120005,  //  0022  JMPF	R4	#0029
      0x8C140989,  //  0023  GETMET	R5	R4	K137
      0x5C1C0200,  //  0024  MOVE	R7	R1
      0x7C140400,  //  0025  CALL	R5	2
      0x8C14016A,  //  0026  GETMET	R5	R0	K106
      0x881C030A,  //  0027  GETMBR	R7	R1	K10
      0x7C140400,  //  0028  CALL	R5	2
      0x50140000,  //  0029  LDBOOL	R5	0	0
      0x80040A00,  //  002A  RET	1	R5
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
      0x880C038A,  //  0000  GETMBR	R3	R1	K138
      0x4C100000,  //  0001  LDNIL	R4
      0x200C0604,  //  0002  NE	R3	R3	R4
      0x780E0085,  //  0003  JMPF	R3	#008A
      0x4C0C0000,  //  0004  LDNIL	R3
      0x4C100000,  //  0005  LDNIL	R4
      0x20100404,  //  0006  NE	R4	R2	R4
      0x78120002,  //  0007  JMPF	R4	#000B
      0x8C10058B,  //  0008  GETMET	R4	R2	K139
      0x7C100200,  //  0009  CALL	R4	1
      0x70020000,  //  000A  JMP		#000C
      0x4C100000,  //  000B  LDNIL	R4
      0x8814038A,  //  000C  GETMBR	R5	R1	K138
      0x78160003,  //  000D  JMPF	R5	#0012
      0x6014000C,  //  000E  GETGBL	R5	G12
      0x8818038A,  //  000F  GETMBR	R6	R1	K138
      0x7C140200,  //  0010  CALL	R5	1
      0x70020000,  //  0011  JMP		#0013
      0x5814001F,  //  0012  LDCONST	R5	K31
      0x24180B2C,  //  0013  GT	R6	R5	K44
      0x781A0002,  //  0014  JMPF	R6	#0018
      0x60180012,  //  0015  GETGBL	R6	G18
      0x7C180000,  //  0016  CALL	R6	0
      0x5C0C0C00,  //  0017  MOVE	R3	R6
      0x8C18018C,  //  0018  GETMET	R6	R0	K140
      0x8820038D,  //  0019  GETMBR	R8	R1	K141
      0x5C240800,  //  001A  MOVE	R9	R4
      0x7C180600,  //  001B  CALL	R6	3
      0x881C038A,  //  001C  GETMBR	R7	R1	K138
      0x781E006A,  //  001D  JMPF	R7	#0089
      0x601C0010,  //  001E  GETGBL	R7	G16
      0x8820038A,  //  001F  GETMBR	R8	R1	K138
      0x7C1C0200,  //  0020  CALL	R7	1
      0xA8020063,  //  0021  EXBLK	0	#0086
      0x5C200E00,  //  0022  MOVE	R8	R7
      0x7C200000,  //  0023  CALL	R8	0
      0xB8260000,  //  0024  GETNGBL	R9	K0
      0x8C24138E,  //  0025  GETMET	R9	R9	K142
      0x882C0138,  //  0026  GETMBR	R11	R0	K56
      0x7C240400,  //  0027  CALL	R9	2
      0x8C28138F,  //  0028  GETMET	R10	R9	K143
      0x8830111B,  //  0029  GETMBR	R12	R8	K27
      0x88341111,  //  002A  GETMBR	R13	R8	K17
      0x88381190,  //  002B  GETMBR	R14	R8	K144
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
      0x882C1190,  //  003F  GETMBR	R11	R8	K144
      0x4C300000,  //  0040  LDNIL	R12
      0x202C160C,  //  0041  NE	R11	R11	R12
      0x782E000D,  //  0042  JMPF	R11	#0051
      0xB82E0000,  //  0043  GETNGBL	R11	K0
      0x8C2C1791,  //  0044  GETMET	R11	R11	K145
      0x88341111,  //  0045  GETMBR	R13	R8	K17
      0x88381190,  //  0046  GETMBR	R14	R8	K144
      0x7C2C0600,  //  0047  CALL	R11	3
      0x5C281600,  //  0048  MOVE	R10	R11
      0x4C2C0000,  //  0049  LDNIL	R11
      0x202C140B,  //  004A  NE	R11	R10	R11
      0x782E0002,  //  004B  JMPF	R11	#004F
      0x002EBA0A,  //  004C  ADD	R11	K93	R10
      0x002C175E,  //  004D  ADD	R11	R11	K94
      0x70020000,  //  004E  JMP		#0050
      0x582C0015,  //  004F  LDCONST	R11	K21
      0x5C281600,  //  0050  MOVE	R10	R11
      0x882C111B,  //  0051  GETMBR	R11	R8	K27
      0x4C300000,  //  0052  LDNIL	R12
      0x202C160C,  //  0053  NE	R11	R11	R12
      0x782E0004,  //  0054  JMPF	R11	#005A
      0x602C0018,  //  0055  GETGBL	R11	G24
      0x58300092,  //  0056  LDCONST	R12	K146
      0x8834111B,  //  0057  GETMBR	R13	R8	K27
      0x7C2C0400,  //  0058  CALL	R11	2
      0x70020000,  //  0059  JMP		#005B
      0x582C0093,  //  005A  LDCONST	R11	K147
      0x88301111,  //  005B  GETMBR	R12	R8	K17
      0x4C340000,  //  005C  LDNIL	R13
      0x2030180D,  //  005D  NE	R12	R12	R13
      0x78320004,  //  005E  JMPF	R12	#0064
      0x60300018,  //  005F  GETGBL	R12	G24
      0x58340094,  //  0060  LDCONST	R13	K148
      0x88381111,  //  0061  GETMBR	R14	R8	K17
      0x7C300400,  //  0062  CALL	R12	2
      0x70020000,  //  0063  JMP		#0065
      0x58300095,  //  0064  LDCONST	R12	K149
      0x88341190,  //  0065  GETMBR	R13	R8	K144
      0x4C380000,  //  0066  LDNIL	R14
      0x20341A0E,  //  0067  NE	R13	R13	R14
      0x78360004,  //  0068  JMPF	R13	#006E
      0x60340018,  //  0069  GETGBL	R13	G24
      0x58380092,  //  006A  LDCONST	R14	K146
      0x883C1190,  //  006B  GETMBR	R15	R8	K144
      0x7C340400,  //  006C  CALL	R13	2
      0x70020000,  //  006D  JMP		#006F
      0x58340093,  //  006E  LDCONST	R13	K147
      0x4C380000,  //  006F  LDNIL	R14
      0x20380C0E,  //  0070  NE	R14	R6	R14
      0x783A0004,  //  0071  JMPF	R14	#0077
      0x60380018,  //  0072  GETGBL	R14	G24
      0x583C0096,  //  0073  LDCONST	R15	K150
      0x5C400C00,  //  0074  MOVE	R16	R6
      0x7C380400,  //  0075  CALL	R14	2
      0x70020000,  //  0076  JMP		#0078
      0x58380015,  //  0077  LDCONST	R14	K21
      0xB83E0600,  //  0078  GETNGBL	R15	K3
      0x60400018,  //  0079  GETGBL	R16	G24
      0x58440097,  //  007A  LDCONST	R17	K151
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
      0x581C0072,  //  0086  LDCONST	R7	K114
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
      0x58080098,  //  0000  LDCONST	R2	K152
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
      0xB81B3200,  //  000B  GETNGBL	R6	K153
      0x8C180D9A,  //  000C  GETMET	R6	R6	K154
      0x88200B9B,  //  000D  GETMBR	R8	R5	K155
      0x7C180400,  //  000E  CALL	R6	2
      0x781A0012,  //  000F  JMPF	R6	#0023
      0x78060011,  //  0010  JMPF	R1	#0023
      0x881C0B9B,  //  0011  GETMBR	R7	R5	K155
      0x8C1C0F9C,  //  0012  GETMET	R7	R7	K156
      0x7C1C0200,  //  0013  CALL	R7	1
      0x201C0E01,  //  0014  NE	R7	R7	R1
      0x781E000C,  //  0015  JMPF	R7	#0023
      0xB81E1600,  //  0016  GETNGBL	R7	K11
      0x8C1C0F03,  //  0017  GETMET	R7	R7	K3
      0x60240018,  //  0018  GETGBL	R9	G24
      0x5828009D,  //  0019  LDCONST	R10	K157
      0x8C2C0D9C,  //  001A  GETMET	R11	R6	K156
      0x7C2C0200,  //  001B  CALL	R11	1
      0x8C2C179E,  //  001C  GETMET	R11	R11	K158
      0x7C2C0200,  //  001D  CALL	R11	1
      0x8C30039E,  //  001E  GETMET	R12	R1	K158
      0x7C300200,  //  001F  CALL	R12	1
      0x7C240600,  //  0020  CALL	R9	3
      0x7C1C0400,  //  0021  CALL	R7	2
      0x7001FFE5,  //  0022  JMP		#0009
      0xB81F3200,  //  0023  GETNGBL	R7	K153
      0x8C1C0F9A,  //  0024  GETMET	R7	R7	K154
      0x88240B9F,  //  0025  GETMBR	R9	R5	K159
      0x7C1C0400,  //  0026  CALL	R7	2
      0x4C200000,  //  0027  LDNIL	R8
      0x1C200608,  //  0028  EQ	R8	R3	R8
      0x74220001,  //  0029  JMPT	R8	#002C
      0x14200607,  //  002A  LT	R8	R3	R7
      0x78220000,  //  002B  JMPF	R8	#002D
      0x5C0C0E00,  //  002C  MOVE	R3	R7
      0x7001FFDA,  //  002D  JMP		#0009
      0x58100072,  //  002E  LDCONST	R4	K114
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
      0x8C140343,  //  0000  GETMET	R5	R1	K67
      0x541E0036,  //  0001  LDINT	R7	55
      0x5820002C,  //  0002  LDCONST	R8	K44
      0x7C140600,  //  0003  CALL	R5	3
      0x8C140343,  //  0004  GETMET	R5	R1	K67
      0x5C1C0600,  //  0005  MOVE	R7	R3
      0x5820002C,  //  0006  LDCONST	R8	K44
      0x7C140600,  //  0007  CALL	R5	3
      0x8814051B,  //  0008  GETMBR	R5	R2	K27
      0x541A00FE,  //  0009  LDINT	R6	255
      0x18140A06,  //  000A  LE	R5	R5	R6
      0x78160008,  //  000B  JMPF	R5	#0015
      0x8C140343,  //  000C  GETMET	R5	R1	K67
      0x541E2401,  //  000D  LDINT	R7	9218
      0x5421FFFD,  //  000E  LDINT	R8	-2
      0x7C140600,  //  000F  CALL	R5	3
      0x8C140343,  //  0010  GETMET	R5	R1	K67
      0x881C051B,  //  0011  GETMBR	R7	R2	K27
      0x5820002C,  //  0012  LDCONST	R8	K44
      0x7C140600,  //  0013  CALL	R5	3
      0x70020007,  //  0014  JMP		#001D
      0x8C140343,  //  0015  GETMET	R5	R1	K67
      0x541E2501,  //  0016  LDINT	R7	9474
      0x5421FFFD,  //  0017  LDINT	R8	-2
      0x7C140600,  //  0018  CALL	R5	3
      0x8C140343,  //  0019  GETMET	R5	R1	K67
      0x881C051B,  //  001A  GETMBR	R7	R2	K27
      0x58200020,  //  001B  LDCONST	R8	K32
      0x7C140600,  //  001C  CALL	R5	3
      0x88140511,  //  001D  GETMBR	R5	R2	K17
      0x541A00FE,  //  001E  LDINT	R6	255
      0x18140A06,  //  001F  LE	R5	R5	R6
      0x78160008,  //  0020  JMPF	R5	#002A
      0x8C140343,  //  0021  GETMET	R5	R1	K67
      0x541E2402,  //  0022  LDINT	R7	9219
      0x5421FFFD,  //  0023  LDINT	R8	-2
      0x7C140600,  //  0024  CALL	R5	3
      0x8C140343,  //  0025  GETMET	R5	R1	K67
      0x881C0511,  //  0026  GETMBR	R7	R2	K17
      0x5820002C,  //  0027  LDCONST	R8	K44
      0x7C140600,  //  0028  CALL	R5	3
      0x70020014,  //  0029  JMP		#003F
      0x88140511,  //  002A  GETMBR	R5	R2	K17
      0x541AFFFE,  //  002B  LDINT	R6	65535
      0x18140A06,  //  002C  LE	R5	R5	R6
      0x78160008,  //  002D  JMPF	R5	#0037
      0x8C140343,  //  002E  GETMET	R5	R1	K67
      0x541E2502,  //  002F  LDINT	R7	9475
      0x5421FFFD,  //  0030  LDINT	R8	-2
      0x7C140600,  //  0031  CALL	R5	3
      0x8C140343,  //  0032  GETMET	R5	R1	K67
      0x881C0511,  //  0033  GETMBR	R7	R2	K17
      0x58200020,  //  0034  LDCONST	R8	K32
      0x7C140600,  //  0035  CALL	R5	3
      0x70020007,  //  0036  JMP		#003F
      0x8C140343,  //  0037  GETMET	R5	R1	K67
      0x541E2602,  //  0038  LDINT	R7	9731
      0x5421FFFD,  //  0039  LDINT	R8	-2
      0x7C140600,  //  003A  CALL	R5	3
      0x8C140343,  //  003B  GETMET	R5	R1	K67
      0x881C0511,  //  003C  GETMBR	R7	R2	K17
      0x54220003,  //  003D  LDINT	R8	4
      0x7C140600,  //  003E  CALL	R5	3
      0x88140512,  //  003F  GETMBR	R5	R2	K18
      0x541A00FE,  //  0040  LDINT	R6	255
      0x18140A06,  //  0041  LE	R5	R5	R6
      0x78160008,  //  0042  JMPF	R5	#004C
      0x8C140343,  //  0043  GETMET	R5	R1	K67
      0x541E2403,  //  0044  LDINT	R7	9220
      0x5421FFFD,  //  0045  LDINT	R8	-2
      0x7C140600,  //  0046  CALL	R5	3
      0x8C140343,  //  0047  GETMET	R5	R1	K67
      0x881C0512,  //  0048  GETMBR	R7	R2	K18
      0x5820002C,  //  0049  LDCONST	R8	K44
      0x7C140600,  //  004A  CALL	R5	3
      0x70020014,  //  004B  JMP		#0061
      0x88140512,  //  004C  GETMBR	R5	R2	K18
      0x541AFFFE,  //  004D  LDINT	R6	65535
      0x18140A06,  //  004E  LE	R5	R5	R6
      0x78160008,  //  004F  JMPF	R5	#0059
      0x8C140343,  //  0050  GETMET	R5	R1	K67
      0x541E2503,  //  0051  LDINT	R7	9476
      0x5421FFFD,  //  0052  LDINT	R8	-2
      0x7C140600,  //  0053  CALL	R5	3
      0x8C140343,  //  0054  GETMET	R5	R1	K67
      0x881C0512,  //  0055  GETMBR	R7	R2	K18
      0x58200020,  //  0056  LDCONST	R8	K32
      0x7C140600,  //  0057  CALL	R5	3
      0x70020007,  //  0058  JMP		#0061
      0x8C140343,  //  0059  GETMET	R5	R1	K67
      0x541E2603,  //  005A  LDINT	R7	9732
      0x5421FFFD,  //  005B  LDINT	R8	-2
      0x7C140600,  //  005C  CALL	R5	3
      0x8C140343,  //  005D  GETMET	R5	R1	K67
      0x881C0512,  //  005E  GETMBR	R7	R2	K18
      0x54220003,  //  005F  LDINT	R8	4
      0x7C140600,  //  0060  CALL	R5	3
      0x78120003,  //  0061  JMPF	R4	#0066
      0x8C140343,  //  0062  GETMET	R5	R1	K67
      0x541E3404,  //  0063  LDINT	R7	13317
      0x5421FFFD,  //  0064  LDINT	R8	-2
      0x7C140600,  //  0065  CALL	R5	3
      0x8C140343,  //  0066  GETMET	R5	R1	K67
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
      0x5808001F,  //  0005  LDCONST	R2	K31
      0x600C000C,  //  0006  GETGBL	R3	G12
      0x88100166,  //  0007  GETMBR	R4	R0	K102
      0x7C0C0200,  //  0008  CALL	R3	1
      0x140C0403,  //  0009  LT	R3	R2	R3
      0x780E0008,  //  000A  JMPF	R3	#0014
      0x880C0166,  //  000B  GETMBR	R3	R0	K102
      0x940C0602,  //  000C  GETIDX	R3	R3	R2
      0x8C1007A0,  //  000D  GETMET	R4	R3	K160
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
      0x8C080185,  //  0000  GETMET	R2	R0	K133
      0x8810030A,  //  0001  GETMBR	R4	R1	K10
      0x7C080400,  //  0002  CALL	R2	2
      0x780A0008,  //  0003  JMPF	R2	#000D
      0x8C0C05A1,  //  0004  GETMET	R3	R2	K161
      0x5C140200,  //  0005  MOVE	R5	R1
      0x7C0C0400,  //  0006  CALL	R3	2
      0x88100567,  //  0007  GETMBR	R4	R2	K103
      0x78120002,  //  0008  JMPF	R4	#000C
      0x8C10016A,  //  0009  GETMET	R4	R0	K106
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
      0x880C0166,  //  0000  GETMBR	R3	R0	K102
      0x8C0C071D,  //  0001  GETMET	R3	R3	K29
      0xB8160000,  //  0002  GETNGBL	R5	K0
      0x8C140BA2,  //  0003  GETMET	R5	R5	K162
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
      0x581400A3,  //  0008  LDCONST	R5	K163
      0x88180506,  //  0009  GETMBR	R6	R2	K6
      0x881C0378,  //  000A  GETMBR	R7	R1	K120
      0x7C100600,  //  000B  CALL	R4	3
      0x58140008,  //  000C  LDCONST	R5	K8
      0x7C0C0400,  //  000D  CALL	R3	2
      0x500C0200,  //  000E  LDBOOL	R3	1	0
      0x90074803,  //  000F  SETMBR	R1	K164	R3
      0x880C0166,  //  0010  GETMBR	R3	R0	K102
      0x8C0C071D,  //  0011  GETMET	R3	R3	K29
      0xB8160000,  //  0012  GETNGBL	R5	K0
      0x8C140BA5,  //  0013  GETMET	R5	R5	K165
      0x881C05A6,  //  0014  GETMBR	R7	R2	K166
      0x5C200400,  //  0015  MOVE	R8	R2
      0x5C240200,  //  0016  MOVE	R9	R1
      0x7C140800,  //  0017  CALL	R5	4
      0x7C0C0400,  //  0018  CALL	R3	2
      0x8C0C01A7,  //  0019  GETMET	R3	R0	K167
      0x881405A6,  //  001A  GETMBR	R5	R2	K166
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
      0x8C0C07A8,  //  0002  GETMET	R3	R3	K168
      0x7C0C0200,  //  0003  CALL	R3	1
      0x50100000,  //  0004  LDBOOL	R4	0	0
      0x900EF204,  //  0005  SETMBR	R3	K121	R4
      0x60100012,  //  0006  GETGBL	R4	G18
      0x7C100000,  //  0007  CALL	R4	0
      0x900EE004,  //  0008  SETMBR	R3	K112	R4
      0x60100010,  //  0009  GETGBL	R4	G16
      0x881403A9,  //  000A  GETMBR	R5	R1	K169
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
      0x881C0770,  //  0018  GETMBR	R7	R3	K112
      0x8C1C0F1D,  //  0019  GETMET	R7	R7	K29
      0x5C240C00,  //  001A  MOVE	R9	R6
      0x7C1C0400,  //  001B  CALL	R7	2
      0x7001FFEF,  //  001C  JMP		#000D
      0x58100072,  //  001D  LDCONST	R4	K114
      0xAC100200,  //  001E  CATCH	R4	1	0
      0xB0080000,  //  001F  RAISE	2	R0	R0
      0xB8120600,  //  0020  GETNGBL	R4	K3
      0x60140018,  //  0021  GETGBL	R5	G24
      0x581800AA,  //  0022  LDCONST	R6	K170
      0x881C0506,  //  0023  GETMBR	R7	R2	K6
      0x88200378,  //  0024  GETMBR	R8	R1	K120
      0x7C140600,  //  0025  CALL	R5	3
      0x58180008,  //  0026  LDCONST	R6	K8
      0x7C100400,  //  0027  CALL	R4	2
      0x50100000,  //  0028  LDBOOL	R4	0	0
      0x90074804,  //  0029  SETMBR	R1	K164	R4
      0x8C10017A,  //  002A  GETMET	R4	R0	K122
      0x5C180600,  //  002B  MOVE	R6	R3
      0x4C1C0000,  //  002C  LDNIL	R7
      0x7C100600,  //  002D  CALL	R4	3
      0x8C1403AB,  //  002E  GETMET	R5	R1	K171
      0x7C140200,  //  002F  CALL	R5	1
      0xB81A0000,  //  0030  GETNGBL	R6	K0
      0x8C180DAC,  //  0031  GETMET	R6	R6	K172
      0x882005A6,  //  0032  GETMBR	R8	R2	K166
      0x5C240400,  //  0033  MOVE	R9	R2
      0x5C280800,  //  0034  MOVE	R10	R4
      0x5C2C0A00,  //  0035  MOVE	R11	R5
      0x5C300200,  //  0036  MOVE	R12	R1
      0x7C180C00,  //  0037  CALL	R6	6
      0x881C0166,  //  0038  GETMBR	R7	R0	K102
      0x8C1C0F1D,  //  0039  GETMET	R7	R7	K29
      0x5C240C00,  //  003A  MOVE	R9	R6
      0x7C1C0400,  //  003B  CALL	R7	2
      0x8C1C01A7,  //  003C  GETMET	R7	R0	K167
      0x882405A6,  //  003D  GETMBR	R9	R2	K166
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
      0x8C0C07AD,  //  0005  GETMET	R3	R3	K173
      0x8814030A,  //  0006  GETMBR	R5	R1	K10
      0x7C0C0400,  //  0007  CALL	R3	2
      0x88100109,  //  0008  GETMBR	R4	R0	K9
      0x8C1009AE,  //  0009  GETMET	R4	R4	K174
      0x8818030A,  //  000A  GETMBR	R6	R1	K10
      0x7C100400,  //  000B  CALL	R4	2
      0x4C100000,  //  000C  LDNIL	R4
      0x1C100604,  //  000D  EQ	R4	R3	R4
      0x78120009,  //  000E  JMPF	R4	#0019
      0xB8120600,  //  000F  GETNGBL	R4	K3
      0x60140018,  //  0010  GETGBL	R5	G24
      0x581800AF,  //  0011  LDCONST	R6	K175
      0x881C0305,  //  0012  GETMBR	R7	R1	K5
      0x881C0F06,  //  0013  GETMBR	R7	R7	K6
      0x7C140400,  //  0014  CALL	R5	2
      0x58180008,  //  0015  LDCONST	R6	K8
      0x7C100400,  //  0016  CALL	R4	2
      0x50100000,  //  0017  LDBOOL	R4	0	0
      0x80040800,  //  0018  RET	1	R4
      0xB8121600,  //  0019  GETNGBL	R4	K11
      0x8C1009B0,  //  001A  GETMET	R4	R4	K176
      0x5C180600,  //  001B  MOVE	R6	R3
      0x7C100400,  //  001C  CALL	R4	2
      0x78120009,  //  001D  JMPF	R4	#0028
      0xB8120600,  //  001E  GETNGBL	R4	K3
      0x60140018,  //  001F  GETGBL	R5	G24
      0x581800B1,  //  0020  LDCONST	R6	K177
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
      0x5804001F,  //  0000  LDCONST	R1	K31
      0x6008000C,  //  0001  GETGBL	R2	G12
      0x880C0166,  //  0002  GETMBR	R3	R0	K102
      0x7C080200,  //  0003  CALL	R2	1
      0x14080202,  //  0004  LT	R2	R1	R2
      0x780A000F,  //  0005  JMPF	R2	#0016
      0x88080166,  //  0006  GETMBR	R2	R0	K102
      0x94080401,  //  0007  GETIDX	R2	R2	R1
      0xB80E1600,  //  0008  GETNGBL	R3	K11
      0x8C0C07B0,  //  0009  GETMET	R3	R3	K176
      0x881405B2,  //  000A  GETMBR	R5	R2	K178
      0x7C0C0400,  //  000B  CALL	R3	2
      0x780E0006,  //  000C  JMPF	R3	#0014
      0x8C0C05B3,  //  000D  GETMET	R3	R2	K179
      0x7C0C0200,  //  000E  CALL	R3	1
      0x880C0166,  //  000F  GETMBR	R3	R0	K102
      0x8C0C07AE,  //  0010  GETMET	R3	R3	K174
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
    ( &(const binstruction[165]) {  /* code */
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
      0x8C28033B,  //  001C  GETMET	R10	R1	K59
      0x5C300600,  //  001D  MOVE	R12	R3
      0x5C340400,  //  001E  MOVE	R13	R2
      0x8838013C,  //  001F  GETMBR	R14	R0	K60
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
      0x8C2C1151,  //  002C  GETMET	R11	R8	K81
      0x7C2C0200,  //  002D  CALL	R11	1
      0x5C281600,  //  002E  MOVE	R10	R11
      0x882C113D,  //  002F  GETMBR	R11	R8	K61
      0x742E0001,  //  0030  JMPT	R11	#0033
      0x882C113E,  //  0031  GETMBR	R11	R8	K62
      0x782E002F,  //  0032  JMPF	R11	#0063
      0x8C2C113F,  //  0033  GETMET	R11	R8	K63
      0x7C2C0200,  //  0034  CALL	R11	1
      0xB8320000,  //  0035  GETNGBL	R12	K0
      0x88301940,  //  0036  GETMBR	R12	R12	K64
      0x88301941,  //  0037  GETMBR	R12	R12	K65
      0x242C160C,  //  0038  GT	R11	R11	R12
      0x782E0028,  //  0039  JMPF	R11	#0063
      0x602C0012,  //  003A  GETGBL	R11	G18
      0x7C2C0000,  //  003B  CALL	R11	0
      0x5C241600,  //  003C  MOVE	R9	R11
      0x602C0015,  //  003D  GETGBL	R11	G21
      0x5432002F,  //  003E  LDINT	R12	48
      0x7C2C0200,  //  003F  CALL	R11	1
      0x8C300BB4,  //  0040  GETMET	R12	R5	K180
      0x7C300200,  //  0041  CALL	R12	1
      0x8C340144,  //  0042  GETMET	R13	R0	K68
      0x5C3C1600,  //  0043  MOVE	R15	R11
      0x5C400400,  //  0044  MOVE	R16	R2
      0x5C441800,  //  0045  MOVE	R17	R12
      0x50480000,  //  0046  LDBOOL	R18	0	0
      0x7C340A00,  //  0047  CALL	R13	5
      0x8C34131D,  //  0048  GETMET	R13	R9	K29
      0x5C3C1600,  //  0049  MOVE	R15	R11
      0x7C340400,  //  004A  CALL	R13	2
      0x60340010,  //  004B  GETGBL	R13	G16
      0x883811B5,  //  004C  GETMBR	R14	R8	K181
      0x7C340200,  //  004D  CALL	R13	1
      0xA802000F,  //  004E  EXBLK	0	#005F
      0x5C381A00,  //  004F  MOVE	R14	R13
      0x7C380000,  //  0050  CALL	R14	0
      0x603C0015,  //  0051  GETGBL	R15	G21
      0x5442002F,  //  0052  LDINT	R16	48
      0x7C3C0200,  //  0053  CALL	R15	1
      0x5C2C1E00,  //  0054  MOVE	R11	R15
      0x8C3C0144,  //  0055  GETMET	R15	R0	K68
      0x5C441600,  //  0056  MOVE	R17	R11
      0x5C480400,  //  0057  MOVE	R18	R2
      0x5C4C1C00,  //  0058  MOVE	R19	R14
      0x50500200,  //  0059  LDBOOL	R20	1	0
      0x7C3C0A00,  //  005A  CALL	R15	5
      0x8C3C131D,  //  005B  GETMET	R15	R9	K29
      0x5C441600,  //  005C  MOVE	R17	R11
      0x7C3C0400,  //  005D  CALL	R15	2
      0x7001FFEF,  //  005E  JMP		#004F
      0x58340072,  //  005F  LDCONST	R13	K114
      0xAC340200,  //  0060  CATCH	R13	1	0
      0xB0080000,  //  0061  RAISE	2	R0	R0
      0x70020008,  //  0062  JMP		#006C
      0x602C0015,  //  0063  GETGBL	R11	G21
      0x5432002F,  //  0064  LDINT	R12	48
      0x7C2C0200,  //  0065  CALL	R11	1
      0x5C241600,  //  0066  MOVE	R9	R11
      0x8C2C0144,  //  0067  GETMET	R11	R0	K68
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
      0x583400B6,  //  0074  LDCONST	R13	K182
      0x88380706,  //  0075  GETMBR	R14	R3	K6
      0x5C3C0400,  //  0076  MOVE	R15	R2
      0x5C400C00,  //  0077  MOVE	R16	R6
      0x5C441400,  //  0078  MOVE	R17	R10
      0x7C300A00,  //  0079  CALL	R12	5
      0x58340008,  //  007A  LDCONST	R13	K8
      0x7C2C0400,  //  007B  CALL	R11	2
      0x70020026,  //  007C  JMP		#00A4
      0x88280516,  //  007D  GETMBR	R10	R2	K22
      0x4C2C0000,  //  007E  LDNIL	R11
      0x2028140B,  //  007F  NE	R10	R10	R11
      0x782A0022,  //  0080  JMPF	R10	#00A4
      0x781E0021,  //  0081  JMPF	R7	#00A4
      0x60280015,  //  0082  GETGBL	R10	G21
      0x542E002F,  //  0083  LDINT	R11	48
      0x7C280200,  //  0084  CALL	R10	1
      0x5C241400,  //  0085  MOVE	R9	R10
      0x8C280146,  //  0086  GETMET	R10	R0	K70
      0x5C301200,  //  0087  MOVE	R12	R9
      0x5C340400,  //  0088  MOVE	R13	R2
      0x88380516,  //  0089  GETMBR	R14	R2	K22
      0x7C280800,  //  008A  CALL	R10	4
      0xB82A1600,  //  008B  GETNGBL	R10	K11
      0x8C281521,  //  008C  GETMET	R10	R10	K33
      0x58300008,  //  008D  LDCONST	R12	K8
      0x7C280400,  //  008E  CALL	R10	2
      0x782A0013,  //  008F  JMPF	R10	#00A4
      0xB82A0600,  //  0090  GETNGBL	R10	K3
      0x602C0018,  //  0091  GETGBL	R11	G24
      0x583000B7,  //  0092  LDCONST	R12	K183
      0x88340706,  //  0093  GETMBR	R13	R3	K6
      0x60380008,  //  0094  GETGBL	R14	G8
      0x5C3C0400,  //  0095  MOVE	R15	R2
      0x7C380200,  //  0096  CALL	R14	1
      0x5C3C0C00,  //  0097  MOVE	R15	R6
      0x88400516,  //  0098  GETMBR	R16	R2	K22
      0x88440516,  //  0099  GETMBR	R17	R2	K22
      0xB84A0000,  //  009A  GETNGBL	R18	K0
      0x8848253A,  //  009B  GETMBR	R18	R18	K58
      0x1C442212,  //  009C  EQ	R17	R17	R18
      0x78460001,  //  009D  JMPF	R17	#00A0
      0x5844003A,  //  009E  LDCONST	R17	K58
      0x70020000,  //  009F  JMP		#00A1
      0x58440015,  //  00A0  LDCONST	R17	K21
      0x7C2C0C00,  //  00A1  CALL	R11	6
      0x58300008,  //  00A2  LDCONST	R12	K8
      0x7C280400,  //  00A3  CALL	R10	2
      0x80041200,  //  00A4  RET	1	R9
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
      0x880C0370,  //  0000  GETMBR	R3	R1	K112
      0x4C100000,  //  0001  LDNIL	R4
      0x200C0604,  //  0002  NE	R3	R3	R4
      0x780E006C,  //  0003  JMPF	R3	#0071
      0x4C0C0000,  //  0004  LDNIL	R3
      0x88100370,  //  0005  GETMBR	R4	R1	K112
      0x78120003,  //  0006  JMPF	R4	#000B
      0x6010000C,  //  0007  GETGBL	R4	G12
      0x88140370,  //  0008  GETMBR	R5	R1	K112
      0x7C100200,  //  0009  CALL	R4	1
      0x70020000,  //  000A  JMP		#000C
      0x5810001F,  //  000B  LDCONST	R4	K31
      0x2414092C,  //  000C  GT	R5	R4	K44
      0x78160002,  //  000D  JMPF	R5	#0011
      0x60140012,  //  000E  GETGBL	R5	G18
      0x7C140000,  //  000F  CALL	R5	0
      0x5C0C0A00,  //  0010  MOVE	R3	R5
      0x60140010,  //  0011  GETGBL	R5	G16
      0x88180370,  //  0012  GETMBR	R6	R1	K112
      0x7C140200,  //  0013  CALL	R5	1
      0xA8020057,  //  0014  EXBLK	0	#006D
      0x5C180A00,  //  0015  MOVE	R6	R5
      0x7C180000,  //  0016  CALL	R6	0
      0xB81E0000,  //  0017  GETNGBL	R7	K0
      0x8C1C0FB8,  //  0018  GETMET	R7	R7	K184
      0x88240138,  //  0019  GETMBR	R9	R0	K56
      0x7C1C0400,  //  001A  CALL	R7	2
      0x8C200F8F,  //  001B  GETMET	R8	R7	K143
      0x88280D1B,  //  001C  GETMBR	R10	R6	K27
      0x882C0D11,  //  001D  GETMBR	R11	R6	K17
      0x88300D12,  //  001E  GETMBR	R12	R6	K18
      0x88340379,  //  001F  GETMBR	R13	R1	K121
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
      0x8C201171,  //  003A  GETMET	R8	R8	K113
      0x7C200200,  //  003B  CALL	R8	1
      0x88240D1B,  //  003C  GETMBR	R9	R6	K27
      0x90223609,  //  003D  SETMBR	R8	K27	R9
      0x88240D11,  //  003E  GETMBR	R9	R6	K17
      0x90222209,  //  003F  SETMBR	R8	K17	R9
      0x88240D12,  //  0040  GETMBR	R9	R6	K18
      0x90222409,  //  0041  SETMBR	R8	K18	R9
      0x88240379,  //  0042  GETMBR	R9	R1	K121
      0x9022F209,  //  0043  SETMBR	R8	K121	R9
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
      0x583400B9,  //  0056  LDCONST	R13	K185
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
      0x583000B9,  //  0065  LDCONST	R12	K185
      0x88340505,  //  0066  GETMBR	R13	R2	K5
      0x88341B06,  //  0067  GETMBR	R13	R13	K6
      0x5C381200,  //  0068  MOVE	R14	R9
      0x7C2C0600,  //  0069  CALL	R11	3
      0x58300008,  //  006A  LDCONST	R12	K8
      0x7C280400,  //  006B  CALL	R10	2
      0x7001FFA7,  //  006C  JMP		#0015
      0x58140072,  //  006D  LDCONST	R5	K114
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
    ( &(const binstruction[234]) {  /* code */
      0xB80E0000,  //  0000  GETNGBL	R3	K0
      0x8C0C0771,  //  0001  GETMET	R3	R3	K113
      0x7C0C0200,  //  0002  CALL	R3	1
      0x900E6E01,  //  0003  SETMBR	R3	K55	R1
      0xB8120000,  //  0004  GETNGBL	R4	K0
      0x8C1009BA,  //  0005  GETMET	R4	R4	K186
      0x7C100200,  //  0006  CALL	R4	1
      0x8C100902,  //  0007  GETMET	R4	R4	K2
      0x5C180400,  //  0008  MOVE	R6	R2
      0x7C100400,  //  0009  CALL	R4	2
      0x8C140156,  //  000A  GETMET	R5	R0	K86
      0x5C1C0200,  //  000B  MOVE	R7	R1
      0x882009BB,  //  000C  GETMBR	R8	R4	K187
      0x7C140600,  //  000D  CALL	R5	3
      0x74160006,  //  000E  JMPT	R5	#0016
      0x8C14010D,  //  000F  GETMET	R5	R0	K13
      0x5C1C0200,  //  0010  MOVE	R7	R1
      0xB8220000,  //  0011  GETNGBL	R8	K0
      0x88201157,  //  0012  GETMBR	R8	R8	K87
      0x7C140600,  //  0013  CALL	R5	3
      0x50140200,  //  0014  LDBOOL	R5	1	0
      0x80040A00,  //  0015  RET	1	R5
      0x881409BC,  //  0016  GETMBR	R5	R4	K188
      0x4C180000,  //  0017  LDNIL	R6
      0x20140A06,  //  0018  NE	R5	R5	R6
      0x781600CE,  //  0019  JMPF	R5	#00E9
      0xB8160000,  //  001A  GETNGBL	R5	K0
      0x8C140BBD,  //  001B  GETMET	R5	R5	K189
      0x7C140200,  //  001C  CALL	R5	1
      0x50180000,  //  001D  LDBOOL	R6	0	0
      0x90177C06,  //  001E  SETMBR	R5	K190	R6
      0x60180012,  //  001F  GETGBL	R6	G18
      0x7C180000,  //  0020  CALL	R6	0
      0x90177E06,  //  0021  SETMBR	R5	K191	R6
      0x60180010,  //  0022  GETGBL	R6	G16
      0x881C09BC,  //  0023  GETMBR	R7	R4	K188
      0x7C180200,  //  0024  CALL	R6	1
      0xA80200AD,  //  0025  EXBLK	0	#00D4
      0x5C1C0C00,  //  0026  MOVE	R7	R6
      0x7C1C0000,  //  0027  CALL	R7	0
      0x88200FC0,  //  0028  GETMBR	R8	R7	K192
      0x8820111B,  //  0029  GETMBR	R8	R8	K27
      0x900E3608,  //  002A  SETMBR	R3	K27	R8
      0x88200FC0,  //  002B  GETMBR	R8	R7	K192
      0x88201111,  //  002C  GETMBR	R8	R8	K17
      0x900E2208,  //  002D  SETMBR	R3	K17	R8
      0x88200FC0,  //  002E  GETMBR	R8	R7	K192
      0x8820115A,  //  002F  GETMBR	R8	R8	K90
      0x900EB408,  //  0030  SETMBR	R3	K90	R8
      0xB8220000,  //  0031  GETNGBL	R8	K0
      0x88201158,  //  0032  GETMBR	R8	R8	K88
      0x900E2C08,  //  0033  SETMBR	R3	K22	R8
      0xB8220000,  //  0034  GETNGBL	R8	K0
      0x8C201159,  //  0035  GETMET	R8	R8	K89
      0x88280711,  //  0036  GETMBR	R10	R3	K17
      0x882C075A,  //  0037  GETMBR	R11	R3	K90
      0x7C200600,  //  0038  CALL	R8	3
      0x60240008,  //  0039  GETGBL	R9	G8
      0x5C280600,  //  003A  MOVE	R10	R3
      0x7C240200,  //  003B  CALL	R9	1
      0x88280138,  //  003C  GETMBR	R10	R0	K56
      0x8C28155B,  //  003D  GETMET	R10	R10	K91
      0x88300305,  //  003E  GETMBR	R12	R1	K5
      0x88340F5C,  //  003F  GETMBR	R13	R7	K92
      0x5C380600,  //  0040  MOVE	R14	R3
      0x7C280800,  //  0041  CALL	R10	4
      0x882C0703,  //  0042  GETMBR	R11	R3	K3
      0x4C300000,  //  0043  LDNIL	R12
      0x202C160C,  //  0044  NE	R11	R11	R12
      0x782E0005,  //  0045  JMPF	R11	#004C
      0x602C0008,  //  0046  GETGBL	R11	G8
      0x88300703,  //  0047  GETMBR	R12	R3	K3
      0x7C2C0200,  //  0048  CALL	R11	1
      0x002EBA0B,  //  0049  ADD	R11	K93	R11
      0x002C175E,  //  004A  ADD	R11	R11	K94
      0x70020000,  //  004B  JMP		#004D
      0x582C0015,  //  004C  LDCONST	R11	K21
      0xB8320600,  //  004D  GETNGBL	R12	K3
      0x60340018,  //  004E  GETGBL	R13	G24
      0x583800C1,  //  004F  LDCONST	R14	K193
      0x883C0305,  //  0050  GETMBR	R15	R1	K5
      0x883C1F06,  //  0051  GETMBR	R15	R15	K6
      0x5C401200,  //  0052  MOVE	R16	R9
      0x78220001,  //  0053  JMPF	R8	#0056
      0x5C441000,  //  0054  MOVE	R17	R8
      0x70020000,  //  0055  JMP		#0057
      0x58440015,  //  0056  LDCONST	R17	K21
      0x5C481600,  //  0057  MOVE	R18	R11
      0x7C340A00,  //  0058  CALL	R13	5
      0x58380008,  //  0059  LDCONST	R14	K8
      0x7C300400,  //  005A  CALL	R12	2
      0x4C300000,  //  005B  LDNIL	R12
      0x900E060C,  //  005C  SETMBR	R3	K3	R12
      0x60300015,  //  005D  GETGBL	R12	G21
      0x5436001F,  //  005E  LDINT	R13	32
      0x7C300200,  //  005F  CALL	R12	1
      0x50340200,  //  0060  LDBOOL	R13	1	0
      0x1C34140D,  //  0061  EQ	R13	R10	R13
      0x74360004,  //  0062  JMPT	R13	#0068
      0x88340716,  //  0063  GETMBR	R13	R3	K22
      0xB83A0000,  //  0064  GETNGBL	R14	K0
      0x88381D0E,  //  0065  GETMBR	R14	R14	K14
      0x1C341A0E,  //  0066  EQ	R13	R13	R14
      0x7836001A,  //  0067  JMPF	R13	#0083
      0xB8360000,  //  0068  GETNGBL	R13	K0
      0x88341B0E,  //  0069  GETMBR	R13	R13	K14
      0x900E2C0D,  //  006A  SETMBR	R3	K22	R13
      0x8C340161,  //  006B  GETMET	R13	R0	K97
      0x5C3C1800,  //  006C  MOVE	R15	R12
      0x5C400600,  //  006D  MOVE	R16	R3
      0x4C440000,  //  006E  LDNIL	R17
      0x7C340800,  //  006F  CALL	R13	4
      0x88340BBF,  //  0070  GETMBR	R13	R5	K191
      0x8C341B1D,  //  0071  GETMET	R13	R13	K29
      0x5C3C1800,  //  0072  MOVE	R15	R12
      0x7C340400,  //  0073  CALL	R13	2
      0xB8361600,  //  0074  GETNGBL	R13	K11
      0x8C341B21,  //  0075  GETMET	R13	R13	K33
      0x583C0008,  //  0076  LDCONST	R15	K8
      0x7C340400,  //  0077  CALL	R13	2
      0x78360008,  //  0078  JMPF	R13	#0082
      0xB8360600,  //  0079  GETNGBL	R13	K3
      0x60380018,  //  007A  GETGBL	R14	G24
      0x583C0062,  //  007B  LDCONST	R15	K98
      0x88400305,  //  007C  GETMBR	R16	R1	K5
      0x88402106,  //  007D  GETMBR	R16	R16	K6
      0x8844030A,  //  007E  GETMBR	R17	R1	K10
      0x7C380600,  //  007F  CALL	R14	3
      0x583C0008,  //  0080  LDCONST	R15	K8
      0x7C340400,  //  0081  CALL	R13	2
      0x7002004F,  //  0082  JMP		#00D3
      0x4C340000,  //  0083  LDNIL	R13
      0x2034140D,  //  0084  NE	R13	R10	R13
      0x78360021,  //  0085  JMPF	R13	#00A8
      0x8C340161,  //  0086  GETMET	R13	R0	K97
      0x5C3C1800,  //  0087  MOVE	R15	R12
      0x5C400600,  //  0088  MOVE	R16	R3
      0x5C441400,  //  0089  MOVE	R17	R10
      0x7C340800,  //  008A  CALL	R13	4
      0x88340BBF,  //  008B  GETMBR	R13	R5	K191
      0x8C341B1D,  //  008C  GETMET	R13	R13	K29
      0x5C3C1800,  //  008D  MOVE	R15	R12
      0x7C340400,  //  008E  CALL	R13	2
      0xB8360000,  //  008F  GETNGBL	R13	K0
      0x8C341B59,  //  0090  GETMET	R13	R13	K89
      0x883C0711,  //  0091  GETMBR	R15	R3	K17
      0x8840075A,  //  0092  GETMBR	R16	R3	K90
      0x7C340600,  //  0093  CALL	R13	3
      0x5C201A00,  //  0094  MOVE	R8	R13
      0x5C341000,  //  0095  MOVE	R13	R8
      0x74360000,  //  0096  JMPT	R13	#0098
      0x58200015,  //  0097  LDCONST	R8	K21
      0xB8361600,  //  0098  GETNGBL	R13	K11
      0x8C341B21,  //  0099  GETMET	R13	R13	K33
      0x583C0008,  //  009A  LDCONST	R15	K8
      0x7C340400,  //  009B  CALL	R13	2
      0x78360009,  //  009C  JMPF	R13	#00A7
      0xB8360600,  //  009D  GETNGBL	R13	K3
      0x60380018,  //  009E  GETGBL	R14	G24
      0x583C0063,  //  009F  LDCONST	R15	K99
      0x88400305,  //  00A0  GETMBR	R16	R1	K5
      0x88402106,  //  00A1  GETMBR	R16	R16	K6
      0x5C440600,  //  00A2  MOVE	R17	R3
      0x5C481000,  //  00A3  MOVE	R18	R8
      0x7C380800,  //  00A4  CALL	R14	4
      0x583C0008,  //  00A5  LDCONST	R15	K8
      0x7C340400,  //  00A6  CALL	R13	2
      0x7002002A,  //  00A7  JMP		#00D3
      0x88340716,  //  00A8  GETMBR	R13	R3	K22
      0x4C380000,  //  00A9  LDNIL	R14
      0x20341A0E,  //  00AA  NE	R13	R13	R14
      0x78360018,  //  00AB  JMPF	R13	#00C5
      0x8C340161,  //  00AC  GETMET	R13	R0	K97
      0x5C3C1800,  //  00AD  MOVE	R15	R12
      0x5C400600,  //  00AE  MOVE	R16	R3
      0x4C440000,  //  00AF  LDNIL	R17
      0x7C340800,  //  00B0  CALL	R13	4
      0x88340BBF,  //  00B1  GETMBR	R13	R5	K191
      0x8C341B1D,  //  00B2  GETMET	R13	R13	K29
      0x5C3C1800,  //  00B3  MOVE	R15	R12
      0x7C340400,  //  00B4  CALL	R13	2
      0xB8361600,  //  00B5  GETNGBL	R13	K11
      0x8C341B21,  //  00B6  GETMET	R13	R13	K33
      0x583C0008,  //  00B7  LDCONST	R15	K8
      0x7C340400,  //  00B8  CALL	R13	2
      0x78360009,  //  00B9  JMPF	R13	#00C4
      0xB8360600,  //  00BA  GETNGBL	R13	K3
      0x60380018,  //  00BB  GETGBL	R14	G24
      0x583C0064,  //  00BC  LDCONST	R15	K100
      0x88400305,  //  00BD  GETMBR	R16	R1	K5
      0x88402106,  //  00BE  GETMBR	R16	R16	K6
      0x88440716,  //  00BF  GETMBR	R17	R3	K22
      0x8848030A,  //  00C0  GETMBR	R18	R1	K10
      0x7C380800,  //  00C1  CALL	R14	4
      0x583C0008,  //  00C2  LDCONST	R15	K8
      0x7C340400,  //  00C3  CALL	R13	2
      0x7002000D,  //  00C4  JMP		#00D3
      0xB8361600,  //  00C5  GETNGBL	R13	K11
      0x8C341B21,  //  00C6  GETMET	R13	R13	K33
      0x583C0008,  //  00C7  LDCONST	R15	K8
      0x7C340400,  //  00C8  CALL	R13	2
      0x78360008,  //  00C9  JMPF	R13	#00D3
      0xB8360600,  //  00CA  GETNGBL	R13	K3
      0x60380018,  //  00CB  GETGBL	R14	G24
      0x583C0065,  //  00CC  LDCONST	R15	K101
      0x88400305,  //  00CD  GETMBR	R16	R1	K5
      0x88402106,  //  00CE  GETMBR	R16	R16	K6
      0x8844030A,  //  00CF  GETMBR	R17	R1	K10
      0x7C380600,  //  00D0  CALL	R14	3
      0x583C0008,  //  00D1  LDCONST	R15	K8
      0x7C340400,  //  00D2  CALL	R13	2
      0x7001FF51,  //  00D3  JMP		#0026
      0x58180072,  //  00D4  LDCONST	R6	K114
      0xAC180200,  //  00D5  CATCH	R6	1	0
      0xB0080000,  //  00D6  RAISE	2	R0	R0
      0x6018000C,  //  00D7  GETGBL	R6	G12
      0x881C0BBF,  //  00D8  GETMBR	R7	R5	K191
      0x7C180200,  //  00D9  CALL	R6	1
      0x24180D1F,  //  00DA  GT	R6	R6	K31
      0x781A0008,  //  00DB  JMPF	R6	#00E5
      0x88180166,  //  00DC  GETMBR	R6	R0	K102
      0x8C180D1D,  //  00DD  GETMET	R6	R6	K29
      0xB8220000,  //  00DE  GETNGBL	R8	K0
      0x8C2011C2,  //  00DF  GETMET	R8	R8	K194
      0x5C280200,  //  00E0  MOVE	R10	R1
      0x5C2C0A00,  //  00E1  MOVE	R11	R5
      0x7C200600,  //  00E2  CALL	R8	3
      0x7C180400,  //  00E3  CALL	R6	2
      0x70020001,  //  00E4  JMP		#00E7
      0x50180000,  //  00E5  LDBOOL	R6	0	0
      0x80040C00,  //  00E6  RET	1	R6
      0x50180200,  //  00E7  LDBOOL	R6	1	0
      0x80040C00,  //  00E8  RET	1	R6
      0x80000000,  //  00E9  RET	0
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
      0x5808001F,  //  0004  LDCONST	R2	K31
      0x600C000C,  //  0005  GETGBL	R3	G12
      0x88100166,  //  0006  GETMBR	R4	R0	K102
      0x7C0C0200,  //  0007  CALL	R3	1
      0x140C0403,  //  0008  LT	R3	R2	R3
      0x780E000C,  //  0009  JMPF	R3	#0017
      0x880C0166,  //  000A  GETMBR	R3	R0	K102
      0x940C0602,  //  000B  GETIDX	R3	R3	R2
      0x8C0C07A0,  //  000C  GETMET	R3	R3	K160
      0x7C0C0200,  //  000D  CALL	R3	1
      0x1C0C0601,  //  000E  EQ	R3	R3	R1
      0x780E0004,  //  000F  JMPF	R3	#0015
      0x880C0166,  //  0010  GETMBR	R3	R0	K102
      0x8C0C07AE,  //  0011  GETMET	R3	R3	K174
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
      0x8C0C07A8,  //  0001  GETMET	R3	R3	K168
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C0702,  //  0003  GETMET	R3	R3	K2
      0x5C140400,  //  0004  MOVE	R5	R2
      0x7C0C0400,  //  0005  CALL	R3	2
      0x8C10017A,  //  0006  GETMET	R4	R0	K122
      0x5C180600,  //  0007  MOVE	R6	R3
      0x5C1C0200,  //  0008  MOVE	R7	R1
      0x7C100600,  //  0009  CALL	R4	3
      0x8C14017B,  //  000A  GETMET	R5	R0	K123
      0x5C1C0600,  //  000B  MOVE	R7	R3
      0x5C200200,  //  000C  MOVE	R8	R1
      0x7C140600,  //  000D  CALL	R5	3
      0x88180166,  //  000E  GETMBR	R6	R0	K102
      0x8C180D1D,  //  000F  GETMET	R6	R6	K29
      0xB8220000,  //  0010  GETNGBL	R8	K0
      0x8C201140,  //  0011  GETMET	R8	R8	K64
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
    ( &(const binstruction[158]) {  /* code */
      0xB80E0000,  //  0000  GETNGBL	R3	K0
      0x8C0C07C3,  //  0001  GETMET	R3	R3	K195
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C0702,  //  0003  GETMET	R3	R3	K2
      0x5C140400,  //  0004  MOVE	R5	R2
      0x7C0C0400,  //  0005  CALL	R3	2
      0xB8120000,  //  0006  GETNGBL	R4	K0
      0x8C100971,  //  0007  GETMET	R4	R4	K113
      0x7C100200,  //  0008  CALL	R4	1
      0x881407BE,  //  0009  GETMBR	R5	R3	K190
      0x881807BB,  //  000A  GETMBR	R6	R3	K187
      0x8C1C0156,  //  000B  GETMET	R7	R0	K86
      0x5C240200,  //  000C  MOVE	R9	R1
      0x5C280C00,  //  000D  MOVE	R10	R6
      0x7C1C0600,  //  000E  CALL	R7	3
      0x741E0006,  //  000F  JMPT	R7	#0017
      0x8C1C010D,  //  0010  GETMET	R7	R0	K13
      0x5C240200,  //  0011  MOVE	R9	R1
      0xB82A0000,  //  0012  GETNGBL	R10	K0
      0x88281557,  //  0013  GETMBR	R10	R10	K87
      0x7C1C0600,  //  0014  CALL	R7	3
      0x501C0200,  //  0015  LDBOOL	R7	1	0
      0x80040E00,  //  0016  RET	1	R7
      0x881C07C4,  //  0017  GETMBR	R7	R3	K196
      0x4C200000,  //  0018  LDNIL	R8
      0x201C0E08,  //  0019  NE	R7	R7	R8
      0x781E0080,  //  001A  JMPF	R7	#009C
      0xB81E0000,  //  001B  GETNGBL	R7	K0
      0x8C1C0FC5,  //  001C  GETMET	R7	R7	K197
      0x7C1C0200,  //  001D  CALL	R7	1
      0x60200012,  //  001E  GETGBL	R8	G18
      0x7C200000,  //  001F  CALL	R8	0
      0x901E3808,  //  0020  SETMBR	R7	K28	R8
      0xB8220000,  //  0021  GETNGBL	R8	K0
      0x8C2011B8,  //  0022  GETMET	R8	R8	K184
      0x88280138,  //  0023  GETMBR	R10	R0	K56
      0x7C200400,  //  0024  CALL	R8	2
      0x60240010,  //  0025  GETGBL	R9	G16
      0x882807C4,  //  0026  GETMBR	R10	R3	K196
      0x7C240200,  //  0027  CALL	R9	1
      0xA8020065,  //  0028  EXBLK	0	#008F
      0x5C281200,  //  0029  MOVE	R10	R9
      0x7C280000,  //  002A  CALL	R10	0
      0x882C1518,  //  002B  GETMBR	R11	R10	K24
      0x883015C6,  //  002C  GETMBR	R12	R10	K198
      0x8C3409C7,  //  002D  GETMET	R13	R4	K199
      0x5C3C1600,  //  002E  MOVE	R15	R11
      0x7C340400,  //  002F  CALL	R13	2
      0x88341711,  //  0030  GETMBR	R13	R11	K17
      0x4C380000,  //  0031  LDNIL	R14
      0x1C341A0E,  //  0032  EQ	R13	R13	R14
      0x74360003,  //  0033  JMPT	R13	#0038
      0x88341712,  //  0034  GETMBR	R13	R11	K18
      0x4C380000,  //  0035  LDNIL	R14
      0x1C341A0E,  //  0036  EQ	R13	R13	R14
      0x78360008,  //  0037  JMPF	R13	#0041
      0xB8360000,  //  0038  GETNGBL	R13	K0
      0x88341B36,  //  0039  GETMBR	R13	R13	K54
      0x90122C0D,  //  003A  SETMBR	R4	K22	R13
      0x8C3401C8,  //  003B  GETMET	R13	R0	K200
      0x5C3C0E00,  //  003C  MOVE	R15	R7
      0x5C400800,  //  003D  MOVE	R16	R4
      0x4C440000,  //  003E  LDNIL	R17
      0x7C340800,  //  003F  CALL	R13	4
      0x7001FFE7,  //  0040  JMP		#0029
      0x8834171B,  //  0041  GETMBR	R13	R11	K27
      0x4C380000,  //  0042  LDNIL	R14
      0x1C341A0E,  //  0043  EQ	R13	R13	R14
      0x78360016,  //  0044  JMPF	R13	#005C
      0xB8361600,  //  0045  GETNGBL	R13	K11
      0x8C341B21,  //  0046  GETMET	R13	R13	K33
      0x583C0008,  //  0047  LDCONST	R15	K8
      0x7C340400,  //  0048  CALL	R13	2
      0x78360011,  //  0049  JMPF	R13	#005C
      0xB8360000,  //  004A  GETNGBL	R13	K0
      0x8C341B10,  //  004B  GETMET	R13	R13	K16
      0x883C1711,  //  004C  GETMBR	R15	R11	K17
      0x88401712,  //  004D  GETMBR	R16	R11	K18
      0x7C340600,  //  004E  CALL	R13	3
      0xB83A0600,  //  004F  GETNGBL	R14	K3
      0x603C0008,  //  0050  GETGBL	R15	G8
      0x5C400800,  //  0051  MOVE	R16	R4
      0x7C3C0200,  //  0052  CALL	R15	1
      0x003F920F,  //  0053  ADD	R15	K201	R15
      0x78360002,  //  0054  JMPF	R13	#0058
      0x0042260D,  //  0055  ADD	R16	K19	R13
      0x00402114,  //  0056  ADD	R16	R16	K20
      0x70020000,  //  0057  JMP		#0059
      0x58400015,  //  0058  LDCONST	R16	K21
      0x003C1E10,  //  0059  ADD	R15	R15	R16
      0x58400008,  //  005A  LDCONST	R16	K8
      0x7C380400,  //  005B  CALL	R14	2
      0x8C34118F,  //  005C  GETMET	R13	R8	K143
      0x883C171B,  //  005D  GETMBR	R15	R11	K27
      0x88401711,  //  005E  GETMBR	R16	R11	K17
      0x88441712,  //  005F  GETMBR	R17	R11	K18
      0x7C340800,  //  0060  CALL	R13	4
      0x8C3411CA,  //  0061  GETMET	R13	R8	K202
      0x7C340200,  //  0062  CALL	R13	1
      0x4C380000,  //  0063  LDNIL	R14
      0x8C3C11CB,  //  0064  GETMET	R15	R8	K203
      0x7C3C0200,  //  0065  CALL	R15	1
      0x5C381E00,  //  0066  MOVE	R14	R15
      0x783E0025,  //  0067  JMPF	R15	#008E
      0x903A6E01,  //  0068  SETMBR	R14	K55	R1
      0x883C1D16,  //  0069  GETMBR	R15	R14	K22
      0x4C400000,  //  006A  LDNIL	R16
      0x203C1E10,  //  006B  NE	R15	R15	R16
      0x783E0007,  //  006C  JMPF	R15	#0075
      0x4C3C0000,  //  006D  LDNIL	R15
      0x903A2C0F,  //  006E  SETMBR	R14	K22	R15
      0x8C3C01C8,  //  006F  GETMET	R15	R0	K200
      0x5C440E00,  //  0070  MOVE	R17	R7
      0x5C481C00,  //  0071  MOVE	R18	R14
      0x5C4C1800,  //  0072  MOVE	R19	R12
      0x7C3C0800,  //  0073  CALL	R15	4
      0x70020017,  //  0074  JMP		#008D
      0x8C3C11CC,  //  0075  GETMET	R15	R8	K204
      0x7C3C0200,  //  0076  CALL	R15	1
      0xB8420000,  //  0077  GETNGBL	R16	K0
      0x884021CD,  //  0078  GETMBR	R16	R16	K205
      0x903A2C10,  //  0079  SETMBR	R14	K22	R16
      0x4C400000,  //  007A  LDNIL	R16
      0x20401E10,  //  007B  NE	R16	R15	R16
      0x78420005,  //  007C  JMPF	R16	#0083
      0x8C401FCE,  //  007D  GETMET	R16	R15	K206
      0x88480305,  //  007E  GETMBR	R18	R1	K5
      0x5C4C1C00,  //  007F  MOVE	R19	R14
      0x5C501800,  //  0080  MOVE	R20	R12
      0x7C400800,  //  0081  CALL	R16	4
      0x70020000,  //  0082  JMP		#0084
      0x4C400000,  //  0083  LDNIL	R16
      0x78420002,  //  0084  JMPF	R16	#0088
      0xB8460000,  //  0085  GETNGBL	R17	K0
      0x8844230E,  //  0086  GETMBR	R17	R17	K14
      0x903A2C11,  //  0087  SETMBR	R14	K22	R17
      0x8C4401C8,  //  0088  GETMET	R17	R0	K200
      0x5C4C0E00,  //  0089  MOVE	R19	R7
      0x5C501C00,  //  008A  MOVE	R20	R14
      0x5C541800,  //  008B  MOVE	R21	R12
      0x7C440800,  //  008C  CALL	R17	4
      0x7001FFD5,  //  008D  JMP		#0064
      0x7001FF99,  //  008E  JMP		#0029
      0x58240072,  //  008F  LDCONST	R9	K114
      0xAC240200,  //  0090  CATCH	R9	1	0
      0xB0080000,  //  0091  RAISE	2	R0	R0
      0x5C240A00,  //  0092  MOVE	R9	R5
      0x74260007,  //  0093  JMPT	R9	#009C
      0x88240166,  //  0094  GETMBR	R9	R0	K102
      0x8C24131D,  //  0095  GETMET	R9	R9	K29
      0xB82E0000,  //  0096  GETNGBL	R11	K0
      0x8C2C17CF,  //  0097  GETMET	R11	R11	K207
      0x5C340200,  //  0098  MOVE	R13	R1
      0x5C380E00,  //  0099  MOVE	R14	R7
      0x7C2C0600,  //  009A  CALL	R11	3
      0x7C240400,  //  009B  CALL	R9	2
      0x501C0200,  //  009C  LDBOOL	R7	1	0
      0x80040E00,  //  009D  RET	1	R7
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
      0x8C0401D0,  //  0000  GETMET	R1	R0	K208
      0x7C040200,  //  0001  CALL	R1	1
      0x6004000C,  //  0002  GETGBL	R1	G12
      0x88080109,  //  0003  GETMBR	R2	R0	K9
      0x7C040200,  //  0004  CALL	R1	1
      0x2404031F,  //  0005  GT	R1	R1	K31
      0x78060024,  //  0006  JMPF	R1	#002C
      0x60040012,  //  0007  GETGBL	R1	G18
      0x7C040000,  //  0008  CALL	R1	0
      0x60080010,  //  0009  GETGBL	R2	G16
      0x880C0109,  //  000A  GETMBR	R3	R0	K9
      0x8C0C07D1,  //  000B  GETMET	R3	R3	K209
      0x7C0C0200,  //  000C  CALL	R3	1
      0x7C080200,  //  000D  CALL	R2	1
      0xA802000B,  //  000E  EXBLK	0	#001B
      0x5C0C0400,  //  000F  MOVE	R3	R2
      0x7C0C0000,  //  0010  CALL	R3	0
      0xB8121600,  //  0011  GETNGBL	R4	K11
      0x8C1009B0,  //  0012  GETMET	R4	R4	K176
      0x88180109,  //  0013  GETMBR	R6	R0	K9
      0x94180C03,  //  0014  GETIDX	R6	R6	R3
      0x7C100400,  //  0015  CALL	R4	2
      0x78120002,  //  0016  JMPF	R4	#001A
      0x8C10031D,  //  0017  GETMET	R4	R1	K29
      0x5C180600,  //  0018  MOVE	R6	R3
      0x7C100400,  //  0019  CALL	R4	2
      0x7001FFF3,  //  001A  JMP		#000F
      0x58080072,  //  001B  LDCONST	R2	K114
      0xAC080200,  //  001C  CATCH	R2	1	0
      0xB0080000,  //  001D  RAISE	2	R0	R0
      0x60080010,  //  001E  GETGBL	R2	G16
      0x5C0C0200,  //  001F  MOVE	R3	R1
      0x7C080200,  //  0020  CALL	R2	1
      0xA8020006,  //  0021  EXBLK	0	#0029
      0x5C0C0400,  //  0022  MOVE	R3	R2
      0x7C0C0000,  //  0023  CALL	R3	0
      0x88100109,  //  0024  GETMBR	R4	R0	K9
      0x8C1009AE,  //  0025  GETMET	R4	R4	K174
      0x5C180600,  //  0026  MOVE	R6	R3
      0x7C100400,  //  0027  CALL	R4	2
      0x7001FFF8,  //  0028  JMP		#0022
      0x58080072,  //  0029  LDCONST	R2	K114
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
      0x8C100343,  //  0000  GETMET	R4	R1	K67
      0x541A0014,  //  0001  LDINT	R6	21
      0x581C002C,  //  0002  LDCONST	R7	K44
      0x7C100600,  //  0003  CALL	R4	3
      0x8C100343,  //  0004  GETMET	R4	R1	K67
      0x541A34FF,  //  0005  LDINT	R6	13568
      0x541DFFFD,  //  0006  LDINT	R7	-2
      0x7C100600,  //  0007  CALL	R4	3
      0x8C1001D2,  //  0008  GETMET	R4	R0	K210
      0x5C180200,  //  0009  MOVE	R6	R1
      0x5C1C0400,  //  000A  MOVE	R7	R2
      0x5820001F,  //  000B  LDCONST	R8	K31
      0x7C100800,  //  000C  CALL	R4	4
      0x8C100343,  //  000D  GETMET	R4	R1	K67
      0x541A3500,  //  000E  LDINT	R6	13569
      0x541DFFFD,  //  000F  LDINT	R7	-2
      0x7C100600,  //  0010  CALL	R4	3
      0x88100516,  //  0011  GETMBR	R4	R2	K22
      0x541600FE,  //  0012  LDINT	R5	255
      0x18100805,  //  0013  LE	R4	R4	R5
      0x78120008,  //  0014  JMPF	R4	#001E
      0x8C100343,  //  0015  GETMET	R4	R1	K67
      0x541A23FF,  //  0016  LDINT	R6	9216
      0x541DFFFD,  //  0017  LDINT	R7	-2
      0x7C100600,  //  0018  CALL	R4	3
      0x8C100343,  //  0019  GETMET	R4	R1	K67
      0x88180516,  //  001A  GETMBR	R6	R2	K22
      0x581C002C,  //  001B  LDCONST	R7	K44
      0x7C100600,  //  001C  CALL	R4	3
      0x70020007,  //  001D  JMP		#0026
      0x8C100343,  //  001E  GETMET	R4	R1	K67
      0x541A24FF,  //  001F  LDINT	R6	9472
      0x541DFFFD,  //  0020  LDINT	R7	-2
      0x7C100600,  //  0021  CALL	R4	3
      0x8C100343,  //  0022  GETMET	R4	R1	K67
      0x88180516,  //  0023  GETMBR	R6	R2	K22
      0x581C0020,  //  0024  LDCONST	R7	K32
      0x7C100600,  //  0025  CALL	R4	3
      0x8C100343,  //  0026  GETMET	R4	R1	K67
      0x541A1817,  //  0027  LDINT	R6	6168
      0x541DFFFD,  //  0028  LDINT	R7	-2
      0x7C100600,  //  0029  CALL	R4	3
      0x8C100343,  //  002A  GETMET	R4	R1	K67
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
      0x880805A6,  //  0005  GETMBR	R2	R2	K166
      0x8C0805D3,  //  0006  GETMET	R2	R2	K211
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
      0x8C140343,  //  0000  GETMET	R5	R1	K67
      0x581C00D4,  //  0001  LDCONST	R7	K212
      0x5421FFFB,  //  0002  LDINT	R8	-4
      0x7C140600,  //  0003  CALL	R5	3
      0x8C140343,  //  0004  GETMET	R5	R1	K67
      0x581C002C,  //  0005  LDCONST	R7	K44
      0x5421FFFD,  //  0006  LDINT	R8	-2
      0x7C140600,  //  0007  CALL	R5	3
      0x8C1401D2,  //  0008  GETMET	R5	R0	K210
      0x5C1C0200,  //  0009  MOVE	R7	R1
      0x5C200400,  //  000A  MOVE	R8	R2
      0x5824002C,  //  000B  LDCONST	R9	K44
      0x5C280800,  //  000C  MOVE	R10	R4
      0x7C140A00,  //  000D  CALL	R5	5
      0x900EFD20,  //  000E  SETMBR	R3	K126	K32
      0x8C14077F,  //  000F  GETMET	R5	R3	K127
      0x5C1C0200,  //  0010  MOVE	R7	R1
      0x7C140400,  //  0011  CALL	R5	2
      0x8C140343,  //  0012  GETMET	R5	R1	K67
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
