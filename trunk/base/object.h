//
// C++ Interface: object
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef GUIOBJECT_H
#define GUIOBJECT_H

#include "base/variant.h"
#include "base/list.h"
#include "base/constants.h"
#include "base/slot.h"

namespace GUI {

/**
	@author Juan Linietsky <reduzio@gmail.com>
	
*/

// use this macro on your inherited classes, and respect the inheritance!

struct PropertyInfo {

	Variant::Type type;	
	String name;
	PropertyHint hint;
	String hint_string;	
	
	PropertyInfo() { type=Variant::NIL; hint=PROPERTY_HINT_NONE; }
	PropertyInfo( Variant::Type p_type, const String p_name, PropertyHint p_hint=PROPERTY_HINT_NONE, const String& p_hint_string="") {
		type=p_type; name=p_name; hint=p_hint; hint_string=p_hint_string;
	}
};

#define GUI_TYPE( m_type, m_inherits )\
private:\
	struct _##m_type##_##_GetSetListProperties {\
\
		void (m_type::*get_property)(const String&,Variant&) const;\
		void (m_type::*set_property)(const String&,Variant&);\
		void (m_type::*get_property_list)(List<PropertyInfo>*) const;\
\
		_##m_type##_##_GetSetListProperties() {\
\
			get_property=NULL;\
			set_property=NULL;\
			get_property_list=NULL;\
		}\
	};\
	_##m_type##_##_GetSetListProperties _##m_type##_get_set_list_properties;\
\
public:\
	virtual String get_type() { return String(#m_type); }\
	virtual bool is_type(String p_type) { return (String(#m_type)==p_type)?true:m_inherits::is_type(p_type); }\
private:\
	virtual void _get_propertyv(const String& p_name,Variant &r_property) const { \
		if (_##m_type##_get_set_list_properties.get_property)\
			(this->*_##m_type##_get_set_list_properties.get_property)(p_name,r_property);\
		return m_type::_get_propertyv(p_name,r_property);\
	}\
	virtual void _set_propertyv(const String& p_name,Variant &p_property) { \
		if (_##m_type##_get_set_list_properties.get_property)\
			(this->*_##m_type##_get_set_list_properties.set_property)(p_name,p_property);\
		return m_type::_set_propertyv(p_name,p_property);\
	}\
	virtual void _get_property_listv(List<PropertyInfo> *p_list) const { \
		if (_##m_type##_get_set_list_properties.get_property_list)\
			(this->*_##m_type##_get_set_list_properties.get_property_list)(p_list);\
		m_type::_get_property_listv(p_list);\
	}\
protected:\
	void connect_get_property(void (m_type::*p_get_property_func)(const String&,Variant&) const ) {\
\
		_##m_type##_get_set_list_properties.get_property=p_get_property_func;\
	}\
\
	void connect_set_property(void (m_type::*p_set_property_func)(const String&,Variant&) ) {\
\
		_##m_type##_get_set_list_properties.set_property=p_set_property_func;\
	}\
	void connect_get_property_list(void (m_type::*p_get_property_list_func)(List<PropertyInfo>*) const ) {\
\
		_##m_type##_get_set_list_properties.get_property_list=p_get_property_list_func;\
	}\
	
class ObjectPrivate;

class Object {

	ObjectPrivate *op;
protected:
	
	virtual void _get_property_listv(List<PropertyInfo> *p_list) const;
	virtual void _get_propertyv(const String& p_name,Variant &r_property) const;
	virtual void _set_propertyv(const String& p_name,Variant &p_property);
public:

	virtual String get_type();
	virtual bool is_type(String p_type);
	
	void set_name(String p_name);
	String get_name() const;
	
	void set_property(const String& p_name,const Variant& p_property);
	Variant get_property(const String& p_name) const;
	
	void get_property_list(List<PropertyInfo> *p_list) const;	
	
	void connect( const String& p_signal, Slot *p_slot );
	
	template<class T>
	void connect( const String& p_signal, T* p_instance, void (T::*p_method)() ) {
	
		connect( p_signal, GUI_NEW(  (Slot0<T>)( p_instance, p_method) ) );
	}
	
	template<class T, class P1>
	void connect( const String& p_signal, T* p_instance, void (T::*p_method)(P1) ) {
	
		connect( p_signal, GUI_NEW(  (Slot1<T,P1>)( p_instance, p_method) ) );
	}
	
	Object();	
	~Object();

};

}

#endif
