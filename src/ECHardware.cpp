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

#include <string>
#include <vector>

#include <yaml-cpp/yaml.h>

#include <rtt/Component.hpp>
#include <rtt/os/TimeService.hpp>

#include "ECDomain.h"
#include <ec_hardware/ECDriverFactory.h>
#include "ECHardware.h"

ECHardware::ECHardware(const std::string &name)
    : RTT::TaskContext(name, PreOperational) {
  master_ = 0;
  this->properties()->addProperty("master_config", config_str_);
}

ECHardware::~ECHardware() {
}

bool ECHardware::configureHook() {
  if (!config_.parse(config_str_)) {
    RTT::log(RTT::Error) << "Unable to parse configuration" << RTT::endlog();
    return false;
  }

  if (!master_) {
    master_ = ecrt_request_master(config_.master_idx_);
  }

  if (!master_) {
    RTT::log(RTT::Error) << "Unable to open master" << RTT::endlog();
    return false;
  }

  bool error = false;

  for (std::vector<ECTerminalConfig>::iterator t = config_.terminals_.begin();
      t != config_.terminals_.end(); ++t) {

    ECSlave::Ptr slave_ptr = ECSlave::Ptr(
        new ECSlave(t->vendor_id_, t->device_id_));

    if (!slave_ptr->configure(master_, t->station_alias_, t->station_offset_)) {
      error = true;
      RTT::log(RTT::Error) << "Unable to configure slave" << RTT::endlog();
      break;
    }

    if (!t->rpdo_.empty() || !t->tpdo_.empty()) {
      if (!slave_ptr->configurePDO(t->tpdo_, t->rpdo_)) {
        error = true;
        break;
      }
    }

    terminals_.insert(
        std::pair<std::string, ECSlave::Ptr>(t->name_, slave_ptr));
  }

  for (std::vector<ECDomainConfig>::iterator d = config_.domains_.begin();
      d != config_.domains_.end(); ++d) {
    bool ret = createDomain(*d);
    if (!ret) {
      RTT::log(RTT::Error) << "Unable to create Domain" << RTT::endlog();
      error = true;
      break;
    }
  }

  if (error) {
    domains_.clear();
    ecrt_release_master(master_);
    master_ = 0;
    return false;
  }

  return true;
}

void ECHardware::cleanupHook() {
  ecrt_release_master(master_);
}

bool ECHardware::startHook() {
  int ret = ecrt_master_activate(master_);
  if (ret) {
    return false;  //  ret < 0 >> error
  }

  for (std::vector<ECDomain::Ptr>::iterator d = domains_.begin();
      d != domains_.end(); ++d) {
    (*d)->start();
  }

  return true;
}

void ECHardware::stopHook() {
  ecrt_master_deactivate(master_);
}

void ECHardware::updateHook() {
  ecrt_master_receive(master_);

  for (std::vector<ECDomain::Ptr>::iterator d = domains_.begin();
      d != domains_.end(); ++d) {
    (*d)->update();
  }

  ecrt_master_application_time(master_, RTT::os::TimeService::Instance()->getNSecs());

  ecrt_master_sync_reference_clock(master_);
  ecrt_master_sync_slave_clocks(master_);

  ecrt_master_send(master_);
}

bool ECHardware::createDomain(const ECDomainConfig& cfg) {
  ECDomain::Ptr dom = ECDomain::Ptr(new ECDomain(master_));
  if (!(*dom)) {
    RTT::log(RTT::Error) << "Unable to create domain" << RTT::endlog();
    return false;
  }

  for (std::vector<ECDriverConfig>::const_iterator driver_cfg = cfg.drivers_
      .begin(); driver_cfg != cfg.drivers_.end(); ++driver_cfg) {
    ECDriver::Ptr driver = createDriver(*driver_cfg);
    if (!driver) {
      return false;
    }

    if (!driver->configure(driver_cfg->driver_cfg_)) {
      RTT::log(RTT::Error) << "Unable to configure Driver" << RTT::endlog();
      return false;
    }
    dom->addDriver(driver);
    this->provides()->addService(driver->provides());
  }

  if (!dom->configure()) {
    RTT::log(RTT::Error) << "Unable to configure Domain" << RTT::endlog();
    return false;
  }

  domains_.push_back(dom);
  return true;
}

ECDriver::Ptr ECHardware::createDriver(const ECDriverConfig& cfg) {
  TerminalMap::iterator it = terminals_.find(cfg.terminal_);
  if (it == terminals_.end()) {
    RTT::log(RTT::Error) << "There is no terminal [" << cfg.terminal_
                         << "] requested by driver [" << cfg.name_ << "]"
                         << RTT::endlog();
    return ECDriver::Ptr();
  }

  ECDriverFactory::Ptr factory = this->getProvider<ECDriverFactory>(cfg.type_);
  if (!factory) {
    RTT::log(RTT::Error) << "Unable to create driver [" << cfg.name_
                         << "] of type [" << cfg.type_ << "]" << RTT::endlog();
    return ECDriver::Ptr();
  }
  ECDriver::Ptr drv = factory->createDriver(cfg.name_);
  drv->setSlave(it->second);
  return drv;
}

ORO_CREATE_COMPONENT(ECHardware)

