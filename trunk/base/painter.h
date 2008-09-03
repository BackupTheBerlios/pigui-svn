//
// C++ Interface: painter
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PAINTER_H
#define PAINTER_H

#include "base/geometry.h"
#include "base/pixmap.h"
#include "base/style_box.h"
#include "base/font.h"

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/

namespace GUI {

class Painter {
public:

	virtual void draw_rect(const Point& p_from,const Size& p_size,const Color& p_color,bool p_fill=true)=0;
	virtual void draw_line(const Point& p_from,const Point& p_to,const Color& p_color,int p_width=1)=0;
	virtual void draw_polygon(const Point *p_points, int p_point_count,const Color& p_color,bool p_fill=true)=0;
	virtual void draw_circle(const Point p_center, int p_radius,const Color& p_color,bool p_fill=true)=0;
	virtual void draw_arrow(const Point& p_from,const Size& p_size,Direction p_dir,const Color& p_color,bool p_fill=true)=0;
	
	void draw_pixmap(const Pixmap& p_pixmap, const Point& p_pos); // helper
	virtual void draw_pixmap(const Pixmap& p_pixmap, const Point& p_pos,const Rect& p_source)=0;
	
	void draw_string(const Font& p_font,const Point& p_point,int p_clip_len=-1); // helper
	virtual void draw_string(const Font& p_font,const Point& p_point,Direction p_dir,int p_clip_len=-1)=0;

	virtual void draw_stylebox( const StyleBox& p_style,const Point& p_from,const Size& p_size)=0;
	
	virtual void set_clipping(bool p_enabled,const Rect& p_rect=Rect())=0;

	virtual ~Painter();

};

} // end of namespace

#endif
