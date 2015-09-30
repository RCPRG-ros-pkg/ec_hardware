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

#include "ECDomain.h"
#include <ec_hardware/ECDriver.h>
#include <ec_hardware/ECPDOEntryInterface.h>

ECDomain::ECDomain(ec_master_t *master) {
  domain_ = ecrt_master_create_domain(master);
}

ECDomain::~ECDomain() {
}

void ECDomain::update() {
  ecrt_domain_process(domain_);

  for (std::vector<ECDriver::Ptr>::iterator driver = drivers_.begin();
      driver != drivers_.end(); ++driver) {
    (*driver)->updateInputs();
  }

  for (std::vector<ECDriver::Ptr>::iterator driver = drivers_.begin();
      driver != drivers_.end(); ++driver) {
    (*driver)->updateOutputs();
  }

  ecrt_domain_queue(domain_);
}

bool ECDomain::configure() {
  for (std::vector<ECDriver::Ptr>::iterator driver = drivers_.begin();
      driver != drivers_.end(); ++driver) {
    std::vector<ECPDOEntryInterface*> pdo_entries = (*driver)->getPDOConfig();
    ECSlave::Ptr slave = (*driver)->getSlave();

    for (std::vector<ECPDOEntryInterface*>::iterator pdo = pdo_entries.begin();
        pdo != pdo_entries.end(); ++pdo) {
      int offset = ecrt_slave_config_reg_pdo_entry(
          slave->slave_, (*pdo)->getIndex(), (*pdo)->getSubIndex(), domain_, 0);
      if (offset < 0) {
        RTT::log(RTT::Error) << "Unable to map PDO entry ["<< (*pdo)->getIndex()
                             << ":" << (*pdo)->getSubIndex() << "] to domain" << RTT::endlog();
        return false;
      } else {
        (*(*pdo)->getOffsetPtr()) = offset;
      }

    }
  }

  return true;
}

bool ECDomain::start() {
  uint8_t * domain_ptr = ecrt_domain_data(domain_);

  for (std::vector<ECDriver::Ptr>::iterator driver = drivers_.begin();
      driver != drivers_.end(); ++driver) {
    std::vector<ECPDOEntryInterface*> pdo_entries = (*driver)->getPDOConfig();

    for (std::vector<ECPDOEntryInterface*>::iterator pdo = pdo_entries.begin();
        pdo != pdo_entries.end(); ++pdo) {
      (*pdo)->setDomainPtr(domain_ptr);
    }
  }

  return true;
}

void ECDomain::addDriver(const ECDriver::Ptr driver) {
  drivers_.push_back(driver);
}

ec_pdo_entry_reg_t ECDomain::createPDOEntry(const ECSlave::Ptr slave,
                                            ECPDOEntryInterface* const pdo) {
  std::cout << pdo->getIndex() << ":" << pdo->getSubIndex() << std::endl;
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
