//
// C++ Implementation: window
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "window.h"

#include "base/platform.h"

namespace GUI {


class WindowPrivate {
public:

	PlatformWindow *platform_window;
	Container *root_container;

};


void Window::_update_notify(Rect p_rect) {

	if (!wp->root_container)
		return;
		
		
}

Frame *Window::get_focus_frame() {

	return NULL;
}

void Window::set_focus_frame(Frame *p_frame) {


}

void Window::frame_deleted_notify(Frame *p_frame) {


}

void Window::update_region(const Rect& p_rect,Frame *p_owner) {


}

Theme *Window::get_theme() {

	return NULL;
}

Container *Window::get_root_container() {


	return NULL;
}

void Window::set_root_container(Container *p_root) {


}


void Window::set_minimum_size_changed(const Size& p_new_minsize) {


}


void Window::add_dirty_container(Container *p_container) {


}

Window::Window() {

	wp = GUI_NEW( WindowPrivate );
	
	wp->platform_window = Platform::get_singleton()->create_window();
	wp->root_container=0;
}


Window::~Window() {

	if (wp->platform_window)
		delete wp->platform_window;
}


}
