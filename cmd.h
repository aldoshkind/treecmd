#pragma once

#include <map>
#include <string>
#include <vector>

#include "tree_node.h"
#include <boost/function.hpp>

#include "type.h"

namespace treecmd
{

class cmd
{
	tree_node_t *root;

	typedef std::vector<std::string> tokens_t;
	typedef boost::function<void(const tokens_t &)> command_t;

	typedef std::map<std::string, command_t>		commands_t;
	commands_t										commands;

	typedef std::map<std::string, type *> types_t;

	types_t types;

	std::string current_node_path = "/";

	void replace_if_at_end(std::string &str, char *pattern, char *replacement);
	void print_node(tree_node_t *n, std::string prefix = "");

	void add_type(type *t);

public:
	/*constructor*/ cmd(tree_node_t *root);
	/*destructor*/ ~cmd();

	void init();
	void run();

	std::string absolute_path(std::string p);

	void exit_cmd(const tokens_t &);
	void pwd(const tokens_t &);
	void cd(const tokens_t &);
	void ls(const tokens_t &);
	void tree(const tokens_t &);
	//void mknode(const tokens_t &);
	//void mkprop(const tokens_t &);
	void rm(const tokens_t &);
	void get_types(const tokens_t &);
	void help(const tokens_t &);
	std::string render(tree_node_t *n, std::string &error);

	cmd::tokens_t tokenize(const std::string &str);
	void exec(const tokens_t &cmd);

	void set(tree_node_t *n, std::string value);
	void set(const std::string &target, const std::string &value);

	void print(const std::string &target);
	void eval(const tokens_t &tok);
};

}
