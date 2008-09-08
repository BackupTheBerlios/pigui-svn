//
// C++ Interface: font_x11
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef GUIFONT_X11_H
#define GUIFONT_X11_H

#include "base/platform.h"
#include <X11/Xlib.h>
#include "X11/extensions/Xrender.h"
#include "X11/Xft/Xft.h"

namespace GUI {


/**
	@author Juan Linietsky <reduzio@gmail.com>
*/

class PlatformX11;

class FontX11 : public PlatformFont {
	
	PlatformX11 *platform_x11;
	Display *x11_display;
	
	XftFont *font;
	XftFontInfo *font_info;
	
	String name;
	float size;
	unsigned int flags;
	
friend class PlatformX11;
	FontX11(PlatformX11 *p_platform,Display *p_x11_display);	
public:
	
	inline XftFont* get_xft_font() { return font; }
	
	virtual Error load(const String& p_name,float p_size,unsigned int p_flags=0);
	
	virtual String get_name() const;
	virtual float get_size() const;
	
	virtual bool has_character(unsigned int p_code) const;
	
	virtual int get_height() const;
	virtual int get_ascent() const;
	virtual int get_descent() const;
	virtual int get_string_width(const String& p_string) const;	
	
	virtual bool is_valid() const;
	
	virtual PlatformFont *copy() const;
	
	FontX11();
	
	~FontX11();

};

}

#endif
