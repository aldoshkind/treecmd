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
#include "../treeipc/client_nonroot.h"
#include "../treeipc/socket_client.h"
#include "../treeipc/package_stream.h"

int main(int argc, char **argv)
{
	tree_node root;

	/*std::string host = "127.0.0.1";
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

	socket_client sc;
	connector conn(port, host);
	conn.set_listener(&sc);
	conn.set_do_reconnect(true);
	sc.set_connector(&conn);
	//sleep(1);
	treeipc::request_reply_dispatcher sd(&sc);

	treeipc::client_nonroot cl;
	cl.set_device(&sd);
	sd.set_listener(&cl);
	
	cl.set_root(&root);
	conn.connect();*/

	treecmd::cmd c(&root);
	c.run();

	return 0;
}
