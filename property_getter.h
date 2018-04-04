#pragma once

#include "tree/resource.h"

#include "typeidable.h"

namespace treecmd
{

class property_getter : public virtual typeidable
{
public:
	/*constructor*/ property_getter(){}
	virtual /*destructor*/ ~property_getter(){}

	virtual int get(property_base *, std::string &) = 0;
};

}
