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


#include "base/platform.h"
#include <X11/Xlib.h>
#include "X11/extensions/Xrender.h"

namespace GUI {

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/

class PlatformX11;

class PixmapX11 : public PlatformPixmap {

	int best_depth;
	::Pixmap x11_pixmap;
	::Picture xrender_picture;
	Display *x11_display;
	XRenderPictFormat *xrender_format;
	PixmapFormat format;
	Size size;

friend class PlatformX11;
	PixmapX11(PlatformX11 *p_platform,Display *p_x11_display);

public:

	inline ::Pixmap get_x11_pixmap() { return x11_pixmap;  }
	inline ::Picture get_xrender_picture() { return xrender_picture;  }

	virtual Error create(const Size& p_size,PixmapFormat p_format=PIXMAP_FORMAT_RGB);

	virtual Error load_file(const String& p_file);
	virtual Error save_file(const String& p_file);
	
	virtual void load_pixels( const void * p_pixels, const Size& p_size, PixmapFormat p_format );
	virtual void save_pixels( void * p_pixels ) const;
	
	virtual void copy_rect_to( const Rect& p_src_rect,PlatformPixmap * p_to_pixmap,const Point& p_dst_pos ) const;

	virtual Size get_size() const;
	virtual PixmapFormat get_format() const;
	virtual bool is_valid() const;

	virtual PlatformPixmap *copy();

	PixmapX11(PlatformX11 *);	
	~PixmapX11();

};

}

#endif
