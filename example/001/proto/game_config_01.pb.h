// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: game_config_01.proto

#ifndef PROTOBUF_game_5fconfig_5f01_2eproto__INCLUDED
#define PROTOBUF_game_5fconfig_5f01_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2006000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2006001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_game_5fconfig_5f01_2eproto();
void protobuf_AssignDesc_game_5fconfig_5f01_2eproto();
void protobuf_ShutdownFile_game_5fconfig_5f01_2eproto();

class GAME_CFG_STRUCT_1;

enum ENUM_WEEK {
  MON = 1,
  TUE = 2,
  WED = 3,
  THU = 4,
  FRI = 5,
  SAT = 6,
  SUN = 7
};
bool ENUM_WEEK_IsValid(int value);
const ENUM_WEEK ENUM_WEEK_MIN = MON;
const ENUM_WEEK ENUM_WEEK_MAX = SUN;
const int ENUM_WEEK_ARRAYSIZE = ENUM_WEEK_MAX + 1;

const ::google::protobuf::EnumDescriptor* ENUM_WEEK_descriptor();
inline const ::std::string& ENUM_WEEK_Name(ENUM_WEEK value) {
  return ::google::protobuf::internal::NameOfEnum(
    ENUM_WEEK_descriptor(), value);
}
inline bool ENUM_WEEK_Parse(
    const ::std::string& name, ENUM_WEEK* value) {
  return ::google::protobuf::internal::ParseNamedEnum<ENUM_WEEK>(
    ENUM_WEEK_descriptor(), name, value);
}
// ===================================================================

class GAME_CFG_STRUCT_1 : public ::google::protobuf::Message {
 public:
  GAME_CFG_STRUCT_1();
  virtual ~GAME_CFG_STRUCT_1();

  GAME_CFG_STRUCT_1(const GAME_CFG_STRUCT_1& from);

  inline GAME_CFG_STRUCT_1& operator=(const GAME_CFG_STRUCT_1& from) {
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
  static const GAME_CFG_STRUCT_1& default_instance();

  void Swap(GAME_CFG_STRUCT_1* other);

  // implements Message ----------------------------------------------

  GAME_CFG_STRUCT_1* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const GAME_CFG_STRUCT_1& from);
  void MergeFrom(const GAME_CFG_STRUCT_1& from);
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

  // optional int32 s1_a = 1;
  inline bool has_s1_a() const;
  inline void clear_s1_a();
  static const int kS1AFieldNumber = 1;
  inline ::google::protobuf::int32 s1_a() const;
  inline void set_s1_a(::google::protobuf::int32 value);

  // optional uint32 s1_b = 2;
  inline bool has_s1_b() const;
  inline void clear_s1_b();
  static const int kS1BFieldNumber = 2;
  inline ::google::protobuf::uint32 s1_b() const;
  inline void set_s1_b(::google::protobuf::uint32 value);

  // optional int64 s1_c = 3;
  inline bool has_s1_c() const;
  inline void clear_s1_c();
  static const int kS1CFieldNumber = 3;
  inline ::google::protobuf::int64 s1_c() const;
  inline void set_s1_c(::google::protobuf::int64 value);

  // optional uint64 s1_d = 4;
  inline bool has_s1_d() const;
  inline void clear_s1_d();
  static const int kS1DFieldNumber = 4;
  inline ::google::protobuf::uint64 s1_d() const;
  inline void set_s1_d(::google::protobuf::uint64 value);

  // optional double s1_e = 5;
  inline bool has_s1_e() const;
  inline void clear_s1_e();
  static const int kS1EFieldNumber = 5;
  inline double s1_e() const;
  inline void set_s1_e(double value);

  // optional float s1_f = 6;
  inline bool has_s1_f() const;
  inline void clear_s1_f();
  static const int kS1FFieldNumber = 6;
  inline float s1_f() const;
  inline void set_s1_f(float value);

  // optional bool s1_g = 7;
  inline bool has_s1_g() const;
  inline void clear_s1_g();
  static const int kS1GFieldNumber = 7;
  inline bool s1_g() const;
  inline void set_s1_g(bool value);

  // optional string s1_h = 8;
  inline bool has_s1_h() const;
  inline void clear_s1_h();
  static const int kS1HFieldNumber = 8;
  inline const ::std::string& s1_h() const;
  inline void set_s1_h(const ::std::string& value);
  inline void set_s1_h(const char* value);
  inline void set_s1_h(const char* value, size_t size);
  inline ::std::string* mutable_s1_h();
  inline ::std::string* release_s1_h();
  inline void set_allocated_s1_h(::std::string* s1_h);

