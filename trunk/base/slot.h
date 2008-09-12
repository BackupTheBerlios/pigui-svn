//
// C++ Interface: slot
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef GUISLOT_H
#define GUISLOT_H

#include "base/variant.h"
#include "base/constants.h"


namespace GUI {

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/

class __UnexistingClassPrototype;

// a strange but useful flaw of the C++ standard. This creates a method
// pointer as big as it can be.

typedef void (__UnexistingClassPrototype::*__MethodPrototype)();

struct SlotMagicID {

	enum {
	
		LENGTH=(sizeof(void*)+sizeof(__MethodPrototype))
	};
	
	unsigned char data[LENGTH];
	
	bool operator==(const SlotMagicID& p_marshall) const {
		

		for (unsigned int i=0;i<LENGTH;i++) {
			if (data[i]!=p_marshall.data[i]) {
				return false;
			}
		}
		
		return true;
	}	
		
	bool operator!=(const SlotMagicID& p_marshall) const {
	
		return !operator==(p_marshall);
	}

	void clear() {
	
		for (unsigned int i=0;i<LENGTH;i++)
			data[i]=0;	
	}
	
	SlotMagicID() {
	
		clear();
	}
};



// .. created with this! ..
template<class T, class M>
SlotMagicID _method_to_magic_id(T* p_instance, M p_method) {

	union {
	 
	 	struct {
	 		T *instance;
	 		M method;
	 	};
	 
	 	unsigned char data[SlotMagicID::LENGTH];
	} u;
	
	for (int i=0;i<SlotMagicID::LENGTH;i++) {
		
		u.data[i]=0;
	}
	u.instance=p_instance;
	u.method=p_method;
	
	SlotMagicID magicid;
	
	for (int i=0;i<SlotMagicID::LENGTH;i++) {
		
		magicid.data[i]=u.data[i];
	}
	
	return magicid;  
}


class Slot {
	
	String signal;
friend class Object;
	
	void set_signal(String p_signal);
public:
	
	
	inline String get_signal() const { return signal; }
	virtual Object *get_owner() const=0;
		
	virtual void call();
	virtual void call(const Variant& p1);
	virtual void call(const Variant& p1,const Variant& p2);
	virtual void call(const Variant& p1,const Variant& p2,const Variant& p3);
	virtual void call(const Variant& p1,const Variant& p2,const Variant& p3,const Variant& p4);
	virtual void call(const Variant& p1,const Variant& p2,const Variant& p3,const Variant& p4,const Variant& p5);
	virtual void call(const Variant& p1,const Variant& p2,const Variant& p3,const Variant& p4,const Variant& p5,const Variant& p6);
		
	virtual SlotMagicID get_magic_id() const=0;
		
	virtual ~Slot();

};

template<class T>
class Slot0 : public Slot {

	T *instance;
	void (T::*method)();
public:	

	virtual Object *get_owner() const { return instance; }

	virtual void call() {
	
		(instance->*method)();
	}	
	
	Slot0( T *p_instance, void (T::*p_method)() ) { instance=p_instance; method=p_method; } 

	virtual SlotMagicID get_magic_id() const { return _method_to_magic_id(instance,method); }
	Slot0() { instance=NULL; method=NULL; }
};


template<class T,class P1>
class Slot1 : public Slot {

	T *instance;
	void (T::*method)(P1);
public:	

	virtual Object *get_owner() const { return instance; }
	virtual void call(const Variant& p1) {
			
		/* If you see a compile-time error here, it means
		   you are connecting to a member function with 
		   other types of parameters than the ones explicitly 
		   defined in variant.h.
		*/
		   
		P1 arg1;
		arg1=p1;
			
		if (method)
			(instance->*method)(arg1);
	}	
	
	Slot1( T *p_instance, void (T::*p_method)(P1) ) { instance=p_instance; method=p_method; } 

	virtual SlotMagicID get_magic_id() const { return _method_to_magic_id(instance,method); }
	Slot1() { instance=NULL; method=NULL; }

};

template<class T,class P1, class P2>
class Slot2 : public Slot {

	T *instance;
	void (T::*method)(P1,P2);
public:	

	virtual Object *get_owner() const { return instance; }
	virtual void call(const Variant& p1,const Variant& p2) {
			
		/* If you see a compile-time error here, it means
		   you are connecting to a member function with 
		   other types of parameters than the ones explicitly 
		   defined in variant.h.
		*/
			
		P1 arg1;
		arg1=p1; // avoid references from working
		P2 arg2;
		arg2=p2; // avoid references from working
			
		if (method)
			(instance->*method)(arg1,arg2);
	}	
	
	Slot2( T *p_instance, void (T::*p_method)(P1,P2) ) { instance=p_instance; method=p_method; } 

