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

enum WindowFlags {

	WINDOW_FLAG_CAN_CLOSE=1,
	WINDOW_FLAG_RESIZABLE=2,
	WINDOW_FLAG_MODAL=4,
	WINDOW_FLAG_POPUP=8,
	WINDOW_FLAGS_DEFAULT=WINDOW_FLAG_CAN_CLOSE|WINDOW_FLAG_RESIZABLE
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
