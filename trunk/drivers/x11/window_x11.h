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
#include "X11/extensions/Xrender.h"
#include "X11/Xft/Xft.h"


namespace GUI {

class PlatformX11;
/**
	@author Juan Linietsky <reduzio@gmail.com>
*/

class WindowX11Private;

class WindowX11 : public PlatformWindow {
friend class PlatformX11;

	WindowX11Private *wp;
    	
	WindowX11( PlatformX11 *p_platform,Display *p_x11_display,::Window p_x11_window,WindowX11 * p_next=NULL,WindowX11 *p_parent=NULL );

	unsigned long _map_color(const Color& p_color);

	unsigned int fill_modifier_button_mask(unsigned int p_x11_state);
	
	void handle_key_event(XKeyEvent *p_event);

	void check_hide_popup_stack();
	
	bool redirect_focus(bool p_force=true);
	// logic
	
	void set_modal(); 
	void remove_modal();
	void set_popup(); 
	void remove_popup();
	
public:

	virtual void set_title(String p_title);
	virtual String get_title() const;

	virtual void set_pos(const Point& p_pos);
	virtual Point get_pos() const;

	virtual void set_size(const Point& p_pos);
	virtual Size get_size() const;

	virtual void set_icon(const Pixmap& p_icon);

	virtual void set_state(WindowState p_state,bool p_enabled);
	virtual bool get_state(WindowState p_state);


	/* drawing interface */

	virtual void draw_rect(const Point& p_from,const Size& p_size,const Color& p_color,bool p_fill=true);
	virtual void draw_line(const Point& p_from,const Point& p_to,const Color& p_color,int p_width=1,bool p_round_endpoints=false);
	virtual void draw_polygon(const Point *p_points, int p_point_count,const Color& p_color,bool p_fill=true,int p_line_width=1);
	virtual void draw_circle(const Point p_center, int p_radius,const Color& p_color,bool p_fill=true);
	virtual void draw_arrow(const Point& p_from,const Size& p_size,Direction p_dir,const Color& p_color,bool p_fill=true);
		
	virtual void draw_pixmap(const Pixmap& p_pixmap, const Point& p_pos,const Rect& p_source);	
	virtual void draw_string(const Font& p_font,const Point& p_pos,const Color& p_color,const String& p_string,Direction p_dir=RIGHT);

	
	virtual void draw_stylebox( const StyleBox& p_style,const Point& p_from,const Size& p_size);
	
	virtual void draw_set_clipping(bool p_enabled,const Rect& p_rect=Rect());

	virtual void add_dirty_region(const Rect& p_rect);
	virtual void request_size_update();

	void process_x11_event(XEvent* p_event);
	WindowX11 *get_next(); 
	Window get_x11_window();

	WindowX11();	
	~WindowX11();

};

}

#endif
#endif