  // optional bytes s1_i = 9;
  inline bool has_s1_i() const;
  inline void clear_s1_i();
  static const int kS1IFieldNumber = 9;
  inline const ::std::string& s1_i() const;
  inline void set_s1_i(const ::std::string& value);
  inline void set_s1_i(const char* value);
  inline void set_s1_i(const void* value, size_t size);
  inline ::std::string* mutable_s1_i();
  inline ::std::string* release_s1_i();
  inline void set_allocated_s1_i(::std::string* s1_i);

  // repeated int32 s1_j = 10;
  inline int s1_j_size() const;
  inline void clear_s1_j();
  static const int kS1JFieldNumber = 10;
  inline ::google::protobuf::int32 s1_j(int index) const;
  inline void set_s1_j(int index, ::google::protobuf::int32 value);
  inline void add_s1_j(::google::protobuf::int32 value);
  inline const ::google::protobuf::RepeatedField< ::google::protobuf::int32 >&
      s1_j() const;
  inline ::google::protobuf::RepeatedField< ::google::protobuf::int32 >*
      mutable_s1_j();

  // repeated string s1_k = 11;
  inline int s1_k_size() const;
  inline void clear_s1_k();
  static const int kS1KFieldNumber = 11;
  inline const ::std::string& s1_k(int index) const;
  inline ::std::string* mutable_s1_k(int index);
  inline void set_s1_k(int index, const ::std::string& value);
  inline void set_s1_k(int index, const char* value);
  inline void set_s1_k(int index, const char* value, size_t size);
  inline ::std::string* add_s1_k();
  inline void add_s1_k(const ::std::string& value);
  inline void add_s1_k(const char* value);
  inline void add_s1_k(const char* value, size_t size);
  inline const ::google::protobuf::RepeatedPtrField< ::std::string>& s1_k() const;
  inline ::google::protobuf::RepeatedPtrField< ::std::string>* mutable_s1_k();

  // optional .ENUM_WEEK s1_l = 12;
  inline bool has_s1_l() const;
  inline void clear_s1_l();
  static const int kS1LFieldNumber = 12;
  inline ::ENUM_WEEK s1_l() const;
  inline void set_s1_l(::ENUM_WEEK value);

  // @@protoc_insertion_point(class_scope:GAME_CFG_STRUCT_1)
 private:
  inline void set_has_s1_a();
  inline void clear_has_s1_a();
  inline void set_has_s1_b();
  inline void clear_has_s1_b();
  inline void set_has_s1_c();
  inline void clear_has_s1_c();
  inline void set_has_s1_d();
  inline void clear_has_s1_d();
  inline void set_has_s1_e();
  inline void clear_has_s1_e();
  inline void set_has_s1_f();
  inline void clear_has_s1_f();
  inline void set_has_s1_g();
  inline void clear_has_s1_g();
  inline void set_has_s1_h();
  inline void clear_has_s1_h();
  inline void set_has_s1_i();
  inline void clear_has_s1_i();
  inline void set_has_s1_l();
  inline void clear_has_s1_l();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::google::protobuf::int32 s1_a_;
  ::google::protobuf::uint32 s1_b_;
  ::google::protobuf::int64 s1_c_;
  ::google::protobuf::uint64 s1_d_;
  double s1_e_;
  float s1_f_;
  bool s1_g_;
  ::std::string* s1_h_;
  ::std::string* s1_i_;
  ::google::protobuf::RepeatedField< ::google::protobuf::int32 > s1_j_;
  ::google::protobuf::RepeatedPtrField< ::std::string> s1_k_;
  int s1_l_;
  friend void  protobuf_AddDesc_game_5fconfig_5f01_2eproto();
  friend void protobuf_AssignDesc_game_5fconfig_5f01_2eproto();
  friend void protobuf_ShutdownFile_game_5fconfig_5f01_2eproto();

  void InitAsDefaultInstance();
  static GAME_CFG_STRUCT_1* default_instance_;
};
// ===================================================================


// ===================================================================

// GAME_CFG_STRUCT_1

