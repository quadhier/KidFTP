#ifndef IPFILTER_H
#define IPFILTER_H

#include <string>

class IPFilter
{
public:
	IPFilter(const std::string file) : config_file(file) {  }
	bool canPass(const std::string ip);
private:
	// the configuration file is in fact a blacklist
	const std::string config_file;
};

#endif
