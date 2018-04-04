#pragma once

#include <map>
#include <string>
#include <vector>

#include <tree/node.h>
#include <boost/function.hpp>

#include "property_getter.h"
#include "property_setter.h"
#include "property_generator.h"
#include "numeric_property_gsg.h"

namespace treecmd
{

class cmd
{
	node *root;

	typedef std::vector<std::string> tokens_t;
	typedef boost::function<void(const tokens_t &)> command_t;

	typedef std::map<std::string, command_t>		commands_t;
	commands_t										commands;

	typedef std::map<std::string, property_setter *> setters_t;
	typedef std::map<std::string, property_getter *> getters_t;
	typedef std::map<std::string, property_generator *> generators_t;

	setters_t setters;
	getters_t getters;
	generators_t generators;

	std::string current_node_path = "/";

	void replace_if_at_end(std::string &str, char *pattern, char *replacement);
	void print_node(node *n, std::string prefix = "");

	void add_gsg(property_gsg *gsg);

public:
	/*constructor*/ cmd(node *root);
	/*destructor*/ ~cmd();

	void init();
	void run();

	std::string absolute_path(std::string p);

	void exit_cmd(const tokens_t &);
	void pwd(const tokens_t &);
	void cd(const tokens_t &);
	void ls(const tokens_t &);
	void lsprops(const tokens_t &);
	void tree(const tokens_t &);
	void mknode(const tokens_t &);
	void mkprop(const tokens_t &);
	void rm(const tokens_t &);
	void types(const tokens_t &);
	void help(const tokens_t &);

	cmd::tokens_t tokenize(const std::string &str);
	void exec(const tokens_t &cmd);

	void set(node *n, std::string &prop, std::string value);
	void set(const std::string &target, const std::string &value);

	void print(const std::string &target);
	void eval(const tokens_t &tok);
};

}
