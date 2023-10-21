#ifndef _SIMPLENODE_H_
#define _SIMPLENODE_H_

#include <string>
#include <vector>

struct SimpleNode
{
	std::string id;
	std::vector<SimpleNode> children;
	int x = 0;
	int y = 0;
};

#endif // _SIMPLENODE_H_