#include "ftpserver.h"
#include <iostream>

int main()
{
	FTPServer server(4321, "blacklist.txt", "userlist.txt", ".");
	server.run();
}
