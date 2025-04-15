#include <iostream>
#include <vector>

#include "Buchheim.h"
#include "CImg.h"
#include "DrawUtils.h"
#include "Knuth.h"
#include "Wetherell-Shannon.h"

// clang-format off
SimpleNode simplenode_tree =
{
	"root",
	{
		{
			"left",
			{
				{ "left-left" },
				{
					"left-right",
					{
						{ "LRL" },
						{
							"LRR",
							{
								{ "LRRL" }
							}
						}
					}
				}
			}
		},
		{
			"right",
			{
				{
					"right-left",
					{
						{
							"RLL",
							{
								{ "RLLL" }
							}
						}
					}
				}
			}
		}
	}
};
// clang-format on

int main()
{
	using namespace cimg_library;

	Theme& current_theme = theme_dracula;
	CImg<unsigned char> knuth_img(970, 430, 1, current_theme.channels);
	CImg<unsigned char> ws_min_img(248, 430, 1, current_theme.channels);
	CImg<unsigned char> ws2_img(610, 430, 1, current_theme.channels);
	CImg<unsigned char> buchheim_img(320, 430, 1, current_theme.channels);

	// Fill the background
	for (int i = 0; i < current_theme.channels; ++i)
	{
		knuth_img.get_shared_channel(i).fill(current_theme.background[i]);
		ws_min_img.get_shared_channel(i).fill(current_theme.background[i]);
		ws2_img.get_shared_channel(i).fill(current_theme.background[i]);
		buchheim_img.get_shared_channel(i).fill(current_theme.background[i]);
	}

	// Draw our trees
	SetupKnuth(simplenode_tree);
	DrawConnections(knuth_img, current_theme, simplenode_tree);
	DrawNodes(knuth_img, current_theme, simplenode_tree);
	// knuth_img.save("knuth.bmp");

	SetupWSMinimum(simplenode_tree);
	DrawConnections(ws_min_img, current_theme, simplenode_tree);
	DrawNodes(ws_min_img, current_theme, simplenode_tree);
	// ws_min_img.save("ws_min.bmp");

	WS2Node ws2tree(simplenode_tree);
	SetupWS2(ws2tree);
	DrawConnections(ws2_img, current_theme, ws2tree);
	DrawNodes(ws2_img, current_theme, ws2tree);
	// ws2_img.save("ws2.bmp");

	BuchheimNode buchheim_tree(simplenode_tree);
	Buchheim(buchheim_tree);
	DrawConnections(buchheim_img, current_theme, buchheim_tree);
	DrawThreads(buchheim_img, current_theme, buchheim_tree);
	DrawNodes(buchheim_img, current_theme, buchheim_tree);
	// buchheim_img.save("buchheim.bmp");

	// Display
	CImgList<unsigned char> list;
	list.insert(knuth_img);
	list.insert(ws_min_img);
	list.insert(ws2_img);
	list.insert(buchheim_img);

	CImgDisplay display(list, "Tree drawing algorithms", 0);
	while (!display.is_closed())
	{
		display.wait();
	}

	return 0;
}
