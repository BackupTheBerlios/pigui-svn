//
// C++ Interface: font
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef FONT_H
#define FONT_H


#include "base/geometry.h"
#include "base/constants.h"
#include "base/pixmap.h"

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/

namespace GUI {

class FontPrivate;
class PlatformFont;

class Font {
	
	mutable FontPrivate *_fp;
	
	void reference(const Font& p_font);
	void unreference();

friend class Platform;

	PlatformFont *get_platform_font() const;
public:
	
	/* CUSTOM FONT METHODS */

	void custom_create(int p_height,int p_ascent); ///< Create a custom font (erases platform font)
	void custom_add_char(unsigned int p_char, const Pixmap& p_pixmap, const Rect& p_rect, int p_valign); ///< Set a custom font character
	Error custom_get_char(unsigned int p_char, Pixmap& r_pixmap, Rect& r_rect, int& r_valign); ///< Get a custom font character

	/* SYSYEM FONT METHODS */

	virtual Error system_load(const String& p_name,float p_size,unsigned int p_font_tyle_flags=0); ///< Load a system font (erases custom font)
	

	virtual String system_get_name() const; ///< get system font name
	virtual unsigned int system_get_flags() const; ///< get system font flags
	virtual float system_get_size() const; ///< get system font size

	/* COMMON METHODS */

	bool is_custom() const;

	virtual bool has_character(unsigned int p_code) const;
	
	virtual int get_height() const;
	virtual int get_ascent() const;
	virtual int get_descent() const;
	virtual int get_string_width(const String& p_string) const;	

	virtual bool is_valid() const;

	void operator=(const Font& p_font);
	Font(const Font& p_font);
	Font();
	~Font();

};

} // end of namespace

#endif
