/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: BmsModeConfig.proto */

/* Do not generate deprecated warnings for self */
#ifndef PROTOBUF_C__NO_DEPRECATED
#define PROTOBUF_C__NO_DEPRECATED
#endif

#include "BmsModeConfig.pb-c.h"
void   bms_mode_config__soc__config__init
                     (BmsModeConfig__SOCConfig         *message)
{
  static BmsModeConfig__SOCConfig init_value = BMS_MODE_CONFIG__SOC__CONFIG__INIT;
  *message = init_value;
}
void   bms_mode_config__fan__config__init
                     (BmsModeConfig__FanConfig         *message)
{
  static BmsModeConfig__FanConfig init_value = BMS_MODE_CONFIG__FAN__CONFIG__INIT;
  *message = init_value;
}
void   bms_mode_config__nomal__config__init
                     (BmsModeConfig__NomalConfig         *message)
{
  static BmsModeConfig__NomalConfig init_value = BMS_MODE_CONFIG__NOMAL__CONFIG__INIT;
  *message = init_value;
}
void   bms_mode_config__init
                     (BmsModeConfig         *message)
{
  static BmsModeConfig init_value = BMS_MODE_CONFIG__INIT;
  *message = init_value;
}
size_t bms_mode_config__get_packed_size
                     (const BmsModeConfig *message)
{
  assert(message->base.descriptor == &bms_mode_config__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t bms_mode_config__pack
                     (const BmsModeConfig *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &bms_mode_config__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t bms_mode_config__pack_to_buffer
                     (const BmsModeConfig *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &bms_mode_config__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
BmsModeConfig *
       bms_mode_config__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (BmsModeConfig *)
     protobuf_c_message_unpack (&bms_mode_config__descriptor,
                                allocator, len, data);
}
void   bms_mode_config__free_unpacked
                     (BmsModeConfig *message,
                      ProtobufCAllocator *allocator)
{
  assert(message->base.descriptor == &bms_mode_config__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
static const ProtobufCFieldDescriptor bms_mode_config__soc__config__field_descriptors[2] =
{
  {
    "Full_Soc",
    1,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_INT32,
    offsetof(BmsModeConfig__SOCConfig, has_full_soc),
    offsetof(BmsModeConfig__SOCConfig, full_soc),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "Current_Soc",
    2,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_INT32,
    offsetof(BmsModeConfig__SOCConfig, has_current_soc),
    offsetof(BmsModeConfig__SOCConfig, current_soc),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned bms_mode_config__soc__config__field_indices_by_name[] = {
  1,   /* field[1] = Current_Soc */
  0,   /* field[0] = Full_Soc */
};
static const ProtobufCIntRange bms_mode_config__soc__config__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 2 }
};
const ProtobufCMessageDescriptor bms_mode_config__soc__config__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "BmsModeConfig.SOC_Config",
  "SOCConfig",
  "BmsModeConfig__SOCConfig",
  "",
  sizeof(BmsModeConfig__SOCConfig),
  2,
  bms_mode_config__soc__config__field_descriptors,
  bms_mode_config__soc__config__field_indices_by_name,
  1,  bms_mode_config__soc__config__number_ranges,
  (ProtobufCMessageInit) bms_mode_config__soc__config__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCFieldDescriptor bms_mode_config__fan__config__field_descriptors[2] =
{
  {
    "open",
    1,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_INT32,
    offsetof(BmsModeConfig__FanConfig, has_open),
    offsetof(BmsModeConfig__FanConfig, open),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "close",
    2,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_INT32,
    offsetof(BmsModeConfig__FanConfig, has_close),
    offsetof(BmsModeConfig__FanConfig, close),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned bms_mode_config__fan__config__field_indices_by_name[] = {
  1,   /* field[1] = close */
  0,   /* field[0] = open */
};
static const ProtobufCIntRange bms_mode_config__fan__config__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 2 }
};
const ProtobufCMessageDescriptor bms_mode_config__fan__config__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "BmsModeConfig.Fan_Config",
  "FanConfig",
  "BmsModeConfig__FanConfig",
  "",
  sizeof(BmsModeConfig__FanConfig),
  2,
  bms_mode_config__fan__config__field_descriptors,
  bms_mode_config__fan__config__field_indices_by_name,
  1,  bms_mode_config__fan__config__number_ranges,
  (ProtobufCMessageInit) bms_mode_config__fan__config__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCFieldDescriptor bms_mode_config__nomal__config__field_descriptors[5] =
{
  {
    "Ctrl_charge",
    1,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_INT32,
    offsetof(BmsModeConfig__NomalConfig, has_ctrl_charge),
    offsetof(BmsModeConfig__NomalConfig, ctrl_charge),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "Ctrl_fan",
    2,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_INT32,
    offsetof(BmsModeConfig__NomalConfig, has_ctrl_fan),
    offsetof(BmsModeConfig__NomalConfig, ctrl_fan),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "Ctrl_reset",
    3,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_INT32,
    offsetof(BmsModeConfig__NomalConfig, has_ctrl_reset),
    offsetof(BmsModeConfig__NomalConfig, ctrl_reset),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "Alarm_release",
    4,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_INT32,
    offsetof(BmsModeConfig__NomalConfig, has_alarm_release),
    offsetof(BmsModeConfig__NomalConfig, alarm_release),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "Ctrl_motor_power",
    5,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_INT32,
    offsetof(BmsModeConfig__NomalConfig, has_ctrl_motor_power),
    offsetof(BmsModeConfig__NomalConfig, ctrl_motor_power),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned bms_mode_config__nomal__config__field_indices_by_name[] = {
  3,   /* field[3] = Alarm_release */
  0,   /* field[0] = Ctrl_charge */
  1,   /* field[1] = Ctrl_fan */
  4,   /* field[4] = Ctrl_motor_power */
  2,   /* field[2] = Ctrl_reset */
};
static const ProtobufCIntRange bms_mode_config__nomal__config__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 5 }
};
const ProtobufCMessageDescriptor bms_mode_config__nomal__config__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "BmsModeConfig.Nomal_Config",
  "NomalConfig",
  "BmsModeConfig__NomalConfig",
  "",
  sizeof(BmsModeConfig__NomalConfig),
  5,
  bms_mode_config__nomal__config__field_descriptors,
  bms_mode_config__nomal__config__field_indices_by_name,
  1,  bms_mode_config__nomal__config__number_ranges,
  (ProtobufCMessageInit) bms_mode_config__nomal__config__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCFieldDescriptor bms_mode_config__field_descriptors[3] =
{
  {
    "Nomal",
    1,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_MESSAGE,
    0,   /* quantifier_offset */
    offsetof(BmsModeConfig, nomal),
    &bms_mode_config__nomal__config__descriptor,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "SOC",
    2,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_MESSAGE,
    0,   /* quantifier_offset */
    offsetof(BmsModeConfig, soc),
    &bms_mode_config__soc__config__descriptor,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "fan",
    3,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_MESSAGE,
    0,   /* quantifier_offset */
    offsetof(BmsModeConfig, fan),
    &bms_mode_config__fan__config__descriptor,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned bms_mode_config__field_indices_by_name[] = {
  0,   /* field[0] = Nomal */
  1,   /* field[1] = SOC */
  2,   /* field[2] = fan */
};
static const ProtobufCIntRange bms_mode_config__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 3 }
};
const ProtobufCMessageDescriptor bms_mode_config__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "BmsModeConfig",
  "BmsModeConfig",
  "BmsModeConfig",
  "",
  sizeof(BmsModeConfig),
  3,
  bms_mode_config__field_descriptors,
  bms_mode_config__field_indices_by_name,
  1,  bms_mode_config__number_ranges,
  (ProtobufCMessageInit) bms_mode_config__init,
  NULL,NULL,NULL    /* reserved[123] */
};
