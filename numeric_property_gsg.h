#pragma once

#include <string>
#include <sstream>
#include <typeinfo>

#include <cxxabi.h>

#include "tree/resource.h"

#include "property_getter.h"
#include "property_setter.h"
#include "property_generator.h"

namespace treecmd
{

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

	int set(property_base *prop, const std::string & value);
	int get(property_base *prop, std::string &value);
	property_base *generate(const std::string &name);

	const std::string get_typeid() const;
	const std::string get_typename() const;
};

template <typename type>
int numeric_property_gsg<type>::set(property_base *prop, const std::string & value)
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

template <typename type>
int numeric_property_gsg<type>::get(property_base *prop, std::string &value)
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

template <typename type>
property_base *numeric_property_gsg<type>::generate(const std::string &name)
{
	property_base *pd = new property_value<type>(name);
	return pd;
}

template <typename type>
const std::string numeric_property_gsg<type>::get_typeid() const
{
	return typeid(type).name();
}

template <typename type>
const std::string numeric_property_gsg<type>::get_typename() const
{
	int status = 0;
	char *n = abi::__cxa_demangle(get_typeid().c_str(), 0, 0, &status);
	std::string name(n);
	free(n);
	return name;
}

}
