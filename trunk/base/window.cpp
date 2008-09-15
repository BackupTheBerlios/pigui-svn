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
#include "base/container.h"

namespace GUI {


class WindowPrivate {
public:

	PlatformWindow *platform_window;
	Container *root_container;

};


bool Window::get_state(WindowState p_state) const {

	if (!wp->platform_window)
		return false;
		
	return wp->platform_window->get_state(p_state);

}
void Window::set_state( WindowState p_state, bool p_enabled) {

	wp->platform_window->set_state( p_state, p_enabled );
}


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

	if (wp->root_container) {	
		GUI_PRINT_ERROR("Window already has root container.");
		return;
	}
	
	wp->root_container=p_root;
	// This will force the root container to expand the window to wathever size it needs, redraw, etc.
	wp->root_container->window_request_size(Size(1,1)); 
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

	if (wp->root_container)
		GUI_DELETE( wp->root_container );

	if (wp->platform_window)
		delete wp->platform_window;
}


}