	virtual SlotMagicID get_magic_id() const { return _method_to_magic_id(instance,method); }
	Slot2() { instance=NULL; method=NULL; }

};

template<class T,class P1, class P2,class P3>
class Slot3 : public Slot {

	T *instance;
	void (T::*method)(P1,P2,P3);
public:	

	virtual Object *get_owner() const { return instance; }
	virtual void call(const Variant& p1,const Variant& p2,const Variant& p3) {
			
		/* If you see a compile-time error here, it means
		   you are connecting to a member function with 
		   other types of parameters than the ones explicitly 
		   defined in variant.h.
		*/
			
		P1 arg1;
		arg1=p1;
		P2 arg2;
		arg2=p2;
		P3 arg3;
		arg3=p3;
			
		if (method)
			(instance->*method)(arg1,arg2,arg3);
	}	
	
	Slot3( T *p_instance, void (T::*p_method)(P1,P2,P3) ) { instance=p_instance; method=p_method; } 

	virtual SlotMagicID get_magic_id() const { return _method_to_magic_id(instance,method); }
	Slot3() { instance=NULL; method=NULL; }

};

template<class T,class P1, class P2,class P3,class P4>
class Slot4 : public Slot {

	T *instance;
	void (T::*method)(P1,P2,P3,P4);
public:	

	virtual Object *get_owner() const { return instance; }
	virtual void call(const Variant& p1,const Variant& p2,const Variant& p3,const Variant& p4) {
			
		/* If you see a compile-time error here, it means
		   you are connecting to a member function with 
		   other types of parameters than the ones explicitly 
		   defined in variant.h.
		*/
			
		P1 arg1;
		arg1=p1;
		P2 arg2;
		arg2=p2;
		P3 arg3;
		arg3=p3;
		P4 arg4;
		arg4=p4;
			
		if (method)
			(instance->*method)(arg1,arg2,arg3,arg4);
	}	
	
	Slot4( T *p_instance, void (T::*p_method)(P1,P2,P3,P4) ) { instance=p_instance; method=p_method; } 

	virtual SlotMagicID get_magic_id() const { return _method_to_magic_id(instance,method); }
	Slot4() { instance=NULL; method=NULL; }

};


template<class T,class P1, class P2,class P3,class P4,class P5>
class Slot5 : public Slot {

	T *instance;
	void (T::*method)(P1,P2,P3,P4,P5);
public:	

	virtual Object *get_owner() const { return instance; }
	virtual void call(const Variant& p1,const Variant& p2,const Variant& p3,const Variant& p4,const Variant& p5) {
			
		/* If you see a compile-time error here, it means
		   you are connecting to a member function with 
		   other types of parameters than the ones explicitly 
		   defined in variant.h.
		*/
			
		P1 arg1;
		arg1=p1;
		P2 arg2;
		arg2=p2;
		P3 arg3;
		arg3=p3;
		P4 arg4;
		arg4=p4;
		P5 arg5;
		arg5=p5;
			
		if (method)
			(instance->*method)(arg1,arg2,arg3,arg4,arg5);
	}	
	
	Slot5( T *p_instance, void (T::*p_method)(P1,P2,P3,P4,P5) ) { instance=p_instance; method=p_method; } 

	virtual SlotMagicID get_magic_id() const { return _method_to_magic_id(instance,method); }
	Slot5() { instance=NULL; method=NULL; }

};


template<class T,class P1, class P2,class P3,class P4,class P5,class P6>
class Slot6 : public Slot {

	T *instance;
	void (T::*method)(P1,P2,P3,P4,P5);
public:	

	virtual Object *get_owner() const { return instance; }
	virtual void call(const Variant& p1,const Variant& p2,const Variant& p3,const Variant& p4,const Variant& p5,const Variant& p6) {
			
		/* If you see a compile-time error here, it means
		   you are connecting to a member function with 
		   other types of parameters than the ones explicitly 
		   defined in variant.h.
		*/
			
		P1 arg1;
		arg1=p1;
		P2 arg2;
		arg2=p2;
		P3 arg3;
		arg3=p3;
		P4 arg4;
		arg4=p4;
		P5 arg5;
		arg5=p5;
		P6 arg6;
		arg6=p6;
			
		if (method)
			(instance->*method)(arg1,arg2,arg3,arg4,arg5,arg6);
	}	
	
	Slot6( T *p_instance, void (T::*p_method)(P1,P2,P3,P4,P5,P6) ) { instance=p_instance; method=p_method; } 

	virtual SlotMagicID get_magic_id() const { return _method_to_magic_id(instance,method); }
	Slot6() { instance=NULL; method=NULL; }

};
}

#endif
