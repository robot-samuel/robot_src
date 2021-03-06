/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: AlarmMessage.proto */

#ifndef PROTOBUF_C_AlarmMessage_2eproto__INCLUDED
#define PROTOBUF_C_AlarmMessage_2eproto__INCLUDED

#include <protobuf-c/protobuf-c.h>

PROTOBUF_C__BEGIN_DECLS

#if PROTOBUF_C_VERSION_NUMBER < 1000000
# error This file was generated by a newer version of protoc-c which is incompatible with your libprotobuf-c headers. Please update your headers.
#elif 1002001 < PROTOBUF_C_MIN_COMPILER_VERSION
# error This file was generated by an older version of protoc-c which is incompatible with your libprotobuf-c headers. Please regenerate this file with a newer version of protoc-c.
#endif


typedef struct _AlarmMessage AlarmMessage;


/* --- enums --- */


/* --- messages --- */

struct  _AlarmMessage
{
  ProtobufCMessage base;
  /*
   *控制箱高温警告
   */
  protobuf_c_boolean has_ctrolboxhightemalarm;
  protobuf_c_boolean ctrolboxhightemalarm;
  /*
   *控制箱低温警告
   */
  protobuf_c_boolean has_ctrolboxfrozentemalarm;
  protobuf_c_boolean ctrolboxfrozentemalarm;
  /*
   *电源管理模块离线告警
   */
  protobuf_c_boolean has_bmsmodcomlostalarm;
  protobuf_c_boolean bmsmodcomlostalarm;
  /*
   *急停开关使能告警
   */
  protobuf_c_boolean has_emergency_status_stop;
  protobuf_c_boolean emergency_status_stop;
  /*
   *外围传感器软模块离线
   */
  protobuf_c_boolean has_ecanexternsnsorndeoff;
  protobuf_c_boolean ecanexternsnsorndeoff;
  /*
   *zmq通信软模块离线
   */
  protobuf_c_boolean has_zmqtransnodeoff;
  protobuf_c_boolean zmqtransnodeoff;
  /*
   *遥控手柄软模块离线
   */
  protobuf_c_boolean has_joytestnodeoff;
  protobuf_c_boolean joytestnodeoff;
};
#define ALARM_MESSAGE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&alarm_message__descriptor) \
    , 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0 }


/* AlarmMessage methods */
void   alarm_message__init
                     (AlarmMessage         *message);
size_t alarm_message__get_packed_size
                     (const AlarmMessage   *message);
size_t alarm_message__pack
                     (const AlarmMessage   *message,
                      uint8_t             *out);
size_t alarm_message__pack_to_buffer
                     (const AlarmMessage   *message,
                      ProtobufCBuffer     *buffer);
AlarmMessage *
       alarm_message__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   alarm_message__free_unpacked
                     (AlarmMessage *message,
                      ProtobufCAllocator *allocator);
/* --- per-message closures --- */

typedef void (*AlarmMessage_Closure)
                 (const AlarmMessage *message,
                  void *closure_data);

/* --- services --- */


/* --- descriptors --- */

extern const ProtobufCMessageDescriptor alarm_message__descriptor;

PROTOBUF_C__END_DECLS


#endif  /* PROTOBUF_C_AlarmMessage_2eproto__INCLUDED */
