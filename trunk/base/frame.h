    //
// C++ Interface: frame
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef GUI_FRAME_H
#define GUI_FRAME_H

#include "base/geometry.h"
#include "base/pixmap.h"
#include "base/font.h"
#include "base/style_box.h"


namespace GUI {

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/

class FramePrivate;
class Window;
class Container;
class Widget;
class Painter;


class Frame {

	FramePrivate *_fp;

friend class Container;
friend class Window;

	virtual void set_window(Window *p_window); 
	void set_parent(Container *p_parent); 
protected:

	virtual Size compute_minimum_size() const;

	void set_update_bg(bool p_enabled); 

	const StyleBox& stylebox(int p_index) const;
	const Font& font(int p_index) const;
	const Pixmap& pixmap(int p_index) const;
	int constant(int p_index) const;
	const Color& color(int p_index) const;

public:

	/* THEME CUSTOMIZATION */

	void replace_stylebox(int p_style, int p_with_style); 
	void replace_font(int p_font, int p_with_font); 
	void replace_pixmap(int p_pixmap, int p_with_pixmap); 
	void replace_constant(int p_constant, int p_with_constant);
	void replace_color(int p_color, int p_with_color); 
	
	void set_custom_stylebox(int p_style, const StyleBox& p_stylebox); 
	void set_custom_font(int p_font, const Font& p_font); 
	void set_custom_pixmap(int p_pixmap, const Pixmap& p_pixmap); 
	void set_custom_constant(int p_constant, int p_with_constant);
	void set_custom_color(int p_color, const Color& p_with_color);

	/* HINTING */

	virtual void set_cursor(Pixmap p_pixmap, const Point& p_hotspot);
	virtual Pixmap get_cursor(const Point& p_point=Point(),Point *p_hotspot=NULL) const;
	
	void set_tooltip(String p_text);
	virtual String get_tooltip(const Point& p_point=Point()) const;

	/* UPDATE */

	bool has_update_bg() const; ///< check wether BG needs to be redrawn behind

	virtual	void update() const;; ///< Update the whole widget frame when idle.
	void update(bool p_only_rect,const Rect& p_rect=Rect()) const;; ///< Call update. This means the frame will be redraw when IDLE (not when update called!). Alternative you can ask to update only a region.

	/* AVAILABLE EXPAND SPACE ALIGNMENT */

	void set_h_align(HAlign p_halign); ///< Determine how the frame will fill the available horizontal space when expanding.
	void set_v_align(VAlign p_valign); ///< Determine how the frame will fill the available vertical space when expanding.
	HAlign get_halign() const; ///< Get horizontal alignment.
	VAlign get_valign() const; ///< Get vertical alignment
	
	/* FOCUS */

	void set_focus_mode(FocusMode p_focus_mode); ///< Alter the way in which this frame will gain focus. check FocusMode enum in defs.h . By default a frame can't aquire focus.
	FocusMode get_focus_mode() const; ///< Get the way this frame can gain focus.
	bool has_focus() const; ///< Return wether this frame is focused
	void grab_focus(); ///< Force the frame to aquire focus.

	virtual void focus_move_next();
	virtual void focus_move_prev();

	/* SIZE AND POS */
		
	void set_minimum_size(const Size & p_size ); ///force a CUSTOM mimimum size for a frame. Almost all the time, frames already compute their minimum size by themselves, but this is just to override it.		
	Size get_minimum_size() const;
	Size get_size() const; ///< Get the size of this frame
	Point get_pos() const; ///< Get the position of this frame, relative to the parent frame
	Point get_global_pos() const; ///< Get the position of this frame (in the window)

	virtual bool has_width_for_height() const;
	virtual int get_width_for_height() const;

	/* EVENTS */

	virtual void draw_event(Painter *p_painter,const Rect& p_exposed);
	virtual void mouse_button_event(const Point& p_pos, int p_button,bool p_press,int p_modifier_mask);
	virtual void mouse_doubleclick_event(const Point& p_pos,int p_modifier_mask);
	virtual void mouse_motion_event(const Point& p_pos, const Point& p_rel, int p_button_mask);
	virtual void key_event(unsigned long p_unicode, unsigned long p_scan_code,bool p_press,bool p_repeat,int p_modifier_mask);

	virtual void mouse_enter_event();
	virtual void mouse_leave_event();

	virtual void focus_enter_event();
	virtual void focus_leave_event();
	virtual void visibility_changed_event(bool p_visible);

	virtual void window_enter_event();
	virtual void window_exit_event();

	/* IDENTITY */

	void set_name(String p_name);
	String get_name() const;

	Window *get_window() const; ///< Get the window owning this frame
	Container* get_parent() const; ///< Parents of frames are always containers
	
	virtual Container *container_cast()=0; ///< if this frame is a container, return itself as container (otherwise null)
	virtual Widget *widget_cast()=0; ///< if this frame is a widget, return itself as widget (otherwise null)

	/* VISIBILITY */

	void show();
	void hide();
	bool is_visible();

	
	Frame();	
	virtual ~Frame();

};

}

#endif
