//
// C++ Interface: pixmap_x11
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef GUIPIXMAP_X11_H
#define GUIPIXMAP_X11_H


#include "drivers/platform.h


namespace GUI {

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class PixmapX11 : public PlatformPixmap {

	::Pixmap x11_pixmap;
	Display *x11_display;

	PixmapX11(PlatformX11 *p_platform,Display *p_x11_display);

public:

	virtual Error create(const Size& p_size,PixmapFormat p_format=PIXMAP_FORMAT_RGB);

	virtual Error load(const String& p_file);
	virtual Error save(const String& p_file);
	virtual void set_pixel(const Point& p_pos,const Color& p_color,unsigned char p_alpha=255);
	virtual Color get_pixel(const Point& p_pos,unsigned char *p_alpha=NULL) const;

	virtual Size get_size() const;
	virtual PixmapFormat get_format() const;
	virtual bool is_valid() const;

	virtual PlatformPixmap *copy();

	PixmapX11(PlatformX11 *);	
	~PixmapX11();

};

}

#endif
