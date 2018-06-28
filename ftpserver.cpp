#include "ftpserver.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <cstdio>
#include <errno.h>
#include <cstring>
#include <unistd.h>
#include <iostream>
#include <iterator>
#include <string>

FTPServer::FTPServer(int port,
					 std::string ip_file, 
					 std::string user_file,
					 std::string root_dir)
	: ip_filter(new IPFilter(ip_file)), 
	  user_file(user_file),
	  root_dir(root_dir),
	  port(port)
{
	// load_users();
	users.push_back(FTPUser("qdh", "xixi", "/qdh"));
	users.push_back(FTPUser("wb", "haha", "/wb"));
}

FTPServer::~FTPServer()
{
	delete(ip_filter);
}

void FTPServer::load_users()
{

}

void FTPServer::run()
{
	const char *ip = "127.0.0.1";
	struct sockaddr_in address;
	address.sin_family = AF_INET;
//	inet_pton(AF_INET, ip, &address.sin_addr);
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_port = htons(port);
	
	int sockfd = socket(PF_INET, SOCK_STREAM, 0);
	if(sockfd == -1)
	{
		perror("create socket failed");
		exit(EXIT_FAILURE);
	}
	
	// for ease of debug
	// let port reusable immediately without TIME_WAIT to wait
	int flag = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));

	if(bind(sockfd, (struct sockaddr *)&address, sizeof(address)) == -1)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	if(listen(sockfd, 5) == -1)
	{
		perror("listen failed");
		exit(EXIT_FAILURE);
	}
		
	struct sockaddr_in clientaddr;
	socklen_t clientaddr_len = sizeof(clientaddr);

	while(1)
	{
		int connfd = accept(sockfd, (struct sockaddr *)&clientaddr, &clientaddr_len);
		
		char client_ip[29];
		inet_ntop(AF_INET, &clientaddr.sin_addr, client_ip, 20);
		if(!ip_filter->canPass(client_ip))
		{
			close(connfd);
			continue;
		}

		if(init_session(connfd) != -1)
		{
			dispatch_cmd(connfd);
		}
		else
		{
			close(connfd);
		}
	}

}

int FTPServer::init_session(int connfd)
{
	int len = 20;
	char msg[len];
	send(connfd, "220\r\n", 5, 0);
	recv(connfd, msg, len, 0);
	
	// check user name
	std::string name = std::string(strchr(msg, ' ') + 1);
	name = name.substr(0, name.size() - 2); // drop \r\n
	bool found = false;
	int idx = -1;
	for(int i = 0; i < users.size(); i++)
	{
		if(users[i].name == name)
		{
			found = true;
			idx = i;
		}
	}

	if(found)
	{
		// user name okay, need password
		send(connfd, "331\r\n", 5, 0);
	}
	else
	{
		// invalid user name
		send(connfd, "530\r\n", 5, 0);
		return -1;
	}

	// check password
	recv(connfd, msg, len, 0);
	std::string passwd = std::string(strchr(msg, ' ') + 1);
	passwd = passwd.substr(0, passwd.size() - 2); // drop \r\n
	if(passwd == users[idx].passwd)
	{
		// user logged in, proceed
		sockfd2session[connfd] = FTPSession(&users[idx], users[idx].home_dir, connfd, -1);
		send(connfd, "230\r\n", 5, 0);
		return 0;
	}
	else
	{
		// authentication failed
		send(connfd, "530\r\n", 5, 0);
		return -1;
	}
}

void FTPServer::dispatch_cmd(int connfd)
{
	int& datafd = sockfd2session[connfd].data_sock;
	// infinite loop to get user's command
	while(1)
	{
		int len = 100;
		char msg[len];
		recv(connfd, msg, len, 0);
		std::string cmd(msg);
		cmd = cmd.substr(0, cmd.size() - 2);
		// query system type
		if(cmd == "SYST")
		{
			strcpy(msg, "215 CentOS\r\n");
			send(connfd, msg, strlen(msg), 0);
		}
		// quit
		else if(cmd == "QUIT")
		{
			strcpy(msg, "200 Goodbye\r\n");
			send(connfd, msg, strlen(msg), 0);
			close(connfd);
			if(datafd != -1)
				close(datafd);
			break;
		}
		// use active mode and prepare to transfer data
		else if(cmd.find("PORT") != std::string::npos)
		{

		}
		else if(cmd.find("LIST") != std::string::npos)
		{

		}
		// upload a file to the server
		else if(cmd.find("STOR") != std::string::npos)
		{

		}
		// retrieve a specific file from server
		else if(cmd.find("RETR") != std::string::npos)
		{

		}
	}
}


