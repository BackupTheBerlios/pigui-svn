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

struct SignalInfo {

	String name;
	List<PropertyInfo> arguments;
};

#define GUI_TYPE( m_type, m_inherits )\
private:\
	struct _##m_type##_##_GetSetListProperties {\
\
		void (m_type::*get_property)(const String&,Variant&) const;\
		void (m_type::*set_property)(const String&,Variant&);\
		void (m_type::*get_property_list)(List<PropertyInfo>*) const;\
		void (m_type::*get_signal_list)(List<SignalInfo>*) const;\
\
		_##m_type##_##_GetSetListProperties() {\
\
			get_property=NULL;\
			set_property=NULL;\
			get_property_list=NULL;\
			get_signal_list=NULL;\
		}\
	};\
	_##m_type##_##_GetSetListProperties _##m_type##_get_set_list_properties;\
\
public:\
	virtual String get_type() const { return String(#m_type); }\
	static String get_type_static() { return String(#m_type); }\
	virtual bool is_type(String p_type) const { return (String(#m_type)==p_type)?true:m_inherits::is_type(p_type); }\
	template<class T>\
	T *cast_to() {\
\
	 	if ( is_type(T::get_type_static()) )\
	 		return static_cast<T*>(this);\
	 	else\
	 		return NULL;\
	}\
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
	virtual void _get_signal_listv(List<SignalInfo> *p_list) const { \
		if (_##m_type##_get_set_list_properties.get_signal_list)\
			(this->*_##m_type##_get_set_list_properties.get_signal_list)(p_list);\
		m_type::_get_signal_listv(p_list);\
	}\
\
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
	void connect_get_signal_list(void (m_type::*p_get_signal_list_func)(List<SignalInfo>*) const ) {\
\
		_##m_type##_get_set_list_properties.get_signal_list=p_get_signal_list_func;\
	}\
	
class ObjectPrivate;

class Object {

	ObjectPrivate *op;
	
	void _slot_owner_reference(Object *p_object);
	void _slot_owner_unreference(Object *p_object);
	void _slot_owner_died_callback(Object *p_object);
	
protected:
	
	virtual void _get_propertyv(const String& p_name,Variant &r_property) const;
	virtual void _set_propertyv(const String& p_name,Variant &p_property);
	virtual void _get_property_listv(List<PropertyInfo> *p_list) const;
	virtual void _get_signal_listv(List<SignalInfo> *p_list) const;
	
public:

	virtual String get_type() const;
	static String get_type_static();
	virtual bool is_type(String p_type) const;
	
	void set_name(String p_name);
	String get_name() const;
	
	void set_property(const String& p_name,const Variant& p_property);
	Variant get_property(const String& p_name) const;
	
	void get_property_list(List<PropertyInfo> *p_list) const;	
	
	SlotMagicID connect( const String& p_signal, Slot *p_slot );	
	void disconnect( const String& p_signal, SlotMagicID p_magic_id );	
	
	/* helper handlers to connect to methods up to 6 parameters long */
	
	template<class T>
	void connect( const String& p_signal, T* p_instance, void (T::*p_method)() ) {
	
		connect( p_signal, GUI_NEW(  (Slot0<T>)( p_instance, p_method) ) );
	}
	
	template<class T, class P1>
	void connect( const String& p_signal, T* p_instance, void (T::*p_method)(P1) ) {
	
		connect( p_signal, GUI_NEW(  (Slot1<T,P1>)( p_instance, p_method) ) );
	}

	template<class T, class P1,class P2>
	void connect( const String& p_signal, T* p_instance, void (T::*p_method)(P1,P2) ) {
	
		connect( p_signal, GUI_NEW(  (Slot2<T,P1,P2>)( p_instance, p_method) ) );
	}
	
	template<class T, class P1,class P2,class P3>
	void connect( const String& p_signal, T* p_instance, void (T::*p_method)(P1,P2,P3) ) {
	
		connect( p_signal, GUI_NEW(  (Slot3<T,P1,P2,P3>)( p_instance, p_method) ) );
	}
	
	template<class T, class P1,class P2,class P3, class P4>
	void connect( const String& p_signal, T* p_instance, void (T::*p_method)(P1,P2,P3,P4) ) {
	
		connect( p_signal, GUI_NEW(  (Slot4<T,P1,P2,P3,P4>)( p_instance, p_method) ) );
	}
	
	template<class T, class P1,class P2,class P3, class P4, class P5>
	void connect( const String& p_signal, T* p_instance, void (T::*p_method)(P1,P2,P3,P4,P5) ) {
	
		connect( p_signal, GUI_NEW(  (Slot5<T,P1,P2,P3,P4,P5>)( p_instance, p_method) ) );
	}
	
	template<class T, class P1,class P2,class P3, class P4, class P5, class P6>
	void connect( const String& p_signal, T* p_instance, void (T::*p_method)(P1,P2,P3,P4,P5,P6) ) {
	
		connect( p_signal, GUI_NEW(  (Slot6<T,P1,P2,P3,P4,P5,P6>)( p_instance, p_method) ) );
	}

	
	template<class T, class M>
	void disconnect( const String& p_signal, T* p_class, M p_method ) {
	
		disconnect( p_signal, _method_to_magic_id(p_class,p_method) );
	}

	/* Calling */
	
	void call(String p_signal);
	void call(String p_signal,const Variant& p1);
	void call(String p_signal,const Variant& p1,const Variant& p2);
	void call(String p_signal,const Variant& p1,const Variant& p2,const Variant& p3);
	void call(String p_signal,const Variant& p1,const Variant& p2,const Variant& p3,const Variant& p4);
	void call(String p_signal,const Variant& p1,const Variant& p2,const Variant& p3,const Variant& p4,const Variant& p5);
	void call(String p_signal,const Variant& p1,const Variant& p2,const Variant& p3,const Variant& p4,const Variant& p5,const Variant& p6);

	Object();	
	~Object();

};

}

#endif
