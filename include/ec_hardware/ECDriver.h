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

#ifndef ECDRIVER_H
#define ECDRIVER_H

#include <boost/smart_ptr/shared_ptr.hpp>
#include <yaml-cpp/yaml.h>
#include <rtt/Service.hpp>

#include <ecrt.h>

#include <ec_hardware/ECPDOEntryInterface.h>
#include <ec_hardware/ECSlave.h>

class ECDriver {
 public:
  typedef boost::shared_ptr<ECDriver> Ptr;

  ECDriver(const std::string &name)
      : service_(new RTT::Service(name)) {
  }
  virtual ~ECDriver() {
  }

  bool configure(const YAML::Node &cfg) {
    if (!slave_) {
      return false;
    }

    return configureHook(cfg);
  }

  RTT::Service::shared_ptr provides() {
    return service_;
  }

  const std::vector<ECPDOEntryInterface *>& getPDOConfig() {
    return pdo_entries_;
  }

  virtual void updateInputs() = 0;
  virtual void updateOutputs() = 0;
  virtual bool configureHook(const YAML::Node &cfg) = 0;

  void addPDOEntry(ECPDOEntryInterface * const pdo) {
    pdo_entries_.push_back(pdo);
  }

  ECSlave::Ptr getSlave() {
    return slave_;
  }

  void setSlave(ECSlave::Ptr slave) {
    slave_ = slave;
  }
 protected:
  ECSlave::Ptr slave_;
  std::vector<ECPDOEntryInterface *> pdo_entries_;
  RTT::Service::shared_ptr service_;
};

#endif //  ECDRIVER_H
