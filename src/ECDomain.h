#ifndef ECDOMAIN_H
#define ECDOMAIN_H

#include <boost/smart_ptr/shared_ptr.hpp>

#include <ecrt.h>

#include "ECDriver.h"
#include "ECSlave.h"

class ECDomain {
 public:
 typedef boost::shared_ptr<ECDomain> Ptr;
 
  ECDomain(ec_master_t *master);
  ~ECDomain();
  bool start();
  void update();
  operator bool() {
    return !(domain_ == NULL);
  };
  void addDriver(const ECDriver::Ptr);
  bool configure();

 private:
  ec_pdo_entry_reg_t createPDOEntry(ECSlave::Ptr slave, ECPDOEntryInterface* const pdo);
 
  ec_domain_t *domain_;

  std::vector<ECDriver::Ptr> drivers_;
  std::vector<ec_pdo_entry_reg_t> pdo_entries_;
};

#endif  // ECDOMAIN_HH
