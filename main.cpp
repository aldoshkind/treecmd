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

#include "tree_node_inherited.h"

#include "../treeipc/client.h"
#include "../treeipc/socket_client.h"
#include "../treeipc/socket_device.h"

int main(int argc, char **argv)
{
	std::string host = "127.0.0.1";
	int port = 13233;
	std::string port_str = std::to_string(port);
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

	tree_node root;


	socket_client sc;
	connector conn;
	conn.set_listener(&sc);
	conn.connect(host, port);
	socket_device sd(&sc);

	client cl;
	cl.set_device(&sd);
	sd.set_listener(&cl);

	boost::erase_all(host, ".");

//	root.attach(std::string("/") + host + ":" + port_str, cl.get_root(), false);

	root.attach("remote", cl.get_root(), false);

	tree_node a;
	root.attach("a", &a, false);
	auto tnid = new tree_node_inherited<property_value<double>>;
	root.attach("b", tnid);

	treecmd::cmd c(&root);
	c.run();

	return 0;
}
