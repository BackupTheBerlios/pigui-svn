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
#include "drivers/x11/pixmap_x11.h"
#include <stdio.h>
#include <alloca.h>
#include "key_mapping_x11.h"

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

	XRenderColor xrcolor;
	xrcolor.red=(unsigned short)p_color.r << 8;
	xrcolor.green=(unsigned short)p_color.g << 8;
	xrcolor.blue=(unsigned short)p_color.b << 8;
	xrcolor.alpha=(unsigned short)p_color.a << 8;

	XRenderFillRectangle( x11_display, PictOpOver, xrender_picture, &xrcolor, p_from.x,p_from.y,p_size.width,p_size.height );

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

	PixmapX11 *pp = platform_x11->extract_pixmap_x11(p_pixmap);

	if (!pp || !pp->is_valid())
		return;

	XRenderComposite (x11_display,PictOpOver,pp->get_xrender_picture(),0,xrender_picture,0,0,0,0,p_pos.x,p_pos.y,pp->get_size().width,pp->get_size().height );
	
}
void WindowX11::draw_string(const Font& p_font,const Point& p_pos,const Color& p_color,const String& p_string,Direction p_dir) {

	FontX11 *fnt = platform_x11->extract_font_x11( p_font );
	if (!fnt)
		return;
	
	XftFont *xft_font = fnt->get_xft_font();
	
	if (!xft_font)
		return;
	
	XftColor xft_color;
	xft_color.pixel=_map_color(p_color);
	xft_color.color.red=(unsigned short)p_color.r << 8;
	xft_color.color.green=(unsigned short)p_color.g << 8;
	xft_color.color.blue=(unsigned short)p_color.b << 8;
	xft_color.color.alpha=(unsigned short)p_color.a << 8;
		
	switch(sizeof(String::CharType)) {
		
		case 1: {
			
			XftDrawString8( xft_draw, &xft_color, xft_font, p_pos.x, p_pos.y, (const FcChar8*)p_string.c_str(), p_string.length() );
			
		} break;
		case 2: {
			
			XftDrawString16( xft_draw, &xft_color, xft_font, p_pos.x, p_pos.y, (const FcChar16*)p_string.c_str(), p_string.length() );
		} break;
		case 4: {
			
			XftDrawString32( xft_draw, &xft_color, xft_font, p_pos.x, p_pos.y, (const FcChar32*)p_string.c_str(), p_string.length() );
		} break;
	}
	
}

void WindowX11::draw_stylebox( const StyleBox& p_style,const Point& p_from,const Size& p_size) {

}

void WindowX11::draw_set_clipping(bool p_enabled,const Rect& p_rect) {

}
static void
dump (char *str, int len)
{
    printf("(");
    len--;
    while (len-- > 0)
        printf("%02x ", (unsigned char) *str++);
    printf("%02x)", (unsigned char) *str++);
}

void WindowX11::handle_key_event(XKeyEvent *p_event) {

			
	// X11 functions don't know what const is
	XKeyEvent *xkeyevent = p_event;
	
	/* Phase 1, obtain a proper keysym */
	
	// For reasons unknown to mankind, 
	// XKeycodeToKeysym doesn't work with kb. layouts.
	// XLookupString must be used instead,
	// which is kinda heavier.
	
	KeySym keysym_keycode; // keysym used to find a keycode
	KeySym keysym_unicode; // keysym used to find unicode
					
	int nbytes=0; // bytes the string takes
						 
	// XLookupString returns keysyms usable as nice scancodes/
	char str[256+1];
	nbytes=XLookupString(xkeyevent, str, 256, &keysym_keycode, NULL);
						 
 	// Meanwhile, XLookupString returns keysyms useful for unicode.

	if (!_xmbstring) {
		// keep a temporary buffer for the string
		_xmbstring=(char*)malloc(sizeof(char)*8);
		_xmblen=8;
	}			 
	
	if (xkeyevent->type == KeyPress && xic) {

		Status status;
		do {
			
			int mnbytes = XmbLookupString (xic, xkeyevent, _xmbstring, _xmblen - 1, &keysym_unicode, &status);
			_xmbstring[mnbytes] = '\0';

			if (status == XBufferOverflow) {
				_xmblen = mnbytes + 1;
				_xmbstring = (char*)realloc (_xmbstring, _xmblen);
			} else {
			
				printf("xmbs %s\n",_xmbstring);
			}
		} while (status == XBufferOverflow);
		
		
	} 		

	printf("keysym_keycode is %i (%s)\n",keysym_keycode,XKeysymToString(keysym_keycode));
	printf("keysym_unicode is %i (%s)\n",keysym_unicode,XKeysymToString(keysym_unicode));

	/* Phase 2, obtain a pigui keycode from the keysym */

	unsigned int keycode = KeyMappingX11::get_keycode(keysym_keycode);
	
	/* Phase 3, obtain an unicode character from the keysym */
	
	unsigned int unicode = KeyMappingX11::get_unicode_from_keysym(keysym_unicode);
	
	printf("unicode is %i, keycode %i\n",unicode,keycode);

	
}

