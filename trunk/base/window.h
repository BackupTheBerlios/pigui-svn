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
