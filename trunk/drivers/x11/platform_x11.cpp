//
// C++ Implementation: platform_x11
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "platform_x11.h"

#ifdef PIGUI_X11_ENABLED

#include <unistd.h>
#include <stdio.h>
#include <X11/Xlocale.h>
namespace GUI {


PlatformTheme *PlatformX11::get_theme() const {

	return NULL;
}
PlatformPointer *PlatformX11::get_pointer() const {

	return NULL;
}

PlatformWindow* PlatformX11::create_window(unsigned int p_flags) {
 
	int black = BlackPixel(x11_display, DefaultScreen(x11_display));
	Window w = XCreateSimpleWindow(x11_display, DefaultRootWindow(x11_display), 0, 0, 
				     350, 350, 0, black, black);
	printf("window ID is %i\n",w);
	if (w==0) {

		GUI_PRINT_ERROR("XCreateSimpleWindow failed.")
		return NULL;
	}

	window_list = GUI_NEW( WindowX11( this, x11_display, w, window_list ) );

	WindowX11 * new_window = window_list;

	return new_window;
}

PlatformPixmap* PlatformX11::create_pixmap() {

	return GUI_NEW( PixmapX11(this,x11_display) );
}
PlatformFont* PlatformX11::create_font() {

	return GUI_NEW( FontX11( this, x11_display ) );
}
PlatformFile *PlatformX11::create_file() {

	return NULL;
}
PlatformFileSystem *PlatformX11::create_file_system() {

	return NULL;
}
PlatformTimer *PlatformX11::create_timer() {

	return NULL;
}


int PlatformX11::run() {

	
	while(!exit) {

		iteration();
	}

	return 0;
}

void PlatformX11::iteration() {

	XFlush(x11_display);

	XEvent event;
	XNextEvent(x11_display, &event); // wait for event

	// dispatch the event to whoever owns it (TODO must optimize this)
	WindowX11 *w = window_list;
	
	while(w) {

		if (w->get_x11_window() == event.xany.window) {

			w->process_x11_event(event);
			break;
		}
		w=w->get_next();
	}

	XFlush(x11_display);
}

static int _cmpstringp(const void *p1, const void *p2) {
	
	return strcmp(* (char * const *) p1, * (char * const *) p2);
}


void PlatformX11::generate_font_list() {
	
	font_info = NULL;
	FcFontSet  *font_set;
	FcPattern   *font_pattern;
	FcObjectSet *font_obj_set = 0; 
	
	font_pattern = FcPatternCreate();
	font_obj_set = FcObjectSetBuild(FC_FAMILY, FC_STYLE, (void *)0);
	font_set = FcFontList(0, font_pattern, font_obj_set);
	
	FcPatternDestroy(font_pattern);
	
	if (font_set) {
		
		
		char **font_list = (char**)malloc( sizeof(char*)*font_set->nfont );
		
		for(int i=0;i<font_set->nfont;i++) {
			
			font_list[i]=(char*)FcNameUnparse(font_set->fonts[i]);
		}
		
		qsort(font_list, font_set->nfont, sizeof(char*), _cmpstringp);
		
		FontInfoListX11 *fonts=NULL;
		
		font_info_count=0;
		
		for(int i=0;i<font_set->nfont;i++) {
			
			String fontstring;
			fontstring.parse_utf8(font_list[i]);
								
			String name = fontstring.get_slice(":",0);
			if (name.find(",")!=-1)
				name=name.get_slice(",",0);
			
			FontInfoListX11 * font=0;;
			
			if (fonts && fonts->name==name) {
				
				font=fonts;
			} else {
				
				font = new FontInfoListX11;
				font->next=fonts;
				fonts=font;
				font->name=name;
				font_info_count++;
//				printf("font: %s\n",fontstring.ascii().get_data());				
			}
			
			String styles=fontstring.get_slice(":",1);
			// could really optimize this
			for (int j=0;j<styles.get_slice_count(",");j++) {
				
				String style=styles.get_slice(",",j);
				
				if (style.find("Italic")!=-1 || style.find("Oblique")!=-1) {
					
					font->flags|=FONT_STYLE_ITALIC;
				}
				if (style.find("Bold")) {
					
					font->flags|=FONT_STYLE_BOLD;
				}				
			}
		}
		
		// clean up temporary allocated font space
		for(int i=0;i<font_set->nfont;i++) {
			
			free(font_list[i]);
		}
		
		free(font_list);
		
		font_info = new FontInfoListX11*[font_info_count];
		
		
		for (int i=font_info_count-1;i>=0;i--) {
			
			font_info[i]=fonts;
			fonts=fonts->next;
		}
		
	}
	
	FcFontSetDestroy(font_set);	
	
}

PlatformX11::PlatformX11() {

	if (setlocale(LC_ALL,"") == NULL) {
		fprintf(stderr, "warning: could not set default locale\n");
	}	
	
	exit=false;
	x11_display = XOpenDisplay(NULL);
	if (!x11_display) {
	
		GUI_PRINT_ERROR("Unable to open X11 x11_display.");
		_exit(255);
	}
	/* we're testing the default input method */
	
	char * modifiers = XSetLocaleModifiers ("@im=none");
	if (modifiers == NULL) {
		fprintf (stderr, "XSetLocaleModifiers failed\n");
		_exit(255);
	}
	
	xim = XOpenIM (x11_display, NULL, NULL, NULL);
	if (xim == NULL) {
        	fprintf (stderr, "XOpenIM failed\n");
//        	_exit(255);
		xim_style=NULL;
    	} else {
		::XIMStyles *xim_styles;
		xim_style=0;
		char *imvalret;	
		imvalret = XGetIMValues(xim, XNQueryInputStyle, &xim_styles, NULL);
		if (imvalret != NULL || xim_styles == NULL) {
			fprintf (stderr, "Input method doesn't support any styles\n");
		}
		
		if (xim_styles) {
			xim_style = 0;
            		for (int i=0;i<xim_styles->count_styles;i++) {
                	
				if (xim_styles->supported_styles[i] ==
					(XIMPreeditNothing | XIMStatusNothing)) {
				
					xim_style = xim_styles->supported_styles[i];
					break;
				}
			}
			
			XFree (xim_styles);
                }
    	}
    	
    	
	XftInitFtLibrary();
	
		::XIM xim;
	::XIMStyles *xim_styles;
	::XIMStyle xim_style;

	
	best_pixmap_depth = DefaultDepth( x11_display, DefaultScreen(x11_display) );

	generate_font_list();
	
}


PlatformX11::~PlatformX11() {

	if (font_info) {
		for (int i=0;i<font_info_count;i++)
			delete font_info[i];
		
		delete[] font_info;
	}
		
		
}

}

#endif
