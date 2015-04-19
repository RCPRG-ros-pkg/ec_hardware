#include "ECConfig.h"

ECConfig::ECConfig()
{
}

ECConfig::~ECConfig()
{
}

bool ECConfig::parse(const std::string &input)
{
  try {
	YAML::Node config = YAML::Load(input);

	if(config["master_idx"]) {
		master_idx_ = config["master_idx"].as<int>();
	} else {
		return false;
	}
  std::cout << "terminals begin" << std::endl;
	if (config["terminals"]) {
		YAML::Node terminals_cfg = config["terminals"];
		for (YAML::Node::iterator t = terminals_cfg.begin(); t != terminals_cfg.end(); ++t) {
			terminals_.push_back(ECTerminalConfig());
			if (!terminals_.back().parse(*t)) {
				return false;
			}
		}
	} else {
		return false;
	}
  std::cout << "terminals end" << std::endl;
  std::cout << "domains begin" << std::endl;
	if (config["domains"]) {
		YAML::Node domains_cfg = config["domains"];
		for (YAML::Node::iterator d = domains_cfg.begin(); d != domains_cfg.end(); ++d) {
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
	  std::cout << e.what() << std::endl;
	  return false;
	}
}

ECDomainConfig::ECDomainConfig()
{
}

ECDomainConfig::~ECDomainConfig()
{
}

bool ECDomainConfig::parse(const YAML::Node &node)
{
  std::cout << "domain config begin" << std::endl;
	if (node["period"]) {
		period_ = node["period"].as<unsigned int>();
	} else {
	  std::cout << "domain missing period fild" << std::endl;
		return false;
	}

	if (node["name"]) {
		name_ = node["name"].as<std::string>();
	} else {
	  std::cout << "domain missing name fild" << std::endl;
		return false;
	}
  
  std::cout << "drivers begin" << std::endl;
	if(node["drivers"]) {
		YAML::Node drivers_cfg = node["drivers"];
		for (YAML::Node::iterator t = drivers_cfg.begin(); t != drivers_cfg.end(); ++t) {
			ECDriverConfig dcfg;
			if (!dcfg.parse(*t)) {
				return false;
			} else {
				drivers_.push_back(dcfg);
			}
		}
	} else {
	  std::cout << "domain missing name fild" << std::endl;
		return false;
	}
  std::cout << "drivers end" << std::endl;
	return true;
}

ECTerminalConfig::ECTerminalConfig()
{
}

ECTerminalConfig::~ECTerminalConfig()
{
}

bool ECTerminalConfig::parse(const YAML::Node &node)
{
	if (node["name"]) {
		name_ = node["name"].as<std::string>();
	} else {
	  std::cout << "missing terminal name" << std::endl;
		return false;
	}

	if (node["vendor_id"]) {
		vendor_id_ = node["vendor_id"].as<int>();
	} else {
		std::cout << "missing terminal vendor_id" << std::endl;
		return false;
	}

	if (node["device_id"]) {
		device_id_ = node["device_id"].as<int>();
	} else {
	  std::cout << "missing terminal device_id" << std::endl;
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
	  std::cout << "missing terminal station_offset" << std::endl;
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

bool ECTerminalConfig::parsePDO(const YAML::Node &node, ec_pdo_info_t *pdo)
{
	if (node["index"]) {
		pdo->index = node["index"].as<int>();
	} else {
		return false;
	}

	if (node["entries"]) {
		YAML::Node entries_cfg = node["entries"];
		std::vector<ec_pdo_entry_info_t> entries;
		for (YAML::Node::iterator e = entries_cfg.begin(); e != entries_cfg.end(); ++e) {
			ec_pdo_entry_info_t entry;
			if (parsePDOEntry(*e, &entry)) {
				entries.push_back(entry);
			} else {
				return false;
			}
		}
		pdo_entries_.push_back(entries);
		pdo->n_entries = pdo_entries_.back().size();
		pdo->entries = &pdo_entries_.back()[0];
	} else {
		return false;
	}

	return true;
}

bool ECTerminalConfig::parsePDOEntry(const YAML::Node &node, ec_pdo_entry_info_t *pdo)
{
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

ECDriverConfig::ECDriverConfig()
{
}

ECDriverConfig::~ECDriverConfig()
{
}

bool ECDriverConfig::parse(const YAML::Node &node)
{
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
