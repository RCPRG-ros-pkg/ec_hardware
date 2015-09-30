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

#ifndef PDOENTRYINTERFACE_H
#define PDOENTRYINTERFACE_H

class ECPDOEntryInterface {
public:
  ECPDOEntryInterface(uint16_t index, uint8_t subindex):
    index_(index),
    subindex_(subindex),
    offset_(0),
    domain_ptr_(0) {
  }
  
  const uint16_t getIndex() const{
    return index_;
  }
  
  const uint8_t getSubIndex() const{
    return subindex_;
  }
  
  unsigned int *getOffsetPtr() {
    return &offset_;
  }

  void setDomainPtr(uint8_t *ptr) {
    domain_ptr_ = ptr;
  }

protected:
  const uint16_t index_;
  const uint8_t subindex_;
  unsigned int offset_;
  uint8_t *domain_ptr_;

private:
  ECPDOEntryInterface() : index_(0), subindex_(0), offset_(0), domain_ptr_(0) {
  }
};

#endif
