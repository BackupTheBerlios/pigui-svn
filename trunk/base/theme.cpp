//
// C++ Implementation: theme
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "theme.h"

namespace GUI {


class ThemePrivate {
public:

	enum {
	
		HASHTABLE_BITS=9,
		HASHTABLE_LEN=1<<HASHTABLE_BITS,
		HASHTABLE_MASK=HASHTABLE_LEN-1
	};

	struct ThemeVariable {
	
		int id;	
		String name;
		Variant variant;
		ThemeVariable *next;
		ThemeVariable() { next=NULL; }
	};
	
	
	
	ThemeVariable *hashtable[HASHTABLE_LEN];


	ThemeVariable *find(int p_id) {
	
		if (p_id<0)
			return NULL;
		int bucket = p_id & HASHTABLE_MASK;
		
		ThemeVariable *v = hashtable[bucket];
		
		while (v) {
		
			if (v->id==p_id)
				return v;
				
			v=v->next;
		}
			
		return NULL;
	}
	
	void clear() {
	
		for (int i=0;i<HASHTABLE_LEN;i++) {
		
			while(hashtable[i]) { 
			
				ThemeVariable *tv = hashtable[i];
				hashtable[i]=hashtable[i]->next;
				GUI_DELETE(tv);
			}
		}
	
	}
	
	ThemePrivate() {
	
		for (int i=0;i<HASHTABLE_LEN;i++) {
		
			hashtable[i]=NULL;
		}
	}
	
	
	~ThemePrivate() {
	
		clear();	
	}

};


void Theme::set_var( int p_id, const Variant& p_var) {

	if (p_id<0) {
	
		GUI_PRINT_ERROR("Negative ID is invalid.");
		return;
	}
	
	ThemePrivate::ThemeVariable * v = tp->find(p_id);
	
	if (v) {
	
		v->variant=p_var;
	} else {
	
		v = GUI_NEW( ThemePrivate::ThemeVariable );
		v->id=p_id;
		v->variant=p_var;
		int bucket = p_id & ThemePrivate::HASHTABLE_MASK;
		
		v->next = tp->hashtable[bucket];
		tp->hashtable[bucket]=v;
		
	}
}
void Theme::set_var_name( int p_id, String p_name ) {

	ThemePrivate::ThemeVariable * v = tp->find(p_id);
	
	if (v) {
	
		v->name=p_name;
	}	
}
const Variant& Theme::get_var( int p_id ) const {

	ThemePrivate::ThemeVariable * v = tp->find(p_id);
	if (v) {
	
		return v->variant;
	}
	static Variant empty;
	return empty;
}
String Theme::get_var_name( int p_id ) const {

	ThemePrivate::ThemeVariable * v = tp->find(p_id);
	if (v) {
	
		return v->name;
	}
	
	return "";
}

Theme::Theme() {

	tp = GUI_NEW( ThemePrivate );
}


Theme::~Theme() {

	GUI_DELETE( tp );
}


}
