//
// C++ Implementation: window_x11
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifdef PIGUI_X11_ENABLED 

#include "window_x11.h"
#include "drivers/x11/platform_x11.h"
#include <stdio.h>


namespace GUI {

void WindowX11::set_title(String p_title) {

}
String WindowX11::get_title() const {

	return "";
}

void WindowX11::set_pos(const Point& p_pos) {

}
Point WindowX11::get_pos() const {

	return Point();
}

void WindowX11::set_size(const Point& p_pos) {

}
Size WindowX11::get_size() const {

	return Size();
}

void WindowX11::set_flags(unsigned int p_flags) {


}
unsigned int WindowX11::get_flags() {

	return 0;
}

void WindowX11::set_icon(const Pixmap& p_icon) {

}

void WindowX11::set_visible(bool p_visible) {

}
bool WindowX11::is_visible() const {

	return true;
}

void WindowX11::make_root() {

}

bool WindowX11::is_root() {

	return false;
}

void WindowX11::draw_rect(const Point& p_from,const Size& p_size,const Color& p_color,bool p_fill) {

}
void WindowX11::draw_line(const Point& p_from,const Point& p_to,const Color& p_color,int p_width) {

}
void WindowX11::draw_poly(const Point *p_points, int p_point_count,const Color& p_color,bool p_fill) {

}
void WindowX11::draw_circle(const Point p_center, int p_radius,const Color& p_color,bool p_fill) {

}
void WindowX11::draw_arrow(const Point& p_from,const Size& p_size,Direction p_dir,const Color& p_color,bool p_fill) {

}
	
void WindowX11::draw_pixmap(const Pixmap& p_pixmap, const Point& p_pos,const Rect& p_source) {

}
void WindowX11::draw_string(const Font& p_font,const Point& p_point,Direction p_dir,int p_clip_len) {

}

void WindowX11::draw_stylebox( const StyleBox& p_style,const Point& p_from,const Size& p_size) {

}

void WindowX11::draw_set_clipping(bool p_enabled,const Rect& p_rect) {

}

void WindowX11::process_x11_event(const XEvent& p_event) {

	switch(p_event.type) {

		case KeyPress: {

			printf("key press\n");
		} break;
		case KeyRelease: {

			printf("key release\n");

		} break;
		case ButtonPress: {

			printf("button press\n");

		} break;
		case ButtonRelease: {
			

			printf("button release\n");

		} break;
		case MotionNotify: {

			printf("motion notify\n");

		} break;
		case EnterNotify: {

			printf("enter notify\n");

		} break;
		case LeaveNotify: {

			printf("leave notify\n");

		} break;
		case FocusIn: {

			printf("focus in\n");

		} break;
		case FocusOut: {

			printf("focus out\n");

		} break;
		case KeymapNotify: {

			printf("key map\n");

		} break;
		case Expose: {

			printf("expose\n");

		} break;
		case GraphicsExpose: {

			printf("graphics expose\n");

		} break;
		case NoExpose: {

			printf("no expose\n");

		} break;
		case VisibilityNotify: {

			printf("visibility notify\n");

		} break;
		case CreateNotify: {

			printf("create notify\n");

		} break;
		case DestroyNotify: {

			printf("destroy notify\n");

		} break;
		case UnmapNotify: {

			printf("unmap notify\n");

		} break;
		case MapNotify: {

			printf("map notify\n");

		} break;
		case MapRequest: {

			printf("map request\n");

		} break;
		case ReparentNotify: {

			printf("reparent notify \n");

		} break;
		case ConfigureNotify: {

			printf("configure notify\n");

		} break;
		case ConfigureRequest: {

			printf("configure request\n");

		} break;
		case GravityNotify: {

			printf("gravity notify\n");

		} break;
		case ResizeRequest: {

			printf("resize notify\n");

		} break;
		case CirculateNotify: {

			printf("circulate notify\n");

		} break;
		case CirculateRequest: {

			printf("circulate request\n");

		} break;
		case PropertyNotify: {

			printf("property notify\n");

		} break;
		case SelectionClear: {

			printf("selection clear notify\n");

		} break;
		case SelectionRequest: {

			printf("selection request notify\n");

		} break;
		case SelectionNotify: {


			printf("selection notify\n");
		} break;
		case ColormapNotify: {

			printf("colormap notify\n");

		} break;
		case ClientMessage: {

			printf("client message\n");

		} break;
		case MappingNotify: {

			printf("mapping notify \n");

		} break;
	}
}

WindowX11::WindowX11( PlatformX11 *p_platform,Display *p_x11_display,Window p_x11_window, WindowX11 * p_next ) : PlatformWindow( p_platform ) {

	x11_window=p_x11_window;
	x11_display=p_x11_display;
	next=p_next;
	visible=true;

	unsigned long events=0;
	events|=KeyPressMask;
	events|=KeyReleaseMask;
	events|=ButtonPressMask;
	events|=ButtonReleaseMask;
	events|=PointerMotionMask;
	events|=EnterWindowMask;
	events|=LeaveWindowMask;
	events|=ExposureMask;
	events|=FocusChangeMask;
	events|=StructureNotifyMask; // map/unmap/destroy/etc/etc/etc
	

	XSelectInput(x11_display, x11_window, events);
	XMapWindow(x11_display, x11_window);


}


WindowX11::~WindowX11() {

}


}

#endif
