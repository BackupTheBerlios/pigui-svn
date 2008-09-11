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

class Object;

class Slot {

	String signal;
friend class Object;
	
	void set_signal(String p_signal);
public:
	
	inline String get_signal() const { return signal; }
	
	virtual Object *get_owner()=0;
	virtual void call();
	virtual void call(const Variant& p1);
	virtual void call(const Variant& p1,const Variant& p2);
	virtual void call(const Variant& p1,const Variant& p2,const Variant& p3);
	virtual void call(const Variant& p1,const Variant& p2,const Variant& p3,const Variant& p4);
	virtual void call(const Variant& p1,const Variant& p2,const Variant& p3,const Variant& p4,const Variant& p5);
	virtual void call(const Variant& p1,const Variant& p2,const Variant& p3,const Variant& p4,const Variant& p5,const Variant& p6);
	
	virtual ~Slot();

};

template<class T>
class Slot0 : public Slot {

	T *instance;
	void (T::*method)();
public:	

	virtual Object *get_owner() { return instance; }
	virtual void call() {
	
		(instance->*method)();
	}	
	
	Slot0( T *p_instance, void (T::*p_method)() ) { instance=p_instance; method=p_method; } 

};

template<class T,class P1>
class Slot1 : public Slot {

	T *instance;
	void (T::*method)(P1);
public:	

	virtual Object *get_owner() { return instance; }
	virtual void call(const Variant& p1) {
	
		P1 arg1 = p1;
		
		if (method)
			(instance->*method)(arg1);
	}	
	
	Slot1( T *p_instance, void (T::*p_method)(P1) ) { instance=p_instance; method=p_method; } 

};

}

#endif
