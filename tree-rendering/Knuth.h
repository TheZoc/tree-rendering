#ifndef _KNUTH_H_
#define _KNUTH_H_

#include "SimpleNode.h"

/**
 * Position the nodes according to D.E.Knuth algorithm
 */
int SetupKnuth(SimpleNode& node, int depth = 0, int i = 0)
{
	// For simplicity's sake, children[0] is left node, children[1] is right node.
	// If there are more children, they're ignored.

	const size_t len = node.children.size();

	node.y = depth;

	// Has left child?
	if (len >= 1)
	{
		i = SetupKnuth(node.children.at(0), depth + 1, i);
	}

	node.x = i++;

	if (len >= 2)
	{
		i = SetupKnuth(node.children.at(1), depth + 1, i);
	}

	return i;
}

#endif // _KNUTH_H_