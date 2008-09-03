//
// C++ Interface: platform_x11
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef GUI_PLATFORM_X11_H
#define GUI_PLATFORM_X11_H

#ifdef PIGUI_X11_ENABLED

#include "base/platform.h"
#include "drivers/x11/window_x11.h"

#include <X11/Xlib.h>


namespace GUI {

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class PlatformX11 : public Platform {

	bool exit;
	WindowX11 *window_list;
	
	Display *x11_display;
public:

	virtual PlatformTheme *get_theme() const;
	virtual PlatformPointer *get_pointer() const;

	virtual PlatformWindow* create_window(unsigned int p_flags=WINDOW_FLAGS_DEFAULT);
	virtual PlatformPixmap* create_pixmap();
	virtual PlatformFont* create_font();
	virtual PlatformFile *create_file();
	virtual PlatformFileSystem *create_file_system();
	virtual PlatformTimer *create_timer();

	virtual void iteration();	
	virtual int run();
	
	PlatformX11();
	
	~PlatformX11();

};


};

#endif
#endif
