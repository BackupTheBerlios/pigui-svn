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

	struct SlotOwner {
	
		Object *object;
		int refcount;		
		SlotOwner() { refcount=0; }	
	};
		
	int last_id;
	List<Slot*> slot_list;
	List<SlotOwner> _referenced_slot_owners;
	String name;
};

void Object::_get_property_listv(List<PropertyInfo> *p_list) const {

	p_list->push_back( PropertyInfo( Variant::STRING, "name", PROPERTY_HINT_LENGTH, "256" ) );
}

void Object::_get_signal_listv(List<SignalInfo> *p_list) const {


}

void Object::_get_propertyv(const String& p_name,Variant &r_property) const {

	if (p_name=="name")
		r_property=op->name;

}

void Object::_set_propertyv(const String& p_name,Variant &p_property) {

	if (p_name=="name")
		op->name=p_property;

}

String Object::get_type() const {

	return "Object";
}

String Object::get_type_static() {

	return "Object";
}

bool Object::is_type(String p_type) const {

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

void Object::_slot_owner_reference(Object *p_object) {

	// as an owner, we are notified that a slot points here

	List<ObjectPrivate::SlotOwner>::Element *I= op->_referenced_slot_owners.begin();
	
	while(I) {
	
		if (I->get().object==p_object) {
		
			I->get().refcount++;
			return;
		};
		I=I->next();
	}
	
	ObjectPrivate::SlotOwner new_owner;
	
	new_owner.refcount=1;
	new_owner.object=p_object;
	
	op->_referenced_slot_owners.push_back(new_owner);
}

void Object::_slot_owner_unreference(Object *p_object) {

	// as an owner, we are notified that a slot no longer points here
	
	List<ObjectPrivate::SlotOwner>::Element *I= op->_referenced_slot_owners.begin();
	
	while(I) {
	
		if (I->get().object==p_object) {
		
			I->get().refcount--;
			if( I->get().refcount<=0 ) {
			
				op->_referenced_slot_owners.erase(I);
			}
			
			return;
		};
		I=I->next();
	}
}

void Object::_slot_owner_died_callback(Object *p_object) {

	// erase everything that connects to the dead owner	
	List<Slot*>::Element *I=op->slot_list.begin(),*N;
	
	while (I) {
	
		N=I->next();
		
		if (I->get()->get_owner()==p_object) {
			/* already have this */
			GUI_DELETE( I->get() );
			op->slot_list.erase(I);
		}
		I=N;
	}	
}

SlotMagicID Object::connect( const String& p_signal, Slot *p_slot ) {

	if (!p_slot->get_owner()) {
	
		GUI_DELETE(p_slot);
		GUI_PRINT_ERROR("Slot Lacks Owner!");
		return SlotMagicID();
	}
	
	SlotMagicID magic_id = p_slot->get_magic_id();
	
	List<Slot*>::Element *I=op->slot_list.begin();
	
	while (I) {
	
		if (I->get()->get_signal() == p_signal && I->get()->get_magic_id()==magic_id) {
			/* already have this */
			GUI_DELETE( p_slot );
			return SlotMagicID();
		}
		I=I->next();
	}
		
	p_slot->set_signal(p_signal);
	
	if (p_slot->get_owner()!=this)
		p_slot->get_owner()->_slot_owner_reference(this);
	
	op->slot_list.push_back(p_slot);
	
	return magic_id;

}

void Object::disconnect( const String& p_signal, SlotMagicID p_magic_id ) {

	
	List<Slot*>::Element *I=op->slot_list.begin();
	
	while (I) {
	
		if (I->get()->get_signal() == p_signal && I->get()->get_magic_id()==p_magic_id) {
			/* already have this */
			if (I->get()->get_owner()!=this)
				I->get()->get_owner()->_slot_owner_unreference(this);
			GUI_DELETE( I->get() );
			op->slot_list.erase( I );
			return;
		}
		I=I->next();
	}

}

void Object::call(String p_signal) {

	List<Slot*>::Element *I=op->slot_list.begin();
	
	while (I) {
	
		if (I->get()->get_signal() == p_signal) {
			I->get()->call();
		}
		I=I->next();
	}

}
void Object::call(String p_signal,const Variant& p1) {
	
	List<Slot*>::Element *I=op->slot_list.begin();
	
	while (I) {
	
		if (I->get()->get_signal() == p_signal) {
			I->get()->call(p1);
		}
		I=I->next();
	}
	
}
void Object::call(String p_signal,const Variant& p1,const Variant& p2) {

	List<Slot*>::Element *I=op->slot_list.begin();
	
	while (I) {
	
		if (I->get()->get_signal() == p_signal) {
			I->get()->call(p1,p2);
		}
		I=I->next();
	}

}
void Object::call(String p_signal,const Variant& p1,const Variant& p2,const Variant& p3) {

	List<Slot*>::Element *I=op->slot_list.begin();
	
	while (I) {
	
		if (I->get()->get_signal() == p_signal) {
			I->get()->call(p1,p2,p3);
		}
		I=I->next();
	}

}
void Object::call(String p_signal,const Variant& p1,const Variant& p2,const Variant& p3,const Variant& p4) {

	List<Slot*>::Element *I=op->slot_list.begin();
	
	while (I) {
	
		if (I->get()->get_signal() == p_signal) {
			I->get()->call(p1,p2,p3,p4);
		}
		I=I->next();
	}

}
void Object::call(String p_signal,const Variant& p1,const Variant& p2,const Variant& p3,const Variant& p4,const Variant& p5) {

	List<Slot*>::Element *I=op->slot_list.begin();
	
	while (I) {
	
		if (I->get()->get_signal() == p_signal) {
			I->get()->call(p1,p2,p3,p4,p5);
		}
		I=I->next();
	}

}
void Object::call(String p_signal,const Variant& p1,const Variant& p2,const Variant& p3,const Variant& p4,const Variant& p5,const Variant& p6) {

	List<Slot*>::Element *I=op->slot_list.begin();
	
	while (I) {
	
		if (I->get()->get_signal() == p_signal) {
			I->get()->call(p1,p2,p3,p4,p5,p6);
		}
		I=I->next();
	}

}

Object::Object() {

	op = GUI_NEW( ObjectPrivate );
	op->last_id=1;
}


Object::~Object() {

	

	{ /* Notify every object we are owner of its slots that we died */
	
		List<ObjectPrivate::SlotOwner>::Element *I= op->_referenced_slot_owners.begin();
		
		while(I) {
		
			I->get().object->_slot_owner_died_callback(this);
			I=I->next();
		}
	}
	
	 { /* Erase all slots */
	
		List<Slot*>::Element *I=op->slot_list.begin();
		
		while (I) {
		
			GUI_DELETE( I->get() );
			I=I->next();
		}
	}	
	
	
	GUI_DELETE( op );
}


}
