#include "ipfilter.h"
#include <string>
#include <fstream>
#include <iostream>
#include <cassert>

bool IPFilter::canPass(const std::string ip)
{
	// no configuration file, all ip can pass
	if(config_file.size() == 0)
		return true;

	int maxlen = 20;
	char line[maxlen];

	std::ifstream infile;
	infile.open(config_file.c_str());
	assert(infile.is_open());

	bool found = false;
	while(infile.getline(line, maxlen))
	{
		if(std::string(line).find(ip) != std::string::npos)
		{
			found = true;
			break;
		}
	}
	
	infile.close();
	return !found;
}
