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


class WindowX11Private {
public:
	// x11

	::Window x11_window; // this is the actuall X11 window
	Display *x11_display;
	::Picture xrender_picture;
	XftDraw *xft_draw;
	::XIC xic;
	WindowX11 * next;
	PlatformX11 *platform_x11;
	GC x11_gc;
	unsigned long last_timestamp;
	
	Atom wm_delete;
	
	WindowX11 *parent;
	
	struct ModalStack {
	
		WindowX11 *window;
		ModalStack *next;
		ModalStack() { window=NULL; next=NULL; }
	};
	
	ModalStack *modal_stack;
		
	struct PopupStack {
	
		WindowX11 *window;
		PopupStack *next;
		PopupStack() { window=NULL; next=NULL; }
	};
	
	PopupStack *popup_stack;
		
	/* XIM */
	char *xmbstring;
	int xmblen;
	::Time last_keyrelease_time;
    	
    	/* MOUSE EVENTS */
    	
    	Point last_mouse_pos;
    	bool last_mouse_pos_valid;
    	
    	/* MISC X Info */
    	
    	bool window_states[WINDOW_STATE_MAX];
    	
    	bool occluded;
    	bool mapped;
    	Rect rect;
    	
	unsigned long r_mask;	
	unsigned long g_mask;	
	unsigned long b_mask;	
	int r_shift,g_shift,b_shift;
    	
};


void WindowX11::set_title(String p_title) {


}

String WindowX11::get_title() const {

	return "";
}

void WindowX11::set_pos(const Point& p_pos) {

}
Point WindowX11::get_pos() const {

	return wp->rect.pos;
}

void WindowX11::set_size(const Point& p_pos) {

}
Size WindowX11::get_size() const {

	return wp->rect.size;
}

void WindowX11::set_icon(const Pixmap& p_icon) {

}

void WindowX11::set_state(WindowState p_state,bool p_enabled) {

	if (p_state<0 || p_state>=WINDOW_STATE_MAX)
		return;

	switch(p_state) { 
		case WINDOW_STATE_CAN_CLOSE: {
			// just handled by the flag
		} break;
		case WINDOW_STATE_RESIZABLE: {
		
		
		} break;
		case WINDOW_STATE_MODAL: {
		
			// popup and modal can't coexist
			if ( p_enabled && wp->window_states[WINDOW_STATE_POPUP] )
				set_state( WINDOW_STATE_POPUP, false );
		
			if (p_enabled) {
				set_modal();
				
			} else {
				remove_modal();
			}
			AtomsX11::set_netwm_single_state( wp->x11_window, "_NET_WM_STATE_MODAL",p_state);
			
					
		} break;
		case WINDOW_STATE_POPUP: {
		
			// popup and modal can't coexist
			if ( p_enabled && wp->window_states[WINDOW_STATE_MODAL] )
				set_state( WINDOW_STATE_MODAL, false );
			
			if (p_enabled)
				set_popup();
			else
				remove_popup();
		
			AtomsX11::set_netwm_single_state( wp->x11_window, "_NET_WM_STATE_POPUP",p_state);
		
		} break;
		case WINDOW_STATE_VISIBLE: {
		
			if (p_enabled == wp->mapped)
				break; // avoid unnecesary events
				
			if (p_enabled) {
				XMapWindow(wp->x11_display,wp->x11_window);
				printf("mapping window\n");
			} else {
				XUnmapWindow(wp->x11_display,wp->x11_window);
			}
			wp->mapped=p_enabled;
		
		} break;
		case WINDOW_STATE_BORDERLESS: {
			
			XSetWindowAttributes xwa;
			xwa.override_redirect=p_enabled?1:0;
			XChangeWindowAttributes(wp->x11_display, wp->x11_window, CWOverrideRedirect,&xwa);
		} break;
		case WINDOW_STATE_SHADED: {
		
			AtomsX11::set_netwm_single_state( wp->x11_window, "_NET_WM_STATE_SHADED",p_state);
		
		} break;
		case WINDOW_STATE_SKIP_TASKBAR: {
		
			AtomsX11::set_netwm_single_state( wp->x11_window, "_NET_WM_STATE_SKIP_TASKBAR",p_state);
		
		} break;
		case WINDOW_STATE_MAX: break;
	}

	XFlush(wp->x11_display); // depending when called, may need to flush
	
	wp->window_states[p_state]=p_enabled;

}
bool WindowX11::get_state(WindowState p_state) {


	return true;
}

