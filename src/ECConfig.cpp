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

#include "ECConfig.h"

#include <rtt/Logger.hpp>

ECConfig::ECConfig() {
}

ECConfig::~ECConfig() {
}

bool ECConfig::parse(const std::string &input) {
  try {
    YAML::Node config = YAML::Load(input);

    if (config["master_idx"]) {
      master_idx_ = config["master_idx"].as<int>();
    } else {
      return false;
    }

    if (config["terminals"]) {
      YAML::Node terminals_cfg = config["terminals"];
      for (YAML::Node::iterator t = terminals_cfg.begin();
          t != terminals_cfg.end(); ++t) {
        terminals_.push_back(ECTerminalConfig());
        if (!terminals_.back().parse(*t)) {
          return false;
        }
      }
    } else {
      return false;
    }

    if (config["domains"]) {
      YAML::Node domains_cfg = config["domains"];
      for (YAML::Node::iterator d = domains_cfg.begin(); d != domains_cfg.end();
          ++d) {
        ECDomainConfig dcfg;
        if (!dcfg.parse(*d)) {
          return false;
        } else {
          domains_.push_back(dcfg);
        }
      }
    } else {
      return false;
    }
    return true;
  } catch (std::exception& e) {
    RTT::log(RTT::Error) << e.what() << RTT::endlog();
    return false;
  }
}

ECDomainConfig::ECDomainConfig() {
}

ECDomainConfig::~ECDomainConfig() {
}

bool ECDomainConfig::parse(const YAML::Node &node) {
  if (node["period"]) {
    period_ = node["period"].as<unsigned int>();
  } else {
    RTT::log(RTT::Error) << "domain missing period field" << RTT::endlog();
    return false;
  }

  if (node["name"]) {
    name_ = node["name"].as<std::string>();
  } else {
    RTT::log(RTT::Error) << "domain missing name field" << RTT::endlog();
    return false;
  }

  if (node["drivers"]) {
    YAML::Node drivers_cfg = node["drivers"];
    for (YAML::Node::iterator t = drivers_cfg.begin(); t != drivers_cfg.end();
        ++t) {
      ECDriverConfig dcfg;
      if (!dcfg.parse(*t)) {
        return false;
      } else {
        drivers_.push_back(dcfg);
      }
    }
  } else {
    RTT::log(RTT::Error) << "domain missing name field" << RTT::endlog();
    return false;
  }
  return true;
}

ECTerminalConfig::ECTerminalConfig() {
}

ECTerminalConfig::~ECTerminalConfig() {
}

bool ECTerminalConfig::parse(const YAML::Node &node) {
  if (node["name"]) {
    name_ = node["name"].as<std::string>();
  } else {
    RTT::log(RTT::Error) << "missing terminal name" << RTT::endlog();
    return false;
  }

  if (node["vendor_id"]) {
    vendor_id_ = node["vendor_id"].as<int>();
  } else {
    RTT::log(RTT::Error) << "missing terminal vendor_id" << RTT::endlog();
    return false;
  }

  if (node["device_id"]) {
    device_id_ = node["device_id"].as<int>();
  } else {
    RTT::log(RTT::Error) << "missing terminal device_id" << RTT::endlog();
    return false;
  }

  if (node["station_alias"]) {
    station_alias_ = node["station_alias"].as<int>();
  } else {
    return false;
  }

  if (node["station_offset"]) {
    station_offset_ = node["station_offset"].as<int>();
  } else {
    RTT::log(RTT::Error) << "missing terminal station_offset" << RTT::endlog();
    return false;
  }

  if (node["TPDO"]) {
    YAML::Node tpdo_cfg = node["TPDO"];
    for (YAML::Node::iterator p = tpdo_cfg.begin(); p != tpdo_cfg.end(); ++p) {
      ec_pdo_info_t pdo;
      if (parsePDO(*p, &pdo)) {
        tpdo_.push_back(pdo);
      } else {
        return false;
      }
    }
  }

  if (node["RPDO"]) {
    YAML::Node rpdo_cfg = node["RPDO"];
    for (YAML::Node::iterator p = rpdo_cfg.begin(); p != rpdo_cfg.end(); ++p) {
      ec_pdo_info_t pdo;
      if (parsePDO(*p, &pdo)) {
        rpdo_.push_back(pdo);
      } else {
        return false;
      }
    }
  }
  return true;
}

bool ECTerminalConfig::parsePDO(const YAML::Node &node, ec_pdo_info_t *pdo) {
  if (node["index"]) {
    pdo->index = node["index"].as<int>();
  } else {
    return false;
  }

  if (node["entries"]) {
    YAML::Node entries_cfg = node["entries"];
    std::vector<ec_pdo_entry_info_t>* entries = new std::vector<ec_pdo_entry_info_t>;
    for (YAML::Node::iterator e = entries_cfg.begin(); e != entries_cfg.end();
        ++e) {
      ec_pdo_entry_info_t entry;
      if (parsePDOEntry(*e, &entry)) {
        entries->push_back(entry);
      } else {
        return false;
      }
    }
    pdo_entries_.push_back(entries);
    pdo->n_entries = pdo_entries_.back()->size();
    pdo->entries = pdo_entries_.back()->data();
  } else {
    pdo->n_entries = 0;
    pdo->entries = 0;
  }
  return true;
}

bool ECTerminalConfig::parsePDOEntry(const YAML::Node &node,
                                     ec_pdo_entry_info_t *pdo) {
  if (node["index"]) {
    pdo->index = node["index"].as<int>();
  } else {
    return false;
  }

  if (node["subindex"]) {
    pdo->subindex = node["subindex"].as<int>();
  } else {
    return false;
  }

  if (node["bit_length"]) {
    pdo->bit_length = node["bit_length"].as<int>();
  } else {
    return false;
  }
  return true;
}

ECDriverConfig::ECDriverConfig() {
}

ECDriverConfig::~ECDriverConfig() {
}

bool ECDriverConfig::parse(const YAML::Node &node) {
  if (node["name"]) {
    name_ = node["name"].as<std::string>();
  } else {
    return false;
  }

  if (node["type"]) {
    type_ = node["type"].as<std::string>();
  } else {
    return false;
  }

  if (node["terminal"]) {
    terminal_ = node["terminal"].as<std::string>();
  } else {
    return false;
  }

  driver_cfg_ = node;
  return true;
}
