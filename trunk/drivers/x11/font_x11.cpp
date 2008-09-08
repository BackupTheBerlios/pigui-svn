//
// C++ Implementation: font_x11
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "font_x11.h"
#include "drivers/x11/platform_x11.h"

namespace GUI {


Error FontX11::load(const String& p_name,float p_size,unsigned int p_flags) {
	
	if (font) {
		XftFontInfoDestroy( x11_display, font_info );
		XftFontClose( x11_display, font );
		
		font=NULL;
	}	

	name="";
	size=0;
	flags=0;
	
	font = XftFontOpen (x11_display, DefaultScreen(x11_display),XFT_FAMILY, XftTypeString, p_name.utf8().get_data(),
		                    XFT_SIZE, XftTypeDouble, p_size,NULL);	
		
	if (!font) {
		
		return ERR_UNSUPPORTED;
	}
	
	font_info = XftFontInfoCreate( x11_display, font->pattern );

	flags=p_flags;
	name=p_name;
	size=p_size;
	return OK;
}


String FontX11::get_name() const {
	
	return name;
}
float FontX11::get_size() const {
	
	return size;
}

bool FontX11::has_character(unsigned int p_code) const {
	
	return true;
}

int FontX11::get_height() const {
	
	if (!font)
		return 0;
	
	return font->height;
}
int FontX11::get_ascent() const {
	
	if (!font)
		return 0;
	
	return font->ascent;
}
int FontX11::get_descent() const {
	
	if (!font)
		return 0;
	
	return font->descent;
}
int FontX11::get_string_width(const String& p_string) const {
	
	if (!font)
		return 0;
	
	XGlyphInfo ginfo;
	
	// this will dissapear at compile time
	switch(sizeof(String::CharType)) {
		
		case 1: { // ascii?
			
			XftTextExtents8( x11_display, font, (FcChar8*)p_string.c_str(), p_string.length(), &ginfo );
		} break;
		case 2: { // ucs16
			
			XftTextExtents16( x11_display, font, (FcChar16*)p_string.c_str(), p_string.length(), &ginfo );
		} break;
		case 4: { // ucs32
			
			XftTextExtents32( x11_display, font, (FcChar32*)p_string.c_str(), p_string.length(), &ginfo );
		} break;
	}
	
	return ginfo.width;
}

bool FontX11::is_valid() const {
	
	return font!=NULL;
}

PlatformFont *FontX11::copy() const {
	
	return NULL;
}

FontX11::FontX11(PlatformX11 *p_platform,Display *p_x11_display) : PlatformFont(p_platform) {
	
	platform_x11=p_platform;
	x11_display=p_x11_display;
	font=NULL;
	size=0;
	flags=0;
}


FontX11::~FontX11() {
	
	if (font) {
		XftFontInfoDestroy( x11_display, font_info );
		XftFontClose( x11_display, font );
	}
		
}


}
