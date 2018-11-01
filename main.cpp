#include <cmd.h>

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <condition_variable>
#include <functional>

#include <boost/algorithm/string.hpp>

#include "tree_node.h"

#include <cxxabi.h>

std::string demangle(const char *name)
{
	int status = 0;
	return abi::__cxa_demangle(name, 0, 0, &status);
}

template <class T>
class tree_node_inherited : public tree_node_t, public T
{
public:
	tree_node_inherited() : tree_node_t(), T()
	{
		tree_node_t::set_type(demangle(typeid(T).name()));
	}
};

int main()
{
	tree_node_t root;

	tree_node_t a;
	root.attach("a", &a, false);
	auto tnid = new tree_node_inherited<property_value<double>>;
	root.attach("b", tnid);

	treecmd::cmd c(&root);
	c.run();

	return 0;
}
