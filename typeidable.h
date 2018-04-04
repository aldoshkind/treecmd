#pragma once

#include <string>

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

}
