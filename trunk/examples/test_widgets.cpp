
#include "drivers/x11/platform_x11.h"
#include "base/constants.h"
#include <stdio.h>


void print_error_test(const char  *p_file,int p_line,const char*p_err) {

	printf("%s:%i - %s\n",p_file,p_line,p_err);
}


class PWTEST : public GUI::SignalTarget {

	GUI::PlatformWindow *w;

	void update( const GUI::Rect& p_rect ) {

		printf("exposed %i,%i - %i,%i\n",p_rect.pos.x,p_rect.pos.y,p_rect.size.x,p_rect.size.y);
		w->draw_rect( GUI::Point(10,10),GUI::Size(20,20),GUI::Color(255,0,255) );
		w->draw_rect( GUI::Point(20,20),GUI::Size(40,5),GUI::Color(100,200,255) );
		w->draw_line( GUI::Point(5,5),GUI::Size(15,73),GUI::Color(255,200,80), 5 );
	}	
public:

	PWTEST() { 

		
		w=GUI::Platform::get_singleton()->create_window(); 
		w->update_event_signal.connect( this, &PWTEST::update );
	}

};


int main(int argc, char *argv[]) {

	_GUI_print_error_func=print_error_test;
	GUI::PlatformX11 platform_x11;

	PWTEST pwtest;

	return platform_x11.run();

}
