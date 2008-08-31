//
// C++ Implementation: pixmap
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "pixmap.h"
#include "base/platform.h"

namespace GUI {


class PixmapPrivate {
public:

	PlatformPixmap *pixmap;
	int refcount;

	PixmapPrivate *copy_on_write() {

		if (refcount==1)
			return this;
		else {
			
			refcount--;
			PixmapPrivate *copy= GUI_NEW( PixmapPrivate );
			
			copy->refcount=1;
			copy->pixmap=pixmap->copy();						
		}
	}

	PixmapPrivate() {

		refcount=1;
		pixmap=NULL;
	}

	~PixmapPrivate() {

		if (pixmap)
			GUI_DELETE( pixmap );
	}
};


PlatformPixmap *Pixmap::get_platform_pixmap() const {

	if (_pp)
		return _pp->pixmap;
	return NULL;
}


Error Pixmap::load(const String& p_file) {
		
	copy_on_write();

	return _pp->pixmap->load( p_file );
}
Error Pixmap::save(const String& p_file) {

	if (!_pp || !_pp->pixmap->is_valid())
		return ERR_UNCONFIGURED;

	return _pp->pixmap->save( p_file );
}
void Pixmap::set_pixel(const Point& p_pos,const Color& p_color,unsigned char p_alpha) {

	if (!_pp || !_pp->pixmap->is_valid())
		return ;

	copy_on_write();

	_pp->pixmap->set_pixel(p_pos,p_color,p_alpha);
}
Color Pixmap::get_pixel(const Point& p_pos,unsigned char *p_alpha) const {

	if (!_pp || !_pp->pixmap->is_valid())
		return Color();
	
	return _pp->pixmap->get_pixel(p_pos,p_alpha);
}

Size Pixmap::get_size() const {

	if (!_pp || !_pp->pixmap->is_valid())
		return Size();
	
	return _pp->pixmap->get_size();
}

PixmapFormat Pixmap::get_format() const {

	return _pp->pixmap->get_format();
}

Error Pixmap::create(const Size& p_size,PixmapFormat p_format) {

	copy_on_write();
	return _pp->pixmap->create(p_size,p_format);	
}

bool Pixmap::is_valid() const {
	
	if (!_pp || !_pp->pixmap->is_valid())
		return false;

	return true;	
}

void Pixmap::copy_on_write() {

	if (!_pp) {	
		_pp = GUI_NEW( PixmapPrivate );
		_pp->pixmap = Platform::get_singleton()->create_pixmap();

	} else {
	
		_pp=_pp->copy_on_write();
	}

}

void Pixmap::reference(const Pixmap& p_pixmap) {

	unreference();

	_pp=p_pixmap._pp;

	if (_pp) {
		
		_pp->refcount++;
	}	
}
void Pixmap::unreference() {

	if (_pp) {

		_pp->refcount--;
		if (_pp->refcount<=0) {

			GUI_DELETE( _pp );
		}
	}

	_pp=NULL;
}

void Pixmap::operator=(const Pixmap& p_pixmap) {
	
	if (&p_pixmap==this)
		return;

	reference(p_pixmap);

}


Pixmap::Pixmap(const Pixmap& p_pixmap) {

	_pp=NULL;
	reference(p_pixmap);
}

Pixmap::Pixmap() {

	_pp=NULL;
}


Pixmap::~Pixmap() {

	unreference();
	
}


} // end of namespace
