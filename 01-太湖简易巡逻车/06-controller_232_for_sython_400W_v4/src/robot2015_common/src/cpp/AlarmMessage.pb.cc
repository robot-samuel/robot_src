// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: AlarmMessage.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "AlarmMessage.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace {

const ::google::protobuf::Descriptor* AlarmMessage_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  AlarmMessage_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_AlarmMessage_2eproto() {
  protobuf_AddDesc_AlarmMessage_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "AlarmMessage.proto");
  GOOGLE_CHECK(file != NULL);
  AlarmMessage_descriptor_ = file->message_type(0);
  static const int AlarmMessage_offsets_[11] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(AlarmMessage, ctrolboxhightemalarm_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(AlarmMessage, ctrolboxfrozentemalarm_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(AlarmMessage, bmsmodcomlostalarm_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(AlarmMessage, emergency_status_stop_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(AlarmMessage, ecanexternsnsorndeoff_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(AlarmMessage, zmqtransnodeoff_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(AlarmMessage, joytestnodeoff_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(AlarmMessage, ph3readnodeoff_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(AlarmMessage, ph3readcomlost_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(AlarmMessage, lasercomlost_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(AlarmMessage, lasernodeoff_),
  };
  AlarmMessage_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      AlarmMessage_descriptor_,
      AlarmMessage::default_instance_,
      AlarmMessage_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(AlarmMessage, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(AlarmMessage, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(AlarmMessage));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_AlarmMessage_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    AlarmMessage_descriptor_, &AlarmMessage::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_AlarmMessage_2eproto() {
  delete AlarmMessage::default_instance_;
  delete AlarmMessage_reflection_;
}

void protobuf_AddDesc_AlarmMessage_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\022AlarmMessage.proto\"\263\002\n\014AlarmMessage\022\034\n"
    "\024CtrolBoxHighTemAlarm\030\001 \001(\010\022\036\n\026CtrolBoxF"
    "rozenTemAlarm\030\002 \001(\010\022\032\n\022BMSModComLostAlar"
    "m\030\003 \001(\010\022\035\n\025Emergency_Status_stop\030\004 \001(\010\022\035"
    "\n\025EcanExternSnsorNdeOFF\030\005 \001(\010\022\027\n\017ZmqTran"
    "sNodeOFF\030\006 \001(\010\022\026\n\016JoyTestNodeOFF\030\007 \001(\010\022\026"
    "\n\016Ph3ReadNodeOFF\030\010 \001(\010\022\026\n\016Ph3ReadComLost"
    "\030\t \001(\010\022\024\n\014LaserComLost\030\n \001(\010\022\024\n\014LaserNod"
    "eOFF\030\013 \001(\010", 330);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "AlarmMessage.proto", &protobuf_RegisterTypes);
  AlarmMessage::default_instance_ = new AlarmMessage();
  AlarmMessage::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_AlarmMessage_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_AlarmMessage_2eproto {
  StaticDescriptorInitializer_AlarmMessage_2eproto() {
    protobuf_AddDesc_AlarmMessage_2eproto();
  }
} static_descriptor_initializer_AlarmMessage_2eproto_;

// ===================================================================

#ifndef _MSC_VER
const int AlarmMessage::kCtrolBoxHighTemAlarmFieldNumber;
const int AlarmMessage::kCtrolBoxFrozenTemAlarmFieldNumber;
const int AlarmMessage::kBMSModComLostAlarmFieldNumber;
const int AlarmMessage::kEmergencyStatusStopFieldNumber;
const int AlarmMessage::kEcanExternSnsorNdeOFFFieldNumber;
const int AlarmMessage::kZmqTransNodeOFFFieldNumber;
const int AlarmMessage::kJoyTestNodeOFFFieldNumber;
const int AlarmMessage::kPh3ReadNodeOFFFieldNumber;
const int AlarmMessage::kPh3ReadComLostFieldNumber;
const int AlarmMessage::kLaserComLostFieldNumber;
const int AlarmMessage::kLaserNodeOFFFieldNumber;
#endif  // !_MSC_VER

AlarmMessage::AlarmMessage()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void AlarmMessage::InitAsDefaultInstance() {
}

AlarmMessage::AlarmMessage(const AlarmMessage& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void AlarmMessage::SharedCtor() {
  _cached_size_ = 0;
  ctrolboxhightemalarm_ = false;
  ctrolboxfrozentemalarm_ = false;
  bmsmodcomlostalarm_ = false;
  emergency_status_stop_ = false;
  ecanexternsnsorndeoff_ = false;
  zmqtransnodeoff_ = false;
  joytestnodeoff_ = false;
  ph3readnodeoff_ = false;
  ph3readcomlost_ = false;
  lasercomlost_ = false;
  lasernodeoff_ = false;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

AlarmMessage::~AlarmMessage() {
  SharedDtor();
}

void AlarmMessage::SharedDtor() {
  if (this != default_instance_) {
  }
}

void AlarmMessage::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* AlarmMessage::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return AlarmMessage_descriptor_;
}

const AlarmMessage& AlarmMessage::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_AlarmMessage_2eproto();
  return *default_instance_;
}

AlarmMessage* AlarmMessage::default_instance_ = NULL;

AlarmMessage* AlarmMessage::New() const {
  return new AlarmMessage;
}

void AlarmMessage::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    ctrolboxhightemalarm_ = false;
    ctrolboxfrozentemalarm_ = false;
    bmsmodcomlostalarm_ = false;
    emergency_status_stop_ = false;
    ecanexternsnsorndeoff_ = false;
    zmqtransnodeoff_ = false;
    joytestnodeoff_ = false;
    ph3readnodeoff_ = false;
  }
  if (_has_bits_[8 / 32] & (0xffu << (8 % 32))) {
    ph3readcomlost_ = false;
    lasercomlost_ = false;
    lasernodeoff_ = false;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool AlarmMessage::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional bool CtrolBoxHighTemAlarm = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   bool, ::google::protobuf::internal::WireFormatLite::TYPE_BOOL>(
                 input, &ctrolboxhightemalarm_)));
          set_has_ctrolboxhightemalarm();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(16)) goto parse_CtrolBoxFrozenTemAlarm;
        break;
      }

      // optional bool CtrolBoxFrozenTemAlarm = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_CtrolBoxFrozenTemAlarm:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   bool, ::google::protobuf::internal::WireFormatLite::TYPE_BOOL>(
                 input, &ctrolboxfrozentemalarm_)));
          set_has_ctrolboxfrozentemalarm();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(24)) goto parse_BMSModComLostAlarm;
        break;
      }

      // optional bool BMSModComLostAlarm = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_BMSModComLostAlarm:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   bool, ::google::protobuf::internal::WireFormatLite::TYPE_BOOL>(
                 input, &bmsmodcomlostalarm_)));
          set_has_bmsmodcomlostalarm();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(32)) goto parse_Emergency_Status_stop;
        break;
      }

      // optional bool Emergency_Status_stop = 4;
      case 4: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_Emergency_Status_stop:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   bool, ::google::protobuf::internal::WireFormatLite::TYPE_BOOL>(
                 input, &emergency_status_stop_)));
          set_has_emergency_status_stop();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(40)) goto parse_EcanExternSnsorNdeOFF;
        break;
      }

      // optional bool EcanExternSnsorNdeOFF = 5;
      case 5: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_EcanExternSnsorNdeOFF:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   bool, ::google::protobuf::internal::WireFormatLite::TYPE_BOOL>(
                 input, &ecanexternsnsorndeoff_)));
          set_has_ecanexternsnsorndeoff();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(48)) goto parse_ZmqTransNodeOFF;
        break;
      }

      // optional bool ZmqTransNodeOFF = 6;
      case 6: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_ZmqTransNodeOFF:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   bool, ::google::protobuf::internal::WireFormatLite::TYPE_BOOL>(
                 input, &zmqtransnodeoff_)));
          set_has_zmqtransnodeoff();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(56)) goto parse_JoyTestNodeOFF;
        break;
      }

      // optional bool JoyTestNodeOFF = 7;
      case 7: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_JoyTestNodeOFF:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   bool, ::google::protobuf::internal::WireFormatLite::TYPE_BOOL>(
                 input, &joytestnodeoff_)));
          set_has_joytestnodeoff();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(64)) goto parse_Ph3ReadNodeOFF;
        break;
      }

      // optional bool Ph3ReadNodeOFF = 8;
      case 8: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_Ph3ReadNodeOFF:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   bool, ::google::protobuf::internal::WireFormatLite::TYPE_BOOL>(
                 input, &ph3readnodeoff_)));
          set_has_ph3readnodeoff();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(72)) goto parse_Ph3ReadComLost;
        break;
      }

      // optional bool Ph3ReadComLost = 9;
      case 9: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_Ph3ReadComLost:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   bool, ::google::protobuf::internal::WireFormatLite::TYPE_BOOL>(
                 input, &ph3readcomlost_)));
          set_has_ph3readcomlost();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(80)) goto parse_LaserComLost;
        break;
      }

      // optional bool LaserComLost = 10;
      case 10: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_LaserComLost:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   bool, ::google::protobuf::internal::WireFormatLite::TYPE_BOOL>(
                 input, &lasercomlost_)));
          set_has_lasercomlost();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(88)) goto parse_LaserNodeOFF;
        break;
      }

      // optional bool LaserNodeOFF = 11;
      case 11: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_LaserNodeOFF:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   bool, ::google::protobuf::internal::WireFormatLite::TYPE_BOOL>(
                 input, &lasernodeoff_)));
          set_has_lasernodeoff();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }

      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void AlarmMessage::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // optional bool CtrolBoxHighTemAlarm = 1;
  if (has_ctrolboxhightemalarm()) {
    ::google::protobuf::internal::WireFormatLite::WriteBool(1, this->ctrolboxhightemalarm(), output);
  }

  // optional bool CtrolBoxFrozenTemAlarm = 2;
  if (has_ctrolboxfrozentemalarm()) {
    ::google::protobuf::internal::WireFormatLite::WriteBool(2, this->ctrolboxfrozentemalarm(), output);
  }

  // optional bool BMSModComLostAlarm = 3;
  if (has_bmsmodcomlostalarm()) {
    ::google::protobuf::internal::WireFormatLite::WriteBool(3, this->bmsmodcomlostalarm(), output);
  }

  // optional bool Emergency_Status_stop = 4;
  if (has_emergency_status_stop()) {
    ::google::protobuf::internal::WireFormatLite::WriteBool(4, this->emergency_status_stop(), output);
  }

  // optional bool EcanExternSnsorNdeOFF = 5;
  if (has_ecanexternsnsorndeoff()) {
    ::google::protobuf::internal::WireFormatLite::WriteBool(5, this->ecanexternsnsorndeoff(), output);
  }

  // optional bool ZmqTransNodeOFF = 6;
  if (has_zmqtransnodeoff()) {
    ::google::protobuf::internal::WireFormatLite::WriteBool(6, this->zmqtransnodeoff(), output);
  }

  // optional bool JoyTestNodeOFF = 7;
  if (has_joytestnodeoff()) {
    ::google::protobuf::internal::WireFormatLite::WriteBool(7, this->joytestnodeoff(), output);
  }

  // optional bool Ph3ReadNodeOFF = 8;
  if (has_ph3readnodeoff()) {
    ::google::protobuf::internal::WireFormatLite::WriteBool(8, this->ph3readnodeoff(), output);
  }

  // optional bool Ph3ReadComLost = 9;
  if (has_ph3readcomlost()) {
    ::google::protobuf::internal::WireFormatLite::WriteBool(9, this->ph3readcomlost(), output);
  }

  // optional bool LaserComLost = 10;
  if (has_lasercomlost()) {
    ::google::protobuf::internal::WireFormatLite::WriteBool(10, this->lasercomlost(), output);
  }

  // optional bool LaserNodeOFF = 11;
  if (has_lasernodeoff()) {
    ::google::protobuf::internal::WireFormatLite::WriteBool(11, this->lasernodeoff(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* AlarmMessage::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // optional bool CtrolBoxHighTemAlarm = 1;
  if (has_ctrolboxhightemalarm()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteBoolToArray(1, this->ctrolboxhightemalarm(), target);
  }

  // optional bool CtrolBoxFrozenTemAlarm = 2;
  if (has_ctrolboxfrozentemalarm()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteBoolToArray(2, this->ctrolboxfrozentemalarm(), target);
  }

  // optional bool BMSModComLostAlarm = 3;
  if (has_bmsmodcomlostalarm()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteBoolToArray(3, this->bmsmodcomlostalarm(), target);
  }

  // optional bool Emergency_Status_stop = 4;
  if (has_emergency_status_stop()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteBoolToArray(4, this->emergency_status_stop(), target);
  }

  // optional bool EcanExternSnsorNdeOFF = 5;
  if (has_ecanexternsnsorndeoff()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteBoolToArray(5, this->ecanexternsnsorndeoff(), target);
  }

  // optional bool ZmqTransNodeOFF = 6;
  if (has_zmqtransnodeoff()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteBoolToArray(6, this->zmqtransnodeoff(), target);
  }

  // optional bool JoyTestNodeOFF = 7;
  if (has_joytestnodeoff()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteBoolToArray(7, this->joytestnodeoff(), target);
  }

  // optional bool Ph3ReadNodeOFF = 8;
  if (has_ph3readnodeoff()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteBoolToArray(8, this->ph3readnodeoff(), target);
  }

  // optional bool Ph3ReadComLost = 9;
  if (has_ph3readcomlost()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteBoolToArray(9, this->ph3readcomlost(), target);
  }

  // optional bool LaserComLost = 10;
  if (has_lasercomlost()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteBoolToArray(10, this->lasercomlost(), target);
  }

  // optional bool LaserNodeOFF = 11;
  if (has_lasernodeoff()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteBoolToArray(11, this->lasernodeoff(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int AlarmMessage::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // optional bool CtrolBoxHighTemAlarm = 1;
    if (has_ctrolboxhightemalarm()) {
      total_size += 1 + 1;
    }

    // optional bool CtrolBoxFrozenTemAlarm = 2;
    if (has_ctrolboxfrozentemalarm()) {
      total_size += 1 + 1;
    }

    // optional bool BMSModComLostAlarm = 3;
    if (has_bmsmodcomlostalarm()) {
      total_size += 1 + 1;
    }

    // optional bool Emergency_Status_stop = 4;
    if (has_emergency_status_stop()) {
      total_size += 1 + 1;
    }

    // optional bool EcanExternSnsorNdeOFF = 5;
    if (has_ecanexternsnsorndeoff()) {
      total_size += 1 + 1;
    }

    // optional bool ZmqTransNodeOFF = 6;
    if (has_zmqtransnodeoff()) {
      total_size += 1 + 1;
    }

    // optional bool JoyTestNodeOFF = 7;
    if (has_joytestnodeoff()) {
      total_size += 1 + 1;
    }

    // optional bool Ph3ReadNodeOFF = 8;
    if (has_ph3readnodeoff()) {
      total_size += 1 + 1;
    }

  }
  if (_has_bits_[8 / 32] & (0xffu << (8 % 32))) {
    // optional bool Ph3ReadComLost = 9;
    if (has_ph3readcomlost()) {
      total_size += 1 + 1;
    }

    // optional bool LaserComLost = 10;
    if (has_lasercomlost()) {
      total_size += 1 + 1;
    }

    // optional bool LaserNodeOFF = 11;
    if (has_lasernodeoff()) {
      total_size += 1 + 1;
    }

  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void AlarmMessage::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const AlarmMessage* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const AlarmMessage*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void AlarmMessage::MergeFrom(const AlarmMessage& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_ctrolboxhightemalarm()) {
      set_ctrolboxhightemalarm(from.ctrolboxhightemalarm());
    }
    if (from.has_ctrolboxfrozentemalarm()) {
      set_ctrolboxfrozentemalarm(from.ctrolboxfrozentemalarm());
    }
    if (from.has_bmsmodcomlostalarm()) {
      set_bmsmodcomlostalarm(from.bmsmodcomlostalarm());
    }
    if (from.has_emergency_status_stop()) {
      set_emergency_status_stop(from.emergency_status_stop());
    }
    if (from.has_ecanexternsnsorndeoff()) {
      set_ecanexternsnsorndeoff(from.ecanexternsnsorndeoff());
    }
    if (from.has_zmqtransnodeoff()) {
      set_zmqtransnodeoff(from.zmqtransnodeoff());
    }
    if (from.has_joytestnodeoff()) {
      set_joytestnodeoff(from.joytestnodeoff());
    }
    if (from.has_ph3readnodeoff()) {
      set_ph3readnodeoff(from.ph3readnodeoff());
    }
  }
  if (from._has_bits_[8 / 32] & (0xffu << (8 % 32))) {
    if (from.has_ph3readcomlost()) {
      set_ph3readcomlost(from.ph3readcomlost());
    }
    if (from.has_lasercomlost()) {
      set_lasercomlost(from.lasercomlost());
    }
    if (from.has_lasernodeoff()) {
      set_lasernodeoff(from.lasernodeoff());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void AlarmMessage::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void AlarmMessage::CopyFrom(const AlarmMessage& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool AlarmMessage::IsInitialized() const {

  return true;
}

void AlarmMessage::Swap(AlarmMessage* other) {
  if (other != this) {
    std::swap(ctrolboxhightemalarm_, other->ctrolboxhightemalarm_);
    std::swap(ctrolboxfrozentemalarm_, other->ctrolboxfrozentemalarm_);
    std::swap(bmsmodcomlostalarm_, other->bmsmodcomlostalarm_);
    std::swap(emergency_status_stop_, other->emergency_status_stop_);
    std::swap(ecanexternsnsorndeoff_, other->ecanexternsnsorndeoff_);
    std::swap(zmqtransnodeoff_, other->zmqtransnodeoff_);
    std::swap(joytestnodeoff_, other->joytestnodeoff_);
    std::swap(ph3readnodeoff_, other->ph3readnodeoff_);
    std::swap(ph3readcomlost_, other->ph3readcomlost_);
    std::swap(lasercomlost_, other->lasercomlost_);
    std::swap(lasernodeoff_, other->lasernodeoff_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata AlarmMessage::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = AlarmMessage_descriptor_;
  metadata.reflection = AlarmMessage_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

// @@protoc_insertion_point(global_scope)