// optional int32 s1_a = 1;
inline bool GAME_CFG_STRUCT_1::has_s1_a() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void GAME_CFG_STRUCT_1::set_has_s1_a() {
  _has_bits_[0] |= 0x00000001u;
}
inline void GAME_CFG_STRUCT_1::clear_has_s1_a() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void GAME_CFG_STRUCT_1::clear_s1_a() {
  s1_a_ = 0;
  clear_has_s1_a();
}
inline ::google::protobuf::int32 GAME_CFG_STRUCT_1::s1_a() const {
  // @@protoc_insertion_point(field_get:GAME_CFG_STRUCT_1.s1_a)
  return s1_a_;
}
inline void GAME_CFG_STRUCT_1::set_s1_a(::google::protobuf::int32 value) {
  set_has_s1_a();
  s1_a_ = value;
  // @@protoc_insertion_point(field_set:GAME_CFG_STRUCT_1.s1_a)
}

// optional uint32 s1_b = 2;
inline bool GAME_CFG_STRUCT_1::has_s1_b() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void GAME_CFG_STRUCT_1::set_has_s1_b() {
  _has_bits_[0] |= 0x00000002u;
}
inline void GAME_CFG_STRUCT_1::clear_has_s1_b() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void GAME_CFG_STRUCT_1::clear_s1_b() {
  s1_b_ = 0u;
  clear_has_s1_b();
}
inline ::google::protobuf::uint32 GAME_CFG_STRUCT_1::s1_b() const {
  // @@protoc_insertion_point(field_get:GAME_CFG_STRUCT_1.s1_b)
  return s1_b_;
}
inline void GAME_CFG_STRUCT_1::set_s1_b(::google::protobuf::uint32 value) {
  set_has_s1_b();
  s1_b_ = value;
  // @@protoc_insertion_point(field_set:GAME_CFG_STRUCT_1.s1_b)
}

// optional int64 s1_c = 3;
inline bool GAME_CFG_STRUCT_1::has_s1_c() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void GAME_CFG_STRUCT_1::set_has_s1_c() {
  _has_bits_[0] |= 0x00000004u;
}
inline void GAME_CFG_STRUCT_1::clear_has_s1_c() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void GAME_CFG_STRUCT_1::clear_s1_c() {
  s1_c_ = GOOGLE_LONGLONG(0);
  clear_has_s1_c();
}
inline ::google::protobuf::int64 GAME_CFG_STRUCT_1::s1_c() const {
  // @@protoc_insertion_point(field_get:GAME_CFG_STRUCT_1.s1_c)
  return s1_c_;
}
inline void GAME_CFG_STRUCT_1::set_s1_c(::google::protobuf::int64 value) {
  set_has_s1_c();
  s1_c_ = value;
  // @@protoc_insertion_point(field_set:GAME_CFG_STRUCT_1.s1_c)
}

// optional uint64 s1_d = 4;
inline bool GAME_CFG_STRUCT_1::has_s1_d() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void GAME_CFG_STRUCT_1::set_has_s1_d() {
  _has_bits_[0] |= 0x00000008u;
}
inline void GAME_CFG_STRUCT_1::clear_has_s1_d() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void GAME_CFG_STRUCT_1::clear_s1_d() {
  s1_d_ = GOOGLE_ULONGLONG(0);
  clear_has_s1_d();
}
inline ::google::protobuf::uint64 GAME_CFG_STRUCT_1::s1_d() const {
  // @@protoc_insertion_point(field_get:GAME_CFG_STRUCT_1.s1_d)
  return s1_d_;
}
inline void GAME_CFG_STRUCT_1::set_s1_d(::google::protobuf::uint64 value) {
  set_has_s1_d();
  s1_d_ = value;
  // @@protoc_insertion_point(field_set:GAME_CFG_STRUCT_1.s1_d)
}

// optional double s1_e = 5;
inline bool GAME_CFG_STRUCT_1::has_s1_e() const {
  return (_has_bits_[0] & 0x00000010u) != 0;
}
inline void GAME_CFG_STRUCT_1::set_has_s1_e() {
  _has_bits_[0] |= 0x00000010u;
}
inline void GAME_CFG_STRUCT_1::clear_has_s1_e() {
  _has_bits_[0] &= ~0x00000010u;
}
inline void GAME_CFG_STRUCT_1::clear_s1_e() {
  s1_e_ = 0;
  clear_has_s1_e();
}
inline double GAME_CFG_STRUCT_1::s1_e() const {
  // @@protoc_insertion_point(field_get:GAME_CFG_STRUCT_1.s1_e)
  return s1_e_;
}
inline void GAME_CFG_STRUCT_1::set_s1_e(double value) {
  set_has_s1_e();
  s1_e_ = value;
  // @@protoc_insertion_point(field_set:GAME_CFG_STRUCT_1.s1_e)
}