void WindowX11::process_x11_event(const XEvent& p_event) {

	switch(p_event.type) {

		case KeyPress:
		case KeyRelease: {

			handle_key_event( (XKeyEvent*)&p_event );
			
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

			XMappingEvent *e = (XMappingEvent *)&p_event;
			XRefreshKeyboardMapping(e);
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

WindowX11::WindowX11( PlatformX11 *p_platform,Display *p_x11_display,::Window p_x11_window, WindowX11 * p_next ) : PlatformWindow( p_platform ) {

	x11_window=p_x11_window;
	x11_display=p_x11_display;
	platform_x11=p_platform;

	next=p_next;
	visible=true;

	unsigned long events=0;
	/*
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
	events|=KeymapStateMask;
	events|=OwnerGrabButtonMask;
	*/
	events|=KeyPressMask | KeyReleaseMask | ButtonPressMask |
			   ButtonReleaseMask | EnterWindowMask |
			   LeaveWindowMask | PointerMotionMask | 
			   Button1MotionMask |
			   Button2MotionMask | Button3MotionMask |
			   Button4MotionMask | Button5MotionMask |
			   ButtonMotionMask | KeymapStateMask |
			   ExposureMask | VisibilityChangeMask | 
			   StructureNotifyMask | /* ResizeRedirectMask | */
			   SubstructureNotifyMask | SubstructureRedirectMask |
			   FocusChangeMask | PropertyChangeMask |
			   ColormapChangeMask | OwnerGrabButtonMask;

	XSelectInput(x11_display, x11_window, events);
	XMapWindow(x11_display, x11_window);
	
	/* Set XIC */
	
	if (p_platform->get_xim() && p_platform->get_xim_style()) {
	
		xic = XCreateIC (p_platform->get_xim(),XNInputStyle, p_platform->get_xim_style(),XNClientWindow,x11_window,XNFocusWindow, x11_window, NULL);
	} else {
	
		xic=NULL;
	}
	
	
	x11_gc = XCreateGC(x11_display, x11_window, 0, 0);
	Visual* visual = DefaultVisual( x11_display, 0 );

	XRenderPictFormat * fmt = XRenderFindVisualFormat(x11_display, visual);
	XRenderPictureAttributes att;
	att.component_alpha = 1;
	att.repeat = 0;
	xrender_picture = XRenderCreatePicture(x11_display, x11_window, fmt, CPRepeat | CPComponentAlpha, &att);
	
	xft_draw = XftDrawCreate( x11_display, x11_window, visual, DefaultColormap( x11_display, DefaultScreen( x11_display ) ) );


	// calculate r_shift, 
	_configure_shift_and_mask(r_shift,r_mask,visual->red_mask);
	_configure_shift_and_mask(g_shift,g_mask,visual->green_mask);
	_configure_shift_and_mask(b_shift,b_mask,visual->blue_mask);
	
	_xmbstring=0;
	_xmblen=0;

}


WindowX11::~WindowX11() {

	if (_xmbstring)
		free(_xmbstring);
}


}

#endif
