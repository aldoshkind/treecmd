#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <condition_variable>

#include <readline/readline.h>
#include <readline/history.h>

#include <boost/algorithm/string.hpp>

#include "tree/node.h"
#include "treeipc/client.h"
#include "observable.h"
#include "reliable_serial.h"
#include "treeipc/socket_client.h"
#include "socket_device.h"

typedef std::vector<std::string> tokens_t;
typedef void (*command_t)(const tokens_t &);

typedef int (*value_setter_t)(property_base *p, const std::string &value);
typedef int (*value_getter_t)(property_base *p, std::string &value);

typedef std::map<std::string, command_t>		commands_t;
commands_t										commands;

typedef std::map<std::string, value_setter_t> setters_t;
typedef std::map<std::string, value_getter_t> getters_t;

setters_t setters;
getters_t getters;

std::string current_node_path = "/";

node root;

std::string absolute_path(std::string p)
{
	if(p.size() == 0)
	{
		return current_node_path;
	}

	if(p[0] == '.')
	{
		p = current_node_path + "/" + p;
	}

	clean_path(p);

	auto dot_pos = p.find_last_of('.');

	if(dot_pos != std::string::npos)
	{
		p = p.substr(0, dot_pos);
	}

	if(p[0] == '/')
	{
		return p;
	}

	p = current_node_path + "/" + p;
	clean_path(p);
	return p;
}

void pwd(const tokens_t &)
{
	printf("%s\n", current_node_path.c_str());
}

void cd(const tokens_t &t)
{
	if(t.size() < 2)
	{
		current_node_path = "/";
	}
	else
	{
		std::string path = absolute_path(t[1]);
		node *n = root.at(path);
		if(n != nullptr)
		{
			current_node_path = path;
		}
		else
		{
			printf("\"%s\" does not exist\n", path.c_str());
		}
	}
}

void ls(const tokens_t &t)
{
	std::string path;

	if(t.size() < 2)
	{
		path = current_node_path;
	}
	else
	{
		path = t[1];
	}
	node *n = root.at(path);
	if(n == nullptr)
	{
		printf("\"%s\" does not exist\n", path.c_str());
		return;
	}
	node::ls_list_t items = n->ls();
	printf("total: %d items\n", (int)items.size());
	for(auto item : items)
	{
		printf("%s\n", item.c_str());
	}
}

void lsprops(const tokens_t &t)
{
	std::string path;

	if(t.size() < 2)
	{
		path = current_node_path;
	}
	else
	{
		path = t[1];
	}
	node *n = root.at(path);
	if(n == nullptr)
	{
		printf("%s does not exist\n", path.c_str());
		return;
	}
	node::props_t props = n->get_properties();
	for(auto prop : props)
	{
		printf("%s %s\n", prop->get_type().c_str(), prop->get_name().c_str());
	}
}

int setter_double(property_base *p, const std::string &value)
{
	property<double> *pd = dynamic_cast<property<double> *>(p);

	if(pd == NULL)
	{
		return -1;
	}

	pd->set_value(strtod(value.c_str(), NULL));

	return 0;
}

int getter_double(property_base *p, std::string &value)
{
	property<double> *pd = dynamic_cast<property<double> *>(p);

	if(pd == NULL)
	{
		return -1;
	}

	std::stringstream render;
	render << pd->get_value();
	value = render.str();

	return 0;
}

void set(node *n, std::string &prop, std::string value)
{
	property_base *p = n->get_property(prop);
	if(p == NULL)
	{
		printf("property %s does not exist\n", prop.c_str());
		return;
	}

	setters_t::iterator it = setters.find(p->get_type());
	if(it == setters.end())
	{
		printf("dont know how to handle value of type %s\n", p->get_type().c_str());
		return;
	}
	int res = (it->second)(p, value);
	if(res != 0)
	{
		printf("error setting value\n");
		return;
	}

	//printf("set property %s has type %s\n", prop.c_str(), p->get_type().c_str());
}

void set(const std::string &target, const std::string &value)
{
	std::string::size_type dot_pos = target.find_last_of('.');
	if(dot_pos == std::string::npos)
	{
		printf("property not specified\n");
		return;
	}
	std::string path = target.substr(0, dot_pos);
	std::string prop_name = target.substr(dot_pos + 1);

	node *n = root.at(absolute_path(path));
	if(n == nullptr)
	{
		printf("%s does not exist\n", path.c_str());
		return;
	}
	set(n, prop_name, value);
}

void print(const std::string &target)
{
	std::string::size_type dot_pos = target.find_last_of('.');
	if(dot_pos == std::string::npos)
	{
		printf("property not specified\n");
		return;
	}
	std::string path = target.substr(0, dot_pos);
	std::string prop = target.substr(dot_pos + 1);

	node *n = root.at(absolute_path(path));
	if(n == nullptr)
	{
		printf("%s does not exist\n", path.c_str());
		return;
	}

	property_base *p = n->get_property(prop);
	if(p == NULL)
	{
		printf("property %s does not exist\n", prop.c_str());
		return;
	}

	getters_t::iterator it = getters.find(p->get_type());
	if(it == getters.end())
	{
		printf("dont know how to handle value of type %s\n", p->get_type().c_str());
		return;
	}
	std::string value;
	int res = (it->second)(p, value);
	if(res != 0)
	{
		printf("error getting value\n");
		return;
	}
	printf("%s = %s\n", target.c_str(), value.c_str());

	//printf("print property %s has type %s\n", prop.c_str(), p->get_type().c_str());
}

