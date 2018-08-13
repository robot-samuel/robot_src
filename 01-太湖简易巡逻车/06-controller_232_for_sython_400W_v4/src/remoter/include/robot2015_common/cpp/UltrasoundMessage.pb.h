// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: UltrasoundMessage.proto

#ifndef PROTOBUF_UltrasoundMessage_2eproto__INCLUDED
#define PROTOBUF_UltrasoundMessage_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2005000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2005000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_UltrasoundMessage_2eproto();
void protobuf_AssignDesc_UltrasoundMessage_2eproto();
void protobuf_ShutdownFile_UltrasoundMessage_2eproto();

class UltrasoundMessage;

// ===================================================================

class UltrasoundMessage : public ::google::protobuf::Message {
 public:
  UltrasoundMessage();
  virtual ~UltrasoundMessage();

  UltrasoundMessage(const UltrasoundMessage& from);

  inline UltrasoundMessage& operator=(const UltrasoundMessage& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const UltrasoundMessage& default_instance();

  void Swap(UltrasoundMessage* other);

  // implements Message ----------------------------------------------

  UltrasoundMessage* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const UltrasoundMessage& from);
  void MergeFrom(const UltrasoundMessage& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional int32 online = 1;
  inline bool has_online() const;
  inline void clear_online();
  static const int kOnlineFieldNumber = 1;
  inline ::google::protobuf::int32 online() const;
  inline void set_online(::google::protobuf::int32 value);

  // repeated int32 distance_Ultrasonic = 2;
  inline int distance_ultrasonic_size() const;
  inline void clear_distance_ultrasonic();
  static const int kDistanceUltrasonicFieldNumber = 2;
  inline ::google::protobuf::int32 distance_ultrasonic(int index) const;
  inline void set_distance_ultrasonic(int index, ::google::protobuf::int32 value);
  inline void add_distance_ultrasonic(::google::protobuf::int32 value);
  inline const ::google::protobuf::RepeatedField< ::google::protobuf::int32 >&
      distance_ultrasonic() const;
  inline ::google::protobuf::RepeatedField< ::google::protobuf::int32 >*
      mutable_distance_ultrasonic();

  // @@protoc_insertion_point(class_scope:UltrasoundMessage)
 private:
  inline void set_has_online();
  inline void clear_has_online();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::RepeatedField< ::google::protobuf::int32 > distance_ultrasonic_;
  ::google::protobuf::int32 online_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];

  friend void  protobuf_AddDesc_UltrasoundMessage_2eproto();
  friend void protobuf_AssignDesc_UltrasoundMessage_2eproto();
  friend void protobuf_ShutdownFile_UltrasoundMessage_2eproto();

  void InitAsDefaultInstance();
  static UltrasoundMessage* default_instance_;
};
// ===================================================================


// ===================================================================

// UltrasoundMessage

// optional int32 online = 1;
inline bool UltrasoundMessage::has_online() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void UltrasoundMessage::set_has_online() {
  _has_bits_[0] |= 0x00000001u;
}
inline void UltrasoundMessage::clear_has_online() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void UltrasoundMessage::clear_online() {
  online_ = 0;
  clear_has_online();
}
inline ::google::protobuf::int32 UltrasoundMessage::online() const {
  return online_;
}
inline void UltrasoundMessage::set_online(::google::protobuf::int32 value) {
  set_has_online();
  online_ = value;
}

// repeated int32 distance_Ultrasonic = 2;
inline int UltrasoundMessage::distance_ultrasonic_size() const {
  return distance_ultrasonic_.size();
}
inline void UltrasoundMessage::clear_distance_ultrasonic() {
  distance_ultrasonic_.Clear();
}
inline ::google::protobuf::int32 UltrasoundMessage::distance_ultrasonic(int index) const {
  return distance_ultrasonic_.Get(index);
}
inline void UltrasoundMessage::set_distance_ultrasonic(int index, ::google::protobuf::int32 value) {
  distance_ultrasonic_.Set(index, value);
}
inline void UltrasoundMessage::add_distance_ultrasonic(::google::protobuf::int32 value) {
  distance_ultrasonic_.Add(value);
}
inline const ::google::protobuf::RepeatedField< ::google::protobuf::int32 >&
UltrasoundMessage::distance_ultrasonic() const {
  return distance_ultrasonic_;
}
inline ::google::protobuf::RepeatedField< ::google::protobuf::int32 >*
UltrasoundMessage::mutable_distance_ultrasonic() {
  return &distance_ultrasonic_;
}


// @@protoc_insertion_point(namespace_scope)

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_UltrasoundMessage_2eproto__INCLUDED