unsigned long WindowX11::_map_color(const Color& p_color) {
	
	unsigned long col_r = p_color.r;
	if (wp->r_shift>0)
		col_r<<=wp->r_shift;
	else if (wp->r_shift<0)
		col_r>>=-wp->r_shift;

	unsigned long col_g = p_color.g;
	if (wp->g_shift>0)
		col_g<<=wp->g_shift;
	else if (wp->g_shift<0)
		col_g>>=-wp->g_shift;

	unsigned long col_b = p_color.b;
	if (wp->b_shift>0)
		col_b<<=wp->b_shift;
	else if (wp->b_shift<0)
		col_b>>=-wp->b_shift;
			
	return (col_r|col_g|col_b);

}

void WindowX11::draw_rect(const Point& p_from,const Size& p_size,const Color& p_color,bool p_fill) {

	XRenderColor xrcolor;
	xrcolor.red=(unsigned short)p_color.r << 8;
	xrcolor.green=(unsigned short)p_color.g << 8;
	xrcolor.blue=(unsigned short)p_color.b << 8;
	xrcolor.alpha=(unsigned short)p_color.a << 8;

	XRenderFillRectangle( wp->x11_display, PictOpOver, wp->xrender_picture, &xrcolor, p_from.x,p_from.y,p_size.width,p_size.height );

}

void WindowX11::draw_line(const Point& p_from,const Point& p_to,const Color& p_color,int p_width,bool p_round_endpoints) {

	XGCValues xgcvalues;
	xgcvalues.line_width=p_width;
	xgcvalues.cap_style=p_round_endpoints?CapRound:CapButt;
	xgcvalues.foreground=_map_color(p_color);

	XChangeGC(wp->x11_display, wp->x11_gc, GCForeground|GCCapStyle|GCLineWidth, &xgcvalues);

	XDrawLine(wp->x11_display, wp->x11_window, wp->x11_gc, p_from.x,p_from.y, p_to.x, p_to.y );
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
	
		XChangeGC(wp->x11_display, wp->x11_gc, GCForeground|GCBackground|GCFillStyle, &xgcvalues);
	
		XFillPolygon(wp->x11_display, wp->x11_window, wp->x11_gc, xpoints, p_point_count,Complex,CoordModeOrigin);
	} else {
	
		XGCValues xgcvalues;
		xgcvalues.line_width=p_line_width;
		xgcvalues.cap_style=CapRound;
		xgcvalues.foreground=_map_color(p_color);
	
		XChangeGC(wp->x11_display, wp->x11_gc, GCForeground|GCCapStyle|GCLineWidth, &xgcvalues);
	
		for (int i=0;i<p_point_count;i++) {
		
			int prev_i=(i==0)?p_point_count-1:i-1;
			
			
			
			XDrawLine(wp->x11_display, wp->x11_window, wp->x11_gc, p_points[prev_i].x,p_points[prev_i].y,p_points[i].x, p_points[i].y );
		}
	}
	
}

void WindowX11::draw_circle(const Point p_center, int p_radius,const Color& p_color,bool p_fill) {

}
void WindowX11::draw_arrow(const Point& p_from,const Size& p_size,Direction p_dir,const Color& p_color,bool p_fill) {

}
	
void WindowX11::draw_pixmap(const Pixmap& p_pixmap, const Point& p_pos,const Rect& p_source) {

	PixmapX11 *pp = wp->platform_x11->extract_pixmap_x11(p_pixmap);

	if (!pp || !pp->is_valid())
		return;

	XRenderComposite (wp->x11_display,PictOpOver,pp->get_xrender_picture(),0,wp->xrender_picture,0,0,0,0,p_pos.x,p_pos.y,pp->get_size().width,pp->get_size().height );
	
}
void WindowX11::draw_string(const Font& p_font,const Point& p_pos,const Color& p_color,const String& p_string,Direction p_dir) {

	FontX11 *fnt = wp->platform_x11->extract_font_x11( p_font );
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
			
			XftDrawString8( wp->xft_draw, &xft_color, xft_font, p_pos.x, p_pos.y, (const FcChar8*)p_string.c_str(), p_string.length() );
			
		} break;
		case 2: {
			
			XftDrawString16( wp->xft_draw, &xft_color, xft_font, p_pos.x, p_pos.y, (const FcChar16*)p_string.c_str(), p_string.length() );
		} break;
		case 4: {
			
			XftDrawString32( wp->xft_draw, &xft_color, xft_font, p_pos.x, p_pos.y, (const FcChar32*)p_string.c_str(), p_string.length() );
		} break;
	}
	
}

void WindowX11::draw_stylebox( const StyleBox& p_style,const Point& p_from,const Size& p_size) {

}

