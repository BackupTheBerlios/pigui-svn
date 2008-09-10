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

namespace GUI {

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/

class Container;
class Frame;

class PlatformWindow;
class Window { 

	PlatformWindow *platform_window;
	
public:

	Frame *get_focus_frame();
	void set_focus_frame(Frame *p_frame);

	void add_dirty_container(Container *p_container);
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
