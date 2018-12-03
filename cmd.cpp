#include "cmd.h"

#include <boost/bind.hpp>

#include <readline/readline.h>
#include <readline/history.h>

using namespace treecmd;

int tab(int, int);

class tree_node;

cmd::cmd(tree_node *root)
{
	this->root = root;

	init();
}

cmd::~cmd()
{
	//
}


void cmd::init()
{
	commands["exit"] = boost::bind(&cmd::exit_cmd, this, _1);
	commands["help"] = boost::bind(&cmd::help, this, _1);
	commands["pwd"] = boost::bind(&cmd::pwd, this, _1);
	commands["cd"] = boost::bind(&cmd::cd, this, _1);
	commands["mk"] = boost::bind(&cmd::mknode, this, _1);
	commands["ls"] = boost::bind(&cmd::ls, this, _1);
	commands["rm"] = boost::bind(&cmd::rm, this, _1);
	commands["types"] = boost::bind(&cmd::get_types, this, _1);
	commands["tree"] = boost::bind(&cmd::tree, this, _1);

	add_type(new type_numeric<double>());
	add_type(new type_numeric<float>());
	add_type(new type_numeric<int>());
	add_type(new type_numeric<long>());
	add_type(new type_qstring());
}

void cmd::add_type(type *t)
{
	auto tp = t->get_typeid();

	types[tp] = t;
}