void WindowX11::draw_set_clipping(bool p_enabled,const Rect& p_rect) {

}

void WindowX11::add_dirty_region(const Rect& p_rect) {
/* @TODO
	[14:27] <daniels> the problem is that if you're composited and hidden behind that window, then X tells you that your whole window is invisible
	[14:27] <daniels> but what if the window in front is transparent?
	
	I need to either figure out a way to fix this, or wait until XComposite works properly
	-juan
*/
	XClearArea(wp->x11_display, wp->x11_window, p_rect.pos.x, p_rect.pos.y, p_rect.size.width, p_rect.size.height, True);
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

	if (!wp->xmbstring) {
		// keep a temporary buffer for the string
		wp->xmbstring=(char*)malloc(sizeof(char)*8);
		wp->xmblen=8;
	}			 
	
	if (xkeyevent->type == KeyPress && wp->xic) {

		Status status;
		do {
			
			int mnbytes = XmbLookupString (wp->xic, xkeyevent, wp->xmbstring, wp->xmblen - 1, &keysym_unicode, &status);
			wp->xmbstring[mnbytes] = '\0';

			if (status == XBufferOverflow) {
				wp->xmblen = mnbytes + 1;
				wp->xmbstring = (char*)realloc (wp->xmbstring, wp->xmblen);
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
	
	if (xkeyevent->type == KeyPress && wp->xic) {
                if (XFilterEvent((XEvent*)xkeyevent, wp->x11_window))
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
		if (xkeyevent->time==wp->last_keyrelease_time)
			echo=true;

	} else {
	
		// make sure there are events pending,
		// so this call won't block.
		if (XPending(wp->x11_display)>0) {
			XEvent peek_event;
			XPeekEvent(wp->x11_display, &peek_event);
			
			// I'm using a treshold of 5 msecs, 
			// since sometimes there seems to be a little
			// jitter. I'm still not convinced that all this approach
			// is correct, but the xorg developers are
			// not very helpful today.
			
			Time tresh=abs(peek_event.xkey.time-xkeyevent->time);
			if (peek_event.type == KeyPress && tresh<5 )
				echo=true;
				
			// use the time from peek_event so it always works
			wp->last_keyrelease_time=peek_event.xkey.time;
		} else {
			wp->last_keyrelease_time=xkeyevent->time;
		}
	
		// save the time to check for echo when keypress happens
		
	}
	
	
	/* Phase 7, send event to Window */
	
	call("key",unicode,keycode,keypress,echo,mask);
	
}

void WindowX11::set_modal() {

	if (!wp->parent)
		return;
		
	/* check we are there */
	
	WindowX11Private::ModalStack *ms=wp->parent->wp->modal_stack;
	
	XSetTransientForHint(wp->x11_display, wp->x11_window, wp->parent->wp->x11_window);
	
	while(ms) {
	
		if (ms->window==this)
			return; // there already
		
		ms=ms->next;
	}
	
	ms = GUI_NEW( WindowX11Private::ModalStack );
	ms->window=this;
	ms->next=wp->parent->wp->modal_stack;
	wp->parent->wp->modal_stack=ms;
		
		
}
void WindowX11::remove_modal() {

	/* check we are there */
	
	if (!wp->parent)
		return;
	
	WindowX11Private::ModalStack *ms=wp->parent->wp->modal_stack,*prev=NULL;
	
	XSetTransientForHint(wp->x11_display, wp->x11_window, 0);
		
	while(ms) {
	
		if (ms->window==this) {
		
			if (prev) {
				
				prev->next=ms->next;
			} else {
			
				wp->parent->wp->modal_stack=ms->next;
			}
			
			GUI_DELETE(ms);
			return;
		}
		prev=ms;
		ms=ms->next;		
	}
}


void WindowX11::set_popup() {

	if (!wp->parent)
		return;
		
	/* check we are there */
	
	XSetTransientForHint(wp->x11_display, wp->x11_window, wp->parent->wp->x11_window);
	
	WindowX11Private::PopupStack *ms=wp->parent->wp->popup_stack;
	
	while(ms) {
	
		if (ms->window==this)
			return; // there already
		
		ms=ms->next;
	}
	
	ms = GUI_NEW( WindowX11Private::PopupStack );
	ms->window=this;
	ms->next=wp->parent->wp->popup_stack;
	wp->parent->wp->popup_stack=ms;
	
	if (wp->parent) 
		AtomsX11::request_active_window( wp->parent->get_x11_window(),get_x11_window(),  wp->last_timestamp);
					
		
}
void WindowX11::remove_popup() {

	if (!wp->parent)
		return;

	/* check we are there */
	
	WindowX11Private::PopupStack *ms=wp->parent->wp->popup_stack,*prev=NULL;
	XSetTransientForHint(wp->x11_display, wp->x11_window, 0);
	
	while(ms) {
	
		if (ms->window==this) {
		
			if (prev) {
				
				prev->next=ms->next;
			} else {
			
				wp->parent->wp->popup_stack=ms->next;
			}
			
			GUI_DELETE(ms);
			return;
		}
		prev=ms;
		ms=ms->next;		
	}
}

void WindowX11::check_hide_popup_stack() {

	while (wp->popup_stack) {
	
		WindowX11 *window =wp->popup_stack->window;
		window->set_state( WINDOW_STATE_VISIBLE, false );
		window->remove_popup();
	}
}

bool WindowX11::redirect_focus(bool p_force_redirect) {

	// I have no idea how to do this in X11
	// it should refocus the modal window..
	
	if (wp->modal_stack) {
	
		if (p_force_redirect)
		
			AtomsX11::request_active_window( wp->x11_window,wp->modal_stack->window->get_x11_window(),  wp->last_timestamp);
		
		return true;
	} else {
	
		return false;
	}

}

void WindowX11::process_x11_event(XEvent* p_event) {
	
	switch(p_event->type) {

		case KeyPress:
		case KeyRelease: {

			wp->last_timestamp=p_event->xkey.time;
			
			check_hide_popup_stack();
			if (redirect_focus(true))
				break;				
				
			// key event is a little complex, so
			// it will be handled in it's own function.
			handle_key_event( (XKeyEvent*)p_event );
			
		} break;
		case ButtonPress:
		case ButtonRelease: {
						
			
			wp->last_timestamp=p_event->xbutton.time;
			check_hide_popup_stack();
			if (redirect_focus(true))
				break;
						
			// ButtonPress and ButtonRelease are pretty
			// standard. No changes need to be made.
			
			Point pos( p_event->xbutton.x, p_event->xbutton.y );
			int button=p_event->xbutton.button;
			bool pressed=(p_event->type==ButtonPress);
			
			unsigned int mask=fill_modifier_button_mask(p_event->xbutton.state);
;
			call("mouse_button",pos,button,pressed,mask);
			
			printf("mouse %s, time is %i\n",pressed?"press":"release",p_event->xbutton.time);
		} break;
		case MotionNotify: {

			wp->last_timestamp=p_event->xmotion.time;
			if (redirect_focus(false))
				break;

			// Motion is also simple.
			// A little hack is in order
			// to be able to send relative motion events.
			
			Point pos( p_event->xmotion.x, p_event->xmotion.y );			

			if (!wp->last_mouse_pos_valid) {
			
				wp->last_mouse_pos=pos;
				wp->last_mouse_pos_valid=true;
			}
			
			Point rel = pos - wp->last_mouse_pos;
			unsigned int mask=fill_modifier_button_mask(p_event->xmotion.state);
			
			wp->last_mouse_pos=pos;
			
			call("mouse_motion",pos,rel,mask);

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
				wp->last_mouse_pos_valid=false;
				call("mouse_enter_window");
			} else {
				call("mouse_exit_window");
			}

		} break;
		case FocusIn: {
	
			if (redirect_focus(true))
				break;
			
			//check_hide_popup_stack();
			call("focus_enter");	
			
		} break;
		case FocusOut: {

			call("focus_exit");	
		} break;
		case KeymapNotify: {

			// I never gt this event.. i have no idea what
			// to do with it.
			//printf("key map\n");
		} break;
		case Expose: {

			// should stack them, but fro now..
			Rect expose_rect( Point( p_event->xexpose.x, p_event->xexpose.y), Size(p_event->xexpose.width, p_event->xexpose.height ));

			call("update",expose_rect);

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

			wp->occluded=p_event->xvisibility.state==VisibilityFullyObscured;

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
			wp->mapped=false;
			if (wp->window_states[WINDOW_STATE_MODAL])
				remove_modal();
				
			check_hide_popup_stack();
			if (wp->window_states[WINDOW_STATE_POPUP])
				remove_popup();
		} break;
		case MapNotify: {

			wp->mapped=true;
				// most false by default
			for (int i=0;i<WINDOW_STATE_MAX;i++) {
			
				set_state(WindowState(i),wp->window_states[i]);
			}

			if (wp->window_states[WINDOW_STATE_MODAL])
				set_modal();

			if (wp->window_states[WINDOW_STATE_POPUP])
				set_popup();

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
			
			bool changed = (new_rect!=wp->rect);
			wp->rect=new_rect;
			if (changed)
				call("rect_change",new_rect);
			
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
			char * name = XGetAtomName(wp->x11_display,p_event->xproperty.atom);
			
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

			if ((int)p_event->xclient.data.l[0] == (int)wp->wm_delete) {
			
				call("close_request");
				if (wp->window_states[WINDOW_STATE_CAN_CLOSE]) {
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

WindowX11 *WindowX11::get_next() {

	return wp->next;
}
Window WindowX11::get_x11_window() {

	return wp->x11_window;
}


WindowX11::WindowX11( PlatformX11 *p_platform,Display *p_x11_display,::Window p_x11_window, WindowX11 * p_next,WindowX11 *p_parent ) : PlatformWindow( p_platform ) {

	wp = GUI_NEW( WindowX11Private );

	wp->parent=p_parent;
	wp->x11_window=p_x11_window;
	wp->x11_display=p_x11_display;
	wp->platform_x11=p_platform;

	wp->next=p_next;


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

	XSelectInput(wp->x11_display, wp->x11_window, events);
	//XMapWindow(wp->x11_display, wp->x11_window);
	
	/* Set wp->xic */
	
	if (p_platform->get_xim() && p_platform->get_xim_style()) {
	
		wp->xic = XCreateIC (p_platform->get_xim(),XNInputStyle, p_platform->get_xim_style(),XNClientWindow,wp->x11_window,XNFocusWindow, wp->x11_window, NULL);
	} else {
	
		wp->xic=NULL;
	}
	
	
	wp->x11_gc = XCreateGC(wp->x11_display, wp->x11_window, 0, 0);
	Visual* visual = DefaultVisual( wp->x11_display, 0 );

	XRenderPictFormat * fmt = XRenderFindVisualFormat(wp->x11_display, visual);
	XRenderPictureAttributes att;
	att.component_alpha = 1;
	att.repeat = 0;
	wp->xrender_picture = XRenderCreatePicture(wp->x11_display, wp->x11_window, fmt, CPRepeat | CPComponentAlpha, &att);
	
	wp->xft_draw = XftDrawCreate( wp->x11_display, wp->x11_window, visual, DefaultColormap( wp->x11_display, DefaultScreen( wp->x11_display ) ) );
   
	wp->wm_delete = XInternAtom(wp->x11_display, "WM_DELETE_WINDOW", false);
	XSetWMProtocols(wp->x11_display, wp->x11_window, &wp->wm_delete, 1);


	// calculate r_shift, 
	_configure_shift_and_mask(wp->r_shift,wp->r_mask,visual->red_mask);
	_configure_shift_and_mask(wp->g_shift,wp->g_mask,visual->green_mask);
	_configure_shift_and_mask(wp->b_shift,wp->b_mask,visual->blue_mask);
	
	wp->xmbstring=0;
	wp->xmblen=0;
	wp->last_keyrelease_time=0;
	wp->last_mouse_pos_valid=false;
	wp->occluded=false;
	wp->mapped=false;
	wp->modal_stack=NULL;
	wp->popup_stack=NULL;
	// most false by default
	for (int i=0;i<WINDOW_STATE_MAX;i++) {
	
		wp->window_states[i]=false;
	}
	
	wp->window_states[WINDOW_STATE_VISIBLE]=true;
	wp->window_states[WINDOW_STATE_CAN_CLOSE]=true;
	wp->last_timestamp=0;
}


WindowX11::~WindowX11() {

	if (wp->xic)
		XDestroyIC(wp->xic);
		
	if (wp->xrender_picture)
		XRenderFreePicture( wp->x11_display, wp->xrender_picture );
		
	if (wp->xft_draw)
		XftDrawDestroy(wp->xft_draw);
	
	if (wp->x11_gc)
		XFreeGC(wp->x11_display,wp->x11_gc);
		
	if (wp->xmbstring)
		free(wp->xmbstring);
		
	if (wp->x11_window)
		XDestroyWindow( wp->x11_display, wp->x11_window );
		
	while (wp->modal_stack) {
	
		WindowX11Private::ModalStack *ms = wp->modal_stack;
		wp->modal_stack=wp->modal_stack->next;
		GUI_DELETE( ms );
	}
		
	while (wp->popup_stack) {
	
		WindowX11Private::PopupStack *ms = wp->popup_stack;
		wp->popup_stack=wp->popup_stack->next;
		GUI_DELETE( ms );
	}
		
	GUI_DELETE( wp );
}


}

#endif
