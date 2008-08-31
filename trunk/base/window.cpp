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

Window::Window(unsigned int p_flags) {

	platform_window = Platform::get_singleton()->create_window(p_flags);
}


Window::~Window() {

	if (platform_window)
		delete platform_window;
}


}
