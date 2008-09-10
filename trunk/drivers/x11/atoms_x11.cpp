//
// C++ Implementation: atoms_x11
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "atoms_x11.h"
#include <stdio.h>

namespace GUI {


::Display *AtomsX11::x11_display=NULL;

bool AtomsX11::has_atom(String p_atom) {

	Atom atom = XInternAtom(x11_display, p_atom.ascii().get_data(), true);
	
	return atom!=None;

}
Atom AtomsX11::get_atom(String p_atom) {

	return XInternAtom(x11_display, p_atom.ascii().get_data(), true);

}

Atom AtomsX11::create_atom(String p_atom) {

	return XInternAtom(x11_display, p_atom.ascii().get_data(), false);

}

void AtomsX11::set_netwm_single_state( ::Window p_window, String p_name,bool p_state) {


	XClientMessageEvent clientmsg;
	
	clientmsg.type=ClientMessage;
	clientmsg.serial=0;
	clientmsg.display=x11_display;
	clientmsg.window=p_window;
	clientmsg.message_type=get_atom("_NET_WM_STATE");
	clientmsg.format=32;
	clientmsg.data.l[0]=p_state?get_atom("_NET_WM_STATE_ADD"):get_atom("_NET_WM_STATE_REMOVE");
	clientmsg.data.l[1]=get_atom(p_name);
	clientmsg.data.l[2]=0;
	
	XSendEvent (x11_display, p_window, false,SubstructureRedirectMask | SubstructureNotifyMask,(XEvent*)&clientmsg);

}

void AtomsX11::request_active_window( ::Window p_who, ::Window p_which ) {
	
	XClientMessageEvent clientmsg;

	clientmsg.type=ClientMessage;
	clientmsg.serial=0;
	clientmsg.display=x11_display;
	clientmsg.window=p_which;
	clientmsg.message_type=get_atom("_NET_ACTIVE_WINDOW");
	clientmsg.format=32;
	clientmsg.data.l[0]=1;
	clientmsg.data.l[1]=0;
	clientmsg.data.l[2]=p_who;

	XSendEvent (x11_display, p_which, false,SubstructureRedirectMask | SubstructureNotifyMask,(XEvent*)&clientmsg);
}

}
