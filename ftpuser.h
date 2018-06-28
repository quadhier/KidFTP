#ifndef FTPUSER_H
#define FTPUSER_H


#include <string>

class FTPUser
{
public:
	FTPUser(std::string n, std::string p, std::string h) : name(n), passwd(p), home_dir(h) {  }
	std::string name;
	std::string passwd;
	std::string home_dir;
private:
	
};


#endif
