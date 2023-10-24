#ifndef _WETHERELL_SHANNON_H_
#define _WETHERELL_SHANNON_H_

#include "SimpleNode.h"
#include <algorithm>
#include <memory>

// Internal functions forward declarations
void SetupWSMinimum_Internal(SimpleNode& node, std::vector<int>& nexts, int depth);
void SetupWS2_Internal(WS2Node& node, std::vector<float>& nexts, std::vector<float>& offset, int depth);
void AddModsWS2_Internal(WS2Node& node, float modsum);

/**
 * Position the nodes according to Wetherell-Shannon minimum algorithm tree
 */
void SetupWSMinimum(SimpleNode& node)
{
	std::vector<int> nexts;
	SetupWSMinimum_Internal(node, nexts, 0);
}

/**
 * Position the nodes according to Wetherell-Shannon 2nd algorithm
 */
void SetupWS2(WS2Node& node)
{
	std::vector<float> nexts;
	std::vector<float> offset;

	SetupWS2_Internal(node, nexts, offset, 0);
	AddModsWS2_Internal(node, 0);
}

//////////////////////////////////////////////////////////////////////////
// Internal functions

/**
 * Internal use for SetupWSMinimum()
 */
void SetupWSMinimum_Internal(SimpleNode& node, std::vector<int>& nexts, int depth)
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

void SetupWS2_Internal(WS2Node& node, std::vector<float>& nexts, std::vector<float>& offset, int depth)
{
	// Make sure our vector contains the index for the current depth
	if (nexts.size() < depth + 1)
	{
		nexts.push_back(0);
		offset.push_back(0);
	}

	// Depth-first transversal recursive call
	for (auto& child : node.children)
	{
		SetupWS2_Internal(child, nexts, offset, depth + 1);
	}

	// Calculate and set current node position
	node.y = static_cast<float>(depth);

	float place = 0.f;
	if (node.children.empty())
	{
		node.x = place = nexts[depth];
	}
	else if (node.children.size() == 1)
	{
		place = static_cast<float>(node.children[0].x - 1);
	}
	else
	{
		place = (node.children[0].x + node.children[1].x) / 2.f;
	}

	offset[depth] = std::max(offset[depth], nexts[depth] - place);

	if (!node.children.empty())
	{
		node.x = place + offset[depth];
	}

	nexts[depth] += 2;
	node.offset = offset[depth];
}

void AddModsWS2_Internal(WS2Node& node, float modsum)
{
	node.x += modsum;
	modsum += node.offset;

	for (auto& child : node.children)
	{
		AddModsWS2_Internal(child, modsum);
	}
}

#endif // _WETHERELL_SHANNON_H_
