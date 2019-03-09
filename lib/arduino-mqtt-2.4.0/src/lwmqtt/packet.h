#ifndef LWMQTT_PACKET_H
#define LWMQTT_PACKET_H

#include "helpers.h"

/**
 * The available packet types.
 */
typedef enum {
  LWMQTT_NO_PACKET = 0,
  LWMQTT_CONNECT_PACKET = 1,
  LWMQTT_CONNACK_PACKET,
  LWMQTT_PUBLISH_PACKET,
  LWMQTT_PUBACK_PACKET,
  LWMQTT_PUBREC_PACKET,
  LWMQTT_PUBREL_PACKET,
  LWMQTT_PUBCOMP_PACKET,
  LWMQTT_SUBSCRIBE_PACKET,
  LWMQTT_SUBACK_PACKET,
  LWMQTT_UNSUBSCRIBE_PACKET,
  LWMQTT_UNSUBACK_PACKET,
  LWMQTT_PINGREQ_PACKET,
  LWMQTT_PINGRESP_PACKET,
  LWMQTT_DISCONNECT_PACKET
} lwmqtt_packet_type_t;

/**
 * Will detect the packet type from the at least one byte long buffer.
 *
 * @param buf - The buffer from which the packet type will be detected.
 * @param buf_len - The length of the specified buffer.
 * @param packet_type - The packet type.
 * @return An error value.
 */
lwmqtt_err_t lwmqtt_detect_packet_type(uint8_t *buf, size_t buf_len, lwmqtt_packet_type_t *packet_type);

/**
 * Will detect the remaining length form the at least on byte long buffer.
 *
 * It will return LWMQTT_BUFFER_TOO_SHORT if the buffer is to short and an additional byte should be read from the
 * network. In case the remaining length is overflowed it will return LWMQTT_REMAINING_LENGTH_OVERFLOW.
 *
 * @param buf - The buffer from which the remaining length will be detected.
 * @param buf_len - The length of the specified buffer.
 * @param rem_len - The detected remaining length.
 * @return An error value.
 */
lwmqtt_err_t lwmqtt_detect_remaining_length(uint8_t *buf, size_t buf_len, uint32_t *rem_len);

/**
 * Encodes a connect packet into the supplied buffer.
 *
 * @param buf - The buffer into which the packet will be encoded.
 * @param buf_len - The length of the specified buffer.
 * @param len - The encoded length of the packet.
 * @param options - The options to be used to build the connect packet.
 * @param will - The last will and testament.
 * @return An error value.
 */
lwmqtt_err_t lwmqtt_encode_connect(uint8_t *buf, size_t buf_len, size_t *len, lwmqtt_options_t options,
                                   lwmqtt_will_t *will);

/**
 * Decodes a connack packet from the supplied buffer.
 *
 * @param buf - The raw buffer data.
 * @param buf_len - The length of the specified buffer.
 * @param session_present - The session present flag.
 * @param return_code - The return code.
 * @return An error value.
 */
lwmqtt_err_t lwmqtt_decode_connack(uint8_t *buf, size_t buf_len, bool *session_present,
                                   lwmqtt_return_code_t *return_code);

/**
 * Encodes a zero (disconnect, pingreq) packet into the supplied buffer.
 *
 * @param buf - The buffer into which the packet will be encoded.
 * @param buf_len - The length of the specified buffer.
 * @param len - The encoded length of the packet.
 * @param packet_type - The packets type.
 * @return An error value.
 */
lwmqtt_err_t lwmqtt_encode_zero(uint8_t *buf, size_t buf_len, size_t *len, lwmqtt_packet_type_t packet_type);

/**
 * Decodes an ack (puback, pubrec, pubrel, pubcomp, unsuback) packet from the supplied buffer.
 *
 * @param buf - The raw buffer data.
 * @param buf_len - The length of the specified buffer.
 * @param packet_type - The packet type.
 * @param dup - The dup flag.
 * @param packet_id - The packet id.
 * @return An error value.
 */
