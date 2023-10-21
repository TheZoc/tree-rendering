#ifndef _DRAWUTILS_H_
#define _DRAWUTILS_H_

#include "CImg.h"
#include "SimpleNode.h"

struct Theme
{
	const int channels                = 0;
	const unsigned char* background   = nullptr;
	const unsigned char* foreground   = nullptr;
	const unsigned char* node_color   = nullptr;
	const unsigned char* accent       = nullptr;
	const unsigned char* thread_color = nullptr;
};

// Grayscale
const unsigned char grayscale_black[]      = { 0 };
const unsigned char grayscale_white[]      = { 255 };
const unsigned char grayscale_light_gray[] = { 160 };
const unsigned char grayscale_dark_gray[]  = { 51 };

Theme theme_greyscale_light = {
	1,
	grayscale_white,
	grayscale_black,
	grayscale_light_gray,
	grayscale_dark_gray,
	grayscale_dark_gray
};

// RGB
const unsigned char rgb_black[] = { 0, 0, 0 };
const unsigned char rgb_white[] = { 255, 255, 255 };

// Dracula
const unsigned char dracula_bg[]      = { 40, 42, 54 };
const unsigned char dracula_fg[]      = { 248, 248, 242 };
const unsigned char dracula_comment[] = { 98, 114, 164 };
const unsigned char dracula_green[]   = { 80, 250, 123 };
const unsigned char dracula_purple[]  = { 189, 147, 249 };

Theme theme_dracula = {
	3,
	dracula_bg,
	dracula_fg,
	dracula_comment,
	dracula_green,
	dracula_purple
};

//////////////////////////////////////////////////////////////////////////

constexpr unsigned int DIAMETER           = 30;
constexpr unsigned int MARGIN_LEFT        = DIAMETER + 4;
constexpr unsigned int MARGIN_TOP         = DIAMETER + 4;
constexpr unsigned int SPACING_VERTICAL   = DIAMETER * 3;
constexpr unsigned int SPACING_HORIZONTAL = DIAMETER * 3;

template <typename T>
void DrawNodes(cimg_library::CImg<T>& img, Theme& current_theme, SimpleNode& node)
{
	using namespace cimg_library;

	// Draw node
	img.draw_circle(MARGIN_LEFT + node.x * SPACING_HORIZONTAL,
					MARGIN_TOP + node.y * SPACING_VERTICAL,
					DIAMETER,
					current_theme.node_color,
					1);

	// Measure the text size
	constexpr unsigned int font_size   = 13;
	constexpr unsigned int local_black = 0;
	CImg<unsigned char> text_measuring;
	text_measuring.draw_text(0, 0, node.id.c_str(), &local_black, 0, 1, font_size);

	// Draw text
	img.draw_text(MARGIN_LEFT + node.x * SPACING_HORIZONTAL - text_measuring.width() / 2,
				  MARGIN_TOP + node.y * SPACING_VERTICAL - text_measuring.height() / 2,
				  node.id.c_str(),
				  current_theme.foreground, 0, 1, font_size);

	for (auto& child : node.children)
	{
		DrawNodes(img, current_theme, child);
	}
}

template <typename T>
void DrawConnections(cimg_library::CImg<T>& img, Theme& current_theme, SimpleNode& node)
{
	using namespace cimg_library;

	// Draw connections to all children
	for (auto& child : node.children)
	{
		img.draw_line(MARGIN_LEFT + node.x * SPACING_HORIZONTAL,
					  MARGIN_TOP + node.y * SPACING_VERTICAL,
					  MARGIN_LEFT + child.x * SPACING_HORIZONTAL,
					  MARGIN_TOP + child.y * SPACING_VERTICAL,
					  current_theme.accent,
					  1,
					  ~0U);

		DrawConnections(img, current_theme, child);
	}
}

template <typename T, typename NODE>
void DrawThreads(cimg_library::CImg<T>& img, Theme& current_theme, NODE& node)
{
	using namespace cimg_library;

	// Draw connections to all children
	for (auto& child : node.children)
	{
		if (child.thread)
		{

			img.draw_line(MARGIN_LEFT + child.x * SPACING_HORIZONTAL,
						  MARGIN_TOP + child.y * SPACING_VERTICAL,
						  MARGIN_LEFT + child.thread.x * SPACING_HORIZONTAL,
						  MARGIN_TOP + child.thread.y * SPACING_VERTICAL,
						  current_theme.thread_color,
						  1,
						  0xF0F0F0F0);
		}

		DrawConnections(img, current_theme, child);
	}
}

#endif // _DRAWUTILS_H_
