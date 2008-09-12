//
// C++ Interface: constants
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef GUICONSTANTS_H
#define GUICONSTANTS_H


// only depend on the very very basic
#include <stddef.h> 
#include "base/memory.h"

extern void (*_GUI_print_error_func)(const char *,int,const char*);
extern void (*_GUI_print_warning_func)(const char *,int,const char*);

namespace GUI {


#define GUI_PRINT_ERROR(m_err) { if (_GUI_print_error_func) _GUI_print_error_func(__FILE__,__LINE__,m_err); };
#define GUI_PRINT_WARNING(m_err) { if (_GUI_print_warning_func) _GUI_print_error_func(__FILE__,__LINE__,m_err); };

enum MouseButton { 

	BUTTON_LEFT=1, ///< Left Mouse Button
	BUTTON_MIDDLE=2, ///< Middle Mouse Button
	BUTTON_RIGHT=3, ///< Right Mouse Button
	BUTTON_WHEEL_UP=4, ///< Mouse wheel UP
	BUTTON_WHEEL_DOWN=5, ///< Mouse wheel DOWN
};

enum MouseButtonMask {

	BUTTON_MASK_LEFT=(1<<(BUTTON_LEFT-1)),
	BUTTON_MASK_MIDDLE=(1<<(BUTTON_MIDDLE-1)),
	BUTTON_MASK_RIGHT=(1<<(BUTTON_RIGHT-1)),
	BUTTON_MASK_WHEEL_UP=(1<<(BUTTON_WHEEL_UP-1)),
	BUTTON_MASK_WHEEL_DOWN=(1<<(BUTTON_WHEEL_DOWN-1)),
};

enum Margin {
	
	MARGIN_LEFT, 
	MARGIN_TOP,	
	MARGIN_RIGHT,	
	MARGIN_BOTTOM,
	
};
	
enum Direction {
	
	UP,
	LEFT,
	DOWN,
	RIGHT
};

enum HAlign {

	HALIGN_LEFT,
	HALIGN_CENTER,
	HALIGN_RIGHT,
	HALIGN_FILL,
};

enum VAlign {

	VALIGN_TOP,
	VALIGN_CENTER,
	VALIGN_BOTTOM,
	VALIGN_FILL,
};

enum FocusMode {
	FOCUS_NONE, ///< Frame can't be focused in any way
	FOCUS_CLICK, ///< This type of focus means the frame will gain focus only when it's clicked
	FOCUS_ALL ///< This focus is like FOCUS_CLICK, but also adds posibility of keyboard focus.
};

enum Orientation {

	VERTICAL,
	HORIZONTAL
};


enum FontStyleFlags {

	FONT_STYLE_BOLD=1,
	FONT_STYLE_ITALIC=2
};

enum WindowState {

	WINDOW_STATE_CAN_CLOSE,
	WINDOW_STATE_RESIZABLE,
	WINDOW_STATE_MODAL,
	WINDOW_STATE_POPUP,
	WINDOW_STATE_VISIBLE,
	WINDOW_STATE_SHADED,
	WINDOW_STATE_BORDERLESS,
	WINDOW_STATE_SKIP_TASKBAR,
	WINDOW_STATE_MAX,
};


enum FileFlags {

	FILE_READ=1,
	FILE_WRITE=2,
	FILE_READ_WRITE=3,
	FILE_BIG_ENDIAN=16
};

enum PixmapFormat {

	PIXMAP_FORMAT_GRAYSCALE, ///< 8 bits grayscale.
	PIXMAP_FORMAT_ALPHA_MASK, ///< Same as grayscale, but RGB = 1 , A=value.
	PIXMAP_FORMAT_RGB,
	PIXMAP_FORMAT_RGBA,
};

enum PropertyHint {
	PROPERTY_HINT_NONE, ///< no hint provided.
	PROPERTY_HINT_RANGE, ///< hint_text = "min,max,step"
	PROPERTY_HINT_ENUM, ///< hint_text= "val1,val2,val3,etc"
	PROPERTY_HINT_LENGTH, ///< hint_text= "length" (as integer)
	PROPERTY_HINT_FLAGS, ///< hint_text= "flag1,flag2,etc" (as bit flags)
	PROPERTY_HINT_PATH, ///< a file path must be passed, hint_text (optionally) is a filter "*.png,*.wav,*.doc"
	PROPERTY_HINT_FILE, ///< a file path must be passed, hint_text (optionally) is a filter "*.png,*.wav,*.doc," 
	PROPERTY_HINT_DIR, ///< a directort path must be passed
};

enum StyleBoxPixmaps {
	STYLEBOX_PIXMAPS_TOP_LEFT,
	STYLEBOX_PIXMAPS_TOP_CENTER,
	STYLEBOX_PIXMAPS_TOP_RIGHT,
	STYLEBOX_PIXMAPS_CENTER_LEFT,
	STYLEBOX_PIXMAPS_CENTER,
	STYLEBOX_PIXMAPS_CENTER_RIGHT,
	STYLEBOX_PIXMAPS_BOTTOM_LEFT,
	STYLEBOX_PIXMAPS_BOTTOM_CENTER,
	STYLEBOX_PIXMAPS_BOTTOM_RIGHT,
	STYLEBOX_PIXMAPS_MAX,
};
//needing to include math.h/cmath for an integer/float abs function is asking too much

template<class T>
inline T minval(const T& a,const T& b) {

	return (a<b)?a:b;
}

#define maxval(m_a,m_b)\
	(((m_a)>(m_b))?(m_a):(m_b))

#define minval(m_a,m_b)\
	(((m_a)<(m_b))?(m_a):(m_b))

template<class T>
inline T absval(const T& a) {

	return (a<0)?-a:a;
}

template<class T>
inline T signval(const T& a) {

	return (a<0)?-1:+1;
}

#ifdef _MSVC
//use the visual C convention
typedef unsigned __int64 gui_u64;

#else
//use the C99 convention (supported almost everywhere else)
typedef unsigned long long gui_u64;

#endif


enum Error {

	OK,
	ERR_CANT_OPEN_FILE,
	ERR_CANT_WRITE_FILE,
	ERR_NOT_FOUND,
	ERR_INVALID_PARAMETER,
	ERR_IN_USE,
	ERR_UNCONFIGURED,
	ERR_UNSUPPORTED,
};


} //end of namespace



#endif
