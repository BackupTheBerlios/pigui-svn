//
// C++ Interface: variant
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef GUI_VARIANT_H
#define GUI_VARIANT_H

#include "base/constants.h"
#include "base/pstring.h"
#include "base/pixmap.h"
#include "base/font.h"
#include "base/geometry.h"
#include "base/list.h"
#include "base/style_box.h"

namespace GUI {
/**
	@author Juan Linietsky <reduzio@gmail.com>
*/

class Object;

class Variant {
public:

	enum Type {
	
		NIL,
		BOOL,
		INT,
		DOUBLE,
		STRING,
		POINT,
		RECT,
		COLOR,
		PIXMAP,
		FONT,
		STYLEBOX,
		OBJECT,
	};
	
private:

	// A variant is, at most. 20 bytes.
	
	Type type; // type part 
		
	union {
	
		bool _bool;
		double _double;
		int _int;
		int _ints[4]; // for point/rect
		unsigned char mem[16]; // this is enough to contain a String, Font, Pixmap or StyleBox
		Object *_object;
	} _data;
	
	void clear();
	void reference(const Variant&p_variant);
	
public:

	inline Type get_type() const { return type; }

	operator bool() const;
	operator signed int() const;
	operator unsigned int() const; // this is the real one
	operator signed short() const;
	operator unsigned short() const;
	operator signed char() const;
	operator unsigned char() const;
	operator float() const;
	operator double() const;
	operator String() const;
	operator Point() const;
	operator Rect() const;
	operator Color() const;
	operator Pixmap() const;
	operator Font() const;
	operator StyleBox() const;
	operator Object*() const;
	
	Variant(bool p_bool);
	Variant(signed int p_int); // real one
	Variant(unsigned int p_int);
	Variant(signed short p_short); // real one
	Variant(unsigned short p_short);
	Variant(signed char p_char); // real one
	Variant(unsigned char p_char);
	Variant(float p_float);
	Variant(double p_double);
	Variant(const String& p_string);
	Variant(const char * p_cstring);
	Variant(const String::CharType * p_wstring);
	Variant(const Point& p_point);
	Variant(const Rect& p_rect);
	Variant(const Color& p_color);
	Variant(const Pixmap& p_pixmap);
	Variant(const Font& p_font);
	Variant(const StyleBox& p_stylebox);
	Variant(Object *p_object);
	

	void operator=(const Variant& p_variant);
	Variant(const Variant& p_variant);
	inline Variant() { type=NIL; }    
	~Variant();

};

};
#endif
