//
// C++ Interface: label
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//

#if 0

#ifndef PIGUITEXT_H
#define PIGUITEXT_H

#include "base/widget.h"

namespace GUI {

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class Text : public Widget {
public:	
	
	enum Align {
		
		ALIGN_LEFT,
		ALIGN_CENTER,
		ALIGN_RIGHT,
		ALIGN_FILL
	};
private:
	Align align;
	String text;

	struct WordCache {

		int char_pos;
		int pixel_width;
	};
	
	WordCache *word_cache;

	virtual void resize(const Size& p_new_size); virtual Size get_minimum_size_internal();
protected:

	virtual String get_type();	
public:
	
	//void mouse_enter() { update(); } for testing
	
	void draw(const Point& p_pos,const Size& p_size,const Rect& p_exposed);
	
	void set_align(Align p_align);
	Align get_align();
	
	void set_text(const String& p_string);
	String get_text();
	
	Text(String p_text="",Align p_align=ALIGN_LEFT);

    ~Text();

};

}

#endif

#endif