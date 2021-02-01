#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>
#include <thread>

#include <boost/function.hpp>

#include "tree/tree_node.h"

#include "type.h"

namespace treecmd
{

class interpreter
{
public:
	virtual ~interpreter(){}
	virtual tree_node *evaluate(std::string) = 0;
	virtual void set_root(tree_node *root) = 0;
};

class cmd
{
	tree_node *root;

	interpreter *inter = nullptr;

	typedef std::vector<std::string> tokens_t;
	typedef boost::function<void(const tokens_t &)> command_t;

	typedef std::map<std::string, command_t>		commands_t;
	commands_t										commands;

	typedef std::map<std::string, type *> types_t;

	types_t types;

	std::string current_node_path = "/";
	
	std::thread cmd_thread;

	void replace_if_at_end(std::string &str, const char *pattern, const char *replacement);
	void print_node(tree_node *n, tree_node *parent, std::string name, std::string prefix = "");

	void add_type(type *t);

	tree_node *generate(const std::string &type);
	void print_status(const std::string &status);

	std::string raw_input;
	
	typedef std::set<tree_node *>		visited_nodes_set_t;
	visited_nodes_set_t					visited_nodes_set;

public:
	/*constructor*/ cmd(tree_node *root);
	/*destructor*/ ~cmd();

	void init();
	void run();
	
	void run_in_thread();

	std::string absolute_path(std::string p);

	void exit_cmd(const tokens_t &);
	void pwd(const tokens_t &);
	void cd(const tokens_t &);
	void ls(const tokens_t &);
	void ln(const tokens_t &);
	void mv(const tokens_t &);
	void tree(const tokens_t &);
	void mknode(const tokens_t &);
	void rm(const tokens_t &);
	void get_types(const tokens_t &);
	void help(const tokens_t &);
	std::string render(tree_node *n, std::string &error);

	cmd::tokens_t tokenize(const std::string &str);
	void exec(const tokens_t &cmd);

	void set(tree_node *n, std::string value);
	void set(const std::string &target, const std::string &value);

	void print(const std::string &target);
	void print(tree_node *n);
	void eval(const tokens_t &tok);

	void set_interpreter(interpreter *in);
	
	tree_node::string_list_t ls_for(const std::string &s);
};

}
