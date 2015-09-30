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

#include <ec_hardware/ECSlave.h>

ECSlave::ECSlave(uint32_t vendor_id, uint32_t product_id)
    : vendor_id_(vendor_id),
      product_id_(product_id),
      station_alias_(0),
      station_offset_(0),
      slave_(0) {
}

bool ECSlave::configure(ec_master_t *master, unsigned int station_alias,
                        unsigned int station_offset) {
  slave_ = ecrt_master_slave_config(master, station_alias, station_offset,
                                    vendor_id_, product_id_);
  if (!slave_) {
    return false;
  }

  station_alias_ = station_alias;
  station_offset_ = station_offset;

  return true;
}

bool ECSlave::configurePDO(const std::vector<ec_pdo_info_t> &tpdo,
                           const std::vector<ec_pdo_info_t> &rpdo) {
  ec_sync_info_t sc[2];
  int syncs_num = 0;

  if (!rpdo.empty()) {
    //ec_sync_info_t sc;
    sc[syncs_num].dir = EC_DIR_OUTPUT;
    sc[syncs_num].index = 2;
    sc[syncs_num].n_pdos = rpdo.size();
    sc[syncs_num].pdos = const_cast<ec_pdo_info_t*>(rpdo.data());
    ++syncs_num;
  }

  if (!tpdo.empty()) {
    sc[syncs_num].dir = EC_DIR_INPUT;
    sc[syncs_num].index = 3;
    sc[syncs_num].n_pdos = tpdo.size();
    sc[syncs_num].pdos = const_cast<ec_pdo_info_t*>(tpdo.data());
    ++syncs_num;
  }

  if (ecrt_slave_config_pdos(slave_, syncs_num, sc)) {
    return false;
  }
  return true;
}

void ECSlave::configureDC(uint16_t assign_activate, uint32_t sync0_cycle,
                     int32_t sync0_shift, uint32_t sync1_cycle,
                     int32_t sync1_shift) {
  ecrt_slave_config_dc(slave_, assign_activate, sync0_cycle, sync0_shift, sync1_cycle, sync1_shift);
}

template<>
void ECSlave::addSDOConfig(uint16_t index, uint8_t subindex, uint8_t value) {
  ecrt_slave_config_sdo8(slave_, index, subindex, value);
}

template<>
void ECSlave::addSDOConfig(uint16_t index, uint8_t subindex, int8_t value) {
  ecrt_slave_config_sdo8(slave_, index, subindex, value);
}

template<>
void ECSlave::addSDOConfig(uint16_t index, uint8_t subindex, uint16_t value) {
  ecrt_slave_config_sdo16(slave_, index, subindex, value);
}

template<>
void ECSlave::addSDOConfig(uint16_t index, uint8_t subindex, int16_t value) {
  ecrt_slave_config_sdo16(slave_, index, subindex, value);
}

template<>
void ECSlave::addSDOConfig(uint16_t index, uint8_t subindex, uint32_t value) {
  ecrt_slave_config_sdo32(slave_, index, subindex, value);
}

template<>
void ECSlave::addSDOConfig(uint16_t index, uint8_t subindex, int32_t value) {
  ecrt_slave_config_sdo32(slave_, index, subindex, value);
}
