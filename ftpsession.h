#ifndef FTPSESSION_H
#define FTPSESSION_H

#include "ftpuser.h"
#include <string>

class FTPSession
{
public:
	FTPSession() {  }
	FTPSession(const FTPUser *user, std::string cwd, int cmd_sock, int data_sock) : user(user), cwd(cwd), cmd_sock(cmd_sock), data_sock(data_sock) {  };
	const FTPUser *user;
	std::string cwd;
	int cmd_sock;
	int data_sock;
private:

};

#endif
