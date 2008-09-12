//
// C++ Interface: geometry
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef GUIGEOMETRY_H
#define GUIGEOMETRY_H


#include "base/constants.h"

namespace GUI {

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/

struct Point { //used for point and size
	
	union {
	
		int x,width;
	};
	union {
	
		int y,height;
	};
	
	bool operator==(const Point& p_point) const { return x==p_point.x && y==p_point.y; }
	bool operator!=(const Point& p_point) const { return x!=p_point.x || y!=p_point.y; }
	
	Point operator+=(const Point& p_point) { x+=p_point.x; y+=p_point.y; return *this; }
	Point operator+(const Point& p_point) const { Point aux=*this; aux+=p_point; return aux; }
	
	Point operator-=(const Point& p_point) { x-=p_point.x; y-=p_point.y; return *this; }
	Point operator-(const Point& p_point) const { Point aux=*this; aux-=p_point; return aux; }
	
	Point operator=(const Point &p_from) { x=p_from.x; y=p_from.y; return *this; }
	
	Point operator*=(int p_v) { x*=p_v; y*=p_v; return *this; }
	Point operator*(int p_v) const { Point aux=*this; aux*=p_v; return aux; }
	
	Point operator/=(int p_v) { x/=p_v; y/=p_v; return *this; }
	Point operator/(int p_v) const { Point aux=*this; aux/=p_v; return aux; }
		
	void swap_xy() { int aux=x; x=y; y=aux; }
	
	Point(const Point &p_from) { x=p_from.x; y=p_from.y; }
	Point(int p_x,int p_y) { x=p_x; y=p_y; }
	inline Point() : x(0), y(0) {}
};

typedef Point Size;

struct Rect {
	
	Point pos;
	Size size;
	
	inline bool intersects_with(const Rect& p_rect) const { 
		if ( pos.x > (p_rect.pos.x + p_rect.size.width) ) 
			return false;
		if ( (pos.x+size.width) < p_rect.pos.x  ) 
			return false;
		if ( pos.y > (p_rect.pos.y + p_rect.size.height) ) 
			return false;
		if ( (pos.y+size.height) < p_rect.pos.y  ) 
			return false;
	
		return true;
	}

	inline bool contains(const Rect& p_rect) const {

		return 	(p_rect.pos.x>=pos.x) && (p_rect.pos.y>=pos.y) &&
				((p_rect.pos.x+p_rect.size.x)<=(pos.x+size.x)) &&
				((p_rect.pos.y+p_rect.size.y)<=(pos.y+size.y));

	}
	
	inline bool is_empty() const {

		return (size.x<=0 || size.y<=0);

	}
	inline Rect clip(const Rect& p_rect) const { /// return a clipped rect
		
		Rect new_rect=p_rect;
		
		if (!intersects_with( new_rect ))
			return Rect();
		
		new_rect.pos.x = maxval( p_rect.pos.x , pos.x );
		new_rect.pos.y = maxval( p_rect.pos.y , pos.y );
		
		Point p_rect_end=p_rect.pos+p_rect.size;
		Point end=pos+size;
		
		new_rect.size.x=minval(p_rect_end.x,end.x) - new_rect.pos.x;
		new_rect.size.y=minval(p_rect_end.y,end.y) - new_rect.pos.y;
	
		return new_rect;
	}
	
	inline Rect merge(const Rect& p_rect) const { ///< return a merged rect
		
		Rect new_rect;
		
		new_rect.pos.x=minval( p_rect.pos.x , pos.x );
		new_rect.pos.y=minval( p_rect.pos.y , pos.y );
		
		
		new_rect.size.x = maxval( p_rect.pos.x+p_rect.size.x , pos.x+size.x );
		new_rect.size.y = maxval( p_rect.pos.y+p_rect.size.y , pos.y+size.y );
		
		new_rect.size = new_rect.size - new_rect.pos; //make relative again
		
		return new_rect;
	};
	bool has_point(const Point& p_point) { 
		if (p_point.x < pos.x)
			return false; 
		if (p_point.y < pos.y)
			return false; 
	
		if (p_point.x >= (pos.x+size.x) )
			return false; 
		if (p_point.y >= (pos.y+size.y) )
			return false; 
		
		return true;
	}
	
	bool operator==(const Rect& p_rect) const {
	
		return pos==p_rect.pos && size==p_rect.size; 
	}
	
	bool operator!=(const Rect& p_rect) const {
	
		return pos!=p_rect.pos || size!=p_rect.size; 
	}

	inline Rect() {}
	Rect( const Point& p_pos, const Size& p_size ) { pos=p_pos; size=p_size; }
	Rect( int p_x,int p_y, int p_width,int p_height ) { pos.x=p_x; pos.y=p_y; size.width=p_width; size.height=p_height;  }
};

struct Color {		
		
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
	
	unsigned char get_v() { return ((int)r+(int)g+(int)b)/3; }
	Color( unsigned char p_mono=0 ) { r=p_mono; g=p_mono; b=p_mono; a=255; }
	Color( unsigned char p_r, unsigned char p_g, unsigned char p_b,unsigned char p_a=255) { r=p_r; g=p_g; b=p_b; a=p_a; }
	
};

}

#endif
