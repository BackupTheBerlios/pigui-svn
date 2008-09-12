//
// C++ Interface: style_box
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef GUISTYLE_BOX_H
#define GUISTYLE_BOX_H

#include "base/geometry.h"
#include "base/constants.h"
#include "base/pixmap.h"

namespace GUI {

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/

class Painter;

class StyleBoxMethod {
friend class StyleBox;
	int refcount;
	int custom_margins[4];	
public:


	virtual void draw(Painter *p_painter,const Rect& p_rect) const=0;
	virtual int get_margin(Margin p_margin) const=0;
	
	void set_custom_margins( int p_left, int p_top, int p_right, int p_bottom);
	int get_custom_margin( Margin p_margin ) const;
	void clear_custom_margins();	
	
	
	StyleBoxMethod() { refcount=0; clear_custom_margins(); };		
	virtual ~StyleBoxMethod() {};
};


class StyleBox {

	StyleBoxMethod *method;
	
	void reference( const StyleBox& p_stylebox );
	void unreference();
public:

	void operator=(const StyleBox& p_stylebox);
		
	void draw(Painter *p_painter, const Rect& p_rect) const;
	int get_margin(Margin p_margin) const;

	Size get_minimum_size() const;
	Rect enter_rect(const Rect& p_rect) const; 

	StyleBox();
	StyleBox(const StyleBox& p_stylebox);
	StyleBox(StyleBoxMethod *p_method);
	~StyleBox();
};

class StyleBoxFlatMethod {

	Color color_center;
	Color color_top_left;
	Color color_bottom_right;
	int margin;	
	bool draw_center;
public:

	virtual void draw(Painter *p_painter,const Rect& p_rect) const=0;
	virtual int get_margin(Margin p_margin) const=0;

	StyleBoxFlatMethod( int p_margin, const Color& p_color_center, const Color& p_color_top_left, const Color& p_color_bottom_right,bool p_draw_center=true );
};


#if 0
class StyleBox {
public:

	enum Mode {
		/* stylebox modes */
		MODE_NONE, ///< empty stylebox
		MODE_FLAT, ///< flat stylebox with gradient margins
		MODE_FLAT_PIXMAP, ///< flat stylebox with pixmap margins
		MODE_PIXMAP, ///< pixmap based stylebox
		MODE_USER
	};
private:	

	Mode mode;
		
	struct {
	
		Color color_center;
		Color color_top_left;
		Color color_bottom_right;
		int margin;
	} flat;
		
	Pixmap pixmaps[STYLEBOX_PIXMAPS_MAX];
	
	StyleBoxMethod * user;
	
	int custom_margins[4];
public:

	Mode get_mode() const;
	
	void clear();
	
	/* FLAT */

	void create_flat(int p_margin, const Color& p_color_center, const Color& p_color_top_left, const Color& p_color_bottom_right );
	
	int get_flat_margin() const;
	Color get_flat_color_center() const;
	Color get_flat_color_top_left() const;
	Color get_flat_color_bottom_right() const;
	
	/* FLAT PIXMAP */
	
	void create_flat_pixmap( Pixmap *p_8_pixmaps, const Color& p_color_center );
	void create_flat_pixmap(const Pixmap& p_pixmap, int p_left, int p_top, int p_right, int p_botton, const Color& p_color_center); 
		
	/* PIXMAP */
		
	void create_pixmap( Pixmap *p_9_pixmaps );
	void create_pixmap(const Pixmap& p_pixmap, int p_left, int p_top, int p_right, int p_botton); 
	
	Pixmap get_pixmap(StyleBoxPixmaps p_pixmap) const;
		
	void create_userdraw( StyleBoxMethodDrawFunc p_draw_func, StyleBoxMethodDrawGetMarginFunc p_get_margin_func, void * p_userdata );
	
	StyleBoxMethodDrawFunc get_usedraw_func() const;
	StyleBoxMethodDrawGetMarginFunc get_get_margin_func() const;
	void *get_usedraw_data() const;
	
	
	void set_custom_margins( int p_left, int p_top, int p_right, int p_bottom);
	int get_custom_margin( Margin p_margin ) const;
	void clear_custom_margins();	

	int get_margin( Margin p_margin ) const;
	Size get_minimum_size() const;
	Rect enter_rect(const Rect& p_rect) const; 

	void draw( Painter *p_painter, const Rect& p_rect );
	
	StyleBox();	
	~StyleBox();

};

#endif
}

#endif
