/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: planetmintgo/machine/params.proto */

/* Do not generate deprecated warnings for self */
#ifndef PROTOBUF_C__NO_DEPRECATED
#define PROTOBUF_C__NO_DEPRECATED
#endif

#include "planetmintgo/machine/params.pb-c.h"
void   planetmintgo__machine__params__init
                     (Planetmintgo__Machine__Params         *message)
{
  static const Planetmintgo__Machine__Params init_value = PLANETMINTGO__MACHINE__PARAMS__INIT;
  *message = init_value;
}
size_t planetmintgo__machine__params__get_packed_size
                     (const Planetmintgo__Machine__Params *message)
{
  assert(message->base.descriptor == &planetmintgo__machine__params__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t planetmintgo__machine__params__pack
                     (const Planetmintgo__Machine__Params *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &planetmintgo__machine__params__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t planetmintgo__machine__params__pack_to_buffer
                     (const Planetmintgo__Machine__Params *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &planetmintgo__machine__params__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Planetmintgo__Machine__Params *
       planetmintgo__machine__params__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Planetmintgo__Machine__Params *)
     protobuf_c_message_unpack (&planetmintgo__machine__params__descriptor,
                                allocator, len, data);
}
void   planetmintgo__machine__params__free_unpacked
                     (Planetmintgo__Machine__Params *message,
                      ProtobufCAllocator *allocator)
{
  if(!message)
    return;
  assert(message->base.descriptor == &planetmintgo__machine__params__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
#define planetmintgo__machine__params__field_descriptors NULL
#define planetmintgo__machine__params__field_indices_by_name NULL
#define planetmintgo__machine__params__number_ranges NULL
const ProtobufCMessageDescriptor planetmintgo__machine__params__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "planetmintgo.machine.Params",
  "Params",
  "Planetmintgo__Machine__Params",
  "planetmintgo.machine",
  sizeof(Planetmintgo__Machine__Params),
  0,
  planetmintgo__machine__params__field_descriptors,
  planetmintgo__machine__params__field_indices_by_name,
  0,  planetmintgo__machine__params__number_ranges,
  (ProtobufCMessageInit) planetmintgo__machine__params__init,
  NULL,NULL,NULL    /* reserved[123] */
};