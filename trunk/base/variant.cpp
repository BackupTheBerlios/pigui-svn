//
// C++ Implementation: variant
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "variant.h"
#include "base/object.h"

namespace GUI {

Variant::operator bool() const {

	switch(type) {
		
		case NIL: {
		
			return false;
		} break;
		case BOOL: {
		
			return _data._bool;

		} break;
		case INT: {
		
			return _data._int!=0;
		
		} break;
		case DOUBLE: {
		
			return _data._double!=0;
		
		} break;
		case STRING: {
		
			return string.length();
		
		} break;
		case POINT: {
		
			return _data._ints[0]!=0 && _data._ints[1]!=0;		
		} break;
		case COLOR:
		case RECT: {
		
			return _data._ints[0]!=0 && _data._ints[1]!=0 && _data._ints[2]!=0 && _data._ints[3]!=0;
		
		} break;
		case PIXMAP: { 
			return _data._pixmap->is_valid();		
		} break;
		case FONT: {
			return _data._font->is_valid();		
		} break;
		case OBJECT: {
		
			return _data._object!=NULL;
		} break;
	}
	
	return false;
}

Variant::operator unsigned int() const { return operator int(); }
Variant::operator signed short() const { return operator int(); }
Variant::operator unsigned short() const { return operator int(); }
Variant::operator signed char() const { return operator int(); }
Variant::operator unsigned char() const { return operator int(); }


Variant::operator signed int() const {

	switch(type) {
		
		case NIL: {
		
			return 0;
		} break;
		case BOOL: {
		
			return _data._bool?1:0;

		} break;
		case INT: {
		
			return _data._int;
		
		} break;
		case DOUBLE: {
		
			return (int)_data._double;
		
		} break;
		case STRING: {
		
			return string.to_int();
		
		} break;
		case POINT: {
		
			return 0;
		} break;
		case RECT: {
		
			return 0;
		
		} break;
		case COLOR: {
		
			return (_data._ints[0]+_data._ints[1]+_data._ints[3])/3;;
		
		} break;
		case PIXMAP:
		case FONT:
		case OBJECT: {
		
			return 0;
		} break;
	}

	return 0;
}

Variant::operator float() const { return operator double(); }

Variant::operator double() const {

	switch(type) {
		
		case NIL: {
		
			return 0;
		} break;
		case BOOL: {
		
			return _data._bool?1.0:0.0;

		} break;
		case INT: {
		
			return (double)_data._int;
		
		} break;
		case DOUBLE: {
		
			return _data._double;
		
		} break;
		case STRING: {
		
			return string.to_double();
		
		} break;
		case POINT: {
		
			return 0;
		} break;
		case RECT: {
		
			return 0;
		
		} break;
		case COLOR: {
		
			return (_data._ints[0]+_data._ints[1]+_data._ints[3])/3.0;
		
		} break;
		case PIXMAP:
		case FONT:
		case OBJECT: {
		
			return 0;
		} break;
	}

	return 0;
}
/* this doesn't work..

Variant::operator const String&() const {

	// this one is kinda risky, but helps in many cases (like connecting signals to const String&)
	static String _err="";
	
	if (type==STRING) {
	
		return string;
	} else {
	
		GUI_PRINT_ERROR("Forced Variant to 'const String&', when it wasn't of type STRING.");
		_err;
	}	
	
}

*/

Variant::operator String() const {

	switch(type) {
		
		case NIL: {
		
			return "";
		} break;
		case BOOL: {
		
			return _data._bool?"True":"False";

		} break;
		case INT: {
		
			return String::num(_data._int);
		
		} break;
		case DOUBLE: {
		
			return String::num(_data._int);		
		} break;
		case STRING: {
		
			return string;
		
		} break;
		case POINT: {
		
			return "("+String::num(_data._ints[0])+","+String::num(_data._ints[1])+")";
		} break;
		case RECT: {
		
			return "("+String::num(_data._ints[0])+","+String::num(_data._ints[1])+","+String::num(_data._ints[2])+","+String::num(_data._ints[3])+")";
		
		} break;
		case COLOR: {
		
			return String::num(_data._ints[0])+","+String::num(_data._ints[1])+","+String::num(_data._ints[2])+","+String::num(_data._ints[3]);
		
		} break;
		case PIXMAP: {
		
			return (_data._pixmap->is_valid()?("Pixmap ("+String::num(_data._pixmap->get_size().width)+"x"+String::num(_data._pixmap->get_size().height)+")"):"Pixmap (NIL)");
		};
		case FONT: {
		
			if (!_data._font->is_valid()) {
			
				return "Font (NIL)";
			} else if (_data._font->is_custom()) {
			
				return "Font (CUSTOM)";
			} else {
			
				String fontstr = "Font ("+_data._font->system_get_name()+","+_data._font->system_get_size();
				unsigned int flags = _data._font->system_get_flags();
				if (flags&FONT_STYLE_BOLD)
					fontstr+=",Bold";
				if (flags&FONT_STYLE_ITALIC)
					fontstr+=",Italic";
					
				fontstr+=")";
				
				return fontstr;			
			}
		
		} break;
		case OBJECT: {
		
			return _data._object?_data._object->get_name():"";
		} break;
	}

	return "";

}
Variant::operator Point() const {

	if (type==POINT) {
	
		return Point( _data._ints[0], _data._ints[1] );
	} else {
	
		return Point();
	}

}
Variant::operator Rect() const {

	if (type==RECT) {
	
		return Rect(_data._ints[0],_data._ints[1],_data._ints[2],_data._ints[3]);
	} else {
		return Rect();
	}

}

Variant::operator Color() const {

	if (type==COLOR) {
	
		return Color(_data._ints[0],_data._ints[1],_data._ints[2],_data._ints[3]);
	} else {
		return Color();
	}

}
Variant::operator Pixmap() const {

	if (type==PIXMAP) {
	
		return *_data._pixmap;
	} else {
		return Pixmap();
	}

}
Variant::operator Font() const {

	if (type==FONT) {
	
		return *_data._font;
	} else {
		return Font();
	}
}

Variant::operator Object*() const {

	if (type==OBJECT) {
	
		return _data._object;
	} else {
	
		return NULL;
	}
}

void Variant::clear() {

	switch(type) {
	
		case PIXMAP: { 
			GUI_DELETE( _data._pixmap );
			
		} break;
		case FONT: {
			GUI_DELETE( _data._font );
		} break;
		default: break;
	}
	
	type=NIL;

}

void Variant::reference(const Variant&p_variant) {

	clear();
	
	type=p_variant.type;
	
	switch(p_variant.type) {
	
		case NIL: {
			// none	
		} break;
		case BOOL: {
		
			_data._bool=p_variant._data._bool;

		} break;
		case INT: {
		
			_data._int=p_variant._data._int;
		
		} break;
		case DOUBLE: {
		
			_data._double=p_variant._data._double;		
		} break;
		case STRING: {
		
			string=p_variant.string;
		
		} break;
		case POINT: {
		
			for (int i=0;i<2;i++) {
			
				_data._ints[i]=p_variant._data._ints[i];
			}
		} break;
		case COLOR:
		case RECT: {
		
			for (int i=0;i<4;i++) {
			
				_data._ints[i]=p_variant._data._ints[i];
			}
		
		} break;
		case PIXMAP: { 
			_data._pixmap=GUI_NEW( Pixmap );
			*_data._pixmap=*p_variant._data._pixmap;
			
		} break;
		case FONT: {
			_data._font=GUI_NEW( Font );
			*_data._font=*p_variant._data._font;
		} break;
		case OBJECT: {
		
			_data._object=p_variant._data._object;
		} break;
	
	}
}

Variant::Variant(bool p_bool) {

	clear();
	type=BOOL;
	_data._bool=p_bool;
}
Variant::Variant(int p_int) {

	clear();
	type=INT;
	_data._int=p_int;

}
Variant::Variant(double p_double) {

	clear();
	type=DOUBLE;
	_data._double=p_double;

}
Variant::Variant(const String& p_string) {

	clear();
	type=STRING;
	string=p_string;

}
Variant::Variant(const Point& p_point) {

	clear();
	type=POINT;
	_data._ints[0]=p_point.x;
	_data._ints[1]=p_point.y;

}
Variant::Variant(const Rect& p_rect) {

	clear();
	type=RECT;
	_data._ints[0]=p_rect.pos.x;
	_data._ints[1]=p_rect.pos.y;
	_data._ints[2]=p_rect.size.x;
	_data._ints[3]=p_rect.size.y;
}
Variant::Variant(const Color& p_color) {

	clear();
	type=COLOR;
	_data._ints[0]=p_color.r;
	_data._ints[1]=p_color.g;
	_data._ints[2]=p_color.b;
	_data._ints[3]=p_color.a;
}
Variant::Variant(const Pixmap& p_pixmap) {

	clear();
	type=PIXMAP;
	_data._pixmap = GUI_NEW( Pixmap );
	*_data._pixmap=p_pixmap;
	
}
Variant::Variant(const Font& p_font) {

	clear();
	type=FONT;
	_data._font = GUI_NEW( Font );
	*_data._font=p_font;
	

}
Variant::Variant(Object *p_object) {

	clear();
	type=OBJECT;
	_data._object=p_object;
	_data._object=p_object;

}

void Variant::operator=(const Variant& p_variant) {

	reference(p_variant);
}
Variant::Variant(const Variant& p_variant) {

	type=NIL;
	reference( p_variant );
}
Variant::~Variant() {

	clear();
}


}
