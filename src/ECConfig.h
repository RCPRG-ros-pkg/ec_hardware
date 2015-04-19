#ifndef ECCONFIG_H
#define ECCONFIG_H

#include <string>
#include <vector>

#include <yaml-cpp/yaml.h>

#include <ecrt.h>

class ECDriverConfig
{
public:
	ECDriverConfig();
	~ECDriverConfig();
	bool parse(const YAML::Node &node);
	YAML::Node driver_cfg_;
	std::string name_;
	std::string type_;
	std::string terminal_;
};

class ECTerminalConfig
{
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
private:
	bool parsePDO(const YAML::Node &node, ec_pdo_info_t* pdo);
	bool parsePDOEntry(const YAML::Node &node, ec_pdo_entry_info_t* pdo);
	std::vector<std::vector<ec_pdo_entry_info_t> > pdo_entries_;
};

class ECDomainConfig
{
public:
	ECDomainConfig();
	~ECDomainConfig();
	bool parse(const YAML::Node &node);
	std::string name_;
	unsigned int period_;
	std::vector<ECDriverConfig> drivers_;
};

class ECConfig
{
public:
	ECConfig();
	~ECConfig();
	bool parse(const std::string &input);
	int master_idx_;
	std::vector<ECTerminalConfig> terminals_;
	std::vector<ECDomainConfig> domains_;
};

#endif  // ECCONFIG_H
