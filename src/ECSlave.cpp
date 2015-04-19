
#include "ECSlave.h"

ECSlave::ECSlave(uint32_t vendor_id, uint32_t product_id):
  vendor_id_(vendor_id),
  product_id_(product_id) {
}

bool ECSlave::configure(ec_master_t *master, unsigned int station_alias,
                        unsigned int station_offset) {
  slave_ = ecrt_master_slave_config(master, station_alias, station_offset, vendor_id_, product_id_);
  if (!slave_) {
    return false;
  }
  
  station_alias_ = station_alias;
  station_offset_ = station_offset;
  
  return true;
}

bool ECSlave::configurePDO(const std::vector<ec_pdo_info_t> &tpdo,
                           const std::vector<ec_pdo_info_t> &rpdo) {
  std::vector<ec_sync_info_t> syncs;
  if (!rpdo.empty()) {
    ec_sync_info_t sc;
    sc.dir = EC_DIR_OUTPUT;
    sc.index = 2;
    sc.n_pdos = rpdo.size();
    sc.pdos = const_cast<ec_pdo_info_t*>(rpdo.data());
    syncs.push_back(sc);
  }

  if (!tpdo.empty()) {
    ec_sync_info_t sc;
    sc.dir = EC_DIR_INPUT;
    sc.index = 3;
    sc.n_pdos = tpdo.size();
    sc.pdos = const_cast<ec_pdo_info_t*>(tpdo.data());
    syncs.push_back(sc);
  }

  if (ecrt_slave_config_pdos(slave_, syncs.size(), syncs.data())) {
    return false;
  }
  return true;
}

template <>
void ECSlave::addSDOConfig(uint16_t index, uint8_t subindex, uint8_t value) {
	  ecrt_slave_config_sdo8(slave_, index, subindex, value);
}

template <>
void ECSlave::addSDOConfig(uint16_t index, uint8_t subindex, int8_t value) {
	  ecrt_slave_config_sdo8(slave_, index, subindex, value);
}

template <>
void ECSlave::addSDOConfig(uint16_t index, uint8_t subindex, uint16_t value) {
	  ecrt_slave_config_sdo16(slave_, index, subindex, value);
}

template <>
void ECSlave::addSDOConfig(uint16_t index, uint8_t subindex, int16_t value) {
	  ecrt_slave_config_sdo16(slave_, index, subindex, value);
}

template <>
void ECSlave::addSDOConfig(uint16_t index, uint8_t subindex, uint32_t value) {
	  ecrt_slave_config_sdo32(slave_, index, subindex, value);
}

template <>
void ECSlave::addSDOConfig(uint16_t index, uint8_t subindex, int32_t value) {
	  ecrt_slave_config_sdo32(slave_, index, subindex, value);
}