void eval(const tokens_t &tok)
{
	if(tok.size() == 0)
	{
		return;
	}

	std::string expr;
	for(tokens_t::size_type i = 0 ; i < tok.size() ; i += 1)
	{
		expr += tok[i];
	}

	std::string::size_type ass_sign_pos = expr.find_last_of("=");
	if(ass_sign_pos == std::string::npos)
	{
		print(tok[0]);
		return;
	}

	std::string path = expr.substr(0, ass_sign_pos);
	std::string value = expr.substr(ass_sign_pos + 1);

	set(path, value);
}

tokens_t tokenize(const std::string &str)
{
	tokens_t res;
	std::string::size_type pos = 0;
	for( ; pos < str.size() ; )
	{
		std::string::size_type start = str.find_first_not_of(" \t\n\r", pos);

		if(start == std::string::npos)
		{
			break;
		}

		std::string::size_type end = std::string::npos;
		if(str[start] == '"')
		{
			start += 1;
			end = str.find_first_of('"', start);
		}
		else
		{
			end = str.find_first_of(' ', start);
		}
		if(end == std::string::npos)
		{
			res.push_back(str.substr(start));
			pos = end;
		}
		else
		{
			res.push_back(str.substr(start, end - start));
			pos = end + 1;
		}
	}

	return res;
}

void exec(const tokens_t &cmd)
{
	if(cmd.size() < 1)
	{
		return;
	}

	commands_t::iterator it = commands.find(cmd[0]);
	if(it == commands.end())
	{
		eval(cmd);
	}
	else
	{
		(it->second)(cmd);
	}
}

int tab(int, int)
{
	return 0;
}

void exit_cmd(const tokens_t &)
{
	exit(0);
}

void help(const tokens_t &)
{
	printf("available commands:\n");
	for(commands_t::iterator it = commands.begin() ; it != commands.end() ; ++it)
	{
		printf("\t%s\n", it->first.c_str());
	}
}

void mknode(const tokens_t &t)
{
	std::string status;
	for(tokens_t::size_type i = 1 ; i < t.size() ; i += 1)
	{
		std::string path = absolute_path(t[i]);

		node *n = root.at(path);

		if(n != nullptr)
		{
			status = "exists";
		}
		else
		{
			n = root.generate(path);
			if(n != nullptr)
			{
				status = "created";
			}
			else
			{
				status = "creation failed";
			}
		}

		printf("%s %s\n", path.c_str(), status.c_str());
	}
}

void rm(const tokens_t &t)
{
	std::string status;
	for(tokens_t::size_type i = 1 ; i < t.size() ; i += 1)
	{
		std::string path = absolute_path(t[i]);

		node *n = root.at(path);

		if(n == nullptr)
		{
			status = "does not exist";
		}
		else
		{
			root.remove(path, true);
			status = "removed";
		}

		printf("%s %s\n", path.c_str(), status.c_str());
	}
}

void types(const tokens_t &/*t*/)
{
	printf("set:\n");
	for(auto setter : setters)
	{
		printf("\t%s\n", setter.first.c_str());
	}
	printf("get:\n");
	for(auto getter : getters)
	{
		printf("\t%s\n", getter.first.c_str());
	}
}

void replace_if_at_end(std::string &str, char *pattern, char *replacement)
{
	if(str.size() < strlen(pattern))
	{
		return;
	}

	if(strncmp(str.c_str() + str.size() - strlen(pattern), pattern, strlen(pattern)) == 0)
	{
		str.resize(str.size() - strlen(pattern));
		str += replacement;
	}
}

void print_node(node *n, std::string prefix = "")
{
	if(n == NULL)
	{
		return;
	}
	std::string name = n->get_name();
	name = name.size() ? name : "/";
	printf("%s%s\n", prefix.c_str(), name.c_str());
	replace_if_at_end(prefix, "\u251c", "\u2502");
	replace_if_at_end(prefix, "\u2514", " ");

	node::ls_list_t children = n->ls();

	for(node::ls_list_t::size_type i = 0 ; i < children.size() ; i += 1)
	{
		node *child = n->at(children[i]);

		if(child == NULL)
		{
			continue;
		}

		print_node(child, prefix + ((i == (children.size() - 1)) ? "\u2514" : "\u251c"));
	}
}

void tree(const tokens_t &t)
{
	std::string path = current_node_path;
	if(t.size() > 1)
	{
		path = absolute_path(t[1]);
	}
	node *n = root.at(path);
	if(n == NULL)
	{
		printf("node does not exist\n");
		return;
	}
	print_node(n);
}

void init()
{
	commands["exit"] = exit_cmd;
	commands["help"] = help;
	commands["pwd"] = pwd;
	commands["cd"] = cd;
	commands["mk"] = mknode;
	commands["ls"] = ls;
	commands["rm"] = rm;
	commands["lsp"] = lsprops;
	commands["types"] = types;
	commands["tree"] = tree;

	setters["d"] = setter_double;
	getters["d"] = getter_double;
}

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

	init();

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

	current_node_path = "/";

	char *input, shell_prompt[] = " > ";
	rl_bind_key('\t', tab);
	rl_bind_keyseq ("\\C-c", tab);		// doesnt work at the moment

	for( ; ; )
	{
		input = readline((current_node_path + shell_prompt).c_str());

		if(input == NULL)
		{
			break;
		}

		tokens_t ts = tokenize(input);
		exec(ts);

		if(strlen(input) != 0)
		{
			add_history(input);
		}
		free(input);
	}
}
