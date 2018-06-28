#ifndef FTPSERVER_H
#define FTPSERVER_H
#include "iplist.h"
#include <string>

class FTPServer 
{
public:
	FTPServer(std::string config_file) : blacklist(config_file) {  }
private:
	IPList blacklist;
};
#endif
