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
#include <stdlib.h>
#include "drivers/x11/platform_x11.h"
#include <stdio.h>

namespace GUI {


Error PixmapX11::create(const Size& p_size,PixmapFormat p_format) {


	switch (p_format) {

		case PIXMAP_FORMAT_GRAYSCALE: {
			// no grayscale in xrender
			xrender_format = XRenderFindStandardFormat(x11_display,PictStandardRGB24);
		} break;
		case PIXMAP_FORMAT_ALPHA_MASK: {			
			xrender_format = XRenderFindStandardFormat(x11_display,PictStandardA8);
		} break;
		case PIXMAP_FORMAT_RGB: {
			
			xrender_format = XRenderFindStandardFormat(x11_display,PictStandardRGB24);

		} break;
		case PIXMAP_FORMAT_RGBA: {

			xrender_format = XRenderFindStandardFormat(x11_display,PictStandardARGB32);
		} break;
	};

	if (!xrender_format)
		return ERR_UNSUPPORTED;

	// create the pixmap and use it as "memory"
	printf("default screen is %i\n",DefaultScreen(x11_display));
	printf("default root is %i\n",DefaultRootWindow(x11_display));
		
	x11_pixmap = XCreatePixmap(x11_display, DefaultRootWindow(x11_display) , p_size.width, p_size.height, xrender_format->depth);
	
	if (x11_pixmap==0)
		return ERR_UNSUPPORTED; // :(

	format=p_format;
	size=p_size;

	// create the picture

	XRenderPictureAttributes att;

	att.dither=10;
	att.repeat=0;
	att.component_alpha=(p_format==PIXMAP_FORMAT_ALPHA_MASK || p_format==PIXMAP_FORMAT_RGBA)?1:0;

	xrender_picture = XRenderCreatePicture(x11_display, x11_pixmap, xrender_format, CPRepeat | CPComponentAlpha, &att);
	
	return OK;
}

Error PixmapX11::load_file(const String& p_file) {


	return OK;
}

Error PixmapX11::save_file(const String& p_file) {

	return OK;

}

void PixmapX11::load_pixels( const void * p_pixels, const Size& p_size, PixmapFormat p_format ) {

	if (x11_pixmap!=0) {
		XFreePixmap(x11_display, x11_pixmap);
		XRenderFreePicture(x11_display, xrender_picture);
	}

	x11_pixmap=0;
	xrender_picture=0;
	
	if (p_size.width<=0 || p_size.height<=0)
		return;

	create(p_size,p_format);
	if ( x11_pixmap == 0 )
		return; // unconfigured

	
	XImage *img;   

	// I imagine this could be optimized with XSHM if available)

  
	img = XCreateImage(x11_display, DefaultVisual( x11_display, 0 ), xrender_format->depth, ZPixmap, 0, NULL, size.width, size.height, 32, 0);
	img->data = (char*)malloc(img->bytes_per_line * img->height);

	Color aux_color; 
	const unsigned char *src_data=(const unsigned char*)p_pixels;
	
	for (int y = 0; y < size.height; y++) {

		for (int x = 0; x < size.width; x++) {

			int src_ofs=y*size.width+x;
			unsigned long new_pixel=0;
		
			// source convert
			switch (p_format) {
		
				case PIXMAP_FORMAT_GRAYSCALE: {
					
					aux_color.r=src_data[src_ofs];
					aux_color.g=src_data[src_ofs];
					aux_color.b=src_data[src_ofs];
					aux_color.a=255;
				} break;
				case PIXMAP_FORMAT_ALPHA_MASK: {			

					
					aux_color.r=255;
					aux_color.g=255;
					aux_color.b=255;
					aux_color.a=src_data[src_ofs];					
				} break;
				case PIXMAP_FORMAT_RGB: {
					
					aux_color.r=src_data[src_ofs*3+0];
					aux_color.g=src_data[src_ofs*3+1];
					aux_color.b=src_data[src_ofs*3+2];
					aux_color.a=255;
		
				} break;
				case PIXMAP_FORMAT_RGBA: {
		
					aux_color.r=src_data[src_ofs*4+0];
					aux_color.g=src_data[src_ofs*4+1];
					aux_color.b=src_data[src_ofs*4+2];
					aux_color.a=src_data[src_ofs*4+3];
				} break;
			};	

			switch (format) {
		
				case PIXMAP_FORMAT_GRAYSCALE: {
					
					new_pixel=aux_color.get_v();
				} break;
				case PIXMAP_FORMAT_ALPHA_MASK: {			

					new_pixel=aux_color.get_v();

				} break;
				case PIXMAP_FORMAT_RGB: {

					new_pixel = ((unsigned long)aux_color.r << 16) | ((unsigned long)aux_color.g << 8) | aux_color.b;
		
				} break;
				case PIXMAP_FORMAT_RGBA: {

					new_pixel = ((unsigned long)aux_color.a << 24) | ((unsigned long)aux_color.r << 16) | ((unsigned long)aux_color.g << 8) | aux_color.b;
				} break;
			}

			 XPutPixel(img, x, y,new_pixel);
		}
	}	

 	XGCValues gcv;
	GC gc;
	gc = XCreateGC(x11_display, x11_pixmap, 0, &gcv);

	XPutImage(x11_display, x11_pixmap, gc, img, 0, 0, 0, 0, size.width, size.height);

	free(img->data);
	img->data = NULL;
	XDestroyImage(img);
	XFreeGC(x11_display, gc);
}	

void PixmapX11::save_pixels( void * p_pixels) const {



}

void PixmapX11::copy_rect_to( const Rect& p_rect,PlatformPixmap * p_to_pixmap,const Point& p_dst_pos ) const {

	if (!is_valid()) {
		GUI_PRINT_WARNING("Src Pixmap isn't valid.");
		return;
	}
	
	if (!p_to_pixmap->is_valid()) {
		GUI_PRINT_WARNING("Dst Pixmap isn't valid.");
		return;	
	}
	if (p_to_pixmap->get_owner()!=get_owner()) {
		GUI_PRINT_WARNING("Dst Pixmap has different owner.");
		return;		
	}
	
	XRenderComposite (x11_display,PictOpOver,xrender_picture,0,((PixmapX11*)p_to_pixmap)->xrender_picture,p_rect.pos.x,p_rect.pos.y,0,0,p_dst_pos.x,p_dst_pos.y,p_rect.size.width,p_rect.size.height);
	
}

Size PixmapX11::get_size() const {


	return size;	
}

PixmapFormat PixmapX11::get_format() const {

	return format;
}

bool PixmapX11::is_valid() const {

	return x11_pixmap!=0;
}


PlatformPixmap *PixmapX11::copy() {

	PixmapX11 * new_pixmap = GUI_NEW( PixmapX11((PlatformX11 *)get_owner(),x11_display) );
	
	new_pixmap->create( get_size(), get_format() );
	copy_rect_to(Rect(Point(),get_size()),new_pixmap,Point());
	
	return new_pixmap;
}


PixmapX11::PixmapX11(PlatformX11 *p_platform,Display *p_x11_display) : PlatformPixmap(p_platform) {

	x11_display=p_x11_display;
	x11_pixmap=0;
	xrender_picture=0;
}


PixmapX11::~PixmapX11() {
	
	if (x11_pixmap!=0) {
		XFreePixmap(x11_display, x11_pixmap);
		XRenderFreePicture(x11_display, xrender_picture);
	}	
}


}
