//
// C++ Implementation: file
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "file.h"

namespace GUI {


File* (*File::create_func)()=0;

File *File::create(){

	if ( !create_func)
		return 0;

	return create_func();
}


/* these are all implemented for ease of porting, then can later be optimized */

uint16_t File::get_16() {
	
	uint16_t res;
	uint8_t a,b;
	
	a=get_8();
	b=get_8();
	
	if (endian_swap) {
		uint8_t aux=a;
		a=b;
		b=aux;
	}
	
	res=b;
	res<<=8;
	res|=a;
	
	return res;
}
uint32_t File::get_32() {
	
	uint32_t res;
	uint16_t a,b;
	
	a=get_16();
	b=get_16();
	
	if (endian_swap) {
		
		uint16_t aux=a;
		a=b;
		b=aux;
	}
	
	res=b;
	res<<=16;
	res|=a;
	
	return res;	
}
uint64_t File::get_64() {
	
	uint64_t res;
	uint32_t a,b;
	
	a=get_32();
	b=get_32();
	
	if (endian_swap) {
		
		uint32_t aux=a;
		a=b;
		b=aux;
	}
	
	res=b;
	res<<=32;
	res|=a;
	
	return res;	
	
}

void File::get_buffer(uint8_t *p_dst,int p_length) {
	
	for (int i=0;i<p_length;i++)
		p_dst[i]=get_8();
	
}

void File::store_16(uint16_t p_dest) {
	
	uint8_t a,b;
	
	a=p_dest&0xFF;
	b=p_dest>>8;
	
	if (endian_swap) {
		
		uint8_t aux=a;
		a=b;
		b=aux;
	}
		
	store_8(a);
	store_8(b);

}
void File::store_32(uint32_t p_dest) {
	
	
	uint16_t a,b;
	
	a=p_dest&0xFFFF;
	b=p_dest>>16;
	
	if (endian_swap) {
		
		uint16_t aux=a;
		a=b;
		b=aux;
	}
		
	store_16(a);
	store_16(b);
	
}
void File::store_64(uint64_t p_dest) {
	
	uint32_t a,b;
	
	a=p_dest&0xFFFFFFFF;
	b=p_dest>>32;
	
	if (endian_swap) {
		
		uint32_t aux=a;
		a=b;
		b=aux;
	}
		
	store_32(a);
	store_32(b);
	
}
void File::store_buffer(const uint8_t *p_src,int p_length) {
	
	for (int i=0;i<p_length;i++)
		store_8(p_src[i]);
}







}
