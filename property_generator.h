#pragma once

#include "tree/resource.h"

#include "typeidable.h"

namespace treecmd
{

class property_generator : public virtual typeidable
{
public:
	/*constructor*/ property_generator(){}
	virtual /*destructor*/ ~property_generator(){}

	virtual property_base *generate(const std::string &name) = 0;
};

}
