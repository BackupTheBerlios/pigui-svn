//
// C++ Interface: pixmap
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef GUIPIXMAP_H
#define GUIPIXMAP_H

#include "base/geometry.h"
#include "base/constants.h"
#include "base/pstring.h"

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/

namespace GUI {

class PixmapPrivate;
class PlatformPixmap;

class Pixmap {
	
	mutable PixmapPrivate *_pp;
	
	void copy_on_write();
	void reference(const Pixmap& p_pixmap);
	void unreference();

friend class Platform;

	PlatformPixmap *get_platform_pixmap() const;

public:

	Error create(const Size& p_size,PixmapFormat p_format=PIXMAP_FORMAT_RGB);
	
	Error load_file(const String& p_file);
	Error save_file(const String& p_file);

	void load_pixels( const void * p_pixels, const Size& p_size, PixmapFormat p_format );
	void save_pixels( void * p_pixels ) const;	

	void copy_rect_to( const Rect& p_src_rect,Pixmap &p_pixmap,const Point& p_dst_pos ) const;

	Size get_size() const;
	PixmapFormat get_format() const;

	bool is_valid() const;

	void operator=(const Pixmap& p_pixmap);
	Pixmap(const Pixmap& p_pixmap);
	Pixmap();
	~Pixmap();

};

} // end of namespace
#endif
