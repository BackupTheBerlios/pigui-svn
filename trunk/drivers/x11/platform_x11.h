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
#include "drivers/x11/pixmap_x11.h"
#include "drivers/x11/font_x11.h"


#include <X11/Xlib.h>
#include <X11/Xft/Xft.h>


namespace GUI {

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class PlatformX11 : public Platform {

	bool exit;
	WindowX11 *window_list;
	int best_pixmap_depth;
	
	::Display *x11_display;
	::XIM xim;
	::XIMStyle xim_style;
	
	bool performing_size_updates;
	List<WindowX11*> pending_size_update_windows;
	
	struct FontInfoListX11 {
		
		String name;
		unsigned int flags;
		FontInfoListX11 *next;		
		FontInfoListX11() { flags=0; next=0; }
	};
	
	FontInfoListX11 **font_info;
	int font_info_count;
	
	void generate_font_list();
	
friend class WindowX11;

	inline PixmapX11 * extract_pixmap_x11( const Pixmap& p_pixmap ) {

		return static_cast<PixmapX11*>( extract_platform_pixmap(p_pixmap) );
	}

	inline FontX11 * extract_font_x11( const Font& p_font ) {
		
		return static_cast<FontX11*>( extract_platform_font(p_font) );
	}
	
	void add_size_update_window(WindowX11 *p_window);
public:

	inline ::XIM get_xim() { return xim; }
	inline ::XIMStyle get_xim_style() { return xim_style; }

	virtual PlatformTheme *get_theme() const;
	virtual PlatformPointer *get_pointer() const;

	virtual PlatformWindow* create_window(PlatformWindow* p_parent=NULL);
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
