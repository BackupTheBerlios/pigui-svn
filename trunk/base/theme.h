//
// C++ Interface: theme
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef GUI_THEME_H
#define GUI_THEME_H


#include "base/variant.h"

namespace GUI {
/**
	@author Juan Linietsky <reduzio@gmail.com>
*/

class ThemePrivate;

enum ThemeDefaultID {

	SB_FIRST=0,
	SB_CONTAINER=SB_FIRST+1,
	SB_ROOT_CONTAINER=SB_FIRST+2,
	SB_POPUP_CONTAINER=SB_FIRST+3,	
	PIXMAP=0x1000,


	FONT_FIRST=0x2000,

	FONT_LABEL=FONT_FIRST+1,

	C_FIRST=0x3000,

	COLOR_FIRST=0x3000,

	COLOR_LABEL=COLOR_FIRST+1,

	USER_FIRST=0x10000
};



class Theme {
	
	ThemePrivate *tp;
	
public:
	
	void set_var( int p_id, const Variant& p_id);
	void set_var_name( int p_id, String p_name );
	const Variant& get_var( int p_id ) const;
	String get_var_name( int p_id ) const;

	Theme();	
	~Theme();

};

}
#endif
