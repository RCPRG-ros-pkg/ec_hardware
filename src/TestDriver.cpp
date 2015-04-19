
#include <rtt/plugin/ServicePlugin.hpp>

#include "ECDriver.h"
#include "ECDriverFactory.h"
#include "ECPDOEntry.h"

class TestDriver : public ECDriver {
public:
  TestDriver(const std::string &name) :
    ECDriver(name),
    entry1_(123, 0),
    entry2_(233, 2) {
    
    this->addPDOEntry(&entry1_);
    this->addPDOEntry(&entry2_);
  }
  
  virtual bool configureHook(const YAML::Node &cfg) {
    return true;
  }
  
  virtual void updateInputs() {
    uint8_t pos;
    
    pos = entry1_.read();
  }
  
  virtual void updateOutputs() {
    double current = 100.0;
    entry2_.write(current);
  }
  
private:
  ECPDOEntry<uint8_t> entry1_;
  ECPDOEntry<double> entry2_;
};

char name[] = "test_driver";

typedef ECDriverFactoryService<name, TestDriver> TestDriverFactory;

ORO_SERVICE_NAMED_PLUGIN(TestDriverFactory, "test_driver");
