#pragma once

#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <typeinfo>

#include <cxxabi.h>

#include <tree/node.h>
#include <boost/function.hpp>

namespace treecmd
{

class typeidable
{
public:
	/*constructor*/ typeidable(){}
	virtual /*destructor*/ ~typeidable(){}

	virtual const std::string get_typeid() const = 0;
	virtual const std::string get_typename() const = 0;
};

class property_setter : public virtual typeidable
{
public:
	/*constructor*/ property_setter(){}
	virtual /*destructor*/ ~property_setter(){}

	virtual int set(property_base *, const std::string &) = 0;
};

class property_getter : public virtual typeidable
{
public:
	/*constructor*/ property_getter(){}
	virtual /*destructor*/ ~property_getter(){}

	virtual int get(property_base *, std::string &) = 0;
};

class property_generator : public virtual typeidable
{
public:
	/*constructor*/ property_generator(){}
	virtual /*destructor*/ ~property_generator(){}

	virtual property_base *generate(const std::string &name) = 0;
};

// gsg значит getter-setter-generator
class property_gsg : public property_setter, public property_getter, public property_generator
{
	//
};

template <typename type>
class numeric_property_gsg : public property_gsg
{
public:
	/*constructor*/ numeric_property_gsg(){}
	/*destructor*/ ~numeric_property_gsg(){}

	int set(property_base *prop, const std::string & value)
	{
		property<type> *pd = dynamic_cast<property<type> *>(prop);

		if(pd == NULL)
		{
			return -1;
		}

		double val = strtod(value.c_str(), NULL);

		pd->set_value(val);

		return 0;
	}

	int get(property_base *prop, std::string &value)
	{
		property<type> *pd = dynamic_cast<property<type> *>(prop);

		if(pd == NULL)
		{
			return -1;
		}

		std::stringstream render;
		render << pd->get_value();
		value = render.str();

		return 0;
	}

	property_base *generate(const std::string &name)
	{
		property_base *pd = new property_value<type>(name);
		return pd;
	}

	const std::string get_typeid() const
	{
		return typeid(type).name();
	}

	const std::string get_typename() const
	{
		int status = 0;
		char *n = abi::__cxa_demangle(get_typeid().c_str(), 0, 0, &status);
		std::string name(n);
		free(n);
		return name;
	}
};

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
