//
// C++ Interface: window
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef GUIWINDOW_H
#define GUIWINDOW_H


#include "base/theme.h"
#include "base/object.h"
#include "base/list.h"
namespace GUI {

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/

class Container;
class Frame;

class WindowPrivate;

class Window : public Object { 

	GUI_TYPE( Window, Object );
	
	WindowPrivate *_wp;
		
	void _update_notify(Rect p_rect);
	
	/* Callbacks from PlatformWindow */	
	
	void mouse_button_callback(Point p_point, int p_button,bool p_pressed, int p_mask);
	void mouse_doubleclick_callback(Point p_point, int p_mask);
	void mouse_motion_callback(Point p_pos,Point p_pos_rel, int p_mask );
	void mouse_enter_window_callback();
	void mouse_exit_window_callback();
	void key_callback(int p_unicode, int p_scancode, bool p_pressed, bool p_echo, int p_mask );
	void update_callback(Rect p_rect);
	void size_update_callback();
	void focus_enter_callback();
	void focus_exit_callback();
	void close_request_callback();
	
friend class Container;		
		
	List<Container*>::Element* add_container_pending_resize(Container *p_container);
		
public:
	/* Window Info */
	
	bool get_state(WindowState p_state) const;
	void set_state( WindowState p_state, bool p_enabled );

	Frame *get_focus_frame();
	void set_focus_frame(Frame *p_frame);

	void set_minimum_size_changed(const Size& p_new_minsize);

	void frame_deleted_notify(Frame *p_frame);

	void update_region(const Rect& p_rect,Frame *p_owner);

	Container *get_root_container();
	void set_root_container(Container *p_root);

	Theme *get_theme();

	Window();	
	~Window();

};


}

#endif
