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

	Error load(const String& p_name,float p_size,unsigned int p_font_tyle_flags=0); ///< Load a system font (erases custom font)
	

	String get_name() const; ///< get system font name
	unsigned int get_flags() const; ///< get system font flags
	float get_size() const; ///< get system font size

	/* COMMON METHODS */

	bool is_custom() const;

	bool has_character(unsigned int p_code) const;
	
	int get_height() const;
	int get_ascent() const;
	int get_descent() const;
	int get_string_width(const String& p_string) const;	

	bool is_valid() const;

	void operator=(const Font& p_font);
	Font(const Font& p_font);
	Font();
	~Font();

};

} // end of namespace

#endif
