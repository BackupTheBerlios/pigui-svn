//
// C++ Interface: window_x11
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef GUIWINDOW_X11_H
#define GUIWINDOW_X11_H

#ifdef PIGUI_X11_ENABLED 

#include "base/platform.h"
#include <X11/Xlib.h>


namespace GUI {

class PlatformX11;
/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class WindowX11 : public PlatformWindow {
friend class PlatformX11;

	// x11

	Window x11_window; // this is the actuall X11 window
	Display *x11_display;
	WindowX11 * next;

	WindowX11( PlatformX11 *p_platform,Display *p_x11_display,Window p_x11_window,WindowX11 * p_next=NULL );

	GC x11_gc;

	unsigned long r_mask;	
	unsigned long g_mask;	
	unsigned long b_mask;	
	int r_shift,g_shift,b_shift;

	unsigned long _map_color(const Color& p_color);

	// logic

	bool visible;
public:

	virtual void set_title(String p_title);
	virtual String get_title() const;

	virtual void set_pos(const Point& p_pos);
	virtual Point get_pos() const;

	virtual void set_size(const Point& p_pos);
	virtual Size get_size() const;

	virtual void set_flags(unsigned int p_flags);
	virtual unsigned int get_flags();

	virtual void set_icon(const Pixmap& p_icon);

	virtual void set_visible(bool p_visible);
	virtual bool is_visible() const;

	virtual void make_root();
	virtual bool is_root();

	/* drawing interface */

	virtual void draw_rect(const Point& p_from,const Size& p_size,const Color& p_color,bool p_fill=true);
	virtual void draw_line(const Point& p_from,const Point& p_to,const Color& p_color,int p_width=1,bool p_round_endpoints=false);
	virtual void draw_poly(const Point *p_points, int p_point_count,const Color& p_color,bool p_fill=true);
	virtual void draw_circle(const Point p_center, int p_radius,const Color& p_color,bool p_fill=true);
	virtual void draw_arrow(const Point& p_from,const Size& p_size,Direction p_dir,const Color& p_color,bool p_fill=true);
		
	virtual void draw_pixmap(const Pixmap& p_pixmap, const Point& p_pos,const Rect& p_source);	
	virtual void draw_string(const Font& p_font,const Point& p_point,Direction p_dir,int p_clip_len=-1);

	virtual void draw_stylebox( const StyleBox& p_style,const Point& p_from,const Size& p_size);
	
	virtual void draw_set_clipping(bool p_enabled,const Rect& p_rect=Rect());

	void process_x11_event(const XEvent& p_event);
	inline WindowX11 *get_next() const { return next; }
	inline Window get_x11_window() const { return x11_window; }

	WindowX11();	
	~WindowX11();

};

}

#endif
#endif

