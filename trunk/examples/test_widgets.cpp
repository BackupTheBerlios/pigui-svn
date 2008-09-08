
#include "drivers/x11/platform_x11.h"
#include "base/constants.h"
#include <stdio.h>


void print_error_test(const char  *p_file,int p_line,const char*p_err) {

	printf("%s:%i - %s\n",p_file,p_line,p_err);
}


class PWTEST : public GUI::SignalTarget {

	GUI::PlatformWindow *w;
	GUI::Pixmap pixmap;
	GUI::Pixmap pixmap_copy;
	GUI::Font font;
	
	void update( const GUI::Rect& p_rect ) {

		printf("exposed %i,%i - %i,%i\n",p_rect.pos.x,p_rect.pos.y,p_rect.size.x,p_rect.size.y);
		w->draw_rect( GUI::Point(10,10),GUI::Size(20,20),GUI::Color(255,0,255) );
		w->draw_rect( GUI::Point(20,20),GUI::Size(40,5),GUI::Color(100,200,255) );
		w->draw_line( GUI::Point(5,5),GUI::Size(15,73),GUI::Color(255,200,80), 5 );
		
		GUI::Point poly[4]={ GUI::Point(50,50), GUI::Point(70,50), GUI::Point(75,80),GUI::Point(43,77) };
		
		w->draw_polygon(poly,4,GUI::Color(180,170,120),false,3);
		
		w->draw_pixmap( pixmap, GUI::Point( 60,20 ), GUI::Rect(0,0,32,32) );
		w->draw_pixmap( pixmap_copy, GUI::Point( 170,220 ), GUI::Rect(0,0,32,32) );
		
		
		w->draw_string( font, GUI::Point(10,180),GUI::Color(100,150,250),"TEXT RENDERING!");
	}	
public:

	PWTEST() { 

		unsigned char * pm1mem = new unsigned char[32*32*3];
		for (int x=0;x<32;x++) {

			for (int y=0;y<32;y++) {
		
				pm1mem[(y*32+x)*3+0]=x*256/32;
				pm1mem[(y*32+x)*3+1]=y*256/32;
				pm1mem[(y*32+x)*3+2]=(x+y)*256/64;
			}
		}
				
		pixmap.load_pixels(pm1mem,GUI::Size(32,32),GUI::PIXMAP_FORMAT_RGB);
		
		pixmap_copy=pixmap;
		pixmap.copy_rect_to(GUI::Rect(0,0,16,16),pixmap_copy,GUI::Point(16,16));
		
		w=GUI::Platform::get_singleton()->create_window(); 
		w->update_event_signal.connect( this, &PWTEST::update );
		if (!font.system_load("FreeSans",12,GUI::FONT_STYLE_BOLD)) {
			
			printf("Success loading font\n");
		}
	}

};


int main(int argc, char *argv[]) {

	_GUI_print_error_func=print_error_test;
	GUI::PlatformX11 platform_x11;

	PWTEST pwtest;

	return platform_x11.run();

}
