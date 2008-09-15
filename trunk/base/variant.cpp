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
#include <new>

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
		
			return ((String*)_data.mem)->length();
		
		} break;
		case POINT: {
		
			return _data._ints[0]!=0 && _data._ints[1]!=0;		
		} break;
		case COLOR:
		case RECT: {
		
			return _data._ints[0]!=0 && _data._ints[1]!=0 && _data._ints[2]!=0 && _data._ints[3]!=0;
		
		} break;
		case PIXMAP: { 
			return ((Pixmap*)_data.mem)->is_valid();		
		} break;
		case FONT: {
			return ((Font*)_data.mem)->is_valid();		
		} break;
		case STYLEBOX: {
			
			return false;
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
		
			return ((String*)_data.mem)->to_int();
		
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
		case STYLEBOX:
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
		
			return ((String*)_data.mem)->to_double();
		
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
		case STYLEBOX:
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
		
			return *((String*)_data.mem);
		
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
			
			Pixmap pixmap = *((Pixmap*)_data.mem);
			return (pixmap.is_valid()?("Pixmap ("+String::num(pixmap.get_size().width)+"x"+String::num(pixmap.get_size().height)+")"):"Pixmap (NIL)");
		};
		case FONT: {
		
			Font font = *((Font*)_data.mem);
		
			if (!font.is_valid()) {
			
				return "Font (NIL)";
			} else {
			
				String fontstr = "Font ("+font.get_name()+","+font.get_size();
				unsigned int flags = font.get_flags();
				if (flags&FONT_STYLE_BOLD)
					fontstr+=",Bold";
				if (flags&FONT_STYLE_ITALIC)
					fontstr+=",Italic";
					
				fontstr+=")";
				
				return fontstr;			
			}
		
		} break;
		case STYLEBOX: {
		
			return "StyleBox";
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
	
		return *((Pixmap*)_data.mem);
	} else {
		return Pixmap();
	}

}
Variant::operator Font() const {

	if (type==FONT) {
	
		return *((Font*)_data.mem);
	} else {
	
		return Font();
	}
}
Variant::operator StyleBox() const {

	if (type==STYLEBOX) {
	
		return *((StyleBox*)_data.mem);
	} else {
		return StyleBox();
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
	
		case STRING: { 
		
			((String*)_data.mem)->~String();
		} break;
		case PIXMAP: { 
			((Pixmap*)_data.mem)->~Pixmap();
			
		} break;
		case FONT: {
			((Font*)_data.mem)->~Font();
		} break;
		case STYLEBOX: {
			((StyleBox*)_data.mem)->~StyleBox();
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
		
			new((void*)&_data.mem[0]) String(*((String*)p_variant._data.mem));
		
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
			new(_data.mem) Pixmap(*((Pixmap*)p_variant._data.mem));			
		} break;
		case FONT: {
			new(_data.mem) Font(*((Font*)p_variant._data.mem));

		} break;
		case STYLEBOX: {
			new(_data.mem) StyleBox(*((StyleBox*)p_variant._data.mem));

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

Variant::Variant(unsigned int p_int) {


	clear();
	type=INT;
	_data._int=p_int;

}
Variant::Variant(signed short p_short) {


	clear();
	type=INT;
	_data._int=p_short;

}
Variant::Variant(unsigned short p_short) {

	clear();
	type=INT;
	_data._int=p_short;

}
Variant::Variant(signed char p_char) {

	clear();
	type=INT;
	_data._int=p_char;

}
Variant::Variant(unsigned char p_char) {

	clear();
	type=INT;
	_data._int=p_char;

}
Variant::Variant(float p_float) {

	clear();
	type=DOUBLE;
	_data._double=p_float;

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
	new(_data.mem) String(p_string);


}

Variant::Variant(const char * p_cstring) {

	clear();
	type=STRING;
	new(_data.mem) String(p_cstring);
	
}
Variant::Variant(const String::CharType * p_wstring) {

	clear();
	type=STRING;
	new(_data.mem) String(p_wstring);

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
	new(_data.mem) Pixmap(p_pixmap);

	
}
Variant::Variant(const Font& p_font) {

	clear();
	type=FONT;
	new(_data.mem) Font(p_font);
	

}

Variant::Variant(const StyleBox& p_stylebox) {

	clear();
	type=STYLEBOX;
	new(_data.mem) StyleBox(p_stylebox);
	

}
Variant::Variant(Object *p_object) {

	clear();
	if (p_object) {
		type=OBJECT;
		_data._object=p_object;
	}

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
