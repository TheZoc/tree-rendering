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

struct WS2Node
{
	std::string id;
	std::vector<WS2Node> children;
	float x      = 0.f;
	float y      = 0.f;
	float offset = 0.f;

	WS2Node() {}
	WS2Node(const SimpleNode& node)
		: id(node.id)
		, x(static_cast<float>(node.x))
		, y(static_cast<float>(node.y))
	{
		for (auto& child : node.children)
		{
			children.push_back(WS2Node(child));
		}
	}
};

#endif // _SIMPLENODE_H_
