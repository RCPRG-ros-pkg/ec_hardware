#ifndef ECDRIVER_H
#define ECDRIVER_H

#include <boost/smart_ptr/shared_ptr.hpp>
#include <yaml-cpp/yaml.h>
#include <rtt/Service.hpp>

#include <ecrt.h>

#include "ECPDOEntryInterface.h"
#include "ECSlave.h"

class ECDriver {
public:
  typedef boost::shared_ptr<ECDriver> Ptr;

  ECDriver(const std::string &name){
  }
  virtual ~ECDriver() {
  }
  
  bool configure(const YAML::Node &cfg) {
    if (!slave_) {
      return false;
    }
    
    return configureHook(cfg);
  }
  RTT::Service::shared_ptr provides();
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

private:
  ECSlave::Ptr slave_;
  std::vector<ECPDOEntryInterface *> pdo_entries_;
};

#endif //  ECDRIVER_H
