#ifndef _BUCHHEIM_H_
#define _BUCHHEIM_H_

/**
 * ============================================================================
 * Buchheim tree layout implementation
 * Source URL: https://github.com/TheZoc/tree-rendering
 * ============================================================================
 *
 * Implementation of the Buchheim, et al. tree layout algorithm with O(n)
 * complexity. It will transverse the tree two times to calculate the position
 * of the nodes - and having a possible third transversal if at the end there
 * are any nodes in a negative x coordinate space.
 *
 * The coordinates system consider that all nodes have the size 1 (i.e. a pixel)
 * and position output coordinates compatible with this. When using this for
 * images or screen drawing, it is necessary to scale the output accordingly.
 *
 * References:
 *
 * C. Buchheim, M. Jünger, and S. Leipert. Improving Walker’s algorithm to run
 * in linear time. Technical Report zaik2002-431, ZAIK, Universität zu
 * Köln, 2002.
 *
 * C. Wetherell and A. Shannon. Tidy drawings of trees. IEEE Transactions on
 * Software Engineering, 5(5):514–520, 1979.
 *
 * E. Reingold and J. Tilford. Tidier drawings of trees. IEEE Transactions on
 * Software Engineering, 7(2):223–228, 1981.
 *
 * J. Walker II. A node-positioning algorithm for general trees.
 * Software – Practice and Experience, 20(7):685–705, 1990.
 */

#include "SimpleNode.h"

#include <string>
#include <vector>

struct BuchheimNode
{
	std::string id;
	std::vector<BuchheimNode> children;
	float x = 0.f;
	float y = 0.f;

	BuchheimNode(const SimpleNode& node)
		: id(node.id)
		, x(static_cast<float>(node.x))
		, y(static_cast<float>(node.y))
	{
		for (auto& child : node.children)
		{
			children.push_back(BuchheimNode(child));
		}
	}

	// Auxiliary stuff
	BuchheimNode* parent           = nullptr;
	BuchheimNode* thread           = nullptr;
	BuchheimNode* leftmost_sibling = nullptr;
	BuchheimNode* ancestor         = nullptr;
	float mod                      = 0.f;
	float change                   = 0.f;
	float shift                    = 0.f;
	size_t sibling_order_number    = 0;

	void InitSetup()
	{
		x                = 0.f;
		y                = 0.f;
		ancestor         = this;
		thread           = nullptr;
		leftmost_sibling = nullptr;
		mod              = 0.f;
		change           = 0.f;

		for (size_t i = 0; i < children.size(); ++i)
		{
			children[i].parent               = this;
			children[i].sibling_order_number = i;
			children[i].InitSetup();
		}
	}

	BuchheimNode* Left() { return thread ? thread : !children.empty() ? &children.at(0)
																	  : nullptr; }
	BuchheimNode* Right() { return thread ? thread : !children.empty() ? &children.at(children.size() - 1)
																	   : nullptr; }
	BuchheimNode* LeftSibling()
	{
		if (parent)
		{
			BuchheimNode* node = nullptr;
			for (auto& child : parent->children)
			{
				if (&child == this)
				{
					return node;
				}
				node = &child;
			}
		}
		return nullptr;
	}

	BuchheimNode* LeftmostSibling()
	{
		if (!leftmost_sibling && parent && this != &(parent->children.at(0)))
		{
			leftmost_sibling = &(parent->children.at(0));
		}
		return leftmost_sibling;
	}
};

void Buchheim(BuchheimNode& tree);

void FirstWalk(BuchheimNode& v, float distance = 1.f);

BuchheimNode& Apportion(BuchheimNode& v, BuchheimNode& default_ancestor, float distance);

void MoveSubtree(BuchheimNode& wl, BuchheimNode& wr, float shift);

void ExecuteShifts(BuchheimNode& v);

BuchheimNode& Ancestor(BuchheimNode& vil, BuchheimNode& v, BuchheimNode& default_ancestor);

float SecondWalk(BuchheimNode& v, float m = 0, int32_t depth = 0, float min = std::numeric_limits<float>::infinity());

void ThirdWalk(BuchheimNode& tree, float n);

#endif // _BUCHHEIM_H_
