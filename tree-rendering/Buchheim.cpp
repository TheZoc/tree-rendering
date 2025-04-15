#include "Buchheim.h"

#include <iostream>

#define SHOW_DEBUG_MESSAGES 0

void Buchheim(BuchheimNode& tree)
{
	tree.InitSetup();
	FirstWalk(tree);
	float min = SecondWalk(tree);
	if (min < 0)
	{
		ThirdWalk(tree, -min);
	}
}

void FirstWalk(BuchheimNode& v, float distance /*= 1.f*/)
{
	if (v.children.empty())
	{
		v.x = (v.LeftmostSibling()) ? v.LeftSibling()->x + distance : 0;
	}
	else
	{
		BuchheimNode* default_ancestor = &(v.children.at(0));
		for (auto& w : v.children)
		{
			FirstWalk(w);
			default_ancestor = &Apportion(w, *default_ancestor, distance);
		}
#if SHOW_DEBUG_MESSAGES
		std::cerr << "finished v = " << v.id << " children" << std::endl;
#endif
		ExecuteShifts(v);

		float midpoint = (v.children.at(0).x + v.children.at(v.children.size() - 1).x) / 2.f;
		if (BuchheimNode* w = v.LeftSibling())
		{
			v.x   = w->x + distance;
			v.mod = v.x - midpoint;
		}
		else
		{
			v.x = midpoint;
		}
	}
}

BuchheimNode& Apportion(BuchheimNode& v, BuchheimNode& default_ancestor, float distance)
{
	if (BuchheimNode* w = v.LeftSibling())
	{
		// Odd variable names? Check the original paper :)
		// i -> inner
		// o -> outer
		// r -> right -> +
		// l -> left -> -
		// s -> countour summation
		BuchheimNode* vir = &v;
		BuchheimNode* vor = &v;
		BuchheimNode* vil = w;
		BuchheimNode* vol = v.LeftmostSibling();
		float sir         = v.mod;
		float sor         = v.mod;
		float sil         = vil->mod;
		float sol         = vol->mod;

		while (vil->Right() && vir->Left())
		{
			vil           = vil->Right();
			vir           = vir->Left();
			vol           = vol->Left();
			vor           = vor->Right();
			vor->ancestor = &v;

			const float shift = (vil->x + sil) - (vir->x + sir) + distance;
			if (shift > 0)
			{
				MoveSubtree(Ancestor(*vil, v, default_ancestor), v, shift);
				sir += shift;
				sor += shift;
			}
			sil += vil->mod;
			sir += vir->mod;
			sol += vol->mod;
			sor += vor->mod;
		}

		if (vil->Right() && !vor->Right())
		{
			vor->thread = vil->Right();
			vor->mod += sil - sor;
		}

		if (vir->Left() && !vol->Left())
		{
			vol->thread = vir->Left();
			vol->mod += sir - sol;

			// References can't be reassigned, so we return here
			return v;
		}
	}
	return default_ancestor;
}

void MoveSubtree(BuchheimNode& wl, BuchheimNode& wr, float shift)
{
	const size_t subtress = wr.sibling_order_number - wl.sibling_order_number;
#if SHOW_DEBUG_MESSAGES
	std::cerr << wl.id << " is conflicted with " << wr.id << " moving " << subtress << " shift " << shift << std::endl;
#endif
	wr.change -= shift / subtress;
	wr.shift += shift;
	wl.change += shift / subtress;
	wr.x += shift;
	wr.mod += shift;
}

void ExecuteShifts(BuchheimNode& v)
{
	float shift  = 0;
	float change = 0;
	for (auto w = v.children.rbegin(); w != v.children.rend(); ++w)
	{
#if SHOW_DEBUG_MESSAGES
		std::cerr << "shift: " << w->id << " x=" << w->x << " " << shift << " " << w->change << std::endl;
#endif
		w->x += shift;
		w->mod += shift;
		change += w->change;
		shift += w->shift + change;
	}
}

BuchheimNode& Ancestor(BuchheimNode& vil, BuchheimNode& v, BuchheimNode& default_ancestor)
{
	bool bContainsAncestor = false;
	if (v.parent)
	{
		for (auto& child : v.parent->children)
		{
			if (vil.ancestor == &child)
			{
				bContainsAncestor = true;
				break;
			}
		}
	}

	return bContainsAncestor ? *vil.ancestor : default_ancestor;
}

float SecondWalk(BuchheimNode& v, float m /*= 0*/, int32_t depth /*= 0*/, float min /*= std::numeric_limits<float>::infinity()*/)
{
	v.x += m;
	v.y = static_cast<float>(depth);

	if (v.x < min || min == std::numeric_limits<float>::infinity())
	{
		min = v.x;
	}

	for (auto& w : v.children)
	{
		min = SecondWalk(w, m + v.mod, depth + 1, min);
	}

	return min;
}

void ThirdWalk(BuchheimNode& tree, float n)
{
	tree.x += n;
	for (auto& child : tree.children)
	{
		ThirdWalk(child, n);
	}
}
