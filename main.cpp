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

#include "tree_node_inherited.h".h"

int main()
{
	tree_node root;

	tree_node a;
	root.attach("a", &a, false);
	auto tnid = new tree_node_inherited<property_value<double>>;
	root.attach("b", tnid);

	treecmd::cmd c(&root);
	c.run();

	return 0;
}