lwmqtt_err_t lwmqtt_decode_ack(uint8_t *buf, size_t buf_len, lwmqtt_packet_type_t packet_type, bool *dup,
                               uint16_t *packet_id);

/**
 * Encodes an ack (puback, pubrec, pubrel, pubcomp) packet into the supplied buffer.
 *
 * @param buf - The buffer into which the packet will be encoded.
 * @param buf_len - The length of the specified buffer.
 * @param len - The encoded length of the packet.
 * @param packet_type - The packets type.
 * @param dup - The dup flag.
 * @param packet_id - The packet id.
 * @return An error value.
 */
lwmqtt_err_t lwmqtt_encode_ack(uint8_t *buf, size_t buf_len, size_t *len, lwmqtt_packet_type_t packet_type, bool dup,
                               uint16_t packet_id);

/**
 * Decodes a publish packet from the supplied buffer.
 *
 * @param buf - The raw buffer data.
 * @param buf_len - The length of the specified buffer.
 * @param dup - The dup flag.
 * @param packet_id  - The packet id.
 * @param topic - The topic.
 * @parma msg - The message.
 * @return An error value.
 */
lwmqtt_err_t lwmqtt_decode_publish(uint8_t *buf, size_t buf_len, bool *dup, uint16_t *packet_id, lwmqtt_string_t *topic,
                                   lwmqtt_message_t *msg);

/**
 * Encodes a publish packet into the supplied buffer.
 *
 * @param buf - The buffer into which the packet will be encoded.
 * @param buf_len - The length of the specified buffer.
 * @param len - The encoded length of the packet.
 * @param dup - The dup flag.
 * @param packet_id  - The packet id.
 * @param topic - The topic.
 * @param msg - The message.
 * @return An error value.
 */
lwmqtt_err_t lwmqtt_encode_publish(uint8_t *buf, size_t buf_len, size_t *len, bool dup, uint16_t packet_id,
                                   lwmqtt_string_t topic, lwmqtt_message_t msg);

/**
 * Encodes a subscribe packet into the supplied buffer.
 *
 * @param buf - The buffer into which the packet will be encoded.
 * @param buf_len - The length of the specified buffer.
 * @param len - The encoded length of the packet.
 * @param packet_id - The packet id.
 * @param count - The number of members in the topic_filters and qos_levels array.
 * @param topic_filters - The array of topic filter.
 * @param qos_levels - The array of requested QoS levels.
 * @return An error value.
 */
lwmqtt_err_t lwmqtt_encode_subscribe(uint8_t *buf, size_t buf_len, size_t *len, uint16_t packet_id, int count,
                                     lwmqtt_string_t *topic_filters, lwmqtt_qos_t *qos_levels);

/**
 * Decodes a suback packet from the supplied buffer.
 *
 * @param buf - The raw buffer data.
 * @param buf_len - The length of the specified buffer.
 * @param packet_id - The packet id.
 * @param max_count - The maximum number of members allowed in the granted_qos_levels array.
 * @param count - The number of members in the granted_qos_levels array.
 * @param granted_qos_levels - The granted QoS levels.
 * @return An error value.
 */
lwmqtt_err_t lwmqtt_decode_suback(uint8_t *buf, size_t buf_len, uint16_t *packet_id, int max_count, int *count,
                                  lwmqtt_qos_t *granted_qos_levels);

/**
 * Encodes the supplied unsubscribe data into the supplied buffer, ready for sending
 *
 * @param buf - The buffer into which the packet will be encoded.
 * @param buf_len - The length of the specified buffer.
 * @param len - The encoded length of the packet.
 * @param packet_id - The packet id.
 * @param count - The number of members in the topic_filters array.
 * @param topic_filters - The array of topic filters.
 * @return An error value.
 */
lwmqtt_err_t lwmqtt_encode_unsubscribe(uint8_t *buf, size_t buf_len, size_t *len, uint16_t packet_id, int count,
                                       lwmqtt_string_t *topic_filters);

#endif  // LWMQTT_PACKET_H
