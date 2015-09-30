/*
 * Copyright (c) 2015, Robot Control and Pattern Recognition Group, Warsaw University of Technology.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Robot Control and Pattern Recognition Group,
 *       Warsaw University of Technology nor the names of its contributors may
 *       be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef PDOENTRY_H
#define PDOENTRY_H

#include <ec_hardware/ECPDOEntryInterface.h>

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
