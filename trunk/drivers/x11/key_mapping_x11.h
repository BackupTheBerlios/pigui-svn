//
// C++ Interface: key_mapping_x11
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef GUIKEY_MAPPING_X11_H
#define GUIKEY_MAPPING_X11_H

#include <X11/Xlib.h>
#include <X11/XF86keysym.h>
#define XK_MISCELLANY
#define XK_LATIN1
#define XK_XKB_KEYS
#include <X11/keysymdef.h>

#include "base/keyboard.h"

namespace GUI {

class KeyMappingX11 {
	KeyMappingX11() {};
public:
	static unsigned int get_keycode(KeySym p_keysym);
	static KeySym get_keysym(unsigned int p_code);
	static unsigned int get_unicode_from_keysym(KeySym p_keysym);
	static KeySym get_keysym_from_unicode(unsigned int p_unicode);

};

}

#endif
