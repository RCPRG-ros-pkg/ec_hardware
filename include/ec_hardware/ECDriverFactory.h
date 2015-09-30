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

#ifndef ECDRIVERFACTORY_H
#define ECDRIVERFACTORY_H

#include <rtt/RTT.hpp>
#include <ecrt.h>

#include <ec_hardware/ECDriver.h>

class ECDriverFactory : public RTT::ServiceRequester {
public:
  explicit ECDriverFactory(RTT::TaskContext* owner) :
      RTT::ServiceRequester("factory", owner),
      createDriver("createDriver") {
    this->addOperationCaller(createDriver);
  }

  virtual ~ECDriverFactory() {
  }

  RTT::OperationCaller<ECDriver::Ptr(const std::string &name)> createDriver;
  
  typedef boost::shared_ptr<ECDriverFactory> Ptr;
};

template <char const *dname, class type>
class ECDriverFactoryService : public RTT::Service {
public:
  explicit ECDriverFactoryService(RTT::TaskContext* owner) : RTT::Service(dname, owner) {
    this->addOperation("createDriver", &ECDriverFactoryService::createDriver, this, RTT::ClientThread);
  }
  
  virtual ~ECDriverFactoryService() {
  }
  
  virtual ECDriver::Ptr createDriver(const std::string &name) {
    return ECDriver::Ptr(new type(name));
  }
};

#endif
