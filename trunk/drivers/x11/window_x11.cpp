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
#include "drivers/x11/atoms_x11.h"
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

void WindowX11::set_icon(const Pixmap& p_icon) {

}

void WindowX11::set_state(WindowState p_state,bool p_enabled) {

	if (p_state<0 || p_state>=WINDOW_STATE_MAX)
		return;

	
	if (!mapped) {
		window_states[p_state]=p_enabled;
	
		return;
	}
		

	switch(p_state) { 
		case WINDOW_STATE_CAN_CLOSE: {
		
		} break;
		case WINDOW_STATE_RESIZABLE: {
		
		
		} break;
		case WINDOW_STATE_MODAL: {
		
			if (parent) {
			
				if (p_enabled) 
					XSetTransientForHint(x11_display, x11_window, parent->x11_window);
				else
					XSetTransientForHint(x11_display, x11_window, 0);
				
			}
			AtomsX11::set_netwm_single_state( x11_window, "_NET_WM_STATE_MODAL",p_state);
			
#if 0			
			if (p_enabled) {
			
				if (transient_for!=0)
					break; // already transient for someone
					
				transient_for=parent->x11_window; // attempt to be transient for the parent
				
				while(true) {
				
					::Window for_redirecting=0;
					
					XGetTransientForHint(x11_display, transient_for, &for_redirecting);
					
					if (for_redirecting!=0) {
						// transient_for is redirecting somewhere, capture that!
						if (for_redirecting==x11_window) {
							// make sure we aren't in the middle of the mess
							transient_for=0;
							for_redirecting=0;
						} else {
							transient_for=for_redirecting;
						}
					}
					
					if (for_redirecting==0)
						break;
				}
				
				if (transient_for!=0) {
				
					XSetTransientForHint(x11_display, transient_for, x11_window);
				}
			} else {
			
				if (transient_for!=0) {
				
					XSetTransientForHint(x11_display, transient_for, 0);
					transient_for=0;
				}
				
			}
#endif			
		} break;
		case WINDOW_STATE_POPUP: {
		
		
		} break;
		case WINDOW_STATE_VISIBLE: {
		
			if (p_enabled == mapped)
				break; // avoid unnecesary events
				
			if (p_enabled)
				XMapWindow(x11_display,x11_window);
			else
				XUnmapWindow(x11_display,x11_window);
			mapped=p_enabled;
		
		} break;
		case WINDOW_STATE_BORDERLESS: {
			
			XSetWindowAttributes xwa;
			xwa.override_redirect=p_enabled?1:0;
			XChangeWindowAttributes(x11_display, x11_window, CWOverrideRedirect,&xwa);
		} break;
		case WINDOW_STATE_SHADED: {
		
			AtomsX11::set_netwm_single_state( x11_window, "_NET_WM_STATE_SHADED",p_state);
		
		} break;
		case WINDOW_STATE_SKIP_TASKBAR: {
		
		
		} break;
		case WINDOW_STATE_MAX: break;
	}

	XFlush(x11_display); // depending when called, may need to flush
	
	window_states[p_state]=p_enabled;

}
bool WindowX11::get_state(WindowState p_state) {


	return true;
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

unsigned int WindowX11::fill_modifier_button_mask(unsigned int p_x11_state) {

	unsigned int mask=0;

	if (p_x11_state&ShiftMask)
		mask|=KEY_MASK_SHIFT;
	
	if (p_x11_state&ControlMask)
		mask|=KEY_MASK_CTRL;

	if (p_x11_state&Mod1Mask || p_x11_state&Mod5Mask)
		mask|=KEY_MASK_ALT;	
		
	if (p_x11_state&Mod4Mask)
		mask|=KEY_MASK_META;

	if (p_x11_state&Button1Mask) {
		
		mask|=BUTTON_MASK_LEFT;
	}
	
	if (p_x11_state&Button3Mask) {
		
		mask|=BUTTON_MASK_RIGHT;
	}
	
	if (p_x11_state&Button4Mask) {
		
		mask|=BUTTON_MASK_WHEEL_UP;
	}
	
	if (p_x11_state&Button5Mask) {
		
		mask|=BUTTON_MASK_WHEEL_DOWN;
	}
	
	return mask;
}

void WindowX11::handle_key_event(XKeyEvent *p_event) {

			
	// X11 functions don't know what const is
	XKeyEvent *xkeyevent = p_event;
	
	// This code was pretty difficult to write.
	// The docs stink and every toolkit seems to
	// do it in a different way. 
	
	/* Phase 1, obtain a proper keysym */
	
	// This was also very difficult to figure out.
	// You'd expect you could just use Keysym provided by
	// XKeycodeToKeysym to obtain internationalized 
	// input.. WRONG!! 
	// you must use XLookupString (???) which not only wastes
	// cycles generating an unnecesary string, but also
	// still works in half the cases. (won't handle deadkeys)
	// For more complex input methods (deadkeys and more advanced)
	// you have to use XmbLookupString (??).
	// So.. then you have to chosse which of both results
	// you want to keep.
	// This is a real bizarreness and cpu waster.
		
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
			} 
		} while (status == XBufferOverflow);
	} 		

	/* Phase 2, obtain a pigui keycode from the keysym */
	
	// KeyMappingX11 just translated the X11 keysym to a PIGUI
	// keysym, so it works in all platforms the same.
	
	unsigned int keycode = KeyMappingX11::get_keycode(keysym_keycode);
	
	/* Phase 3, obtain an unicode character from the keysym */
	
	// KeyMappingX11 also translates keysym to unicode.
	// It does a binary search on a table to translate
	// most properly. 
	
	unsigned int unicode = KeyMappingX11::get_unicode_from_keysym(keysym_unicode);
	
	
	/* Phase 4, determine if event must be filtered */
	
	// This seems to be a side-effect of using XIM.
	// XEventFilter looks like a core X11 funciton,
	// but it's actually just used to see if we must
	// ignore a deadkey, or events XIM determines
	// must not reach the actual toolkit.
	// Guess it was a design problem of the extension/
	
	bool keypress = xkeyevent->type == KeyPress;
	
	if (xkeyevent->type == KeyPress && xic) {
                if (XFilterEvent((XEvent*)xkeyevent, x11_window))
                	return;  
	}
	
	if (keycode==0 && unicode==0)
		return;
		
	/* Phase 5, determine modifier mask */
		
	// No problems here, except I had no way to
	// know Mod1 was ALT and Mod4 was META (applekey/winkey)
	// just tried Mods until i found them.
	
	unsigned int mask=fill_modifier_button_mask(xkeyevent->state);	
	
	/* Phase 6, determine echo character */
	
	// Echo characters in X11 are a keyrelease and a keypress
	// one after the other with the (almot) same timestamp.
	// To detect them, i use XPeekEvent and check that their
	// difference in time is below a treshold.
	
	bool echo=false;
	
	if (xkeyevent->type == KeyPress) {
			
		// saved the time of the last keyrelease to see
		// if it's the same as this keypress.
		if (xkeyevent->time==last_keyrelease_time)
			echo=true;

	} else {
	
		// make sure there are events pending,
		// so this call won't block.
		if (XPending(x11_display)>0) {
			XEvent peek_event;
			XPeekEvent(x11_display, &peek_event);
			
			// I'm using a treshold of 5 msecs, 
			// since sometimes there seems to be a little
			// jitter. I'm still not convinced that all this approach
			// is correct, but the xorg developers are
			// not very helpful today.
			
			Time tresh=abs(peek_event.xkey.time-xkeyevent->time);
			if (peek_event.type == KeyPress && tresh<5 )
				echo=true;
				
			// use the time from peek_event so it always works
			last_keyrelease_time=peek_event.xkey.time;
		} else {
			last_keyrelease_time=xkeyevent->time;
		}
	
		// save the time to check for echo when keypress happens
		
	}
	
	
	/* Phase 7, send event to Window */
	
	key_event_signal.call( unicode, keycode, keypress, echo, mask );
	
}