// optional float s1_f = 6;
inline bool GAME_CFG_STRUCT_1::has_s1_f() const {
  return (_has_bits_[0] & 0x00000020u) != 0;
}
inline void GAME_CFG_STRUCT_1::set_has_s1_f() {
  _has_bits_[0] |= 0x00000020u;
}
inline void GAME_CFG_STRUCT_1::clear_has_s1_f() {
  _has_bits_[0] &= ~0x00000020u;
}
inline void GAME_CFG_STRUCT_1::clear_s1_f() {
  s1_f_ = 0;
  clear_has_s1_f();
}
inline float GAME_CFG_STRUCT_1::s1_f() const {
  // @@protoc_insertion_point(field_get:GAME_CFG_STRUCT_1.s1_f)
  return s1_f_;
}
inline void GAME_CFG_STRUCT_1::set_s1_f(float value) {
  set_has_s1_f();
  s1_f_ = value;
  // @@protoc_insertion_point(field_set:GAME_CFG_STRUCT_1.s1_f)
}

// optional bool s1_g = 7;
inline bool GAME_CFG_STRUCT_1::has_s1_g() const {
  return (_has_bits_[0] & 0x00000040u) != 0;
}
inline void GAME_CFG_STRUCT_1::set_has_s1_g() {
  _has_bits_[0] |= 0x00000040u;
}
inline void GAME_CFG_STRUCT_1::clear_has_s1_g() {
  _has_bits_[0] &= ~0x00000040u;
}
inline void GAME_CFG_STRUCT_1::clear_s1_g() {
  s1_g_ = false;
  clear_has_s1_g();
}
inline bool GAME_CFG_STRUCT_1::s1_g() const {
  // @@protoc_insertion_point(field_get:GAME_CFG_STRUCT_1.s1_g)
  return s1_g_;
}
inline void GAME_CFG_STRUCT_1::set_s1_g(bool value) {
  set_has_s1_g();
  s1_g_ = value;
  // @@protoc_insertion_point(field_set:GAME_CFG_STRUCT_1.s1_g)
}

