//
// C++ Interface: theme
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef GUI_THEME_H
#define GUI_THEME_H


#include "base/style_box.h"
#include "base/font.h"
#include "base/pixmap.h"

namespace GUI {
/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class Theme{
public:
	
	const StyleBox& get_stylebox(int p_index) const;
	const Pixmap& get_pixmap(int p_index) const;
	const Font& get_font(int p_index) const;
	int get_constant(int p_index) const;
	const Color& get_color(int p_index) const;

	Theme();	
	~Theme();

};

}
#endif
