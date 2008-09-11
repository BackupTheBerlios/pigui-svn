//
// C++ Implementation: object
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "object.h"

namespace GUI {

class ObjectPrivate {
public:

	String name;
};

void Object::_get_property_listv(List<PropertyInfo> *p_list) const {

	p_list->push_back( PropertyInfo( Variant::STRING, "name", PROPERTY_HINT_LENGTH, "256" ) );
}

void Object::_get_propertyv(const String& p_name,Variant &r_property) const {

	if (p_name=="name")
		r_property=op->name;

}

void Object::_set_propertyv(const String& p_name,Variant &p_property) {

	if (p_name=="name")
		op->name=p_property;

}

String Object::get_type() {

	return "Object";
}


bool Object::is_type(String p_type) {

	return (p_type=="Object");
}

void Object::set_name(String p_name) {

	set_property("name",p_name); // may want to be overriden
}
String Object::get_name() const {

	return op->name;
}

void Object::set_property(const String& p_name,const Variant& p_property) {

	// This can be modified along the way. 
	// An example is: a container trying to avoid its children from having two equal names
	
	Variant property=p_property; 
	_set_propertyv(p_name,property);
}
Variant Object::get_property(const String& p_name) const {

	Variant result;
	_get_propertyv(p_name,result);
	return result;
}


void Object::get_property_list(List<PropertyInfo> *p_list) const {

	_get_property_listv(p_list);
}

void Object::connect( const String& p_signal, Slot *p_slot ) {

	p_slot->call(String("moush"));

}


Object::Object() {

	op = GUI_NEW( ObjectPrivate );
}


Object::~Object() {

	GUI_DELETE( op );
}


}
