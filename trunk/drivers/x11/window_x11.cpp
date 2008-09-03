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
#include <alloca.h>

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

unsigned long WindowX11::_map_color(const Color& p_color) {
	
	unsigned long col_r = p_color.r;
	if (r_shift>0)
		col_r<<=r_shift;
	else if (r_shift<0)
		col_r>>=-r_shift;

	unsigned long col_g = p_color.g;
	if (g_shift>0)
		col_g<<=g_shift;
	else if (g_shift<0)
		col_g>>=-g_shift;

	unsigned long col_b = p_color.b;
	if (b_shift>0)
		col_b<<=b_shift;
	else if (b_shift<0)
		col_b>>=-b_shift;
			
	return (col_r|col_g|col_b);

}

void WindowX11::draw_rect(const Point& p_from,const Size& p_size,const Color& p_color,bool p_fill) {

	XGCValues xgcvalues;
	xgcvalues.fill_style=FillSolid;
	xgcvalues.foreground=_map_color(p_color);
	xgcvalues.background=_map_color(p_color);

	XChangeGC(x11_display, x11_gc, GCForeground|GCBackground|GCFillStyle, &xgcvalues);

	XFillRectangle(x11_display, x11_window, x11_gc, p_from.x,p_from.y, p_size.width,p_size.height);

}
void WindowX11::draw_line(const Point& p_from,const Point& p_to,const Color& p_color,int p_width,bool p_round_endpoints) {

	XGCValues xgcvalues;
	xgcvalues.line_width=p_width;
	xgcvalues.cap_style=p_round_endpoints?CapRound:CapButt;
	xgcvalues.foreground=_map_color(p_color);

	XChangeGC(x11_display, x11_gc, GCForeground|GCCapStyle|GCLineWidth, &xgcvalues);

	XDrawLine(x11_display, x11_window, x11_gc, p_from.x,p_from.y, p_to.x, p_to.y );
}
void WindowX11::draw_polygon(const Point *p_points, int p_point_count,const Color& p_color,bool p_fill,int p_line_width) {

	if (p_point_count<=0 || p_point_count > 256) {
	
		GUI_PRINT_ERROR("Two many points!");
	}
		
	if (p_fill) {
			
		XPoint *xpoints = (XPoint*)alloca( p_point_count * sizeof(XPoint ) );
		
		//@TODO check against clip region before cancelling the poly
		
		for (int i=0;i<p_point_count;i++) {
			
			if (p_points[i].x<-32768 || p_points[i].x>32767 || p_points[i].y<-32768 || p_points[i].y>32767) {
				
				return; // polygon too big for x11 coordinate
			}
			xpoints[i].x=p_points[i].x;
			xpoints[i].y=p_points[i].y;
		}
		
		XGCValues xgcvalues;
		xgcvalues.fill_style=FillSolid;
		xgcvalues.foreground=_map_color(p_color);
		xgcvalues.background=_map_color(p_color);
	
		XChangeGC(x11_display, x11_gc, GCForeground|GCBackground|GCFillStyle, &xgcvalues);
	
		XFillPolygon(x11_display, x11_window, x11_gc, xpoints, p_point_count,Complex,CoordModeOrigin);
	} else {
	
		XGCValues xgcvalues;
		xgcvalues.line_width=p_line_width;
		xgcvalues.cap_style=CapRound;
		xgcvalues.foreground=_map_color(p_color);
	
		XChangeGC(x11_display, x11_gc, GCForeground|GCCapStyle|GCLineWidth, &xgcvalues);
	
		for (int i=0;i<p_point_count;i++) {
		
			int prev_i=(i==0)?p_point_count-1:i-1;
			
			
			
			XDrawLine(x11_display, x11_window, x11_gc, p_points[prev_i].x,p_points[prev_i].y,p_points[i].x, p_points[i].y );
		}
	}
	
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

			// should stack them, but fro now..
			Rect expose_rect( Point( p_event.xexpose.x, p_event.xexpose.y), Size(p_event.xexpose.width, p_event.xexpose.height ));

			update_event_signal.call( expose_rect );
			

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

static void _configure_shift_and_mask(int& p_shift, unsigned long& p_mask, unsigned long p_src) {

	int state=0; // searching
	int bit_start=-1,bit_end=-1;

	for (int i=(sizeof(unsigned long)*8);i>=0;i--) {

		bool bit;

		if (i==0)
			bit=0;
		else
			bit=(p_src&(1UL<<(i-1)))?true:false;
			

		switch(state) {
			case 0: // waiting for a bit to happen

				if (bit) {
					state=1;
					bit_start=i;
				}
			break;
			case 1: // waiting for no bit
				if (!bit) {
					state=2;
					bit_end=i;
				}
			break;
			default: {}

		}
	}

	if (bit_start<0 || bit_end<0) {

		GUI_PRINT_ERROR("Invalid BitMask.");
		return;
	}

	p_shift=bit_start-8;
	unsigned long len = bit_start-bit_end;
	p_mask=(len-1) << (8-len);

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

 	x11_gc = XCreateGC(x11_display, x11_window, 0, 0);
	Visual* visual = DefaultVisual( x11_display, 0 );

	// calculate r_shift, 
	_configure_shift_and_mask(r_shift,r_mask,visual->red_mask);
	_configure_shift_and_mask(g_shift,g_mask,visual->green_mask);
	_configure_shift_and_mask(b_shift,b_mask,visual->blue_mask);

	printf("r_shift %i, r_mask %i\n",r_shift,r_mask);
	printf("g_shift %i, g_mask %i\n",g_shift,g_mask);
	printf("b_shift %i, b_mask %i\n",b_shift,b_mask);
	printf("brgb %i\n",visual->bits_per_rgb);

}


WindowX11::~WindowX11() {

}


}

#endif
