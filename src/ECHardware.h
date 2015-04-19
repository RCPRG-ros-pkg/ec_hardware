#ifndef ECHARDWARE_H
#define ECHARDWARE_H

#include <vector>
#include <map>

#include <boost/smart_ptr/shared_ptr.hpp>

#include <rtt/TaskContext.hpp>

#include <ecrt.h>

#include "ECConfig.h"
#include "ECDomain.h"
#include "ECSlave.h"

class ECHardware : public RTT::TaskContext {
   public:
    explicit ECHardware(const std::string &name);
    ~ECHardware();

    bool configureHook();
    void cleanupHook();
    bool startHook();
    void updateHook();
    void stopHook();

   private:
    typedef std::map<std::string, ECSlave::Ptr> TerminalMap;
   
    bool createDomain(const ECDomainConfig& cfg);
    ECDriver::Ptr createDriver(const ECDriverConfig& cfg);

    ec_master_t* master_;
	  TerminalMap terminals_;
    std::vector<ECDomain::Ptr> domains_;
    std::string config_str_;
    ECConfig config_;
};

#endif  // ECHARDWARE_H
