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

#ifndef ECCONFIG_H_
#define ECCONFIG_H_

#include <yaml-cpp/yaml.h>
#include <ecrt.h>

#include <string>
#include <vector>


class ECDriverConfig {
 public:
  ECDriverConfig();
  ~ECDriverConfig();
  bool parse(const YAML::Node &node);
  YAML::Node driver_cfg_;
  std::string name_;
  std::string type_;
  std::string terminal_;
};

class ECTerminalConfig {
 public:
  ECTerminalConfig();
  ~ECTerminalConfig();
  bool parse(const YAML::Node &node);
  std::string name_;
  int vendor_id_;
  int device_id_;
  int station_alias_;
  int station_offset_;

  std::vector<ec_pdo_info_t> tpdo_;
  std::vector<ec_pdo_info_t> rpdo_;

  bool parsePDO(const YAML::Node &node, ec_pdo_info_t* pdo);
  bool parsePDOEntry(const YAML::Node &node, ec_pdo_entry_info_t* pdo);
  std::vector<std::vector<ec_pdo_entry_info_t>* > pdo_entries_;
};

class ECDomainConfig {
 public:
  ECDomainConfig();
  ~ECDomainConfig();
  bool parse(const YAML::Node &node);
  std::string name_;
  unsigned int period_;
  std::vector<ECDriverConfig> drivers_;
};

class ECConfig {
 public:
  ECConfig();
  ~ECConfig();
  bool parse(const std::string &input);
  int master_idx_;
  std::vector<ECTerminalConfig> terminals_;
  std::vector<ECDomainConfig> domains_;
};

#endif  // ECCONFIG_H_
