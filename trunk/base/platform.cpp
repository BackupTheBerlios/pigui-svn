//
// C++ Implementation: platform
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "platform.h"

namespace GUI {

PlatformTheme::~PlatformTheme() {

}

PlatformWindow::~PlatformWindow() {

}

PlatformPixmap::~PlatformPixmap() {

}

PlatformFont::~PlatformFont() {

}


unsigned short PlatformFile::get_16() const {

	unsigned short res;
	unsigned char a,b;
	
	a=get_8();
	b=get_8();
	
	if (get_flags()&FILE_BIG_ENDIAN) {
		unsigned char aux=a;
		a=b;
		b=aux;
	}
	
	res=b;
	res<<=8;
	res|=a;
	
	return res;
}
unsigned int PlatformFile::get_32() const {

	unsigned int res;
	unsigned short a,b;
	
	a=get_16();
	b=get_16();
	
	if (get_flags()&FILE_BIG_ENDIAN) {
		
		unsigned short aux=a;
		a=b;
		b=aux;
	}
	
	res=b;
	res<<=16;
	res|=a;
	
	return res;	
}
gui_u64 PlatformFile::get_64() const {

	gui_u64 res;
	unsigned int a,b;
	
	a=get_32();
	b=get_32();
	
	if (get_flags()&FILE_BIG_ENDIAN) {
		
		unsigned int aux=a;
		a=b;
		b=aux;
	}
	
	res=b;
	res<<=32;
	res|=a;
	
	return res;
}

void PlatformFile::get_buffer(unsigned char *p_dst,int p_bytes) const {

	for (int i=0;i<p_bytes;i++)
		p_dst[i]=get_8();
}



void PlatformFile::store_16(unsigned short p_dest) {

	unsigned char a,b;
	
	a=p_dest&0xFF;
	b=p_dest>>8;
	
	if (get_flags()&FILE_BIG_ENDIAN) {
		
		unsigned char aux=a;
		a=b;
		b=aux;
	}
		
	store_8(a);
	store_8(b);
}
void PlatformFile::store_32(unsigned int p_dest) {

	unsigned short a,b;
	
	a=p_dest&0xFFFF;
	b=p_dest>>16;
	
	if (get_flags()&FILE_BIG_ENDIAN) {
		
		unsigned short aux=a;
		a=b;
		b=aux;
	}
		
	store_16(a);
	store_16(b);
	
}
void PlatformFile::store_64(gui_u64 p_dest) {

	unsigned int a,b;
	
	a=p_dest&0xFFFFFFFF;
	b=p_dest>>32;
	
	if (get_flags()&FILE_BIG_ENDIAN) {
		
		unsigned int aux=a;
		a=b;
		b=aux;
	}
		
	store_32(a);
	store_32(b);
}

void PlatformFile::store_buffer(const unsigned char *p_src,int p_bytes) {

	for (int i=0;i<p_bytes;i++)
		store_8(p_src[i]);
}

PlatformFile::~PlatformFile() {

}

String PlatformFileSystem::get_absolute_path(String p_path) const {

	if (p_path.length()!=0 && get_dir_separator()==p_path[0])
		return p_path;

	return get_current_dir() + get_dir_separator() + p_path;
}

PlatformFileSystem::~PlatformFileSystem() {

}

PlatformTimer::~PlatformTimer() {


}

Platform::Platform()
{
}


Platform::~Platform()
{
}


}