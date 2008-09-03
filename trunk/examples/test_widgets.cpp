
#include "drivers/x11/platform_x11.h"
#include "base/constants.h"
#include <stdio.h>


void print_error_test(const char  *p_file,int p_line,const char*p_err) {

	printf("%s:%i - %s\n",p_file,p_line,p_err);
}


int main(int argc, char *argv[]) {

	_GUI_print_error_func=print_error_test;
	GUI::PlatformX11 platform_x11;

	GUI::PlatformWindow *w = platform_x11.create_window();
	printf("w is %x\n",w);
	return platform_x11.run();

}
