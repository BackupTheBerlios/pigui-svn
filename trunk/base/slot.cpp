//
// C++ Implementation: slot
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "slot.h"

namespace GUI {

void Slot::set_signal(String p_signal) {
	
	signal=p_signal;
}

void Slot::call() {

	GUI_PRINT_ERROR( String("Signal '"+signal+"' called incorrectly with 0 parameters.").ascii().get_data() );
}
void Slot::call(const Variant& p1) {

	GUI_PRINT_ERROR( String("Signal '"+signal+"' called incorrectly with 1 parameter.").ascii().get_data() );

}
void Slot::call(const Variant& p1,const Variant& p2) {

	GUI_PRINT_ERROR( String("Signal '"+signal+"' called incorrectly with 2 parameters.").ascii().get_data() );

}
void Slot::call(const Variant& p1,const Variant& p2,const Variant& p3) {

	GUI_PRINT_ERROR( String("Signal '"+signal+"' called incorrectly with 3 parameters.").ascii().get_data() );

}
void Slot::call(const Variant& p1,const Variant& p2,const Variant& p3,const Variant& p4) {

	GUI_PRINT_ERROR( String("Signal '"+signal+"' called incorrectly with 4 parameters.").ascii().get_data() );

}
void Slot::call(const Variant& p1,const Variant& p2,const Variant& p3,const Variant& p4,const Variant& p5) {

	GUI_PRINT_ERROR( String("Signal '"+signal+"' called incorrectly with 5 parameters.").ascii().get_data() );

}
void Slot::call(const Variant& p1,const Variant& p2,const Variant& p3,const Variant& p4,const Variant& p5,const Variant& p6) {

	GUI_PRINT_ERROR( String("Signal '"+signal+"' called incorrectly with 6 parameters.").ascii().get_data() );

}


Slot::~Slot()
{
}


}
