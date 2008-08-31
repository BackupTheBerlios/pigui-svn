//
// C++ Implementation: theme
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "theme.h"

namespace GUI {

const StyleBox& Theme::get_stylebox(int p_index) const {


	static Stylebox none;
	return none;
}
const Pixmap& Theme::get_pixmap(int p_index) const {


	static Pixmap none;
	return none;
}
const Font& Theme::get_font(int p_index) const {


	static Font none;
	return none;
}
int Theme::get_constant(int p_index) const {

	return 0;
}
const Color& Theme::get_color(int p_index) const {


	static Color none;
	return none;
}


Theme::Theme()
{
}


Theme::~Theme()
{
}


}