// optional string s1_h = 8;
inline bool GAME_CFG_STRUCT_1::has_s1_h() const {
  return (_has_bits_[0] & 0x00000080u) != 0;
}
inline void GAME_CFG_STRUCT_1::set_has_s1_h() {
  _has_bits_[0] |= 0x00000080u;
}
inline void GAME_CFG_STRUCT_1::clear_has_s1_h() {
  _has_bits_[0] &= ~0x00000080u;
}
inline void GAME_CFG_STRUCT_1::clear_s1_h() {
  if (s1_h_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    s1_h_->clear();
  }
  clear_has_s1_h();
}
inline const ::std::string& GAME_CFG_STRUCT_1::s1_h() const {
  // @@protoc_insertion_point(field_get:GAME_CFG_STRUCT_1.s1_h)
  return *s1_h_;
}
inline void GAME_CFG_STRUCT_1::set_s1_h(const ::std::string& value) {
  set_has_s1_h();
  if (s1_h_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    s1_h_ = new ::std::string;
  }
  s1_h_->assign(value);
  // @@protoc_insertion_point(field_set:GAME_CFG_STRUCT_1.s1_h)
}
inline void GAME_CFG_STRUCT_1::set_s1_h(const char* value) {
  set_has_s1_h();
  if (s1_h_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    s1_h_ = new ::std::string;
  }
  s1_h_->assign(value);
  // @@protoc_insertion_point(field_set_char:GAME_CFG_STRUCT_1.s1_h)
}
inline void GAME_CFG_STRUCT_1::set_s1_h(const char* value, size_t size) {
  set_has_s1_h();
  if (s1_h_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    s1_h_ = new ::std::string;
  }
  s1_h_->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:GAME_CFG_STRUCT_1.s1_h)
}
inline ::std::string* GAME_CFG_STRUCT_1::mutable_s1_h() {
  set_has_s1_h();
  if (s1_h_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    s1_h_ = new ::std::string;
  }
  // @@protoc_insertion_point(field_mutable:GAME_CFG_STRUCT_1.s1_h)
  return s1_h_;
}
inline ::std::string* GAME_CFG_STRUCT_1::release_s1_h() {
  clear_has_s1_h();
  if (s1_h_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    return NULL;
  } else {
    ::std::string* temp = s1_h_;
    s1_h_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    return temp;
  }
}
inline void GAME_CFG_STRUCT_1::set_allocated_s1_h(::std::string* s1_h) {
  if (s1_h_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete s1_h_;
  }
  if (s1_h) {
    set_has_s1_h();
    s1_h_ = s1_h;
  } else {
    clear_has_s1_h();
    s1_h_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  // @@protoc_insertion_point(field_set_allocated:GAME_CFG_STRUCT_1.s1_h)
}

// optional bytes s1_i = 9;
inline bool GAME_CFG_STRUCT_1::has_s1_i() const {
  return (_has_bits_[0] & 0x00000100u) != 0;
}
inline void GAME_CFG_STRUCT_1::set_has_s1_i() {
  _has_bits_[0] |= 0x00000100u;
}
inline void GAME_CFG_STRUCT_1::clear_has_s1_i() {
  _has_bits_[0] &= ~0x00000100u;
}
inline void GAME_CFG_STRUCT_1::clear_s1_i() {
  if (s1_i_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    s1_i_->clear();
  }
  clear_has_s1_i();
}
inline const ::std::string& GAME_CFG_STRUCT_1::s1_i() const {
  // @@protoc_insertion_point(field_get:GAME_CFG_STRUCT_1.s1_i)
  return *s1_i_;
}
inline void GAME_CFG_STRUCT_1::set_s1_i(const ::std::string& value) {
  set_has_s1_i();
  if (s1_i_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    s1_i_ = new ::std::string;
  }
  s1_i_->assign(value);
  // @@protoc_insertion_point(field_set:GAME_CFG_STRUCT_1.s1_i)
}
inline void GAME_CFG_STRUCT_1::set_s1_i(const char* value) {
  set_has_s1_i();
  if (s1_i_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    s1_i_ = new ::std::string;
  }
  s1_i_->assign(value);
  // @@protoc_insertion_point(field_set_char:GAME_CFG_STRUCT_1.s1_i)
}
inline void GAME_CFG_STRUCT_1::set_s1_i(const void* value, size_t size) {
  set_has_s1_i();
  if (s1_i_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    s1_i_ = new ::std::string;
  }
  s1_i_->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:GAME_CFG_STRUCT_1.s1_i)
}
inline ::std::string* GAME_CFG_STRUCT_1::mutable_s1_i() {
  set_has_s1_i();
  if (s1_i_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    s1_i_ = new ::std::string;
  }
  // @@protoc_insertion_point(field_mutable:GAME_CFG_STRUCT_1.s1_i)
  return s1_i_;
}
inline ::std::string* GAME_CFG_STRUCT_1::release_s1_i() {
  clear_has_s1_i();
  if (s1_i_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    return NULL;
  } else {
    ::std::string* temp = s1_i_;
    s1_i_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    return temp;
  }
}
inline void GAME_CFG_STRUCT_1::set_allocated_s1_i(::std::string* s1_i) {
  if (s1_i_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete s1_i_;
  }
  if (s1_i) {
    set_has_s1_i();
    s1_i_ = s1_i;
  } else {
    clear_has_s1_i();
    s1_i_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  // @@protoc_insertion_point(field_set_allocated:GAME_CFG_STRUCT_1.s1_i)
}

// repeated int32 s1_j = 10;
inline int GAME_CFG_STRUCT_1::s1_j_size() const {
  return s1_j_.size();
}
inline void GAME_CFG_STRUCT_1::clear_s1_j() {
  s1_j_.Clear();
}
inline ::google::protobuf::int32 GAME_CFG_STRUCT_1::s1_j(int index) const {
  // @@protoc_insertion_point(field_get:GAME_CFG_STRUCT_1.s1_j)
  return s1_j_.Get(index);
}
inline void GAME_CFG_STRUCT_1::set_s1_j(int index, ::google::protobuf::int32 value) {
  s1_j_.Set(index, value);
  // @@protoc_insertion_point(field_set:GAME_CFG_STRUCT_1.s1_j)
}
inline void GAME_CFG_STRUCT_1::add_s1_j(::google::protobuf::int32 value) {
  s1_j_.Add(value);
  // @@protoc_insertion_point(field_add:GAME_CFG_STRUCT_1.s1_j)
}
inline const ::google::protobuf::RepeatedField< ::google::protobuf::int32 >&
GAME_CFG_STRUCT_1::s1_j() const {
  // @@protoc_insertion_point(field_list:GAME_CFG_STRUCT_1.s1_j)
  return s1_j_;
}
inline ::google::protobuf::RepeatedField< ::google::protobuf::int32 >*
GAME_CFG_STRUCT_1::mutable_s1_j() {
  // @@protoc_insertion_point(field_mutable_list:GAME_CFG_STRUCT_1.s1_j)
  return &s1_j_;
}

// repeated string s1_k = 11;
inline int GAME_CFG_STRUCT_1::s1_k_size() const {
  return s1_k_.size();
}
inline void GAME_CFG_STRUCT_1::clear_s1_k() {
  s1_k_.Clear();
}
inline const ::std::string& GAME_CFG_STRUCT_1::s1_k(int index) const {
  // @@protoc_insertion_point(field_get:GAME_CFG_STRUCT_1.s1_k)
  return s1_k_.Get(index);
}
inline ::std::string* GAME_CFG_STRUCT_1::mutable_s1_k(int index) {
  // @@protoc_insertion_point(field_mutable:GAME_CFG_STRUCT_1.s1_k)
  return s1_k_.Mutable(index);
}
inline void GAME_CFG_STRUCT_1::set_s1_k(int index, const ::std::string& value) {
  // @@protoc_insertion_point(field_set:GAME_CFG_STRUCT_1.s1_k)
  s1_k_.Mutable(index)->assign(value);
}
inline void GAME_CFG_STRUCT_1::set_s1_k(int index, const char* value) {
  s1_k_.Mutable(index)->assign(value);
  // @@protoc_insertion_point(field_set_char:GAME_CFG_STRUCT_1.s1_k)
}
inline void GAME_CFG_STRUCT_1::set_s1_k(int index, const char* value, size_t size) {
  s1_k_.Mutable(index)->assign(
    reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:GAME_CFG_STRUCT_1.s1_k)
}
inline ::std::string* GAME_CFG_STRUCT_1::add_s1_k() {
  return s1_k_.Add();
}
inline void GAME_CFG_STRUCT_1::add_s1_k(const ::std::string& value) {
  s1_k_.Add()->assign(value);
  // @@protoc_insertion_point(field_add:GAME_CFG_STRUCT_1.s1_k)
}
inline void GAME_CFG_STRUCT_1::add_s1_k(const char* value) {
  s1_k_.Add()->assign(value);
  // @@protoc_insertion_point(field_add_char:GAME_CFG_STRUCT_1.s1_k)
}
inline void GAME_CFG_STRUCT_1::add_s1_k(const char* value, size_t size) {
  s1_k_.Add()->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_add_pointer:GAME_CFG_STRUCT_1.s1_k)
}
inline const ::google::protobuf::RepeatedPtrField< ::std::string>&
GAME_CFG_STRUCT_1::s1_k() const {
  // @@protoc_insertion_point(field_list:GAME_CFG_STRUCT_1.s1_k)
  return s1_k_;
}
inline ::google::protobuf::RepeatedPtrField< ::std::string>*
GAME_CFG_STRUCT_1::mutable_s1_k() {
  // @@protoc_insertion_point(field_mutable_list:GAME_CFG_STRUCT_1.s1_k)
  return &s1_k_;
}

