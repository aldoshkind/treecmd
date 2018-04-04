#include <cmd.h>

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <condition_variable>
#include <functional>

#include <boost/algorithm/string.hpp>

#include "tree/node.h"
#include "treeipc/client.h"
#include "observable.h"
#include "reliable_serial.h"
#include "treeipc/socket_client.h"
#include "socket_device.h"

node root;

int main(int argc, char **argv)
{
	std::string host = "127.0.0.1";
	int port = 12326;
	std::string port_str = "12326";
	if(argc > 1)
	{
		host = std::string(argv[1]);
	}
	if(argc > 2)
	{
		port_str = std::string(argv[2]);
	}

	int rd_port = strtol(port_str.c_str(), NULL, 10);
	if(rd_port > 0 && rd_port < 65535)
	{
		port = rd_port;
	}

	socket_client sc;
	connector conn;
	conn.set_listener(&sc);
	conn.connect(host, port);
	socket_device sd(&sc);

	client cl;
	cl.set_device(&sd);
	sd.set_listener(&cl);

	boost::erase_all(host, ".");

	root.attach(std::string("/") + host + ":" + port_str, cl.get_root(), false);

	treecmd::cmd c(&root);
	c.run();

	return 0;
}
