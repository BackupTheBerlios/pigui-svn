//
// C++ Implementation: style_box
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "style_box.h"
#include "base/painter.h"

namespace GUI {

void StyleBoxMethod::set_custom_margins( int p_left, int p_top, int p_right, int p_bottom) {

	custom_margins[MARGIN_LEFT]=p_left;
	custom_margins[MARGIN_TOP]=p_top;
	custom_margins[MARGIN_RIGHT]=p_right;
	custom_margins[MARGIN_BOTTOM]=p_bottom;

}
int StyleBoxMethod::get_custom_margin( Margin p_margin ) const {
	
	return custom_margins[p_margin];
}
void StyleBoxMethod::clear_custom_margins() {

	for (int i=0;i<4;i++) {
	
		custom_margins[i]=-1;
	}
}



void StyleBox::reference( const StyleBox& p_stylebox ) {

	unreference();
	
	method=p_stylebox.method;
	
	if (method)
		method->refcount++;
}
void StyleBox::unreference() {

	if (method) {
		method->refcount--;
		if (method->refcount<=0) {
			GUI_DELETE( method );
			method=NULL;
		}			
	}
}

void StyleBox::operator=(const StyleBox& p_stylebox) {

	reference(p_stylebox);
}
	
void StyleBox::draw(Painter *p_painter, const Rect& p_rect) const {

}
int StyleBox::get_margin(Margin p_margin) const {

	if (method) {
	
		int custom =  method->get_custom_margin(p_margin);
		if (custom>=0)
			return custom;
			
		return method->get_margin(p_margin);
	}
	
	return 0;
}

Size StyleBox::get_minimum_size() const {

	Size minsize;
	minsize.x=get_margin(MARGIN_LEFT) + get_margin(MARGIN_RIGHT);
	minsize.y=get_margin(MARGIN_TOP) + get_margin(MARGIN_BOTTOM);
	
	return minsize;
}
Rect StyleBox::enter_rect(const Rect& p_rect) const {

	int margins[4];
	margins[MARGIN_LEFT] = get_margin(MARGIN_LEFT);
	margins[MARGIN_RIGHT] = get_margin(MARGIN_RIGHT);
	margins[MARGIN_TOP] = get_margin(MARGIN_TOP);
	margins[MARGIN_BOTTOM] = get_margin(MARGIN_BOTTOM);
	
	Rect rect=p_rect;
	rect.pos+=Point( margins[MARGIN_LEFT] , margins[MARGIN_TOP] );
	rect.size-=Point( margins[MARGIN_LEFT] , margins[MARGIN_TOP] )+Point( margins[MARGIN_RIGHT] , margins[MARGIN_BOTTOM] );
	
	return rect;

}

StyleBox::StyleBox() {

	method=NULL;
}
StyleBox::StyleBox(const StyleBox& p_stylebox) {

	method=NULL;
	reference(p_stylebox);


}
StyleBox::StyleBox(StyleBoxMethod *p_method) {

	if (!p_method)
		return;
	if (p_method->refcount>0) {
	
		GUI_PRINT_ERROR("StyleBoxMethod::refcount > 0. Likely in use somewhere else.");
		return;
	}
	
	method=p_method;
	method->refcount++;
}

StyleBox::~StyleBox() {

	unreference();
}

void StyleBoxFlatMethod::draw(Painter *p_painter,const Rect& p_rect) const {


	Rect r=p_rect;

	int blend = margin > 1;
	
	for (int i=0;i<margin;i++) {



		Color _color_top_left=color_top_left;
		Color _color_bottom_right=color_bottom_right;

		if (blend) {

			_color_top_left.r=(margin-i)*(int)_color_top_left.r/margin + i*(int)color_center.r/margin;
			_color_top_left.g=(margin-i)*(int)_color_top_left.g/margin + i*(int)color_center.g/margin;
			_color_top_left.b=(margin-i)*(int)_color_top_left.b/margin + i*(int)color_center.b/margin;
			_color_top_left.a=(margin-i)*(int)_color_top_left.a/margin + i*(int)color_center.a/margin;

			_color_bottom_right.r=(margin-i)*(int)_color_bottom_right.r/margin + i*(int)color_center.r/margin;
			_color_bottom_right.g=(margin-i)*(int)_color_bottom_right.g/margin + i*(int)color_center.g/margin;
			_color_bottom_right.b=(margin-i)*(int)_color_bottom_right.b/margin + i*(int)color_center.b/margin;
			_color_bottom_right.a=(margin-i)*(int)_color_bottom_right.a/margin + i*(int)color_center.a/margin;

		}

		p_painter->draw_rect( Point( r.pos.x,r.pos.y+r.size.y-1), Size(r.size.x ,1 ), _color_bottom_right );
		p_painter->draw_rect( Point( r.pos.x+r.size.x-1,r.pos.y ), Size( 1 ,r.size.y ), _color_bottom_right);

		p_painter->draw_rect( r.pos, Size(r.size.x ,1 ), _color_top_left);
		p_painter->draw_rect( r.pos, Size( 1 ,r.size.y ), _color_top_left);


		r.pos.x++;
		r.pos.y++;
		r.size.x-=2;
		r.size.y-=2;
	}

	if (draw_center)
		p_painter->draw_rect( r.pos, r.size , color_center );

}
int StyleBoxFlatMethod::get_margin(Margin p_margin) const {

	return margin;
}

StyleBoxFlatMethod::StyleBoxFlatMethod( int p_margin, const Color& p_color_center, const Color& p_color_top_left, const Color& p_color_bottom_right,bool p_draw_center ) {

	if (p_margin<0)
		p_margin=0;

	margin=p_margin;
	color_center=p_color_center;
	color_top_left=p_color_top_left;
	color_bottom_right=p_color_bottom_right;
	draw_center=p_draw_center;
	
}

#if 0

StyleBox::Mode StyleBox::get_mode() const {

	return mode;
}

void StyleBox::clear() {

	mode=MODE_NONE;
	flat.color_center=Color();
	flat.color_top_left=Color();
	flat.color_bottom_right=Color();
	flat.margin=0;
	
	for (int i=0;i<STYLEBOX_PIXMAPS_MAX;i++) {
	
		pixmaps[i]=Pixmap();
	}
	
	userdraw.func=NULL;
	userdraw.get_margin_func=NULL;
	userdraw.data=NULL;
	
	for (int i=0;i<4;i++) {
	
		custom_margins[i]=-1;
	}
	
}

/* FLAT */



void StyleBox::create_flat(int p_margin, const Color& p_color_center, const Color& p_color_top_left, const Color& p_color_bottom_right ) {

	clear();
	
	mode=MODE_FLAT;
	flat.margin=p_margin;
	flat.color_center=p_color_center;
	flat.color_top_left=p_color_top_left;
	flat.color_bottom_right=p_color_bottom_right;

}

int StyleBox::get_flat_margin() const {

	return flat.margin;
}
Color StyleBox::get_flat_color_center() const {

	return flat.color_center;
}
Color StyleBox::get_flat_color_top_left() const {

	return flat.color_top_left;
}
Color StyleBox::get_flat_color_bottom_right() const {

	return flat.color_bottom_right;
}

/* FLAT PIXMAP */

void StyleBox::create_flat_pixmap( Pixmap *p_8_pixmaps, const Color& p_color_center ) {

	clear();
	
	mode=MODE_FLAT_PIXMAP;
	
	for (int i=0;i<4;i++) {
		pixmaps[i]=p_8_pixmaps[i];
		pixmaps[i+5]=p_8_pixmaps[i+4];
	}
	flat.color_center=p_color_center;
}
void StyleBox::create_flat_pixmap(const Pixmap& p_pixmap, int p_left, int p_top, int p_right, int p_botton, const Color& p_color_center) {


}
	
/* PIXMAP */
	
void StyleBox::create_pixmap( Pixmap *p_9_pixmaps ) {

	clear();
	
	mode=MODE_PIXMAP;
	
	for (int i=0;i<9;i++) {
		pixmaps[i]=p_9_pixmaps[i];
	}

}
void StyleBox::create_pixmap(const Pixmap& p_pixmap, int p_left, int p_top, int p_right, int p_botton) {


}

Pixmap StyleBox::get_pixmap(StyleBoxPixmaps p_pixmap) const {

	if (p_pixmap>0 || p_pixmap >= STYLEBOX_PIXMAPS_MAX ) {
		GUI_PRINT_ERROR("Invalid Pixmap index.");
		return Pixmap();
	}
	
	return pixmaps[p_pixmap];
}
	
void StyleBox::create_userdraw( StyleBoxUserDrawFunc p_draw_func, StyleBoxUserDrawGetMarginFunc p_get_margin_func, void * p_userdata ) {

	clear();
	
	mode=MODE_USERDRAW;
	
	userdraw.func=p_draw_func;
	userdraw.get_margin_func=p_get_margin_func;
	userdraw.data=p_userdata;
}

StyleBoxUserDrawFunc StyleBox::get_usedraw_func() const {

	return userdraw.func;
}
StyleBoxUserDrawGetMarginFunc StyleBox::get_get_margin_func() const {

	return userdraw.get_margin_func;
}
void *StyleBox::get_usedraw_data() const {

	return userdraw.data;
}


void StyleBox::set_custom_margins( int p_left, int p_top, int p_right, int p_bottom) {

	custom_margins[MARGIN_LEFT]=p_left;
	custom_margins[MARGIN_TOP]=p_top;
	custom_margins[MARGIN_RIGHT]=p_right;
	custom_margins[MARGIN_BOTTOM]=p_bottom;

}

int StyleBox::get_custom_margin( Margin p_margin ) const {

	return custom_margins[p_margin];
}
void StyleBox::clear_custom_margins() {

	for (int i=0;i<4;i++) {
	
		custom_margins[i]=-1;
	}
}

int StyleBox::get_margin( Margin p_margin ) const {

	if (custom_margins[p_margin]>=0)
		return custom_margins[p_margin]>=0;
		
	switch(mode) {
	
		case MODE_NONE: {
		
			return 0;
		} break;
		case MODE_FLAT: {
		
			return flat.margin;
		} break;
		case MODE_FLAT_PIXMAP:
		case MODE_PIXMAP: {
			
			switch( p_margin ) {
				case MARGIN_LEFT: {
			
					if (pixmaps[STYLEBOX_PIXMAPS_CENTER_LEFT].is_valid())
						return pixmaps[STYLEBOX_PIXMAPS_CENTER_LEFT].get_size().x;
				} break;
				case MARGIN_RIGHT: {
			
					if (pixmaps[STYLEBOX_PIXMAPS_CENTER_RIGHT].is_valid())
						return pixmaps[STYLEBOX_PIXMAPS_CENTER_RIGHT].get_size().x;
				} break;
				case MARGIN_TOP: {
			
					if (pixmaps[STYLEBOX_PIXMAPS_TOP_CENTER].is_valid())
						return pixmaps[STYLEBOX_PIXMAPS_TOP_CENTER].get_size().y;
				} break;
				case MARGIN_BOTTOM: {
			
					if (pixmaps[STYLEBOX_PIXMAPS_BOTTOM_CENTER].is_valid())
						return pixmaps[STYLEBOX_PIXMAPS_BOTTOM_CENTER].get_size().y;
				} break;
			}

			return 0;

		} break;
		case MODE_USERDRAW: {
			
			if (userdraw.get_margin_func)
				return userdraw.get_margin_func( p_margin , userdraw.data );
				
			return 0;
		} break;
	
	}

	return 0;
}

Size StyleBox::get_minimum_size() const {

	Size minsize;
	minsize.x=get_margin(MARGIN_LEFT) + get_margin(MARGIN_RIGHT);
	minsize.y=get_margin(MARGIN_TOP) + get_margin(MARGIN_BOTTOM);
	
	return minsize;
}
Rect StyleBox::enter_rect(const Rect& p_rect) const {

	int margins[4];
	margins[MARGIN_LEFT] = get_margin(MARGIN_LEFT);
	margins[MARGIN_RIGHT] = get_margin(MARGIN_RIGHT);
	margins[MARGIN_TOP] = get_margin(MARGIN_TOP);
	margins[MARGIN_BOTTOM] = get_margin(MARGIN_BOTTOM);
	
	Rect rect=p_rect;
	rect.pos+=Point( margins[MARGIN_LEFT] , margins[MARGIN_TOP] );
	rect.size-=Point( margins[MARGIN_LEFT] , margins[MARGIN_TOP] )+Point( margins[MARGIN_RIGHT] , margins[MARGIN_BOTTOM] );
	
	return rect;
	
}


StyleBox::StyleBox() {

	clear();
}


StyleBox::~StyleBox()
{
}

#endif
}