// optional .ENUM_WEEK s1_l = 12;
inline bool GAME_CFG_STRUCT_1::has_s1_l() const {
  return (_has_bits_[0] & 0x00000800u) != 0;
}
inline void GAME_CFG_STRUCT_1::set_has_s1_l() {
  _has_bits_[0] |= 0x00000800u;
}
inline void GAME_CFG_STRUCT_1::clear_has_s1_l() {
  _has_bits_[0] &= ~0x00000800u;
}
inline void GAME_CFG_STRUCT_1::clear_s1_l() {
  s1_l_ = 1;
  clear_has_s1_l();
}
inline ::ENUM_WEEK GAME_CFG_STRUCT_1::s1_l() const {
  // @@protoc_insertion_point(field_get:GAME_CFG_STRUCT_1.s1_l)
  return static_cast< ::ENUM_WEEK >(s1_l_);
}
inline void GAME_CFG_STRUCT_1::set_s1_l(::ENUM_WEEK value) {
  assert(::ENUM_WEEK_IsValid(value));
  set_has_s1_l();
  s1_l_ = value;
  // @@protoc_insertion_point(field_set:GAME_CFG_STRUCT_1.s1_l)
}


// @@protoc_insertion_point(namespace_scope)

#ifndef SWIG
namespace google {
namespace protobuf {

template <> struct is_proto_enum< ::ENUM_WEEK> : ::google::protobuf::internal::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::ENUM_WEEK>() {
  return ::ENUM_WEEK_descriptor();
}

}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_game_5fconfig_5f01_2eproto__INCLUDED