bool WindowX11::redirect_focus() {

	// since X11 doesn't really support "MODAL" Windows
	
	return false;
	
	::Window redirect_to=x11_window;
	
	while(true) {
	
		::Window dst=0;
		
		XGetTransientForHint(x11_display, redirect_to, &dst);
		
		if (dst!=0) {
		
			redirect_to=dst;
			continue;
		} else {
		
			break;
		}
	}
	
	
	if (redirect_to==x11_window)
		return false; //nothing happened
	
	AtomsX11::request_active_window( x11_window, redirect_to );

	return true;

}

void WindowX11::process_x11_event(XEvent* p_event) {

	switch(p_event->type) {

		case KeyPress:
		case KeyRelease: {

			if (redirect_focus())
				break;
				
			// key event is a little complex, so
			// it will be handled in it's own function.
			handle_key_event( (XKeyEvent*)p_event );
			
		} break;
		case ButtonPress:
		case ButtonRelease: {
						
			if (redirect_focus())
				break;
						
			// ButtonPress and ButtonRelease are pretty
			// standard. No changes need to be made.
			
			Point pos( p_event->xbutton.x, p_event->xbutton.y );
			int button=p_event->xbutton.button;
			bool pressed=(p_event->type==ButtonPress);
			
			unsigned int mask=fill_modifier_button_mask(p_event->xbutton.state);
;
			mouse_button_event_signal.call(pos,button,pressed,mask);
			
			printf("mouse %s, time is %i\n",pressed?"press":"release",p_event->xbutton.time);
		} break;
		case MotionNotify: {

			// Motion is also simple.
			// A little hack is in order
			// to be able to send relative motion events.
			
			Point pos( p_event->xmotion.x, p_event->xmotion.y );			

			if (!last_mouse_pos_valid) {
			
				last_mouse_pos=pos;
				last_mouse_pos_valid=true;
			}
			
			Point rel = pos - last_mouse_pos;
			unsigned int mask=fill_modifier_button_mask(p_event->xmotion.state);
			
			last_mouse_pos=pos;
			
			mouse_motion_event_signal.call(pos,rel,mask);

		} break;
		case EnterNotify:
		case LeaveNotify: {
	
			// if any of the buttons is being pressed,
			// just ignore the enter/leave event,
			// because this means something is being
			// grabbed.
			if (	p_event->xcrossing.state&
				(Button1Mask|Button2Mask|Button3Mask|
				 Button4Mask|Button5Mask) )
				 	break;
				 	
			if (p_event->type==EnterNotify) {
				// invalidate previous motion
				last_mouse_pos_valid=false;
				mouse_entered_window_signal.call();
			} else {
				mouse_left_window_signal.call();
			}

		} break;
		case FocusIn: {
	
			gained_focus_signal.call();	
		} break;
		case FocusOut: {

			lost_focus_signal.call();
		} break;
		case KeymapNotify: {

			// I never gt this event.. i have no idea what
			// to do with it.
			//printf("key map\n");
		} break;
		case Expose: {

			// should stack them, but fro now..
			Rect expose_rect( Point( p_event->xexpose.x, p_event->xexpose.y), Size(p_event->xexpose.width, p_event->xexpose.height ));

			update_event_signal.call( expose_rect );

		} break;
		case GraphicsExpose: {

			// I also have no idea what to do with this.
			//printf("graphics expose\n");

		} break;
		case NoExpose: {
			// Fine, no expose (??)
			//printf("no expose\n");

		} break;
		case VisibilityNotify: {

			occluded=p_event->xvisibility.state==VisibilityFullyObscured;

		} break;
		case CreateNotify: {
			// I know i was created. Else i wouldn't be here.
			//printf("create notify\n");

		} break;
		case DestroyNotify: {
			// Seems this was killed.
			// printf("destroy notify\n");

		} break;
		case UnmapNotify: {

			//printf("unmap notify\n");
			mapped=false;
		} break;
		case MapNotify: {

			mapped=true;
				// most false by default
			for (int i=0;i<WINDOW_STATE_MAX;i++) {
			
				set_state(WindowState(i),window_states[i]);
			}

			printf("map notify\n");
		} break;
		case MapRequest: {
			// never got this.
			printf("map request\n");

		} break;
		case ReparentNotify: {
			// don't think i'll ever need this
			printf("reparent notify \n");

		} break;
		case ConfigureNotify: {

			/* Check wether window pos/size changed */

			Rect new_rect( 
				p_event->xconfigure.x,
				p_event->xconfigure.y,
				p_event->xconfigure.width,
				p_event->xconfigure.height );
			
			bool pos_changed=( new_rect.pos != rect.pos );
			bool size_changed=( new_rect.size != rect.size );
			
			rect=new_rect;
			
			if ( pos_changed )
				position_changed_signal.call( rect.pos );
			
			if ( size_changed )
				size_changed_signal.call( rect.size );
			
		} break;
		case ConfigureRequest: {

			//printf("configure request\n");

		} break;
		case GravityNotify: {
			
			//can't say i ever got this
			//printf("gravity notify\n");

		} break;
		case ResizeRequest: {
			//can't say i ever got this
			//printf("resize notify\n");

		} break;
		case CirculateNotify: {

			//can't say i ever got this
			//printf("circulate notify\n");

		} break;
		case CirculateRequest: {

			//can't say i ever got this
			//printf("circulate request\n");

		} break;
		case PropertyNotify: {

			
			//printf("property notify\n");
			char * name = XGetAtomName(x11_display,p_event->xproperty.atom);
			
			printf("been set property %s\n",name);
			XFree(name);
		} break;
		case SelectionClear: {

			//printf("selection clear notify\n");

		} break;
		case SelectionRequest: {

			//printf("selection request notify\n");

		} break;
		case SelectionNotify: {


			//printf("selection notify\n");
		} break;
		case ColormapNotify: {

			//printf("colormap notify\n");

		} break;
		case ClientMessage: {

			if ((int)p_event->xclient.data.l[0] == (int)wm_delete) {
			
				close_requested_signal.call();
				if (window_states[WINDOW_STATE_CAN_CLOSE]) {
					set_state(WINDOW_STATE_VISIBLE,false);
					// should check if all roots are hidden(closed?) and exit
					// the app
				}
			}
			
		} break;
		case MappingNotify: {

			//printf("mapping notify \n");
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

WindowX11::WindowX11( PlatformX11 *p_platform,Display *p_x11_display,::Window p_x11_window, WindowX11 * p_next,WindowX11 *p_parent ) : PlatformWindow( p_platform ) {

	parent=p_parent;
	x11_window=p_x11_window;
	x11_display=p_x11_display;
	platform_x11=p_platform;

	next=p_next;
	mapped=true;

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
   
	wm_delete = XInternAtom(x11_display, "WM_DELETE_WINDOW", false);
	XSetWMProtocols(x11_display, x11_window, &wm_delete, 1);


	// calculate r_shift, 
	_configure_shift_and_mask(r_shift,r_mask,visual->red_mask);
	_configure_shift_and_mask(g_shift,g_mask,visual->green_mask);
	_configure_shift_and_mask(b_shift,b_mask,visual->blue_mask);
	
	_xmbstring=0;
	_xmblen=0;
	last_keyrelease_time=0;
	last_mouse_pos_valid=false;
	occluded=false;
	mapped=false;
	modal_refcount=0;
	// most false by default
	for (int i=0;i<WINDOW_STATE_MAX;i++) {
	
		window_states[i]=false;
	}
	
	window_states[WINDOW_STATE_VISIBLE]=true;
	window_states[WINDOW_STATE_CAN_CLOSE]=true;
}


WindowX11::~WindowX11() {

	if (xic)
		XDestroyIC(xic);
		
	if (xrender_picture)
		XRenderFreePicture( x11_display, xrender_picture );
		
	if (xft_draw)
		XftDrawDestroy(xft_draw);
	
	if (x11_gc)
		XFreeGC(x11_display,x11_gc);
		
	if (_xmbstring)
		free(_xmbstring);
		
	if (x11_window)
		XDestroyWindow( x11_display, x11_window );
}


}

#endif
