/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: cosmos/base/query/v1beta1/pagination.proto */

#ifndef PROTOBUF_C_cosmos_2fbase_2fquery_2fv1beta1_2fpagination_2eproto__INCLUDED
#define PROTOBUF_C_cosmos_2fbase_2fquery_2fv1beta1_2fpagination_2eproto__INCLUDED

#include <protobuf-c/protobuf-c.h>

PROTOBUF_C__BEGIN_DECLS

#if PROTOBUF_C_VERSION_NUMBER < 1003000
# error This file was generated by a newer version of protoc-c which is incompatible with your libprotobuf-c headers. Please update your headers.
#elif 1004001 < PROTOBUF_C_MIN_COMPILER_VERSION
# error This file was generated by an older version of protoc-c which is incompatible with your libprotobuf-c headers. Please regenerate this file with a newer version of protoc-c.
#endif


typedef struct Cosmos__Base__Query__V1beta1__PageRequest Cosmos__Base__Query__V1beta1__PageRequest;
typedef struct Cosmos__Base__Query__V1beta1__PageResponse Cosmos__Base__Query__V1beta1__PageResponse;


/* --- enums --- */


/* --- messages --- */

/*
 * PageRequest is to be embedded in gRPC request messages for efficient
 * pagination. Ex:
 *  message SomeRequest {
 *          Foo some_parameter = 1;
 *          PageRequest pagination = 2;
 *  }
 */
struct  Cosmos__Base__Query__V1beta1__PageRequest
{
  ProtobufCMessage base;
  /*
   * key is a value returned in PageResponse.next_key to begin
   * querying the next page most efficiently. Only one of offset or key
   * should be set.
   */
  ProtobufCBinaryData key;
  /*
   * offset is a numeric offset that can be used when key is unavailable.
   * It is less efficient than using key. Only one of offset or key should
   * be set.
   */
  uint64_t offset;
  /*
   * limit is the total number of results to be returned in the result page.
   * If left empty it will default to a value to be set by each app.
   */
  uint64_t limit;
  /*
   * count_total is set to true  to indicate that the result set should include
   * a count of the total number of items available for pagination in UIs.
   * count_total is only respected when offset is used. It is ignored when key
   * is set.
   */
  protobuf_c_boolean count_total;
  /*
   * reverse is set to true if results are to be returned in the descending order.
   * Since: cosmos-sdk 0.43
   */
  protobuf_c_boolean reverse;
};
#define COSMOS__BASE__QUERY__V1BETA1__PAGE_REQUEST__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&cosmos__base__query__v1beta1__page_request__descriptor) \
    , {0,NULL}, 0, 0, 0, 0 }


/*
 * PageResponse is to be embedded in gRPC response messages where the
 * corresponding request message has used PageRequest.
 *  message SomeResponse {
 *          repeated Bar results = 1;
 *          PageResponse page = 2;
 *  }
 */
struct  Cosmos__Base__Query__V1beta1__PageResponse
{
  ProtobufCMessage base;
  /*
   * next_key is the key to be passed to PageRequest.key to
   * query the next page most efficiently. It will be empty if
   * there are no more results.
   */
  ProtobufCBinaryData next_key;
  /*
   * total is total number of results available if PageRequest.count_total
   * was set, its value is undefined otherwise
   */
  uint64_t total;
};
#define COSMOS__BASE__QUERY__V1BETA1__PAGE_RESPONSE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&cosmos__base__query__v1beta1__page_response__descriptor) \
    , {0,NULL}, 0 }


/* Cosmos__Base__Query__V1beta1__PageRequest methods */
void   cosmos__base__query__v1beta1__page_request__init
                     (Cosmos__Base__Query__V1beta1__PageRequest         *message);
size_t cosmos__base__query__v1beta1__page_request__get_packed_size
                     (const Cosmos__Base__Query__V1beta1__PageRequest   *message);
size_t cosmos__base__query__v1beta1__page_request__pack
                     (const Cosmos__Base__Query__V1beta1__PageRequest   *message,
                      uint8_t             *out);
size_t cosmos__base__query__v1beta1__page_request__pack_to_buffer
                     (const Cosmos__Base__Query__V1beta1__PageRequest   *message,
                      ProtobufCBuffer     *buffer);
Cosmos__Base__Query__V1beta1__PageRequest *
       cosmos__base__query__v1beta1__page_request__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   cosmos__base__query__v1beta1__page_request__free_unpacked
                     (Cosmos__Base__Query__V1beta1__PageRequest *message,
                      ProtobufCAllocator *allocator);
/* Cosmos__Base__Query__V1beta1__PageResponse methods */
void   cosmos__base__query__v1beta1__page_response__init
                     (Cosmos__Base__Query__V1beta1__PageResponse         *message);
size_t cosmos__base__query__v1beta1__page_response__get_packed_size
                     (const Cosmos__Base__Query__V1beta1__PageResponse   *message);
size_t cosmos__base__query__v1beta1__page_response__pack
                     (const Cosmos__Base__Query__V1beta1__PageResponse   *message,
                      uint8_t             *out);
size_t cosmos__base__query__v1beta1__page_response__pack_to_buffer
                     (const Cosmos__Base__Query__V1beta1__PageResponse   *message,
                      ProtobufCBuffer     *buffer);
Cosmos__Base__Query__V1beta1__PageResponse *
       cosmos__base__query__v1beta1__page_response__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   cosmos__base__query__v1beta1__page_response__free_unpacked
                     (Cosmos__Base__Query__V1beta1__PageResponse *message,
                      ProtobufCAllocator *allocator);
/* --- per-message closures --- */

typedef void (*Cosmos__Base__Query__V1beta1__PageRequest_Closure)
                 (const Cosmos__Base__Query__V1beta1__PageRequest *message,
                  void *closure_data);
typedef void (*Cosmos__Base__Query__V1beta1__PageResponse_Closure)
                 (const Cosmos__Base__Query__V1beta1__PageResponse *message,
                  void *closure_data);

/* --- services --- */


/* --- descriptors --- */

extern const ProtobufCMessageDescriptor cosmos__base__query__v1beta1__page_request__descriptor;
extern const ProtobufCMessageDescriptor cosmos__base__query__v1beta1__page_response__descriptor;

PROTOBUF_C__END_DECLS


#endif  /* PROTOBUF_C_cosmos_2fbase_2fquery_2fv1beta1_2fpagination_2eproto__INCLUDED */