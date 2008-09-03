//
// C++ Implementation: pixmap_x11
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "pixmap_x11.h"

namespace GUI {


Error PixmapX11::create(const Size& p_size,PixmapFormat p_format=PIXMAP_FORMAT_RGB) {

	Visual * visual = DefaultVisual( x11_display, 0 );
	
}

Error PixmapX11::load(const String& p_file) {



}

Error PixmapX11::save(const String& p_file) {



}

void PixmapX11::set_pixel(const Point& p_pos,const Color& p_color,unsigned char p_alpha) {



}

Color PixmapX11::get_pixel(const Point& p_pos,unsigned char *p_alpha=NULL) const {



}


Size PixmapX11::get_size() const {


	
}

PixmapFormat PixmapX11::get_format() const {



}

bool PixmapX11::is_valid() const {

	return pixmap!=0;
}


PlatformPixmap *PixmapX11::copy() {



}


PixmapX11::PixmapX11(PlatformX11 *p_platform,Display *p_x11_display) : PlatformPixmap(p_platform) {

	x11_display=p_x11_display;
	x11_pixmap=0;
}


PixmapX11::~PixmapX11()
{
}


}
