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
#include <stdio.h>

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

void Window::mouse_button_callback(Point p_point, int p_button,bool p_pressed, int p_mask) {

	printf("button\n");
}
void Window::mouse_doubleclick_callback(Point p_point, int p_mask) {


}
void Window::mouse_motion_callback(Point p_pos,Point p_pos_rel, int p_mask ) {


}
void Window::mouse_enter_window_callback() {


}
void Window::mouse_exit_window_callback() {


}
void Window::key_callback(int p_unicode, int p_scancode, bool p_pressed, bool p_echo, int p_mask ) {


}
void Window::update_callback(Rect p_rect) {


}
void Window::size_update_callback() {


}
void Window::focus_enter_callback() {


}
void Window::focus_exit_callback() {


}
void Window::close_request_callback() {


}


Window::Window() {

	_wp = GUI_NEW( WindowPrivate );
	
	_wp->platform_window = Platform::get_singleton()->create_window();
	_wp->root_container=0;
	
	/* Connect Platformwindow Signals */
	_wp->platform_window->connect( "mouse_button", this, &Window::mouse_button_callback );
	_wp->platform_window->connect( "mouse_doubleclick", this, &Window::mouse_doubleclick_callback );
	_wp->platform_window->connect( "mouse_motion", this, &Window::mouse_motion_callback );
	_wp->platform_window->connect( "mouse_enter_window", this, &Window::mouse_enter_window_callback );
	_wp->platform_window->connect( "mouse_exit_window", this, &Window::mouse_exit_window_callback );
	_wp->platform_window->connect( "key", this, &Window::key_callback );
	_wp->platform_window->connect( "update", this, &Window::update_callback );
	_wp->platform_window->connect( "size_update", this, &Window::size_update_callback );
	_wp->platform_window->connect( "focus_enter", this, &Window::focus_enter_callback );
	_wp->platform_window->connect( "focus_exit", this, &Window::focus_exit_callback );
	_wp->platform_window->connect( "close_request", this, &Window::close_request_callback );
	
}


Window::~Window() {

	if (_wp->root_container)
		GUI_DELETE( _wp->root_container );

	if (_wp->platform_window)
		delete _wp->platform_window;
}


}
