#ifndef ECSLAVE_H
#define ECSLAVE_H

#include <vector>

#include <boost/smart_ptr/shared_ptr.hpp>

#include <ecrt.h>

class ECSlave {
public:
  typedef boost::shared_ptr<ECSlave> Ptr;

  ECSlave(uint32_t vendor_id, uint32_t product_id);
  bool configure(ec_master_t *master, unsigned int station_alias, unsigned int station_offset);
  
  bool configurePDO(const std::vector<ec_pdo_info_t> &tpdo, const std::vector<ec_pdo_info_t> &rpdo);
  
  uint32_t getVendorId() {
    return vendor_id_;
  }
 
  uint32_t getProductId() {
    return product_id_;
  }
  
  unsigned int getStationAlias() {
    return station_alias_;
  }
  
  unsigned int getStationOffset() {
    return station_offset_;
  }

  template <class type>
  void addSDOConfig(uint16_t index, uint8_t subindex, type value) {
	  ecrt_slave_config_sdo(slave_, index, subindex, &value, sizeof(value));
  }

private:
  const uint32_t vendor_id_;
  const uint32_t product_id_;

  unsigned int station_alias_;
  unsigned int station_offset_;

  ec_slave_config_t* slave_;
};

#endif
