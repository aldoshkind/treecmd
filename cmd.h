#pragma once

#include <map>
#include <string>
#include <vector>

#include <tree/node.h>
#include <boost/function.hpp>

class cmd
{
	node *root;

	typedef std::vector<std::string> tokens_t;
	typedef boost::function<void(const tokens_t &)> command_t;

	typedef boost::function<int(property_base *, const std::string &)> value_setter_t;
	typedef boost::function<int(property_base *, std::string &)> value_getter_t;

	typedef std::map<std::string, command_t>		commands_t;
	commands_t										commands;

	typedef std::map<std::string, value_setter_t> setters_t;
	typedef std::map<std::string, value_getter_t> getters_t;

	setters_t setters;
	getters_t getters;

	std::string current_node_path = "/";

	void replace_if_at_end(std::string &str, char *pattern, char *replacement);
	void print_node(node *n, std::string prefix = "");

public:
	cmd(node *root);
	~cmd();

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
	void rm(const tokens_t &);
	void types(const tokens_t &);
	void help(const tokens_t &);

	cmd::tokens_t tokenize(const std::string &str);
	void exec(const tokens_t &cmd);

	int setter_double(property_base *p, const std::string &value);
	int getter_double(property_base *p, std::string &value);

	void set(node *n, std::string &prop, std::string value);
	void set(const std::string &target, const std::string &value);

	void print(const std::string &target);
	void eval(const tokens_t &tok);
};
