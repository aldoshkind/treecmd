#pragma once

#include <string>
#include <sstream>
#include <typeinfo>

#include <cxxabi.h>

#include <QString>

#include <tree/property.h>
#include "tree/tree_node_inherited.h"
#include "typeidable.h"

namespace treecmd
{

class type : public typeidable
{
public:
	/*constructor*/ type() {}
	virtual /*destructor*/ ~type(){}

	virtual int set(property_base *, const std::string &) = 0;
	virtual int get(property_base *, std::string &) = 0;
	virtual tree_node *generate() = 0;
};


class type_bool : public type
{
public:
	/*constructor*/ type_bool(){}
	/*destructor*/ ~type_bool(){}

	int set(property_base *prop, const std::string &value)
	{
		property<bool> *pd = dynamic_cast<property<bool> *>(prop);

		if(pd == NULL)
		{
			return -1;
		}
		
		bool ok = false;
		int int_val = QString::fromStdString(value).toInt(&ok, 10);
		
		bool val = value == "true" ? true : value == "false" ? false : (ok == true && int_val != 0) ? true : false;

		pd->set_value(val);

		return 0;
	}

	int get(property_base *prop, std::string &value)
	{
		property<bool> *pd = dynamic_cast<property<bool> *>(prop);

		if(pd == NULL)
		{
			return -1;
		}

		value = pd->get_value() ? "true" : "false";

		return 0;
	}

	tree_node *generate()
	{
		auto *pd = new tree_node_inherited<property_value<bool>>();
		return pd;
	}

	const std::string get_typeid() const
	{
		return typeid(bool).name();
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


class type_qstring : public type
{
public:
	/*constructor*/ type_qstring(){}
	/*destructor*/ ~type_qstring(){}

	int set(property_base *prop, const std::string &value)
	{
		property<QString> *pd = dynamic_cast<property<QString> *>(prop);

		if(pd == NULL)
		{
			return -1;
		}

		pd->set_value(QString::fromStdString(value));

		return 0;
	}

	int get(property_base *prop, std::string &value)
	{
		property<QString> *pd = dynamic_cast<property<QString> *>(prop);

		if(pd == NULL)
		{
			return -1;
		}

		value = pd->get_value().toStdString();

		return 0;
	}

	tree_node *generate()
	{
		auto *pd = new tree_node_inherited<property_value<QString>>();
		return pd;
	}

	const std::string get_typeid() const
	{
		return typeid(QString).name();
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

template <typename T>
class type_numeric : public type
{
public:
	/*constructor*/ type_numeric(){}
	/*destructor*/ ~type_numeric(){}

	int set(property_base *prop, const std::string & value);
	int get(property_base *prop, std::string &value);
	tree_node *generate();

	const std::string get_typeid() const;
	const std::string get_typename() const;
};

template <typename T>
int type_numeric<T>::set(property_base *prop, const std::string & value)
{
	property<T> *pd = dynamic_cast<property<T> *>(prop);

	if(pd == NULL)
	{
		return -1;
	}

	double val = strtod(value.c_str(), NULL);

	pd->set_value(val);

	return 0;
}

template <typename T>
int type_numeric<T>::get(property_base *prop, std::string &value)
{
	property<T> *pd = dynamic_cast<property<T> *>(prop);

	if(pd == NULL)
	{
		return -1;
	}

	std::stringstream render;
	render << pd->get_value();
	value = render.str();

	return 0;
}

template <typename T>
tree_node *type_numeric<T>::generate()
{
	auto *pd = new tree_node_inherited<property_value<T>>();
	return pd;
}

template <typename T>
const std::string type_numeric<T>::get_typeid() const
{
	return typeid(T).name();
}

template <typename T>
const std::string type_numeric<T>::get_typename() const
{
	int status = 0;
	char *n = abi::__cxa_demangle(get_typeid().c_str(), 0, 0, &status);
	std::string name(n);
	free(n);
	return name;
}

}
