//
// C++ Implementation: frame
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "frame.h"
#include "base/theme.h"
#include "base/window.h"
#include "base/container.h"

namespace GUI {

// todo, make faster
struct _ThemeOverride {

	int var;
	int with; 
	Variant with_t;

	_ThemeOverride *next;
	
	_ThemeOverride* get(int p_var) { // return -2 if nothing found, -1 if replacement
	
		if (p_var==var) {
			
			return this;
		}

		else if (next)
			return next->get( p_var);
		else
			return NULL;
	}
	
	_ThemeOverride(int p_var=-1,int p_with=-1,const Variant& p_t=Variant(),_ThemeOverride *p_next=0) { var=p_var; with=p_with; next=p_next; with_t=p_t; }
	~_ThemeOverride() { if (next) GUI_DELETE( next ); }
};

struct FramePrivate {

	String name;

	Container *parent;
	Window *window;
	Pixmap cursor;
	Point cursor_hotspot;
	String tooltip;

	HAlign h_align;
	VAlign v_align;

	FocusMode focus_mode;
	Size minimum_size;

	Point size_cache;
	Point pos_cache;

	_ThemeOverride* theme_overrides;

	Theme *theme;

	bool bg_on_updates;
	bool visible;

	FramePrivate() {

		parent=NULL;
		window=NULL;
		theme_overrides=NULL;
		h_align=HALIGN_FILL;
		v_align=VALIGN_FILL;
		focus_mode=FOCUS_NONE;
		bg_on_updates=true;
		visible=true;
		theme=NULL;
	}

};


void Frame::set_window(Window *p_window) {

	if (_fp->window && p_window!=NULL) {

		GUI_PRINT_ERROR("Frame already in window");
		return;
	}

	if (!_fp->window && !p_window) {

		return;		
	}
		
	_fp->window=p_window;
	if (p_window)
		window_enter_event();	
	else
		window_exit_event();

}


/**
 * Set the parent . This is only available to containers and windows
 * @param p_parent 
 */
void Frame::set_parent(Container *p_parent) {

	_fp->parent=p_parent;
}


void Frame::set_rect( const Rect& p_rect ) {

	_fp->pos_cache=p_rect.pos;
	_fp->size_cache=p_rect.size;
}

Size Frame::compute_minimum_size() const {

	return Size();
}
/**
 * This is a small optional optimization the user can provide. If the frame will cover its entire space, the background may not be draw behind it to save speed.
 * @param p_enabled enable bg on updates (true by default).
 */
void Frame::set_update_bg(bool p_enabled) {

	_fp->bg_on_updates=p_enabled;
} 


/* THEME CUSTOMIZATION */

/**
 * Replace a theme variable with another, all within the Theme.
 * @param p_id variable index
 * @param p_with_id replacement index
 */
void Frame::theme_replace(int p_id, int p_with_id) {

	theme_restore(p_id);
	_fp->theme_overrides = GUI_NEW( _ThemeOverride( p_id, p_with_id,Variant(),_fp->theme_overrides) );
}

/**
 * Override a theme variable by a custom user-provided value
 * @param p_id variable index
 * @param p_with replacement value
 */
void Frame::theme_override(int p_id, const Variant& p_variant) {

	theme_restore(p_id);
	_fp->theme_overrides = GUI_NEW( _ThemeOverride( p_id, -1,p_variant,_fp->theme_overrides) );
}

/**
 * Get a variable from the theme. 
 * if overrides are found, they are returned. Otherwise if the frame has a custom theme,
 * it is used. Then, the window theme is used, and finally the default one if nothing else was found.
 * @param p_index theme index id
 * @return variant
 */

const Variant& Frame::theme(int p_id) const {

	
	/* Check theme overrides */	
	if ( _fp->theme_overrides) {
		_ThemeOverride *to = _fp->theme_overrides->get( p_id );
		if (to) {
		
			if (to->with_t.get_type()!=Variant::NIL)
				return to->with_t;
			else 
				p_id=to->with;
		}
	}

	if (_fp->theme ) {

		return _fp->theme->get_var(p_id);

	} else if (_fp->window) {

		return _fp->window->get_theme()->get_var(p_id);
	} 

	static Variant empty;
	return empty;

}

void Frame::theme_restore(int p_id) {

	_ThemeOverride *to=_fp->theme_overrides, *prev=NULL;
	
	while(to) {
	
		if (to->var==p_id) {
		
			if (prev) {
			
				prev->next=to->next;
			} else {
			
				_fp->theme_overrides=to->next;
			}
			
			GUI_DELETE( to );
			return;
			
		}
	
		to=to->next;
	}
}


/* HINTING */

/**
 * Set the cursor that will appear by default when the mouse pointer moves inside this frame.
 * If get_cursor() is overrided, then this cursor is likely to be ignored.
 * @param p_pixmap 
 * @param p_hotspot 
 */
void Frame::set_cursor(Pixmap p_pixmap, const Point& p_hotspot) {

	_fp->cursor_hotspot=p_hotspot;
	_fp->cursor=p_pixmap;
}

/**
 * Get the cursor that is going to be shown when the mouse pointer enters this frame.
 * By default, a single cursor per frame can be set. More finegrained cursor reporting
 * can be achieved by overriding this method.
 * @param p_point place inside the frame
 * @param r_hotspot returnet hotspot for the cursor
 * @return Cursor pixmap, Pixmap() (empty) for the default cursor.
 */
Pixmap Frame::get_cursor(const Point& p_point,Point *r_hotspot) const {

	if (r_hotspot)
		*r_hotspot=_fp->cursor_hotspot;
	return _fp->cursor;
}


/**
 * Set the tooltip that will appear when the mouse pointer rests over this frame.
 * By default, one tooltip is shown for the entire frame area, but more finegrained 
 * control can be achieved by overriding this method.
 * @param p_text 
 */
void Frame::set_tooltip(String p_text) {

	_fp->tooltip=p_text;
}
/**
 * Get the toolip that will appear when the mouse pointer rests over this frame.
 * Overrinding set_tooltip() will invalidate this tooltip.
 * @param p_point 
 * @return 
 */
String Frame::get_tooltip(const Point& p_point) const {

	return _fp->tooltip;
}

/* UPDATE */

/**
 * BG On Updates.
 * Return true if the frame expects the background thee to be repainted below it (defaults to true)
 * @return true if yes, false if not
 */
bool Frame::has_update_bg() const {
	
	return _fp->bg_on_updates;
}

/**
 * Notify (as a user) that this frame has changed and needs to be repainted.
 * When a frame requests update, the area it covers will be repainted once the application
 * goes into idle state. This can be called many times, with only one repaint as a result.
 */
void Frame::update() const {

	update(false,Rect());
}

/**
* Notify (as a user) that this frame has changed and needs to be repainted.
 * When a frame requests update, the area it covers will be repainted once the application
 * goes into idle state. This can be called many times, with only one repaint as a result.
 * This is a more detailed version of update(), for only updating a certain area.
 * @param p_only_rect 
 * @param p_rect 
 */
void Frame::update(bool p_only_rect,const Rect& p_rect) const {

	if (!_fp->window)
		return; // why updating if there is no window anyway

	Rect rect;
	if (p_only_rect) {
		
		rect=p_rect;			
	} else {

		rect.pos=_fp->pos_cache;
		rect.size=_fp->size_cache;
	}
	//TODO, update the visible area
	//rect = _fp->visible_rect.clip(rect);
	
	if (rect.is_empty())
		return; // no area to update

	_fp->window->update_region( rect, const_cast<Frame*>(this) );
}

/* AVAILABLE EXPAND SPACE ALIGNMENT */

/**
 * Determine how the frame will fill the available horizontal space when expanding.	
 * @param p_h_align horizontal alignment mode
 */
void Frame::set_h_align(HAlign p_h_align) {

	_fp->h_align=p_h_align;
	if (_fp->parent)
		_fp->parent->update_size();

}

/**
 * Determine how the frame will fill the available vertical space when expanding.	
 * @param p_v_align vertical alignment mode
 */
void Frame::set_v_align(VAlign p_v_align) {
	
	_fp->v_align=p_v_align;
	if (_fp->parent)
		_fp->parent->update_size();

}
/**
 * Return the horizontal alignment of the frame.
 * @return horizontal alignment
 */
HAlign Frame::get_halign() const {

	return _fp->h_align;
}
/**
 * Return the vertical alignment of the frame.
 * @return vertical alignment
 */
VAlign Frame::get_valign() const {

	return _fp->v_align;
}
/* FOCUS */

/**
 * Determines the way this frame can acquire keyboard focus.
 * Either FOCUS_NONE (never), FOCUS_CLICK (when clicked) or FOCUS_ALL (clicked/keyboard).
 * @param p_focus_mode focus acquire mode
 */
void Frame::set_focus_mode(FocusMode p_focus_mode) {

	_fp->focus_mode=p_focus_mode;
}

/**
 * Return the way this frame can acquire keyboard focus.
 * @return focus acquire mode
 */
FocusMode Frame::get_focus_mode() const {

	return _fp->focus_mode;
}

/**
 * Return true if this frame is actually enabled for keyboard focus.
 * @return 
 */
bool Frame::has_focus() const {

	if (!_fp->window)
		return false;

	return (_fp->window->get_focus_frame() == this );
}


/**
 * Take the keyboard input focus (and steal it from another frame if necesary).
 */
void Frame::grab_focus() {

	if (!_fp->window)
		return;

	if (_fp->focus_mode==FOCUS_NONE)
		return;
	
	_fp->window->set_focus_frame(this);
}

/**
 * Pass the keyboard focus to the next focusable frame in the tree.
 */

void Frame::focus_move_next() {

	if (!_fp->window)
		return;
	
	if (_fp->parent) {

		Frame * next = _fp->parent->find_next_focusable(this);
		if (!next)
			return;

		next->grab_focus();
	}
}

/**
 * Pass the keyboard focus to the previous focusable frame in the tree.
 */

void Frame::focus_move_prev() {

	if (!_fp->window)
		return;
	
	if (_fp->parent) {

		Frame * prev = _fp->parent->find_prev_focusable(this);
		if (!prev)
			return;

		prev->grab_focus();
	}
}

/* SIZE AND POS */
	
void Frame::set_minimum_size(const Size & p_size ) {

	_fp->minimum_size=p_size;
	
	if (_fp->parent)
		_fp->parent->update_size();
}

Size Frame::get_minimum_size() const {

	Size min=compute_minimum_size();
	
	min.width = maxval( min.width, _fp->minimum_size.width );
	min.height = maxval( min.height, _fp->minimum_size.height );
	
	// add width for height to this minsize when requested
	if (has_width_for_height() && _fp->parent && _fp->parent->is_resizing()) {

		min.height = maxval( min.height, get_width_for_height() );
	}

	return min;
}

Size Frame::get_size() const {

	return _fp->size_cache;
}
Point Frame::get_pos() const {

	Point pos =_fp->pos_cache;
	
	if (_fp->parent)
		pos-=_fp->parent->get_pos();

	return pos;
}

Point Frame::get_global_pos() const {

	return _fp->pos_cache;
}

bool Frame::has_width_for_height() const {

	return false;
}

int Frame::get_width_for_height() const {

	return -1;
}

/* EVENTS */

void Frame::draw_event(Painter *p_painter,const Rect& p_exposed) {

}
void Frame::mouse_button_event(const Point& p_pos, int p_button,bool p_press,int p_modifier_mask) {

}
void Frame::mouse_doubleclick_event(const Point& p_pos,int p_modifier_mask) {

}
void Frame::mouse_motion_event(const Point& p_pos, const Point& p_rel, int p_button_mask) {

}
void Frame::key_event(unsigned long p_unicode, unsigned long p_scan_code,bool p_press,bool p_repeat,int p_modifier_mask) {


}

void Frame::mouse_enter_event() {

}
void Frame::mouse_leave_event() {

}

void Frame::focus_enter_event() {

}
void Frame::focus_leave_event() {

}
void Frame::visibility_changed_event(bool p_visible) {

}

void Frame::window_enter_event() {

}
void Frame::window_exit_event() {

}

/* IDENTITY */

void Frame::set_name(String p_name) {

	_fp->name=p_name;
}
String Frame::get_name() const {

	return _fp->name;
}

Window *Frame::get_window() const {

	return _fp->window;
}
Container* Frame::get_parent() const {

	return _fp->parent;
}

/* VISIBILITY */

void Frame::show() {

	if (_fp->visible)
		return; //useless

	_fp->visible=true;

	if (_fp->parent && is_visible()) {

		_fp->parent->update_size();
	}
}
void Frame::hide() {

	if (!_fp->visible)
		return; //useless

	bool was_visible=is_visible();

	_fp->visible=false;

	if (_fp->parent && was_visible) {

		_fp->parent->update_size();
	}
}

bool Frame::is_visible() {

	Frame *f = this;
	
	while(f) {

		if (!f->_fp->visible)
			return false;
		f=f->_fp->parent;
	}

	return true;
}


Frame::Frame() {

	_fp = GUI_NEW( FramePrivate );
}

Frame::~Frame() {

	if (_fp->parent && _fp->parent->has_child(this))
		_fp->parent->remove_child(this);
		
	if (_fp->window)
		_fp->window->frame_deleted_notify( this );
	
	GUI_DELETE( _fp );
}

}
