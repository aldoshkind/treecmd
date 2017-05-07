#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <string>
#include <vector>
#include <map>

#include <readline/readline.h>
#include <readline/history.h>

#include "tree/node.h"

typedef std::vector<std::string> tokens_t;
typedef void (*command_t)(const tokens_t &);

typedef std::map<std::string, command_t>		commands_t;
commands_t										commands;

std::string current_node_path = "/";

node root;

std::string absolute_path(std::string p)
{
	if(p.size() == 0)
	{
		return p;
	}

	if(p[0] == '.')
	{
		p = current_node_path + "/" + p;
	}

	clean_path(p);

	auto dot_pos = p.find_first_of('.');

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

void set(const std::string &path, const std::string &value)
{
	printf("%s : %s\n", path.c_str(), value.c_str());
}

void eval(const tokens_t &tok)
{
	std::string expr;
	for(tokens_t::size_type i = 0 ; i < tok.size() ; i += 1)
	{
		expr += tok[i];
	}

	std::string::size_type ass_sign_pos = expr.find_last_of("=");
	std::string path = expr.substr(0, ass_sign_pos - 1);
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
			n = root.append(path);
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

void init_commands()
{
	commands["exit"] = exit_cmd;
	commands["help"] = help;
	commands["pwd"] = pwd;
	commands["cd"] = cd;
	commands["mk"] = mknode;
	commands["ls"] = ls;
	commands["rm"] = rm;
	commands["lsprops"] = lsprops;
}

int main()
{
	init_commands();

	node *tmp = root.append("tmp");
	tmp->add_property(new property_value<double>("roll"));
	tmp->add_property(new property_value<double>("pitch"));
	root.append("a/b/c/d/e/f");
	current_node_path = "/a/b/c/d/e/f";

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

		add_history(input);
		free(input);
	}
}
