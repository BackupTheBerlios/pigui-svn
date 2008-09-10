//
// C++ Interface: atoms_x11
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef GUIATOMS_X11_H
#define GUIATOMS_X11_H

#include <X11/Xlib.h>
#include "base/pstring.h"

namespace GUI {

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class AtomsX11 {

	AtomsX11() {}
	
friend class PlatformX11;

	static ::Display *x11_display;
	
public:

	static bool has_atom(String p_atom);
	static Atom get_atom(String p_atom);
	static Atom create_atom(String p_atom);

	static void set_netwm_single_state( ::Window p_window, String p_name,bool p_state);
	static void request_active_window( ::Window p_who, ::Window p_which );


};

}

#endif
