#ifndef ECDRIVERFACTORY_H
#define ECDRIVERFACTORY_H

#include <rtt/RTT.hpp>
#include <ecrt.h>

#include "ECDriver.h"

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
