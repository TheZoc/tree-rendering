#ifndef _WETHERELL_SHANNON_H_
#define _WETHERELL_SHANNON_H_

#include "SimpleNode.h"
#include <stdexcept>
#include <memory>

/**
 * Internal use for SetupWSMinimum()
 */
void SetupWSMinimum_Internal(SimpleNode& node, std::vector<int>& nexts, int depth = 0)
{
	node.y = depth;
	if (nexts.size() > depth)
	{
		node.x = nexts[depth]++;
	}
	else
	{
		node.x = 0;
		nexts.push_back(1);
	}

	for (auto& child : node.children)
	{
		SetupWSMinimum_Internal(child, nexts, depth + 1);
	}
}

/**
 * Position the nodes according to Wetherell-Shannon
 */
void SetupWSMinimum(SimpleNode& node)
{
	std::vector<int> nexts;
	SetupWSMinimum_Internal(node, nexts, 0);
}

#endif // _WETHERELL_SHANNON_H_