void cmd::run()
{
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

std::string cmd::absolute_path(std::string p)
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

void cmd::pwd(const tokens_t &)
{
	printf("%s\n", current_node_path.c_str());
}

void cmd::cd(const tokens_t &t)
{
	if(t.size() < 2)
	{
		current_node_path = "/";
	}
	else
	{
		std::string path = absolute_path(t[1]);
		tree_node *n = root->at(path);
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

std::string cmd::render(tree_node *n, std::string &error)
{
	if(n == nullptr)
	{
		return "";
	}

	property_base *p = dynamic_cast<property_base *>(n);
	if(p == nullptr)
	{
		//printf("Property %s does not exist\n", prop.c_str());
		error = "Not a property";
		return "";
	}

	types_t::iterator it = types.find(p->get_type());
	if(it == types.end())
	{
		//printf("Dont know how to handle value of type '%s'\n", p->get_type().c_str());
		error = "Dont know how to handle value";
		return "";
	}
	std::string value;
	int res = it->second->get(p, value);
	if(res != 0)
	{
		//printf("Error getting value\n");
		error = "Error getting value";
		return "";
	}

	return value;
}

void cmd::ls(const tokens_t &t)
{
	std::string path;
	bool show_values = false;

	if(t.size() < 2)
	{
		path = current_node_path;
	}
	else
	{
		for(int i = 1 ; i < t.size() ; i += 1)
		{
			if(t[i].size() > 1 && t[i][0] == '-')
			{
				if(t[i][1] == 'v')
				{
					show_values = true;
				}
			}
			else
			{
				path = t[i];
				break;
			}
		}
	}
	path = absolute_path(path);
	tree_node *n = root->at(path);
	if(n == nullptr)
	{
		printf("\"%s\" does not exist\n", path.c_str());
		return;
	}
	tree_node::ls_list_t items = n->ls();
	printf("total: %d items\n", (int)items.size());
	for(auto item : items)
	{
		tree_node *nd = n->at(item);
		printf("%s\t\t%s", item.c_str(), nd ? nd->get_type().c_str() : "");

		if(show_values)
		{
			std::string err;
			std::string value = render(nd, err);
			if(err == "")
			{
				printf("\t\t'%s'", value.c_str());
			}
		}
		printf("\n");
	}
}

void cmd::set(tree_node *n, std::string value)
{
	property_base *p = dynamic_cast<property_base *>(n);
	if(p == NULL)
	{
		printf("Cant set node %s\n", n->get_name().c_str());
		return;
	}

	types_t::iterator it = types.find(p->get_type());
	if(it == types.end())
	{
		printf("Dont know how to handle value of type %s\n", p->get_type().c_str());
		return;
	}
	int res = it->second->set(p, value);
	if(res != 0)
	{
		printf("Error setting value\n");
		return;
	}

	//printf("set property %s has type %s\n", prop.c_str(), p->get_type().c_str());
}

void cmd::set(const std::string &target, const std::string &value)
{
	tree_node *n = root->at(absolute_path(target));
	if(n == nullptr)
	{
		printf("%s does not exist\n", target.c_str());
		return;
	}
	set(n, value);
}

void cmd::print(const std::string &target)
{
	tree_node *n = root->at(absolute_path(target));
	if(n == nullptr)
	{
		printf("%s does not exist\n", target.c_str());
		return;
	}

	std::string err;
	auto value = render(n, err);
	if(err != "")
	{
		printf("%s\n", err.c_str());
	}
	else
	{
		printf("%s\n", value.c_str());
	}
}

void cmd::eval(const tokens_t &tok)
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

cmd::tokens_t cmd::tokenize(const std::string &str)
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

void cmd::exec(const tokens_t &cmd)
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

void cmd::exit_cmd(const tokens_t &)
{
	exit(0);
}

void cmd::help(const tokens_t &)
{
	printf("available commands:\n");
	for(commands_t::iterator it = commands.begin() ; it != commands.end() ; ++it)
	{
		printf("\t%s\n", it->first.c_str());
	}
}

void cmd::mknode(const tokens_t &t)
{
	if(t.size() < 2)
	{
		printf("usage: %s (type) name\n(type) is optional. type types to get types\n", t[0].c_str());
		return;
	}

	std::string type = "";

	size_t lbrace_pos = t[1].find_last_of('(');
	size_t rbrace_pos = t[1].find_first_of(')');
	bool t1_is_type = false;
	if(lbrace_pos != std::string::npos && rbrace_pos != std::string::npos && lbrace_pos < rbrace_pos)
	{
		type = t[1].substr(lbrace_pos + 1, rbrace_pos - lbrace_pos - 1);
		t1_is_type = true;
	}


	std::string status;
	for(tokens_t::size_type i = t1_is_type ? 2 : 1 ; i < t.size() ; i += 1)
	{
		std::string path = absolute_path(t[i]);

		size_t last_slash = path.find_last_of('/');
		std::string catalog = path.substr(0, last_slash == std::string::npos ? 0 : last_slash);
		std::string node_name = path.substr(last_slash == std::string::npos ? 0 : last_slash + 1);

		tree_node *n = root->at(catalog);

		if(n == nullptr)
		{
			status = "Error resolving path " + catalog;
		}
		else
		{
			tree_node *new_node = nullptr;
			if(type == "")
			{
				new_node = new tree_node();
			}
			else
			{
				new_node = generate(type);
			}

			if(new_node != nullptr)
			{
				status = "created";
			}
			else
			{
				status = "creation failed";
			}

			if(n->at(node_name))
			{
				status = "existst";
			}

			n->attach(node_name, new_node);
		}

		printf("%s %s\n", path.c_str(), status.c_str());
	}
}

void cmd::rm(const tokens_t &t)
{
	std::string status;
	for(tokens_t::size_type i = 1 ; i < t.size() ; i += 1)
	{
		std::string path = absolute_path(t[i]);

		tree_node *n = root->at(path);

		if(n == nullptr)
		{
			status = "does not exist";
		}
		else
		{
			root->remove(path, true);
			status = "removed";
		}

		printf("%s %s\n", path.c_str(), status.c_str());
	}
}

void cmd::get_types(const tokens_t &/*t*/)
{
	printf("Available types are:\n");
	for(auto type : types)
	{
		printf("\t%s\t%s\n", type.first.c_str(), type.second->get_typename().c_str());
	}
}

void cmd::replace_if_at_end(std::string &str, char *pattern, char *replacement)
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

void cmd::print_node(tree_node *n, std::string prefix)
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

	tree_node::ls_list_t children = n->ls();

	for(tree_node::ls_list_t::size_type i = 0 ; i < children.size() ; i += 1)
	{
		tree_node *child = n->at(children[i]);

		if(child == NULL)
		{
			continue;
		}

		print_node(child, prefix + ((i == (children.size() - 1)) ? "\u2514" : "\u251c"));
	}
}

void cmd::tree(const tokens_t &t)
{
	std::string path = current_node_path;
	if(t.size() > 1)
	{
		path = absolute_path(t[1]);
	}
	tree_node *n = root->at(path);
	if(n == NULL)
	{
		printf("node does not exist\n");
		return;
	}
	print_node(n);
}

int tab(int, int)
{
	return 0;
}

tree_node *cmd::generate(const std::string &type)
{
	auto t = types.find(type);
	if(t == types.end())
	{
		return nullptr;
	}

	return t->second->generate();;
}
