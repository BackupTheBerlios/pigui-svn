
//
// C++ Implementation: font
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "font.h"
#include "base/platform.h"

namespace GUI {

class FontPrivate {
public:
	
	PlatformFont *font;
	int refcount;

	FontPrivate *copy_on_write() {

		if (refcount==1)
			return this;
		else {
			
			refcount--;
			FontPrivate *copy= GUI_NEW( FontPrivate );
			
			copy->refcount=1;
			if (font)
				copy->font=font->copy();
			
			return copy;
		}
	}

	FontPrivate () { font=NULL;refcount=1; }
	~FontPrivate() { 
		if (font) GUI_DELETE( font );
	}
};


PlatformFont *Font::get_platform_font() const {

	if (_fp)
		return _fp->font;
	return NULL;
}


/* SYSYEM FONT METHODS */

Error Font::load(const String& p_name,float p_size,unsigned int p_font_tyle_flags) {

	unreference();

	_fp = GUI_NEW( FontPrivate );
	_fp->font = Platform::get_singleton()->create_font();
	Error err = _fp->font->load(p_name,p_size,p_font_tyle_flags);
	if (err) {

		unreference();
		return err;
	}

	return OK;
}


String Font::get_name() const {

	if (!_fp || !_fp->font)
		return "";

	return _fp->font->get_name();
}
float Font::get_size() const {

	if (!_fp || !_fp->font)
		return -1;

	return _fp->font->get_size();
}

unsigned int Font::get_flags() const {

	if (!_fp || !_fp->font)
		return -1;

	return _fp->font->get_flags();

}
/* COMMON METHODS */

bool Font::has_character(unsigned int p_code) const {

	if (_fp && _fp->font) 
		return _fp->font->has_character(p_code);

	GUI_PRINT_ERROR("Unconfigured Font");	

	return false;
}

int Font::get_height() const {

	if (_fp && _fp->font) 
		return _fp->font->get_height();

	GUI_PRINT_ERROR("Unconfigured Font");
	return -1;
}
int Font::get_ascent() const {

	if (_fp && _fp->font) 
		return _fp->font->get_ascent();

	GUI_PRINT_ERROR("Unconfigured Font");
	return -1;
}
int Font::get_descent() const {

	if (_fp && _fp->font) 
		return _fp->font->get_descent();


	GUI_PRINT_ERROR("Unconfigured Font");
	return -1;
}
int Font::get_string_width(const String& p_string) const {

	if (_fp && _fp->font) 
		return _fp->font->get_string_width(p_string);

	GUI_PRINT_ERROR("Unconfigured Font");
	return 0;

}

bool Font::is_valid() const {

	return (_fp && _fp->font && _fp->font->is_valid());
}

void Font::reference(const Font& p_font) {

	unreference();

	_fp=p_font._fp;

	if (_fp) {
		
		_fp->refcount++;
	}	
}
void Font::unreference() {

	if (_fp) {

		_fp->refcount--;
		if (_fp->refcount<=0) {

			GUI_DELETE( _fp );
		}
	}

	_fp=NULL;
}

void Font::operator=(const Font& p_font) {
	
	if (&p_font==this)
		return;

	reference(p_font);

}


Font::Font(const Font& p_font) {

	_fp=NULL;
	reference(p_font);
}

Font::Font() {

	_fp=NULL;
}


Font::~Font() {

	unreference();
	
}


} // end of namespace
