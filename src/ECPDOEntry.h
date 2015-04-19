#ifndef PDOENTRY_H
#define PDOENTRY_H

#include "ECPDOEntryInterface.h"

template<class type>
class ECPDOEntry : public ECPDOEntryInterface {
public:
  ECPDOEntry(uint16_t index, uint8_t subindex) : ECPDOEntryInterface(index, subindex) {
  }
  
  type read() {
    return ((type) *((type *) (domain_ptr_+offset_)));
  }
  
  void write(const type &data) {
    *((type *)(domain_ptr_+offset_)) = data;
  }
  
private:
  ECPDOEntry() {
  }
};

template<>
class ECPDOEntry<uint8_t> : public ECPDOEntryInterface {
public:
  ECPDOEntry(uint16_t index, uint8_t subindex) : ECPDOEntryInterface(index, subindex) {
  }
  
  uint8_t read() {
    return EC_READ_U8(domain_ptr_+offset_);
  }
  
  void write(const uint8_t &data) {
    EC_WRITE_U8(domain_ptr_+offset_, data);
  }
};

template<>
class ECPDOEntry<int8_t> : public ECPDOEntryInterface {
public:
  ECPDOEntry(uint16_t index, uint8_t subindex) : ECPDOEntryInterface(index, subindex) {
  }
  
  int8_t read() {
    return EC_READ_S8(domain_ptr_+offset_);
  }
  
  void write(const int8_t &data) {
    EC_WRITE_S8(domain_ptr_+offset_, data);
  }
};

template<>
class ECPDOEntry<uint16_t> : public ECPDOEntryInterface {
public:
  ECPDOEntry(uint16_t index, uint8_t subindex) : ECPDOEntryInterface(index, subindex) {
  }
  
  uint16_t read() {
    return EC_READ_U16(domain_ptr_+offset_);
  }
  
  void write(const uint16_t &data) {
    EC_WRITE_U16(domain_ptr_+offset_, data);
  }
};

template<>
class ECPDOEntry<int16_t> : public ECPDOEntryInterface {
public:
  ECPDOEntry(int16_t index, uint8_t subindex) : ECPDOEntryInterface(index, subindex) {
  }
  
  int16_t read() {
    return EC_READ_S16(domain_ptr_+offset_);
  }
  
  void write(const int16_t &data) {
    EC_WRITE_S16(domain_ptr_+offset_, data);
  }
};

template<>
class ECPDOEntry<uint32_t> : public ECPDOEntryInterface {
public:
  ECPDOEntry(uint16_t index, uint8_t subindex) : ECPDOEntryInterface(index, subindex) {
  }
  
  uint32_t read() {
    return EC_READ_U32(domain_ptr_+offset_);
  }
  
  void write(const uint32_t &data) {
    EC_WRITE_U32(domain_ptr_+offset_, data);
  }
};

template<>
class ECPDOEntry<int32_t> : public ECPDOEntryInterface {
public:
  ECPDOEntry(uint16_t index, uint8_t subindex) : ECPDOEntryInterface(index, subindex) {
  }
  
  int32_t read() {
    return EC_READ_S32(domain_ptr_+offset_);
  }
  
  void write(const int32_t &data) {
    EC_WRITE_S32(domain_ptr_+offset_, data);
  }
};

template<>
class ECPDOEntry<uint64_t> : public ECPDOEntryInterface {
public:
  ECPDOEntry(uint16_t index, uint8_t subindex) : ECPDOEntryInterface(index, subindex) {
  }
  
  uint64_t read() {
    return EC_READ_U64(domain_ptr_+offset_);
  }
  
  void write(const uint64_t &data) {
    EC_WRITE_U64(domain_ptr_+offset_, data);
  }
};

template<>
class ECPDOEntry<int64_t> : public ECPDOEntryInterface {
public:
  ECPDOEntry(uint16_t index, uint8_t subindex) : ECPDOEntryInterface(index, subindex) {
  }
  
  int64_t read() {
    return EC_READ_S64(domain_ptr_+offset_);
  }
  
  void write(const int64_t &data) {
    EC_WRITE_S64(domain_ptr_+offset_, data);
  }
};

#endif
