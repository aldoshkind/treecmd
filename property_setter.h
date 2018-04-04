#pragma once

#include "tree/resource.h"

#include "typeidable.h"

namespace treecmd
{

class property_setter : public virtual typeidable
{
public:
	/*constructor*/ property_setter(){}
	virtual /*destructor*/ ~property_setter(){}

	virtual int set(property_base *, const std::string &) = 0;
};

}
