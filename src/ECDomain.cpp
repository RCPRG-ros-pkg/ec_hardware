#include "ECDomain.h"
#include "ECDriver.h"
#include "ECPDOEntryInterface.h"

ECDomain::ECDomain(ec_master_t *master) {
    domain_ = ecrt_master_create_domain(master);
}

ECDomain::~ECDomain() {
}

void ECDomain::update() {
    ecrt_domain_process(domain_);

    ecrt_domain_queue(domain_);
}

bool ECDomain::configure() {
  for (std::vector<ECDriver::Ptr>::iterator driver = drivers_.begin();
       driver != drivers_.end();
       ++driver) {
    std::vector<ECPDOEntryInterface*> pdo_entries = (*driver)->getPDOConfig();
    ECSlave::Ptr slave = (*driver)->getSlave();
    
    for (std::vector<ECPDOEntryInterface*>::iterator pdo = pdo_entries.begin();
         pdo != pdo_entries.end();
         ++pdo) {
      pdo_entries_.push_back(createPDOEntry(slave, *pdo));
    }
  }
  
  ec_pdo_entry_reg_t en_end;
  en_end.index = 0;
  pdo_entries_.push_back(en_end);
  
  int ret = ecrt_domain_reg_pdo_entry_list(domain_, &pdo_entries_[0]);
  if (ret != 0) {
    RTT::log(RTT::Error) << "Unable to configure PDO entry list for domain" << RTT::endlog();
    return false;
  }
  
  return true;
}

bool ECDomain::start() {
	uint8_t * domain_ptr = ecrt_domain_data(domain_);

	for (std::vector<ECDriver::Ptr>::iterator driver = drivers_.begin();
	     driver != drivers_.end();
	     ++driver) {
	    std::vector<ECPDOEntryInterface*> pdo_entries = (*driver)->getPDOConfig();

	    for (std::vector<ECPDOEntryInterface*>::iterator pdo = pdo_entries.begin();
	         pdo != pdo_entries.end();
	         ++pdo) {
	      (*pdo)->setDomainPtr(domain_ptr);
	    }
	  }

	return true;
}

void ECDomain::addDriver(const ECDriver::Ptr driver) {
  drivers_.push_back(driver);
}

ec_pdo_entry_reg_t ECDomain::createPDOEntry(const ECSlave::Ptr slave, ECPDOEntryInterface* const pdo) {
  ec_pdo_entry_reg_t pdo_entry;
  pdo_entry.alias = slave->getStationAlias();
  pdo_entry.position = slave->getStationOffset();
  pdo_entry.vendor_id = slave->getVendorId();
  pdo_entry.product_code = slave->getProductId();
  pdo_entry.index = pdo->getIndex();
  pdo_entry.subindex = pdo->getSubIndex();
  pdo_entry.offset = pdo->getOffsetPtr();
  pdo_entry.bit_position = 0;
  return pdo_entry;
}
