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

struct _CustomFont {


	enum {
		HASHTABLE_BITS=8,
		HASHTABLE_SIZE=(1<<HASHTABLE_BITS),
		HASHTABLE_MASK=HASHTABLE_SIZE-1
	};

	struct Character {

		unsigned int valign;
		Pixmap pixmap;
		unsigned int unicode;
		Rect rect; //rect from the pixmap
		Character *next;
		Character() { next=0; unicode=0; valign=0; }
	};

	Character *characters[HASHTABLE_SIZE];

	int height;
	int ascent;
	bool in_use;

	void cleanup() {
		for (int i=0;i<HASHTABLE_SIZE;i++) {

			while(characters[i]) {
				Character *c=characters[i];
				characters[i]=c->next;
				GUI_DELETE(c);
			}
		}
		height=0;
		ascent=0;
		in_use=false;
	}

	void add_char(unsigned int p_char,Pixmap p_pixmap,const Rect& p_rect,unsigned int p_valign=0) {

		Character *c=find_char( p_char);

		if ( !c ) {

			c = GUI_NEW( Character );
			int bucket=p_char&HASHTABLE_MASK;
			c->next=characters[bucket]; //should be added at the end for speed...
			characters[bucket]=c;
		}


		c->pixmap=p_pixmap;
		c->unicode=p_char;
		c->rect=p_rect;
		c->valign=p_valign;
	}



	inline Character * find_char(unsigned int p_unicode) {


		unsigned int bucket=p_unicode&HASHTABLE_MASK;

		Character *c=characters[bucket];

		while (c) {

			if (c->unicode==p_unicode)
				return c;
			c=c->next;
		}

		return 0; // should change this to some default
	}

	void copy_from(const _CustomFont &p_font_custom) {

		cleanup();
		ascent=p_font_custom.ascent;
		height=p_font_custom.height;

		for(int i=0;i<HASHTABLE_SIZE;i++) {

			Character *c=p_font_custom.characters[i];
		
			while (c) {

				add_char(c->unicode,c->pixmap,c->rect,c->valign);
				c=c->next;
			}			
		}
	}

	_CustomFont() { ascent=0; height=0; for (int i=0;i<HASHTABLE_SIZE;i++) characters[i]=0; }
	_CustomFont(const _CustomFont& p_custom) { 
		ascent=0; 
		height=0; 
		for (int i=0;i<HASHTABLE_SIZE;i++) 
			characters[i]=0; 
		cleanup(); 
		copy_from(p_custom); }
	~_CustomFont() { cleanup(); }

};

class FontPrivate {
public:
	
	_CustomFont *custom;
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

			if (custom) {

				copy->custom = GUI_NEW( _CustomFont );
				copy->custom->copy_from( *custom );
			}
		}
	}

	FontPrivate () { font=NULL; custom=NULL; refcount=1; }
	~FontPrivate() { 
		if (custom) GUI_DELETE( custom );
		if (font) GUI_DELETE( font );
	}
};


PlatformFont *Font::get_platform_font() const {

	if (_fp)
		return _fp->font;
	return NULL;
}

/* CUSTOM FONT METHODS */

void Font::custom_create(int p_height,int p_ascent) {


	if (p_height<1) {

		GUI_PRINT_ERROR("Invalid Font Height");
		return;
	}
	
	unreference();

	_fp = GUI_NEW( FontPrivate );
	_fp->custom = GUI_NEW( _CustomFont );
	_fp->custom->ascent=p_ascent;
	_fp->custom->height=p_height;
}

void Font::custom_add_char(unsigned int p_char, const Pixmap& p_pixmap, const Rect& p_rect, int p_valign) {

	if (!_fp || !_fp->custom)
		return;
	

	_fp->custom->add_char( p_char, p_pixmap, p_rect, p_valign );
}
Error Font::custom_get_char(unsigned int p_char, Pixmap& r_pixmap, Rect& r_rect, int& r_valign) {

	if (!_fp || !_fp->custom)
		return ERR_UNCONFIGURED;

	_CustomFont::Character * c = _fp->custom->find_char( p_char );
	if (!c)
		return ERR_NOT_FOUND;

	r_pixmap=c->pixmap;
	r_rect=c->rect;
	r_valign=c->valign;

	return OK;
}

/* SYSYEM FONT METHODS */

Error Font::system_load(const String& p_name,float p_size,unsigned int p_font_tyle_flags) {

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


String Font::system_get_name() const {

	if (!_fp || !_fp->font)
		return "";

	return _fp->font->get_name();
}
float Font::system_get_size() const {

	if (!_fp || !_fp->font)
		return -1;

	return _fp->font->get_size();
}

unsigned int Font::system_get_flags() const {

	if (!_fp || !_fp->font)
		return -1;

	return _fp->font->get_flags();

}
/* COMMON METHODS */

bool Font::is_custom() const {

	return (_fp && _fp->custom);
}

bool Font::has_character(unsigned int p_code) const {

	if (_fp && _fp->font) 
		return _fp->font->has_character(p_code);

	if (_fp && _fp->custom) 
		return _fp->custom->find_char(p_code)!=NULL;

	GUI_PRINT_ERROR("Unconfigured Font");	

	return false;
}

int Font::get_height() const {

	if (_fp && _fp->font) 
		return _fp->font->get_height();

	if (_fp && _fp->custom) 
		return _fp->custom->height;

	GUI_PRINT_ERROR("Unconfigured Font");
	return -1;
}
int Font::get_ascent() const {

	if (_fp && _fp->font) 
		return _fp->font->get_ascent();

	if (_fp && _fp->custom) 
		return _fp->custom->ascent;

	GUI_PRINT_ERROR("Unconfigured Font");
	return -1;
}
int Font::get_descent() const {

	if (_fp && _fp->font) 
		return _fp->font->get_descent();

	if (_fp && _fp->custom) 
		return _fp->custom->height-_fp->custom->ascent;

	GUI_PRINT_ERROR("Unconfigured Font");
	return -1;
}
int Font::get_string_width(const String& p_string) const {

	if (_fp && _fp->font) 
		return _fp->font->get_string_width(p_string);

	if (_fp && _fp->custom) {

		int w=0;
		for (int i=0;i<p_string.length();i++) {

			_CustomFont::Character *c=_fp->custom->find_char(p_string[i]);
			if (c)
				w+=c->rect.size.width;		
		}
		
		return w;
	}

	GUI_PRINT_ERROR("Unconfigured Font");
	return 0;

}

bool Font::is_valid() const {

	if (!_fp)
		return false;

	if (_fp->font)
		return _fp->font->is_valid();

	return _fp->custom!=NULL;
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