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

#ifndef ECSLAVE_H
#define ECSLAVE_H

#include <vector>

#include <boost/smart_ptr/shared_ptr.hpp>

#include <ecrt.h>

class ECSlave {
 public:
  friend class ECDomain;
  typedef boost::shared_ptr<ECSlave> Ptr;

  ECSlave(uint32_t vendor_id, uint32_t product_id);
  bool configure(ec_master_t *master, unsigned int station_alias,
                 unsigned int station_offset);

  bool configurePDO(const std::vector<ec_pdo_info_t> &tpdo,
                    const std::vector<ec_pdo_info_t> &rpdo);

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

  template<class type>
  void addSDOConfig(const uint16_t index, const uint8_t subindex, const type value) {
    ecrt_slave_config_sdo(slave_, index, subindex, (uint8_t*)&value, sizeof(value));
  }

  void configureDC(uint16_t assign_activate, /**< AssignActivate word. */
                   uint32_t sync0_cycle, /**< SYNC0 cycle time [ns]. */
                   int32_t sync0_shift, /**< SYNC0 shift time [ns]. */
                   uint32_t sync1_cycle, /**< SYNC1 cycle time [ns]. */
                   int32_t sync1_shift /**< SYNC1 shift time [ns]. */);

  template<class type>
  int sdoDownload(int32_t index, int16_t subindex,const type &data) {
    int abort_code;
    int ret = ecrt_slave_sdo_download(slave_, index, subindex, &data, sizeof(data), &abort_code);
    if (ret < 0) {
      return abort_code;
    } else {
      return 0;
    }
  }

  template<class type>
  int sdoUpload(int32_t index, int16_t subindex,type *data) {
    int abort_code;
    int ret = ecrt_slave_sdo_upload(slave_, index, subindex, data, sizeof(type), &abort_code);
    if (ret < 0) {
      return abort_code;
    } else {
      return 0;
    }
  }
 private:
  const uint32_t vendor_id_;
  const uint32_t product_id_;

  ec_slave_config_t* slave_;

  unsigned int station_alias_;
  unsigned int station_offset_;
};

#endif
