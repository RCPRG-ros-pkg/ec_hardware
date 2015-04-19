
#include <string>
#include <vector>

#include <yaml-cpp/yaml.h>

#include <rtt/Component.hpp>

#include "ECDomain.h"
#include "ECDriverFactory.h"
#include "ECHardware.h"

ECHardware::ECHardware (const std::string &name) : RTT::TaskContext(name, PreOperational) {
  master_ = 0;
  this->properties()->addProperty("master_config", config_str_);
}

ECHardware::~ECHardware () {}

bool ECHardware::configureHook () {
  if (!config_.parse (config_str_)) {
    RTT::log(RTT::Error) << "Unable to parse configuration" << RTT::endlog();
    return false;
  }
  
  if (!master_) {
    master_ = ecrt_request_master (config_.master_idx_);
  }
  
  if (!master_) {
    RTT::log(RTT::Error) << "Unable to open master" << RTT::endlog();
    return false;
  }

  bool error = false;

  for (std::vector<ECTerminalConfig>::iterator t = config_.terminals_.begin(); t != config_.terminals_.end(); ++t) {
  
    ECSlave::Ptr slave_ptr = ECSlave::Ptr(new ECSlave(t->vendor_id_, t->device_id_));
  
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

    terminals_.insert(std::pair<std::string, ECSlave::Ptr>(t->name_, slave_ptr));
  }

  for (std::vector<ECDomainConfig>::iterator d = config_.domains_.begin (); d != config_.domains_.end (); ++d) {
    bool ret = createDomain (*d);
    if (!ret) {
      error = true;
      break;
    }
  }

  if (error) {
    domains_.clear ();
    ecrt_release_master (master_);
    master_ = 0;
    return false;
  }

  return true;
}

void ECHardware::cleanupHook() {
  ecrt_release_master(master_);
}

bool ECHardware::startHook () {
  int ret = ecrt_master_activate (master_);
  if (ret) {
    return false; //  ret < 0 >> error
  } else {
    return true; //  ret == 0 >> OK
  }
}

void ECHardware::stopHook () {
  ecrt_master_deactivate (master_);	
}

void ECHardware::updateHook () {
  ecrt_master_receive (master_);

  for (std::vector<ECDomain::Ptr>::iterator d = domains_.begin (); d != domains_.end (); ++d) {
    (*d)->update ();
  }

  ecrt_master_send (master_);
}

bool ECHardware::createDomain (const ECDomainConfig& cfg) {
  ECDomain::Ptr dom = ECDomain::Ptr (new ECDomain (master_));
  if (!(*dom)) {
    RTT::log(RTT::Error) << "Unable to create domain" << RTT::endlog();
    return false;
  }
  
  for (std::vector<ECDriverConfig>::const_iterator driver_cfg = cfg.drivers_.begin();
       driver_cfg != cfg.drivers_.end();
       ++driver_cfg) {
    ECDriver::Ptr driver = createDriver(*driver_cfg);
    if (!driver) {
      return false;  
    }
    
    if (!driver->configure(driver_cfg->driver_cfg_)) {
      return false;
    }
    
    dom->addDriver(driver);
  }
  
  if (!dom->configure()) {
    return false;
  }
  
  domains_.push_back (dom);
  return true;
}

ECDriver::Ptr ECHardware::createDriver(const ECDriverConfig& cfg) {
  TerminalMap::iterator it = terminals_.find(cfg.terminal_);
  if (it == terminals_.end()) {
    RTT::log(RTT::Error) << "There is no terminal [" << cfg.terminal_ << "] requested by driver ["
                         << cfg.name_ << "]" << RTT::endlog();  
    return ECDriver::Ptr();
  }
  
  ECDriverFactory::Ptr factory = this->getProvider<ECDriverFactory>(cfg.type_);
  if (!factory) {
    RTT::log(RTT::Error) << "Unable to create driver [" << cfg.name_ << "] of type ["
                         << cfg.type_ << "]" << RTT::endlog(); 
    return ECDriver::Ptr();
  }
  
  ECDriver::Ptr drv = factory->createDriver(cfg.name_);
  drv->setSlave(it->second);
  return drv;
}

ORO_CREATE_COMPONENT(ECHardware)

