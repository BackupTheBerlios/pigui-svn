//
// C++ Implementation: stylebox
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "style_box.h"

namespace GUI {

StyleBox::StyleBox()
{
	
	for (int i=0;i<POS_MAX;i++) 
		bitmaps[i]=-1; 
	
	for (int i=0;i<4;i++)
		margins[i]=-1; //dont use forced margin
	
	mode=MODE_NONE;
	flat.margin=1;
	flat.blend=false;
	draw_center=true;
}

StyleBox::StyleBox(int p_margin,Color p_upleft,Color p_downright) {
	
	for (int i=0;i<POS_MAX;i++) 
		bitmaps[i]=-1; 
	
	mode=MODE_FLAT;
	flat.blend=false;
	flat.margin=p_margin;
	flat.border_upleft=p_upleft;
	flat.border_downright=p_downright;
	draw_center=false;
	
	for (int i=0;i<4;i++)
		margins[i]=p_margin;
	
}

StyleBox::StyleBox(int p_margin,Color p_center,Color p_upleft,Color p_downright,bool p_blend) {
	
	for (int i=0;i<POS_MAX;i++) 
		bitmaps[i]=-1; 
	
	mode=MODE_FLAT;
	flat.blend=p_blend;
	flat.margin=p_margin;
	flat.center=p_center;
	flat.border_upleft=p_upleft;
	flat.border_downright=p_downright;
	draw_center=true;
	
	for (int i=0;i<4;i++)
		margins[i]=p_margin;
	
}


}
