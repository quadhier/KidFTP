#ifndef FTPSERVER_H
#define FTPSERVER_H
#include "ipfilter.h"
#include "ftpuser.h"
#include "ftpsession.h"
#include <string>
#include <vector>
#include <map>

class FTPServer 
{
public:
	FTPServer(int port, std::string ip_file, std::string user_file, std::string root_dir);
	~FTPServer();
	void run();

private:
	int port;
	// ip filter 
	IPFilter *ip_filter; 
	// user configuration file
	std::string user_file;
	// real root directory for all the files in ftp server
	std::string root_dir;
	// registered users
	std::vector<FTPUser> users;
	// session information
	std::map<int, FTPSession> sockfd2session;

	// load user information from user info file
	void load_users();
	// login and initiate the session
	int init_session(int connfd);
	// deal with different user commands	
	void dispatch_cmd(int connfd);
	
};
#endif
