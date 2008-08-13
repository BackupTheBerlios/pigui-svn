//
// C++ Implementation: label
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "label.h"
#include "base/painter.h"
#include "base/skin.h"
#include <stdio.h>
namespace GUI {


Size Label::get_minimum_size_internal() {
	
	
	Size min(constant( C_LABEL_MARGIN )*2,constant( C_LABEL_MARGIN )*2);
	
	if (!get_painter() || !line_cache) {
		return min;
	}

	min.y+=get_painter()->get_font_height( font(FONT_LABEL) )*line_cache_count;
	min.x+=line_cache_max_w;
		
	return min;
	
}


void Label::draw(const Point& p_pos,const Size& p_size,const Rect& p_exposed) {
	
	Size string_size;
	
	if (!line_cache)
		return;
	
	int font_h=get_painter()->get_font_height( font(FONT_LABEL) );
	int font_ascent=get_painter()->get_font_ascent( font(FONT_LABEL) );
	Size ofs(constant( C_LABEL_MARGIN ),constant( C_LABEL_MARGIN ));
	
	for(int i=0;i<line_cache_count;i++) {
		
		Point ofs;
		ofs.y=i*font_h+constant( C_LABEL_MARGIN )+font_ascent;
		
		switch( align ) {
			
			case ALIGN_LEFT: {
			
				ofs.x=constant( C_LABEL_MARGIN );
			} break;
			case ALIGN_CENTER: {
			
				ofs.x=(p_size.width-line_cache[i].line_size)/2;;
			
			} break;
			case ALIGN_RIGHT: {
			
				ofs.x=p_size.width-constant( C_LABEL_MARGIN )-line_cache[i].line_size;
			} break;
			
		}
		
		get_painter()->draw_text( font(FONT_LABEL), ofs, line_cache[i].text, color(COLOR_LABEL_FONT) );
		
	}
	
	
}

void Label::set_align(Align p_align) {
	
	align=p_align;
	update();
}
Label::Align Label::get_align(){
	
	return align;
}


void Label::set_in_window() {
	
	regenerate_line_cache(); // need painter to do so
}

void Label::regenerate_line_cache() {
	
	if (!get_painter())
		return;
	
	if (line_cache)
		GUI_DELETE_ARRAY( line_cache );
	
	line_cache=0;
	if (text.size()==0)
		return;
		   
	line_cache_count=1;
	
	for (int i=0;i<text.length();i++) {
		
		if (text[i]=='\n')
			line_cache_count++;
	}
	
	line_cache = GUI_NEW_ARRAY( LineCache, line_cache_count );
	
	int from=0;
	int line=0;
	int max_w=0;
	for (int i=0;i<=text.length();i++) {
		
		if (i==text.length() || text[i]=='\n') {
			
			line_cache[line].line_end_ofs=i;
			line_cache[line].line_size=0;
			
			for (int j=from;j<i;j++) {
				
				line_cache[line].line_size+=get_painter()->get_font_char_width( font(FONT_LABEL),text[j]);
			}
			
			line_cache[line].text=text.substr(from,i-from);						
			if (line_cache[line].line_size>max_w) {
			
				max_w=line_cache[line].line_size;
			}
			from=i+1;
		}
	}
	
	line_cache_max_w=max_w;
}

void Label::set_text(const String& p_string) {
	
	if (text==p_string)
		return;
	
	text=p_string;
	regenerate_line_cache();
	check_minimum_size();
	update();
	
}
String Label::get_text() {
	
	return text;
}

String Label::get_type() {

	return "Label";
}

Label::Label(String p_text,Align p_align)
{
	line_cache=0;
	align=p_align;
	text=p_text;
	set_fill_vertical(false);
	
}


Label::~Label() {
	
	if (line_cache)
		GUI_DELETE_ARRAY( line_cache );
	
}


}
