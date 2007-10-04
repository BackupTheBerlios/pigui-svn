//
// C++ Implementation: window_box
//
// Description: 
//
//
// Author: Juan Linietsky <reduz@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "widgets/window_top.h"
#include "widgets/window_button.h"
#include "window_box.h"


namespace GUI {


void WindowBox::set_in_window() {
	
	spacer->set_minimum_size( Size( 0, constant( C_WINDOWBOX_SPACER ) ) );
}

WindowTop * WindowBox::get_window_top() {
	
	return top;
}

void WindowBox::add_frame(Frame *p_frame,int p_stretch) {

	internal_vb->add_frame(p_frame,p_stretch);
}


HBoxContainer *WindowBox::get_top_hb() {
	
	return top_hb;
}

WindowBox::WindowBox(String p_top_text,bool p_close,bool p_resize) {

	top_hb = BoxContainer::add( new HBoxContainer, 0 );
	
	top=top_hb->add( new WindowTop(p_top_text),1);
	
	if (p_close) {
		
		top_hb->add( new WindowButton(WindowButton::MODE_CLOSE) );
	}

	spacer = BoxContainer::add( new Widget, 0 );
	internal_vb=BoxContainer::add( new VBoxContainer, 1);
	
}


WindowBox::~WindowBox()
{
}


}
