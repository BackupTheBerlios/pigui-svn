//
// C++ Implementation: platform_x11
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "platform_x11.h"

#ifdef PIGUI_X11_ENABLED

#include <unistd.h>

namespace GUI {


PlatformTheme *PlatformX11::get_theme() const {

	return NULL;
}
PlatformPointer *PlatformX11::get_pointer() const {

	return NULL;
}

PlatformWindow* PlatformX11::create_window(unsigned int p_flags) {
 
	int black = BlackPixel(x11_display, DefaultScreen(x11_display));
	Window w = XCreateSimpleWindow(x11_display, DefaultRootWindow(x11_display), 0, 0, 
				     50, 50, 0, black, black);
	
	if (w==0) {

		GUI_PRINT_ERROR("XCreateSimpleWindow failed.")
		return NULL;
	}

	window_list = GUI_NEW( WindowX11( this, x11_display, w, window_list ) );

	WindowX11 * new_window = window_list;

	return new_window;
}
PlatformPixmap* PlatformX11::create_pixmap() {

	return NULL;
}
PlatformFont* PlatformX11::create_font() {

	return NULL;
}
PlatformFile *PlatformX11::create_file() {

	return NULL;
}
PlatformFileSystem *PlatformX11::create_file_system() {

	return NULL;
}
PlatformTimer *PlatformX11::create_timer() {

	return NULL;
}


int PlatformX11::run() {

	
	while(!exit) {

		iteration();
	}

	return 0;
}

void PlatformX11::iteration() {

	XFlush(x11_display);

	XEvent event;
	XNextEvent(x11_display, &event);

	// dispatch the event to whoever owns it (TODO must optimize this)
	WindowX11 *w = window_list;
	
	while(w) {

		if (w->get_x11_window() == event.xany.window) {

			w->process_x11_event(event);
			break;
		}
		w=w->get_next();
	}

}

PlatformX11::PlatformX11() {
	
	exit=false;
	x11_display = XOpenDisplay(NULL);
	if (!x11_display) {
	
		GUI_PRINT_ERROR("Unable to open X11 x11_display.");
		_exit(255);
	}
	
	

}


PlatformX11::~PlatformX11() {

}

}

#endif