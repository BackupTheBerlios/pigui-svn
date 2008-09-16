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

	List<Container*> containers_pending_resize;

	PlatformWindow *platform_window;
	Container *root_container;

};

List<Container*>::Element* Window::add_container_pending_resize(Container *p_container) {

	_wp->containers_pending_resize.push_back(p_container);
	return _wp->containers_pending_resize.back();
}

bool Window::get_state(WindowState p_state) const {

	if (!_wp->platform_window)
		return false;
		
	return _wp->platform_window->get_state(p_state);

}
void Window::set_state( WindowState p_state, bool p_enabled) {

	_wp->platform_window->set_state( p_state, p_enabled );
}


void Window::_update_notify(Rect p_rect) {

	if (!_wp->root_container)
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

	if (_wp->root_container) {	
		GUI_PRINT_ERROR("Window already has root container.");
		return;
	}
	
	_wp->root_container=p_root;
	// This will force the root container to expand the window to wathever size it needs, redraw, etc.
	_wp->root_container->window_request_size(Size(1,1)); 
}


void Window::set_minimum_size_changed(const Size& p_new_minsize) {


}



Window::Window() {

	_wp = GUI_NEW( WindowPrivate );
	
	_wp->platform_window = Platform::get_singleton()->create_window();
	_wp->root_container=0;
}


Window::~Window() {

	if (_wp->root_container)
		GUI_DELETE( _wp->root_container );

	if (_wp->platform_window)
		delete _wp->platform_window;
}


}
